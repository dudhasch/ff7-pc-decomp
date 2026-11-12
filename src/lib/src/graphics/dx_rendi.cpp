// src/lib/src/graphics/dx_rendi.cpp
// Original: C:\lib\src\graphics\dx_rendi.cpp
// Contents: Direct3D render-state and preset material handling: builds and frees the preset
//   polygon-set and state tables, applies blend, alpha and texture states, and submits polygon sets
//   through DrawPrimitive, with a software fallback path.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/api/mem.h"
#include "lib/pfile.h"
#include "lib/api/graphics.h"
#include "lib/api/polygon.h"
#include "lib/types/graphics.h"
#include "lib/types/polygon.h"
#include "lib/enums/graphics.h"
#include "lib/codegen.h"
#include "lib/com_abi.h"
#include <stdio.h>

extern "C" {
void __cdecl Gfx_DestroyPresetStateTable(struct GfxPresetPolygonSetTable *table);

/*
 * Destroys and frees preset polygon set and material state table records.
 */
// FUNCTION: FF7 0x0068aac0
void __cdecl Gfx_DestroyPresetStateTable(struct GfxPresetPolygonSetTable *table)
{
    int idx;
    GfxPolygonSet *polySet;

    if (table != 0) {
        for (idx = 0; idx < 0x16; idx++) {
            polySet = table->enabledSets[idx];
            if (polySet != 0) {
                Gfx_FreePHundred(polySet->groupState);
                D3DFreePolygonSet((struct GfxPolygonSet *)polySet);
            }
            polySet = table->disabledSets[idx];
            if (polySet != 0) {
                Gfx_FreePHundred(polySet->groupState);
                D3DFreePolygonSet((struct GfxPolygonSet *)polySet);
            }
        }
        Gfx_FreePHundred(table->untextured0);
        Gfx_FreePHundred(table->untextured1);
        // STRING: FF7 0x009107d8
        GameHeapFree(table, "C:\\lib\\src\\graphics\\dx_rendi.cpp", 0x2D);
    }
}

// clang-format off
// GLOBAL: FF7 0x00910798
float g_PolygonSetIdentityMatrix[4][4] = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
// clang-format on
// The callers pass the original's argument shapes, so these keep this TU's own signatures.
int Gfx_AllocUntexturedHundred(int gouraud, int desc, int context);

/*
 * Allocates and initializes preset polygon set and state lookup tables.
 */
// FUNCTION: FF7 0x0068ab84
int __cdecl Gfx_CreatePresetPolygonSetTable(struct GfxEngineContext *context)
{
    int idx;
    PfileLoadOptions opts;
    PfileLoadOptions untexOpts;
    int optsAddress;
    PolygonHundredState *texRecord;
    int flags;
    PolygonHundredState *flatRecord;
    int bitVal;
    GfxPolygonSet *pTexturedSet;
    GfxPolygonSet *untexSet;
    GfxPresetPolygonSetTable *result;

    flags = GFX_D3D_SUPPORTED_STATE_MASK;
    // STRING: FF7 0x009107fc
    result = (GfxPresetPolygonSetTable *)GameHeapAlloc(1, 0xE4, "C:\\lib\\src\\graphics\\dx_rendi.cpp", 0x3A);
    if (result != 0) {
        Pfile_InitLoadOptions(&opts);
        pTexturedSet = 0;
        untexSet = 0;
        bitVal = 1;
        for (idx = 0; idx < 0x16; idx++) {
            if ((bitVal & flags) != 0) {
                texRecord = (PolygonHundredState *)Gfx_AllocPHundred();
                flatRecord = (PolygonHundredState *)Gfx_AllocPHundred();
                texRecord->flags = bitVal;
                texRecord->changeMask = bitVal;
                flatRecord->flags = 0;
                flatRecord->changeMask = bitVal;
                pTexturedSet = GfxLoadPolygonSet(0, (struct PolygonHundredState *)texRecord, 0, (struct GfxPolygonSetSpec *)&opts, context);
                untexSet = GfxLoadPolygonSet(0, (struct PolygonHundredState *)flatRecord, 0, (struct GfxPolygonSetSpec *)&opts, context);
                result->enabledSets[idx] = pTexturedSet;
                result->disabledSets[idx] = untexSet;
            }
            bitVal <<= 1;
        }
        optsAddress = (int)&untexOpts;
        Pfile_InitLoadOptions((PfileLoadOptions *)optsAddress);
        result->untextured0 = Gfx_AllocUntexturedHundred(0, optsAddress, (int)context);
        result->untextured1 = Gfx_AllocUntexturedHundred(1, optsAddress, (int)context);
    }
    return (int)result;
}

/*
 * Stub function returning zero.
 */
// FUNCTION: FF7 0x0068ad0a
int __cdecl Gfx_StubReturnZero(void)
{
    int val;

    val = 0;
    return val;
}

/*
 * Applies Direct3D render states including blend, alpha, and shading modes.
 */
// FUNCTION: FF7 0x0068ad1c
void __cdecl Gfx_SetRenderStateD3D(int state, int enable, struct GfxEngineContext *context)
{
    int boolValue;
    int statusBit;
    ComDirect3DDevice2 *renderDriver;
    GfxPolygonSet *drawGroup;
    int modeVal;
    GfxPresetPolygonSetTable *groupTable;

    statusBit = 1 << state;
    /* Unparenthesized OR-chain from an unparenthesized retail macro: `&` binds to
     * RENDER_STATE_BIT_WIREFRAME first, so the test is always nonzero. */
    if ((statusBit & RENDER_STATE_BIT_WIREFRAME | RENDER_STATE_BIT_LINEAR_FILTER | RENDER_STATE_BIT_PERSPECTIVE | RENDER_STATE_BIT_TEXTURE_BLEND | RENDER_STATE_BIT_WRAP_U | RENDER_STATE_BIT_WRAP_V | RENDER_STATE_BIT_COLOR_KEY | RENDER_STATE_BIT_DITHER | RENDER_STATE_BIT_ALPHA_BLEND | RENDER_STATE_BIT_ANTIALIAS | RENDER_STATE_BIT_CULL_CLOCKWISE | RENDER_STATE_BIT_NO_CULL | RENDER_STATE_BIT_DEPTH_TEST | RENDER_STATE_BIT_DEPTH_WRITE | RENDER_STATE_BIT_SPECULAR | RENDER_STATE_BIT_TEXTURE_WRAP) != 0) {
        renderDriver = (ComDirect3DDevice2 *)context->d3dDevice2;
        if (renderDriver != 0) {
            switch (state) {
            case RENDER_STATE_WIREFRAME:
                if (enable != 0) {
                    modeVal = D3DFILL_WIREFRAME;
                } else {
                    modeVal = D3DFILL_SOLID;
                }
                renderDriver->SetRenderState(D3DRENDERSTATE_FILLMODE, modeVal);
                break;
            case RENDER_STATE_LINEAR_FILTER:
                if (enable != 0) {
                    if (context != 0) {
                        if (context->modeSetupDone != 0) {
                            modeVal = D3DFILTER_NEAREST;
                        } else {
                            modeVal = D3DFILTER_LINEAR;
                        }
                    } else {
                        modeVal = D3DFILTER_NEAREST;
                    }
                } else {
                    modeVal = D3DFILTER_NEAREST;
                }
                renderDriver->SetRenderState(D3DRENDERSTATE_TEXTUREMAG, modeVal);
                renderDriver->SetRenderState(D3DRENDERSTATE_TEXTUREMIN, modeVal);
                break;
            case RENDER_STATE_PERSPECTIVE:
                if (enable != 0) {
                    boolValue = 1;
                } else {
                    boolValue = 0;
                }
                renderDriver->SetRenderState(D3DRENDERSTATE_TEXTUREPERSPECTIVE, boolValue);
                break;
            case RENDER_STATE_WRAP_U:
                if (enable != 0) {
                    renderDriver->SetRenderState(D3DRENDERSTATE_WRAPU, 1);
                } else {
                    renderDriver->SetRenderState(D3DRENDERSTATE_WRAPU, 0);
                }
                break;
            case RENDER_STATE_WRAP_V:
                if (enable != 0) {
                    renderDriver->SetRenderState(D3DRENDERSTATE_WRAPV, 1);
                } else {
                    renderDriver->SetRenderState(D3DRENDERSTATE_WRAPV, 0);
                }
                break;
            case RENDER_STATE_COLOR_KEY:
                if (enable != 0) {
                    boolValue = 1;
                } else {
                    boolValue = 0;
                }
                renderDriver->SetRenderState(D3DRENDERSTATE_COLORKEYENABLE, boolValue);
                break;
            case RENDER_STATE_DITHER:
                if (enable != 0) {
                    boolValue = 1;
                } else {
                    boolValue = 0;
                }
                renderDriver->SetRenderState(D3DRENDERSTATE_DITHERENABLE, boolValue);
                break;
            case RENDER_STATE_ALPHA_BLEND:
                if (enable != 0) {
                    boolValue = 1;
                } else {
                    boolValue = 0;
                    modeVal = D3DBLEND_ONE;
                    renderDriver->SetRenderState(D3DRENDERSTATE_SRCBLEND, modeVal);
                    modeVal = D3DBLEND_ZERO;
                    renderDriver->SetRenderState(D3DRENDERSTATE_DESTBLEND, modeVal);
                }
                if (context->alphaOption != 0) {
                    renderDriver->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, boolValue);
                } else {
                    renderDriver->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, boolValue);
                }
                break;
            case RENDER_STATE_ANTIALIAS:
                if (enable != 0) {
                    boolValue = 1;
                } else {
                    boolValue = 0;
                }
                renderDriver->SetRenderState(D3DRENDERSTATE_ANTIALIAS, boolValue);
                break;
            case RENDER_STATE_CULL_CLOCKWISE:
                if (enable != 0) {
                    modeVal = D3DCULL_CW;
                } else {
                    modeVal = D3DCULL_CCW;
                }
                renderDriver->SetRenderState(D3DRENDERSTATE_CULLMODE, modeVal);
                break;
            case RENDER_STATE_NO_CULL:
                if (enable != 0) {
                    modeVal = D3DCULL_NONE;
                    renderDriver->SetRenderState(D3DRENDERSTATE_CULLMODE, modeVal);
                } else {
                    modeVal = D3DCULL_CCW;
                    renderDriver->SetRenderState(D3DRENDERSTATE_CULLMODE, modeVal);
                }
                break;
            case RENDER_STATE_DEPTH_TEST:
                if (enable != 0) {
                    boolValue = 1;
                } else {
                    boolValue = 0;
                }
                renderDriver->SetRenderState(D3DRENDERSTATE_ZENABLE, boolValue);
                renderDriver->SetRenderState(D3DRENDERSTATE_ZFUNC, D3DCMP_LESSEQUAL);
                break;
            case RENDER_STATE_DEPTH_WRITE:
                if (enable != 0) {
                    boolValue = 1;
                } else {
                    boolValue = 0;
                }
                renderDriver->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, boolValue);
                break;
            case RENDER_STATE_SPECULAR:
                if (enable != 0) {
                    boolValue = 1;
                } else {
                    boolValue = 0;
                }
                renderDriver->SetRenderState(D3DRENDERSTATE_SPECULARENABLE, boolValue);
                break;
            case RENDER_STATE_TEXTURE_WRAP:
                if (enable != 0) {
                    renderDriver->SetRenderState(D3DRENDERSTATE_TEXTUREADDRESS, D3DTADDRESS_WRAP);
                } else {
                    renderDriver->SetRenderState(D3DRENDERSTATE_TEXTUREADDRESS, D3DTADDRESS_CLAMP);
                }
            }
        } else {
            groupTable = (GfxPresetPolygonSetTable *)context->presetPolygonSets;
            if (groupTable != 0) {
                if (enable != 0) {
                    drawGroup = groupTable->enabledSets[state];
                } else {
                    drawGroup = groupTable->disabledSets[state];
                }
                Gfx_DrawPolygonSetDispatch(drawGroup, context);
            }
        }
    }
}

/*
 * Configures Direct3D render states for untextured primitive drawing.
 */
// FUNCTION: FF7 0x0068b187
void __cdecl Gfx_ApplyStateUntexturedD3D(int polygon, struct GfxEngineContext *context)
{
    if (context->useDirect3D5 != 0) {
        Gfx_ApplyRenderStateCall(polygon, context);
    }
}

/*
 * Configures Direct3D render states for textured primitive drawing.
 */
// FUNCTION: FF7 0x0068b1a8
void __cdecl Gfx_ApplyStateTexturedD3D(struct GfxPolygonTexture *polygon, struct GfxEngineContext *context)
{
    GfxTextureSurface *pTexture;
    int hr;

    if (context->useDirect3D5 != 0 && polygon != 0) {
        pTexture = (GfxTextureSurface *)polygon->pageState;
        if (pTexture != 0 && pTexture->surface != 0) {
            hr = ((ComDirectDrawSurface *)(pTexture->surface))->IsLost();
            if (hr == DDERR_SURFACELOST) {
                polygon->pageState = (int)Gfx_RestoreTextureSurfaces(pTexture);
                if (polygon->pageState == 0) { /* empty first arm: lib/codegen.h */
                } else {
                    Gfx_ApplyRenderStateCall((int)polygon, context);
                }
            } else {
                Gfx_ApplyRenderStateCall((int)polygon, context);
            }
        }
    }
}

/*
 * Binds active texture surface and configures texture filtering modes.
 */
// FUNCTION: FF7 0x0068b239
void __cdecl Gfx_SelectTexturePage(int page, void *polygon)
{
    GfxDriverOps *gfxDriver;
    int slot;
    int stride;
    GfxTextureSurface *pageState;
    GfxEngineContext *engPtr;
    GfxPaletteRecord *texDesc;
    int surfaceInfo;
    int wrappedPage;
    GfxDriverOps *gfxObject;
    int pageNo;
    int texHandle;

    if (polygon != 0) {
        engPtr = GetEngineContext();
        pageState = (GfxTextureSurface *)(((GfxPolygonTexture *)polygon)->pageState);
        if (pageState != 0) {
            texDesc = (GfxPaletteRecord *)pageState->palette;
            if (texDesc != 0) {
                if (pageState->paletteRow != page) {
                    pageState->paletteRow = page;
                    gfxDriver = GetGraphicsDriverObject(engPtr);
                    slot = page;
                    if (slot >= texDesc->groupCount) {
                        slot = 0;
                        pageState->paletteRow = slot;
                    }
                    if (engPtr->fallbackFlag != 0) {
                        ((GfxTexHeader *)pageState->header)->row = slot;
                    }
                    stride = texDesc->entriesPerGroup;
                    gfxDriver->paletteChanged(slot * stride, stride, 0, texDesc, pageState);
                }
                if (pageState->paletteRow >= 0) {
                    if (texDesc->paletteMode != 0) {
                        if ((*(unsigned char *)((char *)texDesc->colorLookup + pageState->paletteRow) & 1) != 0) {
                            Gfx_SetRenderStateCall(RENDER_STATE_COLOR_KEY, 1, (struct GfxEngineContext *)engPtr);
                        } else {
                            Gfx_SetRenderStateCall(RENDER_STATE_COLOR_KEY, 0, (struct GfxEngineContext *)engPtr);
                        }
                    } else if (((GfxTexHeader *)pageState->header)->colorKey != 0) {
                        Gfx_SetRenderStateCall(RENDER_STATE_COLOR_KEY, 1, (struct GfxEngineContext *)engPtr);
                    } else {
                        Gfx_SetRenderStateCall(RENDER_STATE_COLOR_KEY, 0, (struct GfxEngineContext *)engPtr);
                    }
                }
            } else {
                if (pageState->paletteRow != page) {
                    pageState->paletteRow = page;
                    surfaceInfo = pageState->header;
                    gfxObject = GetGraphicsDriverObject(engPtr);
                    wrappedPage = page;
                    if (wrappedPage >= ((GfxTexHeader *)surfaceInfo)->paletteCount) {
                        wrappedPage = 0;
                        pageState->paletteRow = wrappedPage;
                    }
                    gfxObject->paletteChanged(0, 0, 0, texDesc, pageState);
                }
                if (DEAD_IN_RETAIL != 0) {
                    if (engPtr->useDirect3D5 != 0) {
                        pageState->paletteRow = page;
                        pageNo = page;
                        if (pageNo >= texDesc->groupCount) {
                            pageNo = 0;
                            pageState->paletteRow = pageNo;
                        }
                        if (((GfxPolygonTexture *)polygon)->handleEnabled != 0) {
                            texHandle = 0;
                            texHandle = *(int *)((char *)pageState->pageHandles + pageNo * 4);
                            if (engPtr->useDirect3D5 != 0) {
                                if (engPtr->d3dDevice2 != 0) {
                                    ((ComDirect3DDevice2 *)(engPtr->d3dDevice2))->SetRenderState(D3DRENDERSTATE_TEXTUREHANDLE, texHandle);
                                }
                            } else if (((GfxPolygonTexture *)polygon)->texHandleDest != 0) {
                                *(int *)((GfxPolygonTexture *)polygon)->texHandleDest = texHandle;
                            }
                        }
                    }
                }
            }
        }
    }
}

/*
 * Frees allocated auxiliary hundred-byte material state records.
 */
// FUNCTION: FF7 0x0068b4c7
void __cdecl Gfx_FreePresetHundreds(struct GfxEngineContext *context)
{
    int record;
    int i;

    for (i = 0; i < 5; i++) {
        record = context->presetHundreds[i];
        Gfx_FreePHundred(record);
        context->presetHundreds[i] = 0;
    }
}

/*
 * Initializes auxiliary hundred-byte material state records for preset table.
 */
// FUNCTION: FF7 0x0068b518
void __cdecl Gfx_BuildPresetHundreds(struct GfxEngineContext *context)
{
    PolygonHundredState *record;
    int index;

    for (index = 0; index < 5; index++) {
        record = (PolygonHundredState *)Gfx_AllocPHundred();
        if (record != 0) {
            record->changeMask = 0;
            record->flags = 0;
            Pfile_SetHundredBlendMode(1, index, record);
            if (index == BLEND_NONE) {
                record->changeMask |= GFX_STATE_FLAG_ALPHA_BLEND;
                record->flags &= ~GFX_STATE_FLAG_ALPHA_BLEND;
            }
            context->presetHundreds[index] = (int)record;
        }
    }
}

/*
 * Applies material, texture, and blend states from a preset index.
 */
// FUNCTION: FF7 0x0068b5a4
void __cdecl Gfx_ApplyPresetStateD3D(int preset, struct GfxEngineContext *context)
{
    int record;

    if (context->useDirect3D5 != 0 && preset < 5) {
        record = context->presetHundreds[preset];
        Gfx_ApplyRenderStateCall(record, context);
    }
}

/*
 * Fallback software path for submitting polygon set vertices and primitives.
 */
// FUNCTION: FF7 0x0068b5de
void __cdecl Gfx_SubmitPolygonSetFallback(GfxPolygonSet *drawGroup, GfxEngineContext *context)
{
    ComDirect3DDevice *slotFn;
    int matchFlag;
    GfxPolygonSetRecord *recordPtr;
    int matchOk;
    int polyIndex;
    GfxPolygonSetNode *pNode;
    GfxDrawStateNode *list;
    int callState;
    int modeUnit;
    int xformPointer;
    int drawMode;
    int device;
    int basePolySet;
    GlDrawGroup *primGroupPtr;
    GfxSubmitFallbackNode *pFallback;
    int extra;
    GfxTextureSurface *timTexturePtr;
    int append;
    int draw;
    int xform;
    GfxPolygonSetNode *newNodePtr;
    int passMode;
    int transformBlob;
    int val;

    val = 0;
    slotFn = (ComDirect3DDevice *)context->d3dDevice;
    device = context->viewport;
    if (slotFn != 0 && device != 0 && drawGroup != 0 && drawGroup->drawEnabled != 0) {
        recordPtr = (GfxPolygonSetRecord *)(drawGroup->matrices);
        pFallback = (GfxSubmitFallbackNode *)&drawGroup->drawListActive;
        matchFlag = 0;
        if (pFallback->drawListActive != 0) {
            matchFlag = 1;
            if (*(int *)pFallback->matchPtr == pFallback->matchValue) {
                matchOk = 1;
            } else {
                matchOk = 0;
            }
        }
        primGroupPtr = (GlDrawGroup *)drawGroup->groupState;
        extra = 0;
        if (drawGroup->source != 0) {
            extra = drawGroup->polygonData->rasterGroup;
        }
        for (polyIndex = 0; polyIndex < drawGroup->groupCount; polyIndex++) {
            append = 0;
            if (primGroupPtr != 0) {
                switch (context->scenePass) {
                case GFX_SCENE_PASS_OPAQUE:
                    draw = (primGroupPtr->flags & 0x410) == 0;
                    append = draw == 0;
                    draw = 1;
                    break;
                case GFX_SCENE_PASS_TRANSLUCENT:
                    draw = 1;
                    break;
                case GFX_SCENE_PASS_ALL_IMMEDIATE:
                    draw = 1;
                    break;
                default:
                    draw = 0;
                }
            } else {
                draw = 1;
            }
            if (draw != 0) {
                if (primGroupPtr != 0) {
                    timTexturePtr = (GfxTextureSurface *)(primGroupPtr->batch);
                    if (timTexturePtr != 0 && timTexturePtr->surface != 0) {
                        callState = ((ComDirectDrawSurface *)(timTexturePtr->surface))->IsLost();
                        if (callState == DDERR_SURFACELOST) {
                            primGroupPtr->batch = (int)Gfx_RestoreTextureSurfaces(timTexturePtr);
                            if (primGroupPtr->batch == 0) {
                                return;
                            }
                        }
                    }
                }
                if (matchFlag != 0) {
                    if (matchOk != 0) {
                        list = (GfxDrawStateNode *)pFallback->list;
                        if (list != 0) {
                            switch (list->kind) {
                            case GFX_DRAW_OP_BATCH:
                                do {
                                    if (append != 0) {
                                        pNode = (GfxPolygonSetNode *)(AppendBlockHeapNode(context->nodeList));
                                        if (pNode != 0) {
                                            pNode->polyIndex = polyIndex;
                                            pNode->drawGroup = (int)drawGroup;
                                            pNode->primGroupPtr = (int)primGroupPtr;
                                            switch (pFallback->kind) {
                                            case GFX_POLYGON_SET_TRANSFORM_NONE:
                                                pNode->inUse = 0;
                                                pNode->submitted = 0;
                                                break;
                                            case GFX_POLYGON_SET_TRANSFORM_NODE:
                                                pNode->inUse = 0;
                                                pNode->submitted = 1;
                                                pNode->payloadPtr = (int)list->matrix;
                                                break;
                                            case GFX_POLYGON_SET_TRANSFORM_NODE_CAMERA:
                                                pNode->inUse = 1;
                                                ConcatAffineMatrix4x4(list->matrix, (void *)context->cameraMatrix, pNode->matrix);
                                                ClearMatrixColumn4(pNode->matrix);
                                                pNode->submitted = 0;
                                                break;
                                            default:
                                                pNode->inUse = 0;
                                                pNode->submitted = 0;
                                            }
                                        }
                                    } else {
                                        switch (pFallback->kind) {
                                        case GFX_POLYGON_SET_TRANSFORM_NONE:
                                            xform = 0;
                                            break;
                                        case GFX_POLYGON_SET_TRANSFORM_NODE:
                                            xform = (int)list->matrix;
                                            break;
                                        case GFX_POLYGON_SET_TRANSFORM_NODE_CAMERA:
                                            ConcatAffineMatrix4x4(list->matrix, (void *)context->cameraMatrix, &g_PolygonSetIdentityMatrix);
                                            xform = (int)&g_PolygonSetIdentityMatrix;
                                            break;
                                        default:
                                            xform = 0;
                                        }
                                        if (xform != 0 && recordPtr != 0) {
                                            Gfx_SetMatrixCall(GFX_MATRIX_SLOT_WORLD, xform, (int)recordPtr, context);
                                        }
                                        basePolySet = (int)drawGroup->execBuffers[polyIndex];
                                        if (basePolySet != 0) {
                                            if (drawGroup->drawUnsorted != 0) {
                                                drawMode = D3DEXECUTE_CLIPPED;
                                            } else {
                                                drawMode = D3DEXECUTE_UNCLIPPED;
                                            }
                                            // STRING: FF7 0x00910820
                                            CheckDirectDrawResult(slotFn->Execute(basePolySet, device, drawMode), "C:\\lib\\src\\graphics\\dx_rendi.cpp", 0x3DB);
                                        }
                                    }
                                    list = (GfxDrawStateNode *)(list->next);
                                } while (list != 0);
                                break;
                            case GFX_DRAW_OP_DRAW_LIST:
                                do {
                                    Gfx_EmitPolygonSetPrimitive(0, (int)primGroupPtr, (struct GfxDrawListNode *)list->drawNode);
                                    switch (pFallback->kind) {
                                    case GFX_POLYGON_SET_TRANSFORM_NONE:
                                        xformPointer = 0;
                                        break;
                                    case GFX_POLYGON_SET_TRANSFORM_NODE:
                                        xformPointer = (int)list->matrix;
                                        break;
                                    case GFX_POLYGON_SET_TRANSFORM_NODE_CAMERA:
                                        ConcatAffineMatrix4x4(list->matrix, (void *)context->cameraMatrix, &g_PolygonSetIdentityMatrix);
                                        xformPointer = (int)&g_PolygonSetIdentityMatrix;
                                        break;
                                    default:
                                        xformPointer = 0;
                                    }
                                    if (xformPointer != 0 && recordPtr != 0) {
                                        Gfx_SetMatrixCall(GFX_MATRIX_SLOT_WORLD, xformPointer, (int)recordPtr, context);
                                    }
                                    basePolySet = (int)drawGroup->execBuffers[polyIndex];
                                    if (basePolySet != 0) {
                                        if (drawGroup->drawUnsorted != 0) {
                                            modeUnit = D3DEXECUTE_CLIPPED;
                                        } else {
                                            modeUnit = D3DEXECUTE_UNCLIPPED;
                                        }
                                        // STRING: FF7 0x00910844
                                        CheckDirectDrawResult(slotFn->Execute(basePolySet, device, modeUnit), "C:\\lib\\src\\graphics\\dx_rendi.cpp", 0x436);
                                    }
                                    list = (GfxDrawStateNode *)(list->next);
                                } while (list != 0);
                            }
                        }
                    }
                } else if (append != 0) {
                    newNodePtr = (GfxPolygonSetNode *)(AppendBlockHeapNode(context->nodeList));
                    if (newNodePtr != 0) {
                        newNodePtr->polyIndex = polyIndex;
                        newNodePtr->drawGroup = (int)drawGroup;
                        newNodePtr->primGroupPtr = (int)primGroupPtr;
                        transformBlob = GetCachedDeviceTransform(GFX_MATRIX_SLOT_WORLD, recordPtr);
                        if (transformBlob != 0) {
                            newNodePtr->inUse = 1;
                            *(GfxMatrix4x4 *)newNodePtr->matrix = *(GfxMatrix4x4 *)(transformBlob);
                            newNodePtr->submitted = 0;
                        } else {
                            newNodePtr->inUse = 0;
                            newNodePtr->submitted = 0;
                        }
                    }
                } else {
                    basePolySet = (int)drawGroup->execBuffers[polyIndex];
                    if (basePolySet != 0) {
                        if (drawGroup->drawUnsorted != 0) {
                            passMode = D3DEXECUTE_CLIPPED;
                        } else {
                            passMode = D3DEXECUTE_UNCLIPPED;
                        }
                        // STRING: FF7 0x00910868
                        CheckDirectDrawResult(slotFn->Execute(basePolySet, device, passMode), "C:\\lib\\src\\graphics\\dx_rendi.cpp", 0x4A3);
                    }
                }
            }
            if (primGroupPtr != 0) {
                primGroupPtr++;
            }
            if (extra != 0) {
                extra += 0x38;
            }
        }
    }
}

/*
 * Submits polygon set primitives to Direct3D device using DrawPrimitive.
 */
// FUNCTION: FF7 0x0068bbe3
void __cdecl D3D5SubmitPolygonSet(struct GfxPolygonSet *drawGroup, struct GfxEngineContext *context)
{
    int headerPresent;
    int shouldDraw;
    int count;
    int drawResult;
    int nodeMtx;
    int status;
    GfxRasterDrawGroup *localSpecPtr;
    GfxPolygonSetNode *auxLinkPtr;
    int callState;
    int tempMatrix;
    int deviceMatrix;
    int setPosition;
    GfxPolygonSetNode *pNode;
    GfxPolygonSetNode *deviceNode;
    int primResult;
    int msg[50];
    GfxD3DPolygonSetNode *setNodePtrPtr;
    GfxPolygonSetHeader *headerPointer;
    int curSurfaceStatus;
    GfxPolygonSetRecord *polyRecordPtr;
    GlDrawGroup *glGrp;
    int hdrValid;
    GfxTextureSurface *timTexturePtr;
    int tempDev;
    GfxPolygonBatch *batchPtrPtr;
    int sortFlag;
    GfxDrawListNode *drawListNode;
    int stillBusy;
    int wantQueue;
    int xformPointer;

    stillBusy = 0;
    tempDev = context->d3dDevice2;
    if (tempDev != 0 && drawGroup != 0 && drawGroup->drawEnabled != 0) {
        polyRecordPtr = (GfxPolygonSetRecord *)(drawGroup->matrices);
        headerPointer = (GfxPolygonSetHeader *)&drawGroup->drawListActive;
        headerPresent = 0;
        if (headerPointer->drawListActive != 0) {
            headerPresent = 1;
            if (*(int *)headerPointer->tagPtr == headerPointer->tag) {
                hdrValid = 1;
            } else {
                hdrValid = 0;
            }
        }
        glGrp = 0;
        if (drawGroup->hasGroupState != 0) {
            glGrp = (GlDrawGroup *)drawGroup->groupState;
        }
        localSpecPtr = 0;
        if (drawGroup->source != 0) {
            localSpecPtr = (GfxRasterDrawGroup *)drawGroup->polygonData->rasterGroup;
        }
        for (setPosition = 0; setPosition < drawGroup->groupCount; setPosition++) {
            wantQueue = 0;
            if (drawGroup->useGroupStates != 0) {
                glGrp = (GlDrawGroup *)((int *)(drawGroup->groupStates))[setPosition];
            }
            if (context->sortedDrawEnabled != 0 && glGrp != 0 && glGrp->polygonSet != 0) {
                sortFlag = 1;
            } else {
                sortFlag = 0;
            }
            if (glGrp != 0) {
                switch (context->scenePass) {
                case GFX_SCENE_PASS_OPAQUE:
                    shouldDraw = (glGrp->flags & 0x410) == 0;
                    if (sortFlag != 0) { /* empty first arm: lib/codegen.h */
                    } else {
                        wantQueue = shouldDraw == 0;
                    }
                    shouldDraw = 1;
                    break;
                case GFX_SCENE_PASS_TRANSLUCENT:
                    shouldDraw = 1;
                    break;
                case GFX_SCENE_PASS_ALL_IMMEDIATE:
                    shouldDraw = 1;
                    break;
                default:
                    shouldDraw = 0;
                }
            } else {
                shouldDraw = 1;
            }
            if (shouldDraw != 0) {
                if (glGrp != 0) {
                    timTexturePtr = (GfxTextureSurface *)(glGrp->batch);
                    if (timTexturePtr != 0 && timTexturePtr->surface != 0) {
                        curSurfaceStatus = ((ComDirectDrawSurface *)(timTexturePtr->surface))->IsLost();
                        if (curSurfaceStatus == DDERR_SURFACELOST) {
                            glGrp->batch = (int)Gfx_RestoreTextureSurfaces(timTexturePtr);
                            if (glGrp->batch == 0) {
                                break;
                            }
                        }
                    }
                }
                if (wantQueue != 0) { /* empty first arm: lib/codegen.h */
                } else {
                    Gfx_ApplyRenderStateCall((int)glGrp, context);
                }
                if (polyRecordPtr != 0) {
                    if (polyRecordPtr->viewEnabled != 0) {
                        ((ComDirect3DDevice2 *)(tempDev))->SetTransform((int)D3DTRANSFORMSTATE_VIEW, (int)polyRecordPtr->viewMatrixHandle);
                    }
                    if (polyRecordPtr->projectionEnabled != 0) {
                        ((ComDirect3DDevice2 *)(tempDev))->SetTransform((int)D3DTRANSFORMSTATE_PROJECTION, (int)polyRecordPtr->projectionMatrixHandle);
                    }
                }
                if (headerPresent != 0) {
                    if (hdrValid != 0) {
                        setNodePtrPtr = (GfxD3DPolygonSetNode *)(headerPointer->payload);
                        if (setNodePtrPtr != 0) {
                            switch (setNodePtrPtr->kind) {
                            case GFX_DRAW_OP_BATCH:
                                do {
                                    if (wantQueue != 0) {
                                        pNode = (GfxPolygonSetNode *)(AppendBlockHeapNode(context->nodeList));
                                        if (pNode != 0) {
                                            pNode->polyIndex = setPosition;
                                            pNode->drawGroup = (int)drawGroup;
                                            pNode->primGroupPtr = (int)glGrp;
                                            if (drawGroup->drawSpecApplied != 0) {
                                                pNode->stateBlock = *(GfxPolygonGroupDrawSpec *)(drawGroup->drawSpec);
                                            }
                                            switch (headerPointer->kind) {
                                            case GFX_POLYGON_SET_TRANSFORM_NONE:
                                                pNode->inUse = 0;
                                                pNode->submitted = 0;
                                                break;
                                            case GFX_POLYGON_SET_TRANSFORM_NODE:
                                                pNode->inUse = 0;
                                                pNode->submitted = 1;
                                                pNode->payloadPtr = (int)setNodePtrPtr->matrix;
                                                break;
                                            case GFX_POLYGON_SET_TRANSFORM_NODE_CAMERA:
                                                pNode->inUse = 1;
                                                ConcatAffineMatrix4x4(setNodePtrPtr->matrix, (void *)context->cameraMatrix, pNode->matrix);
                                                ClearMatrixColumn4(pNode->matrix);
                                                pNode->submitted = 0;
                                                break;
                                            default:
                                                pNode->inUse = 0;
                                                pNode->submitted = 0;
                                            }
                                        }
                                    } else {
                                        switch (headerPointer->kind) {
                                        case GFX_POLYGON_SET_TRANSFORM_NONE:
                                            tempMatrix = 0;
                                            break;
                                        case GFX_POLYGON_SET_TRANSFORM_NODE:
                                            tempMatrix = (int)setNodePtrPtr->matrix;
                                            break;
                                        case GFX_POLYGON_SET_TRANSFORM_NODE_CAMERA:
                                            ConcatAffineMatrix4x4(setNodePtrPtr->matrix, (void *)context->cameraMatrix, &g_PolygonSetIdentityMatrix);
                                            tempMatrix = (int)&g_PolygonSetIdentityMatrix;
                                            break;
                                        default:
                                            tempMatrix = 0;
                                        }
                                        if (drawGroup->batchList != 0) {
                                            batchPtrPtr = drawGroup->batchList[setPosition];
                                            if (batchPtrPtr != 0) {
                                                if (sortFlag != 0) {
                                                    Gfx_EnqueueGroupForDepthSort(tempMatrix, 0, batchPtrPtr, (int)drawGroup, (int)glGrp, (int)localSpecPtr, context);
                                                } else {
                                                    if (tempMatrix != 0 && polyRecordPtr != 0 && polyRecordPtr->enabled != 0) {
                                                        ((ComDirect3DDevice2 *)(tempDev))->SetTransform((int)D3DTRANSFORMSTATE_WORLD, (int)tempMatrix);
                                                    }
                                                    if (drawGroup->drawUnsorted == 0) {
                                                        batchPtrPtr->flags |= D3DDP_DONOTCLIP;
                                                    } else {
                                                        batchPtrPtr->flags &= ~D3DDP_DONOTCLIP;
                                                    }
                                                    do {
                                                        callState = ((ComDirect3DDevice2 *)(tempDev))->DrawIndexedPrimitive((int)batchPtrPtr->primType, (int)batchPtrPtr->vertexType, (int)batchPtrPtr->vertices, (int)batchPtrPtr->vertexCount, (int)batchPtrPtr->indices, (int)batchPtrPtr->indexCount, (int)batchPtrPtr->flags);
                                                        switch (callState) {
                                                        case DD_OK:
                                                            break;
                                                        case DDERR_WASSTILLDRAWING:
                                                            D3D5CountWasStillDrawing(1, context);
                                                            break;
                                                        default:
                                                            // STRING: FF7 0x0091088c
                                                            CheckDirectDrawResult(callState, "C:\\lib\\src\\graphics\\dx_rendi.cpp", 0x634);
                                                            callState = 0;
                                                        }
                                                    } while (callState != 0);
                                                }
                                            }
                                        }
                                    }
                                    setNodePtrPtr = (GfxD3DPolygonSetNode *)(setNodePtrPtr->next);
                                } while (setNodePtrPtr != 0);
                                break;
                            case GFX_DRAW_OP_BATCH_DRAW_SPEC:
                                do {
                                    if (wantQueue != 0) {
                                        auxLinkPtr = (GfxPolygonSetNode *)(AppendBlockHeapNode(context->nodeList));
                                        if (auxLinkPtr != 0) {
                                            auxLinkPtr->polyIndex = setPosition;
                                            auxLinkPtr->drawGroup = (int)drawGroup;
                                            auxLinkPtr->primGroupPtr = (int)glGrp;
                                            if (drawGroup->drawSpecApplied != 0) {
                                                auxLinkPtr->stateBlock = *(GfxPolygonGroupDrawSpec *)(drawGroup->drawSpec);
                                            }
                                            switch (headerPointer->kind) {
                                            case GFX_POLYGON_SET_TRANSFORM_NONE:
                                                auxLinkPtr->inUse = 0;
                                                auxLinkPtr->submitted = 0;
                                                break;
                                            case GFX_POLYGON_SET_TRANSFORM_NODE:
                                                auxLinkPtr->inUse = 0;
                                                auxLinkPtr->submitted = 1;
                                                auxLinkPtr->payloadPtr = (int)setNodePtrPtr->matrix;
                                                break;
                                            case GFX_POLYGON_SET_TRANSFORM_NODE_CAMERA:
                                                auxLinkPtr->inUse = 1;
                                                ConcatAffineMatrix4x4(setNodePtrPtr->matrix, (void *)context->cameraMatrix, auxLinkPtr->matrix);
                                                ClearMatrixColumn4(auxLinkPtr->matrix);
                                                auxLinkPtr->submitted = 0;
                                                break;
                                            default:
                                                auxLinkPtr->inUse = 0;
                                                auxLinkPtr->submitted = 0;
                                            }
                                        }
                                    } else {
                                        switch (headerPointer->kind) {
                                        case GFX_POLYGON_SET_TRANSFORM_NONE:
                                            nodeMtx = 0;
                                            break;
                                        case GFX_POLYGON_SET_TRANSFORM_NODE:
                                            nodeMtx = (int)setNodePtrPtr->matrix;
                                            break;
                                        case GFX_POLYGON_SET_TRANSFORM_NODE_CAMERA:
                                            ConcatAffineMatrix4x4(setNodePtrPtr->matrix, (void *)context->cameraMatrix, &g_PolygonSetIdentityMatrix);
                                            nodeMtx = (int)&g_PolygonSetIdentityMatrix;
                                            break;
                                        default:
                                            nodeMtx = 0;
                                        }
                                        if (drawGroup->batchList != 0) {
                                            batchPtrPtr = drawGroup->batchList[setPosition];
                                            if (batchPtrPtr != 0) {
                                                if (setNodePtrPtr->flagA != 0 || setNodePtrPtr->flagB != 0) {
                                                    Gfx_ApplyDrawSpecToGroup(setPosition, drawGroup, (GfxDrawSpec *)setNodePtrPtr->drawSpec);
                                                    DxMesh_TransformAndDrawGroup(nodeMtx, batchPtrPtr, drawGroup, glGrp, localSpecPtr, (struct GfxMeshDrawParams *)setNodePtrPtr->drawSpec, context);
                                                } else if (sortFlag != 0) {
                                                    Gfx_EnqueueGroupForDepthSort(nodeMtx, 0, batchPtrPtr, (int)drawGroup, (int)glGrp, (int)localSpecPtr, context);
                                                } else {
                                                    Gfx_ApplyDrawSpecToGroup(setPosition, drawGroup, (GfxDrawSpec *)setNodePtrPtr->drawSpec);
                                                    if (nodeMtx != 0 && polyRecordPtr != 0 && polyRecordPtr->enabled != 0) {
                                                        ((ComDirect3DDevice2 *)(tempDev))->SetTransform((int)D3DTRANSFORMSTATE_WORLD, (int)nodeMtx);
                                                    }
                                                    if (drawGroup->drawUnsorted == 0) {
                                                        batchPtrPtr->flags |= D3DDP_DONOTCLIP;
                                                    } else {
                                                        batchPtrPtr->flags &= ~D3DDP_DONOTCLIP;
                                                    }
                                                    do {
                                                        drawResult = ((ComDirect3DDevice2 *)(tempDev))->DrawIndexedPrimitive((int)batchPtrPtr->primType, (int)batchPtrPtr->vertexType, (int)batchPtrPtr->vertices, (int)batchPtrPtr->vertexCount, (int)batchPtrPtr->indices, (int)batchPtrPtr->indexCount, (int)batchPtrPtr->flags);
                                                        switch (drawResult) {
                                                        case DD_OK:
                                                            break;
                                                        case DDERR_WASSTILLDRAWING:
                                                            D3D5CountWasStillDrawing(1, context);
                                                            break;
                                                        default:
                                                            // STRING: FF7 0x009108b0
                                                            CheckDirectDrawResult(drawResult, "C:\\lib\\src\\graphics\\dx_rendi.cpp", 0x6F2);
                                                            drawResult = 0;
                                                        }
                                                    } while (drawResult != 0);
                                                }
                                            }
                                        }
                                    }
                                    setNodePtrPtr = (GfxD3DPolygonSetNode *)(setNodePtrPtr->next);
                                } while (setNodePtrPtr != 0);
                                break;
                            case GFX_DRAW_OP_DRAW_LIST:
                                if (drawGroup->batchList != 0) {
                                    batchPtrPtr = drawGroup->batchList[setPosition];
                                    if (batchPtrPtr != 0) {
                                        count = batchPtrPtr->vertices;
                                        if (drawGroup->drawUnsorted == 0) {
                                            batchPtrPtr->flags |= D3DDP_DONOTCLIP;
                                        } else {
                                            batchPtrPtr->flags &= ~D3DDP_DONOTCLIP;
                                        }
                                        do {
                                            switch (headerPointer->kind) {
                                            case GFX_POLYGON_SET_TRANSFORM_NONE:
                                                xformPointer = 0;
                                                break;
                                            case GFX_POLYGON_SET_TRANSFORM_NODE:
                                                xformPointer = (int)setNodePtrPtr->matrix;
                                                break;
                                            case GFX_POLYGON_SET_TRANSFORM_NODE_CAMERA:
                                                ConcatAffineMatrix4x4(setNodePtrPtr->matrix, (void *)context->cameraMatrix, &g_PolygonSetIdentityMatrix);
                                                xformPointer = (int)&g_PolygonSetIdentityMatrix;
                                                break;
                                            default:
                                                xformPointer = 0;
                                            }
                                            if (xformPointer != 0 && polyRecordPtr != 0 && polyRecordPtr->enabled != 0 && sortFlag == 0) {
                                                ((ComDirect3DDevice2 *)(tempDev))->SetTransform((int)D3DTRANSFORMSTATE_WORLD, (int)xformPointer);
                                            }
                                            drawListNode = (GfxDrawListNode *)(setNodePtrPtr->drawNode);
                                            if (drawListNode->kind == GFX_DRAW_SPRITE_BATCH) {
                                                count = drawListNode->vertexCount;
                                            } else {
                                                Gfx_EmitPolygonSetPrimitive(sortFlag, (int)glGrp, drawListNode);
                                            }
                                            if (sortFlag != 0) {
                                                Gfx_EnqueuePsxPrimitiveForDepthSort(xformPointer, count, (int)batchPtrPtr, (int)glGrp, drawListNode, context);
                                            } else {
                                                do {
                                                    primResult = ((ComDirect3DDevice2 *)(tempDev))->DrawIndexedPrimitive((int)batchPtrPtr->primType, (int)batchPtrPtr->vertexType, (int)count, (int)batchPtrPtr->vertexCount, (int)batchPtrPtr->indices, (int)batchPtrPtr->indexCount, (int)batchPtrPtr->flags);
                                                    switch (primResult) {
                                                    case DD_OK:
                                                        break;
                                                    case DDERR_WASSTILLDRAWING:
                                                        D3D5CountWasStillDrawing(1, context);
                                                        break;
                                                    default:
                                                        // STRING: FF7 0x009108d4
                                                        CheckDirectDrawResult(primResult, "C:\\lib\\src\\graphics\\dx_rendi.cpp", 0x783);
                                                        primResult = 0;
                                                    }
                                                } while (primResult != 0);
                                            }
                                            setNodePtrPtr = (GfxD3DPolygonSetNode *)(setNodePtrPtr->next);
                                        } while (setNodePtrPtr != 0);
                                    }
                                }
                            }
                        }
                    }
                } else if (wantQueue != 0) {
                    deviceNode = (GfxPolygonSetNode *)(AppendBlockHeapNode(context->nodeList));
                    if (deviceNode != 0) {
                        deviceNode->polyIndex = setPosition;
                        deviceNode->drawGroup = (int)drawGroup;
                        deviceNode->primGroupPtr = (int)glGrp;
                        if (drawGroup->drawSpecApplied != 0) {
                            deviceNode->stateBlock = *(GfxPolygonGroupDrawSpec *)(drawGroup->drawSpec);
                        }
                        deviceMatrix = GetCachedDeviceTransform(GFX_MATRIX_SLOT_WORLD, polyRecordPtr);
                        if (deviceMatrix != 0) {
                            deviceNode->inUse = 1;
                            *(GfxMatrix4x4 *)deviceNode->matrix = *(GfxMatrix4x4 *)(deviceMatrix);
                            deviceNode->submitted = 0;
                        } else {
                            deviceNode->inUse = 0;
                            deviceNode->submitted = 0;
                        }
                    }
                } else if (drawGroup->batchList != 0) {
                    batchPtrPtr = drawGroup->batchList[setPosition];
                    if (batchPtrPtr != 0) {
                        if (sortFlag != 0) {
                            Gfx_EnqueueGroupForDepthSort(polyRecordPtr->depthGroup, 0, batchPtrPtr, (int)drawGroup, (int)glGrp, (int)localSpecPtr, context);
                        } else {
                            if (polyRecordPtr != 0 && polyRecordPtr->enabled != 0) {
                                ((ComDirect3DDevice2 *)(tempDev))->SetTransform((int)D3DTRANSFORMSTATE_WORLD, (int)polyRecordPtr->depthGroup);
                            }
                            if (drawGroup->drawUnsorted == 0) {
                                batchPtrPtr->flags |= D3DDP_DONOTCLIP;
                            } else {
                                batchPtrPtr->flags &= ~D3DDP_DONOTCLIP;
                            }
                            do {
                                status = ((ComDirect3DDevice2 *)(tempDev))->DrawIndexedPrimitive((int)batchPtrPtr->primType, (int)batchPtrPtr->vertexType, (int)batchPtrPtr->vertices, (int)batchPtrPtr->vertexCount, (int)batchPtrPtr->indices, (int)batchPtrPtr->indexCount, (int)batchPtrPtr->flags);
                                switch (status) {
                                case DD_OK:
                                    break;
                                case DDERR_WASSTILLDRAWING:
                                    D3D5CountWasStillDrawing(1, context);
                                    break;
                                default:
                                    // STRING: FF7 0x009108f8
                                    CheckDirectDrawResult(status, "C:\\lib\\src\\graphics\\dx_rendi.cpp", 0x82C);
                                    status = 0;
                                }
                            } while (status != 0);
                        }
                    }
                }
            }
            if (glGrp != 0) {
                glGrp++;
            }
            if (localSpecPtr != 0) {
                localSpecPtr++;
            }
        }
    }
    if (DEAD_IN_RETAIL != 0) {
        // STRING: FF7 0x0091091c
        sprintf((char *)msg, "ASYNC COUNT %d \n", g_GfxStillDrawingCount);
        Gfx_DebugPrint((const char *)msg);
        g_GfxStillDrawingCount = 0;
    }
}

/*
 * Prepares states and submits polygon set primitives to Direct3D.
 */
// FUNCTION: FF7 0x0068c933
void __cdecl D3D5DrawPolygonSet(struct GfxPolygonSet *drawGroup, struct GfxEngineContext *context)
{
    if (drawGroup != 0 && context->sceneActive != 0) {
        if (context->useDirect3D5 != 0) {
            D3D5SubmitPolygonSet(drawGroup, context);
        } else {
            Gfx_SubmitPolygonSetFallback(drawGroup, context);
        }
    }
}

/*
 * Dispatches polygon set drawing to device-specific rendering routine.
 */
// FUNCTION: FF7 0x0068c978
void __cdecl Gfx_DrawPolygonSetDispatch(struct GfxPolygonSet *drawGroup, struct GfxEngineContext *context)
{
    if (drawGroup != 0) {
        if (context->useDirect3D5 != 0) {
            D3D5SubmitPolygonSet(drawGroup, context);
        } else {
            Gfx_SubmitPolygonSetFallback(drawGroup, context);
        }
    }
}

/*
 * Submits all polygon sets within a render object for drawing.
 */
// FUNCTION: FF7 0x0068c9b1
void __cdecl Gfx_DrawRenderObjectD3D(struct GfxRenderObject *renderObject, int context)
{
    if (renderObject != 0) {
        Gfx_DrawPolygonSetCall((PolygonSkeletonFrameRef *)renderObject->polygonSet, context);
    }
}

/*
 * Draws a polygon set associated with a mesh node.
 */
// FUNCTION: FF7 0x0068c9cf
void __cdecl Gfx_DrawMeshPolygonSet(int meshNode, int context)
{
    if (meshNode != 0) {
        Gfx_DrawPolygonSetCall((PolygonSkeletonFrameRef *)(*(int *)((char *)meshNode + 4)), context);
    }
}

/*
 * Iterates through linked polygon set list and submits each set.
 */
// FUNCTION: FF7 0x0068c9ed
void __cdecl Gfx_DrawPolygonSetList(GfxLightEntryList *list, int context)
{
    int i;

    if (list != 0) {
        for (i = 0; i < list->count; i++) {
            Gfx_DrawPolygonSetCall((PolygonSkeletonFrameRef *)(*(int *)((char *)*(int *)((char *)list->entries + i * 4) + 4)), context);
        }
    }
}

/*
 * Configures viewport, clear parameters, and pass state for scene pass.
 */
// FUNCTION: FF7 0x0068ca35
void __cdecl Gfx_SetScenePassD3D(int passMode, GfxEngineContext *context)
{
    if (context != 0 && context->sceneActive != 0) {
        context->scenePass = passMode;
        switch (passMode) {
        case GFX_SCENE_PASS_OPAQUE:
            context->opaqueSet->drawEnabled = 1;
            context->transSet->drawEnabled = 0;
            Gfx_DrawPolygonSetCall((PolygonSkeletonFrameRef *)(context->opaqueSet), (int)context);
            break;
        case GFX_SCENE_PASS_TRANSLUCENT:
            context->opaqueSet->drawEnabled = 0;
            context->transSet->drawEnabled = 1;
            Gfx_DrawPolygonSetCall((PolygonSkeletonFrameRef *)(context->transSet), (int)context);
        }
    }
}

/*
 * Begins Direct3D scene rendering and sets initial render states.
 */
// FUNCTION: FF7 0x0068cae1
int __cdecl Gfx_BeginSceneD3D(int passMode, GfxEngineContext *context)
{
    int result;
    ComDirect3DDevice2 *device;
    ComDirect3DDevice *ddObject;

    result = 0;
    if (context != 0 && context->sceneActive == 0) {
        if (context->useDirect3D5 != 0) {
            if (D3D5SetRenderTargetAndViewport(0, 1, GfxGetSurfaceByIndex(1), context->viewport2, context) != 0) {
                device = (ComDirect3DDevice2 *)context->d3dDevice2;
                if (device != 0) {
                    // STRING: FF7 0x00910930
                    if (CheckDirectDrawResult(device->BeginScene(), "C:\\lib\\src\\graphics\\dx_rendi.cpp", 0x8D3) != 0) {
                        result = 1;
                    }
                }
            }
        } else {
            ddObject = (ComDirect3DDevice *)context->d3dDevice;
            if (ddObject != 0) {
                // STRING: FF7 0x00910954
                if (CheckDirectDrawResult(ddObject->BeginScene(), "C:\\lib\\src\\graphics\\dx_rendi.cpp", 0x8DE) != 0) {
                    result = 1;
                }
            }
        }
        context->sceneActive = result;
        Gfx_SetScenePassCall(passMode, (struct GfxEngineContext *)context);
    }
    context->sceneActive = result;
    return result;
}

/*
 * Concludes Direct3D scene rendering.
 */
// FUNCTION: FF7 0x0068cbe6
void __cdecl Gfx_EndSceneD3D(GfxEngineContext *context)
{
    ComDirect3DDevice2 *device;
    ComDirect3DDevice *ddObject;

    if (context != 0 && context->sceneActive != 0) {
        if (context->useDirect3D5 != 0) {
            device = (ComDirect3DDevice2 *)context->d3dDevice2;
            if (device != 0) {
                // STRING: FF7 0x00910978
                if (CheckDirectDrawResult(device->EndScene(), "C:\\lib\\src\\graphics\\dx_rendi.cpp", 0x8F7) != 0) {
                    context->sceneActive = 0;
                }
            }
        } else {
            ddObject = (ComDirect3DDevice *)context->d3dDevice;
            if (ddObject != 0) {
                // STRING: FF7 0x0091099c
                if (CheckDirectDrawResult(ddObject->EndScene(), "C:\\lib\\src\\graphics\\dx_rendi.cpp", 0x901) != 0) {
                    context->sceneActive = 0;
                }
            }
        }
    }
}

/*
 * Submits execute buffer drawing commands for a scene graph node.
 */
// FUNCTION: FF7 0x0068cc9c
void __cdecl D3DExecuteBufferDrawNode(GfxD3DExecuteDrawNode *drawNode, struct GfxEngineContext *context)
{
    GfxD3DExecuteBufferNode *pBufferNode;
    int legacyObj;
    GlDrawGroup *record;
    int restoreFailed;
    int bufInfo;
    int bufferIdx;
    GfxTextureSurface *tex;
    int surfaceStatus;
    int mode;

    restoreFailed = 0;
    legacyObj = context->d3dDevice;
    pBufferNode = (GfxD3DExecuteBufferNode *)(drawNode->bufferNode);
    record = (GlDrawGroup *)(pBufferNode->records + drawNode->recordIndex * 0x64);
    if (drawNode->active != 0) {
        Gfx_SetMatrixCall(GFX_MATRIX_SLOT_WORLD, (int)drawNode + 0x14, pBufferNode->matrix, context);
    }
    if (drawNode[1].recordIndex != 0) {
        Gfx_SetMatrixCall(GFX_MATRIX_SLOT_WORLD, drawNode[1].bufferNode, pBufferNode->matrix, context);
    }
    bufInfo = 0;
    if (pBufferNode->buffers != 0) {
        bufInfo = *(int *)((char *)pBufferNode->buffers + 0x6C) + drawNode->recordIndex * 0x38;
    }
    if (record != 0) {
        tex = (GfxTextureSurface *)record->batch;
        if (tex != 0 && tex->surface != 0) {
            surfaceStatus = ((ComDirectDrawSurface *)(tex->surface))->IsLost();
            if (surfaceStatus == DDERR_SURFACELOST) {
                record->batch = (int)Gfx_RestoreTextureSurfaces(tex);
                if (record->batch == 0) {
                    restoreFailed = 1;
                }
            }
        }
    }
    if (restoreFailed == 0) {
        bufferIdx = *(int *)((char *)pBufferNode->indexTable + drawNode->recordIndex * 4);
        if (bufferIdx != 0) {
            if (pBufferNode->active != 0) {
                mode = 1;
            } else {
                mode = 2;
            }
            // STRING: FF7 0x009109c0
            CheckDirectDrawResult(((ComDirect3DDevice *)(legacyObj))->Execute(bufferIdx, context->viewport, mode), "C:\\lib\\src\\graphics\\dx_rendi.cpp", 0x96C);
        }
    }
}

/*
 * Draws batch primitive node using Direct3D DrawPrimitive API.
 */
// FUNCTION: FF7 0x0068ce0d
void __cdecl D3D5DrawBatchNode(SwDeferredDrawNode *batchNode, struct GfxEngineContext *context)
{
    GfxPolygonSet *group;
    int glGroup;
    int res;
    int deviceObject;
    GfxPolygonBatch *drawRecord;

    deviceObject = context->d3dDevice2;
    group = (GfxPolygonSet *)(batchNode->drawGroup);
    glGroup = batchNode->glGroup;
    if (group->batchList != 0) {
        drawRecord = group->batchList[batchNode->recordIndex];
        if (drawRecord != 0) {
            Gfx_ApplyRenderStateCall(glGroup, context);
            if (batchNode->mtx != 0) {
                ((ComDirect3DDevice2 *)(deviceObject))->SetTransform(D3DTRANSFORMSTATE_WORLD, (int)batchNode + 0x14);
            }
            if (batchNode[1].glGroup != 0) {
                ((ComDirect3DDevice2 *)(deviceObject))->SetTransform(D3DTRANSFORMSTATE_WORLD, batchNode[1].mtx);
            }
            if (group->drawUnsorted == 0) {
                drawRecord->flags |= D3DDP_DONOTCLIP;
            } else {
                drawRecord->flags &= ~D3DDP_DONOTCLIP;
            }
            do {
                res = ((ComDirect3DDevice2 *)(deviceObject))->DrawIndexedPrimitive(drawRecord->primType, drawRecord->vertexType, drawRecord->vertices, drawRecord->vertexCount, drawRecord->indices, drawRecord->indexCount, drawRecord->flags);
                switch (res) {
                case DD_OK:
                    break;
                case DDERR_WASSTILLDRAWING:
                    D3D5CountWasStillDrawing(1, context);
                    break;
                default:
                    // STRING: FF7 0x009109e4
                    CheckDirectDrawResult(res, "C:\\lib\\src\\graphics\\dx_rendi.cpp", 0x9CD);
                    res = 0;
                }
            } while (res != 0);
        }
    }
}
} /* extern "C" */
