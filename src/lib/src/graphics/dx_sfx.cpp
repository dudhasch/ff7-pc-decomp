// src/lib/src/graphics/dx_sfx.cpp
// Original: C:\lib\src\graphics\dx_sfx.cpp
// Contents: Render object and primitive pool for the Direct3D effects path: locks and unlocks
//   execute buffers, creates flat, Gouraud and textured rect and triangle primitives, manages
//   primitive mesh banks, and resets and trims the render object pool.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include <windows.h>
#include "lib/codegen.h"
#include <d3d.h>
#include "lib/api/graphics.h"
#include "lib/api/mem.h"
#include "lib/api/polygon.h"
#include "lib/types/graphics.h"
#include "lib/types/file.h"
#include "lib/types/mem.h"
#include "lib/types/polygon.h"
#include "lib/enums/graphics.h"
#include "lib/api/file.h"
#include "lib/pfile.h"

struct GfxEngineContext;
struct PolygonTexturePair;
struct GfxMatrixArray;
extern "C" {
/* Retail caller passes explicit 3-arg struct-return buffer; definition returns 8-byte DxTextureResult in EDX:EAX. */
extern DxTextureResult __cdecl Pfile_LoadRsdTexture(int options, int fileName);

// clang-format off
// GLOBAL: FF7 0x0090cbc8
GfxExecPosition g_GfxDefaultQuadVertices[6] = {{(-1035468800), (-1035468800), 0}, {(-1035468800), 1112014848, 0}, {1112014848, (-1035468800), 0}, {1112014848, (-1035468800), 0}, {(-1035468800), 1112014848, 0}, {1112014848, 1112014848, 0}};
// GLOBAL: FF7 0x0090cc10
struct GfxDefaultQuadTexCoord g_GfxDefaultQuadTexCoords[6] = {{0}, {0, FLOAT_ONE_BITS}, {FLOAT_ONE_BITS, 0}, {FLOAT_ONE_BITS, 0}, {0, FLOAT_ONE_BITS}, {FLOAT_ONE_BITS, FLOAT_ONE_BITS}};
// The __FILE__ literals of the functions up to Gfx_DestroyPrimitiveMeshBank precede the table's sixteen names in
// retail (0x0090CC98..0x0090CDD7): each is returned by a static __inline function, a parse-time unit ahead of
// the table, and under /Ob1 the call expands to the plain push.
static __inline char *DxSfxCpp_FileLine018(void)
{
    // STRING: FF7 0x0090cc98
    return "C:\\lib\\src\\graphics\\dx_sfx.cpp";
}
static __inline char *DxSfxCpp_FileLine01D(void)
{
    // STRING: FF7 0x0090ccb8
    return "C:\\lib\\src\\graphics\\dx_sfx.cpp";
}
static __inline char *DxSfxCpp_FileLine02C(void)
{
    // STRING: FF7 0x0090ccd8
    return "C:\\lib\\src\\graphics\\dx_sfx.cpp";
}
static __inline char *DxSfxCpp_FileLine052(void)
{
    // STRING: FF7 0x0090ccf8
    return "C:\\lib\\src\\graphics\\dx_sfx.cpp";
}
static __inline char *DxSfxCpp_FileLine253(void)
{
    // STRING: FF7 0x0090cd18
    return "C:\\lib\\src\\graphics\\dx_sfx.cpp";
}
static __inline char *DxSfxCpp_FileLine254(void)
{
    // STRING: FF7 0x0090cd38
    return "C:\\lib\\src\\graphics\\dx_sfx.cpp";
}
static __inline char *DxSfxCpp_FileLine255(void)
{
    // STRING: FF7 0x0090cd58
    return "C:\\lib\\src\\graphics\\dx_sfx.cpp";
}
static __inline char *DxSfxCpp_FileLine256(void)
{
    // STRING: FF7 0x0090cd78
    return "C:\\lib\\src\\graphics\\dx_sfx.cpp";
}
static __inline char *DxSfxCpp_FileLine257(void)
{
    // STRING: FF7 0x0090cd98
    return "C:\\lib\\src\\graphics\\dx_sfx.cpp";
}
static __inline char *DxSfxCpp_FileLine263(void)
{
    // STRING: FF7 0x0090cdb8
    return "C:\\lib\\src\\graphics\\dx_sfx.cpp";
}
// The sixteen mesh-name pointers Gfx_CreatePrimitiveMeshBank reads: the primitive meshes of the 2D and 3D
// draw paths ("TF2D.P".."SGT3D.P", allocated from this TU's string pool after the helpers above).
// GLOBAL: FF7 0x0090cc40
int g_GfxPrimitiveMeshNames[16] = {
    (int)"TF2D.P", (int)"TF3D.P", (int)"TG2D.P", (int)"TG3D.P",
    (int)"T2D.P", (int)"T3D.P", (int)"TGT2D.P", (int)"TGT3D.P",
    (int)"SF2D.P", (int)"SF3D.P", (int)"SG2D.P", (int)"SG3D.P",
    (int)"S2D.P", (int)"S3D.P", (int)"SGT2D.P", (int)"SGT3D.P"};
// The ARGB channel-mask template PixelFormatLoadMaskTemplate copies (it reads +0x00..+0x14, 24 bytes).
// GLOBAL: FF7 0x0090cc80
struct GfxPixelMaskTemplate g_GfxPixelFormatMaskTemplate = {0, 32, 255, 0xff00, 0xff0000, -0x1000000};
// clang-format on

void __cdecl DestroyRenderObject(void *renderObject);
int __cdecl Gfx_AllocUntexturedHundred(int gouraud, GfxTexturedHundredOptions *desc);
void __cdecl Gfx_ForEachPolygonSetPartPair(DxGroupPairCallback callback, int left, int right);
void __cdecl Gfx_ForEachPolygonSetPart(DxGroupPartCallback callback, int model);

// src/lib/src/polygon/rsd.cpp

// Prototypes of callees the C sources used implicitly (definitions elsewhere), required in C++.

/*
 * Frees vertex and index buffer allocations for polygon set groups.
 */
// FUNCTION: FF7 0x006700e0
void __cdecl Gfx_FreePolygonSetGroups(int count, int groups)
{
    GfxPolygonSetEntry *group;
    int i;

    if (groups != 0) {
        group = (GfxPolygonSetEntry *)groups;
        for (i = 0; i < count; i++) {
            GameHeapFree((void *)group->lightRecords, DxSfxCpp_FileLine018(), 0x18);
            group++;
        }
        GameHeapFree((void *)groups, DxSfxCpp_FileLine01D(), 0x1D);
    }
}

/*
 * Unlocks execute buffers for a polygon set group.
 */
// FUNCTION: FF7 0x00670144
void __cdecl Gfx_UnlockPolygonSetGroup(int group, GfxPolygonSet *model, GfxEngineContext *engine)
{
    if (model != 0) {
        if (group < (model->groupCount)) {
            if ((engine->useDirect3D5) != 0) { /* empty first arm: lib/codegen.h */
            } else {
                CheckDirectDrawResult(IDirect3DExecuteBuffer_Unlock((LPDIRECT3DEXECUTEBUFFER)model->execBuffers[group]),
                                      DxSfxCpp_FileLine02C(), 0x2c);
            }
        }
    }
}

/*
 * Locks execute buffers for a polygon set group and returns pointers.
 */
// FUNCTION: FF7 0x00670196
int __cdecl Gfx_LockPolygonSetGroup(int group, GfxPolygonSet *model, GfxEngineContext *engine)
{
    struct {
        D3DEXECUTEBUFFERDESC descriptor;
        LPD3DEXECUTEBUFFERDESC view;
        int resource;
        GfxPolygonSetEntry *locked;
    } frame;
    frame.locked = 0;
    if (model != 0 && (model->entries) != 0) {
        if (group < (model->groupCount)) {
            if ((engine->useDirect3D5) != 0) {
                if ((model->batchList) != 0) {
                    frame.resource = (int)model->batchList[group];
                    if (frame.resource != 0) {
                        frame.locked = &model->entries[group];
                        frame.locked->vertices = (*(int *)((char *)frame.resource + 0x10));
                    }
                }
            } else {
                frame.view = &frame.descriptor;
                frame.view->dwSize = sizeof(D3DEXECUTEBUFFERDESC);
                frame.view->dwFlags = 0;
                if (CheckDirectDrawResult(IDirect3DExecuteBuffer_Lock((LPDIRECT3DEXECUTEBUFFER)model->execBuffers[group], frame.view),
                                          DxSfxCpp_FileLine052(), 0x52) != 0) {
                    if (frame.view->lpData != 0) {
                        frame.locked = &model->entries[group];
                        frame.locked->vertices = (int)frame.view->lpData;
                    }
                }
            }
        }
    }
    return (int)frame.locked;
}

/*
 * Converts Gouraud shading flag to Direct3D shade mode.
 */
// FUNCTION: FF7 0x0067028e
int __cdecl Gfx_ShadeModeFromGouraudFlag(int value)
{
    int mode;
    switch (value) {
    case GFX_SHADE_FLAG_FLAT:
        mode = D3DSHADE_FLAT;
        break;
    case GFX_SHADE_FLAG_GOURAUD:
        mode = D3DSHADE_GOURAUD;
        break;
    default:
        mode = D3DSHADE_FLAT;
        break;
    }
    return mode;
}

/*
 * Initializes flat-shaded primitive record with specified render parameters.
 */
// FUNCTION: FF7 0x006702c8
void __cdecl Gfx_MakeHundredFlat(int enable, int gouraud, int blendMode, PolygonHundredState *record)
{
    int value;

    if (record != 0) {
        value = 0x20002;
        record->changeMask = value;
        record->flags = 0;
        record->flags |= 0x20000;
        record->shadeMode = Gfx_ShadeModeFromGouraudFlag(gouraud);
        record->flags |= 0x200;
        record->flags |= 0x8000;
        record->ambientColor = 0xFFFFFFFF;
        Pfile_SetHundredBlendMode(enable, blendMode, record);
    }
}

/*
 * Allocates and initializes untextured primitive record.
 */
// FUNCTION: FF7 0x00670350
int __cdecl Gfx_AllocUntexturedHundred(int gouraud, GfxTexturedHundredOptions *desc)
{
    int hundred;

    hundred = Gfx_AllocPHundred();
    if (hundred != 0) {
        Gfx_MakeHundredFlat(1, gouraud, desc->blendMode, (struct PolygonHundredState *)hundred);
    }
    return hundred;
}

/*
 * Initializes textured primitive record with specified texture and blending.
 */
// FUNCTION: FF7 0x00670382
void __cdecl Gfx_MakeHundredTextured(int enable, int gouraud, int blendMode, PolygonHundredState *record)
{
    int value;

    if (record != 0) {
        value = 0x20002;
        record->changeMask = value;
        record->flags = 0;
        record->flags |= 2;
        record->flags |= 0x20000;
        record->shadeMode = Gfx_ShadeModeFromGouraudFlag(gouraud);
        record->flags |= 4;
        record->flags |= 8;
        record->flags |= 0x200;
        record->flags |= 0x8000;
        record->ambientColor = 0xFFFFFFFF;
        Pfile_SetHundredBlendMode(enable, blendMode, record);
    }
}

/*
 * Releases primitive record and destroys associated render state.
 */
// FUNCTION: FF7 0x00670439
void __cdecl Gfx_DestroyPHundredRecord(int releaseTexture, PolygonHundredRecord *record)
{
    if (record != 0) {
        if (record->textureHandle != 0) {
            if (releaseTexture != 0) {
                Gfx_ReleaseCachedTexture(*(int *)((char *)record->textureHandle + 0x84));
            }
            Gfx_ReleaseTextureRecordRef(record->textureHandle);
        }
        Gfx_FreePHundred((int)record);
    }
}

/*
 * Frees textured primitive record memory allocation.
 */
// FUNCTION: FF7 0x00670483
void __cdecl Gfx_DestroyTexturedHundred(int value)
{
    Gfx_DestroyPHundredRecord(1, (PolygonHundredRecord *)value);
}

/*
 * Allocates and initializes textured primitive record.
 */
// FUNCTION: FF7 0x00670496
int __cdecl Gfx_AllocTexturedHundred(int kind, int gouraud, int texName, int options, int context)
{
    struct {
        int uploaded;
        int failed;
        DxTextureResult result;
        int hundred;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    frame.hundred = Gfx_AllocPHundred();
    if (frame.hundred != 0) {
        if ((((GfxTexturedHundredOptions *)options)->texture) != 0) {
            frame.result.status = 0;
            frame.result.tim = 0;
            frame.result.texture = (((GfxTexturedHundredOptions *)options)->texture);
        } else {
            frame.result = Pfile_LoadRsdTexture(options, texName);
        }
        frame.failed = frame.result.status;
        if (frame.failed == 0) {
            if (frame.result.texture != 0) {
                if (((((GfxTexturedHundredOptions *)options)->flags) & 0x20) != 0)
                    ((GfxTextureObject *)frame.result.texture)->flag = 1;
                (((GfxTextureObject *)frame.result.texture)->paletteRow) = (((GfxTexturedHundredOptions *)options)->paletteRow);
            }
            Gfx_MakeHundredTextured(1, gouraud, (((GfxTexturedHundredOptions *)options)->blendMode), (struct PolygonHundredState *)frame.hundred);
            Pfile_CopyBlendStateToTexHeader((PolygonTextureRecord *)frame.result.texture, (PolygonHundredRecord *)frame.hundred);
            frame.uploaded = Gfx_AcquireTextureForRequest(options, frame.result.texture, context);
            if (frame.uploaded != 0) {
                (((GfxTexturedHundredRecord *)frame.hundred)->kind) = kind;
                (((GfxTexturedHundredRecord *)frame.hundred)->uploaded) = frame.uploaded;
                Pfile_DestroyTim((struct PolygonTim *)frame.result.tim);
            } else {
                frame.failed = 1;
            }
        } else {
            frame.failed = 1;
        }
        if (frame.failed != 0) {
            if ((((GfxTexturedHundredOptions *)options)->texture) == 0)
                Pfile_FreeTexturePairRecord((PolygonTexturePair *)&frame.result);
            Gfx_FreePHundred(frame.hundred);
            frame.hundred = 0;
        }
    }
    return frame.hundred;
}

/*
 * Constructs primitive execute buffer mesh with vertices and instructions.
 */
// FUNCTION: FF7 0x006705ed
int __cdecl Gfx_BuildPrimitiveMesh(int kind, int fileName)
{
    int positionX[3];
    int group[14];
    PolygonFileArrays *polygonData;
    int polyCount;
    GfxPrimitiveMeshVertex *texVert;
    int textureCoord[2];
    int shade;
    int index;
    GfxPrimitiveMeshVertex *normal;
    int polygon[6];
    int vertexCnt;
    int *vertexIdRef;
    GfxPrimitiveMeshVertex *groupPtr;
    int spareVal[3];
    GfxPrimitiveMeshVertex *recPtr;
    int vertexHandle;
    int textureFlag;
    GfxPrimitiveMeshVertex *selVertex;
    int built;

    texVert = (GfxPrimitiveMeshVertex *)textureCoord;
    selVertex = (GfxPrimitiveMeshVertex *)positionX;
    normal = (GfxPrimitiveMeshVertex *)spareVal;
    normal = 0;
    vertexIdRef = &vertexHandle;
    recPtr = (GfxPrimitiveMeshVertex *)polygon;
    built = 0;
    textureFlag = 0;
    polygonData = 0;
    shade = 0;
    polyCount = 0;
    vertexCnt = 0;
    switch (kind) {
    case GFX_DRAW_FLAT_TRI_2D:
        shade = 0;
        polyCount = 1;
        vertexCnt = 3;
        polygonData = Pfile_CreatePolygonData(1, 2);
        break;
    case GFX_DRAW_FLAT_TRI_3D:
        shade = 0;
        polyCount = 1;
        vertexCnt = 3;
        polygonData = Pfile_CreatePolygonData(1, 1);
        break;
    case GFX_DRAW_GOURAUD_TRI_2D:
        shade = 1;
        polyCount = 1;
        vertexCnt = 3;
        polygonData = Pfile_CreatePolygonData(1, 2);
        break;
    case GFX_DRAW_GOURAUD_TRI_3D:
        shade = 1;
        polyCount = 1;
        vertexCnt = 3;
        polygonData = Pfile_CreatePolygonData(1, 1);
        break;
    case GFX_DRAW_TEXTURED_TRI_2D:
        textureFlag = 1;
        shade = 2;
        polyCount = 1;
        vertexCnt = 3;
        polygonData = Pfile_CreatePolygonData(1, 2);
        break;
    case GFX_DRAW_TEXTURED_TRI_3D:
        textureFlag = 1;
        shade = 2;
        polyCount = 1;
        vertexCnt = 3;
        polygonData = Pfile_CreatePolygonData(1, 1);
        break;
    case GFX_DRAW_TEXTURED_GOURAUD_TRI_2D:
        textureFlag = 1;
        shade = 4;
        polyCount = 1;
        vertexCnt = 3;
        polygonData = Pfile_CreatePolygonData(1, 2);
        break;
    case GFX_DRAW_TEXTURED_GOURAUD_TRI_3D:
        textureFlag = 1;
        shade = 4;
        polyCount = 1;
        vertexCnt = 3;
        polygonData = Pfile_CreatePolygonData(1, 1);
        break;
    case GFX_DRAW_FLAT_QUAD_2D:
        shade = 0;
        polyCount = 2;
        vertexCnt = 6;
        polygonData = Pfile_CreatePolygonData(1, 2);
        break;
    case GFX_DRAW_FLAT_QUAD_3D:
        shade = 0;
        polyCount = 2;
        vertexCnt = 6;
        polygonData = Pfile_CreatePolygonData(1, 1);
        break;
    case GFX_DRAW_GOURAUD_QUAD_2D:
        shade = 1;
        polyCount = 2;
        vertexCnt = 6;
        polygonData = Pfile_CreatePolygonData(1, 2);
        break;
    case GFX_DRAW_GOURAUD_QUAD_3D:
        shade = 1;
        polyCount = 2;
        vertexCnt = 6;
        polygonData = Pfile_CreatePolygonData(1, 1);
        break;
    case GFX_DRAW_TEXTURED_QUAD_2D:
        textureFlag = 1;
        shade = 2;
        polyCount = 2;
        vertexCnt = 6;
        polygonData = Pfile_CreatePolygonData(1, 2);
        break;
    case GFX_DRAW_TEXTURED_QUAD_3D:
        textureFlag = 1;
        shade = 2;
        polyCount = 2;
        vertexCnt = 6;
        polygonData = Pfile_CreatePolygonData(1, 1);
        break;
    case GFX_DRAW_TEXTURED_GOURAUD_QUAD_2D:
        textureFlag = 1;
        shade = 4;
        polyCount = 2;
        vertexCnt = 6;
        polygonData = Pfile_CreatePolygonData(1, 2);
        break;
    case GFX_DRAW_TEXTURED_GOURAUD_QUAD_3D:
        textureFlag = 1;
        shade = 4;
        polyCount = 2;
        vertexCnt = 6;
        polygonData = Pfile_CreatePolygonData(1, 1);
    }
    if (polygonData != 0) {
        for (index = 0; index < vertexCnt; index++) {
            selVertex->shade = g_GfxDefaultQuadVertices[index].x;
            selVertex->v = g_GfxDefaultQuadVertices[index].y;
            selVertex->polyCount = g_GfxDefaultQuadVertices[index].z;
            texVert->shade = g_GfxDefaultQuadTexCoords[index].u;
            texVert->v = g_GfxDefaultQuadTexCoords[index].v;
            *vertexIdRef = 0xFFFFFFFF;
            if (textureFlag != 0) {
                Pfile_AddVertex(index, vertexIdRef, texVert, normal, selVertex, polygonData);
            } else {
                Pfile_AddVertex(index, vertexIdRef, 0, normal, selVertex, polygonData);
            }
        }
        ZeroFillMemory32(24, recPtr);
        *(short *)&recPtr->shade = 0;
        ((GfxPrimitiveRecord *)recPtr)->kind = 2;
        *(short *)&recPtr->v = 1;
        ((GfxPrimitiveRecord *)recPtr)->mode = 0;
        Pfile_AddPolygon(recPtr, polygonData);
        if (polyCount > 1) {
            ZeroFillMemory32(24, recPtr);
            *(short *)&recPtr->shade = 0;
            ((GfxPrimitiveRecord *)recPtr)->kind = 5;
            *(short *)&recPtr->v = 4;
            ((GfxPrimitiveRecord *)recPtr)->mode = 3;
            Pfile_AddPolygon(recPtr, polygonData);
        }
        groupPtr = (GfxPrimitiveMeshVertex *)group;
        ZeroFillMemory32(56, groupPtr);
        groupPtr->shade = shade;
        groupPtr->v = 0;
        groupPtr->polyCount = polyCount;
        groupPtr->firstPosition = 0;
        groupPtr->vertexCnt = vertexCnt;
        groupPtr->lightIndex = 0;
        groupPtr->textureFlag = textureFlag;
        groupPtr->refIndex = 0;
        Pfile_AddGroup(groupPtr, polygonData);
        Pfile_FlattenEditLists(polygonData);
        Pfile_WriteBinary(fileName, polygonData);
        Pfile_Free(polygonData);
        built = 1;
    }
    return built;
}

/*
 * Releases all primitive execute buffers in a mesh bank.
 */
// FUNCTION: FF7 0x00670c97
void __cdecl Gfx_DestroyPrimitiveMeshBank(GfxPrimitiveMeshBank *bank)
{
    int i;

    if (bank != 0) {
        GameHeapFree((void *)bank->triIndicesReversed, DxSfxCpp_FileLine253(), 0x253);
        GameHeapFree((void *)bank->quadIndicesReversed, DxSfxCpp_FileLine254(), 0x254);
        GameHeapFree((void *)bank->triIndices, DxSfxCpp_FileLine255(), 0x255);
        GameHeapFree((void *)bank->quadIndices, DxSfxCpp_FileLine256(), 0x256);
        GameHeapFree((void *)bank->lineIndices, DxSfxCpp_FileLine257(), 0x257);
        for (i = 0; i < 16; i++) {
            Pfile_Free(bank->meshFiles[i]);
        }
        DestroyBlockHeapChain(bank->indexHeap);
        DestroyBlockHeapChain(bank->entryHeap);
        DestroyBlockHeapChain(bank->recordHeap);
        DestroyBlockHeapChain(bank->vertexHeap);
        GameHeapFree(bank, DxSfxCpp_FileLine263(), 0x263);
    }
}

/*
 * Allocates and initializes primitive execute buffer mesh bank.
 */
// FUNCTION: FF7 0x00670da4
int __cdecl Gfx_CreatePrimitiveMeshBank(int recordCount, int vertexCount, int indexCount, int entryCount, int extraFlags)
{
    int idx;
    int fileName;
    int recordSize;
    GfxPrimitiveMeshBank *meshBankPtr;
    int failed;
    int indexCap;
    char rec[16];

    indexCap = 512;
    failed = 1;
    // STRING: FF7 0x0090ce58
    meshBankPtr = (GfxPrimitiveMeshBank *)GameHeapAlloc(1, 112, "C:\\lib\\src\\graphics\\dx_sfx.cpp", 648);
    if (meshBankPtr != 0) {
        meshBankPtr->recordHeap = (MemBlockHeap *)CreateBlockHeap(224, recordCount * 0xE0, 0);
        if (meshBankPtr->recordHeap != 0) {
            recordSize = 160;
            meshBankPtr->device = (GfxEngineContext *)extraFlags;
            meshBankPtr->vertexHeap = (MemBlockHeap *)CreateBlockHeap(132, vertexCount * 0x84, 0);
            meshBankPtr->indexHeap = (MemBlockHeap *)CreateBlockHeap(52, indexCount * 0x34, 0);
            meshBankPtr->entryHeap = (MemBlockHeap *)CreateBlockHeap(recordSize, entryCount * recordSize, 0);
            if (meshBankPtr->vertexHeap != 0 && meshBankPtr->indexHeap != 0 &&
                meshBankPtr->entryHeap != 0) {
                failed = 0;
                for (idx = 0; idx < 16; idx++) {
                    fileName = g_GfxPrimitiveMeshNames[idx];
                    if (Gfx_BuildPrimitiveMesh(idx, fileName) != 0) {
                        InitFileContext(0, rec);
                        meshBankPtr->meshFiles[idx] = Pfile_LoadBinary((FileOpenRequest *)rec, 0, fileName);
                        if (meshBankPtr->meshFiles[idx] != 0) {
                            /* loaded */
                        } else {
                            failed = 1;
                            break;
                        }
                    } else {
                        failed = 1;
                        break;
                    }
                }
                Gfx_AllocSpriteIndexBuffers(indexCap, meshBankPtr);
            }
        }
    }
    if (failed != 0) {
        Gfx_DestroyPrimitiveMeshBank(meshBankPtr);
        meshBankPtr = 0;
    }
    return (int)meshBankPtr;
}

/*
 * Resets render object pool counters and links.
 */
// FUNCTION: FF7 0x00670f4f
void __cdecl Gfx_ResetRenderObjectPool(int pool)
{
    if (pool != 0) {
        ResetBlockHeapChain(((GfxPrimitiveMeshBank *)pool)->vertexHeap);
        ResetBlockHeapChain(((GfxPrimitiveMeshBank *)pool)->indexHeap);
        ResetBlockHeapChain(((GfxPrimitiveMeshBank *)pool)->entryHeap);
        ((GfxPrimitiveMeshBank *)pool)->clearedCounterA = 0;
        ((GfxPrimitiveMeshBank *)pool)->clearedCounterB = 0;
    }
}

/*
 * Trims excess allocated render pool entries to maximum capacity.
 */
// FUNCTION: FF7 0x00670f9b
void __cdecl Gfx_TrimRenderPoolOverflow(GfxRenderPool *pool)
{
    if (pool != 0) {
        TrimBlockHeapOverflow(pool->blockHeapA);
        TrimBlockHeapOverflow(pool->blockHeapB);
        TrimBlockHeapOverflow(pool->blockHeapC);
    }
}

/*
 * Frees a render object that Gfx_CreateRenderObject built; game modules call it when they unload
 * their sprites, windows and effects. It removes the object's polygon set from the engine
 * context's polygon-set list, destroys its render-state record (releasing the cached texture
 * unless the load options supplied it, releaseTexture 0), destroys its matrix array (member
 * material), frees the polygon set, the sprite batch and the vertex buffer, and returns the
 * 0xE0-byte block to the render-object bank's heap. A null object, or one without a bank, is
 * ignored. The caller's pointer is left dangling; DestroyRenderObjectAndClear also nulls it.
 */
// FUNCTION: FF7 0x00670fd3
void __cdecl DestroyRenderObject(void *renderObject)
{
    GfxRenderObject *block;

    if (renderObject != 0 && ((GfxRenderObject *)renderObject)->init != 0) {
        D3DRemovePolygonSetFromList((void *)((GfxRenderObject *)renderObject)->polygonSet, ((GfxPrimitiveMeshBank *)((GfxRenderObject *)renderObject)->init)->device);
        Gfx_DestroyPHundredRecord(((GfxRenderObject *)renderObject)->releaseTexture, (PolygonHundredRecord *)((GfxRenderObject *)renderObject)->hundredIndex);
        DestroyRenderMaterial((struct GfxRenderMaterial *)(((GfxRenderObject *)renderObject)->material));
        D3DFreePolygonSet((struct GfxPolygonSet *)(((GfxRenderObject *)renderObject)->polygonSet));
        Gfx_DestroySpriteBatch((GfxSpriteBatch *)((GfxRenderObject *)renderObject)->spriteBatch);
        // STRING: FF7 0x0090ce78
        GameHeapFree((void *)((GfxRenderObject *)renderObject)->vertexBuffer, "C:\\lib\\src\\graphics\\dx_sfx.cpp", 750);
        block = (GfxRenderObject *)renderObject;
        FreeBlockToHeap(block, ((GfxPrimitiveMeshBank *)((GfxRenderObject *)renderObject)->init)->recordHeap);
    }
}

/*
 * Frees the render object a variable holds and nulls the variable: when `slotPtr` and the pointer
 * it holds are both nonzero, it calls DestroyRenderObject on that object and stores 0 back, so a
 * module can unload the same slot twice safely. No COM interface is involved.
 */
// FUNCTION: FF7 0x00671082
void __cdecl DestroyRenderObjectAndClear(void *slotPtr)
{
    if (slotPtr != 0 && *(int *)slotPtr != 0) {
        DestroyRenderObject(*(void **)slotPtr);
        *(int *)slotPtr = 0;
    }
}

/*
 * Builds a render object: the container a game module fills with primitives of one GfxDrawKind
 * each frame (AcquireBatchNode and the Gfx_Emit* writers) and draws with Gfx_EmitPrimitiveBatchList
 * or Gfx_DrawRenderObjectCall. `init` is the render-object bank (context +0x910), `loadOptions`
 * the PfileLoadOptions and `name` the texture file for textured kinds. `owner` is stored in the
 * polygon set's word +0x04, which the Direct3D draw emitters read as a clipping switch: 0 draws
 * with D3DDP_DONOTCLIP. GFX_DRAW_SPRITE_BATCH is built as GFX_DRAW_TEXTURED_QUAD_2D but keeps its
 * own kind.
 *
 * 1. Allocates the render-state record ("hundred") for the kind: untextured flat or Gouraud, or
 *    textured (Gfx_AllocTexturedHundred loads `name`, from the options' directory when one is
 *    given, unless the options carry a texture). Lines (0x12, 0x13) get an untextured record and
 *    no template mesh.
 * 2. Takes a zeroed 0xE0-byte block from the bank's heap and records the kind, the record, the
 *    bank, the draw pool (the context's, or the options' own) and whether the caller owns the
 *    texture.
 * 3. With the bank's template mesh for the kind (built at startup from TF2D.P .. SGT3D.P): a
 *    matrix array of the options' matrixCount with an identity first matrix, and a polygon set
 *    loaded from the template (3D kinds flagged as such) that is appended to the context's
 *    polygon-set list. drawUnsorted goes to the polygon set's drawUnsorted (1 at most
 *    calls): the GL driver draws a group of a set that is not depth-sorted only when it is set. Without one (lines): a bare polygon set.
 * 4. For textured kinds, 1/width and 1/height of the texture into invWidth/invHeight; then the
 *    driver's begin and draw emitters for the options' emitterFlags (GfxSelectPrimitiveEmitters).
 *
 * Returns the object, or null when the options or the bank are null or any step fails (the
 * partial object is destroyed). The textured cases of step 1 also ask for the texture size while
 * the object is still null, which does nothing. A kind outside 0x00..0x0F, 0x11..0x13 leaves the
 * render-state record uninitialised and the function then tests that stack garbage.
 */
// FUNCTION: FF7 0x006710ac
void *__cdecl Gfx_CreateRenderObject(int drawUnsorted, int kind, PfileLoadOptions *loadOptions, char *name, void *init)
{
    int polyFlag;
    char savedCtx[200];
    GfxRenderTextureInfo textureRecord;
    GfxMatrixArray *matricesPtr;
    GfxRenderObject *allocated;
    PolygonFileArrays *lightModelPtr;
    GfxRenderTextureInfo textureCell;
    GfxMatrix4x4 *pIdentityMatrix;
    GfxRenderTextureInfo tmpTextureInfo;
    int texTag;
    int failed;
    GfxRenderObject *result;
    int hasModel8;
    int rawKind8;
    int innerHundred;

    failed = 1;
    hasModel8 = 1;
    rawKind8 = kind;
    result = 0;
    if (kind == GFX_DRAW_SPRITE_BATCH) {
        kind = 0xC;
    }
    if (loadOptions != 0 && init != 0) {
        texTag = 0;
        switch (kind) {
        case GFX_DRAW_FLAT_TRI_2D:
        case GFX_DRAW_GOURAUD_TRI_2D:
        case GFX_DRAW_TEXTURED_TRI_2D:
        case GFX_DRAW_TEXTURED_GOURAUD_TRI_2D:
        case GFX_DRAW_FLAT_QUAD_2D:
        case GFX_DRAW_GOURAUD_QUAD_2D:
        case GFX_DRAW_TEXTURED_QUAD_2D:
        case GFX_DRAW_TEXTURED_GOURAUD_QUAD_2D:
            polyFlag = 0;
            break;
        case GFX_DRAW_FLAT_TRI_3D:
        case GFX_DRAW_GOURAUD_TRI_3D:
        case GFX_DRAW_TEXTURED_TRI_3D:
        case GFX_DRAW_TEXTURED_GOURAUD_TRI_3D:
        case GFX_DRAW_FLAT_QUAD_3D:
        case GFX_DRAW_GOURAUD_QUAD_3D:
        case GFX_DRAW_TEXTURED_QUAD_3D:
        case GFX_DRAW_TEXTURED_GOURAUD_QUAD_3D:
            polyFlag = 1;
            break;
        case GFX_DRAW_FLAT_LINE_2D:
        case GFX_DRAW_GOURAUD_LINE_2D:
            polyFlag = 0;
            break;
        default:
            polyFlag = 0;
        }
        switch (kind) {
        case GFX_DRAW_FLAT_TRI_2D:
        case GFX_DRAW_FLAT_TRI_3D:
        case GFX_DRAW_FLAT_QUAD_2D:
        case GFX_DRAW_FLAT_QUAD_3D:
            /* Retail caller passes 3 arg(s) to 2-parameter `Gfx_AllocUntexturedHundred` (__cdecl stack cleanup matches retail). */
            innerHundred = ((int(__cdecl *)(int, void *, int))Gfx_AllocUntexturedHundred)(0, loadOptions, (int)((GfxPrimitiveMeshBank *)init)->device);
            break;
        case GFX_DRAW_GOURAUD_TRI_2D:
        case GFX_DRAW_GOURAUD_TRI_3D:
        case GFX_DRAW_GOURAUD_QUAD_2D:
        case GFX_DRAW_GOURAUD_QUAD_3D:
            /* Retail caller passes 3 arg(s) to 2-parameter `Gfx_AllocUntexturedHundred` (__cdecl stack cleanup matches retail). */
            innerHundred = ((int(__cdecl *)(int, void *, int))Gfx_AllocUntexturedHundred)(1, loadOptions, (int)((GfxPrimitiveMeshBank *)init)->device);
            break;
        case GFX_DRAW_TEXTURED_TRI_2D:
        case GFX_DRAW_TEXTURED_TRI_3D:
        case GFX_DRAW_TEXTURED_QUAD_2D:
        case GFX_DRAW_TEXTURED_QUAD_3D:
            if (loadOptions != 0 && loadOptions->directory != 0) {
                PushFileContextDirectory(&loadOptions->file, loadOptions->directory, savedCtx);
            }
            innerHundred = Gfx_AllocTexturedHundred(texTag, 0, (int)name, (int)loadOptions, (int)((GfxPrimitiveMeshBank *)init)->device);
            if (loadOptions != 0 && loadOptions->directory != 0) {
                PushFileContextDirectory(&loadOptions->file, (char *)savedCtx, 0);
            }
            if (Gfx_GetRenderObjectTextureInfo(&textureRecord, result) != 0) {
                result->invWidth = 1.0f / (float)textureRecord.width;
                result->invHeight = 1.0f / (float)textureRecord.height;
            }
            break;
        case GFX_DRAW_TEXTURED_GOURAUD_TRI_2D:
        case GFX_DRAW_TEXTURED_GOURAUD_TRI_3D:
        case GFX_DRAW_TEXTURED_GOURAUD_QUAD_2D:
        case GFX_DRAW_TEXTURED_GOURAUD_QUAD_3D:
            if (loadOptions != 0 && loadOptions->directory != 0) {
                PushFileContextDirectory(&loadOptions->file, loadOptions->directory, savedCtx);
            }
            innerHundred = Gfx_AllocTexturedHundred(texTag, 1, (int)name, (int)loadOptions, (int)((GfxPrimitiveMeshBank *)init)->device);
            if (loadOptions != 0 && loadOptions->directory != 0) {
                PushFileContextDirectory(&loadOptions->file, (char *)savedCtx, 0);
            }
            if (Gfx_GetRenderObjectTextureInfo(&textureCell, result) != 0) {
                result->invWidth = 1.0f / (float)textureCell.width;
                result->invHeight = 1.0f / (float)textureCell.height;
            }
            break;
        case GFX_DRAW_FLAT_LINE_2D:
            /* Retail caller passes 3 arg(s) to 2-parameter `Gfx_AllocUntexturedHundred` (__cdecl stack cleanup matches retail). */
            innerHundred = ((int(__cdecl *)(int, void *, int))Gfx_AllocUntexturedHundred)(0, loadOptions, (int)((GfxPrimitiveMeshBank *)init)->device);
            hasModel8 = 0;
            break;
        case GFX_DRAW_GOURAUD_LINE_2D:
            /* Retail caller passes 3 arg(s) to 2-parameter `Gfx_AllocUntexturedHundred` (__cdecl stack cleanup matches retail). */
            innerHundred = ((int(__cdecl *)(int, void *, int))Gfx_AllocUntexturedHundred)(1, loadOptions, (int)((GfxPrimitiveMeshBank *)init)->device);
            hasModel8 = 0;
        }
        if (innerHundred != 0) {
            allocated = (GfxRenderObject *)(AllocBlockFromHeap(((GfxPrimitiveMeshBank *)init)->recordHeap));
            result = allocated;
            if (result != 0) {
                ZeroFillMemory32(224, result);
                result->kind = rawKind8;
                result->hundredIndex = innerHundred;
                result->enabled = loadOptions->matrixCount > 0;
                result->releaseTexture = 1;
                if (loadOptions->texture != 0) {
                    result->releaseTexture = 0;
                }
                result->init = (int)init;
                result->drawPool = ((GfxPrimitiveMeshBank *)init)->device->drawPool;
                if (loadOptions->drawPool != 0) {
                    result->drawPool = loadOptions->drawPool;
                }
                lightModelPtr = 0;
                if (hasModel8 != 0) {
                    lightModelPtr = ((GfxPrimitiveMeshBank *)init)->meshFiles[kind];
                }
                if (lightModelPtr != 0) {
                    matricesPtr = (GfxMatrixArray *)(CreateMatrixArray(loadOptions->matrixCount));
                    if (matricesPtr != 0) {
                        pIdentityMatrix = (GfxMatrix4x4 *)Gfx_AllocIdentityMatrix4x4();
                        if (pIdentityMatrix != 0) {
                            Gfx_SetMatrixCall(0, (int)pIdentityMatrix, (int)matricesPtr, ((GfxPrimitiveMeshBank *)init)->device);
                        }
                        result->material = (int)matricesPtr;
                        result->polygonSet = (int)GfxLoadPolygonSet(matricesPtr, (struct PolygonHundredState *)innerHundred, lightModelPtr, (struct GfxPolygonSetSpec *)loadOptions, ((GfxPrimitiveMeshBank *)init)->device);
                        if (result->polygonSet != 0) {
                            ((GfxPolygonSet *)(result->polygonSet))->flags = polyFlag;
                            if (result->enabled != 0) {
                                ((GfxPolygonSet *)(result->polygonSet))->drawListActive = 1;
                            }
                            ((GfxPolygonSet *)(result->polygonSet))->drawUnsorted = drawUnsorted;
                            D3DAppendPolygonSetToList((struct GfxPolygonSet *)result->polygonSet, ((GfxPrimitiveMeshBank *)init)->device);
                            failed = 0;
                        }
                    }
                } else {
                    result->polygonSet = (int)GfxLoadPolygonSet(0, (struct PolygonHundredState *)innerHundred, 0, (struct GfxPolygonSetSpec *)loadOptions, ((GfxPrimitiveMeshBank *)init)->device);
                    if (result->polygonSet != 0) {
                        failed = 0;
                    }
                }
                switch (kind) {
                case GFX_DRAW_TEXTURED_TRI_2D:
                case GFX_DRAW_TEXTURED_TRI_3D:
                case GFX_DRAW_TEXTURED_GOURAUD_TRI_2D:
                case GFX_DRAW_TEXTURED_GOURAUD_TRI_3D:
                case GFX_DRAW_TEXTURED_QUAD_2D:
                case GFX_DRAW_TEXTURED_QUAD_3D:
                case GFX_DRAW_TEXTURED_GOURAUD_QUAD_2D:
                case GFX_DRAW_TEXTURED_GOURAUD_QUAD_3D:
                    if (Gfx_GetRenderObjectTextureInfo(&tmpTextureInfo, result) != 0) {
                        result->invWidth = 1.0f / (float)tmpTextureInfo.width;
                        result->invHeight = 1.0f / (float)tmpTextureInfo.height;
                    }
                }
                GfxSelectPrimitiveEmitters(loadOptions->emitterFlags, result);
            }
            if (failed != 0) {
                DestroyRenderObject(result);
                result = 0;
            }
        }
    }
    return (void *)result;
}

/*
 * Sets the render object's polygon set's drawUnsorted (word 1; D3DAllocPolygonSet sets it to 1,
 * Gfx_CreateRenderObject to its first argument). Nothing in src/ calls it.
 */
// FUNCTION: FF7 0x0067168e
void __cdecl Gfx_SetRenderObjectDrawUnsorted(int drawUnsorted, GfxRenderObject *renderObj)
{
    if (renderObj != 0 && renderObj->polygonSet != 0) {
        ((GfxPolygonSet *)renderObj->polygonSet)->drawUnsorted = drawUnsorted;
    }
}

/*
 * Sets the render object's polygon set's drawEnabled (word 0; D3DAllocPolygonSet sets it to 1).
 * Nothing in src/ calls it.
 */
// FUNCTION: FF7 0x006716ae
void __cdecl Gfx_SetRenderObjectDrawEnabled(int drawEnabled, GfxRenderObject *renderObj)
{
    if (renderObj != 0 && renderObj->polygonSet != 0) {
        ((GfxPolygonSet *)renderObj->polygonSet)->drawEnabled = drawEnabled;
    }
}

/*
 * Resets primitive batch chain pointers on render object.
 */
// FUNCTION: FF7 0x006716cd
void __cdecl Gfx_ResetRenderObjectBatchChain(int renderObj)
{
    if (((GfxRenderObject *)renderObj)->kind == GFX_DRAW_SPRITE_BATCH && ((GfxRenderObject *)renderObj)->spriteBatch != 0) {
        ResetBlockHeapChain(((GfxSpriteBatch *)((GfxRenderObject *)renderObj)->spriteBatch)->vertexHeap);
    }
}

/*
 * Clears active primitive batch references from render object.
 */
// FUNCTION: FF7 0x006716f5
void __cdecl Gfx_EmptyRenderObjectBatch(int renderObj)
{
    GfxRenderObjectBatch *batch;

    if (renderObj != 0 && ((GfxRenderObject *)renderObj)->polygonSet != 0) {
        batch = (GfxRenderObjectBatch *)(((GfxRenderObject *)renderObj)->polygonSet + 0x14);
        batch->count = 0;
        batch->highWater = 0;
        ((GfxRenderObject *)renderObj)->batchGeneration = 0;
        Gfx_ResetRenderObjectBatchChain(renderObj);
    }
}

/*
 * Copies one queued primitive record (a GfxBatchPrimitive that a Gfx_Emit* writer filled) into
 * the vertex buffer of its render object's polygon set, so the driver can draw it. The Direct3D,
 * Direct3D 5, OpenGL and software polygon-set drawers call it for each record they meet.
 *
 * It locks group 0 of the render object's polygon set (Gfx_LockPolygonSetGroup), selects the
 * record's texture page (`page`) on `texturePage` unless `noTexture` is set, and writes three
 * vertices (triangle kinds 0..7) or four (quad kinds) by the record's kind: position from the
 * record's `positions`, colour from `colors`, for 2D kinds the record's `rhw` slot and for 3D kinds 0
 * in that slot, and for textured kinds u and v from `texCoords`. For textured 2D kinds with the context's
 * configApplied option set, u and v are moved by half a texel (0.5 * invWidth, 0.5 * invHeight).
 * Then it unlocks the group. Kinds outside 0x00..0x0F (sprite batches, lines) write nothing. A
 * record without a render object, or a render object without a bank, is ignored.
 */
// FUNCTION: FF7 0x00671742
void __cdecl Gfx_EmitPolygonSetPrimitive(int noTexture, int texturePage, GfxDrawListNode *drawNode)
{
    GfxEngineContext *ctx;
    GfxPolygonSetEntry *setGroup;
    GfxPolygonSet *polygonSet;
    int vertNum;
    GfxRenderObject *renderNode;
    int auxPointer;
    int cnt;
    GfxPrimitiveMeshBank *source;
    GfxTlVertex *writePointer;
    GfxTlVertex *outPointer;
    int auxPos;
    int altPointer;
    GfxTlVertex *outPtr;
    int endPointer;
    GfxTlVertex *dstPointer;
    int altPtr;
    GfxTlVertex *primitivePointer;
    int auxPtr;
    GfxTlVertex *writePtr;

    renderNode = (GfxRenderObject *)(drawNode->nextNode);
    if (renderNode != 0) {
        source = (GfxPrimitiveMeshBank *)renderNode->init;
        if (source != 0) {
            polygonSet = (GfxPolygonSet *)(renderNode->polygonSet);
            ctx = (GfxEngineContext *)source->device;
            setGroup = (GfxPolygonSetEntry *)(Gfx_LockPolygonSetGroup(0, polygonSet, (struct GfxEngineContext *)source->device));
            if (setGroup != 0) {
                if (noTexture == 0) {
                    Gfx_SelectTexturePage(drawNode->page, (void *)texturePage);
                }
                if (drawNode->kind <= GFX_DRAW_TEXTURED_GOURAUD_TRI_3D) {
                    cnt = 3;
                } else {
                    cnt = 4;
                }
                switch (drawNode->kind) {
                case GFX_DRAW_FLAT_TRI_2D:
                case GFX_DRAW_FLAT_QUAD_2D:
                    writePointer = (GfxTlVertex *)setGroup->vertices;
                    auxPointer = setGroup->auxPointer;
                    for (vertNum = 0; vertNum < cnt; vertNum++) {
                        writePointer->xBits = drawNode->positions[vertNum].x;
                        writePointer->yBits = drawNode->positions[vertNum].y;
                        writePointer->zBits = drawNode->positions[vertNum].z;
                        writePointer->rhwBits = drawNode->rhw[vertNum];
                        writePointer->color = drawNode->colors[vertNum];
                        auxPointer += 0x20;
                        writePointer++;
                    }
                    break;
                case GFX_DRAW_FLAT_TRI_3D:
                case GFX_DRAW_FLAT_QUAD_3D:
                    outPointer = (GfxTlVertex *)setGroup->vertices;
                    auxPos = setGroup->auxPointer;
                    for (vertNum = 0; vertNum < cnt; vertNum++) {
                        outPointer->xBits = drawNode->positions[vertNum].x;
                        outPointer->yBits = drawNode->positions[vertNum].y;
                        outPointer->zBits = drawNode->positions[vertNum].z;
                        outPointer->rhwBits = 0;
                        outPointer->color = drawNode->colors[vertNum];
                        auxPos += 32;
                        outPointer++;
                    }
                    break;
                case GFX_DRAW_GOURAUD_TRI_2D:
                case GFX_DRAW_GOURAUD_QUAD_2D:
                    outPtr = (GfxTlVertex *)setGroup->vertices;
                    altPointer = setGroup->auxPointer;
                    for (vertNum = 0; vertNum < cnt; vertNum++) {
                        outPtr->xBits = drawNode->positions[vertNum].x;
                        outPtr->yBits = drawNode->positions[vertNum].y;
                        outPtr->zBits = drawNode->positions[vertNum].z;
                        outPtr->rhwBits = drawNode->rhw[vertNum];
                        outPtr->color = drawNode->colors[vertNum];
                        altPointer += 0x20;
                        outPtr++;
                    }
                    break;
                case GFX_DRAW_GOURAUD_TRI_3D:
                case GFX_DRAW_GOURAUD_QUAD_3D:
                    dstPointer = (GfxTlVertex *)setGroup->vertices;
                    endPointer = setGroup->auxPointer;
                    for (vertNum = 0; vertNum < cnt; vertNum++) {
                        dstPointer->xBits = drawNode->positions[vertNum].x;
                        dstPointer->yBits = drawNode->positions[vertNum].y;
                        dstPointer->zBits = drawNode->positions[vertNum].z;
                        dstPointer->rhwBits = 0;
                        dstPointer->color = drawNode->colors[vertNum];
                        endPointer += 0x20;
                        dstPointer++;
                    }
                    break;
                case GFX_DRAW_TEXTURED_TRI_2D:
                case GFX_DRAW_TEXTURED_GOURAUD_TRI_2D:
                case GFX_DRAW_TEXTURED_QUAD_2D:
                case GFX_DRAW_TEXTURED_GOURAUD_QUAD_2D:
                    writePtr = (GfxTlVertex *)setGroup->vertices;
                    altPtr = setGroup->auxPointer;
                    for (vertNum = 0; vertNum < cnt; vertNum++) {
                        writePtr->xBits = drawNode->positions[vertNum].x;
                        writePtr->yBits = drawNode->positions[vertNum].y;
                        writePtr->zBits = drawNode->positions[vertNum].z;
                        writePtr->rhwBits = drawNode->rhw[vertNum];
                        writePtr->color = drawNode->colors[vertNum];
                        writePtr->uBits = drawNode->texCoords[vertNum].u;
                        writePtr->vBits = drawNode->texCoords[vertNum].v;
                        if (ctx->configApplied != 0) {
                            ((GfxPolygonPrimitive *)writePtr)->tu +=
                                -(renderNode->invWidth * -0.5f);
                            ((GfxPolygonPrimitive *)writePtr)->tv +=
                                -(renderNode->invHeight * -0.5f);
                        }
                        altPtr += 0x20;
                        writePtr++;
                    }
                    break;
                case GFX_DRAW_TEXTURED_TRI_3D:
                case GFX_DRAW_TEXTURED_GOURAUD_TRI_3D:
                case GFX_DRAW_TEXTURED_QUAD_3D:
                case GFX_DRAW_TEXTURED_GOURAUD_QUAD_3D:
                    primitivePointer = (GfxTlVertex *)setGroup->vertices;
                    auxPtr = setGroup->auxPointer;
                    for (vertNum = 0; vertNum < cnt; vertNum++) {
                        primitivePointer->xBits = drawNode->positions[vertNum].x;
                        primitivePointer->yBits = drawNode->positions[vertNum].y;
                        primitivePointer->zBits = drawNode->positions[vertNum].z;
                        primitivePointer->rhwBits = 0;
                        primitivePointer->color = drawNode->colors[vertNum];
                        primitivePointer->uBits = drawNode->texCoords[vertNum].u;
                        primitivePointer->vBits = drawNode->texCoords[vertNum].v;
                        auxPtr += 0x20;
                        primitivePointer++;
                    }
                    break;
                }
                Gfx_UnlockPolygonSetGroup(0, polygonSet, (struct GfxEngineContext *)source->device);
            }
        }
    }
}

/*
 * Acquires primitive batch node and advances cursor for drawing.
 */
// FUNCTION: FF7 0x00671c71
int __cdecl Gfx_AcquireBatchPrimitive(int renderObject)
{
    GfxDrawPoolNode *result;
    GfxPrimitiveMeshBank *batchGroup;
    int groupId;
    GfxBatchPrimitive *allocated;
    GfxBatchPrimitive *batchBlock;

    result = 0;
    if (renderObject != 0 && ((GfxRenderObject *)renderObject)->init != 0) {
        batchGroup = (GfxPrimitiveMeshBank *)((GfxRenderObject *)renderObject)->init;
        if (((GfxRenderObject *)renderObject)->enabled != 0) {
            groupId = (int)batchGroup->device;
            allocated = (GfxBatchPrimitive *)(AllocBlockFromHeap(batchGroup->vertexHeap));
            batchBlock = allocated;
            if (batchBlock != 0) {
                batchBlock->renderKind = ((GfxRenderObject *)renderObject)->kind;
                batchBlock->clearedGroup = 0;
                batchBlock->renderObject = renderObject;
                result = (GfxDrawPoolNode *)(Gfx_AllocDrawPoolListNode((GfxDrawPoolList *)(((GfxRenderObject *)renderObject)->polygonSet + 0x14), (GfxDrawPoolDesc *)(((GfxPolygonSet *)(((GfxRenderObject *)renderObject)->polygonSet))->drawPool)));
                if (result != 0) {
                    result->nodeKind = 2;
                    result->batchBlock = (int)batchBlock;
                }
            }
        }
    }
    return (int)result;
}

/*
 * Queues one flat-coloured screen rectangle in a 2D flat-quad render object: the boxes, bars and
 * window fills of the battle menu, field, chocobo, condor, highway and submarine HUDs.
 * Gfx_AcquireBatchPrimitive takes a primitive record from the bank and links it into the
 * object's draw list; this writes its four vertices in the order top-left, bottom-left,
 * top-right, bottom-right, covering posX..posX+width, posY..posY+height in pixels, all with
 * `color`, rhw 1.0 and z = `depth`, which holds a float's bits (callers pass values such as
 * 0x3B83126F, about 0.004). `reserved` is not used.
 *
 * Returns the record, which the caller may still adjust, or 0 when the object is disabled
 * (created with no matrices) or the bank's heaps are full.
 */
// FUNCTION: FF7 0x00671d2a
int __cdecl Gfx_EmitFlatRect(int posX, int posY, int width, int height, int color, int reserved, int depth, int renderObj)
{
    GfxBatchPrimitive *vertices;
    GfxDrawPoolNode *poolNode;
    float fH;
    float floatW;
    int rhw;
    float x0;
    int vertexIndex;
    float y0;

    vertices = 0;
    poolNode = (GfxDrawPoolNode *)Gfx_AcquireBatchPrimitive(renderObj);
    if (poolNode != 0) {
        rhw = FLOAT_ONE_BITS;
        vertexIndex = 0;
        vertices = (GfxBatchPrimitive *)poolNode->batchBlock;
        x0 = (float)posX;
        y0 = (float)posY;
        floatW = (float)width;
        fH = (float)height;
        vertices->positions[vertexIndex].x = x0;
        vertices->positions[vertexIndex].y = y0;
        vertices->positions[vertexIndex].depthBits = depth;
        vertices->colors[vertexIndex] = color;
        vertices->rhw[vertexIndex] = rhw;
        vertexIndex++;
        vertices->positions[vertexIndex].x = x0;
        vertices->positions[vertexIndex].y = y0 + fH;
        vertices->positions[vertexIndex].depthBits = depth;
        vertices->colors[vertexIndex] = color;
        vertices->rhw[vertexIndex] = rhw;
        vertexIndex++;
        vertices->positions[vertexIndex].x = x0 + floatW;
        vertices->positions[vertexIndex].y = y0;
        vertices->positions[vertexIndex].depthBits = depth;
        vertices->colors[vertexIndex] = color;
        vertices->rhw[vertexIndex] = rhw;
        vertexIndex++;
        vertices->positions[vertexIndex].x = x0 + floatW;
        vertices->positions[vertexIndex].y = y0 + fH;
        vertices->positions[vertexIndex].depthBits = depth;
        vertices->colors[vertexIndex] = color;
        vertices->rhw[vertexIndex] = rhw;
    }
    return (int)vertices;
}

/*
 * Queues one Gouraud-shaded screen rectangle (the submarine HUD uses it): like Gfx_EmitFlatRect,
 * four vertices top-left, bottom-left, top-right, bottom-right over posX..posX+width,
 * posY..posY+height, rhw 1.0 and z = `depth` (a float's bits), but each corner gets its own
 * colour, so Direct3D blends them across the rectangle. `reserved` is not used. Returns the
 * primitive record, or 0 when the render object is disabled or the bank's heaps are full.
 */
// FUNCTION: FF7 0x00671ed5
int __cdecl Gfx_EmitGouraudRect(int posX, int posY, int width, int height, int colorTopLeft, int colorBottomLeft, int colorTopRight, int colorBottomRight, int reserved, int depth, int renderObj)
{
    GfxBatchPrimitive *vertices;
    GfxDrawPoolNode *poolNode;
    float fH;
    float floatW;
    int rhw;
    float x0;
    int vertexIndex;
    float y0;

    vertices = 0;
    poolNode = (GfxDrawPoolNode *)Gfx_AcquireBatchPrimitive(renderObj);
    if (poolNode != 0) {
        rhw = FLOAT_ONE_BITS;
        vertexIndex = 0;
        vertices = (GfxBatchPrimitive *)poolNode->batchBlock;
        x0 = posX;
        y0 = posY;
        floatW = width;
        fH = height;
        vertices->positions[vertexIndex].x = x0;
        vertices->positions[vertexIndex].y = y0;
        vertices->positions[vertexIndex].depthBits = depth;
        vertices->colors[vertexIndex] = colorTopLeft;
        vertices->rhw[vertexIndex] = rhw;
        vertexIndex++;
        vertices->positions[vertexIndex].x = x0;
        vertices->positions[vertexIndex].y = y0 + fH;
        vertices->positions[vertexIndex].depthBits = depth;
        vertices->colors[vertexIndex] = colorBottomLeft;
        vertices->rhw[vertexIndex] = rhw;
        vertexIndex++;
        vertices->positions[vertexIndex].x = x0 + floatW;
        vertices->positions[vertexIndex].y = y0;
        vertices->positions[vertexIndex].depthBits = depth;
        vertices->colors[vertexIndex] = colorTopRight;
        vertices->rhw[vertexIndex] = rhw;
        vertexIndex++;
        vertices->positions[vertexIndex].x = x0 + floatW;
        vertices->positions[vertexIndex].y = y0 + fH;
        vertices->positions[vertexIndex].depthBits = depth;
        vertices->colors[vertexIndex] = colorBottomRight;
        vertices->rhw[vertexIndex] = rhw;
    }
    return (int)vertices;
}

/*
 * Queues one textured screen rectangle, untinted: the chocobo and condor HUD sprites. The quad
 * covers posX..posX+width, posY..posY+height in pixels and samples the texel rectangle texX,
 * texY, texWidth x texHeight, turned into texture coordinates with the render object's
 * invWidth/invHeight. Flag 0x10000 mirrors it horizontally (u swapped), 0x20000 vertically, and
 * the low byte of `flags` is the record's texture page. Every vertex gets diffuse 0xFFFFFFFF,
 * z = `depth` (a float's bits) and `specular` in the slot that becomes the vertex's rhw (callers
 * pass 1.0). Returns the primitive record, or 0 when the render object is disabled or the bank's
 * heaps are full.
 */
// FUNCTION: FF7 0x00672080
int __cdecl Gfx_EmitTexturedRect(int posX, int posY, int width, int height, int texX, int texY, int texWidth, int texHeight, int flags, int depth, int specular, int renderObj)
{
    GfxBatchPrimitive *vertexBuffer;
    float vBottom;
    float fTexX;
    float y0;
    float uRight;
    float u0;
    float y1;
    int index;
    float startX;
    float rightX8;
    float v0;
    GfxDrawPoolNode *batchNode;
    float scaledH;
    float scaledWidth;
    float fTexY;

    vertexBuffer = 0;
    batchNode = (GfxDrawPoolNode *)Gfx_AcquireBatchPrimitive(renderObj);
    if (batchNode != 0) {
        fTexX = texX * ((GfxRenderObject *)renderObj)->invWidth;
        fTexY = texY * ((GfxRenderObject *)renderObj)->invHeight;
        scaledWidth = texWidth * ((GfxRenderObject *)renderObj)->invWidth;
        scaledH = texHeight * ((GfxRenderObject *)renderObj)->invHeight;
        index = 0;
        vertexBuffer = (GfxBatchPrimitive *)batchNode->batchBlock;
        if ((flags & 0x10000) != 0) {
            uRight = fTexX;
            u0 = fTexX + scaledWidth;
        } else {
            u0 = fTexX;
            uRight = fTexX + scaledWidth;
        }
        if ((flags & 0x20000) != 0) {
            vBottom = fTexY;
            v0 = fTexY + scaledH;
        } else {
            v0 = fTexY;
            vBottom = fTexY + scaledH;
        }
        startX = posX;
        y0 = posY;
        rightX8 = width + startX;
        y1 = height + y0;
        vertexBuffer->positions[index].x = startX;
        vertexBuffer->positions[index].y = y0;
        vertexBuffer->positions[index].depthBits = depth;
        vertexBuffer->colors[index] = 0xFFFFFFFF;
        vertexBuffer->texCoords[index].u = u0;
        vertexBuffer->texCoords[index].v = v0;
        vertexBuffer->rhw[index] = specular;
        index++;
        vertexBuffer->positions[index].x = startX;
        vertexBuffer->positions[index].y = y1;
        vertexBuffer->positions[index].depthBits = depth;
        vertexBuffer->colors[index] = 0xFFFFFFFF;
        vertexBuffer->texCoords[index].u = u0;
        vertexBuffer->texCoords[index].v = vBottom;
        vertexBuffer->rhw[index] = specular;
        index++;
        vertexBuffer->positions[index].x = rightX8;
        vertexBuffer->positions[index].y = y0;
        vertexBuffer->positions[index].depthBits = depth;
        vertexBuffer->colors[index] = 0xFFFFFFFF;
        vertexBuffer->texCoords[index].u = uRight;
        vertexBuffer->texCoords[index].v = v0;
        vertexBuffer->rhw[index] = specular;
        index++;
        vertexBuffer->positions[index].x = rightX8;
        vertexBuffer->positions[index].y = y1;
        vertexBuffer->positions[index].depthBits = depth;
        vertexBuffer->colors[index] = 0xFFFFFFFF;
        vertexBuffer->texCoords[index].u = uRight;
        vertexBuffer->texCoords[index].v = vBottom;
        vertexBuffer->rhw[index] = specular;
        vertexBuffer->clutRow = flags & 0xFF;
    }
    return (int)vertexBuffer;
}

/*
 * Queues one textured screen rectangle tinted with one colour: the condor minigame's effects and
 * HUD use it. It is Gfx_EmitTexturedRect with `diffuse` instead of white on all four vertices:
 * the quad covers posX..posX+width, posY..posY+height, samples texel rectangle texX, texY,
 * texWidth x texHeight (scaled by the render object's invWidth/invHeight), flips u on flag
 * 0x10000 and v on 0x20000, takes its texture page from the low byte of `flags`, z = `depth` (a
 * float's bits) and `specular` as rhw. The flips are not what sets it apart: the untinted variant
 * has them too. Returns the primitive record, or 0.
 */
// FUNCTION: FF7 0x00672325
int __cdecl Gfx_EmitTexturedRectFlipped(int posX, int posY, int width, int height, int texX, int texY, int texWidth, int texHeight, int diffuse, int flags, int depth, int specular, int renderObj)
{
    GfxBatchPrimitive *vertexBuffer;
    float vBottom;
    float fTexX;
    float y0;
    float uRight;
    float u0;
    float y1;
    int index;
    float startX;
    float rightX8;
    float v0;
    GfxDrawPoolNode *batchNode;
    float scaledH;
    float scaledWidth;
    float fTexY;

    vertexBuffer = 0;
    batchNode = (GfxDrawPoolNode *)Gfx_AcquireBatchPrimitive(renderObj);
    if (batchNode != 0) {
        fTexX = texX * ((GfxRenderObject *)renderObj)->invWidth;
        fTexY = texY * ((GfxRenderObject *)renderObj)->invHeight;
        scaledWidth = texWidth * ((GfxRenderObject *)renderObj)->invWidth;
        scaledH = texHeight * ((GfxRenderObject *)renderObj)->invHeight;
        index = 0;
        vertexBuffer = (GfxBatchPrimitive *)batchNode->batchBlock;
        if ((flags & 0x10000) != 0) {
            uRight = fTexX;
            u0 = fTexX + scaledWidth;
        } else {
            u0 = fTexX;
            uRight = fTexX + scaledWidth;
        }
        if ((flags & 0x20000) != 0) {
            vBottom = fTexY;
            v0 = fTexY + scaledH;
        } else {
            v0 = fTexY;
            vBottom = fTexY + scaledH;
        }
        startX = posX;
        y0 = posY;
        rightX8 = width + startX;
        y1 = height + y0;
        vertexBuffer->positions[index].x = startX;
        vertexBuffer->positions[index].y = y0;
        vertexBuffer->positions[index].depthBits = depth;
        vertexBuffer->colors[index] = diffuse;
        vertexBuffer->texCoords[index].u = u0;
        vertexBuffer->texCoords[index].v = v0;
        vertexBuffer->rhw[index] = specular;
        index++;
        vertexBuffer->positions[index].x = startX;
        vertexBuffer->positions[index].y = y1;
        vertexBuffer->positions[index].depthBits = depth;
        vertexBuffer->colors[index] = diffuse;
        vertexBuffer->texCoords[index].u = u0;
        vertexBuffer->texCoords[index].v = vBottom;
        vertexBuffer->rhw[index] = specular;
        index++;
        vertexBuffer->positions[index].x = rightX8;
        vertexBuffer->positions[index].y = y0;
        vertexBuffer->positions[index].depthBits = depth;
        vertexBuffer->colors[index] = diffuse;
        vertexBuffer->texCoords[index].u = uRight;
        vertexBuffer->texCoords[index].v = v0;
        vertexBuffer->rhw[index] = specular;
        index++;
        vertexBuffer->positions[index].x = rightX8;
        vertexBuffer->positions[index].y = y1;
        vertexBuffer->positions[index].depthBits = depth;
        vertexBuffer->colors[index] = diffuse;
        vertexBuffer->texCoords[index].u = uRight;
        vertexBuffer->texCoords[index].v = vBottom;
        vertexBuffer->rhw[index] = specular;
        vertexBuffer->clutRow = flags & 0xFF;
    }
    return (int)vertexBuffer;
}

/*
 * Queues one textured screen rectangle with a colour per corner (Gouraud-tinted). It is
 * Gfx_EmitTexturedRect with the four colours top-left, bottom-left, top-right, bottom-right: the
 * quad is axis-aligned over posX..posX+width, posY..posY+height, samples texel rectangle texX,
 * texY, texWidth x texHeight (scaled by invWidth/invHeight), flips u on flag 0x10000 and v on
 * 0x20000, takes its page from the low byte of `flags`, z = `depth` (a float's bits) and
 * `specular` as rhw. Returns the primitive record, or 0. Nothing in src/ calls it.
 */
// FUNCTION: FF7 0x006725c7
int __cdecl Gfx_EmitTexturedQuadFlipped(int posX, int posY, int width, int height, int texX, int texY, int texWidth, int texHeight, int colorTopLeft, int colorBottomLeft, int colorTopRight, int colorBottomRight, int flags, int depth, int specular, GfxRenderObject *renderObj)
{
    GfxBatchPrimitive *vertexBuffer;
    float vBottom;
    float fTexX;
    float y0;
    float uRight;
    float u0;
    float y1;
    int index;
    float startX;
    float rightX8;
    float v0;
    GfxDrawPoolNode *batchNode;
    float scaledH;
    float scaledWidth;
    float fTexY;

    vertexBuffer = 0;
    batchNode = (GfxDrawPoolNode *)Gfx_AcquireBatchPrimitive((int)renderObj);
    if (batchNode != 0) {
        fTexX = texX * renderObj->invWidth;
        fTexY = texY * renderObj->invHeight;
        scaledWidth = texWidth * renderObj->invWidth;
        scaledH = texHeight * renderObj->invHeight;
        index = 0;
        vertexBuffer = (GfxBatchPrimitive *)batchNode->batchBlock;
        if ((flags & 0x10000) != 0) {
            uRight = fTexX;
            u0 = fTexX + scaledWidth;
        } else {
            u0 = fTexX;
            uRight = fTexX + scaledWidth;
        }
        if ((flags & 0x20000) != 0) {
            vBottom = fTexY;
            v0 = fTexY + scaledH;
        } else {
            v0 = fTexY;
            vBottom = fTexY + scaledH;
        }
        startX = posX;
        y0 = posY;
        rightX8 = width + startX;
        y1 = height + y0;
        vertexBuffer->positions[index].x = startX;
        vertexBuffer->positions[index].y = y0;
        vertexBuffer->positions[index].depthBits = depth;
        vertexBuffer->colors[index] = colorTopLeft;
        vertexBuffer->texCoords[index].u = u0;
        vertexBuffer->texCoords[index].v = v0;
        vertexBuffer->rhw[index] = specular;
        index++;
        vertexBuffer->positions[index].x = startX;
        vertexBuffer->positions[index].y = y1;
        vertexBuffer->positions[index].depthBits = depth;
        vertexBuffer->colors[index] = colorBottomLeft;
        vertexBuffer->texCoords[index].u = u0;
        vertexBuffer->texCoords[index].v = vBottom;
        vertexBuffer->rhw[index] = specular;
        index++;
        vertexBuffer->positions[index].x = rightX8;
        vertexBuffer->positions[index].y = y0;
        vertexBuffer->positions[index].depthBits = depth;
        vertexBuffer->colors[index] = colorTopRight;
        vertexBuffer->texCoords[index].u = uRight;
        vertexBuffer->texCoords[index].v = v0;
        vertexBuffer->rhw[index] = specular;
        index++;
        vertexBuffer->positions[index].x = rightX8;
        vertexBuffer->positions[index].y = y1;
        vertexBuffer->positions[index].depthBits = depth;
        vertexBuffer->colors[index] = colorBottomRight;
        vertexBuffer->texCoords[index].u = uRight;
        vertexBuffer->texCoords[index].v = vBottom;
        vertexBuffer->rhw[index] = specular;
        vertexBuffer->clutRow = flags & 0xFF;
    }
    return (int)vertexBuffer;
}

/*
 * Queues one flat-coloured screen triangle with corners (x0, y0), (x1, y1), (x2, y2) in pixels,
 * all with `color`, rhw 1.0 and z = `depth` (a float's bits), into a 2D flat-triangle render
 * object. `reserved` is not used. Returns the primitive record, or 0 when the render object is
 * disabled or the bank's heaps are full. Nothing in src/ calls it.
 */
// FUNCTION: FF7 0x00672869
int __cdecl Gfx_EmitFlatTriangle(int x0, int y0, int x1, int y1, int x2, int y2, int color, int reserved, int depth, int renderObj)
{
    GfxBatchPrimitive *vertices;
    GfxDrawPoolNode *poolNode;
    int rhw;
    int vertexIndex;

    vertices = 0;
    poolNode = (GfxDrawPoolNode *)Gfx_AcquireBatchPrimitive(renderObj);
    if (poolNode != 0) {
        rhw = FLOAT_ONE_BITS;
        vertexIndex = 0;
        vertices = (GfxBatchPrimitive *)poolNode->batchBlock;
        vertices->positions[vertexIndex].x = (float)x0;
        vertices->positions[vertexIndex].y = (float)y0;
        vertices->positions[vertexIndex].depthBits = depth;
        vertices->colors[vertexIndex] = color;
        vertices->rhw[vertexIndex] = rhw;
        vertexIndex++;
        vertices->positions[vertexIndex].x = (float)x1;
        vertices->positions[vertexIndex].y = (float)y1;
        vertices->positions[vertexIndex].depthBits = depth;
        vertices->colors[vertexIndex] = color;
        vertices->rhw[vertexIndex] = rhw;
        vertexIndex++;
        vertices->positions[vertexIndex].x = (float)x2;
        vertices->positions[vertexIndex].y = (float)y2;
        vertices->positions[vertexIndex].depthBits = depth;
        vertices->colors[vertexIndex] = color;
        vertices->rhw[vertexIndex] = rhw;
    }
    return (int)vertices;
}

/*
 * Queues one Gouraud-shaded screen triangle: corners (x0, y0), (x1, y1), (x2, y2) in pixels with
 * colours color0..color2, rhw 1.0 and z = `depth` (a float's bits). `reserved` is not used.
 * Returns the primitive record, or 0 when the render object is disabled or the bank's heaps are
 * full. Nothing in src/ calls it.
 */
// FUNCTION: FF7 0x0067299d
int __cdecl Gfx_EmitGouraudTriangle(int x0, int y0, int x1, int y1, int x2, int y2, int color0, int color1, int color2, int reserved, int depth, int renderObj)
{
    GfxBatchPrimitive *vertices;
    GfxDrawPoolNode *poolNode;
    int rhw;
    int vertexIndex;

    vertices = 0;
    poolNode = (GfxDrawPoolNode *)Gfx_AcquireBatchPrimitive(renderObj);
    if (poolNode != 0) {
        rhw = FLOAT_ONE_BITS;
        vertexIndex = 0;
        vertices = (GfxBatchPrimitive *)poolNode->batchBlock;
        vertices->positions[vertexIndex].x = (float)x0;
        vertices->positions[vertexIndex].y = (float)y0;
        vertices->positions[vertexIndex].depthBits = depth;
        vertices->colors[vertexIndex] = color0;
        vertices->rhw[vertexIndex] = rhw;
        vertexIndex++;
        vertices->positions[vertexIndex].x = (float)x1;
        vertices->positions[vertexIndex].y = (float)y1;
        vertices->positions[vertexIndex].depthBits = depth;
        vertices->colors[vertexIndex] = color1;
        vertices->rhw[vertexIndex] = rhw;
        vertexIndex++;
        vertices->positions[vertexIndex].x = (float)x2;
        vertices->positions[vertexIndex].y = (float)y2;
        vertices->positions[vertexIndex].depthBits = depth;
        vertices->colors[vertexIndex] = color2;
        vertices->rhw[vertexIndex] = rhw;
    }
    return (int)vertices;
}

/*
 * Queues one textured screen triangle: corners (x0, y0), (x1, y1), (x2, y2) in pixels with the
 * texture coordinates texU0/texV0 .. texU2/texV2, which are stored as given (the ints hold float
 * bits; unlike the rectangle writers they are not scaled by the texture size), diffuse
 * 0xFFFFFFFF, z = `depth` and `specular` as rhw. The low byte of `flags` is the texture page; no
 * flip bits are read. Returns the primitive record, or 0. Nothing in src/ calls it.
 */
// FUNCTION: FF7 0x00672ad1
int __cdecl Gfx_EmitTexturedTriangle(int x0, int y0, int x1, int y1, int x2, int y2, int texU0, int texV0, int texU1, int texV1, int texU2, int texV2, int flags, int depth, int specular, int renderObj)
{
    GfxBatchPrimitive *vertices;
    GfxDrawPoolNode *poolNode;
    int vertexIndex;

    vertices = 0;
    poolNode = (GfxDrawPoolNode *)Gfx_AcquireBatchPrimitive(renderObj);
    if (poolNode != 0) {
        vertexIndex = 0;
        vertices = (GfxBatchPrimitive *)poolNode->batchBlock;
        vertices->positions[vertexIndex].x = (float)x0;
        vertices->positions[vertexIndex].y = (float)y0;
        vertices->positions[vertexIndex].depthBits = depth;
        vertices->colors[vertexIndex] = 0xFFFFFFFF;
        vertices->texCoords[vertexIndex].uBits = texU0;
        vertices->texCoords[vertexIndex].vBits = texV0;
        vertices->rhw[vertexIndex] = specular;
        vertexIndex++;
        vertices->positions[vertexIndex].x = (float)x1;
        vertices->positions[vertexIndex].y = (float)y1;
        vertices->positions[vertexIndex].depthBits = depth;
        vertices->colors[vertexIndex] = 0xFFFFFFFF;
        vertices->texCoords[vertexIndex].uBits = texU1;
        vertices->texCoords[vertexIndex].vBits = texV1;
        vertices->rhw[vertexIndex] = specular;
        vertexIndex++;
        vertices->positions[vertexIndex].x = (float)x2;
        vertices->positions[vertexIndex].y = (float)y2;
        vertices->positions[vertexIndex].depthBits = depth;
        vertices->colors[vertexIndex] = 0xFFFFFFFF;
        vertices->texCoords[vertexIndex].uBits = texU2;
        vertices->texCoords[vertexIndex].vBits = texV2;
        vertices->rhw[vertexIndex] = specular;
        vertices->clutRow = flags & 0xFF;
    }
    return (int)vertices;
}

/*
 * Updates primitive batch cursors and command pointers.
 */
// FUNCTION: FF7 0x00672c61
void __cdecl Gfx_AcquireBatchPrimitiveCursors(int cursors, int renderObj)
{
    GfxDrawPoolNode *poolNode;

    poolNode = (GfxDrawPoolNode *)Gfx_AcquireBatchPrimitive(renderObj);
    if (poolNode != 0) {
        ((GfxBatchPrimitiveCursors *)cursors)->matrix = (int)poolNode + 0xC;
        ((GfxBatchPrimitiveCursors *)cursors)->primitive = poolNode->batchBlock;
    }
}

/*
 * Retrieves texture dimensions and format from render object.
 */
// FUNCTION: FF7 0x00672c95
int __cdecl Gfx_GetRenderObjectTextureInfo(GfxRenderTextureInfo *info, GfxRenderObject *renderObj)
{
    GfxTexturePageInfo *texPage;
    int success;

    success = 0;
    if (renderObj != 0 && renderObj->hundredIndex != 0) {
        info->kind = renderObj->kind;
        if (((GfxTexturedHundredRecord *)renderObj->hundredIndex)->uploaded != 0) {
            texPage = (GfxTexturePageInfo *)(*(int *)((char *)((GfxTexturedHundredRecord *)renderObj->hundredIndex)->uploaded + 0x84));
            if (texPage != 0) {
                info->paletteCount = texPage->paletteCount;
                info->pageU = texPage->pageU;
                info->pageV = texPage->pageV;
                success = 1;
            }
        } else {
            info->pageU = 0;
            info->pageV = 0;
            success = 1;
        }
    }
    return success;
}

/*
 * Initializes transformed and lit vertex array with default values.
 */
// FUNCTION: FF7 0x00672d2e
void __cdecl Gfx_InitTLVertices(int vertexCount, GfxSpriteBatchHeader *vertexCursor, GfxSpriteBatch *spriteBatch)
{
    GfxSpriteBatch *batch;
    int index;

    batch = spriteBatch;
    if (batch != 0) {
        for (index = 0; index < vertexCount; index++) {
            vertexCursor->posX = 0;
            vertexCursor->posY = 0;
            vertexCursor->mode = batch->mode;
            vertexCursor->rhw = FLOAT_ONE_BITS;
            vertexCursor->diffuse = 0xFFFFFFFF;
            vertexCursor->specular = 0xFF000000;
            vertexCursor->texU = 0;
            vertexCursor->texV = 0;
            vertexCursor++;
        }
    }
}

/*
 * Fills transformed vertex coordinates and UVs for sprite quad.
 */
// FUNCTION: FF7 0x00672dba
void __cdecl Gfx_FillSpriteBatchVertices(GfxSpriteBatch *spriteBatch, int desc)
{
    GfxSpriteBatchHeader *header;
    GfxSpriteBatch *batch;

    batch = spriteBatch;
    if (batch != 0 && desc != 0) {
        header = (GfxSpriteBatchHeader *)(*(int *)((char *)desc + 0x10));
        Gfx_InitTLVertices(batch->blockCount << 2, header, spriteBatch);
    }
}

/*
 * Releases sprite batch vertex and index allocations.
 */
// FUNCTION: FF7 0x00672df9
void __cdecl Gfx_DestroySpriteBatch(GfxSpriteBatch *spriteBatch)
{
    if (spriteBatch != 0) {
        DestroyBlockHeapChain(spriteBatch->vertexHeap);
        // STRING: FF7 0x0090ce98
        GameHeapFree(spriteBatch, "C:\\lib\\src\\graphics\\dx_sfx.cpp", 2346);
    }
}

/*
 * Allocates and initializes sprite batch vertex structures.
 */
// FUNCTION: FF7 0x00672e29
int __cdecl Gfx_CreateSpriteBatch(int width, int height, int srcX, int srcY, int blockCount, int mode, int renderSource, PfileLoadOptions *loadOptions, int name, int data)
{
    GfxSpriteBatch *heapBatch;
    GfxRenderObject *renderObject;
    GfxSpriteBatch *pBatch;
    GfxSpriteBatchHeader *hdrPtr;
    GfxSpriteBatch *batchHandlePtr;
    GfxRenderTextureInfo textureInfo;

    renderObject = (GfxRenderObject *)((int)Gfx_CreateRenderObject(renderSource, GFX_DRAW_SPRITE_BATCH, loadOptions, (char *)name, (void *)data));
    if (renderObject != 0) {
        // STRING: FF7 0x0090ceb8
        pBatch = (GfxSpriteBatch *)GameHeapAlloc(1, 60, "C:\\lib\\src\\graphics\\dx_sfx.cpp", 2361);
        if (pBatch != 0) {
            pBatch->width = width;
            pBatch->height = height;
            pBatch->widthFloat = (float)width;
            pBatch->heightFloat = (float)height;
            if (Gfx_GetRenderObjectTextureInfo(&textureInfo, renderObject) != 0) {
                pBatch->texWidth = textureInfo.width;
                pBatch->texHeight = textureInfo.height;
                pBatch->invTexWidth = 1.0f / (float)textureInfo.width;
                pBatch->invTexHeight = 1.0f / (float)textureInfo.height;
                pBatch->srcX = srcX;
                pBatch->srcY = srcY;
                pBatch->uOrigin = (float)srcX * pBatch->invTexWidth;
                pBatch->vOrigin = (float)srcY * pBatch->invTexHeight;
                pBatch->mode = mode;
                pBatch->blockCount = blockCount;
                heapBatch = pBatch;
                pBatch->vertexHeap = (MemBlockHeap *)CreateBlockHeapEx(128, blockCount << 7, 0, heapBatch, (int)Gfx_FillSpriteBatchVertices);
                if ((loadOptions->flags & 0x200000) != 0) {
                    batchHandlePtr = pBatch;
                    renderObject->immediateDraw = 1;
                    // STRING: FF7 0x0090ced8
                    renderObject->vertexBuffer = (int)GameHeapAlloc(4, 32, "C:\\lib\\src\\graphics\\dx_sfx.cpp", 2397);
                    hdrPtr = (GfxSpriteBatchHeader *)(renderObject->vertexBuffer);
                    if (hdrPtr != 0) {
                        Gfx_InitTLVertices(4, hdrPtr, batchHandlePtr);
                    }
                }
                renderObject->spriteBatch = (int)pBatch;
            }
        }
    }
    return (int)renderObject;
}

/*
 * Puts one cell of a sprite batch (Gfx_CreateSpriteBatch) on screen at (posX, posY). The quad is
 * the batch's width x height pixels (widthFloat/heightFloat) and samples the texture from texel
 * (sizeX, sizeY), scaled by invTexWidth/invTexHeight, over the batch's cell size in texture
 * coordinates (uOrigin/vOrigin); vertices go top-left, bottom-left, top-right, bottom-right, and
 * only x, y, u and v are written.
 *
 * When the render object's word +0x08 is set (load option flag 0x200000), it writes the four
 * vertices into the object's own vertex buffer and draws them at once
 * (DxRendx_SetStateAndDrawIndexedRetry). Otherwise it queues a primitive record and takes a
 * 0x80-byte block of four pre-initialised vertices (rhw 1.0, white) from the batch's vertex heap,
 * stores it in the record with texture page 0 and writes the vertices there. The only caller,
 * Gfx_SubmitFixedQuadGridBatches, fills the screen with a 20 x 14 grid of 32-pixel cells.
 */
// FUNCTION: FF7 0x00672fbc
void __cdecl Gfx_EmitScreenSprite(int posX, int posY, int sizeX, int sizeY, GfxRenderObject *renderObj)
{
    float yPos;
    float offsetLeftX;
    float subBottom;
    GfxScreenSpriteStatus *primStatus;
    GfxSpriteBatch *params;
    float scaledX;
    GfxScreenVertex *vertexPointer;
    float scaledY;
    float extY;
    float xExtent;
    GfxScreenSpriteCmd *spriteCmd;
    float curRightPart;
    GfxDrawPoolNode *prim;
    float xFloat;
    float yOffset;
    GfxScreenSpriteCmd *cmdPtr;

    if (renderObj != 0) {
        if (renderObj->immediateDraw != 0) {
            params = (GfxSpriteBatch *)renderObj->spriteBatch;
            vertexPointer = (GfxScreenVertex *)renderObj->vertexBuffer;
            ((GfxPolygonSet *)(renderObj->polygonSet))->vertexPointer = (int)vertexPointer;
            if (vertexPointer != 0) {
                scaledX = sizeX * params->invTexWidth;
                scaledY = sizeY * params->invTexHeight;
                xFloat = posX;
                yPos = posY;
                vertexPointer->posX = xFloat;
                vertexPointer->posY = yPos;
                vertexPointer->u = scaledX;
                vertexPointer->v = scaledY;
                vertexPointer++;
                offsetLeftX = xFloat + params->widthFloat;
                yOffset = yPos + params->heightFloat;
                curRightPart = scaledX + params->uOrigin;
                subBottom = scaledY + params->vOrigin;
                vertexPointer->posX = xFloat;
                vertexPointer->posY = yOffset;
                vertexPointer->u = scaledX;
                vertexPointer->v = subBottom;
                vertexPointer++;
                vertexPointer->posX = offsetLeftX;
                vertexPointer->posY = yPos;
                vertexPointer->u = curRightPart;
                vertexPointer->v = scaledY;
                vertexPointer++;
                vertexPointer->posX = offsetLeftX;
                vertexPointer->posY = yOffset;
                vertexPointer->u = curRightPart;
                vertexPointer->v = subBottom;
            }
            DxRendx_SetStateAndDrawIndexedRetry((struct GfxIndexedDrawRequest *)renderObj->polygonSet, GetEngineContext());
        } else {
            prim = (GfxDrawPoolNode *)Gfx_AcquireBatchPrimitive((int)renderObj);
            params = (GfxSpriteBatch *)renderObj->spriteBatch;
            if (prim != 0 && params != 0) {
                spriteCmd = (GfxScreenSpriteCmd *)(AllocBlockFromHeap(params->vertexHeap));
                cmdPtr = (GfxScreenSpriteCmd *)spriteCmd;
                if (cmdPtr != 0) {
                    xExtent = sizeX * params->invTexWidth;
                    extY = sizeY * params->invTexHeight;
                    primStatus = (GfxScreenSpriteStatus *)(prim->batchBlock);
                    primStatus->cmdPtr = (int)cmdPtr;
                    primStatus->wordValue = 0;
                    xFloat = posX;
                    yPos = posY;
                    cmdPtr->posX = xFloat;
                    cmdPtr->posY = yPos;
                    cmdPtr->xExtent = xExtent;
                    cmdPtr->extY = extY;
                    cmdPtr++;
                    offsetLeftX = xFloat + params->widthFloat;
                    yOffset = yPos + params->heightFloat;
                    curRightPart = xExtent + params->uOrigin;
                    subBottom = extY + params->vOrigin;
                    cmdPtr->posX = xFloat;
                    cmdPtr->posY = yOffset;
                    cmdPtr->xExtent = xExtent;
                    cmdPtr->extY = subBottom;
                    cmdPtr++;
                    cmdPtr->posX = offsetLeftX;
                    cmdPtr->posY = yPos;
                    cmdPtr->xExtent = curRightPart;
                    cmdPtr->extY = extY;
                    cmdPtr++;
                    cmdPtr->posX = offsetLeftX;
                    cmdPtr->posY = yOffset;
                    cmdPtr->xExtent = curRightPart;
                    cmdPtr->extY = subBottom;
                }
            }
        }
    }
}

/*
 * Gfx_EmitScreenSprite with a texture page: the same quad of the batch's width x height pixels
 * at (posX, posY), sampling from texel (sizeX, sizeY) over the batch's cell size. On the queued
 * path `wordValue` goes into the primitive record's page word (+0x80, which
 * Gfx_EmitPolygonSetPrimitive selects as the texture page) instead of 0; on the immediate path
 * (render object word +0x08 set) it is ignored and the quad is drawn at once. Nothing in src/
 * calls it.
 */
// FUNCTION: FF7 0x00673282
void __cdecl Gfx_EmitScreenSpriteWithWord(int posX, int posY, int sizeX, int sizeY, int wordValue, GfxRenderObject *renderObj)
{
    float mainRight;
    int result;
    GfxScreenSpriteWordCmd *cmdCursor;
    float offsetRemainingX;
    GfxScreenSpriteWordStatus *primStatus;
    float scaledX;
    GfxScreenVertex *vertexPointer;
    float scaledY;
    float extY;
    float xExtent;
    GfxScreenSpriteWordCmd *spriteCmd;
    float leftX;
    float yPos8;
    float bottomPos;
    float yOffs;
    GfxSpriteBatch *params;

    if (renderObj != 0) {
        if (renderObj->immediateDraw != 0) {
            params = (GfxSpriteBatch *)renderObj->spriteBatch;
            vertexPointer = (GfxScreenVertex *)renderObj->vertexBuffer;
            ((GfxPolygonSet *)(renderObj->polygonSet))->vertexPointer = (int)vertexPointer;
            if (vertexPointer != 0) {
                scaledX = sizeX * params->invTexWidth;
                scaledY = sizeY * params->invTexHeight;
                leftX = posX;
                yPos8 = posY;
                vertexPointer->posX = leftX;
                vertexPointer->posY = yPos8;
                vertexPointer->u = scaledX;
                vertexPointer->v = scaledY;
                vertexPointer++;
                offsetRemainingX = leftX + params->widthFloat;
                yOffs = yPos8 + params->heightFloat;
                mainRight = scaledX + params->uOrigin;
                bottomPos = scaledY + params->vOrigin;
                vertexPointer->posX = leftX;
                vertexPointer->posY = yOffs;
                vertexPointer->u = scaledX;
                vertexPointer->v = bottomPos;
                vertexPointer++;
                vertexPointer->posX = offsetRemainingX;
                vertexPointer->posY = yPos8;
                vertexPointer->u = mainRight;
                vertexPointer->v = scaledY;
                vertexPointer++;
                vertexPointer->posX = offsetRemainingX;
                vertexPointer->posY = yOffs;
                vertexPointer->u = mainRight;
                vertexPointer->v = bottomPos;
            }
            DxRendx_SetStateAndDrawIndexedRetry((struct GfxIndexedDrawRequest *)renderObj->polygonSet, GetEngineContext());
        } else {
            result = Gfx_AcquireBatchPrimitive((int)renderObj);
            params = (GfxSpriteBatch *)renderObj->spriteBatch;
            if (result != 0 && params != 0) {
                spriteCmd = (GfxScreenSpriteWordCmd *)(AllocBlockFromHeap(params->vertexHeap));
                cmdCursor = (GfxScreenSpriteWordCmd *)spriteCmd;
                if (cmdCursor != 0) {
                    xExtent = sizeX * params->invTexWidth;
                    extY = sizeY * params->invTexHeight;
                    primStatus = (GfxScreenSpriteWordStatus *)(*(int *)((char *)result + 8));
                    primStatus->cmdCursor = (int)cmdCursor;
                    primStatus->wordValue = wordValue;
                    leftX = posX;
                    yPos8 = posY;
                    cmdCursor->posX = leftX;
                    cmdCursor->yOffs = yPos8;
                    cmdCursor->xExtent = xExtent;
                    cmdCursor->bottomPos = extY;
                    cmdCursor++;
                    offsetRemainingX = leftX + params->widthFloat;
                    yOffs = yPos8 + params->heightFloat;
                    mainRight = xExtent + params->uOrigin;
                    bottomPos = extY + params->vOrigin;
                    cmdCursor->posX = leftX;
                    cmdCursor->yOffs = yOffs;
                    cmdCursor->xExtent = xExtent;
                    cmdCursor->bottomPos = bottomPos;
                    cmdCursor++;
                    cmdCursor->posX = offsetRemainingX;
                    cmdCursor->yOffs = yPos8;
                    cmdCursor->xExtent = mainRight;
                    cmdCursor->bottomPos = extY;
                    cmdCursor++;
                    cmdCursor->posX = offsetRemainingX;
                    cmdCursor->yOffs = yOffs;
                    cmdCursor->xExtent = mainRight;
                    cmdCursor->bottomPos = bottomPos;
                }
            }
        }
    }
}

/*
 * Queues one sprite-batch cell at (posX, posY) with its own depth: the quad is the batch's
 * width x height pixels, sampling from texel (sizeX, sizeY) scaled by invTexWidth/invTexHeight
 * over the batch's cell size. It takes a 0x80-byte block of four vertices from the batch's vertex
 * heap, stores it in a new primitive record (page 0) and writes x, y, u and v of each vertex and
 * `color` into the vertex's third word, which is z in the D3DTLVERTEX layout these blocks have
 * (Gfx_InitTLVertices fills it with the batch's mode word otherwise). So `color` is a depth, not
 * a colour. There is no immediate-draw path. Nothing in src/ calls it.
 */
// FUNCTION: FF7 0x00673547
void __cdecl Gfx_EmitScaledSpriteQuad(int posX, int posY, int sizeX, int sizeY, int color, int renderObj)
{
    float yPos;
    GfxDrawPoolNode *prim;
    float innerLeft;
    float tmpBottom;
    GfxScaledSpriteQuad *quadPointer;
    GfxSpriteBatch *spriteParams;
    float offTop;
    GfxSpriteQuadPrim *primData;
    float widthScaled;
    GfxScaledSpriteQuad *blockPtr;
    float ySize;
    float xFloat;
    float right;

    if (renderObj != 0) {
        prim = (GfxDrawPoolNode *)Gfx_AcquireBatchPrimitive(renderObj);
        spriteParams = (GfxSpriteBatch *)((GfxRenderObject *)renderObj)->spriteBatch;
        if (prim != 0 && spriteParams != 0) {
            blockPtr = (GfxScaledSpriteQuad *)(AllocBlockFromHeap(spriteParams->vertexHeap));
            quadPointer = (GfxScaledSpriteQuad *)blockPtr;
            if (quadPointer != 0) {
                widthScaled = sizeX * spriteParams->invTexWidth;
                ySize = sizeY * spriteParams->invTexHeight;
                primData = (GfxSpriteQuadPrim *)(prim->batchBlock);
                primData->quadPointer = (int)quadPointer;
                primData->wordValue = 0;
                xFloat = posX;
                yPos = posY;
                quadPointer->posX = xFloat;
                quadPointer->posY = yPos;
                quadPointer->color = color;
                quadPointer->xSize = widthScaled;
                quadPointer->ySize = ySize;
                quadPointer++;
                innerLeft = xFloat + spriteParams->widthFloat;
                offTop = yPos + spriteParams->heightFloat;
                right = widthScaled + spriteParams->uOrigin;
                tmpBottom = ySize + spriteParams->vOrigin;
                quadPointer->posX = xFloat;
                quadPointer->posY = offTop;
                quadPointer->color = color;
                quadPointer->xSize = widthScaled;
                quadPointer->ySize = tmpBottom;
                quadPointer++;
                quadPointer->posX = innerLeft;
                quadPointer->posY = yPos;
                quadPointer->color = color;
                quadPointer->xSize = right;
                quadPointer->ySize = ySize;
                quadPointer++;
                quadPointer->posX = innerLeft;
                quadPointer->posY = offTop;
                quadPointer->color = color;
                quadPointer->xSize = right;
                quadPointer->ySize = tmpBottom;
            }
        }
    }
}

/*
 * Queues one sprite-batch cell at (posX, posY) whose texture position is given directly in
 * texture coordinates: `sizeX` and `sizeY` are the top-left u and v (not scaled), and the quad
 * spans the batch's width x height pixels and its cell size in texture coordinates
 * (uOrigin/vOrigin). It takes a 0x80-byte block of four pre-initialised vertices from the batch's
 * vertex heap, stores it in a new primitive record with page 0 and writes x, y, u and v (order
 * top-left, bottom-left, top-right, bottom-right). Nothing in src/ calls it.
 */
// FUNCTION: FF7 0x006736e7
void __cdecl Gfx_EmitSpriteQuad(int posX, int posY, float sizeX, float sizeY, int renderObj)
{
    float xPosition;
    float yPos;
    GfxDrawPoolNode *prim;
    float stepBottom;
    float rightX;
    float innerLeft;
    GfxSpriteQuadCmd *quadPointer;
    GfxSpriteBatch *spriteParams;
    float offTop;
    GfxSpriteQuadPrim *primData;
    GfxSpriteQuadCmd *allocQuad;

    if (renderObj != 0) {
        prim = (GfxDrawPoolNode *)Gfx_AcquireBatchPrimitive(renderObj);
        spriteParams = (GfxSpriteBatch *)((GfxRenderObject *)renderObj)->spriteBatch;
        if (prim != 0 && spriteParams != 0) {
            allocQuad = (GfxSpriteQuadCmd *)(AllocBlockFromHeap(spriteParams->vertexHeap));
            quadPointer = (GfxSpriteQuadCmd *)allocQuad;
            if (quadPointer != 0) {
                primData = (GfxSpriteQuadPrim *)(prim->batchBlock);
                primData->quadPointer = (int)quadPointer;
                primData->wordValue = 0;
                xPosition = posX;
                yPos = posY;
                quadPointer->posX = xPosition;
                quadPointer->posY = yPos;
                quadPointer->sizeX = sizeX;
                quadPointer->sizeY = sizeY;
                quadPointer++;
                innerLeft = xPosition + spriteParams->widthFloat;
                offTop = yPos + spriteParams->heightFloat;
                rightX = sizeX + spriteParams->uOrigin;
                stepBottom = sizeY + spriteParams->vOrigin;
                quadPointer->posX = xPosition;
                quadPointer->posY = offTop;
                quadPointer->sizeX = sizeX;
                quadPointer->sizeY = stepBottom;
                quadPointer++;
                quadPointer->posX = innerLeft;
                quadPointer->posY = yPos;
                quadPointer->sizeX = rightX;
                quadPointer->sizeY = sizeY;
                quadPointer++;
                quadPointer->posX = innerLeft;
                quadPointer->posY = offTop;
                quadPointer->sizeX = rightX;
                quadPointer->sizeY = stepBottom;
            }
        }
    }
}

/*
 * Gfx_EmitSpriteQuad with a depth: the same quad at (posX, posY) from texture coordinates
 * (sizeX, sizeY) over the batch's cell size, but `wordValue` is written into each vertex's third
 * word, z in the D3DTLVERTEX layout of these blocks. The record's page word stays 0, so the word
 * is a depth, not a page or mode. Nothing in src/ calls it.
 */
// FUNCTION: FF7 0x0067384b
void __cdecl Gfx_EmitSpriteQuadWithWord(int posX, int posY, float sizeX, float sizeY, int wordValue, int renderObj)
{
    float xPosition;
    float yPos;
    GfxDrawPoolNode *prim;
    float stepBottom;
    float rightX;
    float innerLeft;
    GfxSpriteQuadWord *quadPointer;
    GfxSpriteBatch *spriteParams;
    float offTop;
    GfxSpriteQuadPrim *primData;
    GfxSpriteQuadWord *allocQuad;

    if (renderObj != 0) {
        prim = (GfxDrawPoolNode *)Gfx_AcquireBatchPrimitive(renderObj);
        spriteParams = (GfxSpriteBatch *)((GfxRenderObject *)renderObj)->spriteBatch;
        if (prim != 0 && spriteParams != 0) {
            allocQuad = (GfxSpriteQuadWord *)(AllocBlockFromHeap(spriteParams->vertexHeap));
            quadPointer = (GfxSpriteQuadWord *)allocQuad;
            if (quadPointer != 0) {
                primData = (GfxSpriteQuadPrim *)(prim->batchBlock);
                primData->quadPointer = (int)quadPointer;
                primData->wordValue = 0;
                xPosition = posX;
                yPos = posY;
                quadPointer->posX = xPosition;
                quadPointer->posY = yPos;
                quadPointer->wordValue = wordValue;
                quadPointer->sizeX = sizeX;
                quadPointer->sizeY = sizeY;
                quadPointer++;
                innerLeft = xPosition + spriteParams->widthFloat;
                offTop = yPos + spriteParams->heightFloat;
                rightX = sizeX + spriteParams->uOrigin;
                stepBottom = sizeY + spriteParams->vOrigin;
                quadPointer->posX = xPosition;
                quadPointer->posY = offTop;
                quadPointer->wordValue = wordValue;
                quadPointer->sizeX = sizeX;
                quadPointer->sizeY = stepBottom;
                quadPointer++;
                quadPointer->posX = innerLeft;
                quadPointer->posY = yPos;
                quadPointer->wordValue = wordValue;
                quadPointer->sizeX = rightX;
                quadPointer->sizeY = sizeY;
                quadPointer++;
                quadPointer->posX = innerLeft;
                quadPointer->posY = offTop;
                quadPointer->wordValue = wordValue;
                quadPointer->sizeX = rightX;
                quadPointer->sizeY = stepBottom;
            }
        }
    }
}

/*
 * Uploads color palette data to render object texture palette.
 */
// FUNCTION: FF7 0x006739d3
int __cdecl Gfx_UploadRenderObjectPalette(int context, int index, int count, int offset, int renderObj)
{
    int result;

    result = 0;
    if (renderObj != 0 && ((GfxRenderObject *)renderObj)->hundredIndex != 0 && ((GfxTexturedHundredRecord *)((GfxRenderObject *)renderObj)->hundredIndex)->uploaded != 0) {
        result = Gfx_UploadTexturePalette(context, index, count, offset, ((GfxTexturedHundredRecord *)((GfxRenderObject *)renderObj)->hundredIndex)->uploaded);
    }
    return result;
}

/*
 * Applies callback to each geometry group in polygon set.
 */
// FUNCTION: FF7 0x00673a25
void __cdecl Gfx_ForEachPolygonSetGeometry(DxGroupCallback callback, int model)
{
    struct {
        int copyGeometry;
        int part;
        int geometry;
        int group;
        GfxPolygonSetPart *locked;
        struct GfxEngineContext *engine;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    if (callback != 0 && model != 0) {
        frame.engine = (struct GfxEngineContext *)((int)GetEngineContext());
        for (frame.group = 0; frame.group < (((GfxPolygonSet *)model)->groupCount); frame.group++) {
            frame.locked = (GfxPolygonSetPart *)Gfx_LockPolygonSetGroup(frame.group, (struct GfxPolygonSet *)model, frame.engine);
            if (frame.locked != 0) {
                frame.geometry = (frame.locked->geometry);
                for (frame.part = 0; frame.part < (frame.locked->partCount); frame.part++) {
                    frame.copyGeometry = frame.geometry;
                    callback(frame.part, frame.copyGeometry);
                    frame.geometry += 0x20;
                }
                Gfx_UnlockPolygonSetGroup(frame.group, (struct GfxPolygonSet *)model, frame.engine);
            }
        }
    }
}

/*
 * Applies callback to paired parts across two polygon sets.
 */
// FUNCTION: FF7 0x00673ae3
void __cdecl Gfx_ForEachPolygonSetPartPair(DxGroupPairCallback callback, int left, int right)
{
    struct {
        int copyGeometry;
        int copyOther;
        int copyVector;
        int other;
        int part;
        int geometry;
        int vector;
        int group;
        GfxPolygonSetPart *locked;
        struct GfxEngineContext *engine;
        GfxPolygonSetPairTarget *rightLocked;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    if (callback != 0 && left != 0) {
        frame.engine = (struct GfxEngineContext *)((int)GetEngineContext());
        if ((((GfxPolygonSet *)left)->groupCount) == (((GfxPolygonSet *)right)->groupCount)) {
            for (frame.group = 0; frame.group < (((GfxPolygonSet *)left)->groupCount); frame.group++) {
                frame.locked = (GfxPolygonSetPart *)Gfx_LockPolygonSetGroup(frame.group, (struct GfxPolygonSet *)left, frame.engine);
                frame.rightLocked = (GfxPolygonSetPairTarget *)Gfx_LockPolygonSetGroup(frame.group, (struct GfxPolygonSet *)right, frame.engine);
                if (frame.locked != 0 && frame.rightLocked != 0) {
                    if ((frame.locked->partCount) == (frame.rightLocked->partCount)) {
                        frame.geometry = (frame.locked->geometry);
                        frame.other = (frame.rightLocked->pairedPart);
                        frame.vector = (frame.rightLocked->vector);
                        for (frame.part = 0; frame.part < (frame.locked->partCount); frame.part++) {
                            frame.copyGeometry = frame.geometry;
                            frame.copyOther = frame.other;
                            frame.copyVector = frame.vector;
                            callback(frame.part, frame.copyGeometry, frame.copyVector, frame.copyOther);
                            frame.geometry += 0x20;
                        }
                    } else
                        break;
                }
                if (frame.locked != 0)
                    Gfx_UnlockPolygonSetGroup(frame.group, (struct GfxPolygonSet *)left, frame.engine);
                if (frame.rightLocked != 0)
                    Gfx_UnlockPolygonSetGroup(frame.group, (struct GfxPolygonSet *)right, frame.engine);
            }
        }
    }
}

/*
 * Applies callback to each part in a polygon set.
 */
// FUNCTION: FF7 0x00673c30
void __cdecl Gfx_ForEachPolygonSetPart(DxGroupPartCallback callback, int model)
{
    struct {
        int copyGeometry;
        int copyVector;
        int part;
        int geometry;
        int vector;
        int group;
        GfxPolygonSetEntry *locked;
        struct GfxEngineContext *engine;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    if (callback != 0 && model != 0) {
        frame.engine = (struct GfxEngineContext *)((int)GetEngineContext());
        for (frame.group = 0; frame.group < (((GfxPolygonSet *)model)->groupCount); frame.group++) {
            frame.locked = (GfxPolygonSetEntry *)Gfx_LockPolygonSetGroup(frame.group, (struct GfxPolygonSet *)model, frame.engine);
            if (frame.locked != 0 && (frame.locked->indexBase) != 0) {
                frame.geometry = (frame.locked->vertices);
                frame.vector = (frame.locked->vertexTable);
                for (frame.part = 0; frame.part < (frame.locked->elementCount); frame.part++) {
                    frame.copyGeometry = frame.geometry;
                    frame.copyVector = frame.vector;
                    callback(((int *)(frame.locked->indexBase))[frame.part], frame.copyVector, frame.copyGeometry);
                    frame.vector += 0xc;
                    frame.geometry += 0x20;
                }
            }
            if (frame.locked != 0)
                Gfx_UnlockPolygonSetGroup(frame.group, (struct GfxPolygonSet *)model, frame.engine);
        }
    }
}

/*
 * Fills texture page buffer with formatted pixel data.
 */
// FUNCTION: FF7 0x00673d26
void __cdecl Gfx_FillTexturePageBuffer(GfxTexturePageFill *fill)
{
    int basePointer;
    int pitchSkip;
    int rows;
    int sourceSkip;
    int rowEnd;
    short *fbPtr;
    GfxEngineContext *engine;
    int colIdx;
    int rowIndex;
    int rowEndX;
    short *writePtr;
    int destinationStart;
    int pixels;
    int copyBytes;
    int destinationLine;
    int srcRow;
    int rowStep;

    if (fill->stepX == 0 && fill->stepY == 0) {
        copyBytes = fill->width * fill->bytesPerPixel;
        rows = fill->height;
        pitchSkip = fill->pitch - fill->width * fill->bytesPerPixel;
        sourceSkip = 0;
        basePointer = fill->surfacePixels;
        destinationStart = basePointer + fill->originX * fill->bytesPerPixel + fill->originY * fill->pitch;
        pixels = fill->pixelData;
        Mem_CopyDwordRows(copyBytes, rows, pitchSkip, sourceSkip, (void *)destinationStart, (void *)pixels);
    } else if (fill->bytesPerPixel == 2) {
        engine = GetEngineContext();
        rowEndX = fill->originX + fill->width * fill->stepX;
        rowEnd = fill->originY + fill->height * fill->stepY;
        rowStep = (fill->pitch >> 1) * fill->stepY;
        srcRow = fill->surfacePixels;
        srcRow += (fill->originX + fill->originY * (fill->pitch >> 1)) * 2;
        destinationLine = fill->pixelData;
        for (rowIndex = fill->originY; rowIndex < rowEnd; rowIndex += fill->stepY) {
            fbPtr = (short *)srcRow;
            writePtr = (short *)destinationLine;
            if (rowIndex < engine->displayHeight) {
                for (colIdx = fill->originX; colIdx < rowEndX; colIdx += fill->stepX) {
                    if (colIdx < engine->displayWidth) {
                        *writePtr = *fbPtr;
                    } else {
                        *writePtr = 0;
                    }
                    fbPtr = (short *)((int)fbPtr + fill->stepX * 2);
                    writePtr++;
                }
            } else {
                for (colIdx = fill->originX; colIdx < rowEndX; colIdx += fill->stepX) {
                    *writePtr = 0;
                    writePtr++;
                }
            }
            srcRow += rowStep * 2;
            destinationLine += fill->width * 2;
        }
    }
}

/*
 * Creates a DirectDraw surface texture and registers handle.
 */
// FUNCTION: FF7 0x00673f5c
int __cdecl Gfx_CreateSurfaceTexture(GfxTexturePageFill *fill)
{
    GfxPixelFormat *surfDescPtr;
    int surfaceType;
    GfxTexHeader *pTexRec;
    int state;
    GfxEngineContext *engine;
    SwRasterizerState *subCtx;

    state = 1;
    pTexRec = 0;
    if (fill != 0) {
        engine = GetEngineContext();
        pTexRec = (GfxTexHeader *)Gfx_AllocTexRecord();
        if (pTexRec != 0) {
            if (fill->isEnabled != 0) {
                pTexRec->colorKey = 1;
                pTexRec->modeFlag = 1;
            }
            pTexRec->minColorBits = 4;
            pTexRec->maxColorBits = 8;
            pTexRec->minAlphaBits = 0;
            pTexRec->maxAlphaBits = 0;
            pTexRec->minBitsPerPixel = 0x10;
            pTexRec->maxBitsPerPixel = 0x20;
            surfDescPtr = 0;
            switch (fill->mode) {
            case GFX_FILL_SOURCE_FRONT_BUFFER:
            case GFX_FILL_SOURCE_BACK_BUFFER:
                surfDescPtr = &engine->targetFormat;
                break;
            case GFX_FILL_SOURCE_PIXELS:
                surfDescPtr = &fill->format;
                break;
            }
            if (surfDescPtr != 0) {
                pTexRec->pixelBuffer =
                    (int)GameHeapAllocRaw(fill->width * fill->height *
                                              surfDescPtr->bytesPerPixel,
                                          // STRING: FF7 0x0090cef8
                                          "C:\\lib\\src\\graphics\\dx_sfx.cpp", 3203);
                if (pTexRec->pixelBuffer != 0) {
                    fill->pixelData = pTexRec->pixelBuffer;
                    pTexRec->bitsPerPixel = surfDescPtr->bitsPerPixel;
                    pTexRec->format = *surfDescPtr;
                    pTexRec->format.width = fill->width;
                    pTexRec->format.height = fill->height;
                    pTexRec->format.pitch = surfDescPtr->bytesPerPixel;
                    pTexRec->format.pixels = 0;
                    switch (fill->mode) {
                    case GFX_FILL_SOURCE_FRONT_BUFFER:
                    case GFX_FILL_SOURCE_BACK_BUFFER:
                        if (fill->mode == GFX_FILL_SOURCE_FRONT_BUFFER) {
                            surfaceType = 0;
                        } else {
                            surfaceType = 1;
                        }
                        fill->surfaceWidth = surfDescPtr->width;
                        fill->surfaceHeight = surfDescPtr->height;
                        fill->bytesPerPixel = surfDescPtr->bytesPerPixel;
                        fill->pitch = surfDescPtr->pitch;
                        fill->surfacePixels = surfDescPtr->pixels;
                        if (engine->driverType == GFX_DRIVER_SOFTWARE) {
                            subCtx = engine->rasterBase;
                            if (subCtx->usesAlignedHeap != 0) {
                                fill->pitch = subCtx->rowBytes;
                                fill->surfacePixels = subCtx->alignedBase;
                            }
                        }
                        if (GfxLockSurfaceCall(surfaceType) != 0) {
                            Gfx_FillTexturePageBuffer(fill);
                            state = 0;
                            GfxUnlockSurfaceCall(surfaceType);
                        }
                        break;
                    case GFX_FILL_SOURCE_PIXELS:
                        Gfx_FillTexturePageBuffer(fill);
                        state = 0;
                        break;
                    }
                }
            }
        }
        if (state != 0) {
            Gfx_DestroyTexture(pTexRec);
            pTexRec = 0;
        }
    }
    return (int)pTexRec;
}

/*
 * Creates a 32-bit framebuffer-compatible surface texture.
 */
// FUNCTION: FF7 0x006741e8
int __cdecl Gfx_CreateFrameBufferTexture32(GfxTexturePageFill *fill)
{
    int readFlag;
    int sourceDesc;
    int depth;
    GfxTexHeader *texturePtr;
    int pixelBytes;
    int destroyNeeded;
    GfxEngineContext *engine;

    depth = 0x20;
    pixelBytes = 4;
    destroyNeeded = 1;
    texturePtr = 0;
    if (fill != 0) {
        engine = GetEngineContext();
        texturePtr = (GfxTexHeader *)Gfx_AllocTexRecord();
        if (texturePtr != 0) {
            if (fill->isEnabled != 0) {
                texturePtr->colorKey = 1;
                texturePtr->modeFlag = 1;
            }
            texturePtr->minColorBits = 4;
            texturePtr->maxColorBits = 8;
            texturePtr->minAlphaBits = 0;
            texturePtr->maxAlphaBits = 0;
            texturePtr->minBitsPerPixel = 0x10;
            texturePtr->maxBitsPerPixel = 0x20;
            sourceDesc = 0;
            switch (fill->mode) {
            case GFX_FILL_SOURCE_FRONT_BUFFER:
            case GFX_FILL_SOURCE_BACK_BUFFER:
                sourceDesc = (int)&engine->targetFormat;
                break;
            case GFX_FILL_SOURCE_PIXELS:
                sourceDesc = (int)&fill->format;
                break;
            }
            if (sourceDesc != 0) {
                texturePtr->pixelBuffer = (int)GameHeapAllocRaw(
                    fill->width * fill->height * pixelBytes,
                    // STRING: FF7 0x0090cf18
                    "C:\\lib\\src\\graphics\\dx_sfx.cpp", 3341);
                if (texturePtr->pixelBuffer != 0) {
                    fill->pixelData = texturePtr->pixelBuffer;
                    texturePtr->bitsPerPixel = depth;
                    PixelFormatLoadMaskTemplate((GfxPixelMaskTemplate *)&g_GfxPixelFormatMaskTemplate, &texturePtr->format);
                    texturePtr->format.width = fill->width;
                    texturePtr->format.height = fill->height;
                    texturePtr->format.pitch = 0;
                    texturePtr->format.pixels = 0;
                    switch (fill->mode) {
                    case GFX_FILL_SOURCE_FRONT_BUFFER:
                    case GFX_FILL_SOURCE_BACK_BUFFER:
                        if (fill->mode == GFX_FILL_SOURCE_FRONT_BUFFER) {
                            readFlag = GFX_FILL_SOURCE_FRONT_BUFFER;
                        } else {
                            readFlag = GFX_FILL_SOURCE_BACK_BUFFER;
                        }
                        Gl_ReadFrameBuffer(fill->originX, fill->originY,
                                           fill->width,
                                           fill->height,
                                           engine->displayHeight,
                                           fill->stepX,
                                           fill->stepY, readFlag,
                                           texturePtr->pixelBuffer);
                        destroyNeeded = 0;
                        break;
                    }
                }
            }
        }
        if (destroyNeeded != 0) {
            Gfx_DestroyTexture(texturePtr);
            texturePtr = 0;
        }
    }
    return (int)texturePtr;
}

/*
 * Clears 184-byte record and resets internal handles.
 */
// FUNCTION: FF7 0x00674402
void __cdecl Gfx_ClearTexturePageFill(int value)
{
    ZeroFillMemory32(184, (void *)(value));
}

/*
 * Creates a render object backed by offscreen surface.
 */
// FUNCTION: FF7 0x00674418
int __cdecl Gfx_CreateSurfaceRenderObject(int parent, int blendMode, int flag, struct GfxTexturePageFill *surface, int layer, int userData)
{
    // Five used scalar slots precede the 116-byte options object. The switch
    // temporary is compiler-generated; the resulting native frame is 140 bytes.
    struct {
        int zero;
        GfxRenderObject *result;
        PfileLoadOptions *view;
        GfxTexHeader *texture;
        GfxEngineContext *engine;
        PfileLoadOptions options;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    frame.engine = GetEngineContext();
    frame.result = 0;
    frame.zero = 0;
    frame.view = &frame.options;
    Pfile_InitLoadOptionsWithBlend(blendMode, frame.view);
    frame.view->flags |= 4;
    if (flag != 0)
        frame.view->emitterFlags |= 2;
    frame.view->drawPool = layer;
    if (frame.engine->driverType == GFX_DRIVER_OPENGL) {
        frame.texture = (GfxTexHeader *)Gfx_CreateFrameBufferTexture32(surface);
        frame.texture->flags |= 1;
    } else
        frame.texture = (GfxTexHeader *)Gfx_CreateSurfaceTexture(surface);
    if (frame.texture != 0) {
        frame.view->texture = (int)frame.texture;
        frame.result = (GfxRenderObject *)Gfx_CreateRenderObject(1, parent, frame.view, (char *)frame.zero, (void *)userData);
        if (frame.result != 0) {
            switch (surface->mode) {
            case GFX_FILL_SOURCE_FRONT_BUFFER:
            case GFX_FILL_SOURCE_BACK_BUFFER:
                frame.result->releaseTexture = 1;
                break;
            }
        }
    }
    return (int)frame.result;
}
} /* extern "C" */
