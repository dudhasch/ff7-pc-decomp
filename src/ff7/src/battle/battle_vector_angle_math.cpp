// src/ff7/src/battle/battle_vector_angle_math.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Fixed-point vector helpers for battle effects: angle between vectors, axis-angle
//   rotation matrices, plane height deltas, and selection of the effect draw list buffer.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/battle.h"
#include "ff7/gte.h"
#include "lib/crt.h"

#include "ff7/api/battle.h"
#include "lib/api/graphics.h"
#include "lib/types/graphics.h"
#include "ff7/types/battle.h"

#include "lib/codegen.h"
extern "C" {

unsigned char g_bssPad_00c06a50[16] = {0};
// clang-format off
// GLOBAL: FF7 0x00c06a60
unsigned char *g_BattleFxDrawListBufferTaskSlotPtr = 0;
unsigned char g_bssPad_00c06a64[4] = {0};
// GLOBAL: FF7 0x00c06a68
unsigned char g_BattleFxDrawListBuffer0[0xc000] = {0};
// GLOBAL: FF7 0x00c12a68
unsigned char g_BattleFxDrawListBuffer1[0xc000] = {0};
// GLOBAL: FF7 0x00c1ea68
int g_BattleFxActiveDrawListPtr = 0;
unsigned char g_bssPad_00c1ea6c[4] = {0};
// clang-format on

/*
 * Computes fixed dot and cross product, cross magnitude square-root and atan2 angle, then
 * normalizes output cross vector in place and returns angle. Overflow/zero magnitude depend on
 * helpers.
 */
// FUNCTION: FF7 0x005d3240
unsigned int __cdecl ComputeAngleBetweenVectors(struct VECTOR *vecA, struct VECTOR *vecB, struct VECTOR *crossOut)
{
    int angle;
    int padGap[4]; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int dotProduct;
    int magnitude;

    dotProduct = vecA->vx * vecB->vx + vecA->vy * vecB->vy + vecA->vz * vecB->vz >> 0xC;
    CrossProductFixed12(vecA, vecB, crossOut);
    magnitude = crossOut->vx * crossOut->vx + crossOut->vy * crossOut->vy + crossOut->vz * crossOut->vz >> 0xC;
    magnitude = SquareRoot12(magnitude);
    angle = ArcTangentFixed(magnitude, dotProduct);
    NormalizeVectorFixed(&crossOut->vx, &crossOut->vx);
    return angle;
}

/*
 * Builds nine signed-short matrix coefficients from sine/cosine and three input axis dwords using
 * staged 12-bit fixed-point products and axis-angle cross terms. Does not normalize axis or write
 * translation; product overflow and short narrowing retain target arithmetic assumptions.
 */
// FUNCTION: FF7 0x005d32f1
void __cdecl Gte_BuildAxisAngleMatrix(int angle, struct PsxMatrix *matrix, struct VECTOR *axis)
{
    int ay;
    int yy;
    int cosVal;
    int sinValue;
    int zz;
    int vz;
    int axisX;
    int oneMinus;
    int xx;

    sinValue = SinFixed(angle);
    cosVal = CosFixed(angle);
    oneMinus = 0x1000 - cosVal;
    axisX = axis->vx;
    ay = axis->vy;
    vz = axis->vz;
    xx = axisX * axisX >> 0xC;
    yy = ay * ay >> 0xC;
    zz = vz * vz >> 0xC;
    matrix->m[0][0] = xx + ((0x1000 - xx) * cosVal >> 0xC);
    matrix->m[0][1] = ((axisX * ay >> 0xC) * oneMinus >> 0xC) - (vz * sinValue >> 0xC);
    matrix->m[0][2] = ((axisX * vz >> 0xC) * oneMinus >> 0xC) + (ay * sinValue >> 0xC);
    matrix->m[1][0] = ((axisX * ay >> 0xC) * oneMinus >> 0xC) + (vz * sinValue >> 0xC);
    matrix->m[1][1] = yy + ((0x1000 - yy) * cosVal >> 0xC);
    matrix->m[1][2] = ((ay * vz >> 0xC) * oneMinus >> 0xC) - (axisX * sinValue >> 0xC);
    matrix->m[2][0] = ((axisX * vz >> 0xC) * oneMinus >> 0xC) - (ay * sinValue >> 0xC);
    matrix->m[2][1] = ((ay * vz >> 0xC) * oneMinus >> 0xC) + (axisX * sinValue >> 0xC);
    matrix->m[2][2] = zz + ((0x1000 - zz) * cosVal >> 0xC);
}

/*
 * Computes fixed dot product of point-minus-origin with int normal, then subtracts dot-scaled
 * normal from three short point components. Assumes suitable normal magnitude; no normalization or
 * overflow guard.
 */
// FUNCTION: FF7 0x005d348d
void __cdecl Gte_ProjectPointOntoPlaneUnused(Vec3sPacked *point, Vec3sPacked *origin, BattlePlaneNormal *normal)
{
    int diffY;
    int dz;
    int padSlot; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int dotResult;
    int dx;

    dx = point->vx - origin->vx;
    diffY = point->vy - origin->vy;
    dz = point->vz - origin->vz;
    dotResult = normal->normalX * dx + normal->normalY * diffY + normal->normalZ * dz >> 0xC;
    point->vx -= (short)(dotResult * normal->normalX >> 0xC);
    point->vy -= (short)(dotResult * normal->normalY >> 0xC);
    point->vz -= (short)(dotResult * normal->normalZ >> 0xC);
}

/*
 * Computes two fixed point-normal dot products and divides their difference by dot of normal
 * with(0, -4096, 0) shifted twelve. Returns narrowed short vertical delta.
 */
// FUNCTION: FF7 0x005d354c
short __cdecl Gte_PlaneHeightDelta(Vec3sPacked *pointA, Vec3sPacked *pointB, BattlePlaneNormal *normal)
{
    int dotStart;
    int heightDelta;
    int divisor;
    int dotTail;
    int padWord; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int upY;
    int upComp;
    int xComp;

    xComp = 0;
    upY = 0xFFFFF000;
    upComp = 0;
    dotStart = pointA->vx * normal->normalX + pointA->vy * normal->normalY + pointA->vz * normal->normalZ;
    dotTail = pointB->vx * normal->normalX + pointB->vy * normal->normalY + pointB->vz * normal->normalZ;
    divisor = xComp * normal->normalX + upY * normal->normalY + upComp * normal->normalZ >> 0xC;
    heightDelta = (dotTail - dotStart) / divisor;
    return heightDelta;
}

/*
 * Sets g_BattleFxActiveDrawListPtr to g_BattleFxDrawListBuffer0 or g_BattleFxDrawListBuffer1 according to g_BattleDrawFrameParity; when g_BattleSecondaryTaskActiveCount
 * is at most one, writes minus one through g_BattleFxDrawListBufferTaskSlotPtr. Addresses alone do not prove two
 * allocation extents.
 */
// FUNCTION: FF7 0x005d3601
void __cdecl BattleFx_SelectDrawListBuffer(void)
{
    unsigned char *buf;

    if (g_BattleDrawFrameParity == 0) {
        buf = g_BattleFxDrawListBuffer0;
    } else {
        buf = g_BattleFxDrawListBuffer1;
    }
    g_BattleFxActiveDrawListPtr = (int)buf;
    if (g_BattleSecondaryTaskActiveCount <= 1) {
        *(int *)g_BattleFxDrawListBufferTaskSlotPtr = -1;
    }
}
} /* extern "C" */
