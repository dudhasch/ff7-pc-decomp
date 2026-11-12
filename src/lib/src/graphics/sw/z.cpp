// src/lib/src/graphics/sw/z.cpp
// Original: C:\lib\src\graphics\sw\z.cpp

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/api/mem.h"
#include "lib/api/graphics.h"
#include "lib/types/graphics.h"
#include "lib/enums/graphics.h"
extern "C" {

/*
 * Frees a software z-buffer: its buffer when ownsBuffer is set, then the record. Null is accepted.
 */
// FUNCTION: FF7 0x006ab9c0
void __cdecl Gfx_RasterFreeZBuffer(GfxRasterZBuffer *zBuffer)
{
    if (zBuffer != 0) {
        if (zBuffer->ownsBuffer != 0) {
            // STRING: FF7 0x00917c20
            GameHeapFree((void *)zBuffer->buffer, "C:\\lib\\src\\graphics\\sw\\z.cpp", 0x12);
        }
        // STRING: FF7 0x00917c40
        GameHeapFree(zBuffer, "C:\\lib\\src\\graphics\\sw\\z.cpp", 0x15);
    }
}

/*
 * Allocates and initializes a software z-buffer record and buffer.
 */
// FUNCTION: FF7 0x006ab9fd
int __cdecl Gfx_RasterCreateZBuffer(int width, int height, int bytesPerPixel, int format)
{
    GfxRasterZBuffer *zbuf;

    // STRING: FF7 0x00917c60
    zbuf = (GfxRasterZBuffer *)GameHeapAlloc(1, 0x30, "C:\\lib\\src\\graphics\\sw\\z.cpp", 0x1D);
    if (zbuf != 0) {
        zbuf->depthScale = 1;
        zbuf->clearValue = format;
        zbuf->bytesPerPixel = bytesPerPixel;
        zbuf->byteSize = width * height * bytesPerPixel;
        zbuf->stride = width * bytesPerPixel;
        zbuf->ownsBuffer = 1;
        // STRING: FF7 0x00917c80
        zbuf->buffer = (int)GameHeapAlloc(width * height, bytesPerPixel, "C:\\lib\\src\\graphics\\sw\\z.cpp", 0x28);
        if (zbuf->buffer == 0) {
            Gfx_RasterFreeZBuffer(zbuf);
            zbuf = 0;
        }
    }
    return (int)zbuf;
}

// Creates a software z-buffer record of 0x30 bytes over a caller-supplied store. It sets depthScale
// to 1, clearValue to format, bytesPerPixel, byteSize to width * height * bytesPerPixel, stride, and
// buffer to store, and clears ownsBuffer so the record does not own its buffer. A null store destroys
// the record through Gfx_RasterFreeZBuffer and returns 0.
// FUNCTION: FF7 0x006abaa9
int __cdecl Gfx_RasterCreateZBufferWithStore(int width, int height, int bytesPerPixel, int format, int stride, int store)
{
    GfxRasterZBuffer *zbuf;

    // STRING: FF7 0x00917ca0
    zbuf = (GfxRasterZBuffer *)GameHeapAlloc(1, 0x30, "C:\\lib\\src\\graphics\\sw\\z.cpp", 0x37);
    if (zbuf != 0) {
        zbuf->depthScale = 1;
        zbuf->clearValue = format;
        zbuf->bytesPerPixel = bytesPerPixel;
        zbuf->byteSize = width * height * bytesPerPixel;
        zbuf->stride = stride;
        zbuf->ownsBuffer = 0;
        zbuf->buffer = store;
        if (zbuf->buffer == 0) {
            Gfx_RasterFreeZBuffer(zbuf);
            zbuf = 0;
        }
    }
    return (int)zbuf;
}

/*
 * Fills the z-buffer with its clear value (clearValue over byteCount bytes of buffer), but only
 * when zBuffer is non-null and enabled.
 */
// FUNCTION: FF7 0x006abb35
void __cdecl Gfx_RasterClearZBuffer(SwZBuffer *zBuffer)
{
    if (zBuffer != 0 && zBuffer->enabled != 0) {
        Mem_FillDwordsByByteCount(zBuffer->clearValue, zBuffer->byteCount, zBuffer->buffer);
    }
}

/*
 * For a nonnull clearState, decrements countdown while positive. When it is nonpositive,
 * computes the row address from x/y, stride and a pixel width (mode) of 1, 2 or 4, then calls
 * Mem_FillDwordRows for the given rows and the width scaled to bytes, and reloads countdown
 * from interval - 1.
 */
// FUNCTION: FF7 0x006abb66
void __cdecl Gfx_RasterClearZBufferRectIfDue(int x, int y, int width, int rows, GfxZBufferClearState *clearState)
{
    int fillBytes;
    int fillPtr;
    int supported;

    if (clearState != 0) {
        if (clearState->countdown <= 0) {
            supported = 1;
            fillPtr = clearState->buffer;
            switch (clearState->mode) {
            case SW_ZCLEAR_8BIT:
                fillBytes = width;
                fillPtr += x;
                break;
            case SW_ZCLEAR_16BIT:
                fillBytes = width << 1;
                fillPtr += x * 2;
                break;
            case SW_ZCLEAR_32BIT:
                fillBytes = width << 2;
                fillPtr += x * 4;
                break;
            default:
                supported = 0;
            }
            if (supported != 0) {
                fillPtr += y * clearState->stride;
                Mem_FillDwordRows(clearState->clearValue, fillBytes, rows, clearState->stride - fillBytes, (void *)fillPtr);
            }
            clearState->countdown = clearState->interval - 1;
        } else {
            (clearState->countdown)--;
        }
    }
}
} /* extern "C" */
