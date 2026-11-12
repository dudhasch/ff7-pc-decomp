// src/lib/src/graphics/gl/gl_node_draw_emitters.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Node-level OpenGL draw emitters: for model-space and screen-space nodes they bind the
//   node's texture when the state has one, then send triangles or lines to the flat, Gouraud or
//   textured drawers.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/types/graphics.h"
#include "lib/api/graphics.h"
#include "lib/api/token.h"
#include "lib/crt.h"
#include "lib/codegen.h"
extern "C" {

// clang-format off
// clang-format on

// clang-format off
// Pointer words: they point at the objects that hold those bytes. The externs stay above the
// markers.
// GLOBAL: FF7 0x009196c0
void * g_pGlVertexBuffer = (void *)g_GlVertexScratchBuffer;
// The vertex scratch buffer g_pGlVertexBuffer points at (26,688 bytes); it follows token.cpp's .bss
// (whose empty-string word g_GfxRasterLineUnused precedes it) at an 8-aligned start.
unsigned char g_GlVertexScratchBuffer[26688] = {0};


__declspec(dllimport) void __stdcall glFlush(void);

/*
 * Bind the draw state's texture, and upload its palette when it has one. The
 * state's draw group carries the texture surface in its batch word; without
 * one the whole thing collapses to disabling GL_TEXTURE_2D.
 */
// FUNCTION: FF7 0x006b6180
void __cdecl Gl_BindNodeTexture(GlDrawState *state, GfxEngineContext *out)
{
    GlDrawGroup *st;
    GfxTextureSurface *tex;

    glFlush();
    st = state->drawGroup;
    Gl_ApplyRenderState(st, out);
    if (st->batch != 0) {
        tex = (GfxTextureSurface *)st->batch;
        Gl_BindTexture(tex->handle);
        if (tex->palette != 0) {
            Gl_UploadPaletteEXT(
                0, 0, ((GlPaletteSource *)tex->palette)->entryCount,
                tex->paletteRow,
                ((GlPaletteSource *)tex->palette)->paletteEntryCount,
                (int)Gl_GetPaletteColorArrays((GfxPaletteRecord *)tex->palette), tex);
        }
    } else {
        Gl_SetTexture2DEnable(0);
    }
}

/*
 * The screen-space setup slots do nothing but the texture bind that Gl_EmitSetupModelSpaceA ends
 * with -- no matrix, because screen space needs none. Six functions in this TU are byte-identical
 * apart from the call's relative displacement: the four screen-space slots here and the two line
 * slots Gl_EmitSetupLinesA and Gl_EmitSetupLinesB.
 */
// FUNCTION: FF7 0x006b6229
void __cdecl Gl_EmitSetupScreenSpaceA(GlDrawState *state, GlDrawNode *node, GfxEngineContext *out)
{
    if (state->isTextured != 0) {
        Gl_BindNodeTexture(state, out);
    }
}

/*
 * Screen-space setup emitter B: binds the node's texture when the state has
 * one.
 */
// FUNCTION: FF7 0x006b6247
void __cdecl Gl_EmitSetupScreenSpaceB(GlDrawState *state, GlDrawNode *node, GfxEngineContext *out)
{
    if (state->isTextured != 0) {
        Gl_BindNodeTexture(state, out);
    }
}

/*
 * Screen-space setup emitter C: binds the node's texture when the state has
 * one.
 */
// FUNCTION: FF7 0x006b6265
void __cdecl Gl_EmitSetupScreenSpaceC(GlDrawState *state, GlDrawNode *node, GfxEngineContext *out)
{
    if (state->isTextured != 0) {
        Gl_BindNodeTexture(state, out);
    }
}

/*
 * Screen-space setup emitter D: binds the node's texture when the state has
 * one.
 */
// FUNCTION: FF7 0x006b6283
void __cdecl Gl_EmitSetupScreenSpaceD(GlDrawState *state, GlDrawNode *node, GfxEngineContext *out)
{
    if (state->isTextured != 0) {
        Gl_BindNodeTexture(state, out);
    }
}

/*
 * Gl_EmitTrisModelSpace for screen space: the render state's isGouraud flag
 * picks flat or Gouraud, and the same three node members go to either
 * drawer. The screen-space drawers take three arguments where the model-space
 * ones take four; the fourth there is the context matrix, which screen space
 * has no use for.
 */
// FUNCTION: FF7 0x006b62a1
void __cdecl Gl_EmitTrisScreenSpace(GlDrawState *state, GlDrawNode *node, GfxEngineContext *out)
{
    GlRenderState *ctx;

    ctx = (GlRenderState *)out->presetPolygonSets;
    if (ctx->isGouraud == 0) {
        Gl_DrawTrisFlat(node->indexCount,
                        (unsigned short *)node->indices,
                        (int)node->vertices);
    } else {
        Gl_DrawTrisGouraud(node->indexCount,
                           (unsigned short *)node->indices,
                           (int)node->vertices);
    }
}

/*
 * Draw one textured screen-space node: hand it to the paged variant if its
 * bound texture's header says the texture is paged, otherwise offset the
 * texture coordinates by half a texel and draw it flat or Gouraud. The
 * half-texel offset is `fld / fmul [-0.5] / fchs / fstp`, which is
 * `-(v * -0.5f)` and not `-v * 0.5f` -- the latter negates before the
 * multiply.
 */
// FUNCTION: FF7 0x006b62fa
void __cdecl Gl_EmitTrisScreenSpaceTextured(GlDrawState *state, GlDrawNode *node, GfxEngineContext *out)
{
    GlRenderState *rst;
    GfxTexHeader *desc;
    GfxTextureSurface *prim;
    float vy;
    float ox;
    GlNodeMesh *geo;

    rst = (GlRenderState *)out->presetPolygonSets;
    prim = (GfxTextureSurface *)rst->batch;
    if (prim != 0) {
        desc = (GfxTexHeader *)prim->header;
        if (desc->format.palettized != 0) {
            Gl_EmitTrisScreenSpaceTexturedPaged(state, node, out);
        } else {
            if (out->configApplied != 0) {
                geo = node->mesh;
                if (geo->texture->batch != 0) {
                    ox = -(geo->sizeX * -0.5f);
                    vy = -(geo->sizeY * -0.5f);
                    Gfx_OffsetVertexTexCoords(ox, vy,
                                              node->vertexCount,
                                              (GfxVertexTextured *)node->vertices);
                }
            }
            if (rst->isGouraud == 0) {
                Gl_DrawTrisTexFlat(node->indexCount,
                                   (unsigned short *)node->indices,
                                   (int)node->vertices);
            } else {
                Gl_DrawTrisTexGouraud(node->indexCount,
                                      (unsigned short *)node->indices,
                                      (int)node->vertices);
            }
        }
    }
}

// Draws the paged textured triangles of a screen-space node when the state's isTextured flag is
// set. When the context's configApplied is set, the node's texture coordinates are first offset by
// half of the mesh's sizeX and sizeY. The pageIds array is walked over the node's entryCount
// entries, and consecutive equal page IDs are grouped into runs. Each run selects and binds its
// page, advances the vertex cursor by stride times run length times 32, and adds pageStep per
// entry to the index count. The run is drawn flat or Gouraud by the render state's isGouraud.
// FUNCTION: FF7 0x006b63f7
void __cdecl Gl_EmitTrisScreenSpaceTexturedPaged(GlDrawState *state, GlDrawNode *node, GfxEngineContext *out)
{
    GlDrawGroup *polyTexture;
    char *vertCursor;
    GlRenderState *deviceInfo;
    int numIndices;
    int pid;
    GlNodeMesh *pageInformation;
    int runCnt;
    int strideSum;
    char *start;
    float ofsX;
    float halfSizeY;
    int aux;
    unsigned char *idPtr;
    int entryCnt;

    deviceInfo = (GlRenderState *)out->presetPolygonSets;
    aux = state->statusFlags;
    pageInformation = node->mesh;
    if (state->isTextured != 0) {
        polyTexture = state->drawGroup;
        idPtr = node->pageIds;
        vertCursor = (char *)node->vertices;
        entryCnt = node->entryCount;
        if (out->configApplied != 0) {
            ofsX = -(pageInformation->sizeX * -0.5f);
            halfSizeY = -(pageInformation->sizeY * -0.5f);
            Gfx_OffsetVertexTexCoords(ofsX, halfSizeY, node->vertexCount, (GfxVertexTextured *)node->vertices);
        }
        while (entryCnt > 0) {
            runCnt = 0;
            start = vertCursor;
            strideSum = pageInformation->stride;
            numIndices = pageInformation->pageStep;
            pid = *idPtr;
            Gfx_SelectTexturePage(pid, polyTexture);
            Gl_BindNodeTexture(state, out);
            idPtr++;
            runCnt++;
            while (runCnt < entryCnt) {
                if (pid == *idPtr) {
                    strideSum += pageInformation->stride;
                    numIndices += pageInformation->pageStep;
                    idPtr++;
                    runCnt++;
                } else {
                    break;
                }
            }
            vertCursor += pageInformation->stride * runCnt << 5;
            entryCnt -= runCnt;
            if (deviceInfo->isGouraud == 0) {
                Gl_DrawTrisTexFlat(numIndices, (unsigned short *)node->indices, (int)start);
            } else {
                Gl_DrawTrisTexGouraud(numIndices, (unsigned short *)node->indices, (int)start);
            }
        }
    }
}

// Builds the model-space matrix setup for one node of record type A. The record is the node's mesh.
// Its pointed matrix (dataPtr) is used when usePointedMatrix is nonzero, and otherwise the
// mesh's inline matrix is used. MultiplyMatrix4x4ByTranspose takes that matrix, the context's
// transposed screen transform and g_pGlVertexBuffer, and the texture is bound when the state is
// textured. This is one of four identical copies, A to D, which differ only in the record type
// they cast to.
// FUNCTION: FF7 0x006b65a4
void __cdecl Gl_EmitSetupModelSpaceA(GlDrawState *state, GlDrawNode *node, GfxEngineContext *out)
{
    void *mtx;
    GlNodeMesh *record;

    record = node->mesh;
    if (record->usePointedMatrix != 0) {
        mtx = record->dataPtr;
    } else {
        mtx = record->matrix;
    }
    MultiplyMatrix4x4ByTranspose(mtx, out->screenTransformTransposed, g_pGlVertexBuffer);
    if (state->isTextured != 0) {
        Gl_BindNodeTexture(state, out);
    }
}

// Hands one model-space node's triangles to the untextured drawers. The node's indices, indexCount
// (the strip counts) and transformed vertices go to either the flat or the Gouraud drawer, with
// g_pGlVertexBuffer as the last argument. The render state's isGouraud flag picks between the two
// drawers.
// FUNCTION: FF7 0x006b6612
void __cdecl Gl_EmitTrisModelSpace(GlDrawState *state, GlDrawNode *node, GfxEngineContext *out)
{
    GlRenderState *ctx;

    ctx = (GlRenderState *)out->presetPolygonSets;
    if (ctx->isGouraud == 0) {
        Gl_DrawTrisTransformedFlat(node->indexCount,
                                   (unsigned short *)node->indices,
                                   (int)node->vertices, (int)g_pGlVertexBuffer);
    } else {
        Gl_DrawTrisTransformedGouraud(node->indexCount,
                                      (unsigned short *)node->indices,
                                      (int)node->vertices, (int)g_pGlVertexBuffer);
    }
}

// Builds the model-space matrix setup for one node of record type B. The record is the node's mesh.
// Its pointed matrix (dataPtr) is used when usePointedMatrix is nonzero, and otherwise the
// mesh's inline matrix is used. MultiplyMatrix4x4ByTranspose takes that matrix, the context's
// transposed screen transform and g_pGlVertexBuffer, and the texture is bound when the state is
// textured. This is one of four identical copies, A to D, which differ only in the record type
// they cast to.
// FUNCTION: FF7 0x006b6678
void __cdecl Gl_EmitSetupModelSpaceB(GlDrawState *state, GlDrawNode *node, GfxEngineContext *out)
{
    void *mtx;
    GlNodeMesh *record;

    record = node->mesh;
    if (record->usePointedMatrix != 0) {
        mtx = record->dataPtr;
    } else {
        mtx = record->matrix;
    }
    MultiplyMatrix4x4ByTranspose(mtx, out->screenTransformTransposed, g_pGlVertexBuffer);
    if (state->isTextured != 0) {
        Gl_BindNodeTexture(state, out);
    }
}

// Builds the model-space matrix setup for one node of record type C. The record is the node's mesh.
// Its pointed matrix (dataPtr) is used when usePointedMatrix is nonzero, and otherwise the
// mesh's inline matrix is used. MultiplyMatrix4x4ByTranspose takes that matrix, the context's
// transposed screen transform and g_pGlVertexBuffer, and the texture is bound when the state is
// textured. This is one of four identical copies, A to D, which differ only in the record type
// they cast to.
// FUNCTION: FF7 0x006b66e6
void __cdecl Gl_EmitSetupModelSpaceC(GlDrawState *state, GlDrawNode *node, GfxEngineContext *out)
{
    void *mtx;
    GlNodeMesh *record;

    record = node->mesh;
    if (record->usePointedMatrix != 0) {
        mtx = record->dataPtr;
    } else {
        mtx = record->matrix;
    }
    MultiplyMatrix4x4ByTranspose(mtx, out->screenTransformTransposed, g_pGlVertexBuffer);
    if (state->isTextured != 0) {
        Gl_BindNodeTexture(state, out);
    }
}

// Builds the model-space matrix setup for one node of record type D. The record is the node's mesh.
// Its pointed matrix (dataPtr) is used when usePointedMatrix is nonzero, and otherwise the
// mesh's inline matrix is used. MultiplyMatrix4x4ByTranspose takes that matrix, the context's
// transposed screen transform and g_pGlVertexBuffer, and the texture is bound when the state is
// textured. This is one of four identical copies, A to D, which differ only in the record type
// they cast to.
// FUNCTION: FF7 0x006b6754
void __cdecl Gl_EmitSetupModelSpaceD(GlDrawState *state, GlDrawNode *node, GfxEngineContext *out)
{
    void *mtx;
    GlNodeMesh *record;

    record = node->mesh;
    if (record->usePointedMatrix != 0) {
        mtx = record->dataPtr;
    } else {
        mtx = record->matrix;
    }
    MultiplyMatrix4x4ByTranspose(mtx, out->screenTransformTransposed, g_pGlVertexBuffer);
    if (state->isTextured != 0) {
        Gl_BindNodeTexture(state, out);
    }
}

// Textured counterpart of Gl_EmitTrisModelSpace. The node's indices, indexCount and vertices go
// to the textured flat or Gouraud drawer, with g_pGlVertexBuffer as the last argument. The render
// state's isGouraud flag picks between the two drawers.
// FUNCTION: FF7 0x006b67c2
void __cdecl Gl_EmitTrisModelSpaceTextured(GlDrawState *state, GlDrawNode *node, GfxEngineContext *out)
{
    GlRenderState *ctx;

    ctx = (GlRenderState *)out->presetPolygonSets;
    if (ctx->isGouraud == 0) {
        Gl_DrawTrisTransformedTexFlat(node->indexCount,
                                      (unsigned short *)node->indices,
                                      (int)node->vertices, (int)g_pGlVertexBuffer);
    } else {
        Gl_DrawTrisTransformedTexGouraud(node->indexCount,
                                         (unsigned short *)node->indices,
                                         (int)node->vertices, (int)g_pGlVertexBuffer);
    }
}

// Draws the paged textured triangles of a model-space node when the state is textured. The
// indices are the node's, and its pageIds array holds one texture-page ID for each of its
// entryCount entries. Consecutive equal IDs are grouped into runs. Each run selects and binds its
// texture page once, adds the mesh's pageStep to the primitive count per entry, and advances the
// vertex cursor by the mesh's stride times run length times 32. The run is drawn with the textured
// flat or Gouraud drawer, chosen by the render state's isGouraud flag. The junk accumulator is
// written but never read.
// FUNCTION: FF7 0x006b6828
void __cdecl Gl_EmitTrisModelSpaceTexturedPaged(GlDrawState *state, GlDrawNode *node,
                                                GfxEngineContext *out)
{
    unsigned char *p;
    int rem;
    GlRenderState *dev;
    char *vbuf;
    GlDrawGroup *timg;
    int nprims;
    int tpg;
    GlNodeMesh *mesh;
    int k;
    int junk;
    char *base;

    dev = (GlRenderState *)out->presetPolygonSets;
    mesh = node->mesh;
    if (state->isTextured != 0) {
        timg = state->drawGroup;
        p = node->pageIds;
        vbuf = (char *)node->vertices;
        rem = node->entryCount;
        while (rem > 0) {
            k = 0;
            base = vbuf;
            junk = mesh->stride;
            nprims = mesh->pageStep;
            tpg = *p;
            Gfx_SelectTexturePage(tpg, timg);
            Gl_BindNodeTexture(state, out);
            p++;
            k++;
            while (k < rem) {
                if (tpg == *p) {
                    junk += mesh->stride;
                    nprims += mesh->pageStep;
                    p++;
                    k++;
                } else {
                    break;
                }
            }
            vbuf += mesh->stride * k * 0x20;
            rem -= k;
            if (dev->isGouraud == 0) {
                Gl_DrawTrisTransformedTexFlat(
                    nprims, (unsigned short *)node->indices, (int)base,
                    (int)g_pGlVertexBuffer);
            } else {
                Gl_DrawTrisTransformedTexGouraud(
                    nprims, (unsigned short *)node->indices, (int)base,
                    (int)g_pGlVertexBuffer);
            }
        }
    }
}

/*
 * Line setup emitter A: binds the node's texture when the state has one.
 */
// FUNCTION: FF7 0x006b698a
void __cdecl Gl_EmitSetupLinesA(GlDrawState *state, GlDrawNode *node, GfxEngineContext *out)
{
    if (state->isTextured != 0) {
        Gl_BindNodeTexture(state, out);
    }
}

/*
 * Calls Gl_BindNodeTexture with state/output only when the state is textured.
 * The node argument is unused.
 */
// FUNCTION: FF7 0x006b69a8
void __cdecl Gl_EmitSetupLinesB(GlDrawState *state, GlDrawNode *node, GfxEngineContext *out)
{
    if (state->isTextured != 0) {
        Gl_BindNodeTexture(state, out);
    }
}

/*
 * The line counterpart of Gl_EmitTrisModelSpace: the node's index list, count
 * and vertices go to the flat line drawer. There is no render context test
 * here, because the flat and Gouraud variants are separate ops-table slots
 * rather than two arms of one function.
 */
// FUNCTION: FF7 0x006b69c6
void __cdecl Gl_EmitLinesFlat(GlDrawState *state, GlDrawNode *node, GfxEngineContext *out)
{
    Gl_DrawLinesFlat((unsigned short *)node->indices,
                     node->entryCount,
                     (int)node->vertices);
}

/*
 * Passes the node's index list, line count and vertices to
 * Gl_DrawLinesGouraud. State/output arguments are unused.
 */
// FUNCTION: FF7 0x006b69e8
void __cdecl Gl_EmitLinesGouraud(GlDrawState *state, GlDrawNode *node, GfxEngineContext *out)
{
    Gl_DrawLinesGouraud((unsigned short *)node->indices,
                        node->entryCount,
                        (int)node->vertices);
}
} /* extern "C" */
