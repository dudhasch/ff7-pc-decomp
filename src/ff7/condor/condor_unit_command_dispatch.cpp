// src/ff7/condor/condor_unit_command_dispatch.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Dispatches Fort Condor unit commands and the placement choice: opens the choice ring,
//   handles the selection wheel and angle input, purchases or retires unit slots, and rebuilds the
//   list of active units.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/types/condor.h"
#include "lib/types/graphics.h"
#include <stdlib.h>
#include <string.h>
#include "ff7/api/condor.h"
#include "ff7/enums/condor.h"
#include "lib/enums/file.h"
extern "C" {

#pragma intrinsic(strcpy)
#pragma intrinsic(strcat)
#pragma intrinsic(strlen)

#pragma intrinsic(memset)

#pragma intrinsic(abs)


/*
 * Dispatches the active unit action handler based on action code.
 */
// FUNCTION: FF7 0x00603230
void __cdecl Mini_CondorDispatchRecordAction(void)
{
    unsigned char action;
    CondorRecord *record;

    if (g_CondorActionRowIndex == 0) {
        g_CondorActionRowA = CONDOR_ACTION_NONE;
    }
    action = g_CondorActionRowA;
    if (g_CondorActionCursor == 1) {
        action = g_CondorActionRowB;
    }
    if (g_CondorActionCursor == 2) {
        action = g_Condor_Byte;
    }
    record = &g_CondorRecordTable.records[g_CondorSelectedRecord];
    switch (action) {
    case CONDOR_ACTION_SPECIAL_SHOT:
        if (((unsigned short)record->candidate & 0x80) == 0) {
            record->state = CONDOR_UNIT_STATE_SPECIAL_SHOT;
            record->angle = 0;
            record->targetY = 0;
            record->targetX = 0;
            record->velY = 0;
            record->velX = 0;
            Mini_CondorResetRecordMode();
        } else {
            Mini_CondorSubmitSfxPlay(3, 4);
        }
        break;
    case CONDOR_ACTION_CANCEL_1:
        Mini_CondorResetRecordMode();
        break;
    case CONDOR_ACTION_RETIRE:
        Mini_CondorRetireSelectedRecord();
        Mini_CondorResetRecordMode();
        break;
    case CONDOR_ACTION_MOVE:
        g_CondorRecordMode = CONDOR_RECORD_MODE_TARGET;
        g_CondorCursor.x = g_CondorCursorPos.x;
        g_CondorCursor.y = g_CondorCursorPos.y;
        g_CondorCommandCode = CONDOR_COMMAND_MOVE;
        break;
    case CONDOR_ACTION_CANCEL_4:
        Mini_CondorResetRecordMode();
        break;
    case CONDOR_ACTION_TURN:
        g_CondorRecordAngle = g_CondorRecordTable.records[g_CondorSelectedRecord].drawAngle;
        g_CondorPrevWheelSector = 0;
        g_CondorWheelAngle = (unsigned short)g_CondorRecordAngle + 0x200 & 0xFFF;
        (*(short *)&g_CondorSelectedAttackRangeBuf) = (unsigned char)record->attackRange;
        g_CondorMainState = CONDOR_STATE_RECORD_ANGLE_COMMIT;
        break;
    default:
        Mini_CondorResetRecordMode();
    }
}

/*
 * Dispatches unit command handler from current state.
 */
// FUNCTION: FF7 0x006033cd
void __cdecl Mini_CondorDispatchRecordCommand(void)
{
    char val;

    if (g_CondorActionRowIndex == 0) {
        g_CondorActionRowA = CONDOR_ACTION_NONE;
    }
    val = g_CondorActionRowA;
    if (g_CondorActionCursor == 1) {
        val = g_CondorActionRowB;
    }
    if (g_CondorActionCursor == 2) {
        val = g_Condor_Byte;
    }
    switch ((char)val) {
    case CONDOR_ACTION_SPECIAL_SHOT:
        break;
    case CONDOR_ACTION_MOVE:
        Mini_CondorApplyRecordCommand(g_CondorSelectedRecord);
    }
}

/*
 * Initiates targeted movement towards specified destination.
 */
// FUNCTION: FF7 0x00603441
void __cdecl Mini_CondorBeginTargetMove(CondorRecord *record)
{
    record->velX = 0;
    record->velY = 0;
    record->moveStep = 0;
    record->targetX = g_CondorCursor.x;
    record->targetY = g_CondorCursor.y;
    Mini_CondorBeginLinearMotion(record->posX, record->posY, (unsigned short)record->targetX, (unsigned short)record->targetY);
    record->velX = (*(short *)&g_CondorVecX);
    record->velY = *(short *)&g_CondorVecY;
    record->angle = Mini_CondorVectorToAngle((CondorCursor *)&record->velX);
    record->turnRate = 0;
}

/*
 * Applies pending command parameters to unit record.
 */
// FUNCTION: FF7 0x006034e2
void __cdecl Mini_CondorApplyRecordCommand(int recordIndex)
{
    CondorRecord *record;

    record = &g_CondorRecordTable.records[recordIndex];
    record->commandCode = g_CondorCommandCode;
    g_CondorLayoutStart = 0;
    switch ((unsigned char)g_CondorCommandCode) {
    case CONDOR_COMMAND_ATTACK:
    case CONDOR_COMMAND_ATTACK_ALT:
        Mini_CondorPickRecordNearPoint(&g_CondorCursor);
        if (g_CondorMatchCount >= 2) {
            Mini_CondorRunRecordLayoutLoop();
        }
        if (g_CondorLayoutStart != 2) {
            record->state = CONDOR_UNIT_STATE_ATTACK_MOVE;
            record->flags = 3;
            record->targetSlot = (char)g_CondorPickedRecordIndex;
            Mini_CondorBeginTargetMove(record);
        }
        break;
    case CONDOR_COMMAND_TARGET_ALLY:
        Mini_CondorPickRecordNearPoint(&g_CondorCursor);
        if (g_CondorMatchCount >= 2) {
            Mini_CondorRunRecordLayoutLoop();
        }
        if (g_CondorLayoutStart != 2) {
            record->state = CONDOR_UNIT_STATE_SUPPORT_MOVE;
            record->flags = 5;
            record->targetSlot = (char)g_CondorPickedRecordIndex;
            Mini_CondorBeginTargetMove(record);
        }
        break;
    case CONDOR_COMMAND_MOVE:
        record->state = CONDOR_UNIT_STATE_MOVING;
        record->flags = 1;
        record->targetSlot = (char)g_CondorPickedRecordIndex;
        Mini_CondorBeginTargetMove(record);
        record->status = record->status & 0xD3;
        record->contactSlot = 0xFF;
        record->inContact = 0;
        record->bufferIndex = 0;
        record->sequenceFlag = 0;
        record->targetSlot = 0xFF;
        break;
    case CONDOR_COMMAND_HOLD:
        record->state = CONDOR_UNIT_STATE_IDLE;
        record->flags = 0;
        record->targetX = 0;
        record->targetY = 0;
        record->targetSlot = 0xFF;
        break;
    case CONDOR_COMMAND_MOVE_STATE_6:
        record->state = CONDOR_UNIT_STATE_PATROL_6;
        record->flags = 9;
        Mini_CondorBeginTargetMove(record);
        record->targetSlot = 0xFF;
        break;
    case CONDOR_COMMAND_MOVE_STATE_7:
        record->state = CONDOR_UNIT_STATE_PATROL_7;
        record->flags = 0x11;
        Mini_CondorBeginTargetMove(record);
        record->targetSlot = 0xFF;
    }
}

/*
 * Checks whether data entry exceeds boundary threshold.
 */
// FUNCTION: FF7 0x006036bf
short __cdecl Mini_CondorDataEntryExceeds(short entryIndex)
{
    int entry;
    unsigned short *base;

    base = (unsigned short *)g_pCondorDataBin;
    entry = g_pCondorDataBin + (*base & 0xFFFF);
    entry += entryIndex << 5;
    if ((unsigned int)*(unsigned short *)entry > (unsigned int)g_CondorFunds) {
        return 1;
    } else {
        return 0;
    }
}

/*
 * Attempts to begin unit placement or action choice.
 */
// FUNCTION: FF7 0x00603711
short __cdecl Mini_CondorTryBeginChoice(void)
{
    if (g_CondorChoiceTimer != 0) {
        g_CondorChoiceTimer = 0;
        if (g_CondorMainState != CONDOR_STATE_IDLE) {
            return 1;
        }
        if (g_CondorActiveActionRecord != 0) {
            return 1;
        }
        if (g_CondorPlayerUnitCount <= 20) {
            Mini_CondorSubmitSfxPlay(0, 2);
            if ((short)Mini_CondorBeginChoiceRing() == 1) {
                Mini_CondorRequestDisplayMode1();
                g_CondorSelectPanelPhase = CONDOR_PANEL_CLOSED;
            }
        } else {
            Mini_CondorSubmitSfxPlay(3, 4);
        }
    }
    return 0;
}

/*
 * Sets up placement choice ring geometry and options.
 */
// FUNCTION: FF7 0x0060378b
short __cdecl Mini_CondorBeginChoiceRing(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[80];
        CondorCursor cursorX;
        struct { unsigned char pad_to_4[0x4]; short cursorZ; };
        struct { unsigned char pad_to_8[0x8]; short ringIndex; };
        struct { unsigned char pad_to_C[0xC]; int recordSlot; };
        struct { unsigned char pad_to_10[0x10]; short placedCount; };
        struct { unsigned char pad_to_14[0x14]; short tmpIndex; };
        struct { unsigned char pad_to_18[0x18]; char tableD0; };
        struct { unsigned char pad_to_19[0x19]; char tableD1; };
        struct { unsigned char pad_to_1A[0x1A]; char tableD2; };
        struct { unsigned char pad_to_1B[0x1B]; char tableD3; };
        struct { unsigned char pad_to_1C[0x1C]; char tableD4; };
        struct { unsigned char pad_to_1D[0x1D]; char tableD5; };
        struct { unsigned char pad_to_1E[0x1E]; char tableD6; };
        struct { unsigned char pad_to_1F[0x1F]; char tableD7; };
        struct { unsigned char pad_to_20[0x20]; char tableD8; };
        struct { unsigned char pad_to_21[0x21]; char tableD9; };
        struct { unsigned char pad_to_22[0x22]; char tableD10; };
        struct { unsigned char pad_to_24[0x24]; char *tablePtr; };
        struct { unsigned char pad_to_28[0x28]; char tableC0; };
        struct { unsigned char pad_to_29[0x29]; char tableC1; };
        struct { unsigned char pad_to_2A[0x2A]; char tableC2; };
        struct { unsigned char pad_to_2B[0x2B]; char tableC3; };
        struct { unsigned char pad_to_2C[0x2C]; char tableC4; };
        struct { unsigned char pad_to_2D[0x2D]; char tableC5; };
        struct { unsigned char pad_to_2E[0x2E]; char tableC6; };
        struct { unsigned char pad_to_2F[0x2F]; char tableC7; };
        struct { unsigned char pad_to_30[0x30]; char tableC8; };
        struct { unsigned char pad_to_31[0x31]; short tableC9; };
        struct { unsigned char pad_to_34[0x34]; char tableB0; };
        struct { unsigned char pad_to_35[0x35]; char tableB1; };
        struct { unsigned char pad_to_36[0x36]; char tableB2; };
        struct { unsigned char pad_to_37[0x37]; char tableB3; };
        struct { unsigned char pad_to_38[0x38]; char tableB4; };
        struct { unsigned char pad_to_39[0x39]; char tableB5; };
        struct { unsigned char pad_to_3A[0x3A]; char tableB6; };
        struct { unsigned char pad_to_3B[0x3B]; char tableB7; };
        struct { unsigned char pad_to_3C[0x3C]; char tableB8; };
        struct { unsigned char pad_to_3D[0x3D]; short tableB9; };
        struct { unsigned char pad_to_40[0x40]; char tableA0; };
        struct { unsigned char pad_to_41[0x41]; char tableA1; };
        struct { unsigned char pad_to_42[0x42]; char tableA2; };
        struct { unsigned char pad_to_43[0x43]; char tableA3; };
        struct { unsigned char pad_to_44[0x44]; char tableA4; };
        struct { unsigned char pad_to_45[0x45]; char tableA5; };
        struct { unsigned char pad_to_46[0x46]; char tableA6; };
        struct { unsigned char pad_to_47[0x47]; char tableA7; };
        struct { unsigned char pad_to_48[0x48]; short tableA8; };
        struct {
            unsigned char pad[74];
            char value;
        } at6;
        struct { unsigned char pad_to_4C[0x4C]; CondorRecord *record; };
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
#pragma pack(pop)

    frame.tableA0 = 1;
    frame.tableA1 = 2;
    frame.tableA2 = 3;
    frame.tableA3 = 4;
    frame.tableA4 = 0xC;
    frame.tableA5 = 0xD;
    frame.tableA6 = 5;
    frame.tableA7 = 7;
    memset(&frame.tableA8, 0, 3);
    frame.tableB0 = 1;
    frame.tableB1 = 2;
    frame.tableB2 = 3;
    frame.tableB3 = 4;
    frame.tableB4 = 0xC;
    frame.tableB5 = 0xD;
    frame.tableB6 = 5;
    frame.tableB7 = 7;
    frame.tableB8 = 8;
    memset(&frame.tableB9, 0, 2);
    frame.tableC0 = 1;
    frame.tableC1 = 2;
    frame.tableC2 = 3;
    frame.tableC3 = 4;
    frame.tableC4 = 0xC;
    frame.tableC5 = 0xD;
    frame.tableC6 = 5;
    frame.tableC7 = 6;
    frame.tableC8 = 7;
    memset(&frame.tableC9, 0, 2);
    frame.tableD0 = 1;
    frame.tableD1 = 2;
    frame.tableD2 = 3;
    frame.tableD3 = 4;
    frame.tableD4 = 0xC;
    frame.tableD5 = 0xD;
    frame.tableD6 = 5;
    frame.tableD7 = 6;
    frame.tableD8 = 7;
    frame.tableD9 = 8;
    memset(&frame.tableD10, 0, 1);
    switch (g_CondorUnitChoiceTier & 3) {
    case CONDOR_RING_LAYOUT_TWO_WAY:
        frame.tablePtr = &frame.tableA0;
        g_CondorChoiceSectorCount = 8;
        g_CondorSectorAngle = 0x200 /* 45 deg */;
        break;
    case CONDOR_RING_LAYOUT_THREE_WAY:
        frame.tablePtr = &frame.tableB0;
        g_CondorChoiceSectorCount = 9;
        g_CondorSectorAngle = 0x1C7;
        break;
    case CONDOR_RING_LAYOUT_SINGLE:
        frame.tablePtr = &frame.tableC0;
        g_CondorChoiceSectorCount = 9;
        g_CondorSectorAngle = 0x1C7;
        break;
    default:
        frame.tablePtr = &frame.tableD0;
        g_CondorChoiceSectorCount = 10;
        g_CondorSectorAngle = 0x199;
    }
    for (frame.tmpIndex = 0; frame.tmpIndex < 11; frame.tmpIndex++) {
        g_CondorChoiceRing[frame.tmpIndex] = *frame.tablePtr;
        frame.tablePtr++;
    }
    g_CondorSelectPanelPhase = CONDOR_PANEL_OPEN;
    Mini_CondorRequestDisplayMode2();
    g_CondorPrevWheelSector = 0;
    g_CondorWheelAngle = 0;
    g_CondorWheelAngleOffset = 0;
    g_CondorWheelOffset = 0;
    g_CondorWheelSpinOffset = g_CondorWheelOffset;
    g_CondorMatchListState.choiceActionSlot = 0;
    g_CondorCommandResetShortA = 0;
    (*(short *)((char *)&g_CondorCommandResetShortA + 2)) = 0;
    g_CondorCommandResetShortB = 0;
    g_CondorCommandPosX = 0;
    g_CondorCommandPosY = 0;
    g_CondorCommandDoubleSpan = (*(int *)&g_CondorScreenDistanceBuf) << 1;
    *(short *)&g_CondorWheelRotation = 0xB8;
    ((SVECTOR *)&g_CondorWheelRotation)->vy = 0;
    g_CondorWheelRotationZ = 0;
    g_CondorWheelScaleX = (*(int *)&g_CondorScreenDistanceBuf);
    g_CondorWheelScaleY = (*(int *)&g_CondorScreenDistanceBuf);
    g_CondorWheelScaleZ = (*(int *)&g_CondorScreenDistanceBuf);
    frame.placedCount = 0;
    *(short *)&frame.recordSlot = 0x34;
    frame.record = &g_CondorRecordTable.records[(frame.recordSlot & 0xFFFF)];
    g_pCondorConfigEntry->unitIndex = g_CondorChoiceRing[g_CondorRingBase];
    g_pCondorConfigEntry->candidate = 0;
    g_pCondorConfigEntry->initialValue = 0;
    frame.cursorX.x = g_CondorCursorPos.x;
    frame.cursorX.y = 1024 - g_CondorCursorPos.y;
    frame.cursorZ = 0xFFD8 /* -40 */;
    /* Retail caller prototype (int, int, char, void *) -> short preserved for /Od argument promotion on `Mini_CondorInitRecordFromConfig`. */
    frame.tmpIndex = ((short(__cdecl *)(int, int, char, void *))Mini_CondorInitRecordFromConfig)((int)frame.record, (int)g_pCondorConfigEntry, (char)((frame.recordSlot & 0xFFFF) | 0x80), &frame.cursorX);
    if (frame.tmpIndex == 1) {
        Mini_CondorSubmitSfxPlay(3, 4);
        frame.record->status = 0;
        return 1;
    } else {
        Mini_CondorSubmitSfxPlay(1, 4);
        frame.record->velY = 0;
        frame.record->velX = 0;
        frame.record->rotZ = 0;
        frame.record->rotX = 0;
        frame.record->operand = 0;
        g_CondorChoiceUnaffordable = Mini_CondorDataEntryExceeds((unsigned char)g_pCondorConfigEntry->unitIndex);
        Mini_CondorIncrementSequenceCounter(frame.record);
        g_CondorChoiceUnaffordable = 0;
        g_CondorWheelOrigin = frame.record->worldX;
        (*(short *)((char *)&g_CondorWheelOrigin + 2)) = frame.record->worldY;
        g_CondorWheelOriginZ = frame.record->worldZ - 280;
        Mini_CondorPlaceRecordOnRing(frame.record, (short)frame.placedCount);
        frame.placedCount++;
        frame.record++;
        (*(short *)&frame.recordSlot)++;
        g_CondorMainState = CONDOR_STATE_RECORD_SELECT_WHEEL;
        Mini_CondorSetSelectedReadout((unsigned char)g_pCondorConfigEntry->unitIndex, g_CondorPurchaseSlot + 1);
        Mini_CondorSelectRecordDisplay((unsigned char)g_pCondorConfigEntry->unitIndex);
        for (frame.tmpIndex = 1; frame.tmpIndex < g_CondorChoiceSectorCount; frame.tmpIndex++) {
            frame.ringIndex = frame.tmpIndex + g_CondorRingBase;
            if (frame.ringIndex >= g_CondorChoiceSectorCount) {
                frame.ringIndex -= g_CondorChoiceSectorCount;
            }
            g_pCondorConfigEntry->unitIndex = g_CondorChoiceRing[frame.ringIndex];
            Mini_CondorInitRecordFromConfig((CondorRecord *)frame.record, g_pCondorConfigEntry, frame.recordSlot & 0xFFFF, &frame.cursorX);
            frame.record->velY = 0;
            frame.record->velX = 0;
            frame.record->rotZ = 0;
            frame.record->rotX = 0;
            frame.record->operand = 0;
            g_CondorChoiceUnaffordable = Mini_CondorDataEntryExceeds((unsigned char)g_pCondorConfigEntry->unitIndex);
            Mini_CondorIncrementSequenceCounter(frame.record);
            g_CondorChoiceUnaffordable = 0;
            Mini_CondorPlaceRecordOnRing(frame.record, (short)frame.placedCount);
            frame.placedCount++;
            frame.record++;
            (*(short *)&frame.recordSlot)++;
        }
        *(short *)&frame.recordSlot = 0x42;
        frame.record = &g_CondorRecordTable.records[(frame.recordSlot & 0xFFFF)];
        g_pCondorConfigEntry->unitIndex = 17;
        Mini_CondorInitRecordFromConfig((CondorRecord *)frame.record, g_pCondorConfigEntry, frame.recordSlot & 0xFFFF, &frame.cursorX);
        frame.record->worldX = 12;
        frame.record->worldY = 24;
        frame.record->worldZ = 0xFFC0;
        frame.record->ringNewest = ((*(int *)&g_CondorScreenDistanceBuf) / (unsigned short)g_CondorStepDivisor) / 2;
        frame.record->ringMiddle = frame.record->ringNewest;
        frame.record->ringOldest = frame.record->ringMiddle;
        frame.record->ringNewest = (*(int *)&g_CondorScreenDistanceBuf) / (unsigned short)g_CondorStepDivisor;
        frame.record->ringMiddle = frame.record->ringNewest;
        frame.record->ringOldest = frame.record->ringMiddle;
        frame.record->velY = 0;
        frame.record->velX = 0;
        frame.record->rotZ = 0;
        frame.record->rotX = 0;
        frame.record->operand = 0;
        *(short *)&frame.recordSlot = 0x41;
        frame.record = &g_CondorRecordTable.records[(frame.recordSlot & 0xFFFF)];
        g_pCondorConfigEntry->unitIndex = 18;
        Mini_CondorInitRecordFromConfig((CondorRecord *)frame.record, g_pCondorConfigEntry, frame.recordSlot & 0xFFFF, &frame.cursorX);
        frame.record->worldX = 34;
        frame.record->worldY = 24;
        frame.record->worldZ = 0xFFC0;
        frame.record->ringNewest = ((*(int *)&g_CondorScreenDistanceBuf) / (unsigned short)g_CondorStepDivisor) / 2;
        frame.record->ringMiddle = frame.record->ringNewest;
        frame.record->ringOldest = frame.record->ringMiddle;
        frame.record->ringNewest = (*(int *)&g_CondorScreenDistanceBuf) / (unsigned short)g_CondorStepDivisor;
        frame.record->ringMiddle = frame.record->ringNewest;
        frame.record->ringOldest = frame.record->ringMiddle;
        frame.record->velY = 0;
        frame.record->velX = 0;
        frame.record->rotZ = 0;
        frame.record->rotX = 0;
        frame.record->operand = 0;
        *(short *)&frame.recordSlot = 0x40;
        frame.record = &g_CondorRecordTable.records[(frame.recordSlot & 0xFFFF)];
        g_pCondorConfigEntry->unitIndex = 19;
        Mini_CondorInitRecordFromConfig((CondorRecord *)frame.record, g_pCondorConfigEntry, frame.recordSlot & 0xFFFF, &frame.cursorX);
        frame.record->worldX = 56;
        frame.record->worldY = 24;
        frame.record->worldZ = 0xFFC0;
        frame.record->ringNewest = ((*(int *)&g_CondorScreenDistanceBuf) / (unsigned short)g_CondorStepDivisor) / 2;
        frame.record->ringMiddle = frame.record->ringNewest;
        frame.record->ringOldest = frame.record->ringMiddle;
        frame.record->ringNewest = (*(int *)&g_CondorScreenDistanceBuf) / (unsigned short)g_CondorStepDivisor;
        frame.record->ringMiddle = frame.record->ringNewest;
        frame.record->ringOldest = frame.record->ringMiddle;
        frame.record->velY = 0;
        frame.record->velX = 0;
        frame.record->rotZ = 0;
        frame.record->rotX = 0;
        frame.record->operand = 0;
        g_CondorMainState = CONDOR_STATE_RECORD_SELECT_WHEEL;
        return 0;
    }
}

/*
 * Positions unit record on placement choice ring.
 */
// FUNCTION: FF7 0x00603f67
void __cdecl Mini_CondorPlaceRecordOnRing(CondorRecord *record, short sectorIndex)
{
    Mini_CondorAngleMagnitudeToOffset(-(g_CondorSectorAngle * sectorIndex) & 0xFFF, 0x8C);
    record->worldX = (*(short *)&g_CondorVecX);
    record->worldY = 0;
    record->worldZ = -g_CondorVecY;
}

/*
 * Clears leading short fields of unit record.
 */
// FUNCTION: FF7 0x00603fb4
void __cdecl Mini_CondorClearRecordLeadShorts(void)
{
    struct CondorRecord *record;
    int sectorIndex;

    record = &g_CondorRecordTable.records[52];
    for (sectorIndex = 0; sectorIndex < g_CondorChoiceSectorCount; sectorIndex++) {
        record->status = 0;
        record++;
    }
    if (g_CondorPhase == 1) {
        Mini_CondorSetPendingRequest(0xC);
    }
}

/*
 * Validates funds and attempts to purchase new unit slot.
 */
// FUNCTION: FF7 0x00604009
short __cdecl Mini_CondorTryPurchaseRecordSlot(short kind)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[20];
        CondorCursor cursorX;
        struct { unsigned char pad_to_4[0x4]; short cursorZ; };
        struct { unsigned char pad_to_8[0x8]; int recordSlot; };
        struct { unsigned char pad_to_C[0xC]; short initResult; };
        struct { unsigned char pad_to_10[0x10]; CondorRecord *record; };
    } frame;
#pragma pack(pop)

    *(short *)&frame.recordSlot = Mini_CondorFindInactiveRecordInRange(0, 20);
    if ((frame.recordSlot & 0xFFFF) != 0xFFFF) {
        g_CondorMainState = CONDOR_STATE_RECORD_ANGLE_INPUT;
        g_CondorPurchaseSlot = *(short *)&frame.recordSlot;
        frame.record = &g_CondorRecordTable.records[(frame.recordSlot & 0xFFFF)];
        g_pCondorConfigEntry->unitIndex = kind;
        g_pCondorConfigEntry->candidate = 0;
        g_pCondorConfigEntry->initialValue = 0;
        frame.cursorX.x = g_CondorCursorPos.x;
        frame.cursorX.y = 1024 - g_CondorCursorPos.y;
        frame.cursorZ = 40;
        frame.initResult = Mini_CondorInitRecordFromConfig((CondorRecord *)frame.record, g_pCondorConfigEntry, frame.recordSlot & 0xFFFF, &frame.cursorX);
        if (frame.initResult == 1) {
            Mini_CondorSubmitSfxPlay(3, 4);
            frame.record->status = 0;
            g_CondorMainState = CONDOR_STATE_IDLE;
            return 1;
        }
        Mini_CondorSubmitSfxPlay(1, 4);
        g_CondorFunds -= g_CondorRecordCost;
        frame.record->velY = 0;
        frame.record->velX = 0;
        g_CondorPlayerUnitCount++;
        Mini_CondorAdvanceTypeFrame(frame.record);
        g_CondorSelectedRecord = *(short *)&frame.recordSlot;
        Mini_CondorRefreshActiveRecordDisplay();
    }
    if (frame.record->kind == 3 || frame.record->kind == 4) {
        g_CondorMainState = CONDOR_STATE_RECORD_ANGLE_INPUT;
        (*(short *)&g_CondorSelectedAttackRangeBuf) = (unsigned char)frame.record->attackRange;
        g_CondorWheelAngle = 0x200 /* 45 deg */;
        g_CondorPrevWheelSector = 0;
    } else {
        Mini_CondorRequestDisplayMode1();
        g_CondorSelectPanelPhase = CONDOR_PANEL_CLOSED;
        Mini_CondorClearRecordLeadShorts();
        g_CondorMainState = CONDOR_STATE_IDLE;
    }
    return 0;
}

/*
 * Converts angle value to choice wheel sector index.
 */
// FUNCTION: FF7 0x006041b6
short __cdecl Mini_CondorAngleToSectorIndex(short angle)
{
    short normAngle;
    short sector;
    normAngle = angle & 0xfff;
    if (normAngle == 0)
        sector = 0;
    else
        sector = normAngle / g_CondorSectorAngle;
    if (normAngle == g_CondorChoiceSectorCount)
        sector = 0;
    return sector;
}

/*
 * Updates circular unit selection wheel rotation and focus.
 */
// FUNCTION: FF7 0x00604208
void __cdecl Mini_CondorTickRecordSelectWheel(void)
{
    struct CondorRecord *wheelRecord;
    short angle;
    short sectorIdx;
    short selIndex;
    short baseIdx;

    baseIdx = 0x34;
    wheelRecord = &g_CondorRecordTable.records[baseIdx];
    g_CondorWheelSpinOffset += 0x20;
    g_CondorWheelOffset += 0x10;
    for (angle = 0; angle < g_CondorChoiceSectorCount; angle++) {
        wheelRecord->rotY = g_CondorWheelSpinOffset;
        wheelRecord++;
    }
    g_CondorRecordTable.records[66].rotY = -g_CondorWheelOffset;
    g_CondorRecordTable.records[65].rotY = -g_CondorWheelOffset;
    g_CondorRecordTable.records[64].rotY = -g_CondorWheelOffset;
    if ((g_CondorInputMask & INPUT_LEFT) != 0) {
        g_CondorWheelAngleOffset = 0xFFD0;
    }
    if ((g_CondorInputMask & INPUT_RIGHT) != 0) {
        g_CondorWheelAngleOffset = 0x30;
    }
    if (g_CondorWheelAngleOffset != 0) {
        if ((g_CondorInputMask & (INPUT_LEFT | INPUT_RIGHT)) == 0) {
            if (g_CondorWheelAngleOffset > 0) {
                angle = g_CondorWheelAngle & 0xFFF;
                sectorIdx = Mini_CondorAngleToSectorIndex((short)angle);
                if (sectorIdx == (short)Mini_CondorAngleToSectorIndex(angle + g_CondorWheelAngleOffset)) {
                    g_CondorWheelAngle += g_CondorWheelAngleOffset;
                    ((SVECTOR *)&g_CondorWheelRotation)->vy += g_CondorWheelAngleOffset;
                } else {
                    g_CondorWheelAngleOffset = (sectorIdx + 1) * g_CondorSectorAngle - angle;
                    g_CondorWheelAngle += g_CondorWheelAngleOffset;
                    ((SVECTOR *)&g_CondorWheelRotation)->vy += g_CondorWheelAngleOffset;
                    g_CondorWheelAngleOffset = 0;
                }
            } else {
                angle = g_CondorWheelAngle & 0xFFF;
                sectorIdx = Mini_CondorAngleToSectorIndex((short)angle);
                if (sectorIdx == (short)Mini_CondorAngleToSectorIndex(angle + g_CondorWheelAngleOffset)) {
                    g_CondorWheelAngle += g_CondorWheelAngleOffset;
                    ((SVECTOR *)&g_CondorWheelRotation)->vy += g_CondorWheelAngleOffset;
                } else {
                    if (sectorIdx != 0) {
                        angle = -(angle % g_CondorSectorAngle);
                        g_CondorWheelAngleOffset = angle;
                        g_CondorWheelAngle += g_CondorWheelAngleOffset;
                        ((SVECTOR *)&g_CondorWheelRotation)->vy += g_CondorWheelAngleOffset;
                    } else {
                        angle = -angle;
                        g_CondorWheelAngle = 0;
                        ((SVECTOR *)&g_CondorWheelRotation)->vy = 0;
                    }
                    g_CondorWheelAngleOffset = 0;
                }
            }
        } else {
            g_CondorWheelAngle += g_CondorWheelAngleOffset;
            ((SVECTOR *)&g_CondorWheelRotation)->vy += g_CondorWheelAngleOffset;
        }
    }
    selIndex = (g_CondorWheelAngle & 0xFFF) / g_CondorSectorAngle;
    if (selIndex == g_CondorChoiceSectorCount) {
        selIndex = 0;
    }
    if (selIndex != g_CondorPrevWheelSector) {
        g_CondorPrevWheelSector = selIndex;
        Mini_CondorSubmitSfxPlay(1, 1);
        selIndex += g_CondorRingBase;
        if (selIndex >= g_CondorChoiceSectorCount) {
            selIndex -= g_CondorChoiceSectorCount;
        }
        Mini_CondorSetSelectedReadout(g_CondorChoiceRing[selIndex], (int)g_CondorPurchaseSlot + 1);
        Mini_CondorSelectRecordDisplay(g_CondorChoiceRing[selIndex]);
    }
    if ((g_CondorInputMask & (INPUT_LEFT | INPUT_RIGHT)) == 0) {
        g_CondorRecordTable.records[(selIndex + baseIdx)].rotY = 0;
        switch (g_CondorChoiceRing[selIndex]) {
        case CONDOR_WHEEL_PHASE_SPIN:
        case CONDOR_WHEEL_PHASE_SETTLE:
        case CONDOR_WHEEL_PHASE_COMMIT:
            g_CondorMatchListState.choiceActionSlot = g_CondorChoiceRing[selIndex] - 1;
            break;
        default:
            g_CondorMatchListState.choiceActionSlot = 0;
        }
    } else {
        g_CondorMatchListState.choiceActionSlot = 0;
    }
    if ((g_CondorInputPressed & INPUT_CIRCLE) != 0) {
        if (g_CondorChoiceRing[selIndex] != 9) {
            if ((unsigned int)g_CondorFunds >= (unsigned int)g_CondorRecordCost) {
                Mini_CondorClearRecordLeadShorts();
                Mini_CondorTryPurchaseRecordSlot(g_CondorChoiceRing[selIndex]);
                Mini_CondorBuildDigitDisplayRecords();
            } else {
                Mini_CondorSubmitSfxPlay(3, 4);
            }
        } else {
            Mini_CondorSubmitSfxPlay(3, 4);
        }
    }
    if ((g_CondorInputPressed & INPUT_CROSS) != 0) {
        Mini_CondorSubmitSfxPlay(4, 4);
        Mini_CondorClearRecordLeadShorts();
        Mini_CondorRequestDisplayMode1();
        g_CondorSelectPanelPhase = CONDOR_PANEL_CLOSED;
        g_CondorMainState = CONDOR_STATE_IDLE;
    }
}

/*
 * Handles directional angular input for unit aiming.
 */
// FUNCTION: FF7 0x006046a7
void __cdecl Mini_CondorTickRecordAngleInput(void)
{
    CondorRecord *unit;
    short frameCount;

    unit = &g_CondorRecordTable.records[g_CondorSelectedRecord];
    Mini_CondorStepSelectAngleFromInput(unit);
    if ((g_CondorInputPressed & INPUT_CIRCLE) != 0) {
        Mini_CondorSubmitSfxPlay(1, 4);
        Mini_CondorRequestDisplayMode1();
        g_CondorMainState = CONDOR_STATE_IDLE;
        g_CondorSelectPanelPhase = CONDOR_PANEL_CLOSED;
    }
    if ((g_CondorInputPressed & INPUT_CROSS) != 0) {
        Mini_CondorSubmitSfxPlay(4, 4);
        Mini_CondorRequestDisplayMode1();
        g_CondorMainState = CONDOR_STATE_IDLE;
        g_CondorSelectPanelPhase = CONDOR_PANEL_CLOSED;
        g_CondorFunds += g_CondorRecordCost;
        g_CondorRecordTable.records[g_CondorSelectedRecord].status = 0;
        g_CondorPlayerUnitCount--;
        frameCount = g_CondorUnitFrameIndex[unit->unitIndex];
        frameCount--;
        if (frameCount == 0) {
            frameCount = 99;
        }
        g_CondorUnitFrameIndex[unit->unitIndex] = frameCount;
        Mini_CondorBuildDigitDisplayRecords();
    }
}

/*
 * Steps selection angle incrementally based on directional input.
 */
// FUNCTION: FF7 0x006047ac
void __cdecl Mini_CondorStepSelectAngleFromInput(CondorRecord *record)
{
    if ((g_CondorInputMask & INPUT_LEFT) != 0) {
        g_CondorWheelAngle += 0x20;
        if (g_CondorWheelAngle > 0x400) {
            g_CondorWheelAngle = 0x400 /* 90 deg */;
        }
    }
    if ((g_CondorInputMask & INPUT_RIGHT) != 0) {
        g_CondorWheelAngle -= 0x20;
        if (g_CondorWheelAngle < 0) {
            g_CondorWheelAngle = 0;
        }
    }
    record->angle = g_CondorWheelAngle - 0x200;
    record->drawAngle = record->angle;
    (g_CondorSelectAngleState.selectAngle) = record->drawAngle;
}

/*
 * Commits target angle on confirmation or restores previous on cancel.
 */
// FUNCTION: FF7 0x0060484b
void __cdecl Mini_CondorTickRecordAngleCommitOrRestore(void)
{
    CondorRecord *p;
    int q; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */

    p = &g_CondorRecordTable.records[g_CondorSelectedRecord];
    Mini_CondorStepSelectAngleFromInput(p);
    if ((g_CondorInputPressed & INPUT_CIRCLE) != 0) {
        Mini_CondorSubmitSfxPlay(1, 4);
        Mini_CondorRequestDisplayMode1();
        g_CondorMainState = CONDOR_STATE_IDLE;
        g_CondorSelectPanelPhase = CONDOR_PANEL_CLOSED;
        p->attackRange = (char)(*(short *)&g_CondorSelectedAttackRangeBuf);
        Mini_CondorResetRecordMode();
    }
    if ((g_CondorInputPressed & INPUT_CROSS) != 0) {
        Mini_CondorSubmitSfxPlay(4, 4);
        Mini_CondorRequestDisplayMode1();
        g_CondorMainState = CONDOR_STATE_IDLE;
        g_CondorSelectPanelPhase = CONDOR_PANEL_CLOSED;
        p->angle = g_CondorRecordAngle;
        p->drawAngle = g_CondorRecordAngle;
        Mini_CondorResetRecordMode();
    }
}

/*
 * Retires selected unit from battlefield and refunds cost.
 */
// FUNCTION: FF7 0x00604906
void __cdecl Mini_CondorRetireSelectedRecord(void)
{
    g_CondorRecordTable.records[g_CondorSelectedRecord].retired = 0xFF;
    g_CondorPlayerUnitCount--;
    Mini_CondorBuildDigitDisplayRecords();
}

/*
 * Rebuilds list of active units for display and selection.
 */
// FUNCTION: FF7 0x00604930
void __cdecl Mini_CondorBuildActiveRecordList(void)
{
    CondorActiveRecord *pActiveRecord;
    CondorRecord *unit;
    short recordIndex;

    unit = &g_CondorRecordTable.records[0];
    pActiveRecord = (CondorActiveRecord *)&g_CondorActiveRecords;
    for (recordIndex = 0; recordIndex < 20; recordIndex++) {
        if (unit->status != 0) {
            pActiveRecord->posX = unit->posX;
            pActiveRecord->invertedPosY = 1024 - unit->posY;
            pActiveRecord->unitIndex = unit->unitIndex;
            pActiveRecord->angle = unit->angle;
            pActiveRecord++;
        }
        unit++;
    }
    pActiveRecord->posX = 0xFFFF;
}
} /* extern "C" */
