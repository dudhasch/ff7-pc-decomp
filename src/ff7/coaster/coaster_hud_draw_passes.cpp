// src/ff7/coaster/coaster_hud_draw_passes.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Coaster HUD drawing: the ride HUD, the HUD drawn while paused and the submit of every
//   queued batch list, and the minigame's shutdown, which frees its resources and models.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/types/coaster.h"
#include "ff7/coaster_psxdata.h"
#include "ff7/coaster_renderer.h"
#include <windows.h>
#include <ddraw.h>
#include "ff7/api/coaster.h"
#include "lib/api/file.h"
#include "lib/crt.h"
#include <time.h>
#include "ff7/enums/main.h"
#include "lib/enums/file.h"

extern "C" {


/*
 * Draws the ride HUD (the charge gauge, two panels and the score panel) on the drawn tick.
 */
// FUNCTION: FF7 0x005ea7d0
void __cdecl Mini_CoasterDrawHud(void)
{
    if (g_CoasterDrawThisTick != 0) {
        g_pCoasterRenderer->drawCharge();
        g_pCoasterRenderer->drawPanel5F1838();
        g_pCoasterRenderer->drawPanel5F1A71();
        g_pCoasterRenderer->drawScorePanel();
    }
}

/*
 * Draws the two panels shown while the ride is paused, on the drawn tick (Mini_CoasterTick calls
 * it on the paused branch).
 */
// FUNCTION: FF7 0x005ea80a
void __cdecl Mini_CoasterDrawPausedHud(void)
{
    if (g_CoasterDrawThisTick != 0) {
        g_pCoasterRenderer->drawPanel5F1A71();
        g_pCoasterRenderer->drawPanel5F1CB5();
    }
}

/*
 * Submits every batch list the renderer queued this tick (rail, beam, reticle, score, pause,
 * fade) on the drawn tick.
 */
// FUNCTION: FF7 0x005ea82e
void __cdecl Mini_CoasterSubmitRendererBatches(void)
{
    if (g_CoasterDrawThisTick != 0) {
        g_pCoasterRenderer->submitBatches();
    }
}

/*
 * Frees what the ride loaded; called by `Mini_CoasterExit`. Deletes the CoasterRenderer (its
 * textures and batches), closes coaster.lgp (LGP slot 9), releases the 23 cached sound buffers
 * and the xbin.bin data set, and changes back to the working directory `Mini_CoasterInit` saved
 * in g_CoasterSavedDirectory.
 */
// FUNCTION: FF7 0x005ea847
void __cdecl Mini_CoasterShutdown(void)
{
    delete g_pCoasterRenderer;
    CloseLgpArchive(LGP_ARCHIVE_COASTER);
    Mini_CoasterReleaseSfxAndPsxData();
    ChangeDirectorySaving((char *)&g_CoasterSavedDirectory, 0);
}
} /* extern "C" */
