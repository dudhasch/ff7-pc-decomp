// src/lib/src/movie/movie.cpp
// Original: C:\lib\src\movie\movie.cpp
// Contents: Movie playback through DirectShow: opens and runs a multimedia stream, blits its frames
//   to the render target via DirectDraw or Direct3D, and tracks the playing, stop-requested and
//   graph-open state.

/* Compiled as C++ (/Tp); extern "C" keeps C symbol linkage.
 * Retail movie player queries app paths/scaling and invokes App_OnActivateApp/Trace_Printf. */
#include "ff7/api/main.h"
#include "lib/codegen.h"
#include "lib/api/graphics.h"
#include "lib/types/graphics.h"
#include "lib/enums/graphics.h"
#include "lib/enums/movie.h"
#include "lib/types/movie.h"
#include <string.h>
#include "lib/api/movie.h"
#include "lib/com_abi.h"

#pragma intrinsic(strcat, strcmp, strcpy)
extern "C" {
// clang-format off
// 8-byte-aligned .bss padding immediately following main_no_op_hooks.cpp in link order.
unsigned char g_bssPad_009a0d08[268] = {0};
// GLOBAL: FF7 0x009a0e14
struct ComDirectDrawMediaStream *g_MovieDirectDrawMediaStream = 0;
// GLOBAL: FF7 0x009a0e18
int g_Movie_allowFrameDrop = 0;
// GLOBAL: FF7 0x009a0e1c
struct ComUnknown *g_MoviePrimaryVideoStream = 0;
// GLOBAL: FF7 0x009a0e20
int g_Movie_loopFlag = 0;
unsigned char g_bssPad_009a0e24[4] = {0};
// The movie texture's DDSURFACEDESC (Movie_Run fills it and passes it to CreateSurface).
// GLOBAL: FF7 0x009a0e28
GfxTextureSurfaceDesc g_Movie_textureDesc = {0};
// GLOBAL: FF7 0x009a0e94
struct ComUnknown *g_Movie_pGraphBuilder = 0;
// GLOBAL: FF7 0x009a0e98
int g_Movie_scaledHeight = 0;
unsigned char g_bssPad_009a0e9c[4] = {0};
// GLOBAL: FF7 0x009a0ea0
struct ComAMMultiMediaStream *g_MovieAMMultiMediaStream = 0;
// GLOBAL: FF7 0x009a0ea4
int g_Movie_pAmSurface = 0;
// GLOBAL: FF7 0x009a0ea8
int g_Movie_pStreamSample = 0;
// GLOBAL: FF7 0x009a0eac
int g_Movie_movieRect_left = 0;
// GLOBAL: FF7 0x009a0eb0
int g_Movie_movieRect_top = 0;
// GLOBAL: FF7 0x009a0eb4
int g_Movie_movieRect_right = 0;
// GLOBAL: FF7 0x009a0eb8
int g_Movie_movieRect_bottom = 0;
// GLOBAL: FF7 0x009a0ebc
int g_Movie_targetRect_left = 0;
// GLOBAL: FF7 0x009a0ec0
int g_Movie_targetRect_top = 0;
// GLOBAL: FF7 0x009a0ec4
int g_Movie_targetRect_right = 0;
// GLOBAL: FF7 0x009a0ec8
int g_Movie_targetRect_bottom = 0;
// GLOBAL: FF7 0x009a0ecc
struct ComUnknown *g_Movie_pSts1 = 0;
// GLOBAL: FF7 0x009a0ed0
struct ComUnknown *g_Movie_pVts1 = 0;
// GLOBAL: FF7 0x009a0ed4
struct ComUnknown *g_Movie_pSts2 = 0;
// GLOBAL: FF7 0x009a0ed8
struct ComUnknown *g_Movie_pVts2 = 0;
// GLOBAL: FF7 0x009a0edc
struct ComUnknown *g_Movie_pTexSts1 = 0;
// GLOBAL: FF7 0x009a0ee0
struct ComUnknown *g_Movie_pTexVts1 = 0;
// GLOBAL: FF7 0x009a0ee4
struct ComUnknown *g_Movie_pTexSts2 = 0;
// GLOBAL: FF7 0x009a0ee8
struct ComUnknown *g_Movie_pTexVts2 = 0;
// GLOBAL: FF7 0x009a0eec
int g_Movie_hTex1 = 0;
// GLOBAL: FF7 0x009a0ef0
int g_Movie_hTex2 = 0;
// GLOBAL: FF7 0x009a0ef4
int g_Movie_useD3D = 0;
// GLOBAL: FF7 0x009a0ef8
int g_Movie_scaledWidth = 0;
unsigned char g_bssPad_009a0efc[4] = {0};
// GLOBAL: FF7 0x009a0f00
GfxTlVertex g_Movie_quadVertices2[4] = {0};
// GLOBAL: FF7 0x009a0f80
GfxTlVertex g_Movie_quadVertices1[4] = {0};
// GLOBAL: FF7 0x009a1000
struct ComMediaSeeking *g_MovieMediaSeeking = 0;
// GLOBAL: FF7 0x009a1004
int g_Movie_uiScalingMode = 0;
unsigned char g_bssPad_009a1008[4] = {0};
// GLOBAL: FF7 0x009a100c
int g_Movie_scalingEnabled = 0;
// GLOBAL: FF7 0x009a1010
int g_Movie_isPlaying = 0;
// GLOBAL: FF7 0x009a1014
int g_Movie_isStopRequested = 0;
// GLOBAL: FF7 0x009a1018
int g_Movie_isGraphOpen = 0;
unsigned char g_bssPad_009a101c[4] = {0};
// clang-format on

/*
 * Resets playback state, zeroes COM stream pointers and status flags,
 * and configures Direct3D render mode based on engine context.
 */
// FUNCTION: FF7 0x00414f00
void __cdecl Movie_ResetPlaybackState(void)
{
    GfxEngineContext *context;

    context = GetEngineContext();
    g_MovieAMMultiMediaStream = 0;
    g_Movie_pAmSurface = 0;
    g_Movie_pStreamSample = 0;
    g_Movie_scalingEnabled = 0;
    g_Movie_isPlaying = 0;
    g_Movie_isStopRequested = 0;
    g_Movie_isGraphOpen = 0;
    if (context->modeActive == 0) {
        if (context->driverType != GFX_DRIVER_SOFTWARE) {
            g_Movie_useD3D = 1;
        }
    } else {
        g_Movie_useD3D = 0;
    }
}

/*
 * Selects and copies vertex quad scaling tables for UI scaling mode
 * and movie height (224 or 240 lines), and sets scaled dimensions.
 */
// FUNCTION: FF7 0x00414f84
void __cdecl Movie_SelectScalingTables(int movieWidth, int movieHeight)
{
    int i;

    g_Movie_uiScalingMode = GetUiScalingMode();
    if (g_Movie_uiScalingMode == 2) {
        if (movieHeight == 224) {
            for (i = 0; i < 4; i++) {
                g_Movie_quadVertices1[i] = g_Movie_quad_Mode2_224_Left[i];
                g_Movie_quadVertices2[i] = g_Movie_quad_Mode2_224_Right[i];
            }
        } else if (movieHeight == 240) {
            for (i = 0; i < 4; i++) {
                g_Movie_quadVertices1[i] = g_Movie_quad_Mode2_240_Left[i];
                g_Movie_quadVertices2[i] = g_Movie_quad_Mode2_240_Right[i];
            }
        }
    } else if (g_Movie_uiScalingMode == 0) {
        if (movieHeight == 224) {
            for (i = 0; i < 4; i++) {
                g_Movie_quadVertices1[i] = g_Movie_quad_Mode0_224_Left[i];
                g_Movie_quadVertices2[i] = g_Movie_quad_Mode0_224_Right[i];
            }
        } else if (movieHeight == 240) {
            for (i = 0; i < 4; i++) {
                g_Movie_quadVertices1[i] = g_Movie_quad_Mode0_240_Left[i];
                g_Movie_quadVertices2[i] = g_Movie_quad_Mode0_240_Right[i];
            }
        }
    } else if (movieHeight == 224) {
        for (i = 0; i < 4; i++) {
            g_Movie_quadVertices1[i] = g_Movie_quad_Mode1_224_Left[i];
            g_Movie_quadVertices2[i] = g_Movie_quad_Mode1_224_Right[i];
        }
    } else if (movieHeight == 240) {
        for (i = 0; i < 4; i++) {
            g_Movie_quadVertices1[i] = g_Movie_quad_Mode1_240_Left[i];
            g_Movie_quadVertices2[i] = g_Movie_quad_Mode1_240_Right[i];
        }
    }
    if (g_Movie_uiScalingMode == 2 || g_Movie_uiScalingMode == 1) {
        g_Movie_scaledWidth = movieWidth << 1;
        g_Movie_scaledHeight = movieHeight << 1;
    }
}

/*
 * Sets g_Movie_allowFrameDrop to 1 when the movie path is one of the 24 cutscenes permitted to
 * drop frames under low framerate conditions, and to 0 otherwise.
 */
// FUNCTION: FF7 0x00415231
void __cdecl Movie_SetFrameDropForMovie(char *moviePath)
{
    char candidate[256];

#define CLASSIFY_MOVIE(name)                 \
    strcpy(candidate, g_InitPath_MoviePath); \
    strcat(candidate, name);                 \
    if (strcmp(moviePath, candidate) == 0) { \
        g_Movie_allowFrameDrop = 1;                    \
        return;                              \
    }

    g_Movie_allowFrameDrop = 0;

    // STRING: FF7 0x007bfc40
    CLASSIFY_MOVIE("junon.avi");
    // STRING: FF7 0x007bfc4c
    CLASSIFY_MOVIE("biskdead.avi");
    // STRING: FF7 0x007bfc5c
    CLASSIFY_MOVIE("boogdown.avi");
    // STRING: FF7 0x007bfc6c
    CLASSIFY_MOVIE("boogstar.avi");
    // STRING: FF7 0x007bfc7c
    CLASSIFY_MOVIE("boogup.avi");
    // STRING: FF7 0x007bfc88
    CLASSIFY_MOVIE("fallpl.avi");
    // STRING: FF7 0x007bfc94
    CLASSIFY_MOVIE("jairofly.avi");
    // STRING: FF7 0x007bfca4
    CLASSIFY_MOVIE("mkup.avi");
    // STRING: FF7 0x007bfcb0
    CLASSIFY_MOVIE("northmk.avi");
    // STRING: FF7 0x007bfcbc
    CLASSIFY_MOVIE("opening.avi");
    // STRING: FF7 0x007bfcc8
    CLASSIFY_MOVIE("plrexp.avi");
    // STRING: FF7 0x007bfcd4
    CLASSIFY_MOVIE("smk.avi");
    // STRING: FF7 0x007bfcdc
    CLASSIFY_MOVIE("white2.avi");
    // STRING: FF7 0x007bfce8
    CLASSIFY_MOVIE("lastmap.avi");
    // STRING: FF7 0x007bfcf4
    CLASSIFY_MOVIE("lastflor.avi");
    // STRING: FF7 0x007bfd04
    CLASSIFY_MOVIE("last4_4.avi");
    // STRING: FF7 0x007bfd10
    CLASSIFY_MOVIE("last4_3.avi");
    // STRING: FF7 0x007bfd1c
    CLASSIFY_MOVIE("last4_2.avi");
    // STRING: FF7 0x007bfd28
    CLASSIFY_MOVIE("zmind03.avi");
    // STRING: FF7 0x007bfd34
    CLASSIFY_MOVIE("zmind02.avi");
    // STRING: FF7 0x007bfd40
    CLASSIFY_MOVIE("zmind01.avi");
    // STRING: FF7 0x007bfd4c
    CLASSIFY_MOVIE("parashot.avi");
    // STRING: FF7 0x007bfd5c
    CLASSIFY_MOVIE("meteosky.avi");
    // STRING: FF7 0x007bfd6c
    CLASSIFY_MOVIE("greatpit.avi");

#undef CLASSIFY_MOVIE
}
} /* extern "C" */
// clang-format off
// GLOBAL: FF7 0x007bf640
extern "C" GfxTlVertex g_Movie_quad_Mode2_224_Left[4] = {
    {0.0f, 0.0f, 0.999999f, 1.0f, 0xFFFFFFFF, 0xFF000000, 0.0f, 0.0f},
    {512.0f, 0.0f, 0.999999f, 1.0f, 0xFFFFFFFF, 0xFF000000, 1.0f, 0.0f},
    {512.0f, 448.0f, 0.999999f, 1.0f, 0xFFFFFFFF, 0xFF000000, 1.0f, 0.875f},
    {0.0f, 448.0f, 0.999999f, 1.0f, 0xFFFFFFFF, 0xFF000000, 0.0f, 0.875f},
};
// GLOBAL: FF7 0x007bf6c0
extern "C" GfxTlVertex g_Movie_quad_Mode2_224_Right[4] = {
    {512.0f, 0.0f, 0.999999f, 1.0f, 0xFFFFFFFF, 0xFF000000, 0.0f, 0.0f},
    {6.4e+02f, 0.0f, 0.999999f, 1.0f, 0xFFFFFFFF, 0xFF000000, 1.0f, 0.0f},
    {6.4e+02f, 448.0f, 0.999999f, 1.0f, 0xFFFFFFFF, 0xFF000000, 1.0f, 0.875f},
    {512.0f, 448.0f, 0.999999f, 1.0f, 0xFFFFFFFF, 0xFF000000, 0.0f, 0.875f},
};
// GLOBAL: FF7 0x007bf740
extern "C" GfxTlVertex g_Movie_quad_Mode2_240_Left[4] = {
    {0.0f, 0.0f, 0.999999f, 1.0f, 0xFFFFFFFF, 0xFF000000, 0.0f, 0.0f},
    {512.0f, 0.0f, 0.999999f, 1.0f, 0xFFFFFFFF, 0xFF000000, 1.0f, 0.0f},
    {512.0f, 4.8e+02f, 0.999999f, 1.0f, 0xFFFFFFFF, 0xFF000000, 1.0f, 0.875f},
    {0.0f, 4.8e+02f, 0.999999f, 1.0f, 0xFFFFFFFF, 0xFF000000, 0.0f, 0.875f},
};
// GLOBAL: FF7 0x007bf7c0
extern "C" GfxTlVertex g_Movie_quad_Mode2_240_Right[4] = {
    {512.0f, 0.0f, 0.999999f, 1.0f, 0xFFFFFFFF, 0xFF000000, 0.0f, 0.0f},
    {6.4e+02f, 0.0f, 0.999999f, 1.0f, 0xFFFFFFFF, 0xFF000000, 1.0f, 0.0f},
    {6.4e+02f, 4.8e+02f, 0.999999f, 1.0f, 0xFFFFFFFF, 0xFF000000, 1.0f, 0.875f},
    {512.0f, 4.8e+02f, 0.999999f, 1.0f, 0xFFFFFFFF, 0xFF000000, 0.0f, 0.875f},
};
// GLOBAL: FF7 0x007bf840
extern "C" GfxTlVertex g_Movie_quad_Mode1_224_Left[4] = {
    {1.6e+02f, 1.2e+02f, 0.999999f, 1.0f, 0xFFFFFFFF, 0xFF000000, 0.0f, 0.0f},
    {416.0f, 1.2e+02f, 0.999999f, 1.0f, 0xFFFFFFFF, 0xFF000000, 1.0f, 0.0f},
    {416.0f, 344.0f, 0.999999f, 1.0f, 0xFFFFFFFF, 0xFF000000, 1.0f, 0.875f},
    {1.6e+02f, 344.0f, 0.999999f, 1.0f, 0xFFFFFFFF, 0xFF000000, 0.0f, 0.875f},
};
// GLOBAL: FF7 0x007bf8c0
extern "C" GfxTlVertex g_Movie_quad_Mode1_224_Right[4] = {
    {416.0f, 1.2e+02f, 0.999999f, 1.0f, 0xFFFFFFFF, 0xFF000000, 0.0f, 0.0f},
    {4.8e+02f, 1.2e+02f, 0.999999f, 1.0f, 0xFFFFFFFF, 0xFF000000, 1.0f, 0.0f},
    {4.8e+02f, 344.0f, 0.999999f, 1.0f, 0xFFFFFFFF, 0xFF000000, 1.0f, 0.875f},
    {416.0f, 344.0f, 0.999999f, 1.0f, 0xFFFFFFFF, 0xFF000000, 0.0f, 0.875f},
};
// GLOBAL: FF7 0x007bf940
extern "C" GfxTlVertex g_Movie_quad_Mode1_240_Left[4] = {
    {1.6e+02f, 1.2e+02f, 0.999999f, 1.0f, 0xFFFFFFFF, 0xFF000000, 0.0f, 0.0f},
    {416.0f, 1.2e+02f, 0.999999f, 1.0f, 0xFFFFFFFF, 0xFF000000, 1.0f, 0.0f},
    {416.0f, 3.6e+02f, 0.999999f, 1.0f, 0xFFFFFFFF, 0xFF000000, 1.0f, 0.875f},
    {1.6e+02f, 3.6e+02f, 0.999999f, 1.0f, 0xFFFFFFFF, 0xFF000000, 0.0f, 0.875f},
};
// GLOBAL: FF7 0x007bf9c0
extern "C" GfxTlVertex g_Movie_quad_Mode1_240_Right[4] = {
    {416.0f, 1.2e+02f, 0.999999f, 1.0f, 0xFFFFFFFF, 0xFF000000, 0.0f, 0.0f},
    {4.8e+02f, 1.2e+02f, 0.999999f, 1.0f, 0xFFFFFFFF, 0xFF000000, 1.0f, 0.0f},
    {4.8e+02f, 3.6e+02f, 0.999999f, 1.0f, 0xFFFFFFFF, 0xFF000000, 1.0f, 0.875f},
    {416.0f, 3.6e+02f, 0.999999f, 1.0f, 0xFFFFFFFF, 0xFF000000, 0.0f, 0.875f},
};
// GLOBAL: FF7 0x007bfa40
extern "C" GfxTlVertex g_Movie_quad_Mode0_224_Left[4] = {
    {0.0f, 0.0f, 0.999999f, 1.0f, 0xFFFFFFFF, 0xFF000000, 0.0f, 0.0f},
    {256.0f, 0.0f, 0.999999f, 1.0f, 0xFFFFFFFF, 0xFF000000, 1.0f, 0.0f},
    {256.0f, 224.0f, 0.999999f, 1.0f, 0xFFFFFFFF, 0xFF000000, 1.0f, 0.875f},
    {0.0f, 224.0f, 0.999999f, 1.0f, 0xFFFFFFFF, 0xFF000000, 0.0f, 0.875f},
};
// GLOBAL: FF7 0x007bfac0
extern "C" GfxTlVertex g_Movie_quad_Mode0_224_Right[4] = {
    {256.0f, 0.0f, 0.999999f, 1.0f, 0xFFFFFFFF, 0xFF000000, 0.0f, 0.0f},
    {3.2e+02f, 0.0f, 0.999999f, 1.0f, 0xFFFFFFFF, 0xFF000000, 1.0f, 0.0f},
    {3.2e+02f, 224.0f, 0.999999f, 1.0f, 0xFFFFFFFF, 0xFF000000, 1.0f, 0.875f},
    {256.0f, 224.0f, 0.999999f, 1.0f, 0xFFFFFFFF, 0xFF000000, 0.0f, 0.875f},
};
// GLOBAL: FF7 0x007bfb40
extern "C" GfxTlVertex g_Movie_quad_Mode0_240_Left[4] = {
    {0.0f, 0.0f, 0.999999f, 1.0f, 0xFFFFFFFF, 0xFF000000, 0.0f, 0.0f},
    {256.0f, 0.0f, 0.999999f, 1.0f, 0xFFFFFFFF, 0xFF000000, 1.0f, 0.0f},
    {256.0f, 2.4e+02f, 0.999999f, 1.0f, 0xFFFFFFFF, 0xFF000000, 1.0f, 0.875f},
    {0.0f, 2.4e+02f, 0.999999f, 1.0f, 0xFFFFFFFF, 0xFF000000, 0.0f, 0.875f},
};
// GLOBAL: FF7 0x007bfbc0
extern "C" GfxTlVertex g_Movie_quad_Mode0_240_Right[4] = {
    {256.0f, 0.0f, 0.999999f, 1.0f, 0xFFFFFFFF, 0xFF000000, 0.0f, 0.0f},
    {3.2e+02f, 0.0f, 0.999999f, 1.0f, 0xFFFFFFFF, 0xFF000000, 1.0f, 0.0f},
    {3.2e+02f, 2.4e+02f, 0.999999f, 1.0f, 0xFFFFFFFF, 0xFF000000, 1.0f, 0.875f},
    {256.0f, 2.4e+02f, 0.999999f, 1.0f, 0xFFFFFFFF, 0xFF000000, 0.0f, 0.875f},
};
// clang-format on
#pragma intrinsic(memset, memcpy)
extern "C" {
// GLOBAL: FF7 0x007b8140
extern const int CLSID_AMMultiMediaStream;
__declspec(dllimport) int __stdcall CoCreateInstance(int, int, int, int, int);
// GLOBAL: FF7 0x007b80c0
extern const int IID_IAMMultiMediaStream;
// GLOBAL: FF7 0x007b81e0
extern const int MSPID_PrimaryVideo;
// GLOBAL: FF7 0x007b81f0
extern const int MSPID_PrimaryAudio;
__declspec(dllimport) int __stdcall MultiByteToWideChar(int, int, int, int, int, int);
// GLOBAL: FF7 0x007b8180
extern const int IID_IDirectDrawMediaStream;
extern const int IID_IDirect3DTexture2; /* DirectX 5 GUID from dxguid.lib. */
// GLOBAL: FF7 0x007b8070
extern const int IID_IMediaSeeking;
// GLOBAL: FF7 0x007b8060
extern const int TIME_FORMAT_FRAME;
void __cdecl Movie_LogSurfaceDescription(const char *surfaceName, int pSurface);

/*
 * Creates DirectShow filter graph, opens multimedia stream from file, creates
 * primary video stream sample, initializes DirectDraw surfaces and textures,
 * and sets up seeking interface.
 */
// FUNCTION: FF7 0x00416995
int __cdecl Movie_Open(int moviePath, int loopFlag, int pDirectDraw, int pDirectDraw2)
{
    GfxDirectDrawDriverCaps emulCaps;
    unsigned short wideName[260];
    GfxDirectDrawDriverCaps hardwareCaps;
    int res;
    GfxEngineContext *state;
    int device;

    state = GetEngineContext();
    res = 0;
    device = 0;
    if (GetEngineContext()->driverType == GFX_DRIVER_OPENGL) {
        return 1;
    } else {
        g_Movie_loopFlag = loopFlag;
        Movie_SetFrameDropForMovie((char *)moviePath);
        Movie_ResetPlaybackState();
        res = CoCreateInstance((int)&CLSID_AMMultiMediaStream, 0, 1, (int)&IID_IAMMultiMediaStream, (int)&g_MovieAMMultiMediaStream);
        if (res < 0) {
            // STRING: FF7 0x007bfd7c
            Trace_Printf("Failed to create AMMultiMediaStream object (0x%8.8X)\n", res);
            return 0;
        } else {
            res = ((ComAMMultiMediaStream *)(g_MovieAMMultiMediaStream))->Initialize(0, 1, 0);
            if (res < 0) {
                if (g_MovieAMMultiMediaStream != 0) {
                    ((ComUnknown *)(g_MovieAMMultiMediaStream))->Release();
                    g_MovieAMMultiMediaStream = 0;
                }
                // STRING: FF7 0x007bfdb4
                Trace_Printf("Failed to initialize amstream object (0x%8.8X)\n", res);
                return 0;
            } else {
                res = ((ComAMMultiMediaStream *)(g_MovieAMMultiMediaStream))->AddMediaStream((int)pDirectDraw, &MSPID_PrimaryVideo, 0, &g_MoviePrimaryVideoStream);
                if (res < 0) {
                    if (g_MovieAMMultiMediaStream != 0) {
                        ((ComUnknown *)(g_MovieAMMultiMediaStream))->Release();
                        g_MovieAMMultiMediaStream = 0;
                    }
                    // STRING: FF7 0x007bfde4
                    Trace_Printf("Failed to add video stream (0x%8.8X)\n", res);
                    return 0;
                } else {
                    res = ((ComAMMultiMediaStream *)(g_MovieAMMultiMediaStream))->AddMediaStream(0, &MSPID_PrimaryAudio, 1, 0);
                    if (res < 0) {
                        // STRING: FF7 0x007bfe0c
                        Trace_Printf("Failed to add audio stream, so no sound (0x%8.8X)\n", res);
                    }
                    if (MultiByteToWideChar(0, 0, (int)moviePath, -1, (int)&wideName, 0x104) == 0) {
                        if (g_MoviePrimaryVideoStream != 0) {
                            ((ComUnknown *)(g_MoviePrimaryVideoStream))->Release();
                            g_MoviePrimaryVideoStream = 0;
                        }
                        if (g_MovieAMMultiMediaStream != 0) {
                            ((ComUnknown *)(g_MovieAMMultiMediaStream))->Release();
                            g_MovieAMMultiMediaStream = 0;
                        }
                        // STRING: FF7 0x007bfe40
                        Trace_Printf("Failed to convert movie filename (0x%8.8X)\n", res);
                        return 0;
                    } else {
                        res = ((ComAMMultiMediaStream *)(g_MovieAMMultiMediaStream))->OpenFile(&wideName, 0);
                        if (res < 0) {
                            if (g_MoviePrimaryVideoStream != 0) {
                                ((ComUnknown *)(g_MoviePrimaryVideoStream))->Release();
                                g_MoviePrimaryVideoStream = 0;
                            }
                            if (g_MovieAMMultiMediaStream != 0) {
                                ((ComUnknown *)(g_MovieAMMultiMediaStream))->Release();
                                g_MovieAMMultiMediaStream = 0;
                            }
                            // STRING: FF7 0x007bfe6c
                            Trace_Printf("Failed to open movie file (0x%8.8X)\n", res);
                            return 0;
                        } else {
                            Movie_ProbeMultiMediaStreamInfo(moviePath, ((int)((int)g_MovieAMMultiMediaStream)));
                            res = ((ComUnknown *)(g_MoviePrimaryVideoStream))->QueryInterface(&IID_IDirectDrawMediaStream, &g_MovieDirectDrawMediaStream);
                            if (res < 0) {
                                if (g_MoviePrimaryVideoStream != 0) {
                                    ((ComUnknown *)(g_MoviePrimaryVideoStream))->Release();
                                    g_MoviePrimaryVideoStream = 0;
                                }
                                if (g_MovieAMMultiMediaStream != 0) {
                                    ((ComUnknown *)(g_MovieAMMultiMediaStream))->Release();
                                    g_MovieAMMultiMediaStream = 0;
                                }
                                // STRING: FF7 0x007bfe94
                                Trace_Printf("Failed to get IDirectDrawMediaStream (0x%8.8X)\n", res);
                                return 0;
                            } else {
                                memset((void *)&g_Movie_textureDesc, 0, sizeof(g_Movie_textureDesc));
                                g_Movie_textureDesc.size = sizeof(g_Movie_textureDesc);
                                res = ((ComDirectDrawMediaStream *)(g_MovieDirectDrawMediaStream))->GetFormat(&g_Movie_textureDesc, 0, 0, 0);
                                if (res < 0) {
                                    if (g_MovieDirectDrawMediaStream != 0) {
                                        ((ComUnknown *)(g_MovieDirectDrawMediaStream))->Release();
                                        g_MovieDirectDrawMediaStream = 0;
                                    }
                                    if (g_MoviePrimaryVideoStream != 0) {
                                        ((ComUnknown *)(g_MoviePrimaryVideoStream))->Release();
                                        g_MoviePrimaryVideoStream = 0;
                                    }
                                    if (g_MovieAMMultiMediaStream != 0) {
                                        ((ComUnknown *)(g_MovieAMMultiMediaStream))->Release();
                                        g_MovieAMMultiMediaStream = 0;
                                    }
                                    // STRING: FF7 0x007bfec4
                                    Trace_Printf("Failed to get movie surface format (0x%8.8X)\n", res);
                                    return 0;
                                } else {
                                    // STRING: FF7 0x007bfef4
                                    Trace_Printf("Movie dimensions: %ux%u\n", g_Movie_textureDesc.width, g_Movie_textureDesc.height);
                                    memset(&hardwareCaps, 0, sizeof(hardwareCaps));
                                    memset(&emulCaps, 0, sizeof(emulCaps));
                                    hardwareCaps.size = sizeof(hardwareCaps);
                                    emulCaps.size = sizeof(emulCaps);
                                    // STRING: FF7 0x007bff10
                                    if (CheckDirectDrawResult(((ComDirectDraw *)(state->ddObject))->GetCaps(&hardwareCaps, &emulCaps), "C:\\lib\\src\\movie\\movie.cpp", 0x3AC) != 0 && 0 != 0) {
                                        // STRING: FF7 0x007bff2c
                                        Gfx_DebugPrint("HAL \n");
                                        Gfx_LogDirectDrawDriverCaps(&hardwareCaps);
                                        // STRING: FF7 0x007bff34
                                        Gfx_DebugPrint("HEL \n");
                                        Gfx_LogDirectDrawDriverCaps(&emulCaps);
                                    }
                                    g_Movie_scaledWidth = g_Movie_textureDesc.width;
                                    g_Movie_scaledHeight = g_Movie_textureDesc.height;
                                    Movie_SelectScalingTables(g_Movie_scaledWidth, g_Movie_scaledHeight);
                                    g_Movie_movieRect_left = 0;
                                    g_Movie_movieRect_top = 0;
                                    g_Movie_movieRect_right = g_Movie_textureDesc.width;
                                    g_Movie_movieRect_bottom = g_Movie_textureDesc.height;
                                    g_Movie_targetRect_left = g_Movie_movieRect_left;
                                    g_Movie_targetRect_top = g_Movie_movieRect_top;
                                    g_Movie_targetRect_right = g_Movie_movieRect_right;
                                    g_Movie_targetRect_bottom = g_Movie_movieRect_bottom;
                                    if ((hardwareCaps.caps & 0x200) != 0) {
                                        g_Movie_scalingEnabled = 1;
                                    }
                                    if (state->driverType != GFX_DRIVER_SOFTWARE) {
                                        g_Movie_useD3D = 1;
                                    }
                                    if (g_Movie_uiScalingMode == 1 || g_Movie_uiScalingMode == 0) {
                                        g_Movie_scalingEnabled = 0;
                                    }
                                    if (g_Movie_scalingEnabled != 0 && g_Movie_uiScalingMode == 2) {
                                        while (g_Movie_targetRect_right < g_Movie_scaledWidth && g_Movie_targetRect_bottom < g_Movie_scaledHeight) {
                                            g_Movie_targetRect_right <<= 1;
                                            g_Movie_targetRect_bottom <<= 1;
                                        }
                                        if (g_Movie_targetRect_right > g_Movie_scaledWidth || g_Movie_targetRect_bottom > g_Movie_scaledHeight) {
                                            g_Movie_targetRect_right >>= 1;
                                            g_Movie_targetRect_bottom >>= 1;
                                        }
                                    }
                                    if (g_Movie_scalingEnabled != 0) {
                                        g_Movie_targetRect_left = g_Movie_scaledWidth - g_Movie_targetRect_right >> 1;
                                        g_Movie_targetRect_right += g_Movie_targetRect_left;
                                        g_Movie_targetRect_top = g_Movie_scaledHeight - g_Movie_targetRect_bottom >> 1;
                                        g_Movie_targetRect_bottom += g_Movie_targetRect_top;
                                    } else {
                                        g_Movie_targetRect_left = g_Movie_scaledWidth - g_Movie_targetRect_right >> 1;
                                        g_Movie_targetRect_right += g_Movie_targetRect_left;
                                        g_Movie_targetRect_top = g_Movie_scaledHeight - g_Movie_targetRect_bottom >> 1;
                                        g_Movie_targetRect_bottom += g_Movie_targetRect_top;
                                    }
                                    // STRING: FF7 0x007bff3c
                                    Trace_Printf("Movie rect: (%d, %d, %d, %d)\n", g_Movie_movieRect_left, g_Movie_movieRect_top, g_Movie_movieRect_right, g_Movie_movieRect_bottom);
                                    // STRING: FF7 0x007bff5c
                                    Trace_Printf("Target rect: (%d, %d, %d, %d)\n", g_Movie_targetRect_left, g_Movie_targetRect_top, g_Movie_targetRect_right, g_Movie_targetRect_bottom);
                                    if (g_Movie_scalingEnabled != 0) {
                                        if (g_Movie_uiScalingMode == 2) {
                                            g_Movie_textureDesc.caps &= ~DDSCAPS_SYSTEMMEMORY;
                                            g_Movie_textureDesc.caps |= DDSCAPS_VIDEOMEMORY;
                                        }
                                    } else {
                                        g_Movie_textureDesc.caps &= ~DDSCAPS_VIDEOMEMORY;
                                        g_Movie_textureDesc.caps |= DDSCAPS_SYSTEMMEMORY;
                                    }
                                    do {
                                        res = ((ComDirectDraw *)(pDirectDraw))->CreateSurface(&g_Movie_textureDesc, &g_Movie_pAmSurface, 0);
                                        if (res == DDERR_OUTOFVIDEOMEMORY) {
                                            g_Movie_textureDesc.caps &= ~DDSCAPS_VIDEOMEMORY;
                                            g_Movie_textureDesc.caps |= DDSCAPS_SYSTEMMEMORY;
                                            // STRING: FF7 0x007bff7c
                                            Trace_Printf("Out of video memory for movie, trying system\n");
                                        } else {
                                            if (res < 0) {
                                                if (g_MovieDirectDrawMediaStream != 0) {
                                                    ((ComUnknown *)(g_MovieDirectDrawMediaStream))->Release();
                                                    g_MovieDirectDrawMediaStream = 0;
                                                }
                                                if (g_MoviePrimaryVideoStream != 0) {
                                                    ((ComUnknown *)(g_MoviePrimaryVideoStream))->Release();
                                                    g_MoviePrimaryVideoStream = 0;
                                                }
                                                if (g_MovieAMMultiMediaStream != 0) {
                                                    ((ComUnknown *)(g_MovieAMMultiMediaStream))->Release();
                                                    g_MovieAMMultiMediaStream = 0;
                                                }
                                                // STRING: FF7 0x007bffac
                                                Trace_Printf("Failed to create movie surface (0x%8.8X)\n", res);
                                                return 0;
                                            }
                                            // STRING: FF7 0x007bffd8
                                            Movie_LogSurfaceDescription("Movie", g_Movie_pAmSurface);
                                        }
                                    } while (res == DDERR_OUTOFVIDEOMEMORY);
                                    res = ((ComDirectDrawMediaStream *)(g_MovieDirectDrawMediaStream))->CreateSample((int)g_Movie_pAmSurface, 0, 1, &g_Movie_pStreamSample);
                                    if (res < 0) {
                                        if (g_Movie_pAmSurface != 0) {
                                            ((ComUnknown *)(g_Movie_pAmSurface))->Release();
                                            g_Movie_pAmSurface = 0;
                                        }
                                        if (g_MovieDirectDrawMediaStream != 0) {
                                            ((ComUnknown *)(g_MovieDirectDrawMediaStream))->Release();
                                            g_MovieDirectDrawMediaStream = 0;
                                        }
                                        if (g_MoviePrimaryVideoStream != 0) {
                                            ((ComUnknown *)(g_MoviePrimaryVideoStream))->Release();
                                            g_MoviePrimaryVideoStream = 0;
                                        }
                                        if (g_MovieAMMultiMediaStream != 0) {
                                            ((ComUnknown *)(g_MovieAMMultiMediaStream))->Release();
                                            g_MovieAMMultiMediaStream = 0;
                                        }
                                        // STRING: FF7 0x007bffe0
                                        Trace_Printf("Failed to create movie sample (0x%8.8X)\n", res);
                                        return 0;
                                    } else if (g_Movie_useD3D != 0) {
                                        g_Movie_textureDesc.flags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
                                        g_Movie_textureDesc.width = 0x100;
                                        g_Movie_textureDesc.height = 0x100;
                                        g_Movie_textureDesc.caps = DDSCAPS_TEXTURE | DDSCAPS_SYSTEMMEMORY;
                                        res = ((ComDirectDraw *)(pDirectDraw2))->CreateSurface(&g_Movie_textureDesc, &g_Movie_pSts1, 0);
                                        if (res < 0) {
                                            // STRING: FF7 0x007c000c
                                            Trace_Printf("Failed to create sts1 (0x%8.8X)\n", res);
                                            return 0;
                                        }
                                        g_Movie_textureDesc.width = 0x40;
                                        res = ((ComDirectDraw *)(pDirectDraw2))->CreateSurface(&g_Movie_textureDesc, &g_Movie_pSts2, 0);
                                        if (res < 0) {
                                            // STRING: FF7 0x007c0030
                                            Trace_Printf("Failed to create sts2 (0x%8.8X)\n", res);
                                            return 0;
                                        }
                                        g_Movie_textureDesc.caps = DDSCAPS_ALLOCONLOAD | DDSCAPS_TEXTURE | DDSCAPS_VIDEOMEMORY;
                                        res = ((ComDirectDraw *)(pDirectDraw2))->CreateSurface(&g_Movie_textureDesc, &g_Movie_pVts2, 0);
                                        if (res < 0) {
                                            // STRING: FF7 0x007c0054
                                            Trace_Printf("Failed to create vts2 (0x%8.8X)\n", res);
                                            return 0;
                                        }
                                        g_Movie_textureDesc.width = 0x100;
                                        res = ((ComDirectDraw *)(pDirectDraw2))->CreateSurface(&g_Movie_textureDesc, &g_Movie_pVts1, 0);
                                        if (res < 0) {
                                            // STRING: FF7 0x007c0078
                                            Trace_Printf("Failed to create vts1 (0x%8.8X)\n", res);
                                            return 0;
                                        }
                                        res = ((ComUnknown *)(g_Movie_pSts1))->QueryInterface(&IID_IDirect3DTexture2, &g_Movie_pTexSts1);
                                        if (res < 0) {
                                            // STRING: FF7 0x007c009c
                                            Trace_Printf("Failed to create st1 (0x%8.8X)\n", res);
                                            return 0;
                                        }
                                        res = ((ComUnknown *)(g_Movie_pSts2))->QueryInterface(&IID_IDirect3DTexture2, &g_Movie_pTexSts2);
                                        if (res < 0) {
                                            // STRING: FF7 0x007c00bc
                                            Trace_Printf("Failed to create st2 (0x%8.8X)\n", res);
                                            return 0;
                                        }
                                        res = ((ComUnknown *)(g_Movie_pVts1))->QueryInterface(&IID_IDirect3DTexture2, &g_Movie_pTexVts1);
                                        if (res < 0) {
                                            // STRING: FF7 0x007c00dc
                                            Trace_Printf("Failed to create vt1 (0x%8.8X)\n", res);
                                            return 0;
                                        }
                                        res = ((ComUnknown *)(g_Movie_pVts2))->QueryInterface(&IID_IDirect3DTexture2, &g_Movie_pTexVts2);
                                        if (res < 0) {
                                            // STRING: FF7 0x007c00fc
                                            Trace_Printf("Failed to create vt2 (0x%8.8X)\n", res);
                                            return 0;
                                        }
                                        device = state->d3dDevice2;
                                        res = ((ComDirect3DTexture2 *)(g_Movie_pTexVts1))->GetHandle((int)device, &g_Movie_hTex1);
                                        if (res < 0) {
                                            // STRING: FF7 0x007c011c
                                            Trace_Printf("Failed to get th1 (0x%8.8X)\n", res);
                                            return 0;
                                        }
                                        res = ((ComDirect3DTexture2 *)(g_Movie_pTexVts2))->GetHandle((int)device, &g_Movie_hTex2);
                                        if (res < 0) {
                                            // STRING: FF7 0x007c013c
                                            Trace_Printf("Failed to get th2 (0x%8.8X)\n", res);
                                            return 0;
                                        }
                                    }
                                    {
                                        res = ((ComAMMultiMediaStream *)(g_MovieAMMultiMediaStream))->GetFilterGraph(&g_Movie_pGraphBuilder);
                                        if (res < 0) {
                                            // STRING: FF7 0x007c015c
                                            Trace_Printf("Failed to get FilterGraph (0x%8.8X)\n", res);
                                            return 0;
                                        } else {
                                            res = ((ComUnknown *)(g_Movie_pGraphBuilder))->QueryInterface(&IID_IMediaSeeking, &g_MovieMediaSeeking);
                                            if (res < 0) {
                                                // STRING: FF7 0x007c0184
                                                Trace_Printf("Failed to get MediaSeeking(0x%8.8X)\n", res);
                                                if (g_Movie_pGraphBuilder != 0) {
                                                    ((ComUnknown *)(g_Movie_pGraphBuilder))->Release();
                                                    g_Movie_pGraphBuilder = 0;
                                                }
                                                return 0;
                                            } else {
                                                res = ((ComMediaSeeking *)(g_MovieMediaSeeking))->SetTimeFormat(&TIME_FORMAT_FRAME);
                                                if (res < 0) {
                                                    // STRING: FF7 0x007c01ac
                                                    Trace_Printf("Failed to set time format (0x%8.8X)\n", res);
                                                    if (g_MovieMediaSeeking != 0) {
                                                        ((ComUnknown *)(g_MovieMediaSeeking))->Release();
                                                        g_MovieMediaSeeking = 0;
                                                    }
                                                    if (g_Movie_pGraphBuilder != 0) {
                                                        ((ComUnknown *)(g_Movie_pGraphBuilder))->Release();
                                                        g_Movie_pGraphBuilder = 0;
                                                    }
                                                    return 0;
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    g_Movie_isGraphOpen = 1;
    return 1;
}

/*
 * Closes the movie `Movie_Open` opened: clears `g_Movie_isGraphOpen`, stops playback
 * (`Movie_Stop`), and releases every COM object of it. With Direct3D (`g_Movie_useD3D`) these are
 * first the four texture interfaces and the four texture surfaces (the 256- and 64-pixel-wide
 * system-memory and video-memory pairs), then for all drivers the filter graph, the
 * `IMediaSeeking` interface, the stream sample, the movie surface, the multimedia stream, the
 * primary video stream and its DirectDraw media stream; each pointer is set to 0. It does nothing
 * under the OpenGL driver or when no stream sample exists. The credits, field movies, the main
 * dispatcher and `App_Shutdown` call it once a movie has ended or is abandoned.
 *
 * The trailing clear of the texture surfaces writes `g_Movie_pVts1` twice and never
 * `g_Movie_pVts2`; harmless, since each released pointer was already set to 0.
 */
// FUNCTION: FF7 0x00417759
void __cdecl Movie_ReleaseGraph(void)
{

    if (GetEngineContext()->driverType == GFX_DRIVER_OPENGL) {
        return;
    }
    if (g_Movie_pStreamSample != 0) {
        g_Movie_isGraphOpen = 0;
        Movie_Stop();
        if (g_Movie_useD3D != 0) {
            if (g_Movie_pTexSts1 != 0) {
                ((ComUnknown *)(g_Movie_pTexSts1))->Release();
                g_Movie_pTexSts1 = 0;
            }
            if (g_Movie_pTexSts2 != 0) {
                ((ComUnknown *)(g_Movie_pTexSts2))->Release();
                g_Movie_pTexSts2 = 0;
            }
            if (g_Movie_pTexVts1 != 0) {
                ((ComUnknown *)(g_Movie_pTexVts1))->Release();
                g_Movie_pTexVts1 = 0;
            }
            if (g_Movie_pTexVts2 != 0) {
                ((ComUnknown *)(g_Movie_pTexVts2))->Release();
                g_Movie_pTexVts2 = 0;
            }
            if (g_Movie_pSts1 != 0) {
                ((ComUnknown *)(g_Movie_pSts1))->Release();
                g_Movie_pSts1 = 0;
            }
            if (g_Movie_pSts2 != 0) {
                ((ComUnknown *)(g_Movie_pSts2))->Release();
                g_Movie_pSts2 = 0;
            }
            if (g_Movie_pVts1 != 0) {
                ((ComUnknown *)(g_Movie_pVts1))->Release();
                g_Movie_pVts1 = 0;
            }
            if (g_Movie_pVts2 != 0) {
                ((ComUnknown *)(g_Movie_pVts2))->Release();
                g_Movie_pVts2 = 0;
            }
            g_Movie_pTexSts1 = 0;
            g_Movie_pTexSts2 = 0;
            g_Movie_pTexVts1 = 0;
            g_Movie_pTexVts2 = 0;
            g_Movie_pSts1 = 0;
            g_Movie_pSts2 = 0;
            g_Movie_pVts1 = 0;
            g_Movie_pVts1 = 0;
        }
        if (g_Movie_pGraphBuilder != 0) {
            ((ComUnknown *)(g_Movie_pGraphBuilder))->Release();
            g_Movie_pGraphBuilder = 0;
        }
        if (g_MovieMediaSeeking != 0) {
            ((ComUnknown *)(g_MovieMediaSeeking))->Release();
            g_MovieMediaSeeking = 0;
        }
        g_Movie_pGraphBuilder = 0;
        g_MovieMediaSeeking = 0;
        if (g_Movie_pStreamSample != 0) {
            ((ComUnknown *)(g_Movie_pStreamSample))->Release();
            g_Movie_pStreamSample = 0;
        }
        if (g_Movie_pAmSurface != 0) {
            ((ComUnknown *)(g_Movie_pAmSurface))->Release();
            g_Movie_pAmSurface = 0;
        }
        if (g_MovieAMMultiMediaStream != 0) {
            ((ComUnknown *)(g_MovieAMMultiMediaStream))->Release();
            g_MovieAMMultiMediaStream = 0;
        }
        g_Movie_pStreamSample = 0;
        g_Movie_pAmSurface = 0;
        g_MovieAMMultiMediaStream = 0;
        if (g_MoviePrimaryVideoStream != 0) {
            ((ComUnknown *)(g_MoviePrimaryVideoStream))->Release();
            g_MoviePrimaryVideoStream = 0;
        }
        if (g_MovieDirectDrawMediaStream != 0) {
            ((ComUnknown *)(g_MovieDirectDrawMediaStream))->Release();
            g_MovieDirectDrawMediaStream = 0;
        }
        g_MoviePrimaryVideoStream = 0;
        g_MovieDirectDrawMediaStream = 0;
    }
}

/*
 * Restores the movie surface the stream decodes into after the window was switched away from:
 * when the surface reports DDERR_SURFACELOST, `Restore` is called. Returns 0 when the restore
 * fails (logged), otherwise 1, also when no movie surface exists. `App_OnActivateApp` calls it on
 * reactivation and prints "Could not restore movie surface" on 0.
 */
// FUNCTION: FF7 0x00417a4c
int __cdecl Movie_RestoreSurface(void)
{
    int status = 0;
    if (g_Movie_pAmSurface != 0) {
        if (((ComDirectDrawSurface *)g_Movie_pAmSurface)->IsLost() == DDERR_SURFACELOST) {
            status = ((ComDirectDrawSurface *)g_Movie_pAmSurface)->Restore();
            if (status < 0) {
                // STRING: FF7 0x007c01d4
                Trace_Printf("Failed to restore movie surface (0x%8.8X)\n", status);
                return 0;
            }
        }
    }
    return 1;
}

/*
 * Starts the movie `Movie_Open` opened. When no movie is playing and a stream sample exists, it
 * sets the multimedia stream running (`SetState` 1, run), sets `g_Movie_isPlaying`, asks the
 * sample for its first frame (`Update` 0, synchronous) and waits up to 2000 ms for it
 * (`CompletionStatus` 2, wait). The callers then put a frame on screen every frame with
 * `MovieBltFrameToBackBuffer`.
 *
 * Returns 1 when the movie started, or when it already plays or nothing is open; 0 when a stream
 * call fails (logged), with `g_Movie_isPlaying` left set if the failure came after the stream
 * started. The engine context's Direct3D device (`d3dDevice2`) is read and not used.
 */
// FUNCTION: FF7 0x00417ab0
int __cdecl Movie_Run(void)
{
    struct {
        int unused0;
        int unused1;
        int status;
        GfxEngineContext *context;
        int value;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    frame.unused0 = 0;
    frame.unused1 = 0;
    frame.status = 0;
    frame.value = 0;
    frame.context = GetEngineContext();
    frame.value = frame.context->d3dDevice2;
    if (g_Movie_isPlaying == 0 && g_Movie_pStreamSample != 0) {
        frame.status = ((ComAMMultiMediaStream *)g_MovieAMMultiMediaStream)->SetState(1);
        if (frame.status < 0) {
            // STRING: FF7 0x007c0200
            Trace_Printf("Failed to start movie (0x%8.8X)\n", frame.status);
            return 0;
        }
        g_Movie_isPlaying = 1;
        frame.status = ((ComStreamSample *)g_Movie_pStreamSample)->Update(0, 0, 0, 0);
        if (frame.status < 0) {
            // STRING: FF7 0x007c0224
            Trace_Printf("AMStart: Failed to update movie sample (0x%8.8X)\n", frame.status);
            return 0;
        }
        frame.status = ((ComStreamSample *)g_Movie_pStreamSample)->CompletionStatus(2, 0x7D0);
        if (frame.status < 0) {
            // STRING: FF7 0x007c0258
            Trace_Printf("AMStart: Failed to update first sample (0x%8.8X)\n", frame.status);
            return 0;
        }
    }
    return 1;
}

/*
 * Stops the playing movie: waits up to 100 ms for a pending sample update (`CompletionStatus` 2,
 * wait), sets the multimedia stream to stopped (`SetState` 0), and clears `g_Movie_isPlaying` and
 * `g_Movie_isStopRequested`. The main dispatcher calls it when `Movie_IsStopRequested` reports the
 * end of the movie, `App_OnActivateApp` when the window is deactivated, and `Movie_ReleaseGraph`
 * before releasing. Nothing happens when no movie plays.
 *
 * Returns 1, or 0 when `SetState` fails (logged); then the flags stay set. The engine context's
 * Direct3D device (`d3dDevice2`) is read and not used.
 */
// FUNCTION: FF7 0x00417bb2
int __cdecl Movie_Stop(void)
{
    struct {
        int status;
        GfxEngineContext *context;
        int value;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    frame.status = 0;
    if (g_Movie_isPlaying != 0) {
        frame.context = GetEngineContext();
        frame.status = ((ComStreamSample *)g_Movie_pStreamSample)->CompletionStatus(2, 100);
        frame.status = ((ComAMMultiMediaStream *)g_MovieAMMultiMediaStream)->SetState(0);
        if (frame.status < 0) {
            // STRING: FF7 0x007c028c
            Trace_Printf("Failed to stop movie (0x%8.8X)\n", frame.status);
            return 0;
        }
        g_Movie_isPlaying = 0;
        g_Movie_isStopRequested = 0;
        frame.value = frame.context->d3dDevice2;
    }
    return 1;
}

/*
 * Blits the movie sample surface to the render target using DirectDraw Blt.
 */
// FUNCTION: FF7 0x00417c42
int __cdecl Movie_BltSampleSurface(int pTargetSurface)
{
    struct {
        int streamTime[2];
        int center;
        GfxTextureSurfaceDesc desc;
        int height;
        int width;
        int dimensions[2];
        int attempts;
        int status;
        GfxEngineContext *context;
        int ok;
    } frame;

    frame.ok = 1;
    frame.status = 0;
    frame.context = GetEngineContext();
    Blit_CopyContextSurfaceSize(&frame.dimensions[0], frame.context);
    frame.status = ((ComStreamSample *)(g_Movie_pStreamSample))->CompletionStatus(3, 0xA);
    if (frame.status == MOVIE_STATUS_END_OF_STREAM) {
        if (g_Movie_loopFlag != 0) {
            ((ComAMMultiMediaStream *)(g_MovieAMMultiMediaStream))->Seek(0, 0);
        } else {
            g_Movie_isStopRequested = 1;
        }
    }
    if (frame.status == MOVIE_STATUS_PENDING) {
        // STRING: FF7 0x007c02ac
        Trace_Printf("Waiting for frame to update...\n");
        frame.ok = 0;
    }
    if (frame.status < 0) {
        // STRING: FF7 0x007c02cc
        Trace_Printf("Error waiting for sample update (0x%8.8X)\n", frame.status);
        return 0;
    } else if (frame.status >= 0) {
        frame.height = g_Movie_textureDesc.height;
        frame.width = g_Movie_textureDesc.width;
        memset(&frame.desc, 0, sizeof(frame.desc));
        frame.desc.size = sizeof(frame.desc);
        frame.desc.size = sizeof(frame.desc);
        frame.desc.pixelFormatSize = 0x20;
        do {
            frame.status = ((ComDirectDrawSurface *)(g_Movie_pAmSurface))->Lock(0, &frame.desc, 1, 0);
            if (frame.status < 0) {
                // STRING: FF7 0x007c02f8
                Trace_Printf("amsurf is busy\n", frame.status);
            }
        } while (frame.status == DDERR_SURFACEBUSY);
        frame.height = frame.desc.height;
        frame.width = frame.desc.width;
        if (g_Movie_uiScalingMode == 2) {
            Gfx_CopySurfaceRectDoubled16(frame.width, frame.height, 0, 0, frame.desc.pitch, (int)frame.desc.surface, 0, 0, frame.dimensions[0], frame.dimensions[1]);
        } else if (g_Movie_uiScalingMode == 1) {
            frame.center = (0x1E0 - frame.height) / 2;
            Gfx_CopySurfaceRectDwords(frame.width, frame.height, 0, 0, frame.desc.pitch, (int)frame.desc.surface, 0xA0, 0x78, frame.dimensions[0], frame.dimensions[1]);
        } else if (g_Movie_uiScalingMode == 0) {
            Gfx_CopySurfaceRectDwords(frame.width, frame.height, 0, 0, frame.desc.pitch, (int)frame.desc.surface, 0, 0, frame.dimensions[0], frame.dimensions[1]);
        }
        frame.status = ((ComDirectDrawSurface *)(g_Movie_pAmSurface))->Unlock((int)frame.desc.surface);
    }
    if (frame.ok != 0) {
        if (DEAD_IN_RETAIL != 0) {
            frame.status = ((ComMediaSeeking *)(g_MovieMediaSeeking))->GetCurrentPosition(&frame.streamTime[0]);
            frame.status = ((ComMediaSeeking *)(g_MovieMediaSeeking))->SetPositions(&frame.streamTime[0], 1, 0, 0);
        }
        for (frame.attempts = 0; frame.attempts < 0xA; frame.attempts++) {
            if (frame.context->fps < 15.0 && Movie_GetCurrentFrame() > 0x32 && g_Movie_allowFrameDrop != 0) {
                frame.status = ((ComStreamSample *)(g_Movie_pStreamSample))->Update(3, 0, 0, 0);
            } else {
                frame.status = ((ComStreamSample *)(g_Movie_pStreamSample))->Update(0, 0, 0, 0);
            }
            if (frame.status < 0) {
                // STRING: FF7 0x007c0308
                Trace_Printf("AMBlt: Failed to update movie sample (0x%8.8X): %d\n", frame.status, frame.attempts);
                return 0;
            }
            break;
        }
    }
    return 1;
}

/*
 * Puts the playing movie's current frame on `pTargetSurface` (the back buffer) and requests the
 * next one; the main dispatcher, the credits and `Field_RenderModels` call it once per frame. It
 * first clears `g_Movie_isStopRequested`, and does nothing (returns 0) under the OpenGL driver or
 * when no movie plays. Under the software driver it hands the frame to `Movie_BltSampleSurface`
 * and returns 0.
 *
 * Otherwise it polls the sample for 10 ms (`CompletionStatus` 3: no-update-ok, wait). At end of
 * stream (`MOVIE_STATUS_END_OF_STREAM`) a looping movie seeks back to 0 and any other sets
 * `g_Movie_isStopRequested`, which the caller reads through `Movie_IsStopRequested`. Pending
 * (`MOVIE_STATUS_PENDING`) means no new frame: the old one is shown again and no update is
 * requested. Then the frame goes to the target one of three ways:
 * - Direct3D (`g_Movie_useD3D`): a new frame's rows are copied from the movie surface into two
 *   system-memory textures, 512 bytes (the left 256 pixels) into `sts1` and the next 128 bytes
 *   (64 pixels) into `sts2`, loaded into their video-memory textures, and drawn as two
 *   pre-transformed triangle fans (`g_Movie_quadVertices1`/`2`, picked by `Movie_Open` for the UI
 *   scaling mode and the movie height) with texture wrap and linear filtering off; wrap is turned
 *   back on, filtering is not;
 * - DirectDraw with scaling (`g_Movie_scalingEnabled`): `Blt` stretches the movie surface into
 *   the centred target rectangle;
 * - DirectDraw without scaling: `BltFast` copies it unscaled to the target rectangle's corner.
 * After a new frame it requests the next with `Update` 0 (synchronous), or with 3 (asynchronous,
 * continuous) when the game runs under 15 frames per second (`fps` of the engine context), the movie is past
 * frame 25 and `Movie_SetFrameDropForMovie` allowed dropping frames for it. The retry loop
 * around it always ends after the first attempt.
 *
 * Returns 1, or 0 on a failed DirectDraw or stream call (logged). Defect: the Direct3D path
 * returns at once when a lock or unlock of `sts1` or `sts2` fails, leaving the movie surface
 * locked.
 */
// FUNCTION: FF7 0x00417f39
int __cdecl MovieBltFrameToBackBuffer(int pTargetSurface)
{
    struct {
        int streamTime[2];
        GfxTextureSurfaceDesc targetDesc;
        unsigned char *destination;
        GfxTextureSurfaceDesc sourceDesc;
        unsigned char *source;
        int row;
        int height;
        int attempts;
        int status;
        GfxEngineContext *context;
        int device;
        int ok;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    frame.ok = 1;
    frame.status = 0;
    g_Movie_isStopRequested = 0;
    if (GetEngineContext()->driverType == GFX_DRIVER_OPENGL) {
        return 0;
    } else if (g_Movie_isPlaying == 0 || g_Movie_pStreamSample == 0) {
        return 0;
    } else {
        frame.context = GetEngineContext();
        if (frame.context->driverType == GFX_DRIVER_SOFTWARE) {
            Movie_BltSampleSurface(pTargetSurface);
            return 0;
        } else {
            frame.status = ((ComStreamSample *)(g_Movie_pStreamSample))->CompletionStatus(3, 0xA);
            if (frame.status == MOVIE_STATUS_END_OF_STREAM) {
                if (g_Movie_loopFlag != 0) {
                    ((ComAMMultiMediaStream *)(g_MovieAMMultiMediaStream))->Seek(0, 0);
                } else {
                    g_Movie_isStopRequested = 1;
                }
            }
            if (frame.status == MOVIE_STATUS_PENDING) {
                // STRING: FF7 0x007c033c
                Trace_Printf("Waiting for frame to update...\n");
                frame.ok = 0;
            }
            if (frame.status < 0) {
                // STRING: FF7 0x007c035c
                Trace_Printf("Error waiting for sample update (0x%8.8X)\n", frame.status);
                return 0;
            } else if (frame.status >= 0) {
                if (g_Movie_useD3D == 0 && g_Movie_scalingEnabled != 0) {
                    ((ComDirectDrawSurface *)(pTargetSurface))->Blt(&g_Movie_targetRect_left, g_Movie_pAmSurface, 0, 0x1000000, 0);
                } else if (g_Movie_useD3D != 0) {
                    if (frame.ok != 0) {
                        frame.sourceDesc.size = sizeof(frame.sourceDesc);
                        frame.targetDesc.size = sizeof(frame.targetDesc);
                        do {
                            frame.status = ((ComDirectDrawSurface *)(g_Movie_pAmSurface))->Lock(0, &frame.sourceDesc, 1, 0);
                            if (frame.status < 0) {
                                // STRING: FF7 0x007c0388
                                Trace_Printf("amsurf is busy\n", frame.status);
                            }
                        } while (frame.status == DDERR_SURFACEBUSY);
                        if (frame.status < 0) {
                            // STRING: FF7 0x007c0398
                            Trace_Printf("Failed to lock amsurf (0x%8.8X)\n", frame.status);
                            return 0;
                        }
                        frame.status = ((ComDirectDrawSurface *)(g_Movie_pSts1))->Lock(0, &frame.targetDesc, 1, 0);
                        if (frame.status < 0) {
                            // STRING: FF7 0x007c03bc
                            Trace_Printf("Failed to lock sts1 (0x%8.8X)\n", frame.status);
                            return 0;
                        }
                        frame.height = frame.sourceDesc.height;
                        frame.source = (unsigned char *)frame.sourceDesc.surface;
                        frame.destination = (unsigned char *)frame.targetDesc.surface;
                        for (frame.row = 0; frame.row < frame.height; frame.row++) {
                            memcpy(frame.destination, frame.source, 512);
                            frame.source += frame.sourceDesc.pitch;
                            frame.destination += frame.targetDesc.pitch;
                        }
                        frame.status = ((ComDirectDrawSurface *)(g_Movie_pSts1))->Unlock((int)frame.targetDesc.surface);
                        if (frame.status < 0) {
                            // STRING: FF7 0x007c03dc
                            Trace_Printf("Failed to unlock sts1 (0x%8.8X)\n", frame.status);
                            return 0;
                        }
                        frame.status = ((ComDirectDrawSurface *)(g_Movie_pSts2))->Lock(0, &frame.targetDesc, 1, 0);
                        if (frame.status < 0) {
                            // STRING: FF7 0x007c0400
                            Trace_Printf("Failed to lock sts2 (0x%8.8X)\n", frame.status);
                            return 0;
                        }
                        frame.source = (unsigned char *)frame.sourceDesc.surface + 0x200;
                        frame.destination = (unsigned char *)frame.targetDesc.surface;
                        for (frame.row = 0; frame.row < frame.height; frame.row++) {
                            memcpy(frame.destination, frame.source, 128);
                            frame.source += frame.sourceDesc.pitch;
                            frame.destination += frame.targetDesc.pitch;
                        }
                        frame.status = ((ComDirectDrawSurface *)(g_Movie_pSts2))->Unlock((int)frame.targetDesc.surface);
                        if (frame.status < 0) {
                            // STRING: FF7 0x007c0420
                            Trace_Printf("Failed to unlock sts2 (0x%8.8X)\n", frame.status);
                            return 0;
                        }
                        frame.status = ((ComDirectDrawSurface *)(g_Movie_pAmSurface))->Unlock((int)frame.sourceDesc.surface);
                        if (frame.status < 0) {
                            // STRING: FF7 0x007c0444
                            Trace_Printf("Failed to unlock amsurf (0x%8.8X)\n", frame.status);
                            return 0;
                        }
                    }
                    if (frame.ok != 0) {
                        frame.status = ((ComDirect3DTexture2 *)(((int)((int)g_Movie_pTexVts1))))->Load(((int)((int)g_Movie_pTexSts1)));
                        if (frame.status < 0) {
                            // STRING: FF7 0x007c0468
                            Trace_Printf("Failed to load vt1 (0x%8.8X)\n", frame.status);
                            return 0;
                        }
                        frame.status = ((ComDirect3DTexture2 *)(((int)((int)g_Movie_pTexVts2))))->Load(((int)((int)g_Movie_pTexSts2)));
                        if (frame.status < 0) {
                            // STRING: FF7 0x007c0488
                            Trace_Printf("Failed to load vt2 (0x%8.8X)\n", frame.status);
                            return 0;
                        }
                    }
                    frame.device = frame.context->d3dDevice2;
                    Gfx_SetRenderStateCall(RENDER_STATE_TEXTURE_WRAP, 0, (struct GfxEngineContext *)frame.context);
                    Gfx_SetRenderStateCall(RENDER_STATE_LINEAR_FILTER, 0, (struct GfxEngineContext *)frame.context);
                    ((ComDirect3DDevice2 *)(frame.device))->SetRenderState(1, g_Movie_hTex1);
                    // STRING: FF7 0x007c04a8
                    frame.status = CheckDirectDrawResult(((ComDirect3DDevice2 *)(frame.device))->DrawPrimitive(6, 3, (int)&g_Movie_quadVertices1, 4, 0), "C:\\lib\\src\\movie\\movie.cpp", 0x7D1);
                    if (frame.status == D3DERR_INVALIDRAMPTEXTURE) {
                        // STRING: FF7 0x007c04c4
                        Trace_Printf("AMBlt: Invalid ramp texture.\n");
                    }
                    if (frame.status == MOVIE_E_INVALID_ARG) {
                        // STRING: FF7 0x007c04e4
                        Trace_Printf("AMBlt: Invalid params.\n");
                    }
                    if (frame.status < 0) {
                        // STRING: FF7 0x007c04fc
                        Trace_Printf("AMBlt: DrawPrimitive failed (0x%8.8X)\n", frame.status);
                    }
                    ((ComDirect3DDevice2 *)(frame.device))->SetRenderState(1, g_Movie_hTex2);
                    frame.status = ((ComDirect3DDevice2 *)(frame.device))->DrawPrimitive(6, 3, (int)&g_Movie_quadVertices2, 4, 0);
                    if (frame.status == D3DERR_INVALIDRAMPTEXTURE) {
                        // STRING: FF7 0x007c0524
                        Trace_Printf("AMBlt: Invalid ramp texture.\n");
                    }
                    if (frame.status == MOVIE_E_INVALID_ARG) {
                        // STRING: FF7 0x007c0544
                        Trace_Printf("AMBlt: Invalid params.\n");
                    }
                    if (frame.status < 0) {
                        // STRING: FF7 0x007c055c
                        Trace_Printf("AMBlt: DrawPrimitive failed (0x%8.8X)\n", frame.status);
                    }
                    Gfx_SetRenderStateCall(RENDER_STATE_TEXTURE_WRAP, 1, (struct GfxEngineContext *)frame.context);
                } else {
                    ((ComDirectDrawSurface *)(pTargetSurface))->BltFast(g_Movie_targetRect_left, g_Movie_targetRect_top, g_Movie_pAmSurface, 0, 0x10);
                }
            }
            if (frame.ok != 0) {
                if (DEAD_IN_RETAIL != 0) {
                    frame.status = ((ComMediaSeeking *)(g_MovieMediaSeeking))->GetCurrentPosition(&frame.streamTime[0]);
                    frame.status = ((ComMediaSeeking *)(g_MovieMediaSeeking))->SetPositions(&frame.streamTime[0], 1, 0, 0);
                }
                for (frame.attempts = 0; frame.attempts < 0xA; frame.attempts++) {
                    if (frame.context->fps < 15.0 && Movie_GetCurrentFrame() > 0x19 && g_Movie_allowFrameDrop != 0) {
                        frame.status = ((ComStreamSample *)(g_Movie_pStreamSample))->Update(3, 0, 0, 0);
                    } else {
                        frame.status = ((ComStreamSample *)(g_Movie_pStreamSample))->Update(0, 0, 0, 0);
                    }
                    if (frame.status < 0) {
                        // STRING: FF7 0x007c0584
                        Trace_Printf("AMBlt: Failed to update movie sample (0x%8.8X): %d\n", frame.status, frame.attempts);
                        return 0;
                    }
                    break;
                }
            }
        }
    }
    return 1;
}
// DirectShow stream probe helper following movie_directdraw_d3d_setup.cpp in link order.
extern const unsigned char IID_IMultiMediaStream[16];

/*
 * Probes multimedia stream capabilities via IMultiMediaStream interface.
 */
// FUNCTION: FF7 0x004185af
int __cdecl Movie_ProbeMultiMediaStreamInfo(int unused, int pMediaStream)
{
    struct {
        int status;
        int outputB;
        int queried;
        int outputA;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    frame.status = 0;
    frame.status = ((ComUnknown *)pMediaStream)->QueryInterface(&IID_IMultiMediaStream[0], &frame.queried);
    if (frame.status < 0)
        return 0;
    ((ComMultiMediaStream *)frame.queried)->GetInformation(&frame.outputA, &frame.outputB);
    if (frame.queried != 0) {
        ((ComUnknown *)frame.queried)->Release();
        frame.queried = 0;
    }
    return 1;
}

// Returns zero unless g_Movie_isPlaying and g_Movie_pStreamSample are set; queries movie-status slot twelve into
// a zero-initialized output, returns that output on nonnegative status, and on negative status
// releases/clears g_MovieMediaSeeking and g_Movie_pGraphBuilder then returns zero.
// FUNCTION: FF7 0x00418613
int __cdecl Movie_GetCurrentFrame(void)
{
    struct {
        int status;
        int output[2];
    } frame;
    frame.output[0] = 0;
    frame.output[1] = 0;
    frame.status = 0;
    if (g_Movie_isPlaying == 0 || g_Movie_pStreamSample == 0)
        return frame.output[0];
    frame.status = ((ComMediaSeeking *)g_MovieMediaSeeking)->GetCurrentPosition(&frame.output[0]);
    if (frame.status < 0) {
        if (g_MovieMediaSeeking != 0) {
            g_MovieMediaSeeking->Release();
            g_MovieMediaSeeking = 0;
        }
        if (g_Movie_pGraphBuilder != 0) {
            g_Movie_pGraphBuilder->Release();
            g_Movie_pGraphBuilder = 0;
        }
        return 0;
    }
    return frame.output[0];
}

// Returns 1 when g_Movie_isPlaying is zero and 0 otherwise; a one-line predicate on that global, which
// Movie_ResetPlaybackState clears and Movie_GetDuration gates on.
// FUNCTION: FF7 0x004186b6
int __cdecl Movie_IsNotPlaying(void)
{
    return g_Movie_isPlaying == 0;
}

// Returns 1 when the movie has reached its end and should be stopped: g_Movie_isStopRequested,
// which MovieBltFrameToBackBuffer and Movie_BltSampleSurface set at end of stream for a movie
// that does not loop. MovieBltFrameToBackBuffer clears it again at the start of each call, and
// Movie_Stop and Movie_ResetPlaybackState clear it, so callers test it right after the frame's
// blit: the main dispatcher then calls Movie_Stop, the credits Movie_ReleaseGraph and their next
// sequence state, and the field movie code ends the movie.
// FUNCTION: FF7 0x004186c7
int __cdecl Movie_IsStopRequested(void)
{
    return g_Movie_isStopRequested;
}

// Returns 1 while a movie is open: g_Movie_isGraphOpen, which a successful Movie_Open sets and
// Movie_ReleaseGraph and Movie_ResetPlaybackState clear. The field movie code opens a movie only
// when none is open; the main dispatcher releases an open one before Continue loads the game.
// FUNCTION: FF7 0x004186d1
int __cdecl Movie_IsGraphOpen(void)
{
    return g_Movie_isGraphOpen;
}
} /* extern "C" */
