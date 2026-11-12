// src/ff7/chocobo/ch_ddraw.cpp
// Original: C:\FF7\chocobo\ch_ddraw.cpp

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/api/mem.h"
#include "ff7/api/chocobo.h"
#include "lib/api/graphics.h"
#include "lib/api/sound.h"
#include "ff7/types/chocobo.h"
#include "lib/types/polygon.h"
#include "lib/types/graphics.h"
#include "lib/codegen.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "ff7/gte.h"
#include <windows.h>
#include <ddraw.h>

struct GfxFloatColorAddend;
struct PolygonModelVertexColors;
extern "C" {

// clang-format off
// GLOBAL: FF7 0x00e73c10
int g_ChocoboPixelFormatMode = 0;
// GLOBAL: FF7 0x00e73c14
int g_ChocoboPixelRedMask = 0;
// GLOBAL: FF7 0x00e73c18
int g_ChocoboPixelGreenMask = 0;
// GLOBAL: FF7 0x00e73c1c
int g_ChocoboPixelBlueMask = 0;
// GLOBAL: FF7 0x00e73c20
int g_ChocoboDirectDraw = 0;
// GLOBAL: FF7 0x00e73c24
int g_ChocoboPrimarySurface = 0;
// GLOBAL: FF7 0x00e73c28
int g_ChocoboBackSurface = 0;
unsigned char g_rawData_00e73c2c[4] = {0};
// clang-format on

#pragma intrinsic(sqrt, atan2)

#pragma intrinsic(strcpy, strcat)


/* Unprototyped retail call signature (parameter/return width differs from callee definition). */
extern void __cdecl Gfx_AddColorToModelVertexColors(struct GfxFloatColorAddend *, struct PolygonModelVertexColors *, int);
/* Caches three DirectDraw interface pointers out of the engine context into globals. */
// FUNCTION: FF7 0x007794f0
void __cdecl Mini_ChocoboCacheDDrawIfaces(GfxEngineContext *engineCtx)
{
    (*(struct IDirectDraw **)&g_ChocoboDirectDraw) = *(struct IDirectDraw **)&engineCtx->ddObject;
    g_ChocoboPrimarySurface = engineCtx->primarySurface;
    g_ChocoboBackSurface = engineCtx->backSurface;
}
/* Sets graphic init flag, initializes graphics device for engine context, and caches pixel format. */
// FUNCTION: FF7 0x0077951b
int __cdecl Mini_ChocoboInitGraphicsDevice(void)
{
    GfxEngineContext *ctx;

    ctx = GetEngineContext();
    if (InitGraphicsDevice(ctx) != 0) {
        Mini_ChocoboCacheDDrawIfaces(ctx);
    }
    return 1;
}
/* Shuts down the graphics device if active and clears cached surface pointers. */
// FUNCTION: FF7 0x0077954c
void __cdecl Mini_ChocoboShutdownGraphicsDevice(void)
{
    GfxEngineContext *ctx;

    if ((*(struct IDirectDraw **)&g_ChocoboDirectDraw) != 0) {
        ctx = GetEngineContext();
        ShutdownGraphicsDevice(ctx);
        (*(struct IDirectDraw **)&g_ChocoboDirectDraw) = 0;
        g_ChocoboPrimarySurface = 0;
    }
}
/* Inspects engine context pixel format structure and caches format mode and color bitmasks. */
// FUNCTION: FF7 0x00779585
int __cdecl Mini_ChocoboCachePixelFormat(void *engineCtx)
{
    GfxPixelFormat *pixelFormat;

    pixelFormat = &((GfxEngineContext *)engineCtx)->targetFormat;
    if (pixelFormat->redBits == 5 && pixelFormat->greenBits == 6 && pixelFormat->blueBits == 5) {
        g_ChocoboPixelFormatMode = 0;
    } else {
        g_ChocoboPixelFormatMode = 1;
    }
    g_ChocoboPixelRedMask = pixelFormat->redMask;
    g_ChocoboPixelGreenMask = pixelFormat->greenMask;
    g_ChocoboPixelBlueMask = pixelFormat->blueMask;
    return 1;
}
/* Presents the current frame using the active engine context. */
// FUNCTION: FF7 0x007795f1
int __cdecl Mini_ChocoboPresentFrame(void)
{
    GfxEngineContext *ctx;

    ctx = GetEngineContext();
    PresentFrame(ctx);
    return 1;
}

#if 0
// Exact interior views of the two compiler-emitted strcat literals.
// STRING: FF7 0x00971c94
"data\\long\\lsprite";
// STRING: FF7 0x00971c98
"\\long\\lsprite";
// STRING: FF7 0x00971c9c
"g\\lsprite";
// STRING: FF7 0x00971ca0
"prite";
// STRING: FF7 0x00971ca4
"e";
// STRING: FF7 0x00971ca8
"data\\short\\ssprite";
// STRING: FF7 0x00971cac
"\\short\\ssprite";
// STRING: FF7 0x00971cb0
"rt\\ssprite";
// STRING: FF7 0x00971cb4
"sprite";
// STRING: FF7 0x00971cb8
"te";
#endif
// DirectDraw helpers from the Chocobo source range. These require C++ virtual-call ordering;
// the C interface macros fetch the vtable after pushing arguments and do not match VC5's output.
#pragma intrinsic(memset)
void __cdecl Mini_ChocoboSetSurfaceColorKey(LPDIRECTDRAWSURFACE surface, int colour, int flags);
/* Locks a DirectDraw surface with DDLOCK_WAIT and extracts pixel buffer pointer and pitch. */
// FUNCTION: FF7 0x00779612
int __cdecl Mini_ChocoboLockSurfaceForPixels(LPDIRECTDRAWSURFACE surface, void **bits, unsigned int *pitch)
{
    HRESULT result;
    DDSURFACEDESC desc;
    memset(&desc, 0, sizeof(desc));
    desc.dwSize = sizeof(desc);
    result = surface->Lock(NULL, &desc, DDLOCK_WAIT, NULL);
    if (result != DD_OK) {
        return 0;
    }
    *bits = desc.lpSurface;
    *pitch = desc.lPitch;
    return result == DD_OK;
}
/* Unlocks a previously locked DirectDraw surface. */
// FUNCTION: FF7 0x0077966d
int __cdecl Mini_ChocoboUnlockSurface(LPDIRECTDRAWSURFACE surface)
{
    surface->Unlock(NULL);
    return 1;
}
/* Performs a DirectDraw blit with retry logic upon surface loss. */
// FUNCTION: FF7 0x00779688
int __cdecl Mini_ChocoboBltRetryWhileSurfaceLost(LPDIRECTDRAWSURFACE dst, LPDIRECTDRAWSURFACE src, LPRECT srcRect, LPRECT dstRect, DWORD flags, LPDDBLTFX fx)
{
    struct {
        HRESULT result;
        LPDIRECTDRAWSURFACE current;
    } local; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    while (1) {
        local.current = dst;
        local.result = local.current->Blt(dstRect, src, srcRect, flags, fx);
        switch (local.result) {
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
/* Performs a DirectDraw fast blit with retry logic upon surface loss. */
// FUNCTION: FF7 0x007796ff
int __cdecl Mini_ChocoboRetryBltFastOnSurfaceLost(LPDIRECTDRAWSURFACE dst, LPDIRECTDRAWSURFACE src, LPRECT srcRect, LPRECT dstRect, DWORD flags)
{
    struct {
        HRESULT result;
        LPDIRECTDRAWSURFACE current;
    } local;
    while (1) {
        local.current = dst;
        local.result = local.current->BltFast(dstRect->left, dstRect->top, src, srcRect, flags);
        switch (local.result) {
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
/* Sets color key on a DirectDraw surface using specified color value and flags. */
// FUNCTION: FF7 0x0077977b
void __cdecl Mini_ChocoboSetSurfaceColorKey(LPDIRECTDRAWSURFACE surface, int colour, int flags)
{
    DDCOLORKEY key = {colour, colour};
    surface->SetColorKey(flags, &key);
}
/* Creates an offscreen system-memory plain DirectDraw surface with given dimensions. */
// FUNCTION: FF7 0x007797a5
LPDIRECTDRAWSURFACE __cdecl Mini_ChocoboCreateSystemMemorySurface(int width, int height)
{
    LPDIRECTDRAWSURFACE surface;
    DDSURFACEDESC desc;
    HRESULT result;
    desc.dwSize = sizeof(desc);
    desc.dwFlags = DDSD_WIDTH | DDSD_HEIGHT | DDSD_CAPS;
    desc.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
    desc.ddsCaps.dwCaps |= DDSCAPS_SYSTEMMEMORY;
    desc.dwWidth = width;
    desc.dwHeight = height;
    result = (*(struct IDirectDraw **)&g_ChocoboDirectDraw)->CreateSurface(&desc, &surface, NULL);
    if (result != DD_OK) {
        return NULL;
    }
    Mini_ChocoboSetSurfaceColorKey(surface, 0, DDCKEY_SRCBLT);
    return surface;
}

/* Allocates a 16-bit surface buffer in game heap. */
// FUNCTION: FF7 0x00779815
int __cdecl Mini_ChocoboAllocSurface16Bit(int width, int height)
{
    int buffer;

    // STRING: FF7 0x0097c748
    buffer = (int)(GameHeapAllocRaw(width * height << 1, "C:\\FF7\\chocobo\\ch_ddraw.cpp", 228));
    if (buffer == 0) {
        return 0;
    } else {
        return buffer;
    }
}
} /* extern "C" */
