// src/ff7/submarine/sub_view_matrix_build.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Builds the submarine view matrix and the fixed-point rotation matrix from the signed
//   angle values.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/api/submarine.h"
#include "lib/api/graphics.h"
#include "lib/types/graphics.h"
#include "ff7/types/submarine.h"
#include <math.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <mmsystem.h>
#include "ff7/gte.h"

extern "C" {

#pragma intrinsic(abs)

#pragma intrinsic(strcpy, strcat)

#pragma data_seg(".data$u77_lut")
#pragma bss_seg(".data$u77_lut")
#pragma data_seg()
#pragma bss_seg()

#pragma intrinsic(sqrt)

// clang-format on

// Builds a view matrix into the PSX-style MATRIX at g_SubmarineViewMatrix. It borrows scratch space
// from Gfx_GetPsxScratchpadPtr(0), writes the negated camera position there as three shorts (x from
// g_SubmarineState.rotationStatePointer plus g_SubmarineState.axisTermX, less g_SubmarineState.camRotX as
// well when g_SubmarineState.currentFlag is zero; y from g_SubmarineState.camRotY,
// g_SubmarineState.currentAngleA and g_SubmarineState.axisTermY; z from g_SubmarineState.camRotZ,
// g_SubmarineState.currentAngleB and g_SubmarineState.axisTermZ), and passes it to
// Submarine_BuildRotationMatrixFromAngles to fill the rotation part. It then brings
// g_SubmarineState.player.posX, g_SubmarineState.player.posY and g_SubmarineState.player.posZ down
// from 4.12 fixed point with a 12-bit right shift into three adjacent stack slots, calls
// SetRotMatrix, rotates that vector with TransformVectorToInt32 into the scratch block at +8,
// negates the three results into the matrix's translation longs at +0x12, +0x16 and +0x1A (adding
// g_SubmarineState.camOffset to the third), and installs the result with SetTransMatrix.
// FUNCTION: FF7 0x00798160
void __cdecl Submarine_RebuildViewTransform(void)
{
    SubmarineViewBlock *scratch;
    VECTOR viewPos;

    scratch = (SubmarineViewBlock *)Gfx_GetPsxScratchpadPtr(0);
    scratch->rotationX = -g_SubmarineState.rotationStatePointer - g_SubmarineState.axisTermX;
    if (g_SubmarineState.currentFlag == 0) {
        scratch->rotationX -= g_SubmarineState.camRotX;
    }
    scratch->rotationY = -g_SubmarineState.camRotY - g_SubmarineState.currentAngleA - g_SubmarineState.axisTermY;
    scratch->rotationZ = -g_SubmarineState.camRotZ - g_SubmarineState.currentAngleB - g_SubmarineState.axisTermZ;
    Submarine_BuildRotationMatrixFromAngles(scratch, &g_SubmarineViewMatrix);
    viewPos.vx = g_SubmarineState.player.posX >> 12;
    viewPos.vy = g_SubmarineState.player.posY >> 12;
    viewPos.vz = g_SubmarineState.player.posZ >> 12;
    SetRotMatrix(&g_SubmarineViewMatrix);
    TransformVectorToInt32(&g_SubmarineViewMatrix, &viewPos, (char *)scratch + 8);
    g_SubmarineViewMatrix.t[0] = -scratch->translationX;
    g_SubmarineViewMatrix.t[1] = -scratch->translationY;
    g_SubmarineViewMatrix.t[2] = -scratch->translationZ + g_SubmarineState.camOffset;
    SetTransMatrix(&g_SubmarineViewMatrix);
}

// Builds a fixed-point 3 by 3 rotation matrix from three signed-short angles masked to 12 bits.
// First installs the X rotation, transforms three Y-rotation basis vectors through it, then
// installs the Z rotation and transforms those intermediate vectors. Stores the resulting vectors
// as matrix columns at matrix offsets 0 through 0x10, narrowing components to short. Native
// contiguous scalar locals carry the three-component helper inputs and outputs; translation fields
// beyond the nine coefficients are untouched.
// FUNCTION: FF7 0x00798284
void __cdecl Submarine_BuildRotationMatrixFromAngles(SubmarineViewBlock *viewBlock, PsxMatrix *matrix)
{
    VECTOR norm;
    VECTOR vec1X;
    VECTOR vec2X;
    VECTOR vector0X;
    int sinFirst;
    int cosX;
    int cosY;
    int sinSecond;
    int cosZ;
    int sinC;

    cosX = CosFixed(viewBlock->rotationX + 0x1000 & 0xFFF);
    sinFirst = SinFixed(viewBlock->rotationX + 0x1000 & 0xFFF);
    matrix->m[0][0] = 0x1000;
    matrix->m[0][1] = 0;
    matrix->m[0][2] = 0;
    matrix->m[1][0] = 0;
    matrix->m[1][1] = cosX;
    matrix->m[1][2] = -sinFirst;
    matrix->m[2][0] = 0;
    matrix->m[2][1] = sinFirst;
    matrix->m[2][2] = cosX;
    cosY = CosFixed(viewBlock->rotationY + 0x1000 & 0xFFF);
    sinSecond = SinFixed(viewBlock->rotationY + 0x1000 & 0xFFF);
    vector0X.vx = (short)cosY;
    vector0X.vy = 0;
    vector0X.vz = (short)-sinSecond;
    vec1X.vx = 0;
    vec1X.vy = 4096;
    vec1X.vz = 0;
    vec2X.vx = (short)sinSecond;
    vec2X.vy = 0;
    vec2X.vz = (short)cosY;
    TransformVectorToInt32(matrix, &vector0X, &norm);
    vector0X.vx = norm.vx;
    vector0X.vy = norm.vy;
    vector0X.vz = norm.vz;
    TransformVectorToInt32(matrix, &vec1X, &norm);
    vec1X.vx = norm.vx;
    vec1X.vy = norm.vy;
    vec1X.vz = norm.vz;
    TransformVectorToInt32(matrix, &vec2X, &norm);
    vec2X.vx = norm.vx;
    vec2X.vy = norm.vy;
    vec2X.vz = norm.vz;
    cosZ = CosFixed(viewBlock->rotationZ + 0x1000 & 0xFFF);
    sinC = SinFixed(viewBlock->rotationZ + 0x1000 & 0xFFF);
    matrix->m[0][0] = cosZ;
    matrix->m[0][1] = -sinC;
    matrix->m[0][2] = 0;
    matrix->m[1][0] = sinC;
    matrix->m[1][1] = cosZ;
    matrix->m[1][2] = 0;
    matrix->m[2][0] = 0;
    matrix->m[2][1] = 0;
    matrix->m[2][2] = 0x1000;
    TransformVectorToInt32(matrix, &vector0X, &norm);
    vector0X.vx = norm.vx;
    vector0X.vy = norm.vy;
    vector0X.vz = norm.vz;
    TransformVectorToInt32(matrix, &vec1X, &norm);
    vec1X.vx = norm.vx;
    vec1X.vy = norm.vy;
    vec1X.vz = norm.vz;
    TransformVectorToInt32(matrix, &vec2X, &norm);
    vec2X.vx = norm.vx;
    vec2X.vy = norm.vy;
    vec2X.vz = norm.vz;
    matrix->m[0][0] = vector0X.vx;
    matrix->m[1][0] = vector0X.vy;
    matrix->m[2][0] = vector0X.vz;
    matrix->m[0][1] = vec1X.vx;
    matrix->m[1][1] = vec1X.vy;
    matrix->m[2][1] = vec1X.vz;
    matrix->m[0][2] = vec2X.vx;
    matrix->m[1][2] = vec2X.vy;
    matrix->m[2][2] = vec2X.vz;
}
} /* extern "C" */
