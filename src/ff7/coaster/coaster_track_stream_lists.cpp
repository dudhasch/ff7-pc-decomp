// src/ff7/coaster/coaster_track_stream_lists.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Track streaming for the coaster: resolves the rail and scenery stream tables, advances
//   the track cursor, and inserts, removes and retires stream entries in the active linked lists.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/types/coaster.h"
#include "ff7/coaster_psxdata.h"
#include "ff7/coaster_renderer.h"
#include <windows.h>
#include <ddraw.h>
#include "ff7/api/coaster.h"
#include "lib/codegen.h"
#include "lib/crt.h"
#include <time.h>

extern "C" {

/* Doubly linked active stream node pools: 9,000 rail segment nodes and 12,000 scenery nodes. */
// clang-format off
// GLOBAL: FF7 0x00c476e8
unsigned short *g_pCoasterRailRemoveTable = 0;
unsigned char g_bssPad_00c476ec[4] = {0};
// GLOBAL: FF7 0x00c476f0
CoasterStreamNode g_CoasterRailStreamNodes[9000] = {0};
// GLOBAL: FF7 0x00c50390
unsigned short *g_pCoasterSceneryStreamTable = 0;
// GLOBAL: FF7 0x00c50394
unsigned int g_CoasterStreamPosition = 0;
// GLOBAL: FF7 0x00c50398
unsigned short *g_pCoasterSceneryRemoveTable = 0;
// GLOBAL: FF7 0x00c5039c
unsigned short g_CoasterSceneryCount = 0;
// GLOBAL: FF7 0x00c503a0
int g_CoasterCellDelta = 0;
// GLOBAL: FF7 0x00c503a4
unsigned short g_CoasterRailListHead = 0;
// GLOBAL: FF7 0x00c503a8
unsigned short g_CoasterSceneryListTail = 0;
unsigned char g_bssPad_00c503ac[4] = {0};
// GLOBAL: FF7 0x00c503b0
CoasterStreamNode g_CoasterSceneryStreamNodes[12000] = {0};
// GLOBAL: FF7 0x00c5bf30
unsigned short g_CoasterRailCount = 0;
// GLOBAL: FF7 0x00c5bf34
unsigned char g_CoasterStreamPrimePending = 0;
// GLOBAL: FF7 0x00c5bf38
unsigned short g_CoasterRailListTail = 0;
// GLOBAL: FF7 0x00c5bf3c
unsigned short *g_pCoasterRailStreamTable = 0;
// GLOBAL: FF7 0x00c5bf40
unsigned int g_CoasterPrevStreamPosition = 0;
// GLOBAL: FF7 0x00c5bf44
unsigned short g_CoasterSceneryListHead = 0;
// clang-format on

/*
 * Resets the rail and scenery stream lists and resolves stream tables from the PSX data set.
 */
// FUNCTION: FF7 0x005ed8f0
void __cdecl Mini_CoasterResetStreamLists(void)
{
    int nodeIndex;
    int streamBase;
    int offS;
    CoasterStreamNode *pNode;
    int railIdx;
    int dataTable;
    int dataOfs;
    int railBase;
    int removeTable;
    int removeOfs;

    g_CoasterStreamPosition = 0xFFFE;
    g_CoasterPrevStreamPosition = 0;
    offS = *(int *)((char *)g_CoasterPsxDataSet.adrOffsets + (UNFOLDED(8) << 2));
    if (offS >= g_CoasterPsxDataSet.binSize) {
        streamBase = 0;
    } else {
        streamBase = g_CoasterPsxDataSet.binData + offS;
    }
    g_pCoasterSceneryStreamTable = (unsigned short *)streamBase;
    dataOfs = *(int *)((char *)g_CoasterPsxDataSet.adrOffsets + (UNFOLDED(9) << 2));
    if (dataOfs >= g_CoasterPsxDataSet.binSize) {
        dataTable = 0;
    } else {
        dataTable = g_CoasterPsxDataSet.binData + dataOfs;
    }
    g_pCoasterSceneryRemoveTable = (unsigned short *)dataTable;
    railIdx = *(int *)((char *)g_CoasterPsxDataSet.adrOffsets + (UNFOLDED(2) << 2));
    if (railIdx >= g_CoasterPsxDataSet.binSize) {
        railBase = 0;
    } else {
        railBase = g_CoasterPsxDataSet.binData + railIdx;
    }
    g_pCoasterRailStreamTable = (unsigned short *)railBase;
    removeOfs = *(int *)((char *)g_CoasterPsxDataSet.adrOffsets + (UNFOLDED(3) << 2));
    if (removeOfs >= g_CoasterPsxDataSet.binSize) {
        removeTable = 0;
    } else {
        removeTable = g_CoasterPsxDataSet.binData + removeOfs;
    }
    g_pCoasterRailRemoveTable = (unsigned short *)removeTable;
    for (nodeIndex = 0; nodeIndex < 12000; nodeIndex++) {
        pNode = &g_CoasterSceneryStreamNodes[nodeIndex];
        pNode->prevIndex = 0xFFFF;
        pNode->nextIndex = 0xFFFF;
    }
    g_CoasterSceneryCount = 0;
    for (nodeIndex = 0; nodeIndex < 9000; nodeIndex++) {
        pNode = &g_CoasterRailStreamNodes[nodeIndex];
        pNode->prevIndex = 0xFFFF;
        pNode->nextIndex = 0xFFFF;
    }
    g_CoasterRailCount = 0;
    g_CoasterStreamPrimePending = 1;
}

/*
 * Advances coaster track cursor position by current speed and retires streamed entries.
 */
// FUNCTION: FF7 0x005eda91
void Mini_CoasterAdvanceTrackCursorAndRetire(int speed)
{
    struct {
        unsigned short trackId;
        short trackPad;
        int newCell;
        unsigned short sceneryId;
        short sceneryPad;
        int oldCell;
        unsigned int crossed;
        unsigned int i;
    } local; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    g_CoasterPrevStreamPosition = g_CoasterStreamPosition;
    g_CoasterStreamPosition += speed;
    local.oldCell = g_CoasterPrevStreamPosition >> 18;
    local.newCell = g_CoasterStreamPosition >> 18;
    local.crossed = local.newCell - local.oldCell;
    g_CoasterTrackCursor += local.crossed;
    for (local.i = 0; local.i < local.crossed + g_CoasterStreamPrimePending; local.i++) {
        while (1) { /* Constant condition kept for /Od branch/register layout; see lib/codegen.h. */
            local.sceneryId = *g_pCoasterSceneryStreamTable;
            g_pCoasterSceneryStreamTable++;
            if (local.sceneryId == 0xFFFF)
                break;
            Mini_CoasterSceneryListInsert(local.sceneryId);
        }
        while (1) { /* Constant condition kept for /Od branch/register layout; see lib/codegen.h. */
            local.sceneryId = *g_pCoasterSceneryRemoveTable;
            g_pCoasterSceneryRemoveTable++;
            if (local.sceneryId == 0xFFFF)
                break;
            Mini_CoasterSceneryListRemove(local.sceneryId);
        }
    }
    for (local.i = 0; local.i < local.crossed; local.i++) {
        while (1) { /* Constant condition kept for /Od branch/register layout; see lib/codegen.h. */
            local.trackId = *g_pCoasterRailStreamTable;
            g_pCoasterRailStreamTable++;
            if (local.trackId == 0xFFFF)
                break;
            Mini_CoasterRailListInsert(local.trackId);
        }
        while (1) { /* Constant condition kept for /Od branch/register layout; see lib/codegen.h. */
            local.trackId = *g_pCoasterRailRemoveTable;
            g_pCoasterRailRemoveTable++;
            if (local.trackId == 0xFFFF)
                break;
            Mini_CoasterRailListRemove(local.trackId);
        }
    }
    if (g_CoasterStreamPrimePending == 1)
        g_CoasterStreamPrimePending = 0;
}

/*
 * Advances the track cursor and streams in new rail and scenery entries.
 */
// FUNCTION: FF7 0x005edc59
void __cdecl Mini_CoasterAdvanceTrackCursor(int speed)
{
    struct {
        unsigned short trackId;
        short trackPad;
        int newCell;
        unsigned short sceneryId;
        short sceneryPad;
        unsigned int oldCell;
        unsigned int i;
    } local; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    g_CoasterPrevStreamPosition = g_CoasterStreamPosition;
    g_CoasterStreamPosition += speed;
    local.oldCell = g_CoasterPrevStreamPosition >> 18;
    local.newCell = g_CoasterStreamPosition >> 18;
    g_CoasterCellDelta = local.newCell - local.oldCell;
    g_CoasterTrackCursor += g_CoasterCellDelta;
    for (local.i = 0; local.i < (unsigned int)g_CoasterCellDelta + g_CoasterStreamPrimePending; local.i++) {
        while (1) { /* Constant condition kept for /Od branch/register layout; see lib/codegen.h. */
            local.sceneryId = *g_pCoasterSceneryStreamTable;
            g_pCoasterSceneryStreamTable++;
            if (local.sceneryId == 0xFFFF)
                break;
            Mini_CoasterSceneryListInsert(local.sceneryId);
        }
    }
    for (local.i = 0; local.i < (unsigned int)g_CoasterCellDelta; local.i++) {
        while (1) { /* Constant condition kept for /Od branch/register layout; see lib/codegen.h. */
            local.trackId = *g_pCoasterRailStreamTable;
            g_pCoasterRailStreamTable++;
            if (local.trackId == 0xFFFF)
                break;
            Mini_CoasterRailListInsert(local.trackId);
        }
    }
}

/*
 * Retires scenery and rail stream entries that have passed behind the coaster camera.
 */
// FUNCTION: FF7 0x005edd82
void __cdecl Mini_CoasterRetireStreamedEntries(void)
{
    struct {
        unsigned short trackId;
        short trackPad;
        unsigned short sceneryId;
        short sceneryPad;
        unsigned int i;
    } local;

    for (local.i = 0; local.i < (unsigned int)g_CoasterCellDelta + g_CoasterStreamPrimePending; local.i++) {
        while (1) { /* Constant condition kept for /Od branch/register layout; see lib/codegen.h. */
            local.sceneryId = *g_pCoasterSceneryRemoveTable;
            g_pCoasterSceneryRemoveTable++;
            if (local.sceneryId == 0xFFFF)
                break;
            Mini_CoasterSceneryListRemove(local.sceneryId);
        }
    }
    for (local.i = 0; local.i < (unsigned int)g_CoasterCellDelta; local.i++) {
        while (1) { /* Constant condition kept for /Od branch/register layout; see lib/codegen.h. */
            local.trackId = *g_pCoasterRailRemoveTable;
            g_pCoasterRailRemoveTable++;
            if (local.trackId == 0xFFFF)
                break;
            Mini_CoasterRailListRemove(local.trackId);
        }
    }
    if (g_CoasterStreamPrimePending == 1)
        g_CoasterStreamPrimePending = 0;
}

/*
 * Inserts a scenery stream entry into the active scenery doubly linked list.
 */
// FUNCTION: FF7 0x005ede71
void __cdecl Mini_CoasterSceneryListInsert(unsigned short entryIndex)
{
    unsigned short unusedNext;
    unsigned short unusedPrev;

    unusedPrev = *(unsigned short *)&g_CoasterSceneryStreamNodes[entryIndex].prevIndex;
    unusedNext = *(unsigned short *)&g_CoasterSceneryStreamNodes[entryIndex].nextIndex;
    if (g_CoasterSceneryCount == 0) {
        g_CoasterSceneryListHead = entryIndex;
        g_CoasterSceneryListTail = entryIndex;
        g_CoasterSceneryCount = 1;
    } else {
        g_CoasterSceneryStreamNodes[entryIndex].prevIndex = g_CoasterSceneryListTail;
        g_CoasterSceneryStreamNodes[g_CoasterSceneryListTail].nextIndex = entryIndex;
        g_CoasterSceneryListTail = entryIndex;
        g_CoasterSceneryCount++;
    }
}

/*
 * Unlinks a scenery stream entry from the active scenery doubly linked list.
 */
// FUNCTION: FF7 0x005edf18
void __cdecl Mini_CoasterSceneryListRemove(unsigned short nodeIndex)
{
    unsigned short prevIndex;
    unsigned short nextIndex;

    prevIndex = g_CoasterSceneryStreamNodes[nodeIndex].prevIndex;
    nextIndex = g_CoasterSceneryStreamNodes[nodeIndex].nextIndex;
    if (prevIndex != 0xFFFF) {
        g_CoasterSceneryStreamNodes[prevIndex].nextIndex = nextIndex;
    } else {
        g_CoasterSceneryListHead = nextIndex;
    }
    if (nextIndex != 0xFFFF) {
        g_CoasterSceneryStreamNodes[nextIndex].prevIndex = prevIndex;
    } else {
        g_CoasterSceneryListTail = prevIndex;
    }
    g_CoasterSceneryStreamNodes[nodeIndex].prevIndex = 0xFFFF;
    g_CoasterSceneryStreamNodes[nodeIndex].nextIndex = 0xFFFF;
    g_CoasterSceneryCount--;
}

/*
 * Inserts a rail segment stream entry into the active rail doubly linked list.
 */
// FUNCTION: FF7 0x005edfe7
void __cdecl Mini_CoasterRailListInsert(unsigned short entryIndex)
{
    if (g_CoasterRailCount == 0) {
        g_CoasterRailListHead = entryIndex;
        g_CoasterRailStreamNodes[entryIndex].prevIndex = 0xFFFF;
        g_CoasterRailStreamNodes[entryIndex].nextIndex = 0xFFFF;
        g_CoasterRailListTail = entryIndex;
        g_CoasterRailCount = 1;
    } else {
        g_CoasterRailStreamNodes[entryIndex].prevIndex = g_CoasterRailListTail;
        g_CoasterRailStreamNodes[entryIndex].nextIndex = 0xFFFF;
        g_CoasterRailStreamNodes[g_CoasterRailListTail].nextIndex = entryIndex;
        g_CoasterRailListTail = entryIndex;
        g_CoasterRailCount++;
    }
}

/*
 * Unlinks a rail segment stream entry from the active rail doubly linked list.
 */
// FUNCTION: FF7 0x005ee09a
void __cdecl Mini_CoasterRailListRemove(unsigned short nodeIndex)
{
    unsigned short prevIndex;
    unsigned short nextIndex;

    prevIndex = g_CoasterRailStreamNodes[nodeIndex].prevIndex;
    nextIndex = g_CoasterRailStreamNodes[nodeIndex].nextIndex;
    if (prevIndex != 0xFFFF) {
        g_CoasterRailStreamNodes[prevIndex].nextIndex = nextIndex;
    } else {
        g_CoasterRailListHead = nextIndex;
    }
    if (nextIndex != 0xFFFF) {
        g_CoasterRailStreamNodes[nextIndex].prevIndex = prevIndex;
    } else {
        g_CoasterRailListTail = prevIndex;
    }
    g_CoasterRailCount--;
}
} /* extern "C" */
