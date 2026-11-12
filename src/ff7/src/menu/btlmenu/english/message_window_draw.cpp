// src/ff7/src/menu/btlmenu/english/message_window_draw.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Message and popup windows of the menu: window colour and rectangle helpers, window
//   frames and the more-text arrow, rendering of active message windows with their text at both
//   scales, character portraits, the countdown timer, and the flush of UI batches.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/codegen.h"
#include <string.h>
#include "ff7/types/menu.h"
#include "ff7/enums/main.h"
#include "ff7/enums/menu.h"
#include "lib/types/graphics.h"
#include "lib/enums/graphics.h"
#include <stdlib.h>
#include "ff7/api/menu.h"
#define FF7_GFX_EMITFLATRECT_FLOAT_Z
#include "lib/api/graphics.h"
#include "lib/api/token.h"
#include "ff7/savemap.h"
extern "C" {

struct GfxEngineContext;
struct EnglishWindowFrame;
unsigned char g_rawData_0091efa0[40] = {
    0x14, 0x00, 0x00, 0x00, 0xa0, 0x00, 0x00, 0x00, 0x78, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00,
    0x06, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
// GLOBAL: FF7 0x0091efc8
unsigned char g_MenuCornerColorTL[4] = {0xb0, 0x58, 0x00, 0x80};
// GLOBAL: FF7 0x0091efcc
unsigned char g_MenuCornerColorTR[4] = {0x80, 0x00, 0x00, 0x80};
// GLOBAL: FF7 0x0091efd0
unsigned char g_MenuCornerColorBL[4] = {0x50, 0x00, 0x00, 0x80};
// GLOBAL: FF7 0x0091efd4
unsigned char g_MenuCornerColorBR[4] = {0x20, 0x00, 0x00, 0x80};
// GLOBAL: FF7 0x0091efd8
unsigned char g_FieldWindowColours[12] = {0x00, 0x58, 0xb0, 0x00, 0x00, 0x80, 0x00, 0x00, 0x50, 0x00, 0x00, 0x20};
// The draw depth of each message window, as float bit patterns passed to the text and frame draws.
// GLOBAL: FF7 0x0091efe8
int g_MessageWindowDepthBits[4] = {0x3727c5ac, 0x37388ca4, 0x3749539c, 0x375a1a93};
// GLOBAL: FF7 0x0091eff8
int g_MenuTimeDivisors[4] = {600, 60, 10, 1};
// GLOBAL: FF7 0x0091f008
int g_MenuNumberDivisors[8] = {10000000, 1000000, 100000, 10000, 1000, 100, 10, 1};
// GLOBAL: FF7 0x0091f028
short g_MenuTextColour[2] = {7, 0};
unsigned char g_rawData_0091f02c[4] = {0xff, 0xff, 0xff, 0xff};
unsigned char g_rawData_0091f030[4] = {0x0f, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x00dc3ca0
int g_BtlMessagePortraitHandles[4] = {0};
// GLOBAL: FF7 0x00dc3cb0
int g_MenuTextColumn = 0;
// GLOBAL: FF7 0x00dc3cb4
int g_MenuMessageX = 0;
// GLOBAL: FF7 0x00dc3cb8
int g_MenuTextRow = 0;
unsigned char g_bssPad_00dc3cbc[4] = {0};
// GLOBAL: FF7 0x00dc3cc0
short g_MenuTextBlink = 0;
// GLOBAL: FF7 0x00dc3cc4
short g_MenuTextRainbow = 0;
// GLOBAL: FF7 0x00dc3cc8
short g_MenuTextTick = 0;
// GLOBAL: FF7 0x00dc3ccc
short g_MenuTextRemaining = 0;
// GLOBAL: FF7 0x00dc3cd0
int g_MenuMessageWindowStyleFlag4 = 0;
// GLOBAL: FF7 0x00dc3cd4
int g_MenuTextFixedPitch = 0;
// GLOBAL: FF7 0x00dc3cd8
int g_MenuMessageWindowActive = 0;
// GLOBAL: FF7 0x00dc3cdc
int g_MenuBlendWindowBgPending = 0;
// GLOBAL: FF7 0x00dc3ce0
int g_MenuWindowBgPending = 0;
// GLOBAL: FF7 0x00dc3ce4
int g_MenuWinCBlend4 = 0;
// GLOBAL: FF7 0x00dc3ce8
int g_MenuDrawTextBoxes = 0;
// GLOBAL: FF7 0x00dc3cec
int g_MenuDrawCharacter = 0;
// GLOBAL: FF7 0x00dc3cf0
int g_MenuDoDrawBlackQuad = 0;
// GLOBAL: FF7 0x00dc3cf4
int g_MenuMessageWindowYOffset = 0;
// GLOBAL: FF7 0x00dc3cf8
int g_MenuDrawBox = 0;
// GLOBAL: FF7 0x00dc3cfc
unsigned char g_MenuDigitBackdropAlpha = 0;
// GLOBAL: FF7 0x00dc3d00
int g_MenuDigitBackdropEnabled = 0;
// GLOBAL: FF7 0x00dc3d04
int g_MenuShouldDrawBlackQuad = 0;
unsigned char g_bssPad_00dc3d08[4] = {0};
// GLOBAL: FF7 0x00dc3d0c
int g_InputPressedSnapshot = 0;
// GLOBAL: FF7 0x00dc3d10
int g_InputHeldSnapshot = 0;
// GLOBAL: FF7 0x00dc3d14
unsigned char g_MenuNonTopLevelScreenFlag = 0;
// clang-format on
#pragma intrinsic(abs, memcpy, memset)

int __cdecl Gfx_EmitFlatRect(int x, int y, int width, int height,
                             unsigned int colour, int flags, float z,
                             int surface);

/*
 * Store a window colour as B, G, R, then a fixed alpha of 0x80.
 */
// FUNCTION: FF7 0x006e6c30
void __cdecl Menu_PackWindowColor(unsigned char *dest, unsigned char r, unsigned char g, unsigned char b)
{
    dest[2] = r;
    dest[1] = g;
    dest[0] = b;
    dest[3] = 0x80;
}

/*
 * Empty. A menu hook the shipped build left with no body.
 */
// FUNCTION: FF7 0x006e6c56
void BtlMenu_UnusedMessageDrawNoOpA(void)
{
    /* The original is empty. Its whole body is
       `55 8b ec 5d c3` -- 5 bytes at 0x006e6c56,
       push/mov/pop/ret plus alignment padding.
       The C body is empty, as in retail. */
}

/*
 * Write the 84x96 portrait quad for a party slot into that slot's batch node,
 * textured from (0, 0) to (0.65625, 0.75), white with a 0xFF000000 specular and
 * the depth passed in. The batch's dirty byte and count are cleared afterwards, so
 * this is the whole batch.
 */
// FUNCTION: FF7 0x006e6c5b
void __cdecl Menu_DrawCharPortrait2x(int x, int y, int slot, float z)
{
    float vStart;
    float curUStartScalar;
    float uSpan;
    GfxTlVertex *vertexAPtr;
    float texHeight;
    GfxTlVertex *bottomRemaining;
    GfxTlVertex *v;
    GfxTlVertex *bottomRight;

    if (Menu_IsTextSubsystemReady() == 0) {
        return;
    }
    if (AcquireBatchNode(1, g_MenuFontBGraphicsObject.portraitRenderObjects[slot]) != 0) {
        vStart = 0.0f;
        curUStartScalar = vStart;
        uSpan = 0.65625f;
        texHeight = 0.75f;
        vertexAPtr = g_MenuFontBGraphicsObject.portraitRenderObjects[slot]->vertices;
        vertexAPtr->x = x;
        vertexAPtr->y = y;
        vertexAPtr->z = z;
        vertexAPtr->rhwBits = FLOAT_ONE_BITS;
        vertexAPtr->color = 0xFFFFFFFF;
        vertexAPtr->specular = 0xFF000000;
        vertexAPtr->u = curUStartScalar;
        vertexAPtr->v = vStart;
        bottomRemaining = g_MenuFontBGraphicsObject.portraitRenderObjects[slot]->vertices + 1;
        bottomRemaining->x = x;
        bottomRemaining->y = y + 96.0f;
        bottomRemaining->z = z;
        bottomRemaining->rhwBits = FLOAT_ONE_BITS;
        bottomRemaining->color = 0xFFFFFFFF;
        bottomRemaining->specular = 0xFF000000;
        bottomRemaining->u = curUStartScalar;
        bottomRemaining->v = vStart + texHeight;
        v = g_MenuFontBGraphicsObject.portraitRenderObjects[slot]->vertices + 2;
        v->x = x + 84.0f;
        v->y = y;
        v->z = z;
        v->rhwBits = FLOAT_ONE_BITS;
        v->color = 0xFFFFFFFF;
        v->specular = 0xFF000000;
        v->u = curUStartScalar + uSpan;
        v->v = vStart;
        bottomRight = g_MenuFontBGraphicsObject.portraitRenderObjects[slot]->vertices + 3;
        bottomRight->x = x + 84.0f;
        bottomRight->y = y + 96.0f;
        bottomRight->z = z;
        bottomRight->rhwBits = FLOAT_ONE_BITS;
        bottomRight->color = 0xFFFFFFFF;
        bottomRight->specular = 0xFF000000;
        bottomRight->u = curUStartScalar + uSpan;
        bottomRight->v = vStart + texHeight;
        *g_MenuFontBGraphicsObject.portraitRenderObjects[slot]->colorSignedBytePtr = 0;
        g_MenuFontBGraphicsObject.portraitRenderObjects[slot]->paletteIndex = 0;
    }
}

/*
 * Empty. A menu hook the shipped build left with no body.
 */
// FUNCTION: FF7 0x006e6e61
void BtlMenu_UnusedMessageDrawNoOpB(void)
{
    /* The original is empty. Its whole body is
       `55 8b ec 5d c3` -- 5 bytes at 0x006e6e61,
       push/mov/pop/ret plus alignment padding.
       The C body is empty, as in retail. */
}

/*
 * Returns unless the text subsystem is ready, maps character ID 9 to 8 and
 * acquires that character portrait batch from g_MenuCharPortraitGraphicsObjects. On success writes a
 * four-vertex rectangle at caller X/Y and width/height, with caller depth,
 * reciprocal W 1, white diffuse and black specular.
 */
// FUNCTION: FF7 0x006e6e66
void __cdecl Menu_DrawCharPortraitById2x(int originX, int originY, int width, int height, int charId, float depth)
{
    float vOrigin;
    float uOrigin;
    GfxTlVertex *v;
    GfxTlVertex *vBottomLeft;
    GfxTlVertex *vertTopLeft;
    float vSize;
    float uExtent;
    GfxTlVertex *vertBottomRight;

    if (Menu_IsTextSubsystemReady() == 0) {
        return;
    }
    if (charId == 9) {
        charId = 8;
    }
    if (AcquireBatchNode(1, g_MenuCharPortraitGraphicsObjects[charId]) != 0) {
        vOrigin = 0.0f;
        uOrigin = vOrigin;
        uExtent = 0.65625f;
        vSize = 0.75f;
        vertTopLeft = g_MenuCharPortraitGraphicsObjects[charId]->vertices;
        vertTopLeft->x = (float)originX;
        vertTopLeft->y = (float)originY;
        vertTopLeft->z = depth;
        vertTopLeft->rhwBits = FLOAT_ONE_BITS;
        vertTopLeft->color = 0xFFFFFFFF;
        vertTopLeft->specular = 0xFF000000;
        vertTopLeft->u = uOrigin;
        vertTopLeft->v = vOrigin;
        vBottomLeft = g_MenuCharPortraitGraphicsObjects[charId]->vertices + 1;
        vBottomLeft->x = (float)originX;
        vBottomLeft->y = (float)originY + height;
        vBottomLeft->z = depth;
        vBottomLeft->rhwBits = FLOAT_ONE_BITS;
        vBottomLeft->color = 0xFFFFFFFF;
        vBottomLeft->specular = 0xFF000000;
        vBottomLeft->u = uOrigin;
        vBottomLeft->v = vOrigin + vSize;
        v = g_MenuCharPortraitGraphicsObjects[charId]->vertices + 2;
        v->x = (float)originX + width;
        v->y = (float)originY;
        v->z = depth;
        v->rhwBits = FLOAT_ONE_BITS;
        v->color = 0xFFFFFFFF;
        v->specular = 0xFF000000;
        v->u = uOrigin + uExtent;
        v->v = vOrigin;
        vertBottomRight = g_MenuCharPortraitGraphicsObjects[charId]->vertices + 3;
        vertBottomRight->x = (float)originX + width;
        vertBottomRight->y = (float)originY + height;
        vertBottomRight->z = depth;
        vertBottomRight->rhwBits = FLOAT_ONE_BITS;
        vertBottomRight->color = 0xFFFFFFFF;
        vertBottomRight->specular = 0xFF000000;
        vertBottomRight->u = uOrigin + uExtent;
        vertBottomRight->v = vOrigin + vSize;
        *g_MenuCharPortraitGraphicsObjects[charId]->colorSignedBytePtr = 0;
        g_MenuCharPortraitGraphicsObjects[charId]->paletteIndex = 0;
    }
}

/*
 * Renders up to 1024 encoded-message iterations at 2x scale with shared reveal
 * count, row/column counters and wrap width, returning final Y. Handles line
 * breaks, glyph banks, color/blink/rainbow/fixed-width controls and four 32-pixel
 * icons.
 */
// FUNCTION: FF7 0x006e706d
short __cdecl Menu_DrawMessageText2x(short penX, short penY, short wrapWidth, unsigned char *text, float z)
{
    struct {
        GfxTlVertex *vtx7, *vtx6, *vtx5, *vtx4, *vtx3, *vtx2, *vtx1, *vtx0;
        short skip, padSkip, first, padFirst;
        float dv, du;
        short code, padCode;
        float texV;
        short colour, padColour, unused, padUnused;
        float texU;
        short glyph, padGlyph, count, padCount;
        GfxRenderObject *batch;
        short offset, padOffset, clipped, padClipped, atlas, padAtlas, atlasY, padY;
        unsigned short phase, padPhase;
        short atlasX, padX;
        float width;
    } f; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
#define TEXT_METRIC (g_pTextSpacing[f.code + f.offset])
    f.phase = 0;
    for (f.count = 0; f.count < 1024; f.count++, f.phase++) {
        f.first = *text;
        if (g_MenuTextRemaining == 0 || *text == TEXT_END || *text == TEXT_NEW_PAGE || *text == TEXT_NEW_PAGE_ALT)
            break;
        if (*text == TEXT_NEWLINE) {
            penX = g_MenuMessageX + 16;
            penY += 32;
            text++;
            g_MenuTextRow++;
            g_MenuTextColumn++;
            continue;
        }
        f.skip = 0;
        f.unused = f.skip;
        switch (*text) {
        case TEXT_FONT_PAGE_1:
            g_MenuTextColumn++;
            text++;
            f.atlasY = 132;
            f.offset = 0xe7;
            break;
        case TEXT_FONT_PAGE_2:
            g_MenuTextColumn++;
            text++;
            f.atlasY = 0;
            f.skip = 16;
            f.offset = 0x1b9;
            break;
        case TEXT_FONT_PAGE_3:
            g_MenuTextColumn++;
            text++;
            f.atlasY = 132;
            f.skip = 16;
            f.offset = 0x2a0;
            break;
        case TEXT_FONT_PAGE_4:
            g_MenuTextColumn++;
            text++;
            f.atlasY = 132;
            f.offset = 0x372;
            f.unused = -64;
            break;
        case TEXT_EXTENDED:
            text++;
            g_MenuTextColumn++;
            if (*text < TEXT_FE_COLOR_0) {
                f.atlasY = 132;
                f.skip = 16;
                f.offset = 0x444;
                f.unused = -64;
            } else {
                g_MenuTextColumn++;
                if (*text < TEXT_FE_BLINK) {
                    g_MenuTextColour[0] = *text - TEXT_FE_COLOR_0;
                    text++;
                    continue;
                } else if (*text == TEXT_FE_BLINK) {
                    g_MenuTextBlink ^= 1;
                    text++;
                    continue;
                } else if (*text == TEXT_FE_RAINBOW) {
                    g_MenuTextRainbow ^= 1;
                    text++;
                    continue;
                } else if (*text == TEXT_FE_FIXED_PITCH) {
                    g_MenuTextFixedPitch ^= 1;
                    text++;
                    continue;
                }
            }
            break;
        default:
            if (*text >= TEXT_WINDOW_ICON_FIRST && *text <= TEXT_WINDOW_ICON_LAST) {
                switch (*text - TEXT_WINDOW_ICON_FIRST) {
                case TEXT_ICON_OFFSET_CIRCLE:
                    f.atlasX = 192;
                    f.atlasY = 128;
                    f.batch = g_MenuWinABlend4GraphicsObject;
                    break;
                case TEXT_ICON_OFFSET_TRIANGLE:
                    f.atlasX = 32;
                    f.atlasY = 128;
                    f.batch = g_MenuWinBBlend4GraphicsObject;
                    break;
                case TEXT_ICON_OFFSET_SQUARE:
                    f.atlasX = 0;
                    f.atlasY = 128;
                    f.batch = g_MenuWinBBlend4GraphicsObject;
                    break;
                case TEXT_ICON_OFFSET_CROSS:
                    f.atlasX = 224;
                    f.atlasY = 128;
                    f.batch = g_MenuWinABlend4GraphicsObject;
                    break;
                }
                if (AcquireBatchNode(1, f.batch)) {
                    f.texU = f.atlasX * 0.00390625f;
                    f.texV = 0.5f;
                    f.du = 0.125f;
                    f.dv = 0.125f;
                    f.vtx0 = f.batch->vertices;
                    f.vtx0->x = (float)penX;
                    f.vtx0->y = (float)penY - 4.0f;
                    f.vtx0->z = z;
                    f.vtx0->rhwBits = FLOAT_ONE_BITS;
                    f.vtx0->color = 0xffffffff;
                    f.vtx0->specular = 0xff000000;
                    f.vtx0->u = f.texU;
                    f.vtx0->v = f.texV;
                    f.vtx1 = (GfxTlVertex *)(f.batch->vertexCursor + 32);
                    f.vtx1->x = (float)penX;
                    f.vtx1->y = (float)penY - 4.0f + 32.0f;
                    f.vtx1->z = z;
                    f.vtx1->rhwBits = FLOAT_ONE_BITS;
                    f.vtx1->color = 0xffffffff;
                    f.vtx1->specular = 0xff000000;
                    f.vtx1->u = f.texU;
                    f.vtx1->v = f.texV + f.dv;
                    f.vtx2 = (GfxTlVertex *)(f.batch->vertexCursor + 64);
                    f.vtx2->x = (float)penX + 32.0f;
                    f.vtx2->y = (float)penY - 4.0f;
                    f.vtx2->z = z;
                    f.vtx2->rhwBits = FLOAT_ONE_BITS;
                    f.vtx2->color = 0xffffffff;
                    f.vtx2->specular = 0xff000000;
                    f.vtx2->u = f.texU + f.du;
                    f.vtx2->v = f.texV;
                    f.vtx3 = (GfxTlVertex *)(f.batch->vertexCursor + 96);
                    f.vtx3->x = (float)penX + 32.0f;
                    f.vtx3->y = (float)penY - 4.0f + 32.0f;
                    f.vtx3->z = z;
                    f.vtx3->rhwBits = FLOAT_ONE_BITS;
                    f.vtx3->color = 0xffffffff;
                    f.vtx3->specular = 0xff000000;
                    f.vtx3->u = f.texU + f.du;
                    f.vtx3->v = f.texV + f.dv;
                    *(unsigned char *)f.batch->colorIndex = 7;
                    f.batch->paletteIndex = 7;
                }
                g_MenuDrawTextBoxes = 1;
                text++;
                g_MenuTextRemaining--;
                g_MenuTextColumn++;
                penX += 32;
                continue;
            }
            f.offset = 0;
            f.atlasY = 0;
        }
        if (!g_MenuTextBlink && !g_MenuTextRainbow)
            f.colour = g_MenuTextColour[0];
        else if (g_MenuTextRainbow)
            f.colour = ((g_MenuTextTick >> 2) - f.phase) & 7;
        else if ((g_MenuTextTick >> 2) & 1)
            f.colour = g_MenuTextColour[0];
        else {
            if (!g_MenuTextColour[0]) {
                penX += f.skip;
                continue;
            }
            f.colour = 0;
        }
        f.code = *text;
        f.glyph = *text;
        if (f.glyph == 0xd2 || f.glyph == 0xd3)
            f.glyph -= 0x4e;
        f.atlasX = (f.glyph % 21) * 24;
        f.atlasY += (f.glyph / 21) * 24;
        if (penX - g_MenuMessageX + ((TEXT_METRIC >> 5) + (TEXT_METRIC & 31)) * 2 > wrapWidth) {
            penX = g_MenuMessageX + 16;
            penY += 32;
            g_MenuTextRow++;
        }
        if (!g_MenuTextFixedPitch)
            penX += (TEXT_METRIC >> 5) << 1;
        if (f.atlasX > 240) {
            f.atlas = f.atlasX - 256;
            f.width = 24.0f;
            f.clipped = 24;
            f.batch = (GfxRenderObject *)g_MenuFontBGraphicsObject.batchList;
        } else {
            if (f.atlasX == 240) {
                f.atlas = f.atlasX;
                f.width = 16.0f;
                f.clipped = 16;
            } else {
                f.atlas = f.atlasX;
                f.width = 24.0f;
                f.clipped = 24;
            }
            f.batch = g_MenuFontAGraphicsObject;
        }
        if (AcquireBatchNode(1, f.batch)) {
            f.texU = f.atlas / 256.0f;
            f.texV = f.atlasY / 256.0f;
            f.du = f.width / 256.0f;
            f.dv = 0.09375f;
            f.vtx4 = f.batch->vertices;
            f.vtx4->x = (float)penX;
            f.vtx4->y = (float)penY;
            f.vtx4->z = z;
            f.vtx4->rhwBits = FLOAT_ONE_BITS;
            f.vtx4->color = 0xffffffff;
            f.vtx4->specular = 0xff000000;
            *(int *)&f.vtx4->u = *(int *)&f.texU;
            *(int *)&f.vtx4->v = *(int *)&f.texV;
            f.vtx5 = (GfxTlVertex *)(f.batch->vertexCursor + 32);
            f.vtx5->x = (float)penX;
            f.vtx5->y = (float)penY + 24.0f;
            f.vtx5->z = z;
            f.vtx5->rhwBits = FLOAT_ONE_BITS;
            f.vtx5->color = 0xffffffff;
            f.vtx5->specular = 0xff000000;
            *(int *)&f.vtx5->u = *(int *)&f.texU;
            f.vtx5->v = f.texV + f.dv;
            f.vtx6 = (GfxTlVertex *)(f.batch->vertexCursor + 64);
            f.vtx6->x = (float)penX + f.clipped;
            f.vtx6->y = (float)penY;
            f.vtx6->z = z;
            f.vtx6->rhwBits = FLOAT_ONE_BITS;
            f.vtx6->color = 0xffffffff;
            f.vtx6->specular = 0xff000000;
            f.vtx6->u = f.texU + f.du;
            *(int *)&f.vtx6->v = *(int *)&f.texV;
            f.vtx7 = (GfxTlVertex *)(f.batch->vertexCursor + 96);
            f.vtx7->x = (float)penX + f.clipped;
            f.vtx7->y = (float)penY + 24.0f;
            f.vtx7->z = z;
            f.vtx7->rhwBits = FLOAT_ONE_BITS;
            f.vtx7->color = 0xffffffff;
            f.vtx7->specular = 0xff000000;
            f.vtx7->u = f.texU + f.du;
            f.vtx7->v = f.texV + f.dv;
            *(unsigned char *)f.batch->colorIndex = f.colour << 1;
            f.batch->paletteIndex = f.colour << 1;
            g_MenuDrawCharacter = 1;
        }
        if (!g_MenuTextFixedPitch)
            penX += (TEXT_METRIC & 31) << 1;
        else
            penX += 26;
        g_MenuTextRemaining--;
        text++;
        g_MenuTextColumn++;
    }
    return penY;
#undef TEXT_METRIC
}

/*
 * A two-pixel rule from x0 to x1 at y, for the 640x480 layout.
 */
// FUNCTION: FF7 0x006e7b6a
void __cdecl Menu_DrawHorizontalRule2x(int x0, int x1, int y, int colour)
{
    if (Menu_IsTextSubsystemReady() == 0) {
        return;
    }
    Gfx_EmitFlatRect(x0, y, x1 - x0, 2, colour, 0, 0, (int)g_MenuFlatRectGraphicsObject);
}

/*
 * Fill a four-short rectangle from four int arguments. The arguments are int, not
 * short: each value is truncated by the store into the rectangle.
 */
// FUNCTION: FF7 0x006e7ba1
void __cdecl Menu_SetRectShorts(short *rect, int x, int y, int w, int h)
{
    rect[0] = x;
    rect[1] = y;
    rect[2] = w;
    rect[3] = h;
}

/*
 * Move a rectangle by (dx, dy).
 */
// FUNCTION: FF7 0x006e7bd1
void __cdecl Menu_OffsetRectShorts(short *rect, int dx, int dy)
{
    rect[0] += (short)dx;
    rect[1] += (short)dy;
}

/*
 * Copy a four-short rectangle, one field at a time.
 */
// FUNCTION: FF7 0x006e7bf8
void __cdecl Menu_CopyRectShorts(short *dst, short *src)
{
    dst[0] = src[0];
    dst[1] = src[1];
    dst[2] = src[2];
    dst[3] = src[3];
}

/*
 * Keep the four window-corner colours aside.
 */
// FUNCTION: FF7 0x006e7c33
void __cdecl Menu_SaveWindowColors(void)
{
    int i;

    for (i = 0; i < 4; i++) {
        g_BtlMessagePortraitHandles[i] = ((int *)&g_MenuCornerColorTL)[i];
    }
}

/*
 * Put back what Menu_SaveWindowColors kept.
 */
// FUNCTION: FF7 0x006e7c69
void __cdecl Menu_RestoreWindowColors(void)
{
    int i;

    for (i = 0; i < 4; i++) {
        ((int *)&g_MenuCornerColorTL)[i] = g_BtlMessagePortraitHandles[i];
    }
}

/*
 * Load the four window-corner colours from a table, advancing the pointer rather
 * than indexing it.
 */
// FUNCTION: FF7 0x006e7c9f
void __cdecl Menu_SetWindowColors(int *colours)
{
    int i;

    for (i = 0; i < 4; i++) {
        ((int *)&g_MenuCornerColorTL)[i] = *colours;
        colours++;
    }
}

/*
 * Fill a rectangle given as four shorts with a flat colour at depth z.
 */
// FUNCTION: FF7 0x006e7cd9
void __cdecl Menu_FillRectShorts2x(short *rect, unsigned int colour, float z)
{
    if (Menu_IsTextSubsystemReady() == 0) {
        return;
    }
    Gfx_EmitFlatRect(rect[0], rect[1], rect[2], rect[3], colour, 0, z, (int)g_MenuFlatRectGraphicsObject);
}

/*
 * Returns immediately unless the text subsystem is ready, then draws a 2x window
 * from signed-short X/Y/width/height and supplied depth. Independently acquires
 * two textured horizontal edge quads when width exceeds 16, two vertical edges
 * when height exceeds 16, and four 8 by 8 corner quads unconditionally, using the
 * window atlas with white primary/black secondary color, reciprocal W 1 and
 * palette 0.
 */
// FUNCTION: FF7 0x006e7d20
void __cdecl Menu_DrawWindowFrame2x(EnglishWindowFrame *windowFrame, float depth)
{
    int contentHeight;
    GfxTlVertex *bottomEdgeBRPtr;
    GfxTlVertex *bottomEdgeTopRight;
    GfxTlVertex *bottomBottomLeft;
    GfxTlVertex *bottomEdgeTL;
    GfxTlVertex *topBottomRightPtr;
    GfxTlVertex *pRightEdgeBottomRight;
    GfxTlVertex *leftEdgeTRPtr;
    GfxTlVertex *newPTlCornerVertex;
    GfxTlVertex *blCornerTR;
    GfxTlVertex *pCornerBLLower;
    int innerRemainingX;
    GfxTlVertex *newTopEdgeVertex;
    GfxTlVertex *iterTopEdgeBL;
    GfxTlVertex *pTopEdgeTopLeft;
    float texelV;
    GfxTlVertex *v;
    GfxTlVertex *curRemainingEdgeVertex;
    GfxTlVertex *pTlCornerBL;
    GfxTlVertex *blCornerTLPtr;
    GfxTlVertex *bgBRPtr;
    GfxTlVertex *bgTopRight;
    GfxTlVertex *srcBgVert;
    int contentWidth;
    GfxTlVertex *rightEdgeTopRemaining;
    GfxTlVertex *f;
    float cornerU;
    float textureV;
    GfxTlVertex *tmpTrCornerBR;
    GfxTlVertex *blCornerBottomRight;
    GfxTlVertex *tlCornerTL;
    GfxTlVertex *trCornerTopRight;
    GfxTlVertex *bgVertexPtr;
    float uFirstPosition;
    GfxTlVertex *trBottomLeft;
    GfxTlVertex *trCornerTopLeftPtr;
    GfxTlVertex *remainingEdgeTL;
    GfxTlVertex *tlCornerBRPtr;
    GfxTlVertex *blendBgBR;
    GfxTlVertex *blendBgTopRight;
    GfxTlVertex *pBlendBgBL;
    GfxTlVertex *pBlendBgVertex;
    int topInner;
    GfxTlVertex *leftEdgeBR;
    GfxTlVertex *brCornerBR;
    GfxTlVertex *rawBrCornerVertex;
    GfxTlVertex *brBottomLeft;
    GfxTlVertex *brCornerTLPtr;

    if (Menu_IsTextSubsystemReady() == 0) {
        return;
    }
    if (windowFrame->width > 16) {
        if (AcquireBatchNode(2, g_MenuWinCBlend4DiffGraphicsObject) != 0) {
            uFirstPosition = 0.0f;
            textureV = 0.75f;
            cornerU = 0.0625f;
            texelV = 0.03125f;
            pTopEdgeTopLeft = g_MenuWinCBlend4DiffGraphicsObject->vertices;
            pTopEdgeTopLeft->x = (float)windowFrame->x + 8.0f;
            pTopEdgeTopLeft->y = (float)windowFrame->y;
            pTopEdgeTopLeft->z = depth;
            pTopEdgeTopLeft->rhwBits = FLOAT_ONE_BITS;
            pTopEdgeTopLeft->color = 0xFFFFFFFF;
            pTopEdgeTopLeft->specular = 0xFF000000;
            pTopEdgeTopLeft->u = uFirstPosition;
            pTopEdgeTopLeft->v = textureV;
            iterTopEdgeBL = g_MenuWinCBlend4DiffGraphicsObject->vertices + 1;
            iterTopEdgeBL->x = (float)windowFrame->x + 8.0f;
            iterTopEdgeBL->y = (float)windowFrame->y + 8.0f;
            iterTopEdgeBL->z = depth;
            iterTopEdgeBL->rhwBits = FLOAT_ONE_BITS;
            iterTopEdgeBL->color = 0xFFFFFFFF;
            iterTopEdgeBL->specular = 0xFF000000;
            iterTopEdgeBL->u = uFirstPosition;
            iterTopEdgeBL->v = textureV + texelV;
            newTopEdgeVertex = g_MenuWinCBlend4DiffGraphicsObject->vertices + 2;
            newTopEdgeVertex->x = (float)windowFrame->x + 8.0f + windowFrame->width - 16.0f;
            newTopEdgeVertex->y = (float)windowFrame->y;
            newTopEdgeVertex->z = depth;
            newTopEdgeVertex->rhwBits = FLOAT_ONE_BITS;
            newTopEdgeVertex->color = 0xFFFFFFFF;
            newTopEdgeVertex->specular = 0xFF000000;
            newTopEdgeVertex->u = uFirstPosition + cornerU;
            newTopEdgeVertex->v = textureV;
            topBottomRightPtr = g_MenuWinCBlend4DiffGraphicsObject->vertices + 3;
            topBottomRightPtr->x = (float)windowFrame->x + 8.0f + windowFrame->width - 16.0f;
            topBottomRightPtr->y = (float)windowFrame->y + 8.0f;
            topBottomRightPtr->z = depth;
            topBottomRightPtr->rhwBits = FLOAT_ONE_BITS;
            topBottomRightPtr->color = 0xFFFFFFFF;
            topBottomRightPtr->specular = 0xFF000000;
            topBottomRightPtr->u = uFirstPosition + cornerU;
            topBottomRightPtr->v = textureV + texelV;
            *g_MenuWinCBlend4DiffGraphicsObject->colorSignedBytePtr = 0;
            g_MenuWinCBlend4DiffGraphicsObject->paletteIndex = 0;
            g_MenuWinCBlend4DiffGraphicsObject->vertexCursor += g_MenuWinCBlend4DiffGraphicsObject->cursorStep;
            g_MenuWinCBlend4DiffGraphicsObject->colorIndex++;
            uFirstPosition = 0.125f;
            textureV = 0.84375f;
            cornerU = 0.0625f;
            texelV = 0.03125f;
            bottomEdgeTL = g_MenuWinCBlend4DiffGraphicsObject->vertices;
            bottomEdgeTL->x = (float)windowFrame->x + 8.0f;
            bottomEdgeTL->y = (float)windowFrame->y + windowFrame->height - 8.0f;
            bottomEdgeTL->z = depth;
            bottomEdgeTL->rhwBits = FLOAT_ONE_BITS;
            bottomEdgeTL->color = 0xFFFFFFFF;
            bottomEdgeTL->specular = 0xFF000000;
            bottomEdgeTL->u = uFirstPosition;
            bottomEdgeTL->v = textureV;
            bottomBottomLeft = g_MenuWinCBlend4DiffGraphicsObject->vertices + 1;
            bottomBottomLeft->x = (float)windowFrame->x + 8.0f;
            bottomBottomLeft->y = (float)windowFrame->y + windowFrame->height - 8.0f + 8.0f;
            bottomBottomLeft->z = depth;
            bottomBottomLeft->rhwBits = FLOAT_ONE_BITS;
            bottomBottomLeft->color = 0xFFFFFFFF;
            bottomBottomLeft->specular = 0xFF000000;
            bottomBottomLeft->u = uFirstPosition;
            bottomBottomLeft->v = textureV + texelV;
            bottomEdgeTopRight = g_MenuWinCBlend4DiffGraphicsObject->vertices + 2;
            bottomEdgeTopRight->x = (float)windowFrame->x + 8.0f + windowFrame->width - 16.0f;
            bottomEdgeTopRight->y = (float)windowFrame->y + windowFrame->height - 8.0f;
            bottomEdgeTopRight->z = depth;
            bottomEdgeTopRight->rhwBits = FLOAT_ONE_BITS;
            bottomEdgeTopRight->color = 0xFFFFFFFF;
            bottomEdgeTopRight->specular = 0xFF000000;
            bottomEdgeTopRight->u = uFirstPosition + cornerU;
            bottomEdgeTopRight->v = textureV;
            bottomEdgeBRPtr = g_MenuWinCBlend4DiffGraphicsObject->vertices + 3;
            bottomEdgeBRPtr->x = (float)windowFrame->x + 8.0f + windowFrame->width - 16.0f;
            bottomEdgeBRPtr->y = (float)windowFrame->y + windowFrame->height - 8.0f + 8.0f;
            bottomEdgeBRPtr->z = depth;
            bottomEdgeBRPtr->rhwBits = FLOAT_ONE_BITS;
            bottomEdgeBRPtr->color = 0xFFFFFFFF;
            bottomEdgeBRPtr->specular = 0xFF000000;
            bottomEdgeBRPtr->u = uFirstPosition + cornerU;
            bottomEdgeBRPtr->v = textureV + texelV;
            *g_MenuWinCBlend4DiffGraphicsObject->colorSignedBytePtr = 0;
            g_MenuWinCBlend4DiffGraphicsObject->paletteIndex = 0;
        }
    }
    if (windowFrame->height > 16) {
        if (AcquireBatchNode(2, g_MenuWinCBlend4DiffGraphicsObject) != 0) {
            uFirstPosition = 0.21875f;
            textureV = 0.875f;
            cornerU = 0.03125f;
            texelV = 0.0625f;
            rightEdgeTopRemaining = g_MenuWinCBlend4DiffGraphicsObject->vertices;
            rightEdgeTopRemaining->x = (float)windowFrame->x + windowFrame->width - 8.0f;
            rightEdgeTopRemaining->y = (float)windowFrame->y + 8.0f;
            rightEdgeTopRemaining->z = depth;
            rightEdgeTopRemaining->rhwBits = FLOAT_ONE_BITS;
            rightEdgeTopRemaining->color = 0xFFFFFFFF;
            rightEdgeTopRemaining->specular = 0xFF000000;
            rightEdgeTopRemaining->u = uFirstPosition;
            rightEdgeTopRemaining->v = textureV;
            f = g_MenuWinCBlend4DiffGraphicsObject->vertices + 1;
            f->x = (float)windowFrame->x + windowFrame->width - 8.0f;
            f->y = (float)windowFrame->y + 8.0f + windowFrame->height - 16.0f;
            f->z = depth;
            f->rhwBits = FLOAT_ONE_BITS;
            f->color = 0xFFFFFFFF;
            f->specular = 0xFF000000;
            f->u = uFirstPosition;
            f->v = textureV + texelV;
            v = g_MenuWinCBlend4DiffGraphicsObject->vertices + 2;
            v->x = (float)windowFrame->x + windowFrame->width - 8.0f + 8.0f;
            v->y = (float)windowFrame->y + 8.0f;
            v->z = depth;
            v->rhwBits = FLOAT_ONE_BITS;
            v->color = 0xFFFFFFFF;
            v->specular = 0xFF000000;
            v->u = uFirstPosition + cornerU;
            v->v = textureV;
            pRightEdgeBottomRight = g_MenuWinCBlend4DiffGraphicsObject->vertices + 3;
            pRightEdgeBottomRight->x = (float)windowFrame->x + windowFrame->width - 8.0f + 8.0f;
            pRightEdgeBottomRight->y = (float)windowFrame->y + 8.0f + windowFrame->height - 16.0f;
            pRightEdgeBottomRight->z = depth;
            pRightEdgeBottomRight->rhwBits = FLOAT_ONE_BITS;
            pRightEdgeBottomRight->color = 0xFFFFFFFF;
            pRightEdgeBottomRight->specular = 0xFF000000;
            pRightEdgeBottomRight->u = uFirstPosition + cornerU;
            pRightEdgeBottomRight->v = textureV + texelV;
            *g_MenuWinCBlend4DiffGraphicsObject->colorSignedBytePtr = 0;
            g_MenuWinCBlend4DiffGraphicsObject->paletteIndex = 0;
            g_MenuWinCBlend4DiffGraphicsObject->vertexCursor += g_MenuWinCBlend4DiffGraphicsObject->cursorStep;
            g_MenuWinCBlend4DiffGraphicsObject->colorIndex++;
            uFirstPosition = 0.0f;
            textureV = 0.875f;
            cornerU = 0.03125f;
            texelV = 0.0625f;
            remainingEdgeTL = g_MenuWinCBlend4DiffGraphicsObject->vertices;
            remainingEdgeTL->x = (float)windowFrame->x;
            remainingEdgeTL->y = (float)windowFrame->y + 8.0f;
            remainingEdgeTL->z = depth;
            remainingEdgeTL->rhwBits = FLOAT_ONE_BITS;
            remainingEdgeTL->color = 0xFFFFFFFF;
            remainingEdgeTL->specular = 0xFF000000;
            remainingEdgeTL->u = uFirstPosition;
            remainingEdgeTL->v = textureV;
            curRemainingEdgeVertex = g_MenuWinCBlend4DiffGraphicsObject->vertices + 1;
            curRemainingEdgeVertex->x = (float)windowFrame->x;
            curRemainingEdgeVertex->y = (float)windowFrame->y + 8.0f + windowFrame->height - 16.0f;
            curRemainingEdgeVertex->z = depth;
            curRemainingEdgeVertex->rhwBits = FLOAT_ONE_BITS;
            curRemainingEdgeVertex->color = 0xFFFFFFFF;
            curRemainingEdgeVertex->specular = 0xFF000000;
            curRemainingEdgeVertex->u = uFirstPosition;
            curRemainingEdgeVertex->v = textureV + texelV;
            leftEdgeTRPtr = g_MenuWinCBlend4DiffGraphicsObject->vertices + 2;
            leftEdgeTRPtr->x = (float)windowFrame->x + 8.0f;
            leftEdgeTRPtr->y = (float)windowFrame->y + 8.0f;
            leftEdgeTRPtr->z = depth;
            leftEdgeTRPtr->rhwBits = FLOAT_ONE_BITS;
            leftEdgeTRPtr->color = 0xFFFFFFFF;
            leftEdgeTRPtr->specular = 0xFF000000;
            leftEdgeTRPtr->u = uFirstPosition + cornerU;
            leftEdgeTRPtr->v = textureV;
            leftEdgeBR = g_MenuWinCBlend4DiffGraphicsObject->vertices + 3;
            leftEdgeBR->x = (float)windowFrame->x + 8.0f;
            leftEdgeBR->y = (float)windowFrame->y + 8.0f + windowFrame->height - 16.0f;
            leftEdgeBR->z = depth;
            leftEdgeBR->rhwBits = FLOAT_ONE_BITS;
            leftEdgeBR->color = 0xFFFFFFFF;
            leftEdgeBR->specular = 0xFF000000;
            leftEdgeBR->u = uFirstPosition + cornerU;
            leftEdgeBR->v = textureV + texelV;
            *g_MenuWinCBlend4DiffGraphicsObject->colorSignedBytePtr = 0;
            g_MenuWinCBlend4DiffGraphicsObject->paletteIndex = 0;
        }
    }
    if (AcquireBatchNode(4, g_MenuWinCBlend4DiffGraphicsObject) != 0) {
        texelV = 0.03125f;
        cornerU = texelV;
        uFirstPosition = 0.0f;
        textureV = 0.8125f;
        tlCornerTL = g_MenuWinCBlend4DiffGraphicsObject->vertices;
        tlCornerTL->x = (float)windowFrame->x;
        tlCornerTL->y = (float)windowFrame->y;
        tlCornerTL->z = depth;
        tlCornerTL->rhwBits = FLOAT_ONE_BITS;
        tlCornerTL->color = 0xFFFFFFFF;
        tlCornerTL->specular = 0xFF000000;
        tlCornerTL->u = uFirstPosition;
        tlCornerTL->v = textureV;
        pTlCornerBL = g_MenuWinCBlend4DiffGraphicsObject->vertices + 1;
        pTlCornerBL->x = (float)windowFrame->x;
        pTlCornerBL->y = (float)windowFrame->y + 8.0f;
        pTlCornerBL->z = depth;
        pTlCornerBL->rhwBits = FLOAT_ONE_BITS;
        pTlCornerBL->color = 0xFFFFFFFF;
        pTlCornerBL->specular = 0xFF000000;
        pTlCornerBL->u = uFirstPosition;
        pTlCornerBL->v = textureV + texelV;
        newPTlCornerVertex = g_MenuWinCBlend4DiffGraphicsObject->vertices + 2;
        newPTlCornerVertex->x = (float)windowFrame->x + 8.0f;
        newPTlCornerVertex->y = (float)windowFrame->y;
        newPTlCornerVertex->z = depth;
        newPTlCornerVertex->rhwBits = FLOAT_ONE_BITS;
        newPTlCornerVertex->color = 0xFFFFFFFF;
        newPTlCornerVertex->specular = 0xFF000000;
        newPTlCornerVertex->u = uFirstPosition + cornerU;
        newPTlCornerVertex->v = textureV;
        tlCornerBRPtr = g_MenuWinCBlend4DiffGraphicsObject->vertices + 3;
        tlCornerBRPtr->x = (float)windowFrame->x + 8.0f;
        tlCornerBRPtr->y = (float)windowFrame->y + 8.0f;
        tlCornerBRPtr->z = depth;
        tlCornerBRPtr->rhwBits = FLOAT_ONE_BITS;
        tlCornerBRPtr->color = 0xFFFFFFFF;
        tlCornerBRPtr->specular = 0xFF000000;
        tlCornerBRPtr->u = uFirstPosition + cornerU;
        tlCornerBRPtr->v = textureV + texelV;
        *g_MenuWinCBlend4DiffGraphicsObject->colorSignedBytePtr = 0;
        g_MenuWinCBlend4DiffGraphicsObject->paletteIndex = 0;
        g_MenuWinCBlend4DiffGraphicsObject->vertexCursor += g_MenuWinCBlend4DiffGraphicsObject->cursorStep;
        g_MenuWinCBlend4DiffGraphicsObject->colorIndex++;
        uFirstPosition = 0.09375f;
        trCornerTopLeftPtr = g_MenuWinCBlend4DiffGraphicsObject->vertices;
        trCornerTopLeftPtr->x = (float)windowFrame->x + windowFrame->width - 8.0f;
        trCornerTopLeftPtr->y = (float)windowFrame->y;
        trCornerTopLeftPtr->z = depth;
        trCornerTopLeftPtr->rhwBits = FLOAT_ONE_BITS;
        trCornerTopLeftPtr->color = 0xFFFFFFFF;
        trCornerTopLeftPtr->specular = 0xFF000000;
        trCornerTopLeftPtr->u = uFirstPosition;
        trCornerTopLeftPtr->v = textureV;
        trBottomLeft = g_MenuWinCBlend4DiffGraphicsObject->vertices + 1;
        trBottomLeft->x = (float)windowFrame->x + windowFrame->width - 8.0f;
        trBottomLeft->y = (float)windowFrame->y + 8.0f;
        trBottomLeft->z = depth;
        trBottomLeft->rhwBits = FLOAT_ONE_BITS;
        trBottomLeft->color = 0xFFFFFFFF;
        trBottomLeft->specular = 0xFF000000;
        trBottomLeft->u = uFirstPosition;
        trBottomLeft->v = textureV + texelV;
        trCornerTopRight = g_MenuWinCBlend4DiffGraphicsObject->vertices + 2;
        trCornerTopRight->x = (float)windowFrame->x + windowFrame->width - 8.0f + 8.0f;
        trCornerTopRight->y = (float)windowFrame->y;
        trCornerTopRight->z = depth;
        trCornerTopRight->rhwBits = FLOAT_ONE_BITS;
        trCornerTopRight->color = 0xFFFFFFFF;
        trCornerTopRight->specular = 0xFF000000;
        trCornerTopRight->u = uFirstPosition + cornerU;
        trCornerTopRight->v = textureV;
        tmpTrCornerBR = g_MenuWinCBlend4DiffGraphicsObject->vertices + 3;
        tmpTrCornerBR->x = (float)windowFrame->x + windowFrame->width - 8.0f + 8.0f;
        tmpTrCornerBR->y = (float)windowFrame->y + 8.0f;
        tmpTrCornerBR->z = depth;
        tmpTrCornerBR->rhwBits = FLOAT_ONE_BITS;
        tmpTrCornerBR->color = 0xFFFFFFFF;
        tmpTrCornerBR->specular = 0xFF000000;
        tmpTrCornerBR->u = uFirstPosition + cornerU;
        tmpTrCornerBR->v = textureV + texelV;
        *g_MenuWinCBlend4DiffGraphicsObject->colorSignedBytePtr = 0;
        g_MenuWinCBlend4DiffGraphicsObject->paletteIndex = 0;
        g_MenuWinCBlend4DiffGraphicsObject->vertexCursor += g_MenuWinCBlend4DiffGraphicsObject->cursorStep;
        g_MenuWinCBlend4DiffGraphicsObject->colorIndex++;
        uFirstPosition = 0.125f;
        textureV = 0.78125f;
        blCornerTLPtr = g_MenuWinCBlend4DiffGraphicsObject->vertices;
        blCornerTLPtr->x = (float)windowFrame->x;
        blCornerTLPtr->y = (float)windowFrame->y + windowFrame->height - 8.0f;
        blCornerTLPtr->z = depth;
        blCornerTLPtr->rhwBits = FLOAT_ONE_BITS;
        blCornerTLPtr->color = 0xFFFFFFFF;
        blCornerTLPtr->specular = 0xFF000000;
        blCornerTLPtr->u = uFirstPosition;
        blCornerTLPtr->v = textureV;
        pCornerBLLower = g_MenuWinCBlend4DiffGraphicsObject->vertices + 1;
        pCornerBLLower->x = (float)windowFrame->x;
        pCornerBLLower->y = (float)windowFrame->y + windowFrame->height - 8.0f + 8.0f;
        pCornerBLLower->z = depth;
        pCornerBLLower->rhwBits = FLOAT_ONE_BITS;
        pCornerBLLower->color = 0xFFFFFFFF;
        pCornerBLLower->specular = 0xFF000000;
        pCornerBLLower->u = uFirstPosition;
        pCornerBLLower->v = textureV + texelV;
        blCornerTR = g_MenuWinCBlend4DiffGraphicsObject->vertices + 2;
        blCornerTR->x = (float)windowFrame->x + 8.0f;
        blCornerTR->y = (float)windowFrame->y + windowFrame->height - 8.0f;
        blCornerTR->z = depth;
        blCornerTR->rhwBits = FLOAT_ONE_BITS;
        blCornerTR->color = 0xFFFFFFFF;
        blCornerTR->specular = 0xFF000000;
        blCornerTR->u = uFirstPosition + cornerU;
        blCornerTR->v = textureV;
        blCornerBottomRight = g_MenuWinCBlend4DiffGraphicsObject->vertices + 3;
        blCornerBottomRight->x = (float)windowFrame->x + 8.0f;
        blCornerBottomRight->y = (float)windowFrame->y + windowFrame->height - 8.0f + 8.0f;
        blCornerBottomRight->z = depth;
        blCornerBottomRight->rhwBits = FLOAT_ONE_BITS;
        blCornerBottomRight->color = 0xFFFFFFFF;
        blCornerBottomRight->specular = 0xFF000000;
        blCornerBottomRight->u = uFirstPosition + cornerU;
        blCornerBottomRight->v = textureV + texelV;
        *g_MenuWinCBlend4DiffGraphicsObject->colorSignedBytePtr = 0;
        g_MenuWinCBlend4DiffGraphicsObject->paletteIndex = 0;
        g_MenuWinCBlend4DiffGraphicsObject->vertexCursor += g_MenuWinCBlend4DiffGraphicsObject->cursorStep;
        g_MenuWinCBlend4DiffGraphicsObject->colorIndex++;
        uFirstPosition = 0.21875f;
        brCornerTLPtr = g_MenuWinCBlend4DiffGraphicsObject->vertices;
        brCornerTLPtr->x = (float)windowFrame->x + windowFrame->width - 8.0f;
        brCornerTLPtr->y = (float)windowFrame->y + windowFrame->height - 8.0f;
        brCornerTLPtr->z = depth;
        brCornerTLPtr->rhwBits = FLOAT_ONE_BITS;
        brCornerTLPtr->color = 0xFFFFFFFF;
        brCornerTLPtr->specular = 0xFF000000;
        brCornerTLPtr->u = uFirstPosition;
        brCornerTLPtr->v = textureV;
        brBottomLeft = g_MenuWinCBlend4DiffGraphicsObject->vertices + 1;
        brBottomLeft->x = (float)windowFrame->x + windowFrame->width - 8.0f;
        brBottomLeft->y = (float)windowFrame->y + windowFrame->height - 8.0f + 8.0f;
        brBottomLeft->z = depth;
        brBottomLeft->rhwBits = FLOAT_ONE_BITS;
        brBottomLeft->color = 0xFFFFFFFF;
        brBottomLeft->specular = 0xFF000000;
        brBottomLeft->u = uFirstPosition;
        brBottomLeft->v = textureV + texelV;
        rawBrCornerVertex = g_MenuWinCBlend4DiffGraphicsObject->vertices + 2;
        rawBrCornerVertex->x = (float)windowFrame->x + windowFrame->width - 8.0f + 8.0f;
        rawBrCornerVertex->y = (float)windowFrame->y + windowFrame->height - 8.0f;
        rawBrCornerVertex->z = depth;
        rawBrCornerVertex->rhwBits = FLOAT_ONE_BITS;
        rawBrCornerVertex->color = 0xFFFFFFFF;
        rawBrCornerVertex->specular = 0xFF000000;
        rawBrCornerVertex->u = uFirstPosition + cornerU;
        rawBrCornerVertex->v = textureV;
        brCornerBR = g_MenuWinCBlend4DiffGraphicsObject->vertices + 3;
        brCornerBR->x = (float)windowFrame->x + windowFrame->width - 8.0f + 8.0f;
        brCornerBR->y = (float)windowFrame->y + windowFrame->height - 8.0f + 8.0f;
        brCornerBR->z = depth;
        brCornerBR->rhwBits = FLOAT_ONE_BITS;
        brCornerBR->color = 0xFFFFFFFF;
        brCornerBR->specular = 0xFF000000;
        brCornerBR->u = uFirstPosition + cornerU;
        brCornerBR->v = textureV + texelV;
        *g_MenuWinCBlend4DiffGraphicsObject->colorSignedBytePtr = 0;
        g_MenuWinCBlend4DiffGraphicsObject->paletteIndex = 0;
    }
    if (windowFrame->width > 12 && windowFrame->height > 12) {
        innerRemainingX = windowFrame->x + 6;
        topInner = windowFrame->y + 6;
        contentWidth = windowFrame->width - 12;
        contentHeight = windowFrame->height - 12;
        if (g_MenuMessageWindowActive != 0) {
            if (AcquireBatchNode(1, g_MenuBlendWindowBgGraphicsObject) != 0) {
                pBlendBgVertex = g_MenuBlendWindowBgGraphicsObject->vertices;
                pBlendBgVertex->x = (float)innerRemainingX;
                pBlendBgVertex->y = (float)topInner;
                pBlendBgVertex->z = depth + 0.01f;
                pBlendBgVertex->rhwBits = FLOAT_ONE_BITS;
                pBlendBgVertex->color = *(int *)&g_MenuCornerColorTL;
                pBlendBgVertex->specular = 0xFF000000;
                pBlendBgBL = g_MenuBlendWindowBgGraphicsObject->vertices + 1;
                pBlendBgBL->x = (float)innerRemainingX;
                pBlendBgBL->y = (float)topInner + contentHeight;
                pBlendBgBL->z = depth + 0.01f;
                pBlendBgBL->rhwBits = FLOAT_ONE_BITS;
                pBlendBgBL->color = *(int *)&g_MenuCornerColorTR;
                pBlendBgBL->specular = 0xFF000000;
                blendBgTopRight = g_MenuBlendWindowBgGraphicsObject->vertices + 2;
                blendBgTopRight->x = (float)innerRemainingX + contentWidth;
                blendBgTopRight->y = (float)topInner;
                blendBgTopRight->z = depth + 0.01f;
                blendBgTopRight->rhwBits = FLOAT_ONE_BITS;
                blendBgTopRight->color = *(int *)&g_MenuCornerColorBL;
                blendBgTopRight->specular = 0xFF000000;
                blendBgBR = g_MenuBlendWindowBgGraphicsObject->vertices + 3;
                blendBgBR->x = (float)innerRemainingX + contentWidth;
                blendBgBR->y = (float)topInner + contentHeight;
                blendBgBR->z = depth + 0.01f;
                blendBgBR->rhwBits = FLOAT_ONE_BITS;
                blendBgBR->color = *(int *)&g_MenuCornerColorBR;
                blendBgBR->specular = 0xFF000000;
            }
        } else if (AcquireBatchNode(1, g_MenuWindowBgGraphicsObject) != 0) {
            bgVertexPtr = g_MenuWindowBgGraphicsObject->vertices;
            bgVertexPtr->x = (float)innerRemainingX;
            bgVertexPtr->y = (float)topInner;
            bgVertexPtr->z = depth + 0.01f;
            bgVertexPtr->rhwBits = FLOAT_ONE_BITS;
            bgVertexPtr->color = *(int *)&g_MenuCornerColorTL;
            bgVertexPtr->specular = 0xFF000000;
            srcBgVert = g_MenuWindowBgGraphicsObject->vertices + 1;
            srcBgVert->x = (float)innerRemainingX;
            srcBgVert->y = (float)topInner + contentHeight;
            srcBgVert->z = depth + 0.01f;
            srcBgVert->rhwBits = FLOAT_ONE_BITS;
            srcBgVert->color = *(int *)&g_MenuCornerColorTR;
            srcBgVert->specular = 0xFF000000;
            bgTopRight = g_MenuWindowBgGraphicsObject->vertices + 2;
            bgTopRight->x = (float)innerRemainingX + contentWidth;
            bgTopRight->y = (float)topInner;
            bgTopRight->z = depth + 0.01f;
            bgTopRight->rhwBits = FLOAT_ONE_BITS;
            bgTopRight->color = *(int *)&g_MenuCornerColorBL;
            bgTopRight->specular = 0xFF000000;
            bgBRPtr = g_MenuWindowBgGraphicsObject->vertices + 3;
            bgBRPtr->x = (float)innerRemainingX + contentWidth;
            bgBRPtr->y = (float)topInner + contentHeight;
            bgBRPtr->z = depth + 0.01f;
            bgBRPtr->rhwBits = FLOAT_ONE_BITS;
            bgBRPtr->color = *(int *)&g_MenuCornerColorBR;
            bgBRPtr->specular = 0xFF000000;
        }
    }
}

/*
 * When the text subsystem is ready, calls Menu_PackWindowColor for each of four RGB
 * triplets in g_FieldWindowColours into frame-local slots, then emits a 2x
 * dialogue frame from signed-short rectangle fields and supplied depth.
 * Independently acquires two horizontal atlas edges for width above 16, two
 * vertical edges for height above 16, and four 8 by 8 corners unconditionally,
 * using white primary/black secondary colors, reciprocal W 1 and palette 0.
 */
// FUNCTION: FF7 0x006e97e0
void __cdecl Menu_DrawMessageWindowFrame2x(EnglishMessageWindowRect *rect, float depth)
{
    int colourOfs;
    int bodyTop;
    float vBase;
    GfxTlVertex *bottomEdgeRightPtr;
    GfxTlVertex *outerBottomPointer;
    GfxTlVertex *pRightEdgeTop;
    GfxTlVertex *v;
    GfxTlVertex *innerTopLeftAltPtr;
    GfxTlVertex *pOuterTopEdgeLeft;
    GfxTlVertex *localOuterBottomRight;
    GfxTlVertex *pRightOuterBottomTwo;
    int idx;
    int bodyLeft;
    GfxTlVertex *bottomEdgeStart;
    GfxTlVertex *pRightTopInner;
    GfxTlVertex *pTopEdgeRightVtx;
    GfxTlVertex *f;
    GfxTlVertex *pOuterLeftTop;
    GfxTlVertex *outerTopRemaining;
    GfxTlVertex *bottomRightBPtr;
    GfxTlVertex *pBottomRightCorner;
    GfxTlVertex *bgCornerBottomRightPtr;
    GfxTlVertex *bgTopRightPtr;
    int contentSpanY;
    GfxTlVertex *pRightBottomEdge;
    GfxTlVertex *calcPInnerLeftBottom;
    GfxTlVertex *innerTopCorner;
    GfxTlVertex *topLeftEdge;
    GfxTlVertex *outerBottomRemainingTwo;
    GfxTlVertex *innerRemainingBottom;
    GfxTlVertex *innerRemainingTop;
    GfxTlVertex *pRightEdgeTopInner;
    GfxTlVertex *bgCornerBL;
    float vIncrement;
    GfxTlVertex *outerRemainingBottom;
    GfxTlVertex *remainingEdgeLowerTwo;
    GfxTlVertex *leftEdgeLower;
    GfxTlVertex *topRightBase;
    GfxTlVertex *bgVertexTopRemaining;
    float nextZBiasTwo;
    GfxTlVertex *rightOuterTopTwo;
    GfxTlVertex *bgBottomRightBlend;
    int innerExtentX;
    GfxTlVertex *leftEdgeTop;
    GfxTlVertex *pBlendTopRight;
    GfxTlVertex *pBgBlendLowLeft;
    GfxTlVertex *topRightOuterPtr;
    GfxTlVertex *bgTopLeftBPtr;
    int colourBuffer[4];
    float uSpan;
    GfxTlVertex *outerLowRightEndPtr;
    float zBias;
    float uRemainingOrigin;

    if (Menu_IsTextSubsystemReady() == 0) {
        return;
    }
    for (idx = 0; idx < 4; idx++) {
        colourOfs = idx * 3;
        Menu_PackWindowColor((unsigned char *)((char *)colourBuffer + idx * 4), g_FieldWindowColours[colourOfs], g_FieldWindowColours[colourOfs + 1], g_FieldWindowColours[colourOfs + 2]);
    }
    if (rect->width > 16) {
        if (AcquireBatchNode(2, g_MenuWinCBlend4DiffGraphicsObject) != 0) {
            uRemainingOrigin = 0.0f;
            vBase = 0.75f;
            uSpan = 0.0625f;
            vIncrement = 0.03125f;
            topLeftEdge = g_MenuWinCBlend4DiffGraphicsObject->vertices;
            topLeftEdge->x = (float)rect->posX + 8.0f;
            topLeftEdge->y = (float)rect->posY;
            topLeftEdge->z = depth;
            topLeftEdge->rhwBits = FLOAT_ONE_BITS;
            topLeftEdge->color = 0xFFFFFFFF;
            topLeftEdge->specular = 0xFF000000;
            topLeftEdge->u = uRemainingOrigin;
            topLeftEdge->v = vBase;
            innerTopCorner = g_MenuWinCBlend4DiffGraphicsObject->vertices + 1;
            innerTopCorner->x = (float)rect->posX + 8.0f;
            innerTopCorner->y = (float)rect->posY + 8.0f;
            innerTopCorner->z = depth;
            innerTopCorner->rhwBits = FLOAT_ONE_BITS;
            innerTopCorner->color = 0xFFFFFFFF;
            innerTopCorner->specular = 0xFF000000;
            innerTopCorner->u = uRemainingOrigin;
            innerTopCorner->v = vBase + vIncrement;
            pTopEdgeRightVtx = g_MenuWinCBlend4DiffGraphicsObject->vertices + 2;
            pTopEdgeRightVtx->x = (float)rect->posX + 8.0f + rect->width - 16.0f;
            pTopEdgeRightVtx->y = (float)rect->posY;
            pTopEdgeRightVtx->z = depth;
            pTopEdgeRightVtx->rhwBits = FLOAT_ONE_BITS;
            pTopEdgeRightVtx->color = 0xFFFFFFFF;
            pTopEdgeRightVtx->specular = 0xFF000000;
            pTopEdgeRightVtx->u = uRemainingOrigin + uSpan;
            pTopEdgeRightVtx->v = vBase;
            pRightTopInner = g_MenuWinCBlend4DiffGraphicsObject->vertices + 3;
            pRightTopInner->x = (float)rect->posX + 8.0f + rect->width - 16.0f;
            pRightTopInner->y = (float)rect->posY + 8.0f;
            pRightTopInner->z = depth;
            pRightTopInner->rhwBits = FLOAT_ONE_BITS;
            pRightTopInner->color = 0xFFFFFFFF;
            pRightTopInner->specular = 0xFF000000;
            pRightTopInner->u = uRemainingOrigin + uSpan;
            pRightTopInner->v = vBase + vIncrement;
            *g_MenuWinCBlend4DiffGraphicsObject->colorSignedBytePtr = 0;
            g_MenuWinCBlend4DiffGraphicsObject->paletteIndex = 0;
            g_MenuWinCBlend4DiffGraphicsObject->vertexCursor += g_MenuWinCBlend4DiffGraphicsObject->cursorStep;
            g_MenuWinCBlend4DiffGraphicsObject->colorIndex++;
            uRemainingOrigin = 0.125f;
            vBase = 0.84375f;
            uSpan = 0.0625f;
            vIncrement = 0.03125f;
            bottomEdgeStart = g_MenuWinCBlend4DiffGraphicsObject->vertices;
            bottomEdgeStart->x = (float)rect->posX + 8.0f;
            bottomEdgeStart->y = (float)rect->posY + rect->height - 8.0f;
            bottomEdgeStart->z = depth;
            bottomEdgeStart->rhwBits = FLOAT_ONE_BITS;
            bottomEdgeStart->color = 0xFFFFFFFF;
            bottomEdgeStart->specular = 0xFF000000;
            bottomEdgeStart->u = uRemainingOrigin;
            bottomEdgeStart->v = vBase;
            outerBottomPointer = g_MenuWinCBlend4DiffGraphicsObject->vertices + 1;
            outerBottomPointer->x = (float)rect->posX + 8.0f;
            outerBottomPointer->y = (float)rect->posY + rect->height - 8.0f + 8.0f;
            outerBottomPointer->z = depth;
            outerBottomPointer->rhwBits = FLOAT_ONE_BITS;
            outerBottomPointer->color = 0xFFFFFFFF;
            outerBottomPointer->specular = 0xFF000000;
            outerBottomPointer->u = uRemainingOrigin;
            outerBottomPointer->v = vBase + vIncrement;
            bottomEdgeRightPtr = g_MenuWinCBlend4DiffGraphicsObject->vertices + 2;
            bottomEdgeRightPtr->x = (float)rect->posX + 8.0f + rect->width - 16.0f;
            bottomEdgeRightPtr->y = (float)rect->posY + rect->height - 8.0f;
            bottomEdgeRightPtr->z = depth;
            bottomEdgeRightPtr->rhwBits = FLOAT_ONE_BITS;
            bottomEdgeRightPtr->color = 0xFFFFFFFF;
            bottomEdgeRightPtr->specular = 0xFF000000;
            bottomEdgeRightPtr->u = uRemainingOrigin + uSpan;
            bottomEdgeRightPtr->v = vBase;
            outerLowRightEndPtr = g_MenuWinCBlend4DiffGraphicsObject->vertices + 3;
            outerLowRightEndPtr->x = (float)rect->posX + 8.0f + rect->width - 16.0f;
            outerLowRightEndPtr->y = (float)rect->posY + rect->height - 8.0f + 8.0f;
            outerLowRightEndPtr->z = depth;
            outerLowRightEndPtr->rhwBits = FLOAT_ONE_BITS;
            outerLowRightEndPtr->color = 0xFFFFFFFF;
            outerLowRightEndPtr->specular = 0xFF000000;
            outerLowRightEndPtr->u = uRemainingOrigin + uSpan;
            outerLowRightEndPtr->v = vBase + vIncrement;
            *g_MenuWinCBlend4DiffGraphicsObject->colorSignedBytePtr = 0;
            g_MenuWinCBlend4DiffGraphicsObject->paletteIndex = 0;
        }
    }
    if (rect->height > 16) {
        if (AcquireBatchNode(2, g_MenuWinCBlend4DiffGraphicsObject) != 0) {
            uRemainingOrigin = 0.21875f;
            vBase = 0.875f;
            uSpan = 0.03125f;
            vIncrement = 0.0625f;
            pRightEdgeTop = g_MenuWinCBlend4DiffGraphicsObject->vertices;
            pRightEdgeTop->x = (float)rect->posX + rect->width - 8.0f;
            pRightEdgeTop->y = (float)rect->posY + 8.0f;
            pRightEdgeTop->z = depth;
            pRightEdgeTop->rhwBits = FLOAT_ONE_BITS;
            pRightEdgeTop->color = 0xFFFFFFFF;
            pRightEdgeTop->specular = 0xFF000000;
            pRightEdgeTop->u = uRemainingOrigin;
            pRightEdgeTop->v = vBase;
            pRightBottomEdge = g_MenuWinCBlend4DiffGraphicsObject->vertices + 1;
            pRightBottomEdge->x = (float)rect->posX + rect->width - 8.0f;
            pRightBottomEdge->y = (float)rect->posY + 8.0f + rect->height - 16.0f;
            pRightBottomEdge->z = depth;
            pRightBottomEdge->rhwBits = FLOAT_ONE_BITS;
            pRightBottomEdge->color = 0xFFFFFFFF;
            pRightBottomEdge->specular = 0xFF000000;
            pRightBottomEdge->u = uRemainingOrigin;
            pRightBottomEdge->v = vBase + vIncrement;
            f = g_MenuWinCBlend4DiffGraphicsObject->vertices + 2;
            f->x = (float)rect->posX + rect->width - 8.0f + 8.0f;
            f->y = (float)rect->posY + 8.0f;
            f->z = depth;
            f->rhwBits = FLOAT_ONE_BITS;
            f->color = 0xFFFFFFFF;
            f->specular = 0xFF000000;
            f->u = uRemainingOrigin + uSpan;
            f->v = vBase;
            v = g_MenuWinCBlend4DiffGraphicsObject->vertices + 3;
            v->x = (float)rect->posX + rect->width - 8.0f + 8.0f;
            v->y = (float)rect->posY + 8.0f + rect->height - 16.0f;
            v->z = depth;
            v->rhwBits = FLOAT_ONE_BITS;
            v->color = 0xFFFFFFFF;
            v->specular = 0xFF000000;
            v->u = uRemainingOrigin + uSpan;
            v->v = vBase + vIncrement;
            *g_MenuWinCBlend4DiffGraphicsObject->colorSignedBytePtr = 0;
            g_MenuWinCBlend4DiffGraphicsObject->paletteIndex = 0;
            g_MenuWinCBlend4DiffGraphicsObject->vertexCursor += g_MenuWinCBlend4DiffGraphicsObject->cursorStep;
            g_MenuWinCBlend4DiffGraphicsObject->colorIndex++;
            uRemainingOrigin = 0.0f;
            vBase = 0.875f;
            uSpan = 0.03125f;
            vIncrement = 0.0625f;
            leftEdgeTop = g_MenuWinCBlend4DiffGraphicsObject->vertices;
            leftEdgeTop->x = (float)rect->posX;
            leftEdgeTop->y = (float)rect->posY + 8.0f;
            leftEdgeTop->z = depth;
            leftEdgeTop->rhwBits = FLOAT_ONE_BITS;
            leftEdgeTop->color = 0xFFFFFFFF;
            leftEdgeTop->specular = 0xFF000000;
            leftEdgeTop->u = uRemainingOrigin;
            leftEdgeTop->v = vBase;
            leftEdgeLower = g_MenuWinCBlend4DiffGraphicsObject->vertices + 1;
            leftEdgeLower->x = (float)rect->posX;
            leftEdgeLower->y = (float)rect->posY + 8.0f + rect->height - 16.0f;
            leftEdgeLower->z = depth;
            leftEdgeLower->rhwBits = FLOAT_ONE_BITS;
            leftEdgeLower->color = 0xFFFFFFFF;
            leftEdgeLower->specular = 0xFF000000;
            leftEdgeLower->u = uRemainingOrigin;
            leftEdgeLower->v = vBase + vIncrement;
            innerRemainingTop = g_MenuWinCBlend4DiffGraphicsObject->vertices + 2;
            innerRemainingTop->x = (float)rect->posX + 8.0f;
            innerRemainingTop->y = (float)rect->posY + 8.0f;
            innerRemainingTop->z = depth;
            innerRemainingTop->rhwBits = FLOAT_ONE_BITS;
            innerRemainingTop->color = 0xFFFFFFFF;
            innerRemainingTop->specular = 0xFF000000;
            innerRemainingTop->u = uRemainingOrigin + uSpan;
            innerRemainingTop->v = vBase;
            calcPInnerLeftBottom = g_MenuWinCBlend4DiffGraphicsObject->vertices + 3;
            calcPInnerLeftBottom->x = (float)rect->posX + 8.0f;
            calcPInnerLeftBottom->y = (float)rect->posY + 8.0f + rect->height - 16.0f;
            calcPInnerLeftBottom->z = depth;
            calcPInnerLeftBottom->rhwBits = FLOAT_ONE_BITS;
            calcPInnerLeftBottom->color = 0xFFFFFFFF;
            calcPInnerLeftBottom->specular = 0xFF000000;
            calcPInnerLeftBottom->u = uRemainingOrigin + uSpan;
            calcPInnerLeftBottom->v = vBase + vIncrement;
            *g_MenuWinCBlend4DiffGraphicsObject->colorSignedBytePtr = 0;
            g_MenuWinCBlend4DiffGraphicsObject->paletteIndex = 0;
        }
    }
    if (AcquireBatchNode(4, g_MenuWinCBlend4DiffGraphicsObject) != 0) {
        vIncrement = 0.03125f;
        uSpan = vIncrement;
        uRemainingOrigin = 0.0f;
        vBase = 0.8125f;
        outerTopRemaining = g_MenuWinCBlend4DiffGraphicsObject->vertices;
        outerTopRemaining->x = (float)rect->posX;
        outerTopRemaining->y = (float)rect->posY;
        outerTopRemaining->z = depth;
        outerTopRemaining->rhwBits = FLOAT_ONE_BITS;
        outerTopRemaining->color = 0xFFFFFFFF;
        outerTopRemaining->specular = 0xFF000000;
        outerTopRemaining->u = uRemainingOrigin;
        outerTopRemaining->v = vBase;
        pOuterLeftTop = g_MenuWinCBlend4DiffGraphicsObject->vertices + 1;
        pOuterLeftTop->x = (float)rect->posX;
        pOuterLeftTop->y = (float)rect->posY + 8.0f;
        pOuterLeftTop->z = depth;
        pOuterLeftTop->rhwBits = FLOAT_ONE_BITS;
        pOuterLeftTop->color = 0xFFFFFFFF;
        pOuterLeftTop->specular = 0xFF000000;
        pOuterLeftTop->u = uRemainingOrigin;
        pOuterLeftTop->v = vBase + vIncrement;
        pOuterTopEdgeLeft = g_MenuWinCBlend4DiffGraphicsObject->vertices + 2;
        pOuterTopEdgeLeft->x = (float)rect->posX + 8.0f;
        pOuterTopEdgeLeft->y = (float)rect->posY;
        pOuterTopEdgeLeft->z = depth;
        pOuterTopEdgeLeft->rhwBits = FLOAT_ONE_BITS;
        pOuterTopEdgeLeft->color = 0xFFFFFFFF;
        pOuterTopEdgeLeft->specular = 0xFF000000;
        pOuterTopEdgeLeft->u = uRemainingOrigin + uSpan;
        pOuterTopEdgeLeft->v = vBase;
        innerTopLeftAltPtr = g_MenuWinCBlend4DiffGraphicsObject->vertices + 3;
        innerTopLeftAltPtr->x = (float)rect->posX + 8.0f;
        innerTopLeftAltPtr->y = (float)rect->posY + 8.0f;
        innerTopLeftAltPtr->z = depth;
        innerTopLeftAltPtr->rhwBits = FLOAT_ONE_BITS;
        innerTopLeftAltPtr->color = 0xFFFFFFFF;
        innerTopLeftAltPtr->specular = 0xFF000000;
        innerTopLeftAltPtr->u = uRemainingOrigin + uSpan;
        innerTopLeftAltPtr->v = vBase + vIncrement;
        *g_MenuWinCBlend4DiffGraphicsObject->colorSignedBytePtr = 0;
        g_MenuWinCBlend4DiffGraphicsObject->paletteIndex = 0;
        g_MenuWinCBlend4DiffGraphicsObject->vertexCursor += g_MenuWinCBlend4DiffGraphicsObject->cursorStep;
        g_MenuWinCBlend4DiffGraphicsObject->colorIndex++;
        uRemainingOrigin = 0.09375f;
        topRightBase = g_MenuWinCBlend4DiffGraphicsObject->vertices;
        topRightBase->x = (float)rect->posX + rect->width - 8.0f;
        topRightBase->y = (float)rect->posY;
        topRightBase->z = depth;
        topRightBase->rhwBits = FLOAT_ONE_BITS;
        topRightBase->color = 0xFFFFFFFF;
        topRightBase->specular = 0xFF000000;
        topRightBase->u = uRemainingOrigin;
        topRightBase->v = vBase;
        pRightEdgeTopInner = g_MenuWinCBlend4DiffGraphicsObject->vertices + 1;
        pRightEdgeTopInner->x = (float)rect->posX + rect->width - 8.0f;
        pRightEdgeTopInner->y = (float)rect->posY + 8.0f;
        pRightEdgeTopInner->z = depth;
        pRightEdgeTopInner->rhwBits = FLOAT_ONE_BITS;
        pRightEdgeTopInner->color = 0xFFFFFFFF;
        pRightEdgeTopInner->specular = 0xFF000000;
        pRightEdgeTopInner->u = uRemainingOrigin;
        pRightEdgeTopInner->v = vBase + vIncrement;
        topRightOuterPtr = g_MenuWinCBlend4DiffGraphicsObject->vertices + 2;
        topRightOuterPtr->x = (float)rect->posX + rect->width - 8.0f + 8.0f;
        topRightOuterPtr->y = (float)rect->posY;
        topRightOuterPtr->z = depth;
        topRightOuterPtr->rhwBits = FLOAT_ONE_BITS;
        topRightOuterPtr->color = 0xFFFFFFFF;
        topRightOuterPtr->specular = 0xFF000000;
        topRightOuterPtr->u = uRemainingOrigin + uSpan;
        topRightOuterPtr->v = vBase;
        rightOuterTopTwo = g_MenuWinCBlend4DiffGraphicsObject->vertices + 3;
        rightOuterTopTwo->x = (float)rect->posX + rect->width - 8.0f + 8.0f;
        rightOuterTopTwo->y = (float)rect->posY + 8.0f;
        rightOuterTopTwo->z = depth;
        rightOuterTopTwo->rhwBits = FLOAT_ONE_BITS;
        rightOuterTopTwo->color = 0xFFFFFFFF;
        rightOuterTopTwo->specular = 0xFF000000;
        rightOuterTopTwo->u = uRemainingOrigin + uSpan;
        rightOuterTopTwo->v = vBase + vIncrement;
        *g_MenuWinCBlend4DiffGraphicsObject->colorSignedBytePtr = 0;
        g_MenuWinCBlend4DiffGraphicsObject->paletteIndex = 0;
        g_MenuWinCBlend4DiffGraphicsObject->vertexCursor += g_MenuWinCBlend4DiffGraphicsObject->cursorStep;
        g_MenuWinCBlend4DiffGraphicsObject->colorIndex++;
        uRemainingOrigin = 0.125f;
        vBase = 0.78125f;
        remainingEdgeLowerTwo = g_MenuWinCBlend4DiffGraphicsObject->vertices;
        remainingEdgeLowerTwo->x = (float)rect->posX;
        remainingEdgeLowerTwo->y = (float)rect->posY + rect->height - 8.0f;
        remainingEdgeLowerTwo->z = depth;
        remainingEdgeLowerTwo->rhwBits = FLOAT_ONE_BITS;
        remainingEdgeLowerTwo->color = 0xFFFFFFFF;
        remainingEdgeLowerTwo->specular = 0xFF000000;
        remainingEdgeLowerTwo->u = uRemainingOrigin;
        remainingEdgeLowerTwo->v = vBase;
        outerRemainingBottom = g_MenuWinCBlend4DiffGraphicsObject->vertices + 1;
        outerRemainingBottom->x = (float)rect->posX;
        outerRemainingBottom->y = (float)rect->posY + rect->height - 8.0f + 8.0f;
        outerRemainingBottom->z = depth;
        outerRemainingBottom->rhwBits = FLOAT_ONE_BITS;
        outerRemainingBottom->color = 0xFFFFFFFF;
        outerRemainingBottom->specular = 0xFF000000;
        outerRemainingBottom->u = uRemainingOrigin;
        outerRemainingBottom->v = vBase + vIncrement;
        innerRemainingBottom = g_MenuWinCBlend4DiffGraphicsObject->vertices + 2;
        innerRemainingBottom->x = (float)rect->posX + 8.0f;
        innerRemainingBottom->y = (float)rect->posY + rect->height - 8.0f;
        innerRemainingBottom->z = depth;
        innerRemainingBottom->rhwBits = FLOAT_ONE_BITS;
        innerRemainingBottom->color = 0xFFFFFFFF;
        innerRemainingBottom->specular = 0xFF000000;
        innerRemainingBottom->u = uRemainingOrigin + uSpan;
        innerRemainingBottom->v = vBase;
        outerBottomRemainingTwo = g_MenuWinCBlend4DiffGraphicsObject->vertices + 3;
        outerBottomRemainingTwo->x = (float)rect->posX + 8.0f;
        outerBottomRemainingTwo->y = (float)rect->posY + rect->height - 8.0f + 8.0f;
        outerBottomRemainingTwo->z = depth;
        outerBottomRemainingTwo->rhwBits = FLOAT_ONE_BITS;
        outerBottomRemainingTwo->color = 0xFFFFFFFF;
        outerBottomRemainingTwo->specular = 0xFF000000;
        outerBottomRemainingTwo->u = uRemainingOrigin + uSpan;
        outerBottomRemainingTwo->v = vBase + vIncrement;
        *g_MenuWinCBlend4DiffGraphicsObject->colorSignedBytePtr = 0;
        g_MenuWinCBlend4DiffGraphicsObject->paletteIndex = 0;
        g_MenuWinCBlend4DiffGraphicsObject->vertexCursor += g_MenuWinCBlend4DiffGraphicsObject->cursorStep;
        g_MenuWinCBlend4DiffGraphicsObject->colorIndex++;
        uRemainingOrigin = 0.21875f;
        pBottomRightCorner = g_MenuWinCBlend4DiffGraphicsObject->vertices;
        pBottomRightCorner->x = (float)rect->posX + rect->width - 8.0f;
        pBottomRightCorner->y = (float)rect->posY + rect->height - 8.0f;
        pBottomRightCorner->z = depth;
        pBottomRightCorner->rhwBits = FLOAT_ONE_BITS;
        pBottomRightCorner->color = 0xFFFFFFFF;
        pBottomRightCorner->specular = 0xFF000000;
        pBottomRightCorner->u = uRemainingOrigin;
        pBottomRightCorner->v = vBase;
        bottomRightBPtr = g_MenuWinCBlend4DiffGraphicsObject->vertices + 1;
        bottomRightBPtr->x = (float)rect->posX + rect->width - 8.0f;
        bottomRightBPtr->y = (float)rect->posY + rect->height - 8.0f + 8.0f;
        bottomRightBPtr->z = depth;
        bottomRightBPtr->rhwBits = FLOAT_ONE_BITS;
        bottomRightBPtr->color = 0xFFFFFFFF;
        bottomRightBPtr->specular = 0xFF000000;
        bottomRightBPtr->u = uRemainingOrigin;
        bottomRightBPtr->v = vBase + vIncrement;
        pRightOuterBottomTwo = g_MenuWinCBlend4DiffGraphicsObject->vertices + 2;
        pRightOuterBottomTwo->x = (float)rect->posX + rect->width - 8.0f + 8.0f;
        pRightOuterBottomTwo->y = (float)rect->posY + rect->height - 8.0f;
        pRightOuterBottomTwo->z = depth;
        pRightOuterBottomTwo->rhwBits = FLOAT_ONE_BITS;
        pRightOuterBottomTwo->color = 0xFFFFFFFF;
        pRightOuterBottomTwo->specular = 0xFF000000;
        pRightOuterBottomTwo->u = uRemainingOrigin + uSpan;
        pRightOuterBottomTwo->v = vBase;
        localOuterBottomRight = g_MenuWinCBlend4DiffGraphicsObject->vertices + 3;
        localOuterBottomRight->x = (float)rect->posX + rect->width - 8.0f + 8.0f;
        localOuterBottomRight->y = (float)rect->posY + rect->height - 8.0f + 8.0f;
        localOuterBottomRight->z = depth;
        localOuterBottomRight->rhwBits = FLOAT_ONE_BITS;
        localOuterBottomRight->color = 0xFFFFFFFF;
        localOuterBottomRight->specular = 0xFF000000;
        localOuterBottomRight->u = uRemainingOrigin + uSpan;
        localOuterBottomRight->v = vBase + vIncrement;
        *g_MenuWinCBlend4DiffGraphicsObject->colorSignedBytePtr = 0;
        g_MenuWinCBlend4DiffGraphicsObject->paletteIndex = 0;
    }
    if (rect->width > 12 && rect->height > 12) {
        bodyLeft = rect->posX + 6;
        bodyTop = rect->posY + 6;
        innerExtentX = rect->width - 12;
        contentSpanY = rect->height - 12;
        if (g_MenuMessageWindowActive != 0) {
            if (AcquireBatchNode(1, g_MenuBlendWindowBgGraphicsObject) != 0) {
                zBias = 1e-06f;
                bgTopLeftBPtr = g_MenuBlendWindowBgGraphicsObject->vertices;
                bgTopLeftBPtr->x = (float)bodyLeft;
                bgTopLeftBPtr->y = (float)bodyTop;
                bgTopLeftBPtr->z = depth + zBias;
                bgTopLeftBPtr->rhwBits = FLOAT_ONE_BITS;
                bgTopLeftBPtr->color = *(int *)&g_MenuCornerColorTL;
                bgTopLeftBPtr->specular = 0xFF000000;
                pBgBlendLowLeft = g_MenuBlendWindowBgGraphicsObject->vertices + 1;
                pBgBlendLowLeft->x = (float)bodyLeft;
                pBgBlendLowLeft->y = (float)bodyTop + contentSpanY;
                pBgBlendLowLeft->z = depth + zBias;
                pBgBlendLowLeft->rhwBits = FLOAT_ONE_BITS;
                pBgBlendLowLeft->color = *(int *)&g_MenuCornerColorTR;
                pBgBlendLowLeft->specular = 0xFF000000;
                pBlendTopRight = g_MenuBlendWindowBgGraphicsObject->vertices + 2;
                pBlendTopRight->x = (float)bodyLeft + innerExtentX;
                pBlendTopRight->y = (float)bodyTop;
                pBlendTopRight->z = depth + zBias;
                pBlendTopRight->rhwBits = FLOAT_ONE_BITS;
                pBlendTopRight->color = *(int *)&g_MenuCornerColorBL;
                pBlendTopRight->specular = 0xFF000000;
                bgBottomRightBlend = g_MenuBlendWindowBgGraphicsObject->vertices + 3;
                bgBottomRightBlend->x = (float)bodyLeft + innerExtentX;
                bgBottomRightBlend->y = (float)bodyTop + contentSpanY;
                bgBottomRightBlend->z = depth + zBias;
                bgBottomRightBlend->rhwBits = FLOAT_ONE_BITS;
                bgBottomRightBlend->color = *(int *)&g_MenuCornerColorBR;
                bgBottomRightBlend->specular = 0xFF000000;
                g_MenuBlendWindowBgPending = 1;
            }
        } else {
            if (AcquireBatchNode(1, g_MenuWindowBgGraphicsObject) != 0) {
                nextZBiasTwo = 1e-06f;
                bgVertexTopRemaining = g_MenuWindowBgGraphicsObject->vertices;
                bgVertexTopRemaining->x = (float)bodyLeft;
                bgVertexTopRemaining->y = (float)bodyTop;
                bgVertexTopRemaining->z = depth + nextZBiasTwo;
                bgVertexTopRemaining->rhwBits = FLOAT_ONE_BITS;
                bgVertexTopRemaining->color = *(int *)&g_MenuCornerColorTL;
                bgVertexTopRemaining->specular = 0xFF000000;
                bgCornerBL = g_MenuWindowBgGraphicsObject->vertices + 1;
                bgCornerBL->x = (float)bodyLeft;
                bgCornerBL->y = (float)bodyTop + contentSpanY;
                bgCornerBL->z = depth + nextZBiasTwo;
                bgCornerBL->rhwBits = FLOAT_ONE_BITS;
                bgCornerBL->color = *(int *)&g_MenuCornerColorTR;
                bgCornerBL->specular = 0xFF000000;
                bgTopRightPtr = g_MenuWindowBgGraphicsObject->vertices + 2;
                bgTopRightPtr->x = (float)bodyLeft + innerExtentX;
                bgTopRightPtr->y = (float)bodyTop;
                bgTopRightPtr->z = depth + nextZBiasTwo;
                bgTopRightPtr->rhwBits = FLOAT_ONE_BITS;
                bgTopRightPtr->color = *(int *)&g_MenuCornerColorBL;
                bgTopRightPtr->specular = 0xFF000000;
                bgCornerBottomRightPtr = g_MenuWindowBgGraphicsObject->vertices + 3;
                bgCornerBottomRightPtr->x = (float)bodyLeft + innerExtentX;
                bgCornerBottomRightPtr->y = (float)bodyTop + contentSpanY;
                bgCornerBottomRightPtr->z = depth + nextZBiasTwo;
                bgCornerBottomRightPtr->rhwBits = FLOAT_ONE_BITS;
                bgCornerBottomRightPtr->color = *(int *)&g_MenuCornerColorBR;
                bgCornerBottomRightPtr->specular = 0xFF000000;
            }
            g_MenuWindowBgPending = 1;
        }
    }
}

/*
 * The "more text" arrow of the 640x480 message window: a 48x26 quad from the arrow
 * batch, then a 48x8 half-transparent shadow quad 24 pixels lower from the shadow
 * batch, which also marks the pass-1 list dirty. Both quads set their batch's
 * dirty byte and count to 1.
 */
// FUNCTION: FF7 0x006eb3b8
void __cdecl Menu_DrawMoreTextArrow2x(int left, int top, float depth)
{
    float vStart;
    float uSpan;
    float texHeight;
    GfxTlVertex *pATL;
    GfxTlVertex *v;
    GfxTlVertex *subPArrowVtx;
    GfxTlVertex *arrowBL;
    GfxTlVertex *pShadowBL;
    GfxTlVertex *shadowTopLeftPtr;
    GfxTlVertex *pShadowTR;
    GfxTlVertex *shadowBR;
    float curUStartScalar;

    if (Menu_IsTextSubsystemReady() == 0) {
        return;
    }
    if (AcquireBatchNode(1, g_MenuWinBBlend4GraphicsObject) != 0) {
        curUStartScalar = 0.75f;
        vStart = 0.0625f;
        uSpan = 0.1875f;
        texHeight = 0.1015625f;
        pATL = g_MenuWinBBlend4GraphicsObject->vertices;
        pATL->x = left;
        pATL->y = top;
        pATL->z = depth;
        pATL->rhwBits = FLOAT_ONE_BITS;
        pATL->color = 0xFFFFFFFF;
        pATL->specular = 0xFF000000;
        pATL->u = curUStartScalar;
        pATL->v = vStart;
        arrowBL = g_MenuWinBBlend4GraphicsObject->vertices + 1;
        arrowBL->x = left;
        arrowBL->y = top + 26.0f;
        arrowBL->z = depth;
        arrowBL->rhwBits = FLOAT_ONE_BITS;
        arrowBL->color = 0xFFFFFFFF;
        arrowBL->specular = 0xFF000000;
        arrowBL->u = curUStartScalar;
        arrowBL->v = vStart + texHeight;
        subPArrowVtx = g_MenuWinBBlend4GraphicsObject->vertices + 2;
        subPArrowVtx->x = left + 48.0f;
        subPArrowVtx->y = top;
        subPArrowVtx->z = depth;
        subPArrowVtx->rhwBits = FLOAT_ONE_BITS;
        subPArrowVtx->color = 0xFFFFFFFF;
        subPArrowVtx->specular = 0xFF000000;
        subPArrowVtx->u = curUStartScalar + uSpan;
        subPArrowVtx->v = vStart;
        v = g_MenuWinBBlend4GraphicsObject->vertices + 3;
        v->x = left + 48.0f;
        v->y = top + 26.0f;
        v->z = depth;
        v->rhwBits = FLOAT_ONE_BITS;
        v->color = 0xFFFFFFFF;
        v->specular = 0xFF000000;
        v->u = curUStartScalar + uSpan;
        v->v = vStart + texHeight;
        *(char *)g_MenuWinBBlend4GraphicsObject->colorIndex = 1;
        g_MenuWinBBlend4GraphicsObject->paletteIndex = 1;
    }
    if (AcquireBatchNode(1, g_MenuWinABlend0GraphicsObject) != 0) {
        curUStartScalar = 0.1875f;
        vStart = 0.59375f;
        uSpan = 0.1875f;
        texHeight = 0.03125f;
        shadowTopLeftPtr = (GfxTlVertex *)(g_MenuWinABlend0GraphicsObject->vertexCursor);
        shadowTopLeftPtr->x = left;
        shadowTopLeftPtr->y = top + 24.0f;
        shadowTopLeftPtr->z = depth;
        shadowTopLeftPtr->rhwBits = FLOAT_ONE_BITS;
        shadowTopLeftPtr->color = 0x80FFFFFF;
        shadowTopLeftPtr->specular = 0xFF000000;
        shadowTopLeftPtr->u = curUStartScalar;
        shadowTopLeftPtr->v = vStart;
        pShadowBL = g_MenuWinABlend0GraphicsObject->vertices + 1;
        pShadowBL->x = left;
        pShadowBL->y = top + 24.0f + 8.0f;
        pShadowBL->z = depth;
        pShadowBL->rhwBits = FLOAT_ONE_BITS;
        pShadowBL->color = 0x80FFFFFF;
        pShadowBL->specular = 0xFF000000;
        pShadowBL->u = curUStartScalar;
        pShadowBL->v = vStart + texHeight;
        pShadowTR = g_MenuWinABlend0GraphicsObject->vertices + 2;
        pShadowTR->x = left + 48.0f;
        pShadowTR->y = top + 24.0f;
        pShadowTR->z = depth;
        pShadowTR->rhwBits = FLOAT_ONE_BITS;
        pShadowTR->color = 0x80FFFFFF;
        pShadowTR->specular = 0xFF000000;
        pShadowTR->u = curUStartScalar + uSpan;
        pShadowTR->v = vStart;
        shadowBR = g_MenuWinABlend0GraphicsObject->vertices + 3;
        shadowBR->x = left + 48.0f;
        shadowBR->y = top + 24.0f + 8.0f;
        shadowBR->z = depth;
        shadowBR->rhwBits = FLOAT_ONE_BITS;
        shadowBR->color = 0x80FFFFFF;
        shadowBR->specular = 0xFF000000;
        shadowBR->u = curUStartScalar + uSpan;
        shadowBR->v = vStart + texHeight;
        *(char *)g_MenuWinABlend0GraphicsObject->colorIndex = 1;
        g_MenuWinABlend0GraphicsObject->texturePage = 1;
        g_MenuBlendWindowBgPending = 1;
    }
}

/*
 * Acquires five UI quads and emits a 16-by-42 separator at origin X plus 64,
 * choosing its UV column from the original value bit 0 before clamping. Signed
 * values above 5999 clamp to 5999; four 32-by-42 digit quads use successive signed
 * divisor/remainder steps and two atlas rows.
 */
// FUNCTION: FF7 0x006eb793
void __cdecl Menu_DrawCountdownTimer2x(int posX, int posY, unsigned int countdownValue)
{
    /* p,a..o occupy VC5's buckets 0..15, hence slots -4 through -40. */
    int digitIdx;
    int tempStartX;
    int savedY;
    int digitValue;
    float textureV;
    float uValue;
    float texStepV;
    float f;
    GfxTlVertex *separatorVertD;
    GfxTlVertex *separatorTopRightPtr;
    GfxTlVertex *separatorBottomLeftPtr;
    GfxTlVertex *pSeparatorTopLeft;
    GfxTlVertex *digitTopLeft;
    GfxTlVertex *digitBottomRight;
    GfxTlVertex *digitVertexCPtr;
    GfxTlVertex *glyphVertexB;

    tempStartX = posX;
    savedY = posY;
    if (AcquireBatchNode(5, g_MenuWinBBlend4GraphicsObject) != 0) {
        uValue = (countdownValue & 1) ? 0.3125f : 0.25f;
        textureV = 0.625f;
        f = 0.0625f;
        texStepV = 0.1640625f;
        pSeparatorTopLeft = g_MenuWinBBlend4GraphicsObject->vertices;
        pSeparatorTopLeft->x = posX + 64.0f;
        pSeparatorTopLeft->y = posY;
        pSeparatorTopLeft->zBits = 0x3749539C;
        pSeparatorTopLeft->rhwBits = FLOAT_ONE_BITS;
        pSeparatorTopLeft->color = 0xFFFFFFFF;
        pSeparatorTopLeft->specular = 0xFF000000;
        pSeparatorTopLeft->u = uValue;
        pSeparatorTopLeft->v = textureV;
        separatorBottomLeftPtr = g_MenuWinBBlend4GraphicsObject->vertices + 1;
        separatorBottomLeftPtr->x = posX + 64.0f;
        separatorBottomLeftPtr->y = posY + 42.0f;
        separatorBottomLeftPtr->zBits = 0x3749539C;
        separatorBottomLeftPtr->rhwBits = FLOAT_ONE_BITS;
        separatorBottomLeftPtr->color = 0xFFFFFFFF;
        separatorBottomLeftPtr->specular = 0xFF000000;
        separatorBottomLeftPtr->u = uValue;
        separatorBottomLeftPtr->v = textureV + texStepV;
        separatorTopRightPtr = g_MenuWinBBlend4GraphicsObject->vertices + 2;
        separatorTopRightPtr->x = posX + 64.0f + 16.0f;
        separatorTopRightPtr->y = posY;
        separatorTopRightPtr->zBits = 0x3749539C;
        separatorTopRightPtr->rhwBits = FLOAT_ONE_BITS;
        separatorTopRightPtr->color = 0xFFFFFFFF;
        separatorTopRightPtr->specular = 0xFF000000;
        separatorTopRightPtr->u = uValue + f;
        separatorTopRightPtr->v = textureV;
        separatorVertD = g_MenuWinBBlend4GraphicsObject->vertices + 3;
        separatorVertD->x = posX + 64.0f + 16.0f;
        separatorVertD->y = posY + 42.0f;
        separatorVertD->zBits = 0x3749539C;
        separatorVertD->rhwBits = FLOAT_ONE_BITS;
        separatorVertD->color = 0xFFFFFFFF;
        separatorVertD->specular = 0xFF000000;
        separatorVertD->u = uValue + f;
        separatorVertD->v = textureV + texStepV;
        *(char *)g_MenuWinBBlend4GraphicsObject->colorIndex = 0xC;
        g_MenuWinBBlend4GraphicsObject->paletteIndex = 0xC;
        g_MenuWinBBlend4GraphicsObject->vertexCursor += g_MenuWinBBlend4GraphicsObject->cursorStep;
        g_MenuWinBBlend4GraphicsObject->colorIndex += 1;
        if ((int)countdownValue > 5999) {
            countdownValue = 5999;
        }
        for (digitIdx = 0; digitIdx < 4; digitIdx++) {
            digitValue = (int)countdownValue / g_MenuTimeDivisors[digitIdx];
            uValue = ((digitValue % 5) * 32 + 0x60) * 0.00390625f;
            textureV = digitValue > 4 ? 0.8125f : 0.625f;
            f = 0.125f;
            digitTopLeft = g_MenuWinBBlend4GraphicsObject->vertices;
            digitTopLeft->x = posX;
            digitTopLeft->y = posY;
            digitTopLeft->zBits = 0x3749539C;
            digitTopLeft->rhwBits = FLOAT_ONE_BITS;
            digitTopLeft->color = 0xFFFFFFFF;
            digitTopLeft->specular = 0xFF000000;
            digitTopLeft->u = uValue;
            digitTopLeft->v = textureV;
            glyphVertexB = g_MenuWinBBlend4GraphicsObject->vertices + 1;
            glyphVertexB->x = posX;
            glyphVertexB->y = posY + 42.0f;
            glyphVertexB->zBits = 0x3749539C;
            glyphVertexB->rhwBits = FLOAT_ONE_BITS;
            glyphVertexB->color = 0xFFFFFFFF;
            glyphVertexB->specular = 0xFF000000;
            glyphVertexB->u = uValue;
            glyphVertexB->v = textureV + texStepV;
            digitVertexCPtr = g_MenuWinBBlend4GraphicsObject->vertices + 2;
            digitVertexCPtr->x = posX + 32.0f;
            digitVertexCPtr->y = posY;
            digitVertexCPtr->zBits = 0x3749539C;
            digitVertexCPtr->rhwBits = FLOAT_ONE_BITS;
            digitVertexCPtr->color = 0xFFFFFFFF;
            digitVertexCPtr->specular = 0xFF000000;
            digitVertexCPtr->u = uValue + f;
            digitVertexCPtr->v = textureV;
            digitBottomRight = g_MenuWinBBlend4GraphicsObject->vertices + 3;
            digitBottomRight->x = posX + 32.0f;
            digitBottomRight->y = posY + 42.0f;
            digitBottomRight->zBits = 0x3749539C;
            digitBottomRight->rhwBits = FLOAT_ONE_BITS;
            digitBottomRight->color = 0xFFFFFFFF;
            digitBottomRight->specular = 0xFF000000;
            digitBottomRight->u = uValue + f;
            digitBottomRight->v = textureV + texStepV;
            *(char *)g_MenuWinBBlend4GraphicsObject->colorIndex = 0xC;
            g_MenuWinBBlend4GraphicsObject->paletteIndex = 0xC;
            g_MenuWinBBlend4GraphicsObject->vertexCursor += g_MenuWinBBlend4GraphicsObject->cursorStep;
            g_MenuWinBBlend4GraphicsObject->colorIndex += 1;
            if (digitIdx == 1) {
                posX += 16;
            }
            countdownValue = (int)countdownValue % g_MenuTimeDivisors[digitIdx];
            posX += 32;
        }
    }
    if (g_MenuDigitBackdropEnabled != 0) {
        int width;
        int height;

        width = 144;
        height = 42;
        Menu_DrawTranslucentBlackRect(tempStartX, savedY, width, height);
        g_MenuDoDrawBlackQuad = 1;
    }
}

/*
 * Extracts eight decimal positions using the divisor table
 * g_MenuNumberDivisors and draws the rightmost digitCount positions without
 * suppressing leading zeroes. Each acquired batch node receives a white 32 by 42
 * textured quad; digit remainder modulo 5 selects the atlas column and digit
 * greater than 4 selects the second row.
 */
// FUNCTION: FF7 0x006ebc94
void __cdecl Menu_DrawMessageWindowNumber2x(int posX, int posY, int value, int digitCount, int flags)
{
    float tvValue;
    float uVal;
    int digitValue;
    GfxTlVertex *digitTopRightPtr;
    GfxTlVertex *v;
    GfxTlVertex *digitVertA;
    float texStepV;
    float tuStep;
    int uOffset;
    GfxTlVertex *digitVertD;
    int vRowFlags;
    int idx;

    for (idx = 0; idx < 8; idx++) {
        digitValue = value / g_MenuNumberDivisors[idx];
        if (8 - idx <= digitCount) {
            if (AcquireBatchNode(1, g_MenuWinBBlend4GraphicsObject) != 0) {
                uOffset = (digitValue % 5 << 5) + 0x60;
                uVal = uOffset * 0.00390625f;
                if (digitValue > 4) {
                    vRowFlags = 0x3F500000;
                } else {
                    vRowFlags = FLOAT_0_625_BITS;
                }
                *(int *)&tvValue = vRowFlags;
                tuStep = 0.125f;
                texStepV = 0.1640625f;
                digitVertA = g_MenuWinBBlend4GraphicsObject->vertices;
                digitVertA->x = posX;
                digitVertA->y = posY;
                digitVertA->zBits = 0x3727C5AC;
                digitVertA->rhwBits = FLOAT_ONE_BITS;
                digitVertA->color = 0xFFFFFFFF;
                digitVertA->specular = 0xFF000000;
                digitVertA->u = uVal;
                digitVertA->v = tvValue;
                v = g_MenuWinBBlend4GraphicsObject->vertices + 1;
                v->x = posX;
                v->y = posY + 42.0f;
                v->zBits = 0x3727C5AC;
                v->rhwBits = FLOAT_ONE_BITS;
                v->color = 0xFFFFFFFF;
                v->specular = 0xFF000000;
                v->u = uVal;
                v->v = tvValue + texStepV;
                digitTopRightPtr = g_MenuWinBBlend4GraphicsObject->vertices + 2;
                digitTopRightPtr->x = posX + 32.0f;
                digitTopRightPtr->y = posY;
                digitTopRightPtr->zBits = 0x3727C5AC;
                digitTopRightPtr->rhwBits = FLOAT_ONE_BITS;
                digitTopRightPtr->color = 0xFFFFFFFF;
                digitTopRightPtr->specular = 0xFF000000;
                digitTopRightPtr->u = uVal + tuStep;
                digitTopRightPtr->v = tvValue;
                digitVertD = g_MenuWinBBlend4GraphicsObject->vertices + 3;
                digitVertD->x = posX + 32.0f;
                digitVertD->y = posY + 42.0f;
                digitVertD->zBits = 0x3727C5AC;
                digitVertD->rhwBits = FLOAT_ONE_BITS;
                digitVertD->color = 0xFFFFFFFF;
                digitVertD->specular = 0xFF000000;
                digitVertD->u = uVal + tuStep;
                digitVertD->v = tvValue + texStepV;
                *(char *)g_MenuWinBBlend4GraphicsObject->colorIndex = 0xC;
                g_MenuWinBBlend4GraphicsObject->paletteIndex = 0xC;
                g_MenuWinBBlend4GraphicsObject->vertexCursor += g_MenuWinBBlend4GraphicsObject->cursorStep;
                g_MenuWinBBlend4GraphicsObject->colorIndex++;
            }
            posX += 32;
        }
        value = value % g_MenuNumberDivisors[idx];
    }
}

/*
 * Renders active message windows in reverse index order at two-scale or single-
 * scale according to g_MenuIsSmallViewport320x240, resetting shared text state per window and
 * flushing UI batches before setting its opening viewport. Clamps negative clip
 * origin and maximum width; the oversized-height branch mistakenly assigns the
 * height limit to clipWidth and leaves clipHeight unchanged in both scales.
 */
// FUNCTION: FF7 0x006ebf2c
int __cdecl Menu_RenderMessageWindows(struct WindowData *windowArray, short windowCount, int renderPhase)
{
    struct MessageWindow {
        unsigned char *text;
        short x, y, width, height, openWidth, openHeight, scroll, state;
        short cursorX, cursorY;
        signed char more, flags, arrow, addon, addonFlags, digits;
        short padding;
        int value;
        short arrowX, arrowY, addonX, addonY, active, reserved;
    };
    struct {
        short drawHeight, padHeight;
        int clipWidth;
        short index, padIndex;
        int clipY, clipX, clipHeight, y;
        short rect[4];
    } v; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
#define W (((struct MessageWindow *)windowArray)[v.index])
    g_MenuDrawBox = 0;
    g_MenuNonTopLevelScreenFlag = 1;
    g_MenuTextTick++;
    if (g_MenuIsSmallViewport320x240 != 1) {
        for (v.index = windowCount - 1; v.index >= 0; v.index--) {
            if (!W.active)
                continue;
            g_MenuTextFixedPitch = 0;
            g_MenuTextRow = 0;
            g_MenuTextColumn = 0;
            g_MenuMessageX = W.x * 2;
            v.y = W.y * 2 + g_MenuMessageWindowYOffset;
            Menu_FlushUiBatches();
            v.clipX = g_MenuMessageX + W.width - W.openWidth;
            v.clipY = v.y + W.height - W.openHeight;
            v.clipWidth = W.openWidth * 2;
            v.clipHeight = W.openHeight * 2;
            if (v.clipX < 0)
                v.clipX = 0;
            if (v.clipY < 0)
                v.clipY = 0;
            if (v.clipWidth > 639)
                v.clipWidth = 639;
            if (v.clipHeight > 479)
                v.clipWidth = 479;
            GfxSetViewportRect(v.clipX, v.clipY, v.clipWidth, v.clipHeight, GetEngineContext());
            if (W.arrow) {
                Menu_DrawMoreTextArrow2x(g_MenuMessageX + W.arrowX * 2, v.y + W.arrowY * 2, 0);
                g_MenuDrawTextBoxes = 1;
            }
            switch (W.addon) {
            case MESSAGE_WINDOW_ADDON_COUNTDOWN:
                Menu_DrawCountdownTimer2x(g_MenuMessageX + W.addonX * 2, v.y + W.addonY * 2, gSavemap.countdownSeconds);
                g_MenuDrawTextBoxes = 1;
                break;
            case MESSAGE_WINDOW_ADDON_NUMBER:
                Menu_DrawMessageWindowNumber2x(g_MenuMessageX + W.addonX * 2, v.y + W.addonY * 2, W.value, W.digits, W.addonFlags);
                g_MenuDrawTextBoxes = 1;
                break;
            }
            g_MenuMessageWindowStyleFlag4 = (W.flags & 4) != 0;
            g_MenuTextColour[0] = 7;
            g_MenuTextRainbow = 0;
            g_MenuTextBlink = g_MenuTextRainbow;
            g_MenuTextRemaining = W.state;
            /* Retail caller prototype (int, int, int, unsigned char *, int) -> short preserved for /Od argument promotion on `Menu_DrawMessageText2x`. */
            v.drawHeight = ((short(__cdecl *)(int, int, int, unsigned char *, int))Menu_DrawMessageText2x)(g_MenuMessageX + 16, v.y + 12 + W.scroll * 2, W.width * 2 - 6, W.text, g_MessageWindowDepthBits[v.index]);
            if (v.drawHeight + 32 > W.height * 2 - 6)
                W.more = 1;
            else
                W.more = 0;
            if (W.flags & 2)
                g_MenuMessageWindowActive = 1;
            else
                g_MenuMessageWindowActive = 0;
            if (!(W.flags & 1)) {
                g_MenuWinCBlend4 = 1;
                if (W.width != W.openWidth || W.height != W.openHeight) {
                    Menu_SetRectShorts(v.rect, g_MenuMessageX + W.width - W.openWidth, v.y + W.height - W.openHeight, W.openWidth * 2, W.openHeight * 2);
                    /* Retail caller prototype (int, int) -> void preserved for /Od argument promotion on `Menu_DrawMessageWindowFrame2x`. */
                    ((void(__cdecl *)(int, int))Menu_DrawMessageWindowFrame2x)((int)v.rect, g_MessageWindowDepthBits[v.index]);
                } else {
                    Menu_SetRectShorts(v.rect, g_MenuMessageX, v.y, W.width * 2, W.height * 2);
                    /* Retail caller prototype (int, int) -> void preserved for /Od argument promotion on `Menu_DrawMessageWindowFrame2x`. */
                    ((void(__cdecl *)(int, int))Menu_DrawMessageWindowFrame2x)((int)v.rect, g_MessageWindowDepthBits[v.index]);
                }
            }
            W.cursorX = g_MenuTextColumn;
            W.cursorY = g_MenuTextRow;
            g_MenuMessageWindowActive = 0;
            if (g_MenuShouldDrawBlackQuad && W.addon != MESSAGE_WINDOW_ADDON_COUNTDOWN) {
                Menu_DrawTranslucentBlackRect(v.clipX, v.clipY, v.clipWidth, v.clipHeight);
                g_MenuDoDrawBlackQuad = 1;
            }
        }
    } else {
        for (v.index = windowCount - 1; v.index >= 0; v.index--) {
            if (!W.active)
                continue;
            g_MenuTextFixedPitch = 0;
            g_MenuTextRow = 0;
            g_MenuTextColumn = 0;
            g_MenuMessageX = W.x;
            v.y = W.y + g_MenuMessageWindowYOffset;
            Menu_FlushUiBatches();
            v.clipX = g_MenuMessageX + (W.width >> 1) - (W.openWidth >> 1);
            v.clipY = v.y + (W.height >> 1) - (W.openHeight >> 1);
            v.clipWidth = W.openWidth;
            v.clipHeight = W.openHeight;
            if (v.clipX < 0)
                v.clipX = 0;
            if (v.clipY < 0)
                v.clipY = 0;
            if (v.clipWidth > 319)
                v.clipWidth = 319;
            if (v.clipHeight > 239)
                v.clipWidth = 239;
            GfxSetViewportRect(v.clipX + g_MenuViewportX, v.clipY + g_MenuViewportY, v.clipWidth, v.clipHeight, GetEngineContext());
            if (W.arrow) {
                Menu_DrawMoreTextArrow1x(W.arrowX + g_MenuMessageX, W.arrowY + v.y, 0);
                g_MenuDrawTextBoxes = 1;
            }
            switch (W.addon) {
            case MESSAGE_WINDOW_ADDON_COUNTDOWN:
                Menu_DrawCountdownTimer1x(W.addonX + g_MenuMessageX, W.addonY + v.y, gSavemap.countdownSeconds);
                g_MenuDrawTextBoxes = 1;
                break;
            case MESSAGE_WINDOW_ADDON_NUMBER:
                Menu_DrawMessageWindowNumber1x(W.addonX + g_MenuMessageX, W.addonY + v.y, W.value, W.digits, W.addonFlags);
                g_MenuDrawTextBoxes = 1;
                break;
            }
            g_MenuMessageWindowStyleFlag4 = (W.flags & 4) != 0;
            g_MenuTextColour[0] = 7;
            g_MenuTextRainbow = 0;
            g_MenuTextBlink = g_MenuTextRainbow;
            g_MenuTextRemaining = W.state;
            /* Retail caller prototype (int, int, int, unsigned char *, int) -> short preserved for /Od argument promotion on `Menu_DrawMessageText1x`. */
            v.drawHeight = ((short(__cdecl *)(int, int, int, unsigned char *, int))Menu_DrawMessageText1x)(g_MenuMessageX + 8, W.scroll + v.y + 6, W.width - 3, W.text, g_MessageWindowDepthBits[v.index]);
            if (v.drawHeight + 16 > W.height - 3)
                W.more = 1;
            else
                W.more = 0;
            if (W.flags & 2)
                g_MenuMessageWindowActive = 1;
            else
                g_MenuMessageWindowActive = 0;
            if (!(W.flags & 1)) {
                g_MenuWinCBlend4 = 1;
                if (W.width != W.openWidth || W.height != W.openHeight) {
                    Menu_SetRectShorts(v.rect, g_MenuMessageX + (W.width >> 1) - (W.openWidth >> 1), v.y + (W.height >> 1) - (W.openHeight >> 1), W.openWidth, W.openHeight);
                    /* Retail caller prototype (int, int) -> void preserved for /Od argument promotion on `Menu_DrawMessageWindowFrame1x`. */
                    ((void(__cdecl *)(int, int))Menu_DrawMessageWindowFrame1x)((int)v.rect, g_MessageWindowDepthBits[v.index]);
                } else {
                    Menu_SetRectShorts(v.rect, g_MenuMessageX, v.y, W.width, W.height);
                    /* Retail caller prototype (int, int) -> void preserved for /Od argument promotion on `Menu_DrawMessageWindowFrame1x`. */
                    ((void(__cdecl *)(int, int))Menu_DrawMessageWindowFrame1x)((int)v.rect, g_MessageWindowDepthBits[v.index]);
                }
            }
            W.cursorX = g_MenuTextColumn;
            W.cursorY = g_MenuTextRow;
            g_MenuMessageWindowActive = 0;
            if (g_MenuShouldDrawBlackQuad && W.addon != MESSAGE_WINDOW_ADDON_COUNTDOWN) {
                Menu_DrawTranslucentBlackRect(v.clipX + g_MenuViewportX, v.clipY + g_MenuViewportY, v.clipWidth, v.clipHeight);
                g_MenuDoDrawBlackQuad = 1;
            }
        }
    }
    g_MenuDrawBox = 1;
    return 0;
#undef W
}

/*
 * Emits menu primitive lists selected by dirty flags and invalidates each emitted
 * batch, setting scene passes and render state for their groups. Double scale uses
 * paired or five-list groups where low scale uses fewer lists.
 */
// FUNCTION: FF7 0x006eca68
void __cdecl Menu_FlushUiBatches(void)
{
    GfxEngineContext *ctx;

    ctx = GetEngineContext();
    if (g_MenuIsSmallViewport320x240 != 1) {
        if (g_MenuWindowBgPending != 0) {
            Gfx_SetScenePassCall(0, ctx);
            Gfx_SetRenderStateCall(RENDER_STATE_LINEAR_FILTER, 0, ctx);
            Gfx_EmitPrimitiveBatchList(g_MenuWindowBgGraphicsObject, ctx);
            InvalidateBatchGeneration(g_MenuWindowBgGraphicsObject);
            g_MenuWindowBgPending = 0;
        }
        if (g_MenuBlendWindowBgPending != 0) {
            Gfx_SetScenePassCall(1, ctx);
            Gfx_SetRenderStateCall(RENDER_STATE_LINEAR_FILTER, 0, ctx);
            Gfx_EmitPrimitiveBatchList(g_MenuBlendWindowBgGraphicsObject, ctx);
            Gfx_EmitPrimitiveBatchList(g_MenuWinABlend0GraphicsObject, ctx);
            InvalidateBatchGeneration(g_MenuWinABlend0GraphicsObject);
            InvalidateBatchGeneration(g_MenuBlendWindowBgGraphicsObject);
            g_MenuBlendWindowBgPending = 0;
        }
        Gfx_SetScenePassCall(0, ctx);
        Gfx_SetRenderStateCall(RENDER_STATE_LINEAR_FILTER, 0, ctx);
        if (g_MenuDrawCharacter != 0) {
            Gfx_EmitPrimitiveBatchList(g_MenuFontAGraphicsObject, ctx);
            Gfx_EmitPrimitiveBatchList(g_MenuFontBGraphicsObject.batchList, ctx);
            InvalidateBatchGeneration(g_MenuFontAGraphicsObject);
            InvalidateBatchGeneration(g_MenuFontBGraphicsObject.batchList);
            g_MenuDrawCharacter = 0;
        }
        if (g_MenuDrawTextBoxes != 0 || g_MenuWinCBlend4 != 0) {
            Gfx_EmitPrimitiveBatchList(g_MenuWinABlend4GraphicsObject, ctx);
            Gfx_EmitPrimitiveBatchList(g_MenuWinBBlend4GraphicsObject, ctx);
            Gfx_EmitPrimitiveBatchList(g_MenuWinCBlend4GraphicsObject, ctx);
            Gfx_EmitPrimitiveBatchList(g_MenuWinCBlend4DiffGraphicsObject, ctx);
            Gfx_EmitPrimitiveBatchList(g_MenuWinDBlend4GraphicsObject, ctx);
            InvalidateBatchGeneration(g_MenuWinABlend4GraphicsObject);
            InvalidateBatchGeneration(g_MenuWinBBlend4GraphicsObject);
            InvalidateBatchGeneration(g_MenuWinCBlend4GraphicsObject);
            InvalidateBatchGeneration(g_MenuWinDBlend4GraphicsObject);
            InvalidateBatchGeneration(g_MenuWinCBlend4DiffGraphicsObject);
            g_MenuWinCBlend4 = 0;
            g_MenuDrawTextBoxes = g_MenuWinCBlend4;
        }
        if (g_MenuDoDrawBlackQuad != 0) {
            Gfx_SetScenePassCall(1, ctx);
            Gfx_SetRenderStateCall(RENDER_STATE_LINEAR_FILTER, 0, ctx);
            Gfx_EmitPrimitiveBatchList(g_MenuTextBoxQuadGraphicsObject, ctx);
            InvalidateBatchGeneration(g_MenuTextBoxQuadGraphicsObject);
            g_MenuDigitBackdropEnabled = 0;
            g_MenuDoDrawBlackQuad = g_MenuDigitBackdropEnabled;
            if (g_MenuDrawBox != 0) {
                g_MenuShouldDrawBlackQuad = 0;
            }
        }
    } else {
        if (g_MenuWindowBgPending != 0) {
            Gfx_EmitPrimitiveBatchList(g_MenuWindowBgGraphicsObject, ctx);
            InvalidateBatchGeneration(g_MenuWindowBgGraphicsObject);
            g_MenuWindowBgPending = 0;
        }
        if (g_MenuBlendWindowBgPending != 0) {
            Gfx_SetScenePassCall(1, ctx);
            Gfx_SetRenderStateCall(RENDER_STATE_LINEAR_FILTER, 0, ctx);
            Gfx_EmitPrimitiveBatchList(g_MenuBlendWindowBgGraphicsObject, ctx);
            Gfx_EmitPrimitiveBatchList(g_MenuWinBlend0GraphicsObject, ctx);
            InvalidateBatchGeneration(g_MenuWinBlend0GraphicsObject);
            InvalidateBatchGeneration(g_MenuBlendWindowBgGraphicsObject);
            g_MenuBlendWindowBgPending = 0;
        }
        Gfx_SetScenePassCall(0, ctx);
        Gfx_SetRenderStateCall(RENDER_STATE_LINEAR_FILTER, 0, ctx);
        if (g_MenuDrawCharacter != 0) {
            Gfx_EmitPrimitiveBatchList(g_MenuFontBlend4GraphicsObject, ctx);
            InvalidateBatchGeneration(g_MenuFontBlend4GraphicsObject);
            g_MenuDrawCharacter = 0;
        }
        if (g_MenuDrawTextBoxes != 0 || g_MenuWinCBlend4 != 0) {
            Gfx_EmitPrimitiveBatchList(g_MenuWinBlend4GraphicsObject, ctx);
            Gfx_EmitPrimitiveBatchList(g_MenuWinCBlend4DiffGraphicsObject, ctx);
            InvalidateBatchGeneration(g_MenuWinBlend4GraphicsObject);
            InvalidateBatchGeneration(g_MenuWinCBlend4DiffGraphicsObject);
            g_MenuDrawTextBoxes = 0;
        }
        if (g_MenuDoDrawBlackQuad != 0) {
            Gfx_SetScenePassCall(1, ctx);
            Gfx_SetRenderStateCall(RENDER_STATE_LINEAR_FILTER, 0, ctx);
            Gfx_EmitPrimitiveBatchList(g_MenuTextBoxQuadGraphicsObject, ctx);
            InvalidateBatchGeneration(g_MenuTextBoxQuadGraphicsObject);
            g_MenuDigitBackdropEnabled = 0;
            g_MenuDoDrawBlackQuad = g_MenuDigitBackdropEnabled;
            if (g_MenuDrawBox != 0) {
                g_MenuShouldDrawBlackQuad = 0;
            }
        }
    }
}

/*
 * The 320x240 twin of Menu_DrawCharPortrait2x: the quad is 40x48, the texture
 * spans (0, 0) to (0.625, 0.75), and the position is offset by the 1x layout's
 * origin before anything is written. The locals keep the solver's letters: p and q
 * are the top-left texture coordinate, r and s the texture width and height, and
 * t, u, v and w the four vertices in the order top-left, bottom-left, top-right,
 * bottom-right.
 */
// FUNCTION: FF7 0x006ecece
void __cdecl Menu_DrawCharPortrait1x(int x, int y, int slot, float z)
{
    float vStart;
    float curUStartScalar;
    float uSpan;
    GfxTlVertex *vertexAPtr;
    float texHeight;
    GfxTlVertex *bottomRemaining;
    GfxTlVertex *v;
    GfxTlVertex *bottomRight;

    if (Menu_IsTextSubsystemReady() == 0) {
        return;
    }
    x += g_MenuViewportX;
    y += g_MenuViewportY;
    if (AcquireBatchNode(1, g_MenuFontBGraphicsObject.portraitRenderObjects[slot]) != 0) {
        vStart = 0.0f;
        curUStartScalar = vStart;
        uSpan = 0.625f;
        texHeight = 0.75f;
        vertexAPtr = g_MenuFontBGraphicsObject.portraitRenderObjects[slot]->vertices;
        vertexAPtr->x = x;
        vertexAPtr->y = y;
        vertexAPtr->z = z;
        vertexAPtr->rhwBits = FLOAT_ONE_BITS;
        vertexAPtr->color = 0xFFFFFFFF;
        vertexAPtr->specular = 0xFF000000;
        vertexAPtr->u = curUStartScalar;
        vertexAPtr->v = vStart;
        bottomRemaining = g_MenuFontBGraphicsObject.portraitRenderObjects[slot]->vertices + 1;
        bottomRemaining->x = x;
        bottomRemaining->y = y + 48.0f;
        bottomRemaining->z = z;
        bottomRemaining->rhwBits = FLOAT_ONE_BITS;
        bottomRemaining->color = 0xFFFFFFFF;
        bottomRemaining->specular = 0xFF000000;
        bottomRemaining->u = curUStartScalar;
        bottomRemaining->v = vStart + texHeight;
        v = g_MenuFontBGraphicsObject.portraitRenderObjects[slot]->vertices + 2;
        v->x = x + 40.0f;
        v->y = y;
        v->z = z;
        v->rhwBits = FLOAT_ONE_BITS;
        v->color = 0xFFFFFFFF;
        v->specular = 0xFF000000;
        v->u = curUStartScalar + uSpan;
        v->v = vStart;
        bottomRight = g_MenuFontBGraphicsObject.portraitRenderObjects[slot]->vertices + 3;
        bottomRight->x = x + 40.0f;
        bottomRight->y = y + 48.0f;
        bottomRight->z = z;
        bottomRight->rhwBits = FLOAT_ONE_BITS;
        bottomRight->color = 0xFFFFFFFF;
        bottomRight->specular = 0xFF000000;
        bottomRight->u = curUStartScalar + uSpan;
        bottomRight->v = vStart + texHeight;
        *g_MenuFontBGraphicsObject.portraitRenderObjects[slot]->colorSignedBytePtr = 0;
        g_MenuFontBGraphicsObject.portraitRenderObjects[slot]->paletteIndex = 0;
    }
}

/*
 * Returns unless text is ready, maps character ID 9 to 8 and adds the low-scale
 * layout origin to caller X/Y. On acquisition of its g_MenuCharPortraitGraphicsObjects portrait batch,
 * writes a caller-sized rectangle with caller depth, reciprocal W 1, white diffuse
 * and black specular.
 */
// FUNCTION: FF7 0x006ed0ec
void __cdecl Menu_DrawCharPortraitById1x(int posX, int posY, int width, int height, int charId, float depth)
{
    float vStart;
    float texLeft;
    float uRightMax;
    GfxTlVertex *vert1;
    GfxTlVertex *vtx0Ptr;
    float texBottom;
    GfxTlVertex *v;
    GfxTlVertex *nextVertex;

    if (Menu_IsTextSubsystemReady() == 0) {
        return;
    }
    if (charId == 9) {
        charId = 8;
    }
    posX += g_MenuViewportX;
    posY += g_MenuViewportY;
    if (AcquireBatchNode(1, g_MenuCharPortraitGraphicsObjects[charId]) != 0) {
        vStart = 0.0f;
        texLeft = vStart;
        uRightMax = 0.625f;
        texBottom = 0.75f;
        vtx0Ptr = g_MenuCharPortraitGraphicsObjects[charId]->vertices;
        vtx0Ptr->x = (float)posX;
        vtx0Ptr->y = (float)posY;
        vtx0Ptr->z = depth;
        vtx0Ptr->rhwBits = FLOAT_ONE_BITS;
        vtx0Ptr->color = 0xFFFFFFFF;
        vtx0Ptr->specular = 0xFF000000;
        vtx0Ptr->u = texLeft;
        vtx0Ptr->v = vStart;
        vert1 = g_MenuCharPortraitGraphicsObjects[charId]->vertices + 1;
        vert1->x = (float)posX;
        vert1->y = (float)posY + height;
        vert1->z = depth;
        vert1->rhwBits = FLOAT_ONE_BITS;
        vert1->color = 0xFFFFFFFF;
        vert1->specular = 0xFF000000;
        vert1->u = texLeft;
        vert1->v = vStart + texBottom;
        v = g_MenuCharPortraitGraphicsObjects[charId]->vertices + 2;
        v->x = (float)posX + width;
        v->y = (float)posY;
        v->z = depth;
        v->rhwBits = FLOAT_ONE_BITS;
        v->color = 0xFFFFFFFF;
        v->specular = 0xFF000000;
        v->u = texLeft + uRightMax;
        v->v = vStart;
        nextVertex = g_MenuCharPortraitGraphicsObjects[charId]->vertices + 3;
        nextVertex->x = (float)posX + width;
        nextVertex->y = (float)posY + height;
        nextVertex->z = depth;
        nextVertex->rhwBits = FLOAT_ONE_BITS;
        nextVertex->color = 0xFFFFFFFF;
        nextVertex->specular = 0xFF000000;
        nextVertex->u = texLeft + uRightMax;
        nextVertex->v = vStart + texBottom;
        *g_MenuCharPortraitGraphicsObjects[charId]->colorSignedBytePtr = 0;
        g_MenuCharPortraitGraphicsObjects[charId]->paletteIndex = 0;
    }
}

/*
 * Renders up to 1024 encoded-message iterations at 1x scale with viewport offsets,
 * shared reveal count, row/column counters and wrap width, returning the final Y
 * coordinate. Handles line breaks, multibyte glyph banks, color changes,
 * blink/rainbow toggles, fixed-width toggling and four 16-pixel icons; ordinary
 * glyphs use 12-pixel quads and packed advance metrics or a fixed 13-pixel
 * advance.
 */
// FUNCTION: FF7 0x006ed30b
short __cdecl Menu_DrawMessageText1x(short penX, short penY, short wrapWidth, unsigned char *text, float z)
{
    struct {
        GfxTlVertex *vtx7, *vtx6, *vtx5, *vtx4, *vtx3, *vtx2, *vtx1, *vtx0;
        short skip, padSkip, first, padFirst;
        float dv, du;
        short code, padCode;
        float texV;
        short colour, padColour, unused, padUnused;
        float texU;
        short count, padCount, offset, padOffset, atlasY, padY;
        unsigned short phase, padPhase;
        short atlasX, padX;
    } f; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
#define TEXT_METRIC (g_pTextSpacing[f.code + f.offset])
    penX += g_MenuViewportX;
    penY += g_MenuViewportY;
    f.phase = 0;
    for (f.count = 0; f.count < 1024; f.count++, f.phase++) {
        f.first = *text;
        if (g_MenuTextRemaining == 0 || *text == TEXT_END || *text == TEXT_NEW_PAGE || *text == TEXT_NEW_PAGE_ALT)
            break;
        if (*text == TEXT_NEWLINE) {
            penX = g_MenuMessageX + g_MenuViewportX + 8;
            penY += 16;
            text++;
            g_MenuTextRow++;
            g_MenuTextColumn++;
            continue;
        }
        f.skip = 0;
        f.unused = f.skip;
        switch (*text) {
        case TEXT_FONT_PAGE_1:
            g_MenuTextColumn++;
            text++;
            f.atlasY = 132;
            f.offset = 0xe7;
            break;
        case TEXT_FONT_PAGE_2:
            g_MenuTextColumn++;
            text++;
            f.atlasY = 0;
            f.skip = 16;
            f.offset = 0x1b9;
            break;
        case TEXT_FONT_PAGE_3:
            g_MenuTextColumn++;
            text++;
            f.atlasY = 132;
            f.skip = 16;
            f.offset = 0x2a0;
            break;
        case TEXT_FONT_PAGE_4:
            g_MenuTextColumn++;
            text++;
            f.atlasY = 132;
            f.offset = 0x372;
            f.unused = -64;
            break;
        case TEXT_EXTENDED:
            text++;
            g_MenuTextColumn++;
            if (*text < TEXT_FE_COLOR_0) {
                f.atlasY = 132;
                f.skip = 16;
                f.offset = 0x444;
                f.unused = -64;
            } else {
                g_MenuTextColumn++;
                if (*text < TEXT_FE_BLINK) {
                    g_MenuTextColour[0] = *text - TEXT_FE_COLOR_0;
                    text++;
                    continue;
                } else if (*text == TEXT_FE_BLINK) {
                    g_MenuTextBlink ^= 1;
                    text++;
                    continue;
                } else if (*text == TEXT_FE_RAINBOW) {
                    g_MenuTextRainbow ^= 1;
                    text++;
                    continue;
                } else if (*text == TEXT_FE_FIXED_PITCH) {
                    g_MenuTextFixedPitch ^= 1;
                    text++;
                    continue;
                }
            }
            break;
        default:
            if (*text >= TEXT_WINDOW_ICON_FIRST && *text <= TEXT_WINDOW_ICON_LAST) {
                switch (*text - TEXT_WINDOW_ICON_FIRST) {
                case TEXT_ICON_OFFSET_CIRCLE:
                    f.atlasX = 96;
                    f.atlasY = 64;
                    break;
                case TEXT_ICON_OFFSET_TRIANGLE:
                    f.atlasX = 144;
                    f.atlasY = 64;
                    break;
                case TEXT_ICON_OFFSET_SQUARE:
                    f.atlasX = 128;
                    f.atlasY = 64;
                    break;
                case TEXT_ICON_OFFSET_CROSS:
                    f.atlasX = 112;
                    f.atlasY = 64;
                    break;
                }
                if (AcquireBatchNode(1, g_MenuWinBlend4GraphicsObject)) {
                    f.texU = f.atlasX * 0.00390625f;
                    f.texV = f.atlasY * 0.00390625f;
                    f.du = 0.0625f;
                    f.dv = 0.0625f;
                    f.vtx0 = g_MenuWinBlend4GraphicsObject->vertices;
                    f.vtx0->x = (float)penX;
                    f.vtx0->y = (float)penY - 2.0f;
                    f.vtx0->z = z;
                    f.vtx0->rhwBits = FLOAT_ONE_BITS;
                    f.vtx0->color = 0xffffffff;
                    f.vtx0->specular = 0xff000000;
                    f.vtx0->u = f.texU;
                    f.vtx0->v = f.texV;
                    f.vtx1 = (GfxTlVertex *)(g_MenuWinBlend4GraphicsObject->vertexCursor + 32);
                    f.vtx1->x = (float)penX;
                    f.vtx1->y = (float)penY - 2.0f + 16.0f;
                    f.vtx1->z = z;
                    f.vtx1->rhwBits = FLOAT_ONE_BITS;
                    f.vtx1->color = 0xffffffff;
                    f.vtx1->specular = 0xff000000;
                    f.vtx1->u = f.texU;
                    f.vtx1->v = f.texV + f.dv;
                    f.vtx2 = (GfxTlVertex *)(g_MenuWinBlend4GraphicsObject->vertexCursor + 64);
                    f.vtx2->x = (float)penX + 16.0f;
                    f.vtx2->y = (float)penY - 2.0f;
                    f.vtx2->z = z;
                    f.vtx2->rhwBits = FLOAT_ONE_BITS;
                    f.vtx2->color = 0xffffffff;
                    f.vtx2->specular = 0xff000000;
                    f.vtx2->u = f.texU + f.du;
                    f.vtx2->v = f.texV;
                    f.vtx3 = (GfxTlVertex *)(g_MenuWinBlend4GraphicsObject->vertexCursor + 96);
                    f.vtx3->x = (float)penX + 16.0f;
                    f.vtx3->y = (float)penY - 2.0f + 16.0f;
                    f.vtx3->z = z;
                    f.vtx3->rhwBits = FLOAT_ONE_BITS;
                    f.vtx3->color = 0xffffffff;
                    f.vtx3->specular = 0xff000000;
                    f.vtx3->u = f.texU + f.du;
                    f.vtx3->v = f.texV + f.dv;
                    *g_MenuWinBlend4GraphicsObject->colorBytePtr = 10;
                    g_MenuWinBlend4GraphicsObject->texturePage = 10;
                }
                g_MenuDrawTextBoxes = 1;
                text++;
                g_MenuTextRemaining--;
                g_MenuTextColumn++;
                penX += 16;
                continue;
            }
            f.offset = 0;
            f.atlasY = 0;
        }
        if (!g_MenuTextBlink && !g_MenuTextRainbow)
            f.colour = g_MenuTextColour[0];
        else if (g_MenuTextRainbow)
            f.colour = ((g_MenuTextTick >> 2) - f.phase) & 7;
        else if ((g_MenuTextTick >> 2) & 1)
            f.colour = g_MenuTextColour[0];
        else {
            if (!g_MenuTextColour[0]) {
                penX += f.skip;
                continue;
            }
            f.colour = 0;
        }
        f.code = *text;
        f.atlasX = (f.code % 21) * 12;
        f.atlasY += (f.code / 21) * 12;
        if (penX - g_MenuMessageX - g_MenuViewportX + (TEXT_METRIC >> 5) + (TEXT_METRIC & 31) > wrapWidth) {
            penX = g_MenuMessageX + g_MenuViewportX + 8;
            penY += 16;
            g_MenuTextRow++;
        }
        if (!g_MenuTextFixedPitch)
            penX += TEXT_METRIC >> 5;
        if (AcquireBatchNode(1, g_MenuFontBlend4GraphicsObject)) {
            f.texU = f.atlasX * 0.00390625f;
            f.texV = f.atlasY * 0.00390625f;
            f.dv = 0.046875f;
            *(int *)&f.du = *(int *)&f.dv;
            f.vtx4 = g_MenuFontBlend4GraphicsObject->vertices;
            f.vtx4->x = (float)penX;
            f.vtx4->y = (float)penY;
            f.vtx4->z = z;
            f.vtx4->rhwBits = FLOAT_ONE_BITS;
            f.vtx4->color = 0xffffffff;
            f.vtx4->specular = 0xff000000;
            *(int *)&f.vtx4->u = *(int *)&f.texU;
            *(int *)&f.vtx4->v = *(int *)&f.texV;
            f.vtx5 = (GfxTlVertex *)(g_MenuFontBlend4GraphicsObject->vertexCursor + 32);
            f.vtx5->x = (float)penX;
            f.vtx5->y = (float)penY + 12.0f;
            f.vtx5->z = z;
            f.vtx5->rhwBits = FLOAT_ONE_BITS;
            f.vtx5->color = 0xffffffff;
            f.vtx5->specular = 0xff000000;
            *(int *)&f.vtx5->u = *(int *)&f.texU;
            f.vtx5->v = f.texV + f.dv;
            f.vtx6 = (GfxTlVertex *)(g_MenuFontBlend4GraphicsObject->vertexCursor + 64);
            f.vtx6->x = (float)penX + 12.0f;
            f.vtx6->y = (float)penY;
            f.vtx6->z = z;
            f.vtx6->rhwBits = FLOAT_ONE_BITS;
            f.vtx6->color = 0xffffffff;
            f.vtx6->specular = 0xff000000;
            f.vtx6->u = f.texU + f.du;
            *(int *)&f.vtx6->v = *(int *)&f.texV;
            f.vtx7 = (GfxTlVertex *)(g_MenuFontBlend4GraphicsObject->vertexCursor + 96);
            f.vtx7->x = (float)penX + 12.0f;
            f.vtx7->y = (float)penY + 12.0f;
            f.vtx7->z = z;
            f.vtx7->rhwBits = FLOAT_ONE_BITS;
            f.vtx7->color = 0xffffffff;
            f.vtx7->specular = 0xff000000;
            f.vtx7->u = f.texU + f.du;
            f.vtx7->v = f.texV + f.dv;
            *(char *)g_MenuFontBlend4GraphicsObject->colorIndex = f.colour << 1;
            g_MenuFontBlend4GraphicsObject->paletteIndex = f.colour << 1;
            g_MenuDrawCharacter = 1;
        }
        if (!g_MenuTextFixedPitch)
            penX += TEXT_METRIC & 31;
        else
            penX += 13;
        g_MenuTextRemaining--;
        text++;
        g_MenuTextColumn++;
    }
    return penY;
#undef TEXT_METRIC
}

/*
 * A one-pixel rule from x0 to x1 at y for the 320x240 layout, both x ends and the
 * y shifted by the 1x origin first.
 */
// FUNCTION: FF7 0x006edd6f
void __cdecl Menu_DrawHorizontalRule1x(int x0, int x1, int y, int colour)
{
    if (Menu_IsTextSubsystemReady() == 0) {
        return;
    }
    x0 += g_MenuViewportX;
    x1 += g_MenuViewportX;
    y += g_MenuViewportY;
    Gfx_EmitFlatRect(x0, y, x1 - x0, 1, colour, 0, 0, (int)g_MenuFlatRectGraphicsObject);
}

/*
 * The 320x240 twin of Menu_FillRectShorts2x: the rectangle's origin is shifted by
 * the 1x layout's origin in the argument list.
 */
// FUNCTION: FF7 0x006eddca
void __cdecl Menu_FillRectShorts1x(short *rect, unsigned int colour, float z)
{
    if (Menu_IsTextSubsystemReady() == 0) {
        return;
    }
    Gfx_EmitFlatRect(rect[0] + g_MenuViewportX, rect[1] + g_MenuViewportY, rect[2], rect[3], colour, 0, z, (int)g_MenuFlatRectGraphicsObject);
}

/*
 * When the text subsystem is ready, draws a 1x window using signed-short rectangle
 * coordinates plus global X/Y offsets and supplied depth. Independently acquires
 * two horizontal atlas edge quads when width exceeds 8, two vertical edges when
 * height exceeds 8, and four 4 by 4 corners unconditionally; vertices use white
 * primary/black secondary color, reciprocal W 1 and palette 0.
 */
// FUNCTION: FF7 0x006ede1d
void __cdecl Menu_DrawWindowFrame1x(EnglishWindowFrame *frame, float depth)
{
    int frameBaseX;
    GfxTlVertex *vertTopRight;
    GfxTlVertex *auxVertexTopEnd;
    GfxTlVertex *rawVertexTopLow;
    GfxTlVertex *vtxAPtr;
    GfxTlVertex *vtxLeftLowerPtr;
    GfxTlVertex *wndVtxLeftTop;
    GfxTlVertex *pVertexRightBottomEdge;
    GfxTlVertex *vtxBottomRightCornerPtr;
    GfxTlVertex *vtxBottomRightMid;
    GfxTlVertex *windowVertBrLow;
    int contentW;
    float vTextureStepY;
    float texDu;
    float vTextureBase;
    GfxTlVertex *v;
    GfxTlVertex *f;
    GfxTlVertex *vtxRightTop;
    GfxTlVertex *cornerBrVertex;
    GfxTlVertex *wndVtxBlLower;
    GfxTlVertex *windowVertexBlMid;
    GfxTlVertex *windowBgVertexLbPtr;
    GfxTlVertex *windowBgVertexPtr;
    int topY;
    GfxTlVertex *srcVertexRightBottom;
    GfxTlVertex *vtxBottomEnd;
    float textureU;
    GfxTlVertex *pBottomEdgeVertex;
    GfxTlVertex *cornerVertexBlB;
    GfxTlVertex *leftBottomVertexPtr;
    GfxTlVertex *vtxTopRightLowPtr;
    GfxTlVertex *winTopRightMid;
    GfxTlVertex *blendBgVertexBr;
    GfxTlVertex *vertexBottomStart;
    GfxTlVertex *cornerVertexTrBPtr;
    GfxTlVertex *winTrPtrPtr;
    GfxTlVertex *wndVtxCornerTlLow;
    GfxTlVertex *wndVtxTlMid;
    GfxTlVertex *blendVertexBl;
    GfxTlVertex *winBgVertexRb;
    GfxTlVertex *wndVtxCornerTlBPtr;
    GfxTlVertex *blendVertexB;
    int contentTop;
    GfxTlVertex *wndVtxCornerTl;
    GfxTlVertex *blendVertexTlPtr;
    GfxTlVertex *windowBgVertexBl;
    GfxTlVertex *pWndVtxLeftLower;
    GfxTlVertex *wndVtxLeftMidPtr;
    int innerRemaining;
    int bodyH;

    if (Menu_IsTextSubsystemReady() == 0) {
        return;
    }
    frameBaseX = frame->x + g_MenuViewportX;
    topY = frame->y + g_MenuViewportY;
    if (frame->width > 8) {
        if (AcquireBatchNode(2, g_MenuWinCBlend4DiffGraphicsObject) != 0) {
            textureU = 0.0f;
            vTextureBase = 0.875f;
            texDu = 0.03125f;
            vTextureStepY = 0.015625f;
            vtxAPtr = g_MenuWinCBlend4DiffGraphicsObject->vertices;
            vtxAPtr->x = (float)frameBaseX + 4.0f;
            vtxAPtr->y = (float)topY;
            vtxAPtr->z = depth;
            vtxAPtr->rhwBits = FLOAT_ONE_BITS;
            vtxAPtr->color = 0xFFFFFFFF;
            vtxAPtr->specular = 0xFF000000;
            vtxAPtr->u = textureU;
            vtxAPtr->v = vTextureBase;
            rawVertexTopLow = g_MenuWinCBlend4DiffGraphicsObject->vertices + 1;
            rawVertexTopLow->x = (float)frameBaseX + 4.0f;
            rawVertexTopLow->y = (float)topY + 4.0f;
            rawVertexTopLow->z = depth;
            rawVertexTopLow->rhwBits = FLOAT_ONE_BITS;
            rawVertexTopLow->color = 0xFFFFFFFF;
            rawVertexTopLow->specular = 0xFF000000;
            rawVertexTopLow->u = textureU;
            rawVertexTopLow->v = vTextureBase + vTextureStepY;
            auxVertexTopEnd = g_MenuWinCBlend4DiffGraphicsObject->vertices + 2;
            auxVertexTopEnd->x = (float)frameBaseX + 4.0f + frame->width - 8.0f;
            auxVertexTopEnd->y = (float)topY;
            auxVertexTopEnd->z = depth;
            auxVertexTopEnd->rhwBits = FLOAT_ONE_BITS;
            auxVertexTopEnd->color = 0xFFFFFFFF;
            auxVertexTopEnd->specular = 0xFF000000;
            auxVertexTopEnd->u = textureU + texDu;
            auxVertexTopEnd->v = vTextureBase;
            vertTopRight = g_MenuWinCBlend4DiffGraphicsObject->vertices + 3;
            vertTopRight->x = (float)frameBaseX + 4.0f + frame->width - 8.0f;
            vertTopRight->y = (float)topY + 4.0f;
            vertTopRight->z = depth;
            vertTopRight->rhwBits = FLOAT_ONE_BITS;
            vertTopRight->color = 0xFFFFFFFF;
            vertTopRight->specular = 0xFF000000;
            vertTopRight->u = textureU + texDu;
            vertTopRight->v = vTextureBase + vTextureStepY;
            *g_MenuWinCBlend4DiffGraphicsObject->colorSignedBytePtr = 0;
            g_MenuWinCBlend4DiffGraphicsObject->paletteIndex = 0;
            g_MenuWinCBlend4DiffGraphicsObject->vertexCursor += g_MenuWinCBlend4DiffGraphicsObject->cursorStep;
            g_MenuWinCBlend4DiffGraphicsObject->colorIndex++;
            textureU = 0.0625f;
            vTextureBase = 0.921875f;
            texDu = 0.03125f;
            vTextureStepY = 0.015625f;
            vertexBottomStart = g_MenuWinCBlend4DiffGraphicsObject->vertices;
            vertexBottomStart->x = (float)frameBaseX + 4.0f;
            vertexBottomStart->y = (float)topY + frame->height - 4.0f;
            vertexBottomStart->z = depth;
            vertexBottomStart->rhwBits = FLOAT_ONE_BITS;
            vertexBottomStart->color = 0xFFFFFFFF;
            vertexBottomStart->specular = 0xFF000000;
            vertexBottomStart->u = textureU;
            vertexBottomStart->v = vTextureBase;
            pBottomEdgeVertex = g_MenuWinCBlend4DiffGraphicsObject->vertices + 1;
            pBottomEdgeVertex->x = (float)frameBaseX + 4.0f;
            pBottomEdgeVertex->y = (float)topY + frame->height - 4.0f + 4.0f;
            pBottomEdgeVertex->z = depth;
            pBottomEdgeVertex->rhwBits = FLOAT_ONE_BITS;
            pBottomEdgeVertex->color = 0xFFFFFFFF;
            pBottomEdgeVertex->specular = 0xFF000000;
            pBottomEdgeVertex->u = textureU;
            pBottomEdgeVertex->v = vTextureBase + vTextureStepY;
            vtxBottomEnd = g_MenuWinCBlend4DiffGraphicsObject->vertices + 2;
            vtxBottomEnd->x = (float)frameBaseX + 4.0f + frame->width - 8.0f;
            vtxBottomEnd->y = (float)topY + frame->height - 4.0f;
            vtxBottomEnd->z = depth;
            vtxBottomEnd->rhwBits = FLOAT_ONE_BITS;
            vtxBottomEnd->color = 0xFFFFFFFF;
            vtxBottomEnd->specular = 0xFF000000;
            vtxBottomEnd->u = textureU + texDu;
            vtxBottomEnd->v = vTextureBase;
            srcVertexRightBottom = g_MenuWinCBlend4DiffGraphicsObject->vertices + 3;
            srcVertexRightBottom->x = (float)frameBaseX + 4.0f + frame->width - 8.0f;
            srcVertexRightBottom->y = (float)topY + frame->height - 4.0f + 4.0f;
            srcVertexRightBottom->z = depth;
            srcVertexRightBottom->rhwBits = FLOAT_ONE_BITS;
            srcVertexRightBottom->color = 0xFFFFFFFF;
            srcVertexRightBottom->specular = 0xFF000000;
            srcVertexRightBottom->u = textureU + texDu;
            srcVertexRightBottom->v = vTextureBase + vTextureStepY;
            *g_MenuWinCBlend4DiffGraphicsObject->colorSignedBytePtr = 0;
            g_MenuWinCBlend4DiffGraphicsObject->paletteIndex = 0;
        }
    }
    if (frame->height > 8) {
        if (AcquireBatchNode(2, g_MenuWinCBlend4DiffGraphicsObject) != 0) {
            textureU = 0.109375f;
            vTextureBase = 0.9375f;
            texDu = 0.015625f;
            vTextureStepY = 0.03125f;
            vtxRightTop = g_MenuWinCBlend4DiffGraphicsObject->vertices;
            vtxRightTop->x = (float)frameBaseX + frame->width - 4.0f;
            vtxRightTop->y = (float)topY + 4.0f;
            vtxRightTop->z = depth;
            vtxRightTop->rhwBits = FLOAT_ONE_BITS;
            vtxRightTop->color = 0xFFFFFFFF;
            vtxRightTop->specular = 0xFF000000;
            vtxRightTop->u = textureU;
            vtxRightTop->v = vTextureBase;
            f = g_MenuWinCBlend4DiffGraphicsObject->vertices + 1;
            f->x = (float)frameBaseX + frame->width - 4.0f;
            f->y = (float)topY + 4.0f + frame->height - 8.0f;
            f->z = depth;
            f->rhwBits = FLOAT_ONE_BITS;
            f->color = 0xFFFFFFFF;
            f->specular = 0xFF000000;
            f->u = textureU;
            f->v = vTextureBase + vTextureStepY;
            v = g_MenuWinCBlend4DiffGraphicsObject->vertices + 2;
            v->x = (float)frameBaseX + frame->width - 4.0f + 4.0f;
            v->y = (float)topY + 4.0f;
            v->z = depth;
            v->rhwBits = FLOAT_ONE_BITS;
            v->color = 0xFFFFFFFF;
            v->specular = 0xFF000000;
            v->u = textureU + texDu;
            v->v = vTextureBase;
            pVertexRightBottomEdge = g_MenuWinCBlend4DiffGraphicsObject->vertices + 3;
            pVertexRightBottomEdge->x = (float)frameBaseX + frame->width - 4.0f + 4.0f;
            pVertexRightBottomEdge->y = (float)topY + 4.0f + frame->height - 8.0f;
            pVertexRightBottomEdge->z = depth;
            pVertexRightBottomEdge->rhwBits = FLOAT_ONE_BITS;
            pVertexRightBottomEdge->color = 0xFFFFFFFF;
            pVertexRightBottomEdge->specular = 0xFF000000;
            pVertexRightBottomEdge->u = textureU + texDu;
            pVertexRightBottomEdge->v = vTextureBase + vTextureStepY;
            *g_MenuWinCBlend4DiffGraphicsObject->colorSignedBytePtr = 0;
            g_MenuWinCBlend4DiffGraphicsObject->paletteIndex = 0;
            g_MenuWinCBlend4DiffGraphicsObject->vertexCursor += g_MenuWinCBlend4DiffGraphicsObject->cursorStep;
            g_MenuWinCBlend4DiffGraphicsObject->colorIndex++;
            textureU = 0.0f;
            vTextureBase = 0.921875f;
            texDu = 0.015625f;
            vTextureStepY = 0.03125f;
            wndVtxLeftTop = g_MenuWinCBlend4DiffGraphicsObject->vertices;
            wndVtxLeftTop->x = (float)frameBaseX;
            wndVtxLeftTop->y = (float)topY + 4.0f;
            wndVtxLeftTop->z = depth;
            wndVtxLeftTop->rhwBits = FLOAT_ONE_BITS;
            wndVtxLeftTop->color = 0xFFFFFFFF;
            wndVtxLeftTop->specular = 0xFF000000;
            wndVtxLeftTop->u = textureU;
            wndVtxLeftTop->v = vTextureBase;
            vtxLeftLowerPtr = g_MenuWinCBlend4DiffGraphicsObject->vertices + 1;
            vtxLeftLowerPtr->x = (float)frameBaseX;
            vtxLeftLowerPtr->y = (float)topY + 4.0f + frame->height - 8.0f;
            vtxLeftLowerPtr->z = depth;
            vtxLeftLowerPtr->rhwBits = FLOAT_ONE_BITS;
            vtxLeftLowerPtr->color = 0xFFFFFFFF;
            vtxLeftLowerPtr->specular = 0xFF000000;
            vtxLeftLowerPtr->u = textureU;
            vtxLeftLowerPtr->v = vTextureBase + vTextureStepY;
            wndVtxLeftMidPtr = g_MenuWinCBlend4DiffGraphicsObject->vertices + 2;
            wndVtxLeftMidPtr->x = (float)frameBaseX + 4.0f;
            wndVtxLeftMidPtr->y = (float)topY + 4.0f;
            wndVtxLeftMidPtr->z = depth;
            wndVtxLeftMidPtr->rhwBits = FLOAT_ONE_BITS;
            wndVtxLeftMidPtr->color = 0xFFFFFFFF;
            wndVtxLeftMidPtr->specular = 0xFF000000;
            wndVtxLeftMidPtr->u = textureU + texDu;
            wndVtxLeftMidPtr->v = vTextureBase;
            pWndVtxLeftLower = g_MenuWinCBlend4DiffGraphicsObject->vertices + 3;
            pWndVtxLeftLower->x = (float)frameBaseX + 4.0f;
            pWndVtxLeftLower->y = (float)topY + 4.0f + frame->height - 8.0f;
            pWndVtxLeftLower->z = depth;
            pWndVtxLeftLower->rhwBits = FLOAT_ONE_BITS;
            pWndVtxLeftLower->color = 0xFFFFFFFF;
            pWndVtxLeftLower->specular = 0xFF000000;
            pWndVtxLeftLower->u = textureU + texDu;
            pWndVtxLeftLower->v = vTextureBase + vTextureStepY;
            *g_MenuWinCBlend4DiffGraphicsObject->colorSignedBytePtr = 0;
            g_MenuWinCBlend4DiffGraphicsObject->paletteIndex = 0;
        }
    }
    if (AcquireBatchNode(4, g_MenuWinCBlend4DiffGraphicsObject) != 0) {
        vTextureStepY = 0.015625f;
        texDu = vTextureStepY;
        textureU = 0.0f;
        vTextureBase = 0.90625f;
        wndVtxCornerTl = g_MenuWinCBlend4DiffGraphicsObject->vertices;
        wndVtxCornerTl->x = (float)frameBaseX;
        wndVtxCornerTl->y = (float)topY;
        wndVtxCornerTl->z = depth;
        wndVtxCornerTl->rhwBits = FLOAT_ONE_BITS;
        wndVtxCornerTl->color = 0xFFFFFFFF;
        wndVtxCornerTl->specular = 0xFF000000;
        wndVtxCornerTl->u = textureU;
        wndVtxCornerTl->v = vTextureBase;
        wndVtxCornerTlBPtr = g_MenuWinCBlend4DiffGraphicsObject->vertices + 1;
        wndVtxCornerTlBPtr->x = (float)frameBaseX;
        wndVtxCornerTlBPtr->y = (float)topY + 4.0f;
        wndVtxCornerTlBPtr->z = depth;
        wndVtxCornerTlBPtr->rhwBits = FLOAT_ONE_BITS;
        wndVtxCornerTlBPtr->color = 0xFFFFFFFF;
        wndVtxCornerTlBPtr->specular = 0xFF000000;
        wndVtxCornerTlBPtr->u = textureU;
        wndVtxCornerTlBPtr->v = vTextureBase + vTextureStepY;
        wndVtxTlMid = g_MenuWinCBlend4DiffGraphicsObject->vertices + 2;
        wndVtxTlMid->x = (float)frameBaseX + 4.0f;
        wndVtxTlMid->y = (float)topY;
        wndVtxTlMid->z = depth;
        wndVtxTlMid->rhwBits = FLOAT_ONE_BITS;
        wndVtxTlMid->color = 0xFFFFFFFF;
        wndVtxTlMid->specular = 0xFF000000;
        wndVtxTlMid->u = textureU + texDu;
        wndVtxTlMid->v = vTextureBase;
        wndVtxCornerTlLow = g_MenuWinCBlend4DiffGraphicsObject->vertices + 3;
        wndVtxCornerTlLow->x = (float)frameBaseX + 4.0f;
        wndVtxCornerTlLow->y = (float)topY + 4.0f;
        wndVtxCornerTlLow->z = depth;
        wndVtxCornerTlLow->rhwBits = FLOAT_ONE_BITS;
        wndVtxCornerTlLow->color = 0xFFFFFFFF;
        wndVtxCornerTlLow->specular = 0xFF000000;
        wndVtxCornerTlLow->u = textureU + texDu;
        wndVtxCornerTlLow->v = vTextureBase + vTextureStepY;
        *g_MenuWinCBlend4DiffGraphicsObject->colorSignedBytePtr = 0;
        g_MenuWinCBlend4DiffGraphicsObject->paletteIndex = 0;
        g_MenuWinCBlend4DiffGraphicsObject->vertexCursor += g_MenuWinCBlend4DiffGraphicsObject->cursorStep;
        g_MenuWinCBlend4DiffGraphicsObject->colorIndex++;
        textureU = 0.046875f;
        winTrPtrPtr = g_MenuWinCBlend4DiffGraphicsObject->vertices;
        winTrPtrPtr->x = (float)frameBaseX + frame->width - 4.0f;
        winTrPtrPtr->y = (float)topY;
        winTrPtrPtr->z = depth;
        winTrPtrPtr->rhwBits = FLOAT_ONE_BITS;
        winTrPtrPtr->color = 0xFFFFFFFF;
        winTrPtrPtr->specular = 0xFF000000;
        winTrPtrPtr->u = textureU;
        winTrPtrPtr->v = vTextureBase;
        cornerVertexTrBPtr = g_MenuWinCBlend4DiffGraphicsObject->vertices + 1;
        cornerVertexTrBPtr->x = (float)frameBaseX + frame->width - 4.0f;
        cornerVertexTrBPtr->y = (float)topY + 4.0f;
        cornerVertexTrBPtr->z = depth;
        cornerVertexTrBPtr->rhwBits = FLOAT_ONE_BITS;
        cornerVertexTrBPtr->color = 0xFFFFFFFF;
        cornerVertexTrBPtr->specular = 0xFF000000;
        cornerVertexTrBPtr->u = textureU;
        cornerVertexTrBPtr->v = vTextureBase + vTextureStepY;
        winTopRightMid = g_MenuWinCBlend4DiffGraphicsObject->vertices + 2;
        winTopRightMid->x = (float)frameBaseX + frame->width - 4.0f + 4.0f;
        winTopRightMid->y = (float)topY;
        winTopRightMid->z = depth;
        winTopRightMid->rhwBits = FLOAT_ONE_BITS;
        winTopRightMid->color = 0xFFFFFFFF;
        winTopRightMid->specular = 0xFF000000;
        winTopRightMid->u = textureU + texDu;
        winTopRightMid->v = vTextureBase;
        vtxTopRightLowPtr = g_MenuWinCBlend4DiffGraphicsObject->vertices + 3;
        vtxTopRightLowPtr->x = (float)frameBaseX + frame->width - 4.0f + 4.0f;
        vtxTopRightLowPtr->y = (float)topY + 4.0f;
        vtxTopRightLowPtr->z = depth;
        vtxTopRightLowPtr->rhwBits = FLOAT_ONE_BITS;
        vtxTopRightLowPtr->color = 0xFFFFFFFF;
        vtxTopRightLowPtr->specular = 0xFF000000;
        vtxTopRightLowPtr->u = textureU + texDu;
        vtxTopRightLowPtr->v = vTextureBase + vTextureStepY;
        *g_MenuWinCBlend4DiffGraphicsObject->colorSignedBytePtr = 0;
        g_MenuWinCBlend4DiffGraphicsObject->paletteIndex = 0;
        g_MenuWinCBlend4DiffGraphicsObject->vertexCursor += g_MenuWinCBlend4DiffGraphicsObject->cursorStep;
        g_MenuWinCBlend4DiffGraphicsObject->colorIndex++;
        textureU = 0.0625f;
        vTextureBase = 0.890625f;
        leftBottomVertexPtr = g_MenuWinCBlend4DiffGraphicsObject->vertices;
        leftBottomVertexPtr->x = (float)frameBaseX;
        leftBottomVertexPtr->y = (float)topY + frame->height - 4.0f;
        leftBottomVertexPtr->z = depth;
        leftBottomVertexPtr->rhwBits = FLOAT_ONE_BITS;
        leftBottomVertexPtr->color = 0xFFFFFFFF;
        leftBottomVertexPtr->specular = 0xFF000000;
        leftBottomVertexPtr->u = textureU;
        leftBottomVertexPtr->v = vTextureBase;
        cornerVertexBlB = g_MenuWinCBlend4DiffGraphicsObject->vertices + 1;
        cornerVertexBlB->x = (float)frameBaseX;
        cornerVertexBlB->y = (float)topY + frame->height - 4.0f + 4.0f;
        cornerVertexBlB->z = depth;
        cornerVertexBlB->rhwBits = FLOAT_ONE_BITS;
        cornerVertexBlB->color = 0xFFFFFFFF;
        cornerVertexBlB->specular = 0xFF000000;
        cornerVertexBlB->u = textureU;
        cornerVertexBlB->v = vTextureBase + vTextureStepY;
        windowVertexBlMid = g_MenuWinCBlend4DiffGraphicsObject->vertices + 2;
        windowVertexBlMid->x = (float)frameBaseX + 4.0f;
        windowVertexBlMid->y = (float)topY + frame->height - 4.0f;
        windowVertexBlMid->z = depth;
        windowVertexBlMid->rhwBits = FLOAT_ONE_BITS;
        windowVertexBlMid->color = 0xFFFFFFFF;
        windowVertexBlMid->specular = 0xFF000000;
        windowVertexBlMid->u = textureU + texDu;
        windowVertexBlMid->v = vTextureBase;
        wndVtxBlLower = g_MenuWinCBlend4DiffGraphicsObject->vertices + 3;
        wndVtxBlLower->x = (float)frameBaseX + 4.0f;
        wndVtxBlLower->y = (float)topY + frame->height - 4.0f + 4.0f;
        wndVtxBlLower->z = depth;
        wndVtxBlLower->rhwBits = FLOAT_ONE_BITS;
        wndVtxBlLower->color = 0xFFFFFFFF;
        wndVtxBlLower->specular = 0xFF000000;
        wndVtxBlLower->u = textureU + texDu;
        wndVtxBlLower->v = vTextureBase + vTextureStepY;
        *g_MenuWinCBlend4DiffGraphicsObject->colorSignedBytePtr = 0;
        g_MenuWinCBlend4DiffGraphicsObject->paletteIndex = 0;
        g_MenuWinCBlend4DiffGraphicsObject->vertexCursor += g_MenuWinCBlend4DiffGraphicsObject->cursorStep;
        g_MenuWinCBlend4DiffGraphicsObject->colorIndex++;
        textureU = 0.109375f;
        cornerBrVertex = g_MenuWinCBlend4DiffGraphicsObject->vertices;
        cornerBrVertex->x = (float)frameBaseX + frame->width - 4.0f;
        cornerBrVertex->y = (float)topY + frame->height - 4.0f;
        cornerBrVertex->z = depth;
        cornerBrVertex->rhwBits = FLOAT_ONE_BITS;
        cornerBrVertex->color = 0xFFFFFFFF;
        cornerBrVertex->specular = 0xFF000000;
        cornerBrVertex->u = textureU;
        cornerBrVertex->v = vTextureBase;
        windowVertBrLow = g_MenuWinCBlend4DiffGraphicsObject->vertices + 1;
        windowVertBrLow->x = (float)frameBaseX + frame->width - 4.0f;
        windowVertBrLow->y = (float)topY + frame->height - 4.0f + 4.0f;
        windowVertBrLow->z = depth;
        windowVertBrLow->rhwBits = FLOAT_ONE_BITS;
        windowVertBrLow->color = 0xFFFFFFFF;
        windowVertBrLow->specular = 0xFF000000;
        windowVertBrLow->u = textureU;
        windowVertBrLow->v = vTextureBase + vTextureStepY;
        vtxBottomRightMid = g_MenuWinCBlend4DiffGraphicsObject->vertices + 2;
        vtxBottomRightMid->x = (float)frameBaseX + frame->width - 4.0f + 4.0f;
        vtxBottomRightMid->y = (float)topY + frame->height - 4.0f;
        vtxBottomRightMid->z = depth;
        vtxBottomRightMid->rhwBits = FLOAT_ONE_BITS;
        vtxBottomRightMid->color = 0xFFFFFFFF;
        vtxBottomRightMid->specular = 0xFF000000;
        vtxBottomRightMid->u = textureU + texDu;
        vtxBottomRightMid->v = vTextureBase;
        vtxBottomRightCornerPtr = g_MenuWinCBlend4DiffGraphicsObject->vertices + 3;
        vtxBottomRightCornerPtr->x = (float)frameBaseX + frame->width - 4.0f + 4.0f;
        vtxBottomRightCornerPtr->y = (float)topY + frame->height - 4.0f + 4.0f;
        vtxBottomRightCornerPtr->z = depth;
        vtxBottomRightCornerPtr->rhwBits = FLOAT_ONE_BITS;
        vtxBottomRightCornerPtr->color = 0xFFFFFFFF;
        vtxBottomRightCornerPtr->specular = 0xFF000000;
        vtxBottomRightCornerPtr->u = textureU + texDu;
        vtxBottomRightCornerPtr->v = vTextureBase + vTextureStepY;
        *g_MenuWinCBlend4DiffGraphicsObject->colorSignedBytePtr = 0;
        g_MenuWinCBlend4DiffGraphicsObject->paletteIndex = 0;
    }
    if (frame->width > 6 && frame->height > 6) {
        innerRemaining = frameBaseX + 3;
        contentTop = topY + 3;
        contentW = frame->width - 6;
        bodyH = frame->height - 6;
        if (g_MenuMessageWindowActive != 0) {
            if (AcquireBatchNode(1, g_MenuBlendWindowBgGraphicsObject) != 0) {
                blendVertexTlPtr = g_MenuBlendWindowBgGraphicsObject->vertices;
                blendVertexTlPtr->x = (float)innerRemaining;
                blendVertexTlPtr->y = (float)contentTop;
                blendVertexTlPtr->z = depth + 0.01f;
                blendVertexTlPtr->rhwBits = FLOAT_ONE_BITS;
                blendVertexTlPtr->color = *(int *)&g_MenuCornerColorTL;
                blendVertexTlPtr->specular = 0xFF000000;
                blendVertexB = g_MenuBlendWindowBgGraphicsObject->vertices + 1;
                blendVertexB->x = (float)innerRemaining;
                blendVertexB->y = (float)contentTop + bodyH;
                blendVertexB->z = depth + 0.01f;
                blendVertexB->rhwBits = FLOAT_ONE_BITS;
                blendVertexB->color = *(int *)&g_MenuCornerColorTR;
                blendVertexB->specular = 0xFF000000;
                blendVertexBl = g_MenuBlendWindowBgGraphicsObject->vertices + 2;
                blendVertexBl->x = (float)innerRemaining + contentW;
                blendVertexBl->y = (float)contentTop;
                blendVertexBl->z = depth + 0.01f;
                blendVertexBl->rhwBits = FLOAT_ONE_BITS;
                blendVertexBl->color = *(int *)&g_MenuCornerColorBL;
                blendVertexBl->specular = 0xFF000000;
                blendBgVertexBr = g_MenuBlendWindowBgGraphicsObject->vertices + 3;
                blendBgVertexBr->x = (float)innerRemaining + contentW;
                blendBgVertexBr->y = (float)contentTop + bodyH;
                blendBgVertexBr->z = depth + 0.01f;
                blendBgVertexBr->rhwBits = FLOAT_ONE_BITS;
                blendBgVertexBr->color = *(int *)&g_MenuCornerColorBR;
                blendBgVertexBr->specular = 0xFF000000;
            }
        } else if (AcquireBatchNode(1, g_MenuWindowBgGraphicsObject) != 0) {
            windowBgVertexPtr = g_MenuWindowBgGraphicsObject->vertices;
            windowBgVertexPtr->x = (float)innerRemaining;
            windowBgVertexPtr->y = (float)contentTop;
            windowBgVertexPtr->z = depth + 0.01f;
            windowBgVertexPtr->rhwBits = FLOAT_ONE_BITS;
            windowBgVertexPtr->color = *(int *)&g_MenuCornerColorTL;
            windowBgVertexPtr->specular = 0xFF000000;
            windowBgVertexLbPtr = g_MenuWindowBgGraphicsObject->vertices + 1;
            windowBgVertexLbPtr->x = (float)innerRemaining;
            windowBgVertexLbPtr->y = (float)contentTop + bodyH;
            windowBgVertexLbPtr->z = depth + 0.01f;
            windowBgVertexLbPtr->rhwBits = FLOAT_ONE_BITS;
            windowBgVertexLbPtr->color = *(int *)&g_MenuCornerColorTR;
            windowBgVertexLbPtr->specular = 0xFF000000;
            windowBgVertexBl = g_MenuWindowBgGraphicsObject->vertices + 2;
            windowBgVertexBl->x = (float)innerRemaining + contentW;
            windowBgVertexBl->y = (float)contentTop;
            windowBgVertexBl->z = depth + 0.01f;
            windowBgVertexBl->rhwBits = FLOAT_ONE_BITS;
            windowBgVertexBl->color = *(int *)&g_MenuCornerColorBL;
            windowBgVertexBl->specular = 0xFF000000;
            winBgVertexRb = g_MenuWindowBgGraphicsObject->vertices + 3;
            winBgVertexRb->x = (float)innerRemaining + contentW;
            winBgVertexRb->y = (float)contentTop + bodyH;
            winBgVertexRb->z = depth + 0.01f;
            winBgVertexRb->rhwBits = FLOAT_ONE_BITS;
            winBgVertexRb->color = *(int *)&g_MenuCornerColorBR;
            winBgVertexRb->specular = 0xFF000000;
        }
    }
}

/*
 * When the text subsystem is ready, calls Menu_PackWindowColor for four RGB triplets in
 * g_FieldWindowColours into frame-local slots and draws a 1x dialogue frame from
 * signed-short rectangle coordinates plus global X/Y offsets. Independently
 * acquires two horizontal atlas edges for width above 8, two vertical edges for
 * height above 8, and four 4 by 4 corners unconditionally, using white
 * primary/black secondary color, reciprocal W 1 and palette 0.
 */
// FUNCTION: FF7 0x006ef54a
void __cdecl Menu_DrawMessageWindowFrame1x(EnglishMessageWindowRect *windowRect, float depth)
{
    int innerWidth;
    GfxTlVertex *topEdgeTopLeft;
    float vDelta8;
    GfxTlVertex *leftEdgeInnerBottom;
    GfxTlVertex *leftEdgeThirdPtr;
    GfxTlVertex *leftEdgeOuterBPtr;
    GfxTlVertex *leftEdgeTopPtr;
    GfxTlVertex *tlCornerTopRightPtr;
    GfxTlVertex *trCornerTlPtr;
    GfxTlVertex *pBlendBgFirst;
    int screenY;
    float uWidth;
    float vOrigin;
    float uOrigin;
    GfxTlVertex *rightEdgeOuterBottomPtr;
    GfxTlVertex *rightEdgeOuterTopPtr;
    GfxTlVertex *v;
    GfxTlVertex *f;
    GfxTlVertex *brCornerBottomRight;
    GfxTlVertex *pBrCornerTopRight;
    GfxTlVertex *bgVertex1;
    GfxTlVertex *bgTl;
    int palette[4];
    GfxTlVertex *mainPBottomEdgeV;
    GfxTlVertex *resolvedBottomEdgeV;
    int ofs;
    GfxTlVertex *tempBottomEdgeBottomLeft;
    GfxTlVertex *tmpBrCornerBl;
    GfxTlVertex *brCornerTop;
    GfxTlVertex *bottomEdgeV0Ptr;
    GfxTlVertex *tlCornerBottomRight;
    GfxTlVertex *blendBgBottomRightPtr;
    GfxTlVertex *workTopEdgeVElem;
    GfxTlVertex *blCornerFourth;
    GfxTlVertex *cornerBottomLeftCPtr;
    GfxTlVertex *pTopEdgeTopRight;
    GfxTlVertex *tlCornerBottomRemaining;
    GfxTlVertex *pBlendBgTopRight;
    GfxTlVertex *rawBgVert;
    GfxTlVertex *blCornerBl;
    GfxTlVertex *stageBlCornerTopLeft;
    int contentTop;
    GfxTlVertex *tlCornerTopLeftPtr;
    GfxTlVertex *blendBgSecond;
    GfxTlVertex *bgTopRight;
    GfxTlVertex *pTrCornerBr;
    GfxTlVertex *pTrCornerTopRight;
    int frameH;
    int colorNumber;
    GfxTlVertex *pTopEdgeBottomLeft;
    GfxTlVertex *pCornerTopRightB;
    int messageX;
    int innerRemaining;

    if (Menu_IsTextSubsystemReady() == 0) {
        return;
    }
    for (colorNumber = 0; colorNumber < 4; colorNumber++) {
        ofs = colorNumber * 3;
        Menu_PackWindowColor((unsigned char *)((char *)palette + colorNumber * 4), g_FieldWindowColours[ofs], g_FieldWindowColours[ofs + 1], g_FieldWindowColours[ofs + 2]);
    }
    messageX = windowRect->posX + g_MenuViewportX;
    screenY = windowRect->posY + g_MenuViewportY;
    if (windowRect->width > 8) {
        if (AcquireBatchNode(2, g_MenuWinCBlend4DiffGraphicsObject) != 0) {
            uOrigin = 0.0f;
            vOrigin = 0.875f;
            uWidth = 0.03125f;
            vDelta8 = 0.015625f;
            topEdgeTopLeft = g_MenuWinCBlend4DiffGraphicsObject->vertices;
            topEdgeTopLeft->x = (float)messageX + 4.0f;
            topEdgeTopLeft->y = (float)screenY;
            topEdgeTopLeft->z = depth;
            topEdgeTopLeft->rhwBits = FLOAT_ONE_BITS;
            topEdgeTopLeft->color = 0xFFFFFFFF;
            topEdgeTopLeft->specular = 0xFF000000;
            topEdgeTopLeft->u = uOrigin;
            topEdgeTopLeft->v = vOrigin;
            pTopEdgeBottomLeft = g_MenuWinCBlend4DiffGraphicsObject->vertices + 1;
            pTopEdgeBottomLeft->x = (float)messageX + 4.0f;
            pTopEdgeBottomLeft->y = (float)screenY + 4.0f;
            pTopEdgeBottomLeft->z = depth;
            pTopEdgeBottomLeft->rhwBits = FLOAT_ONE_BITS;
            pTopEdgeBottomLeft->color = 0xFFFFFFFF;
            pTopEdgeBottomLeft->specular = 0xFF000000;
            pTopEdgeBottomLeft->u = uOrigin;
            pTopEdgeBottomLeft->v = vOrigin + vDelta8;
            pTopEdgeTopRight = g_MenuWinCBlend4DiffGraphicsObject->vertices + 2;
            pTopEdgeTopRight->x = (float)messageX + 4.0f + windowRect->width - 8.0f;
            pTopEdgeTopRight->y = (float)screenY;
            pTopEdgeTopRight->z = depth;
            pTopEdgeTopRight->rhwBits = FLOAT_ONE_BITS;
            pTopEdgeTopRight->color = 0xFFFFFFFF;
            pTopEdgeTopRight->specular = 0xFF000000;
            pTopEdgeTopRight->u = uOrigin + uWidth;
            pTopEdgeTopRight->v = vOrigin;
            workTopEdgeVElem = g_MenuWinCBlend4DiffGraphicsObject->vertices + 3;
            workTopEdgeVElem->x = (float)messageX + 4.0f + windowRect->width - 8.0f;
            workTopEdgeVElem->y = (float)screenY + 4.0f;
            workTopEdgeVElem->z = depth;
            workTopEdgeVElem->rhwBits = FLOAT_ONE_BITS;
            workTopEdgeVElem->color = 0xFFFFFFFF;
            workTopEdgeVElem->specular = 0xFF000000;
            workTopEdgeVElem->u = uOrigin + uWidth;
            workTopEdgeVElem->v = vOrigin + vDelta8;
            *g_MenuWinCBlend4DiffGraphicsObject->colorSignedBytePtr = 0;
            g_MenuWinCBlend4DiffGraphicsObject->paletteIndex = 0;
            g_MenuWinCBlend4DiffGraphicsObject->vertexCursor += g_MenuWinCBlend4DiffGraphicsObject->cursorStep;
            g_MenuWinCBlend4DiffGraphicsObject->colorIndex++;
            uOrigin = 0.0625f;
            vOrigin = 0.921875f;
            uWidth = 0.03125f;
            vDelta8 = 0.015625f;
            bottomEdgeV0Ptr = g_MenuWinCBlend4DiffGraphicsObject->vertices;
            bottomEdgeV0Ptr->x = (float)messageX + 4.0f;
            bottomEdgeV0Ptr->y = (float)screenY + windowRect->height - 4.0f;
            bottomEdgeV0Ptr->z = depth;
            bottomEdgeV0Ptr->rhwBits = FLOAT_ONE_BITS;
            bottomEdgeV0Ptr->color = 0xFFFFFFFF;
            bottomEdgeV0Ptr->specular = 0xFF000000;
            bottomEdgeV0Ptr->u = uOrigin;
            bottomEdgeV0Ptr->v = vOrigin;
            tempBottomEdgeBottomLeft = g_MenuWinCBlend4DiffGraphicsObject->vertices + 1;
            tempBottomEdgeBottomLeft->x = (float)messageX + 4.0f;
            tempBottomEdgeBottomLeft->y = (float)screenY + windowRect->height - 4.0f + 4.0f;
            tempBottomEdgeBottomLeft->z = depth;
            tempBottomEdgeBottomLeft->rhwBits = FLOAT_ONE_BITS;
            tempBottomEdgeBottomLeft->color = 0xFFFFFFFF;
            tempBottomEdgeBottomLeft->specular = 0xFF000000;
            tempBottomEdgeBottomLeft->u = uOrigin;
            tempBottomEdgeBottomLeft->v = vOrigin + vDelta8;
            resolvedBottomEdgeV = g_MenuWinCBlend4DiffGraphicsObject->vertices + 2;
            resolvedBottomEdgeV->x = (float)messageX + 4.0f + windowRect->width - 8.0f;
            resolvedBottomEdgeV->y = (float)screenY + windowRect->height - 4.0f;
            resolvedBottomEdgeV->z = depth;
            resolvedBottomEdgeV->rhwBits = FLOAT_ONE_BITS;
            resolvedBottomEdgeV->color = 0xFFFFFFFF;
            resolvedBottomEdgeV->specular = 0xFF000000;
            resolvedBottomEdgeV->u = uOrigin + uWidth;
            resolvedBottomEdgeV->v = vOrigin;
            mainPBottomEdgeV = g_MenuWinCBlend4DiffGraphicsObject->vertices + 3;
            mainPBottomEdgeV->x = (float)messageX + 4.0f + windowRect->width - 8.0f;
            mainPBottomEdgeV->y = (float)screenY + windowRect->height - 4.0f + 4.0f;
            mainPBottomEdgeV->z = depth;
            mainPBottomEdgeV->rhwBits = FLOAT_ONE_BITS;
            mainPBottomEdgeV->color = 0xFFFFFFFF;
            mainPBottomEdgeV->specular = 0xFF000000;
            mainPBottomEdgeV->u = uOrigin + uWidth;
            mainPBottomEdgeV->v = vOrigin + vDelta8;
            *g_MenuWinCBlend4DiffGraphicsObject->colorSignedBytePtr = 0;
            g_MenuWinCBlend4DiffGraphicsObject->paletteIndex = 0;
        }
    }
    if (windowRect->height > 8) {
        if (AcquireBatchNode(2, g_MenuWinCBlend4DiffGraphicsObject) != 0) {
            uOrigin = 0.109375f;
            vOrigin = 0.9375f;
            uWidth = 0.015625f;
            vDelta8 = 0.03125f;
            f = g_MenuWinCBlend4DiffGraphicsObject->vertices;
            f->x = (float)messageX + windowRect->width - 4.0f;
            f->y = (float)screenY + 4.0f;
            f->z = depth;
            f->rhwBits = FLOAT_ONE_BITS;
            f->color = 0xFFFFFFFF;
            f->specular = 0xFF000000;
            f->u = uOrigin;
            f->v = vOrigin;
            v = g_MenuWinCBlend4DiffGraphicsObject->vertices + 1;
            v->x = (float)messageX + windowRect->width - 4.0f;
            v->y = (float)screenY + 4.0f + windowRect->height - 8.0f;
            v->z = depth;
            v->rhwBits = FLOAT_ONE_BITS;
            v->color = 0xFFFFFFFF;
            v->specular = 0xFF000000;
            v->u = uOrigin;
            v->v = vOrigin + vDelta8;
            rightEdgeOuterTopPtr = g_MenuWinCBlend4DiffGraphicsObject->vertices + 2;
            rightEdgeOuterTopPtr->x = (float)messageX + windowRect->width - 4.0f + 4.0f;
            rightEdgeOuterTopPtr->y = (float)screenY + 4.0f;
            rightEdgeOuterTopPtr->z = depth;
            rightEdgeOuterTopPtr->rhwBits = FLOAT_ONE_BITS;
            rightEdgeOuterTopPtr->color = 0xFFFFFFFF;
            rightEdgeOuterTopPtr->specular = 0xFF000000;
            rightEdgeOuterTopPtr->u = uOrigin + uWidth;
            rightEdgeOuterTopPtr->v = vOrigin;
            rightEdgeOuterBottomPtr = g_MenuWinCBlend4DiffGraphicsObject->vertices + 3;
            rightEdgeOuterBottomPtr->x = (float)messageX + windowRect->width - 4.0f + 4.0f;
            rightEdgeOuterBottomPtr->y = (float)screenY + 4.0f + windowRect->height - 8.0f;
            rightEdgeOuterBottomPtr->z = depth;
            rightEdgeOuterBottomPtr->rhwBits = FLOAT_ONE_BITS;
            rightEdgeOuterBottomPtr->color = 0xFFFFFFFF;
            rightEdgeOuterBottomPtr->specular = 0xFF000000;
            rightEdgeOuterBottomPtr->u = uOrigin + uWidth;
            rightEdgeOuterBottomPtr->v = vOrigin + vDelta8;
            *g_MenuWinCBlend4DiffGraphicsObject->colorSignedBytePtr = 0;
            g_MenuWinCBlend4DiffGraphicsObject->paletteIndex = 0;
            g_MenuWinCBlend4DiffGraphicsObject->vertexCursor += g_MenuWinCBlend4DiffGraphicsObject->cursorStep;
            g_MenuWinCBlend4DiffGraphicsObject->colorIndex++;
            uOrigin = 0.0f;
            vOrigin = 0.921875f;
            uWidth = 0.015625f;
            vDelta8 = 0.03125f;
            leftEdgeTopPtr = g_MenuWinCBlend4DiffGraphicsObject->vertices;
            leftEdgeTopPtr->x = (float)messageX;
            leftEdgeTopPtr->y = (float)screenY + 4.0f;
            leftEdgeTopPtr->z = depth;
            leftEdgeTopPtr->rhwBits = FLOAT_ONE_BITS;
            leftEdgeTopPtr->color = 0xFFFFFFFF;
            leftEdgeTopPtr->specular = 0xFF000000;
            leftEdgeTopPtr->u = uOrigin;
            leftEdgeTopPtr->v = vOrigin;
            leftEdgeOuterBPtr = g_MenuWinCBlend4DiffGraphicsObject->vertices + 1;
            leftEdgeOuterBPtr->x = (float)messageX;
            leftEdgeOuterBPtr->y = (float)screenY + 4.0f + windowRect->height - 8.0f;
            leftEdgeOuterBPtr->z = depth;
            leftEdgeOuterBPtr->rhwBits = FLOAT_ONE_BITS;
            leftEdgeOuterBPtr->color = 0xFFFFFFFF;
            leftEdgeOuterBPtr->specular = 0xFF000000;
            leftEdgeOuterBPtr->u = uOrigin;
            leftEdgeOuterBPtr->v = vOrigin + vDelta8;
            leftEdgeThirdPtr = g_MenuWinCBlend4DiffGraphicsObject->vertices + 2;
            leftEdgeThirdPtr->x = (float)messageX + 4.0f;
            leftEdgeThirdPtr->y = (float)screenY + 4.0f;
            leftEdgeThirdPtr->z = depth;
            leftEdgeThirdPtr->rhwBits = FLOAT_ONE_BITS;
            leftEdgeThirdPtr->color = 0xFFFFFFFF;
            leftEdgeThirdPtr->specular = 0xFF000000;
            leftEdgeThirdPtr->u = uOrigin + uWidth;
            leftEdgeThirdPtr->v = vOrigin;
            leftEdgeInnerBottom = g_MenuWinCBlend4DiffGraphicsObject->vertices + 3;
            leftEdgeInnerBottom->x = (float)messageX + 4.0f;
            leftEdgeInnerBottom->y = (float)screenY + 4.0f + windowRect->height - 8.0f;
            leftEdgeInnerBottom->z = depth;
            leftEdgeInnerBottom->rhwBits = FLOAT_ONE_BITS;
            leftEdgeInnerBottom->color = 0xFFFFFFFF;
            leftEdgeInnerBottom->specular = 0xFF000000;
            leftEdgeInnerBottom->u = uOrigin + uWidth;
            leftEdgeInnerBottom->v = vOrigin + vDelta8;
            *g_MenuWinCBlend4DiffGraphicsObject->colorSignedBytePtr = 0;
            g_MenuWinCBlend4DiffGraphicsObject->paletteIndex = 0;
        }
    }
    if (AcquireBatchNode(4, g_MenuWinCBlend4DiffGraphicsObject) != 0) {
        vDelta8 = 0.015625f;
        uWidth = vDelta8;
        uOrigin = 0.0f;
        vOrigin = 0.90625f;
        tlCornerTopLeftPtr = g_MenuWinCBlend4DiffGraphicsObject->vertices;
        tlCornerTopLeftPtr->x = (float)messageX;
        tlCornerTopLeftPtr->y = (float)screenY;
        tlCornerTopLeftPtr->z = depth;
        tlCornerTopLeftPtr->rhwBits = FLOAT_ONE_BITS;
        tlCornerTopLeftPtr->color = 0xFFFFFFFF;
        tlCornerTopLeftPtr->specular = 0xFF000000;
        tlCornerTopLeftPtr->u = uOrigin;
        tlCornerTopLeftPtr->v = vOrigin;
        tlCornerBottomRemaining = g_MenuWinCBlend4DiffGraphicsObject->vertices + 1;
        tlCornerBottomRemaining->x = (float)messageX;
        tlCornerBottomRemaining->y = (float)screenY + 4.0f;
        tlCornerBottomRemaining->z = depth;
        tlCornerBottomRemaining->rhwBits = FLOAT_ONE_BITS;
        tlCornerBottomRemaining->color = 0xFFFFFFFF;
        tlCornerBottomRemaining->specular = 0xFF000000;
        tlCornerBottomRemaining->u = uOrigin;
        tlCornerBottomRemaining->v = vOrigin + vDelta8;
        tlCornerTopRightPtr = g_MenuWinCBlend4DiffGraphicsObject->vertices + 2;
        tlCornerTopRightPtr->x = (float)messageX + 4.0f;
        tlCornerTopRightPtr->y = (float)screenY;
        tlCornerTopRightPtr->z = depth;
        tlCornerTopRightPtr->rhwBits = FLOAT_ONE_BITS;
        tlCornerTopRightPtr->color = 0xFFFFFFFF;
        tlCornerTopRightPtr->specular = 0xFF000000;
        tlCornerTopRightPtr->u = uOrigin + uWidth;
        tlCornerTopRightPtr->v = vOrigin;
        tlCornerBottomRight = g_MenuWinCBlend4DiffGraphicsObject->vertices + 3;
        tlCornerBottomRight->x = (float)messageX + 4.0f;
        tlCornerBottomRight->y = (float)screenY + 4.0f;
        tlCornerBottomRight->z = depth;
        tlCornerBottomRight->rhwBits = FLOAT_ONE_BITS;
        tlCornerBottomRight->color = 0xFFFFFFFF;
        tlCornerBottomRight->specular = 0xFF000000;
        tlCornerBottomRight->u = uOrigin + uWidth;
        tlCornerBottomRight->v = vOrigin + vDelta8;
        *g_MenuWinCBlend4DiffGraphicsObject->colorSignedBytePtr = 0;
        g_MenuWinCBlend4DiffGraphicsObject->paletteIndex = 0;
        g_MenuWinCBlend4DiffGraphicsObject->vertexCursor += g_MenuWinCBlend4DiffGraphicsObject->cursorStep;
        g_MenuWinCBlend4DiffGraphicsObject->colorIndex++;
        uOrigin = 0.046875f;
        trCornerTlPtr = g_MenuWinCBlend4DiffGraphicsObject->vertices;
        trCornerTlPtr->x = (float)messageX + windowRect->width - 4.0f;
        trCornerTlPtr->y = (float)screenY;
        trCornerTlPtr->z = depth;
        trCornerTlPtr->rhwBits = FLOAT_ONE_BITS;
        trCornerTlPtr->color = 0xFFFFFFFF;
        trCornerTlPtr->specular = 0xFF000000;
        trCornerTlPtr->u = uOrigin;
        trCornerTlPtr->v = vOrigin;
        pCornerTopRightB = g_MenuWinCBlend4DiffGraphicsObject->vertices + 1;
        pCornerTopRightB->x = (float)messageX + windowRect->width - 4.0f;
        pCornerTopRightB->y = (float)screenY + 4.0f;
        pCornerTopRightB->z = depth;
        pCornerTopRightB->rhwBits = FLOAT_ONE_BITS;
        pCornerTopRightB->color = 0xFFFFFFFF;
        pCornerTopRightB->specular = 0xFF000000;
        pCornerTopRightB->u = uOrigin;
        pCornerTopRightB->v = vOrigin + vDelta8;
        pTrCornerTopRight = g_MenuWinCBlend4DiffGraphicsObject->vertices + 2;
        pTrCornerTopRight->x = (float)messageX + windowRect->width - 4.0f + 4.0f;
        pTrCornerTopRight->y = (float)screenY;
        pTrCornerTopRight->z = depth;
        pTrCornerTopRight->rhwBits = FLOAT_ONE_BITS;
        pTrCornerTopRight->color = 0xFFFFFFFF;
        pTrCornerTopRight->specular = 0xFF000000;
        pTrCornerTopRight->u = uOrigin + uWidth;
        pTrCornerTopRight->v = vOrigin;
        pTrCornerBr = g_MenuWinCBlend4DiffGraphicsObject->vertices + 3;
        pTrCornerBr->x = (float)messageX + windowRect->width - 4.0f + 4.0f;
        pTrCornerBr->y = (float)screenY + 4.0f;
        pTrCornerBr->z = depth;
        pTrCornerBr->rhwBits = FLOAT_ONE_BITS;
        pTrCornerBr->color = 0xFFFFFFFF;
        pTrCornerBr->specular = 0xFF000000;
        pTrCornerBr->u = uOrigin + uWidth;
        pTrCornerBr->v = vOrigin + vDelta8;
        *g_MenuWinCBlend4DiffGraphicsObject->colorSignedBytePtr = 0;
        g_MenuWinCBlend4DiffGraphicsObject->paletteIndex = 0;
        g_MenuWinCBlend4DiffGraphicsObject->vertexCursor += g_MenuWinCBlend4DiffGraphicsObject->cursorStep;
        g_MenuWinCBlend4DiffGraphicsObject->colorIndex++;
        uOrigin = 0.0625f;
        vOrigin = 0.890625f;
        stageBlCornerTopLeft = g_MenuWinCBlend4DiffGraphicsObject->vertices;
        stageBlCornerTopLeft->x = (float)messageX;
        stageBlCornerTopLeft->y = (float)screenY + windowRect->height - 4.0f;
        stageBlCornerTopLeft->z = depth;
        stageBlCornerTopLeft->rhwBits = FLOAT_ONE_BITS;
        stageBlCornerTopLeft->color = 0xFFFFFFFF;
        stageBlCornerTopLeft->specular = 0xFF000000;
        stageBlCornerTopLeft->u = uOrigin;
        stageBlCornerTopLeft->v = vOrigin;
        blCornerBl = g_MenuWinCBlend4DiffGraphicsObject->vertices + 1;
        blCornerBl->x = (float)messageX;
        blCornerBl->y = (float)screenY + windowRect->height - 4.0f + 4.0f;
        blCornerBl->z = depth;
        blCornerBl->rhwBits = FLOAT_ONE_BITS;
        blCornerBl->color = 0xFFFFFFFF;
        blCornerBl->specular = 0xFF000000;
        blCornerBl->u = uOrigin;
        blCornerBl->v = vOrigin + vDelta8;
        cornerBottomLeftCPtr = g_MenuWinCBlend4DiffGraphicsObject->vertices + 2;
        cornerBottomLeftCPtr->x = (float)messageX + 4.0f;
        cornerBottomLeftCPtr->y = (float)screenY + windowRect->height - 4.0f;
        cornerBottomLeftCPtr->z = depth;
        cornerBottomLeftCPtr->rhwBits = FLOAT_ONE_BITS;
        cornerBottomLeftCPtr->color = 0xFFFFFFFF;
        cornerBottomLeftCPtr->specular = 0xFF000000;
        cornerBottomLeftCPtr->u = uOrigin + uWidth;
        cornerBottomLeftCPtr->v = vOrigin;
        blCornerFourth = g_MenuWinCBlend4DiffGraphicsObject->vertices + 3;
        blCornerFourth->x = (float)messageX + 4.0f;
        blCornerFourth->y = (float)screenY + windowRect->height - 4.0f + 4.0f;
        blCornerFourth->z = depth;
        blCornerFourth->rhwBits = FLOAT_ONE_BITS;
        blCornerFourth->color = 0xFFFFFFFF;
        blCornerFourth->specular = 0xFF000000;
        blCornerFourth->u = uOrigin + uWidth;
        blCornerFourth->v = vOrigin + vDelta8;
        *g_MenuWinCBlend4DiffGraphicsObject->colorSignedBytePtr = 0;
        g_MenuWinCBlend4DiffGraphicsObject->paletteIndex = 0;
        g_MenuWinCBlend4DiffGraphicsObject->vertexCursor += g_MenuWinCBlend4DiffGraphicsObject->cursorStep;
        g_MenuWinCBlend4DiffGraphicsObject->colorIndex++;
        uOrigin = 0.109375f;
        brCornerTop = g_MenuWinCBlend4DiffGraphicsObject->vertices;
        brCornerTop->x = (float)messageX + windowRect->width - 4.0f;
        brCornerTop->y = (float)screenY + windowRect->height - 4.0f;
        brCornerTop->z = depth;
        brCornerTop->rhwBits = FLOAT_ONE_BITS;
        brCornerTop->color = 0xFFFFFFFF;
        brCornerTop->specular = 0xFF000000;
        brCornerTop->u = uOrigin;
        brCornerTop->v = vOrigin;
        tmpBrCornerBl = g_MenuWinCBlend4DiffGraphicsObject->vertices + 1;
        tmpBrCornerBl->x = (float)messageX + windowRect->width - 4.0f;
        tmpBrCornerBl->y = (float)screenY + windowRect->height - 4.0f + 4.0f;
        tmpBrCornerBl->z = depth;
        tmpBrCornerBl->rhwBits = FLOAT_ONE_BITS;
        tmpBrCornerBl->color = 0xFFFFFFFF;
        tmpBrCornerBl->specular = 0xFF000000;
        tmpBrCornerBl->u = uOrigin;
        tmpBrCornerBl->v = vOrigin + vDelta8;
        pBrCornerTopRight = g_MenuWinCBlend4DiffGraphicsObject->vertices + 2;
        pBrCornerTopRight->x = (float)messageX + windowRect->width - 4.0f + 4.0f;
        pBrCornerTopRight->y = (float)screenY + windowRect->height - 4.0f;
        pBrCornerTopRight->z = depth;
        pBrCornerTopRight->rhwBits = FLOAT_ONE_BITS;
        pBrCornerTopRight->color = 0xFFFFFFFF;
        pBrCornerTopRight->specular = 0xFF000000;
        pBrCornerTopRight->u = uOrigin + uWidth;
        pBrCornerTopRight->v = vOrigin;
        brCornerBottomRight = g_MenuWinCBlend4DiffGraphicsObject->vertices + 3;
        brCornerBottomRight->x = (float)messageX + windowRect->width - 4.0f + 4.0f;
        brCornerBottomRight->y = (float)screenY + windowRect->height - 4.0f + 4.0f;
        brCornerBottomRight->z = depth;
        brCornerBottomRight->rhwBits = FLOAT_ONE_BITS;
        brCornerBottomRight->color = 0xFFFFFFFF;
        brCornerBottomRight->specular = 0xFF000000;
        brCornerBottomRight->u = uOrigin + uWidth;
        brCornerBottomRight->v = vOrigin + vDelta8;
        *g_MenuWinCBlend4DiffGraphicsObject->colorSignedBytePtr = 0;
        g_MenuWinCBlend4DiffGraphicsObject->paletteIndex = 0;
    }
    if (windowRect->width > 6 && windowRect->height > 6) {
        innerRemaining = messageX + 3;
        contentTop = screenY + 3;
        innerWidth = windowRect->width - 6;
        frameH = windowRect->height - 6;
        if (g_MenuMessageWindowActive != 0) {
            if (AcquireBatchNode(1, g_MenuBlendWindowBgGraphicsObject) != 0) {
                pBlendBgFirst = g_MenuBlendWindowBgGraphicsObject->vertices;
                pBlendBgFirst->x = (float)innerRemaining;
                pBlendBgFirst->y = (float)contentTop;
                pBlendBgFirst->z = depth + 1e-06f;
                pBlendBgFirst->rhwBits = FLOAT_ONE_BITS;
                pBlendBgFirst->color = *(int *)&g_MenuCornerColorTL;
                pBlendBgFirst->specular = 0xFF000000;
                blendBgSecond = g_MenuBlendWindowBgGraphicsObject->vertices + 1;
                blendBgSecond->x = (float)innerRemaining;
                blendBgSecond->y = (float)contentTop + frameH;
                blendBgSecond->z = depth + 1e-06f;
                blendBgSecond->rhwBits = FLOAT_ONE_BITS;
                blendBgSecond->color = *(int *)&g_MenuCornerColorTR;
                blendBgSecond->specular = 0xFF000000;
                pBlendBgTopRight = g_MenuBlendWindowBgGraphicsObject->vertices + 2;
                pBlendBgTopRight->x = (float)innerRemaining + innerWidth;
                pBlendBgTopRight->y = (float)contentTop;
                pBlendBgTopRight->z = depth + 1e-06f;
                pBlendBgTopRight->rhwBits = FLOAT_ONE_BITS;
                pBlendBgTopRight->color = *(int *)&g_MenuCornerColorBL;
                pBlendBgTopRight->specular = 0xFF000000;
                blendBgBottomRightPtr = g_MenuBlendWindowBgGraphicsObject->vertices + 3;
                blendBgBottomRightPtr->x = (float)innerRemaining + innerWidth;
                blendBgBottomRightPtr->y = (float)contentTop + frameH;
                blendBgBottomRightPtr->z = depth + 1e-06f;
                blendBgBottomRightPtr->rhwBits = FLOAT_ONE_BITS;
                blendBgBottomRightPtr->color = *(int *)&g_MenuCornerColorBR;
                blendBgBottomRightPtr->specular = 0xFF000000;
            }
            g_MenuBlendWindowBgPending = 1;
        } else {
            if (AcquireBatchNode(1, g_MenuWindowBgGraphicsObject) != 0) {
                bgTl = g_MenuWindowBgGraphicsObject->vertices;
                bgTl->x = (float)innerRemaining;
                bgTl->y = (float)contentTop;
                bgTl->z = depth + 1e-06f;
                bgTl->rhwBits = FLOAT_ONE_BITS;
                bgTl->color = *(int *)&g_MenuCornerColorTL;
                bgTl->specular = 0xFF000000;
                bgVertex1 = g_MenuWindowBgGraphicsObject->vertices + 1;
                bgVertex1->x = (float)innerRemaining;
                bgVertex1->y = (float)contentTop + frameH;
                bgVertex1->z = depth + 1e-06f;
                bgVertex1->rhwBits = FLOAT_ONE_BITS;
                bgVertex1->color = *(int *)&g_MenuCornerColorTR;
                bgVertex1->specular = 0xFF000000;
                bgTopRight = g_MenuWindowBgGraphicsObject->vertices + 2;
                bgTopRight->x = (float)innerRemaining + innerWidth;
                bgTopRight->y = (float)contentTop;
                bgTopRight->z = depth + 1e-06f;
                bgTopRight->rhwBits = FLOAT_ONE_BITS;
                bgTopRight->color = *(int *)&g_MenuCornerColorBL;
                bgTopRight->specular = 0xFF000000;
                rawBgVert = g_MenuWindowBgGraphicsObject->vertices + 3;
                rawBgVert->x = (float)innerRemaining + innerWidth;
                rawBgVert->y = (float)contentTop + frameH;
                rawBgVert->z = depth + 1e-06f;
                rawBgVert->rhwBits = FLOAT_ONE_BITS;
                rawBgVert->color = *(int *)&g_MenuCornerColorBR;
                rawBgVert->specular = 0xFF000000;
            }
            g_MenuWindowBgPending = 1;
        }
    }
}

/*
 * When the text subsystem is ready, adds the global text origin to the requested
 * coordinates and independently acquires the arrow and shadow batches. Emits a
 * white 24-by-13 arrow quad at the supplied depth with UV origin 0.875, 0.03125,
 * then a half-alpha white 24-by-4 shadow beginning 12 pixels below it with UV
 * origin 0.1015625, 0.296875.
 */
// FUNCTION: FF7 0x006f0d7d
void __cdecl Menu_DrawMoreTextArrow1x(int posX, int posY, float depth)
{
    float uWidth;
    float vOrigin;
    GfxTlVertex *v;
    GfxTlVertex *arrowV2Ptr;
    GfxTlVertex *arrowBottomRemaining;
    GfxTlVertex *arrowTopLeft8;
    float vSize;
    GfxTlVertex *shadowBottomLeft;
    GfxTlVertex *shadowTopLeftPtr;
    GfxTlVertex *shadowBottomRight;
    GfxTlVertex *pShadowTr;
    float uOrigin;

    if (Menu_IsTextSubsystemReady() == 0) {
        return;
    }
    posX += g_MenuViewportX;
    posY += g_MenuViewportY;
    if (AcquireBatchNode(1, g_MenuWinBlend4GraphicsObject) != 0) {
        uOrigin = 0.875f;
        vOrigin = 0.03125f;
        uWidth = 0.09375f;
        vSize = 0.05078125f;
        arrowTopLeft8 = g_MenuWinBlend4GraphicsObject->vertices;
        arrowTopLeft8->x = (float)posX;
        arrowTopLeft8->y = (float)posY;
        arrowTopLeft8->z = depth;
        arrowTopLeft8->rhwBits = FLOAT_ONE_BITS;
        arrowTopLeft8->color = 0xFFFFFFFF;
        arrowTopLeft8->specular = 0xFF000000;
        arrowTopLeft8->u = uOrigin;
        arrowTopLeft8->v = vOrigin;
        arrowBottomRemaining = g_MenuWinBlend4GraphicsObject->vertices + 1;
        arrowBottomRemaining->x = (float)posX;
        arrowBottomRemaining->y = (float)posY + 13.0f;
        arrowBottomRemaining->z = depth;
        arrowBottomRemaining->rhwBits = FLOAT_ONE_BITS;
        arrowBottomRemaining->color = 0xFFFFFFFF;
        arrowBottomRemaining->specular = 0xFF000000;
        arrowBottomRemaining->u = uOrigin;
        arrowBottomRemaining->v = vOrigin + vSize;
        arrowV2Ptr = g_MenuWinBlend4GraphicsObject->vertices + 2;
        arrowV2Ptr->x = (float)posX + 24.0f;
        arrowV2Ptr->y = (float)posY;
        arrowV2Ptr->z = depth;
        arrowV2Ptr->rhwBits = FLOAT_ONE_BITS;
        arrowV2Ptr->color = 0xFFFFFFFF;
        arrowV2Ptr->specular = 0xFF000000;
        arrowV2Ptr->u = uOrigin + uWidth;
        arrowV2Ptr->v = vOrigin;
        v = g_MenuWinBlend4GraphicsObject->vertices + 3;
        v->x = (float)posX + 24.0f;
        v->y = (float)posY + 13.0f;
        v->z = depth;
        v->rhwBits = FLOAT_ONE_BITS;
        v->color = 0xFFFFFFFF;
        v->specular = 0xFF000000;
        v->u = uOrigin + uWidth;
        v->v = vOrigin + vSize;
        *g_MenuWinBlend4GraphicsObject->colorBytePtr = 1;
        g_MenuWinBlend4GraphicsObject->texturePage = 1;
    }
    if (AcquireBatchNode(1, g_MenuWinBlend0GraphicsObject) != 0) {
        uOrigin = 0.1015625f;
        vOrigin = 0.296875f;
        uWidth = 0.09375f;
        vSize = 0.015625f;
        shadowTopLeftPtr = (GfxTlVertex *)(g_MenuWinBlend0GraphicsObject->vertexCursor);
        shadowTopLeftPtr->x = (float)posX;
        shadowTopLeftPtr->y = (float)posY + 12.0f;
        shadowTopLeftPtr->z = depth;
        shadowTopLeftPtr->rhwBits = FLOAT_ONE_BITS;
        shadowTopLeftPtr->color = 0x80FFFFFF;
        shadowTopLeftPtr->specular = 0xFF000000;
        shadowTopLeftPtr->u = uOrigin;
        shadowTopLeftPtr->v = vOrigin;
        shadowBottomLeft = g_MenuWinBlend0GraphicsObject->vertices + 1;
        shadowBottomLeft->x = (float)posX;
        shadowBottomLeft->y = (float)posY + 12.0f + 4.0f;
        shadowBottomLeft->z = depth;
        shadowBottomLeft->rhwBits = FLOAT_ONE_BITS;
        shadowBottomLeft->color = 0x80FFFFFF;
        shadowBottomLeft->specular = 0xFF000000;
        shadowBottomLeft->u = uOrigin;
        shadowBottomLeft->v = vOrigin + vSize;
        pShadowTr = g_MenuWinBlend0GraphicsObject->vertices + 2;
        pShadowTr->x = (float)posX + 24.0f;
        pShadowTr->y = (float)posY + 12.0f;
        pShadowTr->z = depth;
        pShadowTr->rhwBits = FLOAT_ONE_BITS;
        pShadowTr->color = 0x80FFFFFF;
        pShadowTr->specular = 0xFF000000;
        pShadowTr->u = uOrigin + uWidth;
        pShadowTr->v = vOrigin;
        shadowBottomRight = g_MenuWinBlend0GraphicsObject->vertices + 3;
        shadowBottomRight->x = (float)posX + 24.0f;
        shadowBottomRight->y = (float)posY + 12.0f + 4.0f;
        shadowBottomRight->z = depth;
        shadowBottomRight->rhwBits = FLOAT_ONE_BITS;
        shadowBottomRight->color = 0x80FFFFFF;
        shadowBottomRight->specular = 0xFF000000;
        shadowBottomRight->u = uOrigin + uWidth;
        shadowBottomRight->v = vOrigin + vSize;
        *(char *)g_MenuWinBlend0GraphicsObject->colorIndex = 1;
        g_MenuWinBlend0GraphicsObject->texturePage = 1;
        g_MenuBlendWindowBgPending = 1;
    }
}

/*
 * Adds global text origin, acquires five UI quads and emits an 8-by-21 separator
 * at X plus 32, choosing its UV column from the original value bit 0 before
 * clamping. Signed values above 5999 clamp to 5999; four 16-by-21 digit quads use
 * successive signed divisor/remainder steps and two atlas rows.
 */
// FUNCTION: FF7 0x006f1171
void __cdecl Menu_DrawCountdownTimer1x(int posX, int posY, unsigned int timerValue)
{
    /* p,a..o occupy VC5's buckets 0..15, hence slots -4 through -40. */
    int rectX;
    int rectY;
    int digitSlot;
    float vOrigin;
    float uOrigin;
    float f;
    int digitVal;
    float vHeight;
    GfxTlVertex *separatorTopRemaining;
    GfxTlVertex *digitTopLeft;
    GfxTlVertex *pSeparatorFourth;
    GfxTlVertex *separatorTopRight;
    GfxTlVertex *separatorBottomRemaining;
    GfxTlVertex *digitBottomRight;
    GfxTlVertex *digitTr;
    GfxTlVertex *digitBlPtr;

    posX += g_MenuViewportX;
    posY += g_MenuViewportY;
    rectX = posX;
    rectY = posY;
    if (AcquireBatchNode(5, g_MenuWinBlend4GraphicsObject) != 0) {
        uOrigin = (timerValue & 1) ? 0.65625f : 0.625f;
        vOrigin = 0.3125f;
        f = 0.03125f;
        vHeight = 0.08203125f;
        separatorTopRemaining = g_MenuWinBlend4GraphicsObject->vertices;
        separatorTopRemaining->x = posX + 32.0f;
        separatorTopRemaining->y = posY;
        separatorTopRemaining->zBits = 0x3749539C;
        separatorTopRemaining->rhwBits = FLOAT_ONE_BITS;
        separatorTopRemaining->color = 0xFFFFFFFF;
        separatorTopRemaining->specular = 0xFF000000;
        separatorTopRemaining->u = uOrigin;
        separatorTopRemaining->v = vOrigin;
        separatorBottomRemaining = g_MenuWinBlend4GraphicsObject->vertices + 1;
        separatorBottomRemaining->x = posX + 32.0f;
        separatorBottomRemaining->y = posY + 21.0f;
        separatorBottomRemaining->zBits = 0x3749539C;
        separatorBottomRemaining->rhwBits = FLOAT_ONE_BITS;
        separatorBottomRemaining->color = 0xFFFFFFFF;
        separatorBottomRemaining->specular = 0xFF000000;
        separatorBottomRemaining->u = uOrigin;
        separatorBottomRemaining->v = vOrigin + vHeight;
        separatorTopRight = g_MenuWinBlend4GraphicsObject->vertices + 2;
        separatorTopRight->x = posX + 32.0f + 8.0f;
        separatorTopRight->y = posY;
        separatorTopRight->zBits = 0x3749539C;
        separatorTopRight->rhwBits = FLOAT_ONE_BITS;
        separatorTopRight->color = 0xFFFFFFFF;
        separatorTopRight->specular = 0xFF000000;
        separatorTopRight->u = uOrigin + f;
        separatorTopRight->v = vOrigin;
        pSeparatorFourth = g_MenuWinBlend4GraphicsObject->vertices + 3;
        pSeparatorFourth->x = posX + 32.0f + 8.0f;
        pSeparatorFourth->y = posY + 21.0f;
        pSeparatorFourth->zBits = 0x3749539C;
        pSeparatorFourth->rhwBits = FLOAT_ONE_BITS;
        pSeparatorFourth->color = 0xFFFFFFFF;
        pSeparatorFourth->specular = 0xFF000000;
        pSeparatorFourth->u = uOrigin + f;
        pSeparatorFourth->v = vOrigin + vHeight;
        *g_MenuWinBlend4GraphicsObject->colorBytePtr = 0xC;
        g_MenuWinBlend4GraphicsObject->texturePage = 0xC;
        g_MenuWinBlend4GraphicsObject->vertexCursor += g_MenuWinBlend4GraphicsObject->cursorStep;
        g_MenuWinBlend4GraphicsObject->colorBytePtr += 1;
        if ((int)timerValue > 5999) {
            timerValue = 5999;
        }
        for (digitSlot = 0; digitSlot < 4; digitSlot++) {
            digitVal = (int)timerValue / g_MenuTimeDivisors[digitSlot];
            uOrigin = ((digitVal % 5) * 16 + 176) * 0.00390625f;
            vOrigin = digitVal > 4 ? 0.40625f : 0.3125f;
            f = 0.0625f;
            digitTopLeft = g_MenuWinBlend4GraphicsObject->vertices;
            digitTopLeft->x = posX;
            digitTopLeft->y = posY;
            digitTopLeft->zBits = 0x3749539C;
            digitTopLeft->rhwBits = FLOAT_ONE_BITS;
            digitTopLeft->color = 0xFFFFFFFF;
            digitTopLeft->specular = 0xFF000000;
            digitTopLeft->u = uOrigin;
            digitTopLeft->v = vOrigin;
            digitBlPtr = g_MenuWinBlend4GraphicsObject->vertices + 1;
            digitBlPtr->x = posX;
            digitBlPtr->y = posY + 21.0f;
            digitBlPtr->zBits = 0x3749539C;
            digitBlPtr->rhwBits = FLOAT_ONE_BITS;
            digitBlPtr->color = 0xFFFFFFFF;
            digitBlPtr->specular = 0xFF000000;
            digitBlPtr->u = uOrigin;
            digitBlPtr->v = vOrigin + vHeight;
            digitTr = g_MenuWinBlend4GraphicsObject->vertices + 2;
            digitTr->x = posX + 16.0f;
            digitTr->y = posY;
            digitTr->zBits = 0x3749539C;
            digitTr->rhwBits = FLOAT_ONE_BITS;
            digitTr->color = 0xFFFFFFFF;
            digitTr->specular = 0xFF000000;
            digitTr->u = uOrigin + f;
            digitTr->v = vOrigin;
            digitBottomRight = g_MenuWinBlend4GraphicsObject->vertices + 3;
            digitBottomRight->x = posX + 16.0f;
            digitBottomRight->y = posY + 21.0f;
            digitBottomRight->zBits = 0x3749539C;
            digitBottomRight->rhwBits = FLOAT_ONE_BITS;
            digitBottomRight->color = 0xFFFFFFFF;
            digitBottomRight->specular = 0xFF000000;
            digitBottomRight->u = uOrigin + f;
            digitBottomRight->v = vOrigin + vHeight;
            *g_MenuWinBlend4GraphicsObject->colorBytePtr = 0xC;
            g_MenuWinBlend4GraphicsObject->texturePage = 0xC;
            g_MenuWinBlend4GraphicsObject->vertexCursor += g_MenuWinBlend4GraphicsObject->cursorStep;
            g_MenuWinBlend4GraphicsObject->colorBytePtr += 1;
            if (digitSlot == 1) {
                posX += 8;
            }
            timerValue = (int)timerValue % g_MenuTimeDivisors[digitSlot];
            posX += 16;
        }
    }
    if (g_MenuDigitBackdropEnabled != 0) {
        int width;
        int height;

        width = 72;
        height = 21;
        Menu_DrawTranslucentBlackRect(rectX, rectY, width, height);
        g_MenuDoDrawBlackQuad = 1;
    }
}

/*
 * Adds the menu viewport origin to the supplied position, extracts eight decimal
 * positions using the divisor table g_MenuNumberDivisors and draws the
 * rightmost digitCount digits without leading-zero suppression. Each acquired batch node
 * receives a white 16 by 21 atlas quad at fixed depth; remainder modulo 5 selects
 * the column and digit greater than 4 selects its row.
 */
// FUNCTION: FF7 0x006f168b
void __cdecl Menu_DrawMessageWindowNumber1x(int x, int y, int value, int digitCount, int flags)
{
    int idx;
    int digitValue;
    float textureV;
    GfxTlVertex *pTopRight;
    GfxTlVertex *v;
    GfxTlVertex *vertTopLeft;
    float texStepV;
    float stepU;
    int texturePixelX;
    GfxTlVertex *bottomRight;
    int texVBits;
    float uCoord;

    x += g_MenuViewportX;
    y += g_MenuViewportY;
    for (idx = 0; idx < 8; idx++) {
        digitValue = value / g_MenuNumberDivisors[idx];
        if (8 - idx <= digitCount) {
            if (AcquireBatchNode(1, g_MenuWinBlend4GraphicsObject) != 0) {
                texturePixelX = (digitValue % 5 << 4) + 176;
                uCoord = texturePixelX * 0.00390625f;
                if (digitValue > 4) {
                    texVBits = 0x3ED00000;
                } else {
                    texVBits = 0x3EA00000;
                }
                *(int *)&textureV = texVBits;
                stepU = 0.0625f;
                texStepV = 0.08203125f;
                vertTopLeft = g_MenuWinBlend4GraphicsObject->vertices;
                vertTopLeft->x = x;
                vertTopLeft->y = y;
                vertTopLeft->zBits = 0x3727C5AC;
                vertTopLeft->rhwBits = FLOAT_ONE_BITS;
                vertTopLeft->color = 0xFFFFFFFF;
                vertTopLeft->specular = 0xFF000000;
                vertTopLeft->u = uCoord;
                vertTopLeft->v = textureV;
                v = g_MenuWinBlend4GraphicsObject->vertices + 1;
                v->x = x;
                v->y = y + 21.0f;
                v->zBits = 0x3727C5AC;
                v->rhwBits = FLOAT_ONE_BITS;
                v->color = 0xFFFFFFFF;
                v->specular = 0xFF000000;
                v->u = uCoord;
                v->v = textureV + texStepV;
                pTopRight = g_MenuWinBlend4GraphicsObject->vertices + 2;
                pTopRight->x = x + 16.0f;
                pTopRight->y = y;
                pTopRight->zBits = 0x3727C5AC;
                pTopRight->rhwBits = FLOAT_ONE_BITS;
                pTopRight->color = 0xFFFFFFFF;
                pTopRight->specular = 0xFF000000;
                pTopRight->u = uCoord + stepU;
                pTopRight->v = textureV;
                bottomRight = g_MenuWinBlend4GraphicsObject->vertices + 3;
                bottomRight->x = x + 16.0f;
                bottomRight->y = y + 21.0f;
                bottomRight->zBits = 0x3727C5AC;
                bottomRight->rhwBits = FLOAT_ONE_BITS;
                bottomRight->color = 0xFFFFFFFF;
                bottomRight->specular = 0xFF000000;
                bottomRight->u = uCoord + stepU;
                bottomRight->v = textureV + texStepV;
                *g_MenuWinBlend4GraphicsObject->colorBytePtr = 0xC;
                g_MenuWinBlend4GraphicsObject->texturePage = 0xC;
                g_MenuWinBlend4GraphicsObject->vertexCursor += g_MenuWinBlend4GraphicsObject->cursorStep;
                g_MenuWinBlend4GraphicsObject->colorBytePtr++;
            }
            x += 16;
        }
        value = value % g_MenuNumberDivisors[idx];
    }
}

/*
 * Menu_ApplyUiScalingViewport's other half: the same three viewport settings, but with the mode
 * handed in rather than asked for, and the text subsystem marked ready first.
 */
// FUNCTION: FF7 0x006f193e
void __cdecl Menu_ApplyUiScalingViewportForMode(int mode)
{
    Menu_SetTextSubsystemReady(1);
    if (mode == MENU_UI_SCALE_SMALL) {
        g_MenuViewportX = 0;
        g_MenuViewportY = 0;
        g_MenuViewportWidth = 320;
        g_MenuViewportHeight = 240;
        g_MenuIsSmallViewport320x240 = 1;
    } else if (mode == MENU_UI_SCALE_SMALL_CENTERED) {
        g_MenuViewportX = 160;
        g_MenuViewportY = 120;
        g_MenuViewportWidth = 320;
        g_MenuViewportHeight = 240;
        g_MenuIsSmallViewport320x240 = 1;
    } else {
        g_MenuViewportX = 0;
        g_MenuViewportY = 0;
        g_MenuViewportWidth = 640;
        g_MenuViewportHeight = 480;
        g_MenuIsSmallViewport320x240 = 0;
    }
}

/*
 * Sets the vertical offset, in screen pixels, that the dialogue window drawer adds to every
 * message window's text start (g_MenuMessageWindowYOffset; added after the window's y is doubled
 * in the 640x480 path). The field and the world map set it to 0 just before they draw their
 * dialogue windows with Menu_RenderMessageWindows; no caller passes anything else.
 */
// FUNCTION: FF7 0x006f19f3
void __cdecl Menu_SetMessageWindowYOffset(int value)
{
    g_MenuMessageWindowYOffset = value;
}

/*
 * Emits the 2x popup body as four atlas quadrants plus the optional coloured inner
 * rectangle. When the text subsystem is ready, draws a 2x popup frame from signed-
 * short X/Y/width/height, copying depth directly into each depth field as its
 * existing bit representation.
 */
// FUNCTION: FF7 0x006f1a00
void __cdecl Menu_DrawPopupMessageBody2x(EnglishWindowFrame *rect, int depth)
{
    int innerY;
    int innerMarginX;
    GfxTlVertex *vertBottomRightIn;
    GfxTlVertex *pVtxBottomRight;
    GfxTlVertex *pBottomLeftInner;
    GfxTlVertex *vertexBottomRemaining;
    GfxTlVertex *vtxOuterCTRPtr;
    GfxTlVertex *cornerRightTopPtr;
    GfxTlVertex *topRightCorner;
    GfxTlVertex *vertexCornerInPtr;
    GfxTlVertex *bgBottomLeftVtx;
    int interiorH;
    float textureV;
    GfxTlVertex *pTopRightInnerVertex;
    GfxTlVertex *topRightEdgePtr;
    GfxTlVertex *pVtxTopLeftIn;
    GfxTlVertex *vertexCornerTI;
    GfxTlVertex *topLeftCornerPointer;
    GfxTlVertex *pCornerTopLeft;
    GfxTlVertex *pBottomLeftCorner;
    GfxTlVertex *bgTopRightVtx;
    GfxTlVertex *windowBgTopRemaining;
    GfxTlVertex *blendBottomRightVtx;
    int innerWidth;
    GfxTlVertex *vertexLeftBottomInPtr;
    GfxTlVertex *remainingTopInnerVertex;
    GfxTlVertex *pVtxTopLeft;
    float texVDelta;
    GfxTlVertex *vertexOuterCRT;
    GfxTlVertex *blendBgBottomLeft;
    GfxTlVertex *remainingBottomEdge;
    GfxTlVertex *topRemainingPopVtx;
    GfxTlVertex *vtxBlendTR;
    float dUTex;
    GfxTlVertex *outerRightBotV;
    GfxTlVertex *pVtxBlendTL;
    GfxTlVertex *v;
    GfxTlVertex *f;
    GfxTlVertex *vertexOuterCBRXPtr;
    GfxTlVertex *bgBottomRightVtx;
    GfxTlVertex *bottomCornerOut8;
    GfxTlVertex *rightCornerBottomOut;
    float texRemainingU;
    GfxTlVertex *vtxRightTop;
    GfxTlVertex *bottomRightCornerPtr;
    GfxTlVertex *cornerInnerBottomLowPtr;
    GfxTlVertex *pCornerBottomInner;
    GfxTlVertex *bottomRemainingCornerOut;

    if (Menu_IsTextSubsystemReady() == 0) {
        return;
    }
    if (rect->width > 16) {
        if (AcquireBatchNode(2, g_MenuWinCBlend4DiffGraphicsObject) != 0) {
            texRemainingU = 0.0f;
            textureV = 0.75f;
            dUTex = 0.0625f;
            texVDelta = 0.03125f;
            pVtxTopLeft = g_MenuWinCBlend4DiffGraphicsObject->vertices;
            pVtxTopLeft->x = (float)rect->x + 8.0f;
            pVtxTopLeft->y = (float)rect->y;
            pVtxTopLeft->zBits = depth;
            pVtxTopLeft->rhwBits = FLOAT_ONE_BITS;
            pVtxTopLeft->color = 0xFFFFFFFF;
            pVtxTopLeft->specular = 0xFF000000;
            pVtxTopLeft->u = texRemainingU;
            pVtxTopLeft->v = textureV;
            pVtxTopLeftIn = g_MenuWinCBlend4DiffGraphicsObject->vertices + 1;
            pVtxTopLeftIn->x = (float)rect->x + 8.0f;
            pVtxTopLeftIn->y = (float)rect->y + 8.0f;
            pVtxTopLeftIn->zBits = depth;
            pVtxTopLeftIn->rhwBits = FLOAT_ONE_BITS;
            pVtxTopLeftIn->color = 0xFFFFFFFF;
            pVtxTopLeftIn->specular = 0xFF000000;
            pVtxTopLeftIn->u = texRemainingU;
            pVtxTopLeftIn->v = textureV + texVDelta;
            topRightEdgePtr = g_MenuWinCBlend4DiffGraphicsObject->vertices + 2;
            topRightEdgePtr->x = (float)rect->x + 8.0f + rect->width - 16.0f;
            topRightEdgePtr->y = (float)rect->y;
            topRightEdgePtr->zBits = depth;
            topRightEdgePtr->rhwBits = FLOAT_ONE_BITS;
            topRightEdgePtr->color = 0xFFFFFFFF;
            topRightEdgePtr->specular = 0xFF000000;
            topRightEdgePtr->u = texRemainingU + dUTex;
            topRightEdgePtr->v = textureV;
            pTopRightInnerVertex = g_MenuWinCBlend4DiffGraphicsObject->vertices + 3;
            pTopRightInnerVertex->x = (float)rect->x + 8.0f + rect->width - 16.0f;
            pTopRightInnerVertex->y = (float)rect->y + 8.0f;
            pTopRightInnerVertex->zBits = depth;
            pTopRightInnerVertex->rhwBits = FLOAT_ONE_BITS;
            pTopRightInnerVertex->color = 0xFFFFFFFF;
            pTopRightInnerVertex->specular = 0xFF000000;
            pTopRightInnerVertex->u = texRemainingU + dUTex;
            pTopRightInnerVertex->v = textureV + texVDelta;
            *g_MenuWinCBlend4DiffGraphicsObject->colorSignedBytePtr = 0;
            g_MenuWinCBlend4DiffGraphicsObject->paletteIndex = 0;
            g_MenuWinCBlend4DiffGraphicsObject->vertexCursor += g_MenuWinCBlend4DiffGraphicsObject->cursorStep;
            g_MenuWinCBlend4DiffGraphicsObject->colorIndex++;
            texRemainingU = 0.125f;
            textureV = 0.84375f;
            dUTex = 0.0625f;
            texVDelta = 0.03125f;
            vertexBottomRemaining = g_MenuWinCBlend4DiffGraphicsObject->vertices;
            vertexBottomRemaining->x = (float)rect->x + 8.0f;
            vertexBottomRemaining->y = (float)rect->y + rect->height - 8.0f;
            vertexBottomRemaining->zBits = depth;
            vertexBottomRemaining->rhwBits = FLOAT_ONE_BITS;
            vertexBottomRemaining->color = 0xFFFFFFFF;
            vertexBottomRemaining->specular = 0xFF000000;
            vertexBottomRemaining->u = texRemainingU;
            vertexBottomRemaining->v = textureV;
            pBottomLeftInner = g_MenuWinCBlend4DiffGraphicsObject->vertices + 1;
            pBottomLeftInner->x = (float)rect->x + 8.0f;
            pBottomLeftInner->y = (float)rect->y + rect->height - 8.0f + 8.0f;
            pBottomLeftInner->zBits = depth;
            pBottomLeftInner->rhwBits = FLOAT_ONE_BITS;
            pBottomLeftInner->color = 0xFFFFFFFF;
            pBottomLeftInner->specular = 0xFF000000;
            pBottomLeftInner->u = texRemainingU;
            pBottomLeftInner->v = textureV + texVDelta;
            pVtxBottomRight = g_MenuWinCBlend4DiffGraphicsObject->vertices + 2;
            pVtxBottomRight->x = (float)rect->x + 8.0f + rect->width - 16.0f;
            pVtxBottomRight->y = (float)rect->y + rect->height - 8.0f;
            pVtxBottomRight->zBits = depth;
            pVtxBottomRight->rhwBits = FLOAT_ONE_BITS;
            pVtxBottomRight->color = 0xFFFFFFFF;
            pVtxBottomRight->specular = 0xFF000000;
            pVtxBottomRight->u = texRemainingU + dUTex;
            pVtxBottomRight->v = textureV;
            vertBottomRightIn = g_MenuWinCBlend4DiffGraphicsObject->vertices + 3;
            vertBottomRightIn->x = (float)rect->x + 8.0f + rect->width - 16.0f;
            vertBottomRightIn->y = (float)rect->y + rect->height - 8.0f + 8.0f;
            vertBottomRightIn->zBits = depth;
            vertBottomRightIn->rhwBits = FLOAT_ONE_BITS;
            vertBottomRightIn->color = 0xFFFFFFFF;
            vertBottomRightIn->specular = 0xFF000000;
            vertBottomRightIn->u = texRemainingU + dUTex;
            vertBottomRightIn->v = textureV + texVDelta;
            *g_MenuWinCBlend4DiffGraphicsObject->colorSignedBytePtr = 0;
            g_MenuWinCBlend4DiffGraphicsObject->paletteIndex = 0;
        }
    }
    if (rect->height > 16) {
        if (AcquireBatchNode(2, g_MenuWinCBlend4DiffGraphicsObject) != 0) {
            texRemainingU = 0.21875f;
            textureV = 0.875f;
            dUTex = 0.03125f;
            texVDelta = 0.0625f;
            vtxRightTop = g_MenuWinCBlend4DiffGraphicsObject->vertices;
            vtxRightTop->x = (float)rect->x + rect->width - 8.0f;
            vtxRightTop->y = (float)rect->y + 8.0f;
            vtxRightTop->zBits = depth;
            vtxRightTop->rhwBits = FLOAT_ONE_BITS;
            vtxRightTop->color = 0xFFFFFFFF;
            vtxRightTop->specular = 0xFF000000;
            vtxRightTop->u = texRemainingU;
            vtxRightTop->v = textureV;
            f = g_MenuWinCBlend4DiffGraphicsObject->vertices + 1;
            f->x = (float)rect->x + rect->width - 8.0f;
            f->y = (float)rect->y + 8.0f + rect->height - 16.0f;
            f->zBits = depth;
            f->rhwBits = FLOAT_ONE_BITS;
            f->color = 0xFFFFFFFF;
            f->specular = 0xFF000000;
            f->u = texRemainingU;
            f->v = textureV + texVDelta;
            v = g_MenuWinCBlend4DiffGraphicsObject->vertices + 2;
            v->x = (float)rect->x + rect->width - 8.0f + 8.0f;
            v->y = (float)rect->y + 8.0f;
            v->zBits = depth;
            v->rhwBits = FLOAT_ONE_BITS;
            v->color = 0xFFFFFFFF;
            v->specular = 0xFF000000;
            v->u = texRemainingU + dUTex;
            v->v = textureV;
            outerRightBotV = g_MenuWinCBlend4DiffGraphicsObject->vertices + 3;
            outerRightBotV->x = (float)rect->x + rect->width - 8.0f + 8.0f;
            outerRightBotV->y = (float)rect->y + 8.0f + rect->height - 16.0f;
            outerRightBotV->zBits = depth;
            outerRightBotV->rhwBits = FLOAT_ONE_BITS;
            outerRightBotV->color = 0xFFFFFFFF;
            outerRightBotV->specular = 0xFF000000;
            outerRightBotV->u = texRemainingU + dUTex;
            outerRightBotV->v = textureV + texVDelta;
            *g_MenuWinCBlend4DiffGraphicsObject->colorSignedBytePtr = 0;
            g_MenuWinCBlend4DiffGraphicsObject->paletteIndex = 0;
            g_MenuWinCBlend4DiffGraphicsObject->vertexCursor += g_MenuWinCBlend4DiffGraphicsObject->cursorStep;
            g_MenuWinCBlend4DiffGraphicsObject->colorIndex++;
            texRemainingU = 0.0f;
            textureV = 0.875f;
            dUTex = 0.03125f;
            texVDelta = 0.0625f;
            topRemainingPopVtx = g_MenuWinCBlend4DiffGraphicsObject->vertices;
            topRemainingPopVtx->x = (float)rect->x;
            topRemainingPopVtx->y = (float)rect->y + 8.0f;
            topRemainingPopVtx->zBits = depth;
            topRemainingPopVtx->rhwBits = FLOAT_ONE_BITS;
            topRemainingPopVtx->color = 0xFFFFFFFF;
            topRemainingPopVtx->specular = 0xFF000000;
            topRemainingPopVtx->u = texRemainingU;
            topRemainingPopVtx->v = textureV;
            remainingBottomEdge = g_MenuWinCBlend4DiffGraphicsObject->vertices + 1;
            remainingBottomEdge->x = (float)rect->x;
            remainingBottomEdge->y = (float)rect->y + 8.0f + rect->height - 16.0f;
            remainingBottomEdge->zBits = depth;
            remainingBottomEdge->rhwBits = FLOAT_ONE_BITS;
            remainingBottomEdge->color = 0xFFFFFFFF;
            remainingBottomEdge->specular = 0xFF000000;
            remainingBottomEdge->u = texRemainingU;
            remainingBottomEdge->v = textureV + texVDelta;
            remainingTopInnerVertex = g_MenuWinCBlend4DiffGraphicsObject->vertices + 2;
            remainingTopInnerVertex->x = (float)rect->x + 8.0f;
            remainingTopInnerVertex->y = (float)rect->y + 8.0f;
            remainingTopInnerVertex->zBits = depth;
            remainingTopInnerVertex->rhwBits = FLOAT_ONE_BITS;
            remainingTopInnerVertex->color = 0xFFFFFFFF;
            remainingTopInnerVertex->specular = 0xFF000000;
            remainingTopInnerVertex->u = texRemainingU + dUTex;
            remainingTopInnerVertex->v = textureV;
            vertexLeftBottomInPtr = g_MenuWinCBlend4DiffGraphicsObject->vertices + 3;
            vertexLeftBottomInPtr->x = (float)rect->x + 8.0f;
            vertexLeftBottomInPtr->y = (float)rect->y + 8.0f + rect->height - 16.0f;
            vertexLeftBottomInPtr->zBits = depth;
            vertexLeftBottomInPtr->rhwBits = FLOAT_ONE_BITS;
            vertexLeftBottomInPtr->color = 0xFFFFFFFF;
            vertexLeftBottomInPtr->specular = 0xFF000000;
            vertexLeftBottomInPtr->u = texRemainingU + dUTex;
            vertexLeftBottomInPtr->v = textureV + texVDelta;
            *g_MenuWinCBlend4DiffGraphicsObject->colorSignedBytePtr = 0;
            g_MenuWinCBlend4DiffGraphicsObject->paletteIndex = 0;
        }
    }
    if (AcquireBatchNode(4, g_MenuWinCBlend4DiffGraphicsObject) != 0) {
        texVDelta = 0.03125f;
        dUTex = texVDelta;
        texRemainingU = 0.0f;
        textureV = 0.8125f;
        pCornerTopLeft = g_MenuWinCBlend4DiffGraphicsObject->vertices;
        pCornerTopLeft->x = (float)rect->x;
        pCornerTopLeft->y = (float)rect->y;
        pCornerTopLeft->zBits = depth;
        pCornerTopLeft->rhwBits = FLOAT_ONE_BITS;
        pCornerTopLeft->color = 0xFFFFFFFF;
        pCornerTopLeft->specular = 0xFF000000;
        pCornerTopLeft->u = texRemainingU;
        pCornerTopLeft->v = textureV;
        topLeftCornerPointer = g_MenuWinCBlend4DiffGraphicsObject->vertices + 1;
        topLeftCornerPointer->x = (float)rect->x;
        topLeftCornerPointer->y = (float)rect->y + 8.0f;
        topLeftCornerPointer->zBits = depth;
        topLeftCornerPointer->rhwBits = FLOAT_ONE_BITS;
        topLeftCornerPointer->color = 0xFFFFFFFF;
        topLeftCornerPointer->specular = 0xFF000000;
        topLeftCornerPointer->u = texRemainingU;
        topLeftCornerPointer->v = textureV + texVDelta;
        vertexCornerTI = g_MenuWinCBlend4DiffGraphicsObject->vertices + 2;
        vertexCornerTI->x = (float)rect->x + 8.0f;
        vertexCornerTI->y = (float)rect->y;
        vertexCornerTI->zBits = depth;
        vertexCornerTI->rhwBits = FLOAT_ONE_BITS;
        vertexCornerTI->color = 0xFFFFFFFF;
        vertexCornerTI->specular = 0xFF000000;
        vertexCornerTI->u = texRemainingU + dUTex;
        vertexCornerTI->v = textureV;
        vertexCornerInPtr = g_MenuWinCBlend4DiffGraphicsObject->vertices + 3;
        vertexCornerInPtr->x = (float)rect->x + 8.0f;
        vertexCornerInPtr->y = (float)rect->y + 8.0f;
        vertexCornerInPtr->zBits = depth;
        vertexCornerInPtr->rhwBits = FLOAT_ONE_BITS;
        vertexCornerInPtr->color = 0xFFFFFFFF;
        vertexCornerInPtr->specular = 0xFF000000;
        vertexCornerInPtr->u = texRemainingU + dUTex;
        vertexCornerInPtr->v = textureV + texVDelta;
        *g_MenuWinCBlend4DiffGraphicsObject->colorSignedBytePtr = 0;
        g_MenuWinCBlend4DiffGraphicsObject->paletteIndex = 0;
        g_MenuWinCBlend4DiffGraphicsObject->vertexCursor += g_MenuWinCBlend4DiffGraphicsObject->cursorStep;
        g_MenuWinCBlend4DiffGraphicsObject->colorIndex++;
        texRemainingU = 0.09375f;
        topRightCorner = g_MenuWinCBlend4DiffGraphicsObject->vertices;
        topRightCorner->x = (float)rect->x + rect->width - 8.0f;
        topRightCorner->y = (float)rect->y;
        topRightCorner->zBits = depth;
        topRightCorner->rhwBits = FLOAT_ONE_BITS;
        topRightCorner->color = 0xFFFFFFFF;
        topRightCorner->specular = 0xFF000000;
        topRightCorner->u = texRemainingU;
        topRightCorner->v = textureV;
        cornerRightTopPtr = g_MenuWinCBlend4DiffGraphicsObject->vertices + 1;
        cornerRightTopPtr->x = (float)rect->x + rect->width - 8.0f;
        cornerRightTopPtr->y = (float)rect->y + 8.0f;
        cornerRightTopPtr->zBits = depth;
        cornerRightTopPtr->rhwBits = FLOAT_ONE_BITS;
        cornerRightTopPtr->color = 0xFFFFFFFF;
        cornerRightTopPtr->specular = 0xFF000000;
        cornerRightTopPtr->u = texRemainingU;
        cornerRightTopPtr->v = textureV + texVDelta;
        vtxOuterCTRPtr = g_MenuWinCBlend4DiffGraphicsObject->vertices + 2;
        vtxOuterCTRPtr->x = (float)rect->x + rect->width - 8.0f + 8.0f;
        vtxOuterCTRPtr->y = (float)rect->y;
        vtxOuterCTRPtr->zBits = depth;
        vtxOuterCTRPtr->rhwBits = FLOAT_ONE_BITS;
        vtxOuterCTRPtr->color = 0xFFFFFFFF;
        vtxOuterCTRPtr->specular = 0xFF000000;
        vtxOuterCTRPtr->u = texRemainingU + dUTex;
        vtxOuterCTRPtr->v = textureV;
        vertexOuterCRT = g_MenuWinCBlend4DiffGraphicsObject->vertices + 3;
        vertexOuterCRT->x = (float)rect->x + rect->width - 8.0f + 8.0f;
        vertexOuterCRT->y = (float)rect->y + 8.0f;
        vertexOuterCRT->zBits = depth;
        vertexOuterCRT->rhwBits = FLOAT_ONE_BITS;
        vertexOuterCRT->color = 0xFFFFFFFF;
        vertexOuterCRT->specular = 0xFF000000;
        vertexOuterCRT->u = texRemainingU + dUTex;
        vertexOuterCRT->v = textureV + texVDelta;
        *g_MenuWinCBlend4DiffGraphicsObject->colorSignedBytePtr = 0;
        g_MenuWinCBlend4DiffGraphicsObject->paletteIndex = 0;
        g_MenuWinCBlend4DiffGraphicsObject->vertexCursor += g_MenuWinCBlend4DiffGraphicsObject->cursorStep;
        g_MenuWinCBlend4DiffGraphicsObject->colorIndex++;
        texRemainingU = 0.125f;
        textureV = 0.78125f;
        pBottomLeftCorner = g_MenuWinCBlend4DiffGraphicsObject->vertices;
        pBottomLeftCorner->x = (float)rect->x;
        pBottomLeftCorner->y = (float)rect->y + rect->height - 8.0f;
        pBottomLeftCorner->zBits = depth;
        pBottomLeftCorner->rhwBits = FLOAT_ONE_BITS;
        pBottomLeftCorner->color = 0xFFFFFFFF;
        pBottomLeftCorner->specular = 0xFF000000;
        pBottomLeftCorner->u = texRemainingU;
        pBottomLeftCorner->v = textureV;
        bottomRemainingCornerOut = g_MenuWinCBlend4DiffGraphicsObject->vertices + 1;
        bottomRemainingCornerOut->x = (float)rect->x;
        bottomRemainingCornerOut->y = (float)rect->y + rect->height - 8.0f + 8.0f;
        bottomRemainingCornerOut->zBits = depth;
        bottomRemainingCornerOut->rhwBits = FLOAT_ONE_BITS;
        bottomRemainingCornerOut->color = 0xFFFFFFFF;
        bottomRemainingCornerOut->specular = 0xFF000000;
        bottomRemainingCornerOut->u = texRemainingU;
        bottomRemainingCornerOut->v = textureV + texVDelta;
        pCornerBottomInner = g_MenuWinCBlend4DiffGraphicsObject->vertices + 2;
        pCornerBottomInner->x = (float)rect->x + 8.0f;
        pCornerBottomInner->y = (float)rect->y + rect->height - 8.0f;
        pCornerBottomInner->zBits = depth;
        pCornerBottomInner->rhwBits = FLOAT_ONE_BITS;
        pCornerBottomInner->color = 0xFFFFFFFF;
        pCornerBottomInner->specular = 0xFF000000;
        pCornerBottomInner->u = texRemainingU + dUTex;
        pCornerBottomInner->v = textureV;
        cornerInnerBottomLowPtr = g_MenuWinCBlend4DiffGraphicsObject->vertices + 3;
        cornerInnerBottomLowPtr->x = (float)rect->x + 8.0f;
        cornerInnerBottomLowPtr->y = (float)rect->y + rect->height - 8.0f + 8.0f;
        cornerInnerBottomLowPtr->zBits = depth;
        cornerInnerBottomLowPtr->rhwBits = FLOAT_ONE_BITS;
        cornerInnerBottomLowPtr->color = 0xFFFFFFFF;
        cornerInnerBottomLowPtr->specular = 0xFF000000;
        cornerInnerBottomLowPtr->u = texRemainingU + dUTex;
        cornerInnerBottomLowPtr->v = textureV + texVDelta;
        *g_MenuWinCBlend4DiffGraphicsObject->colorSignedBytePtr = 0;
        g_MenuWinCBlend4DiffGraphicsObject->paletteIndex = 0;
        g_MenuWinCBlend4DiffGraphicsObject->vertexCursor += g_MenuWinCBlend4DiffGraphicsObject->cursorStep;
        g_MenuWinCBlend4DiffGraphicsObject->colorIndex++;
        texRemainingU = 0.21875f;
        bottomRightCornerPtr = g_MenuWinCBlend4DiffGraphicsObject->vertices;
        bottomRightCornerPtr->x = (float)rect->x + rect->width - 8.0f;
        bottomRightCornerPtr->y = (float)rect->y + rect->height - 8.0f;
        bottomRightCornerPtr->zBits = depth;
        bottomRightCornerPtr->rhwBits = FLOAT_ONE_BITS;
        bottomRightCornerPtr->color = 0xFFFFFFFF;
        bottomRightCornerPtr->specular = 0xFF000000;
        bottomRightCornerPtr->u = texRemainingU;
        bottomRightCornerPtr->v = textureV;
        rightCornerBottomOut = g_MenuWinCBlend4DiffGraphicsObject->vertices + 1;
        rightCornerBottomOut->x = (float)rect->x + rect->width - 8.0f;
        rightCornerBottomOut->y = (float)rect->y + rect->height - 8.0f + 8.0f;
        rightCornerBottomOut->zBits = depth;
        rightCornerBottomOut->rhwBits = FLOAT_ONE_BITS;
        rightCornerBottomOut->color = 0xFFFFFFFF;
        rightCornerBottomOut->specular = 0xFF000000;
        rightCornerBottomOut->u = texRemainingU;
        rightCornerBottomOut->v = textureV + texVDelta;
        bottomCornerOut8 = g_MenuWinCBlend4DiffGraphicsObject->vertices + 2;
        bottomCornerOut8->x = (float)rect->x + rect->width - 8.0f + 8.0f;
        bottomCornerOut8->y = (float)rect->y + rect->height - 8.0f;
        bottomCornerOut8->zBits = depth;
        bottomCornerOut8->rhwBits = FLOAT_ONE_BITS;
        bottomCornerOut8->color = 0xFFFFFFFF;
        bottomCornerOut8->specular = 0xFF000000;
        bottomCornerOut8->u = texRemainingU + dUTex;
        bottomCornerOut8->v = textureV;
        vertexOuterCBRXPtr = g_MenuWinCBlend4DiffGraphicsObject->vertices + 3;
        vertexOuterCBRXPtr->x = (float)rect->x + rect->width - 8.0f + 8.0f;
        vertexOuterCBRXPtr->y = (float)rect->y + rect->height - 8.0f + 8.0f;
        vertexOuterCBRXPtr->zBits = depth;
        vertexOuterCBRXPtr->rhwBits = FLOAT_ONE_BITS;
        vertexOuterCBRXPtr->color = 0xFFFFFFFF;
        vertexOuterCBRXPtr->specular = 0xFF000000;
        vertexOuterCBRXPtr->u = texRemainingU + dUTex;
        vertexOuterCBRXPtr->v = textureV + texVDelta;
        *g_MenuWinCBlend4DiffGraphicsObject->colorSignedBytePtr = 0;
        g_MenuWinCBlend4DiffGraphicsObject->paletteIndex = 0;
    }
    if (rect->width > 12 && rect->height > 12) {
        innerMarginX = rect->x + 6;
        innerY = rect->y + 6;
        innerWidth = rect->width - 12;
        interiorH = rect->height - 12;
        if (g_MenuMessageWindowActive != 0) {
            if (AcquireBatchNode(1, g_MenuBlendWindowBgGraphicsObject) != 0) {
                pVtxBlendTL = g_MenuBlendWindowBgGraphicsObject->vertices;
                pVtxBlendTL->x = (float)innerMarginX;
                pVtxBlendTL->y = (float)innerY;
                pVtxBlendTL->zBits = depth;
                pVtxBlendTL->rhwBits = FLOAT_ONE_BITS;
                pVtxBlendTL->color = *(int *)&g_MenuCornerColorTL;
                pVtxBlendTL->specular = 0xFF000000;
                vtxBlendTR = g_MenuBlendWindowBgGraphicsObject->vertices + 1;
                vtxBlendTR->x = (float)innerMarginX;
                vtxBlendTR->y = (float)innerY + interiorH;
                vtxBlendTR->zBits = depth;
                vtxBlendTR->rhwBits = FLOAT_ONE_BITS;
                vtxBlendTR->color = *(int *)&g_MenuCornerColorTR;
                vtxBlendTR->specular = 0xFF000000;
                blendBgBottomLeft = g_MenuBlendWindowBgGraphicsObject->vertices + 2;
                blendBgBottomLeft->x = (float)innerMarginX + innerWidth;
                blendBgBottomLeft->y = (float)innerY;
                blendBgBottomLeft->zBits = depth;
                blendBgBottomLeft->rhwBits = FLOAT_ONE_BITS;
                blendBgBottomLeft->color = *(int *)&g_MenuCornerColorBL;
                blendBgBottomLeft->specular = 0xFF000000;
                blendBottomRightVtx = g_MenuBlendWindowBgGraphicsObject->vertices + 3;
                blendBottomRightVtx->x = (float)innerMarginX + innerWidth;
                blendBottomRightVtx->y = (float)innerY + interiorH;
                blendBottomRightVtx->zBits = depth;
                blendBottomRightVtx->rhwBits = FLOAT_ONE_BITS;
                blendBottomRightVtx->color = *(int *)&g_MenuCornerColorBR;
                blendBottomRightVtx->specular = 0xFF000000;
            }
        } else if (AcquireBatchNode(1, g_MenuWindowBgGraphicsObject) != 0) {
            windowBgTopRemaining = g_MenuWindowBgGraphicsObject->vertices;
            windowBgTopRemaining->x = (float)innerMarginX;
            windowBgTopRemaining->y = (float)innerY;
            windowBgTopRemaining->zBits = depth;
            windowBgTopRemaining->rhwBits = FLOAT_ONE_BITS;
            windowBgTopRemaining->color = *(int *)&g_MenuCornerColorTL;
            windowBgTopRemaining->specular = 0xFF000000;
            bgTopRightVtx = g_MenuWindowBgGraphicsObject->vertices + 1;
            bgTopRightVtx->x = (float)innerMarginX;
            bgTopRightVtx->y = (float)innerY + interiorH;
            bgTopRightVtx->zBits = depth;
            bgTopRightVtx->rhwBits = FLOAT_ONE_BITS;
            bgTopRightVtx->color = *(int *)&g_MenuCornerColorTR;
            bgTopRightVtx->specular = 0xFF000000;
            bgBottomLeftVtx = g_MenuWindowBgGraphicsObject->vertices + 2;
            bgBottomLeftVtx->x = (float)innerMarginX + innerWidth;
            bgBottomLeftVtx->y = (float)innerY;
            bgBottomLeftVtx->zBits = depth;
            bgBottomLeftVtx->rhwBits = FLOAT_ONE_BITS;
            bgBottomLeftVtx->color = *(int *)&g_MenuCornerColorBL;
            bgBottomLeftVtx->specular = 0xFF000000;
            bgBottomRightVtx = g_MenuWindowBgGraphicsObject->vertices + 3;
            bgBottomRightVtx->x = (float)innerMarginX + innerWidth;
            bgBottomRightVtx->y = (float)innerY + interiorH;
            bgBottomRightVtx->zBits = depth;
            bgBottomRightVtx->rhwBits = FLOAT_ONE_BITS;
            bgBottomRightVtx->color = *(int *)&g_MenuCornerColorBR;
            bgBottomRightVtx->specular = 0xFF000000;
        }
    }
}

/*
 * When the text subsystem is ready, draws a 1x popup frame from signed-short
 * rectangle coordinates plus global X/Y offsets, copying depth directly into depth
 * fields as its existing bit representation. Independently acquires two horizontal
 * atlas edges for width above 8, two vertical edges for height above 8, and four 4
 * by 4 corners unconditionally, with white primary/black secondary color,
 * reciprocal W 1 and palette 0.
 */
// FUNCTION: FF7 0x006f3492
void __cdecl Menu_DrawPopupMessageBody1x(MenuPopupBodyRect *rect, int depth)
{
    int innerWidth;
    int innerRemainingX;
    float vOrigin;
    GfxTlVertex *leftInnerLowVtxPtr;
    GfxTlVertex *pCornerO;
    GfxTlVertex *vertexNPtr;
    GfxTlVertex *vertRemainingTop;
    GfxTlVertex *rightLowVtx;
    GfxTlVertex *vertTopRightInner;
    GfxTlVertex *vertTopRightInnerLowPtr;
    GfxTlVertex *winBlendB;
    int yViewOrigin;
    float textureU;
    GfxTlVertex *v;
    GfxTlVertex *f;
    GfxTlVertex *rightInnerTopVtxPtr;
    GfxTlVertex *botRightVtx;
    GfxTlVertex *vertBotUpRight;
    GfxTlVertex *cornerInnerVtx;
    GfxTlVertex *pBotLeftCornerUpVtx;
    GfxTlVertex *blendWinTopLeftVtx;
    GfxTlVertex *vertBotRightCorner;
    GfxTlVertex *winPlainD;
    int xViewOrigin;
    GfxTlVertex *botLeftVtx;
    GfxTlVertex *upLeftBotVtx;
    GfxTlVertex *vertD;
    GfxTlVertex *pPopTrCorner;
    GfxTlVertex *cornerX;
    GfxTlVertex *vertBotRightCornerUp;
    GfxTlVertex *topLowLeftVtxPtr;
    GfxTlVertex *popInnerUpVtx;
    GfxTlVertex *winBgTopLeftVtx;
    GfxTlVertex *vertTopLeft;
    GfxTlVertex *topRightCornerVtx;
    GfxTlVertex *pVertexAD;
    float texStepV;
    GfxTlVertex *pCornerLowTLVtx;
    GfxTlVertex *blendWinLowRightVtx;
    GfxTlVertex *winBgTopRightVtx;
    GfxTlVertex *botRightInnerUpVtxPtr;
    GfxTlVertex *botInnerVtxPtr;
    int insetY;
    GfxTlVertex *pCornerTLVtx;
    GfxTlVertex *windowBlendC;
    GfxTlVertex *pVertWinLowLeft;
    GfxTlVertex *botInnerUpVtx;
    GfxTlVertex *vtxZPtr;
    int iterHeightMinus;
    float stepU;

    if (Menu_IsTextSubsystemReady() == 0) {
        return;
    }
    xViewOrigin = rect->x + g_MenuViewportX;
    yViewOrigin = rect->y + g_MenuViewportY;
    if (rect->extentX > 8) {
        if (AcquireBatchNode(2, g_MenuWinCBlend4DiffGraphicsObject) != 0) {
            textureU = 0.0f;
            vOrigin = 0.875f;
            stepU = 0.03125f;
            texStepV = 0.015625f;
            vertTopLeft = g_MenuWinCBlend4DiffGraphicsObject->vertices;
            vertTopLeft->x = (float)xViewOrigin + 4.0f;
            vertTopLeft->y = (float)yViewOrigin;
            vertTopLeft->zBits = depth;
            vertTopLeft->rhwBits = FLOAT_ONE_BITS;
            vertTopLeft->color = 0xFFFFFFFF;
            vertTopLeft->specular = 0xFF000000;
            vertTopLeft->u = textureU;
            vertTopLeft->v = vOrigin;
            topLowLeftVtxPtr = g_MenuWinCBlend4DiffGraphicsObject->vertices + 1;
            topLowLeftVtxPtr->x = (float)xViewOrigin + 4.0f;
            topLowLeftVtxPtr->y = (float)yViewOrigin + 4.0f;
            topLowLeftVtxPtr->zBits = depth;
            topLowLeftVtxPtr->rhwBits = FLOAT_ONE_BITS;
            topLowLeftVtxPtr->color = 0xFFFFFFFF;
            topLowLeftVtxPtr->specular = 0xFF000000;
            topLowLeftVtxPtr->u = textureU;
            topLowLeftVtxPtr->v = vOrigin + texStepV;
            pPopTrCorner = g_MenuWinCBlend4DiffGraphicsObject->vertices + 2;
            pPopTrCorner->x = (float)xViewOrigin + 4.0f + rect->extentX - 8.0f;
            pPopTrCorner->y = (float)yViewOrigin;
            pPopTrCorner->zBits = depth;
            pPopTrCorner->rhwBits = FLOAT_ONE_BITS;
            pPopTrCorner->color = 0xFFFFFFFF;
            pPopTrCorner->specular = 0xFF000000;
            pPopTrCorner->u = textureU + stepU;
            pPopTrCorner->v = vOrigin;
            vertD = g_MenuWinCBlend4DiffGraphicsObject->vertices + 3;
            vertD->x = (float)xViewOrigin + 4.0f + rect->extentX - 8.0f;
            vertD->y = (float)yViewOrigin + 4.0f;
            vertD->zBits = depth;
            vertD->rhwBits = FLOAT_ONE_BITS;
            vertD->color = 0xFFFFFFFF;
            vertD->specular = 0xFF000000;
            vertD->u = textureU + stepU;
            vertD->v = vOrigin + texStepV;
            *g_MenuWinCBlend4DiffGraphicsObject->colorSignedBytePtr = 0;
            g_MenuWinCBlend4DiffGraphicsObject->paletteIndex = 0;
            g_MenuWinCBlend4DiffGraphicsObject->vertexCursor += g_MenuWinCBlend4DiffGraphicsObject->cursorStep;
            g_MenuWinCBlend4DiffGraphicsObject->colorIndex++;
            textureU = 0.0625f;
            vOrigin = 0.921875f;
            stepU = 0.03125f;
            texStepV = 0.015625f;
            upLeftBotVtx = g_MenuWinCBlend4DiffGraphicsObject->vertices;
            upLeftBotVtx->x = (float)xViewOrigin + 4.0f;
            upLeftBotVtx->y = (float)yViewOrigin + rect->extentY - 4.0f;
            upLeftBotVtx->zBits = depth;
            upLeftBotVtx->rhwBits = FLOAT_ONE_BITS;
            upLeftBotVtx->color = 0xFFFFFFFF;
            upLeftBotVtx->specular = 0xFF000000;
            upLeftBotVtx->u = textureU;
            upLeftBotVtx->v = vOrigin;
            botLeftVtx = g_MenuWinCBlend4DiffGraphicsObject->vertices + 1;
            botLeftVtx->x = (float)xViewOrigin + 4.0f;
            botLeftVtx->y = (float)yViewOrigin + rect->extentY - 4.0f + 4.0f;
            botLeftVtx->zBits = depth;
            botLeftVtx->rhwBits = FLOAT_ONE_BITS;
            botLeftVtx->color = 0xFFFFFFFF;
            botLeftVtx->specular = 0xFF000000;
            botLeftVtx->u = textureU;
            botLeftVtx->v = vOrigin + texStepV;
            vertBotUpRight = g_MenuWinCBlend4DiffGraphicsObject->vertices + 2;
            vertBotUpRight->x = (float)xViewOrigin + 4.0f + rect->extentX - 8.0f;
            vertBotUpRight->y = (float)yViewOrigin + rect->extentY - 4.0f;
            vertBotUpRight->zBits = depth;
            vertBotUpRight->rhwBits = FLOAT_ONE_BITS;
            vertBotUpRight->color = 0xFFFFFFFF;
            vertBotUpRight->specular = 0xFF000000;
            vertBotUpRight->u = textureU + stepU;
            vertBotUpRight->v = vOrigin;
            botRightVtx = g_MenuWinCBlend4DiffGraphicsObject->vertices + 3;
            botRightVtx->x = (float)xViewOrigin + 4.0f + rect->extentX - 8.0f;
            botRightVtx->y = (float)yViewOrigin + rect->extentY - 4.0f + 4.0f;
            botRightVtx->zBits = depth;
            botRightVtx->rhwBits = FLOAT_ONE_BITS;
            botRightVtx->color = 0xFFFFFFFF;
            botRightVtx->specular = 0xFF000000;
            botRightVtx->u = textureU + stepU;
            botRightVtx->v = vOrigin + texStepV;
            *g_MenuWinCBlend4DiffGraphicsObject->colorSignedBytePtr = 0;
            g_MenuWinCBlend4DiffGraphicsObject->paletteIndex = 0;
        }
    }
    if (rect->extentY > 8) {
        if (AcquireBatchNode(2, g_MenuWinCBlend4DiffGraphicsObject) != 0) {
            textureU = 0.109375f;
            vOrigin = 0.9375f;
            stepU = 0.015625f;
            texStepV = 0.03125f;
            rightInnerTopVtxPtr = g_MenuWinCBlend4DiffGraphicsObject->vertices;
            rightInnerTopVtxPtr->x = (float)xViewOrigin + rect->extentX - 4.0f;
            rightInnerTopVtxPtr->y = (float)yViewOrigin + 4.0f;
            rightInnerTopVtxPtr->zBits = depth;
            rightInnerTopVtxPtr->rhwBits = FLOAT_ONE_BITS;
            rightInnerTopVtxPtr->color = 0xFFFFFFFF;
            rightInnerTopVtxPtr->specular = 0xFF000000;
            rightInnerTopVtxPtr->u = textureU;
            rightInnerTopVtxPtr->v = vOrigin;
            f = g_MenuWinCBlend4DiffGraphicsObject->vertices + 1;
            f->x = (float)xViewOrigin + rect->extentX - 4.0f;
            f->y = (float)yViewOrigin + 4.0f + rect->extentY - 8.0f;
            f->zBits = depth;
            f->rhwBits = FLOAT_ONE_BITS;
            f->color = 0xFFFFFFFF;
            f->specular = 0xFF000000;
            f->u = textureU;
            f->v = vOrigin + texStepV;
            v = g_MenuWinCBlend4DiffGraphicsObject->vertices + 2;
            v->x = (float)xViewOrigin + rect->extentX - 4.0f + 4.0f;
            v->y = (float)yViewOrigin + 4.0f;
            v->zBits = depth;
            v->rhwBits = FLOAT_ONE_BITS;
            v->color = 0xFFFFFFFF;
            v->specular = 0xFF000000;
            v->u = textureU + stepU;
            v->v = vOrigin;
            rightLowVtx = g_MenuWinCBlend4DiffGraphicsObject->vertices + 3;
            rightLowVtx->x = (float)xViewOrigin + rect->extentX - 4.0f + 4.0f;
            rightLowVtx->y = (float)yViewOrigin + 4.0f + rect->extentY - 8.0f;
            rightLowVtx->zBits = depth;
            rightLowVtx->rhwBits = FLOAT_ONE_BITS;
            rightLowVtx->color = 0xFFFFFFFF;
            rightLowVtx->specular = 0xFF000000;
            rightLowVtx->u = textureU + stepU;
            rightLowVtx->v = vOrigin + texStepV;
            *g_MenuWinCBlend4DiffGraphicsObject->colorSignedBytePtr = 0;
            g_MenuWinCBlend4DiffGraphicsObject->paletteIndex = 0;
            g_MenuWinCBlend4DiffGraphicsObject->vertexCursor += g_MenuWinCBlend4DiffGraphicsObject->cursorStep;
            g_MenuWinCBlend4DiffGraphicsObject->colorIndex++;
            textureU = 0.0f;
            vOrigin = 0.921875f;
            stepU = 0.015625f;
            texStepV = 0.03125f;
            vertRemainingTop = g_MenuWinCBlend4DiffGraphicsObject->vertices;
            vertRemainingTop->x = (float)xViewOrigin;
            vertRemainingTop->y = (float)yViewOrigin + 4.0f;
            vertRemainingTop->zBits = depth;
            vertRemainingTop->rhwBits = FLOAT_ONE_BITS;
            vertRemainingTop->color = 0xFFFFFFFF;
            vertRemainingTop->specular = 0xFF000000;
            vertRemainingTop->u = textureU;
            vertRemainingTop->v = vOrigin;
            vertexNPtr = g_MenuWinCBlend4DiffGraphicsObject->vertices + 1;
            vertexNPtr->x = (float)xViewOrigin;
            vertexNPtr->y = (float)yViewOrigin + 4.0f + rect->extentY - 8.0f;
            vertexNPtr->zBits = depth;
            vertexNPtr->rhwBits = FLOAT_ONE_BITS;
            vertexNPtr->color = 0xFFFFFFFF;
            vertexNPtr->specular = 0xFF000000;
            vertexNPtr->u = textureU;
            vertexNPtr->v = vOrigin + texStepV;
            pCornerO = g_MenuWinCBlend4DiffGraphicsObject->vertices + 2;
            pCornerO->x = (float)xViewOrigin + 4.0f;
            pCornerO->y = (float)yViewOrigin + 4.0f;
            pCornerO->zBits = depth;
            pCornerO->rhwBits = FLOAT_ONE_BITS;
            pCornerO->color = 0xFFFFFFFF;
            pCornerO->specular = 0xFF000000;
            pCornerO->u = textureU + stepU;
            pCornerO->v = vOrigin;
            leftInnerLowVtxPtr = g_MenuWinCBlend4DiffGraphicsObject->vertices + 3;
            leftInnerLowVtxPtr->x = (float)xViewOrigin + 4.0f;
            leftInnerLowVtxPtr->y = (float)yViewOrigin + 4.0f + rect->extentY - 8.0f;
            leftInnerLowVtxPtr->zBits = depth;
            leftInnerLowVtxPtr->rhwBits = FLOAT_ONE_BITS;
            leftInnerLowVtxPtr->color = 0xFFFFFFFF;
            leftInnerLowVtxPtr->specular = 0xFF000000;
            leftInnerLowVtxPtr->u = textureU + stepU;
            leftInnerLowVtxPtr->v = vOrigin + texStepV;
            *g_MenuWinCBlend4DiffGraphicsObject->colorSignedBytePtr = 0;
            g_MenuWinCBlend4DiffGraphicsObject->paletteIndex = 0;
        }
    }
    if (AcquireBatchNode(4, g_MenuWinCBlend4DiffGraphicsObject) != 0) {
        texStepV = 0.015625f;
        stepU = texStepV;
        textureU = 0.0f;
        vOrigin = 0.90625f;
        pCornerTLVtx = g_MenuWinCBlend4DiffGraphicsObject->vertices;
        pCornerTLVtx->x = (float)xViewOrigin;
        pCornerTLVtx->y = (float)yViewOrigin;
        pCornerTLVtx->zBits = depth;
        pCornerTLVtx->rhwBits = FLOAT_ONE_BITS;
        pCornerTLVtx->color = 0xFFFFFFFF;
        pCornerTLVtx->specular = 0xFF000000;
        pCornerTLVtx->u = textureU;
        pCornerTLVtx->v = vOrigin;
        pCornerLowTLVtx = g_MenuWinCBlend4DiffGraphicsObject->vertices + 1;
        pCornerLowTLVtx->x = (float)xViewOrigin;
        pCornerLowTLVtx->y = (float)yViewOrigin + 4.0f;
        pCornerLowTLVtx->zBits = depth;
        pCornerLowTLVtx->rhwBits = FLOAT_ONE_BITS;
        pCornerLowTLVtx->color = 0xFFFFFFFF;
        pCornerLowTLVtx->specular = 0xFF000000;
        pCornerLowTLVtx->u = textureU;
        pCornerLowTLVtx->v = vOrigin + texStepV;
        popInnerUpVtx = g_MenuWinCBlend4DiffGraphicsObject->vertices + 2;
        popInnerUpVtx->x = (float)xViewOrigin + 4.0f;
        popInnerUpVtx->y = (float)yViewOrigin;
        popInnerUpVtx->zBits = depth;
        popInnerUpVtx->rhwBits = FLOAT_ONE_BITS;
        popInnerUpVtx->color = 0xFFFFFFFF;
        popInnerUpVtx->specular = 0xFF000000;
        popInnerUpVtx->u = textureU + stepU;
        popInnerUpVtx->v = vOrigin;
        cornerInnerVtx = g_MenuWinCBlend4DiffGraphicsObject->vertices + 3;
        cornerInnerVtx->x = (float)xViewOrigin + 4.0f;
        cornerInnerVtx->y = (float)yViewOrigin + 4.0f;
        cornerInnerVtx->zBits = depth;
        cornerInnerVtx->rhwBits = FLOAT_ONE_BITS;
        cornerInnerVtx->color = 0xFFFFFFFF;
        cornerInnerVtx->specular = 0xFF000000;
        cornerInnerVtx->u = textureU + stepU;
        cornerInnerVtx->v = vOrigin + texStepV;
        *g_MenuWinCBlend4DiffGraphicsObject->colorSignedBytePtr = 0;
        g_MenuWinCBlend4DiffGraphicsObject->paletteIndex = 0;
        g_MenuWinCBlend4DiffGraphicsObject->vertexCursor += g_MenuWinCBlend4DiffGraphicsObject->cursorStep;
        g_MenuWinCBlend4DiffGraphicsObject->colorIndex++;
        textureU = 0.046875f;
        vertTopRightInner = g_MenuWinCBlend4DiffGraphicsObject->vertices;
        vertTopRightInner->x = (float)xViewOrigin + rect->extentX - 4.0f;
        vertTopRightInner->y = (float)yViewOrigin;
        vertTopRightInner->zBits = depth;
        vertTopRightInner->rhwBits = FLOAT_ONE_BITS;
        vertTopRightInner->color = 0xFFFFFFFF;
        vertTopRightInner->specular = 0xFF000000;
        vertTopRightInner->u = textureU;
        vertTopRightInner->v = vOrigin;
        vertTopRightInnerLowPtr = g_MenuWinCBlend4DiffGraphicsObject->vertices + 1;
        vertTopRightInnerLowPtr->x = (float)xViewOrigin + rect->extentX - 4.0f;
        vertTopRightInnerLowPtr->y = (float)yViewOrigin + 4.0f;
        vertTopRightInnerLowPtr->zBits = depth;
        vertTopRightInnerLowPtr->rhwBits = FLOAT_ONE_BITS;
        vertTopRightInnerLowPtr->color = 0xFFFFFFFF;
        vertTopRightInnerLowPtr->specular = 0xFF000000;
        vertTopRightInnerLowPtr->u = textureU;
        vertTopRightInnerLowPtr->v = vOrigin + texStepV;
        topRightCornerVtx = g_MenuWinCBlend4DiffGraphicsObject->vertices + 2;
        topRightCornerVtx->x = (float)xViewOrigin + rect->extentX - 4.0f + 4.0f;
        topRightCornerVtx->y = (float)yViewOrigin;
        topRightCornerVtx->zBits = depth;
        topRightCornerVtx->rhwBits = FLOAT_ONE_BITS;
        topRightCornerVtx->color = 0xFFFFFFFF;
        topRightCornerVtx->specular = 0xFF000000;
        topRightCornerVtx->u = textureU + stepU;
        topRightCornerVtx->v = vOrigin;
        cornerX = g_MenuWinCBlend4DiffGraphicsObject->vertices + 3;
        cornerX->x = (float)xViewOrigin + rect->extentX - 4.0f + 4.0f;
        cornerX->y = (float)yViewOrigin + 4.0f;
        cornerX->zBits = depth;
        cornerX->rhwBits = FLOAT_ONE_BITS;
        cornerX->color = 0xFFFFFFFF;
        cornerX->specular = 0xFF000000;
        cornerX->u = textureU + stepU;
        cornerX->v = vOrigin + texStepV;
        *g_MenuWinCBlend4DiffGraphicsObject->colorSignedBytePtr = 0;
        g_MenuWinCBlend4DiffGraphicsObject->paletteIndex = 0;
        g_MenuWinCBlend4DiffGraphicsObject->vertexCursor += g_MenuWinCBlend4DiffGraphicsObject->cursorStep;
        g_MenuWinCBlend4DiffGraphicsObject->colorIndex++;
        textureU = 0.0625f;
        vOrigin = 0.890625f;
        pBotLeftCornerUpVtx = g_MenuWinCBlend4DiffGraphicsObject->vertices;
        pBotLeftCornerUpVtx->x = (float)xViewOrigin;
        pBotLeftCornerUpVtx->y = (float)yViewOrigin + rect->extentY - 4.0f;
        pBotLeftCornerUpVtx->zBits = depth;
        pBotLeftCornerUpVtx->rhwBits = FLOAT_ONE_BITS;
        pBotLeftCornerUpVtx->color = 0xFFFFFFFF;
        pBotLeftCornerUpVtx->specular = 0xFF000000;
        pBotLeftCornerUpVtx->u = textureU;
        pBotLeftCornerUpVtx->v = vOrigin;
        vtxZPtr = g_MenuWinCBlend4DiffGraphicsObject->vertices + 1;
        vtxZPtr->x = (float)xViewOrigin;
        vtxZPtr->y = (float)yViewOrigin + rect->extentY - 4.0f + 4.0f;
        vtxZPtr->zBits = depth;
        vtxZPtr->rhwBits = FLOAT_ONE_BITS;
        vtxZPtr->color = 0xFFFFFFFF;
        vtxZPtr->specular = 0xFF000000;
        vtxZPtr->u = textureU;
        vtxZPtr->v = vOrigin + texStepV;
        botInnerUpVtx = g_MenuWinCBlend4DiffGraphicsObject->vertices + 2;
        botInnerUpVtx->x = (float)xViewOrigin + 4.0f;
        botInnerUpVtx->y = (float)yViewOrigin + rect->extentY - 4.0f;
        botInnerUpVtx->zBits = depth;
        botInnerUpVtx->rhwBits = FLOAT_ONE_BITS;
        botInnerUpVtx->color = 0xFFFFFFFF;
        botInnerUpVtx->specular = 0xFF000000;
        botInnerUpVtx->u = textureU + stepU;
        botInnerUpVtx->v = vOrigin;
        botInnerVtxPtr = g_MenuWinCBlend4DiffGraphicsObject->vertices + 3;
        botInnerVtxPtr->x = (float)xViewOrigin + 4.0f;
        botInnerVtxPtr->y = (float)yViewOrigin + rect->extentY - 4.0f + 4.0f;
        botInnerVtxPtr->zBits = depth;
        botInnerVtxPtr->rhwBits = FLOAT_ONE_BITS;
        botInnerVtxPtr->color = 0xFFFFFFFF;
        botInnerVtxPtr->specular = 0xFF000000;
        botInnerVtxPtr->u = textureU + stepU;
        botInnerVtxPtr->v = vOrigin + texStepV;
        *g_MenuWinCBlend4DiffGraphicsObject->colorSignedBytePtr = 0;
        g_MenuWinCBlend4DiffGraphicsObject->paletteIndex = 0;
        g_MenuWinCBlend4DiffGraphicsObject->vertexCursor += g_MenuWinCBlend4DiffGraphicsObject->cursorStep;
        g_MenuWinCBlend4DiffGraphicsObject->colorIndex++;
        textureU = 0.109375f;
        botRightInnerUpVtxPtr = g_MenuWinCBlend4DiffGraphicsObject->vertices;
        botRightInnerUpVtxPtr->x = (float)xViewOrigin + rect->extentX - 4.0f;
        botRightInnerUpVtxPtr->y = (float)yViewOrigin + rect->extentY - 4.0f;
        botRightInnerUpVtxPtr->zBits = depth;
        botRightInnerUpVtxPtr->rhwBits = FLOAT_ONE_BITS;
        botRightInnerUpVtxPtr->color = 0xFFFFFFFF;
        botRightInnerUpVtxPtr->specular = 0xFF000000;
        botRightInnerUpVtxPtr->u = textureU;
        botRightInnerUpVtxPtr->v = vOrigin;
        pVertexAD = g_MenuWinCBlend4DiffGraphicsObject->vertices + 1;
        pVertexAD->x = (float)xViewOrigin + rect->extentX - 4.0f;
        pVertexAD->y = (float)yViewOrigin + rect->extentY - 4.0f + 4.0f;
        pVertexAD->zBits = depth;
        pVertexAD->rhwBits = FLOAT_ONE_BITS;
        pVertexAD->color = 0xFFFFFFFF;
        pVertexAD->specular = 0xFF000000;
        pVertexAD->u = textureU;
        pVertexAD->v = vOrigin + texStepV;
        vertBotRightCornerUp = g_MenuWinCBlend4DiffGraphicsObject->vertices + 2;
        vertBotRightCornerUp->x = (float)xViewOrigin + rect->extentX - 4.0f + 4.0f;
        vertBotRightCornerUp->y = (float)yViewOrigin + rect->extentY - 4.0f;
        vertBotRightCornerUp->zBits = depth;
        vertBotRightCornerUp->rhwBits = FLOAT_ONE_BITS;
        vertBotRightCornerUp->color = 0xFFFFFFFF;
        vertBotRightCornerUp->specular = 0xFF000000;
        vertBotRightCornerUp->u = textureU + stepU;
        vertBotRightCornerUp->v = vOrigin;
        vertBotRightCorner = g_MenuWinCBlend4DiffGraphicsObject->vertices + 3;
        vertBotRightCorner->x = (float)xViewOrigin + rect->extentX - 4.0f + 4.0f;
        vertBotRightCorner->y = (float)yViewOrigin + rect->extentY - 4.0f + 4.0f;
        vertBotRightCorner->zBits = depth;
        vertBotRightCorner->rhwBits = FLOAT_ONE_BITS;
        vertBotRightCorner->color = 0xFFFFFFFF;
        vertBotRightCorner->specular = 0xFF000000;
        vertBotRightCorner->u = textureU + stepU;
        vertBotRightCorner->v = vOrigin + texStepV;
        *g_MenuWinCBlend4DiffGraphicsObject->colorSignedBytePtr = 0;
        g_MenuWinCBlend4DiffGraphicsObject->paletteIndex = 0;
    }
    if (rect->extentX > 6 && rect->extentY > 6) {
        innerRemainingX = xViewOrigin + 3;
        insetY = yViewOrigin + 3;
        innerWidth = rect->extentX - 6;
        iterHeightMinus = rect->extentY - 6;
        if (g_MenuMessageWindowActive != 0) {
            if (AcquireBatchNode(1, g_MenuBlendWindowBgGraphicsObject) != 0) {
                blendWinTopLeftVtx = g_MenuBlendWindowBgGraphicsObject->vertices;
                blendWinTopLeftVtx->x = (float)innerRemainingX;
                blendWinTopLeftVtx->y = (float)insetY;
                blendWinTopLeftVtx->zBits = depth;
                blendWinTopLeftVtx->rhwBits = FLOAT_ONE_BITS;
                blendWinTopLeftVtx->color = *(int *)&g_MenuCornerColorTL;
                blendWinTopLeftVtx->specular = 0xFF000000;
                winBlendB = g_MenuBlendWindowBgGraphicsObject->vertices + 1;
                winBlendB->x = (float)innerRemainingX;
                winBlendB->y = (float)insetY + iterHeightMinus;
                winBlendB->zBits = depth;
                winBlendB->rhwBits = FLOAT_ONE_BITS;
                winBlendB->color = *(int *)&g_MenuCornerColorTR;
                winBlendB->specular = 0xFF000000;
                windowBlendC = g_MenuBlendWindowBgGraphicsObject->vertices + 2;
                windowBlendC->x = (float)innerRemainingX + innerWidth;
                windowBlendC->y = (float)insetY;
                windowBlendC->zBits = depth;
                windowBlendC->rhwBits = FLOAT_ONE_BITS;
                windowBlendC->color = *(int *)&g_MenuCornerColorBL;
                windowBlendC->specular = 0xFF000000;
                blendWinLowRightVtx = g_MenuBlendWindowBgGraphicsObject->vertices + 3;
                blendWinLowRightVtx->x = (float)innerRemainingX + innerWidth;
                blendWinLowRightVtx->y = (float)insetY + iterHeightMinus;
                blendWinLowRightVtx->zBits = depth;
                blendWinLowRightVtx->rhwBits = FLOAT_ONE_BITS;
                blendWinLowRightVtx->color = *(int *)&g_MenuCornerColorBR;
                blendWinLowRightVtx->specular = 0xFF000000;
            }
        } else if (AcquireBatchNode(1, g_MenuWindowBgGraphicsObject) != 0) {
            winBgTopLeftVtx = g_MenuWindowBgGraphicsObject->vertices;
            winBgTopLeftVtx->x = (float)innerRemainingX;
            winBgTopLeftVtx->y = (float)insetY;
            winBgTopLeftVtx->zBits = depth;
            winBgTopLeftVtx->rhwBits = FLOAT_ONE_BITS;
            winBgTopLeftVtx->color = *(int *)&g_MenuCornerColorTL;
            winBgTopLeftVtx->specular = 0xFF000000;
            pVertWinLowLeft = g_MenuWindowBgGraphicsObject->vertices + 1;
            pVertWinLowLeft->x = (float)innerRemainingX;
            pVertWinLowLeft->y = (float)insetY + iterHeightMinus;
            pVertWinLowLeft->zBits = depth;
            pVertWinLowLeft->rhwBits = FLOAT_ONE_BITS;
            pVertWinLowLeft->color = *(int *)&g_MenuCornerColorTR;
            pVertWinLowLeft->specular = 0xFF000000;
            winBgTopRightVtx = g_MenuWindowBgGraphicsObject->vertices + 2;
            winBgTopRightVtx->x = (float)innerRemainingX + innerWidth;
            winBgTopRightVtx->y = (float)insetY;
            winBgTopRightVtx->zBits = depth;
            winBgTopRightVtx->rhwBits = FLOAT_ONE_BITS;
            winBgTopRightVtx->color = *(int *)&g_MenuCornerColorBL;
            winBgTopRightVtx->specular = 0xFF000000;
            winPlainD = g_MenuWindowBgGraphicsObject->vertices + 3;
            winPlainD->x = (float)innerRemainingX + innerWidth;
            winPlainD->y = (float)insetY + iterHeightMinus;
            winPlainD->zBits = depth;
            winPlainD->rhwBits = FLOAT_ONE_BITS;
            winPlainD->color = *(int *)&g_MenuCornerColorBR;
            winPlainD->specular = 0xFF000000;
        }
    }
}

/*
 * Write the four vertices of an axis-aligned quad into the batch node's buffer:
 * (x,y), (x,y+h), (x+w,y), (x+w,y+h), all at depth 0, rhw 1.0, black at the
 * backdrop alpha g_MenuDigitBackdropAlpha with a black specular. The far edges are
 * `(float)x + w`, converted before the add.
 */
// FUNCTION: FF7 0x006f4b91
void __cdecl Menu_DrawTranslucentBlackRect(int x, int y, int w, int h)
{
    int rectW;
    unsigned char fillColor[4];
    int quadHeight;
    GfxTlVertex *topLeft;
    int depthBits;
    GfxTlVertex *bottomRightPtr;
    GfxTlVertex *bottomLeftPtr;
    GfxTlVertex *pVtxTR;

    depthBits = 0;
    fillColor[0] = 0;
    fillColor[1] = fillColor[0];
    fillColor[2] = fillColor[1];
    fillColor[3] = g_MenuDigitBackdropAlpha;
    rectW = w;
    quadHeight = h;
    if (AcquireBatchNode(1, g_MenuTextBoxQuadGraphicsObject) != 0) {
        topLeft = g_MenuTextBoxQuadGraphicsObject->vertices;
        topLeft->x = (float)x;
        topLeft->y = (float)y;
        topLeft->zBits = depthBits;
        topLeft->rhwBits = FLOAT_ONE_BITS;
        topLeft->color = *(int *)fillColor;
        topLeft->specular = 0xff000000;
        bottomLeftPtr = g_MenuTextBoxQuadGraphicsObject->vertices + 1;
        bottomLeftPtr->x = (float)x;
        bottomLeftPtr->y = (float)y + quadHeight;
        bottomLeftPtr->zBits = depthBits;
        bottomLeftPtr->rhwBits = FLOAT_ONE_BITS;
        bottomLeftPtr->color = *(int *)fillColor;
        bottomLeftPtr->specular = 0xff000000;
        pVtxTR = g_MenuTextBoxQuadGraphicsObject->vertices + 2;
        pVtxTR->x = (float)x + rectW;
        pVtxTR->y = (float)y;
        pVtxTR->zBits = depthBits;
        pVtxTR->rhwBits = FLOAT_ONE_BITS;
        pVtxTR->color = *(int *)fillColor;
        pVtxTR->specular = 0xff000000;
        bottomRightPtr = g_MenuTextBoxQuadGraphicsObject->vertices + 3;
        bottomRightPtr->x = (float)x + rectW;
        bottomRightPtr->y = (float)y + quadHeight;
        bottomRightPtr->zBits = depthBits;
        bottomRightPtr->rhwBits = FLOAT_ONE_BITS;
        bottomRightPtr->color = *(int *)fillColor;
        bottomRightPtr->specular = 0xff000000;
    }
}

/*
 * Sets g_MenuDigitBackdropEnabled (the countdown timer then draws a translucent black
 * backdrop behind its digits) and the backdrop's alpha byte g_MenuDigitBackdropAlpha.
 */
// FUNCTION: FF7 0x006f4cff
void __cdecl Menu_SetDigitBackdropQuad(int enabled, char alpha)
{
    g_MenuDigitBackdropEnabled = enabled;
    g_MenuDigitBackdropAlpha = alpha;
}

/*
 * Sets g_MenuShouldDrawBlackQuad, a text-drawing parameter.
 */
// FUNCTION: FF7 0x006f4d15
void __cdecl Menu_SetMessageWindowBlackQuadEnabled(int value)
{
    g_MenuShouldDrawBlackQuad = value;
}
} /* extern "C" */
