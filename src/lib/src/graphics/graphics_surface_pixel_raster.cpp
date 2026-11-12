// src/lib/src/graphics/graphics_surface_pixel_raster.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Computes framebuffer pixel addresses, writes 16-bit pixels, draws clipped triangle and
//   indexed wireframes, and copies surface rectangles, including a doubled 16-bit copy.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/types/graphics.h"
#include "lib/codegen.h"
#include "lib/api/graphics.h"
extern "C" {

// The colour words the wireframe edge draws pass to Gfx_RasterClipAndDrawLine16, one per edge.
// GLOBAL: FF7 0x0090f480
int g_GfxWireframeColors[4] = {0, 0xFFFF, 0xF800, 0x3800};

#pragma optimize("p", on)
// The optimize("p") pairs in this file set /Op per function, as the retail build did (lib/codegen.h, "/Op pairs").
#pragma optimize("p", off)

/*
 * Computes a framebuffer pixel address from (x, y) coordinates and context.
 */
// FUNCTION: FF7 0x0067f5f0
int __cdecl Gfx_PixelAddressFromContext(int x, int y, int context)
{
    int pointer;
    if (((GfxEngineContext *)context)->rasterBase->usesAlignedHeap != 0) {
        pointer = ((GfxEngineContext *)context)->rasterBase->alignedBase;
        __asm {
   mov eax,y
   mov ebx,x
   mov edi,pointer
   shl eax,12
   lea eax,[eax+ebx*2]
   add edi,eax
   mov pointer,edi
        }
    } else {
        __asm {
   mov esi,context
   mov eax,y
   mov ebx,x
   mov edx,[esi+07a4h]
   mov edi,[esi+07a8h]
   imul edx
   lea eax,[eax+ebx*2]
   add edi,eax
   mov pointer,edi
        }
    }
    return pointer;
}

/*
 * Writes a 16-bit pixel into the surface at specified coordinates.
 */
// FUNCTION: FF7 0x0067f654
void __cdecl Gfx_PutSurfacePixel16(int x, int y, int color, int surface)
{
    __asm {
 mov eax, y
 mov ebx, x
 mov esi, surface
 mov ecx, color
 mov edx, dword ptr [esi + 07a4h]
 mov edi, dword ptr [esi + 07a8h]
 shl ebx, 1
 imul edx
 add edi, ebx
 mov word ptr [edi + eax], cx
    }
}

/*
 * Plots the three vertices of a triangle onto the surface.
 */
// FUNCTION: FF7 0x0067f681
void __cdecl Gfx_PutTriangleVertexPixels16(int color, GfxTriangleVertexSet *vertices, int surface)
{
    GfxPixelVertexA *vertC;
    GfxPixelVertexA *vertexA;
    GfxPixelVertexA *vertex1;

    vertexA = (GfxPixelVertexA *)(vertices->vertexA);
    vertex1 = (GfxPixelVertexA *)(vertices->vertexB);
    vertC = (GfxPixelVertexA *)(vertices->vertexC);
    if (DEAD_IN_RETAIL != 0) {
        Gfx_PutSurfacePixel16(vertexA->pixelX, vertexA->pixelY, color, surface);
        Gfx_PutSurfacePixel16(vertex1->pixelX, vertex1->pixelY, color, surface);
        Gfx_PutSurfacePixel16(vertC->pixelX, vertC->pixelY, color, surface);
    }
}

/*
 * Draws the three wireframe edges of a triangle with line clipping.
 */
// FUNCTION: FF7 0x0067f705
void __cdecl Gfx_RasterDrawTriangleWireframe(int color, GfxRasterTriangle *triangle, GfxEngineContext *surface)
{
    GfxRasterWireVertex *vertexBPtr;
    GfxRasterWireVertex *vertC;
    GfxRasterWireVertex *vertexA;

    vertexA = (GfxRasterWireVertex *)(triangle->vertex0);
    vertexBPtr = (GfxRasterWireVertex *)(triangle->vertex1);
    vertC = (GfxRasterWireVertex *)(triangle->vertex2);
    if (vertexA->clipFlag != 0 && vertexBPtr->clipFlag != 0 && 0 != 0) {
        Gfx_RasterDrawLine16(vertexA->screenX, vertexA->screenY, vertexBPtr->screenX, vertexBPtr->screenY, color, surface);
    } else {
        Gfx_RasterClipAndDrawLine16(vertexA->screenX, vertexA->screenY, vertexBPtr->screenX, vertexBPtr->screenY, color, surface);
    }
    if (vertexBPtr->clipFlag != 0 && vertC->clipFlag != 0 && 0 != 0) {
        Gfx_RasterDrawLine16(vertexBPtr->screenX, vertexBPtr->screenY, vertC->screenX, vertC->screenY, color, surface);
    } else {
        Gfx_RasterClipAndDrawLine16(vertexBPtr->screenX, vertexBPtr->screenY, vertC->screenX, vertC->screenY, color, surface);
    }
    if (vertexA->clipFlag != 0 && vertC->clipFlag != 0 && 0 != 0) {
        Gfx_RasterDrawLine16(vertexA->screenX, vertexA->screenY, vertC->screenX, vertC->screenY, color, surface);
    } else {
        Gfx_RasterClipAndDrawLine16(vertexA->screenX, vertexA->screenY, vertC->screenX, vertC->screenY, color, surface);
    }
}

/*
 * Draws indexed wireframe edges from vertex list with color cycling.
 */
// FUNCTION: FF7 0x0067f87f
void __cdecl Gfx_RasterDrawWireEdges(GfxWireMesh *mesh, GfxEngineContext *surface)
{
    GfxWireEndVertex *startVertex;
    int edgeIdx;
    GfxWireEndVertex *endVertex;
    int startIndex;
    int toIdx;
    int iterToIndex;
    int stepStartIdx;

    for (edgeIdx = 0; edgeIdx < mesh->edgeCount; edgeIdx++) {
        startIndex = ((GfxRasterWireEdge *)mesh)[edgeIdx].startIndex;
        toIdx = ((GfxRasterWireEdge *)mesh)[edgeIdx].endIndex;
        startVertex = (GfxWireEndVertex *)(((int *)mesh)[startIndex]);
        endVertex = (GfxWireEndVertex *)(((int *)mesh)[toIdx]);
        Gfx_RasterClipAndDrawLine16(startVertex->x, startVertex->y, endVertex->x, endVertex->y, g_GfxWireframeColors[edgeIdx], surface);
    }
    for (edgeIdx = 0; edgeIdx < mesh->secondEdgeCount; edgeIdx++) {
        stepStartIdx = ((GfxRasterWireEdge *)((char *)mesh + 0x48))[edgeIdx].startIndex;
        iterToIndex = ((GfxRasterWireEdge *)((char *)mesh + 0x48))[edgeIdx].endIndex;
        startVertex = (GfxWireEndVertex *)(((int *)mesh)[stepStartIdx]);
        endVertex = (GfxWireEndVertex *)(((int *)mesh)[iterToIndex]);
        Gfx_RasterClipAndDrawLine16(startVertex->x, startVertex->y, endVertex->x, endVertex->y, g_GfxWireframeColors[edgeIdx], surface);
    }
}

/*
 * Copies a rectangular surface region using 32-bit dword moves.
 */
// FUNCTION: FF7 0x0067f9a3
void __cdecl Gfx_CopySurfaceRectDwords(int widthBytes, int height, int srcX, int srcY, int srcPitch, int srcBase, int dstX, int dstY, int dstPitch, int dstBase)
{
    __asm {
 mov eax, srcY
 mov edx, srcPitch
 imul edx
 mov esi, srcBase
 add esi, eax
 mov eax, srcX
 lea esi, [esi + eax*2]
 mov srcBase, esi
 mov eax, dstY
 mov edx, dstPitch
 imul edx
 mov edi, dstBase
 add edi, eax
 mov eax, dstX
 lea edi, [edi + eax*2]
 mov dstBase, edi
copy_row_loop:
 mov esi, srcBase
 mov edi, dstBase
 mov ecx, widthBytes
 shr ecx, 1
copy_dword_loop:
 mov eax, dword ptr [esi]
 add esi, 4
 mov dword ptr [edi], eax
 add edi, 4
 dec ecx
 jg copy_dword_loop
 mov esi, srcBase
 add esi, srcPitch
 mov srcBase, esi
 mov edi, dstBase
 add edi, dstPitch
 mov dstBase, edi
 dec height
 jg copy_row_loop
    }
}

/*
 * Copies and 2x scales a 16-bit surface rectangle to two destination rows.
 */
// FUNCTION: FF7 0x0067fa09
void __cdecl Gfx_CopySurfaceRectDoubled16(int widthPixels, int height, int srcX, int srcY, int srcPitch, int srcBase, int dstX, int dstY, int dstPitch, int dstBase)
{
    __asm {
 mov eax, srcY
 mov edx, srcPitch
 imul edx
 mov esi, srcBase
 add esi, eax
 mov eax, srcX
 lea esi, [esi + eax*2]
 mov srcBase, esi
 mov eax, dstY
 mov edx, dstPitch
 imul edx
 mov edi, dstBase
 add edi, eax
 mov eax, dstX
 lea edi, [edi + eax*2]
 mov dstBase, edi
scale_row_loop:
 mov esi, srcBase
 mov edi, dstBase
 mov edx, edi
 add edx, dstPitch
 mov ecx, widthPixels
scale_pixel_loop:
 xor eax, eax
 mov ax, word ptr [esi]
 add esi, 2
 mov ebx, eax
 rol ebx, 0x10
 or eax, ebx
 mov dword ptr [edi], eax
 add edi, 4
 mov dword ptr [edx], eax
 add edx, 4
 dec ecx
 jg scale_pixel_loop
 mov esi, srcBase
 add esi, srcPitch
 mov srcBase, esi
 mov edi, dstBase
 add edi, dstPitch
 add edi, dstPitch
 mov dstBase, edi
 dec height
 jg scale_row_loop
    }
}
} /* extern "C" */
