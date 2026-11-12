// src/ff7/coaster/coaster_input_poll.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Reads the coaster controls each frame: maps the d-pad to direction codes and moves the
//   targeting crosshair.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/types/coaster.h"
#include "ff7/coaster_psxdata.h"
#include "ff7/coaster_renderer.h"
#include <windows.h>
#include <ddraw.h>
#include "ff7/api/coaster.h"
#include "lib/api/file.h"
#include "lib/enums/file.h"
#include "lib/crt.h"
#include <time.h>

extern "C" {

// clang-format off
// GLOBAL: FF7 0x00c5bf48
int g_CoasterInputUnused = 0;
// GLOBAL: FF7 0x00c5bf4c
int g_CoasterInputDirection = 0;
// GLOBAL: FF7 0x00c5bf50
int g_CoasterPauseButtonCounter = 0;
// GLOBAL: FF7 0x00c5bf54
int g_CoasterBeamFiringActive = 0;
// clang-format on

/*
 * Reads coaster controls each frame: maps d-pad input into direction codes,
 * moves the crosshair within screen bounds, manages laser beam firing and recharge cycle,
 * or handles free camera adjustments when outside ride mode.
 */
// FUNCTION: FF7 0x005ee150
void __cdecl Mini_CoasterPollInput(void)
{
    if (g_CoasterPaused == 0) {
        g_CoasterInputDirection = 0;
        g_CoasterInputUnused = 0;
        if (Input_GetHeld(INPUT_LEFT) != 0) {
            g_CoasterInputDirection = 4;
        }
        if (Input_GetHeld(INPUT_RIGHT) != 0) {
            g_CoasterInputDirection = 6;
        }
        if (Input_GetHeld(INPUT_UP) != 0) {
            g_CoasterInputDirection = 8;
            if (Input_GetHeld(INPUT_LEFT) != 0) {
                g_CoasterInputDirection = 7;
            }
            if (Input_GetHeld(INPUT_RIGHT) != 0) {
                g_CoasterInputDirection = 9;
            }
        }
        if (Input_GetHeld(INPUT_DOWN) != 0) {
            g_CoasterInputDirection = 2;
            if (Input_GetHeld(INPUT_LEFT) != 0) {
                g_CoasterInputDirection = 1;
            }
            if (Input_GetHeld(INPUT_RIGHT) != 0) {
                g_CoasterInputDirection = 3;
            }
        }
        if (g_CoasterRideMode == 1) {
            if (Input_GetHeld(INPUT_DOWN) != 0) {
                g_CoasterReticleY += 10;
            }
            if (Input_GetHeld(INPUT_UP) != 0) {
                g_CoasterReticleY -= 10;
            }
            if (Input_GetHeld(INPUT_LEFT) != 0) {
                g_CoasterReticleX -= 10;
            }
            if (Input_GetHeld(INPUT_RIGHT) != 0) {
                g_CoasterReticleX += 10;
            }
            g_CoasterShotFlag = 0;
            if (Input_GetHeld(INPUT_CIRCLE) != 0) {
                Mini_CoasterUpdateBeamLoopSound(g_CoasterBeamCharge);
                g_CoasterBeamFiringActive = 1;
                if (g_CoasterBeamCharge > 8) {
                    g_CoasterBeamCharge--;
                }
                if (g_CoasterShotCooldown == 0) {
                    g_CoasterShotCooldown = 1;
                    g_CoasterShotFlag = 1;
                    g_CoasterBeamIndex++;
                    g_CoasterBeamIndex = g_CoasterBeamIndex % 3;
                } else {
                    g_CoasterShotCooldown--;
                }
            } else {
                if (g_CoasterBeamFiringActive != 0) {
                    Mini_CoasterUpdateBeamLoopSound(0);
                    g_CoasterBeamFiringActive = 0;
                }
                if (g_CoasterBeamCharge < 128) {
                    g_CoasterBeamCharge++;
                }
            }
            if (g_CoasterReticleX > 320) {
                g_CoasterReticleX = 320;
            }
            if (g_CoasterReticleX < 0) {
                g_CoasterReticleX = 0;
            }
            if (g_CoasterReticleY > 240) {
                g_CoasterReticleY = 240;
            }
            if (g_CoasterReticleY < 0) {
                g_CoasterReticleY = 0;
            }
        }
        if (g_CoasterRideMode == 0) {
            if (Input_GetHeld(INPUT_DOWN) != 0) {
                g_CoasterFadeFarDepth -= 0xA;
            }
            if (Input_GetHeld(INPUT_UP) != 0) {
                g_CoasterFadeFarDepth += 0xA;
            }
            if (Input_GetHeld(INPUT_LEFT) != 0) {
                g_CoasterFadeNearDepth -= 0xA;
            }
            if (Input_GetHeld(INPUT_RIGHT) != 0) {
                g_CoasterFadeNearDepth += 0xA;
            }
            if (Input_GetHeld(INPUT_CROSS) != 0) {
                g_CoasterDebugTuneCrossTriangle -= 0x64;
            }
            if (Input_GetHeld(INPUT_TRIANGLE) != 0) {
                g_CoasterDebugTuneCrossTriangle += 0x64;
            }
            if (Input_GetHeld(INPUT_SQUARE) != 0) {
                g_CoasterDebugTuneSquareCircle -= 0x64;
            }
            if (Input_GetHeld(INPUT_CIRCLE) != 0) {
                g_CoasterDebugTuneSquareCircle += 0x64;
            }
            if (Input_GetHeld(INPUT_R1) != 0) {
                g_CoasterDebugTuneR1R2 -= 0x64;
            }
            if (Input_GetHeld(INPUT_R2) != 0) {
                g_CoasterDebugTuneR1R2 += 0x64;
            }
            if (Input_GetHeld(INPUT_L1) != 0) {
                g_CoasterRideSpeed += 1024;
            }
            if (Input_GetHeld(INPUT_L2) != 0 && g_CoasterRideSpeed > 1023) {
                g_CoasterRideSpeed -= 1024;
            }
            if (Input_GetHeld(INPUT_START) != 0) {
                g_CoasterRideSpeed = 0;
            }
        }
    }
    if (Input_GetHeld(INPUT_START) != 0) {
        g_CoasterPauseButtonCounter++;
    } else {
        g_CoasterPauseButtonCounter = 0;
    }
    if (g_CoasterPauseButtonCounter == 1) {
        if (g_CoasterPaused == 1) {
            g_CoasterPaused = 0;
        } else {
            g_CoasterPaused = 1;
        }
        Mini_CoasterPlaySfxAlternating(0x3B);
    }
}
} /* extern "C" */
