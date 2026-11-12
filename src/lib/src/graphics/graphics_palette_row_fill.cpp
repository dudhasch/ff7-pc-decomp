// src/lib/src/graphics/graphics_palette_row_fill.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Fills and converts palette table rows: packed colour fill from float or packed sources,
//   and scaling, blending and modulating palette entries into bytes or BGR output.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/types/mem.h"
#include "lib/types/graphics.h"
#include "lib/api/graphics.h"
extern "C" {

/*
 * Fills packed 4-byte colors for a palette table row from float or packed source.
 */
// FUNCTION: FF7 0x006a5a70
void __cdecl Gfx_FillColorRowBytes(int row, GfxColorRowTable *table)
{
    int idx;
    GfxBgrPixel *dst;
    ColorWord *src;
    GfxFloatColor *floatSrc;
    if (table != 0 && row < table->rowCount) {
        dst = (GfxBgrPixel *)((int *)table->dstBase + row * table->rowLength);
        if (table->srcBase != 0) {
            src = (ColorWord *)((int *)table->srcBase + row * table->rowLength);
            for (idx = 0; idx < table->rowLength; idx++) {
                *(ColorWord *)dst = *src;
                src = (ColorWord *)((char *)src + 4);
                dst++;
            }
        } else if (table->floatBase != 0) {
            floatSrc = (GfxFloatColor *)(table->floatBase + ((row * table->rowLength) << 4));
            for (idx = 0; idx < table->rowLength; idx++) {
                dst->channel2 = (unsigned char)floatSrc->r;
                dst->channel1 = (unsigned char)floatSrc->g;
                *(unsigned char *)dst = (unsigned char)*(float *)floatSrc;
                dst->channel3 = (unsigned char)floatSrc->a;
                floatSrc++;
                dst++;
            }
        }
    }
}
/*
 * Walks table row and scales float color entries to bytes.
 */
// FUNCTION: FF7 0x006a5ba0
void __cdecl Gfx_ScaleFloatColorRowToBytes(int scale, int row, MemFloatColorRowBuffer *buffer)
{
    int floatPointer;
    int i;
    int bytes;

    if (buffer != 0 && row < buffer->rowCount && buffer->floatBuffer != 0) {
        floatPointer = buffer->floatBuffer + (row * buffer->width << 4);
        bytes = buffer->byteBuffer + row * buffer->width * 4;
        for (i = 0; i < buffer->width; i++) {
            Gfx_ScaleFloatColorToBytes(scale, floatPointer, bytes);
            floatPointer += 0x10;
            bytes += 4;
        }
    }
}

/*
 * Walks table row and blends palette color entries into BGR output.
 */
// FUNCTION: FF7 0x006a5c3b
void __cdecl Gfx_BlendPaletteRowToBgr(float factor, float factor2, GfxFloatColor *factor3, int row, GfxPaletteBlock *palette)
{
    int floatPointer;
    int i;
    int bytes;

    if (palette != 0 && row < palette->count && palette->dataPtr != 0) {
        floatPointer = palette->dataPtr + (row * palette->stride << 4);
        bytes = palette->startIndex + row * palette->stride * 4;
        for (i = 0; i < palette->stride; i++) {
            Gfx_BlendFloatColorsToBgr(factor, factor2, factor3, (GfxFloatColor *)floatPointer, bytes);
            floatPointer += 0x10;
            bytes += 4;
        }
    }
}

/*
 * Walks table row and modulates palette colors divided by 255 into bytes.
 */
// FUNCTION: FF7 0x006a5ce2
void __cdecl Gfx_ModulatePaletteRowToBytes(void *factor, int row, GfxPaletteBlock *palette)
{
    int floatPointer;
    int i;
    int bytes;

    if (palette != 0 && row < palette->count && palette->dataPtr != 0) {
        floatPointer = palette->dataPtr + (row * palette->stride << 4);
        bytes = palette->startIndex + row * palette->stride * 4;
        for (i = 0; i < palette->stride; i++) {
            Gfx_ModulateFloatColorsDiv255ToBytes((GfxFloatColor *)factor, (GfxLitLightColor *)floatPointer, (struct GlTexelDest *)bytes);
            floatPointer += 0x10;
            bytes += 4;
        }
    }
}
} /* extern "C" */
