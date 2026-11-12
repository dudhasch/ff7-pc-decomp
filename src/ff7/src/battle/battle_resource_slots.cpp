// src/ff7/src/battle/battle_resource_slots.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: The battle task-slot allocators that hand out primary, overlay, secondary and auxiliary slots and
//   run each active slot pool.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/battle_tasks.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_actors.h"
#include <stdlib.h>
#include "ff7/camera_script.h"
#include <string.h>
#include "lib/codegen.h"
#include "ff7/api/battle.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
extern "C" {

#pragma intrinsic(abs)

#pragma intrinsic(memset)
#pragma intrinsic(memcpy)

/*
 * Allocates a primary battle task slot (up to 100 slots).
 */
// FUNCTION: FF7 0x005bec50
short __cdecl Battle_AllocateResourceSlot(BattleTaskFn callback)
{
    short slot;

    for (slot = 0; slot < 100; slot++) {
        if (g_BattleMainTaskCallbacks[slot] == 0 && g_BattleActiveTaskSlotIdx <= slot) {
            g_BattleMainTaskCallbacks[slot] = callback;
            g_BattleTaskSlotBuffer.mainSlots[slot].ownerSlot = g_BattleActiveTaskSlotIdx;
            g_BattleSecondaryTaskActiveCount++;
            return slot;
        }
    }
    Gfx_PsxHookNoOp_2();
    Gfx_PsxHookNoOp_1(1);
    Gfx_PsxHookNoOp_3();
    Gfx_PsxHookNoOp_4(0x61, 1);
    return -1;
}

/*
 * Allocates an overlay task slot (up to 10 slots).
 */
// FUNCTION: FF7 0x005becf1
short __cdecl Battle_AllocateOverlayTaskSlot(BattleTaskFn callback)
{
    short slot;

    for (slot = 0; slot < 10; slot++) {
        if (g_BattleOverlayTaskCallbacks[slot] == 0 && g_BattleOverlayTaskSlotIdx <= slot) {
            g_BattleOverlayTaskCallbacks[slot] = callback;
            g_BattleOverlayTaskSlotBuffer[slot].ownerSlot = g_BattleOverlayTaskSlotIdx;
            g_BattleOverlayTaskActiveCount++;
            return slot;
        }
    }
    Gfx_PsxHookNoOp_2();
    Gfx_PsxHookNoOp_1(1);
    Gfx_PsxHookNoOp_3();
    Gfx_PsxHookNoOp_4(0x61, 2);
    return -1;
}

/*
 * Allocates a secondary task slot (up to 60 slots).
 */
// FUNCTION: FF7 0x005bed92
short __cdecl Battle_AllocateSecondaryResourceSlot(BattleTaskFn callback)
{
    short slot;

    for (slot = 0; slot < 60; slot++) {
        if (g_BattleSecondaryTaskCallbacks[slot] == 0 && g_BattleSecondaryTaskActiveSlotIdx <= slot) {
            g_BattleSecondaryTaskCallbacks[slot] = callback;
            g_BattleTaskSlotBuffer.secondarySlots[slot].ownerSlot = g_BattleSecondaryTaskActiveSlotIdx;
            g_BattleSecondaryResourceCount++;
            return slot;
        }
    }
    Gfx_PsxHookNoOp_2();
    Gfx_PsxHookNoOp_1(1);
    Gfx_PsxHookNoOp_3();
    Gfx_PsxHookNoOp_4(0x61, 4);
    return -1;
}

/*
 * Allocates an auxiliary task slot (up to 16 slots).
 */
// FUNCTION: FF7 0x005bee33
short __cdecl Battle_AllocateAuxTaskSlot(BattleTaskFn callback)
{
    short slot;

    for (slot = 0; slot < 16; slot++) {
        if (g_BattleAuxTaskCallbackArray[slot] == 0) {
            g_BattleAuxTaskCallbackArray[slot] = callback;
            g_BattleAuxTaskSlotBuffer[slot].ownerSlot = g_BattleCameraAuxTaskSlot;
            g_BattleAuxTaskActiveCount++;
            return slot;
        }
    }
    Gfx_PsxHookNoOp_2();
    Gfx_PsxHookNoOp_1(1);
    Gfx_PsxHookNoOp_3();
    Gfx_PsxHookNoOp_4(0x61, 3);
    return -1;
}

/*
 * Resets all primary, overlay, secondary, and auxiliary task slots.
 */
// FUNCTION: FF7 0x005beec6
void __cdecl Battle_ResetAllTaskSlots(void)
{
    int task;

    g_BattleSecondaryResourceCount = 0;
    g_BattleOverlayTaskActiveCount = g_BattleSecondaryResourceCount;
    g_BattleSecondaryTaskActiveCount = g_BattleOverlayTaskActiveCount;
    for (task = 0; task < 100; task++) {
        g_BattleMainTaskCallbacks[task] = 0;
        g_BattleTaskSlotBuffer.mainSlots[task].frameCounter = 0;
        g_BattleTaskSlotBuffer.mainSlots[task].ownerSlot = 0;
    }
    for (task = 0; task < 10; task++) {
        g_BattleOverlayTaskCallbacks[task] = 0;
        g_BattleOverlayTaskSlotBuffer[task].state = 0;
        g_BattleOverlayTaskSlotBuffer[task].ownerSlot = 0;
    }
    for (task = 0; task < 60; task++) {
        g_BattleSecondaryTaskCallbacks[task] = 0;
        g_BattleTaskSlotBuffer.secondarySlots[task].frameCounter = 0;
        g_BattleTaskSlotBuffer.secondarySlots[task].ownerSlot = 0;
    }
    Battle_ResetAuxTaskSlots();
}

/*
 * Resets auxiliary task slot arrays.
 */
// FUNCTION: FF7 0x005befc8
void __cdecl Battle_ResetAuxTaskSlots(void)
{
    int i;

    g_BattleAuxTaskActiveCount = 0;
    for (i = 0; i < 16; i++) {
        g_BattleAuxTaskCallbackArray[i] = 0;
        g_BattleAuxTaskSlotBuffer[i].ownerSlot = 0;
        g_BattleAuxTaskSlotBuffer[i].timerWord = 0;
    }
}

/*
 * Iterates and executes active primary battle task slots.
 */
// FUNCTION: FF7 0x005bf01f
void __cdecl Battle_RunMainTaskSlots(void)
{
    for (g_BattleActiveTaskSlotIdx = 0; g_BattleActiveTaskSlotIdx < 100; g_BattleActiveTaskSlotIdx++) {
        if (g_BattleMainTaskCallbacks[g_BattleActiveTaskSlotIdx] != 0 && g_BattleIsRunning != 0) {
            g_BattleMainTaskCallbacks[g_BattleActiveTaskSlotIdx]();
            if (g_BattleTaskSlotBuffer.mainSlots[g_BattleActiveTaskSlotIdx].ownerSlot == -1) {
                g_BattleTaskSlotBuffer.mainSlots[g_BattleActiveTaskSlotIdx].ownerSlot = 0;
                g_BattleTaskSlotBuffer.mainSlots[g_BattleActiveTaskSlotIdx].frameCounter = 0;
                g_BattleMainTaskCallbacks[g_BattleActiveTaskSlotIdx] = 0;
                g_BattleSecondaryTaskActiveCount--;
            }
        } else if (g_BattleMainTaskCallbacks[g_BattleActiveTaskSlotIdx] == Battle_MainTaskWaitMessageWindow) {
            g_BattleMainTaskCallbacks[g_BattleActiveTaskSlotIdx]();
        }
    }
    g_BattleActiveTaskSlotIdx = 0;
}

/*
 * Iterates and executes active overlay task slots.
 */
// FUNCTION: FF7 0x005bf107
void __cdecl Battle_RunOverlayTaskSlots(void)
{
    for (g_BattleOverlayTaskSlotIdx = 0; g_BattleOverlayTaskSlotIdx < 10; g_BattleOverlayTaskSlotIdx++) {
        if (g_BattleOverlayTaskCallbacks[g_BattleOverlayTaskSlotIdx] != 0) {
            g_BattleOverlayTaskCallbacks[g_BattleOverlayTaskSlotIdx]();
            if (g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].ownerSlot == -1) {
                g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].ownerSlot = 0;
                g_BattleOverlayTaskSlotBuffer[g_BattleOverlayTaskSlotIdx].state = 0;
                g_BattleOverlayTaskCallbacks[g_BattleOverlayTaskSlotIdx] = 0;
                g_BattleOverlayTaskActiveCount--;
            }
        }
    }
    g_BattleOverlayTaskSlotIdx = 0;
}

/*
 * Iterates and executes active effect task slots.
 */
// FUNCTION: FF7 0x005bf1c2
void __cdecl Battle_RunEffectTaskSlots(void)
{
    for (g_BattleSecondaryTaskActiveSlotIdx = 0; g_BattleSecondaryTaskActiveSlotIdx < 60; g_BattleSecondaryTaskActiveSlotIdx++) {
        if (g_BattleSecondaryTaskCallbacks[g_BattleSecondaryTaskActiveSlotIdx] != 0) {
            g_BattleSecondaryTaskCallbacks[g_BattleSecondaryTaskActiveSlotIdx]();
            if (g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx].ownerSlot == -1) {
                g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx].ownerSlot = 0;
                g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx].frameCounter = 0;
                g_BattleSecondaryTaskCallbacks[g_BattleSecondaryTaskActiveSlotIdx] = 0;
                g_BattleSecondaryResourceCount--;
            }
        }
    }
    g_BattleSecondaryTaskActiveSlotIdx = 0;
}

/*
 * Iterates and executes active auxiliary task slots.
 */
// FUNCTION: FF7 0x005bf27d
void __cdecl Battle_RunAuxTaskSlots(void)
{
    for (g_BattleCameraAuxTaskSlot = 0; g_BattleCameraAuxTaskSlot < 16; g_BattleCameraAuxTaskSlot++) {
        if (g_BattleAuxTaskCallbackArray[g_BattleCameraAuxTaskSlot] != 0) {
            g_BattleAuxTaskCallbackArray[g_BattleCameraAuxTaskSlot]();
            if (g_BattleAuxTaskSlotBuffer[g_BattleCameraAuxTaskSlot].ownerSlot == -1) {
                g_BattleAuxTaskSlotBuffer[g_BattleCameraAuxTaskSlot].ownerSlot = 0;
                g_BattleAuxTaskSlotBuffer[g_BattleCameraAuxTaskSlot].timerWord = 0;
                g_BattleAuxTaskCallbackArray[g_BattleCameraAuxTaskSlot] = 0;
                g_BattleAuxTaskActiveCount--;
            }
        }
    }
    g_BattleCameraAuxTaskSlot = 0;
}
} /* extern "C" */
