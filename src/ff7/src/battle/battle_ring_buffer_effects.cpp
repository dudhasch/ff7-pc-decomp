// src/ff7/src/battle/battle_ring_buffer_effects.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: A ring buffer of battle effect records, with the screen flash and fixed-point ramp
//   tasks, bit-pattern callback iteration, actor mask geometry, sound effect dispatch and stereo
//   pan for effects.

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
#include "lib/api/sound.h"
#include "lib/enums/sound.h"
extern "C" {

#pragma intrinsic(abs)

#pragma intrinsic(memset)
#pragma intrinsic(memcpy)


/*
 * Allocates a ring buffer record with the specified tag.
 */
// FUNCTION: FF7 0x005bfaa0
int __cdecl Battle_AllocSceneMessage(int tag)
{
    int record;

    ((int *)&g_BattleRingBufferTags)[g_BattleRingBufferWriteIndex] = tag;
    record = g_BattleRingBufferWriteIndex * 0x14 + (int)&g_BattleRingBufferRecords;
    g_BattleRingBufferWriteIndex = g_BattleRingBufferWriteIndex + 1 & 0xF;
    return record;
}

/*
 * Dequeues the next pending ring buffer record.
 */
// FUNCTION: FF7 0x005bfadc
int __cdecl Battle_DequeueSceneMessage(void *recordOut)
{
    int tag;

    if (g_BattleRingBufferWriteIndex == g_BattleRingBufferReadIndex) {
        *(int *)recordOut = (g_BattleRingBufferReadIndex - 1 & 0xF) * 0x14 + (int)&g_BattleRingBufferRecords;
        return 0;
    } else {
        tag = ((int *)&g_BattleRingBufferTags)[g_BattleRingBufferReadIndex];
        *(int *)recordOut = g_BattleRingBufferReadIndex * 0x14 + (int)&g_BattleRingBufferRecords;
        g_BattleRingBufferReadIndex = g_BattleRingBufferReadIndex + 1 & 0xF;
        return tag;
    }
}

/*
 * Initiates a screen flash ramp over the specified duration.
 */
// FUNCTION: FF7 0x005bfb47
void __cdecl BattleFx_StartScreenFlashRamp(int duration)
{
    if (g_BattleScreenFlashRampTaskHandle == 0) {
        g_BattleScreenFlashRampTaskHandle = (BattleScreenFlashRamp *)BATTLE_NEW_TASK_SLOT(BattleFx_ScreenFlashRampTask);
    }
    g_BattleScreenFlashRampTaskHandle->value = 0;
    g_BattleScreenFlashRampTaskHandle->step = 0x10000 / duration;
}

/*
 * Updates intensity and renders full-screen flash ramp quad.
 */
// FUNCTION: FF7 0x005bfb91
void __cdecl BattleFx_ScreenFlashRampTask(void)
{
    struct {
        int shade;
        BattleScreenFlashRamp *slot;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    frame.slot = (BattleScreenFlashRamp *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused == 0 && frame.slot->step != 0) {
        frame.slot->value += frame.slot->step;
        if (frame.slot->value <= 0) {
            frame.slot->ownerSlot = BATTLE_TASK_FREE;
            g_BattleScreenFlashRampTaskHandle = 0;
            return;
        }
        if (frame.slot->value > 65535) {
            frame.slot->value = 0xffff;
            frame.slot->step = 0;
        }
    }
    frame.shade = (frame.slot->value >> 8) & 0xff;
    g_BattleOverlayDrawListHead = BattleFx_FlashScreenWhite(frame.shade, frame.shade, frame.shade);
}

/*
 * Fades out the active screen flash ramp.
 */
// FUNCTION: FF7 0x005bfc3c
void __cdecl BattleFx_FadeOutScreenFlashRamp(int duration)
{
    if (g_BattleScreenFlashRampTaskHandle == 0) {
        return;
    }
    g_BattleScreenFlashRampTaskHandle->step = (0 - g_BattleScreenFlashRampTaskHandle->value) / duration;
}

/*
 * Starts a fixed-point interpolation ramp toward a target value.
 */
// FUNCTION: FF7 0x005bfc65
void __cdecl Battle_StartFixedPointRamp(int targetValue, int duration)
{
    if (g_BattleFixedPointRampTaskHandle != 0) {
        return;
    }
    g_BattleFixedPointRampTaskHandle = (BattleFixedPointRamp *)BATTLE_NEW_TASK_SLOT(Battle_FixedPointRampTaskHandler);
    g_BattleFixedPointRampTaskHandle->value = g_BattleFxTimelineState.startValue << 16;
    g_BattleFixedPointRampTaskHandle->step = ((targetValue << 16) - g_BattleFixedPointRampTaskHandle->value) / duration;
    g_BattleFixedPointRampTaskHandle->duration = duration;
}

/*
 * Updates fixed-point accumulator ramp task.
 */
// FUNCTION: FF7 0x005bfccc
void __cdecl Battle_FixedPointRampTaskHandler(void)
{
    BattleFxRampTask *task;

    task = (BattleFxRampTask *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused == 0) {
        task->value += task->step;
        g_BattleFxTimelineState.startValue = task->value >> 16;
        (task->timer)--;
        if (task->timer == 0) {
            g_BattleFixedPointRampTaskHandle = 0;
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Allocates a bit pattern traversal callback task: Battle_BitPatternCallbackTaskHandler calls
 * callback(bit, callbackArg) for each set bit of mask, all in one frame when interval is 0,
 * else one every interval frames.
 */
// FUNCTION: FF7 0x005bfd3e
void __cdecl Battle_CreateBitPatternCallbackTask(unsigned int mask, int callbackArg, int interval, void *callback)
{
    BattleBitPatternTask *taskHandle;

    taskHandle = (BattleBitPatternTask *)BATTLE_NEW_TASK_SLOT(Battle_BitPatternCallbackTaskHandler);
    taskHandle->bitIndex = 0;
    taskHandle->mask = mask;
    taskHandle->callbackArg = callbackArg;
    taskHandle->interval = interval;
    taskHandle->callback = (BattleBitPatternCallback)callback;
}

/*
 * Iterates set bits in mask and triggers callbacks sequentially.
 */
// FUNCTION: FF7 0x005bfd93
void __cdecl Battle_BitPatternCallbackTaskHandler(void)
{
    BattleBitPatternTask *task;

    task = (BattleBitPatternTask *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (task->counter == 0) {
        do {
            while ((task->mask & (1 << task->bitIndex)) == 0) {
                (task->bitIndex)++;
            }
            task->callback(task->bitIndex, task->callbackArg);
            (task->bitIndex)++;
            if (task->mask < (1 << task->bitIndex)) {
                task->bitIndex = 0xFFFF;
                return;
            }
        } while (task->interval == 0);
    }
    (task->counter)++;
    if (task->counter >= task->interval) {
        task->counter = 0;
    }
}

/*
 * Counts set bits in the low 10 bits of an actor mask.
 */
// FUNCTION: FF7 0x005bfe7f
int __cdecl Battle_PopCountActorMask(unsigned int actorMask)
{
    int bitIdx;
    int count;

    count = 0;
    for (bitIdx = 0; bitIdx < 0xA; bitIdx++) {
        if ((actorMask & 1 << bitIdx) != 0) {
            count++;
        }
    }
    return count;
}

/*
 * Computes the geometric centroid of actors matching a bit mask.
 */
// FUNCTION: FF7 0x005bfec9
short *__cdecl Battle_GetActorMaskCentroid(int mask, short *out)
{
    int maxDepth;
    int xHigh;
    int zMin;
    int minX;
    short *posPtr;
    int actorIndex;

    minX = 0x7FFF;
    zMin = 0x7FFF;
    xHigh = -0x8000;
    maxDepth = -0x8000;
    for (actorIndex = 0; actorIndex < 10; actorIndex++) {
        if ((mask & (1 << actorIndex)) != 0) {
            posPtr = &g_BattleActorRecords[actorIndex].posX;
            if (*posPtr < minX) {
                minX = *posPtr;
            }
            if (*posPtr > xHigh) {
                xHigh = *posPtr;
            }
            if (posPtr[2] < zMin) {
                zMin = posPtr[2];
            }
            if (posPtr[2] > maxDepth) {
                maxDepth = posPtr[2];
            }
        }
    }
    out[0] = (minX + xHigh) / 2;
    out[1] = 0;
    out[2] = (zMin + maxDepth) / 2;
    return out;
}

/*
 * Calculates scaled effect size for an actor using fixed-point scale.
 */
// FUNCTION: FF7 0x005bffb4
int __cdecl Battle_GetActorScaledEffectSize(int actorIndex)
{
    int effectBytes;
    int scaleValue;

    scaleValue = g_BattleActorRecords[actorIndex].radius;
    effectBytes = (scaleValue << 4) * g_BattleActorRecords[actorIndex].scale >> 12;
    return effectBytes;
}

/*
 * Dispatches sound effect opcodes to battle audio channels.
 */
// FUNCTION: FF7 0x005bfff3
void __cdecl Battle_DispatchSfxOpcodeDirect(int opcode, int pan, int id)
{
    int *args;
    int padWord;
    int unusedValue;

    args = &opcode;
    switch (opcode) {
    case SOUND_SFX_PLAY_2:
    case SOUND_SFX_PLAY_3:
    case SOUND_SFX_PLAY_4:
        SfxPlaySoundGroup(pan, id);
        break;
    case SOUND_SFX_PLAY_CH1:
        SfxPlayOnChannel(pan, id, 1);
        break;
    case SOUND_SFX_PLAY_CH2:
        SfxPlayOnChannel(pan, id, 2);
        break;
    case SOUND_SFX_PLAY_CH3:
        SfxPlayOnChannel(pan, id, 3);
        break;
    case SOUND_SFX_PLAY_CH4:
        SfxPlayOnChannel(pan, id, 4);
        break;
    default:
        /* SfxPlayUpToFour takes an int pan; this call pushes its low byte, as SfxPlayOnChannel takes it. */
        ((void(__cdecl *)(char, int, int, int, int))SfxPlayUpToFour)((char)pan, id, 0, 0, 0);
        break;
    }
}

/*
 * Projects a 3D position to screen X and computes stereo panning.
 */
// FUNCTION: FF7 0x005c00c5
int __cdecl Battle_ComputeScreenPanFromPosition(void *worldPosition)
{
    int perspective;
    int depth;
    short screenX;

    SetRotMatrix(&g_BattleCameraTransform);
    SetTransMatrix(&g_BattleCameraTransform);
    RotTransPers((struct SVECTOR *)worldPosition, &screenX, &depth, &perspective);
    if (screenX < 0) {
        screenX = 0;
    } else if (screenX >= 320) {
        screenX = 319;
    }
    return (screenX << 7) / 320;
}

/*
 * Computes audio panning for actors matching an entity mask.
 */
// FUNCTION: FF7 0x005c0132
int __cdecl Battle_ComputePanForEntity(int entityMask)
{
    int centroid[2];

    Battle_GetActorMaskCentroid(entityMask, (short *)centroid);
    return Battle_ComputeScreenPanFromPosition(centroid);
}

/*
 * Allocates and initializes a typed ring buffer record for an actor.
 */
// FUNCTION: FF7 0x005c0158
int __cdecl Battle_PostActorHitOrHurtMessage(int actorIndex)
{
    int record;

    if ((g_BattleHurtMessageActorMask & 1 << actorIndex) != 0) {
        record = Battle_AllocSceneMessage(6);
    } else {
        record = Battle_AllocSceneMessage(4);
    }
    *(short *)record = actorIndex;
    return record;
}
} /* extern "C" */
