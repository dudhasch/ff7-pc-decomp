// src/ff7/coaster/coaster_module_entry_frame.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Module entry, exit and per-frame loop of the roller coaster minigame (Speed Square):
//   sets up the camera and viewport, advances the simulation tick, handles input messages and debug
//   text, and returns to the field with the score.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/types/mem.h"
#include "ff7/types/coaster.h"
#include "lib/codegen.h"
#include "ff7/coaster_psxdata.h"
#include "ff7/coaster_renderer.h"
#include <windows.h>
#include <ddraw.h>
#include "ff7/api/coaster.h"
#include "ff7/api/main.h"
#include "lib/api/graphics.h"
#include "lib/api/mem.h"
#include "lib/enums/graphics.h"
#include "lib/types/graphics.h"
#include "ff7/enums/main.h"
#include "lib/api/file.h"
#include "lib/crt.h"
#include <time.h>
#include "ff7/savemap.h"

struct GfxEngineContext;
extern "C" {


// clang-format off
// GLOBAL: FF7 0x0090140c
int g_CoasterFirstFrame = 1;
// GLOBAL: FF7 0x00c3f6e8
float g_CoasterAccumulatedTime = 0;
// GLOBAL: FF7 0x00c3f6ec
float g_CoasterSimulatedTime = 0;
// clang-format on


/*
 * Outputs debug text for the roller coaster minigame.
 */
// FUNCTION: FF7 0x005e8a70
void __cdecl Mini_CoasterDebugTextOut(int x, int y, char *text, struct GfxEngineContext *ctx)
{
    HDC hdc;
    (*(IDirectDrawSurface **)&ctx->backSurface)->GetDC(&hdc);
    SetBkColor(hdc, 0);
    SetTextColor(hdc, 0xffff);
    TextOutA(hdc, x, y, text, lstrlenA(text));
    (*(IDirectDrawSurface **)&ctx->backSurface)->ReleaseDC(hdc);
}

/*
 * Configures the roller coaster 3D camera projection and view matrix.
 */
// FUNCTION: FF7 0x005e8aec
void __cdecl Mini_CoasterSetupCamera(struct GfxEngineContext *ctx)
{
    struct {
        float view[16];
        float world[16];
    } local; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    ctx->fov = 45.0f;
    ctx->nearPlane = 10.0f;
    ctx->farPlane = 14300.0f;
    ctx->offsetX = 0.0f;
    Mini_CoasterSetViewport(ctx);
    ctx->offsetY = g_CoasterDisplayScaleY * 40.0f;
    SetIdentityMatrix4x4(local.view);
    Gfx_SetContextMatrix((void *)local.view, ctx);
    SetIdentityMatrix4x4(local.world);
    Gfx_RebuildContextScreenTransform(local.world, ctx);
    SetPerspective(ctx->fov, ctx->nearPlane, ctx->farPlane, ctx->offsetX, ctx->offsetY, (float)ctx->viewSizeX, (float)ctx->viewSizeY, ctx);
}

/*
 * Sets up the rendering viewport and projection parameters for the coaster minigame.
 */
// FUNCTION: FF7 0x005e8bde
void __cdecl Mini_CoasterSetViewport(GfxEngineContext *ctx)
{
    int val;

    val = GetUiScalingMode();
    switch (val) {
    case MAIN_GRAPHICS_MODE_320X240:
        GfxSetViewportRect(0, 0, 320, 240, ctx);
        g_CoasterDisplayScaleY = 1;
        g_coasterSubVolumeFactor = g_CoasterDisplayScaleY;
        break;
    case MAIN_GRAPHICS_MODE_320X240_CENTERED:
        GfxSetViewportRect(160, 120, 320, 240, ctx);
        g_CoasterDisplayScaleY = 1;
        g_coasterSubVolumeFactor = g_CoasterDisplayScaleY;
        break;
    case MAIN_GRAPHICS_MODE_640X480:
        GfxSetViewportRect(0, 0, 640, 480, ctx);
        g_CoasterDisplayScaleY = 2;
        g_coasterSubVolumeFactor = g_CoasterDisplayScaleY;
        break;
    default:
        GfxSetViewportRect(0, 0, 320, 240, ctx);
        g_CoasterDisplayScaleY = 1;
        g_coasterSubVolumeFactor = g_CoasterDisplayScaleY;
    }
    g_CoasterClipLeft = ctx->clipLeft;
    g_CoasterClipTop = ctx->clipTop;
    g_CoasterReticleScaledSize = g_coasterSubVolumeFactor << 5;
}

/*
 * Sets the background colour and clears rendering buffers.
 */
// FUNCTION: FF7 0x005e8d03
void __cdecl Mini_SetBackgroundColorAndClear(int red, int green, int blue, int alpha)
{
    GfxEngineContext *engine;
    GfxClearColorBgra bgColor;

    engine = GetEngineContext();
    bgColor.r = red;
    bgColor.g = green;
    bgColor.b = blue;
    bgColor.a = alpha;
    GfxSetBackgroundColorCall(&bgColor, engine);
    GfxClearAllCall(engine);
}

/*
 * Enters the Speed Square shooting ride (game mode GAME_MODE_JET_COASTER), the module's installed
 * enter callback. Sets matrix layout mode 0 and sets up the camera (field of view 45, near and
 * far planes 10 and 14300, the viewport by UI scaling mode: 320x240 for modes 0 and 1, 640x480
 * for mode 2). Turns all 22 render states (0..0x15) off, then turns depth test, depth write,
 * dithering and perspective correction on. Last, `Mini_CoasterInit` loads the ride and starts
 * the music.
 */
// FUNCTION: FF7 0x005e8d49
void __cdecl Mini_CoasterEnter(struct GfxEngineContext *context)
{
    int i;

    SetMatrixLayoutMode(0);
    Mini_CoasterSetupCamera(context);
    for (i = 0; i < 22; i++) {
        Gfx_SetRenderStateCall(i, 0, (struct GfxEngineContext *)context);
    }
    Gfx_SetRenderStateCall(RENDER_STATE_DEPTH_TEST, 1, (struct GfxEngineContext *)context);
    Gfx_SetRenderStateCall(RENDER_STATE_DEPTH_WRITE, 1, (struct GfxEngineContext *)context);
    Gfx_SetRenderStateCall(RENDER_STATE_DITHER, 1, (struct GfxEngineContext *)context);
    Gfx_SetRenderStateCall(RENDER_STATE_PERSPECTIVE, 1, (struct GfxEngineContext *)context);
    Mini_CoasterInit();
}

/*
 * Leaves the ride, the module's installed exit callback, which the dispatcher runs after
 * `Mini_CoasterReturnToField` requested the switch. Clears the screen, presents that blank frame
 * and clears the back buffer again so no ride image is left on screen, then `Mini_CoasterShutdown`
 * deletes the renderer, closes coaster.lgp, releases the sounds and data and restores the
 * working directory.
 */
// FUNCTION: FF7 0x005e8dd8
void __cdecl Mini_CoasterExit(struct GfxEngineContext *context)
{
    GfxClearAllCall(context);
    PresentFrame(context);
    GfxClearAllCall(context);
    Mini_CoasterShutdown();
}

/*
 * Empty module hook for coaster event handling.
 */
// FUNCTION: FF7 0x005e8e06
void __cdecl Mini_CoasterEmptyHook(void)
{
    /* The original is empty. */
}

/*
 * Ends the ride and returns to the Gold Saucer field. Writes the ride's score g_CoasterScore to
 * the savemap as two bytes, coasterScoreLowByte and coasterScoreHighByte;
 * sets the previous game mode to GAME_MODE_JET_COASTER and the game mode to GAME_MODE_FIELD;
 * and requests a module switch to the `GameModeDispatcher_*`
 * callbacks with the app's mouse and key handlers. The dispatcher then calls
 * `Mini_CoasterExit`. Called by `Mini_CoasterFrame` when the ride is over.
 */
// FUNCTION: FF7 0x005e8e0b
void __cdecl Mini_CoasterReturnToField(struct GfxEngineContext *context)
{
    GameModule mod;

    gSavemap.coasterScoreLowByte = g_CoasterScore & 0xFF;
    gSavemap.coasterScoreHighByte = g_CoasterScore >> 8 & 0xFF;
    g_EnginePreviousMode = GAME_MODE_JET_COASTER;
    g_EngineGameMode = GAME_MODE_FIELD;
    mod.frame = GameModeDispatcher_Frame;
    mod.onMouse = App_OnMouseMessage;
    mod.onKey = App_OnKeyMessage;
    mod.enter = GameModeDispatcher_Enter;
    mod.exit = GameModeDispatcher_Exit;
    RequestModuleSwitch(&mod, context);
}

/*
 * The ride's installed frame callback. Clears the screen to black, then:
 * - when the ride is over, stops the music and returns to the field. The ride is over when four
 *   times the track cursor passes the track length (g_CoasterTrackLength, the first word of
 *   the track data) minus 16, or when the closing fade (actor behaviour 0xFD) has set
 *   g_CoasterRideFinished;
 * - otherwise adds the elapsed time to the tick budget (60 ticks per second, at most 32 per
 *   frame), polls input and, if the scene begins, resets the frame's draw pools, runs every owed
 *   tick with g_CoasterDrawThisTick 0 (logic only), then one more tick with it 1 so that tick
 *   draws, and ends the scene. Calls the empty `Mini_CoasterEmptyHook` either way.
 *
 * Codegen: the scene test reads eax after the call to `Gfx_BeginSceneCall`, which is declared
 * void, as the original does.
 */
// FUNCTION: FF7 0x005e8e7e
void __cdecl Mini_CoasterFrame(struct GfxEngineContext *ctx)
{
    Mini_SetBackgroundColorAndClear(0, 0, 0, FLOAT_ONE_BITS);
    if (g_CoasterTrackCursor * 4 > g_CoasterTrackLength - 16 || (*(signed char *)&g_CoasterRideFinished) == 1) {
        Mini_CoasterStopMusic();
        Mini_CoasterReturnToField(ctx);
    } else {
        Mini_CoasterAccumulateStepTarget(ctx);
        Input_Update(ctx);
        if (((int (__cdecl *)(int scene, void *context))Gfx_BeginSceneCall)(0, (void *)ctx)) { /* reads eax after a void call, as the original does */
            ResetFrameDrawPool(ctx);
            ResetRenderPools(ctx);
            Gfx_ResetNodeList(ctx);
            while (g_CoasterSimulatedTime + 1.0f < g_CoasterAccumulatedTime) {
                g_CoasterDrawThisTick = 0;
                Mini_CoasterTick(ctx);
                g_CoasterSimulatedTime += 1.0f;
            }
            g_CoasterDrawThisTick = 1;
            Mini_CoasterTick(ctx);
            g_CoasterSimulatedTime += 1.0f;
            Gfx_EndSceneCall(ctx);
        }
        Mini_CoasterEmptyHook();
    }
}

// GLOBAL: FF7 0x00c3f6e0
Timestamp64 g_coasterLastTimestamp;

/*
 * Accumulates frame time and steps using high-resolution timestamp counter.
 */
// FUNCTION: FF7 0x005e8f9b
void __cdecl Mini_CoasterAccumulateStepTarget(struct GfxEngineContext *ctx)
{
#pragma pack(1)
    struct {
        float step;
        double delay;
        Timestamp64 diff;
        Timestamp64 now;
    } local; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
#pragma pack()

    if (g_CoasterFirstFrame) {
        ReadTimestampCounter(&g_coasterLastTimestamp);
        g_CoasterAccumulatedTime = 0.0f;
        g_CoasterSimulatedTime = 0.0f;
        g_CoasterFirstFrame = 0;
        return;
    }

    ReadTimestampCounter(&local.now);
    Sub64(&local.now, &g_coasterLastTimestamp, &local.diff);
    local.delay = CombineSplitValueToFloat(&local.diff) / ctx->tscFrequency;
    local.step = (float)(local.delay * 60.0);
    if (local.step >= 32.0f)
        local.step = 32.0f;
    g_CoasterAccumulatedTime += local.step;
    g_coasterLastTimestamp = local.now;
}

/*
 * Runs one 60 Hz tick of the ride; `Mini_CoasterFrame` calls it once per owed tick. Polls the
 * reticle, fire and pause input. While not paused (g_CoasterPaused clear): builds the camera
 * matrix, moves the car along the track (the slope changes the speed), advances the track cursor
 * by the ride speed, streams the rail and scenery, updates the score popup for the last target
 * hit (g_CoasterKillScore holds its popup model), retires passed stream entries, runs the
 * actors (spawn script, behaviours, beam hits) and draws the HUD; the engine loop plays at
 * volume 0x7F when the speed is at least 0x4000, else at 0. While paused it draws the pause HUD
 * and mutes the engine and beam loops. Then sends both loop volumes to the sound driver, and
 * while g_CoasterRideActive is set (from the opening fade, behaviour 0xFC, until the closing
 * fade 0xFD ends) submits the renderer's batch lists, which happens only on the drawing tick.
 */
// FUNCTION: FF7 0x005e9051
void __cdecl Mini_CoasterTick(struct GfxEngineContext *context)
{
    Mini_CoasterPollInput();
    if (g_CoasterPaused == 0) {
        Mini_CoasterBuildCameraMatrix((struct PsxMatrix *)&g_CoasterCameraMatrixBuf);
        Mini_CoasterAdvanceRide();
        Mini_CoasterAdvanceTrackCursor(g_CoasterRideSpeed);
        Mini_CoasterDrawRailStream();
        Mini_CoasterDrawSceneryStream();
        Mini_CoasterUpdateScorePopup((short)g_CoasterKillScore, 5, 0x28, 0);
        Mini_CoasterRetireStreamedEntries();
        Mini_CoasterUpdateActors();
        Mini_CoasterDrawHud();
        if (g_CoasterRideSpeed < 16384) {
            g_CoasterEngineVolume = 0;
        } else {
            g_CoasterEngineVolume = 0x7F;
        }
    } else {
        Mini_CoasterDrawPausedHud();
        g_CoasterEngineVolume = 0;
        g_CoasterBeamLoopVolume = 0;
    }
    Mini_CoasterUpdateLoopVolumes();
    if (g_CoasterRideActive != 0) {
        Mini_CoasterSubmitRendererBatches();
    }
}

/*
 * The ride's installed mouse callback. It does nothing: the reticle and beam are driven by the
 * input `Mini_CoasterPollInput` reads each tick.
 */
// FUNCTION: FF7 0x005e9108
void __cdecl Mini_CoasterMouse(void)
{
    /* The original is empty. */
}

/*
 * The ride's installed keyboard callback. It has no effect: on WM_KEYDOWN (0x100) it switches on
 * the key with only an empty default case, and on WM_CHAR (0x102) it copies the character into
 * a local nothing reads. The ride's controls come from `Mini_CoasterPollInput`.
 */
// FUNCTION: FF7 0x005e910d
void __cdecl Mini_CoasterKeyboard(int message, int key)
{
    int keyCode;
    char charCode;
    switch (message) {
    case WM_KEYDOWN:
        keyCode = key;
        switch (keyCode) {
        default:
            break;
        }
        break;
    case WM_CHAR:
        charCode = (char)key;
        break;
    }
}
} /* extern "C" */
