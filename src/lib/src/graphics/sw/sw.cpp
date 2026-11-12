// src/lib/src/graphics/sw/sw.cpp
// Original: C:\lib\src\graphics\sw\sw.cpp
// Contents: Sets up the software rasterizer: it creates and destroys render state, registers
//   texture formats, initializes target tables and clip rectangles, clears targets, and compiles
//   and draws polygon batches.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/api/graphics.h"
#include "lib/api/mem.h"
#include "lib/types/graphics.h"
#include "lib/types/polygon.h"
#include "lib/codegen.h"
#include "lib/crt.h"
#include <windows.h>
#include <ddraw.h>
#include "lib/enums/graphics.h"

extern "C" {
extern int g_GfxSwTextureFormatSpecs[6]; /* Forward-declared before C++ definition below for C linkage. */


int __cdecl Trans_BuildScaledLut(double factor, int bits, struct GfxPixelFormat *format);
int __cdecl Trans_BuildDoubleBrightLutSaturate(int bits, struct GfxPixelFormat *format);

// Struct types for the assignments in the bodies below.



extern int g_GfxSwBlendModeRecord[10]; /* Forward-declared before C++ definition below for C linkage. */

// clang-format off
// Software rasterizer default texture-format spec table (words 1, 8, and four zero dwords).
// GLOBAL: FF7 0x0090f490
extern "C" int g_GfxSwTextureFormatSpecs[6] = {1, 8};
// Software rasterizer default blend-mode record (enabled flag 1 followed by nine zero dwords).
// GLOBAL: FF7 0x0090f4a8
extern "C" int g_GfxSwBlendModeRecord[10] = {1};
// clang-format on
// GLOBAL: FF7 0x0090f4d0
float g_GfxSwMatrixScratch[4][4] = {
    {1.0f, 0.0f, 0.0f, 0.0f},
    {0.0f, 1.0f, 0.0f, 0.0f},
    {0.0f, 0.0f, 1.0f, 0.0f},
    {0.0f, 0.0f, 0.0f, 1.0f}};
#pragma optimize("p", on)
// The optimize("p") pairs in this file set /Op per function, as the retail build did (lib/codegen.h, "/Op pairs").
#pragma optimize("p", off)
// Prototypes of callees the C sources used implicitly (definitions elsewhere), required in C++.

/*
 * Registers default software texture pixel format records into context.
 */
// FUNCTION: FF7 0x0067fa90
void __cdecl Blit_InsertTextureFormatRecords(GfxEngineContext *context)
{
    GfxTextureFormatRecord *recordPtr;
    int formatIdx;
    GfxPixelMaskTemplate *specPointer;
    GfxTextureFormatRecord *extraRecordPtr;
    GfxTextureFormatRecord *allocatedPtr;
    GfxTextureFormatRecord *entry;

    specPointer = (GfxPixelMaskTemplate *)&g_GfxSwTextureFormatSpecs;
    for (formatIdx = 0; formatIdx < 1; formatIdx++) {
        allocatedPtr = (GfxTextureFormatRecord *)AllocTextureFormatRecord();
        recordPtr = allocatedPtr;
        entry = recordPtr;
        if (entry != 0) {
            PixelFormatLoadMaskTemplate(specPointer, &entry->format);
            InsertTextureFormatRecord(recordPtr, context);
        }
        specPointer++;
    }
    extraRecordPtr = (GfxTextureFormatRecord *)AllocTextureFormatRecord();
    recordPtr = extraRecordPtr;
    entry = recordPtr;
    if (entry != 0) {
        recordPtr->format = context->targetFormat;
        InsertTextureFormatRecord(recordPtr, context);
    }
}

/*
 * Fills software render surface descriptor with pitch and dimensions.
 */
// FUNCTION: FF7 0x0067fb3f
void __cdecl Blit_FillRenderDescriptor(SwBlitDescriptor *descriptor, GfxEngineContext *context)
{
    if (descriptor != 0) {
        ZeroFillMemory32(36, descriptor);
        descriptor->displayWidth = context->displayWidth;
        descriptor->displayHeight = context->displayHeight;
        descriptor->hasOwnBuffer = 1;
        descriptor->bytesPerPixel = 2;
        descriptor->usesAlignedBlock = 1;
        descriptor->zDepth = 4;
        descriptor->zClearValue = 0xFFFFFFFF;
        descriptor->zScale = FLOAT_16777216_BITS;
        descriptor->vertexCount = 2048;
    }
}

/*
 * Initializes software render target dispatch tables and color lookups.
 */
// FUNCTION: FF7 0x0067fbbb
void __cdecl Blit_InitTargetTables(SwBlitTargetTable *target, GfxEngineContext *context)
{
    struct {
        int mapped;
        SwArgbColor color;
        int integer;
        int outer;
        int inner;
        float product;
        float scale;
        int index;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    ZeroFillMemory32(346308, target);
    target->defaultMode00 = 2;
    target->defaultMode04 = 0;
    target->defaultMode08 = 1;
    target->defaultMode0C = 1;
    target->defaultMode10 = 2;
    target->defaultMode14 = 0;
    target->defaultMode18 = 0;
    target->defaultMode1C = 2;
    target->defaultMode20 = 1;
    target->defaultMode28 = 2;
    target->defaultMode2C = 0;
    target->defaultMode30 = 0;
    target->defaultMode38 = 1;
    target->defaultMode3C = 0;
    target->defaultMode40 = 0;

    for (frame.index = 0; frame.index <= 640; frame.index++) {
        target->indexAsFloat[frame.index] = (float)frame.index;
        if (frame.index > 0)
            target->indexReciprocal[frame.index] = 1.0f / (float)frame.index;
    }
    for (frame.index = 0; frame.index < 768; frame.index++)
        target->thirds[frame.index] = (char)(frame.index / 3);
    target->drawTriangle = (int)Gfx_RasterDrawTriangle;
    for (frame.index = 0; frame.index < 65536; frame.index++) {
        frame.mapped = UnpackPixelToArgb8888(frame.index, &context->targetFormat);
        target->argbOfPixel[frame.index] = frame.mapped;
    }
    for (frame.outer = 0; frame.outer < 256; frame.outer++) {
        frame.scale = (float)frame.outer / 255.0f;
        for (frame.inner = 0; frame.inner < 256; frame.inner++) {
            frame.product = (float)frame.inner / 255.0f * frame.scale * 255.0f;
            /* Retail rounds via bare x87 fistp rather than VC5's truncating _ftol. */
            __asm { fld frame.product }
            __asm {fistp frame.integer} target->channelProduct[frame.inner + (frame.outer << 8)] = (char)frame.integer;
        }
    }
    frame.color.ch.r = 0;
    frame.color.ch.g = 0;
    frame.color.ch.b = 0;
    frame.color.ch.a = 255;
    target->mappedColor = Blit_MapArgb8888ViaRampTables(*(GfxArgb32 *)&frame.color, context);
}

/*
 * Frees software render state memory allocation.
 */
// FUNCTION: FF7 0x0067fde3
void __cdecl Blit_FreeRenderState(SwRasterizerState *state)
{
    if (state != 0) {
        Gfx_RasterFreeVertexBuffer((GfxRasterVertexBuffer *)(state->rasterVertexBuffer));
        // STRING: FF7 0x0090f510
        GameHeapFree((void *)state->halfScaleLut, "C:\\lib\\src\\graphics\\sw\\sw.cpp", 194);
        // STRING: FF7 0x0090f530
        GameHeapFree((void *)state->doubleBrightLut, "C:\\lib\\src\\graphics\\sw\\sw.cpp", 195);
        // STRING: FF7 0x0090f550
        GameHeapFree((void *)state->quarterScaleLut, "C:\\lib\\src\\graphics\\sw\\sw.cpp", 196);
        // STRING: FF7 0x0090f570
        GameHeapFree((void *)state->threeQuarterScaleLut, "C:\\lib\\src\\graphics\\sw\\sw.cpp", 197);
        if (state->usesAlignedHeap != 0) {
            // STRING: FF7 0x0090f590
            GameHeapFree((void *)state->heapBlock, "C:\\lib\\src\\graphics\\sw\\sw.cpp", 201);
            state->heapBlock = 0;
            state->alignedBase = 0;
        } else {
            // STRING: FF7 0x0090f5b0
            GameHeapFree((void *)state->block, "C:\\lib\\src\\graphics\\sw\\sw.cpp", 207);
            state->block = 0;
        }
        Gfx_RasterFreeZBuffer((GfxRasterZBuffer *)(state->zBuffer));
        state->zBuffer = 0;
        // STRING: FF7 0x0090f5d0
        GameHeapFree(state, "C:\\lib\\src\\graphics\\sw\\sw.cpp", 215);
    }
}

/*
 * Allocates and initializes software render state structure.
 */
// FUNCTION: FF7 0x0067fefe
int __cdecl Blit_CreateRenderState(GfxRasterRenderDesc *desc, GfxEngineContext *context)
{
    int valid;
    SwRasterizerState *state;
    int storePtr;
    int blockSize;

    valid = 0;
    // STRING: FF7 0x0090f5f0
    state = (SwRasterizerState *)GameHeapAlloc(1, 346740, "C:\\lib\\src\\graphics\\sw\\sw.cpp", 227);
    if (state != 0) {
        *(GfxRasterRenderDesc *)state = *desc;
        valid = 1;
        if (desc->fixedPitch != 0) {
            blockSize = 4096;
            // STRING: FF7 0x0090f610
            state->heapBlock = (int)GameHeapAlloc(blockSize << 9, 1, "C:\\lib\\src\\graphics\\sw\\sw.cpp", 242);
            state->alignedBase = state->heapBlock + blockSize & ~(blockSize - 1);
            state->rowBytes = blockSize;
            state->block = state->alignedBase + 0x1000;
            state->renderSetup = state->alignedBase + 0xEA0;
            state->renderSetup = context->rampBlock + 0x8EA0;
            *(int *)((char *)state->renderSetup + 0x15C) = context->rampBlock + 0x8EA0;
            if (valid != 0) {
                storePtr = state->alignedBase + 0x1500;
                state->zBuffer = Gfx_RasterCreateZBufferWithStore(desc->width, desc->height, desc->zDepth, desc->zClearValue, blockSize, storePtr);
                if (state->zBuffer != 0) {
                    *(int *)((char *)state->zBuffer + 4) = (int)(4294967296.0 / state->zScale);
                } else {
                    valid = 0;
                }
            }
        } else {
            if (desc->allocatePixelBlock != 0) {
                state->rowBytes = desc->width * desc->bytesPerPixel;
                // STRING: FF7 0x0090f630
                state->block = (int)GameHeapAlloc(desc->width * desc->height, desc->bytesPerPixel, "C:\\lib\\src\\graphics\\sw\\sw.cpp", 282);
            }
            state->renderSetup = (int)state + 0x54900;
            if (valid != 0) {
                state->zBuffer = Gfx_RasterCreateZBuffer(desc->width, desc->height, desc->zDepth, desc->zClearValue);
                if (state->zBuffer != 0) {
                    *(int *)((char *)state->zBuffer + 4) = (int)(4294967296.0 / state->zScale);
                } else {
                    valid = 0;
                }
            }
        }
        Blit_InitTargetTables((struct SwBlitTargetTable *)((int)state + 0x38), context);
        state->setupBlock = state->renderSetup;
        Gfx_RasterClearBatchArena((char *)state + 0x1C8);
        Gfx_RasterClearBatchArena((char *)state + 0x14BC);
        if (valid != 0) {
            state->rasterVertexBuffer = Gfx_RasterCreateVertexBuffer(desc->vertexCount);
            if (state->rasterVertexBuffer != 0) {
                state->vertexBuffer = state->rasterVertexBuffer;
            } else {
                valid = 0;
            }
        }
    }
    if (valid == 0) {
        Blit_FreeRenderState(state);
        state = 0;
    }
    return (int)state;
}

/*
 * Releases software rasterizer state and destroys render state.
 */
// FUNCTION: FF7 0x006801d5
void __cdecl Blit_OpsDestroyRenderState(GfxEngineContext *context)
{
    SwRasterizerState *state;

    state = (SwRasterizerState *)GetRenderStateObject(context);
    Blit_FreeRenderState(state);
}

/*
 * Initializes software render state with default operations.
 */
// FUNCTION: FF7 0x006801f8
int __cdecl Blit_OpsInitRenderState(GfxEngineContext *context)
{
    struct SwInitFrame {
        int format;
        int renderer;
        struct SwRasterizerState *state;
        int descriptor_address;
        int result;
        GfxRasterRenderDesc descriptor;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    frame.result = 0;
    Gfx_RasterNoOpDispatchGuards();
    frame.descriptor_address = (int)&frame.descriptor;
    Blit_FillRenderDescriptor((struct SwBlitDescriptor *)frame.descriptor_address, context);
    frame.state = (struct SwRasterizerState *)Blit_CreateRenderState((struct GfxRasterRenderDesc *)frame.descriptor_address, context);
    if (frame.state != 0) {
        frame.renderer = (int)frame.state + 0x38;
        SetRenderStateObject((int)frame.state, context);
        Blit_InsertTextureFormatRecords(context);
        Gfx_RasterFillRenderSetupBlock((GfxRasterSetupBlock *)(frame.state->renderSetup), context);
        Gfx_RasterCopyStatePair((GfxRasterStatePairIn *)(frame.state->zBuffer), (GfxRasterStatePairOut *)(frame.state->renderSetup));
        frame.format = 0x10;
        frame.state->halfScaleLut = Trans_BuildScaledLut(0.5, frame.format, &context->targetFormat);
        frame.state->doubleBrightLut = Trans_BuildDoubleBrightLutSaturate(frame.format, &context->targetFormat);
        frame.state->quarterScaleLut = Trans_BuildScaledLut(0.25, frame.format, &context->targetFormat);
        frame.state->threeQuarterScaleLut = Trans_BuildScaledLut(0.75, frame.format, &context->targetFormat);
        ((SwBlitDrawQueue *)(frame.state->renderSetup))->colorScaleLut = frame.state->halfScaleLut;
        ((SwBlitDrawQueue *)(frame.state->renderSetup))->sumRemapLut = frame.state->doubleBrightLut;
        frame.result = 1;
    }
    return frame.result;
}

/*
 * Clears rectangular viewport region with color and depth fill.
 */
// FUNCTION: FF7 0x00680367
void __cdecl Blit_OpsClearTargetRect(int clearColor, int clearDepth, int useViewport, GfxEngineContext *context)
{
    struct SwBltFrame {
        int zClearState, n, clearPixel;
        GfxPixelFormat *lockedFormat;
        int backSurface;
        DDBLTFX fx;
        RECT rect;
        int d, pixelBase, s;
        GfxPixelFormat *clearFormat;
        int r, b, clipTop, q, a, renderState;
        GfxDriverOps *p;
    } bltFrame;
    typedef char SwFrameIs180[(sizeof(SwBltFrame) == 180) ? 1 : -1];
    typedef char SwFxIs100[(sizeof(DDBLTFX) == 100) ? 1 : -1];
    typedef char SwRectIs16[(sizeof(RECT) == 16) ? 1 : -1];
    bltFrame.renderState = GetRenderStateObject(context);
    bltFrame.p = GetGraphicsDriverObject(context);
    if (bltFrame.renderState != 0) {
        if (useViewport != 0) {
            bltFrame.a = context->clipLeft;
            bltFrame.clipTop = context->clipTop;
            bltFrame.q = context->viewSizeX;
            bltFrame.b = context->viewSizeY;
        } else {
            bltFrame.a = 0;
            bltFrame.clipTop = 0;
            bltFrame.q = context->displayWidth;
            bltFrame.b = context->displayHeight;
        }
        bltFrame.r = ((BlitRowFillRecord *)bltFrame.renderState)->fillColor;
        if (bltFrame.r != 0) {
            if (bltFrame.a != ((BlitClearRectCache *)bltFrame.r)->cacheA || (bltFrame.clipTop != ((BlitClearRectCache *)bltFrame.r)->cacheSlot || (bltFrame.q != ((BlitClearRectCache *)bltFrame.r)->cacheQ || bltFrame.b != ((BlitClearRectCache *)bltFrame.r)->cacheB))) {
                ((BlitClearRectCache *)bltFrame.r)->clearCountdown = 0;
            }
            ((BlitClearRectCache *)bltFrame.r)->cacheA = bltFrame.a;
            ((BlitClearRectCache *)bltFrame.r)->cacheSlot = bltFrame.clipTop;
            ((BlitClearRectCache *)bltFrame.r)->cacheQ = bltFrame.q;
            ((BlitClearRectCache *)bltFrame.r)->cacheB = bltFrame.b;
        }
        if (clearColor != 0) {
            if (((BlitRowFillRecord *)bltFrame.renderState)->fillFlag != 0) {
                bltFrame.clearFormat = &context->targetFormat;
                if (bltFrame.clearFormat->bytesPerPixel == 2) {
                    bltFrame.s = Blit_ReplicateWordToDword(bltFrame.p->backgroundPixel);
                } else {
                    bltFrame.s = bltFrame.p->backgroundPixel;
                }
                bltFrame.pixelBase = ((BlitRowFillRecord *)bltFrame.renderState)->pixels;
                Mem_FillDwordRows(bltFrame.s, bltFrame.q * ((BlitRowFillRecord *)bltFrame.renderState)->rowStride, bltFrame.b, ((BlitRowFillRecord *)bltFrame.renderState)->width - bltFrame.q * ((BlitRowFillRecord *)bltFrame.renderState)->rowStride, (void *)(bltFrame.pixelBase + (bltFrame.a * ((BlitRowFillRecord *)bltFrame.renderState)->rowStride + bltFrame.clipTop * ((BlitRowFillRecord *)bltFrame.renderState)->width)));
            } else {
                bltFrame.d = 1;
                if ((context->blitFlags & 0x4000000) != 0) {
                    bltFrame.rect.left = bltFrame.a;
                    bltFrame.rect.top = bltFrame.clipTop;
                    bltFrame.rect.right = bltFrame.a + bltFrame.q;
                    bltFrame.rect.bottom = bltFrame.clipTop + bltFrame.b;
                    ZeroFillMemory32(100, &bltFrame.fx);
                    bltFrame.fx.dwSize = 100;
                    bltFrame.fx.dwFillColor = bltFrame.p->backgroundPixel;
                    bltFrame.backSurface = GfxGetSurfaceByIndex(1);
                    if (bltFrame.backSurface != 0) {
                        // STRING: FF7 0x0090f650
                        if (CheckDirectDrawResult(((IDirectDrawSurface *)(bltFrame.backSurface))->Blt(&bltFrame.rect, 0, 0, 0x600, &bltFrame.fx), "C:\\lib\\src\\graphics\\sw\\sw.cpp", 517) != 0) {
                            bltFrame.d = 0;
                        } else {
                            bltFrame.d = 1;
                        }
                    }
                }
                if (bltFrame.d != 0) {
                    if (GfxLockSurfaceCall(1) != 0) {
                        bltFrame.lockedFormat = &context->targetFormat;
                        bltFrame.n = bltFrame.lockedFormat->pixels;
                        if (bltFrame.lockedFormat->bytesPerPixel == 2) {
                            bltFrame.clearPixel = Blit_ReplicateWordToDword(bltFrame.p->backgroundPixel);
                        } else {
                            bltFrame.clearPixel = bltFrame.p->backgroundPixel;
                        }
                        Mem_FillDwordRows(bltFrame.clearPixel, bltFrame.q * bltFrame.lockedFormat->bytesPerPixel, bltFrame.b, bltFrame.lockedFormat->pitch - bltFrame.q * bltFrame.lockedFormat->bytesPerPixel, (void *)(bltFrame.n + (bltFrame.a * bltFrame.lockedFormat->bytesPerPixel + bltFrame.clipTop * bltFrame.lockedFormat->pitch)));
                        GfxUnlockSurfaceCall(1);
                    }
                }
            }
        }
        if (clearDepth != 0) {
            bltFrame.zClearState = ((BlitRowFillRecord *)bltFrame.renderState)->fillColor;
            if (useViewport != 0) {
                Gfx_RasterClearZBufferRectIfDue(bltFrame.a, bltFrame.clipTop, bltFrame.q, bltFrame.b, (GfxZBufferClearState *)bltFrame.zClearState);
            } else {
                Gfx_RasterClearZBufferRectIfDue(bltFrame.a, bltFrame.clipTop, bltFrame.q, bltFrame.b, (GfxZBufferClearState *)bltFrame.zClearState);
            }
        }
    }
}

/*
 * Clears render target buffer with all buffer flags enabled.
 */
// FUNCTION: FF7 0x006806ef
void __cdecl Gfx_RasterClearTargetAllFlags(void *context)
{
    Blit_OpsClearTargetRect(1, 1, 1, (GfxEngineContext *)context);
}

/*
 * Updates software rasterizer clipping rectangles from context.
 */
// FUNCTION: FF7 0x00680706
void __cdecl Gfx_RasterRefreshClipRects(int unused1, int unused2, int unused3, int unused4, GfxEngineContext *context)
{
    SwRasterizerState *state;

    state = (SwRasterizerState *)GetRenderStateObject(context);
    if (state != 0) {
        Gfx_RasterCopyContextRectExclusive((SwIntClipRect *)(state->rasterVertexBuffer), context);
        Gfx_RasterBuildFloatClipRect((SwFloatClipRect *)((int)state + 0x1C8), context);
        Gfx_RasterBuildFloatClipRect((SwFloatClipRect *)((int)state + 0x14BC), context);
    }
}

/*
 * Sets rasterizer clear background color and converts to target pixel format.
 */
// FUNCTION: FF7 0x00680765
void __cdecl Blit_OpsSetBackgroundColor(void *color, GfxEngineContext *context)
{
    GfxDriverOps *driver;

    driver = GetGraphicsDriverObject(context);
    Blit_PackBgraFloatsToArgb8888((unsigned char *)&driver->backgroundArgb, (float *)(color));
    driver->backgroundPixel = Blit_MapArgb8888ViaRampTables(*(GfxArgb32 *)&driver->backgroundArgb, context);
}

/*
 * Allocates vertex batch array memory for primitive rasterization.
 */
// FUNCTION: FF7 0x006807a8
int __cdecl Gfx_RasterAllocBatchArray(void *count)
{
    int result;

    result = 0;
    if (count != 0) {
        if (D3D5AllocBatchArray((struct GfxPolySetBatchArray *)count) != 0) {
            result = 1;
        }
    }
    return result;
}

/*
 * Compiles polygon set group into rasterizer draw batch.
 */
// FUNCTION: FF7 0x006807d7
int __cdecl Gfx_RasterCompileGroup(int group, int batch, int flags, int stride, int option, int enable, int extra)
{
    int result;

    result = 0;
    if (enable != 0) {
        /* Retail caller passes 7 arg(s) to 6-parameter `D3D5CompileGroup` (__cdecl stack cleanup matches retail). */
        if (((int(__cdecl *)(int, int, int, int, int, int, int))D3D5CompileGroup)(group, batch, flags, stride, option, enable, extra) != 0) {
            if (option != 0) {
                result = 1;
            } else {
                result = 1;
            }
        }
    }
    return result;
}

/*
 * Stores or copies primitive vertex slot data.
 */
// FUNCTION: FF7 0x0068082d
void __cdecl Gfx_RasterSetMatrixSlot(int slot, int value, SwRasterSlots *slots)
{
    if (slots != 0) {
        switch (slot) {
        case GFX_MATRIX_SLOT_WORLD:
            if (slots->worldMatrix != 0) {
                *(GfxMatrix4x4 *)(slots->worldMatrix) = *(GfxMatrix4x4 *)(value);
            } else {
                slots->worldMatrix = value;
            }
            break;
        case GFX_MATRIX_SLOT_VIEW:
            if (slots->viewMatrix != 0) {
                *(GfxMatrix4x4 *)(slots->viewMatrix) = *(GfxMatrix4x4 *)(value);
            } else {
                slots->viewMatrix = value;
            }
            break;
        case GFX_MATRIX_SLOT_PROJECTION:
            if (slots->projectionMatrix != 0) {
                *(GfxMatrix4x4 *)(slots->projectionMatrix) = *(GfxMatrix4x4 *)(value);
            } else {
                slots->projectionMatrix = value;
            }
        }
    }
}

/*
 * No-op stub for raster operation callback.
 */
// FUNCTION: FF7 0x006808cd
void __cdecl Gfx_RasterUnloadTextureNoOp(void)
{
}

/*
 * Prepares rasterizer group record and allocates vertex storage.
 */
// FUNCTION: FF7 0x006808d2
GfxTextureSurface *__cdecl Gfx_RasterPrepareGroupRecord(GfxTextureSurface *record, GfxTexHeader *groupDesc, GfxPixelFormat *texture)
{
    GfxEngineContext *ctx;
    int changed;
    int state;
    GfxPixelFormat *pTexDesc;
    GfxPixelFormat *groupRec;
    int pixelBuf;
    GfxPaletteRecord *palRecPtr;

    state = 1;
    ctx = GetEngineContext();
    if (ctx != 0 && texture != 0) {
        if (record == 0) {
            record = (GfxTextureSurface *)Gfx_AllocSurfaceRecord();
        }
        if (record != 0) {
            record->header = (int)groupDesc;
            record->format = (int)texture;
            groupRec = &groupDesc->format;
            pTexDesc = texture;
            if (groupRec->palettized == pTexDesc->palettized && groupRec->indexBits == pTexDesc->indexBits && groupRec->indexedTo8 == pTexDesc->indexedTo8 && groupRec->bitsPerPixel == pTexDesc->bitsPerPixel && groupRec->bytesPerPixel == pTexDesc->bytesPerPixel && groupRec->redMask == pTexDesc->redMask && groupRec->greenMask == pTexDesc->greenMask && groupRec->blueMask == pTexDesc->blueMask && groupRec->alphaMask == pTexDesc->alphaMask) {
                changed = 0;
            } else {
                changed = 1;
            }
            if (texture->palettized != 0) {
                palRecPtr = (GfxPaletteRecord *)(Gfx_CreateSurfacePaletteRecord(texture->bitsPerPixel, groupDesc, record));
                if (palRecPtr != 0) {
                    record->palette = (int)palRecPtr;
                    state = 0;
                } else {
                    state = 1;
                }
            } else {
                state = 0;
            }
            if (state == 0 && changed != 0) {
                // STRING: FF7 0x0090f670
                pixelBuf = (int)GameHeapAlloc(groupRec->width * groupRec->height * pTexDesc->bytesPerPixel, 1, "C:\\lib\\src\\graphics\\sw\\sw.cpp", 806);
                if (pixelBuf != 0) {
                    pTexDesc->pitch = groupRec->width * pTexDesc->bytesPerPixel;
                    Gfx_ConvertTexturePixelsToSurface(groupDesc, pTexDesc, pixelBuf);
                    groupDesc->storage = pixelBuf;
                } else {
                    state = 1;
                }
            }
            if (state != 0) {
                Gfx_ReleaseTextureRecordRef((int)record);
                record = 0;
            }
        }
    }
    return record;
}

/*
 * Stub callback for deferred render list execution.
 */
// FUNCTION: FF7 0x00680aae
void __cdecl Blit_RenderDeferredListsStub(void)
{
}

/*
 * Returns pointer to software rasterizer blend mode configuration record.
 */
// FUNCTION: FF7 0x00680ab3
int __cdecl Blit_GetBlendModeRecord(void)
{
    int val;

    val = (int)&g_GfxSwBlendModeRecord;
    return val;
}

/*
 * Sets software rasterizer render state value.
 */
// FUNCTION: FF7 0x00680ac5
void __cdecl Blit_SetRenderState(int offset, int value, struct GfxEngineContext *ctx)
{
    SwBlitRenderState *stateBlock;

    stateBlock = (SwBlitRenderState *)ctx->presetPolygonSets;
    if (stateBlock != 0) {
        *((char *)stateBlock + offset) = value;
    }
}

/*
 * Applies untextured primitive render state configuration.
 */
// FUNCTION: FF7 0x00680aea
void __cdecl Blit_ApplyStateUntextured(void *group, struct GfxEngineContext *ctx)
{
    Blit_ApplyRenderSpec((struct GlDrawGroup *)group, ctx);
}

/*
 * Applies textured primitive render state configuration.
 */
// FUNCTION: FF7 0x00680aff
void __cdecl Blit_ApplyStateTextured(void *group, struct GfxEngineContext *ctx)
{
    Blit_ApplyRenderSpec((struct GlDrawGroup *)group, ctx);
}

/*
 * Applies preset render state flags to software context.
 */
// FUNCTION: FF7 0x00680b14
void __cdecl Blit_ApplyPresetState(int flags, struct GfxEngineContext *ctx)
{
    SwBlitRenderState *stateBlock;

    stateBlock = (SwBlitRenderState *)ctx->presetPolygonSets;
    if (stateBlock != 0) {
        stateBlock->blendMode = flags;
    }
}

/*
 * Applies detailed render specification to software rasterizer state.
 */
// FUNCTION: FF7 0x00680b37
void __cdecl Blit_ApplyRenderSpec(GlDrawGroup *group, struct GfxEngineContext *ctx)
{
    int flags;
    SwBlitRenderState *renderStatePtr;
    int flag;
    GfxTexHeader *tex;
    GfxPaletteRecord *page;
    GfxPaletteRecord *pageInfo;
    GfxTextureSurface *batchIndex;
    GfxTextureSurface *batchRec;
    GfxTexHeader *texRec;

    if (group != 0) {
        flags = group->changeMask;
        renderStatePtr = (SwBlitRenderState *)(ctx->presetPolygonSets);
        if ((flags & 0xFFFDFBFD) != 0) {
            if ((flags & 2) != 0) {
                batchIndex = (GfxTextureSurface *)(group->batch);
                renderStatePtr->batch = (int)batchIndex;
                if (batchIndex != 0 && (group->flags & 2) != 0) {
                    tex = (GfxTexHeader *)batchIndex->header;
                    page = (GfxPaletteRecord *)(batchIndex->palette);
                    if (page != 0) {
                        if (page->paletteMode != 0) {
                            if ((*(unsigned char *)((char *)page->colorLookup + batchIndex->paletteRow) & 1) != 0) {
                                flag = 1;
                            } else {
                                flag = 0;
                            }
                        } else if (tex->colorKey != 0) {
                            flag = 1;
                        } else {
                            flag = 0;
                        }
                    } else if (tex->colorKey != 0) {
                        flag = 1;
                    } else {
                        flag = 0;
                    }
                    renderStatePtr->colorKeyEnabled = flag;
                }
            }
            if ((flags & 0x200) != 0) {
                if ((group->flags & 0x200) != 0) {
                    flag = 1;
                } else {
                    flag = 0;
                }
                renderStatePtr->ditherEnabled = flag;
            }
            if ((flags & 8) != 0) {
                if ((group->flags & 8) != 0) {
                    flag = 1;
                } else {
                    flag = 0;
                }
                renderStatePtr->perspectiveCorrect = flag;
            }
            if ((flags & 0x4000) != 0) {
                if ((group->flags & 0x4000) != 0) {
                    renderStatePtr->cullingDisabled = 1;
                } else if ((flags & 0x2000) != 0) {
                    if ((group->flags & 0x2000) != 0) {
                        renderStatePtr->cullFrontFaces = 1;
                    } else {
                        renderStatePtr->cullFrontFaces = 0;
                    }
                }
            } else if ((flags & 0x2000) != 0) {
                if ((group->flags & 0x2000) != 0) {
                    renderStatePtr->cullFrontFaces = 1;
                } else {
                    renderStatePtr->cullFrontFaces = 0;
                }
            }
            if ((flags & 0x8000) != 0) {
                if ((group->flags & 0x8000) != 0) {
                    flag = 1;
                } else {
                    flag = 0;
                }
                renderStatePtr->depthTestEnabled = flag;
            }
            if ((flags & 0x10000) != 0) {
                if ((group->flags & 0x10000) != 0) {
                    flag = 1;
                } else {
                    flag = 0;
                }
                renderStatePtr->depthWriteEnabled = flag;
            }
            if ((flags & 0x100) != 0) {
                if ((group->flags & 0x100) != 0) {
                    flag = 1;
                } else {
                    flag = 0;
                }
                renderStatePtr->colorKeyEnabled = flag;
            }
            if ((flags & 0x400) != 0) {
                if ((group->flags & 0x400) != 0) {
                    flag = 1;
                } else {
                    flag = 0;
                }
                if (flag != 0) {
                    if (ctx->overrideActive != 0) {
                        renderStatePtr->blendMode = ((GfxRenderStateTable *)(ctx->overrideRenderState))->blendMode;
                    } else {
                        renderStatePtr->blendMode = group->blendMode;
                    }
                } else {
                    renderStatePtr->blendMode = BLEND_NONE;
                }
            }
            if ((flags & 0x20000) != 0) {
                if ((group->flags & 0x20000) != 0 && ctx->renderStateApplied == 0) {
                    if (group->groupKind == 1) {
                        renderStatePtr->isActive = 0;
                    } else {
                        renderStatePtr->isActive = 1;
                    }
                } else {
                    renderStatePtr->isActive = 0;
                }
            }
            if (REG_ALLOC_HINT) {
            }
            if ((flags & 0x20) != 0) {
                if ((group->flags & 0x20) != 0) {
                    renderStatePtr->wrapUEnabled = 1;
                } else {
                    renderStatePtr->isEnabled = 0;
                }
            }
            if ((flags & 0x40) != 0) {
                if ((group->flags & 0x40) != 0) {
                    renderStatePtr->isEnabled = 1;
                } else {
                    renderStatePtr->isEnabled = 0;
                }
            }
        } else {
            if ((flags & 0xFFFDFFFD) != 0) {
                if ((flags & 0x400) != 0) {
                    if ((group->flags & 0x400) != 0) {
                        flag = 1;
                    } else {
                        flag = 0;
                    }
                    if (flag != 0) {
                        if (ctx->overrideActive != 0) {
                            renderStatePtr->blendMode = ((GfxRenderStateTable *)(ctx->overrideRenderState))->blendMode;
                        } else {
                            renderStatePtr->blendMode = group->blendMode;
                        }
                    } else {
                        renderStatePtr->blendMode = BLEND_NONE;
                    }
                } else {
                    renderStatePtr->blendMode = BLEND_NONE;
                }
            }
            if ((flags & 2) != 0) {
                batchRec = (GfxTextureSurface *)(group->batch);
                renderStatePtr->batch = (int)batchRec;
                if (batchRec != 0 && (group->flags & 2) != 0) {
                    texRec = (GfxTexHeader *)batchRec->header;
                    pageInfo = (GfxPaletteRecord *)(batchRec->palette);
                    if (pageInfo != 0) {
                        if (pageInfo->paletteMode != 0) {
                            if ((*(unsigned char *)((char *)pageInfo->colorLookup + batchRec->paletteRow) & 1) != 0) {
                                flag = 1;
                            } else {
                                flag = 0;
                            }
                        } else if (texRec->colorKey != 0) {
                            flag = 1;
                        } else {
                            flag = 0;
                        }
                    } else if (texRec->colorKey != 0) {
                        flag = 1;
                    } else {
                        flag = 0;
                    }
                    renderStatePtr->colorKeyEnabled = flag;
                }
            }
            if ((flags & 0x20000) != 0) {
                if ((group->flags & 0x20000) != 0 && ctx->renderStateApplied == 0) {
                    if (group->groupKind == 1) {
                        renderStatePtr->isActive = 0;
                    } else {
                        renderStatePtr->isActive = 1;
                    }
                } else {
                    renderStatePtr->isActive = 0;
                }
            }
        }
    }
}

/*
 * Draws a 3D render object via polygon set rasterizer.
 */
// FUNCTION: FF7 0x00681075
void __cdecl Blit_DrawRenderObject(void *object, struct GfxEngineContext *ctx)
{
    if (object != 0) {
        Blit_DrawPolygonSet((struct GfxPolygonSet *)(*(int *)((char *)object + 0x14)), ctx);
    }
}

/*
 * Configures active scene render pass and selects render object.
 */
// FUNCTION: FF7 0x00681093
void __cdecl Blit_OpsSetScenePass(int pass, GfxEngineContext *ctx)
{
    ctx->scenePass = pass;
    switch (pass) {
    case GFX_SCENE_PASS_OPAQUE:
        ctx->opaqueSet->drawEnabled = 1;
        ctx->transSet->drawEnabled = 0;
        Gfx_DrawPolygonSetCall((PolygonSkeletonFrameRef *)(ctx->opaqueSet), (int)ctx);
        break;
    case GFX_SCENE_PASS_TRANSLUCENT:
        ctx->opaqueSet->drawEnabled = 0;
        ctx->transSet->drawEnabled = 1;
        Gfx_DrawPolygonSetCall((PolygonSkeletonFrameRef *)(ctx->transSet), (int)ctx);
    }
}

/*
 * Begins software scene rendering and locks target surface.
 */
// FUNCTION: FF7 0x00681125
int __cdecl Blit_OpsBeginScene(int pass, GfxEngineContext *ctx)
{
    int result;
    SwRasterizerState *renderState;
    result = 0;
    if (ctx->sceneActive == 0) {
        renderState = (SwRasterizerState *)GetRenderStateObject(ctx);
        Blit_OpsSetScenePass(pass, ctx);
        result = 1;
        ctx->sceneActive = 1;
        if (ctx->configPending != 0) {
            if (renderState->allocatePixelBlock != 0) { /* empty first arm: lib/codegen.h */
            } else {
                if (GfxLockSurfaceCall(1) != 0) { /* empty first arm: lib/codegen.h */
                } else {
                    result = 0;
                }
            }
        }
        Gfx_ResetRasterFrameCounter(GetEngineContext());
        Gfx_RasterSetStateFloatTag((float)*(int *)renderState->zBuffer * renderState->zScale, *(int *)&renderState->zScale, (SwStateFloatTag *)(renderState->rasterVertexBuffer));
    }
    return result;
}

/*
 * Ends software scene rendering and unlocks target surface.
 */
// FUNCTION: FF7 0x006811e3
void __cdecl Blit_OpsEndScene(GfxEngineContext *ctx)
{
    SwRasterizerState *renderState;

    if (ctx->sceneActive != 0) {
        if (ctx->configPending != 0) {
            renderState = (SwRasterizerState *)GetRenderStateObject(ctx);
            if (renderState->allocatePixelBlock != 0) { /* empty first arm: lib/codegen.h */
            } else {
                GfxUnlockSurfaceCall(1);
            }
        }
        if (ctx->statsRecord != 0) {
            ctx->statsRecord->rasterFrameCount = Gfx_ReadRasterFrameCounter(GetEngineContext());
        }
        ctx->sceneActive = 0;
    }
}

/*
 * Returns status indicating palette changes are acknowledged.
 */
// FUNCTION: FF7 0x0068125d
int __cdecl Blit_OpsPaletteChangedStub(void)
{
    return 1;
}

/*
 * Updates rasterizer palette table entries via ramp mapping.
 */
// FUNCTION: FF7 0x00681267
int __cdecl Blit_WritePaletteEntries(int start, int table, int rampA, int rampB, SwPaletteDesc *paletteDesc)
{
    int result;
    SwRasterizerState *renderState;
    int savedValue;

    result = 0;
    if (paletteDesc != 0) {
        renderState = (SwRasterizerState *)GetRenderStateObject(GetEngineContext());
        if (renderState != 0) {
            savedValue = renderState->halfScaleLut;
            Gfx_MapPaletteEntriesViaRampTables(table, start, rampA, rampB, paletteDesc->entryCount);
            Gfx_GatherPaletteEntriesByIndex(table, start, start, paletteDesc->entryCount, savedValue, paletteDesc->dstEntries);
            result = 1;
        }
    }
    return result;
}

/*
 * Calculates lighting values for polygon set vertices.
 */
// FUNCTION: FF7 0x006812f0
void __cdecl Blit_LightPolygonSet(int polySet, int ctx)
{
    Gfx_LightPolygonSet((GfxPolygonSet *)polySet, (GfxLightPolygonSet *)ctx);
}

/*
 * Stub callback for 2D draw record processing.
 */
// FUNCTION: FF7 0x00681305
void __cdecl Blit_Draw2DRecordStub(int record)
{
    int value;
    int ptr;

    ptr = record;
    value = *(int *)ptr;
}

/*
 * Renders polygon set with sorting, matrices, and rasterization passes.
 */
// FUNCTION: FF7 0x0068131d
void __cdecl Blit_DrawPolygonSet(GfxPolygonSet *drawGroup, struct GfxEngineContext *engine)
{
    int idx;
    GfxPolygonSetHeader *pSetHeader;
    int valid;
    int glKind;
    GfxRasterDrawGroup *rasterGroup;
    int depthSort;
    SwBlitPolygonNode *curPtr;
    int mtxPointer;
    GfxPolygonSetNode *altSetNodePtr;
    int primXform;
    int cachedMatrix;
    int hasData;
    GfxPolygonSetRecord *pPolySetRecord;
    GfxPolygonBatch *batchCur;
    int result;
    int visible;
    int applyFlag;
    int matPointer;
    GfxPolygonSetNode *queueNode;
    int depthKey;
    GfxPolygonSetNode *pDeferredNode;
    GfxDrawListNode *pDrawEntry;

    result = 0;
    if (drawGroup != 0 && drawGroup->drawEnabled != 0) {
        pPolySetRecord = (GfxPolygonSetRecord *)(drawGroup->matrices);
        pSetHeader = (GfxPolygonSetHeader *)((int)drawGroup + 0x14);
        hasData = 0;
        if (pSetHeader->drawListActive != 0) {
            hasData = 1;
            if (*(int *)pSetHeader->tagPtr == pSetHeader->tag) {
                valid = 1;
            } else {
                valid = 0;
            }
        }
        glKind = 0;
        if (drawGroup->hasGroupState != 0) {
            glKind = drawGroup->groupState;
        }
        rasterGroup = 0;
        if (drawGroup->source != 0) {
            rasterGroup = (GfxRasterDrawGroup *)(drawGroup->polygonData->rasterGroup);
        }
        for (idx = 0; idx < drawGroup->groupCount; idx++) {
            applyFlag = 0;
            if (drawGroup->useGroupStates != 0) {
                glKind = ((int *)(drawGroup->groupStates))[idx];
            }
            if (engine->sortedDrawEnabled != 0 && glKind != 0 && ((GlDrawGroup *)glKind)->polygonSet != 0) {
                depthSort = 1;
            } else {
                depthSort = 0;
            }
            if (glKind != 0) {
                switch (engine->scenePass) {
                case GFX_SCENE_PASS_OPAQUE:
                    visible = (((GlDrawGroup *)glKind)->flags & 0x410) == 0;
                    if (depthSort != 0) { /* empty first arm: lib/codegen.h */
                    } else {
                        applyFlag = visible == 0;
                    }
                    visible = 1;
                    break;
                case GFX_SCENE_PASS_TRANSLUCENT:
                    visible = 1;
                    break;
                case GFX_SCENE_PASS_ALL_IMMEDIATE:
                    visible = 1;
                    break;
                default:
                    visible = 0;
                }
            } else {
                visible = 1;
            }
            if (visible != 0) {
                if (applyFlag != 0) { /* empty first arm: lib/codegen.h */
                } else {
                    Blit_ApplyRenderSpec((struct GlDrawGroup *)glKind, engine);
                }
                if (hasData != 0) {
                    if (valid != 0) {
                        curPtr = (SwBlitPolygonNode *)(pSetHeader->payload);
                        if (curPtr != 0) {
                            switch (curPtr->kind) {
                            case GFX_DRAW_OP_BATCH:
                                do {
                                    if (applyFlag != 0) {
                                        queueNode = (GfxPolygonSetNode *)(AppendBlockHeapNode(engine->nodeList));
                                        if (queueNode != 0) {
                                            queueNode->polyIndex = idx;
                                            queueNode->drawGroup = (int)drawGroup;
                                            queueNode->primGroupPtr = glKind;
                                            if (drawGroup->drawSpecApplied != 0) {
                                                queueNode->stateBlock = *(GfxPolygonGroupDrawSpec *)(drawGroup->drawSpec);
                                            }
                                            switch (pSetHeader->kind) {
                                            case GFX_POLYGON_SET_TRANSFORM_NONE:
                                                queueNode->inUse = 0;
                                                queueNode->submitted = 0;
                                                break;
                                            case GFX_POLYGON_SET_TRANSFORM_NODE:
                                                queueNode->inUse = 0;
                                                queueNode->submitted = 1;
                                                queueNode->payloadPtr = (int)curPtr + 0xC;
                                                break;
                                            case GFX_POLYGON_SET_TRANSFORM_NODE_CAMERA:
                                                queueNode->inUse = 1;
                                                ConcatAffineMatrix4x4((float(*)[4])((int)curPtr + 0xC), (float(*)[4])(engine->cameraMatrix), (float(*)[4])((int)queueNode + 0x14));
                                                ClearMatrixColumn4(queueNode->matrix);
                                                queueNode->submitted = 0;
                                                break;
                                            default:
                                                queueNode->inUse = 0;
                                                queueNode->submitted = 0;
                                            }
                                        }
                                    } else {
                                        switch (pSetHeader->kind) {
                                        case GFX_POLYGON_SET_TRANSFORM_NONE:
                                            matPointer = 0;
                                            break;
                                        case GFX_POLYGON_SET_TRANSFORM_NODE:
                                            matPointer = (int)curPtr + 0xC;
                                            break;
                                        case GFX_POLYGON_SET_TRANSFORM_NODE_CAMERA:
                                            ConcatAffineMatrix4x4((float(*)[4])((int)curPtr + 0xC), (float(*)[4])(engine->cameraMatrix), (float(*)[4])((int)&g_GfxSwMatrixScratch));
                                            matPointer = (int)&g_GfxSwMatrixScratch;
                                            break;
                                        default:
                                            matPointer = 0;
                                        }
                                        if (drawGroup->batchList != 0) {
                                            batchCur = drawGroup->batchList[idx];
                                            if (batchCur != 0) {
                                                if (depthSort != 0) {
                                                    Gfx_EnqueueGroupForDepthSort(matPointer, 0, batchCur, (int)drawGroup, glKind, (int)rasterGroup, engine);
                                                } else if (drawGroup->drawUnsorted != 0 && matPointer != 0) {
                                                    Gfx_RasterDrawGroupWithMatrix(matPointer, batchCur, drawGroup, (GlDrawGroup *)glKind, rasterGroup, engine);
                                                }
                                            }
                                        }
                                    }
                                    curPtr = (SwBlitPolygonNode *)(curPtr->next);
                                } while (curPtr != 0);
                                break;
                            case GFX_DRAW_OP_BATCH_DRAW_SPEC:
                                do {
                                    if (applyFlag != 0) {
                                        altSetNodePtr = (GfxPolygonSetNode *)(AppendBlockHeapNode(engine->nodeList));
                                        if (altSetNodePtr != 0) {
                                            altSetNodePtr->polyIndex = idx;
                                            altSetNodePtr->drawGroup = (int)drawGroup;
                                            altSetNodePtr->primGroupPtr = glKind;
                                            if (drawGroup->drawSpecApplied != 0) {
                                                altSetNodePtr->stateBlock = *(GfxPolygonGroupDrawSpec *)(drawGroup->drawSpec);
                                            }
                                            switch (pSetHeader->kind) {
                                            case GFX_POLYGON_SET_TRANSFORM_NONE:
                                                altSetNodePtr->inUse = 0;
                                                altSetNodePtr->submitted = 0;
                                                break;
                                            case GFX_POLYGON_SET_TRANSFORM_NODE:
                                                altSetNodePtr->inUse = 0;
                                                altSetNodePtr->submitted = 1;
                                                altSetNodePtr->payloadPtr = (int)curPtr + 0xC;
                                                break;
                                            case GFX_POLYGON_SET_TRANSFORM_NODE_CAMERA:
                                                altSetNodePtr->inUse = 1;
                                                ConcatAffineMatrix4x4((float(*)[4])((int)curPtr + 0xC), (float(*)[4])(engine->cameraMatrix), (float(*)[4])((int)altSetNodePtr + 0x14));
                                                ClearMatrixColumn4(altSetNodePtr->matrix);
                                                altSetNodePtr->submitted = 0;
                                                break;
                                            default:
                                                altSetNodePtr->inUse = 0;
                                                altSetNodePtr->submitted = 0;
                                            }
                                        }
                                    } else {
                                        switch (pSetHeader->kind) {
                                        case GFX_POLYGON_SET_TRANSFORM_NONE:
                                            mtxPointer = 0;
                                            break;
                                        case GFX_POLYGON_SET_TRANSFORM_NODE:
                                            mtxPointer = (int)curPtr + 0xC;
                                            break;
                                        case GFX_POLYGON_SET_TRANSFORM_NODE_CAMERA:
                                            ConcatAffineMatrix4x4((float(*)[4])((int)curPtr + 0xC), (float(*)[4])(engine->cameraMatrix), (float(*)[4])((int)&g_GfxSwMatrixScratch));
                                            mtxPointer = (int)&g_GfxSwMatrixScratch;
                                            break;
                                        default:
                                            mtxPointer = 0;
                                        }
                                        if (drawGroup->batchList != 0) {
                                            batchCur = drawGroup->batchList[idx];
                                            if (batchCur != 0) {
                                                if (curPtr->offsetEnabled != 0 || curPtr->offsetMode != 0) {
                                                    Gfx_ApplyDrawSpecToGroup(idx, drawGroup, (GfxDrawSpec *)((int)curPtr + 0x4C));
                                                    Gfx_RasterDrawIndexedDepthMesh(mtxPointer, batchCur, drawGroup, (GlDrawGroup *)glKind, rasterGroup, (SwRasterTransform *)((int)curPtr + 0x4C), engine);
                                                } else if (depthSort != 0) {
                                                    Gfx_EnqueueGroupForDepthSort(mtxPointer, 0, batchCur, (int)drawGroup, glKind, (int)rasterGroup, engine);
                                                } else {
                                                    Gfx_ApplyDrawSpecToGroup(idx, drawGroup, (GfxDrawSpec *)((int)curPtr + 0x4C));
                                                    if (drawGroup->drawUnsorted != 0 && mtxPointer != 0) {
                                                        Gfx_RasterDrawGroupWithMatrix(mtxPointer, batchCur, drawGroup, (GlDrawGroup *)glKind, rasterGroup, engine);
                                                    }
                                                }
                                            }
                                        }
                                    }
                                    curPtr = (SwBlitPolygonNode *)(curPtr->next);
                                } while (curPtr != 0);
                                break;
                            case GFX_DRAW_OP_DRAW_LIST:
                                if (drawGroup->batchList != 0) {
                                    batchCur = drawGroup->batchList[idx];
                                    if (batchCur != 0) {
                                        depthKey = batchCur->vertices;
                                        do {
                                            switch (pSetHeader->kind) {
                                            case GFX_POLYGON_SET_TRANSFORM_NONE:
                                                primXform = 0;
                                                break;
                                            case GFX_POLYGON_SET_TRANSFORM_NODE:
                                                primXform = (int)curPtr + 0xC;
                                                break;
                                            case GFX_POLYGON_SET_TRANSFORM_NODE_CAMERA:
                                                ConcatAffineMatrix4x4((float(*)[4])((int)curPtr + 0xC), (float(*)[4])(engine->cameraMatrix), (float(*)[4])((int)&g_GfxSwMatrixScratch));
                                                primXform = (int)&g_GfxSwMatrixScratch;
                                                break;
                                            default:
                                                primXform = 0;
                                            }
                                            pDrawEntry = (GfxDrawListNode *)(curPtr->drawNode);
                                            if (pDrawEntry->kind == 0x11) {
                                                depthKey = pDrawEntry->vertexCount;
                                            } else {
                                                Gfx_EmitPolygonSetPrimitive(depthSort, glKind, pDrawEntry);
                                            }
                                            if (depthSort != 0) {
                                                Gfx_EnqueuePsxPrimitiveForDepthSort(primXform, depthKey, (int)batchCur, glKind, pDrawEntry, engine);
                                            } else if (drawGroup->drawUnsorted != 0) {
                                                if (primXform != 0) {
                                                    Gfx_RasterDrawGroupWithMatrix(primXform, batchCur, drawGroup, (GlDrawGroup *)glKind, 0, engine);
                                                } else {
                                                    Gfx_RasterDrawGroupWithoutMatrix(batchCur, drawGroup, (GlDrawGroup *)glKind, rasterGroup, engine);
                                                }
                                            }
                                            curPtr = (SwBlitPolygonNode *)(curPtr->next);
                                        } while (curPtr != 0);
                                    }
                                }
                            }
                        }
                    }
                } else if (applyFlag != 0) {
                    pDeferredNode = (GfxPolygonSetNode *)(AppendBlockHeapNode(engine->nodeList));
                    if (pDeferredNode != 0) {
                        pDeferredNode->polyIndex = idx;
                        pDeferredNode->drawGroup = (int)drawGroup;
                        pDeferredNode->primGroupPtr = glKind;
                        if (drawGroup->drawSpecApplied != 0) {
                            pDeferredNode->stateBlock = *(GfxPolygonGroupDrawSpec *)(drawGroup->drawSpec);
                        }
                        cachedMatrix = GetCachedDeviceTransform(0, pPolySetRecord);
                        if (cachedMatrix != 0) {
                            pDeferredNode->inUse = 1;
                            *(GfxMatrix4x4 *)((int)pDeferredNode + 0x14) = *(GfxMatrix4x4 *)(cachedMatrix);
                            pDeferredNode->submitted = 0;
                        } else {
                            pDeferredNode->inUse = 0;
                            pDeferredNode->submitted = 0;
                        }
                    }
                } else if (drawGroup->batchList != 0) {
                    batchCur = drawGroup->batchList[idx];
                    if (batchCur != 0) {
                        if (depthSort != 0) {
                            Gfx_EnqueueGroupForDepthSort(pPolySetRecord->depthGroup, 0, batchCur, (int)drawGroup, glKind, (int)rasterGroup, engine);
                        } else if (drawGroup->drawUnsorted != 0 && pPolySetRecord->depthGroup != 0) {
                            Gfx_RasterDrawGroupWithMatrix(pPolySetRecord->depthGroup, batchCur, drawGroup, (GlDrawGroup *)glKind, rasterGroup, engine);
                        }
                    }
                }
            }
            if (glKind != 0) {
                glKind += 0x64;
            }
            if (rasterGroup != 0) {
                rasterGroup++;
            }
        }
    }
}

/*
 * Renders deferred polygon set node using cached transforms.
 */
// FUNCTION: FF7 0x00681c45
void __cdecl Blit_DrawDeferredNode(SwDeferredDrawNode *deferredNode, struct GfxEngineContext *engine)
{
    int mtx;
    GfxPolygonSet *pGroup;
    GfxPolygonBatch *batch;
    GlDrawGroup *glGroup;
    GfxRasterDrawGroup *pRasterDrawGroup;

    pGroup = (GfxPolygonSet *)(deferredNode->drawGroup);
    glGroup = (GlDrawGroup *)(deferredNode->glGroup);
    if (pGroup->batchList != 0) {
        batch = pGroup->batchList[deferredNode->recordIndex];
        if (batch != 0) {
            if (pGroup->polygonData->rasterGroup != 0) {
                pRasterDrawGroup = (GfxRasterDrawGroup *)(pGroup->polygonData->rasterGroup + deferredNode->recordIndex * 0x38);
            } else {
                pRasterDrawGroup = 0;
            }
            Blit_ApplyRenderSpec(glGroup, engine);
            if (deferredNode->mtx != 0) {
                mtx = (int)deferredNode + 0x14;
            }
            if (deferredNode[1].glGroup != 0) {
                mtx = deferredNode[1].mtx;
            }
            if (pGroup->drawUnsorted != 0) {
                Gfx_RasterDrawGroupWithMatrix(mtx, batch, pGroup, glGroup, pRasterDrawGroup, engine);
            }
        }
    }
}

/*
 * Blits dirty rects to primary surface and presents current frame.
 */
// FUNCTION: FF7 0x00681d13
void __cdecl Gfx_BlitContextRectsAndPresent(GfxEngineContext *context)
{
    int surfacePointer;
    GfxRenderState *pState;
    int width;
    int rectX;
    int rectY;
    int dstBase;
    int slot;
    GfxPixelFormat *rectCtx;
    int rows;
    SwBlitRectEntry *rectEnt;
    int clipBase;

    pState = (GfxRenderState *)GetRenderStateObject(context);
    if (pState != 0 && pState->dirty != 0) {
        if (DEAD_IN_RETAIL != 0) {
            surfacePointer = pState->surfaceB;
            Gfx_CopySurfaceRectDwords(100, 100, 0, 0, pState->surfaceA, surfacePointer, 100, 100, pState->surfaceA, surfacePointer);
        }
        if (GfxLockSurfaceCall(1) != 0) {
            rectCtx = &context->targetFormat;
            clipBase = rectCtx->pixels;
            dstBase = pState->surfaceB;
            if (context->blitRectsEnabled != 0) {
                for (slot = 0; slot < 2; slot++) {
                    rectEnt = &context->blitRects[slot];
                    if (rectEnt->active != 0) {
                        rectX = rectEnt->x;
                        rectY = rectEnt->y;
                        width = rectEnt->width;
                        rows = rectEnt->rows;
                        Gfx_SetContextBlitRect(slot, rectX, rectY, width, rows, context);
                        Mem_CopyDwordRows(width * rectCtx->bytesPerPixel, rows, pState->surfaceA - width * pState->rowOffset, rectCtx->pitch - width * rectCtx->bytesPerPixel, (void *)(dstBase + (rectX * pState->rowOffset + rectY * pState->surfaceA)), (void *)(clipBase + (rectX * rectCtx->bytesPerPixel + rectY * rectCtx->pitch)));
                    }
                }
            } else {
                rectX = context->clipLeft;
                rectY = context->clipTop;
                width = context->viewSizeX;
                rows = context->viewSizeY;
                Mem_CopyDwordRows(width * rectCtx->bytesPerPixel, rows, pState->surfaceA - width * pState->rowOffset, rectCtx->pitch - width * rectCtx->bytesPerPixel, (void *)(dstBase + (rectX * pState->rowOffset + rectY * pState->surfaceA)), (void *)(clipBase + (rectX * rectCtx->bytesPerPixel + rectY * rectCtx->pitch)));
            }
            GfxUnlockSurfaceCall(1);
        }
    }
    /* VC5 consumes one register choice without emitting code. */
    if (REG_ALLOC_HINT) {
    }
    GfxPresentBackBuffer(context);
}

/*
 * Retrieves surface dimensions from engine context.
 */
// FUNCTION: FF7 0x00681f6d
int __cdecl Blit_CopyContextSurfaceSize(void *sizeOut, struct GfxEngineContext *engine)
{
    SwSurfaceInfo *surfaceInformation;
    int res;

    res = 0;
    surfaceInformation = (SwSurfaceInfo *)(engine->rasterBase);
    if (surfaceInformation != 0) {
        ((BlitSurfaceSize *)sizeOut)->width = surfaceInformation->width;
        ((BlitSurfaceSize *)sizeOut)->height = surfaceInformation->height;
        res = 1;
    }
    return res;
}

/*
 * Initializes 44KB page-aligned stack scratch arena in engine context.
 */
// FUNCTION: FF7 0x00681fb1
void __cdecl InstallScratchArena(int arena, GfxEngineContext *engine)
{
    int alignedPtr;

    ZeroFillMemory32(0xAC30, (void *)(arena));
    alignedPtr = arena;
    alignedPtr = alignedPtr + 0x1000 & 0xFFFFF000;
    engine->rampBlock = alignedPtr;
}
} /* extern "C" */
