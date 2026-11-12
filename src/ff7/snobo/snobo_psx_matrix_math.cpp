// src/ff7/snobo/snobo_psx_matrix_math.cpp
// Fixed-point 3x3 matrix arithmetic for the snowboard's PSX-style transforms: matrix
// multiplication, applying a matrix to short and int vectors, and the Matrix30 helpers that set
// identity, rotations from angles and translations in short-format matrix records.
#include "ff7/types/snobo.h"
#include "lib/types/graphics.h"
#include <stdarg.h>
#include <math.h>
#include "ff7/api/snobo.h"

#pragma intrinsic(sin, cos)
/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
extern "C" {
SVECTOR * __cdecl PsxApplyMatrix33ToShortVector(void *matrix, void *inVector, void *dst);
SnowboardVec * __cdecl PsxApplyMatrix33ToIntVector(SnoboMatrix *matrix, SnowboardSpawnPos *inVector, SnowboardVec *dst);
inline double RotationSinFloat(float angle) { return (double)(float)sin(angle); }
inline double RotationCosFloat(float angle) { return (double)(float)cos(angle); }

/*
 * Multiply borrowed three-by-three signed-WORD matrices in fixed point.
 */
// FUNCTION: FF7 0x0065ed50
PsxMatrix * __cdecl PsxMultiplyMatrix33(PsxMatrix *matrixA, PsxMatrix *matrixB, PsxMatrix *result)
{
    int i;
    int mul;
    int j;
    int k;

    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            mul = 0;
            for (k = 0; k < 3; k++) {
                mul += matrixA->m[i][k] * matrixB->m[k][j];
            }
            result->m[i][j] = (short)(mul / 4096);
        }
    }
    return result;
}

/*
 * Multiplies two three-by-three signed-short matrices using int dot sums divided by 4096 and
 * narrowed to short. Copies the entire 30-byte local record to dst, including six uninitialized
 * trailing shorts, and returns dst.
 */
// FUNCTION: FF7 0x0065ee0c
PsxMatrix * __cdecl PsxMultiplyMatrix33ViaLocal(PsxMatrix *matrixA, PsxMatrix *matrixB, PsxMatrix *dst)
{
    PsxMatrix res;
    int i;
    int mul;
    int j;
    int k;

    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            mul = 0;
            for (k = 0; k < 3; k++) {
                mul += matrixA->m[i][k] * matrixB->m[k][j];
            }
            res.m[i][j] = (short)(mul / 4096);
        }
    }
    *dst = res;
    return dst;
}

/*
 * Computes each output[i][j] from the dot product of input-one column i and input-two row j,
 * divides the int sum by 4096 and narrows to short. Writes directly to result and returns it.
 */
// FUNCTION: FF7 0x0065eeda
PsxMatrix * __cdecl PsxMultiplyMatrix33Transposed(PsxMatrix *matrixA, PsxMatrix *matrixB, PsxMatrix *result)
{
    int i;
    int mul;
    int j;
    int k;

    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            mul = 0;
            for (k = 0; k < 3; k++) {
                mul += matrixA->m[k][i] * matrixB->m[j][k];
            }
            result->m[i][j] = (short)(mul / 4096);
        }
    }
    return result;
}

/*
 * Applies the 3x3 short rotation matrix at matrix to the three shorts at inVector, each row's dot
 * product divided by 4096 back to 12-bit fixed point, builds the answer in a four-int scratch
 * vector and copies all sixteen bytes (the fourth int is never written) to dst, returning dst.
 */
// FUNCTION: FF7 0x0065ef96
int __cdecl PsxApplyMatrix33ToLongVector(SnoboMatrix *matrix, SnoboVector *inVector, void *dst)
{
    struct LongVector {
        int vx;
        int vy;
        int vz;
        int pad;
    } vec;

    vec.vx = (matrix->m00 * inVector->x + matrix->m01 * inVector->y + matrix->m02 * inVector->z) / 4096;
    vec.vy = (matrix->m10 * inVector->x + matrix->m11 * inVector->y + matrix->m12 * inVector->z) / 4096;
    vec.vz = (matrix->m20 * inVector->x + matrix->m21 * inVector->y + matrix->m22 * inVector->z) / 4096;
    *(struct LongVector *)dst = vec;
    return (int)dst;
}

/*
 * The short-output form of the same rotation: applies the 3x3 matrix at matrix to the three shorts
 * at inVector, divides each row's dot product by 4096, saturates it to the signed 16-bit range, and
 * copies the resulting four-short vector (the fourth short is never written) to dst, returning
 * dst.
 */
// FUNCTION: FF7 0x0065f08b
SVECTOR * __cdecl PsxApplyMatrix33ToShortVector(void *matrix, void *inVector, void *dst)
{
    SVECTOR vec;
    int rotX;
    int rotY;
    int rotZ;

    rotX = (((SnoboMatrix *)matrix)->m00 * ((SnoboVector *)inVector)->x + ((SnoboMatrix *)matrix)->m01 * ((SnoboVector *)inVector)->y + ((SnoboMatrix *)matrix)->m02 * ((SnoboVector *)inVector)->z) / 4096;
    if (rotX > 32767) {
        vec.vx = 32767;
    } else if (rotX < -32768) {
        vec.vx = -32768;
    } else {
        vec.vx = (short)rotX;
    }
    rotY = (((SnoboMatrix *)matrix)->m10 * ((SnoboVector *)inVector)->x + ((SnoboMatrix *)matrix)->m11 * ((SnoboVector *)inVector)->y + ((SnoboMatrix *)matrix)->m12 * ((SnoboVector *)inVector)->z) / 4096;
    if (rotY > 32767) {
        vec.vy = 32767;
    } else if (rotY < -32768) {
        vec.vy = -32768;
    } else {
        vec.vy = (short)rotY;
    }
    rotZ = (((SnoboMatrix *)matrix)->m20 * ((SnoboVector *)inVector)->x + ((SnoboMatrix *)matrix)->m21 * ((SnoboVector *)inVector)->y + ((SnoboMatrix *)matrix)->m22 * ((SnoboVector *)inVector)->z) / 4096;
    if (rotZ > 32767) {
        vec.vz = 32767;
    } else if (rotZ < -32768) {
        vec.vz = -32768;
    } else {
        vec.vz = (short)rotZ;
    }
    *((SVECTOR *)dst) = vec;
    return (SVECTOR *)dst;
}

/*
 * Shifts the 64-bit pair (highWord:lowWord) right by shift bits, extracting the low 32-bit word
 * for shifts 1..32 and arithmetic right-shifting highWord beyond 32.
 */
// FUNCTION: FF7 0x0065f1f2
int __cdecl PsxShiftWordPair(int lowWord, int highWord, unsigned int shift)
{
    int loPart, outWord, reverseShift, wordB;

    if (shift == 0) {
        return lowWord;
    }
    if (shift > 32) {
        shift -= 32;
        return highWord >> shift;
    } else {
        loPart = lowWord;
        wordB = highWord;
        reverseShift = 32 - shift;
        /* Retail's inline x86 double-precision shift sequence. */
        __asm {
            mov eax, loPart
            mov edx, wordB
            mov ecx, reverseShift
            shrd eax, eax, cl
            mov ecx, shift
            shld edx, edx, cl
            or eax, edx
            mov outWord, eax
        }
        return outWord;
    }
}

/*
 * Converts the three input ints to float, computes three dot products against signed-short matrix
 * rows, divides each by 4096.0 and converts to int in output. Returns output.
 */
// FUNCTION: FF7 0x0065f257
SnowboardVec * __cdecl PsxApplyMatrix33ToIntVector(SnoboMatrix *matrix, SnowboardSpawnPos *inVector, SnowboardVec *dst)
{
    float fvec[3];

    fvec[0] = (float)inVector->vecX;
    fvec[1] = (float)inVector->vecY;
    fvec[2] = (float)inVector->vecZ;
    dst->x = (int)((matrix->m00 * fvec[0] + matrix->m01 * fvec[1] + matrix->m02 * fvec[2]) / 4096.0);
    dst->y = (int)((matrix->m10 * fvec[0] + matrix->m11 * fvec[1] + matrix->m12 * fvec[2]) / 4096.0);
    dst->z = (int)((matrix->m20 * fvec[0] + matrix->m21 * fvec[1] + matrix->m22 * fvec[2]) / 4096.0);
    return dst;
}

/*
 * Writes the nine rotation shorts m00 through m22 from r0c0 through r2c2 and returns matrix. Three of those
 * arguments (r1c0, r1c2, r2c0) arrive as int and are truncated on the store. The shorts are the rotation half of
 * a PsxMatrix record, whose translation is the three ints after them.
 */
// FUNCTION: FF7 0x0065f34b
int __cdecl PsxSetMatrix33Elements(SnoboMatrix *matrix, short r0c0, short r0c1, short r0c2, int r1c0, short r1c1, int r1c2, int r2c0, short r2c1, short r2c2)
{
    matrix->m00 = r0c0;
    matrix->m01 = r0c1;
    matrix->m02 = r0c2;
    matrix->m10 = r1c0;
    matrix->m11 = r1c1;
    matrix->m12 = r1c2;
    matrix->m20 = r2c0;
    matrix->m21 = r2c1;
    matrix->m22 = r2c2;
    return (int)matrix;
}

/*
 * Copies the nine rotation shorts from src to dst and returns dst; the translation ints of dst are untouched.
 */
// FUNCTION: FF7 0x0065f3b5
int __cdecl Matrix30CopyRotation(PsxMatrix *dst, SnoboMatrix *src)
{
    dst->m[0][0] = src->m00;
    dst->m[0][1] = src->m01;
    dst->m[0][2] = src->m02;
    dst->m[1][0] = src->m10;
    dst->m[1][1] = src->m11;
    dst->m[1][2] = src->m12;
    dst->m[2][0] = src->m20;
    dst->m[2][1] = src->m21;
    dst->m[2][2] = src->m22;
    return (int)dst;
}

/*
 * Stores transX, transY and transZ as the translation of matrix and returns matrix. The translation follows the
 * nine rotation shorts, so it is not four-byte aligned.
 */
// FUNCTION: FF7 0x0065f439
int __cdecl Matrix30SetTranslation(PsxMatrix *matrix, int transX, int transY, int transZ)
{
    matrix->t[0] = transX;
    matrix->t[1] = transY;
    matrix->t[2] = transZ;
    return (int)matrix;
}

/*
 * Copies the three ints of translation into the translation of dst and returns dst. The rotation shorts of dst
 * are untouched.
 */
// FUNCTION: FF7 0x0065f45c
int __cdecl Matrix30CopyTranslation(PsxMatrix *dst, SnowboardEffectPos *translation)
{
    dst->t[0] = translation->posX;
    dst->t[1] = translation->posY;
    dst->t[2] = translation->posZ;
    return (int)dst;
}

/*
 * Writes the nine supplied shorts through PsxSetMatrix33Elements, writes three supplied translation ints
 * through Matrix30SetTranslation and returns the destination record.
 */
// FUNCTION: FF7 0x0065f487
int __cdecl Matrix30SetRotationAndTranslation(int mat, short r00, short r01, short r02, short r10, short r11, short r12, short r20, short r21, short r22, int tx, int ty, int tz)
{
    /* Retail caller prototype (int, short, short, short, short, short, short, short, short, short) -> int preserved for /Od argument promotion on `PsxSetMatrix33Elements`. */
    ((int(__cdecl *)(int, short, short, short, short, short, short, short, short, short))PsxSetMatrix33Elements)(mat, r00, r01, r02, r10, r11, r12, r20, r21, r22);
    Matrix30SetTranslation((PsxMatrix *)mat, tx, ty, tz);
    return mat;
}

/*
 * Computes signed-short sine and cosine of angle*pi/2048 scaled by 4096, then returns PsxSetMatrix33Elements
 * with matrix and coefficients (4096, 0, 0;0, cos, -sin;0, sin, cos).
 */
// FUNCTION: FF7 0x0065f4e0
int __cdecl Matrix30SetRotationX(short angle, struct SnoboMatrix *matrix)
{
    short cosVal, sinVal;
    sinVal = (short)(sin(angle * 3.1415927410125732f / 2048.0) * 4096.0);
    cosVal = (short)(cos(angle * 3.1415927410125732f / 2048.0) * 4096.0);
    return PsxSetMatrix33Elements(matrix, 4096, 0, 0, 0, cosVal, -sinVal, 0, sinVal, cosVal);
}

/*
 * Computes signed-short sine and cosine of angle*pi/2048 scaled by 4096, then returns PsxSetMatrix33Elements
 * with matrix and coefficients (cos, 0, sin;0, 4096, 0;-sin, 0, cos).
 */
// FUNCTION: FF7 0x0065f56d
int __cdecl Matrix30SetRotationY(short angle, void *matrix)
{
    short cosVal, sinVal;
    sinVal = (short)(sin(angle * 3.1415927410125732f / 2048.0) * 4096.0);
    cosVal = (short)(cos(angle * 3.1415927410125732f / 2048.0) * 4096.0);
    return PsxSetMatrix33Elements((struct SnoboMatrix *)matrix, cosVal, 0, sinVal, 0, 4096, 0, -sinVal, 0, cosVal);
}

/*
 * Computes signed-short sine and cosine of angle*pi/2048 scaled by 4096, then returns PsxSetMatrix33Elements
 * with matrix and coefficients (cos, sin, 0;-sin, cos, 0;0, 0, 4096).
 */
// FUNCTION: FF7 0x0065f5fa
int __cdecl Matrix30SetRotationZ(short angle, struct SnoboMatrix *matrix)
{
    short cosVal, sinVal;
    sinVal = (short)(sin(angle * 3.1415927410125732f / 2048.0) * 4096.0);
    cosVal = (short)(cos(angle * 3.1415927410125732f / 2048.0) * 4096.0);
    return PsxSetMatrix33Elements(matrix, cosVal, sinVal, 0, -sinVal, cosVal, 0, 0, 0, 4096);
}

/*
 * Builds three fixed-point axis matrices from the input shorts, negating the third angle,
 * multiplies the first by the second and that result by the third into output. Returns output;
 * only the rotation entries are written by the multiplication helper.
 */
// FUNCTION: FF7 0x0065f687
int __cdecl Matrix30SetRotationFromAngles(SnowboardAngles *input, void *output)
{
    PsxMatrix xMatrix, rotXY, rotY, rotZ;
    Matrix30SetRotationX(input->angle, (struct SnoboMatrix *)&xMatrix);
    Matrix30SetRotationY(input->angleY, &rotY);
    Matrix30SetRotationZ(-input->angleZ, (struct SnoboMatrix *)&rotZ);
    PsxMultiplyMatrix33(&xMatrix, &rotY, &rotXY);
    PsxMultiplyMatrix33(&rotXY, &rotZ, (PsxMatrix *)output);
    return (int)output;
}

/*
 * Builds three fixed-point axis matrices from input shorts, multiplies the second by the first and
 * that result by the third into output, then returns output. Only rotation entries are written by
 * the multiplication helper.
 */
// FUNCTION: FF7 0x0065f6fc
int __cdecl Matrix30SetRotationFromAnglesYXZ(SnowboardAngles *input, void *output)
{
    PsxMatrix xMatrix, partial, rotY, rotZ;
    Matrix30SetRotationX(input->angle, (struct SnoboMatrix *)&xMatrix);
    Matrix30SetRotationY(input->angleY, &rotY);
    Matrix30SetRotationZ(input->angleZ, (struct SnoboMatrix *)&rotZ);
    PsxMultiplyMatrix33(&rotY, &xMatrix, &partial);
    PsxMultiplyMatrix33(&partial, &rotZ, (PsxMatrix *)output);
    return (int)output;
}

/*
 * Fills the matrix record with the identity: the diagonal m00, m11 and m22 get 0x1000, which is 1.0 in the 4.12
 * fixed-point convention this record uses, the other six rotation shorts get 0, and the three translation ints
 * get 0. Returns matrix.
 */
// FUNCTION: FF7 0x0065f76f
int __cdecl Matrix30SetIdentity(SnoboMatrix *matrix)
{
    matrix->m00 = 0x1000;
    matrix->m01 = 0;
    matrix->m02 = 0;
    matrix->m10 = 0;
    matrix->m11 = 0x1000;
    matrix->m12 = 0;
    matrix->m20 = 0;
    matrix->m21 = 0;
    matrix->m22 = 0x1000;
    ((PsxMatrix *)matrix)->t[0] = 0;
    ((PsxMatrix *)matrix)->t[1] = 0;
    ((PsxMatrix *)matrix)->t[2] = 0;
    return (int)matrix;
}

/*
 * Multiplies the two rotation matrices into a local record, reads only the low signed shorts of input two's
 * translation and rotates that triple by input one. Copies the local rotation and writes the rotated triple as
 * translation through Matrix30CopyTranslation into output; input one's translation is not added. Returns output;
 * the full-width translation of input two is not read.
 */
// FUNCTION: FF7 0x0065f7e5
int __cdecl Matrix30ConcatenateTransform(void *matrixA, SnowboardMatrix302 *matrixB, void *dst)
{
    VECTOR rotatedTrans;
    PsxMatrix concat;
    SVECTOR shortVec;
    PsxMultiplyMatrix33((PsxMatrix *)matrixA, (PsxMatrix *)matrixB, &concat);
    shortVec.vx = matrixB->translationX;
    shortVec.vy = matrixB->translationY;
    shortVec.vz = matrixB->translationZ;
    PsxApplyMatrix33ToLongVector((struct SnoboMatrix *)matrixA, (struct SnoboVector *)&shortVec, &rotatedTrans);
    Matrix30CopyRotation((PsxMatrix *)dst, (struct SnoboMatrix *)&concat);
    Matrix30CopyTranslation((PsxMatrix *)dst, (struct SnowboardEffectPos *)&rotatedTrans);
    return (int)dst;
}

/*
 * Builds a 3x3 float rotation from angles and writes its nine floats to the three rows of
 * dst.
 */
// FUNCTION: FF7 0x0065f85b
void __cdecl SetRotationMatrix4x4FromAngles_1(SnowboardRotationAngles *angles, SnowboardFloatRowB *dst)
{
    float cosAngleX, sinX, sin1, cosAngleY, tempCos, sin2;
    sinX = RotationSinFloat(angles->angleX);
    cosAngleX = RotationCosFloat(angles->angleX);
    sin1 = RotationSinFloat(angles->angleY);
    cosAngleY = RotationCosFloat(angles->angleY);
    sin2 = RotationSinFloat(angles->angle);
    tempCos = RotationCosFloat(angles->angle);
    dst->col0 = cosAngleY * tempCos + sinX * sin1 * sin2;
    dst->col1 = -cosAngleY * sin2 + tempCos * sinX * sin1;
    dst->col2 = cosAngleX * sin1;
    dst[1].col0 = cosAngleX * sin2;
    dst[1].col1 = cosAngleX * tempCos;
    dst[1].col2 = -sinX;
    dst[2].col0 = -tempCos * sin1 + cosAngleY * sinX * sin2;
    dst[2].col1 = sin1 * sin2 + cosAngleY * tempCos * sinX;
    dst[2].col2 = cosAngleX * cosAngleY;
}

/*
 * Reads three float angles and evaluates their sine/cosine helpers, then writes nine rotation floats to the
 * first three columns of the three rows of dst using the composition formula below. The other entries of the
 * rows are untouched.
 */
// FUNCTION: FF7 0x0065f987
void __cdecl SetRotationMatrix4x4FromAngles_2(SnowboardRotationAngles *angles, SnowboardFloatRowB *dst)
{
    float cosAngleX, sinX, sin1, cosAngleY, tempCos, sin2;
    sinX = RotationSinFloat(angles->angleX);
    cosAngleX = RotationCosFloat(angles->angleX);
    sin1 = RotationSinFloat(angles->angleY);
    cosAngleY = RotationCosFloat(angles->angleY);
    sin2 = RotationSinFloat(angles->angle);
    tempCos = RotationCosFloat(angles->angle);
    dst->col0 = cosAngleY * tempCos;
    dst->col1 = -cosAngleY * sin2;
    dst->col2 = sin1;
    dst[1].col0 = cosAngleX * sin2 + tempCos * sinX * sin1;
    dst[1].col1 = cosAngleX * tempCos - sinX * sin1 * sin2;
    dst[1].col2 = -cosAngleY * sinX;
    dst[2].col0 = sinX * sin2 - cosAngleX * tempCos * sin1;
    dst[2].col1 = tempCos * sinX + cosAngleX * sin1 * sin2;
    dst[2].col2 = cosAngleX * cosAngleY;
}
} /* extern "C" */
