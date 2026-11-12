// src/lib/src/graphics/light.cpp
// Original: C:\lib\src\graphics\light.cpp
// Contents: Light records and color conversion: packs and unpacks BGRA colors to and from floats,
//   creates and frees light records, and builds direction and color matrices and specular ramps.
//   The rest of the file applies these matrices to model vertices and cached products.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/api/mem.h"
#include <string.h>
#include "lib/crt.h"
#include "lib/api/graphics.h"
#include "lib/types/graphics.h"
#include "lib/types/polygon.h"
#include "lib/codegen.h"
extern "C" {

#pragma data_seg(".u69str")
#pragma data_seg(".u69ptr")
#pragma data_seg()

/*
 * Unpacks packed 32-bit BGRA color bytes into an array of four unscaled floats.
 */
// FUNCTION: FF7 0x0069bf90
void __cdecl Gfx_UnpackBgraToFloat4(int color, float *out)
{
    int red;
    int green;
    int blue;
    int alphaByte;
    red = *(int *)((char *)&color + 2) & 255;
    out[2] = red;
    green = *(int *)((char *)&color + 1) & 255;
    out[1] = green;
    blue = color & 255;
    out[0] = blue;
    alphaByte = *(int *)((char *)&color + 3) & 255;
    out[3] = alphaByte;
}

/*
 * Converts four unscaled float color channels into 32-bit ARGB8888 byte format.
 */
// FUNCTION: FF7 0x0069bfeb
void __cdecl Blit_PackBgraFloatsToArgb8888Unscaled(unsigned char *destination, float *source)
{
    destination[2] = (int)source[2];
    destination[1] = (int)source[1];
    destination[0] = (int)source[0];
    destination[3] = (int)source[3];
}

/*
 * Scales four normalized float color channels by 255 into 32-bit ARGB8888.
 */
// FUNCTION: FF7 0x0069c032
void __cdecl Blit_PackBgraFloatsToArgb8888(unsigned char *destination, float *source)
{
    destination[2] = (int)(source[2] * 255.0f);
    destination[1] = (int)(source[1] * 255.0f);
    destination[0] = (int)(source[0] * 255.0f);
    destination[3] = (int)(source[3] * 255.0f);
}

/*
 * Unpacks 32-bit BGRA color into floats with alpha forced to 255.0f.
 */
// FUNCTION: FF7 0x0069c091
void __cdecl Gfx_UnpackBgraToFloat4OpaqueAlpha(int color, float *out)
{
    int red;
    int green;
    int blue;
    red = *(int *)((char *)&color + 2) & 255;
    out[2] = red;
    green = *(int *)((char *)&color + 1) & 255;
    out[1] = green;
    blue = color & 255;
    out[0] = blue;
    out[3] = 255.0f;
}

/*
 * Packs RGB floats into ARGB8888 bytes with alpha forced to 0xFF.
 */
// FUNCTION: FF7 0x0069c0e2
void __cdecl Blit_PackBgraFloatsToArgb8888UnscaledOpaque(unsigned char *destination, float *source)
{
    destination[2] = (int)source[2];
    destination[1] = (int)source[1];
    destination[0] = (int)source[0];
    destination[3] = 0xFF;
}

/*
 * Normalizes four 0-255 float color channels to 0.0-1.0 range in place.
 */
// FUNCTION: FF7 0x0069c11f
void __cdecl Gfx_LightNormalizeColorInPlace(GfxNormalizedColor *color)
{
    color->colorB *= 0.003921569f;
    color->colorG *= 0.003921569f;
    color->colorR *= 0.003921569f;
    color->colorA *= 0.003921569f;
}

/*
 * Normalizes four float color channels to 0.0-1.0 and copies to destination.
 */
// FUNCTION: FF7 0x0069c16a
void __cdecl Gfx_LightNormalizeColorCopy(GfxNormalizedColor *src, GfxFloatColor *dst)
{
    dst->r = src->colorB * 0.003921569f;
    dst->g = src->colorG * 0.003921569f;
    dst->b = src->colorR * 0.003921569f;
    dst->a = src->colorA * 0.003921569f;
}

/*
 * Frees an individual light record structure using the game heap.
 */
// FUNCTION: FF7 0x0069c1b5
void __cdecl Gfx_LightFreeRecord(void *record)
{
    if (record != 0) {
        // STRING: FF7 0x0091212c
        GameHeapFree(record, "C:\\lib\\src\\graphics\\light.cpp", 0x49);
    }
}

/*
 * Allocates memory for a single light record from the game heap.
 */
// FUNCTION: FF7 0x0069c1d3
int __cdecl Gfx_LightAllocRecord(void)
{
    GfxLight *rec;

    // STRING: FF7 0x0091214c
    rec = (GfxLight *)GameHeapAlloc(1, 0x24, "C:\\lib\\src\\graphics\\light.cpp", 0x51);
    return (int)rec;
}

/*
 * Creates a light record with specified packed BGRA color and direction vector.
 */
// FUNCTION: FF7 0x0069c1f4
int __cdecl Gfx_LightCreate(int color, void *direction)
{
    GfxLight *light;

    light = (GfxLight *)Gfx_LightAllocRecord();
    if (light != 0) {
        light->clearedFlags = 0;
        light->color = color;
        *(GfxDirectionVec3 *)&light->dirX = *(GfxDirectionVec3 *)(direction);
        Gfx_UnpackBgraToFloat4(color, (float *)&light->colorB);
        Gfx_LightNormalizeColorInPlace((GfxNormalizedColor *)&light->colorB);
    }
    return (int)light;
}

/*
 * Builds a 4x4 matrix with three light direction vectors as rows.
 */
// FUNCTION: FF7 0x0069c25a
void __cdecl Gfx_LightBuildDirectionMatrix(GfxLightSource2 *dir0, GfxLightSource2 *dir1, GfxLightSource2 *dir2, GfxLightDirectionMatrix *dirMatrix)
{
    SetIdentityMatrix4x4(dirMatrix);
    if (dir0 != 0) {
        dirMatrix->dir0X = dir0->dirX;
        dirMatrix->dir0Y = dir0->dirY;
        dirMatrix->dir0Z = dir0->dirZ;
    }
    if (dir1 != 0) {
        dirMatrix->dir1X = dir1->dirX;
        dirMatrix->dir1Y = dir1->dirY;
        dirMatrix->dir1Z = dir1->dirZ;
    }
    if (dir2 != 0) {
        dirMatrix->dir2X = dir2->dirX;
        dirMatrix->dir2Y = dir2->dirY;
        dirMatrix->dir2Z = dir2->dirZ;
    }
}

/*
 * Builds a 4x4 matrix with three light color vectors as columns.
 */
// FUNCTION: FF7 0x0069c2e8
void __cdecl Gfx_LightBuildColorMatrix(GfxLightSource2 *color0, GfxLightSource2 *color1, GfxLightSource2 *color2, GfxLightColorMatrix *colorMatrix)
{
    SetIdentityMatrix4x4(colorMatrix);
    if (color0 != 0) {
        colorMatrix->m00 = color0->colorR;
        colorMatrix->m10 = color0->colorG;
        colorMatrix->m20 = color0->colorB;
    }
    if (color1 != 0) {
        colorMatrix->m01 = color1->colorR;
        colorMatrix->m11 = color1->colorG;
        colorMatrix->m21 = color1->colorB;
    }
    if (color2 != 0) {
        colorMatrix->m02 = color2->colorR;
        colorMatrix->m12 = color2->colorG;
        colorMatrix->m22 = color2->colorB;
    }
}

/*
 * Multiplies two 4x4 matrices into a destination matrix.
 */
// FUNCTION: FF7 0x0069c376
void __cdecl Gfx_LightMultiplyMatrices(void *lhs, void *rhs, void *dst)
{
    MultiplyMatrix4x4Core(lhs, rhs, dst);
}

/*
 * Multiplies two matrices, normalizes result, and writes ambient color to column 4.
 */
// FUNCTION: FF7 0x0069c38f
void __cdecl Gfx_LightMultiplyMatricesWithAmbient(void *lhs, int rhs, GfxAmbientColor *ambient, GfxLightMatrixTransposed *dst)
{
    MultiplyMatrix4x4Core(lhs, (void *)rhs, dst);
    ClearMatrixRow4((struct GfxMatrixRow4 *)dst);
    dst->ambientThird = ambient->ambientThird;
    dst->ambientSecond = ambient->ambientSecond;
    dst->ambientFirst = ambient->ambientFirst;
}

/*
 * Multiplies rotation matrix by transpose, normalizes, and sets ambient color.
 */
// FUNCTION: FF7 0x0069c3d7
void __cdecl Gfx_LightMultiplyTransposedWithAmbient(GfxLightMatrixA *rotation, PolygonTransform *transform, GfxAmbientColor *ambient, GfxLightMatrixTransposed *dst)
{
    MultiplyRotation3x3ByTranspose(rotation, transform, (struct GfxMatrix3Row *)dst);
    ClearMatrixRow4((struct GfxMatrixRow4 *)dst);
    dst->ambientThird = ambient->ambientThird;
    dst->ambientSecond = ambient->ambientSecond;
    dst->ambientFirst = ambient->ambientFirst;
}

/*
 * Builds lighting records for model vertices from up to three active lights.
 */
// FUNCTION: FF7 0x0069c41f
int __cdecl Gfx_LightBuildModelVertexRecords(GfxLightRange *range, PolygonFileArrays *source)
{
    int result;
    int colorBase;
    int elemCount;
    int faceIndex;
    int packedColor;
    GfxLightVertexRecord *vertRecord;
    int vertexIndex;
    int triangle;
    int k;
    int normalAddress;
    int normalIndex;
    int count;
    int ofs;

    result = 0;
    if (source != 0) {
        if (range != 0) {
            colorBase = range->colorBase;
            ofs = range->offset;
            elemCount = range->elemCount;
            count = range->count;
        } else {
            colorBase = 0;
            ofs = 0;
            elemCount = source->vertexCount;
            count = source->polygonCount;
        }
        // STRING: FF7 0x0091216c
        result = (int)GameHeapAlloc(elemCount, 0x18, "C:\\lib\\src\\graphics\\light.cpp", 0xF1);
        if (result != 0) {
            for (faceIndex = 0; faceIndex < count; faceIndex++) {
                triangle = source->polygons + (ofs + faceIndex) * 0x18;
                for (k = 0; k < 3; k++) {
                    vertexIndex = *(unsigned short *)((char *)triangle + k * 2 + 2);
                    vertRecord = (GfxLightVertexRecord *)(result + vertexIndex * 0x18);
                    if (source->normals != 0) {
                        normalIndex = *(unsigned short *)((char *)triangle + k * 2 + 8);
                        normalAddress = source->normals + normalIndex * 0xC;
                        if (vertRecord->tag == 0) {
                            vertRecord->tag = normalAddress;
                            packedColor = source->vertexColorWords[colorBase + vertexIndex];
                            vertRecord->packedColor = packedColor;
                            Gfx_UnpackBgraToFloat4(vertRecord->packedColor, (float *)((int)vertRecord + 4));
                        } else if (normalAddress != vertRecord->tag && 0 != 0) {
                            // STRING: FF7 0x0091218c
                            Gfx_DebugPrint("VERTEX NORMAL CONFLICT \n");
                        }
                    } else {
                        vertRecord->tag = 0;
                        packedColor = source->vertexColorWords[colorBase + vertexIndex];
                        vertRecord->packedColor = packedColor;
                        Gfx_UnpackBgraToFloat4(vertRecord->packedColor, (float *)((int)vertRecord + 4));
                    }
                }
            }
        }
    }
    return result;
}

/*
 * Rebuilds cached direction, color, and product matrices for a light context.
 */
// FUNCTION: FF7 0x0069c5ee
void __cdecl Gfx_LightRebuildCachedMatrices(GfxLightOwner *owner)
{
    GfxLightSource2 *firstLight;
    GfxLightSource2 *lightB;
    GfxLightSource2 *thirdLight;

    if (owner != 0) {
        firstLight = (GfxLightSource2 *)owner->light0;
        lightB = (GfxLightSource2 *)owner->light1;
        thirdLight = (GfxLightSource2 *)owner->light2;
        Gfx_LightBuildDirectionMatrix(firstLight, lightB, thirdLight, (GfxLightDirectionMatrix *)owner->directionMatrix);
        Gfx_LightBuildColorMatrix((GfxLightSource2 *)firstLight, (GfxLightSource2 *)lightB, (GfxLightSource2 *)thirdLight, (GfxLightColorMatrix *)owner->colorMatrix);
        Gfx_LightMultiplyMatrices(owner->colorMatrix, owner->directionMatrix, owner->productMatrix);
    }
}

/*
 * Transforms cached light matrix by transform and updates ambient component.
 */
// FUNCTION: FF7 0x0069c66f
void __cdecl Gfx_LightApplyMatrixToCachedProduct(int transform, GfxLightOwner *owner)
{
    if (owner != 0) {
        Gfx_LightMultiplyMatricesWithAmbient((void *)owner->productMatrix, transform, (GfxAmbientColor *)owner->ambientFloats, (GfxLightMatrixTransposed *)owner->lightMatrix);
    }
}

/*
 * Transforms cached light matrix by transposed transform with ambient lighting.
 */
// FUNCTION: FF7 0x0069c69f
void __cdecl Gfx_LightApplyTransposedMatrixToCachedProduct(PolygonTransform *transform, GfxLightOwner *owner)
{
    if (owner != 0) {
        Gfx_LightMultiplyTransposedWithAmbient((GfxLightMatrixA *)owner->productMatrix, transform, (GfxAmbientColor *)owner->ambientFloats, (GfxLightMatrixTransposed *)owner->lightMatrix);
    }
}

/*
 * Frees a light owner container and its constituent light records.
 */
// FUNCTION: FF7 0x0069c6cf
void __cdecl Gfx_LightFreeOwner(int what, GfxLightOwner *handle)
{
    int i;
    if (handle != 0) {
        if (what != 0) {
            for (i = 0; i < handle->maxLights; i++) {
                Gfx_LightFreeRecord((void *)(&handle->light0)[i]);
            }
        }
        // STRING: FF7 0x009121a8
        GameHeapFree(handle, "C:\\lib\\src\\graphics\\light.cpp", 0x164);
    }
}

/*
 * Allocates and initializes a light owner with up to three child lights.
 */
// FUNCTION: FF7 0x0069c72b
int __cdecl Gfx_LightCreateOwner(GfxLight *light0, GfxLight *light1, GfxLight *light2, int color)
{
    GfxLightOwner *owner;

    // STRING: FF7 0x009121c8
    owner = (GfxLightOwner *)GameHeapAlloc(1, 0x5AC, "C:\\lib\\src\\graphics\\light.cpp", 0x16C);
    if (owner != 0) {
        owner->flags = 9;
        owner->maxLights = 3;
        owner->light0 = light0;
        owner->light1 = light1;
        owner->light2 = light2;
        owner->color = color;
        Gfx_UnpackBgraToFloat4(color, owner->colorFloats);
        Gfx_LightNormalizeColorCopy((GfxNormalizedColor *)owner->colorFloats, (GfxFloatColor *)owner->ambientFloats);
        Gfx_LightRebuildCachedMatrices(owner);
        owner->lightMatrixEnabled = 0;
        SetIdentityMatrix4x4(owner->identityMatrix);
        owner->specularColor = 0xFF000000;
    }
    return (int)owner;
}

/*
 * Applies lighting calculations to a single polygon set entry.
 */
// FUNCTION: FF7 0x0069c7f4
void __cdecl Gfx_LightPolygonSetCallFromEntry(int entry, GfxLightOwner *owner)
{
    if (entry != 0 && owner != 0) {
        Gfx_LightPolygonSetCall((PolygonSkeletonFrameRef *)(*(int *)((char *)entry + 4)), owner);
    }
}

/*
 * Applies lighting calculations across an array of polygon set entries.
 */
// FUNCTION: FF7 0x0069c818
void __cdecl Gfx_LightPolygonSetListCall(GfxLightEntryList *list, GfxLightOwner *owner)
{
    int index;

    if (list != 0 && owner != 0) {
        for (index = 0; index < list->count; index++) {
            Gfx_LightPolygonSetCallFromEntry(*(int *)((char *)list->entries + index * 4), owner);
        }
    }
}

/*
 * Stores specular depth matrix pointer into a light owner record.
 */
// FUNCTION: FF7 0x0069c863
void __cdecl Gfx_LightSetSpecularDepthMatrix(int matrix, GfxLightOwner *owner)
{
    if (owner != 0) {
        owner->deviceTransform = matrix;
    }
}

/*
 * Resets specular mode to 0 and specular color to default opaque black.
 */
// FUNCTION: FF7 0x0069c87a
void __cdecl Gfx_LightResetSpecular(GfxLightOwner *owner)
{
    if (owner != 0) {
        owner->specularMode = 0;
        owner->specularColor = 0xFF000000;
    }
}

/*
 * Configures specular mode 1 and near color value in a light owner record.
 */
// FUNCTION: FF7 0x0069c89f
void __cdecl Gfx_LightSetSpecularNearColor(int nearColor, GfxLightOwner *owner)
{
    if (owner != 0) {
        owner->specularMode = 1;
        owner->specularColor = nearColor;
    }
}

/*
 * Stores cached depth value into a light owner specular structure.
 */
// FUNCTION: FF7 0x0069c8c3
void __cdecl Gfx_LightSetSpecularCachedDepth(int depth, GfxLightOwner *owner)
{
    if (owner != 0) {
        owner->cachedDepth = depth;
    }
}

/*
 * Builds specular ramp table and sets specular mode 2 in light record.
 */
// FUNCTION: FF7 0x0069c8da
void __cdecl Gfx_LightSetSpecularCachedRamp(int depth, float rampStart, float rampEnd, int depthMatrix, int color, GfxLightOwner *owner)
{
    if (owner != 0) {
        Gfx_LightBuildSpecularRamp(rampStart, rampEnd, depthMatrix, color, owner);
        owner->rampMode = 2;
        owner->rampDepth = depth;
    }
}

/*
 * Generates a 256-entry lookup table for specular highlight falloff.
 */
// FUNCTION: FF7 0x0069c91a
void __cdecl Gfx_LightBuildSpecularRamp(float rampStart, float rampEnd, int depthMatrix, unsigned int color, GfxLightOwner *owner)
{
    float fraction;
    unsigned char *lut;
    int index;
    if (owner != 0) {
        owner->rampMode = 3;
        owner->rampColor = color;
        owner->rampStart = rampStart;
        owner->rampEnd = rampEnd;
        owner->rampSpan = owner->rampEnd - owner->rampStart;
        owner->invRampSpan = 1.0f / owner->rampSpan;
        owner->depthMatrix = depthMatrix;
        lut = &owner->rampLut;
        for (index = 0; index < 256; index++) {
            fraction = (float)index / 255.0f;
            lut[2] = (unsigned char)(((unsigned char *)&color)[2] * fraction);
            lut[1] = (unsigned char)(((unsigned char *)&color)[1] * fraction);
            lut[0] = (unsigned char)(((unsigned char *)&color)[0] * fraction);
            lut[3] = 255;
            lut += 4;
        }
        owner->rampScale = 255.0f / owner->rampSpan;
    }
}

/*
 * Creates three directional lights from their direction vectors and BGRA colours and bundles them
 * into a light owner with the ambient colour.
 */
// FUNCTION: FF7 0x0069ca53
int __cdecl Gfx_LightCreateThreeLightOwner(void *dir0, void *dir1, void *dir2, int color0, int color1, int color2, int ambient)
{
    GfxLight *firstLight;
    GfxLight *calcLight;
    GfxLight *thirdLight;
    GfxLightOwner *owner;

    firstLight = (GfxLight *)Gfx_LightCreate(color0, dir0);
    calcLight = (GfxLight *)Gfx_LightCreate(color1, dir1);
    thirdLight = (GfxLight *)Gfx_LightCreate(color2, dir2);
    owner = (GfxLightOwner *)Gfx_LightCreateOwner(firstLight, calcLight, thirdLight, ambient);
    if (owner != 0) {
        Gfx_LightResetSpecular(owner);
    }
    return (int)owner;
}

/*
 * Creates the default three-light directional setup with standard ambient color.
 */
// FUNCTION: FF7 0x0069cac6
int __cdecl Gfx_LightCreateDefaultThreeLightSet(void)
{
    struct {
        struct GfxEngineContext *context;
        unsigned int color;
        float direction1[3], direction2[3];
        int result;
        float direction0[3];
        unsigned int color0, ambient, color2, color1;
    } f; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    f.direction0[0] = 0.30322265625f;
    f.direction0[1] = 0.330078125f;
    f.direction0[2] = 0.896240234375f;
    f.direction1[0] = 0.09375f;
    f.direction1[1] = 0.274658203125f;
    f.direction1[2] = -0.958251953125f;
    f.direction2[0] = -0.72119140625f;
    f.direction2[1] = -0.357177734375f;
    f.direction2[2] = -0.599853515625f;
    f.color0 = -1;
    f.color1 = -1;
    f.color2 = -1;
    ((unsigned char *)&f.color0)[2] = 0xAB;
    ((unsigned char *)&f.color0)[1] = 0x70;
    ((unsigned char *)&f.color0)[0] = 0x71;
    ((unsigned char *)&f.color1)[2] = 0x6E;
    ((unsigned char *)&f.color1)[1] = 0x39;
    ((unsigned char *)&f.color1)[0] = 0x3B;
    ((unsigned char *)&f.color2)[2] = 0x4D;
    ((unsigned char *)&f.color2)[1] = 0x4D;
    ((unsigned char *)&f.color2)[0] = 0x4D;
    ((unsigned char *)&f.ambient)[2] = 0x74;
    ((unsigned char *)&f.ambient)[1] = 0x73;
    ((unsigned char *)&f.ambient)[0] = 0x72;
    ((unsigned char *)&f.ambient)[3] = 0xFF;
    f.result = Gfx_LightCreateThreeLightOwner((void *)f.direction0, (void *)f.direction1, (void *)f.direction2, f.color0, f.color1, f.color2, f.ambient);
    if (f.result != 0) {
        f.context = GetEngineContext();
        f.color = 0xFF808080;
        Gfx_LightBuildSpecularRamp(f.context->nearPlane, f.context->farPlane, f.context->cameraMatrix, f.color, (GfxLightOwner *)f.result);
    }
    return f.result;
}

// Native definition of Gfx_TextureCacheLookupSurface is in texcache.c.

// Native definition of Gfx_TextureCacheFindSurface is in texcache.c.

// Native definition of Gfx_TextureCacheReleaseAllSurfaces is in texcache.c.

// Native definition of Gfx_TextureCacheReleaseByTexture is in texcache.c.

// Native definition of Gfx_TextureCacheReleaseEntry is in texcache.c.

// Native definition of Gfx_TextureCacheReleaseAllSurfacesMember is in texcache.c.
} /* extern "C" */
