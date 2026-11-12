// src/ff7/src/wm/wm_world_minimap_render.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Resets and stores the world map's mini-map state and requested mode. It also draws the
//   large and small mini-map backdrops and the position marker through the render batch.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/types/graphics.h"
#include "ff7/enums/wm.h"
#include "lib/codegen.h"
#include <string.h>
#include "ff7/world.h"
#include "ff7/types/wm.h"
#include "ff7/api/wm.h"
#include "ff7/api/main.h"
#include "lib/api/token.h"
#include "lib/api/graphics.h"
extern "C" {

#pragma intrinsic(strcpy, memcpy)

// clang-format off
// GLOBAL: FF7 0x00e3a898
int g_WorldMiniMapMode = 0;
// GLOBAL: FF7 0x00e3a89c
int g_WorldMiniMapScale = 0;
// GLOBAL: FF7 0x00e3a8a0
int g_WorldMiniMapRequestedMode = 0;
unsigned char g_bssPad_00e3a8a4[4] = {0};
// clang-format on

// The minimap's marker colours (BGRA) of the player and the five actors World_DrawMiniMap draws.
// GLOBAL: FF7 0x0096def0
int g_WorldMiniMapColorTable[6] = {(int)0x80800000, (int)0x80006000, (int)0x80006060, (int)0x80606060, (int)0x80600060, (int)0x80606000};
// The model ids of the five actors (index 1..5).
// GLOBAL: FF7 0x0096df08
char g_WorldMiniMapMarkerTypeTable[8] = {0x00, 0x06, 0x05, 0x03, 0x0d, 0x13, 0x00, 0x00};

/*
 * Resets the mini-map: mode g_WorldMiniMapRequestedMode and g_WorldMiniMapMode
 * to 0, and its scale g_WorldMiniMapScale to eight times g_WorldDisplayScale.
 */
// FUNCTION: FF7 0x00767d40
void __cdecl World_InitMiniMap(void)
{
    g_WorldMiniMapRequestedMode = 0;
    g_WorldMiniMapMode = g_WorldMiniMapRequestedMode;
    g_WorldMiniMapScale = g_WorldDisplayScale * 8;
}

/*
 * When minimap mode is below 2, selects scale 24 per unit for mode 1 on map
 * variant zero or 8 otherwise and computes the inset origin. Draws a blinking
 * player marker and up to five vehicle markers, omitting the player model ID
 * and unavailable vehicles.
 */
// FUNCTION: FF7 0x00767d68
void __cdecl World_DrawMiniMap(short angle)
{
    short av[4];
    VECTOR vb;
    int mid;
    int out;
    short tip[4];
    VECTOR vv;
    int d0;
    int bx;
    int by;
    int marker;
    short base[8];
    char matrix[32];
    GfxTlVertex *node0;
    GfxTlVertex *n0;
    GfxTlVertex *node1;

    if (g_WorldMiniMapMode < 2) {
        g_WorldMiniMapScale = g_WorldMiniMapMode == 1 && World_GetMapVariant() == 0
                           ? g_WorldDisplayScale * 0x18
                           : g_WorldDisplayScale * 8;
        bx = g_WorldDisplayScale * 316 - g_WorldMiniMapScale * 11;
        by = g_WorldDisplayScale * 220 - g_WorldMiniMapScale * 8;
        d0 = World_GetRenderBufferIndex();
        mid = WorldEntity_GetPlayerModelId();
        for (marker = 0; marker < 6; marker++) {
            if (marker != 0 ? g_WorldMiniMapMarkerTypeTable[marker] != mid && WorldEntity_SetCurrentById(g_WorldMiniMapMarkerTypeTable[marker]) != 0
                       : (WorldEntity_SelectPlayerAsCurrentActor(), 1)) {
                WorldEntity_GetCurrentPosition(&vv);
                tip[0] = bx + (g_WorldMiniMapScale * vv.vx >> 15) - g_WorldDisplayScale * 4 + g_WorldViewportX;
                tip[1] = by + (g_WorldMiniMapScale * vv.vz >> 15) - g_WorldDisplayScale * 4 + g_WorldViewportY;
                tip[3] = ((((unsigned int)(Savemap_GetClockTickCount() + marker * 4) >> (marker != 0)) & 0x10)
                          << (g_WorldMiniMapScale > g_WorldDisplayScale * 8)) +
                         4;
                World_DrawMiniMapMarker(tip[0], tip[1], tip[3], g_WorldMiniMapColorTable[marker], g_WorldMapRenderObject);
            }
        }
        if (World_GetCameraMode() == 2 || World_GetCameraMode() == 3) {
            WorldEntity_GetPlayerPosition(&vv);
            base[0] = bx + (g_WorldMiniMapScale * vv.vx >> 15) + g_WorldViewportX;
            base[1] = by + (g_WorldMiniMapScale * vv.vz >> 15) + g_WorldViewportY;
            av[0] = av[1] = 0;
            av[2] = angle;
            RotMatrixXYZ(av, matrix);
            vb.vx = vb.vy = vb.vz = 0;
            SetMatrixTranslation(matrix, &vb);
            SetRotMatrix(matrix);
            SetTransMatrix(matrix);
            marker = g_WorldMiniMapScale == g_WorldDisplayScale * 8 ? g_WorldDisplayScale * -8 : g_WorldDisplayScale * -0x10;
            av[0] = marker >> 1;
            av[1] = marker;
            RotTransPoint(av, &vb, &out);
            base[2] = base[0] + vb.vx;
            base[3] = base[1] + vb.vy;
            av[0] = -av[0];
            RotTransPoint(av, &vb, &out);
            base[4] = base[0] + vb.vx;
            base[5] = base[1] + vb.vy;
            if (AcquireBatchNode(1, g_WorldTerrainRenderObject) != 0 && World_IsTerrainRenderEnabled() != 0) {
                n0 = (GfxTlVertex *)(g_WorldTerrainRenderObject->vertexCursor);
                n0->x = base[0];
                n0->y = base[1];
                n0->zBits = FLOAT_0_01_BITS;
                n0->rhwBits = FLOAT_ONE_BITS;
                n0->color = 0x80606000;
                n0->specular = 0xFF000000;
                node0 = (GfxTlVertex *)(g_WorldTerrainRenderObject->vertexCursor + 0x20);
                node0->x = base[2];
                node0->y = base[3];
                node0->zBits = FLOAT_0_01_BITS;
                node0->rhwBits = FLOAT_ONE_BITS;
                node0->color = 0x80101000;
                node0->specular = 0xFF000000;
                node1 = (GfxTlVertex *)(g_WorldTerrainRenderObject->vertexCursor + 0x40);
                node1->x = base[4];
                node1->y = base[5];
                node1->zBits = FLOAT_0_01_BITS;
                node1->rhwBits = FLOAT_ONE_BITS;
                node1->color = 0x80101000;
                node1->specular = 0xFF000000;
            }
        }
        if (g_WorldMiniMapScale == g_WorldDisplayScale * 8) {
            World_DrawMiniMapBackdropSmall(bx + g_WorldViewportX, by + g_WorldViewportY, g_WorldDisplayScale * 72, g_WorldDisplayScale * 56);
        } else {
            World_DrawMiniMapBackdropLarge(bx + g_WorldViewportX, by + g_WorldViewportY, g_WorldMiniMapScale * 9, g_WorldMiniMapScale * 7);
        }
    }
}

/*
 * Stores the signed-short value in mini-map state g_WorldMiniMapMode. Copies
 * it to requested mode g_WorldMiniMapRequestedMode unless the player model is
 * 3.
 */
// FUNCTION: FF7 0x0076831f
void __cdecl World_SetMiniMapState(short value)
{
    g_WorldMiniMapMode = value;
    if (WorldEntity_GetPlayerModelId() != WORLD_MODEL_HIGHWIND) {
        g_WorldMiniMapRequestedMode = g_WorldMiniMapMode;
    }
}

/*
 * Returns the mini-map state g_WorldMiniMapMode as a short.
 */
// FUNCTION: FF7 0x00768343
short __cdecl World_GetMiniMapState(void)
{
    return (short)g_WorldMiniMapMode;
}

/*
 * Leaves mini-map state 1 for the requested mode g_WorldMiniMapRequestedMode.
 */
// FUNCTION: FF7 0x0076834e
void __cdecl World_RestoreMiniMapRequestedMode(void)
{
    if (g_WorldMiniMapMode == 1) {
        g_WorldMiniMapMode = g_WorldMiniMapRequestedMode;
    }
}

/*
 * Acquires one batch node through g_WorldMiniMapRenderObject, then writes an axis-aligned
 * textured quad only when terrain rendering is enabled. Uses corners (x0,y0),
 * (x0,y0+sy), (x0+sx,y0), (x0+sx,y0+sy), depth bits 0x3A83126F, reciprocal W
 * 1, diffuse 0x80FFFFFF and specular 0xFF000000.
 */
// FUNCTION: FF7 0x00768366
void __cdecl World_DrawMiniMapBackdropLarge(float x0, float y0, float sx, float sy)
{
    float tu0;
    float tv0;
    float du;
    int zBits;
    GfxTlVertex *pVert0;
    float vSpan;
    GfxTlVertex *pVert1;
    GfxTlVertex *vertex2;
    GfxTlVertex *vertex3;

    if (AcquireBatchNode(1, g_WorldMiniMapRenderObject) != 0) {
        if (World_IsTerrainRenderEnabled() != 0) {
            zBits = FLOAT_0_001_BITS;
            tu0 = 0.00390625f;
            tv0 = 0.00390625f;
            du = 0.8359375f;
            vSpan = 0.6484375f;
            pVert0 = (GfxTlVertex *)(g_WorldMiniMapRenderObject->vertexCursor);
            pVert0->x = x0;
            pVert0->y = y0;
            pVert0->zBits = zBits;
            pVert0->rhwBits = FLOAT_ONE_BITS;
            pVert0->color = 0x80FFFFFF;
            pVert0->specular = 0xFF000000;
            pVert0->u = tu0;
            pVert0->v = tv0;
            pVert1 = (GfxTlVertex *)(g_WorldMiniMapRenderObject->vertexCursor + 0x20);
            pVert1->x = x0;
            pVert1->y = y0 + sy;
            pVert1->zBits = zBits;
            pVert1->rhwBits = FLOAT_ONE_BITS;
            pVert1->color = 0x80FFFFFF;
            pVert1->specular = 0xFF000000;
            pVert1->u = tu0;
            pVert1->v = tv0 + vSpan;
            vertex2 = (GfxTlVertex *)(g_WorldMiniMapRenderObject->vertexCursor + 0x40);
            vertex2->x = x0 + sx;
            vertex2->y = y0;
            vertex2->zBits = zBits;
            vertex2->rhwBits = FLOAT_ONE_BITS;
            vertex2->color = 0x80FFFFFF;
            vertex2->specular = 0xFF000000;
            vertex2->u = tu0 + du;
            vertex2->v = tv0;
            vertex3 = (GfxTlVertex *)(g_WorldMiniMapRenderObject->vertexCursor + 0x60);
            vertex3->x = x0 + sx;
            vertex3->y = y0 + sy;
            vertex3->zBits = zBits;
            vertex3->rhwBits = FLOAT_ONE_BITS;
            vertex3->color = 0x80FFFFFF;
            vertex3->specular = 0xFF000000;
            vertex3->u = tu0 + du;
            vertex3->v = tv0 + vSpan;
        }
    }
}

// Emits one textured minimap backdrop quad into the batch node of g_WorldRadarRenderObject, and
// only when the node is acquired and terrain rendering is enabled. The vertex pointer at +0x70 of
// the render object points to four 32-byte GfxTlVertex records at +0x00, +0x20, +0x40 and
// +0x60, written in the order top-left, bottom-left, top-right, bottom-right. Each vertex has depth
// 0x3A83126F, rhw 1.0f, diffuse 0x80FFFFFF and specular 0xFF000000, and differs only in position
// and texture coordinates.
// FUNCTION: FF7 0x00768530
void __cdecl World_DrawMiniMapBackdropSmall(float x0, float y0, float sx, float sy)
{
    float tu0;
    float tv0;
    float du;
    int zBits;
    GfxTlVertex *pVert0;
    float vSpan;
    GfxTlVertex *pVert1;
    GfxTlVertex *vertex2;
    GfxTlVertex *vertex3;

    if (AcquireBatchNode(1, g_WorldRadarRenderObject) != 0) {
        if (World_IsTerrainRenderEnabled() != 0) {
            zBits = FLOAT_0_001_BITS;
            tu0 = 0.0078125f;
            tv0 = 0.015625f;
            du = 0.546875f;
            vSpan = 0.84375f;
            pVert0 = (GfxTlVertex *)(g_WorldRadarRenderObject->vertexCursor);
            pVert0->x = x0;
            pVert0->y = y0;
            pVert0->zBits = zBits;
            pVert0->rhwBits = FLOAT_ONE_BITS;
            pVert0->color = 0x80FFFFFF;
            pVert0->specular = 0xFF000000;
            pVert0->u = tu0;
            pVert0->v = tv0;
            pVert1 = (GfxTlVertex *)(g_WorldRadarRenderObject->vertexCursor + 0x20);
            pVert1->x = x0;
            pVert1->y = y0 + sy;
            pVert1->zBits = zBits;
            pVert1->rhwBits = FLOAT_ONE_BITS;
            pVert1->color = 0x80FFFFFF;
            pVert1->specular = 0xFF000000;
            pVert1->u = tu0;
            pVert1->v = tv0 + vSpan;
            vertex2 = (GfxTlVertex *)(g_WorldRadarRenderObject->vertexCursor + 0x40);
            vertex2->x = x0 + sx;
            vertex2->y = y0;
            vertex2->zBits = zBits;
            vertex2->rhwBits = FLOAT_ONE_BITS;
            vertex2->color = 0x80FFFFFF;
            vertex2->specular = 0xFF000000;
            vertex2->u = tu0 + du;
            vertex2->v = tv0;
            vertex3 = (GfxTlVertex *)(g_WorldRadarRenderObject->vertexCursor + 0x60);
            vertex3->x = x0 + sx;
            vertex3->y = y0 + sy;
            vertex3->zBits = zBits;
            vertex3->rhwBits = FLOAT_ONE_BITS;
            vertex3->color = 0x80FFFFFF;
            vertex3->specular = 0xFF000000;
            vertex3->u = tu0 + du;
            vertex3->v = tv0 + vSpan;
        }
    }
}

/*
 * Acquires a node, then writes a textured square only when terrain rendering
 * is enabled. Uses caller X/Y, side length eight times g_WorldDisplayScale, depth 0,
 * reciprocal W 1, caller diffuse color and black specular.
 */
// FUNCTION: FF7 0x007686fa
void __cdecl World_DrawMiniMapMarker(float x0, float y0, float v0, int colour, struct GfxRenderObject *node)
{
    float tu0;
    float w;
    float h;
    float du;
    int zBits;
    GfxTlVertex *pVert0;
    float vSpan;
    GfxTlVertex *pVert1;
    GfxTlVertex *vertex2;
    GfxTlVertex *vertex3;

    if (AcquireBatchNode(1, node) != 0) {
        if (World_IsTerrainRenderEnabled() != 0) {
            zBits = 0;
            w = g_WorldDisplayScale * 8.0f;
            h = g_WorldDisplayScale * 8.0f;
            tu0 = 0.03125f;
            v0 = v0 / 64.0f;
            du = 0.0625f;
            vSpan = 0.125f;
            pVert0 = (GfxTlVertex *)(node->vertexCursor);
            pVert0->x = x0;
            pVert0->y = y0;
            pVert0->zBits = zBits;
            pVert0->rhwBits = FLOAT_ONE_BITS;
            pVert0->color = colour;
            pVert0->specular = 0xFF000000;
            pVert0->u = tu0;
            pVert0->v = v0;
            pVert1 = (GfxTlVertex *)(node->vertexCursor + 0x20);
            pVert1->x = x0;
            pVert1->y = y0 + h;
            pVert1->zBits = zBits;
            pVert1->rhwBits = FLOAT_ONE_BITS;
            pVert1->color = colour;
            pVert1->specular = 0xFF000000;
            pVert1->u = tu0;
            pVert1->v = v0 + vSpan;
            vertex2 = (GfxTlVertex *)(node->vertexCursor + 0x40);
            vertex2->x = x0 + w;
            vertex2->y = y0;
            vertex2->zBits = zBits;
            vertex2->rhwBits = FLOAT_ONE_BITS;
            vertex2->color = colour;
            vertex2->specular = 0xFF000000;
            vertex2->u = tu0 + du;
            vertex2->v = v0;
            vertex3 = (GfxTlVertex *)(node->vertexCursor + 0x60);
            vertex3->x = x0 + w;
            vertex3->y = y0 + h;
            vertex3->zBits = zBits;
            vertex3->rhwBits = FLOAT_ONE_BITS;
            vertex3->color = colour;
            vertex3->specular = 0xFF000000;
            vertex3->u = tu0 + du;
            vertex3->v = v0 + vSpan;
        }
    }
}
} /* extern "C" */
