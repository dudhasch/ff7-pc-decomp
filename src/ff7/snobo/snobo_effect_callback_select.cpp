// src/ff7/snobo/snobo_effect_callback_select.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: The dispatch layer for snowboard effect callbacks. It selects the standard or alternate
//   effect callback set by mode and forwards the update, clear and visit calls to whichever set is
//   installed.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/types/snobo.h"
#include <stdlib.h>
#include "ff7/api/snobo.h"
extern "C" {

#pragma intrinsic(abs)

// clang-format off
// GLOBAL: FF7 0x00ddad0c
void (__cdecl *g_SnoboEventVisitCallback)(short) = 0;
// GLOBAL: FF7 0x00ddad10
void (__cdecl *g_SnoboEffectUpdateCallback)(struct SnowboardCourseNodeState *node) = 0;
// GLOBAL: FF7 0x00ddad14
void (__cdecl *g_SnoboEffectClearCallback)(void) = 0;
// clang-format on

/*
 * Installs the effect code of the current course when a run starts at the course entry
 * (`Mini_SnowboardInitRunAtCourseEntry`). With g_SnoboModeFlag 1 (GAME_MODE_SNOWBOARD, the
 * for_ev.tmd course) it installs the Alt set: `Mini_SnowboardUpdateAltEffectRecords`,
 * `Mini_SnowboardClearAltEffectRecords` and `Mini_SnowboardVisitAltEventSlot`, and clears the Alt
 * event-slot visit flags. Otherwise it installs the standard set
 * (`Mini_SnowboardUpdateEffectRecords`, `Mini_SnowboardClearEffectRecords`,
 * `Mini_SnowboardVisitEventSlot`) and clears its visit flags. The caller then runs the installed
 * clear callback to empty the effect pool.
 */
// FUNCTION: FF7 0x00735370
void __cdecl Mini_SnowboardSelectEffectCallbacks(void)
{
    if (g_SnoboModeFlag == 1) {
        g_SnoboEffectUpdateCallback = Mini_SnowboardUpdateAltEffectRecords;
        g_SnoboEffectClearCallback = Mini_SnowboardClearAltEffectRecords;
        g_SnoboEventVisitCallback = Mini_SnowboardVisitAltEventSlot;
        Mini_SnowboardClearAltEventVisitFlags();
    } else {
        g_SnoboEffectUpdateCallback = Mini_SnowboardUpdateEffectRecords;
        g_SnoboEffectClearCallback = Mini_SnowboardClearEffectRecords;
        g_SnoboEventVisitCallback = Mini_SnowboardVisitEventSlot;
        Mini_SnowboardClearEventVisitFlags();
    }
}

/*
 * Runs this frame's course effects (the objects the event slots spawned) through the update
 * callback `Mini_SnowboardSelectEffectCallbacks` installed: it retires effects behind the
 * rider's section, runs their callbacks, queues their models and tests them against the rider,
 * setting the contact response. `frame` is the course node state `Mini_SnowboardEmitCourseChain`
 * passes; the callback writes the current section, the running flag and the rider position
 * into it.
 */
// FUNCTION: FF7 0x007353c6
void __cdecl Mini_SnowboardUpdateEffectNodes(struct SnowboardCourseNodeState *frame)
{
    g_SnoboEffectUpdateCallback(frame);
}

/*
 * Invokes the installed snowboard clear effect callback.
 */
// FUNCTION: FF7 0x007353d8
void __cdecl Mini_SnowboardInvokeClearEffectCallback(void)
{
    g_SnoboEffectClearCallback();
}

/*
 * Triggers one course event slot through the visit callback `Mini_SnowboardSelectEffectCallbacks`
 * installed: the callback marks the slot visited and calls the slot's spawner in its callback
 * table with the slot and whether this is the first visit; the spawner places the slot's effects.
 * `Mini_SnowboardInvokeRecordVisitCallbacks` calls it for each course record, with the record's
 * section base from the section table plus its slot offset.
 */
// FUNCTION: FF7 0x007353e3
void __cdecl Mini_SnowboardInvokeVisitCallback(short visitSlot)
{
    g_SnoboEventVisitCallback(visitSlot);
}
} /* extern "C" */
