// src/ff7/coaster/coaster_clip_plane_tests.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Frustum clip-plane tests for the roller coaster: computes the left and right clip
//   planes, then tests points, distances and spheres against them.
/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/types/coaster.h"
#include <stdlib.h>
#include <string.h>
#include "ff7/coaster_psxdata.h"
#include "ff7/coaster_renderer.h"
#include "ff7/api/coaster.h"
#include "lib/api/graphics.h"
#include "lib/pfile.h"


#pragma intrinsic(abs)
extern "C" {

#pragma intrinsic(memset)

int __cdecl Mini_CoasterPointInsideClipPlanesVec16(MFVec16 *);

// clang-format off
// GLOBAL: FF7 0x00c5d328
CoasterHalfSpaceState g_CoasterClipPlanes = {0};
/*
 * Computes left and right frustum clip plane normals, offsets, helper values,
 * and normal lengths from standard field-of-view corners.
 */
// FUNCTION: FF7 0x005eea50
void __cdecl Mini_CoasterBuildClipPlanes(void)
{
    VECTOR topLeft = {-160, 120, 256};
    VECTOR urCorner = {160, 120, 256};
    VECTOR bottomRemaining = {-160, -120, 256};
    VECTOR lrCorner = {160, -120, 256};
    VECTOR smallTL, tempEdgeBL, dirTR, vecBR8;

    tempEdgeBL.vx = bottomRemaining.vx >> 2;
    tempEdgeBL.vy = bottomRemaining.vy >> 2;
    tempEdgeBL.vz = bottomRemaining.vz >> 2;
    smallTL.vx = topLeft.vx >> 2;
    smallTL.vy = topLeft.vy >> 2;
    smallTL.vz = topLeft.vz >> 2;
    vecBR8.vx = lrCorner.vx >> 2;
    vecBR8.vy = lrCorner.vy >> 2;
    vecBR8.vz = lrCorner.vz >> 2;
    dirTR.vx = urCorner.vx >> 2;
    dirTR.vy = urCorner.vy >> 2;
    dirTR.vz = urCorner.vz >> 2;

    CrossProductInt((struct VECTOR *)&tempEdgeBL, (struct VECTOR *)&smallTL, &g_CoasterClipPlanes.leftNormal);
    CrossProductInt((struct VECTOR *)&vecBR8, (struct VECTOR *)&dirTR, &g_CoasterClipPlanes.rightNormal);

    g_CoasterClipPlanes.leftDistance =
        -(g_CoasterClipPlanes.leftNormal.vx * (bottomRemaining.vx >> 2)) - (g_CoasterClipPlanes.leftNormal.vy * (bottomRemaining.vy >> 2)) - (g_CoasterClipPlanes.leftNormal.vz * (bottomRemaining.vz >> 2));
    g_CoasterClipPlanes.rightDistance =
        -(g_CoasterClipPlanes.rightNormal.vx * (lrCorner.vx >> 2)) - (g_CoasterClipPlanes.rightNormal.vy * (lrCorner.vy >> 2)) - (g_CoasterClipPlanes.rightNormal.vz * (lrCorner.vz >> 2));

    g_CoasterClipPlanes.leftHelper =
        g_CoasterClipPlanes.leftNormal.vx * (lrCorner.vx >> 2) + g_CoasterClipPlanes.leftNormal.vy * (lrCorner.vy >> 2) + g_CoasterClipPlanes.leftNormal.vz * (lrCorner.vz >> 2) + g_CoasterClipPlanes.leftDistance;
    g_CoasterClipPlanes.rightHelper =
        g_CoasterClipPlanes.rightNormal.vx * (bottomRemaining.vx >> 2) + g_CoasterClipPlanes.rightNormal.vy * (bottomRemaining.vy >> 2) + g_CoasterClipPlanes.rightNormal.vz * (bottomRemaining.vz >> 2) + g_CoasterClipPlanes.rightDistance;

    g_CoasterClipPlanes.leftNormalLength = SqrtToInt(
        g_CoasterClipPlanes.leftNormal.vx * g_CoasterClipPlanes.leftNormal.vx + g_CoasterClipPlanes.leftNormal.vy * g_CoasterClipPlanes.leftNormal.vy + g_CoasterClipPlanes.leftNormal.vz * g_CoasterClipPlanes.leftNormal.vz);
    g_CoasterClipPlanes.rightNormalLength = SqrtToInt(
        g_CoasterClipPlanes.rightNormal.vx * g_CoasterClipPlanes.rightNormal.vx + g_CoasterClipPlanes.rightNormal.vy * g_CoasterClipPlanes.rightNormal.vy + g_CoasterClipPlanes.rightNormal.vz * g_CoasterClipPlanes.rightNormal.vz);
}
/*
 * Tests whether a 32-bit integer point vector lies inside both the left and right clip planes.
 */
// FUNCTION: FF7 0x005eecb5
int Mini_CoasterPointInsideClipPlanesVec32(MFVec32 *v)
{
    struct {
        int dl, fr, fl, dr;
    } f; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    f.fl = 0;
    f.fr = f.fl;
    f.dl = (g_CoasterClipPlanes.leftNormal.vx * (v->x >> 2) + g_CoasterClipPlanes.leftNormal.vy * (v->y >> 2)) + (g_CoasterClipPlanes.leftDistance + g_CoasterClipPlanes.leftNormal.vz * (v->z >> 2));
    f.dr = (g_CoasterClipPlanes.rightNormal.vx * (v->x >> 2) + g_CoasterClipPlanes.rightNormal.vy * (v->y >> 2)) + (g_CoasterClipPlanes.rightDistance + g_CoasterClipPlanes.rightNormal.vz * (v->z >> 2));
    if (f.dl > 0 && g_CoasterClipPlanes.leftHelper > 0)
        f.fl = 1;
    if (f.dl < 0 && g_CoasterClipPlanes.leftHelper < 0)
        f.fl = 1;
    if (f.dr > 0 && g_CoasterClipPlanes.rightHelper > 0)
        f.fr = 1;
    if (f.dr < 0 && g_CoasterClipPlanes.rightHelper < 0)
        f.fr = 1;
    return f.fl & f.fr;
}
/*
 * Tests whether a 16-bit integer point vector lies inside both the left and right clip planes.
 */
// FUNCTION: FF7 0x005eedae
int Mini_CoasterPointInsideClipPlanesVec16(MFVec16 *v)
{
    struct {
        int dl, fr, fl, dr;
    } f;
    f.fl = 0;
    f.fr = f.fl;
    f.dl = (g_CoasterClipPlanes.leftNormal.vx * (v->x >> 2) + g_CoasterClipPlanes.leftNormal.vy * (v->y >> 2)) + (g_CoasterClipPlanes.leftDistance + g_CoasterClipPlanes.leftNormal.vz * (v->z >> 2));
    f.dr = (g_CoasterClipPlanes.rightNormal.vx * (v->x >> 2) + g_CoasterClipPlanes.rightNormal.vy * (v->y >> 2)) + (g_CoasterClipPlanes.rightDistance + g_CoasterClipPlanes.rightNormal.vz * (v->z >> 2));
    if (f.dl > 0 && g_CoasterClipPlanes.leftHelper > 0)
        f.fl = 1;
    if (f.dl < 0 && g_CoasterClipPlanes.leftHelper < 0)
        f.fl = 1;
    if (f.dr > 0 && g_CoasterClipPlanes.rightHelper > 0)
        f.fr = 1;
    if (f.dr < 0 && g_CoasterClipPlanes.rightHelper < 0)
        f.fr = 1;
    return f.fl & f.fr;
}
/*
 * Calculates signed distance from the given 3D coordinates to the left clip plane.
 */
// FUNCTION: FF7 0x005eeead
int __cdecl Mini_CoasterLeftClipPlaneDistance(int x, int y, int z)
{
    return ((g_CoasterClipPlanes.leftNormal.vx * (x >> 2) + g_CoasterClipPlanes.leftNormal.vy * (y >> 2)) + (g_CoasterClipPlanes.leftDistance + g_CoasterClipPlanes.leftNormal.vz * (z >> 2)));
}
/*
 * Calculates signed distance from the given 3D coordinates to the right clip plane.
 */
// FUNCTION: FF7 0x005eeeea
int __cdecl Mini_CoasterRightClipPlaneDistance(int x, int y, int z)
{
    return ((g_CoasterClipPlanes.rightNormal.vx * (x >> 2) + g_CoasterClipPlanes.rightNormal.vy * (y >> 2)) + (g_CoasterClipPlanes.rightDistance + g_CoasterClipPlanes.rightNormal.vz * (z >> 2)));
}
/*
 * Tests whether a sphere centered at point v with the given radius intersects or lies inside both clip planes.
 */
// FUNCTION: FF7 0x005eef27
int Mini_CoasterSphereInsideBothClipPlanes(MFVec32 *v, short radius)
{
    struct {
        int fl, dl, fr, dr, q;
    } f; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    f.fl = 0;
    f.fr = 0;
    f.dl = (g_CoasterClipPlanes.leftNormal.vx * (v->x >> 2) + g_CoasterClipPlanes.leftNormal.vy * (v->y >> 2)) + (g_CoasterClipPlanes.leftDistance + g_CoasterClipPlanes.leftNormal.vz * (v->z >> 2));
    if (g_CoasterClipPlanes.leftHelper > 0 && f.dl >= 0)
        f.fl = 1;
    if (g_CoasterClipPlanes.leftHelper < 0 && f.dl <= 0)
        f.fl = 1;
    if (f.fl == 0) {
        f.q = abs(f.dl) / g_CoasterClipPlanes.leftNormalLength;
        if (f.q < radius)
            f.fl = 1;
    }
    f.dr = (g_CoasterClipPlanes.rightNormal.vx * (v->x >> 2) + g_CoasterClipPlanes.rightNormal.vy * (v->y >> 2)) + (g_CoasterClipPlanes.rightDistance + g_CoasterClipPlanes.rightNormal.vz * (v->z >> 2));
    if (g_CoasterClipPlanes.rightHelper > 0 && f.dr >= 0)
        f.fr = 1;
    if (g_CoasterClipPlanes.rightHelper < 0 && f.dr <= 0)
        f.fr = 1;
    if (f.fr == 0) {
        f.q = abs(f.dr) / g_CoasterClipPlanes.rightNormalLength;
        if (f.q < radius)
            f.fr = 1;
    }
    return f.fl & f.fr;
}
/*
 * Tests whether a sphere centered at (x, y, z) with the given radius intersects or lies inside the left clip plane.
 */
// FUNCTION: FF7 0x005ef071
int Mini_CoasterSphereInsideLeftClipPlane(int x, int y, int z, short radius)
{
    int planeValue, dist, res;
    res = 0;
    planeValue = (g_CoasterClipPlanes.leftNormal.vx * (x >> 2) + g_CoasterClipPlanes.leftNormal.vy * (y >> 2)) + (g_CoasterClipPlanes.leftDistance + g_CoasterClipPlanes.leftNormal.vz * (z >> 2));
    if (g_CoasterClipPlanes.leftHelper > 0 && planeValue >= 0)
        res = 1;
    if (g_CoasterClipPlanes.leftHelper < 0 && planeValue <= 0)
        res = 1;
    if (res == 0) {
        dist = abs(planeValue) / g_CoasterClipPlanes.leftNormalLength;
        if (dist < radius)
            res = 1;
    }
    return res;
}
/*
 * Tests whether a sphere centered at (x, y, z) with the given radius intersects or lies inside the right clip plane.
 */
// FUNCTION: FF7 0x005ef114
int __cdecl Mini_CoasterSphereInsideClipPlane(int x, int y, int z, short radius)
{
    int signedDistance;
    int absDistance;
    int isInside;

    isInside = 0;
    signedDistance = (((g_CoasterClipPlanes.rightNormal.vx * (x >> 2) + g_CoasterClipPlanes.rightNormal.vy * (y >> 2)) + g_CoasterClipPlanes.rightNormal.vz * (z >> 2)) + g_CoasterClipPlanes.rightDistance);
    if (g_CoasterClipPlanes.rightHelper > 0 && signedDistance >= 0) {
        isInside = 1;
    }
    if (g_CoasterClipPlanes.rightHelper < 0 && signedDistance <= 0) {
        isInside = 1;
    }
    if (isInside == 0) {
        absDistance = abs(signedDistance) / g_CoasterClipPlanes.rightNormalLength;
        if (absDistance < radius) {
            isInside = 1;
        }
    }
    return isInside;
}
} /* extern "C" */
