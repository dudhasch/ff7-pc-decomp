// src/ff7/highway/highway_hud_render_setup.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Sets up the highway's rendering: resource and sprite setup for the render owner, the
//   timed effect slots, and the mode controller that draws the HUD, digits, numbers and fade.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include <string.h>
#include "ff7/highway_palette.h"
#include "ff7/highway_psxdata.h"
#include <math.h>
#include <stdlib.h>
#include "ff7/types/highway.h"
#include "lib/types/graphics.h"
#include "lib/enums/graphics.h"
#include "ff7/api/highway.h"
#define FF7_GFX_EMITFLATRECT_FLOAT_Z
#include "lib/api/graphics.h"
#include "lib/pfile.h"

#pragma intrinsic(strlen, strcpy, strcat, strcmp)
extern "C" {
#pragma intrinsic(memset)
#pragma intrinsic(atan2, sqrt)

// clang-format off
// The TIM names of both tables are their initializer literals, emitted after the two tables.
// GLOBAL: FF7 0x009096a0
const char *g_HighwaySpriteTimNames1[6] = {
    // STRING: FF7 0x009096c4
    "guaa.tim",
    // STRING: FF7 0x009096d0
    "huaa.tim",
    // STRING: FF7 0x009096dc
    "iuaa.tim",
    // STRING: FF7 0x009096e8
    "juaa.tim",
    // STRING: FF7 0x009096f4
    "kuaa.tim",
    // STRING: FF7 0x00909700
    "luaa.tim",
};
// GLOBAL: FF7 0x009096b8
const char *g_HighwaySpriteTimNames2[3] = {
    // STRING: FF7 0x0090970c
    "muaa.tim",
    // STRING: FF7 0x00909718
    "nuaa.tim",
    0,
};
// clang-format on

/* float z makes integer literal 0 emit VC5's 5-byte push 0.0f instead of 2-byte push 0. */
int __cdecl Gfx_EmitFlatRect(int x, int y, int width, int height, unsigned int colour, int flags,
                             float z, int surface);

#pragma intrinsic(abs)
} // extern "C"

/*
 * Initializes zero-blend load options, stores the global asset source mode and sets option bit 0x4000,
 * then creates an eight-kind render object from the engine context's render object bank. Sets bit
 * two of its emitterFlags and returns it without a null-result check.
 */
// FUNCTION: FF7 0x00658700
void *HighwayRenderOwner::create()
{
    // VC5's name buckets place result -4, context -8, options -0x7c.
    GfxRenderObject *result;
    PfileLoadOptions loadOptions;
    GfxEngineContext *context;

    context = GetEngineContext();
    result = 0;
    memset(&loadOptions, 0, sizeof(loadOptions));
    Pfile_InitLoadOptionsWithBlend(BLEND_AVERAGE, &loadOptions);
    loadOptions.sourceMode = g_HighwayAssetSourceMode;
    Pfile_SetLoadOptionsBlendMode(BLEND_AVERAGE, &loadOptions);
    loadOptions.renderFlags |= 0x4000;
    result = (GfxRenderObject *)Gfx_CreateRenderObject(1, GFX_DRAW_FLAT_QUAD_2D, &loadOptions, 0, (void *)context->renderObjectBank);
    result->emitterFlags = result->emitterFlags | 2;
    return result;
}

/*
 * Zeroes nine resource entries and populates six from one table when the global equals one,
 * otherwise two from the other. Creates the sprite group, clears the leading int, calls initField
 * only for global zero, creates the render object and clears fade byte and flag.
 */
// FUNCTION: FF7 0x0065878e
HighwayRenderOwner::HighwayRenderOwner()
{
    int entries[9];
    int i;
    int j;

    entries[0] = 0;
    entries[1] = 0;
    entries[2] = 0;
    entries[3] = 0;
    entries[4] = 0;
    entries[5] = 0;
    entries[6] = 0;
    entries[7] = 0;
    entries[8] = 0;
    if (g_HighwayGameMode == 1) {
        for (i = 0; i <= 5; i++) {
            entries[i] = (int)g_HighwaySpriteTimNames1[i];
        }
    } else {
        for (j = 0; j <= 1; j++) {
            entries[j] = (int)g_HighwaySpriteTimNames2[j];
        }
    }
    this->spriteGroup = new HighwayScaledSpriteGroup(entries);
    this->banner = 0;
    if (g_HighwayGameMode == 0) {
        initField();
    }
    this->renderObject = (GfxRenderObject *)create();
    this->fadeAlpha = 0;
    this->fadeActive = 0;
}

/*
 * Deletes the sprite group, then passes the render object to DestroyRenderObject.
 */
// FUNCTION: FF7 0x006588de
HighwayRenderOwner::~HighwayRenderOwner()
{
    delete this->spriteGroup;
    DestroyRenderObject(this->renderObject);
}

/*
 * Initializes the five HUD slots. Stores each slot's position and the opposite corner derived from
 * it, then its texture coordinates, sizes and offset. Slot 0 spans (116, 200) through (141, 217);
 * the other four use fixed X 20 or 217, Y 20 or 40. Does not clear the object or touch anything but
 * the slots.
 */
// FUNCTION: FF7 0x0065893e
void HighwayRenderOwner::initField()
{
    HighwayHudSlot *record = this->slots;

    record[4].posX = 217;
    record[4].posY = 40;
    record[4].drawX = record[4].posX + 2;
    record[4].drawY = record[4].posY + 9;
    record[4].texU[0] = 0;
    record[4].texV[0] = 0;
    record[4].texU[1] = 0;
    record[4].texV[1] = 0x54;
    record[4].texU[2] = 0x1C;
    record[4].texV[2] = 0x54;
    record[4].texU[3] = 0x38;
    record[4].texV[3] = 0x54;
    record[4].width = 84;
    record[4].height = 16;
    record[4].size = 14;
    record[4].offsetX = 0x45;

    record[2].posX = 217;
    record[2].posY = 20;
    record[2].drawX = record[2].posX + 2;
    record[2].drawY = record[2].posY + 9;
    record[2].texU[0] = 0;
    record[2].texV[0] = 0;
    record[2].texU[1] = 0;
    record[2].texV[1] = 0x1C;
    record[2].texU[2] = 0x1C;
    record[2].texV[2] = 0x1C;
    record[2].texU[3] = 0x38;
    record[2].texV[3] = 0x1C;
    record[2].width = 84;
    record[2].height = 16;
    record[2].size = 14;
    record[2].offsetX = 0x45;
    record[3].posX = 20;
    record[3].posY = 20;
    record[3].drawX = record[3].posX + 17;
    record[3].drawY = record[3].posY + 9;
    record[3].texU[0] = 0;
    record[3].texV[0] = 0x22;
    record[3].texU[1] = 0;
    record[3].texV[1] = 0x38;
    record[3].texU[2] = 0x1C;
    record[3].texV[2] = 0x38;
    record[3].texU[3] = 0x38;
    record[3].texV[3] = 0x38;
    record[3].width = 84;
    record[3].height = 16;
    record[3].size = 14;
    record[3].offsetX = 1;

    record[1].posX = 20;
    record[1].posY = 40;
    record[1].drawX = record[1].posX + 17;
    record[1].drawY = record[1].posY + 9;
    record[1].texU[0] = 0;
    record[1].texV[0] = 0x22;
    record[1].texU[1] = 0;
    record[1].texV[1] = 0;
    record[1].texU[2] = 0x1C;
    record[1].texV[2] = 0;
    record[1].texU[3] = 0x38;
    record[1].texV[3] = 0;
    record[1].width = 84;
    record[1].height = 16;
    record[1].size = 14;
    record[1].offsetX = 1;

    record[0].posX = 116;
    record[0].posY = 200;
    record[0].drawX = record[0].posX + 25;
    record[0].drawY = record[0].posY + 17;
    record[0].texU[0] = 0;
    record[0].texV[0] = 0x44;
    record[0].texU[1] = 0;
    record[0].texV[1] = 0x70;
    record[0].texU[2] = 0x2C;
    record[0].texV[2] = 0x70;
    record[0].texU[3] = 0x58;
    record[0].texV[3] = 0x70;
    record[0].width = 92;
    record[0].height = 24;
    record[0].size = 22;
    record[0].offsetX = 1;
}

/*
 * Animates the five characters' HP bars (g_HighwayHpBarTable), draws each, then lowers its
 * flash timer. For each entry a rand modulo 30 zero can change the icon state from one to three
 * while its remaining HP exceeds ten. Decrements icon states above one, calls process with the
 * flash timer and icon state, then subtracts eight from a positive flash timer with byte wrapping
 * rather than saturation.
 */
// FUNCTION: FF7 0x00658bfa
void HighwayRenderOwner::tick()
{
    HighwayHudStatusEntry *index = g_HighwayHpBarTable.bars;
    int p;
    for (p = 0; p < 5; p++) {
        if (rand() % 30 == 0 &&
            index[p].iconState == 1 &&
            index[p].remaining > 10) {
            index[p].iconState = 3;
        }
        if (index[p].iconState > 1) {
            index[p].iconState--;
        }
        process((short)p, index[p].flashTimer, index[p].iconState);
        if (index[p].flashTimer > 0) {
            index[p].flashTimer -= 8;
        }
    }
}

/*
 * Draws a character's animated icon, background and nonzero HP bar. Tick supplies five
 * indices; the bar's width is the entry's HP fraction over its full value.
 * Unsigned byte conversion and wrapping are intentional. The scaled drawing member
 * is inlined by the TU's existing VC5 /Ob1 setting, including its argument copies.
 */
// FUNCTION: FF7 0x00658cff
void HighwayRenderOwner::process(short index, char value, char state)
{
    // VC5 name buckets retain the ten working slots; inlining supplies the argument copies.
    unsigned char v040m; // Icon green.
    unsigned char v036i; // Bar red.
    unsigned char v032i;
    unsigned char v028a;
    unsigned char v024a;
    int v020a;
    HighwayHudStatusEntry *v016i;
    HighwayHudSlot *v012j;
    unsigned char v008a;
    float v004b;
    v016i = g_HighwayHpBarTable.bars;
    v004b = 1.0f * g_pHighwaySceneBlock->scale;
    if (v016i[index].total) {
        v020a = ((v016i[index].remaining << 6) / v016i[index].total) - 1;
    } else {
        v020a = 0;
    }
    v036i = (unsigned char)value * 2;
    v008a = 128 - (unsigned char)value;
    v032i = 0;
    if (v016i[index].remaining <= 0) {
        v028a = 48;
        v040m = 48;
        v024a = 48;
        v020a = 0;
    } else {
        v028a = 128;
        v040m = 128;
        v024a = 128;
    }
    v012j = &this->slots[index];
    this->spriteGroup->drawScaled(1, v012j->posX + v012j->offsetX, v012j->posY + 1, v012j->size, v012j->size, v012j->texU[(unsigned char)state], v012j->texV[(unsigned char)state], v012j->size << 1, v012j->size << 1, v028a, v040m, v024a, v004b);
    this->spriteGroup->drawScaled(0, v012j->posX, v012j->posY, v012j->width, v012j->height, v012j->texU[0], v012j->texV[0], v012j->width << 1, v012j->height << 1, 128, 128, 128, v004b);
    if (v020a != 0) {
        this->spriteGroup->drawScaled(0, v012j->drawX, v012j->drawY, v020a, 4, 0, 118, 2, 10, v036i, v008a, v032i, v004b);
    }
}

/*
 * Splits a counter into decimal digits and draws the two halves of its digit atlas.
 * Extracts five decimal-place counts by repeated subtraction from amount plus one, suppresses
 * leading zeroes unless showZero equals one and always shows the final zero for amount zero. Draws
 * scaled16x16 quads from two digit rows using count below five or count-minus-five.
 */
// FUNCTION: FF7 0x00659073
void HighwayRenderOwner::drawNumber(int amount, int x, int y, short showZero, float depth)
{
    int digitPosition;
    int placeValues[5] = {0x2710, 0x3E8, 0x64, 0xA, 1};
    int remainder;
    char leading = 1;
    HighwayScaledSpriteGroup *groupLow;
    int digit8;
    HighwayScaledSpriteGroup *pSpritesHigh;

    remainder = amount + 1;
    for (digitPosition = 0; digitPosition < 5; digitPosition++) {
        digit8 = 0;
        while (remainder > placeValues[digitPosition]) {
            digit8++;
            remainder -= placeValues[digitPosition];
        }
        if (digit8 != 0) {
            leading = 0;
        }
        if (amount == 0 && digitPosition == 4) {
            leading = 0;
        }
        if (showZero == 1 || digit8 != 0 || !leading) {
            if (digit8 < 5) {
                groupLow = this->spriteGroup;
                groupLow->drawQuad(4, (float)(x + digitPosition * 10) * groupLow->globalScale,
                            (float)y * groupLow->globalScale, groupLow->globalScale * 16.0f,
                            groupLow->globalScale * 16.0f, (float)(digit8 << 5), 0.0f, 32.0f, 32.0f, -1,
                            depth);
            } else {
                pSpritesHigh = this->spriteGroup;
                pSpritesHigh->drawQuad(4, (float)(x + digitPosition * 10) * pSpritesHigh->globalScale,
                            (float)y * pSpritesHigh->globalScale, pSpritesHigh->globalScale * 16.0f,
                            pSpritesHigh->globalScale * 16.0f, (float)((digit8 - 5) << 5), 32.0f, 32.0f, 32.0f,
                            -1, depth);
            }
        }
    }
}

/*
 * Splits a five-digit counter and draws its visible digits through a sprite group.
 * Extracts five decimal-place counts by repeated subtraction from amount plus one and draws
 * scaled8x16 quads for requested or nonleading digits, always showing final zero for amount zero.
 * Uses each count times 16 as texture x without range validation.
 */
// FUNCTION: FF7 0x00659242
void HighwayRenderOwner::drawDigits(int amount, int x, int y, short showZero, float depth)
{
    int digitPosition;
    int placeValues[5] = {0x2710, 0x3E8, 0x64, 0xA, 1};
    int remainder;
    char leading = 1;
    HighwayScaledSpriteGroup *spriteGroupPtr;
    int digit8;

    remainder = amount + 1;
    for (digitPosition = 0; digitPosition < 5; digitPosition++) {
        digit8 = 0;
        while (remainder > placeValues[digitPosition]) {
            digit8++;
            remainder -= placeValues[digitPosition];
        }
        if (digit8 != 0) {
            leading = 0;
        }
        if (amount == 0 && digitPosition == 4) {
            leading = 0;
        }
        if (showZero == 1 || digit8 != 0 || !leading) {
            spriteGroupPtr = this->spriteGroup;
            spriteGroupPtr->drawQuad(5, (float)(x + digitPosition * 8) * spriteGroupPtr->globalScale, (float)y * spriteGroupPtr->globalScale,
                        spriteGroupPtr->globalScale * 8.0f, spriteGroupPtr->globalScale * 16.0f, (float)(digit8 * 16), 0.0f,
                        16.0f, 32.0f, -1, depth);
        }
    }
}

/*
 * Draws the score panel, counter and state-specific indicator.
 * Draws two scaled sprite panels and the two global numeric values using the context depth factor.
 * For leading state one, three, two draws centered sprite zero, one, two respectively, with sprite
 * one using texture y56. Pointer validity and numeric bounds are delegated.
 */
// FUNCTION: FF7 0x00659379
void HighwayRenderOwner::drawHud()
{
    float val;
    HighwayScaledSpriteGroup *ptr;
    HighwayScaledSpriteGroup *blk;

    val = 1.0f * g_pHighwaySceneBlock->scale;
    ptr = this->spriteGroup;
    ptr->drawQuad(3, ptr->globalScale * 18.0f, ptr->globalScale * 20.0f,
                  ptr->globalScale * 108.0f, ptr->globalScale * 20.0f, 0.0f, 64.0f,
                  216.0f, 40.0f, -1, 2.0f * val);
    drawDigits(g_HighwayHighScore, 85, 24, 1, val);
    blk = this->spriteGroup;
    blk->drawQuad(3, blk->globalScale * 200.0f, blk->globalScale * 20.0f,
                  blk->globalScale * 109.0f, blk->globalScale * 27.0f, 0.0f, 0.0f,
                  218.0f, 54.0f, -1, 2.0f * val);
    drawNumber(g_HighwayCurrentScore, 253, 26, 1, val);
    if (this->banner == 1) {
        (this->spriteGroup)->drawCentered(0, val, 0.0f, 0.0f);
    }
    if (this->banner == 3) {
        (this->spriteGroup)->drawCentered(1, val, 0.0f, 56.0f);
    }
    if (this->banner == 2) {
        (this->spriteGroup)->drawCentered(2, val, 0.0f, 0.0f);
    }
}

/*
 * When g_HighwayGameMode equals one, calls drawHud; otherwise calls tick.
 */
// FUNCTION: FF7 0x0065950c
void HighwayRenderOwner::dispatch()
{
    if (g_HighwayGameMode == 1) {
        drawHud();
    } else {
        tick();
    }
}

/*
 * When fadeActive is nonzero adds five to fadeAlpha when it is at most 250, builds a black packed
 * color with that alpha and submits a 640x480 rectangle. Selects scene pass one, draws the render
 * object and invalidates its batch generation. Does nothing when fadeActive is zero.
 */
// FUNCTION: FF7 0x00659532
void HighwayRenderOwner::drawFade()
{
    unsigned char colour[4];

    if (this->fadeActive != 0) {
        if (this->fadeAlpha <= 0xFA) {
            this->fadeAlpha += 5;
        }
        colour[0] = 0;
        colour[1] = 0;
        colour[2] = 0;
        colour[3] = this->fadeAlpha;
        Gfx_EmitFlatRect(0, 0, 640, 480, *(unsigned int *)colour, 0, 0,
                         (int)this->renderObject);
        Gfx_SetScenePassCall(1, (struct GfxEngineContext *)GetEngineContext());
        Gfx_DrawRenderObjectCall(this->renderObject, GetEngineContext());
        InvalidateBatchGeneration((void *)this->renderObject);
    }
}
