// src/ff7/field/src/field_run_polygon.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Initializes or updates the per-entity polygon ramp and lighting effects in the field.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/types/field.h"
#include "lib/types/polygon.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "ff7/api/field.h"
#include "lib/api/graphics.h"
#include "lib/api/polygon.h"
#include "ff7/enums/field.h"
extern "C" {

#pragma intrinsic(abs)

#pragma intrinsic(pow)
#pragma intrinsic(sqrt)

#pragma intrinsic(memcpy)
#pragma function(memcpy)

/*
 * Initializes (flags->active clear) or updates (set) per-entity polygon ramp and lighting effects.
 * The update applies the current ramp position, then steps the FieldPolygonRampPhase cycle.
 */
// FUNCTION: FF7 0x0064e1b0
int __cdecl Field_RunPolygonRampCycle(FieldModelInstance *model, FieldPolygonRampRecord *ramp, int mode, int delay, FieldRuntimeFlags *flags)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[72];
        FieldModelCommandRecord *commandRecord;
        struct { unsigned char pad_to_4[0x4]; short rateScale; };
        struct { unsigned char pad_to_8[0x8]; int rampDelta; };
        struct { unsigned char pad_to_C[0xC]; int polySetRoot; };
        struct { unsigned char pad_to_10[0x10]; FieldPolygonRampRecord *rampSet; };
        struct { unsigned char pad_to_14[0x14]; int dataBlock; };
        struct { unsigned char pad_to_18[0x18]; int commandP; };
        struct { unsigned char pad_to_1C[0x1C]; int commandQ; };
        struct { unsigned char pad_to_20[0x20]; int commandR; };
        struct { unsigned char pad_to_28[0x28]; int rampIndex; };
        struct { unsigned char pad_to_2C[0x2C]; int runMode; };
        struct { unsigned char pad_to_30[0x30]; int modelIndex; };
        struct { unsigned char pad_to_34[0x34]; FieldModelRuntimeRecord *runtimeRecord; };
        struct { unsigned char pad_to_38[0x38]; FieldCurveStepRecord curveStep; };
    } frame;
#pragma pack(pop)

    frame.runMode = *(unsigned char *)&flags->active;
    frame.modelIndex = *(unsigned char *)&flags->modelIndex;
    frame.runtimeRecord = (FieldModelRuntimeRecord *)(frame.modelIndex * 0x3C + (char *)&g_FieldModelRuntimeRecords);
    if (((int *)&g_FieldModelCommandGate)[frame.modelIndex] != 0) {
        frame.runMode = 0;
    }
    switch (frame.runMode) {
    case FIELD_RAMP_COMMAND_LOAD:
        for (frame.rampIndex = 0; frame.rampIndex < ramp->count; frame.rampIndex++) {
            ((FieldPolygonRampEntry *)ramp)[frame.rampIndex].active = 0;
        }
        frame.runtimeRecord->rampPos = 0;
        frame.runtimeRecord->rampCounter = 0;
        frame.runtimeRecord->rampPhase = FIELD_POLYGON_RAMP_WAIT;
        frame.runtimeRecord->animFrame = 0;
        frame.runtimeRecord->animTick = 0;
        frame.runtimeRecord->effectFlags = 0;
        frame.runtimeRecord->rampStep = 16;
        frame.runtimeRecord->rampLimit = 100;
        frame.runtimeRecord->rampRate = 0x800;
        frame.runtimeRecord->curveShiftX = 4;
        frame.runtimeRecord->curveShiftY = 4;
        frame.runtimeRecord->curveShiftZ = 5;
        frame.runtimeRecord->curveShiftBy = 0;
        frame.runtimeRecord->curveOffset = 0;
        if (ramp->count == 1) {
            if (ramp->kind0 == 0x36) {
                frame.runtimeRecord->rampStep = 32;
                frame.runtimeRecord->rampLimit = 16;
                frame.runtimeRecord->rampRate = 0x400;
                frame.runtimeRecord->curveShiftZ = 9;
            } else if (ramp->kind0 == 0x23) {
                frame.runtimeRecord->rampRate = 0x200;
                frame.runtimeRecord->curveShiftX = 7;
                frame.runtimeRecord->curveShiftY = 7;
                frame.runtimeRecord->curveShiftZ = 6;
                frame.runtimeRecord->curveOffset = 0x10;
            } else if ((ramp->kind0 == 0x70 && ramp->subKind0 == 0x40) ||
                       (ramp->kind0 == 0x6D && ramp->subKind0 == 0x40) ||
                       (ramp->kind0 == 0x5E && ramp->subKind0 == 0x40) ||
                       (ramp->kind0 == 0x5F && ramp->subKind0 == 0x3E)) {
                frame.runtimeRecord->rampRate = 0xE0;
                frame.runtimeRecord->curveShiftX = 6;
                frame.runtimeRecord->curveShiftY = 6;
                frame.runtimeRecord->curveShiftZ = 7;
            } else if (ramp->kind0 == 0x48 && ramp->subKind0 == 0x8C) {
                frame.runtimeRecord->rampStep = 32;
            } else if (ramp->kind0 == 0x50 && ramp->subKind0 == 0x4C) {
                frame.runtimeRecord->rampStep = 12;
                frame.runtimeRecord->rampRate = 0x400;
                frame.runtimeRecord->curveShiftX = 7;
                frame.runtimeRecord->curveShiftY = 7;
                frame.runtimeRecord->curveShiftZ = 7;
            }
        } else if (ramp->count == 4) {
            if (ramp->kind0 == 0x23 && ramp->kind1 == 0x24) {
                frame.runtimeRecord->rampStep = 24;
                frame.runtimeRecord->rampRate = 0x400;
                frame.runtimeRecord->curveShiftX = 5;
                frame.runtimeRecord->curveShiftY = 5;
                frame.runtimeRecord->curveShiftZ = 6;
                frame.runtimeRecord->curveOffset = 0x28;
            } else if (ramp->kind0 == 0x10 && ramp->kind1 == 0x2A) {
                frame.runtimeRecord->rampStep = 16;
                frame.runtimeRecord->rampRate = 0x400;
                frame.runtimeRecord->curveShiftX = 6;
                frame.runtimeRecord->curveShiftY = 6;
                frame.runtimeRecord->curveShiftZ = 5;
                frame.runtimeRecord->curveOffset = 0x10;
            } else if (ramp->kind0 == 0x24 && ramp->kind1 == 0x24) {
                ((int *)&g_FieldModelCommandGate)[frame.modelIndex] = 0;
                return 0;
            }
        } else if (ramp->count == 2 || ramp->count == 3) {
            frame.runtimeRecord->rampStep = 24;
            frame.runtimeRecord->rampRate = 0x400;
            frame.runtimeRecord->curveShiftX = 5;
            frame.runtimeRecord->curveShiftY = 5;
            frame.runtimeRecord->curveShiftZ = 5;
            frame.runtimeRecord->curveOffset = 0x10;
        }
        ((int *)&g_FieldModelCommandGate)[frame.modelIndex] = 0;
        return 1;
    case FIELD_RAMP_COMMAND_STEP:
        frame.commandRecord = (FieldModelCommandRecord *)&g_FieldModelRuntimeFlags;
        frame.dataBlock = Gfx_GetPsxScratchpadPtr(0);
        frame.rateScale = g_pFieldModelSlots[frame.modelIndex].scale > 0x200 ? g_pFieldModelSlots[frame.modelIndex].scale / 512 : 1;
        frame.rampDelta = frame.runtimeRecord->rampRate * frame.rateScale;
        frame.commandRecord->command = frame.commandP & 0xFF;
        frame.commandRecord->pHigh = frame.commandP >> 8 & 0xFF;
        frame.commandRecord->qLow = frame.commandQ & 0xFF;
        frame.commandRecord->qHigh = frame.commandQ >> 8 & 0xFF;
        frame.commandRecord->rLow = frame.commandR & 0xFF;
        frame.commandRecord->rHigh = frame.commandR >> 8 & 0xFF;
        frame.commandRecord->param3Low = frame.rampDelta & 0xFF;
        frame.commandRecord->param3High = frame.rampDelta >> 8 & 0xFF;
        frame.commandRecord->param7Low = 0xC0;
        frame.commandRecord->param7High = 0;
        frame.commandRecord->param8Low = 0xC0;
        frame.commandRecord->param8High = 0;
        frame.commandRecord->param9Low = 0xC0;
        frame.commandRecord->param9High = 0;
        frame.commandRecord->param4Low = 0xFF;
        frame.commandRecord->param4High = 0;
        frame.commandRecord->param5Low = 0xFF;
        frame.commandRecord->param5High = 0;
        frame.commandRecord->param6Low = 0xFF;
        frame.commandRecord->param6High = 0;
        ramp->restartTraversal = 1;
        frame.rampSet = ramp;
        Gfx_ForEachModelPolygonSet((int)frame.rampSet, (GfxPolygonSetFn)Field_TraverseModelFlagRecord, (PolygonModel *)model->skeleton);
        if (ramp->count == 4) {
            if (ramp->kind0 == 0x23 && ramp->kind1 == 0x24) {
                ramp->active3 = 0;
                ramp->active0 = 0;
            } else if (ramp->kind0 == 0x10 && ramp->kind1 == 0x2A) {
                ramp->active2 = 0;
                ramp->active0 = 0;
            } else if (ramp->kind0 == 0x24 && ramp->kind1 == 0x24) {
                ramp->active1 = 0;
                ramp->active0 = 0;
            }
        }
        if (frame.runtimeRecord->rampStep <= 0) {
            frame.runtimeRecord->rampStep = 16;
        }
        frame.curveStep.magnitude = frame.rateScale;
        frame.curveStep.shiftX = frame.runtimeRecord->curveShiftX;
        frame.curveStep.shiftY = frame.runtimeRecord->curveShiftY;
        frame.curveStep.shiftZ = frame.runtimeRecord->curveShiftZ;
        frame.curveStep.shiftBy = frame.runtimeRecord->curveShiftBy;
        frame.curveStep.offset = frame.runtimeRecord->curveOffset;
        frame.curveStep.phaseDelta = frame.runtimeRecord->rampStep;
        Field_ApplyPolygonRecordFrame(model, ramp, frame.commandRecord, frame.modelIndex, frame.runtimeRecord->rampPos / frame.runtimeRecord->rampStep, &frame.curveStep);
        Gfx_ForEachModelPolygonSet(frame.polySetRoot, (GfxPolygonSetFn)Field_WriteTrueFlag, (PolygonModel *)model->skeleton);
        switch (frame.runtimeRecord->rampPhase) {
        case FIELD_POLYGON_RAMP_WAIT:
            (frame.runtimeRecord->rampCounter)++;
            if (frame.runtimeRecord->rampCounter >= frame.runtimeRecord->rampLimit) {
                frame.runtimeRecord->rampPos = 0;
                frame.runtimeRecord->animFrame = 256;
                frame.runtimeRecord->animTick = 256;
                frame.runtimeRecord->effectFlags = 0x100;
                frame.runtimeRecord->rampPhase = FIELD_POLYGON_RAMP_SWEEP;
            }
            break;
        case FIELD_POLYGON_RAMP_SWEEP:
            frame.runtimeRecord->rampPos += frame.runtimeRecord->rampStep;
            if (frame.runtimeRecord->rampPos >= 288) {
                frame.runtimeRecord->rampCounter = 0;
                frame.runtimeRecord->animFrame = 0;
                frame.runtimeRecord->animTick = 0;
                frame.runtimeRecord->effectFlags = 0;
                frame.runtimeRecord->rampPhase = FIELD_POLYGON_RAMP_PAUSE;
            }
            break;
        case FIELD_POLYGON_RAMP_PAUSE:
            (frame.runtimeRecord->rampCounter)++;
            if (frame.runtimeRecord->rampCounter >= 10) {
                frame.runtimeRecord->rampPos = 0;
                frame.runtimeRecord->animFrame = 256;
                frame.runtimeRecord->animTick = 256;
                frame.runtimeRecord->effectFlags = 0x100;
                frame.runtimeRecord->rampPhase = FIELD_POLYGON_RAMP_SWEEP_AGAIN;
            }
            break;
        case FIELD_POLYGON_RAMP_SWEEP_AGAIN:
            frame.runtimeRecord->rampPos += frame.runtimeRecord->rampStep;
            if (frame.runtimeRecord->rampPos >= 288) {
                frame.runtimeRecord->rampCounter = 0;
                frame.runtimeRecord->animFrame = 0;
                frame.runtimeRecord->animTick = 0;
                frame.runtimeRecord->effectFlags = 0;
                frame.runtimeRecord->rampPhase = FIELD_POLYGON_RAMP_WAIT;
            }
        }
    default:
        return 0;
    }
}
} /* extern "C" */
