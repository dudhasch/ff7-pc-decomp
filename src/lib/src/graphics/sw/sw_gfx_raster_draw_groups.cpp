// src/lib/src/graphics/sw/sw_gfx_raster_draw_groups.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Draw entry points of the software rasterizer: submits indexed transformed triangles and
//   vertex groups with or without a matrix, copies vertex records into the triangle workspace, and
//   draws depth meshes.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/api/graphics.h"
#include "lib/types/graphics.h"
#include "lib/enums/graphics.h"
extern "C" {
// GLOBAL: FF7 0x00919514
unsigned short g_GfxSingleTriangleIndices[3] = {0, 1, 2};

/*
 * Transforms the vertices (with the assembly or the C helper, per useAsm), then draws triCount
 * triangles given by 16-bit indices into the 96-byte transformed records, clipping those that
 * need it and culling back faces unless the preset asks to keep them. Returns early when every
 * vertex is rejected.
 */
// FUNCTION: FF7 0x006b1d60
void __cdecl Gfx_RasterDrawIndexedTransformedTriangles(int useAsm, int srcVertices, int indices, int triCount, int vertexCount, GfxEngineContext *context)
{
    GfxRasterContext *raster;
    int renderer;
    GfxRasterVertexBuffer *indexStatus;
    int cfg;
    int pipeline;
    int base;
    int idxA;
    GfxRasterVertex *secondPtr;
    GfxRasterVertex *v0;
    int left;
    GfxRasterCallbacks *hooks;
    GfxRasterVertex *vertexC;
    int idxB;
    int backFace;
    int mainIdx;

    renderer = (*(int *)&context->rasterBase);
    raster = (GfxRasterContext *)(renderer + 0x38);
    indexStatus = (GfxRasterVertexBuffer *)(*(int *)((char *)renderer + 0x548FC));
    pipeline = *(int *)((char *)renderer + 0x54A60);
    *(int *)((char *)pipeline + 0x144) = indexStatus->depthOffset;
    if (useAsm != 0) {
        Gfx_RasterPrepareVertexRecordsAsm(srcVertices, vertexCount, (int)indexStatus, raster);
    } else {
        Gfx_RasterPrepareVertexRecords(srcVertices, (SwLineVertexRecord *)vertexCount, (struct GfxRasterVertexBuffer *)indexStatus, raster);
    }
    if (indexStatus->clipAnd != 0) {
        return;
    }
    hooks = (GfxRasterCallbacks *)((int)raster + 0x154);
    base = indexStatus->alignedVertices;
    cfg = (*(int *)&context->presetPolygonSets);
    backFace = *(unsigned char *)((char *)cfg + 0xE);
    if (indexStatus->clipFlags != 0) {
        for (left = triCount; left > 0; left--) {
            idxA = *(unsigned short *)indices;
            indices += 2;
            v0 = (GfxRasterVertex *)(base + idxA * 0x60);
            idxB = *(unsigned short *)indices;
            indices += 2;
            secondPtr = (GfxRasterVertex *)(base + idxB * 0x60);
            mainIdx = *(unsigned short *)indices;
            indices += 2;
            vertexC = (GfxRasterVertex *)(base + mainIdx * 0x60);
            if (v0->flags != 0 && secondPtr->flags != 0 &&
                vertexC->flags != 0) {
                if ((v0->x - secondPtr->x) *
                        (vertexC->y - secondPtr->y) <
                    (v0->y - secondPtr->y) *
                        (vertexC->x - secondPtr->x)) {
                    hooks->drawTriangle((int)v0, (int)secondPtr, (int)vertexC, (int)hooks,
                                                                                     (int)raster);
                } else if (backFace != 0) {
                    hooks->drawTriangle((int)vertexC, (int)secondPtr, (int)v0, (int)hooks,
                                                                                     (int)raster);
                }
            } else {
                Gfx_RasterClipAndDrawTriangle(useAsm, backFace, v0, secondPtr, vertexC, hooks, raster);
            }
        }
    } else {
        for (left = triCount; left > 0; left--) {
            idxA = *(unsigned short *)indices;
            indices += 2;
            v0 = (GfxRasterVertex *)(base + idxA * 0x60);
            idxB = *(unsigned short *)indices;
            indices += 2;
            secondPtr = (GfxRasterVertex *)(base + idxB * 0x60);
            mainIdx = *(unsigned short *)indices;
            indices += 2;
            vertexC = (GfxRasterVertex *)(base + mainIdx * 0x60);
            if ((v0->x - secondPtr->x) *
                    (vertexC->y - secondPtr->y) <
                (v0->y - secondPtr->y) *
                    (vertexC->x - secondPtr->x)) {
                hooks->drawTriangle((int)v0, (int)secondPtr, (int)vertexC, (int)hooks, (int)raster);
            } else if (backFace != 0) {
                hooks->drawTriangle((int)v0, (int)vertexC, (int)secondPtr, (int)hooks, (int)raster);
            }
        }
    }
}

/*
 * Draws one draw group with a matrix applied: sets up the texture mask state and scale pair when
 * the group has a texture, selects the vertex callbacks, multiplies the matrix and draws the
 * indexed triangles with the group's counts.
 */
// FUNCTION: FF7 0x006b2074
void __cdecl Gfx_RasterDrawGroupWithMatrix(int value, GfxPolygonBatch *batch, GfxPolygonSet *drawGroup, GlDrawGroup *glGroup, GfxRasterDrawGroup *rasterGroup, GfxEngineContext *context)
{
    int tableBase;
    GfxRasterDrawRecordA *record;
    GfxRasterVertexBuffer *targetObj;
    int primitiveCount;
    int indexCnt;
    int valA;
    GfxRasterSetupBlock *maskState;
    GfxRasterTextureMaskA *mask;
    GfxTextureSurface *drawGroup2Ptr;
    int valB;
    int vertArray;
    int renderState;
    GfxRasterGroupSetup *groupState;

    renderState = (*(int *)&context->rasterBase);
    tableBase = renderState + 0x38;
    targetObj = (GfxRasterVertexBuffer *)(*(int *)((char *)renderState + 0x548FC));
    groupState = (GfxRasterGroupSetup *)((*(int *)&context->presetPolygonSets));
    drawGroup2Ptr = (GfxTextureSurface *)(groupState->resource);
    if (drawGroup2Ptr != 0) {
        maskState = (GfxRasterSetupBlock *)(*(int *)((char *)renderState + 0x54A60));
        mask = (GfxRasterTextureMaskA *)drawGroup2Ptr->header;
        valA = *(int *)((char *)tableBase + mask->tableIndexA * 4 + 0x27B8);
        valB = *(int *)((char *)tableBase + mask->tableIndexB * 4 + 0x27B8);
        if (((GfxPixelFormat *)drawGroup2Ptr->format)->palettized != 0) {
            Gfx_RasterSetTextureMaskSetupRows(drawGroup2Ptr, maskState);
        } else {
            Gfx_RasterSetTextureMaskSetup(drawGroup2Ptr, maskState);
        }
        Gfx_RasterSetStateScalePair(valA, valB, targetObj);
    }
    Gfx_RasterSelectGeneratedVertexCallbacks(drawGroup->renderFlags, groupState, (GfxRasterVertexCallbacks *)(tableBase + 0x154));
    if (rasterGroup != 0) {
        primitiveCount = rasterGroup->primitiveCount;
        indexCnt = rasterGroup->elementCount;
    } else {
        record = (GfxRasterDrawRecordA *)drawGroup->source;
        primitiveCount = record->primitiveCount;
        indexCnt = record->indexCount;
    }
    vertArray = batch->vertices;
    MultiplyMatrix4x4ByTranspose((void *)value, (char *)context + 0x8D0, (void *)(renderState + 0x27B0));
    Gfx_RasterSetStateMatrixPair(value, renderState + 0x27B0, (GfxRasterVertexBuffer *)targetObj);
    Gfx_RasterDrawIndexedTransformedTriangles(1, indexCnt, batch->indices, primitiveCount, vertArray, context);
}

/*
 * The same as Gfx_RasterDrawGroupWithMatrix without the matrix step, drawing with useAsm 0.
 */
// FUNCTION: FF7 0x006b21f9
void __cdecl Gfx_RasterDrawGroupWithoutMatrix(GfxPolygonBatch *batch, GfxPolygonSet *drawGroup, GlDrawGroup *glGroup, GfxRasterDrawGroup *rasterGroup, GfxEngineContext *context)
{
    int tableBase;
    GfxRasterDrawRecordB *record;
    int primitiveCount;
    int valA;
    int indexSum;
    GfxRasterVertexBuffer *drawObj;
    GfxRasterSetupBlock *maskState;
    GfxRasterTextureMaskA *mask;
    GfxTextureSurface *drawGroup2Ptr;
    int valB;
    int vertArray;
    int renderState;
    GfxRasterGroupSetup *groupState;

    renderState = (*(int *)&context->rasterBase);
    tableBase = renderState + 0x38;
    drawObj = (GfxRasterVertexBuffer *)(*(int *)((char *)renderState + 0x548FC));
    groupState = (GfxRasterGroupSetup *)((*(int *)&context->presetPolygonSets));
    drawGroup2Ptr = (GfxTextureSurface *)(groupState->resource);
    if (drawGroup2Ptr != 0) {
        maskState = (GfxRasterSetupBlock *)(*(int *)((char *)renderState + 0x54A60));
        mask = (GfxRasterTextureMaskA *)drawGroup2Ptr->header;
        valA = *(int *)((char *)tableBase + mask->tableIndexA * 4 + 0x27B8);
        valB = *(int *)((char *)tableBase + mask->tableIndexB * 4 + 0x27B8);
        if (((GfxPixelFormat *)drawGroup2Ptr->format)->palettized != 0) {
            Gfx_RasterSetTextureMaskSetupRows(drawGroup2Ptr, maskState);
        } else {
            Gfx_RasterSetTextureMaskSetup(drawGroup2Ptr, maskState);
        }
        Gfx_RasterSetStateScalePair(valA, valB, drawObj);
    }
    Gfx_RasterSelectGeneratedVertexCallbacks(drawGroup->renderFlags, groupState, (GfxRasterVertexCallbacks *)(tableBase + 0x154));
    if (rasterGroup != 0) {
        primitiveCount = rasterGroup->primitiveCount;
        indexSum = rasterGroup->elementCount;
    } else {
        record = (GfxRasterDrawRecordB *)drawGroup->source;
        primitiveCount = record->primitiveCount;
        indexSum = record->indexSum;
    }
    vertArray = batch->vertices;
    Gfx_RasterDrawIndexedTransformedTriangles(0, indexSum, batch->indices, primitiveCount, vertArray, context);
}

/*
 * For transform modes 0, 1 and 2, copies three caller vertex records into a 96-byte local
 * triangle block (adding the transform's offset when it is enabled), with their diffuse color and
 * UVs, and draws it.
 */
// FUNCTION: FF7 0x006b2344
void __cdecl Gfx_RasterSubmitTriangleVertices(int vertexA, int vertexB, int vertexC, SwRasterTransform *transform, GfxEngineContext *context)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[124];
        unsigned char triangleBlock[96];
        struct { unsigned char pad_to_60[0x60]; int triangleBase; };
        struct { unsigned char pad_to_64[0x64]; int vertexIndex; };
        struct { unsigned char pad_to_68[0x68]; int outVertex; };
        struct { unsigned char pad_to_6C[0x6C]; GfxRasterSubmitVertex *srcVertex; };
        struct { unsigned char pad_to_70[0x70]; int sourcePtrs[3]; };
        struct {
            unsigned char pad[116];
            int value;
        } at8;
        struct {
            unsigned char pad[120];
            int value;
        } at4;
    } frame;
#pragma pack(pop)

    frame.sourcePtrs[0] = vertexA;
    frame.sourcePtrs[1] = vertexB;
    frame.sourcePtrs[2] = vertexC;
    frame.outVertex = (int)frame.triangleBlock;
    frame.triangleBase = (int)frame.triangleBlock;
    switch (transform->mode) {
    case GFX_POLYGON_SET_TRANSFORM_NONE:
    case GFX_POLYGON_SET_TRANSFORM_NODE:
    case GFX_POLYGON_SET_TRANSFORM_NODE_CAMERA:
        for (frame.vertexIndex = 0; frame.vertexIndex < 3; frame.vertexIndex++) {
            frame.srcVertex = (GfxRasterSubmitVertex *)(frame.sourcePtrs[frame.vertexIndex]);
            if (transform->enabled != 0) {
                ((GfxRasterSubmitVertex *)frame.outVertex)->x = frame.srcVertex->x + transform->offsetX;
                ((GfxRasterSubmitVertex *)frame.outVertex)->y = frame.srcVertex->y + transform->offsetY;
                ((GfxRasterSubmitVertex *)frame.outVertex)->z = frame.srcVertex->z + transform->offsetZ;
            } else {
                ((GfxExecPosition *)frame.outVertex)->x = *(int *)&frame.srcVertex->x;
                ((GfxExecPosition *)frame.outVertex)->y = *(int *)&frame.srcVertex->y;
                ((GfxExecPosition *)frame.outVertex)->z = *(int *)&frame.srcVertex->z;
            }
            ((GfxRasterSubmitVertex *)frame.outVertex)->packedColor = frame.srcVertex->packedColor;
            ((GfxRasterSubmitVertex *)frame.outVertex)->u = frame.srcVertex->u;
            ((GfxRasterSubmitVertex *)frame.outVertex)->v = frame.srcVertex->v;
            frame.outVertex += 0x20;
        }
        Gfx_RasterDrawIndexedTransformedTriangles(1, 3, (int)&g_GfxSingleTriangleIndices, 1, frame.triangleBase, context);
    }
}

/*
 * Prepares the renderer workspace and the texture state, takes the triangle count from rasterGroup
 * or the draw group's source record, combines value with the context matrix and draws every
 * triangle of the mesh through Gfx_RasterDrawTriangleDepth.
 */
// FUNCTION: FF7 0x006b2464
void __cdecl Gfx_RasterDrawIndexedDepthMesh(int value, GfxPolygonBatch *mesh, GfxPolygonSet *drawGroup, GlDrawGroup *glGroup, GfxRasterDrawGroup *rasterGroup, SwRasterTransform *depthValue, GfxEngineContext *context)
{
    SwRenderCallbacks *tableBase;
    GfxRasterDrawRecordC *record;
    int renderState;
    GfxRasterGroupSetup *groupSetupPtr;
    int prevCallback;
    GfxRasterTextureMaskA *mask;
    int valA;
    GfxTextureSurface *drawGroup2Ptr;
    GfxRasterSetupBlock *maskState;
    int idxLimit;
    int valB;
    GfxRasterVertexBuffer *stateObj;
    int primitiveCnt;
    int remaining;
    int firstVert;
    int idxPointer;
    int indexB;
    int pickedIndex;
    int idx0;
    int height;
    int ae;
    int vertexB;

    renderState = (*(int *)&context->rasterBase);
    tableBase = (SwRenderCallbacks *)(renderState + 0x38);
    stateObj = (GfxRasterVertexBuffer *)(*(int *)((char *)renderState + 0x548FC));
    groupSetupPtr = (GfxRasterGroupSetup *)((*(int *)&context->presetPolygonSets));
    maskState = (GfxRasterSetupBlock *)(*(int *)((char *)renderState + 0x54A60));
    maskState->depthOffset = stateObj->depthOffset;
    drawGroup2Ptr = (GfxTextureSurface *)(groupSetupPtr->resource);
    if (drawGroup2Ptr != 0) {
        mask = (GfxRasterTextureMaskA *)drawGroup2Ptr->header;
        valA = *(int *)((char *)tableBase + mask->tableIndexA * 4 + 0x27B8);
        valB = *(int *)((char *)tableBase + mask->tableIndexB * 4 + 0x27B8);
        if (((GfxPixelFormat *)drawGroup2Ptr->format)->palettized != 0) {
            Gfx_RasterSetTextureMaskSetupRows(drawGroup2Ptr, (GfxRasterSetupBlock *)maskState);
        } else {
            Gfx_RasterSetTextureMaskSetup(drawGroup2Ptr, (GfxRasterSetupBlock *)maskState);
        }
        Gfx_RasterSetStateScalePair(valA, valB, (GfxRasterVertexBuffer *)stateObj);
    }
    Gfx_RasterSelectGeneratedVertexCallbacks(drawGroup->renderFlags, groupSetupPtr, (GfxRasterVertexCallbacks *)((int)tableBase + 0x154));
    if (rasterGroup != 0) {
        primitiveCnt = rasterGroup->primitiveCount;
        idxLimit = rasterGroup->elementCount;
    } else {
        record = (GfxRasterDrawRecordC *)drawGroup->source;
        primitiveCnt = record->primitiveCount;
        idxLimit = record->indexLimit;
    }
    MultiplyMatrix4x4ByTranspose((void *)value, (char *)context + 0x8D0, (void *)(renderState + 0x27B0));
    Gfx_RasterSetStateModelMatrix(value, (struct GfxRasterVertexBuffer *)stateObj);
    Gfx_RasterSetStateCombinedMatrix(renderState + 0x27B0, (struct GfxRasterVertexBuffer *)stateObj);
    prevCallback = tableBase->triangleCallback;
    tableBase->triangleCallback = (int)Gfx_RasterDrawTriangleDepth;
    tableBase->depthValue = (int)depthValue;
    ae = mesh->vertices;
    idxPointer = mesh->indices;
    for (remaining = primitiveCnt; remaining > 0; remaining--) {
        idx0 = *(unsigned short *)idxPointer;
        idxPointer += 2;
        firstVert = ae + idx0 * 0x20;
        indexB = *(unsigned short *)idxPointer;
        idxPointer += 2;
        vertexB = ae + indexB * 0x20;
        pickedIndex = *(unsigned short *)idxPointer;
        idxPointer += 2;
        height = ae + pickedIndex * 0x20;
        Gfx_RasterSubmitTriangleVertices(firstVert, vertexB, height, depthValue, context);
    }
    tableBase->triangleCallback = prevCallback;
}
} /* extern "C" */
