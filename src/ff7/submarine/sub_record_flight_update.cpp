// src/ff7/submarine/sub_record_flight_update.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Updates the flight records (aim angles, orientation, position history and rotation
//   basis), resets and launches torpedo records, and draws record quads.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/types/graphics.h"
#include "ff7/api/submarine.h"
#include "lib/api/graphics.h"
#include "lib/api/sound.h"
#include "ff7/types/submarine.h"
#include "lib/enums/graphics.h"
#include "ff7/enums/submarine.h"
#include "lib/enums/sound.h"
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

// GLOBAL: FF7 0x00ec3138
unsigned char g_SubmarineFlightVertexData[18792] = {0};
// GLOBAL: FF7 0x00ec7aa0
int g_SubmarineActiveTorpedoCount = 0;
unsigned char g_bssPad_00ec7aa4[4] = {0};
// GLOBAL: FF7 0x00ec7aa8
struct SubmarineGridCell g_SubmarineGridCellTable[4096] = {0};
unsigned char g_bssPad_00ed7aa8[11200] = {0};
// GLOBAL: FF7 0x00eda668
struct GfxRenderObject *g_SubmarineFixedBatchList = 0;
unsigned char g_bssPad_00eda66c[4] = {0};
// GLOBAL: FF7 0x00eda670
struct SubmarineTriangleAnchor g_SubmarineTriangleColorBuffer = {0};
// GLOBAL: FF7 0x00eda678
short g_SubmarineRenderBatchState = 0;
// GLOBAL: FF7 0x00eda67c
short g_SubmarineTriangleBatchCount = 0;
// GLOBAL: FF7 0x00eda680
struct GfxRenderObject *g_SubmarineGridBatchListA = 0;
unsigned char g_bssPad_00eda684[4] = {0};
// GLOBAL: FF7 0x00eda688
unsigned char g_SubmarineGridVertexBuffer[65536] = {0};
// GLOBAL: FF7 0x00eea688
unsigned char g_SubmarineCameraBatchBuffer[12288] = {0};
// clang-format on

// The two Gfx_Set*Call prototypes take the engine context as a pointer, unlike the snobo header's int.

#pragma data_seg(".data$u77_lut")
#pragma bss_seg(".data$u77_lut")
#pragma data_seg()
#pragma bss_seg()

#pragma intrinsic(sqrt)

/*
 * For 32 submarine records, forms the target-minus-position vector shifted
 * right 12 in a shared data block and stores the Submarine_NormalizeVectorFixed result as the
 * record's elapsed distance. Builds a perpendicular vector using fixed-point cross products
 * against three fallback reference vectors, trying another when normalization
 * returns zero.
 */
// FUNCTION: FF7 0x0079c030
void __cdecl Submarine_UpdateRecordAimAngles(void)
{
    SubmarineAimRecord *aimPtr;
    int zSquared;
    int totalSq;
    SubmarineFlightRecord *flightPtr;
    struct VECTOR outRefVecX;
    int shiftBits;
    int leadBits;
    struct VECTOR resX;
    struct VECTOR tempDirX;

    aimPtr = (SubmarineAimRecord *)Gfx_GetPsxScratchpadPtr(0);
    flightPtr = &g_SubmarineFlightRecords[0];
    do {
        aimPtr->deltaX = flightPtr->targetX - flightPtr->posX >> 0xC;
        aimPtr->deltaY = flightPtr->targetY - flightPtr->posY >> 0xC;
        aimPtr->deltaZ = flightPtr->targetZ - flightPtr->posZ >> 0xC;
        flightPtr->elapsed = Submarine_NormalizeVectorFixed(aimPtr);
        tempDirX.vx = aimPtr->deltaX;
        tempDirX.vy = aimPtr->deltaY;
        tempDirX.vz = aimPtr->deltaZ;
        outRefVecX.vx = g_SubmarineUpVectorX;
        outRefVecX.vy = g_SubmarineUpVectorY;
        outRefVecX.vz = g_SubmarineUpVectorZ;
        CrossProductFixed12((VECTOR *)(&tempDirX), (VECTOR *)(&outRefVecX), (VECTOR *)(&resX));
        aimPtr->componentB = resX.vx;
        aimPtr->componentY = resX.vy;
        aimPtr->componentA = resX.vz;
        if (Submarine_NormalizeVectorFixed((struct SubmarineAimRecord *)((int)aimPtr + 0x10)) == 0) {
            outRefVecX.vx = g_SubmarineState.copyVector;
            outRefVecX.vy = g_SubmarineState.copyVectorY;
            outRefVecX.vz = g_SubmarineState.copyVectorZ;
            CrossProductFixed12((VECTOR *)(&tempDirX), (VECTOR *)(&outRefVecX), (VECTOR *)(&resX));
            aimPtr->componentB = resX.vx;
            aimPtr->componentY = resX.vy;
            aimPtr->componentA = resX.vz;
            if (Submarine_NormalizeVectorFixed((struct SubmarineAimRecord *)((int)aimPtr + 0x10)) == 0) {
                outRefVecX.vx = g_SubmarineState.seedVector;
                outRefVecX.vy = g_SubmarineState.seedVectorY;
                outRefVecX.vz = g_SubmarineState.seedVectorZ;
                CrossProductFixed12((VECTOR *)(&tempDirX), (VECTOR *)(&outRefVecX), (VECTOR *)(&resX));
                aimPtr->componentB = resX.vx;
                aimPtr->componentY = resX.vy;
                aimPtr->componentA = resX.vz;
                Submarine_NormalizeVectorFixed((struct SubmarineAimRecord *)((int)aimPtr + 0x10));
            }
        }
        outRefVecX.vx = aimPtr->deltaX;
        outRefVecX.vy = 0;
        outRefVecX.vz = aimPtr->deltaZ;
        resX.vx = outRefVecX.vx * outRefVecX.vx;
        resX.vy = 0;
        resX.vz = outRefVecX.vz * outRefVecX.vz;
        totalSq = resX.vx;
        zSquared = resX.vz;
        totalSq += zSquared;
        leadBits = Bit_CountLeadingSignBits(totalSq);
        leadBits = leadBits >> 1 << 1;
        shiftBits = (0x1F - leadBits) / 2;
        if (leadBits == 0x20) {
            totalSq = 0;
        }
        if (leadBits < 0x18) {
            totalSq >>= 0x18 - leadBits;
        } else {
            totalSq <<= leadBits - 0x18;
        }
        totalSq = g_SubmarineModelIndicesA[totalSq - 64] << shiftBits >> 0xC;
        flightPtr->aimPitch = -Submarine_ComputeFixedAngleFromComponents(aimPtr->deltaY, totalSq);
        flightPtr->aimYaw = -Submarine_ComputeFixedAngleFromComponents(aimPtr->componentA, aimPtr->componentB);
        flightPtr->aimRoll = 0;
        flightPtr++;
    } while (flightPtr < &g_SubmarineFlightRecords[32]);
}

/*
 * Computes a signed fixed angle from two components, handling zero vector and
 * zero second component explicitly. Uses absolute components in the first
 * octant, complements around quarter- turn after swapping, then applies half-
 * turn and sign corrections.
 */
// FUNCTION: FF7 0x0079c2e3
int __cdecl Submarine_ComputeFixedAngleFromComponents(int x, int y)
{
    int result;
    int absY;
    int xabs;
    int swap;

    result = 0;
    if (x < 0) {
        xabs = -x;
    } else {
        xabs = x;
    }
    if (y < 0) {
        absY = -y;
    } else {
        absY = y;
    }
    if (absY == 0) {
        if (xabs == 0) {
            return 0;
        }
        result = 0x400;
        /* goto kept: the original jumps to a stub after the function end that jumps on; falling out of the if jumps directly */
        goto signs;
    } else {
        swap = 0;
        if (xabs >= absY) {
            swap = absY;
            absY = xabs;
            xabs = swap;
            swap = 1;
        }
        result = ArcTangentFixed(xabs, absY);
        if (swap == 1) {
            result = 0x400 - result;
        }
    }
signs:
    if (y < 0) {
        result = 0x800 - result;
    }
    if (x < 0) {
        result = -result;
    }
    return result;
}

/*
 * Computes squared vector magnitude, derives an even normalization exponent
 * and indexes two fixed tables. Scales all three components in place by the
 * selected factor/shift and returns the separately shifted magnitude-table
 * value.
 */
// FUNCTION: FF7 0x0079c3ab
int __cdecl Submarine_NormalizeVectorFixed(SubmarineAimRecord *vector)
{
    int zSqTmp;
    int padSlot; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int ySquared;
    int xSquared;
    int magnitude;
    int normExponent;
    int shiftValue;
    int zComp;
    int vecY;
    int scaleRatio;
    int xComp;

    xComp = vector->deltaX;
    vecY = vector->deltaY;
    zComp = vector->deltaZ;
    xSquared = xComp * xComp;
    ySquared = vecY * vecY;
    zSqTmp = zComp * zComp;
    magnitude = xSquared + ySquared + zSqTmp;
    normExponent = Bit_CountLeadingSignBits(magnitude);
    normExponent = normExponent >> 1 << 1;
    shiftValue = ((0x1F - normExponent) / 2);
    if (normExponent == 0x20) {
        magnitude = 0;
    }
    if (normExponent < 0x18) {
        magnitude >>= 0x18 - normExponent;
    } else {
        magnitude <<= normExponent - 0x18;
    }
    scaleRatio = g_SubmarineModelIndicesB[magnitude - 64];
    magnitude = g_SubmarineModelIndicesA[magnitude - 64];
    xSquared = xComp * scaleRatio;
    ySquared = vecY * scaleRatio;
    zSqTmp = zComp * scaleRatio;
    magnitude = magnitude << shiftValue >> 0xC;
    xComp = xSquared;
    vecY = ySquared;
    zComp = zSqTmp;
    vector->deltaX = xComp >> shiftValue;
    vector->deltaY = vecY >> shiftValue;
    vector->deltaZ = zComp >> shiftValue;
    return magnitude;
}

/*
 * Eases two 12-bit record angles toward targets, trying error divisors 16, 8,
 * 4, 2 and 1 to obtain a nonzero step, normalizes them to the signed half-
 * circle, spins roll by 256 and rebuilds orientation through
 * Submarine_BuildRecordRotationBasis. The step local is shared: if the second
 * angle already matches its target, it retains and applies the first angle
 * step.
 */
// FUNCTION: FF7 0x0079c4df
void __cdecl Submarine_UpdateRecordOrientation(SubmarineFlightRecord *orientation)
{
    short vecArr[4];
    char a6[0x18];
    short dirVector[4];
    int angleTarget;
    int angleDiff;
    int stepAngle;
    int curAngle;
    int dot;
    short yAngleSin;
    short sinAngle;
    short cosineYValue;
    short cosine;
    int cellZ;
    int gridCol;
    int index;
    int cur;
    int left;

    stepAngle = 0;
    angleTarget = (orientation->aimPitch + 0x1000) & 0xFFF;
    curAngle = (orientation->angleA + 0x1000) & 0xFFF;
    angleDiff = angleTarget - curAngle;
    if (angleDiff > 0x800)
        angleDiff -= 0x1000;
    if (angleDiff < -0x800)
        angleDiff += 0x1000;
    if (angleDiff > 0) {
        stepAngle = angleDiff / 16;
        if (stepAngle == 0)
            stepAngle = angleDiff / 8;
        if (stepAngle == 0)
            stepAngle = angleDiff / 4;
        if (stepAngle == 0)
            stepAngle = angleDiff / 2;
        if (stepAngle == 0)
            stepAngle = angleDiff;
    } else if (angleDiff < 0) {
        stepAngle = angleDiff / 16;
        if (stepAngle == 0)
            stepAngle = angleDiff / 8;
        if (stepAngle == 0)
            stepAngle = angleDiff / 4;
        if (stepAngle == 0)
            stepAngle = angleDiff / 2;
        if (stepAngle == 0)
            stepAngle = angleDiff;
    }
    curAngle += stepAngle;
    if (curAngle > 0x800)
        curAngle -= 0x1000;
    if (curAngle < -0x800)
        curAngle += 0x1000;
    orientation->angleA = curAngle;
    angleTarget = (orientation->aimYaw + 0x1000) & 0xFFF;
    curAngle = (orientation->angleB + 0x1000) & 0xFFF;
    angleDiff = angleTarget - curAngle;
    if (angleDiff > 0x800)
        angleDiff -= 0x1000;
    else if (angleDiff < -0x800)
        angleDiff += 0x1000;
    if (angleDiff > 0) {
        stepAngle = angleDiff / 16;
        if (stepAngle == 0)
            stepAngle = angleDiff / 8;
        if (stepAngle == 0)
            stepAngle = angleDiff / 4;
        if (stepAngle == 0)
            stepAngle = angleDiff / 2;
        if (stepAngle == 0)
            stepAngle = angleDiff;
    } else if (angleDiff < 0) {
        stepAngle = angleDiff / 16;
        if (stepAngle == 0)
            stepAngle = angleDiff / 8;
        if (stepAngle == 0)
            stepAngle = angleDiff / 4;
        if (stepAngle == 0)
            stepAngle = angleDiff / 2;
        if (stepAngle == 0)
            stepAngle = angleDiff;
    }
    curAngle += stepAngle;
    if (curAngle > 0x800)
        curAngle -= 0x1000;
    if (curAngle < -0x800)
        curAngle += 0x1000;
    orientation->angleB = curAngle;
    orientation->heightOffset = (orientation->heightOffset + 0x100) & 0xFFF;
    Submarine_BuildRecordRotationBasis((struct SubmarineRecordAngles *)&orientation->angleA, (struct SubmarineRecordMatrixRow *)&orientation->vecX);
    sinAngle = SinFixed(orientation->aimPitch);
    cosine = CosFixed(orientation->aimPitch);
    yAngleSin = SinFixed(orientation->aimYaw);
    cosineYValue = CosFixed(orientation->aimYaw);
    orientation->aimDirX = cosine * yAngleSin >> 0xC;
    orientation->aimDirY = -sinAngle;
    orientation->aimDirZ = cosine * cosineYValue >> 0xC;
    vecArr[0] = orientation->velocityX;
    vecArr[1] = orientation->velocityY;
    vecArr[2] = orientation->velocityZ;
    dirVector[0] = orientation->aimDirX;
    dirVector[1] = orientation->aimDirY;
    dirVector[2] = orientation->aimDirZ;
    dot = (vecArr[0] * dirVector[0] + vecArr[1] * dirVector[1] + vecArr[2] * dirVector[2]) >> 0xC;
    orientation->aimDotProduct = dot;
    gridCol = ((orientation->posX >> 0xC) + 0x800) / 64;
    cellZ = (0x800 - (orientation->posZ >> 0xC)) / 64;
    index = gridCol + (cellZ << 6);
    orientation->gridCellIndex = index;
    cur = orientation->speed;
    left = orientation->workingLevel - cur;
    if (left > 4)
        cur += left / 4;
    else if (left > 0)
        cur += 1;
    else if (left < 0)
        cur -= 1;
    orientation->speed = cur;
}

/*
 * Builds three fixed-point rotation basis vectors from the angleX, angleY and angleZ shorts of
 * rotationAngles, masking each to 12 bits for sine/cosine lookup. Forms an X-axis short matrix at scale 4096, transforms the Z-axis
 * basis vectors through TransformVectorToInt32, then applies a Y-axis short
 * matrix to the intermediates.
 */
// FUNCTION: FF7 0x0079c970
void __cdecl Submarine_BuildRecordRotationBasis(SubmarineRecordAngles *rotationAngles, SubmarineRecordMatrixRow *matrix)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[120];
        int cosAngleY;
        struct { unsigned char pad_to_4[0x4]; int sinAngleY; };
        struct { unsigned char pad_to_8[0x8]; int cosAngleZ; };
        struct { unsigned char pad_to_C[0xC]; int sinAngleZ; };
        struct { unsigned char pad_to_10[0x10]; int cosAngleX; };
        struct { unsigned char pad_to_14[0x14]; int sinAngleX; };
        struct { unsigned char pad_to_18[0x18]; VECTOR basisAX; };
        struct { unsigned char pad_to_28[0x28]; VECTOR basisCX; };
        struct { unsigned char pad_to_38[0x38]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_58[0x58]; VECTOR basisBX; };
        struct { unsigned char pad_to_68[0x68]; VECTOR xform; };
    } frame;
#pragma pack(pop)

    frame.cosAngleX = CosFixed(rotationAngles->angleX + 0x1000 & 0xFFF);
    frame.sinAngleX = SinFixed(rotationAngles->angleX + 0x1000 & 0xFFF);
    frame.rotMatrix.m[0][0] = 0x1000;
    frame.rotMatrix.m[0][1] = 0;
    frame.rotMatrix.m[0][2] = 0;
    frame.rotMatrix.m[1][0] = 0;
    frame.rotMatrix.m[1][1] = *(short *)&frame.cosAngleX;
    frame.rotMatrix.m[1][2] = -frame.sinAngleX;
    frame.rotMatrix.m[2][0] = 0;
    frame.rotMatrix.m[2][1] = *(short *)&frame.sinAngleX;
    frame.rotMatrix.m[2][2] = *(short *)&frame.cosAngleX;
    frame.cosAngleZ = CosFixed(rotationAngles->angleZ + 0x1000 & 0xFFF);
    frame.sinAngleZ = SinFixed(rotationAngles->angleZ + 0x1000 & 0xFFF);
    frame.basisAX.vx = *(short *)&frame.cosAngleZ;
    frame.basisAX.vy = *(short *)&frame.sinAngleZ;
    frame.basisAX.vz = 0;
    frame.basisBX.vx = (short)-frame.sinAngleZ;
    frame.basisBX.vy = *(short *)&frame.cosAngleZ;
    frame.basisBX.vz = 0;
    frame.basisCX.vx = 0;
    frame.basisCX.vy = 0;
    frame.basisCX.vz = 0x1000;
    TransformVectorToInt32(&frame.rotMatrix, &frame.basisAX, &frame.xform.vx);
    frame.basisAX.vx = frame.xform.vx;
    frame.basisAX.vy = frame.xform.vy;
    frame.basisAX.vz = frame.xform.vz;
    TransformVectorToInt32(&frame.rotMatrix, &frame.basisBX, &frame.xform.vx);
    frame.basisBX.vx = frame.xform.vx;
    frame.basisBX.vy = frame.xform.vy;
    frame.basisBX.vz = frame.xform.vz;
    TransformVectorToInt32(&frame.rotMatrix, &frame.basisCX, &frame.xform.vx);
    frame.basisCX.vx = frame.xform.vx;
    frame.basisCX.vy = frame.xform.vy;
    frame.basisCX.vz = frame.xform.vz;
    frame.cosAngleY = CosFixed(rotationAngles->angleY + 0x1000 & 0xFFF);
    frame.sinAngleY = SinFixed(rotationAngles->angleY + 0x1000 & 0xFFF);
    frame.rotMatrix.m[0][0] = *(short *)&frame.cosAngleY;
    frame.rotMatrix.m[0][1] = 0;
    frame.rotMatrix.m[0][2] = *(short *)&frame.sinAngleY;
    frame.rotMatrix.m[1][0] = 0;
    frame.rotMatrix.m[1][1] = 0x1000;
    frame.rotMatrix.m[1][2] = 0;
    frame.rotMatrix.m[2][0] = -frame.sinAngleY;
    frame.rotMatrix.m[2][1] = 0;
    frame.rotMatrix.m[2][2] = *(short *)&frame.cosAngleY;
    TransformVectorToInt32(&frame.rotMatrix, &frame.basisAX, &frame.xform.vx);
    matrix->elemX = *(short *)&frame.xform.vx;
    matrix->elemY = *(short *)&frame.xform.vy;
    matrix->elemZ = *(short *)&frame.xform.vz;
    TransformVectorToInt32(&frame.rotMatrix, &frame.basisBX, &frame.xform.vx);
    matrix[1].elemX = *(short *)&frame.xform.vx;
    matrix[1].elemY = *(short *)&frame.xform.vy;
    matrix[1].elemZ = *(short *)&frame.xform.vz;
    TransformVectorToInt32(&frame.rotMatrix, &frame.basisCX, &frame.xform.vx);
    matrix[2].elemX = *(short *)&frame.xform.vx;
    matrix[2].elemY = *(short *)&frame.xform.vy;
    matrix[2].elemZ = *(short *)&frame.xform.vz;
}

// Computes the displacement of the record from the submarine player position: the record's posX,
// posY and posZ minus the player's, each shifted right by 12. The low shorts of the three
// components are stored as offsetX, offsetY and offsetZ. The components are squared and summed, and
// Bit_CountLeadingSignBits of that sum gives the leading sign-bit count from which an even
// normalization exponent is derived. Two fixed lookup tables are indexed with that exponent to
// compute playerDistance and a normalized vector, then stores its dot products with the player's
// direction (dotForward) and right basis (dotRight).
// FUNCTION: FF7 0x0079cbfd
void __cdecl Submarine_ComputeRecordOffsetAndBasisDots(SubmarineFlightRecord *record)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[132];
        int dotOut;
        struct { unsigned char pad_to_4[0x4]; int scaledZ; };
        struct { unsigned char pad_to_8[0x8]; int scaledY; };
        struct { unsigned char pad_to_C[0xC]; int scaledX; };
        struct { unsigned char pad_to_10[0x10]; int leadingSignBits; };
        struct { unsigned char pad_to_14[0x14]; int normShift; };
        struct { unsigned char pad_to_18[0x18]; int partZ; };
        struct { unsigned char pad_to_1C[0x1C]; int partY; };
        struct { unsigned char pad_to_20[0x20]; int offsetZ; };
        struct { unsigned char pad_to_24[0x24]; int offsetY; };
        struct { unsigned char pad_to_28[0x28]; int offsetX; };
        struct { unsigned char pad_to_2C[0x2C]; short directionX; };
        struct { unsigned char pad_to_2E[0x2E]; short directionY; };
        struct { unsigned char pad_to_30[0x30]; short directionZ; };
        struct { unsigned char pad_to_34[0x34]; int vecX; };
        struct { unsigned char pad_to_38[0x38]; int vecY; };
        struct { unsigned char pad_to_3C[0x3C]; int vecZ; };
        struct { unsigned char pad_to_44[0x44]; short normX; };
        struct { unsigned char pad_to_46[0x46]; short normY; };
        struct { unsigned char pad_to_48[0x48]; short normZ; };
        struct { unsigned char pad_to_4A[0x4A]; short negNormX; };
        struct { unsigned char pad_to_4C[0x4C]; short negNormY; };
        struct { unsigned char pad_to_4E[0x4E]; short negNormZ; };
        struct { unsigned char pad_to_64[0x64]; int sumSq; };
        struct { unsigned char pad_to_68[0x68]; int lookupScale; };
        struct { unsigned char pad_to_6C[0x6C]; short rightBasisX; };
        struct { unsigned char pad_to_6E[0x6E]; short rightBasisY; };
        struct { unsigned char pad_to_70[0x70]; short rightBasisZ; };
        struct { unsigned char pad_to_74[0x74]; int tmpA; };
        struct { unsigned char pad_to_78[0x78]; int tmpB; };
        struct { unsigned char pad_to_7C[0x7C]; int tmpC; };
    } frame;
#pragma pack(pop)

    frame.offsetX = record->posX - g_SubmarineState.player.posX >> 0xC;
    frame.offsetY = record->posY - g_SubmarineState.player.posY >> 0xC;
    frame.offsetZ = record->posZ - g_SubmarineState.player.posZ >> 0xC;
    frame.vecX = frame.offsetX;
    frame.vecY = frame.offsetY;
    frame.vecZ = frame.offsetZ;
    frame.tmpA = frame.vecX * frame.vecX;
    frame.tmpB = frame.vecY * frame.vecY;
    frame.tmpC = frame.vecZ * frame.vecZ;
    record->offsetX = *(short *)&frame.offsetX;
    record->offsetY = *(short *)&frame.offsetY;
    record->offsetZ = *(short *)&frame.offsetZ;
    frame.sumSq = frame.tmpA;
    frame.partY = frame.tmpB;
    frame.partZ = frame.tmpC;
    frame.sumSq += frame.partY;
    frame.sumSq += frame.partZ;
    frame.vecX = frame.offsetX;
    frame.vecY = frame.offsetY;
    frame.vecZ = frame.offsetZ;
    frame.leadingSignBits = Bit_CountLeadingSignBits(frame.sumSq);
    frame.leadingSignBits = frame.leadingSignBits >> 1 << 1;
    frame.normShift = (0x1F - frame.leadingSignBits) / 2;
    if (frame.leadingSignBits == 0x20) {
        frame.sumSq = 0;
    }
    if (frame.leadingSignBits < 0x18) {
        frame.sumSq >>= 0x18 - frame.leadingSignBits;
    } else {
        frame.sumSq <<= frame.leadingSignBits - 0x18;
    }
    frame.lookupScale = g_SubmarineModelIndicesB[frame.sumSq - 64];
    frame.sumSq = g_SubmarineModelIndicesA[frame.sumSq - 64];
    frame.tmpA = frame.vecX * frame.lookupScale;
    frame.tmpB = frame.vecY * frame.lookupScale;
    frame.tmpC = frame.vecY * frame.lookupScale;
    frame.sumSq = frame.sumSq << frame.normShift >> 0xC;
    record->playerDistance = frame.sumSq;
    frame.scaledX = frame.tmpA;
    frame.scaledY = frame.tmpB;
    frame.scaledZ = frame.tmpC;
    frame.scaledX >>= frame.normShift;
    frame.scaledY >>= frame.normShift;
    frame.scaledZ >>= frame.normShift;
    frame.normX = *(short *)&frame.scaledX;
    frame.normY = *(short *)&frame.scaledY;
    frame.normZ = *(short *)&frame.scaledZ;
    frame.negNormX = -frame.scaledX;
    frame.negNormY = -frame.scaledY;
    frame.negNormZ = -frame.scaledZ;
    frame.directionX = g_SubmarineState.directionX;
    frame.directionY = g_SubmarineState.directionY;
    frame.directionZ = g_SubmarineState.directionZ;
    frame.tmpA = frame.normX * frame.directionX + frame.normY * frame.directionY + frame.normZ * frame.directionZ >> 0xC;
    frame.rightBasisX = g_SubmarineState.rightBasisX;
    frame.rightBasisY = g_SubmarineState.rightBasisY;
    frame.rightBasisZ = g_SubmarineState.rightBasisZ;
    frame.dotOut = frame.tmpA;
    frame.tmpB = frame.negNormX * frame.rightBasisX + frame.negNormY * frame.rightBasisY + frame.negNormZ * frame.rightBasisZ >> 0xC;
    record->dotForward = frame.dotOut;
    frame.dotOut = frame.tmpB;
    record->dotRight = frame.dotOut;
}

// Rotates a five-slot position history kept in the record. The slot pointer connectorPtrB
// receives posX, posY and posZ, each a 4.12 fixed-point value shifted right by 12 and stored
// as shorts in the slot's x, y and z. The pointers then shift down: connectorPtrB takes
// connectorPtrA, which takes flightPointPtrC, which takes flightPointPtrB, which takes
// flightPointPtrA. The just-written slot becomes flightPointPtrA, so the oldest slot becomes
// the newest.
// FUNCTION: FF7 0x0079ce9d
void __cdecl Submarine_PushRecordPositionHistory(SubmarineFlightRecord *record)
{
    int coord;
    SubmarineDirectionVector *slot;
    int coordY;

    slot = (SubmarineDirectionVector *)(record->connectorPtrB);
    coord = record->posX;
    coordY = record->posY;
    slot->x = coord >> 0xC;
    coord = record->posZ;
    slot->y = coordY >> 0xC;
    slot->z = coord >> 0xC;
    record->connectorPtrB = record->connectorPtrA;
    record->connectorPtrA = record->flightPointPtrC;
    record->flightPointPtrC = record->flightPointPtrB;
    record->flightPointPtrB = record->flightPointPtrA;
    record->flightPointPtrA = (int)slot;
}

/*
 * Calls Submarine_UpdateRecordAimAngles once, then updates 32 records of 0xC8
 * bytes. State bit 2 steers; bits 2 or 4 move by direction times speed, tick
 * the refresh countdown and animate.
 */
// FUNCTION: FF7 0x0079cf47
void __cdecl Submarine_UpdateRecordFlightStates(void)
{
    int posX;
    SubmarineFlightRecord *record;
    int posY;
    int posZ;
    int floorGap;
    int diffTop;

    Submarine_UpdateRecordAimAngles();
    record = &g_SubmarineFlightRecords[0];
    do {
        if ((record->state & SUBMARINE_FLIGHT_HOMING) != 0) {
            Submarine_UpdateRecordOrientation(record);
        }
        posX = record->posX;
        posY = record->posY;
        posZ = record->posZ;
        if ((record->state & (SUBMARINE_FLIGHT_HOMING | SUBMARINE_FLIGHT_COASTING)) != 0) {
            posX += record->velocityX * record->speed >> 0xC;
            posY += record->velocityY * record->speed >> 0xC;
            posZ += record->velocityZ * record->speed >> 0xC;
        }
        if (posX < -0x800000 || posX > 0x800000 || posZ < -0x800000 || posZ > 0x800000 || posY < 0 || posY > 0x400000) {
            record->state = SUBMARINE_FLIGHT_IDLE;
            record->stepsLeft = 0;
            record->target = 0;
        } else {
            record->posX = posX;
            record->posY = posY;
            record->posZ = posZ;
        }
        if ((record->state & (SUBMARINE_FLIGHT_HOMING | SUBMARINE_FLIGHT_COASTING)) != 0) {
            record->timer -= 1;
            if (record->timer <= 0) {
                Submarine_PushRecordPositionHistory(record);
                record->timer = g_SubmarineTorpedoInitialSpeed;
            }
        }
        if ((record->state & (SUBMARINE_FLIGHT_HOMING | SUBMARINE_FLIGHT_COASTING)) != 0) {
            Submarine_ComputeRecordOffsetAndBasisDots(record);
        }
        if ((record->state & (SUBMARINE_FLIGHT_HOMING | SUBMARINE_FLIGHT_COASTING)) != 0) {
            if ((g_SubmarineGridCellTable[record->gridCellIndex].flags & 0x2000) != 0 && (g_SubmarineGridCellTable[record->gridCellIndex].flags & 0x8000) == 0) {
                diffTop = (record->posY >> 0xC) - g_SubmarineGridCellTable[record->gridCellIndex].raisedHeight;
                if (diffTop > -0x40 && diffTop < 0) {
                    record->state = SUBMARINE_FLIGHT_HIT_GROUND;
                    record->stepsLeft = 0xC;
                    record->target = 0;
                    g_SubmarineGridCellTable[record->gridCellIndex].flags |= 0x8080;
                    SoundDispatchCommand(SOUND_SFX_PLAY_CH4, 0x40, 0x229, 0, 0, 0, 0, 0, 0);
                }
            }
            floorGap = g_SubmarineGridCellTable[record->gridCellIndex].height - (record->posY >> 0xC);
            if (floorGap < 0) {
                record->state = SUBMARINE_FLIGHT_HIT_GROUND;
                record->stepsLeft = 0xC;
                record->target = 0;
            }
        }
        switch (record->state) {
        case SUBMARINE_FLIGHT_HOMING:
            if (record->stepsLeft > 0) {
                if (record->elapsed < record->limit) {
                    if (record->target != 0) {
                        SoundDispatchCommand(SOUND_SFX_PLAY_CH4, 0x40, 0x22A, 0, 0, 0, 0, 0, 0);
                        if (record->target == (int)&g_SubmarineState.player) {
                            (g_SubmarineScreenShake.accumulator) += g_SubmarineHudOffset;
                        }
                        ((SubmarineFlightParams *)(record->target))->countdown -= 0x200;
                        record->target = 0;
                    }
                    record->state = SUBMARINE_FLIGHT_HIT_TARGET;
                    record->stepsLeft = 0xC;
                } else if (record->aimDotProduct < 0x800 || (record->target != 0 && ((SubmarineFlightParams *)(record->target))->countdown <= 0)) {
                    record->state = SUBMARINE_FLIGHT_COASTING;
                    record->target = 0;
                    if (record->stepsLeft >= 0x20) {
                        record->stepsLeft = 0x20;
                    }
                    if (record < &g_SubmarineFlightRecords[16]) {
                        g_SubmarineActiveTorpedoCount += 1;
                    }
                } else {
                    record->targetX = ((SubmarineFlightParams *)(record->target))->posX;
                    record->targetY = ((SubmarineFlightParams *)(record->target))->posY;
                    record->targetZ = ((SubmarineFlightParams *)(record->target))->posZ;
                }
                record->stepsLeft -= 1;
            } else {
                record->state = SUBMARINE_FLIGHT_EXPIRED;
                record->target = 0;
                record->stepsLeft = 0xC;
                if (record < &g_SubmarineFlightRecords[16]) {
                    g_SubmarineActiveTorpedoCount += 1;
                }
            }
            break;
        case SUBMARINE_FLIGHT_COASTING:
            if (record->stepsLeft > 0) {
                if (record->elapsed < record->limit) {
                    record->state = SUBMARINE_FLIGHT_EXPIRED;
                    record->stepsLeft = 0xC;
                }
                record->stepsLeft -= 1;
            } else {
                record->state = SUBMARINE_FLIGHT_EXPIRED;
                record->stepsLeft = 0xC;
            }
            break;
        case SUBMARINE_FLIGHT_HIT_GROUND:
        case SUBMARINE_FLIGHT_EXPIRED:
        case SUBMARINE_FLIGHT_HIT_TARGET:
            record->stepsLeft -= 1;
            if (record->stepsLeft <= 0) {
                record->state = SUBMARINE_FLIGHT_IDLE;
                record->stepsLeft = 0;
                record->target = 0;
            }
            break;
        }
        record++;
    } while (record < &g_SubmarineFlightRecords[32]);
    SetRotMatrix(&g_SubmarineViewMatrix);
    SetTransMatrix(&g_SubmarineViewMatrix);
}

/*
 * Initializes frame-selected output storage, copies the default color tables
 * into the data block, and processes the 32 records that are not idle.
 * Selects the first two colors by record type and constructs six pairs of
 * short endpoints from the record position, five position pointers and its
 * shifted width vector.
 */
// FUNCTION: FF7 0x0079d4ad
void __cdecl Submarine_EmitRecordConnectorQuads(void)
{
    GfxHomogeneousPoint cornerVecs[12];
    GfxHomogeneousVec4 projPts[12];
    int hasVertex7;
    int flag8;
    int rotMat[18];
    int *destinationColor0;
    int valid9;
    int typeId;
    GfxTlVertex *vertPtr32;
    GfxTlVertex *row5Pointer;
    int *sourceCursor1;
    int curScreenMatrixPack[16];
    GfxTlVertex *quad3Row1;
    GfxTlVertex *quad3Vtx0;
    GfxTlVertex *rowPointer52;
    GfxTlVertex *quad2VertPtr;
    GfxTlVertex *vertPointer22;
    GfxTlVertex *quad2FirstVtx;
    GfxTlVertex *vtxFirst5Ptr;
    GfxTlVertex *outVertex;
    GfxTlVertex *quad1ThirdVtx;
    GfxTlVertex *vtxSecond1Pointer;
    int subHasVert;
    GfxTlVertex *quad1Row1;
    GfxTlVertex *quad1Vtx0;
    GfxTlVertex *quad5Row0;
    int dy2;
    int xW;
    int connectorZVal;
    GfxTlVertex *rowThird4Ptr;
    int baseY;
    int connectorX8;
    int vtx3Valid6;
    int vtx4Valid3;
    SubmarineConnectorRecord *pConnectorPtr;
    GfxTlVertex *quad4Pointer2;
    int recWZ;
    GfxTlVertex *rowFirst4Ptr;
    int vecY;
    int dx;
    int hasVertex5;
    int zCoord;
    int posY8;
    GfxTlVertex *rowPtr40;
    int *colorDest1;
    int anchorX;
    SubmarineFlightRecordPoint *flightPointPtr;
    GfxTlVertex *quad3VtxPointer;
    int *sourceColor0;
    int widthZ;
    float workRecip;
    int recordIdx;
    SubmarineConnectorQuadValues *colorPair;
    int valid6;
    SubmarineRecordConnector *recPtr;

    recPtr = (SubmarineRecordConnector *)Gfx_GetPsxScratchpadPtr(0);
    recPtr->vertexData = g_SubmarineSubState * 0x1734 + (int)(int)((char *)&g_SubmarineFlightVertexData + 0x1b00);
    recPtr->cameraBatch = g_SubmarineSubState * 0x1800 + (int)&g_SubmarineCameraBatchBuffer;
    recPtr->primTag = 0x3A000000;
    sourceColor0 = (int *)&g_SubmarineConnectorColor0;
    destinationColor0 = (int *)((int)recPtr + 0x9C);
    recordIdx = 0;
    do {
        *destinationColor0 = *sourceColor0;
        destinationColor0++;
        sourceColor0++;
    } while ((unsigned int)recordIdx++ < (unsigned int)0x14);
    sourceCursor1 = (int *)&g_SubmarineConnectorColor1;
    colorDest1 = (int *)((int)recPtr + 0xEC);
    recordIdx = 0;
    do {
        *colorDest1 = *sourceCursor1;
        colorDest1++;
        sourceCursor1++;
    } while ((unsigned int)recordIdx++ < (unsigned int)6);
    recPtr->quadColors = (SubmarineConnectorQuadValues *)((int)recPtr + 0xEC);
    SetRotMatrix(&g_SubmarineViewMatrix);
    SetTransMatrix(&g_SubmarineViewMatrix);
    GtePsxMatrixToFloat4x4Transposed(GetInstalledRotMatrixPtr(), rotMat);
    ComposeWithScreenMatrix(rotMat, curScreenMatrixPack);
    recordIdx = 0;
    do {
        typeId = g_SubmarineFlightRecords[recordIdx].state;
        if (typeId == SUBMARINE_FLIGHT_IDLE) { /* empty first arm: lib/codegen.h */
        } else {
            if (typeId == SUBMARINE_FLIGHT_HOMING) {
                recPtr->quadColors->colorA = recPtr->colorSteering;
                recPtr->quadColors->colorB = recPtr->quadColors->colorA;
            } else if (typeId == SUBMARINE_FLIGHT_COASTING) {
                recPtr->quadColors->colorA = recPtr->colorCoasting;
                recPtr->quadColors->colorB = recPtr->quadColors->colorA;
            } else if (typeId == SUBMARINE_FLIGHT_HIT_TARGET) {
                recPtr->quadColors->colorA = recPtr->colorHitTarget;
                recPtr->quadColors->colorB = recPtr->quadColors->colorA;
            } else if (typeId == SUBMARINE_FLIGHT_HIT_GROUND) {
                recPtr->quadColors->colorA = recPtr->colorHitGround;
                recPtr->quadColors->colorB = recPtr->quadColors->colorA;
            } else {
                recPtr->quadColors->colorA = recPtr->colorExpired;
                recPtr->quadColors->colorB = recPtr->quadColors->colorA;
            }
            colorPair = recPtr->quadColors;
            dx = g_SubmarineFlightRecords[recordIdx].vecX >> 0xA;
            vecY = g_SubmarineFlightRecords[recordIdx].vecY >> 0xA;
            widthZ = g_SubmarineFlightRecords[recordIdx].vecZ >> 0xA;
            recPtr->deltaX = dx;
            recPtr->deltaY = vecY;
            recPtr->deltaZ = widthZ;
            flightPointPtr = (SubmarineFlightRecordPoint *)(g_SubmarineFlightRecords[recordIdx].flightPointPtrA);
            anchorX = g_SubmarineFlightRecords[recordIdx].posX;
            posY8 = g_SubmarineFlightRecords[recordIdx].posY;
            zCoord = g_SubmarineFlightRecords[recordIdx].posZ;
            anchorX >>= 0xC;
            posY8 >>= 0xC;
            zCoord >>= 0xC;
            recPtr->corner0X = anchorX + (short)dx;
            cornerVecs[0].x = (float)recPtr->corner0X;
            recPtr->corner0Y = posY8 + (short)vecY;
            cornerVecs[0].y = (float)recPtr->corner0Y;
            recPtr->corner0Z = zCoord + (short)widthZ;
            cornerVecs[0].z = (float)recPtr->corner0Z;
            recPtr->corner1X = anchorX - (short)dx;
            cornerVecs[1].x = (float)recPtr->corner1X;
            recPtr->corner1Y = posY8 - (short)vecY;
            cornerVecs[1].y = (float)recPtr->corner1Y;
            recPtr->corner1Z = zCoord - (short)widthZ;
            cornerVecs[1].z = (float)recPtr->corner1Z;
            anchorX = flightPointPtr->anchorX;
            posY8 = flightPointPtr->anchorY;
            zCoord = flightPointPtr->zCoord;
            recPtr->corner2X = anchorX + (short)dx;
            cornerVecs[2].x = (float)recPtr->corner2X;
            recPtr->corner2Y = posY8 + (short)vecY;
            cornerVecs[2].y = (float)recPtr->corner2Y;
            recPtr->corner2Z = zCoord + (short)widthZ;
            cornerVecs[2].z = (float)recPtr->corner2Z;
            recPtr->corner3X = anchorX - (short)dx;
            cornerVecs[3].x = (float)recPtr->corner3X;
            recPtr->corner3Y = posY8 - (short)vecY;
            cornerVecs[3].y = (float)recPtr->corner3Y;
            recPtr->corner3Z = zCoord - (short)widthZ;
            cornerVecs[3].z = (float)recPtr->corner3Z;
            flightPointPtr = (SubmarineFlightRecordPoint *)(g_SubmarineFlightRecords[recordIdx].flightPointPtrB);
            anchorX = flightPointPtr->anchorX;
            posY8 = flightPointPtr->anchorY;
            zCoord = flightPointPtr->zCoord;
            flightPointPtr = (SubmarineFlightRecordPoint *)(g_SubmarineFlightRecords[recordIdx].flightPointPtrC);
            recPtr->corner4X = anchorX + (short)dx;
            cornerVecs[4].x = (float)recPtr->corner4X;
            recPtr->corner4Y = posY8 + (short)vecY;
            cornerVecs[4].y = (float)recPtr->corner4Y;
            recPtr->corner4Z = zCoord + (short)widthZ;
            cornerVecs[4].z = (float)recPtr->corner4Z;
            recPtr->corner5X = anchorX - (short)dx;
            cornerVecs[5].x = (float)recPtr->corner5X;
            recPtr->corner5Y = posY8 - (short)vecY;
            cornerVecs[5].y = (float)recPtr->corner5Y;
            recPtr->corner5Z = zCoord - (short)widthZ;
            cornerVecs[5].z = (float)recPtr->corner5Z;
            anchorX = flightPointPtr->anchorX;
            posY8 = flightPointPtr->anchorY;
            zCoord = flightPointPtr->zCoord;
            recPtr->corner6X = anchorX + (short)dx;
            cornerVecs[6].x = (float)recPtr->corner6X;
            recPtr->corner6Y = posY8 + (short)vecY;
            cornerVecs[6].y = (float)recPtr->corner6Y;
            recPtr->corner6Z = zCoord + (short)widthZ;
            cornerVecs[6].z = (float)recPtr->corner6Z;
            recPtr->corner7X = anchorX - (short)dx;
            cornerVecs[7].x = (float)recPtr->corner7X;
            recPtr->corner7Y = posY8 - (short)vecY;
            cornerVecs[7].y = (float)recPtr->corner7Y;
            recPtr->corner7Z = zCoord - (short)widthZ;
            cornerVecs[7].z = (float)recPtr->corner7Z;
            pConnectorPtr = (SubmarineConnectorRecord *)(g_SubmarineFlightRecords[recordIdx].connectorPtrA);
            connectorX8 = pConnectorPtr->connectorX;
            baseY = pConnectorPtr->baseY;
            connectorZVal = pConnectorPtr->connectorZVal;
            xW = recPtr->deltaX;
            dy2 = recPtr->deltaY;
            recWZ = recPtr->deltaZ;
            pConnectorPtr = (SubmarineConnectorRecord *)(g_SubmarineFlightRecords[recordIdx].connectorPtrB);
            recPtr->corner8X = connectorX8 + (short)xW;
            cornerVecs[8].x = (float)recPtr->corner8X;
            recPtr->corner8Y = baseY + (short)dy2;
            cornerVecs[8].y = (float)recPtr->corner8Y;
            recPtr->corner8Z = connectorZVal + (short)recWZ;
            cornerVecs[8].z = (float)recPtr->corner8Z;
            recPtr->corner9X = connectorX8 - (short)xW;
            cornerVecs[9].x = (float)recPtr->corner9X;
            recPtr->corner9Y = baseY - (short)dy2;
            cornerVecs[9].y = (float)recPtr->corner9Y;
            recPtr->corner9Z = connectorZVal - (short)recWZ;
            cornerVecs[9].z = (float)recPtr->corner9Z;
            connectorX8 = pConnectorPtr->connectorX;
            baseY = pConnectorPtr->baseY;
            connectorZVal = pConnectorPtr->connectorZVal;
            recPtr->corner10X = connectorX8 + (short)xW;
            cornerVecs[10].x = (float)recPtr->corner10X;
            recPtr->corner10Y = baseY + (short)dy2;
            cornerVecs[10].y = (float)recPtr->corner10Y;
            recPtr->corner10Z = connectorZVal + (short)recWZ;
            cornerVecs[10].z = (float)recPtr->corner10Z;
            recPtr->corner11X = connectorX8 - (short)xW;
            cornerVecs[11].x = (float)recPtr->corner11X;
            recPtr->corner11Y = baseY - (short)dy2;
            cornerVecs[11].y = (float)recPtr->corner11Y;
            recPtr->corner11Z = connectorZVal - (short)recWZ;
            cornerVecs[11].z = (float)recPtr->corner11Z;
            TransformPointByMatrix4x4(curScreenMatrixPack, &cornerVecs[0], &projPts[0]);
            if (projPts[0].w > 0.0) {
                TransformPointByMatrix4x4(curScreenMatrixPack, &cornerVecs[1], &projPts[1]);
                if (projPts[1].w > 0.0) {
                    TransformPointByMatrix4x4(curScreenMatrixPack, &cornerVecs[2], &projPts[2]);
                    if (projPts[2].w > 0.0) {
                        subHasVert = 1;
                        TransformPointByMatrix4x4(curScreenMatrixPack, &cornerVecs[3], &projPts[3]);
                        if (projPts[3].w > 0.0) {
                            vtx3Valid6 = 1;
                            workRecip = 1.0f / projPts[0].w;
                            projPts[0].x *= workRecip;
                            projPts[0].y *= workRecip;
                            projPts[0].z *= workRecip;
                            workRecip = 1.0f / projPts[1].w;
                            projPts[1].x *= workRecip;
                            projPts[1].y *= workRecip;
                            projPts[1].z *= workRecip;
                            workRecip = 1.0f / projPts[2].w;
                            projPts[2].x *= workRecip;
                            projPts[2].y *= workRecip;
                            projPts[2].z *= workRecip;
                            workRecip = 1.0f / projPts[3].w;
                            projPts[3].x *= workRecip;
                            projPts[3].y *= workRecip;
                            projPts[3].z *= workRecip;
                            if (AcquireBatchNode(1, (void *)(((int)g_SubmarineConnectorBatchContext))) != 0) {
                                quad1Vtx0 = (GfxTlVertex *)(g_SubmarineConnectorBatchContext->vertexCursor);
                                quad1Vtx0->x = projPts[0].x;
                                quad1Vtx0->y = projPts[0].y;
                                quad1Vtx0->z = projPts[0].z;
                                quad1Vtx0->rhw = projPts[0].w;
                                quad1Vtx0->color = colorPair->colorA;
                                quad1Row1 = (GfxTlVertex *)(g_SubmarineConnectorBatchContext->vertexCursor + 0x20);
                                quad1Row1->x = projPts[1].x;
                                quad1Row1->y = projPts[1].y;
                                quad1Row1->z = projPts[1].z;
                                quad1Row1->rhw = projPts[1].w;
                                quad1Row1->color = colorPair->colorA;
                                vtxSecond1Pointer = (GfxTlVertex *)(g_SubmarineConnectorBatchContext->vertexCursor + 0x40);
                                vtxSecond1Pointer->x = projPts[2].x;
                                vtxSecond1Pointer->y = projPts[2].y;
                                vtxSecond1Pointer->z = projPts[2].z;
                                vtxSecond1Pointer->rhw = projPts[2].w;
                                vtxSecond1Pointer->color = colorPair->colorB;
                                quad1ThirdVtx = (GfxTlVertex *)(g_SubmarineConnectorBatchContext->vertexCursor + 0x60);
                                quad1ThirdVtx->x = projPts[3].x;
                                quad1ThirdVtx->y = projPts[3].y;
                                quad1ThirdVtx->z = projPts[3].z;
                                quad1ThirdVtx->rhw = projPts[3].w;
                                quad1ThirdVtx->color = colorPair->colorB;
                            }
                        }
                    }
                }
            }
            TransformPointByMatrix4x4(curScreenMatrixPack, &cornerVecs[4], &projPts[4]);
            if (projPts[4].w > 0.0) {
                vtx4Valid3 = 1;
                TransformPointByMatrix4x4(curScreenMatrixPack, &cornerVecs[5], &projPts[5]);
                if (projPts[5].w > 0.0) {
                    hasVertex5 = 1;
                    if (subHasVert != 0 && vtx3Valid6 != 0) {
                        workRecip = 1.0f / projPts[4].w;
                        projPts[4].x *= workRecip;
                        projPts[4].y *= workRecip;
                        projPts[4].z *= workRecip;
                        workRecip = 1.0f / projPts[5].w;
                        projPts[5].x *= workRecip;
                        projPts[5].y *= workRecip;
                        projPts[5].z *= workRecip;
                        if (AcquireBatchNode(1, (void *)(((int)g_SubmarineConnectorBatchContext))) != 0) {
                            outVertex = (GfxTlVertex *)(g_SubmarineConnectorBatchContext->vertexCursor);
                            outVertex->x = projPts[2].x;
                            outVertex->y = projPts[2].y;
                            outVertex->z = projPts[2].z;
                            outVertex->rhw = projPts[2].w;
                            outVertex->color = colorPair->colorB;
                            quad2FirstVtx = (GfxTlVertex *)(g_SubmarineConnectorBatchContext->vertexCursor + 0x20);
                            quad2FirstVtx->x = projPts[3].x;
                            quad2FirstVtx->y = projPts[3].y;
                            quad2FirstVtx->z = projPts[3].z;
                            quad2FirstVtx->rhw = projPts[3].w;
                            quad2FirstVtx->color = colorPair->colorB;
                            vertPointer22 = (GfxTlVertex *)(g_SubmarineConnectorBatchContext->vertexCursor + 0x40);
                            vertPointer22->x = projPts[4].x;
                            vertPointer22->y = projPts[4].y;
                            vertPointer22->z = projPts[4].z;
                            vertPointer22->rhw = projPts[4].w;
                            vertPointer22->color = colorPair->colorC;
                            quad2VertPtr = (GfxTlVertex *)(g_SubmarineConnectorBatchContext->vertexCursor + 0x60);
                            quad2VertPtr->x = projPts[5].x;
                            quad2VertPtr->y = projPts[5].y;
                            quad2VertPtr->z = projPts[5].z;
                            quad2VertPtr->rhw = projPts[5].w;
                            quad2VertPtr->color = colorPair->colorC;
                        }
                    }
                }
            }
            TransformPointByMatrix4x4(curScreenMatrixPack, &cornerVecs[6], &projPts[6]);
            if (projPts[6].w > 0.0) {
                valid6 = 1;
                TransformPointByMatrix4x4(curScreenMatrixPack, &cornerVecs[7], &projPts[7]);
                if (projPts[7].w > 0.0) {
                    hasVertex7 = 1;
                    if (vtx4Valid3 != 0 && hasVertex5 != 0) {
                        workRecip = 1.0f / projPts[6].w;
                        projPts[6].x *= workRecip;
                        projPts[6].y *= workRecip;
                        projPts[6].z *= workRecip;
                        workRecip = 1.0f / projPts[7].w;
                        projPts[7].x *= workRecip;
                        projPts[7].y *= workRecip;
                        projPts[7].z *= workRecip;
                        if (AcquireBatchNode(1, (void *)(((int)g_SubmarineConnectorBatchContext))) != 0) {
                            quad3Vtx0 = (GfxTlVertex *)(g_SubmarineConnectorBatchContext->vertexCursor);
                            quad3Vtx0->x = projPts[4].x;
                            quad3Vtx0->y = projPts[4].y;
                            quad3Vtx0->z = projPts[4].z;
                            quad3Vtx0->rhw = projPts[4].w;
                            quad3Vtx0->color = colorPair->colorC;
                            quad3Row1 = (GfxTlVertex *)(g_SubmarineConnectorBatchContext->vertexCursor + 0x20);
                            quad3Row1->x = projPts[5].x;
                            quad3Row1->y = projPts[5].y;
                            quad3Row1->z = projPts[5].z;
                            quad3Row1->rhw = projPts[5].w;
                            quad3Row1->color = colorPair->colorC;
                            vertPtr32 = (GfxTlVertex *)(g_SubmarineConnectorBatchContext->vertexCursor + 0x40);
                            vertPtr32->x = projPts[6].x;
                            vertPtr32->y = projPts[6].y;
                            vertPtr32->z = projPts[6].z;
                            vertPtr32->rhw = projPts[6].w;
                            vertPtr32->color = colorPair->colorD;
                            quad3VtxPointer = (GfxTlVertex *)(g_SubmarineConnectorBatchContext->vertexCursor + 0x60);
                            quad3VtxPointer->x = projPts[7].x;
                            quad3VtxPointer->y = projPts[7].y;
                            quad3VtxPointer->z = projPts[7].z;
                            quad3VtxPointer->rhw = projPts[7].w;
                            quad3VtxPointer->color = colorPair->colorD;
                        }
                    }
                }
            }
            TransformPointByMatrix4x4(curScreenMatrixPack, &cornerVecs[8], &projPts[8]);
            if (projPts[8].w > 0.0) {
                flag8 = 1;
                TransformPointByMatrix4x4(curScreenMatrixPack, &cornerVecs[9], &projPts[9]);
                if (projPts[9].w > 0.0) {
                    valid9 = 1;
                    if (valid6 != 0 && hasVertex7 != 0) {
                        workRecip = 1.0f / projPts[8].w;
                        projPts[8].x *= workRecip;
                        projPts[8].y *= workRecip;
                        projPts[8].z *= workRecip;
                        workRecip = 1.0f / projPts[9].w;
                        projPts[9].x *= workRecip;
                        projPts[9].y *= workRecip;
                        projPts[9].z *= workRecip;
                        if (AcquireBatchNode(1, (void *)(((int)g_SubmarineConnectorBatchContext))) != 0) {
                            rowPtr40 = (GfxTlVertex *)(g_SubmarineConnectorBatchContext->vertexCursor);
                            rowPtr40->x = projPts[6].x;
                            rowPtr40->y = projPts[6].y;
                            rowPtr40->z = projPts[6].z;
                            rowPtr40->rhw = projPts[6].w;
                            rowPtr40->color = colorPair->colorD;
                            rowFirst4Ptr = (GfxTlVertex *)(g_SubmarineConnectorBatchContext->vertexCursor + 0x20);
                            rowFirst4Ptr->x = projPts[7].x;
                            rowFirst4Ptr->y = projPts[7].y;
                            rowFirst4Ptr->z = projPts[7].z;
                            rowFirst4Ptr->rhw = projPts[7].w;
                            rowFirst4Ptr->color = colorPair->colorD;
                            quad4Pointer2 = (GfxTlVertex *)(g_SubmarineConnectorBatchContext->vertexCursor + 0x40);
                            quad4Pointer2->x = projPts[8].x;
                            quad4Pointer2->y = projPts[8].y;
                            quad4Pointer2->z = projPts[8].z;
                            quad4Pointer2->rhw = projPts[8].w;
                            quad4Pointer2->color = colorPair->colorE;
                            rowThird4Ptr = (GfxTlVertex *)(g_SubmarineConnectorBatchContext->vertexCursor + 0x60);
                            rowThird4Ptr->x = projPts[9].x;
                            rowThird4Ptr->y = projPts[9].y;
                            rowThird4Ptr->z = projPts[9].z;
                            rowThird4Ptr->rhw = projPts[9].w;
                            rowThird4Ptr->color = colorPair->colorE;
                        }
                    }
                }
            }
            TransformPointByMatrix4x4(curScreenMatrixPack, &cornerVecs[10], &projPts[10]);
            if (projPts[10].w > 0.0) {
                TransformPointByMatrix4x4(curScreenMatrixPack, &cornerVecs[11], &projPts[11]);
                if (projPts[11].w > 0.0 && flag8 != 0 && valid9 != 0) {
                    workRecip = 1.0f / projPts[10].w;
                    projPts[10].x *= workRecip;
                    projPts[10].y *= workRecip;
                    projPts[10].z *= workRecip;
                    workRecip = 1.0f / projPts[11].w;
                    projPts[11].x *= workRecip;
                    projPts[11].y *= workRecip;
                    projPts[11].z *= workRecip;
                    if (AcquireBatchNode(1, (void *)(((int)g_SubmarineConnectorBatchContext))) != 0) {
                        quad5Row0 = (GfxTlVertex *)(g_SubmarineConnectorBatchContext->vertexCursor);
                        quad5Row0->x = projPts[8].x;
                        quad5Row0->y = projPts[8].y;
                        quad5Row0->z = projPts[8].z;
                        quad5Row0->rhw = projPts[8].w;
                        quad5Row0->color = colorPair->colorE;
                        vtxFirst5Ptr = (GfxTlVertex *)(g_SubmarineConnectorBatchContext->vertexCursor + 0x20);
                        vtxFirst5Ptr->x = projPts[9].x;
                        vtxFirst5Ptr->y = projPts[9].y;
                        vtxFirst5Ptr->z = projPts[9].z;
                        vtxFirst5Ptr->rhw = projPts[9].w;
                        vtxFirst5Ptr->color = colorPair->colorE;
                        rowPointer52 = (GfxTlVertex *)(g_SubmarineConnectorBatchContext->vertexCursor + 0x40);
                        rowPointer52->x = projPts[10].x;
                        rowPointer52->y = projPts[10].y;
                        rowPointer52->z = projPts[10].z;
                        rowPointer52->rhw = projPts[10].w;
                        rowPointer52->color = colorPair->colorF;
                        row5Pointer = (GfxTlVertex *)(g_SubmarineConnectorBatchContext->vertexCursor + 0x60);
                        row5Pointer->x = projPts[11].x;
                        row5Pointer->y = projPts[11].y;
                        row5Pointer->z = projPts[11].z;
                        row5Pointer->rhw = projPts[11].w;
                        row5Pointer->color = colorPair->colorF;
                    }
                }
            }
        }

        recordIdx++;
    } while ((unsigned int)recordIdx < (unsigned int)0x20);
}

// Initialises the thirty-two records in g_SubmarineFlightRecords. Each pass zeroes the position,
// target, angles, elapsed distance, speed and grid cell, sets the velocity and aim direction to
// the 4.12 unit Z vector (0x1000), aimDotProduct to 0x1000, stepsLeft to 0x30, limit to 0x20 and
// state to SUBMARINE_FLIGHT_IDLE. The first sixteen records take workingLevel from
// g_SubmarineWorkingLevelAngles[0], and the last sixteen take it from g_SubmarineWorkingLevelStep.
// Finally g_SubmarineActiveTorpedoCount is cleared.
// FUNCTION: FF7 0x0079eea1
void __cdecl Submarine_ResetRecordTable(void)
{
    int i;

    for (i = 0; (unsigned int)i < (unsigned int)0x20; i++) {
        g_SubmarineFlightRecords[i].posX = 0;
        g_SubmarineFlightRecords[i].posY = 0;
        g_SubmarineFlightRecords[i].posZ = 0;
        g_SubmarineFlightRecords[i].targetX = 0;
        g_SubmarineFlightRecords[i].targetY = 0;
        g_SubmarineFlightRecords[i].targetZ = 0;
        g_SubmarineFlightRecords[i].angleA = 0;
        g_SubmarineFlightRecords[i].angleB = 0;
        g_SubmarineFlightRecords[i].heightOffset = 0;
        g_SubmarineFlightRecords[i].velocityX = 0;
        g_SubmarineFlightRecords[i].velocityY = 0;
        g_SubmarineFlightRecords[i].velocityZ = 0x1000;
        g_SubmarineFlightRecords[i].aimPitch = 0;
        g_SubmarineFlightRecords[i].aimYaw = 0;
        g_SubmarineFlightRecords[i].aimRoll = 0;
        g_SubmarineFlightRecords[i].aimDirX = 0;
        g_SubmarineFlightRecords[i].aimDirY = 0;
        g_SubmarineFlightRecords[i].aimDirZ = 0x1000;
        g_SubmarineFlightRecords[i].elapsed = 0;
        g_SubmarineFlightRecords[i].aimDotProduct = 0x1000;
        g_SubmarineFlightRecords[i].speed = 0;
        g_SubmarineFlightRecords[i].stepsLeft = 0x30;
        g_SubmarineFlightRecords[i].limit = 0x20;
        g_SubmarineFlightRecords[i].state = SUBMARINE_FLIGHT_IDLE;
        g_SubmarineFlightRecords[i].gridCellIndex = 0;
        g_SubmarineFlightRecords[i].target = 0;
    }
    for (i = 0; (unsigned int)i < (unsigned int)0x10; i++) {
        g_SubmarineFlightRecords[i].workingLevel = g_SubmarineWorkingLevelAngles[0];
    }
    for (i = 0x10; (unsigned int)i < (unsigned int)0x20; i++) {
        g_SubmarineFlightRecords[i].workingLevel = g_SubmarineWorkingLevelStep;
    }
    g_SubmarineActiveTorpedoCount = 0;
}

/*
 * Finds the first ready launcher among four values equal to 0x20000, the first
 * of sixteen flight records that are idle and the first of twelve target
 * records flagged 0x800. Builds a projectile origin from player position and
 * two orientation vectors, negating the first offset for odd launcher slots.
 */
// FUNCTION: FF7 0x0079f114
void __cdecl Submarine_LaunchTorpedoRecord(void)
{
    struct {
        int packedZ;
        int packedXY;
        int speed;
        int originY;
        int originX;
        int originZ;
        struct SubmarineLaunchTorpedoRecordRecord *record;
        int slot;
        int index;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    frame.index = 0;
    do {
        if (g_SubmarineState.cooldownTable[frame.index] == 0x20000)
            break;
        frame.index++;
    } while (frame.index < 4);
    if (frame.index >= 4)
        return;
    frame.slot = frame.index;
    frame.index = 0;
    do {
        if (g_SubmarineFlightRecords[frame.index].state == SUBMARINE_FLIGHT_IDLE)
            break;
        frame.index++;
    } while (frame.index < 0x10);
    if (frame.index >= 0x10)
        return;

    frame.record = (struct SubmarineLaunchTorpedoRecordRecord *)&g_SubmarineModelVertexData;
    do {
        if (!(frame.record->flags & 0x800)) { /* empty first arm: lib/codegen.h */
        } else {
            {
                frame.originX = g_SubmarineState.rightBasisX * g_SubmarineTorpedoLaunchOffsetFactor;
                frame.originY = g_SubmarineState.rightBasisY * g_SubmarineTorpedoLaunchOffsetFactor;
                frame.originZ = g_SubmarineState.rightBasisZ * g_SubmarineTorpedoLaunchOffsetFactor;
                if (frame.slot & 1) {
                    frame.originX = -frame.originX;
                    frame.originY = -frame.originY;
                    frame.originZ = -frame.originZ;
                }
                frame.originX += (g_SubmarineState.probeOffsetX)*g_SubmarineTorpedoForwardOffsetFactor;
                frame.originY += (g_SubmarineState.probeOffsetY)*g_SubmarineTorpedoForwardOffsetFactor;
                frame.originZ += (g_SubmarineState.probeOffsetZ)*g_SubmarineTorpedoForwardOffsetFactor;
                frame.originX += g_SubmarineState.player.posX;
                frame.originY += g_SubmarineState.player.posY;
                frame.originZ += g_SubmarineState.player.posZ;

                g_SubmarineFlightRecords[frame.index].posX = frame.originX;
                g_SubmarineFlightRecords[frame.index].posY = frame.originY;
                g_SubmarineFlightRecords[frame.index].posZ = frame.originZ;
                g_SubmarineFlightRecords[frame.index].shortPosX = frame.originX >> 12;
                g_SubmarineFlightRecords[frame.index].shortPosY = frame.originY >> 12;
                g_SubmarineFlightRecords[frame.index].shortPosZ = frame.originZ >> 12;
                g_SubmarineFlightRecords[frame.index].targetX = frame.record->posX;
                g_SubmarineFlightRecords[frame.index].targetY = frame.record->posY;
                g_SubmarineFlightRecords[frame.index].targetZ = frame.record->posZ;
                g_SubmarineFlightRecords[frame.index].angleA = g_SubmarineState.player.angle;
                g_SubmarineFlightRecords[frame.index].angleB = g_SubmarineState.player.yaw;
                g_SubmarineFlightRecords[frame.index].heightOffset = g_SubmarineState.player.roll;

                frame.speed = g_SubmarineState.speedScale;
                if (frame.speed <= g_SubmarineWorkingLevelAngles[0] / 2)
                    frame.speed = g_SubmarineWorkingLevelAngles[0] / 2;
                g_SubmarineFlightRecords[frame.index].speed = frame.speed;
                g_SubmarineFlightRecords[frame.index].stepsLeft = 0x30;
                g_SubmarineFlightRecords[frame.index].state = SUBMARINE_FLIGHT_HOMING;
                g_SubmarineFlightRecords[frame.index].target = (int)frame.record;
                g_SubmarineState.cooldownTable[frame.slot] = 0;
                g_SubmarineFlightRecords[frame.index].timer = g_SubmarineTorpedoInitialSpeed;

                g_SubmarineFlightRecords[frame.index].flightPointPtrA = (int)&g_SubmarineFlightRecords[frame.index].shortPosX;
                g_SubmarineFlightRecords[frame.index].flightPointPtrB = (int)&g_SubmarineFlightRecords[frame.index].flightBValue;
                g_SubmarineFlightRecords[frame.index].flightPointPtrC = (int)&g_SubmarineFlightRecords[frame.index].flightCValue;
                g_SubmarineFlightRecords[frame.index].connectorPtrA = (int)&g_SubmarineFlightRecords[frame.index].connectorAValue;
                g_SubmarineFlightRecords[frame.index].connectorPtrB = (int)&g_SubmarineFlightRecords[frame.index].connectorBValue;
                frame.packedXY = *(int *)&g_SubmarineFlightRecords[frame.index].shortPosX;
                frame.packedZ = *(int *)&g_SubmarineFlightRecords[frame.index].shortPosZ;
                g_SubmarineFlightRecords[frame.index].flightBValue = frame.packedXY;
                g_SubmarineFlightRecords[frame.index].flightBParam = frame.packedZ;
                g_SubmarineFlightRecords[frame.index].flightCValue = frame.packedXY;
                g_SubmarineFlightRecords[frame.index].flightCParam = frame.packedZ;
                g_SubmarineFlightRecords[frame.index].connectorAValue = frame.packedXY;
                g_SubmarineFlightRecords[frame.index].connectorAParam = frame.packedZ;
                g_SubmarineFlightRecords[frame.index].connectorBValue = frame.packedXY;
                g_SubmarineFlightRecords[frame.index].connectorBParam = frame.packedZ;
                SoundDispatchCommand(SOUND_SFX_PLAY_CH4, 0x40, 0x1d2, 0, 0, 0, 0, 0, 0);
                break;
            }
        }

        frame.record++;
    } while ((unsigned int)(int)frame.record < (unsigned int)(int)&g_SubmarineLevel0Map);
}

/*
 * Draw hook for render context renderContext: selects scene pass 1, turns render state
 * 0xE on, emits the primitive batch list g_SubmarineConnectorBatchContext,
 * turns render state 0xE back off and returns to scene pass 0.
 */
// FUNCTION: FF7 0x0079f56b
void __cdecl Submarine_DrawRecordQuadPass(struct GfxEngineContext *renderContext)
{
    Gfx_SetScenePassCall(1, (struct GfxEngineContext *)renderContext);
    Gfx_SetRenderStateCall(RENDER_STATE_NO_CULL, 1, (struct GfxEngineContext *)renderContext);
    Gfx_EmitPrimitiveBatchList(g_SubmarineConnectorBatchContext, renderContext);
    Gfx_SetRenderStateCall(RENDER_STATE_NO_CULL, 0, (struct GfxEngineContext *)renderContext);
    Gfx_SetScenePassCall(0, (struct GfxEngineContext *)renderContext);
}
} /* extern "C" */
