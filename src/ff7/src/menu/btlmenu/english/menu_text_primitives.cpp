// src/ff7/src/menu/btlmenu/english/menu_text_primitives.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Drawing primitives for menus at both scales: encoded and fixed-pitch text measurement
//   and drawing, decimal numbers, icons, boxes, bars, gauges and atlas quads, and the batch flush
//   and scissor that bracket each frame's menu output.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/codegen.h"
#include <string.h>
#include "ff7/types/menu.h"
#include "lib/types/graphics.h"
#include "lib/enums/graphics.h"
#include "ff7/enums/main.h"
#include <stdlib.h>
#include "ff7/api/menu.h"
#define FF7_GFX_EMITFLATRECT_FLOAT_Z
#include "lib/api/graphics.h"
#include "lib/api/token.h"
#include "ff7/enums/menu.h"
extern "C" {

struct GfxEngineContext;
#pragma intrinsic(abs, memcpy, memset)

// clang-format off
// GLOBAL: FF7 0x0091f034
char g_MenuMaxGlyphsPerString = 64;
// clang-format on


/* The calls here pass batch objects, so they need this TU's signature rather than the header's. */
int __cdecl Gfx_EmitFlatRect(int x, int y, int width, int height,
                             unsigned int colour, int flags, float z,
                             int surface);

/*
 * Empty: a text hook the menu screens call (0x80 before the shop title) and that ignores its argument.
 */
// FUNCTION: FF7 0x006f5490
void Menu_EmptyTextHook(int what)
{
}

/*
 * Sets the glyph limit g_MenuMaxGlyphsPerString for encoded-string drawing.
 */
// FUNCTION: FF7 0x006f5495
void __cdecl Menu_SetMaxGlyphsPerString(int value)
{
    g_MenuMaxGlyphsPerString = value;
}

/*
 * Measures at most g_MenuMaxGlyphsPerString encoded glyphs, stopping for a null
 * pointer or 0xFF at the current glyph. Prefixes 0xFA through 0xFE consume the
 * next byte and select width-table offsets 0xE7, 0x1B9, 0x2A0, 0x372 or 0x444.
 */
// FUNCTION: FF7 0x006f54a2
int __cdecl Menu_MeasureEncodedString2x(unsigned char *text)
{
    int prefixOfs;
    int glyphIndex;
    int sumWidth;

    glyphIndex = 0;
    sumWidth = glyphIndex;
    for (; glyphIndex < g_MenuMaxGlyphsPerString; glyphIndex++) {
        if (text == 0) {
            break;
        }
        if (*text == TEXT_END) {
            break;
        }
        switch (*text) {
        case TEXT_FONT_PAGE_1:
            text++;
            prefixOfs = 0xe7;
            break;
        case TEXT_FONT_PAGE_2:
            text++;
            prefixOfs = 0x1b9;
            break;
        case TEXT_FONT_PAGE_3:
            text++;
            prefixOfs = 0x2a0;
            break;
        case TEXT_FONT_PAGE_4:
            text++;
            prefixOfs = 0x372;
            break;
        case TEXT_EXTENDED:
            text++;
            prefixOfs = 0x444;
            break;
        default:
            prefixOfs = 0;
            break;
        }
        if (g_MenuModuleActive != 0) {
            sumWidth += (int)((g_pTextSpacing[*text + prefixOfs] >> 5) * 1.6666666f);
            sumWidth += (int)((g_pTextSpacing[*text + prefixOfs] & 0x1f) * 1.6666666f);
        } else {
            sumWidth += (g_pTextSpacing[*text + prefixOfs] >> 5) * 2;
            sumWidth += (g_pTextSpacing[*text + prefixOfs] & 0x1f) * 2;
        }
        text++;
    }
    return sumWidth;
}

/*
 * Returns penX unchanged for prefix 0xF8; prefixes 0xFA through 0xFE select the
 * high byte and different atlas/width-table regions, while other prefixes use the
 * low byte. Remaps glyphs 0xD2/0xD3 for atlas lookup and emits a white 24-pixel-
 * high quad, with width 16 at atlas column 240 and width 24 otherwise, selecting
 * the second batch for columns beyond 240.
 */
// FUNCTION: FF7 0x006f564e
int __cdecl Menu_DrawFontGlyph2x(int penX, int posY, unsigned int colorIndex,
                            unsigned short charCode, float z)
{
    struct {
        struct GfxTlVertex *vertBR, *vertTR, *vertBL, *vertTL;
        short pageShift, padj;
        float spanV, spanU, baseV;
        short rowDelta, padv;
        float baseU;
        unsigned short glyph, padglyph;
        struct GfxRenderObject *batch;
        short spacingBase, pads;
        short clipped, padclipped;
        short atlasX, padatlasX;
        short atlasY, padr;
        short rawAtlasX, padq;
        unsigned char *codeBytes;
        float width;
    } f; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    f.pageShift = 0;
    f.rowDelta = f.pageShift;
    f.codeBytes = (unsigned char *)&charCode;
    switch (*f.codeBytes) {
    case TEXT_BATTLE_EMPHASIS:
        return penX;
    case TEXT_FONT_PAGE_1:
        f.codeBytes++;
        f.atlasY = 0x84;
        f.spacingBase = 0xE7;
        break;
    case TEXT_FONT_PAGE_2:
        f.codeBytes++;
        f.atlasY = 0;
        f.pageShift = 0x10;
        f.spacingBase = 0x1B9;
        break;
    case TEXT_FONT_PAGE_3:
        f.codeBytes++;
        f.atlasY = 0x84;
        f.pageShift = 0x10;
        f.spacingBase = 0x2A0;
        break;
    case TEXT_FONT_PAGE_4:
        f.codeBytes++;
        f.atlasY = 0x84;
        f.spacingBase = 0x372;
        f.rowDelta = -0x40;
        break;
    case TEXT_EXTENDED:
        f.codeBytes++;
        f.atlasY = 0x84;
        f.pageShift = 0x10;
        f.spacingBase = 0x444;
        f.rowDelta = -0x40;
        break;
    default:
        f.spacingBase = 0;
        f.atlasY = 0;
        break;
    }
    charCode = *f.codeBytes;
    f.glyph = *f.codeBytes;
    if (f.glyph == 0xd2 || f.glyph == 0xd3)
        f.glyph -= 0x4e;
    f.rawAtlasX = f.glyph % 21 * 24;
    f.atlasY += f.glyph / 21 * 24;
    if (f.rawAtlasX > 240) {
        f.atlasX = f.rawAtlasX - 256;
        f.width = 24.0f;
        f.clipped = 24;
        f.batch = g_MenuFontBGraphicsObject.batchList;
    } else {
        if (f.rawAtlasX == 240) {
            f.atlasX = f.rawAtlasX;
            f.width = 16.0f;
            f.clipped = 16;
        } else {
            f.atlasX = f.rawAtlasX;
            f.width = 24.0f;
            f.clipped = 24;
        }
        f.batch = g_MenuFontAGraphicsObject;
    }
    if (g_MenuModuleActive != 0) {
        penX += (int)((g_pTextSpacing[charCode + f.spacingBase] >> 5) * 1.6666666f);
    } else {
        penX += (g_pTextSpacing[charCode + f.spacingBase] >> 5) * 2;
    }
    if (Menu_IsTextSubsystemReady() != 0) {
        if (AcquireBatchNode(1, (void *)f.batch) != 0) {
            f.baseU = f.atlasX * 0.00390625f;
            f.baseV = f.atlasY * 0.00390625f;
            f.spanU = f.width * 0.00390625f;
            f.spanV = 0.09375f;
            f.vertTL = f.batch->vertices;
            f.vertTL->x = (float)penX;
            f.vertTL->y = posY;
            f.vertTL->z = z;
            f.vertTL->rhwBits = FLOAT_ONE_BITS;
            f.vertTL->color = 0xFFFFFFFF;
            f.vertTL->specular = 0xFF000000;
            f.vertTL->u = f.baseU;
            f.vertTL->v = f.baseV;
            f.vertBL = f.batch->vertices + 1;
            f.vertBL->x = (float)penX;
            f.vertBL->y = posY + 24.0f;
            f.vertBL->z = z;
            f.vertBL->rhwBits = FLOAT_ONE_BITS;
            f.vertBL->color = 0xFFFFFFFF;
            f.vertBL->specular = 0xFF000000;
            f.vertBL->u = f.baseU;
            f.vertBL->v = f.baseV + f.spanV;
            f.vertTR = f.batch->vertices + 2;
            f.vertTR->x = (float)penX + f.clipped;
            f.vertTR->y = posY;
            f.vertTR->z = z;
            f.vertTR->rhwBits = FLOAT_ONE_BITS;
            f.vertTR->color = 0xFFFFFFFF;
            f.vertTR->specular = 0xFF000000;
            f.vertTR->u = f.baseU + f.spanU;
            f.vertTR->v = f.baseV;
            f.vertBR = f.batch->vertices + 3;
            f.vertBR->x = (float)penX + f.clipped;
            f.vertBR->y = posY + 24.0f;
            f.vertBR->z = z;
            f.vertBR->rhwBits = FLOAT_ONE_BITS;
            f.vertBR->color = 0xFFFFFFFF;
            f.vertBR->specular = 0xFF000000;
            f.vertBR->u = f.baseU + f.spanU;
            f.vertBR->v = f.baseV + f.spanV;
            *f.batch->colorBytePtr = (unsigned char)colorIndex << 1;
            f.batch->texturePageUnsigned = colorIndex << 1;
        }
    }
    if (g_MenuModuleActive != 0) {
        penX += (int)((g_pTextSpacing[charCode + f.spacingBase] & 0x1F) * 1.6666666f);
    } else {
        penX += (g_pTextSpacing[charCode + f.spacingBase] & 0x1F) * 2;
    }
    return penX;
}

/*
 * When the text subsystem is ready and text is nonnull, draws at most
 * g_MenuMaxGlyphsPerString encoded glyphs, stopping at 0xFF. Bytes 0xF8 and
 * 0xFA..0xFE consume the following byte as the high byte of a glyph code; others
 * consume one byte.
 */
// FUNCTION: FF7 0x006f5b03
void __cdecl Menu_DrawEncodedString2x(int startX, int posY, unsigned char *text,
                               unsigned char colorIndex, float z)
{
    short i;

    if (Menu_IsTextSubsystemReady() == 0) {
        return;
    }
    if (text == 0) {
        return;
    }
    for (i = 0; i < g_MenuMaxGlyphsPerString; i++) {
        if (*text == TEXT_END) {
            return;
        }
        if ((*text >= TEXT_FONT_PAGE_1 && *text <= TEXT_EXTENDED) || *text == TEXT_BATTLE_EMPHASIS) {
            startX = Menu_DrawFontGlyph2x(startX, posY, colorIndex,
                                     *text | (text[1] << 8), z);
            text += 2;
        } else {
            startX = Menu_DrawFontGlyph2x(startX, posY, colorIndex, *text++,
                                     z);
        }
    }
}

/*
 * When text is ready, writes white 16-by-16 icon quads at the supplied depth and
 * stores colorIndex as palette selector. Codes zero through 0x28 first draw a
 * fixed upper tile at Y minus 16 then remap the lower tile by adding 64; codes
 * 0x29 through 0x32 use a different upper tile then remap by adding 23.
 */
// FUNCTION: FF7 0x006f5c0c
void __cdecl DrawIconGlyph16_2x(int x, int y, unsigned char code,
                             unsigned char colorIndex, float z)
{
    struct {
        struct GfxTlVertex *l, *k, *j, *i, *h, *g, *f, *e, *d, *c, *b, *a;
        float dv, du, v, u;
        short ty, padty, tx, padtx;
    } s; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    if (Menu_IsTextSubsystemReady() == 0)
        return;
    if (code <= 0x28) {
        if (AcquireBatchNode(1, g_MenuWinDBlend4GraphicsObject) != 0) {
            s.u = 0.0625f;
            s.v = 0.1875f;
            s.du = 0.0625f;
            s.dv = 0.0625f;
            s.a = (struct GfxTlVertex *)(g_MenuWinDBlend4GraphicsObject->vertexCursor);
            s.a->x = (float)x;
            s.a->y = (float)y - 16.0f;
            s.a->z = z;
            s.a->rhwBits = FLOAT_ONE_BITS;
            s.a->color = 0xffffffff;
            s.a->specular = 0xff000000;
            s.a->u = s.u;
            s.a->v = s.v;
            s.b = (struct GfxTlVertex *)(g_MenuWinDBlend4GraphicsObject->vertexCursor + 32);
            s.b->x = (float)x;
            s.b->y = (float)y - 16.0f + 16.0f;
            s.b->z = z;
            s.b->rhwBits = FLOAT_ONE_BITS;
            s.b->color = 0xffffffff;
            s.b->specular = 0xff000000;
            s.b->u = s.u;
            s.b->v = s.v + s.dv;
            s.c = (struct GfxTlVertex *)(g_MenuWinDBlend4GraphicsObject->vertexCursor + 64);
            s.c->x = (float)x + 16.0f;
            s.c->y = (float)y - 16.0f;
            s.c->z = z;
            s.c->rhwBits = FLOAT_ONE_BITS;
            s.c->color = 0xffffffff;
            s.c->specular = 0xff000000;
            s.c->u = s.u + s.du;
            s.c->v = s.v;
            s.d = (struct GfxTlVertex *)(g_MenuWinDBlend4GraphicsObject->vertexCursor + 96);
            s.d->x = (float)x + 16.0f;
            s.d->y = (float)y - 16.0f + 16.0f;
            s.d->z = z;
            s.d->rhwBits = FLOAT_ONE_BITS;
            s.d->color = 0xffffffff;
            s.d->specular = 0xff000000;
            s.d->u = s.u + s.du;
            s.d->v = s.v + s.dv;
            *g_MenuWinDBlend4GraphicsObject->colorBytePtr = colorIndex;
            g_MenuWinDBlend4GraphicsObject->paletteIndex = colorIndex;
        }
        s.tx = ((code + 64) & 15) << 4;
        s.ty = ((code + 64) >> 4) << 4;
    } else if (code >= 0xb4 && code <= 0xcd) {
        s.tx = (code & 15) << 4;
        s.ty = (code >> 4) << 4;
    } else if (code >= 0x29 && code <= 0x32) {
        if (AcquireBatchNode(1, g_MenuWinDBlend4GraphicsObject) != 0) {
            s.u = 0.125f;
            s.v = 0.1875f;
            s.du = 0.0625f;
            s.dv = 0.0625f;
            s.e = (struct GfxTlVertex *)(g_MenuWinDBlend4GraphicsObject->vertexCursor);
            s.e->x = (float)x;
            s.e->y = (float)y - 16.0f;
            s.e->z = z;
            s.e->rhwBits = FLOAT_ONE_BITS;
            s.e->color = 0xffffffff;
            s.e->specular = 0xff000000;
            s.e->u = s.u;
            s.e->v = s.v;
            s.f = (struct GfxTlVertex *)(g_MenuWinDBlend4GraphicsObject->vertexCursor + 32);
            s.f->x = (float)x;
            s.f->y = (float)y - 16.0f + 16.0f;
            s.f->z = z;
            s.f->rhwBits = FLOAT_ONE_BITS;
            s.f->color = 0xffffffff;
            s.f->specular = 0xff000000;
            s.f->u = s.u;
            s.f->v = s.v + s.dv;
            s.g = (struct GfxTlVertex *)(g_MenuWinDBlend4GraphicsObject->vertexCursor + 64);
            s.g->x = (float)x + 16.0f;
            s.g->y = (float)y - 16.0f;
            s.g->z = z;
            s.g->rhwBits = FLOAT_ONE_BITS;
            s.g->color = 0xffffffff;
            s.g->specular = 0xff000000;
            s.g->u = s.u + s.du;
            s.g->v = s.v;
            s.h = (struct GfxTlVertex *)(g_MenuWinDBlend4GraphicsObject->vertexCursor + 96);
            s.h->x = (float)x + 16.0f;
            s.h->y = (float)y - 16.0f + 16.0f;
            s.h->z = z;
            s.h->rhwBits = FLOAT_ONE_BITS;
            s.h->color = 0xffffffff;
            s.h->specular = 0xff000000;
            s.h->u = s.u + s.du;
            s.h->v = s.v + s.dv;
            *g_MenuWinDBlend4GraphicsObject->colorBytePtr = colorIndex;
            g_MenuWinDBlend4GraphicsObject->paletteIndex = colorIndex;
        }
        s.tx = ((code + 23) & 15) << 4;
        s.ty = ((code + 23) >> 4) << 4;
    } else {
        s.tx = (code & 15) << 4;
        s.ty = (code >> 4) << 4;
    }
    if (AcquireBatchNode(1, g_MenuWinDBlend4GraphicsObject) != 0) {
        s.u = s.tx * 0.00390625f;
        s.v = s.ty * 0.00390625f;
        s.du = 0.0625f;
        s.dv = 0.0625f;
        s.i = (struct GfxTlVertex *)(g_MenuWinDBlend4GraphicsObject->vertexCursor);
        s.i->x = (float)x;
        s.i->y = (float)y;
        s.i->z = z;
        s.i->rhwBits = FLOAT_ONE_BITS;
        s.i->color = 0xffffffff;
        s.i->specular = 0xff000000;
        s.i->u = s.u;
        s.i->v = s.v;
        s.j = (struct GfxTlVertex *)(g_MenuWinDBlend4GraphicsObject->vertexCursor + 32);
        s.j->x = (float)x;
        s.j->y = (float)y + 16.0f;
        s.j->z = z;
        s.j->rhwBits = FLOAT_ONE_BITS;
        s.j->color = 0xffffffff;
        s.j->specular = 0xff000000;
        s.j->u = s.u;
        s.j->v = s.v + s.dv;
        s.k = (struct GfxTlVertex *)(g_MenuWinDBlend4GraphicsObject->vertexCursor + 64);
        s.k->x = (float)x + 16.0f;
        s.k->y = (float)y;
        s.k->z = z;
        s.k->rhwBits = FLOAT_ONE_BITS;
        s.k->color = 0xffffffff;
        s.k->specular = 0xff000000;
        s.k->u = s.u + s.du;
        s.k->v = s.v;
        s.l = (struct GfxTlVertex *)(g_MenuWinDBlend4GraphicsObject->vertexCursor + 96);
        s.l->x = (float)x + 16.0f;
        s.l->y = (float)y + 16.0f;
        s.l->z = z;
        s.l->rhwBits = FLOAT_ONE_BITS;
        s.l->color = 0xffffffff;
        s.l->specular = 0xff000000;
        s.l->u = s.u + s.du;
        s.l->v = s.v + s.dv;
        *g_MenuWinDBlend4GraphicsObject->colorBytePtr = colorIndex;
        g_MenuWinDBlend4GraphicsObject->paletteIndex = colorIndex;
    }
}

/*
 * One glyph per cell, at most g_MenuMaxGlyphsPerString of them, stopping at the 0xFF
 * terminator. The cell is 13 wide or 16 wide depending on g_MenuModuleActive.
 */
// FUNCTION: FF7 0x006f62f7
void __cdecl Menu_DrawFixedPitchString2x(int x, int y, unsigned char *text,
                                    unsigned char colorIndex, float z)
{
    short i;

    if (text == 0) {
        return;
    }
    for (i = 0; i < g_MenuMaxGlyphsPerString; i++) {
        if (*text == TEXT_END) {
            return;
        }
        DrawIconGlyph16_2x(x, y, *text++, colorIndex, z);
        if (g_MenuModuleActive != 0) {
            x += 0xd;
        } else {
            x += 0x10;
        }
    }
}

/*
 * When text is ready and the signed gauge->max is nonzero, draws a
 * double- resolution resource gauge from the packed descriptor. Computes primary
 * and secondary widths as signed width times the corresponding signed value
 * divided by maximum, truncating to shorts without clamping.
 */
// FUNCTION: FF7 0x006f638c
void __cdecl Menu_DrawResourceGauge2x(MenuGauge *gauge, float depth)
{
    short leftEdgeX;
    short barHeight;
    short secondGauge;
    struct GfxTlVertex *vert0;
    struct GfxTlVertex *vert1;
    struct GfxTlVertex *vert3;
    struct GfxTlVertex *vertF;
    struct GfxTlVertex *vertH;
    struct GfxTlVertex *vertJ;
    struct GfxTlVertex *vertK;
    struct GfxTlVertex *vertL;
    short value;
    short widthA;
    int vertexColor;
    short positionY;
    struct GfxTlVertex *vert2;
    struct GfxTlVertex *vertE;
    struct GfxTlVertex *vertG;
    struct GfxTlVertex *vertI;

    if (Menu_IsTextSubsystemReady() == 0) {
        return;
    }
    if (gauge->max == 0) {
        return;
    }
    leftEdgeX = gauge->x;
    positionY = gauge->y;
    value = gauge->width;
    barHeight = gauge->height;
    widthA = gauge->width * gauge->low / gauge->max;
    secondGauge = gauge->width * gauge->high / gauge->max;
    if (AcquireBatchNode((gauge->direction != MENU_GAUGE_DIR_STEADY) + 2, g_MenuWindowBgGraphicsObject) != 0) {
        vert0 = (struct GfxTlVertex *)(g_MenuWindowBgGraphicsObject->vertexCursor);
        vert0->x = (float)leftEdgeX;
        vert0->y = (float)positionY;
        vert0->z = depth;
        vert0->rhwBits = FLOAT_ONE_BITS;
        vert0->color = 0x80500000;
        vert0->specular = 0xFF000000;
        vert1 = (struct GfxTlVertex *)(g_MenuWindowBgGraphicsObject->vertexCursor + 0x20);
        vert1->x = (float)leftEdgeX;
        vert1->y = (float)positionY + barHeight + 2.0f;
        vert1->z = depth;
        vert1->rhwBits = FLOAT_ONE_BITS;
        vert1->color = 0x80000000;
        vert1->specular = 0xFF000000;
        vert2 = (struct GfxTlVertex *)(g_MenuWindowBgGraphicsObject->vertexCursor + 0x40);
        vert2->x = (float)leftEdgeX + value;
        vert2->y = (float)positionY;
        vert2->z = depth;
        vert2->rhwBits = FLOAT_ONE_BITS;
        vert2->color = 0x80500000;
        vert2->specular = 0xFF000000;
        vert3 = (struct GfxTlVertex *)(g_MenuWindowBgGraphicsObject->vertexCursor + 0x60);
        vert3->x = (float)leftEdgeX + value;
        vert3->y = (float)positionY + barHeight + 2.0f;
        vert3->z = depth;
        vert3->rhwBits = FLOAT_ONE_BITS;
        vert3->color = 0x80000000;
        vert3->specular = 0xFF000000;
        g_MenuWindowBgGraphicsObject->vertexCursor += g_MenuWindowBgGraphicsObject->cursorStep;
        if (gauge->direction != MENU_GAUGE_DIR_STEADY) {
            if (gauge->direction == MENU_GAUGE_DIR_RISING) {
                vertexColor = 0x8000C850;
            } else {
                vertexColor = 0x80C80000;
            }
            vertE = (struct GfxTlVertex *)(g_MenuWindowBgGraphicsObject->vertexCursor);
            vertE->x = (float)leftEdgeX;
            vertE->y = (float)positionY;
            vertE->z = depth;
            vertE->rhwBits = FLOAT_ONE_BITS;
            vertE->color = vertexColor;
            vertE->specular = 0xFF000000;
            vertF = (struct GfxTlVertex *)(g_MenuWindowBgGraphicsObject->vertexCursor + 0x20);
            vertF->x = (float)leftEdgeX;
            vertF->y = (float)positionY + barHeight;
            vertF->z = depth;
            vertF->rhwBits = FLOAT_ONE_BITS;
            vertF->color = vertexColor;
            vertF->specular = 0xFF000000;
            vertG = (struct GfxTlVertex *)(g_MenuWindowBgGraphicsObject->vertexCursor + 0x40);
            vertG->x = (float)leftEdgeX + secondGauge;
            vertG->y = (float)positionY;
            vertG->z = depth;
            vertG->rhwBits = FLOAT_ONE_BITS;
            vertG->color = vertexColor;
            vertG->specular = 0xFF000000;
            vertH = (struct GfxTlVertex *)(g_MenuWindowBgGraphicsObject->vertexCursor + 0x60);
            vertH->x = (float)leftEdgeX + secondGauge;
            vertH->y = (float)positionY + barHeight;
            vertH->z = depth;
            vertH->rhwBits = FLOAT_ONE_BITS;
            vertH->color = vertexColor;
            vertH->specular = 0xFF000000;
            g_MenuWindowBgGraphicsObject->vertexCursor += g_MenuWindowBgGraphicsObject->cursorStep;
        }
        vertexColor = gauge->colour;
        vertI = (struct GfxTlVertex *)(g_MenuWindowBgGraphicsObject->vertexCursor);
        vertI->x = (float)leftEdgeX;
        vertI->y = (float)positionY;
        vertI->z = depth;
        vertI->rhwBits = FLOAT_ONE_BITS;
        vertI->color = vertexColor;
        vertI->specular = 0xFF000000;
        vertJ = (struct GfxTlVertex *)(g_MenuWindowBgGraphicsObject->vertexCursor + 0x20);
        vertJ->x = (float)leftEdgeX;
        vertJ->y = (float)positionY + barHeight;
        vertJ->z = depth;
        vertJ->rhwBits = FLOAT_ONE_BITS;
        vertJ->color = vertexColor;
        vertJ->specular = 0xFF000000;
        vertK = (struct GfxTlVertex *)(g_MenuWindowBgGraphicsObject->vertexCursor + 0x40);
        vertK->x = (float)leftEdgeX + widthA;
        vertK->y = (float)positionY;
        vertK->z = depth;
        vertK->rhwBits = FLOAT_ONE_BITS;
        vertK->color = 0x80C8C8C8;
        vertK->specular = 0xFF000000;
        vertL = (struct GfxTlVertex *)(g_MenuWindowBgGraphicsObject->vertexCursor + 0x60);
        vertL->x = (float)leftEdgeX + widthA;
        vertL->y = (float)positionY + barHeight;
        vertL->z = depth;
        vertL->rhwBits = FLOAT_ONE_BITS;
        vertL->color = 0x80C8C8C8;
        vertL->specular = 0xFF000000;
    }
}

/*
 * A grey box one pixel larger than the rectangle on every side, at depth 0.2: the
 * shade is replicated into all three channels under an alpha of 0xA0.
 */
// FUNCTION: FF7 0x006f69aa
void __cdecl Menu_DrawShopPartyTileHighlight2x(int x, int y, int w, int h, int shade)
{
    int colour;

    if (Menu_IsTextSubsystemReady() == 0) {
        return;
    }
    colour = shade << 0x10 | 0xA0000000 | shade << 8 | shade;
    Gfx_EmitFlatRect(x - 1, y - 1, w + 2, h + 2, colour, 0, 0.2f, (int)g_MenuFlatRectGraphicsObject);
}

/*
 * When text is ready and five untextured quads are acquired, draws an etched
 * rectangle from four signed shorts at rect, using depth depth plus 0.001. Emits
 * one-pixel top/bottom/left/right strips colored 0x10101010, 0x70707070,
 * 0x20202020 and 0x70707070, followed by the full rectangle colored 0x50505050.
 */
// FUNCTION: FF7 0x006f6a0c
void __cdecl Menu_DrawEtchedBox2x(EnglishWindowFrame *rect, float depth)
{
    int stripHeight;
    GfxTlVertex *topStripTopPtr;
    GfxTlVertex *topStripBottom;
    GfxTlVertex *bottomStripTopPtr;
    GfxTlVertex *rightStripTopPtr;
    GfxTlVertex *leftStripInnerLowPtr;
    GfxTlVertex *leftStripInnerPtr;
    GfxTlVertex *remainingStripStartLow;
    GfxTlVertex *rightStripInner;
    GfxTlVertex *pVtxRightBottom;
    GfxTlVertex *rectTopLeft;
    int rectangleTop;
    int rectangleLeft;
    GfxTlVertex *pTopRightLowVertex;
    GfxTlVertex *pVtxTopRight;
    GfxTlVertex *pLeftStripTop;
    GfxTlVertex *pBottomRightLowVertex;
    GfxTlVertex *pVtxBottomRight;
    GfxTlVertex *vtxBottomRemainingLow;
    GfxTlVertex *vtxRightInnerLow;
    GfxTlVertex *pFullBottom;
    GfxTlVertex *pFullTopRight;
    GfxTlVertex *rectBottomRightPtr;
    int nextExtentX;

    if (Menu_IsTextSubsystemReady() == 0) {
        return;
    }
    depth += 0.001f;
    if (AcquireBatchNode(5, g_MenuBandGraphicsObject) != 0) {
        rectangleLeft = rect->x;
        rectangleTop = rect->y;
        nextExtentX = rect->width - 1;
        stripHeight = 1;
        topStripTopPtr = (GfxTlVertex *)(g_MenuBandGraphicsObject->vertexCursor);
        topStripTopPtr->x = (float)rectangleLeft;
        topStripTopPtr->y = (float)rectangleTop;
        topStripTopPtr->z = depth;
        topStripTopPtr->rhwBits = FLOAT_ONE_BITS;
        topStripTopPtr->color = 0x10101010;
        topStripTopPtr->specular = 0xFF000000;
        topStripBottom = (GfxTlVertex *)(g_MenuBandGraphicsObject->vertexCursor + 0x20);
        topStripBottom->x = (float)rectangleLeft;
        topStripBottom->y = (float)rectangleTop + stripHeight;
        topStripBottom->z = depth;
        topStripBottom->rhwBits = FLOAT_ONE_BITS;
        topStripBottom->color = 0x10101010;
        topStripBottom->specular = 0xFF000000;
        pVtxTopRight = (GfxTlVertex *)(g_MenuBandGraphicsObject->vertexCursor + 0x40);
        pVtxTopRight->x = (float)rectangleLeft + nextExtentX;
        pVtxTopRight->y = (float)rectangleTop;
        pVtxTopRight->z = depth;
        pVtxTopRight->rhwBits = FLOAT_ONE_BITS;
        pVtxTopRight->color = 0x10101010;
        pVtxTopRight->specular = 0xFF000000;
        pTopRightLowVertex = (GfxTlVertex *)(g_MenuBandGraphicsObject->vertexCursor + 0x60);
        pTopRightLowVertex->x = (float)rectangleLeft + nextExtentX;
        pTopRightLowVertex->y = (float)rectangleTop + stripHeight;
        pTopRightLowVertex->z = depth;
        pTopRightLowVertex->rhwBits = FLOAT_ONE_BITS;
        pTopRightLowVertex->color = 0x10101010;
        pTopRightLowVertex->specular = 0xFF000000;
        g_MenuBandGraphicsObject->vertexCursor += g_MenuBandGraphicsObject->cursorStep;
        rectangleLeft = rect->x;
        rectangleTop = rect->y + rect->height + -1;
        nextExtentX = rect->width - 1;
        stripHeight = 1;
        bottomStripTopPtr = (GfxTlVertex *)(g_MenuBandGraphicsObject->vertexCursor);
        bottomStripTopPtr->x = (float)rectangleLeft;
        bottomStripTopPtr->y = (float)rectangleTop;
        bottomStripTopPtr->z = depth;
        bottomStripTopPtr->rhwBits = FLOAT_ONE_BITS;
        bottomStripTopPtr->color = 0x70707070;
        bottomStripTopPtr->specular = 0xFF000000;
        vtxBottomRemainingLow = (GfxTlVertex *)(g_MenuBandGraphicsObject->vertexCursor + 0x20);
        vtxBottomRemainingLow->x = (float)rectangleLeft;
        vtxBottomRemainingLow->y = (float)rectangleTop + stripHeight;
        vtxBottomRemainingLow->z = depth;
        vtxBottomRemainingLow->rhwBits = FLOAT_ONE_BITS;
        vtxBottomRemainingLow->color = 0x70707070;
        vtxBottomRemainingLow->specular = 0xFF000000;
        pVtxBottomRight = (GfxTlVertex *)(g_MenuBandGraphicsObject->vertexCursor + 0x40);
        pVtxBottomRight->x = (float)rectangleLeft + nextExtentX;
        pVtxBottomRight->y = (float)rectangleTop;
        pVtxBottomRight->z = depth;
        pVtxBottomRight->rhwBits = FLOAT_ONE_BITS;
        pVtxBottomRight->color = 0x70707070;
        pVtxBottomRight->specular = 0xFF000000;
        pBottomRightLowVertex = (GfxTlVertex *)(g_MenuBandGraphicsObject->vertexCursor + 0x60);
        pBottomRightLowVertex->x = (float)rectangleLeft + nextExtentX;
        pBottomRightLowVertex->y = (float)rectangleTop + stripHeight;
        pBottomRightLowVertex->z = depth;
        pBottomRightLowVertex->rhwBits = FLOAT_ONE_BITS;
        pBottomRightLowVertex->color = 0x70707070;
        pBottomRightLowVertex->specular = 0xFF000000;
        g_MenuBandGraphicsObject->vertexCursor += g_MenuBandGraphicsObject->cursorStep;
        rectangleLeft = rect->x;
        rectangleTop = rect->y;
        nextExtentX = 1;
        stripHeight = rect->height - 1;
        pLeftStripTop = (GfxTlVertex *)(g_MenuBandGraphicsObject->vertexCursor);
        pLeftStripTop->x = (float)rectangleLeft;
        pLeftStripTop->y = (float)rectangleTop;
        pLeftStripTop->z = depth;
        pLeftStripTop->rhwBits = FLOAT_ONE_BITS;
        pLeftStripTop->color = 0x20202020;
        pLeftStripTop->specular = 0xFF000000;
        remainingStripStartLow = (GfxTlVertex *)(g_MenuBandGraphicsObject->vertexCursor + 0x20);
        remainingStripStartLow->x = (float)rectangleLeft;
        remainingStripStartLow->y = (float)rectangleTop + stripHeight;
        remainingStripStartLow->z = depth;
        remainingStripStartLow->rhwBits = FLOAT_ONE_BITS;
        remainingStripStartLow->color = 0x20202020;
        remainingStripStartLow->specular = 0xFF000000;
        leftStripInnerPtr = (GfxTlVertex *)(g_MenuBandGraphicsObject->vertexCursor + 0x40);
        leftStripInnerPtr->x = (float)rectangleLeft + nextExtentX;
        leftStripInnerPtr->y = (float)rectangleTop;
        leftStripInnerPtr->z = depth;
        leftStripInnerPtr->rhwBits = FLOAT_ONE_BITS;
        leftStripInnerPtr->color = 0x20202020;
        leftStripInnerPtr->specular = 0xFF000000;
        leftStripInnerLowPtr = (GfxTlVertex *)(g_MenuBandGraphicsObject->vertexCursor + 0x60);
        leftStripInnerLowPtr->x = (float)rectangleLeft + nextExtentX;
        leftStripInnerLowPtr->y = (float)rectangleTop + stripHeight;
        leftStripInnerLowPtr->z = depth;
        leftStripInnerLowPtr->rhwBits = FLOAT_ONE_BITS;
        leftStripInnerLowPtr->color = 0x20202020;
        leftStripInnerLowPtr->specular = 0xFF000000;
        g_MenuBandGraphicsObject->vertexCursor += g_MenuBandGraphicsObject->cursorStep;
        rectangleLeft = rect->x + rect->width + -1;
        rectangleTop = rect->y;
        nextExtentX = 1;
        stripHeight = rect->height;
        rightStripTopPtr = (GfxTlVertex *)(g_MenuBandGraphicsObject->vertexCursor);
        rightStripTopPtr->x = (float)rectangleLeft;
        rightStripTopPtr->y = (float)rectangleTop;
        rightStripTopPtr->z = depth;
        rightStripTopPtr->rhwBits = FLOAT_ONE_BITS;
        rightStripTopPtr->color = 0x70707070;
        rightStripTopPtr->specular = 0xFF000000;
        pVtxRightBottom = (GfxTlVertex *)(g_MenuBandGraphicsObject->vertexCursor + 0x20);
        pVtxRightBottom->x = (float)rectangleLeft;
        pVtxRightBottom->y = (float)rectangleTop + stripHeight;
        pVtxRightBottom->z = depth;
        pVtxRightBottom->rhwBits = FLOAT_ONE_BITS;
        pVtxRightBottom->color = 0x70707070;
        pVtxRightBottom->specular = 0xFF000000;
        rightStripInner = (GfxTlVertex *)(g_MenuBandGraphicsObject->vertexCursor + 0x40);
        rightStripInner->x = (float)rectangleLeft + nextExtentX;
        rightStripInner->y = (float)rectangleTop;
        rightStripInner->z = depth;
        rightStripInner->rhwBits = FLOAT_ONE_BITS;
        rightStripInner->color = 0x70707070;
        rightStripInner->specular = 0xFF000000;
        vtxRightInnerLow = (GfxTlVertex *)(g_MenuBandGraphicsObject->vertexCursor + 0x60);
        vtxRightInnerLow->x = (float)rectangleLeft + nextExtentX;
        vtxRightInnerLow->y = (float)rectangleTop + stripHeight;
        vtxRightInnerLow->z = depth;
        vtxRightInnerLow->rhwBits = FLOAT_ONE_BITS;
        vtxRightInnerLow->color = 0x70707070;
        vtxRightInnerLow->specular = 0xFF000000;
        g_MenuBandGraphicsObject->vertexCursor += g_MenuBandGraphicsObject->cursorStep;
        rectangleLeft = rect->x;
        rectangleTop = rect->y;
        nextExtentX = rect->width;
        stripHeight = rect->height;
        rectTopLeft = (GfxTlVertex *)(g_MenuBandGraphicsObject->vertexCursor);
        rectTopLeft->x = (float)rectangleLeft;
        rectTopLeft->y = (float)rectangleTop;
        rectTopLeft->z = depth;
        rectTopLeft->rhwBits = FLOAT_ONE_BITS;
        rectTopLeft->color = 0x50505050;
        rectTopLeft->specular = 0xFF000000;
        pFullBottom = (GfxTlVertex *)(g_MenuBandGraphicsObject->vertexCursor + 0x20);
        pFullBottom->x = (float)rectangleLeft;
        pFullBottom->y = (float)rectangleTop + stripHeight;
        pFullBottom->z = depth;
        pFullBottom->rhwBits = FLOAT_ONE_BITS;
        pFullBottom->color = 0x50505050;
        pFullBottom->specular = 0xFF000000;
        pFullTopRight = (GfxTlVertex *)(g_MenuBandGraphicsObject->vertexCursor + 0x40);
        pFullTopRight->x = (float)rectangleLeft + nextExtentX;
        pFullTopRight->y = (float)rectangleTop;
        pFullTopRight->z = depth;
        pFullTopRight->rhwBits = FLOAT_ONE_BITS;
        pFullTopRight->color = 0x50505050;
        pFullTopRight->specular = 0xFF000000;
        rectBottomRightPtr = (GfxTlVertex *)(g_MenuBandGraphicsObject->vertexCursor + 0x60);
        rectBottomRightPtr->x = (float)rectangleLeft + nextExtentX;
        rectBottomRightPtr->y = (float)rectangleTop + stripHeight;
        rectBottomRightPtr->z = depth;
        rectBottomRightPtr->rhwBits = FLOAT_ONE_BITS;
        rectBottomRightPtr->color = 0x50505050;
        rectBottomRightPtr->specular = 0xFF000000;
    }
}

/*
 * A bevelled box: four one-pixel edges and a fill, five calls to the same
 * primitive with five greys. Top 0xF0, bottom 0x40, left 0xC8, right 0x70, fill
 * 0xA0, all at alpha 0xA0 -- light above and left, dark below and right, which is
 * what makes it read as raised.
 */
// FUNCTION: FF7 0x006f713b
void __cdecl Menu_DrawRaisedBox2x(short *rect, float z)
{
    if (Menu_IsTextSubsystemReady() == 0) {
        return;
    }
    Gfx_EmitFlatRect(rect[0], rect[1], rect[2] - 1,
                     1, 0xa0f0f0f0, 0, z, (int)g_MenuFlatRectGraphicsObject);
    Gfx_EmitFlatRect(rect[0], rect[1] + rect[3] - 1, rect[2] - 1,
                     1, 0xa0404040, 0, z, (int)g_MenuFlatRectGraphicsObject);
    Gfx_EmitFlatRect(rect[0], rect[1], 1,
                     rect[3] - 1, 0xa0c8c8c8, 0, z, (int)g_MenuFlatRectGraphicsObject);
    Gfx_EmitFlatRect(rect[0] + rect[2] - 1, rect[1], 1,
                     rect[3], 0xa0707070, 0, z, (int)g_MenuFlatRectGraphicsObject);
    Gfx_EmitFlatRect(rect[0], rect[1], rect[2],
                     rect[3], 0xa0a0a0a0, 0, z, (int)g_MenuFlatRectGraphicsObject);
}

/*
 * Draw a scroll bar: a raised thumb sized and placed out of the three counts in
 * `desc`, then an etched frame around the whole track. `desc` is seven shorts:
 * desc[0] the visible run, desc[1] the total, desc[2] the first visible row, desc[3]
 * x, desc[4] y, desc[5] width, desc[6] height.
 */
// FUNCTION: FF7 0x006f7270
void __cdecl Menu_DrawScrollBar2x(short *desc, float z)
{
    EnglishWindowFrame rect;
    short len;
    short gap;

    if (Menu_IsTextSubsystemReady() == 0) {
        return;
    }
    len = (short)(desc[6] * desc[0] / desc[1]);
    gap = (short)(desc[6] * desc[2] / desc[1]);
    rect.x = desc[3];
    rect.y = (short)(desc[4] + gap);
    rect.width = (short)(desc[5] - 1);
    rect.height = (short)(len + 1);
    Menu_DrawRaisedBox2x(&rect.x, z);
    rect.x = desc[3];
    rect.y = desc[4];
    rect.width = desc[5];
    rect.height = desc[6];
    Menu_DrawEtchedBox2x(&rect, z);
}

/*
 * When text is ready and acquisition of two batch nodes succeeds, fills two four-
 * vertex untextured gauge quads at the supplied depth with reciprocal W 1 and
 * black specular fields. The lower half interpolates from color 0x90808080 at its
 * top to fillColor at its bottom; the upper half interpolates from 0x90000000 to fillColor.
 */
// FUNCTION: FF7 0x006f7346
void __cdecl Menu_DrawGaugeBar2x(int originX, int originY, int width, int height, unsigned int fillColor, float depth)
{
    GfxTlVertex *lowerVertexA;
    GfxTlVertex *lowerBottomLeftPtr;
    GfxTlVertex *lowerBottomRight;
    GfxTlVertex *lowerRightTop;
    GfxTlVertex *upperLeftTop;
    GfxTlVertex *pUpperBottomLeft;
    GfxTlVertex *upperBottomRight;
    GfxTlVertex *upperTopRight;

    if (Menu_IsTextSubsystemReady() == 0) {
        return;
    }
    if (AcquireBatchNode(2, g_MenuGaugeBarGraphicsObject) != 0) {
        lowerVertexA = (GfxTlVertex *)(g_MenuGaugeBarGraphicsObject->vertexCursor);
        lowerVertexA->x = (float)originX;
        lowerVertexA->y = (float)originY + height / 2;
        lowerVertexA->z = depth;
        lowerVertexA->rhwBits = FLOAT_ONE_BITS;
        lowerVertexA->color = 0x90808080;
        lowerVertexA->specular = 0xFF000000;
        lowerBottomLeftPtr = (GfxTlVertex *)(g_MenuGaugeBarGraphicsObject->vertexCursor + 0x20);
        lowerBottomLeftPtr->x = (float)originX;
        lowerBottomLeftPtr->y = (float)originY + height;
        lowerBottomLeftPtr->z = depth;
        lowerBottomLeftPtr->rhwBits = FLOAT_ONE_BITS;
        lowerBottomLeftPtr->color = fillColor;
        lowerBottomLeftPtr->specular = 0xFF000000;
        lowerRightTop = (GfxTlVertex *)(g_MenuGaugeBarGraphicsObject->vertexCursor + 0x40);
        lowerRightTop->x = (float)originX + width;
        lowerRightTop->y = (float)originY + height / 2;
        lowerRightTop->z = depth;
        lowerRightTop->rhwBits = FLOAT_ONE_BITS;
        lowerRightTop->color = 0x90808080;
        lowerRightTop->specular = 0xFF000000;
        lowerBottomRight = (GfxTlVertex *)(g_MenuGaugeBarGraphicsObject->vertexCursor + 0x60);
        lowerBottomRight->x = (float)originX + width;
        lowerBottomRight->y = (float)originY + height;
        lowerBottomRight->z = depth;
        lowerBottomRight->rhwBits = FLOAT_ONE_BITS;
        lowerBottomRight->color = fillColor;
        lowerBottomRight->specular = 0xFF000000;
        g_MenuGaugeBarGraphicsObject->vertexCursor += g_MenuGaugeBarGraphicsObject->cursorStep;
        upperLeftTop = (GfxTlVertex *)(g_MenuGaugeBarGraphicsObject->vertexCursor);
        upperLeftTop->x = (float)originX;
        upperLeftTop->y = (float)originY;
        upperLeftTop->z = depth;
        upperLeftTop->rhwBits = FLOAT_ONE_BITS;
        upperLeftTop->color = 0x90000000;
        upperLeftTop->specular = 0xFF000000;
        pUpperBottomLeft = (GfxTlVertex *)(g_MenuGaugeBarGraphicsObject->vertexCursor + 0x20);
        pUpperBottomLeft->x = (float)originX;
        pUpperBottomLeft->y = (float)originY + height / 2;
        pUpperBottomLeft->z = depth;
        pUpperBottomLeft->rhwBits = FLOAT_ONE_BITS;
        pUpperBottomLeft->color = fillColor;
        pUpperBottomLeft->specular = 0xFF000000;
        upperTopRight = (GfxTlVertex *)(g_MenuGaugeBarGraphicsObject->vertexCursor + 0x40);
        upperTopRight->x = (float)originX + width;
        upperTopRight->y = (float)originY;
        upperTopRight->z = depth;
        upperTopRight->rhwBits = FLOAT_ONE_BITS;
        upperTopRight->color = 0x90000000;
        upperTopRight->specular = 0xFF000000;
        upperBottomRight = (GfxTlVertex *)(g_MenuGaugeBarGraphicsObject->vertexCursor + 0x60);
        upperBottomRight->x = (float)originX + width;
        upperBottomRight->y = (float)originY + height / 2;
        upperBottomRight->z = depth;
        upperBottomRight->rhwBits = FLOAT_ONE_BITS;
        upperBottomRight->color = fillColor;
        upperBottomRight->specular = 0xFF000000;
    }
}

/*
 * When text is ready, draws black fading bands across width width at depth 0.3.
 * With g_MenuTifaReelFadeActive nonzero, acquires two quads and uses full-alpha black at the
 * top/bottom edges fading to zero over the integer-divided top/bottom thirds,
 * advancing the pointer between quads.
 */
// FUNCTION: FF7 0x006f7607
void Menu_DrawTifaReelFadeBands2x(int x, int y, int width, int height)
{
    struct {
        GfxTlVertex *loopH;
        GfxTlVertex *loopG;
        GfxTlVertex *loopF;
        GfxTlVertex *loopE;
        GfxTlVertex *loopD;
        GfxTlVertex *loopC;
        GfxTlVertex *loopB;
        GfxTlVertex *loopA;
        int upperY;
        int lowerY;
        MenuColorWord bottomColor;
        int i;
        MenuColorWord topColor;
        GfxTlVertex *simpleH;
        GfxTlVertex *simpleG;
        GfxTlVertex *simpleF;
        GfxTlVertex *simpleE;
        GfxTlVertex *simpleD;
        GfxTlVertex *simpleC;
        GfxTlVertex *simpleB;
        GfxTlVertex *simpleA;
        float depth;
    } bands;

    if (Menu_IsTextSubsystemReady() == 0) {
        return;
    }
    bands.depth = 0.3f;
    if (g_MenuTifaReelFadeActive != 0) {
        if (AcquireBatchNode(2, g_MenuBlendWindowBgGraphicsObject) != 0) {
            bands.simpleA = (GfxTlVertex *)(g_MenuBlendWindowBgGraphicsObject->vertexCursor);
            bands.simpleA->x = (float)x;
            bands.simpleA->y = (float)y;
            bands.simpleA->z = bands.depth;
            bands.simpleA->rhwBits = FLOAT_ONE_BITS;
            bands.simpleA->color = 0xFF000000;
            bands.simpleA->specular = 0xFF000000;
            bands.simpleB = (GfxTlVertex *)(g_MenuBlendWindowBgGraphicsObject->vertexCursor + 0x20);
            bands.simpleB->x = (float)x;
            bands.simpleB->y = (float)y + height / 3;
            bands.simpleB->z = bands.depth;
            bands.simpleB->rhwBits = FLOAT_ONE_BITS;
            bands.simpleB->color = 0;
            bands.simpleB->specular = 0xFF000000;
            bands.simpleC = (GfxTlVertex *)(g_MenuBlendWindowBgGraphicsObject->vertexCursor + 0x40);
            bands.simpleC->x = (float)x + width;
            bands.simpleC->y = (float)y;
            bands.simpleC->z = bands.depth;
            bands.simpleC->rhwBits = FLOAT_ONE_BITS;
            bands.simpleC->color = 0xFF000000;
            bands.simpleC->specular = 0xFF000000;
            bands.simpleD = (GfxTlVertex *)(g_MenuBlendWindowBgGraphicsObject->vertexCursor + 0x60);
            bands.simpleD->x = (float)x + width;
            bands.simpleD->y = (float)y + height / 3;
            bands.simpleD->z = bands.depth;
            bands.simpleD->rhwBits = FLOAT_ONE_BITS;
            bands.simpleD->color = 0;
            bands.simpleD->specular = 0xFF000000;
            g_MenuBlendWindowBgGraphicsObject->vertexCursor += g_MenuBlendWindowBgGraphicsObject->cursorStep;
            bands.simpleE = (GfxTlVertex *)(g_MenuBlendWindowBgGraphicsObject->vertexCursor);
            bands.simpleE->x = (float)x;
            bands.simpleE->y = (float)y + height - height / 3;
            bands.simpleE->z = bands.depth;
            bands.simpleE->rhwBits = FLOAT_ONE_BITS;
            bands.simpleE->color = 0;
            bands.simpleE->specular = 0xFF000000;
            bands.simpleF = (GfxTlVertex *)(g_MenuBlendWindowBgGraphicsObject->vertexCursor + 0x20);
            bands.simpleF->x = (float)x;
            bands.simpleF->y = (float)y + height;
            bands.simpleF->z = bands.depth;
            bands.simpleF->rhwBits = FLOAT_ONE_BITS;
            bands.simpleF->color = 0xFF000000;
            bands.simpleF->specular = 0xFF000000;
            bands.simpleG = (GfxTlVertex *)(g_MenuBlendWindowBgGraphicsObject->vertexCursor + 0x40);
            bands.simpleG->x = (float)x + width;
            bands.simpleG->y = (float)y + height - height / 3;
            bands.simpleG->z = bands.depth;
            bands.simpleG->rhwBits = FLOAT_ONE_BITS;
            bands.simpleG->color = 0;
            bands.simpleG->specular = 0xFF000000;
            bands.simpleH = (GfxTlVertex *)(g_MenuBlendWindowBgGraphicsObject->vertexCursor + 0x60);
            bands.simpleH->x = (float)x + width;
            bands.simpleH->y = (float)y + height;
            bands.simpleH->z = bands.depth;
            bands.simpleH->rhwBits = FLOAT_ONE_BITS;
            bands.simpleH->color = 0xFF000000;
            bands.simpleH->specular = 0xFF000000;
        }
    } else if (AcquireBatchNode(0x20, g_MenuBandGraphicsObject) != 0) {
        bands.topColor.word = bands.bottomColor.word = 0;
        for (bands.i = 0; bands.i < 0x10; bands.i++) {
            bands.upperY = bands.i * 2;
            bands.lowerY = bands.upperY + 2;
            bands.topColor.bytes[3] = (unsigned char)(0xE0 - bands.i * 14);
            bands.bottomColor.bytes[3] = (unsigned char)(bands.i * 14);
            bands.loopA = (GfxTlVertex *)g_MenuBandGraphicsObject->vertexCursor;
            bands.loopA->x = (float)x;
            bands.loopA->y = (float)y + bands.upperY;
            bands.loopA->z = bands.depth;
            bands.loopA->rhwBits = FLOAT_ONE_BITS;
            bands.loopA->color = bands.topColor.word;
            bands.loopA->specular = 0xFF000000;
            bands.loopB = (GfxTlVertex *)(g_MenuBandGraphicsObject->vertexCursor + 0x20);
            bands.loopB->x = (float)x;
            bands.loopB->y = (float)y + bands.lowerY;
            bands.loopB->z = bands.depth;
            bands.loopB->rhwBits = FLOAT_ONE_BITS;
            bands.loopB->color = bands.topColor.word;
            bands.loopB->specular = 0xFF000000;
            bands.loopC = (GfxTlVertex *)(g_MenuBandGraphicsObject->vertexCursor + 0x40);
            bands.loopC->x = (float)x + width;
            bands.loopC->y = (float)y + bands.upperY;
            bands.loopC->z = bands.depth;
            bands.loopC->rhwBits = FLOAT_ONE_BITS;
            bands.loopC->color = bands.topColor.word;
            bands.loopC->specular = 0xFF000000;
            bands.loopD = (GfxTlVertex *)(g_MenuBandGraphicsObject->vertexCursor + 0x60);
            bands.loopD->x = (float)x + width;
            bands.loopD->y = (float)y + bands.lowerY;
            bands.loopD->z = bands.depth;
            bands.loopD->rhwBits = FLOAT_ONE_BITS;
            bands.loopD->color = bands.topColor.word;
            bands.loopD->specular = 0xFF000000;
            g_MenuBandGraphicsObject->vertexCursor += g_MenuBandGraphicsObject->cursorStep;
            bands.loopE = (GfxTlVertex *)g_MenuBandGraphicsObject->vertexCursor;
            bands.loopE->x = (float)x;
            bands.loopE->y = (float)y + height - bands.lowerY;
            bands.loopE->z = bands.depth;
            bands.loopE->rhwBits = FLOAT_ONE_BITS;
            bands.loopE->color = bands.topColor.word;
            bands.loopE->specular = 0xFF000000;
            bands.loopF = (GfxTlVertex *)(g_MenuBandGraphicsObject->vertexCursor + 0x20);
            bands.loopF->x = (float)x;
            bands.loopF->y = (float)y + height - bands.upperY;
            bands.loopF->z = bands.depth;
            bands.loopF->rhwBits = FLOAT_ONE_BITS;
            bands.loopF->color = bands.topColor.word;
            bands.loopF->specular = 0xFF000000;
            bands.loopG = (GfxTlVertex *)(g_MenuBandGraphicsObject->vertexCursor + 0x40);
            bands.loopG->x = (float)x + width;
            bands.loopG->y = (float)y + height - bands.lowerY;
            bands.loopG->z = bands.depth;
            bands.loopG->rhwBits = FLOAT_ONE_BITS;
            bands.loopG->color = bands.topColor.word;
            bands.loopG->specular = 0xFF000000;
            bands.loopH = (GfxTlVertex *)(g_MenuBandGraphicsObject->vertexCursor + 0x60);
            bands.loopH->x = (float)x + width;
            bands.loopH->y = (float)y + height - bands.upperY;
            bands.loopH->z = bands.depth;
            bands.loopH->rhwBits = FLOAT_ONE_BITS;
            bands.loopH->color = bands.topColor.word;
            bands.loopH->specular = 0xFF000000;
            if (bands.i != 0x10) {
                g_MenuBandGraphicsObject->vertexCursor += g_MenuBandGraphicsObject->cursorStep;
            }
        }
    }
}

/*
 * When the text subsystem is ready, doubles source atlas coordinates and
 * dimensions and emits a screen quad split into one, two or four pieces at the
 * 256-pixel atlas boundaries. Selects one of four normal or alternate atlas
 * batches; alternate uses primary color 0x80FFFFFF and normal 0xFFFFFFFF, with
 * black secondary color, reciprocal W 1, supplied depth and attrByte stored both
 * as a byte and full batch attribute.
 */
// FUNCTION: FF7 0x006f7bfb
void __cdecl Menu_DrawAtlasQuad2x(int screenX, int screenY, int srcU, int srcV, int width, int height, unsigned int attrByte, int useAltAtlas, int unusedArg, float z)
{
    struct {
        struct GfxTlVertex *p[36];
        short mode, padMode, ty1, padTy1;
        float dv, du;
        int colour;
        short yEnd, padYEnd, yStart, padYStart, tx2, padTx2;
        float v;
        short tx1, padTx1;
        float u;
        short xEnd, padXEnd, xStart, padXStart, h2, padH2, h1, padH1;
        struct GfxRenderObject *batch;
        short w2, padW2, w1, padW1, ty2, padTy2;
    } f; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    f.colour = useAltAtlas ? 0x80ffffff : 0xffffffff;
    if (!Menu_IsTextSubsystemReady())
        return;
    srcU *= 2;
    srcV *= 2;
    width *= 2;
    height *= 2;
    f.xStart = srcU >= 256;
    f.xEnd = srcU + width > 256;
    f.yStart = srcV >= 256;
    f.yEnd = srcV + height > 256;
    if (f.xStart == f.xEnd && f.yStart == f.yEnd) {
        f.mode = (f.xStart << 1) | f.yStart;
        switch (f.mode) {
        case MENU_ATLAS_QUADRANT_TOP_LEFT:
            f.tx1 = srcU;
            f.ty1 = srcV;
            if (useAltAtlas)
                f.batch = g_MenuWinABlend1GraphicsObject;
            else
                f.batch = (struct GfxRenderObject *)g_MenuWinABlend4GraphicsObject;
            break;
        case MENU_ATLAS_QUADRANT_BOTTOM_LEFT:
            f.tx1 = srcU;
            f.ty1 = srcV - 256;
            if (useAltAtlas)
                f.batch = g_MenuWinCBlend1GraphicsObject;
            else
                f.batch = (struct GfxRenderObject *)g_MenuWinCBlend4GraphicsObject;
            break;
        case MENU_ATLAS_QUADRANT_TOP_RIGHT:
            f.tx1 = srcU - 256;
            f.ty1 = srcV;
            if (useAltAtlas)
                f.batch = g_MenuWinBBlend1GraphicsObject;
            else
                f.batch = (struct GfxRenderObject *)g_MenuWinBBlend4GraphicsObject;
            break;
        case MENU_ATLAS_QUADRANT_BOTTOM_RIGHT:
            f.tx1 = srcU - 256;
            f.ty1 = srcV - 256;
            if (useAltAtlas)
                f.batch = g_MenuWinDBlend1GraphicsObject;
            else
                f.batch = (struct GfxRenderObject *)g_MenuWinDBlend4GraphicsObject;
            break;
        }
        if (AcquireBatchNode(1, (void *)f.batch)) {
            f.u = f.tx1 * 0.00390625f;
            f.v = f.ty1 * 0.00390625f;
            f.du = width * 0.00390625f;
            f.dv = height * 0.00390625f;
            f.p[35] = f.batch->vertices;
            f.p[35]->x = (float)screenX;
            f.p[35]->y = (float)screenY;
            f.p[35]->z = z;
            f.p[35]->rhwBits = FLOAT_ONE_BITS;
            f.p[35]->color = f.colour;
            f.p[35]->specular = 0xff000000;
            f.p[35]->u = f.u;
            f.p[35]->v = f.v;
            f.p[34] = f.batch->vertices + 1;
            f.p[34]->x = (float)screenX;
            f.p[34]->y = (float)screenY + height;
            f.p[34]->z = z;
            f.p[34]->rhwBits = FLOAT_ONE_BITS;
            f.p[34]->color = f.colour;
            f.p[34]->specular = 0xff000000;
            f.p[34]->u = f.u;
            f.p[34]->v = f.v + f.dv;
            f.p[33] = f.batch->vertices + 2;
            f.p[33]->x = (float)screenX + width;
            f.p[33]->y = (float)screenY;
            f.p[33]->z = z;
            f.p[33]->rhwBits = FLOAT_ONE_BITS;
            f.p[33]->color = f.colour;
            f.p[33]->specular = 0xff000000;
            f.p[33]->u = f.u + f.du;
            f.p[33]->v = f.v;
            f.p[32] = f.batch->vertices + 3;
            f.p[32]->x = (float)screenX + width;
            f.p[32]->y = (float)screenY + height;
            f.p[32]->z = z;
            f.p[32]->rhwBits = FLOAT_ONE_BITS;
            f.p[32]->color = f.colour;
            f.p[32]->specular = 0xff000000;
            f.p[32]->u = f.u + f.du;
            f.p[32]->v = f.v + f.dv;
            *f.batch->colorBytePtr = attrByte;
            f.batch->texturePageUnsigned = attrByte;
        }
    } else {
        if (!f.xStart && f.xEnd) {
            if (f.yStart)
                f.mode = MENU_ATLAS_SPLIT_X_BOTTOM;
            else if (f.yEnd)
                f.mode = MENU_ATLAS_SPLIT_XY;
            else
                f.mode = MENU_ATLAS_SPLIT_X_TOP;
        } else {
            if (f.xStart)
                f.mode = MENU_ATLAS_SPLIT_Y_RIGHT;
            else
                f.mode = MENU_ATLAS_SPLIT_Y_LEFT;
        }
        switch (f.mode) {
        case MENU_ATLAS_SPLIT_X_TOP:
        case MENU_ATLAS_SPLIT_X_BOTTOM:
            f.tx1 = srcU;
            f.ty1 = srcV;
            f.w1 = 256 - srcU;
            f.h1 = height;
            f.tx2 = 0;
            f.ty2 = srcV;
            f.w2 = srcU - 256 + width;
            f.h2 = height;
            if (useAltAtlas)
                f.batch = (struct GfxRenderObject *)(f.mode == MENU_ATLAS_SPLIT_X_TOP ? g_MenuWinABlend1GraphicsObject : g_MenuWinCBlend1GraphicsObject);
            else
                f.batch = (struct GfxRenderObject *)(f.mode == MENU_ATLAS_SPLIT_X_TOP ? g_MenuWinABlend4GraphicsObject : g_MenuWinCBlend4GraphicsObject);
            if (AcquireBatchNode(1, (void *)f.batch)) {
                f.u = f.tx1 * 0.00390625f;
                f.v = f.ty1 * 0.00390625f;
                f.du = f.w1 * 0.00390625f;
                f.dv = f.h1 * 0.00390625f;
                f.p[31] = f.batch->vertices;
                f.p[31]->x = (float)screenX;
                f.p[31]->y = (float)screenY;
                f.p[31]->z = z;
                f.p[31]->rhwBits = FLOAT_ONE_BITS;
                f.p[31]->color = f.colour;
                f.p[31]->specular = 0xff000000;
                f.p[31]->u = f.u;
                f.p[31]->v = f.v;
                f.p[30] = f.batch->vertices + 1;
                f.p[30]->x = (float)screenX;
                f.p[30]->y = (float)screenY + f.h1;
                f.p[30]->z = z;
                f.p[30]->rhwBits = FLOAT_ONE_BITS;
                f.p[30]->color = f.colour;
                f.p[30]->specular = 0xff000000;
                f.p[30]->u = f.u;
                f.p[30]->v = f.v + f.dv;
                f.p[29] = f.batch->vertices + 2;
                f.p[29]->x = (float)screenX + f.w1;
                f.p[29]->y = (float)screenY;
                f.p[29]->z = z;
                f.p[29]->rhwBits = FLOAT_ONE_BITS;
                f.p[29]->color = f.colour;
                f.p[29]->specular = 0xff000000;
                f.p[29]->u = f.u + f.du;
                f.p[29]->v = f.v;
                f.p[28] = f.batch->vertices + 3;
                f.p[28]->x = (float)screenX + f.w1;
                f.p[28]->y = (float)screenY + f.h1;
                f.p[28]->z = z;
                f.p[28]->rhwBits = FLOAT_ONE_BITS;
                f.p[28]->color = f.colour;
                f.p[28]->specular = 0xff000000;
                f.p[28]->u = f.u + f.du;
                f.p[28]->v = f.v + f.dv;
                *f.batch->colorBytePtr = attrByte;
                f.batch->texturePageUnsigned = attrByte;
            }
            if (useAltAtlas)
                f.batch = (struct GfxRenderObject *)(f.mode == MENU_ATLAS_SPLIT_X_TOP ? g_MenuWinBBlend1GraphicsObject : g_MenuWinDBlend1GraphicsObject);
            else
                f.batch = (struct GfxRenderObject *)(f.mode == MENU_ATLAS_SPLIT_X_TOP ? g_MenuWinBBlend4GraphicsObject : g_MenuWinDBlend4GraphicsObject);
            if (AcquireBatchNode(1, (void *)f.batch)) {
                f.u = f.tx2 * 0.00390625f;
                f.v = f.ty2 * 0.00390625f;
                f.du = f.w2 * 0.00390625f;
                f.dv = f.h2 * 0.00390625f;
                screenX += f.w1;
                f.p[27] = f.batch->vertices;
                f.p[27]->x = (float)screenX;
                f.p[27]->y = (float)screenY;
                f.p[27]->z = z;
                f.p[27]->rhwBits = FLOAT_ONE_BITS;
                f.p[27]->color = f.colour;
                f.p[27]->specular = 0xff000000;
                f.p[27]->u = f.u;
                f.p[27]->v = f.v;
                f.p[26] = f.batch->vertices + 1;
                f.p[26]->x = (float)screenX;
                f.p[26]->y = (float)screenY + f.h2;
                f.p[26]->z = z;
                f.p[26]->rhwBits = FLOAT_ONE_BITS;
                f.p[26]->color = f.colour;
                f.p[26]->specular = 0xff000000;
                f.p[26]->u = f.u;
                f.p[26]->v = f.v + f.dv;
                f.p[25] = f.batch->vertices + 2;
                f.p[25]->x = (float)screenX + f.w2;
                f.p[25]->y = (float)screenY;
                f.p[25]->z = z;
                f.p[25]->rhwBits = FLOAT_ONE_BITS;
                f.p[25]->color = f.colour;
                f.p[25]->specular = 0xff000000;
                f.p[25]->u = f.u + f.du;
                f.p[25]->v = f.v;
                f.p[24] = f.batch->vertices + 3;
                f.p[24]->x = (float)screenX + f.w2;
                f.p[24]->y = (float)screenY + f.h2;
                f.p[24]->z = z;
                f.p[24]->rhwBits = FLOAT_ONE_BITS;
                f.p[24]->color = f.colour;
                f.p[24]->specular = 0xff000000;
                f.p[24]->u = f.u + f.du;
                f.p[24]->v = f.v + f.dv;
                *f.batch->colorBytePtr = attrByte;
                f.batch->texturePageUnsigned = attrByte;
            }
            break;
        case MENU_ATLAS_SPLIT_Y_LEFT:
        case MENU_ATLAS_SPLIT_Y_RIGHT:
            f.tx1 = srcU;
            f.ty1 = srcV;
            f.w1 = width;
            f.h1 = 256 - srcV;
            f.tx2 = srcU;
            f.ty2 = 0;
            f.w2 = width;
            f.h2 = srcV - 256 + height;
            if (useAltAtlas)
                f.batch = (struct GfxRenderObject *)(f.mode == MENU_ATLAS_SPLIT_Y_LEFT ? g_MenuWinABlend1GraphicsObject : g_MenuWinBBlend1GraphicsObject);
            else
                f.batch = (struct GfxRenderObject *)(f.mode == MENU_ATLAS_SPLIT_Y_LEFT ? g_MenuWinABlend4GraphicsObject : g_MenuWinBBlend4GraphicsObject);
            if (AcquireBatchNode(1, (void *)f.batch)) {
                f.u = f.tx1 * 0.00390625f;
                f.v = f.ty1 * 0.00390625f;
                f.du = f.w1 * 0.00390625f;
                f.dv = f.h1 * 0.00390625f;
                f.p[23] = f.batch->vertices;
                f.p[23]->x = (float)screenX;
                f.p[23]->y = (float)screenY;
                f.p[23]->z = z;
                f.p[23]->rhwBits = FLOAT_ONE_BITS;
                f.p[23]->color = f.colour;
                f.p[23]->specular = 0xff000000;
                f.p[23]->u = f.u;
                f.p[23]->v = f.v;
                f.p[22] = f.batch->vertices + 1;
                f.p[22]->x = (float)screenX;
                f.p[22]->y = (float)screenY + f.h1;
                f.p[22]->z = z;
                f.p[22]->rhwBits = FLOAT_ONE_BITS;
                f.p[22]->color = f.colour;
                f.p[22]->specular = 0xff000000;
                f.p[22]->u = f.u;
                f.p[22]->v = f.v + f.dv;
                f.p[21] = f.batch->vertices + 2;
                f.p[21]->x = (float)screenX + f.w1;
                f.p[21]->y = (float)screenY;
                f.p[21]->z = z;
                f.p[21]->rhwBits = FLOAT_ONE_BITS;
                f.p[21]->color = f.colour;
                f.p[21]->specular = 0xff000000;
                f.p[21]->u = f.u + f.du;
                f.p[21]->v = f.v;
                f.p[20] = f.batch->vertices + 3;
                f.p[20]->x = (float)screenX + f.w1;
                f.p[20]->y = (float)screenY + f.h1;
                f.p[20]->z = z;
                f.p[20]->rhwBits = FLOAT_ONE_BITS;
                f.p[20]->color = f.colour;
                f.p[20]->specular = 0xff000000;
                f.p[20]->u = f.u + f.du;
                f.p[20]->v = f.v + f.dv;
                *f.batch->colorBytePtr = attrByte;
                f.batch->texturePageUnsigned = attrByte;
            }
            if (useAltAtlas)
                f.batch = (struct GfxRenderObject *)(f.mode == MENU_ATLAS_SPLIT_Y_LEFT ? g_MenuWinCBlend1GraphicsObject : g_MenuWinDBlend1GraphicsObject);
            else
                f.batch = (struct GfxRenderObject *)(f.mode == MENU_ATLAS_SPLIT_Y_LEFT ? g_MenuWinCBlend4GraphicsObject : g_MenuWinDBlend4GraphicsObject);
            if (AcquireBatchNode(1, (void *)f.batch)) {
                f.u = f.tx2 * 0.00390625f;
                f.v = f.ty2 * 0.00390625f;
                f.du = f.w2 * 0.00390625f;
                f.dv = f.h2 * 0.00390625f;
                screenY += f.h1;
                f.p[19] = f.batch->vertices;
                f.p[19]->x = (float)screenX;
                f.p[19]->y = (float)screenY;
                f.p[19]->z = z;
                f.p[19]->rhwBits = FLOAT_ONE_BITS;
                f.p[19]->color = f.colour;
                f.p[19]->specular = 0xff000000;
                f.p[19]->u = f.u;
                f.p[19]->v = f.v;
                f.p[18] = f.batch->vertices + 1;
                f.p[18]->x = (float)screenX;
                f.p[18]->y = (float)screenY + f.h2;
                f.p[18]->z = z;
                f.p[18]->rhwBits = FLOAT_ONE_BITS;
                f.p[18]->color = f.colour;
                f.p[18]->specular = 0xff000000;
                f.p[18]->u = f.u;
                f.p[18]->v = f.v + f.dv;
                f.p[17] = f.batch->vertices + 2;
                f.p[17]->x = (float)screenX + f.w2;
                f.p[17]->y = (float)screenY;
                f.p[17]->z = z;
                f.p[17]->rhwBits = FLOAT_ONE_BITS;
                f.p[17]->color = f.colour;
                f.p[17]->specular = 0xff000000;
                f.p[17]->u = f.u + f.du;
                f.p[17]->v = f.v;
                f.p[16] = f.batch->vertices + 3;
                f.p[16]->x = (float)screenX + f.w2;
                f.p[16]->y = (float)screenY + f.h2;
                f.p[16]->z = z;
                f.p[16]->rhwBits = FLOAT_ONE_BITS;
                f.p[16]->color = f.colour;
                f.p[16]->specular = 0xff000000;
                f.p[16]->u = f.u + f.du;
                f.p[16]->v = f.v + f.dv;
                *f.batch->colorBytePtr = attrByte;
                f.batch->texturePageUnsigned = attrByte;
            }
            break;
        case MENU_ATLAS_SPLIT_XY:
            f.tx1 = srcU;
            f.ty1 = srcV;
            f.w1 = 256 - srcU;
            f.h1 = 256 - srcV;
            f.tx2 = 0;
            f.ty2 = srcV;
            f.w2 = srcU - 256 + width;
            f.h2 = 256 - srcV;
            if (useAltAtlas)
                f.batch = g_MenuWinABlend1GraphicsObject;
            else
                f.batch = (struct GfxRenderObject *)g_MenuWinABlend4GraphicsObject;
            if (AcquireBatchNode(1, (void *)f.batch)) {
                f.u = f.tx1 * 0.00390625f;
                f.v = f.ty1 * 0.00390625f;
                f.du = f.w1 * 0.00390625f;
                f.dv = f.h1 * 0.00390625f;
                f.p[15] = f.batch->vertices;
                f.p[15]->x = (float)screenX;
                f.p[15]->y = (float)screenY;
                f.p[15]->z = z;
                f.p[15]->rhwBits = FLOAT_ONE_BITS;
                f.p[15]->color = f.colour;
                f.p[15]->specular = 0xff000000;
                f.p[15]->u = f.u;
                f.p[15]->v = f.v;
                f.p[14] = f.batch->vertices + 1;
                f.p[14]->x = (float)screenX;
                f.p[14]->y = (float)screenY + f.h1;
                f.p[14]->z = z;
                f.p[14]->rhwBits = FLOAT_ONE_BITS;
                f.p[14]->color = f.colour;
                f.p[14]->specular = 0xff000000;
                f.p[14]->u = f.u;
                f.p[14]->v = f.v + f.dv;
                f.p[13] = f.batch->vertices + 2;
                f.p[13]->x = (float)screenX + f.w1;
                f.p[13]->y = (float)screenY;
                f.p[13]->z = z;
                f.p[13]->rhwBits = FLOAT_ONE_BITS;
                f.p[13]->color = f.colour;
                f.p[13]->specular = 0xff000000;
                f.p[13]->u = f.u + f.du;
                f.p[13]->v = f.v;
                f.p[12] = f.batch->vertices + 3;
                f.p[12]->x = (float)screenX + f.w1;
                f.p[12]->y = (float)screenY + f.h1;
                f.p[12]->z = z;
                f.p[12]->rhwBits = FLOAT_ONE_BITS;
                f.p[12]->color = f.colour;
                f.p[12]->specular = 0xff000000;
                f.p[12]->u = f.u + f.du;
                f.p[12]->v = f.v + f.dv;
                *f.batch->colorBytePtr = attrByte;
                f.batch->texturePageUnsigned = attrByte;
            }
            if (useAltAtlas)
                f.batch = g_MenuWinBBlend1GraphicsObject;
            else
                f.batch = (struct GfxRenderObject *)g_MenuWinBBlend4GraphicsObject;
            if (AcquireBatchNode(1, (void *)f.batch)) {
                f.u = f.tx2 * 0.00390625f;
                f.v = f.ty2 * 0.00390625f;
                f.du = f.w2 * 0.00390625f;
                f.dv = f.h2 * 0.00390625f;
                screenX += f.w1;
                f.p[11] = f.batch->vertices;
                f.p[11]->x = (float)screenX;
                f.p[11]->y = (float)screenY;
                f.p[11]->z = z;
                f.p[11]->rhwBits = FLOAT_ONE_BITS;
                f.p[11]->color = f.colour;
                f.p[11]->specular = 0xff000000;
                f.p[11]->u = f.u;
                f.p[11]->v = f.v;
                f.p[10] = f.batch->vertices + 1;
                f.p[10]->x = (float)screenX;
                f.p[10]->y = (float)screenY + f.h2;
                f.p[10]->z = z;
                f.p[10]->rhwBits = FLOAT_ONE_BITS;
                f.p[10]->color = f.colour;
                f.p[10]->specular = 0xff000000;
                f.p[10]->u = f.u;
                f.p[10]->v = f.v + f.dv;
                f.p[9] = f.batch->vertices + 2;
                f.p[9]->x = (float)screenX + f.w2;
                f.p[9]->y = (float)screenY;
                f.p[9]->z = z;
                f.p[9]->rhwBits = FLOAT_ONE_BITS;
                f.p[9]->color = f.colour;
                f.p[9]->specular = 0xff000000;
                f.p[9]->u = f.u + f.du;
                f.p[9]->v = f.v;
                f.p[8] = f.batch->vertices + 3;
                f.p[8]->x = (float)screenX + f.w2;
                f.p[8]->y = (float)screenY + f.h2;
                f.p[8]->z = z;
                f.p[8]->rhwBits = FLOAT_ONE_BITS;
                f.p[8]->color = f.colour;
                f.p[8]->specular = 0xff000000;
                f.p[8]->u = f.u + f.du;
                f.p[8]->v = f.v + f.dv;
                *f.batch->colorBytePtr = attrByte;
                f.batch->texturePageUnsigned = attrByte;
            }

            screenX -= f.w1;
            screenY += f.h1;
            f.ty1 = 0;
            f.h1 = srcV - 256 + height;
            f.ty2 = 0;
            f.h2 = srcV - 256 + height;
            if (useAltAtlas)
                f.batch = g_MenuWinCBlend1GraphicsObject;
            else
                f.batch = (struct GfxRenderObject *)g_MenuWinCBlend4GraphicsObject;
            if (AcquireBatchNode(1, (void *)f.batch)) {
                f.u = f.tx1 * 0.00390625f;
                f.v = f.ty1 * 0.00390625f;
                f.du = f.w1 * 0.00390625f;
                f.dv = f.h1 * 0.00390625f;
                f.p[7] = f.batch->vertices;
                f.p[7]->x = (float)screenX;
                f.p[7]->y = (float)screenY;
                f.p[7]->z = z;
                f.p[7]->rhwBits = FLOAT_ONE_BITS;
                f.p[7]->color = f.colour;
                f.p[7]->specular = 0xff000000;
                f.p[7]->u = f.u;
                f.p[7]->v = f.v;
                f.p[6] = f.batch->vertices + 1;
                f.p[6]->x = (float)screenX;
                f.p[6]->y = (float)screenY + f.h1;
                f.p[6]->z = z;
                f.p[6]->rhwBits = FLOAT_ONE_BITS;
                f.p[6]->color = f.colour;
                f.p[6]->specular = 0xff000000;
                f.p[6]->u = f.u;
                f.p[6]->v = f.v + f.dv;
                f.p[5] = f.batch->vertices + 2;
                f.p[5]->x = (float)screenX + f.w1;
                f.p[5]->y = (float)screenY;
                f.p[5]->z = z;
                f.p[5]->rhwBits = FLOAT_ONE_BITS;
                f.p[5]->color = f.colour;
                f.p[5]->specular = 0xff000000;
                f.p[5]->u = f.u + f.du;
                f.p[5]->v = f.v;
                f.p[4] = f.batch->vertices + 3;
                f.p[4]->x = (float)screenX + f.w1;
                f.p[4]->y = (float)screenY + f.h1;
                f.p[4]->z = z;
                f.p[4]->rhwBits = FLOAT_ONE_BITS;
                f.p[4]->color = f.colour;
                f.p[4]->specular = 0xff000000;
                f.p[4]->u = f.u + f.du;
                f.p[4]->v = f.v + f.dv;
                *f.batch->colorBytePtr = attrByte;
                f.batch->texturePageUnsigned = attrByte;
            }
            if (useAltAtlas)
                f.batch = g_MenuWinDBlend1GraphicsObject;
            else
                f.batch = (struct GfxRenderObject *)g_MenuWinDBlend4GraphicsObject;
            if (AcquireBatchNode(1, (void *)f.batch)) {
                f.u = f.tx2 * 0.00390625f;
                f.v = f.ty2 * 0.00390625f;
                f.du = f.w2 * 0.00390625f;
                f.dv = f.h2 * 0.00390625f;
                screenX += f.w1;
                f.p[3] = f.batch->vertices;
                f.p[3]->x = (float)screenX;
                f.p[3]->y = (float)screenY;
                f.p[3]->z = z;
                f.p[3]->rhwBits = FLOAT_ONE_BITS;
                f.p[3]->color = f.colour;
                f.p[3]->specular = 0xff000000;
                f.p[3]->u = f.u;
                f.p[3]->v = f.v;
                f.p[2] = f.batch->vertices + 1;
                f.p[2]->x = (float)screenX;
                f.p[2]->y = (float)screenY + f.h2;
                f.p[2]->z = z;
                f.p[2]->rhwBits = FLOAT_ONE_BITS;
                f.p[2]->color = f.colour;
                f.p[2]->specular = 0xff000000;
                f.p[2]->u = f.u;
                f.p[2]->v = f.v + f.dv;
                f.p[1] = f.batch->vertices + 2;
                f.p[1]->x = (float)screenX + f.w2;
                f.p[1]->y = (float)screenY;
                f.p[1]->z = z;
                f.p[1]->rhwBits = FLOAT_ONE_BITS;
                f.p[1]->color = f.colour;
                f.p[1]->specular = 0xff000000;
                f.p[1]->u = f.u + f.du;
                f.p[1]->v = f.v;
                f.p[0] = f.batch->vertices + 3;
                f.p[0]->x = (float)screenX + f.w2;
                f.p[0]->y = (float)screenY + f.h2;
                f.p[0]->z = z;
                f.p[0]->rhwBits = FLOAT_ONE_BITS;
                f.p[0]->color = f.colour;
                f.p[0]->specular = 0xff000000;
                f.p[0]->u = f.u + f.du;
                f.p[0]->v = f.v + f.dv;
                *f.batch->colorBytePtr = attrByte;
                f.batch->texturePageUnsigned = attrByte;
            }
            break;
        }
    }
}

/*
 * When text is ready, draws an unsigned decimal value with leading zeroes
 * suppressed and a final digit emitted unconditionally, using white 14-by-16 quads
 * and colorIndex as the palette selector. Clamps values above 999 only when
 * digitCount equals three.
 */
// FUNCTION: FF7 0x006f9739
void __cdecl Menu_DrawDecimalNumber2x(int x, int y, unsigned int value, char digitCount, char colorIndex, float z)
{
    struct {
        float dv;
        float du;
        float v;
        float u;
        short digit;
        int screenX;
        unsigned int remaining;
        unsigned int divisor;
        short leadingZero;
    } state;

    if (Menu_IsTextSubsystemReady() == 0) {
        return;
    }
    state.divisor = 1;
    state.leadingZero = (short)state.divisor;
    for (state.digit = 1; state.digit < digitCount; state.digit++) {
        state.divisor *= 10;
    }
    state.remaining = value;
    if (digitCount == 3 && state.remaining > 999) {
        state.remaining = 999;
    }
    for (state.digit = 1; state.digit < digitCount; state.digit++) {
        if (state.leadingZero == 0 || state.remaining / state.divisor != 0) {
            state.leadingZero = 0;
            if (AcquireBatchNode(1, g_MenuWinBBlend4GraphicsObject) != 0) {
                GfxTlVertex *topRemaining;
                GfxTlVertex *b;
                GfxTlVertex *nextCorner;
                GfxTlVertex *bottomRightAddr;

                state.u = (float)((state.remaining / state.divisor) * 0x10 + 0x10) * 0.00390625f;
                state.v = 0.0f;
                state.du = 0.0546875f;
                state.dv = 0.0625f;
                if (g_MenuModuleActive != 0) {
                    state.screenX = x + (state.digit - 1) * 12;
                } else {
                    state.screenX = x + (state.digit - 1) * 14;
                }
                topRemaining = (GfxTlVertex *)(g_MenuWinBBlend4GraphicsObject->vertexCursor);
                topRemaining->x = (float)state.screenX;
                topRemaining->y = (float)y;
                topRemaining->z = z;
                topRemaining->rhwBits = FLOAT_ONE_BITS;
                topRemaining->color = 0xFFFFFFFF;
                topRemaining->specular = 0xFF000000;
                topRemaining->u = state.u;
                topRemaining->v = state.v;
                b = (GfxTlVertex *)(g_MenuWinBBlend4GraphicsObject->vertexCursor + 0x20);
                b->x = (float)state.screenX;
                b->y = (float)y + 16.0f;
                b->z = z;
                b->rhwBits = FLOAT_ONE_BITS;
                b->color = 0xFFFFFFFF;
                b->specular = 0xFF000000;
                b->u = state.u;
                b->v = state.v + state.dv;
                nextCorner = (GfxTlVertex *)(g_MenuWinBBlend4GraphicsObject->vertexCursor + 0x40);
                nextCorner->x = (float)state.screenX + 14.0f;
                nextCorner->y = (float)y;
                nextCorner->z = z;
                nextCorner->rhwBits = FLOAT_ONE_BITS;
                nextCorner->color = 0xFFFFFFFF;
                nextCorner->specular = 0xFF000000;
                nextCorner->u = state.u + state.du;
                nextCorner->v = state.v;
                bottomRightAddr = (GfxTlVertex *)(g_MenuWinBBlend4GraphicsObject->vertexCursor + 0x60);
                bottomRightAddr->x = (float)state.screenX + 14.0f;
                bottomRightAddr->y = (float)y + 16.0f;
                bottomRightAddr->z = z;
                bottomRightAddr->rhwBits = FLOAT_ONE_BITS;
                bottomRightAddr->color = 0xFFFFFFFF;
                bottomRightAddr->specular = 0xFF000000;
                bottomRightAddr->u = state.u + state.du;
                bottomRightAddr->v = state.v + state.dv;
                *g_MenuWinBBlend4GraphicsObject->colorBytePtr = colorIndex;
                g_MenuWinBBlend4GraphicsObject->paletteIndex = colorIndex;
            }
        }
        state.remaining %= state.divisor;
        state.divisor /= 10;
    }
    if (AcquireBatchNode(1, g_MenuWinBBlend4GraphicsObject) != 0) {
        GfxTlVertex *baseLastTopLeft;
        GfxTlVertex *lastBottomLeft;
        GfxTlVertex *nextLastTopRight;
        GfxTlVertex *h;

        state.u = (float)(state.remaining * 0x10 + 0x10) * 0.00390625f;
        state.v = 0.0f;
        state.du = 0.0546875f;
        state.dv = 0.0625f;
        if (g_MenuModuleActive != 0) {
            state.screenX = x + (digitCount - 1) * 12;
        } else {
            state.screenX = x + (digitCount - 1) * 14;
        }
        baseLastTopLeft = (GfxTlVertex *)(g_MenuWinBBlend4GraphicsObject->vertexCursor);
        baseLastTopLeft->x = (float)state.screenX;
        baseLastTopLeft->y = (float)y;
        baseLastTopLeft->z = z;
        baseLastTopLeft->rhwBits = FLOAT_ONE_BITS;
        baseLastTopLeft->color = 0xFFFFFFFF;
        baseLastTopLeft->specular = 0xFF000000;
        baseLastTopLeft->u = state.u;
        baseLastTopLeft->v = state.v;
        lastBottomLeft = (GfxTlVertex *)(g_MenuWinBBlend4GraphicsObject->vertexCursor + 0x20);
        lastBottomLeft->x = (float)state.screenX;
        lastBottomLeft->y = (float)y + 16.0f;
        lastBottomLeft->z = z;
        lastBottomLeft->rhwBits = FLOAT_ONE_BITS;
        lastBottomLeft->color = 0xFFFFFFFF;
        lastBottomLeft->specular = 0xFF000000;
        lastBottomLeft->u = state.u;
        lastBottomLeft->v = state.v + state.dv;
        nextLastTopRight = (GfxTlVertex *)(g_MenuWinBBlend4GraphicsObject->vertexCursor + 0x40);
        nextLastTopRight->x = (float)state.screenX + 14.0f;
        nextLastTopRight->y = (float)y;
        nextLastTopRight->z = z;
        nextLastTopRight->rhwBits = FLOAT_ONE_BITS;
        nextLastTopRight->color = 0xFFFFFFFF;
        nextLastTopRight->specular = 0xFF000000;
        nextLastTopRight->u = state.u + state.du;
        nextLastTopRight->v = state.v;
        h = (GfxTlVertex *)(g_MenuWinBBlend4GraphicsObject->vertexCursor + 0x60);
        h->x = (float)state.screenX + 14.0f;
        h->y = (float)y + 16.0f;
        h->z = z;
        h->rhwBits = FLOAT_ONE_BITS;
        h->color = 0xFFFFFFFF;
        h->specular = 0xFF000000;
        h->u = state.u + state.du;
        h->v = state.v + state.dv;
        *g_MenuWinBBlend4GraphicsObject->colorBytePtr = colorIndex;
        g_MenuWinBBlend4GraphicsObject->paletteIndex = colorIndex;
    }
}

/*
 * When text is ready, draws a zero-padded decimal value using the low byte of
 * digitCount. Builds the unsigned decimal divisor, acquires each digit
 * independently and emits 14-by-16 white atlas quads at 12-pixel pitch when
 * g_MenuModuleActive is nonzero or 14-pixel pitch otherwise.
 */
// FUNCTION: FF7 0x006f9c44
void __cdecl Menu_DrawDecimalNumberZeroPad2x(int x, int y, unsigned int value, unsigned int digitCount, unsigned char colorIndex, float z)
{
    struct {
        float dv;
        float du;
        float v;
        float u;
        short digit;
        int screenX;
        unsigned int remaining;
        unsigned int divisor;
        short unusedLeadingZero;
    } state;

    if (Menu_IsTextSubsystemReady() == 0) {
        return;
    }
    state.divisor = 1;
    state.unusedLeadingZero = (short)state.divisor;
    for (state.digit = 1; state.digit < (int)(digitCount & 0xFF); state.digit++) {
        state.divisor *= 10;
    }
    state.remaining = value;
    for (state.digit = 1; state.digit < (int)(digitCount & 0xFF); state.digit++) {
        if (AcquireBatchNode(1, g_MenuWinBBlend4GraphicsObject) != 0) {
            GfxTlVertex *topRemaining;
            GfxTlVertex *b;
            GfxTlVertex *nextCorner;
            GfxTlVertex *bottomRightAddr;

            state.u = (float)((state.remaining / state.divisor) * 0x10 + 0x10) * 0.00390625f;
            state.v = 0.0f;
            state.du = 0.0546875f;
            state.dv = 0.0625f;
            if (g_MenuModuleActive != 0) {
                state.screenX = x + (state.digit - 1) * 12;
            } else {
                state.screenX = x + (state.digit - 1) * 14;
            }
            topRemaining = (GfxTlVertex *)(g_MenuWinBBlend4GraphicsObject->vertexCursor);
            topRemaining->x = (float)state.screenX;
            topRemaining->y = (float)y;
            topRemaining->z = z;
            topRemaining->rhwBits = FLOAT_ONE_BITS;
            topRemaining->color = 0xFFFFFFFF;
            topRemaining->specular = 0xFF000000;
            topRemaining->u = state.u;
            topRemaining->v = state.v;
            b = (GfxTlVertex *)(g_MenuWinBBlend4GraphicsObject->vertexCursor + 0x20);
            b->x = (float)state.screenX;
            b->y = (float)y + 16.0f;
            b->z = z;
            b->rhwBits = FLOAT_ONE_BITS;
            b->color = 0xFFFFFFFF;
            b->specular = 0xFF000000;
            b->u = state.u;
            b->v = state.v + state.dv;
            nextCorner = (GfxTlVertex *)(g_MenuWinBBlend4GraphicsObject->vertexCursor + 0x40);
            nextCorner->x = (float)state.screenX + 14.0f;
            nextCorner->y = (float)y;
            nextCorner->z = z;
            nextCorner->rhwBits = FLOAT_ONE_BITS;
            nextCorner->color = 0xFFFFFFFF;
            nextCorner->specular = 0xFF000000;
            nextCorner->u = state.u + state.du;
            nextCorner->v = state.v;
            bottomRightAddr = (GfxTlVertex *)(g_MenuWinBBlend4GraphicsObject->vertexCursor + 0x60);
            bottomRightAddr->x = (float)state.screenX + 14.0f;
            bottomRightAddr->y = (float)y + 16.0f;
            bottomRightAddr->z = z;
            bottomRightAddr->rhwBits = FLOAT_ONE_BITS;
            bottomRightAddr->color = 0xFFFFFFFF;
            bottomRightAddr->specular = 0xFF000000;
            bottomRightAddr->u = state.u + state.du;
            bottomRightAddr->v = state.v + state.dv;
            *g_MenuWinBBlend4GraphicsObject->colorBytePtr = colorIndex;
            g_MenuWinBBlend4GraphicsObject->paletteIndex = colorIndex;
        }
        state.remaining %= state.divisor;
        state.divisor /= 10;
    }
    if (AcquireBatchNode(1, g_MenuWinBBlend4GraphicsObject) != 0) {
        GfxTlVertex *baseLastTopLeft;
        GfxTlVertex *lastBottomLeft;
        GfxTlVertex *nextLastTopRight;
        GfxTlVertex *h;

        state.u = (float)(state.remaining * 0x10 + 0x10) * 0.00390625f;
        state.v = 0.0f;
        state.du = 0.0546875f;
        state.dv = 0.0625f;
        if (g_MenuModuleActive != 0) {
            state.screenX = x + ((digitCount & 0xFF) - 1) * 12;
        } else {
            state.screenX = x + ((digitCount & 0xFF) - 1) * 14;
        }
        baseLastTopLeft = (GfxTlVertex *)(g_MenuWinBBlend4GraphicsObject->vertexCursor);
        baseLastTopLeft->x = (float)state.screenX;
        baseLastTopLeft->y = (float)y;
        baseLastTopLeft->z = z;
        baseLastTopLeft->rhwBits = FLOAT_ONE_BITS;
        baseLastTopLeft->color = 0xFFFFFFFF;
        baseLastTopLeft->specular = 0xFF000000;
        baseLastTopLeft->u = state.u;
        baseLastTopLeft->v = state.v;
        lastBottomLeft = (GfxTlVertex *)(g_MenuWinBBlend4GraphicsObject->vertexCursor + 0x20);
        lastBottomLeft->x = (float)state.screenX;
        lastBottomLeft->y = (float)y + 16.0f;
        lastBottomLeft->z = z;
        lastBottomLeft->rhwBits = FLOAT_ONE_BITS;
        lastBottomLeft->color = 0xFFFFFFFF;
        lastBottomLeft->specular = 0xFF000000;
        lastBottomLeft->u = state.u;
        lastBottomLeft->v = state.v + state.dv;
        nextLastTopRight = (GfxTlVertex *)(g_MenuWinBBlend4GraphicsObject->vertexCursor + 0x40);
        nextLastTopRight->x = (float)state.screenX + 14.0f;
        nextLastTopRight->y = (float)y;
        nextLastTopRight->z = z;
        nextLastTopRight->rhwBits = FLOAT_ONE_BITS;
        nextLastTopRight->color = 0xFFFFFFFF;
        nextLastTopRight->specular = 0xFF000000;
        nextLastTopRight->u = state.u + state.du;
        nextLastTopRight->v = state.v;
        h = (GfxTlVertex *)(g_MenuWinBBlend4GraphicsObject->vertexCursor + 0x60);
        h->x = (float)state.screenX + 14.0f;
        h->y = (float)y + 16.0f;
        h->z = z;
        h->rhwBits = FLOAT_ONE_BITS;
        h->color = 0xFFFFFFFF;
        h->specular = 0xFF000000;
        h->u = state.u + state.du;
        h->v = state.v + state.dv;
        *g_MenuWinBBlend4GraphicsObject->colorBytePtr = colorIndex;
        g_MenuWinBBlend4GraphicsObject->paletteIndex = colorIndex;
    }
}

/*
 * Flush every menu batch list in both passes, invalidate them all, and then set
 * the viewport to the rectangle: the 640x480 way to start a clipped region.
 */
// FUNCTION: FF7 0x006fa12f
void __cdecl Menu_FlushAndSetScissor2x(int x, int y, int w, int h)
{
    GfxEngineContext *ctx;

    ctx = GetEngineContext();
    Gfx_SetScenePassCall(0, ctx);
    Gfx_SetRenderStateCall(RENDER_STATE_LINEAR_FILTER, 0, ctx);
    Gfx_EmitPrimitiveBatchList(g_MenuWindowBgGraphicsObject, ctx);
    Gfx_EmitPrimitiveBatchList(g_MenuWinABlend4GraphicsObject, ctx);
    Gfx_EmitPrimitiveBatchList(g_MenuWinBBlend4GraphicsObject, ctx);
    Gfx_EmitPrimitiveBatchList(g_MenuWinCBlend4GraphicsObject, ctx);
    Gfx_EmitPrimitiveBatchList(g_MenuWinCBlend4DiffGraphicsObject, ctx);
    Gfx_EmitPrimitiveBatchList(g_MenuWinDBlend4GraphicsObject, ctx);
    Gfx_EmitPrimitiveBatchList(g_MenuFontAGraphicsObject, ctx);
    Gfx_EmitPrimitiveBatchList(g_MenuFontBGraphicsObject.batchList, ctx);
    Gfx_SetScenePassCall(1, ctx);
    Gfx_SetRenderStateCall(RENDER_STATE_LINEAR_FILTER, 0, ctx);
    Gfx_EmitPrimitiveBatchList(g_MenuWinABlend1GraphicsObject, ctx);
    Gfx_EmitPrimitiveBatchList(g_MenuWinBBlend1GraphicsObject, ctx);
    Gfx_EmitPrimitiveBatchList(g_MenuWinCBlend1GraphicsObject, ctx);
    Gfx_EmitPrimitiveBatchList(g_MenuWinDBlend1GraphicsObject, ctx);
    Gfx_EmitPrimitiveBatchList(g_MenuBlendWindowBgGraphicsObject, ctx);
    InvalidateBatchGeneration(g_MenuBlendWindowBgGraphicsObject);
    InvalidateBatchGeneration(g_MenuFontAGraphicsObject);
    InvalidateBatchGeneration(g_MenuFontBGraphicsObject.batchList);
    InvalidateBatchGeneration(g_MenuWinABlend4GraphicsObject);
    InvalidateBatchGeneration(g_MenuWinBBlend4GraphicsObject);
    InvalidateBatchGeneration(g_MenuWinCBlend4GraphicsObject);
    InvalidateBatchGeneration(g_MenuWinCBlend4DiffGraphicsObject);
    InvalidateBatchGeneration(g_MenuWinDBlend4GraphicsObject);
    InvalidateBatchGeneration(g_MenuWinABlend1GraphicsObject);
    InvalidateBatchGeneration(g_MenuWinBBlend1GraphicsObject);
    InvalidateBatchGeneration(g_MenuWinCBlend1GraphicsObject);
    InvalidateBatchGeneration(g_MenuWinDBlend1GraphicsObject);
    InvalidateBatchGeneration(g_MenuWindowBgGraphicsObject);
    GfxSetViewportRect(x, y, w, h, (GfxEngineContext *)ctx);
}

/*
 * The closing half of Menu_FlushAndSetScissor2x: the same flush, then the viewport
 * back to the full 640x480.
 */
// FUNCTION: FF7 0x006fa347
void __cdecl Menu_FlushAndResetScissor2x(void)
{
    GfxEngineContext *ctx;

    ctx = GetEngineContext();
    Gfx_SetScenePassCall(0, ctx);
    Gfx_SetRenderStateCall(RENDER_STATE_LINEAR_FILTER, 0, ctx);
    Gfx_EmitPrimitiveBatchList(g_MenuWindowBgGraphicsObject, ctx);
    Gfx_EmitPrimitiveBatchList(g_MenuWinABlend4GraphicsObject, ctx);
    Gfx_EmitPrimitiveBatchList(g_MenuWinBBlend4GraphicsObject, ctx);
    Gfx_EmitPrimitiveBatchList(g_MenuWinCBlend4GraphicsObject, ctx);
    Gfx_EmitPrimitiveBatchList(g_MenuWinCBlend4DiffGraphicsObject, ctx);
    Gfx_EmitPrimitiveBatchList(g_MenuWinDBlend4GraphicsObject, ctx);
    Gfx_EmitPrimitiveBatchList(g_MenuFontAGraphicsObject, ctx);
    Gfx_EmitPrimitiveBatchList(g_MenuFontBGraphicsObject.batchList, ctx);
    Gfx_SetScenePassCall(1, ctx);
    Gfx_SetRenderStateCall(RENDER_STATE_LINEAR_FILTER, 0, ctx);
    Gfx_EmitPrimitiveBatchList(g_MenuBlendWindowBgGraphicsObject, ctx);
    InvalidateBatchGeneration(g_MenuBlendWindowBgGraphicsObject);
    Gfx_EmitPrimitiveBatchList(g_MenuWinABlend1GraphicsObject, ctx);
    Gfx_EmitPrimitiveBatchList(g_MenuWinBBlend1GraphicsObject, ctx);
    Gfx_EmitPrimitiveBatchList(g_MenuWinCBlend1GraphicsObject, ctx);
    Gfx_EmitPrimitiveBatchList(g_MenuWinDBlend1GraphicsObject, ctx);
    InvalidateBatchGeneration(g_MenuFontAGraphicsObject);
    InvalidateBatchGeneration(g_MenuFontBGraphicsObject.batchList);
    InvalidateBatchGeneration(g_MenuWindowBgGraphicsObject);
    InvalidateBatchGeneration(g_MenuWinABlend4GraphicsObject);
    InvalidateBatchGeneration(g_MenuWinBBlend4GraphicsObject);
    InvalidateBatchGeneration(g_MenuWinCBlend4GraphicsObject);
    InvalidateBatchGeneration(g_MenuWinCBlend4DiffGraphicsObject);
    InvalidateBatchGeneration(g_MenuWinDBlend4GraphicsObject);
    InvalidateBatchGeneration(g_MenuWinABlend1GraphicsObject);
    InvalidateBatchGeneration(g_MenuWinBBlend1GraphicsObject);
    InvalidateBatchGeneration(g_MenuWinCBlend1GraphicsObject);
    InvalidateBatchGeneration(g_MenuWinDBlend1GraphicsObject);
    GfxSetViewportRect(0, 0, 0x280, 0x1E0, (GfxEngineContext *)ctx);
}

/*
 * Measures at most g_MenuMaxGlyphsPerString encoded glyphs, stopping for a null
 * pointer or 0xFF at the current glyph. Prefixes 0xFA through 0xFE consume the
 * next byte and select width-table offsets 0xE7, 0x1B9, 0x2A0, 0x372 or 0x444.
 */
// FUNCTION: FF7 0x006fa55d
int __cdecl Menu_MeasureEncodedString1x(unsigned char *text)
{
    int prefixOfs;
    int glyphIndex;
    int sumWidth;

    glyphIndex = 0;
    sumWidth = glyphIndex;
    for (; glyphIndex < g_MenuMaxGlyphsPerString; glyphIndex++) {
        if (text == 0) {
            break;
        }
        if (*text == TEXT_END) {
            break;
        }
        switch (*text) {
        case TEXT_FONT_PAGE_1:
            text++;
            prefixOfs = 0xe7;
            break;
        case TEXT_FONT_PAGE_2:
            text++;
            prefixOfs = 0x1b9;
            break;
        case TEXT_FONT_PAGE_3:
            text++;
            prefixOfs = 0x2a0;
            break;
        case TEXT_FONT_PAGE_4:
            text++;
            prefixOfs = 0x372;
            break;
        case TEXT_EXTENDED:
            text++;
            prefixOfs = 0x444;
            break;
        default:
            prefixOfs = 0;
            break;
        }
        if (g_MenuModuleActive != 0) {
            sumWidth += (int)((g_pTextSpacing[*text + prefixOfs] >> 5) * 0.8333333f);
            sumWidth += (int)((g_pTextSpacing[*text + prefixOfs] & 0x1f) * 0.8333333f);
        } else {
            sumWidth += g_pTextSpacing[*text + prefixOfs] >> 5;
            sumWidth += g_pTextSpacing[*text + prefixOfs] & 0x1f;
        }
        text++;
    }
    return sumWidth;
}

/*
 * Draws one 12-by-12 font glyph when text is ready and batch acquisition succeeds,
 * and returns the advanced pen X even when emission fails. Prefix 0xF8 returns
 * unchanged; prefixes 0xFA through 0xFE select the high byte as glyph code and
 * alter texture row/width-table offsets.
 */
// FUNCTION: FF7 0x006fa706
int __cdecl Menu_DrawFontGlyph1x(int penX, int posY, unsigned int colorIndex,
                            unsigned short charCode, float z)
{
    unsigned char *p;
    short q;
    short r;
    short s;
    int t;
    float u;
    short v;
    float w;
    float x;
    float y;
    short j;
    struct GfxTlVertex *k;
    struct GfxTlVertex *l;
    struct GfxTlVertex *m;
    struct GfxTlVertex *n;

    t = penX + g_MenuViewportX;
    posY += g_MenuViewportY;
    j = 0;
    v = j;
    p = (unsigned char *)&charCode;
    switch (*p) {
    case TEXT_BATTLE_EMPHASIS:
        return penX;
    case TEXT_FONT_PAGE_1:
        p++;
        r = 0x84;
        s = 0xE7;
        break;
    case TEXT_FONT_PAGE_2:
        p++;
        r = 0;
        j = 0x10;
        s = 0x1B9;
        break;
    case TEXT_FONT_PAGE_3:
        p++;
        r = 0x84;
        j = 0x10;
        s = 0x2A0;
        break;
    case TEXT_FONT_PAGE_4:
        p++;
        r = 0x84;
        s = 0x372;
        v = -0x40;
        break;
    case TEXT_EXTENDED:
        p++;
        r = 0x84;
        j = 0x10;
        s = 0x444;
        v = -0x40;
        break;
    default:
        s = 0;
        r = 0;
        break;
    }
    charCode = *p;
    q = charCode % 0x15 * 0xC;
    r += charCode / 0x15 * 0xC;
    if (g_MenuModuleActive != 0) {
        penX += (int)((g_pTextSpacing[charCode + s] >> 5) * 0.8333333f);
    } else {
        penX += g_pTextSpacing[charCode + s] >> 5;
    }
    if (Menu_IsTextSubsystemReady() != 0) {
        if (AcquireBatchNode(1, g_MenuFontBlend4GraphicsObject) != 0) {
            u = q * 0.00390625f;
            w = r * 0.00390625f;
            y = 0.046875f;
            x = y;
            k = (struct GfxTlVertex *)g_MenuFontBlend4GraphicsObject->vertexCursor;
            k->x = t;
            k->y = posY;
            k->z = z;
            k->rhwBits = FLOAT_ONE_BITS;
            k->color = 0xFFFFFFFF;
            k->specular = 0xFF000000;
            k->u = u;
            k->v = w;
            l = (struct GfxTlVertex *)(g_MenuFontBlend4GraphicsObject->vertexCursor + 0x20);
            l->x = t;
            l->y = posY + 12.0f;
            l->z = z;
            l->rhwBits = FLOAT_ONE_BITS;
            l->color = 0xFFFFFFFF;
            l->specular = 0xFF000000;
            l->u = u;
            l->v = w + y;
            m = (struct GfxTlVertex *)(g_MenuFontBlend4GraphicsObject->vertexCursor + 0x40);
            m->x = t + 12.0f;
            m->y = posY;
            m->z = z;
            m->rhwBits = FLOAT_ONE_BITS;
            m->color = 0xFFFFFFFF;
            m->specular = 0xFF000000;
            m->u = u + x;
            m->v = w;
            n = (struct GfxTlVertex *)(g_MenuFontBlend4GraphicsObject->vertexCursor + 0x60);
            n->x = t + 12.0f;
            n->y = posY + 12.0f;
            n->z = z;
            n->rhwBits = FLOAT_ONE_BITS;
            n->color = 0xFFFFFFFF;
            n->specular = 0xFF000000;
            n->u = u + x;
            n->v = w + y;
            *g_MenuFontBlend4GraphicsObject->colorBytePtr = (unsigned char)colorIndex << 1;
            g_MenuFontBlend4GraphicsObject->paletteIndex = colorIndex << 1;
        }
    }
    if (g_MenuModuleActive != 0) {
        penX += (int)((g_pTextSpacing[charCode + s] & 0x1F) * 0.8333333f);
    } else {
        penX += g_pTextSpacing[charCode + s] & 0x1F;
    }
    return penX;
}

/*
 * When the text subsystem is ready and text is nonnull, draws at most
 * g_MenuMaxGlyphsPerString encoded glyphs, stopping at 0xFF. Bytes 0xF8 and
 * 0xFA..0xFE consume the following byte as the high byte of a glyph code; others
 * consume one byte.
 */
// FUNCTION: FF7 0x006fab2f
void __cdecl Menu_DrawEncodedString1x(int startX, int posY,
                                  unsigned char *text,
                                  unsigned char colorIndex, float z)
{
    short i;

    if (Menu_IsTextSubsystemReady() == 0) {
        return;
    }
    if (text == 0) {
        return;
    }
    for (i = 0; i < g_MenuMaxGlyphsPerString; i++) {
        if (*text == TEXT_END) {
            return;
        }
        if ((*text >= TEXT_FONT_PAGE_1 && *text <= TEXT_EXTENDED) || *text == TEXT_BATTLE_EMPHASIS) {
            startX = Menu_DrawFontGlyph1x(startX, posY, colorIndex,
                                     *text | (text[1] << 8), z);
            text += 2;
        } else {
            startX = Menu_DrawFontGlyph1x(startX, posY, colorIndex, *text++,
                                     z);
        }
    }
}

/*
 * When text is ready, adds the text origin and writes white 8-by-8 icon quads at
 * the supplied depth, storing colorIndex as palette selector. Codes zero through
 * 0x28 independently draw a fixed upper tile at Y minus eight and remap the lower
 * tile by adding 64; codes 0x29 through 0x32 use a different upper tile and add
 * 23.
 */
// FUNCTION: FF7 0x006fac38
void __cdecl DrawIconGlyph16_1x(int x, int y, unsigned char code,
                                unsigned char colorIndex, float z)
{
    struct {
        struct GfxTlVertex *l, *k, *j, *i, *h, *g, *f, *e, *d, *c, *b, *a;
        float dv, du, v, u;
        short ty, padty, tx, padtx;
    } s; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    if (Menu_IsTextSubsystemReady() == 0)
        return;
    x += g_MenuViewportX;
    y += g_MenuViewportY;
    if (code <= 0x28) {
        if (AcquireBatchNode(1, g_MenuWinBlend4GraphicsObject) != 0) {
            s.u = 0.53125f;
            s.v = 0.59375f;
            s.du = 0.03125f;
            s.dv = 0.03125f;
            s.a = (struct GfxTlVertex *)(g_MenuWinBlend4GraphicsObject->vertexCursor);
            s.a->x = (float)x;
            s.a->y = (float)y - 8.0f;
            s.a->z = z;
            s.a->rhwBits = FLOAT_ONE_BITS;
            s.a->color = 0xffffffff;
            s.a->specular = 0xff000000;
            s.a->u = s.u;
            s.a->v = s.v;
            s.b = (struct GfxTlVertex *)(g_MenuWinBlend4GraphicsObject->vertexCursor + 32);
            s.b->x = (float)x;
            s.b->y = (float)y - 8.0f + 8.0f;
            s.b->z = z;
            s.b->rhwBits = FLOAT_ONE_BITS;
            s.b->color = 0xffffffff;
            s.b->specular = 0xff000000;
            s.b->u = s.u;
            s.b->v = s.v + s.dv;
            s.c = (struct GfxTlVertex *)(g_MenuWinBlend4GraphicsObject->vertexCursor + 64);
            s.c->x = (float)x + 8.0f;
            s.c->y = (float)y - 8.0f;
            s.c->z = z;
            s.c->rhwBits = FLOAT_ONE_BITS;
            s.c->color = 0xffffffff;
            s.c->specular = 0xff000000;
            s.c->u = s.u + s.du;
            s.c->v = s.v;
            s.d = (struct GfxTlVertex *)(g_MenuWinBlend4GraphicsObject->vertexCursor + 96);
            s.d->x = (float)x + 8.0f;
            s.d->y = (float)y - 8.0f + 8.0f;
            s.d->z = z;
            s.d->rhwBits = FLOAT_ONE_BITS;
            s.d->color = 0xffffffff;
            s.d->specular = 0xff000000;
            s.d->u = s.u + s.du;
            s.d->v = s.v + s.dv;
            *g_MenuWinBlend4GraphicsObject->colorBytePtr = colorIndex;
            g_MenuWinBlend4GraphicsObject->texturePage = colorIndex;
        }
        s.tx = ((code + 64) & 15) * 8 + 128;
        s.ty = ((code + 64) >> 4) * 8 + 128;
    } else if (code >= 0xb4 && code <= 0xcd) {
        s.tx = (code & 15) * 8 + 128;
        s.ty = (code >> 4) * 8 + 128;
    } else if (code >= 0x29 && code <= 0x32) {
        if (AcquireBatchNode(1, g_MenuWinBlend4GraphicsObject) != 0) {
            s.u = 0.5625f;
            s.v = 0.59375f;
            s.du = 0.03125f;
            s.dv = 0.03125f;
            s.e = (struct GfxTlVertex *)(g_MenuWinBlend4GraphicsObject->vertexCursor);
            s.e->x = (float)x;
            s.e->y = (float)y - 8.0f;
            s.e->z = z;
            s.e->rhwBits = FLOAT_ONE_BITS;
            s.e->color = 0xffffffff;
            s.e->specular = 0xff000000;
            s.e->u = s.u;
            s.e->v = s.v;
            s.f = (struct GfxTlVertex *)(g_MenuWinBlend4GraphicsObject->vertexCursor + 32);
            s.f->x = (float)x;
            s.f->y = (float)y - 8.0f + 8.0f;
            s.f->z = z;
            s.f->rhwBits = FLOAT_ONE_BITS;
            s.f->color = 0xffffffff;
            s.f->specular = 0xff000000;
            s.f->u = s.u;
            s.f->v = s.v + s.dv;
            s.g = (struct GfxTlVertex *)(g_MenuWinBlend4GraphicsObject->vertexCursor + 64);
            s.g->x = (float)x + 8.0f;
            s.g->y = (float)y - 8.0f;
            s.g->z = z;
            s.g->rhwBits = FLOAT_ONE_BITS;
            s.g->color = 0xffffffff;
            s.g->specular = 0xff000000;
            s.g->u = s.u + s.du;
            s.g->v = s.v;
            s.h = (struct GfxTlVertex *)(g_MenuWinBlend4GraphicsObject->vertexCursor + 96);
            s.h->x = (float)x + 8.0f;
            s.h->y = (float)y - 8.0f + 8.0f;
            s.h->z = z;
            s.h->rhwBits = FLOAT_ONE_BITS;
            s.h->color = 0xffffffff;
            s.h->specular = 0xff000000;
            s.h->u = s.u + s.du;
            s.h->v = s.v + s.dv;
            *g_MenuWinBlend4GraphicsObject->colorBytePtr = colorIndex;
            g_MenuWinBlend4GraphicsObject->texturePage = colorIndex;
        }
        s.tx = ((code + 23) & 15) * 8 + 128;
        s.ty = ((code + 23) >> 4) * 8 + 128;
    } else {
        s.tx = (code & 15) * 8 + 128;
        s.ty = (code >> 4) * 8 + 128;
    }
    if (AcquireBatchNode(1, g_MenuWinBlend4GraphicsObject) != 0) {
        s.u = s.tx * 0.00390625f;
        s.v = s.ty * 0.00390625f;
        s.du = 0.03125f;
        s.dv = 0.03125f;
        s.i = (struct GfxTlVertex *)(g_MenuWinBlend4GraphicsObject->vertexCursor);
        s.i->x = (float)x;
        s.i->y = (float)y;
        s.i->z = z;
        s.i->rhwBits = FLOAT_ONE_BITS;
        s.i->color = 0xffffffff;
        s.i->specular = 0xff000000;
        s.i->u = s.u;
        s.i->v = s.v;
        s.j = (struct GfxTlVertex *)(g_MenuWinBlend4GraphicsObject->vertexCursor + 32);
        s.j->x = (float)x;
        s.j->y = (float)y + 8.0f;
        s.j->z = z;
        s.j->rhwBits = FLOAT_ONE_BITS;
        s.j->color = 0xffffffff;
        s.j->specular = 0xff000000;
        s.j->u = s.u;
        s.j->v = s.v + s.dv;
        s.k = (struct GfxTlVertex *)(g_MenuWinBlend4GraphicsObject->vertexCursor + 64);
        s.k->x = (float)x + 8.0f;
        s.k->y = (float)y;
        s.k->z = z;
        s.k->rhwBits = FLOAT_ONE_BITS;
        s.k->color = 0xffffffff;
        s.k->specular = 0xff000000;
        s.k->u = s.u + s.du;
        s.k->v = s.v;
        s.l = (struct GfxTlVertex *)(g_MenuWinBlend4GraphicsObject->vertexCursor + 96);
        s.l->x = (float)x + 8.0f;
        s.l->y = (float)y + 8.0f;
        s.l->z = z;
        s.l->rhwBits = FLOAT_ONE_BITS;
        s.l->color = 0xffffffff;
        s.l->specular = 0xff000000;
        s.l->u = s.u + s.du;
        s.l->v = s.v + s.dv;
        *g_MenuWinBlend4GraphicsObject->colorBytePtr = colorIndex;
        g_MenuWinBlend4GraphicsObject->texturePage = colorIndex;
    }
}

/*
 * The 1x half of Menu_DrawFixedPitchString2x: seven- or eight-pixel cells rather than
 * thirteen or sixteen, and one extra gate on the text subsystem before anything
 * else.
 */
// FUNCTION: FF7 0x006fb35c
void __cdecl Menu_DrawFixedPitchString1x(int x, int y, unsigned char *text,
                                    unsigned char colorIndex, float z)
{
    short i;

    if (Menu_IsTextSubsystemReady() == 0) {
        return;
    }
    if (text == 0) {
        return;
    }
    for (i = 0; i < g_MenuMaxGlyphsPerString; i++) {
        if (*text == TEXT_END) {
            return;
        }
        DrawIconGlyph16_1x(x, y, *text++, colorIndex, z);
        if (g_MenuModuleActive != 0) {
            x += 7;
        } else {
            x += 8;
        }
    }
}

/*
 * When text is ready and the signed gauge->max is nonzero, draws a
 * low-resolution resource gauge from the packed descriptor. Computes primary and
 * secondary widths as signed width times the corresponding signed value divided by
 * maximum, truncating to shorts without clamping.
 */
// FUNCTION: FF7 0x006fb3ff
void __cdecl Menu_DrawResourceGauge1x(MenuGauge *gauge, float depth)
{
    short secondaryWidth;
    short val;
    short gaugeRemaining;
    struct GfxTlVertex *vertC;
    struct GfxTlVertex *vertexBPtr;
    struct GfxTlVertex *vertD;
    struct GfxTlVertex *vertF;
    struct GfxTlVertex *vertH;
    struct GfxTlVertex *vertJ;
    struct GfxTlVertex *vertK;
    struct GfxTlVertex *vertL;
    short fillWidthStep;
    short barHeight;
    short screenY;
    struct GfxTlVertex *vtxAPtr;
    int color;
    struct GfxTlVertex *vertE;
    struct GfxTlVertex *vertG;
    struct GfxTlVertex *vertI;

    if (Menu_IsTextSubsystemReady() == 0) {
        return;
    }
    if (gauge->max == 0) {
        return;
    }
    gaugeRemaining = gauge->x + g_MenuViewportX;
    screenY = gauge->y + g_MenuViewportY;
    val = gauge->width;
    barHeight = gauge->height;
    fillWidthStep = gauge->width * gauge->low / gauge->max;
    secondaryWidth = gauge->width * gauge->high / gauge->max;
    if (AcquireBatchNode((gauge->direction != MENU_GAUGE_DIR_STEADY) + 2, g_MenuWindowBgGraphicsObject) != 0) {
        vtxAPtr = (struct GfxTlVertex *)(g_MenuWindowBgGraphicsObject->vertexCursor);
        vtxAPtr->x = (float)gaugeRemaining;
        vtxAPtr->y = (float)screenY;
        vtxAPtr->z = depth;
        vtxAPtr->rhwBits = FLOAT_ONE_BITS;
        vtxAPtr->color = 0x80500000;
        vtxAPtr->specular = 0xFF000000;
        vertexBPtr = (struct GfxTlVertex *)(g_MenuWindowBgGraphicsObject->vertexCursor + 0x20);
        vertexBPtr->x = (float)gaugeRemaining;
        vertexBPtr->y = (float)screenY + barHeight + 1.0f;
        vertexBPtr->z = depth;
        vertexBPtr->rhwBits = FLOAT_ONE_BITS;
        vertexBPtr->color = 0x80000000;
        vertexBPtr->specular = 0xFF000000;
        vertC = (struct GfxTlVertex *)(g_MenuWindowBgGraphicsObject->vertexCursor + 0x40);
        vertC->x = (float)gaugeRemaining + val;
        vertC->y = (float)screenY;
        vertC->z = depth;
        vertC->rhwBits = FLOAT_ONE_BITS;
        vertC->color = 0x80500000;
        vertC->specular = 0xFF000000;
        vertD = (struct GfxTlVertex *)(g_MenuWindowBgGraphicsObject->vertexCursor + 0x60);
        vertD->x = (float)gaugeRemaining + val;
        vertD->y = (float)screenY + barHeight + 1.0f;
        vertD->z = depth;
        vertD->rhwBits = FLOAT_ONE_BITS;
        vertD->color = 0x80000000;
        vertD->specular = 0xFF000000;
        g_MenuWindowBgGraphicsObject->vertexCursor += g_MenuWindowBgGraphicsObject->cursorStep;
        if (gauge->direction != MENU_GAUGE_DIR_STEADY) {
            if (gauge->direction == MENU_GAUGE_DIR_RISING) {
                color = 0x8000C850;
            } else {
                color = 0x80C80000;
            }
            vertE = (struct GfxTlVertex *)(g_MenuWindowBgGraphicsObject->vertexCursor);
            vertE->x = (float)gaugeRemaining;
            vertE->y = (float)screenY;
            vertE->z = depth;
            vertE->rhwBits = FLOAT_ONE_BITS;
            vertE->color = color;
            vertE->specular = 0xFF000000;
            vertF = (struct GfxTlVertex *)(g_MenuWindowBgGraphicsObject->vertexCursor + 0x20);
            vertF->x = (float)gaugeRemaining;
            vertF->y = (float)screenY + barHeight;
            vertF->z = depth;
            vertF->rhwBits = FLOAT_ONE_BITS;
            vertF->color = color;
            vertF->specular = 0xFF000000;
            vertG = (struct GfxTlVertex *)(g_MenuWindowBgGraphicsObject->vertexCursor + 0x40);
            vertG->x = (float)gaugeRemaining + secondaryWidth;
            vertG->y = (float)screenY;
            vertG->z = depth;
            vertG->rhwBits = FLOAT_ONE_BITS;
            vertG->color = color;
            vertG->specular = 0xFF000000;
            vertH = (struct GfxTlVertex *)(g_MenuWindowBgGraphicsObject->vertexCursor + 0x60);
            vertH->x = (float)gaugeRemaining + secondaryWidth;
            vertH->y = (float)screenY + barHeight;
            vertH->z = depth;
            vertH->rhwBits = FLOAT_ONE_BITS;
            vertH->color = color;
            vertH->specular = 0xFF000000;
            g_MenuWindowBgGraphicsObject->vertexCursor += g_MenuWindowBgGraphicsObject->cursorStep;
        }
        color = gauge->colour;
        vertI = (struct GfxTlVertex *)(g_MenuWindowBgGraphicsObject->vertexCursor);
        vertI->x = (float)gaugeRemaining;
        vertI->y = (float)screenY;
        vertI->z = depth;
        vertI->rhwBits = FLOAT_ONE_BITS;
        vertI->color = color;
        vertI->specular = 0xFF000000;
        vertJ = (struct GfxTlVertex *)(g_MenuWindowBgGraphicsObject->vertexCursor + 0x20);
        vertJ->x = (float)gaugeRemaining;
        vertJ->y = (float)screenY + barHeight;
        vertJ->z = depth;
        vertJ->rhwBits = FLOAT_ONE_BITS;
        vertJ->color = color;
        vertJ->specular = 0xFF000000;
        vertK = (struct GfxTlVertex *)(g_MenuWindowBgGraphicsObject->vertexCursor + 0x40);
        vertK->x = (float)gaugeRemaining + fillWidthStep;
        vertK->y = (float)screenY;
        vertK->z = depth;
        vertK->rhwBits = FLOAT_ONE_BITS;
        vertK->color = 0x80C8C8C8;
        vertK->specular = 0xFF000000;
        vertL = (struct GfxTlVertex *)(g_MenuWindowBgGraphicsObject->vertexCursor + 0x60);
        vertL->x = (float)gaugeRemaining + fillWidthStep;
        vertL->y = (float)screenY + barHeight;
        vertL->z = depth;
        vertL->rhwBits = FLOAT_ONE_BITS;
        vertL->color = 0x80C8C8C8;
        vertL->specular = 0xFF000000;
    }
}

/*
 * When the text subsystem is ready, adds the 1x origin to x and y and emits a
 * filled rectangle enlarged by one pixel on every side at depth 0.2. The color ORs
 * level and its eight- and sixteen-bit shifts with 0xa0000000; level is not masked
 * to a byte.
 */
// FUNCTION: FF7 0x006fba29
void __cdecl Menu_DrawShopPartyTileHighlight1x(int x, int y, int width, int height,
                                               unsigned int level)
{
    unsigned int colour;

    if (Menu_IsTextSubsystemReady() == 0) {
        return;
    }
    x += g_MenuViewportX;
    y += g_MenuViewportY;
    colour = (level << 0x10) | 0xa0000000 | (level << 8) | level;
    Gfx_EmitFlatRect(x - 1, y - 1, width + 2, height + 2, colour, 0, 0.2f,
                     (int)g_MenuFlatRectGraphicsObject);
}

/*
 * When text is ready and five untextured quads are acquired, adds the text origin
 * to the signed- short rectangle at rect and draws an etched box at depth plus
 * 0.001 depth. Emits one-pixel top/bottom/left/right strips colored 0x10101010,
 * 0x70707070, 0x20202020 and 0x70707070, then the full rectangle colored
 * 0x50505050.
 */
// FUNCTION: FF7 0x006fbaa4
void __cdecl Menu_DrawEtchedBox1x(EnglishWindowFrame *rect, float depth)
{
    int rowOfs;
    GfxTlVertex *pVertexI;
    GfxTlVertex *vertexHeight;
    GfxTlVertex *vertexG;
    GfxTlVertex *quadFPtr;
    GfxTlVertex *vertEPtr;
    GfxTlVertex *vertexJ;
    GfxTlVertex *pQuadM;
    GfxTlVertex *vertexN;
    GfxTlVertex *vertexO;
    GfxTlVertex *pQuadQ;
    GfxTlVertex *pQuadD;
    GfxTlVertex *pVertC;
    GfxTlVertex *quadB;
    GfxTlVertex *vertA;
    int boxTopY;
    int positionX;
    GfxTlVertex *vertexK;
    GfxTlVertex *pVertL;
    GfxTlVertex *pVertexP;
    GfxTlVertex *vertTPtr;
    GfxTlVertex *quadSPtr;
    GfxTlVertex *vertexR;
    int width;

    if (Menu_IsTextSubsystemReady() == 0) {
        return;
    }
    depth += 0.001f;
    if (AcquireBatchNode(5, g_MenuBandGraphicsObject) != 0) {
        positionX = rect->x + g_MenuViewportX;
        boxTopY = rect->y + g_MenuViewportY;
        width = rect->width - 1;
        rowOfs = 1;
        vertA = (GfxTlVertex *)(g_MenuBandGraphicsObject->vertexCursor);
        vertA->x = (float)positionX;
        vertA->y = (float)boxTopY;
        vertA->z = depth;
        vertA->rhwBits = FLOAT_ONE_BITS;
        vertA->color = 0x10101010;
        vertA->specular = 0xFF000000;
        quadB = (GfxTlVertex *)(g_MenuBandGraphicsObject->vertexCursor + 0x20);
        quadB->x = (float)positionX;
        quadB->y = (float)boxTopY + rowOfs;
        quadB->z = depth;
        quadB->rhwBits = FLOAT_ONE_BITS;
        quadB->color = 0x10101010;
        quadB->specular = 0xFF000000;
        pVertC = (GfxTlVertex *)(g_MenuBandGraphicsObject->vertexCursor + 0x40);
        pVertC->x = (float)positionX + width;
        pVertC->y = (float)boxTopY;
        pVertC->z = depth;
        pVertC->rhwBits = FLOAT_ONE_BITS;
        pVertC->color = 0x10101010;
        pVertC->specular = 0xFF000000;
        pQuadD = (GfxTlVertex *)(g_MenuBandGraphicsObject->vertexCursor + 0x60);
        pQuadD->x = (float)positionX + width;
        pQuadD->y = (float)boxTopY + rowOfs;
        pQuadD->z = depth;
        pQuadD->rhwBits = FLOAT_ONE_BITS;
        pQuadD->color = 0x10101010;
        pQuadD->specular = 0xFF000000;
        g_MenuBandGraphicsObject->vertexCursor += g_MenuBandGraphicsObject->cursorStep;
        positionX = rect->x + g_MenuViewportX;
        boxTopY = rect->y + (g_MenuViewportY + rect->height) + -1;
        width = rect->width - 1;
        rowOfs = 1;
        vertEPtr = (GfxTlVertex *)(g_MenuBandGraphicsObject->vertexCursor);
        vertEPtr->x = (float)positionX;
        vertEPtr->y = (float)boxTopY;
        vertEPtr->z = depth;
        vertEPtr->rhwBits = FLOAT_ONE_BITS;
        vertEPtr->color = 0x70707070;
        vertEPtr->specular = 0xFF000000;
        quadFPtr = (GfxTlVertex *)(g_MenuBandGraphicsObject->vertexCursor + 0x20);
        quadFPtr->x = (float)positionX;
        quadFPtr->y = (float)boxTopY + rowOfs;
        quadFPtr->z = depth;
        quadFPtr->rhwBits = FLOAT_ONE_BITS;
        quadFPtr->color = 0x70707070;
        quadFPtr->specular = 0xFF000000;
        vertexG = (GfxTlVertex *)(g_MenuBandGraphicsObject->vertexCursor + 0x40);
        vertexG->x = (float)positionX + width;
        vertexG->y = (float)boxTopY;
        vertexG->z = depth;
        vertexG->rhwBits = FLOAT_ONE_BITS;
        vertexG->color = 0x70707070;
        vertexG->specular = 0xFF000000;
        vertexHeight = (GfxTlVertex *)(g_MenuBandGraphicsObject->vertexCursor + 0x60);
        vertexHeight->x = (float)positionX + width;
        vertexHeight->y = (float)boxTopY + rowOfs;
        vertexHeight->z = depth;
        vertexHeight->rhwBits = FLOAT_ONE_BITS;
        vertexHeight->color = 0x70707070;
        vertexHeight->specular = 0xFF000000;
        g_MenuBandGraphicsObject->vertexCursor += g_MenuBandGraphicsObject->cursorStep;
        positionX = rect->x + g_MenuViewportX;
        boxTopY = rect->y + g_MenuViewportY;
        width = 1;
        rowOfs = rect->height - 1;
        pVertexI = (GfxTlVertex *)(g_MenuBandGraphicsObject->vertexCursor);
        pVertexI->x = (float)positionX;
        pVertexI->y = (float)boxTopY;
        pVertexI->z = depth;
        pVertexI->rhwBits = FLOAT_ONE_BITS;
        pVertexI->color = 0x20202020;
        pVertexI->specular = 0xFF000000;
        vertexJ = (GfxTlVertex *)(g_MenuBandGraphicsObject->vertexCursor + 0x20);
        vertexJ->x = (float)positionX;
        vertexJ->y = (float)boxTopY + rowOfs;
        vertexJ->z = depth;
        vertexJ->rhwBits = FLOAT_ONE_BITS;
        vertexJ->color = 0x20202020;
        vertexJ->specular = 0xFF000000;
        vertexK = (GfxTlVertex *)(g_MenuBandGraphicsObject->vertexCursor + 0x40);
        vertexK->x = (float)positionX + width;
        vertexK->y = (float)boxTopY;
        vertexK->z = depth;
        vertexK->rhwBits = FLOAT_ONE_BITS;
        vertexK->color = 0x20202020;
        vertexK->specular = 0xFF000000;
        pVertL = (GfxTlVertex *)(g_MenuBandGraphicsObject->vertexCursor + 0x60);
        pVertL->x = (float)positionX + width;
        pVertL->y = (float)boxTopY + rowOfs;
        pVertL->z = depth;
        pVertL->rhwBits = FLOAT_ONE_BITS;
        pVertL->color = 0x20202020;
        pVertL->specular = 0xFF000000;
        g_MenuBandGraphicsObject->vertexCursor += g_MenuBandGraphicsObject->cursorStep;
        positionX = rect->x + (g_MenuViewportX + rect->width) + -1;
        boxTopY = rect->y + g_MenuViewportY;
        width = 1;
        rowOfs = rect->height;
        pQuadM = (GfxTlVertex *)(g_MenuBandGraphicsObject->vertexCursor);
        pQuadM->x = (float)positionX;
        pQuadM->y = (float)boxTopY;
        pQuadM->z = depth;
        pQuadM->rhwBits = FLOAT_ONE_BITS;
        pQuadM->color = 0x70707070;
        pQuadM->specular = 0xFF000000;
        vertexN = (GfxTlVertex *)(g_MenuBandGraphicsObject->vertexCursor + 0x20);
        vertexN->x = (float)positionX;
        vertexN->y = (float)boxTopY + rowOfs;
        vertexN->z = depth;
        vertexN->rhwBits = FLOAT_ONE_BITS;
        vertexN->color = 0x70707070;
        vertexN->specular = 0xFF000000;
        vertexO = (GfxTlVertex *)(g_MenuBandGraphicsObject->vertexCursor + 0x40);
        vertexO->x = (float)positionX + width;
        vertexO->y = (float)boxTopY;
        vertexO->z = depth;
        vertexO->rhwBits = FLOAT_ONE_BITS;
        vertexO->color = 0x70707070;
        vertexO->specular = 0xFF000000;
        pVertexP = (GfxTlVertex *)(g_MenuBandGraphicsObject->vertexCursor + 0x60);
        pVertexP->x = (float)positionX + width;
        pVertexP->y = (float)boxTopY + rowOfs;
        pVertexP->z = depth;
        pVertexP->rhwBits = FLOAT_ONE_BITS;
        pVertexP->color = 0x70707070;
        pVertexP->specular = 0xFF000000;
        g_MenuBandGraphicsObject->vertexCursor += g_MenuBandGraphicsObject->cursorStep;
        positionX = rect->x + g_MenuViewportX;
        boxTopY = rect->y + g_MenuViewportY;
        width = rect->width;
        rowOfs = rect->height;
        pQuadQ = (GfxTlVertex *)(g_MenuBandGraphicsObject->vertexCursor);
        pQuadQ->x = (float)positionX;
        pQuadQ->y = (float)boxTopY;
        pQuadQ->z = depth;
        pQuadQ->rhwBits = FLOAT_ONE_BITS;
        pQuadQ->color = 0x50505050;
        pQuadQ->specular = 0xFF000000;
        vertexR = (GfxTlVertex *)(g_MenuBandGraphicsObject->vertexCursor + 0x20);
        vertexR->x = (float)positionX;
        vertexR->y = (float)boxTopY + rowOfs;
        vertexR->z = depth;
        vertexR->rhwBits = FLOAT_ONE_BITS;
        vertexR->color = 0x50505050;
        vertexR->specular = 0xFF000000;
        quadSPtr = (GfxTlVertex *)(g_MenuBandGraphicsObject->vertexCursor + 0x40);
        quadSPtr->x = (float)positionX + width;
        quadSPtr->y = (float)boxTopY;
        quadSPtr->z = depth;
        quadSPtr->rhwBits = FLOAT_ONE_BITS;
        quadSPtr->color = 0x50505050;
        quadSPtr->specular = 0xFF000000;
        vertTPtr = (GfxTlVertex *)(g_MenuBandGraphicsObject->vertexCursor + 0x60);
        vertTPtr->x = (float)positionX + width;
        vertTPtr->y = (float)boxTopY + rowOfs;
        vertTPtr->z = depth;
        vertTPtr->rhwBits = FLOAT_ONE_BITS;
        vertTPtr->color = 0x50505050;
        vertTPtr->specular = 0xFF000000;
    }
}

/*
 * Menu_DrawRaisedBox2x's twin, with the viewport origin added to every coordinate.
 * Same five greys, same edges.
 */
// FUNCTION: FF7 0x006fc218
void __cdecl Menu_DrawRaisedBox1x(short *rect, float z)
{
    if (Menu_IsTextSubsystemReady() == 0) {
        return;
    }
    Gfx_EmitFlatRect(rect[0] + g_MenuViewportX,
                     rect[1] + g_MenuViewportY,
                     rect[2] - 1, 1, 0xa0f0f0f0, 0, z,
                     (int)g_MenuFlatRectGraphicsObject);
    Gfx_EmitFlatRect(rect[0] + g_MenuViewportX,
                     rect[1] + (g_MenuViewportY + rect[3]) - 1,
                     rect[2] - 1, 1, 0xa0404040, 0, z,
                     (int)g_MenuFlatRectGraphicsObject);
    Gfx_EmitFlatRect(rect[0] + g_MenuViewportX,
                     rect[1] + g_MenuViewportY,
                     1, rect[3] - 1, 0xa0c8c8c8, 0, z,
                     (int)g_MenuFlatRectGraphicsObject);
    Gfx_EmitFlatRect(rect[0] + (g_MenuViewportX + rect[2]) - 1,
                     rect[1] + g_MenuViewportY,
                     1, rect[3], 0xa0707070, 0, z,
                     (int)g_MenuFlatRectGraphicsObject);
    Gfx_EmitFlatRect(rect[0] + g_MenuViewportX,
                     rect[1] + g_MenuViewportY,
                     rect[2], rect[3], 0xa0a0a0a0, 0, z,
                     (int)g_MenuFlatRectGraphicsObject);
}

/*
 * Menu_DrawScrollBar2x at the 1x scale. Only the two box drawers differ; every
 * coordinate comes out of `desc` either way.
 */
// FUNCTION: FF7 0x006fc38c
void __cdecl Menu_DrawScrollBar1x(short *desc, float z)
{
    EnglishWindowFrame rect;
    short len;
    short gap;

    if (Menu_IsTextSubsystemReady() == 0) {
        return;
    }
    len = (short)(desc[6] * desc[0] / desc[1]);
    gap = (short)(desc[6] * desc[2] / desc[1]);
    rect.x = desc[3];
    rect.y = (short)(desc[4] + gap);
    rect.width = (short)(desc[5] - 1);
    rect.height = (short)(len + 1);
    Menu_DrawRaisedBox1x(&rect.x, z);
    rect.x = desc[3];
    rect.y = desc[4];
    rect.width = desc[5];
    rect.height = desc[6];
    Menu_DrawEtchedBox1x(&rect, z);
}

/*
 * When text is ready, adds g_MenuViewportX and g_MenuViewportY to the origin and, if
 * acquisition of two batch nodes succeeds, fills two four-vertex untextured gauge
 * quads at the supplied depth. The lower half interpolates from 0x90808080 to color
 * and the upper half from 0x90000000 to color, with reciprocal W 1 and black
 * specular fields.
 */
// FUNCTION: FF7 0x006fc462
void __cdecl Menu_DrawGaugeBar1x(int x, int y, int width, int height, unsigned int color, float depth)
{
    GfxTlVertex *vertA;
    GfxTlVertex *vertB;
    GfxTlVertex *vertC;
    GfxTlVertex *vertD;
    GfxTlVertex *vertE;
    GfxTlVertex *vertF;
    GfxTlVertex *vertG;
    GfxTlVertex *vertH;

    if (Menu_IsTextSubsystemReady() == 0) {
        return;
    }
    x += g_MenuViewportX;
    y += g_MenuViewportY;
    if (AcquireBatchNode(2, g_MenuGaugeBarGraphicsObject) != 0) {
        vertA = (GfxTlVertex *)(g_MenuGaugeBarGraphicsObject->vertexCursor);
        vertA->x = (float)x;
        vertA->y = (float)y + height / 2;
        vertA->z = depth;
        vertA->rhwBits = FLOAT_ONE_BITS;
        vertA->color = 0x90808080;
        vertA->specular = 0xFF000000;
        vertB = (GfxTlVertex *)(g_MenuGaugeBarGraphicsObject->vertexCursor + 0x20);
        vertB->x = (float)x;
        vertB->y = (float)y + height;
        vertB->z = depth;
        vertB->rhwBits = FLOAT_ONE_BITS;
        vertB->color = color;
        vertB->specular = 0xFF000000;
        vertC = (GfxTlVertex *)(g_MenuGaugeBarGraphicsObject->vertexCursor + 0x40);
        vertC->x = (float)x + width;
        vertC->y = (float)y + height / 2;
        vertC->z = depth;
        vertC->rhwBits = FLOAT_ONE_BITS;
        vertC->color = 0x90808080;
        vertC->specular = 0xFF000000;
        vertD = (GfxTlVertex *)(g_MenuGaugeBarGraphicsObject->vertexCursor + 0x60);
        vertD->x = (float)x + width;
        vertD->y = (float)y + height;
        vertD->z = depth;
        vertD->rhwBits = FLOAT_ONE_BITS;
        vertD->color = color;
        vertD->specular = 0xFF000000;
        g_MenuGaugeBarGraphicsObject->vertexCursor += g_MenuGaugeBarGraphicsObject->cursorStep;
        vertE = (GfxTlVertex *)(g_MenuGaugeBarGraphicsObject->vertexCursor);
        vertE->x = (float)x;
        vertE->y = (float)y;
        vertE->z = depth;
        vertE->rhwBits = FLOAT_ONE_BITS;
        vertE->color = 0x90000000;
        vertE->specular = 0xFF000000;
        vertF = (GfxTlVertex *)(g_MenuGaugeBarGraphicsObject->vertexCursor + 0x20);
        vertF->x = (float)x;
        vertF->y = (float)y + height / 2;
        vertF->z = depth;
        vertF->rhwBits = FLOAT_ONE_BITS;
        vertF->color = color;
        vertF->specular = 0xFF000000;
        vertG = (GfxTlVertex *)(g_MenuGaugeBarGraphicsObject->vertexCursor + 0x40);
        vertG->x = (float)x + width;
        vertG->y = (float)y;
        vertG->z = depth;
        vertG->rhwBits = FLOAT_ONE_BITS;
        vertG->color = 0x90000000;
        vertG->specular = 0xFF000000;
        vertH = (GfxTlVertex *)(g_MenuGaugeBarGraphicsObject->vertexCursor + 0x60);
        vertH->x = (float)x + width;
        vertH->y = (float)y + height / 2;
        vertH->z = depth;
        vertH->rhwBits = FLOAT_ONE_BITS;
        vertH->color = color;
        vertH->specular = 0xFF000000;
    }
}

/*
 * When text is ready, adds the text origin and draws black fading bands across
 * width width at depth 0.3. With g_MenuTifaReelFadeActive nonzero, acquires two quads fading
 * from full-alpha black at the outer edges to transparent over integer-divided
 * top/bottom thirds, advancing between them.
 */
// FUNCTION: FF7 0x006fc73a
void Menu_DrawTifaReelFadeBands1x(int originX, int originY, int width, int height)
{
    struct {
        GfxTlVertex *loopH;
        GfxTlVertex *loopG;
        GfxTlVertex *loopF;
        GfxTlVertex *loopE;
        GfxTlVertex *loopD;
        GfxTlVertex *loopC;
        GfxTlVertex *loopB;
        GfxTlVertex *loopA;
        int upperY;
        int lowerY;
        MenuColorWord bottomColor;
        int i;
        MenuColorWord topColor;
        GfxTlVertex *simpleH;
        GfxTlVertex *simpleG;
        GfxTlVertex *simpleF;
        GfxTlVertex *simpleE;
        GfxTlVertex *simpleD;
        GfxTlVertex *simpleC;
        GfxTlVertex *simpleB;
        GfxTlVertex *simpleA;
        float depth;
    } status;

    if (Menu_IsTextSubsystemReady() == 0) {
        return;
    }
    originX += g_MenuViewportX;
    originY += g_MenuViewportY;
    status.depth = 0.3f;
    if (g_MenuTifaReelFadeActive != 0) {
        if (AcquireBatchNode(2, g_MenuBlendWindowBgGraphicsObject) != 0) {
            status.simpleA = (GfxTlVertex *)(g_MenuBlendWindowBgGraphicsObject->vertexCursor);
            status.simpleA->x = (float)originX;
            status.simpleA->y = (float)originY;
            status.simpleA->z = status.depth;
            status.simpleA->rhwBits = FLOAT_ONE_BITS;
            status.simpleA->color = 0xFF000000;
            status.simpleA->specular = 0xFF000000;
            status.simpleB = (GfxTlVertex *)(g_MenuBlendWindowBgGraphicsObject->vertexCursor + 0x20);
            status.simpleB->x = (float)originX;
            status.simpleB->y = (float)originY + height / 3;
            status.simpleB->z = status.depth;
            status.simpleB->rhwBits = FLOAT_ONE_BITS;
            status.simpleB->color = 0;
            status.simpleB->specular = 0xFF000000;
            status.simpleC = (GfxTlVertex *)(g_MenuBlendWindowBgGraphicsObject->vertexCursor + 0x40);
            status.simpleC->x = (float)originX + width;
            status.simpleC->y = (float)originY;
            status.simpleC->z = status.depth;
            status.simpleC->rhwBits = FLOAT_ONE_BITS;
            status.simpleC->color = 0xFF000000;
            status.simpleC->specular = 0xFF000000;
            status.simpleD = (GfxTlVertex *)(g_MenuBlendWindowBgGraphicsObject->vertexCursor + 0x60);
            status.simpleD->x = (float)originX + width;
            status.simpleD->y = (float)originY + height / 3;
            status.simpleD->z = status.depth;
            status.simpleD->rhwBits = FLOAT_ONE_BITS;
            status.simpleD->color = 0;
            status.simpleD->specular = 0xFF000000;
            g_MenuBlendWindowBgGraphicsObject->vertexCursor += g_MenuBlendWindowBgGraphicsObject->cursorStep;
            status.simpleE = (GfxTlVertex *)(g_MenuBlendWindowBgGraphicsObject->vertexCursor);
            status.simpleE->x = (float)originX;
            status.simpleE->y = (float)originY + height - height / 3;
            status.simpleE->z = status.depth;
            status.simpleE->rhwBits = FLOAT_ONE_BITS;
            status.simpleE->color = 0;
            status.simpleE->specular = 0xFF000000;
            status.simpleF = (GfxTlVertex *)(g_MenuBlendWindowBgGraphicsObject->vertexCursor + 0x20);
            status.simpleF->x = (float)originX;
            status.simpleF->y = (float)originY + height;
            status.simpleF->z = status.depth;
            status.simpleF->rhwBits = FLOAT_ONE_BITS;
            status.simpleF->color = 0xFF000000;
            status.simpleF->specular = 0xFF000000;
            status.simpleG = (GfxTlVertex *)(g_MenuBlendWindowBgGraphicsObject->vertexCursor + 0x40);
            status.simpleG->x = (float)originX + width;
            status.simpleG->y = (float)originY + height - height / 3;
            status.simpleG->z = status.depth;
            status.simpleG->rhwBits = FLOAT_ONE_BITS;
            status.simpleG->color = 0;
            status.simpleG->specular = 0xFF000000;
            status.simpleH = (GfxTlVertex *)(g_MenuBlendWindowBgGraphicsObject->vertexCursor + 0x60);
            status.simpleH->x = (float)originX + width;
            status.simpleH->y = (float)originY + height;
            status.simpleH->z = status.depth;
            status.simpleH->rhwBits = FLOAT_ONE_BITS;
            status.simpleH->color = 0xFF000000;
            status.simpleH->specular = 0xFF000000;
        }
    } else if (AcquireBatchNode(0x10, g_MenuBandGraphicsObject) != 0) {
        status.topColor.word = status.bottomColor.word = 0;
        for (status.i = 0; status.i < 8; status.i++) {
            status.upperY = status.i * 2;
            status.lowerY = status.upperY + 2;
            status.topColor.bytes[3] = (unsigned char)(0xE0 - status.i * 28);
            status.bottomColor.bytes[3] = (unsigned char)(status.i * 28);
            status.loopA = (GfxTlVertex *)g_MenuBandGraphicsObject->vertexCursor;
            status.loopA->x = (float)originX;
            status.loopA->y = (float)originY + status.upperY;
            status.loopA->z = status.depth;
            status.loopA->rhwBits = FLOAT_ONE_BITS;
            status.loopA->color = status.topColor.word;
            status.loopA->specular = 0xFF000000;
            status.loopB = (GfxTlVertex *)(g_MenuBandGraphicsObject->vertexCursor + 0x20);
            status.loopB->x = (float)originX;
            status.loopB->y = (float)originY + status.lowerY;
            status.loopB->z = status.depth;
            status.loopB->rhwBits = FLOAT_ONE_BITS;
            status.loopB->color = status.topColor.word;
            status.loopB->specular = 0xFF000000;
            status.loopC = (GfxTlVertex *)(g_MenuBandGraphicsObject->vertexCursor + 0x40);
            status.loopC->x = (float)originX + width;
            status.loopC->y = (float)originY + status.upperY;
            status.loopC->z = status.depth;
            status.loopC->rhwBits = FLOAT_ONE_BITS;
            status.loopC->color = status.topColor.word;
            status.loopC->specular = 0xFF000000;
            status.loopD = (GfxTlVertex *)(g_MenuBandGraphicsObject->vertexCursor + 0x60);
            status.loopD->x = (float)originX + width;
            status.loopD->y = (float)originY + status.lowerY;
            status.loopD->z = status.depth;
            status.loopD->rhwBits = FLOAT_ONE_BITS;
            status.loopD->color = status.topColor.word;
            status.loopD->specular = 0xFF000000;
            g_MenuBandGraphicsObject->vertexCursor += g_MenuBandGraphicsObject->cursorStep;
            status.loopE = (GfxTlVertex *)g_MenuBandGraphicsObject->vertexCursor;
            status.loopE->x = (float)originX;
            status.loopE->y = (float)originY + height - status.lowerY;
            status.loopE->z = status.depth;
            status.loopE->rhwBits = FLOAT_ONE_BITS;
            status.loopE->color = status.topColor.word;
            status.loopE->specular = 0xFF000000;
            status.loopF = (GfxTlVertex *)(g_MenuBandGraphicsObject->vertexCursor + 0x20);
            status.loopF->x = (float)originX;
            status.loopF->y = (float)originY + height - status.upperY;
            status.loopF->z = status.depth;
            status.loopF->rhwBits = FLOAT_ONE_BITS;
            status.loopF->color = status.topColor.word;
            status.loopF->specular = 0xFF000000;
            status.loopG = (GfxTlVertex *)(g_MenuBandGraphicsObject->vertexCursor + 0x40);
            status.loopG->x = (float)originX + width;
            status.loopG->y = (float)originY + height - status.lowerY;
            status.loopG->z = status.depth;
            status.loopG->rhwBits = FLOAT_ONE_BITS;
            status.loopG->color = status.topColor.word;
            status.loopG->specular = 0xFF000000;
            status.loopH = (GfxTlVertex *)(g_MenuBandGraphicsObject->vertexCursor + 0x60);
            status.loopH->x = (float)originX + width;
            status.loopH->y = (float)originY + height - status.upperY;
            status.loopH->z = status.depth;
            status.loopH->rhwBits = FLOAT_ONE_BITS;
            status.loopH->color = status.topColor.word;
            status.loopH->specular = 0xFF000000;
            if (status.i != 8) {
                g_MenuBandGraphicsObject->vertexCursor += g_MenuBandGraphicsObject->cursorStep;
            }
        }
    }
}

/*
 * When text is ready, selects g_MenuWinBlend1GraphicsObject for a nonzero useBlend1 or g_MenuWinBlend4GraphicsObject
 * otherwise, adds the layout origin to X/Y and acquires one node. Writes a caller-
 * sized textured quad with UVs derived from source X/Y/width/height divided by
 * 256, caller depth, reciprocal W 1, white diffuse and black specular.
 */
// FUNCTION: FF7 0x006fcd45
void __cdecl Menu_DrawAtlasQuad1x(int posX, int posY, int srcX, int srcY, int width, int height, unsigned int alpha, int useBlend1, int extra, float depth)
{
    float texOriginU;
    GfxRenderObject *blendGraphics;
    float texOriginV;
    float uSpan;
    float texHeight;
    GfxTlVertex *vertexTopLeft;
    GfxTlVertex *vertBottomLeft;
    GfxRenderObject *blendObj;
    GfxTlVertex *pBottomRight;
    GfxTlVertex *vertexTopRightPtr;

    if (Menu_IsTextSubsystemReady() == 0) {
        return;
    }
    if (useBlend1 != 0) {
        blendObj = g_MenuWinBlend1GraphicsObject;
    } else {
        blendObj = (GfxRenderObject *)g_MenuWinBlend4GraphicsObject;
    }
    blendGraphics = blendObj;
    posX += g_MenuViewportX;
    posY += g_MenuViewportY;
    if (AcquireBatchNode(1, blendGraphics) != 0) {
        texOriginU = (float)srcX * 0.00390625f;
        texOriginV = (float)srcY * 0.00390625f;
        uSpan = (float)width * 0.00390625f;
        texHeight = (float)height * 0.00390625f;
        vertexTopLeft = (GfxTlVertex *)(blendGraphics->vertexCursor);
        vertexTopLeft->x = (float)posX;
        vertexTopLeft->y = (float)posY;
        vertexTopLeft->z = depth;
        vertexTopLeft->rhwBits = FLOAT_ONE_BITS;
        vertexTopLeft->color = 0xFFFFFFFF;
        vertexTopLeft->specular = 0xFF000000;
        vertexTopLeft->u = texOriginU;
        vertexTopLeft->v = texOriginV;
        vertBottomLeft = (GfxTlVertex *)(blendGraphics->vertexCursor + 0x20);
        vertBottomLeft->x = (float)posX;
        vertBottomLeft->y = (float)posY + height;
        vertBottomLeft->z = depth;
        vertBottomLeft->rhwBits = FLOAT_ONE_BITS;
        vertBottomLeft->color = 0xFFFFFFFF;
        vertBottomLeft->specular = 0xFF000000;
        vertBottomLeft->u = texOriginU;
        vertBottomLeft->v = texOriginV + texHeight;
        vertexTopRightPtr = (GfxTlVertex *)(blendGraphics->vertexCursor + 0x40);
        vertexTopRightPtr->x = (float)posX + width;
        vertexTopRightPtr->y = (float)posY;
        vertexTopRightPtr->z = depth;
        vertexTopRightPtr->rhwBits = FLOAT_ONE_BITS;
        vertexTopRightPtr->color = 0xFFFFFFFF;
        vertexTopRightPtr->specular = 0xFF000000;
        vertexTopRightPtr->u = texOriginU + uSpan;
        vertexTopRightPtr->v = texOriginV;
        pBottomRight = (GfxTlVertex *)(blendGraphics->vertexCursor + 0x60);
        pBottomRight->x = (float)posX + width;
        pBottomRight->y = (float)posY + height;
        pBottomRight->z = depth;
        pBottomRight->rhwBits = FLOAT_ONE_BITS;
        pBottomRight->color = 0xFFFFFFFF;
        pBottomRight->specular = 0xFF000000;
        pBottomRight->u = texOriginU + uSpan;
        pBottomRight->v = texOriginV + texHeight;
        *(char *)blendGraphics->colorIndex = alpha;
        blendGraphics->texturePage = alpha;
    }
}

/*
 * When text is ready, adds the text origin to coordinates and draws an unsigned
 * decimal value with leading zeroes suppressed and a final digit emitted
 * unconditionally, using white 7-by-8 quads and palette as the palette selector.
 * Clamps values above 999 only for a count of three.
 */
// FUNCTION: FF7 0x006fcf5b
void Menu_DrawDecimalNumber1x(int originX, int originY, unsigned int value, char digits, char palette, float depth)
{
    struct {
        float dv;
        float du;
        float v;
        float u;
        short digit;
        int screenX;
        unsigned int remaining;
        unsigned int divisor;
        short leadingZero;
    } state;

    if (Menu_IsTextSubsystemReady() == 0) {
        return;
    }
    originY += g_MenuViewportY;
    state.divisor = 1;
    state.leadingZero = (short)state.divisor;
    for (state.digit = 1; state.digit < digits; state.digit++) {
        state.divisor *= 10;
    }
    state.remaining = value;
    if (digits == 3 && state.remaining > 999) {
        state.remaining = 999;
    }
    for (state.digit = 1; state.digit < digits; state.digit++) {
        if (state.leadingZero == 0 || state.remaining / state.divisor != 0) {
            state.leadingZero = 0;
            if (AcquireBatchNode(1, g_MenuWinBlend4GraphicsObject) != 0) {
                GfxTlVertex *topRemaining;
                GfxTlVertex *taskVertexBottomLeft;
                GfxTlVertex *vtxTR;
                GfxTlVertex *vertexBottomRight;

                state.u = (float)((state.remaining / state.divisor) * 8 + 0x88) * 0.00390625f;
                state.v = 0.0f;
                state.du = 0.02734375f;
                state.dv = 0.03125f;
                if (g_MenuModuleActive != 0) {
                    state.screenX = originX + (state.digit - 1) * 6 + g_MenuViewportX;
                } else {
                    state.screenX = originX + (state.digit - 1) * 7 + g_MenuViewportX;
                }
                topRemaining = (GfxTlVertex *)(g_MenuWinBlend4GraphicsObject->vertexCursor);
                topRemaining->x = (float)state.screenX;
                topRemaining->y = (float)originY;
                topRemaining->z = depth;
                topRemaining->rhwBits = FLOAT_ONE_BITS;
                topRemaining->color = 0xFFFFFFFF;
                topRemaining->specular = 0xFF000000;
                topRemaining->u = state.u;
                topRemaining->v = state.v;
                taskVertexBottomLeft = (GfxTlVertex *)(g_MenuWinBlend4GraphicsObject->vertexCursor + 0x20);
                taskVertexBottomLeft->x = (float)state.screenX;
                taskVertexBottomLeft->y = (float)originY + 8.0f;
                taskVertexBottomLeft->z = depth;
                taskVertexBottomLeft->rhwBits = FLOAT_ONE_BITS;
                taskVertexBottomLeft->color = 0xFFFFFFFF;
                taskVertexBottomLeft->specular = 0xFF000000;
                taskVertexBottomLeft->u = state.u;
                taskVertexBottomLeft->v = state.v + state.dv;
                vtxTR = (GfxTlVertex *)(g_MenuWinBlend4GraphicsObject->vertexCursor + 0x40);
                vtxTR->x = (float)state.screenX + 7.0f;
                vtxTR->y = (float)originY;
                vtxTR->z = depth;
                vtxTR->rhwBits = FLOAT_ONE_BITS;
                vtxTR->color = 0xFFFFFFFF;
                vtxTR->specular = 0xFF000000;
                vtxTR->u = state.u + state.du;
                vtxTR->v = state.v;
                vertexBottomRight = (GfxTlVertex *)(g_MenuWinBlend4GraphicsObject->vertexCursor + 0x60);
                vertexBottomRight->x = (float)state.screenX + 7.0f;
                vertexBottomRight->y = (float)originY + 8.0f;
                vertexBottomRight->z = depth;
                vertexBottomRight->rhwBits = FLOAT_ONE_BITS;
                vertexBottomRight->color = 0xFFFFFFFF;
                vertexBottomRight->specular = 0xFF000000;
                vertexBottomRight->u = state.u + state.du;
                vertexBottomRight->v = state.v + state.dv;
                *g_MenuWinBlend4GraphicsObject->colorBytePtr = palette;
                g_MenuWinBlend4GraphicsObject->texturePage = palette;
            }
        }
        state.remaining %= state.divisor;
        state.divisor /= 10;
    }
    if (AcquireBatchNode(1, g_MenuWinBlend4GraphicsObject) != 0) {
        GfxTlVertex *altTopRemaining;
        GfxTlVertex *lastBottomLeft;
        GfxTlVertex *altTopRight;
        GfxTlVertex *tempFinalBottomRight;

        state.u = (float)(state.remaining * 8 + 0x88) * 0.00390625f;
        state.v = 0.0f;
        state.du = 0.02734375f;
        state.dv = 0.03125f;
        if (g_MenuModuleActive != 0) {
            state.screenX = originX + (digits - 1) * 6 + g_MenuViewportX;
        } else {
            state.screenX = originX + (digits - 1) * 7 + g_MenuViewportX;
        }
        altTopRemaining = (GfxTlVertex *)(g_MenuWinBlend4GraphicsObject->vertexCursor);
        altTopRemaining->x = (float)state.screenX;
        altTopRemaining->y = (float)originY;
        altTopRemaining->z = depth;
        altTopRemaining->rhwBits = FLOAT_ONE_BITS;
        altTopRemaining->color = 0xFFFFFFFF;
        altTopRemaining->specular = 0xFF000000;
        altTopRemaining->u = state.u;
        altTopRemaining->v = state.v;
        lastBottomLeft = (GfxTlVertex *)(g_MenuWinBlend4GraphicsObject->vertexCursor + 0x20);
        lastBottomLeft->x = (float)state.screenX;
        lastBottomLeft->y = (float)originY + 8.0f;
        lastBottomLeft->z = depth;
        lastBottomLeft->rhwBits = FLOAT_ONE_BITS;
        lastBottomLeft->color = 0xFFFFFFFF;
        lastBottomLeft->specular = 0xFF000000;
        lastBottomLeft->u = state.u;
        lastBottomLeft->v = state.v + state.dv;
        altTopRight = (GfxTlVertex *)(g_MenuWinBlend4GraphicsObject->vertexCursor + 0x40);
        altTopRight->x = (float)state.screenX + 7.0f;
        altTopRight->y = (float)originY;
        altTopRight->z = depth;
        altTopRight->rhwBits = FLOAT_ONE_BITS;
        altTopRight->color = 0xFFFFFFFF;
        altTopRight->specular = 0xFF000000;
        altTopRight->u = state.u + state.du;
        altTopRight->v = state.v;
        tempFinalBottomRight = (GfxTlVertex *)(g_MenuWinBlend4GraphicsObject->vertexCursor + 0x60);
        tempFinalBottomRight->x = (float)state.screenX + 7.0f;
        tempFinalBottomRight->y = (float)originY + 8.0f;
        tempFinalBottomRight->z = depth;
        tempFinalBottomRight->rhwBits = FLOAT_ONE_BITS;
        tempFinalBottomRight->color = 0xFFFFFFFF;
        tempFinalBottomRight->specular = 0xFF000000;
        tempFinalBottomRight->u = state.u + state.du;
        tempFinalBottomRight->v = state.v + state.dv;
        *g_MenuWinBlend4GraphicsObject->colorBytePtr = palette;
        g_MenuWinBlend4GraphicsObject->texturePage = palette;
    }
}

/*
 * When text is ready, draws a zero-padded decimal value using the low byte of
 * digitCount. Builds the unsigned decimal divisor, acquires each digit
 * independently and emits 7-by-8 white atlas quads at 6-pixel pitch when
 * g_MenuModuleActive is nonzero or 7-pixel pitch otherwise, adding global text origin.
 */
// FUNCTION: FF7 0x006fd48b
void __cdecl Menu_DrawDecimalNumberZeroPad1x(int x, int y, unsigned int value, unsigned int digitCount, unsigned char colorIndex, float z)
{
    struct {
        float dv;
        float du;
        float v;
        float u;
        short digit;
        int screenX;
        unsigned int remaining;
        unsigned int divisor;
        short unusedLeadingZero;
    } state;

    if (Menu_IsTextSubsystemReady() == 0) {
        return;
    }
    y += g_MenuViewportY;
    state.divisor = 1;
    state.unusedLeadingZero = (short)state.divisor;
    for (state.digit = 1; state.digit < (int)(digitCount & 0xFF); state.digit++) {
        state.divisor *= 10;
    }
    state.remaining = value;
    for (state.digit = 1; state.digit < (int)(digitCount & 0xFF); state.digit++) {
        if (AcquireBatchNode(1, g_MenuWinBlend4GraphicsObject) != 0) {
            GfxTlVertex *topRemaining;
            GfxTlVertex *b;
            GfxTlVertex *nextCorner;
            GfxTlVertex *bottomRightAddr;

            state.u = (float)((state.remaining / state.divisor) * 8 + 0x88) * 0.00390625f;
            state.v = 0.0f;
            state.du = 0.02734375f;
            state.dv = 0.03125f;
            if (g_MenuModuleActive != 0) {
                state.screenX = x + (state.digit - 1) * 6 + g_MenuViewportX;
            } else {
                state.screenX = x + (state.digit - 1) * 7 + g_MenuViewportX;
            }
            topRemaining = (GfxTlVertex *)(g_MenuWinBlend4GraphicsObject->vertexCursor);
            topRemaining->x = (float)state.screenX;
            topRemaining->y = (float)y;
            topRemaining->z = z;
            topRemaining->rhwBits = FLOAT_ONE_BITS;
            topRemaining->color = 0xFFFFFFFF;
            topRemaining->specular = 0xFF000000;
            topRemaining->u = state.u;
            topRemaining->v = state.v;
            b = (GfxTlVertex *)(g_MenuWinBlend4GraphicsObject->vertexCursor + 0x20);
            b->x = (float)state.screenX;
            b->y = (float)y + 8.0f;
            b->z = z;
            b->rhwBits = FLOAT_ONE_BITS;
            b->color = 0xFFFFFFFF;
            b->specular = 0xFF000000;
            b->u = state.u;
            b->v = state.v + state.dv;
            nextCorner = (GfxTlVertex *)(g_MenuWinBlend4GraphicsObject->vertexCursor + 0x40);
            nextCorner->x = (float)state.screenX + 7.0f;
            nextCorner->y = (float)y;
            nextCorner->z = z;
            nextCorner->rhwBits = FLOAT_ONE_BITS;
            nextCorner->color = 0xFFFFFFFF;
            nextCorner->specular = 0xFF000000;
            nextCorner->u = state.u + state.du;
            nextCorner->v = state.v;
            bottomRightAddr = (GfxTlVertex *)(g_MenuWinBlend4GraphicsObject->vertexCursor + 0x60);
            bottomRightAddr->x = (float)state.screenX + 7.0f;
            bottomRightAddr->y = (float)y + 8.0f;
            bottomRightAddr->z = z;
            bottomRightAddr->rhwBits = FLOAT_ONE_BITS;
            bottomRightAddr->color = 0xFFFFFFFF;
            bottomRightAddr->specular = 0xFF000000;
            bottomRightAddr->u = state.u + state.du;
            bottomRightAddr->v = state.v + state.dv;
            *g_MenuWinBlend4GraphicsObject->colorBytePtr = colorIndex;
            g_MenuWinBlend4GraphicsObject->texturePage = colorIndex;
        }
        state.remaining %= state.divisor;
        state.divisor /= 10;
    }
    if (AcquireBatchNode(1, g_MenuWinBlend4GraphicsObject) != 0) {
        GfxTlVertex *baseLastTopLeft;
        GfxTlVertex *lastBottomLeft;
        GfxTlVertex *nextLastTopRight;
        GfxTlVertex *h;

        state.u = (float)(state.remaining * 8 + 0x88) * 0.00390625f;
        state.v = 0.0f;
        state.du = 0.02734375f;
        state.dv = 0.03125f;
        if (g_MenuModuleActive != 0) {
            state.screenX = x + ((digitCount & 0xFF) - 1) * 6 + g_MenuViewportX;
        } else {
            state.screenX = x + ((digitCount & 0xFF) - 1) * 7 + g_MenuViewportX;
        }
        baseLastTopLeft = (GfxTlVertex *)(g_MenuWinBlend4GraphicsObject->vertexCursor);
        baseLastTopLeft->x = (float)state.screenX;
        baseLastTopLeft->y = (float)y;
        baseLastTopLeft->z = z;
        baseLastTopLeft->rhwBits = FLOAT_ONE_BITS;
        baseLastTopLeft->color = 0xFFFFFFFF;
        baseLastTopLeft->specular = 0xFF000000;
        baseLastTopLeft->u = state.u;
        baseLastTopLeft->v = state.v;
        lastBottomLeft = (GfxTlVertex *)(g_MenuWinBlend4GraphicsObject->vertexCursor + 0x20);
        lastBottomLeft->x = (float)state.screenX;
        lastBottomLeft->y = (float)y + 8.0f;
        lastBottomLeft->z = z;
        lastBottomLeft->rhwBits = FLOAT_ONE_BITS;
        lastBottomLeft->color = 0xFFFFFFFF;
        lastBottomLeft->specular = 0xFF000000;
        lastBottomLeft->u = state.u;
        lastBottomLeft->v = state.v + state.dv;
        nextLastTopRight = (GfxTlVertex *)(g_MenuWinBlend4GraphicsObject->vertexCursor + 0x40);
        nextLastTopRight->x = (float)state.screenX + 7.0f;
        nextLastTopRight->y = (float)y;
        nextLastTopRight->z = z;
        nextLastTopRight->rhwBits = FLOAT_ONE_BITS;
        nextLastTopRight->color = 0xFFFFFFFF;
        nextLastTopRight->specular = 0xFF000000;
        nextLastTopRight->u = state.u + state.du;
        nextLastTopRight->v = state.v;
        h = (GfxTlVertex *)(g_MenuWinBlend4GraphicsObject->vertexCursor + 0x60);
        h->x = (float)state.screenX + 7.0f;
        h->y = (float)y + 8.0f;
        h->z = z;
        h->rhwBits = FLOAT_ONE_BITS;
        h->color = 0xFFFFFFFF;
        h->specular = 0xFF000000;
        h->u = state.u + state.du;
        h->v = state.v + state.dv;
        *g_MenuWinBlend4GraphicsObject->colorBytePtr = colorIndex;
        g_MenuWinBlend4GraphicsObject->texturePage = colorIndex;
    }
}

/*
 * Flush the six primitive lists in two passes, then move the viewport.
 */
// FUNCTION: FF7 0x006fd9a3
void __cdecl Menu_FlushAndSetScissor1x(int x, int y, int w, int h)
{
    GfxEngineContext *ctx;

    ctx = GetEngineContext();
    x += g_MenuViewportX;
    y += g_MenuViewportY;
    Gfx_SetScenePassCall(0, ctx);
    Gfx_SetRenderStateCall(RENDER_STATE_LINEAR_FILTER, 0, ctx);
    Gfx_EmitPrimitiveBatchList(g_MenuWindowBgGraphicsObject, ctx);
    Gfx_EmitPrimitiveBatchList(g_MenuWinBlend4GraphicsObject, ctx);
    Gfx_EmitPrimitiveBatchList(g_MenuFontBlend4GraphicsObject, ctx);
    Gfx_EmitPrimitiveBatchList(g_MenuWinCBlend4DiffGraphicsObject, ctx);
    Gfx_SetScenePassCall(1, ctx);
    Gfx_SetRenderStateCall(RENDER_STATE_LINEAR_FILTER, 0, ctx);
    Gfx_EmitPrimitiveBatchList(g_MenuWinBlend1GraphicsObject, ctx);
    Gfx_EmitPrimitiveBatchList(g_MenuBlendWindowBgGraphicsObject, ctx);
    InvalidateBatchGeneration(g_MenuBlendWindowBgGraphicsObject);
    InvalidateBatchGeneration(g_MenuFontBlend4GraphicsObject);
    InvalidateBatchGeneration(g_MenuWinBlend4GraphicsObject);
    InvalidateBatchGeneration(g_MenuWinCBlend4DiffGraphicsObject);
    InvalidateBatchGeneration(g_MenuWinBlend1GraphicsObject);
    InvalidateBatchGeneration(g_MenuWindowBgGraphicsObject);
    GfxSetViewportRect(x, y, w, h, ctx);
}

/*
 * Menu_FlushAndSetScissor1x with the viewport put back to the whole screen instead of a rectangle
 * the caller chose. Same two passes and the same six lists, but the flushing and the invalidating
 * are interleaved rather than separated: g_MenuBlendWindowBgGraphicsObject is
 * invalidated between the two pass-1 emits, and g_MenuWinBlend1GraphicsObject's
 * invalidation waits until the end.
 */
// FUNCTION: FF7 0x006fdaeb
void __cdecl Menu_FlushAndResetScissor1x(void)
{
    GfxEngineContext *ctx;

    ctx = GetEngineContext();
    Gfx_SetScenePassCall(0, ctx);
    Gfx_SetRenderStateCall(RENDER_STATE_LINEAR_FILTER, 0, ctx);
    Gfx_EmitPrimitiveBatchList(g_MenuWindowBgGraphicsObject, ctx);
    Gfx_EmitPrimitiveBatchList(g_MenuWinBlend4GraphicsObject, ctx);
    Gfx_EmitPrimitiveBatchList(g_MenuFontBlend4GraphicsObject, ctx);
    Gfx_EmitPrimitiveBatchList(g_MenuWinCBlend4DiffGraphicsObject, ctx);
    Gfx_SetScenePassCall(1, ctx);
    Gfx_SetRenderStateCall(RENDER_STATE_LINEAR_FILTER, 0, ctx);
    Gfx_EmitPrimitiveBatchList(g_MenuBlendWindowBgGraphicsObject, ctx);
    InvalidateBatchGeneration(g_MenuBlendWindowBgGraphicsObject);
    Gfx_EmitPrimitiveBatchList(g_MenuWinBlend1GraphicsObject, ctx);
    InvalidateBatchGeneration(g_MenuFontBlend4GraphicsObject);
    InvalidateBatchGeneration(g_MenuWindowBgGraphicsObject);
    InvalidateBatchGeneration(g_MenuWinBlend4GraphicsObject);
    InvalidateBatchGeneration(g_MenuWinCBlend4DiffGraphicsObject);
    InvalidateBatchGeneration(g_MenuWinBlend1GraphicsObject);
    GfxSetViewportRect(g_MenuViewportX, g_MenuViewportY, g_MenuViewportWidth,
                       g_MenuViewportHeight, ctx);
}

/*
 * Flush either one batch list or two, depending on g_MenuIsSmallViewport320x240, and
 * invalidate whichever it flushed.
 */
// FUNCTION: FF7 0x006fdc26
void __cdecl Menu_EmitFontBatchLists(struct GfxEngineContext *ctx)
{
    if (g_MenuIsSmallViewport320x240 != 1) {
        Gfx_EmitPrimitiveBatchList(g_MenuFontAGraphicsObject, ctx);
        Gfx_EmitPrimitiveBatchList(g_MenuFontBGraphicsObject.batchList, ctx);
        InvalidateBatchGeneration(g_MenuFontAGraphicsObject);
        InvalidateBatchGeneration(g_MenuFontBGraphicsObject.batchList);
    } else {
        Gfx_EmitPrimitiveBatchList(g_MenuFontBlend4GraphicsObject, ctx);
        InvalidateBatchGeneration(g_MenuFontBlend4GraphicsObject);
    }
}
} /* extern "C" */
