// src/lib/src/graphics/sw/sw_gfx_raster_vertex_snap.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Vertex preparation and snapping for the software rasterizer: brings each vertex into
//   view once, computes its Z, UV or colour interpolation, and snaps generated vertices to the
//   integer grid with clamping.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/types/graphics.h"
#include "lib/api/graphics.h"
#include "lib/crt.h"
#include "lib/codegen.h"
extern "C" {

// clang-format off
// clang-format on

void __cdecl crt_FloatToInt64(float value, void *out);

/*
 * Computes the transformed Z component from source coordinates and matrix
 * terms.
 */
// FUNCTION: FF7 0x006b77f0
void __cdecl Gfx_RasterInterpolateVertexZ(float tx, float ty, SwTriangleGradients *xf, SwRasterVertex *src,
                                          SwRasterVertex *dst, void *unused)
{
    dst->depth =
        tx * xf->depthGradX + src->depth + ty * xf->depthGradY;
}

/*
 * Writes the destination's texCoordU and texCoordV as the source's plus tx and ty times their
 * gradients from xf. The final argument is unused.
 */
// FUNCTION: FF7 0x006b7815
void __cdecl Gfx_RasterInterpolateVertexUv(float tx, float ty, SwTriangleGradients *xf, SwRasterVertex *src,
                                           SwRasterVertex *dst, void *unused)
{
    dst->texCoordU =
        tx * xf->uGradX +
        src->texCoordU +
        ty * xf->uGradY;
    dst->texCoordV =
        tx * xf->vGradX +
        src->texCoordV +
        ty * xf->vGradY;
}

// Gouraud interpolator for the destination's red, green and blue channels, each the line of
// Gfx_RasterInterpolateVertexZ for that channel, each clamped to 0 to 255. The clamps read as fcomp
// against 0.0f and 255.0f in .rdata and store the literal back, so both bounds are float constants
// on one side of the comparison and immediates on the other.
// FUNCTION: FF7 0x006b785a
void __cdecl Gfx_RasterInterpolateVertexRgbClamped(float tx, float ty, SwTriangleGradients *xf, SwRasterVertex *src, SwRasterVertex *dst,
                                                   void *unused)
{
    dst->red =
        tx * xf->redGradX +
        src->red +
        ty * xf->redGradY;
    if (dst->red < 0.0f) {
        dst->red = 0.0f;
    }
    if (dst->red > 255.0f) {
        dst->red = 255.0f;
    }
    dst->green =
        tx * xf->greenGradX +
        src->green +
        ty * xf->greenGradY;
    if (dst->green < 0.0f) {
        dst->green = 0.0f;
    }
    if (dst->green > 255.0f) {
        dst->green = 255.0f;
    }
    dst->blue =
        tx * xf->blueGradX +
        src->blue +
        ty * xf->blueGradY;
    if (dst->blue < 0.0f) {
        dst->blue = 0.0f;
    }
    if (dst->blue > 255.0f) {
        dst->blue = 255.0f;
    }
}

/*
 * Converts vertex X/Y into retained integer fields, clamps their pixelX and pixelY below zero,
 * reports viewport exclusion as the return value and resolves coordinate table entries. The first
 * parameter is the triangle's gradients (it sits where the other snappers take their xf). Runs
 * three interpolation helpers from differences to the preceding vertex, overwrites the
 * destination packed color first from srcRecord then from prev, and marks the vertex resolved.
 */
// FUNCTION: FF7 0x006b796d
int __cdecl Gfx_RasterSnapGeneratedVertexZUvRgbOutsideFlag(SwTriangleGradients *index, SwRasterSourceVertex *srcRecord, SwRasterVertex *prev, SwRasterVertex *vertex, SwRasterData *coordTable)
{
    int outside;
    GfxEngineContext *engine;
    float deltaY;
    float dx;

    crt_FloatToInt64(vertex->x, &vertex->fixedXLow);
    crt_FloatToInt64(vertex->y, &vertex->fixedYLow);
    if (vertex->pixelX < 0) {
        vertex->pixelX = 0;
    }
    if (vertex->pixelY < 0) {
        vertex->pixelY = 0;
    }
    engine = GetEngineContext();
    outside = 0;
    if (vertex->pixelX < engine->clipLeft) {
        outside = 1;
    }
    if (vertex->pixelX > engine->clipRight) {
        outside = 1;
    }
    if (vertex->pixelY < engine->clipTop) {
        outside = 1;
    }
    if (vertex->pixelY > engine->clipBottom) {
        outside = 1;
    }
    vertex->xBits = (*(int *)&coordTable->intToFloat[vertex->pixelX]);
    vertex->yBits = (*(int *)&coordTable->intToFloat[vertex->pixelY]);
    dx = vertex->x - prev->x;
    deltaY = vertex->y - prev->y;
    Gfx_RasterInterpolateVertexZ(dx, deltaY, index, prev, vertex, coordTable->scale);
    Gfx_RasterInterpolateVertexUv(dx, deltaY, index, prev, vertex, coordTable->scale);
    Gfx_RasterInterpolateVertexRgbClamped(dx, deltaY, index, prev, vertex, coordTable->scale);
    vertex->packedColor = srcRecord->attr14;
    vertex->packedColor = prev->packedColor;
    vertex->resolved = 1;
    return outside;
}

// Converts the destination X and Y to 64-bit values (fixedXLow and fixedYLow), clamps pixelX and
// pixelY to zero when negative, and replaces X and Y with the intToFloat entries those indices
// select. The deltas from src go to Gfx_RasterInterpolateVertexZ with the gradients and the scale
// object. Finally src's packedColor is copied to dst and dst->resolved is set to 1.
// FUNCTION: FF7 0x006b7b15
void __cdecl Gfx_RasterSnapGeneratedVertexZ(SwTriangleGradients *xf, void *unused, SwRasterVertex *src, SwRasterVertex *dst, SwRasterData *ctx)
{
    float tx;
    float ty;

    crt_FloatToInt64(dst->x, &dst->fixedXLow);
    crt_FloatToInt64(dst->y, &dst->fixedYLow);
    if (dst->pixelX < 0) {
        dst->pixelX = 0;
    }
    if (dst->pixelY < 0) {
        dst->pixelY = 0;
    }
    dst->xBits =
        (*(int *)&ctx->intToFloat[dst->pixelX]);
    dst->yBits =
        (*(int *)&ctx->intToFloat[dst->pixelY]);
    tx = dst->x - src->x;
    ty = dst->y - src->y;
    Gfx_RasterInterpolateVertexZ(tx, ty, xf, src, dst, ctx->scale);
    dst->packedColor = src->packedColor;
    dst->resolved = 1;
}

/*
 * Gfx_RasterSnapGeneratedVertexZ without the interpolator: snap the vertex to the grid, clamp
 * pixelX and pixelY at zero, look the coordinates up in the context's intToFloat table and carry
 * the packedColor across. With no interpolator call there are no deltas to hold.
 */
// FUNCTION: FF7 0x006b7bf3
void __cdecl Gfx_RasterSnapGeneratedVertexColorOnly(SwTriangleGradients *xf, void *unused, SwRasterVertex *src, SwRasterVertex *dst,
                                                    SwRasterData *ctx)
{
    crt_FloatToInt64(dst->x, &dst->fixedXLow);
    crt_FloatToInt64(dst->y, &dst->fixedYLow);
    if (dst->pixelX < 0) {
        dst->pixelX = 0;
    }
    if (dst->pixelY < 0) {
        dst->pixelY = 0;
    }
    dst->xBits =
        (*(int *)&ctx->intToFloat[dst->pixelX]);
    dst->yBits =
        (*(int *)&ctx->intToFloat[dst->pixelY]);
    dst->packedColor = src->packedColor;
    dst->resolved = 1;
}

// Converts the destination X and Y to 64-bit values (fixedXLow and fixedYLow), clamps pixelX and
// pixelY to zero when negative, and replaces X and Y with the intToFloat entries those indices
// select. The deltas from src go to Gfx_RasterInterpolateVertexZ and
// Gfx_RasterInterpolateVertexRgbClamped with the gradients and the scale object. The node's
// attr14 is stored to dst's packedColor and at once overwritten with src's, so the first store has
// no effect; dst->resolved is set to 1.
// FUNCTION: FF7 0x006b7c8a
void __cdecl Gfx_RasterSnapGeneratedVertexZRgbNodeColorRead(SwTriangleGradients *xf, SwRasterSourceVertex *node, SwRasterVertex *src, SwRasterVertex *dst,
                                                            SwRasterData *ctx)
{
    float tx;
    float ty;

    crt_FloatToInt64(dst->x, &dst->fixedXLow);
    crt_FloatToInt64(dst->y, &dst->fixedYLow);
    if (dst->pixelX < 0) {
        dst->pixelX = 0;
    }
    if (dst->pixelY < 0) {
        dst->pixelY = 0;
    }
    dst->xBits =
        (*(int *)&ctx->intToFloat[dst->pixelX]);
    dst->yBits =
        (*(int *)&ctx->intToFloat[dst->pixelY]);
    tx = dst->x - src->x;
    ty = dst->y - src->y;
    Gfx_RasterInterpolateVertexZ(tx, ty, xf, src, dst, ctx->scale);
    Gfx_RasterInterpolateVertexRgbClamped(tx, ty, xf, src, dst, ctx->scale);
    dst->packedColor = node->attr14;
    dst->packedColor = src->packedColor;
    dst->resolved = 1;
}

// Converts the destination X and Y to 64-bit values (fixedXLow and fixedYLow), clamps pixelX and
// pixelY to zero when negative, and replaces X and Y with the intToFloat entries those indices
// select. The deltas from src go to Gfx_RasterInterpolateVertexRgbClamped with the gradients and
// the scale object. The node's attr14 is stored to dst's packedColor and at once overwritten with
// src's, so the first store has no effect; dst->resolved is set to 1.
// FUNCTION: FF7 0x006b7d9a
void __cdecl Gfx_RasterSnapGeneratedVertexRgbNodeColorRead(SwTriangleGradients *xf, SwRasterSourceVertex *node, SwRasterVertex *src, SwRasterVertex *dst,
                                                           SwRasterData *ctx)
{
    float tx;
    float ty;

    crt_FloatToInt64(dst->x, &dst->fixedXLow);
    crt_FloatToInt64(dst->y, &dst->fixedYLow);
    if (dst->pixelX < 0) {
        dst->pixelX = 0;
    }
    if (dst->pixelY < 0) {
        dst->pixelY = 0;
    }
    dst->xBits =
        (*(int *)&ctx->intToFloat[dst->pixelX]);
    dst->yBits =
        (*(int *)&ctx->intToFloat[dst->pixelY]);
    tx = dst->x - src->x;
    ty = dst->y - src->y;
    Gfx_RasterInterpolateVertexRgbClamped(tx, ty, xf, src, dst, ctx->scale);
    dst->packedColor = node->attr14;
    dst->packedColor = src->packedColor;
    dst->resolved = 1;
}

// Converts the destination X and Y to 64-bit values (fixedXLow and fixedYLow), clamps pixelX and
// pixelY to zero when negative, and replaces X and Y with the intToFloat entries those indices
// select. The deltas from src go to Gfx_RasterInterpolateVertexZ and
// Gfx_RasterInterpolateVertexUv with the gradients and the scale object. Then src's packedColor is
// copied to dst and dst->resolved is set to 1.
// FUNCTION: FF7 0x006b7e84
void __cdecl Gfx_RasterSnapGeneratedVertexZUv(SwTriangleGradients *xf, void *unused, SwRasterVertex *src, SwRasterVertex *dst, SwRasterData *ctx)
{
    float tx;
    float ty;

    crt_FloatToInt64(dst->x, &dst->fixedXLow);
    crt_FloatToInt64(dst->y, &dst->fixedYLow);
    if (dst->pixelX < 0) {
        dst->pixelX = 0;
    }
    if (dst->pixelY < 0) {
        dst->pixelY = 0;
    }
    dst->xBits =
        (*(int *)&ctx->intToFloat[dst->pixelX]);
    dst->yBits =
        (*(int *)&ctx->intToFloat[dst->pixelY]);
    tx = dst->x - src->x;
    ty = dst->y - src->y;
    Gfx_RasterInterpolateVertexZ(tx, ty, xf, src, dst, ctx->scale);
    Gfx_RasterInterpolateVertexUv(tx, ty, xf, src, dst, ctx->scale);
    dst->packedColor = src->packedColor;
    dst->resolved = 1;
}

// Converts the destination X and Y to 64-bit values (fixedXLow and fixedYLow), clamps pixelX and
// pixelY to zero when negative, and replaces X and Y with the intToFloat entries those indices
// select. The deltas from src go to Gfx_RasterInterpolateVertexUv with the gradients and the scale
// object. Then src's packedColor is copied to dst and dst->resolved is set to 1.
// FUNCTION: FF7 0x006b7f88
void __cdecl Gfx_RasterSnapGeneratedVertexUv(SwTriangleGradients *xf, void *unused, SwRasterVertex *src, SwRasterVertex *dst, SwRasterData *ctx)
{
    float tx;
    float ty;

    crt_FloatToInt64(dst->x, &dst->fixedXLow);
    crt_FloatToInt64(dst->y, &dst->fixedYLow);
    if (dst->pixelX < 0) {
        dst->pixelX = 0;
    }
    if (dst->pixelY < 0) {
        dst->pixelY = 0;
    }
    dst->xBits =
        (*(int *)&ctx->intToFloat[dst->pixelX]);
    dst->yBits =
        (*(int *)&ctx->intToFloat[dst->pixelY]);
    tx = dst->x - src->x;
    ty = dst->y - src->y;
    Gfx_RasterInterpolateVertexUv(tx, ty, xf, src, dst, ctx->scale);
    dst->packedColor = src->packedColor;
    dst->resolved = 1;
}

// Converts the destination X and Y to signed 32.32 fixed point, then adds one to each integer word
// whose fractional word has bit 31 set. Negative integer words are clamped to zero, and X and Y are
// replaced with the intToFloat entries those words index. The deltas from src, taken after that
// replacement, go to Gfx_RasterInterpolateVertexZ and Gfx_RasterInterpolateVertexUv; then src's
// packedColor is copied to dst and dst->resolved is set to 1.
// FUNCTION: FF7 0x006b8066
void __cdecl Gfx_RasterSnapGeneratedVertexRoundedZUv(SwTriangleGradients *xf, void *unused, SwRasterVertex *src, SwRasterVertex *dst,
                                                     SwRasterData *ctx)
{
    float tx;
    float ty;

    crt_FloatToInt64(dst->x, &dst->fixedXLow);
    crt_FloatToInt64(dst->y, &dst->fixedYLow);
    if ((dst->fixedXLow & 0x80000000) != 0) {
        dst->pixelX = dst->pixelX + 1;
    }
    if ((dst->fixedYLow & 0x80000000) != 0) {
        dst->pixelY = dst->pixelY + 1;
    }
    if (dst->pixelX < 0) {
        dst->pixelX = 0;
    }
    if (dst->pixelY < 0) {
        dst->pixelY = 0;
    }
    dst->xBits =
        (*(int *)&ctx->intToFloat[dst->pixelX]);
    dst->yBits =
        (*(int *)&ctx->intToFloat[dst->pixelY]);
    tx = dst->x - src->x;
    ty = dst->y - src->y;
    Gfx_RasterInterpolateVertexZ(tx, ty, xf, src, dst, ctx->scale);
    Gfx_RasterInterpolateVertexUv(tx, ty, xf, src, dst, ctx->scale);
    dst->packedColor = src->packedColor;
    dst->resolved = 1;
}

// Converts the destination X and Y to signed 32.32 fixed point and rounds each integer word up by
// one when its fraction word has bit 31 set. Negative integer words are clamped to zero, and X and
// Y are then replaced by intToFloat entries. The deltas are taken from src after those
// replacements, Gfx_RasterInterpolateVertexUv is called, src's packedColor is copied to dst, and
// dst->resolved is set to 1.
// FUNCTION: FF7 0x006b81a7
void __cdecl Gfx_RasterSnapGeneratedVertexRoundedUv(SwTriangleGradients *xf, void *unused, SwRasterVertex *src, SwRasterVertex *dst,
                                                    SwRasterData *ctx)
{
    float tx;
    float ty;

    crt_FloatToInt64(dst->x, &dst->fixedXLow);
    crt_FloatToInt64(dst->y, &dst->fixedYLow);
    if ((dst->fixedXLow & 0x80000000) != 0) {
        dst->pixelX = dst->pixelX + 1;
    }
    if ((dst->fixedYLow & 0x80000000) != 0) {
        dst->pixelY = dst->pixelY + 1;
    }
    if (dst->pixelX < 0) {
        dst->pixelX = 0;
    }
    if (dst->pixelY < 0) {
        dst->pixelY = 0;
    }
    dst->xBits =
        (*(int *)&ctx->intToFloat[dst->pixelX]);
    dst->yBits =
        (*(int *)&ctx->intToFloat[dst->pixelY]);
    tx = dst->x - src->x;
    ty = dst->y - src->y;
    Gfx_RasterInterpolateVertexUv(tx, ty, xf, src, dst, ctx->scale);
    dst->packedColor = src->packedColor;
    dst->resolved = 1;
}

/*
 * Converts destination X/Y to signed 32.32 fixed point and computes deltas
 * from src before adjusting either coordinate. Increments each integer word
 * when fractional bit 31 is set, clamps negative indices and replaces X/Y from
 * the intToFloat table.
 */
// FUNCTION: FF7 0x006b82c2
void __cdecl Gfx_RasterSnapGeneratedVertexRoundedZUvPreDelta(SwTriangleGradients *xf, void *unused, SwRasterVertex *src, SwRasterVertex *dst,
                                                             SwRasterData *ctx)
{
    float tx;
    float ty;

    crt_FloatToInt64(dst->x, &dst->fixedXLow);
    crt_FloatToInt64(dst->y, &dst->fixedYLow);
    tx = dst->x - src->x;
    ty = dst->y - src->y;
    if ((dst->fixedXLow & 0x80000000) != 0) {
        dst->pixelX = dst->pixelX + 1;
    }
    if ((dst->fixedYLow & 0x80000000) != 0) {
        dst->pixelY = dst->pixelY + 1;
    }
    if (dst->pixelX < 0) {
        dst->pixelX = 0;
    }
    if (dst->pixelY < 0) {
        dst->pixelY = 0;
    }
    dst->xBits =
        (*(int *)&ctx->intToFloat[dst->pixelX]);
    dst->yBits =
        (*(int *)&ctx->intToFloat[dst->pixelY]);
    Gfx_RasterInterpolateVertexZ(tx, ty, xf, src, dst, ctx->scale);
    Gfx_RasterInterpolateVertexUv(tx, ty, xf, src, dst, ctx->scale);
    dst->packedColor = src->packedColor;
    dst->resolved = 1;
}

/*
 * Converts destination X/Y to signed 32.32 fixed point and computes deltas
 * from src before adjusting either coordinate. Increments each integer word
 * when fractional bit 31 is set, clamps negative indices and replaces X/Y from
 * the intToFloat table.
 */
// FUNCTION: FF7 0x006b8404
void __cdecl Gfx_RasterSnapGeneratedVertexRoundedUvPreDelta(SwTriangleGradients *xf, void *unused, SwRasterVertex *src, SwRasterVertex *dst,
                                                            SwRasterData *ctx)
{
    float tx;
    float ty;

    crt_FloatToInt64(dst->x, &dst->fixedXLow);
    crt_FloatToInt64(dst->y, &dst->fixedYLow);
    tx = dst->x - src->x;
    ty = dst->y - src->y;
    if ((dst->fixedXLow & 0x80000000) != 0) {
        dst->pixelX = dst->pixelX + 1;
    }
    if ((dst->fixedYLow & 0x80000000) != 0) {
        dst->pixelY = dst->pixelY + 1;
    }
    if (dst->pixelX < 0) {
        dst->pixelX = 0;
    }
    if (dst->pixelY < 0) {
        dst->pixelY = 0;
    }
    dst->xBits =
        (*(int *)&ctx->intToFloat[dst->pixelX]);
    dst->yBits =
        (*(int *)&ctx->intToFloat[dst->pixelY]);
    Gfx_RasterInterpolateVertexUv(tx, ty, xf, src, dst, ctx->scale);
    dst->packedColor = src->packedColor;
    dst->resolved = 1;
}

// Converts the destination X and Y to 64-bit integers (fixedXLow and fixedYLow), then clamps
// pixelX and pixelY to zero when negative. Each index selects an intToFloat entry that replaces
// the destination's X or Y. The deltas are taken from src and passed with the gradients and the
// scale object to Gfx_RasterInterpolateVertexZ and Gfx_RasterInterpolateVertexRgbClamped. Then
// src's packedColor is copied to dst and dst->resolved is set to 1.
// FUNCTION: FF7 0x006b8520
void __cdecl Gfx_RasterSnapGeneratedVertexZRgb(SwTriangleGradients *xf, void *unused, SwRasterVertex *src, SwRasterVertex *dst, SwRasterData *ctx)
{
    float tx;
    float ty;

    crt_FloatToInt64(dst->x, &dst->fixedXLow);
    crt_FloatToInt64(dst->y, &dst->fixedYLow);
    if (dst->pixelX < 0) {
        dst->pixelX = 0;
    }
    if (dst->pixelY < 0) {
        dst->pixelY = 0;
    }
    dst->xBits =
        (*(int *)&ctx->intToFloat[dst->pixelX]);
    dst->yBits =
        (*(int *)&ctx->intToFloat[dst->pixelY]);
    tx = dst->x - src->x;
    ty = dst->y - src->y;
    Gfx_RasterInterpolateVertexZ(tx, ty, xf, src, dst, ctx->scale);
    Gfx_RasterInterpolateVertexRgbClamped(tx, ty, xf, src, dst, ctx->scale);
    dst->packedColor = src->packedColor;
    dst->resolved = 1;
}

// Converts the destination X and Y to 64-bit integers (fixedXLow and fixedYLow), then clamps
// pixelX and pixelY to zero when negative. Each index selects an intToFloat entry that replaces
// the destination's X or Y. The deltas are taken from src and passed with the gradients and the
// scale object to Gfx_RasterInterpolateVertexRgbClamped. Then src's packedColor is copied to dst
// and dst->resolved is set to 1.
// FUNCTION: FF7 0x006b8624
void __cdecl Gfx_RasterSnapGeneratedVertexRgb(SwTriangleGradients *xf, void *unused, SwRasterVertex *src, SwRasterVertex *dst, SwRasterData *ctx)
{
    float tx;
    float ty;

    crt_FloatToInt64(dst->x, &dst->fixedXLow);
    crt_FloatToInt64(dst->y, &dst->fixedYLow);
    if (dst->pixelX < 0) {
        dst->pixelX = 0;
    }
    if (dst->pixelY < 0) {
        dst->pixelY = 0;
    }
    dst->xBits =
        (*(int *)&ctx->intToFloat[dst->pixelX]);
    dst->yBits =
        (*(int *)&ctx->intToFloat[dst->pixelY]);
    tx = dst->x - src->x;
    ty = dst->y - src->y;
    Gfx_RasterInterpolateVertexRgbClamped(tx, ty, xf, src, dst, ctx->scale);
    dst->packedColor = src->packedColor;
    dst->resolved = 1;
}

// Brings one vertex into the current view once and marks it done: resolved guards the whole body,
// and the body sets it. The context's scale object supplies scaleDepth, scaleU and scaleV, which
// scale depth and the source texture coordinates. The source vertex is the vertex's source, and
// its texU and texV feed texCoordU and texCoordV. The three color bytes of the source (rgb[2],
// rgb[1], rgb[0]) are resolved through the intToFloat table into red, green and blue, and the
// packed color word is stored in packedColor.
// FUNCTION: FF7 0x006b8702
void __cdecl Gfx_RasterPrepareVertexZUvRgb(SwRasterVertex *v, SwRasterData *ctx)
{
    SwScaleZUv *scale;
    GfxVertexTextured *vtx;
    unsigned char *rgb;

    if (v->resolved == 0) {
        scale = ctx->scale;
        v->resolved = 1;
        vtx = (GfxVertexTextured *)v->source;
        v->depth =
            v->depth * scale->scaleDepth;
        v->texCoordU =
            vtx->texU * scale->scaleU;
        v->texCoordV =
            vtx->texV * scale->scaleV;
        rgb = vtx->rgb;
        v->redBits =
            (*(int *)&ctx->intToFloat[rgb[2]]);
        v->greenBits =
            (*(int *)&ctx->intToFloat[rgb[1]]);
        v->blueBits =
            (*(int *)&ctx->intToFloat[rgb[0]]);
        v->packedColor = *(int *)rgb;
    }
}

// Scales the vertex's depth by the context's scale object once, guarded by the resolved flag,
// which the body sets. It also copies the source vertex's packed color into packedColor. The
// source vertex and the scale object are distinct pointers, and neither pointer is checked.
// FUNCTION: FF7 0x006b87c9
void __cdecl Gfx_RasterPrepareVertexZ(SwRasterVertex *vertex, SwRasterData *context)
{
    SwScaleZUv *scaleObj;
    int *srcPtr;
    GfxVertexTextured *src;

    if (vertex->resolved == 0) {
        scaleObj = context->scale;
        vertex->resolved = 1;
        src = (GfxVertexTextured *)vertex->source;
        vertex->depth *= scaleObj->scaleDepth;
        srcPtr = &src->packedColor;
        vertex->packedColor = *srcPtr;
    }
}

// Copies the packed color of the source vertex into packedColor once, guarded by resolved, which
// the body sets. The context's scale object is loaded into ctxObj but never used.
// FUNCTION: FF7 0x006b8821
void __cdecl Gfx_RasterPrepareVertexColorOnly(SwRasterVertex *vertex, SwRasterData *context)
{
    SwScaleZUv *ctxObj;
    int *srcPtr;
    GfxVertexTextured *src;

    if (vertex->resolved == 0) {
        ctxObj = context->scale;
        vertex->resolved = 1;
        src = (GfxVertexTextured *)vertex->source;
        srcPtr = &src->packedColor;
        vertex->packedColor = *srcPtr;
    }
}

// Scales the depth by the context's scale object once, guarded by resolved, which the body sets.
// texCoordU and texCoordV are the source's texU and texV scaled by the object's scaleU and scaleV.
// The source's packed color is copied into packedColor.
// FUNCTION: FF7 0x006b8867
void __cdecl Gfx_RasterPrepareVertexZUv(SwRasterVertex *vertex, SwRasterData *context)
{
    SwScaleZUv *scaleObj;
    int *srcPtr;
    GfxVertexTextured *src;

    if (vertex->resolved == 0) {
        scaleObj = context->scale;
        vertex->resolved = 1;
        src = (GfxVertexTextured *)(vertex->source);
        vertex->depth *= scaleObj->scaleDepth;
        vertex->texCoordU = src->texU * scaleObj->scaleU;
        vertex->texCoordV = src->texV * scaleObj->scaleV;
        srcPtr = &src->packedColor;
        vertex->packedColor = *srcPtr;
    }
}

/*
 * Gfx_RasterPrepareVertexZUvRgb without the first scale: the same guard, the same two products
 * and the same dword carried across, and no scaling of the depth.
 */
// FUNCTION: FF7 0x006b88e3
void __cdecl Gfx_RasterPrepareVertexUv(SwRasterVertex *v, SwRasterData *ctx)
{
    SwScaleZUv *scale;
    GfxVertexTextured *vtx;
    unsigned char *rgb;

    if (v->resolved == 0) {
        scale = ctx->scale;
        v->resolved = 1;
        vtx = (GfxVertexTextured *)(v->source);
        v->texCoordU =
            vtx->texU * scale->scaleU;
        v->texCoordV =
            vtx->texV * scale->scaleV;
        rgb = vtx->rgb;
        v->packedColor = *(int *)rgb;
    }
}

// Scales the depth by the context's scale object once, guarded by resolved, which the body sets.
// The source's three palette indices (colorIndex2, colorIndex1, colorIndex0) are resolved through
// the context's intToFloat table into redBits, greenBits and blueBits. The packed source color is
// copied into packedColor.
// FUNCTION: FF7 0x006b894d
void __cdecl Gfx_RasterPrepareVertexZRgb(SwRasterVertex *vertex, SwRasterData *context)
{
    SwScaleZUv *scaleObj;
    SwRasterRgbPaletteIndices *srcPtr;
    GfxVertexTextured *src;

    if (vertex->resolved == 0) {
        scaleObj = context->scale;
        vertex->resolved = 1;
        src = (GfxVertexTextured *)vertex->source;
        vertex->depth *= scaleObj->scaleDepth;
        srcPtr = (SwRasterRgbPaletteIndices *)src->rgb;
        vertex->redBits = (*(int *)&context->intToFloat[srcPtr->colorIndex2]);
        vertex->greenBits = (*(int *)&context->intToFloat[srcPtr->colorIndex1]);
        vertex->blueBits = (*(int *)&context->intToFloat[srcPtr->colorIndex0]);
        vertex->packedColor = *(int *)srcPtr;
    }
}

// Resolves the source's three palette indices (colorIndex2, colorIndex1, colorIndex0) through the
// context's intToFloat table into redBits, greenBits and blueBits, once, guarded by resolved,
// which the body sets. The packed source color is copied into packedColor. The context's scale
// object is loaded into ctxObj but never used.
// FUNCTION: FF7 0x006b89f0
void __cdecl Gfx_RasterPrepareVertexRgb(SwRasterVertex *vertex, SwRasterData *context)
{
    SwScaleZUv *ctxObj;
    SwRasterRgbPaletteIndices *srcPtr;
    GfxVertexTextured *src;

    if (vertex->resolved == 0) {
        ctxObj = context->scale;
        vertex->resolved = 1;
        src = (GfxVertexTextured *)vertex->source;
        srcPtr = (SwRasterRgbPaletteIndices *)src->rgb;
        vertex->redBits = (*(int *)&context->intToFloat[srcPtr->colorIndex2]);
        vertex->greenBits = (*(int *)&context->intToFloat[srcPtr->colorIndex1]);
        vertex->blueBits = (*(int *)&context->intToFloat[srcPtr->colorIndex0]);
        vertex->packedColor = *(int *)srcPtr;
    }
}

/*
 * For a vertex whose clip flags are both clear, transforms its srcPos through the engine's screen
 * transform and the projection helper, converts X/Y and computes four viewport outcode bits.
 * In-bounds vertices resolve their coordinates through the intToFloat table and set clipXFlag.
 */
// FUNCTION: FF7 0x006b8a7d
void __cdecl Gfx_RasterProjectClippedVertex(GfxClipVertex *vertex, GlRasterClipBounds *bounds, SwRasterData *rasterData, GfxRasterContext *ctx)
{
    float clipPos[4];
    GfxEngineContext *engine;

    if (vertex->clipXFlag == 0 && vertex->clipYFlag == 0) {
        engine = GetEngineContext();
        TransformPointByMatrix4x4(engine->screenTransform, vertex->srcPos, clipPos);
        PerspectiveDivideWithRhw((struct GfxHomogeneousVec4 *)clipPos, vertex);
        vertex->clipXFlag = 0;
        vertex->clipYFlag = 0;
        vertex->flags = 0;
        crt_FloatToInt64(vertex->x, &vertex->fixedXLow);
        crt_FloatToInt64(vertex->y, &vertex->fixedYLow);
        if (vertex->xIndex < bounds->clipMinX) {
            vertex->flags |= 1;
        }
        if (vertex->xIndex > bounds->clipMaxX) {
            vertex->flags |= 2;
        }
        if (vertex->yIndex < bounds->clipMinY) {
            vertex->flags |= 4;
        }
        if (vertex->yIndex > bounds->clipMaxY) {
            vertex->flags |= 8;
        }
        if (vertex->flags == 0) {
            *(int *)&vertex->x = *(int *)&((SwRasterData *)ctx)->intToFloat[vertex->xIndex];
            *(int *)&vertex->y = *(int *)&((SwRasterData *)ctx)->intToFloat[vertex->yIndex];
            vertex->clipXFlag = 1;
        }
        vertex->clipYFlag = 1;
        vertex->z *= bounds->scaleDepth;
        vertex->texU *= bounds->scaleU;
        vertex->texV *= bounds->scaleV;
    }
}

/*
 * Rejects a triangle when all three vertex clip-code words share any bit. If
 * their combined code includes mask 0x130, only prepare-enabled calls prepare
 * missing source UV/color fields, clip through
 * Gfx_RasterTransformAndClipTriangleDepthPlanes (mode four or five), transform
 * the resulting polygon vertices and recursively process its fan triangles
 * with prepare zero.
 */
// FUNCTION: FF7 0x006b8c11
void __cdecl Gfx_RasterClipAndDrawTriangle(int prepare, int flipWinding, GfxRasterVertex *v0, GfxRasterVertex *v1, GfxRasterVertex *v2, GfxRasterCallbacks *callbacks, GfxRasterContext *ctx)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[124];
        GfxRasterClipState *planeFanEntry;
        struct { unsigned char pad_to_4[0x4]; int clippedVertex; };
        struct { unsigned char pad_to_8[0x8]; int clipIndex; };
        struct { unsigned char pad_to_C[0xC]; int clippedPtrs[8]; };
        struct { unsigned char pad_to_2C[0x2C]; int planeVertCount; };
        struct { unsigned char pad_to_30[0x30]; GfxRasterClipScratch *planeScratch; };
        struct { unsigned char pad_to_34[0x34]; GfxRasterVertex *tmpVertex; };
        struct { unsigned char pad_to_38[0x38]; int wireframeFlag; };
        struct { unsigned char pad_to_3C[0x3C]; GfxRasterClipState *fanEntry; };
        struct { unsigned char pad_to_40[0x40]; int polyIndex; };
        struct { unsigned char pad_to_44[0x44]; GfxClipVertex *clipVertex; };
        struct { unsigned char pad_to_48[0x48]; SwRasterRgbPaletteIndices *colorIndices2; };
        struct { unsigned char pad_to_4C[0x4C]; SwRasterRgbPaletteIndices *colorIndices1; };
        struct { unsigned char pad_to_50[0x50]; SwRasterRgbPaletteIndices *colorIndices0; };
        struct { unsigned char pad_to_54[0x54]; GfxRasterClipScratch *clipScratch; };
        struct { unsigned char pad_to_58[0x58]; int polyVertCount; };
        struct { unsigned char pad_to_5C[0x5C]; int lineContext; };
        struct { unsigned char pad_to_60[0x60]; GfxRasterCallbacks *rasterCallbacks; };
        struct { unsigned char pad_to_64[0x64]; GfxRasterTriangle *triangle; };
        struct { unsigned char pad_to_68[0x68]; GlRasterClipBounds *clipBounds; };
        struct { unsigned char pad_to_6C[0x6C]; int stateBlock; };
        struct { unsigned char pad_to_70[0x70]; int combinedClipMask; };
        struct { unsigned char pad_to_74[0x74]; GfxEngineContext *engineContext; };
        struct { unsigned char pad_to_78[0x78]; int sharedClipMask; };
    } frame;
#pragma pack(pop)

    frame.engineContext = GetEngineContext();
    frame.sharedClipMask = v0->clipMask & v1->clipMask & v2->clipMask;
    if (frame.sharedClipMask != 0) {
        return;
    }
    frame.stateBlock = (int)ctx + 0xF4;
    frame.triangle = (GfxRasterTriangle *)((int)ctx + 0x48);
    frame.rasterCallbacks = (GfxRasterCallbacks *)((int)ctx + 0x154);
    frame.lineContext = ctx->lineContext;
    frame.clipBounds = (GlRasterClipBounds *)(ctx->mtxSet);
    frame.combinedClipMask = v0->clipMask | v1->clipMask | v2->clipMask;
    if ((frame.combinedClipMask & 0x130) != 0) {
        if (prepare != 0) {
            frame.clipScratch = (GfxRasterClipScratch *)((int)ctx + 0x1484);
            if (v0->flags == 0) {
                v0->srcAttr0 = ((GfxRasterSourceVertex *)(v0->srcPtr))->srcAttr0;
                v0->srcAttr1 = ((GfxRasterSourceVertex *)(v0->srcPtr))->srcAttr1;
                frame.colorIndices0 = (SwRasterRgbPaletteIndices *)(v0->srcPtr + 0x10);
                v0->paletteEntry2 = *(int *)&((SwRasterData *)ctx)->intToFloat[frame.colorIndices0->colorIndex2];
                v0->paletteEntry1 = *(int *)&((SwRasterData *)ctx)->intToFloat[frame.colorIndices0->colorIndex1];
                v0->paletteEntry0 = *(int *)&((SwRasterData *)ctx)->intToFloat[frame.colorIndices0->colorIndex0];
                v0->srcWord = *(int *)frame.colorIndices0;
                v0->resolved = 0;
            }
            if (v1->flags == 0) {
                v1->srcAttr0 = ((GfxRasterSourceVertex *)(v1->srcPtr))->srcAttr0;
                v1->srcAttr1 = ((GfxRasterSourceVertex *)(v1->srcPtr))->srcAttr1;
                frame.colorIndices1 = (SwRasterRgbPaletteIndices *)(v1->srcPtr + 0x10);
                v1->paletteEntry2 = *(int *)&((SwRasterData *)ctx)->intToFloat[frame.colorIndices1->colorIndex2];
                v1->paletteEntry1 = *(int *)&((SwRasterData *)ctx)->intToFloat[frame.colorIndices1->colorIndex1];
                v1->paletteEntry0 = *(int *)&((SwRasterData *)ctx)->intToFloat[frame.colorIndices1->colorIndex0];
                v1->srcWord = *(int *)frame.colorIndices1;
                v1->resolved = 0;
            }
            if (v2->flags == 0) {
                v2->srcAttr0 = ((GfxRasterSourceVertex *)(v2->srcPtr))->srcAttr0;
                v2->srcAttr1 = ((GfxRasterSourceVertex *)(v2->srcPtr))->srcAttr1;
                frame.colorIndices2 = (SwRasterRgbPaletteIndices *)(v2->srcPtr + 0x10);
                v2->paletteEntry2 = *(int *)&((SwRasterData *)ctx)->intToFloat[frame.colorIndices2->colorIndex2];
                v2->paletteEntry1 = *(int *)&((SwRasterData *)ctx)->intToFloat[frame.colorIndices2->colorIndex1];
                v2->paletteEntry0 = *(int *)&((SwRasterData *)ctx)->intToFloat[frame.colorIndices2->colorIndex0];
                v2->srcWord = *(int *)frame.colorIndices2;
                v2->resolved = 0;
            }
            if ((frame.combinedClipMask & 0x100) != 0) {
                Gfx_RasterTransformAndClipTriangleDepthPlanes(v0, v1, v2, 4, frame.clipScratch, ctx);
            } else if ((frame.combinedClipMask & 0x20) != 0) {
                Gfx_RasterTransformAndClipTriangleDepthPlanes(v0, v1, v2, 5, frame.clipScratch, ctx);
            } else {
                Gfx_RasterTransformAndClipTriangleDepthPlanes(v0, v1, v2, 4, frame.clipScratch, ctx);
            }
            frame.polyVertCount = frame.clipScratch->vertCount;
            if (frame.polyVertCount >= 3) {
                for (frame.polyIndex = 0; frame.polyIndex < frame.polyVertCount; frame.polyIndex++) {
                    frame.clipVertex = (GfxClipVertex *)(((int *)(frame.clipScratch->destinationList))[frame.polyIndex]);
                    Gfx_RasterProjectClippedVertex(frame.clipVertex, frame.clipBounds, (SwRasterData *)((char *)ctx + 0x154), ctx);
                }
                frame.fanEntry = g_GfxClipPolygonFanIndices;
                for (frame.polyIndex = frame.polyVertCount - 2; frame.polyIndex > 0; frame.polyIndex--) {
                    Gfx_RasterClipAndDrawTriangle(0, flipWinding, (GfxRasterVertex *)(((int *)(frame.clipScratch->destinationList))[frame.fanEntry->vertexIndex]), (GfxRasterVertex *)(((int *)(frame.clipScratch->destinationList))[frame.fanEntry->secondIndex]), (GfxRasterVertex *)(((int *)(frame.clipScratch->destinationList))[frame.fanEntry->thirdIndex]), frame.rasterCallbacks, ctx);
                    frame.fanEntry++;
                }
            }
        }
    } else {
        if ((v0->x - v1->x) * (v2->y - v1->y) < (v0->y - v1->y) * (v2->x - v1->x)) {
            frame.wireframeFlag = 1;
        } else if (flipWinding != 0) {
            frame.tmpVertex = v0;
            v0 = v2;
            v2 = frame.tmpVertex;
            frame.wireframeFlag = 1;
        } else {
            frame.wireframeFlag = 0;
        }
        if (frame.wireframeFlag != 0) {
            if (DEAD_IN_RETAIL != 0) {
                frame.triangle->vertex0 = (int)v0;
                frame.triangle->vertex1 = (int)v1;
                frame.triangle->vertex2 = (int)v2;
                Gfx_RasterDrawTriangleWireframe(0xFFFF, frame.triangle, frame.engineContext);
            }
            frame.planeScratch = (GfxRasterClipScratch *)((int)ctx + 0x190);
            Gfx_RasterClipTriangleToPlaneMask(frame.combinedClipMask, v0, v1, v2, 3, frame.planeScratch);
            frame.planeVertCount = frame.planeScratch->vertCount;
            if (frame.planeVertCount >= 3) {
                frame.triangle->vertex0 = (int)v0;
                frame.triangle->vertex1 = (int)v1;
                frame.triangle->vertex2 = (int)v2;
                if (v0->flags == 0) {
                    frame.rasterCallbacks->projectVertex((int)v0, (int)ctx);
                }
                if (v1->flags == 0) {
                    frame.rasterCallbacks->projectVertex((int)v1, (int)ctx);
                }
                if (v2->flags == 0) {
                    frame.rasterCallbacks->projectVertex((int)v2, (int)ctx);
                }
                frame.rasterCallbacks->prepareCallback(frame.stateBlock, frame.lineContext, (int)frame.triangle);
                for (frame.clipIndex = 0; frame.clipIndex < frame.planeVertCount; frame.clipIndex++) {
                    frame.clippedVertex = *(int *)((char *)frame.planeScratch->destinationList + frame.clipIndex * 4);
                    if (((GfxRasterVertex *)frame.clippedVertex)->flags != 0) {
                        frame.clippedPtrs[frame.clipIndex] = frame.clippedVertex;
                    } else {
                        /* Invokes untyped callback slot `frame.rasterCallbacks->snapVertex` with its runtime (int, int, int, int, int) -> int signature. */
                        ((int(__cdecl *)(int, int, int, int, int)) frame.rasterCallbacks->snapVertex)(frame.stateBlock, frame.lineContext, (int)v0, frame.clippedVertex, (int)ctx);
                        frame.clippedPtrs[frame.clipIndex] = frame.clippedVertex;
                    }
                }
                frame.planeFanEntry = g_GfxClipPolygonFanIndices;
                for (frame.clipIndex = frame.planeVertCount - 2; frame.clipIndex > 0; frame.clipIndex--) {
                    frame.rasterCallbacks->drawTriangle(frame.clippedPtrs[frame.planeFanEntry->vertexIndex], frame.clippedPtrs[frame.planeFanEntry->secondIndex], frame.clippedPtrs[frame.planeFanEntry->thirdIndex], (int)frame.rasterCallbacks, (int)ctx);
                    frame.planeFanEntry++;
                }
            }
        }
    }
}
// clang-format off
// GLOBAL: FF7 0x00919800
struct GfxRasterClipState g_GfxClipPolygonFanIndices[8] = {{0, 1, 2}, {2, 3, 0}, {3, 4, 0}, {4, 5, 0}, {5, 6, 0}, {6, 7, 0}, {7, 8, 0}, {8, 9, 0}};
unsigned char g_rawData_00919860[28] = {
    0xe0, 0xff, 0x00, 0x00, 0xff, 0x07, 0x00, 0x00, 0x1f, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0xf8, 0x00, 0x00,
    0xe0, 0x07, 0x00, 0x00
};
unsigned char g_rawData_0091987c[4] = {0x1f, 0x00, 0x00, 0x00};
// clang-format on
} /* extern "C" */
