// src/ff7/src/battle/battle_camera_matrix_compose.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Builds battle render matrices: it composes matrices relative to the camera, installs
//   view-space and billboard transforms, and builds look-at basis matrices from a direction.
/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/types/graphics.h"
#include "ff7/types/battle.h"
#include "ff7/battle.h"
#include "ff7/gte.h"
#include "lib/crt.h"
#include "lib/api/graphics.h"
#include "lib/codegen.h"
#include "ff7/api/battle.h"
extern "C" {

// clang-format off
// GLOBAL: FF7 0x00c06a30
PsxMatrix g_BattleFxBillboardMatrix = {0};
// clang-format on

/*
 * Composes matrix matrix relative to camera matrix cameraMatrix into dstMatrix: the translation becomes matrix's
 * minus cameraMatrix's, rotated by cameraMatrix's transpose, and dstMatrix's rotation is cameraMatrix-transpose times matrix.
 */
// FUNCTION: FF7 0x005d2a30
void __cdecl Battle_ComposeMatrixRelativeToCamera(short *cameraMatrix, void *matrix, struct PsxMatrix *dstMatrix)
{
    dstMatrix->t[0] = *(int *)((char *)matrix + 0x12) - *(int *)((char *)cameraMatrix + 0x12);
    dstMatrix->t[1] = *(int *)((char *)matrix + 0x16) - *(int *)((char *)cameraMatrix + 0x16);
    dstMatrix->t[2] = *(int *)((char *)matrix + 0x1A) - *(int *)((char *)cameraMatrix + 0x1A);
    TransposeMatrix(cameraMatrix, (short *)dstMatrix);
    TransformVectorToInt32(dstMatrix, dstMatrix->t, dstMatrix->t);
    ConcatMatrixIntoFirst((short *)dstMatrix, matrix);
}

/*
 * Installs global view matrix and transforms point into output translation. Nonzero push length
 * normalizes into separate locals then adds scaled components. Returns output.
 */
// FUNCTION: FF7 0x005d2aa5
int __cdecl BattleFx_MakeViewSpacePointPushed(void *srcPoint, int pushLength, Vec3sPacked *outPoint)
{
    int depth;
    VECTOR normLength;

    SetRotMatrix(&g_BattleCameraTransform);
    SetTransMatrix(&g_BattleCameraTransform);
    RotTransPoint(srcPoint, (char *)outPoint + 0x12, &depth);
    if (pushLength != 0) {
        NormalizeVectorFixed((char *)outPoint + 0x12, &normLength);
        *(int *)((char *)outPoint + 0x12) += normLength.vx * pushLength >> 0xC;
        *(int *)((char *)outPoint + 0x16) += normLength.vy * pushLength >> 0xC;
        *(int *)((char *)outPoint + 0x1A) += normLength.vz * pushLength >> 0xC;
    }
    return (int)outPoint;
}

/*
 * Sets billboard diagonal scale shorts, transforms point into global translation and optionally
 * applies normalized depth push. Installs resulting matrix and returns global address.
 */
// FUNCTION: FF7 0x005d2b44
void *__cdecl BattleFx_InstallBillboardMatrix(void *srcPoint, short scale, int pushLength)
{
    int depth;
    VECTOR normLength;

    g_BattleFxBillboardMatrix.m[2][2] = scale;
    g_BattleFxBillboardMatrix.m[1][1] = g_BattleFxBillboardMatrix.m[2][2];
    g_BattleFxBillboardMatrix.m[0][0] = g_BattleFxBillboardMatrix.m[1][1];
    SetRotMatrix(&g_BattleCameraTransform);
    SetTransMatrix(&g_BattleCameraTransform);
    RotTransPoint(srcPoint, g_BattleFxBillboardMatrix.t, &depth);
    if (pushLength != 0) {
        NormalizeVectorFixed(g_BattleFxBillboardMatrix.t, &normLength);
        g_BattleFxBillboardMatrix.t[0] += normLength.vx * pushLength >> 0xC;
        g_BattleFxBillboardMatrix.t[1] += normLength.vy * pushLength >> 0xC;
        g_BattleFxBillboardMatrix.t[2] += normLength.vz * pushLength >> 0xC;
    }
    SetRotMatrix(&g_BattleFxBillboardMatrix);
    SetTransMatrix(&g_BattleFxBillboardMatrix);
    return (void *)&g_BattleFxBillboardMatrix;
}

/*
 * Builds a look-at rotation matrix (3 x 3 shorts, column-major in basisOut) from a direction: dir is
 * the normalised input, right is (dir.z, 0, -dir.x) normalised, up is dir x right normalised.
 */
// FUNCTION: FF7 0x005d2c1f
void __cdecl Gte_BuildLookBasisFromDirection(short *direction, short *basisOut)
{
    /* Three GTE vectors, 0x30 bytes; declaration order puts right at
       [ebp-0x10], up at [ebp-0x20] and dir at [ebp-0x30]. The up vector's
       initializer is what 0x005D2C33 shows: `mov [..], 0` for vz and then
       `xor eax, eax` for the pad, the implicit zero. */
    VECTOR right;
    VECTOR up = {0, -0x1000, 0};
    VECTOR dir;

    dir.vx = direction[0];
    dir.vy = direction[1];
    dir.vz = direction[2];
    NormalizeVectorFixed(&dir, &dir);
    right.vx = dir.vz;
    right.vy = 0;
    right.vz = -dir.vx;
    NormalizeVectorFixed(&right, &right);
    CrossProductFixed12((struct VECTOR *)&dir, (struct VECTOR *)&right, (struct VECTOR *)&up);
    NormalizeVectorFixed(&up, &up);
    basisOut[0] = right.vx;
    basisOut[3] = right.vy;
    basisOut[6] = right.vz;
    basisOut[1] = up.vx;
    basisOut[4] = up.vy;
    basisOut[7] = up.vz;
    basisOut[2] = dir.vx;
    basisOut[5] = dir.vy;
    basisOut[8] = dir.vz;
}

/*
 * Loads second matrix column into locals and normalizes, builds first vector,
 * crosses/normalizes into third column.
 */
// FUNCTION: FF7 0x005d2d1b
void __cdecl Gte_OrthonormaliseFromYAxis(BattleOrthoMatrix *matrix)
{
    VECTOR crossOut;
    VECTOR firstAxis;
    VECTOR xVector;

    xVector.vx = matrix->xVector;
    xVector.vy = matrix->axisY;
    xVector.vz = matrix->axisZ;
    NormalizeVectorFixed(&xVector, &xVector);
    matrix->xVector = xVector.vx;
    matrix->axisY = xVector.vy;
    matrix->axisZ = xVector.vz;
    firstAxis.vx = xVector.vy;
    firstAxis.vy = -xVector.vx;
    firstAxis.vz = 0;
    NormalizeVectorFixed(&firstAxis, &firstAxis);
    matrix->firstAxis = firstAxis.vx;
    matrix->axisNeg = firstAxis.vy;
    matrix->firstAxisZ = firstAxis.vz;
    CrossProductFixed12(&firstAxis, &xVector, &crossOut);
    NormalizeVectorFixed(&crossOut, &crossOut);
    matrix->crossOut = crossOut.vx;
    matrix->crossY = crossOut.vy;
    matrix->crossZ = crossOut.vz;
}
} /* extern "C" */
