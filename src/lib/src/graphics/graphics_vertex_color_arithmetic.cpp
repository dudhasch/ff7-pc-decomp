// src/lib/src/graphics/graphics_vertex_color_arithmetic.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Vertex lighting and color arithmetic: it transforms and lights vertices, adds and
//   modulates float colors, packs them to BGRA, and walks or saves and restores group and
//   polygon-set vertex colors.
/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
// Also: Applies lighting and color operations to polygon sets: cached specular color, ambient
//   and directional lighting, and scaling, blending and masking group vertex colors from saved
//   buffers or palette entries.
#include "lib/types/graphics.h"
#include "lib/api/graphics.h"
#include "lib/types/mem.h"
#include "lib/enums/graphics.h"

extern "C" {
// The optimize("p") pairs in this file set /Op per function, as the retail build did (lib/codegen.h, "/Op pairs").
#pragma optimize("p", off)

/*
 * Traverses vertices in a polygon group without modifying values.
 */
// FUNCTION: FF7 0x0068d6f0
void __cdecl Gfx_UnusedNoOpGroupWalk(int groupIndex, GfxPolygonSet *polygonSet)
{
    GfxEngineContext *ctx;
    int source;
    GfxGroupWalkHeader *group;
    int i;
    int vertex;

    ctx = GetEngineContext();
    group = (GfxGroupWalkHeader *)(Gfx_LockPolygonSetGroup(groupIndex, polygonSet, ctx));
    if (group != 0) {
        source = group->source;
        if (source != 0) {
            vertex = group->vertices;
            for (i = 0; i < group->count; i++) {
                source += 0x18;
                vertex += 0x20;
            }
        }
        Gfx_UnlockPolygonSetGroup(groupIndex, polygonSet, ctx);
    }
}
#pragma optimize("p", on)
#pragma optimize("p", off)

/*
 * Iterates across polygon set groups without performing modifications.
 */
// FUNCTION: FF7 0x0068d77c
void __cdecl Gfx_UnusedNoOpPolygonSetWalk(GfxPolygonSet *polygonSet)
{
    int groupIndex;

    if (polygonSet != 0) {
        for (groupIndex = 0; groupIndex < polygonSet->groupCount; groupIndex++) {
            Gfx_UnusedNoOpGroupWalk(groupIndex, polygonSet);
        }
    }
}
#pragma optimize("p", on)

/*
 * Adds float RGB color channels to vertex BGR channels with saturation.
 */
// FUNCTION: FF7 0x0068d7b9
void __cdecl Gfx_AddFloatColorsToVertexBgr(GfxFloatColorAddend *colorA, GfxFloatColorAddend *colorB, GfxVertexBgr *vertexColor)
{
    struct {
        float b, g, r;
        int q;
    } colorTemp;
    colorTemp.r = colorB->red + colorA->red;
    if (colorTemp.r < 0.0f)
        colorTemp.r = 0.0f;
    if (colorTemp.r > 1.0f)
        colorTemp.r = 1.0f;
    colorTemp.r = colorTemp.r * 255.0f;
    /* Retail rounds channel floats via bare x87 fistp throughout this TU. */
    __asm {fld colorTemp.r}
    __asm {fistp colorTemp.q} vertexColor->colorR = colorTemp.q;
    colorTemp.g = colorB->green + colorA->green;
    if (colorTemp.g < 0.0f)
        colorTemp.g = 0.0f;
    if (colorTemp.g > 1.0f)
        colorTemp.g = 1.0f;
    colorTemp.g = colorTemp.g * 255.0f;
    __asm {fld colorTemp.g}
    __asm {fistp colorTemp.q} vertexColor->colorG = colorTemp.q;
    colorTemp.b = colorB->blue + colorA->blue;
    if (colorTemp.b < 0.0f)
        colorTemp.b = 0.0f;
    if (colorTemp.b > 1.0f)
        colorTemp.b = 1.0f;
    colorTemp.b = colorTemp.b * 255.0f;
    __asm {fld colorTemp.b}
    __asm {fistp colorTemp.q} vertexColor->colorB = colorTemp.q;
}
#pragma optimize("p", off)

/*
 * Multiplies vertex color bytes by unit float color coefficients.
 */
// FUNCTION: FF7 0x0068d8ce
void __cdecl Gfx_ModulateUnitFloatColorsToBytes(GfxFloatColor *unitColor, GfxFloatColorAddend *floatColor, GlTexelDest *vertexColor)
{
    struct {
        float b, g, r;
        int q;
    } colorTemp;
    colorTemp.r = floatColor->red * unitColor->r * 255.0f;
    __asm {fld colorTemp.r}
    __asm {fistp colorTemp.q} vertexColor->red = colorTemp.q;
    colorTemp.g = floatColor->green * unitColor->g * 255.0f;
    __asm {fld colorTemp.g}
    __asm {fistp colorTemp.q} vertexColor->green = colorTemp.q;
    colorTemp.b = floatColor->blue * unitColor->b * 255.0f;
    __asm {fld colorTemp.b}
    __asm {fistp colorTemp.q} vertexColor->blue = colorTemp.q;
}
#pragma optimize("p", on)
#pragma optimize("p", off)

/*
 * Multiplies vertex colors by normalized float color factors.
 */
// FUNCTION: FF7 0x0068d947
void __cdecl Gfx_ModulateFloatColorsDiv255ToBytes(GfxFloatColor *normalizedColor, GfxLitLightColor *floatColor, GlTexelDest *vertexColor)
{
    struct {
        float b, g, r;
        int q;
    } colorTemp;
    colorTemp.r = floatColor->r * normalizedColor->r * 0.003921569f;
    __asm {fld colorTemp.r}
    __asm {fistp colorTemp.q} vertexColor->red = colorTemp.q;
    colorTemp.g = floatColor->g * normalizedColor->g * 0.003921569f;
    __asm {fld colorTemp.g}
    __asm {fistp colorTemp.q} vertexColor->green = colorTemp.q;
    colorTemp.b = floatColor->b * normalizedColor->b * 0.003921569f;
    __asm {fld colorTemp.b}
    __asm {fistp colorTemp.q} vertexColor->blue = colorTemp.q;
}
#pragma optimize("p", on)
#pragma optimize("p", off)

/*
 * Transforms vertex normal and scales color by directional lighting.
 */
// FUNCTION: FF7 0x0068d9c0
void __cdecl Gfx_TransformedColorScaleClamped(GfxFloorVertex *matrix, int normal, GfxFloatColorAddend *lightColor, GfxScaledColor *maxColor, GfxBgraPixel *vertexColor)
{
    struct {
        float b, g, r;
        int q;
        float v[3];
    } scratch;
    TransformPoint3ColVec((float *)matrix, (float *)normal, (float *)scratch.v);
    if (scratch.v[0] < 0.0f)
        vertexColor->red = 0;
    else if (scratch.v[0] >= 1.0f)
        vertexColor->red = maxColor->channel2;
    else {
        scratch.r = lightColor->red * scratch.v[0];
        __asm {fld scratch.r}
        __asm {fistp scratch.q} vertexColor->red = scratch.q;
    }
    if (scratch.v[1] < 0.0f)
        vertexColor->green = 0;
    else if (scratch.v[1] >= 1.0f)
        vertexColor->green = maxColor->channel1;
    else {
        scratch.g = lightColor->green * scratch.v[1];
        __asm {fld scratch.g}
        __asm {fistp scratch.q} vertexColor->green = scratch.q;
    }
    if (scratch.v[2] < 0.0f)
        vertexColor->blue = 0;
    else if (scratch.v[2] >= 1.0f)
        vertexColor->blue = maxColor->channel0;
    else {
        scratch.b = lightColor->blue * scratch.v[2];
        __asm {fld scratch.b}
        __asm {fistp scratch.q} vertexColor->blue = scratch.q;
    }
    vertexColor->alpha = maxColor->channel3;
}
#pragma optimize("p", on)

/*
 * Calculates diffuse lighting for a vertex and writes clamped BGR channels.
 */
// FUNCTION: FF7 0x0068dae1
void __cdecl Gfx_LightVertexDiffuse(GfxFloorVertex *matrix, int normal, GfxFloatColorAddend *lightColor, void *srcColor, GfxVertexColor *vertexColor)
{
    struct {
        float b, g, r;
        int q;
        float v[3];
    } scratch;
    TransformPoint3ColVec((float *)matrix, (float *)normal, (float *)scratch.v);
    if (scratch.v[0] < 0.0f)
        vertexColor->colorB = 0;
    else {
        scratch.r = lightColor->red * scratch.v[0];
        if (scratch.r >= 255.0f)
            vertexColor->colorB = 255;
        else {
            __asm {fld scratch.r}
            __asm {fistp scratch.q} vertexColor->colorB = scratch.q;
        }
    }
    if (scratch.v[1] < 0.0f)
        vertexColor->colorG = 0;
    else {
        scratch.g = lightColor->green * scratch.v[1];
        if (scratch.g >= 255.0f)
            vertexColor->colorG = 255;
        else {
            __asm {fld scratch.g}
            __asm {fistp scratch.q} vertexColor->colorG = scratch.q;
        }
    }
    if (scratch.v[2] < 0.0f)
        vertexColor->colorR = 0;
    else {
        scratch.b = lightColor->blue * scratch.v[2];
        if (scratch.b >= 255.0f)
            vertexColor->colorR = 255;
        else {
            __asm {fld scratch.b}
            __asm {fistp scratch.q} vertexColor->colorR = scratch.q;
        }
    }
    vertexColor->colorA = ((GfxBgraPixel *)srcColor)->alpha;
}

/*
 * Transforms vertex coordinate and applies directional lighting to BGRA color.
 */
// FUNCTION: FF7 0x0068dbf4
void __cdecl Gfx_TransformPointLitColorToBgra(int color, int colorScale, GfxFloorVertex *vertex, int matrix, GfxFloatColorAddend *lightColor, void *srcColor, GfxBgraPixel *dst)
{
    struct {
        float b, g, r;
        int q;
        float v[3];
    } scratch;
    TransformPoint3ColVec((float *)vertex, (float *)matrix, (float *)scratch.v);
    if (scratch.v[0] < 0.0f)
        dst->red = 0;
    else {
        scratch.r = lightColor->red * scratch.v[0];
        if (scratch.r >= 255.0f)
            dst->red = ((char *)&color)[2];
        else {
            __asm {mov eax,colorScale}
            __asm { fld scratch.r }
            __asm {fmul dword ptr [eax+8]}
            __asm {fistp scratch.q} dst->red = scratch.q;
        }
    }
    if (scratch.v[1] < 0.0f)
        dst->green = 0;
    else {
        scratch.g = lightColor->green * scratch.v[1];
        if (scratch.g >= 255.0f)
            dst->green = ((char *)&color)[1];
        else {
            __asm {mov eax,colorScale}
            __asm { fld scratch.g }
            __asm {fmul dword ptr [eax+4]}
            __asm {fistp scratch.q} dst->green = scratch.q;
        }
    }
    if (scratch.v[2] < 0.0f)
        dst->blue = 0;
    else {
        scratch.b = lightColor->blue * scratch.v[2];
        if (scratch.b >= 255.0f)
            dst->blue = ((char *)&color)[0];
        else {
            __asm {mov eax,colorScale}
            __asm { fld scratch.b }
            __asm {fmul dword ptr [eax+0]}
            __asm {fistp scratch.q} dst->blue = scratch.q;
        }
    }
    dst->alpha = ((GfxBgraPixel *)srcColor)->alpha;
}

/*
 * Calculates floor-lit point lighting and packs to BGRA color.
 */
// FUNCTION: FF7 0x0068dd1e
void __cdecl Gfx_TransformPointFloorColorToBgra(GfxFloorVertex *vertex, int matrix, GfxLitLightColor *lightColor, void *srcColor, GfxBgraPixel *dst)
{
    struct {
        float b, g, r;
        int q;
        float v[3];
    } scratch;
    TransformPoint3ColVec((float *)vertex, (float *)matrix, (float *)scratch.v);
    if (scratch.v[0] < vertex->floorX) {
        scratch.r = lightColor->r * vertex->floorX;
        __asm {fld scratch.r}
        __asm {fistp scratch.q} dst->red = scratch.q;
    } else {
        scratch.r = lightColor->r * scratch.v[0];
        if (scratch.r >= 255.0f)
            dst->red = 255;
        else {
            __asm {fld scratch.r}
            __asm {fistp scratch.q} dst->red = scratch.q;
        }
    }
    if (scratch.v[1] < vertex->floorY) {
        scratch.g = lightColor->g * vertex->floorY;
        __asm {fld scratch.g}
        __asm {fistp scratch.q} dst->green = scratch.q;
    } else {
        scratch.g = lightColor->g * scratch.v[1];
        if (scratch.g >= 255.0f)
            dst->green = 255;
        else {
            __asm {fld scratch.g}
            __asm {fistp scratch.q} dst->green = scratch.q;
        }
    }
    if (scratch.v[2] < vertex->floorZ) {
        scratch.b = lightColor->b * vertex->floorZ;
        __asm {fld scratch.b}
        __asm {fistp scratch.q} dst->blue = scratch.q;
    } else {
        scratch.b = lightColor->b * scratch.v[2];
        if (scratch.b >= 255.0f)
            dst->blue = 255;
        else {
            __asm {fld scratch.b}
            __asm {fistp scratch.q} dst->blue = scratch.q;
        }
    }
    dst->alpha = ((GfxBgraPixel *)srcColor)->alpha;
}

/*
 * Computes floor ambient and directional lighting to pack into BGRA color.
 */
// FUNCTION: FF7 0x0068de75
void __cdecl Gfx_TransformPointFloorLitColorToBgra(int color, int colorScale, GfxFloorVertex *vertex, int matrix, GfxLitLightColor *lightColor, void *srcColor, GfxBgraPixel *dst)
{
    struct {
        float b, g, r;
        int q;
        float v[3];
    } scratch;
    TransformPoint3ColVec((float *)vertex, (float *)matrix, (float *)scratch.v);
    if (scratch.v[0] < vertex->floorX) {
        scratch.r = lightColor->r * vertex->floorX;
        __asm {mov eax,colorScale}
        __asm { fld scratch.r }
        __asm {fmul dword ptr [eax+8]}
        __asm {fistp scratch.q} dst->red = scratch.q;
    } else {
        scratch.r = lightColor->r * scratch.v[0];
        if (scratch.r >= 255.0f)
            dst->red = ((char *)&color)[2];
        else {
            __asm {mov eax,colorScale}
            __asm { fld scratch.r }
            __asm {fmul dword ptr [eax+8]}
            __asm {fistp scratch.q} dst->red = scratch.q;
        }
    }
    if (scratch.v[1] < vertex->floorY) {
        scratch.g = lightColor->g * vertex->floorY;
        __asm {mov eax,colorScale}
        __asm { fld scratch.g }
        __asm {fmul dword ptr [eax+4]}
        __asm {fistp scratch.q} dst->green = scratch.q;
    } else {
        scratch.g = lightColor->g * scratch.v[1];
        if (scratch.g >= 255.0f)
            dst->green = ((char *)&color)[1];
        else {
            __asm {mov eax,colorScale}
            __asm { fld scratch.g }
            __asm {fmul dword ptr [eax+4]}
            __asm {fistp scratch.q} dst->green = scratch.q;
        }
    }
    if (scratch.v[2] < vertex->floorZ) {
        scratch.b = lightColor->b * vertex->floorZ;
        __asm {mov eax,colorScale}
        __asm { fld scratch.b }
        __asm {fmul dword ptr [eax+0]}
        __asm {fistp scratch.q} dst->blue = scratch.q;
    } else {
        scratch.b = lightColor->b * scratch.v[2];
        if (scratch.b >= 255.0f)
            dst->blue = ((char *)&color)[0];
        else {
            __asm {mov eax,colorScale}
            __asm { fld scratch.b }
            __asm {fmul dword ptr [eax+0]}
            __asm {fistp scratch.q} dst->blue = scratch.q;
        }
    }
    dst->alpha = ((GfxBgraPixel *)srcColor)->alpha;
}
#pragma optimize("p", off)

/*
 * Overwrites vertex color across all vertices in a polygon group.
 */
// FUNCTION: FF7 0x0068dff4
void __cdecl Gfx_SetGroupVertexColor(int color, int groupIndex, GfxPolygonSet *polySet)
{
    GfxEngineContext *ctx;
    GfxPolygonGroupColor *group;
    GfxTlVertex *vertex;
    int vertexIndex;

    ctx = GetEngineContext();
    group = (GfxPolygonGroupColor *)(Gfx_LockPolygonSetGroup(groupIndex, polySet, ctx));
    if (group != 0) {
        vertex = (GfxTlVertex *)group->vertices;
        for (vertexIndex = 0; vertexIndex < group->vertexCount; vertexIndex++) {
            vertex->color = color;
            vertex++;
        }
        Gfx_UnlockPolygonSetGroup(groupIndex, polySet, ctx);
    }
}
#pragma optimize("p", on)
#pragma optimize("p", off)

/*
 * Sets alpha component of vertex color for all vertices in a group.
 */
// FUNCTION: FF7 0x0068e071
void __cdecl Gfx_SetGroupVertexAlpha(unsigned char alpha, int groupIndex, GfxPolygonSet *polySet)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[20];
        int vertexIndex;
        struct { unsigned char pad_to_4[0x4]; GfxTlVertex *vertex; };
        struct { unsigned char pad_to_8[0x8]; int vertexColor; };
        struct { unsigned char pad_to_B[0xB]; char alphaByte; };
        struct { unsigned char pad_to_C[0xC]; GfxPolygonGroupColor *group; };
        struct { unsigned char pad_to_10[0x10]; GfxEngineContext *engineContext; };
    } frame;
#pragma pack(pop)

    frame.engineContext = GetEngineContext();
    frame.group = (GfxPolygonGroupColor *)(Gfx_LockPolygonSetGroup(groupIndex, polySet, (struct GfxEngineContext *)frame.engineContext));
    if (frame.group != 0) {
        frame.vertex = (GfxTlVertex *)frame.group->vertices;
        for (frame.vertexIndex = 0; frame.vertexIndex < frame.group->vertexCount; frame.vertexIndex++) {
            frame.vertexColor = frame.vertex->color;
            frame.alphaByte = alpha;
            frame.vertex->color = frame.vertexColor;
            frame.vertex++;
        }
        Gfx_UnlockPolygonSetGroup(groupIndex, polySet, (struct GfxEngineContext *)frame.engineContext);
    }
}
#pragma optimize("p", on)
#pragma optimize("p", off)

/*
 * Scales saved vertex colors and stores result into active vertex color.
 */
// FUNCTION: FF7 0x0068e0fd
void __cdecl Gfx_ScaleGroupVertexColorsFromSaved(int scale, int groupIndex, GfxPolygonSet *polySet)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[24];
        int vertexIndex;
        struct { unsigned char pad_to_4[0x4]; GfxTlVertex *vertex; };
        struct { unsigned char pad_to_8[0x8]; int savedColor; };
        struct { unsigned char pad_to_9[0x9]; int colorGreen; };
        struct { unsigned char pad_to_A[0xA]; int colorRed; };
        struct { unsigned char pad_to_B[0xB]; char colorAlpha; };
        struct { unsigned char pad_to_C[0xC]; GfxLightVertexRecord *savedCursor; };
        struct { unsigned char pad_to_10[0x10]; GfxPolygonSetEntry *group; };
        struct { unsigned char pad_to_14[0x14]; GfxEngineContext *engineContext; };
    } frame;
#pragma pack(pop)

    frame.engineContext = GetEngineContext();
    frame.group = (GfxPolygonSetEntry *)(Gfx_LockPolygonSetGroup(groupIndex, polySet, (struct GfxEngineContext *)frame.engineContext));
    if (frame.group != 0) {
        frame.savedCursor = (GfxLightVertexRecord *)frame.group->lightRecords;
        if (frame.savedCursor != 0) {
            frame.vertex = (GfxTlVertex *)frame.group->vertices;
            for (frame.vertexIndex = 0; frame.vertexIndex < frame.group->elementCount; frame.vertexIndex++) {
                frame.savedColor = frame.savedCursor->packedColor;
                frame.colorAlpha = 0xFF;
                *(char *)&frame.colorRed = (frame.colorRed & 0xFF) * scale / 0xFF;
                *(char *)&frame.colorGreen = (frame.colorGreen & 0xFF) * scale / 0xFF;
                *(char *)&frame.savedColor = (frame.savedColor & 0xFF) * scale / 0xFF;
                frame.vertex->color = frame.savedColor;
                frame.savedCursor++;
                frame.vertex++;
            }
        }
        Gfx_UnlockPolygonSetGroup(groupIndex, polySet, (struct GfxEngineContext *)frame.engineContext);
    }
}
#pragma optimize("p", on)
#pragma optimize("p", off)

/*
 * Invokes a color processing callback on each vertex in a group.
 */
// FUNCTION: FF7 0x0068e1ef
void __cdecl Gfx_ForEachGroupVertexColorWithContext(int callback, GfxDrawSpec *userData, int groupIndex, GfxPolygonSet *polySet)
{
    GfxEngineContext *ctx;
    GfxPolygonGroupColor *group;
    GfxTlVertex *vertex;
    int vertexIndex;

    ctx = GetEngineContext();
    group = (GfxPolygonGroupColor *)(Gfx_LockPolygonSetGroup(groupIndex, polySet, ctx));
    if (group != 0) {
        vertex = (GfxTlVertex *)group->vertices;
        for (vertexIndex = 0; vertexIndex < group->vertexCount; vertexIndex++) {
            /* Invokes untyped callback slot `callback` with its runtime (int, int) -> int signature. */
            ((int(__cdecl *)(int, int))callback)((int)&vertex->color, (int)userData);
            vertex++;
        }
        Gfx_UnlockPolygonSetGroup(groupIndex, polySet, ctx);
    }
}
#pragma optimize("p", on)
#pragma optimize("p", off)

/*
 * Invokes callback passing vertex coordinate and color for each vertex.
 */
// FUNCTION: FF7 0x0068e274
void __cdecl Gfx_ForEachGroupVertexAndColor(int callback, int groupIndex, GfxPolygonSet *polySet)
{
    GfxEngineContext *ctx;
    GfxPolygonGroupColor *group;
    GfxTlVertex *vertex;
    int vertexIndex;

    ctx = GetEngineContext();
    group = (GfxPolygonGroupColor *)(Gfx_LockPolygonSetGroup(groupIndex, polySet, ctx));
    if (group != 0) {
        vertex = (GfxTlVertex *)group->vertices;
        for (vertexIndex = 0; vertexIndex < group->vertexCount; vertexIndex++) {
            /* Invokes untyped callback slot `callback` with its runtime (int, int) -> int signature. */
            ((int(__cdecl *)(int, int))callback)((int)vertex, (int)&vertex->color);
            vertex++;
        }
        Gfx_UnlockPolygonSetGroup(groupIndex, polySet, ctx);
    }
}
#pragma optimize("p", on)
#pragma optimize("p", off)

/*
 * Invokes callback with saved vertex color and current vertex attributes.
 */
// FUNCTION: FF7 0x0068e2f9
void __cdecl Gfx_ForEachGroupSavedColorVertexAndColor(int callback, int groupIndex, GfxPolygonSet *polySet)
{
    GfxEngineContext *ctx;
    GfxPolygonSetEntry *group;
    GfxLightVertexRecord *snapshotCursor;
    GfxTlVertex *vertex;
    int vertexIndex;

    ctx = GetEngineContext();
    group = (GfxPolygonSetEntry *)(Gfx_LockPolygonSetGroup(groupIndex, polySet, ctx));
    if (group != 0) {
        snapshotCursor = (GfxLightVertexRecord *)group->lightRecords;
        if (snapshotCursor != 0) {
            vertex = (GfxTlVertex *)group->vertices;
            for (vertexIndex = 0; vertexIndex < group->elementCount; vertexIndex++) {
                /* Invokes untyped callback slot `callback` with its runtime (int, int, int) -> int signature. */
                ((int(__cdecl *)(int, int, int))callback)((int)&snapshotCursor->packedColor, (int)vertex, (int)&vertex->color);
                snapshotCursor++;
                vertex++;
            }
        }
        Gfx_UnlockPolygonSetGroup(groupIndex, polySet, ctx);
    }
}
#pragma optimize("p", on)
#pragma optimize("p", off)

/*
 * Restores original vertex colors from backup storage for a group.
 */
// FUNCTION: FF7 0x0068e39d
void __cdecl Gfx_RestoreGroupVertexColors(int groupIndex, GfxPolygonSet *polySet)
{
    GfxEngineContext *ctx;
    GfxPolygonSetEntry *group;
    GfxLightVertexRecord *restoreCursor;
    GfxTlVertex *vertex;
    int vertexIndex;

    ctx = GetEngineContext();
    group = (GfxPolygonSetEntry *)(Gfx_LockPolygonSetGroup(groupIndex, polySet, ctx));
    if (group != 0) {
        restoreCursor = (GfxLightVertexRecord *)group->lightRecords;
        if (restoreCursor != 0) {
            vertex = (GfxTlVertex *)group->vertices;
            for (vertexIndex = 0; vertexIndex < group->elementCount; vertexIndex++) {
                vertex->color = restoreCursor->packedColor;
                vertex++;
                restoreCursor++;
            }
        }
        Gfx_UnlockPolygonSetGroup(groupIndex, polySet, ctx);
    }
}
#pragma optimize("p", on)
#pragma optimize("p", off)

/*
 * Copies active group vertex colors into backup color buffer.
 */
// FUNCTION: FF7 0x0068e435
void __cdecl Gfx_SaveGroupVertexColors(int groupIndex, GfxPolygonSet *polySet)
{
    GfxEngineContext *ctx;
    GfxLightVertexRecord *storeCursor;
    GfxPolygonSetEntry *group;
    GfxTlVertex *vertex;
    int vertexIndex;

    ctx = GetEngineContext();
    group = (GfxPolygonSetEntry *)(Gfx_LockPolygonSetGroup(groupIndex, polySet, ctx));
    if (group != 0) {
        storeCursor = (GfxLightVertexRecord *)group->lightRecords;
        if (storeCursor != 0) {
            vertex = (GfxTlVertex *)group->vertices;
            for (vertexIndex = 0; vertexIndex < group->elementCount; vertexIndex++) {
                storeCursor->packedColor = vertex->color;
                Gfx_UnpackBgraToFloat4(storeCursor->packedColor, storeCursor->color);
                vertex++;
                storeCursor++;
            }
        }
        Gfx_UnlockPolygonSetGroup(groupIndex, polySet, ctx);
    }
}
#pragma optimize("p", on)
#pragma optimize("p", off)

/*
 * Sets uniform color across all groups of a polygon set.
 */
// FUNCTION: FF7 0x0068e4e3
void __cdecl Gfx_SetPolygonSetVertexColor(int color, GfxPolygonSet *polySet)
{
    int groupIndex;

    if (polySet != 0) {
        for (groupIndex = 0; groupIndex < polySet->groupCount; groupIndex++) {
            Gfx_SetGroupVertexColor(color, groupIndex, polySet);
        }
    }
}
#pragma optimize("p", on)
#pragma optimize("p", off)

/*
 * Sets uniform alpha value across all groups of a polygon set.
 */
// FUNCTION: FF7 0x0068e524
void __cdecl Gfx_SetPolygonSetVertexAlpha(int alpha, GfxPolygonSet *polySet)
{
    int groupIndex;

    if (polySet != 0) {
        for (groupIndex = 0; groupIndex < polySet->groupCount; groupIndex++) {
            Gfx_SetGroupVertexAlpha((char)alpha, groupIndex, polySet);
        }
    }
}
#pragma optimize("p", on)
#pragma optimize("p", off)

/*
 * Modulates vertex colors in all groups of a polygon set.
 */
// FUNCTION: FF7 0x0068e565
void __cdecl Gfx_ModulatePolygonSetVertexColors(int color, GfxPolygonSet *polySet)
{
    int groupIndex;
    GfxPolygonGroupColor *groupPointer;
    GfxEngineContext *engineContext;
    int vertIndex;
    GfxTlVertex *vertCur;
    int normColor[4];
    int modColor[4];

    Gfx_UnpackBgraToFloat4(color, (float *)modColor);
    Gfx_LightNormalizeColorInPlace((GfxNormalizedColor *)modColor);
    if (polySet != 0) {
        engineContext = GetEngineContext();
        for (groupIndex = 0; groupIndex < polySet->groupCount; groupIndex++) {
            groupPointer = (GfxPolygonGroupColor *)(Gfx_LockPolygonSetGroup(groupIndex, polySet, engineContext));
            if (groupPointer != 0) {
                vertCur = (GfxTlVertex *)groupPointer->vertices;
                for (vertIndex = 0; vertIndex < groupPointer->vertexCount; vertIndex++) {
                    Gfx_UnpackBgraToFloat4(vertCur->color, (float *)normColor);
                    Gfx_LightNormalizeColorInPlace((GfxNormalizedColor *)normColor);
                    Gfx_ModulateUnitFloatColorsToBytes((GfxFloatColor *)modColor, (GfxFloatColorAddend *)normColor, (struct GlTexelDest *)&vertCur->color);
                    vertCur++;
                }
                Gfx_UnlockPolygonSetGroup(groupIndex, polySet, engineContext);
            }
        }
    }
}
#pragma optimize("p", on)
#pragma optimize("p", off)

/*
 * Adds color bias to all vertex colors across a polygon set.
 */
// FUNCTION: FF7 0x0068e65b
void __cdecl Gfx_AddColorToPolygonSetVertexColors(GfxFloatColorAddend *bias, GfxPolygonSet *polySet)
{
    GfxEngineContext *ctx;
    GfxPolygonGroupColor *group;
    GfxTlVertex *vert;
    int groupSlot;
    int vertexIdx;
    int floatColor[4];

    if (polySet != 0) {
        ctx = GetEngineContext();
        for (groupSlot = 0; groupSlot < polySet->groupCount; groupSlot++) {
            group = (GfxPolygonGroupColor *)(Gfx_LockPolygonSetGroup(groupSlot, polySet, ctx));
            if (group != 0) {
                vert = (GfxTlVertex *)group->vertices;
                for (vertexIdx = 0; vertexIdx < group->vertexCount; vertexIdx++) {
                    Gfx_UnpackBgraToFloat4(vert->color, (float *)floatColor);
                    Gfx_LightNormalizeColorInPlace((GfxNormalizedColor *)floatColor);
                    Gfx_AddFloatColorsToVertexBgr(bias, (GfxFloatColorAddend *)floatColor, (struct GfxVertexBgr *)&vert->color);
                    vert++;
                }
                Gfx_UnlockPolygonSetGroup(groupSlot, polySet, ctx);
            }
        }
    }
}
#pragma optimize("p", on)
#pragma optimize("p", off)

/*
 * Restores backed-up vertex colors for all groups in a polygon set.
 */
// FUNCTION: FF7 0x0068e735
void __cdecl Gfx_RestorePolygonSetVertexColors(GfxPolygonSet *polySet)
{
    int groupIndex;

    if (polySet != 0) {
        for (groupIndex = 0; groupIndex < polySet->groupCount; groupIndex++) {
            Gfx_RestoreGroupVertexColors(groupIndex, polySet);
        }
    }
}
#pragma optimize("p", on)
#pragma optimize("p", off)

/*
 * Backs up vertex colors across all groups in a polygon set.
 */
// FUNCTION: FF7 0x0068e772
void __cdecl Gfx_SavePolygonSetVertexColors(GfxPolygonSet *polySet)
{
    int groupIndex;

    if (polySet != 0) {
        for (groupIndex = 0; groupIndex < polySet->groupCount; groupIndex++) {
            Gfx_SaveGroupVertexColors(groupIndex, polySet);
        }
    }
}
#pragma optimize("p", on)
#pragma optimize("p", off)

/*
 * Sets specular color mode to opaque black (0xFF000000).
 */
// FUNCTION: FF7 0x0068e7af
void __cdecl Gfx_SetVertexSpecularOpaque(GfxTlVertex *vertex)
{
    vertex->specular = 0xFF000000;
}
#pragma optimize("p", on)
#pragma optimize("p", off)

/*
 * Sets specular color based on proximity threshold.
 */
// FUNCTION: FF7 0x0068e7be
void __cdecl Gfx_SetVertexSpecularNear(GfxTlVertex *vertex, GfxSpecularDepthState *specState)
{
    vertex->specular = specState->specularValue;
}
#pragma optimize("p", on)
#pragma optimize("p", off)

/*
 * Sets specular color using cached specular color.
 */
// FUNCTION: FF7 0x0068e7d2
void __cdecl Gfx_SetVertexSpecularCached(GfxTlVertex *vertex, GfxSpecularDepthState *specState)
{
    vertex->specular = specState->cachedSpecular;
}
#pragma optimize("p", on)
#pragma optimize("p", off)

/*
 * Calculates specular color mapped by vertex depth range.
 */
// FUNCTION: FF7 0x0068e7e6
void __cdecl Gfx_SetVertexSpecularByDepth(GfxTlVertex *vertex, GfxSpecularDepthState *specState)
{
    struct {
        int q;
        float p;
        float vec[3];
    } scratch;
    TransformPointDepthOnly((float *)specState->matrix, (float *)vertex, (float *)scratch.vec);
    if (scratch.vec[2] <= specState->depthLow) {
        vertex->specular = 0xff000000;
    } else if (scratch.vec[2] >= specState->depthHigh) {
        vertex->specular = specState->specularValue;
    } else {
        scratch.p = scratch.vec[2] * specState->depthScale;
        __asm { fld scratch.p }
        __asm {fistp scratch.q} vertex->specular = *(int *)((char *)specState + scratch.q * 4 + 0x1a4);
    }
}
#pragma optimize("p", on)
#pragma optimize("p", off)

/*
 * Disables specular lighting on vertex (no-op).
 */
// FUNCTION: FF7 0x0068e87b
void __cdecl Gfx_SetVertexSpecularNone(void)
{
}
#pragma optimize("p", on)
} /* extern "C" */

extern "C" {
// The distinct original five-entry table, including the unused/no-op mode.
// GLOBAL: FF7 0x00910a08
int g_GfxSpecularDispatchTable[5] = {
    (int)Gfx_SetVertexSpecularOpaque,
    (int)Gfx_SetVertexSpecularNear,
    (int)Gfx_SetVertexSpecularCached,
    (int)Gfx_SetVertexSpecularByDepth,
    (int)Gfx_SetVertexSpecularNone,
};

/*
 * Updates cached specular color based on depth value and threshold ranges.
 */
// FUNCTION: FF7 0x0068e880
void __cdecl Gfx_UpdateCachedSpecularColor(GfxSpecularState *specState)
{
    float scaledDepth;
    int index;
    switch (specState->mode) {
    case GFX_SPECULAR_MODE_CACHED:
        if (specState->depth <= specState->depthMin) {
            specState->cachedColor = 0xff000000;
        } else if (specState->depth >= specState->depthMax) {
            specState->cachedColor = specState->defaultColor;
        } else {
            scaledDepth = specState->depth * specState->depthScale;
            /* Retail rounds via bare x87 fistp rather than VC5's truncating _ftol. */
            __asm { fld scaledDepth }
            __asm {fistp index} specState->cachedColor = specState->colorTable[index];
        }
    }
}

/*
 * Applies ambient, directional, and specular lighting to polygon set vertices.
 */
// FUNCTION: FF7 0x0068e92d
void __cdecl Gfx_LightPolygonSet(GfxPolygonSet *polygonSet, GfxLightPolygonSet *lightSet)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[124];
        float specLitColor[4];
        struct { unsigned char pad_to_10[0x10]; float specFloorLitColor[4]; };
        struct { unsigned char pad_to_20[0x20]; int specLitPointsLeft; };
        struct { unsigned char pad_to_24[0x24]; int specPointCursor; };
        struct { unsigned char pad_to_28[0x28]; GfxLightVertexRecord *specRecordCursor; };
        struct { unsigned char pad_to_2C[0x2C]; int specPointsLeft; };
        struct { unsigned char pad_to_30[0x30]; int dispatchPointCursor; };
        struct { unsigned char pad_to_34[0x34]; GfxLightVertexRecord *dispatchRecordCursor; };
        struct { unsigned char pad_to_38[0x38]; float litColor[4]; };
        struct { unsigned char pad_to_48[0x48]; float floorLitColor[4]; };
        struct { unsigned char pad_to_58[0x58]; int pointsLeft; };
        struct { unsigned char pad_to_5C[0x5C]; int pointCursor; };
        struct { unsigned char pad_to_60[0x60]; GfxLightVertexRecord *recordCursor; };
        struct { unsigned char pad_to_64[0x64]; int lightMode; };
        struct { unsigned char pad_to_68[0x68]; int groupIndex; };
        struct { unsigned char pad_to_6C[0x6C]; int pointPtr; };
        struct { unsigned char pad_to_70[0x70]; int (__cdecl *specularFn)(int, int); };
        struct { unsigned char pad_to_74[0x74]; GfxPolygonSetEntry *group; };
        struct { unsigned char pad_to_78[0x78]; GfxEngineContext *engineContext; };
    } frame;
#pragma pack(pop)

    if (polygonSet != 0 && lightSet != 0) {
        frame.engineContext = GetEngineContext();
        frame.lightMode = lightSet->flags & GFX_LIGHT_MODE_MASK;
        switch (frame.lightMode) {
        case GFX_LIGHT_MODE_NONE:
            break;
        case GFX_LIGHT_MODE_DIFFUSE:
            for (frame.groupIndex = 0; frame.groupIndex < polygonSet->groupCount; frame.groupIndex++) {
                frame.group = (GfxPolygonSetEntry *)Gfx_LockPolygonSetGroup(frame.groupIndex, polygonSet, (struct GfxEngineContext *)frame.engineContext);
                if (frame.group != 0) {
                    frame.recordCursor = (GfxLightVertexRecord *)frame.group->lightRecords;
                    if (frame.recordCursor != 0) {
                        frame.pointCursor = frame.group->vertices;
                        if ((lightSet->flags & GFX_LIGHT_FLAG_FLOOR) != 0) {
                            if ((lightSet->flags & GFX_LIGHT_FLAG_TINTED) != 0) {
                                Gfx_UnpackBgraToFloat4(lightSet->colorBgra, (float *)frame.floorLitColor);
                                Gfx_LightNormalizeColorInPlace((GfxNormalizedColor *)frame.floorLitColor);
                                for (frame.pointsLeft = frame.group->elementCount; frame.pointsLeft > 0; frame.pointsLeft--) {
                                    Gfx_TransformPointFloorLitColorToBgra(lightSet->colorBgra, (int)frame.floorLitColor, (GfxFloorVertex *)lightSet->floorVertex, frame.recordCursor->tag, (GfxLitLightColor *)frame.recordCursor->color, &frame.recordCursor->packedColor, (struct GfxBgraPixel *)(frame.pointCursor + 0x10));
                                    frame.pointCursor += 0x20;
                                    frame.recordCursor++;
                                }
                            } else {
                                for (frame.pointsLeft = frame.group->elementCount; frame.pointsLeft > 0; frame.pointsLeft--) {
                                    Gfx_TransformPointFloorColorToBgra((GfxFloorVertex *)lightSet->floorVertex, frame.recordCursor->tag, (GfxLitLightColor *)frame.recordCursor->color, &frame.recordCursor->packedColor, (struct GfxBgraPixel *)(frame.pointCursor + 0x10));
                                    frame.pointCursor += 0x20;
                                    frame.recordCursor++;
                                }
                            }
                        } else if ((lightSet->flags & GFX_LIGHT_FLAG_TINTED) != 0) {
                            Gfx_UnpackBgraToFloat4(lightSet->colorBgra, (float *)frame.litColor);
                            Gfx_LightNormalizeColorInPlace((GfxNormalizedColor *)frame.litColor);
                            for (frame.pointsLeft = frame.group->elementCount; frame.pointsLeft > 0; frame.pointsLeft--) {
                                Gfx_TransformPointLitColorToBgra(lightSet->colorBgra, (int)frame.litColor, (GfxFloorVertex *)lightSet->floorVertex, frame.recordCursor->tag, (GfxFloatColorAddend *)frame.recordCursor->color, &frame.recordCursor->packedColor, (struct GfxBgraPixel *)(frame.pointCursor + 0x10));
                                frame.pointCursor += 0x20;
                                frame.recordCursor++;
                            }
                        } else {
                            for (frame.pointsLeft = frame.group->elementCount; frame.pointsLeft > 0; frame.pointsLeft--) {
                                Gfx_LightVertexDiffuse((GfxFloorVertex *)lightSet->floorVertex, frame.recordCursor->tag, (GfxFloatColorAddend *)frame.recordCursor->color, &frame.recordCursor->packedColor, (struct GfxVertexColor *)(frame.pointCursor + 0x10));
                                frame.pointCursor += 0x20;
                                frame.recordCursor++;
                            }
                        }
                    }
                    Gfx_UnlockPolygonSetGroup(frame.groupIndex, polygonSet, (struct GfxEngineContext *)frame.engineContext);
                }
            }
            break;
        case GFX_LIGHT_MODE_SPECULAR:
            Gfx_UpdateCachedSpecularColor((struct GfxSpecularState *)lightSet);
            frame.specularFn = (int (__cdecl *)(int, int))g_GfxSpecularDispatchTable[lightSet->specularIndex];
            for (frame.groupIndex = 0; frame.groupIndex < polygonSet->groupCount; frame.groupIndex++) {
                frame.group = (GfxPolygonSetEntry *)Gfx_LockPolygonSetGroup(frame.groupIndex, polygonSet, (struct GfxEngineContext *)frame.engineContext);
                if (frame.group != 0) {
                    frame.dispatchRecordCursor = (GfxLightVertexRecord *)frame.group->lightRecords;
                    if (frame.dispatchRecordCursor != 0) {
                        frame.dispatchPointCursor = frame.group->vertices;
                        for (frame.specPointsLeft = frame.group->elementCount; frame.specPointsLeft > 0; frame.specPointsLeft--) {
                            frame.pointPtr = frame.dispatchPointCursor;
                            frame.specularFn(frame.pointPtr, (int)lightSet);
                            frame.dispatchPointCursor += 0x20;
                            frame.dispatchRecordCursor++;
                        }
                    }
                    Gfx_UnlockPolygonSetGroup(frame.groupIndex, polygonSet, (struct GfxEngineContext *)frame.engineContext);
                }
            }
            break;
        case GFX_LIGHT_MODE_DIFFUSE_SPECULAR:
            Gfx_UpdateCachedSpecularColor((struct GfxSpecularState *)lightSet);
            frame.specularFn = (int (__cdecl *)(int, int))g_GfxSpecularDispatchTable[lightSet->specularIndex];
            for (frame.groupIndex = 0; frame.groupIndex < polygonSet->groupCount; frame.groupIndex++) {
                frame.group = (GfxPolygonSetEntry *)Gfx_LockPolygonSetGroup(frame.groupIndex, polygonSet, (struct GfxEngineContext *)frame.engineContext);
                if (frame.group != 0) {
                    frame.specRecordCursor = (GfxLightVertexRecord *)frame.group->lightRecords;
                    if (frame.specRecordCursor != 0) {
                        frame.specPointCursor = frame.group->vertices;
                        if ((lightSet->flags & GFX_LIGHT_FLAG_FLOOR) != 0) {
                            if ((lightSet->flags & GFX_LIGHT_FLAG_TINTED) != 0) {
                                Gfx_UnpackBgraToFloat4(lightSet->colorBgra, (float *)frame.specFloorLitColor);
                                Gfx_LightNormalizeColorInPlace((GfxNormalizedColor *)frame.specFloorLitColor);
                                for (frame.specLitPointsLeft = frame.group->elementCount; frame.specLitPointsLeft > 0; frame.specLitPointsLeft--) {
                                    Gfx_TransformPointFloorLitColorToBgra(lightSet->colorBgra, (int)frame.specFloorLitColor, (GfxFloorVertex *)lightSet->floorVertex, frame.specRecordCursor->tag, (GfxLitLightColor *)frame.specRecordCursor->color, &frame.specRecordCursor->packedColor, (struct GfxBgraPixel *)(frame.specPointCursor + 0x10));
                                    frame.pointPtr = frame.specPointCursor;
                                    frame.specularFn(frame.pointPtr, (int)lightSet);
                                    frame.specPointCursor += 0x20;
                                    frame.specRecordCursor++;
                                }
                            } else {
                                for (frame.specLitPointsLeft = frame.group->elementCount; frame.specLitPointsLeft > 0; frame.specLitPointsLeft--) {
                                    Gfx_TransformPointFloorColorToBgra((GfxFloorVertex *)lightSet->floorVertex, frame.specRecordCursor->tag, (GfxLitLightColor *)frame.specRecordCursor->color, &frame.specRecordCursor->packedColor, (struct GfxBgraPixel *)(frame.specPointCursor + 0x10));
                                    frame.pointPtr = frame.specPointCursor;
                                    frame.specularFn(frame.pointPtr, (int)lightSet);
                                    frame.specPointCursor += 0x20;
                                    frame.specRecordCursor++;
                                }
                            }
                        } else if ((lightSet->flags & GFX_LIGHT_FLAG_TINTED) != 0) {
                            Gfx_UnpackBgraToFloat4(lightSet->colorBgra, (float *)frame.specLitColor);
                            Gfx_LightNormalizeColorInPlace((GfxNormalizedColor *)frame.specLitColor);
                            for (frame.specLitPointsLeft = frame.group->elementCount; frame.specLitPointsLeft > 0; frame.specLitPointsLeft--) {
                                Gfx_TransformPointLitColorToBgra(lightSet->colorBgra, (int)frame.specLitColor, (GfxFloorVertex *)lightSet->floorVertex, frame.specRecordCursor->tag, (GfxFloatColorAddend *)frame.specRecordCursor->color, &frame.specRecordCursor->packedColor, (struct GfxBgraPixel *)(frame.specPointCursor + 0x10));
                                frame.pointPtr = frame.specPointCursor;
                                frame.specularFn(frame.pointPtr, (int)lightSet);
                                frame.specPointCursor += 0x20;
                                frame.specRecordCursor++;
                            }
                        } else {
                            for (frame.specLitPointsLeft = frame.group->elementCount; frame.specLitPointsLeft > 0; frame.specLitPointsLeft--) {
                                Gfx_LightVertexDiffuse((GfxFloorVertex *)lightSet->floorVertex, frame.specRecordCursor->tag, (GfxFloatColorAddend *)frame.specRecordCursor->color, &frame.specRecordCursor->packedColor, (struct GfxVertexColor *)(frame.specPointCursor + 0x10));
                                frame.pointPtr = frame.specPointCursor;
                                frame.specularFn(frame.pointPtr, (int)lightSet);
                                frame.specPointCursor += 0x20;
                                frame.specRecordCursor++;
                            }
                        }
                    }
                    Gfx_UnlockPolygonSetGroup(frame.groupIndex, polygonSet, (struct GfxEngineContext *)frame.engineContext);
                }
            }
        }
    }
}

/*
 * Scales float color by scale factor and writes clamped byte values.
 */
// FUNCTION: FF7 0x0068efce
void __cdecl Gfx_ScaleFloatColorToBytes(int scaleBits, int input, int output)
{
    int p;

    __asm {
        mov ebx, input
        mov edx, output
        fld scaleBits
        fmul dword ptr [ebx]
        fld scaleBits
        fmul dword ptr [ebx + 4]
        fld scaleBits
        fmul dword ptr [ebx + 8]
        fxch st(2)
        fistp p
        mov eax, p
        fistp p
        mov byte ptr [edx], al
        mov ecx, p
        fistp p
        mov eax, p
        mov byte ptr [edx + 1], cl
        mov byte ptr [edx + 2], al
    }
}

/*
 * Blends float color channels with scaling and writes to BGR destination.
 */
// FUNCTION: FF7 0x0068f00f
void __cdecl Gfx_BlendRedScaleBgr(float scale, float blendWeight, void *colorB, GfxFloatColorAddend *colorA, GlTexelDest *dstBgr)
{
    struct {
        float b, g, r;
        int q;
    } color;
    color.r = colorA->red * scale + *(float *)((char *)colorB + 8) * blendWeight;
    __asm {fld color.r}
    __asm {fistp color.q} dstBgr->red = color.q;
    color.g = colorA->green * scale;
    __asm {fld color.g}
    __asm {fistp color.q} dstBgr->green = color.q;
    color.b = colorA->blue * scale;
    __asm {fld color.b}
    __asm {fistp color.q} dstBgr->blue = color.q;
}

/*
 * Blends two float color vectors and writes clamped BGR bytes.
 */
// FUNCTION: FF7 0x0068f079
void __cdecl Gfx_BlendFloatColorsToBgr(float weightA, float blendWeight, GfxFloatColor *colorB, GfxFloatColor *colorA, int dstBgr)
{
    struct {
        float b, g, r;
        int q;
    } color;
    color.r = colorA->r * weightA + colorB->r * blendWeight;
    __asm {fld color.r}
    __asm {fistp color.q} ((GlTexelDest *)dstBgr)->red = color.q;
    color.g = colorA->g * weightA + colorB->g * blendWeight;
    __asm {fld color.g}
    __asm {fistp color.q} ((GlTexelDest *)dstBgr)->green = color.q;
    color.b = colorA->b * weightA + colorB->b * blendWeight;
    __asm {fld color.b}
    __asm {fistp color.q} ((GlTexelDest *)dstBgr)->blue = color.q;
}

/*
 * Scales group vertex colors from saved color buffer by float multiplier.
 */
// FUNCTION: FF7 0x0068f0f8
void __cdecl Gfx_ScalePolygonSetGroupVertexColorsFromSaved(float scale, int groupIndex, GfxPolygonSet *polygonSet)
{
    GfxEngineContext *ctx;
    GfxPolygonSetEntry *groupHdr;
    GfxLightVertexRecord *srcColors;
    int vertices;
    int i;

    ctx = GetEngineContext();
    groupHdr = (GfxPolygonSetEntry *)(Gfx_LockPolygonSetGroup(groupIndex, polygonSet, ctx));
    if (groupHdr != 0) {
        srcColors = (GfxLightVertexRecord *)groupHdr->lightRecords;
        if (srcColors != 0) {
            vertices = groupHdr->vertices;
            for (i = 0; i < groupHdr->elementCount; i++) {
                Gfx_ScaleFloatColorToBytes(*(int *)&scale, (int)&srcColors->color, vertices + 0x10);
                srcColors++;
                vertices += 0x20;
            }
        }
        Gfx_UnlockPolygonSetGroup(groupIndex, polygonSet, ctx);
    }
}

/*
 * Blends group vertex colors between saved and base buffers.
 */
// FUNCTION: FF7 0x0068f19e
void __cdecl Gfx_BlendPolygonSetGroupVertexColorsFromSaved(float weightA, float weightB, GfxFloatColor *colorB, int groupIndex, GfxPolygonSet *polygonSet)
{
    GfxEngineContext *ctx;
    GfxPolygonSetEntry *groupHdr;
    GfxLightVertexRecord *srcColors;
    int vertices;
    int i;

    ctx = GetEngineContext();
    groupHdr = (GfxPolygonSetEntry *)(Gfx_LockPolygonSetGroup(groupIndex, polygonSet, ctx));
    if (groupHdr != 0) {
        srcColors = (GfxLightVertexRecord *)groupHdr->lightRecords;
        if (srcColors != 0) {
            vertices = groupHdr->vertices;
            for (i = 0; i < groupHdr->elementCount; i++) {
                Gfx_BlendFloatColorsToBgr(weightA, weightB, colorB, (GfxFloatColor *)&srcColors->color, vertices + 0x10);
                srcColors++;
                vertices += 0x20;
            }
        }
        Gfx_UnlockPolygonSetGroup(groupIndex, polygonSet, ctx);
    }
}

/*
 * Scales vertex colors across all groups of a polygon set from saved buffers.
 */
// FUNCTION: FF7 0x0068f24c
void __cdecl Gfx_ScalePolygonSetVertexColorsFromSaved(void *scale, GfxPolygonSet *polygonSet)
{
    int groupIndex;

    if (polygonSet != 0) {
        for (groupIndex = 0; groupIndex < polygonSet->groupCount; groupIndex++) {
            Gfx_ScalePolygonSetGroupVertexColorsFromSaved(*(float *)&scale, groupIndex, polygonSet);
        }
    }
}

/*
 * Blends vertex colors across all groups of a polygon set with target color.
 */
// FUNCTION: FF7 0x0068f28d
void __cdecl Gfx_BlendPolygonSetVertexColorsFromSavedWithColor(void *weightA, void *weightB, GfxFloatColor *colorB, GfxPolygonSet *polygonSet)
{
    int groupIndex;

    if (polygonSet != 0) {
        for (groupIndex = 0; groupIndex < polygonSet->groupCount; groupIndex++) {
            Gfx_BlendPolygonSetGroupVertexColorsFromSaved(*(float *)&weightA, *(float *)&weightB, colorB, groupIndex, polygonSet);
        }
    }
}

/*
 * Scales vertex colors from saved buffers or palette entries.
 */
// FUNCTION: FF7 0x0068f2d6
void __cdecl Gfx_ScalePolygonSetGroupColorsOrPalette(float scale, int rowIndex, int groupIndex, GfxPolygonSet *polygonSet)
{
    GfxEngineContext *ctx;
    GfxPolygonSetEntry *groupHdr;
    GfxLightVertexRecord *srcColors;
    GfxPaletteBlock *pPalette;
    int rowOffset;
    int vertices;
    int i;

    ctx = GetEngineContext();
    groupHdr = (GfxPolygonSetEntry *)(Gfx_LockPolygonSetGroup(groupIndex, polygonSet, ctx));
    if (groupHdr != 0) {
        if (groupHdr->texture != 0 && ((GfxTextureSurface *)groupHdr->texture)->palette != 0) {
            pPalette = (GfxPaletteBlock *)(((GfxTextureSurface *)groupHdr->texture)->palette);
            rowOffset = rowIndex * pPalette->stride;
            Gfx_ScaleFloatColorRowToBytes(*(int *)&scale, rowIndex, (struct MemFloatColorRowBuffer *)(((GfxTextureSurface *)groupHdr->texture)->palette));
            Gfx_WritePaletteEntriesCall(rowOffset, pPalette->stride, pPalette->startIndex, rowOffset, (int)pPalette, groupHdr->texture);
            Gfx_PaletteChangedCall(rowIndex, groupHdr->texture);
        } else {
            srcColors = (GfxLightVertexRecord *)groupHdr->lightRecords;
            if (srcColors != 0) {
                vertices = groupHdr->vertices;
                for (i = 0; i < groupHdr->elementCount; i++) {
                    Gfx_ScaleFloatColorToBytes(*(int *)&scale, (int)&srcColors->color, vertices + 0x10);
                    srcColors++;
                    vertices += 0x20;
                }
            }
        }
        Gfx_UnlockPolygonSetGroup(groupIndex, polygonSet, ctx);
    }
}

/*
 * Blends vertex colors from saved buffers or palette entries with coefficients.
 */
// FUNCTION: FF7 0x0068f413
void __cdecl Gfx_BlendPolygonSetGroupColorsOrPalette(float weightA, float weightB, GfxFloatColor *colorB, int rowIndex, int groupIndex, GfxPolygonSet *polygonSet)
{
    GfxEngineContext *ctx;
    GfxPolygonSetEntry *groupHdr;
    GfxLightVertexRecord *srcColors;
    GfxPaletteBlock *pPalette;
    int rowOffset;
    int vertices;
    int i;

    ctx = GetEngineContext();
    groupHdr = (GfxPolygonSetEntry *)(Gfx_LockPolygonSetGroup(groupIndex, polygonSet, ctx));
    if (groupHdr != 0) {
        if (groupHdr->texture != 0 && ((GfxTextureSurface *)groupHdr->texture)->palette != 0) {
            pPalette = (GfxPaletteBlock *)(((GfxTextureSurface *)groupHdr->texture)->palette);
            rowOffset = rowIndex * pPalette->stride;
            Gfx_BlendPaletteRowToBgr(weightA, weightB, colorB, rowIndex, (struct GfxPaletteBlock *)pPalette);
            Gfx_WritePaletteEntriesCall(rowOffset, pPalette->stride, pPalette->startIndex, rowOffset, (int)pPalette, groupHdr->texture);
            Gfx_PaletteChangedCall(rowIndex, groupHdr->texture);
        } else {
            srcColors = (GfxLightVertexRecord *)groupHdr->lightRecords;
            if (srcColors != 0) {
                vertices = groupHdr->vertices;
                for (i = 0; i < groupHdr->elementCount; i++) {
                    Gfx_BlendFloatColorsToBgr(weightA, weightB, colorB, (GfxFloatColor *)&srcColors->color, vertices + 0x10);
                    srcColors++;
                    vertices += 0x20;
                }
            }
        }
        Gfx_UnlockPolygonSetGroup(groupIndex, polygonSet, ctx);
    }
}

/*
 * Modulates vertex colors using saved vertex colors or surface palette.
 */
// FUNCTION: FF7 0x0068f557
void __cdecl Gfx_ModulateGroupVertexColorsFromSavedOrPalette(GfxFloatColor *modulation, int rowIndex, int groupIndex, GfxPolygonSet *polygonSet)
{
    GfxEngineContext *ctx;
    GfxPolygonSetEntry *groupHdr;
    GfxLightVertexRecord *srcColors;
    GfxPaletteBlock *pPalette;
    int rowOffset;
    int vertices;
    int i;

    ctx = GetEngineContext();
    groupHdr = (GfxPolygonSetEntry *)(Gfx_LockPolygonSetGroup(groupIndex, polygonSet, ctx));
    if (groupHdr != 0) {
        if (groupHdr->texture != 0 && ((GfxTextureSurface *)groupHdr->texture)->palette != 0) {
            pPalette = (GfxPaletteBlock *)(((GfxTextureSurface *)groupHdr->texture)->palette);
            rowOffset = rowIndex * pPalette->stride;
            Gfx_ModulatePaletteRowToBytes(modulation, rowIndex, (struct GfxPaletteBlock *)pPalette);
            Gfx_WritePaletteEntriesCall(rowOffset, pPalette->stride, pPalette->startIndex, rowOffset, (int)pPalette, groupHdr->texture);
            Gfx_PaletteChangedCall(rowIndex, groupHdr->texture);
        } else {
            srcColors = (GfxLightVertexRecord *)groupHdr->lightRecords;
            if (srcColors != 0) {
                vertices = groupHdr->vertices;
                for (i = 0; i < groupHdr->elementCount; i++) {
                    Gfx_ModulateUnitFloatColorsToBytes(modulation, (GfxFloatColorAddend *)&srcColors->color, (struct GlTexelDest *)(vertices + 0x10));
                    srcColors++;
                    vertices += 0x20;
                }
            }
        }
        Gfx_UnlockPolygonSetGroup(groupIndex, polygonSet, ctx);
    }
}

/*
 * Replaces green and red channels with saved blue channel values.
 */
// FUNCTION: FF7 0x0068f687
void __cdecl Gfx_MaskGroupSavedBlueToGreenRed(int colorMask, int groupIndex, GfxPolygonSet *polygonSet)
{
    GfxEngineContext *context;
    GfxSavedColorRecord *colorCursor;
    GfxPolygonSetEntry *groupPointer;
    int maskedBlue;
    int n;
    float curMaskedFloat;
    int maskByte;
    int dstVertices;

    context = GetEngineContext();
    groupPointer = (GfxPolygonSetEntry *)(Gfx_LockPolygonSetGroup(groupIndex, polygonSet, context));
    if (groupPointer != 0) {
        maskByte = colorMask >> 8;
        colorCursor = (GfxSavedColorRecord *)groupPointer->lightRecords;
        if (colorCursor != 0) {
            dstVertices = groupPointer->vertices;
            for (n = 0; n < groupPointer->elementCount; n++) {
                maskedBlue = colorCursor->blue & maskByte;
                curMaskedFloat = maskedBlue;
                colorCursor->green = maskedBlue;
                colorCursor->red = maskedBlue;
                colorCursor->redFloat = curMaskedFloat;
                colorCursor->greenFloat = curMaskedFloat;
                colorCursor++;
                dstVertices += 0x20;
            }
        }
        Gfx_UnlockPolygonSetGroup(groupIndex, polygonSet, context);
    }
}

/*
 * Applies blue channel mask to green and red for the first polygon group.
 */
// FUNCTION: FF7 0x0068f758
void __cdecl Gfx_MaskFirstGroupSavedBlueToGreenRed(int colorMask, GfxPolygonSet *polygonSet)
{
    int groupIndex;

    if (polygonSet != 0) {
        for (groupIndex = 0; groupIndex < 1; groupIndex++) {
            Gfx_MaskGroupSavedBlueToGreenRed(colorMask, groupIndex, polygonSet);
        }
    }
}
} /* extern "C" */
