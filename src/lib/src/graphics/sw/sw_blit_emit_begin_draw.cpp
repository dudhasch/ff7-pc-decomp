// src/lib/src/graphics/sw/sw_blit_emit_begin_draw.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: The software blit emitters: each begin and draw slot for 2D, 3D and line batches
//   applies the texture and render state, selects the node matrix, and hands the finished node to
//   the shared drawer through a common finish step.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/types/graphics.h"
#include "lib/api/graphics.h"
#include "lib/crt.h"
#include "lib/types/mem.h"
#include "lib/codegen.h"
extern "C" {

// clang-format off
// clang-format on


/*
 * The tail every Blit_EmitBegin* slot ends with: file the render-state block's
 * node into the display list, then hand the block on. The two scale lookups
 * read the rasterizer state's coordinate table, which Gl_EmitSetupModelSpaceA's
 * neighbours use too.
 */
// FUNCTION: FF7 0x006b6a10
void __cdecl Blit_EmitFinishNode(void *state, GfxEngineContext *out)
{
    SwFinishNode *node;
    GfxRasterGroupSetup *rst;
    SwRasterizerState *ctx;
    void *scale;
    SwFinishNodeDesc *desc;
    GfxRasterVertexBuffer *dl;
    int ax;
    int uy;
    GfxRasterSetupBlock *bucket;

    rst = (GfxRasterGroupSetup *)out->presetPolygonSets;
    ctx = out->rasterBase;
    scale = (char *)ctx + 0x38;
    node = (SwFinishNode *)rst->resource;
    if (node != 0) {
        desc = (SwFinishNodeDesc *)(node->descriptor);
        dl = (GfxRasterVertexBuffer *)ctx->rasterVertexBuffer;
        bucket = (GfxRasterSetupBlock *)ctx->renderSetup;
        ax = *(int *)((char *)scale + desc->scaleIndexX * 4 + 0x27b8);
        uy = *(int *)((char *)scale + desc->scaleIndexY * 4 + 0x27b8);
        if (*(int *)(node->streamState + 0x10) != 0) {
            Gfx_RasterSetTextureMaskSetupRows((GfxTextureSurface *)node, bucket);
        } else {
            Gfx_RasterSetTextureMaskSetup((GfxTextureSurface *)node, bucket);
        }
        Gfx_RasterSetStateScalePair(ax, uy, dl);
        dl->scaleIndexX = desc->scaleIndexX;
    }
    Gfx_RasterSelectGeneratedVertexCallbacks((int)*(void **)((char *)state + 0xa8), rst, (GfxRasterVertexCallbacks *)((char *)scale + 0x154));
}

/*
 * The 2D half of Blit_EmitBegin3DUntextured: the same conditional state apply
 * and the same Blit_EmitFinishNode tail, with the matrix work that only 3D
 * needs left out. The state's texture goes through a local because it is
 * computed before the argument list and read from its slot inside it.
 */
// FUNCTION: FF7 0x006b6b08
void __cdecl Blit_EmitBegin2DUntextured(BlitLineFlatState *state, void *node, GfxEngineContext *out)
{
    void *tex;

    if (state->hasTexture != 0) {
        tex = state->texture;
        Gfx_ApplyStateUntexturedCall(tex, out);
    }
    Blit_EmitFinishNode(state, out);
}

/*
 * 2D blit begin emitter (textured, no texture bound yet): applies the state's
 * texture if the state is textured, then finishes the node.
 */
// FUNCTION: FF7 0x006b6b42
void __cdecl Blit_EmitBegin2DTexturedNoTex(BlitLineFlatState *state, void *node, GfxEngineContext *out)
{
    void *tex;

    if (state->hasTexture != 0) {
        tex = state->texture;
        Gfx_ApplyStateTexturedCall(tex, out);
    }
    Blit_EmitFinishNode(state, out);
}

/*
 * 2D blit begin emitter (textured, opaque): applies the state's texture
 * if the state is textured, then finishes the node.
 */
// FUNCTION: FF7 0x006b6b7c
void __cdecl Blit_EmitBegin2DTexturedOpaque(BlitLineFlatState *state, void *node, GfxEngineContext *out)
{
    void *tex;

    if (state->hasTexture != 0) {
        tex = state->texture;
        Gfx_ApplyStateTexturedCall(tex, out);
    }
    Blit_EmitFinishNode(state, out);
}

/*
 * The last of the four; three of them share Gfx_ApplyStateTexturedCall and
 * only the first calls the untextured one.
 */
// FUNCTION: FF7 0x006b6bb6
void __cdecl Blit_EmitBegin2DTexturedBlended(BlitLineFlatState *state, void *node, GfxEngineContext *out)
{
    void *tex;

    if (state->hasTexture != 0) {
        tex = state->texture;
        Gfx_ApplyStateTexturedCall(tex, out);
    }
    Blit_EmitFinishNode(state, out);
}

/*
 * Hand the node to the shared drawer, doubling the primitive count when the
 * node's primitive kind (read through countPtr) is above 7.
 */
// FUNCTION: FF7 0x006b6bf0
void __cdecl Blit_EmitDraw2DUntextured(void *state, GlDraw2DNode *node, GfxEngineContext *out)
{
    int count;

    if (*node->countPtr <= 7) {
        count = node->count;
    } else {
        count = node->count * 2;
    }
    Gfx_RasterDrawIndexedTransformedTriangles(0, (int)node->srcVertices,
                                              (int)node->indices, count,
                                              (int)node->vertexCount, out);
}

// The untextured 2D blit path, Blit_EmitDraw2DUntextured, with a detour. When the state word at
// +0xA8 has bit 0x100000 set, the setup block's colorScaleLut and sumRemapLut take the values
// quarterScaleLut and threeQuarterScaleLut from the rasterizer state for the draw, and the saved values halfScaleLut and
// doubleBrightLut are put back afterwards. The primitive count is doubled when *node->blendModeRef is
// above 7, and each arm repeats that computation before calling
// Gfx_RasterDrawIndexedTransformedTriangles.
// FUNCTION: FF7 0x006b6c40
void __cdecl Blit_EmitDraw2DTexturedNoTex(void *state, GlBlit2DNode *node, GfxEngineContext *out)
{
    int count;
    SwRasterizerState *gfx;
    SwBlitDrawQueue *queue;

    if ((*(int *)((char *)state + 0xa8) & 0x100000) != 0) {
        gfx = out->rasterBase;
        queue = (SwBlitDrawQueue *)gfx->renderSetup;
        queue->colorScaleLut = gfx->quarterScaleLut;
        queue->sumRemapLut = gfx->threeQuarterScaleLut;
        if (*node->blendModeRef <= 7) {
            count = node->count;
        } else {
            count = node->count * 2;
        }
        Gfx_RasterDrawIndexedTransformedTriangles(0, (int)node->vertexData,
                                                  (int)node->indices, count,
                                                  (int)node->vertexCount, out);
        queue->colorScaleLut = gfx->halfScaleLut;
        queue->sumRemapLut = gfx->doubleBrightLut;
    } else {
        if (*node->blendModeRef <= 7) {
            count = node->count;
        } else {
            count = node->count * 2;
        }
        Gfx_RasterDrawIndexedTransformedTriangles(0, (int)node->vertexData,
                                                  (int)node->indices, count,
                                                  (int)node->vertexCount, out);
    }
}

/*
 * When the source has a texture object, walks its primitive count and selects
 * texture pages before finishing render state. Flag 0x1000000 processes each
 * primitive separately, validates palette row index, converts the first vertex
 * color into that palette and uploads it.
 */
// FUNCTION: FF7 0x006b6d53
void __cdecl Blit_EmitDraw2DTextured(SwBlitState *blitState, GlBlit2DTexturedSource *source, GfxEngineContext *context)
{
    int vertexCount;
    unsigned char *pagePtr;
    int vertCursor;
    GlBlitTexturedStep *textureStep;
    int paletteY;
    GfxPaletteBlock *palBlock;
    int runVtxTotal;
    int floatColor[4];
    int bgra;
    int numberPrims;
    int statusFlags;
    int textureV;
    int texObject;
    int runCount;
    int indexOfs;
    int paletteOffset;

    statusFlags = blitState->statusFlags;
    textureStep = (GlBlitTexturedStep *)(source->textureStep);
    if (blitState->hasTexture != 0) {
        texObject = blitState->texObject;
        pagePtr = (unsigned char *)source->pagePtr;
        vertCursor = source->vertCursor;
        numberPrims = source->numberPrims;
        if ((blitState->statusFlags & 0x1000000) != 0) {
            while (numberPrims > 0) {
                runCount = 0;
                runVtxTotal = vertCursor;
                indexOfs = textureStep->stepU;
                textureV = textureStep->stepV;
                paletteY = *pagePtr;
                palBlock = (GfxPaletteBlock *)(((GfxTextureSurface *)*(int *)((char *)texObject + 0x14))->palette);
                if (paletteY >= 0 && paletteY < palBlock->count) { /* empty first arm: lib/codegen.h */
                } else {
                    paletteY = 0;
                }
                paletteOffset = paletteY * palBlock->stride;
                bgra = *(int *)((char *)runVtxTotal + 0x10);
                Gfx_UnpackBgraToFloat4(bgra, (float *)floatColor);
                Gfx_ModulatePaletteRowToBytes(floatColor, paletteY, (struct GfxPaletteBlock *)palBlock);
                Gfx_UploadTexturePalette(paletteOffset, palBlock->stride, palBlock->startIndex, paletteOffset, *(int *)((char *)texObject + 0x14));
                Gfx_SelectTexturePage(paletteY, (void *)texObject);
                Blit_EmitFinishNode(blitState, context);
                pagePtr++;
                runCount++;
                vertCursor += textureStep->stepU * runCount << 5;
                numberPrims -= runCount;
                if (textureStep->blendMode <= 7) {
                    vertexCount = runCount;
                    Gfx_RasterDrawIndexedTransformedTriangles(0, indexOfs, source->count, vertexCount, runVtxTotal, context);
                } else {
                    vertexCount = runCount << 1;
                    if ((statusFlags & 0xE0) != 0) {
                        if (Gfx_RasterTryDrawTexturedRectQuads(statusFlags, runCount, source->count, (GfxRasterTexRect *)runVtxTotal, context) != 0) { /* empty first arm: lib/codegen.h */
                        } else {
                            Gfx_RasterDrawIndexedTransformedTriangles(0, indexOfs, source->count, vertexCount, runVtxTotal, context);
                        }
                    } else {
                        Gfx_RasterDrawIndexedTransformedTriangles(0, indexOfs, source->count, vertexCount, runVtxTotal, context);
                    }
                }
            }
        } else {
            while (numberPrims > 0) {
                runCount = 0;
                runVtxTotal = vertCursor;
                indexOfs = textureStep->stepU;
                textureV = textureStep->stepV;
                paletteY = *pagePtr;
                Gfx_SelectTexturePage(paletteY, (void *)texObject);
                Blit_EmitFinishNode(blitState, context);
                pagePtr++;
                runCount++;
                while (runCount < numberPrims) {
                    if (paletteY == *pagePtr) {
                        indexOfs += textureStep->stepU;
                        textureV += textureStep->stepV;
                        pagePtr++;
                        runCount++;
                    } else {
                        break;
                    }
                }
                vertCursor += textureStep->stepU * runCount << 5;
                numberPrims -= runCount;
                if (textureStep->blendMode <= 7) {
                    vertexCount = runCount;
                    Gfx_RasterDrawIndexedTransformedTriangles(0, indexOfs, source->count, vertexCount, runVtxTotal, context);
                } else {
                    vertexCount = runCount << 1;
                    if ((statusFlags & 0xE0) != 0) {
                        if (Gfx_RasterTryDrawTexturedRectQuads(statusFlags, runCount, source->count, (GfxRasterTexRect *)runVtxTotal, context) != 0) { /* empty first arm: lib/codegen.h */
                        } else {
                            Gfx_RasterDrawIndexedTransformedTriangles(0, indexOfs, source->count, vertexCount, runVtxTotal, context);
                        }
                    } else {
                        Gfx_RasterDrawIndexedTransformedTriangles(0, indexOfs, source->count, vertexCount, runVtxTotal, context);
                    }
                }
            }
        }
    }
}

// Begins a 3D untextured model blit. The matrix is mm->material when mm->useNodeMatrix is nonzero,
// otherwise the inline mm->matrix. It is multiplied with the transposed screen transform into the
// rasterizer state's modelMatrix, and Gfx_RasterSetStateMatrixPair is called with that result and
// the state's rasterVertexBuffer vertex buffer. If state->hasTexture is set, Gfx_ApplyStateUntexturedCall applies
// state->texture. Blit_EmitFinishNode then finishes the node.
// FUNCTION: FF7 0x006b70f7
void __cdecl Blit_EmitBegin3DUntextured(BlitLineFlatState *state, void *node, GfxEngineContext *out)
{
    SwRasterizerState *ctx;
    void *mat;
    SwEmitModelStateOpaque *mm;
    void *dst;
    void *tex;

    ctx = out->rasterBase;
    mm = ((SwEmitModelNode *)node)->model;
    if (mm->useNodeMatrix != 0) {
        mat = mm->material;
    } else {
        mat = mm->matrix;
    }
    dst = ctx->modelMatrix;
    MultiplyMatrix4x4ByTranspose(mat, out->screenTransformTransposed, dst);
    Gfx_RasterSetStateMatrixPair((int)mat, (int)dst, (GfxRasterVertexBuffer *)ctx->rasterVertexBuffer);
    if (state->hasTexture != 0) {
        tex = state->texture;
        Gfx_ApplyStateUntexturedCall(tex, out);
    }
    Blit_EmitFinishNode(state, out);
}

/*
 * Blit_EmitDraw2DUntextured with a 1 in the leading argument.
 */
// FUNCTION: FF7 0x006b71ae
void __cdecl Blit_EmitDraw3DUntextured(void *state, GlDraw2DNode *node, GfxEngineContext *out)
{
    int count;

    if (*node->countPtr <= 7) {
        count = node->count;
    } else {
        count = node->count * 2;
    }
    Gfx_RasterDrawIndexedTransformedTriangles(1, (int)node->srcVertices,
                                              (int)node->indices, count,
                                              (int)node->vertexCount, out);
}

// Begins a 3D textured-no-tex model blit. The matrix is mm->material when mm->useNodeMatrix is
// nonzero, otherwise the inline mm->matrix. It is multiplied with the transposed screen transform
// into the rasterizer state's modelMatrix, and Gfx_RasterSetStateMatrixPair is called with that
// result and the state's rasterVertexBuffer vertex buffer. If state->hasTexture is set, Gfx_ApplyStateTexturedCall applies state->texture.
// Blit_EmitFinishNode then finishes the node.
// FUNCTION: FF7 0x006b71fe
void __cdecl Blit_EmitBegin3DTexturedNoTex(BlitLineFlatState *state, void *node, GfxEngineContext *out)
{
    SwRasterizerState *ctx;
    void *mat;
    SwEmitModelStateOpaque *mm;
    void *dst;
    void *tex;

    ctx = out->rasterBase;
    mm = ((SwEmitModelNode *)node)->model;
    if (mm->useNodeMatrix != 0) {
        mat = mm->material;
    } else {
        mat = mm->matrix;
    }
    dst = ctx->modelMatrix;
    MultiplyMatrix4x4ByTranspose(mat, out->screenTransformTransposed, dst);
    Gfx_RasterSetStateMatrixPair((int)mat, (int)dst, (GfxRasterVertexBuffer *)ctx->rasterVertexBuffer);
    if (state->hasTexture != 0) {
        tex = state->texture;
        Gfx_ApplyStateTexturedCall(tex, out);
    }
    Blit_EmitFinishNode(state, out);
}

// Begins a 3D textured opaque model blit. The matrix is mm->material when mm->useNodeMatrix is
// nonzero, otherwise the inline mm->matrix. It is multiplied with the transposed screen transform
// into the rasterizer state's modelMatrix, and Gfx_RasterSetStateMatrixPair is called with that
// result and the state's rasterVertexBuffer vertex buffer. If state->hasTexture is set, Gfx_ApplyStateTexturedCall applies state->texture.
// Blit_EmitFinishNode then finishes the node.
// FUNCTION: FF7 0x006b72b5
void __cdecl Blit_EmitBegin3DTexturedOpaque(BlitLineFlatState *state, void *node, GfxEngineContext *out)
{
    SwRasterizerState *ctx;
    void *mat;
    SwEmitModelStateOpaque *mm;
    void *dst;
    void *tex;

    ctx = out->rasterBase;
    mm = ((SwEmitModelNode *)node)->model;
    if (mm->useNodeMatrix != 0) {
        mat = mm->material;
    } else {
        mat = mm->matrix;
    }
    dst = ctx->modelMatrix;
    MultiplyMatrix4x4ByTranspose(mat, out->screenTransformTransposed, dst);
    Gfx_RasterSetStateMatrixPair((int)mat, (int)dst, (GfxRasterVertexBuffer *)ctx->rasterVertexBuffer);
    if (state->hasTexture != 0) {
        tex = state->texture;
        Gfx_ApplyStateTexturedCall(tex, out);
    }
    Blit_EmitFinishNode(state, out);
}

// Begins a 3D textured blended model blit. The matrix is mm->material when mm->useNodeMatrix is
// nonzero, otherwise the inline mm->matrix. It is multiplied with the transposed screen transform
// into the rasterizer state's modelMatrix, and Gfx_RasterSetStateMatrixPair is called with that
// result and the state's rasterVertexBuffer vertex buffer. If state->hasTexture is set, Gfx_ApplyStateTexturedCall applies state->texture.
// Blit_EmitFinishNode then finishes the node.
// FUNCTION: FF7 0x006b736c
void __cdecl Blit_EmitBegin3DTexturedBlended(BlitLineFlatState *state, void *node, GfxEngineContext *out)
{
    SwRasterizerState *ctx;
    void *mat;
    SwEmitModelStateOpaque *mm;
    void *dst;
    void *tex;

    ctx = out->rasterBase;
    mm = ((SwEmitModelNode *)node)->model;
    if (mm->useNodeMatrix != 0) {
        mat = mm->material;
    } else {
        mat = mm->matrix;
    }
    dst = ctx->modelMatrix;
    MultiplyMatrix4x4ByTranspose(mat, out->screenTransformTransposed, dst);
    Gfx_RasterSetStateMatrixPair((int)mat, (int)dst, (GfxRasterVertexBuffer *)ctx->rasterVertexBuffer);
    if (state->hasTexture != 0) {
        tex = state->texture;
        Gfx_ApplyStateTexturedCall(tex, out);
    }
    Blit_EmitFinishNode(state, out);
}

/*
 * Byte-identical to Blit_EmitDraw3DUntextured: the texture is already bound by
 * the matching Begin slot, so the draw does not differ.
 */
// FUNCTION: FF7 0x006b7423
void __cdecl Blit_EmitDraw3DTexturedNoTex(void *state, GlDraw2DNode *node, GfxEngineContext *out)
{
    int count;

    if (*node->countPtr <= 7) {
        count = node->count;
    } else {
        count = node->count * 2;
    }
    Gfx_RasterDrawIndexedTransformedTriangles(1, (int)node->srcVertices,
                                              (int)node->indices, count,
                                              (int)node->vertexCount, out);
}

/*
 * When blitState->hasTexture is nonzero, groups the positive count of byte
 * texture-page IDs at source->pagePtr into consecutive equal runs. Selects each
 * run page, finishes the current node, sums the descriptor's verticesPerPrim and
 * indicesPerPrim, advances the vertex cursor by verticesPerPrim times run length
 * times 32, and calls Gfx_RasterDrawIndexedTransformedTriangles in mode 1 with
 * the accumulated counts.
 */
// FUNCTION: FF7 0x006b7473
void __cdecl Blit_EmitDraw3DTextured(BlitDraw3DTexturedState *blitState, BlitDraw3DTexturedSource *source, GfxEngineContext *context)
{
    int vertexCount;
    unsigned char *pagePtr;
    int numberPrims;
    int vertCursor;
    int texObj;
    int altCount;
    int pageIndex;
    int runLength;
    GfxRenderObject *pDescriptor;
    int runVertNumber;
    int indexOffset;

    pDescriptor = (GfxRenderObject *)(source->descriptor);
    if (blitState->hasTexture != 0) {
        texObj = blitState->texObj;
        pagePtr = (unsigned char *)source->pagePtr;
        vertCursor = source->vertexCursor;
        numberPrims = source->primitiveCount;
        while (numberPrims > 0) {
            runLength = 0;
            runVertNumber = vertCursor;
            indexOffset = pDescriptor->verticesPerPrim;
            altCount = pDescriptor->indicesPerPrim;
            pageIndex = *pagePtr;
            Gfx_SelectTexturePage(pageIndex, (void *)texObj);
            Blit_EmitFinishNode(blitState, context);
            pagePtr++;
            runLength++;
            while (runLength < numberPrims) {
                if (pageIndex == *pagePtr) {
                    indexOffset += pDescriptor->verticesPerPrim;
                    altCount += pDescriptor->indicesPerPrim;
                    pagePtr++;
                    runLength++;
                } else {
                    break;
                }
            }
            vertCursor += pDescriptor->verticesPerPrim * runLength << 5;
            numberPrims -= runLength;
            if (pDescriptor->kind <= 7) {
                vertexCount = runLength;
            } else {
                vertexCount = runLength << 1;
            }
            Gfx_RasterDrawIndexedTransformedTriangles(1, indexOffset, source->indices, vertexCount, runVertNumber, context);
        }
    }
}

/*
 * Start a flat line batch: apply the texture if there is one, finish the node,
 * and hand the draw state's callback its buffer. The texture is applied only
 * when the state's hasTexture says the node has one -- the same flag
 * Gl_EmitSetupModelSpaceA tests before binding.
 */
// FUNCTION: FF7 0x006b75bc
void __cdecl Blit_EmitBeginLineFlat(BlitLineFlatState *state, void *unused, GfxEngineContext *out)
{
    void *tex;
    SwLineSpanPrim *fn;
    void *target;

    if (state->hasTexture != 0) {
        tex = state->texture;
        Gfx_ApplyStateTexturedCall(tex, out);
    }
    Blit_EmitFinishNode(state, out);
    target = (char *)out->rasterBase + 0x18c;
    fn = (SwLineSpanPrim *)out->presetPolygonSets;
    Gfx_RasterSelectLineSpanWriter(fn, (SwLineOps *)target);
}

/*
 * Blit_EmitBeginLineFlat again, byte for byte apart from one jump target. The
 * original ships both.
 */
// FUNCTION: FF7 0x006b7626
void __cdecl Blit_EmitBeginLineGouraud(BlitLineFlatState *state, void *unused, GfxEngineContext *out)
{
    void *tex;
    SwLineSpanPrim *fn;
    void *target;

    if (state->hasTexture != 0) {
        tex = state->texture;
        Gfx_ApplyStateTexturedCall(tex, out);
    }
    Blit_EmitFinishNode(state, out);
    target = (char *)out->rasterBase + 0x18c;
    fn = (SwLineSpanPrim *)out->presetPolygonSets;
    Gfx_RasterSelectLineSpanWriter(fn, (SwLineOps *)target);
}

/*
 * Five arguments to the shared line drawer, Gfx_RasterDrawIndexedLineList. The flat
 * and Gouraud slots are byte-identical apart from the call displacement, and both reach the same
 * target -- so the shading is decided inside it, or not at all.
 */
// FUNCTION: FF7 0x006b7690
void __cdecl Blit_EmitDrawLineFlat(void *state, SwLineDrawFlatNode *node, GfxEngineContext *out)
{
    Gfx_RasterDrawIndexedLineList((int)node->vertexBase,
                                  (int)node->indices, node->lineCount,
                                  (SwLineVertexRecord *)(node->vertices), out);
}

// Draws a node's Gouraud line list through Gfx_RasterDrawIndexedLineList. The first argument is the
// node's colour block (vertexBase), and the indices, line count and vertices come from the node's
// own fields.
// FUNCTION: FF7 0x006b76bd
void __cdecl Blit_EmitDrawLineGouraud(void *state, SwLineDrawFlatNode *node, GfxEngineContext *out)
{
    Gfx_RasterDrawIndexedLineList((int)node->vertexBase,
                                  (int)node->indices, node->lineCount,
                                  (SwLineVertexRecord *)(node->vertices), out);
}
} /* extern "C" */
