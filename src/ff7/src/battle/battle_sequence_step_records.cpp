// src/ff7/src/battle/battle_sequence_step_records.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle sequence bookkeeping: allocates the queued sequence steps, hit records and
//   damage display records, saves and restores their cursors, and plays the queued steps. Also
//   clears and retargets actor animation holds.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/api/battle.h"
#include "lib/api/graphics.h"
#include "ff7/types/battle.h"
#include "ff7/enums/battle.h"
#include "lib/codegen.h"
#include "ff7/battle.h"
#include "lib/crt.h"
extern "C" {


struct BattleHitRecord;

// clang-format off
// GLOBAL: FF7 0x009aea98
int g_BattleSeqDamageDisplayCursor = 0;
// GLOBAL: FF7 0x009aea9c
int g_BattleSeqStepCursor = 0;
// GLOBAL: FF7 0x009aeaa0
int g_BattleSeqHitRecordCursor = 0;
// GLOBAL: FF7 0x009aeaa4
int g_BattleSeqStepCursorBackup = 0;
// GLOBAL: FF7 0x009aeaa8
int g_BattleSeqHitRecordCursorBackup = 0;
// GLOBAL: FF7 0x009aeaac
int g_BattleSeqPlaybackState = 0;
// GLOBAL: FF7 0x009aeab0
int g_BattleSeqWaitDoneState = 0;
unsigned char g_bssPad_009aeab4[4] = {0};
// clang-format on

/*
 * Saves the current sequence step and hit record cursors into their backup globals.
 */
// FUNCTION: FF7 0x00436c30
void __cdecl Battle_SnapshotSequenceCursors(void)
{
    g_BattleSeqStepCursorBackup = g_BattleSeqStepCursor;
    g_BattleSeqHitRecordCursorBackup = g_BattleSeqHitRecordCursor;
}

/*
 * Restores the sequence step and hit record cursors from their backup globals.
 */
// FUNCTION: FF7 0x00436c4b
void __cdecl Battle_RestoreSequenceCursors(void)
{
    g_BattleSeqStepCursor = g_BattleSeqStepCursorBackup;
    g_BattleSeqHitRecordCursor = g_BattleSeqHitRecordCursorBackup;
}

/*
 * Allocates and initializes the next 12-byte sequence step in g_BattleSequenceBuffer.
 * Advances g_BattleSeqStepCursor up to the maximum capacity of 64.
 */
// FUNCTION: FF7 0x00436c66
int __cdecl Battle_AllocSequenceStep(void)
{
    BattleSeqStep *step;

    step = &g_BattleSequenceBuffer.steps[g_BattleSeqStepCursor];
    step->stepFlags = 0;
    step->effectId = 0;
    step->hitRecordIndex = *(short *)&g_BattleSeqHitRecordCursor;
    if (g_BattleSeqStepCursor < 0x40) {
        g_BattleSeqStepCursor++;
    }
    return (int)step;
}

/*
 * Allocates the next 12-byte hit record in g_BattleEffectOrderTable, marking it active (0xFF).
 * Advances g_BattleSeqHitRecordCursor up to capacity 128.
 */
// FUNCTION: FF7 0x00436cb5
int __cdecl Battle_AllocHitRecord(void)
{
    BattleHitRecord *hitRecord;

    hitRecord = &g_BattleEffectOrderTable[g_BattleSeqHitRecordCursor];
    hitRecord->damageDisplayIndex = 0xFF;
    if (g_BattleSeqHitRecordCursor < 0x80) {
        g_BattleSeqHitRecordCursor++;
    }
    return (int)hitRecord;
}

/*
 * Scans allocated hit records up to g_BattleSeqHitRecordCursor and retains wait flag 4
 * only on the most recent record for each actor, clearing earlier occurrences.
 */
// FUNCTION: FF7 0x00436cf2
void __cdecl Battle_PruneSequenceWaitFlags(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[48];
        int actorId;
        struct { unsigned char pad_to_4[0x4]; int recordIndex; };
        struct { unsigned char pad_to_8[0x8]; int lastRecordByActor[10]; };
    } frame;
#pragma pack(pop)

    for (frame.recordIndex = 0; frame.recordIndex < 0xA; frame.recordIndex++) {
        frame.lastRecordByActor[frame.recordIndex] = 0xFFFFFFFF;
    }
    for (frame.recordIndex = 0; frame.recordIndex < g_BattleSeqHitRecordCursor; frame.recordIndex++) {
        frame.actorId = g_BattleEffectOrderTable[frame.recordIndex].targetActor;
        if (frame.actorId != -1 && (*(unsigned short *)&g_BattleEffectOrderTable[frame.recordIndex].flags & 4) != 0) {
            if (frame.lastRecordByActor[frame.actorId] != -1) {
                *(unsigned short *)&g_BattleEffectOrderTable[frame.lastRecordByActor[frame.actorId]].flags &= 0xFFFB;
            }
            frame.lastRecordByActor[frame.actorId] = frame.recordIndex;
        }
    }
}

/*
 * Allocates and initializes the next 14-byte damage display record in g_BattleDamageDisplayRecords,
 * writing its index into hitRecord->damageDisplayIndex and advancing the cursor modulo 128.
 */
// FUNCTION: FF7 0x00436da7
int __cdecl Battle_AllocDamageDisplayRecord(BattleHitRecord *hitRecord)
{
    BattleDamageDisplayRecord *displayRec;

    displayRec = &g_BattleDamageDisplayRecords[g_BattleSeqDamageDisplayCursor];
    hitRecord->damageDisplayIndex = *(char *)&g_BattleSeqDamageDisplayCursor;
    displayRec->combatantIndex = hitRecord->targetActor;
    displayRec->hp = 0xFFFF;
    displayRec->mp = 0xFFFF;
    g_BattleSeqDamageDisplayCursor = g_BattleSeqDamageDisplayCursor + 1 & 0x7F;
    return (int)displayRec;
}

/*
 * Allocates a hit record and marks it empty with 0xFF.
 */
// FUNCTION: FF7 0x00436dff
void __cdecl Battle_PushEmptyHitRecord(void)
{
    BattleHitRecord *hitRecord;

    hitRecord = (BattleHitRecord *)Battle_AllocHitRecord();
    hitRecord->targetActor = 0xFF;
}

/*
 * Allocates a sequence step and populates actorIdx, actionType, animId, and sequenceId.
 */
// FUNCTION: FF7 0x00436e15
void __cdecl Battle_PushSequenceStep(int actorIdx, int actionType, int animId, int sequenceId)
{
    BattleSeqStep *step;

    step = (BattleSeqStep *)Battle_AllocSequenceStep();
    step->actorIndex = actorIdx;
    step->actionType = actionType;
    step->animId = animId;
    step->power = sequenceId;
    step->targetIndex = 0xFFFF;
    step->hitRecordIndex = 0xFFFF;
}

/*
 * Sets the message timing and animation timing on the most recently allocated sequence step.
 */
// FUNCTION: FF7 0x00436e5c
void __cdecl Battle_SetSequenceStepMessageTiming(int messageTiming, int animationTiming)
{
    BattleSeqStep *step;

    if (g_BattleSeqStepCursor != 0) {
        step = &g_BattleSequenceBuffer.steps[(g_BattleSeqStepCursor - 1)];
        step->stepFlags = messageTiming;
        step->effectId = animationTiming;
    }
}

/*
 * Clears the target word to 0xFFFF on the most recently allocated sequence step.
 */
// FUNCTION: FF7 0x00436e92
void __cdecl Battle_ClearSequenceStepTargetWord(void)
{
    if (g_BattleSeqStepCursor != 0) {
        g_BattleSequenceBuffer.steps[(g_BattleSeqStepCursor - 1)].targetIndex = 0xFFFF;
    }
}

/*
 * Resets the active battle sequence step and hit record cursors, marking the sequence invalid.
 */
// FUNCTION: FF7 0x00436eb4
void __cdecl Battle_ResetSequence(void)
{
    g_BattleSeqStepCursor = 0;
    g_BattleSeqHitRecordCursor = 0;
    g_BattleSequenceBuffer.steps[0].actorIndex = 0xFF;
}

/*
 * Pops the last allocated sequence step by decrementing g_BattleSeqStepCursor.
 */
// FUNCTION: FF7 0x00436ed4
void __cdecl Battle_PopSequenceStepUnused(void)
{
    if (g_BattleSeqStepCursor != 0) {
        g_BattleSeqStepCursor--;
    }
}

/*
 * Signals the ATB wait state to battle event handling based on current freeze flags and gauge states.
 */
// FUNCTION: FF7 0x00436eef
void __cdecl Battle_SignalAtbWaitState(int state)
{
    int val;

    if ((*(unsigned short *)&g_BattleState.atbMode) > 0) {
        if (state != 0) {
            if (g_BattleState.runningActionPriority == 6) {
                val = 1;
                if (g_BattleState.actionRunSequence[6] != g_BattleState.actionEnqueueSequence[6]) {
                    val = 3;
                }
                Battle_PostEvent(BATTLE_EVENT_QUEUE_ATB, 0, BATTLE_ATB_EVENT_SET_ATB_FREEZE, val);
            }
        } else if (Battle_GetSetAtbFreezeFlag(-1) != 0) {
            Battle_PostEvent(BATTLE_EVENT_QUEUE_ATB, 0, BATTLE_ATB_EVENT_SET_ATB_FREEZE, 0);
        }
    }
}

/*
 * Plays the queued battle sequence (the steps of an action: animations, messages, hits) over as
 * many frames as it takes; callers run it each frame until it sets g_BattleSequencePlayDone. On the first
 * call (BATTLE_SEQ_PLAYBACK_START) it tells the ATB event handling whether a sequence is queued
 * (Battle_SignalAtbWaitState), closes a non-empty queue with an end step (actor index -1) and, for
 * each queued step of an enemy actor (4..9), copies the combatant's script id into the scene
 * actor. Then, every call, it runs the current step (Battle_RunSequenceStep). When that reports
 * the end (g_BattleSequenceEndReached), it empties the queue (Battle_ResetSequence), copies the script id of all
 * six enemy combatants into their scene actors again, returns to BATTLE_SEQ_PLAYBACK_START and sets g_BattleSequencePlayDone.
 * An unknown state resets to BATTLE_SEQ_PLAYBACK_START and sets g_BattleSequencePlayDone at once.
 */
// FUNCTION: FF7 0x00436f6a
void __cdecl Battle_PlaySequence(void)
{
    int stepIdx;
    BattleSeqStep *pEndStep;
    int stepActor;

    switch (g_BattleSeqPlaybackState) {
    case BATTLE_SEQ_PLAYBACK_START:
        Battle_SignalAtbWaitState(g_BattleSeqStepCursor);
        if (g_BattleSeqStepCursor != 0) {
            pEndStep = (BattleSeqStep *)Battle_AllocSequenceStep();
            pEndStep->actorIndex = 0xFF;
        }
        for (stepIdx = 0; stepIdx < 0x40; stepIdx++) {
            stepActor = g_BattleSequenceBuffer.steps[stepIdx].actorIndex;
            if (stepActor != -1) {
                if (stepActor >= 4 && stepActor < 0xA) {
                    g_BattleScene.actors[stepActor].scriptId = g_aCombatants[stepActor].scriptId;
                }
            } else {
                break;
            }
        }
        g_BattleSeqPlaybackState = BATTLE_SEQ_PLAYBACK_RUN;
    case BATTLE_SEQ_PLAYBACK_RUN:
        Battle_RunSequenceStep();
        if (g_BattleSequenceEndReached != 0) {
            g_BattleSequenceEndReached = 0;
            Battle_ResetSequence();
            for (stepIdx = 4; stepIdx < 0xA; stepIdx++) {
                g_BattleScene.actors[stepIdx].scriptId = g_aCombatants[stepIdx].scriptId;
            }
            g_BattleSeqPlaybackState = BATTLE_SEQ_PLAYBACK_START;
            g_BattleSequencePlayDone = 1;
        }
        break;
    default:
        g_BattleSeqPlaybackState = BATTLE_SEQ_PLAYBACK_START;
        g_BattleSequencePlayDone = 1;
    }
}

/*
 * Waits for battle sequence playback to complete. If a sequence is pending, initiates
 * playback; otherwise signals completion immediately.
 */
// FUNCTION: FF7 0x00437092
void __cdecl Battle_WaitSequenceDone(void)
{
    switch (g_BattleSeqWaitDoneState) {
    case BATTLE_SEQ_WAIT_START:
        if (g_BattleSeqStepCursor != 0) {
            g_BattleSeqWaitDoneState = BATTLE_SEQ_WAIT_PLAY;
            g_BattleSequenceWaitPlayed = 1;
        } else {
            g_BattleSeqWaitDoneState = BATTLE_SEQ_WAIT_START;
            g_BattleSequenceWaitDone = 1;
            g_BattleSequenceWaitPlayed = 0;
            break;
        }
    case BATTLE_SEQ_WAIT_PLAY:
        Battle_PlaySequence();
        if (g_BattleSequencePlayDone != 0) {
            g_BattleSequencePlayDone = 0;
            g_BattleSequenceWaitDone = 1;
            g_BattleSeqWaitDoneState = BATTLE_SEQ_WAIT_START;
        }
        break;
    default:
        g_BattleSequenceWaitDone = 1;
        g_BattleSeqWaitDoneState = BATTLE_SEQ_WAIT_START;
    }
}

/*
 * Clears animation hold flag 4 on all animation records matching actorIdx.
 */
// FUNCTION: FF7 0x00437131
void __cdecl Battle_ClearActorAnimHoldFlags(int actorIdx)
{
    int recordIndex;
    BattleHitRecord *record;

    for (recordIndex = 0; recordIndex < 0x80; recordIndex++) {
        record = &g_BattleEffectOrderTable[recordIndex];
        if (record->targetActor == actorIdx) {
            *(unsigned short *)&record->flags &= 0xFFFB;
        }
    }
}

/*
 * Scans animation records and retargets records with pose oldPose to newPose for actorIdx.
 * If gateFlag is 1, only updates records with bit 4 set in flags.
 */
// FUNCTION: FF7 0x00437185
void __cdecl Battle_RetargetActorAnimPose(int actorIdx, int oldPose, int newPose, int gateFlag)
{
    int recordIndex;
    BattleHitRecord *record;

    for (recordIndex = 0; recordIndex < 0x80; recordIndex++) {
        record = &g_BattleEffectOrderTable[recordIndex];
        if (record->targetActor == actorIdx && record->animId == oldPose) {
            if (gateFlag == 1) {
                if ((*(unsigned short *)&record->flags & 4) != 0) {
                    record->animId = newPose;
                }
            } else {
                record->animId = newPose;
            }
        }
    }
}

/*
 * Copies damage display values into party actor HP/MP fields if within combatant maximums.
 */
// FUNCTION: FF7 0x004371fc
void __cdecl Battle_CopyDamageDisplayValuesWithinLimits(short unused, short displayIdx)
{
    int val;

    if (displayIdx >= 0) {
        val = g_BattleDamageDisplayRecords[displayIdx].combatantIndex;
        if (val >= 0 && val < 3) {
            if ((unsigned int)g_BattleDamageDisplayRecords[displayIdx].hp <= (unsigned int)g_aCombatants[val].maxHp) {
                g_BattleState.actors[val].hp = *(short *)&g_BattleDamageDisplayRecords[displayIdx].hp;
            }
            if (g_BattleDamageDisplayRecords[displayIdx].mp <= g_aCombatants[val].maxMp) {
                g_BattleState.actors[val].mp = *(short *)&g_BattleDamageDisplayRecords[displayIdx].mp;
            }
        }
    }
}
} /* extern "C" */
