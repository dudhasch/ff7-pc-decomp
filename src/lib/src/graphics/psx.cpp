// src/lib/src/graphics/psx.cpp
// Original: C:\lib\src\graphics\psx.cpp
// Contents: Emulates the PlayStation geometry unit (GTE) for the PC renderer. It converts between
//   fixed-point, integer and float values, maintains the installed matrix, geometry offsets and
//   projection settings, and transforms vectors.
/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/types/graphics.h"
#include "lib/types/stack.h"
#include "lib/codegen.h"
#include "lib/api/graphics.h"
#include "lib/api/stack.h"
#include "lib/api/mem.h"
#include "lib/api/polygon.h"
#include <math.h>
#include "lib/pfile.h"
#include "lib/crt.h"
#include "lib/enums/graphics.h"
extern "C" {


// The GTE stand-in: the code reads bytes 0..91; the double's alignment pads it to 96 bytes. It is
// the head of psx.cpp's .data, so the pointer below holds its address.
typedef char GraphicsGteStateSizeCheck[sizeof(GraphicsGteState) == 96 ? 1 : -1];
// GLOBAL: FF7 0x0090aa90
GraphicsGteState g_GteState = {1.0f, {0}, 480.0};
// clang-format off
// The GTE state pointer: every GTE getter and setter reaches g_GteState through it.
// GLOBAL: FF7 0x0090aaf0
GraphicsGteState *g_pGteState = &g_GteState;
// clang-format off
// clang-format on
// GLOBAL: FF7 0x00d8d488
unsigned char g_GfxPsxDataBuffer[1508] = {0};
unsigned char g_bssPad_00d8da6c[4] = {0};
unsigned char g_bssPad_00d8da70[2584] = {0};
// clang-format on
#pragma intrinsic(sqrt)
#pragma intrinsic(atan2)
#pragma intrinsic(cos, sin, tan, atan)

/*
 * Returns the address `offset` bytes into g_GfxPsxDataBuffer, a 1508-byte zero-initialised
 * scratch buffer. Battle (most of the 656 call sites), field, snowboard and submarine code
 * overlay their own temporary work records on it,
 * nearly always at offset 0. The offset is not checked against the buffer's size.
 */
// FUNCTION: FF7 0x00661000
int __cdecl Gfx_GetPsxScratchpadPtr(int offset)
{
    return offset + (int)&g_GfxPsxDataBuffer;
}

/*
 * Returns the address of the installed rotation matrix as an int.
 */
// FUNCTION: FF7 0x0066100d
PsxMatrix *__cdecl GetInstalledRotMatrixPtr(void)
{
    return &g_pGteState->rotMatrix;
}

/*
 * Returns the packed draw colour (BGRA bytes) of the GTE state.
 */
// FUNCTION: FF7 0x0066101a
int __cdecl GetDrawColorPacked(void)
{
    return g_pGteState->drawColorPacked;
}

/*
 * Returns the projected Z that ProjectPointHardware last stored.
 */
// FUNCTION: FF7 0x00661027
float __cdecl GetLastProjectedNdcZ(void)
{
    return g_pGteState->lastNdcZ;
}

/*
 * Returns the projected W that ProjectPointHardware last stored.
 */
// FUNCTION: FF7 0x00661034
float __cdecl GetLastProjectedNdcW(void)
{
    return g_pGteState->lastNdcW;
}

/*
 * Promotes the signed short argument to int and returns its float conversion.
 */
// FUNCTION: FF7 0x00661041
float __cdecl GteInt16ToFloat(short value)
{
    int wide;

    wide = value;
    return wide;
}

/*
 * Converts three consecutive shorts through GteInt16ToFloat into three floats.
 */
// FUNCTION: FF7 0x00661053
void __cdecl GteSVectorToFloat3(void *in, void *out)
{
    ((float *)out)[0] = GteInt16ToFloat(((short *)in)[0]);
    ((float *)out)[1] = GteInt16ToFloat(((short *)in)[1]);
    ((float *)out)[2] = GteInt16ToFloat(((short *)in)[2]);
}

/*
 * Converts a short 3-vector to three floats, each through an int temporary.
 */
// FUNCTION: FF7 0x00661098
void __cdecl GteSVectorToFloat3Fast(SVECTOR *src, GfxHomogeneousPoint *dst)
{
    int xInt;
    int yInt;
    int zInt;

    xInt = src->vx;
    dst->x = xInt;
    yInt = src->vy;
    dst->y = yInt;
    zInt = src->vz;
    dst->z = zInt;
}

/*
 * Returns value converted to float.
 */
// FUNCTION: FF7 0x006610d9
float __cdecl GteInt32ToFloat(int value)
{
    return value;
}

/*
 * Converts three consecutive ints through GteInt32ToFloat into three floats.
 */
// FUNCTION: FF7 0x006610e1
void __cdecl GteVectorToFloat3(int *in, float *out)
{
    out[0] = GteInt32ToFloat(in[0]);
    out[1] = GteInt32ToFloat(in[1]);
    out[2] = GteInt32ToFloat(in[2]);
}

/*
 * Converts a 12-bit fixed-point 3-vector to floats by dividing each component by 4096.
 */
// FUNCTION: FF7 0x00661123
void __cdecl GteFixed12VectorToFloat3(VECTOR *vec, GfxHomogeneousPoint *dst)
{
    dst->x = vec->vx / 4096.0f;
    dst->y = vec->vy / 4096.0f;
    dst->z = vec->vz / 4096.0f;
}

/*
 * Multiplies three input floats by 4096.0f and converts each to an output int.
 */
// FUNCTION: FF7 0x0066115c
void __cdecl GteFloat3ToFixed12Vector(int *out, float *in)
{
    out[0] = (int)(in[0] * 4096.0f);
    out[1] = (int)(in[1] * 4096.0f);
    out[2] = (int)(in[2] * 4096.0f);
}

/*
 * Converts three consecutive input floats to ints.
 */
// FUNCTION: FF7 0x006611a4
void __cdecl GteFloat3ToVector(int *out, float *in)
{
    out[0] = (int)in[0];
    out[1] = (int)in[1];
    out[2] = (int)in[2];
}

/*
 * Returns value divided by 1.0f as a float.
 */
// FUNCTION: FF7 0x006611da
float __cdecl GteTransUnitToFloat(int value)
{
    return value / 1.0f;
}

/*
 * Multiplies value by 1.0f and converts the result to int.
 */
// FUNCTION: FF7 0x006611e8
int __cdecl GteFloatToTransUnit(float value)
{
    return value * 1.0f;
}

/*
 * Expands nine signed-short rotation entries divided by 4096 into the leading3x3 float matrix,
 * converts three packed translation ints through GteTransUnitToFloat into indices 12..14, writes
 * indices 3, 7, 11 zero and 15 one.
 */
// FUNCTION: FF7 0x006611fb
void __cdecl GtePsxMatrixToFloat4x4(void *src, void *mm)
{
    ((float *)mm)[0] = ((PsxMatrix *)src)->m[0][0] / 4096.0f;
    ((float *)mm)[1] = ((PsxMatrix *)src)->m[0][1] / 4096.0f;
    ((float *)mm)[2] = ((PsxMatrix *)src)->m[0][2] / 4096.0f;
    ((float *)mm)[3] = 0.0f;
    ((float *)mm)[4] = ((PsxMatrix *)src)->m[1][0] / 4096.0f;
    ((float *)mm)[5] = ((PsxMatrix *)src)->m[1][1] / 4096.0f;
    ((float *)mm)[6] = ((PsxMatrix *)src)->m[1][2] / 4096.0f;
    ((float *)mm)[7] = 0.0f;
    ((float *)mm)[8] = ((PsxMatrix *)src)->m[2][0] / 4096.0f;
    ((float *)mm)[9] = ((PsxMatrix *)src)->m[2][1] / 4096.0f;
    ((float *)mm)[10] = ((PsxMatrix *)src)->m[2][2] / 4096.0f;
    ((float *)mm)[11] = 0.0f;
    ((float *)mm)[12] = GteTransUnitToFloat(((PsxMatrix *)src)->t[0]);
    ((float *)mm)[13] = GteTransUnitToFloat(((PsxMatrix *)src)->t[1]);
    ((float *)mm)[14] = GteTransUnitToFloat(((PsxMatrix *)src)->t[2]);
    ((float *)mm)[15] = 1.0f;
}

/*
 * Scales the leading3x3 float entries by 4096 and narrows to nine shorts, then converts indices
 * 12, 13, 14 through GteFloatToTransUnit into three packed translation ints. Other float
 * components are ignored.
 */
// FUNCTION: FF7 0x0066134b
void __cdecl GteFloat4x4ToPsxMatrix(PsxMatrix *dst, float *mm)
{
    dst->m[0][0] = (short)(mm[0] * 4096.0f);
    dst->m[0][1] = (short)(mm[1] * 4096.0f);
    dst->m[0][2] = (short)(mm[2] * 4096.0f);
    dst->m[1][0] = (short)(mm[4] * 4096.0f);
    dst->m[1][1] = (short)(mm[5] * 4096.0f);
    dst->m[1][2] = (short)(mm[6] * 4096.0f);
    dst->m[2][0] = (short)(mm[8] * 4096.0f);
    dst->m[2][1] = (short)(mm[9] * 4096.0f);
    dst->m[2][2] = (short)(mm[10] * 4096.0f);
    dst->t[0] = GteFloatToTransUnit(mm[12]);
    dst->t[1] = GteFloatToTransUnit(mm[13]);
    dst->t[2] = GteFloatToTransUnit(mm[14]);
}

/*
 * Expands nine signed-short rotation entries divided by 4096 into the leading3x3, converts
 * translation ints into float indices 3, 7, 11 and writes 12..14 zero and 15 one.
 */
// FUNCTION: FF7 0x00661465
void __cdecl GtePsxMatrixToFloat4x4ColVec(void *src, void *mm)
{
    ((float *)mm)[0] = ((PsxMatrix *)src)->m[0][0] / 4096.0f;
    ((float *)mm)[1] = ((PsxMatrix *)src)->m[0][1] / 4096.0f;
    ((float *)mm)[2] = ((PsxMatrix *)src)->m[0][2] / 4096.0f;
    ((float *)mm)[4] = ((PsxMatrix *)src)->m[1][0] / 4096.0f;
    ((float *)mm)[5] = ((PsxMatrix *)src)->m[1][1] / 4096.0f;
    ((float *)mm)[6] = ((PsxMatrix *)src)->m[1][2] / 4096.0f;
    ((float *)mm)[8] = ((PsxMatrix *)src)->m[2][0] / 4096.0f;
    ((float *)mm)[9] = ((PsxMatrix *)src)->m[2][1] / 4096.0f;
    ((float *)mm)[10] = ((PsxMatrix *)src)->m[2][2] / 4096.0f;
    ((float *)mm)[3] = GteTransUnitToFloat(((PsxMatrix *)src)->t[0]);
    ((float *)mm)[7] = GteTransUnitToFloat(((PsxMatrix *)src)->t[1]);
    ((float *)mm)[11] = GteTransUnitToFloat(((PsxMatrix *)src)->t[2]);
    ((float *)mm)[12] = 0.0f;
    ((float *)mm)[13] = 0.0f;
    ((float *)mm)[14] = 0.0f;
    ((float *)mm)[15] = 1.0f;
}

/*
 * Scales the leading3x3 floats by 4096 into nine narrowed shorts and converts float indices 3, 7,
 * 11 into packed translation ints.
 */
// FUNCTION: FF7 0x006615b5
void __cdecl GteFloat4x4ToPsxMatrixColVec(PsxMatrix *dst, float *mm)
{
    dst->m[0][0] = (short)(mm[0] * 4096.0f);
    dst->m[0][1] = (short)(mm[1] * 4096.0f);
    dst->m[0][2] = (short)(mm[2] * 4096.0f);
    dst->m[1][0] = (short)(mm[4] * 4096.0f);
    dst->m[1][1] = (short)(mm[5] * 4096.0f);
    dst->m[1][2] = (short)(mm[6] * 4096.0f);
    dst->m[2][0] = (short)(mm[8] * 4096.0f);
    dst->m[2][1] = (short)(mm[9] * 4096.0f);
    dst->m[2][2] = (short)(mm[10] * 4096.0f);
    dst->t[0] = GteFloatToTransUnit(mm[3]);
    dst->t[1] = GteFloatToTransUnit(mm[7]);
    dst->t[2] = GteFloatToTransUnit(mm[11]);
}

/*
 * Transposes the leading3x3 float input while scaling by 4096 into nine narrowed shorts. Converts
 * indices 12, 13, 14 into packed translation ints.
 */
// FUNCTION: FF7 0x006616cf
void __cdecl GteFloat4x4TransposedToPsxMatrix(PsxMatrix *dst, float *mm)
{
    dst->m[0][0] = (short)(mm[0] * 4096.0f);
    dst->m[0][1] = (short)(mm[4] * 4096.0f);
    dst->m[0][2] = (short)(mm[8] * 4096.0f);
    dst->m[1][0] = (short)(mm[1] * 4096.0f);
    dst->m[1][1] = (short)(mm[5] * 4096.0f);
    dst->m[1][2] = (short)(mm[9] * 4096.0f);
    dst->m[2][0] = (short)(mm[2] * 4096.0f);
    dst->m[2][1] = (short)(mm[6] * 4096.0f);
    dst->m[2][2] = (short)(mm[10] * 4096.0f);
    dst->t[0] = GteFloatToTransUnit(mm[12]);
    dst->t[1] = GteFloatToTransUnit(mm[13]);
    dst->t[2] = GteFloatToTransUnit(mm[14]);
}

/*
 * Expands the transposed nine signed-short rotation entries divided by 4096 into the float3x3,
 * converts packed translation into indices 12..14 and writes 3, 7, 11 zero and 15 one.
 */
// FUNCTION: FF7 0x006617e9
void __cdecl GtePsxMatrixToFloat4x4Transposed(void *src, void *mm)
{
    ((float *)mm)[0] = ((PsxMatrix *)src)->m[0][0] / 4096.0f;
    ((float *)mm)[1] = ((PsxMatrix *)src)->m[1][0] / 4096.0f;
    ((float *)mm)[2] = ((PsxMatrix *)src)->m[2][0] / 4096.0f;
    ((float *)mm)[3] = 0.0f;
    ((float *)mm)[4] = ((PsxMatrix *)src)->m[0][1] / 4096.0f;
    ((float *)mm)[5] = ((PsxMatrix *)src)->m[1][1] / 4096.0f;
    ((float *)mm)[6] = ((PsxMatrix *)src)->m[2][1] / 4096.0f;
    ((float *)mm)[7] = 0.0f;
    ((float *)mm)[8] = ((PsxMatrix *)src)->m[0][2] / 4096.0f;
    ((float *)mm)[9] = ((PsxMatrix *)src)->m[1][2] / 4096.0f;
    ((float *)mm)[10] = ((PsxMatrix *)src)->m[2][2] / 4096.0f;
    ((float *)mm)[11] = 0.0f;
    ((float *)mm)[12] = GteTransUnitToFloat(((PsxMatrix *)src)->t[0]);
    ((float *)mm)[13] = GteTransUnitToFloat(((PsxMatrix *)src)->t[1]);
    ((float *)mm)[14] = GteTransUnitToFloat(((PsxMatrix *)src)->t[2]);
    ((float *)mm)[15] = 1.0f;
}

/*
 * Clears the 30-byte matrix, then sets the three diagonal entries to 0x1000 (1.0): an identity
 * rotation with zero translation.
 */
// FUNCTION: FF7 0x00661939
void __cdecl SetIdentityPsxMatrix(PsxMatrix *dst)
{
    ZeroFillMemory32(30, dst);
    dst->m[0][0] = 0x1000;
    dst->m[1][1] = 0x1000;
    dst->m[2][2] = 0x1000;
}

/*
 * Selects the matrix layout the GTE helpers convert to: nonzero for column vectors, zero for row
 * vectors.
 */
// FUNCTION: FF7 0x00661966
void __cdecl SetMatrixLayoutMode(int mode)
{
    g_pGteState->matrixLayoutMode = mode;
}

// Sets the GTE geometry offset, which is the screen centre, in both integer and float form. The
// integer form is geomOffsetX and geomOffsetY, and the float form is screenOffsetX and
// screenOffsetY.
// FUNCTION: FF7 0x00661976
void __cdecl SetGeomOffset(int offsetX, int offsetY)
{
    g_pGteState->geomOffsetX = offsetX;
    g_pGteState->geomOffsetY = offsetY;
    g_pGteState->screenOffsetX = offsetX;
    g_pGteState->screenOffsetY = offsetY;
}

/*
 * Stores d as the GTE state's projection distance.
 */
// FUNCTION: FF7 0x006619aa
void __cdecl SetGeomScreenReference(double distance)
{
    g_pGteState->projectionDistance = distance;
}

/*
 * Returns width divided by twice tan of half DegreesToRadians(fov narrowed to float).
 */
// FUNCTION: FF7 0x006619c0
double __cdecl FovToProjectionDistance(double width, double fov)
{
    return width / (tan(DegreesToRadians((float)fov) / 2.0) * 2.0);
}

/*
 * Returns RadiansToDegrees of twice atan(width/(2*distance)), narrowed to float for the call.
 */
// FUNCTION: FF7 0x006619ee
double __cdecl ProjectionDistanceToFov(double width, double distance)
{
    return RadiansToDegrees((float)(atan(width / (2.0 * distance)) * 2.0));
}

/*
 * Stores distance as global float. For nonzero distance and nonnull context computes field-of-view
 * from the global double/float and resets perspective using context values. Otherwise replaces the
 * stored distance with 1.0; no context perspective update occurs on that fallback.
 */
// FUNCTION: FF7 0x00661a1c
void __cdecl SetGeomScreen(int distance)
{
    struct {
        struct GfxEngineContext *ctx;
        int fallback;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    g_pGteState->projectionScale = distance;
    frame.fallback = 1;
    if (distance != 0) {
        frame.ctx = GetEngineContext();
        if (frame.ctx != 0) {
            frame.ctx->fov = ProjectionDistanceToFov(g_pGteState->projectionDistance, g_pGteState->projectionScale);
            SetPerspective(frame.ctx->fov, frame.ctx->nearPlane, frame.ctx->farPlane, frame.ctx->offsetX, frame.ctx->offsetY, (float)frame.ctx->viewSizeX, (float)frame.ctx->viewSizeY, frame.ctx);
            frame.fallback = 0;
        }
    }
    if (frame.fallback != 0) {
        g_pGteState->projectionScale = 1.0f;
    }
}

/*
 * Writes the integer geometry offset (geomOffsetX, geomOffsetY) through outX and outY.
 */
// FUNCTION: FF7 0x00661af1
void __cdecl GetGeomOffset(void *outX, void *outY)
{
    *(int *)outX = g_pGteState->geomOffsetX;
    *(int *)outY = g_pGteState->geomOffsetY;
}

/*
 * Returns the projection scale (the screen distance) converted to int.
 */
// FUNCTION: FF7 0x00661b12
int __cdecl GetGeomScreen(void)
{
    return g_pGteState->projectionScale;
}

/*
 * For nonnull engine context stores both integer offsets in the GTE state and, converted to float,
 * in the context's offsetX and offsetY. Does nothing for null context.
 */
// FUNCTION: FF7 0x00661b23
void __cdecl SetProjectionOffset(int offsetX, int offsetY)
{
    GfxEngineContext *ctx;

    ctx = GetEngineContext();
    if (ctx != 0) {
        g_pGteState->projOffsetX = offsetX;
        g_pGteState->projOffsetY = offsetY;
        ctx->offsetX = offsetX;
        ctx->offsetY = offsetY;
    }
}

/*
 * For nonnull context sets the two projection offsets and rebuilds perspective with the current
 * context fields and integer dimensions converted to float. Does nothing for null context.
 */
// FUNCTION: FF7 0x00661b68
void __cdecl SetProjectionOffsetAndRebuild(int x, int y)
{
    GfxEngineContext *ctx = GetEngineContext();
    if (ctx != 0) {
        SetProjectionOffset(x, y);
        SetPerspective(ctx->fov, ctx->nearPlane, ctx->farPlane, ctx->offsetX, ctx->offsetY, (float)ctx->viewSizeX, (float)ctx->viewSizeY, ctx);
    }
}

/*
 * Writes the projection offset (projOffsetX, projOffsetY) through outX and outY.
 */
// FUNCTION: FF7 0x00661be6
void __cdecl GetProjectionOffset(int *outX, int *outY)
{
    *outX = g_pGteState->projOffsetX;
    *outY = g_pGteState->projOffsetY;
}

/*
 * Converts three input shorts to floats, converts the packed matrix using column or row layout
 * according to matrixLayoutMode, calls its matching rotate-vector helper and narrows the three float
 * results to output ints. Returns output.
 */
// FUNCTION: FF7 0x00661c07
int __cdecl ApplyMatrixToSVectorInt32(void *matrix, SVECTOR *input, void *output)
{
    struct {
        float vector[3];
        float matrix[16];
        float result[3];
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    GteSVectorToFloat3Fast(input, (GfxHomogeneousPoint *)frame.vector);
    if (g_pGteState->matrixLayoutMode != 0) {
        GtePsxMatrixToFloat4x4ColVec((PsxMatrix *)matrix, frame.matrix);
        RotateVector3ColVec(frame.matrix, frame.vector, frame.result);
    } else {
        GtePsxMatrixToFloat4x4((PsxMatrix *)matrix, frame.matrix);
        RotateVector3RowVec(frame.matrix, frame.vector, frame.result);
    }
    ((int *)output)[0] = (int)frame.result[0];
    ((int *)output)[1] = (int)frame.result[1];
    ((int *)output)[2] = (int)frame.result[2];
    return (int)output;
}

/*
 * Converts three input ints to floats, converts the packed matrix using global-selected column or
 * row layout, invokes the matching rotate-vector helper and converts results to three output ints.
 * Returns output.
 */
// FUNCTION: FF7 0x00661ca3
int __cdecl TransformVectorToInt32(PsxMatrix *matrix, void *input, void *output)
{
    struct {
        float vector[3];
        float matrix[16];
        float result[3];
    } frame;
    GteVectorToFloat3((int *)input, frame.vector);
    if (g_pGteState->matrixLayoutMode != 0) {
        GtePsxMatrixToFloat4x4ColVec(matrix, frame.matrix);
        RotateVector3ColVec(frame.matrix, frame.vector, frame.result);
    } else {
        GtePsxMatrixToFloat4x4(matrix, frame.matrix);
        RotateVector3RowVec(frame.matrix, frame.vector, frame.result);
    }
    ((int *)output)[0] = (int)frame.result[0];
    ((int *)output)[1] = (int)frame.result[1];
    ((int *)output)[2] = (int)frame.result[2];
    return (int)output;
}

/*
 * Converts three input shorts to float, converts matrix with global-selected column or row layout,
 * rotates through the matching helper and narrows results to three output shorts without
 * saturation. Returns output.
 */
// FUNCTION: FF7 0x00661d3f
int __cdecl TransformVectorToInt16(void *matrix, void *input, void *output)
{
    struct {
        float vector[3];
        float matrix[16];
        float result[3];
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    GteSVectorToFloat3Fast((SVECTOR *)input, (GfxHomogeneousPoint *)frame.vector);
    if (g_pGteState->matrixLayoutMode != 0) {
        GtePsxMatrixToFloat4x4ColVec(matrix, frame.matrix);
        RotateVector3ColVec(frame.matrix, frame.vector, frame.result);
    } else {
        GtePsxMatrixToFloat4x4(matrix, frame.matrix);
        RotateVector3RowVec(frame.matrix, frame.vector, frame.result);
    }
    ((short *)output)[0] = (short)frame.result[0];
    ((short *)output)[1] = (short)frame.result[1];
    ((short *)output)[2] = (short)frame.result[2];
    return (int)output;
}

/*
 * Converts the input short vector to float and rotates it using the installed packed matrix with
 * the layout selected by matrixLayoutMode. Writes three float results narrowed to int and
 * returns output.
 */
// FUNCTION: FF7 0x00661dde
int __cdecl RotateSVectorByInstalledMatrix(void *input, void *output)
{
    struct {
        float vector[3];
        float matrix[16];
        float result[3];
    } frame;
    GteSVectorToFloat3((short *)input, frame.vector);
    if (g_pGteState->matrixLayoutMode != 0) {
        GtePsxMatrixToFloat4x4ColVec(&g_pGteState->rotMatrix, frame.matrix);
        RotateVector3ColVec(frame.matrix, frame.vector, frame.result);
    } else {
        GtePsxMatrixToFloat4x4(&g_pGteState->rotMatrix, frame.matrix);
        RotateVector3RowVec(frame.matrix, frame.vector, frame.result);
    }
    ((int *)output)[0] = (int)frame.result[0];
    ((int *)output)[1] = (int)frame.result[1];
    ((int *)output)[2] = (int)frame.result[2];
    return (int)output;
}

/*
 * Converts both packed matrices to float with global-selected column or row layout, multiplies the
 * full4x4 and converts result back into dst with the corresponding conversion. Returns dst.
 */
// FUNCTION: FF7 0x00661e85
int __cdecl MultiplyMatrix(void *srcA, void *srcB, void *dst)
{
    struct {
        float first[4][4];
        float second[4][4];
        float result[4][4];
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    if (g_pGteState->matrixLayoutMode != 0) {
        GtePsxMatrixToFloat4x4ColVec((PsxMatrix *)srcA, frame.first);
        GtePsxMatrixToFloat4x4ColVec((PsxMatrix *)srcB, frame.second);
        MultiplyMatrix4x4Full(frame.first, frame.second, frame.result);
        GteFloat4x4ToPsxMatrixColVec((PsxMatrix *)dst, (float *)frame.result);
    } else {
        GtePsxMatrixToFloat4x4((PsxMatrix *)srcA, frame.first);
        GtePsxMatrixToFloat4x4((PsxMatrix *)srcB, frame.second);
        MultiplyMatrix4x4Full(frame.first, frame.second, frame.result);
        GteFloat4x4ToPsxMatrix((PsxMatrix *)dst, (float *)frame.result);
    }
    return (int)dst;
}

/*
 * Converts both matrices in the global-selected layout, clears result translation/W and multiplies
 * the rotation core. Copies matrix translation ints directly as float components before converting
 * result back into matrix and returning it. Translation passes through float/unit conversion rather
 * than raw byte preservation.
 */
// FUNCTION: FF7 0x00661f36
int __cdecl ConcatMatrixIntoFirst(void *matrix, void *src)
{
    struct {
        float first[4][4];
        float second[4][4];
        float result[4][4];
    } frame;

    if (g_pGteState->matrixLayoutMode != 0) {
        GtePsxMatrixToFloat4x4ColVec((PsxMatrix *)matrix, frame.first);
        GtePsxMatrixToFloat4x4ColVec((PsxMatrix *)src, frame.second);
        ClearMatrixTranslationAndW(frame.result);
        MultiplyMatrix4x4Core(frame.first, frame.second, frame.result);
        frame.result[0][3] = (float)((PsxMatrix *)matrix)->t[0];
        frame.result[1][3] = (float)((PsxMatrix *)matrix)->t[1];
        frame.result[2][3] = (float)((PsxMatrix *)matrix)->t[2];
        GteFloat4x4ToPsxMatrixColVec((PsxMatrix *)matrix, (float *)frame.result);
    } else {
        GtePsxMatrixToFloat4x4((PsxMatrix *)matrix, frame.first);
        GtePsxMatrixToFloat4x4((PsxMatrix *)src, frame.second);
        ClearMatrixTranslationAndW(frame.result);
        MultiplyMatrix4x4Core(frame.first, frame.second, frame.result);
        frame.result[3][0] = (float)((PsxMatrix *)matrix)->t[0];
        frame.result[3][1] = (float)((PsxMatrix *)matrix)->t[1];
        frame.result[3][2] = (float)((PsxMatrix *)matrix)->t[2];
        GteFloat4x4ToPsxMatrix((PsxMatrix *)matrix, (float *)frame.result);
    }
    return (int)matrix;
}

/*
 * Converts srcA/srcB, multiplies their rotation core in global-selected layout and uses existing
 * dst translation ints converted directly to float as result translation. Converts back into dst
 * and returns it; exact translation preservation depends on float/unit conversion.
 */
// FUNCTION: FF7 0x00662035
int __cdecl ConcatMatrixPreservingTranslation(void *srcA, void *srcB, void *dst)
{
    struct {
        float first[4][4];
        float second[4][4];
        float result[4][4];
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    if (g_pGteState->matrixLayoutMode != 0) {
        GtePsxMatrixToFloat4x4ColVec((PsxMatrix *)srcA, frame.first);
        GtePsxMatrixToFloat4x4ColVec((PsxMatrix *)srcB, frame.second);
        ClearMatrixTranslationAndW(frame.result);
        MultiplyMatrix4x4Core(frame.first, frame.second, frame.result);
        frame.result[0][3] = (float)((PsxMatrix *)dst)->t[0];
        frame.result[1][3] = (float)((PsxMatrix *)dst)->t[1];
        frame.result[2][3] = (float)((PsxMatrix *)dst)->t[2];
        GteFloat4x4ToPsxMatrixColVec((PsxMatrix *)dst, (float *)frame.result);
    } else {
        GtePsxMatrixToFloat4x4((PsxMatrix *)srcA, frame.first);
        GtePsxMatrixToFloat4x4((PsxMatrix *)srcB, frame.second);
        ClearMatrixTranslationAndW(frame.result);
        MultiplyMatrix4x4Core(frame.first, frame.second, frame.result);
        frame.result[3][0] = (float)((PsxMatrix *)dst)->t[0];
        frame.result[3][1] = (float)((PsxMatrix *)dst)->t[1];
        frame.result[3][2] = (float)((PsxMatrix *)dst)->t[2];
        GteFloat4x4ToPsxMatrix((PsxMatrix *)dst, (float *)frame.result);
    }
    return (int)dst;
}

/*
 * Converts src/matrix and multiplies the rotation core using selected layout. Supplies matrix
 * existing translation ints directly as float translation, converts result back into matrix and
 * returns it. Translation is subject to float/unit conversion.
 */
// FUNCTION: FF7 0x00662134
int __cdecl ConcatMatrixInPlace(void *src, void *matrix)
{
    struct {
        float first[4][4];
        float second[4][4];
        float result[4][4];
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    if (g_pGteState->matrixLayoutMode != 0) {
        GtePsxMatrixToFloat4x4ColVec((PsxMatrix *)src, frame.first);
        GtePsxMatrixToFloat4x4ColVec((PsxMatrix *)matrix, frame.second);
        ClearMatrixTranslationAndW(frame.result);
        MultiplyMatrix4x4Core(frame.first, frame.second, frame.result);
        frame.result[0][3] = (float)((PsxMatrix *)matrix)->t[0];
        frame.result[1][3] = (float)((PsxMatrix *)matrix)->t[1];
        frame.result[2][3] = (float)((PsxMatrix *)matrix)->t[2];
        GteFloat4x4ToPsxMatrixColVec((PsxMatrix *)matrix, (float *)frame.result);
    } else {
        GtePsxMatrixToFloat4x4((PsxMatrix *)src, frame.first);
        GtePsxMatrixToFloat4x4((PsxMatrix *)matrix, frame.second);
        ClearMatrixTranslationAndW(frame.result);
        MultiplyMatrix4x4Core(frame.first, frame.second, frame.result);
        frame.result[3][0] = (float)((PsxMatrix *)matrix)->t[0];
        frame.result[3][1] = (float)((PsxMatrix *)matrix)->t[1];
        frame.result[3][2] = (float)((PsxMatrix *)matrix)->t[2];
        GteFloat4x4ToPsxMatrix((PsxMatrix *)matrix, (float *)frame.result);
    }
    return (int)matrix;
}

/*
 * Converts the installed matrix and matrix using selected layout, multiplies their rotation core and
 * supplies matrix translation ints directly as floats. Converts back into matrix and returns it;
 * translation is subject to float/unit conversion.
 */
// FUNCTION: FF7 0x00662233
int __cdecl PreMultiplyByInstalledMatrix(PsxMatrix *matrix)
{
    struct {
        float first[4][4];
        float second[4][4];
        float result[4][4];
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    if (g_pGteState->matrixLayoutMode != 0) {
        GtePsxMatrixToFloat4x4ColVec(&g_pGteState->rotMatrix, frame.first);
        GtePsxMatrixToFloat4x4ColVec(matrix, frame.second);
        ClearMatrixTranslationAndW(frame.result);
        MultiplyMatrix4x4Core(frame.first, frame.second, frame.result);
        frame.result[0][3] = (float)matrix->t[0];
        frame.result[1][3] = (float)matrix->t[1];
        frame.result[2][3] = (float)matrix->t[2];
        GteFloat4x4ToPsxMatrixColVec(matrix, (float *)frame.result);
    } else {
        GtePsxMatrixToFloat4x4(&g_pGteState->rotMatrix, frame.first);
        GtePsxMatrixToFloat4x4(matrix, frame.second);
        ClearMatrixTranslationAndW(frame.result);
        MultiplyMatrix4x4Core(frame.first, frame.second, frame.result);
        frame.result[3][0] = (float)matrix->t[0];
        frame.result[3][1] = (float)matrix->t[1];
        frame.result[3][2] = (float)matrix->t[2];
        GteFloat4x4ToPsxMatrix(matrix, (float *)frame.result);
    }
    return (int)matrix;
}

/*
 * Converts installed matrix and src in selected layout, multiplies their rotation core and
 * supplies existing dst translation ints directly as floats. Converts back into dst and returns
 * it; translation is subject to float/unit conversion.
 */
// FUNCTION: FF7 0x0066233d
int __cdecl MultiplyByInstalledMatrixInto(PsxMatrix *src, PsxMatrix *dst)
{
    struct {
        float first[4][4];
        float second[4][4];
        float result[4][4];
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    if (g_pGteState->matrixLayoutMode != 0) {
        GtePsxMatrixToFloat4x4ColVec(&g_pGteState->rotMatrix, frame.first);
        GtePsxMatrixToFloat4x4ColVec(src, frame.second);
        ClearMatrixTranslationAndW(frame.result);
        MultiplyMatrix4x4Core(frame.first, frame.second, frame.result);
        frame.result[0][3] = (float)dst->t[0];
        frame.result[1][3] = (float)dst->t[1];
        frame.result[2][3] = (float)dst->t[2];
        GteFloat4x4ToPsxMatrixColVec(dst, (float *)frame.result);
    } else {
        GtePsxMatrixToFloat4x4(&g_pGteState->rotMatrix, frame.first);
        GtePsxMatrixToFloat4x4(src, frame.second);
        ClearMatrixTranslationAndW(frame.result);
        MultiplyMatrix4x4Core(frame.first, frame.second, frame.result);
        frame.result[3][0] = (float)dst->t[0];
        frame.result[3][1] = (float)dst->t[1];
        frame.result[3][2] = (float)dst->t[2];
        GteFloat4x4ToPsxMatrix(dst, (float *)frame.result);
    }
    return (int)dst;
}

/*
 * For nonnull context pops its matrix stack (the context's `renderResourceTable` member, a
 * MatrixStack of PsxMatrix pointers) and, if the returned pointer is nonzero, copies the full
 * 30-byte record to the installed global matrix. Null context or empty return leaves the
 * installed record unchanged.
 */
// FUNCTION: FF7 0x00662447
void __cdecl PopRotMatrix(void)
{
    struct {
        int returned;
        PsxMatrix *pointer;
        struct GfxEngineContext *ctx;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    frame.ctx = GetEngineContext();
    if (frame.ctx != 0) {

        frame.returned = MatrixStack_Pop((MatrixStack *)frame.ctx->renderResourceTable);
        frame.pointer = (PsxMatrix *)frame.returned;
        if (frame.pointer != 0) {
            g_pGteState->rotMatrix = *frame.pointer;
        }
    }
}

/*
 * For nonnull context pushes the matrix stack, obtains its top and copies the installed 30-byte
 * matrix there if the returned pointer is nonzero.
 */
// FUNCTION: FF7 0x00662499
void __cdecl PushRotMatrix(void)
{
    struct {
        int returned;
        PsxMatrix *pointer;
        struct GfxEngineContext *ctx;
    } frame;
    frame.ctx = GetEngineContext();
    if (frame.ctx != 0) {
        MatrixStack_Push((MatrixStack *)frame.ctx->renderResourceTable);
        frame.returned = MatrixStack_Top((MatrixStack *)frame.ctx->renderResourceTable);
        frame.pointer = (PsxMatrix *)frame.returned;
        if (frame.pointer != 0) {
            *frame.pointer = g_pGteState->rotMatrix;
        }
    }
}

/*
 * Fixed-point cosine in the PlayStation's units: `angle` is in 1/4096 turns (4096 = 360 degrees,
 * so angle * 0.087890625 is degrees) and the result is scaled by 4096 (4096 = 1.0), truncated
 * toward zero. It computes the value with the C library's cos() in double precision on the angle
 * converted to radians. Battle and the other ported code use it for rotations and positions (423
 * call sites).
 */
// FUNCTION: FF7 0x006624fd
int __cdecl CosFixed(int angle)
{
    return (int)(cos(DegreesToRadians(angle * 0.087890625f)) * 4096.0);
}

/*
 * Calls CosFixed(value) and discards its result.
 */
// FUNCTION: FF7 0x00662527
void __cdecl CosFixedAlias(int value)
{
    CosFixed(value);
}

/*
 * Fixed-point sine in the PlayStation's units: `angle` is in 1/4096 turns (4096 = 360 degrees,
 * so angle * 0.087890625 is degrees) and the result is scaled by 4096 (4096 = 1.0), truncated
 * toward zero. It computes the value with the C library's sin() in double precision on the angle
 * converted to radians. Battle and the other ported code use it for rotations and positions (688
 * call sites).
 */
// FUNCTION: FF7 0x00662538
int __cdecl SinFixed(int angle)
{
    return (int)(sin(DegreesToRadians(angle * 0.087890625f)) * 4096.0);
}

/*
 * Calls SinFixed(value) and discards its result.
 */
// FUNCTION: FF7 0x00662562
void __cdecl SinFixedAlias(int value)
{
    SinFixed(value);
}

/*
 * Computes atan2 of integer y/x as double, narrows radians to float, converts to degrees,
 * multiplies by 11.377778053283691f and converts to int. Returns that fixed-angle value.
 */
// FUNCTION: FF7 0x00662573
int __cdecl ArcTangentFixed(int y, int x)
{
    struct {
        float radiansFloat;
        int result;
        double radians;
    } local; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    local.result = (int)(RadiansToDegrees((float)(local.radiansFloat = local.radians = atan2((double)y, (double)x))) * 11.377778053283691f);
    return local.result;
}

/*
 * Builds X rotation from angle times 0.087890625f. In column mode multiplies rotation by existing
 * matrix; in row mode multiplies existing matrix by rotation. Replaces translation with existing
 * packed ints passed through float and converts back into matrix, returning it.
 */
// FUNCTION: FF7 0x006625ab
int __cdecl RotateMatrixX(int angle, void *matrix)
{
    struct {
        float first[4][4];
        float rotation[4][4];
        float result[4][4];
    } frame;
    if (g_pGteState->matrixLayoutMode != 0) {
        GtePsxMatrixToFloat4x4ColVec((PsxMatrix *)matrix, frame.first);
        BuildRotationXColVec(angle * 0.087890625f, (GfxMatrix4x4 *)frame.rotation);
        MultiplyMatrix4x4Core(frame.rotation, frame.first, frame.result);
        ClearMatrixTranslationAndW(frame.result);
        frame.result[0][3] = (float)((PsxMatrix *)matrix)->t[0];
        frame.result[1][3] = (float)((PsxMatrix *)matrix)->t[1];
        frame.result[2][3] = (float)((PsxMatrix *)matrix)->t[2];
        GteFloat4x4ToPsxMatrixColVec((PsxMatrix *)matrix, (float *)frame.result);
    } else {
        GtePsxMatrixToFloat4x4((PsxMatrix *)matrix, frame.first);
        BuildRotationXRowVec(angle * 0.087890625f, frame.rotation);
        MultiplyMatrix4x4Core(frame.first, frame.rotation, frame.result);
        ClearMatrixTranslationAndW(frame.result);
        frame.result[3][0] = (float)((PsxMatrix *)matrix)->t[0];
        frame.result[3][1] = (float)((PsxMatrix *)matrix)->t[1];
        frame.result[3][2] = (float)((PsxMatrix *)matrix)->t[2];
        GteFloat4x4ToPsxMatrix((PsxMatrix *)matrix, (float *)frame.result);
    }
    return (int)matrix;
}

/*
 * Builds Y rotation from angle times 0.087890625f. Column mode multiplies rotation by existing
 * matrix; row mode existing matrix by rotation. Converts existing translation ints through float
 * in the result and writes the packed matrix back, returning it.
 */
// FUNCTION: FF7 0x006626bc
int __cdecl RotateMatrixY(int angle, void *matrix)
{
    struct {
        float first[4][4];
        float rotation[4][4];
        float result[4][4];
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    if (g_pGteState->matrixLayoutMode != 0) {
        GtePsxMatrixToFloat4x4ColVec((PsxMatrix *)matrix, frame.first);
        BuildRotationYColVec(angle * 0.087890625f, (GfxMatrix4x4 *)frame.rotation);
        MultiplyMatrix4x4Core(frame.rotation, frame.first, frame.result);
        ClearMatrixTranslationAndW(frame.result);
        frame.result[0][3] = (float)((PsxMatrix *)matrix)->t[0];
        frame.result[1][3] = (float)((PsxMatrix *)matrix)->t[1];
        frame.result[2][3] = (float)((PsxMatrix *)matrix)->t[2];
        GteFloat4x4ToPsxMatrixColVec((PsxMatrix *)matrix, (float *)frame.result);
    } else {
        GtePsxMatrixToFloat4x4((PsxMatrix *)matrix, frame.first);
        BuildRotationYRowVec(angle * 0.087890625f, frame.rotation);
        MultiplyMatrix4x4Core(frame.first, frame.rotation, frame.result);
        ClearMatrixTranslationAndW(frame.result);
        frame.result[3][0] = (float)((PsxMatrix *)matrix)->t[0];
        frame.result[3][1] = (float)((PsxMatrix *)matrix)->t[1];
        frame.result[3][2] = (float)((PsxMatrix *)matrix)->t[2];
        GteFloat4x4ToPsxMatrix((PsxMatrix *)matrix, (float *)frame.result);
    }
    return (int)matrix;
}

/*
 * Builds Z rotation from angle times 0.087890625f. Column mode multiplies rotation by existing
 * matrix; row mode existing matrix by rotation. Copies translation through float conversion and
 * writes the packed matrix back, returning it.
 */
// FUNCTION: FF7 0x006627cd
int __cdecl RotateMatrixZ(int angle, void *matrix)
{
    struct {
        float first[4][4];
        float rotation[4][4];
        float result[4][4];
    } frame;
    if (g_pGteState->matrixLayoutMode != 0) {
        GtePsxMatrixToFloat4x4ColVec((PsxMatrix *)matrix, frame.first);
        BuildRotationZColVec(angle * 0.087890625f, (GfxMatrix4x4 *)frame.rotation);
        MultiplyMatrix4x4Core(frame.rotation, frame.first, frame.result);
        ClearMatrixTranslationAndW(frame.result);
        frame.result[0][3] = (float)((PsxMatrix *)matrix)->t[0];
        frame.result[1][3] = (float)((PsxMatrix *)matrix)->t[1];
        frame.result[2][3] = (float)((PsxMatrix *)matrix)->t[2];
        GteFloat4x4ToPsxMatrixColVec((PsxMatrix *)matrix, (float *)frame.result);
    } else {
        GtePsxMatrixToFloat4x4((PsxMatrix *)matrix, frame.first);
        BuildRotationZRowVec(angle * 0.087890625f, frame.rotation);
        MultiplyMatrix4x4Core(frame.first, frame.rotation, frame.result);
        ClearMatrixTranslationAndW(frame.result);
        frame.result[3][0] = (float)((PsxMatrix *)matrix)->t[0];
        frame.result[3][1] = (float)((PsxMatrix *)matrix)->t[1];
        frame.result[3][2] = (float)((PsxMatrix *)matrix)->t[2];
        GteFloat4x4ToPsxMatrix((PsxMatrix *)matrix, (float *)frame.result);
    }
    return (int)matrix;
}

/*
 * Builds X, Y, Z rotations from three input shorts times 0.087890625f using global-selected
 * layout, composes X times Y then times Z, clears translation/W and inserts existing destination
 * translation ints as floats. Converts back to destination and returns it; translation is subject
 * to float/unit conversion.
 */
// FUNCTION: FF7 0x006628de
int __cdecl RotMatrixXYZ(void *angles, void *dst)
{
    float myr[4][4];
    float mres[4][4];
    float mzr[4][4];
    float mxr[4][4];
    float mt[4][4];

    if (g_pGteState->matrixLayoutMode != 0) {
        BuildRotationXColVec(((SVECTOR *)angles)->vx * 0.087890625f, (GfxMatrix4x4 *)mxr);
        BuildRotationYColVec(((SVECTOR *)angles)->vy * 0.087890625f, (GfxMatrix4x4 *)myr);
        BuildRotationZColVec(((SVECTOR *)angles)->vz * 0.087890625f, (GfxMatrix4x4 *)mzr);
        MultiplyMatrix4x4Core(mxr, myr, mt);
        MultiplyMatrix4x4Core(mt, mzr, mres);
        ClearMatrixTranslationAndW(mres);
        mres[0][3] = ((PsxMatrix *)dst)->t[0];
        mres[1][3] = ((PsxMatrix *)dst)->t[1];
        mres[2][3] = ((PsxMatrix *)dst)->t[2];
        GteFloat4x4ToPsxMatrixColVec((PsxMatrix *)dst, (float *)mres);
    } else {
        BuildRotationXRowVec(((SVECTOR *)angles)->vx * 0.087890625f, mxr);
        BuildRotationYRowVec(((SVECTOR *)angles)->vy * 0.087890625f, myr);
        BuildRotationZRowVec(((SVECTOR *)angles)->vz * 0.087890625f, mzr);
        MultiplyMatrix4x4Core(mxr, myr, mt);
        MultiplyMatrix4x4Core(mt, mzr, mres);
        ClearMatrixTranslationAndW(mres);
        mres[3][0] = ((PsxMatrix *)dst)->t[0];
        mres[3][1] = ((PsxMatrix *)dst)->t[1];
        mres[3][2] = ((PsxMatrix *)dst)->t[2];
        GteFloat4x4ToPsxMatrix((PsxMatrix *)dst, (float *)mres);
    }
    return (int)dst;
}

/*
 * Builds X, Y, Z rotations from three input shorts times 0.087890625f in selected layout, composes
 * Y times X then times Z, inserts existing destination translation ints as floats after clearing
 * translation/W, converts back and returns destination.
 */
// FUNCTION: FF7 0x00662ad8
int __cdecl RotMatrix(void *angles, void *dst)
{
    float myr[4][4];
    float mres[4][4];
    float mzr[4][4];
    float mxr[4][4];
    float mt[4][4];

    if (g_pGteState->matrixLayoutMode != 0) {
        BuildRotationXColVec(((SVECTOR *)angles)->vx * 0.087890625f, (GfxMatrix4x4 *)mxr);
        BuildRotationYColVec(((SVECTOR *)angles)->vy * 0.087890625f, (GfxMatrix4x4 *)myr);
        BuildRotationZColVec(((SVECTOR *)angles)->vz * 0.087890625f, (GfxMatrix4x4 *)mzr);
        MultiplyMatrix4x4Core(myr, mxr, mt);
        MultiplyMatrix4x4Core(mt, mzr, mres);
        ClearMatrixTranslationAndW(mres);
        mres[0][3] = ((PsxMatrix *)dst)->t[0];
        mres[1][3] = ((PsxMatrix *)dst)->t[1];
        mres[2][3] = ((PsxMatrix *)dst)->t[2];
        GteFloat4x4ToPsxMatrixColVec((PsxMatrix *)dst, (float *)mres);
    } else {
        BuildRotationXRowVec(((SVECTOR *)angles)->vx * 0.087890625f, mxr);
        BuildRotationYRowVec(((SVECTOR *)angles)->vy * 0.087890625f, myr);
        BuildRotationZRowVec(((SVECTOR *)angles)->vz * 0.087890625f, mzr);
        MultiplyMatrix4x4Core(myr, mxr, mt);
        MultiplyMatrix4x4Core(mt, mzr, mres);
        ClearMatrixTranslationAndW(mres);
        mres[3][0] = ((PsxMatrix *)dst)->t[0];
        mres[3][1] = ((PsxMatrix *)dst)->t[1];
        mres[3][2] = ((PsxMatrix *)dst)->t[2];
        GteFloat4x4ToPsxMatrix((PsxMatrix *)dst, (float *)mres);
    }
    return (int)dst;
}

/*
 * Builds X, Y, Z rotations from three input shorts times 0.087890625f in selected layout, composes
 * Z times Y then times X, inserts existing destination translation ints as floats and converts
 * back to destination. Returns destination; translation is subject to conversion.
 */
// FUNCTION: FF7 0x00662cd2
int __cdecl RotMatrixZYX(SVECTOR *angles, void *dst)
{
    float myr[4][4];
    float mres[4][4];
    float mzr[4][4];
    float mxr[4][4];
    float mt[4][4];

    if (g_pGteState->matrixLayoutMode != 0) {
        BuildRotationXColVec(angles->vx * 0.087890625f, (GfxMatrix4x4 *)mxr);
        BuildRotationYColVec(angles->vy * 0.087890625f, (GfxMatrix4x4 *)myr);
        BuildRotationZColVec(angles->vz * 0.087890625f, (GfxMatrix4x4 *)mzr);
        MultiplyMatrix4x4Core(mzr, myr, mt);
        MultiplyMatrix4x4Core(mt, mxr, mres);
        ClearMatrixTranslationAndW(mres);
        mres[0][3] = ((PsxMatrix *)dst)->t[0];
        mres[1][3] = ((PsxMatrix *)dst)->t[1];
        mres[2][3] = ((PsxMatrix *)dst)->t[2];
        GteFloat4x4ToPsxMatrixColVec((PsxMatrix *)dst, (float *)mres);
    } else {
        BuildRotationXRowVec(angles->vx * 0.087890625f, mxr);
        BuildRotationYRowVec(angles->vy * 0.087890625f, myr);
        BuildRotationZRowVec(angles->vz * 0.087890625f, mzr);
        MultiplyMatrix4x4Core(mzr, myr, mt);
        MultiplyMatrix4x4Core(mt, mxr, mres);
        ClearMatrixTranslationAndW(mres);
        mres[3][0] = ((PsxMatrix *)dst)->t[0];
        mres[3][1] = ((PsxMatrix *)dst)->t[1];
        mres[3][2] = ((PsxMatrix *)dst)->t[2];
        GteFloat4x4ToPsxMatrix((PsxMatrix *)dst, (float *)mres);
    }
    return (int)dst;
}

/*
 * Rotates the input short vector through the installed matrix in global-selected layout, narrows
 * each float result to int and adds the three installed translation ints. Writes three output ints
 * and zero flag.
 */
// FUNCTION: FF7 0x00662ecc
void __cdecl RotTransPoint(void *input, void *output, void *flag)
{
    struct {
        float vector[3];
        float matrix[16];
        float result[3];
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    if (g_pGteState->matrixLayoutMode != 0) {
        GtePsxMatrixToFloat4x4ColVec(&g_pGteState->rotMatrix, frame.matrix);
        GteSVectorToFloat3Fast((SVECTOR *)input, (GfxHomogeneousPoint *)frame.vector);
        RotateVector3ColVec(frame.matrix, frame.vector, frame.result);
    } else {
        GtePsxMatrixToFloat4x4(&g_pGteState->rotMatrix, frame.matrix);
        GteSVectorToFloat3Fast((SVECTOR *)input, (GfxHomogeneousPoint *)frame.vector);
        RotateVector3RowVec(frame.matrix, frame.vector, frame.result);
    }
    ((int *)output)[0] = (int)frame.result[0] + g_pGteState->rotMatrix.t[0];
    ((int *)output)[1] = (int)frame.result[1] + g_pGteState->rotMatrix.t[1];
    ((int *)output)[2] = (int)frame.result[2] + g_pGteState->rotMatrix.t[2];
    *(int *)flag = 0;
}

/*
 * Rotates the input short vector through the installed matrix using selected layout, converts
 * results to int and adds installed translation ints, then narrows the sums to three output
 * shorts. Writes zero flag without saturation or overflow checking.
 */
// FUNCTION: FF7 0x00662fa3
void __cdecl RotTransPointToInt16(void *input, short *output, int *flag)
{
    struct {
        float vector[3];
        float matrix[16];
        float result[3];
    } frame;
    if (g_pGteState->matrixLayoutMode != 0) {
        GtePsxMatrixToFloat4x4ColVec(&g_pGteState->rotMatrix, frame.matrix);
        GteSVectorToFloat3Fast((SVECTOR *)input, (GfxHomogeneousPoint *)frame.vector);
        RotateVector3ColVec(frame.matrix, frame.vector, frame.result);
    } else {
        GtePsxMatrixToFloat4x4(&g_pGteState->rotMatrix, frame.matrix);
        GteSVectorToFloat3Fast((SVECTOR *)input, (GfxHomogeneousPoint *)frame.vector);
        RotateVector3RowVec(frame.matrix, frame.vector, frame.result);
    }
    output[0] = (int)frame.result[0] + g_pGteState->rotMatrix.t[0];
    output[1] = (int)frame.result[1] + g_pGteState->rotMatrix.t[1];
    output[2] = (int)frame.result[2] + g_pGteState->rotMatrix.t[2];
    *flag = 0;
}

/*
 * Always uses column-vector rotation, adds installed translation in float and projects x/y by
 * stored distance divided by z plus offsets when z is nonzero. Returns int(z*0.25), or zero for z
 * zero leaving output shorts unchanged. Both flags are always cleared.
 */
// FUNCTION: FF7 0x0066307d
int __cdecl RotTransPers(SVECTOR *input, short *output, int *flag1, int *flag2)
{
    struct {
        float vector[3];
        float matrix[16];
        float viewZ;
        float viewY;
        float viewX;
        int ret;
        float result[3];
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    GteSVectorToFloat3Fast(input, (GfxHomogeneousPoint *)frame.vector);
    GtePsxMatrixToFloat4x4ColVec(&g_pGteState->rotMatrix, frame.matrix);
    RotateVector3ColVec(frame.matrix, frame.vector, frame.result);
    frame.viewX = g_pGteState->rotMatrix.t[0] + frame.result[0];
    frame.viewY = g_pGteState->rotMatrix.t[1] + frame.result[1];
    frame.viewZ = g_pGteState->rotMatrix.t[2] + frame.result[2];
projected:
    if (frame.viewZ != 0.0f) {
        output[0] = (short)(frame.viewX * g_pGteState->projectionScale / frame.viewZ + g_pGteState->screenOffsetX);
        output[1] = (short)(frame.viewY * g_pGteState->projectionScale / frame.viewZ + g_pGteState->screenOffsetY);
        frame.ret = (int)(frame.viewZ * 0.25);
    } else {
        frame.ret = 0;
    }
    *flag1 = 0;
    *flag2 = 0;
    return frame.ret;
}

/*
 * Converts depth shifted two to float, unprojects x/y with stored offsets/distance and subtracts
 * packed matrix translation. Transposes the float matrix and rotates that vector into out, then
 * narrows out to three result shorts. The general inverse branch is constant false.
 */
// FUNCTION: FF7 0x0066316f
void __cdecl ScreenPointToWorld(int x, int y, int depth, int matrix, float *out, short *result)
{
    struct {
        float inverse[16];
        float vector[3];
        float matrix[16];
        float depthScaled;
        float viewY;
        float viewX;
    } frame;
    frame.depthScaled = depth << 2;
    frame.viewX = (x - g_pGteState->screenOffsetX) * frame.depthScaled / g_pGteState->projectionScale;
    frame.viewY = (y - g_pGteState->screenOffsetY) * frame.depthScaled / g_pGteState->projectionScale;
    frame.vector[0] = frame.viewX - ((PsxMatrix *)matrix)->t[0];
    frame.vector[1] = frame.viewY - ((PsxMatrix *)matrix)->t[1];
    frame.vector[2] = frame.depthScaled - ((PsxMatrix *)matrix)->t[2];
    GtePsxMatrixToFloat4x4ColVec((PsxMatrix *)matrix, frame.matrix);
    if (DEAD_IN_RETAIL) {
        Matrix4x4Invert((int)frame.matrix, frame.inverse);
        RotateVector3ColVec(frame.inverse, frame.vector, out);
    } else {
        TransposeMatrix4x4InPlace((int)frame.matrix);
        RotateVector3ColVec(frame.matrix, frame.vector, out);
    }
    result[0] = (short)out[0];
    result[1] = (short)out[1];
    result[2] = (short)out[2];
}

/*
 * Converts installed matrix with transposed layout and input shorts to floats, calls
 * ProjectPointWithMatrix and on success narrows screen coordinates to shorts, stores result
 * depth/W globals and returns int(clip z*0.25). Failure returns zero without modifying those
 * outputs. Both flags are always zeroed.
 */
// FUNCTION: FF7 0x0066327e
int __cdecl ProjectPointHardware(void *input, short *output, int *flag1, int *flag2)
{
    struct {
        float vector[3];
        float matrix[16];
        float result[4];
        int ret;
        float clip[4];
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    GtePsxMatrixToFloat4x4Transposed(&g_pGteState->rotMatrix, frame.matrix);
    GteSVectorToFloat3Fast((SVECTOR *)input, (GfxHomogeneousPoint *)frame.vector);
    if (ProjectPointWithMatrix((int)frame.matrix, (int)frame.vector, (int)frame.clip, (int)frame.result)) {
        output[0] = (short)frame.result[0];
        output[1] = (short)frame.result[1];
        g_pGteState->lastNdcZ = frame.result[2];
        g_pGteState->lastNdcW = frame.result[3];
        frame.ret = (int)(frame.clip[2] * 0.25);
    } else {
        frame.ret = 0;
    }
    *flag1 = 0;
    *flag2 = 0;
    return frame.ret;
}

/*
 * Sets the GTE state's secondary (back) colour from r, g, b, with alpha 0xFF.
 */
// FUNCTION: FF7 0x0066332e
void __cdecl SetSecondaryColorRGB(int red, int green, int blue)
{
    g_pGteState->secondaryColorR = red;
    g_pGteState->secondaryColorG = green;
    g_pGteState->secondaryColorB = blue;
    g_pGteState->secondaryColorA = 0xFF;
}

/*
 * Sets the GTE state's draw colour from r, g, b, with alpha 0xFF.
 */
// FUNCTION: FF7 0x0066335f
void __cdecl SetDrawColorRGB(int red, int green, int blue)
{
    g_pGteState->drawColorR = red;
    g_pGteState->drawColorG = green;
    g_pGteState->drawColorB = blue;
    g_pGteState->drawColorA = 0xFF;
}

/*
 * Scales the three rows of a PSX matrix by a fixed-point 3-vector: converts both to floats
 * (column-vector layout when matrixLayoutMode is set), multiplies each row's components by the
 * vector's, and converts back. Returns the matrix.
 */
// FUNCTION: FF7 0x00663390
int __cdecl ScaleMatrix(void *matrix, void *scale)
{
    GfxHomogeneousPoint sz;
    GfxMatrix4x4 zElementZero;

    if (g_pGteState->matrixLayoutMode != 0) {
        GtePsxMatrixToFloat4x4ColVec((PsxMatrix *)matrix, &zElementZero);
        GteFixed12VectorToFloat3((VECTOR *)scale, &sz);
        zElementZero.m00 *= sz.x;
        zElementZero.m10 *= sz.x;
        zElementZero.m20 *= sz.x;
        zElementZero.m01 *= sz.y;
        zElementZero.m11 *= sz.y;
        zElementZero.m21 *= sz.y;
        zElementZero.m02 *= sz.z;
        zElementZero.m12 *= sz.z;
        zElementZero.m22 *= sz.z;
        GteFloat4x4ToPsxMatrixColVec((PsxMatrix *)matrix, (float *)&zElementZero);
    } else {
        GtePsxMatrixToFloat4x4((PsxMatrix *)matrix, &zElementZero);
        GteFixed12VectorToFloat3((VECTOR *)scale, &sz);
        zElementZero.m00 *= sz.x;
        zElementZero.m10 *= sz.x;
        zElementZero.m20 *= sz.x;
        zElementZero.m01 *= sz.y;
        zElementZero.m11 *= sz.y;
        zElementZero.m21 *= sz.y;
        zElementZero.m02 *= sz.z;
        zElementZero.m12 *= sz.z;
        zElementZero.m22 *= sz.z;
        GteFloat4x4ToPsxMatrix((PsxMatrix *)matrix, (float *)&zElementZero);
    }
    return (int)matrix;
}

/*
 * Scales the three columns of a PSX matrix by a fixed-point 3-vector, the column-wise counterpart
 * of ScaleMatrix.
 */
// FUNCTION: FF7 0x006634b3
int __cdecl ScaleMatrixL(void *matrix, VECTOR *scale)
{
    GfxMatrix4x4 matZ0;
    GfxHomogeneousPoint sz;

    if (g_pGteState->matrixLayoutMode != 0) {
        GtePsxMatrixToFloat4x4ColVec((PsxMatrix *)matrix, &matZ0);
        GteFixed12VectorToFloat3(scale, &sz);
        matZ0.m00 *= sz.x;
        matZ0.m01 *= sz.x;
        matZ0.m02 *= sz.x;
        matZ0.m10 *= sz.y;
        matZ0.m11 *= sz.y;
        matZ0.m12 *= sz.y;
        matZ0.m20 *= sz.z;
        matZ0.m21 *= sz.z;
        matZ0.m22 *= sz.z;
        GteFloat4x4ToPsxMatrixColVec((PsxMatrix *)matrix, (float *)&matZ0);
    } else {
        GtePsxMatrixToFloat4x4((PsxMatrix *)matrix, &matZ0);
        GteFixed12VectorToFloat3(scale, &sz);
        matZ0.m00 *= sz.x;
        matZ0.m01 *= sz.x;
        matZ0.m02 *= sz.x;
        matZ0.m10 *= sz.y;
        matZ0.m11 *= sz.y;
        matZ0.m12 *= sz.y;
        matZ0.m20 *= sz.z;
        matZ0.m21 *= sz.z;
        matZ0.m22 *= sz.z;
        GteFloat4x4ToPsxMatrix((PsxMatrix *)matrix, (float *)&matZ0);
    }
    return (int)matrix;
}

/*
 * Copies the nine rotation entries of the installed matrix into rotMatrix; its translation is left
 * as it is.
 */
// FUNCTION: FF7 0x006635d6
void __cdecl GetRotMatrix(PsxMatrix *rotMatrix)
{
    rotMatrix->m[0][0] = g_pGteState->rotMatrix.m[0][0];
    rotMatrix->m[0][1] = g_pGteState->rotMatrix.m[0][1];
    rotMatrix->m[0][2] = g_pGteState->rotMatrix.m[0][2];
    rotMatrix->m[1][0] = g_pGteState->rotMatrix.m[1][0];
    rotMatrix->m[1][1] = g_pGteState->rotMatrix.m[1][1];
    rotMatrix->m[1][2] = g_pGteState->rotMatrix.m[1][2];
    rotMatrix->m[2][0] = g_pGteState->rotMatrix.m[2][0];
    rotMatrix->m[2][1] = g_pGteState->rotMatrix.m[2][1];
    rotMatrix->m[2][2] = g_pGteState->rotMatrix.m[2][2];
}

/*
 * Loads the rotation part of a PsxMatrix (`src`, its nine shorts) into the GTE stand-in
 * (g_pGteState->rotMatrix), the matrix that RotTransPoint and the other transform helpers then
 * apply. The translation is left as it is; SetTransMatrix loads it. The battle code and the other
 * code ported from the PlayStation call the pair before transforming points (1,326 call sites).
 */
// FUNCTION: FF7 0x00663673
void __cdecl SetRotMatrix(void *src)
{
    g_pGteState->rotMatrix.m[0][0] = ((PsxMatrix *)src)->m[0][0];
    g_pGteState->rotMatrix.m[0][1] = ((PsxMatrix *)src)->m[0][1];
    g_pGteState->rotMatrix.m[0][2] = ((PsxMatrix *)src)->m[0][2];
    g_pGteState->rotMatrix.m[1][0] = ((PsxMatrix *)src)->m[1][0];
    g_pGteState->rotMatrix.m[1][1] = ((PsxMatrix *)src)->m[1][1];
    g_pGteState->rotMatrix.m[1][2] = ((PsxMatrix *)src)->m[1][2];
    g_pGteState->rotMatrix.m[2][0] = ((PsxMatrix *)src)->m[2][0];
    g_pGteState->rotMatrix.m[2][1] = ((PsxMatrix *)src)->m[2][1];
    g_pGteState->rotMatrix.m[2][2] = ((PsxMatrix *)src)->m[2][2];
}

/*
 * Loads the translation of a PsxMatrix (`src->t`) into the GTE stand-in
 * (g_pGteState->rotMatrix.t), which RotTransPoint and the other transform helpers
 * add after rotating; the rotation is left as it is (SetRotMatrix loads it).
 */
// FUNCTION: FF7 0x00663707
void __cdecl SetTransMatrix(void *src)
{
    g_pGteState->rotMatrix.t[0] = ((PsxMatrix *)src)->t[0];
    g_pGteState->rotMatrix.t[1] = ((PsxMatrix *)src)->t[1];
    g_pGteState->rotMatrix.t[2] = ((PsxMatrix *)src)->t[2];
}

/*
 * Returns sqrt of value converted to double, converted back to int.
 */
// FUNCTION: FF7 0x00663736
int __cdecl SqrtToInt(int value)
{
    return (int)sqrt((double)value);
}

/*
 * Returns sqrt(value/4096.0f)*4096.0 converted to int.
 */
// FUNCTION: FF7 0x00663748
int __cdecl SquareRoot12(int value)
{
    return (int)(sqrt(value / 4096.0f) * 4096.0);
}

/*
 * Copies a 3-int translation into a PSX matrix's translation (t) and returns the matrix.
 */
// FUNCTION: FF7 0x00663766
int __cdecl SetMatrixTranslation(void *matrix, void *translation)
{
    ((PsxMatrix *)matrix)->t[0] = ((VECTOR *)translation)->vx;
    ((PsxMatrix *)matrix)->t[1] = ((VECTOR *)translation)->vy;
    ((PsxMatrix *)matrix)->t[2] = ((VECTOR *)translation)->vz;
    return (int)matrix;
}

/*
 * Copies the full 30-byte input record locally and writes only its nine rotation shorts transposed
 * to output, leaving output translation untouched. Returns output; local copy supports overlapping
 * input/output.
 */
// FUNCTION: FF7 0x00663791
int __cdecl TransposeMatrix(void *input, void *output)
{
    PsxMatrix matrix = *(PsxMatrix *)input;
    ((short *)output)[0] = matrix.m[0][0];
    ((short *)output)[1] = matrix.m[1][0];
    ((short *)output)[2] = matrix.m[2][0];
    ((short *)output)[3] = matrix.m[0][1];
    ((short *)output)[4] = matrix.m[1][1];
    ((short *)output)[5] = matrix.m[2][1];
    ((short *)output)[6] = matrix.m[0][2];
    ((short *)output)[7] = matrix.m[1][2];
    ((short *)output)[8] = matrix.m[2][2];
    return (int)output;
}

/*
 * Copies the full 30-byte record locally, writes its nine rotation shorts transposed and copies all
 * three translation ints unchanged into output. Returns output; local copy supports overlap.
 */
// FUNCTION: FF7 0x00663813
int __cdecl TransposeMatrixFull(void *input, PsxMatrix *output)
{
    PsxMatrix matrix = *(PsxMatrix *)input;
    output->m[0][0] = matrix.m[0][0];
    output->m[0][1] = matrix.m[1][0];
    output->m[0][2] = matrix.m[2][0];
    output->m[1][0] = matrix.m[0][1];
    output->m[1][1] = matrix.m[1][1];
    output->m[1][2] = matrix.m[2][1];
    output->m[2][0] = matrix.m[0][2];
    output->m[2][1] = matrix.m[1][2];
    output->m[2][2] = matrix.m[2][2];
    output->t[0] = matrix.t[0];
    output->t[1] = matrix.t[1];
    output->t[2] = matrix.t[2];
    return (int)output;
}

/*
 * Converts three input ints to float, computes float squared length and square root. For positive
 * norm writes each component times 4096/norm narrowed to int, otherwise writes zeroes. Returns
 * squared length narrowed to int.
 */
// FUNCTION: FF7 0x006638b0
int __cdecl NormalizeVectorFixed(void *input, void *output)
{
    struct {
        float inputZ;
        float inputY;
        float norm;
        float inputX;
        float square;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    frame.inputX = ((VECTOR *)input)->vx;
    frame.inputY = ((VECTOR *)input)->vy;
    frame.inputZ = ((VECTOR *)input)->vz;
    frame.square = frame.inputX * frame.inputX + frame.inputY * frame.inputY + frame.inputZ * frame.inputZ;
    frame.norm = frame.square;
    frame.norm = sqrt(frame.norm);
normalized:
    if (frame.norm > 0.0f) {
        frame.norm = 4096.0f / frame.norm;
        ((VECTOR *)output)->vx = (int)(frame.inputX * frame.norm);
        ((VECTOR *)output)->vy = (int)(frame.inputY * frame.norm);
        ((VECTOR *)output)->vz = (int)(frame.inputZ * frame.norm);
    } else {
        ((VECTOR *)output)->vx = 0;
        ((VECTOR *)output)->vy = 0;
        ((VECTOR *)output)->vz = 0;
    }
    return (int)frame.square;
}

/*
 * Converts three input ints to float and computes squared length/norm. For positive norm writes
 * each component times reciprocal norm times 4096 narrowed to short, otherwise zeroes. Returns
 * squared length narrowed to int.
 */
// FUNCTION: FF7 0x00663973
int __cdecl VectorNormalToSVector(int *input, short *output)
{
    struct {
        float inputZ;
        float inputY;
        float norm;
        float inputX;
        float square;
    } frame;
    frame.inputX = input[0];
    frame.inputY = input[1];
    frame.inputZ = input[2];
    frame.square = frame.inputX * frame.inputX + frame.inputY * frame.inputY + frame.inputZ * frame.inputZ;
    frame.norm = frame.square;
    frame.norm = sqrt(frame.norm);
normalized:
    if (frame.norm > 0.0f) {
        frame.norm = 1.0f / frame.norm;
        output[0] = (short)(frame.inputX * frame.norm * 4096.0f);
        output[1] = (short)(frame.inputY * frame.norm * 4096.0f);
        output[2] = (short)(frame.inputZ * frame.norm * 4096.0f);
    } else {
        output[0] = 0;
        output[1] = 0;
        output[2] = 0;
    }
    return (int)frame.square;
}

/*
 * Converts three input shorts to float and computes squared length/norm. For positive norm writes
 * normalized components times 4096 narrowed to shorts, otherwise zeroes. Returns squared length
 * narrowed to int.
 */
// FUNCTION: FF7 0x00663a48
int __cdecl VectorNormalSVectorToSVector(short *input, short *output)
{
    struct {
        float inputZ;
        float inputY;
        float norm;
        float inputX;
        float square;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    frame.inputX = input[0];
    frame.inputY = input[1];
    frame.inputZ = input[2];
    frame.square = frame.inputX * frame.inputX + frame.inputY * frame.inputY + frame.inputZ * frame.inputZ;
    frame.norm = frame.square;
    frame.norm = sqrt(frame.norm);
normalized:
    if (frame.norm > 0.0f) {
        frame.norm = 1.0f / frame.norm;
        output[0] = (short)(frame.inputX * frame.norm * 4096.0f);
        output[1] = (short)(frame.inputY * frame.norm * 4096.0f);
        output[2] = (short)(frame.inputZ * frame.norm * 4096.0f);
    } else {
        output[0] = 0;
        output[1] = 0;
        output[2] = 0;
    }
    return (int)frame.square;
}

/*
 * Cross product of two int 3-vectors through floats, written back as ints.
 */
// FUNCTION: FF7 0x00663b32
void __cdecl CrossProductInt(VECTOR *vecA, VECTOR *vecB, void *result)
{
    GfxHomogeneousPoint vecB3;
    GfxHomogeneousPoint resultVec;
    GfxHomogeneousPoint vecA3;

    GteVectorToFloat3(&vecA->vx, &vecA3.x);
    GteVectorToFloat3(&vecB->vx, &vecB3.x);
    resultVec.x = vecA3.y * vecB3.z - vecA3.z * vecB3.y;
    resultVec.y = vecA3.z * vecB3.x - vecA3.x * vecB3.z;
    resultVec.z = vecA3.x * vecB3.y - vecA3.y * vecB3.x;
    GteFloat3ToVector((int *)result, &resultVec.x);
}

/*
 * Cross product of two 12-bit fixed-point 3-vectors through floats, written back in fixed point.
 */
// FUNCTION: FF7 0x00663b9f
void __cdecl CrossProductFixed12(VECTOR *vecA, VECTOR *vecB, VECTOR *result)
{
    GfxHomogeneousPoint rhsX;
    GfxHomogeneousPoint rxValue;
    GfxHomogeneousPoint aFieldX;

    GteFixed12VectorToFloat3(vecA, &aFieldX);
    GteFixed12VectorToFloat3(vecB, &rhsX);
    rxValue.x = aFieldX.y * rhsX.z - aFieldX.z * rhsX.y;
    rxValue.y = aFieldX.z * rhsX.x - aFieldX.x * rhsX.z;
    rxValue.z = aFieldX.x * rhsX.y - aFieldX.y * rhsX.x;
    GteFloat3ToFixed12Vector(&result->vx, &rxValue.x);
}

/*
 * Returns value unchanged.
 */
// FUNCTION: FF7 0x00663c0c
int __cdecl Gfx_PsxReturnArgument(int value)
{
    return value;
}

/*
 * Returns zero unconditionally.
 */
// FUNCTION: FF7 0x00663c14
int __cdecl Gfx_PsxReturnZero(void)
{
    return 0;
}

/*
 * Empty. A GTE hook the shipped build left with no body.
 */
// FUNCTION: FF7 0x00663c1b
void __cdecl Gfx_PsxHookNoOp_1(int flag)
{
    /* The original is empty. */
}

/*
 * Empty. A GTE hook the shipped build left with no body.
 */
// FUNCTION: FF7 0x00663c20
void __cdecl Gfx_PsxHookNoOp_2(void)
{
    /* The original is empty. */
}

/*
 * Empty. A GTE hook the shipped build left with no body.
 */
// FUNCTION: FF7 0x00663c25
void __cdecl Gfx_PsxHookNoOp_3(void)
{
    /* The original is empty. */
}

/*
 * Empty. A GTE hook the shipped build left with no body.
 */
// FUNCTION: FF7 0x00663c2a
void __cdecl Gfx_PsxHookNoOp_4(int code, int value)
{
    /* The original is empty. */
}

/*
 * Transforms a short 3-vector by the engine's world matrix (the context's cameraMatrix): rotates it
 * as a row vector, adds the matrix's translation row, and writes the result as ints to dst. The
 * flag is cleared.
 */
// FUNCTION: FF7 0x00663c2f
void __cdecl TransformPointByWorldMatrix(SVECTOR *point, int *dst, int *flag)
{
    GfxEngineContext *ctx;
    GfxHomogeneousPoint resultVec;
    float pointX[3];

    ctx = GetEngineContext();
    *flag = 0;
    GteSVectorToFloat3(&point->vx, pointX);
    RotateVector3RowVec((void *)ctx->cameraMatrix, pointX, &resultVec.x);
    resultVec.x += ((GfxWorldOffset *)(ctx->cameraMatrix))->offsetX;
    resultVec.y += ((GfxWorldOffset *)(ctx->cameraMatrix))->offsetY;
    resultVec.z += ((GfxWorldOffset *)(ctx->cameraMatrix))->offsetZ;
    GteFloat3ToVector(dst, &resultVec.x);
}

/*
 * Original fallback frees the first model before the optional second model.
 */
// FUNCTION: FF7 0x00663cba
void __cdecl Psx_FreeModelTable(GfxPsxModelTable *modelTable)
{
    int entries;
    GfxEngineContext *ctx;
    GfxModelRecord *record;
    int i;

    if (modelTable != 0) {
        if (modelTable->entries != 0) {
            ctx = GetEngineContext();
            record = modelTable->recordTable;
            entries = (int)modelTable->entries;
            for (i = 0; i < modelTable->count; i++) {
                if ((record->flags & 0x200) != 0) {
                    Gfx_SptFreeRecordAndNull(entries);
                    DestroyRenderObjectAndClear((void *)(entries + 4));
                } else {
                    Pfile_FreeModelAndNull(entries, ctx);
                    if ((record->flags & 0x1000) != 0) {
                        Pfile_FreeModelAndNull(entries + 4, ctx);
                    }
                }
                record++;
                entries += 8;
            }
            // STRING: FF7 0x0090aaf4
            GameHeapFree(modelTable->entries, "C:\\lib\\src\\graphics\\psx.cpp", 1686);
        }
        // STRING: FF7 0x0090ab10
        GameHeapFree(modelTable, "C:\\lib\\src\\graphics\\psx.cpp", 1689);
    }
}

/*
 * Frees the model table *slot points to through Psx_FreeModelTable and nulls the slot. A null slot
 * or an empty slot does nothing.
 */
// FUNCTION: FF7 0x00663db9
void __cdecl Psx_FreeModelTableAndNull(GfxPsxModelTable **slot)
{
    if (slot != 0 && *slot != 0) {
        Psx_FreeModelTable(*slot);
        *slot = 0;
    }
}

/*
 * Allocates a model table for `count` model records and loads each record's models from the
 * directory `owner`: an .SPT sprite record and its render object when the record's flag 0x200 is
 * set, otherwise one .RSD model and a second one when flag 0x1000 is set. Returns the table, or
 * zero (after freeing what was loaded) when an allocation fails.
 */
// FUNCTION: FF7 0x00663de3
GfxPsxModelTable *__cdecl Psx_LoadModelTableFromRecords(char *owner, int count, GfxModelRecord *recordTable)
{
    struct {
        int flag;
        int index;
        PfileLoadOptions *opts;
        GfxModelRecord *records;
        GfxPsxModelTableEntry *entries;
        PfileLoadOptions options;
        GfxEngineContext *context;
        int failed;
        GfxPsxModelTable *result;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    frame.failed = 1;
    // STRING: FF7 0x0090ab2c
    frame.result = (GfxPsxModelTable *)GameHeapAlloc(1, 16, "C:\\lib\\src\\graphics\\psx.cpp", 1708);
    if (frame.result != 0) {
        frame.result->directory = owner;
        frame.result->count = count;
        frame.result->recordTable = recordTable;
        // STRING: FF7 0x0090ab48
        frame.result->entries = (GfxPsxModelTableEntry *)GameHeapAlloc(frame.result->count, 8, "C:\\lib\\src\\graphics\\psx.cpp", 1715);
        if (frame.result->entries != 0) {
            frame.context = GetEngineContext();
            frame.opts = &frame.options;
            Pfile_InitLoadOptions(frame.opts);
            frame.opts->directory = owner;
            frame.records = recordTable;
            frame.entries = frame.result->entries;
            for (frame.index = 0; frame.index < frame.result->count; frame.index++) {
                frame.opts->matrixCount = 0;
                if ((frame.records->flags & 0x10) != 0) {
                    frame.opts->matrixCount = 1;
                }
                frame.opts->scale = (float)frame.records->scale;
                Pfile_SetLoadOptionsBlendMode(frame.records->flags & 7, frame.opts);
                if ((frame.records->flags & 8) != 0) {
                    frame.opts->blendFlags = 1;
                }
                if ((frame.records->flags & 0x200) != 0) {
                    frame.entries->model = Gfx_SptLoadRecord(frame.records->modelA, frame.opts);
                    if ((frame.records->flags & 0x400) != 0) {
                        frame.flag = 1;
                    } else {
                        frame.flag = 0;
                    }
                    frame.entries->secondModel = (int)Gfx_CreateRenderObject(frame.flag, GFX_DRAW_TEXTURED_QUAD_3D, frame.opts, (char *)frame.records->modelB, (void *)frame.context->renderObjectBank);
                } else {
                    frame.entries->model = LoadRsdModel(frame.opts, (const char *)frame.records->modelA, frame.context);
                    if ((frame.records->flags & 0x1000) != 0) {
                        frame.entries->secondModel = LoadRsdModel(frame.opts, (const char *)frame.records->modelB, frame.context);
                    }
                }
                frame.records++;
                frame.entries++;
            }
            frame.failed = 0;
        }
    }
    if (frame.failed != 0) {
        Psx_FreeModelTable(frame.result);
        frame.result = 0;
    }
    return frame.result;
}

/*
 * Returns the first model of entry `index` of the model table, or zero if the table or its entries
 * array is null.
 */
// FUNCTION: FF7 0x00664099
int __cdecl Psx_GetModelTablePrimary(int index, GfxPsxModelTable *owner)
{
    int result;

    result = 0;
    if (owner != 0 && owner->entries != 0) {
        result = owner->entries[index].model;
    }
    return result;
}

/*
 * Returns the second model of entry `index` of the model table, or zero if the table or its entries
 * array is null.
 */
// FUNCTION: FF7 0x006640c9
int __cdecl Psx_GetModelTableSecondary(int index, GfxPsxModelTable *owner)
{
    int result;

    result = 0;
    if (owner != 0 && owner->entries != 0) {
        result = owner->entries[index].secondModel;
    }
    return result;
}

/*
 * Returns the first model of entry `index` of the model table, or zero if the table or its entries
 * array is null.
 */
// FUNCTION: FF7 0x006640fa
int __cdecl Psx_GetModelTablePrimaryAlt(int index, GfxPsxModelTable *owner)
{
    int result;

    result = 0;
    if (owner != 0 && owner->entries != 0) {
        result = owner->entries[index].model;
    }
    return result;
}

/*
 * Returns the second model of entry `index` of the model table, or zero if the table or its entries
 * array is null.
 */
// FUNCTION: FF7 0x0066412a
int __cdecl Psx_GetModelTableSecondaryAlt(int index, GfxPsxModelTable *owner)
{
    int result;

    result = 0;
    if (owner != 0 && owner->entries != 0) {
        result = owner->entries[index].secondModel;
    }
    return result;
}
} /* extern "C" */
