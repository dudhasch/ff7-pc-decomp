// src/ff7/src/wm/wm_world_model_state.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Resets and tears down the world map's model records and overlay model state. It also
//   loads the leader model, copies the stage model slot table and resolves model IDs to their
//   records.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/api/wm.h"
#include "lib/api/token.h"
#include <string.h>
#include "ff7/world.h"
#include "ff7/types/wm.h"
#include "lib/codegen.h"
extern "C" {

#pragma intrinsic(strcpy, memcpy)

// clang-format off
// GLOBAL: FF7 0x0096df10
unsigned char g_WorldModelSlotTable[32] = {0x00, 0x00, 0x00, 0xff, 0x01, 0x02, 0x03, 0x09, 0xff, 0xff, 0xff, 0xff, 0x05, 0xff, 0x08, 0x06, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x04, 0x0a, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
unsigned char g_rawData_0096df30[384] = {
0x00, 0x00, 0x00, 0x03, 0x01, 0xff, 0xff, 0x09, 0xff, 0x0a, 0x0c, 0xff, 0x05, 0x04, 0x08, 0x06,
0x07, 0xff, 0xff, 0x02, 0xff, 0xff, 0xff, 0xff, 0x0b, 0x0d, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0x00, 0x00, 0x00, 0x03, 0x01, 0xff, 0xff, 0xff, 0xff, 0x0a, 0x0c, 0xff, 0x05, 0x04, 0x08, 0x06,
0x07, 0xff, 0xff, 0x02, 0x09, 0xff, 0xff, 0xff, 0x0b, 0x0d, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0x00, 0x00, 0x00, 0x03, 0x01, 0xff, 0xff, 0x09, 0xff, 0x0a, 0x0c, 0xff, 0x05, 0x04, 0x08, 0x06,
0x07, 0xff, 0xff, 0x02, 0xff, 0xff, 0xff, 0xff, 0x0b, 0x0d, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0x00, 0x00, 0x00, 0x03, 0x01, 0xff, 0xff, 0xff, 0xff, 0x0a, 0x0c, 0xff, 0x05, 0x04, 0x08, 0x06,
0x07, 0xff, 0xff, 0x02, 0x09, 0xff, 0xff, 0xff, 0x0b, 0x0d, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0x00, 0x00, 0x00, 0x03, 0x01, 0xff, 0xff, 0xff, 0xff, 0x0a, 0xff, 0x0c, 0x05, 0x04, 0x08, 0x06,
0x07, 0xff, 0xff, 0x02, 0x09, 0xff, 0xff, 0xff, 0x0b, 0x0d, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0x00, 0x00, 0x00, 0x03, 0x01, 0xff, 0xff, 0xff, 0xff, 0x0a, 0xff, 0x0c, 0x05, 0x04, 0x08, 0x06,
0x07, 0xff, 0xff, 0x02, 0x09, 0xff, 0xff, 0xff, 0x0b, 0x0d, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0x00, 0x00, 0x00, 0x03, 0x01, 0xff, 0xff, 0xff, 0xff, 0x0a, 0xff, 0xff, 0x05, 0x04, 0x08, 0x06,
0x07, 0xff, 0xff, 0x02, 0x09, 0xff, 0xff, 0xff, 0xff, 0x0c, 0xff, 0xff, 0xff, 0x0b, 0xff, 0xff,
0x00, 0x00, 0x00, 0x03, 0x01, 0xff, 0xff, 0xff, 0xff, 0x0a, 0xff, 0xff, 0x05, 0x04, 0x08, 0x06,
0x07, 0xff, 0xff, 0x02, 0x09, 0xff, 0xff, 0xff, 0xff, 0x0c, 0xff, 0xff, 0xff, 0x0b, 0xff, 0xff,
0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x02, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x02, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x01, 0xff, 0xff,
0xff, 0x02, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x04, 0x05, 0x06, 0xff, 0x07, 0xff,
0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0x01, 0x02, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
// GLOBAL: FF7 0x00e3a8a8
int g_WorldLeaderModelLoading = 0;
// GLOBAL: FF7 0x00e3a8ac
int g_WorldLeaderModelReady = 0;
// GLOBAL: FF7 0x00e3a8b0
int g_WorldStageVariantFlag = 0;
// GLOBAL: FF7 0x00e3a8b4
int g_WorldOverlayModelActive = 0;
// GLOBAL: FF7 0x00e3a8b8
int g_WorldOverlayModelCountdown = 0;
// GLOBAL: FF7 0x00e3a8bc
int g_WorldOverlayModelState = 0;
// GLOBAL: FF7 0x00e3a8c0
signed char g_WorldOverlayModelRecord[44] = {0};
unsigned char g_bssPad_00e3a8ec[4] = {0};
unsigned char g_bssPad_00e3a8f0[2048] = {0};
// GLOBAL: FF7 0x00e3b0f0
int g_WorldStageModelIndex = 0;
// GLOBAL: FF7 0x00e3b0f4
int g_WorldStageModelCount = 0;
// GLOBAL: FF7 0x00e3b0f8
signed char g_WorldStageModelRecord[44] = {0};
// GLOBAL: FF7 0x00e3b124
int g_WorldLeaderModelIndex = 0;
// GLOBAL: FF7 0x00e3b128
int g_WorldLeaderModelCount = 0;
// GLOBAL: FF7 0x00e3b12c
int g_WorldLeaderModelId = 0;
// clang-format on

/*
 * Reset the world-map minimap state: 0x2B entries of 0xFF at g_WorldStageModelRecord
 * and 0x2B of zero at g_WorldOverlayModelRecord, then eleven scalars cleared. The
 * scalars come in two chains -- `g_WorldStageModelCount = 0` feeding g_WorldLeaderModelLoading
 * and g_WorldLeaderModelCount, and `g_WorldStageModelIndex = 0` feeding
 * three more.
 */
// FUNCTION: FF7 0x007688e0
void __cdecl World_ResetModelRecordState(void)
{
    int i;

    for (i = 0; i < 0x2B; i++) {
        g_WorldStageModelRecord[i] = 0xFF;
        g_WorldOverlayModelRecord[i] = 0;
    }
    g_WorldStageVariantFlag = 0;
    g_WorldOverlayModelCountdown = 0;
    g_WorldStageModelCount = 0;
    g_WorldLeaderModelLoading = g_WorldStageModelCount;
    g_WorldLeaderModelCount = g_WorldLeaderModelLoading;
    g_WorldOverlayModelState = 0;
    g_WorldStageModelIndex = 0;
    g_WorldLeaderModelId = g_WorldStageModelIndex;
    g_WorldLeaderModelReady = g_WorldLeaderModelId;
    g_WorldOverlayModelActive = g_WorldLeaderModelReady;
    g_WorldLeaderModelIndex = 0;
}

/*
 * Tear down whatever World_ResetModelRecordState set up, but only the part above index 0x20: the
 * first 0x20 entries of g_WorldStageModelRecord are left alone, which is what
 * World_CopyStageModelSlotTable below refills from the table at g_WorldModelSlotTable.
 * The two loops have different bounds and the same body shape, so they are two loops and not one
 * with a conditional.
 */
// FUNCTION: FF7 0x0076898a
void __cdecl World_ClearOverlayModelState(void)
{
    int i;

    if (g_WorldStageModelCount != 0) {
        g_WorldStageModelCount = 0;
        g_WorldStageModelIndex = 3;
        for (i = 0; i < 0x2B; i++) {
            g_WorldOverlayModelRecord[i] = 0;
        }
        for (i = 0x20; i < 0x2B; i++) {
            g_WorldStageModelRecord[i] = 0xFF;
        }
    }
}

/*
 * When overlay flag g_WorldStageModelCount is set and the actor-list predicate
 * WorldEntity_HasActorOfTypeAtLeast32 returns zero, tears down the overlay.
 * Independently decrements a nonzero countdown and calls
 * World_EnableTerrainStreaming when it reaches zero.
 */
// FUNCTION: FF7 0x007689f7
void __cdecl World_UpdateOverlayModelStateAndIdleCountdown(void)
{
    if (g_WorldStageModelCount != 0) {
        if (WorldEntity_HasActorOfTypeAtLeast32() == 0) {
            World_ClearOverlayModelState();
        }
    }
    if (g_WorldStageModelIndex != 0) {
        g_WorldStageModelIndex--;
        if (g_WorldStageModelIndex == 0) {
            World_EnableTerrainStreaming();
        }
    }
}

/*
 * Clamps invalid model IDs to zero and resolves models below 0x20 through the
 * two loaded-record groups, invoking the reload helper for a negative mapping
 * below three. Negative mappings for IDs three through 0x1F return null.
 */
// FUNCTION: FF7 0x00768a37
WorldModelRecord *__cdecl World_GetModelRecord(short model)
{
    struct {
        void *local_2_unused;
        int variantIndex;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    if (model < 0 || model >= 0x2b) {
        model = 0;
    }
    if (g_WorldStageModelRecord[model] < 0) {
        if (model < 3) {
            World_RequestLeaderModelLoad(model);
        } else if (model < 0x20) {
            return 0;
        }
    }
    if (model < 3) {
        return g_WorldLeaderModelCount != 0 ? g_WorldLeaderModels : 0;
    }
    if (model < 0x20) {
        return g_WorldLeaderModelLoading != 0
                   ? &g_WorldStageModels[g_WorldStageModelRecord[model] - 1]
                   : 0;
    }
    if (g_WorldStageModelCount != 0) {
        frame.variantIndex = g_WorldStageModelRecord[model] - 0xd;
        if (g_WorldOverlayModelRecord[model] == 0) {
            frame.local_2_unused = (void *)(*(int *)((char *)g_WorldStageVariantFlag + 0x10) + frame.variantIndex * 0x24);
            g_WorldOverlayModelRecord[model] = 1;
        }
        return 0;
    }
    return 0;
}

/*
 * When the load latch is clear and World_TryEnterInitStep(2) succeeds, clears
 * g_WorldLeaderModelCount, loads the requested leader model and sets the
 * latch. Always stores the requested model and immediately calls
 * World_FinishPendingLeaderModelLoad to process any set latch.
 */
// FUNCTION: FF7 0x00768b36
void __cdecl World_RequestLeaderModelLoad(short model)
{
    if (g_WorldLeaderModelReady == 0) {
        if (World_TryEnterInitStep(2) != 0) {
            g_WorldLeaderModelCount = 0;
            World_LoadLeaderModel(model);
            g_WorldLeaderModelReady = 1;
        }
    }
    g_WorldLeaderModelId = model;
    World_FinishPendingLeaderModelLoad();
}

/*
 * If a world message is pending (g_WorldLeaderModelReady), clears it, tells
 * the sound side with World_LeaveInitStep(2), and runs
 * World_MarkLeaderModelReady.
 */
// FUNCTION: FF7 0x00768b82
void __cdecl World_FinishPendingLeaderModelLoad(void)
{
    if (g_WorldLeaderModelReady != 0) {
        g_WorldLeaderModelReady = 0;
        World_LeaveInitStep(2);
        World_MarkLeaderModelReady();
    }
}

/*
 * Sets g_WorldLeaderModelCount to 1, sets the first three model-state bytes to
 * 0xff, then sets the unchecked g_WorldLeaderModelId entry to zero.
 */
// FUNCTION: FF7 0x00768ba9
void __cdecl World_MarkLeaderModelReady(void)
{
    g_WorldLeaderModelCount = 1;
    g_WorldStageModelRecord[2] = 0xff;
    g_WorldStageModelRecord[1] = g_WorldStageModelRecord[2];
    g_WorldStageModelRecord[0] = g_WorldStageModelRecord[1];
    g_WorldStageModelRecord[g_WorldLeaderModelId] = 0;
}

/*
 * Stores g_WorldOverlayModelActive and refreshes through
 * World_CopyStageModelSlotTable.
 */
// FUNCTION: FF7 0x00768be2
void __cdecl World_SetStageAndCopyModelSlotTable(short value)
{
    g_WorldOverlayModelActive = value;
    World_CopyStageModelSlotTable();
}

/*
 * Copies entries 3 through 31 of the selected 32-byte model-state row into
 * g_WorldStageModelRecord, leaving the first three unchanged, then sets
 * g_WorldLeaderModelLoading to 1.
 */
// FUNCTION: FF7 0x00768bf5
void __cdecl World_CopyStageModelSlotTable(void)
{
    int i;

    for (i = 3; i < 0x20; i++) {
        g_WorldStageModelRecord[i] = (char)g_WorldModelSlotTable[(g_WorldOverlayModelActive << 5) + i];
    }
    g_WorldLeaderModelLoading = 1;
}

/*
 * Clears the pending world message flag g_WorldLeaderModelReady.
 */
// FUNCTION: FF7 0x00768c3d
void __cdecl World_ClearLeaderModelLoadLatch(void)
{
    g_WorldLeaderModelReady = 0;
}

/*
 * Stores g_WorldLeaderModelIndex.
 */
// FUNCTION: FF7 0x00768c4c
void __cdecl World_SetStageVariantFlag(int value)
{
    g_WorldLeaderModelIndex = value;
}

/*
 * Returns g_WorldLeaderModelIndex.
 */
// FUNCTION: FF7 0x00768c59
int __cdecl World_GetStageVariantFlag(void)
{
    return g_WorldLeaderModelIndex;
}
} /* extern "C" */
