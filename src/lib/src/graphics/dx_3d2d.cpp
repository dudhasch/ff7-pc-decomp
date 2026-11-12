// src/lib/src/graphics/dx_3d2d.cpp
// Original: C:\lib\src\graphics\dx_3d2d.cpp
// Contents: Maintains the depth-sorted draw queue for 2D and 3D primitives: creating, inserting
//   into, flushing and dispatching its sorted records.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include <stdio.h>
#include <windows.h>
#include "lib/crt.h"
#include "lib/api/graphics.h"
#include "lib/api/mem.h"
#include "lib/api/sort.h"
#include "lib/types/graphics.h"
#include "lib/types/mem.h"
#include "lib/types/sort.h"
#include "lib/types/polygon.h"
#include "lib/enums/graphics.h"
#include "lib/codegen.h"
#include "lib/com_abi.h"

// Native COM slot views preserve the original SDK5 calling convention.
extern "C" {
// clang-format off
// GLOBAL: FF7 0x00d8e488
struct GfxGroupRenderState *g_GfxCurrentRenderState = 0;
unsigned char g_bssPad_00d8e48c[4] = {0};
// clang-format on

 // extern "C"





/*
 * Returns whether (a[0]-b[0])*(c[1]-b[1]) is at most (a[1]-b[1])*(c[0]-b[0]).
 */
// FUNCTION: FF7 0x00664e90
int __cdecl Gfx_IsEdgeCrossNonPositive2d(float *ptA, float *ptB, float *ptC)
{
    int val;
    if ((ptA[0] - ptB[0]) * (ptC[1] - ptB[1]) <= (ptA[1] - ptB[1]) * (ptC[0] - ptB[0])) {
        val = 1;
    } else {
        val = 0;
    }
    return val;
}

/*
 * Draws one sorted screen primitive: restores its texture if the surface was lost, applies its render
 * state when it differs from the current one, then issues the indexed draw, retrying while the
 * device is still drawing.
 */
// FUNCTION: FF7 0x00664ee4
void __cdecl Gfx_DrawSortedScreenPrimitive(GfxSortedDrawRecord *drawRecord)
{
    GfxEngineContext *ctx;
    int driver;
    GfxGroupRenderState *rs;
    int callResult;
    GfxTextureSurface *timTex;
    GfxScreenPrimitiveItem *item;
    int res;

    ctx = GetEngineContext();
    driver = ctx->d3dDevice2;
    rs = drawRecord->renderState;
    if (rs != 0) {
        timTex = rs->texture;
        if (timTex != 0 && timTex->surface != 0) {
            callResult = ((ComDirectDrawSurface *)(timTex->surface))->IsLost();
            if (callResult == DDERR_SURFACELOST) {
                rs->texture = Gfx_RestoreTextureSurfaces(timTex);
                if (rs->texture == 0) {
                }
            }
            Gfx_SelectTexturePage(drawRecord->texturePage, rs);
        }
        if (g_GfxCurrentRenderState != rs) {
            g_GfxCurrentRenderState = rs;
            Gfx_ApplyRenderStateCall((int)rs, ctx);
        }
    }
    item = drawRecord->item;
    do {
        res = ((ComDirect3DDevice2 *)(driver))->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, D3DVT_TLVERTEX, (int)drawRecord + 0x70, item->vertexCount, item->indices, item->indexCount, item->flags);
        switch (res) {
        case DD_OK:
            break;
        case DDERR_WASSTILLDRAWING:
            D3D5CountWasStillDrawing(1, ctx);
            break;
        default:
            // STRING: FF7 0x0090c86c
            CheckDirectDrawResult(res, "C:\\lib\\src\\graphics\\dx_3d2d.cpp", 0x7B);
            res = 0;
        }
    } while (res != 0);
}

/*
 * Draws one sorted group record: restores its texture if the surface was lost, applies its render
 * state when it differs from the current one, sets the next record's world transform, then issues
 * the group's indexed draw, retrying while the device is still drawing.
 */
// FUNCTION: FF7 0x00665018
void __cdecl Gfx_DrawSortedGroupRecord(GfxSortedGroupRecord *groupRecord)
{
    int texRef;
    int statusObj;
    GfxGroupRenderState *nextRenderState;
    GfxPolygonBatch *pDrawRec;
    GfxGroupRenderState *texState;
    GfxEngineContext *gfxCtx;
    int driverObj;
    GfxTextureSurface *texturePtr;
    int hr;
    int code;

    gfxCtx = GetEngineContext();
    driverObj = gfxCtx->d3dDevice2;
    statusObj = groupRecord->statusObj;
    pDrawRec = groupRecord->drawRecord;
    texState = groupRecord->renderState;
    nextRenderState = groupRecord[1].renderState;
    if (groupRecord[1].texturePage != 0) {
        texRef = groupRecord[1].texturePage;
    } else {
        texRef = (int)groupRecord + 0x1C;
    }
    if (texState != 0) {
        texturePtr = texState->texture;
        if (texturePtr != 0 && texturePtr->surface != 0) {
            hr = ((ComDirectDrawSurface *)(texturePtr->surface))->IsLost();
            if (hr == DDERR_SURFACELOST) {
                texState->texture = Gfx_RestoreTextureSurfaces(texturePtr);
                if (texState->texture == 0) {
                }
            }
            Gfx_SelectTexturePage(groupRecord->texturePage, texState);
        }
        if (g_GfxCurrentRenderState != texState) {
            g_GfxCurrentRenderState = texState;
            Gfx_ApplyRenderStateCall((int)texState, gfxCtx);
        }
    }
    if (pDrawRec != 0) {
        if (nextRenderState != 0 && nextRenderState->usesWorldTransform != 0) {
            ((ComDirect3DDevice2 *)(driverObj))->SetTransform(D3DTRANSFORMSTATE_WORLD, texRef);
        }
        if (*(int *)((char *)statusObj + 4) == 0) {
            pDrawRec->flags |= D3DDP_DONOTCLIP;
        } else {
            pDrawRec->flags &= ~D3DDP_DONOTCLIP;
        }
        do {
            code = ((ComDirect3DDevice2 *)(driverObj))->DrawIndexedPrimitive(pDrawRec->primType, pDrawRec->vertexType, pDrawRec->vertices, pDrawRec->vertexCount, pDrawRec->indices, pDrawRec->indexCount, pDrawRec->flags);
            switch (code) {
            case DD_OK:
                break;
            case DDERR_WASSTILLDRAWING:
                D3D5CountWasStillDrawing(1, gfxCtx);
                break;
            default:
                // STRING: FF7 0x0090c88c
                CheckDirectDrawResult(code, "C:\\lib\\src\\graphics\\dx_3d2d.cpp", 0x12A);
                code = 0;
            }
        } while (code != 0);
    }
}

/*
 * Draws one sorted triangle record: restores its texture if the surface was lost, applies its render
 * state when it differs from the current one, then draws its three vertices, retrying while the
 * device is still drawing.
 */
// FUNCTION: FF7 0x006651df
void __cdecl Gfx_DrawSortedTriangleRecord(GfxSortedTriangleRecord *triangleRecord)
{
    GfxEngineContext *ctx;
    int driver;
    GfxGroupRenderState *rs;
    GfxTextureSurface *texturePtr;
    int hr;
    int code;

    ctx = GetEngineContext();
    driver = ctx->d3dDevice2;
    rs = triangleRecord->renderState;
    if (rs != 0) {
        texturePtr = rs->texture;
        if (texturePtr != 0 && texturePtr->surface != 0) {
            hr = ((ComDirectDrawSurface *)(texturePtr->surface))->IsLost();
            if (hr == DDERR_SURFACELOST) {
                rs->texture = Gfx_RestoreTextureSurfaces(texturePtr);
                if (rs->texture == 0) {
                }
            }
            Gfx_SelectTexturePage(triangleRecord->texturePage, rs);
        }
        if (g_GfxCurrentRenderState != rs) {
            g_GfxCurrentRenderState = rs;
            Gfx_ApplyRenderStateCall((int)rs, ctx);
        }
    }
    do {
        code = ((ComDirect3DDevice2 *)(driver))->DrawPrimitive(D3DPT_TRIANGLELIST, D3DVT_TLVERTEX, (int)triangleRecord + 0x70, 3, 8);
        switch (code) {
        case DD_OK:
            break;
        case DDERR_WASSTILLDRAWING:
            D3D5CountWasStillDrawing(1, ctx);
            break;
        default:
            // STRING: FF7 0x0090c8ac
            CheckDirectDrawResult(code, "C:\\lib\\src\\graphics\\dx_3d2d.cpp", 0x19A);
            code = 0;
        }
    } while (code != 0);
}

/*
 * Draws one sorted primitive-batch record: restores the texture of its polygon set's group state
 * if the surface was lost, applies that state when it differs from the current one, then issues
 * the batch's indexed draw, retrying while the device is still drawing.
 */
// FUNCTION: FF7 0x006652f5
void __cdecl Gfx_DrawSortedPrimitiveBatchRecord(GfxSortedBatchRecord *batchRecord)
{
    GfxEngineContext *ctx;
    GfxGroupRenderState *rs;
    int driverObj;
    GfxRenderObject *batch;
    GfxTextureSurface *texturePtr;
    int hr;
    int code;

    ctx = GetEngineContext();
    driverObj = ctx->d3dDevice2;
    batch = batchRecord->batch;
    rs = (GfxGroupRenderState *)((GfxPolygonSet *)batch->polygonSet)->groupState;
    if (rs != 0) {
        texturePtr = rs->texture;
        if (texturePtr != 0 && texturePtr->surface != 0) {
            hr = ((ComDirectDrawSurface *)(texturePtr->surface))->IsLost();
            if (hr == DDERR_SURFACELOST) {
                rs->texture = Gfx_RestoreTextureSurfaces(texturePtr);
                if (rs->texture == 0) {
                }
            }
            Gfx_SelectTexturePage(batchRecord->texturePage, rs);
        }
        if (g_GfxCurrentRenderState != rs) {
            g_GfxCurrentRenderState = rs;
            Gfx_ApplyRenderStateCall((int)rs, ctx);
        }
    }
    do {
        code = ((ComDirect3DDevice2 *)(driverObj))->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, D3DVT_TLVERTEX, batchRecord->primData, batch->verticesPerPrim, batchRecord->indices, batch->indicesPerPrim, 8);
        switch (code) {
        case DD_OK:
            break;
        case DDERR_WASSTILLDRAWING:
            D3D5CountWasStillDrawing(1, ctx);
            break;
        default:
            // STRING: FF7 0x0090c8cc
            CheckDirectDrawResult(code, "C:\\lib\\src\\graphics\\dx_3d2d.cpp", 0x20E);
            code = 0;
        }
    } while (code != 0);
}

/*
 * Dispatches one sorted-draw record by its state: 0 and 1 are screen primitives, 2 and 3
 * primitive batches, 4 a group and 5 a triangle. The paired cases are separate in the image.
 */
// FUNCTION: FF7 0x0066542a
void __cdecl Gfx_Draw2DRecordD3D(GfxSortedPrimitive *drawRecord)
{
    GfxSortedPrimitive *recordCopy;

    recordCopy = drawRecord;
    switch (recordCopy->state) {
    case GFX_SORTED_RECORD_TRIANGLE:
        Gfx_DrawSortedScreenPrimitive((GfxSortedDrawRecord *)recordCopy);
        break;
    case GFX_SORTED_RECORD_QUAD:
        Gfx_DrawSortedScreenPrimitive((GfxSortedDrawRecord *)recordCopy);
        break;
    case GFX_SORTED_RECORD_BATCH_TRIANGLES:
        Gfx_DrawSortedPrimitiveBatchRecord((GfxSortedBatchRecord *)recordCopy);
        break;
    case GFX_SORTED_RECORD_BATCH_QUADS:
        Gfx_DrawSortedPrimitiveBatchRecord((GfxSortedBatchRecord *)recordCopy);
        break;
    case GFX_SORTED_RECORD_GROUP:
        Gfx_DrawSortedGroupRecord((GfxSortedGroupRecord *)recordCopy);
        break;
    case GFX_SORTED_RECORD_GROUP_TRIANGLE:
        Gfx_DrawSortedTriangleRecord((GfxSortedTriangleRecord *)recordCopy);
    }
}

/*
 * Destroys a sorted draw queue: its block-heap chain at +0x20, its bucket table at +0x24, then the
 * queue. Null is accepted.
 */
// FUNCTION: FF7 0x006654bc
void __cdecl Gfx_DestroySortedDrawQueue(GfxSortedDrawQueue *drawQueue)
{
    if (drawQueue != 0) {
        DestroyBlockHeapChain((MemBlockHeap *)(drawQueue->blockHeap));
        Sort_DestroyBucketTable((SortBucketTable *)(drawQueue->bucketTable));
        // STRING: FF7 0x0090c8ec
        GameHeapFree(drawQueue, "C:\\lib\\src\\graphics\\dx_3d2d.cpp", 0x257);
    }
}

/*
 * Creates a depth-sorted draw queue: a 0x28-byte queue with depthRange and bucketCount (also kept
 * as floats), a block heap of blockCount 0xF0-byte blocks and a bucketCount-bucket sort table.
 * Returns the queue, or 0 after destroying what was made when an allocation failed.
 */
// FUNCTION: FF7 0x006654fb
GfxSortedDrawQueue *__cdecl Gfx_CreateSortedDrawQueue(int blockCount, int depthRange, int bucketCount)
{
    GfxSortedDrawQueue *queue;
    int error;

    error = 1;
    // STRING: FF7 0x0090c90c
    queue = (GfxSortedDrawQueue *)GameHeapAlloc(1, 0x28, "C:\\lib\\src\\graphics\\dx_3d2d.cpp", 0x261);
    if (queue != 0) {
        queue->setRenderState = 1;
        queue->depthRange = depthRange;
        queue->depthRangeFloat = (float)depthRange;
        queue->bucketCount = bucketCount;
        queue->bucketCountFloat = (float)bucketCount;
        queue->blockHeap = CreateBlockHeap(0xF0, blockCount * 0xF0, 0);
        if (queue->blockHeap != 0) {
            queue->bucketTable = (int)Sort_CreateBucketTable(bucketCount, blockCount);
            if (queue->bucketTable != 0) {
                error = 0;
            }
        }
    }
    if (error != 0) {
        Gfx_DestroySortedDrawQueue(queue);
        queue = 0;
    }
    return queue;
}

/*
 * Returns AllocBlockArrayFromHeap(count, the queue's block heap), or zero when queue is null.
 */
// FUNCTION: FF7 0x006655c3
GfxSortedPrimitive *__cdecl Gfx_AllocSortedDrawRecords(int count, GfxSortedDrawQueue *queue)
{
    GfxSortedPrimitive *result;
    GfxSortedPrimitive *records;

    result = 0;
    if (queue != 0) {
        records = (GfxSortedPrimitive *)(AllocBlockArrayFromHeap(count, (MemBlockArrayHeap *)(queue->blockHeap)));
        result = records;
    }
    return result;
}

/*
 * For a nonnull drawQueue, clears its leading dword, calls ResetBlockHeapChain on its block heap,
 * then Sort_ResetBucketTable on its bucket table.
 */
// FUNCTION: FF7 0x006655f9
void __cdecl Gfx_ResetSortedDrawQueue(GfxSortedDrawQueue *drawQueue)
{
    if (drawQueue != 0) {
        drawQueue->clearedState = 0;
        ResetBlockHeapChain((MemBlockHeap *)(drawQueue->blockHeap));
        Sort_ResetBucketTable((SortBucketTable *)(drawQueue->bucketTable));
    }
}

// Flushes the sorted-draw queue for one graphics context and does nothing when the queue is null.
// The graphics driver object is fetched first. If setScenePass is set, the scene-pass call runs; if
// setRenderState is set, the depth-test render-state call runs. When the sort table is present, the
// function clears g_GfxCurrentRenderState and walks the table in order with Sort_ForEachInOrder,
// using the driver's draw2DRecord callback.
// FUNCTION: FF7 0x0066562b
void __cdecl Gfx_FlushSortedDrawQueue(GfxSortedDrawQueue *drawQueue, struct GfxEngineContext *context)
{
    GfxDriverOps *driverObj;

    if (drawQueue != 0) {
        driverObj = GetGraphicsDriverObject(context);
        if (drawQueue->setScenePass != 0) {
            Gfx_SetScenePassCall(1, (struct GfxEngineContext *)(context));
        }
        if (drawQueue->setRenderState != 0) {
            Gfx_SetRenderStateCall(RENDER_STATE_DEPTH_TEST, 1, (struct GfxEngineContext *)(context));
        }
        if (drawQueue->bucketTable != 0) {
            g_GfxCurrentRenderState = 0;
            Sort_ForEachInOrder((SortBucketTable *)(drawQueue->bucketTable), driverObj->draw2DRecord);
        }
    }
}

/*
 * When context is nonnull and sortedDrawEnabled is set, calls Gfx_ResetSortedDrawQueue with
 * depthSortQueue and context.
 */
// FUNCTION: FF7 0x006656a4
void __cdecl Gfx_BeginSortedDrawFrame(struct GfxEngineContext *context)
{
    if (context != 0 && context->sortedDrawEnabled != 0) {
        /* Retail caller passes 2 arg(s) to 1-parameter `Gfx_ResetSortedDrawQueue` (__cdecl stack cleanup matches retail). */
        ((void(__cdecl *)(int, int))Gfx_ResetSortedDrawQueue)((int)context->depthSortQueue, (int)context);
    }
}

/*
 * When context is nonnull and sortedDrawEnabled is set, calls Gfx_FlushSortedDrawQueue with
 * depthSortQueue and context.
 */
// FUNCTION: FF7 0x006656d1
void __cdecl Gfx_EndSortedDrawFrame(struct GfxEngineContext *context)
{
    if (context != 0 && context->sortedDrawEnabled != 0) {
        Gfx_FlushSortedDrawQueue(context->depthSortQueue, context);
    }
}

/*
 * Insert a depth record using x87 current-rounding conversion for bucket and sort key.
 */
// FUNCTION: FF7 0x006656fe
int __cdecl Gfx_InsertSortedDrawRecord(float depth, GfxSortedPrimitive *record, GfxSortedDrawQueue *queue)
{
    int result, bucketIndex, sortKey;
    float bucketFloat, sortVal;
    GfxSortedPrimitive *nodePtr;

    result = 0;
    depth = 1.0f - depth;
    bucketFloat = depth * queue->bucketCountFloat;
    /* Retail rounds via bare x87 fistp rather than VC5's truncating _ftol. */
    __asm { fld bucketFloat }
    __asm {fistp bucketIndex} bucketIndex--;
    if (bucketIndex >= 0 && bucketIndex < queue->bucketCount) {
        sortVal = depth * queue->depthRangeFloat;
        __asm { fld sortVal }
        __asm {fistp sortKey} record->sortDepth = sortVal;
        nodePtr = record;
        Sort_InsertNode((SortBucketTable *)(queue->bucketTable), sortKey, bucketIndex, (int)nodePtr);
        result = 1;
    }
    return result;
}

/*
 * Enqueues draw work only when context depth sorting is enabled. Whole-group mode allocates one
 * record, derives a sort depth from matrixRef or inlineMatrix and fills type 4 plus draw references
 * and an optional copied/identity matrix after insertion succeeds. Per-triangle mode allocates the
 * triangle count, transforms group vertices into cached projected coordinates and queues type 5
 * records only for positive projection W and the accepted winding, using average vertex depth. The
 * record pointer advances only after successful insertion. Null allocation or failed
 * composition/insertion skips work. A null inlineMatrix with nonnull matrixRef leaves the inline
 * matrix uncopied because the external matrix reference is retained.
 */
// FUNCTION: FF7 0x00665793
void __cdecl Gfx_EnqueueGroupForDepthSort(int inlineMatrix, int matrixRef, GfxPolygonBatch *batch, int group, int renderState, int rasterGroup, struct GfxEngineContext *context)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[196];
        float avgDepth;
        struct { unsigned char pad_to_4[0x4]; GfxSortVertex *vertexB; };
        struct { unsigned char pad_to_8[0x8]; GfxDepthSortVertex *vertexA; };
        struct { unsigned char pad_to_C[0xC]; GfxDepthSortEntryA *entryB; };
        struct { unsigned char pad_to_10[0x10]; GfxDepthSortEntryA *entryA; };
        struct { unsigned char pad_to_14[0x14]; int projectedPtr; };
        struct { unsigned char pad_to_18[0x18]; int indexPtr; };
        struct { unsigned char pad_to_1C[0x1C]; GfxSortVertex *vertexC; };
        struct { unsigned char pad_to_20[0x20]; GfxHomogeneousVec4 transformed; };
        struct { unsigned char pad_to_30[0x30]; GfxDepthSortEntryA *entryC; };
        struct { unsigned char pad_to_34[0x34]; int faceRemaining; };
        struct { unsigned char pad_to_38[0x38]; GfxMatrix4x4 screenMatrix; };
        struct { unsigned char pad_to_78[0x78]; int triMatrix; };
        struct { unsigned char pad_to_7C[0x7C]; int tmpVertexIndex; };
        struct { unsigned char pad_to_80[0x80]; int faceCount; };
        struct { unsigned char pad_to_84[0x84]; int vertexCount; };
        struct { unsigned char pad_to_88[0x88]; int vertexPtr; };
        struct { unsigned char pad_to_8C[0x8C]; int vertexCache; };
        struct { unsigned char pad_to_90[0x90]; GfxFloat4 *groupEntries; };
        struct {
            unsigned char pad[148];
            int value;
        } at48;
        struct { unsigned char pad_to_9C[0x9C]; int screenDepth; };
        struct { unsigned char pad_to_A4[0xA4]; int modelPoint; };
        struct { unsigned char pad_to_B4[0xB4]; int wholeDepth; };
        struct { unsigned char pad_to_B8[0xB8]; int groupMatrix; };
        struct { unsigned char pad_to_BC[0xBC]; GfxSortedPrimitive *sortRecord; };
        struct { unsigned char pad_to_C0[0xC0]; GfxSortedDrawQueue *sortList; };
    } frame;
#pragma pack(pop)

    if (context->sortedDrawEnabled != 0) {
        frame.sortList = context->depthSortQueue;
        if (((GfxGroupRenderState *)renderState)->sortPerTriangle == 0) {
            frame.sortRecord = Gfx_AllocSortedDrawRecords(1, frame.sortList);
            if (frame.sortRecord != 0) {
                if (matrixRef != 0) {
                    frame.groupMatrix = matrixRef;
                } else {
                    frame.groupMatrix = inlineMatrix;
                }
                ProjectModelPointToScreen(frame.groupMatrix, &frame.modelPoint, (struct GfxHomogeneousVec43 *)(&frame.at48.value));
                frame.wholeDepth = frame.screenDepth;
                if (Gfx_InsertSortedDrawRecord(*(float *)&frame.wholeDepth, frame.sortRecord, frame.sortList) != 0) {
                    frame.sortRecord->state = GFX_SORTED_RECORD_GROUP;
                    frame.sortRecord->group = group;
                    frame.sortRecord->batch = (int)batch;
                    frame.sortRecord->texturePage = 0;
                    frame.sortRecord->renderState = renderState;
                    frame.sortRecord->rasterGroup = rasterGroup;
                    frame.sortRecord->matrixRef = matrixRef;
                    if (inlineMatrix != 0) {
                        frame.sortRecord->inlineMatrix = *(GfxMatrix4x4 *)(inlineMatrix);
                    } else if (matrixRef == 0) {
                        SetIdentityMatrix4x4(&frame.sortRecord->inlineMatrix);
                    }
                    frame.sortRecord->polyRecords = ((GfxPolygonSet *)group)->matrices;
                }
            }
        } else {
            if (rasterGroup != 0) {
                frame.faceCount = ((GfxRasterDrawGroup *)rasterGroup)->primitiveCount;
                frame.vertexCount = ((GfxRasterDrawGroup *)rasterGroup)->elementCount;
                frame.vertexPtr = ((GfxPolygonData *)(((GfxPolygonSet *)group)->source))->vertexBase + ((GfxRasterDrawGroup *)rasterGroup)->vertexStart * 0xC;
                frame.groupEntries = (GfxFloat4 *)(((GfxPolygonSet *)group)->elements + ((GfxRasterDrawGroup *)rasterGroup)->vertexStart * 0x14);
            } else {
                frame.faceCount = ((GfxPolygonData *)(((GfxPolygonSet *)group)->source))->faceCount;
                frame.vertexCount = ((GfxPolygonData *)(((GfxPolygonSet *)group)->source))->vertexCount;
                frame.vertexPtr = ((GfxPolygonData *)(((GfxPolygonSet *)group)->source))->vertexBase;
                frame.groupEntries = (GfxFloat4 *)(((GfxPolygonSet *)group)->elements);
            }
            frame.vertexCache = batch->vertices;
            frame.sortRecord = Gfx_AllocSortedDrawRecords(frame.faceCount, frame.sortList);
            if (frame.sortRecord != 0) {
                if (matrixRef != 0) {
                    frame.triMatrix = matrixRef;
                } else {
                    frame.triMatrix = inlineMatrix;
                }
                if (((GfxPolygonSet *)group)->elements != 0) {
                    if (ComposeModelToScreenMatrix((void *)frame.triMatrix, &frame.screenMatrix) != 0) {
                        frame.projectedPtr = (int)frame.groupEntries;
                        for (frame.tmpVertexIndex = frame.vertexCount; frame.tmpVertexIndex > 0; frame.tmpVertexIndex--) {
                            TransformPointByMatrix4x4(&frame.screenMatrix, (void *)(frame.vertexPtr), &frame.transformed.x);
                            PerspectiveDivideRecipW(&frame.transformed, (struct GfxRecipWVec4 *)frame.projectedPtr);
                            frame.vertexPtr += 0xC;
                            frame.projectedPtr += 0x14;
                        }
                        frame.indexPtr = batch->indices;
                        for (frame.faceRemaining = frame.faceCount; frame.faceRemaining > 0; frame.faceRemaining--) {
                            frame.tmpVertexIndex = *(unsigned short *)frame.indexPtr;
                            frame.indexPtr += 2;
                            frame.vertexA = (GfxDepthSortVertex *)((int)frame.groupEntries + frame.tmpVertexIndex * 0x14);
                            frame.entryA = (GfxDepthSortEntryA *)(frame.vertexCache + (frame.tmpVertexIndex << 5));
                            frame.tmpVertexIndex = *(unsigned short *)frame.indexPtr;
                            frame.indexPtr += 2;
                            frame.vertexB = (GfxSortVertex *)((int)frame.groupEntries + frame.tmpVertexIndex * 0x14);
                            frame.entryB = (GfxDepthSortEntryA *)(frame.vertexCache + (frame.tmpVertexIndex << 5));
                            frame.tmpVertexIndex = *(unsigned short *)frame.indexPtr;
                            frame.indexPtr += 2;
                            frame.vertexC = (GfxSortVertex *)((int)frame.groupEntries + frame.tmpVertexIndex * 0x14);
                            frame.entryC = (GfxDepthSortEntryA *)(frame.vertexCache + (frame.tmpVertexIndex << 5));
                            if (frame.vertexA->clipW > 0.0 && frame.vertexB->w > 0.0 && frame.vertexC->w > 0.0 && (frame.vertexA->posX - frame.vertexB->screenX) * (frame.vertexC->screenY - frame.vertexB->screenY) <= (frame.vertexA->posY - frame.vertexB->screenY) * (frame.vertexC->screenX - frame.vertexB->screenX)) {
                                frame.avgDepth = (frame.vertexA->depth + frame.vertexB->depth + frame.vertexC->depth) * 0.33333334f;
                                if (Gfx_InsertSortedDrawRecord(frame.avgDepth, frame.sortRecord, frame.sortList) != 0) {
                                    frame.sortRecord->state = GFX_SORTED_RECORD_GROUP_TRIANGLE;
                                    frame.sortRecord->batch = (int)batch;
                                    frame.sortRecord->texturePage = 0;
                                    frame.sortRecord->renderState = renderState;
                                    frame.sortRecord->vertex0X = *(int *)&frame.vertexA->posX;
                                    frame.sortRecord->vertex0Y = *(int *)&frame.vertexA->posY;
                                    frame.sortRecord->vertex0Z = *(int *)&frame.vertexA->depth;
                                    frame.sortRecord->vertex0Rhw = frame.vertexA->recipW;
                                    frame.sortRecord->vertex0Color = frame.entryA->color;
                                    frame.sortRecord->vertex0Specular = frame.entryA->specular;
                                    frame.sortRecord->vertex0U = frame.entryA->texU;
                                    frame.sortRecord->vertex0V = frame.entryA->texV;
                                    frame.sortRecord->vertex1X = *(int *)&frame.vertexB->screenX;
                                    frame.sortRecord->vertex1Y = *(int *)&frame.vertexB->screenY;
                                    frame.sortRecord->vertex1Z = *(int *)&frame.vertexB->depth;
                                    frame.sortRecord->vertex1Rhw = frame.vertexB->recipW;
                                    frame.sortRecord->vertex1Color = frame.entryB->color;
                                    frame.sortRecord->vertex1Specular = frame.entryB->specular;
                                    frame.sortRecord->vertex1U = frame.entryB->texU;
                                    frame.sortRecord->vertex1V = frame.entryB->texV;
                                    frame.sortRecord->vertex2X = *(int *)&frame.vertexC->screenX;
                                    frame.sortRecord->vertex2Y = *(int *)&frame.vertexC->screenY;
                                    frame.sortRecord->vertex2Z = *(int *)&frame.vertexC->depth;
                                    frame.sortRecord->vertex2Rhw = frame.vertexC->recipW;
                                    frame.sortRecord->vertex2Color = frame.entryC->color;
                                    frame.sortRecord->vertex2Specular = frame.entryC->specular;
                                    frame.sortRecord->vertex2U = frame.entryC->texU;
                                    frame.sortRecord->vertex2V = frame.entryC->texV;
                                    frame.sortRecord++;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

/*
 * Allocates one sorted record when context sorting is enabled and dispatches the render object's
 * GfxDrawKind. The 2D kinds (flat, Gouraud and textured triangles and quads, and the sprite batch)
 * copy already projected triangle/quad data using the first vertex depth; the 3D triangle and quad
 * kinds transform three/four vertices, require positive projection W and accepted winding, then
 * sort by average depth. Successful insertions store draw references, primitive flags,
 * diffuse/texture attributes and black specular color in GFX_SORTED_RECORD_TRIANGLE or _QUAD records.
 * Unsupported types, allocation/composition failures and rejected winding produce no populated
 * draw record. Extra is stored in a local but otherwise unused.
 */
// FUNCTION: FF7 0x00665d9a
void __cdecl Gfx_EnqueuePsxPrimitiveForDepthSort(int model, int extra, int diffuse, int texAttr, GfxDrawListNode *node, struct GfxEngineContext *context)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[208];
        GfxDepthSortVertexA *secondVert;
        struct { unsigned char pad_to_4[0x4]; GfxDepthSortVertexA *firstVert; };
        struct { unsigned char pad_to_8[0x8]; float sortDepth; };
        struct { unsigned char pad_to_C[0xC]; GfxMatrix4x4 screenMatrix; };
        struct { unsigned char pad_to_4C[0x4C]; int srcVertex; };
        struct { unsigned char pad_to_50[0x50]; int primData; };
        struct { unsigned char pad_to_54[0x54]; int dstVert; };
        struct { unsigned char pad_to_58[0x58]; GfxPsxSortVertex *fourthVert; };
        struct { unsigned char pad_to_5C[0x5C]; GfxDepthSortVertexA *thirdVert; };
        struct { unsigned char pad_to_60[0x60]; GfxSortedPrimitive *drawRecord; };
        struct { unsigned char pad_to_64[0x64]; int vertsLeft; };
        struct { unsigned char pad_to_68[0x68]; GfxSortedDrawQueue *sortList; };
        struct { unsigned char pad_to_6C[0x6C]; GfxHomogeneousVec4 homogVec; };
        struct { unsigned char pad_to_7C[0x7C]; int extraArg; };
        struct { unsigned char pad_to_80[0x80]; int projFirst; };
        struct { unsigned char pad_to_94[0x94]; int projSecond; };
        struct { unsigned char pad_to_A8[0xA8]; int projThird; };
        struct { unsigned char pad_to_BC[0xBC]; int projFourth; };
    } frame;
#pragma pack(pop)

    if (context->sortedDrawEnabled != 0) {
        frame.sortList = context->depthSortQueue;
        frame.drawRecord = Gfx_AllocSortedDrawRecords(1, frame.sortList);
        if (frame.drawRecord != 0) {
            frame.srcVertex = (int)node + 0xC;
            frame.extraArg = extra;
            frame.primData = node->nextNode;
            switch (*(int *)frame.primData) {
            case GFX_DRAW_FLAT_TRI_2D:
            case GFX_DRAW_GOURAUD_TRI_2D:
            case GFX_DRAW_TEXTURED_TRI_2D:
                *(int *)&frame.sortDepth = node->vertex0Z;
                if (Gfx_InsertSortedDrawRecord(frame.sortDepth, frame.drawRecord, frame.sortList) != 0) {
                    frame.drawRecord->state = GFX_SORTED_RECORD_TRIANGLE;
                    frame.drawRecord->batch = diffuse;
                    frame.drawRecord->texturePage = node->page;
                    frame.drawRecord->renderState = texAttr;
                    frame.drawRecord->vertex0X = node->vertex0X;
                    frame.drawRecord->vertex0Y = node->drawEnabled;
                    frame.drawRecord->vertex0Z = node->vertex0Z;
                    frame.drawRecord->vertex0Rhw = node->vertex0Rhw;
                    frame.drawRecord->vertex0Color = node->vertex0Color;
                    frame.drawRecord->vertex0Specular = 0xFF000000;
                    frame.drawRecord->vertex0U = node->vertex0U;
                    frame.drawRecord->vertex0V = node->vertex0V;
                    frame.drawRecord->vertex1X = node->vertex1X;
                    frame.drawRecord->vertex1Y = node->vertex1Y;
                    frame.drawRecord->vertex1Z = node->vertex1Z;
                    frame.drawRecord->vertex1Rhw = node->vertex1Rhw;
                    frame.drawRecord->vertex1Color = node->vertex1Color;
                    frame.drawRecord->vertex1Specular = 0xFF000000;
                    frame.drawRecord->vertex1U = node->vertex1U;
                    frame.drawRecord->vertex1V = node->vertex1V;
                    frame.drawRecord->vertex2X = node->vertex2X;
                    frame.drawRecord->vertex2Y = node->vertex2Y;
                    frame.drawRecord->vertex2Z = node->vertex2Z;
                    frame.drawRecord->vertex2Rhw = node->vertex2Rhw;
                    frame.drawRecord->vertex2Color = node->vertex2Color;
                    frame.drawRecord->vertex2Specular = 0xFF000000;
                    frame.drawRecord->vertex2U = node->vertex2U;
                    frame.drawRecord->vertex2V = node->vertex2V;
                    frame.drawRecord->vertex3X = node->vertex3X;
                    frame.drawRecord->vertex3Y = node->vertex3Y;
                    frame.drawRecord->vertex3Z = node->vertex3Z;
                    frame.drawRecord->vertex3Rhw = node->vertex3Rhw;
                    frame.drawRecord->vertex3Color = node->vertex3Color;
                    frame.drawRecord->vertex3Specular = 0xFF000000;
                    frame.drawRecord->vertex3U = node->vertex3UOrCallback;
                    frame.drawRecord->vertex3V = node->callbackArg;
                }
                break;
            case GFX_DRAW_FLAT_QUAD_2D:
            case GFX_DRAW_GOURAUD_QUAD_2D:
            case GFX_DRAW_TEXTURED_QUAD_2D:
            case GFX_DRAW_SPRITE_BATCH:
                *(int *)&frame.sortDepth = node->vertex0Z;
                if (Gfx_InsertSortedDrawRecord(frame.sortDepth, frame.drawRecord, frame.sortList) != 0) {
                    frame.drawRecord->state = GFX_SORTED_RECORD_QUAD;
                    frame.drawRecord->batch = diffuse;
                    frame.drawRecord->texturePage = node->page;
                    frame.drawRecord->renderState = texAttr;
                    frame.drawRecord->vertex0X = node->vertex0X;
                    frame.drawRecord->vertex0Y = node->drawEnabled;
                    frame.drawRecord->vertex0Z = node->vertex0Z;
                    frame.drawRecord->vertex0Rhw = node->vertex0Rhw;
                    frame.drawRecord->vertex0Color = node->vertex0Color;
                    frame.drawRecord->vertex0Specular = 0xFF000000;
                    frame.drawRecord->vertex0U = node->vertex0U;
                    frame.drawRecord->vertex0V = node->vertex0V;
                    frame.drawRecord->vertex1X = node->vertex1X;
                    frame.drawRecord->vertex1Y = node->vertex1Y;
                    frame.drawRecord->vertex1Z = node->vertex1Z;
                    frame.drawRecord->vertex1Rhw = node->vertex1Rhw;
                    frame.drawRecord->vertex1Color = node->vertex1Color;
                    frame.drawRecord->vertex1Specular = 0xFF000000;
                    frame.drawRecord->vertex1U = node->vertex1U;
                    frame.drawRecord->vertex1V = node->vertex1V;
                    frame.drawRecord->vertex2X = node->vertex2X;
                    frame.drawRecord->vertex2Y = node->vertex2Y;
                    frame.drawRecord->vertex2Z = node->vertex2Z;
                    frame.drawRecord->vertex2Rhw = node->vertex2Rhw;
                    frame.drawRecord->vertex2Color = node->vertex2Color;
                    frame.drawRecord->vertex2Specular = 0xFF000000;
                    frame.drawRecord->vertex2U = node->vertex2U;
                    frame.drawRecord->vertex2V = node->vertex2V;
                    frame.drawRecord->vertex3X = node->vertex3X;
                    frame.drawRecord->vertex3Y = node->vertex3Y;
                    frame.drawRecord->vertex3Z = node->vertex3Z;
                    frame.drawRecord->vertex3Rhw = node->vertex3Rhw;
                    frame.drawRecord->vertex3Color = node->vertex3Color;
                    frame.drawRecord->vertex3Specular = 0xFF000000;
                    frame.drawRecord->vertex3U = node->vertex3UOrCallback;
                    frame.drawRecord->vertex3V = node->callbackArg;
                }
                break;
            case GFX_DRAW_FLAT_TRI_3D:
            case GFX_DRAW_GOURAUD_TRI_3D:
            case GFX_DRAW_TEXTURED_TRI_3D:
                if (ComposeModelToScreenMatrix((void *)model, &frame.screenMatrix) != 0) {
                    frame.dstVert = (int)(&frame.projFirst);
                    for (frame.vertsLeft = 3; frame.vertsLeft > 0; frame.vertsLeft--) {
                        TransformPointByMatrix4x4(&frame.screenMatrix, (void *)(frame.srcVertex), &frame.homogVec.x);
                        PerspectiveDivideRecipW(&frame.homogVec, (struct GfxRecipWVec4 *)frame.dstVert);
                        frame.srcVertex += 0xC;
                        frame.dstVert += 0x14;
                    }
                    frame.firstVert = (GfxDepthSortVertexA *)(&frame.projFirst);
                    frame.secondVert = (GfxDepthSortVertexA *)(&frame.projSecond);
                    frame.thirdVert = (GfxDepthSortVertexA *)(&frame.projThird);
                    if (frame.firstVert->clipW > 0.0 && frame.secondVert->clipW > 0.0 && frame.thirdVert->clipW > 0.0 && (frame.firstVert->x - frame.secondVert->x) * (frame.thirdVert->y - frame.secondVert->y) >= (frame.firstVert->y - frame.secondVert->y) * (frame.thirdVert->x - frame.secondVert->x)) {
                        frame.sortDepth = (frame.firstVert->z + frame.secondVert->z + frame.thirdVert->z) * 0.33333334f;
                        if (Gfx_InsertSortedDrawRecord(frame.sortDepth, frame.drawRecord, frame.sortList) != 0) {
                            frame.drawRecord->state = GFX_SORTED_RECORD_TRIANGLE;
                            frame.drawRecord->batch = diffuse;
                            frame.drawRecord->texturePage = node->page;
                            frame.drawRecord->renderState = texAttr;
                            frame.drawRecord->vertex0X = *(int *)&frame.firstVert->x;
                            frame.drawRecord->vertex0Y = *(int *)&frame.firstVert->y;
                            frame.drawRecord->vertex0Z = *(int *)&frame.firstVert->z;
                            frame.drawRecord->vertex0Rhw = frame.firstVert->recipW;
                            frame.drawRecord->vertex0Color = node->vertex0Color;
                            frame.drawRecord->vertex0Specular = 0xFF000000;
                            frame.drawRecord->vertex0U = node->vertex0U;
                            frame.drawRecord->vertex0V = node->vertex0V;
                            frame.drawRecord->vertex1X = *(int *)&frame.secondVert->x;
                            frame.drawRecord->vertex1Y = *(int *)&frame.secondVert->y;
                            frame.drawRecord->vertex1Z = *(int *)&frame.secondVert->z;
                            frame.drawRecord->vertex1Color = node->vertex1Color;
                            frame.drawRecord->vertex1Specular = 0xFF000000;
                            frame.drawRecord->vertex1U = node->vertex1U;
                            frame.drawRecord->vertex1V = node->vertex1V;
                            frame.drawRecord->vertex1Rhw = frame.secondVert->recipW;
                            frame.drawRecord->vertex2X = *(int *)&frame.thirdVert->x;
                            frame.drawRecord->vertex2Y = *(int *)&frame.thirdVert->y;
                            frame.drawRecord->vertex2Z = *(int *)&frame.thirdVert->z;
                            frame.drawRecord->vertex2Rhw = frame.thirdVert->recipW;
                            frame.drawRecord->vertex2Color = node->vertex2Color;
                            frame.drawRecord->vertex2Specular = 0xFF000000;
                            frame.drawRecord->vertex2U = node->vertex2U;
                            frame.drawRecord->vertex2V = node->vertex2V;
                        }
                    }
                }
                break;
            case GFX_DRAW_FLAT_QUAD_3D:
            case GFX_DRAW_GOURAUD_QUAD_3D:
            case GFX_DRAW_TEXTURED_QUAD_3D:
                if (ComposeModelToScreenMatrix((void *)model, &frame.screenMatrix) != 0) {
                    frame.dstVert = (int)(&frame.projFirst);
                    for (frame.vertsLeft = 4; frame.vertsLeft > 0; frame.vertsLeft--) {
                        TransformPointByMatrix4x4(&frame.screenMatrix, (void *)(frame.srcVertex), &frame.homogVec.x);
                        PerspectiveDivideRecipW(&frame.homogVec, (struct GfxRecipWVec4 *)frame.dstVert);
                        frame.srcVertex += 0xC;
                        frame.dstVert += 0x14;
                    }
                    frame.firstVert = (GfxDepthSortVertexA *)(&frame.projFirst);
                    frame.secondVert = (GfxDepthSortVertexA *)(&frame.projSecond);
                    frame.thirdVert = (GfxDepthSortVertexA *)(&frame.projThird);
                    frame.fourthVert = (GfxPsxSortVertex *)(&frame.projFourth);
                    if (frame.firstVert->clipW > 0.0 && frame.secondVert->clipW > 0.0 && frame.thirdVert->clipW > 0.0 && frame.fourthVert->clipW > 0.0 && (frame.firstVert->x - frame.secondVert->x) * (frame.thirdVert->y - frame.secondVert->y) >= (frame.firstVert->y - frame.secondVert->y) * (frame.thirdVert->x - frame.secondVert->x)) {
                        frame.sortDepth = (frame.firstVert->z + frame.secondVert->z + frame.thirdVert->z + frame.fourthVert->z) * 0.25f;
                        if (Gfx_InsertSortedDrawRecord(frame.sortDepth, frame.drawRecord, frame.sortList) != 0) {
                            frame.drawRecord->state = GFX_SORTED_RECORD_QUAD;
                            frame.drawRecord->batch = diffuse;
                            frame.drawRecord->texturePage = node->page;
                            frame.drawRecord->renderState = texAttr;
                            frame.drawRecord->vertex0X = *(int *)&frame.firstVert->x;
                            frame.drawRecord->vertex0Y = *(int *)&frame.firstVert->y;
                            frame.drawRecord->vertex0Z = *(int *)&frame.firstVert->z;
                            frame.drawRecord->vertex0Rhw = frame.firstVert->recipW;
                            frame.drawRecord->vertex0Color = node->vertex0Color;
                            frame.drawRecord->vertex0Specular = 0xFF000000;
                            frame.drawRecord->vertex0U = node->vertex0U;
                            frame.drawRecord->vertex0V = node->vertex0V;
                            frame.drawRecord->vertex1X = *(int *)&frame.secondVert->x;
                            frame.drawRecord->vertex1Y = *(int *)&frame.secondVert->y;
                            frame.drawRecord->vertex1Z = *(int *)&frame.secondVert->z;
                            frame.drawRecord->vertex1Rhw = frame.secondVert->recipW;
                            frame.drawRecord->vertex1Color = node->vertex1Color;
                            frame.drawRecord->vertex1Specular = 0xFF000000;
                            frame.drawRecord->vertex1U = node->vertex1U;
                            frame.drawRecord->vertex1V = node->vertex1V;
                            frame.drawRecord->vertex2X = *(int *)&frame.thirdVert->x;
                            frame.drawRecord->vertex2Y = *(int *)&frame.thirdVert->y;
                            frame.drawRecord->vertex2Z = *(int *)&frame.thirdVert->z;
                            frame.drawRecord->vertex2Rhw = frame.thirdVert->recipW;
                            frame.drawRecord->vertex2Color = node->vertex2Color;
                            frame.drawRecord->vertex2Specular = 0xFF000000;
                            frame.drawRecord->vertex2U = node->vertex2U;
                            frame.drawRecord->vertex2V = node->vertex2V;
                            frame.drawRecord->vertex3X = frame.fourthVert->x;
                            frame.drawRecord->vertex3Y = frame.fourthVert->y;
                            frame.drawRecord->vertex3Z = *(int *)&frame.fourthVert->z;
                            frame.drawRecord->vertex3Rhw = frame.fourthVert->recipW;
                            frame.drawRecord->vertex3Color = node->vertex3Color;
                            frame.drawRecord->vertex3Specular = 0xFF000000;
                            frame.drawRecord->vertex3U = node->vertex3UOrCallback;
                            frame.drawRecord->vertex3V = node->callbackArg;
                        }
                    }
                }
            }
        }
    }
}

/*
 * Queues every primitive of a render object's batch chain as sorted records, keyed by each
 * primitive's depth.
 */
// FUNCTION: FF7 0x0066695c
void __cdecl Gfx_EnqueuePrimitiveBatchForDepthSort(GfxRenderObject *batch, struct GfxEngineContext *context)
{
    GfxDepthSortNode *node;
    int colorSrc;
    GfxSortedDrawQueue *sortStatus;
    int state;
    GfxSortedPrimitive *record;
    int zero;
    int remaining;
    int primData;

    node = (GfxDepthSortNode *)(batch->chainHead);
    if (node != 0 && batch->beginEmitter != 0) {
        sortStatus = context->depthSortQueue;
        if (batch->kind <= 5) {
            state = GFX_SORTED_RECORD_BATCH_TRIANGLES;
        } else {
            state = GFX_SORTED_RECORD_BATCH_QUADS;
        }
        zero = 0;
        do {
            if (REG_ALLOC_HINT) {
            }
            record = Gfx_AllocSortedDrawRecords(node->recordCount, sortStatus);
            if (record != 0) {
                primData = node->primData;
                colorSrc = node->colorSrc;
                for (remaining = node->recordCount; remaining > 0; remaining--) {
                    if (Gfx_InsertSortedDrawRecord(*(float *)((char *)primData + 8), record, sortStatus) != 0) {
                        record->state = state;
                        if (batch->textureMode == 2) {
                            record->texturePage = *(unsigned char *)colorSrc;
                        } else {
                            record->texturePage = zero;
                        }
                        record->batchPtr = (int)batch;
                        record->primData = primData;
                        *(struct DirectWord *)((char *)record + 0x6C) = *(struct DirectWord *)((char *)node + 0x20);
                    }
                    primData += batch->verticesPerPrim << 5;
                    colorSrc++;
                    record++;
                }
            }
            node = (GfxDepthSortNode *)(node->nextNode);
        } while (node != 0);
    }
}
} /* extern "C" */
