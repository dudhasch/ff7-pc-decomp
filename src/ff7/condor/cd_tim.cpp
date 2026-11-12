// src/ff7/condor/cd_tim.cpp
// Original: C:\FF7\condor\cd_tim.cpp
// Contents: Converts the Fort Condor battlefield's PSX TIM texture images for display: parses TIM
//   blocks, repacks palettes, scales palette channels for dimmed variants, builds DirectDraw
//   surfaces, copies rectangles, and exports debug terrain tiles.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/api/mem.h"
#include <stdio.h>
#include <string.h>
#include "ff7/api/condor.h"
#include "ff7/types/condor.h"
#include "ff7/api/field.h"
extern "C" {

#pragma intrinsic(strcat, memcpy)

/*
 * Frees allocated TIM texture surface and palette buffers and nulls their pointers.
 */
// FUNCTION: FF7 0x005f5c40
void __cdecl Mini_CondorFreeTimSurfaceAndPaletteRecords(void)
{
    if (g_CondorTimSurface != 0) {
        // STRING: FF7 0x00901ab4
        GameHeapFree((void *)g_CondorTimSurface, "C:\\FF7\\condor\\cd_tim.cpp", 49);
        g_CondorTimSurface = 0;
    }
    if (g_pCondorTimPixelRecords != 0) {
        // STRING: FF7 0x00901ad0
        GameHeapFree(g_pCondorTimPixelRecords, "C:\\FF7\\condor\\cd_tim.cpp", 54);
        g_pCondorTimPixelRecords = 0;
    }
}
/*
 * Unpacks indexed palette pixels into 32-bit pixel data with 2x horizontal scaling.
 */
// FUNCTION: FF7 0x005f5c96
int __cdecl Mini_CondorExpandIndexedPixels2x(int dst, struct CondorTimPalette *palette, int indexData, short width, short height, int stride)
{
    unsigned short pixel;
    int pixelPair;
    int outBase;
    unsigned short rowIndex;
    unsigned short colIndex;

    outBase = 0;
    if (indexData == 0) {
        return 0;
    } else {
        outBase = dst;
        if (palette->records == 0) {
            return 0;
        } else {
            for (rowIndex = 0; rowIndex < ((unsigned short)height); rowIndex++) {
                for (colIndex = 0; colIndex < ((unsigned short)width); colIndex++) {
                    pixel = palette->records[*(unsigned char *)((indexData + ((rowIndex * ((unsigned short)width)) + colIndex)))].pixelWord;
                    pixelPair = pixel << 0x10 | pixel;
                    ((int *)outBase)[rowIndex * stride + colIndex] = pixelPair;
                    ((int *)outBase)[rowIndex * stride + ((unsigned int)stride >> 1) + colIndex] = pixelPair;
                }
            }
            return 1;
        }
    }
}
/*
 * Unpacks indexed palette pixels into 16-bit pixel data with 1x scale.
 */
// FUNCTION: FF7 0x005f5dcb
int __cdecl Mini_CondorExpandIndexedPixels1x(int dst, struct CondorTimPalette *palette, int indexData, short width, short height, int pitch)
{
    unsigned short pixel;
    int outBase;
    unsigned short rowIndex;
    unsigned short colIndex;

    outBase = 0;
    if (indexData == 0) {
        return 0;
    } else {
        outBase = dst;
        if (palette->records == 0) {
            return 0;
        } else {
            for (rowIndex = 0; rowIndex < ((unsigned short)height); rowIndex++) {
                for (colIndex = 0; colIndex < ((unsigned short)width); colIndex++) {
                    pixel = palette->records[*(unsigned char *)((indexData + ((rowIndex * ((unsigned short)width)) + colIndex)))].pixelWord;
                    *(short *)((char *)outBase + (((unsigned int)(rowIndex * pitch) >> 1) + colIndex) * 2) = pixel;
                }
            }
            return 1;
        }
    }
}
/*
 * Reorders 15-bit RGB channels to match target 5-5-5 or 5-6-5 display format.
 */
// FUNCTION: FF7 0x005f5ebc
short __cdecl Mini_CondorSwapPixelChannelsForDisplayFormat(unsigned short pixel)
{
    unsigned short swapped;

    swapped = pixel;
    if (g_CondorDisplayFormatValid == 1) {
        swapped = (pixel & 0x1F) << 0xA | pixel & 0x3E0 | (pixel & 0x7C00) >> 10;
    } else {
        swapped = (pixel & 0x1F) << 0xB | (pixel & 0x3E0) << 1 | (pixel & 0x7C00) >> 10;
    }
    return (short)swapped;
}
/*
 * Decomposes a 15-bit packed pixel word into red, green, and blue 5-bit color channels.
 */
// FUNCTION: FF7 0x005f5f49
void __cdecl Mini_CondorSplitPixel555Channels(unsigned short pixel, int redOut, int greenOut, int blueOut)
{
    *(char *)redOut = pixel & 0x1F;
    *(char *)greenOut = (pixel & 0x3E0) >> 5;
    *(char *)blueOut = (pixel & 0x7C00) >> 10;
}
/*
 * Converts raw TIM image pixel data into unpacked 6-byte palette record entries.
 */
// FUNCTION: FF7 0x005f5f8c
int __cdecl Mini_CondorTimBuildPixelRecords(unsigned short width, unsigned short height, int pixels)
{
    unsigned short row;
    unsigned short x;

    if (pixels == 0) {
        return 0;
    } else {
        g_CondorDisplayFormat = g_CondorDisplayFormatValid;
        g_CondorTimHeight = height;
        g_CondorTimPixelRecordSize[1] = width;
        if (((int)g_pCondorTimPixelRecords) != 0) {
            // STRING: FF7 0x00901aec
            GameHeapFree((void *)((int)g_pCondorTimPixelRecords), "C:\\FF7\\condor\\cd_tim.cpp", 144);
            g_pCondorTimPixelRecords = 0;
        }
        // STRING: FF7 0x00901b08
        g_pCondorTimPixelRecords = (struct CondorTimPixelRecord *)(GameHeapAllocRaw(height * width * 6 + 1, "C:\\FF7\\condor\\cd_tim.cpp", 148));
        if (((int)g_pCondorTimPixelRecords) == 0) {
            return 0;
        } else {
            for (row = 0; row < height; row++) {
                for (x = 0; x < width; x++) {
                    if (*(unsigned short *)((char *)(pixels + row * width * 2) + x * 2) == 0) {
                        g_pCondorTimPixelRecords[row * width + x].pixelWord = 0;
                        g_pCondorTimPixelRecords[row * width + x].red = 0;
                        g_pCondorTimPixelRecords[row * width + x].green = 0;
                        g_pCondorTimPixelRecords[row * width + x].blue = 0;
                    } else if ((*(unsigned short *)((char *)(pixels + row * width * 2) + x * 2) & 0x8000) != 0) {
                        if ((*(unsigned short *)((char *)(pixels + row * width * 2) + x * 2) & 0x7FFF) != 0) {
                            g_pCondorTimPixelRecords[row * width + x].pixelWord = Mini_CondorSwapPixelChannelsForDisplayFormat(*(short *)((char *)(pixels + row * width * 2) + x * 2));
                            Mini_CondorSplitPixel555Channels(*(short *)((char *)(pixels + row * width * 2) + x * 2), ((int)g_pCondorTimPixelRecords) + (row * width + x) * 6 + 2, ((int)g_pCondorTimPixelRecords) + (row * width + x) * 6 + 3, ((int)g_pCondorTimPixelRecords) + (row * width + x) * 6 + 4);
                        } else {
                            if (g_CondorDisplayFormat == 1) {
                                g_pCondorTimPixelRecords[row * width + x].pixelWord = 1;
                            } else {
                                g_pCondorTimPixelRecords[row * width + x].pixelWord = 1;
                            }
                            g_pCondorTimPixelRecords[row * width + x].red = 0;
                            g_pCondorTimPixelRecords[row * width + x].green = 0;
                            g_pCondorTimPixelRecords[row * width + x].blue = 1;
                        }
                    } else {
                        g_pCondorTimPixelRecords[row * width + x].pixelWord = Mini_CondorSwapPixelChannelsForDisplayFormat(*(short *)((char *)(pixels + row * width * 2) + x * 2));
                        Mini_CondorSplitPixel555Channels(*(short *)((char *)(pixels + row * width * 2) + x * 2), ((int)g_pCondorTimPixelRecords) + (row * width + x) * 6 + 2, ((int)g_pCondorTimPixelRecords) + (row * width + x) * 6 + 3, ((int)g_pCondorTimPixelRecords) + (row * width + x) * 6 + 4);
                    }
                }
            }
            return 1;
        }
    }
}
/*
 * Repacks color channels in the 6-byte palette record table into 16-bit words.
 */
// FUNCTION: FF7 0x005f650b
int __cdecl Mini_CondorRepackPaletteWords(void)
{
    unsigned short rowIndex;
    unsigned short colIndex;

    for (rowIndex = 0; rowIndex < g_CondorTimHeight; rowIndex++) {
        for (colIndex = 0; colIndex < g_CondorTimPixelRecordSize[1]; colIndex++) {
            if (g_pCondorTimPixelRecords[rowIndex * g_CondorTimPixelRecordSize[1] + colIndex].red < 128) {
                if (g_CondorDisplayFormat == 1) {
                    g_pCondorTimPixelRecords[rowIndex * g_CondorTimPixelRecordSize[1] + colIndex].pixelWord = g_pCondorTimPixelRecords[rowIndex * g_CondorTimPixelRecordSize[1] + colIndex].red << 0xA | g_pCondorTimPixelRecords[rowIndex * g_CondorTimPixelRecordSize[1] + colIndex].green << 5 | g_pCondorTimPixelRecords[rowIndex * g_CondorTimPixelRecordSize[1] + colIndex].blue;
                } else {
                    g_pCondorTimPixelRecords[rowIndex * g_CondorTimPixelRecordSize[1] + colIndex].pixelWord = g_pCondorTimPixelRecords[rowIndex * g_CondorTimPixelRecordSize[1] + colIndex].red << 0xB | g_pCondorTimPixelRecords[rowIndex * g_CondorTimPixelRecordSize[1] + colIndex].green << 6 | g_pCondorTimPixelRecords[rowIndex * g_CondorTimPixelRecordSize[1] + colIndex].blue;
                }
            }
        }
    }
    return 1;
}
/*
 * Scales palette color channels by 80/128 for dimmed/shaded texture variant.
 */
// FUNCTION: FF7 0x005f6747
int __cdecl Mini_CondorTimScalePaletteChannels80Of128(void)
{
    unsigned short rowIndex;
    unsigned short colIndex;
    for (rowIndex = 0; rowIndex < g_CondorTimHeight; rowIndex++) {
        for (colIndex = 0; colIndex < g_CondorTimPixelRecordSize[1]; colIndex++) {
            if (g_CondorDisplayFormat == 1) {
                g_pCondorTimPixelRecords[rowIndex * g_CondorTimPixelRecordSize[1] + colIndex].pixelWord = ((g_pCondorTimPixelRecords[rowIndex * g_CondorTimPixelRecordSize[1] + colIndex].red * 80 / 128) << 10) | ((g_pCondorTimPixelRecords[rowIndex * g_CondorTimPixelRecordSize[1] + colIndex].green * 80 / 128) << 5) | (g_pCondorTimPixelRecords[rowIndex * g_CondorTimPixelRecordSize[1] + colIndex].blue * 80 / 128);
            } else {
                g_pCondorTimPixelRecords[rowIndex * g_CondorTimPixelRecordSize[1] + colIndex].pixelWord = ((g_pCondorTimPixelRecords[rowIndex * g_CondorTimPixelRecordSize[1] + colIndex].red * 80 / 128) << 11) | ((g_pCondorTimPixelRecords[rowIndex * g_CondorTimPixelRecordSize[1] + colIndex].green * 80 / 128) << 6) | (g_pCondorTimPixelRecords[rowIndex * g_CondorTimPixelRecordSize[1] + colIndex].blue * 80 / 128);
            }
        }
    }
    return 1;
}
/*
 * Scales palette color channels by 48/128 for dark/shadowed texture variant.
 */
// FUNCTION: FF7 0x005f699a
int __cdecl Mini_CondorTimScalePaletteChannels48Of128(void)
{
    unsigned short rowIndex;
    unsigned short colIndex;
    for (rowIndex = 0; rowIndex < g_CondorTimHeight; rowIndex++) {
        for (colIndex = 0; colIndex < g_CondorTimPixelRecordSize[1]; colIndex++) {
            if (g_CondorDisplayFormat == 1) {
                g_pCondorTimPixelRecords[rowIndex * g_CondorTimPixelRecordSize[1] + colIndex].pixelWord = ((g_pCondorTimPixelRecords[rowIndex * g_CondorTimPixelRecordSize[1] + colIndex].red * 48 / 128) << 10) | ((g_pCondorTimPixelRecords[rowIndex * g_CondorTimPixelRecordSize[1] + colIndex].green * 48 / 128) << 5) | (g_pCondorTimPixelRecords[rowIndex * g_CondorTimPixelRecordSize[1] + colIndex].blue * 48 / 128);
            } else {
                g_pCondorTimPixelRecords[rowIndex * g_CondorTimPixelRecordSize[1] + colIndex].pixelWord = ((g_pCondorTimPixelRecords[rowIndex * g_CondorTimPixelRecordSize[1] + colIndex].red * 48 / 128) << 11) | ((g_pCondorTimPixelRecords[rowIndex * g_CondorTimPixelRecordSize[1] + colIndex].green * 48 / 128) << 6) | (g_pCondorTimPixelRecords[rowIndex * g_CondorTimPixelRecordSize[1] + colIndex].blue * 48 / 128);
            }
        }
    }
    return 1;
}
/*
 * Parses PSX TIM file header and extracts CLUT and pixel image block records.
 */
// FUNCTION: FF7 0x005f6bed
int __cdecl Mini_CondorTimParseBlocks(int timData)
{
    int cursor;

    cursor = timData;
    g_CondorTimId = *(int *)cursor;
    cursor += 4;
    g_CondorTimFlags = *(int *)cursor;
    cursor += 4;
    (*(unsigned int *)&g_CondorTimBlockSize) = *(int *)cursor;
    cursor += 4;
    g_CondorTimClutOrigin[0] = *(int *)cursor & 0xFFFF;
    g_CondorTimClutOrigin[1] = (*(int *)cursor & 0xFFFF0000) >> 16;
    cursor += 4;
    g_CondorTimClutSize[0] = *(int *)cursor & 0xFFFF;
    g_CondorTimClutSize[1] = (*(int *)cursor & 0xFFFF0000) >> 16;
    cursor += 4;
    g_CondorTimPixelData = cursor;
    cursor += ((*(unsigned int *)&g_CondorTimBlockSize) - 0xC >> 2) * 4;
    (*(unsigned int *)&g_CondorTimImageBlockSize) = *(int *)cursor;
    cursor += 4;
    g_CondorTimImageOrigin[0] = *(int *)cursor & 0xFFFF;
    g_CondorTimImageOrigin[1] = (*(int *)cursor & 0xFFFF0000) >> 16;
    cursor += 4;
    g_CondorTimImageSize[0] = (*(int *)cursor & 0xFFFF & 0xFFFF) << 1;
    g_CondorTimImageSize[1] = (*(int *)cursor & 0xFFFF0000) >> 16;
    cursor += 4;
    g_pCondorTimData = cursor;
    if (Mini_CondorTimBuildPixelRecords((short)*(unsigned short *)&g_CondorTimClutSize[0], (short)*(unsigned short *)&g_CondorTimClutSize[1], g_CondorTimPixelData) == 0) {
        return 0;
    } else {
        cursor = cursor + ((*(unsigned int *)&g_CondorTimImageBlockSize) >> 2) * 4 + -0xC;
        g_CondorTimImage2BlockSize = *(int *)cursor;
        cursor += 4;
        g_CondorTimImage2Origin[0] = *(int *)cursor & 0xFFFF;
        g_CondorTimImage2Origin[1] = (*(int *)cursor & 0xFFFF0000) >> 16;
        cursor += 4;
        g_CondorTimImage2Size[0] = (*(int *)cursor & 0xFFFF & 0xFFFF) << 1;
        g_CondorTimImage2Size[1] = (*(int *)cursor & 0xFFFF0000) >> 16;
        cursor += 4;
        g_pCondorTimImage2Data = cursor;
        return 1;
    }
}
/*
 * Debug export utility that writes eight 256x256 battlefield terrain tiles to disk.
 */
// FUNCTION: FF7 0x005f6df4
void __cdecl Mini_CondorTimExportMapTiles(void)
{
    char *buf;
    char path[32];
    char *srcRow;
    FILE *handle;
    char *dstRow;
    unsigned short rowIndex;
    unsigned short tileIdx;
    // STRING: FF7 0x00901b24
    dstRow = (char *)GameHeapAllocRaw(65536, "C:\\FF7\\condor\\cd_tim.cpp", 296);
    buf = dstRow;
    for (tileIdx = 0; tileIdx < 8; tileIdx++) {
        dstRow = buf;
        // STRING: FF7 0x00901b40
        sprintf(path, "map%d", tileIdx);
        // STRING: FF7 0x00901b48
        strcat(path, ".tim");
        // STRING: FF7 0x00901b50
        handle = Field_FileOpen(path, "wb");
        if (handle != 0) {
            Field_DebugWriteU32(handle, g_CondorTimId);
            Field_DebugWriteU32(handle, g_CondorTimFlags);
            Field_DebugWriteU32(handle, (*(unsigned int *)&g_CondorTimBlockSize));
            Field_DebugWriteU32(handle, g_CondorTimClutOrigin[1] << 16 | g_CondorTimClutOrigin[0]);
            Field_DebugWriteU32(handle, *(unsigned short *)&g_CondorTimClutSize[1] << 16 | *(unsigned short *)&g_CondorTimClutSize[0]);
            Field_FileWrite(handle, (void *)g_CondorTimPixelData, (*(unsigned int *)&g_CondorTimBlockSize) - 12);
            Field_DebugWriteU32(handle, 0x1000c);
            Field_DebugWriteU32(handle, g_CondorTimImageOrigin[1] << 16 | g_CondorTimImageOrigin[0]);
            Field_DebugWriteU32(handle, *(unsigned short *)&g_CondorTimImageSize[0] / 4 | 0x1000000);
            srcRow = (char *)g_pCondorTimData + ((tileIdx / 2) << 8 << 9) + ((tileIdx % 2) << 8);
            for (rowIndex = 0; rowIndex < 256; rowIndex++) {
                memcpy(dstRow, srcRow, 256);
                dstRow += 256;
                srcRow += 512;
            }
            Field_FileWrite(handle, buf, 65536);
            Field_FileClose(handle);
        }
    }
    // STRING: FF7 0x00901b54
    GameHeapFree(dstRow, "C:\\FF7\\condor\\cd_tim.cpp", 331);
}
#pragma intrinsic(strcpy, strcat)

int __cdecl Mini_CondorLockSurfaceBits(int surface, int **bits, unsigned int *pitch);

int __cdecl Mini_CondorUnlockSurface(int surface);
/*
 * Allocates DirectDraw surface for TIM texture data and copies pixel buffer.
 */
// FUNCTION: FF7 0x005f704c
int __cdecl Mini_CondorBuildTimSurface(void *timData)
{
    if (Mini_CondorTimParseBlocks((int)timData) != 0) {
        g_CondorTimSurface = Mini_CondorAllocSurfaceBuffer16(*(unsigned short *)&g_CondorTimImageSize[0], *(unsigned short *)&g_CondorTimImageSize[1]);
        if (g_CondorTimSurface != 0) {
            Mini_CondorExpandIndexedPixels1x(g_CondorTimSurface, (struct CondorTimPalette *)&g_CondorDisplayFormat, g_pCondorTimData, (short)*(unsigned short *)&g_CondorTimImageSize[0], g_CondorTimImageSize[1], *(unsigned short *)&g_CondorTimImageSize[0] << 1);
        }
    }
    return 1;
}
/*
 * Generates normal and dimmed texture surfaces with 2x magnification.
 */
// FUNCTION: FF7 0x005f70c5
int __cdecl Mini_CondorBuildDimmedTimSurfaces2x(int timBuffer)
{
    if (Mini_CondorTimParseBlocks(timBuffer) != 0) {
        g_CondorDimmedTimSurface0 = Mini_CondorAllocSurfaceBuffer16(*(unsigned short *)&g_CondorTimImageSize[0] << 1, *(unsigned short *)&g_CondorTimImageSize[1] << 1);
        g_CondorDimmedTimSurface1 = Mini_CondorAllocSurfaceBuffer16(*(unsigned short *)&g_CondorTimImageSize[0] << 1, *(unsigned short *)&g_CondorTimImageSize[1] << 1);
        g_CondorDimmedTimSurface2 = Mini_CondorAllocSurfaceBuffer16(*(unsigned short *)&g_CondorTimImageSize[0] << 1, *(unsigned short *)&g_CondorTimImageSize[1] << 1);
        if (g_CondorDimmedTimSurface0 != 0) {
            Mini_CondorExpandIndexedPixels2x(g_CondorDimmedTimSurface0, (struct CondorTimPalette *)&g_CondorDisplayFormat, g_pCondorTimData, (short)*(unsigned short *)&g_CondorTimImageSize[0], g_CondorTimImageSize[1], *(unsigned short *)&g_CondorTimImageSize[0] << 1);
        }
        if (g_CondorDimmedTimSurface1 != 0) {
            if (Mini_CondorTimScalePaletteChannels80Of128() != 0) {
                Mini_CondorExpandIndexedPixels2x(g_CondorDimmedTimSurface1, (struct CondorTimPalette *)&g_CondorDisplayFormat, g_pCondorTimData, (short)*(unsigned short *)&g_CondorTimImageSize[0], g_CondorTimImageSize[1], *(unsigned short *)&g_CondorTimImageSize[0] << 1);
            }
        }
        if (g_CondorDimmedTimSurface2 != 0) {
            if (Mini_CondorTimScalePaletteChannels48Of128() != 0) {
                Mini_CondorExpandIndexedPixels2x(g_CondorDimmedTimSurface2, (struct CondorTimPalette *)&g_CondorDisplayFormat, g_pCondorTimData, (short)*(unsigned short *)&g_CondorTimImageSize[0], g_CondorTimImageSize[1], *(unsigned short *)&g_CondorTimImageSize[0] << 1);
            }
        }
    }
    return 1;
}
/*
 * Locks destination surface and copies rectangular sub-region from source buffer.
 */
// FUNCTION: FF7 0x005f721d
void __cdecl Mini_CondorCopyRectToSurface(int surface, int srcBase, CondorRect *srcRect, CondorOrigin *dstRect, int row)
{
    int dstBits;
    int pitch;

    dstBits = 0;
    if (surface == 0) {
        return;
    }
    if (Mini_CondorLockSurfaceBits(surface, (int **)&dstBits, (unsigned int *)&pitch) != 0) {
        dstBits += (((unsigned int)(dstRect->y * pitch) >> 1) + dstRect->x) * 2;
        srcBase += (srcRect->y * row + srcRect->x) * 2;
        Mem_CopyDwordRows(g_CondorViewportWidth << 1, srcRect->yEnd - srcRect->y, (row << 1) - (g_CondorViewportWidth << 1), pitch - (g_CondorViewportWidth << 1), (void *)srcBase, (void *)dstBits);
        Mini_CondorUnlockSurface(surface);
    }
}
/*
 * Copies rectangular block of pixel rows between memory buffers.
 */
// FUNCTION: FF7 0x005f72d5
void __cdecl Mini_CondorCopyRectToBuffer(int dstBase, int srcBase, CondorRect *srcRect, CondorOrigin *dstRect, int row, int dstRow)
{
    int value;

    value = 0;
    if (dstBase == 0) {
        return;
    }
    dstBase += (((dstRect->y * dstRow) / 2) + dstRect->x) * 2;
    srcBase += (srcRect->y * row + srcRect->x) * 2;
    Mem_CopyDwordRows(g_CondorViewportWidth << 1, srcRect->yEnd - srcRect->y, (row << 1) - (g_CondorViewportWidth << 1), dstRow - (g_CondorViewportWidth << 1), (void *)srcBase, (void *)dstBase);
}
/*
 * Scales three color channels by lighting intensity and packs into 16-bit pixel.
 */
// FUNCTION: FF7 0x005f7364
unsigned short __cdecl Mini_CondorPackScaledColor(unsigned char red, unsigned char green, unsigned char blue, unsigned short intensity)
{
    short packed;

    if (g_CondorDisplayFormat == 1) {
        packed = (red * intensity / 128) << 0xA | (green * intensity / 128) << 5 | (blue * intensity / 128);
    } else {
        packed = (red * intensity / 128) << 0xB | (green * intensity / 128) << 5 | (blue * intensity / 128);
    }
    return (short)packed;
}
/*
 * Blits 320x220 source pixels to destination surface with double scaling and color modulation.
 */
// FUNCTION: FF7 0x005f7449
int __cdecl Mini_CondorBlitModulatedFrameDoubled(int surface, unsigned short *srcPixels, unsigned short srcX, unsigned short srcY, unsigned short srcStride, unsigned short intensity)
{
    int pair;
    int *dstWords;
    unsigned short color;
    unsigned char redValue;
    unsigned char blueVal;
    int *surfaceFlags;
    unsigned char greenChannel;
    unsigned short rowIndex;
    unsigned int rowPitch;
    unsigned short colIndex;

    surfaceFlags = 0;
    dstWords = 0;
    if (surface == 0) {
        return 0;
    }
    if (Mini_CondorLockSurfaceBits(surface, &surfaceFlags, &rowPitch)) {
        dstWords = surfaceFlags;
        for (rowIndex = srcY; rowIndex < srcY + 0xDC; rowIndex++) {
            for (colIndex = srcX; colIndex < srcX + 0x140; colIndex++) {
                color = srcPixels[rowIndex * srcStride * 2 + colIndex * 2];
                if (g_CondorDisplayFormat == 1) {
                    redValue = (unsigned char)((unsigned int)(color & g_CondorPixelMaskRed) >> 10);
                    greenChannel = (unsigned char)((unsigned int)(color & g_CondorPixelMaskGreen) >> 5);
                    blueVal = (unsigned char)(color & g_CondorPixelMaskBlue);
                    color = Mini_CondorPackScaledColor(redValue, greenChannel, blueVal, intensity);
                } else {
                    redValue = (unsigned char)((unsigned int)(color & g_CondorPixelMaskRed) >> 11);
                    greenChannel = (unsigned char)((unsigned int)(color & g_CondorPixelMaskGreen) >> 5);
                    blueVal = (unsigned char)(color & g_CondorPixelMaskBlue);
                    color = Mini_CondorPackScaledColor(redValue, greenChannel, blueVal, intensity);
                }
                pair = (color << 16) | color;
                dstWords[(rowIndex - srcY + 0xA) * rowPitch / 4 * 2 + (colIndex - srcX)] = pair;
                dstWords[(rowIndex - srcY + 0xA) * rowPitch / 4 * 2 + rowPitch / 4 + (colIndex - srcX)] = pair;
            }
        }
        Mini_CondorUnlockSurface(surface);
    }
    return 1;
}
} /* extern "C" */
