// src/lib/src/graphics/graphics_app_entry_point.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: The Windows entry point: it bootstraps the application, creates the engine context and
//   game window, and runs the message loop.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/types/graphics.h"
#include "lib/codegen.h"
/* Retail WinMain entry point dispatches into App_Bootstrap and App_Shutdown. */
#include "ff7/api/main.h"
#include "lib/api/graphics.h"
#include "lib/api/mem.h"
#include <stdlib.h>
#include <time.h>
#include <windows.h>
extern "C" {

#pragma optimize("p", on)
// The optimize("p") pairs in this file set /Op per function, as the retail build did (lib/codegen.h, "/Op pairs").
#pragma optimize("p", off)

/*
 * The process entry point: runs the whole game from start to exit. It calls App_Bootstrap, which
 * builds the engine context and installs the game's callbacks; only when no context is active
 * does it create a default one (CreateEngineContext). It makes the context active, hands it the
 * 0xAC30-byte stack buffer `scratch` (InstallScratchArena zeroes it and points rampBlock, where
 * InitGraphicsDevice later builds the colour ramp tables, at its first 4 KB boundary) and seeds
 * rand() from the context's randomSeed, or from time() when that is 0.
 *
 * When CreateGameWindow succeeds (it also runs the init and enter callbacks), it sets
 * SPI_SCREENSAVERRUNNING (which on Windows 9x disables Ctrl+Alt+Del
 * and Alt+Tab) and loops until the context's quitFlag is set (WM_CLOSE):
 * - a waiting window message is translated and dispatched to GameWindowProc;
 * - otherwise, while a quit request is pending (pendingWindowEvent, set by Ctrl+Q in
 *   GameWindowProc), it runs the quitConfirmRun callback (the menu's quit dialog) once the window
 *   is not minimised and RestoreLostSurfaces succeeds, then clears the request;
 * - otherwise it runs one frame: frameCount += 1, ServiceModuleSwitch, the active module's frame
 *   callback (only when not minimised and RestoreLostSurfaces succeeds; tickCount counts these
 *   runs, and UpdateFrameTiming turns it into frames per second), PresentFrame unless minimised,
 *   and the frame's timestamp-counter delta into the stats record's timeF.
 * While paused or minimised it blocks in WaitMessage after each pass.
 *
 * On quit it runs the exit callback, releases every texture-cache surface, runs the shutdown
 * callback (App_Shutdown, which shuts the graphics device down), clears SPI_SCREENSAVERRUNNING,
 * destroys the window and returns the context's result as the exit code. DestroyEngineContext
 * runs in every case; the return value is 0 when the window could not be created.
 */
// FUNCTION: FF7 0x0067db30
int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, LPSTR cmdLine, int cmdShow)
{
    // Names preserve VC5's stack-slot order: scratch arena, context, MSG, exit code,
    // bootstrap arguments, random seed, system-parameter output, end/start timestamps.
    int result;
    MSG rawMsg;
    GfxEngineContext *ctx;
    char scratch[0xAC30];
    int bootBits;
    int selBootArg;
    long seed;
    int output;
    Timestamp64 startTime;
    Timestamp64 closingStamp;

    bootBits = 0;
    selBootArg = 0;
    App_Bootstrap(bootBits, selBootArg);
    result = 0;
    ctx = GetEngineContext();
    if (ctx == 0) {
        ctx = (GfxEngineContext *)CreateEngineContext();
    }
    SetEngineContext((int)ctx);
    InstallScratchArena((int)scratch, ctx);
    if (ctx->randomSeed != 0) {
        srand(ctx->randomSeed);
    } else {
        time(&seed);
        srand(seed);
    }
    if (ctx != 0) {
        ctx->hInstance = (int)hInstance;
    }
    if (ctx != 0 && CreateGameWindow((int)hInstance, ctx) != 0) {
        SystemParametersInfoA(SPI_SCREENSAVERRUNNING, TRUE, &output, 0);
        while (ctx->quitFlag == 0) {
            if (PeekMessageA(&rawMsg, 0, 0, 0, PM_REMOVE) != 0) {
                TranslateMessage(&rawMsg);
                DispatchMessageA(&rawMsg);
            } else if (ctx->pendingWindowEvent != 0) {
                if (ctx->minimized == 0 && RestoreLostSurfaces(ctx) != 0) {
                    if (ctx->quitConfirmRun != 0) {
                        ctx->quitConfirmRun((int)ctx);
                    }
                    ctx->pendingWindowEvent = 0;
                }
                if (ctx->paused != 0 || ctx->minimized != 0) {
                    WaitMessage();
                }
            } else {
                ReadTimestampCounter(&startTime);
                ctx->frameCount = ctx->frameCount + 1.0;
                ServiceModuleSwitch(ctx);
                if (ctx->minimized == 0 && RestoreLostSurfaces(ctx) != 0 && ctx->activeModule.frame != 0) {
                    ctx->activeModule.frame(ctx);
                    ctx->framesThisSecond = ctx->framesThisSecond + 1.0;
                }
                if (ctx->minimized == 0) {
                    PresentFrame(ctx);
                }
                if (ctx->paused != 0 || ctx->minimized != 0) {
                    WaitMessage();
                }
                ReadTimestampCounter(&closingStamp);
                if (ctx->statsRecord != 0) {
                    Sub64(&closingStamp, &startTime, &ctx->statsRecord->timeF);
                }
            }
        }
        if (ctx->activeModule.exit != 0) {
            ctx->activeModule.exit(ctx);
        }
        Gfx_TextureCacheReleaseAllSurfaces();
        if (ctx->activeModule.shutdown != 0) {
            ctx->activeModule.shutdown(ctx);
        }
        SystemParametersInfoA(SPI_SCREENSAVERRUNNING, FALSE, &output, 0);
        DestroyWindow((HWND)ctx->hwnd);
        result = ctx->result;
    }
    DestroyEngineContext();
    return result;
}
} /* extern "C" */
