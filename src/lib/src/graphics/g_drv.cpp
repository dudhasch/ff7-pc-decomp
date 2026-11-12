// src/lib/src/graphics/g_drv.cpp
// Original: C:\lib\src\graphics\g_drv.cpp
// Contents: Graphics driver dispatch: wrappers that look up the context's driver and call the
//   matching function-table entry for surface lock, present, clear, viewport, matrix, render state,
//   polygon-set drawing and scene begin and end.
/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/api/mem.h"
#include "lib/types/graphics.h"
#include "lib/types/polygon.h"
#include "lib/codegen.h"
#include <string.h>
#include <math.h>
#include "lib/api/polygon.h"
#include "lib/api/graphics.h"
#include "lib/enums/graphics.h"
extern "C" {

struct GfxEngineContext;
struct GfxMatrixArray;

#pragma intrinsic(strlen)

/*
 * Gets the engine context and its graphics driver; calls the driver's lockSurface slot with
 * surface and returns its result.
 */
// FUNCTION: FF7 0x00660540
int __cdecl GfxLockSurfaceCall(int surface)
{
    GfxEngineContext *context;
    GfxDriverOps *driverObject;

    context = GetEngineContext();
    driverObject = GetGraphicsDriverObject(context);
    return driverObject->lockSurface(surface);
}

/*
 * Gets the engine context and its graphics driver; calls the driver's unlockSurface slot with
 * surface.
 */
// FUNCTION: FF7 0x0066056e
void __cdecl GfxUnlockSurfaceCall(int surface)
{
    GfxEngineContext *context;
    GfxDriverOps *driverObject;

    context = GetEngineContext();
    driverObject = GetGraphicsDriverObject(context);
    driverObject->unlockSurface(surface);
}

/*
 * Gets the driver for context and calls its present slot with context only when presentEnable is
 * nonzero.
 */
// FUNCTION: FF7 0x0066059c
void __cdecl PresentFrame(GfxEngineContext *context)
{
    GfxDriverOps *driver;

    driver = GetGraphicsDriverObject(context);
    if (context->presentEnable != 0) {
        driver->present(context);
    }
}

/*
 * Returns 1.0f by default. If viewSizeX is positive, divides viewSizeY by it using double
 * arithmetic and narrows the result to float. Otherwise divides displayHeight by displayWidth when
 * displayWidth is positive. Context validity is assumed.
 */
// FUNCTION: FF7 0x006605cc
float __cdecl GetViewportAspectRatio(GfxEngineContext *viewport)
{
    float aspectRatio;
    aspectRatio = 1.0f;
    if (viewport->viewSizeX > 0) {
        aspectRatio = (double)viewport->viewSizeY / ((double)viewport->viewSizeX);
    } else if (viewport->displayWidth > 0) {
        aspectRatio = (double)viewport->displayHeight / ((double)viewport->displayWidth);
    }
    return aspectRatio;
}

/*
 * Clears the whole render target through the driver's clearAll slot. Module enters and
 * frames call it before drawing, and InitGraphicsDevice and ShutdownGraphicsDevice around a
 * present. On Direct3D the slot is GfxClearColorAndDepth, which clears colour and depth with
 * GfxClearCall(1, 1, 1). Neither the context nor the slot is checked for null.
 */
// FUNCTION: FF7 0x00660626
void __cdecl GfxClearAllCall(struct GfxEngineContext *context)
{
    GfxDriverOps *driver;

    driver = GetGraphicsDriverObject(context);
    driver->clearAll(context);
}

/*
 * Gets the driver for context and calls its clearViewport slot with all four arguments.
 */
// FUNCTION: FF7 0x0066064a
void __cdecl GfxClearCall(int clearTarget, int clearDepth, int clearValue, struct GfxEngineContext *context)
{
    GfxDriverOps *driver;

    driver = GetGraphicsDriverObject(context);
    driver->clearViewport(clearTarget, clearDepth, clearValue, context);
}

/*
 * Stores x, y, a, b and the inclusive endpoints x+a-1, y+b-1 in the context's clip and view-size
 * fields; calls the driver's setViewport slot with all five arguments, then SetPerspective with the
 * context's fov, planes and offsets and the view size converted to float.
 */
// FUNCTION: FF7 0x0066067a
void __cdecl GfxSetViewportRect(int x, int y, int width, int height, GfxEngineContext *ctx)
{
    GfxDriverOps *ops;
    ctx->clipLeft = x;
    ctx->clipTop = y;
    ctx->viewSizeX = width;
    ctx->viewSizeY = height;
    ctx->clipRight = x + width - 1;
    ctx->clipBottom = y + height - 1;
    ops = GetGraphicsDriverObject(ctx);
    ops->setViewport(x, y, width, height, ctx);
    SetPerspective(ctx->fov, ctx->nearPlane,
                   ctx->farPlane, ctx->offsetX,
                   ctx->offsetY,
                   (float)ctx->viewSizeX,
                   (float)ctx->viewSizeY, (GfxEngineContext *)ctx);
}

/*
 * Gets the driver for context, copies the 16-byte color to the driver's backgroundColor, then calls
 * the setBackgroundColor slot with both arguments.
 */
// FUNCTION: FF7 0x0066075c
void __cdecl GfxSetBackgroundColorCall(void *color, struct GfxEngineContext *context)
{
    GfxDriverOps *driver;

    driver = GetGraphicsDriverObject(context);
    *(Rgba *)driver->backgroundColor = *(Rgba *)color;
    driver->setBackgroundColor(color, context);
}

/*
 * Builds a polygon set and its per-group resources through the active driver.
 * Native mode 4 skips blend setup through an explicit empty true branch.
 */
// FUNCTION: FF7 0x006607a3
GfxPolygonSet *__cdecl GfxLoadPolygonSet(GfxMatrixArray *matrices, PolygonHundredState *groupState, PolygonFileArrays *lightModel, GfxPolygonSetSpec *spec, GfxEngineContext *context)
{
    int idx;
    int hasExtra;
    int successValue;
    GfxPolygonSet *polygonSet;
    int groupRecord;
    GfxDriverOps *driver;
    PolygonFileArrays *source;
    GfxPolygonSetEntry *ent;
    GfxPolygonGroupRecord *polyRecord;

    hasExtra = 0;
    successValue = 0;
    polygonSet = (GfxPolygonSet *)D3DAllocPolygonSet();
    if (polygonSet != 0) {
        driver = GetGraphicsDriverObject(context);
        polygonSet->source = (int)lightModel;
        polygonSet->renderFlags = spec->renderFlags;
        Gfx_InitPolygonSetStateRecord(&polygonSet->drawListActive);
        polygonSet->drawPool = context->drawPool;
        if (spec->drawPoolGeneration != 0) {
            polygonSet->drawPool = spec->drawPoolGeneration;
        }
        polygonSet->flags = spec->polygonFlags;
        if (groupState == 0 && lightModel != 0 && lightModel->groupCount > 0) {
            polygonSet->groupCount = lightModel->groupCount;
            groupRecord = lightModel->groups;
        } else {
            polygonSet->groupCount = 1;
            groupRecord = 0;
        }
        if (groupState == 0 && lightModel != 0 && lightModel->hundredCount > 0) {
            polygonSet->groupState = lightModel->hundreds;
        } else {
            polygonSet->groupState = (int)groupState;
        }
        polygonSet->matrices = (int)matrices;
        if ((spec->optionBits & 1) != 0 || (spec->flags & 0x2000000) != 0) {
            // STRING: FF7 0x0090aa2c
            polygonSet->entries = (GfxPolygonSetEntry *)GameHeapAlloc(polygonSet->groupCount, 36, "C:\\lib\\src\\graphics\\g_drv.cpp", 178);
            if (polygonSet->entries != 0) {
                polygonSet->lightGroup = spec->lightGroup;
            }
        }
        // STRING: FF7 0x0090aa4c
        polygonSet->groupStates = (int)GameHeapAlloc(polygonSet->groupCount, 4, "C:\\lib\\src\\graphics\\g_drv.cpp", 186);
        if (driver->allocBatchArray(polygonSet) != 0) {
            hasExtra = 0;
            groupState = (PolygonHundredState *)polygonSet->groupState;
            for (idx = 0; idx < polygonSet->groupCount; idx++) {
                if (groupState != 0) {
                    if (spec->blendMode == BLEND_NONE) { /* empty first arm: lib/codegen.h */
                    } else {
                        Pfile_SetHundredBlendMode(1, spec->blendMode, (struct PolygonHundredState *)groupState);
                    }
                    if ((groupState->flags & GFX_DRAW_GROUP_DEPTH_TEST) != 0) {
                        if (context->zBufferEnabled != 0) { /* empty first arm: lib/codegen.h */
                        } else {
                            groupState->flags &= 0xFFFF7FFF;
                        }
                    }
                    if ((spec->flags & 0x10000000) != 0) {
                        groupState->depthSort = 1;
                    }
                    if ((spec->flags & 0x20000000) != 0) {
                        groupState->sortPerTriangle = 1;
                        hasExtra = 1;
                    }
                }
                if (polygonSet->entries != 0) {
                    ent = &polygonSet->entries[idx];
                    source = (PolygonFileArrays *)polygonSet->source;
                    if (source != 0) {
                        if (source->groupCount > 1) {
                            polyRecord = (GfxPolygonGroupRecord *)(source->groups + idx * 0x38);
                            ent->vertexTable = source->vertices + polyRecord->firstVertex * 12;
                            if (source->sourceIndices != 0) {
                                ent->indexBase = source->sourceIndices + polyRecord->firstVertex * 4;
                            }
                        } else {
                            ent->vertexTable = source->vertices;
                            ent->indexBase = source->sourceIndices;
                        }
                    }
                    if (groupRecord != 0) {
                        ent->elementCount = ((GfxPolygonGroupRecord *)groupRecord)->elementCount;
                    } else if (lightModel != 0) {
                        ent->elementCount = lightModel->vertexCount;
                    }
                    if (groupRecord != 0 && lightModel != 0 && ((GfxPolygonGroupRecord *)groupRecord)->textureFlag != 0) {
                        ent->texture = groupState->texture;
                        ent->lightRecordPtr = lightModel->texCoords + ((GfxPolygonGroupRecord *)groupRecord)->lightIndex * 8;
                    }
                    if ((spec->flags & 0x2000000) != 0 && polygonSet->entries != 0) {
                        ent->lightRecords = Gfx_LightBuildModelVertexRecords((GfxLightRange *)groupRecord, lightModel);
                    }
                }
                if ((spec->flags & 0x8000000) != 0 && groupState != 0) {
                    groupState->shadeMode = D3DSHADE_GOURAUD;
                }
                if (lightModel != 0) {
                    if (driver->compileGroup(idx, matrices, groupState, groupRecord, lightModel, polygonSet, context) != 0) {
                        successValue = 1;
                    } else {
                        successValue = 0;
                        break;
                    }
                } else {
                    successValue = 1;
                }
                if (groupRecord != 0) {
                    groupRecord += 0x38;
                }
                if (groupState != 0) {
                    groupState++;
                }
            }
            if (hasExtra != 0 && lightModel != 0) {
                // STRING: FF7 0x0090aa6c
                polygonSet->elements = (int)GameHeapAlloc(lightModel->vertexCount, 20, "C:\\lib\\src\\graphics\\g_drv.cpp", 346);
            }
        }
    }
    if (successValue == 0) {
        D3DFreePolygonSet((struct GfxPolygonSet *)polygonSet);
        polygonSet = 0;
    }
    return polygonSet;
}
#pragma intrinsic(sqrt)
#pragma intrinsic(atan2)
#pragma intrinsic(cos, sin, tan, atan)

/*
 * Gets the driver for context and calls its setMatrix slot with all four arguments.
 */
// FUNCTION: FF7 0x00660be3
void __cdecl Gfx_SetMatrixCall(int matrix, int index, int mode, struct GfxEngineContext *context)
{
    GfxDriverOps *driver;

    driver = GetGraphicsDriverObject(context);
    driver->setMatrix(matrix, index, mode, context);
}

/*
 * Gets the driver for context and calls its renderDeferredLists slot with context.
 */
// FUNCTION: FF7 0x00660c13
void __cdecl Gfx_RenderDeferredListsCall(GfxEngineContext *context)
{
    GfxDriverOps *driver;

    driver = GetGraphicsDriverObject(context);
    driver->renderDeferredLists(context);
}

/*
 * Switches one render state on or off through the driver's setRenderState slot. `state`
 * is a RenderState code (RENDER_STATE_LINEAR_FILTER, RENDER_STATE_DEPTH_TEST, ...) and `value`
 * its setting; the game modules (battle, field, menu, world map, the minigames) and the PSX
 * drawing code here call it around their draws. On Direct3D the
 * slot is Gfx_SetRenderStateD3D, which maps the code to the device's SetRenderState calls (for
 * example the linear filter to TEXTUREMAG/TEXTUREMIN). Nothing is checked for null.
 */
// FUNCTION: FF7 0x00660c3a
void __cdecl Gfx_SetRenderStateCall(int state, int value, GfxEngineContext *context)
{
    GfxDriverOps *driver;

    driver = GetGraphicsDriverObject(context);
    driver->setRenderState(state, value, context);
}

/*
 * Gets the context driver and calls its applyStateUntextured slot with presetAArg of the
 * context's preset polygon sets and ctx.
 */
// FUNCTION: FF7 0x00660c66
void __cdecl Gfx_ApplyStateUntexturedPresetA(GfxEngineContext *ctx)
{
    GfxDriverOps *driver;

    driver = GetGraphicsDriverObject(ctx);
    driver->applyStateUntextured(((GfxTexturedRectState *)(ctx->presetPolygonSets))->presetAArg, ctx);
}

/*
 * Gets the context driver and calls its applyStateUntextured slot with presetBArg of the
 * context's preset polygon sets and ctx.
 */
// FUNCTION: FF7 0x00660c9a
void __cdecl Gfx_ApplyStateUntexturedPresetB(GfxEngineContext *ctx)
{
    GfxDriverOps *driver;

    driver = GetGraphicsDriverObject(ctx);
    driver->applyStateUntextured(((GfxTexturedRectState *)(ctx->presetPolygonSets))->presetBArg, ctx);
}

/*
 * Gets the driver for context and calls its applyStateUntextured slot with both arguments.
 */
// FUNCTION: FF7 0x00660cce
void __cdecl Gfx_ApplyStateUntexturedCall(void *state, GfxEngineContext *context)
{
    GfxDriverOps *driver;

    driver = GetGraphicsDriverObject(context);
    driver->applyStateUntextured((int)state, context);
}

/*
 * Gets the driver for context and calls its applyStateTextured slot with both arguments.
 */
// FUNCTION: FF7 0x00660cf6
void __cdecl Gfx_ApplyStateTexturedCall(void *state, void *context)
{
    GfxDriverOps *driver;

    driver = GetGraphicsDriverObject((GfxEngineContext *)context);
    driver->applyStateTextured((int)state, (GfxEngineContext *)context);
}

/*
 * Gets the driver for context and calls its applyPresetState slot with both arguments.
 */
// FUNCTION: FF7 0x00660d1e
void __cdecl Gfx_ApplyPresetStateCall(int preset, GfxEngineContext *context)
{
    GfxDriverOps *driver;

    driver = GetGraphicsDriverObject(context);
    driver->applyPresetState(preset, context);
}

/*
 * Gets the driver for context and calls its applyRenderState slot with both arguments.
 */
// FUNCTION: FF7 0x00660d46
void __cdecl Gfx_ApplyRenderStateCall(int renderState, struct GfxEngineContext *context)
{
    GfxDriverOps *driver;

    driver = GetGraphicsDriverObject(context);
    driver->applyRenderState(renderState, context);
}

/*
 * Gets the driver for context and calls its drawPolygonSet slot with both arguments.
 */
// FUNCTION: FF7 0x00660d6e
void __cdecl Gfx_DrawPolygonSetCall(PolygonSkeletonFrameRef *polySet, int context)
{
    GfxDriverOps *driver;

    driver = GetGraphicsDriverObject((GfxEngineContext *)context);
    driver->drawPolygonSet(polySet, (GfxEngineContext *)context);
}

/*
 * Traverses drawList linked entries when nonnull. For a group whose drawSpecApplied is nonzero,
 * optionally invokes the draw callback for the embedded and the pointed data when the spec's mode
 * equals one, then applies the draw spec. Calls the driver's drawBatchNode slot for every entry
 * regardless of group state.
 */
// FUNCTION: FF7 0x00660d96
void __cdecl GfxFlushDrawList(int drawList, GfxEngineContext *context)
{
    GfxDriverOps *driver;
    GfxPolygonSet *group;
    GfxDrawListNode *pNode;
    GfxDrawSpec *drawSpec;

    driver = GetGraphicsDriverObject(context);
    if (drawList != 0) {
        pNode = (GfxDrawListNode *)(((GfxBlockHeapNodeList *)drawList)->head);
        while (pNode != 0) {
            group = (GfxPolygonSet *)pNode->group;
            if (group->drawSpecApplied != 0) {
                drawSpec = (GfxDrawSpec *)((int)pNode + 0x5C);
                switch ((char)drawSpec->mode) {
                case GFX_DRAW_SPEC_MODE_NONE:
                    break;
                case GFX_DRAW_SPEC_MODE_CALLBACK:
                    if (pNode->drawEnabled != 0) {
                        /* Invokes untyped callback slot `drawSpec->callback` with its runtime (int) -> int signature. */
                        ((int(__cdecl *)(int)) drawSpec->callback)((int)pNode + 0x14);
                    }
                    if (pNode->vertex3UOrCallback != 0) {
                        /* Invokes untyped callback slot `drawSpec->callback` with its runtime (int) -> int signature. */
                        ((int(__cdecl *)(int)) drawSpec->callback)(pNode->callbackArg);
                    }
                }
                Gfx_ApplyDrawSpecToGroup(pNode->kind, group, drawSpec);
            }
            driver->drawBatchNode(pNode, context);
            pNode = (GfxDrawListNode *)(pNode->nextNode);
        }
    }
}

/*
 * Draws a whole render object through the driver's drawRenderObject slot. On Direct3D the
 * slot is Gfx_DrawRenderObjectD3D, which draws the object's polygon set (+0x14) with
 * Gfx_DrawPolygonSetCall and ignores a null object. Game modules call it after
 * Gfx_CreateRenderObject built the object and its vertices were written. The context is not
 * checked for null.
 */
// FUNCTION: FF7 0x00660e6a
void __cdecl Gfx_DrawRenderObjectCall(GfxRenderObject *renderObject, GfxEngineContext *context)
{
    GfxDriverOps *driver;

    driver = GetGraphicsDriverObject(context);
    driver->drawRenderObject(renderObject, context);
}

/*
 * Selects the scene pass (a GfxScenePass) through the driver's selectScenePass slot. A module
 * frame draws its opaque work in GFX_SCENE_PASS_OPAQUE, which Gfx_BeginSceneCall selects, and
 * switches to GFX_SCENE_PASS_TRANSLUCENT before the translucent work and GfxFlushFrameDrawList. On
 * Direct3D the slot is Gfx_SetScenePassD3D: inside a scene, the opaque pass flags and draws the
 * context's opaque preset polygon set and the translucent pass the translucent one; outside a
 * scene it does nothing. The context is not checked for null.
 */
// FUNCTION: FF7 0x00660e95
void __cdecl Gfx_SetScenePassCall(int pass, GfxEngineContext *context)
{
    GfxDriverOps *driver;

    driver = GetGraphicsDriverObject(context);
    driver->selectScenePass(pass, context);
}

/*
 * Opens the frame's scene through the driver's beginScene slot, with `scene` as the first
 * scene pass; module frames call it with 0 after resetting their pools. On Direct3D the slot is
 * Gfx_BeginSceneD3D: unless a scene is already open it sets the render target and viewport on the
 * Direct3D 5 path, calls BeginScene, records whether that succeeded and selects the pass. The
 * slot's result is dropped. The context is not checked for null.
 */
// FUNCTION: FF7 0x00660ec0
void __cdecl Gfx_BeginSceneCall(int scene, struct GfxEngineContext *context)
{
    GfxDriverOps *driver;

    driver = GetGraphicsDriverObject(context);
    driver->beginScene(scene, context);
}

/*
 * Closes the frame's scene through the driver's endScene slot, at the end of a module
 * frame before WinMain presents. On Direct3D the slot is Gfx_EndSceneD3D, which calls EndScene on
 * the Direct3D 5 or the execute-buffer device when a scene is open and marks it closed on
 * success. The context is not checked for null.
 */
// FUNCTION: FF7 0x00660eeb
void __cdecl Gfx_EndSceneCall(struct GfxEngineContext *context)
{
    GfxDriverOps *driver;

    driver = GetGraphicsDriverObject(context);
    driver->endScene(context);
}

/*
 * Obtains current engine context and graphics driver and forwards all six arguments to the driver's
 * writePaletteEntries slot. Does not check context, driver or callback.
 */
// FUNCTION: FF7 0x00660f12
int __cdecl Gfx_WritePaletteEntriesCall(int startIndex, int count, int entries, int flags, int palette, int extra)
{
    GfxEngineContext *context;
    GfxDriverOps *driverObject;

    context = GetEngineContext();
    driverObject = GetGraphicsDriverObject(context);
    return driverObject->writePaletteEntries(startIndex, count, entries, flags, palette, extra);
}

/*
 * Obtains current graphics driver, returns zero when the target's palette is null, otherwise computes
 * index times the palette's entriesPerGroup and calls the paletteChanged slot with zero, that count,
 * the offset, palette, target. Returns its result.
 */
// FUNCTION: FF7 0x00660f54
int __cdecl Gfx_PaletteChangedCall(int index, int target)
{
    GfxPaletteRecord *palObj;
    int ofs;
    int ret;
    GfxEngineContext *context;
    GfxDriverOps *driverObject;

    context = GetEngineContext();
    driverObject = GetGraphicsDriverObject(context);
    ret = 0;
    palObj = (GfxPaletteRecord *)((GfxTextureSurface *)target)->palette;
    if (palObj != 0) {
        ofs = index * palObj->entriesPerGroup;
        ret = driverObject->paletteChanged(0, palObj->entriesPerGroup, ofs, palObj, (GfxTextureSurface *)target);
    }
    return ret;
}

/*
 * Gets the engine context and its driver, then calls the lightPolygonSet slot with both
 * arguments.
 */
// FUNCTION: FF7 0x00660fbf
void __cdecl Gfx_LightPolygonSetCall(PolygonSkeletonFrameRef *light, GfxLightOwner *owner)
{
    GfxEngineContext *context;
    GfxDriverOps *driverObject;

    context = GetEngineContext();
    driverObject = GetGraphicsDriverObject(context);
    driverObject->lightPolygonSet(light, owner);
}
} /* extern "C" */
