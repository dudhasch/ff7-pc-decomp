// Contents: DirectDraw and Direct3D setup for movie playback: creates the DirectDraw and Direct3D
//   objects, the fullscreen flip surfaces and viewport, logs surface descriptions, and restores or
//   releases them.
// Original: unknown (no retail __FILE__ path names this TU)
/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/com_abi.h"
#include <windows.h>
#include <ddraw.h>
#include <d3d.h>
#include <string.h>
#include "lib/api/movie.h"
#include "lib/enums/graphics.h"
/* Retail DirectDraw movie setup logs diagnostics via Trace_Printf. */
#include "ff7/api/main.h"

#pragma intrinsic(memset)

extern "C" {
// GLOBAL: FF7 0x007b8150
extern const GUID IID_IMultiMediaStream;
// GLOBAL: FF7 0x007b8250
extern const GUID IID_IDirect3DRGBDevice;
// GLOBAL: FF7 0x007b8260
extern const GUID IID_IDirect3DHALDevice;
// GLOBAL: FF7 0x007b8270
extern const GUID IID_IDirect3D2;
// The DirectDraw GUIDs are strmiids.lib members.
extern const GUID IID_IDirectDraw2;
// GLOBAL: FF7 0x007b8080
extern const GUID IID_IDirectDraw;
// GLOBAL: FF7 0x007b8090
extern const GUID CLSID_DirectDraw;

// GLOBAL: FF7 0x007c05b8
GUID g_Movie_guid3Dfx = {0x3A0CFD01, 0x9320, 0x11CF, {0xAC, 0xA1, 0x00, 0xA0, 0x24, 0x13, 0xC2, 0xE2}};
// GLOBAL: FF7 0x007c05c8
int g_Movie_displayWidth = 640;
// GLOBAL: FF7 0x007c05cc
int g_Movie_displayHeight = 480;
// GLOBAL: FF7 0x007c05d0
int g_Movie_displayBpp = 16;

// GLOBAL: FF7 0x009a1020
DDCAPS g_Movie_ddDriverCaps = {0};
unsigned char g_bssPad_009a118c[4] = {0};
// GLOBAL: FF7 0x009a1190
DDSURFACEDESC g_Movie_ddSurfaceDesc = {0};
// GLOBAL: FF7 0x009a11fc
DDSCAPS g_Movie_ddCaps = {0};
// GLOBAL: FF7 0x009a1200
int g_Movie_using3Dfx = 0;
unsigned char g_bssPad_009a1204[4] = {0};
// GLOBAL: FF7 0x009a1208
DDCAPS g_Movie_ddHelCaps = {0};

// GLOBAL: FF7 0x009a1374
LPDIRECTDRAW g_Movie_pDirectDraw = 0;
// GLOBAL: FF7 0x009a1378
LPDIRECTDRAW g_Movie_pDirectDraw2 = 0;
// GLOBAL: FF7 0x009a137c
LPDIRECTDRAWSURFACE g_Movie_pPrimarySurface = 0;
// GLOBAL: FF7 0x009a1380
LPDIRECTDRAWSURFACE g_Movie_pBackBuffer = 0;
// GLOBAL: FF7 0x009a1384
LPDIRECT3D2 g_Movie_pDirect3D2 = 0;
// GLOBAL: FF7 0x009a1388
LPDIRECT3DDEVICE2 g_Movie_pDirect3DDevice2 = 0;
// GLOBAL: FF7 0x009a138c
LPDIRECT3DVIEWPORT2 g_Movie_pDirect3DViewport2 = 0;
unsigned char g_bssPad_009a1390[12] = {0};
// GLOBAL: FF7 0x009a139c
HWND g_Movie_hWnd = 0;
unsigned char g_bssPad_009a13a0[8] = {0};

void __cdecl Movie_LogSurfaceDescription(const char *surfaceName, IDirectDrawSurface *pSurface);
int __cdecl Movie_ClearSurface(IDirectDrawSurface *pSurface);

/*
 * Queries a DirectDraw surface description into a shared descriptor and logs its
 * attributes (memory location, dimensions, pitch, back-buffer count, RGB masks, caps).
 */
// FUNCTION: FF7 0x004186e0
void __cdecl Movie_LogSurfaceDescription(const char *surfaceName, IDirectDrawSurface *pSurface)
{
    HRESULT hr = 0;
    memset(&g_Movie_ddSurfaceDesc, 0, sizeof(g_Movie_ddSurfaceDesc));
    g_Movie_ddSurfaceDesc.dwSize = sizeof(g_Movie_ddSurfaceDesc);
    hr = pSurface->GetSurfaceDesc(&g_Movie_ddSurfaceDesc);
    if (hr < 0) {
        // STRING: FF7 0x007c05d4
        Trace_Printf("Failed to get surface description\n");
    }
    // STRING: FF7 0x007c05f8
    Trace_Printf("Surface (%s: 0x%8.8X) format: \n", surfaceName, pSurface);
    if ((g_Movie_ddSurfaceDesc.ddsCaps.dwCaps & DDSCAPS_SYSTEMMEMORY) != 0) {
        // STRING: FF7 0x007c0618
        Trace_Printf("    Surface in system memory\n");
    } else if ((g_Movie_ddSurfaceDesc.ddsCaps.dwCaps & DDSCAPS_VIDEOMEMORY) != 0) {
        // STRING: FF7 0x007c0638
        Trace_Printf("    Surface in video memory\n");
    }
    // STRING: FF7 0x007c0658
    Trace_Printf("    dwFlags: %u\n", g_Movie_ddSurfaceDesc.dwFlags);
    if ((g_Movie_ddSurfaceDesc.dwFlags & DDSD_WIDTH) != 0) {
        // STRING: FF7 0x007c066c
        Trace_Printf("    dwWidth: %u\n", g_Movie_ddSurfaceDesc.dwWidth);
    }
    if ((g_Movie_ddSurfaceDesc.dwFlags & DDSD_HEIGHT) != 0) {
        // STRING: FF7 0x007c0680
        Trace_Printf("    dwHeight: %u\n", g_Movie_ddSurfaceDesc.dwHeight);
    }
    if ((g_Movie_ddSurfaceDesc.dwFlags & DDSD_PITCH) != 0) {
        // STRING: FF7 0x007c0694
        Trace_Printf("    lPitch: %u\n", g_Movie_ddSurfaceDesc.lPitch);
    }
    if ((g_Movie_ddSurfaceDesc.dwFlags & DDSD_BACKBUFFERCOUNT) != 0) {
        // STRING: FF7 0x007c06a4
        Trace_Printf("    dwBackBufferCount: %u\n", g_Movie_ddSurfaceDesc.dwBackBufferCount);
    }
    if ((g_Movie_ddSurfaceDesc.dwFlags & DDSD_PIXELFORMAT) != 0) {
        if ((g_Movie_ddSurfaceDesc.ddpfPixelFormat.dwFlags & DDPF_RGB) != 0) {
            // STRING: FF7 0x007c06c0
            Trace_Printf("    dwRGBBitCount: %u\n", g_Movie_ddSurfaceDesc.ddpfPixelFormat.dwRGBBitCount);
            // STRING: FF7 0x007c06d8
            Trace_Printf("    dwRBitMask: %u\n", g_Movie_ddSurfaceDesc.ddpfPixelFormat.dwRBitMask);
            // STRING: FF7 0x007c06ec
            Trace_Printf("    dwGBitMask: %u\n", g_Movie_ddSurfaceDesc.ddpfPixelFormat.dwGBitMask);
            // STRING: FF7 0x007c0700
            Trace_Printf("    dwBBitMask: %u\n", g_Movie_ddSurfaceDesc.ddpfPixelFormat.dwBBitMask);
        }
    }
    // STRING: FF7 0x007c0714
    Trace_Printf("    dwCaps: %u\n", g_Movie_ddSurfaceDesc.ddsCaps.dwCaps);
}

/*
 * Creates and initializes the DirectDraw object, querying IDirectDraw interface.
 * Tries 3Dfx driver GUID first, falling back to NULL GUID.
 */
// FUNCTION: FF7 0x00418893
int __cdecl Movie_CreateDirectDrawObject(void)
{
    HRESULT hr = 0;
    hr = CoCreateInstance(CLSID_DirectDraw, 0, 7, IID_IDirectDraw2, (void **)&g_Movie_pDirectDraw2);
    if (hr < 0) {
        // STRING: FF7 0x007c0724
        Trace_Printf("Failed to create DirectDraw object (0x%8.8X)\n", hr);
        return 0;
    }
    if (g_Movie_pDirectDraw2->Initialize(&g_Movie_guid3Dfx) < 0) {
        hr = g_Movie_pDirectDraw2->Initialize(0);
    } else {
        g_Movie_using3Dfx = 1;
        // STRING: FF7 0x007c0754
        Trace_Printf("Using 3Dfx...\n");
    }
    if (hr < 0) {
        // STRING: FF7 0x007c0764
        Trace_Printf("Failed to initialize DirectDraw object (0x%8.8X)\n", hr);
        if (g_Movie_pDirectDraw2 != 0) {
            g_Movie_pDirectDraw2->Release();
            g_Movie_pDirectDraw2 = 0;
        }
        return 0;
    }
    hr = g_Movie_pDirectDraw2->QueryInterface(IID_IDirectDraw, (void **)&g_Movie_pDirectDraw);
    if (hr < 0) {
        // STRING: FF7 0x007c0798
        Trace_Printf("Failed to get IDirectDraw (0x%8.8X)\n", hr);
        if (g_Movie_pDirectDraw2 != 0) {
            g_Movie_pDirectDraw2->Release();
            g_Movie_pDirectDraw2 = 0;
        }
        return 0;
    }
    return 1;
}

/*
 * Sets fullscreen exclusive mode and display resolution, creates the flipping
 * primary surface and back buffer, clears both surfaces, and queries caps.
 */
// FUNCTION: FF7 0x004189c7
int __cdecl Movie_CreateFullscreenFlipSurfaces(void)
{
    DDSURFACEDESC desc;
    HRESULT hr = 0;
    hr = ((IDirectDraw2 *)g_Movie_pDirectDraw2)->SetCooperativeLevel(g_Movie_hWnd, DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN);
    if (hr < 0) {
        // STRING: FF7 0x007c07c0
        Trace_Printf("Failed to set cooperative level for fullscreen (0x%8.8X)\n", hr);
        return 0;
    }
    hr = ((IDirectDraw2 *)g_Movie_pDirectDraw2)->SetDisplayMode(g_Movie_displayWidth, g_Movie_displayHeight, g_Movie_displayBpp, 0, 0);
    if (hr < 0) {
        ((IDirectDraw2 *)g_Movie_pDirectDraw2)->SetCooperativeLevel(g_Movie_hWnd, DDSCL_NORMAL);
        // STRING: FF7 0x007c07fc
        Trace_Printf("Failed to set display mode (0x%8.8X)\n", hr);
        return 0;
    }
    // STRING: FF7 0x007c0824
    Trace_Printf("Set display mode to %ux%ux%u\n", g_Movie_displayWidth, g_Movie_displayHeight, g_Movie_displayBpp);
    memset(&desc, 0, sizeof(desc));
    desc.dwSize = sizeof(desc);
    desc.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
    desc.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP | DDSCAPS_COMPLEX | DDSCAPS_VIDEOMEMORY;
    desc.ddsCaps.dwCaps |= DDSCAPS_3DDEVICE;
    desc.dwBackBufferCount = 1;
    hr = ((IDirectDraw2 *)g_Movie_pDirectDraw2)->CreateSurface(&desc, &g_Movie_pPrimarySurface, 0);
    if (hr < 0) {
        ((IDirectDraw2 *)g_Movie_pDirectDraw2)->SetCooperativeLevel(g_Movie_hWnd, DDSCL_NORMAL);
        // STRING: FF7 0x007c0844
        Trace_Printf("Failed to create primary surface (0x%8.8X)\n", hr);
        return 0;
    }
    // STRING: FF7 0x007c0870
    Movie_LogSurfaceDescription("Primary", g_Movie_pPrimarySurface);
    g_Movie_ddCaps.dwCaps = DDSCAPS_BACKBUFFER;
    hr = g_Movie_pPrimarySurface->GetAttachedSurface(&g_Movie_ddCaps, &g_Movie_pBackBuffer);
    if (hr < 0) {
        ((IDirectDraw2 *)g_Movie_pDirectDraw2)->SetCooperativeLevel(g_Movie_hWnd, DDSCL_NORMAL);
        // STRING: FF7 0x007c0878
        Trace_Printf("Failed to get back buffer (0x%8.8X)\n", hr);
        return 0;
    }
    // STRING: FF7 0x007c08a0
    Movie_LogSurfaceDescription("Back", g_Movie_pBackBuffer);
    Movie_ClearSurface(g_Movie_pPrimarySurface);
    Movie_ClearSurface(g_Movie_pBackBuffer);
    memset(&g_Movie_ddDriverCaps, 0, sizeof(g_Movie_ddDriverCaps));
    g_Movie_ddDriverCaps.dwSize = sizeof(g_Movie_ddDriverCaps);
    g_Movie_ddHelCaps = g_Movie_ddDriverCaps;
    hr = ((IDirectDraw2 *)g_Movie_pDirectDraw2)->GetCaps(&g_Movie_ddDriverCaps, &g_Movie_ddHelCaps);
    if (hr < 0) {
        // STRING: FF7 0x007c08a8
        Trace_Printf("Failed to get DirectDraw caps (0x%8.8X)\n", hr);
        Movie_ReleaseDirectDrawObjects();
        return 0;
    }
    return 1;
}

/*
 * Restores normal cooperative level on DirectDraw and releases the primary surface.
 */
// FUNCTION: FF7 0x00418c34
void __cdecl Movie_ReleasePrimarySurface(void)
{
    if (g_Movie_pDirectDraw2 != 0) {
        g_Movie_pDirectDraw2->SetCooperativeLevel(g_Movie_hWnd, DDSCL_NORMAL);
        if (g_Movie_pPrimarySurface != 0) {
            g_Movie_pPrimarySurface->Release();
            g_Movie_pPrimarySurface = 0;
        }
    }
}

/*
 * Releases and nulls IDirectDraw and IDirectDraw2 interfaces.
 */
// FUNCTION: FF7 0x00418c7f
void __cdecl Movie_ReleaseDirectDrawObjects(void)
{
    if (g_Movie_pDirectDraw != 0) {
        g_Movie_pDirectDraw->Release();
        g_Movie_pDirectDraw = 0;
    }
    if (g_Movie_pDirectDraw2 != 0) {
        g_Movie_pDirectDraw2->Release();
        g_Movie_pDirectDraw2 = 0;
    }
}

/*
 * Restores the primary surface if it has been lost.
 */
// FUNCTION: FF7 0x00418ccc
int __cdecl Movie_RestorePrimarySurface(void)
{
    HRESULT hr = 0;
    if (g_Movie_pPrimarySurface != 0) {
        if (g_Movie_pPrimarySurface->IsLost() == DDERR_SURFACELOST) {
            hr = g_Movie_pPrimarySurface->Restore();
            if (hr < 0) {
                // STRING: FF7 0x007c08d4
                Trace_Printf("Failed to restore primary surface (0x%8.8X)\n", hr);
                return 0;
            }
        }
    }
    return 1;
}

/*
 * Flips the primary surface to the attached back buffer.
 */
// FUNCTION: FF7 0x00418d30
int __cdecl Movie_FlipPrimarySurface(void)
{
    HRESULT hr = 0;
    hr = g_Movie_pPrimarySurface->Flip(0, 1);
    if (hr < 0) {
        // STRING: FF7 0x007c0904
        Trace_Printf("Failed to flip screen buffers (0x%8.8X)\n", hr);
        return 0;
    }
    return 1;
}

/*
 * Clears the specified DirectDraw surface to black via Blt.
 */
// FUNCTION: FF7 0x00418d77
int __cdecl Movie_ClearSurface(IDirectDrawSurface *pSurface)
{
    DDBLTFX bltFx;
    HRESULT hr = 0;
    bltFx.dwSize = sizeof(bltFx);
    bltFx.dwFillColor = 0;
    hr = pSurface->Blt(0, 0, 0, 0x1000400, &bltFx);
    if (hr < 0) {
        // STRING: FF7 0x007c0930
        Trace_Printf("Failed to clear screen (0x%8.8X)\n", hr);
        return 0;
    }
    return 1;
}

/*
 * Creates Direct3D2 device, viewport, adds and activates the viewport.
 */
// FUNCTION: FF7 0x00418dd4
int __cdecl Movie_CreateDirect3DObjects(void)
{
    HRESULT hr = 0;
    hr = IDirectDraw_QueryInterface(g_Movie_pDirectDraw2, IID_IDirect3D2, (void **)&g_Movie_pDirect3D2);
    if (hr < 0) {
        // STRING: FF7 0x007c0954
        Trace_Printf("Failed to get IDirect3D (0x%8.8X)\n", hr);
        return 0;
    }
    hr = IDirect3D2_CreateDevice(g_Movie_pDirect3D2, IID_IDirect3DHALDevice, g_Movie_pBackBuffer, &g_Movie_pDirect3DDevice2);
    if (hr < 0) {
        hr = IDirect3D2_CreateDevice(g_Movie_pDirect3D2, IID_IDirect3DRGBDevice, g_Movie_pBackBuffer, &g_Movie_pDirect3DDevice2);
        if (hr < 0) {
            if (g_Movie_pDirect3D2 != 0) {
                IDirect3D2_Release(g_Movie_pDirect3D2);
                g_Movie_pDirect3D2 = 0;
            }
            // STRING: FF7 0x007c0978
            Trace_Printf("Failed to create Direct3D device (0x%8.8X)\n", hr);
            return 0;
        }
    }
    hr = IDirect3D2_CreateViewport(g_Movie_pDirect3D2, &g_Movie_pDirect3DViewport2, 0);
    if (hr < 0) {
        if (g_Movie_pDirect3DDevice2 != 0) {
            IDirect3DDevice2_Release(g_Movie_pDirect3DDevice2);
            g_Movie_pDirect3DDevice2 = 0;
        }
        if (g_Movie_pDirect3D2 != 0) {
            IDirect3D2_Release(g_Movie_pDirect3D2);
            g_Movie_pDirect3D2 = 0;
        }
        // STRING: FF7 0x007c09a4
        Trace_Printf("Failed to create Direct3D viewport (0x%8.8X)\n", hr);
        return 0;
    }
    hr = IDirect3DDevice2_AddViewport(g_Movie_pDirect3DDevice2, g_Movie_pDirect3DViewport2);
    if (hr < 0) {
        if (g_Movie_pDirect3DViewport2 != 0) {
            IDirect3DViewport2_Release(g_Movie_pDirect3DViewport2);
            g_Movie_pDirect3DViewport2 = 0;
        }
        if (g_Movie_pDirect3DDevice2 != 0) {
            IDirect3DDevice2_Release(g_Movie_pDirect3DDevice2);
            g_Movie_pDirect3DDevice2 = 0;
        }
        if (g_Movie_pDirect3D2 != 0) {
            IDirect3D2_Release(g_Movie_pDirect3D2);
            g_Movie_pDirect3D2 = 0;
        }
        // STRING: FF7 0x007c09d4
        Trace_Printf("Failed to add Direct3D viewport to device (0x%8.8X)\n", hr);
        return 0;
    }
    {
        D3DVIEWPORT2 viewport;
        memset(&viewport, 0, sizeof(viewport));
        // Both stores occur in the original after the complete 44-byte zero fill.
        viewport.dwSize = sizeof(D3DVIEWPORT2);
        viewport.dwSize = sizeof(D3DVIEWPORT2);
        viewport.dwX = 0;
        viewport.dwY = 0;
        viewport.dwWidth = g_Movie_displayWidth;
        viewport.dwHeight = g_Movie_displayHeight;
        viewport.dvClipX = 0.0f;
        viewport.dvClipY = 0.0f;
        viewport.dvClipWidth = (float)g_Movie_displayWidth;
        viewport.dvClipHeight = (float)g_Movie_displayHeight;
        viewport.dvMinZ = 0.0f;
        viewport.dvMaxZ = 1.0f;
        // The original ignores this HRESULT and checks the preceding AddViewport result.
        IDirect3DViewport2_SetViewport2(g_Movie_pDirect3DViewport2, &viewport);
        if (hr < 0) {
            if (g_Movie_pDirect3DViewport2 != 0) {
                IDirect3DViewport2_Release(g_Movie_pDirect3DViewport2);
                g_Movie_pDirect3DViewport2 = 0;
            }
            if (g_Movie_pDirect3DDevice2 != 0) {
                IDirect3DDevice2_Release(g_Movie_pDirect3DDevice2);
                g_Movie_pDirect3DDevice2 = 0;
            }
            if (g_Movie_pDirect3D2 != 0) {
                IDirect3D2_Release(g_Movie_pDirect3D2);
                g_Movie_pDirect3D2 = 0;
            }
            // STRING: FF7 0x007c0a0c
            Trace_Printf("Failed to set viewport data (0x%8.8X)\n", hr);
            return 0;
        }
    }
    hr = IDirect3DDevice2_SetCurrentViewport(g_Movie_pDirect3DDevice2, g_Movie_pDirect3DViewport2);
    if (hr < 0) {
        if (g_Movie_pDirect3DViewport2 != 0) {
            IDirect3DViewport2_Release(g_Movie_pDirect3DViewport2);
            g_Movie_pDirect3DViewport2 = 0;
        }
        if (g_Movie_pDirect3DDevice2 != 0) {
            IDirect3DDevice2_Release(g_Movie_pDirect3DDevice2);
            g_Movie_pDirect3DDevice2 = 0;
        }
        if (g_Movie_pDirect3D2 != 0) {
            IDirect3D2_Release(g_Movie_pDirect3D2);
            g_Movie_pDirect3D2 = 0;
        }
        // STRING: FF7 0x007c0a34
        Trace_Printf("Failed to set device current viewport (0x%8.8X)\n", hr);
        return 0;
    }
    return 1;
}

/*
 * Releases and nulls Direct3D viewport, device, and Direct3D2 interfaces.
 */
// FUNCTION: FF7 0x00419195
void __cdecl Movie_ReleaseDirect3DObjects(void)
{
    if (g_Movie_pDirect3DViewport2 != 0) {
        g_Movie_pDirect3DViewport2->Release();
        g_Movie_pDirect3DViewport2 = 0;
    }
    if (g_Movie_pDirect3DDevice2 != 0) {
        g_Movie_pDirect3DDevice2->Release();
        g_Movie_pDirect3DDevice2 = 0;
    }
    if (g_Movie_pDirect3D2 != 0) {
        g_Movie_pDirect3D2->Release();
        g_Movie_pDirect3D2 = 0;
    }
}
} /* extern "C" */
