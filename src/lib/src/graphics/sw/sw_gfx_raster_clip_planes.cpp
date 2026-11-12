// src/lib/src/graphics/sw/sw_gfx_raster_clip_planes.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Clipping for the software rasterizer: builds the float clip rectangle, clears the batch
//   arena, and clips edges and triangles against the six clip planes.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/api/graphics.h"
#include "lib/types/graphics.h"
#include "lib/enums/graphics.h"
#include "lib/api/mem.h"
extern "C" {

// Zero-fills one raster batch arena block (0x12F4 units).
// FUNCTION: FF7 0x006ac860
void __cdecl Gfx_RasterClearBatchArena(void *block)
{
    ZeroFillMemory32(0x12F4, block);
}

// Builds the float clip rectangle from the integer clip bounds in the context. clipLeft and clipTop
// are the context's integer bounds converted to float, and clipRight and clipBottom are the integer
// bounds plus 1.0. nearPlane copies the raw bits of context->nearPlane unchanged, and farLimit is
// the float farPlane minus 5.0.
// FUNCTION: FF7 0x006ac876
void __cdecl Gfx_RasterBuildFloatClipRect(SwFloatClipRect *rect, GfxEngineContext *context)
{
    rect->clipLeft = context->clipLeft;
    rect->clipRight = context->clipRight + 1.0f;
    rect->clipTop = context->clipTop;
    rect->clipBottom = context->clipBottom + 1.0f;
    rect->nearPlane = (*(int *)&context->nearPlane);
    rect->farLimit = context->farPlane - 5.0f;
}

/*
 * Returns the vertex's cachedResult immediately when nonzero. Otherwise compares clipX, clipY
 * or clipZ against the plane's planeThreshold with alternating
 * greater-equal/less-equal tests, returning one for a passing test and zero
 * otherwise; unsupported plane indices return zero.
 */
// FUNCTION: FF7 0x006ac8e7
int __cdecl Gfx_RasterIsVertexInsideClipPlane(int plane, SwClipVertex *vertex, struct GfxRasterClipScratch *thresholds)
{
    int result;

    result = vertex->cachedResult;
    if (result != 0) { /* empty first arm: lib/codegen.h */
    } else {
        switch (plane) {
        case GFX_CLIP_PLANE_MIN_X:
            if (vertex->clipX >= thresholds->planeThreshold[plane]) {
                result = 1;
            }
            break;
        case GFX_CLIP_PLANE_MAX_X:
            if (vertex->clipX <= thresholds->planeThreshold[plane]) {
                result = 1;
            }
            break;
        case GFX_CLIP_PLANE_MIN_Y:
            if (vertex->clipY >= thresholds->planeThreshold[plane]) {
                result = 1;
            }
            break;
        case GFX_CLIP_PLANE_MAX_Y:
            if (vertex->clipY <= thresholds->planeThreshold[plane]) {
                result = 1;
            }
            break;
        case GFX_CLIP_PLANE_MIN_Z:
            if (vertex->clipZ >= thresholds->planeThreshold[plane]) {
                result = 1;
            }
            break;
        case GFX_CLIP_PLANE_MAX_Z:
            if (vertex->clipZ <= thresholds->planeThreshold[plane]) {
                result = 1;
            }
        }
    }
    return result;
}

/*
 * Intersects an edge with one of six clip planes using the planeThreshold table. The X planes set X and solve Y, the Y planes set Y and solve X, and the Z planes interpolate position, UVs and attributes,
 * copying the source component for a zero denominator.
 */
// FUNCTION: FF7 0x006ac9f9
void __cdecl Gfx_RasterIntersectEdgeWithClipPlane(int plane, GfxRasterEdgeVertex *startVertex, SwClipVertex *endVertex, GfxRasterClipVertex *outVertex, GfxRasterClipScratch *thresholds)
{
    float dyEdge;
    float workStartV;
    float width;
    float divisor;
    float vEnd;
    GfxVertexTextured *startExt;
    GfxVertexTextured *endSlot;
    float startU;
    float endA;
    float ratio;
    float planeNumer;

    switch (plane) {
    case GFX_CLIP_PLANE_MIN_X:
    case GFX_CLIP_PLANE_MAX_X:
        width = endVertex->clipX - startVertex->clipX;
        dyEdge = endVertex->clipY - startVertex->clipY;
        outVertex->clipX = thresholds->planeThreshold[plane];
        if (width == 0.0f) {
            outVertex->clipY = startVertex->clipY;
        } else {
            outVertex->clipY =
                (outVertex->clipX - startVertex->clipX) * dyEdge / width + startVertex->clipY;
        }
        break;
    case GFX_CLIP_PLANE_MIN_Y:
    case GFX_CLIP_PLANE_MAX_Y:
        width = endVertex->clipX - startVertex->clipX;
        dyEdge = endVertex->clipY - startVertex->clipY;
        outVertex->clipY = thresholds->planeThreshold[plane];
        if (dyEdge == 0.0f) {
            outVertex->clipX = startVertex->clipX;
        } else {
            outVertex->clipX =
                (outVertex->clipY - startVertex->clipY) * width / dyEdge +
                startVertex->clipX;
        }
        break;
    case GFX_CLIP_PLANE_MIN_Z:
    case GFX_CLIP_PLANE_MAX_Z:
        startExt = (GfxVertexTextured *)startVertex->extPtr;
        if (startExt != 0) {
            startU = startExt->texU;
            workStartV = startExt->texV;
        } else {
            startU = startVertex->startU;
            workStartV = startVertex->startV;
        }
        endSlot = (GfxVertexTextured *)endVertex->endSlot;
        if (endSlot != 0) {
            endA = endSlot->texU;
            vEnd = endSlot->texV;
        } else {
            endA = endVertex->endA;
            vEnd = endVertex->vEnd;
        }
        divisor = endVertex->clipZ - startVertex->clipCoord;
        outVertex->clipCoord = thresholds->planeThreshold[plane];
        if (divisor == 0.0f) {
            outVertex->x = startVertex->x;
            outVertex->y = startVertex->y;
            outVertex->startU = startU;
            outVertex->clipAttr0 = workStartV;
            outVertex->clipAttr1 = startVertex->clipAttr1;
            outVertex->clipAttr2 = startVertex->clipAttr2;
            outVertex->clipAttr3 = startVertex->clipAttr3;
            outVertex->clipAttr4 = startVertex->clipAttr4;
        } else {
            width = endVertex->x - startVertex->x;
            dyEdge = endVertex->y - startVertex->y;
            ratio = (planeNumer = outVertex->clipCoord - startVertex->clipCoord) / divisor;
            outVertex->x = width * ratio + startVertex->x;
            outVertex->y = dyEdge * ratio + startVertex->y;
            outVertex->startU = (endA - startU) * ratio + startU;
            outVertex->clipAttr0 = (vEnd - workStartV) * ratio + workStartV;
            outVertex->clipAttr1 =
                (endVertex->clipAttr1 - startVertex->clipAttr1) * ratio +
                startVertex->clipAttr1;
            outVertex->clipAttr2 =
                (endVertex->clipAttr2 - startVertex->clipAttr2) * ratio +
                startVertex->clipAttr2;
            outVertex->clipAttr3 =
                (endVertex->clipAttr3 - startVertex->clipAttr3) * ratio +
                startVertex->clipAttr3;
            outVertex->clipAttr4 = startVertex->clipAttr4;
        }
        break;
    }
}

/*
 * Appends an edge end vertex when both endpoints pass the selected clipping test.
 * For a crossing edge, initializes a scratch vertex and invokes the interpolation
 * helper, appending that intersection and, for an entering edge, the end vertex.
 */
// FUNCTION: FF7 0x006accd8
int __cdecl Gfx_RasterClipEdgeToPlane(int plane, int count, void *outList, GfxRasterEdgeVertex *startVertex, SwClipVertex *endVertex, GfxRasterClipScratch *context)
{
    GfxRasterClipVertex *intersection;

    if (Gfx_RasterIsVertexInsideClipPlane(plane, (SwClipVertex *)startVertex, context) != 0) {
        if (Gfx_RasterIsVertexInsideClipPlane(plane, endVertex, context) != 0) {
            ((int *)outList)[count] = (int)endVertex;
            count++;
        } else {
            intersection = &context->planeVertices[plane][count];
            ((int *)outList)[count] = (int)intersection;
            intersection->forcedInside = 0;
            intersection->resolved = 0;
            Gfx_RasterIntersectEdgeWithClipPlane(plane, startVertex, endVertex, intersection, context);
            count++;
        }
    } else if (Gfx_RasterIsVertexInsideClipPlane(plane, endVertex, context) != 0) {
        intersection = &context->planeVertices[plane][count];
        ((int *)outList)[count] = (int)intersection;
        intersection->forcedInside = 0;
        intersection->resolved = 0;
        Gfx_RasterIntersectEdgeWithClipPlane(plane, startVertex, endVertex, intersection, context);
        count++;
        ((int *)outList)[count] = (int)endVertex;
        count++;
    }
    return count;
}

/*
 * Initializes a three-vertex polygon and sequentially clips it against selected
 * mask planes through maxPlane inclusive, applying the edge helper to every adjacent
 * pair and the closing edge. Stops after an empty polygon, then stores its final
 * vertex count and list pointer in the scratch context.
 */
// FUNCTION: FF7 0x006ace26
void __cdecl Gfx_RasterClipTriangleToPlaneMask(int planeMask, GfxRasterVertex *vertexA, GfxRasterVertex *vertexB, GfxRasterVertex *vertexC, int maxPlane, GfxRasterClipScratch *clipCtx)
{
    int prevCnt;
    int plane;
    int edgeIndex;
    int *destinationList;
    int bit;
    int vertCount;
    int *inList;

    clipCtx->maxPlane = maxPlane;
    clipCtx->vertexA = (int)vertexA;
    clipCtx->vertexB = (int)vertexB;
    clipCtx->vertexC = (int)vertexC;
    clipCtx->vertCount = 0;
    vertCount = 3;
    inList = &clipCtx->vertexA;
    destinationList = inList;
    bit = 1;
    for (plane = 0; plane <= maxPlane; plane++) {
        if ((planeMask & bit) != 0) {
            prevCnt = vertCount;
            vertCount = 0;
            destinationList = clipCtx->planeLists[plane];
            for (edgeIndex = 0; edgeIndex < prevCnt - 1; edgeIndex++) {
                vertCount = Gfx_RasterClipEdgeToPlane(plane, vertCount, destinationList, (GfxRasterEdgeVertex *)inList[edgeIndex], (SwClipVertex *)inList[edgeIndex + 1], clipCtx);
            }
            vertCount = Gfx_RasterClipEdgeToPlane(plane, vertCount, destinationList, (GfxRasterEdgeVertex *)inList[prevCnt - 1], (SwClipVertex *)inList[0], clipCtx);
            inList = destinationList;
            if (vertCount <= 0) {
                break;
            }
        }
        bit <<= 1;
    }
    clipCtx->vertCount = vertCount;
    clipCtx->destinationList = (int)destinationList;
}

/*
 * Transforms the three vertex source positions through the context matrix into
 * their transformed-position fields, then clips the triangle sequentially against every plane
 * from GFX_CLIP_PLANE_MIN_Z through maxPlane. Stops after an empty polygon and stores the resulting
 * count/list pointer in the supplied scratch context.
 */
// FUNCTION: FF7 0x006acf74
void __cdecl Gfx_RasterTransformAndClipTriangleDepthPlanes(GfxRasterVertex *vertexA, GfxRasterVertex *vertexB, GfxRasterVertex *vertexC, int maxPlane, GfxRasterClipScratch *scratch, GfxRasterContext *context)
{
    int prevCnt;
    int plane;
    int edgeIndex;
    int *destinationList;
    int mtxSet;
    int vertCount;
    int *inList;

    mtxSet = context->mtxSet;
    scratch->maxPlane = maxPlane;
    scratch->vertexA = (int)vertexA;
    scratch->vertexB = (int)vertexB;
    scratch->vertexC = (int)vertexC;
    scratch->vertCount = 0;
    TransformPoint3RowVec((void *)*(int *)((char *)mtxSet + 0x2C), (void *)vertexA->srcPtr, (char *)vertexA + 0x14);
    TransformPoint3RowVec((void *)*(int *)((char *)mtxSet + 0x2C), (void *)vertexB->srcPtr, (char *)vertexB + 0x14);
    TransformPoint3RowVec((void *)*(int *)((char *)mtxSet + 0x2C), (void *)vertexC->srcPtr, (char *)vertexC + 0x14);
    vertCount = 3;
    inList = &scratch->vertexA;
    destinationList = inList;
    for (plane = GFX_CLIP_PLANE_MIN_Z; plane <= maxPlane; plane++) {
        prevCnt = vertCount;
        vertCount = 0;
        destinationList = scratch->planeLists[plane];
        for (edgeIndex = 0; edgeIndex < prevCnt - 1; edgeIndex++) {
            vertCount = Gfx_RasterClipEdgeToPlane(plane, vertCount, destinationList, (GfxRasterEdgeVertex *)inList[edgeIndex], (SwClipVertex *)inList[edgeIndex + 1], scratch);
        }
        vertCount = Gfx_RasterClipEdgeToPlane(plane, vertCount, destinationList, (GfxRasterEdgeVertex *)inList[prevCnt - 1], (SwClipVertex *)inList[0], scratch);
        inList = destinationList;
        if (vertCount <= 0) {
            break;
        }
    }
    scratch->vertCount = vertCount;
    scratch->destinationList = (int)destinationList;
}
} /* extern "C" */
