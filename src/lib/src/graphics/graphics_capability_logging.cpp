// src/lib/src/graphics/graphics_capability_logging.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Logs DirectDraw and Direct3D capabilities (surface, driver, comparison, primitive and
//   device descriptions) to the debug target. It also holds a flagged record initializer and a
//   capability flag test.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/types/graphics.h"
#include "lib/enums/graphics.h"
#include "lib/codegen.h"
#include "lib/api/graphics.h"
#include <stdio.h>
#include "lib/api/mem.h"
extern "C" {

struct GfxDirectDrawDriverCaps;
#pragma optimize("p", on)
// The optimize("p") pairs in this file set /Op per function, as the retail build did (lib/codegen.h, "/Op pairs").
#pragma optimize("p", off)

/*
 * Initializes an 8-byte record and sets its initial flag.
 */
// FUNCTION: FF7 0x0067ded0
void __cdecl Gfx_InitFlaggedRecord(GfxFlaggedRecord *record)
{
    ZeroFillMemory32(8, record);
    record->flags = 0;
    record->clearedWord = 0;
    record->flags |= 0x8000;
}

/*
 * Tests a capability flag bit and formats diagnostic logging output.
 */
// FUNCTION: FF7 0x0067df03
int __cdecl ReportFlagAndTest(int flags, int mask, const char *label)
{
    int res;
    char logBuffer[200];

    res = 0;
    if ((flags & mask) != 0) {
        // STRING: FF7 0x0090e06c
        sprintf(logBuffer, "  %s \n", label);
        Gfx_DebugPrint(logBuffer);
        res = 1;
    } else {
        // STRING: FF7 0x0090e074
        sprintf(logBuffer, "  X  %s \n", label);
        Gfx_DebugPrint(logBuffer);
    }
    return res;
}

/*
 * Logs DirectDraw surface capability flags to the active debug target.
 */
// FUNCTION: FF7 0x0067df84
void __cdecl Gfx_LogDirectDrawSurfaceCaps(int *surfaceDesc)
{
    // STRING: FF7 0x0090e080
    ReportFlagAndTest(*surfaceDesc, DDSCAPS_3DDEVICE, "3DDEVICE");
    // STRING: FF7 0x0090e08c
    ReportFlagAndTest(*surfaceDesc, DDSCAPS_ALLOCONLOAD, "ALLOCONLOAD");
    // STRING: FF7 0x0090e098
    ReportFlagAndTest(*surfaceDesc, DDSCAPS_ALPHA, "ALPHA");
    // STRING: FF7 0x0090e0a0
    ReportFlagAndTest(*surfaceDesc, DDSCAPS_BACKBUFFER, "BACKBUFFER");
    // STRING: FF7 0x0090e0ac
    ReportFlagAndTest(*surfaceDesc, DDSCAPS_COMPLEX, "COMPLEX");
    // STRING: FF7 0x0090e0b4
    ReportFlagAndTest(*surfaceDesc, DDSCAPS_FLIP, "FLIP");
    // STRING: FF7 0x0090e0bc
    ReportFlagAndTest(*surfaceDesc, DDSCAPS_FRONTBUFFER, "FRONTBUFFER");
    // STRING: FF7 0x0090e0c8
    ReportFlagAndTest(*surfaceDesc, DDSCAPS_OFFSCREENPLAIN, "OFFSCREENPLAIN");
    // STRING: FF7 0x0090e0d8
    ReportFlagAndTest(*surfaceDesc, DDSCAPS_OWNDC, "OWNDC");
    // STRING: FF7 0x0090e0e0
    ReportFlagAndTest(*surfaceDesc, DDSCAPS_PALETTE, "PALETTE");
    // STRING: FF7 0x0090e0e8
    ReportFlagAndTest(*surfaceDesc, DDSCAPS_PRIMARYSURFACE, "PRIMARYSURFACE");
    // STRING: FF7 0x0090e0f8
    ReportFlagAndTest(*surfaceDesc, DDSCAPS_SYSTEMMEMORY, "SYSTEMMEMORY");
    // STRING: FF7 0x0090e108
    ReportFlagAndTest(*surfaceDesc, DDSCAPS_TEXTURE, "TEXTURE");
    // STRING: FF7 0x0090e110
    ReportFlagAndTest(*surfaceDesc, DDSCAPS_VIDEOMEMORY, "VIDEOMEMORY");
    // STRING: FF7 0x0090e11c
    ReportFlagAndTest(*surfaceDesc, DDSCAPS_VISIBLE, "VISIBLE");
    // STRING: FF7 0x0090e124
    ReportFlagAndTest(*surfaceDesc, DDSCAPS_WRITEONLY, "WRITEONLY");
    // STRING: FF7 0x0090e130
    ReportFlagAndTest(*surfaceDesc, DDSCAPS_ZBUFFER, "ZBUFFER");
}

/*
 * Logs DirectDraw driver and palette capabilities and video memory statistics.
 */
// FUNCTION: FF7 0x0067e10f
void __cdecl Gfx_LogDirectDrawDriverCaps(GfxDirectDrawDriverCaps *driverCaps)
{
    char buffer[0xC8];

    // STRING: FF7 0x0090e138
    Gfx_DebugPrint("DRIVER CAPS\n");
    // STRING: FF7 0x0090e148
    ReportFlagAndTest(driverCaps->caps, 1, "DDCAPS_3D");
    // STRING: FF7 0x0090e154
    ReportFlagAndTest(driverCaps->caps, 2, "DDCAPS_ALIGNBOUNDARYDEST");
    // STRING: FF7 0x0090e170
    ReportFlagAndTest(driverCaps->caps, 8, "DDCAPS_ALIGNBOUNDARYSRC");
    // STRING: FF7 0x0090e188
    ReportFlagAndTest(driverCaps->caps, 4, "DDCAPS_ALIGNSIZEDEST");
    // STRING: FF7 0x0090e1a0
    ReportFlagAndTest(driverCaps->caps, 0x10, "DDCAPS_ALIGNSIZESRC");
    // STRING: FF7 0x0090e1b4
    ReportFlagAndTest(driverCaps->caps, 0x20, "DDCAPS_ALIGNSTRIDE");
    // STRING: FF7 0x0090e1c8
    ReportFlagAndTest(driverCaps->caps, 0x800000, "DDCAPS_ALPHA");
    // STRING: FF7 0x0090e1d8
    ReportFlagAndTest(driverCaps->caps, 0x8000000, "DDCAPS_BANKSWITCHED");
    // STRING: FF7 0x0090e1ec
    ReportFlagAndTest(driverCaps->caps, 0x40, "DDCAPS_BLT");
    // STRING: FF7 0x0090e1f8
    ReportFlagAndTest(driverCaps->caps, 0x4000000, "DDCAPS_BLTCOLORFILL");
    // STRING: FF7 0x0090e20c
    ReportFlagAndTest(driverCaps->caps, 0x10000000, "DDCAPS_BLTDEPTHFILL");
    // STRING: FF7 0x0090e220
    ReportFlagAndTest(driverCaps->caps, 0x100, "DDCAPS_BLTFOURCC");
    // STRING: FF7 0x0090e234
    ReportFlagAndTest(driverCaps->caps, 0x80, "DDCAPS_BLTQUEUE");
    // STRING: FF7 0x0090e244
    ReportFlagAndTest(driverCaps->caps, 0x200, "DDCAPS_BLTSTRETCH");
    // STRING: FF7 0x0090e258
    ReportFlagAndTest(driverCaps->caps, 0x80000000, "DDCAPS_CANBLTSYSMEM");
    // STRING: FF7 0x0090e26c
    ReportFlagAndTest(driverCaps->caps, 0x20000000, "DDCAPS_CANCLIP");
    // STRING: FF7 0x0090e27c
    ReportFlagAndTest(driverCaps->caps, 0x40000000, "DDCAPS_CANCLIPSTRETCHED");
    // STRING: FF7 0x0090e294
    ReportFlagAndTest(driverCaps->caps, 0x400000, "DDCAPS_COLORKEY");
    // STRING: FF7 0x0090e2a4
    ReportFlagAndTest(driverCaps->caps, 0x1000000, "DDCAPS_COLORKEYHWASSIST");
    // STRING: FF7 0x0090e2bc
    ReportFlagAndTest(driverCaps->caps, 0x400, "DDCAPS_GDI");
    // STRING: FF7 0x0090e2c8
    ReportFlagAndTest(driverCaps->caps, 0x2000000, "DDCAPS_NOHARDWARE");
    // STRING: FF7 0x0090e2dc
    ReportFlagAndTest(driverCaps->caps, 0x800, "DDCAPS_OVERLAY");
    // STRING: FF7 0x0090e2ec
    ReportFlagAndTest(driverCaps->caps, 0x1000, "DDCAPS_OVERLAYCANTCLIP");
    // STRING: FF7 0x0090e304
    ReportFlagAndTest(driverCaps->caps, 0x2000, "DDCAPS_OVERLAYFOURCC");
    // STRING: FF7 0x0090e31c
    ReportFlagAndTest(driverCaps->caps, 0x4000, "DDCAPS_OVERLAYSTRETCH");
    // STRING: FF7 0x0090e334
    ReportFlagAndTest(driverCaps->caps, 0x8000, "DDCAPS_PALETTE");
    // STRING: FF7 0x0090e344
    ReportFlagAndTest(driverCaps->caps, 0x10000, "DDCAPS_PALETTEVSYNC");
    // STRING: FF7 0x0090e358
    ReportFlagAndTest(driverCaps->caps, 0x20000, "DDCAPS_READSCANLINE");
    // STRING: FF7 0x0090e36c
    ReportFlagAndTest(driverCaps->caps, 0x40000, "DDCAPS_STEREOVIEW");
    // STRING: FF7 0x0090e380
    ReportFlagAndTest(driverCaps->caps, 0x80000, "DDCAPS_VBI");
    // STRING: FF7 0x0090e38c
    ReportFlagAndTest(driverCaps->caps, 0x100000, "DDCAPS_ZBLTS");
    // STRING: FF7 0x0090e39c
    ReportFlagAndTest(driverCaps->caps, 0x200000, "DDCAPS_ZOVERLAYS");
    // STRING: FF7 0x0090e3b0
    Gfx_DebugPrint("DRIVER CAPS 2\n");
    // STRING: FF7 0x0090e3c0
    ReportFlagAndTest(driverCaps->caps2, 8, "DDCAPS2_AUTOFLIPOVERLAY");
    // STRING: FF7 0x0090e3d8
    ReportFlagAndTest(driverCaps->caps2, 0x10, "DDCAPS2_CANBOBINTERLEAVED");
    // STRING: FF7 0x0090e3f4
    ReportFlagAndTest(driverCaps->caps2, 0x20, "DDCAPS2_CANBOBNONINTERLEAVED");
    // STRING: FF7 0x0090e414
    ReportFlagAndTest(driverCaps->caps2, 0x100, "DDCAPS2_CANDROPZ16BIT");
    // STRING: FF7 0x0090e42c
    ReportFlagAndTest(driverCaps->caps2, 0x2000, "DDCAPS2_CANFLIPODDEVEN");
    // STRING: FF7 0x0090e444
    ReportFlagAndTest(driverCaps->caps2, 1, "DDCAPS2_CERTIFIED");
    // STRING: FF7 0x0090e458
    ReportFlagAndTest(driverCaps->caps2, 0x80, "DDCAPS2_COLORCONTROLPRIMARY");
    // STRING: FF7 0x0090e474
    ReportFlagAndTest(driverCaps->caps2, 0x40, "DDCAPS2_COLORCONTROLOVERLAY");
    // STRING: FF7 0x0090e490
    ReportFlagAndTest(driverCaps->caps2, 2, "DDCAPS2_NO2DDURING3DSCENE");
    // STRING: FF7 0x0090e4ac
    ReportFlagAndTest(driverCaps->caps2, 0x200, "DDCAPS2_NONLOCALVIDMEM");
    // STRING: FF7 0x0090e4c4
    ReportFlagAndTest(driverCaps->caps2, 0x400, "DDCAPS2_NONLOCALVIDMEMCAPS");
    // STRING: FF7 0x0090e4e0
    ReportFlagAndTest(driverCaps->caps2, 0x800, "DDCAPS2_NOPAGELOCKREQUIRED");
    // STRING: FF7 0x0090e4fc
    ReportFlagAndTest(driverCaps->caps2, 4, "DDCAPS2_VIDEOPORT");
    // STRING: FF7 0x0090e510
    ReportFlagAndTest(driverCaps->caps2, 0x1000, "DDCAPS2_WIDESURFACES");
    // STRING: FF7 0x0090e528
    Gfx_DebugPrint("DRIVER PALETTE CAPS \n");
    // STRING: FF7 0x0090e540
    ReportFlagAndTest(driverCaps->palCaps, 0x40, "DDPCAPS_ALLOW256");
    // STRING: FF7 0x0090e554
    sprintf(buffer, "  VIDEO MEMORY: TOTAL %d FREE %d \n", driverCaps->videoMemTotal, driverCaps->videoMemFree);
    Gfx_DebugPrint(buffer);
    // STRING: FF7 0x0090e578
    Gfx_DebugPrint("\n DRIVER SURFACE CAPS\n");
    Gfx_LogDirectDrawSurfaceCaps((int *)((char *)driverCaps + 0x84));
    // STRING: FF7 0x0090e590
    Gfx_DebugPrint("\n");
}

/*
 * Reports Direct3D depth and alpha comparison capability flags.
 */
// FUNCTION: FF7 0x0067e60a
int __cdecl Gfx_LogD3DCompareCaps(int compareCaps)
{
    int result;

    result = 1;
    // STRING: FF7 0x0090e594
    if (ReportFlagAndTest(compareCaps, 1, "D3DPCMPCAPS_NEVER") != 0) {
        result = 0;
    }
    // STRING: FF7 0x0090e5a8
    ReportFlagAndTest(compareCaps, 2, "D3DPCMPCAPS_LESS");
    // STRING: FF7 0x0090e5bc
    ReportFlagAndTest(compareCaps, 4, "D3DPCMPCAPS_EQUAL");
    // STRING: FF7 0x0090e5d0
    ReportFlagAndTest(compareCaps, 8, "D3DPCMPCAPS_LESSEQUAL");
    // STRING: FF7 0x0090e5e8
    ReportFlagAndTest(compareCaps, 0x10, "D3DPCMPCAPS_GREATER");
    // STRING: FF7 0x0090e5fc
    ReportFlagAndTest(compareCaps, 0x20, "D3DPCMPCAPS_NOTEQUAL");
    // STRING: FF7 0x0090e614
    ReportFlagAndTest(compareCaps, 0x40, "D3DPCMPCAPS_GREATEREQUAL");
    // STRING: FF7 0x0090e630
    ReportFlagAndTest(compareCaps, 0x80, "D3DPCMPCAPS_ALWAYS");
    return result;
}

/*
 * Logs Direct3D primitive raster, blend, texture, and shading capabilities.
 */
// FUNCTION: FF7 0x0067e6c2
void __cdecl Gfx_LogD3DPrimCapsToFlags(GfxD3dPrimCaps *primCaps, GfxD3dPrimCaps *outCaps)
{
    // STRING: FF7 0x0090e644
    Gfx_DebugPrint(" MISC:\n");
    // STRING: FF7 0x0090e64c
    ReportFlagAndTest(primCaps->miscCaps, 1, "D3DPMISCCAPS_MASKPLANES");
    // STRING: FF7 0x0090e664
    if (ReportFlagAndTest(primCaps->miscCaps, 2, "D3DPMISCCAPS_MASKZ") != 0) {
        outCaps->flags |= 0x10000;
    }
    // STRING: FF7 0x0090e678
    ReportFlagAndTest(primCaps->miscCaps, 4, "D3DPMISCCAPS_LINEPATTERNREP");
    // STRING: FF7 0x0090e694
    ReportFlagAndTest(primCaps->miscCaps, 8, "D3DPMISCCAPS_CONFORMANT");
    // STRING: FF7 0x0090e6ac
    if (ReportFlagAndTest(primCaps->miscCaps, 0x10, "D3DPMISCCAPS_CULLNONE") != 0) {
        outCaps->flags |= 0x4000;
    }
    // STRING: FF7 0x0090e6c4
    if (ReportFlagAndTest(primCaps->miscCaps, 0x20, "D3DPMISCCAPS_CULLCW") != 0) {
        // STRING: FF7 0x0090e6d8
        if (ReportFlagAndTest(primCaps->miscCaps, 0x40, "D3DPMISCCAPS_CULLCCW") != 0) {
            outCaps->flags |= 0x2000;
        }
    }
    // STRING: FF7 0x0090e6f0
    Gfx_DebugPrint(" RASTER:\n");
    // STRING: FF7 0x0090e6fc
    if (ReportFlagAndTest(primCaps->rasterCaps, 1, "D3DPRASTERCAPS_DITHER") != 0) {
        outCaps->flags |= 0x200;
    }
    // STRING: FF7 0x0090e714
    ReportFlagAndTest(primCaps->rasterCaps, 2, "D3DPRASTERCAPS_ROP2");
    // STRING: FF7 0x0090e728
    ReportFlagAndTest(primCaps->rasterCaps, 4, "D3DPRASTERCAPS_XOR");
    // STRING: FF7 0x0090e73c
    ReportFlagAndTest(primCaps->rasterCaps, 8, "D3DPRASTERCAPS_PAT");
    // STRING: FF7 0x0090e750
    ReportFlagAndTest(primCaps->rasterCaps, 0x10, "D3DPRASTERCAPS_ZTEST");
    // STRING: FF7 0x0090e768
    ReportFlagAndTest(primCaps->rasterCaps, 0x20, "D3DPRASTERCAPS_SUBPIXEL");
    // STRING: FF7 0x0090e780
    ReportFlagAndTest(primCaps->rasterCaps, 0x40, "D3DPRASTERCAPS_SUBPIXELX");
    // STRING: FF7 0x0090e79c
    ReportFlagAndTest(primCaps->rasterCaps, 0x80, "D3DPRASTERCAPS_FOGVERTEX");
    // STRING: FF7 0x0090e7b8
    ReportFlagAndTest(primCaps->rasterCaps, 0x100, "D3DPRASTERCAPS_FOGTABLE");
    // STRING: FF7 0x0090e7d0
    ReportFlagAndTest(primCaps->rasterCaps, 0x200, "D3DPRASTERCAPS_STIPPLE");
    // STRING: FF7 0x0090e7e8
    ReportFlagAndTest(primCaps->rasterCaps, 0x400, "D3DPRASTERCAPS_ANTIALIASSORTDEPENDENT");
    // STRING: FF7 0x0090e810
    ReportFlagAndTest(primCaps->rasterCaps, 0x800, "D3DPRASTERCAPS_ANTIALIASSORTINDEPENDENT");
    // STRING: FF7 0x0090e838
    ReportFlagAndTest(primCaps->rasterCaps, 0x1000, "D3DPRASTERCAPS_ANTIALIASEDGES");
    // STRING: FF7 0x0090e858
    ReportFlagAndTest(primCaps->rasterCaps, 0x2000, "D3DPRASTERCAPS_MIPMAPLODBIAS");
    // STRING: FF7 0x0090e878
    ReportFlagAndTest(primCaps->rasterCaps, 0x4000, "D3DPRASTERCAPS_ZBIAS");
    // STRING: FF7 0x0090e890
    ReportFlagAndTest(primCaps->rasterCaps, 0x8000, "D3DPRASTERCAPS_ZBUFFERLESSHSR");
    // STRING: FF7 0x0090e8b0
    ReportFlagAndTest(primCaps->rasterCaps, 0x10000, "D3DPRASTERCAPS_FOGRANGE");
    // STRING: FF7 0x0090e8c8
    ReportFlagAndTest(primCaps->rasterCaps, 0x20000, "D3DPRASTERCAPS_ANISOTROPY");
    // STRING: FF7 0x0090e8e4
    Gfx_DebugPrint(" Z COMPARE:\n");
    Gfx_LogD3DCompareCaps(primCaps->zCompareCaps);
    // STRING: FF7 0x0090e8f4
    Gfx_DebugPrint(" SOURCE BLEND:\n");
    // STRING: FF7 0x0090e904
    ReportFlagAndTest(primCaps->srcBlendCaps, 1, "D3DPBLENDCAPS_ZERO");
    // STRING: FF7 0x0090e918
    ReportFlagAndTest(primCaps->srcBlendCaps, 2, "D3DPBLENDCAPS_ONE");
    // STRING: FF7 0x0090e92c
    ReportFlagAndTest(primCaps->srcBlendCaps, 4, "D3DPBLENDCAPS_SRCCOLOR");
    // STRING: FF7 0x0090e944
    ReportFlagAndTest(primCaps->srcBlendCaps, 8, "D3DPBLENDCAPS_INVSRCCOLOR");
    // STRING: FF7 0x0090e960
    ReportFlagAndTest(primCaps->srcBlendCaps, 0x10, "D3DPBLENDCAPS_SRCALPHA");
    // STRING: FF7 0x0090e978
    ReportFlagAndTest(primCaps->srcBlendCaps, 0x20, "D3DPBLENDCAPS_INVSRCALPHA");
    // STRING: FF7 0x0090e994
    ReportFlagAndTest(primCaps->srcBlendCaps, 0x40, "D3DPBLENDCAPS_DESTALPHA");
    // STRING: FF7 0x0090e9ac
    ReportFlagAndTest(primCaps->srcBlendCaps, 0x80, "D3DPBLENDCAPS_INVDESTALPHA");
    // STRING: FF7 0x0090e9c8
    ReportFlagAndTest(primCaps->srcBlendCaps, 0x100, "D3DPBLENDCAPS_DESTCOLOR");
    // STRING: FF7 0x0090e9e0
    ReportFlagAndTest(primCaps->srcBlendCaps, 0x200, "D3DPBLENDCAPS_INVDESTCOLOR");
    // STRING: FF7 0x0090e9fc
    ReportFlagAndTest(primCaps->srcBlendCaps, 0x400, "D3DPBLENDCAPS_SRCALPHASAT");
    // STRING: FF7 0x0090ea18
    ReportFlagAndTest(primCaps->srcBlendCaps, 0x800, "D3DPBLENDCAPS_BOTHSRCALPHA");
    // STRING: FF7 0x0090ea34
    ReportFlagAndTest(primCaps->srcBlendCaps, 0x1000, "D3DPBLENDCAPS_BOTHINVSRCALPHA");
    // STRING: FF7 0x0090ea54
    Gfx_DebugPrint(" DESTINATION BLEND:\n");
    // STRING: FF7 0x0090ea6c
    ReportFlagAndTest(primCaps->destBlendCaps, 1, "D3DPBLENDCAPS_ZERO");
    // STRING: FF7 0x0090ea80
    ReportFlagAndTest(primCaps->destBlendCaps, 2, "D3DPBLENDCAPS_ONE");
    // STRING: FF7 0x0090ea94
    ReportFlagAndTest(primCaps->destBlendCaps, 4, "D3DPBLENDCAPS_SRCCOLOR");
    // STRING: FF7 0x0090eaac
    ReportFlagAndTest(primCaps->destBlendCaps, 8, "D3DPBLENDCAPS_INVSRCCOLOR");
    // STRING: FF7 0x0090eac8
    ReportFlagAndTest(primCaps->destBlendCaps, 0x10, "D3DPBLENDCAPS_SRCALPHA");
    // STRING: FF7 0x0090eae0
    ReportFlagAndTest(primCaps->destBlendCaps, 0x20, "D3DPBLENDCAPS_INVSRCALPHA");
    // STRING: FF7 0x0090eafc
    ReportFlagAndTest(primCaps->destBlendCaps, 0x40, "D3DPBLENDCAPS_DESTALPHA");
    // STRING: FF7 0x0090eb14
    ReportFlagAndTest(primCaps->destBlendCaps, 0x80, "D3DPBLENDCAPS_INVDESTALPHA");
    // STRING: FF7 0x0090eb30
    ReportFlagAndTest(primCaps->destBlendCaps, 0x100, "D3DPBLENDCAPS_DESTCOLOR");
    // STRING: FF7 0x0090eb48
    ReportFlagAndTest(primCaps->destBlendCaps, 0x200, "D3DPBLENDCAPS_INVDESTCOLOR");
    // STRING: FF7 0x0090eb64
    ReportFlagAndTest(primCaps->destBlendCaps, 0x400, "D3DPBLENDCAPS_SRCALPHASAT");
    // STRING: FF7 0x0090eb80
    ReportFlagAndTest(primCaps->destBlendCaps, 0x800, "D3DPBLENDCAPS_BOTHSRCALPHA");
    // STRING: FF7 0x0090eb9c
    ReportFlagAndTest(primCaps->destBlendCaps, 0x1000, "D3DPBLENDCAPS_BOTHINVSRCALPHA");
    // STRING: FF7 0x0090ebbc
    Gfx_DebugPrint(" ALPHA COMPARE:\n");
    if (Gfx_LogD3DCompareCaps(primCaps->alphaCompareCaps) != 0) {
        outCaps->flags |= 0x800;
    }
    // STRING: FF7 0x0090ebd0
    Gfx_DebugPrint(" SHADING CAPS:\n");
    // STRING: FF7 0x0090ebe0
    ReportFlagAndTest(primCaps->shadeCaps, 1, "D3DPSHADECAPS_COLORFLATMONO");
    // STRING: FF7 0x0090ebfc
    ReportFlagAndTest(primCaps->shadeCaps, 2, "D3DPSHADECAPS_COLORFLATRGB");
    // STRING: FF7 0x0090ec18
    ReportFlagAndTest(primCaps->shadeCaps, 4, "D3DPSHADECAPS_COLORGOURAUDMONO");
    // STRING: FF7 0x0090ec38
    ReportFlagAndTest(primCaps->shadeCaps, 8, "D3DPSHADECAPS_COLORGOURAUDRGB");
    // STRING: FF7 0x0090ec58
    ReportFlagAndTest(primCaps->shadeCaps, 0x10, "D3DPSHADECAPS_COLORPHONGMONO");
    // STRING: FF7 0x0090ec78
    ReportFlagAndTest(primCaps->shadeCaps, 0x20, "D3DPSHADECAPS_COLORPHONGRGB");
    // STRING: FF7 0x0090ec94
    ReportFlagAndTest(primCaps->shadeCaps, 0x40, "D3DPSHADECAPS_SPECULARFLATMONO");
    // STRING: FF7 0x0090ecb4
    ReportFlagAndTest(primCaps->shadeCaps, 0x80, "D3DPSHADECAPS_SPECULARFLATRGB");
    // STRING: FF7 0x0090ecd4
    ReportFlagAndTest(primCaps->shadeCaps, 0x100, "D3DPSHADECAPS_SPECULARGOURAUDMONO");
    // STRING: FF7 0x0090ecf8
    ReportFlagAndTest(primCaps->shadeCaps, 0x200, "D3DPSHADECAPS_SPECULARGOURAUDRGB");
    // STRING: FF7 0x0090ed1c
    ReportFlagAndTest(primCaps->shadeCaps, 0x400, "D3DPSHADECAPS_SPECULARPHONGMONO");
    // STRING: FF7 0x0090ed3c
    ReportFlagAndTest(primCaps->shadeCaps, 0x800, "D3DPSHADECAPS_SPECULARPHONGRGB");
    // STRING: FF7 0x0090ed5c
    if (ReportFlagAndTest(primCaps->shadeCaps, 0x1000, "D3DPSHADECAPS_ALPHAFLATBLEND") != 0) {
        outCaps->flags |= 0x400;
        outCaps->miscCaps |= 1;
    }
    // STRING: FF7 0x0090ed7c
    if (ReportFlagAndTest(primCaps->shadeCaps, 0x2000, "D3DPSHADECAPS_ALPHAFLATSTIPPLED") != 0) {
        outCaps->flags |= 0x400;
        outCaps->miscCaps |= 4;
    }
    // STRING: FF7 0x0090ed9c
    if (ReportFlagAndTest(primCaps->shadeCaps, 0x4000, "D3DPSHADECAPS_ALPHAGOURAUDBLEND") != 0) {
        outCaps->flags |= 0x400;
        outCaps->miscCaps |= 2;
    }
    // STRING: FF7 0x0090edbc
    if (ReportFlagAndTest(primCaps->shadeCaps, 0x8000, "D3DPSHADECAPS_ALPHAGOURAUDSTIPPLED") != 0) {
        outCaps->flags |= 0x400;
        outCaps->miscCaps |= 4;
    }
    // STRING: FF7 0x0090ede0
    ReportFlagAndTest(primCaps->shadeCaps, 0x10000, "D3DPSHADECAPS_ALPHAPHONGBLEND");
    // STRING: FF7 0x0090ee00
    ReportFlagAndTest(primCaps->shadeCaps, 0x20000, "D3DPSHADECAPS_ALPHAPHONGSTIPPLED");
    // STRING: FF7 0x0090ee24
    ReportFlagAndTest(primCaps->shadeCaps, 0x40000, "D3DPSHADECAPS_FOGFLAT");
    // STRING: FF7 0x0090ee3c
    ReportFlagAndTest(primCaps->shadeCaps, 0x80000, "D3DPSHADECAPS_FOGGOURAUD");
    // STRING: FF7 0x0090ee58
    ReportFlagAndTest(primCaps->shadeCaps, 0x100000, "D3DPSHADECAPS_FOGPHONG");
    // STRING: FF7 0x0090ee70
    Gfx_DebugPrint(" TEXTURE CAPS:\n");
    // STRING: FF7 0x0090ee80
    if (ReportFlagAndTest(primCaps->textureCaps, 1, "D3DPTEXTURECAPS_PERSPECTIVE") != 0) {
        outCaps->flags |= 8;
    }
    // STRING: FF7 0x0090ee9c
    ReportFlagAndTest(primCaps->textureCaps, 2, "D3DPTEXTURECAPS_POW2");
    // STRING: FF7 0x0090eeb4
    if (ReportFlagAndTest(primCaps->textureCaps, 4, "D3DPTEXTURECAPS_ALPHA") != 0) {
        outCaps->flags |= 0x10;
    }
    // STRING: FF7 0x0090eecc
    ReportFlagAndTest(primCaps->textureCaps, 8, "D3DPTEXTURECAPS_TRANSPARENCY");
    // STRING: FF7 0x0090eeec
    ReportFlagAndTest(primCaps->textureCaps, 0x10, "D3DPTEXTURECAPS_BORDER");
    // STRING: FF7 0x0090ef04
    ReportFlagAndTest(primCaps->textureCaps, 0x20, "D3DPTEXTURECAPS_SQUAREONLY");
    // STRING: FF7 0x0090ef20
    Gfx_DebugPrint(" TEXTURE FILTER:\n");
    // STRING: FF7 0x0090ef34
    ReportFlagAndTest(primCaps->textureFilterCaps, 1, "D3DPTFILTERCAPS_NEAREST");
    // STRING: FF7 0x0090ef4c
    if (ReportFlagAndTest(primCaps->textureFilterCaps, 2, "D3DPTFILTERCAPS_LINEAR") != 0) {
        outCaps->flags |= 4;
    }
    // STRING: FF7 0x0090ef64
    ReportFlagAndTest(primCaps->textureFilterCaps, 4, "D3DPTFILTERCAPS_MIPNEAREST");
    // STRING: FF7 0x0090ef80
    ReportFlagAndTest(primCaps->textureFilterCaps, 8, "D3DPTFILTERCAPS_MIPLINEAR");
    // STRING: FF7 0x0090ef9c
    ReportFlagAndTest(primCaps->textureFilterCaps, 0x10, "D3DPTFILTERCAPS_LINEARMIPNEAREST");
    // STRING: FF7 0x0090efc0
    ReportFlagAndTest(primCaps->textureFilterCaps, 0x20, "D3DPTFILTERCAPS_LINEARMIPLINEAR");
    // STRING: FF7 0x0090efe0
    Gfx_DebugPrint(" TEXTURE BLEND:\n");
    // STRING: FF7 0x0090eff4
    ReportFlagAndTest(primCaps->textureBlendCaps, 1, "D3DPTBLENDCAPS_DECAL");
    // STRING: FF7 0x0090f00c
    ReportFlagAndTest(primCaps->textureBlendCaps, 2, "D3DPTBLENDCAPS_MODULATE");
    // STRING: FF7 0x0090f024
    ReportFlagAndTest(primCaps->textureBlendCaps, 4, "D3DPTBLENDCAPS_DECALALPHA");
    // STRING: FF7 0x0090f040
    ReportFlagAndTest(primCaps->textureBlendCaps, 8, "D3DPTBLENDCAPS_MODULATEALPHA");
    // STRING: FF7 0x0090f060
    ReportFlagAndTest(primCaps->textureBlendCaps, 0x10, "D3DPTBLENDCAPS_DECALMASK");
    // STRING: FF7 0x0090f07c
    ReportFlagAndTest(primCaps->textureBlendCaps, 0x20, "D3DPTBLENDCAPS_MODULATEMASK");
    // STRING: FF7 0x0090f098
    ReportFlagAndTest(primCaps->textureBlendCaps, 0x40, "D3DPTBLENDCAPS_COPY");
    // STRING: FF7 0x0090f0ac
    ReportFlagAndTest(primCaps->textureBlendCaps, 0x80, "D3DPTBLENDCAPS_ADD");
    // STRING: FF7 0x0090f0c0
    Gfx_DebugPrint(" TEXTURE ADDRESS:\n");
    // STRING: FF7 0x0090f0d4
    ReportFlagAndTest(primCaps->textureAddressCaps, 4, "D3DPTADDRESSCAPS_CLAMP");
    // STRING: FF7 0x0090f0ec
    ReportFlagAndTest(primCaps->textureAddressCaps, 2, "D3DPTADDRESSCAPS_MIRROR");
    // STRING: FF7 0x0090f104
    ReportFlagAndTest(primCaps->textureAddressCaps, 1, "D3DPTADDRESSCAPS_WRAP");
    // STRING: FF7 0x0090f11c
    ReportFlagAndTest(primCaps->textureAddressCaps, 8, "D3DPTADDRESSCAPS_BORDER");
    // STRING: FF7 0x0090f134
    ReportFlagAndTest(primCaps->textureAddressCaps, 0x10, "D3DPTADDRESSCAPS_INDEPENDENTUV");
}

/*
 * Logs Direct3D device description, capabilities, and supported depth formats.
 */
// FUNCTION: FF7 0x0067f149
void __cdecl Gfx_LogD3DDeviceDesc(int verbosity, GfxD3DDeviceDesc *deviceDesc, GfxD3dPrimCaps *triangleCaps, GfxD3dPrimCaps *lineCaps)
{
    int maxVertices;
    int savedVerbosity;
    char buffer[0xC8];
    int batchLimit;
    int clipSupported;

    savedVerbosity = GetLogVerbosity(GetEngineContext());
    SetLogVerbosity(verbosity, GetEngineContext());
    if (deviceDesc != 0) {
        batchLimit = 0;
        maxVertices = 0;
        // STRING: FF7 0x0090f154
        Gfx_DebugPrint("D3D DEVICE \n");
        if ((deviceDesc->flags & D3DDD_COLORMODEL) != 0) {
            switch (deviceDesc->colorModel) {
            case D3DCOLOR_MONO:
                // STRING: FF7 0x0090f164
                Gfx_DebugPrint("  MONO COLOR MODEL \n");
                break;
            case D3DCOLOR_RGB:
                // STRING: FF7 0x0090f17c
                Gfx_DebugPrint("  RGB COLOR MODEL \n");
                break;
            default:
                // STRING: FF7 0x0090f190
                sprintf(buffer, "  UNKNOWN COLOR MODEL: %d \n", deviceDesc->colorModel);
                Gfx_DebugPrint(buffer);
            }
        }
        if ((deviceDesc->flags & D3DDD_DEVCAPS) != 0) {
            // STRING: FF7 0x0090f1ac
            ReportFlagAndTest(deviceDesc->devCaps, 1, "D3DDEVCAPS_FLOATTLVERTEX");
            // STRING: FF7 0x0090f1c8
            ReportFlagAndTest(deviceDesc->devCaps, 2, "D3DDEVCAPS_SORTINCREASINGZ");
            // STRING: FF7 0x0090f1e4
            ReportFlagAndTest(deviceDesc->devCaps, 4, "D3DDEVCAPS_SORTDECREASINGZ");
            // STRING: FF7 0x0090f200
            ReportFlagAndTest(deviceDesc->devCaps, 8, "D3DDEVCAPS_SORTEXACT");
            // STRING: FF7 0x0090f218
            ReportFlagAndTest(deviceDesc->devCaps, 0x10, "D3DDEVCAPS_EXECUTESYSTEMMEMORY");
            // STRING: FF7 0x0090f238
            ReportFlagAndTest(deviceDesc->devCaps, 0x20, "D3DDEVCAPS_EXECUTEVIDEOMEMORY");
            // STRING: FF7 0x0090f258
            ReportFlagAndTest(deviceDesc->devCaps, 0x40, "D3DDEVCAPS_TLVERTEXSYSTEMMEMORY");
            // STRING: FF7 0x0090f278
            ReportFlagAndTest(deviceDesc->devCaps, 0x80, "D3DDEVCAPS_TLVERTEXVIDEOMEMORY");
            // STRING: FF7 0x0090f298
            ReportFlagAndTest(deviceDesc->devCaps, 0x100, "D3DDEVCAPS_TEXTURESYSTEMMEMORY");
            // STRING: FF7 0x0090f2b8
            ReportFlagAndTest(deviceDesc->devCaps, 0x200, "D3DDEVCAPS_TEXTUREVIDEOMEMORY");
            // STRING: FF7 0x0090f2d8
            ReportFlagAndTest(deviceDesc->devCaps, 0x400, "D3DDEVCAPS_DRAWPRIMTLVERTEX");
            // STRING: FF7 0x0090f2f4
            ReportFlagAndTest(deviceDesc->devCaps, 0x800, "D3DDEVCAPS_CANRENDERAFTERFLIP");
            // STRING: FF7 0x0090f314
            ReportFlagAndTest(deviceDesc->devCaps, 0x1000, "D3DDEVCAPS_TEXTURENONLOCALVIDMEM");
        }
        if ((deviceDesc->flags & D3DDD_TRANSFORMCAPS) != 0) {
            // STRING: FF7 0x0090f338
            Gfx_DebugPrint("TRANSFORM:\n");
            // STRING: FF7 0x0090f344
            ReportFlagAndTest(deviceDesc->transformCaps, 1, "D3DTRANSFORMCAPS_CLIP");
        }
        clipSupported = 0;
        if ((deviceDesc->flags & D3DDD_BCLIPPING) != 0 && deviceDesc->clippingSupported != 0) {
            clipSupported = 1;
        }
        // STRING: FF7 0x0090f35c
        sprintf(buffer, "  CLIP 3D %d \n", clipSupported);
        Gfx_DebugPrint(buffer);
        if ((deviceDesc->flags & D3DDD_LIGHTINGCAPS) != 0) {
            // STRING: FF7 0x0090f36c
            Gfx_DebugPrint("LIGHTING:\n");
            switch (deviceDesc->shadeColorModel) {
            case D3DLIGHTINGMODEL_RGB:
                // STRING: FF7 0x0090f378
                Gfx_DebugPrint("  RGB COLOR MODEL \n");
                break;
            case D3DLIGHTINGMODEL_MONO:
                // STRING: FF7 0x0090f38c
                Gfx_DebugPrint("  MONO COLOR MODEL \n");
                break;
            default:
                // STRING: FF7 0x0090f3a4
                sprintf(buffer, "  UNKNOWN COLOR MODEL: %d \n", deviceDesc->shadeColorModel);
                Gfx_DebugPrint(buffer);
            }
            // STRING: FF7 0x0090f3c0
            ReportFlagAndTest(deviceDesc->lightCaps, 1, "D3DLIGHTCAPS_POINT");
            // STRING: FF7 0x0090f3d4
            ReportFlagAndTest(deviceDesc->lightCaps, 2, "D3DLIGHTCAPS_SPOT");
            // STRING: FF7 0x0090f3e8
            ReportFlagAndTest(deviceDesc->lightCaps, 4, "D3DLIGHTCAPS_DIRECTIONAL");
            // STRING: FF7 0x0090f404
            ReportFlagAndTest(deviceDesc->lightCaps, 8, "D3DLIGHTCAPS_PARALLELPOINT");
            // STRING: FF7 0x0090f420
            sprintf(buffer, "  MAXIMUM LIGHTS %d \n", deviceDesc->maxLights);
            Gfx_DebugPrint(buffer);
        }
        if ((deviceDesc->flags & D3DDD_LINECAPS) != 0) {
            // STRING: FF7 0x0090f438
            Gfx_DebugPrint("LINE:\n");
            Gfx_LogD3DPrimCapsToFlags((GfxD3dPrimCaps *)((int)deviceDesc + 0x64), lineCaps);
        }
        if ((deviceDesc->flags & D3DDD_TRICAPS) != 0) {
            // STRING: FF7 0x0090f440
            Gfx_DebugPrint("TRIANGLE:\n");
            Gfx_LogD3DPrimCapsToFlags((GfxD3dPrimCaps *)((int)deviceDesc + 0x64), triangleCaps);
        }
        batchLimit = deviceDesc->batchLimit;
        maxVertices = deviceDesc->maxVertices;
        // STRING: FF7 0x0090f44c
        sprintf(buffer, "  MAX B %d MAX V %d \n", batchLimit, maxVertices);
        Gfx_DebugPrint(buffer);
        // STRING: FF7 0x0090f464
        sprintf(buffer, "  BITS R: %d Z: %d \n", PickZBufferDepth(deviceDesc->rBitDepth), PickZBufferDepth(deviceDesc->zBitDepth));
        Gfx_DebugPrint(buffer);
    }
    SetLogVerbosity(savedVerbosity, GetEngineContext());
}
} /* extern "C" */
