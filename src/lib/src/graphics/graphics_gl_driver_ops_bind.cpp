// src/lib/src/graphics/graphics_gl_driver_ops_bind.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Allocates the graphics driver operation table and fills its callback slots.
/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/types/graphics.h"

#include "lib/api/graphics.h"
extern "C" {

/*
 * Allocates a graphics driver operation table and, on success, fills its explicit
 * callback slots with OpenGL initialization/cleanup, surface/viewport/clear,
 * matrix/texture/palette, lighting/render-state/blend, scene/render-target and
 * geometry operations. Installs screen-space and model-space setup/draw families,
 * including shared duplicate entries and paged textured variants, plus line and
 * no-op shutdown callbacks.
 */
// FUNCTION: FF7 0x006a9b60
GfxDriverOps *__cdecl Gl_BindDriverOps(struct GfxEngineContext *unused)
{
    GfxDriverOps *ops;

    ops = GfxAllocDriverOps();
    if (ops != 0) {
        ops->init = Gl_OpsInit;
        ops->cleanup = Gl_OpsCleanup;
        ops->lockSurface = (GfxDriverSurfaceFn)Gl_OpsLockSurface;
        ops->unlockSurface = (GfxDriverSurfaceFn)Gl_OpsUnlockSurface;
        ops->clearViewport = Gl_OpsClear;
        ops->clearAll = (GfxDriverContextFn)Gl_OpsClearAll;
        ops->setViewport = Gl_OpsSetViewport;
        ops->setBackgroundColor = (GfxDriverSetBgColorFn)Gl_OpsSetBackgroundColor;
        ops->present = Gl_OpsFlip;
        ops->allocBatchArray = (GfxDriverAllocBatchFn)Gfx_RasterAllocBatchArray;
        ops->compileGroup = (GfxDriverCompileGroupFn)Gfx_RasterCompileGroup;
        ops->setMatrix = (GfxDriverSetMatrixFn)Gl_OpsSetMatrix;
        ops->unloadTexture = Gl_OpsUnloadTexture;
        ops->loadTexture = (GfxDriverLoadTextureFn)Gl_OpsLoadTexture;
        ops->paletteChanged = (GfxDriverPaletteChangedFn)Gl_OpsPaletteChanged;
        ops->writePaletteEntries = (GfxDriverWritePaletteFn)Gl_OpsUpdatePaletteEntries;
        ops->getBlendMode = (GfxDriverGetBlendModeFn)Gl_OpsGetBlendDescriptor;
        ops->lightPolygonSet = (GfxDriverLightPolygonSetFn)Blit_LightPolygonSet;
        ops->setRenderState = (GfxDriverSetRenderStateFn)Gl_OpsSetRenderStateByte;
        ops->applyRenderState = (GfxDriverApplyStateFn)Gl_ApplyRenderState;
        ops->applyStateUntextured = (GfxDriverApplyStateFn)Gl_OpsApplyRenderStateB;
        ops->applyStateTextured = (GfxDriverApplyStateFn)Gl_OpsApplyRenderStateC;
        ops->applyPresetState = Gl_OpsSetBlendMode;
        ops->drawPolygonSet = (GfxDriverDrawPolygonSetFn)Gl_OpsDrawPolygonSet;
        ops->drawBatchNode = (GfxDriverDrawBatchNodeFn)Gl_OpsDrawDeferredNode;
        ops->drawRenderObject = (GfxDriverDrawRenderObjectFn)Gl_OpsDrawPolygonSetIndirect;
        ops->selectScenePass = Gl_OpsSetScenePass;
        ops->beginScene = Gl_OpsBeginScene;
        ops->endScene = Gl_OpsEndScene;
        ops->draw2DRecord = (GfxDriverDraw2DRecordFn)Gl_OpsNoOp;
        ops->begin2DUntextured = (GfxBatchEmitterFn)Gl_EmitSetupScreenSpaceA;
        ops->emitBegin2DUntextured = (GfxBatchEmitterFn)Gl_EmitSetupScreenSpaceA;
        ops->begin2DTexturedNoTex = (GfxBatchEmitterFn)Gl_EmitSetupScreenSpaceB;
        ops->begin2DTexturedOpaque = (GfxBatchEmitterFn)Gl_EmitSetupScreenSpaceC;
        ops->begin2DTexturedBlended = (GfxBatchEmitterFn)Gl_EmitSetupScreenSpaceD;
        ops->draw2DUntextured = (GfxBatchEmitterFn)Gl_EmitTrisScreenSpace;
        ops->emitDraw2DUntextured = (GfxBatchEmitterFn)Gl_EmitTrisScreenSpace;
        ops->emitDraw2DTextured = (GfxBatchEmitterFn)Gl_EmitTrisScreenSpaceTextured;
        ops->draw2DTextured = (GfxBatchEmitterFn)Gl_EmitTrisScreenSpaceTexturedPaged;
        ops->begin3DUntextured = (GfxBatchEmitterFn)Gl_EmitSetupModelSpaceA;
        ops->emitBegin3DUntextured = (GfxBatchEmitterFn)Gl_EmitSetupModelSpaceA;
        ops->begin3DTexturedNoTex = (GfxBatchEmitterFn)Gl_EmitSetupModelSpaceB;
        ops->begin3DTexturedOpaque = (GfxBatchEmitterFn)Gl_EmitSetupModelSpaceC;
        ops->begin3DTexturedBlended = (GfxBatchEmitterFn)Gl_EmitSetupModelSpaceD;
        ops->draw3DUntextured = (GfxBatchEmitterFn)Gl_EmitTrisModelSpace;
        ops->emitDraw3DUntextured = (GfxBatchEmitterFn)Gl_EmitTrisModelSpace;
        ops->emitDraw3DTextured = (GfxBatchEmitterFn)Gl_EmitTrisModelSpaceTextured;
        ops->draw3DTextured = (GfxBatchEmitterFn)Gl_EmitTrisModelSpaceTexturedPaged;
        ops->beginLine = (GfxBatchEmitterFn)Gl_EmitSetupLinesA;
        ops->beginLineGouraud = (GfxBatchEmitterFn)Gl_EmitSetupLinesB;
        ops->drawLineFlat = (GfxBatchEmitterFn)Gl_EmitLinesFlat;
        ops->drawLineGouraud = (GfxBatchEmitterFn)Gl_EmitLinesGouraud;
        ops->renderDeferredLists = (GfxDriverContextFn)Gl_OpsShutdownNoOp;
    }
    return ops;
}
} /* extern "C" */
