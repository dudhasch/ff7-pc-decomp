// src/ff7/highway/highway_minigame_lifecycle.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Controls the highway minigame's lifecycle: enter and exit, returning to the field,
//   background setup, the per-frame update, and the mouse and keyboard input handlers, which are
//   stubs.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/codegen.h"
#include "lib/types/graphics.h"
#include "ff7/enums/main.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "ff7/api/main.h"
#include "ff7/api/highway.h"
#include "ff7/types/highway.h"
#include "lib/api/file.h"
#include "lib/api/graphics.h"
extern "C" {

#pragma intrinsic(abs)

// clang-format off
// GLOBAL: FF7 0x00d857a0
char g_HighwaySavedDirectory[260] = {0};
// GLOBAL: FF7 0x00d858a4
int g_HighwayFrameCounter = 0;
// Highway run state block up to 0xD858E8: highway_minigame_frame_loop.cpp's .bss starts with its
// dynamically initialized g_HighwayPsxDataSet, which VC5 emits ahead of zero-initialized globals,
// so the preceding .bss bytes belong to this TU in link order.
// GLOBAL: FF7 0x00d858a8
struct HighwayEffectStatusTable g_HighwayHpBarTable = {0};
// GLOBAL: FF7 0x00d858dc
int g_HighwayCountdown = 0;
// GLOBAL: FF7 0x00d858e0
struct HighwayFileBlock3 *g_pHighwayFileBlock = 0;
// GLOBAL: FF7 0x00d858e4
char *g_HighwayDataDirectory = 0;
// clang-format on

#pragma intrinsic(pow)
#pragma intrinsic(sqrt)
// The TU's own view of the moved definition, declared with another type before.

#pragma intrinsic(memcpy)
#pragma function(memcpy)

/*
 * Entry point for the highway minigame: zeroes frame counter, switches to minigame
 * directory saving the previous directory, and initializes highway state.
 */
// FUNCTION: FF7 0x00650310
void __cdecl Mini_HighwayEnter(void)
{
    char *rec;

    g_HighwayFrameCounter = 0;
    rec = GetMinigameDirForHighway();
    ChangeDirectorySaving(rec, g_HighwaySavedDirectory);
    Mini_HighwayInit();
}

/*
 * Leaves the motorcycle chase, the module's installed exit callback, which the dispatcher runs
 * after `Mini_HighwayReturnToField` requested the switch. Clears the screen, presents the blank
 * frame and clears again, then `Mini_Mode6ExitStoreScores` stops the music and sounds, writes the
 * scores to the savemap (high and last score in variant 1, the five 16-bit scores of
 * g_HighwayHpBarTable into script bank 1 in variant 0), frees every block and the data
 * set and closes high-us.lgp when it was opened. Last, changes back to the working
 * directory `Mini_HighwayEnter` saved in g_HighwaySavedDirectory.
 */
// FUNCTION: FF7 0x00650340
void __cdecl Mini_HighwayExit(struct GfxEngineContext *context)
{
    GfxClearAllCall(context);
    PresentFrame(context);
    GfxClearAllCall(context);
    Mini_Mode6ExitStoreScores();
    ChangeDirectorySaving(g_HighwaySavedDirectory, 0);
}

/*
 * Configures mode switch callbacks and requests a return to the main field engine.
 */
// FUNCTION: FF7 0x0065037d
void __cdecl Mini_HighwayReturnToField(struct GfxEngineContext *context)
{
    GameModule switchParams;
    g_EnginePreviousMode = GAME_MODE_HIGHWAY;
    g_EngineGameMode = GAME_MODE_FIELD;
    switchParams.frame = GameModeDispatcher_Frame;
    switchParams.onMouse = App_OnMouseMessage;
    switchParams.onKey = App_OnKeyMessage;
    switchParams.enter = GameModeDispatcher_Enter;
    switchParams.exit = GameModeDispatcher_Exit;
    RequestModuleSwitch(&switchParams, context);
}

/*
 * Sets background color on the graphics engine context and clears the screen buffer.
 */
// FUNCTION: FF7 0x006503cc
void __cdecl Mini_HighwaySetBackgroundColorAndClear(int red, int green, int blue, int alpha)
{
    GfxEngineContext *context;
    GfxClearColorBgra bgColor;

    context = GetEngineContext();
    bgColor.r = red;
    bgColor.g = green;
    bgColor.b = blue;
    bgColor.a = alpha;
    GfxSetBackgroundColorCall(&bgColor, context);
    GfxClearAllCall(context);
}

/*
 * The chase's installed frame callback. When `Mini_HighwayInit` failed (g_HighwayRunningFlag
 * still 0) it only returns to the field. Otherwise it counts the frame, clears the screen to
 * black, polls the input and takes the highway's snapshot of it (`Mini_HighwayReadInput`), runs
 * and draws the frame (`Mini_HighwayRunFrame`: event timeline, track scroll, actors, layers,
 * track, effects, HUD, scene, fade), clears the snapshot's action, direction and button mask,
 * and returns to the field once the event timeline's ending sequence has set
 * g_HighwayExitRequested.
 */
// FUNCTION: FF7 0x00650412
void __cdecl Mini_HighwayFrame(struct GfxEngineContext *context)
{
    if (g_HighwayRunningFlag == 0) {
        Mini_HighwayReturnToField(context);
    } else {
        g_HighwayFrameCounter++;
        Mini_HighwaySetBackgroundColorAndClear(0, 0, 0, FLOAT_ONE_BITS);
        Input_Update(context);
        Mini_HighwayReadInput();
        Mini_HighwayRunFrame(context);
        g_HighwayInputButtonMask = 0;
        g_HighwayInputDirection = g_HighwayInputButtonMask;
        g_HighwayInputAction = g_HighwayInputDirection;
        if (g_HighwayExitRequested != 0) {
            Mini_HighwayReturnToField(context);
        }
    }
}

/*
 * The chase's installed mouse callback. It does nothing: the chase reads its controls from the
 * input snapshot `Mini_HighwayFrame` takes each frame.
 */
// FUNCTION: FF7 0x006504a7
void __cdecl Mini_HighwayMouse(void)
{
    // The original is empty (five code bytes plus any alignment padding).
}

/*
 * The chase's installed keyboard callback. It does nothing: the chase reads its controls from the
 * input snapshot `Mini_HighwayFrame` takes each frame.
 */
// FUNCTION: FF7 0x006504ac
void __cdecl Mini_HighwayKeyboard(void)
{
    // The original is empty (five code bytes plus any alignment padding).
}
} /* extern "C" */
