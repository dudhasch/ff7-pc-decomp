// src/ff7/condor/condor_unit_records.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: The Fort Condor unit records: the per-frame update loop, movement and turning, target
//   selection, peer linking, collision probes and drawing of the active units.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/types/condor.h"
#include "lib/codegen.h"
#include "ff7/api/condor.h"
#include "lib/api/graphics.h"
#include "lib/types/graphics.h"
#include "ff7/enums/condor.h"
#include <stdlib.h>
#include <string.h>
extern "C" {


#pragma intrinsic(strcpy)
#pragma intrinsic(strcat)
#pragma intrinsic(strlen)


#pragma intrinsic(memset)

struct CondorLinkRecord;

#pragma intrinsic(abs)


/*
 * Tests unit class type against valid category ranges.
 */
// FUNCTION: FF7 0x005ff2e0
short __cdecl Mini_CondorIsPairableUnitKind(struct CondorRecord *unit)
{
    unsigned short result;

    result = 0;
    switch (unit->kind) {
    case CONDOR_BEHAVIOR_FIGHTER:
    case CONDOR_BEHAVIOR_SUPPORT:
    case CONDOR_BEHAVIOR_CATAPULT:
    case CONDOR_BEHAVIOR_ONESHOT:
    case CONDOR_BEHAVIOR_AUTONOMOUS:
        result = 1;
    }
    return (short)result;
}

/*
 * Empty stub callback.
 */
// FUNCTION: FF7 0x005ff339
void __cdecl Mini_CondorNoOp_1(CondorRecord *record)
{
}

/*
 * Updates animation state for active battle units.
 */
// FUNCTION: FF7 0x005ff33e
void __cdecl Mini_CondorUpdateUnitAnims(void)
{
    CondorRecord *unit;
    int recordIndex;

    unit = &g_CondorRecordTable.records[0];
    for (recordIndex = 0; recordIndex < 40; recordIndex++) {
        if (unit->status != 0) {
            Mini_CondorRampUnitTurnRate(unit);
        }
        unit++;
    }
}

/*
 * Main per-frame update loop for all Fort Condor units.
 */
// FUNCTION: FF7 0x005ff38a
void __cdecl Mini_CondorUpdateUnits(void)
{
    int highest;
    CondorRecord *rec;
    short was;
    short temp;
    int i;
    int lowest;

    lowest = 0x400;
    highest = 0x1E0;
    rec = &g_CondorRecordTable.records[0];
    for (i = 0; i < 40; i++) {
        if (rec->status != 0) {
            if (rec->retired == 0 && (rec->status & 0x10) != 0) {
                Mini_CondorTickReloadTimer(rec);
            }
            if (rec->retired != 0) {
                if (g_CondorUnitDeathActive == 0) {
                    if (rec->retired == 0xFF) {
                        rec->retired = 0x50;
                        Mini_CondorPlaySfxPanned(0x16, 3, rec->posX);
                        Mini_CondorRecordCodeHookNoOp(rec, 0);
                        g_CondorUnitDeathActive = 1;
                        g_CondorDyingUnitIndex = (short)i;
                    }
                } else if (g_CondorDyingUnitIndex == i) {
                    if (rec->retired != 0xFF) {
                        rec->retired -= 2;
                        if (rec->retired == 0) {
                            rec->retired = 0;
                            rec->status = 0;
                            g_CondorUnitDeathActive = 0;
                            g_CondorDyingUnitIndex = -1;
                            if (rec->unitIndex == 16 && g_CondorBattleOutcome == 0) {
                                g_CondorBattleOutcome = 1;
                            }
                        } else {
                            Mini_CondorNoOp_1(rec);
                        }
                    } else {
                        rec->retired = 0x3C;
                    }
                }
            } else {
                if (rec->unitIndex >= 16) {
                    if (lowest >= rec->posY) {
                        lowest = rec->posY;
                    }
                } else {
                    temp = rec->posY + (rec->offsetY << 4);
                    if (highest <= temp) {
                        highest = temp;
                    }
                }
                if ((rec->status & 4) == 0) {
                    if (g_CondorRecordMode <= 1 && g_CondorSelectPanelPhase == 0) {
                        rec->actionCountdown -= g_CondorTickStep;
                        if (rec->actionCountdown == 0 || rec->actionCountdown >= 65280) {
                            rec->actionCountdown = rec->timerReload;
                            Mini_CondorDispatchUnitFlags(rec, i);
                        }
                    }
                } else {
                    if (g_CondorRecordMode <= 1 && g_CondorActiveActionRecord == 0 && Mini_CondorIsPairableUnitKind(rec) != 0) {
                        Mini_CondorUpdateUnitContactState(rec, i);
                    }
                }
                Mini_CondorRampUnitTurnRate(rec);
            }
        }
        rec++;
    }
    was = *(short *)&g_CondorAlertLevel;
    if (lowest <= 448) {
        *(int *)&g_CondorAlertLevel = 0x60;
    } else {
        *(int *)&g_CondorAlertLevel = 0x60 - (lowest - 0x1C0) / 6;
    }
    if (*(int *)&g_CondorAlertLevel < 88 && was >= 88) {
        Mini_CondorSubmitSfxPlay(0, 1);
    }
    *(int *)&g_CondorHighestUnitY = highest;
    if (*(int *)&g_CondorHighestUnitY >= 928) {
        *(int *)&g_CondorHighestUnitY = 928;
    }
}

/*
 * Ticks reload or delay timer for active unit.
 */
// FUNCTION: FF7 0x005ff690
void __cdecl Mini_CondorTickReloadTimer(CondorRecord *unit)
{
    unsigned short status;

    if (unit->cooldown != 0) {
        (unit->cooldown)--;
        if (unit->cooldown == 0) {
            unit->status = unit->status & 0xEF;
            if (unit->sequenceIndex != 2) {
                unit->sequenceIndex = 1;
                status = Mini_CondorBeginRecordSequence(unit);
            }
        }
    }
}

/*
 * Dispatches unit state update flags.
 */
// FUNCTION: FF7 0x005ff6fc
void __cdecl Mini_CondorDispatchUnitFlags(CondorRecord *unit, int context)
{
    short flags;

    flags = *(short *)&unit->status;
    if ((flags & 0x40) != 0) {
        Mini_CondorAddUnitCounterWithEffect(unit);
    }
    if ((flags & 0x50) == 0) {
        Mini_CondorDispatchUnitKindHandler(unit, context);
    }
}

/*
 * Updates unit counter and triggers corresponding effects.
 */
// FUNCTION: FF7 0x005ff740
void __cdecl Mini_CondorAddUnitCounterWithEffect(CondorRecord *unit)
{
    short slot;
    short *source;
    short newCounter;
    short increment;

    unit->status = unit->status & 0xBF;
    increment = *(char *)(g_pCondorDataBin + 0xE);
    source = (short *)(g_pCondorDataBin + 0x1C);
    increment = *source;
    slot = Mini_CondorFindInactiveCounterEffectSlot();
    if (slot != 0xFF) {
        g_CondorCounterEffects[slot].counter = 0x8001;
        g_CondorCounterEffects[slot].displayNumber = increment;
        g_CondorCounterEffects[slot].posX = unit->posX;
        g_CondorCounterEffects[slot].posY = unit->posY;
        Mini_CondorSetRecordTwoDigits(slot, increment);
    }
    Mini_CondorSubmitSfxPlay(1, 4);
    newCounter = unit->counter;
    newCounter += increment;
    if (newCounter >= unit->counterMax) {
        newCounter = unit->counterMax;
        unit->status = unit->status & 0xBF;
    }
    unit->counter = newCounter;
    if (g_CondorSelectedRecord != -1) {
        Mini_CondorRefreshActiveRecordDisplay();
    }
}

/*
 * Checks and updates unit combat contact state.
 */
// FUNCTION: FF7 0x005ff863
void __cdecl Mini_CondorUpdateUnitContactState(CondorRecord *unit, int index)
{
    int attackStatus;
    int targetIndex;
    short status;
    short hitCount;

    status = Mini_CondorTestRecordOverlap(unit, unit->contactSlot);
    if (status != -1) {
        if (unit->inContact == 0) {
            if (g_CondorRecordTable.records[g_CondorRecordTable.records[index].contactSlot].inContact == 0) {
                unit->inContact = 1;
            }
        } else if (g_CondorMainState == 0) {
            targetIndex = index;
            attackStatus = g_CondorRecordTable.records[index].contactSlot;
            status = Mini_CondorAdvanceUnitAttack(targetIndex, attackStatus);
            if (status == 1) {
                unit->inContact = 0;
                unit->bufferIndex = 120;
            }
            if (status == 2) {
                hitCount++;
            }
        }
    } else {
        unit->status = 1;
        unit->contactSlot = 0xFF;
        unit->inContact = 0;
        unit->bufferIndex = 0;
        unit->sequenceFlag = 0;
        if (unit->unitIndex < 16) {
            unit->angle = 0;
        } else {
            if ((unit->candidate & 0xFF) != 0) {
                (unit->candidate)--;
            }
            Mini_CondorAdvanceRecordPathSegment(unit);
        }
    }
}

/*
 * Ramps unit rotational turning speed towards target heading.
 */
// FUNCTION: FF7 0x005ff995
void __cdecl Mini_CondorRampUnitTurnRate(CondorRecord *unit)
{
    unsigned short sequenceWord;
    unsigned short turnRate;

    turnRate = unit->turnRate & 0x1FF;
    if (unit->sequenceCommand != 0xFF) {
        if (unit->sequenceCommand == 0) {
            sequenceWord = Mini_CondorReadRecordSequenceWord(unit);
            if ((sequenceWord & 0x80) != 0) {
                if ((unit->turnRate & 0x200) == 0) {
                    turnRate += 0x10;
                    unit->turnRate = turnRate;
                    if (turnRate >= 511) {
                        unit->turnRate = 0x3FF;
                    }
                } else {
                    turnRate -= 0x10;
                    unit->turnRate = turnRate | 0x200;
                    if (turnRate >= 65280) {
                        unit->turnRate = 0;
                    }
                }
            }
        } else {
            unit->sequenceCommand--;
        }
    }
    if (unit->unitIndex < 16) {
        unit->drawAngle = unit->angle;
    } else {
        unit->drawAngle = unit->angle;
    }
}

/*
 * Dispatches unit behavior based on unit classification.
 */
// FUNCTION: FF7 0x005ffab0
void __cdecl Mini_CondorDispatchUnitKindHandler(CondorRecord *unit, int context)
{
    g_CondorSavedSubPixelX = unit->subPixelX;
    g_CondorSavedSubPixelY = unit->subPixelY;
    g_CondorUnitPosition = unit->posX;
    (*(short *)((char *)&g_CondorUnitPosition + 2)) = unit->posY;
    g_CondorUnitPositionZ = unit->posZ;
    g_CondorScratchPair = unit->worldX;
    (*(short *)((char *)&g_CondorScratchPair + 2)) = unit->worldY;
    g_CondorUnitWorldZ = unit->worldZ;
    if (unit->contactSlot == 0xFF) {
        switch (unit->kind) {
        case CONDOR_BEHAVIOR_STATIONARY:
            break;
        case CONDOR_BEHAVIOR_FIGHTER:
            Mini_CondorStepUnitKind1(unit);
            break;
        case CONDOR_BEHAVIOR_SUPPORT:
            Mini_CondorStepUnlinkedUnit(unit);
            break;
        case CONDOR_BEHAVIOR_TURRET_A:
        case CONDOR_BEHAVIOR_TURRET_B:
            Mini_CondorCycleRecordStates(unit);
            break;
        case CONDOR_BEHAVIOR_CATAPULT:
            Mini_CondorUpdateUnitRecord(unit);
            break;
        case CONDOR_BEHAVIOR_ONESHOT:
            Mini_CondorStepUnitKind6(unit);
            break;
        case CONDOR_BEHAVIOR_AUTONOMOUS:
            Mini_CondorUpdateUnitTargetState(unit);
        }
    } else {
        unit->flags = unit->flags & 0xFD;
    }
}

/*
 * Integrates unit velocity vector into position coordinates.
 */
// FUNCTION: FF7 0x005ffbd6
void __cdecl Mini_CondorIntegrateUnitVelocity(CondorRecord *unit)
{
    unsigned short accum;

    accum = unit->subPixelX & 0xFF;
    accum += unit->velX;
    unit->subPixelX = accum;
    accum = accum >> 8;
    if (accum != 0) {
        if (accum >= 128) {
            accum |= 0xFF00;
        }
        unit->posX += accum;
        unit->worldX += accum;
    }
    accum = unit->subPixelY & 0xFF;
    accum += unit->velY;
    unit->subPixelY = accum;
    accum = accum >> 8;
    if (accum != 0) {
        if (accum >= 128) {
            accum |= 0xFF00;
        }
        unit->posY += accum;
        unit->worldY += accum;
    }
}

/*
 * Pairs unit with peer unit for cooperative behavior.
 */
// FUNCTION: FF7 0x005ffce9
void __cdecl Mini_CondorPairUnitWithPeer(CondorRecord *unit, int recordIndex)
{
    CondorRecord *peer;
    short peerIndex;
    short linked;

    peerIndex = 0xFFFF;
    if (unit->unitIndex < 16) {
        if ((short)Mini_CondorIsPairableUnitKind(unit) != 0) {
            peerIndex = Mini_CondorFindNearestOtherRecordInRange(unit, 20, 40);
        }
    } else {
        peerIndex = Mini_CondorFindNearestOtherRecordInRange(unit, 0, 20);
    }
    if (peerIndex != -1) {
        peer = &g_CondorRecordTable.records[peerIndex];
        linked = (unit->status | peer->status) & 4;
        if ((short)Mini_CondorIsPairableUnitKind(unit) != 0) {
            unit->status = unit->status | 4;
            Mini_CondorBeginLinearMotion(unit->posX, unit->posY, peer->posX, peer->posY);
            unit->velX = (short)g_CondorVecX;
            unit->velY = (short)g_CondorVecY;
            unit->angle = Mini_CondorVectorToAngle((CondorCursor *)&unit->velX);
            if (unit->contactSlot == 0xFF) {
                unit->contactSlot = peerIndex;
            }
        }
        if ((short)Mini_CondorIsPairableUnitKind(peer) != 0) {
            peer->status = peer->status | 4;
            Mini_CondorBeginLinearMotion(peer->posX, peer->posY, unit->posX, unit->posY);
            peer->velX = (short)g_CondorVecX;
            peer->velY = (short)g_CondorVecY;
            peer->angle = Mini_CondorVectorToAngle((CondorCursor *)&peer->velX);
            if ((peer->contactSlot) == 0xFF) {
                peer->contactSlot = recordIndex;
            }
        }
        if (unit->unitIndex < 16) {
            g_CondorCursor.x = unit->posX;
            g_CondorCursor.y = unit->posY;
            g_CondorPairUnitIndex = recordIndex;
            peerIndex = (unit - g_CondorRecordTable.records);
        } else {
            g_CondorCursor.x = peer->posX;
            g_CondorCursor.y = peer->posY;
            g_CondorPairUnitIndex = peerIndex;
            peerIndex = (peer - g_CondorRecordTable.records);
        }
        if (linked == 0) {
            Mini_CondorStartRecordAction(0, (short)peerIndex);
        }
    }
}

/*
 * Updates target acquisition and tracking state for unit.
 */
// FUNCTION: FF7 0x005fff45
short __cdecl Mini_CondorUpdateUnitTargetState(CondorRecord *unit)
{
    short cnt;
    short slot;
    short unused;
    short res;

    if (unit->unitIndex == 19 && Mini_CondorTickRecordLinkWait(unit) == 1) {
        return 0;
    }
    cnt = 0;
    switch (unit->state) {
    case CONDOR_UNIT_STATE_IDLE:
        unit->state = 1;
        unit->flags = 1;
        if (unit->sequenceIndex != 0) {
            unit->sequenceIndex = 0;
            unit->flags = 0;
            cnt++;
        }
        break;
    case CONDOR_UNIT_STATE_MOVING:
        if (unit->timer == 0) {
            slot = Mini_CondorResolveTargetByMode(unit);
        } else {
            unit->timer -= 1;
            slot = -1;
        }
        if (slot == -1) {
            if (unit->sequenceIndex != 1) {
                unit->sequenceIndex = 1;
                unit->flags = unit->flags | 1;
                cnt++;
            }
        } else {
            unit->targetSlot = (unsigned char)slot;
            unit->flags = unit->flags | 2;
            unit->state = 3;
            g_CondorCursor.x = g_CondorRecordTable.records[slot].posX;
            g_CondorCursor.y = g_CondorRecordTable.records[slot].posY;
            Mini_CondorBeginTargetMove(unit);
        }
        break;
    case CONDOR_UNIT_STATE_ENGAGED:
        unit->flags = 2;
        break;
    case CONDOR_UNIT_STATE_ATTACK_MOVE:
        if ((unit->status & 0x20) != 0) {
            if (unit->targetSlot != 0xFF) {
                unit->targetSlot = 0xFF;
                unit->timer = unit->timerReload;
                unit->state = 1;
                Mini_CondorStepRecordAlongPath(unit);
            }
        } else {
            res = Mini_CondorResolveRecordTarget(unit);
            if (res == -2) {
                Mini_CondorStepRecordAlongPath(unit);
            }
        }
        break;
    }
    if (cnt != 0) {
        unused = Mini_CondorBeginRecordSequence(unit);
    }
    Mini_CondorFinishUnitUpdate(unit);
    if (unit->targetIndex >= 0xCC && unit->targetIndex <= 0xCF && g_CondorBattleOutcome == 0) {
        g_CondorBattleOutcome = 2;
    }
    if (unit->worldX == 48 && unit->worldY == -96 && g_CondorBattleOutcome == 0) {
        g_CondorBattleOutcome = 2;
    }
    return 0;
}

/*
 * Finalizes per-frame state update for unit.
 */
// FUNCTION: FF7 0x006001b2
void __cdecl Mini_CondorFinishUnitUpdate(CondorRecord *unit)
{
    int unused; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int recordIndex;

    recordIndex = (unit - g_CondorRecordTable.records);
    if ((unit->targetX | unit->targetY) != 0) {
        Mini_CondorStepMotionIfPending(unit);
    }
    if ((short)Mini_CondorIsPairableUnitKind(unit) != 0) {
        Mini_CondorPairUnitWithPeer(unit, recordIndex);
    }
}

/*
 * Executes pending motion step if unit has remaining path distance.
 */
// FUNCTION: FF7 0x00600216
void __cdecl Mini_CondorStepMotionIfPending(CondorRecord *unit)
{
    if ((unit->velX | unit->velY) != 0) {
        Mini_CondorIntegrateUnitVelocity(unit);
        Mini_CondorSettleRecordAtTarget(unit);
    }
}

/*
 * Settles unit record at target destination point.
 */
// FUNCTION: FF7 0x00600247
void __cdecl Mini_CondorSettleRecordAtTarget(CondorRecord *unit)
{
    short regionIndex;
    int unused; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */

    regionIndex = Mini_CondorTestRegionIndexAccepted(unit, (short)unit->targetIndex);
    if ((unit->status & 0x20) != 0) {
        if (regionIndex != unit->targetIndex) {
            regionIndex = Mini_CondorFindAcceptedRegionIndex(unit);
            if (regionIndex != -1) {
                unit->targetIndex = regionIndex;
                Mini_CondorLoadRecordVertexTarget(unit);
            } else {
                Mini_CondorRetargetRecord(unit);
            }
        } else {
            Mini_CondorRefreshRecordMotion(unit);
            Mini_CondorIntegrateUnitVelocity(unit);
            regionIndex = Mini_CondorFindAcceptedRegionIndex(unit);
            if (regionIndex != -1) {
                unit->targetIndex = regionIndex;
                Mini_CondorLoadRecordVertexTarget(unit);
                unit->status = unit->status & 0xDF;
            } else {
                Mini_CondorRetargetRecord(unit);
            }
        }
    } else if (regionIndex != unit->targetIndex) {
        regionIndex = Mini_CondorFindAcceptedRegionIndex(unit);
        if (regionIndex != -1) {
            unit->targetIndex = regionIndex;
            Mini_CondorLoadRecordVertexTarget(unit);
        } else {
            Mini_CondorRetargetRecord(unit);
        }
    }
    if (unit->targetX == unit->posX) {
        unit->velX = 0;
    }
    if (unit->targetY == unit->posY) {
        unit->velY = 0;
    }
    if ((unit->velX | unit->velY) == 0) {
        unit->targetY = 0;
        unit->targetX = 0;
        if (unit->unitIndex >= 16) {
            Mini_CondorAdvanceRecordPathSegment(unit);
        } else {
            unit->state = 0;
            unit->flags = unit->flags & 0xFE;
            unit->velX = 0;
            unit->velY = 0;
            unit->moveStep = 0;
            if (g_CondorSelectedRecord == -1) {
                g_CondorSelectedRecord = (unit - g_CondorRecordTable.records);
                Mini_CondorRefreshActiveRecordDisplay();
                g_CondorSelectedRecord = 0xFFFF;
            }
            Mini_CondorStartRecordAction(3, (unit - g_CondorRecordTable.records));
        }
    }
}

/*
 * Refreshes directional motion parameters for unit record.
 */
// FUNCTION: FF7 0x0060048c
void __cdecl Mini_CondorRefreshRecordMotion(CondorRecord *unit)
{
    if (unit->unitIndex >= 16) {
        if ((unit->candidate & 0xFF) != 0) {
            (unit->candidate)--;
        }
        Mini_CondorAdvanceRecordPathSegment(unit);
    } else {
        unit->velX = 0;
        unit->velY = 0;
        unit->moveStep = 0;
        if ((unit->targetX | unit->targetY) != 0) {
            Mini_CondorBeginLinearMotion(unit->posX, unit->posY, unit->targetX, unit->targetY);
            unit->velX = (short)g_CondorVecX;
            unit->velY = (short)g_CondorVecY;
        }
        unit->angle = Mini_CondorVectorToAngle((CondorCursor *)&unit->velX);
    }
    unit->status = unit->status & 0xF7;
}

/*
 * Re-evaluates and selects new target destination for unit record.
 */
// FUNCTION: FF7 0x00600577
void __cdecl Mini_CondorRetargetRecord(CondorRecord *unit)
{
    short result;

    Mini_CondorIntegrateUnitVelocity(unit);
    Mini_CondorIntegrateUnitVelocity(unit);
    result = Mini_CondorFindAcceptedRegionIndex(unit);
    if (result != -1) {
        unit->targetIndex = result;
        Mini_CondorLoadRecordVertexTarget(unit);
        Mini_CondorCopyGlobalsIntoRecord(unit);
        Mini_CondorIntegrateUnitVelocity(unit);
    } else {
        unit->status = unit->status | 0x20;
        Mini_CondorCopyGlobalsIntoRecord(unit);
        result = Mini_CondorSteerRecordAlongRegionEdge(unit);
        if (result == 0) {
            result = Mini_CondorTryAngleOffsetCandidates(unit);
        }
        if (result == 0) {
            Mini_CondorCopyGlobalsIntoRecord(unit);
        } else {
            Mini_CondorIntegrateUnitVelocity(unit);
        }
    }
}

/*
 * Copies global combat parameters into unit record.
 */
// FUNCTION: FF7 0x00600648
void __cdecl Mini_CondorCopyGlobalsIntoRecord(CondorRecord *unit)
{
    unit->subPixelX = g_CondorSavedSubPixelX;
    unit->subPixelY = g_CondorSavedSubPixelY;
    unit->posX = g_CondorUnitPosition;
    unit->posY = (*(short *)((char *)&g_CondorUnitPosition + 2));
    unit->posZ = g_CondorUnitPositionZ;
    unit->worldX = g_CondorScratchPair;
    unit->worldY = (*(short *)((char *)&g_CondorScratchPair + 2));
    unit->worldZ = g_CondorUnitWorldZ;
}

/*
 * Evaluates candidate heading angles to bypass obstacles.
 */
// FUNCTION: FF7 0x006006b6
short __cdecl Mini_CondorTryAngleOffsetCandidates(CondorRecord *unit)
{
    short result;

    result = Mini_CondorTryHeadingOffset(unit, -0x80);
    if (result != -1) {
        return (short)result;
    } else {
        result = Mini_CondorTryHeadingOffset(unit, 0x80);
        if (result != -1) {
            return (short)result;
        } else {
            result = Mini_CondorTryHeadingOffset(unit, 0xFFFFFBE0);
            if (result != -1) {
                return (short)result;
            } else {
                result = Mini_CondorTryHeadingOffset(unit, 0x420);
                if (result != -1) {
                    return (short)result;
                } else {
                    result = Mini_CondorTryHeadingOffset(unit, 0xFFFFF8E0);
                    if (result != -1) {
                        return (short)result;
                    } else {
                        result = Mini_CondorTryHeadingOffset(unit, 0x720);
                        if (result != -1) {
                            return (short)result;
                        } else {
                            return 0;
                        }
                    }
                }
            }
        }
    }
}

/*
 * Tests validity of specific heading angle offset.
 */
// FUNCTION: FF7 0x0060079c
short __cdecl Mini_CondorTryHeadingOffset(CondorRecord *unit, short angleOffset)
{
    Mini_CondorCopyGlobalsIntoRecord(unit);
    Mini_CondorAngleToStepComponents(unit->angle + angleOffset & 0xFFF);
    unit->velX = (short)g_CondorVecX;
    unit->velY = (short)g_CondorVecY;
    Mini_CondorIntegrateUnitVelocity(unit);
    Mini_CondorIntegrateUnitVelocity(unit);
    return Mini_CondorFindAcceptedRegionIndex(unit);
}

/*
 * Resolves primary combat target for unit record.
 */
// FUNCTION: FF7 0x0060080b
short __cdecl Mini_CondorResolveRecordTarget(CondorRecord *unit)
{
    short slot;

    slot = unit->targetSlot;
    if (slot != -1 && (unit->state == 3 || unit->state == 5)) {
        if (g_CondorRecordTable.records[slot].status != 0) {
            g_CondorCursor.x = g_CondorRecordTable.records[slot].posX;
            g_CondorCursor.y = g_CondorRecordTable.records[slot].posY;
            Mini_CondorBeginTargetMove(unit);
            return (short)slot;
        }
        unit->status = 1;
        unit->state = 0;
        unit->flags = 0;
        unit->targetX = 0;
        unit->targetY = 0;
        unit->inContact = 0;
        unit->bufferIndex = 0;
        unit->cycleCountdown = 0;
        unit->contactSlot = 0xFF;
        unit->targetSlot = 0xFF;
        return 0xFFFE;
    } else {
        return -1;
    }
}

/*
 * Resolves target based on unit tactical behavior mode.
 */
// FUNCTION: FF7 0x006008f0
short __cdecl Mini_CondorResolveTargetByMode(CondorRecord *unit)
{
    short slot;
    short candidate;

    switch (unit->targetMode) {
    case CONDOR_TARGET_MODE_PROBE:
        slot = Mini_CondorTickCollisionProbe(unit);
        break;
    case CONDOR_TARGET_MODE_PRIORITY:
        slot = Mini_CondorTickCollisionProbe(unit);
        if (slot != -1) {
            candidate = Mini_CondorCollectDeltaRecordsAndPickMax(g_CondorMatchListState.entries, g_CondorMatchCount);
            if (candidate != -1) {
                slot = candidate;
            }
        }
        break;
    case CONDOR_TARGET_MODE_PREFER_KIND:
        slot = Mini_CondorTickCollisionProbe(unit);
        if (slot != -1) {
            candidate = Mini_CondorCollectRecordsMatchingType(unit, g_CondorMatchListState.entries, g_CondorMatchCount);
            if (candidate != -1) {
                candidate = Mini_CondorCollectDeltaRecordsAndPickMax(g_CondorCollectedRecords, g_CondorCollectedRecordCount);
                if (candidate != -1) {
                    slot = candidate;
                }
            } else {
                candidate = Mini_CondorCollectDeltaRecordsAndPickMax(g_CondorMatchListState.entries, g_CondorMatchCount);
                if (candidate != -1) {
                    slot = candidate;
                }
            }
        }
        break;
    case CONDOR_TARGET_MODE_PAIR:
        if (g_CondorSpawnCount == 1) {
            unit->targetMode = CONDOR_TARGET_MODE_NONE;
            unit->flags = 1;
            unit->state = 1;
            candidate = unit->candidate >> 8 & 0xF;
            unit->targetSlot = 0xFF;
            unit->contactSlot = 0xFF;
            if (candidate == 6) {
                candidate = 0x202;
            } else {
                candidate = 0xA02;
            }
            unit->candidate = candidate;
            Mini_CondorAdvanceRecordPathSegment(unit);
            slot = 0xFFFF;
        } else {
            Mini_CondorPairUnitWithPeer(unit, (unit - g_CondorRecordTable.records));
            slot = Mini_CondorTickCollisionProbe(unit);
        }
        break;
    default:
        slot = 0xFFFF;
    }
    return (short)slot;
}

/*
 * Collects active records matching target unit type.
 */
// FUNCTION: FF7 0x00600ad4
short __cdecl Mini_CondorCollectRecordsMatchingType(CondorRecord *unit, CondorLinkRecord *records, short count)
{
    struct {
        short kind;
        short pad0;
        short index;
        short pad1;
        short max;
        short pad2;
        CondorLinkRecord *list;
        short result;
        short pad3;
        CondorRecord *item;
    } scan;
    if (count == 0)
        return -1;
    g_CondorCollectedRecordCount = 0;
    scan.result = -1;
    scan.max = 0;
    scan.list = records;
    switch (unit->unitIndex) {
    case CONDOR_UNIT_BOSS_WYVERN:
        scan.kind = 3;
        break;
    case CONDOR_UNIT_ENEMY_BEAST:
        scan.kind = 4;
        break;
    case CONDOR_UNIT_ENEMY_SHOOTER:
        scan.kind = 2;
        break;
    default:
        scan.kind = 0;
        break;
    }
    for (scan.index = 0; scan.index < count; scan.index++) {
        scan.item = scan.list->peer;
        if (scan.item->unitIndex == scan.kind) {
            g_CondorCollectedRecords[g_CondorCollectedRecordCount].peer = scan.item;
            g_CondorCollectedRecords[g_CondorCollectedRecordCount].kind = scan.kind;
            g_CondorCollectedRecordCount++;
            scan.result = g_CondorCollectedRecordCount;
        }
        scan.list++;
    }
    return scan.result;
}

/*
 * Collects candidate records and selects highest-priority target.
 */
// FUNCTION: FF7 0x00600bd1
short __cdecl Mini_CondorCollectDeltaRecordsAndPickMax(CondorLinkRecord *records, short count)
{
    struct {
        short difference;
        short pad0;
        short index;
        short pad1;
        short max;
        short pad2;
        CondorLinkRecord *list;
        short result;
        short pad3;
        CondorRecord *item;
    } scan;
    if (count == 0)
        return -1;
    g_CondorDeltaRecordCount = 0;
    scan.result = -1;
    scan.max = 0;
    scan.list = records;
    for (scan.index = 0; scan.index < count; scan.index++) {
        scan.item = scan.list->peer;
        scan.difference = scan.item->counterMax - scan.item->counter;
        if (scan.difference != 0) {
            g_CondorDeltaRecords[g_CondorDeltaRecordCount].item = scan.item;
            g_CondorDeltaRecords[g_CondorDeltaRecordCount].listValue = scan.list->recordIndex;
            g_CondorDeltaRecordCount++;
            if (scan.max <= scan.difference) {
                scan.max = scan.difference;
                scan.result = scan.list->recordIndex;
            }
        }
        scan.list++;
    }
    return scan.result;
}

/*
 * Ticks terrain and unit collision detection probes.
 */
// FUNCTION: FF7 0x00600cbb
short __cdecl Mini_CondorTickCollisionProbe(CondorRecord *unit)
{
    CondorLinkSlot *probe;
    char probeBuf[120];
    short found;
    if (unit->cycleCountdown == 0) {
        unit->cycleCountdown = (((unit - g_CondorRecordTable.records)) + 20) & 7;
        if (unit->contactSlot == 255) {
            probe = (CondorLinkSlot *)probeBuf;
            probe->posX = unit->posX;
            probe->posY = unit->posY - (unit->offsetY << 2);
            probe->posZ = unit->posZ;
            probe->offsetY = unit->offsetY << 4;
            probe->prevOffsetY = probe->offsetY;
            found = Mini_CondorFindNearestRecordInRange(probe, 0, 20);
            if (found != -1)
                return found;
        }
    } else {
        (unit->cycleCountdown)--;
    }
    return -1;
}

/*
 * Steps record position along current movement vector.
 */
// FUNCTION: FF7 0x00600d9b
void __cdecl Mini_CondorStepRecordAlongPath(CondorRecord *unit)
{
    if ((unit->candidate & 0xFF) != 0) {
        (unit->candidate)--;
    }
    Mini_CondorAdvanceRecordPathSegment(unit);
    if (unit->targetY >= unit->posY) {
        Mini_CondorAdvanceRecordPathSegment(unit);
    }
}

/*
 * Cycles through unit record state progression.
 */
// FUNCTION: FF7 0x00600df1
void __cdecl Mini_CondorCycleRecordStates(CondorRecord *unit)
{
    short result;
    unsigned short changed;
    short spawned;

    changed = 0;
    switch ((*(char *)&unit->state)) {
    case CONDOR_UNIT_STATE_IDLE:
        if (unit->cycleCountdown != 0) {
            (unit->cycleCountdown)--;
        } else {
            unit->state = 2;
            unit->sequenceIndex = 2;
            unit->flags = unit->flags | 2;
            changed++;
        }
        break;
    case CONDOR_UNIT_STATE_ENGAGED:
        if (unit->sequenceCommand == 0xFF) {
            if (unit->bufferIndex == 0) {
                spawned = Mini_CondorSpawnMovingRecord(unit, 0x18);
                if (spawned != -1) {
                    unit->bufferIndex = spawned;
                }
            } else if (g_CondorRecordTable.records[unit->bufferIndex].status == 0) {
                unit->state = 4;
                unit->sequenceIndex = 5;
                unit->bufferIndex = 0;
                unit->cycleCountdown = unit->cycleReload;
                unit->flags = unit->flags & 0xFD;
                changed++;
            }
        }
        break;
    case CONDOR_UNIT_STATE_WAITING_PEER:
        if (unit->cycleCountdown != 0) {
            (unit->cycleCountdown)--;
        } else {
            unit->cycleCountdown = unit->cyclePeriod;
            unit->state = 0;
            unit->sequenceIndex = 0;
            unit->bufferIndex = 0;
            changed++;
        }
    }
    if (changed != 0) {
        result = Mini_CondorBeginRecordSequence(unit);
    }
}

/*
 * Applies single step transition to unit record state.
 */
// FUNCTION: FF7 0x00600f88
void __cdecl Mini_CondorStepUnitKind1(CondorRecord *unit)
{
    Mini_CondorApplyRequestedRecordState(unit);
    Mini_CondorFinishUnitUpdate(unit);
}

/*
 * Applies pending requested state transition to unit record.
 */
// FUNCTION: FF7 0x00600fa5
void __cdecl Mini_CondorApplyRequestedRecordState(CondorRecord *unit)
{
    unsigned short result;
    short changed;
    unsigned short resolved;

    changed = 0;
    switch (unit->state) {
    case CONDOR_UNIT_STATE_IDLE:
        if (unit->sequenceIndex != 0) {
            unit->sequenceIndex = 0;
            unit->flags = 0;
            changed++;
            unit->contactSlot = 0xFF;
            unit->velY = 0;
            unit->velX = 0;
            unit->targetY = 0;
            unit->targetX = 0;
        }
        break;
    case CONDOR_UNIT_STATE_MOVING:
        if (unit->sequenceIndex != 1) {
            unit->sequenceIndex = 1;
            unit->flags = unit->flags | 1;
            changed++;
        }
        break;
    case CONDOR_UNIT_STATE_ATTACK_MOVE:
        if (unit->sequenceIndex != 1) {
            unit->sequenceIndex = 1;
            unit->flags = unit->flags | 1;
            changed++;
        }
        resolved = Mini_CondorResolveRecordTarget(unit);
        break;
    case CONDOR_UNIT_STATE_WAITING_PEER:
        if (unit->sequenceIndex != 6) {
            unit->sequenceIndex = 6;
            unit->flags = unit->flags | 4;
            changed++;
        }
        break;
    case CONDOR_UNIT_STATE_SUPPORT_MOVE:
        if (unit->sequenceIndex != 1) {
            unit->sequenceIndex = 1;
            unit->flags = unit->flags | 1;
            changed++;
        }
        resolved = Mini_CondorResolveRecordTarget(unit);
    }
    if (changed != 0) {
        result = Mini_CondorBeginRecordSequence(unit);
    }
}

/*
 * Ticks wait timer for linked peer coordination.
 */
// FUNCTION: FF7 0x00601145
short __cdecl Mini_CondorTickRecordLinkWait(CondorRecord *unit)
{
    short waiting;

    waiting = 0;
    if ((unit->flags & 0x80) != 0) {
        if (unit->cycleCountdown != 0) {
            (unit->cycleCountdown)--;
        } else {
            unit->flags = unit->flags & 0x7F;
            Mini_CondorRefreshRecordMotion(unit);
        }
        waiting++;
    } else if (unit->targetSlot != 0xFF) {
        if (unit->cycleCountdown != 0) {
            (unit->cycleCountdown)--;
        } else if ((short)Mini_CondorTryLinkRecordToPeer(unit) == 0) {
            unit->targetX = g_CondorRecordTable.records[unit->targetSlot].posX;
            unit->targetY = g_CondorRecordTable.records[unit->targetSlot].posY;
            Mini_CondorRefreshRecordMotion(unit);
        } else {
            waiting++;
        }
    } else if (unit->cycleCountdown != 0) {
        (unit->cycleCountdown)--;
    } else if ((unit - g_CondorRecordTable.records) < 20) {
        if (unit->state == 0 && (short)Mini_CondorTryLinkFirstPeer(unit) == 1) {
            waiting++;
        }
    } else if ((short)Mini_CondorTryLinkFirstPeer(unit) == 1) {
        waiting++;
    }
    return (short)waiting;
}

/*
 * Attempts to link unit record to candidate peer.
 */
// FUNCTION: FF7 0x006012d5
short __cdecl Mini_CondorTryLinkRecordToPeer(CondorRecord *record)
{
    CondorRecord *peer;
    short nearestIndex;
    short scanIndex;
    int slotStorage[30];
    CondorLinkSlot *linkSlotPtr;

    linkSlotPtr = (CondorLinkSlot *)slotStorage;
    linkSlotPtr->posX = record->posX;
    linkSlotPtr->posY = record->posY;
    linkSlotPtr->posZ = record->posZ;
    linkSlotPtr->offsetY = record->attackRange;
    linkSlotPtr->prevOffsetY = linkSlotPtr->offsetY;
    if ((record - g_CondorRecordTable.records) < 20) {
        nearestIndex = Mini_CondorFindNearestRecordInRange(linkSlotPtr, 20, 40);
    } else {
        nearestIndex = Mini_CondorFindNearestRecordInRange(linkSlotPtr, 0, 20);
    }
    if (g_CondorMatchCount != 0) {
        for (scanIndex = 0; scanIndex < g_CondorMatchCount; scanIndex++) {
            peer = g_CondorMatchListState.entries[scanIndex].peer;
            if (peer == &g_CondorRecordTable.records[record->targetSlot]) {
                if ((short)Mini_CondorBeginPeerLinkMotion(record, record->targetSlot) == 1) {
                    record->flags = record->flags | 0x80;
                    record->cycleCountdown = record->cycleReload;
                    record->sequenceIndex = 2;
                    Mini_CondorBeginRecordSequence(record);
                    return 1;
                }
            }
        }
    }
    record->cycleCountdown = record->cyclePeriod;
    return 0;
}

/*
 * Initiates coordinated motion with linked peer unit.
 */
// FUNCTION: FF7 0x0060142f
short __cdecl Mini_CondorBeginPeerLinkMotion(CondorRecord *record, int targetIndex)
{
    CondorRecord *peer;
    short moverIndex;

    moverIndex = Mini_CondorSpawnMovingRecord(record, 24);
    if (moverIndex != -1) {
        record->bufferIndex = moverIndex;
        peer = &g_CondorRecordTable.records[targetIndex];
        Mini_CondorBeginLinearMotion(record->posX, record->posY, peer->posX, peer->posY);
        record->velX = (short)g_CondorVecX;
        record->velY = (short)g_CondorVecY;
        record->angle = Mini_CondorVectorToAngle((CondorCursor *)&record->velX);
        g_CondorRecordTable.records[moverIndex].velX = (short)g_CondorVecX;
        g_CondorRecordTable.records[moverIndex].velY = (short)g_CondorVecY;
        g_CondorRecordTable.records[moverIndex].rotX = 0x400 /* 90 deg */;
        g_CondorRecordTable.records[moverIndex].rotY = record->angle;
        record->velY = 0;
        record->velX = 0;
        record->targetY = 0;
        record->targetX = 0;
        return 1;
    } else {
        return 0;
    }
}

/*
 * Attempts to link unit with first available peer in range.
 */
// FUNCTION: FF7 0x00601545
short __cdecl Mini_CondorTryLinkFirstPeer(CondorRecord *record)
{
    CondorRecord *peer;
    short nearestIndex;
    short slot;
    int slotStorage[30];
    CondorLinkSlot *linkSlotPtr;

    linkSlotPtr = (CondorLinkSlot *)slotStorage;
    linkSlotPtr->posX = record->posX;
    linkSlotPtr->posY = record->posY;
    linkSlotPtr->posZ = record->posZ;
    linkSlotPtr->offsetY = record->attackRange;
    linkSlotPtr->prevOffsetY = linkSlotPtr->offsetY;
    if ((record - g_CondorRecordTable.records) < 20) {
        nearestIndex = Mini_CondorFindNearestRecordInRange(linkSlotPtr, 20, 40);
    } else {
        nearestIndex = Mini_CondorFindNearestRecordInRange(linkSlotPtr, 0, 20);
    }
    if (g_CondorMatchCount != 0) {
        for (slot = 0; slot < g_CondorMatchCount; slot++) {
            peer = g_CondorMatchListState.entries[slot].peer;
            if ((short)Mini_CondorBeginPeerLinkMotion(record, peer - g_CondorRecordTable.records) == 1) {
                record->flags = record->flags | 0x80;
                record->cycleCountdown = record->cycleReload;
                record->sequenceIndex = 2;
                Mini_CondorBeginRecordSequence(record);
                return 1;
            }
        }
    }
    record->cycleCountdown = record->cyclePeriod;
    return 0;
}

/*
 * Steps autonomous motion logic for unlinked unit.
 */
// FUNCTION: FF7 0x00601691
void __cdecl Mini_CondorStepUnlinkedUnit(CondorRecord *unit)
{
    if ((short)Mini_CondorTickRecordLinkWait(unit) == 0) {
        Mini_CondorApplyRequestedRecordState(unit);
        Mini_CondorFinishUnitUpdate(unit);
    }
}

/*
 * Top-level update dispatcher for individual unit record.
 */
// FUNCTION: FF7 0x006016c1
void __cdecl Mini_CondorUpdateUnitRecord(CondorRecord *unit)
{
    Mini_CondorPollLinkedTargetState(unit);
    Mini_CondorApplyRequestedRecordState(unit);
    Mini_CondorFinishUnitUpdate(unit);
}

/*
 * Polls status and distance of linked target unit.
 */
// FUNCTION: FF7 0x006016ea
void __cdecl Mini_CondorPollLinkedTargetState(CondorRecord *unit)
{
    CondorRecord *peer;
    short i;
    short linkCount;

    if ((unit->flags & 4) != 0) {
        if (g_CondorRecordTable.records[unit->targetSlot].status == 0) {
            unit->flags = 0;
            unit->velY = 0;
            unit->velX = 0;
            unit->targetY = 0;
            unit->targetX = 0;
            g_CondorMatchCount = 0;
        }
        Mini_CondorFindNearestOtherRecordInRange(unit, 0, 0x13);
        linkCount = 0;
        if (g_CondorMatchCount != 0) {
            for (i = 0; i < g_CondorMatchCount; i++) {
                peer = g_CondorMatchListState.entries[i].peer;
                if (peer == &g_CondorRecordTable.records[unit->targetSlot]) {
                    unit->flags = unit->flags & 0xFE;
                    unit->state = 4;
                    unit->velY = 0;
                    unit->velX = 0;
                    unit->targetY = 0;
                    unit->targetX = 0;
                    if (peer->counter < peer->counterMax) {
                        peer->status = peer->status | 0x40;
                        unit->actionCountdown = unit->timerReload * unit->cycleReload;
                        linkCount++;
                    } else {
                        unit->flags = 0;
                    }
                }
            }
        }
        if (linkCount == 0 && (unit->flags & 1) == 0) {
            unit->flags = 0;
            unit->state = 0;
            Mini_CondorBeginRecordSequence(unit);
        }
    }
}

/*
 * The step Mini_CondorDispatchUnitKindHandler runs for unit kind 6: in state 0xA it plays sequence 6,
 * then spawns a moving kind-0xB record linked back to this unit and returns to state 0.
 */
// FUNCTION: FF7 0x00601890
void __cdecl Mini_CondorStepUnitKind6(CondorRecord *unit)
{
    short spawnIndex;

    if (unit->state == 0xA) {
        if (unit->sequenceIndex != 6) {
            unit->sequenceIndex = 6;
            Mini_CondorBeginRecordSequence(unit);
        } else if (unit->sequenceCommand == 0xFF) {
            unit->sequenceIndex = 0;
            Mini_CondorBeginRecordSequence(unit);
            unit->state = 0;
            spawnIndex = Mini_CondorSpawnMovingRecord(unit, 11);
            if (spawnIndex != -1) {
                unit->candidate = 0x80;
                g_CondorRecordTable.records[spawnIndex].rotX = 0x180;
                g_CondorRecordTable.records[spawnIndex].contactSlot = (unit - g_CondorRecordTable.records);
            }
        }
    } else {
        Mini_CondorApplyRequestedRecordState(unit);
        Mini_CondorFinishUnitUpdate(unit);
    }
}

/*
 * Performs scaled byte lookup for unit attribute pair.
 */
// FUNCTION: FF7 0x00601960
short __cdecl Mini_CondorLookupScaledPairByte(CondorRecord *unitA, CondorRecord *unitB)
{
    unsigned char byte;
    char *ptr;
    unsigned short *header;

    header = (unsigned short *)(g_pCondorDataBin + 0x18);
    ptr = (char *)(g_pCondorDataBin + (*header & 0xFFFF));
    ptr += unitA->attackerSlot * 20 + unitB->attackerSlot;
    byte = *ptr;
    if (byte != 0) {
        byte = unitA->scalePercent * byte / 100;
    } else {
        byte = 0;
    }
    return byte;
}

/*
 * Renders all active unit sprites and 3D models.
 */
// FUNCTION: FF7 0x006019e7
void __cdecl Mini_CondorDrawActiveUnits(GfxEngineContext *context, int param)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[180];
        struct { unsigned char pad_to_4[0x4]; int screenPoint; };
        struct { unsigned char pad_to_10[0x10]; int unitMatrix; };
        struct { unsigned char pad_to_30[0x30]; short unitIndex; };
        struct { unsigned char pad_to_34[0x34]; int modelMatrixPtr; };
        struct { unsigned char pad_to_38[0x38]; int worldMatrix; };
        struct { unsigned char pad_to_6C[0x6C]; SVECTOR vecX; };
        struct { unsigned char pad_to_78[0x78]; int worldMatrixPtr; };
        struct { unsigned char pad_to_7C[0x7C]; int modelMatrix; };
        struct { unsigned char pad_to_9C[0x9C]; SVECTOR rotX; };
        struct { unsigned char pad_to_A4[0xA4]; int unitMatrixPtr; };
        struct { unsigned char pad_to_A8[0xA8]; SVECTOR scaledX; };
        struct { unsigned char pad_to_B0[0xB0]; CondorRecord *recordPtr; };
    } frame;
#pragma pack(pop)

    frame.worldMatrixPtr = (int)&frame.worldMatrix;
    PushRotMatrix();
    g_CondorRotationAngles.rotX = 0;
    g_CondorRotationAngles.rotY = 0;
    g_CondorRotationAngles.rotZ = 0;
    g_CondorUnitRotationReset.rotX = 0;
    g_CondorUnitRotationReset.rotY = 0;
    g_CondorUnitRotationReset.rotZ = 0;
    g_CondorCameraTranslateX = 0;
    g_CondorCameraTranslateY = 0;
    g_CondorCameraTranslateZ = 0;
    frame.rotX.vx = 0;
    frame.rotX.vy = 0;
    frame.rotX.vz = 0;
    frame.scaledX.vx = 0;
    frame.scaledX.vy = 0;
    frame.scaledX.vz = 0;
    frame.unitMatrixPtr = (int)&frame.unitMatrix;
    frame.modelMatrixPtr = (int)&frame.modelMatrix;
    RotMatrixXYZ(&g_CondorRotationAngles.rotX, (void *)frame.worldMatrixPtr);
    SetMatrixTranslation((void *)frame.worldMatrixPtr, &g_CondorCameraTranslateX);
    SetRotMatrix((void *)frame.worldMatrixPtr);
    SetTransMatrix((void *)frame.worldMatrixPtr);
    frame.recordPtr = &g_CondorRecordTable.records[0];
    for (frame.unitIndex = 0; frame.unitIndex < 40; frame.unitIndex++, frame.recordPtr++) {
        if (frame.recordPtr->status == 0) {
            continue;
        }
        if (frame.recordPtr->posY > *(unsigned short *)&g_CondorSnapshotY + 0xF0 || frame.recordPtr->posY < *(unsigned short *)&g_CondorSnapshotY) {
            continue;
        }
        if (frame.recordPtr->posX > *(unsigned short *)&g_CondorSnapshotX + 0x140 || frame.recordPtr->posX < g_CondorCameraX) {
            continue;
        }
        PushRotMatrix();
        frame.rotX.vy = -frame.recordPtr->drawAngle;
        if (frame.unitIndex < 20) {
            frame.vecX.vx = -frame.recordPtr->rotX;
        } else {
            frame.vecX.vx = frame.recordPtr->rotX;
        }
        frame.vecX.vy = -frame.recordPtr->rotY;
        frame.vecX.vz = frame.recordPtr->rotZ;
        RotMatrixXYZ(&frame.vecX, (void *)frame.unitMatrixPtr);
        frame.vecX.vx = frame.recordPtr->worldX + 0x100 - *(unsigned short *)&g_CondorSnapshotX - 0xA0;
        frame.vecX.vy = frame.recordPtr->worldY + 0x200 - *(unsigned short *)&g_CondorSnapshotY - 0x68;
        frame.vecX.vz = frame.recordPtr->worldZ;
        RotTransPoint(&frame.vecX, (void *)(frame.unitMatrixPtr + 0x12), &frame.screenPoint);
        SetRotMatrix((void *)frame.unitMatrixPtr);
        SetTransMatrix((void *)frame.unitMatrixPtr);
        RotMatrixXYZ(&frame.rotX, (void *)frame.modelMatrixPtr);
        PreMultiplyByInstalledMatrix((PsxMatrix *)frame.modelMatrixPtr);
        ScaleMatrix((void *)frame.modelMatrixPtr, &frame.recordPtr->ringOldest);
        RotTransPoint(&frame.scaledX, (void *)(frame.modelMatrixPtr + 0x12), &frame.screenPoint);
        SetRotMatrix((void *)frame.modelMatrixPtr);
        SetTransMatrix((void *)frame.modelMatrixPtr);
        if (frame.recordPtr->retired != 0 && g_CondorDyingUnitIndex == frame.unitIndex) {
            Mini_CondorDrawUnitModel(context, frame.recordPtr, frame.modelMatrixPtr, 1, 2);
        } else {
            Mini_CondorDrawUnitModel(context, frame.recordPtr, frame.modelMatrixPtr, 0, 0);
        }
        PopRotMatrix();
    }
    for (frame.unitIndex = 40; frame.unitIndex < 52; frame.unitIndex++) {
        frame.recordPtr = &g_CondorRecordTable.records[frame.unitIndex];
        if (frame.recordPtr->status == 0) {
            continue;
        }
        PushRotMatrix();
        frame.rotX.vy = -frame.recordPtr->drawAngle;
        frame.vecX.vx = -frame.recordPtr->rotX;
        if (frame.recordPtr->unitIndex == 24) {
            if (frame.recordPtr->rotY > 0x400 && frame.recordPtr->rotY < 0xC00) {
                frame.vecX.vy = frame.recordPtr->rotY;
            } else {
                frame.vecX.vy = -frame.recordPtr->rotY;
            }
        } else {
            frame.vecX.vy = frame.recordPtr->rotY;
        }
        frame.vecX.vz = frame.recordPtr->rotZ;
        RotMatrixXYZ(&frame.vecX, (void *)frame.unitMatrixPtr);
        frame.vecX.vx = frame.recordPtr->worldX + 0x100 - *(unsigned short *)&g_CondorSnapshotX - 0xA0;
        frame.vecX.vy = frame.recordPtr->worldY + 0x200 - *(unsigned short *)&g_CondorSnapshotY - 0x68;
        frame.vecX.vz = frame.recordPtr->worldZ;
        RotTransPoint(&frame.vecX, (void *)(frame.unitMatrixPtr + 0x12), &frame.screenPoint);
        SetRotMatrix((void *)frame.unitMatrixPtr);
        SetTransMatrix((void *)frame.unitMatrixPtr);
        RotMatrixXYZ(&frame.rotX, (void *)frame.modelMatrixPtr);
        PreMultiplyByInstalledMatrix((PsxMatrix *)frame.modelMatrixPtr);
        ScaleMatrix((void *)frame.modelMatrixPtr, &frame.recordPtr->ringOldest);
        RotTransPoint(&frame.scaledX, (void *)(frame.modelMatrixPtr + 0x12), &frame.screenPoint);
        SetRotMatrix((void *)frame.modelMatrixPtr);
        SetTransMatrix((void *)frame.modelMatrixPtr);
        Mini_CondorDrawUnitModel(context, frame.recordPtr, frame.modelMatrixPtr, 0, 0);
        PopRotMatrix();
    }
    for (frame.unitIndex = 67; frame.unitIndex < 70; frame.unitIndex++) {
        frame.recordPtr = &g_CondorRecordTable.records[frame.unitIndex];
        if (frame.recordPtr->status == 0) {
            continue;
        }
        PushRotMatrix();
        frame.rotX.vy = frame.recordPtr->drawAngle;
        frame.vecX.vx = -frame.recordPtr->rotX;
        frame.vecX.vy = frame.recordPtr->rotY;
        frame.vecX.vz = -frame.recordPtr->rotZ;
        if (frame.unitIndex == 68) {
            frame.vecX.vy = -frame.recordPtr->rotZ;
            frame.vecX.vz = -frame.recordPtr->rotY;
        }
        RotMatrixXYZ(&frame.vecX, (void *)frame.unitMatrixPtr);
        frame.vecX.vx = frame.recordPtr->worldX + 0x100 - *(unsigned short *)&g_CondorSnapshotX - 0xA0;
        frame.vecX.vy = frame.recordPtr->worldY + 0x200 - *(unsigned short *)&g_CondorSnapshotY - 0x68;
        frame.vecX.vz = frame.recordPtr->worldZ;
        RotTransPoint(&frame.vecX, (void *)(frame.unitMatrixPtr + 0x12), &frame.screenPoint);
        SetRotMatrix((void *)frame.unitMatrixPtr);
        SetTransMatrix((void *)frame.unitMatrixPtr);
        RotMatrixXYZ(&frame.rotX, (void *)frame.modelMatrixPtr);
        PreMultiplyByInstalledMatrix((PsxMatrix *)frame.modelMatrixPtr);
        ScaleMatrix((void *)frame.modelMatrixPtr, &frame.recordPtr->ringOldest);
        RotTransPoint(&frame.scaledX, (void *)(frame.modelMatrixPtr + 0x12), &frame.screenPoint);
        SetRotMatrix((void *)frame.modelMatrixPtr);
        SetTransMatrix((void *)frame.modelMatrixPtr);
        if (frame.unitIndex == 69) {
            Mini_CondorDrawUnitModel(context, frame.recordPtr, frame.modelMatrixPtr, 0, 0);
        } else {
            Mini_CondorDrawUnitModel(context, frame.recordPtr, frame.modelMatrixPtr, 1, 1);
        }
        PopRotMatrix();
    }
    if (g_CondorMainState == 0xF) {
        Mini_CondorDrawTranslucentRect(param);
    }
    PopRotMatrix();
}

/*
 * Renders translucent overlay rectangle on battlefield.
 */
// FUNCTION: FF7 0x006020d9
void __cdecl Mini_CondorDrawTranslucentRect(int param)
{
    int width;
    int x;
    int h;
    int top;
    int emitRes;

    x = g_CondorViewportOffsetX + (g_CondorPanelRectX - *(unsigned short *)&g_CondorSnapshotX - 0x14) * g_CondorViewportScale;
    top = g_CondorViewportOffsetY + (g_CondorPanelRectY - *(unsigned short *)&g_CondorSnapshotY) * g_CondorViewportScale;
    width = (g_CondorLayoutPanelHeight + 30) * g_CondorViewportScale;
    h = (g_CondorRecordPanelHeight + 15) * g_CondorViewportScale;
    emitRes = Gfx_EmitFlatRect(x, top, width, h, 0xA0000000, 0, FLOAT_0_952_BITS, g_CondorTranslucentRectRenderObject);
    (*(int *)&g_CondorBuffer) = 1;
}

/*
 * Renders phase 7 special unit models and visual effects.
 */
// FUNCTION: FF7 0x0060218e
void __cdecl Mini_CondorDrawPhaseSevenUnitModels(GfxEngineContext *drawMode, int subView)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[272];
        int transResult;
        struct { unsigned char pad_to_4[0x4]; SVECTOR worldX; };
        struct { unsigned char pad_to_10[0x10]; int recordMatrixBuf; };
        struct { unsigned char pad_to_30[0x30]; short recordIndex; };
        struct { unsigned char pad_to_34[0x34]; int partMatrix; };
        struct { unsigned char pad_to_38[0x38]; int tableIndex; };
        struct { unsigned char pad_to_3C[0x3C]; int tmpMatrix; };
        struct { unsigned char pad_to_40[0x40]; int groupIndex; };
        struct { unsigned char pad_to_4C[0x4C]; SVECTOR vecX; };
        struct { unsigned char pad_to_54[0x54]; short unitTable00; };
        struct { unsigned char pad_to_56[0x56]; short unitTable01; };
        struct { unsigned char pad_to_58[0x58]; short unitTable02; };
        struct { unsigned char pad_to_5A[0x5A]; short unitTable03; };
        struct { unsigned char pad_to_5C[0x5C]; short unitTable04; };
        struct { unsigned char pad_to_5E[0x5E]; short unitTable05; };
        struct { unsigned char pad_to_60[0x60]; short unitTable06; };
        struct { unsigned char pad_to_62[0x62]; short unitTable07; };
        struct { unsigned char pad_to_64[0x64]; short unitTable08; };
        struct { unsigned char pad_to_66[0x66]; short unitTable09; };
        struct { unsigned char pad_to_68[0x68]; short unitTable10; };
        struct { unsigned char pad_to_6A[0x6A]; short unitTable11; };
        struct { unsigned char pad_to_6C[0x6C]; short unitTable12; };
        struct { unsigned char pad_to_6E[0x6E]; short unitTable13; };
        struct { unsigned char pad_to_70[0x70]; short unitTable14; };
        struct { unsigned char pad_to_72[0x72]; short unitTable15; };
        struct { unsigned char pad_to_74[0x74]; short unitTable16; };
        struct { unsigned char pad_to_76[0x76]; short unitTable17; };
        struct { unsigned char pad_to_78[0x78]; short unitTable18; };
        struct { unsigned char pad_to_7A[0x7A]; short unitTable19; };
        struct { unsigned char pad_to_7C[0x7C]; short unitTable20; };
        struct { unsigned char pad_to_7E[0x7E]; short unitTable21; };
        struct { unsigned char pad_to_80[0x80]; short unitTable22; };
        struct { unsigned char pad_to_82[0x82]; short unitTable23; };
        struct { unsigned char pad_to_84[0x84]; short unitTable24; };
        struct { unsigned char pad_to_86[0x86]; short unitTable25; };
        struct { unsigned char pad_to_88[0x88]; short unitTable26; };
        struct { unsigned char pad_to_8C[0x8C]; int peakValue; };
        struct { unsigned char pad_to_94[0x94]; int unitMatrix; };
        struct { unsigned char pad_to_98[0x98]; int partMatrixBuf; };
        struct { unsigned char pad_to_B8[0xB8]; int tmpMatrixBuf; };
        struct { unsigned char pad_to_D8[0xD8]; SVECTOR partRotX; };
        struct { unsigned char pad_to_E0[0xE0]; int recordMatrix; };
        struct { unsigned char pad_to_E4[0xE4]; int unitMatrixBuf; };
        struct { unsigned char pad_to_104[0x104]; SVECTOR localX; };
        struct { unsigned char pad_to_10C[0x10C]; CondorRecord *condorRecord; };
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
#pragma pack(pop)

    frame.unitTable00 = 2;
    frame.unitTable01 = 0x22;
    frame.unitTable02 = 0x15;
    frame.unitTable03 = 1;
    frame.unitTable04 = 0xC;
    frame.unitTable05 = 0x15;
    frame.unitTable06 = 0;
    frame.unitTable07 = 0;
    frame.unitTable08 = 0;
    frame.unitTable09 = 1;
    frame.unitTable10 = 0xC;
    frame.unitTable11 = 0x15;
    frame.unitTable12 = 0;
    frame.unitTable13 = 0;
    frame.unitTable14 = 0;
    frame.unitTable15 = 2;
    frame.unitTable16 = 0x22;
    frame.unitTable17 = 0x18;
    frame.unitTable18 = 0;
    frame.unitTable19 = 0;
    frame.unitTable20 = 0;
    frame.unitTable21 = 2;
    frame.unitTable22 = 0x22;
    frame.unitTable23 = 0x15;
    frame.unitTable24 = 1;
    frame.unitTable25 = 0xC;
    frame.unitTable26 = 0x18;
    if (g_CondorMainState == 7) {
        PushRotMatrix();
        g_CondorRotationAngles.rotX = 0;
        g_CondorRotationAngles.rotY = 0;
        g_CondorRotationAngles.rotZ = 0;
        g_CondorUnitRotationReset.rotX = 0;
        g_CondorUnitRotationReset.rotY = 0;
        g_CondorUnitRotationReset.rotZ = 0;
        g_CondorCameraTranslateX = 0;
        g_CondorCameraTranslateY = 0;
        g_CondorCameraTranslateZ = 0;
        frame.partRotX.vx = 0;
        frame.partRotX.vy = 0;
        frame.partRotX.vz = 0;
        frame.localX.vx = 0;
        frame.localX.vy = 0;
        frame.localX.vz = 0;
        frame.recordMatrix = (int)&frame.recordMatrixBuf;
        frame.partMatrix = (int)&frame.partMatrixBuf;
        frame.unitMatrix = (int)&frame.unitMatrixBuf;
        frame.tmpMatrix = (int)&frame.tmpMatrixBuf;
        RotMatrixXYZ(&g_CondorRotationAngles.rotX, (void *)frame.unitMatrix);
        SetMatrixTranslation((void *)frame.unitMatrix, &g_CondorCameraTranslateX);
        SetRotMatrix((void *)frame.unitMatrix);
        SetTransMatrix((void *)frame.unitMatrix);
        frame.worldX.vx = 0;
        frame.worldX.vy = 0;
        frame.worldX.vz = g_CondorWheelOriginZ;
        PushRotMatrix();
        RotMatrixXYZ(&g_CondorWheelRotation, (void *)g_CondorWorldMatrixPtr);
        ScaleMatrix((void *)g_CondorWorldMatrixPtr, &g_CondorWheelScaleX);
        RotTransPoint(&frame.worldX, (void *)(g_CondorWorldMatrixPtr + 0x12), &frame.transResult);
        SetRotMatrix((void *)g_CondorWorldMatrixPtr);
        SetTransMatrix((void *)g_CondorWorldMatrixPtr);
        for (frame.recordIndex = 52; frame.recordIndex < 63; frame.recordIndex++) {
            frame.condorRecord = &g_CondorRecordTable.records[frame.recordIndex];
            if (frame.condorRecord->status == 0) {
                continue;
            }
            PushRotMatrix();
            frame.partRotX.vy = -frame.condorRecord->drawAngle;
            frame.vecX.vx = -frame.condorRecord->rotX;
            frame.vecX.vy = -frame.condorRecord->rotY;
            frame.vecX.vz = frame.condorRecord->rotZ;
            RotMatrixXYZ(&frame.vecX, (void *)frame.recordMatrix);
            RotTransPoint(&frame.condorRecord->worldX, (void *)(frame.recordMatrix + 0x12), &frame.transResult);
            SetRotMatrix((void *)frame.recordMatrix);
            SetTransMatrix((void *)frame.recordMatrix);
            RotMatrixXYZ(&frame.partRotX, (void *)frame.partMatrix);
            PreMultiplyByInstalledMatrix((PsxMatrix *)frame.partMatrix);
            ScaleMatrix((void *)frame.partMatrix, &frame.condorRecord->ringOldest);
            RotTransPoint(&frame.localX, (void *)(frame.partMatrix + 0x12), &frame.transResult);
            SetRotMatrix((void *)frame.partMatrix);
            SetTransMatrix((void *)frame.partMatrix);
            Mini_CondorDrawUnitModel(drawMode, frame.condorRecord, frame.partMatrix, 0, 0);
            if ((unsigned int)frame.peakValue > (unsigned int)g_CondorPeakValue) {
                g_CondorPeakValue = frame.peakValue;
            }
            PopRotMatrix();
        }
        PopRotMatrix();
        frame.groupIndex = 0;
        if (g_CondorMatchListState.choiceActionSlot != 0) {
            for (frame.recordIndex = 64; frame.recordIndex < 67; frame.recordIndex++) {
                frame.condorRecord = &g_CondorRecordTable.records[frame.recordIndex];
                frame.tableIndex = (g_CondorMatchListState.choiceActionSlot - 1) * 9 + frame.groupIndex * 3;
                frame.groupIndex++;
                if (frame.condorRecord->status == 0) {
                    continue;
                }
                if (*(short *)(((char *)&frame.unitTable00 + frame.tableIndex * 2)) == 0) {
                    continue;
                }
                frame.condorRecord->worldX = *(short *)(((char *)&frame.unitTable01 + frame.tableIndex * 2));
                frame.condorRecord->worldY = *(short *)(((char *)&frame.unitTable02 + frame.tableIndex * 2));
                PushRotMatrix();
                frame.partRotX.vz = frame.condorRecord->drawAngle;
                frame.vecX.vx = frame.condorRecord->rotX;
                frame.vecX.vy = -frame.condorRecord->rotY;
                frame.vecX.vz = frame.condorRecord->rotZ;
                RotMatrixXYZ(&frame.vecX, (void *)frame.recordMatrix);
                frame.vecX.vx = frame.condorRecord->worldX << 1;
                frame.vecX.vy = frame.condorRecord->worldY * 2 + 10;
                frame.vecX.vz = frame.condorRecord->worldZ + 2010;
                RotTransPoint(&frame.vecX, (void *)(frame.recordMatrix + 0x12), &frame.transResult);
                SetRotMatrix((void *)frame.recordMatrix);
                SetTransMatrix((void *)frame.recordMatrix);
                RotMatrixXYZ(&frame.partRotX, (void *)frame.partMatrix);
                PreMultiplyByInstalledMatrix((PsxMatrix *)frame.partMatrix);
                ScaleMatrix((void *)frame.partMatrix, &frame.condorRecord->ringOldest);
                RotTransPoint(&frame.localX, (void *)(frame.partMatrix + 0x12), &frame.transResult);
                SetRotMatrix((void *)frame.partMatrix);
                SetTransMatrix((void *)frame.partMatrix);
                Mini_CondorDrawUnitModel(drawMode, frame.condorRecord, frame.partMatrix, 0, 0);
                PopRotMatrix();
            }
        }
        PopRotMatrix();
    }
}

/*
 * Sets pending unit command request code.
 */
// FUNCTION: FF7 0x006027b3
void __cdecl Mini_CondorSetPendingRequest(short requestCode)
{
    g_CondorPendingRequest = requestCode;
}

/*
 * Initiates combat or movement action for specified unit record.
 */
// FUNCTION: FF7 0x006027c2
void __cdecl Mini_CondorStartRecordAction(short recordIndex, short slot)
{
    g_CondorActiveActionRecord = recordIndex + 1;
    g_CondorPhaseFrames = 0;
    g_CondorActionRecordSlot = slot;
    Mini_CondorSetPendingRequest((short)recordIndex);
    Mini_CondorSetSelectedReadout(*(short *)&g_CondorRecordTable.records[slot].unitIndex, (short)slot);
    Mini_CondorSubmitSfxPlay(2, 1);
    Mini_CondorSubmitSfxPlay(0, 2);
    Mini_CondorSubmitSfxPlay(0, 3);
    Mini_CondorSubmitSfxPlay(0, 4);
    g_CondorSelectPanelPhase = 1;
    g_CondorPhaseFrames = 0;
    g_CondorBannerCounter = -32;
    Mini_CondorRequestDisplayMode2();
    g_CondorMainState = 0x11;
}

/*
 * Updates red flashing fade visual effect on damaged unit.
 */
// FUNCTION: FF7 0x0060286b
void __cdecl Mini_CondorUpdateRedFadeSprite(void)
{
    int lineOfs;
    GfxBatchPrimitive *rect;
    int corner;
    char redByte;
    int clamped;

    if (g_CondorRedFadeLevel != 0) {
        g_CondorRedFadeLevel -= 2;
        g_CondorCurrentValues.lineStartX = -96;
        g_CondorDefaultValues.lineEndX = 512;
        if (g_CondorPhase == 1) {
            g_CondorDefaultValues.defaultValue = 0x2A0;
            g_CondorCurrentValues.currentValue = g_CondorDefaultValues.defaultValue;
        } else {
            g_CondorDefaultValues.defaultValue = (*(short *)&g_CondorHighestUnitY);
            g_CondorCurrentValues.currentValue = g_CondorDefaultValues.defaultValue;
        }
        g_CondorFlags.red = *(char *)(&g_CondorRedFadeLevel);
        g_CondorFlags.green = 0;
        g_CondorFlags.blue = 0;
        lineOfs = g_CondorViewportScale * (g_CondorCurrentValues.currentValue - g_CondorCameraY);
        if (g_CondorFlags.red > 255) {
            clamped = 0xFF;
        } else {
            clamped = g_CondorFlags.red;
        }
        redByte = clamped;
        if (lineOfs > 0 && lineOfs < g_CondorViewportScale * 240) {
            rect = (GfxBatchPrimitive *)Gfx_EmitFlatRect(g_CondorViewportOffsetX, g_CondorViewportOffsetY + lineOfs, g_CondorViewportWidth, g_CondorViewportScale, 0xA0FF0000, 0, FLOAT_0_2_BITS, g_CondorRedFadeLineRenderObject);
            g_CondorRedFadeLineDrawn = 1;
            if (rect == 0) {
                return;
            }
            for (corner = 0; corner < 4; corner++) {
                ((char *)&rect->colors[corner])[2] = redByte;
                ((char *)&rect->colors[corner])[1] = 0;
                ((char *)&rect->colors[corner])[0] = 0;
                ((char *)&rect->colors[corner])[3] = 0xFF;
            }
        }
    }
}

/*
 * Empty unit rendering callback stub.
 */
// FUNCTION: FF7 0x006029f8
void __cdecl Mini_CondorNoOp_2(void)
{
}

/*
 * Finds closest unit record to specified screen point.
 */
// FUNCTION: FF7 0x006029fd
short __cdecl Mini_CondorPickRecordNearPoint(CondorCursor *cursor)
{
    CondorRecord *ptr;
    short absDeltaY;
    short distance;
    short unitY;
    short targetPy;
    short mouseX;
    short slot;
    short bestIndex;
    short recordX;
    short bestDist;

    g_CondorMatchCount = 0;
    mouseX = cursor->x;
    targetPy = cursor->y;
    ptr = &g_CondorRecordTable.records[0];
    bestIndex = 0xFFFF;
    bestDist = 16384;
    for (slot = 0; slot < 40; slot++) {
        if (ptr->status != 0 && ptr->retired == 0) {
            recordX = ptr->posX;
            if (recordX > mouseX - 0xD && recordX < mouseX + 0xD) {
                unitY = ptr->posY;
                if (unitY > targetPy - 0xA && unitY < targetPy + 0xE) {
                    g_CondorMatchListState.entries[g_CondorMatchCount].peer = ptr;
                    g_CondorMatchListState.entries[g_CondorMatchCount].recordIndex = 1;
                    g_CondorMatchCount++;
                    distance = recordX - mouseX;
                    if (distance < 0) {
                        distance = -distance;
                    }
                    absDeltaY = unitY - targetPy;
                    if (absDeltaY < 0) {
                        absDeltaY = -absDeltaY;
                    }
                    if (distance > absDeltaY) {
                        distance = absDeltaY;
                    }
                    if (distance < bestDist) {
                        bestDist = distance;
                        bestIndex = slot;
                    }
                }
            }
        }
        ptr++;
    }
    return (short)bestIndex;
}

/*
 * Finds nearest different unit within search range.
 */
// FUNCTION: FF7 0x00602b91
short __cdecl Mini_CondorFindNearestOtherRecordInRange(CondorRecord *origin, short first, short last)
{
    CondorRecord *ptr;
    short slot;
    short yGap;
    short absDx;
    unsigned short bestIndex;
    short bestDist;
    short reach;

    g_CondorMatchCount = 0;
    bestDist = 16384;
    bestIndex = 0xFFFF;
    ptr = &g_CondorRecordTable.records[first];
    for (slot = first; slot < last; slot++, ptr++) {
        if (ptr->status == 0) {
            continue;
        }
        if (ptr->posY < origin->posY) {
            if (ptr->posY < origin->posY - origin->extentY) {
                continue;
            }
        } else if (origin->posY < ptr->posY - ptr->extentY) {
            continue;
        }
        reach = ptr->extentX + origin->extentX >> 1;
        if (ptr->posX < origin->posX) {
            if (ptr->posX < origin->posX - reach) {
                continue;
            }
        } else if (origin->posX < ptr->posX - reach) {
            continue;
        }
        if (ptr == origin) {
            continue;
        }
        if (ptr->retired != 0) {
            continue;
        }
        g_CondorMatchListState.entries[g_CondorMatchCount].peer = ptr;
        g_CondorMatchListState.entries[g_CondorMatchCount].recordIndex = slot;
        g_CondorMatchCount++;
        absDx = ptr->posX - origin->posX;
        if (absDx < 0) {
            absDx = -absDx;
        }
        yGap = ptr->posY - origin->posY;
        if (yGap < 0) {
            yGap = -yGap;
        }
        if (absDx > yGap) {
            absDx = yGap;
        }
        if (absDx < bestDist) {
            bestDist = absDx;
            bestIndex = slot;
        }
    }
    return (short)bestIndex;
}

/*
 * Finds closest unit of specified team within range.
 */
// FUNCTION: FF7 0x00602d8e
short __cdecl Mini_CondorFindNearestRecordInRange(CondorLinkSlot *linkSlot, short first, short last)
{
    short distA;
    short heightRange;
    CondorRecord *ent;
    short distB;
    short i;
    unsigned short bestIdx;
    short dMin;
    short reach;

    g_CondorMatchCount = 0;
    dMin = 0x4000;
    bestIdx = 0xFFFF;
    ent = &g_CondorRecordTable.records[first];
    heightRange = linkSlot->offsetY;
    for (i = first; i < last; i++, ent++) {
        if (ent->status == 0) {
            continue;
        }
        if (ent->posY < linkSlot->posY) {
            if (ent->posY < linkSlot->posY - heightRange) {
                continue;
            }
        } else if (linkSlot->posY < ent->posY - heightRange) {
            continue;
        }
        reach = ent->extentX + linkSlot->prevOffsetY;
        if (ent->posX < linkSlot->posX) {
            if (ent->posX < linkSlot->posX - reach) {
                continue;
            }
        } else if (linkSlot->posX < ent->posX - reach) {
            continue;
        }
        if (ent->retired != 0) {
            continue;
        }
        g_CondorMatchListState.entries[g_CondorMatchCount].peer = ent;
        g_CondorMatchListState.entries[g_CondorMatchCount].recordIndex = i;
        g_CondorMatchCount++;
        distA = ent->posX - linkSlot->posX;
        if (distA < 0) {
            distA = -distA;
        }
        distB = ent->posY - linkSlot->posY;
        if (distB < 0) {
            distB = -distB;
        }
        if (distA > distB) {
            distA = distB;
        }
        if (distA < dMin) {
            dMin = distA;
            bestIdx = i;
        }
    }
    return (short)bestIdx;
}

/*
 * Counts number of units within given circular range.
 */
// FUNCTION: FF7 0x00602f7d
void __cdecl Mini_CondorCountNearbyRecordsInRange(CondorRecord *origin, short first, short last)
{
    CondorRecord *record;
    short inRange;
    short reach;
    short index;

    g_CondorMatchCount = 0;
    record = &g_CondorRecordTable.records[first];
    for (index = first; index < last; index++) {
        if (record->status != 0) {
            inRange = 0;
            reach = record->extentX + 0x1C >> 1;
            if (record->posX < origin->posX) {
                if (record->posX >= origin->posX - reach) {
                    inRange++;
                }
            } else if (origin->posX >= record->posX - reach) {
                inRange++;
            }
            if (inRange != 0) {
                inRange = 0;
                if (record->posY < origin->posY) {
                    if (record->posY >= origin->posY - 0x16) {
                        inRange++;
                    }
                } else if (origin->posY >= record->posY - record->extentY) {
                    inRange++;
                }
                if (inRange != 0 && record != origin) {
                    g_CondorMatchCount++;
                }
            }
        }
        record++;
    }
}

/*
 * Tests geometric bounding overlap between two unit records.
 */
// FUNCTION: FF7 0x006030e9
short __cdecl Mini_CondorTestRecordOverlap(CondorRecord *origin, short index)
{
    short overlap;
    CondorRecord *box;
    short inRange;
    short reach;

    overlap = 0xFFFF;
    box = &g_CondorRecordTable.records[index];
    if (box->status != 0) {
        inRange = 0;
        reach = box->extentX + origin->extentX >> 1;
        if (box->posX < origin->posX) {
            if (box->posX >= origin->posX - reach) {
                inRange++;
            }
        } else if (origin->posX >= box->posX - reach) {
            inRange++;
        }
        if (inRange != 0) {
            inRange = 0;
            if (box->posY < origin->posY) {
                if (box->posY >= origin->posY - origin->extentY) {
                    inRange++;
                }
            } else if (origin->posY >= box->posY - box->extentY) {
                inRange++;
            }
            if (inRange != 0 && box != origin) {
                overlap = index;
            }
        }
    }
    return (short)overlap;
}
} /* extern "C" */
