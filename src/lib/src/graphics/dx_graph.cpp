// src/lib/src/graphics/dx_graph.cpp
// Original: C:\lib\src\graphics\dx_graph.cpp
// Contents: Selects texture and pixel formats from the device, builds color ramp tables, packs and
//   unpacks pixels, and uploads, caches and releases textures and their surfaces.
/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/types/file.h"
#include "lib/enums/file.h"
#include "lib/types/graphics.h"
#include "lib/types/polygon.h"
#include "lib/types/list.h"
#include "lib/enums/graphics.h"
#include "lib/codegen.h"
#include <string.h>
#include "lib/api/file.h"
#include "lib/api/graphics.h"
#include "lib/api/list.h"
#include "lib/api/mem.h"
#include "lib/crt.h"
#include "lib/com_abi.h"
#include <stdio.h>

#pragma intrinsic(memset)
extern "C" {
unsigned int __cdecl Blit_MapRgb24ViaRampTables(unsigned int *red, unsigned int *green, unsigned int *blue, BlitRgb24 color);
unsigned int __cdecl Blit_MapArgb8888ViaRampTables(GfxArgb32 color, GfxEngineContext *context);

// GLOBAL: FF7 0x007b8240
extern const unsigned char IID_IDirect3DTexture2[16];
// clang-format off
// GLOBAL: FF7 0x0090fbb8
unsigned char g_GfxTextureColorKeyEnabled = 0x01;
// GLOBAL: FF7 0x0090fbbc
unsigned short g_GfxTextureColorKeySubst = 1;
// clang-format off
extern "C" unsigned char g_rawData_0090fbc0[4] = {0xff, 0xff, 0x00, 0x00};
// clang-format on
// GLOBAL: FF7 0x00db2bf4
unsigned char g_GfxTextureSrcColorKeyEnabled = 0;
unsigned char g_GfxTextureBssPadA[4] = {0};
// GLOBAL: FF7 0x00db2bfc
unsigned short g_GfxTextureTransparentColor = 0;
unsigned char g_GfxTextureBssPadB[4] = {0};
// clang-format on



// GLOBAL: FF7 0x00db2c04
char g_GfxTexFileExtension = 0;

// dx_graph.cpp's literal stream in retail order: Gfx_SelectTextureFormat's two debug strings (0x0090FE2C, 0x0090FE44)
// follow the literals of the functions after it up to 0x0090FE2B. Those 18 and the two are returned by static __inline
// functions, parse-time units, in retail order; under /Ob1 the call expands to the plain operand.
static __inline char *DxGraphLit_TextureFormatCallback_1(void)
{
    // STRING: FF7 0x0090fbc4
    return "TEXTURE FORMAT CALLBACK\n";
}
static __inline char *DxGraphLit_PaletteBitsRgba_1(void)
{
    // STRING: FF7 0x0090fbe0
    return "PALETTE %d BITS %d RGBA %d:%d:%d:%d \n";
}
static __inline char *DxGraphLit_FoundTextureFormat_1(void)
{
    // STRING: FF7 0x0090fc08
    return "FOUND TEXTURE FORMAT\n";
}
static __inline char *DxGraphLit_PaletteBitsRgba_2(void)
{
    // STRING: FF7 0x0090fc20
    return "PALETTE %d BITS %d RGBA %d:%d:%d:%d\n";
}
static __inline char *DxGraphLit_InvalidTextureDataPointer_1(void)
{
    // STRING: FF7 0x0090fc48
    return "INVALID TEXTURE DATA POINTER\n";
}
static __inline char *DxGraphLit_TextureFormatCallback2(void)
{
    // STRING: FF7 0x0090fc68
    return "TEXTURE FORMAT CALLBACK 2\n";
}
static __inline char *DxGraphLit_N2PaletteBitsRgba(void)
{
    // STRING: FF7 0x0090fc84
    return "2 PALETTE %d BITS %d RGBA %d:%d:%d:%d \n";
}
static __inline char *DxGraphLit_FoundTextureFormat_2(void)
{
    // STRING: FF7 0x0090fcac
    return "FOUND TEXTURE FORMAT\n";
}
static __inline char *DxGraphLit_InvalidTextureDataPointer_2(void)
{
    // STRING: FF7 0x0090fcc4
    return "INVALID TEXTURE DATA POINTER\n";
}
static __inline char *DxGraphLit_TextureFormatCallback_2(void)
{
    // STRING: FF7 0x0090fce4
    return "TEXTURE FORMAT CALLBACK\n";
}
static __inline char *DxGraphLit_PaletteBitsRgba_3(void)
{
    // STRING: FF7 0x0090fd00
    return "PALETTE %d BITS %d RGBA %d:%d:%d:%d \n";
}
static __inline char *DxGraphLit_FoundTextureFormat_3(void)
{
    // STRING: FF7 0x0090fd28
    return "FOUND TEXTURE FORMAT\n";
}
static __inline char *DxGraphLit_PaletteBitsRgba_4(void)
{
    // STRING: FF7 0x0090fd40
    return "PALETTE %d BITS %d RGBA %d:%d:%d:%d\n";
}
static __inline char *DxGraphLit_InvalidTextureDataPointer_3(void)
{
    // STRING: FF7 0x0090fd68
    return "INVALID TEXTURE DATA POINTER\n";
}
static __inline char *DxGraphLit_TextureFormatCallback_3(void)
{
    // STRING: FF7 0x0090fd88
    return "TEXTURE FORMAT CALLBACK\n";
}
static __inline char *DxGraphLit_PaletteBitsRgba_5(void)
{
    // STRING: FF7 0x0090fda4
    return "PALETTE %d BITS %d RGBA %d:%d:%d:%d \n";
}
static __inline char *DxGraphLit_FoundTextureFormat_4(void)
{
    // STRING: FF7 0x0090fdcc
    return "FOUND TEXTURE FORMAT\n";
}
static __inline char *DxGraphLit_PaletteBitsRgba_6(void)
{
    // STRING: FF7 0x0090fde4
    return "PALETTE %d BITS %d RGBA %d:%d:%d:%d\n";
}
static __inline char *DxGraphLit_InvalidTextureDataPointer_4(void)
{
    // STRING: FF7 0x0090fe0c
    return "INVALID TEXTURE DATA POINTER\n";
}
static __inline char *DxGraphLit_FindTextureFormat(void)
{
    // STRING: FF7 0x0090fe2c
    return "FIND TEXTURE FORMAT\n";
}
static __inline char *DxGraphLit_AfterFindTextureFormat(void)
{
    // STRING: FF7 0x0090fe44
    return "AFTER FIND TEXTURE FORMAT\n";
}

/*
 * Returns the number of set bits in a 32-bit integer.
 */
// FUNCTION: FF7 0x00685da0
int __cdecl CountSetBits32(int word)
{
    int count;
    int i;
    int mask;

    mask = 1;
    count = 0;
    for (i = 0; i < 32; i++) {
        if ((word & mask) != 0) {
            count++;
        }
        mask <<= 1;
    }
    return count;
}

/*
 * Returns the index of the lowest set bit in a 32-bit integer.
 */
// FUNCTION: FF7 0x00685df0
int __cdecl LowestSetBitIndex32(int word)
{
    int idx;
    int i;
    int mask;
    mask = 1;
    idx = 0;
    for (i = 0; i < 32; i++) {
        if ((word & mask) != 0) {
            idx = i;
            break;
        }
        mask <<= 1;
    }
    return idx;
}

/*
 * Computes the bit precision lost when converting from 8-bit color channels.
 */
// FUNCTION: FF7 0x00685e3f
int __cdecl BitsLostFromEight(int bits)
{
    int lost;

    lost = 0;
    if (bits > 0) {
        lost = 8 - bits;
    }
    return lost;
}

/*
 * Fills the format's bit count, bytes per pixel and, for each of the four channel masks, its bit
 * count, the mask, its shift (the lowest set bit), the bits lost from eight and the channel
 * maximum.
 */
// FUNCTION: FF7 0x00685e62
void __cdecl PixelFormatSetRgbMasks(int rgbBitCount, int redMask, int greenMask, int blueMask, int alphaMask, GfxPixelFormat *format)
{

    format->bitsPerPixel = rgbBitCount;
    format->bytesPerPixel = (format->bitsPerPixel / 8) + (format->bitsPerPixel & 1);
    format->redBits = CountSetBits32(redMask);
    format->greenBits = CountSetBits32(greenMask);
    format->blueBits = CountSetBits32(blueMask);
    format->alphaBits = CountSetBits32(alphaMask);
    format->redMask = redMask;
    format->greenMask = greenMask;
    format->blueMask = blueMask;
    format->alphaMask = alphaMask;
    format->redShift = LowestSetBitIndex32(redMask);
    format->greenShift = LowestSetBitIndex32(greenMask);
    format->blueShift = LowestSetBitIndex32(blueMask);
    format->alphaShift = LowestSetBitIndex32(alphaMask);
    format->redLoss = BitsLostFromEight(format->redBits);
    format->greenLoss = BitsLostFromEight(format->greenBits);
    format->blueLoss = BitsLostFromEight(format->blueBits);
    format->alphaLoss = BitsLostFromEight(format->alphaBits);
    format->redMax = (1 << format->redBits) - 1;
    format->greenMax = (1 << format->greenBits) - 1;
    format->blueMax = (1 << format->blueBits) - 1;
    format->alphaMax = (1 << format->alphaBits) - 1;
}

/*
 * Fills a pixel format from a DirectDraw DDPIXELFORMAT: palettized and the index bits from the
 * DDPF_PALETTEINDEXED1/2/4/8 flags, indexedTo8 from DDPF_PALETTEINDEXEDTO8, and the channels
 * (PixelFormatSetRgbMasks) when DDPF_RGB is set.
 */
// FUNCTION: FF7 0x00685ff0
void __cdecl PixelFormatFromDDPixelFormat(GfxDdPixelFormat *ddFormat, GfxPixelFormat *format)
{
    format->palettized = 0;
    format->indexBits = 0;
    if ((ddFormat->flags & DDPF_PALETTEINDEXED1) != 0) {
        format->palettized = 1;
        format->indexBits = 1;
    }
    if ((ddFormat->flags & DDPF_PALETTEINDEXED2) != 0) {
        format->palettized = 1;
        format->indexBits = 2;
    }
    if ((ddFormat->flags & DDPF_PALETTEINDEXED4) != 0) {
        format->palettized = 1;
        format->indexBits = 4;
    }
    if ((ddFormat->flags & DDPF_PALETTEINDEXED8) != 0) {
        format->palettized = 1;
        format->indexBits = 8;
    }
    format->indexedTo8 = 0;
    if ((ddFormat->flags & DDPF_PALETTEINDEXEDTO8) != 0) {
        format->indexedTo8 = 1;
    }
    if ((ddFormat->flags & DDPF_RGB) != 0) {
        PixelFormatSetRgbMasks(ddFormat->rgbBitCount, ddFormat->redMask, ddFormat->greenMask, ddFormat->blueMask, ddFormat->alphaMask, format);
    }
}

/*
 * Fills a pixel format from a mask template: palettized, the index bits when it is palettized,
 * and the channels (PixelFormatSetRgbMasks) from the template's bit count and masks.
 */
// FUNCTION: FF7 0x006860ef
void __cdecl PixelFormatLoadMaskTemplate(GfxPixelMaskTemplate *maskTemplate, GfxPixelFormat *format)
{

    format->palettized = maskTemplate->palettized;
    if (format->palettized != 0) {
        format->indexBits = maskTemplate->bits;
    }
    PixelFormatSetRgbMasks(maskTemplate->bits, maskTemplate->redMask, maskTemplate->greenMask, maskTemplate->blueMask, maskTemplate->alphaMask, format);
}

/*
 * Packs 8-bit RGB components into the context target pixel format.
 */
// FUNCTION: FF7 0x00686143
int __cdecl PackRgbToContextPixelFormat(unsigned int red, unsigned int green, unsigned int blue)
{
    int result;
    GfxEngineContext *ctx;
    GfxPixelFormat *format;
    unsigned int alphaValue;
    result = 0;
    ctx = GetEngineContext();
    if (ctx != 0) {
        format = &ctx->targetFormat;
        if (format->palettized != 0) { /* empty first arm: lib/codegen.h */
        } else {
            result = red >> format->redLoss << format->redShift | green >> format->greenLoss << format->greenShift | blue >> format->blueLoss << format->blueShift;
            if (format->alphaBits > 0) {
                alphaValue = 0xFF;
                result |= alphaValue >> format->alphaLoss << format->alphaShift;
            }
        }
    }
    return result;
}

/*
 * Packs an ARGB8888 color value into the destination pixel format. Scales RGB channels by
 * the format's channel maxima, rounds on division by 255, and shifts into the packed pixel.
 * For a palettized format, overwrites its copy of color and returns result uninitialized.
 */
// FUNCTION: FF7 0x006861ec
unsigned int __cdecl PackArgb8888ToPixelFormat(GfxArgb32 color, GfxPixelFormat *format)
{
    unsigned int result, redBits, blueFlags, redScaled, pickedGreenBits, greenScaled8, blueProduct;
    if (format->palettized != 0) {
        color.packed = 0xffffffff;
    } else {
        redScaled = color.ch.r * format->redMax;
        redBits = redScaled / 255 + (redScaled % 255 > 127);
        greenScaled8 = color.ch.g * format->greenMax;
        pickedGreenBits = greenScaled8 / 255 + (greenScaled8 % 255 > 127);
        blueProduct = color.ch.b * format->blueMax;
        blueFlags = blueProduct / 255 + (blueProduct % 255 > 127);
        result = redBits << format->redShift | pickedGreenBits << format->greenShift | blueFlags << format->blueShift;
        if (format->alphaBits > 0)
            result |= (color.ch.a * format->alphaMax / 255) << format->alphaShift;
    }
    return result;
}

/*
 * Maps 24-bit RGB values through color ramp tables.
 */
// FUNCTION: FF7 0x0068631e
unsigned int __cdecl Blit_MapRgb24ViaRampTables(unsigned int *red, unsigned int *green, unsigned int *blue, BlitRgb24 color)
{
    return red[color.r] | green[color.g] | blue[color.b];
}

/*
 * Maps ARGB8888 color through context color ramp tables.
 */
// FUNCTION: FF7 0x00686351
unsigned int __cdecl Blit_MapArgb8888ViaRampTables(GfxArgb32 color, GfxEngineContext *context)
{
    return ((unsigned int *)context->rampTableR)[color.ch.r] | ((unsigned int *)context->rampTableG)[color.ch.g] | ((unsigned int *)context->rampTableB)[color.ch.b];
}

/*
 * Constructs gamma ramp lookup tables for color component mapping.
 */
// FUNCTION: FF7 0x00686396
void __cdecl Blit_BuildColorRampTables(unsigned int *rampR, unsigned int *rampG, unsigned int *rampB, GfxPixelFormat *format)
{
    struct {
        union {
            int rampIndex;
            unsigned char rampByte;
        };
        GfxArgb32 colorB;
        GfxArgb32 colorG;
        GfxArgb32 colorR;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    frame.colorR.packed = 0xFF000000;
    frame.colorG.packed = 0xFF000000;
    frame.colorB.packed = 0xFF000000;
    for (frame.rampIndex = 0; frame.rampIndex < 2; frame.rampIndex++) {
        rampR[frame.rampIndex] = PackArgb8888ToPixelFormat(frame.colorR, format);
        rampG[frame.rampIndex] = PackArgb8888ToPixelFormat(frame.colorG, format);
        rampB[frame.rampIndex] = PackArgb8888ToPixelFormat(frame.colorB, format);
    }
    for (frame.rampIndex = 0; frame.rampIndex < 256; frame.rampIndex++) {
        frame.colorR.ch.r = frame.rampByte;
        frame.colorG.ch.g = frame.rampByte;
        frame.colorB.ch.b = frame.rampByte;
        rampR[frame.rampIndex + 2] = PackArgb8888ToPixelFormat(frame.colorR, format);
        rampG[frame.rampIndex + 2] = PackArgb8888ToPixelFormat(frame.colorG, format);
        rampB[frame.rampIndex + 2] = PackArgb8888ToPixelFormat(frame.colorB, format);
    }
    for (frame.rampIndex = 0; frame.rampIndex < 2; frame.rampIndex++) {
        frame.colorR.ch.r = 0xFF;
        frame.colorG.ch.g = 0xFF;
        frame.colorB.ch.b = 0xFF;
        rampR[frame.rampIndex + 0x102] = PackArgb8888ToPixelFormat(frame.colorR, format);
        rampG[frame.rampIndex + 0x102] = PackArgb8888ToPixelFormat(frame.colorG, format);
        rampB[frame.rampIndex + 0x102] = PackArgb8888ToPixelFormat(frame.colorB, format);
    }
}

/*
 * Unpacks a pixel in context format to 32-bit ARGB8888 representation.
 */
// FUNCTION: FF7 0x00686514
int __cdecl UnpackPixelToArgb8888(unsigned int pixel, GfxPixelFormat *format)
{
    union {
        int argb;
        GfxArgb32 color;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    if (format->palettized != 0) {
        frame.argb = 0xFFFF0000;
        if ((unsigned int)pixel < (unsigned int)format->paletteEntryCount && format->palette != 0) {
            frame.argb = format->paletteEntries[pixel];
        }
    } else {
        frame.color.ch.r = (unsigned int)(((pixel & format->redMask) >> format->redShift) * 255) / (unsigned int)format->redMax;
        frame.color.ch.g = (unsigned int)(((pixel & format->greenMask) >> format->greenShift) * 255) / (unsigned int)format->greenMax;
        frame.color.ch.b = (unsigned int)(((pixel & format->blueMask) >> format->blueShift) * 255) / (unsigned int)format->blueMax;
        if (format->alphaBits > 0) {
            frame.color.ch.a = (unsigned int)(((pixel & format->alphaMask) >> format->alphaShift) * 255) / (unsigned int)format->alphaMax;
        } else {
            frame.color.ch.a = 0xFF;
        }
    }
    return frame.argb;
}

/*
 * Unpacks a row pixel value into 32-bit ARGB8888 representation.
 */
// FUNCTION: FF7 0x006865f0
int __cdecl UnpackPixelRowToArgb8888(unsigned int pixelIndex, int row, GfxPixelFormat *format)
{
    union {
        int argb;
        GfxArgb32 color;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    if (format->palettized != 0) {
        frame.argb = 0xFFFF0000;
        if (format->palette != 0) {
            pixelIndex += row * format->entriesPerPalette;
            if ((unsigned int)pixelIndex < (unsigned int)format->paletteEntryCount) {
                frame.argb = format->paletteEntries[pixelIndex];
            }
        }
    } else {
        frame.color.ch.r = (unsigned int)(((pixelIndex & format->redMask) >> format->redShift) * 255) / (unsigned int)format->redMax;
        frame.color.ch.g = (unsigned int)(((pixelIndex & format->greenMask) >> format->greenShift) * 255) / (unsigned int)format->greenMax;
        frame.color.ch.b = (unsigned int)(((pixelIndex & format->blueMask) >> format->blueShift) * 255) / (unsigned int)format->blueMax;
        if (format->alphaBits > 0) {
            frame.color.ch.a = (unsigned int)(((pixelIndex & format->alphaMask) >> format->alphaShift) * 255) / (unsigned int)format->alphaMax;
        } else {
            frame.color.ch.a = 0xFF;
        }
    }
    return frame.argb;
}

/*
 * Enumerates and selects the optimal texture pixel format for device.
 */
// FUNCTION: FF7 0x006866de
int __cdecl Gfx_SelectTextureFormat(int *flagsPtr, GfxTexHeader *formatDesc, GfxEngineContext *target)
{
    int scratch[5];
    int bits;
    int probeHandle;
    GfxTextureFormatProbe *probeCtx;
    List *list;
    int res;
    int palettized;

    bits = 0;
    if (flagsPtr != 0) {
        bits = *flagsPtr;
    }
    res = 0;
    probeCtx = (GfxTextureFormatProbe *)(&scratch);
    if (DEBUG_TRACE != 0) {
        Gfx_DebugPrint(DxGraphLit_FindTextureFormat());
    }
    if (target != 0) {
        list = (List *)(target->textureFormatList);
        if (list != 0) {
            ZeroFillMemory32(20, probeCtx);
            probeCtx->formatDesc = formatDesc;
            probeHandle = (int)probeCtx;
            if ((bits & GFX_TEXFORMAT_SEARCH_PALETTIZED) != 0) {
                ListForEachValueRecursive((ListVisitFn)Gfx_TextureFormatEnumPalettizedCallback, (int)(&probeHandle), list);
            }
            if (probeCtx->formatFound == 0 && (bits & GFX_TEXFORMAT_SEARCH_RANGE_FALLBACK) != 0) {
                ZeroFillMemory32(20, probeCtx);
                probeCtx->formatDesc = formatDesc;
                ListForEachValueRecursive((ListVisitFn)Gfx_TextureFormatEnumRangeFallbackCallback, (int)(&probeHandle), list);
            }
            if (probeCtx->formatFound == 0 && (bits & GFX_TEXFORMAT_SEARCH_CURRENT_MODE) != 0) {
                ZeroFillMemory32(20, probeCtx);
                probeCtx->formatDesc = formatDesc;
                ListForEachValueRecursive((ListVisitFn)Gfx_TextureFormatEnumCurrentModeCallback, (int)(&probeHandle), list);
            }
            if (probeCtx->formatFound == 0) {
                palettized = 0;
                if (formatDesc->paletteCount > 1) {
                    palettized = 1;
                }
                palettized = 1;
                if (palettized != 0) {
                    ListForEachValueRecursive((ListVisitFn)Gfx_TextureFormatEnumPalettizedCallback, (int)(&probeHandle), list);
                }
                if (REG_ALLOC_HINT) {
                }
                if (palettized != 0 && probeCtx->formatFound != 0) {
                    res = probeCtx->selectedFormat;
                } else {
                    ZeroFillMemory32(20, probeCtx);
                    probeCtx->formatDesc = formatDesc;
                    ListForEachValueRecursive((ListVisitFn)Gfx_TextureFormatEnumRangeFallbackCallback, (int)(&probeHandle), list);
                    if (probeCtx->formatFound != 0) {
                        res = probeCtx->selectedFormat;
                    } else {
                        res = probeCtx->result;
                        ListForEachValueRecursive((ListVisitFn)Gfx_TextureFormatEnumCurrentModeCallback, (int)(&probeHandle), list);
                        if (probeCtx->formatFound != 0) {
                            res = probeCtx->selectedFormat;
                        } else {
                            ZeroFillMemory32(20, probeCtx);
                            probeCtx->formatDesc = formatDesc;
                            ListForEachValueRecursive((ListVisitFn)Gfx_TextureFormatEnumRangeCallback, (int)(&probeHandle), list);
                            if (probeCtx->formatFound != 0) {
                                res = probeCtx->selectedFormat;
                            }
                        }
                    }
                }
            } else {
                res = probeCtx->selectedFormat;
            }
        }
    }
    if (DEBUG_TRACE != 0) {
        Gfx_DebugPrint(DxGraphLit_AfterFindTextureFormat());
    }
    return res;
}

/*
 * Callback for enumerating texture formats with range fallback criteria.
 */
// FUNCTION: FF7 0x00686924
int __cdecl Gfx_TextureFormatEnumRangeFallbackCallback(GfxTextureFormatRecord *candidate, GfxTextureFormatProbe **stateRef)
{
    struct {
        char buf[200];
        GfxTexHeader *formatDesc;
        GfxTextureFormatProbe *probe;
        int continueEnum;
    } local; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    local.continueEnum = 0;
    if (candidate != 0) {
        local.continueEnum = 1;
        local.probe = *stateRef;
        (local.probe->count)++;
        local.formatDesc = local.probe->formatDesc;
        if (local.formatDesc != 0) {
            if (DEBUG_TRACE) {
                Gfx_DebugPrint(DxGraphLit_TextureFormatCallback_1());
                sprintf(local.buf, DxGraphLit_PaletteBitsRgba_1(), candidate->format.palettized, candidate->format.bitsPerPixel, candidate->format.redBits, candidate->format.greenBits, candidate->format.blueBits, candidate->format.alphaBits);
                Gfx_DebugPrint(local.buf);
            }
            if (local.formatDesc->unusedFormatFlag != 0) { /* empty first arm: lib/codegen.h */
            } else {
                if (candidate->format.palettized == 0) {
                    local.probe->result = (int)candidate;
                }
                if (candidate->format.bitsPerPixel >= local.formatDesc->minBitsPerPixel && candidate->format.bitsPerPixel <= local.formatDesc->maxBitsPerPixel && candidate->format.greenBits >= local.formatDesc->minColorBits && candidate->format.greenBits <= local.formatDesc->maxColorBits && candidate->format.alphaBits >= local.formatDesc->minAlphaBits && candidate->format.alphaBits <= local.formatDesc->maxAlphaBits) {
                    local.probe->formatFound = 1;
                    local.probe->selectedFormat = (int)candidate;
                    local.continueEnum = 0;
                    if (DEBUG_TRACE) {
                        Gfx_DebugPrint(DxGraphLit_FoundTextureFormat_1());
                    }
                    if (DEBUG_TRACE) {
                        sprintf(local.buf, DxGraphLit_PaletteBitsRgba_2(), candidate->format.palettized, candidate->format.bitsPerPixel, candidate->format.redBits, candidate->format.greenBits, candidate->format.blueBits, candidate->format.alphaBits);
                        Gfx_DebugPrint(local.buf);
                    }
                }
            }
        } else {
            Gfx_DebugPrint(DxGraphLit_InvalidTextureDataPointer_1());
        }
    }
    return local.continueEnum;
}

/*
 * Callback for enumerating texture formats matching current display mode.
 */
// FUNCTION: FF7 0x00686aec
int __cdecl Gfx_TextureFormatEnumCurrentModeCallback(GfxTextureFormatRecord *candidate, GfxTextureFormatProbe **stateRef)
{
    struct {
        char buf[200];
        GfxTexHeader *format;
        GfxTextureFormatProbe *state;
        int result;
        GfxEngineContext *engine;
    } local; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    local.engine = GetEngineContext();
    local.result = 0;
    if (local.engine != 0 && candidate != 0) {
        local.result = 1;
        local.state = *stateRef;
        if (DEBUG_TRACE != 0) {
            Gfx_DebugPrint(
                                       DxGraphLit_TextureFormatCallback2());
        }
        (local.state->count)++;
        local.format = local.state->formatDesc;
        if (local.format != 0) {
            if (DEBUG_TRACE != 0) {
                sprintf(local.buf,
                        DxGraphLit_N2PaletteBitsRgba(), candidate->format.palettized, candidate->format.bitsPerPixel, candidate->format.redBits, candidate->format.greenBits, candidate->format.blueBits, candidate->format.alphaBits);
                Gfx_DebugPrint(local.buf);
            }
            if (local.format->unusedFormatFlag == 0 || local.engine->fallbackFlag != 0) {
                if (REG_ALLOC_HINT) {
                }
                if (candidate->format.bitsPerPixel >= local.format->minBitsPerPixel && candidate->format.bitsPerPixel == local.engine->colorDepth) {
                    local.state->result = (int)candidate;
                    if (candidate->format.greenBits >= local.format->minColorBits && candidate->format.greenBits <= local.format->maxColorBits && local.engine->targetFormat.redBits == candidate->format.redBits && local.engine->targetFormat.greenBits == candidate->format.greenBits && local.engine->targetFormat.blueBits == candidate->format.blueBits) {
                        local.state->formatFound = 1;
                        local.state->selectedFormat = (int)candidate;
                        local.result = 0;
                        if (DEBUG_TRACE != 0) {
                            Gfx_DebugPrint(
                                                       DxGraphLit_FoundTextureFormat_2());
                        }
                    }
                }
            }
        } else {
            Gfx_DebugPrint(
                                       DxGraphLit_InvalidTextureDataPointer_2());
        }
    }
    return local.result;
}

/*
 * Callback for enumerating texture formats matching specified bit range.
 */
// FUNCTION: FF7 0x00686c89
int __cdecl Gfx_TextureFormatEnumRangeCallback(GfxTextureFormatRecord *candidate, GfxTextureFormatProbe **stateRef)
{
    struct {
        char buf[200];
        GfxTexHeader *formatDesc;
        GfxTextureFormatProbe *probe;
        int continueEnum;
    } local; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    local.continueEnum = 0;
    if (candidate != 0) {
        local.continueEnum = 1;
        local.probe = *stateRef;
        (local.probe->count)++;
        local.formatDesc = local.probe->formatDesc;
        if (local.formatDesc != 0) {
            if (DEBUG_TRACE) {
                Gfx_DebugPrint(DxGraphLit_TextureFormatCallback_2());
                sprintf(local.buf, DxGraphLit_PaletteBitsRgba_3(), candidate->format.palettized, candidate->format.bitsPerPixel, candidate->format.redBits, candidate->format.greenBits, candidate->format.blueBits, candidate->format.alphaBits);
                Gfx_DebugPrint(local.buf);
            }
            if (candidate->format.bitsPerPixel >= local.formatDesc->minBitsPerPixel && candidate->format.bitsPerPixel <= local.formatDesc->maxBitsPerPixel && candidate->format.greenBits >= local.formatDesc->minColorBits && candidate->format.greenBits <= local.formatDesc->maxColorBits && candidate->format.alphaBits >= local.formatDesc->minAlphaBits && candidate->format.alphaBits <= local.formatDesc->maxAlphaBits) {
                local.probe->formatFound = 1;
                local.probe->selectedFormat = (int)candidate;
                local.continueEnum = 0;
                if (DEBUG_TRACE) {
                    Gfx_DebugPrint(DxGraphLit_FoundTextureFormat_3());
                }
                if (DEBUG_TRACE) {
                    sprintf(local.buf, DxGraphLit_PaletteBitsRgba_4(), candidate->format.palettized, candidate->format.bitsPerPixel, candidate->format.redBits, candidate->format.greenBits, candidate->format.blueBits, candidate->format.alphaBits);
                    Gfx_DebugPrint(local.buf);
                }
            }
        } else {
            Gfx_DebugPrint(DxGraphLit_InvalidTextureDataPointer_3());
        }
    }
    return local.continueEnum;
}

/*
 * Callback for enumerating palettized texture formats.
 */
// FUNCTION: FF7 0x00686e31
int __cdecl Gfx_TextureFormatEnumPalettizedCallback(GfxTextureFormatRecord *candidate, GfxTextureFormatProbe **stateRef)
{
    struct {
        char buf[200];
        GfxTexHeader *formatDesc;
        GfxTextureFormatProbe *probe;
        int continueEnum;
    } local; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    local.continueEnum = 0;
    if (candidate != 0) {
        local.continueEnum = 1;
        local.probe = *stateRef;
        (local.probe->count)++;
        local.formatDesc = local.probe->formatDesc;
        if (local.formatDesc != 0) {
            if (DEBUG_TRACE) {
                Gfx_DebugPrint(DxGraphLit_TextureFormatCallback_3());
                sprintf(local.buf, DxGraphLit_PaletteBitsRgba_5(), candidate->format.palettized, candidate->format.bitsPerPixel, candidate->format.redBits, candidate->format.greenBits, candidate->format.blueBits, candidate->format.alphaBits);
                Gfx_DebugPrint(local.buf);
            }
            if (candidate->format.palettized != 0 && local.formatDesc->bitsPerPixel <= candidate->format.bitsPerPixel) {
                local.probe->formatFound = 1;
                local.probe->selectedFormat = (int)candidate;
                local.continueEnum = 0;
                if (DEBUG_TRACE) {
                    Gfx_DebugPrint(DxGraphLit_FoundTextureFormat_4());
                }
                if (DEBUG_TRACE) {
                    sprintf(local.buf, DxGraphLit_PaletteBitsRgba_6(), candidate->format.palettized, candidate->format.bitsPerPixel, candidate->format.redBits, candidate->format.greenBits, candidate->format.blueBits, candidate->format.alphaBits);
                    Gfx_DebugPrint(local.buf);
                }
            }
        } else {
            Gfx_DebugPrint(DxGraphLit_InvalidTextureDataPointer_4());
        }
    }
    return local.continueEnum;
}

/*
 * Frees the texture's palette record and clears the palette member.
 */
// FUNCTION: FF7 0x00686f8c
void __cdecl Gfx_ReleaseTexturePalette(GfxTextureSurface *texture)
{
    if (texture != 0) {
        Gfx_FreePaletteRecord(texture->paletteRecord);
        texture->palette = 0;
    }
}

/*
 * Creates and attaches a DirectDraw palette to a surface record.
 */
// FUNCTION: FF7 0x00686fb6
int __cdecl Gfx_CreateSurfacePaletteRecord(int bitDepth, GfxTexHeader *params, GfxTextureSurface *texture)
{
    GfxPaletteRecord *palette;

    palette = 0;
    if (params->format.paletteEntryCount > 0) {
        palette = (GfxPaletteRecord *)(Gfx_CreatePaletteRecord(params->paletteFlags, bitDepth, params->entriesPerPalette, params->format.paletteEntryCount, (int)params->format.palette));
        if (palette != 0) {
            palette->paletteMode = params->paletteMode;
            palette->colorLookup = params->colorLookup;
            texture->palette = (int)palette;
        }
    }
    return (int)palette;
}

/*
 * Converts texture pixel data into surface pixel format.
 */
// FUNCTION: FF7 0x0068702c
void __cdecl Gfx_ConvertTexturePixelsToSurface(GfxTexHeader *params, GfxPixelFormat *pixelFormat, int dest)
{
    struct {
        int argb32, argb24, argb16, argb8, liftOpaqueBlack;
        union {
            int dstAddr;
            short *dst16;
            int *dst32;
            GlTexelDest *dstTexel;
        };
        GfxArgb32 rgb;
        union {
            int packedPixel;
            char packedByte;
            short packedShort;
        };
        int row, col;
        union {
            int srcAddr;
            unsigned short *src16;
            int *src32;
            GfxTexelSource *srcTexel;
        };
        GfxArgb32 color;
        int dstRow;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    if (params != 0) {
        frame.srcAddr = params->pixelBuffer;
        frame.dstRow = dest;
        if (params->format.entriesPerPalette == 0) {
            params->format.entriesPerPalette = params->entriesPerPalette;
        }
        for (frame.row = 0; frame.row < params->format.height; frame.row++) {
            frame.dstAddr = frame.dstRow;
            for (frame.col = 0; frame.col < params->format.width; frame.col++) {
                /* Numeric pixel stride in bytes (1..4). */
                switch (params->format.bytesPerPixel) {
                case GFX_BYTES_PER_PIXEL_1:
                    frame.argb8 = UnpackPixelRowToArgb8888(frame.srcTexel->blue, params->row, &params->format);
                    frame.color.packed = frame.argb8;
                    break;
                case GFX_BYTES_PER_PIXEL_2:
                    frame.argb16 = UnpackPixelToArgb8888(*frame.src16, &params->format);
                    frame.color.packed = frame.argb16;
                    break;
                case GFX_BYTES_PER_PIXEL_3:
                    frame.rgb.ch.b = frame.srcTexel->blue;
                    frame.rgb.ch.g = frame.srcTexel->green;
                    frame.rgb.ch.r = frame.srcTexel->red;
                    frame.rgb.ch.a = 0;
                    frame.argb24 = UnpackPixelToArgb8888(frame.rgb.packed, &params->format);
                    frame.color.packed = frame.argb24;
                    break;
                case GFX_BYTES_PER_PIXEL_4:
                    frame.argb32 = UnpackPixelToArgb8888(*frame.src32, &params->format);
                    frame.color.packed = frame.argb32;
                }
                if (frame.color.ch.a != 0) {
                    frame.liftOpaqueBlack = params->modeFlag == 0;
                    if (frame.color.ch.a == 0xFE && params->alphaValue >= 0) {
                        frame.color.ch.a = params->alphaByte;
                    }
                    if (REG_ALLOC_HINT) {
                    }
                    if (frame.liftOpaqueBlack != 0) {
                        if (frame.color.ch.r < 1 && frame.color.ch.g < 1 && frame.color.ch.b < 1) {
                            frame.color.ch.r = 1;
                            frame.color.ch.g = 1;
                            frame.color.ch.b = 1;
                        }
                    }
                }
                frame.packedPixel = PackArgb8888ToPixelFormat(frame.color, pixelFormat);
                /* Numeric pixel stride in bytes (1..4). */
                switch (pixelFormat->bytesPerPixel) {
                case GFX_BYTES_PER_PIXEL_1:
                    frame.dstTexel->blue = frame.packedByte;
                    break;
                case GFX_BYTES_PER_PIXEL_2:
                    *frame.dst16 = frame.packedShort;
                    break;
                case GFX_BYTES_PER_PIXEL_3:
                    frame.dstTexel->blue = frame.color.ch.b;
                    frame.dstTexel->green = frame.color.ch.g;
                    frame.dstTexel->red = frame.color.ch.r;
                    break;
                case GFX_BYTES_PER_PIXEL_4:
                    *frame.dst32 = frame.packedPixel;
                }
                frame.srcAddr += params->format.bytesPerPixel;
                frame.dstAddr += pixelFormat->bytesPerPixel;
            }
            frame.dstRow += pixelFormat->pitch;
        }
    }
}

/*
 * Uploads converted texture pixel data to DirectDraw surface.
 */
// FUNCTION: FF7 0x006872bf
void __cdecl GfxUploadTexturePixels(GfxTexHeader *params, GfxTextureSurfaceDesc *texture, int ddSurface)
{
#pragma pack(push, 1)
    union {
        unsigned char storage[1056];
        int rowArgb;
        struct { unsigned char pad_to_4[0x4]; int frameIndex; };
        struct {
            unsigned char pad_to_8[0x8];
            union {
                int destPixel;
                short *destPixel16;
            };
        };
        struct { unsigned char pad_to_C[0xC]; int colorKey; };
        struct { unsigned char pad_to_10[0x10]; int lookupRow; };
        struct { unsigned char pad_to_14[0x14]; int dstRowIndex; };
        struct { unsigned char pad_to_18[0x18]; int dstColIndex; };
        struct {
            unsigned char pad_to_1C[0x1C];
            union {
                short palette16[256];
                unsigned short palette16u[256];
            };
        };
        struct { unsigned char pad_to_21C[0x21C]; int entryCount; };
        struct {
            unsigned char pad_to_220[0x220];
            union {
                int indexCursor;
                unsigned char *indexBytes;
            };
        };
        struct {
            unsigned char pad_to_224[0x224];
            union {
                int argbColor;
                GfxArgb32 argb;
            };
        };
        struct { unsigned char pad_to_227[0x227]; int alphaByte; };
        struct { unsigned char pad_to_228[0x228]; int destRow; };
        struct { unsigned char pad_to_22C[0x22C]; char packedByte; };
        struct { unsigned char pad_to_230[0x230]; short nibblePair; };
        struct { unsigned char pad_to_234[0x234]; int convertBits; };
        struct {
            unsigned char pad_to_238[0x238];
            union {
                int destCursor;
                char *destBytes;
            };
        };
        struct {
            unsigned char pad_to_23C[0x23C];
            union {
                int srcCursor;
                char *srcBytes;
                short *srcShorts;
            };
        };
        struct { unsigned char pad_to_240[0x240]; int surfaceRow; };
        struct { unsigned char pad_to_244[0x244]; int debugFlag; };
        struct { unsigned char pad_to_248[0x248]; GfxDdSurfaceDesc surfaceDesc; };
        struct { unsigned char pad_to_2B4[0x2B4]; int createdSurface; };
        struct { unsigned char pad_to_2B8[0x2B8]; int tmpDdPtr; };
        struct { unsigned char pad_to_2BC[0x2BC]; int paramWidth; };
        struct { unsigned char pad_to_2C0[0x2C0]; int paramRows; };
        struct { unsigned char pad_to_2C4[0x2C4]; int zeroArg; };
        struct { unsigned char pad_to_2C8[0x2C8]; int rowIndex; };
        struct { unsigned char pad_to_2CC[0x2CC]; int colIndex; };
        struct { unsigned char pad_to_2D0[0x2D0]; GfxPixelFormat format; };
        struct { unsigned char pad_to_350[0x350]; GfxEngineContext *engineCtx; };
        struct { unsigned char pad_to_354[0x354]; char msgBuf[200]; };
        struct { unsigned char pad_to_41C[0x41C]; GfxDdSurfaceDesc *surfaceDescPtr; };
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
#pragma pack(pop)

    frame.debugFlag = 0;
    frame.createdSurface = 0;
    frame.tmpDdPtr = 0;
    frame.paramWidth = params->format.width;
    frame.paramRows = params->format.height;
    frame.zeroArg = 0;
    frame.surfaceDescPtr = &frame.surfaceDesc;
    memset(frame.surfaceDescPtr, 0, 0x6C);
    frame.surfaceDescPtr->size = 108;
    // STRING: FF7 0x0090fe60
    if (CheckDirectDrawResult(((ComDirectDrawSurface *)(ddSurface))->Lock(&frame.createdSurface, frame.surfaceDescPtr, DDLOCK_WAIT, frame.zeroArg), "C:\\lib\\src\\graphics\\dx_graph.cpp", 1128) != 0) {
        ZeroFillMemory32(sizeof(frame.format), &frame.format);
        frame.format.width = params->format.width;
        frame.format.height = params->format.height;
        frame.format.pitch = frame.surfaceDescPtr->pitch;
        PixelFormatFromDDPixelFormat((GfxDdPixelFormat *)&texture->pixelFormatSize, &frame.format);
        frame.srcCursor = params->pixelBuffer;
        frame.surfaceRow = frame.surfaceDescPtr->surface;
        if (frame.debugFlag != 0) {
            // STRING: FF7 0x0090fe84
            sprintf(frame.msgBuf, "TEXTURE WIDTH %d HEIGHT %d\n", frame.format.width, frame.format.height);
            Gfx_DebugPrint(frame.msgBuf);
        }
        frame.engineCtx = GetEngineContext();
        if (frame.engineCtx->fallbackFlag != 0) {
            frame.convertBits = 0;
        } else {
            frame.convertBits = frame.format.palettized;
        }
        if (frame.convertBits != 0) {
            /* Numeric colour depth in bits per pixel. */
            switch (frame.format.bitsPerPixel) {
            case GFX_BPP_4:
                for (frame.rowIndex = 0; frame.rowIndex < frame.format.height; frame.rowIndex++) {
                    frame.destCursor = frame.surfaceRow;
                    for (frame.colIndex = 0; frame.colIndex < frame.format.width; frame.colIndex += 2) {
                        frame.nibblePair = *frame.srcShorts;
                        /* Retail caller prototype (short) -> char preserved for /Od argument promotion on `Gfx_PackNibblePairToByte`. */
                        frame.packedByte = ((char(__cdecl *)(short))Gfx_PackNibblePairToByte)(frame.nibblePair);
                        *frame.destBytes = frame.packedByte;
                        frame.srcCursor += 2;
                        frame.destCursor++;
                    }
                    frame.surfaceRow += frame.format.pitch;
                }
                break;
            case GFX_BPP_8:
                for (frame.rowIndex = 0; frame.rowIndex < frame.format.height; frame.rowIndex++) {
                    frame.destCursor = frame.surfaceRow;
                    for (frame.colIndex = 0; frame.colIndex < frame.format.width; frame.colIndex++) {
                        *frame.destBytes = *frame.srcBytes;
                        frame.srcCursor += params->format.bytesPerPixel;
                        frame.destCursor += frame.format.bytesPerPixel;
                    }
                    frame.surfaceRow += frame.format.pitch;
                }
            }
        } else if (frame.engineCtx->fallbackFlag != 0 && params->format.bytesPerPixel == 1) {
            frame.entryCount = 256;
            frame.colorKey = 0;
            frame.lookupRow = params->row;
            if (params != 0) {
                frame.indexCursor = params->pixelBuffer;
                frame.destRow = frame.surfaceRow;
                frame.colorKey = params->colorKey;
                if (params->paletteMode == 1) {
                    frame.colorKey = params->colorLookupBytes[frame.lookupRow];
                }
                if (params->format.entriesPerPalette == 0) {
                    params->format.entriesPerPalette = params->entriesPerPalette;
                }
                for (frame.frameIndex = 0; frame.frameIndex < params->format.entriesPerPalette; frame.frameIndex++) {
                    frame.rowArgb = UnpackPixelRowToArgb8888(frame.frameIndex, params->row, &params->format);
                    frame.argbColor = frame.rowArgb;
                    if ((frame.alphaByte & 0xFF) != 0 && (frame.alphaByte & 0xFF) == 0xFE && params->alphaValue >= 0) {
                        frame.argb.ch.a = params->alphaByte;
                    }
                    frame.palette16[frame.frameIndex] = PackArgb8888ToPixelFormat(frame.argb, &frame.format);
                    if (frame.palette16u[frame.frameIndex] == g_GfxTextureTransparentColor && (frame.frameIndex != 0 || frame.colorKey == 0)) {
                        frame.palette16[frame.frameIndex] = g_GfxTextureColorKeySubst;
                    }
                }
                if (frame.colorKey != 0) {
                    frame.palette16u[0] = g_GfxTextureTransparentColor;
                }
                for (frame.dstRowIndex = 0; frame.dstRowIndex < params->format.height; frame.dstRowIndex++) {
                    frame.destPixel = frame.destRow;
                    for (frame.dstColIndex = 0; frame.dstColIndex < params->format.width; frame.dstColIndex++) {
                        *frame.destPixel16 = frame.palette16[*frame.indexBytes];
                        frame.indexCursor += params->format.bytesPerPixel;
                        frame.destPixel += frame.format.bytesPerPixel;
                    }
                    frame.destRow += frame.format.pitch;
                }
            }
        } else {
            Gfx_ConvertTexturePixelsToSurface(params, &frame.format, frame.surfaceRow);
        }
        // STRING: FF7 0x0090fea0
        CheckDirectDrawResult(((ComDirectDrawSurface *)(ddSurface))->Unlock(frame.surfaceDescPtr->surface), "C:\\lib\\src\\graphics\\dx_graph.cpp", 1338);
    }
}

/*
 * Packs two 4-bit nibbles into a single byte.
 */
// FUNCTION: FF7 0x006878c3
char __cdecl Gfx_PackNibblePairToByte(int value)
{
    unsigned char packed;

    packed = value & 0xFFFF & 0xF | (value & 0xFFFF & 0xF00) >> 4;
    return (char)packed;
}

/*
 * Releases all surfaces and COM interfaces held by a texture record.
 */
// FUNCTION: FF7 0x006878f0
void __cdecl Gfx_ReleaseTextureSurfacesAndObject(GfxTextureSurface *texture)
{
    if (texture != 0) {
        if (texture->surface != 0) {
            ((ComUnknown *)(texture->surface))->Release();
            texture->surface = 0;
        }
        if (texture->d3dTexture != 0) {
            ((ComUnknown *)(texture->d3dTexture))->Release();
        }
        texture->d3dTexture = 0;
        if (texture->sourceSurface != 0) {
            ((ComUnknown *)(texture->sourceSurface))->Release();
            texture->sourceSurface = 0;
        }
        if (texture->sourceTexture != 0) {
            ((ComUnknown *)(texture->sourceTexture))->Release();
            texture->sourceTexture = 0;
        }
        Gfx_ReleaseTexturePalette(texture);
    }
}

/*
 * Releases DirectDraw and Direct3D surface objects in a texture record.
 */
// FUNCTION: FF7 0x0068799b
void __cdecl GfxReleaseTextureSurfaces(GfxTextureSurface *texture)
{
    if (texture != 0) {
        if (texture->surface != 0) {
            ((ComUnknown *)(texture->surface))->Release();
            texture->surface = 0;
        }
        if (texture->d3dTexture != 0) {
            ((ComUnknown *)(texture->d3dTexture))->Release();
        }
        texture->d3dTexture = 0;
        if (texture->sourceSurface != 0) {
            ((ComUnknown *)(texture->sourceSurface))->Release();
            texture->sourceSurface = 0;
        }
        if (texture->sourceTexture != 0) {
            ((ComUnknown *)(texture->sourceTexture))->Release();
            texture->sourceTexture = 0;
        }
    }
}

/*
 * Frees allocated memory and buffers for a texture record.
 */
// FUNCTION: FF7 0x00687a3a
void __cdecl Gfx_FreeTextureRecord(GfxTextureSurface *texture)
{
    GfxEngineContext *engine;
    char released;
    GfxDriverOps *driverObject;

    if (texture != 0) {
        engine = GetEngineContext();
        driverObject = GetGraphicsDriverObject(engine);
        if (engine->fallbackFlag != 0) {
            released = Gfx_TextureCacheReleaseByTexture(texture->texHeader);
        }
        driverObject->unloadTexture(texture);
        Gfx_ReleaseTexturePalette(texture);
        // STRING: FF7 0x0090fec4
        GameHeapFree(texture, "C:\\lib\\src\\graphics\\dx_graph.cpp", 1448);
    }
}

/*
 * Decrements reference count on a texture record and frees when zero.
 */
// FUNCTION: FF7 0x00687ab1
void __cdecl Gfx_ReleaseTextureRecordRef(int texture)
{
    GfxTextureSurface *texPtrPtr;
    GfxEngineContext *engine;
    GfxTextureRecord *pRecord;
    GfxTextureRecord *found;

    engine = GetEngineContext();
    if (texture != 0) {
        texPtrPtr = (GfxTextureSurface *)texture;
        found = (GfxTextureRecord *)(ListFindPayloadInHolder(1, texPtrPtr, (int(__cdecl *)(int, int, int))IndexedEntryKeyMatches, engine->fileRegistry));
        pRecord = found;
        if (pRecord != 0) {
            (pRecord->refs)--;
            if (pRecord->refs <= 0) {
                Gfx_FreeTextureRecord((GfxTextureSurface *)pRecord->recordHandle);
                if (pRecord->cacheRefs <= 0) {
                    ListRemoveValueFromHolder((int)pRecord, engine->fileRegistry);
                }
            }
        } else {
            Gfx_FreeTextureRecord((GfxTextureSurface *)texture);
        }
    }
}

/*
 * Allocates an empty texture surface management record.
 */
// FUNCTION: FF7 0x00687b53
int __cdecl Gfx_AllocSurfaceRecord(void)
{
    GfxTextureSurface *rec;

    // STRING: FF7 0x0090fee8
    rec = (GfxTextureSurface *)GameHeapAlloc(1, 160, "C:\\lib\\src\\graphics\\dx_graph.cpp", 1489);
    return (int)rec;
}

/*
 * Creates DirectDraw surfaces and Direct3D texture interfaces for texture record.
 */
// FUNCTION: FF7 0x00687b7a
int __cdecl GfxCreateTextureSurfaces(GfxTextureSurface *texture, GfxTexHeader *convertParams, GfxTextureFormatRecord *surfaceSpec)
{
    GfxPaletteRecord *palette;
    int blobB[27];
    int success;
    int srcColorKey;
    GfxEngineContext *engineOther;
    int descData[27];
    int cachedSurface;
    GfxPaletteRecord *pPaletteHandle;
    GfxTextureSurfaceDesc *surfaceDesc;
    int readyValue;
    int paletteOkFlag;
    int sizeBytes;
    int created;
    unsigned char lockNeeded;
    unsigned char rebound;
    int bufA[27];
    int colorKeyValue;
    int refsSpare;
    int blobWork[27];
    unsigned char hit;
    GfxTextureSurfaceDesc *pDescPtrB;
    GfxEngineContext *ctx;
    GfxTextureSurfaceDesc *pPrimaryDesc;
    GfxBlitRect rectBuf;
    GfxTextureSurfaceDesc *descVal;
    int remaining;
    int hasPal;
    int ok;

    hit = 1;
    ctx = GetEngineContext();
    if (ctx != 0 && ctx->fallbackFlag != 0) {
        created = 0;
        pDescPtrB = (GfxTextureSurfaceDesc *)blobB;
        pPrimaryDesc = (GfxTextureSurfaceDesc *)bufA;
        ctx = GetEngineContext();
        if (ctx != 0 && surfaceSpec != 0) {
            if (texture == 0) {
                texture = (GfxTextureSurface *)Gfx_AllocSurfaceRecord();
            }
            rebound = 0;
            lockNeeded = 0;
            if (texture != 0) {
                texture->header = (int)convertParams;
                texture->format = (int)surfaceSpec;
                cachedSurface = (int)Gfx_TextureCacheFindSurface(texture->texHeader, texture->texHeader->row);
                if (cachedSurface != 0 && texture->surface != 0) {
                    if (((ComDirectDrawSurface *)(texture->surface))->IsLost() == DDERR_SURFACELOST) {
                        rebound = 0;
                        ((ComDirectDrawSurface *)(texture->surface))->Restore();
                        lockNeeded = 1;
                    } else {
                        rebound = 1;
                        if (texture->surface != cachedSurface) {
                            ((ComUnknown *)(texture->surface))->Release();
                            texture->surface = cachedSurface;
                            ((ComUnknown *)(cachedSurface))->AddRef();
                        }
                        if (texture->d3dTexture != 0) {
                            ((ComUnknown *)(texture->d3dTexture))->Release();
                        }
                        // STRING: FF7 0x0090ff0c
                        if (CheckDirectDrawResult(((ComUnknown *)(texture->surface))->QueryInterface(&IID_IDirect3DTexture2, &texture->d3dTexture), "C:\\lib\\src\\graphics\\dx_graph.cpp", 1566) != 0) {
                            // STRING: FF7 0x0090ff30
                            CheckDirectDrawResult(((ComDirect3DTexture2 *)(texture->d3dTexture))->GetHandle((int)ctx->d3dDevice2, &texture->handle), "C:\\lib\\src\\graphics\\dx_graph.cpp", 1568);
                        }
                    }
                }
                if (0) /* Disabled in retail (/Od keeps the zero-test and body); see lib/codegen.h. */
                    ;
                if (rebound == 0) {
                    *pPrimaryDesc = *(GfxTextureSurfaceDesc *)surfaceSpec->surfaceDesc;
                    pPrimaryDesc->pixelFormatSize = 32;
                    pPrimaryDesc->flags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT;
                    pPrimaryDesc->caps = DDSCAPS_TEXTURE | DDSCAPS_SYSTEMMEMORY;
                    pPrimaryDesc->width = convertParams->format.width;
                    pPrimaryDesc->height = convertParams->format.height;
                    if (texture->sourceSurface == 0) {
                        hasPal = 0;
                        // STRING: FF7 0x0090ff54
                        if (CheckDirectDrawResult(((ComDirectDraw *)(ctx->ddObject))->CreateSurface(pPrimaryDesc, &texture->sourceSurface, (int)0), "C:\\lib\\src\\graphics\\dx_graph.cpp", 1600) != 0) {
                            if (texture->texHeader->paletteCount >= 1) {
                                if (texture->paletteRow == -1) {
                                    convertParams->row = 0;
                                } else {
                                    convertParams->row = texture->paletteRow;
                                }
                            }
                        }
                        srcColorKey = convertParams->colorKey;
                        if (convertParams->paletteMode == 1) {
                            srcColorKey = convertParams->colorLookupBytes[convertParams->row];
                        }
                        if (srcColorKey != 0 && g_GfxTextureSrcColorKeyEnabled != 0) {
                            DDSurfaceSetSrcColorKey(ctx->ddSurface, texture->sourceSurface);
                        }
                        palette = 0;
                        if (texture->texHeader->paletteCount >= 1) {
                            palette = (GfxPaletteRecord *)(Gfx_CreateSurfacePaletteRecord(surfaceSpec->format.bitsPerPixel, convertParams, texture));
                            if (palette != 0) {
                                texture->palette = (int)palette;
                            } else {
                                hasPal = 1;
                            }
                        }
                        if (texture->sourceTexture != 0) {
                            ((ComUnknown *)(texture->sourceTexture))->Release();
                        }
                        if (hasPal == 0 && texture->sourceSurface != 0) {
                            // STRING: FF7 0x0090ff78
                            CheckDirectDrawResult(((ComUnknown *)(texture->sourceSurface))->QueryInterface(&IID_IDirect3DTexture2, &texture->sourceTexture), "C:\\lib\\src\\graphics\\dx_graph.cpp", 1652);
                        }
                    }
                    GfxUploadTexturePixels(convertParams, pPrimaryDesc, texture->sourceSurface);
                    if (lockNeeded == 1) {
                        rectBuf.left = 0;
                        rectBuf.top = 0;
                        rectBuf.right = convertParams->format.width;
                        rectBuf.bottom = convertParams->format.height;
                        // STRING: FF7 0x0090ff9c
                        CheckDirectDrawResult(((ComDirectDrawSurface *)(texture->surface))->BltFast((int)0, (int)0, (int)texture->sourceSurface, (int)&rectBuf, (int)DDBLTFAST_WAIT), "C:\\lib\\src\\graphics\\dx_graph.cpp", 1676);
                        if (texture->sourceSurface != 0 && texture->texHeader->paletteCount <= 1) {
                            refsSpare = ((ComUnknown *)(texture->sourceSurface))->Release();
                            texture->sourceSurface = 0;
                        }
                        return (int)texture;
                    }
                    *pDescPtrB = *(GfxTextureSurfaceDesc *)surfaceSpec->surfaceDesc;
                    pDescPtrB->pixelFormatSize = 32;
                    pDescPtrB->flags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT;
                    pDescPtrB->caps = DDSCAPS_ALLOCONLOAD | DDSCAPS_VIDEOMEMORY | DDSCAPS_TEXTURE;
                    pDescPtrB->width = convertParams->format.width;
                    pDescPtrB->height = convertParams->format.height;
                    if (texture->surface != 0) {
                        ((ComUnknown *)(texture->surface))->Release();
                    }
                    // STRING: FF7 0x0090ffc0
                    if (CheckDirectDrawResult(((ComDirectDraw *)(ctx->ddObject))->CreateSurface(pDescPtrB, &texture->surface, (int)0), "C:\\lib\\src\\graphics\\dx_graph.cpp", 1708) == 0) {
                        sizeBytes = convertParams->format.width * convertParams->format.height << 1;
                        if (Gfx_TextureCacheEvictForBytes(sizeBytes) == true) {
                            // STRING: FF7 0x0090ffe4
                            if (CheckDirectDrawResult(((ComDirectDraw *)(ctx->ddObject))->CreateSurface(pDescPtrB, &texture->surface, (int)0), "C:\\lib\\src\\graphics\\dx_graph.cpp", 1714) == 0) {
                                pDescPtrB->caps = DDSCAPS_ALLOCONLOAD | DDSCAPS_TEXTURE;
                                // STRING: FF7 0x00910008
                                if (CheckDirectDrawResult(((ComDirectDraw *)(ctx->ddObject))->CreateSurface(pDescPtrB, &texture->surface, (int)0), "C:\\lib\\src\\graphics\\dx_graph.cpp", 1718) == 0) {
                                    return (int)texture;
                                }
                            }
                        } else {
                            pDescPtrB->caps = DDSCAPS_ALLOCONLOAD | DDSCAPS_TEXTURE;
                            // STRING: FF7 0x0091002c
                            if (CheckDirectDrawResult(((ComDirectDraw *)(ctx->ddObject))->CreateSurface(pDescPtrB, &texture->surface, (int)0), "C:\\lib\\src\\graphics\\dx_graph.cpp", 1728) == 0) {
                                return (int)texture;
                            }
                        }
                    }
                    if (hit == 1) {
                        if (0) /* Disabled in retail (/Od keeps the zero-test and body); see lib/codegen.h. */
                            ;
                        if (0)
                            ;
                        colorKeyValue = convertParams->colorKey;
                        if (convertParams->paletteMode == 1) {
                            colorKeyValue = convertParams->colorLookupBytes[convertParams->row];
                        }
                        if (colorKeyValue != 0 && g_GfxTextureColorKeyEnabled != 0) {
                            DDSurfaceSetSrcColorKey(ctx->ddSurface, texture->surface);
                        }
                        if (texture->d3dTexture != 0) {
                            ((ComUnknown *)(texture->d3dTexture))->Release();
                        }
                        if (texture->surface != 0) {
                            // STRING: FF7 0x00910050
                            if (CheckDirectDrawResult(((ComUnknown *)(texture->surface))->QueryInterface(&IID_IDirect3DTexture2, &texture->d3dTexture), "C:\\lib\\src\\graphics\\dx_graph.cpp", 1767) != 0) {
                                // STRING: FF7 0x00910074
                                if (CheckDirectDrawResult(((ComDirect3DTexture2 *)(texture->d3dTexture))->Load((int)texture->sourceTexture), "C:\\lib\\src\\graphics\\dx_graph.cpp", 1770) != 0) {
                                    ok = 0;
                                    if (ctx->useDirect3D5 != 0) {
                                        ok = 0;
                                        // STRING: FF7 0x00910098
                                        if (CheckDirectDrawResult(((ComDirect3DTexture2 *)(texture->d3dTexture))->GetHandle((int)ctx->d3dDevice2, &texture->handle), "C:\\lib\\src\\graphics\\dx_graph.cpp", 1779) != 0) {
                                            ok = 1;
                                        }
                                    }
                                    if (ok != 0) {
                                        texture->surfaceDesc.size = 108;
                                        // STRING: FF7 0x009100bc
                                        if (CheckDirectDrawResult(((ComDirectDrawSurface *)(texture->surface))->GetSurfaceDesc(&texture->surfaceDesc), "C:\\lib\\src\\graphics\\dx_graph.cpp", 1788) != 0) {
                                            if (1 != 0) { /* Constant condition kept for /Od branch layout; see lib/codegen.h. */
                                                if ((texture->surfaceDesc.caps & DDSCAPS_VIDEOMEMORY) != 0) {
                                                    // STRING: FF7 0x009100e0
                                                    Gfx_DebugPrint("TEXTURE LOADED INTO VIDEO MEMORY\n");
                                                } else {
                                                    // STRING: FF7 0x00910104
                                                    Gfx_DebugPrint("TEXTURE LOADED INTO SYSTEM MEMORY\n");
                                                }
                                            }
                                            created = 1;
                                        }
                                    }
                                } else {
                                    return (int)texture;
                                }
                            }
                        }
                    } else {
                        return (int)texture;
                    }
                    if (texture->sourceSurface != 0 && texture->texHeader->paletteCount <= 1) {
                        remaining = ((ComUnknown *)(texture->sourceSurface))->Release();
                        texture->sourceSurface = 0;
                    }
                    ((ComUnknown *)(texture->surface))->AddRef();
                    hit = Gfx_TextureCacheInsertSurface(texture->texHeader, texture->texHeader->row, (struct IDirectDrawSurface *)texture->surface);
                }
            }
        }
        return (int)texture;
    } else {
        success = 0;
        descVal = (GfxTextureSurfaceDesc *)blobWork;
        surfaceDesc = (GfxTextureSurfaceDesc *)descData;
        engineOther = GetEngineContext();
        if (engineOther != 0 && surfaceSpec != 0) {
            if (texture == 0) {
                texture = (GfxTextureSurface *)Gfx_AllocSurfaceRecord();
            }
            if (texture != 0) {
                texture->header = (int)convertParams;
                texture->format = (int)surfaceSpec;
                *surfaceDesc = *(GfxTextureSurfaceDesc *)surfaceSpec->surfaceDesc;
                surfaceDesc->pixelFormatSize = 32;
                surfaceDesc->flags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT;
                surfaceDesc->caps = DDSCAPS_TEXTURE | DDSCAPS_SYSTEMMEMORY;
                surfaceDesc->width = convertParams->format.width;
                surfaceDesc->height = convertParams->format.height;
                *descVal = *(GfxTextureSurfaceDesc *)surfaceSpec->surfaceDesc;
                descVal->pixelFormatSize = 32;
                descVal->flags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT;
                descVal->caps = DDSCAPS_ALLOCONLOAD | DDSCAPS_TEXTURE;
                descVal->width = convertParams->format.width;
                descVal->height = convertParams->format.height;
                paletteOkFlag = 0;
                // STRING: FF7 0x00910128
                if (CheckDirectDrawResult(((ComDirectDraw *)(engineOther->ddObject))->CreateSurface(surfaceDesc, &texture->sourceSurface, (int)0), "C:\\lib\\src\\graphics\\dx_graph.cpp", 1893) != 0) {
                    GfxUploadTexturePixels(convertParams, surfaceDesc, texture->sourceSurface);
                    if (convertParams->colorKey != 0) {
                        DDSurfaceSetSrcColorKey(engineOther->ddSurface, texture->sourceSurface);
                    }
                    pPaletteHandle = 0;
                    if (surfaceSpec->format.palettized != 0) {
                        pPaletteHandle = (GfxPaletteRecord *)(Gfx_CreateSurfacePaletteRecord(surfaceSpec->format.bitsPerPixel, convertParams, texture));
                        if (pPaletteHandle != 0) {
                            texture->palette = (int)pPaletteHandle;
                        } else {
                            paletteOkFlag = 1;
                        }
                    }
                    if (paletteOkFlag == 0 && texture->sourceSurface != 0) {
                        // STRING: FF7 0x0091014c
                        if (CheckDirectDrawResult(((ComUnknown *)(texture->sourceSurface))->QueryInterface(&IID_IDirect3DTexture2, &texture->sourceTexture), "C:\\lib\\src\\graphics\\dx_graph.cpp", 1924) != 0) {
                            // STRING: FF7 0x00910170
                            if (CheckDirectDrawResult(((ComDirectDraw *)(engineOther->ddObject))->CreateSurface(descVal, &texture->surface, (int)0), "C:\\lib\\src\\graphics\\dx_graph.cpp", 1927) != 0) {
                                if (surfaceSpec->format.palettized != 0 && pPaletteHandle != 0) {
                                    D3DSurfaceSetPalette(pPaletteHandle, texture->surface);
                                }
                                if (convertParams->colorKey != 0) {
                                    DDSurfaceSetSrcColorKey(engineOther->ddSurface, texture->surface);
                                }
                                if (texture->surface != 0) {
                                    // STRING: FF7 0x00910194
                                    if (CheckDirectDrawResult(((ComUnknown *)(texture->surface))->QueryInterface(&IID_IDirect3DTexture2, &texture->d3dTexture), "C:\\lib\\src\\graphics\\dx_graph.cpp", 1951) != 0) {
                                        // STRING: FF7 0x009101b8
                                        if (CheckDirectDrawResult(((ComDirect3DTexture2 *)(texture->d3dTexture))->Load((int)texture->sourceTexture), "C:\\lib\\src\\graphics\\dx_graph.cpp", 1954) != 0) {
                                            readyValue = 0;
                                            if (engineOther->useDirect3D5 != 0) {
                                                readyValue = 0;
                                                // STRING: FF7 0x009101dc
                                                if (CheckDirectDrawResult(((ComDirect3DTexture2 *)(texture->d3dTexture))->GetHandle((int)engineOther->d3dDevice2, &texture->handle), "C:\\lib\\src\\graphics\\dx_graph.cpp", 1970) != 0) {
                                                    readyValue = 1;
                                                }
                                            }
                                            if (readyValue != 0) {
                                                texture->surfaceDesc.size = 108;
                                                // STRING: FF7 0x00910200
                                                if (CheckDirectDrawResult(((ComDirectDrawSurface *)(texture->surface))->GetSurfaceDesc(&texture->surfaceDesc), "C:\\lib\\src\\graphics\\dx_graph.cpp", 1979) != 0) {
                                                    if (DEAD_IN_RETAIL != 0) {
                                                        if ((texture->surfaceDesc.caps & DDSCAPS_VIDEOMEMORY) != 0) {
                                                            // STRING: FF7 0x00910224
                                                            Gfx_DebugPrint("TEXTURE LOADED INTO VIDEO MEMORY\n");
                                                        } else {
                                                            // STRING: FF7 0x00910248
                                                            Gfx_DebugPrint("TEXTURE LOADED INTO SYSTEM MEMORY\n");
                                                        }
                                                    }
                                                    success = 1;
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                    if (texture->sourceSurface != 0) {
                        ((ComUnknown *)(texture->sourceSurface))->Release();
                        texture->sourceSurface = 0;
                    }
                }
            }
        }
        if (success == 0) {
            Gfx_ReleaseTextureRecordRef((int)texture);
            texture = 0;
        }
        return (int)texture;
    }
}

/*
 * Restores lost DirectDraw surfaces and re-uploads texture pixels.
 */
// FUNCTION: FF7 0x006888b2
GfxTextureSurface *__cdecl Gfx_RestoreTextureSurfaces(void *texture)
{
    GfxEngineContext *engine;
    int savedValue;

    engine = GetEngineContext();
    if (texture != 0 && engine != 0) {
        if (engine->fallbackFlag != 0) {
            savedValue = ((GfxTextureSurface *)texture)->paletteRow;
            // STRING: FF7 0x0091026c
            CheckDirectDrawResult(((ComDirectDrawSurface *)(((GfxTextureSurface *)texture)->surface))->Restore(), "C:\\lib\\src\\graphics\\dx_graph.cpp", 2039);
            Gfx_ReleaseTextureSurfacesAndObject((GfxTextureSurface *)texture);
            Gfx_TextureCacheReleaseByTexture(((GfxTextureSurface *)texture)->texHeader);
            texture = (GfxTextureSurface *)GfxCreateTextureSurfaces((GfxTextureSurface *)texture, ((GfxTextureSurface *)texture)->texHeader, ((GfxTextureSurface *)texture)->formatRecord);
            if (texture != 0) {
                ((GfxTextureSurface *)texture)->paletteRow = savedValue;
            }
        } else {
            // STRING: FF7 0x00910290
            CheckDirectDrawResult(((ComDirectDrawSurface *)(((GfxTextureSurface *)texture)->surface))->Restore(), "C:\\lib\\src\\graphics\\dx_graph.cpp", 2060);
            Gfx_ReleaseTextureSurfacesAndObject((GfxTextureSurface *)texture);
            texture = (GfxTextureSurface *)GfxCreateTextureSurfaces((GfxTextureSurface *)texture, ((GfxTextureSurface *)texture)->texHeader, ((GfxTextureSurface *)texture)->formatRecord);
            if (texture != 0) {
                ((GfxTextureSurface *)texture)->paletteRow = 0;
            }
        }
    }
    return (GfxTextureSurface *)texture;
}

/*
 * Acquires or creates a texture surface matching specified request.
 */
// FUNCTION: FF7 0x006889d8
int __cdecl Gfx_AcquireTextureSurface(int unused, int request, int format)
{
    GfxEngineContext *ctx;
    GfxTextureSurface *pTexture;
    GfxDriverOps *driver;
    int res;
    GfxTextureRecord *record;
    GfxTextureRecord *found;

    res = 0;
    ctx = GetEngineContext();
    if (request != 0) {
        driver = GetGraphicsDriverObject(ctx);
        pTexture = (GfxTextureSurface *)request;
        found = (GfxTextureRecord *)(ListFindPayloadInHolder(0, pTexture, (int(__cdecl *)(int, int, int))IndexedEntryKeyMatches, ctx->fileRegistry));
        record = found;
        if (record != 0) {
            if (record->refs <= 0) {
                record->refs = 1;
                res = driver->loadTexture(0, request, format);
                if (res != 0) {
                    record->recordHandle = res;
                }
            } else {
                (record->refs)++;
                res = record->recordHandle;
            }
        } else {
            res = driver->loadTexture(0, request, format);
        }
    }
    return res;
}

/*
 * Finds or allocates a texture object satisfying descriptor parameters.
 */
// FUNCTION: FF7 0x00688aa9
int __cdecl Gfx_AcquireTextureForRequest(int options, int texHeader, int device)
{
    int format;
    int texture;

    texture = 0;
    if (texHeader != 0 && device != 0) {
        ((GfxTexHeader *)texHeader)->paletteFlags = ((GfxTexturedHundredOptions *)options)->paletteFlags;
        format = Gfx_SelectTextureFormat((int *)options, (GfxTexHeader *)texHeader, (GfxEngineContext *)device);
        texture = Gfx_AcquireTextureSurface(options, texHeader, format);
    }
    return texture;
}

/*
 * Destroys a texture object and frees all associated surface records.
 */
// FUNCTION: FF7 0x00688b03
void __cdecl Gfx_DestroyTexture(GfxTexHeader *texture)
{
    if (texture != 0) {
        // STRING: FF7 0x009102b4
        GameHeapFree((void *)texture->storage, "C:\\lib\\src\\graphics\\dx_graph.cpp", 2162);
        // STRING: FF7 0x009102d8
        GameHeapFree((void *)texture->colorLookup, "C:\\lib\\src\\graphics\\dx_graph.cpp", 2163);
        // STRING: FF7 0x009102fc
        GameHeapFree((void *)texture->format.palette, "C:\\lib\\src\\graphics\\dx_graph.cpp", 2164);
        // STRING: FF7 0x00910320
        GameHeapFree((void *)texture->pixelBuffer, "C:\\lib\\src\\graphics\\dx_graph.cpp", 2165);
        // STRING: FF7 0x00910344
        GameHeapFree(texture, "C:\\lib\\src\\graphics\\dx_graph.cpp", 2167);
    }
}

/*
 * Releases cached texture surfaces and associated file data.
 */
// FUNCTION: FF7 0x00688b95
void __cdecl Gfx_ReleaseCachedTexture(int texture)
{
    void *texPtrPtr;
    GfxEngineContext *engine;
    GfxTextureRecord *pRecord;
    char status;
    GfxTextureRecord *pLookupResult;

    engine = GetEngineContext();
    if (texture != 0) {
        texPtrPtr = (void *)texture;
        status = Gfx_TextureCacheReleaseByTexture((GfxTexHeader *)texture);
        pLookupResult = (GfxTextureRecord *)(ListFindPayloadInHolder(0, texPtrPtr, (int(__cdecl *)(int, int, int))IndexedEntryKeyMatches, engine->fileRegistry));
        pRecord = pLookupResult;
        if (pRecord != 0) {
            (pRecord->cacheRefs)--;
            if (pRecord->cacheRefs <= 0) {
                Gfx_DestroyTexture((struct GfxTexHeader *)pRecord->texture);
                if (pRecord->refs <= 0) {
                    ListRemoveValueFromHolder((int)pRecord, engine->fileRegistry);
                }
            }
        } else {
            Gfx_DestroyTexture((struct GfxTexHeader *)texture);
        }
    }
}

/*
 * Allocates and initializes a new texture file record.
 */
// FUNCTION: FF7 0x00688c46
int __cdecl Gfx_AllocTexRecord(void)
{
    GfxTexHeader *rec;

    // STRING: FF7 0x00910368
    rec = (GfxTexHeader *)GameHeapAlloc(1, 236, "C:\\lib\\src\\graphics\\dx_graph.cpp", 2211);
    if (rec != 0) {
        rec->version = 1;
        rec->alphaValue = 0xFF;
        rec->blendMode = BLEND_NONE;
    }
    return (int)rec;
}

/*
 * Loads and parses texture file header and pixel data from disk.
 */
// FUNCTION: FF7 0x00688c96
int __cdecl Gfx_LoadTexFile(int request, const char *fileName)
{
    GfxTexHeader *tex;
    int error;
    struct FileHandle *file;

    error = 1;
    tex = (GfxTexHeader *)Gfx_AllocTexRecord();
    if (tex != 0) {
        file = OpenFileEx((FileOpenRequest *)request, fileName);
        if (file != 0) {
            if (ReadFileData(236, tex, file) != 0) {
                tex->pixelBuffer = 0;
                tex->storage = 0;
                tex->colorLookup = 0;
                tex->format.palette = 0;
                if (tex->version == 1) {
                    error = 0;
                    if (tex->format.palettized != 0) {
                        tex->format.palette = (struct PolygonTimColor *)ReadFileIntoNewBuffer(4, tex->format.paletteEntryCount, file);
                        if (tex->format.palette == 0) {
                            error = 1;
                        }
                    }
                    if (error == 0) {
                        tex->pixelBuffer = ReadFileIntoNewBuffer(tex->format.bytesPerPixel, tex->format.width * tex->format.height, file);
                        if (tex->pixelBuffer == 0) {
                            error = 1;
                        }
                    }
                    if (error == 0 && tex->paletteMode != 0) {
                        tex->colorLookup = ReadFileIntoNewBuffer(1, tex->paletteCount, file);
                        if (tex->colorLookup == 0) {
                            error = 1;
                        }
                    }
                }
            }
            CloseFileEx(file);
        }
    }
    if (error != 0) {
        Gfx_ReleaseCachedTexture((int)tex);
        tex = 0;
    }
    return (int)tex;
}

/*
 * Loads a texture file from disk or retrieves from existing cache.
 */
// FUNCTION: FF7 0x00688e19
int __cdecl Gfx_LoadTexFileCached(int outTexture, const char *fileName)
{
    struct {
        int found;
        char cached[200];
        GfxTextureRecord *node;
        int result;
        GfxTextureRecord *request;
        GfxEngineContext *context;
        char error[200];
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    frame.result = 0;
    frame.context = GetEngineContext();
    frame.request = (GfxTextureRecord *)FileCreateRecord(0, (char *)fileName, 0);
    if (frame.request != 0) {
        frame.found = ListFindValueByCompareInHolder((int)frame.request, (ListCompareFn)FileRecordMatchesCompare, frame.context->fileRegistry);
        frame.node = (GfxTextureRecord *)frame.found;
        if (frame.node != 0) {
            FileFreeRecord((struct FileRecord *)frame.request);
            (frame.node->cacheRefs)++;
            frame.result = frame.node->texture;
            if (DEBUG_TRACE != 0) {
                // STRING: FF7 0x0091038c
                sprintf(frame.cached, "TEXTURE DATA %s USAGE %d\n", fileName, frame.node->cacheRefs);
                Gfx_DebugPrint(frame.cached);
            }
        } else {
            frame.result = Gfx_LoadTexFile(outTexture, fileName);
            if (frame.result != 0) {
                ListAppendValueToHolder((int)frame.request, frame.context->fileRegistry);
                frame.request->cacheRefs = 1;
                frame.request->texture = frame.result;
            } else {
                FileFreeRecord((struct FileRecord *)frame.request);
            }
        }
    }
    if (frame.result == 0) {
        // STRING: FF7 0x009103a8
        sprintf(frame.error, "ERROR: COULD NOT LOAD TEXTURE DATA FILE %s\n", fileName);
        Gfx_DebugPrint(frame.error);
    }
    return frame.result;
}

/*
 * Serializes texture record to disk as a binary TEX file.
 */
// FUNCTION: FF7 0x00688fa4
int __cdecl Gfx_WriteTexFile(char *fileName, GfxTexHeader *texRecord)
{
    struct {
        struct FileHandle *file;
        int failed;
        char message[200];
        const char *suffix;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    frame.failed = 1;
    frame.suffix = &g_GfxTexFileExtension;
    if (texRecord != 0 && texRecord->pixelBuffer != 0) {
        frame.file = (struct FileHandle *)OpenFileSimple(FILE_OPEN_CREATE, fileName);
        if (frame.file != 0) {
            if (WriteFileData(236, texRecord, frame.file) != 0) {
                frame.failed = 0;
                if (texRecord->format.palettized != 0 && texRecord->format.palette != 0) {
                    if (texRecord->format.paletteEntryCount <= 0) {
                        // STRING: FF7 0x009103d4
                        frame.suffix = " NO PALETTE.";
                        frame.failed = 1;
                    } else if (WriteFileData(texRecord->format.paletteEntryCount << 2, texRecord->format.paletteEntries, frame.file) != 0) { /* empty first arm: lib/codegen.h */
                    } else {
                        frame.failed = 1;
                    }
                }
                if (frame.failed == 0) {
                    if (WriteFileData(texRecord->format.bytesPerPixel * texRecord->format.width * texRecord->format.height, (void *)texRecord->pixelBuffer, frame.file) != 0) { /* empty first arm: lib/codegen.h */
                    } else {
                        frame.failed = 1;
                    }
                }
                if (frame.failed == 0 && texRecord->paletteMode != 0) {
                    if (WriteFileData(texRecord->paletteCount, texRecord->colorLookupBytes, frame.file) != 0) { /* empty first arm: lib/codegen.h */
                    } else {
                        frame.failed = 1;
                    }
                }
            }
            CloseFileEx(frame.file);
        }
    }
    if (frame.failed != 0) {
        // STRING: FF7 0x009103e4
        sprintf(frame.message, "ERROR: COULD NOT SAVE TEXTURE DATA TO FILE %s%s\n", fileName, frame.suffix);
        Gfx_DebugPrint(frame.message);
    }
    return frame.failed == 0;
}

/*
 * Logs texture file registry entry dimensions and format details.
 */
// FUNCTION: FF7 0x00689160
int __cdecl Gfx_LogTexFileRegistryEntry(int entry, int totalPixels)
{
    struct {
        GfxTextureHeader *texture;
        char message[200];
    } frame;

    frame.texture = (GfxTextureHeader *)(((GfxTextureRecord *)entry)->texture);
    if (frame.texture != 0) {
        // STRING: FF7 0x00910418
        sprintf(frame.message, "REGISTRY TEXTURE DATA FILE: %s  USAGE: %d %d W:%d H:%d \n", ((GfxTextureRecord *)entry)->fileName, ((GfxTextureRecord *)entry)->cacheRefs, ((GfxTextureRecord *)entry)->refs, frame.texture->width, frame.texture->height);
        Gfx_DebugPrint(frame.message);
        *(int *)totalPixels += frame.texture->width * frame.texture->height;
    }
    return 1;
}

/*
 * Logs total memory consumption of registered texture records.
 */
// FUNCTION: FF7 0x006891f2
void __cdecl Gfx_LogTexFileRegistryTotalPixelSize(int holder)
{
    struct {
        int total;
        char message[200];
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    frame.total = 0;
    ListForEachEntryInHolder(Gfx_LogTexFileRegistryEntry, (int)&frame.total, holder);
    // STRING: FF7 0x00910454
    sprintf(frame.message, "REGISTRY TEXTURE DATA TOTAL PIXEL SIZE: %d\n", frame.total);
    Gfx_DebugPrint(frame.message);
}

/*
 * Uploads palette color data to the texture surface palette.
 */
// FUNCTION: FF7 0x0068924b
int __cdecl Gfx_UploadTexturePalette(int device, int surface, int startIndex, int entryCount, int texRecord)
{
    int result;

    result = 0;
    if (texRecord != 0 && ((GfxTextureSurface *)texRecord)->palette != 0) {
        result = (int)(Gfx_WritePaletteEntriesCall(device, surface, startIndex, entryCount, ((GfxTextureSurface *)texRecord)->palette, texRecord));
        ((GfxTextureSurface *)texRecord)->paletteRow = 0xFFFFFFFF;
    }
    return result;
}
} /* extern "C" */
