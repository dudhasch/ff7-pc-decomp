// src/lib/src/graphics/dx_spr.cpp
// Original: C:\lib\src\graphics\dx_spr.cpp
// Contents: Begin and draw emitters for 2D and 3D untextured, textured and line primitives through
//   Direct3D execute buffers, plus index-buffer and fixed-batch submission helpers for sprites and
//   quads.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/api/graphics.h"
#include "lib/codegen.h"
#include "lib/api/mem.h"
#include "lib/types/graphics.h"
#include "lib/types/mem.h"
#include <windows.h>
#include <d3d.h>
#include "lib/enums/graphics.h"
#include "lib/com_abi.h"

struct GfxEngineContext;
extern "C" {


/*
 * Direct3D begin emitter for untextured 2D batches (flat and Gouraud triangles and quads): the
 * driver's begin2DUntextured and emitBegin2DUntextured slots, which Gfx_EmitPrimitiveBatchList
 * calls once before drawing a batch's nodes. When the polygon set `emitter` has
 * hasGroupState set, it applies the set's groupState as an untextured render state
 * (Gfx_ApplyStateUntexturedCall); `model`, the first batch node, is not used.
 */
// FUNCTION: FF7 0x0066d060
void __cdecl Gfx_EmitBegin2DUntextured(GfxPolygonSet *emitter, GfxBatchNode *model, GfxEngineContext *driver)
{
    int state;

    if (emitter->hasGroupState != 0) {
        state = emitter->groupState;
        Gfx_ApplyStateUntexturedCall((void *)(state), driver);
    }
}

/*
 * Direct3D begin emitter (slot begin2DTexturedNoTex) that GfxSelectPrimitiveEmitters picks for a
 * textured 2D kind whose group state's texture surface has no palette record. When the polygon
 * set `emitter` has hasGroupState set, it applies the set's groupState as a textured render state
 * (Gfx_ApplyStateTexturedCall) without selecting a texture page; `model` is not used.
 */
// FUNCTION: FF7 0x0066d08a
void __cdecl Gfx_EmitBegin2DTexturedNoTex(GfxPolygonSet *emitter, GfxBatchNode *model, GfxEngineContext *driver)
{
    int state;

    if (emitter->hasGroupState != 0) {
        state = emitter->groupState;
        Gfx_ApplyStateTexturedCall((void *)(state), driver);
    }
}

/*
 * Direct3D begin emitter (slot begin2DTexturedOpaque) that GfxSelectPrimitiveEmitters picks for a
 * textured 2D kind when the texture surface's palette has groupCount 1 or the emitter flags have 0x8000 (and not
 * 0x10000). When the polygon set `emitter` has hasGroupState set, it applies the set's
 * groupState as a textured render state and, unless it is -1, selects the texture page of the
 * render object that owns the batch node `model` (its texturePage) with Gfx_SelectTexturePage.
 * Nothing here concerns opacity: it differs from the "blended" variant only by the page select.
 */
// FUNCTION: FF7 0x0066d0b4
void __cdecl Gfx_EmitBegin2DTexturedOpaque(GfxPolygonSet *emitter, GfxBatchNode *model, GfxEngineContext *driver)
{
    int state;

    if (emitter->hasGroupState != 0) {
        state = emitter->groupState;
        Gfx_ApplyStateTexturedCall((void *)(state), driver);
        if (model->owner->texturePage != -1) {
            Gfx_SelectTexturePage(model->owner->texturePage, (void *)(state));
        }
    }
}

/*
 * Direct3D begin emitter (slot begin2DTexturedBlended) that GfxSelectPrimitiveEmitters picks for a
 * textured 2D kind whose palette's groupCount is not 1 (or with emitter flag 0x10000). When the
 * polygon set `emitter` has hasGroupState set, it applies the set's groupState as a
 * textured render state; the texture pages are left to Gfx_EmitDraw2DTextured, which selects one
 * per run of primitives. The body is the same as Gfx_EmitBegin2DTexturedNoTex; `model` is not
 * used.
 */
// FUNCTION: FF7 0x0066d100
void __cdecl Gfx_EmitBegin2DTexturedBlended(GfxPolygonSet *emitter, GfxBatchNode *model, GfxEngineContext *driver)
{
    int state;

    if (emitter->hasGroupState != 0) {
        state = emitter->groupState;
        Gfx_ApplyStateTexturedCall((void *)(state), driver);
    }
}

/*
 * Direct3D begin emitter for untextured 3D batches (flat and Gouraud triangles and quads): the
 * begin3DUntextured and emitBegin3DUntextured slots. When the polygon set `emitter` has hasGroupState
 * set, it applies the set's groupState as an untextured render state and sets the
 * Direct3D 5 device's world transform from the render object that owns the batch node `model`:
 * the matrix its worldMatrix points to when matrixByReference is set, else the 4x4 matrix stored in
 * the render object (both set by Gfx_SetBatchContextMatrix).
 */
// FUNCTION: FF7 0x0066d12a
void __cdecl Gfx_EmitBegin3DUntextured(GfxPolygonSet *emitter, GfxBatchNode *model, GfxEngineContext *driver)
{
    int state;
    if (emitter->hasGroupState != 0) {
        state = emitter->groupState;
        Gfx_ApplyStateUntexturedCall((void *)state, driver);
        if (model->owner->matrixByReference != 0) {
            ((IDirect3DDevice2 *)driver->d3dDevice2)->SetTransform(D3DTRANSFORMSTATE_WORLD, (D3DMATRIX *)model->owner->worldMatrix);
        } else {
            ((IDirect3DDevice2 *)driver->d3dDevice2)->SetTransform(D3DTRANSFORMSTATE_WORLD, (D3DMATRIX *)&model->owner->inlineMatrix);
        }
    }
}

/*
 * Direct3D begin emitter (slot begin3DTexturedNoTex) that GfxSelectPrimitiveEmitters picks for a
 * textured 3D kind whose group state's texture surface has no palette record. When the polygon
 * set `emitter` has hasGroupState set, it applies the set's groupState as a textured render state,
 * selects no page, and sets the world transform from the render object owning the batch node `model`: the
 * matrix worldMatrix points to when matrixByReference is set, else the matrix stored in the render
 * object.
 */
// FUNCTION: FF7 0x0066d1b3
void __cdecl Gfx_EmitBegin3DTexturedNoTex(GfxPolygonSet *emitter, GfxBatchNode *model, GfxEngineContext *driver)
{
    int state;
    if (emitter->hasGroupState != 0) {
        state = emitter->groupState;
        Gfx_ApplyStateTexturedCall((void *)state, driver);
        if (model->owner->matrixByReference != 0) {
            ((IDirect3DDevice2 *)driver->d3dDevice2)->SetTransform(D3DTRANSFORMSTATE_WORLD, (D3DMATRIX *)model->owner->worldMatrix);
        } else {
            ((IDirect3DDevice2 *)driver->d3dDevice2)->SetTransform(D3DTRANSFORMSTATE_WORLD, (D3DMATRIX *)&model->owner->inlineMatrix);
        }
    }
}

/*
 * Direct3D begin emitter (slot begin3DTexturedOpaque) that GfxSelectPrimitiveEmitters picks for a
 * textured 3D kind when the texture surface's palette has groupCount 1 or the emitter flags have 0x8000 (and not
 * 0x10000). When the polygon set `emitter` has hasGroupState set, it applies the set's
 * groupState as a textured render state, selects the texture page of the render object owning
 * the batch node `model` (texturePage) unless it is -1, and sets the world transform: the matrix
 * worldMatrix points to when matrixByReference is set, else the matrix stored in the render
 * object. As in 2D, "opaque" only means the page select.
 */
// FUNCTION: FF7 0x0066d23c
void __cdecl Gfx_EmitBegin3DTexturedOpaque(GfxPolygonSet *emitter, GfxBatchNode *model, GfxEngineContext *driver)
{
    int state;
    if (emitter->hasGroupState != 0) {
        state = emitter->groupState;
        Gfx_ApplyStateTexturedCall((void *)state, driver);
        if (model->owner->texturePage != -1)
            Gfx_SelectTexturePage(model->owner->texturePage, (void *)(state));
        if (model->owner->matrixByReference != 0) {
            ((IDirect3DDevice2 *)driver->d3dDevice2)->SetTransform(D3DTRANSFORMSTATE_WORLD, (D3DMATRIX *)model->owner->worldMatrix);
        } else {
            ((IDirect3DDevice2 *)driver->d3dDevice2)->SetTransform(D3DTRANSFORMSTATE_WORLD, (D3DMATRIX *)&model->owner->inlineMatrix);
        }
    }
}

/*
 * Direct3D begin emitter (slot begin3DTexturedBlended) that GfxSelectPrimitiveEmitters picks for a
 * textured 3D kind whose palette's groupCount is not 1 (or with emitter flag 0x10000). When the
 * polygon set `emitter` has hasGroupState set, it applies the set's groupState as a
 * textured render state and sets the world transform from the render object owning the batch node
 * `model` (the matrix worldMatrix points to when matrixByReference is set, else the one stored in the render object). It binds no
 * texture: Gfx_EmitDraw3DTextured selects a page per run of primitives. The body is the same as
 * Gfx_EmitBegin3DTexturedNoTex.
 */
// FUNCTION: FF7 0x0066d2eb
void __cdecl Gfx_EmitBegin3DTexturedBlended(GfxPolygonSet *emitter, GfxBatchNode *model, GfxEngineContext *driver)
{
    int state;
    if (emitter->hasGroupState != 0) {
        state = emitter->groupState;
        Gfx_ApplyStateTexturedCall((void *)state, driver);
        if (model->owner->matrixByReference != 0) {
            ((IDirect3DDevice2 *)driver->d3dDevice2)->SetTransform(D3DTRANSFORMSTATE_WORLD, (D3DMATRIX *)model->owner->worldMatrix);
        } else {
            ((IDirect3DDevice2 *)driver->d3dDevice2)->SetTransform(D3DTRANSFORMSTATE_WORLD, (D3DMATRIX *)&model->owner->inlineMatrix);
        }
    }
}

/*
 * Direct3D begin emitter for line batches (kinds 0x12 and 0x13, slots beginLine and
 * beginLineGouraud). When the polygon set `emitter` has hasGroupState set, it applies the set's
 * groupState as an untextured render state; `model` is not used. The body is the same
 * as Gfx_EmitBegin2DUntextured.
 */
// FUNCTION: FF7 0x0066d374
void __cdecl Gfx_EmitBeginLine(GfxPolygonSet *emitter, GfxBatchNode *model, GfxEngineContext *driver)
{
    int state;

    if (emitter->hasGroupState != 0) {
        state = emitter->groupState;
        Gfx_ApplyStateUntexturedCall((void *)(state), driver);
    }
}

/*
 * Direct3D draw emitter for one batch node of a 2D batch that needs no per-primitive texture
 * page: the draw2DUntextured, emitDraw2DUntextured and emitDraw2DTextured slots (flat, Gouraud,
 * and textured kinds whose texture surface has no palette record). It draws the node's screen-space vertices
 * (D3DVT_TLVERTEX) as an indexed triangle list with IDirect3DDevice2::DrawIndexedPrimitive
 * (vertices, vertexCount, indices, indexCount), not with an
 * execute buffer. The draw flags are D3DDP_DONOTUPDATEEXTENTS, plus D3DDP_DONOTCLIP when the
 * polygon set's drawUnsorted (Gfx_CreateRenderObject's first argument) is 0.
 *
 * With the context's configApplied option set and a texture in the owning render object's state
 * record, it first moves every vertex's u and v by half a texel (0.5 * invWidth, 0.5 * invHeight)
 * in place. DDERR_WASSTILLDRAWING is counted (D3D5CountWasStillDrawing) and retried; any other
 * error is logged and the node is dropped.
 */
// FUNCTION: FF7 0x0066d39e
void __cdecl Gfx_EmitDraw2DUntextured(GfxPolygonSet *emitter, GfxDraw2DRecord *record, struct GfxEngineContext *driver)
{
    ComDirect3DDevice2 *device;
    int vertexSize;
    float texShiftV;
    float offsetX;
    GfxRenderObject *textureRec;
    int status;

    if (emitter->drawUnsorted != 0) {
        vertexSize = D3DDP_DONOTUPDATEEXTENTS;
    } else {
        vertexSize = D3DDP_DONOTCLIP | D3DDP_DONOTUPDATEEXTENTS;
    }
    device = (ComDirect3DDevice2 *)driver->d3dDevice2;
    if (driver->configApplied != 0) {
        textureRec = record->owner;
        if (textureRec->groupState->texture != 0) {
            offsetX = -(textureRec->invWidth * -0.5f);
            texShiftV = -(textureRec->invHeight * -0.5f);
            Gfx_OffsetVertexTexCoords(offsetX, texShiftV, record->vertexCount, (GfxVertexTextured *)(record->vertices));
        }
    }
    do {
        status = device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, D3DVT_TLVERTEX, record->vertices, record->vertexCount, record->indices, record->indexCount, vertexSize);
        if (status == 0) {
            return;
        }
        if (status == DDERR_WASSTILLDRAWING) {
            D3D5CountWasStillDrawing(1, driver);
        } else {
            // STRING: FF7 0x0090ca84
            CheckDirectDrawResult(status, "C:\\lib\\src\\graphics\\dx_spr.cpp", 255);
            status = 0;
        }
    } while (status != 0);
}

/*
 * Direct3D draw emitter (slot draw2DTextured) for one batch node of a textured 2D batch whose
 * texture surface has a palette record. When the polygon set `emitter` has hasGroupState set, it walks the
 * node's primitives in runs that share a texture page (one page byte per primitive, written
 * through commandIndex), selects each run's page with Gfx_SelectTexturePage and draws the run's
 * screen-space vertices (D3DVT_TLVERTEX) as an indexed triangle list with DrawIndexedPrimitive.
 * The run's vertex and index counts are the owning render object's per-primitive counts
 * (verticesPerPrim, indicesPerPrim) times its length. With configApplied set, each run's texture coordinates are first moved
 * by half a texel in place. Clip flags and error handling are as in Gfx_EmitDraw2DUntextured.
 */
// FUNCTION: FF7 0x0066d4a5
void __cdecl Gfx_EmitDraw2DTextured(GfxPolygonSet *emitter, GfxDraw2DTexturedRecord *record, struct GfxEngineContext *driver)
{
    int vtxFormat;
    ComDirect3DDevice2 *device;
    int leftCount;
    float shiftX;
    unsigned char *subPagePtr;
    float offsetYAmount;
    int vtxPos, state, accumV, pgIdx;
    GfxTexturedRunInfo *info;
    int nodeRunLength, currAccumU;
    GfxVertexTextured *vBegin;
    int code;
    if (emitter->drawUnsorted)
        vtxFormat = D3DDP_DONOTUPDATEEXTENTS;
    else
        vtxFormat = D3DDP_DONOTCLIP | D3DDP_DONOTUPDATEEXTENTS;
    device = (ComDirect3DDevice2 *)driver->d3dDevice2;
    info = record->runInfo;
    shiftX = -(info->sizeX * -0.5f);
    offsetYAmount = -(info->sizeY * -0.5f);
    if (emitter->hasGroupState) {
        state = emitter->groupState;
        subPagePtr = record->pagePtr;
        vtxPos = record->vertexPos;
        leftCount = record->leftCount;
        while (leftCount > 0) {
            nodeRunLength = 0;
            vBegin = (GfxVertexTextured *)vtxPos;
            currAccumU = info->stepU;
            accumV = info->stepV;
            pgIdx = *subPagePtr;
            Gfx_SelectTexturePage(pgIdx, (void *)state);
            subPagePtr++;
            nodeRunLength++;
            while (nodeRunLength < leftCount) {
                if (pgIdx == *subPagePtr) {
                    currAccumU += info->stepU;
                    accumV += info->stepV;
                    subPagePtr++;
                    nodeRunLength++;
                } else {
                    break;
                }
            }
            vtxPos += (info->stepU * nodeRunLength) << 5;
            leftCount -= nodeRunLength;
            if (driver->configApplied)
                Gfx_OffsetVertexTexCoords(shiftX, offsetYAmount, currAccumU, vBegin);
            do {
                code = device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, D3DVT_TLVERTEX, (int)vBegin, currAccumU, record->indices, accumV, vtxFormat);
                if (code == 0)
                    break;
                if (code == DDERR_WASSTILLDRAWING)
                    D3D5CountWasStillDrawing(1, driver);
                else {
                    // STRING: FF7 0x0090caa4
                    CheckDirectDrawResult(code, "C:\\lib\\src\\graphics\\dx_spr.cpp", 385);
                    code = 0;
                }
            } while (code != 0);
        }
    }
}

/*
 * Direct3D draw emitter for one batch node of a line batch (slots drawLineFlat and
 * drawLineGouraud): draws the node's screen-space vertices (D3DVT_TLVERTEX) as an indexed line
 * list (D3DPT_LINELIST) with DrawIndexedPrimitive, from the node's vertices, vertexCount,
 * indices and indexCount. Clip flags and error handling are as in
 * Gfx_EmitDraw2DUntextured: D3DDP_DONOTCLIP is added when the polygon set's drawUnsorted is 0, and
 * DDERR_WASSTILLDRAWING is counted and retried.
 */
// FUNCTION: FF7 0x0066d67e
void __cdecl Gfx_EmitDrawLine(GfxPolygonSet *emitter, GfxDraw3DRecord *record, struct GfxEngineContext *driver)
{
    int stride;
    ComDirect3DDevice2 *d3dDevice;
    int res;

    if (emitter->drawUnsorted != 0) {
        stride = D3DDP_DONOTUPDATEEXTENTS;
    } else {
        stride = D3DDP_DONOTCLIP | D3DDP_DONOTUPDATEEXTENTS;
    }
    d3dDevice = (ComDirect3DDevice2 *)driver->d3dDevice2;
    do {
        res = d3dDevice->DrawIndexedPrimitive(D3DPT_LINELIST, D3DVT_TLVERTEX, record->vertices, record->vertexCount, record->indices, record->indexCount, stride);
        if (res == 0) {
            return;
        }
        if (res == DDERR_WASSTILLDRAWING) {
            D3D5CountWasStillDrawing(1, driver);
        } else {
            // STRING: FF7 0x0090cac4
            CheckDirectDrawResult(res, "C:\\lib\\src\\graphics\\dx_spr.cpp", 446);
            res = 0;
        }
    } while (res != 0);
}

/*
 * Direct3D draw emitter for one batch node of a 3D batch that needs no per-primitive texture
 * page: the draw3DUntextured, emitDraw3DUntextured and emitDraw3DTextured slots. It draws the
 * node's lit model-space vertices (D3DVT_LVERTEX, transformed by the world matrix the begin
 * emitter set) as an indexed triangle list with DrawIndexedPrimitive, from the node's vertex
 * pointer (vertices), vertexCount, indices and indexCount. Clip flags and
 * error handling are as in Gfx_EmitDraw2DUntextured.
 */
// FUNCTION: FF7 0x0066d724
void __cdecl Gfx_EmitDraw3DUntextured(GfxPolygonSet *emitter, GfxDraw3DRecord *record, struct GfxEngineContext *driver)
{
    int stride;
    ComDirect3DDevice2 *d3dDevice;
    int res;

    if (emitter->drawUnsorted != 0) {
        stride = D3DDP_DONOTUPDATEEXTENTS;
    } else {
        stride = D3DDP_DONOTCLIP | D3DDP_DONOTUPDATEEXTENTS;
    }
    d3dDevice = (ComDirect3DDevice2 *)driver->d3dDevice2;
    do {
        res = d3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, D3DVT_LVERTEX, record->vertices, record->vertexCount, record->indices, record->indexCount, stride);
        if (res == 0) {
            return;
        }
        if (res == DDERR_WASSTILLDRAWING) {
            D3D5CountWasStillDrawing(1, driver);
        } else {
            // STRING: FF7 0x0090cae4
            CheckDirectDrawResult(res, "C:\\lib\\src\\graphics\\dx_spr.cpp", 508);
            res = 0;
        }
    } while (res != 0);
}

/*
 * Direct3D draw emitter (slot draw3DTextured) for one batch node of a textured 3D batch whose
 * texture surface has a palette record. When the polygon set `emitter` has hasGroupState set, it walks the
 * node's primitives in runs that share a texture page (one page byte per primitive), selects each
 * run's page with Gfx_SelectTexturePage and draws the run's lit model-space vertices
 * (D3DVT_LVERTEX) as an indexed triangle list with DrawIndexedPrimitive; the run's vertex and
 * index counts are the owning render object's per-primitive counts (verticesPerPrim, indicesPerPrim) times its
 * length. Unlike the 2D variant it applies no half-texel shift. Clip flags and error handling are
 * as in Gfx_EmitDraw2DUntextured.
 */
// FUNCTION: FF7 0x0066d7ca
void __cdecl Gfx_EmitDraw3DTextured(GfxPolygonSet *emitter, GfxDraw3DTexturedRecord *record, struct GfxEngineContext *driver)
{
    int vertFmt;
    ComDirect3DDevice2 *device;
    int spritesToDo;
    unsigned char *subPagePtr;
    int vertexOff, state, accumV, basePage;
    Gfx3DTexturedInfo *info;
    int nodeRunLength, uRunTotal, vertStart, code;
    if (emitter->drawUnsorted)
        vertFmt = D3DDP_DONOTUPDATEEXTENTS;
    else
        vertFmt = D3DDP_DONOTCLIP | D3DDP_DONOTUPDATEEXTENTS;
    device = (ComDirect3DDevice2 *)driver->d3dDevice2;
    info = record->runInfo;
    if (emitter->hasGroupState) {
        state = emitter->groupState;
        subPagePtr = record->pagePtr;
        vertexOff = record->vertexOffset;
        spritesToDo = record->spritesToDo;
        while (spritesToDo > 0) {
            nodeRunLength = 0;
            vertStart = vertexOff;
            uRunTotal = info->uRunStep;
            accumV = info->vRunStep;
            basePage = *subPagePtr;
            Gfx_SelectTexturePage(basePage, (void *)state);
            subPagePtr++;
            nodeRunLength++;
            while (nodeRunLength < spritesToDo) {
                if (basePage == *subPagePtr) {
                    uRunTotal += info->uRunStep;
                    accumV += info->vRunStep;
                    subPagePtr++;
                    nodeRunLength++;
                } else {
                    break;
                }
            }
            vertexOff += (info->uRunStep * nodeRunLength) << 5;
            spritesToDo -= nodeRunLength;
            do {
                code = device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, D3DVT_LVERTEX, vertStart, uRunTotal, record->drawFlags, accumV, vertFmt);
                if (code == 0)
                    break;
                if (code == DDERR_WASSTILLDRAWING)
                    D3D5CountWasStillDrawing(1, driver);
                else {
                    // STRING: FF7 0x0090cb04
                    CheckDirectDrawResult(code, "C:\\lib\\src\\graphics\\dx_spr.cpp", 624);
                    code = 0;
                }
            } while (code != 0);
        }
    }
}

/*
 * Sets up a render object's primitive state for drawing: its index buffer (from the mesh bank, in
 * reversed winding with flag 2) and per-primitive vertex and index counts, then picks its
 * begin and draw emitters from the driver's slots by primitive type and, for textured kinds, by the
 * group state's texture surface and palette.
 */
// FUNCTION: FF7 0x0066d95d
void __cdecl GfxSelectPrimitiveEmitters(int flags, GfxRenderObject *primitive)
{
    GfxEngineContext *ctx;
    GfxDriverOps *driver;
    GfxPolygonSet *parentPtr;
    int variant;
    GfxPaletteRecord *texObj;
    GfxGroupRenderState *subObject;
    GfxTextureSurface *backing;
    GfxPaletteRecord *altTexture;
    GfxGroupRenderState *selInnerAlt;
    GfxTextureSurface *shellAlt;
    ctx = GetEngineContext();
    driver = GetGraphicsDriverObject(ctx);
    if (primitive) {
        primitive->emitterFlags = flags;
        primitive->beginEmitter = 0;
        primitive->drawEmitter = 0;
        if (primitive->kind >= GFX_DRAW_FLAT_LINE_2D && primitive->kind < GFX_DRAW_GOURAUD_LINE_2D + 1) {
            primitive->lineVertexCount = 2;
            primitive->lineIndexCount = 2;
            primitive->lineVertexBytes = 64;
            primitive->lineIndexBytes = 4;
            primitive->lineIndexBuffer = primitive->meshBank->lineIndices;
        } else {
            if (primitive->kind <= GFX_DRAW_TEXTURED_GOURAUD_TRI_3D) {
                if (primitive->emitterFlags & 2)
                    variant = primitive->meshBank->triIndicesReversed;
                else
                    variant = primitive->meshBank->triIndices;
            } else {
                if (primitive->emitterFlags & 2)
                    variant = primitive->meshBank->quadIndicesReversed;
                else
                    variant = primitive->meshBank->quadIndices;
            }
            primitive->indexBuffer = variant;
            if (primitive->kind <= GFX_DRAW_TEXTURED_GOURAUD_TRI_3D) {
                primitive->verticesPerPrim = 3;
                primitive->indicesPerPrim = 3;
            } else {
                primitive->verticesPerPrim = 4;
                primitive->indicesPerPrim = 6;
            }
            primitive->cursorStep = primitive->verticesPerPrim << 5;
            primitive->indexCursorStep = primitive->indicesPerPrim << 1;
        }
        primitive->textureMode = 0;
        switch (primitive->kind) {
        case GFX_DRAW_FLAT_QUAD_2D:
            primitive->beginEmitter = driver->begin2DUntextured;
            primitive->drawEmitter = driver->draw2DUntextured;
            break;
        case GFX_DRAW_GOURAUD_QUAD_2D:
            primitive->beginEmitter = driver->emitBegin2DUntextured;
            primitive->drawEmitter = driver->emitDraw2DUntextured;
            break;
        case GFX_DRAW_FLAT_TRI_2D:
            primitive->beginEmitter = driver->begin2DUntextured;
            primitive->drawEmitter = driver->draw2DUntextured;
            break;
        case GFX_DRAW_GOURAUD_TRI_2D:
            primitive->beginEmitter = driver->emitBegin2DUntextured;
            primitive->drawEmitter = driver->emitDraw2DUntextured;
            break;
        case GFX_DRAW_TEXTURED_TRI_2D:
        case GFX_DRAW_TEXTURED_GOURAUD_TRI_2D:
        case GFX_DRAW_TEXTURED_QUAD_2D:
        case GFX_DRAW_TEXTURED_GOURAUD_QUAD_2D:
        case GFX_DRAW_SPRITE_BATCH:
            parentPtr = primitive->polygonSetPtr;
            if (parentPtr && parentPtr->groupState) {
                subObject = (GfxGroupRenderState *)parentPtr->groupState;
                backing = subObject->texture;
                if (backing) {
                    texObj = (GfxPaletteRecord *)backing->palette;
                    if (texObj) {
                        if ((texObj->groupCount == 1 || (primitive->emitterFlags & 0x8000)) && !(primitive->emitterFlags & 0x10000)) {
                            primitive->beginEmitter = driver->begin2DTexturedOpaque;
                            primitive->drawEmitter = driver->draw2DTextured;
                            primitive->textureMode = 1;
                        } else {
                            primitive->beginEmitter = driver->begin2DTexturedBlended;
                            primitive->drawEmitter = driver->draw2DTextured;
                            primitive->textureMode = 2;
                        }
                    } else {
                        primitive->beginEmitter = driver->begin2DTexturedNoTex;
                        primitive->drawEmitter = driver->emitDraw2DTextured;
                    }
                }
            }
            break;
        case GFX_DRAW_FLAT_QUAD_3D:
            primitive->beginEmitter = driver->begin3DUntextured;
            primitive->drawEmitter = driver->draw3DUntextured;
            break;
        case GFX_DRAW_GOURAUD_QUAD_3D:
            primitive->beginEmitter = driver->emitBegin3DUntextured;
            primitive->drawEmitter = driver->emitDraw3DUntextured;
            break;
        case GFX_DRAW_FLAT_TRI_3D:
            primitive->beginEmitter = driver->begin3DUntextured;
            primitive->drawEmitter = driver->draw3DUntextured;
            break;
        case GFX_DRAW_GOURAUD_TRI_3D:
            primitive->beginEmitter = driver->emitBegin3DUntextured;
            primitive->drawEmitter = driver->emitDraw3DUntextured;
            break;
        case GFX_DRAW_TEXTURED_TRI_3D:
        case GFX_DRAW_TEXTURED_GOURAUD_TRI_3D:
        case GFX_DRAW_TEXTURED_QUAD_3D:
        case GFX_DRAW_TEXTURED_GOURAUD_QUAD_3D:
            parentPtr = primitive->polygonSetPtr;
            if (parentPtr && parentPtr->groupState) {
                selInnerAlt = (GfxGroupRenderState *)parentPtr->groupState;
                shellAlt = selInnerAlt->texture;
                if (shellAlt) {
                    altTexture = (GfxPaletteRecord *)shellAlt->palette;
                    if (altTexture) {
                        if ((altTexture->groupCount == 1 || (primitive->emitterFlags & 0x8000)) && !(primitive->emitterFlags & 0x10000)) {
                            primitive->beginEmitter = driver->begin3DTexturedOpaque;
                            primitive->drawEmitter = driver->draw3DTextured;
                            primitive->textureMode = 1;
                        } else {
                            primitive->beginEmitter = driver->begin3DTexturedBlended;
                            primitive->drawEmitter = driver->draw3DTextured;
                            primitive->textureMode = 2;
                        }
                    } else {
                        primitive->beginEmitter = driver->begin3DTexturedNoTex;
                        primitive->drawEmitter = driver->emitDraw3DTextured;
                    }
                }
            }
            break;
        case GFX_DRAW_FLAT_LINE_2D:
            primitive->beginEmitter = driver->beginLine;
            primitive->drawEmitter = driver->drawLineFlat;
            break;
        case GFX_DRAW_GOURAUD_LINE_2D:
            primitive->beginEmitter = driver->beginLineGouraud;
            primitive->drawEmitter = driver->drawLineGouraud;
            break;
        }
    }
}

/*
 * Writes `count` primitives' vertex indices into dst, starting at baseIndex: three per triangle
 * (vertsPerPrim 3) or six per quad (two triangles), in either winding, advancing dst by entryStride
 * shorts per primitive.
 */
// FUNCTION: FF7 0x0066deb2
void __cdecl Gfx_FillPrimitiveIndexRun(int reversed, int vertsPerPrim, int entryStride, int count, int baseIndex, GfxSpriteIndexBuffer *dst)
{
    int remaining;

    if (vertsPerPrim == 3) {
        if (reversed != 0) {
            for (remaining = count; remaining > 0; remaining--) {
                dst->vertexIndexA = baseIndex;
                dst->vertexIndexB = baseIndex + 1;
                dst->vertexIndexC = baseIndex + 2;
                dst = (GfxSpriteIndexBuffer *)((int)dst + entryStride * 2);
                baseIndex += vertsPerPrim;
            }
        } else {
            for (remaining = count; remaining > 0; remaining--) {
                dst->vertexIndexA = baseIndex + 2;
                dst->vertexIndexB = baseIndex + 1;
                dst->vertexIndexC = baseIndex;
                dst = (GfxSpriteIndexBuffer *)((int)dst + entryStride * 2);
                baseIndex += vertsPerPrim;
            }
        }
    } else if (reversed != 0) {
        for (remaining = count; remaining > 0; remaining--) {
            dst->vertexIndexA = baseIndex;
            dst->vertexIndexB = baseIndex + 1;
            dst->vertexIndexC = baseIndex + 2;
            dst->vertexIndexD = baseIndex + 3;
            dst->vertexIndexE = baseIndex + 2;
            dst->vertexIndexF = baseIndex + 1;
            dst = (GfxSpriteIndexBuffer *)((int)dst + entryStride * 2);
            baseIndex += vertsPerPrim;
        }
    } else {
        for (remaining = count; remaining > 0; remaining--) {
            dst->vertexIndexA = baseIndex + 2;
            dst->vertexIndexB = baseIndex + 1;
            dst->vertexIndexC = baseIndex;
            dst->vertexIndexD = baseIndex + 3;
            dst->vertexIndexE = baseIndex + 1;
            dst->vertexIndexF = baseIndex + 2;
            dst = (GfxSpriteIndexBuffer *)((int)dst + entryStride * 2);
            baseIndex += vertsPerPrim;
        }
    }
}

/*
 * Fills context index buffer with sequential primitive indices.
 */
// FUNCTION: FF7 0x0066e06f
void __cdecl Gfx_FillContextIndexRun(int count, GfxSpriteIndexBuffer *dst, GfxRenderObject *context)
{
    int indexBuf;

    if (context != 0) {
        indexBuf = context->chainHeadNode->dataBytes;
        Gfx_FillPrimitiveIndexRun((context->emitterFlags & 2) != 0, context->verticesPerPrim, context->indicesPerPrim, count, indexBuf, dst);
    }
}

/*
 * Fills sequential index pairs for line primitive rendering.
 */
// FUNCTION: FF7 0x0066e0be
void __cdecl Gfx_FillSequentialIndexPairs(int count, GfxSpriteIndexBuffer *dst)
{
    int index;

    for (index = 0; index < count; index++) {
        dst->vertexIndexA = index << 1;
        dst->vertexIndexB = index * 2 + 1;
        dst = (GfxSpriteIndexBuffer *)((char *)dst + 4);
    }
}

/*
 * Allocates and initializes sprite primitive index buffers.
 */
// FUNCTION: FF7 0x0066e104
void __cdecl Gfx_AllocSpriteIndexBuffers(int count, GfxPrimitiveMeshBank *meshBank)
{
    int entrySize;
    GfxSpriteIndexBuffer *buffer;

    if (meshBank != 0) {
        entrySize = 3;
        // STRING: FF7 0x0090cb24
        buffer = (GfxSpriteIndexBuffer *)GameHeapAlloc(count * entrySize, 2, "C:\\lib\\src\\graphics\\dx_spr.cpp", 964);
        meshBank->triIndicesReversed = (int)buffer;
        Gfx_FillPrimitiveIndexRun(1, 3, entrySize, count, 0, buffer);
        entrySize = 6;
        // STRING: FF7 0x0090cb44
        buffer = (GfxSpriteIndexBuffer *)GameHeapAlloc(count * entrySize, 2, "C:\\lib\\src\\graphics\\dx_spr.cpp", 969);
        meshBank->quadIndicesReversed = (int)buffer;
        Gfx_FillPrimitiveIndexRun(1, 4, entrySize, count, 0, buffer);
        entrySize = 3;
        // STRING: FF7 0x0090cb64
        buffer = (GfxSpriteIndexBuffer *)GameHeapAlloc(count * entrySize, 2, "C:\\lib\\src\\graphics\\dx_spr.cpp", 974);
        meshBank->triIndices = (int)buffer;
        Gfx_FillPrimitiveIndexRun(0, 3, entrySize, count, 0, buffer);
        entrySize = 6;
        // STRING: FF7 0x0090cb84
        buffer = (GfxSpriteIndexBuffer *)GameHeapAlloc(count * entrySize, 2, "C:\\lib\\src\\graphics\\dx_spr.cpp", 979);
        meshBank->quadIndices = (int)buffer;
        Gfx_FillPrimitiveIndexRun(0, 4, entrySize, count, 0, buffer);
        // STRING: FF7 0x0090cba4
        buffer = (GfxSpriteIndexBuffer *)GameHeapAlloc(count << 1, 2, "C:\\lib\\src\\graphics\\dx_spr.cpp", 983);
        meshBank->lineIndices = (int)buffer;
        Gfx_FillSequentialIndexPairs(count, buffer);
    }
}

/*
 * Reserves room for `count` primitives in a render object's batch for this frame; the game's
 * sprite, window and effect code calls it before writing each primitive's vertices.
 *
 * When the batch's batchGeneration is older than its draw pool's generation (ResetFrameDrawPool
 * ran since it was last filled), the batch is emptied and stamped with the current generation, so
 * last frame's primitives drop out. When fewer than `count` slots are left, it takes a new node
 * from the bank's block heap, links it at the head of the batch's chain, and gives it storage for
 * max(count, 0x20) primitives from the array heap: 0x80 bytes of vertices (four 0x20-byte
 * vertices), then 0x10 bytes of indices, then one texture-page byte per primitive. For a request
 * of up to 0x200 primitives the node draws with the batch's shared prebuilt index buffer
 * (regionStart); a larger one fills its own index run (Gfx_FillContextIndexRun).
 *
 * It then points the batch's vertexCursor and indexCursor at the first reserved vertex and
 * index and commandIndex at its page byte, advances the write cursors past the reservation and
 * adds the primitives, vertices and indices to the node's counts. Returns 1, or 0 for a null
 * batch. The heap allocations are not checked: when a heap is exhausted the node pointer is null
 * and the function writes through it.
 */
// FUNCTION: FF7 0x0066e272
int __cdecl AcquireBatchNode(int count, void *batchContext)
{
    GfxBatchNode *node;
    int result;
    int allocCount;
    GfxDrawPool *capacityInfo;
    GfxBatchNode *allocBlock;
    GfxSpriteIndexBuffer *spriteIndicesPtr;
    GfxSortedPrimitive *buffer;
    GfxSortedPrimitive *fallbackArrayPtr;
    GfxSortedPrimitive *pBufferArray;

    result = 0;
    if (batchContext != 0) {
        capacityInfo = ((GfxRenderObject *)batchContext)->drawPoolPtr;
        if ((unsigned int)((GfxRenderObject *)batchContext)->batchGeneration < (unsigned int)capacityInfo->generation) {
            ((GfxRenderObject *)batchContext)->commandCount = 0;
            ((GfxRenderObject *)batchContext)->allocCount = 0;
            ((GfxRenderObject *)batchContext)->chainHeadNode = 0;
            ((GfxRenderObject *)batchContext)->texturePage = 0xFFFFFFFF;
            ((GfxRenderObject *)batchContext)->batchGeneration = capacityInfo->generation;
        }
        if (count > ((GfxRenderObject *)batchContext)->allocCount) {
            allocBlock = (GfxBatchNode *)AllocBlockFromHeap(((GfxRenderObject *)batchContext)->meshBank->indexHeap);
            node = allocBlock;
            node->owner = (GfxRenderObject *)batchContext;
            node->nextNode = ((GfxRenderObject *)batchContext)->chainHeadNode;
            ((GfxRenderObject *)batchContext)->chainHeadNode = node;
            node->commandCount = 0;
            node->vertexCount = 0;
            node->dataBytes = 0;
            if (count > 32) {
                allocCount = count;
                pBufferArray = (GfxSortedPrimitive *)AllocBlockArrayFromHeap(allocCount, ((GfxRenderObject *)batchContext)->meshBank->arrayHeap);
                buffer = pBufferArray;
                node->buffer = (int)buffer;
                node->regionStart = (int)buffer + (allocCount << 7);
                node->regionEnd = node->regionStart + (allocCount << 4);
            } else {
                allocCount = 32;
                fallbackArrayPtr = (GfxSortedPrimitive *)AllocBlockArrayFromHeap(allocCount, ((GfxRenderObject *)batchContext)->meshBank->arrayHeap);
                buffer = fallbackArrayPtr;
                node->buffer = (int)buffer;
                node->regionStart = (int)buffer + 0x1000;
                node->regionEnd = node->regionStart + 0x200;
            }
            if (count > 512) {
                spriteIndicesPtr = (GfxSpriteIndexBuffer *)(node->regionStart);
                Gfx_FillContextIndexRun(allocCount, spriteIndicesPtr, (GfxRenderObject *)batchContext);
            } else {
                node->regionStart = ((GfxRenderObject *)batchContext)->indexBuffer;
            }
            node->vertexBaseWord = node->bufferWord;
            node->indexBaseWord = node->regionStartWord;
            node->cursor = node->regionEnd;
            ((GfxRenderObject *)batchContext)->cursorAWord = node->vertexBaseWord;
            ((GfxRenderObject *)batchContext)->cursorBWord = node->indexBaseWord;
            ((GfxRenderObject *)batchContext)->cursor = node->cursor;
            ((GfxRenderObject *)batchContext)->allocCount = allocCount;
        }
        node = ((GfxRenderObject *)batchContext)->chainHeadNode;
        node->commandCount += count;
        ((GfxRenderObject *)batchContext)->commandCount += count;
        ((GfxRenderObject *)batchContext)->allocCount -= count;
        ((GfxRenderObject *)batchContext)->vertexCursorWord = ((GfxRenderObject *)batchContext)->cursorAWord;
        ((GfxRenderObject *)batchContext)->indexCursorWord = ((GfxRenderObject *)batchContext)->cursorBWord;
        ((GfxRenderObject *)batchContext)->commandIndex = ((GfxRenderObject *)batchContext)->cursor;
        if (count > 1) {
            ((GfxRenderObject *)batchContext)->cursorA += ((GfxRenderObject *)batchContext)->cursorStep * count;
            ((GfxRenderObject *)batchContext)->cursorB += ((GfxRenderObject *)batchContext)->indexCursorStep * count;
            ((GfxRenderObject *)batchContext)->cursor += count;
            node->vertexCount += ((GfxRenderObject *)batchContext)->verticesPerPrim * count;
            node->dataBytes += ((GfxRenderObject *)batchContext)->indicesPerPrim * count;
        } else {
            ((GfxRenderObject *)batchContext)->cursorA += ((GfxRenderObject *)batchContext)->cursorStep;
            ((GfxRenderObject *)batchContext)->cursorB += ((GfxRenderObject *)batchContext)->indexCursorStep;
            (((GfxRenderObject *)batchContext)->cursor)++;
            node->vertexCount += ((GfxRenderObject *)batchContext)->verticesPerPrim;
            node->dataBytes += ((GfxRenderObject *)batchContext)->indicesPerPrim;
        }
        result = 1;
    }
    return result;
}

/*
 * Advances batch context vertex pointer by entry stride.
 */
// FUNCTION: FF7 0x0066e5e3
void __cdecl AdvanceBatchVertexPointer(GfxRenderObject *batchContext)
{
    if (batchContext != 0) {
        batchContext->vertexCursor += batchContext->cursorStep;
    }
}

/*
 * Advances batch vertex pointer and increments command counter.
 */
// FUNCTION: FF7 0x0066e600
void __cdecl AdvanceBatchVertexPointerAndCommandIndex(GfxRenderObject *batchContext)
{
    if (batchContext != 0) {
        batchContext->vertexCursor += batchContext->cursorStep;
        (batchContext->commandIndex)++;
    }
}

/*
 * Clears the active batch generation counter.
 */
// FUNCTION: FF7 0x0066e62c
void __cdecl InvalidateBatchGeneration(void *renderObject)
{
    if (renderObject != 0) {
        ((GfxRenderObject *)renderObject)->batchGeneration = 0;
    }
}

/*
 * Draws what a render object's batch holds this frame: the game's sprite, window and effect code
 * calls it after writing primitives with AcquireBatchNode or the Gfx_Emit* writers. It does
 * nothing unless the batch was filled in the current draw-pool generation (batchGeneration equals the
 * pool's generation), so a batch left from an earlier frame is not drawn again.
 *
 * When depth sorting is on (context sortedDrawEnabled) and the polygon set's groupState
 * has depthSorted set, the batch goes to Gfx_EnqueuePrimitiveBatchForDepthSort and is
 * drawn later in depth order. Otherwise, when the chain has nodes and a begin emitter is set, it
 * calls the begin emitter (beginEmitter) once with the polygon set, the first node and the context, then
 * the draw emitter (drawEmitter) for every node in the chain. GfxSelectPrimitiveEmitters chose both from
 * the driver's slots (on Direct3D the Gfx_EmitBegin* and Gfx_EmitDraw* functions).
 */
// FUNCTION: FF7 0x0066e641
void __cdecl Gfx_EmitPrimitiveBatchList(void *list, struct GfxEngineContext *ctx)
{
    GfxBatchNode *node;
    GfxPolygonSet *polygonSet;
    GfxDrawPool *poolPointer;
    GfxGroupRenderState *state;

    if (list != 0) {
        poolPointer = (GfxDrawPool *)((GfxRenderObject *)list)->drawPool;
        if (((GfxRenderObject *)list)->batchGeneration == poolPointer->generation) {
            polygonSet = (GfxPolygonSet *)((GfxRenderObject *)list)->polygonSet;
            state = (GfxGroupRenderState *)polygonSet->groupState;
            if (ctx->sortedDrawEnabled != 0 && state != 0 && state->depthSorted != 0) {
                Gfx_EnqueuePrimitiveBatchForDepthSort((GfxRenderObject *)list, ctx);
            } else {
                node = (GfxBatchNode *)((GfxRenderObject *)list)->chainHead;
                if (node != 0 && ((GfxRenderObject *)list)->beginEmitter != 0) {
                    ((GfxRenderObject *)list)->beginEmitter(polygonSet, node, ctx);
                    do {
                        ((GfxRenderObject *)list)->drawEmitter(polygonSet, node, ctx);
                        node = node->nextNode;
                    } while (node != 0);
                }
            }
        }
    }
}

/*
 * Sets transformation matrix in batch context by reference or value.
 */
// FUNCTION: FF7 0x0066e70a
void __cdecl Gfx_SetBatchContextMatrix(int byReference, int matrixSource, int batchContext)
{
    if (batchContext != 0) {
        ((GfxRenderObject *)batchContext)->matrixByReference = byReference;
        if (byReference != 0) {
            ((GfxRenderObject *)batchContext)->matrixSource = matrixSource;
        } else {
            ((GfxRenderObject *)batchContext)->inlineMatrix = *(GfxMatrix4x4 *)matrixSource;
        }
    }
}

/*
 * Allocates or reserves primitive batch nodes without command storage.
 */
// FUNCTION: FF7 0x0066e74c
int __cdecl AcquireBatchNodeNoCommandArea(int count, GfxRenderObject *batchContext)
{
    GfxBatchNode *node;
    int result;
    int allocCount;
    GfxDrawPool *capacityInfo;
    GfxBatchNode *allocBlock;
    GfxSpriteIndexBuffer *spriteIndicesPtr;
    GfxSortedPrimitive *buffer;
    GfxSortedPrimitive *fallbackArrayPtr;
    GfxSortedPrimitive *pBufferArray;

    result = 0;
    if (batchContext != 0) {
        capacityInfo = batchContext->drawPoolPtr;
        if ((unsigned int)batchContext->batchGeneration < (unsigned int)capacityInfo->generation) {
            batchContext->commandCount = 0;
            batchContext->allocCount = 0;
            batchContext->chainHeadNode = 0;
            batchContext->batchGeneration = capacityInfo->generation;
        }
        if (count > batchContext->allocCount) {
            allocBlock = (GfxBatchNode *)AllocBlockFromHeap(batchContext->meshBank->indexHeap);
            node = allocBlock;
            node->owner = batchContext;
            node->nextNode = batchContext->chainHeadNode;
            batchContext->chainHeadNode = node;
            node->commandCount = 0;
            node->vertexCount = 0;
            node->dataBytes = 0;
            if (count > 32) {
                allocCount = count;
                pBufferArray = (GfxSortedPrimitive *)AllocBlockArrayFromHeap(allocCount, batchContext->meshBank->arrayHeap);
                buffer = pBufferArray;
                node->buffer = (int)buffer;
                node->regionStart = (int)buffer + (allocCount << 7);
            } else {
                allocCount = 32;
                fallbackArrayPtr = (GfxSortedPrimitive *)AllocBlockArrayFromHeap(allocCount, batchContext->meshBank->arrayHeap);
                buffer = fallbackArrayPtr;
                node->buffer = (int)buffer;
                node->regionStart = (int)buffer + 0x1000;
            }
            if (count > 512) {
                spriteIndicesPtr = (GfxSpriteIndexBuffer *)(node->regionStart);
                Gfx_FillSequentialIndexPairs(allocCount, spriteIndicesPtr);
            } else {
                node->regionStart = batchContext->lineIndexBuffer;
            }
            node->vertexBaseWord = node->bufferWord;
            node->indexBaseWord = node->regionStartWord;
            batchContext->cursorAWord = node->vertexBaseWord;
            batchContext->cursorBWord = node->indexBaseWord;
            batchContext->allocCount = allocCount;
        }
        node = batchContext->chainHeadNode;
        node->commandCount += count;
        batchContext->commandCount += count;
        batchContext->allocCount -= count;
        batchContext->vertexCursorWord = batchContext->cursorAWord;
        batchContext->indexCursorWord = batchContext->cursorBWord;
        batchContext->commandIndex = batchContext->cursor;
        if (count > 1) {
            batchContext->cursorA += batchContext->lineVertexBytes * count;
            batchContext->cursorB += batchContext->lineIndexBytes * count;
            node->vertexCount += batchContext->lineVertexCount * count;
            node->dataBytes += batchContext->lineIndexCount * count;
        } else {
            batchContext->cursorA += batchContext->lineVertexBytes;
            batchContext->cursorB += batchContext->lineIndexBytes;
            node->vertexCount += batchContext->lineVertexCount;
            node->dataBytes += batchContext->lineIndexCount;
        }
        result = 1;
    }
    return result;
}

/*
 * Advances vertex pointer in batch context without command area.
 */
// FUNCTION: FF7 0x0066ea3f
void __cdecl AdvanceBatchVertexPointerNoCommandArea(GfxRenderObject *batchContext)
{
    if (batchContext != 0) {
        batchContext->vertexCursor += batchContext->lineVertexBytes;
    }
}

/*
 * Submits a test batch of two fixed colored quads.
 */
// FUNCTION: FF7 0x0066ea5c
void __cdecl Gfx_SubmitFixedTwoQuadBatch(GfxRenderObject *quadBatch)
{
    float quadHeight;
    float width;
    float originX;
    GfxTlVertex *vertexBottomLeft;
    GfxTlVertex *pVertex0;
    GfxTlVertex *fixedTopRightPtr;
    GfxTlVertex *pNextBottomLeft;
    GfxTlVertex *nextTopRemaining;
    GfxTlVertex *vertex3Ptr;
    GfxTlVertex *vertexTopRight;
    GfxTlVertex *fixedBottomRightPtr;
    int vertexColor;
    int numberPrims;
    float originY;
    int zDepth;

    numberPrims = 2;
    if (AcquireBatchNode(numberPrims, quadBatch) != 0) {
        originX = 3e+02f;
        originY = 3e+02f;
        zDepth = FLOAT_0_2_BITS;
        width = 1e+02f;
        quadHeight = 1e+02f;
        vertexColor = 0xFF000080;
        pVertex0 = (GfxTlVertex *)(quadBatch->vertexCursor);
        pVertex0->x = originX;
        pVertex0->y = originY;
        pVertex0->zBits = zDepth;
        pVertex0->rhwBits = FLOAT_ONE_BITS;
        pVertex0->color = vertexColor;
        pVertex0->specular = 0xFF000000;
        vertexColor = 0xFF008000;
        vertexBottomLeft = (GfxTlVertex *)(quadBatch->vertexCursor + 0x20);
        vertexBottomLeft->x = originX;
        vertexBottomLeft->y = originY + quadHeight;
        vertexBottomLeft->zBits = zDepth;
        vertexBottomLeft->rhwBits = FLOAT_ONE_BITS;
        vertexBottomLeft->color = vertexColor;
        vertexBottomLeft->specular = 0xFF000000;
        vertexColor = 0xFF800000;
        vertexTopRight = (GfxTlVertex *)(quadBatch->vertexCursor + 0x40);
        vertexTopRight->x = originX + width;
        vertexTopRight->y = originY;
        vertexTopRight->zBits = zDepth;
        vertexTopRight->rhwBits = FLOAT_ONE_BITS;
        vertexTopRight->color = vertexColor;
        vertexTopRight->specular = 0xFF000000;
        vertexColor = 0xFF808080;
        vertex3Ptr = (GfxTlVertex *)(quadBatch->vertexCursor + 0x60);
        vertex3Ptr->x = originX + width;
        vertex3Ptr->y = originY + quadHeight;
        vertex3Ptr->zBits = zDepth;
        vertex3Ptr->rhwBits = FLOAT_ONE_BITS;
        vertex3Ptr->color = vertexColor;
        vertex3Ptr->specular = 0xFF000000;
        quadBatch->vertexCursor += quadBatch->cursorStep;
        originX += 5e+01f;
        originY += 5e+01f;
        vertexColor = 0xFF00FF00;
        nextTopRemaining = (GfxTlVertex *)(quadBatch->vertexCursor);
        nextTopRemaining->x = originX;
        nextTopRemaining->y = originY;
        nextTopRemaining->zBits = zDepth;
        nextTopRemaining->rhwBits = FLOAT_ONE_BITS;
        nextTopRemaining->color = vertexColor;
        nextTopRemaining->specular = 0xFF000000;
        pNextBottomLeft = (GfxTlVertex *)(quadBatch->vertexCursor + 0x20);
        pNextBottomLeft->x = originX;
        pNextBottomLeft->y = originY + quadHeight;
        pNextBottomLeft->zBits = zDepth;
        pNextBottomLeft->rhwBits = FLOAT_ONE_BITS;
        pNextBottomLeft->color = vertexColor;
        pNextBottomLeft->specular = 0xFF000000;
        fixedTopRightPtr = (GfxTlVertex *)(quadBatch->vertexCursor + 0x40);
        fixedTopRightPtr->x = originX + width;
        fixedTopRightPtr->y = originY;
        fixedTopRightPtr->zBits = zDepth;
        fixedTopRightPtr->rhwBits = FLOAT_ONE_BITS;
        fixedTopRightPtr->color = vertexColor;
        fixedTopRightPtr->specular = 0xFF000000;
        fixedBottomRightPtr = (GfxTlVertex *)(quadBatch->vertexCursor + 0x60);
        fixedBottomRightPtr->x = originX + width;
        fixedBottomRightPtr->y = originY + quadHeight;
        fixedBottomRightPtr->zBits = zDepth;
        fixedBottomRightPtr->rhwBits = FLOAT_ONE_BITS;
        fixedBottomRightPtr->color = vertexColor;
        fixedBottomRightPtr->specular = 0xFF000000;
        Gfx_EmitPrimitiveBatchList(quadBatch, GetEngineContext());
        InvalidateBatchGeneration(quadBatch);
    }
}

/*
 * Submits a test batch of four fixed textured quads.
 */
// FUNCTION: FF7 0x0066ed3d
void __cdecl Gfx_SubmitFixedFourTexturedQuadBatch(GfxRenderObject *batch)
{
    char flagC;
    int color;
    float originV;
    float h;
    float originY;
    GfxTlVertex *vertexBottomLeft;
    GfxTlVertex *pVertex0;
    GfxTlVertex *nextBottomRight;
    GfxTlVertex *vertex6Ptr;
    GfxTlVertex *localVertexBottomRight;
    GfxTlVertex *currVertTopRight;
    char lowByte;
    float originU;
    float quadW;
    float currentX;
    float du;
    float dv;
    GfxTlVertex *vertexBottomLeft2Ptr;
    GfxTlVertex *workNextTopLeft;
    GfxTlVertex *pFourthBottomLeft;
    GfxTlVertex *pFourthTopLeft;
    GfxTlVertex *outPVertexBottomRight;
    GfxTlVertex *thirdTopRightPtr;
    int quadCnt;
    int rhwValue;
    GfxTlVertex *bottomRightPtr;
    char sentinel;
    int zValue;
    GfxTlVertex *thirdBottomLeft;
    GfxTlVertex *activeVertexTopLeft;
    GfxTlVertex *pVtxTR;

    quadCnt = 4;
    if (AcquireBatchNode(quadCnt, batch) != 0) {
        currentX = 1e+02f;
        originY = 2e+02f;
        zValue = FLOAT_0_2_BITS;
        rhwValue = FLOAT_ONE_BITS;
        quadW = 1e+02f;
        h = 1e+02f;
        originU = 0.0f;
        originV = 0.0f;
        du = 1.0f;
        dv = 1.0f;
        color = 0xFF0000FF;
        pVertex0 = (GfxTlVertex *)(batch->vertexCursor);
        pVertex0->x = currentX;
        pVertex0->y = originY;
        pVertex0->zBits = zValue;
        pVertex0->rhwBits = FLOAT_ONE_BITS;
        pVertex0->color = color;
        pVertex0->specular = 0xFF000000;
        pVertex0->u = originU;
        pVertex0->v = originV;
        color = 0xFF00FF00;
        vertexBottomLeft = (GfxTlVertex *)(batch->vertexCursor + 0x20);
        vertexBottomLeft->x = currentX;
        vertexBottomLeft->y = originY + h;
        vertexBottomLeft->zBits = zValue;
        vertexBottomLeft->rhwBits = FLOAT_ONE_BITS;
        vertexBottomLeft->color = color;
        vertexBottomLeft->specular = 0xFF000000;
        vertexBottomLeft->u = originU;
        vertexBottomLeft->v = originV + dv;
        color = 0xFFFF0000;
        pVtxTR = (GfxTlVertex *)(batch->vertexCursor + 0x40);
        pVtxTR->x = currentX + quadW;
        pVtxTR->y = originY;
        pVtxTR->zBits = zValue;
        pVtxTR->rhwBits = FLOAT_ONE_BITS;
        pVtxTR->color = color;
        pVtxTR->specular = 0xFF000000;
        pVtxTR->u = originU + du;
        pVtxTR->v = originV;
        color = 0xFFFFFFFF;
        bottomRightPtr = (GfxTlVertex *)(batch->vertexCursor + 0x60);
        bottomRightPtr->x = currentX + quadW;
        bottomRightPtr->y = originY + h;
        bottomRightPtr->zBits = zValue;
        bottomRightPtr->rhwBits = FLOAT_ONE_BITS;
        bottomRightPtr->color = color;
        bottomRightPtr->specular = 0xFF000000;
        bottomRightPtr->u = originU + du;
        bottomRightPtr->v = originV + dv;
        batch->vertexCursor += batch->cursorStep;
        currentX += 5e+01f;
        originY += 5e+01f;
        color = 0xFF00FF00;
        workNextTopLeft = (GfxTlVertex *)(batch->vertexCursor);
        workNextTopLeft->x = currentX;
        workNextTopLeft->y = originY;
        workNextTopLeft->zBits = zValue;
        workNextTopLeft->rhwBits = FLOAT_ONE_BITS;
        workNextTopLeft->color = color;
        workNextTopLeft->specular = 0xFF000000;
        workNextTopLeft->u = originU;
        workNextTopLeft->v = originV;
        vertexBottomLeft2Ptr = (GfxTlVertex *)(batch->vertexCursor + 0x20);
        vertexBottomLeft2Ptr->x = currentX;
        vertexBottomLeft2Ptr->y = originY + h;
        vertexBottomLeft2Ptr->zBits = zValue;
        vertexBottomLeft2Ptr->rhwBits = FLOAT_ONE_BITS;
        vertexBottomLeft2Ptr->color = color;
        vertexBottomLeft2Ptr->specular = 0xFF000000;
        vertexBottomLeft2Ptr->u = originU;
        vertexBottomLeft2Ptr->v = originV + dv;
        vertex6Ptr = (GfxTlVertex *)(batch->vertexCursor + 0x40);
        vertex6Ptr->x = currentX + quadW;
        vertex6Ptr->y = originY;
        vertex6Ptr->zBits = zValue;
        vertex6Ptr->rhwBits = FLOAT_ONE_BITS;
        vertex6Ptr->color = 0xFF0000FF;
        vertex6Ptr->specular = 0xFF000000;
        vertex6Ptr->u = originU + du;
        vertex6Ptr->v = originV;
        nextBottomRight = (GfxTlVertex *)(batch->vertexCursor + 0x60);
        nextBottomRight->x = currentX + quadW;
        nextBottomRight->y = originY + h;
        nextBottomRight->zBits = zValue;
        nextBottomRight->rhwBits = FLOAT_ONE_BITS;
        nextBottomRight->color = 0xFF0000FF;
        nextBottomRight->specular = 0xFF000000;
        nextBottomRight->u = originU + du;
        nextBottomRight->v = originV + dv;
        batch->vertexCursor += batch->cursorStep;
        currentX += 5e+01f;
        originY += 5e+01f;
        lowByte = 0x80;
        sentinel = 0;
        flagC = 0;
        color = 0xFF0000FF;
        activeVertexTopLeft = (GfxTlVertex *)(batch->vertexCursor);
        activeVertexTopLeft->x = currentX;
        activeVertexTopLeft->y = originY;
        activeVertexTopLeft->zBits = zValue;
        activeVertexTopLeft->rhwBits = FLOAT_ONE_BITS;
        activeVertexTopLeft->color = color;
        activeVertexTopLeft->specular = 0xFF000000;
        activeVertexTopLeft->u = originU;
        activeVertexTopLeft->v = originV;
        color = 0xFF00FF00;
        thirdBottomLeft = (GfxTlVertex *)(batch->vertexCursor + 0x20);
        thirdBottomLeft->x = currentX;
        thirdBottomLeft->y = originY + h;
        thirdBottomLeft->zBits = zValue;
        thirdBottomLeft->rhwBits = FLOAT_ONE_BITS;
        thirdBottomLeft->color = color;
        thirdBottomLeft->specular = 0xFF000000;
        thirdBottomLeft->u = originU;
        thirdBottomLeft->v = originV + dv;
        color = 0xFFFF0000;
        thirdTopRightPtr = (GfxTlVertex *)(batch->vertexCursor + 0x40);
        thirdTopRightPtr->x = currentX + quadW;
        thirdTopRightPtr->y = originY;
        thirdTopRightPtr->zBits = zValue;
        thirdTopRightPtr->rhwBits = FLOAT_ONE_BITS;
        thirdTopRightPtr->color = color;
        thirdTopRightPtr->specular = 0xFF000000;
        thirdTopRightPtr->u = originU + du;
        thirdTopRightPtr->v = originV;
        color = 0xFFFFFFFF;
        outPVertexBottomRight = (GfxTlVertex *)(batch->vertexCursor + 0x60);
        outPVertexBottomRight->x = currentX + quadW;
        outPVertexBottomRight->y = originY + h;
        outPVertexBottomRight->zBits = zValue;
        outPVertexBottomRight->rhwBits = FLOAT_ONE_BITS;
        outPVertexBottomRight->color = color;
        outPVertexBottomRight->specular = 0xFF000000;
        outPVertexBottomRight->u = originU + du;
        outPVertexBottomRight->v = originV + dv;
        batch->vertexCursor += batch->cursorStep;
        currentX += 5e+01f;
        originY += 5e+01f;
        color = 0xFF0000FF;
        pFourthTopLeft = (GfxTlVertex *)(batch->vertexCursor);
        pFourthTopLeft->x = currentX;
        pFourthTopLeft->y = originY;
        pFourthTopLeft->zBits = zValue;
        pFourthTopLeft->rhwBits = FLOAT_ONE_BITS;
        pFourthTopLeft->color = color;
        pFourthTopLeft->specular = 0xFF000000;
        pFourthTopLeft->u = originU;
        pFourthTopLeft->v = originV;
        color = 0xFF00FF00;
        pFourthBottomLeft = (GfxTlVertex *)(batch->vertexCursor + 0x20);
        pFourthBottomLeft->x = currentX;
        pFourthBottomLeft->y = originY + h;
        pFourthBottomLeft->zBits = zValue;
        pFourthBottomLeft->rhwBits = FLOAT_ONE_BITS;
        pFourthBottomLeft->color = color;
        pFourthBottomLeft->specular = 0xFF000000;
        pFourthBottomLeft->u = originU;
        pFourthBottomLeft->v = originV + dv;
        color = 0xFFFF0000;
        currVertTopRight = (GfxTlVertex *)(batch->vertexCursor + 0x40);
        currVertTopRight->x = currentX + quadW;
        currVertTopRight->y = originY;
        currVertTopRight->zBits = zValue;
        currVertTopRight->rhwBits = FLOAT_ONE_BITS;
        currVertTopRight->color = color;
        currVertTopRight->specular = 0xFF000000;
        currVertTopRight->u = originU + du;
        currVertTopRight->v = originV;
        color = 0xFFFFFFFF;
        localVertexBottomRight = (GfxTlVertex *)(batch->vertexCursor + 0x60);
        localVertexBottomRight->x = currentX + quadW;
        localVertexBottomRight->y = originY + h;
        localVertexBottomRight->zBits = zValue;
        localVertexBottomRight->rhwBits = FLOAT_ONE_BITS;
        localVertexBottomRight->color = color;
        localVertexBottomRight->specular = 0xFF000000;
        localVertexBottomRight->u = originU + du;
        localVertexBottomRight->v = originV + dv;
        Gfx_EmitPrimitiveBatchList(batch, GetEngineContext());
        InvalidateBatchGeneration(batch);
    }
}

/*
 * Submits a test batch of a fixed colored triangle.
 */
// FUNCTION: FF7 0x0066f455
void __cdecl Gfx_SubmitFixedTriangleBatch(GfxRenderObject *batch)
{
    int color;
    float originU;
    float x0;
    float triH;
    float du;
    int zValue;
    GfxTlVertex *pVertex0;
    float dv;
    int rhwValue;
    GfxTlVertex *secondVert;
    GfxTlVertex *outVertex;
    int count;
    float w;
    float originV;
    float originY;

    count = 1;
    if (AcquireBatchNode(count, batch) != 0) {
        x0 = 2e+02f;
        originY = 1e+02f;
        zValue = FLOAT_0_2_BITS;
        rhwValue = FLOAT_ONE_BITS;
        w = 1e+02f;
        triH = 1e+02f;
        originU = 0.0f;
        originV = 0.0f;
        du = 1.0f;
        dv = 1.0f;
        color = 0xFF0000FF;
        pVertex0 = (GfxTlVertex *)(batch->vertexCursor);
        pVertex0->x = x0;
        pVertex0->y = originY;
        pVertex0->zBits = zValue;
        pVertex0->rhwBits = FLOAT_ONE_BITS;
        pVertex0->color = color;
        pVertex0->specular = 0xFF000000;
        pVertex0->u = originU;
        pVertex0->v = originV;
        color = 0xFF00FF00;
        secondVert = (GfxTlVertex *)(batch->vertexCursor + 0x20);
        secondVert->x = x0;
        secondVert->y = originY + triH;
        secondVert->zBits = zValue;
        secondVert->rhwBits = FLOAT_ONE_BITS;
        secondVert->color = color;
        secondVert->specular = 0xFF000000;
        secondVert->u = originU;
        secondVert->v = originV + dv;
        color = 0xFFFF0000;
        outVertex = (GfxTlVertex *)(batch->vertexCursor + 0x40);
        outVertex->x = x0 + w;
        outVertex->y = originY;
        outVertex->zBits = zValue;
        outVertex->rhwBits = FLOAT_ONE_BITS;
        outVertex->color = color;
        outVertex->specular = 0xFF000000;
        outVertex->u = originU + du;
        outVertex->v = originV;
        Gfx_EmitPrimitiveBatchList(batch, GetEngineContext());
        InvalidateBatchGeneration(batch);
    }
}

/*
 * Submits test batches of textured quad grids.
 */
// FUNCTION: FF7 0x0066f5ff
void __cdecl Gfx_SubmitFixedQuadGridBatches(GfxRenderObject *batch)
{
    float gridX;
    float cellSpan;
    float cellWidth;
    int nBytes;
    float uSpan;
    GfxTlVertex *vertex0;
    GfxTlVertex *pVtxStart;
    GfxTlVertex *quadVertexDPtr;
    GfxTlVertex *trVertex;
    GfxTlVertex *gridVert1;
    int rowIndex;
    float rawTexStartV;
    float uRoot;
    int argb;
    int depth;
    int column;
    GfxTlVertex *rawPVertex;
    GfxTlVertex *pTlVertex;
    GfxTlVertex *lastVert;
    GfxTlVertex *pQuadVertexBR;
    GfxTlVertex *pQuadTR;
    int screenX;
    int spriteCol;
    int rowCounter;
    GfxTlVertex *pVertexC;
    GfxTlVertex *vertex1;
    float positionY;
    float inVSpan;
    GfxTlVertex *pQuadBL;
    GfxTlVertex *quadVert0;
    GfxTlVertex *topRightVertex;
    GfxTlVertex *bottomVert;
    int screenPositionY;
    int rhwOne;

    if (batch->kind != GFX_DRAW_SPRITE_BATCH) {
        if (DEAD_IN_RETAIL != 0) {
            nBytes = 280;
            depth = FLOAT_0_9_BITS;
            rhwOne = FLOAT_ONE_BITS;
            cellWidth = 32.0f;
            cellSpan = 32.0f;
            uRoot = 0.0f;
            rawTexStartV = 0.0f;
            uSpan = 0.25f;
            inVSpan = 0.5f;
            positionY = 0.0f;
            for (rowCounter = 0; rowCounter < 14; rowCounter++) {
                gridX = 0.0f;
                for (column = 0; column < 20; column++) {
                    if (AcquireBatchNode(1, batch) != 0) {
                        argb = 0xFF0000FF;
                        vertex0 = (GfxTlVertex *)(batch->vertexCursor);
                        vertex0->x = gridX;
                        vertex0->y = positionY;
                        vertex0->zBits = depth;
                        vertex0->rhwBits = FLOAT_ONE_BITS;
                        vertex0->color = argb;
                        vertex0->specular = 0xFF000000;
                        vertex0->u = uRoot;
                        vertex0->v = rawTexStartV;
                        argb = 0xFF00FF00;
                        vertex1 = (GfxTlVertex *)(batch->vertexCursor + 0x20);
                        vertex1->x = gridX;
                        vertex1->y = positionY + cellSpan;
                        vertex1->zBits = depth;
                        vertex1->rhwBits = FLOAT_ONE_BITS;
                        vertex1->color = argb;
                        vertex1->specular = 0xFF000000;
                        vertex1->u = uRoot;
                        vertex1->v = rawTexStartV + inVSpan;
                        argb = 0xFFFF0000;
                        pVertexC = (GfxTlVertex *)(batch->vertexCursor + 0x40);
                        pVertexC->x = gridX + cellWidth;
                        pVertexC->y = positionY;
                        pVertexC->zBits = depth;
                        pVertexC->rhwBits = FLOAT_ONE_BITS;
                        pVertexC->color = argb;
                        pVertexC->specular = 0xFF000000;
                        pVertexC->u = uRoot + uSpan;
                        pVertexC->v = rawTexStartV;
                        argb = 0xFFFFFFFF;
                        rawPVertex = (GfxTlVertex *)(batch->vertexCursor + 0x60);
                        rawPVertex->x = gridX + cellWidth;
                        rawPVertex->y = positionY + cellSpan;
                        rawPVertex->zBits = depth;
                        rawPVertex->rhwBits = FLOAT_ONE_BITS;
                        rawPVertex->color = argb;
                        rawPVertex->specular = 0xFF000000;
                        rawPVertex->u = uRoot + uSpan;
                        rawPVertex->v = rawTexStartV + inVSpan;
                    }
                    gridX += 32.0f;
                }
                positionY += 32.0f;
            }
            Gfx_EmitPrimitiveBatchList(batch, GetEngineContext());
            InvalidateBatchGeneration(batch);
            nBytes = 280;
            positionY = 0.0f;
            for (rowCounter = 0; rowCounter < 14; rowCounter++) {
                gridX = 0.0f;
                for (column = 0; column < 20; column++) {
                    if (AcquireBatchNode(1, batch) != 0) {
                        argb = 0xFF0000FF;
                        pVtxStart = (GfxTlVertex *)(batch->vertexCursor);
                        pVtxStart->x = gridX;
                        pVtxStart->y = positionY;
                        pVtxStart->zBits = depth;
                        pVtxStart->rhwBits = FLOAT_ONE_BITS;
                        pVtxStart->color = argb;
                        pVtxStart->specular = 0xFF000000;
                        pVtxStart->u = uRoot;
                        pVtxStart->v = rawTexStartV;
                        argb = 0xFF00FF00;
                        bottomVert = (GfxTlVertex *)(batch->vertexCursor + 0x20);
                        bottomVert->x = gridX;
                        bottomVert->y = positionY + cellSpan;
                        bottomVert->zBits = depth;
                        bottomVert->rhwBits = FLOAT_ONE_BITS;
                        bottomVert->color = argb;
                        bottomVert->specular = 0xFF000000;
                        bottomVert->u = uRoot;
                        bottomVert->v = rawTexStartV + inVSpan;
                        argb = 0xFFFF0000;
                        topRightVertex = (GfxTlVertex *)(batch->vertexCursor + 0x40);
                        topRightVertex->x = gridX + cellWidth;
                        topRightVertex->y = positionY;
                        topRightVertex->zBits = depth;
                        topRightVertex->rhwBits = FLOAT_ONE_BITS;
                        topRightVertex->color = argb;
                        topRightVertex->specular = 0xFF000000;
                        topRightVertex->u = uRoot + uSpan;
                        topRightVertex->v = rawTexStartV;
                        argb = 0xFFFFFFFF;
                        lastVert = (GfxTlVertex *)(batch->vertexCursor + 0x60);
                        lastVert->x = gridX + cellWidth;
                        lastVert->y = positionY + cellSpan;
                        lastVert->zBits = depth;
                        lastVert->rhwBits = FLOAT_ONE_BITS;
                        lastVert->color = argb;
                        lastVert->specular = 0xFF000000;
                        lastVert->u = uRoot + uSpan;
                        lastVert->v = rawTexStartV + inVSpan;
                    }
                    gridX += 32.0f;
                }
                positionY += 32.0f;
            }
            Gfx_EmitPrimitiveBatchList(batch, GetEngineContext());
            InvalidateBatchGeneration(batch);
        } else {
            nBytes = 280;
            if (AcquireBatchNode(nBytes, batch) != 0) {
                *batch->colorBytePtr = 0;
                batch->paletteIndex = 0;
                depth = FLOAT_0_9_BITS;
                rhwOne = FLOAT_ONE_BITS;
                cellWidth = 32.0f;
                cellSpan = 32.0f;
                uRoot = 0.0f;
                rawTexStartV = 0.0f;
                uSpan = 0.25f;
                inVSpan = 0.5f;
                positionY = 0.0f;
                for (rowCounter = 0; rowCounter < 14; rowCounter++) {
                    gridX = 0.0f;
                    for (column = 0; column < 20; column++) {
                        argb = 0xFFFF0000;
                        argb = 0xFF00FF00;
                        argb = 0xFF0000FF;
                        argb = 0xFFFFFFFF;
                        pTlVertex = (GfxTlVertex *)(batch->vertexCursor);
                        pTlVertex->x = gridX;
                        pTlVertex->y = positionY;
                        pTlVertex->zBits = depth;
                        pTlVertex->rhwBits = FLOAT_ONE_BITS;
                        pTlVertex->color = argb;
                        pTlVertex->specular = 0xFF000000;
                        pTlVertex->u = uRoot;
                        pTlVertex->v = rawTexStartV;
                        gridVert1 = (GfxTlVertex *)(batch->vertexCursor + 0x20);
                        gridVert1->x = gridX;
                        gridVert1->y = positionY + cellSpan;
                        gridVert1->zBits = depth;
                        gridVert1->rhwBits = FLOAT_ONE_BITS;
                        gridVert1->color = argb;
                        gridVert1->specular = 0xFF000000;
                        gridVert1->u = uRoot;
                        gridVert1->v = rawTexStartV + inVSpan;
                        trVertex = (GfxTlVertex *)(batch->vertexCursor + 0x40);
                        trVertex->x = gridX + cellWidth;
                        trVertex->y = positionY;
                        trVertex->zBits = depth;
                        trVertex->rhwBits = FLOAT_ONE_BITS;
                        trVertex->color = argb;
                        trVertex->specular = 0xFF000000;
                        trVertex->u = uRoot + uSpan;
                        trVertex->v = rawTexStartV;
                        quadVertexDPtr = (GfxTlVertex *)(batch->vertexCursor + 0x60);
                        quadVertexDPtr->x = gridX + cellWidth;
                        quadVertexDPtr->y = positionY + cellSpan;
                        quadVertexDPtr->zBits = depth;
                        quadVertexDPtr->rhwBits = FLOAT_ONE_BITS;
                        quadVertexDPtr->color = argb;
                        quadVertexDPtr->specular = 0xFF000000;
                        quadVertexDPtr->u = uRoot + uSpan;
                        quadVertexDPtr->v = rawTexStartV + inVSpan;
                        batch->vertexCursor += batch->cursorStep;
                        gridX += 32.0f;
                    }
                    positionY += 32.0f;
                }
                Gfx_EmitPrimitiveBatchList(batch, GetEngineContext());
                InvalidateBatchGeneration(batch);
            }
            nBytes = 280;
            if (AcquireBatchNode(nBytes, batch) != 0) {
                positionY = 0.0f;
                for (rowCounter = 0; rowCounter < 14; rowCounter++) {
                    gridX = 0.0f;
                    for (column = 0; column < 20; column++) {
                        argb = 0xFFFF0000;
                        argb = 0xFF00FF00;
                        argb = 0xFF0000FF;
                        argb = 0xFFFFFFFF;
                        quadVert0 = (GfxTlVertex *)(batch->vertexCursor);
                        quadVert0->x = gridX;
                        quadVert0->y = positionY;
                        quadVert0->zBits = depth;
                        quadVert0->rhwBits = FLOAT_ONE_BITS;
                        quadVert0->color = argb;
                        quadVert0->specular = 0xFF000000;
                        quadVert0->u = uRoot;
                        quadVert0->v = rawTexStartV;
                        pQuadBL = (GfxTlVertex *)(batch->vertexCursor + 0x20);
                        pQuadBL->x = gridX;
                        pQuadBL->y = positionY + cellSpan;
                        pQuadBL->zBits = depth;
                        pQuadBL->rhwBits = FLOAT_ONE_BITS;
                        pQuadBL->color = argb;
                        pQuadBL->specular = 0xFF000000;
                        pQuadBL->u = uRoot;
                        pQuadBL->v = rawTexStartV + inVSpan;
                        pQuadTR = (GfxTlVertex *)(batch->vertexCursor + 0x40);
                        pQuadTR->x = gridX + cellWidth;
                        pQuadTR->y = positionY;
                        pQuadTR->zBits = depth;
                        pQuadTR->rhwBits = FLOAT_ONE_BITS;
                        pQuadTR->color = argb;
                        pQuadTR->specular = 0xFF000000;
                        pQuadTR->u = uRoot + uSpan;
                        pQuadTR->v = rawTexStartV;
                        pQuadVertexBR = (GfxTlVertex *)(batch->vertexCursor + 0x60);
                        pQuadVertexBR->x = gridX + cellWidth;
                        pQuadVertexBR->y = positionY + cellSpan;
                        pQuadVertexBR->zBits = depth;
                        pQuadVertexBR->rhwBits = FLOAT_ONE_BITS;
                        pQuadVertexBR->color = argb;
                        pQuadVertexBR->specular = 0xFF000000;
                        pQuadVertexBR->u = uRoot + uSpan;
                        pQuadVertexBR->v = rawTexStartV + inVSpan;
                        batch->vertexCursor += batch->cursorStep;
                        gridX += 32.0f;
                    }
                    positionY += 32.0f;
                }
                Gfx_EmitPrimitiveBatchList(batch, GetEngineContext());
                InvalidateBatchGeneration(batch);
            }
        }
    } else {
        screenPositionY = 0;
        for (rowIndex = 0; rowIndex < 14; rowIndex++) {
            screenX = 0;
            for (spriteCol = 0; spriteCol < 20; spriteCol++) {
                Gfx_EmitScreenSprite(screenX, screenPositionY, 0, 0, batch);
                screenX += 32;
            }
            screenPositionY += 32;
        }
        Gfx_DrawRenderObjectCall(batch, GetEngineContext());
        Gfx_ResetRenderObjectBatchChain((int)batch);
        Gfx_EmptyRenderObjectBatch((int)batch);
        screenPositionY = 0;
        for (rowIndex = 0; rowIndex < 14; rowIndex++) {
            screenX = 0;
            for (spriteCol = 0; spriteCol < 20; spriteCol++) {
                Gfx_EmitScreenSprite(screenX, screenPositionY, 0, 0, batch);
                screenX += 32;
            }
            screenPositionY += 32;
        }
        Gfx_DrawRenderObjectCall(batch, GetEngineContext());
        Gfx_ResetRenderObjectBatchChain((int)batch);
        Gfx_EmptyRenderObjectBatch((int)batch);
    }
}
} /* extern "C" */
