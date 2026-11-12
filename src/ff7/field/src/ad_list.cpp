// src/ff7/field/src/ad_list.cpp
// Original: C:\FF7\field\src\ad_list.cpp
// First 137 bytes are file-anchored; the extension is inferred from native layout.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/api/field.h"
#include "lib/codegen.h"

#include "lib/api/mem.h"
#include "ff7/types/field.h"
#include "lib/types/graphics.h"
#include "lib/api/graphics.h"
extern "C" {

/* Small negative texture-coordinate bias (-7/4096, 0xBAE00000) used by the fallback renderer. */
#define FIELD_AD_LIST_CORNER_BIAS (-0.001708984375f)

// clang-format off
// GLOBAL: FF7 0x00d00010
float g_FieldAdListMatrix_00 = 0.0f;
// GLOBAL: FF7 0x00d00014
float g_FieldAdListMatrix_01 = 0.0f;
// GLOBAL: FF7 0x00d00018
float g_FieldAdListMatrix_02 = 0.0f;
// GLOBAL: FF7 0x00d0001c
float g_FieldAdListMatrix_03 = 0.0f;
// GLOBAL: FF7 0x00d00020
float g_FieldAdListMatrix_10 = 0.0f;
// GLOBAL: FF7 0x00d00024
float g_FieldAdListMatrix_11 = 0.0f;
// GLOBAL: FF7 0x00d00028
float g_FieldAdListMatrix_12 = 0.0f;
// GLOBAL: FF7 0x00d0002c
float g_FieldAdListMatrix_13 = 0.0f;
// GLOBAL: FF7 0x00d00030
float g_FieldAdListMatrix_20 = 0.0f;
// GLOBAL: FF7 0x00d00034
float g_FieldAdListMatrix_21 = 0.0f;
// GLOBAL: FF7 0x00d00038
float g_FieldAdListMatrix_22 = 0.0f;
// GLOBAL: FF7 0x00d0003c
float g_FieldAdListMatrix_23 = 0.0f;
// GLOBAL: FF7 0x00d00040
float g_FieldAdListMatrix_30 = 0.0f;
// GLOBAL: FF7 0x00d00044
float g_FieldAdListMatrix_31 = 0.0f;
// GLOBAL: FF7 0x00d00048
float g_FieldAdListMatrix_32 = 0.0f;
// GLOBAL: FF7 0x00d0004c
float g_FieldAdListMatrix_33 = 0.0f;
// GLOBAL: FF7 0x00d00050
FieldAdListBank *g_pFieldAdListBuffer = 0;
unsigned char g_bssPad_00d00054[4] = {0};
// clang-format on

/*
 * Reallocates the field's 0x3F0A8-byte list block at g_pFieldAdListBuffer, freeing the previous one,
 * initialises it through Field_ResetAdListBankCounts and returns 1; 0 when the allocation failed.
 */
// FUNCTION: FF7 0x006463f0
int __cdecl Field_AllocAdListBuffer(void)
{
    if (g_pFieldAdListBuffer != 0) {
        // STRING: FF7 0x00907f64
        GameHeapFree(g_pFieldAdListBuffer, "C:\\FF7\\field\\src\\ad_list.cpp", 154);
    }
    // STRING: FF7 0x00907f84
    g_pFieldAdListBuffer = (FieldAdListBank *)GameHeapAllocRaw(258216, "C:\\FF7\\field\\src\\ad_list.cpp", 157);
    if (g_pFieldAdListBuffer == 0) {
        return 0;
    } else {
        Field_ResetAdListBankCounts();
        return 1;
    }
}

/*
 * Frees the ad_list buffer g_pFieldAdListBuffer if allocated.
 */
// FUNCTION: FF7 0x00646449
void __cdecl Field_FreeAdListBuffer(void)
{
    if (g_pFieldAdListBuffer != 0) {
        // STRING: FF7 0x00907fa4
        GameHeapFree(g_pFieldAdListBuffer, "C:\\FF7\\field\\src\\ad_list.cpp", 171);
        g_pFieldAdListBuffer = 0;
    }
}

/*
 * Empties the 42 (0x2A) banks of the ad list: sets every bank's entry count to 0 when the buffer exists.
 */
// FUNCTION: FF7 0x00646479
void __cdecl Field_ResetAdListBankCounts(void)
{
    int i;

    if (g_pFieldAdListBuffer != 0) {
        for (i = 0; i < 42; i++) {
            g_pFieldAdListBuffer[i].count = 0;
        }
    }
}

// Appends one tile to a bank of g_pFieldAdListBuffer, at the index given by the bank's current count.
// The entry gets posX (g_FieldDisplayOffsetX + offsetX), posY (g_FieldDisplayHeight + offsetY), depth,
// texU (attrB) and texV (attrC) as floats, and attrD as an int. The count is then incremented.
// FUNCTION: FF7 0x006464ba
void __cdecl Field_EmitBackgroundTile(float offsetX, float offsetY, float depthBits, float attrB, float attrC, int attrD, int bank)
{
    g_pFieldAdListBuffer[bank].entries[g_pFieldAdListBuffer[bank].count].posX = g_FieldDisplayOffsetX + offsetX;
    g_pFieldAdListBuffer[bank].entries[g_pFieldAdListBuffer[bank].count].posY = g_FieldDisplayHeight + offsetY;
    g_pFieldAdListBuffer[bank].entries[g_pFieldAdListBuffer[bank].count].depth = depthBits;
    g_pFieldAdListBuffer[bank].entries[g_pFieldAdListBuffer[bank].count].texU = attrB;
    g_pFieldAdListBuffer[bank].entries[g_pFieldAdListBuffer[bank].count].texV = attrC;
    g_pFieldAdListBuffer[bank].entries[g_pFieldAdListBuffer[bank].count].attrD = attrD;
    (g_pFieldAdListBuffer[bank].count)++;
}

/*
 * Builds the background tiles' quads for the frame: for each of the 42 background layers with
 * entries in g_pFieldAdListBuffer, acquires that many quads in the layer's batch node and writes
 * four vertices per tile from its position and depth, with rhw 1, the fade colour g_FieldFxFadeColor
 * and texture coordinates scaled by 1/16 (1/8 and a doubled x scale for a layer with hasDoubleSizeTiles
 * set). The corner offsets g_FieldAdListMatrix_* are a small negative bias (FIELD_AD_LIST_CORNER_BIAS) when the
 * engine context runs its fallback renderer, else zero.
 */
// FUNCTION: FF7 0x006465fb
void __cdecl Field_BuildBackgroundQuads(void)
{
    float texStep;
    GfxEngineContext *context;
    int entry;
    GfxTlVertex *pTlVertex;
    GfxTlVertex *lowerRight;
    GfxTlVertex *upperRightPtr;
    GfxTlVertex *lowerRemaining;
    GfxTlVertex *upperLeftPtr;
    GfxTlVertex *pBottomRight;
    GfxTlVertex *pTrVertex;
    GfxTlVertex *bottomLeftPtr;
    float quadSize;
    int bgLayer;

    context = GetEngineContext();
    if (context->fallbackFlag != 0 && context->fallbackUsed == 0) {
        g_FieldAdListMatrix_00 = FIELD_AD_LIST_CORNER_BIAS;
        g_FieldAdListMatrix_01 = FIELD_AD_LIST_CORNER_BIAS;
        g_FieldAdListMatrix_02 = 0.0f;
        g_FieldAdListMatrix_03 = 0.0f;
        g_FieldAdListMatrix_21 = FIELD_AD_LIST_CORNER_BIAS;
        g_FieldAdListMatrix_20 = FIELD_AD_LIST_CORNER_BIAS;
        g_FieldAdListMatrix_23 = 0.0f;
        g_FieldAdListMatrix_22 = 0.0f;
        g_FieldAdListMatrix_10 = FIELD_AD_LIST_CORNER_BIAS;
        g_FieldAdListMatrix_11 = FIELD_AD_LIST_CORNER_BIAS;
        g_FieldAdListMatrix_12 = 0.0f;
        g_FieldAdListMatrix_13 = 0.0f;
        g_FieldAdListMatrix_30 = FIELD_AD_LIST_CORNER_BIAS;
        g_FieldAdListMatrix_32 = FIELD_AD_LIST_CORNER_BIAS;
        g_FieldAdListMatrix_31 = 0.0f;
        g_FieldAdListMatrix_33 = 0.0f;
    } else {
        g_FieldAdListMatrix_00 = 0.0f;
        g_FieldAdListMatrix_01 = 0.0f;
        g_FieldAdListMatrix_02 = 0.0f;
        g_FieldAdListMatrix_03 = 0.0f;
        g_FieldAdListMatrix_21 = 0.0f;
        g_FieldAdListMatrix_20 = 0.0f;
        g_FieldAdListMatrix_23 = 0.0f;
        g_FieldAdListMatrix_22 = 0.0f;
        g_FieldAdListMatrix_10 = 0.0f;
        g_FieldAdListMatrix_11 = 0.0f;
        g_FieldAdListMatrix_12 = 0.0f;
        g_FieldAdListMatrix_13 = 0.0f;
        g_FieldAdListMatrix_30 = 0.0f;
        g_FieldAdListMatrix_32 = 0.0f;
        g_FieldAdListMatrix_31 = 0.0f;
        g_FieldAdListMatrix_33 = 0.0f;
    }
    for (bgLayer = 0; bgLayer < 0x2A; bgLayer++) {
        if (g_pFieldAdListBuffer[bgLayer].count > 0) {
            if (g_FieldLayers[bgLayer]->hasDoubleSizeTiles == 0) {
                texStep = 0.0625f;
                quadSize = g_FieldDisplayScaleX;
            } else {
                texStep = 0.125f;
                quadSize = g_FieldDisplayScaleX * 2.0f;
            }
            if (AcquireBatchNode(g_pFieldAdListBuffer[bgLayer].count, g_FieldLayers[bgLayer]->batch) != 0) {
                if (g_FieldLayers[bgLayer]->bytesPerPixel != 2) {
                    for (entry = 0; entry < g_pFieldAdListBuffer[bgLayer].count; entry++) {
                        *g_FieldLayers[bgLayer]->batch->colorSignedBytePtr = g_pFieldAdListBuffer[bgLayer].entries[entry].attrDByte;
                        g_FieldLayers[bgLayer]->batch->texturePage = g_pFieldAdListBuffer[bgLayer].entries[entry].attrD;
                        pTlVertex = g_FieldLayers[bgLayer]->batch->vertices;
                        pTlVertex->xBits = g_pFieldAdListBuffer[bgLayer].entries[entry].posXBits;
                        pTlVertex->yBits = g_pFieldAdListBuffer[bgLayer].entries[entry].posYBits;
                        pTlVertex->zBits = g_pFieldAdListBuffer[bgLayer].entries[entry].depthBits;
                        pTlVertex->rhwBits = FLOAT_ONE_BITS;
                        pTlVertex->color = g_FieldFxFadeColor;
                        pTlVertex->specular = 0xFF000000;
                        pTlVertex->u = g_pFieldAdListBuffer[bgLayer].entries[entry].texU + g_FieldAdListMatrix_00;
                        pTlVertex->v = g_pFieldAdListBuffer[bgLayer].entries[entry].texV + g_FieldAdListMatrix_21;
                        bottomLeftPtr = &g_FieldLayers[bgLayer]->batch->vertices[1];
                        bottomLeftPtr->xBits = g_pFieldAdListBuffer[bgLayer].entries[entry].posXBits;
                        bottomLeftPtr->y = g_pFieldAdListBuffer[bgLayer].entries[entry].posY + quadSize;
                        bottomLeftPtr->zBits = g_pFieldAdListBuffer[bgLayer].entries[entry].depthBits;
                        bottomLeftPtr->rhwBits = FLOAT_ONE_BITS;
                        bottomLeftPtr->color = g_FieldFxFadeColor;
                        bottomLeftPtr->specular = 0xFF000000;
                        bottomLeftPtr->u = g_pFieldAdListBuffer[bgLayer].entries[entry].texU + g_FieldAdListMatrix_01;
                        bottomLeftPtr->v = g_pFieldAdListBuffer[bgLayer].entries[entry].texV + g_FieldAdListMatrix_20 + texStep;
                        pTrVertex = &g_FieldLayers[bgLayer]->batch->vertices[2];
                        pTrVertex->x = g_pFieldAdListBuffer[bgLayer].entries[entry].posX + quadSize;
                        pTrVertex->yBits = g_pFieldAdListBuffer[bgLayer].entries[entry].posYBits;
                        pTrVertex->zBits = g_pFieldAdListBuffer[bgLayer].entries[entry].depthBits;
                        pTrVertex->rhwBits = FLOAT_ONE_BITS;
                        pTrVertex->color = g_FieldFxFadeColor;
                        pTrVertex->specular = 0xFF000000;
                        pTrVertex->u = g_pFieldAdListBuffer[bgLayer].entries[entry].texU + texStep + g_FieldAdListMatrix_02;
                        pTrVertex->v = g_pFieldAdListBuffer[bgLayer].entries[entry].texV + g_FieldAdListMatrix_23;
                        pBottomRight = &g_FieldLayers[bgLayer]->batch->vertices[3];
                        pBottomRight->x = g_pFieldAdListBuffer[bgLayer].entries[entry].posX + quadSize;
                        pBottomRight->y = g_pFieldAdListBuffer[bgLayer].entries[entry].posY + quadSize;
                        pBottomRight->zBits = g_pFieldAdListBuffer[bgLayer].entries[entry].depthBits;
                        pBottomRight->rhwBits = FLOAT_ONE_BITS;
                        pBottomRight->color = g_FieldFxFadeColor;
                        pBottomRight->specular = 0xFF000000;
                        pBottomRight->u = g_pFieldAdListBuffer[bgLayer].entries[entry].texU + g_FieldAdListMatrix_03 + texStep;
                        pBottomRight->v = g_pFieldAdListBuffer[bgLayer].entries[entry].texV + g_FieldAdListMatrix_22 + texStep;
                        g_FieldLayers[bgLayer]->batch->vertexCursor += g_FieldLayers[bgLayer]->batch->cursorStep;
                        g_FieldLayers[bgLayer]->batch->colorSignedBytePtr++;
                    }
                } else {
                    for (entry = 0; entry < g_pFieldAdListBuffer[bgLayer].count; entry++) {
                        upperLeftPtr = g_FieldLayers[bgLayer]->batch->vertices;
                        upperLeftPtr->xBits = g_pFieldAdListBuffer[bgLayer].entries[entry].posXBits;
                        upperLeftPtr->yBits = g_pFieldAdListBuffer[bgLayer].entries[entry].posYBits;
                        upperLeftPtr->zBits = g_pFieldAdListBuffer[bgLayer].entries[entry].depthBits;
                        upperLeftPtr->rhwBits = FLOAT_ONE_BITS;
                        upperLeftPtr->color = g_FieldFxFadeColor;
                        upperLeftPtr->specular = 0xFF000000;
                        upperLeftPtr->u = g_pFieldAdListBuffer[bgLayer].entries[entry].texU + g_FieldAdListMatrix_10;
                        upperLeftPtr->v = g_pFieldAdListBuffer[bgLayer].entries[entry].texV + g_FieldAdListMatrix_30;
                        lowerRemaining = &g_FieldLayers[bgLayer]->batch->vertices[1];
                        lowerRemaining->xBits = g_pFieldAdListBuffer[bgLayer].entries[entry].posXBits;
                        lowerRemaining->y = g_pFieldAdListBuffer[bgLayer].entries[entry].posY + quadSize;
                        lowerRemaining->zBits = g_pFieldAdListBuffer[bgLayer].entries[entry].depthBits;
                        lowerRemaining->rhwBits = FLOAT_ONE_BITS;
                        lowerRemaining->color = g_FieldFxFadeColor;
                        lowerRemaining->specular = 0xFF000000;
                        lowerRemaining->u = g_pFieldAdListBuffer[bgLayer].entries[entry].texU + g_FieldAdListMatrix_11;
                        lowerRemaining->v = g_pFieldAdListBuffer[bgLayer].entries[entry].texV + texStep + g_FieldAdListMatrix_32;
                        upperRightPtr = &g_FieldLayers[bgLayer]->batch->vertices[2];
                        upperRightPtr->x = g_pFieldAdListBuffer[bgLayer].entries[entry].posX + quadSize;
                        upperRightPtr->yBits = g_pFieldAdListBuffer[bgLayer].entries[entry].posYBits;
                        upperRightPtr->zBits = g_pFieldAdListBuffer[bgLayer].entries[entry].depthBits;
                        upperRightPtr->rhwBits = FLOAT_ONE_BITS;
                        upperRightPtr->color = g_FieldFxFadeColor;
                        upperRightPtr->specular = 0xFF000000;
                        upperRightPtr->u = g_pFieldAdListBuffer[bgLayer].entries[entry].texU + texStep + g_FieldAdListMatrix_12;
                        upperRightPtr->v = g_pFieldAdListBuffer[bgLayer].entries[entry].texV + g_FieldAdListMatrix_31;
                        lowerRight = &g_FieldLayers[bgLayer]->batch->vertices[3];
                        lowerRight->x = g_pFieldAdListBuffer[bgLayer].entries[entry].posX + quadSize;
                        lowerRight->y = g_pFieldAdListBuffer[bgLayer].entries[entry].posY + quadSize;
                        lowerRight->zBits = g_pFieldAdListBuffer[bgLayer].entries[entry].depthBits;
                        lowerRight->rhwBits = FLOAT_ONE_BITS;
                        lowerRight->color = g_FieldFxFadeColor;
                        lowerRight->specular = 0xFF000000;
                        lowerRight->u = g_pFieldAdListBuffer[bgLayer].entries[entry].texU + texStep + g_FieldAdListMatrix_13;
                        lowerRight->v = g_pFieldAdListBuffer[bgLayer].entries[entry].texV + texStep + g_FieldAdListMatrix_33;
                        g_FieldLayers[bgLayer]->batch->vertexCursor += g_FieldLayers[bgLayer]->batch->cursorStep;
                    }
                }
            }
            g_FieldLayers[bgLayer]->built = 1;
        }
    }
}
} /* extern "C" */
