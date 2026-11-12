// src/lib/src/graphics/graphics_raster_triangle_dispatch.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Selects the rasterizer callback bundle from vertex and setup format flags, then stores
//   triangle vertices in the workspace and draws them, with a depth-mode variant.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/api/graphics.h"
#include "lib/types/graphics.h"
#include "lib/enums/graphics.h"
extern "C" {
// clang-format off
void __cdecl Gfx_RasterIndexedTextureSpanColorKeyDepthWrite(void);
void __cdecl Gfx_RasterIndexedTextureSpanDepthWrite(void);
void __cdecl Gfx_RasterDirectTextureSpanColorKeyDepthWrite(void);
void __cdecl Gfx_RasterDirectTextureSpanDepthWrite(void);
void __cdecl Gfx_RasterPaletteTexturedTriangleColorKey(void);
void __cdecl Gfx_RasterPaletteTexturedTriangleOpaque(void);
void __cdecl Gfx_RasterIndexedTextureSpanColorKeyBlend(void);
void __cdecl Gfx_RasterIndexedTextureSpanBlend(void);
void __cdecl Gfx_RasterDirectTextureSpanColorKeyBlend(void);
void __cdecl Gfx_RasterDirectTextureSpanBlend(void);
void __cdecl Gfx_RasterDirectTextureSpanColorKeyBlendDestRemap(void);
void __cdecl Gfx_RasterIndexedTextureSpanColorKeyBlendRemap(void);
void __cdecl Gfx_RasterIndexedTextureSpanBlendRemap(void);
void __cdecl Gfx_RasterDirectTextureSpanColorKeyBlendRemap(void);
void __cdecl Gfx_RasterDirectTextureSpanBlendRemap(void);
void __cdecl Gfx_RasterTriangleFlatZ(void);
void __cdecl Gfx_RasterTriangleFlatZTestBlend(void);
void __cdecl Gfx_RasterSpanRecolorDestination(void);
void __cdecl Gfx_RasterTriangleFlatZTestBlendLookup(void);
// GLOBAL: FF7 0x009179f0
int g_GfxRasterSetupTriangleCallbacks[10] = {(unsigned int)&Gfx_RasterSetupTriangleFlatColorDepth, (unsigned int)&Gfx_RasterSetupTriangleRgbGradientDepth, (unsigned int)&Gfx_RasterSetupTriangleVertexColorSumDepth, (unsigned int)&Gfx_RasterSetupTriangleRgbGradientDepth, (unsigned int)&Gfx_RasterSetupTriangleUvDepth, (unsigned int)&Gfx_RasterSetupTriangleFlatColorNoDepth, (unsigned int)&Gfx_RasterSetupTriangleRgbGradientNoDepth, (unsigned int)&Gfx_RasterSetupTriangleVertexColorSumNoDepth, (unsigned int)&Gfx_RasterSetupTriangleRgbGradientNoDepth, (unsigned int)&Gfx_RasterSetupTriangleUvNoDepth};
// GLOBAL: FF7 0x00917a18
int g_GfxRasterSetupEdgePairCallbacks[10] = {(unsigned int)&Gfx_RasterSetupEdgePairZ, (unsigned int)&Gfx_RasterSetupEdgePairZRgb, (unsigned int)&Gfx_RasterSetupEdgePairZ, (unsigned int)&Gfx_RasterSetupEdgePairZRgb, (unsigned int)&Gfx_RasterSetupEdgePairZUv, (unsigned int)&Gfx_RasterSetupEdgePair, (unsigned int)&Gfx_RasterSetupEdgePairRgb, (unsigned int)&Gfx_RasterSetupEdgePair, (unsigned int)&Gfx_RasterSetupEdgePairRgb, (unsigned int)&Gfx_RasterSetupEdgePairUv};
// GLOBAL: FF7 0x00917a40
int g_GfxRasterSetupEdgePairNextCallbacks[10] = {(unsigned int)&Gfx_RasterSetupEdgePairNextZ, (unsigned int)&Gfx_RasterSetupEdgePairNextZRgb, (unsigned int)&Gfx_RasterSetupEdgePairNextZ, (unsigned int)&Gfx_RasterSetupEdgePairNextZRgb, (unsigned int)&Gfx_RasterSetupEdgePairNextZUv, (unsigned int)&Gfx_RasterSetupEdgePairNext, (unsigned int)&Gfx_RasterSetupEdgePairNextZRgbSeeded, (unsigned int)&Gfx_RasterSetupEdgePairNext, (unsigned int)&Gfx_RasterSetupEdgePairNextZRgbSeeded, (unsigned int)&Gfx_RasterSetupEdgePairNextUv};
// GLOBAL: FF7 0x00917a68
int g_GfxRasterTextureSpanCallbacksMode4[8] = {(unsigned int)&Gfx_RasterIndexedTextureSpanColorKeyDepthWrite, (unsigned int)&Gfx_RasterIndexedTextureSpanDepthWrite, (unsigned int)&Gfx_RasterDirectTextureSpanColorKeyDepthWrite, (unsigned int)&Gfx_RasterDirectTextureSpanDepthWrite, (unsigned int)&Gfx_RasterPaletteTexturedTriangleColorKey, (unsigned int)&Gfx_RasterPaletteTexturedTriangleOpaque, (unsigned int)&Gfx_NoOpHandler, (unsigned int)&Gfx_NoOpHandler};
// Unreferenced Mode 4 alt callback table {Gfx_RasterIndexedTextureSpanColorKeyDepthWrite,
// Gfx_RasterIndexedTextureSpanOffsetTableDepthWrite, Gfx_NoOpHandler x2,
// Gfx_RasterPaletteTexturedTriangleColorKey, Gfx_RasterPaletteTexturedTriangleOpaque, Gfx_NoOpHandler x2}
// (kept as raw bytes so COFF emits no extra DIR32 relocations).
// clang-format off
unsigned char g_rawData_00917a88[32] = {
    0x4b, 0x9f, 0x6b, 0x00, 0x52, 0xa4, 0x6b, 0x00, 0x00, 0x92, 0x6b, 0x00, 0x00, 0x92, 0x6b, 0x00, 0xc8, 0x9d, 0x6b, 0x00, 0xd8, 0xa2, 0x6b, 0x00,
    0x00, 0x92, 0x6b, 0x00, 0x00, 0x92, 0x6b, 0x00
};
// clang-format on
// GLOBAL: FF7 0x00917aa8
int g_GfxRasterTextureSpanCallbacksMode0[8] = {(unsigned int)&Gfx_RasterIndexedTextureSpanColorKeyBlend, (unsigned int)&Gfx_RasterIndexedTextureSpanBlend, (unsigned int)&Gfx_RasterDirectTextureSpanColorKeyBlend, (unsigned int)&Gfx_RasterDirectTextureSpanBlend, (unsigned int)&Gfx_NoOpHandler, (unsigned int)&Gfx_NoOpHandler, (unsigned int)&Gfx_NoOpHandler, (unsigned int)&Gfx_NoOpHandler};
// GLOBAL: FF7 0x00917ac8
int g_GfxRasterTextureSpanCallbacksMode0Alt[8] = {(unsigned int)&Gfx_NoOpHandler, (unsigned int)&Gfx_NoOpHandler, (unsigned int)&Gfx_RasterDirectTextureSpanColorKeyBlendDestRemap, (unsigned int)&Gfx_NoOpHandler, (unsigned int)&Gfx_NoOpHandler, (unsigned int)&Gfx_NoOpHandler, (unsigned int)&Gfx_NoOpHandler, (unsigned int)&Gfx_NoOpHandler};
// GLOBAL: FF7 0x00917ae8
int g_GfxRasterTextureSpanCallbacksMode123[8] = {(unsigned int)&Gfx_RasterIndexedTextureSpanColorKeyBlendRemap, (unsigned int)&Gfx_RasterIndexedTextureSpanBlendRemap, (unsigned int)&Gfx_RasterDirectTextureSpanColorKeyBlendRemap, (unsigned int)&Gfx_RasterDirectTextureSpanBlendRemap, (unsigned int)&Gfx_NoOpHandler, (unsigned int)&Gfx_NoOpHandler, (unsigned int)&Gfx_NoOpHandler, (unsigned int)&Gfx_NoOpHandler};
// 10-entry depth-write flat/Gouraud triangle callback table:
// [0]=Gfx_RasterTriangleFlatZ, [1]=Gfx_RasterTriangleGouraudZ (0x00917b0c),
// [2]=Gfx_RasterTriangleFlatZ, [3]=Gfx_RasterTriangleRowShadedZ, [4]=Gfx_NoOpHandler,
// [5]=Gfx_RasterTriangleFlatNoDepth, [6]=Gfx_NoOpHandler, [7]=Gfx_RasterTriangleFlatNoDepth,
// [8..9]=Gfx_NoOpHandler (tail kept as raw bytes).
// GLOBAL: FF7 0x00917b08
int g_pGfxRasterTriangleFlatZ = (int)&Gfx_RasterTriangleFlatZ;
#define g_GfxRasterTriangleFlatZTable (&g_pGfxRasterTriangleFlatZ)
// clang-format off
unsigned char g_rawData_00917b0c[4] = {0xf8, 0x96, 0x6b, 0x00};
unsigned char g_rawData_00917b10[32] = {
    0x9b, 0x92, 0x6b, 0x00, 0x71, 0x95, 0x6b, 0x00, 0x00, 0x92, 0x6b, 0x00, 0x05, 0x92, 0x6b, 0x00, 0x00, 0x92, 0x6b, 0x00, 0x05, 0x92, 0x6b, 0x00,
    0x00, 0x92, 0x6b, 0x00, 0x00, 0x92, 0x6b, 0x00
};
// clang-format on
// 10-entry blend-mode-0 flat/Gouraud triangle callback table:
// [0]=Gfx_RasterTriangleFlatZTestBlend, [1]=Gfx_RasterGouraudSpanBlend (0x00917b34),
// [2]=Gfx_RasterTriangleFlatZTestBlend, [3..9]=Gfx_NoOpHandler (tail kept raw).
// GLOBAL: FF7 0x00917b30
int g_pGfxRasterTriangleFlatZTestBlend = (int)&Gfx_RasterTriangleFlatZTestBlend;
#define g_GfxRasterTriangleFlatZTestBlendTable (&g_pGfxRasterTriangleFlatZTestBlend)
// clang-format off
unsigned char g_rawData_00917b34[4] = {0x2e, 0x99, 0x6b, 0x00};
unsigned char g_rawData_00917b38[32] = {
    0x77, 0x93, 0x6b, 0x00, 0x00, 0x92, 0x6b, 0x00, 0x00, 0x92, 0x6b, 0x00, 0x00, 0x92, 0x6b, 0x00, 0x00, 0x92, 0x6b, 0x00, 0x00, 0x92, 0x6b, 0x00,
    0x00, 0x92, 0x6b, 0x00, 0x00, 0x92, 0x6b, 0x00
};
// clang-format on
// 10-entry blend-mode-0 alt recolor/Gouraud triangle callback table:
// [0]=Gfx_RasterSpanRecolorDestination, [1]=Gfx_RasterGouraudSpanBlend (0x00917b5c),
// [2]=Gfx_RasterTriangleFlatZTestBlend, [3..9]=Gfx_NoOpHandler (tail kept raw).
// GLOBAL: FF7 0x00917b58
int g_pGfxRasterSpanRecolorDestination = (int)&Gfx_RasterSpanRecolorDestination;
#define g_GfxRasterSpanRecolorDestinationTable (&g_pGfxRasterSpanRecolorDestination)
// clang-format off
unsigned char g_rawData_00917b5c[4] = {0x2e, 0x99, 0x6b, 0x00};
unsigned char g_rawData_00917b60[32] = {
    0x77, 0x93, 0x6b, 0x00, 0x00, 0x92, 0x6b, 0x00, 0x00, 0x92, 0x6b, 0x00, 0x00, 0x92, 0x6b, 0x00, 0x00, 0x92, 0x6b, 0x00, 0x00, 0x92, 0x6b, 0x00,
    0x00, 0x92, 0x6b, 0x00, 0x00, 0x92, 0x6b, 0x00
};
// clang-format on
// 10-entry blend-modes-1..3 lookup flat/Gouraud triangle callback table:
// [0]=Gfx_RasterTriangleFlatZTestBlendLookup, [1]=Gfx_RasterGouraudSpanBlendRemap (0x00917b84),
// [2]=Gfx_RasterTriangleFlatZTestBlendLookup, [3..9]=Gfx_NoOpHandler (tail kept raw).
// GLOBAL: FF7 0x00917b80
int g_pGfxRasterTriangleFlatZTestBlendLookup = (int)&Gfx_RasterTriangleFlatZTestBlendLookup;
#define g_GfxRasterTriangleFlatZTestBlendLookupTable (&g_pGfxRasterTriangleFlatZTestBlendLookup)
// clang-format off
unsigned char g_rawData_00917b84[4] = {0x76, 0x9b, 0x6b, 0x00};
unsigned char g_rawData_00917b88[32] = {
    0x6f, 0x94, 0x6b, 0x00, 0x00, 0x92, 0x6b, 0x00, 0x00, 0x92, 0x6b, 0x00, 0x00, 0x92, 0x6b, 0x00, 0x00, 0x92, 0x6b, 0x00, 0x00, 0x92, 0x6b, 0x00,
    0x00, 0x92, 0x6b, 0x00, 0x00, 0x92, 0x6b, 0x00
};
// clang-format on
// GLOBAL: FF7 0x00917ba8
int g_GfxRasterResolveVertexCallbacks[10] = {(unsigned int)&Gfx_RasterResolveVertexZ, (unsigned int)&Gfx_RasterResolveVertexZRgb, (unsigned int)&Gfx_RasterResolveVertexZ, (unsigned int)&Gfx_RasterResolveVertexZRgb, (unsigned int)&Gfx_RasterResolveVertexZUv, (unsigned int)&Gfx_RasterResolveVertexColorOnly, (unsigned int)&Gfx_RasterResolveVertexRgb, (unsigned int)&Gfx_RasterResolveVertexColorOnly, (unsigned int)&Gfx_RasterResolveVertexRgb, (unsigned int)&Gfx_RasterResolveVertexUv};
// GLOBAL: FF7 0x00917bd0
int g_GfxRasterPrepareVertexCallbacks[10] = {(unsigned int)&Gfx_RasterPrepareVertexZ, (unsigned int)&Gfx_RasterPrepareVertexZRgb, (unsigned int)&Gfx_RasterPrepareVertexZ, (unsigned int)&Gfx_RasterPrepareVertexZRgb, (unsigned int)&Gfx_RasterPrepareVertexZUv, (unsigned int)&Gfx_RasterPrepareVertexColorOnly, (unsigned int)&Gfx_RasterPrepareVertexRgb, (unsigned int)&Gfx_RasterPrepareVertexColorOnly, (unsigned int)&Gfx_RasterPrepareVertexRgb, (unsigned int)&Gfx_RasterPrepareVertexUv};
// GLOBAL: FF7 0x00917bf8
int g_GfxRasterSnapVertexCallbacks[10] = {(int)&Gfx_RasterSnapGeneratedVertexZ, (int)&Gfx_RasterSnapGeneratedVertexZRgb, (int)&Gfx_RasterSnapGeneratedVertexZRgbNodeColorRead, (int)&Gfx_RasterSnapGeneratedVertexZRgb, (int)&Gfx_RasterSnapGeneratedVertexRoundedZUv, (int)&Gfx_RasterSnapGeneratedVertexColorOnly, (int)&Gfx_RasterSnapGeneratedVertexRgb, (int)&Gfx_RasterSnapGeneratedVertexRgbNodeColorRead, (int)&Gfx_RasterSnapGeneratedVertexRgb, (int)&Gfx_RasterSnapGeneratedVertexRoundedUv};
// clang-format on

// Selects the rasterizer callback bundle in callbacks from flags and the setup format fields. A
// nonnull setup->resource forces setupIndex to 4. setupOfs is 0 when setup->generatedVertices is
// nonzero and 5 otherwise, and snapVertex is one of six callbacks, with flag 0x8000 taking
// precedence over 0x10000. The texture-state table index is built from the byte at +0xF,
// setup->generatedSpan and the nested resource word at +0x80/+0x10. The triangle, edge-pair and
// edge-pair-next setup callbacks are read at setupIndex plus setupOfs. The texture span callback
// comes from a table chosen by the blend mode setup->mode (the render state's blendMode, enum
// BlendMode; flag 0x100000 selects the alternate BLEND_AVERAGE table) and indexed by stateIndex.
// FUNCTION: FF7 0x006ab1d0
void __cdecl Gfx_RasterSelectGeneratedVertexCallbacks(int flags, GfxRasterGroupSetup *setup, GfxRasterVertexCallbacks *callbacks)
{
    int stateIndex;
    int setupOfs;
    int resource;

    resource = setup->resource;
    if (resource != 0) {
        callbacks->setupIndex = 4;
        if (setup->generatedVertices != 0) {
            setupOfs = 0;
            stateIndex = 0;
            if ((flags & 0x8000) != 0) {
                callbacks->snapVertex = (int)Gfx_RasterSnapGeneratedVertexRoundedZUvPreDelta;
            } else if ((flags & 0x10000) != 0) {
                callbacks->snapVertex = (int)Gfx_RasterSnapGeneratedVertexRoundedZUv;
            } else {
                callbacks->snapVertex = (int)Gfx_RasterSnapGeneratedVertexZUv;
            }
        } else {
            setupOfs = 5;
            stateIndex = 4;
            if ((flags & 0x8000) != 0) {
                callbacks->snapVertex = (int)Gfx_RasterSnapGeneratedVertexRoundedUvPreDelta;
            } else if ((flags & 0x10000) != 0) {
                callbacks->snapVertex = (int)Gfx_RasterSnapGeneratedVertexRoundedUv;
            } else {
                callbacks->snapVertex = (int)Gfx_RasterSnapGeneratedVertexUv;
            }
        }
        if (setup->generatedSpan != 0) {
            callbacks->generatedSpan = 1;
        } else {
            callbacks->generatedSpan = 0;
            stateIndex++;
        }
        if (*(int *)((char *)*(int *)((char *)resource + 0x80) + 0x10) != 0) {
            callbacks->generatedSetup = 1;
        } else {
            callbacks->generatedSetup = 0;
            stateIndex += 2;
        }
        callbacks->triangleSetup = g_GfxRasterSetupTriangleCallbacks[callbacks->setupIndex + setupOfs];
        callbacks->edgePairSetup = g_GfxRasterSetupEdgePairCallbacks[callbacks->setupIndex + setupOfs];
        callbacks->edgePairNextSetup = g_GfxRasterSetupEdgePairNextCallbacks[callbacks->setupIndex + setupOfs];
        switch (setup->mode) {
        case BLEND_AVERAGE:
            if ((flags & 0x100000) != 0) {
                callbacks->textureSpan = g_GfxRasterTextureSpanCallbacksMode0Alt[stateIndex];
            } else {
                callbacks->textureSpan = g_GfxRasterTextureSpanCallbacksMode0[stateIndex];
            }
            break;
        case BLEND_ADDITIVE:
            callbacks->textureSpan = g_GfxRasterTextureSpanCallbacksMode123[stateIndex];
            break;
        case BLEND_SUBTRACTIVE:
            callbacks->textureSpan = g_GfxRasterTextureSpanCallbacksMode123[stateIndex];
            break;
        case BLEND_QUARTER_ADDITIVE:
            callbacks->textureSpan = g_GfxRasterTextureSpanCallbacksMode123[stateIndex];
            break;
        case BLEND_NONE:
            callbacks->textureSpan = g_GfxRasterTextureSpanCallbacksMode4[stateIndex];
        }
    } else {
        callbacks->setupIndex = setup->variant;
        if (setup->variant == 1) {
            if ((flags & 1) != 0) {
                callbacks->setupIndex = 2;
            }
            if ((flags & 2) != 0) {
                callbacks->setupIndex = 3;
            }
        }
        if (setup->generatedVertices != 0) {
            setupOfs = 0;
        } else {
            setupOfs = 5;
        }
        stateIndex = callbacks->setupIndex;
        callbacks->triangleSetup = g_GfxRasterSetupTriangleCallbacks[callbacks->setupIndex + setupOfs];
        callbacks->edgePairSetup = g_GfxRasterSetupEdgePairCallbacks[callbacks->setupIndex + setupOfs];
        callbacks->edgePairNextSetup = g_GfxRasterSetupEdgePairNextCallbacks[callbacks->setupIndex + setupOfs];
        switch (setup->mode) {
        case BLEND_AVERAGE:
            if ((flags & 0x4000) != 0) {
                callbacks->textureSpan = g_GfxRasterSpanRecolorDestinationTable[stateIndex + setupOfs];
                if (callbacks->setupIndex == 0) {
                    callbacks->triangleSetup = (int)Gfx_RasterSetupTriangleFlatColorDepthAlphaRemap;
                }
            } else {
                callbacks->textureSpan = g_GfxRasterTriangleFlatZTestBlendTable[stateIndex + setupOfs];
            }
            break;
        case BLEND_ADDITIVE:
            callbacks->textureSpan = g_GfxRasterTriangleFlatZTestBlendLookupTable[stateIndex + setupOfs];
            break;
        case BLEND_SUBTRACTIVE:
            callbacks->textureSpan = g_GfxRasterTriangleFlatZTestBlendLookupTable[stateIndex + setupOfs];
            break;
        case BLEND_QUARTER_ADDITIVE:
            callbacks->textureSpan = g_GfxRasterTriangleFlatZTestBlendLookupTable[stateIndex + setupOfs];
            break;
        case BLEND_NONE:
            callbacks->textureSpan = g_GfxRasterTriangleFlatZTable[stateIndex + setupOfs];
        }
        callbacks->snapVertex = g_GfxRasterSnapVertexCallbacks[callbacks->setupIndex + setupOfs];
    }
    callbacks->resolveVertex = g_GfxRasterResolveVertexCallbacks[callbacks->setupIndex + setupOfs];
    callbacks->prepareVertex = g_GfxRasterPrepareVertexCallbacks[callbacks->setupIndex + setupOfs];
}

// Stores the three vertex pointers in the context's edge chain, then finds the minimum and maximum
// x and y of the three vertices. It remembers which vertices have the lowest and highest y (minVert
// and maxIdx). For strictly positive x and y extents it stores the x extent as spanWidth, calls the
// callback at callbacks+0xC, and then prepares the edges with Gfx_RasterBuildTriangleEdgeChains.
// FUNCTION: FF7 0x006ab584
void __cdecl Gfx_RasterDrawTriangle(GfxWireEndVertex *vertexA, GfxWireEndVertex *vertexB, GfxWireEndVertex *vertexC, GfxRasterCallbacks *callbacks, GfxRasterTriangleContext *context)
{
    int scratch;
    int spanY;
    int minX;
    int minY;
    int maxX;
    int status;
    int xSpan;
    int minVert;
    GfxRasterEdgeChain *pChain;
    int maxY;
    int target;
    int maxIdx;

    scratch = (int)context->scratch;
    pChain = &context->edgeChain;
    target = context->target;
    pChain->vertexA = (int)vertexA;
    pChain->vertexBWord = (int)vertexB;
    pChain->vertexC = (int)vertexC;
    minX = vertexA->x;
    minY = vertexA->y;
    maxX = vertexA->x;
    maxY = vertexA->y;
    minVert = 0;
    maxIdx = 0;
    if (vertexB->x < minX) {
        minX = vertexB->x;
    }
    if (vertexB->x > maxX) {
        maxX = vertexB->x;
    }
    if (vertexC->x < minX) {
        minX = vertexC->x;
    }
    if (vertexC->x > maxX) {
        maxX = vertexC->x;
    }
    if (vertexB->y < minY) {
        minY = vertexB->y;
        minVert = 1;
    }
    if (vertexB->y > maxY) {
        maxY = vertexB->y;
        maxIdx = 1;
    }
    if (vertexC->y < minY) {
        minY = vertexC->y;
        minVert = 2;
    }
    if (vertexC->y > maxY) {
        maxY = vertexC->y;
        maxIdx = 2;
    }
    xSpan = maxX - minX;
    if (xSpan > 0) {
        spanY = maxY - minY;
        if (spanY > 0) {
            pChain->spanWidth = xSpan;
            callbacks->prepareCallback(scratch, target, (int)pChain);
            Gfx_RasterBuildTriangleEdgeChains(minVert, maxIdx, pChain, (GfxRasterIntermediates *)context);
            if (pChain->active > 0) {
                status = callbacks->stepCallback(target, (int)pChain, scratch);
                callbacks->releaseCallback(target);
                if (status != 0) {
                    callbacks->finishCallback(status, target, (int)pChain, scratch);
                    callbacks->releaseCallback(target);
                }
            }
        }
    }
}

/*
 * Stores three vertex pointers in the raster workspace and, for mode byte 2,
 * replaces their depth with the configured depth multiplied by the context scale.
 * Finds X and Y extrema, rejects nonpositive extents, stores width and calls the
 * setup callback.
 */
// FUNCTION: FF7 0x006ab76f
void __cdecl Gfx_RasterDrawTriangleDepth(SwRasterVertexSource *vertexA, GfxTriangleDepthRange *vertexB, GfxTriangleDepthRange *vertexC, GfxRasterCallbacks *callbacks, GfxRasterTriangleContext *context)
{
    int scratch;
    int spanY;
    int widthExtent;
    int status;
    int minVert;
    GfxRasterEdgeChain *pChain;
    int maxY;
    int xMax;
    int depthState;
    int depthSample;
    int target;
    int maxIdx;
    int minX;
    int minY;
    GfxRasterDepthConfig *config;

    config = (GfxRasterDepthConfig *)(callbacks->depthConfig);
    scratch = (int)context->scratch;
    pChain = &context->edgeChain;
    depthState = context->depthState;
    target = context->target;
    pChain->vertexA = (int)vertexA;
    pChain->vertexBWord = (int)vertexB;
    pChain->vertexC = (int)vertexC;
    switch (config->mode) {
    case GFX_MESH_DEPTH_KEEP:
        break;
    case GFX_MESH_DEPTH_REPLACE:
        depthSample = config->depthSample;
        *(float *)&depthSample *= *(float *)(depthState + 0x38);
        vertexA->depth = depthSample;
        vertexB->depthSample = depthSample;
        vertexC->depthSample = depthSample;
    }
    minX = vertexA->screenX;
    minY = vertexA->screenY;
    xMax = vertexA->screenX;
    maxY = vertexA->screenY;
    minVert = 0;
    maxIdx = 0;
    if (vertexB->x < minX) {
        minX = vertexB->x;
    }
    if (vertexB->x > xMax) {
        xMax = vertexB->x;
    }
    if (vertexC->x < minX) {
        minX = vertexC->x;
    }
    if (vertexC->x > xMax) {
        xMax = vertexC->x;
    }
    if (vertexB->y < minY) {
        minY = vertexB->y;
        minVert = 1;
    }
    if (vertexB->y > maxY) {
        maxY = vertexB->y;
        maxIdx = 1;
    }
    if (vertexC->y < minY) {
        minY = vertexC->y;
        minVert = 2;
    }
    if (vertexC->y > maxY) {
        maxY = vertexC->y;
        maxIdx = 2;
    }
    widthExtent = xMax - minX;
    if (widthExtent > 0) {
        spanY = maxY - minY;
        if (spanY > 0) {
            pChain->spanWidth = widthExtent;
            callbacks->prepareCallback(scratch, target, (int)pChain);
            Gfx_RasterBuildTriangleEdgeChains(minVert, maxIdx, pChain, (GfxRasterIntermediates *)context);
            if (pChain->active > 0) {
                status = callbacks->stepCallback(target, (int)pChain, scratch);
                callbacks->releaseCallback(target);
                if (status != 0) {
                    callbacks->finishCallback(status, target, (int)pChain, scratch);
                    callbacks->releaseCallback(target);
                }
            }
        }
    }
}
} /* extern "C" */
