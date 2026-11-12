// src/ff7/field/src/ad_tile.cpp
// Original: C:\FF7\field\src\ad_tile.cpp
// Contents: Field tile records and textures: allocating and freeing tile records and their
//   textures, converting RGB565 pixels to RGB555, and texture format setup and palette dumps for
//   tiles.
/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
// Also: Manages the field's texture pages and background layers: invalidates and creates page
//   batches, draws page ranges and the background layers, and keeps the projection offset in step
//   with the camera.
#include "ff7/types/field.h"
#include "lib/api/graphics.h"
#include "lib/types/graphics.h"
#include "ff7/api/field.h"
#include "lib/api/mem.h"
#include "lib/crt.h"
#include <string.h>
#include "lib/pfile.h"
#include "lib/codegen.h"
#include "ff7/field.h"
#include "lib/api/polygon.h"
#include "lib/enums/graphics.h"
extern "C" {

// clang-format on

/*
 * Extracts the low-sixteen-bit masks 0xF800, 0x7E0 and 0x1F, shifts the first two right by one,
 * and ORs the fields.
 */
// FUNCTION: FF7 0x0063f350
int __cdecl Gfx_ConvertRgb565WordToRgb555(int color)
{
    return (color & 0xFFFF & 0xF800) >> 1 | (color & 0xFFFF & 0x7E0) >> 1 | color & 0xFFFF & 0x1F;
}

/*
 * Returns when g_FieldTileDumpPaletteFlag is nonzero; otherwise computes a mapped black value and visits 42
 * table records. For records with a nonzero field at twelve and unsigned-short field at twenty-six
 * equal to two, rewrites 65536 shorts: zero to mapped black, 0x821 to 0x403, otherwise through
 * Gfx_ConvertRgb565WordToRgb555 with the WORD call view. Record and pixel backing bounds require independent
 * evidence.
 */
// FUNCTION: FF7 0x0063f385
void __cdecl Field_ConvertTexturePagePixelsRgb565To555(void)
{
    int i;
    int colorArgb;
    unsigned short *pPixels;
    unsigned short keyPixel;
    int pixelIdx;

    if (g_FieldTileDumpPaletteFlag != 0)
        return;
    colorArgb = 0xff000000;
    keyPixel = Blit_MapArgb8888ViaRampTables(*(GfxArgb32 *)&colorArgb, GetEngineContext());
    for (i = 0; i < 42; i++) {
        if (g_FieldLayers[i]->enabled != 0 &&
            g_FieldLayers[i]->bytesPerPixel == 2) {
            pPixels = g_FieldLayers[i]->pixels16;
            for (pixelIdx = 0; pixelIdx < 65536; pixelIdx++) {
                if (pPixels[pixelIdx] == 0) {
                    pPixels[pixelIdx] = keyPixel;
                    continue;
                }
                if (pPixels[pixelIdx] == 0x821)
                    pPixels[pixelIdx] = 0x403;
                // The accepted helper masks its DWORD argument to the low WORD.
                // This call view preserves the original WORD argument load.
                else
                    /* Retail caller prototype (unsigned short) -> int preserved for /Od argument promotion on `Gfx_ConvertRgb565WordToRgb555`. */
                    pPixels[pixelIdx] = ((int(__cdecl *)(unsigned short))Gfx_ConvertRgb565WordToRgb555)(pPixels[pixelIdx]);
            }
        }
    }
}

#pragma intrinsic(strlen)
#define g_FieldPaletteGridStride (*(unsigned short *)((char *)&g_FieldPaletteGridHeight + 2))
typedef struct TileBgra {
    unsigned char blue, green, red, alpha;
} TileBgra;

/*
 * Allocate the 42 tile records and their optional 8/16-bit pixel buffers.
 */
// FUNCTION: FF7 0x0063f494
int __cdecl Field_AllocTileRecords(void)
{
    unsigned short i;
    g_FieldSplashHeightBias = 0;
    for (i = 0; i < 0x2a; i++) {
        // STRING: FF7 0x00907c34
        g_FieldLayers[i] = (FieldLayer *)GameHeapAlloc(1, 0x1c, "C:\\FF7\\field\\src\\ad_tile.cpp", 0x5c);
        if (g_FieldLayers[i] == 0) {
            return 0;
        }
        g_FieldLayers[i]->pixels = 0;
        if (g_FieldTileStateDc4 == 0) {
            if (i < 0x1a) {
                // STRING: FF7 0x00907c54
                g_FieldLayers[i]->pixels = (unsigned char *)GameHeapAlloc(0x10000, 1, "C:\\FF7\\field\\src\\ad_tile.cpp", 0x64);
                g_FieldLayers[i]->bytesPerPixel = 1;
            } else {
                // STRING: FF7 0x00907c74
                g_FieldLayers[i]->pixels = (unsigned char *)GameHeapAlloc(0x10000, 2, "C:\\FF7\\field\\src\\ad_tile.cpp", 0x69);
                g_FieldLayers[i]->bytesPerPixel = 2;
            }
            if (g_FieldLayers[i]->pixels == 0) {
                return 0;
            }
        }
        g_FieldLayers[i]->enabled = 0;
        g_FieldLayers[i]->clearedState = 0;
        g_FieldLayers[i]->built = 0;
        g_FieldLayers[i]->hasDoubleSizeTiles = 0;
    }
    return 1;
}

/*
 * Forty-two slots, each holding a record that owns a second buffer
 * at +0x04. The inner buffer goes first and is nulled, then the
 * record, then the slot.
 */
// FUNCTION: FF7 0x0063f635
void __cdecl Field_FreeTileRecords(void)
{
    unsigned short i;

    for (i = 0; i < 0x2a; i++) {
        if (g_FieldLayers[i] != 0) {
            if (g_FieldLayers[i]->pixels != 0) {
                // STRING: FF7 0x00907c94
                GameHeapFree(g_FieldLayers[i]->pixels, "C:\\FF7\\field\\src\\ad_tile.cpp", 0x83);
                g_FieldLayers[i]->pixels = 0;
            }
            // STRING: FF7 0x00907cb4
            GameHeapFree(g_FieldLayers[i], "C:\\FF7\\field\\src\\ad_tile.cpp", 0x86);
            g_FieldLayers[i] = 0;
        }
    }
}

/*
 * When g_FieldTileStateDbc is set, frees the 42 tile textures at g_FieldLayers. A loaded one (+0xC set)
 * has its render object at +8 destroyed, is dropped from the engine's texture list through
 * Gfx_TextureCacheReleaseByTexture when the context's +0xA90 is set, and is released through Gfx_ReleaseCachedTexture; an unloaded
 * one only has its buffer at +4 freed. Then the record itself is freed and the slot nulled.
 */
// FUNCTION: FF7 0x0063f707
void __cdecl Field_FreeTileTextures(void)
{
    unsigned short layerIndex;
    GfxEngineContext *engine;

    if (g_FieldTileStateDbc == 0) {
        return;
    }
    for (layerIndex = 0; layerIndex < 0x2A; layerIndex++) {
        if (g_FieldLayers[layerIndex] != 0) {
            if (g_FieldLayers[layerIndex]->enabled != 0) {
                if (g_FieldLayers[layerIndex]->renderObject != 0) {
                    DestroyRenderObject((void *)g_FieldLayers[layerIndex]->renderObject);
                    g_FieldLayers[layerIndex]->renderObject = 0;
                }
                engine = GetEngineContext();
                if (engine->fallbackFlag != 0) {
                    Gfx_TextureCacheReleaseByTexture((GfxTexHeader *)g_FieldLayers[layerIndex]->texRecord);
                }
                Gfx_ReleaseCachedTexture(g_FieldLayers[layerIndex]->texRecord);
                g_FieldLayers[layerIndex]->pixels = 0;
                g_FieldLayers[layerIndex]->enabled = 0;
            } else if (g_FieldLayers[layerIndex]->pixels != 0) {
                // STRING: FF7 0x00907cd4
                GameHeapFree(g_FieldLayers[layerIndex]->pixels, "C:\\FF7\\field\\src\\ad_tile.cpp", 0xB1);
                g_FieldLayers[layerIndex]->pixels = 0;
            }
            // STRING: FF7 0x00907cf4
            GameHeapFree(g_FieldLayers[layerIndex], "C:\\FF7\\field\\src\\ad_tile.cpp", 0xB6);
            g_FieldLayers[layerIndex] = 0;
        }
    }
}

/*
 * Dump each palette row as text, including its red, green and blue channels.
 */
// FUNCTION: FF7 0x0063f8c5
int __cdecl Field_DumpPaletteTextFile(int count)
{
    int result;
    char textBuf[480];
    FILE *filePtr;
    unsigned short paletteIndex;
    unsigned short colorIndex;
    result = 1;
    filePtr = Field_FileOpen(
        // STRING: FF7 0x00907d18
        "pal.tst",
        // STRING: FF7 0x00907d14
        "w");
    if (filePtr == 0) {
        return 0;
    }
    Field_FileWrite(filePtr,
                         // STRING: FF7 0x00907d20
                         "Palette Data\n", 0x10);
    for (paletteIndex = 0; paletteIndex < count; paletteIndex++) {
        sprintf(textBuf,
                // STRING: FF7 0x00907d30
                "#pal %d: \n", paletteIndex);
        Field_FileWrite(filePtr, textBuf, strlen(textBuf));
        for (colorIndex = 0; colorIndex < 0x100; colorIndex++) {
            sprintf(textBuf,
                    // STRING: FF7 0x00907d3c
                    "r %d g %d b%d\n",
                    g_FieldPaletteGridBuffer[(paletteIndex << 8) + colorIndex].pixelFlagA,
                    g_FieldPaletteGridBuffer[(paletteIndex << 8) + colorIndex].pixelFlagB,
                    g_FieldPaletteGridBuffer[(paletteIndex << 8) + colorIndex].pixelFlagC);
            Field_FileWrite(filePtr, textBuf, strlen(textBuf));
        }
    }
    Field_FileClose(filePtr);
    return 1;
}

/*
 * Sixteen bits per pixel with the three colour masks taken from
 * globals and the alpha mask either 0 or 0x8000 -- which is the one
 * alpha bit of 1555, so g_FieldTileDumpPaletteFlag is the switch between an
 * opaque format and a keyed one.
 * The masks land in a pixel-format block at +0x3C of the record,
 * and the rest of the fields are constants written straight out.
 * `alphaBit` is spelled that way because `alpha` takes [ebp-4] and
 * the image has the bit depth there.
 */
// FUNCTION: FF7 0x0063faab
void __cdecl Field_InitTileTextureFormat(GfxTexHeader *tex)
{
    int bits;
    int alphaBit;
    int red;
    GfxPixelFormat *format;
    int green;
    int blue;

    format = &tex->format;
    bits = 0x10;
    red = g_FieldTileColorRed;
    green = g_FieldTileColorGreen;
    blue = g_FieldTileColorBlue;
    if (g_FieldTileDumpPaletteFlag != 0) {
        alphaBit = 0;
    } else {
        alphaBit = 0x8000;
    }
    PixelFormatSetRgbMasks(bits, red, green, blue, alphaBit, format);
    tex->unusedFormatFlag = 0;
    tex->colorKey = 1;
    tex->hasSemiTransparency = 0;
    tex->minColorBits = 4;
    tex->maxColorBits = 8;
    tex->minAlphaBits = 0;
    tex->maxAlphaBits = 8;
    tex->minBitsPerPixel = 0x10;
    tex->maxBitsPerPixel = 0x20;
    tex->modeFlag = 1;
    format->width = 0x100;
    format->height = 0x100;
    format->pitch = 0x200;
    format->palettized = 0;
}

/*
 * Configure an indexed texture, copy BGRA palettes and append black/white rows.
 */
// FUNCTION: FF7 0x0063fba3
void __cdecl Field_InitTileIndexedTexture(GfxTexHeader *tex)
{
    int alphaBits;
    int bitCount;
    int red;
    GfxPixelFormat *format;
    unsigned short entIndex;
    unsigned short rowIndex;
    int greenMask;
    int maskB;
    format = &tex->format;
    bitCount = 8;
    red = g_FieldTileColorRed;
    greenMask = g_FieldTileColorGreen;
    maskB = g_FieldTileColorBlue;
    if (g_FieldTileDumpPaletteFlag != 0) {
        alphaBits = 0;
    } else {
        alphaBits = 0x8000;
    }
    red = 0;
    greenMask = 0;
    maskB = 0;
    alphaBits = 0;
    PixelFormatSetRgbMasks(bitCount, red, greenMask, maskB, alphaBits, format);
    tex->unusedFormatFlag = 1;
    tex->paletteMode = 1;
    tex->colorKey = 1;
    tex->hasSemiTransparency = 0;
    tex->minColorBits = 4;
    tex->maxColorBits = 8;
    tex->minAlphaBits = 0;
    tex->maxAlphaBits = 8;
    tex->minBitsPerPixel = 8;
    tex->paletteCount = g_FieldPaletteGridHeight + 2;
    tex->entriesPerPalette = 0x100;
    tex->bitsPerPixel = 8;
    tex->maxBitsPerPixel = 0x20;
    format->width = 0x100;
    format->height = 0x100;
    format->pitch = 0x200;
    format->palettized = 1;
    format->indexBits = 8;
    format->indexedTo8 = 0;
    format->paletteEntryCount = (g_FieldPaletteGridHeight + 2) << 8;
    format->entriesPerPalette = 0x100;
    format->palette = (struct PolygonTimColor *)GameHeapAllocRaw(format->paletteEntryCount << 2,
                                            // STRING: FF7 0x00907d4c
                                            "C:\\FF7\\field\\src\\ad_tile.cpp", 0x140);
    *(void **)&tex->colorLookup = GameHeapAllocRaw(g_FieldPaletteGridHeight + 2,
                                                      // STRING: FF7 0x00907d6c
                                                      "C:\\FF7\\field\\src\\ad_tile.cpp", 0x142);
    for (rowIndex = 0; rowIndex < g_FieldPaletteGridHeight; rowIndex++) {
        ((unsigned char *)tex->colorLookup)[rowIndex] = g_FieldPaletteFlags[rowIndex];
    }
    ((unsigned char *)tex->colorLookup)[g_FieldPaletteGridHeight] = 1;
    ((unsigned char *)tex->colorLookup)[g_FieldPaletteGridHeight + 1] = 1;
    for (rowIndex = 0; rowIndex < g_FieldPaletteGridHeight; rowIndex++) {
        for (entIndex = 0; entIndex < 0x100; entIndex++) {
            format->tilePalette[(rowIndex << 8) + entIndex].red = g_FieldPaletteGridBuffer[rowIndex * g_FieldPaletteGridStride + entIndex].pixelFlagA;
            format->tilePalette[(rowIndex << 8) + entIndex].green = g_FieldPaletteGridBuffer[rowIndex * g_FieldPaletteGridStride + entIndex].pixelFlagB;
            format->tilePalette[(rowIndex << 8) + entIndex].blue = g_FieldPaletteGridBuffer[rowIndex * g_FieldPaletteGridStride + entIndex].pixelFlagC;
            format->tilePalette[(rowIndex << 8) + entIndex].alpha = g_FieldPaletteGridBuffer[rowIndex * g_FieldPaletteGridStride + entIndex].paletteIndex;
        }
    }
    for (rowIndex = 0; rowIndex < 2; rowIndex++) {
        if (rowIndex == 0) {
            for (entIndex = 0; entIndex < 0x100; entIndex++) {
                format->tilePalette[((g_FieldPaletteGridHeight + rowIndex) << 8) + entIndex].red = 0;
                format->tilePalette[((g_FieldPaletteGridHeight + rowIndex) << 8) + entIndex].green = 0;
                format->tilePalette[((g_FieldPaletteGridHeight + rowIndex) << 8) + entIndex].blue = 0;
                format->tilePalette[((g_FieldPaletteGridHeight + rowIndex) << 8) + entIndex].alpha = 0xff;
            }
        } else {
            for (entIndex = 0; entIndex < 0x100; entIndex++) {
                format->tilePalette[((g_FieldPaletteGridHeight + rowIndex) << 8) + entIndex].red = 0xff;
                format->tilePalette[((g_FieldPaletteGridHeight + rowIndex) << 8) + entIndex].green = 0xff;
                format->tilePalette[((g_FieldPaletteGridHeight + rowIndex) << 8) + entIndex].blue = 0xff;
                format->tilePalette[((g_FieldPaletteGridHeight + rowIndex) << 8) + entIndex].alpha = 0x80;
            }
        }
    }
}
// clang-format off
unsigned char g_rawData_00907c20[20] = {0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01};
// clang-format on
} /* extern "C" */


extern "C" {

// Tears down all 0x2A texture pages in g_FieldLayers. For each enabled page, it invalidates the
// batch generation of its renderObject, empties that render object's batch, and clears clearedState and
// built. It finishes by calling Field_ReleaseLineDrawBatch, which releases the separate batch
// g_FieldLineDrawBuffer the same way.
// FUNCTION: FF7 0x00640160
void __cdecl Field_InvalidateTexturePageBatches(void)
{
    unsigned short i;

    for (i = 0; i < 0x2A; i++) {
        if (g_FieldLayers[i]->enabled != 0) {
            InvalidateBatchGeneration(g_FieldLayers[i]->batch);
            Gfx_EmptyRenderObjectBatch(g_FieldLayers[i]->renderObject);
            g_FieldLayers[i]->clearedState = 0;
            g_FieldLayers[i]->built = 0;
        }
    }
    Field_ReleaseLineDrawBatch();
}

/*
 * Submits the texture pages with indices startIndex up to endIndex & 0xFFFF from g_FieldLayers: every page
 * that is both enabled and built has its renderObject handed to Gfx_EmitPrimitiveBatchList
 * along with the engine context.
 */
// FUNCTION: FF7 0x00640213
void __cdecl Field_DrawTexturePageRange(GfxEngineContext *context, int startIndex, int endIndex)
{
    unsigned short pageIndex;

    for (pageIndex = startIndex; pageIndex < (endIndex & 0xFFFF); pageIndex++) {
        if (g_FieldLayers[pageIndex]->enabled != 0 && g_FieldLayers[pageIndex]->built != 0) {
            Gfx_EmitPrimitiveBatchList(g_FieldLayers[pageIndex]->batch, context);
        }
    }
}

/*
 * Prepares field texture pages and scans the 42 page records, processing only the enabled ones.
 * Initializes blend options by bytes per pixel and index: 1 byte per pixel uses mode 4 below 15,
 * mode 1 from 15 through 23, then zero; 2 bytes use mode 4 below 33, mode 1 through 39, then zero,
 * with option flags 0x24. Sets the display-mode-dependent flags2 bit 0x40 or 0x20, allocates and
 * initializes a texture header, points its pixelBuffer at the page's pixels, and creates a render
 * object from the engine context's renderObjectBank. The headers of the higher indexed pages
 * (1 byte per pixel from 15, 2 bytes from 33) get hasSemiTransparency set to one. The first failed render object
 * returns zero with partial results retained; success clears the temporary flags2 bit for each
 * processed page and returns one.
 */
// FUNCTION: FF7 0x00640292
int __cdecl Field_CreateTexturePages(struct GfxEngineContext *ctx, PfileLoadOptions *opts)
{
    int scratch;
    int blendMode;
    unsigned short pageIndex;

    scratch = 0;
    Field_ConvertTexturePagePixelsRgb565To555();
    for (pageIndex = 0; pageIndex < 0x2A; pageIndex++) {
        if (g_FieldLayers[pageIndex]->enabled != 0) {
            scratch = 0;
            if (g_FieldLayers[pageIndex]->bytesPerPixel == 1) {
                if (pageIndex >= 0xF) {
                    if (pageIndex >= 0x18) {
                        blendMode = BLEND_AVERAGE;
                    } else {
                        blendMode = BLEND_ADDITIVE;
                    }
                    Pfile_InitLoadOptionsWithBlend(blendMode, opts);
                    if (g_FieldDisplayMode == 2) {
                        opts->renderFlags |= 0x40;
                    } else {
                        opts->renderFlags |= 0x20;
                    }
                } else {
                    Pfile_InitLoadOptionsWithBlend(BLEND_NONE, opts);
                    opts->flags |= 0x20;
                    if (g_FieldDisplayMode == 2) {
                        opts->renderFlags |= 0x40;
                    } else {
                        opts->renderFlags |= 0x20;
                    }
                }
            } else if (pageIndex >= 0x21) {
                if (pageIndex >= 0x28) {
                    blendMode = BLEND_AVERAGE;
                } else {
                    blendMode = BLEND_ADDITIVE;
                }
                Pfile_InitLoadOptionsWithBlend(blendMode, opts);
                opts->flags |= 0x24;
                if (g_FieldDisplayMode == 2) {
                    opts->renderFlags |= 0x40;
                } else {
                    opts->renderFlags |= 0x20;
                }
            } else {
                Pfile_InitLoadOptionsWithBlend(BLEND_NONE, opts);
                opts->flags |= 0x24;
                if (g_FieldDisplayMode == 2) {
                    opts->renderFlags |= 0x40;
                } else {
                    opts->renderFlags |= 0x20;
                }
            }
            g_FieldLayers[pageIndex]->texRecord = Gfx_AllocTexRecord();
            if (g_FieldLayers[pageIndex]->bytesPerPixel == 1) {
                Field_InitTileIndexedTexture(g_FieldLayers[pageIndex]->texHeader);
            } else {
                Field_InitTileTextureFormat(g_FieldLayers[pageIndex]->texHeader);
            }
            g_FieldLayers[pageIndex]->texHeader->pixelBuffer = (int)g_FieldLayers[pageIndex]->pixels;
            opts->texHeader = g_FieldLayers[pageIndex]->texHeader;
            if ((pageIndex >= 0xF && g_FieldLayers[pageIndex]->bytesPerPixel == 1) || (pageIndex >= 0x21 && g_FieldLayers[pageIndex]->bytesPerPixel == 2)) {
                opts->texHeader->hasSemiTransparency = 1;
            }
            g_FieldLayers[pageIndex]->batch = (GfxRenderObject *)Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, opts, 0, (void *)ctx->renderObjectBank);
            if (g_FieldLayers[pageIndex]->renderObject == 0) {
                g_FieldLayers[pageIndex]->renderObject = 0;
                return 0;
            }
            if (g_FieldDisplayMode == 2) {
                opts->renderFlags &= 0xFFFFFFBF;
            } else {
                opts->renderFlags &= 0xFFFFFFDF;
            }
        }
    }
    return 1;
}

/*
 * Visits in-use layer 2 tiles not yet sampled, marks them active and sampled before requesting draw,
 * computes doubled positions narrowed to shorts, submits a 32x32 quad through the tile's texture page
 * and marks that page built. Always returns one; allocation/draw outcome ignored and no camera bounds
 * rejection.
 */
// FUNCTION: FF7 0x0064060d
int __cdecl Field_DrawBackgroundLayer2DoubledQuads(short cameraX, short cameraY)
{
    short posX;
    GfxEngineContext *context;
    float texU;
    float texV;
    short top;
    unsigned short tileIdx;

    context = GetEngineContext();
    texU = 0.0625f;
    texV = 0.0625f;
    for (tileIdx = 0; tileIdx < g_FieldLayer2TilesNum; tileIdx++) {
        if (g_FieldLayer2Tiles[tileIdx].inUse != 0 && g_FieldLayer2Tiles[tileIdx].sampled == 0) {
            g_FieldLayer2Tiles[tileIdx].active = 1;
            g_FieldLayer2Tiles[tileIdx].sampled = 1;
            posX = g_FieldLayer2Tiles[tileIdx].tileX + 0xA0 - cameraX << 1;
            top = (g_FieldLayer2Tiles[tileIdx].spanY + 0x78 - cameraY) * 2 + 0x10;
            Field_EmitBatchNodeTexturedQuad(((FieldLayer *)g_FieldLayers[g_FieldLayer2Tiles[tileIdx].texPage])->batchNode, (float)posX, (float)top, g_FieldLayer2Tiles[tileIdx].depthZ, g_FieldLayer2Tiles[tileIdx].texU, g_FieldLayer2Tiles[tileIdx].texV, texU, texV, g_FieldLayer2Tiles[tileIdx].paletteRow, 32.0f, 32.0f, ((FieldLayer *)g_FieldLayers[g_FieldLayer2Tiles[tileIdx].texPage])->bytesPerPixel);
            ((FieldLayer *)g_FieldLayers[g_FieldLayer2Tiles[tileIdx].texPage])->built = 1;
        }
    }
    return 1;
}

/*
 * Visits the configured background-layer tile index list with strict position culling and an
 * optional parameter-mask check. Computes scaled screen offsets from cameraX and cameraY, including the
 * conditional vertical adjustment, and narrows tile screen coordinates to SHORT before float
 * conversion. Marks each accepted tile before skipping the field-ready special texture case. Emits
 * the remaining tile with textureId or texPage according to its inUse state.
 * Returns one even when nothing is drawn.
 */
// FUNCTION: FF7 0x0064085c
int __cdecl Field_DrawBackgroundLayer2(short cameraX, short cameraY)
{
    short xPos;
    short yOrigin;
    short xBase;
    float texU;
    float texV;
    short top;
    unsigned short tileIdx;
    unsigned short listIndex;

    texU = 0.0625f;
    texV = 0.0625f;
    xBase = (0x140 - cameraX) * g_FieldDisplayScaleFactor;
    yOrigin = (0xE0 - cameraY) * g_FieldDisplayScaleFactor;
    if (g_FieldBgOriginOffsetY > 0 && cameraY <= 8) {
        yOrigin -= (short)(g_FieldBgOriginOffsetY * g_FieldDisplayScaleFactor);
    }
    for (listIndex = 0; listIndex < g_FieldLayer2TilesNum; listIndex++) {
        tileIdx = g_pFieldLayer2DrawOrder[listIndex].index16;
        if (g_FieldLayer2Tiles[tileIdx].spanY > cameraY - 0x100 && (g_FieldLayer2Tiles[tileIdx].spanY < cameraY && (g_FieldLayer2Tiles[tileIdx].tileX > cameraX - 0x150 && (g_FieldLayer2Tiles[tileIdx].tileX < cameraX && (g_FieldLayer2Tiles[tileIdx].zBufferValue == 0 || (g_FieldLayer2Tiles[tileIdx].visibilityMask & g_FieldActorState.state.backgroundLayerVisibility[g_FieldLayer2Tiles[tileIdx].zBufferValue]) != 0))))) {
            g_FieldLayer2Tiles[tileIdx].active = 1;
            xPos = g_FieldLayer2Tiles[tileIdx].tileX * g_FieldDisplayScaleFactor + xBase;
            top = g_FieldLayer2Tiles[tileIdx].spanY * g_FieldDisplayScaleFactor + yOrigin;
            if (g_FieldDataReady != 0 && g_FieldLayer2Tiles[tileIdx].paletteRow == g_FieldPaletteGridHeight + 1) {
                continue;
            }
            if (g_FieldLayer2Tiles[tileIdx].inUse == 0) {
                Field_EmitBackgroundTile((float)xPos, (float)top, g_FieldLayer2Tiles[tileIdx].depthZ, g_FieldLayer2Tiles[tileIdx].texU, g_FieldLayer2Tiles[tileIdx].texV, g_FieldLayer2Tiles[tileIdx].paletteRow, g_FieldLayer2Tiles[tileIdx].textureId);
            } else {
                Field_EmitBackgroundTile((float)xPos, (float)top, g_FieldLayer2Tiles[tileIdx].depthZ, g_FieldLayer2Tiles[tileIdx].texU, g_FieldLayer2Tiles[tileIdx].texV, g_FieldLayer2Tiles[tileIdx].paletteRow, g_FieldLayer2Tiles[tileIdx].texPage);
            }
        }
    }
    return 1;
}

/*
 * Iterates layer-one tile index list, marks every referenced tile active and emits only tiles
 * strictly inside camera-relative X/Y bounds. Scales integer positions into narrowed shorts before
 * float conversion, applies optional vertical offset near top and submits fixed depth with tile
 * texture fields. Always returns one.
 */
// FUNCTION: FF7 0x00640c49
int __cdecl Field_DrawBackgroundLayer1(short cameraX, short cameraY)
{
    short xPos;
    short yOrigin;
    short xBase;
    float texU;
    float texV;
    short top;
    unsigned short tileIdx;
    unsigned short listIndex;

    texU = 0.0625f;
    texV = 0.0625f;
    xBase = (0x140 - cameraX) * g_FieldDisplayScaleFactor;
    yOrigin = (0xE0 - cameraY) * g_FieldDisplayScaleFactor;
    if (g_FieldBgOriginOffsetY > 0 && cameraY <= 6) {
        yOrigin -= (short)(g_FieldBgOriginOffsetY * g_FieldDisplayScaleFactor);
    }
    for (listIndex = 0; listIndex < g_FieldLayer1TilesNum; listIndex++) {
        tileIdx = g_pFieldLayer1DrawOrder[listIndex].index16;
        g_FieldLayer1Tiles[tileIdx].drawn = 1;
        if (g_FieldLayer1Tiles[tileIdx].depth > cameraY - 0x100 && g_FieldLayer1Tiles[tileIdx].depth < cameraY && g_FieldLayer1Tiles[tileIdx].posX > cameraX - 0x150 && g_FieldLayer1Tiles[tileIdx].posX < cameraX) {
            xPos = g_FieldLayer1Tiles[tileIdx].posX * g_FieldDisplayScaleFactor + xBase;
            top = g_FieldLayer1Tiles[tileIdx].depth * g_FieldDisplayScaleFactor + yOrigin;
            Field_EmitBackgroundTile((float)xPos, (float)top, 0.9997f, g_FieldLayer1Tiles[tileIdx].texU, g_FieldLayer1Tiles[tileIdx].texV, g_FieldLayer1Tiles[tileIdx].paletteRow, g_FieldLayer1Tiles[tileIdx].mapId);
        }
    }
    return 1;
}

/*
 * Returns the constant 1 and touches nothing.
 */
// FUNCTION: FF7 0x00640ead
int __cdecl Field_UnusedReturnOne_1(void)
{
    return 1;
}

/*
 * When either short g_FieldPaletteCurrentOrgX or g_FieldPaletteCurrentOrgY differs from g_FieldPaletteTargetOrgX or g_FieldPaletteTargetOrgY, it
 * copies the second pair over the first and calls SetProjectionOffsetAndRebuild(g_FieldPaletteTargetOrgX *
 * g_FieldDisplayScaleFactor, g_FieldPaletteTargetOrgY * g_FieldDisplayScaleFactor), g_FieldDisplayScaleFactor being the scale factor the rest of this
 * code multiplies field coordinates by. Otherwise it does nothing.
 */
// FUNCTION: FF7 0x00640eb7
void __cdecl Field_SyncProjectionOffsetToCamera(void)
{
    if (g_FieldPaletteCurrentOrgX != g_FieldPaletteTargetOrgX || g_FieldPaletteCurrentOrgY != g_FieldPaletteTargetOrgY) {
        g_FieldPaletteCurrentOrgX = g_FieldPaletteTargetOrgX;
        g_FieldPaletteCurrentOrgY = g_FieldPaletteTargetOrgY;
        SetProjectionOffsetAndRebuild(g_FieldPaletteTargetOrgX * g_FieldDisplayScaleFactor, g_FieldPaletteTargetOrgY * g_FieldDisplayScaleFactor);
    }
}

/*
 * Draws the four background layers in order: layer 3 at the fixed offsets
 * g_FieldBgLayer3ScrollX/g_FieldBgLayer3ScrollY, layers 1 and 2 at (cameraX, cameraY) when g_FieldCameraScrollE74 and g_FieldLayer2ParamE04
 * respectively are set, layer 4 at g_FieldBgLayer4ScrollX/g_FieldBgLayer4ScrollY, then Field_BuildBackgroundQuads.
 * Always returns 1.
 */
// FUNCTION: FF7 0x00640f22
int __cdecl Field_DrawBackgroundLayers(short cameraX, short cameraY)
{
    Field_DrawBackgroundLayer3(g_FieldBgLayer3ScrollX, g_FieldBgLayer3ScrollY);
    if (g_FieldCameraScrollE74 != 0) {
        Field_DrawBackgroundLayer1(cameraX, cameraY);
    }
    if (g_FieldLayer2ParamE04 != 0) {
        Field_DrawBackgroundLayer2(cameraX, cameraY);
    }
    Field_DrawBackgroundLayer4(g_FieldBgLayer4ScrollX, g_FieldBgLayer4ScrollY);
    Field_BuildBackgroundQuads();
    return 1;
}

/*
 * Returns zero when background layer three is disabled. Otherwise selects depth 0.9998 for depth
 * code at least 4095 or computes depth through the helper, visits its tile index list, and wraps
 * each tile coordinate once by trigger dimensions when outside the configured view bounds. Does
 * not cull again after wrapping. Tiles passing the optional parameter mask are marked, scaled into
 * SHORT screen coordinates, and emitted with texId or mapId according to the tile's ownerTag.
 * Sets the global layer flag and returns one even if no tile emits.
 */
// FUNCTION: FF7 0x00640f95
int __cdecl Field_DrawBackgroundLayer3(short cameraX, short cameraY)
{
    short xPos;
    short yOrigin;
    short xBase;
    float texU;
    float texV;
    short top;
    float zDepth;
    unsigned short tileIdx;
    unsigned short listIndex;

    if (g_FieldDoDrawLayer3 == 0) {
        return 0;
    } else {
        texU = 0.125f;
        texV = 0.125f;
        xBase = (0x140 - cameraX) * g_FieldDisplayScaleFactor;
        yOrigin = (0xE0 - cameraY) * g_FieldDisplayScaleFactor;
        if (g_FieldActorState.state.layer2Depth >= 0xFFF) {
            zDepth = 0.9998f;
        } else {
            zDepth = Field_ComputeTileDepthZ(g_FieldCameraTransform, g_FieldActorState.state.layer2Depth, 0, 0);
        }
        for (listIndex = 0; listIndex < g_FieldLayer3TilesNum; listIndex++) {
            tileIdx = g_pFieldLayer3DrawOrder[listIndex].index16;
            xPos = g_FieldLayer3Tiles[tileIdx].xPos;
            top = g_FieldLayer3Tiles[tileIdx].tileY;
            if (top <= cameraY - 0x100 || top >= cameraY) {
                if (top < cameraY - 0x70) {
                    top += g_pFieldScrollBounds->tileHeight;
                } else {
                    top -= g_pFieldScrollBounds->tileHeight;
                }
            }
            if (xPos <= cameraX - 0x160 || xPos >= cameraX) {
                if (xPos < cameraX - 0xA0) {
                    xPos += g_pFieldScrollBounds->tileWidth;
                } else {
                    xPos -= g_pFieldScrollBounds->tileWidth;
                }
            }
            if (g_FieldLayer3Tiles[tileIdx].zDepth == 0 || (g_FieldLayer3Tiles[tileIdx].zFlags & g_FieldActorState.state.backgroundLayerVisibility[g_FieldLayer3Tiles[tileIdx].zDepth]) != 0) {
                g_FieldLayer3Tiles[tileIdx].inUse = 1;
                xPos = xPos * g_FieldDisplayScaleFactor + xBase;
                top = top * g_FieldDisplayScaleFactor + yOrigin;
                if (g_FieldLayer3Tiles[tileIdx].ownerTag == 0) {
                    Field_EmitBackgroundTile((float)xPos, (float)top, zDepth, g_FieldLayer3Tiles[tileIdx].texU, g_FieldLayer3Tiles[tileIdx].texV, g_FieldLayer3Tiles[tileIdx].paletteRow, g_FieldLayer3Tiles[tileIdx].texId);
                } else {
                    Field_EmitBackgroundTile((float)xPos, (float)top, zDepth, g_FieldLayer3Tiles[tileIdx].texU, g_FieldLayer3Tiles[tileIdx].texV, g_FieldLayer3Tiles[tileIdx].paletteRow, g_FieldLayer3Tiles[tileIdx].mapId);
                }
            }
        }
        g_FieldLayer3Flag = 1;
        return 1;
    }
}

/*
 * Returns zero when background layer four is disabled. Otherwise computes depth and screen
 * offsets, visits its tile index list, and wraps coordinates once by trigger dimensions when
 * outside view limits. After wrapping, applies strict X culling and the parameter-mask condition,
 * but does not perform another Y cull. Marks accepted tiles before skipping the field-ready
 * special texture case and emits remaining tiles with textureId or texSlot according to the
 * tile's active flag. Screen positions narrow to SHORT before float conversion. Sets the layer flag and returns
 * one regardless of emitted count.
 */
// FUNCTION: FF7 0x00641358
int __cdecl Field_DrawBackgroundLayer4(short viewX, short viewY)
{
    short tileX;
    short screenOfsX;
    short offsetY;
    short tilePositionY;
    float depthVal;
    unsigned short tileId;
    float biasBitsY;
    float offsetBitsX;
    unsigned short n;

    if (g_FieldDoDrawLayer4 == 0) {
        return 0;
    } else {
        offsetBitsX = 0.125f;
        biasBitsY = 0.125f;
        screenOfsX = (0x140 - viewX) * g_FieldDisplayScaleFactor;
        offsetY = (0xE0 - viewY) * g_FieldDisplayScaleFactor;
        depthVal = Field_ComputeTileDepthZ(g_FieldCameraTransform, g_FieldActorState.state.layer3Depth, 0, 0);
        for (n = 0; n < g_FieldLayer4TilesNum; n++) {
            tileId = g_pFieldLayer4DrawOrder[n].index16;
            tileX = g_FieldLayer4Tiles[tileId].posX;
            tilePositionY = g_FieldLayer4Tiles[tileId].posY;
            if (tilePositionY <= viewY - 0x100 || tilePositionY >= viewY) {
                if (tilePositionY < viewY) {
                    tilePositionY += g_pFieldScrollBounds->tileStrideY;
                } else {
                    tilePositionY -= g_pFieldScrollBounds->tileStrideY;
                }
            }
            if (tileX <= viewX - 0x160 || tileX >= viewX) {
                if (tileX < viewX - 0xA0) {
                    tileX += g_pFieldScrollBounds->tileStrideX;
                } else {
                    tileX -= g_pFieldScrollBounds->tileStrideX;
                }
            }
            if (tileX > viewX - 0x160 && (tileX < viewX && (g_FieldLayer4Tiles[tileId].zDepth == 0 || (g_FieldLayer4Tiles[tileId].zFlags & g_FieldActorState.state.backgroundLayerVisibility[g_FieldLayer4Tiles[tileId].zDepth]) != 0))) {
                g_FieldLayer4Tiles[tileId].inUse = 1;
                tileX = tileX * g_FieldDisplayScaleFactor + screenOfsX;
                tilePositionY = tilePositionY * g_FieldDisplayScaleFactor + offsetY;
                if (g_FieldDataReady != 0 && g_FieldLayer4Tiles[tileId].paletteRow == g_FieldPaletteGridHeight + 1) {
                    continue;
                }
                if (g_FieldLayer4Tiles[tileId].active == 0) {
                    Field_EmitBackgroundTile((float)tileX, (float)tilePositionY, depthVal, g_FieldLayer4Tiles[tileId].texUNorm, g_FieldLayer4Tiles[tileId].texVNorm, g_FieldLayer4Tiles[tileId].paletteRow, g_FieldLayer4Tiles[tileId].textureId);
                } else {
                    Field_EmitBackgroundTile((float)tileX, (float)tilePositionY, depthVal, g_FieldLayer4Tiles[tileId].texUNorm, g_FieldLayer4Tiles[tileId].texVNorm, g_FieldLayer4Tiles[tileId].paletteRow, g_FieldLayer4Tiles[tileId].texSlot);
                }
            }
        }
        g_FieldLayer4Flag = 1;
        return 1;
    }
}
} /* extern "C" */
