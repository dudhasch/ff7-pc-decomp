// src/lib/src/graphics/dx_mesh.cpp
// Original: C:\lib\src\graphics\dx_mesh.cpp
// Contents: Draws primitives through Direct3D, retrying while the device is busy, and transforms
//   and draws a mesh group using the context's matrix.
/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/types/graphics.h"
#include "lib/enums/graphics.h"
#include "lib/codegen.h"
#include "lib/api/graphics.h"
#include <string.h>
#include "lib/crt.h"
#include "lib/com_abi.h"

extern "C" {

/*
 * Repeatedly attempts to draw non-indexed primitive until completion or error.
 */
// FUNCTION: FF7 0x006b26c0
void __cdecl DxMesh_DrawPrimitiveRetry(int vertices, ComDirect3DDevice2 *device, struct GfxEngineContext *context)
{
    int result;
    do {
        result = device->DrawPrimitive(4, 3, vertices, 3, 8);
        switch (result) {
        case DD_OK:
            break;
        case DDERR_WASSTILLDRAWING:
            break;
        default:
            // STRING: FF7 0x0091951c
            CheckDirectDrawResult(result, "C:\\lib\\src\\graphics\\dx_mesh.cpp", 0x2B);
            result = 0;
        }
    } while (result != 0);
}

/*
 * Repeatedly attempts to draw indexed primitive until completion or error,
 * tracking drawing busy count.
 */
// FUNCTION: FF7 0x006b2720
void __cdecl DxMesh_DrawIndexedPrimitiveRetry(GfxPolygonBatch *batch, ComDirect3DDevice2 *device, struct GfxEngineContext *context)
{
    int result;
    do {
        result = device->DrawIndexedPrimitive(batch->primType, 3, batch->vertices, batch->vertexCount, batch->indices, batch->indexCount, 8);
        switch (result) {
        case DD_OK:
            break;
        case DDERR_WASSTILLDRAWING:
            D3D5CountWasStillDrawing(1, context);
            break;
        default:
            // STRING: FF7 0x0091953c
            CheckDirectDrawResult(result, "C:\\lib\\src\\graphics\\dx_mesh.cpp", 0x58);
            result = 0;
        }
    } while (result != 0);
}

#pragma intrinsic(strlen)



// Transforms one draw group's vertices into the 32-byte records of batch->vertices and draws them.
// The matrix is combined with the context's transposed screen transform only when the matrix and
// the context's d3dDevice2 are both nonzero. The vertices come from drawGroup, or from the
// optional subrange when one is given. An optional offset is read from drawParams, and its mode
// keeps the depth, adds depthBias to it, or replaces it with depthBias. The original's
// constant-false triangle traversal is retained, and unreferenced padding slots preserve VC5 /Od stack layout.
// FUNCTION: FF7 0x006b27a9
void __cdecl DxMesh_TransformAndDrawGroup(int matrix, GfxPolygonBatch *batch, GfxPolygonSet *drawGroup, void *reserved, GfxRasterDrawGroup *subrange, GfxMeshDrawParams *drawParams, void *context)
{
    int fieldC18;
    int cValueRec;
    GfxMeshAttribRecordB *vertexPtrRecB;
    int attrOffB;
    int subIndexB;
    GfxHomogeneousPoint *workRecordC;
    GfxMeshAttribRecordA *recordPointerA;
    GfxMeshScreenVertex *pSecondVertPtr;
    GfxHomogeneousVec4 *taskClipPosC;
    int clipVectorA[4];
    GfxHomogeneousVec4 *nextClipPtrB;
    GfxHomogeneousPoint pointAX;
    int remainingPts;
    float invWOfs;
    GfxHomogeneousPoint *recEntryA;
    int thirdIdx;
    GfxHomogeneousPoint *recordBView;
    float widthInvPlain;
    int padGap; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    GfxMeshScreenVertex *screenPtrA;
    GfxHomogeneousVec4 *clipPosA;
    int clipVectorB[4];
    float widthInv;
    float invWidth;
    float recipC;
    GfxHomogeneousPoint xOffsetT;
    int curIndexAValue;
    float rcpSet;
    float invWidthB;
    int remainingTris;
    int stepMtx[16];
    GfxHomogeneousVec4 xClipValue;
    ComDirect3DDevice2 *device;
    int recRefA;
    int valueStrideB;
    GfxHomogeneousPoint xB4;
    int padSpace; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int valueSpotC;
    int attrB1C;
    int homogC[4];
    GfxHomogeneousPoint xC;
    float recipShiftPlain;
    int vtxCount;
    GfxMeshVertex *outPosition;
    GfxHomogeneousPoint *srcPointer;
    int idxPtr;
    float recipWA;
    GfxHomogeneousPoint *posBasePtr;
    int faceCount;
    int tagPtr;
    int fieldA18;
    GfxMeshAttribRecordC *recordPointerC;
    GfxGeometrySource *geomSource;
    DxMeshGroupRecord *recordArray;
    int recOffset1C;
    int screenVerts[4];
    int unusedValue; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int vertC[4];
    GfxMeshScreenVertex *vertPtrCPtr;
    int screenVertB[4];

    device = (ComDirect3DDevice2 *)((GfxEngineContext *)context)->d3dDevice2;
    if (device != 0 && matrix != 0) {
        geomSource = (GfxGeometrySource *)(drawGroup->source);
        if (subrange != 0) {
            faceCount = subrange->primitiveCount;
            vtxCount = subrange->elementCount;
            posBasePtr = (GfxHomogeneousPoint *)(geomSource->positionBase + subrange->vertexStart * 0xC);
            tagPtr = geomSource->tagBase + subrange->vertexStart * 4;
        } else {
            faceCount = drawGroup->polygonData->faceCount;
            vtxCount = drawGroup->polygonData->vertexCount;
            posBasePtr = (GfxHomogeneousPoint *)(geomSource->positionBase);
            tagPtr = geomSource->tagBase;
        }
        recordArray = (DxMeshGroupRecord *)(batch->vertices);
        MultiplyMatrix4x4ByTranspose((void *)matrix, (void *)((GfxEngineContext *)context)->screenTransformTransposed, stepMtx);
        if (DEAD_IN_RETAIL != 0) {
            clipPosA = (GfxHomogeneousVec4 *)(clipVectorA);
            nextClipPtrB = (GfxHomogeneousVec4 *)(clipVectorB);
            taskClipPosC = (GfxHomogeneousVec4 *)(homogC);
            screenPtrA = (GfxMeshScreenVertex *)(screenVerts);
            pSecondVertPtr = (GfxMeshScreenVertex *)(screenVertB);
            vertPtrCPtr = (GfxMeshScreenVertex *)(vertC);
            idxPtr = batch->indices;
            for (remainingTris = faceCount; remainingTris > 0; remainingTris--) {
                curIndexAValue = *(unsigned short *)idxPtr;
                idxPtr += 2;
                subIndexB = *(unsigned short *)idxPtr;
                idxPtr += 2;
                thirdIdx = *(unsigned short *)idxPtr;
                idxPtr += 2;
                recEntryA = (GfxHomogeneousPoint *)((int)recordArray + (curIndexAValue << 5));
                recordBView = (GfxHomogeneousPoint *)((int)recordArray + (subIndexB << 5));
                workRecordC = (GfxHomogeneousPoint *)((int)recordArray + (thirdIdx << 5));
                if (drawParams->offsetEnabled != 0) {
                    pointAX.x = recEntryA->x + drawParams->offsetX;
                    pointAX.y = recEntryA->y + drawParams->offsetY;
                    pointAX.z = recEntryA->z + drawParams->offsetZ;
                    xB4.x = recordBView->x + drawParams->offsetX;
                    xB4.y = recordBView->y + drawParams->offsetY;
                    xB4.z = recordBView->z + drawParams->offsetZ;
                    xC.x = workRecordC->x + drawParams->offsetX;
                    xC.y = workRecordC->y + drawParams->offsetY;
                    xC.z = workRecordC->z + drawParams->offsetZ;
                    TransformPointByMatrix4x4(stepMtx, &pointAX, clipPosA);
                    TransformPointByMatrix4x4(stepMtx, &xB4, nextClipPtrB);
                    TransformPointByMatrix4x4(stepMtx, &xC, taskClipPosC);
                } else {
                    TransformPointByMatrix4x4(stepMtx, recEntryA, clipPosA);
                    TransformPointByMatrix4x4(stepMtx, recordBView, nextClipPtrB);
                    TransformPointByMatrix4x4(stepMtx, workRecordC, taskClipPosC);
                }
                if (clipPosA->w > 0.0 && nextClipPtrB->w > 0.0 && taskClipPosC->w > 0.0) {
                    recipWA = 1.0f / clipPosA->w;
                    invWidthB = 1.0f / nextClipPtrB->w;
                    recipC = 1.0f / taskClipPosC->w;
                    screenPtrA->x = clipPosA->x * recipWA;
                    screenPtrA->y = clipPosA->y * recipWA;
                    screenPtrA->z = clipPosA->z * recipWA;
                    screenPtrA->scale = recipWA;
                    pSecondVertPtr->x = nextClipPtrB->x * invWidthB;
                    pSecondVertPtr->y = nextClipPtrB->y * invWidthB;
                    pSecondVertPtr->z = nextClipPtrB->z * invWidthB;
                    pSecondVertPtr->scale = invWidthB;
                    vertPtrCPtr->x = taskClipPosC->x * recipC;
                    vertPtrCPtr->y = taskClipPosC->y * recipC;
                    vertPtrCPtr->z = taskClipPosC->z * recipC;
                    vertPtrCPtr->scale = recipC;
                    switch ((char)drawParams->mode) {
                    case GFX_MESH_DEPTH_KEEP:
                        break;
                    case GFX_MESH_DEPTH_ADD_BIAS:
                        screenPtrA->z += drawParams->depthBias;
                        pSecondVertPtr->z += drawParams->depthBias;
                        vertPtrCPtr->z += drawParams->depthBias;
                        break;
                    case GFX_MESH_DEPTH_REPLACE:
                        *(int *)&screenPtrA->z = *(int *)&drawParams->depthBias;
                        *(int *)&pSecondVertPtr->z = *(int *)&drawParams->depthBias;
                        *(int *)&vertPtrCPtr->z = *(int *)&drawParams->depthBias;
                    }
                    if ((screenPtrA->x - pSecondVertPtr->x) * (vertPtrCPtr->y - pSecondVertPtr->y) <= (screenPtrA->y - pSecondVertPtr->y) * (vertPtrCPtr->x - pSecondVertPtr->x)) {
                        recordPointerA = (GfxMeshAttribRecordA *)((int)recordArray + (curIndexAValue << 5));
                        vertexPtrRecB = (GfxMeshAttribRecordB *)((int)recordArray + (subIndexB << 5));
                        recordPointerC = (GfxMeshAttribRecordC *)((int)recordArray + (thirdIdx << 5));
                        recRefA = recordPointerA->recRef;
                        fieldA18 = recordPointerA->texU;
                        recOffset1C = recordPointerA->offset;
                        attrOffB = vertexPtrRecB->attrOffset;
                        valueStrideB = vertexPtrRecB->valueStride;
                        attrB1C = vertexPtrRecB->texV;
                        valueSpotC = recordPointerC->valueRef;
                        fieldC18 = recordPointerC->texU;
                        cValueRec = recordPointerC->cValueRec;
                        DxMesh_DrawPrimitiveRetry((int)(screenVerts), device, (struct GfxEngineContext *)context);
                    }
                }
            }
        } else {
            srcPointer = (GfxHomogeneousPoint *)posBasePtr;
            outPosition = (GfxMeshVertex *)recordArray;
            switch ((char)drawParams->mode) {
            case GFX_MESH_DEPTH_KEEP:
                if (drawParams->offsetEnabled != 0) {
                    for (remainingPts = vtxCount; remainingPts > 0; remainingPts--) {
                        xOffsetT.x = srcPointer->x + drawParams->offsetX;
                        xOffsetT.y = srcPointer->y + drawParams->offsetY;
                        xOffsetT.z = srcPointer->z + drawParams->offsetZ;
                        TransformPointByMatrix4x4(stepMtx, &xOffsetT, &xClipValue);
                        if (xClipValue.w > 0.0f) {
                            widthInv = 1.0f / xClipValue.w;
                            outPosition->clipX = xClipValue.x * widthInv;
                            outPosition->clipY = xClipValue.y * widthInv;
                            outPosition->depth = xClipValue.z * widthInv;
                            outPosition->invW = widthInv;
                        } else {
                            outPosition->clipXBits = FLOAT_MINUS_ONE_BITS;
                            outPosition->clipYBits = FLOAT_MINUS_ONE_BITS;
                            outPosition->clipZ = FLOAT_MINUS_ONE_BITS;
                            outPosition->invWBits = FLOAT_MINUS_ONE_BITS;
                        }
                        outPosition++;
                        srcPointer++;
                    }
                } else {
                    for (remainingPts = vtxCount; remainingPts > 0; remainingPts--) {
                        TransformPointByMatrix4x4(stepMtx, srcPointer, &xClipValue);
                        if (xClipValue.w > 0.0f) {
                            widthInvPlain = 1.0f / xClipValue.w;
                            outPosition->clipX = xClipValue.x * widthInvPlain;
                            outPosition->clipY = xClipValue.y * widthInvPlain;
                            outPosition->depth = xClipValue.z * widthInvPlain;
                            outPosition->invW = widthInvPlain;
                        } else {
                            outPosition->clipXBits = FLOAT_MINUS_ONE_BITS;
                            outPosition->clipYBits = FLOAT_MINUS_ONE_BITS;
                            outPosition->clipZ = FLOAT_MINUS_ONE_BITS;
                            outPosition->invWBits = FLOAT_MINUS_ONE_BITS;
                        }
                        outPosition++;
                        srcPointer++;
                    }
                }
                break;
            case GFX_MESH_DEPTH_ADD_BIAS:
                if (drawParams->offsetEnabled != 0) {
                    for (remainingPts = vtxCount; remainingPts > 0; remainingPts--) {
                        xOffsetT.x = srcPointer->x + drawParams->offsetX;
                        xOffsetT.y = srcPointer->y + drawParams->offsetY;
                        xOffsetT.z = srcPointer->z + drawParams->offsetZ;
                        TransformPointByMatrix4x4(stepMtx, &xOffsetT, &xClipValue);
                        if (xClipValue.w > 0.0f) {
                            invWOfs = 1.0f / xClipValue.w;
                            outPosition->clipX = xClipValue.x * invWOfs;
                            outPosition->clipY = xClipValue.y * invWOfs;
                            outPosition->depth = xClipValue.z * invWOfs + drawParams->depthBias;
                            outPosition->invW = invWOfs;
                        } else {
                            outPosition->clipXBits = FLOAT_MINUS_ONE_BITS;
                            outPosition->clipYBits = FLOAT_MINUS_ONE_BITS;
                            outPosition->clipZ = FLOAT_MINUS_ONE_BITS;
                            outPosition->invWBits = FLOAT_MINUS_ONE_BITS;
                        }
                        outPosition++;
                        srcPointer++;
                    }
                } else {
                    for (remainingPts = vtxCount; remainingPts > 0; remainingPts--) {
                        TransformPointByMatrix4x4(stepMtx, srcPointer, &xClipValue);
                        if (xClipValue.w > 0.0f) {
                            recipShiftPlain = 1.0f / xClipValue.w;
                            outPosition->clipX = xClipValue.x * recipShiftPlain;
                            outPosition->clipY = xClipValue.y * recipShiftPlain;
                            outPosition->depth = xClipValue.z * recipShiftPlain + drawParams->depthBias;
                            outPosition->invW = recipShiftPlain;
                        } else {
                            outPosition->clipXBits = FLOAT_MINUS_ONE_BITS;
                            outPosition->clipYBits = FLOAT_MINUS_ONE_BITS;
                            outPosition->clipZ = FLOAT_MINUS_ONE_BITS;
                            outPosition->invWBits = FLOAT_MINUS_ONE_BITS;
                        }
                        outPosition++;
                        srcPointer++;
                    }
                }
                break;
            case GFX_MESH_DEPTH_REPLACE:
                if (drawParams->offsetEnabled != 0) {
                    for (remainingPts = vtxCount; remainingPts > 0; remainingPts--) {
                        xOffsetT.x = srcPointer->x + drawParams->offsetX;
                        xOffsetT.y = srcPointer->y + drawParams->offsetY;
                        xOffsetT.z = srcPointer->z + drawParams->offsetZ;
                        TransformPointByMatrix4x4(stepMtx, &xOffsetT, &xClipValue);
                        if (xClipValue.w > 0.0f) {
                            rcpSet = 1.0f / xClipValue.w;
                            outPosition->clipX = xClipValue.x * rcpSet;
                            outPosition->clipY = xClipValue.y * rcpSet;
                            outPosition->clipZ = *(int *)&drawParams->depthBias;
                            outPosition->invW = rcpSet;
                        } else {
                            outPosition->clipXBits = FLOAT_MINUS_ONE_BITS;
                            outPosition->clipYBits = FLOAT_MINUS_ONE_BITS;
                            outPosition->clipZ = FLOAT_MINUS_ONE_BITS;
                            outPosition->invWBits = FLOAT_MINUS_ONE_BITS;
                        }
                        outPosition++;
                        srcPointer++;
                    }
                } else {
                    for (remainingPts = vtxCount; remainingPts > 0; remainingPts--) {
                        TransformPointByMatrix4x4(stepMtx, srcPointer, &xClipValue);
                        if (xClipValue.w > 0.0f) {
                            invWidth = 1.0f / xClipValue.w;
                            outPosition->clipX = xClipValue.x * invWidth;
                            outPosition->clipY = xClipValue.y * invWidth;
                            outPosition->clipZ = *(int *)&drawParams->depthBias;
                            outPosition->invW = invWidth;
                        } else {
                            outPosition->clipXBits = FLOAT_MINUS_ONE_BITS;
                            outPosition->clipYBits = FLOAT_MINUS_ONE_BITS;
                            outPosition->clipZ = FLOAT_MINUS_ONE_BITS;
                            outPosition->invWBits = FLOAT_MINUS_ONE_BITS;
                        }
                        outPosition++;
                        srcPointer++;
                    }
                }
            }
            DxMesh_DrawIndexedPrimitiveRetry(batch, device, (struct GfxEngineContext *)context);
        }
    }
}
} /* extern "C" */
