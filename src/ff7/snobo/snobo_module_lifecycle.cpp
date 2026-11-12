// src/ff7/snobo/snobo_module_lifecycle.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: The lifecycle of the snowboard minigame: entering and leaving it, returning to the
//   field, the per-frame tick, and the render-state and background-clear helpers. The unused mouse
//   and keyboard message handlers are kept here too.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/codegen.h"
#include "ff7/snobo_course.h"
#include <stdlib.h>
#include "lib/crt.h"
#include "ff7/enums/main.h"
#include "lib/enums/graphics.h"
#include "lib/types/graphics.h"
#include <math.h>
#include <string.h>
#include "ff7/api/snobo.h"
#include "ff7/api/main.h"
#include "lib/api/graphics.h"
#include "lib/api/file.h"
#include "lib/pfile.h"

#pragma intrinsic(abs)
#pragma intrinsic(memset)
extern "C" {


#pragma intrinsic(abs)
#pragma intrinsic(cos)
#pragma intrinsic(sin)
#pragma intrinsic(sqrt, atan2)

// clang-format off
// GLOBAL: FF7 0x00dd7c30
int g_SnoboInitialized = 0;
unsigned char g_bssPad_00dd7c34[4] = {0};
// clang-format on


/*
 * Enters the snowboard minigame, the installed enter callback of both snowboard game modes. Sets
 * g_SnoboModeFlag to 1 for GAME_MODE_SNOWBOARD (0x08), which runs the for_ev.tmd course and the
 * Alt effect set, and to 0 for any other mode (GAME_MODE_SNOWBOARD_2, 0x0E), which runs
 * for_gs.tmd and picks course 0, 1 or 2 from the savemap. Sets matrix layout mode 0 and stores
 * the result of `Mini_SnowboardInit` (opens snowboard-us.lgp, loads the course, creates the
 * renderer and HUD; 0 on failure) in g_SnoboInitialized, which makes the first frame return to
 * the field when it is 0.
 */
// FUNCTION: FF7 0x00722c10
void __cdecl Mini_SnowboardEnter(struct GfxEngineContext *ctx)
{
    if (g_EngineGameMode == GAME_MODE_SNOWBOARD) {
        g_SnoboModeFlag = 1;
    } else {
        g_SnoboModeFlag = 0;
    }
    SetMatrixLayoutMode(0);
    g_SnoboInitialized = Mini_SnowboardInit(ctx);
}

/*
 * Leaves the snowboard minigame, the installed exit callback, which the dispatcher runs after
 * `Mini_SnowboardReturnToField` requested the switch. Clears the screen, presents the blank frame
 * and clears again, then `Mini_SnowboardShutdown` stores the result in the savemap (best times
 * and scores in mode flag 0, one course result byte in mode flag 1), releases the sounds, text
 * buffer and course objects, closes archive 10 and restores the working directory.
 */
// FUNCTION: FF7 0x00722c52
void __cdecl Mini_SnowboardExit(struct GfxEngineContext *context)
{
    GfxClearAllCall(context);
    PresentFrame(context);
    GfxClearAllCall(context);
    Mini_SnowboardShutdown();
}

/*
 * Sets engine state back to field mode and requests module switch.
 */
// FUNCTION: FF7 0x00722c80
void __cdecl Mini_SnowboardReturnToField(struct GfxEngineContext *context)
{
    GameModule moduleTable;
    g_EnginePreviousMode = GAME_MODE_JET_COASTER;
    g_EngineGameMode = GAME_MODE_FIELD;
    moduleTable.frame = GameModeDispatcher_Frame;
    moduleTable.onMouse = App_OnMouseMessage;
    moduleTable.onKey = App_OnKeyMessage;
    moduleTable.enter = GameModeDispatcher_Enter;
    moduleTable.exit = GameModeDispatcher_Exit;
    RequestModuleSwitch(&moduleTable, context);
}

/*
 * The snowboard minigame's installed frame callback. When `Mini_SnowboardInit` failed
 * (g_SnoboInitialized 0) it only returns to the field. Otherwise it clears the screen to white
 * (1.0, 1.0, 1.0, alpha 0), polls the input and, when the scene begins, resets the frame's draw
 * pools, turns all render states off except depth test and depth write, turns perspective
 * correction, linear filtering and dithering on, and runs `Mini_SnowboardUpdate`: the fixed-step
 * loop that runs the state callback until the simulated frames catch up with the elapsed time,
 * drawing on the last step. When that returns nonzero (g_SnoboActive cleared, the run is over)
 * it returns to the field; either way it ends the scene.
 * `Mini_SnowboardReturnToField` records GAME_MODE_JET_COASTER, not a snowboard mode, as the
 * previous game mode.
 */
// FUNCTION: FF7 0x00722ccf
void __cdecl Mini_SnowboardFrame(struct GfxEngineContext *ctx)
{
    if (g_SnoboInitialized != 0) {
        Mini_SnowboardSetBackgroundColorAndClear(FLOAT_ONE_BITS, FLOAT_ONE_BITS, FLOAT_ONE_BITS, 0);
        Input_Update(ctx);
        /* Retail caller passes -1 arg(s) to 2-parameter `Gfx_BeginSceneCall` (__cdecl stack cleanup matches retail). */
        if (((int(__cdecl *)(...))Gfx_BeginSceneCall)(0, (int)ctx)) {
            /* Retail caller passes -1 arg(s) to 1-parameter `ResetFrameDrawPool` (__cdecl stack cleanup matches retail). */
            ((int(__cdecl *)(...))ResetFrameDrawPool)((int)ctx);
            /* Retail caller passes -1 arg(s) to 1-parameter `ResetRenderPools` (__cdecl stack cleanup matches retail). */
            ((int(__cdecl *)(...))ResetRenderPools)((int)ctx);
            Mini_SnowboardResetRenderStates(ctx);
            Mini_SnowboardSetThreeRenderStates(ctx, 1);
            /* Retail caller passes -1 arg(s) to 1-parameter `Mini_SnowboardUpdate` (__cdecl stack cleanup matches retail). */
            if (((int(__cdecl *)(...))Mini_SnowboardUpdate)((int)ctx)) {
                Mini_SnowboardReturnToField(ctx);
            }
            /* Retail caller passes -1 arg(s) to 1-parameter `Gfx_EndSceneCall` (__cdecl stack cleanup matches retail). */
            ((int(__cdecl *)(...))Gfx_EndSceneCall)((int)ctx);
        }
    } else {
        Mini_SnowboardReturnToField(ctx);
    }
}

/*
 * Resets rendering pipeline state flags and enables alpha blending/testing.
 */
// FUNCTION: FF7 0x00722d80
void __cdecl Mini_SnowboardResetRenderStates(struct GfxEngineContext *context)
{
    int stateIndex;

    for (stateIndex = 0; stateIndex < 0x16; stateIndex++) {
        Gfx_SetRenderStateCall(stateIndex, 0, (struct GfxEngineContext *)context);
    }
    Gfx_SetRenderStateCall(RENDER_STATE_DEPTH_TEST, 1, (struct GfxEngineContext *)context);
    Gfx_SetRenderStateCall(RENDER_STATE_DEPTH_WRITE, 1, (struct GfxEngineContext *)context);
}

/*
 * Sets render states 3, 2, and 9 to the specified value.
 */
// FUNCTION: FF7 0x00722dd4
void __cdecl Mini_SnowboardSetThreeRenderStates(struct GfxEngineContext *context, int value)
{
    Gfx_SetRenderStateCall(RENDER_STATE_PERSPECTIVE, value, (struct GfxEngineContext *)context);
    Gfx_SetRenderStateCall(RENDER_STATE_LINEAR_FILTER, value, (struct GfxEngineContext *)context);
    Gfx_SetRenderStateCall(RENDER_STATE_DITHER, value, (struct GfxEngineContext *)context);
}

/*
 * Sets viewport background clear color and clears render buffers.
 */
// FUNCTION: FF7 0x00722e0f
void __cdecl Mini_SnowboardSetBackgroundColorAndClear(int red, int green, int blue, int alpha)
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
 * Unused mouse message handler for snowboard minigame.
 */
// FUNCTION: FF7 0x00722e55
void __cdecl Mini_SnowboardMouse(void)
{
}

/*
 * Unused keyboard message handler for snowboard minigame.
 */
// FUNCTION: FF7 0x00722e5a
void __cdecl Mini_SnowboardKeyboard(void)
{
}
} /* extern "C" */
