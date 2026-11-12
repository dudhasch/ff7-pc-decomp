// src/ff7/src/credits/credits_fade_overlay_screen.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Credits screen overlay and backdrop drawing: a full-screen fade overlay, the tiled
//   background, logo and half-alpha text quads in both resolution modes, and the fade strips with
//   their particle records.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include <stdlib.h>
#include "lib/codegen.h"
#include <string.h>
#include "lib/pfile.h"
#include "lib/crt.h"
#include "ff7/enums/main.h"
#include "ff7/types/credits.h"
#include "ff7/api/credits.h"
#include "ff7/api/main.h"
#include "lib/api/graphics.h"
#include "lib/types/graphics.h"
extern "C" {

#pragma intrinsic(strcat)
void __cdecl Credits_DrawLogoWithAlpha2x(char alpha);
void __cdecl Credits_DrawLogoWithAlpha1x(char alpha);

// clang-format off
// GLOBAL: FF7 0x00f4f5a0
int g_CreditsFadeOverlayLeft = 0;
// GLOBAL: FF7 0x00f4f5a4
int g_CreditsFadeOverlayTop = 0;
// GLOBAL: FF7 0x00f4f5a8
int g_CreditsFadeOverlayRight = 0;
// GLOBAL: FF7 0x00f4f5ac
int g_CreditsFadeOverlayBottom = 0;
// One record per fade strip (Credits_InitParticleRecords, Credits_UpdateFadeStrips); record 0 also holds
// the count of strips still fading.
// GLOBAL: FF7 0x00f4f5b0
struct CreditsParticleRecord g_CreditsParticleRecords[76] = {0};
// GLOBAL: FF7 0x00f4fa70
int g_CreditsParticleStartX = 0;
// GLOBAL: FF7 0x00f4fa74
int g_CreditsParticleStartY = 0;
// GLOBAL: FF7 0x00f4fa78
int g_CreditsParticleStepX = 0;
// GLOBAL: FF7 0x00f4fa7c
int g_CreditsParticleHeight = 0;
// GLOBAL: FF7 0x00f4fa80
CreditsAlphaValue g_CreditsFadeOverlayAlpha = {0};
// GLOBAL: FF7 0x00f4fa84
int g_CreditsFadeOverlayStep = 0;
unsigned char g_bssPad_00f4fa88[4] = {0};
// GLOBAL: FF7 0x00f4fa8c
int g_CreditsFadeOverlayActive = 0;
// clang-format on

/*
 * Store top-left coordinates and compute right/bottom boundaries for the credits fade overlay.
 */
// FUNCTION: FF7 0x007aa820
void __cdecl Credits_SetFadeOverlayRect(int left, int top, int width, int height)
{
    g_CreditsFadeOverlayLeft = left;
    g_CreditsFadeOverlayTop = top;
    g_CreditsFadeOverlayRight = left + width;
    g_CreditsFadeOverlayBottom = top + height;
}

/*
 * Begin fading out the credits overlay starting from fully opaque (alpha 0xFF) with negative step.
 */
// FUNCTION: FF7 0x007aa84d
void __cdecl Credits_StartFadeOverlayFromOpaque(int step)
{
    g_CreditsFadeOverlayStep = -step;
    g_CreditsFadeOverlayAlpha.value = 0xFF;
    g_CreditsFadeOverlayActive = 1;
}

/*
 * Begin fading in the credits overlay starting from fully clear (alpha 0) with the given step.
 */
// FUNCTION: FF7 0x007aa870
void __cdecl Credits_StartFadeOverlayFromClear(int value)
{
    g_CreditsFadeOverlayStep = value;
    g_CreditsFadeOverlayAlpha.value = 0;
    g_CreditsFadeOverlayActive = 1;
}

/*
 * Return whether the credits fade overlay is currently active.
 */
// FUNCTION: FF7 0x007aa891
int __cdecl Credits_IsFadeOverlayActive(void)
{
    return g_CreditsFadeOverlayActive;
}

/*
 * Steps the credits fade to or from black by one frame. While a fade runs
 * (g_CreditsFadeOverlayActive, started by Credits_StartFadeOverlayFromOpaque or
 * Credits_StartFadeOverlayFromClear) it adds g_CreditsFadeOverlayStep to the alpha; once the alpha
 * leaves 0..255 it ends the fade and uses 0 or 255 for this last frame. It then writes the four
 * corners of the overlay rectangle (Credits_SetFadeOverlayRect) into the fade render object as
 * black with that alpha, depth 0. It only fills the vertices: Credits_ModuleFrame, which calls it
 * each frame, emits the object with the render passes. Without a fade it does nothing.
 */
// FUNCTION: FF7 0x007aa89b
void __cdecl Credits_UpdateAndDrawFadeOverlay(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[20];
        GfxTlVertex *vertBottomRight;
        struct { unsigned char pad_to_4[0x4]; GfxTlVertex *vertTopRight; };
        struct { unsigned char pad_to_8[0x8]; GfxTlVertex *vertBottomLeft; };
        struct { unsigned char pad_to_C[0xC]; GfxTlVertex *vertTopLeft; };
        struct { unsigned char pad_to_10[0x10]; union { int color; char blue; }; };
        struct { unsigned char pad_to_11[0x11]; char tmpByteA; };
        struct { unsigned char pad_to_12[0x12]; char tmpByteB; };
        struct { unsigned char pad_to_13[0x13]; char alpha; };
    } frame;
#pragma pack(pop)

    if (g_CreditsFadeOverlayActive != 0) {
        frame.blue = 0;
        frame.tmpByteA = frame.blue;
        frame.tmpByteB = frame.tmpByteA;
        g_CreditsFadeOverlayAlpha.value += g_CreditsFadeOverlayStep;
        if (g_CreditsFadeOverlayAlpha.value < 0) {
            g_CreditsFadeOverlayActive = 0;
            frame.alpha = 0;
        } else if (g_CreditsFadeOverlayAlpha.value > 0xFF) {
            g_CreditsFadeOverlayActive = 0;
            frame.alpha = 0xFF;
        } else {
            frame.alpha = g_CreditsFadeOverlayAlpha.lowByte;
        }
        if (AcquireBatchNode(1, (void *)g_CreditsFadeOverlayBatch.renderObject) != 0) {
            frame.vertTopLeft = g_CreditsFadeOverlayBatch.fadeOverlay->vertices;
            frame.vertTopLeft->x = (float)g_CreditsFadeOverlayLeft;
            frame.vertTopLeft->y = (float)g_CreditsFadeOverlayTop;
            frame.vertTopLeft->zBits = 0;
            frame.vertTopLeft->rhwBits = FLOAT_ONE_BITS;
            frame.vertTopLeft->color = frame.color;
            frame.vertTopLeft->specular = 0xFF000000;
            frame.vertBottomLeft = g_CreditsFadeOverlayBatch.fadeOverlay->vertices + 1;
            frame.vertBottomLeft->x = (float)g_CreditsFadeOverlayLeft;
            frame.vertBottomLeft->y = (float)g_CreditsFadeOverlayBottom;
            frame.vertBottomLeft->zBits = 0;
            frame.vertBottomLeft->rhwBits = FLOAT_ONE_BITS;
            frame.vertBottomLeft->color = frame.color;
            frame.vertBottomLeft->specular = 0xFF000000;
            frame.vertTopRight = g_CreditsFadeOverlayBatch.fadeOverlay->vertices + 2;
            frame.vertTopRight->x = (float)g_CreditsFadeOverlayRight;
            frame.vertTopRight->y = (float)g_CreditsFadeOverlayTop;
            frame.vertTopRight->zBits = 0;
            frame.vertTopRight->rhwBits = FLOAT_ONE_BITS;
            frame.vertTopRight->color = frame.color;
            frame.vertTopRight->specular = 0xFF000000;
            frame.vertBottomRight = g_CreditsFadeOverlayBatch.fadeOverlay->vertices + 3;
            frame.vertBottomRight->x = (float)g_CreditsFadeOverlayRight;
            frame.vertBottomRight->y = (float)g_CreditsFadeOverlayBottom;
            frame.vertBottomRight->zBits = 0;
            frame.vertBottomRight->rhwBits = FLOAT_ONE_BITS;
            frame.vertBottomRight->color = frame.color;
            frame.vertBottomRight->specular = 0xFF000000;
        }
    }
}

/*
 * Acquire texture batches and draw the six tiled background quads covering a 640x480 resolution display.
 */
// FUNCTION: FF7 0x007aaa5b
void __cdecl Credits_DrawBackground2x(void)
{
    float vStart;
    GfxTlVertex *pTlVertex1;
    GfxTlVertex *vertTl0;
    GfxTlVertex *vertTm1;
    GfxTlVertex *tmVertexPtr;
    GfxTlVertex *rawPTrVertex;
    GfxTlVertex *vertexBl0Ptr;
    GfxTlVertex *pickedVertexBl;
    GfxTlVertex *bmVertex0;
    GfxTlVertex *bmCorner1Ptr;
    GfxTlVertex *quadBr0;
    int depth;
    float vSum;
    float uSpan;
    GfxTlVertex *tlVertex3Ptr;
    GfxTlVertex *trVtxPtr;
    GfxTlVertex *trCorner0;
    GfxTlVertex *blVertex1Ptr;
    GfxTlVertex *blLowLeftPtr;
    GfxTlVertex *stageVertexBm;
    GfxTlVertex *brVertex1;
    GfxTlVertex *vertexBr3Ptr;
    GfxTlVertex *brVertex2;
    float curUStartScalar;
    GfxTlVertex *quadTm3Ptr;
    GfxTlVertex *trVert;
    GfxTlVertex *pTlVertex2;
    GfxTlVertex *outTmVertex;
    GfxTlVertex *bmVertex2;

    depth = FLOAT_0_2_BITS;
    vStart = 0.0f;
    curUStartScalar = vStart;
    vSum = 1.0f;
    uSpan = vSum;
    if (AcquireBatchNode(1, (void *)g_CreditsFadeOverlayBatch.backdropSlotA) != 0) {
        vertTl0 = g_CreditsFadeOverlayBatch.backdropA->vertices;
        vertTl0->xBits = 0;
        vertTl0->yBits = 0;
        vertTl0->zBits = depth;
        vertTl0->rhwBits = FLOAT_ONE_BITS;
        vertTl0->color = 0x80FFFFFF;
        vertTl0->specular = 0xFF000000;
        vertTl0->u = curUStartScalar;
        vertTl0->v = vStart;
        pTlVertex1 = g_CreditsFadeOverlayBatch.backdropA->vertices + 1;
        pTlVertex1->xBits = 0;
        pTlVertex1->yBits = FLOAT_256_BITS;
        pTlVertex1->zBits = depth;
        pTlVertex1->rhwBits = FLOAT_ONE_BITS;
        pTlVertex1->color = 0x80FFFFFF;
        pTlVertex1->specular = 0xFF000000;
        pTlVertex1->u = curUStartScalar;
        pTlVertex1->v = vStart + vSum;
        pTlVertex2 = g_CreditsFadeOverlayBatch.backdropA->vertices + 2;
        pTlVertex2->xBits = FLOAT_256_BITS;
        pTlVertex2->yBits = 0;
        pTlVertex2->zBits = depth;
        pTlVertex2->rhwBits = FLOAT_ONE_BITS;
        pTlVertex2->color = 0x80FFFFFF;
        pTlVertex2->specular = 0xFF000000;
        pTlVertex2->u = curUStartScalar + uSpan;
        pTlVertex2->v = vStart;
        tlVertex3Ptr = g_CreditsFadeOverlayBatch.backdropA->vertices + 3;
        tlVertex3Ptr->xBits = FLOAT_256_BITS;
        tlVertex3Ptr->yBits = FLOAT_256_BITS;
        tlVertex3Ptr->zBits = depth;
        tlVertex3Ptr->rhwBits = FLOAT_ONE_BITS;
        tlVertex3Ptr->color = 0x80FFFFFF;
        tlVertex3Ptr->specular = 0xFF000000;
        tlVertex3Ptr->u = curUStartScalar + uSpan;
        tlVertex3Ptr->v = vStart + vSum;
        *g_CreditsFadeOverlayBatch.backdropA->colorSignedBytePtr = 0;
        g_CreditsFadeOverlayBatch.backdropA->texturePage = 0;
    }
    if (AcquireBatchNode(1, (void *)g_CreditsFadeOverlayBatch.packetSlotB) != 0) {
        tmVertexPtr = g_CreditsFadeOverlayBatch.backdropB->vertices;
        tmVertexPtr->xBits = FLOAT_256_BITS;
        tmVertexPtr->yBits = 0;
        tmVertexPtr->zBits = depth;
        tmVertexPtr->rhwBits = FLOAT_ONE_BITS;
        tmVertexPtr->color = 0x80FFFFFF;
        tmVertexPtr->specular = 0xFF000000;
        tmVertexPtr->u = curUStartScalar;
        tmVertexPtr->v = vStart;
        vertTm1 = g_CreditsFadeOverlayBatch.backdropB->vertices + 1;
        vertTm1->xBits = FLOAT_256_BITS;
        vertTm1->yBits = FLOAT_256_BITS;
        vertTm1->zBits = depth;
        vertTm1->rhwBits = FLOAT_ONE_BITS;
        vertTm1->color = 0x80FFFFFF;
        vertTm1->specular = 0xFF000000;
        vertTm1->u = curUStartScalar;
        vertTm1->v = vStart + vSum;
        outTmVertex = g_CreditsFadeOverlayBatch.backdropB->vertices + 2;
        outTmVertex->xBits = FLOAT_512_BITS;
        outTmVertex->yBits = 0;
        outTmVertex->zBits = depth;
        outTmVertex->rhwBits = FLOAT_ONE_BITS;
        outTmVertex->color = 0x80FFFFFF;
        outTmVertex->specular = 0xFF000000;
        outTmVertex->u = curUStartScalar + uSpan;
        outTmVertex->v = vStart;
        quadTm3Ptr = g_CreditsFadeOverlayBatch.backdropB->vertices + 3;
        quadTm3Ptr->xBits = FLOAT_512_BITS;
        quadTm3Ptr->yBits = FLOAT_256_BITS;
        quadTm3Ptr->zBits = depth;
        quadTm3Ptr->rhwBits = FLOAT_ONE_BITS;
        quadTm3Ptr->color = 0x80FFFFFF;
        quadTm3Ptr->specular = 0xFF000000;
        quadTm3Ptr->u = curUStartScalar + uSpan;
        quadTm3Ptr->v = vStart + vSum;
        *g_CreditsFadeOverlayBatch.backdropB->colorSignedBytePtr = 0;
        g_CreditsFadeOverlayBatch.backdropB->texturePage = 0;
    }
    if (AcquireBatchNode(1, (void *)g_CreditsFadeOverlayBatch.partA) != 0) {
        trCorner0 = g_CreditsFadeOverlayBatch.backdropPartA->vertices;
        trCorner0->xBits = FLOAT_512_BITS;
        trCorner0->yBits = 0;
        trCorner0->zBits = depth;
        trCorner0->rhwBits = FLOAT_ONE_BITS;
        trCorner0->color = 0x80FFFFFF;
        trCorner0->specular = 0xFF000000;
        trCorner0->u = curUStartScalar;
        trCorner0->v = vStart;
        trVtxPtr = g_CreditsFadeOverlayBatch.backdropPartA->vertices + 1;
        trVtxPtr->xBits = FLOAT_512_BITS;
        trVtxPtr->yBits = FLOAT_256_BITS;
        trVtxPtr->zBits = depth;
        trVtxPtr->rhwBits = FLOAT_ONE_BITS;
        trVtxPtr->color = 0x80FFFFFF;
        trVtxPtr->specular = 0xFF000000;
        trVtxPtr->u = curUStartScalar;
        trVtxPtr->v = vStart + vSum;
        rawPTrVertex = g_CreditsFadeOverlayBatch.backdropPartA->vertices + 2;
        rawPTrVertex->xBits = FLOAT_640_BITS;
        rawPTrVertex->yBits = 0;
        rawPTrVertex->zBits = depth;
        rawPTrVertex->rhwBits = FLOAT_ONE_BITS;
        rawPTrVertex->color = 0x80FFFFFF;
        rawPTrVertex->specular = 0xFF000000;
        rawPTrVertex->u = curUStartScalar + uSpan;
        rawPTrVertex->v = vStart;
        trVert = g_CreditsFadeOverlayBatch.backdropPartA->vertices + 3;
        trVert->xBits = FLOAT_640_BITS;
        trVert->yBits = FLOAT_256_BITS;
        trVert->zBits = depth;
        trVert->rhwBits = FLOAT_ONE_BITS;
        trVert->color = 0x80FFFFFF;
        trVert->specular = 0xFF000000;
        trVert->u = curUStartScalar + uSpan;
        trVert->v = vStart + vSum;
        *g_CreditsFadeOverlayBatch.backdropPartA->colorSignedBytePtr = 0;
        g_CreditsFadeOverlayBatch.backdropPartA->texturePage = 0;
    }
    vSum = 0.875f;
    if (AcquireBatchNode(1, (void *)g_CreditsFadeOverlayBatch.partB) != 0) {
        vertexBl0Ptr = g_CreditsFadeOverlayBatch.backdropPartB->vertices;
        vertexBl0Ptr->xBits = 0;
        vertexBl0Ptr->yBits = FLOAT_256_BITS;
        vertexBl0Ptr->zBits = depth;
        vertexBl0Ptr->rhwBits = FLOAT_ONE_BITS;
        vertexBl0Ptr->color = 0x80FFFFFF;
        vertexBl0Ptr->specular = 0xFF000000;
        vertexBl0Ptr->u = curUStartScalar;
        vertexBl0Ptr->v = vStart;
        blVertex1Ptr = g_CreditsFadeOverlayBatch.backdropPartB->vertices + 1;
        blVertex1Ptr->xBits = 0;
        blVertex1Ptr->yBits = FLOAT_480_BITS;
        blVertex1Ptr->zBits = depth;
        blVertex1Ptr->rhwBits = FLOAT_ONE_BITS;
        blVertex1Ptr->color = 0x80FFFFFF;
        blVertex1Ptr->specular = 0xFF000000;
        blVertex1Ptr->u = curUStartScalar;
        blVertex1Ptr->v = vStart + vSum;
        pickedVertexBl = g_CreditsFadeOverlayBatch.backdropPartB->vertices + 2;
        pickedVertexBl->xBits = FLOAT_256_BITS;
        pickedVertexBl->yBits = FLOAT_256_BITS;
        pickedVertexBl->zBits = depth;
        pickedVertexBl->rhwBits = FLOAT_ONE_BITS;
        pickedVertexBl->color = 0x80FFFFFF;
        pickedVertexBl->specular = 0xFF000000;
        pickedVertexBl->u = curUStartScalar + uSpan;
        pickedVertexBl->v = vStart;
        blLowLeftPtr = g_CreditsFadeOverlayBatch.backdropPartB->vertices + 3;
        blLowLeftPtr->xBits = FLOAT_256_BITS;
        blLowLeftPtr->yBits = FLOAT_480_BITS;
        blLowLeftPtr->zBits = depth;
        blLowLeftPtr->rhwBits = FLOAT_ONE_BITS;
        blLowLeftPtr->color = 0x80FFFFFF;
        blLowLeftPtr->specular = 0xFF000000;
        blLowLeftPtr->u = curUStartScalar + uSpan;
        blLowLeftPtr->v = vStart + vSum;
        *g_CreditsFadeOverlayBatch.backdropPartB->colorSignedBytePtr = 0;
        g_CreditsFadeOverlayBatch.backdropPartB->texturePage = 0;
    }
    if (AcquireBatchNode(1, (void *)g_CreditsFadeOverlayBatch.partC) != 0) {
        bmVertex0 = g_CreditsFadeOverlayBatch.backdropPartC->vertices;
        bmVertex0->xBits = FLOAT_256_BITS;
        bmVertex0->yBits = FLOAT_256_BITS;
        bmVertex0->zBits = depth;
        bmVertex0->rhwBits = FLOAT_ONE_BITS;
        bmVertex0->color = 0x80FFFFFF;
        bmVertex0->specular = 0xFF000000;
        bmVertex0->u = curUStartScalar;
        bmVertex0->v = vStart;
        bmCorner1Ptr = g_CreditsFadeOverlayBatch.backdropPartC->vertices + 1;
        bmCorner1Ptr->xBits = FLOAT_256_BITS;
        bmCorner1Ptr->yBits = FLOAT_480_BITS;
        bmCorner1Ptr->zBits = depth;
        bmCorner1Ptr->rhwBits = FLOAT_ONE_BITS;
        bmCorner1Ptr->color = 0x80FFFFFF;
        bmCorner1Ptr->specular = 0xFF000000;
        bmCorner1Ptr->u = curUStartScalar;
        bmCorner1Ptr->v = vStart + vSum;
        bmVertex2 = g_CreditsFadeOverlayBatch.backdropPartC->vertices + 2;
        bmVertex2->xBits = FLOAT_512_BITS;
        bmVertex2->yBits = FLOAT_256_BITS;
        bmVertex2->zBits = depth;
        bmVertex2->rhwBits = FLOAT_ONE_BITS;
        bmVertex2->color = 0x80FFFFFF;
        bmVertex2->specular = 0xFF000000;
        bmVertex2->u = curUStartScalar + uSpan;
        bmVertex2->v = vStart;
        stageVertexBm = g_CreditsFadeOverlayBatch.backdropPartC->vertices + 3;
        stageVertexBm->xBits = FLOAT_512_BITS;
        stageVertexBm->yBits = FLOAT_480_BITS;
        stageVertexBm->zBits = depth;
        stageVertexBm->rhwBits = FLOAT_ONE_BITS;
        stageVertexBm->color = 0x80FFFFFF;
        stageVertexBm->specular = 0xFF000000;
        stageVertexBm->u = curUStartScalar + uSpan;
        stageVertexBm->v = vStart + vSum;
        *g_CreditsFadeOverlayBatch.backdropPartC->colorSignedBytePtr = 0;
        g_CreditsFadeOverlayBatch.backdropPartC->texturePage = 0;
    }
    if (AcquireBatchNode(1, (void *)g_CreditsFadeOverlayBatch.partD) != 0) {
        quadBr0 = g_CreditsFadeOverlayBatch.backdropPartD->vertices;
        quadBr0->xBits = FLOAT_512_BITS;
        quadBr0->yBits = FLOAT_256_BITS;
        quadBr0->zBits = depth;
        quadBr0->rhwBits = FLOAT_ONE_BITS;
        quadBr0->color = 0x80FFFFFF;
        quadBr0->specular = 0xFF000000;
        quadBr0->u = curUStartScalar;
        quadBr0->v = vStart;
        brVertex1 = g_CreditsFadeOverlayBatch.backdropPartD->vertices + 1;
        brVertex1->xBits = FLOAT_512_BITS;
        brVertex1->yBits = FLOAT_480_BITS;
        brVertex1->zBits = depth;
        brVertex1->rhwBits = FLOAT_ONE_BITS;
        brVertex1->color = 0x80FFFFFF;
        brVertex1->specular = 0xFF000000;
        brVertex1->u = curUStartScalar;
        brVertex1->v = vStart + vSum;
        brVertex2 = g_CreditsFadeOverlayBatch.backdropPartD->vertices + 2;
        brVertex2->xBits = FLOAT_640_BITS;
        brVertex2->yBits = FLOAT_256_BITS;
        brVertex2->zBits = depth;
        brVertex2->rhwBits = FLOAT_ONE_BITS;
        brVertex2->color = 0x80FFFFFF;
        brVertex2->specular = 0xFF000000;
        brVertex2->u = curUStartScalar + uSpan;
        brVertex2->v = vStart;
        vertexBr3Ptr = g_CreditsFadeOverlayBatch.backdropPartD->vertices + 3;
        vertexBr3Ptr->xBits = FLOAT_640_BITS;
        vertexBr3Ptr->yBits = FLOAT_480_BITS;
        vertexBr3Ptr->zBits = depth;
        vertexBr3Ptr->rhwBits = FLOAT_ONE_BITS;
        vertexBr3Ptr->color = 0x80FFFFFF;
        vertexBr3Ptr->specular = 0xFF000000;
        vertexBr3Ptr->u = curUStartScalar + uSpan;
        vertexBr3Ptr->v = vStart + vSum;
        *g_CreditsFadeOverlayBatch.backdropPartD->colorSignedBytePtr = 0;
        g_CreditsFadeOverlayBatch.backdropPartD->texturePage = 0;
    }
}

/*
 * Acquire texture batches and draw the two background quads covering a 320x240 resolution display.
 */
// FUNCTION: FF7 0x007ab469
void __cdecl Credits_DrawBackground1x(void)
{
    float uCoord;
    int originX;
    float texW;
    GfxTlVertex *pLeftTopLeft;
    GfxTlVertex *leftTR;
    GfxTlVertex *remainingBottomLeft;
    GfxTlVertex *pRightTopLeft;
    GfxTlVertex *leftBottomRight;
    GfxTlVertex *rightBR;
    GfxTlVertex *tmpPRightVert;
    GfxTlVertex *pRightBL;
    float textureV;
    int originY;
    int zValue;
    float texH;

    zValue = FLOAT_0_2_BITS;
    originX = g_CreditsViewportX;
    originY = g_CreditsViewportY;
    textureV = 0.0f;
    uCoord = textureV;
    texW = 1.0f;
    texH = 0.9375f;
    if (AcquireBatchNode(1, (void *)g_CreditsFadeOverlayBatch.backdropSlotA) != 0) {
        pLeftTopLeft = g_CreditsFadeOverlayBatch.backdropA->vertices;
        pLeftTopLeft->x = originX;
        pLeftTopLeft->y = originY;
        pLeftTopLeft->zBits = zValue;
        pLeftTopLeft->rhwBits = FLOAT_ONE_BITS;
        pLeftTopLeft->color = 0x80FFFFFF;
        pLeftTopLeft->specular = 0xFF000000;
        pLeftTopLeft->u = uCoord;
        pLeftTopLeft->v = textureV;
        remainingBottomLeft = g_CreditsFadeOverlayBatch.backdropA->vertices + 1;
        remainingBottomLeft->x = originX;
        remainingBottomLeft->y = originY + 2.4e+02f;
        remainingBottomLeft->zBits = zValue;
        remainingBottomLeft->rhwBits = FLOAT_ONE_BITS;
        remainingBottomLeft->color = 0x80FFFFFF;
        remainingBottomLeft->specular = 0xFF000000;
        remainingBottomLeft->u = uCoord;
        remainingBottomLeft->v = textureV + texH;
        leftTR = g_CreditsFadeOverlayBatch.backdropA->vertices + 2;
        leftTR->x = originX + 256.0f;
        leftTR->y = originY;
        leftTR->zBits = zValue;
        leftTR->rhwBits = FLOAT_ONE_BITS;
        leftTR->color = 0x80FFFFFF;
        leftTR->specular = 0xFF000000;
        leftTR->u = uCoord + texW;
        leftTR->v = textureV;
        leftBottomRight = g_CreditsFadeOverlayBatch.backdropA->vertices + 3;
        leftBottomRight->x = originX + 256.0f;
        leftBottomRight->y = originY + 2.4e+02f;
        leftBottomRight->zBits = zValue;
        leftBottomRight->rhwBits = FLOAT_ONE_BITS;
        leftBottomRight->color = 0x80FFFFFF;
        leftBottomRight->specular = 0xFF000000;
        leftBottomRight->u = uCoord + texW;
        leftBottomRight->v = textureV + texH;
        *g_CreditsFadeOverlayBatch.backdropA->colorSignedBytePtr = 0;
        g_CreditsFadeOverlayBatch.backdropA->texturePage = 0;
    }
    originX += 0x100;
    if (AcquireBatchNode(1, (void *)g_CreditsFadeOverlayBatch.packetSlotB) != 0) {
        pRightTopLeft = g_CreditsFadeOverlayBatch.backdropB->vertices;
        pRightTopLeft->x = originX;
        pRightTopLeft->y = originY;
        pRightTopLeft->zBits = zValue;
        pRightTopLeft->rhwBits = FLOAT_ONE_BITS;
        pRightTopLeft->color = 0x80FFFFFF;
        pRightTopLeft->specular = 0xFF000000;
        pRightTopLeft->u = uCoord;
        pRightTopLeft->v = textureV;
        pRightBL = g_CreditsFadeOverlayBatch.backdropB->vertices + 1;
        pRightBL->x = originX;
        pRightBL->y = originY + 2.4e+02f;
        pRightBL->zBits = zValue;
        pRightBL->rhwBits = FLOAT_ONE_BITS;
        pRightBL->color = 0x80FFFFFF;
        pRightBL->specular = 0xFF000000;
        pRightBL->u = uCoord;
        pRightBL->v = textureV + texH;
        tmpPRightVert = g_CreditsFadeOverlayBatch.backdropB->vertices + 2;
        tmpPRightVert->x = originX + 64.0f;
        tmpPRightVert->y = originY;
        tmpPRightVert->zBits = zValue;
        tmpPRightVert->rhwBits = FLOAT_ONE_BITS;
        tmpPRightVert->color = 0x80FFFFFF;
        tmpPRightVert->specular = 0xFF000000;
        tmpPRightVert->u = uCoord + texW;
        tmpPRightVert->v = textureV;
        rightBR = g_CreditsFadeOverlayBatch.backdropB->vertices + 3;
        rightBR->x = originX + 64.0f;
        rightBR->y = originY + 2.4e+02f;
        rightBR->zBits = zValue;
        rightBR->rhwBits = FLOAT_ONE_BITS;
        rightBR->color = 0x80FFFFFF;
        rightBR->specular = 0xFF000000;
        rightBR->u = uCoord + texW;
        rightBR->v = textureV + texH;
        *g_CreditsFadeOverlayBatch.backdropB->colorSignedBytePtr = 0;
        g_CreditsFadeOverlayBatch.backdropB->texturePage = 0;
    }
}

/*
 * Draw the multi-part FF7 logo quads with the specified alpha transparency in 640x480 mode.
 */
// FUNCTION: FF7 0x007ab81c
void __cdecl Credits_DrawLogoWithAlpha2x(char alpha)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[104];
        GfxTlVertex *logo2xBottomRight;
        struct { unsigned char pad_to_4[0x4]; GfxTlVertex *logo2xTopRight; };
        struct { unsigned char pad_to_8[0x8]; GfxTlVertex *logo2xBottomLeft; };
        struct { unsigned char pad_to_C[0xC]; GfxTlVertex *logo2xTopLeft; };
        struct { unsigned char pad_to_10[0x10]; GfxTlVertex *scrollEdgeBottom; };
        struct { unsigned char pad_to_14[0x14]; GfxTlVertex *scrollEdgeTop; };
        struct { unsigned char pad_to_18[0x18]; GfxTlVertex *scrollMidBottom; };
        struct { unsigned char pad_to_1C[0x1C]; GfxTlVertex *scrollMidTop; };
        struct { unsigned char pad_to_20[0x20]; GfxTlVertex *scrollRightBottom; };
        struct { unsigned char pad_to_24[0x24]; GfxTlVertex *scrollRightTop; };
        struct { unsigned char pad_to_28[0x28]; GfxTlVertex *scrollLeftBottom; };
        struct { unsigned char pad_to_2C[0x2C]; GfxTlVertex *scrollLeftTop; };
        struct { unsigned char pad_to_30[0x30]; GfxTlVertex *flagE8BottomRight; };
        struct { unsigned char pad_to_34[0x34]; GfxTlVertex *flagE8TopRight; };
        struct { unsigned char pad_to_38[0x38]; GfxTlVertex *flagE8BottomLeft; };
        struct { unsigned char pad_to_3C[0x3C]; GfxTlVertex *flagE8TopLeft; };
        struct { unsigned char pad_to_40[0x40]; GfxTlVertex *batchCBottomRight; };
        struct { unsigned char pad_to_44[0x44]; GfxTlVertex *batchCTopRight; };
        struct { unsigned char pad_to_48[0x48]; GfxTlVertex *batchCBottomLeft; };
        struct { unsigned char pad_to_4C[0x4C]; GfxTlVertex *batchCTopLeft; };
        struct { unsigned char pad_to_50[0x50]; float texStepV; };
        struct { unsigned char pad_to_54[0x54]; int vertexColor; };
        struct { unsigned char pad_to_57[0x57]; char logoAlpha; };
        struct { unsigned char pad_to_58[0x58]; float texStepU; };
        struct { unsigned char pad_to_5C[0x5C]; int vertexDepth; };
        struct { unsigned char pad_to_60[0x60]; float texBaseV; };
        struct { unsigned char pad_to_64[0x64]; float texBaseU; };
    } frame;
#pragma pack(pop)

    frame.vertexDepth = FLOAT_0_1_BITS;
    frame.texBaseV = 0.0f;
    frame.texBaseU = frame.texBaseV;
    frame.texStepV = 1.0f;
    frame.texStepU = frame.texStepV;
    frame.vertexColor = 0xFFFFFFFF;
    frame.logoAlpha = alpha;
    if (AcquireBatchNode(1, g_CreditsLogoQuadBatches) != 0) {
        frame.batchCTopLeft = g_CreditsLogoQuadBatches->vertices;
        frame.batchCTopLeft->xBits = 0;
        frame.batchCTopLeft->yBits = FLOAT_20_BITS;
        frame.batchCTopLeft->zBits = frame.vertexDepth;
        frame.batchCTopLeft->rhwBits = FLOAT_ONE_BITS;
        frame.batchCTopLeft->color = frame.vertexColor;
        frame.batchCTopLeft->specular = 0xFF000000;
        frame.batchCTopLeft->u = frame.texBaseU;
        frame.batchCTopLeft->v = frame.texBaseV;
        frame.batchCBottomLeft = g_CreditsLogoQuadBatches->vertices + 1;
        frame.batchCBottomLeft->xBits = 0;
        frame.batchCBottomLeft->yBits = FLOAT_276_BITS;
        frame.batchCBottomLeft->zBits = frame.vertexDepth;
        frame.batchCBottomLeft->rhwBits = FLOAT_ONE_BITS;
        frame.batchCBottomLeft->color = frame.vertexColor;
        frame.batchCBottomLeft->specular = 0xFF000000;
        frame.batchCBottomLeft->u = frame.texBaseU;
        frame.batchCBottomLeft->v = frame.texBaseV + frame.texStepV;
        frame.batchCTopRight = g_CreditsLogoQuadBatches->vertices + 2;
        frame.batchCTopRight->xBits = FLOAT_256_BITS;
        frame.batchCTopRight->yBits = FLOAT_20_BITS;
        frame.batchCTopRight->zBits = frame.vertexDepth;
        frame.batchCTopRight->rhwBits = FLOAT_ONE_BITS;
        frame.batchCTopRight->color = frame.vertexColor;
        frame.batchCTopRight->specular = 0xFF000000;
        frame.batchCTopRight->u = frame.texBaseU + frame.texStepU;
        frame.batchCTopRight->v = frame.texBaseV;
        frame.batchCBottomRight = g_CreditsLogoQuadBatches->vertices + 3;
        frame.batchCBottomRight->xBits = FLOAT_256_BITS;
        frame.batchCBottomRight->yBits = FLOAT_276_BITS;
        frame.batchCBottomRight->zBits = frame.vertexDepth;
        frame.batchCBottomRight->rhwBits = FLOAT_ONE_BITS;
        frame.batchCBottomRight->color = frame.vertexColor;
        frame.batchCBottomRight->specular = 0xFF000000;
        frame.batchCBottomRight->u = frame.texBaseU + frame.texStepU;
        frame.batchCBottomRight->v = frame.texBaseV + frame.texStepV;
        *g_CreditsLogoQuadBatches->colorSignedBytePtr = 0;
        g_CreditsLogoQuadBatches->texturePage = 0;
    }
    if (AcquireBatchNode(1, g_CreditsUnusedFlagE8) != 0) {
        frame.flagE8TopLeft = g_CreditsUnusedFlagE8->vertices;
        frame.flagE8TopLeft->xBits = FLOAT_256_BITS;
        frame.flagE8TopLeft->yBits = FLOAT_20_BITS;
        frame.flagE8TopLeft->zBits = frame.vertexDepth;
        frame.flagE8TopLeft->rhwBits = FLOAT_ONE_BITS;
        frame.flagE8TopLeft->color = frame.vertexColor;
        frame.flagE8TopLeft->specular = 0xFF000000;
        frame.flagE8TopLeft->u = frame.texBaseU;
        frame.flagE8TopLeft->v = frame.texBaseV;
        frame.flagE8BottomLeft = g_CreditsUnusedFlagE8->vertices + 1;
        frame.flagE8BottomLeft->xBits = FLOAT_256_BITS;
        frame.flagE8BottomLeft->yBits = FLOAT_276_BITS;
        frame.flagE8BottomLeft->zBits = frame.vertexDepth;
        frame.flagE8BottomLeft->rhwBits = FLOAT_ONE_BITS;
        frame.flagE8BottomLeft->color = frame.vertexColor;
        frame.flagE8BottomLeft->specular = 0xFF000000;
        frame.flagE8BottomLeft->u = frame.texBaseU;
        frame.flagE8BottomLeft->v = frame.texBaseV + frame.texStepV;
        frame.flagE8TopRight = g_CreditsUnusedFlagE8->vertices + 2;
        frame.flagE8TopRight->xBits = FLOAT_512_BITS;
        frame.flagE8TopRight->yBits = FLOAT_20_BITS;
        frame.flagE8TopRight->zBits = frame.vertexDepth;
        frame.flagE8TopRight->rhwBits = FLOAT_ONE_BITS;
        frame.flagE8TopRight->color = frame.vertexColor;
        frame.flagE8TopRight->specular = 0xFF000000;
        frame.flagE8TopRight->u = frame.texBaseU + frame.texStepU;
        frame.flagE8TopRight->v = frame.texBaseV;
        frame.flagE8BottomRight = g_CreditsUnusedFlagE8->vertices + 3;
        frame.flagE8BottomRight->xBits = FLOAT_512_BITS;
        frame.flagE8BottomRight->yBits = FLOAT_276_BITS;
        frame.flagE8BottomRight->zBits = frame.vertexDepth;
        frame.flagE8BottomRight->rhwBits = FLOAT_ONE_BITS;
        frame.flagE8BottomRight->color = frame.vertexColor;
        frame.flagE8BottomRight->specular = 0xFF000000;
        frame.flagE8BottomRight->u = frame.texBaseU + frame.texStepU;
        frame.flagE8BottomRight->v = frame.texBaseV + frame.texStepV;
        *g_CreditsUnusedFlagE8->colorSignedBytePtr = 0;
        g_CreditsUnusedFlagE8->texturePage = 0;
    }
    if (AcquireBatchNode(2, g_CreditsLogoDrawState) != 0) {
        frame.texStepU = 0.5f;
        frame.scrollLeftTop = g_CreditsLogoDrawState->vertices;
        frame.scrollLeftTop->xBits = FLOAT_512_BITS;
        frame.scrollLeftTop->yBits = FLOAT_20_BITS;
        frame.scrollLeftTop->zBits = frame.vertexDepth;
        frame.scrollLeftTop->rhwBits = FLOAT_ONE_BITS;
        frame.scrollLeftTop->color = frame.vertexColor;
        frame.scrollLeftTop->specular = 0xFF000000;
        frame.scrollLeftTop->u = frame.texBaseU;
        frame.scrollLeftTop->v = frame.texBaseV;
        frame.scrollLeftBottom = g_CreditsLogoDrawState->vertices + 1;
        frame.scrollLeftBottom->xBits = FLOAT_512_BITS;
        frame.scrollLeftBottom->yBits = FLOAT_276_BITS;
        frame.scrollLeftBottom->zBits = frame.vertexDepth;
        frame.scrollLeftBottom->rhwBits = FLOAT_ONE_BITS;
        frame.scrollLeftBottom->color = frame.vertexColor;
        frame.scrollLeftBottom->specular = 0xFF000000;
        frame.scrollLeftBottom->u = frame.texBaseU;
        frame.scrollLeftBottom->v = frame.texBaseV + frame.texStepV;
        frame.scrollRightTop = g_CreditsLogoDrawState->vertices + 2;
        frame.scrollRightTop->xBits = FLOAT_640_BITS;
        frame.scrollRightTop->yBits = FLOAT_20_BITS;
        frame.scrollRightTop->zBits = frame.vertexDepth;
        frame.scrollRightTop->rhwBits = FLOAT_ONE_BITS;
        frame.scrollRightTop->color = frame.vertexColor;
        frame.scrollRightTop->specular = 0xFF000000;
        frame.scrollRightTop->u = frame.texBaseU + frame.texStepU;
        frame.scrollRightTop->v = frame.texBaseV;
        frame.scrollRightBottom = g_CreditsLogoDrawState->vertices + 3;
        frame.scrollRightBottom->xBits = FLOAT_640_BITS;
        frame.scrollRightBottom->yBits = FLOAT_276_BITS;
        frame.scrollRightBottom->zBits = frame.vertexDepth;
        frame.scrollRightBottom->rhwBits = FLOAT_ONE_BITS;
        frame.scrollRightBottom->color = frame.vertexColor;
        frame.scrollRightBottom->specular = 0xFF000000;
        frame.scrollRightBottom->u = frame.texBaseU + frame.texStepU;
        frame.scrollRightBottom->v = frame.texBaseV + frame.texStepV;
        *g_CreditsLogoDrawState->colorSignedBytePtr = 0;
        g_CreditsLogoDrawState->texturePage = 0;
        g_CreditsLogoDrawState->vertexCursor += g_CreditsLogoDrawState->cursorStep;
        (g_CreditsLogoDrawState->colorSignedBytePtr)++;
        frame.texBaseU = 0.5f;
        frame.texStepV = 0.25f;
        frame.scrollMidTop = g_CreditsLogoDrawState->vertices;
        frame.scrollMidTop->xBits = FLOAT_128_BITS;
        frame.scrollMidTop->yBits = FLOAT_276_BITS;
        frame.scrollMidTop->zBits = frame.vertexDepth;
        frame.scrollMidTop->rhwBits = FLOAT_ONE_BITS;
        frame.scrollMidTop->color = frame.vertexColor;
        frame.scrollMidTop->specular = 0xFF000000;
        frame.scrollMidTop->u = frame.texBaseU;
        frame.scrollMidTop->v = frame.texBaseV;
        frame.scrollMidBottom = g_CreditsLogoDrawState->vertices + 1;
        frame.scrollMidBottom->xBits = FLOAT_128_BITS;
        frame.scrollMidBottom->yBits = FLOAT_340_BITS;
        frame.scrollMidBottom->zBits = frame.vertexDepth;
        frame.scrollMidBottom->rhwBits = FLOAT_ONE_BITS;
        frame.scrollMidBottom->color = frame.vertexColor;
        frame.scrollMidBottom->specular = 0xFF000000;
        frame.scrollMidBottom->u = frame.texBaseU;
        frame.scrollMidBottom->v = frame.texBaseV + frame.texStepV;
        frame.scrollEdgeTop = g_CreditsLogoDrawState->vertices + 2;
        frame.scrollEdgeTop->xBits = FLOAT_256_BITS;
        frame.scrollEdgeTop->yBits = FLOAT_276_BITS;
        frame.scrollEdgeTop->zBits = frame.vertexDepth;
        frame.scrollEdgeTop->rhwBits = FLOAT_ONE_BITS;
        frame.scrollEdgeTop->color = frame.vertexColor;
        frame.scrollEdgeTop->specular = 0xFF000000;
        frame.scrollEdgeTop->u = frame.texBaseU + frame.texStepU;
        frame.scrollEdgeTop->v = frame.texBaseV;
        frame.scrollEdgeBottom = g_CreditsLogoDrawState->vertices + 3;
        frame.scrollEdgeBottom->xBits = FLOAT_256_BITS;
        frame.scrollEdgeBottom->yBits = FLOAT_340_BITS;
        frame.scrollEdgeBottom->zBits = frame.vertexDepth;
        frame.scrollEdgeBottom->rhwBits = FLOAT_ONE_BITS;
        frame.scrollEdgeBottom->color = frame.vertexColor;
        frame.scrollEdgeBottom->specular = 0xFF000000;
        frame.scrollEdgeBottom->u = frame.texBaseU + frame.texStepU;
        frame.scrollEdgeBottom->v = frame.texBaseV + frame.texStepV;
        *g_CreditsLogoDrawState->colorSignedBytePtr = 0;
        g_CreditsLogoDrawState->texturePage = 0;
    }
    if (AcquireBatchNode(1, g_CreditsUnusedFlagF0) != 0) {
        frame.texBaseV = 0.0f;
        frame.texBaseU = frame.texBaseV;
        frame.texStepU = 0.25f;
        frame.logo2xTopLeft = g_CreditsUnusedFlagF0->vertices;
        frame.logo2xTopLeft->xBits = FLOAT_256_BITS;
        frame.logo2xTopLeft->yBits = FLOAT_276_BITS;
        frame.logo2xTopLeft->zBits = frame.vertexDepth;
        frame.logo2xTopLeft->rhwBits = FLOAT_ONE_BITS;
        frame.logo2xTopLeft->color = frame.vertexColor;
        frame.logo2xTopLeft->specular = 0xFF000000;
        frame.logo2xTopLeft->u = frame.texBaseU;
        frame.logo2xTopLeft->v = frame.texBaseV;
        frame.logo2xBottomLeft = g_CreditsUnusedFlagF0->vertices + 1;
        frame.logo2xBottomLeft->xBits = FLOAT_256_BITS;
        frame.logo2xBottomLeft->yBits = FLOAT_340_BITS;
        frame.logo2xBottomLeft->zBits = frame.vertexDepth;
        frame.logo2xBottomLeft->rhwBits = FLOAT_ONE_BITS;
        frame.logo2xBottomLeft->color = frame.vertexColor;
        frame.logo2xBottomLeft->specular = 0xFF000000;
        frame.logo2xBottomLeft->u = frame.texBaseU;
        frame.logo2xBottomLeft->v = frame.texBaseV + frame.texStepV;
        frame.logo2xTopRight = g_CreditsUnusedFlagF0->vertices + 2;
        frame.logo2xTopRight->xBits = FLOAT_320_BITS;
        frame.logo2xTopRight->yBits = FLOAT_276_BITS;
        frame.logo2xTopRight->zBits = frame.vertexDepth;
        frame.logo2xTopRight->rhwBits = FLOAT_ONE_BITS;
        frame.logo2xTopRight->color = frame.vertexColor;
        frame.logo2xTopRight->specular = 0xFF000000;
        frame.logo2xTopRight->u = frame.texBaseU + frame.texStepU;
        frame.logo2xTopRight->v = frame.texBaseV;
        frame.logo2xBottomRight = g_CreditsUnusedFlagF0->vertices + 3;
        frame.logo2xBottomRight->xBits = FLOAT_320_BITS;
        frame.logo2xBottomRight->yBits = FLOAT_340_BITS;
        frame.logo2xBottomRight->zBits = frame.vertexDepth;
        frame.logo2xBottomRight->rhwBits = FLOAT_ONE_BITS;
        frame.logo2xBottomRight->color = frame.vertexColor;
        frame.logo2xBottomRight->specular = 0xFF000000;
        frame.logo2xBottomRight->u = frame.texBaseU + frame.texStepU;
        frame.logo2xBottomRight->v = frame.texBaseV + frame.texStepV;
        *g_CreditsUnusedFlagF0->colorSignedBytePtr = 0;
        g_CreditsUnusedFlagF0->texturePage = 0;
    }
}

/*
 * Draw the three textured logo quads with the specified alpha transparency in 320x240 mode.
 */
// FUNCTION: FF7 0x007ac0b7
void __cdecl Credits_DrawLogoWithAlpha1x(char alpha)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[80];
        GfxTlVertex *quadCBottomRight;
        struct { unsigned char pad_to_4[0x4]; GfxTlVertex *quadCTopRight; };
        struct { unsigned char pad_to_8[0x8]; GfxTlVertex *quadCBottomLeft; };
        struct { unsigned char pad_to_C[0xC]; GfxTlVertex *quadCTopLeft; };
        struct { unsigned char pad_to_10[0x10]; GfxTlVertex *quadBBottomRight; };
        struct { unsigned char pad_to_14[0x14]; GfxTlVertex *quadBTopRight; };
        struct { unsigned char pad_to_18[0x18]; GfxTlVertex *quadBBottomLeft; };
        struct { unsigned char pad_to_1C[0x1C]; GfxTlVertex *quadBTopLeft; };
        struct { unsigned char pad_to_20[0x20]; GfxTlVertex *quadABottomRight; };
        struct { unsigned char pad_to_24[0x24]; GfxTlVertex *quadATopRight; };
        struct { unsigned char pad_to_28[0x28]; GfxTlVertex *quadABottomLeft; };
        struct { unsigned char pad_to_2C[0x2C]; GfxTlVertex *quadATopLeft; };
        struct { unsigned char pad_to_30[0x30]; float texStepV; };
        struct { unsigned char pad_to_34[0x34]; int vertexColor; };
        struct { unsigned char pad_to_37[0x37]; char logoAlpha; };
        struct { unsigned char pad_to_38[0x38]; float texStepU; };
        struct { unsigned char pad_to_3C[0x3C]; int vertexDepth; };
        struct { unsigned char pad_to_40[0x40]; int viewportY; };
        struct { unsigned char pad_to_44[0x44]; int viewportX; };
        struct { unsigned char pad_to_48[0x48]; float texBaseV; };
        struct { unsigned char pad_to_4C[0x4C]; float texBaseU; };
    } frame;
#pragma pack(pop)

    frame.vertexDepth = FLOAT_0_1_BITS;
    frame.viewportX = g_CreditsViewportX;
    frame.viewportY = g_CreditsViewportY;
    frame.texBaseV = 0.0f;
    frame.texBaseU = frame.texBaseV;
    frame.texStepU = 1.0f;
    frame.texStepV = 0.5f;
    frame.vertexColor = 0xFFFFFFFF;
    frame.logoAlpha = alpha;
    if (AcquireBatchNode(3, g_CreditsLogoQuadBatches) != 0) {
        frame.quadATopLeft = g_CreditsLogoQuadBatches->vertices;
        frame.quadATopLeft->x = (float)frame.viewportX;
        frame.quadATopLeft->y = (float)frame.viewportY + 1e+01f;
        frame.quadATopLeft->zBits = frame.vertexDepth;
        frame.quadATopLeft->rhwBits = FLOAT_ONE_BITS;
        frame.quadATopLeft->color = frame.vertexColor;
        frame.quadATopLeft->specular = 0xFF000000;
        frame.quadATopLeft->u = frame.texBaseU;
        frame.quadATopLeft->v = frame.texBaseV;
        frame.quadABottomLeft = g_CreditsLogoQuadBatches->vertices + 1;
        frame.quadABottomLeft->x = (float)frame.viewportX;
        frame.quadABottomLeft->y = (float)frame.viewportY + 1e+01f + 128.0f;
        frame.quadABottomLeft->zBits = frame.vertexDepth;
        frame.quadABottomLeft->rhwBits = FLOAT_ONE_BITS;
        frame.quadABottomLeft->color = frame.vertexColor;
        frame.quadABottomLeft->specular = 0xFF000000;
        frame.quadABottomLeft->u = frame.texBaseU;
        frame.quadABottomLeft->v = frame.texBaseV + frame.texStepV;
        frame.quadATopRight = g_CreditsLogoQuadBatches->vertices + 2;
        frame.quadATopRight->x = (float)frame.viewportX + 256.0f;
        frame.quadATopRight->y = (float)frame.viewportY + 1e+01f;
        frame.quadATopRight->zBits = frame.vertexDepth;
        frame.quadATopRight->rhwBits = FLOAT_ONE_BITS;
        frame.quadATopRight->color = frame.vertexColor;
        frame.quadATopRight->specular = 0xFF000000;
        frame.quadATopRight->u = frame.texBaseU + frame.texStepU;
        frame.quadATopRight->v = frame.texBaseV;
        frame.quadABottomRight = g_CreditsLogoQuadBatches->vertices + 3;
        frame.quadABottomRight->x = (float)frame.viewportX + 256.0f;
        frame.quadABottomRight->y = (float)frame.viewportY + 1e+01f + 128.0f;
        frame.quadABottomRight->zBits = frame.vertexDepth;
        frame.quadABottomRight->rhwBits = FLOAT_ONE_BITS;
        frame.quadABottomRight->color = frame.vertexColor;
        frame.quadABottomRight->specular = 0xFF000000;
        frame.quadABottomRight->u = frame.texBaseU + frame.texStepU;
        frame.quadABottomRight->v = frame.texBaseV + frame.texStepV;
        *g_CreditsLogoQuadBatches->colorSignedBytePtr = 0;
        g_CreditsLogoQuadBatches->texturePage = 0;
        g_CreditsLogoQuadBatches->vertexCursor += g_CreditsLogoQuadBatches->cursorStep;
        (g_CreditsLogoQuadBatches->colorSignedBytePtr)++;
        frame.texBaseV = 0.5f;
        frame.texStepU = 0.25f;
        frame.texStepV = 0.5f;
        frame.quadBTopLeft = g_CreditsLogoQuadBatches->vertices;
        frame.quadBTopLeft->x = (float)frame.viewportX + 256.0f;
        frame.quadBTopLeft->y = (float)frame.viewportY + 1e+01f;
        frame.quadBTopLeft->zBits = frame.vertexDepth;
        frame.quadBTopLeft->rhwBits = FLOAT_ONE_BITS;
        frame.quadBTopLeft->color = frame.vertexColor;
        frame.quadBTopLeft->specular = 0xFF000000;
        frame.quadBTopLeft->u = frame.texBaseU;
        frame.quadBTopLeft->v = frame.texBaseV;
        frame.quadBBottomLeft = g_CreditsLogoQuadBatches->vertices + 1;
        frame.quadBBottomLeft->x = (float)frame.viewportX + 256.0f;
        frame.quadBBottomLeft->y = (float)frame.viewportY + 1e+01f + 128.0f;
        frame.quadBBottomLeft->zBits = frame.vertexDepth;
        frame.quadBBottomLeft->rhwBits = FLOAT_ONE_BITS;
        frame.quadBBottomLeft->color = frame.vertexColor;
        frame.quadBBottomLeft->specular = 0xFF000000;
        frame.quadBBottomLeft->u = frame.texBaseU;
        frame.quadBBottomLeft->v = frame.texBaseV + frame.texStepV;
        frame.quadBTopRight = g_CreditsLogoQuadBatches->vertices + 2;
        frame.quadBTopRight->x = (float)frame.viewportX + 256.0f + 64.0f;
        frame.quadBTopRight->y = (float)frame.viewportY + 1e+01f;
        frame.quadBTopRight->zBits = frame.vertexDepth;
        frame.quadBTopRight->rhwBits = FLOAT_ONE_BITS;
        frame.quadBTopRight->color = frame.vertexColor;
        frame.quadBTopRight->specular = 0xFF000000;
        frame.quadBTopRight->u = frame.texBaseU + frame.texStepU;
        frame.quadBTopRight->v = frame.texBaseV;
        frame.quadBBottomRight = g_CreditsLogoQuadBatches->vertices + 3;
        frame.quadBBottomRight->x = (float)frame.viewportX + 256.0f + 64.0f;
        frame.quadBBottomRight->y = (float)frame.viewportY + 1e+01f + 128.0f;
        frame.quadBBottomRight->zBits = frame.vertexDepth;
        frame.quadBBottomRight->rhwBits = FLOAT_ONE_BITS;
        frame.quadBBottomRight->color = frame.vertexColor;
        frame.quadBBottomRight->specular = 0xFF000000;
        frame.quadBBottomRight->u = frame.texBaseU + frame.texStepU;
        frame.quadBBottomRight->v = frame.texBaseV + frame.texStepV;
        *g_CreditsLogoQuadBatches->colorSignedBytePtr = 0;
        g_CreditsLogoQuadBatches->texturePage = 0;
        g_CreditsLogoQuadBatches->vertexCursor += g_CreditsLogoQuadBatches->cursorStep;
        (g_CreditsLogoQuadBatches->colorSignedBytePtr)++;
        frame.texBaseU = 0.25f;
        frame.texStepU = 0.375f;
        frame.texStepV = 0.125f;
        frame.quadCTopLeft = g_CreditsLogoQuadBatches->vertices;
        frame.quadCTopLeft->x = (float)frame.viewportX + 64.0f;
        frame.quadCTopLeft->y = (float)frame.viewportY + 1e+01f + 128.0f;
        frame.quadCTopLeft->zBits = frame.vertexDepth;
        frame.quadCTopLeft->rhwBits = FLOAT_ONE_BITS;
        frame.quadCTopLeft->color = frame.vertexColor;
        frame.quadCTopLeft->specular = 0xFF000000;
        frame.quadCTopLeft->u = frame.texBaseU;
        frame.quadCTopLeft->v = frame.texBaseV;
        frame.quadCBottomLeft = g_CreditsLogoQuadBatches->vertices + 1;
        frame.quadCBottomLeft->x = (float)frame.viewportX + 64.0f;
        frame.quadCBottomLeft->y = (float)frame.viewportY + 1e+01f + 128.0f + 32.0f;
        frame.quadCBottomLeft->zBits = frame.vertexDepth;
        frame.quadCBottomLeft->rhwBits = FLOAT_ONE_BITS;
        frame.quadCBottomLeft->color = frame.vertexColor;
        frame.quadCBottomLeft->specular = 0xFF000000;
        frame.quadCBottomLeft->u = frame.texBaseU;
        frame.quadCBottomLeft->v = frame.texBaseV + frame.texStepV;
        frame.quadCTopRight = g_CreditsLogoQuadBatches->vertices + 2;
        frame.quadCTopRight->x = (float)frame.viewportX + 64.0f + 96.0f;
        frame.quadCTopRight->y = (float)frame.viewportY + 1e+01f + 128.0f;
        frame.quadCTopRight->zBits = frame.vertexDepth;
        frame.quadCTopRight->rhwBits = FLOAT_ONE_BITS;
        frame.quadCTopRight->color = frame.vertexColor;
        frame.quadCTopRight->specular = 0xFF000000;
        frame.quadCTopRight->u = frame.texBaseU + frame.texStepU;
        frame.quadCTopRight->v = frame.texBaseV;
        frame.quadCBottomRight = g_CreditsLogoQuadBatches->vertices + 3;
        frame.quadCBottomRight->x = (float)frame.viewportX + 64.0f + 96.0f;
        frame.quadCBottomRight->y = (float)frame.viewportY + 1e+01f + 128.0f + 32.0f;
        frame.quadCBottomRight->zBits = frame.vertexDepth;
        frame.quadCBottomRight->rhwBits = FLOAT_ONE_BITS;
        frame.quadCBottomRight->color = frame.vertexColor;
        frame.quadCBottomRight->specular = 0xFF000000;
        frame.quadCBottomRight->u = frame.texBaseU + frame.texStepU;
        frame.quadCBottomRight->v = frame.texBaseV + frame.texStepV;
        *g_CreditsLogoQuadBatches->colorSignedBytePtr = 0;
        g_CreditsLogoQuadBatches->texturePage = 0;
    }
}

/*
 * Draw two adjacent atlas quads forming the lower credits logo strip in 640x480 mode.
 */
// FUNCTION: FF7 0x007ac6f8
void __cdecl Credits_DrawLogoStrip2x(void)
{
    float texU;
    float texW;
    GfxTlVertex *pLeftTopLeft;
    float texH;
    GfxTlVertex *leftTR;
    GfxTlVertex *remainingBottomLeft;
    GfxTlVertex *pRightTopLeft;
    GfxTlVertex *leftBottomRight;
    GfxTlVertex *rightBR;
    GfxTlVertex *tmpPRightVert;
    GfxTlVertex *pRightBL;
    float texV;
    int zValue;

    zValue = FLOAT_0_1_BITS;
    if (AcquireBatchNode(1, g_CreditsLogoDrawState) != 0) {
        texU = 0.5f;
        texV = 0.375f;
        texW = 0.5f;
        texH = 0.125f;
        pLeftTopLeft = g_CreditsLogoDrawState->vertices;
        pLeftTopLeft->xBits = FLOAT_160_BITS;
        pLeftTopLeft->yBits = FLOAT_408_BITS;
        pLeftTopLeft->zBits = zValue;
        pLeftTopLeft->rhwBits = FLOAT_ONE_BITS;
        pLeftTopLeft->color = 0xFFFFFFFF;
        pLeftTopLeft->specular = 0xFF000000;
        pLeftTopLeft->u = texU;
        pLeftTopLeft->v = texV;
        remainingBottomLeft = g_CreditsLogoDrawState->vertices + 1;
        remainingBottomLeft->xBits = FLOAT_160_BITS;
        remainingBottomLeft->yBits = FLOAT_440_BITS;
        remainingBottomLeft->zBits = zValue;
        remainingBottomLeft->rhwBits = FLOAT_ONE_BITS;
        remainingBottomLeft->color = 0xFFFFFFFF;
        remainingBottomLeft->specular = 0xFF000000;
        remainingBottomLeft->u = texU;
        remainingBottomLeft->v = texV + texH;
        leftTR = g_CreditsLogoDrawState->vertices + 2;
        leftTR->xBits = FLOAT_288_BITS;
        leftTR->yBits = FLOAT_408_BITS;
        leftTR->zBits = zValue;
        leftTR->rhwBits = FLOAT_ONE_BITS;
        leftTR->color = 0xFFFFFFFF;
        leftTR->specular = 0xFF000000;
        leftTR->u = texU + texW;
        leftTR->v = texV;
        leftBottomRight = g_CreditsLogoDrawState->vertices + 3;
        leftBottomRight->xBits = FLOAT_288_BITS;
        leftBottomRight->yBits = FLOAT_440_BITS;
        leftBottomRight->zBits = zValue;
        leftBottomRight->rhwBits = FLOAT_ONE_BITS;
        leftBottomRight->color = 0xFFFFFFFF;
        leftBottomRight->specular = 0xFF000000;
        leftBottomRight->u = texU + texW;
        leftBottomRight->v = texV + texH;
        *g_CreditsLogoDrawState->colorSignedBytePtr = 0;
        g_CreditsLogoDrawState->texturePage = 0;
    }
    if (AcquireBatchNode(1, g_CreditsUnusedFlagF0) != 0) {
        texU = 0.0f;
        texV = 0.375f;
        texW = 0.75f;
        texH = 0.125f;
        pRightTopLeft = g_CreditsUnusedFlagF0->vertices;
        pRightTopLeft->xBits = FLOAT_288_BITS;
        pRightTopLeft->yBits = FLOAT_408_BITS;
        pRightTopLeft->zBits = zValue;
        pRightTopLeft->rhwBits = FLOAT_ONE_BITS;
        pRightTopLeft->color = 0xFFFFFFFF;
        pRightTopLeft->specular = 0xFF000000;
        pRightTopLeft->u = texU;
        pRightTopLeft->v = texV;
        pRightBL = g_CreditsUnusedFlagF0->vertices + 1;
        pRightBL->xBits = FLOAT_288_BITS;
        pRightBL->yBits = FLOAT_440_BITS;
        pRightBL->zBits = zValue;
        pRightBL->rhwBits = FLOAT_ONE_BITS;
        pRightBL->color = 0xFFFFFFFF;
        pRightBL->specular = 0xFF000000;
        pRightBL->u = texU;
        pRightBL->v = texV + texH;
        tmpPRightVert = g_CreditsUnusedFlagF0->vertices + 2;
        tmpPRightVert->xBits = FLOAT_480_BITS;
        tmpPRightVert->yBits = FLOAT_408_BITS;
        tmpPRightVert->zBits = zValue;
        tmpPRightVert->rhwBits = FLOAT_ONE_BITS;
        tmpPRightVert->color = 0xFFFFFFFF;
        tmpPRightVert->specular = 0xFF000000;
        tmpPRightVert->u = texU + texW;
        tmpPRightVert->v = texV;
        rightBR = g_CreditsUnusedFlagF0->vertices + 3;
        rightBR->xBits = FLOAT_480_BITS;
        rightBR->yBits = FLOAT_440_BITS;
        rightBR->zBits = zValue;
        rightBR->rhwBits = FLOAT_ONE_BITS;
        rightBR->color = 0xFFFFFFFF;
        rightBR->specular = 0xFF000000;
        rightBR->u = texU + texW;
        rightBR->v = texV + texH;
        *g_CreditsUnusedFlagF0->colorSignedBytePtr = 0;
        g_CreditsUnusedFlagF0->texturePage = 0;
    }
}

/*
 * Draw the textured logo strip quad relative to the global screen origin in 320x240 mode.
 */
// FUNCTION: FF7 0x007aca8a
void __cdecl Credits_DrawLogoStrip1x(void)
{
    float uCoord;
    float textureV;
    int originX;
    int originY;
    float texW;
    int zValue;
    float texH;
    GfxTlVertex *outVertex;
    GfxTlVertex *vertBLPtr;
    GfxTlVertex *tlVertPtr;
    GfxTlVertex *pVertBR;

    zValue = FLOAT_0_1_BITS;
    originX = g_CreditsViewportX;
    originY = g_CreditsViewportY;
    if (AcquireBatchNode(1, g_CreditsLogoQuadBatches) != 0) {
        uCoord = 0.25f;
        textureV = 0.6875f;
        texW = 0.625f;
        texH = 0.0625f;
        tlVertPtr = g_CreditsLogoQuadBatches->vertices;
        tlVertPtr->x = (float)originX + 8e+01f;
        tlVertPtr->y = (float)originY + 204.0f;
        tlVertPtr->zBits = zValue;
        tlVertPtr->rhwBits = FLOAT_ONE_BITS;
        tlVertPtr->color = 0xFFFFFFFF;
        tlVertPtr->specular = 0xFF000000;
        tlVertPtr->u = uCoord;
        tlVertPtr->v = textureV;
        vertBLPtr = g_CreditsLogoQuadBatches->vertices + 1;
        vertBLPtr->x = (float)originX + 8e+01f;
        vertBLPtr->y = (float)originY + 204.0f + 16.0f;
        vertBLPtr->zBits = zValue;
        vertBLPtr->rhwBits = FLOAT_ONE_BITS;
        vertBLPtr->color = 0xFFFFFFFF;
        vertBLPtr->specular = 0xFF000000;
        vertBLPtr->u = uCoord;
        vertBLPtr->v = textureV + texH;
        outVertex = g_CreditsLogoQuadBatches->vertices + 2;
        outVertex->x = (float)originX + 8e+01f + 1.6e+02f;
        outVertex->y = (float)originY + 204.0f;
        outVertex->zBits = zValue;
        outVertex->rhwBits = FLOAT_ONE_BITS;
        outVertex->color = 0xFFFFFFFF;
        outVertex->specular = 0xFF000000;
        outVertex->u = uCoord + texW;
        outVertex->v = textureV;
        pVertBR = g_CreditsLogoQuadBatches->vertices + 3;
        pVertBR->x = (float)originX + 8e+01f + 1.6e+02f;
        pVertBR->y = (float)originY + 204.0f + 16.0f;
        pVertBR->zBits = zValue;
        pVertBR->rhwBits = FLOAT_ONE_BITS;
        pVertBR->color = 0xFFFFFFFF;
        pVertBR->specular = 0xFF000000;
        pVertBR->u = uCoord + texW;
        pVertBR->v = textureV + texH;
        *g_CreditsLogoQuadBatches->colorSignedBytePtr = 0;
        g_CreditsLogoQuadBatches->texturePage = 0;
    }
}

/*
 * Draw the two half-alpha text overlay quads in 640x480 mode.
 */
// FUNCTION: FF7 0x007accad
void __cdecl Credits_DrawAfterTextQuads2x(void)
{
    int wide;
    float uCoord;
    float textureV;
    int xLeft;
    int rightX;
    int calcDepth;
    GfxTlVertex *pFirstTopLeft;
    float vHeight;
    float texWidth;
    GfxTlVertex *firstTRPtr;
    GfxTlVertex *firstBottomLeftPtr;
    GfxTlVertex *secondV0Ptr;
    GfxTlVertex *firstBR;
    GfxTlVertex *secondTR;
    GfxTlVertex *secondBottomRemaining;
    GfxTlVertex *secondBottomRight;

    calcDepth = FLOAT_0_1_BITS;
    xLeft = 0x83;
    rightX = 0x144;
    wide = 0xC8;
    texWidth = 0.78125f;
    if (AcquireBatchNode(2, g_CreditsTextState) != 0) {
        textureV = 0.0f;
        uCoord = textureV;
        vHeight = 0.25f;
        pFirstTopLeft = g_CreditsTextState->vertices;
        pFirstTopLeft->x = xLeft;
        pFirstTopLeft->yBits = FLOAT_224_BITS;
        pFirstTopLeft->zBits = calcDepth;
        pFirstTopLeft->rhwBits = FLOAT_ONE_BITS;
        pFirstTopLeft->color = 0x80FFFFFF;
        pFirstTopLeft->specular = 0xFF000000;
        pFirstTopLeft->u = uCoord;
        pFirstTopLeft->v = textureV;
        firstBottomLeftPtr = g_CreditsTextState->vertices + 1;
        firstBottomLeftPtr->x = xLeft;
        firstBottomLeftPtr->yBits = FLOAT_256_BITS;
        firstBottomLeftPtr->zBits = calcDepth;
        firstBottomLeftPtr->rhwBits = FLOAT_ONE_BITS;
        firstBottomLeftPtr->color = 0x80FFFFFF;
        firstBottomLeftPtr->specular = 0xFF000000;
        firstBottomLeftPtr->u = uCoord;
        firstBottomLeftPtr->v = textureV + vHeight;
        firstTRPtr = g_CreditsTextState->vertices + 2;
        firstTRPtr->x = (float)xLeft + wide;
        firstTRPtr->yBits = FLOAT_224_BITS;
        firstTRPtr->zBits = calcDepth;
        firstTRPtr->rhwBits = FLOAT_ONE_BITS;
        firstTRPtr->color = 0x80FFFFFF;
        firstTRPtr->specular = 0xFF000000;
        firstTRPtr->u = uCoord + texWidth;
        firstTRPtr->v = textureV;
        firstBR = g_CreditsTextState->vertices + 3;
        firstBR->x = (float)xLeft + wide;
        firstBR->yBits = FLOAT_256_BITS;
        firstBR->zBits = calcDepth;
        firstBR->rhwBits = FLOAT_ONE_BITS;
        firstBR->color = 0x80FFFFFF;
        firstBR->specular = 0xFF000000;
        firstBR->u = uCoord + texWidth;
        firstBR->v = textureV + vHeight;
        *g_CreditsTextState->colorSignedBytePtr = 0;
        g_CreditsTextState->texturePage = 0;
        g_CreditsTextState->vertexCursor =
            g_CreditsTextState->vertexCursor + g_CreditsTextState->cursorStep;
        g_CreditsTextState->colorSignedBytePtr = g_CreditsTextState->colorSignedBytePtr + 1;
        textureV = 0.25f;
        texWidth = 0.73046875f;
        secondV0Ptr = g_CreditsTextState->vertices;
        secondV0Ptr->x = rightX;
        secondV0Ptr->yBits = FLOAT_224_BITS;
        secondV0Ptr->zBits = calcDepth;
        secondV0Ptr->rhwBits = FLOAT_ONE_BITS;
        secondV0Ptr->color = 0x80FFFFFF;
        secondV0Ptr->specular = 0xFF000000;
        secondV0Ptr->u = uCoord;
        secondV0Ptr->v = textureV;
        secondBottomRemaining = g_CreditsTextState->vertices + 1;
        secondBottomRemaining->x = rightX;
        secondBottomRemaining->yBits = FLOAT_256_BITS;
        secondBottomRemaining->zBits = calcDepth;
        secondBottomRemaining->rhwBits = FLOAT_ONE_BITS;
        secondBottomRemaining->color = 0x80FFFFFF;
        secondBottomRemaining->specular = 0xFF000000;
        secondBottomRemaining->u = uCoord;
        secondBottomRemaining->v = textureV + vHeight;
        secondTR = g_CreditsTextState->vertices + 2;
        secondTR->x = rightX + 187.0f;
        secondTR->yBits = FLOAT_224_BITS;
        secondTR->zBits = calcDepth;
        secondTR->rhwBits = FLOAT_ONE_BITS;
        secondTR->color = 0x80FFFFFF;
        secondTR->specular = 0xFF000000;
        secondTR->u = uCoord + texWidth;
        secondTR->v = textureV;
        secondBottomRight = g_CreditsTextState->vertices + 3;
        secondBottomRight->x = rightX + 187.0f;
        secondBottomRight->yBits = FLOAT_256_BITS;
        secondBottomRight->zBits = calcDepth;
        secondBottomRight->rhwBits = FLOAT_ONE_BITS;
        secondBottomRight->color = 0x80FFFFFF;
        secondBottomRight->specular = 0xFF000000;
        secondBottomRight->u = uCoord + texWidth;
        secondBottomRight->v = textureV + vHeight;
        *g_CreditsTextState->colorSignedBytePtr = 0;
        g_CreditsTextState->texturePage = 0;
    }
}

/*
 * Draw the two half-alpha text overlay quads relative to the screen origin in 320x240 mode.
 */
// FUNCTION: FF7 0x007ad064
void __cdecl Credits_DrawAfterTextQuads1x(void)
{
    int wide;
    float uCoord;
    float textureV;
    int xLeft;
    int originX;
    int rightX;
    float textureWidth;
    int viewY;
    int zVal;
    GfxTlVertex *vtxB0Ptr;
    GfxTlVertex *firstBottomRight;
    GfxTlVertex *firstVert2Ptr;
    GfxTlVertex *firstBottomLeft;
    GfxTlVertex *firstTopRemaining;
    float tmpTexHeight;
    GfxTlVertex *secondTR;
    GfxTlVertex *secondBottomRemaining;
    GfxTlVertex *secondBottomRight;

    zVal = FLOAT_0_1_BITS;
    originX = g_CreditsViewportX;
    viewY = g_CreditsViewportY;
    xLeft = 0x43;
    rightX = 0xA2;
    wide = 0x64;
    textureWidth = 0.390625f;
    if (AcquireBatchNode(2, g_CreditsTextState) != 0) {
        uCoord = 0.0f;
        textureV = 0.5f;
        tmpTexHeight = 0.125f;
        firstTopRemaining = g_CreditsTextState->vertices;
        firstTopRemaining->x = (float)originX + xLeft;
        firstTopRemaining->y = viewY + 112.0f;
        firstTopRemaining->zBits = zVal;
        firstTopRemaining->rhwBits = FLOAT_ONE_BITS;
        firstTopRemaining->color = 0x80FFFFFF;
        firstTopRemaining->specular = 0xFF000000;
        firstTopRemaining->u = uCoord;
        firstTopRemaining->v = textureV;
        firstBottomLeft = g_CreditsTextState->vertices + 1;
        firstBottomLeft->x = (float)originX + xLeft;
        firstBottomLeft->y = viewY + 112.0f + 16.0f;
        firstBottomLeft->zBits = zVal;
        firstBottomLeft->rhwBits = FLOAT_ONE_BITS;
        firstBottomLeft->color = 0x80FFFFFF;
        firstBottomLeft->specular = 0xFF000000;
        firstBottomLeft->u = uCoord;
        firstBottomLeft->v = textureV + tmpTexHeight;
        firstVert2Ptr = g_CreditsTextState->vertices + 2;
        firstVert2Ptr->x = (float)originX + xLeft + wide;
        firstVert2Ptr->y = viewY + 112.0f;
        firstVert2Ptr->zBits = zVal;
        firstVert2Ptr->rhwBits = FLOAT_ONE_BITS;
        firstVert2Ptr->color = 0x80FFFFFF;
        firstVert2Ptr->specular = 0xFF000000;
        firstVert2Ptr->u = uCoord + textureWidth;
        firstVert2Ptr->v = textureV;
        firstBottomRight = g_CreditsTextState->vertices + 3;
        firstBottomRight->x = (float)originX + xLeft + wide;
        firstBottomRight->y = viewY + 112.0f + 16.0f;
        firstBottomRight->zBits = zVal;
        firstBottomRight->rhwBits = FLOAT_ONE_BITS;
        firstBottomRight->color = 0x80FFFFFF;
        firstBottomRight->specular = 0xFF000000;
        firstBottomRight->u = uCoord + textureWidth;
        firstBottomRight->v = textureV + tmpTexHeight;
        *g_CreditsTextState->colorSignedBytePtr = 0;
        g_CreditsTextState->texturePage = 0;
        g_CreditsTextState->vertexCursor =
            g_CreditsTextState->vertexCursor + g_CreditsTextState->cursorStep;
        g_CreditsTextState->colorSignedBytePtr = g_CreditsTextState->colorSignedBytePtr + 1;
        textureV = 0.625f;
        textureWidth = 0.3671875f;
        vtxB0Ptr = g_CreditsTextState->vertices;
        vtxB0Ptr->x = (float)originX + rightX;
        vtxB0Ptr->y = viewY + 112.0f;
        vtxB0Ptr->zBits = zVal;
        vtxB0Ptr->rhwBits = FLOAT_ONE_BITS;
        vtxB0Ptr->color = 0x80FFFFFF;
        vtxB0Ptr->specular = 0xFF000000;
        vtxB0Ptr->u = uCoord;
        vtxB0Ptr->v = textureV;
        secondBottomRemaining = g_CreditsTextState->vertices + 1;
        secondBottomRemaining->x = (float)originX + rightX;
        secondBottomRemaining->y = viewY + 112.0f + 16.0f;
        secondBottomRemaining->zBits = zVal;
        secondBottomRemaining->rhwBits = FLOAT_ONE_BITS;
        secondBottomRemaining->color = 0x80FFFFFF;
        secondBottomRemaining->specular = 0xFF000000;
        secondBottomRemaining->u = uCoord;
        secondBottomRemaining->v = textureV + tmpTexHeight;
        secondTR = g_CreditsTextState->vertices + 2;
        secondTR->x = (float)originX + rightX + 94.0f;
        secondTR->y = viewY + 112.0f;
        secondTR->zBits = zVal;
        secondTR->rhwBits = FLOAT_ONE_BITS;
        secondTR->color = 0x80FFFFFF;
        secondTR->specular = 0xFF000000;
        secondTR->u = uCoord + textureWidth;
        secondTR->v = textureV;
        secondBottomRight = g_CreditsTextState->vertices + 3;
        secondBottomRight->x = (float)originX + rightX + 94.0f;
        secondBottomRight->y = viewY + 112.0f + 16.0f;
        secondBottomRight->zBits = zVal;
        secondBottomRight->rhwBits = FLOAT_ONE_BITS;
        secondBottomRight->color = 0x80FFFFFF;
        secondBottomRight->specular = 0xFF000000;
        secondBottomRight->u = uCoord + textureWidth;
        secondBottomRight->v = textureV + tmpTexHeight;
        *g_CreditsTextState->colorSignedBytePtr = 0;
        g_CreditsTextState->texturePage = 0;
    }
}

/*
 * Initialize the 76 particle records with staggered fade offsets and compute layout parameters based on UI scaling.
 */
// FUNCTION: FF7 0x007ad486
void __cdecl Credits_InitParticleRecords(void)
{
    int strip;
    int lowResult;

    lowResult = GetUiScalingMode() != MAIN_GRAPHICS_MODE_640X480;
    for (strip = 0; strip < 0x4C; strip++) {
        g_CreditsParticleRecords[strip].fadeLevel = 0xFF;
        g_CreditsParticleRecords[strip].fadeDelta = -(0x4C - strip + 4) >> 1;
        g_CreditsParticleRecords[strip].active = 1;
    }
    g_CreditsParticleRecords[0].stripCount = 0x4C;
    g_CreditsParticleStartX = (lowResult != 0 ? 0x42 : 0x83) + g_CreditsViewportX;
    g_CreditsParticleStartY = (0xE0 >> lowResult) + g_CreditsViewportY;
    g_CreditsParticleStepX = lowResult != 0 ? 3 : 5;
    g_CreditsParticleHeight = 0x20 >> lowResult;
}

/*
 * Render active fade strips, update particle counters, and deactivate completed strips.
 */
// FUNCTION: FF7 0x007ad552
int __cdecl Credits_UpdateFadeStrips(void)
{
    CreditsColorWord diffuse;
    int xPos;
    int positionY;
    int complete;
    int stripIndex;
    int count;
    GfxTlVertex *blVertex;
    GfxTlVertex *topLeft;
    GfxTlVertex *trVertex;
    GfxTlVertex *pBottomRight;

    count = g_CreditsParticleRecords[0].stripCount;
    complete = 0;
    positionY = g_CreditsParticleStartY;
    diffuse.value = 0;
    if (count > 0) {
        if (AcquireBatchNode(count, (void *)g_CreditsFadeOverlayBatch.renderObject) != 0) {
            for (stripIndex = 0; stripIndex < 0x4C; stripIndex++) {
                if (g_CreditsParticleRecords[stripIndex].active == 0) {
                    continue;
                }
                xPos = stripIndex * g_CreditsParticleStepX + g_CreditsParticleStartX;
                diffuse.alpha = g_CreditsParticleRecords[stripIndex].fadeAlpha;
                topLeft = g_CreditsFadeOverlayBatch.fadeOverlay->vertices;
                topLeft->x = xPos;
                topLeft->y = positionY;
                topLeft->zBits = 0;
                topLeft->rhwBits = FLOAT_ONE_BITS;
                topLeft->color = diffuse.value;
                topLeft->specular = 0xFF000000;
                blVertex = g_CreditsFadeOverlayBatch.fadeOverlay->vertices + 1;
                blVertex->x = xPos;
                blVertex->y = (float)positionY + g_CreditsParticleHeight;
                blVertex->zBits = 0;
                blVertex->rhwBits = FLOAT_ONE_BITS;
                blVertex->color = diffuse.value;
                blVertex->specular = 0xFF000000;
                trVertex = g_CreditsFadeOverlayBatch.fadeOverlay->vertices + 2;
                trVertex->x = (float)xPos + g_CreditsParticleStepX;
                trVertex->y = positionY;
                trVertex->zBits = 0;
                trVertex->rhwBits = FLOAT_ONE_BITS;
                trVertex->color = diffuse.value;
                trVertex->specular = 0xFF000000;
                pBottomRight = g_CreditsFadeOverlayBatch.fadeOverlay->vertices + 3;
                pBottomRight->x = (float)xPos + g_CreditsParticleStepX;
                pBottomRight->y = (float)positionY + g_CreditsParticleHeight;
                pBottomRight->zBits = 0;
                pBottomRight->rhwBits = FLOAT_ONE_BITS;
                pBottomRight->color = diffuse.value;
                pBottomRight->specular = 0xFF000000;
                g_CreditsFadeOverlayBatch.fadeOverlay->vertexCursor =
                    g_CreditsFadeOverlayBatch.fadeOverlay->vertexCursor + g_CreditsFadeOverlayBatch.fadeOverlay->cursorStep;
                g_CreditsParticleRecords[stripIndex].fadeLevel =
                    g_CreditsParticleRecords[stripIndex].fadeLevel + g_CreditsParticleRecords[stripIndex].fadeDelta;
                if (g_CreditsParticleRecords[stripIndex].fadeLevel <= 0) {
                    g_CreditsParticleRecords[stripIndex].active = 0;
                    g_CreditsParticleRecords[0].stripCount = g_CreditsParticleRecords[0].stripCount - 1;
                }
            }
        }
    } else {
        complete = 1;
    }
    return complete;
}
} /* extern "C" */
