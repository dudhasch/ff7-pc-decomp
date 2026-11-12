// src/lib/src/graphics/sw/sw_gfx_raster_seed_gradients.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Computes the per-edge slope terms and seeds the depth, colour and texture-coordinate
//   gradients of the fixed-point raster block from transformed vertices.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/types/graphics.h"
#include "lib/api/graphics.h"
#include "lib/crt.h"
#include "lib/codegen.h"
extern "C" {

// clang-format off
// clang-format on

struct GfxRasterVertexTexV;
struct SwRasterSetupState;
struct SwRasterSeedBlock;
struct GfxRasterVertexSeed;

void __cdecl crt_FloatToInt64(float value, void *out);

/*
 * The edge setup for one pair of transformed vertices: the difference in x, the
 * difference in pixel y, and the slope that comes of multiplying the first by the
 * reciprocal table. The vertex array is indexed twice per difference and the
 * compiler reloads both the index and the base each time, which is what a frame of
 * zero and no saved register look like.
 */
// FUNCTION: FF7 0x006bbdc0
void __cdecl Gfx_RasterComputeEdgeXSlope(int i0, int i1, GlRasterEdgeSlope *out, SwRasterVertex **verts, SwRasterData *ctx)
{
    out->xSlope =
        verts[i1]->x - verts[i0]->x;
    out->deltaY = verts[i1]->pixelY -
                                  verts[i0]->pixelY;
    out->rowCount = out->deltaY;
    out->x =
        out->xSlope *
        ctx->reciprocal[out->deltaY];
    crt_FloatToInt64(out->x, &out->xFixedLow);
}

/*
 * Gfx_RasterComputeEdgeXSlope for an edge whose difference is taken in the integer pixel
 * coordinates rather than in x. A zero difference short-circuits the whole
 * slope, including the crt_FloatToInt64 that would fix it: the three stores go out
 * and the function returns.
 */
// FUNCTION: FF7 0x006bbe3d
void __cdecl Gfx_RasterComputeEdgeSlope(int i0, int i1, GfxRasterEdgeSlope *out, void **verts, struct GfxRasterIntermediates *ctx)
{
    out->dx = ((SwRasterVertex *)(verts[i1]))->pixelX -
                                ((SwRasterVertex *)(verts[i0]))->pixelX;
    out->dy = ((SwRasterVertex *)(verts[i1]))->pixelY -
                                  ((SwRasterVertex *)(verts[i0]))->pixelY;
    out->rowCount = out->dy;
    if (out->dx == 0) {
        out->slope = 0.0f;
        out->slopeFixedHigh = 0;
        out->slopeFixedLow = 0;
    } else {
        if (out->dx >= 0) {
            out->slope =
                ((SwRasterData *)ctx)->intToFloat[out->dx] *
                ((SwRasterData *)ctx)->reciprocal[out->dy];
        } else {
            out->slope =
                -(((SwRasterData *)ctx)->intToFloat[-out->dx] *
                  ((SwRasterData *)ctx)->reciprocal[out->dy]);
        }
        crt_FloatToInt64(out->slope, &out->slopeFixedLow);
    }
}

// One step of a two-axis walk. The depth u is converted straight into depthFixedLow, and
// depthSeed (the high word of that pair) is advanced by depthOffset before the pair is carried
// to depthCopyLow; v is xf->depthSource times t plus xf->depthOffset and is converted into
// nextDepthFixedLow. The copy loads both dwords before it stores either, so it is the Int64Pair
// assignment and not two ints.
// FUNCTION: FF7 0x006bbf26
void __cdecl Gfx_RasterSeedDepthFromVertex(float t, float u, GfxRasterVertexTexV *xf, SwRasterSetupState *out)
{
    float v;

    v = xf->depthSource * t + xf->depthOffset;
    crt_FloatToInt64(u, &out->depthFixedLow);
    out->depthSeed =
        out->depthSeed + out->depthOffset;
    *(Int64Pair *)&out->depthCopyLow = *(Int64Pair *)&out->depthFixedLow;
    crt_FloatToInt64(v, &out->nextDepthFixedLow);
}

/*
 * Convert one axis of a transform into the fixed-point block: the value straight
 * through, and the scaled-and-offset one after it. Two int64 conversions and one
 * int64 copy.
 */
// FUNCTION: FF7 0x006bbf8d
void __cdecl Gfx_RasterSeedColorChannel1FromVertex(float scale, float value, GfxRasterVertexTexV *src, SwRasterSeedBlock *dst)
{
    float t;

    t = src->channelSource1 * scale +
        src->axisValue1;
    crt_FloatToInt64(value, &dst->channel1StartLow);
    *(Int64Pair *)&dst->channel1CopyLow =
        *(Int64Pair *)&dst->channel1StartLow;
    crt_FloatToInt64(t, &dst->channel1EndLow);
}

/*
 * Gfx_RasterSeedColorChannel1FromVertex for the second channel. Two int64 conversions and one
 * int64 copy.
 */
// FUNCTION: FF7 0x006bbff1
void __cdecl Gfx_RasterSeedColorChannel2FromVertex(float scale, float value, GfxRasterVertexTexV *src, SwRasterSeedBlock *dst)
{
    float t;

    t = src->channelSource2 * scale +
        src->axisValue2;
    crt_FloatToInt64(value, &dst->channel2StartLow);
    *(Int64Pair *)&dst->channel2CopyLow =
        *(Int64Pair *)&dst->channel2StartLow;
    crt_FloatToInt64(t, &dst->channel2EndLow);
}

/*
 * Gfx_RasterSeedColorChannel1FromVertex for the third channel. Two int64 conversions and one int64
 * copy.
 */
// FUNCTION: FF7 0x006bc055
void __cdecl Gfx_RasterSeedColorChannel3FromVertex(float scale, float value, GfxRasterVertexTexV *src, SwRasterSeedBlock *dst)
{
    float t;

    t = src->channelSource3 * scale +
        src->axisValue3;
    crt_FloatToInt64(value, &dst->channel3StartLow);
    *(Int64Pair *)&dst->channel3CopyLow =
        *(Int64Pair *)&dst->channel3StartLow;
    crt_FloatToInt64(t, &dst->channel3EndLow);
}

// Computes u as src->texUSource times scale plus src->texUOffset. It converts x into startUFixedLow,
// copies that eight-byte pair to startUCopyLow, and converts u into endUFixedLow.
// FUNCTION: FF7 0x006bc0b9
void __cdecl Gfx_RasterSeedTextureCoordUFromVertex(float scale, float x, GfxRasterVertexTexV *src, GfxRasterVertexSeed *dst)
{
    float u;

    u = src->texUSource * scale + src->texUOffset;
    crt_FloatToInt64(x, &dst->startUFixedLow);
    *(Int64Pair *)&dst->startUCopyLow = *(Int64Pair *)&dst->startUFixedLow;
    crt_FloatToInt64(u, &dst->endUFixedLow);
}

/*
 * The V counterpart of Gfx_RasterSeedTextureCoordUFromVertex, using the transform's texVSource and
 * texVOffset gradients.
 */
// FUNCTION: FF7 0x006bc117
void __cdecl Gfx_RasterSeedTextureCoordVFromVertex(float t, float u, GfxRasterVertexTexV *xf, GfxRasterVertexSeed *out)
{
    float v;

    v = xf->texVSource * t + xf->texVOffset;
    crt_FloatToInt64(u, &out->startVFixedLow);
    *(Int64Pair *)&out->startVCopyLow = *(Int64Pair *)&out->startVFixedLow;
    crt_FloatToInt64(v, &out->endVFixedLow);
}
} /* extern "C" */
