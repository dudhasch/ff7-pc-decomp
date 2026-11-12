// src/ff7/field/src/field_polygon_frame_animation.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Decodes polygon frame records, evaluates curve steps, and applies the polygon
//   animations to the model.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/types/field.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "ff7/api/field.h"
extern "C" {

#pragma intrinsic(abs)

#pragma intrinsic(pow)
#pragma intrinsic(sqrt)
// clang-format off
// GLOBAL: FF7 0x00d85130
int g_FieldCurveStepFlags = 0;
unsigned char g_bssPad_00d85134[4] = {0};
unsigned char g_bssPad_00d85138[64] = {0};
// clang-format on

#pragma intrinsic(memcpy)
#pragma function(memcpy)

/*
 * Decodes polygon frame records, evaluates curve steps, and applies polygon animations.
 */
// FUNCTION: FF7 0x0064e940
int __cdecl Field_ApplyPolygonRecordFrame(FieldModelInstance *modelRecord, FieldPolygonRampRecord *rampRecord, FieldModelCommandRecord *commandRecord, int slot, int mode, FieldCurveStepRecord *curveStep)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[28];
        int commandWord;
        struct { unsigned char pad_to_2[0x2]; short qWord; };
        struct { unsigned char pad_to_4[0x4]; int rWord; };
        struct { unsigned char pad_to_6[0x6]; short wordF06; };
        struct { unsigned char pad_to_8[0x8]; int rampIndex; };
        struct { unsigned char pad_to_C[0xC]; int lightCommandWord; };
        struct { unsigned char pad_to_10[0x10]; int lightRWord; };
        struct { unsigned char pad_to_14[0x14]; char byteF12; };
        struct { unsigned char pad_to_15[0x15]; char byteF10; };
        struct { unsigned char pad_to_16[0x16]; char byteF0E; };
        struct { unsigned char pad_to_18[0x18]; char byteF0C; };
        struct { unsigned char pad_to_19[0x19]; char byteF0A; };
        struct { unsigned char pad_to_1A[0x1A]; char byteF08; };
    } frame;
#pragma pack(pop)

    frame.byteF0E = commandRecord->param7High << 8 & 0xFF00 | commandRecord->param7Low & 0xFF;
    frame.byteF10 = commandRecord->param8High << 8 & 0xFF00 | commandRecord->param8Low & 0xFF;
    frame.byteF12 = commandRecord->param9High << 8 & 0xFF00 | commandRecord->param9Low & 0xFF;
    frame.byteF08 = commandRecord->param4High << 8 & 0xFF00 | commandRecord->param4Low & 0xFF;
    frame.byteF0A = commandRecord->param5High << 8 & 0xFF00 | commandRecord->param5Low & 0xFF;
    frame.byteF0C = commandRecord->param6High << 8 & 0xFF00 | commandRecord->param6Low & 0xFF;
    *(short *)&frame.commandWord = (*(unsigned char *)&commandRecord->pHigh) << 8 & 0xFF00 | commandRecord->command & 0xFF;
    frame.qWord = commandRecord->qHigh << 8 & 0xFF00 | commandRecord->qLow & 0xFF;
    *(short *)&frame.rWord = commandRecord->rHigh << 8 & 0xFF00 | commandRecord->rLow & 0xFF;
    frame.wordF06 = commandRecord->param3High << 8 & 0xFF00 | commandRecord->param3Low & 0xFF;
    frame.lightCommandWord = frame.commandWord;
    frame.lightRWord = frame.rWord;
    if (g_FieldCurveStepFlagTable[slot] != 0 && curveStep != 0) {
        Field_EvaluateCurveSteps(modelRecord, (FieldLightSource *)&frame.lightCommandWord, commandRecord, slot, curveStep);
        g_FieldCurveStepFlagTable[slot] = 0;
    }
    for (frame.rampIndex = 0; frame.rampIndex < rampRecord->count; frame.rampIndex++) {
        if (((FieldPolygonRampEntry *)rampRecord)[frame.rampIndex].active != 0) {
            Field_UpdateContainerLighting(modelRecord, (FieldLightSource *)&frame.lightCommandWord, commandRecord, slot, mode, 1);
        }
    }
    return 1;
}
} /* extern "C" */
