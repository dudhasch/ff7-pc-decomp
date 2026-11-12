// src/ff7/src/battle/battle_gte_matrix_builders.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Builds GTE transform matrices for the battle camera: copies a matrix with its
//   translation, and constructs Euler-angle view matrices and look-at matrices.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/types/battle.h"
#include <stdlib.h>
#include "ff7/camera_script.h"
#include <string.h>
#include "lib/codegen.h"
#include "ff7/api/battle.h"
#include "lib/api/graphics.h"
#include "lib/types/graphics.h"
extern "C" {

struct BattleFxVector3;

#pragma intrinsic(abs)

#pragma intrinsic(memset)
#pragma intrinsic(memcpy)


// clang-format off
// GLOBAL: FF7 0x00c06620
int g_BattleGteZeroTranslation = 0;
unsigned char g_bssPad_00c06624[4] = {0};
unsigned char g_bssPad_00c06628[24] = {0};
// clang-format on

/*
 * Copies nine short matrix entries in column traversal order then three translation ints. Aliasing
 * can change behavior versus block copy.
 */
// FUNCTION: FF7 0x005c67a0
void __cdecl Gte_CopyMatrixWithTranslation(PsxMatrix *dst, PsxMatrix *src)
{
    dst->m[0][0] = src->m[0][0];
    dst->m[1][0] = src->m[1][0];
    dst->m[2][0] = src->m[2][0];
    dst->m[0][1] = src->m[0][1];
    dst->m[1][1] = src->m[1][1];
    dst->m[2][1] = src->m[2][1];
    dst->m[0][2] = src->m[0][2];
    dst->m[1][2] = src->m[1][2];
    dst->m[2][2] = src->m[2][2];
    dst->t[0] = src->t[0];
    dst->t[1] = src->t[1];
    dst->t[2] = src->t[2];
}

/*
 * Builds and installs the XYZ rotation matrix in matrix, installs g_BattleGteZeroTranslation as the
 * translation, transforms vector into matrix->translateX/Y/Z and negates the result.
 */
// FUNCTION: FF7 0x005c6845
void __cdecl Gte_BuildInverseEulerXyzViewMatrix(struct BattleGteViewMatrixScratch *matrix, void *vector, void *rotation)
{
    int depth;

    RotMatrixXYZ(rotation, (void *)matrix);
    SetRotMatrix((void *)matrix);
    SetTransMatrix(&g_BattleGteZeroTranslation);
    RotTransPoint(vector, &matrix->translateX, &depth);
    matrix->translateX = -matrix->translateX;
    matrix->translateY = -matrix->translateY;
    matrix->translateZ = -matrix->translateZ;
}

/*
 * Builds direction/cross-product basis through separately declared local vectors, writes three
 * matrix rows, transforms eye into translation and negates all three translation dwords. Output
 * components require native local-layout proof; source x perturbation compares against up third
 * component.
 */
// FUNCTION: FF7 0x005c68b7
void __cdecl Gte_BuildLookAtMatrix(struct PsxMatrix *matrix, struct SVECTOR *eye, struct SVECTOR *target, struct VECTOR *up)
{
    VECTOR xDelta;
    VECTOR rightX;
    VECTOR upX;
    VECTOR forwardVector;
    int result;

    xDelta.vx = target->vx - eye->vx;
    xDelta.vy = target->vy - eye->vy;
    xDelta.vz = target->vz - eye->vz;
    NormalizeVectorFixed(&xDelta, &forwardVector);
    if (forwardVector.vz == up->vz) {
        forwardVector.vz++;
    }
    CrossProductFixed12(&forwardVector, up, &xDelta);
    NormalizeVectorFixed(&xDelta, &rightX);
    CrossProductFixed12(&forwardVector, &rightX, &xDelta);
    NormalizeVectorFixed(&xDelta, &upX);
    matrix->m[0][0] = rightX.vx;
    matrix->m[0][1] = rightX.vy;
    matrix->m[0][2] = rightX.vz;
    matrix->m[1][0] = upX.vx;
    matrix->m[1][1] = upX.vy;
    matrix->m[1][2] = upX.vz;
    matrix->m[2][0] = forwardVector.vx;
    matrix->m[2][1] = forwardVector.vy;
    matrix->m[2][2] = forwardVector.vz;
    SetRotMatrix((void *)matrix);
    SetTransMatrix(&g_BattleGteZeroTranslation);
    RotTransPoint((void *)eye, matrix->t, &result);
    matrix->t[0] = -matrix->t[0];
    matrix->t[1] = -matrix->t[1];
    matrix->t[2] = -matrix->t[2];
}

/*
 * The same look-at matrix construction as Gte_BuildLookAtMatrix, but it leaves the three
 * translation ints positive instead of negating them. The 3x3 part is written through
 * PsxMatrix; the translation is the PsxMatrix translation that follows it.
 */
// FUNCTION: FF7 0x005c6a20
void __cdecl Gte_BuildLookAtMatrixPositiveTranslation(PsxMatrix *matrix, Vec3sPacked *eye, Vec3sPacked *target, BattleFxVector3 *up)
{
    VECTOR xChange;
    VECTOR rightX;
    VECTOR upX;
    VECTOR forwardVector;
    int result;

    xChange.vx = target->vx - eye->vx;
    xChange.vy = target->vy - eye->vy;
    xChange.vz = target->vz - eye->vz;
    NormalizeVectorFixed(&xChange, &forwardVector);
    if (forwardVector.vz == up->z) {
        forwardVector.vz++;
    }
    CrossProductFixed12(&forwardVector, (VECTOR *)up, &xChange);
    NormalizeVectorFixed(&xChange, &rightX);
    CrossProductFixed12(&forwardVector, &rightX, &xChange);
    NormalizeVectorFixed(&xChange, &upX);
    matrix->m[0][0] = rightX.vx;
    matrix->m[0][1] = rightX.vy;
    matrix->m[0][2] = rightX.vz;
    matrix->m[1][0] = upX.vx;
    matrix->m[1][1] = upX.vy;
    matrix->m[1][2] = upX.vz;
    matrix->m[2][0] = forwardVector.vx;
    matrix->m[2][1] = forwardVector.vy;
    matrix->m[2][2] = forwardVector.vz;
    SetRotMatrix((void *)matrix);
    SetTransMatrix(&g_BattleGteZeroTranslation);
    RotTransPoint((void *)eye, &matrix->t[0], &result);
}
} /* extern "C" */
