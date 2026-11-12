// src/ff7/condor/condor_sound_motion_state.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: The Fort Condor sound and music commands, together with the record-select panel, unit
//   attack and motion helpers, phase counters, input handling and the state dispatcher that drives
//   the mini-game.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/types/condor.h"
#include <stdlib.h>
#include <string.h>
#include "ff7/api/condor.h"
#include "lib/api/sound.h"
#include "ff7/enums/condor.h"
#include "lib/enums/file.h"
#include "lib/enums/sound.h"
extern "C" {

#pragma intrinsic(strcpy)
#pragma intrinsic(strcat)
#pragma intrinsic(strlen)

#pragma intrinsic(memset)

#pragma intrinsic(abs)

// clang-format off
// GLOBAL: FF7 0x00905528
int g_CondorSoundEffectIds[14] = {1, 2, 3, 4, 15, 22, 222, 225, 226, 227, 228, 230, 231, 232};
// clang-format on

/*
 * Initializes sound effect command buffer pointer.
 */
// FUNCTION: FF7 0x005fb300
void __cdecl Mini_CondorInitSfxCommandBuffer(void)
{
    g_CondorSoundCommandCursor = (struct CondorSoundCommand *)&g_CondorSoundCommandBuffer;
}

/*
 * Maps track selector code to music track ID.
 */
// FUNCTION: FF7 0x005fb30f
int __cdecl Mini_CondorMapMusicSelector(short selector)
{
    int padSlot[2]; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */

    switch (selector) {
    case CONDOR_MUSIC_SELECT_BATTLE:
        return 0x3B;
    case CONDOR_MUSIC_SELECT_FANFARE:
        return 0xA;
    case CONDOR_MUSIC_SELECT_REQUIEM:
        return 7;
    case CONDOR_MUSIC_SELECT_THEME:
        return 0x36;
    default:
        return 0x50;
    }
}

/*
 * Stops current music playback.
 */
// FUNCTION: FF7 0x005fb381
void __cdecl Mini_CondorStopMusic(void)
{
    g_CondorSoundCommandCursor->command = SOUND_MUSIC_STOP;
    SoundDispatchCommand((short)g_CondorSoundCommandCursor->command, g_CondorSoundCommandCursor->param, 0, 0, 0, 0, 0, 0, 0);
}

/*
 * Plays specified music track ID.
 */
// FUNCTION: FF7 0x005fb3b9
void __cdecl Mini_CondorPlayMusicTrack(short trackId)
{
    g_CondorSoundCommandCursor->command = SOUND_MUSIC_PLAY;
    g_CondorSoundCommandCursor->param = trackId;
    SoundDispatchCommand((short)g_CondorSoundCommandCursor->command, g_CondorSoundCommandCursor->param, 0, 0, 0, 0, 0, 0, 0);
}

/*
 * Maps track selector and begins playback.
 */
// FUNCTION: FF7 0x005fb3fe
void __cdecl Mini_CondorPlayMappedMusic(int selector)
{
    int track;

    track = Mini_CondorMapMusicSelector((short)selector);
    g_CondorSoundCommandCursor->command = SOUND_MUSIC_PLAY;
    g_CondorSoundCommandCursor->param = track;
    SoundDispatchCommand((short)g_CondorSoundCommandCursor->command, g_CondorSoundCommandCursor->param, 0, 0, 0, 0, 0, 0, 0);
}

/*
 * Submits sound effect playback command.
 */
// FUNCTION: FF7 0x005fb456
void __cdecl Mini_CondorSubmitSfxPlay(short soundId, short channel)
{
    g_CondorSoundCommandCursor->command = channel + (SOUND_SFX_PLAY_CH1 - 1);
    g_CondorSoundCommandCursor->param = 0x40;
    g_CondorSoundCommandCursor->soundId = soundId;
    SoundDispatchCommand((short)g_CondorSoundCommandCursor->command, g_CondorSoundCommandCursor->param, g_CondorSoundCommandCursor->soundId, 0, 0, 0, 0, 0, 0);
}

/*
 * Restarts sound effect centered.
 */
// FUNCTION: FF7 0x005fb4b6
void __cdecl Mini_CondorRestartSfxCentered(int soundId, int channel)
{
    Mini_CondorSubmitSfxPlay(0, (short)channel);
    Mini_CondorSubmitSfxPlay((short)soundId, (short)channel);
}

/*
 * Plays sound effect with stereo panning.
 */
// FUNCTION: FF7 0x005fb4dc
void __cdecl Mini_CondorPlaySfxPanned(short soundId, short channel, short posX)
{
    g_CondorSoundCommandCursor->command = channel + (SOUND_SFX_PLAY_CH1 - 1);
    g_CondorSoundCommandCursor->param = (posX & 0x1FF) >> 2;
    g_CondorSoundCommandCursor->soundId = soundId;
    SoundDispatchCommand((short)g_CondorSoundCommandCursor->command, g_CondorSoundCommandCursor->param, g_CondorSoundCommandCursor->soundId, 0, 0, 0, 0, 0, 0);
}

/*
 * Restarts sound effect with stereo panning.
 */
// FUNCTION: FF7 0x005fb544
void __cdecl Mini_CondorRestartSfxPanned(int soundId, int channel, short posX)
{
    Mini_CondorSubmitSfxPlay(0, (short)channel);
    Mini_CondorPlaySfxPanned((short)soundId, (short)channel, (short)posX);
}

/*
 * Preloads sound effect buffers for Fort Condor.
 */
// FUNCTION: FF7 0x005fb56f
void __cdecl Mini_CondorLoadSoundTable(void)
{
    unsigned short i;

    for (i = 0; i < 14; i++) {
        SfxLoadIntoBuffer(g_CondorSoundEffectIds[i], 0);
    }
}

/*
 * Releases cached sound effect audio buffers.
 */
// FUNCTION: FF7 0x005fb5b6
void __cdecl Mini_CondorReleaseCachedSfxBuffers(void)
{
    unsigned short i;

    for (i = 0; i < 14; i++) {
        Sfx_ReleaseCachedBuffer(g_CondorSoundEffectIds[i], 0xF);
    }
}

/*
 * Advances slot movement along path.
 */
// FUNCTION: FF7 0x005fb5fd
void __cdecl Mini_CondorAdvanceSlotMotion(CondorSlotMotion *slot)
{
    unsigned short delta;
    unsigned short index;

    for (index = 0; index < 8; index++) {
        if (slot->posX == slot->targetX) {
            slot->pendingDeltaX = 0;
        } else {
            delta = *(unsigned char *)&slot->carryX & 0xFF;
            delta += slot->pendingDeltaX;
            slot->carryX = delta;
            delta = delta >> 8;
            if (delta != 0) {
                if (delta >= 128) {
                    delta |= 0xFF00;
                }
                slot->posX += delta;
            }
        }
        if (slot->posY == slot->targetY) {
            slot->pendingDeltaY = 0;
        } else {
            delta = *(unsigned char *)&slot->carryY & 0xFF;
            delta += slot->pendingDeltaY;
            slot->carryY = delta;
            delta = delta >> 8;
            if (delta != 0) {
                if (delta >= 128) {
                    delta |= 0xFF00;
                }
                slot->posY += delta;
            }
        }
    }
}

/*
 * Empty motion callback.
 */
// FUNCTION: FF7 0x005fb74f
void __cdecl Mini_CondorMainState0FNoOp(void)
{
}

/*
 * Updates unit record selection HUD panel.
 */
// FUNCTION: FF7 0x005fb754
void __cdecl Mini_CondorUpdateRecordSelectPanel(void)
{
    CondorRecordSelectPanel *pPanel;
    CondorSlotMotion *motion;
    short i;
    short remaining;

    switch (g_CondorSelectPanelPhase) {
    case CONDOR_PANEL_OPEN:
        motion = (CondorSlotMotion *)g_CondorRunRecordLayouts;
        remaining = g_CondorRecordLayoutCount;
        for (i = 0; i < g_CondorRecordLayoutCount; i++) {
            if ((motion->pendingDeltaX | motion->pendingDeltaY) == 0) {
                remaining -= 1;
            } else {
                Mini_CondorAdvanceSlotMotion(motion);
                pPanel = (CondorRecordSelectPanel *)(motion->panelPtr);
                pPanel->posX = motion->posX;
                pPanel->posY = motion->posY;
            }
            motion++;
        }
        if (remaining == 0) {
            g_CondorSelectPanelPhase = CONDOR_PANEL_BROWSE;
        }
        break;
    case CONDOR_PANEL_BROWSE:
        if ((g_CondorFrameInput & INPUT_RIGHT) != 0) {
            Mini_CondorSubmitSfxPlay(SFX_MENU_CURSOR, 4);
            g_CondorLayoutIndex += 1;
            if (g_CondorLayoutIndex >= g_CondorRecordLayoutCount) {
                g_CondorLayoutIndex = 0;
            }
            motion = (CondorSlotMotion *)(g_CondorLayoutIndex * 0x2C + (char *)g_CondorRunRecordLayouts);
            g_CondorSelectedRecord = (motion->panelPtr - (int)&g_CondorRecordTable.records[0].status) / 0x78;
            Mini_CondorSetSelectedReadout(g_CondorRecordTable.records[g_CondorSelectedRecord].unitIndex, g_CondorSelectedRecord);
            Mini_CondorRefreshActiveRecordDisplay();
        }
        if ((g_CondorFrameInput & INPUT_LEFT) != 0) {
            Mini_CondorSubmitSfxPlay(SFX_MENU_CURSOR, 4);
            g_CondorLayoutIndex -= 1;
            if (g_CondorLayoutIndex < 0) {
                g_CondorLayoutIndex = g_CondorRecordLayoutCount - 1;
            }
            motion = (CondorSlotMotion *)(g_CondorLayoutIndex * 0x2C + (char *)g_CondorRunRecordLayouts);
            g_CondorSelectedRecord = (motion->panelPtr - (int)&g_CondorRecordTable.records[0].status) / 0x78;
            Mini_CondorSetSelectedReadout(g_CondorRecordTable.records[g_CondorSelectedRecord].unitIndex, g_CondorSelectedRecord);
            Mini_CondorRefreshActiveRecordDisplay();
        }
        if ((g_CondorInputPressed & (INPUT_CIRCLE | INPUT_CROSS)) != 0) {
            if ((g_CondorInputPressed & INPUT_CIRCLE) != 0) {
                g_CondorLayoutStart = 1;
                Mini_CondorSubmitSfxPlay(SFX_MENU_CURSOR, 4);
            } else {
                g_CondorLayoutStart = 2;
                Mini_CondorSubmitSfxPlay(SFX_MENU_CANCEL, 4);
            }
            motion = (CondorSlotMotion *)g_CondorRunRecordLayouts;
            for (i = 0; i < g_CondorRecordLayoutCount; i++) {
                pPanel = (CondorRecordSelectPanel *)(motion->panelPtr);
                Mini_CondorBeginLinearMotion(pPanel->posX, pPanel->posY, motion->destX, motion->destY);
                motion->pendingDeltaX = (*(short *)&g_CondorVecX);
                motion->pendingDeltaY = *(short *)&g_CondorVecY;
                motion->carryY = 0;
                motion->carryX = 0;
                motion->targetX = motion->destX;
                motion->targetY = motion->destY;
                motion->targetZ = motion->sourceX;
                pPanel->layoutX = motion->sourceX;
                motion++;
            }
            g_CondorSelectPanelPhase = CONDOR_PANEL_CLOSING;
        } else {
            motion = (CondorSlotMotion *)g_CondorRunRecordLayouts;
            for (i = 0; i < g_CondorRecordLayoutCount; i++) {
                pPanel = (CondorRecordSelectPanel *)(motion->panelPtr);
                motion++;
            }
        }
        break;
    case CONDOR_PANEL_CLOSING:
        motion = (CondorSlotMotion *)g_CondorRunRecordLayouts;
        remaining = g_CondorRecordLayoutCount;
        for (i = 0; i < g_CondorRecordLayoutCount; i++) {
            if ((motion->pendingDeltaX | motion->pendingDeltaY) == 0) {
                remaining -= 1;
            } else {
                Mini_CondorAdvanceSlotMotion(motion);
                pPanel = (CondorRecordSelectPanel *)(motion->panelPtr);
                pPanel->posX = motion->posX;
                pPanel->posY = motion->posY;
            }
            motion++;
        }
        if (remaining == 0) {
            motion = (CondorSlotMotion *)g_CondorRunRecordLayouts;
            for (i = 0; i < g_CondorRecordLayoutCount; i++) {
                pPanel = (CondorRecordSelectPanel *)(motion->panelPtr);
                pPanel->posX = motion->destX;
                pPanel->posY = motion->destY;
                pPanel->layoutX = motion->sourceX;
                pPanel->rotY = motion->savedRotY;
                pPanel->angle = motion->savedAngle;
                motion++;
            }
            if (g_CondorLayoutStart == 1) {
                g_CondorSelectedRecord = *(short *)(g_CondorLayoutIndex * 0x2C + ((char *)g_CondorRunRecordLayouts + 0x1e));
            } else {
                g_CondorSelectedRecord = g_CondorSavedSelectedRecord;
            }
            Mini_CondorSetSelectedReadout(g_CondorRecordTable.records[g_CondorSelectedRecord].unitIndex, g_CondorSelectedRecord);
            Mini_CondorRefreshActiveRecordDisplay();
            g_CondorMainState = CONDOR_STATE_IDLE;
            g_CondorSelectPanelPhase = CONDOR_PANEL_CLOSED;
            if (g_CondorRecordMode == CONDOR_RECORD_MODE_CURSOR) {
                if (g_CondorLayoutStart == 1) {
                    if (g_CondorRecordTable.records[g_CondorSelectedRecord].unitIndex < 16) {
                        Mini_CondorOpenSelectedRecordActionMenu();
                    } else {
                        Mini_CondorRequestDisplayMode1();
                    }
                } else {
                    Mini_CondorRequestDisplayMode1();
                }
            }
            if (g_CondorRecordMode == CONDOR_RECORD_MODE_TARGET) {
                if (g_CondorLayoutStart == 1) {
                    g_CondorPickedRecordIndex = g_CondorSelectedRecord;
                }
                g_CondorSelectedRecord = g_CondorSavedSelectedRecord;
            }
            g_CondorLayoutCounter += 1;
        }
        break;
    }
}

/*
 * Help screen (CONDOR_STATE_HELP_SCREEN): when Select is latched again, restores the camera and
 * returns to CONDOR_STATE_IDLE.
 */
// FUNCTION: FF7 0x005fbcdf
void __cdecl Mini_CondorBeginRecordSelect(void)
{
    g_CondorSelectPanelPhase = CONDOR_PANEL_OPEN;
    if (Input_GetLatchedSelect() != 0) {
        Mini_CondorSubmitSfxPlay(SFX_MENU_CURSOR, 4);
        *(short *)&g_CondorCameraX = g_CondorSavedCameraX;
        *(short *)&g_CondorCameraY = g_CondorSavedCameraY;
        g_CondorMainState = CONDOR_STATE_IDLE;
        g_CondorSelectPanelPhase = CONDOR_PANEL_CLOSED;
    }
}

/*
 * Updates combat attack sequence between units.
 */
// FUNCTION: FF7 0x005fbd2f
short __cdecl Mini_CondorAdvanceUnitAttack(int attackerSlot, int targetSlot)
{
    short result;
    CondorRecord *pSpawnRecord;
    CondorRecord *linkedRecord;
    CondorRecord *pAttackerRecord;
    short freeSlot;
    short spawnIndex;
    short state;
    CondorRecord *targetPtr;
    short damage;

    pAttackerRecord = &g_CondorRecordTable.records[attackerSlot];
    targetPtr = &g_CondorRecordTable.records[targetSlot];
    state = pAttackerRecord->inContact;
    switch (state) {
    case CONDOR_ATTACK_STEP_BEGIN:
        pAttackerRecord->sequenceIndex = 2;
        pAttackerRecord->flags = pAttackerRecord->flags | 2;
        result = Mini_CondorBeginRecordSequence(pAttackerRecord);
        pAttackerRecord->inContact = CONDOR_ATTACK_STEP_SWING;
        Mini_CondorRestartUnitSound(pAttackerRecord);
        pAttackerRecord->attackTarget = 0xFF;
        spawnIndex = Mini_CondorSpawnRecordAtSource(pAttackerRecord, *(short *)&pAttackerRecord->unitIndex);
        if (spawnIndex != -1) {
            pAttackerRecord->attackTarget = spawnIndex;
            pSpawnRecord = &g_CondorRecordTable.records[spawnIndex];
            pSpawnRecord->sequenceIndex = 7;
            pSpawnRecord->timerReload = 1;
            pSpawnRecord->actionCountdown = 1;
            pSpawnRecord->attackerSlot = attackerSlot;
            result = Mini_CondorBeginRecordSequence(pSpawnRecord);
            if (result == 0) {
                pSpawnRecord->status = 0;
            }
        }
        break;
    case CONDOR_ATTACK_STEP_SWING:
        if (pAttackerRecord->sequenceCommand == 0xFF) {
            pAttackerRecord->sequenceIndex = 0;
            result = Mini_CondorBeginRecordSequence(pAttackerRecord);
            pAttackerRecord->inContact = CONDOR_ATTACK_STEP_HIT;
            if (g_CondorMainState == CONDOR_STATE_UNIT_ATTACK && pAttackerRecord->attackTarget != 0xFF) {
                linkedRecord = (CondorRecord *)&g_CondorRecordTable.records[pAttackerRecord->attackTarget];
                linkedRecord->status = 0;
            }
        } else if (g_CondorMainState == CONDOR_STATE_UNIT_ATTACK && pAttackerRecord->attackTarget != 0xFF) {
            linkedRecord = (CondorRecord *)&g_CondorRecordTable.records[pAttackerRecord->attackTarget];
            if (linkedRecord->status != 0) {
                Mini_CondorRampUnitTurnRate(linkedRecord);
            }
        }
        break;
    case CONDOR_ATTACK_STEP_HIT:
        freeSlot = Mini_CondorFindInactiveCounterEffectSlot();
        if (freeSlot == 0xFF) { /* empty first arm: lib/codegen.h */
        } else {
            g_CondorCounterEffects[freeSlot].counter = 1;
            g_CondorCounterEffects[freeSlot].displayNumber = Mini_CondorLookupScaledPairByte(pAttackerRecord, targetPtr);
            g_CondorCounterEffects[freeSlot].posX = targetPtr->posX;
            g_CondorCounterEffects[freeSlot].posY = targetPtr->posY;
            Mini_CondorSetRecordTwoDigits(freeSlot, g_CondorCounterEffects[freeSlot].displayNumber);
        }
        targetPtr->flags = targetPtr->flags | 2;
        if (targetPtr->sequenceIndex == 2 || targetPtr->sequenceIndex == 3) {
            pAttackerRecord->inContact = CONDOR_ATTACK_STEP_RESOLVE;
        } else {
            targetPtr->sequenceIndex = 3;
            result = Mini_CondorBeginRecordSequence(targetPtr);
            pAttackerRecord->inContact = CONDOR_ATTACK_STEP_TARGET_REACT;
        }
        break;
    case CONDOR_ATTACK_STEP_TARGET_REACT:
        if (targetPtr->sequenceCommand == 0xFF || targetPtr->sequenceIndex != 3) {
            if (targetPtr->sequenceIndex != 2 && targetPtr->sequenceIndex != 3) {
                targetPtr->sequenceIndex = 0;
                result = Mini_CondorBeginRecordSequence(targetPtr);
            }
            pAttackerRecord->inContact = CONDOR_ATTACK_STEP_RESOLVE;
        }
        break;
    case CONDOR_ATTACK_STEP_RESOLVE:
        pAttackerRecord->inContact = CONDOR_ATTACK_STEP_NONE;
        pAttackerRecord->bufferIndex = 120;
        pAttackerRecord->sequenceIndex = 0;
        result = Mini_CondorBeginRecordSequence(pAttackerRecord);
        state = targetPtr->counter;
        damage = Mini_CondorLookupScaledPairByte(pAttackerRecord, targetPtr);
        state -= damage;
        if (state < 0) {
            state = 0;
        }
        targetPtr->counter = state;
        if (g_CondorSelectedRecord != -1) {
            Mini_CondorRefreshActiveRecordDisplay();
        }
        if (state == 0) {
            if (targetPtr->unitIndex >= 16) {
                if (g_CondorSelectedRecord == -1) {
                    g_CondorSelectedRecord = ((int)pAttackerRecord - (int)&g_CondorRecordTable.records[0].status) / 0x78;
                    Mini_CondorRefreshActiveRecordDisplay();
                    g_CondorSelectedRecord = 0xFFFF;
                }
                Mini_CondorStartRecordAction(10, ((int)pAttackerRecord - (int)&g_CondorRecordTable.records[0].status) / 0x78);
            }
            if ((int)targetPtr == pAttackerRecord->contactSlot * 0x78 + (int)&g_CondorRecordTable.records[0].status) {
                pAttackerRecord->contactSlot = 0xFF;
                pAttackerRecord->targetY = 0;
                pAttackerRecord->targetX = 0;
                pAttackerRecord->velY = 0;
                pAttackerRecord->velX = 0;
                pAttackerRecord->flags = pAttackerRecord->flags & 0xFC;
            }
            if (targetPtr->retired == 0) {
                targetPtr->retired = 0xFF;
                if (targetPtr->unitIndex < 16) {
                    g_CondorPlayerUnitCount--;
                } else {
                    g_CondorSpawnCount--;
                }
                Mini_CondorBuildDigitDisplayRecords();
            }
            return CONDOR_ATTACK_TARGET_DEFEATED;
        }
        if (targetPtr->sequenceIndex != 2 && targetPtr->sequenceIndex != 3) {
            targetPtr->sequenceIndex = 0;
            result = Mini_CondorBeginRecordSequence(targetPtr);
        }
        return CONDOR_ATTACK_DONE;
    }
    return CONDOR_ATTACK_RUNNING;
}

/*
 * Plays attack or unit action sound effect.
 */
// FUNCTION: FF7 0x005fc1f2
void __cdecl Mini_CondorRestartUnitSound(CondorRecord *unitRecord)
{
    switch (unitRecord->unitIndex) {
    case CONDOR_UNIT_ENEMY_SHOOTER:
        Mini_CondorRestartSfxPanned(SFX_ID_CONDOR_FAST_SHOT_ALT, 3, (short)unitRecord->posX);
        break;
    case CONDOR_UNIT_SHOOTER:
        Mini_CondorRestartSfxPanned(SFX_ID_CONDOR_FAST_SHOT, 3, (short)unitRecord->posX);
        break;
    case CONDOR_UNIT_REPAIRER:
    case CONDOR_UNIT_WORKER:
    case CONDOR_UNIT_ENEMY_BEAST:
        Mini_CondorRestartSfxPanned(SFX_ID_CONDOR_ALLY_HIT, 3, (short)unitRecord->posX);
        break;
    case CONDOR_UNIT_BOSS_WYVERN:
        Mini_CondorRestartSfxPanned(SFX_ID_CONDOR_BOSS_STEP, 3, (short)unitRecord->posX);
        break;
    default:
        Mini_CondorRestartSfxPanned(SFX_ID_CONDOR_UNIT_STEP, 3, (short)unitRecord->posX);
    }
}

/*
 * Resets active unit pair and select state.
 */
// FUNCTION: FF7 0x005fc2ca
void __cdecl Mini_CondorResetUnitPairAndSelect(int firstSlot, int secondSlot)
{
    g_CondorMainState = CONDOR_STATE_IDLE;
    g_CondorSelectPanelPhase = CONDOR_PANEL_CLOSED;
    g_CondorRecordTable.records[firstSlot].inContact = CONDOR_ATTACK_STEP_NONE;
    g_CondorRecordTable.records[secondSlot].inContact = CONDOR_ATTACK_STEP_NONE;
    if (g_CondorDeselectAfterAction == 0) {
        g_CondorSelectedRecord = g_CondorPairUnitIndex;
        if (g_CondorRecordTable.records[g_CondorSelectedRecord].status != 0 && g_CondorRecordTable.records[g_CondorSelectedRecord].retired == 0) {
            g_CondorMatchListState.cursorActive = 0;
            if ((short)Mini_CondorIsPairableUnitKind(&g_CondorRecordTable.records[g_CondorPairUnitIndex]) != 0) {
                Mini_CondorOpenSelectedRecordActionMenu();
            } else {
                Mini_CondorRequestDisplayMode1();
            }
        } else {
            Mini_CondorRequestDisplayMode1();
        }
    } else {
        g_CondorRecordMode = CONDOR_RECORD_MODE_CURSOR;
        g_CondorSelectedRecord = 0xFFFF;
        Mini_CondorRequestDisplayMode1();
    }
    g_CondorDeselectAfterAction = 0;
}

/*
 * Sets up motion ramp step limit.
 */
// FUNCTION: FF7 0x005fc39f
void __cdecl Mini_CondorArmRampLimit(void)
{
    g_CondorRampLimit = 1;
    g_CondorRampLimitTarget = 0x10;
}

/*
 * Steps fractional motion calculation.
 */
// FUNCTION: FF7 0x005fc3b6
void __cdecl Mini_CondorStepFractionalMotion(void)
{
    g_CondorMoveFlags = 0;
    g_CondorMotionAccX += g_CondorMotionStepX;
    g_CondorMotionAccY += g_CondorMotionStepY;
    if (g_CondorMotionStepX == 0) {
        g_CondorMotionStepsX = 0;
    } else if ((g_CondorMotionAccX & 0xFF00) != 0) {
        if (g_CondorLinearMotionSaved != 0) {
            g_CondorMoveFlags |= INPUT_LEFT;
        } else {
            g_CondorMoveFlags |= INPUT_RIGHT;
        }
        if (g_CondorMotionStepsX != 0) {
            g_CondorMotionStepsX--;
        } else {
            g_CondorMotionAccX = 0;
            g_CondorMotionStepX = g_CondorMotionAccX;
        }
    }
    if (g_CondorMotionStepY == 0) {
        g_CondorMotionStepsY = 0;
    } else if ((g_CondorMotionAccY & 0xFF00) != 0) {
        if (g_CondorLinearMotionActive != 0) {
            g_CondorMoveFlags |= INPUT_UP;
        } else {
            g_CondorMoveFlags |= INPUT_DOWN;
        }
        if (g_CondorMotionStepsY != 0) {
            g_CondorMotionStepsY--;
        } else {
            g_CondorMotionAccY = 0;
            g_CondorMotionStepY = g_CondorMotionAccY;
        }
    }
    g_CondorMotionAccX &= 0xFF;
    g_CondorMotionAccY &= 0xFF;
    Mini_CondorMoveActiveCursor(g_CondorMoveFlags);
    if (((g_CondorMotionStepsX | g_CondorMotionStepsY) & 0xFFFF) == 0) {
        g_CondorMainState = CONDOR_STATE_IDLE;
    }
}

/*
 * Initiates linear motion between coordinates.
 */
// FUNCTION: FF7 0x005fc52b
void __cdecl Mini_CondorBeginLinearMotion(int startX, int startY, int endX, int endY)
{
    int scaled;

    g_CondorMotionAccY = 0;
    g_CondorMotionAccX = g_CondorMotionAccY;
    g_CondorLinearMotionActive = 0;
    g_CondorLinearMotionSaved = g_CondorLinearMotionActive;
    g_CondorMotionStepsX = endX - startX;
    if (g_CondorMotionStepsX < 0) {
        g_CondorLinearMotionSaved = 1;
        g_CondorMotionStepsX = -g_CondorMotionStepsX;
    }
    g_CondorMotionStepsY = endY - startY;
    if (g_CondorMotionStepsY < 0) {
        g_CondorLinearMotionActive = 1;
        g_CondorMotionStepsY = -g_CondorMotionStepsY;
    }
    if (g_CondorMotionStepsX >= g_CondorMotionStepsY) {
        g_CondorVecX = 256;
        scaled = g_CondorMotionStepsY << 8;
        if (scaled == 0) {
            g_CondorVecY = 1;
        } else {
            g_CondorVecY = scaled / g_CondorMotionStepsX;
        }
        if (g_CondorVecY == 0) {
            g_CondorVecY = 1;
        }
    } else {
        g_CondorVecY = 256;
        scaled = g_CondorMotionStepsX << 8;
        if (scaled == 0) {
            g_CondorVecX = 1;
        } else {
            g_CondorVecX = scaled / g_CondorMotionStepsY;
        }
        if (g_CondorVecX == 0) {
            g_CondorVecX = 1;
        }
    }
    if (g_CondorLinearMotionSaved != 0) {
        g_CondorVecX = -g_CondorVecX;
    }
    if (g_CondorLinearMotionActive != 0) {
        g_CondorVecY = -g_CondorVecY;
    }
}

/*
 * Calculates angle from directional vector components.
 */
// FUNCTION: FF7 0x005fc684
short __cdecl Mini_CondorVectorToAngle(CondorCursor *vector)
{
    short yNegBit;
    short mx;
    short my;
    short angle;

    yNegBit = 0;
    angle = yNegBit;
    mx = vector->x;
    if (mx < 0) {
        mx = -mx;
        angle++;
    }
    my = vector->y;
    if (my < 0) {
        my = -my;
        yNegBit = 2;
    }
    switch (angle | yNegBit) {
    case CONDOR_QUADRANT_POS_X_POS_Y:
        if (mx == 0x100) {
            angle = my * 2 + 0xC00;
        } else {
            angle = 0x1000 - (mx << 1);
        }
        break;
    case CONDOR_QUADRANT_POS_X_NEG_Y:
        if (my == 0x100) {
            angle = mx << 1;
        } else {
            angle = 0x400 - (my << 1);
        }
        break;
    case CONDOR_QUADRANT_NEG_X_POS_Y:
        if (my == 0x100) {
            angle = mx * 2 + 0x800;
        } else {
            angle = 0xC00 - (my << 1);
        }
        break;
    case CONDOR_QUADRANT_NEG_X_NEG_Y:
        if (mx == 0x100) {
            angle = my * 2 + 0x400;
        } else {
            angle = 0x800 - (mx << 1);
        }
        break;
    default:
        angle = 0;
    }
    return (short)angle;
}

/*
 * Converts angle to X and Y step components.
 */
// FUNCTION: FF7 0x005fc7de
void __cdecl Mini_CondorAngleToStepComponents(int angle)
{
    short offset;
    offset = (*(short *)&angle & 0x1ff) >> 1;
    switch (*(short *)&angle & 0xe00) {
    case CONDOR_OCTANT_N_NW:
        g_CondorVecX = -offset;
        g_CondorVecY = 256;
        break;
    case CONDOR_OCTANT_NW_W:
        g_CondorVecX = -256;
        g_CondorVecY = 255 - offset;
        break;
    case CONDOR_OCTANT_W_SW:
        g_CondorVecX = -256;
        g_CondorVecY = -offset;
        break;
    case CONDOR_OCTANT_SW_S:
        g_CondorVecX = -(255 - offset);
        g_CondorVecY = -256;
        break;
    case CONDOR_OCTANT_S_SE:
        g_CondorVecX = offset;
        g_CondorVecY = -256;
        break;
    case CONDOR_OCTANT_SE_E:
        g_CondorVecX = 256;
        g_CondorVecY = -(255 - offset);
        break;
    case CONDOR_OCTANT_E_NE:
        g_CondorVecX = 256;
        g_CondorVecY = offset;
        break;
    case CONDOR_OCTANT_NE_N:
        g_CondorVecX = 255 - offset;
        g_CondorVecY = 256;
        break;
    }
}

/*
 * Dispatches current Fort Condor game state handler.
 */
// FUNCTION: FF7 0x005fc96f
void __cdecl Mini_CondorDispatchStateHandler(void)
{
    switch (g_CondorMainState) {
    case CONDOR_STATE_CLEAR:
        Mini_CondorClearStateWord();
        break;
    case CONDOR_STATE_CURSOR_GLIDE_TO_ACTION:
        Mini_CondorStepAndDispatchRequest();
        break;
    case CONDOR_STATE_CURSOR_GLIDE:
        Mini_CondorStepUntilIdle();
        break;
    case CONDOR_STATE_INTRO:
        Mini_CondorRunPhasedMotion();
        break;
    case CONDOR_STATE_UNIT_ATTACK:
        Mini_CondorBeginUnitAttack();
        break;
    case CONDOR_STATE_UNIT_COUNTERATTACK:
        Mini_CondorTickUnitAttack();
        break;
    case CONDOR_STATE_RECORD_SELECT_WHEEL:
        Mini_CondorTickRecordSelectWheel();
        break;
    case CONDOR_STATE_RECORD_ANGLE_INPUT:
        Mini_CondorTickRecordAngleInput();
        break;
    case CONDOR_STATE_START_MENU:
        Mini_CondorProcessInputFlags();
        break;
    case CONDOR_STATE_CONFIRM_ROWS:
        Mini_CondorHandleRecordSelectInput();
        break;
    case CONDOR_STATE_END_BANNER_EXIT_FLAGGED:
        Mini_CondorTickCounterOrRequestState4();
        break;
    case CONDOR_STATE_END_BANNER_EXIT:
        Mini_CondorTickCounterOrRequestState5();
        break;
    case CONDOR_STATE_CURSOR_RAMP:
        Mini_CondorRampStepMotion();
        break;
    case CONDOR_STATE_HELP_SCREEN:
        Mini_CondorBeginRecordSelect();
        break;
    case CONDOR_STATE_RECORD_PANEL:
        Mini_CondorMainState0FNoOp();
        break;
    case CONDOR_STATE_RECORD_ANGLE_COMMIT:
        Mini_CondorTickRecordAngleCommitOrRestore();
        break;
    case CONDOR_STATE_BANNER_GROW:
        Mini_CondorAdvanceCounterToTarget();
    }
}

/*
 * Increments phase counter towards target value.
 */
// FUNCTION: FF7 0x005fca51
void __cdecl Mini_CondorAdvanceCounterToTarget(void)
{
    g_CondorBannerCounter += 4;
    if (g_CondorBannerCounter >= g_CondorBannerTarget) {
        g_CondorBannerCounter = g_CondorBannerTarget;
        g_CondorMainState = CONDOR_STATE_IDLE;
    }
}

/*
 * Ticks the end banner for 360 frames, then sets the exit word to CONDOR_STAGE_EXIT.
 */
// FUNCTION: FF7 0x005fca8e
void __cdecl Mini_CondorTickCounterOrRequestState5(void)
{
    if (g_CondorPhaseFrames < 360) {
        g_CondorPhaseFrames++;
        g_CondorBannerCounter++;
        if (g_CondorBannerCounter >= 64) {
            g_CondorBannerCounter = 64;
        }
    } else {
        g_CondorStage = CONDOR_STAGE_EXIT;
    }
}

/*
 * Ticks the end banner; after 600 frames, or Circle or Cross once it is full, sets the exit
 * word to CONDOR_STAGE_EXIT_FLAGGED.
 */
// FUNCTION: FF7 0x005fcae6
void __cdecl Mini_CondorTickCounterOrRequestState4(void)
{
    short val;

    val = 0;
    val = 0;
    g_CondorPhaseFrames++;
    if (g_CondorPhaseFrames > 600) {
        val++;
    }
    g_CondorBannerCounter++;
    if (g_CondorBannerCounter >= 64) {
        g_CondorBannerCounter = 64;
        if ((g_CondorInputPressed & (INPUT_CIRCLE | INPUT_CROSS)) != 0) {
            val++;
        }
    }
    if (val != 0) {
        g_CondorStage = CONDOR_STAGE_EXIT_FLAGGED;
    }
}

/*
 * Processes player input flags during battle.
 */
// FUNCTION: FF7 0x005fcb75
void __cdecl Mini_CondorProcessInputFlags(void)
{
    g_CondorSelectPanelPhase = CONDOR_PANEL_OPEN;
    if ((g_CondorInputMask & (INPUT_UP | INPUT_RIGHT | INPUT_DOWN | INPUT_LEFT)) != 0) {
        Mini_CondorScrollCamera(g_CondorInputMask);
        Mini_CondorScrollCamera(g_CondorInputMask);
        Mini_CondorScrollCamera(g_CondorInputMask);
        Mini_CondorScrollCamera(g_CondorInputMask);
        if ((g_CondorInputMask & INPUT_SQUARE) != 0) {
            Mini_CondorScrollCamera(g_CondorInputMask);
            Mini_CondorScrollCamera(g_CondorInputMask);
            Mini_CondorScrollCamera(g_CondorInputMask);
            Mini_CondorScrollCamera(g_CondorInputMask);
        }
    }
    if ((g_CondorInputPressed & INPUT_START) != 0) {
        Mini_CondorSubmitSfxPlay(SFX_MENU_CURSOR, 4);
        if (g_CondorOutcomeIndex != g_CondorPrevOutcomeIndex && g_CondorActionSelectState == 0) {
            g_CondorStage = CONDOR_STAGE_RESTART;
        } else {
            g_CondorMainState = CONDOR_STATE_IDLE;
            g_CondorSelectPanelPhase = CONDOR_PANEL_CLOSED;
            if (g_CondorActionSelectState != 0) {
                g_CondorActionSelectState = 0;
            } else {
                g_CondorActionSelectState = 1;
            }
            g_CondorDisplayMode = g_CondorSavedDisplayMode;
            if (g_CondorDisplayMode == CONDOR_DISPLAY_FULL) {
                Mini_CondorRequestDisplayMode1();
            } else {
                Mini_CondorRequestDisplayMode2();
            }
            Mini_CondorRequestDisplayMode1();
            Mini_CondorBuildDigitDisplayRecords();
            Mini_CondorClearActiveRecord();
            g_CondorMatchListState.cursorActive = 1;
            g_CondorCursorPos.x = g_CondorCursorScreenX + g_CondorCameraX;
            g_CondorCursorPos.y = g_CondorCursorScreenY + g_CondorCameraY;
        }
    }
    if ((g_CondorInputPressed & INPUT_TRIANGLE) != 0) {
        Mini_CondorSubmitSfxPlay(SFX_MENU_CURSOR, 4);
        g_CondorStage = CONDOR_STAGE_SWITCH_MODE;
    }
    if ((g_CondorInputPressed & INPUT_CROSS) != 0) {
        Mini_CondorSubmitSfxPlay(SFX_MENU_CURSOR, 4);
        g_CondorOutcomeIndex++;
        if (g_CondorOutcomeIndex >= 7) {
            g_CondorOutcomeIndex = 0;
        }
        Mini_CondorBuildDigitDisplayRecords();
    }
}

/*
 * Handles cursor input in unit record selection.
 */
// FUNCTION: FF7 0x005fcd6d
void __cdecl Mini_CondorHandleRecordSelectInput(void)
{
    if ((g_CondorInputPressed & INPUT_CIRCLE) != 0) {
        if (g_CondorSelectRowY == 0) {
            Mini_CondorSubmitSfxPlay(SFX_MENU_CURSOR, 4);
            g_CondorStage = CONDOR_STAGE_RESTART_RECORDS;
        } else {
            Mini_CondorCancelAction();
        }
    }
    if ((g_CondorInputPressed & INPUT_CROSS) != 0) {
        Mini_CondorCancelAction();
    }
    if ((g_CondorInputPressed & INPUT_UP) != 0) {
        Mini_CondorSubmitSfxPlay(SFX_MENU_CURSOR, 4);
        g_CondorSelectRowY -= 16;
        if (g_CondorSelectRowY < 0) {
            g_CondorSelectRowY = 16;
        }
    }
    if ((g_CondorInputPressed & INPUT_DOWN) != 0) {
        Mini_CondorSubmitSfxPlay(SFX_MENU_CURSOR, 4);
        g_CondorSelectRowY += 16;
        if (g_CondorSelectRowY > 16) {
            g_CondorSelectRowY = 0;
        }
    }
    g_CondorSpriteSlot6ScreenY = g_CondorCameraY + g_CondorSelectRowY + 76;
}

/*
 * Cancels pending action and resets state.
 */
// FUNCTION: FF7 0x005fce53
void __cdecl Mini_CondorCancelAction(void)
{
    Mini_CondorSubmitSfxPlay(SFX_MENU_CANCEL, 4);
    g_CondorMainState = CONDOR_STATE_IDLE;
    g_CondorSelectPanelPhase = CONDOR_PANEL_CLOSED;
    Mini_CondorRequestDisplayMode1();
    Mini_CondorSetPendingRequest(0xC);
}

/*
 * Clears motion state word.
 */
// FUNCTION: FF7 0x005fce86
void __cdecl Mini_CondorClearStateWord(void)
{
    g_CondorMainState = CONDOR_STATE_IDLE;
}

/*
 * Steps state machine and dispatches phase transition.
 */
// FUNCTION: FF7 0x005fce95
int __cdecl Mini_CondorStepAndDispatchRequest(void)
{
    int i;

    if (g_CondorRampLimit < g_CondorRampLimitTarget) {
        g_CondorRampLimit++;
    } else {
        g_CondorRampLimit = g_CondorRampLimitTarget;
    }
    for (i = 0; i < g_CondorRampLimit; i++) {
        Mini_CondorStepFractionalMotion();
        if (g_CondorMainState == CONDOR_STATE_IDLE) {
            g_CondorSelectedRecord = g_CondorActionRecordSlot;
            Mini_CondorSetUnitMenuLayout();
            Mini_CondorRefreshActiveRecordDisplay();
            if (g_CondorActiveActionRecord == 0) {
                g_CondorSelectedRecord = 0xFFFF;
                g_CondorMatchListState.cursorActive = 1;
                g_CondorSelectedRecord = Mini_CondorPickRecordNearPoint(&g_CondorCursorPos);
                Mini_CondorOpenSelectedRecordActionMenu();
            } else if (g_CondorActiveActionRecord == 1) {
                g_CondorMainState = CONDOR_STATE_UNIT_ATTACK;
                g_CondorRecordTable.records[g_CondorPairUnitIndex].inContact = CONDOR_ATTACK_STEP_BEGIN;
                g_CondorRecordMode = CONDOR_RECORD_MODE_CURSOR;
            } else {
                g_CondorMainState = CONDOR_STATE_IDLE;
                g_CondorActiveActionRecord = 0;
                g_CondorSelectPanelPhase = CONDOR_PANEL_CLOSED;
                g_CondorRecordMode = CONDOR_RECORD_MODE_ACTION_MENU;
            }
            return 0;
        }
    }
    return 0;
}

/*
 * Loops step processing until idle state reached.
 */
// FUNCTION: FF7 0x005fcfb4
int __cdecl Mini_CondorStepUntilIdle(void)
{
    int i;

    if ((*(unsigned short *)&g_CondorWobbleTimer) != 0) {
        g_CondorSpriteSlotAHeightX.screenX = 0xFFF8;
        g_CondorSpriteSlotAScreenY = 0xFFFFFFE0 - ((*(unsigned short *)&g_CondorWobbleTimer) / 4) % 3;
    } else {
        if (g_CondorRampLimit < g_CondorRampLimitTarget) {
            g_CondorRampLimit++;
        } else {
            g_CondorRampLimit = g_CondorRampLimitTarget;
        }
        for (i = 0; i < g_CondorRampLimit; i++) {
            if (*(unsigned char *)&g_CondorCommandCode == CONDOR_COMMAND_HOLD) {
                g_CondorMainState = CONDOR_STATE_IDLE;
            } else {
                Mini_CondorStepFractionalMotion();
            }
            if (g_CondorMainState == CONDOR_STATE_IDLE) {
                Mini_CondorRequestDisplayMode1();
                g_CondorSelectPanelPhase = CONDOR_PANEL_CLOSED;
                g_CondorSelectedRecord = 0xFFFF;
                g_CondorMatchListState.cursorActive = 1;
                g_CondorSelectedRecord = Mini_CondorPickRecordNearPoint(&g_CondorCursorPos);
                Mini_CondorRefreshActiveRecordDisplay();
                return 0;
            }
        }
    }
    return 0;
}

/*
 * Adjusts ramped movement speed.
 */
// FUNCTION: FF7 0x005fd0b3
void __cdecl Mini_CondorRampStepMotion(void)
{
    int i;

    if (g_CondorRampLimit < g_CondorRampLimitTarget) {
        g_CondorRampLimit++;
    } else {
        g_CondorRampLimit = g_CondorRampLimitTarget;
    }
    for (i = 0; i < g_CondorRampLimit; i++) {
        Mini_CondorStepFractionalMotion();
    }
}

/*
 * Runs phased step motion updates.
 */
// FUNCTION: FF7 0x005fd112
void __cdecl Mini_CondorRunPhasedMotion()
{

    g_CondorSelectPanelPhase = CONDOR_PANEL_OPEN;
    if (g_CondorPhaseFrames == 0) {
        if (g_CondorFadeAlpha < 0x80) {
            g_CondorFadeAlpha += g_CondorPhasedMotionStep;
            g_CondorPhasedMotionStep += 2;
            if (g_CondorFadeAlpha > 0x80) {
                g_CondorFadeAlpha = 0x80;
            }
        } else {
            g_CondorFadeAlpha = 0x80;
            Mini_CondorRequestDisplayMode1();
            g_CondorPhaseFrames++;
        }
    } else if (g_CondorPhaseFrames == 1) {
        if (g_CondorPhase == 1) {
            g_CondorPhaseFrames++;
        } else {
            g_CondorBannerCounter -= 2;
            if (g_CondorBannerCounter == -24) {
                g_CondorPhaseFrames++;
            }
        }
    } else {
        Mini_CondorMoveCursorAndScroll(INPUT_DOWN, &g_CondorCursorPos);
        Mini_CondorMoveCursorAndScroll(INPUT_DOWN, &g_CondorCursorPos);
        Mini_CondorMoveCursorAndScroll(INPUT_DOWN, &g_CondorCursorPos);
        if (g_CondorCameraY >= 328) {
            g_CondorMainState = CONDOR_STATE_IDLE;
            g_CondorSelectPanelPhase = CONDOR_PANEL_CLOSED;
        }
    }
}

/*
 * Initiates unit attack action.
 */
// FUNCTION: FF7 0x005fd245
void __cdecl Mini_CondorBeginUnitAttack(void)
{
    int contactSlot;
    short res;
    int unitIndex;

    g_CondorActiveActionRecord = 0;
    unitIndex = g_CondorPairUnitIndex;
    contactSlot = g_CondorRecordTable.records[g_CondorPairUnitIndex].contactSlot;
    if ((short)Mini_CondorIsPairableUnitKind(&g_CondorRecordTable.records[unitIndex]) != 0) {
        res = Mini_CondorAdvanceUnitAttack(unitIndex, contactSlot);
    } else {
        res = 1;
    }
    if (res == CONDOR_ATTACK_TARGET_DEFEATED) {
        Mini_CondorResetUnitPairAndSelect(unitIndex, contactSlot);
    }
    if (res == CONDOR_ATTACK_DONE) {
        g_CondorMainState = CONDOR_STATE_UNIT_COUNTERATTACK;
        g_CondorRecordTable.records[(g_CondorRecordTable.records[g_CondorPairUnitIndex].contactSlot)].inContact = CONDOR_ATTACK_STEP_BEGIN;
    }
}

/*
 * Ticks active unit attack state machine.
 */
// FUNCTION: FF7 0x005fd2f6
void __cdecl Mini_CondorTickUnitAttack(void)
{
    int contactSlot;
    int unitIdx;
    short res;

    unitIdx = g_CondorPairUnitIndex;
    contactSlot = g_CondorRecordTable.records[g_CondorPairUnitIndex].contactSlot;
    if ((short)Mini_CondorIsPairableUnitKind(&g_CondorRecordTable.records[g_CondorPairUnitIndex]) != 0) {
        res = Mini_CondorAdvanceUnitAttack(contactSlot, unitIdx);
        if (res == CONDOR_ATTACK_TARGET_DEFEATED) {
            Mini_CondorResetUnitPairAndSelect(contactSlot, unitIdx);
        }
        if (res == CONDOR_ATTACK_DONE) {
            Mini_CondorResetUnitPairAndSelect(contactSlot, unitIdx);
        }
    } else {
        Mini_CondorResetUnitPairAndSelect(contactSlot, unitIdx);
    }
}
} /* extern "C" */
