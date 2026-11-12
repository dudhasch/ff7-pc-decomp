// src/lib/src/graphics/graphics_driver_context_fields.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Initializes the graphics driver descriptor and reads or writes graphics context fields:
//   render state, driver object, log verbosity, context slots, rectangle and frame counters. It
//   also commits and requests module switches.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/types/graphics.h"
#include "lib/enums/graphics.h"
#include "lib/api/mem.h"
#include "lib/api/graphics.h"
extern "C" {

// The engine services the DLL graphics driver calls (heap, texture formats, pixel conversion, palettes,
// lighting) are named by the export table below; Blit_MapRgb24ViaRampTables, which no header declares,
// is declared without a prototype: only its address is taken.
unsigned int __cdecl Blit_MapRgb24ViaRampTables();
// Export callback table directx.cpp stores in context->driverExportTable for external DLL
// graphics drivers, placed at the start of this TU's .data section.
// GLOBAL: FF7 0x0090c930
GfxDllExportFn g_GfxDllDriverExportTable[36] = {
    (GfxDllExportFn)GameHeapFree, (GfxDllExportFn)GameHeapAllocRaw, (GfxDllExportFn)GameHeapAlloc,
    (GfxDllExportFn)AllocTextureFormatRecord, (GfxDllExportFn)InsertTextureFormatRecord,
    (GfxDllExportFn)GetEngineContext, (GfxDllExportFn)GfxFreeDriverOps, (GfxDllExportFn)GfxAllocDriverOps,
    (GfxDllExportFn)PixelFormatSetRgbMasks, (GfxDllExportFn)PixelFormatLoadMaskTemplate,
    (GfxDllExportFn)PackRgbToContextPixelFormat, (GfxDllExportFn)PackArgb8888ToPixelFormat,
    (GfxDllExportFn)Blit_MapRgb24ViaRampTables, (GfxDllExportFn)Blit_MapArgb8888ViaRampTables,
    (GfxDllExportFn)UnpackPixelToArgb8888, (GfxDllExportFn)UnpackPixelRowToArgb8888,
    (GfxDllExportFn)Gfx_ReleaseTexturePalette, (GfxDllExportFn)Gfx_CreateSurfacePaletteRecord,
    (GfxDllExportFn)Gfx_ConvertTexturePixelsToSurface, (GfxDllExportFn)Gfx_ReleaseTextureRecordRef,
    (GfxDllExportFn)Gfx_AllocSurfaceRecord, (GfxDllExportFn)Gfx_CopyPaletteEntriesD3D,
    (GfxDllExportFn)Gfx_ConvertPaletteEntriesToFloat, (GfxDllExportFn)Gfx_MapPaletteEntriesViaRampTables,
    (GfxDllExportFn)Gfx_GatherPaletteEntriesByIndex, (GfxDllExportFn)Gfx_FreePaletteRecord,
    (GfxDllExportFn)Gfx_CreatePaletteRecord, (GfxDllExportFn)Gfx_WritePaletteEntriesCall,
    (GfxDllExportFn)Gfx_PaletteChangedCall, (GfxDllExportFn)Gfx_FillColorRowBytes,
    (GfxDllExportFn)Gfx_ScaleFloatColorRowToBytes, (GfxDllExportFn)Gfx_BlendPaletteRowToBgr,
    (GfxDllExportFn)Gfx_ModulatePaletteRowToBytes, (GfxDllExportFn)AppendBlockHeapNode,
    (GfxDllExportFn)D3D5CompileGroup, (GfxDllExportFn)Gfx_LightPolygonSet};
__declspec(dllimport) int __stdcall LoadLibraryA(const char *);
__declspec(dllimport) int __stdcall GetProcAddress(int, const char *);

/*
 * Zero-fills the 0x24-byte driver descriptor, stores driverType in it and sets dllName to the
 * new_dll_graphics_driver literal.
 */
// FUNCTION: FF7 0x00666ab0
void __cdecl InitGraphicsDriverDesc(int driverType, GfxDriverDesc *driverDesc)
{
    ZeroFillMemory32(0x24, driverDesc);
    driverDesc->driverType = driverType;
    // STRING: FF7 0x0090c9c0
    driverDesc->dllName = (int)"new_dll_graphics_driver";
}

/*
 * For nonnull context copies the driver type and 36-byte blob. Type two loads the named library and
 * export, stores loaded flag/handle/procedure and returns one only when both succeed. Other types
 * return one after copying. Returns zero for null context or failure; failed export lookup does
 * not unload its successful library here.
 */
// FUNCTION: FF7 0x00666ad5
int __cdecl InstallGraphicsDriver(GfxDriverLoadSpec *driverBlob, GfxEngineContext *context)
{
    int result;
    int library;
    int procedure;

    result = 0;
    if (context != 0) {
        context->driverType = driverBlob->driverType;
        *(GfxDriverLoadSpec *)((char *)context + 0xA68) = *driverBlob;
        if (driverBlob->driverType == GFX_DRIVER_DLL) {
            library = LoadLibraryA((const char *)driverBlob->libraryName);
            if (library != 0) {
                procedure = GetProcAddress(library, (const char *)driverBlob->procedureName);
                if (procedure != 0) {
                    context->driverInstalled = 1;
                    context->driverLibrary = library;
                    context->driverProcedure = procedure;
                    result = 1;
                }
            }
        } else {
            result = 1;
        }
    }
    return result;
}

/*
 * Stores renderState in the context's rasterBase.
 */
// FUNCTION: FF7 0x00666b85
void __cdecl SetRenderStateObject(int renderState, GfxEngineContext *context)
{
    context->rasterBase = (SwRasterizerState *)renderState;
}

/*
 * Returns the context's rasterBase.
 */
// FUNCTION: FF7 0x00666b96
int __cdecl GetRenderStateObject(void *context)
{
    int state;

    state = (int)((GfxEngineContext *)context)->rasterBase;
    return state;
}

/*
 * Stores driverObject in the context's driverObject.
 */
// FUNCTION: FF7 0x00666bad
void __cdecl SetGraphicsDriverObject(GfxDriverOps *driverObject, struct GfxEngineContext *context)
{
    context->driverObject = driverObject;
}

/*
 * Returns the context's driverObject.
 */
// FUNCTION: FF7 0x00666bbe
GfxDriverOps *__cdecl GetGraphicsDriverObject(GfxEngineContext *context)
{
    return context->driverObject;
}

/*
 * Returns the context's activeB log verbosity, or zero when context is null.
 */
// FUNCTION: FF7 0x00666bcc
int __cdecl GetLogVerbosity(GfxEngineContext *context)
{
    int verbosity;

    verbosity = 0;
    if (context != 0) {
        verbosity = context->activeB;
    }
    return verbosity;
}

/*
 * For nonnull context, stores verbosity in its activeB and activeA.
 */
// FUNCTION: FF7 0x00666bf0
void __cdecl SetLogVerbosity(int verbosity, GfxEngineContext *context)
{
    if (context != 0) {
        context->activeB = verbosity;
        context->activeA = verbosity;
    }
}

// If context is nonnull, copies the pending module's five callbacks (enter, exit, frame, onMouse
// and onKey) into the active module. This promotes the pending module record to the active one; the
// active record's init and shutdown stay.
// FUNCTION: FF7 0x00666c13
void __cdecl CommitPendingModule(GfxEngineContext *context)
{
    if (context != 0) {
        context->activeModule.enter = context->pendingModule.enter;
        context->activeModule.exit = context->pendingModule.exit;
        context->activeModule.frame = context->pendingModule.frame;
        context->activeModule.onMouse = context->pendingModule.onMouse;
        context->activeModule.onKey = context->pendingModule.onKey;
    }
}

/*
 * Carries out a module switch that RequestModuleSwitch staged, at the top of each WinMain loop
 * pass before the frame callback runs: it runs the leaving module's exit callback, installs the
 * staged module's enter, exit, frame, mouse and key callbacks (CommitPendingModule), frees the
 * overflow blocks the render-object bank's three per-frame heaps grew in the old module
 * (Gfx_TrimRenderPoolOverflow), runs the new module's enter callback and clears
 * moduleSwitchPending, so the next request can be staged. Does nothing when no switch is pending.
 */
// FUNCTION: FF7 0x00666c78
void __cdecl ServiceModuleSwitch(GfxEngineContext *ctx)
{
    if (ctx != 0 && ctx->moduleSwitchPending != 0) {
        if (ctx->activeModule.exit != 0) {
            ctx->activeModule.exit(ctx);
        }
        CommitPendingModule(ctx);
        Gfx_TrimRenderPoolOverflow((struct GfxRenderPool *)ctx->renderObjectBank);
        if (ctx->activeModule.enter != 0) {
            ctx->activeModule.enter(ctx);
        }
        ctx->moduleSwitchPending = 0;
    }
}

// When both arguments are nonnull and no switch is pending (moduleSwitchPending), copies
// the module record to the context's pendingModule, sets moduleSwitchPending and returns
// one. Otherwise it returns zero and changes nothing.
// FUNCTION: FF7 0x00666cf2
int __cdecl RequestModuleSwitch(GameModule *moduleRecord, struct GfxEngineContext *context)
{
    int result;

    result = 0;
    if (moduleRecord != 0 && context != 0 && context->moduleSwitchPending == 0) {
        context->pendingModule = *moduleRecord;
        context->moduleSwitchPending = 1;
        result = 1;
    }
    return result;
}

// If both pointers are nonnull, copies the active module's five callbacks (enter, exit, frame,
// onMouse and onKey) from context into moduleRecord; its init and shutdown are left alone.
// FUNCTION: FF7 0x00666d47
void __cdecl GetActiveModule(GameModule *moduleRecord, GfxEngineContext *context)
{
    if (moduleRecord != 0 && context != 0) {
        moduleRecord->enter = context->activeModule.enter;
        moduleRecord->exit = context->activeModule.exit;
        moduleRecord->frame = context->activeModule.frame;
        moduleRecord->onMouse = context->activeModule.onMouse;
        moduleRecord->onKey = context->activeModule.onKey;
    }
}

/*
 * Starts a new frame in the draw pool: a module frame calls it before it fills batches. It passes
 * the context's drawPool (whose first word is the generation) to Gfx_AdvanceDrawPoolGeneration, which increments the generation and rewinds the
 * pool's node heap. Batches and node lists stamped with an older generation are emptied on their
 * next use, so last frame's primitives drop out. Does nothing for a null context.
 */
// FUNCTION: FF7 0x00666da3
void __cdecl ResetFrameDrawPool(struct GfxEngineContext *context)
{
    if (context != 0) {
        Gfx_AdvanceDrawPoolGeneration(context->drawPool);
    }
}

/*
 * Rewinds the per-frame storage of the context's renderObjectBank, once per module frame:
 * Gfx_ResetRenderObjectPool rewinds the bank's three block heaps at +0x04, +0x08 and +0x0C
 * (0x84-, 0x34- and 0xA0-byte blocks) with their overflow chains, without freeing memory, and
 * zeroes the bank's words +0x50 and +0x54. The render objects themselves (the heap at +0x00) are
 * kept. Does nothing for a null context.
 */
// FUNCTION: FF7 0x00666dc0
void __cdecl ResetRenderPools(struct GfxEngineContext *context)
{
    if (context != 0) {
        Gfx_ResetRenderObjectPool(context->renderObjectBank);
    }
}

/*
 * Empties the context's deferred-draw nodeList at the start of a module frame: the list's head
 * (+0x04) and word +0x08 are zeroed and its block heap rewound (ResetBlockHeapNodeList). The
 * polygon-set submitters then append the groups they defer this frame, and GfxFlushFrameDrawList
 * draws them.
 * Unlike its neighbours it does not check the context for null.
 */
// FUNCTION: FF7 0x00666ddd
void __cdecl Gfx_ResetNodeList(struct GfxEngineContext *context)
{
    ResetBlockHeapNodeList((struct GfxBlockHeapNodeList *)(context->nodeList));
}

/*
 * Draws the groups that D3D5SubmitPolygonSet and Gfx_SubmitPolygonSetFallback deferred to the
 * nodeList this frame, near the end of a module frame (after Gfx_SetScenePassCall(1)).
 * GfxFlushDrawList walks the list: for a group whose word +0x94 is set it runs the node's draw-spec
 * callback when the spec asks for one and applies the draw spec to the group, then it draws every
 * node through the driver's drawBatchNode slot (+0x7C). The list is not emptied here (Gfx_ResetNodeList
 * does that next frame), and the context must not be null.
 */
// FUNCTION: FF7 0x00666df4
void __cdecl GfxFlushFrameDrawList(GfxEngineContext *context)
{
    GfxFlushDrawList(context->nodeList, context);
}

/*
 * For slot from zero through four, sets overrideActive and copies presetHundreds[slot] to
 * overrideRenderState.
 */
// FUNCTION: FF7 0x00666e0f
void __cdecl SelectContextSlotOverride(int slot, GfxEngineContext *context)
{
    if (slot >= 0 && slot < 5) {
        context->overrideActive = 1;
        context->overrideRenderState = context->presetHundreds[slot];
    }
}

/*
 * Clears overrideActive.
 */
// FUNCTION: FF7 0x00666e43
void __cdecl ClearContextSlotOverride(GfxEngineContext *context)
{
    context->overrideActive = 0;
}

/*
 * Stores value in blitRectsEnabled.
 */
// FUNCTION: FF7 0x00666e55
void __cdecl Gfx_SetContextBlitRectsEnabled(int value, struct GfxEngineContext *context)
{
    context->blitRectsEnabled = value;
}

/*
 * If slot is 0 or 1, writes left, top, right and bottom into the second to fifth ints of
 * blitRects[slot]. The record's first int is left alone.
 */
// FUNCTION: FF7 0x00666e66
void __cdecl Gfx_SetContextBlitRect(int slot, int left, int top, int right, int bottom, GfxEngineContext *context)
{
    GfxContextRectSlot *record;

    if (slot >= 0 && slot < 2) {
        record = (GfxContextRectSlot *)&context->blitRects[slot];
        record->left = left;
        record->top = top;
        record->right = right;
        record->bottom = bottom;
    }
}

/*
 * When rasterBase is nonnull, clears the frame counter its object points to at +0x54A60.
 */
// FUNCTION: FF7 0x00666eb1
void __cdecl Gfx_ResetRasterFrameCounter(GfxEngineContext *context)
{
    if (context->rasterBase != 0) {
        context->rasterBase->renderSetupBlock->frameCounter = 0;
    }
}

/*
 * When rasterBase is nonnull, returns the frame counter its object points to at +0x54A60;
 * otherwise returns zero.
 */
// FUNCTION: FF7 0x00666ed7
int __cdecl Gfx_ReadRasterFrameCounter(GfxEngineContext *context)
{
    int result;

    result = 0;
    if (context->rasterBase != 0) {
        result = context->rasterBase->renderSetupBlock->frameCounter;
    }
    return result;
}

/*
 * Adds deltaU and deltaV to the u and v of count consecutive vertices beginning at vertices.
 */
// FUNCTION: FF7 0x00666f09
void __cdecl Gfx_OffsetVertexTexCoords(float deltaU, float deltaV, int count, GfxVertexTextured *vertices)
{
    GfxTexVertex *vertex;
    int index;

    vertex = (GfxTexVertex *)vertices;
    for (index = 0; index < count; index++) {
        vertex->u += deltaU;
        vertex->v += deltaV;
        vertex++;
    }
}
} /* extern "C" */
