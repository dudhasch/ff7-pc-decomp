// src/ff7/field/src/ad_ddraw.cpp
// Original: C:\FF7\field\src\ad_ddraw.cpp

// The IDirectDrawSurface calls of the field's DirectDraw code, compiled as C++: a virtual call such as
// `surface->Unlock(NULL)` fetches the vtable before pushing `this`, an order no C spelling produces. The
// original ad_*.cpp were C++, as is src/ff7/coaster/exact.cpp. Names keep C linkage so the .c
// neighbours call them unchanged.

#include "lib/api/mem.h"
#include <windows.h>
#include <ddraw.h>
#include <string.h>
#include "ff7/api/field.h"
#include "lib/api/graphics.h"
#include "ff7/types/field.h"
#include "lib/types/graphics.h"
#include "lib/enums/graphics.h"

// The texture-page row copy must be the memcpy intrinsic: a struct assignment orders its registers differently.
/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
extern "C" {

#define g_FieldPaletteGridStride (*(unsigned short *)((char *)&g_FieldPaletteGridHeight + 2))
// GLOBAL: FF7 0x00cffc70
FieldLayer *g_FieldLayers[0x2a] = {0};
unsigned char g_bssPad_00cffd18[8] = {0};
// The pixel format Field_BuildBrightnessTables builds its four LUTs through (Trans_BuildScaledLut and
// the two Trans_BuildDoubleBrightLut* read it whole, through UnpackPixelToArgb8888 and
// PackArgb8888ToPixelFormat). Nothing in src/ writes it, so the tables are built from a zero format.
// GLOBAL: FF7 0x00cffd20
GfxPixelFormat g_FieldBrightnessLutFormat = {0};
// GLOBAL: FF7 0x00cffda0
int g_FieldTileColorRed = 0;
// GLOBAL: FF7 0x00cffda4
int g_FieldTileColorGreen = 0;
// GLOBAL: FF7 0x00cffda8
int g_FieldTileColorBlue = 0;
// Surface flag read by this TU's code at 0x6424F2, immediately ahead of the 8-byte-aligned block
// below.
// GLOBAL: FF7 0x00cffdac
int g_FieldTileStateDac = 0;
unsigned char g_bssPad_00cffdb0[8] = {0};
// GLOBAL: FF7 0x00cffdb8
int g_FieldTileDumpPaletteFlag = 0;
// GLOBAL: FF7 0x00cffdbc
int g_FieldTileStateDbc = 0;
// GLOBAL: FF7 0x00cffdc0
unsigned short g_FieldTileStateDc0 = 0;
// GLOBAL: FF7 0x00cffdc4
int g_FieldTileStateDc4 = 0;
// clang-format off
// GLOBAL: FF7 0x00cffdc8
LPDIRECTDRAW g_pFieldDirectDraw = 0;
// GLOBAL: FF7 0x00cffdcc
LPDIRECTDRAWSURFACE g_pFieldPrimarySurface = 0;
// GLOBAL: FF7 0x00cffdd0
int g_pFieldBackSurface = 0;
// GLOBAL: FF7 0x00cffdd4
int g_FieldDDrawSurfaceDd4 = 0;
// GLOBAL: FF7 0x00cffdd8
int g_FieldDDrawSurfaceDd8 = 0;
unsigned char g_bssPad_00cffddc[4] = {0};
// GLOBAL: FF7 0x00cffde0
int g_FieldSurfaceColorKey = 0;
// The blit's destination X.
// GLOBAL: FF7 0x00cffde4
unsigned short g_FieldSurfaceDestX = 0;
// clang-format on
// These helpers take DirectDraw surface pointers, so this TU declares them with its own signatures.
extern int __cdecl Field_LockSurfaceBits(LPDIRECTDRAWSURFACE surface, void **bits, int *pitch);
int __cdecl Field_UnlockSurface(LPDIRECTDRAWSURFACE surface);
void __cdecl Field_ReleaseSurfaceIfPresent(LPDIRECTDRAWSURFACE surface);
int __cdecl Field_RetryBltOnSurfaceLost(LPDIRECTDRAWSURFACE dstSurface, LPDIRECTDRAWSURFACE srcSurface, RECT *srcRect, RECT *dstRect, int bltFlags, DDBLTFX *bltFx);
int __cdecl Field_BltFastAtPoint(LPDIRECTDRAWSURFACE dstSurface, LPDIRECTDRAWSURFACE srcSurface, RECT *srcRect, int *point, int bltFlags);
void __cdecl Field_SetSurfaceSingleColorKey(LPDIRECTDRAWSURFACE surface, int colour, int flags);
LPDIRECTDRAWSURFACE __cdecl Field_CreateSystemOffscreenSurface(int width, int height);
#pragma intrinsic(memset)
#pragma intrinsic(strcpy)
#pragma intrinsic(memcpy, memset)

/*
 * Unused; returns 1.
 */
// FUNCTION: FF7 0x00641760
int __cdecl Field_UnusedReturnOne_2(void)
{
    return 1;
}

// Copies the DirectDraw object and the primary and back surfaces out of the GfxEngineContext
// (directDraw, primarySurface and backSurface) into g_pFieldDirectDraw, g_pFieldPrimarySurface and
// g_pFieldBackSurface, and sets g_FieldTileStateDbc to 1.
// FUNCTION: FF7 0x0064176a
void __cdecl Field_CacheDirectDrawSurfaceHandles(GfxEngineContext *context)
{
    g_pFieldDirectDraw = (LPDIRECTDRAW)context->ddObject;
    g_pFieldPrimarySurface = (LPDIRECTDRAWSURFACE)context->primarySurface;
    g_pFieldBackSurface = context->backSurface;
    g_FieldTileStateDbc = 1;
}

/*
 * Initialises the graphics device on the engine context and, when that succeeds, caches its
 * DirectDraw handles through Field_CacheDirectDrawSurfaceHandles. Returns 1 on success and 0 otherwise.
 */
// FUNCTION: FF7 0x0064179f
int __cdecl Field_InitGraphicsDeviceAndCacheSurfaces(void)
{
    GfxEngineContext *ctx;

    ctx = GetEngineContext();
    if (InitGraphicsDevice(ctx) != 0) {
        Field_CacheDirectDrawSurfaceHandles(ctx);
    } else {
        return 0;
    }
    return 1;
}

/*
 * Empty.
 */
// FUNCTION: FF7 0x006417d6
void __cdecl Field_ReleaseGraphicsNoOp(void)
{
    int unused; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
}

/*
 * A window procedure that only forwards to DefWindowProcA. __stdcall like every WNDPROC, and the
 * import is called through the IAT.
 */
// FUNCTION: FF7 0x006417de
int __stdcall Field_DefaultWindowProc(void *hWnd, unsigned int msg, unsigned int wParam, int lParam)
{
    return DefWindowProcA(hWnd, msg, wParam, lParam);
}

/*
 * Returns zero for null DirectDraw global; sets display mode, creates a primary surface with
 * back-buffer count one and retrieves its attached back surface. Each failed step copies a literal
 * error to errorMsg and returns zero without cleanup here. On success clears palette global, calls
 * Field_SetSurfaceSingleColorKey with back surface, null, eight and returns one.
 */
// FUNCTION: FF7 0x006417fb
int __cdecl Field_SetDisplayModeAndCreateSurfaces(int width, int height, int bpp, char *errorMsg)
{
    HRESULT result;
    DDSURFACEDESC desc;
    DDSCAPS caps;
    if (g_pFieldDirectDraw == 0)
        return 0;
    result = g_pFieldDirectDraw->SetDisplayMode(width, height, bpp);
    if (result != 0) {
        // STRING: FF7 0x00907d8c
        strcpy(errorMsg, "this video mode is not supported!");
        return 0;
    }
    desc.dwSize = sizeof(desc);
    desc.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
    desc.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP | DDSCAPS_COMPLEX;
    desc.dwBackBufferCount = 1;
    result = g_pFieldDirectDraw->CreateSurface(&desc, &g_pFieldPrimarySurface, 0);
    if (result != 0) {
        // STRING: FF7 0x00907db0
        strcpy(errorMsg, "Fail to create primary surface!");
        return 0;
    }
    caps.dwCaps = DDSCAPS_BACKBUFFER;
    result = g_pFieldPrimarySurface->GetAttachedSurface(&caps, (LPDIRECTDRAWSURFACE *)&g_pFieldBackSurface);
    if (result != 0) {
        // STRING: FF7 0x00907dd0
        strcpy(errorMsg, "Fail to create back surface!");
        return 0;
    }
    g_FieldSurfaceColorKey = 0;
    Field_SetSurfaceSingleColorKey((LPDIRECTDRAWSURFACE)g_pFieldBackSurface, g_FieldSurfaceColorKey, 8);
    return 1;
}

/*
 * Unused; returns 1.
 */
// FUNCTION: FF7 0x00641951
int __cdecl Field_UnusedReturnOne_3(void)
{
    return 1;
}

// Reads the render target's pixel format (context->targetFormat). g_FieldTileDumpPaletteFlag becomes 1 only
// when redBits, greenBits and blueBits are 5, 6 and 5 (an R5G6B5 layout), and 0 otherwise. redMask,
// greenMask and blueMask are copied into g_FieldTileColorRed, g_FieldTileColorGreen and
// g_FieldTileColorBlue. Always returns 1.
// FUNCTION: FF7 0x0064195b
int __cdecl Field_ReadSurfacePixelFormat(struct GfxEngineContext *context)
{
    GfxPixelFormat *format;

    format = &context->targetFormat;
    if (format->redBits == 5 && format->greenBits == 6 && format->blueBits == 5) {
        g_FieldTileDumpPaletteFlag = 1;
    } else {
        g_FieldTileDumpPaletteFlag = 0;
    }
    g_FieldTileColorRed = format->redMask;
    g_FieldTileColorGreen = format->greenMask;
    g_FieldTileColorBlue = format->blueMask;
    return 1;
}

/*
 * Fetches the engine context with GetEngineContext, passes it to PresentFrame and returns 1.
 */
// FUNCTION: FF7 0x006419c7
int __cdecl Field_PresentEngineFrame(void)
{
    GfxEngineContext *ctx;

    ctx = GetEngineContext();
    PresentFrame(ctx);
    return 1;
}

/*
 * Locks the whole surface with DDLOCK_WAIT and hands back its bits pointer
 * and pitch. Returns 1 on success and 0 when Lock fails, leaving the two
 * outputs untouched.
 */
// FUNCTION: FF7 0x006419e8
int __cdecl Field_LockSurfaceBits(LPDIRECTDRAWSURFACE surface, void **bits, int *pitch)
{
    HRESULT hr;
    DDSURFACEDESC surfaceDesc;

    memset(&surfaceDesc, 0, sizeof(surfaceDesc));
    surfaceDesc.dwSize = sizeof(surfaceDesc);
    hr = surface->Lock(NULL, &surfaceDesc, DDLOCK_WAIT, NULL);
    if (hr != 0) {
        return 0;
    }
    *bits = surfaceDesc.lpSurface;
    *pitch = surfaceDesc.lPitch;
    return hr == 0;
}

/*
 * Unlocks the surface and reports success.
 */
// FUNCTION: FF7 0x00641a43
int __cdecl Field_UnlockSurface(LPDIRECTDRAWSURFACE surface)
{
    surface->Unlock(NULL);
    return 1;
}

/*
 * Uploads one of two 48-byte image descriptors to the locked 16-bit field surface, rejecting an
 * invalid selector, missing source pointer, or failed lock. By the TIM pixel mode in
 * g_AdPalWords.second: 4bpp expands low/high nibbles through 12-byte palette entries; 8bpp maps
 * source bytes starting at the global horizontal source offset into destination column zero;
 * 16bpp converts source SHORT pixels with
 * Field_ConvertPixelWordMode argument 2. Rows use source byte width and destination byte pitch.
 * 4bpp repeats the first pixel bound test before its partner write, so an odd pixel limit can
 * be exceeded by one. Missing palettes in 4bpp/8bpp return without unlocking; unknown modes
 * unlock and return success without copying.
 */
// FUNCTION: FF7 0x00641a5e
int __cdecl Field_UploadImageToBackSurface(void *descriptors, unsigned short selector)
{
    struct {
        unsigned short *source16;
        unsigned int pitch;
        unsigned short destX;
        unsigned short reservedX;
        unsigned short column;
        unsigned short reservedY;
        unsigned short row;
        unsigned short reservedZ;
        void *bits;
        unsigned char *pixels;
        unsigned short *target;
        int mode;
        unsigned short value;
        unsigned short reservedValue;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    frame.bits = 0;
    frame.target = 0;
    frame.source16 = 0;
    frame.pixels = 0;
    if (selector >= 2)
        return 0;
    if (((FieldUploadDescriptor *)descriptors)[selector].pixels == 0)
        return 0;
    if (!Field_LockSurfaceBits((LPDIRECTDRAWSURFACE)g_pFieldBackSurface, &frame.bits, (int *)&frame.pitch))
        return 0;
    frame.pixels = (unsigned char *)((FieldUploadDescriptor *)descriptors)[selector].pixels;
    frame.target = (unsigned short *)frame.bits;
    frame.mode = g_AdPalWords.second;
    switch (frame.mode) {
    case TIM_PIXEL_MODE_4BPP:
        if (g_FieldPaletteGridBuffer == 0)
            return 0;
        for (frame.row = 0; frame.row < ((FieldUploadDescriptor *)descriptors)[selector].height; frame.row++) {
            for (frame.column = 0, frame.destX = 0;
                 frame.destX < (frame.pitch >> 1) && frame.column < ((FieldUploadDescriptor *)descriptors)[selector].width;
                 frame.column++, frame.destX += 2) {
                frame.value = *(unsigned short *)&g_FieldPaletteGridBuffer[g_AdPalWords.first * g_FieldPaletteGridStride + (unsigned char)(frame.pixels[frame.row * ((FieldUploadDescriptor *)descriptors)[selector].width + frame.column] & 15)].pixelWord;
                frame.target[((frame.row * frame.pitch) >> 1) + frame.destX] = frame.value;
                if (frame.destX < (frame.pitch >> 1)) {
                    frame.value = *(unsigned short *)&g_FieldPaletteGridBuffer[g_AdPalWords.first * g_FieldPaletteGridStride + (unsigned char)((frame.pixels[frame.row * ((FieldUploadDescriptor *)descriptors)[selector].width + frame.column] & 240) >> 4)].pixelWord;
                    frame.target[((frame.row * frame.pitch) >> 1) + frame.destX + 1] = frame.value;
                }
            }
        }
        break;
    case TIM_PIXEL_MODE_8BPP:
        if (g_FieldPaletteGridBuffer == 0)
            return 0;
        for (frame.row = 0; frame.row < ((FieldUploadDescriptor *)descriptors)[selector].height; frame.row++) {
            for (frame.column = 0, frame.destX = g_FieldSurfaceDestX;
                 frame.column < (frame.pitch >> 1) && frame.destX < ((FieldUploadDescriptor *)descriptors)[selector].width;
                 frame.column++, frame.destX++) {
                frame.value = *(unsigned short *)&g_FieldPaletteGridBuffer[g_AdPalWords.first * g_FieldPaletteGridStride + frame.pixels[frame.row * ((FieldUploadDescriptor *)descriptors)[selector].width + frame.destX]].pixelWord;
                frame.target[((frame.row * frame.pitch) >> 1) + frame.column] = frame.value;
            }
        }
        break;
    case TIM_PIXEL_MODE_16BPP:
        frame.source16 = (unsigned short *)((FieldUploadDescriptor *)descriptors)[selector].pixels;
        for (frame.row = 0; frame.row < ((FieldUploadDescriptor *)descriptors)[selector].height; frame.row++) {
            for (frame.column = 0; frame.column < ((frame.pitch >> 1) < ((FieldUploadDescriptor *)descriptors)[selector].width / 2 ? (frame.pitch >> 1) : ((FieldUploadDescriptor *)descriptors)[selector].width / 2); frame.column++) {
                frame.value = frame.source16[frame.row * ((FieldUploadDescriptor *)descriptors)[selector].width / 2 + frame.column];
                frame.target[((frame.row * frame.pitch) >> 1) + frame.column] = Field_ConvertPixelWordMode(frame.value, 2);
            }
        }
        break;
    }
    Field_UnlockSurface((LPDIRECTDRAWSURFACE)g_pFieldBackSurface);
    Field_PresentEngineFrame();
    return 1;
}

/*
 * Uploads one of two 48-byte image descriptors to the locked 16-bit field surface, rejecting an
 * out-of-range selector, missing source pointer, or failed lock. By the TIM pixel mode, 4bpp expands
 * low/high nibbles through 12-byte palette entries, 8bpp maps bytes through the same palette, and 16bpp
 * converts source SHORT pixels using Field_ConvertPixelWordMode with argument 2. Each row uses the
 * source byte width and destination byte pitch; horizontal writes are pitch limited, but 4bpp
 * tests the first pixel again before writing its partner and can exceed an odd pixel limit by one.
 * 4bpp/8bpp return early without unlocking if the palette is missing. Other modes still
 * unlock, run the finishing helper, and return one without copying.
 */
// FUNCTION: FF7 0x00641f9b
int __cdecl Field_UploadImageToBackSurfaceNoSourceOffset(FieldUploadDescriptor *descriptors, unsigned short selector)
{
    struct {
        unsigned short *source16;
        unsigned int pitch;
        unsigned short destX;
        unsigned short reservedX;
        unsigned short column;
        unsigned short reservedY;
        unsigned short row;
        unsigned short reservedZ;
        void *bits;
        unsigned char *pixels;
        unsigned short *target;
        int mode;
        unsigned short value;
        unsigned short reservedValue;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    frame.bits = 0;
    frame.target = 0;
    frame.source16 = 0;
    frame.pixels = 0;
    if (selector >= 2)
        return 0;
    if (descriptors[selector].pixels == 0)
        return 0;
    frame.mode = g_AdPalWords.second;
    if (!Field_LockSurfaceBits((LPDIRECTDRAWSURFACE)g_pFieldBackSurface, &frame.bits, (int *)&frame.pitch))
        return 0;
    frame.pixels = (unsigned char *)descriptors[selector].pixels;
    frame.target = (unsigned short *)frame.bits;
    switch (frame.mode) {
    case TIM_PIXEL_MODE_4BPP:
        if (g_FieldPaletteGridBuffer == 0)
            return 0;
        for (frame.row = 0; frame.row < descriptors[selector].height; frame.row++) {
            for (frame.column = 0, frame.destX = 0;
                 frame.destX < (frame.pitch >> 1) && frame.column < descriptors[selector].width;
                 frame.column++, frame.destX += 2) {
                frame.value = *(unsigned short *)&g_FieldPaletteGridBuffer[g_AdPalWords.first * g_FieldPaletteGridStride + (unsigned char)(frame.pixels[frame.row * descriptors[selector].width + frame.column] & 15)].pixelWord;
                frame.target[((frame.row * frame.pitch) >> 1) + frame.destX] = frame.value;
                if (frame.destX < (frame.pitch >> 1)) {
                    frame.value = *(unsigned short *)&g_FieldPaletteGridBuffer[g_AdPalWords.first * g_FieldPaletteGridStride + (unsigned char)((frame.pixels[frame.row * descriptors[selector].width + frame.column] & 240) >> 4)].pixelWord;
                    frame.target[((frame.row * frame.pitch) >> 1) + frame.destX + 1] = frame.value;
                }
            }
        }
        break;
    case TIM_PIXEL_MODE_8BPP:
        if (g_FieldPaletteGridBuffer == 0)
            return 0;
        for (frame.row = 0; frame.row < descriptors[selector].height; frame.row++) {
            for (frame.column = 0; frame.column < ((frame.pitch >> 1) < descriptors[selector].width ? (frame.pitch >> 1) : descriptors[selector].width); frame.column++) {
                frame.value = *(unsigned short *)&g_FieldPaletteGridBuffer[g_AdPalWords.first * g_FieldPaletteGridStride + frame.pixels[frame.row * descriptors[selector].width + frame.column]].pixelWord;
                frame.target[((frame.row * frame.pitch) >> 1) + frame.column] = frame.value;
            }
        }
        break;
    case TIM_PIXEL_MODE_16BPP:
        frame.source16 = (unsigned short *)descriptors[selector].pixels;
        for (frame.row = 0; frame.row < descriptors[selector].height; frame.row++) {
            for (frame.column = 0; frame.column < ((frame.pitch >> 1) < descriptors[selector].width / 2 ? (frame.pitch >> 1) : descriptors[selector].width / 2); frame.column++) {
                frame.value = frame.source16[frame.row * descriptors[selector].width / 2 + frame.column];
                frame.target[((frame.row * frame.pitch) >> 1) + frame.column] = Field_ConvertPixelWordMode(frame.value, 2);
            }
        }
        break;
    }
    Field_UnlockSurface((LPDIRECTDRAWSURFACE)g_pFieldBackSurface);
    Field_PresentEngineFrame();
    return 1;
}

/*
 * When hardware flag is set loops Blt colorfill until success; on SURFACELOST restores the primary
 * surface but then returns even if restore succeeded because the new result is not
 * WASSTILLDRAWING. Retries only WASSTILLDRAWING. Otherwise locks back surface, clears pitch*480
 * bytes and unlocks.
 */
// FUNCTION: FF7 0x006424d6
void __cdecl Field_ClearBackSurfaceBltFillOrLock(void)
{
    HRESULT res;
    void *pBits;
    DDBLTFX fx;
    int pitch;
    pBits = 0;
    fx.dwSize = sizeof(fx);
    fx.dwFillColor = 0;
    if (g_FieldTileStateDac != 0) {
        while (1) { /* Constant condition kept for /Od branch/register layout; see lib/codegen.h. */
            res = ((LPDIRECTDRAWSURFACE)g_pFieldBackSurface)->Blt(0, 0, 0, DDBLT_COLORFILL, &fx);
            if (res == 0)
                break;
            if (res == DDERR_SURFACELOST) {
                res = g_pFieldPrimarySurface->Restore();
                if (res != 0)
                    return;
            }
            if (res != DDERR_WASSTILLDRAWING)
                return;
        }
    } else {
        if (!Field_LockSurfaceBits((LPDIRECTDRAWSURFACE)g_pFieldBackSurface, &pBits, &pitch))
            return;
        memset(pBits, 0, pitch * 480);
        Field_UnlockSurface((LPDIRECTDRAWSURFACE)g_pFieldBackSurface);
    }
}

/*
 * Locks g_pFieldBackSurface, zeroes its 0x1E0 rows with the memset intrinsic, and unlocks it.
 * Nothing happens when the lock fails.
 */
// FUNCTION: FF7 0x006425b1
void __cdecl Field_ClearBackSurface(void)
{
    void *bits;
    int pitch;

    bits = 0;
    if (Field_LockSurfaceBits((LPDIRECTDRAWSURFACE)g_pFieldBackSurface, &bits, &pitch) == 0) {
        return;
    }
    memset(bits, 0, pitch * 0x1E0);
    Field_UnlockSurface((LPDIRECTDRAWSURFACE)g_pFieldBackSurface);
}

/*
 * Releases the surface if there is one; the parameter is cleared afterwards
 * and the caller's copy is not, which is what the image does.
 */
// FUNCTION: FF7 0x0064260b
void __cdecl Field_ReleaseSurfaceIfPresent(LPDIRECTDRAWSURFACE surface)
{
    if (surface != NULL) {
        surface->Release();
        surface = NULL;
    }
}

/*
 * Calls destination Blt repeatedly on SURFACELOST without restoring any surface. Returns one on
 * DD_OK OR WASSTILLDRAWING and zero for other errors. Thus successful return alone does not
 * establish completion of the blit.
 */
// FUNCTION: FF7 0x00642629
int __cdecl Field_RetryBltOnSurfaceLost(LPDIRECTDRAWSURFACE dstSurface, LPDIRECTDRAWSURFACE srcSurface, RECT *srcRect, RECT *dstRect, int bltFlags, DDBLTFX *bltFx)
{
    LPDIRECTDRAWSURFACE surface;
    int res;
    while (1) { /* Constant condition kept for /Od branch/register layout; see lib/codegen.h. */
        surface = dstSurface;
        res = surface->Blt(dstRect, srcSurface, srcRect, bltFlags, bltFx);
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
 * Calls destination BltFast repeatedly on SURFACELOST without restore, using the first two point
 * ints as destination coordinates. Returns one on DD_OK OR WASSTILLDRAWING and zero for other
 * errors; successful return does not establish completed copy.
 */
// FUNCTION: FF7 0x006426a0
int __cdecl Field_BltFastAtPoint(LPDIRECTDRAWSURFACE dstSurface, LPDIRECTDRAWSURFACE srcSurface, RECT *srcRect, int *point, int bltFlags)
{
    LPDIRECTDRAWSURFACE surface;
    int res;
    while (1) { /* Constant condition kept for /Od branch/register layout; see lib/codegen.h. */
        surface = dstSurface;
        res = surface->BltFast(point[0], point[1], srcSurface, srcRect, bltFlags);
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
 * Sets a single-colour colour key on the surface: both ends of the range are
 * the one colour given.
 */
// FUNCTION: FF7 0x0064271c
void __cdecl Field_SetSurfaceSingleColorKey(LPDIRECTDRAWSURFACE surface, int colour, int flags)
{
    DDCOLORKEY key = {colour, colour};

    surface->SetColorKey(flags, &key);
}

/*
 * Creates a plain off-screen surface in system memory of the given size on
 * g_pFieldDirectDraw, gives it a black source colour key, and returns it; 0 when
 * CreateSurface fails.
 */
// FUNCTION: FF7 0x00642746
LPDIRECTDRAWSURFACE __cdecl Field_CreateSystemOffscreenSurface(int width, int height)
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
    result = g_pFieldDirectDraw->CreateSurface(&ddsd, &surface, NULL);
    if (result != 0) {
        return 0;
    }
    Field_SetSurfaceSingleColorKey(surface, 0, DDCKEY_SRCBLT);
    return surface;
}

/*
 * Allocates a 16-bit surface of width by height texels from the game heap, tagged with this file and
 * line. The null test that returns the pointer either way is the original's.
 */
// FUNCTION: FF7 0x006427b6
int __cdecl Field_AllocSurfaceBuffer16(int width, int height)
{
    int buffer;

    // STRING: FF7 0x00907df0
    buffer = (int)(GameHeapAllocRaw(width * height << 1, "C:\\FF7\\field\\src\\ad_ddraw.cpp", 0x398));
    if (buffer == 0) {
        return 0;
    } else {
        return buffer;
    }
}

/*
 * Clears 1280 bytes per row in the first and last 16 of 480 locked surface rows.
 */
// FUNCTION: FF7 0x006427ea
void __cdecl Field_ClearBackSurfaceTopAndBottomBands(void)
{
    int cursor;
    int baseBits;
    unsigned short rowIndex;
    int pitch;
    cursor = 0;
    if (Field_LockSurfaceBits((LPDIRECTDRAWSURFACE)g_pFieldBackSurface, (void **)(&cursor), &pitch) == 0)
        return;
    baseBits = cursor;
    for (rowIndex = 0; rowIndex < 16; rowIndex++) {
        memset((void *)cursor, 0, 0x500);
        cursor += pitch;
    }
    cursor = baseBits + pitch * 0x1d0;
    for (rowIndex = 0x1d0; rowIndex < 0x1e0; rowIndex++) {
        memset((void *)cursor, 0, 0x500);
        cursor += pitch;
    }
    Field_UnlockSurface((LPDIRECTDRAWSURFACE)g_pFieldBackSurface);
}

/*
 * Copies the 256 rows of 0x200 bytes of texture page `page` (the pixel buffer of g_FieldLayers[page]) into
 * the locked surface g_pFieldPrimarySurface, one memcpy per row stepped by the surface pitch, then unlocks
 * it. Nothing happens when the lock fails.
 */
// FUNCTION: FF7 0x006428b7
void __cdecl Field_CopyTexturePageToPrimarySurface(int page)
{
    int dest;
    int source;
    int surfaceFlags;
    int rowIndex;
    int pitch[2];

    surfaceFlags = 0;
    if (Field_LockSurfaceBits(g_pFieldPrimarySurface, (void **)(&surfaceFlags), pitch) == 0) {
        return;
    }
    dest = surfaceFlags;
    source = (int)g_FieldLayers[page]->pixels;
    for (rowIndex = 0; rowIndex < 0x100; rowIndex++) {
        memcpy((void *)dest, (void *)source, 0x200);
        dest += pitch[0];
        source += 0x200;
    }
    Field_UnlockSurface(g_pFieldPrimarySurface);
}
} /* extern "C" */
