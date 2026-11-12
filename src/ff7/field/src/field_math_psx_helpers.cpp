// src/ff7/field/src/field_math_psx_helpers.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Small math and PSX helpers for the field: constant predicates, fixed-point square root,
//   vector cross product, normalization, subtraction and point height, and decoding of PSX texture
//   page and CLUT attributes.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/types/field.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "ff7/api/field.h"
extern "C" {

#pragma intrinsic(abs)

#pragma intrinsic(pow)
#pragma intrinsic(sqrt)

void __cdecl Field_DecodeClutToPaletteRow(int clut, void *row);
void __cdecl Field_PackTileDepth(void *dst, int depthLow, int depthHigh);
#pragma intrinsic(memcpy)
#pragma function(memcpy)

/*
 * Constant true predicate.
 */
// FUNCTION: FF7 0x00649de0
int __cdecl Field_ReturnTrue_1(void)
{
    return 1;
}

/*
 * Constant true predicate.
 */
// FUNCTION: FF7 0x00649dea
int __cdecl Field_ReturnTrue_2(void)
{
    return 1;
}

/*
 * Constant false predicate.
 */
// FUNCTION: FF7 0x00649df4
int __cdecl Field_ReturnFalse(void)
{
    return 0;
}

/*
 * Computes square root in 12-bit fixed-point format.
 */
// FUNCTION: FF7 0x00649dfb
int __cdecl Field_SqrtFixed12(int fixedValue)
{
    float real;

    real = fixedValue / 4096.0f;
    return (int)(sqrt(real) * 4096.0);
}

/*
 * Computes 3D cross product of two 32-bit integer vectors.
 */
// FUNCTION: FF7 0x00649e1f
void __cdecl Field_VecCross32(FieldVec3i *vecA, FieldVec3i *vecB, FieldVec3i *out)
{
    out->x = vecA->y * vecB->z - vecA->z * vecB->y;
    out->y = vecA->z * vecB->x - vecA->x * vecB->z;
    out->z = vecA->x * vecB->y - vecA->y * vecB->x;
}

/*
 * Forwards to fixed-point vector normalization function.
 */
// FUNCTION: FF7 0x00649e85
void __cdecl Field_NormalizeVectorFixedThunk(int vector, int out)
{
    NormalizeVectorFixed((void *)vector, (void *)out);
}

/*
 * Decodes PSX texture page attributes into mode and coordinate offsets.
 */
// FUNCTION: FF7 0x00649e9a
void __cdecl Field_DecodeTPage(short tpage, void *colorMode, void *pageX, void *pageY, void *transparency)
{
    *(int *)colorMode = (*(int *)&tpage & 0xFFFF) >> 7 & 3;
    *(short *)pageX = (*(int *)&tpage & 0xFFFF) << 6 & 0x7C0;
    *(short *)pageY = ((*(int *)&tpage & 0xFFFF) << 4 & 0x100) + ((*(int *)&tpage & 0xFFFF) >> 2 & 0x200);
    *(int *)transparency = (*(int *)&tpage & 0xFFFF) >> 5 & 3;
}

/*
 * Decodes PSX CLUT attribute into palette row index.
 */
// FUNCTION: FF7 0x00649f0a
void __cdecl Field_DecodeClutToPaletteRow(int clut, void *row)
{
    *(short *)row = ((clut & 0xFFFF) >> 6) - 0x1E0;
}

/*
 * Packs two 8-bit depth components into a 16-bit word.
 */
// FUNCTION: FF7 0x00649f25
void __cdecl Field_PackTileDepth(void *dst, int depthLow, int depthHigh)
{
    *(short *)dst = (depthLow & 0xFF) + ((depthHigh & 0xFF) << 8);
}

/*
 * Subtracts two 16-bit 3D vectors and writes result as 32-bit vector.
 */
// FUNCTION: FF7 0x00649f46
void __cdecl Field_VecSub16To32(FieldVec3i *dst, FieldVec16 *vecA, FieldVec16 *vecB)
{
    dst->x = vecA->x - vecB->x;
    dst->y = vecA->y - vecB->y;
    dst->z = vecA->z - vecB->z;
}

/*
 * Computes interpolated height of a 2D point inside a 3D triangle plane.
 */
// FUNCTION: FF7 0x00649f8a
int __cdecl Field_PointHeightInTriangle(FieldVec3i *vec, FieldVec3i *edge, FieldPointTo *basePoint, FieldVec16 *point)
{
    int planeOffset;
    int denom;
    int unusedSpace; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int normalY;
    int crossX;

    crossX = -vec->y * edge->z + edge->y * vec->z;
    normalY = -vec->z * edge->x + vec->x * edge->z;
    denom = -vec->x * edge->y + edge->x * vec->y;
    vec->x = point->x;
    vec->y = point->y;
    vec->z = point->z;
    planeOffset = crossX * vec->x + normalY * vec->y + denom * vec->z - crossX * basePoint->x - normalY * basePoint->y;
    if (denom != 0) {
        return planeOffset / denom;
    } else {
        return planeOffset;
    }
}
} /* extern "C" */
