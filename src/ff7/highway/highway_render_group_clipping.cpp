// src/ff7/highway/highway_render_group_clipping.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: The highway render group: it builds and draws batches of shaded polygons, clipping them
//   against the near plane and interpolating colours and points. Small assembly routines handle the
//   matrix transform and perspective divide of its vertices.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include <string.h>
#include "ff7/api/snobo.h"
#include "ff7/highway_palette.h"
#include "ff7/highway_psxdata.h"
#include <math.h>
#include <stdlib.h>
#include "ff7/types/highway.h"
#include "lib/enums/graphics.h"
#include "lib/types/graphics.h"
#include "ff7/api/highway.h"
#include "lib/pfile.h"
#include "lib/api/graphics.h"

#pragma intrinsic(strlen, strcpy, strcat, strcmp)
extern "C" {
#pragma intrinsic(memset)
#pragma intrinsic(atan2, sqrt)
// clang-format off
// GLOBAL: FF7 0x0090a6c8
int g_HighwayClipTable1[4][5] = {(-1), 1, 3, 2, (-2), 0, (-1), (-3), 3, 2, 0, 1, 3, (-3), (-2), 0, 1, (-1), (-2), 2};
// GLOBAL: FF7 0x0090a718
int g_HighwayClipEdges[4][2] = {0, 1, 0, 2, 1, 3, 2, 3};
// GLOBAL: FF7 0x0090a738
int g_HighwayClipTable2[4][4] = {2, 3, (-1), (-1), 1, (-1), 3, (-1), (-1), 0, (-1), 2, (-1), (-1), 0, 1};
// GLOBAL: FF7 0x0090a778
int g_HighwayClipTriangles[4][3] = {0, 1, 2, 0, 1, 3, 0, 3, 2, 1, 3, 2};
// The two UV quads HighwayRenderGroup::drawSegment (0x0065caf8) selects by draw mode,
// laid out in this TU's .data span.
// GLOBAL: FF7 0x0090a7a8
HighwayVec2 g_HighwayQuadUVMode1[4] = {{1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 0.0f}, {0.0f, 1.0f}};
// GLOBAL: FF7 0x0090a7c8
HighwayVec2 g_HighwayQuadUVMode0[4] = {{0.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}};
// GLOBAL: FF7 0x0090a7e8
int g_HighwayQuadRemap4[4] = {0, 1, 4, 2};
// GLOBAL: FF7 0x0090a7f8
int g_HighwayQuadRemap3[3] = {2, 3, 4};
unsigned char g_rawData_0090a804[4] = {0xff, 0xff, 0xff, 0xff};
// clang-format on


void __cdecl Mini_HighwayTransformPoint3ByMatrix(int arg1, int arg2, int arg3);
#pragma intrinsic(abs)
int __cdecl Mini_HighwayClipQuadAgainstZPlane(HighwayFloat4 *positions, HighwayVec2 *texCoords, int *colors,
                                              HighwayFloat4 *outPositions, HighwayVec2 *outTexCoords, int *outColors);
} // extern "C"

/*
 * Counts the null-terminated source list, stores reserved value and allocates four count-sized
 * dword arrays. For each entry initializes blend-four load options with global path/archive
 * settings and creates object kinds twelve and four. With the software driver every entry uses
 * the first source; otherwise each entry uses its own source. Clears both corresponding flags.
 */
// FUNCTION: FF7 0x0065c910
HighwayRenderGroup::HighwayRenderGroup(int *sources, int reservedValue)
{
    GfxEngineContext *ctx;
    int slotIndex;
    PfileLoadOptions loadOptions;
    int entry;

    reserved = reservedValue;
    ctx = GetEngineContext();
    slotIndex = 0;
    for (; sources[slotIndex] != 0; slotIndex++) {
    }
    count = slotIndex;
    objects = (int *)operator new(count * 4);
    first = (int *)operator new(count * 4);
    masks = (int *)operator new(count * 4);
    second = (int *)operator new(count * 4);

    for (slotIndex = 0; slotIndex < count; slotIndex++) {
        memset(&loadOptions, 0, sizeof(loadOptions));
        Pfile_InitLoadOptionsWithBlend(BLEND_NONE, &loadOptions);
        loadOptions.sourceMode = g_HighwayAssetSourceMode;
        loadOptions.directory = g_HighwayDataDirectory;
        if (g_HighwayUseLgpArchive) {
            loadOptions.file.fromArchive = 1;
            loadOptions.file.archiveId = 8;
            loadOptions.file.callback = 0;
        }
        if (ctx->driverType == GFX_DRIVER_SOFTWARE) {
            entry = sources[0];
        } else {
            entry = sources[slotIndex];
        }
        objects[slotIndex] = (int)Gfx_CreateRenderObject(
            1, GFX_DRAW_TEXTURED_QUAD_2D, &loadOptions, (char *)entry, (void *)ctx->renderObjectBank);
        masks[slotIndex] = (int)Gfx_CreateRenderObject(
            1, 4, &loadOptions, (char *)entry, (void *)ctx->renderObjectBank);
        first[slotIndex] = 0;
        second[slotIndex] = 0;
    }
}

/*
 * Shades and transforms four points, clips their polygon and writes the active render batches.
 * Draws a clipped render-group segment for a valid entry, reversing the supplied four points into
 * local order d/c/b/a. Computes grayscale diffuse colors from distance/fade helpers, composes the
 * global model and engine projection matrices, transforms four points, and clips them with one of
 * two UV tables chosen by the low byte of mode. A four-vertex result writes a quad object, three
 * writes a triangle mask, and five writes both via fixed remap tables; each successful acquisition
 * sets the corresponding first/second flag. Writes projected XYZW, diffuse color, and UV fields
 * while leaving the vertex padding untouched. frame and flag are unused; color conversion has no
 * clamp and global pointers are read before entry validation.
 */
// FUNCTION: FF7 0x0065caf8
void HighwayRenderGroup::drawSegment(int entry, int frame, HighwayFloat3 *a, HighwayFloat3 *b, HighwayFloat3 *c, HighwayFloat3 *d, int mode, char flag)
{
    HighwayFloat4 transformed[4];
    HighwayFloat3 localPointsTerm[4];
    GfxEngineContext *workCtxView;
    int clipTint[5];
    int litColors[4];
    int i;
    HighwayVec2 *pUvTable;
    HighwayVec2 texCoords[5];
    float combinedMtx[16];
    int vertCount;
    HighwayFloat4 outVerts[5];
    workCtxView = GetEngineContext();
    pUvTable = (unsigned char)mode ? g_HighwayQuadUVMode1 : g_HighwayQuadUVMode0;
    if (entry < 0 || entry >= count) {
        return;
    }
    localPointsTerm[0] = *d;
    localPointsTerm[1] = *c;
    localPointsTerm[2] = *b;
    localPointsTerm[3] = *a;
    for (i = 0; i < 4; i++) {
        float a;
        int b;
        float c;
        c = Mini_HighwayTransformDepth((struct HighwayDepthTransform *)(g_pHighwayFileBlock->baseMatrix), (struct HighwayDepthPoint *)(localPointsTerm + i));
        b = (int)((a = Mini_HighwayLinearFalloff(c)) * 255.0f);
        litColors[i] = (b << 16) | 0xFF000000 | (b << 8) | b;
    }
    MultiplyMatrix4x4Full(g_pHighwayFileBlock->baseMatrix, (void *)workCtxView->screenTransform, combinedMtx);
    for (i = 0; i < 4; i++) {
        Mini_HighwayTransformPoint3ByMatrixForward((int)combinedMtx, (int)(localPointsTerm + i), (int)(transformed + i));
    }
    vertCount = Mini_HighwayClipQuadAgainstZPlane(transformed, pUvTable, litColors, outVerts, texCoords, clipTint);
    if (vertCount == 4) {
        GfxRenderObject *a;
        a = (GfxRenderObject *)objects[entry];
        if (AcquireBatchNode(1, (void *)a)) {
            int b;
            for (b = 0; b < 4; b++) {
                HighwayFloat4 transformed;
                HighwayRenderSegment *workCtxView;
                Mini_HighwayPerspectiveDivideVector4((int)(outVerts + b), (int)&transformed);
                workCtxView = (HighwayRenderSegment *)(a->vertexCursor + (b << 5));
                workCtxView->x = transformed.x;
                workCtxView->y = transformed.y;
                workCtxView->z = transformed.z;
                workCtxView->w = transformed.w;
                workCtxView->color = clipTint[b];
                workCtxView->pointX = texCoords[b].x;
                workCtxView->pointY = texCoords[b].y;
            }
            first[entry] = 1;
        }
    } else if (vertCount == 3) {
        GfxRenderObject *a;
        a = (GfxRenderObject *)masks[entry];
        if (AcquireBatchNode(1, (void *)a)) {
            int b;
            for (b = 0; b < 3; b++) {
                HighwayFloat4 transformed;
                HighwayRenderSegment *workCtxView;
                Mini_HighwayPerspectiveDivideVector4((int)(outVerts + b), (int)&transformed);
                workCtxView = (HighwayRenderSegment *)(a->vertexCursor + (b << 5));
                workCtxView->x = transformed.x;
                workCtxView->y = transformed.y;
                workCtxView->z = transformed.z;
                workCtxView->w = transformed.w;
                workCtxView->color = clipTint[b];
                workCtxView->pointX = texCoords[b].x;
                workCtxView->pointY = texCoords[b].y;
            }
            second[entry] = 1;
        }
    } else if (vertCount == 5) {
        GfxRenderObject *a;
        a = (GfxRenderObject *)objects[entry];
        if (AcquireBatchNode(1, (void *)a)) {
            int b;
            for (b = 0; b < 4; b++) {
                HighwayFloat4 transformed;
                int localPointsTerm;
                HighwayRenderSegment *workCtxView;
                localPointsTerm = g_HighwayQuadRemap4[b];
                Mini_HighwayPerspectiveDivideVector4((int)(outVerts + localPointsTerm), (int)&transformed);
                workCtxView = (HighwayRenderSegment *)(a->vertexCursor + (b << 5));
                workCtxView->x = transformed.x;
                workCtxView->y = transformed.y;
                workCtxView->z = transformed.z;
                workCtxView->w = transformed.w;
                workCtxView->color = clipTint[localPointsTerm];
                workCtxView->pointX = texCoords[localPointsTerm].x;
                workCtxView->pointY = texCoords[localPointsTerm].y;
            }
            first[entry] = 1;
        }
        a = (GfxRenderObject *)masks[entry];
        if (AcquireBatchNode(1, (void *)a)) {
            int b;
            for (b = 0; b < 3; b++) {
                HighwayFloat4 transformed;
                int localPointsTerm;
                HighwayRenderSegment *workCtxView;
                localPointsTerm = g_HighwayQuadRemap3[b];
                Mini_HighwayPerspectiveDivideVector4((int)(outVerts + localPointsTerm), (int)&transformed);
                workCtxView = (HighwayRenderSegment *)(a->vertexCursor + (b << 5));
                workCtxView->x = transformed.x;
                workCtxView->y = transformed.y;
                workCtxView->z = transformed.z;
                workCtxView->w = transformed.w;
                workCtxView->color = clipTint[localPointsTerm];
                workCtxView->pointX = texCoords[localPointsTerm].x;
                workCtxView->pointY = texCoords[localPointsTerm].y;
            }
            second[entry] = 1;
        }
    }
}
extern "C" {

/*
 * Clips four vertices against Z = 0, treating negative Z as outside, and copies/interpolates
 * position, two texture coordinates and packed color through lookup-table topology. Returns 4 for
 * no outside vertices, 5 for one, 4 for two, 3 for three and 0 for four. The two-outside case
 * searches only four supported index pairs; an unsupported pair leaves the table selector at -1
 * and is still indexed.
 */
// FUNCTION: FF7 0x0065d1de
int __cdecl Mini_HighwayClipQuadAgainstZPlane(HighwayFloat4 *positions, HighwayVec2 *texCoords, int *colors, HighwayFloat4 *outPositions, HighwayVec2 *outTexCoords, int *outColors)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[76];
        int outsideFlag;
        struct { unsigned char pad_to_4[0x4]; float triLerpT; };
        struct { unsigned char pad_to_8[0x8]; int triSource; };
        struct { unsigned char pad_to_C[0xC]; int insideVertex; };
        struct { unsigned char pad_to_10[0x10]; int partnerIndex; };
        struct { unsigned char pad_to_14[0x14]; float pairLerpT; };
        struct { unsigned char pad_to_18[0x18]; int edgeIndex; };
        struct { unsigned char pad_to_1C[0x1C]; int pairIndex; };
        struct { unsigned char pad_to_20[0x20]; int outsideIndexB; };
        struct { unsigned char pad_to_24[0x24]; int outsideIndexA; };
        struct { unsigned char pad_to_28[0x28]; float lerpT; };
        struct { unsigned char pad_to_2C[0x2C]; int sourceIndex; };
        struct { unsigned char pad_to_30[0x30]; int outsideVertex; };
        struct { unsigned char pad_to_34[0x34]; int outsideCount; };
        struct { unsigned char pad_to_38[0x38]; int outsideFlags[4]; };
        struct { unsigned char pad_to_48[0x48]; int vertexIndex; };
    } frame;
#pragma pack(pop)

    frame.outsideCount = 0;
    for (frame.vertexIndex = 0; frame.vertexIndex < 4; frame.vertexIndex++) {
        if (positions[frame.vertexIndex].z < 0.0f) {
            frame.outsideFlag = 1;
        } else {
            frame.outsideFlag = 0;
        }
        frame.outsideFlags[frame.vertexIndex] = frame.outsideFlag;
        if (frame.outsideFlags[frame.vertexIndex] != 0) {
            frame.outsideCount++;
        }
    }
    if (frame.outsideCount == 0) {
        for (frame.vertexIndex = 0; frame.vertexIndex < 4; frame.vertexIndex++) {
            outPositions[frame.vertexIndex] = positions[frame.vertexIndex];
            outTexCoords[frame.vertexIndex] = texCoords[frame.vertexIndex];
            outColors[frame.vertexIndex] = colors[frame.vertexIndex];
        }
        return 4;
    } else if (frame.outsideCount == 1) {
        frame.outsideVertex = 0;
        for (frame.vertexIndex = 1; frame.vertexIndex < 4; frame.vertexIndex++) {
            if (frame.outsideFlags[frame.vertexIndex] != 0) {
                frame.outsideVertex = frame.vertexIndex;
            }
        }
        for (frame.vertexIndex = 0; frame.vertexIndex < 5; frame.vertexIndex++) {
            frame.sourceIndex = g_HighwayClipTable1[frame.outsideVertex][frame.vertexIndex];
            if (frame.sourceIndex >= 0) {
                outPositions[frame.vertexIndex] = positions[frame.sourceIndex];
                outTexCoords[frame.vertexIndex] = texCoords[frame.sourceIndex];
                outColors[frame.vertexIndex] = colors[frame.sourceIndex];
            } else {
                if (frame.sourceIndex == -frame.outsideVertex) {
                    frame.sourceIndex = 0;
                } else {
                    frame.sourceIndex = -frame.sourceIndex;
                }
                frame.lerpT = (0.0f - positions[frame.outsideVertex].z) / (positions[frame.sourceIndex].z - positions[frame.outsideVertex].z);
                Mini_HighwayInterpolateVector((HighwayLerpVector *)&positions[frame.outsideVertex], (HighwayLerpTarget *)&positions[frame.sourceIndex], frame.lerpT, (HighwayInterpolatedVector *)&outPositions[frame.vertexIndex]);
                Mini_HighwayInterpolateVec2(&texCoords[frame.outsideVertex], &texCoords[frame.sourceIndex], frame.lerpT, &outTexCoords[frame.vertexIndex]);
                Mini_HighwayInterpolatePackedColor(&colors[frame.outsideVertex], &colors[frame.sourceIndex], frame.lerpT, &outColors[frame.vertexIndex]);
            }
        }
        return 5;
    } else if (frame.outsideCount == 2) {
        frame.outsideIndexA = 0xFFFFFFFF;
        frame.outsideIndexB = 0xFFFFFFFF;
        frame.pairIndex = 0xFFFFFFFF;
        for (frame.vertexIndex = 0; frame.vertexIndex < 4; frame.vertexIndex++) {
            if (frame.outsideFlags[frame.vertexIndex] != 0) {
                if (frame.outsideIndexA < 0) {
                    frame.outsideIndexA = frame.vertexIndex;
                } else {
                    frame.outsideIndexB = frame.vertexIndex;
                }
            }
        }
        for (frame.edgeIndex = 0; frame.edgeIndex < 4; frame.edgeIndex++) {
            if (frame.outsideIndexA == g_HighwayClipEdges[frame.edgeIndex][0] && frame.outsideIndexB == g_HighwayClipEdges[frame.edgeIndex][1]) {
                frame.pairIndex = frame.edgeIndex;
            }
        }
        for (frame.edgeIndex = 0; frame.edgeIndex < 4; frame.edgeIndex++) {
            if (g_HighwayClipTable2[frame.pairIndex][frame.edgeIndex] == -1) {
                outPositions[frame.edgeIndex] = positions[frame.edgeIndex];
                outTexCoords[frame.edgeIndex] = texCoords[frame.edgeIndex];
                outColors[frame.edgeIndex] = colors[frame.edgeIndex];
            } else {
                frame.partnerIndex = g_HighwayClipTable2[frame.pairIndex][frame.edgeIndex];
                frame.pairLerpT = (0.0f - positions[frame.edgeIndex].z) / (positions[frame.partnerIndex].z - positions[frame.edgeIndex].z);
                Mini_HighwayInterpolateVector((HighwayLerpVector *)&positions[frame.edgeIndex], (HighwayLerpTarget *)&positions[frame.partnerIndex], frame.pairLerpT, (HighwayInterpolatedVector *)&outPositions[frame.edgeIndex]);
                Mini_HighwayInterpolateVec2(&texCoords[frame.edgeIndex], &texCoords[frame.partnerIndex], frame.pairLerpT, &outTexCoords[frame.edgeIndex]);
                Mini_HighwayInterpolatePackedColor(&colors[frame.edgeIndex], &colors[frame.partnerIndex], frame.pairLerpT, &outColors[frame.edgeIndex]);
            }
        }
        return 4;
    } else if (frame.outsideCount == 3) {
        frame.insideVertex = 0xFFFFFFFF;
        for (frame.vertexIndex = 0; frame.vertexIndex < 4; frame.vertexIndex++) {
            if (frame.outsideFlags[frame.vertexIndex] == 0) {
                frame.insideVertex = frame.vertexIndex;
            }
        }
        for (frame.vertexIndex = 0; frame.vertexIndex < 3; frame.vertexIndex++) {
            frame.triSource = g_HighwayClipTriangles[frame.insideVertex][frame.vertexIndex];
            if (frame.triSource == frame.insideVertex) {
                outPositions[frame.vertexIndex] = positions[frame.insideVertex];
                outTexCoords[frame.vertexIndex] = texCoords[frame.insideVertex];
                outColors[frame.vertexIndex] = colors[frame.insideVertex];
            } else {
                frame.triLerpT = (0.0f - positions[frame.triSource].z) / (positions[frame.insideVertex].z - positions[frame.triSource].z);
                Mini_HighwayInterpolateVector((HighwayLerpVector *)&positions[frame.triSource], (HighwayLerpTarget *)&positions[frame.insideVertex], frame.triLerpT, (HighwayInterpolatedVector *)&outPositions[frame.vertexIndex]);
                Mini_HighwayInterpolateVec2(&texCoords[frame.triSource], &texCoords[frame.insideVertex], frame.triLerpT, &outTexCoords[frame.vertexIndex]);
                Mini_HighwayInterpolatePackedColor(&colors[frame.triSource], &colors[frame.insideVertex], frame.triLerpT, &outColors[frame.vertexIndex]);
            }
        }
        return 3;
    } else {
        return 0;
    }
}

/*
 * Copies two packed dwords, computes each of their four unsigned-byte differences times t plus
 * first byte, narrows each result to unsigned byte and writes the packed result. Does not clamp t
 * or channel values; reconstructed unaligned dword byte extraction reaches beyond local dword
 * extent.
 */
// FUNCTION: FF7 0x0065d80d
void __cdecl Mini_HighwayInterpolatePackedColor(const int *first, const int *second, float t, int *result)
{
    int c0 = *first;
    int colorB = *second;
    int value;

    ((unsigned char *)&value)[0] = (unsigned char)(((colorB & 0xFF) - (c0 & 0xFF)) * t + (c0 & 0xFF));
    ((unsigned char *)&value)[1] = (unsigned char)((((*(int *)((char *)&colorB + 1) & 0xFF) -
                                                     (*(int *)((char *)&c0 + 1) & 0xFF)) *
                                                    t) +
                                                   (*(int *)((char *)&c0 + 1) & 0xFF));
    ((unsigned char *)&value)[2] = (unsigned char)((((*(int *)((char *)&colorB + 2) & 0xFF) -
                                                     (*(int *)((char *)&c0 + 2) & 0xFF)) *
                                                    t) +
                                                   (*(int *)((char *)&c0 + 2) & 0xFF));
    ((unsigned char *)&value)[3] = (unsigned char)((((*(int *)((char *)&colorB + 3) & 0xFF) -
                                                     (*(int *)((char *)&c0 + 3) & 0xFF)) *
                                                    t) +
                                                   (*(int *)((char *)&c0 + 3) & 0xFF));
    *result = value;
}

/*
 * Interpolates both components of a copied two-float point.
 */
// FUNCTION: FF7 0x0065d8fb
void __cdecl Mini_HighwayInterpolateVec2(const HighwayVec2 *first, const HighwayVec2 *second, float t,
                                         HighwayVec2 *result)
{
    HighwayVec2 a;
    HighwayVec2 b;
    HighwayVec2 value;

    a = *first;
    b = *second;
    value.x = (b.x - a.x) * t + a.x;
    value.y = (b.y - a.y) * t + a.y;
    *result = value;
}

/*
 * Linear interpolation between two vectors: writes into out the x, y and w components of from
 * moved towards to by the float fraction t, and stores 0 in z. The z component is not
 * interpolated.
 */
// FUNCTION: FF7 0x0065d94d
void __cdecl Mini_HighwayInterpolateVector(HighwayLerpVector *from, HighwayLerpTarget *to, float t, HighwayInterpolatedVector *out)
{
    out->x = (to->posX - from->x) * t + from->x;
    out->y = (to->posY - from->y) * t + from->y;
    out->z = 0;
    out->w = (to->posW - from->w) * t + from->w;
}

/*
 * Thin wrapper that forwards its three arguments unchanged to Mini_HighwayTransformPoint3ByMatrix and returns.
 */
// FUNCTION: FF7 0x0065d9a9
void __cdecl Mini_HighwayTransformPoint3ByMatrixForward(int matrix, int point, int out)
{
    Mini_HighwayTransformPoint3ByMatrix(matrix, point, out);
}

/*
 * Naked x87 assembly transforms input three floats by 16-float matrix and translation to four
 * floats, accumulating all products/translation before stores in reverse output order.
 * Saves/restores EBX/ESI/EDI despite no use. Does not read input W; x87 schedule/precision
 * explicit, no TU inference from hand assembly.
 */
// FUNCTION: FF7 0x0065d9c2
__declspec(naked) void __cdecl Mini_HighwayTransformPoint3ByMatrix(int matrix, int point, int out)
{
    // Original hand-scheduled x87 matrix/vector transform. EBX, ESI and EDI
    // are saved despite no use; ordinary VC5 C does not emit this schedule.
    __asm {
        push ebp
        mov ebp, esp
        push ebx
        push esi
        push edi
        mov ecx, dword ptr [ebp + 0Ch]
        mov edx, dword ptr [ebp + 8]
        mov eax, dword ptr [ebp + 10h]
        fld dword ptr [ecx]
        fmul dword ptr [edx]
        fld dword ptr [ecx]
        fmul dword ptr [edx + 4]
        fld dword ptr [ecx]
        fmul dword ptr [edx + 8]
        fld dword ptr [ecx]
        fmul dword ptr [edx + 0Ch]
        fld dword ptr [ecx + 4]
        fmul dword ptr [edx + 1Ch]
        fld dword ptr [ecx + 4]
        fmul dword ptr [edx + 14h]
        fld dword ptr [ecx + 4]
        fmul dword ptr [edx + 18h]
        fld dword ptr [ecx + 4]
        fmul dword ptr [edx + 10h]
        fxch st(3)
        faddp st(4), st(0)
        faddp st(4), st(0)
        faddp st(4), st(0)
        faddp st(4), st(0)
        fld dword ptr [ecx + 8]
        fmul dword ptr [edx + 2Ch]
        fld dword ptr [ecx + 8]
        fmul dword ptr [edx + 24h]
        fld dword ptr [ecx + 8]
        fmul dword ptr [edx + 28h]
        fld dword ptr [ecx + 8]
        fmul dword ptr [edx + 20h]
        fxch st(3)
        faddp st(4), st(0)
        faddp st(4), st(0)
        faddp st(4), st(0)
        faddp st(4), st(0)
        fld dword ptr [edx + 30h]
        fld dword ptr [edx + 34h]
        fld dword ptr [edx + 38h]
        fld dword ptr [edx + 3Ch]
        faddp st(4), st(0)
        faddp st(4), st(0)
        faddp st(4), st(0)
        faddp st(4), st(0)
        fstp dword ptr [eax + 0Ch]
        fstp dword ptr [eax + 8]
        fstp dword ptr [eax + 4]
        fstp dword ptr [eax]
        pop edi
        pop esi
        pop ebx
        pop ebp
        ret
    }
}

/*
 * Assembly computes reciprocal of input float at 0xC, multiplies input x, y, z by it and writes
 * those three results plus the reciprocal into output. x87 stores preserve stack scheduling.
 */
// FUNCTION: FF7 0x0065da4c
void __cdecl Mini_HighwayPerspectiveDivideVector4(int input, int output)
{
    int local = input;
    // The original preserves an otherwise unused EBX and schedules the FPU
    // stack explicitly. VC5 generates the frame and save/restore around asm.
    __asm {
        mov esi, local
        mov edi, output
        fld1
        fdiv dword ptr [esi + 0Ch]
        fld dword ptr [esi + 4]
        fmul st(0), st(1)
        fld dword ptr [esi + 8]
        fmul st(0), st(2)
        fld dword ptr [esi]
        fmul st(0), st(3)
        fstp dword ptr [edi]
        fxch st(1)
        fstp dword ptr [edi + 4]
        fstp dword ptr [edi + 8]
        fstp dword ptr [edi + 0Ch]
    }
}
} // extern "C"

/*
 * Enables the no-cull render state, emits objects for nonzero first flags and masks for nonzero
 * second flags, clearing each emitted flag. Disables the state afterward without restoring
 * its previous value.
 */
// FUNCTION: FF7 0x0065da86
void HighwayRenderGroup::reset()
{
    struct GfxEngineContext *context;
    int i;

    context = GetEngineContext();
    Gfx_SetRenderStateCall(RENDER_STATE_NO_CULL, 1, (struct GfxEngineContext *)context);
    for (i = 0; i < count; i++) {
        if (first[i] != 0) {
            Gfx_EmitPrimitiveBatchList((void *)objects[i], context);
            first[i] = 0;
        }
        if (second[i] != 0) {
            Gfx_EmitPrimitiveBatchList((void *)masks[i], context);
            second[i] = 0;
        }
    }
    Gfx_SetRenderStateCall(RENDER_STATE_NO_CULL, 0, (struct GfxEngineContext *)context);
}

/*
 * Destroys each nonzero object and mask under count, then releases objects, first, masks, second
 * arrays in that order. Member pointers and count are not cleared.
 */
// FUNCTION: FF7 0x0065db4c
HighwayRenderGroup::~HighwayRenderGroup()
{
    int i;
    int objectsBlock;
    int firstBits;
    int flagsSecond;
    int masksBlock;

    for (i = 0; i < count; i++) {
        if (objects[i] != 0) {
            DestroyRenderObject((void *)objects[i]);
        }
        if (masks[i] != 0) {
            DestroyRenderObject((void *)masks[i]);
        }
    }
    objectsBlock = (int)objects;
    operator delete((void *)objectsBlock);
    firstBits = (int)first;
    operator delete((void *)firstBits);
    masksBlock = (int)masks;
    operator delete((void *)masksBlock);
    flagsSecond = (int)second;
    operator delete((void *)flagsSecond);
}
