// src/lib/src/graphics/dx_rend5.cpp
// Original: C:\lib\src\graphics\dx_rend5.cpp
// Contents: Direct3D 5 rendering back end: sets the render target and viewport, compiles
//   polygon-set groups into vertex and index batches, applies render states, and draws the batch
//   groups.
/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/types/graphics.h"
#include "lib/codegen.h"
#include "lib/types/polygon.h"
#include <windows.h>
#include "lib/enums/graphics.h"
#include "lib/com_abi.h"
#include "lib/api/graphics.h"
#include "lib/api/mem.h"
#include <stdio.h>

struct D3DQueryUnknown {
    virtual int __stdcall query(const GUID &, void **) = 0;
    virtual unsigned long __stdcall addRef(void) = 0;
    virtual unsigned long __stdcall release(void) = 0;
};
extern "C" {
extern const GUID IID_IDirect3D2; // DirectX 5 GUID from dxguid.lib.

/*
 * Releases Direct3D COM interfaces on the context record.
 */
// FUNCTION: FF7 0x006a3280
void __cdecl D3DReleaseContextInterfaces(struct GfxEngineContext *context)
{
    if (context != 0 && context->direct3D != 0) {
        if (*(D3DQueryUnknown **)&context->viewport2 != 0) {
            (*(D3DQueryUnknown **)&context->viewport2)->release();
            context->viewport2 = 0;
        }
        if (*(D3DQueryUnknown **)&context->d3dDevice2 != 0) {
            (*(D3DQueryUnknown **)&context->d3dDevice2)->release();
            context->d3dDevice2 = 0;
        }
        if (*(D3DQueryUnknown **)&context->direct3D2 != 0) {
            (*(D3DQueryUnknown **)&context->direct3D2)->release();
            context->direct3D2 = 0;
        }
    }
}

/*
 * Queries IDirect3D2 interface from context DirectDraw interface.
 */
// FUNCTION: FF7 0x006a3332
int __cdecl D3DQueryIDirect3D2(int context)
{
    int result;
    int iface;

    result = 0;
    if (context != 0 && ((GfxEngineContext *)context)->useDirect3D5 != 0 && ((GfxEngineContext *)context)->direct3D != 0) {
        // STRING: FF7 0x00912c90
        if (CheckDirectDrawResult((*(D3DQueryUnknown **)&((GfxEngineContext *)context)->direct3D)->query(IID_IDirect3D2, (void **)&((GfxEngineContext *)context)->direct3D2), "C:\\lib\\src\\graphics\\dx_rend5.cpp", 0x121)) {
            iface = ((GfxEngineContext *)context)->direct3D2;
            if (iface != 0) {
                result = 1;
            }
        }
    }
    return result;
}


// clang-format off
// GLOBAL: FF7 0x00912c50
float g_D3D5IdentityMatrix[4][4] = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
// GLOBAL: FF7 0x00db2d18
int g_GfxStillDrawingCount = 0;
// clang-format on

/*
 * Sets render target surface and creates Direct3D viewport.
 */
// FUNCTION: FF7 0x006a33ba
int __cdecl D3D5SetRenderTargetAndViewport(int setTarget, int setViewport, int surface, int viewport, GfxEngineContext *state)
{
    int flags;
    int ok;
    int createSlot;
    int detachSlot;

    ok = 0;
    if (surface != 0 && viewport != 0 && state != 0) {
        createSlot = state->direct3D2;
        detachSlot = state->d3dDevice2;
        if (createSlot != 0 && detachSlot != 0) {
            ok = 1;
            flags = 0;
            if (setTarget != 0) {
                // STRING: FF7 0x00912cb4
                if (CheckDirectDrawResult(((ComDirect3DDevice2 *)(detachSlot))->SetRenderTarget(surface, flags), "C:\\lib\\src\\graphics\\dx_rend5.cpp", 0x142) != 0) { /* empty first arm: lib/codegen.h */
                } else {
                    ok = 0;
                }
            }
            if (setViewport != 0) {
                ok = 0;
                // STRING: FF7 0x00912cd8
                if (CheckDirectDrawResult(((ComDirect3DDevice2 *)(detachSlot))->SetCurrentViewport(viewport), "C:\\lib\\src\\graphics\\dx_rend5.cpp", 0x14F) != 0) {
                    ok = 1;
                }
            }
        }
    }
    return ok;
}

/*
 * Initializes a D3D5 batch record and default fields.
 */
// FUNCTION: FF7 0x006a349b
void __cdecl D3D5InitBatch(GfxD3d5BatchHeader *batch)
{
    if (batch != 0) {
        ZeroFillMemory32(0x28, batch);
        batch->primType = 4;
        batch->flags = 8;
    }
}

/*
 * Frees a D3D5 batch and its vertex and index arrays.
 */
// FUNCTION: FF7 0x006a34c8
void __cdecl D3D5FreeBatch(GfxD3d5BatchState *batch)
{
    if (batch != 0) {
        // STRING: FF7 0x00912cfc
        GameHeapFree((void *)batch->vertices, "C:\\lib\\src\\graphics\\dx_rend5.cpp", 0x16D);
        // STRING: FF7 0x00912d20
        GameHeapFree((void *)batch->indices, "C:\\lib\\src\\graphics\\dx_rend5.cpp", 0x16E);
        // STRING: FF7 0x00912d44
        GameHeapFree(batch, "C:\\lib\\src\\graphics\\dx_rend5.cpp", 0x16F);
    }
}

/*
 * Allocates and initializes a D3D5 batch record.
 */
// FUNCTION: FF7 0x006a351b
int __cdecl D3D5AllocBatch(void)
{
    GfxRenderBatch *rec;

    // STRING: FF7 0x00912d68
    rec = (GfxRenderBatch *)((int)GameHeapAllocRaw(0x28, "C:\\lib\\src\\graphics\\dx_rend5.cpp", 0x177));
    if (rec != 0) {
        D3D5InitBatch((struct GfxD3d5BatchHeader *)rec);
    }
    return (int)rec;
}

/*
 * Compiles vertex and index buffers for a polygon set group batch.
 */
// FUNCTION: FF7 0x006a354f
int __cdecl D3D5CompileGroupBatch(int groupIndex, PolygonFileArrays *batchState)
{
    GfxColorSourceGroup *colorSrc;
    int rangeFail;
    int faceCount;
    int startVert;
    int baseIndex;
    GfxGroupBatchEntry *ent;
    int maxIdx;
    int verts;
    GfxFaceRecord *faceRecord;
    int idxPos;
    int faceIdx;
    int failed;
    GfxRenderBatch *result;

    failed = 1;
    result = 0;
    if (batchState != 0) {
        result = (GfxRenderBatch *)D3D5AllocBatch();
        if (result != 0) {
            rangeFail = 0;
            colorSrc = 0;
            ent = 0;
            if (batchState->groupCount > 0) {
                if (groupIndex < batchState->groupCount) {
                    ent = (GfxGroupBatchEntry *)(batchState->groups + groupIndex * 0x38);
                    maxIdx = ent->maxIdx;
                    faceCount = ent->faceCount;
                    startVert = ent->startVert;
                    baseIndex = ent->baseIndex;
                    if (batchState->hundreds != 0) {
                        colorSrc = (GfxColorSourceGroup *)(batchState->hundreds + groupIndex * 0x64);
                    } else {
                        colorSrc = 0;
                    }
                } else {
                    rangeFail = 1;
                }
            } else {
                maxIdx = batchState->vertexCount;
                faceCount = batchState->polygonCount;
                startVert = 0;
                baseIndex = 0;
                colorSrc = (GfxColorSourceGroup *)(batchState->hundreds);
            }
            if (rangeFail == 0) {
                result->vertexStride = D3D5GetVertexStride(batchState);
                result->primType = D3DPT_TRIANGLELIST;
                switch (batchState->kind) {
                case GFX_D3D_VERTEX_FORMAT_VERTEX:
                    result->vertexType = D3DVT_VERTEX;
                    break;
                case GFX_D3D_VERTEX_FORMAT_LVERTEX:
                    result->vertexType = D3DVT_LVERTEX;
                    break;
                case GFX_D3D_VERTEX_FORMAT_TLVERTEX:
                    result->vertexType = D3DVT_TLVERTEX;
                    break;
                default:
                    result->vertexType = D3DVT_VERTEX;
                }
                result->maxIndex = maxIdx;
                // STRING: FF7 0x00912d8c
                result->vertices = (int)GameHeapAlloc(result->maxIndex + 1, result->vertexStride, "C:\\lib\\src\\graphics\\dx_rend5.cpp", 0x1D6);
                verts = result->vertices;
                result->vertexOut = verts + 0x1F & 0xFFFFFFE0;
                result->indexCount = faceCount * 3;
                // STRING: FF7 0x00912db0
                result->indices = (short *)GameHeapAlloc(result->indexCount, 2, "C:\\lib\\src\\graphics\\dx_rend5.cpp", 0x1E2);
                if (result->vertexOut != 0 && result->indices != 0) {
                    D3D5BuildVertexArray((struct GfxTlVertex *)result->vertexOut, colorSrc, (struct GfxVertexGroup *)ent, batchState);
                    idxPos = 0;
                    for (faceIdx = baseIndex; faceIdx < baseIndex + faceCount; faceIdx++) {
                        faceRecord = (GfxFaceRecord *)(batchState->polygons + faceIdx * 0x18);
                        result->indices[idxPos] = faceRecord->index0;
                        result->indices[idxPos + 1] = faceRecord->index1;
                        result->indices[idxPos + 2] = faceRecord->index2;
                        idxPos += 3;
                    }
                    failed = 0;
                }
            }
            if (failed != 0) {
                D3D5FreeBatch((struct GfxD3d5BatchState *)result);
                result = 0;
            }
        }
    }
    return (int)result;
}

/*
 * Builds vertex array from polygon set data into batch buffer.
 */
// FUNCTION: FF7 0x006a37f5
int __cdecl D3D5BuildVertexArray(GfxTlVertex *vertexOut, void *colorSource, GfxVertexGroup *groupRecord, PolygonFileArrays *source)
{
    int textureStart;
    int textureIndex;
    int firstVertex;
    int vertexCnt;
    int hasTexCoords;
    int vertIndex;
    GfxTlVertex *vertexPtr;
    float invWidth;
    GfxTlVertex *destinationPtr;
    GfxTlVertex *destination;

    hasTexCoords = 0;
    if (groupRecord != 0) {
        vertexCnt = groupRecord->vertexCount;
        firstVertex = groupRecord->firstVertex;
        textureStart = groupRecord->textureStart;
        textureIndex = textureStart;
        hasTexCoords = groupRecord->hasTexCoords;
    } else {
        vertexCnt = source->vertexCount;
        firstVertex = 0;
        textureStart = 0;
        textureIndex = textureStart;
        hasTexCoords = source->texCoordCount > 0;
    }
    switch (source->kind) {
    case GFX_D3D_VERTEX_FORMAT_VERTEX:
        destination = vertexOut;
        for (vertIndex = firstVertex; vertIndex < firstVertex + vertexCnt; vertIndex++) {
            destination->xBits = source->positions[vertIndex].x;
            destination->yBits = source->positions[vertIndex].y;
            destination->zBits = source->positions[vertIndex].z;
            if (source->normalCount > 0) {
                destination->rhwBits = source->sourceEntries[vertIndex].reciprocalW;
                destination->color = source->sourceEntries[vertIndex].vertexColor;
                destination->specular = source->sourceEntries[vertIndex].specularColor;
            } else {
                destination->rhwBits = FLOAT_ONE_BITS;
                destination->color = 0;
                destination->specular = 0;
            }
            if (hasTexCoords != 0 && textureIndex < source->texCoordCount) {
                destination->uBits = source->texCoordPairs[textureIndex].texU;
                destination->vBits = source->texCoordPairs[textureIndex].texV;
            } else {
                destination->uBits = 0;
                destination->vBits = 0;
            }
            destination++;
            textureIndex++;
        }
        vertexOut = (GfxTlVertex *)destination;
        break;
    case GFX_D3D_VERTEX_FORMAT_LVERTEX:
        destinationPtr = vertexOut;
        for (vertIndex = firstVertex; vertIndex < firstVertex + vertexCnt; vertIndex++) {
            destinationPtr->xBits = source->positions[vertIndex].x;
            destinationPtr->yBits = source->positions[vertIndex].y;
            destinationPtr->zBits = source->positions[vertIndex].z;
            destinationPtr->rhwBits = 0;
            destinationPtr->color = source->vertexColorWords[vertIndex];
            ApplyGlobalColorModulate(&destinationPtr->color, colorSource);
            destinationPtr->specular = 0xFF000000;
            if (hasTexCoords > 0 && textureIndex < source->texCoordCount) {
                destinationPtr->uBits = source->texCoordPairs[textureIndex].texU;
                destinationPtr->vBits = source->texCoordPairs[textureIndex].texV;
            } else {
                destinationPtr->uBits = 0;
                destinationPtr->vBits = 0;
            }
            destinationPtr++;
            textureIndex++;
        }
        vertexOut = (GfxTlVertex *)destinationPtr;
        break;
    case GFX_D3D_VERTEX_FORMAT_TLVERTEX:
        vertexPtr = vertexOut;
        for (vertIndex = firstVertex; vertIndex < firstVertex + vertexCnt; vertIndex++) {
            vertexPtr->xBits = source->positions[vertIndex].x;
            vertexPtr->yBits = source->positions[vertIndex].y;
            vertexPtr->zBits = source->positions[vertIndex].z;
            invWidth = 1.0f;
            *(float *)&vertexPtr->rhwBits = 1.0f / invWidth;
            vertexPtr->color = source->vertexColorWords[vertIndex];
            ApplyGlobalColorModulate(&vertexPtr->color, colorSource);
            vertexPtr->specular = 0xFF000000;
            if (hasTexCoords > 0 && textureIndex < source->texCoordCount) {
                vertexPtr->uBits = source->texCoordPairs[textureIndex].texU;
                vertexPtr->vBits = source->texCoordPairs[textureIndex].texV;
            } else {
                vertexPtr->uBits = 0;
                vertexPtr->vBits = 0;
            }
            vertexPtr++;
            textureIndex++;
        }
        vertexOut = (GfxTlVertex *)vertexPtr;
    }
    return (int)vertexOut;
}

/*
 * Applies global color modulation to vertex color.
 */
// FUNCTION: FF7 0x006a3bee
void __cdecl ApplyGlobalColorModulate(void *colorPtr, void *record)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[4];
        int colorWord;
        struct { unsigned char pad_to_3[0x3]; char modulateValue; };
    } frame;
#pragma pack(pop)

    if (record != 0 && (((GfxColorModulateRecord *)record)->flags & 0x400) != 0) {
        frame.colorWord = *(int *)colorPtr;
        frame.modulateValue = ((GfxColorModulateRecord *)record)->modulateValue;
        *(int *)colorPtr = frame.colorWord;
    }
}

/*
 * Looks up vertex stride for batch vertex kind.
 */
// FUNCTION: FF7 0x006a3c25
int __cdecl D3D5GetVertexStride(PolygonFileArrays *batchState)
{
    return D3D5VertexStride(batchState->kind);
}

/*
 * Returns vertex byte stride for given vertex format.
 */
// FUNCTION: FF7 0x006a3c39
int __cdecl D3D5VertexStride(int kind)
{
    int stride;

    switch (kind) {
    case GFX_D3D_VERTEX_FORMAT_VERTEX:
        stride = 0x20;
        break;
    case GFX_D3D_VERTEX_FORMAT_LVERTEX:
        stride = 0x20;
        break;
    case GFX_D3D_VERTEX_FORMAT_TLVERTEX:
        stride = 0x20;
        break;
    default:
        stride = 0x20;
    }
    return stride;
}

/*
 * Allocates batch array for polygon set group.
 */
// FUNCTION: FF7 0x006a3c82
int __cdecl D3D5AllocBatchArray(GfxPolySetBatchArray *polySet)
{
    int ok;

    ok = 0;
    if (polySet != 0) {
        // STRING: FF7 0x00912dd4
        polySet->batchArray = (int *)GameHeapAlloc(polySet->batchCount, 4, "C:\\lib\\src\\graphics\\dx_rend5.cpp", 0x219);
        if (polySet->batchArray != 0) {
            ok = 1;
        }
    }
    return ok;
}

/*
 * Compiles rendering group into batches for device.
 */
// FUNCTION: FF7 0x006a3ccb
int __cdecl D3D5CompileGroup(int index, int reserved2, int reserved3, int reserved4, PolygonFileArrays *batchState, GfxPolySetBatchArray *group)
{
    int ok;

    ok = 0;
    if (batchState != 0) {
        if (group->batchArray != 0) {
            group->batchArray[index] = D3D5CompileGroupBatch(index, batchState);
            if (group->batchArray[index] != 0) {
                ok = 1;
            } else {
                ok = 0;
            }
        }
    } else {
        ok = 1;
    }
    return ok;
}

/*
 * Applies render states and texture settings to Direct3D device.
 */
// FUNCTION: FF7 0x006a3d30
void __cdecl D3D5ApplyRenderState(GfxRenderStateRecord *renderState, struct GfxEngineContext *context)
{
    int device;
    int enable;
    GfxPaletteRecord *table;
    int texInfo;
    GfxTextureSurface *textureEntry;
    int curInfo;
    int textureId;
    GfxPaletteRecord *auxTable;
    int biasOfs;
    int parameterVal;
    GfxTextureSurface *texture;
    int bits;
    int state;
    int textureHandle;

    device = context->d3dDevice2;
    if (renderState != 0 && device != 0) {
        bits = renderState->bits;
        if ((bits & 0xFFFDFBFD) != 0) {
            if ((bits & 1) != 0) {
                if ((renderState->flags & 1) != 0) {
                    state = 2;
                } else {
                    state = 3;
                }
                ((ComDirect3DDevice2 *)(device))->SetRenderState(D3DRENDERSTATE_FILLMODE, state);
            }
            if ((bits & 2) != 0) {
                texture = (GfxTextureSurface *)(renderState->texturePtr);
                if (texture != 0 && (renderState->flags & 2) != 0) {
                    texInfo = texture->header;
                    textureHandle = texture->handle;
                    table = (GfxPaletteRecord *)(texture->palette);
                    if (table != 0) {
                        if (table->paletteMode != 0) {
                            if ((*(unsigned char *)((char *)table->colorLookup + texture->paletteRow) & 1) != 0) {
                                enable = 1;
                            } else {
                                enable = 0;
                            }
                        } else if (*(int *)((char *)texInfo + 8) != 0) {
                            enable = 1;
                        } else {
                            enable = 0;
                        }
                    } else if (*(int *)((char *)texInfo + 8) != 0) {
                        enable = 1;
                    } else {
                        enable = 0;
                    }
                    ((ComDirect3DDevice2 *)(device))->SetRenderState(D3DRENDERSTATE_COLORKEYENABLE, enable);
                } else {
                    ((ComDirect3DDevice2 *)(device))->SetRenderState(D3DRENDERSTATE_COLORKEYENABLE, 0);
                    textureHandle = 0;
                }
                ((ComDirect3DDevice2 *)(device))->SetRenderState(D3DRENDERSTATE_TEXTUREHANDLE, textureHandle);
            }
            if ((bits & 0x200) != 0) {
                if ((renderState->flags & 0x200) != 0) {
                    enable = 1;
                } else {
                    enable = 0;
                }
                ((ComDirect3DDevice2 *)(device))->SetRenderState(D3DRENDERSTATE_DITHERENABLE, enable);
            }
            if ((bits & 4) != 0) {
                if ((renderState->flags & 4) != 0) {
                    if (context != 0) {
                        if (context->modeSetupDone != 0) {
                            state = 1;
                        } else {
                            state = 2;
                        }
                    } else {
                        state = 1;
                    }
                } else {
                    state = 1;
                }
                ((ComDirect3DDevice2 *)(device))->SetRenderState(D3DRENDERSTATE_TEXTUREMAG, state);
                ((ComDirect3DDevice2 *)(device))->SetRenderState(D3DRENDERSTATE_TEXTUREMIN, state);
            }
            if ((bits & 8) != 0) {
                if ((renderState->flags & 8) != 0) {
                    enable = 1;
                } else {
                    enable = 0;
                }
                ((ComDirect3DDevice2 *)(device))->SetRenderState(D3DRENDERSTATE_TEXTUREPERSPECTIVE, enable);
            }
            if ((bits & 0x4000) != 0) {
                if ((renderState->flags & 0x4000) != 0) {
                    state = 1;
                    ((ComDirect3DDevice2 *)(device))->SetRenderState(D3DRENDERSTATE_CULLMODE, state);
                } else if ((bits & 0x2000) != 0) {
                    if ((renderState->flags & 0x2000) != 0) {
                        state = 2;
                    } else {
                        state = 3;
                    }
                    ((ComDirect3DDevice2 *)(device))->SetRenderState(D3DRENDERSTATE_CULLMODE, state);
                }
            } else if ((bits & 0x2000) != 0) {
                if ((renderState->flags & 0x2000) != 0) {
                    state = 2;
                } else {
                    state = 3;
                }
                ((ComDirect3DDevice2 *)(device))->SetRenderState(D3DRENDERSTATE_CULLMODE, state);
            }
            if ((bits & 0x8000) != 0) {
                if ((renderState->flags & 0x8000) != 0) {
                    enable = 1;
                } else {
                    enable = 0;
                }
                ((ComDirect3DDevice2 *)(device))->SetRenderState(D3DRENDERSTATE_ZENABLE, enable);
                ((ComDirect3DDevice2 *)(device))->SetRenderState(D3DRENDERSTATE_ZFUNC, D3DCMP_LESSEQUAL);
            }
            if ((bits & 0x10000) != 0) {
                if ((renderState->flags & 0x10000) != 0) {
                    enable = 1;
                } else {
                    enable = 0;
                }
                ((ComDirect3DDevice2 *)(device))->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, enable);
            }
            if ((bits & 0x100) != 0) {
                if ((renderState->flags & 0x100) != 0) {
                    enable = 1;
                } else {
                    enable = 0;
                }
                ((ComDirect3DDevice2 *)(device))->SetRenderState(D3DRENDERSTATE_COLORKEYENABLE, enable);
            }
            if ((bits & 0x400) != 0) {
                if ((renderState->flags & 0x400) != 0) {
                    enable = 1;
                } else {
                    enable = 0;
                }
                if (context->alphaOption != 0) {
                    ((ComDirect3DDevice2 *)(device))->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, enable);
                } else {
                    ((ComDirect3DDevice2 *)(device))->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, enable);
                }
                if (enable != 0) {
                    if (context->overrideActive != 0) {
                        state = ((GfxRenderStateTable *)(context->overrideRenderState))->stateValue;
                        ((ComDirect3DDevice2 *)(device))->SetRenderState(D3DRENDERSTATE_SRCBLEND, state);
                        state = ((GfxRenderStateTable *)(context->overrideRenderState))->stateFallback;
                        ((ComDirect3DDevice2 *)(device))->SetRenderState(D3DRENDERSTATE_DESTBLEND, state);
                    } else {
                        state = renderState->stateValue;
                        ((ComDirect3DDevice2 *)(device))->SetRenderState(D3DRENDERSTATE_SRCBLEND, state);
                        state = renderState->stateFallback;
                        ((ComDirect3DDevice2 *)(device))->SetRenderState(D3DRENDERSTATE_DESTBLEND, state);
                    }
                } else {
                    state = 2;
                    ((ComDirect3DDevice2 *)(device))->SetRenderState(D3DRENDERSTATE_SRCBLEND, state);
                    state = 1;
                    ((ComDirect3DDevice2 *)(device))->SetRenderState(D3DRENDERSTATE_DESTBLEND, state);
                }
            }
            if ((bits & 0x10) != 0) {
                if ((renderState->flags & 0x10) != 0) {
                    state = 2;
                } else {
                    state = 2;
                }
                ((ComDirect3DDevice2 *)(device))->SetRenderState(D3DRENDERSTATE_TEXTUREMAPBLEND, state);
            }
            if ((bits & 0x800) != 0) {
                if ((renderState->flags & 0x800) != 0) {
                    enable = 1;
                } else {
                    enable = 0;
                }
                ((ComDirect3DDevice2 *)(device))->SetRenderState(D3DRENDERSTATE_ALPHATESTENABLE, enable);
                if (enable != 0) {
                    state = 5;
                    parameterVal = renderState->parameterVal;
                    ((ComDirect3DDevice2 *)(device))->SetRenderState(D3DRENDERSTATE_ALPHAFUNC, state);
                    ((ComDirect3DDevice2 *)(device))->SetRenderState(D3DRENDERSTATE_ALPHAREF, parameterVal);
                } else {
                    state = 8;
                    ((ComDirect3DDevice2 *)(device))->SetRenderState(D3DRENDERSTATE_ALPHAFUNC, state);
                    ((ComDirect3DDevice2 *)(device))->SetRenderState(D3DRENDERSTATE_ALPHAREF, renderState->parameterVal);
                }
            }
            if ((bits & 0x1000) != 0) {
                if ((renderState->flags & 0x1000) != 0) {
                    enable = 1;
                } else {
                    enable = 0;
                }
                ((ComDirect3DDevice2 *)(device))->SetRenderState(D3DRENDERSTATE_ANTIALIAS, enable);
            }
            if ((bits & 0x100000) != 0) {
                if ((renderState->flags & 0x100000) != 0) {
                    enable = 1;
                } else {
                    enable = 0;
                }
                ((ComDirect3DDevice2 *)(device))->SetRenderState(D3DRENDERSTATE_FOGENABLE, enable);
            }
            if ((bits & 0x40000) != 0) {
                if ((renderState->flags & 0x40000) != 0) {
                    enable = 1;
                } else {
                    enable = 0;
                }
                ((ComDirect3DDevice2 *)(device))->SetRenderState(D3DRENDERSTATE_SPECULARENABLE, enable);
            }
            if ((bits & 0x20000) != 0) {
                if ((renderState->flags & 0x20000) != 0 && context->renderStateApplied == 0) {
                    state = renderState->stateId;
                } else {
                    state = 1;
                }
                ((ComDirect3DDevice2 *)(device))->SetRenderState(D3DRENDERSTATE_SHADEMODE, state);
            }
            if ((bits & 0x80000) != 0) {
                biasOfs = renderState->biasOfs;
                if (renderState->textureRef != 0 && (renderState->flags & 0x80000) != 0) {
                    ((ComDirect3DDevice2 *)(device))->SetLightState(D3DLIGHTSTATE_MATERIAL, *(int *)renderState->textureRef);
                    ((ComDirect3DDevice2 *)(device))->SetLightState(D3DLIGHTSTATE_AMBIENT, biasOfs);
                } else {
                    ((ComDirect3DDevice2 *)(device))->SetLightState(D3DLIGHTSTATE_MATERIAL, 0);
                    ((ComDirect3DDevice2 *)(device))->SetLightState(D3DLIGHTSTATE_AMBIENT, biasOfs);
                }
            }
            if ((bits & 0x20) != 0) {
                if ((renderState->flags & 0x20) != 0) {
                    ((ComDirect3DDevice2 *)(device))->SetRenderState(D3DRENDERSTATE_WRAPU, 1);
                } else {
                    ((ComDirect3DDevice2 *)(device))->SetRenderState(D3DRENDERSTATE_WRAPU, 0);
                }
            }
            if ((bits & 0x40) != 0) {
                if ((renderState->flags & 0x40) != 0) {
                    ((ComDirect3DDevice2 *)(device))->SetRenderState(D3DRENDERSTATE_WRAPV, 1);
                } else {
                    ((ComDirect3DDevice2 *)(device))->SetRenderState(D3DRENDERSTATE_WRAPV, 0);
                }
            }
        } else {
            if ((bits & 0xFFFDFFFD) != 0 && (bits & 0x400) != 0) {
                if ((renderState->flags & 0x400) != 0) {
                    enable = 1;
                } else {
                    enable = 0;
                }
                if (context->alphaOption != 0) {
                    ((ComDirect3DDevice2 *)(device))->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, enable);
                } else {
                    ((ComDirect3DDevice2 *)(device))->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, enable);
                }
                if (enable != 0) {
                    if (context->overrideActive != 0) {
                        state = ((GfxRenderStateTable *)(context->overrideRenderState))->stateValue;
                        ((ComDirect3DDevice2 *)(device))->SetRenderState(D3DRENDERSTATE_SRCBLEND, state);
                        state = ((GfxRenderStateTable *)(context->overrideRenderState))->stateFallback;
                        ((ComDirect3DDevice2 *)(device))->SetRenderState(D3DRENDERSTATE_DESTBLEND, state);
                    } else {
                        state = renderState->stateValue;
                        ((ComDirect3DDevice2 *)(device))->SetRenderState(D3DRENDERSTATE_SRCBLEND, state);
                        state = renderState->stateFallback;
                        ((ComDirect3DDevice2 *)(device))->SetRenderState(D3DRENDERSTATE_DESTBLEND, state);
                    }
                } else {
                    state = 2;
                    ((ComDirect3DDevice2 *)(device))->SetRenderState(D3DRENDERSTATE_SRCBLEND, state);
                    state = 1;
                    ((ComDirect3DDevice2 *)(device))->SetRenderState(D3DRENDERSTATE_DESTBLEND, state);
                }
            }
            if ((bits & 2) != 0) {
                textureEntry = (GfxTextureSurface *)(renderState->texturePtr);
                if (textureEntry != 0 && (renderState->flags & 2) != 0) {
                    curInfo = textureEntry->header;
                    textureId = textureEntry->handle;
                    auxTable = (GfxPaletteRecord *)(textureEntry->palette);
                    if (auxTable != 0) {
                        if (auxTable->paletteMode != 0) {
                            if ((*(unsigned char *)((char *)auxTable->colorLookup + textureEntry->paletteRow) & 1) != 0) {
                                enable = 1;
                            } else {
                                enable = 0;
                            }
                        } else if (*(int *)((char *)curInfo + 8) != 0) {
                            enable = 1;
                        } else {
                            enable = 0;
                        }
                    } else if (*(int *)((char *)curInfo + 8) != 0) {
                        enable = 1;
                    } else {
                        enable = 0;
                    }
                    ((ComDirect3DDevice2 *)(device))->SetRenderState(D3DRENDERSTATE_COLORKEYENABLE, enable);
                } else {
                    ((ComDirect3DDevice2 *)(device))->SetRenderState(D3DRENDERSTATE_COLORKEYENABLE, 0);
                    textureId = 0;
                }
                ((ComDirect3DDevice2 *)(device))->SetRenderState(D3DRENDERSTATE_TEXTUREHANDLE, textureId);
            }
            if ((bits & 0x20000) != 0) {
                if ((renderState->flags & 0x20000) != 0 && context->renderStateApplied == 0) {
                    state = renderState->stateId;
                } else {
                    state = 1;
                }
                ((ComDirect3DDevice2 *)(device))->SetRenderState(D3DRENDERSTATE_SHADEMODE, state);
            }
        }
    }
}

/*
 * Increments counter when Direct3D returns WASSTILLDRAWING.
 */
// FUNCTION: FF7 0x006a479c
void __cdecl D3D5CountWasStillDrawing(int step, struct GfxEngineContext *context)
{
    g_GfxStillDrawingCount++;
}

/*
 * Draws primitive batches for deferred group lists.
 */
// FUNCTION: FF7 0x006a47ae
void __cdecl D3D5DrawGroupListPass(int passKind, int groupHead, struct GfxEngineContext *context)
{
    int drawStatus;
    int mtxPtr;
    GfxDrawStateNode *stateNode;
    int result;
    int drawResult;
    GfxTextureSurface *pTexPtr;
    int renderable;
    int infoPtr;
    int workStatus;
    int on;
    int worldMtx;
    int buffer[50];
    int auxCursor;
    int idx;
    GfxDeferredPassEntry *deferEntryPtr;
    int hit;
    GlDrawGroup *calcGroup;
    int rawHasState;
    GfxDrawListTransforms *listState;
    int d3d6;
    GfxDrawGroupNode *curGroupNodeAddr;
    GfxPolygonBatch *groupEntryPtr;
    GfxDrawListNode *pDrawNode;
    int pendingStep;
    GfxPolygonSet *activeListPtr;
    int vertices;

    pendingStep = 0;
    d3d6 = context->d3dDevice2;
    if (groupHead != 0 && context != 0 && d3d6 != 0) {
        curGroupNodeAddr = (GfxDrawGroupNode *)(*(int *)((char *)groupHead + 0xC));
        while (curGroupNodeAddr != 0) {
            activeListPtr = (GfxPolygonSet *)(curGroupNodeAddr->drawGroupList);
            if (activeListPtr != 0 && activeListPtr->drawEnabled != 0) {
                listState = (GfxDrawListTransforms *)activeListPtr->matrices;
                deferEntryPtr = (GfxDeferredPassEntry *)((int)activeListPtr + 0x14);
                rawHasState = 0;
                if (deferEntryPtr->active != 0) {
                    rawHasState = 1;
                    if (*(int *)deferEntryPtr->statePtr == deferEntryPtr->expected) {
                        hit = 1;
                    } else {
                        hit = 0;
                    }
                }
                calcGroup = (GlDrawGroup *)activeListPtr->groupState;
                on = 0;
                infoPtr = activeListPtr->lightGroup;
                if (infoPtr != 0 && (*(int *)infoPtr & 2) != 0) {
                    on = 1;
                }
                Gfx_SetRenderStateCall(RENDER_STATE_SPECULAR, on, (struct GfxEngineContext *)context);
                auxCursor = 0;
                if (activeListPtr->source != 0) {
                    auxCursor = *(int *)((char *)activeListPtr->source + 0x6C);
                }
                for (idx = 0; idx < activeListPtr->groupCount; idx++) {
                    if (calcGroup != 0) {
                        switch (passKind) {
                        case GFX_SCENE_PASS_OPAQUE:
                            renderable = (calcGroup->flags & 0x410) == 0;
                            break;
                        case GFX_SCENE_PASS_TRANSLUCENT:
                            renderable = calcGroup->flags & 0x410;
                            break;
                        default:
                            renderable = 0;
                        }
                    } else {
                        renderable = 1;
                    }
                    if (renderable != 0) {
                        if (calcGroup != 0) {
                            pTexPtr = (GfxTextureSurface *)(calcGroup->batch);
                            if (pTexPtr != 0 && pTexPtr->surface != 0) {
                                result = ((ComDirectDrawSurface *)(pTexPtr->surface))->IsLost();
                                if (result == DDERR_SURFACELOST) {
                                    calcGroup->batch = (int)Gfx_RestoreTextureSurfaces(pTexPtr);
                                    if (calcGroup->batch == 0) {
                                        break;
                                    }
                                }
                            }
                        }
                        Gfx_ApplyRenderStateCall((int)calcGroup, context);
                        if (listState != 0) {
                            if (listState->setView != 0) {
                                ((ComDirect3DDevice2 *)(d3d6))->SetTransform(D3DTRANSFORMSTATE_VIEW, listState->viewMatrix);
                            }
                            if (listState->setProjection != 0) {
                                ((ComDirect3DDevice2 *)(d3d6))->SetTransform(D3DTRANSFORMSTATE_PROJECTION, listState->projectionMatrix);
                            }
                        }
                        if (rawHasState != 0) {
                            if (hit != 0) {
                                stateNode = (GfxDrawStateNode *)deferEntryPtr->nodePtr;
                                if (stateNode != 0) {
                                    switch (stateNode->kind) {
                                    case GFX_DRAW_OP_BATCH:
                                        do {
                                            switch (deferEntryPtr->kind) {
                                            case GFX_POLYGON_SET_TRANSFORM_NONE:
                                                mtxPtr = 0;
                                                break;
                                            case GFX_POLYGON_SET_TRANSFORM_NODE:
                                                mtxPtr = (int)stateNode + 0xC;
                                                break;
                                            case GFX_POLYGON_SET_TRANSFORM_NODE_CAMERA:
                                                ConcatAffineMatrix4x4(stateNode->matrix, (void *)context->cameraMatrix, &g_D3D5IdentityMatrix);
                                                mtxPtr = (int)&g_D3D5IdentityMatrix;
                                                break;
                                            default:
                                                mtxPtr = 0;
                                            }
                                            if (mtxPtr != 0 && listState != 0 && listState->setWorld != 0) {
                                                ((ComDirect3DDevice2 *)(d3d6))->SetTransform(D3DTRANSFORMSTATE_WORLD, mtxPtr);
                                            }
                                            if (activeListPtr->batchList != 0) {
                                                groupEntryPtr = activeListPtr->batchList[idx];
                                                if (groupEntryPtr != 0) {
                                                    if (activeListPtr->drawUnsorted == 0) {
                                                        groupEntryPtr->flags |= D3DDP_DONOTCLIP;
                                                    } else {
                                                        groupEntryPtr->flags &= ~D3DDP_DONOTCLIP;
                                                    }
                                                    do {
                                                        drawStatus = ((ComDirect3DDevice2 *)(d3d6))->DrawIndexedPrimitive(groupEntryPtr->primType, groupEntryPtr->vertexType, groupEntryPtr->vertices, groupEntryPtr->vertexCount, groupEntryPtr->indices, groupEntryPtr->indexCount, groupEntryPtr->flags);
                                                        switch (drawStatus) {
                                                        case DD_OK:
                                                            break;
                                                        case DDERR_WASSTILLDRAWING:
                                                            D3D5CountWasStillDrawing(1, context);
                                                            break;
                                                        default:
                                                            // STRING: FF7 0x00912df8
                                                            CheckDirectDrawResult(drawStatus, "C:\\lib\\src\\graphics\\dx_rend5.cpp", 0x5C8);
                                                            drawStatus = 0;
                                                        }
                                                    } while (drawStatus != 0);
                                                }
                                            }
                                            stateNode = (GfxDrawStateNode *)(stateNode->next);
                                        } while (stateNode != 0);
                                        break;
                                    case GFX_DRAW_OP_DRAW_LIST:
                                        do {
                                            switch (deferEntryPtr->kind) {
                                            case GFX_POLYGON_SET_TRANSFORM_NONE:
                                                worldMtx = 0;
                                                break;
                                            case GFX_POLYGON_SET_TRANSFORM_NODE:
                                                worldMtx = (int)stateNode + 0xC;
                                                break;
                                            case GFX_POLYGON_SET_TRANSFORM_NODE_CAMERA:
                                                ConcatAffineMatrix4x4(stateNode->matrix, (void *)context->cameraMatrix, &g_D3D5IdentityMatrix);
                                                worldMtx = (int)&g_D3D5IdentityMatrix;
                                                break;
                                            default:
                                                worldMtx = 0;
                                            }
                                            if (worldMtx != 0 && listState != 0 && listState->setWorld != 0) {
                                                ((ComDirect3DDevice2 *)(d3d6))->SetTransform(D3DTRANSFORMSTATE_WORLD, worldMtx);
                                            }
                                            if (activeListPtr->batchList != 0) {
                                                groupEntryPtr = activeListPtr->batchList[idx];
                                                if (groupEntryPtr != 0) {
                                                    pDrawNode = (GfxDrawListNode *)(stateNode->drawNode);
                                                    if (pDrawNode->kind == GFX_DRAW_SPRITE_BATCH) {
                                                        vertices = pDrawNode->vertexCount;
                                                    } else {
                                                        Gfx_EmitPolygonSetPrimitive(0, (int)calcGroup, pDrawNode);
                                                        vertices = groupEntryPtr->vertices;
                                                    }
                                                    if (activeListPtr->drawUnsorted == 0) {
                                                        groupEntryPtr->flags |= D3DDP_DONOTCLIP;
                                                    } else {
                                                        groupEntryPtr->flags &= ~D3DDP_DONOTCLIP;
                                                    }
                                                    do {
                                                        workStatus = ((ComDirect3DDevice2 *)(d3d6))->DrawIndexedPrimitive(groupEntryPtr->primType, groupEntryPtr->vertexType, vertices, groupEntryPtr->vertexCount, groupEntryPtr->indices, groupEntryPtr->indexCount, groupEntryPtr->flags);
                                                        switch (workStatus) {
                                                        case DD_OK:
                                                            break;
                                                        case DDERR_WASSTILLDRAWING:
                                                            D3D5CountWasStillDrawing(1, context);
                                                            break;
                                                        default:
                                                            // STRING: FF7 0x00912e1c
                                                            CheckDirectDrawResult(workStatus, "C:\\lib\\src\\graphics\\dx_rend5.cpp", 0x64F);
                                                            workStatus = 0;
                                                        }
                                                    } while (workStatus != 0);
                                                }
                                            }
                                            stateNode = (GfxDrawStateNode *)(stateNode->next);
                                        } while (stateNode != 0);
                                    }
                                }
                            }
                        } else if (activeListPtr->batchList != 0) {
                            groupEntryPtr = activeListPtr->batchList[idx];
                            if (groupEntryPtr != 0) {
                                if (listState != 0 && listState->setWorld != 0) {
                                    ((ComDirect3DDevice2 *)(d3d6))->SetTransform(D3DTRANSFORMSTATE_WORLD, listState->worldMatrix);
                                }
                                if (activeListPtr->drawUnsorted == 0) {
                                    groupEntryPtr->flags |= D3DDP_DONOTCLIP;
                                } else {
                                    groupEntryPtr->flags &= ~D3DDP_DONOTCLIP;
                                }
                                do {
                                    drawResult = ((ComDirect3DDevice2 *)(d3d6))->DrawIndexedPrimitive(groupEntryPtr->primType, groupEntryPtr->vertexType, groupEntryPtr->vertices, groupEntryPtr->vertexCount, groupEntryPtr->indices, groupEntryPtr->indexCount, groupEntryPtr->flags);
                                    switch (drawResult) {
                                    case DD_OK:
                                        break;
                                    case DDERR_WASSTILLDRAWING:
                                        D3D5CountWasStillDrawing(1, context);
                                        break;
                                    default:
                                        // STRING: FF7 0x00912e40
                                        CheckDirectDrawResult(drawResult, "C:\\lib\\src\\graphics\\dx_rend5.cpp", 0x6C8);
                                        drawResult = 0;
                                    }
                                } while (drawResult != 0);
                            }
                        }
                    }
                    if (calcGroup != 0) {
                        calcGroup++;
                    }
                    if (auxCursor != 0) {
                        auxCursor += 0x38;
                    }
                }
            }
            curGroupNodeAddr = (GfxDrawGroupNode *)(curGroupNodeAddr->next);
        }
    }
    if (DEAD_IN_RETAIL != 0) {
        // STRING: FF7 0x00912e64
        sprintf((char *)buffer, "ASYNC COUNT %d \n", g_GfxStillDrawingCount);
        Gfx_DebugPrint((char *)buffer);
        g_GfxStillDrawingCount = 0;
    }
}
} /* extern "C" */
