// src/lib/src/graphics/graphics_bind_driver_ddraw.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Builds the operation table for the DirectDraw and software blit graphics driver,
//   filling its explicit slots.
/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/types/graphics.h"

#include "lib/api/graphics.h"
extern "C" {
/*
 * Allocates a graphics operation table and fills it for the DirectDraw/software blit driver:
 * surface locking, clear/present/viewport/background, matrix, texture and palette, lighting,
 * render state and blend, scene handlers, the 2D and 3D begin/draw families and the line emitters.
 */
// FUNCTION: FF7 0x006a9df0
GfxDriverOps *__cdecl GfxBindDriverOpsDDrawBlit(struct GfxEngineContext *unused)
{
    GfxDriverOps *ops;

    ops = GfxAllocDriverOps();
    if (ops != 0) {
        ops->init = Blit_OpsInitRenderState;
        ops->cleanup = Blit_OpsDestroyRenderState;
        ops->lockSurface = GfxLockSurface;
        ops->unlockSurface = GfxUnlockSurface;
        ops->clearViewport = Blit_OpsClearTargetRect;
        ops->clearAll = (GfxDriverContextFn)Gfx_RasterClearTargetAllFlags;
        ops->setViewport = Gfx_RasterRefreshClipRects;
        ops->setBackgroundColor = Blit_OpsSetBackgroundColor;
        ops->present = Gfx_BlitContextRectsAndPresent;
        ops->allocBatchArray = (GfxDriverAllocBatchFn)Gfx_RasterAllocBatchArray;
        ops->compileGroup = (GfxDriverCompileGroupFn)Gfx_RasterCompileGroup;
        ops->setMatrix = (GfxDriverSetMatrixFn)Gfx_RasterSetMatrixSlot;
        ops->unloadTexture = (GfxDriverUnloadTextureFn)Gfx_RasterUnloadTextureNoOp;
        ops->loadTexture = (GfxDriverLoadTextureFn)Gfx_RasterPrepareGroupRecord;
        ops->paletteChanged = (GfxDriverPaletteChangedFn)Blit_OpsPaletteChangedStub;
        ops->writePaletteEntries = (GfxDriverWritePaletteFn)Blit_WritePaletteEntries;
        ops->getBlendMode = (GfxDriverGetBlendModeFn)Blit_GetBlendModeRecord;
        ops->lightPolygonSet = (GfxDriverLightPolygonSetFn)Blit_LightPolygonSet;
        ops->setRenderState = Blit_SetRenderState;
        ops->applyRenderState = (GfxDriverApplyStateFn)Blit_ApplyRenderSpec;
        ops->applyStateUntextured = (GfxDriverApplyStateFn)Blit_ApplyStateUntextured;
        ops->applyStateTextured = (GfxDriverApplyStateFn)Blit_ApplyStateTextured;
        ops->applyPresetState = Blit_ApplyPresetState;
        ops->drawPolygonSet = (GfxDriverDrawPolygonSetFn)Blit_DrawPolygonSet;
        ops->drawBatchNode = (GfxDriverDrawBatchNodeFn)Blit_DrawDeferredNode;
        ops->drawRenderObject = (GfxDriverDrawRenderObjectFn)Blit_DrawRenderObject;
        ops->selectScenePass = Blit_OpsSetScenePass;
        ops->beginScene = Blit_OpsBeginScene;
        ops->endScene = Blit_OpsEndScene;
        ops->draw2DRecord = (GfxDriverDraw2DRecordFn)Blit_Draw2DRecordStub;
        ops->begin2DUntextured = (GfxBatchEmitterFn)Blit_EmitBegin2DUntextured;
        ops->emitBegin2DUntextured = (GfxBatchEmitterFn)Blit_EmitBegin2DUntextured;
        ops->begin2DTexturedNoTex = (GfxBatchEmitterFn)Blit_EmitBegin2DTexturedNoTex;
        ops->begin2DTexturedOpaque = (GfxBatchEmitterFn)Blit_EmitBegin2DTexturedOpaque;
        ops->begin2DTexturedBlended = (GfxBatchEmitterFn)Blit_EmitBegin2DTexturedBlended;
        ops->draw2DUntextured = (GfxBatchEmitterFn)Blit_EmitDraw2DUntextured;
        ops->emitDraw2DUntextured = (GfxBatchEmitterFn)Blit_EmitDraw2DUntextured;
        ops->emitDraw2DTextured = (GfxBatchEmitterFn)Blit_EmitDraw2DTexturedNoTex;
        ops->draw2DTextured = (GfxBatchEmitterFn)Blit_EmitDraw2DTextured;
        ops->begin3DUntextured = (GfxBatchEmitterFn)Blit_EmitBegin3DUntextured;
        ops->emitBegin3DUntextured = (GfxBatchEmitterFn)Blit_EmitBegin3DUntextured;
        ops->begin3DTexturedNoTex = (GfxBatchEmitterFn)Blit_EmitBegin3DTexturedNoTex;
        ops->begin3DTexturedOpaque = (GfxBatchEmitterFn)Blit_EmitBegin3DTexturedOpaque;
        ops->begin3DTexturedBlended = (GfxBatchEmitterFn)Blit_EmitBegin3DTexturedBlended;
        ops->draw3DUntextured = (GfxBatchEmitterFn)Blit_EmitDraw3DUntextured;
        ops->emitDraw3DUntextured = (GfxBatchEmitterFn)Blit_EmitDraw3DUntextured;
        ops->emitDraw3DTextured = (GfxBatchEmitterFn)Blit_EmitDraw3DTexturedNoTex;
        ops->draw3DTextured = (GfxBatchEmitterFn)Blit_EmitDraw3DTextured;
        ops->beginLine = (GfxBatchEmitterFn)Blit_EmitBeginLineFlat;
        ops->beginLineGouraud = (GfxBatchEmitterFn)Blit_EmitBeginLineGouraud;
        ops->drawLineFlat = (GfxBatchEmitterFn)Blit_EmitDrawLineFlat;
        ops->drawLineGouraud = (GfxBatchEmitterFn)Blit_EmitDrawLineGouraud;
        ops->renderDeferredLists = (GfxDriverContextFn)Blit_RenderDeferredListsStub;
    }
    return ops;
}
} /* extern "C" */
