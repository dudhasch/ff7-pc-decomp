// src/lib/src/graphics/sw/sw_gfx_raster_gradient_basis.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Edge basis and gradient helpers for the software triangle rasterizer: builds the 2x2
//   edge basis of a triangle, inverts it, and computes the cross-product gradient terms.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/types/graphics.h"
#include "lib/api/graphics.h"
#include "lib/crt.h"
#include "lib/codegen.h"
extern "C" {

// clang-format off
// clang-format on

struct GfxRasterUvDepthGradients;


/*
 * Build the two-by-two edge basis of a triangle and invert it. Four differences
 * from the first argument fill the first four words of the output, and the two
 * cross products of those four give the reciprocal of the determinant and of its
 * negation.
 */
// FUNCTION: FF7 0x006bc180
void __cdecl Gfx_RasterBuildTriangleEdgeBasis(float *vtxA, float *vtxB, float *vtxC, GfxRasterUvDepthGradients *out)
{
    float crossA;
    float productB;

    out->deltaBa = *vtxB - *vtxA;
    out->deltaCa = *vtxC - *vtxA;
    out->scaleC =
        vtxB[1] - vtxA[1];
    out->scaleB =
        vtxC[1] - vtxA[1];
    crossA = out->deltaBa * out->scaleB;
    productB = out->deltaCa * out->scaleC;
    out->invDenominator = 1.0f / (crossA - productB);
    out->invDenominatorNeg = 1.0f / (productB - crossA);
}

/*
 * Identical to Gfx_RasterBuildTriangleEdgeBasis; the original ships two byte-identical copies.
 */
// FUNCTION: FF7 0x006bc20e
void __cdecl Gfx_RasterTriangleGradientSetup(float *vtxA, float *vtxB, float *vtxC, GfxRasterUvDepthGradients *out)
{
    float crossA;
    float productB;

    out->deltaBa = *vtxB - *vtxA;
    out->deltaCa = *vtxC - *vtxA;
    out->scaleC =
        vtxB[1] - vtxA[1];
    out->scaleB =
        vtxC[1] - vtxA[1];
    crossA = out->deltaBa * out->scaleB;
    productB = out->deltaCa * out->scaleC;
    out->invDenominator = 1.0f / (crossA - productB);
    out->invDenominatorNeg = 1.0f / (productB - crossA);
}

/*
 * Cross-product term of a 2D edge function: (dx * dxCoef - dy * dyCoef) * scaleX.
 */
// FUNCTION: FF7 0x006bc29c
float __cdecl Gfx_RasterTriangleGradientX(float dx, float dy, SwGradientMatrixX *matrix)
{
    return (dx * matrix->dxCoef - dy * matrix->dyCoef) * matrix->scaleX;
}

/*
 * Cross-product term of a 2D edge function: (dx * dxCoef - dy * dyCoef) * scaleY.
 */
// FUNCTION: FF7 0x006bc2bb
float __cdecl Gfx_RasterTriangleGradientY(float dx, float dy, SwGradientMatrixY *matrix)
{
    return (dx * matrix->dxCoef - dy * matrix->dyCoef) * matrix->scaleY;
}
} /* extern "C" */
