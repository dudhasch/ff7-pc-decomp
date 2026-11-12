// src/lib/src/graphics/directx.cpp
// Original: C:\lib\src\graphics\directx.cpp
// Contents: Creates and destroys the engine context and game window, enumerates DirectDraw and
//   Direct3D drivers and devices, and manages surfaces and engine error reporting.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/api/file.h"
#include "lib/codegen.h"
#include "lib/api/graphics.h"
#include "lib/api/list.h"
#include "lib/api/mem.h"
#include "lib/api/stack.h"
#include "lib/types/graphics.h"
#include "lib/types/stack.h"
#include "lib/types/list.h"
#include "lib/types/polygon.h"
#include "lib/enums/graphics.h"
#include "lib/enums/win32.h"
#include <direct.h>
#include "lib/crt.h"
#include "lib/com_abi.h"

struct GfxEngineContext;
struct GfxDirectDrawDriverCaps;

extern "C" {
// clang-format off
// GLOBAL: FF7 0x0090d268
int g_GfxBitDepths[5] = {16, 32, 8, 24, 0};
// directx.cpp's literal stream in retail order: the code literals up to InitGraphicsDevice's 0x818 check
// (0x0090D288..0x0090DB03), then "HW" and "SW", g_GfxDriverTypeNames's initializer literals, then the rest. Each literal
// ahead of "HW" is returned by a static __inline function, a parse-time unit ahead of the table; under /Ob1 the call
// expands to the plain operand.
static __inline char *DirectXLit_AppLog(void)
{
    // STRING: FF7 0x0090d288
    return "APP.LOG";
}
static __inline char *DirectXLit_SourcePath_1(void)
{
    // STRING: FF7 0x0090d290
    return "C:\\lib\\src\\graphics\\directx.cpp";
}
static __inline char *DirectXLit_PercentS(void)
{
    // STRING: FF7 0x0090d2b0
    return "%s";
}
static __inline char *DirectXLit_PathJoin(void)
{
    // STRING: FF7 0x0090d2b4
    return "%s/%s";
}
static __inline char *DirectXLit_SourcePath_2(void)
{
    // STRING: FF7 0x0090d2bc
    return "C:\\lib\\src\\graphics\\directx.cpp";
}
static __inline char *DirectXLit_WmClose(void)
{
    // STRING: FF7 0x0090d2dc
    return "WM_CLOSE \n";
}
static __inline char *DirectXLit_WmQuit(void)
{
    // STRING: FF7 0x0090d2e8
    return "WM_QUIT \n";
}
static __inline char *DirectXLit_RequestedBitsPerPixel(void)
{
    // STRING: FF7 0x0090d2f4
    return "Requested bits per pixel (%d) doesn't match desktop bits per pixel (%d)";
}
static __inline char *DirectXLit_Error(void)
{
    // STRING: FF7 0x0090d33c
    return "ERROR";
}
static __inline char *DirectXLit_LineS(void)
{
    // STRING: FF7 0x0090d344
    return "%s\n";
}
static __inline char *DirectXLit_IndentedLineS(void)
{
    // STRING: FF7 0x0090d348
    return "  %s\n";
}
static __inline char *DirectXLit_Default(void)
{
    // STRING: FF7 0x0090d350
    return "DEFAULT";
}
static __inline char *DirectXLit_DefaultClass(void)
{
    // STRING: FF7 0x0090d358
    return "DEFAULT CLASS";
}
static __inline char *DirectXLit_SourcePath_3(void)
{
    // STRING: FF7 0x0090d368
    return "C:\\lib\\src\\graphics\\directx.cpp";
}
static __inline char *DirectXLit_SourcePath_4(void)
{
    // STRING: FF7 0x0090d388
    return "C:\\lib\\src\\graphics\\directx.cpp";
}
static __inline char *DirectXLit_SourcePath_5(void)
{
    // STRING: FF7 0x0090d3a8
    return "C:\\lib\\src\\graphics\\directx.cpp";
}
static __inline char *DirectXLit_SourcePath_6(void)
{
    // STRING: FF7 0x0090d3c8
    return "C:\\lib\\src\\graphics\\directx.cpp";
}
static __inline char *DirectXLit_SourcePath_7(void)
{
    // STRING: FF7 0x0090d3e8
    return "C:\\lib\\src\\graphics\\directx.cpp";
}
static __inline char *DirectXLit_SourcePath_8(void)
{
    // STRING: FF7 0x0090d408
    return "C:\\lib\\src\\graphics\\directx.cpp";
}
static __inline char *DirectXLit_LineSS(void)
{
    // STRING: FF7 0x0090d428
    return "%s%s\n";
}
static __inline char *DirectXLit_SourcePath_9(void)
{
    // STRING: FF7 0x0090d430
    return "C:\\lib\\src\\graphics\\directx.cpp";
}
static __inline char *DirectXLit_SourcePath_10(void)
{
    // STRING: FF7 0x0090d450
    return "C:\\lib\\src\\graphics\\directx.cpp";
}
static __inline char *DirectXLit_SourcePath_11(void)
{
    // STRING: FF7 0x0090d470
    return "C:\\lib\\src\\graphics\\directx.cpp";
}
static __inline char *DirectXLit_SourcePath_12(void)
{
    // STRING: FF7 0x0090d490
    return "C:\\lib\\src\\graphics\\directx.cpp";
}
static __inline char *DirectXLit_TwoSpaces_1(void)
{
    // STRING: FF7 0x0090d4b0
    return "  ";
}
static __inline char *DirectXLit_DdModeWH(void)
{
    // STRING: FF7 0x0090d4b4
    return "DD MODE: W %d H %d P %d RGB %d BPP %d \n";
}
static __inline char *DirectXLit_DisplayModeMatch(void)
{
    // STRING: FF7 0x0090d4dc
    return "DISPLAY MODE MATCH \n";
}
static __inline char *DirectXLit_SourcePath_13(void)
{
    // STRING: FF7 0x0090d4f4
    return "C:\\lib\\src\\graphics\\directx.cpp";
}
static __inline char *DirectXLit_SourcePath_14(void)
{
    // STRING: FF7 0x0090d514
    return "C:\\lib\\src\\graphics\\directx.cpp";
}
static __inline char *DirectXLit_SourcePath_15(void)
{
    // STRING: FF7 0x0090d534
    return "C:\\lib\\src\\graphics\\directx.cpp";
}
static __inline char *DirectXLit_SourcePath_16(void)
{
    // STRING: FF7 0x0090d554
    return "C:\\lib\\src\\graphics\\directx.cpp";
}
static __inline char *DirectXLit_SourcePath_17(void)
{
    // STRING: FF7 0x0090d574
    return "C:\\lib\\src\\graphics\\directx.cpp";
}
static __inline char *DirectXLit_SourcePath_18(void)
{
    // STRING: FF7 0x0090d594
    return "C:\\lib\\src\\graphics\\directx.cpp";
}
static __inline char *DirectXLit_TwoSpaces_2(void)
{
    // STRING: FF7 0x0090d5b4
    return "  ";
}
static __inline char *DirectXLit_SourcePath_19(void)
{
    // STRING: FF7 0x0090d5b8
    return "C:\\lib\\src\\graphics\\directx.cpp";
}
static __inline char *DirectXLit_SourcePath_20(void)
{
    // STRING: FF7 0x0090d5d8
    return "C:\\lib\\src\\graphics\\directx.cpp";
}
static __inline char *DirectXLit_SourcePath_21(void)
{
    // STRING: FF7 0x0090d5f8
    return "C:\\lib\\src\\graphics\\directx.cpp";
}
static __inline char *DirectXLit_SourcePath_22(void)
{
    // STRING: FF7 0x0090d618
    return "C:\\lib\\src\\graphics\\directx.cpp";
}
static __inline char *DirectXLit_SourcePath_23(void)
{
    // STRING: FF7 0x0090d638
    return "C:\\lib\\src\\graphics\\directx.cpp";
}
static __inline char *DirectXLit_PaletteBitsRgba(void)
{
    // STRING: FF7 0x0090d658
    return "  PALETTE %d BITS %d RGBA %d:%d:%d:%d \n";
}
static __inline char *DirectXLit_MatrixInitialize(void)
{
    // STRING: FF7 0x0090d680
    return "MATRIX INITIALIZE \n";
}
static __inline char *DirectXLit_SourcePath_24(void)
{
    // STRING: FF7 0x0090d694
    return "C:\\lib\\src\\graphics\\directx.cpp";
}
static __inline char *DirectXLit_InitializeDdD3dStart(void)
{
    // STRING: FF7 0x0090d6b4
    return "INITIALIZE DD/D3D START \n";
}
static __inline char *DirectXLit_SourcePath_25(void)
{
    // STRING: FF7 0x0090d6d0
    return "C:\\lib\\src\\graphics\\directx.cpp";
}
static __inline char *DirectXLit_SourcePath_26(void)
{
    // STRING: FF7 0x0090d6f0
    return "C:\\lib\\src\\graphics\\directx.cpp";
}
static __inline char *DirectXLit_SourcePath_27(void)
{
    // STRING: FF7 0x0090d710
    return "C:\\lib\\src\\graphics\\directx.cpp";
}
static __inline char *DirectXLit_SourcePath_28(void)
{
    // STRING: FF7 0x0090d730
    return "C:\\lib\\src\\graphics\\directx.cpp";
}
static __inline char *DirectXLit_SourcePath_29(void)
{
    // STRING: FF7 0x0090d750
    return "C:\\lib\\src\\graphics\\directx.cpp";
}
static __inline char *DirectXLit_SourcePath_30(void)
{
    // STRING: FF7 0x0090d770
    return "C:\\lib\\src\\graphics\\directx.cpp";
}
static __inline char *DirectXLit_SourcePath_31(void)
{
    // STRING: FF7 0x0090d790
    return "C:\\lib\\src\\graphics\\directx.cpp";
}
static __inline char *DirectXLit_SourcePath_32(void)
{
    // STRING: FF7 0x0090d7b0
    return "C:\\lib\\src\\graphics\\directx.cpp";
}
static __inline char *DirectXLit_Hal(void)
{
    // STRING: FF7 0x0090d7d0
    return "HAL \n";
}
static __inline char *DirectXLit_Hel(void)
{
    // STRING: FF7 0x0090d7d8
    return "HEL \n";
}
static __inline char *DirectXLit_SourcePath_33(void)
{
    // STRING: FF7 0x0090d7e0
    return "C:\\lib\\src\\graphics\\directx.cpp";
}
static __inline char *DirectXLit_DirectDraw2(void)
{
    // STRING: FF7 0x0090d800
    return "DIRECT DRAW 2 \n";
}
static __inline char *DirectXLit_SourcePath_34(void)
{
    // STRING: FF7 0x0090d810
    return "C:\\lib\\src\\graphics\\directx.cpp";
}
static __inline char *DirectXLit_TotalFree(void)
{
    // STRING: FF7 0x0090d830
    return "TOTAL %d FREE %d \n";
}
static __inline char *DirectXLit_SourcePath_35(void)
{
    // STRING: FF7 0x0090d844
    return "C:\\lib\\src\\graphics\\directx.cpp";
}
static __inline char *DirectXLit_SourcePath_36(void)
{
    // STRING: FF7 0x0090d864
    return "C:\\lib\\src\\graphics\\directx.cpp";
}
static __inline char *DirectXLit_SourcePath_37(void)
{
    // STRING: FF7 0x0090d884
    return "C:\\lib\\src\\graphics\\directx.cpp";
}
static __inline char *DirectXLit_NoRequestedDisplayModes(void)
{
    // STRING: FF7 0x0090d8a4
    return "NO REQUESTED DISPLAY MODES SUPPORTED \n";
}
static __inline char *DirectXLit_SourcePath_38(void)
{
    // STRING: FF7 0x0090d8cc
    return "C:\\lib\\src\\graphics\\directx.cpp";
}
static __inline char *DirectXLit_ExclusiveMode(void)
{
    // STRING: FF7 0x0090d8ec
    return "EXCLUSIVE MODE \n";
}
static __inline char *DirectXLit_SourcePath_39(void)
{
    // STRING: FF7 0x0090d900
    return "C:\\lib\\src\\graphics\\directx.cpp";
}
static __inline char *DirectXLit_SourcePath_40(void)
{
    // STRING: FF7 0x0090d920
    return "C:\\lib\\src\\graphics\\directx.cpp";
}
static __inline char *DirectXLit_DirectDrawNoHardware(void)
{
    // STRING: FF7 0x0090d940
    return "DIRECT DRAW: NO HARDWARE \n";
}
static __inline char *DirectXLit_SourcePath_41(void)
{
    // STRING: FF7 0x0090d95c
    return "C:\\lib\\src\\graphics\\directx.cpp";
}
static __inline char *DirectXLit_DdFrontSurface(void)
{
    // STRING: FF7 0x0090d97c
    return "DD FRONT SURFACE \n";
}
static __inline char *DirectXLit_SourcePath_42(void)
{
    // STRING: FF7 0x0090d990
    return "C:\\lib\\src\\graphics\\directx.cpp";
}
static __inline char *DirectXLit_SourcePath_43(void)
{
    // STRING: FF7 0x0090d9b0
    return "C:\\lib\\src\\graphics\\directx.cpp";
}
static __inline char *DirectXLit_DdBackSurface(void)
{
    // STRING: FF7 0x0090d9d0
    return "DD BACK SURFACE \n";
}
static __inline char *DirectXLit_SourcePath_44(void)
{
    // STRING: FF7 0x0090d9e4
    return "C:\\lib\\src\\graphics\\directx.cpp";
}
static __inline char *DirectXLit_SourcePath_45(void)
{
    // STRING: FF7 0x0090da04
    return "C:\\lib\\src\\graphics\\directx.cpp";
}
static __inline char *DirectXLit_SourcePath_46(void)
{
    // STRING: FF7 0x0090da24
    return "C:\\lib\\src\\graphics\\directx.cpp";
}
static __inline char *DirectXLit_SourcePath_47(void)
{
    // STRING: FF7 0x0090da44
    return "C:\\lib\\src\\graphics\\directx.cpp";
}
static __inline char *DirectXLit_SourcePath_48(void)
{
    // STRING: FF7 0x0090da64
    return "C:\\lib\\src\\graphics\\directx.cpp";
}
static __inline char *DirectXLit_SourcePath_49(void)
{
    // STRING: FF7 0x0090da84
    return "C:\\lib\\src\\graphics\\directx.cpp";
}
static __inline char *DirectXLit_SourcePath_50(void)
{
    // STRING: FF7 0x0090daa4
    return "C:\\lib\\src\\graphics\\directx.cpp";
}
static __inline char *DirectXLit_SourcePath_51(void)
{
    // STRING: FF7 0x0090dac4
    return "C:\\lib\\src\\graphics\\directx.cpp";
}
static __inline char *DirectXLit_SourcePath_52(void)
{
    // STRING: FF7 0x0090dae4
    return "C:\\lib\\src\\graphics\\directx.cpp";
}
// GLOBAL: FF7 0x0090d280
char *g_GfxDriverTypeNames[2] = {
    // STRING: FF7 0x0090db04
    "HW",
    // STRING: FF7 0x0090db08
    "SW"};
// GLOBAL: FF7 0x00db2af0
unsigned char g_GfxDriverCapsBuffer[200] = {0};
// GLOBAL: FF7 0x00db2bb8
struct GfxEngineContext *g_pGfxEngineContext = 0;
// GLOBAL: FF7 0x00db2bbc
unsigned char g_GfxPrimaryDriverFlag[4] = {0};
// The seam to file.cpp's .bss (D1: the left TU's): the shadow COMMON 0xDB2BC0 resolves to this definition (its marker
char g_DirectDrawEnumDevicePrefix[1] = {0};
unsigned char g_bssPad_00db2bc4[4] = {0};
// clang-format on
__declspec(dllimport) int __stdcall FreeLibrary(int);

#pragma intrinsic(memset)

__declspec(dllimport) int __stdcall MessageBoxA(int hWnd, char *text, char *caption, unsigned int type);
__declspec(dllimport) int __stdcall DefWindowProcA(int hWnd, unsigned int msg, unsigned int wParam, int lParam);
__declspec(dllimport) int __stdcall BeginPaint(int hWnd, void *ps);
__declspec(dllimport) int __stdcall EndPaint(int hWnd, void *ps);
__declspec(dllimport) short __stdcall GetAsyncKeyState(int vKey);
__declspec(dllimport) void __stdcall PostQuitMessage(int exitCode);
typedef struct GfxWindowClassRec {
    int e[10];
} GfxWindowClass;
__declspec(dllimport) int __stdcall LoadCursorA(int hInstance, int lpCursorName);
__declspec(dllimport) int __stdcall GetStockObject(int object);
__declspec(dllimport) unsigned short __stdcall RegisterClassA(void *wc);
__declspec(dllimport) int __stdcall AdjustWindowRect(int *rc, int style, int menu);
__declspec(dllimport) int __stdcall CreateWindowExA(int exStyle, int className, int windowName, int style, int x, int y, int w, int h, int parent, int menu, int inst, int param);
__declspec(dllimport) int __stdcall ShowCursor(int show);
__declspec(dllimport) int __stdcall ShowWindow(int hWnd, int cmd);
__declspec(dllimport) int __stdcall UpdateWindow(int hWnd);
__declspec(dllimport) int __stdcall GetDC(int hWnd);
__declspec(dllimport) int __stdcall GetDeviceCaps(int hdc, int index);
__declspec(dllimport) int __stdcall ReleaseDC(int hWnd, int hdc);
// Original caller supplies two DWORDs; the callee only reads the first.
extern void __cdecl Gfx_DestroyPresetStateTable(int arg1, int unused); // src/lib/src/graphics/dx_rendi.cpp

int __stdcall DirectDrawCreate(int, int, int);
int __stdcall DirectDrawEnumerateA(int, int);
// QueryInterface reads the complete 16-byte IID: SDK library GUIDs.
// GLOBAL: FF7 0x007b80a0
extern const unsigned char IID_IDirectDraw2[16];
// GLOBAL: FF7 0x007b82a0
extern const unsigned char IID_IDirect3D[16];

/*
 * Initializes application descriptor with zeroed fields and default size.
 */
// FUNCTION: FF7 0x006763f0
void __cdecl InitAppDescriptor(GfxAppDescriptor *descriptor)
{
    ZeroFillMemory32(8, descriptor);
    descriptor->debugLogEnabled = 1;
    descriptor->logFileName = (int)(DirectXLit_AppLog());
}

/*
 * Allocates and initializes the main engine context structure.
 */
// FUNCTION: FF7 0x00676416
void *__cdecl AllocEngineContext(GfxAppDescriptor *descriptor)
{
    GfxEngineContext *pEngine;
    char cwd[200];

    pEngine = (GfxEngineContext *)(GameHeapAlloc(1, 2720,

                           DirectXLit_SourcePath_1(), 56));
    if (pEngine != 0) {
        pEngine->activeA = 1;
        pEngine->activeB = 1;
        sprintf((char *)&g_GfxDriverCapsBuffer,

                DirectXLit_PercentS(), (char *)descriptor->logFileName);
        if (_getcwd(cwd, 200) != 0) {
            sprintf((char *)&g_GfxDriverCapsBuffer,

                    DirectXLit_PathJoin(), cwd, (char *)descriptor->logFileName);
        }
        if (descriptor != 0) {
            pEngine->activeB = descriptor->debugLogEnabled;
            if (descriptor->debugLogEnabled != 0) {
                pEngine->debugLogTarget = Gfx_CreateDebugLogTargetTruncating((int)&g_GfxDriverCapsBuffer);
            }
        }
        pEngine->tscFrequency = Timer_CalibrateTscFrequency(&pEngine->tscCalibration);
        pEngine->fov = 90.0f;
        pEngine->nearPlane = 5.0f;
        pEngine->farPlane = 2000.0f;
        pEngine->presentEnable = 1;
        pEngine->driverExportTable = (int)g_GfxDllDriverExportTable;
        pEngine->configApplied = 0;
    }
    return (void *)pEngine;
}

/*
 * Creates engine context using default descriptor parameters.
 */
// FUNCTION: FF7 0x00676549
void __cdecl CreateEngineContextFromDefaultDescriptor(void)
{
    int val[2];

    InitAppDescriptor((struct GfxAppDescriptor *)val);
    AllocEngineContext((GfxAppDescriptor *)val);
}

/*
 * Sets the active global engine context pointer.
 */
// FUNCTION: FF7 0x0067656b
void __cdecl SetEngineContext(int value)
{
    g_pGfxEngineContext = (struct GfxEngineContext *)(value);
}

/*
 * Returns the active engine context, g_pGfxEngineContext, which SetEngineContext installs
 * (App_Bootstrap and WinMain at startup; DestroyEngineContext clears it). Nearly every subsystem
 * reaches the window, the driver and the module callbacks through it; the result is null before
 * the context exists and after it is destroyed.
 */
// FUNCTION: FF7 0x00676578
struct GfxEngineContext *__cdecl GetEngineContext(void)
{
    return g_pGfxEngineContext;
}

/*
 * Converts engine context pointer to callback user parameter.
 */
// FUNCTION: FF7 0x00676582
int __cdecl EngineContextToCallbackParam(struct GfxEngineContext *context)
{
    return (int)context;
}

/*
 * Extracts engine context pointer from callback user parameter.
 */
// FUNCTION: FF7 0x0067658a
int __cdecl GetEngineContextFromCallbackParam(int value)
{
    return value;
}

/*
 * Retrieves main window handle from active engine context.
 */
// FUNCTION: FF7 0x00676592
int __cdecl GetEngineWindowHandle(void)
{
    int hwnd;
    GfxEngineContext *pEngine;

    hwnd = 0;
    pEngine = GetEngineContext();
    if (pEngine != 0) {
        hwnd = pEngine->hwnd;
    }
    return hwnd;
}

/*
 * Destroys engine context and releases its allocated memory.
 */
// FUNCTION: FF7 0x006765bd
void __cdecl DestroyEngineContext(void)
{
    GfxEngineContext *ctx;

    ctx = GetEngineContext();
    if (ctx != 0) {
        Gfx_DestroyDebugLogTarget((GfxDebugLogTarget *)(ctx->debugLogTarget));

        GameHeapFree(ctx, DirectXLit_SourcePath_2(), 146);
        SetEngineContext(0);
    }
}

/*
 * Sets the quit flag, the engine context's word +0x00, to 1 when the context is not null. The exit
 * code at +0x60 is set elsewhere (GameWindowProc, WM_QUERYENDSESSION).
 */
// FUNCTION: FF7 0x00676605
void __cdecl RequestEngineQuit(int engine)
{
    if (engine != 0) {
        ((GfxEngineContext *)engine)->quitFlag = 1;
    }
}

/*
 * The game window's message procedure: turns window messages into engine-context state and the
 * game's input callbacks. It first stores hWnd in the active context, then:
 * - WM_ACTIVATEAPP calls onActivateApp(wParam) and WM_DEVICECHANGE calls onDeviceChange(wParam,
 *   lParam), when they are set;
 * - WM_MOVE stores the window position (windowX/windowY), WM_SIZE the client size and sets
 *   minimized for SIZE_MINIMIZED (cleared for any other size message), WM_PAINT only validates;
 * - the mouse messages WM_MOUSEMOVE..WM_MBUTTONDBLCLK (0x200..0x209) go to the active module's onMouse and
 *   WM_KEYDOWN, WM_KEYUP and WM_CHAR to its onKey, with the message, wParam and lParam;
 * - releasing Q (WM_KEYUP, 'Q') while bit 0 of GetAsyncKeyState(VK_CONTROL) is set raises a quit
 *   request (pendingWindowEvent, which WinMain answers with the quit dialog) when allowCtrlQQuit
 *   is unset or agrees. Bit 0 reports that Ctrl was pressed since the previous query, not that it
 *   is held, so the shortcut does not fire reliably;
 * - WM_CLOSE logs, sets the quit flag (RequestEngineQuit) and posts WM_QUIT; WM_QUIT would store
 *   wParam as the exit code, but a posted WM_QUIT has no window and never reaches this procedure.
 * WM_CREATE, WM_ACTIVATE, WM_TIMER and WM_DESTROY return 0; every other message goes to
 * DefWindowProcA. Without an active context every message returns 0.
 */
// FUNCTION: FF7 0x00676619
int __stdcall GameWindowProc(int hWnd, unsigned int msg, unsigned int wParam, int lParam)
{
    GfxEngineContext *ctx;
    int res;
    int vk;
    char ps[64];

    res = 0;
    ctx = GetEngineContext();
    if (ctx != 0) {
        ctx->hwnd = hWnd;
        switch (msg) {
        case WM_CREATE:
            break;
        case WM_ACTIVATE:
            break;
        case WM_ACTIVATEAPP:
            if (ctx->onActivateApp != 0) {
                ctx->onActivateApp(wParam, (int)ctx);
            }
            break;
        case WM_DEVICECHANGE:
            if (ctx->onDeviceChange != 0) {
                ctx->onDeviceChange(wParam, lParam, (int)ctx);
            }
            break;
        case WM_TIMER:
            break;
        case WM_MOVE:
            ctx->windowX = (short)lParam;
            ctx->windowY = (short)((lParam >> 16) & 0xFFFF);
            break;
        case WM_PAINT:
            BeginPaint(hWnd, ps);
            EndPaint(hWnd, ps);
            break;
        case WM_MOUSEMOVE:
        case WM_LBUTTONDOWN:
        case WM_LBUTTONUP:
        case WM_LBUTTONDBLCLK:
        case WM_RBUTTONDOWN:
        case WM_RBUTTONUP:
        case WM_RBUTTONDBLCLK:
        case WM_MBUTTONDOWN:
        case WM_MBUTTONUP:
        case WM_MBUTTONDBLCLK:
            if (ctx->activeModule.onMouse != 0) {
                ctx->activeModule.onMouse(msg, wParam, lParam, ctx);
            }
            break;
        case WM_KEYDOWN:
        case WM_KEYUP:
        case WM_CHAR:
            if (ctx->activeModule.onKey != 0) {
                ctx->activeModule.onKey(msg, wParam, lParam, ctx);
            }
            if (msg == WM_KEYUP) {
                vk = wParam;
                if (vk == 'Q') {
                    if ((GetAsyncKeyState(VK_CONTROL) & 1) == 1) {
                        if (ctx->allowCtrlQQuit != 0) {
                            if (ctx->allowCtrlQQuit((int)ctx) != 0) {
                                ctx->pendingWindowEvent = 1;
                            }
                        } else {
                            ctx->pendingWindowEvent = 1;
                        }
                    }
                }
            }
            break;
        case WM_SIZE:
            ctx->clientWidth = (unsigned short)lParam;
            ctx->clientHeight = (unsigned short)(((unsigned int)lParam >> 16) & 0xFFFF);
            ctx->minimized = 0;
            switch (wParam) {
            case SIZE_MINIMIZED:
                ctx->minimized = 1;
                break;
            case SIZE_RESTORED:
                break;
            }
            break;
        case WM_CLOSE:

            Gfx_DebugPrint(DirectXLit_WmClose());
            RequestEngineQuit((int)ctx);
            PostQuitMessage(0);
            break;
        case WM_QUIT:

            Gfx_DebugPrint(DirectXLit_WmQuit());
            ctx->result = wParam;
            break;
        case WM_DESTROY:
            break;
        default:
            res = 1;
            break;
        }
    }
    if (res != 0) {
        res = DefWindowProcA(hWnd, msg, wParam, lParam);
    }
    return res;
}

/*
 * Opens the game window and brings the engine up in it. It registers the window class with
 * GameWindowProc as its procedure: the context's own class when customWindowClass is set,
 * otherwise style CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS, the arrow cursor, a black brush and the
 * context's windowClassName. The window style is WS_POPUP in full screen and WS_POPUP |
 * WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX in a window, or the context's windowStyle when
 * customWindowStyle is set; OpenGL adds WS_CLIPSIBLINGS | WS_CLIPCHILDREN. The window
 * (WS_EX_APPWINDOW, default position) is displayWidth x displayHeight with the context's
 * windowTitle. The cursor is hidden when showCursor is 0.
 *
 * From the window's DC it stores the desktop width, height and bits per pixel. A windowed context
 * that asks for a colour depth other than the desktop's reports an error and fails; a depth of 0
 * takes the desktop's. Then it calls the active module's init callback (App_InitSubsystems, which sets up
 * the graphics device) and, when that succeeds, its enter callback (GameModeDispatcher_Enter
 * in the game).
 *
 * Returns 1 when the window exists and initialisation succeeded, else 0 (WinMain then skips the
 * message loop). In a window, AdjustWindowRect computes the outer size for the requested client
 * area, but the result is never used: the window is created at the client size, so its client
 * area is smaller than displayWidth x displayHeight by the frame and caption.
 */
// FUNCTION: FF7 0x0067692a
int __cdecl CreateGameWindow(int hInstance, GfxEngineContext *ctx)
{
    int high;
    int hwnd;
    int wide;
    int flags;
    GfxWindowClass wcd;
    int valid;
    int aw;
    int rc[4];
    int ah;
    int hdc;
    char msg[200];

    valid = 0;
    if (ctx->customWindowClass != 0) {
        wcd = *(GfxWindowClass *)ctx->windowClass;
        wcd.e[1] = (int)GameWindowProc;
    } else {
        wcd.e[0] = CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS;
        wcd.e[1] = (int)GameWindowProc;
        wcd.e[2] = 0;
        wcd.e[3] = 0;
        wcd.e[4] = hInstance;
        wcd.e[5] = 0;
        wcd.e[6] = LoadCursorA(0, IDC_ARROW);
        wcd.e[7] = GetStockObject(BLACK_BRUSH);
        wcd.e[8] = 0;
        wcd.e[9] = (int)ctx->windowClassName;
    }
    if (RegisterClassA(&wcd) != 0) {
        if (ctx->fullScreen != 0) {
            flags = WS_POPUP;
        } else {
            flags = WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
        }
        if (ctx->customWindowStyle != 0) {
            flags = ctx->windowStyle;
        }
        if (ctx->driverType == GFX_DRIVER_OPENGL) {
            flags |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
        }
        wide = ctx->displayWidth;
        high = ctx->displayHeight;
        if (ctx->fullScreen == 0) {
            rc[0] = 0;
            rc[1] = 0;
            rc[2] = wide;
            rc[3] = high;
            AdjustWindowRect(rc, flags, (flags & WS_SYSMENU) != 0);
            aw = rc[2] - rc[0] + 1;
            ah = rc[3] - rc[1] + 1;
        }
        hwnd = CreateWindowExA(WS_EX_APPWINDOW, (int)ctx->windowClassName, (int)ctx->windowTitle, flags, CW_USEDEFAULT, CW_USEDEFAULT, wide, high, 0, 0, hInstance, 0);
        if (ctx->showCursor == 0) {
            ShowCursor(0);
        }
        ctx->hwnd = hwnd;
        if (hwnd != 0) {
            valid = 1;
            ShowWindow(hwnd, SW_SHOWNORMAL);
            UpdateWindow(hwnd);
            hdc = GetDC(ctx->hwnd);
            if (hdc != 0) {
                ctx->desktopWidth = GetDeviceCaps(hdc, HORZRES);
                ctx->desktopHeight = GetDeviceCaps(hdc, VERTRES);
                ctx->desktopBitsPerPixel = GetDeviceCaps(hdc, BITSPIXEL);
                if (ctx->fullScreen == 0 && ctx->colorDepth > 0 && ctx->colorDepth != ctx->desktopBitsPerPixel) {

                    sprintf(msg, DirectXLit_RequestedBitsPerPixel(), ctx->colorDepth, ctx->desktopBitsPerPixel);

                    ReportEngineError(ctx, msg, DirectXLit_Error(), 0);
                    valid = 0;
                }
                if (ctx->colorDepth == 0) {
                    ctx->colorDepth = ctx->desktopBitsPerPixel;
                }
                ReleaseDC(ctx->hwnd, hdc);
            }
            if (valid != 0) {
                if (ctx->activeModule.init != 0) {
                    if (ctx->activeModule.init(ctx) == 0) {
                        valid = 0;
                    } else {
                        if (ctx->activeModule.enter != 0) {
                            ctx->activeModule.enter(ctx);
                        }
                    }
                }
            }
        }
    }
    return valid;
}

/*
 * Displays error message dialog and logs engine failure.
 */
// FUNCTION: FF7 0x00676c56
int __cdecl ReportEngineError(struct GfxEngineContext *ctx, char *message, char *title, int type)
{
    int result;
    char line[200];

    result = 2;
    if (ctx != 0) {
        sprintf(line,

                DirectXLit_LineS(), title);
        Gfx_DebugPrint(line);
        sprintf(line,

                DirectXLit_IndentedLineS(), message);
        Gfx_DebugPrint(line);
        if (ctx->fullScreen != 0) {
            result = 1;
        } else if (ctx->activeB != 0) {
            result = MessageBoxA(ctx->hwnd, message, title, type);
        } else {
            result = 1;
        }
    }
    return result;
}

/*
 * Creates engine context with default configuration and drivers.
 */
// FUNCTION: FF7 0x00676d0b
void *__cdecl CreateEngineContextDefaults(GfxAppDescriptor *descriptor)
{
    GfxEngineContext *ctx;

    ctx = (GfxEngineContext *)AllocEngineContext(descriptor);
    if (ctx != 0) {
        ctx->displayWidth = 640;
        ctx->displayHeight = 480;
        ctx->clipLeft = 0;
        ctx->clipTop = 0;
        ctx->viewSizeX = 640;
        ctx->viewSizeY = 480;
        ctx->clipRight = ctx->clipLeft + ctx->viewSizeX - 1;
        ctx->clipBottom = ctx->clipTop + ctx->viewSizeY - 1;
        ctx->colorDepth = 0x10;
        ctx->fullScreen = 1;
        ctx->modeActive = 1;
        ctx->modeTransition = 0;
        ctx->bufferCount = 2;
        ctx->driverType = GFX_DRIVER_DIRECTX;
        ctx->use3DDevice = 1;
        ctx->modeSetupDone = 1;
        ctx->zBufferEnabled = 1;
        ctx->modeLoaded = 1;
        ctx->matrixStackSize = 32;
        ctx->windowTitle =

            DirectXLit_Default();
        ctx->windowClassName =

            DirectXLit_DefaultClass();
        ctx->activeModule.shutdown = (GameModuleCallback)RequestEngineQuit;
        ctx->presentEnable = 1;
    }
    return (void *)ctx;
}

/*
 * Creates a new engine context with the library's defaults and returns it (null when the heap
 * allocation fails); it does not make it active. The descriptor from InitAppDescriptor names the
 * debug log APP.LOG and enables it, so AllocEngineContext allocates the 0xAA0-byte context, opens
 * APP.LOG in the current directory, calibrates the timestamp counter and sets the perspective to
 * 90 degrees, near 5, far 2000. CreateEngineContextDefaults then sets a 640x480 display and
 * viewport, 16 bits per pixel, full screen, two buffers, the Direct3D driver, a Z buffer, a
 * 0x20-entry matrix stack, the title "DEFAULT" and class "DEFAULT CLASS", RequestEngineQuit as the
 * shutdown callback, and presenting enabled. App_Bootstrap, the game's caller, then applies the
 * registry settings over these (ApplyConfigToContext) and installs its own callbacks.
 */
// FUNCTION: FF7 0x00676e7e
void *__cdecl CreateEngineContext(void)
{
    GfxAppDescriptor rec;

    InitAppDescriptor(&rec);
    return CreateEngineContextDefaults(&rec);
}

/*
 * Updates per-frame timing and frame rate calculations.
 */
// FUNCTION: FF7 0x00676ea0
void __cdecl UpdateFrameTiming(struct GfxEngineContext *ctx)
{
    double elapsed;
    unsigned int diff[2];
    double excess;
    unsigned int now[2];

    // The names deliberately select the original VC5 local-slot buckets.
    ReadTimestampCounter((struct Timestamp64 *)now);
    Sub64((struct Timestamp64 *)now, &ctx->secondStart, (struct Timestamp64 *)diff);
    elapsed = CombineSplitValueToFloat((struct Timestamp64 *)diff);
    if (elapsed >= ctx->tscFrequency) {
        excess = elapsed - ctx->tscFrequency;
        *(unsigned __int64 *)&ctx->fps = *(unsigned __int64 *)&ctx->framesThisSecond;
        *(unsigned __int64 *)&ctx->framesThisSecond = 0;
        ReadTimestampCounter(&ctx->secondStart);
    }
}

/*
 * Restores lost DirectDraw surfaces and reloads textures.
 */
// FUNCTION: FF7 0x00676f2a
int __cdecl RestoreLostSurfaces(GfxEngineContext *context)
{
    int val;
    val = 0;
    if (context->driverType == GFX_DRIVER_DIRECTX) {
        if (context->primarySurface != 0) {
            if (((ComDirectDrawSurface *)context->primarySurface)->IsLost() == DDERR_SURFACELOST) {
                val |= !CheckDirectDrawResult(((ComDirectDrawSurface *)context->primarySurface)->Restore(),

                                            DirectXLit_SourcePath_3(), 634);
            }
        }
        if (context->backSurface != 0) {
            if (((ComDirectDrawSurface *)context->backSurface)->IsLost() == DDERR_SURFACELOST) {
                val |= !CheckDirectDrawResult(((ComDirectDrawSurface *)context->backSurface)->Restore(),

                                            DirectXLit_SourcePath_4(), 642);
            }
        }
        if (context->zBufferSurface != 0) {
            if (((ComDirectDrawSurface *)context->zBufferSurface)->IsLost() == DDERR_SURFACELOST) {
                val |= CheckDirectDrawResult(((ComDirectDrawSurface *)context->zBufferSurface)->Restore(),

                                           DirectXLit_SourcePath_5(), 651);
            }
        }
    }
    return val == 0;
}

/*
 * Flips back buffer or blits rendered frame to primary surface.
 */
// FUNCTION: FF7 0x00677067
void __cdecl GfxPresentBackBuffer(struct GfxEngineContext *context)
{
    struct {
        int effect[25];
        int errors;
        int source[4];
        int destination[4];
        int result;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    if (context->fullScreen != 0) {
        while (1) {
            frame.result = ((ComDirectDrawSurface *)context->primarySurface)->Flip(0, 1);
            if (frame.result == 0)
                break;
            if (frame.result == DDERR_SURFACELOST) {
                frame.result = ((ComDirectDrawSurface *)context->primarySurface)->Restore();
                if (frame.result != 0)
                    break;
            }
            if (frame.result != DDERR_WASSTILLDRAWING)
                break;
        }
    } else if (context->primarySurface != 0 && context->backSurface != 0) {
        frame.source[0] = 0;
        frame.source[1] = 0;
        frame.source[2] = context->clientWidth;
        frame.source[3] = context->clientHeight;
        frame.destination[0] = context->windowX;
        frame.destination[1] = context->windowY;
        frame.destination[2] = context->windowX + context->clientWidth;
        frame.destination[3] = context->windowY + context->clientHeight;
        frame.errors = 0;
        if (((ComDirectDrawSurface *)context->primarySurface)->IsLost() == DDERR_SURFACELOST) {
            frame.errors |= !CheckDirectDrawResult(((ComDirectDrawSurface *)context->primarySurface)->Restore(),

                                                   DirectXLit_SourcePath_6(), 716);
        }
        if (((ComDirectDrawSurface *)context->backSurface)->IsLost() == DDERR_SURFACELOST) {
            frame.errors |= !CheckDirectDrawResult(((ComDirectDrawSurface *)context->backSurface)->Restore(),

                                                   DirectXLit_SourcePath_7(), 720);
        }
        if (frame.errors == 0) {
            memset(frame.effect, 0, 100);
            frame.effect[0] = 100;
            frame.effect[1] = 8;
            CheckDirectDrawResult(((ComDirectDrawSurface *)context->primarySurface)->Blt(frame.destination, context->backSurface, frame.source, 0x1000000, frame.effect),

                                  DirectXLit_SourcePath_8(), 740);
        }
    }
    UpdateFrameTiming(context);
}

/*
 * Formats diagnostic description text for enumerated DirectDraw driver.
 */
// FUNCTION: FF7 0x00677272
void __cdecl FormatDDEnumText(int index, int value, int buffer)
{
    sprintf((char *)buffer,
            DirectXLit_LineSS(), index, value);
}

/*
 * Allocates a new DirectDraw driver information record.
 */
// FUNCTION: FF7 0x00677290
int __cdecl AllocDDDriverRecord(void)
{
    int val;

    val = (int)GameHeapAlloc(1, 32, DirectXLit_SourcePath_9(), 768);
    return val;
}

/*
 * Releases DirectDraw driver information record and display mode list.
 */
// FUNCTION: FF7 0x006772b4
void __cdecl FreeDDDriverRecord(GfxDdDriverRecord *driverRecord)
{
    if (driverRecord != 0) {

        GameHeapFree((void *)driverRecord->driverDescription, DirectXLit_SourcePath_10(), 777);

        GameHeapFree((void *)driverRecord->driverName, DirectXLit_SourcePath_11(), 778);

        GameHeapFree(driverRecord, DirectXLit_SourcePath_12(), 779);
    }
}

/*
 * Appends DirectDraw driver record to driver linked list.
 */
// FUNCTION: FF7 0x00677307
void __cdecl AppendDDDriverRecord(int record, GfxEngineContext *holder)
{
    if (record != 0) {
        if (holder->ddDriverList == 0) {
            holder->ddDriverList = (int)CreateList((ListFreeValueFn)FreeDDDriverRecord);
        }
        ListPrependValue(record, (List *)(holder->ddDriverList));
    }
}

/*
 * Callback that processes enumerated DirectDraw display drivers.
 */
// FUNCTION: FF7 0x00677341
int __stdcall DDEnumDriversCallback(DxGuid *guid, int description, int name, int data)
{
    struct {
        int record;
        GfxEngineContext *context;
        int result;
        char text[200];
    } f; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    f.result = 1;
    if (guid != 0) {
        FormatDDEnumText((int)&g_GfxPrimaryDriverFlag, name, (int)f.text);
        Gfx_DebugPrint(f.text);
        FormatDDEnumText((int)
                             DirectXLit_TwoSpaces_1(),
                         description, (int)f.text);
        Gfx_DebugPrint(f.text);
        f.context = (GfxEngineContext *)GetEngineContextFromCallbackParam(data);
        if (f.context != 0) {
            f.record = AllocDDDriverRecord();
            if (f.record != 0) {
                *(DxGuid *)f.record = *guid;
                ((GfxDdDriverRecord *)f.record)->driverName = (int)GameHeapStrDup((char *)name);
                ((GfxDdDriverRecord *)f.record)->driverDescription = (int)GameHeapStrDup((char *)description);
                AppendDDDriverRecord(f.record, f.context);
            }
        }
    }
    return f.result;
}

/*
 * Callback that records enumerated DirectDraw display modes.
 */
// FUNCTION: FF7 0x00677446
int __stdcall DDEnumDisplayModesCallback(GfxDisplayModeDesc *desc, int data)
{
    struct {
        GfxEngineContext *context;
        char text[200];
        int mask;
        int result;
    } f;
    f.result = 1;
    f.mask = 0x100e;
    if (((desc->flags | 1) & f.mask) == f.mask) {
        sprintf(f.text,
                DirectXLit_DdModeWH(), desc->width, desc->height, desc->pitch, (desc[1].height & 0x40) != 0, desc[1].pitch);
        Gfx_DebugPrint(f.text);
        f.context = (GfxEngineContext *)GetEngineContextFromCallbackParam(data);
        if (f.context != 0) {
            (f.context->displayModeCount)++;
            if (desc->width == f.context->displayWidth && desc->height == f.context->displayHeight && (desc[1].height & 0x40) != 0 && desc[1].pitch == f.context->colorDepth) {
                (f.context->matchingDisplayModes)++;
                Gfx_DebugPrint(
                                           DirectXLit_DisplayModeMatch());
            }
        }
    }
    return f.result;
}

/*
 * Allocates a new Direct3D device capability record.
 */
// FUNCTION: FF7 0x00677569
int __cdecl AllocD3DDeviceRecord(void)
{
    int val;

    val = (int)GameHeapAlloc(1, 32, DirectXLit_SourcePath_13(), 888);
    return val;
}

/*
 * Releases Direct3D device capability record.
 */
// FUNCTION: FF7 0x0067758d
void __cdecl FreeD3DDeviceRecord(GfxD3DDeviceRecord *deviceRecord)
{
    if (deviceRecord != 0) {

        GameHeapFree((void *)deviceRecord->name, DirectXLit_SourcePath_14(), 897);

        GameHeapFree((void *)deviceRecord->description, DirectXLit_SourcePath_15(), 898);

        GameHeapFree((void *)deviceRecord->hardwareDesc, DirectXLit_SourcePath_16(), 899);

        GameHeapFree((void *)deviceRecord->softwareDesc, DirectXLit_SourcePath_17(), 900);

        GameHeapFree(deviceRecord, DirectXLit_SourcePath_18(), 902);
    }
}

/*
 * Appends Direct3D device record to device linked list.
 */
// FUNCTION: FF7 0x00677612
void __cdecl AppendD3DDeviceRecord(int record, GfxEngineContext *holder)
{
    if (record != 0) {
        if (holder->d3dDeviceList == 0) {
            holder->d3dDeviceList = (int)CreateList((ListFreeValueFn)FreeD3DDeviceRecord);
        }
        ListPrependValue(record, (List *)(holder->d3dDeviceList));
    }
}

/*
 * Callback that processes enumerated Direct3D devices and capabilities.
 */
// FUNCTION: FF7 0x00677655
int __stdcall D3DEnumDevicesCallback(DxGuid *guid, int description, int name, GfxD3DDeviceDesc *hardware, GfxD3DDeviceDesc *software, int data)
{
    struct {
        GfxD3DDeviceRecord *record;
        GfxEngineContext *context;
        int result;
        char text[200];
    } f; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    f.result = 0;
    f.result = 1;
    if (guid != 0) {
        FormatDDEnumText((int)g_DirectDrawEnumDevicePrefix, name, (int)f.text);
        Gfx_DebugPrint(f.text);
        FormatDDEnumText((int)
                             DirectXLit_TwoSpaces_2(),
                         description, (int)f.text);
        Gfx_DebugPrint(f.text);
        f.context = (GfxEngineContext *)GetEngineContextFromCallbackParam(data);
        if (f.context != 0) {
            f.record = (GfxD3DDeviceRecord *)AllocD3DDeviceRecord();
            if (f.record != 0) {
                *(DxGuid *)f.record = *guid;
                f.record->name = (int)GameHeapStrDup((char *)name);
                f.record->description = (int)GameHeapStrDup((char *)description);
                f.record->hardwareDesc = (int)GameHeapMemDup(hardware->size, hardware);
                f.record->softwareDesc = (int)GameHeapMemDup(software->size, software);
                AppendD3DDeviceRecord((int)f.record, f.context);
            }
        }
    }
    return f.result;
}

/*
 * Tests whether Direct3D device matches required capability mask.
 */
// FUNCTION: FF7 0x006777a2
int __cdecl TestD3DDeviceCapabilityMask(int mask, GfxD3DDeviceDesc *record)
{
    int result;

    result = 0;
    if (record->flags != 0 && (record->flags & mask) != 0) {
        result = 1;
    }
    return result;
}

/*
 * Checks if Direct3D device color model matches requested model.
 */
// FUNCTION: FF7 0x006777d1
int __cdecl D3DDeviceColorModelMatches(GfxD3DDeviceDesc *device, struct GfxEngineContext *config)
{
    int result;

    result = 0;
    if (TestD3DDeviceCapabilityMask(D3DDD_COLORMODEL, device) != 0) {
        switch (device->colorModel) {
        case D3DCOLOR_MONO:
            if (config->modeLoaded == 0) {
                result = 1;
            }
            break;
        case D3DCOLOR_RGB:
            if (config->modeLoaded != 0) {
                result = 1;
            }
        }
    }
    return result;
}

/*
 * Callback to select preferred Direct3D device from candidate list.
 */
// FUNCTION: FF7 0x00677836
int __cdecl D3DPickDeviceCallback(GfxD3DDeviceRecord *candidate, GfxD3DDeviceRecord **outDevice)
{
    int result;
    GfxEngineContext *ctx;
    GfxD3DDeviceDesc *model;
    int isMatch;
    GfxD3DDeviceDesc *hwDesc;

    result = 1;
    isMatch = 0;
    ctx = GetEngineContext();
    if (ctx != 0) {
        hwDesc = (GfxD3DDeviceDesc *)candidate->hardwareDesc;
        model = (GfxD3DDeviceDesc *)candidate->softwareDesc;
        if (ctx->modeSetupDone != 0) {
            if (hwDesc->flags == 0) {
                if (D3DDeviceColorModelMatches(model, ctx) != 0) {
                    isMatch = 1;
                }
            }
        } else if (hwDesc->flags != 0) {
            isMatch = 1;
        }
        if (isMatch != 0) {
            *outDevice = candidate;
            result = 0;
        }
    }
    return result;
}

/*
 * Retrieves DirectDraw surface pointer for given buffer index.
 */
// FUNCTION: FF7 0x006778c8
int __cdecl GfxGetSurfaceByIndex(int bufferIndex)
{
    GfxEngineContext *ctx;
    int surface;

    surface = 0;
    ctx = GetEngineContext();
    if (ctx != 0) {
        surface = ctx->surfaces[bufferIndex];
    }
    return surface;
}

/*
 * Retrieves current 3D rendering target DirectDraw surface.
 */
// FUNCTION: FF7 0x006778f7
int __cdecl GfxGetRenderTargetSurface(struct GfxEngineContext *context)
{
    int surface;
    int index;

    if (context->bufferCount > 1) {
        index = 1;
    } else {
        index = 0;
    }
    surface = context->surfaces[index];
    return surface;
}

/*
 * Creates Direct3D device interface from DirectDraw surface.
 */
// FUNCTION: FF7 0x0067792d
int __cdecl D3DCreateDeviceFromSurface(int surface, int iid)
{
    int result;
    result = 0;
    if (CheckDirectDrawResult(((ComUnknown *)surface)->QueryInterface((const void *)iid, &result),
                              DirectXLit_SourcePath_19(), 1091)) { /* empty first arm: lib/codegen.h */
    } else {
        result = 0;
    }
    return result;
}

/*
 * Creates Direct3D device interface using engine context.
 */
// FUNCTION: FF7 0x00677972
int __cdecl GfxCreateD3DDevice(int iid, struct GfxEngineContext *context)
{
    context->d3dDevice = D3DCreateDeviceFromSurface(GfxGetRenderTargetSurface(context), iid);
    return context->d3dDevice != 0;
}

/*
 * Creates Direct3D2 device interface from DirectDraw surface.
 */
// FUNCTION: FF7 0x006779aa
int __cdecl D3DCreateDevice2FromSurface(int iid, struct GfxEngineContext *context)
{
    int result;
    result = 0;
    if (CheckDirectDrawResult(((ComDirect3D2 *)context->direct3D2)->CreateDevice((const void *)iid, GfxGetRenderTargetSurface(context), &result),
                              DirectXLit_SourcePath_20(), 1115)) { /* empty first arm: lib/codegen.h */
    } else {
        result = 0;
    }
    return result;
}

/*
 * Creates Direct3D2 device interface using engine context.
 */
// FUNCTION: FF7 0x00677a09
int __cdecl GfxCreateD3DDevice2(int iid, struct GfxEngineContext *context)
{
    context->d3dDevice2 = D3DCreateDevice2FromSurface(iid, context);
    return context->d3dDevice2 != 0;
}

/*
 * Allocates a new texture pixel format record structure.
 */
// FUNCTION: FF7 0x00677a38
int __cdecl AllocTextureFormatRecord(void)
{
    GfxTextureFormatRecord *rec;

    rec = (GfxTextureFormatRecord *)((int)GameHeapAlloc(1, 132, DirectXLit_SourcePath_21(), 1138));
    return (int)rec;
}

/*
 * Frees a texture pixel format record structure.
 */
// FUNCTION: FF7 0x00677a5f
void __cdecl FreeTextureFormatRecord(int formatRecord)
{
    if (formatRecord != 0) {

        GameHeapFree(((GfxTextureFormatRecord *)formatRecord)->surfaceDesc, DirectXLit_SourcePath_22(), 1147);

        GameHeapFree((void *)formatRecord, DirectXLit_SourcePath_23(), 1148);
    }
}

/*
 * Compares two texture format records for sort ordering.
 */
// FUNCTION: FF7 0x00677a9c
int __cdecl CompareTextureFormatRecords(GfxTextureFormatRecord *recordA, GfxTextureFormatRecord *recordB)
{
    int result;

    result = 0;
    if (recordA->format.bitsPerPixel == recordB->format.bitsPerPixel) {
        if (recordA->format.alphaBits == recordB->format.alphaBits) {
            result = 0;
        } else if (recordA->format.alphaBits < recordB->format.alphaBits) {
            result = 0xFFFFFFFF;
        } else {
            result = 1;
        }
    } else if (recordA->format.bitsPerPixel < recordB->format.bitsPerPixel) {
        result = 0xFFFFFFFF;
    } else {
        result = 1;
    }
    return result;
}

/*
 * Inserts texture format record into sorted linked list.
 */
// FUNCTION: FF7 0x00677b11
void __cdecl InsertTextureFormatRecord(GfxTextureFormatRecord *formatRecord, GfxEngineContext *context)
{
    if (context->textureFormatList == 0) {
        context->textureFormatList = (int)CreateList(FreeTextureFormatRecord);
    }
    ListInsertSortedByCompare((ListCompareFn)CompareTextureFormatRecords, (int)formatRecord, (List *)(context->textureFormatList));
}

/*
 * Callback that records enumerated Direct3D texture pixel formats.
 */
// FUNCTION: FF7 0x00677b53
int __stdcall D3DEnumTextureFormatsCallback(GfxTextureSurfaceDesc *desc, int data)
{
    struct {
        int allocated;
        GfxTextureFormatRecord *record;
        int context;
    } f; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    if (data != 0) {
        f.context = GetEngineContextFromCallbackParam(data);
        if (f.context != 0) {
            f.allocated = AllocTextureFormatRecord();
            f.record = (GfxTextureFormatRecord *)f.allocated;
            if (f.record != 0) {
                f.record->surfaceDesc = GameHeapMemDup(desc->size, desc);
                PixelFormatFromDDPixelFormat((GfxDdPixelFormat *)&desc->pixelFormatSize, &f.record->format);
                InsertTextureFormatRecord(f.record, (GfxEngineContext *)f.context);
            }
            f.record = 0;
        }
    }
    return 1;
}

/*
 * Logs diagnostic information for texture pixel format.
 */
// FUNCTION: FF7 0x00677bd8
int __cdecl LogTextureFormatRecord(GfxTextureFormatRecord *record)
{
    struct {
        char text[200];
        int result;
    } f;
    f.result = 1;
    if (record != 0) {
        sprintf(f.text,
                DirectXLit_PaletteBitsRgba(), record->format.palettized, record->format.bitsPerPixel, record->format.redBits, record->format.greenBits, record->format.blueBits, record->format.alphaBits);
        Gfx_DebugPrint(f.text);
    }
    return f.result;
}

/*
 * Checks whether requested Z-buffer bit depth is supported.
 */
// FUNCTION: FF7 0x00677c42
int __cdecl ZBufferDepthSupported(int bitDepth, int zCaps)
{
    int supported;

    supported = 0;
    /* Numeric colour depth in bits per pixel. */
    switch (bitDepth) {
    case GFX_BPP_8:
        if ((zCaps & DDBD_8) != 0) {
            supported = 1;
        }
        break;
    case GFX_BPP_16:
        if ((zCaps & DDBD_16) != 0) {
            supported = 1;
        }
        break;
    case GFX_BPP_24:
        if ((zCaps & DDBD_24) != 0) {
            supported = 1;
        }
        break;
    case GFX_BPP_32:
        if ((zCaps & DDBD_32) != 0) {
            supported = 1;
        }
    }
    return supported;
}

/*
 * Selects best supported Z-buffer bit depth from surface caps.
 */
// FUNCTION: FF7 0x00677cff
int __cdecl PickZBufferDepth(int desc)
{
    struct {
        unsigned index;
        unsigned depth;
    } f; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    f.index = 0;
    do {
        f.depth = g_GfxBitDepths[f.index];
        if (ZBufferDepthSupported(f.depth, desc)) {
            break;
        }
        f.index++;
    } while (f.depth > 0);
    return f.depth;
}

/*
 * Initializes Direct3D device description structure size.
 */
// FUNCTION: FF7 0x00677d45
void __cdecl D3DDeviceDescInit(GfxD3DDeviceDesc *desc)
{
    memset((void *)desc, 0, 0xCC);
    desc->size = 204;
    desc->triCapsSize = 56;
    desc->lineCapsSize = 56;
    desc->transformCapsSize = 8;
    desc->lightingCapsSize = 16;
}

/*
 * Initializes DirectDraw surface description structure size.
 */
// FUNCTION: FF7 0x00677d89
void __cdecl DDSurfaceDescInit(GfxTextureSurfaceDesc *desc)
{
    memset(desc, 0, 0x6C);
    desc->size = 108;
    desc->pixelFormatSize = 32;
}

/*
 * Stub callback for graphics driver initialization.
 */
// FUNCTION: FF7 0x00677daf
int __cdecl GfxDriverInitStub(void)
{
    return 1;
}

/*
 * Stub callback for graphics driver cleanup.
 */
// FUNCTION: FF7 0x00677db9
void __cdecl GfxDriverCleanupStub(void)
{
    /* The original is empty. Its whole body is
       `55 8b ec 5d c3` -- 5 bytes at 0x00677db9,
       push/mov/pop/ret plus alignment padding.
       The C body is empty, as in retail. */
}

/*
 * Initializes graphics rendering resources, viewports, and materials.
 */
// FUNCTION: FF7 0x00677dbe
int __cdecl GfxInitRenderResources(struct GfxEngineContext *context)
{
    int idx;
    MatrixStack *matrixStackPtr;
    int result;
    int blockCount;
    int bucketCount;
    int depthRange;
    int entIndex;

    result = 0;
    if (context != 0) {
        context->polygonSetList = (int)CreateList((ListFreeValueFn)D3DListNoopCallback);
        context->presetPolygonSets = Gfx_CreatePresetPolygonSetTable(context);
        Gfx_BuildPresetHundreds(context);
        D3DSetupDevicePolygonSets(context);
        context->statsRecord = (GfxStatsRecord *)Gfx_StatsCreateRecord();
        context->frameStatsRecord = Gfx_StatsCreateRecord();
        ZeroFillMemory32(252, (void *)context->frameStatsRecord);
        context->fileRegistry = FileRegistryCreate((int)FileFreeRecordCallback);
        Gfx_DebugPrint(DirectXLit_MatrixInitialize());
        if (context->matrixStackSize > 0) {
            matrixStackPtr = (MatrixStack *)(MatrixStack_Create(context->matrixStackSize));
            if (matrixStackPtr != 0) {
                context->matrixStack = (int)matrixStackPtr;
                for (idx = 0; idx < matrixStackPtr->capacity; idx++) {
                    ((int *)(matrixStackPtr->entries))[idx] = Gfx_AllocIdentityMatrix4x4();
                }
            }
            matrixStackPtr = (MatrixStack *)(MatrixStack_Create(context->matrixStackSize));
            if (matrixStackPtr != 0) {
                context->matrixStack2 = (int)matrixStackPtr;
                for (idx = 0; idx < matrixStackPtr->capacity; idx++) {
                    ((int *)(matrixStackPtr->entries))[idx] = Gfx_AllocIdentityMatrix4x4();
                }
            }
        }
        context->cameraMatrix = Gfx_AllocIdentityMatrix4x4();
        blockCount = 2048;
        depthRange = 0x10000;
        bucketCount = 512;
        context->depthSortQueue = Gfx_CreateSortedDrawQueue(blockCount, depthRange, bucketCount);
        context->drawPool = Gfx_CreateDrawPool(512);
        context->renderObjectBank = Gfx_CreatePrimitiveMeshBank(50, 256, 50, 256, (int)context);
        context->renderResourceTable = MatrixStack_Create(20);
        if (context->renderResourceTable != 0) {
            for (entIndex = 0; entIndex < ((MatrixStack *)(context->renderResourceTable))->capacity; entIndex++) {

                ((int *)(((MatrixStack *)(context->renderResourceTable))->entries))[entIndex] = (int)GameHeapAlloc(1, 30, DirectXLit_SourcePath_24(), 1440);
                if (((int *)(((MatrixStack *)(context->renderResourceTable))->entries))[entIndex] != 0) {
                    SetIdentityPsxMatrix((PsxMatrix *)((((int *)(((MatrixStack *)(context->renderResourceTable))->entries))[entIndex])));
                }
            }
        }
        ReadTimestampCounter(&context->secondStart);
        context->nodeList = CreateBlockHeapNodeList(100);
        result = 1;
    }
    return result;
}

/*
 * Brings up the graphics driver the context's driverType selects and leaves a cleared screen.
 * Returns nonzero on success; on 0 App_InitSubsystems skips the rest of the game's setup and
 * CreateGameWindow fails.
 *
 * 1. Binds the driver ops table: Direct3D (GfxBindDriverOpsD3D, setupFlags DirectDraw and
 *    Direct3D), software (GfxBindDriverOpsDDrawBlit, setupFlags DirectDraw only, no 3D
 *    surfaces), a driver DLL (its procedure) or OpenGL (Gl_BindDriverOps). The last two set no
 *    setupFlags here and start with status 1 (the DLL only when it has a procedure). No ops
 *    table, or an unknown type, returns 0.
 * 2. DirectDraw (GFX_SETUP_DIRECTDRAW): with modeActive 0 (the registry picks a non-primary display
 *    device) it forces full screen, uses the GUID of the first enumerated secondary driver and
 *    also switches the primary display to the requested mode. It creates the DirectDraw object,
 *    logs the HAL/HEL caps and free video memory, queries IDirectDraw2, enumerates display modes
 *    and Direct3D devices, and takes exclusive full-screen access and the displayWidth x
 *    displayHeight x colorDepth mode (failure to set the mode returns 0), or normal access in a
 *    window. Without DirectDraw hardware, or with modeTransition set, the primary surface is in
 *    system memory instead of video memory. It gets bufferCount (clamped to 1..3) buffers as a
 *    flip chain in full screen; in a window it gets a clipper and bufferCount - 1 separate
 *    off-screen back surfaces.
 * 3. Direct3D (GFX_SETUP_DIRECT3D): picks a device (D3DPickDeviceCallback), creates a Z buffer at the
 *    display size with the requested depth or one the device supports (PickZBufferDepth), in
 *    system memory for the software device description (modeSetupDone set) or video memory for
 *    the hardware one, then the device, its caps and texture formats, the Direct3D 5 device when
 *    useDirect3D5 is set, both viewports and the material.
 * 4. A lock/unlock test of the back surface, the colour ramp tables in rampBlock
 *    (Blit_BuildColorRampTables), the driver's init slot, a full-screen viewport, a black
 *    background, clear, present, clear, and GfxInitRenderResources (pools, queues, matrix stacks,
 *    the render-object bank).
 *
 * The status is set to 1 only inside the back-buffer loops, so with bufferCount 1 the Direct3D and
 * software drivers skip steps 3 and 4 and the function returns 0. A failed exclusive cooperative
 * level is logged and ignored: the display mode is then not set and initialisation goes on.
 */
// FUNCTION: FF7 0x0067806e
int __cdecl InitGraphicsDevice(GfxEngineContext *context)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[2160];
        GfxClearColorBgra bgColor;
        struct { unsigned char pad_to_10[0x10]; GfxDriverOps *driverObject; };
        struct { unsigned char pad_to_14[0x14]; GfxD3DViewport viewport2Data; };
        struct { unsigned char pad_to_40[0x40]; GfxD3DScaledViewport viewportData; };
        struct { unsigned char pad_to_6C[0x6C]; GfxD3DDeviceDesc hwDeviceDesc; };
        struct { unsigned char pad_to_138[0x138]; GfxD3DDeviceDesc swDeviceDesc; };
        struct { unsigned char pad_to_204[0x204]; GfxTextureSurfaceDesc dxDesc; };
        struct { unsigned char pad_to_270[0x270]; char driverMsgBuffer[200]; };
        struct { unsigned char pad_to_338[0x338]; GfxTextureSurfaceDesc zSurfaceDesc; };
        struct { unsigned char pad_to_3A4[0x3A4]; int driverTypeIndex; };
        struct { unsigned char pad_to_3A8[0x3A8]; int zDepthOk; };
        struct { unsigned char pad_to_3AC[0x3AC]; int surfaceIndex; };
        struct { unsigned char pad_to_3B0[0x3B0]; int indexSurface; };
        struct { unsigned char pad_to_3B4[0x3B4]; int zDepthMask; };
        struct { unsigned char pad_to_3B8[0x3B8]; GfxInitContext *deviceEntry; };
        struct { unsigned char pad_to_3BC[0x3BC]; int pickedDevice; };
        struct { unsigned char pad_to_3C0[0x3C0]; GfxTextureSurfaceDesc offscreenDesc; };
        struct { unsigned char pad_to_42C[0x42C]; int bufferIndex; };
        struct { unsigned char pad_to_430[0x430]; int attachedCapsOut; };
        struct { unsigned char pad_to_434[0x434]; int primaryCapsOut; };
        struct { unsigned char pad_to_438[0x438]; int modeFlag; };
        struct { unsigned char pad_to_43C[0x43C]; GfxTextureSurfaceDesc primaryDesc; };
        struct { unsigned char pad_to_4A8[0x4A8]; int attachCaps; };
        struct { unsigned char pad_to_4AC[0x4AC]; int vidMemFree; };
        struct { unsigned char pad_to_4B0[0x4B0]; int vidMemTotal; };
        struct { unsigned char pad_to_4B4[0x4B4]; int vidMemCaps; };
        struct { unsigned char pad_to_4B8[0x4B8]; GfxDirectDrawDriverCaps driverCapsHw; };
        struct { unsigned char pad_to_624[0x624]; int d3dInterface; };
        struct { unsigned char pad_to_628[0x628]; GfxDirectDrawDriverCaps driverCapsHel; };
        struct { unsigned char pad_to_794[0x794]; int ddInterface; };
        struct { unsigned char pad_to_798[0x798]; int coopObject; };
        struct { unsigned char pad_to_79C[0x79C]; int driverProcedure; };
        struct { unsigned char pad_to_7A0[0x7A0]; int driverGuid; };
        struct { unsigned char pad_to_7A4[0x7A4]; int initStatus; };
        struct { unsigned char pad_to_7A8[0x7A8]; char msgBuffer[200]; };
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
#pragma pack(pop)

    Gfx_DebugPrint(
                               DirectXLit_InitializeDdD3dStart());
    frame.initStatus = 0;
    if (context != 0) {
        switch (context->driverType) {
        case GFX_DRIVER_DIRECTX:
            context->driverObject = GfxBindDriverOpsD3D(context);
            context->setupFlags = GFX_SETUP_DIRECTDRAW | GFX_SETUP_DIRECT3D;
            break;
        case GFX_DRIVER_SOFTWARE:
            context->driverObject = GfxBindDriverOpsDDrawBlit(context);
            context->setupFlags = GFX_SETUP_DIRECTDRAW;
            context->use3DDevice = 0;
            break;
        case GFX_DRIVER_DLL:
            frame.driverProcedure = context->driverProcedure;
            if (frame.driverProcedure != 0) {
                /* Invokes untyped callback slot `frame.driverProcedure` with its runtime (GfxEngineContext *) -> GfxDriverOps * signature. */
                context->driverObject = ((GfxDriverOps *(__cdecl *)(GfxEngineContext *))frame.driverProcedure)(context);
                frame.initStatus = 1;
            }
            break;
        case GFX_DRIVER_OPENGL:
            context->driverObject = Gl_BindDriverOps(context);
            context->use3DDevice = 0;
            frame.initStatus = 1;
            break;
        default:
            return 0;
        }
        if (context->driverObject != 0) { /* empty first arm: lib/codegen.h */
        } else {
            return 0;
        }
        if (context->modeActive == 0) {
            context->fullScreen = 1;
        }
        if ((context->setupFlags & GFX_SETUP_DIRECTDRAW) != 0) {
            frame.driverGuid = 0;
            if (CheckDirectDrawResult(DirectDrawEnumerateA((int)DDEnumDriversCallback, EngineContextToCallbackParam(context)), DirectXLit_SourcePath_25(), 1539) != 0 && context->modeActive == 0) {
                if (context->ddDriverList != 0 && ((GfxDeviceInfo *)(context->ddDriverList))->count > 0) {
                    frame.driverGuid = *(int *)((char *)((GfxDeviceInfo *)(context->ddDriverList))->driverRecord + 4);
                } else {
                    context->modeActive = 1;
                }
            }
            if (context->modeActive == 0 && context->fullScreen != 0) {
                if (CheckDirectDrawResult(DirectDrawCreate(0, (int)&context->displayDdObject, 0), DirectXLit_SourcePath_26(), 1564) != 0) {
                    frame.coopObject = context->displayDdObject;
                    DDSurfaceDescInit(&context->displayDdSavedMode);
                    if (CheckDirectDrawResult(((ComDirectDraw *)(frame.coopObject))->GetDisplayMode(&context->displayDdSavedMode), DirectXLit_SourcePath_27(), 1572) != 0) {
                        context->displayDdModeSaved = 1;
                    }
                    if (CheckDirectDrawResult(((ComDirectDraw *)(frame.coopObject))->SetCooperativeLevel((int)(context->hwnd), (int)(DDSCL_NORMAL)), DirectXLit_SourcePath_28(), 1577) != 0) {
                        CheckDirectDrawResult(((ComDirectDraw *)(frame.coopObject))->SetDisplayMode((int)(context->displayWidth), (int)(context->displayHeight), (int)(context->colorDepth)), DirectXLit_SourcePath_29(), 1579);
                    }
                }
            }
            if (CheckDirectDrawResult(DirectDrawCreate(frame.driverGuid, (int)&context->ddObject, 0), DirectXLit_SourcePath_30(), 1588) != 0) {
                frame.ddInterface = context->ddObject;
                if (context->fullScreen != 0) {
                    DDSurfaceDescInit(&context->savedDisplayMode);
                    if (CheckDirectDrawResult(((ComDirectDraw *)(frame.ddInterface))->GetDisplayMode(&context->savedDisplayMode), DirectXLit_SourcePath_31(), 1601) != 0) {
                        context->displayModeSaved = 1;
                    }
                }
                memset(&frame.driverCapsHw, 0, sizeof(frame.driverCapsHw));
                memset(&frame.driverCapsHel, 0, sizeof(frame.driverCapsHel));
                frame.driverCapsHw.size = sizeof(frame.driverCapsHw);
                frame.driverCapsHel.size = sizeof(frame.driverCapsHel);
                if (CheckDirectDrawResult(((ComDirectDraw *)(context->ddObject))->GetCaps(&frame.driverCapsHw, &frame.driverCapsHel), DirectXLit_SourcePath_32(), 1614) != 0) {
                    context->driverCapsHw = frame.driverCapsHw;
                    context->driverCapsHel = frame.driverCapsHel;
                    if (context->deviceIndex != 0) {
                        Gfx_DebugPrint(
                                                   DirectXLit_Hal());
                        Gfx_LogDirectDrawDriverCaps(&frame.driverCapsHw);
                        Gfx_DebugPrint(
                                                   DirectXLit_Hel());
                        Gfx_LogDirectDrawDriverCaps(&frame.driverCapsHel);
                    }
                }
                frame.d3dInterface = 0;
                if (CheckDirectDrawResult(((ComUnknown *)(frame.ddInterface))->QueryInterface(&IID_IDirectDraw2, &context->ddObject2), DirectXLit_SourcePath_33(), 1631) != 0) {
                    frame.d3dInterface = context->ddObject2;
                    Gfx_DebugPrint(
                                               DirectXLit_DirectDraw2());
                }
                if (frame.d3dInterface != 0 && (frame.driverCapsHw.caps & DDCAPS_NOHARDWARE) == 0) {
                    frame.vidMemCaps = DDSCAPS_OFFSCREENPLAIN;
                    if (CheckDirectDrawResult(((ComDirectDraw *)(frame.d3dInterface))->GetAvailableVidMem(&frame.vidMemCaps, &frame.vidMemTotal, &frame.vidMemFree), DirectXLit_SourcePath_34(), 1645) != 0) {
                        sprintf(frame.msgBuffer, (const char *)DirectXLit_TotalFree(),
                                frame.vidMemTotal, frame.vidMemFree);
                        Gfx_DebugPrint(frame.msgBuffer);
                    }
                }
                CheckDirectDrawResult(((ComDirectDraw *)(frame.ddInterface))->EnumDisplayModes((int)(0), (int)(0), (int)(EngineContextToCallbackParam(context)), (int)((int)DDEnumDisplayModesCallback)), DirectXLit_SourcePath_35(), 1659);
                if (CheckDirectDrawResult(((ComUnknown *)(frame.ddInterface))->QueryInterface(&IID_IDirect3D, &context->direct3D), DirectXLit_SourcePath_36(), 1671) != 0) {
                    CheckDirectDrawResult(((ComDirect3D *)(context->direct3D))->EnumDevices((int)((int)D3DEnumDevicesCallback), (int)(EngineContextToCallbackParam(context))), DirectXLit_SourcePath_37(), 1673);
                }
                // VC5 empty conditional nodes preserve the original register choices.
                if (0)
                    ;
                if (0) /* Disabled in retail (/Od keeps the zero-test and body); see lib/codegen.h. */
                    ;
                if (context->fullScreen != 0) {
                    if (context->matchingDisplayModes <= 0) {
                        Gfx_DebugPrint(
                                                   DirectXLit_NoRequestedDisplayModes());
                    }
                    if (CheckDirectDrawResult(((ComDirectDraw *)(frame.ddInterface))->SetCooperativeLevel((int)(context->hwnd), (int)(DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN)), DirectXLit_SourcePath_38(), 1698) != 0) {
                        Gfx_DebugPrint(
                                                   DirectXLit_ExclusiveMode());
                        if (CheckDirectDrawResult(((ComDirectDraw *)(frame.ddInterface))->SetDisplayMode((int)(context->displayWidth), (int)(context->displayHeight), (int)(context->colorDepth)), DirectXLit_SourcePath_39(), 1713) != 0) { /* empty first arm: lib/codegen.h */
                        } else {
                            return 0;
                        }
                    }
                } else {
                    CheckDirectDrawResult(((ComDirectDraw *)(frame.ddInterface))->SetCooperativeLevel((int)(context->hwnd), (int)(DDSCL_NORMAL)), DirectXLit_SourcePath_40(), 1728);
                }
                if ((frame.driverCapsHw.caps & DDCAPS_NOHARDWARE) != 0) {
                    Gfx_DebugPrint(
                                               DirectXLit_DirectDrawNoHardware());
                    context->modeTransition = 1;
                }
                memset((void *)&frame.attachCaps, 0, 4);
                DDSurfaceDescInit(&frame.primaryDesc);
                frame.primaryDesc.flags = DDSD_CAPS;
                if (context->bufferCount <= 0) {
                    context->bufferCount = 1;
                }
                if (context->bufferCount > 3) {
                    context->bufferCount = 3;
                }
                frame.primaryDesc.width = context->displayWidth;
                frame.primaryDesc.height = context->displayHeight;
                if (context->fullScreen != 0) {
                    if (context->bufferCount > 1) {
                        frame.primaryDesc.flags |= DDSD_BACKBUFFERCOUNT;
                        frame.primaryDesc.caps |= DDSCAPS_COMPLEX | DDSCAPS_FLIP;
                    }
                } else {
                    frame.primaryDesc.caps |= DDSCAPS_FRONTBUFFER;
                }
                frame.primaryDesc.caps |= DDSCAPS_PRIMARYSURFACE;
                if (DEAD_IN_RETAIL != 0) {
                    frame.modeFlag = 0;
                    if ((context->ddHwSurfaceCaps & DDSCAPS_VIDEOMEMORY) != 0) {
                        frame.modeFlag = 1;
                    }
                    if (context->modeTransition != 0) {
                        frame.primaryDesc.caps |= DDSCAPS_SYSTEMMEMORY;
                    } else if (frame.modeFlag != 0) {
                        frame.primaryDesc.caps |= DDSCAPS_VIDEOMEMORY;
                    } else {
                        frame.primaryDesc.caps |= DDSCAPS_SYSTEMMEMORY;
                    }
                } else if (context->modeTransition != 0) {
                    frame.primaryDesc.caps |= DDSCAPS_SYSTEMMEMORY;
                } else {
                    frame.primaryDesc.caps |= DDSCAPS_VIDEOMEMORY;
                }
                if (context->use3DDevice != 0) {
                    frame.primaryDesc.caps |= DDSCAPS_3DDEVICE;
                }
                frame.primaryDesc.backBufferCount = context->bufferCount - 1;
                if (CheckDirectDrawResult(((ComDirectDraw *)(frame.ddInterface))->CreateSurface(&frame.primaryDesc, &context->primarySurface, (int)(0)), DirectXLit_SourcePath_41(), 1826) != 0) {
                    if (context->deviceIndex != 0) {
                        Gfx_DebugPrint(
                                                   DirectXLit_DdFrontSurface());
                        ((ComDirectDrawSurface *)(context->primarySurface))->GetCaps(&frame.primaryCapsOut);
                        Gfx_LogDirectDrawSurfaceCaps((int *)(&frame.primaryCapsOut));
                    }
                    DDSurfaceDescInit(&context->surfaceDescs[0]);
                    CheckDirectDrawResult(((ComDirectDrawSurface *)(context->primarySurface))->GetSurfaceDesc(&context->surfaceDescs[0]), DirectXLit_SourcePath_42(), 1843);
                    if (context->fullScreen != 0) {
                        for (frame.bufferIndex = 1; frame.bufferIndex < context->bufferCount; frame.bufferIndex++) {
                            frame.attachCaps = DDSCAPS_BACKBUFFER;
                            if (CheckDirectDrawResult(((ComDirectDrawSurface *)(context->primarySurface))->GetAttachedSurface(&frame.attachCaps, &context->surfaces[frame.bufferIndex]), DirectXLit_SourcePath_43(), 1855) != 0) {
                                if (context->deviceIndex != 0) {
                                    Gfx_DebugPrint(
                                                               DirectXLit_DdBackSurface());
                                    ((ComDirectDrawSurface *)(context->surfaces[frame.bufferIndex]))->GetCaps(&frame.attachedCapsOut);
                                    Gfx_LogDirectDrawSurfaceCaps((int *)(&frame.attachedCapsOut));
                                }
                                DDSurfaceSetSrcColorKey(context->ddSurface, context->surfaces[frame.bufferIndex]);
                                DDSurfaceDescInit(&context->surfaceDescs[frame.bufferIndex]);
                                CheckDirectDrawResult(((ComDirectDrawSurface *)(context->surfaces[frame.bufferIndex]))->GetSurfaceDesc(&context->surfaceDescs[frame.bufferIndex]), DirectXLit_SourcePath_44(), 1873);
                                frame.initStatus = 1;
                            } else {
                                frame.initStatus = 0;
                                break;
                            }
                        }
                    } else {
                        if (CheckDirectDrawResult(((ComDirectDraw *)(frame.ddInterface))->CreateClipper((int)(0), &context->ddClipper, (int)(0)), DirectXLit_SourcePath_45(), 1891) != 0) {
                            if (CheckDirectDrawResult(((ComDirectDrawClipper *)(context->ddClipper))->SetHWnd((int)(0), (int)(context->hwnd)), DirectXLit_SourcePath_46(), 1893) != 0) {
                                CheckDirectDrawResult(((ComDirectDrawSurface *)(context->primarySurface))->SetClipper((int)(context->ddClipper)), DirectXLit_SourcePath_47(), 1896);
                            }
                        }
                        memset((void *)&frame.attachCaps, 0, 4);
                        DDSurfaceDescInit(&frame.offscreenDesc);
                        frame.offscreenDesc.flags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
                        frame.offscreenDesc.width = context->displayWidth;
                        frame.offscreenDesc.height = context->displayHeight;
                        frame.offscreenDesc.caps |= DDSCAPS_OFFSCREENPLAIN;
                        if (context->use3DDevice != 0) {
                            frame.offscreenDesc.caps |= DDSCAPS_3DDEVICE;
                        }
                        if (context->backBuffersInSystemMemory != 0) {
                            frame.offscreenDesc.caps |= DDSCAPS_SYSTEMMEMORY;
                        } else {
                            frame.offscreenDesc.caps |= DDSCAPS_VIDEOMEMORY;
                        }
                        for (frame.bufferIndex = 1; frame.bufferIndex < context->bufferCount; frame.bufferIndex++) {
                            if (CheckDirectDrawResult(((ComDirectDraw *)(frame.ddInterface))->CreateSurface(&frame.offscreenDesc, &context->surfaces[frame.bufferIndex], (int)(0)), DirectXLit_SourcePath_48(), 1932) != 0) {
                                if (CheckDirectDrawResult(((ComDirectDrawSurface *)(context->primarySurface))->SetClipper((int)(context->ddClipper)), DirectXLit_SourcePath_49(), 1935) != 0) {
                                    DDSurfaceDescInit(&context->surfaceDescs[frame.bufferIndex]);
                                    CheckDirectDrawResult(((ComDirectDrawSurface *)(context->surfaces[frame.bufferIndex]))->GetSurfaceDesc(&context->surfaceDescs[frame.bufferIndex]), DirectXLit_SourcePath_50(), 1938);
                                    DDSurfaceSetSrcColorKey(context->ddSurface, context->surfaces[frame.bufferIndex]);
                                    frame.initStatus = 1;
                                } else {
                                    frame.initStatus = 0;
                                    break;
                                }
                            } else {
                                frame.initStatus = 0;
                                break;
                            }
                        }
                    }
                }
            }
        }
        if ((context->setupFlags & GFX_SETUP_DIRECT3D) != 0) {
            if (frame.initStatus != 0 && context->use3DDevice != 0) {
                frame.initStatus = 0;
                frame.pickedDevice = 0;
                ListForEachValueRecursive((ListVisitFn)D3DPickDeviceCallback, (int)(&frame.pickedDevice), (List *)((context->d3dDeviceList)));
                if (frame.pickedDevice != 0) {
                    frame.initStatus = 1;
                    frame.deviceEntry = (GfxInitContext *)frame.pickedDevice;
                    if (context->modeSetupDone == 0) {
                        frame.zDepthMask = ((GfxD3DDeviceDesc *)frame.deviceEntry->hardwareDesc)->zBitDepth;
                        if (frame.zDepthMask == 0) {
                            context->zBufferEnabled = 0;
                        }
                    }
                    if (context->use3DDevice != 0 && context->zBufferEnabled != 0) {
                        frame.initStatus = 0;
                        if (context->bufferCount > 1) {
                            frame.surfaceIndex = 1;
                        } else {
                            frame.surfaceIndex = 0;
                        }
                        frame.indexSurface = GfxGetSurfaceByIndex(frame.surfaceIndex);
                        if (frame.indexSurface != 0) {
                            DDSurfaceDescInit(&frame.zSurfaceDesc);
                            frame.zSurfaceDesc.flags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_ZBUFFERBITDEPTH;
                            frame.zSurfaceDesc.width = context->displayWidth;
                            frame.zSurfaceDesc.height = context->displayHeight;
                            frame.zSurfaceDesc.caps |= DDSCAPS_ZBUFFER;
                            if (context->modeSetupDone != 0) {
                                frame.zDepthMask = ((GfxD3DDeviceDesc *)frame.deviceEntry->softwareDesc)->zBitDepth;
                                frame.driverTypeIndex = 1;
                            } else {
                                frame.zDepthMask = ((GfxD3DDeviceDesc *)frame.deviceEntry->hardwareDesc)->zBitDepth;
                                frame.driverTypeIndex = 0;
                            }
                            frame.zDepthOk = 0;
                            if (ZBufferDepthSupported(context->zBufferDepth, frame.zDepthMask) != 0) {
                                frame.zDepthOk = 1;
                            }
                            if (frame.zDepthOk != 0) {
                                frame.zSurfaceDesc.zBufferBitDepth = context->zBufferDepth;
                            } else {
                                frame.zSurfaceDesc.zBufferBitDepth = PickZBufferDepth(frame.zDepthMask);
                            }
                            if (frame.driverTypeIndex != 0) {
                                frame.zSurfaceDesc.caps |= DDSCAPS_SYSTEMMEMORY;
                            } else {
                                frame.zSurfaceDesc.caps |= DDSCAPS_VIDEOMEMORY;
                            }
                            if (CheckDirectDrawResult(((ComDirectDraw *)(context->ddObject))->CreateSurface(&frame.zSurfaceDesc, &context->zBufferSurface, (int)(0)), DirectXLit_SourcePath_51(), 2070) != 0) {
                                if (CheckDirectDrawResult(((ComDirectDrawSurface *)(frame.indexSurface))->AddAttachedSurface((int)(context->zBufferSurface)), DirectXLit_SourcePath_52(), 2072) != 0) {
                                    DDSurfaceDescInit(&frame.dxDesc);
                                    // STRING: FF7 0x0090db0c
                                    CheckDirectDrawResult(((ComDirectDrawSurface *)(context->zBufferSurface))->GetSurfaceDesc(&frame.dxDesc), "C:\\lib\\src\\graphics\\directx.cpp", 2086);
                                    sprintf(frame.driverMsgBuffer, (const char *)
                                            // STRING: FF7 0x90db2c
                                            "%d-BIT Z BUFFER CREATED IN %s \n",
                                            frame.dxDesc.zBufferBitDepth, ((int *)&g_GfxDriverTypeNames)[frame.driverTypeIndex]);
                                    Gfx_DebugPrint(frame.driverMsgBuffer);
                                    frame.initStatus = 1;
                                }
                            }
                        }
                    }
                    if (frame.initStatus != 0) {
                        if (GfxCreateD3DDevice((int)frame.deviceEntry, context) != 0) {
                            D3DDeviceDescInit(&frame.hwDeviceDesc);
                            D3DDeviceDescInit(&frame.swDeviceDesc);
                            // STRING: FF7 0x0090db4c
                            if (CheckDirectDrawResult(((ComDirect3DDevice *)(context->d3dDevice))->GetCaps(&frame.hwDeviceDesc, &frame.swDeviceDesc), "C:\\lib\\src\\graphics\\directx.cpp", 2110) != 0) {
                                context->hwDeviceDesc = frame.hwDeviceDesc;
                                context->swDeviceDesc = frame.swDeviceDesc;
                                Gfx_DebugPrint(
                                                       // STRING: FF7 0x90db6c
                                                       "HAL \n");
                                Gfx_LogD3DDeviceDesc((int)(context->deviceIndex), &frame.hwDeviceDesc, (GfxD3dPrimCaps *)((int)context + 0x304), (GfxD3dPrimCaps *)((int)context + 0x30C));
                                Gfx_DebugPrint(
                                                       // STRING: FF7 0x90db74
                                                       "HEL \n");
                                Gfx_LogD3DDeviceDesc((int)(context->deviceIndex), &frame.swDeviceDesc, (GfxD3dPrimCaps *)((int)context + 0x314), (GfxD3dPrimCaps *)((int)context + 0x31C));
                            }
                            frame.initStatus = 0;
                            // STRING: FF7 0x0090db7c
                            if (CheckDirectDrawResult(((ComDirect3DDevice *)(context->d3dDevice))->EnumTextureFormats((int)((int)D3DEnumTextureFormatsCallback), (int)(EngineContextToCallbackParam(context))), "C:\\lib\\src\\graphics\\directx.cpp", 2124) != 0) {
                                Gfx_DebugPrint(
                                                       // STRING: FF7 0x90db9c
                                                       "TEXTURE FORMATS: \n");
                                ForEachListEntry((ListVisitFn)LogTextureFormatRecord, (int)(0), (List *)(context->textureFormatList));
                                context->d3dDeviceRecord = (int)frame.deviceEntry;
                                frame.initStatus = 1;
                            }
                        }
                    }
                    if (context->useDirect3D5 != 0) {
                        if (frame.initStatus != 0) {
                            Gfx_DebugPrint(
                                                   // STRING: FF7 0x90dbb0
                                                   "INITIALIZE D3D 2 \n");
                            if (D3DQueryIDirect3D2((int)(context)) != 0) {
                                frame.initStatus = 1;
                            } else {
                                frame.initStatus = 0;
                            }
                        }
                        if (frame.initStatus != 0) {
                            frame.initStatus = GfxCreateD3DDevice2((int)frame.deviceEntry, context);
                        }
                    }
                }
                if (frame.initStatus == 0) {
                    return frame.initStatus;
                }
                Gfx_DebugPrint(
                                       // STRING: FF7 0x90dbc4
                                       "VIEWPORT INITIALIZE \n");
                D3DCreateAndAttachViewport(context);
                BuildFullscreenD3DViewportData(&frame.viewportData, context);
                SetD3DViewportData((int)(&frame.viewportData), context);
                context->backgroundMaterial = D3DCreateMaterial(context);
                Gfx_DebugPrint(
                                       // STRING: FF7 0x90dbdc
                                       "VIEWPORT 2 INITIALIZE \n");
                D3DCreateAndAttachViewport2(context);
                BuildFullscreenD3DViewport2(&frame.viewport2Data, context);
                SetD3DViewport2Data((int)(&frame.viewport2Data), context);
                if (context->useDirect3D5 != 0 && frame.initStatus != 0) {
                    if (D3D5SetRenderTargetAndViewport((int)(1), (int)(1), (int)(GfxGetSurfaceByIndex(1)), (int)(context->viewport2), context) != 0) {
                        frame.initStatus = 1;
                    } else {
                        frame.initStatus = 0;
                    }
                }
            }
            if (frame.initStatus != 0 && (context->triAlphaShadeCaps & 2) != 0) {
                context->flags |= 1;
            }
        }
        context->deviceReady = frame.initStatus;
        if (frame.initStatus != 0) {
            Gfx_DebugPrint(
                                   // STRING: FF7 0x90dbf4
                                   "LOCK UNLOCK TEST \n");
            if (GfxLockSurfaceCall((int)(4)) != 0) {
                GfxUnlockSurfaceCall((int)(1));
            }
        }
        if (frame.initStatus != 0) {
            Blit_BuildColorRampTables((unsigned int *)(context->rampBlock + 0x9000), (unsigned int *)(context->rampBlock + 0x9410), (unsigned int *)(context->rampBlock + 0x9820), &context->targetFormat);
            context->rampTableR = context->rampBlock + 0x9008;
            context->rampTableG = context->rampBlock + 0x9418;
            context->rampTableB = context->rampBlock + 0x9828;
            frame.driverObject = GetGraphicsDriverObject(context);
            frame.initStatus = frame.driverObject->init(context);
        }
        if (frame.initStatus != 0) {
            frame.bgColor.r = 0;
            frame.bgColor.g = 0;
            frame.bgColor.b = 0;
            frame.bgColor.a = FLOAT_ONE_BITS;
            GfxSetViewportRect((int)(0), (int)(0), (int)(context->displayWidth), (int)(context->displayHeight), context);
            GfxSetBackgroundColorCall(&frame.bgColor, context);
            GfxClearAllCall(context);
            PresentFrame(context);
            GfxClearAllCall(context);
        }
        if (frame.initStatus != 0) {
            frame.initStatus = GfxInitRenderResources(context);
        }
        Gfx_DebugPrint(
                               // STRING: FF7 0x90dc08
                               "INITIALIZE DD/D3D END \n");
    }
    return frame.initStatus;
}

/*
 * Shuts down graphics rendering resources and releases buffers.
 */
// FUNCTION: FF7 0x00679633
void __cdecl GfxShutdownRenderResources(struct GfxEngineContext *context)
{
    int idx;
    MatrixStack *matrixStack;
    int j;

    DestroyBlockHeapNodeList(context->nodeList);
    Gfx_DestroySortedDrawQueue((struct GfxSortedDrawQueue *)(context->depthSortQueue));
    Gfx_FreePresetHundreds(context);
    Gfx_DestroyPresetStateTable(context->presetPolygonSets, (int)context);
    context->presetPolygonSets = 0;
    D3DReleaseDevicePolygonSets(context);
    Gfx_StatsFreeRecord(context->frameStatsRecord);
    context->frameStatsRecord = 0;
    Gfx_StatsFreeRecord((int)context->statsRecord);
    context->statsRecord = 0;
    if (context->renderResourceTable != 0) {
        for (idx = 0; idx < ((MatrixStack *)(context->renderResourceTable))->capacity; idx++) {

            // STRING: FF7 0x0090dc20
            GameHeapFree((void *)((int *)((MatrixStack *)(context->renderResourceTable))->entries)[idx], "C:\\lib\\src\\graphics\\directx.cpp", 2332);
        }
        MatrixStack_Destroy((MatrixStack *)(context->renderResourceTable));
    }
    matrixStack = (MatrixStack *)(context->matrixStack2);
    if (matrixStack != 0) {
        for (j = 0; j < matrixStack->capacity; j++) {
            Gfx_FreeMatrix4x4((void *)((int *)(matrixStack->entries))[j]);
        }
        MatrixStack_Destroy(matrixStack);
    }
    matrixStack = (MatrixStack *)(context->matrixStack);
    if (matrixStack != 0) {
        for (j = 0; j < matrixStack->capacity; j++) {
            Gfx_FreeMatrix4x4((void *)((int *)(matrixStack->entries))[j]);
        }
        MatrixStack_Destroy(matrixStack);
    }
    Gfx_FreeMatrix4x4((void *)context->cameraMatrix);
    Gfx_DestroyDrawPool(context->drawPool);
    context->drawPool = 0;
    Gfx_DestroyPrimitiveMeshBank((struct GfxPrimitiveMeshBank *)(context->renderObjectBank));
    context->renderObjectBank = 0;
    FileRegistryDestroy(context->fileRegistry);
    context->fileRegistry = 0;
    DestroyListAndNull((List **)&context->polygonSetList);
}

/*
 * Tears down what InitGraphicsDevice built, at game exit (App_Shutdown) and from the chocobo and
 * condor device wrappers. It clears the screen to black, presents, clears again, frees the render
 * resources (GfxShutdownRenderResources) and runs the driver's shutdown slot (+0x04). In full
 * screen with modeActive set it flips back to the GDI surface. It then releases the Direct3D
 * objects (context interfaces, background material, viewport, IDirect3DDevice, IDirect3D), the
 * Z buffer and the surface at +0x1D0; in full screen the primary surface (its attached back
 * buffers go with it), in a window every buffer from the last to the first; then IDirectDraw2.
 * The main DirectDraw object (when init read its full-screen display mode) and the primary-display
 * object of the modeActive 0 path get normal cooperative level and their display mode restored
 * before release. Last it destroys the DirectDraw driver, Direct3D device and texture-format
 * lists, unloads an installed driver DLL, frees the driver name and the ops table, and clears
 * initParam.
 */
// FUNCTION: FF7 0x00679864
void __cdecl ShutdownGraphicsDevice(struct GfxEngineContext *context)
{
    struct {
        int index;
        GfxDriverOps *driver;
        GfxClearColorBgra color;
    } f; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    Gfx_DebugPrint(
                           // STRING: FF7 0x0090dc40
                           "UNINITIALIZE DD \n");
    if (context != 0) {
        f.color.r = 0;
        f.color.g = 0;
        f.color.b = 0;
        f.color.a = FLOAT_ONE_BITS;
        GfxSetBackgroundColorCall(&f.color, context);
        GfxClearAllCall(context);
        PresentFrame(context);
        GfxClearAllCall(context);
        GfxShutdownRenderResources(context);
        f.driver = GetGraphicsDriverObject(context);
        f.driver->cleanup(context);
        if (context->modeActive != 0 && context->fullScreen != 0 && context->ddObject != 0)
            ((ComDirectDraw *)(context->ddObject))->FlipToGDISurface();
        D3DReleaseContextInterfaces(context);
        D3DReleaseComSlotAndClear((struct IUnknown **)&context->backgroundMaterial);
        D3DDetachViewport(context);
        if (context->d3dDevice != 0) {
            ((ComUnknown *)(context->d3dDevice))->Release();
            context->d3dDevice = 0;
        }
        if (context->direct3D != 0) {
            ((ComUnknown *)(context->direct3D))->Release();
            context->direct3D = 0;
        }
        if (context->zBufferSurface != 0) {
            ((ComUnknown *)(context->zBufferSurface))->Release();
            context->zBufferSurface = 0;
        }
        if (context->unusedComSurface != 0) {
            CheckDirectDrawResult(((ComUnknown *)(context->unusedComSurface))->Release(),
                                  // STRING: FF7 0x0090dc54
                                  "C:\\lib\\src\\graphics\\directx.cpp", 2477);
            context->unusedComSurface = 0;
        }
        if (context->fullScreen != 0) {
            if (context->primarySurface != 0) {
                CheckDirectDrawResult(((ComUnknown *)(context->primarySurface))->Release(),
                                      // STRING: FF7 0x0090dc74
                                      "C:\\lib\\src\\graphics\\directx.cpp", 2493);
                context->primarySurface = 0;
            }
        } else {
            for (f.index = context->bufferCount - 1; f.index >= 0; f.index--) {
                if (context->surfaces[f.index] != 0) {
                    CheckDirectDrawResult(((ComUnknown *)context->surfaces[f.index])->Release(),
                                          // STRING: FF7 0x0090dc94
                                          "C:\\lib\\src\\graphics\\directx.cpp", 2508);
                    context->surfaces[f.index] = 0;
                }
            }
        }
        if (context->ddObject2 != 0) {
            CheckDirectDrawResult(((ComUnknown *)(context->ddObject2))->Release(),
                                  // STRING: FF7 0x0090dcb4
                                  "C:\\lib\\src\\graphics\\directx.cpp", 2523);
            context->ddObject2 = 0;
        }
        if (context->displayModeSaved != 0 && context->ddObject != 0) {
            CheckDirectDrawResult(((ComDirectDraw *)(context->ddObject))->SetCooperativeLevel(context->hwnd, DDSCL_NORMAL),
                                  // STRING: FF7 0x0090dcd4
                                  "C:\\lib\\src\\graphics\\directx.cpp", 2536);
            CheckDirectDrawResult(((ComDirectDraw *)(context->ddObject))->RestoreDisplayMode(),
                                  // STRING: FF7 0x0090dcf4
                                  "C:\\lib\\src\\graphics\\directx.cpp", 2545);
            context->displayModeSaved = 0;
        }
        if (context->ddObject != 0) {
            ((ComUnknown *)(context->ddObject))->Release();
            context->ddObject = 0;
        }
        if (context->displayDdObject != 0) {
            if (context->displayDdModeSaved != 0) {
                CheckDirectDrawResult(((ComDirectDraw *)(context->displayDdObject))->SetCooperativeLevel(context->hwnd, DDSCL_NORMAL),
                                      // STRING: FF7 0x0090dd14
                                      "C:\\lib\\src\\graphics\\directx.cpp", 2567);
                CheckDirectDrawResult(((ComDirectDraw *)(context->displayDdObject))->RestoreDisplayMode(),
                                      // STRING: FF7 0x0090dd34
                                      "C:\\lib\\src\\graphics\\directx.cpp", 2576);
                context->displayDdModeSaved = 0;
            }
            CheckDirectDrawResult(((ComUnknown *)(context->displayDdObject))->Release(),
                                  // STRING: FF7 0x0090dd54
                                  "C:\\lib\\src\\graphics\\directx.cpp", 2589);
            context->displayDdObject = 0;
        }
        DestroyListAndNull((List **)&context->ddDriverList);
        DestroyListAndNull((List **)&context->textureFormatList);
        DestroyListAndNull((List **)&context->d3dDeviceList);
        if (context->driverInstalled != 0 && context->driverLibrary != 0)
            FreeLibrary(context->driverLibrary);
        GameHeapFree((void *)context->driverName,
                     // STRING: FF7 0x0090dd74
                     "C:\\lib\\src\\graphics\\directx.cpp", 2616);
        GfxFreeDriverOps(context->driverObject);
        context->deviceReady = 0;
        Gfx_DebugPrint(
                               // STRING: FF7 0x0090dd94
                               "END UNINITIALIZE DD \n");
    }
}

/*
 * Locks specified DirectDraw surface for direct CPU pixel access.
 */
// FUNCTION: FF7 0x00679d12
int __cdecl GfxLockSurface(int index)
{
    struct {
        int errors;
        int rect[4];
        int flags;
        GfxSurfaceLockDesc *descriptor;
        GfxEngineContext *context;
        int special;
    } f; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    f.special = 0;
    if (index == 4) {
        index = 1;
        f.special = 1;
    }
    f.descriptor = 0;
    f.context = GetEngineContext();
    if (f.context != 0) {
        if (f.context->surfaces[index] != 0) {
            f.errors = 1;
            f.flags = 0;
            if (f.context->fullScreen != 0) {
                f.rect[0] = 0;
                f.rect[1] = 0;
                f.rect[2] = f.context->displayWidth - 1;
                f.rect[3] = f.context->displayHeight - 1;
            } else {
                f.rect[0] = 0;
                f.rect[1] = 0;
                f.rect[2] = f.context->clientWidth - 1;
                f.rect[3] = f.context->clientHeight - 1;
            }
            f.context->surfaceDescs[index].size = 108;
            f.context->surfaceDescs[index].pixelFormatSize = 32;
            f.errors = 0;
            if (f.context->fullScreen != 0) {
                if (((ComDirectDrawSurface *)f.context->primarySurface)->IsLost() == DDERR_SURFACELOST) {
                    f.errors |= CheckDirectDrawResult(((ComDirectDrawSurface *)f.context->primarySurface)->Restore(),
                                                      // STRING: FF7 0x0090ddac
                                                      "C:\\lib\\src\\graphics\\directx.cpp", 2678);
                }
            } else {
                if (((ComDirectDrawSurface *)f.context->surfaces[index])->IsLost() == DDERR_SURFACELOST) {
                    f.errors |= CheckDirectDrawResult(((ComDirectDrawSurface *)f.context->surfaces[index])->Restore(),
                                                      // STRING: FF7 0x0090ddcc
                                                      "C:\\lib\\src\\graphics\\directx.cpp", 2685);
                }
            }
            if (f.errors == 0) {
                if (CheckDirectDrawResult(((ComDirectDrawSurface *)f.context->surfaces[index])->Lock(f.rect, &f.context->surfaceDescs[index], 1, f.flags),
                                          // STRING: FF7 0x0090ddec
                                          "C:\\lib\\src\\graphics\\directx.cpp", 2698)) {
                    f.descriptor = (GfxSurfaceLockDesc *)&f.context->surfaceDescs[index];
                    f.context->targetFormat.width = f.descriptor->width;
                    f.context->targetFormat.height = f.descriptor->height;
                    f.context->targetFormat.pitch = f.descriptor->pitch;
                    f.context->targetFormat.pixels = f.descriptor->pixels;
                    if (f.special != 0)
                        PixelFormatFromDDPixelFormat((GfxDdPixelFormat *)((char *)f.descriptor + 0x48), &f.context->targetFormat);
                }
            }
        }
    }
    return (int)f.descriptor;
}

/*
 * Unlocks specified DirectDraw surface after pixel access.
 */
// FUNCTION: FF7 0x00679f6e
int __cdecl GfxUnlockSurface(int index)
{
    struct {
        GfxEngineContext *context;
        int success;
    } f; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    f.success = 0;
    f.context = GetEngineContext();
    if (f.context != 0) {
        if (f.context->surfaces[index] != 0) {
            if (CheckDirectDrawResult(((ComDirectDrawSurface *)f.context->surfaces[index])->Unlock(*(int *)((char *)&f.context->surfaceDescs[index] + 0x24)),
                                      // STRING: FF7 0x0090de0c
                                      "C:\\lib\\src\\graphics\\directx.cpp", 2731))
                f.success = 1;
        }
    }
    return f.success;
}

/*
 * Sets source color key on DirectDraw surface.
 */
// FUNCTION: FF7 0x00679fe9
int __cdecl DDSurfaceSetSrcColorKey(int value, int surface)
{
    struct {
        int key[2];
        int success;
        int flags;
    } f;
    f.success = 0;
    if (surface != 0) {
        f.flags = 0;
        f.flags |= 8;
        f.key[0] = value;
        f.key[1] = value;
        if (CheckDirectDrawResult(((ComDirectDrawSurface *)surface)->SetColorKey(f.flags, f.key),
                                  // STRING: FF7 0x0090de2c
                                  "C:\\lib\\src\\graphics\\directx.cpp", 2761)) {
            f.success = 1;
        }
    }
    return f.success;
}
} /* extern "C" */
