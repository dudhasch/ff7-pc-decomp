// src/ff7/condor/condor_effects_hits_damage.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: The Fort Condor combat update: hits and damage applied to units, visual effect
//   sequences and timers, and the spawning and resetting of moving effect and auxiliary records.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/types/condor.h"
#include "lib/codegen.h"
#include <stdlib.h>
#include <string.h>
#include "ff7/api/condor.h"
#include "lib/api/graphics.h"
#include "ff7/enums/condor.h"
#include "lib/enums/sound.h"
extern "C" {

#pragma intrinsic(strcpy)
#pragma intrinsic(strcat)
#pragma intrinsic(strlen)

#pragma intrinsic(memset)

#pragma intrinsic(abs)

/*
 * Updates active visual effect slots across frames.
 */
// FUNCTION: FF7 0x006049d0
void __cdecl Mini_CondorUpdateEffectSlots(void)
{
    CondorEffectSequence *effectAddr;
    short slotIdx;

    effectAddr = g_CondorEffectSlots;
    for (slotIdx = 0; slotIdx < 8; slotIdx++) {
        if (effectAddr->tableIndex != 0xFF) {
            if (effectAddr->uAttackerSlot == 7) {
                Mini_CondorStepEffectSequenceAlt(effectAddr, slotIdx);
            } else {
                Mini_CondorStepEffectSequence(effectAddr, slotIdx);
            }
        }
        effectAddr++;
    }
}

/*
 * Steps primary visual effect animation sequence.
 */
// FUNCTION: FF7 0x00604a49
void __cdecl Mini_CondorStepEffectSequence(CondorEffectSequence *sequence, int slot)
{
    CondorRecord *pRecPtr;
    CondorRecord buf;
    short phase;
    short n[36] = {4, 1, 0, 1, 1, 0, 0, 1, 0, 0, 2, 1,
                   0, 2, 2, 0, 2, 3, 0, 4, 4, 0, 6, 5,
                   0, 6, 6, 1, 6, 7, 4, 6, 7, 4, 1, 7};
    int lastResult;

    if (sequence->tableIndex >= 64) {
        sequence->tableIndex = 0xFF;
        sequence->timer = 0;
    } else if (sequence->tableIndex <= 10) {
        g_CondorEffectSpriteSlots[slot].drawFlags = 0;
        if (sequence->timer == 0) {
            sequence->tableIndex += 1;
            sequence->timer = n[sequence->tableIndex * 3 + 1] - 1;
            sequence->flags = (unsigned char)n[sequence->tableIndex * 3 + 2];
            g_CondorEffectSpriteSlots[slot].duration = 832;
            g_CondorEffectSpriteSlots[slot].effectId = 0xF1;
            g_CondorEffectSpriteSlots[slot].drawFlags = 0;
        } else {
            sequence->timer -= 1;
        }
        phase = n[sequence->tableIndex * 3];
        if (phase == 0) {
            g_CondorEffectSpriteSlots[slot].active = 0;
        }
        g_CondorEffectSpriteSlots[slot].renderSlot = 0;
        if (phase == 1) {
            g_CondorEffectSpriteSlots[slot].active = 1;
            g_CondorEffectSpriteSlots[slot].unused30 = 0;
            g_CondorEffectSpriteSlots[slot].state = 1;
            g_CondorEffectSpriteSlots[slot].renderSlot = 9;
        }
        if (phase == 4) {
            g_CondorEffectSpriteSlots[slot].active = 1;
            g_CondorEffectSpriteSlots[slot].unused30 = 0;
            g_CondorEffectSpriteSlots[slot].state = 3;
            g_CondorEffectSpriteSlots[slot].renderSlot = 0xA;
        }
        g_CondorEffectSpriteSlots[slot].texX = (sequence->flags & 3) << 6;
        g_CondorEffectSpriteSlots[slot].texY = (sequence->flags & 4) << 3;
        g_CondorEffectSpriteSlots[slot].screenX = sequence->posX + 384 - (unsigned short)g_CondorSnapshotX - 160;
        g_CondorEffectSpriteSlots[slot].screenY = sequence->posY + 616 - (unsigned short)g_CondorSnapshotY - 104;
        g_CondorEffectSpriteSlots[slot].baseSize = 0x40;
        g_CondorEffectSpriteSlots[slot].baseHeight = 0x20;
        lastResult = Gfx_EmitTexturedRect(g_CondorViewportOffsetX + g_CondorEffectSpriteSlots[slot].screenX * g_CondorViewportScale,
                                 g_CondorViewportOffsetY + g_CondorEffectSpriteSlots[slot].screenY * g_CondorViewportScale + g_CondorViewportInset,
                                 g_CondorEffectSpriteSlots[slot].baseSize * g_CondorViewportScale,
                                 g_CondorEffectSpriteSlots[slot].baseHeight * g_CondorViewportScale,
                                 g_CondorEffectSpriteSlots[slot].texX,
                                 g_CondorEffectSpriteSlots[slot].texY,
                                 g_CondorEffectSpriteSlots[slot].baseSize,
                                 g_CondorEffectSpriteSlots[slot].baseHeight,
                                 g_CondorEffectSpriteSlots[slot].drawFlags,
                                 FLOAT_0_2_BITS, FLOAT_HALF_BITS,
                                 ((CondorRenderObjectSlotEntry *)g_CondorRenderObjectSlots)[g_CondorEffectSpriteSlots[slot].renderSlot].renderObject);
        ((CondorRenderObjectSlotEntry *)g_CondorRenderObjectSlots)[g_CondorEffectSpriteSlots[slot].renderSlot].drawn = 1;
    } else {
        sequence->tableIndex += 1;
    }
    if (sequence->tableIndex == 12) {
        pRecPtr = &buf;
        pRecPtr->posX = sequence->posX + 256;
        pRecPtr->posY = sequence->posY + 528;
        pRecPtr->posZ = sequence->posZ;
        pRecPtr->extentX = 64;
        pRecPtr->extentY = 32;
        pRecPtr->scalePercent = sequence->scalePercent;
        pRecPtr->attackerSlot = sequence->attackerSlot;
        Mini_CondorApplyHitToTargets(pRecPtr);
    }
}

/*
 * Applies combat hit and damage calculation to targets in area.
 */
// FUNCTION: FF7 0x00604f67
short __cdecl Mini_CondorApplyHitToTargets(CondorRecord *hit)
{
    CondorRecord *pTarget;
    short index;

    index = Mini_CondorFindNearestOtherRecordInRange(hit, 20, 40);
    if (g_CondorMatchCount != 0) {
        for (index = 0; index < g_CondorMatchCount; index++) {
            pTarget = g_CondorMatchListState.entries[index].peer;
            switch (hit->uOperand) {
            case CONDOR_SHOT_FAST:
            case CONDOR_SHOT_FAST_ALT:
                pTarget->status = (short)(pTarget->status | 0x10);
                if (pTarget->cooldown == 0) {
                    pTarget->cooldown = 0x28;
                }
                pTarget->actionCountdown = 0;
                pTarget->bufferIndex = Mini_CondorLookupScaledPairByte(hit, pTarget);
                Mini_CondorApplyRecordDamage(pTarget);
                return 1;
            }
            if ((pTarget->status & 0x10) == 0 && pTarget->cooldown == 0) {
                pTarget->status = (short)(pTarget->status | 0x10);
                pTarget->cooldown = 0x28;
                pTarget->actionCountdown = 0;
                pTarget->bufferIndex = Mini_CondorLookupScaledPairByte(hit, pTarget);
                Mini_CondorApplyRecordDamage(pTarget);
            }
        }
    }
    return 0;
}

/*
 * Applies damage to unit record and handles HP reduction.
 */
// FUNCTION: FF7 0x006050a1
void __cdecl Mini_CondorApplyRecordDamage(CondorRecord *target)
{
    short slot;
    short hp;
    short damage;

    damage = target->bufferIndex;
    if (target->sequenceIndex != 2 || target->sequenceIndex != 3) {
        target->sequenceIndex = 3;
        slot = Mini_CondorBeginRecordSequence(target);
    }
    slot = Mini_CondorFindInactiveCounterEffectSlot();
    if (slot != 0xFF) {
        g_CondorCounterEffects[slot].counter = 1;
        g_CondorCounterEffects[slot].displayNumber = damage;
        g_CondorCounterEffects[slot].posX = target->posX;
        g_CondorCounterEffects[slot].posY = target->posY;
        Mini_CondorSetRecordTwoDigits(slot, damage);
    }
    hp = target->counter;
    hp -= damage;
    if (hp < 0) {
        hp = 0;
    }
    target->counter = hp;
    if (g_CondorSelectedRecord != -1) {
        Mini_CondorRefreshActiveRecordDisplay();
    }
    if (hp == 0) {
        if (target->retired == 0) {
            target->retired = 0xFF;
            if (target->unitIndex < 16) {
                g_CondorPlayerUnitCount--;
            } else {
                g_CondorSpawnCount--;
            }
            Mini_CondorBuildDigitDisplayRecords();
        }
    } else {
        Mini_CondorRestartSfxPanned(SFX_ID_CONDOR_ALLY_HIT, 3, (short)target->posX);
    }
}

/*
 * Steps alternate visual effect animation sequence.
 */
// FUNCTION: FF7 0x00605200
void __cdecl Mini_CondorStepEffectSequenceAlt(CondorEffectSequence *sequence, int slot)
{
    CondorRecord *pRecPtr;
    CondorRecord buf;
    short code;
    short n[36] = {4, 1, 0, 0, 1, 0, 0, 1, 0, 0, 2, 1,
                   4, 2, 2, 4, 2, 3, 4, 4, 4, 4, 6, 5,
                   4, 6, 6, 4, 3, 7, 4, 2, 7, 4, 1, 7};
    CondorBatchPrimitive *vertexPtr;

    if (sequence->tableIndex >= 64) {
        sequence->tableIndex = 0xFF;
        sequence->timer = 0;
    } else if (sequence->tableIndex <= 10) {
        if (sequence->timer == 0) {
            sequence->tableIndex += 1;
            sequence->timer = n[sequence->tableIndex * 3 + 1] - 1;
            sequence->flags = (unsigned char)n[sequence->tableIndex * 3 + 2];
            g_CondorEffectSpriteSlots[slot].duration = 832;
            g_CondorEffectSpriteSlots[slot].effectId = 0xF1;
            g_CondorEffectSpriteSlots[slot].drawFlags = 0;
        } else {
            sequence->timer -= 1;
        }
        code = n[sequence->tableIndex * 3];
        if (code == 0) {
            g_CondorEffectSpriteSlots[slot].active = 0;
        }
        g_CondorEffectSpriteSlots[slot].renderSlot = 0;
        if (code == 1) {
            g_CondorEffectSpriteSlots[slot].active = 1;
            g_CondorEffectSpriteSlots[slot].unused30 = 0;
            g_CondorEffectSpriteSlots[slot].state = 1;
            g_CondorEffectSpriteSlots[slot].renderSlot = 9;
        }
        if (code == 4) {
            g_CondorEffectSpriteSlots[slot].active = 1;
            g_CondorEffectSpriteSlots[slot].unused30 = 0;
            g_CondorEffectSpriteSlots[slot].state = 3;
            g_CondorEffectSpriteSlots[slot].renderSlot = 0xA;
        }
        g_CondorEffectSpriteSlots[slot].texX = (sequence->flags & 3) << 6;
        g_CondorEffectSpriteSlots[slot].texY = (sequence->flags & 4) * 8 + 0x40;
        g_CondorEffectSpriteSlots[slot].screenX = sequence->posX + 384 - (unsigned short)g_CondorSnapshotX - 160;
        g_CondorEffectSpriteSlots[slot].screenY = sequence->posY + 616 - (unsigned short)g_CondorSnapshotY - 110;
        g_CondorEffectSpriteSlots[slot].baseSize = 0x40;
        g_CondorEffectSpriteSlots[slot].baseHeight = 0x20;
        vertexPtr = (CondorBatchPrimitive *)Gfx_EmitTexturedRect(g_CondorEffectSpriteSlots[slot].screenX * g_CondorViewportScale + g_CondorViewportOffsetX,
                                 g_CondorEffectSpriteSlots[slot].screenY * g_CondorViewportScale + g_CondorViewportOffsetY + g_CondorViewportInset,
                                 g_CondorEffectSpriteSlots[slot].baseSize * g_CondorViewportScale,
                                 g_CondorEffectSpriteSlots[slot].baseHeight * g_CondorViewportScale,
                                 g_CondorEffectSpriteSlots[slot].texX,
                                 g_CondorEffectSpriteSlots[slot].texY,
                                 g_CondorEffectSpriteSlots[slot].baseSize,
                                 g_CondorEffectSpriteSlots[slot].baseHeight,
                                 g_CondorEffectSpriteSlots[slot].drawFlags,
                                 FLOAT_0_2_BITS, FLOAT_HALF_BITS,
                                 ((CondorRenderObjectSlotEntry *)g_CondorRenderObjectSlots)[g_CondorEffectSpriteSlots[slot].renderSlot].renderObject);
        if (g_CondorEffectSpriteSlots[slot].state == 3) {
            for (code = 0; code < 4; code++) {
                vertexPtr->colorBytes[code].red = 0x20;
                vertexPtr->colorBytes[code].green = 0x20;
                vertexPtr->colorBytes[code].blue = 0x20;
                vertexPtr->colorBytes[code].alpha = 0xFF;
            }
        }
        ((CondorRenderObjectSlotEntry *)g_CondorRenderObjectSlots)[g_CondorEffectSpriteSlots[slot].renderSlot].drawn = 1;
    } else {
        sequence->tableIndex += 1;
    }
    if (sequence->tableIndex == 10) {
        pRecPtr = &buf;
        pRecPtr->posX = sequence->posX + 256;
        pRecPtr->posY = sequence->posY + 528;
        pRecPtr->posZ = sequence->posZ;
        pRecPtr->extentX = 32;
        pRecPtr->extentY = 32;
        pRecPtr->scalePercent = sequence->scalePercent;
        pRecPtr->attackerSlot = sequence->attackerSlot;
        Mini_CondorApplyHitToTargets(pRecPtr);
    }
}

/*
 * Decrements and updates combat effect timers for units.
 */
// FUNCTION: FF7 0x00605779
void __cdecl Mini_CondorTickUnitTimers(void)
{
    CondorRecord *unit;
    short recordIndex;

    if (g_CondorRecordMode <= CONDOR_RECORD_MODE_CURSOR && g_CondorSelectPanelPhase == CONDOR_PANEL_CLOSED && g_CondorMainState != CONDOR_STATE_RECORD_SELECT_WHEEL) {
        unit = &g_CondorRecordTable.records[40];
        for (recordIndex = 40; recordIndex < 70; recordIndex++) {
            if (unit->status != 0) {
                (unit->actionCountdown)--;
                if (unit->actionCountdown == 0) {
                    unit->actionCountdown = unit->timerReload;
                    if (unit->unitIndex == 24) {
                        Mini_CondorUpdateUnitByType(unit);
                    } else {
                        Mini_CondorTickTimedRecordState(unit);
                    }
                }
            }
            unit++;
        }
    }
}

/*
 * Updates unit state based on unit type characteristics.
 */
// FUNCTION: FF7 0x0060583f
void __cdecl Mini_CondorUpdateUnitByType(CondorRecord *unit)
{
    short result;
    short arrived;

    Mini_CondorIntegrateUnitVelocity(unit);
    switch (unit->uOperand) {
    case CONDOR_SHOT_SPIN:
    case CONDOR_SHOT_SPIN_ALT:
        unit->rotX += 0x80;
        Mini_CondorApplyHitToTargets(unit);
        break;
    case CONDOR_SHOT_ARC:
    case CONDOR_SHOT_ARC_ALT:
        unit->rotX += 0x200 /* 45 deg */;
        result = unit->costOrArcVelY >> 8;
        unit->worldY += result;
        unit->costOrArcVelY += 20;
        break;
    case CONDOR_SHOT_FAST:
    case CONDOR_SHOT_FAST_ALT:
        Mini_CondorIntegrateUnitVelocity(unit);
        Mini_CondorIntegrateUnitVelocity(unit);
        unit->rotX = 0x200 /* 45 deg */;
        if (unit->counter > 3) {
            if (unit->attackerSlot == 4) {
                result = Mini_CondorApplyHitToTargets(unit);
            } else {
                result = Mini_CondorApplyHitToFirstTarget(unit);
            }
            if (result != 0) {
                unit->counter = 2;
            }
        }
        (unit->counter)--;
        if (unit->counter == 0) {
            unit->status = 0;
        }
    }
    switch (unit->uOperand) {
    case CONDOR_SHOT_SPIN:
    case CONDOR_SHOT_SPIN_ALT:
        (unit->counter)--;
        if (unit->counter == 0) {
            unit->status = 0;
            unit->retired = 0;
            Mini_CondorSubmitSfxPlay(0, 2);
        }
        break;
    case CONDOR_SHOT_ARC:
    case CONDOR_SHOT_ARC_ALT:
        arrived = 0;
        if (unit->targetY < unit->worldY + 0x200) {
            arrived++;
        } else {
            if (unit->targetX == unit->posX) {
                unit->velX = 0;
            }
            if (unit->targetY == unit->posY) {
                unit->velY = 0;
            }
            if ((unit->velX | unit->velY) == 0) {
                arrived++;
            }
        }
        if (arrived != 0) {
            unit->status = 0;
            unit->retired = 0;
            result = Mini_CondorSpawnEffectSlot(unit);
        }
    }
}

/*
 * Applies damage to first valid target within range.
 */
// FUNCTION: FF7 0x00605aa1
short __cdecl Mini_CondorApplyHitToFirstTarget(CondorRecord *source)
{
    CondorRecord *pTarget;
    short index;

    index = Mini_CondorFindNearestOtherRecordInRange(source, 0, 20);
    if (g_CondorMatchCount != 0) {
        for (index = 0; index < g_CondorMatchCount; index++) {
            pTarget = g_CondorMatchListState.entries[index].peer;
            if ((pTarget->status & 0x10) == 0) {
                pTarget->status = pTarget->status | 0x10;
                if (pTarget->cooldown == 0) {
                    pTarget->cooldown = 0x28;
                }
                pTarget->actionCountdown = 0;
                pTarget->bufferIndex = Mini_CondorLookupScaledPairByte(source, pTarget);
                Mini_CondorApplyRecordDamage(pTarget);
                return 1;
            }
        }
    }
    return 0;
}

/*
 * Allocates and initializes an effect slot for visual playback.
 */
// FUNCTION: FF7 0x00605b6e
short __cdecl Mini_CondorSpawnEffectSlot(CondorRecord *source)
{
    CondorEffectSequence *slot;
    short index;

    slot = g_CondorEffectSlots;
    for (index = 0; index < 8; index++) {
        if (slot->tableIndex == 0xFF) {
            Mini_CondorRestartSfxPanned(0xF, 2, (short)source->posX);
            g_CondorEffectSlots[index].tableIndex = 0;
            g_CondorEffectSlots[index].posX = source->worldX;
            g_CondorEffectSlots[index].posY = source->worldY;
            g_CondorEffectSlots[index].posZ = source->worldZ;
            g_CondorEffectSlots[index].scalePercent = source->scalePercent;
            g_CondorEffectSlots[index].attackerSlot = source->attackerSlot;
            return 1;
        }
        slot++;
    }
    return 0;
}

/*
 * Ticks timed duration for temporary unit record state.
 */
// FUNCTION: FF7 0x00605c51
void __cdecl Mini_CondorTickTimedRecordState(CondorRecord *record)
{
    CondorRecord *contact;
    short res;
    CondorRecord *entry;

    if (record->sequenceIndex == 7) {
        entry = &g_CondorRecordTable.records[record->attackerSlot];
        if (entry->retired != 0) {
            record->status = 0;
        }
        if (entry->sequenceIndex != 2) {
            record->status = 0;
        }
    }
    if (record->unitIndex == 11) {
        res = Mini_CondorFindNearestOtherRecordInRange(record, 0x14, 0x28);
        if (res != -1) {
            record->status = 0;
            record->retired = 0;
            res = Mini_CondorSpawnEffectSlot(record);
            contact = &g_CondorRecordTable.records[record->contactSlot];
            if (contact->status != 0 && contact->unitIndex == 13) {
                contact->candidate = contact->candidate & 0x7F;
            }
        }
    }
    Mini_CondorRampUnitTurnRate(record);
    if (record->sequenceCommand == 0xFF) {
        record->status = 0;
        record->retired = 0;
    }
}

/*
 * Converts angle and distance magnitude to coordinate offsets.
 */
// FUNCTION: FF7 0x00605d59
void __cdecl Mini_CondorAngleMagnitudeToOffset(int angle, int magnitude)
{
    int cosVal, sinPart;
    sinPart = SinFixed((short)angle & 0x3ff) * (short)magnitude >> 12;
    cosVal = CosFixed((short)angle & 0x3ff) * (short)magnitude >> 12;
    switch ((short)angle & 0xc00) {
    case CONDOR_OCTANT_N_NW:
        g_CondorVecX = -sinPart;
        g_CondorVecY = cosVal;
        break;
    case CONDOR_OCTANT_W_SW:
        g_CondorVecX = -cosVal;
        g_CondorVecY = -sinPart;
        break;
    case CONDOR_OCTANT_S_SE:
        g_CondorVecX = sinPart;
        g_CondorVecY = -cosVal;
        break;
    case CONDOR_OCTANT_E_NE:
        g_CondorVecX = cosVal;
        g_CondorVecY = sinPart;
        break;
    default:
        g_CondorVecX = sinPart;
        g_CondorVecY = cosVal;
    }
}

/*
 * Spawns a moving projectile or effect record.
 */
// FUNCTION: FF7 0x00605e43
short __cdecl Mini_CondorSpawnMovingRecord(CondorRecord *source, int motionKind)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[28];
        CondorCursor tileX;
        struct { unsigned char pad_to_8[0x8]; short distance; };
        struct {
            unsigned char pad_to_C[0xC];
            union {
                int recordIndex;
                short recordIndexShort;
            };
        };
        struct { unsigned char pad_to_10[0x10]; short accumulator; };
        struct {
            unsigned char pad_to_14[0x14];
            union {
                short unitKind;
                char unitKindByte;
            };
        };
        struct { unsigned char pad_to_18[0x18]; CondorRecord *condorRecord; };
    } frame;
#pragma pack(pop)

    frame.recordIndexShort = Mini_CondorFindInactiveRecordInRange(40, 69);
    if ((frame.recordIndex & 0xFFFF) != 0xFFFF) {
        if ((short)motionKind <= 1) {
            frame.unitKind = 0x18;
        } else {
            frame.unitKind = (short)motionKind;
        }
        frame.condorRecord = &g_CondorRecordTable.records[(frame.recordIndex & 0xFFFF)];
        g_pCondorConfigEntry->unitIndex = frame.unitKindByte;
        g_pCondorConfigEntry->initialValue = 0;
        g_pCondorConfigEntry->candidate = 0;
        g_pCondorConfigEntry->tileY = 0;
        g_pCondorConfigEntry->tileX = 0;
        frame.tileX.x = g_pCondorConfigEntry->uTileX;
        frame.tileX.y = g_pCondorConfigEntry->uTileY;
        Mini_CondorInitRecordFromConfig(frame.condorRecord, g_pCondorConfigEntry, frame.recordIndex & 0xFFFF, &frame.tileX);
        frame.condorRecord->posX = source->posX;
        frame.condorRecord->posY = source->posY;
        frame.condorRecord->posZ = source->posZ;
        frame.condorRecord->worldX = source->worldX;
        frame.condorRecord->worldY = source->worldY;
        frame.condorRecord->worldZ = source->worldZ;
        frame.condorRecord->angle = source->angle;
        frame.condorRecord->posZ -= 20;
        frame.condorRecord->worldZ -= 20;
        frame.condorRecord->scalePercent = source->scalePercent;
        frame.condorRecord->actionCountdown = 1;
        frame.condorRecord->timerReload = 1;
        frame.condorRecord->attackerSlot = source->unitIndexByte;
        if ((short)motionKind <= 0x18) {
            if ((short)motionKind <= 1) {
                frame.condorRecord->operand = motionKind;
                frame.condorRecord->counter = source->counter;
                frame.condorRecord->attackerSlot = source->attackerSlot;
            } else {
                frame.condorRecord->counter = source->attackRange;
            }
            Mini_CondorApplySourceStateToRecord(source, frame.condorRecord);
            Mini_CondorReadRecordSequenceWord(frame.condorRecord);
        }
        frame.condorRecord->counterMax = frame.condorRecord->counter;
        switch (frame.condorRecord->uOperand) {
        case CONDOR_SHOT_ARC:
        case CONDOR_SHOT_ARC_ALT:
            /* Retail caller prototype (short, short) -> void preserved for /Od argument promotion on `Mini_CondorAngleMagnitudeToOffset`. */
            ((void(__cdecl *)(short, short))Mini_CondorAngleMagnitudeToOffset)((short)frame.condorRecord->angle, source->uAttackRange);
            break;
        default:
            /* Retail caller prototype (short, short) -> void preserved for /Od argument promotion on `Mini_CondorAngleMagnitudeToOffset`. */
            ((void(__cdecl *)(short, short))Mini_CondorAngleMagnitudeToOffset)((short)frame.condorRecord->angle, 0x64);
        }
        frame.condorRecord->targetX = frame.condorRecord->posX + g_CondorVecX;
        frame.condorRecord->targetY = frame.condorRecord->posY + g_CondorVecY;
        Mini_CondorBeginLinearMotion(frame.condorRecord->posX, frame.condorRecord->posY, frame.condorRecord->targetX, frame.condorRecord->targetY);
        frame.condorRecord->velX = (short)g_CondorVecX;
        frame.condorRecord->velY = (short)g_CondorVecY;
        switch (frame.condorRecord->uOperand) {
        case CONDOR_SHOT_ARC:
        case CONDOR_SHOT_ARC_ALT:
            if (frame.condorRecord->velX == 256) {
                frame.distance = frame.condorRecord->posX - frame.condorRecord->targetX;
            } else {
                frame.distance = frame.condorRecord->posY - frame.condorRecord->targetY;
            }
            if (frame.distance < 0) {
                frame.distance = -frame.distance;
            }
            frame.accumulator = 0;
            for (frame.unitKind = 0; frame.unitKind < frame.distance / 2; frame.unitKind++) {
                frame.accumulator += 0x14;
            }
            frame.condorRecord->costOrArcVelY = -frame.accumulator + frame.distance * 2;
        }
        return frame.recordIndexShort;
    } else {
        return -1;
    }
}

/*
 * Copies source entity properties into newly spawned record.
 */
// FUNCTION: FF7 0x0060619d
void __cdecl Mini_CondorApplySourceStateToRecord(CondorRecord *source, CondorRecord *spawned)
{
    switch (source->unitIndex) {
    case CONDOR_UNIT_SHOOTER:
    case CONDOR_UNIT_ENEMY_SHOOTER:
        spawned->operand = CONDOR_SHOT_FAST;
        spawned->posY -= 16;
        spawned->worldY -= 16;
        spawned->extentY = 2;
        spawned->extentX = 2;
        Mini_CondorRestartSfxPanned(SFX_ID_CONDOR_FAST_SHOT, 3, (short)spawned->posX);
        break;
    case CONDOR_UNIT_STONER:
        spawned->operand = CONDOR_SHOT_SPIN;
        Mini_CondorPlaySfxPanned(SFX_ID_CONDOR_SPIN_SHOT, 2, (short)spawned->posX);
        break;
    case CONDOR_UNIT_TRISTONER:
        spawned->operand = CONDOR_SHOT_SPIN;
        Mini_CondorSpawnMovingRecord(spawned, CONDOR_SHOT_SPIN);
        Mini_CondorSpawnMovingRecord(spawned, CONDOR_SHOT_SPIN_ALT);
        Mini_CondorPlaySfxPanned(SFX_ID_CONDOR_SPIN_BURST, 2, (short)spawned->posX);
        break;
    case CONDOR_UNIT_CATAPULT:
        spawned->operand = CONDOR_SHOT_ARC;
        spawned->posY -= 40;
        spawned->worldY -= 40;
        Mini_CondorPlaySfxPanned(SFX_ID_CONDOR_ARC_SHOT, 2, (short)spawned->posX);
        break;
    case CONDOR_UNIT_FIRE_CATAPULT:
        spawned->operand = CONDOR_SHOT_ARC_ALT;
        spawned->posY -= 40;
        spawned->worldY -= 40;
        Mini_CondorPlaySfxPanned(SFX_ID_CONDOR_ARC_SHOT, 2, (short)spawned->posX);
        break;
    case CONDOR_UNIT_SUB_SHOT:
        if (spawned->uOperand == CONDOR_SHOT_SPIN) {
            spawned->posX -= 16;
            spawned->worldX -= 16;
        } else {
            spawned->posX += 16;
            spawned->worldX += 16;
        }
        spawned->operand = CONDOR_SHOT_SPIN;
    }
}

/*
 * Resets auxiliary effect and projectile records.
 */
// FUNCTION: FF7 0x006063aa
void __cdecl Mini_CondorResetAuxiliaryRecords(void)
{
    CondorRecord *rec;

    rec = (CondorRecord *)g_CondorRecordTable.auxiliaryRecords;
    rec->worldX = 1024;
    rec->posX = 1024;
    rec->rotX = 0;
    rec->rotY = 0;
    rec->rotZ = 0;
    rec = &g_CondorRecordTable.records[68];
    rec->worldX = 1024;
    rec->posX = 1024;
    rec->rotX = 0;
    rec->rotY = 0;
    rec->rotZ = 0;
    rec = &g_CondorRecordTable.records[67];
    rec->worldX = 1024;
    rec->posX = 1024;
    rec->rotX = 0;
    rec->rotY = 0;
    rec->rotZ = 0;
}

/*
 * Refreshes auxiliary records associated with selection.
 */
// FUNCTION: FF7 0x0060644e
void __cdecl Mini_CondorRefreshSelectionAuxRecords(void)
{
    CondorRecord *pSelected;
    CondorRecord *aux;

    if (g_CondorSelectedRecord != -1) {
        aux = (CondorRecord *)g_CondorRecordTable.auxiliaryRecords;
        pSelected = &g_CondorRecordTable.records[g_CondorSelectedRecord];
        aux->posX = pSelected->posX;
        aux->posY = pSelected->posY;
        aux->posZ = pSelected->posZ;
        aux->worldX = pSelected->worldX;
        aux->worldY = pSelected->worldY;
        aux->worldZ = pSelected->worldZ;
        aux->angle = pSelected->sAngle;
        aux->posZ -= 10;
        aux->worldZ -= 10;
        aux->worldY -= (short)(char)g_CondorSelectionAuxByteTable[pSelected->unitIndex];
        aux->rotX = 0;
        aux->rotY = 0;
        aux->rotZ = aux->angle;
        aux->drawAngle = 0;
        if (g_CondorMainState == CONDOR_STATE_IDLE) {
            /* Retail caller reads residual EAX/AX as `short` after `void Mini_CondorAngleMagnitudeToOffset` returns. */
            ((short(__cdecl *)(short, short))Mini_CondorAngleMagnitudeToOffset)((short)aux->angle, 0x64);
            aux->targetX = aux->posX + g_CondorVecX;
            aux->targetY = aux->posY + g_CondorVecY;
            Mini_CondorBeginLinearMotion(aux->posX, aux->posY, aux->targetX, aux->targetY);
            aux->velX = (short)g_CondorVecX;
            aux->velY = (short)g_CondorVecY;
        }
    } else {
        Mini_CondorResetAuxiliaryRecords();
    }
    if (g_CondorMainState == CONDOR_STATE_RECORD_ANGLE_INPUT || g_CondorMainState == CONDOR_STATE_RECORD_ANGLE_COMMIT) {
        pSelected = &g_CondorRecordTable.records[g_CondorSelectedRecord];
        switch (pSelected->unitIndex) {
        case CONDOR_UNIT_STONER:
            aux = &g_CondorRecordTable.records[68];
            aux->operand = 0;
            Mini_CondorRecordCodeHookNoOp(aux, 1);
            aux->ringNewest = *(int *)&g_CondorScreenDistanceBuf / (unsigned short)g_CondorStepDivisor;
            break;
        case CONDOR_UNIT_TRISTONER:
            aux = &g_CondorRecordTable.records[68];
            aux->operand = 0;
            Mini_CondorRecordCodeHookNoOp(aux, 1);
            aux->ringNewest = *(int *)&g_CondorScreenDistanceBuf / (unsigned short)g_CondorStepDivisor * 0x64 / 0x4B;
            break;
        case CONDOR_UNIT_CATAPULT:
            aux = &g_CondorRecordTable.records[67];
            aux->operand = 1;
            Mini_CondorRecordCodeHookNoOp(aux, 2);
            break;
        default:
            aux = &g_CondorRecordTable.records[67];
            aux->operand = 1;
            Mini_CondorRecordCodeHookNoOp(aux, 2);
        }
        aux->posX = pSelected->posX;
        aux->posY = pSelected->posY;
        aux->posZ = pSelected->posZ;
        aux->worldX = pSelected->worldX;
        aux->worldY = pSelected->worldY;
        aux->worldZ = pSelected->worldZ;
        aux->angle = pSelected->sAngle;
        aux->posZ -= 10;
        aux->worldZ -= 10;
        aux->rotX = 0x400 /* 90 deg */;
        aux->rotY = 0;
        aux->rotZ = 0;
        aux->drawAngle = 0;
        if (aux - g_CondorRecordTable.records == 68) {
            aux->rotY = pSelected->sAngle;
        } else {
            /* Retail caller reads residual EAX/AX as `short` after `void Mini_CondorAngleMagnitudeToOffset` returns. */
            ((short(__cdecl *)(short, short))Mini_CondorAngleMagnitudeToOffset)(pSelected->sDrawAngle, (*(short *)&g_CondorSelectedAttackRangeBuf));
            aux->worldX = pSelected->worldX + g_CondorVecX;
            aux->worldY = pSelected->worldY + g_CondorVecY + -40;
            if (pSelected->unitIndex == 8) {
                aux->ringOldest = *(int *)&g_CondorScreenDistanceBuf / (unsigned short)g_CondorStepDivisor << 1;
            } else {
                aux->ringOldest = *(int *)&g_CondorScreenDistanceBuf / (unsigned short)g_CondorStepDivisor;
            }
        }
    }
}

/*
 * Initializes auxiliary record buffers and slot state.
 */
// FUNCTION: FF7 0x0060683e
void __cdecl Mini_CondorInitAuxiliaryRecords(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[12];
        CondorCursor initX;
        struct { unsigned char pad_to_4[0x4]; short initZ; };
        struct { unsigned char pad_to_8[0x8]; CondorRecord *condorRecord; };
    } frame;
#pragma pack(pop)

    g_pCondorConfigEntry->initialValue = 0;
    g_pCondorConfigEntry->candidate = 0;
    g_pCondorConfigEntry->tileY = 0;
    g_pCondorConfigEntry->tileX = 0;
    frame.initZ = 0;
    frame.initX.y = frame.initZ;
    frame.initX.x = frame.initX.y;
    frame.condorRecord = (CondorRecord *)g_CondorRecordTable.auxiliaryRecords;
    g_pCondorConfigEntry->unitIndex = 26;
    Mini_CondorInitRecordFromConfig(frame.condorRecord, g_pCondorConfigEntry, 0x45, &frame.initX);
    frame.condorRecord = &g_CondorRecordTable.records[68];
    g_pCondorConfigEntry->unitIndex = 27;
    Mini_CondorInitRecordFromConfig(frame.condorRecord, g_pCondorConfigEntry, 0x44, &frame.initX);
    frame.condorRecord->operand = 0;
    Mini_CondorRecordCodeHookNoOp(frame.condorRecord, 1);
    frame.condorRecord = &g_CondorRecordTable.records[67];
    g_pCondorConfigEntry->unitIndex = 27;
    Mini_CondorInitRecordFromConfig(frame.condorRecord, g_pCondorConfigEntry, 0x43, &frame.initX);
    frame.condorRecord->operand = 1;
    Mini_CondorRecordCodeHookNoOp(frame.condorRecord, 2);
    Mini_CondorResetAuxiliaryRecords();
}

/*
 * Spawns new unit record at source position.
 */
// FUNCTION: FF7 0x0060692d
short __cdecl Mini_CondorSpawnRecordAtSource(CondorRecord *source, short kind)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[16];
        CondorCursor tileX;
        struct {
            unsigned char pad_to_8[0x8];
            union {
                int recordIndex;
                short recordIndexShort;
            };
        };
        struct { unsigned char pad_to_C[0xC]; CondorRecord *condorRecord; };
    } frame;
#pragma pack(pop)

    frame.recordIndexShort = Mini_CondorFindInactiveRecordInRange(40, 69);
    if ((frame.recordIndex & 0xFFFF) != 0xFFFF) {
        frame.condorRecord = &g_CondorRecordTable.records[(frame.recordIndex & 0xFFFF)];
        g_pCondorConfigEntry->unitIndex = kind;
        g_pCondorConfigEntry->initialValue = 0;
        g_pCondorConfigEntry->candidate = 0;
        g_pCondorConfigEntry->tileY = 0;
        g_pCondorConfigEntry->tileX = 0;
        frame.tileX.x = g_pCondorConfigEntry->uTileX;
        frame.tileX.y = g_pCondorConfigEntry->uTileY;
        Mini_CondorInitRecordFromConfig(frame.condorRecord, g_pCondorConfigEntry, frame.recordIndex & 0xFFFF, &frame.tileX);
        frame.condorRecord->posX = source->posX;
        frame.condorRecord->posY = source->posY;
        frame.condorRecord->posZ = source->posZ;
        frame.condorRecord->worldX = source->worldX;
        frame.condorRecord->worldY = source->worldY;
        frame.condorRecord->worldZ = source->worldZ;
        frame.condorRecord->angle = source->angle;
        return frame.recordIndexShort;
    } else {
        return -1;
    }
}

} /* extern "C" */
