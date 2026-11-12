// src/ff7/field/src/field_model_matrix_record.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: A two-command editor for indexed model matrix records.

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
 * Two-command editor for model matrix records: FIELD_MODEL_MATRIX_RECORD_STORE loads the record from
 * the packet, while FIELD_MODEL_MATRIX_RECORD_APPLY repacks the record into a 30-byte buffer and
 * invokes Field_ApplyModelMatrixPacket.
 */
// FUNCTION: FF7 0x0064efc0
int __cdecl Field_ApplyIndexedModelMatrixRecord(PolygonModel *context, unsigned char *packet)
{
    int action;
    int recordIndex;
    FieldModelMatrix *record;
    char *scratchBuffer;

    action = packet[0];
    recordIndex = packet[1];
    record = (FieldModelMatrix *)(((int)g_FieldModelRuntimeRecords) + recordIndex * 0x3C);
    switch (action) {
    case FIELD_MODEL_MATRIX_RECORD_STORE:
        ((char *)record)[0] = packet[2];
        ((char *)record)[1] = packet[3];
        ((char *)record)[2] = packet[4];
        ((char *)record)[3] = packet[5];
        ((char *)record)[4] = packet[6];
        ((char *)record)[5] = packet[7];
        ((char *)record)[6] = packet[8];
        ((char *)record)[7] = packet[9];
        ((char *)record)[8] = packet[0xA];
        ((char *)record)[9] = packet[0xB];
        ((char *)record)[0xA] = packet[0xC];
        ((char *)record)[0xB] = packet[0xD];
        record->matrixElem0 = (short)((packet[0xF] << 8 & 0xFF00) | (packet[0xE] & 0xFF));
        record->matrixElem1 = (short)((packet[0x11] << 8 & 0xFF00) | (packet[0x10] & 0xFF));
        record->matrixElem2 = (short)((packet[0x13] << 8 & 0xFF00) | (packet[0x12] & 0xFF));
        record->matrixElem3 = (short)((packet[0x15] << 8 & 0xFF00) | (packet[0x14] & 0xFF));
        record->matrixElem4 = (short)((packet[0x17] << 8 & 0xFF00) | (packet[0x16] & 0xFF));
        record->matrixElem5 = (short)((packet[0x19] << 8 & 0xFF00) | (packet[0x18] & 0xFF));
        record->matrixElem6 = (short)((packet[0x1B] << 8 & 0xFF00) | (packet[0x1A] & 0xFF));
        record->matrixElem7 = (short)((packet[0x1D] << 8 & 0xFF00) | (packet[0x1C] & 0xFF));
        record->matrixElem8 = (short)((packet[0x1F] << 8 & 0xFF00) | (packet[0x1E] & 0xFF));
        return 1;
    case FIELD_MODEL_MATRIX_RECORD_APPLY:
        scratchBuffer = (char *)g_FieldModelRuntimeFlags;
        scratchBuffer[0] = ((char *)record)[0];
        scratchBuffer[1] = ((char *)record)[1];
        scratchBuffer[2] = ((char *)record)[2];
        scratchBuffer[3] = ((char *)record)[3];
        scratchBuffer[4] = ((char *)record)[4];
        scratchBuffer[5] = ((char *)record)[5];
        scratchBuffer[6] = ((char *)record)[6];
        scratchBuffer[7] = ((char *)record)[7];
        scratchBuffer[8] = ((char *)record)[8];
        scratchBuffer[9] = ((char *)record)[9];
        scratchBuffer[0xA] = ((char *)record)[0xA];
        scratchBuffer[0xB] = ((char *)record)[0xB];
        scratchBuffer[0xC] = (char)(record->matrixElem0 & 0xFF);
        scratchBuffer[0xD] = (char)(record->matrixElem0 >> 8 & 0xFF);
        scratchBuffer[0xE] = (char)(record->matrixElem1 & 0xFF);
        scratchBuffer[0xF] = (char)(record->matrixElem1 >> 8 & 0xFF);
        scratchBuffer[0x10] = (char)(record->matrixElem2 & 0xFF);
        scratchBuffer[0x11] = (char)(record->matrixElem2 >> 8 & 0xFF);
        scratchBuffer[0x12] = (char)(record->matrixElem3 & 0xFF);
        scratchBuffer[0x13] = (char)(record->matrixElem3 >> 8 & 0xFF);
        scratchBuffer[0x14] = (char)(record->matrixElem4 & 0xFF);
        scratchBuffer[0x15] = (char)(record->matrixElem4 >> 8 & 0xFF);
        scratchBuffer[0x16] = (char)(record->matrixElem5 & 0xFF);
        scratchBuffer[0x17] = (char)(record->matrixElem5 >> 8 & 0xFF);
        scratchBuffer[0x18] = (char)(record->matrixElem6 & 0xFF);
        scratchBuffer[0x19] = (char)(record->matrixElem6 >> 8 & 0xFF);
        scratchBuffer[0x1A] = (char)(record->matrixElem7 & 0xFF);
        scratchBuffer[0x1B] = (char)(record->matrixElem7 >> 8 & 0xFF);
        scratchBuffer[0x1C] = (char)(record->matrixElem8 & 0xFF);
        scratchBuffer[0x1D] = (char)(record->matrixElem8 >> 8 & 0xFF);
        Field_ApplyModelMatrixPacket(context, (FieldModelCommandRecord *)scratchBuffer);
        break;
    }
    return 0;
}
} /* extern "C" */
