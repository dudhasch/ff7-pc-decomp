// src/ff7/src/main/main_clock_timer.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Starts and stops the savemap play-clock multimedia timer, advances the savemap clock on each tick
//   and returns the tick count.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/types/main.h"
#include "lib/types/graphics.h"
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <mmsystem.h>
#include "lib/codegen.h"
#include "ff7/api/main.h"
#include "ff7/api/menu.h"
#include "ff7/savemap.h"
#include "lib/api/graphics.h"
#include "lib/enums/graphics.h"
extern "C" {

#pragma intrinsic(strcpy, strcat, strlen)

// The play-clock timer: word 0 is the multimedia timer id Savemap_StartClockTimer keeps, words 1..6 the
// CRITICAL_SECTION Savemap_ClockTick holds while it advances the savemap clock (1 timer ID + 6 CS dwords).
// GLOBAL: FF7 0x009a06f0
int g_Savemap_ClockTimerData[7] = {0};
// GLOBAL: FF7 0x009a070c
int g_Savemap_ClockTickCount = 0;

/*
 * Chooses a periodic multimedia timer interval of 40, 25, or 17 from context flags, UI mode and
 * clock-rate threshold 266000000; clears timer counters, initializes the clock critical section,
 * starts Savemap_ClockTick with resolution 17, and returns whether its timer ID is nonzero.
 */
// FUNCTION: FF7 0x0040aab0
int __cdecl Savemap_StartClockTimer(void)
{
    GfxEngineContext *enginePtr;
    int intervalMs;
    enginePtr = GetEngineContext();
    if (enginePtr->driverType == GFX_DRIVER_SOFTWARE) {
        if (GetUiScalingMode() == 2) {
            intervalMs = 40;
        } else {
            if (enginePtr->tscFrequency < 266000000.0) {
                intervalMs = 25;
            } else {
                intervalMs = 17;
            }
        }
    } else {
        intervalMs = 17;
    }
    g_Savemap_ClockTickCount = 0;
    g_Savemap_ClockTimerData[0] = 0;
    InitializeCriticalSection((CRITICAL_SECTION *)&g_Savemap_ClockTimerData[1]);
    g_Savemap_ClockTimerData[0] = timeSetEvent(intervalMs, 17, Savemap_ClockTick, 0, 1);
    return g_Savemap_ClockTimerData[0] != 0;
}

/*
 * When the timer id in g_Savemap_ClockTimerData[0] is nonzero, kills that multimedia timer, clears the id
 * and deletes the critical section in the remaining words.
 */
// FUNCTION: FF7 0x0040ab52
void __cdecl Savemap_StopClockTimer(void)
{
    if (g_Savemap_ClockTimerData[0] != 0) {
        timeKillEvent(g_Savemap_ClockTimerData[0]);
        g_Savemap_ClockTimerData[0] = 0;
        DeleteCriticalSection((CRITICAL_SECTION *)&g_Savemap_ClockTimerData[1]);
    }
}

/*
 * Returns when the engine context has a window event pending; otherwise locks the clock critical section, increments its
 * tick count, and when both pause flags are zero advances two fractional accumulators by 0x444,
 * increments savemap seconds on carry, and increments or conditionally decrements countdown
 * seconds according to flag bit two before unlocking.
 */
// FUNCTION: FF7 0x0040ab81
void __stdcall Savemap_ClockTick(unsigned int timer, unsigned int message, unsigned long user, unsigned long first, unsigned long second)
{
    if (GetEngineContext()->pendingWindowEvent != 0) {
        return;
    }
    EnterCriticalSection((CRITICAL_SECTION *)&g_Savemap_ClockTimerData[1]);
    g_Savemap_ClockTickCount++;
    if ((g_BattleIsPaused == 0 && g_GamePauseLatch == 0) || 0) {
        gSavemap.millisecondCounter += 1092;
        if ((unsigned int)gSavemap.millisecondCounter >> 16) {
            gSavemap.seconds++;
            gSavemap.millisecondCounter &= 0xffff;
        }
        gSavemap.clockAccumulator += 0x444;
        if ((unsigned int)gSavemap.clockAccumulator >> 16) {
            if ((gSavemap.countdownFlags & 2) == 0) {
                if (gSavemap.countdownSeconds != 0) {
                    gSavemap.countdownSeconds--;
                }
            } else {
                gSavemap.countdownSeconds++;
            }
            gSavemap.clockAccumulator &= 0xffff;
        }
    }
    LeaveCriticalSection((CRITICAL_SECTION *)&g_Savemap_ClockTimerData[1]);
}

/*
 * Returns how many times the play-time clock timer has fired: g_Savemap_ClockTickCount, which
 * Savemap_ClockTick counts up on every tick, also while battle is paused but not while the engine
 * context has a window event pending. The play-time seconds advance by 0x444 / 0x10000 per tick,
 * so the timer runs at 60 ticks a second. It is a free-running counter, read without the clock's
 * critical section, that other code uses as an animation clock and a cheap random source: the
 * world map's water gradient, minimap blinking and texture animations, and the character the
 * "insert disc" screen shows.
 */
// FUNCTION: FF7 0x0040ac84
int __cdecl Savemap_GetClockTickCount(void)
{
    return g_Savemap_ClockTickCount;
}
} /* extern "C" */
