// src/ff7/condor/condor_unit_record_advance.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Advances Fort Condor unit records along their waypoint paths and animation scripts,
//   initializes records from configuration tables, checks placement against terrain and peer units,
//   and spawns scheduled reinforcements.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/types/condor.h"
#include <stdlib.h>
#include <string.h>
#include "ff7/api/condor.h"
extern "C" {

#pragma intrinsic(strcpy)
#pragma intrinsic(strcat)
#pragma intrinsic(strlen)

#pragma intrinsic(memset)

#pragma intrinsic(abs)


/*
 * Advances unit frame animation based on unit type.
 */
// FUNCTION: FF7 0x00606a50
void __cdecl Mini_CondorAdvanceTypeFrame(CondorRecord *record)
{
    int frameIndex;
    *(char *)&frameIndex = g_CondorUnitFrameIndex[record->unitIndex];
    record->frameIndex = *(char *)&frameIndex;
    (*(char *)&frameIndex)++;
    if ((frameIndex & 0xff) >= 100)
        *(char *)&frameIndex = 1;
    g_CondorUnitFrameIndex[record->unitIndex] = *(char *)&frameIndex;
}

/*
 * Empty sequence callback stub.
 */
// FUNCTION: FF7 0x00606a9f
void __cdecl Mini_CondorRecordCodeHookNoOp(CondorRecord *record, int code)
{
}

/*
 * Increments global sequence counter.
 */
// FUNCTION: FF7 0x00606aa4
void __cdecl Mini_CondorIncrementSequenceCounter(CondorRecord *record)
{
    g_CondorSequenceCounter++;
}

/*
 * Computes per-axis step increments for unit path traversal.
 */
// FUNCTION: FF7 0x00606ab6
void __cdecl Mini_CondorComputeRecordStepIncrements(CondorRecord *record)
{
    int step;

    step = *(int *)&g_CondorScreenDistanceBuf / (unsigned short)g_CondorStepDivisor;
    record->ringOldest = step;
    record->ringMiddle = step;
    record->ringNewest = step;
    record->worldZ = (*(short *)&g_CondorScreenDistanceBuf);
}

/*
 * Loads destination vertex coordinates for unit record.
 */
// FUNCTION: FF7 0x00606afb
void __cdecl Mini_CondorLoadRecordVertexTarget(CondorRecord *record)
{
    CondorRegionRecord *target;

    target = (&g_CondorUnitTable + record->targetIndex);
    record->rotX = target->rotX;
    record->rotZ = target->rotZ;
    record->worldZ = (*(short *)&g_CondorScreenDistanceBuf);
}

/*
 * Advances unit position along active waypoint path segment.
 */
// FUNCTION: FF7 0x00606b41
void __cdecl Mini_CondorAdvanceRecordPathSegment(CondorRecord *record)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[16];
        int pathEntryPtr;
        struct { unsigned char pad_to_4[0x4]; int segmentWord; };
        struct { unsigned char pad_to_8[0x8]; short targetX; };
        struct { unsigned char pad_to_A[0xA]; short targetY; };
    } frame;
#pragma pack(pop)

    record->subPixelX = 0;
    record->subPixelY = 0;
    record->velX = 0;
    record->velY = 0;
    record->moveStep = 0;
    if (record->unitIndex <= 0xF) {
        record->velY = 0x100;
        record->targetX = 0;
        record->targetY = 0;
        record->candidate = 0;
        record->angle = Mini_CondorVectorToAngle((CondorCursor *)&record->velX);
    } else {
        frame.pathEntryPtr = g_pCondorDataBin + 4;
        frame.pathEntryPtr = g_pCondorDataBin + (*(unsigned short *)frame.pathEntryPtr & 0xFFFF);
        frame.pathEntryPtr += (record->candidate >> 8) * 2;
        frame.pathEntryPtr = g_pCondorDataBin + (*(unsigned short *)frame.pathEntryPtr & 0xFFFF);
        frame.pathEntryPtr += (record->candidate & 0xFF) * 2;
        *(short *)&frame.segmentWord = *(unsigned short *)frame.pathEntryPtr & 0xFFFF;
        if ((frame.segmentWord & 0xFFFF & 0xFF) != 0xFF) {
            (record->candidate)++;
            record->targetX = (frame.segmentWord & 0xFFFF & 0xFF) << 4;
            frame.targetX = record->targetX;
            record->targetY = 0x400 - (((frame.segmentWord & 0xFFFF) >> 8 & 0xFF) << 4);
            frame.targetY = record->targetY;
            Mini_CondorBeginLinearMotion(record->posX, record->posY, frame.targetX, frame.targetY);
            record->velX = (short)g_CondorVecX;
            record->velY = (short)g_CondorVecY;
            record->angle = Mini_CondorVectorToAngle((CondorCursor *)&record->velX);
        } else {
            record->flags = record->flags & 0xFE;
            record->angle = 0x800;
        }
    }
    record->turnRate = 0;
    record->drawAngle = 0;
}

/*
 * Reads next command word from unit animation sequence script.
 */
// FUNCTION: FF7 0x00606d3c
short __cdecl Mini_CondorReadRecordSequenceWord(CondorRecord *record)
{
    unsigned short command;
    unsigned short operand;
    int addr;

    command = *(unsigned short *)&record->sequenceBase + *(unsigned short *)&record->sequenceOffset;
    addr = command + g_pCondorDataBin;
    command = *(short *)addr;
    if (command == 0xFFFF) {
        record->sequenceCommand = 0xFF;
        Mini_CondorIncrementSequenceCounter(record);
        return 0;
    } else {
        if (command == 0xFFFE) {
            record->sequenceOffset = 0;
            command = record->sequenceBase;
            addr = command + g_pCondorDataBin;
            command = *(short *)addr;
        }
        operand = command & 0xFF;
        if ((command & 0x80) != 0) {
            operand &= 0x7F;
            operand = 0;
        }
        record->operand = operand;
        record->sequenceCommand = command >> 8;
        if ((command & 0x80) == 0) {
            Mini_CondorIncrementSequenceCounter(record);
        }
        record->sequenceOffset += 2;
        return (short)command;
    }
}

/*
 * Initializes and begins unit animation sequence script.
 */
// FUNCTION: FF7 0x00606e6b
short __cdecl Mini_CondorBeginRecordSequence(CondorRecord *record)
{
    int addr;
    unsigned short base;

    record->operand = 0;
    record->sequenceOffset = 0;
    record->sequenceCommand = 0;
    record->inContact = 0;
    record->bufferIndex = 0;
    record->sequenceFlag = 0;
    addr = g_pCondorDataBin + 6;
    addr = (((*(unsigned short *)addr & 0xFFFF) + (record->unitIndex << 4)) + (g_pCondorDataBin + record->sequenceIndex * 2));
    base = *(short *)addr;
    record->sequenceBase = base;
    if ((base & 0xFF) == 0xFF) {
        return 0;
    } else {
        base = Mini_CondorReadRecordSequenceWord(record);
        return (short)base;
    }
}

/*
 * Tests unit placement validity against terrain and friendly units.
 */
// FUNCTION: FF7 0x00606f20
short __cdecl Mini_CondorTestRecordPlacementWithPeers(short typeIndex, CondorCursor *position)
{
    CondorTypeRecord *typeRecord;
    int recordBuf[30];
    int tableBase;
    CondorRecord *pProbe;
    short regionIndex;

    tableBase = g_pCondorDataBin;
    typeRecord = (CondorTypeRecord *)(g_pCondorDataBin + (*(unsigned short *)tableBase & 0xFFFF));
    typeRecord = (CondorTypeRecord *)((int)typeRecord + (typeIndex << 5));
    pProbe = (CondorRecord *)recordBuf;
    pProbe->posX = position->x;
    pProbe->posY = 0x400 - position->y;
    pProbe->posZ = (*(short *)&g_CondorScreenDistanceBuf);
    pProbe->worldX = position->x - 0x100;
    pProbe->worldY = 0x400 - position->y - 0x200;
    pProbe->worldZ = (*(short *)&g_CondorScreenDistanceBuf);
    pProbe->extentX = typeRecord->extentX;
    pProbe->extentY = typeRecord->extentY;
    Mini_CondorCountNearbyRecordsInRange(pProbe, 0, 0x27);
    if (g_CondorMatchCount != 0) {
        return 1;
    } else {
        regionIndex = Mini_CondorScanAcceptedRegions(pProbe);
        if (regionIndex == -1) {
            return 1;
        } else {
            return 0;
        }
    }
}

/*
 * Tests unit placement validity against terrain bounds.
 */
// FUNCTION: FF7 0x00607032
short __cdecl Mini_CondorTestRecordPlacement(short typeIndex, CondorCursor *position)
{
    CondorTypeRecord *typeRecord;
    int recordBuf[30];
    int tableBase;
    CondorRecord *pProbe;
    short regionIndex;

    tableBase = g_pCondorDataBin;
    typeRecord = (CondorTypeRecord *)(g_pCondorDataBin + (*(unsigned short *)tableBase & 0xFFFF));
    typeRecord = (CondorTypeRecord *)((int)typeRecord + (typeIndex << 5));
    pProbe = (CondorRecord *)recordBuf;
    pProbe->posX = position->x;
    pProbe->posY = 0x400 - position->y;
    pProbe->posZ = (*(short *)&g_CondorScreenDistanceBuf);
    pProbe->worldX = position->x - 0x100;
    pProbe->worldY = 0x400 - position->y - 0x200;
    pProbe->worldZ = (*(short *)&g_CondorScreenDistanceBuf);
    pProbe->extentX = typeRecord->extentX;
    pProbe->extentY = typeRecord->extentY;
    regionIndex = Mini_CondorScanAcceptedRegions(pProbe);
    if (regionIndex == -1) {
        return 1;
    } else {
        return 0;
    }
}

/*
 * Initializes unit record attributes from configuration table.
 */
// FUNCTION: FF7 0x00607123
short __cdecl Mini_CondorInitRecordFromConfig(CondorRecord *record, CondorConfigEntry *configEntry, int flags, void *pos)
{
    short level;
    CondorUnitTypeEntry *curSubEntry;
    short regionIndex;
    int base;

    base = g_pCondorDataBin;
    curSubEntry = (CondorUnitTypeEntry *)(g_pCondorDataBin + (*(unsigned short *)base & 0xFFFF));
    curSubEntry = (CondorUnitTypeEntry *)((int)curSubEntry + (*(unsigned char *)&configEntry->unitIndex << 5));
    level = flags & 0x7F;
    record->posX = ((CondorCursor *)pos)->x;
    record->posY = 0x400 - ((CondorCursor *)pos)->y;
    record->posZ = (*(short *)&g_CondorScreenDistanceBuf);
    record->worldX = ((CondorCursor *)pos)->x - 0x100;
    record->worldY = 0x400 - ((CondorCursor *)pos)->y - 0x200;
    record->worldZ = (*(short *)&g_CondorScreenDistanceBuf);
    record->extentX = curSubEntry->extentX;
    record->extentY = curSubEntry->extentY;
    record->rotX = 0;
    record->rotY = 0;
    record->rotZ = 0;
    if (level < 0x28 || (flags & 0x80) != 0) {
        Mini_CondorCountNearbyRecordsInRange(record, 0, 0x27);
        if (g_CondorMatchCount != 0) {
            return 1;
        }
        regionIndex = Mini_CondorScanAcceptedRegions(record);
        record->targetIndex = regionIndex;
        if (regionIndex == -1) {
            return 1;
        }
        Mini_CondorLoadRecordVertexTarget(record);
    }
    Mini_CondorComputeRecordStepIncrements(record);
    record->status = 1;
    record->state = 0;
    record->commandCode = 0;
    record->flags = 0;
    record->retired = 0;
    record->level = level;
    record->unitIndex = *(unsigned char *)&configEntry->unitIndex;
    record->initialValue = configEntry->initialValue;
    record->costOrArcVelY = curSubEntry->cost;
    record->counter = curSubEntry->counter;
    record->counterMax = curSubEntry->counter;
    record->scalePercent = curSubEntry->scalePercent;
    record->kind = curSubEntry->kind;
    record->attackRange = curSubEntry->rangeCells << 4;
    record->offsetY = curSubEntry->offsetY;
    record->unusedConfigByte = curSubEntry->unusedConfigByte;
    record->actionCountdown = curSubEntry->timerReload + level + 1;
    record->targetMode = configEntry[1].unitIndex;
    record->timer = 0;
    record->frameIndex = g_CondorUnitFrameIndex[record->unitIndex];
    record->cooldown = 0;
    record->timerReload = curSubEntry->timerReload;
    record->candidate = *(unsigned char *)&configEntry->candidate << 8;
    record->turnRate = rand() & 0x3FF;
    record->sequenceIndex = 0;
    Mini_CondorBeginRecordSequence(record);
    *(char *)&record->sequenceCommand += level & 7;
    record->cycleReload = curSubEntry->cycleReload;
    record->cyclePeriod = curSubEntry->cycleCount;
    record->cycleCountdown = *(unsigned char *)&curSubEntry->cycleCount;
    record->targetSlot = 0xFF;
    record->contactSlot = 0xFF;
    record->attackerSlot = *(char *)&record->unitIndex;
    Mini_CondorAdvanceRecordPathSegment(record);
    if (record->targetX == record->posX && record->targetY == record->posY) {
        Mini_CondorAdvanceRecordPathSegment(record);
    }
    if (((int)record - (int)&g_CondorRecordTable) / 0x78 >= 0x14) {
        record->costOrArcVelY = 0;
    }
    return 0;
}

/*
 * Initializes unit record attributes from byte configuration entry.
 */
// FUNCTION: FF7 0x0060747c
short __cdecl Mini_CondorInitRecordFromByteEntry(CondorRecord *record, CondorConfigSourceEntry *sourceEntry, int flags)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[8];
        CondorCursor posX;
    } frame;
#pragma pack(pop)

    g_pCondorConfigEntry->unitIndex = sourceEntry->unitType;
    g_pCondorConfigEntry->tileX = sourceEntry->tileX;
    g_pCondorConfigEntry->tileY = sourceEntry->tileY;
    g_pCondorConfigEntry->candidate = sourceEntry->candidate;
    g_pCondorConfigEntry->initialValue = sourceEntry->initialValue;
    g_pCondorConfigEntry[1].unitIndex = sourceEntry->targetMode;
    frame.posX.x = *(unsigned char *)&g_pCondorConfigEntry->tileX << 4;
    frame.posX.y = *(unsigned char *)&g_pCondorConfigEntry->tileY << 4;
    return Mini_CondorInitRecordFromConfig(record, g_pCondorConfigEntry, flags, &frame.posX);
}

/*
 * Finds first inactive unit record slot within specified index range.
 */
// FUNCTION: FF7 0x00607517
short __cdecl Mini_CondorFindInactiveRecordInRange(int startIndex, int endIndex)
{
    CondorRecord *record;
    int sentinel;
    int i;

    record = &g_CondorRecordTable.records[startIndex];
    for (i = startIndex; i < endIndex; i++) {
        if (record->status == 0) {
            return (short)i;
        }
        record++;
    }
    sentinel = 0xFFFF;
    return (short)sentinel;
}

/*
 * Preloads unit configuration records for current battle mode.
 */
// FUNCTION: FF7 0x00607570
void __cdecl Mini_CondorLoadRecordsForMode(void)
{
    CondorRecord *recordPtr;
    CondorActiveRecord *activeEnt;
    int headerAddr;
    CondorConfigSourceEntry *ent;

    if (g_CondorPhase == 0) {
        headerAddr = g_pCondorDataBin + 8;
        ent = (CondorConfigSourceEntry *)(g_pCondorDataBin + (*(unsigned short *)headerAddr & 0xFFFF));
        recordPtr = g_CondorRecordTable.records;
        while (*(unsigned char *)&ent->unitType != 0xFF) {
            if ((short)Mini_CondorInitRecordFromByteEntry(recordPtr, ent, g_CondorPlayerUnitCount) == 0) {
                Mini_CondorAdvanceTypeFrame(recordPtr);
                g_CondorPlayerUnitCount++;
                recordPtr++;
            }
            ent++;
        }
    }
    if (g_CondorPhase == 2) {
        recordPtr = g_CondorRecordTable.records;
        activeEnt = (CondorActiveRecord *)&g_CondorActiveRecords;
        while (activeEnt->posX != -1) {
            if ((short)Mini_CondorInitRecordFromShortEntry(recordPtr, (CondorShortEntry *)activeEnt, g_CondorPlayerUnitCount) == 0) {
                Mini_CondorAdvanceTypeFrame(recordPtr);
                g_CondorPlayerUnitCount++;
                recordPtr++;
            }
            activeEnt++;
        }
    }
    Mini_CondorBuildDigitDisplayRecords();
}

/*
 * Initializes unit record attributes from short configuration entry.
 */
// FUNCTION: FF7 0x0060767e
short __cdecl Mini_CondorInitRecordFromShortEntry(CondorRecord *record, CondorShortEntry *shortEntry, int flags)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[12];
        short initResult;
        struct { unsigned char pad_to_4[0x4]; CondorCursor posX; };
    } frame;
#pragma pack(pop)

    g_pCondorConfigEntry->unitIndex = shortEntry->unitIndex;
    g_pCondorConfigEntry->tileX = 0;
    g_pCondorConfigEntry->tileY = 0;
    g_pCondorConfigEntry->candidate = 0;
    g_pCondorConfigEntry->initialValue = 0;
    g_pCondorConfigEntry[1].unitIndex = 0;
    frame.posX.x = shortEntry->posX;
    frame.posX.y = shortEntry->posY;
    frame.initResult = Mini_CondorInitRecordFromConfig(record, g_pCondorConfigEntry, flags, &frame.posX);
    if (record->status != 0) {
        record->angle = shortEntry->angle;
        record->drawAngle = record->angle;
    }
    return (short)frame.initResult;
}

/*
 * Spawns scheduled enemy reinforcement unit onto battlefield.
 */
// FUNCTION: FF7 0x00607727
void __cdecl Mini_CondorSpawnScheduledRecord(void)
{
    CondorRecord *recordPtr;
    int seconds;
    CondorScheduledRecord *schedule;
    unsigned short slot;
    int offsetAddr;

    if (g_CondorPhase != 1) {
        offsetAddr = g_pCondorDataBin + g_CondorOutcomeIndex * 2 + 0xA;
        schedule = (CondorScheduledRecord *)(g_pCondorDataBin + (*(unsigned short *)offsetAddr & 0xFFFF));
        schedule = (CondorScheduledRecord *)((int)schedule + (*(int *)&g_CondorScheduledCount) * 8);
        if (schedule->unitType != 0xFF && g_CondorSpawnCount < 0x14) {
            g_CondorScheduleClock += g_CondorTickStep;
            seconds = g_CondorScheduleClock;
            if (seconds != 0) {
                seconds = seconds / 0x3C;
            }
            g_CondorScheduleSeconds = seconds;
            g_CondorNextSpawnTime = schedule->dueTime;
            if (*(unsigned short *)&schedule->dueTime <= seconds) {
                slot = Mini_CondorFindInactiveRecordInRange(0x14, 0x28);
                if (slot != 0xFFFF) {
                    recordPtr = &g_CondorRecordTable.records[slot];
                    if ((short)Mini_CondorInitRecordFromByteEntry(recordPtr, (CondorConfigSourceEntry *)schedule, slot) == 0) {
                        g_CondorSpawnCount++;
                        Mini_CondorBuildDigitDisplayRecords();
                        Mini_CondorAdvanceTypeFrame(recordPtr);
                        (*(int *)&g_CondorScheduledCount)++;
                    }
                }
            }
        }
    }
}
} /* extern "C" */
