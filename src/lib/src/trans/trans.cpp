// Original C:\lib\src\trans\trans.cpp color lookup helpers.
/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/api/mem.h"
#include "lib/api/graphics.h"
extern "C" {

/*
 * Builds a color lookup table scaling RGB channels by factor. Each entry's ARGB8888 colour is
 * scaled byte by byte (alpha too when the format has alpha bits, else 255).
 */
// FUNCTION: FF7 0x0069f910
void *__cdecl Trans_BuildScaledLut(double factor, int bits, GfxPixelFormat *format)
{
    unsigned int pixelValue;
    unsigned int tableBytes;
    GfxArgb32 work;
    int *table;
    int baseColor;
    tableBytes = 1 << bits;
    if (factor >= 0.0 && factor < 1.0) {
        // STRING: FF7 0x00912234
        table = (int *)GameHeapAlloc(tableBytes, 4, "C:\\lib\\src\\trans\\trans.cpp", 40);
        if (table != 0) {
            for (pixelValue = 0; pixelValue < tableBytes; pixelValue++) {
                baseColor = UnpackPixelToArgb8888(pixelValue, format);
                work.packed = baseColor;
                work.ch.b = (int)((*(int *)&work & 255) * factor);
                work.ch.g = (int)((*(int *)((char *)&work + 1) & 255) * factor);
                work.ch.r = (int)((*(int *)((char *)&work + 2) & 255) * factor);
                if (format->alphaBits > 0) {
                    work.ch.a = (int)((*(int *)((char *)&work + 3) & 255) * factor);
                } else {
                    work.ch.a = 255;
                }
                table[pixelValue] = PackArgb8888ToPixelFormat(work, format);
            }
            return table;
        }
    }
    return 0;
}

/*
 * Builds a 2x brightness LUT saturating overflowed channels to 255. Each entry's ARGB8888
 * colour is doubled byte by byte.
 */
// FUNCTION: FF7 0x0069fa43
void *__cdecl Trans_BuildDoubleBrightLutSaturate(int bits, GfxPixelFormat *format)
{
    unsigned int tableBytes;
    unsigned int entryIndex;
    int *table;
    GfxArgb32 doubled;
    int sourceColor;
    tableBytes = 1 << bits;
    // STRING: FF7 0x00912250
    table = (int *)GameHeapAlloc(tableBytes, 4, "C:\\lib\\src\\trans\\trans.cpp", 91);
    if (table != 0) {
        for (entryIndex = 0; entryIndex < tableBytes; entryIndex++) {
            sourceColor = UnpackPixelToArgb8888(entryIndex, format);
            doubled.packed = sourceColor;
            if ((*(int *)&doubled & 255) & 128) {
                doubled.ch.b = 255;
            } else {
                doubled.ch.b <<= 1;
            }
            if ((*(int *)((char *)&doubled + 1) & 255) & 128) {
                doubled.ch.g = 255;
            } else {
                doubled.ch.g <<= 1;
            }
            if ((*(int *)((char *)&doubled + 2) & 255) & 128) {
                doubled.ch.r = 255;
            } else {
                doubled.ch.r <<= 1;
            }
            if (format->alphaBits > 0 && ((*(int *)((char *)&doubled + 3) & 255) & 128)) {
                doubled.ch.a = 255;
            } else if (format->alphaBits > 0) {
                doubled.ch.a <<= 1;
            }
            table[entryIndex] = PackArgb8888ToPixelFormat(doubled, format);
        }
        return table;
    }
    return 0;
}

/*
 * Builds a 2x brightness LUT wrapping overflowed colour channels to zero; alpha (when the format
 * has alpha bits) saturates to 255 as in Trans_BuildDoubleBrightLutSaturate.
 */
// FUNCTION: FF7 0x0069fb6c
void *__cdecl Trans_BuildDoubleBrightLutWrapToZero(int bits, GfxPixelFormat *format)
{
    unsigned int tableBytes;
    unsigned int entryIndex;
    int *table;
    GfxArgb32 doubled;
    int sourceColor;
    tableBytes = 1 << bits;
    // STRING: FF7 0x0091226c
    table = (int *)GameHeapAlloc(tableBytes, 4, "C:\\lib\\src\\trans\\trans.cpp", 166);
    if (table != 0) {
        for (entryIndex = 0; entryIndex < tableBytes; entryIndex++) {
            sourceColor = UnpackPixelToArgb8888(entryIndex, format);
            doubled.packed = sourceColor;
            if ((*(int *)&doubled & 255) & 128) {
                doubled.ch.b = 0;
            } else {
                doubled.ch.b <<= 1;
            }
            if ((*(int *)((char *)&doubled + 1) & 255) & 128) {
                doubled.ch.g = 0;
            } else {
                doubled.ch.g <<= 1;
            }
            if ((*(int *)((char *)&doubled + 2) & 255) & 128) {
                doubled.ch.r = 0;
            } else {
                doubled.ch.r <<= 1;
            }
            if (format->alphaBits > 0 && ((*(int *)((char *)&doubled + 3) & 255) & 128)) {
                doubled.ch.a = 255;
            } else if (format->alphaBits > 0) {
                doubled.ch.a <<= 1;
            }
            table[entryIndex] = PackArgb8888ToPixelFormat(doubled, format);
        }
        return table;
    }
    return 0;
}
} /* extern "C" */
