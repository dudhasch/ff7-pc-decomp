// src/ff7/condor/cd_ddraw.cpp
// Original: C:\FF7\condor\cd_ddraw.cpp
//
// One original contribution. VC5 emits .text with 16-byte alignment, so every input contribution
// starts on a 16-byte boundary. Mini_CondorCacheDirectDrawObjects (0x005fa660) is the only aligned
// member start between the preceding CC run and the five CC after Mini_CondorClearTopAndBottomBands
// (0x005faa91), and the file anchor at Mini_CondorAllocSurfaceBuffer16 (0x005fa985) lies inside it.
// All fourteen members therefore come from cd_ddraw.cpp, which is compiled here as C++
// for its DirectDraw COM calls. The section below reproduces the
// contribution, so LINK supplies the original trailing CC.

#include "lib/api/mem.h"
#include <windows.h>
#include <ddraw.h>
#include <string.h>
#include "lib/api/graphics.h"
#include "ff7/types/condor.h"
#include "lib/types/graphics.h"
#include "ff7/api/condor.h"

#pragma intrinsic(memset)

extern "C" {
// clang-format on

#pragma code_seg(".text")

/*
 * Caches DirectDraw interface and surface pointers from the engine context.
 */
// FUNCTION: FF7 0x005fa660
void __cdecl Mini_CondorCacheDirectDrawObjects(struct GfxEngineContext *context)
{
    g_CondorDirectDraw = context->ddObject;
    g_CondorPrimarySurface = context->primarySurface;
    g_CondorBackSurface = context->backSurface;
}

/*
 * Initializes graphics device and caches DirectDraw objects.
 */
// FUNCTION: FF7 0x005fa68b
int __cdecl Mini_CondorInitGraphicsDevice(void)
{
    GfxEngineContext *ctx;

    ctx = GetEngineContext();
    if (InitGraphicsDevice(ctx) != 0) {
        Mini_CondorCacheDirectDrawObjects(ctx);
    }
    return 1;
}

/*
 * Shuts down the DirectDraw graphics device and clears cached pointers.
 */
// FUNCTION: FF7 0x005fa6bc
void __cdecl Mini_CondorShutdownGraphicsDevice(void)
{
    GfxEngineContext *ctx;

    if (g_CondorDirectDraw != 0) {
        ctx = GetEngineContext();
        ShutdownGraphicsDevice(ctx);
        g_CondorDirectDraw = 0;
        g_CondorPrimarySurface = 0;
    }
}

/*
 * Queries and caches display RGB bit masks from the engine context.
 */
// FUNCTION: FF7 0x005fa6f5
int __cdecl Mini_CondorCacheDisplayFormat(struct GfxEngineContext *context)
{
    GfxPixelFormat *pixelFormat;

    pixelFormat = &context->targetFormat;
    if (pixelFormat->redBits == 5 && pixelFormat->greenBits == 6 && pixelFormat->blueBits == 5) {
        g_CondorDisplayFormatValid = 0;
    } else {
        g_CondorDisplayFormatValid = 1;
    }
    g_CondorPixelMaskRed = pixelFormat->redMask;
    g_CondorPixelMaskGreen = pixelFormat->greenMask;
    g_CondorPixelMaskBlue = pixelFormat->blueMask;
    return 1;
}

/*
 * Presents the current rendered frame via DirectDraw.
 */
// FUNCTION: FF7 0x005fa761
int __cdecl Mini_CondorPresentFrame(void)
{
    GfxEngineContext *ctx;

    ctx = GetEngineContext();
    PresentFrame(ctx);
    return 1;
}

/*
 * Locks a DirectDraw surface and returns pixel pointer and pitch.
 */
// FUNCTION: FF7 0x005fa782
int __cdecl Mini_CondorLockSurfaceBits(LPDIRECTDRAWSURFACE surface, void **bits, unsigned int *pitch)
{
    HRESULT result;
    DDSURFACEDESC desc;
    memset(&desc, 0, sizeof(desc));
    desc.dwSize = sizeof(desc);
    result = surface->Lock(NULL, &desc, DDLOCK_WAIT, NULL);
    if (result != 0) {
        return 0;
    }
    *bits = desc.lpSurface;
    *pitch = desc.lPitch;
    return result == 0;
}

/*
 * Unlocks a previously locked DirectDraw surface.
 */
// FUNCTION: FF7 0x005fa7dd
int __cdecl Mini_CondorUnlockSurface(LPDIRECTDRAWSURFACE surface)
{
    surface->Unlock(NULL);
    return 1;
}

/*
 * Performs a surface Blt operation, retrying if the surface was lost.
 */
// FUNCTION: FF7 0x005fa7f8
int __cdecl Mini_CondorRetryBltOnSurfaceLost(LPDIRECTDRAWSURFACE dstSurface, LPDIRECTDRAWSURFACE srcSurface, RECT *srcRect, RECT *destRect, int flags, DDBLTFX *fx)
{
    LPDIRECTDRAWSURFACE surface;
    int res;
    while (1) { /* Constant condition kept for /Od branch/register layout; see lib/codegen.h. */
        surface = dstSurface;
        res = surface->Blt(destRect, srcSurface, srcRect, flags, fx);
        switch (res) {
        case DD_OK:
            return 1;
        case DDERR_SURFACELOST:
            break;
        case DDERR_WASSTILLDRAWING:
            return 1;
        default:
            return 0;
        }
    }
    return 1;
}

/*
 * Performs a surface BltFast operation, retrying if the surface was lost.
 */
// FUNCTION: FF7 0x005fa86f
int __cdecl Mini_CondorBltFastRetryOnSurfaceLost(LPDIRECTDRAWSURFACE dstSurface, LPDIRECTDRAWSURFACE srcSurface, RECT *srcRect, int *destPoint, int flags)
{
    LPDIRECTDRAWSURFACE surface;
    int res;
    while (1) { /* Constant condition kept for /Od branch/register layout; see lib/codegen.h. */
        surface = dstSurface;
        res = surface->BltFast(destPoint[0], destPoint[1], srcSurface, srcRect, flags);
        switch (res) {
        case DD_OK:
            return 1;
        case DDERR_SURFACELOST:
            break;
        case DDERR_WASSTILLDRAWING:
            return 1;
        default:
            return 0;
        }
    }
    return 1;
}

/*
 * Sets the color key for a DirectDraw surface to a specified value.
 */
// FUNCTION: FF7 0x005fa8eb
void __cdecl Mini_CondorSetSurfaceSingleColorKey(LPDIRECTDRAWSURFACE surface, int colour, int flags)
{
    DDCOLORKEY key = {colour, colour};
    surface->SetColorKey(flags, &key);
}

/*
 * Creates a system-memory offscreen DirectDraw surface with given dimensions.
 */
// FUNCTION: FF7 0x005fa915
LPDIRECTDRAWSURFACE __cdecl Mini_CondorCreateSystemOffscreenSurface(int width, int height)
{
    LPDIRECTDRAWSURFACE surface;
    DDSURFACEDESC ddsd;
    HRESULT result;
    ddsd.dwSize = sizeof(ddsd);
    ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
    ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
    ddsd.ddsCaps.dwCaps |= DDSCAPS_SYSTEMMEMORY;
    ddsd.dwWidth = width;
    ddsd.dwHeight = height;
    result = ((LPDIRECTDRAW)g_CondorDirectDraw)->CreateSurface(&ddsd, &surface, NULL);
    if (result != 0)
        return 0;
    Mini_CondorSetSurfaceSingleColorKey(surface, 0, DDCKEY_SRCBLT);
    return surface;
}

/*
 * Allocates a 16-bit surface memory buffer on the game heap.
 */
// FUNCTION: FF7 0x005fa985
int __cdecl Mini_CondorAllocSurfaceBuffer16(int width, int height)
{
    int buffer;

    // STRING: FF7 0x00905464
    buffer = (int)GameHeapAllocRaw(width * height << 1, "C:\\FF7\\condor\\cd_ddraw.cpp", 0xE1);
    if (buffer == 0) {
        return 0;
    } else {
        return buffer;
    }
}

/*
 * Fills two horizontal rows of a 16-bit surface with a duplicated color pattern.
 */
// FUNCTION: FF7 0x005fa9b9
void __cdecl Mini_CondorFillTwoRowsColor16(int surface, int row, int color)
{
    int *flags;
    int pair;
    void *lockedFlags;
    unsigned short column;
    unsigned int pitch;

    lockedFlags = 0;
    if (surface == 0) {
        return;
    }
    row += 0x14;
    if (Mini_CondorLockSurfaceBits((LPDIRECTDRAWSURFACE)surface, &lockedFlags, &pitch) != 0) {
        pair = (color & 0xFFFF) << 0x10 | color & 0xFFFF;
        flags = (int *)lockedFlags;
        for (column = 0; column < 0x140; column++) {
            flags[(row * pitch >> 2) + column] = pair;
            flags[(row * pitch >> 2) + (pitch >> 2) + column] = pair;
        }
        Mini_CondorUnlockSurface((LPDIRECTDRAWSURFACE)surface);
    }
}

/*
 * Clears the top and bottom 20-row letterbox bands on the back surface.
 */
// FUNCTION: FF7 0x005faa91
void __cdecl Mini_CondorClearTopAndBottomBands(void)
{
    void *pixelFlags;
    char *basePointer;
    char *rowPtr;
    unsigned short rowIndex;
    unsigned int pitch;

    rowPtr = 0;
    basePointer = 0;
    if (Mini_CondorLockSurfaceBits((LPDIRECTDRAWSURFACE)g_CondorBackSurface, &pixelFlags, &pitch) == 0) {
        return;
    }
    rowPtr = (char *)pixelFlags;
    basePointer = rowPtr;
    for (rowIndex = 0; rowIndex < 0x14; rowIndex++) {
        memset(rowPtr, 0, 0x500);
        rowPtr += pitch;
    }
    rowPtr = basePointer + pitch * 0x1CC;
    for (rowIndex = 0x1CC; rowIndex < 0x1E0; rowIndex++) {
        memset(rowPtr, 0, 0x500);
        rowPtr += pitch;
    }
    Mini_CondorUnlockSurface((LPDIRECTDRAWSURFACE)g_CondorBackSurface);
}

#pragma code_seg()
} // extern "C"
