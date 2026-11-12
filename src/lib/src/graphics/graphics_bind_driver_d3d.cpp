// src/lib/src/graphics/graphics_bind_driver_d3d.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Builds the operation table for the Direct3D graphics driver.
/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/types/graphics.h"

#include "lib/api/graphics.h"
extern "C" {

/*
 * Allocates and fills the Direct3D graphics operation table. context->useDirect3D5 selects
 * the Direct3D 5 batch allocation, compilation and node drawing over the execute-buffer
 * alternatives.
 */
// FUNCTION: FF7 0x006aa080
GfxDriverOps *__cdecl GfxBindDriverOpsD3D(GfxEngineContext *context)
{
    GfxDriverOps *ops;

    ops = GfxAllocDriverOps();
    if (ops != 0) {
        ops->init = (GfxDriverInitFn)GfxDriverInitStub;
        ops->cleanup = (GfxDriverCleanupFn)GfxDriverCleanupStub;
        ops->lockSurface = GfxLockSurface;
        ops->unlockSurface = GfxUnlockSurface;
        ops->clearViewport = D3DViewportClear;
        ops->clearAll = GfxClearColorAndDepth;
        ops->setViewport = D3DSetViewportRect;
        ops->setBackgroundColor = (GfxDriverSetBgColorFn)D3DSetBackgroundMaterial;
        ops->present = GfxPresentBackBuffer;
        if (context->useDirect3D5 != 0) {
            ops->allocBatchArray = (GfxDriverAllocBatchFn)D3D5AllocBatchArray;
            ops->compileGroup = (GfxDriverCompileGroupFn)D3D5CompileGroup;
        } else {
            ops->allocBatchArray = (GfxDriverAllocBatchFn)D3DExecAllocGroupArrays;
            ops->compileGroup = (GfxDriverCompileGroupFn)D3DExecCompileGroup;
        }
        ops->setMatrix = (GfxDriverSetMatrixFn)D3DSetMatrix;
        ops->unloadTexture = GfxReleaseTextureSurfaces;
        ops->loadTexture = (GfxDriverLoadTextureFn)GfxCreateTextureSurfaces;
        ops->paletteChanged = (GfxDriverPaletteChangedFn)D3DTexturePaletteChanged;
        ops->writePaletteEntries = (GfxDriverWritePaletteFn)Gfx_WritePaletteEntriesD3D;
        ops->getBlendMode = (GfxDriverGetBlendModeFn)Gfx_FindBlendModeRecordD3D;
        ops->lightPolygonSet = (GfxDriverLightPolygonSetFn)Gfx_LightPolygonSet;
        ops->setRenderState = Gfx_SetRenderStateD3D;
        ops->applyRenderState = (GfxDriverApplyStateFn)D3D5ApplyRenderState;
        ops->applyStateUntextured = Gfx_ApplyStateUntexturedD3D;
        ops->applyStateTextured = (GfxDriverApplyStateFn)Gfx_ApplyStateTexturedD3D;
        ops->applyPresetState = Gfx_ApplyPresetStateD3D;
        ops->drawPolygonSet = (GfxDriverDrawPolygonSetFn)D3D5DrawPolygonSet;
        if (context->useDirect3D5 != 0) {
            ops->drawBatchNode = (GfxDriverDrawBatchNodeFn)D3D5DrawBatchNode;
        } else {
            ops->drawBatchNode = (GfxDriverDrawBatchNodeFn)D3DExecuteBufferDrawNode;
        }
        ops->drawRenderObject = (GfxDriverDrawRenderObjectFn)Gfx_DrawRenderObjectD3D;
        ops->selectScenePass = Gfx_SetScenePassD3D;
        ops->beginScene = Gfx_BeginSceneD3D;
        ops->endScene = Gfx_EndSceneD3D;
        ops->draw2DRecord = (GfxDriverDraw2DRecordFn)Gfx_Draw2DRecordD3D;
        ops->begin2DUntextured = Gfx_EmitBegin2DUntextured;
        ops->emitBegin2DUntextured = Gfx_EmitBegin2DUntextured;
        ops->begin2DTexturedNoTex = Gfx_EmitBegin2DTexturedNoTex;
        ops->begin2DTexturedOpaque = Gfx_EmitBegin2DTexturedOpaque;
        ops->begin2DTexturedBlended = Gfx_EmitBegin2DTexturedBlended;
        ops->draw2DUntextured = (GfxBatchEmitterFn)Gfx_EmitDraw2DUntextured;
        ops->emitDraw2DUntextured = (GfxBatchEmitterFn)Gfx_EmitDraw2DUntextured;
        ops->emitDraw2DTextured = (GfxBatchEmitterFn)Gfx_EmitDraw2DUntextured;
        ops->draw2DTextured = (GfxBatchEmitterFn)Gfx_EmitDraw2DTextured;
        ops->begin3DUntextured = Gfx_EmitBegin3DUntextured;
        ops->emitBegin3DUntextured = Gfx_EmitBegin3DUntextured;
        ops->begin3DTexturedNoTex = Gfx_EmitBegin3DTexturedNoTex;
        ops->begin3DTexturedOpaque = Gfx_EmitBegin3DTexturedOpaque;
        ops->begin3DTexturedBlended = Gfx_EmitBegin3DTexturedBlended;
        ops->draw3DUntextured = (GfxBatchEmitterFn)Gfx_EmitDraw3DUntextured;
        ops->emitDraw3DUntextured = (GfxBatchEmitterFn)Gfx_EmitDraw3DUntextured;
        ops->emitDraw3DTextured = (GfxBatchEmitterFn)Gfx_EmitDraw3DUntextured;
        ops->draw3DTextured = (GfxBatchEmitterFn)Gfx_EmitDraw3DTextured;
        ops->beginLine = Gfx_EmitBeginLine;
        ops->beginLineGouraud = Gfx_EmitBeginLine;
        ops->drawLineFlat = (GfxBatchEmitterFn)Gfx_EmitDrawLine;
        ops->drawLineGouraud = (GfxBatchEmitterFn)Gfx_EmitDrawLine;
        ops->renderDeferredLists = Gfx_RenderDeferredListsD3D;
    }
    return ops;
}
} /* extern "C" */
