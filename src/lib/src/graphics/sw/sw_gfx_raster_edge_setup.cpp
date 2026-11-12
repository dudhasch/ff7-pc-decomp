// src/lib/src/graphics/sw/sw_gfx_raster_edge_setup.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Triangle and edge setup for the software rasterizer: builds the two edge chains, fills
//   the render setup block, and primes edge pairs and triangle interpolators for flat colour,
//   vertex colour, UV and RGB gradient draws.
/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/types/graphics.h"
#include "lib/api/graphics.h"
extern "C" {

/* Unprototyped retail call signature (parameter/return width differs from callee definition). */
void __cdecl crt_FloatToInt64(int value, void *out);

// Builds two triangle edge chains from the intermediate vertex index in intermediates->midTable,
// indexed by vertexA + 4 * vertexB. It compares the vertexKey of vertex B with the pivot's vertex
// key to choose one or two segments for each chain, and records the start and end vertex indices.
// Each segment's slope is computed by Gfx_RasterComputeEdgeSlope into the edge record that starts
// at chain->startVertex, secondStartVertex, midVertexIndex or secondMidVertexIndex. When the vertex
// keys are equal, sortKey decides: equal sort
// keys clear the chain, and a smaller sortKey on vertex A starts the chain at A and ends it at B.
// FUNCTION: FF7 0x006ad110
void __cdecl Gfx_RasterBuildTriangleEdgeChains(int vertexA, int vertexB, GfxRasterEdgeChain *chain, GfxRasterIntermediates *intermediates)
{
    int midIdx;
    SwChainVertex *vertA;
    SwChainVertex *pivotVert;
    SwChainVertex *vertB;
    int midA;
    int midB;

    vertA = (SwChainVertex *)(((int *)chain)[vertexA]);
    vertB = (SwChainVertex *)(((int *)chain)[vertexB]);
    midIdx = intermediates->midTable[vertexA + vertexB * 4];
    pivotVert = (SwChainVertex *)(((int *)chain)[midIdx]);
    if (vertA->vertexKey != pivotVert->vertexKey) {
        if (vertB->vertexKey == pivotVert->vertexKey) {
            midA = intermediates->midA[vertexA];
            midB = intermediates->midB[vertexA];
            chain->active = 1;
            chain->startVertex = vertexA;
            chain->endVertex = midA;
            Gfx_RasterComputeEdgeSlope(vertexA, midA, (GfxRasterEdgeSlope *)&chain->startVertex, (void **)chain, intermediates);
            chain->edgeState = 1;
            chain->midVertexIndex = vertexA;
            chain->vertexB = midB;
            Gfx_RasterComputeEdgeSlope(vertexA, midB, (GfxRasterEdgeSlope *)&chain->midVertexIndex, (void **)chain, intermediates);
        } else {
            midA = intermediates->midA[vertexA];
            if (midIdx == midA) {
                chain->active = 2;
                chain->startVertex = vertexA;
                chain->endVertex = midIdx;
                chain->secondStartVertex = midIdx;
                chain->secondEndVertex = vertexB;
                Gfx_RasterComputeEdgeSlope(vertexA, midIdx, (GfxRasterEdgeSlope *)&chain->startVertex, (void **)chain, intermediates);
                Gfx_RasterComputeEdgeSlope(midIdx, vertexB, (GfxRasterEdgeSlope *)&chain->secondStartVertex, (void **)chain, intermediates);
                chain->edgeState = 1;
                chain->midVertexIndex = vertexA;
                chain->vertexB = vertexB;
                Gfx_RasterComputeEdgeSlope(vertexA, vertexB, (GfxRasterEdgeSlope *)&chain->midVertexIndex, (void **)chain, intermediates);
            } else {
                chain->active = 1;
                chain->startVertex = vertexA;
                chain->endVertex = vertexB;
                Gfx_RasterComputeEdgeSlope(vertexA, vertexB, (GfxRasterEdgeSlope *)&chain->startVertex, (void **)chain, intermediates);
                chain->edgeState = 2;
                chain->midVertexIndex = vertexA;
                chain->vertexB = midIdx;
                chain->secondMidVertexIndex = midIdx;
                chain->secondVertexB = vertexB;
                Gfx_RasterComputeEdgeSlope(vertexA, midIdx, (GfxRasterEdgeSlope *)&chain->midVertexIndex, (void **)chain, intermediates);
                Gfx_RasterComputeEdgeSlope(midIdx, vertexB, (GfxRasterEdgeSlope *)&chain->secondMidVertexIndex, (void **)chain, intermediates);
            }
        }
    } else if (vertA->sortKey == pivotVert->sortKey) {
        chain->active = 0;
        chain->edgeState = 0;
    } else if (vertA->sortKey < pivotVert->sortKey) {
        chain->active = 1;
        chain->startVertex = vertexA;
        chain->endVertex = vertexB;
        Gfx_RasterComputeEdgeSlope(vertexA, vertexB, (GfxRasterEdgeSlope *)&chain->startVertex, (void **)chain, intermediates);
        chain->edgeState = 1;
        chain->midVertexIndex = midIdx;
        chain->vertexB = vertexB;
        Gfx_RasterComputeEdgeSlope(midIdx, vertexB, (GfxRasterEdgeSlope *)&chain->midVertexIndex, (void **)chain, intermediates);
    } else {
        chain->active = 1;
        chain->startVertex = midIdx;
        chain->endVertex = vertexB;
        Gfx_RasterComputeEdgeSlope(midIdx, vertexB, (GfxRasterEdgeSlope *)&chain->startVertex, (void **)chain, intermediates);
        chain->edgeState = 1;
        chain->midVertexIndex = vertexA;
        chain->vertexB = vertexB;
        Gfx_RasterComputeEdgeSlope(vertexA, vertexB, (GfxRasterEdgeSlope *)&chain->midVertexIndex, (void **)chain, intermediates);
    }
}

/*
 * Gfx_RasterFillRenderSetupBlock Fills the render setup block setupBlock from the
 * context engine and from the object returned by GetRenderStateObject(engine).
 * When that object is non-null, setupBlock->scanSize and setupBlock->pixels come from its
 * rowBytes and block if either usesAlignedHeap or allocatePixelBlock is set (the two branches
 * store the same thing) and from engine->targetFormat.pitch and .pixels otherwise.
 */
// FUNCTION: FF7 0x006ad4a4
void __cdecl Gfx_RasterFillRenderSetupBlock(GfxRasterSetupBlock *setupBlock, GfxEngineContext *engine)
{
    SwRasterizerState *stateObj;

    stateObj = (SwRasterizerState *)GetRenderStateObject(engine);
    if (stateObj != 0) {
        if (stateObj->usesAlignedHeap != 0) {
            setupBlock->scanSize = stateObj->rowBytes;
            setupBlock->pixels = stateObj->block;
        } else if (stateObj->allocatePixelBlock != 0) {
            setupBlock->scanSize = stateObj->rowBytes;
            setupBlock->pixels = stateObj->block;
        } else {
            setupBlock->scanSize = engine->targetFormat.pitch;
            setupBlock->pixels = engine->targetFormat.pixels;
        }
    }
    setupBlock->rampTableR = engine->rampTableR;
    setupBlock->rampTableG = engine->rampTableG;
    setupBlock->rampTableB = engine->rampTableB;
    setupBlock->divideByThreeTable = (int)stateObj + 0x45FC;
    setupBlock->dxTableStart = (int)stateObj + 0x3BF8;
    setupBlock->pixelToRgbTable = (int)stateObj + 0x48FC;
    setupBlock->alphaTable = (int)stateObj + 0x448FC;
}

// Mechanical copy: stateA and stateB are copied from the source pair into the destination pair,
// with no other logic.
// FUNCTION: FF7 0x006ad5a1
void __cdecl Gfx_RasterCopyStatePair(GfxRasterStatePairIn *src, GfxRasterStatePairOut *dst)
{
    dst->stateA = src->stateA;
    dst->stateB = src->stateB;
}

/*
 * Gfx_RasterMapPackedColorTables Maps three packed-color bytes through a table
 * bank selected by alpha, combines three mapped- channel table words into
 * packedColor and XORs the engine alpha mask into that word. Stores 0xFF minus
 * alpha in invAlpha.
 */
// FUNCTION: FF7 0x006ad5be
void __cdecl Gfx_RasterMapPackedColorTables(SwRasterState *state)
{
    char *palRow;
    GfxEngineContext *engine;

    palRow = state->paletteBase + (state->alpha << 8);
    state->lutIndexE = palRow[state->lutIndexE];
    state->lutIndexD = palRow[state->lutIndexD];
    state->lutIndexC = palRow[state->lutIndexC];
    state->packedColor = state->lutE[state->lutIndexE] | state->lutD[state->lutIndexD] | state->lutC[state->lutIndexC];
    engine = GetEngineContext();
    state->packedColor ^= engine->targetFormat.alphaMask;
    state->invAlpha = 0xFF - state->alpha;
}

/*
 * Gfx_RasterSetupTriangleFlatColorNoDepth Stores into dst->packedColor the result of
 * Blit_MapArgb8888ViaRampTables(the argbColor of the first vertex of vertices,
 * GetEngineContext()). The `unused` parameter is ignored.
 */
// FUNCTION: FF7 0x006ad6b4
void __cdecl Gfx_RasterSetupTriangleFlatColorNoDepth(int unused, SwRasterState *dst, GfxTriangleVertexSet *vertices)
{
    dst->packedColor = Blit_MapArgb8888ViaRampTables(*(GfxArgb32 *)&((SwFlatTriangleVertex *)vertices->vertexA)->argbColor, GetEngineContext());
}

/*
 * Gfx_RasterSetupTriangleFlatColorDepth Initializes triangle interpolation
 * data from three supplied vertices, maps the first packed color through the
 * engine ramp tables and computes two interpolation coefficients from vertex
 * depth differences. Calls the retained float-to-integer helper with the first
 * coefficient and dst->depthStep.
 */
// FUNCTION: FF7 0x006ad6d6
void __cdecl Gfx_RasterSetupTriangleFlatColorDepth(SwTriangleGradients *gradients, SwRasterState *dst, GfxTriangleVertexSet *vertices)
{
    float depthDeltaA;
    SwFlatTriangleVertex *vertC;
    SwFlatTriangleVertex *vertexA;
    float deltaB;
    SwFlatTriangleVertex *vertexB;

    vertexA = (SwFlatTriangleVertex *)(vertices->vertexA);
    vertexB = (SwFlatTriangleVertex *)(vertices->vertexB);
    vertC = (SwFlatTriangleVertex *)(vertices->vertexC);
    Gfx_RasterTriangleGradientSetup((float *)vertexA, (float *)vertexB, (float *)vertC, (GfxRasterUvDepthGradients *)gradients);
    dst->packedColor = Blit_MapArgb8888ViaRampTables(*(GfxArgb32 *)&vertexA->argbColor, GetEngineContext());
    depthDeltaA = vertexB->depth - vertexA->depth;
    deltaB = vertC->depth - vertexA->depth;
    gradients->depthGradX = Gfx_RasterTriangleGradientX(depthDeltaA, deltaB, (SwGradientMatrixX *)gradients);
    gradients->depthGradY = Gfx_RasterTriangleGradientY(depthDeltaA, deltaB, (SwGradientMatrixY *)gradients);
    crt_FloatToInt64(*(int *)&gradients->depthGradX, dst->depthStep);
}

/*
 * Gfx_RasterSetupTriangleFlatColorDepthAlphaRemap Initializes triangle
 * interpolation data, maps the first packed color and computes the two depth
 * interpolation coefficients, then calls the retained float-to-integer helper
 * for dst->depthStep. Copies the first packed color into dst->color
 * and invokes the destination setup helper.
 */
// FUNCTION: FF7 0x006ad795
void __cdecl Gfx_RasterSetupTriangleFlatColorDepthAlphaRemap(SwTriangleGradients *gradients, SwRasterState *dst, GfxTriangleVertexSet *vertices)
{
    float depthDeltaA;
    SwFlatColorVertex *vertC;
    SwFlatColorVertex *vertexA;
    float deltaB;
    SwFlatColorVertex *vertexB;

    vertexA = (SwFlatColorVertex *)(vertices->vertexA);
    vertexB = (SwFlatColorVertex *)(vertices->vertexB);
    vertC = (SwFlatColorVertex *)(vertices->vertexC);
    Gfx_RasterTriangleGradientSetup((float *)vertexA, (float *)vertexB, (float *)vertC, (GfxRasterUvDepthGradients *)gradients);
    dst->packedColor = Blit_MapArgb8888ViaRampTables(*(GfxArgb32 *)&vertexA->color, GetEngineContext());
    depthDeltaA = vertexB->depth - vertexA->depth;
    deltaB = vertC->depth - vertexA->depth;
    gradients->depthGradX = Gfx_RasterTriangleGradientX(depthDeltaA, deltaB, (SwGradientMatrixX *)gradients);
    gradients->depthGradY = Gfx_RasterTriangleGradientY(depthDeltaA, deltaB, (SwGradientMatrixY *)gradients);
    crt_FloatToInt64(*(int *)&gradients->depthGradX, dst->depthStep);
    dst->color = vertexA->color;
    Gfx_RasterMapPackedColorTables(dst);
}

/*
 * Gfx_RasterSetupTriangleVertexColorSumNoDepth Sums each of three packed-color
 * byte components across the supplied vertices, maps each sum through the
 * destination lookup table and builds a local packed color with alpha 0xFF.
 * Maps that color through the engine ramp tables into dest->packedColor; unused
 * is unused.
 */
// FUNCTION: FF7 0x006ad86f
void __cdecl Gfx_RasterSetupTriangleVertexColorSumNoDepth(int unused, SwRasterState *dest, GfxTriangleVertexSet *vertices)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[16];
        SwColorSumVertexB *vertexB;
        struct { unsigned char pad_to_4[0x4]; SwColorSumVertexA *vertexA; };
        struct { unsigned char pad_to_8[0x8]; SwColorSumVertexB *vertexC; };
        struct { unsigned char pad_to_C[0xC]; int mapped0; };
        struct { unsigned char pad_to_D[0xD]; char mapped1; };
        struct { unsigned char pad_to_E[0xE]; char mapped2; };
        struct { unsigned char pad_to_F[0xF]; char alphaByte; };
    } frame;
#pragma pack(pop)

    frame.vertexA = (SwColorSumVertexA *)(vertices->vertexA);
    frame.vertexB = (SwColorSumVertexB *)(vertices->vertexB);
    frame.vertexC = (SwColorSumVertexB *)(vertices->vertexC);
    frame.mapped2 = dest->rampTable[frame.vertexA->colorByte2 + frame.vertexB->colorChannel3 + frame.vertexC->colorChannel3];
    frame.mapped1 = dest->rampTable[frame.vertexA->colorByte1 + frame.vertexB->colorChannel2 + frame.vertexC->colorChannel2];
    *(char *)&frame.mapped0 = dest->rampTable[frame.vertexA->colorByte0 + frame.vertexB->colorChannel1 + frame.vertexC->colorChannel1];
    frame.alphaByte = 0xFF;
    dest->packedColor = Blit_MapArgb8888ViaRampTables(*(GfxArgb32 *)&frame.mapped0, GetEngineContext());
}

// Reads three vertex pointers from vertices and calls Gfx_RasterTriangleGradientSetup to prepare
// the gradients. For each color channel it sums that channel's byte across the three vertices, maps
// each sum through the byte table at dest->rampTable, and packs the results with opaque alpha.
// Blit_MapArgb8888ViaRampTables maps the packed color into dest->packedColor. It then computes the
// X and Y depth gradients from the vertices' depth differences and stores the depth step with
// crt_FloatToInt64 in dest->depthStep.
// FUNCTION: FF7 0x006ad930
void __cdecl Gfx_RasterSetupTriangleVertexColorSumDepth(SwTriangleGradients *gradients, SwRasterState *dest, GfxTriangleVertexSet *vertices)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[24];
        SwColorSumVertexB *vertexB;
        struct { unsigned char pad_to_4[0x4]; float depthDeltaY; };
        struct { unsigned char pad_to_8[0x8]; SwRasterVertexColorSum *vertexA; };
        struct { unsigned char pad_to_C[0xC]; SwColorSumVertexB *vertexC; };
        struct { unsigned char pad_to_10[0x10]; float depthDeltaX; };
        struct { unsigned char pad_to_14[0x14]; int packedArgb; };
        struct { unsigned char pad_to_15[0x15]; char greenByte; };
        struct { unsigned char pad_to_16[0x16]; char blueByte; };
        struct { unsigned char pad_to_17[0x17]; char alphaByte; };
    } frame;
#pragma pack(pop)

    frame.vertexA = (SwRasterVertexColorSum *)(vertices->vertexA);
    frame.vertexB = (SwColorSumVertexB *)(vertices->vertexB);
    frame.vertexC = (SwColorSumVertexB *)(vertices->vertexC);
    Gfx_RasterTriangleGradientSetup((float *)frame.vertexA, (float *)frame.vertexB, (float *)frame.vertexC, (GfxRasterUvDepthGradients *)gradients);
    frame.blueByte = dest->rampTable[frame.vertexA->colorB + frame.vertexB->colorChannel3 + frame.vertexC->colorChannel3];
    frame.greenByte = dest->rampTable[frame.vertexA->colorG + frame.vertexB->colorChannel2 + frame.vertexC->colorChannel2];
    *(char *)&frame.packedArgb = dest->rampTable[frame.vertexA->colorR + frame.vertexB->colorChannel1 + frame.vertexC->colorChannel1];
    frame.alphaByte = 0xFF;
    dest->packedColor = Blit_MapArgb8888ViaRampTables(*(GfxArgb32 *)&frame.packedArgb, GetEngineContext());
    frame.depthDeltaX = frame.vertexB->depth - frame.vertexA->depth;
    frame.depthDeltaY = frame.vertexC->depth - frame.vertexA->depth;
    gradients->depthGradX = Gfx_RasterTriangleGradientX(frame.depthDeltaX, frame.depthDeltaY, (SwGradientMatrixX *)gradients);
    gradients->depthGradY = Gfx_RasterTriangleGradientY(frame.depthDeltaX, frame.depthDeltaY, (SwGradientMatrixY *)gradients);
    crt_FloatToInt64(*(int *)&gradients->depthGradX, dest->depthStep);
}

// Initializes the triangle interpolation data and computes X and Y gradients for the texture U and
// V differences of vertices B and C against vertex A. Calls the retained float-to-integer helper
// crt_FloatToInt64 for dest->uStep and dest->vStep, then copies the triangle's mappedColor into
// dest->color.
// FUNCTION: FF7 0x006ada71
void __cdecl Gfx_RasterSetupTriangleUvNoDepth(SwTriangleGradients *gradients, SwRasterState *dest, SwUvTriangle *vertices)
{
    SwUvVertex *vertC;
    float deltaA;
    float diffStep;
    SwUvVertex *vertexA;
    SwUvVertex *vertexB;

    vertexA = (SwUvVertex *)(vertices->vertexA);
    vertexB = (SwUvVertex *)(vertices->vertexB);
    vertC = (SwUvVertex *)(vertices->vertexC);
    Gfx_RasterTriangleGradientSetup((float *)vertexA, (float *)vertexB, (float *)vertC, (GfxRasterUvDepthGradients *)gradients);
    deltaA = vertexB->u - vertexA->u;
    diffStep = vertC->u - vertexA->u;
    gradients->uGradX = Gfx_RasterTriangleGradientX(deltaA, diffStep, (SwGradientMatrixX *)gradients);
    gradients->uGradY = Gfx_RasterTriangleGradientY(deltaA, diffStep, (SwGradientMatrixY *)gradients);
    deltaA = vertexB->v - vertexA->v;
    diffStep = vertC->v - vertexA->v;
    gradients->vGradX = Gfx_RasterTriangleGradientX(deltaA, diffStep, (SwGradientMatrixX *)gradients);
    gradients->vGradY = Gfx_RasterTriangleGradientY(deltaA, diffStep, (SwGradientMatrixY *)gradients);
    crt_FloatToInt64(*(int *)&gradients->uGradX, dest->uStep);
    crt_FloatToInt64(*(int *)&gradients->vGradX, dest->vStep);
    dest->color = vertices->mappedColor;
}

// Initializes the triangle interpolation data and computes X and Y gradients for the depth
// differences of vertices B and C against vertex A, and for their texCoordU and texCoordV
// differences (stored in uGradX/uGradY and vGradX/vGradY). Calls the retained float-to-integer
// helper crt_FloatToInt64 for the depth step in dest->depthStep and for uGradX and vGradX in
// dest->uStep and dest->vStep. Then copies the triangle's mappedColor into dest->color.
// FUNCTION: FF7 0x006adb92
void __cdecl Gfx_RasterSetupTriangleUvDepth(SwTriangleGradients *gradients, SwRasterState *dest, SwUvTriangle *vertices)
{
    SwRasterVertex *vertC;
    float deltaA;
    float diffStep;
    SwRasterVertex *vertexA;
    SwRasterVertex *vertexB;

    vertexA = (SwRasterVertex *)(vertices->vertexA);
    vertexB = (SwRasterVertex *)(vertices->vertexB);
    vertC = (SwRasterVertex *)(vertices->vertexC);
    Gfx_RasterTriangleGradientSetup((float *)vertexA, (float *)vertexB, (float *)vertC, (GfxRasterUvDepthGradients *)gradients);
    deltaA = vertexB->depth - vertexA->depth;
    diffStep = vertC->depth - vertexA->depth;
    gradients->depthGradX = Gfx_RasterTriangleGradientX(deltaA, diffStep, (SwGradientMatrixX *)gradients);
    gradients->depthGradY = Gfx_RasterTriangleGradientY(deltaA, diffStep, (SwGradientMatrixY *)gradients);
    crt_FloatToInt64(*(int *)&gradients->depthGradX, dest->depthStep);
    deltaA = vertexB->texCoordU - vertexA->texCoordU;
    diffStep = vertC->texCoordU - vertexA->texCoordU;
    gradients->uGradX = Gfx_RasterTriangleGradientX(deltaA, diffStep, (SwGradientMatrixX *)gradients);
    gradients->uGradY = Gfx_RasterTriangleGradientY(deltaA, diffStep, (SwGradientMatrixY *)gradients);
    deltaA = vertexB->texCoordV - vertexA->texCoordV;
    diffStep = vertC->texCoordV - vertexA->texCoordV;
    gradients->vGradX = Gfx_RasterTriangleGradientX(deltaA, diffStep, (SwGradientMatrixX *)gradients);
    gradients->vGradY = Gfx_RasterTriangleGradientY(deltaA, diffStep, (SwGradientMatrixY *)gradients);
    crt_FloatToInt64(*(int *)&gradients->uGradX, dest->uStep);
    crt_FloatToInt64(*(int *)&gradients->vGradX, dest->vStep);
    dest->color = vertices->mappedColor;
}

// Initializes the triangle interpolation data and computes X and Y gradients for depth and for
// texCoordU and texCoordV, storing each set in the gradients struct. Passes the bit pattern of
// gradientX0 to the retained conversion helper crt_FloatToInt64 for dest->depthStep, and of
// gradientX1 and gradientX2 for dest->uStep and dest->vStep. Runs Gfx_RasterStepDdaSpanOutputs with
// the triangle's spanWidth and dest->output, then copies the triangle's mappedColor into
// dest->color.
// FUNCTION: FF7 0x006add1b
void __cdecl Gfx_RasterSetupTriangleUvDepthSpanTable(GfxRasterUvDepthGradients *gradients, SwRasterState *dest, SwRasterTriangle *vertices)
{
    SwRasterVertex *vertC;
    float deltaA;
    float diffStep;
    SwRasterVertex *vertexA;
    SwRasterVertex *vertexB;

    vertexA = (SwRasterVertex *)(vertices->vertexA);
    vertexB = (SwRasterVertex *)(vertices->vertexB);
    vertC = (SwRasterVertex *)(vertices->vertexC);
    Gfx_RasterTriangleGradientSetup((float *)vertexA, (float *)vertexB, (float *)vertC, gradients);
    deltaA = vertexB->depth - vertexA->depth;
    diffStep = vertC->depth - vertexA->depth;
    gradients->gradientX0 = Gfx_RasterTriangleGradientX(deltaA, diffStep, (SwGradientMatrixX *)gradients);
    gradients->gradientY0 = Gfx_RasterTriangleGradientY(deltaA, diffStep, (SwGradientMatrixY *)gradients);
    crt_FloatToInt64(*(int *)&gradients->gradientX0, dest->depthStep);
    deltaA = vertexB->texCoordU - vertexA->texCoordU;
    diffStep = vertC->texCoordU - vertexA->texCoordU;
    gradients->gradientX1 = Gfx_RasterTriangleGradientX(deltaA, diffStep, (SwGradientMatrixX *)gradients);
    gradients->gradientY1 = Gfx_RasterTriangleGradientY(deltaA, diffStep, (SwGradientMatrixY *)gradients);
    deltaA = vertexB->texCoordV - vertexA->texCoordV;
    diffStep = vertC->texCoordV - vertexA->texCoordV;
    gradients->gradientX2 = Gfx_RasterTriangleGradientX(deltaA, diffStep, (SwGradientMatrixX *)gradients);
    gradients->gradientY2 = Gfx_RasterTriangleGradientY(deltaA, diffStep, (SwGradientMatrixY *)gradients);
    crt_FloatToInt64(*(int *)&gradients->gradientX1, dest->uStep);
    crt_FloatToInt64(*(int *)&gradients->gradientX2, dest->vStep);
    Gfx_RasterStepDdaSpanOutputs(vertices->spanWidth, (int *)dest->output, dest);
    dest->color = vertices->mappedColor;
}

// Initializes the triangle interpolation data and computes X and Y gradients for each of the three
// color channels, from the vertices' colorChannel1 through colorChannel3 fields. Calls the retained
// conversion helper crt_FloatToInt64 on the first gradient of each channel (gradientR, gradientG,
// gradientB), storing the results in dest->redStep, greenStep and blueStep.
// FUNCTION: FF7 0x006adec4
void __cdecl Gfx_RasterSetupTriangleRgbGradientNoDepth(SwTriangleGradients *gradients, SwRasterState *dest, GfxTriangleVertexSet *vertices)
{
    SwRgbGradientVertexB *vertC;
    float deltaA;
    float diffStep;
    SwRasterVertex3 *vertexA;
    SwRgbGradientVertexB *vertexB;

    vertexA = (SwRasterVertex3 *)(vertices->vertexA);
    vertexB = (SwRgbGradientVertexB *)(vertices->vertexB);
    vertC = (SwRgbGradientVertexB *)(vertices->vertexC);
    Gfx_RasterTriangleGradientSetup((float *)vertexA, (float *)vertexB, (float *)vertC, (GfxRasterUvDepthGradients *)gradients);
    deltaA = vertexB->colorChannel1 - vertexA->colorChannel1;
    diffStep = vertC->colorChannel1 - vertexA->colorChannel1;
    gradients->redGradX = Gfx_RasterTriangleGradientX(deltaA, diffStep, (SwGradientMatrixX *)gradients);
    gradients->redGradY = Gfx_RasterTriangleGradientY(deltaA, diffStep, (SwGradientMatrixY *)gradients);
    deltaA = vertexB->colorChannel2 - vertexA->colorChannel2;
    diffStep = vertC->colorChannel2 - vertexA->colorChannel2;
    gradients->greenGradX = Gfx_RasterTriangleGradientX(deltaA, diffStep, (SwGradientMatrixX *)gradients);
    gradients->greenGradY = Gfx_RasterTriangleGradientY(deltaA, diffStep, (SwGradientMatrixY *)gradients);
    deltaA = vertexB->colorChannel3 - vertexA->colorChannel3;
    diffStep = vertC->colorChannel3 - vertexA->colorChannel3;
    gradients->blueGradX = Gfx_RasterTriangleGradientX(deltaA, diffStep, (SwGradientMatrixX *)gradients);
    gradients->blueGradY = Gfx_RasterTriangleGradientY(deltaA, diffStep, (SwGradientMatrixY *)gradients);
    crt_FloatToInt64(*(int *)&gradients->redGradX, dest->redStep);
    crt_FloatToInt64(*(int *)&gradients->greenGradX, dest->greenStep);
    crt_FloatToInt64(*(int *)&gradients->blueGradX, dest->blueStep);
}

// Initializes the triangle gradient set from the three vertices. Gfx_RasterTriangleGradientSetup
// runs first, then the depth and each colour channel get an X and a Y coefficient computed from the
// vertex deltas against vertex A. The depth and the R, G and B gradients are converted to 64-bit
// values through the retained conversion helper crt_FloatToInt64, into the destination at
// dest->depthStep, redStep, greenStep and blueStep.
// FUNCTION: FF7 0x006ae041
void __cdecl Gfx_RasterSetupTriangleRgbGradientDepth(SwTriangleGradients *gradients, SwRasterState *dest, GfxTriangleVertexSet *vertices)
{
    SwRgbGradientVertexB *vertC;
    float deltaA;
    float diffStep;
    SwRgbGradientVertexB *vertexA;
    SwRgbGradientVertexB *vertexB;

    vertexA = (SwRgbGradientVertexB *)(vertices->vertexA);
    vertexB = (SwRgbGradientVertexB *)(vertices->vertexB);
    vertC = (SwRgbGradientVertexB *)(vertices->vertexC);
    Gfx_RasterTriangleGradientSetup((float *)vertexA, (float *)vertexB, (float *)vertC, (GfxRasterUvDepthGradients *)gradients);
    deltaA = vertexB->depth - vertexA->depth;
    diffStep = vertC->depth - vertexA->depth;
    gradients->depthGradX = Gfx_RasterTriangleGradientX(deltaA, diffStep, (SwGradientMatrixX *)gradients);
    gradients->depthGradY = Gfx_RasterTriangleGradientY(deltaA, diffStep, (SwGradientMatrixY *)gradients);
    crt_FloatToInt64(*(int *)&gradients->depthGradX, dest->depthStep);
    deltaA = vertexB->colorChannel1 - vertexA->colorChannel1;
    diffStep = vertC->colorChannel1 - vertexA->colorChannel1;
    gradients->redGradX = Gfx_RasterTriangleGradientX(deltaA, diffStep, (SwGradientMatrixX *)gradients);
    gradients->redGradY = Gfx_RasterTriangleGradientY(deltaA, diffStep, (SwGradientMatrixY *)gradients);
    deltaA = vertexB->colorChannel2 - vertexA->colorChannel2;
    diffStep = vertC->colorChannel2 - vertexA->colorChannel2;
    gradients->greenGradX = Gfx_RasterTriangleGradientX(deltaA, diffStep, (SwGradientMatrixX *)gradients);
    gradients->greenGradY = Gfx_RasterTriangleGradientY(deltaA, diffStep, (SwGradientMatrixY *)gradients);
    deltaA = vertexB->colorChannel3 - vertexA->colorChannel3;
    diffStep = vertC->colorChannel3 - vertexA->colorChannel3;
    gradients->blueGradX = Gfx_RasterTriangleGradientX(deltaA, diffStep, (SwGradientMatrixX *)gradients);
    gradients->blueGradY = Gfx_RasterTriangleGradientY(deltaA, diffStep, (SwGradientMatrixY *)gradients);
    crt_FloatToInt64(*(int *)&gradients->redGradX, dest->redStep);
    crt_FloatToInt64(*(int *)&gradients->greenGradX, dest->greenStep);
    crt_FloatToInt64(*(int *)&gradients->blueGradX, dest->blueStep);
}

// Sets up one edge pair from two half-records, recordA at segment->startVertex and recordB at
// segment->midVertexIndex.
// It stores the smaller of the two sort keys in pair->sortKey and returns 0 when they are equal, 1
// when the first half is smaller and 2 when the second is. This is the plain member of a family of
// ten that differ only in which interpolators they prime.
// FUNCTION: FF7 0x006ae226
int __cdecl Gfx_RasterSetupEdgePair(SwEdgePair *pair, GfxRasterEdgeChain *segment)
{
    SwEdgePairVertex *vert;
    int side;
    SwEdgeRecord *recordB;
    SwEdgePairSource *recordA;

    recordA = (SwEdgePairSource *)&segment->startVertex;
    recordB = (SwEdgeRecord *)&segment->midVertexIndex;
    if (recordA->sortKey == recordB->sortKey) {
        pair->sortKey = recordA->sortKey;
        side = 0;
    } else if (recordA->sortKey > recordB->sortKey) {
        pair->sortKey = recordB->sortKey;
        side = 2;
    } else {
        pair->sortKey = recordA->sortKey;
        side = 1;
    }
    vert = (SwEdgePairVertex *)(((int *)segment)[recordA->vertexIndex]);
    pair->vertexKey = vert->screenY;
    pair->xFractionA = 0;
    pair->sortKeyA = vert->sortKey;
    pair->slopeLowA = recordA->slopeLow;
    pair->slopeHighA = recordA->slopeHigh;
    vert = (SwEdgePairVertex *)(((int *)segment)[recordB->vertexIndex]);
    pair->xFractionB = 0;
    pair->sortKeyB = vert->sortKey;
    pair->slopeLowB = recordB->edgeAttrA;
    pair->slopeHighB = recordB->edgeAttrB;
    return side;
}

// Follow-on to the Gfx_RasterSetupEdgePair family. It fills one half of the edge pair next for the
// next segment of a chain. When side is 1 it reads the record at segment->secondStartVertex and
// writes edgeParam, vertexKey, xFractionA (set to 0), zIndexA, slopeLowA and slopeHighA. Otherwise
// it reads the record at segment->secondMidVertexIndex and writes edgeParam, vertexKey, xFractionB
// (set to 0), zIndexB, slopeLowB and slopeHighB.
// FUNCTION: FF7 0x006ae322
void __cdecl Gfx_RasterSetupEdgePairNext(int side, SwEdgePairNext *next, GfxRasterEdgeChain *segment)
{
    SwEdgeRecord *record;
    SwEdgeVertex *vert;
    SwEdgeRecord *recordB;

    if (side == 1) {
        record = (SwEdgeRecord *)&segment->secondStartVertex;
        next->edgeParam = record->sortKey;
        vert = (SwEdgeVertex *)(((int *)segment)[record->vertexIndex]);
        next->vertexKey = vert->vertexKey;
        next->xFractionA = 0;
        next->zIndexA = vert->pixelX;
        next->slopeLowA = record->edgeAttrA;
        next->slopeHighA = record->edgeAttrB;
    } else {
        recordB = (SwEdgeRecord *)&segment->secondMidVertexIndex;
        next->edgeParam = recordB->sortKey;
        vert = (SwEdgeVertex *)(((int *)segment)[recordB->vertexIndex]);
        next->vertexKey = vert->vertexKey;
        next->xFractionB = 0;
        next->zIndexB = vert->pixelX;
        next->slopeLowB = recordB->edgeAttrA;
        next->slopeHighB = recordB->edgeAttrB;
    }
}

/*
 * Gfx_RasterSetupEdgePairRgb Gfx_RasterSetupEdgePair with three interpolators
 * primed for the first half: after choosing the smaller of the two sort
 * keys and returning 0, 1 or 2 the same way, it calls
 * Gfx_RasterSeedColorChannel1FromVertex, Gfx_RasterSeedColorChannel2FromVertex
 * and Gfx_RasterSeedColorChannel3FromVertex with the half-record's scale,
 * the vertex's colorChannel1, colorChannel2 and colorChannel3, gradients and pair. The rest of the field
 * copying is identical.
 */
// FUNCTION: FF7 0x006ae3f0
int __cdecl Gfx_RasterSetupEdgePairRgb(SwEdgePairRgb *pair, GfxRasterEdgeChain *segment, GfxRasterVertexTexV *gradients)
{
    SwRasterVertex4 *vert;
    int side;
    SwEdgeRecord *recordB;
    GfxRasterEdgePairSource *recordA;

    recordA = (GfxRasterEdgePairSource *)&segment->startVertex;
    recordB = (SwEdgeRecord *)&segment->midVertexIndex;
    if (recordA->sortKey == recordB->sortKey) {
        pair->sortKey = recordA->sortKey;
        side = 0;
    } else if (recordA->sortKey > recordB->sortKey) {
        pair->sortKey = recordB->sortKey;
        side = 2;
    } else {
        pair->sortKey = recordA->sortKey;
        side = 1;
    }
    vert = (SwRasterVertex4 *)(((int *)segment)[recordA->vertexIndex]);
    Gfx_RasterSeedColorChannel1FromVertex(*(float *)&recordA->scale, *(float *)&vert->colorChannel1, gradients, (SwRasterSeedBlock *)pair);
    Gfx_RasterSeedColorChannel2FromVertex(*(float *)&recordA->scale, *(float *)&vert->colorChannel2, gradients, (SwRasterSeedBlock *)pair);
    Gfx_RasterSeedColorChannel3FromVertex(*(float *)&recordA->scale, *(float *)&vert->colorChannel3, gradients, (SwRasterSeedBlock *)pair);
    pair->vertexKey = vert->pixelY;
    pair->xFractionA = 0;
    pair->zIndex = vert->pixelX;
    pair->setupValueA = recordA->setupValueA;
    pair->setupValueB = recordA->setupValueB;
    vert = (SwRasterVertex4 *)(((int *)segment)[recordB->vertexIndex]);
    pair->xFractionB = 0;
    pair->zIndexDup = vert->pixelX;
    pair->edgeAttrA = recordB->edgeAttrA;
    pair->edgeAttrB = recordB->edgeAttrB;
    return side;
}

/*
 * Gfx_RasterSetupEdgePairNextZRgbSeeded Gfx_RasterSetupEdgePairNext with
 * four interpolators: in the side == 1 branch it additionally calls
 * Gfx_RasterSeedDepthFromVertex,
 * Gfx_RasterSeedColorChannel1FromVertex, Gfx_RasterSeedColorChannel2FromVertex
 * and Gfx_RasterSeedColorChannel3FromVertex with the record's scale, the
 * vertex's depth, colorChannel1, colorChannel2 and colorChannel3, gradients and next. The other branch is
 * the plain copy with no calls.
 */
// FUNCTION: FF7 0x006ae546
void __cdecl Gfx_RasterSetupEdgePairNextZRgbSeeded(int side, SwEdgePairNextZRgb *next, GfxRasterEdgeChain *segment, GfxRasterVertexTexV *gradients)
{
    SwEdgeSpec *entry;
    SwVertex2 *vertex;
    SwZRgbEdgeRecord *recordB;

    if (side == 1) {
        entry = (SwEdgeSpec *)&segment->secondStartVertex;
        next->edgeParam = entry->edgeParam;
        vertex = (SwVertex2 *)(((int *)segment)[entry->vertexIndex]);
        next->vertexKey = vertex->screenY;
        next->xFractionA = 0;
        next->zIndex = vertex->screenX;
        next->edgeAttrA = entry->edgeAttrA;
        next->edgeAttrB = entry->edgeAttrB;
        Gfx_RasterSeedDepthFromVertex(*(float *)&entry->scale, *(float *)&vertex->depth, gradients, (SwRasterSetupState *)next);
        Gfx_RasterSeedColorChannel1FromVertex(*(float *)&entry->scale, *(float *)&vertex->colorChannel1, gradients, (SwRasterSeedBlock *)next);
        Gfx_RasterSeedColorChannel2FromVertex(*(float *)&entry->scale, *(float *)&vertex->colorChannel2, gradients, (SwRasterSeedBlock *)next);
        Gfx_RasterSeedColorChannel3FromVertex(*(float *)&entry->scale, *(float *)&vertex->colorChannel3, gradients, (SwRasterSeedBlock *)next);
    } else {
        recordB = (SwZRgbEdgeRecord *)&segment->secondMidVertexIndex;
        next->edgeParam = recordB->edgeParam;
        vertex = (SwVertex2 *)(((int *)segment)[recordB->vertexIndex]);
        next->vertexKey = vertex->screenY;
        next->xFractionB = 0;
        next->zIndexDup = vertex->screenX;
        next->altAttrA = recordB->altAttrA;
        next->altAttrB = recordB->altAttrB;
    }
}

/*
 * Gfx_RasterSetupEdgePairUv Gfx_RasterSetupEdgePair with two interpolators
 * primed for the first half: it calls
 * Gfx_RasterSeedTextureCoordUFromVertex and
 * Gfx_RasterSeedTextureCoordVFromVertex with the half-record's
 * scale, the vertex's u and v, gradients and seed. Selector and field
 * copying are otherwise identical.
 */
// FUNCTION: FF7 0x006ae690
int __cdecl Gfx_RasterSetupEdgePairUv(GfxRasterVertexSeed *seed, GfxRasterEdgeChain *segment, GfxRasterVertexTexV *gradients)
{
    SwRasterVertexUv *vert;
    int side;
    SwEdgeRecord *recordB;
    GfxRasterEdgePair *recordA;

    recordA = (GfxRasterEdgePair *)&segment->startVertex;
    recordB = (SwEdgeRecord *)&segment->midVertexIndex;
    if (recordA->sortKey == recordB->sortKey) {
        seed->sortKey = recordA->sortKey;
        side = 0;
    } else if (recordA->sortKey > recordB->sortKey) {
        seed->sortKey = recordB->sortKey;
        side = 2;
    } else {
        seed->sortKey = recordA->sortKey;
        side = 1;
    }
    vert = (SwRasterVertexUv *)(((int *)segment)[recordA->vertexIndex]);
    Gfx_RasterSeedTextureCoordUFromVertex(*(float *)&recordA->scale, *(float *)&vert->u, gradients, seed);
    Gfx_RasterSeedTextureCoordVFromVertex(*(float *)&recordA->scale, *(float *)&vert->v, gradients, seed);
    seed->y = vert->screenY;
    seed->stepU = 0;
    seed->x = vert->screenX;
    seed->startU = recordA->startU;
    seed->startV = recordA->startV;
    vert = (SwRasterVertexUv *)(((int *)segment)[recordB->vertexIndex]);
    seed->stepV = 0;
    seed->vertexAttr = vert->screenX;
    seed->endU = recordB->edgeAttrA;
    seed->endV = recordB->edgeAttrB;
    return side;
}

/*
 * Gfx_RasterSetupEdgePairNextUv Gfx_RasterSetupEdgePairNext with two
 * interpolators: the side == 1 branch also calls
 * Gfx_RasterSeedTextureCoordUFromVertex and
 * Gfx_RasterSeedTextureCoordVFromVertex with the record's scale,
 * the vertex's texCoordU and texCoordV, gradients and next. The other branch copies
 * fields only.
 */
// FUNCTION: FF7 0x006ae7c8
void __cdecl Gfx_RasterSetupEdgePairNextUv(int side, GfxRasterVertexSeed *next, GfxRasterEdgeChain *segment, GfxRasterVertexTexV *gradients)
{
    SwEdgeRecord *entry;
    SwRasterVertex5 *vertex;
    SwUvEdgeRecord *recordB;

    if (side == 1) {
        entry = (SwEdgeRecord *)&segment->secondStartVertex;
        next->sortKey = entry->sortKey;
        vertex = (SwRasterVertex5 *)(((int *)segment)[entry->vertexIndex]);
        next->y = vertex->screenY;
        next->stepU = 0;
        next->x = vertex->screenX;
        next->startU = entry->edgeAttrA;
        next->startV = entry->edgeAttrB;
        Gfx_RasterSeedTextureCoordUFromVertex(*(float *)&entry->scale, *(float *)&vertex->texCoordU, gradients, next);
        Gfx_RasterSeedTextureCoordVFromVertex(*(float *)&entry->scale, *(float *)&vertex->texCoordV, gradients, next);
    } else {
        recordB = (SwUvEdgeRecord *)&segment->secondMidVertexIndex;
        next->sortKey = recordB->sortKey;
        vertex = (SwRasterVertex5 *)(((int *)segment)[recordB->vertexIndex]);
        next->y = vertex->screenY;
        next->stepV = 0;
        next->vertexAttr = vertex->screenX;
        next->endU = recordB->endU;
        next->endV = recordB->endV;
    }
}

/*
 * Gfx_RasterSetupEdgePairZ Gfx_RasterSetupEdgePair with one interpolator
 * primed for the first half: it calls Gfx_RasterSeedDepthFromVertex
 * with the half-record's scale, the vertex's depth, gradients and pair.
 * Selector and field copying are otherwise identical.
 */
// FUNCTION: FF7 0x006ae8d6
int __cdecl Gfx_RasterSetupEdgePairZ(SwEdgePairZ *pair, GfxRasterEdgeChain *segment, GfxRasterVertexTexV *gradients)
{
    SwDepthVertex *vert;
    int side;
    SwEdgeRecord *recordB;
    SwEdgeZSpec *recordA;

    recordA = (SwEdgeZSpec *)&segment->startVertex;
    recordB = (SwEdgeRecord *)&segment->midVertexIndex;
    if (recordA->sortKey == recordB->sortKey) {
        pair->sortKey = recordA->sortKey;
        side = 0;
    } else if (recordA->sortKey > recordB->sortKey) {
        pair->sortKey = recordB->sortKey;
        side = 2;
    } else {
        pair->sortKey = recordA->sortKey;
        side = 1;
    }
    vert = (SwDepthVertex *)(((int *)segment)[recordA->vertexIndex]);
    Gfx_RasterSeedDepthFromVertex(*(float *)&recordA->scale, *(float *)&vert->depth, gradients, (SwRasterSetupState *)pair);
    pair->vertexKey = vert->vertexKey;
    pair->xFractionA = 0;
    pair->zIndex = vert->pixelX;
    pair->slopeLowA = recordA->slopeLow;
    pair->slopeHighA = recordA->slopeHigh;
    vert = (SwDepthVertex *)(((int *)segment)[recordB->vertexIndex]);
    pair->xFractionB = 0;
    pair->zIndexDup = vert->pixelX;
    pair->edgeAttrA = recordB->edgeAttrA;
    pair->edgeAttrB = recordB->edgeAttrB;
    return side;
}

/*
 * Gfx_RasterSetupEdgePairNextZ Gfx_RasterSetupEdgePairNext with one
 * interpolator: the mode == 1 branch also calls
 * Gfx_RasterSeedDepthFromVertex with the record's depthSeed, the
 * vertex's depth, seedCtx and dst. The other branch copies fields only.
 */
// FUNCTION: FF7 0x006ae9f0
void __cdecl Gfx_RasterSetupEdgePairNextZ(int mode, SwEdgePairNextZ *dst, GfxRasterEdgeChain *src, GfxRasterVertexTexV *seedCtx)
{
    SwEdgePairSourceEntry *entry;
    SwRasterVertexSource *vertex;
    SwEdgeRecord *altEntry;

    if (mode == 1) {
        entry = (SwEdgePairSourceEntry *)&src->secondStartVertex;
        dst->edgeParam = entry->edgeParam;
        vertex = (SwRasterVertexSource *)(((int *)src)[entry->vertexIndex]);
        dst->vertexKey = vertex->screenY;
        dst->xFractionA = 0;
        dst->screenX = vertex->screenX;
        dst->slopeLowA = entry->slopeLow;
        dst->slopeHighA = entry->slopeHigh;
        Gfx_RasterSeedDepthFromVertex(*(float *)&entry->depthSeed, *(float *)&vertex->depth, seedCtx, (SwRasterSetupState *)dst);
    } else {
        altEntry = (SwEdgeRecord *)&src->secondMidVertexIndex;
        dst->edgeParam = altEntry->sortKey;
        vertex = (SwRasterVertexSource *)(((int *)src)[altEntry->vertexIndex]);
        dst->vertexKey = vertex->screenY;
        dst->xFractionB = 0;
        dst->screenXDup = vertex->screenX;
        dst->slopeLowB = altEntry->edgeAttrA;
        dst->slopeHighB = altEntry->edgeAttrB;
    }
}

/*
 * Gfx_RasterSetupEdgePairZRgb Gfx_RasterSetupEdgePair with four interpolators
 * primed for the first half, the widest member of the family:
 * Gfx_RasterSeedDepthFromVertex,
 * Gfx_RasterSeedColorChannel1FromVertex, Gfx_RasterSeedColorChannel2FromVertex
 * and Gfx_RasterSeedColorChannel3FromVertex, taking the half-record's field
 * scale, the vertex's depth, colorChannel1, colorChannel2 and colorChannel3 respectively, seedCtx and dst.
 * Selector and field copying are otherwise identical.
 */
// FUNCTION: FF7 0x006aeadc
int __cdecl Gfx_RasterSetupEdgePairZRgb(SwEdgePairZRgb *dst, GfxRasterEdgeChain *src, GfxRasterVertexTexV *seedCtx)
{
    int res;
    SwVertexZRgb *vert;
    SwEdgeRecord *endSetup;
    SwEdgeZSpec *startSetup;

    startSetup = (SwEdgeZSpec *)&src->startVertex;
    endSetup = (SwEdgeRecord *)&src->midVertexIndex;
    if (startSetup->sortKey == endSetup->sortKey) {
        dst->sortKey = startSetup->sortKey;
        res = 0;
    } else if (startSetup->sortKey > endSetup->sortKey) {
        dst->sortKey = endSetup->sortKey;
        res = 2;
    } else {
        dst->sortKey = startSetup->sortKey;
        res = 1;
    }
    vert = (SwVertexZRgb *)(((int *)src)[startSetup->vertexIndex]);
    Gfx_RasterSeedDepthFromVertex(*(float *)&startSetup->scale, *(float *)&vert->depth, seedCtx, (SwRasterSetupState *)dst);
    Gfx_RasterSeedColorChannel1FromVertex(*(float *)&startSetup->scale, *(float *)&vert->colorChannel1, seedCtx, (SwRasterSeedBlock *)dst);
    Gfx_RasterSeedColorChannel2FromVertex(*(float *)&startSetup->scale, *(float *)&vert->colorChannel2, seedCtx, (SwRasterSeedBlock *)dst);
    Gfx_RasterSeedColorChannel3FromVertex(*(float *)&startSetup->scale, *(float *)&vert->colorChannel3, seedCtx, (SwRasterSeedBlock *)dst);
    dst->vertexKey = vert->pixelY;
    dst->xFractionA = 0;
    dst->screenX = vert->screenX;
    dst->slopeLowA = startSetup->slopeLow;
    dst->slopeHighA = startSetup->slopeHigh;
    vert = (SwVertexZRgb *)(((int *)src)[endSetup->vertexIndex]);
    dst->xFractionB = 0;
    dst->screenXDup = vert->screenX;
    dst->edgeAttrA = endSetup->edgeAttrA;
    dst->edgeAttrB = endSetup->edgeAttrB;
    return res;
}

/*
 * Gfx_RasterSetupEdgePairNextZRgb Gfx_RasterSetupEdgePairNext with the same
 * four interpolators Gfx_RasterSetupEdgePairZRgb primes: in the mode == 1
 * branch it calls Gfx_RasterSeedDepthFromVertex,
 * Gfx_RasterSeedColorChannel1FromVertex, Gfx_RasterSeedColorChannel2FromVertex
 * and Gfx_RasterSeedColorChannel3FromVertex on the vertex's depth, colorChannel1,
 * colorChannel2 and colorChannel3 with seedCtx and dst. The other branch copies fields only.
 */
// FUNCTION: FF7 0x006aec50
void __cdecl Gfx_RasterSetupEdgePairNextZRgb(int mode, SwEdgePairNextZRgb *dst, GfxRasterEdgeChain *src, GfxRasterVertexTexV *seedCtx)
{
    SwVertex2 *vertex;
    SwZRgbEdgeRecord *altEnt;
    SwEdgeSpec *edgeSpec;

    if (mode == 1) {
        edgeSpec = (SwEdgeSpec *)&src->secondStartVertex;
        dst->edgeParam = edgeSpec->edgeParam;
        vertex = (SwVertex2 *)(((int *)src)[edgeSpec->vertexIndex]);
        dst->vertexKey = vertex->screenY;
        dst->xFractionA = 0;
        dst->zIndex = vertex->screenX;
        dst->edgeAttrA = edgeSpec->edgeAttrA;
        dst->edgeAttrB = edgeSpec->edgeAttrB;
        Gfx_RasterSeedDepthFromVertex(*(float *)&edgeSpec->scale, *(float *)&vertex->depth, seedCtx, (SwRasterSetupState *)dst);
        Gfx_RasterSeedColorChannel1FromVertex(*(float *)&edgeSpec->scale, *(float *)&vertex->colorChannel1, seedCtx, (SwRasterSeedBlock *)dst);
        Gfx_RasterSeedColorChannel2FromVertex(*(float *)&edgeSpec->scale, *(float *)&vertex->colorChannel2, seedCtx, (SwRasterSeedBlock *)dst);
        Gfx_RasterSeedColorChannel3FromVertex(*(float *)&edgeSpec->scale, *(float *)&vertex->colorChannel3, seedCtx, (SwRasterSeedBlock *)dst);
    } else {
        altEnt = (SwZRgbEdgeRecord *)&src->secondMidVertexIndex;
        dst->edgeParam = altEnt->edgeParam;
        vertex = (SwVertex2 *)(((int *)src)[altEnt->vertexIndex]);
        dst->vertexKey = vertex->screenY;
        dst->xFractionB = 0;
        dst->zIndexDup = vertex->screenX;
        dst->altAttrA = altEnt->altAttrA;
        dst->altAttrB = altEnt->altAttrB;
    }
}

/*
 * Gfx_RasterSetupEdgePairZUv Gfx_RasterSetupEdgePair with three interpolators
 * primed for the first half: Gfx_RasterSeedDepthFromVertex on the
 * vertex's depth, Gfx_RasterSeedTextureCoordUFromVertex on texCoordU and
 * Gfx_RasterSeedTextureCoordVFromVertex on texCoordV, each also given the
 * half-record's scale, seedCtx and dst. Selector and field copying are
 * otherwise identical.
 */
// FUNCTION: FF7 0x006aed9a
int __cdecl Gfx_RasterSetupEdgePairZUv(GfxRasterVertexSeed *dst, GfxRasterEdgeChain *src, GfxRasterVertexTexV *seedCtx)
{
    int res;
    SwVertexZUv *vert;
    SwEdgeRecord *endSetup;
    GfxRasterEdgePairSource *startSetup;

    startSetup = (GfxRasterEdgePairSource *)&src->startVertex;
    endSetup = (SwEdgeRecord *)&src->midVertexIndex;
    if (startSetup->sortKey == endSetup->sortKey) {
        dst->sortKey = startSetup->sortKey;
        res = 0;
    } else if (startSetup->sortKey > endSetup->sortKey) {
        dst->sortKey = endSetup->sortKey;
        res = 2;
    } else {
        dst->sortKey = startSetup->sortKey;
        res = 1;
    }
    vert = (SwVertexZUv *)(((int *)src)[startSetup->vertexIndex]);
    Gfx_RasterSeedDepthFromVertex(*(float *)&startSetup->scale, *(float *)&vert->depth, seedCtx, (SwRasterSetupState *)dst);
    Gfx_RasterSeedTextureCoordUFromVertex(*(float *)&startSetup->scale, *(float *)&vert->texCoordU, seedCtx, dst);
    Gfx_RasterSeedTextureCoordVFromVertex(*(float *)&startSetup->scale, *(float *)&vert->texCoordV, seedCtx, dst);
    dst->y = vert->screenY;
    dst->stepU = 0;
    dst->x = vert->screenX;
    dst->startU = startSetup->setupValueA;
    dst->startV = startSetup->setupValueB;
    vert = (SwVertexZUv *)(((int *)src)[endSetup->vertexIndex]);
    dst->stepV = 0;
    dst->vertexAttr = vert->screenX;
    dst->endU = endSetup->edgeAttrA;
    dst->endV = endSetup->edgeAttrB;
    return res;
}

/*
 * Gfx_RasterSetupEdgePairNextZUv Gfx_RasterSetupEdgePairNext with the same
 * three interpolators Gfx_RasterSetupEdgePairZUv primes: in the mode == 1
 * branch it calls Gfx_RasterSeedDepthFromVertex,
 * Gfx_RasterSeedTextureCoordUFromVertex and
 * Gfx_RasterSeedTextureCoordVFromVertex on the vertex's depth, texCoordU
 * and texCoordV with seedCtx and dst. The other branch copies fields only.
 */
// FUNCTION: FF7 0x006aeef0
void __cdecl Gfx_RasterSetupEdgePairNextZUv(int mode, GfxRasterVertexSeed *dst, GfxRasterEdgeChain *src, GfxRasterVertexTexV *seedCtx)
{
    SwVertexZUv *vertex;
    SwUvEdgeRecord *altEnt;
    GfxRasterEdgePair *edgeSpec;

    if (mode == 1) {
        edgeSpec = (GfxRasterEdgePair *)&src->secondStartVertex;
        dst->sortKey = edgeSpec->sortKey;
        vertex = (SwVertexZUv *)(((int *)src)[edgeSpec->vertexIndex]);
        dst->y = vertex->screenY;
        dst->stepU = 0;
        dst->x = vertex->screenX;
        dst->startU = edgeSpec->startU;
        dst->startV = edgeSpec->startV;
        Gfx_RasterSeedDepthFromVertex(*(float *)&edgeSpec->scale, *(float *)&vertex->depth, seedCtx, (SwRasterSetupState *)dst);
        Gfx_RasterSeedTextureCoordUFromVertex(*(float *)&edgeSpec->scale, *(float *)&vertex->texCoordU, seedCtx, dst);
        Gfx_RasterSeedTextureCoordVFromVertex(*(float *)&edgeSpec->scale, *(float *)&vertex->texCoordV, seedCtx, dst);
    } else {
        altEnt = (SwUvEdgeRecord *)&src->secondMidVertexIndex;
        dst->sortKey = altEnt->sortKey;
        vertex = (SwVertexZUv *)(((int *)src)[altEnt->vertexIndex]);
        dst->y = vertex->screenY;
        dst->stepV = 0;
        dst->vertexAttr = vertex->screenX;
        dst->endU = altEnt->endU;
        dst->endV = altEnt->endV;
    }
}

// Copies the texture's index of the lowest set bit of its width, and its width and height masks,
// into the mask state: shiftX, maskX (width minus 1) and maskY (height minus 1). maskSource is
// texture->maskSource when that is nonzero, otherwise texture->maskSourceFallback.
// FUNCTION: FF7 0x006af01c
void __cdecl Gfx_RasterSetTextureMaskSetup(GfxTextureSurface *drawGroup, GfxRasterSetupBlock *maskState)
{
    SwTextureInfo *texture;

    texture = (SwTextureInfo *)(drawGroup->header);
    maskState->shiftX = LowestSetBitIndex32(texture->width);
    maskState->maskX = texture->width - 1;
    maskState->maskY = texture->height - 1;
    if (texture->maskSource != 0) {
        maskState->maskSource = texture->maskSource;
    } else {
        maskState->maskSource = texture->maskSourceFallback;
    }
}

/*
 * Gfx_RasterSetTextureMaskSetupRows Copies texture width-bit index,
 * width/height masks and the pixelBuffer source into destination setup fields.
 * Clamps a negative source row index to zero and derives two table pointers
 * from that index times row stride and four bytes.
 */
// FUNCTION: FF7 0x006af09e
void __cdecl Gfx_RasterSetTextureMaskSetupRows(GfxTextureSurface *drawGroup, GfxRasterSetupBlock *maskState)
{
    int rowOffset;
    SwRowTable *rowTable;
    SwTextureMask *tex;
    int rowNumber;

    rowTable = (SwRowTable *)drawGroup->palette;
    tex = (SwTextureMask *)drawGroup->header;
    maskState->shiftX = LowestSetBitIndex32(tex->width);
    maskState->maskX = tex->width - 1;
    maskState->maskY = tex->height - 1;
    maskState->maskSource = tex->pixelBuffer;
    rowNumber = drawGroup->paletteRow;
    if (rowNumber < 0) {
        rowNumber = 0;
    }
    rowOffset = rowNumber * rowTable->rowStride;
    maskState->rowPtr0 = rowTable->rowBase0 + rowOffset * 4;
    maskState->rowPtr1 = rowTable->rowBase1 + rowOffset * 4;
}
} /* extern "C" */
