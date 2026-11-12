// src/ff7/field/src/field_model_ramp_delta.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Drives interpolator records that ramp a field model's value toward a target, read from
//   a packet by command.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/types/field.h"
#include "lib/types/polygon.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "ff7/api/field.h"
#include "ff7/enums/field.h"
extern "C" {

#pragma intrinsic(abs)

#pragma intrinsic(pow)
#pragma intrinsic(sqrt)

#pragma intrinsic(memcpy)
#pragma function(memcpy)

/*
 * Two-command driver for interpolator records: FIELD_RAMP_COMMAND_LOAD loads the record from the
 * packet, while FIELD_RAMP_COMMAND_STEP packs the coordinates into a buffer, invokes
 * Field_ApplyModelDeltaPacket, and steps coordinates toward their targets until clamped.
 */
// FUNCTION: FF7 0x0064f420
int __cdecl Field_RampModelDeltaToTarget(PolygonModel *model, unsigned char *packet)
{
    FieldRampModelDelta *recordPtr;
    int command;
    int slot;
    int reached;
    unsigned char *buffer;

    command = packet[0];
    slot = packet[1];
    recordPtr = (FieldRampModelDelta *)(((int)g_FieldModelRuntimeRecords) + slot * 0x3C);
    switch (command) {
    case FIELD_RAMP_COMMAND_LOAD:
        recordPtr->currentA = (short)((packet[3] << 8 & 0xFF00) | (packet[2] & 0xFF));
        recordPtr->currentB = (short)((packet[5] << 8 & 0xFF00) | (packet[4] & 0xFF));
        recordPtr->currentC = (short)((packet[7] << 8 & 0xFF00) | (packet[6] & 0xFF));
        recordPtr->targetA = (short)((packet[9] << 8 & 0xFF00) | (packet[8] & 0xFF));
        recordPtr->targetB = (short)((packet[0xB] << 8 & 0xFF00) | (packet[0xA] & 0xFF));
        recordPtr->targetC = (short)((packet[0xD] << 8 & 0xFF00) | (packet[0xC] & 0xFF));
        recordPtr->rateA = (short)((packet[0xF] << 8 & 0xFF00) | (packet[0xE] & 0xFF));
        recordPtr->rateB = (short)((packet[0x11] << 8 & 0xFF00) | (packet[0x10] & 0xFF));
        recordPtr->rateC = (short)((packet[0x13] << 8 & 0xFF00) | (packet[0x12] & 0xFF));
        if (recordPtr->targetA < 0 && recordPtr->rateA > 0) {
            recordPtr->rateA *= -1;
        }
        if (recordPtr->targetB < 0 && recordPtr->rateB > 0) {
            recordPtr->rateB *= -1;
        }
        if (recordPtr->targetC < 0 && recordPtr->rateC > 0) {
            recordPtr->rateC *= -1;
        }
        recordPtr->deltaParam = packet[0x14];
        recordPtr->finished = 0;
        return 1;
    case FIELD_RAMP_COMMAND_STEP:
        reached = 0;
        buffer = (unsigned char *)g_FieldModelRuntimeFlags;
        buffer[0] = (unsigned char)(recordPtr->currentA & 0xFF);
        buffer[1] = (unsigned char)(recordPtr->currentA >> 8 & 0xFF);
        buffer[2] = (unsigned char)(recordPtr->currentB & 0xFF);
        buffer[3] = (unsigned char)(recordPtr->currentB >> 8 & 0xFF);
        buffer[4] = (unsigned char)(recordPtr->currentC & 0xFF);
        buffer[5] = (unsigned char)(recordPtr->currentC >> 8 & 0xFF);
        buffer[6] = recordPtr->deltaParam;
        Field_ApplyModelDeltaPacket(model, (FieldModelCommandRecord *)buffer);
        if (recordPtr->finished == 0) {
            recordPtr->currentA += recordPtr->rateA;
            if ((recordPtr->rateA >= 0 && recordPtr->currentA >= recordPtr->targetA) ||
                (recordPtr->rateA < 0 && recordPtr->currentA <= recordPtr->targetA)) {
                recordPtr->currentA = recordPtr->targetA;
                reached |= 1;
            }
            recordPtr->currentB += recordPtr->rateB;
            if ((recordPtr->rateB >= 0 && recordPtr->currentB >= recordPtr->targetB) ||
                (recordPtr->rateB < 0 && recordPtr->currentB <= recordPtr->targetB)) {
                recordPtr->currentB = recordPtr->targetB;
                reached |= 2;
            }
            recordPtr->currentC += recordPtr->rateC;
            if ((recordPtr->rateC >= 0 && recordPtr->currentC >= recordPtr->targetC) ||
                (recordPtr->rateC < 0 && recordPtr->currentC <= recordPtr->targetC)) {
                recordPtr->currentC = recordPtr->targetC;
                reached |= 4;
            }
            if (reached == 7) {
                recordPtr->finished += 1;
            }
            return 0;
        }
        return 1;
    }
    return 1;
}
} /* extern "C" */
