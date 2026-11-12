// src/ff7/src/credits/credits_glyph_string_draw.cpp
// Original: unknown (no retail __FILE__ path names this TU)

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/codegen.h"
#include "ff7/api/credits.h"
#include "ff7/types/credits.h"
#include "lib/api/graphics.h"
#include <math.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <mmsystem.h>
#include "ff7/gte.h"
#include "ff7/enums/credits.h"
#include "lib/types/graphics.h"

extern "C" {

#pragma intrinsic(abs)


#pragma intrinsic(strcpy, strcat)

// The credits glyph widths the string measurers and drawers index by glyph (cy * 8 + cx).
// GLOBAL: FF7 0x0099c5f0
extern "C" unsigned int g_CreditsGlyphWidthTable[48] = {
    30, 26, 27, 28, 25, 24, 29, 27,
    11, 17, 28, 22, 32, 27, 32, 28,
    32, 27, 26, 26, 28, 29, 32, 30,
    29, 28, 16, 21, 10, 10, 10, 30,
    14, 15, 10, 19, 20, 18, 22, 32,
    30, 30, 32, 32, 25, 25, 26, 32
};
#pragma data_seg(".data$u77_lut")
#pragma bss_seg(".data$u77_lut")
#pragma data_seg()
#pragma bss_seg()

#pragma intrinsic(sqrt)

/*
 * Scans text until CREDITS_GLYPH_END, accumulating g_CreditsGlyphWidthTable glyph
 * widths shifted right by shift for signed char values up to CREDITS_GLYPH_LAST.
 * CREDITS_GLYPH_TO_MIDDLE immediately returns g_CreditsViewportWidth; the other codes
 * contribute nothing.
 */
// FUNCTION: FF7 0x007a8150
int __cdecl Credits_MeasureGlyphStringWidth(char *text, int shift)
{
    int width;
    int column;
    int rowIndex;
    width = 0;
    while (*text != CREDITS_GLYPH_END) {
        if (*text > CREDITS_GLYPH_LAST) {
            switch (*text) {
            case CREDITS_GLYPH_TO_MIDDLE:
                return g_CreditsViewportWidth;
            }
        } else {
            column = *text % 8;
            rowIndex = *text / 8;
            width += g_CreditsGlyphWidthTable[column + rowIndex * 8] >> shift;
        }
        text++;
    }
    return width;
}

/*
 * Draws credits glyph bytes through g_CreditsQuadBatchListD until terminator CREDITS_GLYPH_END. Bytes
 * up to CREDITS_GLYPH_LAST select an eight-column atlas and produce 32 by 40 quads at
 * viewport-adjusted coordinates, depth 0.1, color 0x80FFFFFF and reciprocal W
 * 1; color selects the batch texture state.
 */
// FUNCTION: FF7 0x007a81e2
void __cdecl Credits_DrawGlyphString32x40(int x, int y, char *s, int color)
{
    float glyphU;
    float glyphV;
    int x0;
    int vy;
    int fz;
    float cw;
    float ch;
    int cx;
    int cy;
    GfxTlVertex *q1;
    GfxTlVertex *q2;
    GfxTlVertex *q3;
    GfxTlVertex *v0;

    fz = FLOAT_0_1_BITS;
    cw = 0.125f;
    ch = 0.15625f;
    while (*s != CREDITS_GLYPH_END) {
        if (*s > CREDITS_GLYPH_LAST) {
            switch (*s) {
            case CREDITS_GLYPH_TO_MIDDLE:
                x = g_CreditsViewportWidth >> 1;
                break;
            }
        } else {
            cx = *s % 8;
            cy = *s / 8;
            glyphU = (float)cx * 0.125f;
            glyphV = (float)cy * 0.15625f;
            if (AcquireBatchNode(1, g_CreditsQuadBatchListD) != 0) {
                x0 = x + g_CreditsViewportX;
                vy = y + g_CreditsViewportY;
                q1 = (GfxTlVertex *)(g_CreditsQuadBatchListD->vertexCursor);
                q1->x = (float)x0;
                q1->y = (float)vy;
                q1->zBits = fz;
                q1->rhwBits = FLOAT_ONE_BITS;
                q1->color = 0x80FFFFFF;
                q1->specular = 0xFF000000;
                q1->u = glyphU;
                q1->v = glyphV;
                q2 = (GfxTlVertex *)(g_CreditsQuadBatchListD->vertexCursor + 0x20);
                q2->x = (float)x0;
                q2->y = (float)vy + 40.0f;
                q2->zBits = fz;
                q2->rhwBits = FLOAT_ONE_BITS;
                q2->color = 0x80FFFFFF;
                q2->specular = 0xFF000000;
                q2->u = glyphU;
                q2->v = glyphV + ch;
                q3 = (GfxTlVertex *)(g_CreditsQuadBatchListD->vertexCursor + 0x40);
                q3->x = (float)x0 + 32.0f;
                q3->y = (float)vy;
                q3->zBits = fz;
                q3->rhwBits = FLOAT_ONE_BITS;
                q3->color = 0x80FFFFFF;
                q3->specular = 0xFF000000;
                q3->u = glyphU + cw;
                q3->v = glyphV;
                v0 = (GfxTlVertex *)(g_CreditsQuadBatchListD->vertexCursor + 0x60);
                v0->x = (float)x0 + 32.0f;
                v0->y = (float)vy + 40.0f;
                v0->zBits = fz;
                v0->rhwBits = FLOAT_ONE_BITS;
                v0->color = 0x80FFFFFF;
                v0->specular = 0xFF000000;
                v0->u = glyphU + cw;
                v0->v = glyphV + ch;
                *g_CreditsQuadBatchListD->colorSignedBytePtr = (char)color;
                g_CreditsQuadBatchListD->paletteIndex = color;
            }
            x += g_CreditsGlyphWidthTable[cy * 8 + cx];
        }
        s++;
    }
}

/*
 * Draws credits glyph bytes through g_CreditsTextBuffer.batchNodeA until
 * terminator CREDITS_GLYPH_END. Bytes up to CREDITS_GLYPH_LAST select an eight-column atlas and produce
 * 32 by 40 quads at viewport-adjusted coordinates, depth 0.1, color 0x80FFFFFF
 * and reciprocal W 1; color selects the batch texture state.
 */
// FUNCTION: FF7 0x007a8469
void __cdecl Credits_DrawGlyphString32x40TextBufferNodeA(int x, int y, char *s, int color)
{
    float glyphU;
    float glyphV;
    int x0;
    int vy;
    int fz;
    float cw;
    float ch;
    int cx;
    int cy;
    GfxTlVertex *q1;
    GfxTlVertex *q2;
    GfxTlVertex *q3;
    GfxTlVertex *v0;

    fz = FLOAT_0_1_BITS;
    cw = 0.125f;
    ch = 0.15625f;
    while (*s != CREDITS_GLYPH_END) {
        if (*s > CREDITS_GLYPH_LAST) {
            switch (*s) {
            case CREDITS_GLYPH_TO_MIDDLE:
                x = g_CreditsViewportWidth >> 1;
                break;
            }
        } else {
            cx = *s % 8;
            cy = *s / 8;
            glyphU = (float)cx * 0.125f;
            glyphV = (float)cy * 0.15625f;
            if (AcquireBatchNode(1, g_CreditsTextBuffer.batchNodeA) != 0) {
                x0 = x + g_CreditsViewportX;
                vy = y + g_CreditsViewportY;
                q1 = (GfxTlVertex *)(g_CreditsTextBuffer.batchNodeA->vertexCursor);
                q1->x = (float)x0;
                q1->y = (float)vy;
                q1->zBits = fz;
                q1->rhwBits = FLOAT_ONE_BITS;
                q1->color = 0x80FFFFFF;
                q1->specular = 0xFF000000;
                q1->u = glyphU;
                q1->v = glyphV;
                q2 = (GfxTlVertex *)(g_CreditsTextBuffer.batchNodeA->vertexCursor + 0x20);
                q2->x = (float)x0;
                q2->y = (float)vy + 40.0f;
                q2->zBits = fz;
                q2->rhwBits = FLOAT_ONE_BITS;
                q2->color = 0x80FFFFFF;
                q2->specular = 0xFF000000;
                q2->u = glyphU;
                q2->v = glyphV + ch;
                q3 = (GfxTlVertex *)(g_CreditsTextBuffer.batchNodeA->vertexCursor + 0x40);
                q3->x = (float)x0 + 32.0f;
                q3->y = (float)vy;
                q3->zBits = fz;
                q3->rhwBits = FLOAT_ONE_BITS;
                q3->color = 0x80FFFFFF;
                q3->specular = 0xFF000000;
                q3->u = glyphU + cw;
                q3->v = glyphV;
                v0 = (GfxTlVertex *)(g_CreditsTextBuffer.batchNodeA->vertexCursor + 0x60);
                v0->x = (float)x0 + 32.0f;
                v0->y = (float)vy + 40.0f;
                v0->zBits = fz;
                v0->rhwBits = FLOAT_ONE_BITS;
                v0->color = 0x80FFFFFF;
                v0->specular = 0xFF000000;
                v0->u = glyphU + cw;
                v0->v = glyphV + ch;
                *g_CreditsTextBuffer.batchNodeA->colorSignedBytePtr = (char)color;
                g_CreditsTextBuffer.batchNodeA->paletteIndex = color;
            }
            x += g_CreditsGlyphWidthTable[cy * 8 + cx];
        }
        s++;
    }
}

/*
 * Draws credits glyph bytes through g_CreditsTextBuffer.batchNode0 until
 * terminator CREDITS_GLYPH_END. Bytes up to CREDITS_GLYPH_LAST select an eight-column atlas and produce 32 by 40 quads at
 * viewport-adjusted coordinates, depth 0.1, color 0x80FFFFFF and reciprocal W 1; color selects the
 * batch texture state.
 */
// FUNCTION: FF7 0x007a86f0
void __cdecl Credits_DrawGlyphString32x40TextBufferNode0(int x, int y, char *s, int color)
{
    float glyphU;
    float glyphV;
    int x0;
    int vy;
    int fz;
    float cw;
    float ch;
    int cx;
    int cy;
    GfxTlVertex *q1;
    GfxTlVertex *q2;
    GfxTlVertex *q3;
    GfxTlVertex *v0;

    fz = FLOAT_0_1_BITS;
    cw = 0.125f;
    ch = 0.15625f;
    while (*s != CREDITS_GLYPH_END) {
        if (*s > CREDITS_GLYPH_LAST) {
            switch (*s) {
            case CREDITS_GLYPH_TO_MIDDLE:
                x = g_CreditsViewportWidth >> 1;
                break;
            }
        } else {
            cx = *s % 8;
            cy = *s / 8;
            glyphU = (float)cx * 0.125f;
            glyphV = (float)cy * 0.15625f;
            if (AcquireBatchNode(1, g_CreditsTextBuffer.batchNode0) != 0) {
                x0 = x + g_CreditsViewportX;
                vy = y + g_CreditsViewportY;
                q1 = (GfxTlVertex *)(g_CreditsTextBuffer.batchNode0->vertexCursor);
                q1->x = (float)x0;
                q1->y = (float)vy;
                q1->zBits = fz;
                q1->rhwBits = FLOAT_ONE_BITS;
                q1->color = 0x80FFFFFF;
                q1->specular = 0xFF000000;
                q1->u = glyphU;
                q1->v = glyphV;
                q2 = (GfxTlVertex *)(g_CreditsTextBuffer.batchNode0->vertexCursor + 0x20);
                q2->x = (float)x0;
                q2->y = (float)vy + 40.0f;
                q2->zBits = fz;
                q2->rhwBits = FLOAT_ONE_BITS;
                q2->color = 0x80FFFFFF;
                q2->specular = 0xFF000000;
                q2->u = glyphU;
                q2->v = glyphV + ch;
                q3 = (GfxTlVertex *)(g_CreditsTextBuffer.batchNode0->vertexCursor + 0x40);
                q3->x = (float)x0 + 32.0f;
                q3->y = (float)vy;
                q3->zBits = fz;
                q3->rhwBits = FLOAT_ONE_BITS;
                q3->color = 0x80FFFFFF;
                q3->specular = 0xFF000000;
                q3->u = glyphU + cw;
                q3->v = glyphV;
                v0 = (GfxTlVertex *)(g_CreditsTextBuffer.batchNode0->vertexCursor + 0x60);
                v0->x = (float)x0 + 32.0f;
                v0->y = (float)vy + 40.0f;
                v0->zBits = fz;
                v0->rhwBits = FLOAT_ONE_BITS;
                v0->color = 0x80FFFFFF;
                v0->specular = 0xFF000000;
                v0->u = glyphU + cw;
                v0->v = glyphV + ch;
                *g_CreditsTextBuffer.batchNode0->colorSignedBytePtr = (char)color;
                g_CreditsTextBuffer.batchNode0->paletteIndex = color;
            }
            x += g_CreditsGlyphWidthTable[cy * 8 + cx];
        }
        s++;
    }
}

/*
 * Draws credits glyph bytes through g_CreditsTextBuffer.batchNodeB until
 * terminator CREDITS_GLYPH_END, using no atlas offset. Bytes up to CREDITS_GLYPH_LAST select an eight-
 * column atlas and produce 16 by 20 quads at viewport-adjusted coordinates,
 * depth 0.1, color 0x80FFFFFF and reciprocal W 1; color selects batch texture
 * state.
 */
// FUNCTION: FF7 0x007a8977
void __cdecl Credits_DrawGlyphString16x20(int x, int y, char *s, int color)
{
    float glyphU;
    float glyphV;
    int x0;
    int vy;
    int fz;
    float cw;
    float ch;
    int cx;
    int cy;
    GfxTlVertex *q1;
    GfxTlVertex *q2;
    GfxTlVertex *q3;
    GfxTlVertex *v0;

    fz = FLOAT_0_1_BITS;
    cw = 0.0625f;
    ch = 0.078125f;
    while (*s != CREDITS_GLYPH_END) {
        if (*s > CREDITS_GLYPH_LAST) {
            switch (*s) {
            case CREDITS_GLYPH_TO_MIDDLE:
                x = g_CreditsViewportWidth >> 1;
                break;
            }
        } else {
            cx = *s % 8;
            cy = *s / 8;
            glyphU = (float)cx * 0.0625f;
            glyphV = (float)cy * 0.078125f;
            if (AcquireBatchNode(1, g_CreditsTextBuffer.batchNodeB) != 0) {
                x0 = x + g_CreditsViewportX;
                vy = y + g_CreditsViewportY;
                q1 = (GfxTlVertex *)(g_CreditsTextBuffer.batchNodeB->vertexCursor);
                q1->x = (float)x0;
                q1->y = (float)vy;
                q1->zBits = fz;
                q1->rhwBits = FLOAT_ONE_BITS;
                q1->color = 0x80FFFFFF;
                q1->specular = 0xFF000000;
                q1->u = glyphU;
                q1->v = glyphV;
                q2 = (GfxTlVertex *)(g_CreditsTextBuffer.batchNodeB->vertexCursor + 0x20);
                q2->x = (float)x0;
                q2->y = (float)vy + 20.0f;
                q2->zBits = fz;
                q2->rhwBits = FLOAT_ONE_BITS;
                q2->color = 0x80FFFFFF;
                q2->specular = 0xFF000000;
                q2->u = glyphU;
                q2->v = glyphV + ch;
                q3 = (GfxTlVertex *)(g_CreditsTextBuffer.batchNodeB->vertexCursor + 0x40);
                q3->x = (float)x0 + 16.0f;
                q3->y = (float)vy;
                q3->zBits = fz;
                q3->rhwBits = FLOAT_ONE_BITS;
                q3->color = 0x80FFFFFF;
                q3->specular = 0xFF000000;
                q3->u = glyphU + cw;
                q3->v = glyphV;
                v0 = (GfxTlVertex *)(g_CreditsTextBuffer.batchNodeB->vertexCursor + 0x60);
                v0->x = (float)x0 + 16.0f;
                v0->y = (float)vy + 20.0f;
                v0->zBits = fz;
                v0->rhwBits = FLOAT_ONE_BITS;
                v0->color = 0x80FFFFFF;
                v0->specular = 0xFF000000;
                v0->u = glyphU + cw;
                v0->v = glyphV + ch;
                *g_CreditsTextBuffer.batchNodeB->colorSignedBytePtr = (char)color;
                g_CreditsTextBuffer.batchNodeB->paletteIndex = color;
            }
            x += g_CreditsGlyphWidthTable[cy * 8 + cx] >> 1;
        }
        s++;
    }
}

/*
 * Draws credits glyph bytes through g_CreditsTextBuffer.batchNodeB until
 * terminator CREDITS_GLYPH_END, using a V atlas offset of 0.5. Bytes up to CREDITS_GLYPH_LAST select an
 * eight-column atlas and produce 16 by 20 quads at viewport-adjusted
 * coordinates, depth 0.1, color 0x80FFFFFF and reciprocal W 1; color selects
 * batch texture state.
 */
// FUNCTION: FF7 0x007a8c02
void __cdecl Credits_DrawGlyphString16x20VHalf(int x, int y, char *s, int color)
{
    float glyphU;
    float glyphV;
    int x0;
    int vy;
    int fz;
    float cw;
    float ch;
    int cx;
    int cy;
    GfxTlVertex *q1;
    GfxTlVertex *q2;
    GfxTlVertex *q3;
    GfxTlVertex *v0;

    fz = FLOAT_0_1_BITS;
    cw = 0.0625f;
    ch = 0.078125f;
    while (*s != CREDITS_GLYPH_END) {
        if (*s > CREDITS_GLYPH_LAST) {
            switch (*s) {
            case CREDITS_GLYPH_TO_MIDDLE:
                x = g_CreditsViewportWidth >> 1;
                break;
            }
        } else {
            cx = *s % 8;
            cy = *s / 8;
            glyphU = (float)cx * 0.0625f;
            glyphV = (float)cy * 0.078125f + 0.5f;
            if (AcquireBatchNode(1, g_CreditsTextBuffer.batchNodeB) != 0) {
                x0 = x + g_CreditsViewportX;
                vy = y + g_CreditsViewportY;
                q1 = (GfxTlVertex *)(g_CreditsTextBuffer.batchNodeB->vertexCursor);
                q1->x = (float)x0;
                q1->y = (float)vy;
                q1->zBits = fz;
                q1->rhwBits = FLOAT_ONE_BITS;
                q1->color = 0x80FFFFFF;
                q1->specular = 0xFF000000;
                q1->u = glyphU;
                q1->v = glyphV;
                q2 = (GfxTlVertex *)(g_CreditsTextBuffer.batchNodeB->vertexCursor + 0x20);
                q2->x = (float)x0;
                q2->y = (float)vy + 20.0f;
                q2->zBits = fz;
                q2->rhwBits = FLOAT_ONE_BITS;
                q2->color = 0x80FFFFFF;
                q2->specular = 0xFF000000;
                q2->u = glyphU;
                q2->v = glyphV + ch;
                q3 = (GfxTlVertex *)(g_CreditsTextBuffer.batchNodeB->vertexCursor + 0x40);
                q3->x = (float)x0 + 16.0f;
                q3->y = (float)vy;
                q3->zBits = fz;
                q3->rhwBits = FLOAT_ONE_BITS;
                q3->color = 0x80FFFFFF;
                q3->specular = 0xFF000000;
                q3->u = glyphU + cw;
                q3->v = glyphV;
                v0 = (GfxTlVertex *)(g_CreditsTextBuffer.batchNodeB->vertexCursor + 0x60);
                v0->x = (float)x0 + 16.0f;
                v0->y = (float)vy + 20.0f;
                v0->zBits = fz;
                v0->rhwBits = FLOAT_ONE_BITS;
                v0->color = 0x80FFFFFF;
                v0->specular = 0xFF000000;
                v0->u = glyphU + cw;
                v0->v = glyphV + ch;
                *g_CreditsTextBuffer.batchNodeB->colorSignedBytePtr = (char)color;
                g_CreditsTextBuffer.batchNodeB->paletteIndex = color;
            }
            x += g_CreditsGlyphWidthTable[cy * 8 + cx] >> 1;
        }
        s++;
    }
}

/*
 * Draws credits glyph bytes through g_CreditsTextBuffer.batchNodeB until
 * terminator CREDITS_GLYPH_END, using a U atlas offset of 0.5. Bytes up to CREDITS_GLYPH_LAST select an
 * eight-column atlas and produce 16 by 20 quads at viewport-adjusted
 * coordinates, depth 0.1, color 0x80FFFFFF and reciprocal W 1; color selects
 * batch texture state.
 */
// FUNCTION: FF7 0x007a8e93
void __cdecl Credits_DrawGlyphString16x20UHalf(int x, int y, char *s, int color)
{
    float glyphU;
    float glyphV;
    int x0;
    int vy;
    int fz;
    float cw;
    float ch;
    int cx;
    int cy;
    GfxTlVertex *q1;
    GfxTlVertex *q2;
    GfxTlVertex *q3;
    GfxTlVertex *v0;

    fz = FLOAT_0_1_BITS;
    cw = 0.0625f;
    ch = 0.078125f;
    while (*s != CREDITS_GLYPH_END) {
        if (*s > CREDITS_GLYPH_LAST) {
            switch (*s) {
            case CREDITS_GLYPH_TO_MIDDLE:
                x = g_CreditsViewportWidth >> 1;
                break;
            }
        } else {
            cx = *s % 8;
            cy = *s / 8;
            glyphU = (float)cx * 0.0625f + 0.5f;
            glyphV = (float)cy * 0.078125f;
            if (AcquireBatchNode(1, g_CreditsTextBuffer.batchNodeB) != 0) {
                x0 = x + g_CreditsViewportX;
                vy = y + g_CreditsViewportY;
                q1 = (GfxTlVertex *)(g_CreditsTextBuffer.batchNodeB->vertexCursor);
                q1->x = (float)x0;
                q1->y = (float)vy;
                q1->zBits = fz;
                q1->rhwBits = FLOAT_ONE_BITS;
                q1->color = 0x80FFFFFF;
                q1->specular = 0xFF000000;
                q1->u = glyphU;
                q1->v = glyphV;
                q2 = (GfxTlVertex *)(g_CreditsTextBuffer.batchNodeB->vertexCursor + 0x20);
                q2->x = (float)x0;
                q2->y = (float)vy + 20.0f;
                q2->zBits = fz;
                q2->rhwBits = FLOAT_ONE_BITS;
                q2->color = 0x80FFFFFF;
                q2->specular = 0xFF000000;
                q2->u = glyphU;
                q2->v = glyphV + ch;
                q3 = (GfxTlVertex *)(g_CreditsTextBuffer.batchNodeB->vertexCursor + 0x40);
                q3->x = (float)x0 + 16.0f;
                q3->y = (float)vy;
                q3->zBits = fz;
                q3->rhwBits = FLOAT_ONE_BITS;
                q3->color = 0x80FFFFFF;
                q3->specular = 0xFF000000;
                q3->u = glyphU + cw;
                q3->v = glyphV;
                v0 = (GfxTlVertex *)(g_CreditsTextBuffer.batchNodeB->vertexCursor + 0x60);
                v0->x = (float)x0 + 16.0f;
                v0->y = (float)vy + 20.0f;
                v0->zBits = fz;
                v0->rhwBits = FLOAT_ONE_BITS;
                v0->color = 0x80FFFFFF;
                v0->specular = 0xFF000000;
                v0->u = glyphU + cw;
                v0->v = glyphV + ch;
                *g_CreditsTextBuffer.batchNodeB->colorSignedBytePtr = (char)color;
                g_CreditsTextBuffer.batchNodeB->paletteIndex = color;
            }
            x += g_CreditsGlyphWidthTable[cy * 8 + cx] >> 1;
        }
        s++;
    }
}

/*
 * Draws credits glyph bytes through g_CreditsTextBuffer.batchNodeB until
 * terminator CREDITS_GLYPH_END, using U and V offsets of 0.5. Bytes up to CREDITS_GLYPH_LAST select an
 * eight-column atlas and produce 8 by 10 quads at viewport-adjusted
 * coordinates, depth 0.1, color 0x80FFFFFF and reciprocal W 1; color selects
 * batch texture state.
 */
// FUNCTION: FF7 0x007a9124
void __cdecl Credits_DrawGlyphString8x10UVHalf(int x, int y, char *s, int color)
{
    float glyphU;
    float glyphV;
    int x0;
    int vy;
    int fz;
    float cw;
    float ch;
    int cx;
    int cy;
    GfxTlVertex *q1;
    GfxTlVertex *q2;
    GfxTlVertex *q3;
    GfxTlVertex *v0;

    fz = FLOAT_0_1_BITS;
    cw = 0.03125f;
    ch = 0.0390625f;
    while (*s != CREDITS_GLYPH_END) {
        if (*s > CREDITS_GLYPH_LAST) {
            switch (*s) {
            case CREDITS_GLYPH_TO_MIDDLE:
                x = g_CreditsViewportWidth >> 1;
                break;
            }
        } else {
            cx = *s % 8;
            cy = *s / 8;
            glyphU = (float)cx * 0.03125f + 0.5f;
            glyphV = (float)cy * 0.0390625f + 0.5f;
            if (AcquireBatchNode(1, g_CreditsTextBuffer.batchNodeB) != 0) {
                x0 = x + g_CreditsViewportX;
                vy = y + g_CreditsViewportY;
                q1 = (GfxTlVertex *)(g_CreditsTextBuffer.batchNodeB->vertexCursor);
                q1->x = (float)x0;
                q1->y = (float)vy;
                q1->zBits = fz;
                q1->rhwBits = FLOAT_ONE_BITS;
                q1->color = 0x80FFFFFF;
                q1->specular = 0xFF000000;
                q1->u = glyphU;
                q1->v = glyphV;
                q2 = (GfxTlVertex *)(g_CreditsTextBuffer.batchNodeB->vertexCursor + 0x20);
                q2->x = (float)x0;
                q2->y = (float)vy + 10.0f;
                q2->zBits = fz;
                q2->rhwBits = FLOAT_ONE_BITS;
                q2->color = 0x80FFFFFF;
                q2->specular = 0xFF000000;
                q2->u = glyphU;
                q2->v = glyphV + ch;
                q3 = (GfxTlVertex *)(g_CreditsTextBuffer.batchNodeB->vertexCursor + 0x40);
                q3->x = (float)x0 + 8.0f;
                q3->y = (float)vy;
                q3->zBits = fz;
                q3->rhwBits = FLOAT_ONE_BITS;
                q3->color = 0x80FFFFFF;
                q3->specular = 0xFF000000;
                q3->u = glyphU + cw;
                q3->v = glyphV;
                v0 = (GfxTlVertex *)(g_CreditsTextBuffer.batchNodeB->vertexCursor + 0x60);
                v0->x = (float)x0 + 8.0f;
                v0->y = (float)vy + 10.0f;
                v0->zBits = fz;
                v0->rhwBits = FLOAT_ONE_BITS;
                v0->color = 0x80FFFFFF;
                v0->specular = 0xFF000000;
                v0->u = glyphU + cw;
                v0->v = glyphV + ch;
                *g_CreditsTextBuffer.batchNodeB->colorSignedBytePtr = (char)color;
                g_CreditsTextBuffer.batchNodeB->paletteIndex = color;
            }
            x += g_CreditsGlyphWidthTable[cy * 8 + cx] >> 2;
        }
        s++;
    }
}

/*
 * Draws credits glyph bytes through g_CreditsTextBuffer.batchNodeB until
 * terminator CREDITS_GLYPH_END, using U offset 0.5 and V offset 0.75. Bytes up to CREDITS_GLYPH_LAST
 * select an eight-column atlas and produce 8 by 10 quads at viewport-adjusted
 * coordinates, depth 0.1, color 0x80FFFFFF and reciprocal W 1; color selects
 * batch texture state.
 */
// FUNCTION: FF7 0x007a93bc
void __cdecl Credits_DrawGlyphString8x10UHalfV3Quarter(int x, int y, char *s, int color)
{
    float glyphU;
    float glyphV;
    int x0;
    int vy;
    int fz;
    float cw;
    float ch;
    int cx;
    int cy;
    GfxTlVertex *q1;
    GfxTlVertex *q2;
    GfxTlVertex *q3;
    GfxTlVertex *v0;

    fz = FLOAT_0_1_BITS;
    cw = 0.03125f;
    ch = 0.0390625f;
    while (*s != CREDITS_GLYPH_END) {
        if (*s > CREDITS_GLYPH_LAST) {
            switch (*s) {
            case CREDITS_GLYPH_TO_MIDDLE:
                x = g_CreditsViewportWidth >> 1;
                break;
            }
        } else {
            cx = *s % 8;
            cy = *s / 8;
            glyphU = (float)cx * 0.03125f + 0.5f;
            glyphV = (float)cy * 0.0390625f + 0.75f;
            if (AcquireBatchNode(1, g_CreditsTextBuffer.batchNodeB) != 0) {
                x0 = x + g_CreditsViewportX;
                vy = y + g_CreditsViewportY;
                q1 = (GfxTlVertex *)(g_CreditsTextBuffer.batchNodeB->vertexCursor);
                q1->x = (float)x0;
                q1->y = (float)vy;
                q1->zBits = fz;
                q1->rhwBits = FLOAT_ONE_BITS;
                q1->color = 0x80FFFFFF;
                q1->specular = 0xFF000000;
                q1->u = glyphU;
                q1->v = glyphV;
                q2 = (GfxTlVertex *)(g_CreditsTextBuffer.batchNodeB->vertexCursor + 0x20);
                q2->x = (float)x0;
                q2->y = (float)vy + 10.0f;
                q2->zBits = fz;
                q2->rhwBits = FLOAT_ONE_BITS;
                q2->color = 0x80FFFFFF;
                q2->specular = 0xFF000000;
                q2->u = glyphU;
                q2->v = glyphV + ch;
                q3 = (GfxTlVertex *)(g_CreditsTextBuffer.batchNodeB->vertexCursor + 0x40);
                q3->x = (float)x0 + 8.0f;
                q3->y = (float)vy;
                q3->zBits = fz;
                q3->rhwBits = FLOAT_ONE_BITS;
                q3->color = 0x80FFFFFF;
                q3->specular = 0xFF000000;
                q3->u = glyphU + cw;
                q3->v = glyphV;
                v0 = (GfxTlVertex *)(g_CreditsTextBuffer.batchNodeB->vertexCursor + 0x60);
                v0->x = (float)x0 + 8.0f;
                v0->y = (float)vy + 10.0f;
                v0->zBits = fz;
                v0->rhwBits = FLOAT_ONE_BITS;
                v0->color = 0x80FFFFFF;
                v0->specular = 0xFF000000;
                v0->u = glyphU + cw;
                v0->v = glyphV + ch;
                *g_CreditsTextBuffer.batchNodeB->colorSignedBytePtr = (char)color;
                g_CreditsTextBuffer.batchNodeB->paletteIndex = color;
            }
            x += g_CreditsGlyphWidthTable[cy * 8 + cx] >> 2;
        }
        s++;
    }
}

/*
 * Draws credits glyph bytes through g_CreditsTextBuffer.batchNodeB until
 * terminator CREDITS_GLYPH_END, using U offset 0.75 and V offset 0.5. Bytes up to CREDITS_GLYPH_LAST
 * select an eight-column atlas and produce 8 by 10 quads at viewport-adjusted
 * coordinates, depth 0.1, color 0x80FFFFFF and reciprocal W 1; color selects
 * batch texture state.
 */
// FUNCTION: FF7 0x007a9654
void __cdecl Credits_DrawGlyphString8x10U3QuarterVHalf(int x, int y, char *s, int color)
{
    float glyphU;
    float glyphV;
    int x0;
    int vy;
    int fz;
    float cw;
    float ch;
    int cx;
    int cy;
    GfxTlVertex *q1;
    GfxTlVertex *q2;
    GfxTlVertex *q3;
    GfxTlVertex *v0;

    fz = FLOAT_0_1_BITS;
    cw = 0.03125f;
    ch = 0.0390625f;
    while (*s != CREDITS_GLYPH_END) {
        if (*s > CREDITS_GLYPH_LAST) {
            switch (*s) {
            case CREDITS_GLYPH_TO_MIDDLE:
                x = g_CreditsViewportWidth >> 1;
                break;
            }
        } else {
            cx = *s % 8;
            cy = *s / 8;
            glyphU = (float)cx * 0.03125f + 0.75f;
            glyphV = (float)cy * 0.0390625f + 0.5f;
            if (AcquireBatchNode(1, g_CreditsTextBuffer.batchNodeB) != 0) {
                x0 = x + g_CreditsViewportX;
                vy = y + g_CreditsViewportY;
                q1 = (GfxTlVertex *)(g_CreditsTextBuffer.batchNodeB->vertexCursor);
                q1->x = (float)x0;
                q1->y = (float)vy;
                q1->zBits = fz;
                q1->rhwBits = FLOAT_ONE_BITS;
                q1->color = 0x80FFFFFF;
                q1->specular = 0xFF000000;
                q1->u = glyphU;
                q1->v = glyphV;
                q2 = (GfxTlVertex *)(g_CreditsTextBuffer.batchNodeB->vertexCursor + 0x20);
                q2->x = (float)x0;
                q2->y = (float)vy + 10.0f;
                q2->zBits = fz;
                q2->rhwBits = FLOAT_ONE_BITS;
                q2->color = 0x80FFFFFF;
                q2->specular = 0xFF000000;
                q2->u = glyphU;
                q2->v = glyphV + ch;
                q3 = (GfxTlVertex *)(g_CreditsTextBuffer.batchNodeB->vertexCursor + 0x40);
                q3->x = (float)x0 + 8.0f;
                q3->y = (float)vy;
                q3->zBits = fz;
                q3->rhwBits = FLOAT_ONE_BITS;
                q3->color = 0x80FFFFFF;
                q3->specular = 0xFF000000;
                q3->u = glyphU + cw;
                q3->v = glyphV;
                v0 = (GfxTlVertex *)(g_CreditsTextBuffer.batchNodeB->vertexCursor + 0x60);
                v0->x = (float)x0 + 8.0f;
                v0->y = (float)vy + 10.0f;
                v0->zBits = fz;
                v0->rhwBits = FLOAT_ONE_BITS;
                v0->color = 0x80FFFFFF;
                v0->specular = 0xFF000000;
                v0->u = glyphU + cw;
                v0->v = glyphV + ch;
                *g_CreditsTextBuffer.batchNodeB->colorSignedBytePtr = (char)color;
                g_CreditsTextBuffer.batchNodeB->paletteIndex = color;
            }
            x += g_CreditsGlyphWidthTable[cy * 8 + cx] >> 2;
        }
        s++;
    }
}
} /* extern "C" */
