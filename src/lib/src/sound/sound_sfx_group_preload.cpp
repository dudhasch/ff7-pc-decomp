// src/lib/src/sound/sound_sfx_group_preload.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Preloads sound effect groups on a background job: starts and waits for the job, loads
//   each group's buffers, and waits until a given group is loaded.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/types/sound.h"
#include "lib/types/graphics.h"
#include "lib/types/thread.h"
#include "lib/codegen.h"
#include "lib/api/sound.h"
#include "lib/api/thread.h"
#include <windows.h>
#include <mmsystem.h>
#include <string.h>
#include <stdio.h>
extern "C" {

// clang-format off
// GLOBAL: FF7 0x00de65c0
struct SoundGroup *g_SfxPreloadGroups = 0;
unsigned char g_bssPad_00de65c4[4] = {0};
// GLOBAL: FF7 0x00de65c8
int g_SfxPreloadJobRecords[16] = {0};
// GLOBAL: FF7 0x00de6608
int g_SfxPreloadGroupTotal = 0;
// GLOBAL: FF7 0x00de660c
int g_SfxPreloadCurrentIndex = 0;
// clang-format on
#pragma intrinsic(memcpy)

__declspec(dllimport) void __stdcall OutputDebugStringA(const char *text);
#pragma intrinsic(strcpy, strcat, memset, strcmp)

/*
 * The preload job procedure: loads the up-to-four buffers of the current preload group
 * (g_SfxPreloadGroups, index g_SfxPreloadCurrentIndex).
 */
// FUNCTION: FF7 0x00749940
void __cdecl SfxPreloadGroupJobProc(void)
{
    int i;

    for (i = 0; i < 4; i++) {
        if (g_SfxPreloadGroups[g_SfxPreloadCurrentIndex].sound[i] != 0) {
            SfxLoadIntoBuffer(g_SfxPreloadGroups[g_SfxPreloadCurrentIndex].sound[i], 0);
        }
    }
}

/*
 * Releases the current preload job's thread handle g_SfxPreloadJobRecords[g_SfxPreloadCurrentIndex] and clears it.
 */
// FUNCTION: FF7 0x0074999e
void __cdecl SfxReleaseCurrentPreloadJob(void)
{
    ThreadFreeOwnedRecord((struct ThreadRecord *)g_SfxPreloadJobRecords[g_SfxPreloadCurrentIndex]);
    g_SfxPreloadJobRecords[g_SfxPreloadCurrentIndex] = 0;
}

/*
 * Copies sixteen sound-group first IDs into g_SfxPreloadJobRecords, returning early if an existing entry is
 * occupied. Stores the group pointer, prepares/creates the preload job and starts a nonzero handle
 * with g_SfxPreloadGroupTotal set to 1; on creation failure clears the 64-byte entry table. Preserves
 * partial table writes on early return and disabled debug branches.
 */
// FUNCTION: FF7 0x007499c9
void __cdecl SfxStartGroupPreload(struct SoundGroup *groups)
{
    struct {
        int index;
        int thread[4];
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    for (frame.index = 0; frame.index < 16; frame.index++) {
        if (g_SfxPreloadJobRecords[frame.index] != 0) {
            if (DEBUG_TRACE) {
                // STRING: FF7 0x00967fa0
                OutputDebugStringA("ERROR: SOUND THREAD ALREADY IN USE \n");
            };
            return;
        }
        g_SfxPreloadJobRecords[frame.index] = groups[frame.index].sound[0];
    }
    g_SfxPreloadGroups = groups;
    ThreadInitCallbackRecord((int)SfxPreloadGroupJobProc, (ThreadCallbackRecord *)frame.thread);
    g_SfxPreloadCurrentIndex = 0;
    g_SfxPreloadJobRecords[0] = ThreadCreateRecordFromTemplate((int)frame.thread);
    if (g_SfxPreloadJobRecords[0] != 0) {
        ThreadStartRecord((struct ThreadRecord *)(g_SfxPreloadJobRecords[0]));
        if (DEBUG_TRACE) {
            // STRING: FF7 0x00967fc8
            OutputDebugStringA("SOUND THREAD START \n");
        };
        g_SfxPreloadGroupTotal = 1;
    } else {
        if (DEBUG_TRACE) {
            // STRING: FF7 0x00967fe0
            OutputDebugStringA("ERROR: COULD NOT START SOUND THREAD \n");
        };
        memset(g_SfxPreloadJobRecords, 0, 64);
    }
}

/*
 * For a present preload job, waits for job state three, releases it and advances group index.
 * Stops/reset state at index sixteen or an empty group, otherwise constructs/starts the next job
 * and marks preload active. On job creation failure clears all sixteen job handles and resets
 * state; retained debug branches are disabled.
 */
// FUNCTION: FF7 0x00749ab1
void __cdecl SfxPumpGroupPreload(void)
{
    struct {
        int thread[4];
        int state;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    if (g_SfxPreloadJobRecords[g_SfxPreloadCurrentIndex] != 0) {
        frame.state = ThreadGetState((struct ThreadRecord *)(g_SfxPreloadJobRecords[g_SfxPreloadCurrentIndex]));
        if (frame.state == 3) {
            if (DEBUG_TRACE) {
                // STRING: FF7 0x00968008
                OutputDebugStringA("SOUND THREAD END \n");
            };
            SfxReleaseCurrentPreloadJob();
            g_SfxPreloadCurrentIndex++;
            if (g_SfxPreloadCurrentIndex == 16) {
                g_SfxPreloadGroupTotal = 0;
                g_SfxPreloadCurrentIndex = g_SfxPreloadGroupTotal;
                return;
            }
            if (g_SfxPreloadGroups[g_SfxPreloadCurrentIndex].sound[0] != 0) {
                ThreadInitCallbackRecord((int)SfxPreloadGroupJobProc, (ThreadCallbackRecord *)frame.thread);
                g_SfxPreloadJobRecords[g_SfxPreloadCurrentIndex] = ThreadCreateRecordFromTemplate((int)frame.thread);
                if (g_SfxPreloadJobRecords[g_SfxPreloadCurrentIndex] != 0) {
                    ThreadStartRecord((struct ThreadRecord *)(g_SfxPreloadJobRecords[g_SfxPreloadCurrentIndex]));
                    if (DEBUG_TRACE) {
                        // STRING: FF7 0x0096801c
                        OutputDebugStringA("	SOUND THREAD START \n");
                    };
                    g_SfxPreloadGroupTotal = 1;
                } else {
                    if (DEBUG_TRACE) {
                        // STRING: FF7 0x00968034
                        OutputDebugStringA("	ERROR: COULD NOT START SOUND THREAD \n");
                    };
                    memset(g_SfxPreloadJobRecords, 0, 64);
                    g_SfxPreloadGroupTotal = 0;
                    g_SfxPreloadCurrentIndex = g_SfxPreloadGroupTotal;
                }
            } else {
                g_SfxPreloadGroupTotal = 0;
                g_SfxPreloadCurrentIndex = g_SfxPreloadGroupTotal;
            }
        }
    }
}

/*
 * Returns if group is below g_SfxPreloadCurrentIndex or group preloading is inactive. Otherwise
 * repeatedly calls SfxPumpGroupPreload while the thread-handle table entry for group remains
 * nonzero. The wait has no timeout.
 */
// FUNCTION: FF7 0x00749c0d
void __cdecl SfxWaitForGroupLoaded(int group)
{
    int thread;
    if (group < g_SfxPreloadCurrentIndex) {
        return;
    }
    if (g_SfxPreloadGroupTotal == 0) {
        return;
    }
    while ((thread = g_SfxPreloadJobRecords[group]) != 0) {
        SfxPumpGroupPreload();
    }
}
} /* extern "C" */
