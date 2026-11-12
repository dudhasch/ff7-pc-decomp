// src/ff7/src/wm/wm_world_terrain_geometry.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Rounds floats through the x87 path and projects world terrain vertices and chunk
//   triangles to the screen. It also tests triangle heights and provides the vector and sorting
//   helpers that the terrain code uses.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/api/wm.h"
#include "lib/api/token.h"
#include <string.h>
#include "ff7/world.h"
#include "lib/types/graphics.h"
#include "lib/types/polygon.h"
#include "ff7/types/wm.h"
#include "lib/api/graphics.h"
#include "lib/codegen.h"
extern "C" {

#pragma intrinsic(strcpy, memcpy)
// GLOBAL: FF7 0x00e38938
WorldProjectedVertexEntry g_WorldProjectedVertices[128] = {0};
// GLOBAL: FF7 0x00e38f38
WorldTransformedVertexEntry g_WorldTransformedVertices[128] = {0};
// GLOBAL: FF7 0x00e39738
GfxMatrix4x4 g_WorldScreenMatrix = {0};
// GLOBAL: FF7 0x00e39778
float g_WorldMeshRotationMatrix[16] = {0};
// GLOBAL: FF7 0x00e397b8
int g_WorldMeshClipBits[126] = {0};
unsigned char g_bssPad_00e399b0[8] = {0};
// clang-format on

/*
 * Loads the float into x87 and converts it with FISTP to a signed qword using the current FPU
 * rounding mode, then returns the low dword.
 */
// FUNCTION: FF7 0x0075f090
int __cdecl World_RoundFloatToInt(float value)
{
    int result;

    __asm {
        fld value
        fistp qword ptr [ebp - 0xc]
        mov eax, dword ptr [ebp - 0xc]
        mov result, eax
    }
    return result;
}

/*
 * Converts the installed rotation matrix, composes the screen matrix and uploads 0x7A short
 * vertices into float XYZ and depth tables. After the first vertex, a positive previous depth
 * minus fadeScale applies a squared depth/screen-distance Y adjustment to the preceding vertex.
 */
// FUNCTION: FF7 0x0075f0ad
void __cdecl World_UploadMeshVertices(short *meshVerts, int fadeScale)
{
    int lastX;
    int depth;
    int halfWidth;
    float clip[4];
    int vertexIdx;
    float xDist;
    float depthTerm;

    GtePsxMatrixToFloat4x4Transposed(GetInstalledRotMatrixPtr(), g_WorldMeshRotationMatrix);
    Gfx_LightApplyTransposedMatrixToCachedProduct((PolygonTransform *)g_WorldMeshRotationMatrix, g_pWorldPrimaryLightGroup);
    ComposeWithScreenMatrix(g_WorldMeshRotationMatrix, &g_WorldScreenMatrix);
    halfWidth = ((unsigned int)g_WorldRenderWidth >> 1) + g_WorldViewportX;
    depth = 0;
    vertexIdx = 0;
    do {
        g_WorldMeshVertices[vertexIdx].posX = (float)meshVerts[0];
        g_WorldMeshVertices[vertexIdx].posY = (float)meshVerts[1];
        g_WorldMeshVertices[vertexIdx].posZ = (float)meshVerts[2];
        TransformPointByMatrix4x4(&g_WorldScreenMatrix,
                                  &g_WorldMeshVertices[vertexIdx], clip);
        TransformPointDepthOnly(g_WorldMeshRotationMatrix,
                                &g_WorldMeshVertices[vertexIdx].posX,
                                &g_WorldProjectedVertices[vertexIdx].x);
        meshVerts += 4;
        depth -= fadeScale;
        vertexIdx++;
        if (depth > 0) {
            depthTerm = depth * (1.0f / 64.0f);
            lastX += -halfWidth;
            if (lastX < 0)
                lastX = 0 - lastX;
            xDist = lastX * (1.0f / 32.0f);
            depthTerm += xDist;
            depthTerm *= depthTerm;
            g_WorldMeshVertices[vertexIdx - 2].posY -= depthTerm;
        }
        depth = World_RoundFloatToInt(g_WorldProjectedVertices[vertexIdx - 1].depth);
        lastX = World_RoundFloatToInt(clip[0] / clip[3]);
        depth >>= 2;
    } while (vertexIdx < 122);
}

/*
 * Transforms 126 terrain vertices through g_WorldScreenMatrix into homogeneous records, in triples. After
 * each new triple, perspective-divides only the preceding triple X/Y into the screen table, rounds
 * those coordinates and sets bit 0 for X inside and bit 8 for Y inside the half-open viewport.
 * The clip bits of the preceding triple are g_WorldMeshClipBits[vertIndex - 3..vertIndex - 1]; the
 * original addresses them from the last three words of g_WorldMeshRotationMatrix, which ends where
 * g_WorldMeshClipBits starts, so the code keeps that base.
 */
// FUNCTION: FF7 0x0075f263
void __cdecl World_ProjectMeshVertices(void)
{
    int vpTop;
    int right8;
    int x0;
    int insideY;
    int y0;
    int tripleCnt;
    float widthInv1;
    float invWidth0;
    int vertIndex;
    int y2;
    float rw2;
    int vpRemaining;
    int limitY;
    int x1;
    int x2;
    int y1;

    vertIndex = 0;
    vpRemaining = g_WorldViewportX;
    vpTop = g_WorldViewportY;
    right8 = g_WorldRenderWidth + g_WorldViewportX;
    limitY = g_WorldRenderHeight + g_WorldViewportY;
    TransformPointByMatrix4x4(&g_WorldScreenMatrix, &g_WorldMeshVertices[vertIndex], &g_WorldTransformedVertices[vertIndex]);
    TransformPointByMatrix4x4(&g_WorldScreenMatrix, &g_WorldMeshVertices[vertIndex + 1], &g_WorldTransformedVertices[vertIndex + 1]);
    TransformPointByMatrix4x4(&g_WorldScreenMatrix, &g_WorldMeshVertices[vertIndex + 2], &g_WorldTransformedVertices[vertIndex + 2]);
    tripleCnt = 41;
    for (vertIndex = 3; tripleCnt > 0; tripleCnt--, vertIndex += 3) {
        TransformPointByMatrix4x4(&g_WorldScreenMatrix, &g_WorldMeshVertices[vertIndex], &g_WorldTransformedVertices[vertIndex]);
        TransformPointByMatrix4x4(&g_WorldScreenMatrix, &g_WorldMeshVertices[vertIndex + 1], &g_WorldTransformedVertices[vertIndex + 1]);
        TransformPointByMatrix4x4(&g_WorldScreenMatrix, &g_WorldMeshVertices[vertIndex + 2], &g_WorldTransformedVertices[vertIndex + 2]);
        invWidth0 = 1.0f / g_WorldTransformedVertices[vertIndex - 3].depthW;
        widthInv1 = 1.0f / g_WorldTransformedVertices[vertIndex - 2].depthW;
        rw2 = 1.0f / g_WorldTransformedVertices[vertIndex - 1].depthW;
        g_WorldProjectedVertices[vertIndex - 3].x = g_WorldTransformedVertices[vertIndex - 3].perspScale * invWidth0;
        g_WorldProjectedVertices[vertIndex - 3].y = g_WorldTransformedVertices[vertIndex - 3].y * invWidth0;
        g_WorldProjectedVertices[vertIndex - 2].x = g_WorldTransformedVertices[vertIndex - 2].perspScale * widthInv1;
        g_WorldProjectedVertices[vertIndex - 2].y = g_WorldTransformedVertices[vertIndex - 2].y * widthInv1;
        g_WorldProjectedVertices[vertIndex - 1].x = g_WorldTransformedVertices[vertIndex - 1].perspScale * rw2;
        g_WorldProjectedVertices[vertIndex - 1].y = g_WorldTransformedVertices[vertIndex - 1].y * rw2;
        x0 = World_RoundFloatToInt(g_WorldProjectedVertices[vertIndex - 3].x);
        y0 = World_RoundFloatToInt(g_WorldProjectedVertices[vertIndex - 3].y);
        x1 = World_RoundFloatToInt(g_WorldProjectedVertices[vertIndex - 2].x);
        y1 = World_RoundFloatToInt(g_WorldProjectedVertices[vertIndex - 2].y);
        x2 = World_RoundFloatToInt(g_WorldProjectedVertices[vertIndex - 1].x);
        y2 = World_RoundFloatToInt(g_WorldProjectedVertices[vertIndex - 1].y);
        ((int *)g_WorldMeshRotationMatrix + 13)[vertIndex] = x0 < right8 & x0 >= vpRemaining;
        insideY = y0 < limitY & y0 >= vpTop;
        ((int *)g_WorldMeshRotationMatrix + 13)[vertIndex] |= insideY << 8;
        ((int *)g_WorldMeshRotationMatrix + 14)[vertIndex] = x1 < right8 & x1 >= vpRemaining;
        insideY = y1 < limitY & y1 >= vpTop;
        ((int *)g_WorldMeshRotationMatrix + 14)[vertIndex] |= insideY << 8;
        ((int *)g_WorldMeshRotationMatrix + 15)[vertIndex] = x2 < right8 & x2 >= vpRemaining;
        insideY = y2 < limitY & y2 >= vpTop;
        ((int *)g_WorldMeshRotationMatrix + 15)[vertIndex] |= insideY << 8;
    }
}

/*
 * Processes terrain triangles through clip-mask and rounded winding rejection, selects their
 * maximum transformed depth using rounded differences, and marks occlusion when a nearby triangle
 * spans the shared center value in both X and Y.
 */
// FUNCTION: FF7 0x0075f68c
void __cdecl World_SubmitTerrainChunkTriangles(WorldTerrainTriangle *first, WorldTerrainTriangle *end, short (*normals)[4], char *unused)
{
    struct {
        GfxTlVertex *v2;
        float iw2;
        GfxTlVertex *v1;
        float iw1;
        GfxTlVertex *v0;
        float iw0;
        BgraValue color0;
        unsigned char above2, padE0[3];
        int index2;
        BgraValue color2;
        float dz12;
        unsigned char above, padD0[3];
        BgraValue color1;
        WorldFloatColor light[3];
        float dz02;
        unsigned char below, pad94[3];
        float tv2, dz01;
        short *normal2;
        int texture;
        float tv1;
        FloatVector3 nf2;
        int clipMask;
        short *normal1;
        unsigned char below2, pad68[3];
        float tv0;
        FloatVector3 nf1;
        float center;
        short *normal0;
        unsigned int palette;
        int index0, index1;
        FloatVector3 nf0;
        float tu2, tu1, determinant, depth, tu0;
        BgraValue baseColor[3];
        int *packed;
        int distance, row, unused;
        WorldTerrainTriangle *triangle;
    } s; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    s.unused = 0;
    s.baseColor[0].bgra = s.baseColor[1].bgra = s.baseColor[2].bgra = 0xffffffff;
    s.light[0].r = s.light[1].r = s.light[2].r = 255.0f;
    s.light[0].g = s.light[1].g = s.light[2].g = 255.0f;
    s.light[0].b = s.light[1].b = s.light[2].b = 255.0f;
    s.light[0].a = s.light[1].a = s.light[2].a = 255.0f;
    s.center = (float)(((unsigned int)g_WorldRenderWidth >> 1) + g_WorldViewportX);
    s.triangle = &first[0];
    do {
        s.clipMask = g_WorldMeshClipBits[s.triangle->v0] | g_WorldMeshClipBits[s.triangle->v1] | g_WorldMeshClipBits[s.triangle->v2];
        if (!(s.clipMask & 255) | !(s.clipMask & 0xff00))
            goto next_triangle;
        s.dz01 = g_WorldProjectedVertices[s.triangle->v0].depth - g_WorldProjectedVertices[s.triangle->v1].depth;
        s.depth = g_WorldProjectedVertices[s.triangle->v2].depth;
        s.determinant = World_TriangleSignedArea2(&g_WorldProjectedVertices[s.triangle->v0], &g_WorldProjectedVertices[s.triangle->v1], &g_WorldProjectedVertices[s.triangle->v2]);
        if (g_pWorldSelectedTerrainTriangle == 0)
            goto selection_done;
        if (g_pWorldSelectedTerrainTriangle - s.triangle != 0)
            goto selection_done;
        g_pWorldSelectedTerrainTriangle = 0;
    selection_done:
        s.dz02 = g_WorldProjectedVertices[s.triangle->v0].depth - g_WorldProjectedVertices[s.triangle->v2].depth;
        s.normal0 = &normals[s.triangle->v0][0];
        if (World_RoundFloatToInt(s.determinant) < 0)
            goto next_triangle;
        s.normal1 = &normals[s.triangle->v1][0];
        if (World_RoundFloatToInt(s.dz01) < 0)
            goto second_depth;
        s.normal2 = &normals[s.triangle->v2][0];
        if (World_RoundFloatToInt(s.dz02) < 0)
            goto depth_done;
        s.depth = g_WorldProjectedVertices[s.triangle->v0].depth;
        goto depth_done;
    second_depth:
        s.dz12 = g_WorldProjectedVertices[s.triangle->v1].depth - g_WorldProjectedVertices[s.triangle->v2].depth;
        s.normal2 = &normals[s.triangle->v2][0];
        if (World_RoundFloatToInt(s.dz12) < 0)
            goto depth_done;
        s.depth = g_WorldProjectedVertices[s.triangle->v1].depth;
    depth_done:
        s.distance = World_RoundFloatToInt(s.depth) - g_WorldTerrainOcclusionDepth;
        if (s.distance > 0)
            goto occlusion_done;
        s.below = g_WorldProjectedVertices[s.triangle->v0].x < s.center;
        s.above = s.center < g_WorldProjectedVertices[s.triangle->v0].x;
        s.below2 = g_WorldProjectedVertices[s.triangle->v1].x < s.center;
        s.above2 = s.center < g_WorldProjectedVertices[s.triangle->v1].x;
        s.below |= s.below2;
        s.above |= s.above2;
        s.below2 = g_WorldProjectedVertices[s.triangle->v2].x < s.center;
        s.above2 = s.center < g_WorldProjectedVertices[s.triangle->v2].x;
        s.below |= s.below2;
        s.above |= s.above2;
        s.below &= s.above;
        if (!s.below)
            goto occlusion_done;
        s.below = g_WorldProjectedVertices[s.triangle->v0].y < s.center;
        s.above = s.center < g_WorldProjectedVertices[s.triangle->v0].y;
        s.below2 = g_WorldProjectedVertices[s.triangle->v1].y < s.center;
        s.above2 = s.center < g_WorldProjectedVertices[s.triangle->v1].y;
        s.below |= s.below2;
        s.above |= s.above2;
        s.below2 = g_WorldProjectedVertices[s.triangle->v2].y < s.center;
        s.above2 = s.center < g_WorldProjectedVertices[s.triangle->v2].y;
        s.below |= s.below2;
        s.above |= s.above2;
        s.below &= s.above;
        if (!s.below)
            goto occlusion_done;
        g_WorldTerrainHasWater = 1;
    occlusion_done:
        if (World_RoundFloatToInt(g_WorldTransformedVertices[s.triangle->v0].depthW) > 0 && World_RoundFloatToInt(g_WorldTransformedVertices[s.triangle->v1].depthW) > 0 && World_RoundFloatToInt(g_WorldTransformedVertices[s.triangle->v2].depthW) > 0) {
            s.palette = 0;
            if (g_WorldTerrainFogDistance == 0)
                goto fog_done;
            World_SortTriangleIndicesByDepth(&g_WorldProjectedVertices[s.triangle->v0].depth, &g_WorldProjectedVertices[s.triangle->v1].depth, &g_WorldProjectedVertices[s.triangle->v2].depth, &s.index0, &s.index1, &s.index2);
            s.baseColor[s.index0].bgra = ((WorldTerrainEffectSlot *)g_WorldTerrainEffectSlots)->vertexColor0;
            s.baseColor[s.index1].bgra = ((WorldTerrainEffectSlot *)g_WorldTerrainEffectSlots)->vertexColor1;
            s.baseColor[s.index2].bgra = ((WorldTerrainEffectSlot *)g_WorldTerrainEffectSlots)->vertexColor2;
            s.light[s.index0].r = ((WorldTerrainLightBlock *)g_WorldScratchColorBuffer)->colorR0;
            s.light[s.index1].r = ((WorldTerrainLightBlock *)g_WorldScratchColorBuffer)->colorR1;
            s.light[s.index2].r = ((WorldTerrainLightBlock *)g_WorldScratchColorBuffer)->colorR2;
            s.light[s.index0].g = ((WorldTerrainLightBlock *)g_WorldScratchColorBuffer)->colorG0;
            s.light[s.index1].g = ((WorldTerrainLightBlock *)g_WorldScratchColorBuffer)->colorG1;
            s.light[s.index2].g = ((WorldTerrainLightBlock *)g_WorldScratchColorBuffer)->colorG2;
            s.light[s.index0].b = ((WorldTerrainLightBlock *)g_WorldScratchColorBuffer)->colorB0;
            s.light[s.index1].b = ((WorldTerrainLightBlock *)g_WorldScratchColorBuffer)->colorB1;
            s.light[s.index2].b = ((WorldTerrainLightBlock *)g_WorldScratchColorBuffer)->colorB2;
            s.light[0].a = s.light[1].a = s.light[2].a = 255.0f;
            s.distance = World_RoundFloatToInt(s.depth) - g_WorldTerrainFogDistance;
            if (s.distance <= 0)
                goto fog_done;
            s.distance = (unsigned int)s.distance >> 3;
            s.distance &= 0xffc0;
            s.palette = s.distance;
            s.row = (s.palette + 64) >> 2;
            s.baseColor[s.index0].bgra = ((BgraValue *)g_WorldTerrainEffectSlots)[s.row - 8].bgra;
            s.baseColor[s.index1].bgra = ((unsigned int *)&g_WorldOverlayChannelTimers)[s.row];
            s.baseColor[s.index2].bgra = ((BgraValue *)g_WorldTerrainEffectSlots)[s.row - 0].bgra;
            s.light[s.index0].b = ((WorldFloatColor *)g_WorldScratchColorBuffer)[s.row - 8].b;
            s.light[s.index0].g = ((WorldFloatColor *)g_WorldScratchColorBuffer)[s.row - 4].g;
            s.light[s.index0].r = ((WorldFloatColor *)g_WorldScratchColorBuffer)[s.row - 0].r;
            s.light[s.index0].a = 255.0f;
            s.light[s.index1].b = ((WorldFloatColor *)g_WorldScratchColorBuffer)[s.row - 8].b;
            s.light[s.index1].g = ((WorldFloatColor *)g_WorldScratchColorBuffer)[s.row - 4].g;
            s.light[s.index1].r = ((WorldFloatColor *)g_WorldScratchColorBuffer)[s.row - 0].r;
            s.light[s.index1].a = 255.0f;
            s.light[s.index2].b = ((WorldFloatColor *)g_WorldScratchColorBuffer)[s.row - 8].b;
            s.light[s.index2].g = ((WorldFloatColor *)g_WorldScratchColorBuffer)[s.row - 4].g;
            s.light[s.index2].r = ((WorldFloatColor *)g_WorldScratchColorBuffer)[s.row - 0].r;
            s.light[s.index2].a = 255.0f;
            s.distance -= 960;
            if (s.distance <= 0)
                goto fog_done;
            s.palette = 255;
            s.baseColor[s.index0].bgra = ((BgraValue *)g_WorldTerrainEffectSlots)[s.palette - 8].bgra;
            s.baseColor[s.index1].bgra = ((unsigned int *)&g_WorldOverlayChannelTimers)[s.palette];
            s.baseColor[s.index2].bgra = ((BgraValue *)g_WorldTerrainEffectSlots)[s.palette - 0].bgra;
            s.light[s.index0].b = ((WorldFloatColor *)g_WorldScratchColorBuffer)[s.palette - 8].b;
            s.light[s.index0].g = ((WorldFloatColor *)g_WorldScratchColorBuffer)[s.palette - 4].g;
            s.light[s.index0].r = ((WorldFloatColor *)g_WorldScratchColorBuffer)[s.palette - 0].r;
            s.light[s.index0].a = 255.0f;
            s.light[s.index1].b = ((WorldFloatColor *)g_WorldScratchColorBuffer)[s.palette - 8].b;
            s.light[s.index1].g = ((WorldFloatColor *)g_WorldScratchColorBuffer)[s.palette - 4].g;
            s.light[s.index1].r = ((WorldFloatColor *)g_WorldScratchColorBuffer)[s.palette - 0].r;
            s.light[s.index1].a = 255.0f;
            s.light[s.index2].b = ((WorldFloatColor *)g_WorldScratchColorBuffer)[s.palette - 8].b;
            s.light[s.index2].g = ((WorldFloatColor *)g_WorldScratchColorBuffer)[s.palette - 4].g;
            s.light[s.index2].r = ((WorldFloatColor *)g_WorldScratchColorBuffer)[s.palette - 0].r;
            s.light[s.index2].a = 255.0f;
        fog_done:
            s.packed = &s.triangle->packedTexture;
            s.texture = *s.packed;
            s.texture >>= 14;
            s.texture &= 0x7fc;
            s.texture >>= 2;
            if (World_IsTerrainRenderEnabled()) {
                if (AcquireBatchNode(1, g_WorldTexturePageRenderObjects[s.texture])) {
                    s.tu0 = (float)(s.triangle->u0 - ((WorldTextureOrigin *)g_WorldTextureAuxTable)[s.texture].u) * g_WorldTexturePageRenderObjects[s.texture]->invWidth;
                    s.tv0 = (float)(s.triangle->v0tex - ((WorldTextureOrigin *)g_WorldTextureAuxTable)[s.texture].v) * g_WorldTexturePageRenderObjects[s.texture]->invHeight;
                    s.tu1 = (float)(s.triangle->u1 - ((WorldTextureOrigin *)g_WorldTextureAuxTable)[s.texture].u) * g_WorldTexturePageRenderObjects[s.texture]->invWidth;
                    s.tv1 = (float)(s.triangle->v1tex - ((WorldTextureOrigin *)g_WorldTextureAuxTable)[s.texture].v) * g_WorldTexturePageRenderObjects[s.texture]->invHeight;
                    s.tu2 = (float)(s.triangle->u2 - ((WorldTextureOrigin *)g_WorldTextureAuxTable)[s.texture].u) * g_WorldTexturePageRenderObjects[s.texture]->invWidth;
                    s.tv2 = (float)(s.triangle->v2tex - ((WorldTextureOrigin *)g_WorldTextureAuxTable)[s.texture].v) * g_WorldTexturePageRenderObjects[s.texture]->invHeight;
                    s.nf0.x = (float)s.normal0[0] * (1.0f / 4096.0f);
                    s.nf0.y = (float)s.normal0[1] * (1.0f / 4096.0f);
                    s.nf0.z = (float)s.normal0[2] * (1.0f / 4096.0f);
                    s.nf1.x = (float)s.normal1[0] * (1.0f / 4096.0f);
                    s.nf1.y = (float)s.normal1[1] * (1.0f / 4096.0f);
                    s.nf1.z = (float)s.normal1[2] * (1.0f / 4096.0f);
                    s.nf2.x = (float)s.normal2[0] * (1.0f / 4096.0f);
                    s.nf2.y = (float)s.normal2[1] * (1.0f / 4096.0f);
                    s.nf2.z = (float)s.normal2[2] * (1.0f / 4096.0f);
                    Gfx_LightVertexDiffuse((GfxFloorVertex *)g_pWorldPrimaryLightGroup->lightMatrix, (int)&s.nf0, (GfxFloatColorAddend *)(&s.light[0]), &s.baseColor[0], (struct GfxVertexColor *)&s.color0);
                    Gfx_LightVertexDiffuse((GfxFloorVertex *)g_pWorldPrimaryLightGroup->lightMatrix, (int)&s.nf1, (GfxFloatColorAddend *)(&s.light[1]), &s.baseColor[1], (struct GfxVertexColor *)&s.color1);
                    Gfx_LightVertexDiffuse((GfxFloorVertex *)g_pWorldPrimaryLightGroup->lightMatrix, (int)&s.nf2, (GfxFloatColorAddend *)(&s.light[2]), &s.baseColor[2], (struct GfxVertexColor *)&s.color2);
                    s.iw0 = 1.0f / g_WorldTransformedVertices[s.triangle->v0].depthW;
                    s.v0 = &g_WorldTexturePageRenderObjects[s.texture]->vertices[0];
                    s.v0->x = g_WorldTransformedVertices[s.triangle->v0].perspScale * s.iw0;
                    s.v0->y = g_WorldTransformedVertices[s.triangle->v0].y * s.iw0;
                    s.v0->z = g_WorldTransformedVertices[s.triangle->v0].z * s.iw0;
                    s.v0->rhw = s.iw0;
                    s.v0->colorUnsigned = s.color0.bgra;
                    s.v0->u = s.tu0;
                    s.v0->v = s.tv0;
                    s.iw1 = 1.0f / g_WorldTransformedVertices[s.triangle->v1].depthW;
                    s.v1 = &g_WorldTexturePageRenderObjects[s.texture]->vertices[1];
                    s.v1->x = g_WorldTransformedVertices[s.triangle->v1].perspScale * s.iw1;
                    s.v1->y = g_WorldTransformedVertices[s.triangle->v1].y * s.iw1;
                    s.v1->z = g_WorldTransformedVertices[s.triangle->v1].z * s.iw1;
                    s.v1->rhw = s.iw1;
                    s.v1->colorUnsigned = s.color1.bgra;
                    s.v1->u = s.tu1;
                    s.v1->v = s.tv1;
                    s.iw2 = 1.0f / g_WorldTransformedVertices[s.triangle->v2].depthW;
                    s.v2 = &g_WorldTexturePageRenderObjects[s.texture]->vertices[2];
                    s.v2->x = g_WorldTransformedVertices[s.triangle->v2].perspScale * s.iw2;
                    s.v2->y = g_WorldTransformedVertices[s.triangle->v2].y * s.iw2;
                    s.v2->z = g_WorldTransformedVertices[s.triangle->v2].z * s.iw2;
                    s.v2->rhw = s.iw2;
                    s.v2->colorUnsigned = s.color2.bgra;
                    s.v2->u = s.tu2;
                    s.v2->v = s.tv2;
                }
            }
        }
        unused += 0x28;
    next_triangle:
        s.triangle++;
    } while (s.triangle != end);
}

/*
 * Tests the supplied X and Z against each oriented triangle edge using rounded float cross
 * products, rejecting when any rounded result is positive and leaving out unchanged. For accepted
 * points, computes the full plane normal, scales its components by 1/256 and solves plane height
 * when the rounded normal Y is nonzero.
 */
// FUNCTION: FF7 0x0076085f
int __cdecl World_TriangleHeightAt(WorldTerrainTriangle *triangle, short *position, int *out)
{
    struct {
        WorldVec edge1;
        float height;
        float termZ;
        float termX;
        float total;
        WorldVec normal;
        FloatVector3 point;
        WorldVec edge2;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    frame.point.x = (float)position[0];
    frame.point.y = (float)position[1];
    frame.point.z = (float)position[2];
    frame.edge1.y = frame.edge2.y = 0;

    frame.edge1.x = g_WorldMeshVertices[triangle->v1].posX - g_WorldMeshVertices[triangle->v0].posX;
    frame.edge1.z = g_WorldMeshVertices[triangle->v1].posZ - g_WorldMeshVertices[triangle->v0].posZ;
    frame.edge2.z = frame.point.z - g_WorldMeshVertices[triangle->v0].posZ;
    frame.edge2.x = frame.point.x - g_WorldMeshVertices[triangle->v0].posX;
    frame.normal.y = frame.edge1.z * frame.edge2.x - frame.edge1.x * frame.edge2.z;
    if (World_RoundFloatToInt(frame.normal.y) > 0)
        goto reject;

    frame.edge1.x = g_WorldMeshVertices[triangle->v2].posX - g_WorldMeshVertices[triangle->v1].posX;
    frame.edge1.z = g_WorldMeshVertices[triangle->v2].posZ - g_WorldMeshVertices[triangle->v1].posZ;
    frame.edge2.z = frame.point.z - g_WorldMeshVertices[triangle->v1].posZ;
    frame.edge2.x = frame.point.x - g_WorldMeshVertices[triangle->v1].posX;
    frame.normal.y = frame.edge1.z * frame.edge2.x - frame.edge1.x * frame.edge2.z;
    if (World_RoundFloatToInt(frame.normal.y) > 0)
        goto reject;

    frame.edge1.x = g_WorldMeshVertices[triangle->v0].posX - g_WorldMeshVertices[triangle->v2].posX;
    frame.edge1.z = g_WorldMeshVertices[triangle->v0].posZ - g_WorldMeshVertices[triangle->v2].posZ;
    frame.edge2.z = frame.point.z - g_WorldMeshVertices[triangle->v2].posZ;
    frame.edge2.x = frame.point.x - g_WorldMeshVertices[triangle->v2].posX;
    frame.normal.y = frame.edge1.z * frame.edge2.x - frame.edge1.x * frame.edge2.z;
    if (World_RoundFloatToInt(frame.normal.y) > 0)
        goto reject;

    frame.edge1.x = g_WorldMeshVertices[triangle->v1].posX - g_WorldMeshVertices[triangle->v0].posX;
    frame.edge1.y = g_WorldMeshVertices[triangle->v1].posY - g_WorldMeshVertices[triangle->v0].posY;
    frame.edge2.x = g_WorldMeshVertices[triangle->v2].posX - g_WorldMeshVertices[triangle->v0].posX;
    frame.edge2.y = g_WorldMeshVertices[triangle->v2].posY - g_WorldMeshVertices[triangle->v0].posY;
    frame.edge1.z = g_WorldMeshVertices[triangle->v1].posZ - g_WorldMeshVertices[triangle->v0].posZ;
    frame.edge2.z = g_WorldMeshVertices[triangle->v2].posZ - g_WorldMeshVertices[triangle->v0].posZ;
    World_CrossProductFloat3(&frame.edge1, &frame.edge2, &frame.normal);
    frame.normal.y *= 0.00390625f;
    if (World_RoundFloatToInt(frame.normal.y) == 0)
        goto flat_body;
    frame.total = frame.normal.y * g_WorldMeshVertices[triangle->v0].posY;
    frame.normal.x *= 0.00390625f;
    frame.termX = (frame.termX = g_WorldMeshVertices[triangle->v0].posX - frame.point.x) * frame.normal.x;
    frame.normal.z *= 0.00390625f;
    frame.termZ = (frame.termZ = g_WorldMeshVertices[triangle->v0].posZ - frame.point.z) * frame.normal.z;
    frame.height = (frame.total = frame.termX + frame.termZ + frame.total) / frame.normal.y;
    goto output;
flat_body:
    frame.height = (frame.total = g_WorldMeshVertices[triangle->v0].posY + g_WorldMeshVertices[triangle->v1].posY + g_WorldMeshVertices[triangle->v2].posY) * 0.3333333432674408f;
output:
    *out = World_RoundFloatToInt(frame.height);
    return 1;
reject:
    return 0;
}

/*
 * The cross product of two three-float vectors into a third. Written out
 * component by component, no loop.
 */
// FUNCTION: FF7 0x00760c64
void __cdecl World_CrossProductFloat3(WorldVec *lhs, WorldVec *rhs, WorldVec *out)
{
    out->x = lhs->y * rhs->z - lhs->z * rhs->y;
    out->y = lhs->z * rhs->x - lhs->x * rhs->z;
    out->z = lhs->x * rhs->y - lhs->y * rhs->x;
}

/*
 * Returns twice the signed area of the screen triangle formed by the projected x and y of a, b
 * and c, using the expanded six-product determinant. The sign gives the planar winding.
 */
// FUNCTION: FF7 0x00760cc4
float __cdecl World_TriangleSignedArea2(WorldProjectedVertexEntry *vtxA, WorldProjectedVertexEntry *vtxB, WorldProjectedVertexEntry *vtxC)
{
    float area;

    area = vtxA->x * vtxB->y + vtxB->x * vtxC->y + vtxC->x * vtxA->y - vtxA->x * vtxC->y - vtxB->x * vtxA->y - vtxC->x * vtxB->y;
    return area;
}

/*
 * Writes indices for the three supplied depths in ascending order: selects the largest index for
 * the third output and orders the remaining pair for the first two outputs.
 */
// FUNCTION: FF7 0x00760d1b
void __cdecl World_SortTriangleIndicesByDepth(float *pfZ0, float *pfZ1, float *pfZ2, int *pInd0, int *pInd1, int *pInd2)
{
    struct {
        int pairIndex;
        float depths[3];
        int maxIndex;
    } frameSlot; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    frameSlot.depths[0] = *pfZ0;
    frameSlot.depths[1] = *pfZ1;
    frameSlot.depths[2] = *pfZ2;

    frameSlot.pairIndex = frameSlot.depths[0] < frameSlot.depths[1] ? 1 : 0;
    frameSlot.maxIndex = frameSlot.depths[frameSlot.pairIndex] < frameSlot.depths[2] ? 2 : frameSlot.pairIndex;
    if (frameSlot.maxIndex != 0) {
        *pInd2 = frameSlot.maxIndex;
        if (frameSlot.maxIndex == 1) {
            frameSlot.pairIndex = 2;
        } else {
            frameSlot.pairIndex = 1;
        }
        if (frameSlot.depths[0] < frameSlot.depths[frameSlot.pairIndex]) {
            *pInd0 = 0;
            *pInd1 = frameSlot.pairIndex;
        } else {
            *pInd0 = frameSlot.pairIndex;
            *pInd1 = 0;
        }
    } else {
        *pInd2 = 0;
        if (frameSlot.depths[1] < frameSlot.depths[2]) {
            *pInd0 = 1;
            *pInd1 = 2;
        } else {
            *pInd0 = 2;
            *pInd1 = 1;
        }
    }
}

/*
 * Walks the WorldTerrainTriangle records from tri up to end and moves the three texture u and v
 * coordinates of each by delta, subtracting when the triangle's mean u or v is below that coordinate
 * and adding otherwise.
 */
// FUNCTION: FF7 0x00760e1d
void __cdecl World_ShrinkMeshUVs(unsigned char *tri, unsigned char *end, unsigned char delta)
{
    WorldTerrainTriangle *q;
    int cu;
    int cv;

    q = (WorldTerrainTriangle *)tri;
    while (q != (WorldTerrainTriangle *)end) {
        cu = (q->u0 + q->u1 + q->u2) / 3;
        cv = (q->v0tex + q->v1tex + q->v2tex) / 3;
        if (cu < q->u0) {
            q->u0 -= delta;
        } else {
            q->u0 += delta;
        }
        if (cu < q->u1) {
            q->u1 -= delta;
        } else {
            q->u1 += delta;
        }
        if (cu < q->u2) {
            q->u2 -= delta;
        } else {
            q->u2 += delta;
        }
        if (cv < q->v0tex) {
            q->v0tex -= delta;
        } else {
            q->v0tex += delta;
        }
        if (cv < q->v1tex) {
            q->v1tex -= delta;
        } else {
            q->v1tex += delta;
        }
        if (cv < q->v2tex) {
            q->v2tex -= delta;
        } else {
            q->v2tex += delta;
        }
        q++;
    }
}
} /* extern "C" */
