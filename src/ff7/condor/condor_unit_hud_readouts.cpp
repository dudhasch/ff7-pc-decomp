// src/ff7/condor/condor_unit_hud_readouts.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Draws the Fort Condor HUD sprites and numeric readouts for the selected unit, with
//   damage counter effects, display mode transitions, fade ramps and a tiled fade overlay.
/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/types/condor.h"
#include "lib/codegen.h"
#include "ff7/api/condor.h"
#include "ff7/api/field.h"
#include "lib/api/graphics.h"
#include "ff7/enums/condor.h"
#include <stdlib.h>
#include <string.h>
#include "lib/types/graphics.h"
#include "ff7/types/field.h"
extern "C" {

// clang-format off
// clang-format on

#pragma intrinsic(strcpy)
#pragma intrinsic(strcat)
#pragma intrinsic(strlen)

__declspec(dllimport) int __stdcall SetRect(void *, int, int, int, int);
#pragma intrinsic(memset)

#pragma intrinsic(abs)

// clang-format off
// GLOBAL: FF7 0x00905560
int g_CondorFadeTileOffsets[8][2] = {{0x0, 0x0}, {0x100, 0x0}, {0x0, 0x100}, {0x100, 0x100}, {0x0, 0x200}, {0x100, 0x200}, {0x0, 0x300}, {0x100, 0x300}};
// clang-format on

void __cdecl Mini_CondorDrawSpriteSlot(int spriteIndex, int xPos, int yPos);

/*
 * Empty callback stub.
 */
// FUNCTION: FF7 0x00607880
void __cdecl Mini_CondorNoOp_3(void)
{
}

/*
 * Updates damage counter and floating text effects.
 */
// FUNCTION: FF7 0x00607885
void __cdecl Mini_CondorStepCounterEffects(void)
{
    int effectIndex;
    short tick;

    for (effectIndex = 0; effectIndex < 0x10; effectIndex++) {
        if (g_CondorCounterEffects[effectIndex].counter != 0) {
            (g_CondorCounterEffects[effectIndex].counter)++;
            tick = g_CondorCounterEffects[effectIndex].counter & 0x7FFF;
            if ((tick & 0x7FFF) == 2) {
                if ((g_CondorCounterEffects[effectIndex].counter & 0x8000) == 0) {
                    Mini_CondorSetSpriteSlotPalette(effectIndex + 0x3B, 0);
                } else {
                    Mini_CondorSetSpriteSlotPalette(effectIndex + 0x3B, 4);
                }
            }
            if (tick >= 0x24) {
                g_CondorCounterEffects[effectIndex].counter = 0;
            } else {
                g_CondorSpriteEntries[effectIndex + 0x3B].posX = g_CondorCounterEffects[effectIndex].posX;
                g_CondorSpriteEntries[effectIndex + 0x3B].posY = g_CondorCounterEffects[effectIndex].posY - (tick >> 1);
                if (g_CondorCounterEffects[effectIndex].tensDigit != 0) {
                    g_CondorSpriteEntries[g_CondorCounterEffects[effectIndex].tensDigit + 0x3B].posX = g_CondorCounterEffects[effectIndex].posX;
                    g_CondorSpriteEntries[g_CondorCounterEffects[effectIndex].tensDigit + 0x3B].posY = g_CondorCounterEffects[effectIndex].posY - (tick >> 1);
                    if (g_CondorSpriteEntries[effectIndex + 0x3B].paletteIndex == 0) {
                        Mini_CondorDrawSpriteSlot(g_CondorCounterEffects[effectIndex].tensDigit + 0x3B, 0xFFFF, 0);
                    } else {
                        Mini_CondorDrawSpriteSlotGreen(g_CondorCounterEffects[effectIndex].tensDigit + 0x3B, 0xFFFF, 0);
                    }
                }
                g_CondorSpriteEntries[g_CondorCounterEffects[effectIndex].onesDigit + 0x3B].posX = g_CondorCounterEffects[effectIndex].posX + 8;
                g_CondorSpriteEntries[g_CondorCounterEffects[effectIndex].onesDigit + 0x3B].posY = g_CondorCounterEffects[effectIndex].posY - (tick >> 1);
                if (g_CondorSpriteEntries[effectIndex + 0x3B].paletteIndex == 0) {
                    Mini_CondorDrawSpriteSlot(g_CondorCounterEffects[effectIndex].onesDigit + 0x3B, 0xFFFF, 0);
                } else {
                    Mini_CondorDrawSpriteSlotGreen(g_CondorCounterEffects[effectIndex].onesDigit + 0x3B, 0xFFFF, 0);
                }
            }
        }
    }
}

/*
 * Finds an inactive counter effect slot.
 */
// FUNCTION: FF7 0x00607b05
short __cdecl Mini_CondorFindInactiveCounterEffectSlot(void)
{
    int i;

    for (i = 0; i < 0x10; i++) {
        if (g_CondorCounterEffects[i].counter == 0) {
            return (short)i;
        }
    }
    return 0xFF;
}

/*
 * Encodes two-digit number into record sprite slots.
 */
// FUNCTION: FF7 0x00607b42
void __cdecl Mini_CondorSetRecordTwoDigits(int slot, int number)
{
    g_CondorCounterEffects[slot].tensDigit = number % 0x64 / 0xA;
    g_CondorCounterEffects[slot].onesDigit = number % 0xA;
    g_CondorPendingCommandCount++;
}

/*
 * Calls Mini_CondorInitSpriteSlot for sprite `slot`, with the palette and page indices swapped.
 */
// FUNCTION: FF7 0x00607b91
void __cdecl Mini_CondorInitSprite(int slot, int sizeX, int sizeY, int uvX, int uvY, int paletteIndex, int pageIndex, void *offsets, int renderSlot, int depth)
{
    /* Retail caller prototype (short, short, short, short, short, short, short, int, int, int) -> void preserved for /Od argument promotion on `Mini_CondorInitSpriteSlot`. */
    ((void(__cdecl *)(short, short, short, short, short, short, short, int, int, int))Mini_CondorInitSpriteSlot)((short)slot, (short)sizeX, (short)sizeY, (short)uvX, (short)uvY, (short)pageIndex, (short)paletteIndex, (int)offsets, renderSlot, depth);
}

/*
 * Initializes sprite rendering slot with position and UV parameters.
 */
// FUNCTION: FF7 0x00607bcd
void __cdecl Mini_CondorInitSpriteSlot(int spriteIndex, short sizeX, short sizeY, short uvX, short uvY, int pageIndex, int paletteIndex, struct CondorSpriteOffset *offsets, int renderSlot, int depth)
{
    CondorSpriteEntry *sprite;
    unsigned short pageSlot;

    if ((pageIndex & 0xFFFF) == 0) {
        pageSlot = 0;
    } else {
        pageSlot = (pageIndex & 0xFFFF) - 1;
    }
    sprite = &g_CondorSpriteEntries[spriteIndex & 0xFFFF];
    sprite->width = sizeX;
    sprite->height = sizeY;
    sprite->posX = 0;
    sprite->posY = 0;
    sprite->depth = depth;
    sprite->unusedBaseField = g_CondorNumberSpriteBaseField;
    sprite->offsets = offsets;
    sprite->renderSlot = renderSlot;
    sprite->paletteIndex = paletteIndex & 0xFFFF;
    if ((pageIndex & 0xFFFF) != 0) {
        sprite->isActive = 1;
        sprite->renderSlot += 2;
    } else {
        sprite->isActive = 0;
    }
    sprite->pageSlot = pageSlot;
    sprite->texU = uvX;
    sprite->texV = uvY;
}

/*
 * Renders sprite slot with specified transform and color.
 */
// FUNCTION: FF7 0x00607cc5
void __cdecl Mini_CondorDrawSpriteSlot(int spriteIndex, int xPos, int yPos)
{
    int excessX;
    int doubledTextureU;
    int rawDoubledWidth;
    CondorSpriteOffset *offsetData;
    int scaledHeight;
    int uEnd;
    int vEnd;
    int res;
    int doubledTexV;
    short viewX;
    int overflowY;
    short screenY;
    CondorSpriteEntry *slotPointer;

    slotPointer = &g_CondorSpriteEntries[spriteIndex & 0xFFFF];
    if ((xPos & 0xFFFF) != 0xFFFF) {
        slotPointer->posX = xPos;
        slotPointer->posY = yPos;
    }
    if (slotPointer->offsets != 0) {
        offsetData = slotPointer->offsets;
        slotPointer->posX += offsetData->offsetX;
        slotPointer->posY += offsetData->offsetY;
    }
    viewX = slotPointer->uPosX - g_CondorCameraX;
    screenY = slotPointer->uPosY - g_CondorCameraY;
    if (g_CondorPresetMode == 2) {
        if (slotPointer->renderSlot < 1 || slotPointer->renderSlot > 6) {
            res = Gfx_EmitTexturedRectFlipped(viewX << 1, screenY * 2 + 0x14, slotPointer->width << 1, slotPointer->height << 1, slotPointer->texU, slotPointer->texV, slotPointer->width, slotPointer->height, 0x80FFFFFF, slotPointer->paletteIndex, slotPointer->depth, FLOAT_ONE_BITS, ((struct CondorRenderObjectSlotEntry *)&g_CondorRenderObjectSlots)[slotPointer->renderSlot].renderObject);
            ((struct CondorRenderObjectSlotEntry *)&g_CondorRenderObjectSlots)[slotPointer->renderSlot].drawn = 1;
        } else {
            doubledTextureU = slotPointer->texU << 1;
            rawDoubledWidth = slotPointer->width << 1;
            uEnd = rawDoubledWidth + slotPointer->texU * 2;
            doubledTexV = slotPointer->texV << 1;
            scaledHeight = slotPointer->height << 1;
            vEnd = scaledHeight + slotPointer->texV * 2;
            if (uEnd <= 0x100 && vEnd <= 0x100) {
                res = Gfx_EmitTexturedRectFlipped(viewX << 1, screenY * 2 + 0x14, slotPointer->width << 1, slotPointer->height << 1, doubledTextureU, doubledTexV, rawDoubledWidth, scaledHeight, 0x80FFFFFF, slotPointer->paletteIndex, slotPointer->depth, FLOAT_ONE_BITS, g_CondorUnitModelTable[slotPointer->renderSlot - 1].parts[0].renderObject);
                g_CondorUnitModelTable[slotPointer->renderSlot - 1].parts[0].batched = 1;
                return;
            }
            if (doubledTextureU >= 0x100 && doubledTexV >= 0x100) {
                res = Gfx_EmitTexturedRectFlipped(viewX << 1, screenY * 2 + 0x14, slotPointer->width << 1, slotPointer->height << 1, doubledTextureU - 0x100, doubledTexV - 0x100, rawDoubledWidth, scaledHeight, 0x80FFFFFF, slotPointer->paletteIndex, slotPointer->depth, FLOAT_ONE_BITS, g_CondorUnitModelTable[slotPointer->renderSlot - 1].parts[3].renderObject);
                g_CondorUnitModelTable[slotPointer->renderSlot - 1].parts[3].batched = 1;
                return;
            }
            if (doubledTextureU >= 0x100 && vEnd <= 0x100) {
                res = Gfx_EmitTexturedRectFlipped(viewX << 1, screenY * 2 + 0x14, slotPointer->width << 1, slotPointer->height << 1, doubledTextureU - 0x100, doubledTexV, rawDoubledWidth, scaledHeight, 0x80FFFFFF, slotPointer->paletteIndex, slotPointer->depth, FLOAT_ONE_BITS, g_CondorUnitModelTable[slotPointer->renderSlot - 1].parts[1].renderObject);
                g_CondorUnitModelTable[slotPointer->renderSlot - 1].parts[1].batched = 1;
                return;
            }
            if (uEnd <= 0x100 && doubledTexV >= 0x100) {
                res = Gfx_EmitTexturedRectFlipped(viewX << 1, screenY * 2 + 0x14, slotPointer->width << 1, slotPointer->height << 1, doubledTextureU, doubledTexV - 0x100, rawDoubledWidth, scaledHeight, 0x80FFFFFF, slotPointer->paletteIndex, slotPointer->depth, FLOAT_ONE_BITS, g_CondorUnitModelTable[slotPointer->renderSlot - 1].parts[2].renderObject);
                g_CondorUnitModelTable[slotPointer->renderSlot - 1].parts[2].batched = 1;
                return;
            }
            if (vEnd <= 0x100) {
                excessX = uEnd - 0x100;
                res = Gfx_EmitTexturedRectFlipped(viewX << 1, screenY * 2 + 0x14, (slotPointer->width << 1) - excessX, slotPointer->height << 1, doubledTextureU, doubledTexV, rawDoubledWidth - excessX, scaledHeight, 0x80FFFFFF, slotPointer->paletteIndex, slotPointer->depth, FLOAT_ONE_BITS, g_CondorUnitModelTable[slotPointer->renderSlot - 1].parts[0].renderObject);
                g_CondorUnitModelTable[slotPointer->renderSlot - 1].parts[0].batched = 1;
                res = Gfx_EmitTexturedRectFlipped(viewX * 2 + (slotPointer->width << 1) - excessX, screenY * 2 + 0x14, excessX, slotPointer->height << 1, 0, doubledTexV, excessX, scaledHeight, 0x80FFFFFF, slotPointer->paletteIndex, slotPointer->depth, FLOAT_ONE_BITS, g_CondorUnitModelTable[slotPointer->renderSlot - 1].parts[1].renderObject);
                g_CondorUnitModelTable[slotPointer->renderSlot - 1].parts[1].batched = 1;
                return;
            }
            if (doubledTexV >= 0x100) {
                excessX = uEnd - 0x100;
                res = Gfx_EmitTexturedRectFlipped(viewX << 1, screenY * 2 + 0x14, (slotPointer->width << 1) - excessX, slotPointer->height << 1, doubledTextureU, doubledTexV - 0x100, rawDoubledWidth - excessX, scaledHeight, 0x80FFFFFF, slotPointer->paletteIndex, slotPointer->depth, FLOAT_ONE_BITS, g_CondorUnitModelTable[slotPointer->renderSlot - 1].parts[2].renderObject);
                g_CondorUnitModelTable[slotPointer->renderSlot - 1].parts[2].batched = 1;
                res = Gfx_EmitTexturedRectFlipped(viewX * 2 + (slotPointer->width << 1) - excessX, screenY * 2 + 0x14, excessX, slotPointer->height << 1, 0, doubledTexV - 0x100, excessX, scaledHeight, 0x80FFFFFF, slotPointer->paletteIndex, slotPointer->depth, FLOAT_ONE_BITS, g_CondorUnitModelTable[slotPointer->renderSlot - 1].parts[3].renderObject);
                g_CondorUnitModelTable[slotPointer->renderSlot - 1].parts[3].batched = 1;
                return;
            }
            if (uEnd <= 0x100) {
                overflowY = vEnd - 0x100;
                res = Gfx_EmitTexturedRectFlipped(viewX << 1, screenY * 2 + 0x14, slotPointer->width << 1, (slotPointer->height << 1) - overflowY, doubledTextureU, doubledTexV, rawDoubledWidth, scaledHeight - overflowY, 0x80FFFFFF, slotPointer->paletteIndex, slotPointer->depth, FLOAT_ONE_BITS, g_CondorUnitModelTable[slotPointer->renderSlot - 1].parts[0].renderObject);
                g_CondorUnitModelTable[slotPointer->renderSlot - 1].parts[0].batched = 1;
                res = Gfx_EmitTexturedRectFlipped(viewX << 1, screenY * 2 + (slotPointer->height << 1) + 0x14 - overflowY, slotPointer->width << 1, overflowY, doubledTextureU, 0, rawDoubledWidth, overflowY, 0x80FFFFFF, slotPointer->paletteIndex, slotPointer->depth, FLOAT_ONE_BITS, g_CondorUnitModelTable[slotPointer->renderSlot - 1].parts[2].renderObject);
                g_CondorUnitModelTable[slotPointer->renderSlot - 1].parts[2].batched = 1;
                return;
            }
            if (doubledTextureU >= 0x100) {
                overflowY = vEnd - 0x100;
                res = Gfx_EmitTexturedRectFlipped(viewX << 1, screenY * 2 + 0x14, slotPointer->width << 1, (slotPointer->height << 1) - overflowY, doubledTextureU - 0x100, doubledTexV, rawDoubledWidth, scaledHeight - overflowY, 0x80FFFFFF, slotPointer->paletteIndex, slotPointer->depth, FLOAT_ONE_BITS, g_CondorUnitModelTable[slotPointer->renderSlot - 1].parts[1].renderObject);
                g_CondorUnitModelTable[slotPointer->renderSlot - 1].parts[1].batched = 1;
                res = Gfx_EmitTexturedRectFlipped(viewX << 1, screenY * 2 + (slotPointer->height << 1) + 0x14 - overflowY, slotPointer->width << 1, overflowY, doubledTextureU - 0x100, 0, rawDoubledWidth, overflowY, 0x80FFFFFF, slotPointer->paletteIndex, slotPointer->depth, FLOAT_ONE_BITS, g_CondorUnitModelTable[slotPointer->renderSlot - 1].parts[3].renderObject);
                g_CondorUnitModelTable[slotPointer->renderSlot - 1].parts[3].batched = 1;
                return;
            }
        }
    } else {
        res = Gfx_EmitTexturedRectFlipped(viewX + g_CondorViewportOffsetX, screenY + g_CondorViewportOffsetY + 0xA, slotPointer->width, slotPointer->height, slotPointer->texU, slotPointer->texV, slotPointer->width, slotPointer->height, 0x80FFFFFF, slotPointer->paletteIndex, slotPointer->depth, FLOAT_ONE_BITS, ((struct CondorRenderObjectSlotEntry *)&g_CondorRenderObjectSlots)[slotPointer->renderSlot].renderObject);
        ((struct CondorRenderObjectSlotEntry *)&g_CondorRenderObjectSlots)[slotPointer->renderSlot].drawn = 1;
    }
}

/*
 * Renders sprite slot with green modulated tinting.
 */
// FUNCTION: FF7 0x006086ab
void __cdecl Mini_CondorDrawSpriteSlotGreen(int slotIndex, int posX, int posY)
{
    int wrapU;
    CondorSpriteEntry *slotPtr;
    int textureBottom;
    CondorBatchPrimitive *rectangle;
    int textureTop;
    unsigned short index;
    int textureRight;
    int texH;
    int texRemaining;
    short screenX;
    int excessV;
    short screenY;
    CondorSpriteOffset *offsetPtr;
    int srcW;

    slotPtr = &g_CondorSpriteEntries[slotIndex & 0xFFFF];
    if ((posX & 0xFFFF) != 0xFFFF) {
        slotPtr->posX = posX;
        slotPtr->posY = posY;
    }
    if (slotPtr->offsets != 0) {
        offsetPtr = slotPtr->offsets;
        slotPtr->posX += offsetPtr->offsetX;
        slotPtr->posY += offsetPtr->offsetY;
    }
    screenX = slotPtr->uPosX - g_CondorCameraX;
    screenY = slotPtr->uPosY - g_CondorCameraY;
    if (g_CondorPresetMode == 2) {
        if (slotPtr->renderSlot < 1 || slotPtr->renderSlot > 6) {
            rectangle = (CondorBatchPrimitive *)Gfx_EmitTexturedRect(g_CondorViewportOffsetX + screenX * g_CondorViewportScale, g_CondorViewportOffsetY + g_CondorViewportInset + screenY * g_CondorViewportScale, slotPtr->width * g_CondorViewportScale, slotPtr->height * g_CondorViewportScale, slotPtr->texU, slotPtr->texV, slotPtr->width, slotPtr->height, 0, slotPtr->depth, FLOAT_ONE_BITS, ((struct CondorRenderObjectSlotEntry *)&g_CondorRenderObjectSlots)[slotPtr->renderSlot].renderObject);
            if (rectangle == 0) {
                return;
            }
            for (index = 0; index < 4; index++) {
                rectangle->colorBytes[index].red = 0;
                rectangle->colorBytes[index].green = 0xFF;
                rectangle->colorBytes[index].blue = 0;
                rectangle->colorBytes[index].alpha = 0xFF;
            }
            ((struct CondorRenderObjectSlotEntry *)&g_CondorRenderObjectSlots)[slotPtr->renderSlot].drawn = 1;
        } else {
            texRemaining = slotPtr->texU << 1;
            srcW = slotPtr->width << 1;
            textureRight = srcW + slotPtr->texU * 2;
            textureTop = slotPtr->texV << 1;
            texH = slotPtr->height << 1;
            textureBottom = texH + slotPtr->texV * 2;
            if (textureRight <= 0x100 && textureBottom <= 0x100) {
                Gfx_EmitTexturedRectFlipped(screenX << 1, screenY * 2 + 0x14, slotPtr->width << 1, slotPtr->height << 1, texRemaining, textureTop, srcW, texH, 0xFF00FF00, slotPtr->paletteIndex, slotPtr->depth, FLOAT_ONE_BITS, g_CondorUnitModelTable[slotPtr->renderSlot - 1].parts[0].renderObject);
                g_CondorUnitModelTable[slotPtr->renderSlot - 1].parts[0].batched = 1;
                return;
            }
            if (texRemaining >= 0x100 && textureTop >= 0x100) {
                rectangle = (CondorBatchPrimitive *)Gfx_EmitTexturedRectFlipped(screenX << 1, screenY * 2 + 0x14, slotPtr->width << 1, slotPtr->height << 1, texRemaining - 0x100, textureTop - 0x100, srcW, texH, 0xFF00FF00, slotPtr->paletteIndex, slotPtr->depth, FLOAT_ONE_BITS, g_CondorUnitModelTable[slotPtr->renderSlot - 1].parts[3].renderObject);
                g_CondorUnitModelTable[slotPtr->renderSlot - 1].parts[3].batched = 1;
                return;
            }
            if (texRemaining >= 0x100 && textureBottom <= 0x100) {
                rectangle = (CondorBatchPrimitive *)Gfx_EmitTexturedRectFlipped(screenX << 1, screenY * 2 + 0x14, slotPtr->width << 1, slotPtr->height << 1, texRemaining - 0x100, textureTop, srcW, texH, 0xFF00FF00, slotPtr->paletteIndex, slotPtr->depth, FLOAT_ONE_BITS, g_CondorUnitModelTable[slotPtr->renderSlot - 1].parts[1].renderObject);
                g_CondorUnitModelTable[slotPtr->renderSlot - 1].parts[1].batched = 1;
                return;
            }
            if (textureRight <= 0x100 && textureTop >= 0x100) {
                rectangle = (CondorBatchPrimitive *)Gfx_EmitTexturedRectFlipped(screenX << 1, screenY * 2 + 0x14, slotPtr->width << 1, slotPtr->height << 1, texRemaining, textureTop - 0x100, srcW, texH, 0xFF00FF00, slotPtr->paletteIndex, slotPtr->depth, FLOAT_ONE_BITS, g_CondorUnitModelTable[slotPtr->renderSlot - 1].parts[2].renderObject);
                g_CondorUnitModelTable[slotPtr->renderSlot - 1].parts[2].batched = 1;
                return;
            }
            if (textureBottom <= 0x100) {
                wrapU = textureRight - 0x100;
                rectangle = (CondorBatchPrimitive *)Gfx_EmitTexturedRectFlipped(screenX << 1, screenY * 2 + 0x14, (slotPtr->width << 1) - wrapU, slotPtr->height << 1, texRemaining, textureTop, srcW - wrapU, texH, 0xFF00FF00, slotPtr->paletteIndex, slotPtr->depth, FLOAT_ONE_BITS, g_CondorUnitModelTable[slotPtr->renderSlot - 1].parts[0].renderObject);
                g_CondorUnitModelTable[slotPtr->renderSlot - 1].parts[0].batched = 1;
                rectangle = (CondorBatchPrimitive *)Gfx_EmitTexturedRectFlipped(screenX * 2 + (slotPtr->width << 1) - wrapU, screenY * 2 + 0x14, wrapU, slotPtr->height << 1, 0, textureTop, wrapU, texH, 0xFF00FF00, slotPtr->paletteIndex, slotPtr->depth, FLOAT_ONE_BITS, g_CondorUnitModelTable[slotPtr->renderSlot - 1].parts[1].renderObject);
                g_CondorUnitModelTable[slotPtr->renderSlot - 1].parts[1].batched = 1;
                return;
            }
            if (textureTop >= 0x100) {
                wrapU = textureRight - 0x100;
                rectangle = (CondorBatchPrimitive *)Gfx_EmitTexturedRectFlipped(screenX << 1, screenY * 2 + 0x14, (slotPtr->width << 1) - wrapU, slotPtr->height << 1, texRemaining, textureTop - 0x100, srcW - wrapU, texH, 0xFF00FF00, slotPtr->paletteIndex, slotPtr->depth, FLOAT_ONE_BITS, g_CondorUnitModelTable[slotPtr->renderSlot - 1].parts[2].renderObject);
                g_CondorUnitModelTable[slotPtr->renderSlot - 1].parts[2].batched = 1;
                rectangle = (CondorBatchPrimitive *)Gfx_EmitTexturedRectFlipped(screenX * 2 + (slotPtr->width << 1) - wrapU, screenY * 2 + 0x14, wrapU, slotPtr->height << 1, 0, textureTop - 0x100, wrapU, texH, 0xFF00FF00, slotPtr->paletteIndex, slotPtr->depth, FLOAT_ONE_BITS, g_CondorUnitModelTable[slotPtr->renderSlot - 1].parts[3].renderObject);
                g_CondorUnitModelTable[slotPtr->renderSlot - 1].parts[3].batched = 1;
                return;
            }
            if (textureRight <= 0x100) {
                excessV = textureBottom - 0x100;
                rectangle = (CondorBatchPrimitive *)Gfx_EmitTexturedRectFlipped(screenX << 1, screenY * 2 + 0x14, slotPtr->width << 1, (slotPtr->height << 1) - excessV, texRemaining, textureTop, srcW, texH - excessV, 0xFF00FF00, slotPtr->paletteIndex, slotPtr->depth, FLOAT_ONE_BITS, g_CondorUnitModelTable[slotPtr->renderSlot - 1].parts[0].renderObject);
                g_CondorUnitModelTable[slotPtr->renderSlot - 1].parts[0].batched = 1;
                rectangle = (CondorBatchPrimitive *)Gfx_EmitTexturedRectFlipped(screenX << 1, screenY * 2 + (slotPtr->height << 1) + 0x14 - excessV, slotPtr->width << 1, excessV, texRemaining, 0, srcW, excessV, 0xFF00FF00, slotPtr->paletteIndex, slotPtr->depth, FLOAT_ONE_BITS, g_CondorUnitModelTable[slotPtr->renderSlot - 1].parts[2].renderObject);
                g_CondorUnitModelTable[slotPtr->renderSlot - 1].parts[2].batched = 1;
                return;
            }
            if (texRemaining >= 0x100) {
                excessV = textureBottom - 0x100;
                rectangle = (CondorBatchPrimitive *)Gfx_EmitTexturedRectFlipped(screenX << 1, screenY * 2 + 0x14, slotPtr->width << 1, (slotPtr->height << 1) - excessV, texRemaining - 0x100, textureTop, srcW, texH - excessV, 0xFF00FF00, slotPtr->paletteIndex, slotPtr->depth, FLOAT_ONE_BITS, g_CondorUnitModelTable[slotPtr->renderSlot - 1].parts[1].renderObject);
                g_CondorUnitModelTable[slotPtr->renderSlot - 1].parts[1].batched = 1;
                rectangle = (CondorBatchPrimitive *)Gfx_EmitTexturedRectFlipped(screenX << 1, screenY * 2 + (slotPtr->height << 1) + 0x14 - excessV, slotPtr->width << 1, excessV, texRemaining - 0x100, 0, srcW, excessV, 0xFF00FF00, slotPtr->paletteIndex, slotPtr->depth, FLOAT_ONE_BITS, g_CondorUnitModelTable[slotPtr->renderSlot - 1].parts[3].renderObject);
                g_CondorUnitModelTable[slotPtr->renderSlot - 1].parts[3].batched = 1;
                return;
            }
        }
    } else {
        rectangle = (CondorBatchPrimitive *)Gfx_EmitTexturedRect(g_CondorViewportOffsetX + screenX * g_CondorViewportScale, g_CondorViewportOffsetY + g_CondorViewportInset + screenY * g_CondorViewportScale, slotPtr->width * g_CondorViewportScale, slotPtr->height * g_CondorViewportScale, slotPtr->texU, slotPtr->texV, slotPtr->width, slotPtr->height, 0, slotPtr->depth, FLOAT_ONE_BITS, ((struct CondorRenderObjectSlotEntry *)&g_CondorRenderObjectSlots)[slotPtr->renderSlot].renderObject);
        if (rectangle == 0) {
            return;
        }
        for (index = 0; index < 4; index++) {
            rectangle->colorBytes[index].red = 0;
            rectangle->colorBytes[index].green = 0xFF;
            rectangle->colorBytes[index].blue = 0;
            rectangle->colorBytes[index].alpha = 0xFF;
        }
        ((struct CondorRenderObjectSlotEntry *)&g_CondorRenderObjectSlots)[slotPtr->renderSlot].drawn = 1;
    }
}

/*
 * Sets width and height dimensions for sprite slot.
 */
// FUNCTION: FF7 0x006091a1
void __cdecl Mini_CondorSetSpriteSlotSize(int slotIndex, int width, int height)
{
    CondorSpriteEntry *slotPtr;

    slotPtr = &g_CondorSpriteEntries[slotIndex & 0xFFFF];
    slotPtr->texU = width;
    slotPtr->texV = height;
}

/*
 * Sets the sprite slot's paletteIndex to the low 16 bits of `value`.
 */
// FUNCTION: FF7 0x006091d2
void __cdecl Mini_CondorSetSpriteSlotPalette(int slotIndex, int value)
{
    CondorSpriteEntry *slotPtr;

    slotPtr = &g_CondorSpriteEntries[slotIndex & 0xFFFF];
    slotPtr->paletteIndex = value & 0xFFFF;
}

/*
 * Generates digit glyph display records.
 */
// FUNCTION: FF7 0x006091fc
void __cdecl Mini_CondorBuildDigitDisplayRecords(void)
{
    CondorDigitPair rec;
    g_CondorDigitStarted = 0;
    g_CondorReadoutValue = g_CondorPlayerUnitCount;
    rec.tens = Mini_CondorExtractDecimalDigit(10);
    rec.ones = Mini_CondorExtractDecimalDigit(1);
    g_CondorFirstNumberDigitCount = 2;
    g_CondorFirstNumberLowSlot = rec.tens;
    g_CondorFirstNumberHighSlot = rec.ones;
    g_CondorFirstNumberVisible = 1;
    g_CondorDigitStarted = 0;
    g_CondorReadoutValue = g_CondorSpawnCount;
    rec.tens = Mini_CondorExtractDecimalDigit(10);
    rec.ones = Mini_CondorExtractDecimalDigit(1);
    g_CondorSecondNumberDigitCount = 2;
    g_CondorSecondNumberLowSlot = rec.tens;
    g_CondorSecondNumberHighSlot = rec.ones;
    g_CondorSecondNumberVisible = 1;
}

/*
 * Sets numeric value readout for selected unit.
 */
// FUNCTION: FF7 0x006092c2
void __cdecl Mini_CondorSetSelectedReadout(short unitIndex, short recordIndex)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[12];
        union {
            int tensDigit;
            char tensByte;
        };
        struct {
            unsigned char pad_to_1[0x1];
            union {
                int onesDigit;
                char onesByte;
            };
        };
        struct { unsigned char pad_to_8[0x8]; int tmpZero; };
    } frame;
#pragma pack(pop)

    frame.tmpZero = 0;
    if (unitIndex == -1) {
        return;
    }
    if (recordIndex == -1) {
        frame.tensByte = 0xB;
        frame.onesByte = 0xB;
    } else {
        g_CondorReadoutValue = g_CondorRecordTable.records[recordIndex].uFrameIndex;
        if (g_CondorMainState == CONDOR_STATE_RECORD_SELECT_WHEEL) {
            g_CondorReadoutValue = g_CondorUnitFrameIndex[unitIndex];
        }
        g_CondorDigitStarted = 0;
        frame.tensByte = Mini_CondorExtractDecimalDigit(0xA);
        frame.onesByte = Mini_CondorExtractDecimalDigit(1);
        if ((frame.tensDigit & 0xFF) == 0xFF) {
            frame.tensByte = 0;
        }
    }
    g_CondorSelectedReadoutUnit = unitIndex;
    g_CondorSelectedUnitIndex = unitIndex;
    g_CondorTensDigitSlot = frame.tensDigit & 0xFF;
    g_CondorOnesDigitSlot = frame.onesDigit & 0xFF;
    g_CondorSelectedReadoutVisible = 1;
}

/*
 * Extracts single decimal digit from integer value.
 */
// FUNCTION: FF7 0x00609393
char __cdecl Mini_CondorExtractDecimalDigit(unsigned int divisor)
{
    char digit;
    if (g_CondorReadoutValue == 0) {
        if (g_CondorDigitStarted == 0) {
            if (divisor != 1)
                return -1;
            return 0;
        }
        return 0;
    }
    if ((unsigned int)g_CondorReadoutValue / divisor == 0) {
        if (g_CondorDigitStarted == 0)
            return -1;
        return 0;
    }
    g_CondorDigitStarted++;
    digit = (unsigned int)g_CondorReadoutValue / divisor;
    g_CondorReadoutValue -= (unsigned int)g_CondorReadoutValue / divisor * divisor;
    return digit;
}

/*
 * Configures display layout for selected unit record.
 */
// FUNCTION: FF7 0x00609420
void __cdecl Mini_CondorSelectRecordDisplay(short recordIndex)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[60];
        union {
            int glyphHundreds;
            char glyphHundredsByte;
        };
        struct {
            unsigned char pad_to_1[0x1];
            union {
                int glyphTens;
                char glyphTensByte;
            };
        };
        struct { unsigned char pad_to_2[0x2]; char glyphOnes; };
        struct { unsigned char pad_to_C[0xC]; char byteA; };
        struct { unsigned char pad_to_D[0xD]; char byteB; };
        struct { unsigned char pad_to_E[0xE]; char byteC; };
        struct { unsigned char pad_to_F[0xF]; char byteD; };
        struct { unsigned char pad_to_10[0x10]; char byteE; };
        struct { unsigned char pad_to_11[0x11]; char byteF; };
        struct { unsigned char pad_to_12[0x12]; char byteG; };
        struct { unsigned char pad_to_13[0x13]; char byteH; };
        struct { unsigned char pad_to_14[0x14]; char byteI; };
        struct { unsigned char pad_to_15[0x15]; char byteJ; };
        struct { unsigned char pad_to_16[0x16]; char byteK; };
        struct { unsigned char pad_to_17[0x17]; char byteL; };
        struct { unsigned char pad_to_18[0x18]; char byteM; };
        struct { unsigned char pad_to_19[0x19]; char byteN; };
        struct { unsigned char pad_to_1A[0x1A]; char byteO; };
        struct { unsigned char pad_to_1B[0x1B]; char byteP; };
        struct { unsigned char pad_to_1C[0x1C]; char byteQ; };
        struct { unsigned char pad_to_1D[0x1D]; char byteR; };
        struct { unsigned char pad_to_1E[0x1E]; char byteS; };
        struct { unsigned char pad_to_1F[0x1F]; char byteT; };
        struct { unsigned char pad_to_20[0x20]; char byteU; };
        struct { unsigned char pad_to_21[0x21]; char byteV; };
        struct { unsigned char pad_to_22[0x22]; char byteW; };
        struct { unsigned char pad_to_23[0x23]; char byteX; };
        struct { unsigned char pad_to_24[0x24]; char byteY; };
        struct { unsigned char pad_to_25[0x25]; char byteZ; };
        struct { unsigned char pad_to_26[0x26]; char byteAA; };
        struct { unsigned char pad_to_27[0x27]; char byteAB; };
        struct { unsigned char pad_to_2C[0x2C]; int digitIndex; };
        struct { unsigned char pad_to_34[0x34]; unsigned short *fileBase; };
        struct {
            unsigned char pad_to_38[0x38];
            union {
                int displayRecordAddr;
                CondorRecordDisplay *displayRecord;
            };
        };
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
#pragma pack(pop)

    frame.byteA = 0xA;
    frame.byteB = 6;
    frame.byteC = 0;
    frame.byteD = 0;
    frame.byteE = 0;
    frame.byteF = 2;
    frame.byteG = 0;
    frame.byteH = 4;
    frame.byteI = 0;
    frame.byteJ = 6;
    frame.byteK = 5;
    frame.byteL = 0;
    frame.byteM = 5;
    frame.byteN = 2;
    frame.byteO = 5;
    frame.byteP = 4;
    frame.byteQ = 5;
    frame.byteR = 6;
    frame.byteS = 0xA;
    frame.byteT = 6;
    frame.byteU = 0xA;
    frame.byteV = 6;
    frame.byteW = 0xA;
    frame.byteX = 6;
    frame.byteY = 0xA;
    frame.byteZ = 0;
    frame.byteAA = 0xA;
    frame.byteAB = 2;
    g_CondorRecordDisplaySlot = recordIndex;
    frame.fileBase = (unsigned short *)g_pCondorDataBin;
    frame.displayRecordAddr = g_pCondorDataBin + (*frame.fileBase & 0xFFFF);
    frame.displayRecordAddr += recordIndex << 5;
    g_CondorReadoutValue = frame.displayRecord->counter;
    g_CondorDigitStarted = 0;
    frame.glyphHundredsByte = Mini_CondorExtractDecimalDigit(0x64);
    frame.glyphTensByte = Mini_CondorExtractDecimalDigit(0xA);
    frame.glyphOnes = Mini_CondorExtractDecimalDigit(1);
    for (frame.digitIndex = 0; frame.digitIndex < 3; frame.digitIndex++) {
        g_CondorCounterDigitSprites[frame.digitIndex] = frame.storage[frame.digitIndex];
    }
    g_CondorCounterDigitCount = 3;
    g_CondorCounterDigitsVisible = 1;
    g_CondorCounterDisplayOffset = 0;
    g_CondorReadoutValue = frame.displayRecord->scalePercent;
    if (g_CondorReadoutValue == 0) {
        frame.glyphHundredsByte = 0;
        frame.glyphTensByte = 0xE;
        for (frame.digitIndex = 0; frame.digitIndex < 2; frame.digitIndex++) {
            g_CondorScaleDigitSprites[frame.digitIndex] = frame.storage[frame.digitIndex];
        }
        g_CondorScaleDigitCount = 2;
        g_CondorScaleDigitsVisible = 1;
        g_CondorRecordDisplayOffset = 0;
    } else {
        g_CondorDigitStarted = 0;
        frame.glyphHundredsByte = Mini_CondorExtractDecimalDigit(0x64);
        frame.glyphTensByte = Mini_CondorExtractDecimalDigit(0xA);
        frame.glyphOnes = Mini_CondorExtractDecimalDigit(1);
        for (frame.digitIndex = 0; frame.digitIndex < 3; frame.digitIndex++) {
            g_CondorScaleDigitSprites[frame.digitIndex] = frame.storage[frame.digitIndex];
        }
        g_CondorScaleDigitCount = 3;
        g_CondorScaleDigitsVisible = 1;
        g_CondorRecordDisplayOffset = 8;
        if ((frame.glyphTens & 0xFF) != 0xFF) {
            g_CondorRecordDisplayOffset = 4;
        }
        if ((frame.glyphHundreds & 0xFF) != 0xFF) {
            g_CondorRecordDisplayOffset = 0;
        }
    }
    g_CondorReadoutValue = frame.displayRecord->rangeCells;
    if ((unsigned int)g_CondorReadoutValue <= (unsigned int)1) {
        frame.glyphHundredsByte = 0;
        frame.glyphTensByte = 0;
        if (g_CondorReadoutValue == 0) {
            frame.glyphOnes = 0xE;
        } else {
            frame.glyphOnes = 1;
        }
        for (frame.digitIndex = 0; frame.digitIndex < 3; frame.digitIndex++) {
            g_CondorRangeDigitSprites[frame.digitIndex] = frame.storage[frame.digitIndex];
        }
        g_CondorRangeDigitCount = 3;
        g_CondorRangeDigitsVisible = 1;
    } else {
        frame.glyphHundredsByte = 1;
        frame.glyphTensByte = 0xD;
        frame.glyphOnes = (char)g_CondorReadoutValue;
        for (frame.digitIndex = 0; frame.digitIndex < 3; frame.digitIndex++) {
            g_CondorRangeDigitSprites[frame.digitIndex] = frame.storage[frame.digitIndex];
        }
    }
    g_CondorRangeDigitCount = 3;
    g_CondorRangeDigitsVisible = 1;
    Mini_CondorBuildRecordValueGlyphs(frame.displayRecord);
}

/*
 * Builds numeric glyph sprites for unit stats.
 */
// FUNCTION: FF7 0x00609748
void __cdecl Mini_CondorBuildRecordValueGlyphs(CondorRecordDisplay *recordPtr)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[16];
        char digitMillions;
        struct { unsigned char pad_to_1[0x1]; char digitHundredThousands; };
        struct { unsigned char pad_to_2[0x2]; char digitTenThousands; };
        struct { unsigned char pad_to_3[0x3]; char digitThousands; };
        struct { unsigned char pad_to_4[0x4]; char digitHundreds; };
        struct { unsigned char pad_to_5[0x5]; char digitTens; };
        struct { unsigned char pad_to_6[0x6]; char digitOnes; };
        struct { unsigned char pad_to_7[0x7]; char glyphA; };
        struct { unsigned char pad_to_8[0x8]; char glyphB; };
        struct { unsigned char pad_to_9[0x9]; char glyphC; };
        struct { unsigned char pad_to_C[0xC]; int glyphIndex; };
    } frame;
#pragma pack(pop)

    g_CondorReadoutValue = g_CondorFunds;
    g_CondorDigitStarted = 0;
    frame.digitMillions = Mini_CondorExtractDecimalDigit(0xF4240);
    frame.digitHundredThousands = Mini_CondorExtractDecimalDigit(0x186A0);
    frame.digitTenThousands = Mini_CondorExtractDecimalDigit(0x2710);
    frame.digitThousands = Mini_CondorExtractDecimalDigit(0x3E8);
    frame.digitHundreds = Mini_CondorExtractDecimalDigit(0x64);
    frame.digitTens = Mini_CondorExtractDecimalDigit(0xA);
    frame.digitOnes = Mini_CondorExtractDecimalDigit(1);
    frame.glyphA = 0x8B;
    frame.glyphB = 0x8C;
    frame.glyphC = 0x8D;
    for (frame.glyphIndex = 0; frame.glyphIndex < 0xA; frame.glyphIndex++) {
        g_CondorGilDigitSprites[frame.glyphIndex] = frame.storage[frame.glyphIndex];
    }
    g_CondorGilDigitCount = 0xA;
    g_CondorGilDigitsVisible = 1;
    g_CondorRecordCost = recordPtr->cost;
    g_CondorReadoutValue = recordPtr->cost;
    g_CondorDigitStarted = 0;
    frame.digitMillions = 0;
    frame.digitHundredThousands = 0;
    frame.digitTenThousands = 0;
    frame.digitThousands = Mini_CondorExtractDecimalDigit(0x3E8);
    frame.digitHundreds = Mini_CondorExtractDecimalDigit(0x64);
    frame.digitTens = Mini_CondorExtractDecimalDigit(0xA);
    frame.digitOnes = Mini_CondorExtractDecimalDigit(1);
    frame.glyphA = 0x8B;
    frame.glyphB = 0x8C;
    frame.glyphC = 0x8D;
    for (frame.glyphIndex = 0; frame.glyphIndex < 0xA; frame.glyphIndex++) {
        g_CondorCostDigitSprites[frame.glyphIndex] = frame.storage[frame.glyphIndex];
    }
    g_CondorCostDigitCount = 0xA;
    g_CondorCostDigitsVisible = 1;
}

/*
 * Refreshes status and stats display for active unit.
 */
// FUNCTION: FF7 0x006098cd
void __cdecl Mini_CondorRefreshActiveRecordDisplay(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[16];
        char blankA;
        struct { unsigned char pad_to_1[0x1]; char blankB; };
        struct { unsigned char pad_to_2[0x2]; char blankC; };
        struct { unsigned char pad_to_3[0x3]; char counterHundreds; };
        struct { unsigned char pad_to_4[0x4]; char counterTens; };
        struct { unsigned char pad_to_5[0x5]; char counterOnes; };
        struct { unsigned char pad_to_6[0x6]; char counterGlyph; };
        struct { unsigned char pad_to_7[0x7]; char counterMaxHundreds; };
        struct { unsigned char pad_to_8[0x8]; char counterMaxTens; };
        struct { unsigned char pad_to_9[0x9]; char counterMaxOnes; };
        struct { unsigned char pad_to_C[0xC]; int glyphIndex; };
    } frame;
#pragma pack(pop)

    frame.blankA = 0;
    frame.blankB = 0;
    frame.blankC = 0;
    g_CondorDigitStarted = 0;
    g_CondorReadoutValue = g_CondorRecordTable.records[g_CondorSelectedRecord].counter;
    frame.counterHundreds = Mini_CondorExtractDecimalDigit(0x64);
    frame.counterTens = Mini_CondorExtractDecimalDigit(0xA);
    frame.counterOnes = Mini_CondorExtractDecimalDigit(1);
    frame.counterGlyph = 0xC;
    g_CondorDigitStarted = 0;
    g_CondorReadoutValue = g_CondorRecordTable.records[g_CondorSelectedRecord].counterMax;
    frame.counterMaxHundreds = Mini_CondorExtractDecimalDigit(0x64);
    frame.counterMaxTens = Mini_CondorExtractDecimalDigit(0xA);
    frame.counterMaxOnes = Mini_CondorExtractDecimalDigit(1);
    for (frame.glyphIndex = 0; frame.glyphIndex < 0xA; frame.glyphIndex++) {
        g_CondorCounterDigitSprites[frame.glyphIndex] = frame.storage[frame.glyphIndex];
    }
    g_CondorCounterDigitCount = 0xA;
    g_CondorCounterDigitsVisible = 1;
    Mini_CondorSetSelectedReadout(g_CondorRecordTable.records[g_CondorSelectedRecord].unitIndexSigned, g_CondorSelectedRecord);
}

/*
 * Captures snapshot of display viewport origin coordinates.
 */
// FUNCTION: FF7 0x006099d9
void __cdecl Mini_CondorSnapshotDisplayOrigin(void)
{
    g_CondorSnapshotX = (short)g_CondorCameraX;
    g_CondorSnapshotY = (short)g_CondorCameraY;
}

/*
 * Draws HUD background backdrop panel.
 */
// FUNCTION: FF7 0x006099f8
short __cdecl Mini_CondorDrawDisplayBackground(void)
{
    struct {
        CondorOrigin rectZ;
        int rectZEnd[2];
        int scaledSnapshotY;
        int pair[2];
        int scaledSnapshotX;
        union {
            CondorFadeRect fadeRect;
            CondorRect copyRect;
        } rectR;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    if (g_CondorPhaseFrames == 0 && g_CondorPhase == 1) {
        SetRect(&frame.rectZ, g_CondorViewportX, g_CondorViewportY, g_CondorViewportWidth + g_CondorViewportX - g_CondorViewportOffsetX, g_CondorViewportHeight + g_CondorViewportInset);
        SetRect(&frame.rectR, (unsigned short)g_CondorSnapshotX, (unsigned short)g_CondorSnapshotY, (unsigned short)g_CondorSnapshotX + 0x140, (unsigned short)g_CondorSnapshotY + 0xDC);
        Mini_CondorDrawTiledFadeOverlay(&frame.rectR.fadeRect, &frame.rectZ, (char)g_CondorFadeAlpha);
        return 1;
    } else if (g_CondorMainState == CONDOR_STATE_HELP_SCREEN) {
        if (g_CondorTimSurface != 0) {
            SetRect(&frame.rectZ, g_CondorViewportX, g_CondorViewportY, g_CondorViewportWidth + g_CondorViewportX - g_CondorViewportOffsetX, g_CondorViewportHeight + g_CondorViewportInset);
            SetRect(&frame.rectR, 0, 0, g_CondorViewportWidth, g_CondorViewportHeight);
            if (g_CondorUsesSoftwareDriver == 0) {
                Mini_CondorCopyRectToSurface(g_CondorBackSurface, g_CondorTimSurface, &frame.rectR.copyRect, &frame.rectZ, g_CondorViewportWidth);
            } else if (Blit_CopyContextSurfaceSize(&frame.pair[0], GetEngineContext()) != 0) {
                Mini_CondorCopyRectToBuffer(frame.pair[1], g_CondorTimSurface, &frame.rectR.copyRect, &frame.rectZ, g_CondorViewportWidth, frame.pair[0]);
            }
        }
        return 1;
    } else {
        frame.scaledSnapshotX = (unsigned short)g_CondorSnapshotX * g_CondorViewportScale;
        frame.scaledSnapshotY = (unsigned short)g_CondorSnapshotY * g_CondorViewportScale;
        switch (g_CondorRequestedDisplayMode) {
        case CONDOR_DISPLAY_NONE:
        case CONDOR_DISPLAY_FULL:
            SetRect(&frame.rectZ, g_CondorViewportX, g_CondorViewportY, g_CondorViewportWidth + g_CondorViewportX - g_CondorViewportOffsetX, g_CondorViewportHeight + g_CondorViewportInset);
            SetRect(&frame.rectR, (unsigned short)g_CondorSnapshotX, (unsigned short)g_CondorSnapshotY, (unsigned short)g_CondorSnapshotX + 0x140, (unsigned short)g_CondorSnapshotY + 0xDC);
            Mini_CondorDrawTiledFadeOverlay(&frame.rectR.fadeRect, &frame.rectZ, 0x80);
            break;
        case CONDOR_DISPLAY_DIMMED:
            SetRect(&frame.rectZ, g_CondorViewportX, g_CondorViewportY, g_CondorViewportWidth + g_CondorViewportX - g_CondorViewportOffsetX, g_CondorViewportHeight + g_CondorViewportInset);
            SetRect(&frame.rectR, (unsigned short)g_CondorSnapshotX, (unsigned short)g_CondorSnapshotY, (unsigned short)g_CondorSnapshotX + 0x140, (unsigned short)g_CondorSnapshotY + 0xDC);
            Mini_CondorDrawTiledFadeOverlay(&frame.rectR.fadeRect, &frame.rectZ, 0x50);
            break;
        case CONDOR_DISPLAY_DARK:
            SetRect(&frame.rectZ, g_CondorViewportX, g_CondorViewportY, g_CondorViewportWidth + g_CondorViewportX - g_CondorViewportOffsetX, g_CondorViewportHeight + g_CondorViewportInset);
            SetRect(&frame.rectR, (unsigned short)g_CondorSnapshotX, (unsigned short)g_CondorSnapshotY, (unsigned short)g_CondorSnapshotX + 0x140, (unsigned short)g_CondorSnapshotY + 0xDC);
            Mini_CondorDrawTiledFadeOverlay(&frame.rectR.fadeRect, &frame.rectZ, 0x30);
        }
        return 1;
    }
}

/*
 * Requests the CONDOR_DISPLAY_FULL fade level.
 */
// FUNCTION: FF7 0x00609d89
void __cdecl Mini_CondorRequestDisplayMode1(void)
{
    g_CondorRequestedDisplayMode = CONDOR_DISPLAY_FULL;
    g_CondorDisplayModeRequest = 1;
    g_CondorDisplayModePending = g_CondorDisplayModeRequest;
}

/*
 * Requests the CONDOR_DISPLAY_DIMMED fade level.
 */
// FUNCTION: FF7 0x00609dac
void __cdecl Mini_CondorRequestDisplayMode2(void)
{
    g_CondorRequestedDisplayMode = CONDOR_DISPLAY_DIMMED;
    g_CondorDisplayModeRequest = 1;
    g_CondorDisplayModePending = g_CondorDisplayModeRequest;
}

/*
 * Requests the CONDOR_DISPLAY_DARK fade level.
 */
// FUNCTION: FF7 0x00609dcf
void __cdecl Mini_CondorRequestDisplayMode3(void)
{
    g_CondorRequestedDisplayMode = CONDOR_DISPLAY_DARK;
    g_CondorDisplayModeRequest = 1;
    g_CondorDisplayModePending = g_CondorDisplayModeRequest;
}

/*
 * Finalizes pending display mode request transition.
 */
// FUNCTION: FF7 0x00609df2
void __cdecl Mini_CondorCompleteDisplayModeRequest(void)
{
    if (g_CondorSubViewIndex == 0) {
        g_CondorDisplayModePending = 0;
    } else {
        g_CondorDisplayModeRequest = 0;
    }
    if ((g_CondorDisplayModePending | g_CondorDisplayModeRequest) == 0) {
        g_CondorDisplayMode = g_CondorRequestedDisplayMode;
        g_CondorRequestedDisplayMode = CONDOR_DISPLAY_NONE;
    }
}

/*
 * Ramps fade alpha level upwards toward 0x80.
 */
// FUNCTION: FF7 0x00609e3f
void __cdecl Mini_CondorRampFadeUpTo80(void)
{
    short val;

    val = g_CondorFadeAlpha & 0xFF;
    if (val < 0x80) {
        g_CondorFadeAlpha += 0x10;
    } else {
        Mini_CondorCompleteDisplayModeRequest();
    }
}

/*
 * Ramps the fade alpha towards 0x50: down from CONDOR_DISPLAY_FULL, up from CONDOR_DISPLAY_DARK.
 */
// FUNCTION: FF7 0x00609e77
void __cdecl Mini_CondorRampFadeTo50ByMode(void)
{
    if (g_CondorDisplayMode == CONDOR_DISPLAY_FULL) {
        if (g_CondorFadeAlpha > 0x50) {
            g_CondorFadeAlpha -= 0x10;
        } else {
            Mini_CondorCompleteDisplayModeRequest();
        }
    }
    if (g_CondorDisplayMode == CONDOR_DISPLAY_DARK) {
        if (g_CondorFadeAlpha < 0x50) {
            g_CondorFadeAlpha += 0x10;
        } else {
            Mini_CondorCompleteDisplayModeRequest();
        }
    }
    if (g_CondorDisplayMode == CONDOR_DISPLAY_DIMMED) {
        Mini_CondorCompleteDisplayModeRequest();
    }
}

/*
 * Ramps fade alpha level downwards toward 0x30.
 */
// FUNCTION: FF7 0x00609ee1
void __cdecl Mini_CondorRampFadeDownTo30(void)
{
    if (g_CondorFadeAlpha > 0x30) {
        g_CondorFadeAlpha -= 0x10;
    } else {
        Mini_CondorCompleteDisplayModeRequest();
    }
}

/*
 * Dispatches fade ramp step handler based on fade state.
 */
// FUNCTION: FF7 0x00609f03
void __cdecl Mini_CondorDispatchFadeRamp(void)
{
    if (g_CondorSequenceCounter != 0) {
        g_CondorSequenceCounter = 0;
    }
    switch (g_CondorRequestedDisplayMode) {
    case CONDOR_DISPLAY_FULL:
        Mini_CondorRampFadeUpTo80();
        break;
    case CONDOR_DISPLAY_DIMMED:
        Mini_CondorRampFadeTo50ByMode();
        break;
    case CONDOR_DISPLAY_DARK:
        Mini_CondorRampFadeDownTo30();
    }
}

/*
 * Writes quad vertex coordinates for textured polygon.
 */
// FUNCTION: FF7 0x00609f4f
void __cdecl Mini_CondorWriteQuadVertices(CondorQuadVertices *quad)
{
    GfxTlVertex *vertex1;
    GfxTlVertex *vertex2;
    GfxTlVertex *vertex4Ptr;
    GfxTlVertex *vertex3;

    if (AcquireBatchNode(1, quad->batchNode) != 0) {
        *quad->batchNode->colorSignedBytePtr = quad->colorByte;
        quad->batchNode->texturePage = quad->texturePage;
        vertex1 = quad->batchNode->vertices;
        vertex1->xBits = quad->x;
        vertex1->yBits = quad->y;
        vertex1->zBits = quad->posZ;
        vertex1->rhwBits = FLOAT_ONE_BITS;
        vertex1->color = quad->color;
        vertex1->specular = 0xFF000000;
        vertex1->uBits = quad->x2;
        vertex1->vBits = quad->y2;
        vertex2 = quad->batchNode->vertices + 1;
        vertex2->xBits = quad->x;
        vertex2->y = quad->fy + quad->dy;
        vertex2->zBits = quad->posZ;
        vertex2->rhwBits = FLOAT_ONE_BITS;
        vertex2->color = quad->color;
        vertex2->specular = 0xFF000000;
        vertex2->uBits = quad->x2;
        vertex2->v = quad->v + quad->dy2;
        vertex3 = quad->batchNode->vertices + 2;
        vertex3->x = quad->fx + quad->dx;
        vertex3->yBits = quad->y;
        vertex3->zBits = quad->posZ;
        vertex3->rhwBits = FLOAT_ONE_BITS;
        vertex3->color = quad->color;
        vertex3->specular = 0xFF000000;
        vertex3->u = quad->u + quad->dx2;
        vertex3->vBits = quad->y2;
        vertex4Ptr = quad->batchNode->vertices + 3;
        vertex4Ptr->x = quad->fx + quad->dx;
        vertex4Ptr->y = quad->fy + quad->dy;
        vertex4Ptr->zBits = quad->posZ;
        vertex4Ptr->rhwBits = FLOAT_ONE_BITS;
        vertex4Ptr->color = quad->color;
        vertex4Ptr->specular = 0xFF000000;
        vertex4Ptr->u = quad->u + quad->dx2;
        vertex4Ptr->v = quad->v + quad->dy2;
    }
}

/*
 * Renders tiled fullscreen fade transition overlay.
 */
// FUNCTION: FF7 0x0060a160
void __cdecl Mini_CondorDrawTiledFadeOverlay(CondorFadeRect *clipRect, CondorOrigin *origin, unsigned char fadeAmount)
{
    int width;
    int hgt;
    CondorQuadVertices box;
    int top;
    int junk;
    int l;
    unsigned short index;
    int shade;
    int left;

    shade = fadeAmount << 1;
    if (shade > 0xFF) {
        shade = 0xFF;
    }
    box.color = (shade << 16) | 0x80000000 | (shade << 8) | shade;
    for (index = 0; index < 8; index++) {
        if (g_CondorFadeTileOffsets[index][0] + 0x100 < clipRect->x || g_CondorFadeTileOffsets[index][0] > clipRect->width) {
            continue;
        }
        if (g_CondorFadeTileOffsets[index][1] + 0x100 < clipRect->y || g_CondorFadeTileOffsets[index][1] > clipRect->height) {
            continue;
        }
        left = g_CondorFadeTileOffsets[index][0] > clipRect->x ? g_CondorFadeTileOffsets[index][0] : clipRect->x;
        top = g_CondorFadeTileOffsets[index][1] > clipRect->y ? g_CondorFadeTileOffsets[index][1] : clipRect->y;
        width = (g_CondorFadeTileOffsets[index][0] + 0x100 < clipRect->width ? g_CondorFadeTileOffsets[index][0] + 0x100 : clipRect->width) - left;
        hgt = (g_CondorFadeTileOffsets[index][1] + 0x100 < clipRect->height ? g_CondorFadeTileOffsets[index][1] + 0x100 : clipRect->height) - top;
        box.tex = 0;
        box.batch = ((CondorRenderObject *)&g_CondorRenderObjects)[index].renderObject;
        box.fz = 0.98f;
        box.fx = (float)(origin->x + (left - clipRect->x) * g_CondorViewportScale);
        box.fy = (float)(origin->y + (top - clipRect->y) * g_CondorViewportScale);
        box.dx = (float)(width * g_CondorViewportScale);
        box.dy = (float)(hgt * g_CondorViewportScale);
        left -= g_CondorFadeTileOffsets[index][0];
        top -= g_CondorFadeTileOffsets[index][1];
        box.u = (float)left / 256.0f;
        box.v = (float)top / 256.0f;
        box.dx2 = (float)width / 256.0f;
        box.dy2 = (float)hgt / 256.0f;
        Mini_CondorWriteQuadVertices(&box);
        ((CondorRenderObject *)&g_CondorRenderObjectsTail)[index].renderObject = 1;
    }
}
} /* extern "C" */
