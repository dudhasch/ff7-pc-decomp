// src/ff7/field/src/ad_pal.cpp
// Original: C:\FF7\field\src\ad_pal.cpp

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
// Also: Converts and decodes 16-bit pixel words between the field's pixel modes.
#include "ff7/api/field.h"
#include "lib/enums/graphics.h"
#include "lib/api/trans.h"
#include "ff7/types/field.h"
#include "lib/types/graphics.h"
#include "ff7/enums/field.h"
#include "lib/api/mem.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
extern "C" {

/*
 * Converts a 15-bit pixel word of a TIM with pixel mode `mode` between channel orders. The CLUT modes
 * swap the red and blue fields; TIM_PIXEL_MODE_16BPP also keeps 0 (transparent) as 0 and maps 0x8000
 * (opaque black) to a fixed word. The swap depends on g_FieldTileDumpPaletteFlag.
 */
// FUNCTION: FF7 0x006470e0
short __cdecl Field_ConvertPixelWordMode(unsigned short pixel, int mode)
{
    unsigned short result;

    result = pixel;

    switch (mode) {
    case TIM_PIXEL_MODE_4BPP:
    case TIM_PIXEL_MODE_8BPP:
        if (g_FieldTileDumpPaletteFlag == 0)
            result = (pixel & 0x1f) << 10 | pixel & 0x3e0 | (pixel & 0x7c00) >> 10;
        else
            result = (pixel & 0x1f) << 11 | (pixel & 0x3e0) << 1 | (pixel & 0x7c00) >> 10;
        break;
    case TIM_PIXEL_MODE_16BPP:
        if (pixel == 0)
            result = 0;
        else if (pixel == 0x8000) {
            if (g_FieldTileDumpPaletteFlag == 0)
                result = 0x421;
            else
                result = 0x821;
        } else if (g_FieldTileDumpPaletteFlag == 0)
            result = (pixel & 0x1f) << 10 | pixel & 0x3e0 | (pixel & 0x7c00) >> 10;
        else
            result = (pixel & 0x1f) << 11 | (pixel & 0x3e0) << 1 | (pixel & 0x7c00) >> 10;
        break;
    }
    return (short)result;
}

/*
 * Returns the input narrowed to short for pixel modes other than TIM_PIXEL_MODE_16BPP. For that mode, maps zero to zero
 * and 0x8000 to 0x421 or 0x821 according to g_FieldTileDumpPaletteFlag; otherwise derives three
 * complemented component bytes and packs them with mode-dependent shifts. With the flag set, the
 * last component is an unsigned byte shifted right by ten, so that term is zero.
 */
// FUNCTION: FF7 0x00647252
short __cdecl Field_RepackPixelWordMode2(int input, int mode)
{
    unsigned char highComp;
    unsigned char compLow;
    short packed;
    unsigned char compMid;

    packed = input;
    switch (mode) {
    case TIM_PIXEL_MODE_16BPP:
        if ((input & 0xFFFF) == 0) {
            packed = 0;
        } else if ((input & 0xFFFF) == 0x8000) {
            if (g_FieldTileDumpPaletteFlag == 0) {
                packed = 0x421;
            } else {
                packed = 0x821;
            }
        } else {
            compLow = 0xFF - (input & 0xFFFF) & 0x1F;
            compMid = 0xFF - ((input & 0xFFFF & 0x3E0) >> 5);
            highComp = 0xFF - ((input & 0xFFFF & 0x7C00) >> 10);
            if (g_FieldTileDumpPaletteFlag == 0) {
                packed = compLow << 0xA | compMid << 5 | highComp;
            } else {
                packed = compLow << 0xB | compMid << 1 | highComp >> 10;
            }
        }
    }
    return (short)packed;
}

/*
 * Decodes a 15-bit pixel word into three byte outputs and returns the word.
 * The CLUT pixel modes expand each 5-bit channel to 8 bits (times 255 / 31); TIM_PIXEL_MODE_16BPP
 * writes the unscaled channels in red, blue, green order.
 */
// FUNCTION: FF7 0x00647365
unsigned short __cdecl Field_DecodePixelWordMode(unsigned short pixel, int mode, void *red, void *comp1, void *comp2)
{
    unsigned short saved = pixel;
    switch (mode) {
    case TIM_PIXEL_MODE_4BPP:
    case TIM_PIXEL_MODE_8BPP:
        *(unsigned char *)red = ((pixel & 0x1f) * 255) / 31;
        *(unsigned char *)comp1 = (((pixel & 0x3e0) >> 5) * 255) / 31;
        *(unsigned char *)comp2 = (((pixel & 0x7c00) >> 10) * 255) / 31;
        break;
    case TIM_PIXEL_MODE_16BPP:
        *(unsigned char *)red = pixel & 0x1f;
        *(unsigned char *)comp1 = (pixel & 0x7c00) >> 10;
        *(unsigned char *)comp2 = (pixel & 0x3e0) >> 5;
        break;
    }
    return saved;
}
} /* extern "C" */

extern "C" {

// clang-format off
unsigned char g_bssPad_00d00058[4] = {0};
// GLOBAL: FF7 0x00d0005c
FieldPackedColorRecord *g_FieldPackedPaletteBuffer = 0;
// GLOBAL: FF7 0x00d00060
struct FieldPackedPaletteRows g_FieldPackedPaletteRows = {0};
// GLOBAL: FF7 0x00d00064
struct FieldPackedPaletteRows g_FieldPackedPaletteOffset = {0};
unsigned char g_bssPad_00d00068[24] = {0};
// GLOBAL: FF7 0x00d00080
int g_FieldPaletteFormatFlag = 0;
// GLOBAL: FF7 0x00d00084
struct FieldPaletteGridCell *g_FieldPaletteGridBuffer = 0;
// GLOBAL: FF7 0x00d00088
unsigned short g_FieldPaletteGridHeight = 0;
// GLOBAL: FF7 0x00d0008c
struct AdPalWordPair g_AdPalWords = {0};
// The 20 palette flags: Field_DebugDumpFieldState writes frameIndex 0..19 of them, the palette loader reads them back.
// GLOBAL: FF7 0x00d00090
unsigned char g_FieldPaletteFlags[20] = {0};
// GLOBAL: FF7 0x00d000a4
unsigned short *g_FieldBrightnessLutHalf = 0;
// GLOBAL: FF7 0x00d000a8
unsigned short *g_FieldBrightnessLutQuarter = 0;
// GLOBAL: FF7 0x00d000ac
short *g_FieldBrightnessLutDoubleSaturate = 0;
// GLOBAL: FF7 0x00d000b0
short *g_FieldBrightnessLutDoubleWrap = 0;
// GLOBAL: FF7 0x00d000b4
void * g_FieldPaletteExtraRowsBuffer = 0;
// GLOBAL: FF7 0x00d000b8
int g_FieldPaletteDirtyFlag = 0;
// GLOBAL: FF7 0x00d000bc
int g_FieldPalettePendingFlag = 0;
// GLOBAL: FF7 0x00d000c0
struct FieldPalettePoolRecord *g_FieldPalettePoolBuffer = 0;
unsigned char g_bssPad_00d000c4[4] = {0};
// GLOBAL: FF7 0x00d000c8
unsigned char g_FieldHeadMeshCommands[4] = {0};
unsigned char g_bssPad_00d000cc[4] = {0};
// clang-format on

#define g_FieldPaletteGridStride (*(unsigned short *)((char *)&g_FieldPaletteGridHeight + 2))

/*
 * Allocates the global palette pool buffer: one FieldPalettePoolRecord per palette row.
 */
// FUNCTION: FF7 0x00647440
void __cdecl Field_AllocPalettePool(int count)
{
    unsigned short i;

    Field_FreeAdPalBuffer();
    if (g_FieldPalettePoolBuffer == 0) {
        // STRING: FF7 0x00907fc4
        g_FieldPalettePoolBuffer = (FieldPalettePoolRecord *)GameHeapAllocRaw(count * 0x40c, "C:\\FF7\\field\\src\\ad_pal.cpp", 166);
        if (g_FieldPalettePoolBuffer != 0) {
            for (i = 0; i < count; i++) {
                g_FieldPalettePoolBuffer[i].count = count;
                g_FieldPalettePoolBuffer[i].usedSlots = 0;
                g_FieldPalettePoolBuffer[i].capacity = 256;
            }
        }
    }
}

/*
 * Frees the global palette pool buffer if allocated.
 */
// FUNCTION: FF7 0x00647500
void __cdecl Field_FreeAdPalBuffer(void)
{
    if (g_FieldPalettePoolBuffer != 0) {
        // STRING: FF7 0x00907fe0
        GameHeapFree(g_FieldPalettePoolBuffer, "C:\\FF7\\field\\src\\ad_pal.cpp", 182);
        g_FieldPalettePoolBuffer = 0;
    }
}

/*
 * Allocates the palette work buffers, records the transparent source entries and expands the
 * source palette into the grid cells (colour word, RGB flags, mode and palette index). Two
 * field-specific dark-blue overrides and repeated flag branches are present in the original.
 */
// FUNCTION: FF7 0x00647530
int __cdecl Field_BuildPaletteRecords(unsigned short width, unsigned short height, unsigned short *source, int mode)
{
    struct {
        unsigned short count, pad0;
        unsigned short q, pad1;
        unsigned short p, pad2;
        unsigned short first, pad3;
        unsigned short unused, pad4;
    } f;
    f.count = 0;
    f.unused = 0;
    g_FieldPaletteExtraRowsBuffer = 0;
    if (source == 0)
        return 0;
    if (mode == 2) {
        g_FieldTileStateDc0 = 2;
        g_AdPalWords.second = 2;
        return 1;
    }
    g_FieldPaletteExtraRowsBuffer = GameHeapAllocRaw(1030,
                                    // STRING: FF7 0x00907ffc
                                    "C:\\FF7\\field\\src\\ad_pal.cpp", 205);
    if (g_FieldPaletteExtraRowsBuffer == 0)
        return 0;
    g_FieldTileStateDc0 = 1;
    if (g_FieldTileDumpPaletteFlag != 0)
        g_FieldPaletteFormatFlag = 1;
    else
        g_FieldPaletteFormatFlag = 0;
    g_FieldPaletteGridHeight = height;
    g_AdPalWords.first = 0;
    g_AdPalWords.second = mode;
    g_FieldPaletteGridStride = width;
    if (g_FieldPaletteGridBuffer != 0) {
        // STRING: FF7 0x00908018
        GameHeapFree(g_FieldPaletteGridBuffer, "C:\\FF7\\field\\src\\ad_pal.cpp", 220);
        g_FieldPaletteGridBuffer = 0;
    }
    if (g_FieldTileStateDbc != 0 && g_FieldReloadPending != 0) {
        Field_AllocPalettePool(height);
        for (f.p = 0; f.p < height; f.p++) {
            f.first = source[f.p * width];
            f.count = 0;
            for (f.q = 0; f.q < width; f.q++) {
                if (source[f.p * width + f.q] == 0) {
                    g_FieldPalettePoolBuffer[f.p].transparentIndices[f.count] = (int)f.q;
                    if (f.count == 0) {
                        source[f.p * width + f.q] = f.first;
                        if (f.q == 0) {
                            g_FieldPalettePoolBuffer[f.p].capacity = 256;
                        } else {
                            g_FieldPalettePoolBuffer[f.p].capacity = (int)f.q;
                        }
                    }
                    f.count++;
                }
            }
            if (f.count > 0) {
                source[f.p * width] = 0;
                g_FieldPalettePoolBuffer[f.p].usedSlots = (int)f.count;
                g_FieldPaletteFlags[f.p] = 1;
            } else {
                g_FieldPalettePoolBuffer[f.p].usedSlots = 0;
                g_FieldPaletteFlags[f.p] = 0;
            }
        }
    }
    g_FieldPaletteGridBuffer = (FieldPaletteGridCell *)GameHeapAllocRaw(height * width * 12 + 1,
                                    // STRING: FF7 0x00908034
                                    "C:\\FF7\\field\\src\\ad_pal.cpp", 256);
    if (g_FieldPaletteGridBuffer == 0)
        return 0;
    for (f.p = 0; f.p < height; f.p++) {
        for (f.q = 0; f.q < width; f.q++) {
            g_FieldPaletteGridBuffer[f.p * width + f.q].dirty = 0;
            g_FieldPaletteGridBuffer[f.p * width + f.q].clearedByte = 0;
            if ((source + f.p * width)[f.q] == 0) {
                g_FieldPaletteGridBuffer[f.p * width + f.q].mode = FIELD_PALETTE_CELL_EMPTY;
                g_FieldPaletteGridBuffer[f.p * width + f.q].pixelWord = 0;
                g_FieldPaletteGridBuffer[f.p * width + f.q].pixelFlagA = 0;
                g_FieldPaletteGridBuffer[f.p * width + f.q].pixelFlagB = 0;
                g_FieldPaletteGridBuffer[f.p * width + f.q].pixelFlagC = 0;
                g_FieldPaletteGridBuffer[f.p * width + f.q].paletteIndex = FIELD_PALETTE_INDEX_NONE;
            } else if (((source + f.p * width)[f.q] & 0x8000) != 0) {
                if (((source + f.p * width)[f.q] & 0x7fff) != 0) {
                    g_FieldPaletteGridBuffer[f.p * width + f.q].mode = FIELD_PALETTE_CELL_SEMI_TRANSPARENT;
                    g_FieldPaletteGridBuffer[f.p * width + f.q].pixelWord = Field_ConvertPixelWordMode((short)(source + f.p * width)[f.q], mode);
                    Field_DecodePixelWordMode((short)(source + f.p * width)[f.q], mode, &g_FieldPaletteGridBuffer[f.p * width + f.q].pixelFlagA, &g_FieldPaletteGridBuffer[f.p * width + f.q].pixelFlagB, &g_FieldPaletteGridBuffer[f.p * width + f.q].pixelFlagC);
                    g_FieldPaletteGridBuffer[f.p * width + f.q].paletteIndex = FIELD_PALETTE_INDEX_NONE;
                } else {
                    g_FieldPaletteGridBuffer[f.p * width + f.q].mode = FIELD_PALETTE_CELL_BLACK_SEMI_TRANSPARENT;
                    if (g_FieldTileDumpPaletteFlag == 0) {
                        g_FieldPaletteGridBuffer[f.p * width + f.q].pixelWord = 1;
                    } else {
                        g_FieldPaletteGridBuffer[f.p * width + f.q].pixelWord = 1;
                    }
                    g_FieldPaletteGridBuffer[f.p * width + f.q].paletteIndex = FIELD_PALETTE_INDEX_NONE;
                    g_FieldPaletteGridBuffer[f.p * width + f.q].pixelFlagA = 1;
                    g_FieldPaletteGridBuffer[f.p * width + f.q].pixelFlagB = 1;
                    g_FieldPaletteGridBuffer[f.p * width + f.q].pixelFlagC = 1;
                }
            } else {
                g_FieldPaletteGridBuffer[f.p * width + f.q].mode = FIELD_PALETTE_CELL_OPAQUE;
                g_FieldPaletteGridBuffer[f.p * width + f.q].pixelWord = Field_ConvertPixelWordMode((short)(source + f.p * width)[f.q], mode);
                Field_DecodePixelWordMode((short)(source + f.p * width)[f.q], mode, &g_FieldPaletteGridBuffer[f.p * width + f.q].pixelFlagA, &g_FieldPaletteGridBuffer[f.p * width + f.q].pixelFlagB, &g_FieldPaletteGridBuffer[f.p * width + f.q].pixelFlagC);
                g_FieldPaletteGridBuffer[f.p * width + f.q].paletteIndex = FIELD_PALETTE_INDEX_NONE;
                if (g_FieldCurrentMapId == 0x78 || g_FieldCurrentMapId == 0xda) {
                    if (g_FieldPaletteGridBuffer[f.p * width + f.q].pixelFlagA == 0 && g_FieldPaletteGridBuffer[f.p * width + f.q].pixelFlagB == 0 && g_FieldPaletteGridBuffer[f.p * width + f.q].pixelFlagC <= 20) {
                        g_FieldPaletteGridBuffer[f.p * width + f.q].mode = FIELD_PALETTE_CELL_BLACK_SEMI_TRANSPARENT;
                        if (g_FieldTileDumpPaletteFlag == 0) {
                            g_FieldPaletteGridBuffer[f.p * width + f.q].pixelWord = 0;
                        } else {
                            g_FieldPaletteGridBuffer[f.p * width + f.q].pixelWord = 0;
                        }
                        g_FieldPaletteGridBuffer[f.p * width + f.q].paletteIndex = FIELD_PALETTE_INDEX_NONE;
                        g_FieldPaletteGridBuffer[f.p * width + f.q].pixelFlagA = 0;
                        g_FieldPaletteGridBuffer[f.p * width + f.q].pixelFlagB = 0;
                        g_FieldPaletteGridBuffer[f.p * width + f.q].pixelFlagC = 1;
                    }
                }
            }
        }
    }
    g_FieldPaletteDirtyFlag = 0;
    return 1;
}

/*
 * Refreshes a rectangular region of the expanded palette and marks entries dirty.
 */
// FUNCTION: FF7 0x00648159
int __cdecl Field_RefreshPaletteRectEntries(int *rect, unsigned short *source, int mode)
{
    unsigned short row;
    unsigned short column;
    if (source == 0)
        return 0;
    if (mode == 2)
        return 0;
    if (g_FieldPaletteGridBuffer == 0)
        return 0;
    for (row = rect[1]; row < rect[1] + rect[3]; row++) {
        for (column = rect[0]; column < rect[0] + rect[2]; column++) {
            g_FieldPaletteGridBuffer[row * g_FieldPaletteGridStride + column].dirty = 1;
            if ((source + row * g_FieldPaletteGridStride)[column] == 0) {
                g_FieldPaletteGridBuffer[row * g_FieldPaletteGridStride + column].mode = FIELD_PALETTE_CELL_EMPTY;
                g_FieldPaletteGridBuffer[row * g_FieldPaletteGridStride + column].pixelWord = 0;
                g_FieldPaletteGridBuffer[row * g_FieldPaletteGridStride + column].pixelFlagA = 0;
                g_FieldPaletteGridBuffer[row * g_FieldPaletteGridStride + column].pixelFlagB = 0;
                g_FieldPaletteGridBuffer[row * g_FieldPaletteGridStride + column].pixelFlagC = 0;
                g_FieldPaletteGridBuffer[row * g_FieldPaletteGridStride + column].paletteIndex = FIELD_PALETTE_INDEX_NONE;
            } else if (((source + row * g_FieldPaletteGridStride)[column] & 0x8000) != 0) {
                if (((source + row * g_FieldPaletteGridStride)[column] & 0x7fff) != 0) {
                    g_FieldPaletteGridBuffer[row * g_FieldPaletteGridStride + column].mode = FIELD_PALETTE_CELL_SEMI_TRANSPARENT;
                    g_FieldPaletteGridBuffer[row * g_FieldPaletteGridStride + column].pixelWord = Field_ConvertPixelWordMode((short)(source + row * g_FieldPaletteGridStride)[column], mode);
                    Field_DecodePixelWordMode((short)(source + row * g_FieldPaletteGridStride)[column], mode, &g_FieldPaletteGridBuffer[row * g_FieldPaletteGridStride + column].pixelFlagA, &g_FieldPaletteGridBuffer[row * g_FieldPaletteGridStride + column].pixelFlagB, &g_FieldPaletteGridBuffer[row * g_FieldPaletteGridStride + column].pixelFlagC);
                    g_FieldPaletteGridBuffer[row * g_FieldPaletteGridStride + column].paletteIndex = FIELD_PALETTE_INDEX_NONE;
                } else {
                    g_FieldPaletteGridBuffer[row * g_FieldPaletteGridStride + column].mode = FIELD_PALETTE_CELL_BLACK_SEMI_TRANSPARENT;
                    if (g_FieldTileDumpPaletteFlag == 0) {
                        g_FieldPaletteGridBuffer[row * g_FieldPaletteGridStride + column].pixelWord = 1;
                    } else {
                        g_FieldPaletteGridBuffer[row * g_FieldPaletteGridStride + column].pixelWord = 1;
                    }
                    g_FieldPaletteGridBuffer[row * g_FieldPaletteGridStride + column].paletteIndex = FIELD_PALETTE_INDEX_NONE;
                    g_FieldPaletteGridBuffer[row * g_FieldPaletteGridStride + column].pixelFlagA = 1;
                    g_FieldPaletteGridBuffer[row * g_FieldPaletteGridStride + column].pixelFlagB = 1;
                    g_FieldPaletteGridBuffer[row * g_FieldPaletteGridStride + column].pixelFlagC = 1;
                }
            } else {
                g_FieldPaletteGridBuffer[row * g_FieldPaletteGridStride + column].mode = FIELD_PALETTE_CELL_OPAQUE;
                g_FieldPaletteGridBuffer[row * g_FieldPaletteGridStride + column].pixelWord = Field_ConvertPixelWordMode((short)(source + row * g_FieldPaletteGridStride)[column], mode);
                Field_DecodePixelWordMode((short)(source + row * g_FieldPaletteGridStride)[column], mode, &g_FieldPaletteGridBuffer[row * g_FieldPaletteGridStride + column].pixelFlagA, &g_FieldPaletteGridBuffer[row * g_FieldPaletteGridStride + column].pixelFlagB, &g_FieldPaletteGridBuffer[row * g_FieldPaletteGridStride + column].pixelFlagC);
                g_FieldPaletteGridBuffer[row * g_FieldPaletteGridStride + column].paletteIndex = FIELD_PALETTE_INDEX_NONE;
            }
        }
    }
    g_FieldPaletteDirtyFlag = 0;
    return 1;
}

/*
 * Builds packed-color and flag records from the source palette.
 */
// FUNCTION: FF7 0x00648828
int __cdecl Field_BuildPackedColorRecords(unsigned short columns, unsigned short rows, unsigned short *srcPalette, int mode)
{
    unsigned short row;
    unsigned short x;

    if (srcPalette == 0) {
        return 0;
    } else if (g_FieldTileDumpPaletteFlag != 0) {
        g_FieldPaletteFormatFlag = 1;
    } else {
        g_FieldPaletteFormatFlag = 0;
    }
    g_FieldPackedPaletteRows.rowsOrOffset = rows;
    g_FieldPackedPaletteOffset.rowsOrOffset = 0;
    g_FieldPackedPaletteOffset.colsOrMode = mode;
    g_FieldPackedPaletteRows.colsOrMode = columns;
    if (g_FieldPackedPaletteBuffer != 0) {
        // STRING: FF7 0x00908050
        GameHeapFree(g_FieldPackedPaletteBuffer, "C:\\FF7\\field\\src\\ad_pal.cpp", 449);
        g_FieldPackedPaletteBuffer = 0;
    }
    // STRING: FF7 0x0090806c
    g_FieldPackedPaletteBuffer = (FieldPackedColorRecord *)GameHeapAllocRaw(rows * columns * 0xC + 1, "C:\\FF7\\field\\src\\ad_pal.cpp", 453);
    if (g_FieldPackedPaletteBuffer == 0) {
        return 0;
    } else {
        for (row = 0; row < rows; row++) {
            for (x = 0; x < columns; x++) {
                if ((srcPalette + row * columns)[x] == 0) {
                    g_FieldPackedPaletteBuffer[row * columns + x].mode = FIELD_PALETTE_CELL_EMPTY;
                    g_FieldPackedPaletteBuffer[row * columns + x].pixelWord = 0;
                } else if (((srcPalette + row * columns)[x] & 0x8000) != 0) {
                    if (((srcPalette + row * columns)[x] & 1) != 0) {
                        g_FieldPackedPaletteBuffer[row * columns + x].mode = FIELD_PALETTE_CELL_SEMI_TRANSPARENT;
                        g_FieldPackedPaletteBuffer[row * columns + x].pixelWord = Field_ConvertPixelWordMode((short)(srcPalette + row * columns)[x], mode);
                    } else {
                        g_FieldPackedPaletteBuffer[row * columns + x].mode = FIELD_PALETTE_CELL_BLACK_SEMI_TRANSPARENT;
                        if (g_FieldTileDumpPaletteFlag == 0) {
                            g_FieldPackedPaletteBuffer[row * columns + x].pixelWord = 1;
                        } else {
                            g_FieldPackedPaletteBuffer[row * columns + x].pixelWord = 1;
                        }
                    }
                } else {
                    g_FieldPackedPaletteBuffer[row * columns + x].mode = FIELD_PALETTE_CELL_OPAQUE;
                    g_FieldPackedPaletteBuffer[row * columns + x].pixelWord = Field_ConvertPixelWordMode((short)(srcPalette + row * columns)[x], mode);
                }
            }
        }
        return 1;
    }
}

/*
 * Updates palette selection word if the given value is greater than or equal to current.
 */
// FUNCTION: FF7 0x00648c12
void __cdecl Field_SelectPaletteRowIfInRange(unsigned short value)
{
    if (value < g_FieldPaletteGridHeight) {
        g_AdPalWords.first = value;
    }
}

/*
 * Frees the three palette work buffers if allocated.
 */
// FUNCTION: FF7 0x00648c37
void __cdecl Field_FreePaletteWorkBuffers(void)
{
    if (g_FieldPaletteExtraRowsBuffer != 0) {
        // STRING: FF7 0x00908088
        GameHeapFree(g_FieldPaletteExtraRowsBuffer, "C:\\FF7\\field\\src\\ad_pal.cpp", 502);
        g_FieldPaletteExtraRowsBuffer = 0;
    }
    if (g_FieldPaletteGridBuffer != 0) {
        // STRING: FF7 0x009080a4
        GameHeapFree(g_FieldPaletteGridBuffer, "C:\\FF7\\field\\src\\ad_pal.cpp", 507);
        g_FieldPaletteGridBuffer = 0;
    }
    if (g_FieldPackedPaletteBuffer != 0) {
        // STRING: FF7 0x009080c0
        GameHeapFree(g_FieldPackedPaletteBuffer, "C:\\FF7\\field\\src\\ad_pal.cpp", 512);
        g_FieldPackedPaletteBuffer = 0;
    }
}

/*
 * Builds four 16-bit brightness lookup tables for palette adjustments.
 */
// FUNCTION: FF7 0x00648cbf
int __cdecl Field_BuildBrightnessTables(void)
{
    g_FieldBrightnessLutHalf = (unsigned short *)Trans_BuildScaledLut(0.5, 0x10, &g_FieldBrightnessLutFormat);
    if (g_FieldBrightnessLutHalf == 0) {
        return 0;
    }
    g_FieldBrightnessLutQuarter = (unsigned short *)Trans_BuildScaledLut(0.25, 0x10, &g_FieldBrightnessLutFormat);
    if (g_FieldBrightnessLutQuarter == 0) {
        return 0;
    }
    g_FieldBrightnessLutDoubleSaturate = (short *)Trans_BuildDoubleBrightLutSaturate(0x10, &g_FieldBrightnessLutFormat);
    if (g_FieldBrightnessLutDoubleSaturate == 0) {
        return 0;
    }
    g_FieldBrightnessLutDoubleWrap = (short *)Trans_BuildDoubleBrightLutWrapToZero(0x10, &g_FieldBrightnessLutFormat);
    if (g_FieldBrightnessLutDoubleWrap == 0) {
        return 0;
    }
    return 1;
}

/*
 * Frees the brightness lookup tables if not owned externally.
 */
// FUNCTION: FF7 0x00648d5b
void __cdecl Field_FreeBrightnessTables(void)
{
    if (g_FieldTileStateDbc != 0) {
        return;
    }
    if (g_FieldBrightnessLutHalf != 0) {
        // STRING: FF7 0x009080dc
        GameHeapFree(g_FieldBrightnessLutHalf, "C:\\FF7\\field\\src\\ad_pal.cpp", 547);
        g_FieldBrightnessLutHalf = 0;
    }
    if (g_FieldBrightnessLutQuarter != 0) {
        // STRING: FF7 0x009080f8
        GameHeapFree(g_FieldBrightnessLutQuarter, "C:\\FF7\\field\\src\\ad_pal.cpp", 552);
        g_FieldBrightnessLutQuarter = 0;
    }
    if (g_FieldBrightnessLutDoubleSaturate != 0) {
        // STRING: FF7 0x00908114
        GameHeapFree(g_FieldBrightnessLutDoubleSaturate, "C:\\FF7\\field\\src\\ad_pal.cpp", 557);
        g_FieldBrightnessLutDoubleSaturate = 0;
    }
    if (g_FieldBrightnessLutDoubleWrap != 0) {
        // STRING: FF7 0x00908130
        GameHeapFree(g_FieldBrightnessLutDoubleWrap, "C:\\FF7\\field\\src\\ad_pal.cpp", 562);
        g_FieldBrightnessLutDoubleWrap = 0;
    }
}

__declspec(dllimport) short __stdcall GetAsyncKeyState(int key);
__declspec(dllimport) short __stdcall GetKeyState(int key);

#pragma intrinsic(abs)

#pragma intrinsic(pow)
#pragma intrinsic(sqrt)

/*
 * Looks up a brightness-adjusted palette entry based on mode and delta.
 */
// FUNCTION: FF7 0x00648e1c
short __cdecl Field_LookupBrightnessPaletteEntry(short color, short delta, int mode, int rawDelta)
{
    unsigned short scaledDelta;

    if ((*(int *)&delta & 0xFFFF) == 0) {
        return (short)color;
    } else if (rawDelta != 0) {
        scaledDelta = delta;
    } else {
        scaledDelta = Field_LookupScaledPaletteValue((short)delta, mode);
    }
    if (mode == 0) {
        return g_FieldBrightnessLutHalf[*(int *)&color & 0xFFFF] + scaledDelta;
    } else if (mode == 1) {
        return (short)g_FieldBrightnessLutDoubleSaturate[g_FieldBrightnessLutHalf[*(int *)&color & 0xFFFF] + scaledDelta];
    } else if (mode == 2) {
        return g_FieldBrightnessLutDoubleWrap[g_FieldBrightnessLutHalf[*(int *)&color & 0xFFFF] - scaledDelta];
    } else if (mode == 3) {
        return (short)g_FieldBrightnessLutDoubleSaturate[g_FieldBrightnessLutHalf[*(int *)&color & 0xFFFF] + scaledDelta];
    } else {
        return 0;
    }
}

/*
 * Looks up a scaled palette value in the brightness tables.
 */
// FUNCTION: FF7 0x00648f1e
short __cdecl Field_LookupScaledPaletteValue(short color, int mode)
{
    if ((*(int *)&color & 0xFFFF) == 0) {
        return 0;
    } else if (mode == 0) {
        return *(short *)&g_FieldBrightnessLutHalf[*(int *)&color & 0xFFFF];
    } else if (mode == 1) {
        return *(short *)&g_FieldBrightnessLutHalf[*(int *)&color & 0xFFFF];
    } else if (mode == 2) {
        return *(short *)&g_FieldBrightnessLutHalf[*(int *)&color & 0xFFFF];
    } else if (mode == 3) {
        return *(short *)&g_FieldBrightnessLutHalf[g_FieldBrightnessLutQuarter[*(int *)&color & 0xFFFF]];
    } else {
        return 0;
    }
}

#pragma intrinsic(memcpy)

/*
 * Reads a rectangular palette region from memory into the destination buffer.
 */
// FUNCTION: FF7 0x00648fad
void __cdecl Field_ReadPaletteRect(FieldPaletteRect *rect, void *dst)
{
    unsigned short *pSrcBase;
    unsigned short row;
    unsigned short startRow;
    unsigned short startX;
    pSrcBase = g_pFieldTimPaletteData;
    if (rect->startColumn < 0)
        startX = 0;
    else
        startX = (unsigned short)rect->startColumn;
    if (rect->startRow < 480)
        startRow = 480;
    else
        startRow = (unsigned short)rect->startRow;
    if (startRow + rect->rowCount - 480 > g_FieldPaletteGridHeight + 2)
        return;
    for (row = startRow - 480; row < startRow + rect->rowCount - 480; row++) {
        if (row < g_FieldPaletteGridHeight)
            memcpy(dst, pSrcBase + row * (unsigned short)g_FieldTimPaletteOrgY + startX, rect->copyWidth * 2);
        else
            memcpy(dst, (char *)g_FieldPaletteExtraRowsBuffer + (row - g_FieldPaletteGridHeight) * (unsigned short)g_FieldTimPaletteOrgY + startX, rect->copyWidth * 2);
    }
}

/*
 * Writes a rectangular palette region from the source buffer and marks it dirty.
 */
// FUNCTION: FF7 0x00649111
void __cdecl Field_WritePaletteRect(FieldPaletteRect *range, void *src)
{
    struct {
        unsigned short startCol;
        unsigned short pad1;
        unsigned short rowOffset;
        unsigned short pad2;
        unsigned short *paletteData;
        unsigned short row;
        unsigned short pad3;
        FieldPaletteEntryBgra buffer[256];
    } frame;
    frame.paletteData = g_pFieldTimPaletteData;
    if (range->startColumn < 0)
        frame.startCol = 0;
    else
        frame.startCol = (unsigned short)range->startColumn;
    if (range->startRow < 480)
        frame.rowOffset = 0;
    else
        frame.rowOffset = range->startRow - 480;
    range->startRow -= 480;
    if (frame.rowOffset + range->rowCount > g_FieldPaletteGridHeight + 2)
        return;
    for (frame.row = frame.rowOffset; frame.row < frame.rowOffset + range->rowCount; frame.row++) {
        if (frame.row < g_FieldPaletteGridHeight)
            memcpy(frame.paletteData + frame.row * (unsigned short)g_FieldTimPaletteOrgY + frame.startCol, src, range->copyWidth * 2);
        else
            memcpy((char *)g_FieldPaletteExtraRowsBuffer + (frame.row - g_FieldPaletteGridHeight) * (unsigned short)g_FieldTimPaletteOrgY + frame.startCol, src, range->copyWidth * 2);
    }
    for (frame.row = (unsigned short)range->startRow; frame.row < range->startRow + range->rowCount; frame.row++) {
        Field_ConvertPaletteRowToBgra(range, frame.paletteData, frame.buffer, (short)frame.row);
        Field_UploadTexturePagePalettes(0, range->copyWidth, (BattleRgbaColor *)frame.buffer, (frame.row << 8) + range->startColumn);
    }
    g_FieldPaletteDirtyFlag = 1;
}
#pragma function(memcpy)

/*
 * Converts a 16-color palette row to 32-bit BGRA format.
 */
// FUNCTION: FF7 0x00649345
int __cdecl Field_ConvertPaletteRowToBgra(FieldPaletteRect *range, unsigned short *palette, FieldPaletteEntryBgra *dst, short row)
{
    unsigned short entry;

    if (palette == 0) {
        return 0;
    } else {
        for (entry = *(short *)&range->startColumn; entry < range->startColumn + range->copyWidth; entry++) {
            if ((palette + (*(int *)&row & 0xFFFF) * g_FieldPaletteGridStride)[entry] == 0) {
                dst[entry - range->startColumn].red = 0;
                dst[entry - range->startColumn].green = 0;
                dst[entry - range->startColumn].blue = 0;
                dst[entry - range->startColumn].alpha = 0xFF;
            } else if (((palette + (*(int *)&row & 0xFFFF) * g_FieldPaletteGridStride)[entry] & 0x8000) != 0) {
                if (((palette + (*(int *)&row & 0xFFFF) * g_FieldPaletteGridStride)[entry] & 0x7FFF) != 0) {
                    Field_DecodePixelWordMode((short)(palette + (*(int *)&row & 0xFFFF) * g_FieldPaletteGridStride)[entry], 1, &dst[entry - range->startColumn].red, &dst[entry - range->startColumn].green, &dst[entry - range->startColumn].blue);
                    dst[entry - range->startColumn].alpha = 0xFF;
                } else {
                    dst[entry - range->startColumn].alpha = 0xFF;
                    dst[entry - range->startColumn].red = 1;
                    dst[entry - range->startColumn].green = 1;
                    dst[entry - range->startColumn].blue = 1;
                }
            } else {
                Field_DecodePixelWordMode((short)(palette + (*(int *)&row & 0xFFFF) * g_FieldPaletteGridStride)[entry], 1, &dst[entry - range->startColumn].red, &dst[entry - range->startColumn].green, &dst[entry - range->startColumn].blue);
                dst[entry - range->startColumn].alpha = 0xFF;
            }
        }
        return 1;
    }
}

/*
 * Clears the dirty flag of every cell in the palette grid buffer.
 */
// FUNCTION: FF7 0x006495f7
void __cdecl Field_ClearPaletteGridDirtyFlags(void)
{
    unsigned short row;
    unsigned short column;

    if (g_FieldPaletteGridBuffer == 0) {
        return;
    }
    for (row = 0; row < g_FieldPaletteGridHeight; row++) {
        for (column = 0; column < g_FieldPaletteGridStride; column++) {
            g_FieldPaletteGridBuffer[row * g_FieldPaletteGridStride + column].dirty = 0;
        }
    }
}

/*
 * Promotes the palette dirty flag to the pending flag if active.
 */
// FUNCTION: FF7 0x00649697
void __cdecl Field_PromotePaletteDirtyFlag(void)
{
    if (g_FieldPaletteDirtyFlag == 0) {
        return;
    }
    g_FieldPalettePendingFlag = 1;
    g_FieldPaletteDirtyFlag = 0;
}

/*
 * Clears the palette pending flag.
 */
// FUNCTION: FF7 0x006496bb
void __cdecl Field_ClearPalettePendingFlag(void)
{
    g_FieldPalettePendingFlag = 0;
}

/*
 * Returns the current palette pending flag.
 */
// FUNCTION: FF7 0x006496ca
int __cdecl Field_GetPalettePendingFlag(void)
{
    return g_FieldPalettePendingFlag;
}
} /* extern "C" */
