// src/lib/src/graphics/dx_mat.cpp
// Original: C:\lib\src\graphics\dx_mat.cpp
// Contents: Direct3D COM release helpers, material creation, and matrix-handle management, together
//   with the matrix math library: 4x4 and 3x3 multiplication, vector and point transforms,
//   perspective divide, and degree-radian conversion. It also allocates and frees the matrix stack
//   and render-state records on the game heap.
/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/api/mem.h"
#include "lib/codegen.h"
#include "lib/types/graphics.h"
#include "lib/types/polygon.h"
#include <windows.h>
#include <unknwn.h>
#include "lib/api/graphics.h"
#include "lib/enums/graphics.h"
#include <math.h>
#include "lib/com_abi.h"

// Direct3D material and viewport COM wrapper helpers.
// Compiled as C++ so virtual __stdcall COM method dispatch matches the DirectX 5 SDK vtables.
extern "C" {

#pragma intrinsic(cos, sin)
#pragma intrinsic(tan)
void __cdecl BuildUniformScaleMatrix(int scale, GfxScaleMatrix *matrix);

/*
 * Safely releases a Direct3D COM interface pointer.
 */
// FUNCTION: FF7 0x0067a050
void __cdecl D3DReleaseComInterface(IUnknown *comInterface)
{
    if (comInterface != 0)
        comInterface->Release();
}

/*
 * Releases Direct3D COM interface pointer from slot and clears pointer.
 */
// FUNCTION: FF7 0x0067a067
void __cdecl D3DReleaseComSlotAndClear(IUnknown **slot)
{
    D3DReleaseComInterface(*slot);
    *slot = 0;
}

/*
 * Creates a Direct3D material interface using the engine device.
 */
// FUNCTION: FF7 0x0067a083
int __cdecl D3DCreateMaterial(GfxEngineContext *ctx)
{
    int result;
    result = 0;
    // STRING: FF7 0x0090de4c
    CheckDirectDrawResult(((ComDirect3D *)ctx->direct3D)->CreateMaterial(&result, 0), "C:\\lib\\src\\graphics\\dx_mat.cpp", 36);
    return result;
}

/*
 * Frees a 0x50-byte matrix stack record on the game heap.
 */
// FUNCTION: FF7 0x0067a0c3
void __cdecl Gfx_FreeD3DMaterialStruct(void *record)
{
    if (record != 0) {
        // STRING: FF7 0x0090de6c
        GameHeapFree(record, "C:\\lib\\src\\graphics\\dx_mat.cpp", 49);
    }
}

/*
 * Allocates and initializes a 0x50-byte matrix stack record.
 */
// FUNCTION: FF7 0x0067a0e1
int __cdecl Gfx_AllocD3DMaterialStruct(void)
{
    GfxSizedRecord *rec;

    // STRING: FF7 0x0090de8c
    rec = (GfxSizedRecord *)GameHeapAlloc(1, 80, "C:\\lib\\src\\graphics\\dx_mat.cpp", 57);
    if (rec != 0) {
        rec->size = 80;
        rec->rampSize = 1;
    }
    return (int)rec;
}

/*
 * Converts BGRA color values to Direct3D material color format.
 */
// FUNCTION: FF7 0x0067a11b
void __cdecl D3DFillMaterialColorFromBgra(int colorSlot, GfxBgraColorSource *color, void *material)
{
    GfxMaterialColor *colorPtr;

    if (material != 0) {
        colorPtr = 0;
        switch (colorSlot) {
        case GFX_MATERIAL_COLOR_AMBIENT:
            colorPtr = (GfxMaterialColor *)((char *)material + 0x14);
            break;
        case GFX_MATERIAL_COLOR_DIFFUSE:
            colorPtr = (GfxMaterialColor *)((char *)material + 4);
            break;
        case GFX_MATERIAL_COLOR_SPECULAR:
            colorPtr = (GfxMaterialColor *)((char *)material + 0x24);
            break;
        case GFX_MATERIAL_COLOR_EMISSIVE:
            colorPtr = (GfxMaterialColor *)((char *)material + 0x34);
        }
        if (colorPtr != 0) {
            // Explicit double conversion preserves the original x87 load/store pairs.
            colorPtr->r = (double)color->red;
            colorPtr->g = (double)color->green;
            colorPtr->bgra = color->bgra;
            colorPtr->a = (double)color->alpha;
        }
    }
}

/*
 * Frees a PHundred render state record on the game heap.
 */
// FUNCTION: FF7 0x0067a1b6
void __cdecl Gfx_FreePHundred(int renderState)
{
    if (renderState != 0) {
        // STRING: FF7 0x0090deac
        GameHeapFree((void *)renderState, "C:\\lib\\src\\graphics\\dx_mat.cpp", 107);
    }
}

/*
 * Allocates and initializes a 0x64-byte PHundred render state record.
 */
// FUNCTION: FF7 0x0067a1d4
int __cdecl Gfx_AllocPHundred(void)
{
    PolygonHundredState *rec;

    // STRING: FF7 0x0090decc
    rec = (PolygonHundredState *)GameHeapAlloc(1, 100, "C:\\lib\\src\\graphics\\dx_mat.cpp", 115);
    if (rec != 0) {
        rec->initFlag = 1;
        rec->emitStates = 1;
        rec->changeMask = 0xFFFFFFFF;
        rec->shadeMode = 2;
        rec->ambientColor = 0xFFFFFFFF;
        rec->srcBlend = 2;
        rec->dstBlend = 1;
        rec->reservedPreset = 2;
        rec->blendMode = BLEND_NONE;
    }
    return (int)rec;
}

/*
 * Converts angle in degrees to radians.
 */
// FUNCTION: FF7 0x0067a254
float __cdecl DegreesToRadians(float degrees)
{
    float radians;

    radians = degrees * 0.017453292519944444;
    return radians;
}

/*
 * Converts angle in radians to degrees.
 */
// FUNCTION: FF7 0x0067a268
float __cdecl RadiansToDegrees(float radians)
{
    float degrees;

    degrees = radians * 57.29577951307855;
    return degrees;
}

/*
 * Deletes a Direct3D matrix handle using the device interface.
 */
// FUNCTION: FF7 0x0067a27c
void __cdecl D3DDeleteMatrixHandle(int matrix, GfxEngineContext *ctx)
{
    if (matrix != 0 && ctx != 0 && ctx->d3dDevice != 0) {
        // STRING: FF7 0x0090deec
        CheckDirectDrawResult(((ComDirect3DDevice *)ctx->d3dDevice)->DeleteMatrix(matrix), "C:\\lib\\src\\graphics\\dx_mat.cpp", 163);
    }
}

/*
 * Creates a Direct3D matrix handle from the active device.
 */
// FUNCTION: FF7 0x0067a2c8
int __cdecl D3DCreateMatrixHandle(GfxEngineContext *ctx)
{
    int result;
    result = 0;
    if (ctx != 0 && ctx->d3dDevice != 0) {
        // STRING: FF7 0x0090df0c
        CheckDirectDrawResult(((ComDirect3DDevice *)ctx->d3dDevice)->CreateMatrix(&result), "C:\\lib\\src\\graphics\\dx_mat.cpp", 178);
    }
    return result;
}

/*
 * Uploads matrix data to an allocated Direct3D matrix handle.
 */
// FUNCTION: FF7 0x0067a31b
void __cdecl D3DUploadMatrixToHandle(int matrix, int handle, GfxEngineContext *ctx)
{
    if (matrix != 0 && handle != 0 && ctx != 0) {
        // STRING: FF7 0x0090df2c
        CheckDirectDrawResult(((ComDirect3DDevice *)ctx->d3dDevice)->SetMatrix(handle, matrix), "C:\\lib\\src\\graphics\\dx_mat.cpp", 203);
    }
}

/*
 * Retrieves matrix data from a Direct3D matrix handle.
 */
// FUNCTION: FF7 0x0067a365
void __cdecl D3DGetMatrix(int matrix, int handle, struct GfxEngineContext *ctx)
{
    if (matrix != 0 && handle != 0 && ctx != 0) {
        // STRING: FF7 0x0090df4c
        CheckDirectDrawResult(((ComDirect3DDevice *)ctx->d3dDevice)->GetMatrix(handle, matrix), "C:\\lib\\src\\graphics\\dx_mat.cpp", 223);
    }
}

/*
 * Frees a 4x4 matrix allocation on the game heap.
 */
// FUNCTION: FF7 0x0067a3af
void __cdecl Gfx_FreeMatrix4x4(void *matrix)
{
    if (matrix != 0) {
        // STRING: FF7 0x0090df6c
        GameHeapFree(matrix, "C:\\lib\\src\\graphics\\dx_mat.cpp", 234);
    }
}

/*
 * Allocates and initializes a 4x4 identity matrix on the game heap.
 */
// FUNCTION: FF7 0x0067a3d0
int __cdecl Gfx_AllocIdentityMatrix4x4(void)
{
    GfxMatrix4x4 *rec;

    // STRING: FF7 0x0090df8c
    rec = (GfxMatrix4x4 *)GameHeapAlloc(1, 64, "C:\\lib\\src\\graphics\\dx_mat.cpp", 242);
    if (rec != 0) {
        rec->m00 = 1.0f;
        rec->m11 = 1.0f;
        rec->m22 = 1.0f;
        rec->m33 = 1.0f;
    }
    return (int)rec;
}

/*
 * Multiplies two 3x3 rotation matrices.
 */
// FUNCTION: FF7 0x0067a421
void __cdecl MultiplyRotation3x3(GfxRotationMatrixA *lhs, GfxMatrix3Row *rhs, GfxRotationMatrixA *result)
{
    /* Retail's hand-scheduled inline x87 3x3 matrix multiply. */
    __asm {
        mov     eax, lhs
        mov     ebx, rhs
        mov     ecx, dword ptr [eax]
        mov     edx, dword ptr [ebx]
        mov     ecx, dword ptr [eax+20h]
        mov     edx, dword ptr [ebx+20h]
        mov     eax, lhs
        mov     ebx, rhs
        mov     edx, result
        fld     dword ptr [eax]
        fmul    dword ptr [ebx]
        fld     dword ptr [eax+4]
        fmul    dword ptr [ebx+10h]
        fld     dword ptr [eax+8]
        fmul    dword ptr [ebx+20h]
        fxch    st(2)
        faddp   st(1), st(0)
        fld     dword ptr [eax]
        fmul    dword ptr [ebx+4]
        fxch    st(2)
        faddp   st(1), st(0)
        fxch    st(1)
        fld     dword ptr [eax+4]
        fmul    dword ptr [ebx+14h]
        fld     dword ptr [eax+8]
        fmul    dword ptr [ebx+24h]
        fxch    st(3)
        fstp    dword ptr [edx]
        fld     dword ptr [eax]
        fmul    dword ptr [ebx+8]
        fxch    st(3)
        faddp   st(1), st(0)
        fld     dword ptr [eax+4]
        fmul    dword ptr [ebx+18h]
        fxch    st(2)
        fld     dword ptr [eax+8]
        fmul    dword ptr [ebx+28h]
        fxch    st(2)
        faddp   st(1), st(0)
        fxch    st(3)
        faddp   st(1), st(0)
        fxch    st(2)
        fstp    dword ptr [edx+4]
        faddp   st(1), st(0)
        fstp    dword ptr [edx+8]
    }
    result->m00 = lhs->m00 * rhs->col0 + lhs->m01 * rhs[1].col0 + lhs->m02 * rhs[2].col0;
    result->m01 = lhs->m00 * rhs->col1 + lhs->m01 * rhs[1].col1 + lhs->m02 * rhs[2].col1;
    result->m02 = lhs->m00 * rhs->col2 + lhs->m01 * rhs[1].col2 + lhs->m02 * rhs[2].col2;
    result->m10 = lhs->m10 * rhs->col0 + lhs->m11 * rhs[1].col0 + lhs->m12 * rhs[2].col0;
    result->m11 = lhs->m10 * rhs->col1 + lhs->m11 * rhs[1].col1 + lhs->m12 * rhs[2].col1;
    result->m12 = lhs->m10 * rhs->col2 + lhs->m11 * rhs[1].col2 + lhs->m12 * rhs[2].col2;
}

/*
 * Multiplies a 3x3 matrix by the transpose of a second matrix.
 */
// FUNCTION: FF7 0x0067a5af
void __cdecl MultiplyRotation3x3ByTranspose(GfxLightMatrixA *lhs, PolygonTransform *rhs, GfxMatrix3Row *result)
{
    result->col0 = lhs->m00 * rhs->rot00 + lhs->m01 * rhs->rot01 + lhs->m02 * rhs->rot02;
    result->col1 = lhs->m00 * rhs->rot10 + lhs->m01 * rhs->rot11 + lhs->m02 * rhs->rot12;
    result->col2 = lhs->m00 * rhs->rot20 + lhs->m01 * rhs->rot21 + lhs->m02 * rhs->rot22;
    result[1].col0 = lhs->m10 * rhs->rot00 + lhs->m11 * rhs->rot01 + lhs->m12 * rhs->rot02;
    result[1].col1 = lhs->m10 * rhs->rot10 + lhs->m11 * rhs->rot11 + lhs->m12 * rhs->rot12;
    result[1].col2 = lhs->m10 * rhs->rot20 + lhs->m11 * rhs->rot21 + lhs->m12 * rhs->rot22;
    result[2].col0 = lhs->m20 * rhs->rot00 + lhs->m21 * rhs->rot01 + lhs->m22 * rhs->rot02;
    result[2].col1 = lhs->m20 * rhs->rot10 + lhs->m21 * rhs->rot11 + lhs->m22 * rhs->rot12;
    result[2].col2 = lhs->m20 * rhs->rot20 + lhs->m21 * rhs->rot21 + lhs->m22 * rhs->rot22;
}

/*
 * Concatenates rotation and row translation components.
 */
// FUNCTION: FF7 0x0067a74b
void __cdecl ConcatRotationAndTranslationRow(GfxMatrix3Row *lhs, GfxMatrix3Row *rhs, GfxMatrix3Row *dest)
{
    dest->col0 = lhs->col0 * rhs->col0 + lhs->col1 * rhs[1].col0 + lhs->col2 * rhs[2].col0;
    dest->col1 = lhs->col0 * rhs->col1 + lhs->col1 * rhs[1].col1 + lhs->col2 * rhs[2].col1;
    dest->col2 = lhs->col0 * rhs->col2 + lhs->col1 * rhs[1].col2 + lhs->col2 * rhs[2].col2;
    dest[1].col0 = lhs[1].col0 * rhs->col0 + lhs[1].col1 * rhs[1].col0 + lhs[1].col2 * rhs[2].col0;
    dest[1].col1 = lhs[1].col0 * rhs->col1 + lhs[1].col1 * rhs[1].col1 + lhs[1].col2 * rhs[2].col1;
    dest[1].col2 = lhs[1].col0 * rhs->col2 + lhs[1].col1 * rhs[1].col2 + lhs[1].col2 * rhs[2].col2;
    dest[2].col0 = lhs[2].col0 * rhs->col0 + lhs[2].col1 * rhs[1].col0 + lhs[2].col2 * rhs[2].col0;
    dest[2].col1 = lhs[2].col0 * rhs->col1 + lhs[2].col1 * rhs[1].col1 + lhs[2].col2 * rhs[2].col1;
    dest[2].col2 = lhs[2].col0 * rhs->col2 + lhs[2].col1 * rhs[1].col2 + lhs[2].col2 * rhs[2].col2;
    dest[3].col0 = lhs[3].col0 * rhs->col0 + lhs[3].col1 * rhs[1].col0 + lhs[3].col2 * rhs[2].col0 + rhs[3].col0;
    dest[3].col1 = lhs[3].col0 * rhs->col1 + lhs[3].col1 * rhs[1].col1 + lhs[3].col2 * rhs[2].col1 + rhs[3].col1;
    dest[3].col2 = lhs[3].col0 * rhs->col2 + lhs[3].col1 * rhs[1].col2 + lhs[3].col2 * rhs[2].col2 + rhs[3].col2;
}

/*
 * Concatenates rotation and column translation components.
 */
// FUNCTION: FF7 0x0067a982
void __cdecl ConcatRotationAndTranslationColumn(GfxMatrix4x4 *lhs, GfxMatrix3x *rhs, GfxMatrix3x *dest)
{
    dest->m00 = lhs->m00 * rhs->m00 + lhs->m01 * rhs->m10 + lhs->m02 * rhs->m20;
    dest->m01 = lhs->m00 * rhs->m01 + lhs->m01 * rhs->m11 + lhs->m02 * rhs->m21;
    dest->m02 = lhs->m00 * rhs->m02 + lhs->m01 * rhs->m12 + lhs->m02 * rhs->m22;
    dest->m10 = lhs->m10 * rhs->m00 + lhs->m11 * rhs->m10 + lhs->m12 * rhs->m20;
    dest->m11 = lhs->m10 * rhs->m01 + lhs->m11 * rhs->m11 + lhs->m12 * rhs->m21;
    dest->m12 = lhs->m10 * rhs->m02 + lhs->m11 * rhs->m12 + lhs->m12 * rhs->m22;
    dest->m20 = lhs->m20 * rhs->m00 + lhs->m21 * rhs->m10 + lhs->m22 * rhs->m20;
    dest->m21 = lhs->m20 * rhs->m01 + lhs->m21 * rhs->m11 + lhs->m22 * rhs->m21;
    dest->m22 = lhs->m20 * rhs->m02 + lhs->m21 * rhs->m12 + lhs->m22 * rhs->m22;
    dest->m03 = lhs->m00 * rhs->m03 + lhs->m01 * rhs->m13 + lhs->m02 * rhs->m23 + lhs->m03;
    dest->m13 = lhs->m10 * rhs->m03 + lhs->m11 * rhs->m13 + lhs->m12 * rhs->m23 + lhs->m13;
    dest->m23 = lhs->m30 * rhs->m03 + lhs->m31 * rhs->m13 + lhs->m32 * rhs->m23 + lhs->m23;
}

/*
 * Multiplies two 4x4 matrices.
 */
// FUNCTION: FF7 0x0067abb9
void __cdecl MultiplyMatrix4x4(GfxMatrix4x4 *lhs, GfxMatrix4x4 *rhs, GfxMatrix4x4 *dest)
{
    dest->m00 = lhs->m00 * rhs->m00 + lhs->m01 * rhs->m10 + lhs->m02 * rhs->m20 + lhs->m03 * rhs->m30;
    dest->m01 = lhs->m00 * rhs->m01 + lhs->m01 * rhs->m11 + lhs->m02 * rhs->m21 + lhs->m03 * rhs->m31;
    dest->m02 = lhs->m00 * rhs->m02 + lhs->m01 * rhs->m12 + lhs->m02 * rhs->m22 + lhs->m03 * rhs->m32;
    dest->m03 = lhs->m00 * rhs->m03 + lhs->m01 * rhs->m13 + lhs->m02 * rhs->m23 + lhs->m03 * rhs->m33;
    dest->m10 = lhs->m10 * rhs->m00 + lhs->m11 * rhs->m10 + lhs->m12 * rhs->m20 + lhs->m13 * rhs->m30;
    dest->m11 = lhs->m10 * rhs->m01 + lhs->m11 * rhs->m11 + lhs->m12 * rhs->m21 + lhs->m13 * rhs->m31;
    dest->m12 = lhs->m10 * rhs->m02 + lhs->m11 * rhs->m12 + lhs->m12 * rhs->m22 + lhs->m13 * rhs->m32;
    dest->m13 = lhs->m10 * rhs->m03 + lhs->m11 * rhs->m13 + lhs->m12 * rhs->m23 + lhs->m13 * rhs->m33;
    dest->m20 = lhs->m20 * rhs->m00 + lhs->m21 * rhs->m10 + lhs->m22 * rhs->m20 + lhs->m23 * rhs->m30;
    dest->m21 = lhs->m20 * rhs->m01 + lhs->m21 * rhs->m11 + lhs->m22 * rhs->m21 + lhs->m23 * rhs->m31;
    dest->m22 = lhs->m20 * rhs->m02 + lhs->m21 * rhs->m12 + lhs->m22 * rhs->m22 + lhs->m23 * rhs->m32;
    dest->m23 = lhs->m20 * rhs->m03 + lhs->m21 * rhs->m13 + lhs->m22 * rhs->m23 + lhs->m23 * rhs->m33;
    dest->m30 = lhs->m30 * rhs->m00 + lhs->m31 * rhs->m10 + lhs->m32 * rhs->m20 + lhs->m33 * rhs->m30;
    dest->m31 = lhs->m30 * rhs->m01 + lhs->m31 * rhs->m11 + lhs->m32 * rhs->m21 + lhs->m33 * rhs->m31;
    dest->m32 = lhs->m30 * rhs->m02 + lhs->m31 * rhs->m12 + lhs->m32 * rhs->m22 + lhs->m33 * rhs->m32;
    dest->m33 = lhs->m30 * rhs->m03 + lhs->m31 * rhs->m13 + lhs->m32 * rhs->m23 + lhs->m33 * rhs->m33;
}

/*
 * Multiplies first 4x4 matrix by second in-place.
 */
// FUNCTION: FF7 0x0067af75
void __cdecl PostMultiplyMatrix4x4InPlace(int lhs, int matrix)
{
    GfxMatrix4x4 copy;

    copy = *(GfxMatrix4x4 *)matrix;
    MultiplyMatrix4x4Full(&copy, (void *)lhs, (void *)matrix);
}

/*
 * Multiplies 4x4 matrix by right-transposed matrix.
 */
// FUNCTION: FF7 0x0067afa4
void __cdecl MultiplyMatrix4x4RightTransposed(GfxMatrix4x4 *matrixA, GfxMatrix4x4 *matrixB, GfxMatrix4x4 *product)
{
    product->m00 = matrixA->m00 * matrixB->m00 + matrixA->m01 * matrixB->m01 + matrixA->m02 * matrixB->m02 + matrixA->m03 * matrixB->m03;
    product->m01 = matrixA->m00 * matrixB->m10 + matrixA->m01 * matrixB->m11 + matrixA->m02 * matrixB->m12 + matrixA->m03 * matrixB->m13;
    product->m02 = matrixA->m00 * matrixB->m20 + matrixA->m01 * matrixB->m21 + matrixA->m02 * matrixB->m22 + matrixA->m03 * matrixB->m23;
    product->m03 = matrixA->m00 * matrixB->m30 + matrixA->m01 * matrixB->m31 + matrixA->m02 * matrixB->m32 + matrixA->m03 * matrixB->m33;
    product->m10 = matrixA->m10 * matrixB->m00 + matrixA->m11 * matrixB->m01 + matrixA->m12 * matrixB->m02 + matrixA->m13 * matrixB->m03;
    product->m11 = matrixA->m10 * matrixB->m10 + matrixA->m11 * matrixB->m11 + matrixA->m12 * matrixB->m12 + matrixA->m13 * matrixB->m13;
    product->m12 = matrixA->m10 * matrixB->m20 + matrixA->m11 * matrixB->m21 + matrixA->m12 * matrixB->m22 + matrixA->m13 * matrixB->m23;
    product->m13 = matrixA->m10 * matrixB->m30 + matrixA->m11 * matrixB->m31 + matrixA->m12 * matrixB->m32 + matrixA->m13 * matrixB->m33;
    product->m20 = matrixA->m20 * matrixB->m00 + matrixA->m21 * matrixB->m01 + matrixA->m22 * matrixB->m02 + matrixA->m23 * matrixB->m03;
    product->m21 = matrixA->m20 * matrixB->m10 + matrixA->m21 * matrixB->m11 + matrixA->m22 * matrixB->m12 + matrixA->m23 * matrixB->m13;
    product->m22 = matrixA->m20 * matrixB->m20 + matrixA->m21 * matrixB->m21 + matrixA->m22 * matrixB->m22 + matrixA->m23 * matrixB->m23;
    product->m23 = matrixA->m20 * matrixB->m30 + matrixA->m21 * matrixB->m31 + matrixA->m22 * matrixB->m32 + matrixA->m23 * matrixB->m33;
    product->m30 = matrixA->m30 * matrixB->m00 + matrixA->m31 * matrixB->m01 + matrixA->m32 * matrixB->m02 + matrixA->m33 * matrixB->m03;
    product->m31 = matrixA->m30 * matrixB->m10 + matrixA->m31 * matrixB->m11 + matrixA->m32 * matrixB->m12 + matrixA->m33 * matrixB->m13;
    product->m32 = matrixA->m30 * matrixB->m20 + matrixA->m31 * matrixB->m21 + matrixA->m32 * matrixB->m22 + matrixA->m33 * matrixB->m23;
    product->m33 = matrixA->m30 * matrixB->m30 + matrixA->m31 * matrixB->m31 + matrixA->m32 * matrixB->m32 + matrixA->m33 * matrixB->m33;
}

/*
 * Transforms 3D vector by transpose of 3x3 matrix.
 */
// FUNCTION: FF7 0x0067b360
void __cdecl TransformVector3ByTranspose3x3(GfxMatrix3Row *matrix, GfxHomogeneousPoint *inVec, GfxHomogeneousPoint *outVec)
{
    outVec->x = matrix->col0 * inVec->x + matrix[1].col0 * inVec->y + matrix[2].col0 * inVec->z;
    outVec->y = matrix->col1 * inVec->x + matrix[1].col1 * inVec->y + matrix[2].col1 * inVec->z;
    outVec->z = matrix->col2 * inVec->x + matrix[1].col2 * inVec->y + matrix[2].col2 * inVec->z;
}

/*
 * Transforms point by transpose matrix with homogeneous coordinates.
 */
// FUNCTION: FF7 0x0067b3ea
void __cdecl TransformPointByTransposeHomogeneous(GfxMatrix4x4 *matrix, GfxHomogeneousPoint *inPoint, GfxHomogeneousVec4 *outPoint)
{
    outPoint->x = matrix->m00 * inPoint->x + matrix->m10 * inPoint->y + matrix->m20 * inPoint->z + matrix->m30;
    outPoint->y = matrix->m01 * inPoint->x + matrix->m11 * inPoint->y + matrix->m21 * inPoint->z + matrix->m31;
    outPoint->z = matrix->m02 * inPoint->x + matrix->m12 * inPoint->y + matrix->m22 * inPoint->z + matrix->m32;
    outPoint->w = matrix->m03 * inPoint->x + matrix->m13 * inPoint->y + matrix->m23 * inPoint->z + matrix->m33;
}

/*
 * Transforms 3D point by transpose of matrix.
 */
// FUNCTION: FF7 0x0067b4b9
void __cdecl TransformPoint3ByTranspose(GfxMatrix3Row *matrix, GfxHomogeneousPoint *inPoint, GfxHomogeneousPoint *outPoint)
{
    outPoint->x = matrix->col0 * inPoint->x + matrix[1].col0 * inPoint->y + matrix[2].col0 * inPoint->z + matrix[3].col0;
    outPoint->y = matrix->col1 * inPoint->x + matrix[1].col1 * inPoint->y + matrix[2].col1 * inPoint->z + matrix[3].col1;
    outPoint->z = matrix->col2 * inPoint->x + matrix[1].col2 * inPoint->y + matrix[2].col2 * inPoint->z + matrix[3].col2;
}

/*
 * Transforms 4D vector by 4x4 matrix.
 */
// FUNCTION: FF7 0x0067b555
void __cdecl TransformVector4ByMatrix4x4(GfxMatrix4x4 *matrix, GfxHomogeneousVec4 *inVec, GfxHomogeneousVec4 *outVec)
{
    outVec->x = matrix->m00 * inVec->x + matrix->m01 * inVec->y + matrix->m02 * inVec->z + matrix->m03 * inVec->w;
    outVec->y = matrix->m10 * inVec->x + matrix->m11 * inVec->y + matrix->m12 * inVec->z + matrix->m13 * inVec->w;
    outVec->z = matrix->m20 * inVec->x + matrix->m21 * inVec->y + matrix->m22 * inVec->z + matrix->m23 * inVec->w;
    outVec->w = matrix->m30 * inVec->x + matrix->m31 * inVec->y + matrix->m32 * inVec->z + matrix->m33 * inVec->w;
}

/*
 * Computes transformed Z coordinate using transpose matrix.
 */
// FUNCTION: FF7 0x0067b644
void __cdecl TransformPointZByTranspose(GfxMatrixZRowSource *matrix, GfxHomogeneousPoint *inPoint, GfxHomogeneousPoint *outPoint)
{
    outPoint->z = matrix->m02 * inPoint->x + matrix->m12 * inPoint->y + matrix->m22 * inPoint->z + matrix->m32;
}

/*
 * Transforms 3D vector by 3x3 matrix.
 */
// FUNCTION: FF7 0x0067b67c
void __cdecl TransformVector3ByMatrix3x3(GfxMatrix3Row *matrix, GfxHomogeneousPoint *inVec, GfxHomogeneousPoint *outVec)
{
    outVec->x = matrix->col0 * inVec->x + matrix->col1 * inVec->y + matrix->col2 * inVec->z;
    outVec->y = matrix[1].col0 * inVec->x + matrix[1].col1 * inVec->y + matrix[1].col2 * inVec->z;
    outVec->z = matrix[2].col0 * inVec->x + matrix[2].col1 * inVec->y + matrix[2].col2 * inVec->z;
}

/*
 * Transforms 3D point by matrix into homogeneous 4D coordinates.
 */
// FUNCTION: FF7 0x0067b706
void __cdecl TransformPointHomogeneous(GfxMatrix4x4 *matrix, GfxHomogeneousPoint *inPoint, GfxHomogeneousVec4 *outPoint)
{
    outPoint->x = matrix->m00 * inPoint->x + matrix->m01 * inPoint->y + matrix->m02 * inPoint->z + matrix->m03;
    outPoint->y = matrix->m10 * inPoint->x + matrix->m11 * inPoint->y + matrix->m12 * inPoint->z + matrix->m13;
    outPoint->z = matrix->m20 * inPoint->x + matrix->m21 * inPoint->y + matrix->m22 * inPoint->z + matrix->m23;
    outPoint->w = matrix->m30 * inPoint->x + matrix->m31 * inPoint->y + matrix->m32 * inPoint->z + matrix->m33;
}

/*
 * Transforms 3D point by 4x4 matrix.
 */
// FUNCTION: FF7 0x0067b7d5
void __cdecl TransformPoint3ByMatrix4x4(GfxMatrix3x *matrix, GfxHomogeneousPoint *inPoint, GfxHomogeneousPoint *outPoint)
{
    outPoint->x = matrix->m00 * inPoint->x + matrix->m01 * inPoint->y + matrix->m02 * inPoint->z + matrix->m03;
    outPoint->y = matrix->m10 * inPoint->x + matrix->m11 * inPoint->y + matrix->m12 * inPoint->z + matrix->m13;
    outPoint->z = matrix->m20 * inPoint->x + matrix->m21 * inPoint->y + matrix->m22 * inPoint->z + matrix->m23;
}

/*
 * Transforms 4D vector by transpose matrix.
 */
// FUNCTION: FF7 0x0067b871
void __cdecl TransposeTransformVec4(GfxMatrix4x4 *matrix, GfxHomogeneousVec4 *inVec, GfxHomogeneousVec4 *outVec)
{
    outVec->x = matrix->m00 * inVec->x + matrix->m10 * inVec->y + matrix->m20 * inVec->z + matrix->m30 * inVec->w;
    outVec->y = matrix->m01 * inVec->x + matrix->m11 * inVec->y + matrix->m21 * inVec->z + matrix->m31 * inVec->w;
    outVec->z = matrix->m02 * inVec->x + matrix->m12 * inVec->y + matrix->m22 * inVec->z + matrix->m32 * inVec->w;
    outVec->w = matrix->m03 * inVec->x + matrix->m13 * inVec->y + matrix->m23 * inVec->z + matrix->m33 * inVec->w;
}

/*
 * Computes transformed Z coordinate by transpose matrix.
 */
// FUNCTION: FF7 0x0067b960
void __cdecl TransposeTransformPointZ(GfxTransposeZSource *matrix, GfxHomogeneousPoint *inPoint, GfxHomogeneousPoint *outPoint)
{
    outPoint->z = matrix->weightX * inPoint->x + matrix->weightY * inPoint->y + matrix->weightZ * inPoint->z + matrix->offsetZ;
}

/*
 * Performs perspective divide by W component in-place.
 */
// FUNCTION: FF7 0x0067b998
void __cdecl PerspectiveDivideInPlace(GfxHomogeneousVec4 *vec4)
{
    float recipW;

    if (vec4->w > 0.0f) {
        recipW = 1.0f / vec4->w;
        vec4->x *= recipW;
        vec4->y *= recipW;
        vec4->z *= recipW;
        *(int *)&vec4->w = FLOAT_ONE_BITS;
    }
}

/*
 * Performs perspective divide from input point to output point.
 */
// FUNCTION: FF7 0x0067b9f7
void __cdecl PerspectiveDivide(GfxHomogeneousVec4 *inVec, GfxHomogeneousVec43 *outVec)
{
    float recipW;

    if (inVec->w > 0.0f) {
        recipW = 1.0f / inVec->w;
        outVec->x = inVec->x * recipW;
        outVec->y = inVec->y * recipW;
        outVec->z = inVec->z * recipW;
        outVec->w = FLOAT_ONE_BITS;
    }
}

/*
 * Performs perspective divide and computes reciprocal W.
 */
// FUNCTION: FF7 0x0067ba56
void __cdecl PerspectiveDivideRecipW(GfxHomogeneousVec4 *inVec, GfxRecipWVec4 *outVec)
{
    float recipW;

    if (inVec->w > 0.0f) {
        recipW = 1.0f / inVec->w;
        outVec->x = inVec->x * recipW;
        outVec->y = inVec->y * recipW;
        outVec->z = inVec->z * recipW;
        outVec->recipW = recipW;
    }
}

/*
 * Performs perspective divide and writes reciprocal homogeneous W.
 */
// FUNCTION: FF7 0x0067bab4
void __cdecl PerspectiveDivideWithRhw(GfxHomogeneousVec4 *inVec, GfxClipVertex *clipVertex)
{
    float recipW;

    if (inVec->w > 0.0f) {
        recipW = 1.0f / inVec->w;
        clipVertex->x = inVec->x * recipW;
        clipVertex->y = inVec->y * recipW;
        clipVertex->z = inVec->z * recipW;
        clipVertex->homogeneousW = *(int *)&inVec->w;
        clipVertex->recipW = recipW;
    } else {
        clipVertex->homogeneousW = *(int *)&inVec->w;
    }
}

/*
 * Sets a 4x4 matrix to the identity matrix.
 */
// FUNCTION: FF7 0x0067bb2c
void __cdecl WriteIdentityMatrix4x4(GfxTransposedMatrix *matrix)
{
    matrix->m00 = FLOAT_ONE_BITS;
    matrix->m01 = 0;
    matrix->m02 = 0;
    matrix->m03 = 0;
    matrix->m10 = 0;
    matrix->m11 = FLOAT_ONE_BITS;
    matrix->m12 = 0;
    matrix->m13 = 0;
    matrix->m20 = 0;
    matrix->m21 = 0;
    matrix->m22 = FLOAT_ONE_BITS;
    matrix->m23 = 0;
    matrix->m30 = 0;
    matrix->m31 = 0;
    matrix->m32 = 0;
    matrix->m33 = FLOAT_ONE_BITS;
}

/*
 * Sets a 3x3 matrix to the identity matrix.
 */
// FUNCTION: FF7 0x0067bbd0
void __cdecl WriteIdentityMatrix3x3(GfxRawMatrix3Row *matrix3x3)
{
    matrix3x3->col0 = FLOAT_ONE_BITS;
    matrix3x3->col1 = 0;
    matrix3x3->col2 = 0;
    matrix3x3[1].col0 = 0;
    matrix3x3[1].col1 = FLOAT_ONE_BITS;
    matrix3x3[1].col2 = 0;
    matrix3x3[2].col0 = 0;
    matrix3x3[2].col1 = 0;
    matrix3x3[2].col2 = FLOAT_ONE_BITS;
}

/*
 * Clears the fourth row of a 4x4 matrix.
 */
// FUNCTION: FF7 0x0067bc2e
void __cdecl ClearMatrixRow4(GfxMatrixRow4 *matrix)
{
    matrix->m30 = 0;
    matrix->m31 = 0;
    matrix->m32 = 0;
    matrix->m33 = FLOAT_ONE_BITS;
}

/*
 * Clears the fourth column of a 4x4 matrix.
 */
// FUNCTION: FF7 0x0067bc5b
void __cdecl ClearMatrixColumn4(void *matrix)
{
    ((GfxMatrixColumn4 *)matrix)->m03 = 0;
    ((GfxMatrixColumn4 *)matrix)->m13 = 0;
    ((GfxMatrixColumn4 *)matrix)->m23 = 0;
    ((GfxMatrixColumn4 *)matrix)->m33 = FLOAT_ONE_BITS;
}

/*
 * Clears translation components of a 4x4 matrix.
 */
// FUNCTION: FF7 0x0067bc88
void __cdecl ClearMatrixTranslation(GfxTransformMatrix *matrix)
{
    matrix->translateX = 0;
    matrix->translateY = 0;
    matrix->translateZ = 0;
    matrix->m33 = FLOAT_ONE_BITS;
    matrix->m30 = 0;
    matrix->m31 = 0;
    matrix->m32 = 0;
}

/*
 * Builds 4x4 uniform scale transformation matrix.
 */
// FUNCTION: FF7 0x0067bcd3
void __cdecl BuildUniformScaleMatrix(int scale, GfxScaleMatrix *matrix)
{
    SetIdentityMatrix4x4(matrix);
    matrix->scaleX = scale;
    matrix->scaleY = scale;
    matrix->scaleZ = scale;
}

/*
 * Builds 4x4 non-uniform (x, y, z) scale transformation matrix.
 */
// FUNCTION: FF7 0x0067bcfe
void __cdecl BuildScaleMatrixXYZ(GfxExecPosition *scaleVec, GfxScaleMatrix *matrix)
{
    SetIdentityMatrix4x4(matrix);
    matrix->scaleX = scaleVec->x;
    matrix->scaleY = scaleVec->y;
    matrix->scaleZ = scaleVec->z;
}

/*
 * Sets row-vector translation components of a 4x4 matrix.
 */
// FUNCTION: FF7 0x0067bd31
void __cdecl SetMatrixTranslationRow(GfxExecPosition *translation, GfxMatrix4x4Translation *matrix)
{
    matrix->translateX = translation->x;
    matrix->translateY = translation->y;
    matrix->translateZ = translation->z;
}

/*
 * Sets row translation components of a 4x4 matrix.
 */
// FUNCTION: FF7 0x0067bd59
void __cdecl SetMatrixTranslationRowDup(GfxExecPosition *translation, GfxTransformMatrixRowForm *matrix)
{
    matrix->translationX = translation->x;
    matrix->translationY = translation->y;
    matrix->translationZ = translation->z;
}

/*
 * Builds 4x4 translation matrix using row vector convention.
 */
// FUNCTION: FF7 0x0067bd81
void __cdecl BuildTranslationMatrixRowVec(void *translation, void *matrix)
{
    SetIdentityMatrix4x4(matrix);
    SetMatrixTranslationRow((struct GfxExecPosition *)translation, (struct GfxMatrix4x4Translation *)matrix);
}

/*
 * Sets column translation components of a 4x4 matrix.
 */
// FUNCTION: FF7 0x0067bda2
void __cdecl SetMatrixTranslationColumn(GfxExecPosition *translation, GfxTransformMatrix *matrix)
{
    matrix->translateX = translation->x;
    matrix->translateY = translation->y;
    matrix->translateZ = translation->z;
}

/*
 * Sets column-vector translation components of a 4x4 matrix.
 */
// FUNCTION: FF7 0x0067bdca
void __cdecl SetMatrixTranslationColumnDup(GfxExecPosition *translation, GfxTransformMatrix *matrix)
{
    matrix->translateX = translation->x;
    matrix->translateY = translation->y;
    matrix->translateZ = translation->z;
}

/*
 * Builds 4x4 translation matrix using column vector convention.
 */
// FUNCTION: FF7 0x0067bdf2
void __cdecl BuildTranslationMatrixColVec(int translation, void *matrix)
{
    SetIdentityMatrix4x4(matrix);
    SetMatrixTranslationColumn((struct GfxExecPosition *)translation, (struct GfxTransformMatrix *)matrix);
}

/*
 * Builds 4x4 X-axis rotation matrix using row vector convention.
 */
// FUNCTION: FF7 0x0067be13
void __cdecl BuildRotationXRowVec(float angle, void *matrix)
{
    float cosVal;
    float sinVal;
    float rad;
    SetIdentityMatrix4x4(matrix);
    // The assignment retains ST(0) for cos and stores radians for sin.
    cosVal = cos(rad = DegreesToRadians(angle));
    sinVal = sin(rad);
    ((GfxMatrix4x4 *)matrix)->m11 = cosVal;
    ((GfxMatrix4x4 *)matrix)->m12 = sinVal;
    ((GfxMatrix4x4 *)matrix)->m21 = -sinVal;
    ((GfxMatrix4x4 *)matrix)->m22 = cosVal;
}

/*
 * Builds 4x4 Y-axis rotation matrix using row vector convention.
 */
// FUNCTION: FF7 0x0067be71
void __cdecl BuildRotationYRowVec(float angle, void *matrix)
{
    float cosVal;
    float sinVal;
    float rad;
    SetIdentityMatrix4x4(matrix);
    cosVal = cos(rad = DegreesToRadians(angle));
    sinVal = sin(rad);
    ((GfxMatrix4x4 *)matrix)->m00 = cosVal;
    ((GfxMatrix4x4 *)matrix)->m02 = -sinVal;
    ((GfxMatrix4x4 *)matrix)->m20 = sinVal;
    ((GfxMatrix4x4 *)matrix)->m22 = cosVal;
}

/*
 * Builds 4x4 Z-axis rotation matrix using row vector convention.
 */
// FUNCTION: FF7 0x0067bece
void __cdecl BuildRotationZRowVec(float angle, void *matrix)
{
    float cosVal;
    float sinVal;
    float rad;
    SetIdentityMatrix4x4(matrix);
    cosVal = cos(rad = DegreesToRadians(angle));
    sinVal = sin(rad);
    ((GfxMatrix4x4 *)matrix)->m00 = cosVal;
    ((GfxMatrix4x4 *)matrix)->m01 = sinVal;
    ((GfxMatrix4x4 *)matrix)->m10 = -sinVal;
    ((GfxMatrix4x4 *)matrix)->m11 = cosVal;
}

/*
 * Builds 4x4 X-axis rotation matrix using column vector convention.
 */
// FUNCTION: FF7 0x0067bf2b
void __cdecl BuildRotationXColVec(float angle, GfxMatrix4x4 *matrix)
{
    float cosVal;
    float sinVal;
    float rad;
    SetIdentityMatrix4x4(matrix);
    cosVal = cos(rad = DegreesToRadians(angle));
    sinVal = sin(rad);
    matrix->m11 = cosVal;
    matrix->m12 = -sinVal;
    matrix->m21 = sinVal;
    matrix->m22 = cosVal;
}

/*
 * Builds 4x4 Y-axis rotation matrix using column vector convention.
 */
// FUNCTION: FF7 0x0067bf89
void __cdecl BuildRotationYColVec(float angle, GfxMatrix4x4 *matrix)
{
    float cosVal;
    float sinVal;
    float rad;
    SetIdentityMatrix4x4(matrix);
    cosVal = cos(rad = DegreesToRadians(angle));
    sinVal = sin(rad);
    matrix->m00 = cosVal;
    matrix->m02 = sinVal;
    matrix->m20 = -sinVal;
    matrix->m22 = cosVal;
}

/*
 * Builds 4x4 Z-axis rotation matrix using column vector convention.
 */
// FUNCTION: FF7 0x0067bfe6
void __cdecl BuildRotationZColVec(float angle, GfxMatrix4x4 *matrix)
{
    float cosVal;
    float sinVal;
    float rad;
    SetIdentityMatrix4x4(matrix);
    cosVal = cos(rad = DegreesToRadians(angle));
    sinVal = sin(rad);
    matrix->m00 = cosVal;
    matrix->m01 = -sinVal;
    matrix->m10 = sinVal;
    matrix->m11 = cosVal;
}

/*
 * Translates 4x4 matrix by vector using row convention.
 */
// FUNCTION: FF7 0x0067c043
void __cdecl TranslateMatrixRowVec(void *translation, void *matrix, void *outMatrix)
{
    int translationMat[16];

    BuildTranslationMatrixRowVec(translation, translationMat);
    MultiplyMatrix4x4Full(matrix, translationMat, outMatrix);
}

/*
 * Adds offset vector to row translation of a 4x4 matrix.
 */
// FUNCTION: FF7 0x0067c071
void __cdecl AddMatrixTranslationRow(GfxHomogeneousPoint *offset, GfxMatrixTranslationRow *matrix)
{
    matrix->translateX += offset->x;
    matrix->translateY += offset->y;
    matrix->translateZ += offset->z;
}

/*
 * Pre-multiplies 4x4 matrix by translation vector.
 */
// FUNCTION: FF7 0x0067c0ab
void __cdecl PreTranslateMatrix4x4(int translation, void *matrix, void *outMatrix)
{
    int translationMat[16];

    BuildTranslationMatrixColVec(translation, translationMat);
    MultiplyMatrix4x4Full(translationMat, matrix, outMatrix);
}

/*
 * Adds offset vector to column translation of a 4x4 matrix.
 */
// FUNCTION: FF7 0x0067c0d9
void __cdecl AddTranslationColumn(GfxHomogeneousPoint *offset, GfxMatrixTranslationColumn *matrix)
{
    matrix->translateX += offset->x;
    matrix->translateY += offset->y;
    matrix->translateZ += offset->z;
}

/*
 * Multiplies matrix by axis rotation using row convention.
 */
// FUNCTION: FF7 0x0067c113
void __cdecl MultiplyAxisRotationRowVec(int axis, float angle, void *src, void *dst)
{
    GfxMatrix4x4 matrix;

    switch (axis) {
    case ANIM_AXIS_X:
        BuildRotationXRowVec(angle, &matrix);
        break;
    case ANIM_AXIS_Y:
        BuildRotationYRowVec(angle, &matrix);
        break;
    case ANIM_AXIS_Z:
        BuildRotationZRowVec(angle, &matrix);
    }
    MultiplyMatrix4x4Full(src, &matrix, dst);
}

/*
 * Applies axis rotation to 4x4 transformation matrix.
 */
// FUNCTION: FF7 0x0067c17f
void __cdecl RotateMatrixAboutAxis(int axis, float angle, void *matrix)
{
    int copy[16];

    *(GfxMatrix4x4 *)(copy) = *(GfxMatrix4x4 *)(matrix);
    MultiplyAxisRotationRowVec(axis, angle, copy, matrix);
}

/*
 * Multiplies matrix by axis rotation using column convention.
 */
// FUNCTION: FF7 0x0067c1b2
void __cdecl MultiplyAxisRotationColVec(int axis, float angle, void *src, void *dst)
{
    GfxMatrix4x4 matrix;

    switch (axis) {
    case ANIM_AXIS_X:
        BuildRotationXColVec(angle, &matrix);
        break;
    case ANIM_AXIS_Y:
        BuildRotationYColVec(angle, &matrix);
        break;
    case ANIM_AXIS_Z:
        BuildRotationZColVec(angle, &matrix);
    }
    MultiplyMatrix4x4Full(&matrix, src, dst);
}

/*
 * Rotates matrix about axis using column vector convention.
 */
// FUNCTION: FF7 0x0067c21e
void __cdecl RotateMatrixAboutAxisColVec(int axis, float angle, void *matrix)
{
    int copy[16];

    *(GfxMatrix4x4 *)(copy) = *(GfxMatrix4x4 *)(matrix);
    MultiplyAxisRotationColVec(axis, angle, copy, matrix);
}

/*
 * Transposes a 3x3 matrix.
 */
// FUNCTION: FF7 0x0067c251
void __cdecl TransposeMatrix3x3(void *src, void *dst)
{
    ((GfxRawMatrix3Row *)dst)->col0 = ((GfxRawMatrix3Row *)src)->col0;
    ((GfxRawMatrix3Row *)dst)->col1 = ((GfxRawMatrix3Row *)src)[1].col0;
    ((GfxRawMatrix3Row *)dst)->col2 = ((GfxRawMatrix3Row *)src)[2].col0;
    ((GfxRawMatrix3Row *)dst)[1].col0 = ((GfxRawMatrix3Row *)src)->col1;
    ((GfxRawMatrix3Row *)dst)[1].col1 = ((GfxRawMatrix3Row *)src)[1].col1;
    ((GfxRawMatrix3Row *)dst)[1].col2 = ((GfxRawMatrix3Row *)src)[2].col1;
    ((GfxRawMatrix3Row *)dst)[2].col0 = ((GfxRawMatrix3Row *)src)->col2;
    ((GfxRawMatrix3Row *)dst)[2].col1 = ((GfxRawMatrix3Row *)src)[1].col2;
    ((GfxRawMatrix3Row *)dst)[2].col2 = ((GfxRawMatrix3Row *)src)[2].col2;
}

/*
 * Transposes a 4x4 matrix.
 */
// FUNCTION: FF7 0x0067c2c0
void __cdecl TransposeMatrix4x4(GfxTransposedMatrix *src, GfxTransposedMatrix *dst)
{
    dst->m00 = src->m00;
    dst->m01 = src->m10;
    dst->m02 = src->m20;
    dst->m03 = src->m30;
    dst->m10 = src->m01;
    dst->m11 = src->m11;
    dst->m12 = src->m21;
    dst->m13 = src->m31;
    dst->m20 = src->m02;
    dst->m21 = src->m12;
    dst->m22 = src->m22;
    dst->m23 = src->m32;
    dst->m30 = src->m03;
    dst->m31 = src->m13;
    dst->m32 = src->m23;
    dst->m33 = src->m33;
}

/*
 * Transposes a 4x4 matrix in-place.
 */
// FUNCTION: FF7 0x0067c383
void __cdecl TransposeMatrix4x4InPlace(int matrix)
{
    int copy[16];

    *(GfxMatrix4x4 *)(copy) = *(GfxMatrix4x4 *)(matrix);
    TransposeMatrix4x4((struct GfxTransposedMatrix *)copy, (struct GfxTransposedMatrix *)matrix);
}

/*
 * Constructs perspective projection matrix with viewport parameters.
 */
// FUNCTION: FF7 0x0067c3ae
void __cdecl BuildProjectionMatrix(float sx, float fov, float zn, float zf, float cx, float cy, float vw, float vh, float (*out)[4])
{
    GfxEngineContext *ctx;
    float(*p)[4];
    float mat[4][4];
    float mproj[4][4];
    float rad;
    float(*pr)[4];
    float ht;
    float xscl;

    ctx = GetEngineContext();
    p = mat;
    pr = mproj;
    SetIdentityMatrix4x4(p);
    SetIdentityMatrix4x4(pr);
    if (out != 0 && p != 0 && pr != 0 && zn > 0.0f && zn < zf) {
        if (ctx->viewportHeightOverride != 0) {
            p[0][0] = 0.75f;
        } else {
            p[0][0] = sx;
        }
        p[1][1] = -1.0f;
        p[2][2] = 1.0f;
        if (vw > 0.0f && vh > 0.0f) {
            if (ctx->viewportHeightOverride != 0) {
                xscl = sx / 0.75f * 2.0f;
            } else {
                xscl = 2.0f;
            }
            p[3][0] = cx * 2.0f / vw;
            p[3][1] = -(cy * xscl) / vh;
        }
        rad = DegreesToRadians(fov);
        pr[2][2] = (ht = zn * tan(rad / 2.0f)) * zf / ((zf - zn) * zn);
        pr[2][3] = ht / zn;
        pr[3][2] = -ht * zf / (zf - zn);
        pr[3][3] = 0.0f;
        MultiplyMatrix4x4Full(pr, p, out);
    }
}

/*
 * Constructs symmetric square perspective projection matrix.
 */
// FUNCTION: FF7 0x0067c573
void __cdecl BuildSquareProjectionMatrix(float fov, float zn, float zf, float (*out)[4])
{
    float(*p)[4];
    float mat[4][4];
    float mproj[4][4];
    float rad;
    float(*pr)[4];
    float ht;

    p = mat;
    pr = mproj;
    SetIdentityMatrix4x4(p);
    SetIdentityMatrix4x4(pr);
    if (out != 0 && p != 0 && pr != 0 && zn > 0.0f && zn < zf) {
        SetIdentityMatrix4x4(out);
        rad = DegreesToRadians(fov);
        ht = zn * tan(rad / 2.0f);
        p[0][0] = zf / ht;
        p[1][1] = zf / ht;
        pr[2][2] = zn / (zn - zf);
        pr[2][3] = 1.0f;
        pr[3][2] = -zn * zf / (zn - zf);
        pr[3][3] = 0.0f;
        MultiplyMatrix4x4Full(pr, p, out);
    }
}

/*
 * Releases Direct3D render material interface.
 */
// FUNCTION: FF7 0x0067c6a6
void __cdecl DestroyRenderMaterial(GfxRenderMaterial *material)
{
    GfxEngineContext *context;

    if (material != 0) {
        if (material->matrix != 0) {
            // STRING: FF7 0x0090dfac
            GameHeapFree((void *)material->heapBlockA, "C:\\lib\\src\\graphics\\dx_mat.cpp", 1705);
            // STRING: FF7 0x0090dfcc
            GameHeapFree((void *)material->heapBlockB, "C:\\lib\\src\\graphics\\dx_mat.cpp", 1706);
        }
        Gfx_FreeMatrix4x4(material->matrix4A);
        Gfx_FreeMatrix4x4((void *)material->matrix4B);
        Gfx_FreeMatrix4x4((void *)material->matrix4C);
        context = GetEngineContext();
        if (context != 0) {
            D3DDeleteMatrixHandle(material->matrixHandleA, context);
            D3DDeleteMatrixHandle(material->matrixHandleB, context);
            D3DDeleteMatrixHandle(material[1].matrix, context);
        }
        // STRING: FF7 0x0090dfec
        GameHeapFree(material, "C:\\lib\\src\\graphics\\dx_mat.cpp", 1722);
    }
}

/*
 * Allocates and initializes array of matrix stack records.
 */
// FUNCTION: FF7 0x0067c77c
int __cdecl CreateMatrixArray(int n)
{
    GfxMatrixArray *array;
    int index;

    // STRING: FF7 0x0090e00c
    array = (GfxMatrixArray *)GameHeapAlloc(1, 44, "C:\\lib\\src\\graphics\\dx_mat.cpp", 1730);
    if (array != 0 && n > 0) {
        array->hasMatrices = 1;
        array->count = n;
        // STRING: FF7 0x0090e02c
        array->matrixBuffer = (int)GameHeapAlloc(n, 1, "C:\\lib\\src\\graphics\\dx_mat.cpp", 1739);
        // STRING: FF7 0x0090e04c
        array->matrices = (int)GameHeapAlloc(n, 64, "C:\\lib\\src\\graphics\\dx_mat.cpp", 1740);
        if (array->matrices != 0) {
            for (index = 0; index < n; index++) {
                SetIdentityMatrix4x4((void *)(array->matrices + index * 0x40));
            }
        }
        array->handle = D3DCreateMatrixHandle(GetEngineContext());
    }
    return (int)array;
}

/*
 * Initializes empty matrix table for graphics engine context.
 */
// FUNCTION: FF7 0x0067c854
void __cdecl Gfx_CreateEmptyMatrixTable(void)
{
    CreateMatrixArray(0);
}

/*
 * Uploads matrix data and updates Direct3D device matrix slot.
 */
// FUNCTION: FF7 0x0067c863
void __cdecl D3DSetMatrix(int slot, GfxMatrix4x4 *matrix, GfxMatrixState *state, GfxEngineContext *context)
{
    if (state != 0) {
        switch (slot) {
        case GFX_MATRIX_SLOT_WORLD:
            if (state->matrixPtr0 != 0) {
                *(GfxMatrix4x4 *)(state->matrixPtr0) = *matrix;
            } else {
                state->matrixPtr0 = (int)matrix;
            }
            if (state->matrixHandle0 == 0) {
                state->matrixHandle0 = D3DCreateMatrixHandle(context);
            }
            if (state->matrixPtr0 != 0) {
                D3DUploadMatrixToHandle((int)matrix, state->matrixHandle0, context);
            }
            break;
        case GFX_MATRIX_SLOT_VIEW:
            if (state->matrixPtr1 != 0) {
                *(GfxMatrix4x4 *)(state->matrixPtr1) = *matrix;
            } else {
                state->matrixPtr1 = (int)matrix;
            }
            if (state->matrixHandle1 == 0) {
                state->matrixHandle1 = D3DCreateMatrixHandle(context);
            }
            if (state->matrixPtr1 != 0) {
                D3DUploadMatrixToHandle((int)matrix, state->matrixHandle1, context);
            }
            break;
        case GFX_MATRIX_SLOT_PROJECTION:
            if (state->matrixPtr2 != 0) {
                *(GfxMatrix4x4 *)(state->matrixPtr2) = *matrix;
            } else {
                state->matrixPtr2 = (int)matrix;
            }
            if (state->matrixHandle2 == 0) {
                state->matrixHandle2 = D3DCreateMatrixHandle(context);
            }
            if (state->matrixPtr2 != 0) {
                D3DUploadMatrixToHandle((int)matrix, state->matrixHandle2, context);
            }
        }
    }
}

/*
 * Retrieves cached device matrix handle for transform state.
 */
// FUNCTION: FF7 0x0067c9be
int __cdecl GetCachedDeviceTransform(int slot, void *record)
{
    int handle;

    handle = 0;
    if (record != 0) {
        switch (slot) {
        case GFX_MATRIX_SLOT_WORLD:
            handle = ((GfxCachedTransformRecord *)record)->matrixPtr0;
            break;
        case GFX_MATRIX_SLOT_VIEW:
            handle = ((GfxCachedTransformRecord *)record)->matrixPtr1;
            break;
        case GFX_MATRIX_SLOT_PROJECTION:
            handle = ((GfxCachedTransformRecord *)record)->matrixPtr2;
        }
    }
    return handle;
}

/*
 * Sets status byte for specified matrix table slot.
 */
// FUNCTION: FF7 0x0067ca11
void __cdecl Gfx_SetMatrixTableSlotStatus(int value, int slot, GfxMatrixTable *table)
{
    if (table != 0 && table->slotBytes != 0 && slot < table->slotCount) {
        *(char *)((char *)table[1].slotBytes + slot) = value;
    }
}

/*
 * Updates matrix table slot with matrix and handles.
 */
// FUNCTION: FF7 0x0067ca3e
void __cdecl Gfx_SetMatrixTableSlot(int status, int slot, int matrix, GfxMatrixTable *table)
{
    if (table != 0 && table->slotBytes != 0 && slot >= 0 && slot < table->slotCount) {
        Gfx_SetMatrixTableSlotStatus(status, slot, table);
        *(GfxMatrix4x4 *)(table[1].slotCount + (slot << 6)) = *(GfxMatrix4x4 *)(matrix);
    }
}

/*
 * Calls matrix slot update callback with parameters.
 */
// FUNCTION: FF7 0x0067ca92
void __cdecl Gfx_SetMatrixTableSlotCall(int mode, int slot, GfxMatrixTable2 *table, struct GfxEngineContext *context)
{
    if (table != 0 && table->enabled != 0 && slot < table->slotCount) {
        Gfx_SetMatrixCall(mode, table->slotBase + (slot << 6), (int)table, context);
    }
}

/*
 * Multiplies and updates Direct3D device transform matrix.
 */
// FUNCTION: FF7 0x0067cad3
void __cdecl D3DMultiplyMatrix(int slot, void *rhs, GfxD3DMatrixSlots *slots, GfxEngineContext *context)
{
    float matrix[16];

    if (slots != 0) {
        switch (slot) {
        case GFX_MATRIX_SLOT_WORLD:
            if (slots->matrixPtr0 != 0) {
                *(GfxMatrix4x4 *)(matrix) = *(GfxMatrix4x4 *)(slots->matrixPtr0);
                MultiplyMatrix4x4Full(matrix, rhs, (void *)slots->matrixPtr0);
                D3DUploadMatrixToHandle(slots->matrixPtr0, slots->handle0, context);
            }
            break;
        case GFX_MATRIX_SLOT_VIEW:
            if (slots->matrixPtr1 != 0) {
                *(GfxMatrix4x4 *)(matrix) = *(GfxMatrix4x4 *)(slots->matrixPtr1);
                MultiplyMatrix4x4Full(matrix, rhs, (void *)slots->matrixPtr1);
                D3DUploadMatrixToHandle(slots->matrixPtr1, slots->handle1, context);
            }
            break;
        case GFX_MATRIX_SLOT_PROJECTION:
            if (slots->matrixPtr2 != 0) {
                *(GfxMatrix4x4 *)(matrix) = *(GfxMatrix4x4 *)(slots->matrixPtr2);
                MultiplyMatrix4x4Full(matrix, rhs, (void *)slots->matrixPtr2);
                D3DUploadMatrixToHandle(slots->matrixPtr2, slots->handle2, context);
            }
        }
    }
}

/*
 * Sets active model-to-world matrix in graphics context.
 */
// FUNCTION: FF7 0x0067cbf1
void __cdecl Gfx_SetContextMatrix(void *matrix, struct GfxEngineContext *context)
{
    if (context != 0 && context->cameraMatrix != 0) {
        *(GfxMatrix4x4 *)(context->cameraMatrix) = *(GfxMatrix4x4 *)(matrix);
    }
}

/*
 * Multiplies active graphics context transformation matrix.
 */
// FUNCTION: FF7 0x0067cc1f
void __cdecl Gfx_MultiplyContextMatrix(void *matrix, struct GfxEngineContext *context)
{
    int copy[16];

    if (context != 0 && context->cameraMatrix != 0) {
        *(GfxMatrix4x4 *)(copy) = *(GfxMatrix4x4 *)(context->cameraMatrix);
        MultiplyMatrix4x4Full(copy, matrix, (void *)context->cameraMatrix);
    }
}

/*
 * Rebuilds combined model-to-screen matrix in context.
 */
// FUNCTION: FF7 0x0067cc6c
void __cdecl Gfx_RebuildContextScreenTransform(void *slot, void *context)
{
    int table;

    if (context != 0 && ((GfxEngineContext *)context)->opaqueSet != 0) {
        table = ((GfxPolygonSet *)((GfxEngineContext *)context)->opaqueSet)->matrices;
        if (table != 0) {
            Gfx_SetMatrixCall(GFX_MATRIX_SLOT_VIEW, (int)slot, table, (GfxEngineContext *)context);
        }
        BuildScreenTransformMatrix((void *)((GfxEngineContext *)context)->screenTransform);
        TransposeMatrix4x4((struct GfxTransposedMatrix *)((GfxEngineContext *)context)->screenTransform, (struct GfxTransposedMatrix *)((GfxEngineContext *)context)->screenTransformTransposed);
    }
}

/*
 * Configures viewport perspective projection parameters.
 */
// FUNCTION: FF7 0x0067ccde
void __cdecl SetPerspective(float fov, float nearPlane, float farPlane, float offsetX, float offsetY, float viewWidth, float viewHeight, GfxEngineContext *context)
{
    struct {
        int cached;
        float matrix[16];
        float aspect;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    if (context != 0 && context->opaqueSet != 0) {
        frame.cached = context->opaqueSet->matrices;
        if (frame.cached != 0) {
            frame.aspect = GetViewportAspectRatio(context);
            BuildProjectionMatrix(frame.aspect, fov, nearPlane, farPlane, offsetX, offsetY, viewWidth, viewHeight, (float(*)[4])((int)frame.matrix));
            Gfx_SetMatrixCall(GFX_MATRIX_SLOT_PROJECTION, (int)frame.matrix, frame.cached, context);
            context->fov = fov;
            context->nearPlane = nearPlane;
            context->farPlane = farPlane;
            context->offsetX = offsetX;
            context->offsetY = offsetY;
        }
        BuildScreenTransformMatrix((void *)context->screenTransform);
        TransposeMatrix4x4((struct GfxTransposedMatrix *)context->screenTransform, (struct GfxTransposedMatrix *)context->screenTransformTransposed);
    }
}

/*
 * Builds viewport screen-space mapping matrix.
 */
// FUNCTION: FF7 0x0067cdd5
void __cdecl BuildViewportMatrix(GfxViewportMatrix *matrix, GfxEngineContext *context)
{
    SetIdentityMatrix4x4((void *)matrix);
    matrix->scaleX = (float)(context->viewSizeX / 2);
    if (context->viewportHeightOverride != 0) {
        matrix->scaleY = (float)-(context->viewportHeightOverride / 2);
    } else {
        matrix->scaleY = (float)-(context->viewSizeY / 2);
    }
    matrix->translateX = (float)context->clipLeft + matrix->scaleX;
    matrix[1].scaleX = (float)context->clipTop - -(context->viewSizeY / 2);
}

/*
 * Composes model-view and projection matrices into screen matrix.
 */
// FUNCTION: FF7 0x0067ce83
int __cdecl ComposeModelToScreenMatrix(void *matrix, void *dst)
{
    ComposeWithScreenMatrix(matrix, dst);
    return 1;
}

/*
 * Transforms and projects 3D point using specified matrix.
 */
// FUNCTION: FF7 0x0067ce9d
int __cdecl ProjectPointWithMatrix(int matrix, int point, int dst, int scale)
{
    int result;
    int composed[16];

    result = 0;
    if (ComposeModelToScreenMatrix((void *)matrix, composed) != 0) {
        TransformPointByMatrix4x4((float *)(composed), (float *)(point), &((GfxHomogeneousVec4 *)dst)->x);
        PerspectiveDivide((struct GfxHomogeneousVec4 *)dst, (struct GfxHomogeneousVec43 *)scale);
        if (((GfxHomogeneousVec4 *)dst)->w > 0.0) {
            result = 1;
        }
    }
    return result;
}

/*
 * Builds model-to-screen transformation matrix from context.
 */
// FUNCTION: FF7 0x0067cf03
int __cdecl BuildModelToScreenMatrix(int matrix, void *dst)
{
    int projection;
    int viewProj[16];
    int view;
    int viewportBuf[16];
    GfxPolygonSetRecord *polySet;
    int flag;
    int modelView[16];
    GfxEngineContext *ctx;

    flag = 0;
    ctx = GetEngineContext();
    if (ctx != 0 && ctx->opaqueSet != 0) {
        polySet = (GfxPolygonSetRecord *)ctx->opaqueSet->matrices;
        view = GetCachedDeviceTransform(GFX_MATRIX_SLOT_VIEW, polySet);
        projection = GetCachedDeviceTransform(GFX_MATRIX_SLOT_PROJECTION, polySet);
        if (view != 0 && projection != 0) {
            ConcatAffineMatrix4x4((float(*)[4])(matrix), (float(*)[4])(view), (float(*)[4])(modelView));
            ClearMatrixColumn4(modelView);
            MultiplyMatrix4x4Full(modelView, (void *)projection, viewProj);
            BuildViewportMatrix((struct GfxViewportMatrix *)(viewportBuf), ctx);
            MultiplyMatrix4x4Full(viewProj, viewportBuf, dst);
            flag = 1;
        }
    }
    return flag;
}

/*
 * Transforms point using built model-to-screen matrix.
 */
// FUNCTION: FF7 0x0067cff0
int __cdecl ProjectPointWithBuiltMatrix(int matrix, int point, GfxHomogeneousVec4 *dst, int scale)
{
    int visible;
    int built[16];

    visible = 0;
    if (BuildModelToScreenMatrix(matrix, built) != 0) {
        TransformPointByMatrix4x4((float *)(built), (float *)(point), &dst->x);
        PerspectiveDivide(dst, (struct GfxHomogeneousVec43 *)scale);
        if (dst->w > 0.0) {
            visible = 1;
        }
    }
    return visible;
}

/*
 * Projects 3D model point to screen coordinates.
 */
// FUNCTION: FF7 0x0067d056
int __cdecl ProjectModelPointToScreen(int matrix, void *dst, struct GfxHomogeneousVec43 *scale)
{
    int visible;
    int tmpMatrix[4][4];

    visible = 0;
    if (ComposeModelToScreenMatrix((void *)matrix, tmpMatrix) != 0) {
        ((GfxProjectedPoint *)dst)->x = tmpMatrix[3][0];
        ((GfxProjectedPoint *)dst)->y = tmpMatrix[3][1];
        ((GfxProjectedPoint *)dst)->z = tmpMatrix[3][2];
        ((GfxProjectedPoint *)dst)->coordWidth = tmpMatrix[3][3];
        PerspectiveDivide((struct GfxHomogeneousVec4 *)dst, scale);
        if (((GfxHomogeneousVec4 *)dst)->w > 0.0) {
            visible = 1;
        }
    }
    return visible;
}

/*
 * Transforms point with context matrix into screen space.
 */
// FUNCTION: FF7 0x0067d0cb
int __cdecl ProjectPointWithContextMatrix(int point, int dst, int scale)
{
    GfxEngineContext *ctx;
    int visible;

    visible = 0;
    ctx = GetEngineContext();
    if (ctx != 0) {
        visible = ProjectPointWithMatrix(ctx->cameraMatrix, point, dst, scale);
    }
    return visible;
}

/*
 * Transforms point with context built matrix and projects to screen.
 */
// FUNCTION: FF7 0x0067d10e
int __cdecl ProjectPointWithContextBuiltMatrix(int point, struct GfxHomogeneousVec4 *dst, int scale)
{
    GfxEngineContext *ctx;
    int visible;

    visible = 0;
    ctx = GetEngineContext();
    if (ctx != 0) {
        visible = ProjectPointWithBuiltMatrix(ctx->cameraMatrix, point, dst, scale);
    }
    return visible;
}

/*
 * Composes input matrix with cached device transform states.
 */
// FUNCTION: FF7 0x0067d151
int __cdecl ComposeInputWithCachedTransforms(int matrix, void *dst)
{
    int projection;
    int view;
    GfxPolygonSetRecord *polySet;
    GfxEngineContext *ctx;
    int flag;
    int modelView[16];

    flag = 0;
    ctx = GetEngineContext();
    if (ctx != 0 && ctx->opaqueSet != 0) {
        polySet = (GfxPolygonSetRecord *)ctx->opaqueSet->matrices;
        view = GetCachedDeviceTransform(GFX_MATRIX_SLOT_VIEW, polySet);
        projection = GetCachedDeviceTransform(GFX_MATRIX_SLOT_PROJECTION, polySet);
        if (view != 0 && projection != 0) {
            ConcatAffineMatrix4x4((float(*)[4])(matrix), (float(*)[4])(view), (float(*)[4])(modelView));
            ClearMatrixColumn4(modelView);
            MultiplyMatrix4x4Full(modelView, (void *)projection, dst);
            flag = 1;
        }
    }
    return flag;
}

/*
 * Builds combined world-view-projection screen transform matrix.
 */
// FUNCTION: FF7 0x0067d1fb
int __cdecl BuildScreenTransformMatrix(void *dst)
{
    int projection;
    int viewportBuf[16];
    int viewProj[16];
    GfxEngineContext *ctx;
    GfxPolygonSetRecord *polySet;
    int view;
    int flag;

    flag = 0;
    ctx = GetEngineContext();
    if (ctx != 0 && ctx->opaqueSet != 0) {
        polySet = (GfxPolygonSetRecord *)ctx->opaqueSet->matrices;
        view = GetCachedDeviceTransform(GFX_MATRIX_SLOT_VIEW, polySet);
        projection = GetCachedDeviceTransform(GFX_MATRIX_SLOT_PROJECTION, polySet);
        if (view != 0 && projection != 0) {
            MultiplyMatrix4x4Full((void *)view, (void *)projection, viewProj);
            BuildViewportMatrix((struct GfxViewportMatrix *)(viewportBuf), ctx);
            MultiplyMatrix4x4Full(viewProj, viewportBuf, dst);
            flag = 1;
        }
    }
    return flag;
}

/*
 * Multiplies input matrix with context screen transform matrix.
 */
// FUNCTION: FF7 0x0067d2bf
void __cdecl ComposeWithScreenMatrix(void *out, void *context)
{
    MultiplyMatrix4x4ByTranspose(out, GetEngineContext()->screenTransformTransposed, context);
}

/*
 * Calculates 3x3 determinant minor of a 4x4 matrix.
 */
// FUNCTION: FF7 0x0067d2df
float __cdecl Matrix4x4Minor(int skipRow, int skipCol, GfxMatrix4x4 *matrix)
{
    float *rowPtrB;
    float *rowFirst;
    int rCol;
    int col0;
    int colP;
    float *rowThird;
    float determinant;

    determinant = 0.0f;
    /* 1-based matrix column index (1..4). */
    switch (skipCol) {
    case 1:
        col0 = 1;
        colP = 2;
        rCol = 3;
        break;
    case 2:
        col0 = 0;
        colP = 2;
        rCol = 3;
        break;
    case 3:
        col0 = 0;
        colP = 1;
        rCol = 3;
        break;
    case 4:
        col0 = 0;
        colP = 1;
        rCol = 2;
    }
    /* 1-based matrix row index (1..4). */
    switch (skipRow) {
    case 1:
        rowFirst = &matrix->m10;
        rowPtrB = &matrix->m20;
        rowThird = &matrix->m30;
        break;
    case 2:
        rowFirst = &matrix->m00;
        rowPtrB = &matrix->m20;
        rowThird = &matrix->m30;
        break;
    case 3:
        rowFirst = &matrix->m00;
        rowPtrB = &matrix->m10;
        rowThird = &matrix->m30;
        break;
    case 4:
        rowFirst = &matrix->m00;
        rowPtrB = &matrix->m10;
        rowThird = &matrix->m20;
    }
    determinant = (rowPtrB[colP] * rowThird[rCol] - rowPtrB[rCol] * rowThird[colP]) * rowFirst[col0] + (rowFirst[rCol] * rowThird[colP] - rowFirst[colP] * rowThird[rCol]) * rowPtrB[col0] + (rowFirst[colP] * rowPtrB[rCol] - rowFirst[rCol] * rowPtrB[colP]) * rowThird[col0];
    return determinant;
}

/*
 * Calculates cofactor value for specified matrix row and column.
 */
// FUNCTION: FF7 0x0067d4a7
float __cdecl Matrix4x4Cofactor(int row, int column, GfxMatrix4x4 *matrix)
{
    float result;

    result = Matrix4x4Minor(row, column, matrix);
    if ((row + column) & 1) {
        result = -result;
    }
    return result;
}

/*
 * Calculates cofactor row vector for matrix inversion.
 */
// FUNCTION: FF7 0x0067d4de
void __cdecl Matrix4x4CofactorRow(int row, GfxMatrix4x4 *matrix, GfxMatrix4x4 *output)
{
    /* 1-based matrix row index (1..4). */
    switch (row) {
    case 1:
        output->m00 = Matrix4x4Cofactor(row, 1, matrix);
        output->m01 = Matrix4x4Cofactor(row, 2, matrix);
        output->m02 = Matrix4x4Cofactor(row, 3, matrix);
        output->m03 = Matrix4x4Cofactor(row, 4, matrix);
        break;
    case 2:
        output->m10 = Matrix4x4Cofactor(row, 1, matrix);
        output->m11 = Matrix4x4Cofactor(row, 2, matrix);
        output->m12 = Matrix4x4Cofactor(row, 3, matrix);
        output->m13 = Matrix4x4Cofactor(row, 4, matrix);
        break;
    case 3:
        output->m20 = Matrix4x4Cofactor(row, 1, matrix);
        output->m21 = Matrix4x4Cofactor(row, 2, matrix);
        output->m22 = Matrix4x4Cofactor(row, 3, matrix);
        output->m23 = Matrix4x4Cofactor(row, 4, matrix);
        break;
    case 4:
        output->m30 = Matrix4x4Cofactor(row, 1, matrix);
        output->m31 = Matrix4x4Cofactor(row, 2, matrix);
        output->m32 = Matrix4x4Cofactor(row, 3, matrix);
        output->m33 = Matrix4x4Cofactor(row, 4, matrix);
        break;
    }
}

/*
 * Multiplies all elements of a 4x4 matrix by scalar in-place.
 */
// FUNCTION: FF7 0x0067d6a4
void __cdecl Matrix4x4ScaleInPlace(float scale, GfxMatrix4x4 *matrix)
{
    matrix->m00 *= scale;
    matrix->m01 *= scale;
    matrix->m02 *= scale;
    matrix->m03 *= scale;
    matrix->m10 *= scale;
    matrix->m11 *= scale;
    matrix->m12 *= scale;
    matrix->m13 *= scale;
    matrix->m20 *= scale;
    matrix->m21 *= scale;
    matrix->m22 *= scale;
    matrix->m23 *= scale;
    matrix->m30 *= scale;
    matrix->m31 *= scale;
    matrix->m32 *= scale;
    matrix->m33 *= scale;
}

/*
 * Divides all elements of a 4x4 matrix by scalar value.
 */
// FUNCTION: FF7 0x0067d797
void __cdecl Matrix4x4DivideByScalar(float scale, void *matrix)
{
    float reciprocal;

    if (scale != 0.0f) {
        reciprocal = 1.0f / scale;
        Matrix4x4ScaleInPlace(reciprocal, (struct GfxMatrix4x4 *)matrix);
    }
}

/*
 * Computes dot product of two 4D vectors.
 */
// FUNCTION: FF7 0x0067d7cb
float __cdecl Vector4Dot(GfxHomogeneousVec4 *vecA, GfxHomogeneousVec4 *vecB)
{
    float dot;

    dot = vecA->x * vecB->x + vecA->y * vecB->y + vecA->z * vecB->z + vecA->w * vecB->w;
    return dot;
}

#pragma optimize("p", on)

/*
 * Computes the inverse of a 4x4 matrix using cofactor expansion.
 */
// FUNCTION: FF7 0x0067d80a
int __cdecl Matrix4x4Invert(int matrix, void *output)
{
    struct {
        float determinant;
        GfxMatrix4x4 matrix;
        int success;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    frame.success = 0;
    Matrix4x4CofactorRow(1, (GfxMatrix4x4 *)matrix, &frame.matrix);
    frame.determinant = Vector4Dot((GfxHomogeneousVec4 *)matrix, (GfxHomogeneousVec4 *)&frame.matrix);
    if (frame.determinant != 0.0f) {
        Matrix4x4CofactorRow(2, (GfxMatrix4x4 *)matrix, &frame.matrix);
        Matrix4x4CofactorRow(3, (GfxMatrix4x4 *)matrix, &frame.matrix);
        Matrix4x4CofactorRow(4, (GfxMatrix4x4 *)matrix, &frame.matrix);
        TransposeMatrix4x4((GfxTransposedMatrix *)&frame.matrix, (GfxTransposedMatrix *)output);
        Matrix4x4DivideByScalar(frame.determinant, output);
        frame.success = 1;
    }
    return frame.success;
}
#pragma optimize("p", off)

/*
 * Diagnostic probe checking projection matrix validity.
 */
// FUNCTION: FF7 0x0067d8b0
void __cdecl Gfx_ProbeProjectionMatrix(GfxEngineContext *viewport)
{
    GfxMatrix4x4 *productMtx;
    float origin[3];
    float clipVec[4];
    float bytesY;
    GfxMatrix4x4 *pCombinedMatrix;
    float distances[3];
    float widthF;
    float inputVector[4];
    GfxMatrix4x4 *translateMatrixPtr;
    float offsetY;
    GfxMatrix4x4 *pOriginMatrix;
    float ofsX;

    ofsX = 0.0f;
    offsetY = 0.0f;
    widthF = (float)viewport->viewSizeX;
    bytesY = (float)viewport->viewSizeX;
    origin[0] = 0.0f;
    origin[1] = 0.0f;
    origin[2] = 0.0f;
    distances[0] = 10.0f;
    distances[1] = 100.0f;
    distances[2] = 1000.0f;
    pOriginMatrix = (GfxMatrix4x4 *)Gfx_AllocIdentityMatrix4x4();
    translateMatrixPtr = (GfxMatrix4x4 *)Gfx_AllocIdentityMatrix4x4();
    pCombinedMatrix = (GfxMatrix4x4 *)Gfx_AllocIdentityMatrix4x4();
    productMtx = (GfxMatrix4x4 *)Gfx_AllocIdentityMatrix4x4();
    if (pOriginMatrix != 0 && translateMatrixPtr != 0 && pCombinedMatrix != 0 && productMtx != 0) {
        BuildTranslationMatrixRowVec(origin, pOriginMatrix);
        BuildTranslationMatrixRowVec(distances, translateMatrixPtr);
        MultiplyMatrix4x4Full((void *)pOriginMatrix, (void *)translateMatrixPtr, (void *)pCombinedMatrix);
        MultiplyMatrix4x4Full((void *)pCombinedMatrix, (void *)translateMatrixPtr, (void *)productMtx);
        SetIdentityMatrix4x4((void *)pCombinedMatrix);
        BuildProjectionMatrix(GetViewportAspectRatio(viewport), 30.0f, 10.0f, 1000.0f, ofsX, offsetY, widthF, bytesY, (float(*)[4])((int)pCombinedMatrix));
        BuildProjectionMatrix(GetViewportAspectRatio(viewport), 90.0f, 100.0f, 1000.0f, ofsX, offsetY, widthF, bytesY, (float(*)[4])((int)pCombinedMatrix));
        inputVector[0] = 10.0f;
        inputVector[1] = 10.0f;
        inputVector[2] = 999.0f;
        inputVector[2] = 1000.0f;
        inputVector[0] = 20.0f;
        inputVector[1] = 20.0f;
        inputVector[2] = 200.0f;
        inputVector[3] = 1.0f;
        TransformVector4ByMatrix4x4((struct GfxMatrix4x4 *)pCombinedMatrix, (struct GfxHomogeneousVec4 *)(inputVector), (struct GfxHomogeneousVec4 *)(clipVec));
        PerspectiveDivideInPlace((struct GfxHomogeneousVec4 *)(clipVec));
        inputVector[0] = 10.0f;
        inputVector[1] = 10.0f;
        inputVector[2] = 10.0f;
        inputVector[0] = 20.0f;
        inputVector[1] = 20.0f;
        inputVector[2] = 799.0f;
        inputVector[3] = 1.0f;
        TransformVector4ByMatrix4x4((struct GfxMatrix4x4 *)pCombinedMatrix, (struct GfxHomogeneousVec4 *)(inputVector), (struct GfxHomogeneousVec4 *)(clipVec));
        PerspectiveDivideInPlace((struct GfxHomogeneousVec4 *)(clipVec));
        inputVector[0] = 10.0f;
        inputVector[1] = 10.0f;
        inputVector[2] = 5.0f;
        inputVector[3] = 1.0f;
        TransformVector4ByMatrix4x4((struct GfxMatrix4x4 *)pCombinedMatrix, (struct GfxHomogeneousVec4 *)(inputVector), (struct GfxHomogeneousVec4 *)(clipVec));
        PerspectiveDivideInPlace((struct GfxHomogeneousVec4 *)(clipVec));
    }
    Gfx_FreeMatrix4x4(productMtx);
    Gfx_FreeMatrix4x4(pCombinedMatrix);
    Gfx_FreeMatrix4x4(translateMatrixPtr);
    Gfx_FreeMatrix4x4(pOriginMatrix);
}
} /* extern "C" */
