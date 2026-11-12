// src/ff7/submarine/sub_atlas_quad_emit.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Emits a colored atlas quad for the submarine UI, scaled according to the supported UI
//   scaling mode.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/codegen.h"
#include "ff7/api/submarine.h"
#include "ff7/api/main.h"
#include "ff7/types/submarine.h"
#include "lib/api/graphics.h"
#include <math.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <mmsystem.h>
#include "ff7/gte.h"
#include "ff7/enums/main.h"
#include "ff7/enums/submarine.h"
#include "lib/types/graphics.h"

extern "C" {

#pragma intrinsic(abs)


#pragma intrinsic(strcpy, strcat)

#pragma data_seg(".data$u77_lut")
#pragma bss_seg(".data$u77_lut")
#pragma data_seg()
#pragma bss_seg()

#pragma intrinsic(sqrt)

/*
 * Emits a colored textured quad from the 256x256 atlas pages. The two 320x240 graphics modes halve the
 * screen displacement around the viewport centre and draw into batch alone; MAIN_GRAPHICS_MODE_640X480
 * doubles the source rectangle and splits it across the four page batches (batch, batchB, batchC,
 * batchD) where it crosses a page edge.
 */
// FUNCTION: FF7 0x007a3110
void __cdecl Submarine_DrawAtlasQuad(int originX, int originY, short srcX, short srcY, short width, short height, short alpha, int depth, int batch, int batchB, int batchC, int batchD, int color0, int color1, int color2, int color3)
{
    short widthRight;
    short crossYFlag;
    short sourceYHigh;
    short srcYNext;
    GfxTlVertex *pCornerTr;
    GfxTlVertex *subPVtxBl;
    GfxTlVertex *blVtxPtr;
    GfxTlVertex *pVtxTlPos;
    GfxTlVertex *mainVtxTlPos;
    GfxTlVertex *posVertexBrE;
    GfxTlVertex *dstPVtxTrPos;
    short rightSourceX;
    float vOrigin;
    float vSize;
    GfxTlVertex *curVertexBottomLeft;
    GfxTlVertex *vtxTl2;
    GfxTlVertex *vertLeftLower;
    GfxTlVertex *pVertexPosBr;
    GfxTlVertex *pickedVtxBlPosition;
    GfxTlVertex *posVertexTlD;
    GfxTlVertex *tmpPosVertexBrD;
    GfxTlVertex *vertPosTlQ;
    int scalingMode;
    short atlasX;
    GfxTlVertex *vtxTl3;
    GfxTlVertex *posVertexTr;
    GfxTlVertex *cornerTlPtr;
    GfxTlVertex *pVertexBottomRight;
    GfxTlVertex *tempPCornerTrPos;
    GfxTlVertex *workPVtxBlPos;
    GfxTlVertex *pPosVertexTlC;
    GfxTlVertex *pPosVertexBrC;
    GfxTlVertex *cornerBrPosBase;
    short quadCase;
    GfxTlVertex *vertexPositionTrC;
    GfxTlVertex *dstVtxBlPos;
    GfxTlVertex *vtxTlPos2Ptr;
    GfxTlVertex *rawPVtxBrPos;
    GfxTlVertex *mainPosVertexBrG;
    GfxTlVertex *vertTopRightC;
    GfxTlVertex *posVertexTrF;
    GfxTlVertex *posVertexBlG;
    float uOrigin;
    GfxTlVertex *vertPosTrB;
    GfxTlVertex *pVertexPosTlF;
    GfxTlVertex *inCornerBlPosition;
    GfxTlVertex *positionVertexBlE;
    GfxTlVertex *posVertexBrHeight;
    short crossesX;
    float uExtent;
    short remainingWidth;
    short xUpperHalf;
    GfxTlVertex *vertexBottomRightB;
    GfxTlVertex *posVertexTrGPtr;
    short hLower;
    short firstHeight;
    short srcYSecond;
    GfxRenderObject *quadBatch;
    GfxTlVertex *vertexTopRightB;

    scalingMode = GetUiScalingMode();
    if (scalingMode >= MAIN_GRAPHICS_MODE_320X240) {
        if (scalingMode > MAIN_GRAPHICS_MODE_320X240_CENTERED) {
            if (scalingMode != MAIN_GRAPHICS_MODE_640X480) {
                return;
            }
            srcX <<= 1;
            srcY <<= 1;
            width <<= 1;
            height <<= 1;
            xUpperHalf = srcX >= 0x100;
            crossesX = srcX + width > 0x100;
            sourceYHigh = srcY >= 0x100;
            crossYFlag = srcY + height > 0x100;
            if (xUpperHalf == crossesX && sourceYHigh == crossYFlag) {
                quadCase = xUpperHalf << 1 | sourceYHigh;
                switch (quadCase) {
                case SUBMARINE_ATLAS_PAGE_TOP_LEFT:
                    atlasX = srcX;
                    srcYNext = srcY;
                    quadBatch = (GfxRenderObject *)batch;
                    break;
                case SUBMARINE_ATLAS_PAGE_BOTTOM_LEFT:
                    atlasX = srcX;
                    srcYNext = srcY - 0x100;
                    quadBatch = (GfxRenderObject *)batchC;
                    break;
                case SUBMARINE_ATLAS_PAGE_TOP_RIGHT:
                    atlasX = srcX - 0x100;
                    srcYNext = srcY;
                    quadBatch = (GfxRenderObject *)batchB;
                    break;
                case SUBMARINE_ATLAS_PAGE_BOTTOM_RIGHT:
                    atlasX = srcX - 0x100;
                    srcYNext = srcY - 0x100;
                    quadBatch = (GfxRenderObject *)batchD;
                }
                if (AcquireBatchNode(1, quadBatch) != 0) {
                    uOrigin = (float)atlasX * 0.00390625f;
                    vOrigin = (float)srcYNext * 0.00390625f;
                    uExtent = (float)width * 0.00390625f;
                    vSize = (float)height * 0.00390625f;
                    cornerTlPtr = (GfxTlVertex *)quadBatch->vertexCursor;
                    cornerTlPtr->x = (float)(short)originX;
                    cornerTlPtr->y = (float)(short)originY;
                    cornerTlPtr->zBits = depth;
                    cornerTlPtr->rhwBits = FLOAT_HALF_BITS;
                    cornerTlPtr->color = color0;
                    cornerTlPtr->u = uOrigin;
                    cornerTlPtr->v = vOrigin;
                    curVertexBottomLeft = (GfxTlVertex *)(quadBatch->vertexCursor + 0x20);
                    curVertexBottomLeft->x = (float)(short)originX;
                    curVertexBottomLeft->y = (float)(short)originY + height;
                    curVertexBottomLeft->zBits = depth;
                    curVertexBottomLeft->rhwBits = FLOAT_HALF_BITS;
                    curVertexBottomLeft->color = color1;
                    curVertexBottomLeft->u = uOrigin;
                    curVertexBottomLeft->v = vOrigin + vSize;
                    pCornerTr = (GfxTlVertex *)(quadBatch->vertexCursor + 0x40);
                    pCornerTr->x = (float)(short)originX + width;
                    pCornerTr->y = (float)(short)originY;
                    pCornerTr->zBits = depth;
                    pCornerTr->rhwBits = FLOAT_HALF_BITS;
                    pCornerTr->color = color2;
                    pCornerTr->u = uOrigin + uExtent;
                    pCornerTr->v = vOrigin;
                    pVertexBottomRight = (GfxTlVertex *)(quadBatch->vertexCursor + 0x60);
                    pVertexBottomRight->x = (float)(short)originX + width;
                    pVertexBottomRight->y = (float)(short)originY + height;
                    pVertexBottomRight->zBits = depth;
                    pVertexBottomRight->rhwBits = FLOAT_HALF_BITS;
                    pVertexBottomRight->color = color3;
                    pVertexBottomRight->u = uOrigin + uExtent;
                    pVertexBottomRight->v = vOrigin + vSize;
                    *quadBatch->colorSignedBytePtr = alpha;
                    quadBatch->texturePage = alpha;
                }
            } else {
                if (xUpperHalf == 0 && crossesX != 0) {
                    if (sourceYHigh != 0) {
                        quadCase = SUBMARINE_ATLAS_SPLIT_X_BOTTOM;
                    } else if (crossYFlag != 0) {
                        quadCase = SUBMARINE_ATLAS_SPLIT_XY;
                    } else {
                        quadCase = SUBMARINE_ATLAS_SPLIT_X_TOP;
                    }
                } else if (xUpperHalf != 0) {
                    quadCase = SUBMARINE_ATLAS_SPLIT_Y_RIGHT;
                } else {
                    quadCase = SUBMARINE_ATLAS_SPLIT_Y_LEFT;
                }
                switch (quadCase) {
                case SUBMARINE_ATLAS_SPLIT_X_TOP:
                case SUBMARINE_ATLAS_SPLIT_X_BOTTOM:
                    atlasX = srcX;
                    srcYNext = srcY;
                    remainingWidth = 0x100 - srcX;
                    firstHeight = height;
                    rightSourceX = 0;
                    srcYSecond = srcY;
                    widthRight = srcX + width + -0x100;
                    hLower = height;
                    quadBatch = (GfxRenderObject *)((quadCase == SUBMARINE_ATLAS_SPLIT_X_TOP) ? batch : batchC);
                    if (AcquireBatchNode(1, quadBatch) != 0) {
                        uOrigin = (float)atlasX * 0.00390625f;
                        vOrigin = (float)srcYNext * 0.00390625f;
                        uExtent = (float)remainingWidth * 0.00390625f;
                        vSize = (float)firstHeight * 0.00390625f;
                        vtxTl2 = (GfxTlVertex *)quadBatch->vertexCursor;
                        vtxTl2->x = (float)(short)originX;
                        vtxTl2->y = (float)(short)originY;
                        vtxTl2->zBits = depth;
                        vtxTl2->rhwBits = FLOAT_HALF_BITS;
                        vtxTl2->color = color0;
                        vtxTl2->u = uOrigin;
                        vtxTl2->v = vOrigin;
                        subPVtxBl = (GfxTlVertex *)(quadBatch->vertexCursor + 0x20);
                        subPVtxBl->x = (float)(short)originX;
                        subPVtxBl->y = (float)(short)originY + firstHeight;
                        subPVtxBl->zBits = depth;
                        subPVtxBl->rhwBits = FLOAT_HALF_BITS;
                        subPVtxBl->color = color1;
                        subPVtxBl->u = uOrigin;
                        subPVtxBl->v = vOrigin + vSize;
                        vertexTopRightB = (GfxTlVertex *)(quadBatch->vertexCursor + 0x40);
                        vertexTopRightB->x = (float)(short)originX + remainingWidth;
                        vertexTopRightB->y = (float)(short)originY;
                        vertexTopRightB->zBits = depth;
                        vertexTopRightB->rhwBits = FLOAT_HALF_BITS;
                        vertexTopRightB->color = color2;
                        vertexTopRightB->u = uOrigin + uExtent;
                        vertexTopRightB->v = vOrigin;
                        vertexBottomRightB = (GfxTlVertex *)(quadBatch->vertexCursor + 0x60);
                        vertexBottomRightB->x = (float)(short)originX + remainingWidth;
                        vertexBottomRightB->y = (float)(short)originY + firstHeight;
                        vertexBottomRightB->zBits = depth;
                        vertexBottomRightB->rhwBits = FLOAT_HALF_BITS;
                        vertexBottomRightB->color = color3;
                        vertexBottomRightB->u = uOrigin + uExtent;
                        vertexBottomRightB->v = vOrigin + vSize;
                        *quadBatch->colorSignedBytePtr = alpha;
                        quadBatch->texturePage = alpha;
                    }
                    quadBatch = (GfxRenderObject *)((quadCase == SUBMARINE_ATLAS_SPLIT_X_TOP) ? batchB : batchD);
                    if (AcquireBatchNode(1, quadBatch) != 0) {
                        uOrigin = (float)rightSourceX * 0.00390625f;
                        vOrigin = (float)srcYSecond * 0.00390625f;
                        uExtent = (float)widthRight * 0.00390625f;
                        vSize = (float)hLower * 0.00390625f;
                        *(short *)&originX += remainingWidth;
                        vtxTl3 = (GfxTlVertex *)quadBatch->vertexCursor;
                        vtxTl3->x = (float)(short)originX;
                        vtxTl3->y = (float)(short)originY;
                        vtxTl3->zBits = depth;
                        vtxTl3->rhwBits = FLOAT_HALF_BITS;
                        vtxTl3->color = color0;
                        vtxTl3->u = uOrigin;
                        vtxTl3->v = vOrigin;
                        vertLeftLower = (GfxTlVertex *)(quadBatch->vertexCursor + 0x20);
                        vertLeftLower->x = (float)(short)originX;
                        vertLeftLower->y = (float)(short)originY + hLower;
                        vertLeftLower->zBits = depth;
                        vertLeftLower->rhwBits = FLOAT_HALF_BITS;
                        vertLeftLower->color = color1;
                        vertLeftLower->u = uOrigin;
                        vertLeftLower->v = vOrigin + vSize;
                        posVertexTr = (GfxTlVertex *)(quadBatch->vertexCursor + 0x40);
                        posVertexTr->x = (float)(short)originX + widthRight;
                        posVertexTr->y = (float)(short)originY;
                        posVertexTr->zBits = depth;
                        posVertexTr->rhwBits = FLOAT_HALF_BITS;
                        posVertexTr->color = color2;
                        posVertexTr->u = uOrigin + uExtent;
                        posVertexTr->v = vOrigin;
                        pVertexPosBr = (GfxTlVertex *)(quadBatch->vertexCursor + 0x60);
                        pVertexPosBr->x = (float)(short)originX + widthRight;
                        pVertexPosBr->y = (float)(short)originY + hLower;
                        pVertexPosBr->zBits = depth;
                        pVertexPosBr->rhwBits = FLOAT_HALF_BITS;
                        pVertexPosBr->color = color3;
                        pVertexPosBr->u = uOrigin + uExtent;
                        pVertexPosBr->v = vOrigin + vSize;
                        *quadBatch->colorSignedBytePtr = alpha;
                        quadBatch->texturePage = alpha;
                    }
                    break;
                case SUBMARINE_ATLAS_SPLIT_Y_LEFT:
                case SUBMARINE_ATLAS_SPLIT_Y_RIGHT:
                    atlasX = srcX;
                    srcYNext = srcY;
                    remainingWidth = width;
                    firstHeight = 0x100 - srcY;
                    rightSourceX = srcX;
                    srcYSecond = 0;
                    widthRight = width;
                    hLower = srcY + height + -0x100;
                    quadBatch = (GfxRenderObject *)((quadCase == SUBMARINE_ATLAS_SPLIT_Y_LEFT) ? batch : batchB);
                    if (AcquireBatchNode(1, quadBatch) != 0) {
                        uOrigin = (float)atlasX * 0.00390625f;
                        vOrigin = (float)srcYNext * 0.00390625f;
                        uExtent = (float)remainingWidth * 0.00390625f;
                        vSize = (float)firstHeight * 0.00390625f;
                        pVtxTlPos = (GfxTlVertex *)quadBatch->vertexCursor;
                        pVtxTlPos->x = (float)(short)originX;
                        pVtxTlPos->y = (float)(short)originY;
                        pVtxTlPos->zBits = depth;
                        pVtxTlPos->rhwBits = FLOAT_HALF_BITS;
                        pVtxTlPos->color = color0;
                        pVtxTlPos->u = uOrigin;
                        pVtxTlPos->v = vOrigin;
                        blVtxPtr = (GfxTlVertex *)(quadBatch->vertexCursor + 0x20);
                        blVtxPtr->x = (float)(short)originX;
                        blVtxPtr->y = (float)(short)originY + firstHeight;
                        blVtxPtr->zBits = depth;
                        blVtxPtr->rhwBits = FLOAT_HALF_BITS;
                        blVtxPtr->color = color1;
                        blVtxPtr->u = uOrigin;
                        blVtxPtr->v = vOrigin + vSize;
                        vertPosTrB = (GfxTlVertex *)(quadBatch->vertexCursor + 0x40);
                        vertPosTrB->x = (float)(short)originX + remainingWidth;
                        vertPosTrB->y = (float)(short)originY;
                        vertPosTrB->zBits = depth;
                        vertPosTrB->rhwBits = FLOAT_HALF_BITS;
                        vertPosTrB->color = color2;
                        vertPosTrB->u = uOrigin + uExtent;
                        vertPosTrB->v = vOrigin;
                        rawPVtxBrPos = (GfxTlVertex *)(quadBatch->vertexCursor + 0x60);
                        rawPVtxBrPos->x = (float)(short)originX + remainingWidth;
                        rawPVtxBrPos->y = (float)(short)originY + firstHeight;
                        rawPVtxBrPos->zBits = depth;
                        rawPVtxBrPos->rhwBits = FLOAT_HALF_BITS;
                        rawPVtxBrPos->color = color3;
                        rawPVtxBrPos->u = uOrigin + uExtent;
                        rawPVtxBrPos->v = vOrigin + vSize;
                        *quadBatch->colorSignedBytePtr = alpha;
                        quadBatch->texturePage = alpha;
                    }
                    quadBatch = (GfxRenderObject *)((quadCase == SUBMARINE_ATLAS_SPLIT_Y_LEFT) ? batchC : batchD);
                    if (AcquireBatchNode(1, quadBatch) != 0) {
                        uOrigin = (float)rightSourceX * 0.00390625f;
                        vOrigin = (float)srcYSecond * 0.00390625f;
                        uExtent = (float)widthRight * 0.00390625f;
                        vSize = (float)hLower * 0.00390625f;
                        *(short *)&originY += firstHeight;
                        vtxTlPos2Ptr = (GfxTlVertex *)quadBatch->vertexCursor;
                        vtxTlPos2Ptr->x = (float)(short)originX;
                        vtxTlPos2Ptr->y = (float)(short)originY;
                        vtxTlPos2Ptr->zBits = depth;
                        vtxTlPos2Ptr->rhwBits = FLOAT_HALF_BITS;
                        vtxTlPos2Ptr->color = color0;
                        vtxTlPos2Ptr->u = uOrigin;
                        vtxTlPos2Ptr->v = vOrigin;
                        dstVtxBlPos = (GfxTlVertex *)(quadBatch->vertexCursor + 0x20);
                        dstVtxBlPos->x = (float)(short)originX;
                        dstVtxBlPos->y = (float)(short)originY + hLower;
                        dstVtxBlPos->zBits = depth;
                        dstVtxBlPos->rhwBits = FLOAT_HALF_BITS;
                        dstVtxBlPos->color = color1;
                        dstVtxBlPos->u = uOrigin;
                        dstVtxBlPos->v = vOrigin + vSize;
                        vertexPositionTrC = (GfxTlVertex *)(quadBatch->vertexCursor + 0x40);
                        vertexPositionTrC->x = (float)(short)originX + widthRight;
                        vertexPositionTrC->y = (float)(short)originY;
                        vertexPositionTrC->zBits = depth;
                        vertexPositionTrC->rhwBits = FLOAT_HALF_BITS;
                        vertexPositionTrC->color = color2;
                        vertexPositionTrC->u = uOrigin + uExtent;
                        vertexPositionTrC->v = vOrigin;
                        pPosVertexBrC = (GfxTlVertex *)(quadBatch->vertexCursor + 0x60);
                        pPosVertexBrC->x = (float)(short)originX + widthRight;
                        pPosVertexBrC->y = (float)(short)originY + hLower;
                        pPosVertexBrC->zBits = depth;
                        pPosVertexBrC->rhwBits = FLOAT_HALF_BITS;
                        pPosVertexBrC->color = color3;
                        pPosVertexBrC->u = uOrigin + uExtent;
                        pPosVertexBrC->v = vOrigin + vSize;
                        *quadBatch->colorSignedBytePtr = alpha;
                        quadBatch->texturePage = alpha;
                    }
                    break;
                case SUBMARINE_ATLAS_SPLIT_XY:
                    atlasX = srcX;
                    srcYNext = srcY;
                    remainingWidth = 0x100 - srcX;
                    firstHeight = 0x100 - srcY;
                    rightSourceX = 0;
                    srcYSecond = srcY;
                    widthRight = srcX + width + -0x100;
                    hLower = 0x100 - srcY;
                    quadBatch = (GfxRenderObject *)batch;
                    if (AcquireBatchNode(1, quadBatch) != 0) {
                        uOrigin = (float)atlasX * 0.00390625f;
                        vOrigin = (float)srcYNext * 0.00390625f;
                        uExtent = (float)remainingWidth * 0.00390625f;
                        vSize = (float)firstHeight * 0.00390625f;
                        pPosVertexTlC = (GfxTlVertex *)quadBatch->vertexCursor;
                        pPosVertexTlC->x = (float)(short)originX;
                        pPosVertexTlC->y = (float)(short)originY;
                        pPosVertexTlC->zBits = depth;
                        pPosVertexTlC->rhwBits = FLOAT_HALF_BITS;
                        pPosVertexTlC->color = color0;
                        pPosVertexTlC->u = uOrigin;
                        pPosVertexTlC->v = vOrigin;
                        workPVtxBlPos = (GfxTlVertex *)(quadBatch->vertexCursor + 0x20);
                        workPVtxBlPos->x = (float)(short)originX;
                        workPVtxBlPos->y = (float)(short)originY + firstHeight;
                        workPVtxBlPos->zBits = depth;
                        workPVtxBlPos->rhwBits = FLOAT_HALF_BITS;
                        workPVtxBlPos->color = color1;
                        workPVtxBlPos->u = uOrigin;
                        workPVtxBlPos->v = vOrigin + vSize;
                        tempPCornerTrPos = (GfxTlVertex *)(quadBatch->vertexCursor + 0x40);
                        tempPCornerTrPos->x = (float)(short)originX + remainingWidth;
                        tempPCornerTrPos->y = (float)(short)originY;
                        tempPCornerTrPos->zBits = depth;
                        tempPCornerTrPos->rhwBits = FLOAT_HALF_BITS;
                        tempPCornerTrPos->color = color2;
                        tempPCornerTrPos->u = uOrigin + uExtent;
                        tempPCornerTrPos->v = vOrigin;
                        tmpPosVertexBrD = (GfxTlVertex *)(quadBatch->vertexCursor + 0x60);
                        tmpPosVertexBrD->x = (float)(short)originX + remainingWidth;
                        tmpPosVertexBrD->y = (float)(short)originY + firstHeight;
                        tmpPosVertexBrD->zBits = depth;
                        tmpPosVertexBrD->rhwBits = FLOAT_HALF_BITS;
                        tmpPosVertexBrD->color = color3;
                        tmpPosVertexBrD->u = uOrigin + uExtent;
                        tmpPosVertexBrD->v = vOrigin + vSize;
                        *quadBatch->colorSignedBytePtr = alpha;
                        quadBatch->texturePage = alpha;
                    }
                    quadBatch = (GfxRenderObject *)batchB;
                    if (AcquireBatchNode(1, quadBatch) != 0) {
                        uOrigin = (float)rightSourceX * 0.00390625f;
                        vOrigin = (float)srcYSecond * 0.00390625f;
                        uExtent = (float)widthRight * 0.00390625f;
                        vSize = (float)hLower * 0.00390625f;
                        *(short *)&originX += remainingWidth;
                        posVertexTlD = (GfxTlVertex *)quadBatch->vertexCursor;
                        posVertexTlD->x = (float)(short)originX;
                        posVertexTlD->y = (float)(short)originY;
                        posVertexTlD->zBits = depth;
                        posVertexTlD->rhwBits = FLOAT_HALF_BITS;
                        posVertexTlD->color = color0;
                        posVertexTlD->u = uOrigin;
                        posVertexTlD->v = vOrigin;
                        pickedVtxBlPosition = (GfxTlVertex *)(quadBatch->vertexCursor + 0x20);
                        pickedVtxBlPosition->x = (float)(short)originX;
                        pickedVtxBlPosition->y = (float)(short)originY + hLower;
                        pickedVtxBlPosition->zBits = depth;
                        pickedVtxBlPosition->rhwBits = FLOAT_HALF_BITS;
                        pickedVtxBlPosition->color = color1;
                        pickedVtxBlPosition->u = uOrigin;
                        pickedVtxBlPosition->v = vOrigin + vSize;
                        dstPVtxTrPos = (GfxTlVertex *)(quadBatch->vertexCursor + 0x40);
                        dstPVtxTrPos->x = (float)(short)originX + widthRight;
                        dstPVtxTrPos->y = (float)(short)originY;
                        dstPVtxTrPos->zBits = depth;
                        dstPVtxTrPos->rhwBits = FLOAT_HALF_BITS;
                        dstPVtxTrPos->color = color2;
                        dstPVtxTrPos->u = uOrigin + uExtent;
                        dstPVtxTrPos->v = vOrigin;
                        posVertexBrE = (GfxTlVertex *)(quadBatch->vertexCursor + 0x60);
                        posVertexBrE->x = (float)(short)originX + widthRight;
                        posVertexBrE->y = (float)(short)originY + hLower;
                        posVertexBrE->zBits = depth;
                        posVertexBrE->rhwBits = FLOAT_HALF_BITS;
                        posVertexBrE->color = color3;
                        posVertexBrE->u = uOrigin + uExtent;
                        posVertexBrE->v = vOrigin + vSize;
                        *quadBatch->colorSignedBytePtr = alpha;
                        quadBatch->texturePage = alpha;
                    }
                    *(short *)&originX -= remainingWidth;
                    *(short *)&originY += firstHeight;
                    srcYNext = 0;
                    firstHeight = srcY + height + -0x100;
                    srcYSecond = 0;
                    hLower = srcY + height + -0x100;
                    quadBatch = (GfxRenderObject *)batchC;
                    if (AcquireBatchNode(1, quadBatch) != 0) {
                        uOrigin = (float)atlasX * 0.00390625f;
                        vOrigin = (float)srcYNext * 0.00390625f;
                        uExtent = (float)remainingWidth * 0.00390625f;
                        vSize = (float)firstHeight * 0.00390625f;
                        mainVtxTlPos = (GfxTlVertex *)quadBatch->vertexCursor;
                        mainVtxTlPos->x = (float)(short)originX;
                        mainVtxTlPos->y = (float)(short)originY;
                        mainVtxTlPos->zBits = depth;
                        mainVtxTlPos->rhwBits = FLOAT_HALF_BITS;
                        mainVtxTlPos->color = color0;
                        mainVtxTlPos->u = uOrigin;
                        mainVtxTlPos->v = vOrigin;
                        positionVertexBlE = (GfxTlVertex *)(quadBatch->vertexCursor + 0x20);
                        positionVertexBlE->x = (float)(short)originX;
                        positionVertexBlE->y = (float)(short)originY + firstHeight;
                        positionVertexBlE->zBits = depth;
                        positionVertexBlE->rhwBits = FLOAT_HALF_BITS;
                        positionVertexBlE->color = color1;
                        positionVertexBlE->u = uOrigin;
                        positionVertexBlE->v = vOrigin + vSize;
                        posVertexTrGPtr = (GfxTlVertex *)(quadBatch->vertexCursor + 0x40);
                        posVertexTrGPtr->x = (float)(short)originX + remainingWidth;
                        posVertexTrGPtr->y = (float)(short)originY;
                        posVertexTrGPtr->zBits = depth;
                        posVertexTrGPtr->rhwBits = FLOAT_HALF_BITS;
                        posVertexTrGPtr->color = color2;
                        posVertexTrGPtr->u = uOrigin + uExtent;
                        posVertexTrGPtr->v = vOrigin;
                        posVertexBrHeight = (GfxTlVertex *)(quadBatch->vertexCursor + 0x60);
                        posVertexBrHeight->x = (float)(short)originX + remainingWidth;
                        posVertexBrHeight->y = (float)(short)originY + firstHeight;
                        posVertexBrHeight->zBits = depth;
                        posVertexBrHeight->rhwBits = FLOAT_HALF_BITS;
                        posVertexBrHeight->color = color3;
                        posVertexBrHeight->u = uOrigin + uExtent;
                        posVertexBrHeight->v = vOrigin + vSize;
                        *quadBatch->colorSignedBytePtr = alpha;
                        quadBatch->texturePage = alpha;
                    }
                    quadBatch = (GfxRenderObject *)batchD;
                    if (AcquireBatchNode(1, quadBatch) != 0) {
                        uOrigin = (float)rightSourceX * 0.00390625f;
                        vOrigin = (float)srcYSecond * 0.00390625f;
                        uExtent = (float)widthRight * 0.00390625f;
                        vSize = (float)hLower * 0.00390625f;
                        *(short *)&originX += remainingWidth;
                        pVertexPosTlF = (GfxTlVertex *)quadBatch->vertexCursor;
                        pVertexPosTlF->x = (float)(short)originX;
                        pVertexPosTlF->y = (float)(short)originY;
                        pVertexPosTlF->zBits = depth;
                        pVertexPosTlF->rhwBits = FLOAT_HALF_BITS;
                        pVertexPosTlF->color = color0;
                        pVertexPosTlF->u = uOrigin;
                        pVertexPosTlF->v = vOrigin;
                        posVertexBlG = (GfxTlVertex *)(quadBatch->vertexCursor + 0x20);
                        posVertexBlG->x = (float)(short)originX;
                        posVertexBlG->y = (float)(short)originY + hLower;
                        posVertexBlG->zBits = depth;
                        posVertexBlG->rhwBits = FLOAT_HALF_BITS;
                        posVertexBlG->color = color1;
                        posVertexBlG->u = uOrigin;
                        posVertexBlG->v = vOrigin + vSize;
                        posVertexTrF = (GfxTlVertex *)(quadBatch->vertexCursor + 0x40);
                        posVertexTrF->x = (float)(short)originX + widthRight;
                        posVertexTrF->y = (float)(short)originY;
                        posVertexTrF->zBits = depth;
                        posVertexTrF->rhwBits = FLOAT_HALF_BITS;
                        posVertexTrF->color = color2;
                        posVertexTrF->u = uOrigin + uExtent;
                        posVertexTrF->v = vOrigin;
                        mainPosVertexBrG = (GfxTlVertex *)(quadBatch->vertexCursor + 0x60);
                        mainPosVertexBrG->x = (float)(short)originX + widthRight;
                        mainPosVertexBrG->y = (float)(short)originY + hLower;
                        mainPosVertexBrG->zBits = depth;
                        mainPosVertexBrG->rhwBits = FLOAT_HALF_BITS;
                        mainPosVertexBrG->color = color3;
                        mainPosVertexBrG->u = uOrigin + uExtent;
                        mainPosVertexBrG->v = vOrigin + vSize;
                        *quadBatch->colorSignedBytePtr = alpha;
                        quadBatch->texturePage = alpha;
                    }
                }
            }
        } else {
            *(short *)&originX = ((short)originX - g_SubmarineViewportCenterX >> 1) + g_SubmarineViewportCenterX;
            *(short *)&originY = ((short)originY - g_SubmarineViewportCenterY >> 1) + g_SubmarineViewportCenterY;
            if (AcquireBatchNode(1, (void *)batch) != 0) {
                uOrigin = (float)srcX * 0.00390625f;
                vOrigin = (float)srcY * 0.00390625f;
                uExtent = (float)width * 0.00390625f;
                vSize = (float)height * 0.00390625f;
                vertPosTlQ = (GfxTlVertex *)(((GfxRenderObject *)batch)->vertexCursor);
                vertPosTlQ->x = (float)(short)originX;
                vertPosTlQ->y = (float)(short)originY;
                vertPosTlQ->zBits = depth;
                vertPosTlQ->rhwBits = FLOAT_HALF_BITS;
                vertPosTlQ->color = color0;
                vertPosTlQ->u = uOrigin;
                vertPosTlQ->v = vOrigin;
                inCornerBlPosition = (GfxTlVertex *)(((GfxRenderObject *)batch)->vertexCursor + 0x20);
                inCornerBlPosition->x = (float)(short)originX;
                inCornerBlPosition->y = (float)(short)originY + height;
                inCornerBlPosition->zBits = depth;
                inCornerBlPosition->rhwBits = FLOAT_HALF_BITS;
                inCornerBlPosition->color = color1;
                inCornerBlPosition->u = uOrigin;
                inCornerBlPosition->v = vOrigin + vSize;
                vertTopRightC = (GfxTlVertex *)(((GfxRenderObject *)batch)->vertexCursor + 0x40);
                vertTopRightC->x = (float)(short)originX + width;
                vertTopRightC->y = (float)(short)originY;
                vertTopRightC->zBits = depth;
                vertTopRightC->rhwBits = FLOAT_HALF_BITS;
                vertTopRightC->color = color2;
                vertTopRightC->u = uOrigin + uExtent;
                vertTopRightC->v = vOrigin;
                cornerBrPosBase = (GfxTlVertex *)(((GfxRenderObject *)batch)->vertexCursor + 0x60);
                cornerBrPosBase->x = (float)(short)originX + width;
                cornerBrPosBase->y = (float)(short)originY + height;
                cornerBrPosBase->zBits = depth;
                cornerBrPosBase->rhwBits = FLOAT_HALF_BITS;
                cornerBrPosBase->color = color3;
                cornerBrPosBase->u = uOrigin + uExtent;
                cornerBrPosBase->v = vOrigin + vSize;
                *(char *)((GfxRenderObject *)batch)->colorSignedBytePtr = alpha;
                ((GfxRenderObject *)batch)->texturePage = alpha;
            }
        }
    }
}
} /* extern "C" */
