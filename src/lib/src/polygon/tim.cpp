// src/lib/src/polygon/tim.cpp
// Original: C:\lib\src\polygon\tim.cpp
// Contents: TIM image loading: reads and validates TIM headers, CLUT and pixel data, computes
//   texture sizes, unpacks and repacks 4-bit pixels, and builds and frees textures from them.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
// Also: Builds texture tile sets from TIM images for polygon models and writes TIM records back
//   to file.
#include "lib/api/file.h"
#include "lib/api/graphics.h"
#include "lib/types/graphics.h"
#include "lib/api/polygon.h"
#include "lib/types/file.h"
#include "lib/types/polygon.h"
#include "lib/codegen.h"
#include <string.h>
#include <stdio.h>
#include "lib/api/mem.h"
#include "lib/enums/graphics.h"
#include "lib/enums/file.h"
extern "C" {

/*
 * Computes texture width and height from TIM header, rounding to powers of two.
 */
// FUNCTION: FF7 0x00696d00
void __cdecl Pfile_ComputeTimTextureSize(PolygonTim *texture)
{
    int pow2Height;
    int width;
    int paddedW;
    PolygonTimVramRect *record;
    int height;

    if (texture != 0) {
        record = &texture->image->rect;
        width = record->width;
        height = record->height;
        texture->texX = 0;
        texture->texY = 0;
        texture->texPage = 0;
        /* Numeric colour depth in bits per pixel. */
        switch (texture->bitsPerPixel) {
        case GFX_BPP_4:
            width *= 4;
            texture->texX = record->x % 0x40;
            texture->texX *= 4;
            texture->texY = record->y % 0x100;
            texture->texPage = record->x / 0x40;
            texture->texPage += record->y / 0x100 * 16;
            break;
        case GFX_BPP_8:
            width *= 2;
            texture->texX = record->x % 0x80;
            texture->texX *= 2;
            texture->texY = record->y % 0x100;
            texture->texPage = record->x / 0x80;
            texture->texPage += record->y / 0x100 * 8;
            break;
        case GFX_BPP_16:
            width *= 1;
            texture->texX = record->x % 0x100;
            texture->texX *= 1;
            texture->texY = record->y % 0x100;
            texture->texPage = record->x / 0x100;
            texture->texPage += record->y / 0x100 * 4;
            break;
        case GFX_BPP_24:
            width = width * 2 / 3;
            break;
        }
        texture->texX = 0;
        texture->texY = 0;
        paddedW = width;
        if (IsPowerOfTwo(width) == 0) {
            paddedW = RoundUpToPowerOfTwo(width);
        }
        pow2Height = height;
        if (IsPowerOfTwo(height) == 0) {
            pow2Height = RoundUpToPowerOfTwo(height);
        }
        texture->paddedWidth = paddedW;
        texture->paddedHeight = pow2Height;
    }
}

#pragma intrinsic(memset, memcpy)

// clang-format off
// GLOBAL: FF7 0x00db2ce0
int g_TimClutIndex4Bit = 0;
// GLOBAL: FF7 0x00db2ce4
int g_TimClutIndex8Bit = 0;
// clang-format on

/*
 * Frees pixel blocks, CLUT data, and headers associated with a loaded TIM.
 */
// FUNCTION: FF7 0x00696fba
void __cdecl Pfile_DestroyTim(PolygonTim *timTexture)
{
    if (timTexture != 0) {
        if (timTexture->image != 0) {
            // STRING: FF7 0x009116dc
            GameHeapFree((void *)timTexture->image->data, "C:\\lib\\src\\polygon\\tim.cpp", 0x5E);
            // STRING: FF7 0x009116f8
            GameHeapFree(timTexture->image, "C:\\lib\\src\\polygon\\tim.cpp", 0x5F);
        }
        if (timTexture->clut != 0) {
            // STRING: FF7 0x00911714
            GameHeapFree((void *)timTexture->clut->data, "C:\\lib\\src\\polygon\\tim.cpp", 0x64);
            // STRING: FF7 0x00911730
            GameHeapFree(timTexture->clut, "C:\\lib\\src\\polygon\\tim.cpp", 0x65);
        }
        // STRING: FF7 0x0091174c
        GameHeapFree(timTexture->header, "C:\\lib\\src\\polygon\\tim.cpp", 0x68);
        // STRING: FF7 0x00911768
        GameHeapFree(timTexture, "C:\\lib\\src\\polygon\\tim.cpp", 0x69);
    }
}

/*
 * Reads and validates a TIM header, optional CLUT, and pixel data from context.
 */
// FUNCTION: FF7 0x00697061
int __cdecl Pfile_LoadTimFromContext(FileOpenRequest *fileContext, const char *fileName)
{
    PolygonTimHeader *hdrBlock;
    int errorCode;
    PolygonTim *pResult;
    char message[200];
    struct FileHandle *handle;
    int clutBytes;
    int imageBytes;

    errorCode = 1;
    // STRING: FF7 0x00911784
    pResult = (PolygonTim *)GameHeapAlloc(1, 0x44, "C:\\lib\\src\\polygon\\tim.cpp", 0x77);
    if (pResult != 0) {
        // STRING: FF7 0x009117a0
        pResult->header = (PolygonTimHeader *)GameHeapAlloc(1, 8, "C:\\lib\\src\\polygon\\tim.cpp", 0x7A);
        hdrBlock = pResult->header;
        if (hdrBlock != 0) {
            handle = OpenFileEx(fileContext, fileName);
            if (handle != 0) {
                if (ReadFileData(8, pResult->header, handle) != 0) {
                    if ((hdrBlock->id & 0xFF) == TIM_FILE_ID) {
                        if ((hdrBlock->id & 0xFF00) == 0) {
                            if ((hdrBlock->flags & TIM_FLAG_HAS_CLUT) != 0) {
                                // STRING: FF7 0x009117bc
                                pResult->clut = (PolygonTimBlock *)GameHeapAlloc(1, 0x10, "C:\\lib\\src\\polygon\\tim.cpp", 0x8D);
                                if (pResult->clut != 0) {
                                    if (ReadFileData(4, pResult->clut, handle) != 0) {
                                        if (ReadFileData(8, &pResult->clut->rect, handle) != 0) {
                                            clutBytes = pResult->clut->size - 12;
                                            if (clutBytes > 0) {
                                                pResult->clut->data = ReadFileIntoNewBuffer(clutBytes, 1, handle);
                                                if (pResult->clut->data != 0) {
                                                    switch (pResult->header->flags & TIM_FLAG_PIXEL_MODE_MASK) {
                                                    case TIM_PIXEL_MODE_4BPP:
                                                        pResult->clutCount = clutBytes / 32;
                                                        break;
                                                    case TIM_PIXEL_MODE_8BPP:
                                                        pResult->clutCount = clutBytes / 512;
                                                    }
                                                    errorCode = 0;
                                                }
                                            }
                                        }
                                    }
                                }
                            } else {
                                errorCode = 0;
                            }
                            if (errorCode == 0) {
                                errorCode = 1;
                                // STRING: FF7 0x009117d8
                                pResult->image = (PolygonTimBlock *)GameHeapAlloc(1, 0x10, "C:\\lib\\src\\polygon\\tim.cpp", 0xB7);
                                if (pResult->image != 0) {
                                    if (ReadFileData(4, pResult->image, handle) != 0) {
                                        if (ReadFileData(8, &pResult->image->rect, handle) != 0) {
                                            imageBytes = pResult->image->size - 12;
                                            if (imageBytes > 0) {
                                                pResult->image->data = ReadFileIntoNewBuffer(imageBytes, 1, handle);
                                                if (pResult->image->data != 0) {
                                                    switch (pResult->header->flags & TIM_FLAG_PIXEL_MODE_MASK) {
                                                    case TIM_PIXEL_MODE_4BPP:
                                                        pResult->bitsPerPixel = 4;
                                                        pResult->width = pResult->image->rect.width << 2;
                                                        pResult->height = pResult->image->rect.height;
                                                        break;
                                                    case TIM_PIXEL_MODE_8BPP:
                                                        pResult->bitsPerPixel = 8;
                                                        pResult->width = pResult->image->rect.width << 1;
                                                        pResult->height = pResult->image->rect.height;
                                                        break;
                                                    case TIM_PIXEL_MODE_16BPP:
                                                        pResult->bitsPerPixel = 16;
                                                        pResult->width = pResult->image->rect.width;
                                                        pResult->height = pResult->image->rect.height;
                                                        break;
                                                    case TIM_PIXEL_MODE_24BPP:
                                                        pResult->bitsPerPixel = 24;
                                                        pResult->width = (pResult->image->rect.width << 1) / 3;
                                                        pResult->height = pResult->image->rect.height;
                                                    }
                                                    Pfile_ComputeTimTextureSize(pResult);
                                                    errorCode = 0;
                                                }
                                            } else {
                                                errorCode = 0;
                                            }
                                        }
                                    }
                                }
                            }
                        } else {
                            // STRING: FF7 0x009117f4
                            sprintf(message, "ERROR: INCORRECT TIM VERSION FOR FILE %s \n", fileName);
                            Gfx_DebugPrint(message);
                        }
                    } else {
                        // STRING: FF7 0x00911820
                        sprintf(message, "ERROR: FILE %s IS NOT A TIM FILE \n", fileName);
                        Gfx_DebugPrint(message);
                    }
                }
                CloseFileEx(handle);
            }
        }
    }
    if (errorCode != 0) {
        Pfile_DestroyTim(pResult);
        pResult = 0;
    }
    return (int)pResult;
}

/*
 * Loads the PlayStation `.TIM` image `fileName` from disk (a zeroed read request on the stack,
 * no archive) with `Pfile_LoadTimFromContext`: the 8-byte header must carry TIM_FILE_ID and
 * version 0, TIM_FLAG_HAS_CLUT adds a palette (CLUT) block, and the TimPixelMode (4, 8, 16 or 24
 * bits per pixel) gives the width in pixels. Battle's legacy party, enemy and stage DAT loaders
 * write a model's texture out as a TIM file and read it back this way. Returns the loaded
 * `PolygonTim`, or 0, as the value `Pfile_LoadTimFromContext` leaves in EAX; the body has
 * no return statement.
 */
// FUNCTION: FF7 0x00697558
void __cdecl Pfile_LoadTim(const char *fileName)
{
    int fileContext[4];

    InitFileContext(0, fileContext);
    Pfile_LoadTimFromContext((FileOpenRequest *)fileContext, fileName);
}

/*
 * Calculates pixel width from halfword width based on the TIM's TimPixelMode.
 */
// FUNCTION: FF7 0x00697580
int __cdecl Pfile_TimHalfwordWidthToPixelWidth(int mode, int halfwordWidth)
{
    int pixelWidth;

    pixelWidth = 0;
    switch (mode) {
    case TIM_PIXEL_MODE_4BPP:
        pixelWidth = halfwordWidth << 2;
        break;
    case TIM_PIXEL_MODE_8BPP:
        pixelWidth = halfwordWidth << 1;
        break;
    case TIM_PIXEL_MODE_16BPP:
        pixelWidth = halfwordWidth;
        break;
    case TIM_PIXEL_MODE_24BPP:
        pixelWidth = (halfwordWidth << 1) / 3;
    }
    return pixelWidth;
}

/*
 * Unpacks 4-bit nibbles from a 16-bit word into separate bytes.
 */
// FUNCTION: FF7 0x006975e7
int __cdecl Pfile_TimSpreadNibbles(int value)
{
    int result;

    result = value & 0xF | (value & 0xF0) << 4 | (value & 0xF00) << 8 | (value & 0xF000) << 0xC;
    return result;
}

/*
 * Packs two separated nibbles back into a single byte.
 */
// FUNCTION: FF7 0x00697625
char __cdecl Pfile_TimPackNibblePair(int value)
{
    unsigned char packed;

    packed = value & 0xFFFF & 0xF | (value & 0xFFFF & 0xF00) >> 4;
    return (char)packed;
}

/*
 * Converts TIM pixels and palette into a texture, optionally extracting a subrect.
 */
// FUNCTION: FF7 0x00697652
int __cdecl Pfile_BuildTextureFromTimRect(int rectX, int rectY, int rectWidth, int rectHeight, PolygonTim *timRecord, const char *fileName)
{
    int palStop;
    int dstRow;
    int argbOpaque;
    int curFirstPaletteStep;
    int outW;
    int semiArgb;
    PolygonTimColor *palCursor;
    int savedColor8;
    int *entAddr;
    int palLoop;
    int pixCnt;
    int buffer8;
    int lookupIndex;
    char fileText[200];
    int padBuf;
    unsigned short clutColor;
    char textB[200];
    int stepClutStart;
    int *remapTable; // Palette-index remap.
    int pixelIndex;
    short *shortBuf;
    int srcPtr;
    int rowOffset;
    int stepClutEntry;
    char warnBuffer[200];
    int bitCnt;
    int tmpClutEnd;
    int blueMask;
    int outHeight;
    char descBuf[128]; // Source pixel-format description.
    GfxPixelFormat *scratchFmt;
    int failure; // Failure flag; remains set until the pixel buffer is complete.
    int entireImage; // Nonzero selects the full TIM image.
    int tempQuadAddr;
    int nibbleIdxUnit;
    int mainPadBottom;
    int outIdx;
    int blackFlag; // Transparent black was present.
    PolygonTimBlock *pImageBlock; // TIM pixel-block header.
    int spareHeight;
    int curAvailWidth;
    GfxTexHeader *headerPtr; // Allocated texture header returned to the caller.
    int rowIdx;
    int rowWords;
    int column;
    int rawWordIdx;
    char messageD[200];
    PolygonTimColor *argbPtrPtr;
    int curRemappedSpan;
    int unitSawZero;
    int tempGreenMask;
    int imageWidth;
    GfxPixelFormat *texFormat8; // The texture pixel format inside headerPtr.
    int timHeight;
    int semiTrans;
    int *curBufEntry;
    int xPad;
    int selRedMask;
    unsigned short pixel;
    int tempAlphaMask;

    scratchFmt = (GfxPixelFormat *)&descBuf;
    memset((void *)scratchFmt, 0, 128);
    if (rectWidth > 0 && rectHeight > 0) {
        entireImage = 0;
    } else {
        entireImage = 1;
    }
    failure = 1;
    headerPtr = 0;
    if (timRecord != 0) {
        headerPtr = (GfxTexHeader *)Gfx_AllocTexRecord();
        if (headerPtr != 0) {
            remapTable = 0;
            texFormat8 = &headerPtr->format;
            pImageBlock = timRecord->image;
            bitCnt = 0x10;
            selRedMask = 0x1F;
            tempGreenMask = 0x3E0;
            blueMask = 0x7C00;
            tempAlphaMask = 0x8000;
            PixelFormatSetRgbMasks(bitCnt, selRedMask, tempGreenMask, blueMask, tempAlphaMask, scratchFmt);
            blackFlag = 0;
            semiTrans = 0;
            headerPtr->unusedFormatFlag = 0;
            headerPtr->colorKey = 0;
            headerPtr->hasSemiTransparency = 0;
            headerPtr->minColorBits = 4;
            headerPtr->maxColorBits = 8;
            headerPtr->minAlphaBits = 0;
            headerPtr->maxAlphaBits = 8;
            headerPtr->minBitsPerPixel = 8;
            headerPtr->maxBitsPerPixel = 0x20;
            headerPtr->imageX = timRecord->image->rect.x;
            headerPtr->imageY = timRecord->image->rect.y;
            switch (timRecord->header->flags & TIM_FLAG_PIXEL_MODE_MASK) {
            case TIM_PIXEL_MODE_4BPP:
            case TIM_PIXEL_MODE_8BPP:
                texFormat8->bitsPerPixel = 8;
                texFormat8->bytesPerPixel = 1;
                switch (timRecord->header->flags & TIM_FLAG_PIXEL_MODE_MASK) {
                case TIM_PIXEL_MODE_4BPP:
                    headerPtr->entriesPerPalette = 0x10;
                    headerPtr->bitsPerPixel = 4;
                    break;
                case TIM_PIXEL_MODE_8BPP:
                    headerPtr->entriesPerPalette = 0x100;
                    headerPtr->bitsPerPixel = 8;
                }
                if (headerPtr->entriesPerPalette > 0) {
                    headerPtr->paletteCount = timRecord->clutCount;
                }
                texFormat8->palettized = 1;
                texFormat8->indexBits = 8;
                texFormat8->indexedTo8 = 0;
                if (timRecord->singlePalette != 0) {
                    texFormat8->paletteEntryCount = headerPtr->entriesPerPalette;
                } else {
                    texFormat8->paletteEntryCount = headerPtr->entriesPerPalette * headerPtr->paletteCount;
                }
                texFormat8->entriesPerPalette = headerPtr->entriesPerPalette;
                // STRING: FF7 0x00911844
                texFormat8->palette = (PolygonTimColor *)GameHeapAlloc(texFormat8->paletteEntryCount, 4, "C:\\lib\\src\\polygon\\tim.cpp", 0x1D4);
                if (DEBUG_TRACE) {
                    // STRING: FF7 0x00911860
                    sprintf(textB, "%s TOTAL PALETTES %d ENTRIES PER PALETTE %d \n", fileName, headerPtr->paletteCount, headerPtr->entriesPerPalette);
                    Gfx_DebugPrint(textB);
                }
                headerPtr->clutX = timRecord->clut->rect.x;
                headerPtr->clutY = timRecord->clut->rect.y;
                if (timRecord->singlePalette != 0) {
                    stepClutStart = timRecord->paletteRow * texFormat8->entriesPerPalette;
                    tmpClutEnd = stepClutStart + texFormat8->entriesPerPalette;
                } else {
                    stepClutStart = 0;
                    tmpClutEnd = texFormat8->paletteEntryCount;
                }
                outIdx = 0;
                for (stepClutEntry = stepClutStart; stepClutEntry < tmpClutEnd; stepClutEntry++) {
                    clutColor = ((short *)timRecord->clut->data)[stepClutEntry];
                    if (clutColor == 0) {
                        texFormat8->paletteEntries[outIdx] = 0;
                        blackFlag = 1;
                    } else if ((clutColor & 0x8000) != 0) {
                        if ((clutColor & ~0x8000) != 0) {
                            semiArgb = UnpackPixelToArgb8888(clutColor, scratchFmt);
                            texFormat8->paletteEntries[outIdx] = semiArgb;
                            texFormat8->palette[outIdx].alpha = 0xFE;
                            if (texFormat8->palette[outIdx].colorR < 1 && texFormat8->palette[outIdx].colorG < 1 && texFormat8->palette[outIdx].colorB < 1) {
                                if (DEBUG_TRACE) {
                                    texFormat8->palette[outIdx].colorR = 0xFF;
                                    texFormat8->palette[outIdx].colorG = 0;
                                    texFormat8->palette[outIdx].colorB = 0;
                                    texFormat8->palette[outIdx].alpha = 0xFF;
                                }
                            } else {
                                semiTrans = 1;
                            }
                        } else {
                            texFormat8->palette[outIdx].colorR = 1;
                            texFormat8->palette[outIdx].colorG = 1;
                            texFormat8->palette[outIdx].colorB = 1;
                            texFormat8->palette[outIdx].alpha = 0xFF;
                            if (DEBUG_TRACE) {
                                texFormat8->palette[outIdx].colorR = 0xFF;
                                texFormat8->palette[outIdx].colorG = 0;
                                texFormat8->palette[outIdx].colorB = 0;
                                texFormat8->palette[outIdx].alpha = 0xFF;
                            }
                        }
                    } else {
                        argbOpaque = UnpackPixelToArgb8888(clutColor, scratchFmt);
                        texFormat8->paletteEntries[outIdx] = argbOpaque;
                        texFormat8->palette[outIdx].alpha = 0xFF;
                        if (UNFOLDED(0) && texFormat8->palette[outIdx].colorR < 1 && texFormat8->palette[outIdx].colorG < 1 && texFormat8->palette[outIdx].colorB < 1) {
                            texFormat8->paletteEntries[outIdx] = 0;
                        }
                    }
                    if (DEBUG_TRACE) {
                        texFormat8->palette[outIdx].colorR = 0xFF;
                        texFormat8->palette[outIdx].colorG = 0;
                        texFormat8->palette[outIdx].colorB = 0;
                        texFormat8->palette[outIdx].alpha = 0xFF;
                    }
                    if (DEBUG_TRACE) {
                        if (outIdx == 0) {
                            // STRING: FF7 0x00911890
                            sprintf(fileText, "TIM FILE %s \n", fileName);
                            Gfx_DebugPrint(fileText);
                        }
                        // STRING: FF7 0x009118a0
                        sprintf(fileText, "PALETTE INDEX %d RGBA %2X %2X %2X %2X \n", outIdx, texFormat8->palette[outIdx].colorR, texFormat8->palette[outIdx].colorG, texFormat8->palette[outIdx].colorB, texFormat8->palette[outIdx].alpha);
                        Gfx_DebugPrint(fileText);
                    }
                    outIdx++;
                }
            }
            if (blackFlag != 0 && texFormat8->paletteEntryCount > 0) {
                // STRING: FF7 0x009118c8
                remapTable = (int *)GameHeapAlloc(texFormat8->paletteEntryCount, 4, "C:\\lib\\src\\polygon\\tim.cpp", 0x264);
                for (outIdx = 0; outIdx < headerPtr->entriesPerPalette; outIdx++) {
                    remapTable[outIdx] = outIdx;
                }
                if (timRecord->transparentToIndex0 != 0) {
                    if (timRecord->singlePalette != 0) {
                        curFirstPaletteStep = timRecord->paletteRow;
                        palStop = curFirstPaletteStep + 1;
                    } else {
                        curFirstPaletteStep = 0;
                        palStop = headerPtr->paletteCount;
                    }
                    palCursor = texFormat8->palette;
                    for (palLoop = curFirstPaletteStep; palLoop < palStop; palLoop++) {
                        unitSawZero = 0;
                        curRemappedSpan = 0;
                        entAddr = (int *)palCursor;
                        savedColor8 = *entAddr;
                        for (outIdx = 0; outIdx < headerPtr->entriesPerPalette; outIdx++) {
                            remapTable[outIdx] = outIdx;
                            if (*(int *)palCursor == 0) {
                                if (outIdx == 0) {
                                    if (DEBUG_TRACE) {
                                        palCursor->colorR = 0xFF;
                                        palCursor->colorG = 0;
                                        palCursor->colorB = 0;
                                        palCursor->alpha = 0xFF;
                                    }
                                    unitSawZero = 1;
                                } else {
                                    if (unitSawZero != 0) {
                                        unitSawZero = 1;
                                    } else {
                                        *entAddr = 0;
                                        *(int *)palCursor = savedColor8;
                                        *remapTable = outIdx;
                                        if (DEBUG_TRACE) {
                                            // STRING: FF7 0x009118e4
                                            Gfx_DebugPrint("COLOR SWAP \n");
                                        }
                                        unitSawZero = 1;
                                    }
                                    remapTable[outIdx] = 0;
                                    curRemappedSpan = 1;
                                }
                            }
                            palCursor++;
                        }
                    }
                }
            }
            if (texFormat8->paletteEntryCount > 0) {
                argbPtrPtr = texFormat8->palette;
                for (outIdx = 0; outIdx < texFormat8->paletteEntryCount; outIdx++) {
                    if ((timRecord->flags & 0x10000) != 0) {
                        if (outIdx == 0) {
                            // STRING: FF7 0x009118f4
                            sprintf(messageD, "TIM FILE %s \n", fileName);
                            Gfx_DebugPrint(messageD);
                        }
                        // STRING: FF7 0x00911904
                        sprintf(messageD, "P %03d RGBA %2X %2X %2X %2X \n", outIdx, argbPtrPtr->colorR, argbPtrPtr->colorG, argbPtrPtr->colorB, argbPtrPtr->alpha);
                        Gfx_DebugPrint(messageD);
                    }
                    if ((timRecord->flags & 1) != 0 && argbPtrPtr->colorR <= timRecord->maxRed && argbPtrPtr->colorG <= timRecord->maxGreen && argbPtrPtr->colorB <= timRecord->maxBlue) {
                        *(int *)argbPtrPtr &= 0xFF000000;
                    }
                    argbPtrPtr++;
                }
            }
            imageWidth = Pfile_TimHalfwordWidthToPixelWidth(timRecord->header->flags & TIM_FLAG_PIXEL_MODE_MASK, pImageBlock->rect.width);
            timHeight = pImageBlock->rect.height;
            if (entireImage != 0) {
                texFormat8->width = imageWidth;
                texFormat8->height = timHeight;
            } else {
                texFormat8->width = rectWidth;
                texFormat8->height = rectHeight;
                curAvailWidth = imageWidth - rectX;
                spareHeight = timHeight - rectY;
                if (rectWidth > curAvailWidth) {
                    texFormat8->width = curAvailWidth;
                }
                if (rectHeight > spareHeight) {
                    texFormat8->height = spareHeight;
                }
            }
            switch (timRecord->header->flags & TIM_FLAG_PIXEL_MODE_MASK) {
            case TIM_PIXEL_MODE_4BPP:
                // STRING: FF7 0x00911924
                headerPtr->pixelBuffer = (int)GameHeapAlloc(texFormat8->width * texFormat8->height, 1, "C:\\lib\\src\\polygon\\tim.cpp", 0x30C);
                curBufEntry = (int *)headerPtr->pixelBuffer;
                if (curBufEntry != 0) {
                    rowWords = imageWidth / 4;
                    rowOffset = rectY * rowWords + rectX / 4;
                    outIdx = 0;
                    for (rowIdx = 0; rowIdx < texFormat8->height; rowIdx++) {
                        rawWordIdx = rowOffset;
                        for (column = 0; column < texFormat8->width; column += 4) {
                            curBufEntry[outIdx] = Pfile_TimSpreadNibbles(*(unsigned short *)&((short *)(pImageBlock->data))[rawWordIdx]);
                            if (remapTable != 0) {
                                tempQuadAddr = (int)curBufEntry + outIdx * 4;
                                for (nibbleIdxUnit = 0; nibbleIdxUnit < 4; nibbleIdxUnit++) {
                                    g_TimClutIndex4Bit = *(unsigned char *)(tempQuadAddr + nibbleIdxUnit);
                                    *(char *)(tempQuadAddr + nibbleIdxUnit) = *(char *)&remapTable[g_TimClutIndex4Bit];
                                }
                            }
                            rawWordIdx++;
                            outIdx++;
                        }
                        rowOffset += rowWords;
                    }
                }
                break;
            case TIM_PIXEL_MODE_8BPP:
                // STRING: FF7 0x00911940
                headerPtr->pixelBuffer = (int)GameHeapAlloc(texFormat8->width * texFormat8->height, 1, "C:\\lib\\src\\polygon\\tim.cpp", 0x344);
                shortBuf = (short *)headerPtr->pixelBuffer;
                if (shortBuf != 0) {
                    rowWords = imageWidth / 2;
                    rowOffset = rectY * rowWords + rectX / 2;
                    outIdx = 0;
                    for (rowIdx = 0; rowIdx < texFormat8->height; rowIdx++) {
                        rawWordIdx = rowOffset;
                        for (column = 0; column < texFormat8->width; column += 2) {
                            shortBuf[outIdx] = ((short *)(pImageBlock->data))[rawWordIdx];
                            rawWordIdx++;
                            outIdx++;
                        }
                        rowOffset += rowWords;
                    }
                }
                if (remapTable != 0) {
                    buffer8 = headerPtr->pixelBuffer;
                    pixCnt = texFormat8->width * texFormat8->height;
                    for (pixelIndex = 0; pixelIndex < pixCnt; pixelIndex++) {
                        g_TimClutIndex8Bit = *(unsigned char *)(buffer8 + pixelIndex);
                        *(char *)(buffer8 + pixelIndex) = *(char *)&remapTable[g_TimClutIndex8Bit];
                    }
                }
                break;
            case TIM_PIXEL_MODE_16BPP:
                headerPtr->bitsPerPixel = 0x10;
                bitCnt = 0x10;
                selRedMask = 0x1F;
                tempGreenMask = 0x3E0;
                blueMask = 0x7C00;
                tempAlphaMask = 0x8000;
                PixelFormatSetRgbMasks(bitCnt, selRedMask, tempGreenMask, blueMask, tempAlphaMask, texFormat8);
                outIdx = 0;
                for (rowIdx = 0; rowIdx < pImageBlock->rect.height; rowIdx++) {
                    pixel = ((short *)(pImageBlock->data))[outIdx];
                    for (column = 0; column < pImageBlock->rect.width; column++) {
                        if (pixel == 0) {
                            blackFlag = 1;
                        } else if ((pixel & 0x8000) != 0) {
                            semiTrans = 1;
                        }
                        outIdx++;
                    }
                }
                headerPtr->pixelBuffer = (int)GameHeapMemDup(texFormat8->width * texFormat8->height * texFormat8->bytesPerPixel, (void *)pImageBlock->data);
                break;
            case TIM_PIXEL_MODE_24BPP:
                headerPtr->bitsPerPixel = 0x18;
                bitCnt = 0x18;
                selRedMask = 0xFF;
                tempGreenMask = 0xFF00;
                blueMask = 0xFF0000;
                tempAlphaMask = 0;
                PixelFormatSetRgbMasks(bitCnt, selRedMask, tempGreenMask, blueMask, tempAlphaMask, texFormat8);
                headerPtr->pixelBuffer = (int)GameHeapMemDup(texFormat8->width * texFormat8->height * texFormat8->bytesPerPixel, (void *)pImageBlock->data);
            }
            if (headerPtr->pixelBuffer != 0) {
                if (IsPowerOfTwo(texFormat8->width) != 0 && IsPowerOfTwo(texFormat8->height) != 0) {
                    failure = 0;
                } else {
                    outW = texFormat8->width;
                    if (IsPowerOfTwo(texFormat8->width) == 0) {
                        outW = RoundUpToPowerOfTwo(texFormat8->width);
                        if (DEBUG_TRACE) {
                            // STRING: FF7 0x0091195c
                            sprintf(warnBuffer, "TIM FILE %s: WIDTH %d IS NOT A POWER OF TWO \n", fileName, texFormat8->width);
                            Gfx_DebugPrint(warnBuffer);
                        }
                    }
                    outHeight = texFormat8->height;
                    if (IsPowerOfTwo(texFormat8->height) == 0) {
                        outHeight = RoundUpToPowerOfTwo(texFormat8->height);
                        if (DEBUG_TRACE) {
                            // STRING: FF7 0x0091198c
                            sprintf(warnBuffer, "TIM FILE %s: HEIGHT %d IS NOT A POWER OF TWO \n", fileName, texFormat8->height);
                            Gfx_DebugPrint(warnBuffer);
                        }
                    }
                    xPad = outW - texFormat8->width;
                    mainPadBottom = outHeight - texFormat8->height;
                    // STRING: FF7 0x009119bc
                    padBuf = (int)GameHeapAlloc(1, outW * outHeight * texFormat8->bytesPerPixel, "C:\\lib\\src\\polygon\\tim.cpp", 0x3E2);
                    if (padBuf != 0) {
                        dstRow = padBuf;
                        srcPtr = headerPtr->pixelBuffer;
                        for (rowIdx = 0; rowIdx < texFormat8->height; rowIdx++) {
                            memcpy((void *)dstRow, (void *)srcPtr, texFormat8->width * texFormat8->bytesPerPixel);
                            srcPtr += texFormat8->width * texFormat8->bytesPerPixel;
                            dstRow += outW * texFormat8->bytesPerPixel;
                        }
                        // STRING: FF7 0x009119d8
                        GameHeapFree((void *)headerPtr->pixelBuffer, "C:\\lib\\src\\polygon\\tim.cpp", 0x3F1);
                        if (texFormat8->bytesPerPixel == 1) {
                            if ((timRecord->flags & 2) != 0 && xPad > 0) {
                                dstRow = padBuf;
                                for (rowIdx = 0; rowIdx < texFormat8->height; rowIdx++) {
                                    Mem_FillBytes(*(unsigned char *)((char *)dstRow + texFormat8->width + -1), xPad * texFormat8->bytesPerPixel, dstRow + texFormat8->width);
                                    dstRow += outW * texFormat8->bytesPerPixel;
                                }
                            }
                            if ((timRecord->flags & 4) != 0) {
                                dstRow = padBuf + texFormat8->height * outW * texFormat8->bytesPerPixel;
                                for (rowIdx = texFormat8->height; rowIdx < outHeight; rowIdx++) {
                                    Mem_CopyBytesFast(texFormat8->width * texFormat8->bytesPerPixel, (void *)dstRow, (const void *)(padBuf + (texFormat8->height - 1) * outW * texFormat8->bytesPerPixel));
                                    dstRow += outW * texFormat8->bytesPerPixel;
                                }
                            }
                        }
                        texFormat8->width = outW;
                        texFormat8->height = outHeight;
                        headerPtr->pixelBuffer = padBuf;
                        failure = 0;
                    }
                }
                if (semiTrans != 0) {
                    headerPtr->minAlphaBits = 4;
                }
                headerPtr->colorKey = blackFlag;
                headerPtr->hasSemiTransparency = semiTrans;
                headerPtr->timTexPage = timRecord->texPage;
                if (UNFOLDED(0) && headerPtr->paletteCount > 0) {
                    headerPtr->paletteMode = 1;
                    // STRING: FF7 0x009119f4
                    headerPtr->colorLookup = (int)GameHeapAlloc(headerPtr->paletteCount, 1, "C:\\lib\\src\\polygon\\tim.cpp", 0x433);
                    for (lookupIndex = 0; lookupIndex < headerPtr->paletteCount; lookupIndex++) {
                        if ((lookupIndex & 1) != 0) {
                            *(char *)((char *)headerPtr->colorLookup + lookupIndex) = 1;
                        }
                    }
                }
            }
            // STRING: FF7 0x00911a10
            GameHeapFree(remapTable, "C:\\lib\\src\\polygon\\tim.cpp", 0x443);
        }
    }
    if (failure != 0) {
        Gfx_ReleaseCachedTexture((int)headerPtr);
        headerPtr = 0;
    }
    return (int)headerPtr;
}

/*
 * Builds a full texture from a loaded TIM record.
 */
// FUNCTION: FF7 0x00698e80
int __cdecl Pfile_BuildTextureFromTim(PolygonTim *timRecord, const char *fileName)
{
    GfxTexHeader *texture;

    if (DEBUG_TRACE) {
        if (((timRecord->header->flags & TIM_FLAG_HAS_CLUT) != 0 &&
             (timRecord->header->flags & TIM_FLAG_PIXEL_MODE_MASK) == TIM_PIXEL_MODE_4BPP &&
             (timRecord->image->rect.width << 2) > 256) ||
            ((timRecord->header->flags & TIM_FLAG_PIXEL_MODE_MASK) == TIM_PIXEL_MODE_8BPP &&
             (timRecord->image->rect.width << 1) > 256)) {
            texture = (GfxTexHeader *)Pfile_BuildTextureFromTimRect(256, 0, 64, 64, timRecord, fileName);
            return (int)texture;
        }
    }
    texture = (GfxTexHeader *)Pfile_BuildTextureFromTimRect(0, 0, 0, 0, timRecord, fileName);
    return (int)texture;
}

/*
 * Releases a texture set and its allocated textures or array.
 */
// FUNCTION: FF7 0x00698f24
void __cdecl Pfile_FreeTextureSet(int releaseTextures, PolygonTextureSet *textureSet)
{
    int index;

    if (textureSet != 0) {
        if (textureSet->textures != 0) {
            if (releaseTextures != 0) {
                for (index = 0; index < textureSet->textureCount; index++) {
                    Gfx_ReleaseCachedTexture(((int *)textureSet->textures)[index]);
                }
            }
            // STRING: FF7 0x00911a2c
            GameHeapFree((void *)textureSet->textures, "C:\\lib\\src\\polygon\\tim.cpp", 0x477);
        }
        // STRING: FF7 0x00911a48
        GameHeapFree(textureSet, "C:\\lib\\src\\polygon\\tim.cpp", 0x47A);
    }
}

/*
 * Allocates a texture set header structure and pointer array.
 */
// FUNCTION: FF7 0x00698fa3
int __cdecl Pfile_AllocTextureSetHeader(int count)
{
    PolygonTextureSet *header;

    header = 0;
    if (count > 0) {
        // STRING: FF7 0x00911a64
        header = (PolygonTextureSet *)GameHeapAlloc(1, 8, "C:\\lib\\src\\polygon\\tim.cpp", 0x485);
        if (header != 0) {
            header->textureCount = count;
            // STRING: FF7 0x00911a80
            header->textures = (int)GameHeapAlloc(count, 4, "C:\\lib\\src\\polygon\\tim.cpp", 0x489);
        }
    }
    return (int)header;
}
// clang-format on
} /* extern "C" */

extern "C" {

/*
 * Divides a TIM image into tiles and builds a texture set from them.
 */
// FUNCTION: FF7 0x00699000
int __cdecl Pfile_BuildTextureTileSetFromTim(int tileWidth, int tileHeight, PolygonTim *timRecord, int context)
{
    int column;
    int imageWidth;
    int tileCount;
    PolygonTimBlock *imageRectangle;
    struct GfxTexHeader *tilePtr;
    PolygonTextureSet *textureSet;
    int y;
    int imageH;
    int tileIdx;

    imageRectangle = timRecord->image;
    imageWidth = Pfile_TimHalfwordWidthToPixelWidth(timRecord->header->flags & TIM_FLAG_PIXEL_MODE_MASK, imageRectangle->rect.width);
    imageH = imageRectangle->rect.height;
    tileCount = 0;
    for (y = 0; y < imageH + tileHeight + -1; y += tileHeight) {
        for (column = 0; column < imageWidth + tileWidth + -1; column += tileWidth) {
            tileCount++;
        }
    }
    textureSet = (PolygonTextureSet *)Pfile_AllocTextureSetHeader(tileCount);
    if (textureSet != 0) {
        tileIdx = 0;
        for (y = 0; y < imageH + tileHeight + -1; y += tileHeight) {
            for (column = 0; column < imageWidth + tileWidth + -1; column += tileWidth) {
                tilePtr = (struct GfxTexHeader *)Pfile_BuildTextureFromTimRect(column, y, tileWidth, tileHeight, timRecord, (const char *)context);
                if (tilePtr != 0) {
                    ((int *)textureSet->textures)[tileIdx] = (int)tilePtr;
                    tileIdx++;
                }
            }
        }
    }
    return (int)textureSet;
}

/*
 * Writes a TIM record header, CLUT, and image pixel data to a file.
 */
// FUNCTION: FF7 0x0069913c
int __cdecl Pfile_WriteTimRecordToFile(int useRegion, int regionX, int regionY, int regionWidth, int height, PolygonTim *timRecord, int fileName)
{
    struct TimRect {
        short x, y, w, h;
    };
    struct {
        int row;
        short *pixels;
        int left;
        int rowBytes;
        struct TimRect rect;
        int pixelBytes;
        int sourceWidth;
        int width;
        int blockBytes;
        int paletteBytes;
        struct FileHandle *file;
        int failed;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */


    frame.failed = 1;
    if (timRecord != 0) {
        frame.file = (struct FileHandle *)OpenFileSimple(FILE_OPEN_CREATE, (void *)fileName);
        if (frame.file != 0) {
            if (WriteFileData(8, timRecord->header, frame.file)) {
                if ((timRecord->header->flags & TIM_FLAG_HAS_CLUT) != 0) {
                    if (timRecord->clut != 0) {
                        if (WriteFileData(4, timRecord->clut, frame.file)) {
                            if (WriteFileData(8, &timRecord->clut->rect, frame.file)) {
                                frame.paletteBytes = timRecord->clut->size - 12;
                                if (frame.paletteBytes > 0) {
                                    if (WriteFileData(frame.paletteBytes, (void *)timRecord->clut->data, frame.file)) {
                                        frame.failed = 0;
                                    }
                                }
                            }
                        }
                    }
                } else {
                    frame.failed = 0;
                }
                if (frame.failed == 0) {
                    frame.failed = 1;
                    if (timRecord->image != 0) {
                        frame.rect = *(struct TimRect *)&timRecord->image->rect;
                        frame.sourceWidth = frame.rect.w;
                        if (useRegion != 0) {
                            regionWidth = regionWidth;
                            height = height;
                        } else {
                            regionX = 0;
                            regionY = 0;
                            regionWidth = timRecord->width;
                            height = timRecord->height;
                        }
                        switch (timRecord->header->flags & TIM_FLAG_PIXEL_MODE_MASK) {
                        case TIM_PIXEL_MODE_4BPP:
                            frame.rect.w = regionWidth / 4;
                            frame.rect.h = height;
                            frame.pixelBytes = frame.rect.w * frame.rect.h * 2;
                            frame.left = regionX / 4;
                            break;
                        case TIM_PIXEL_MODE_8BPP:
                            frame.rect.w = regionWidth / 2;
                            frame.rect.h = height;
                            frame.pixelBytes = frame.rect.w * frame.rect.h * 2;
                            frame.left = regionX / 2;
                            break;
                        case TIM_PIXEL_MODE_16BPP:
                            frame.rect.w = regionWidth;
                            frame.rect.h = height;
                            frame.pixelBytes = frame.rect.w * frame.rect.h * 2;
                            frame.left = regionX;
                            break;
                        case TIM_PIXEL_MODE_24BPP:
                            frame.rect.w = regionWidth * 3 / 2;
                            frame.rect.h = height;
                            frame.pixelBytes = frame.rect.w * frame.rect.h * 2;
                            frame.left = regionX * 3 / 2;
                            break;
                        }
                        frame.width = frame.rect.w;
                        frame.rowBytes = frame.width * 2;
                        frame.blockBytes = frame.pixelBytes + 12;
                        if (WriteFileData(4, &frame.blockBytes, frame.file)) {
                            if (WriteFileData(8, &frame.rect, frame.file)) {
                                if (timRecord->image->data != 0) {
                                    frame.failed = 0;
                                    frame.pixels = *(short **)&timRecord->image->data;
                                    frame.pixels += regionY * frame.sourceWidth;
                                    frame.pixels += frame.left;
                                    for (frame.row = 0; frame.row < height; frame.row++) {
                                        if (WriteFileData(frame.rowBytes, frame.pixels, frame.file)) { /* empty first arm: lib/codegen.h */
                                        } else {
                                            frame.failed = 1;
                                            break;
                                        }
                                        frame.pixels += frame.sourceWidth;
                                    }
                                }
                            }
                        }
                    }
                }
            }
            CloseFileEx((struct FileHandle *)frame.file);
        }
    }
    return frame.failed == 0;
}
} /* extern "C" */
