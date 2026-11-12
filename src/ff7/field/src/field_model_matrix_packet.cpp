// src/ff7/field/src/field_model_matrix_packet.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Decodes control bytes and two 3-by-3 matrices from a parameter block into a model's
//   matrix records.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/types/field.h"
#include "lib/types/polygon.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "lib/api/graphics.h"
#include "ff7/api/field.h"
#include "lib/api/polygon.h"
extern "C" {

#pragma intrinsic(abs)

#pragma intrinsic(pow)
#pragma intrinsic(sqrt)

#pragma intrinsic(memcpy)
#pragma function(memcpy)

/*
 * Decodes three leading control bytes and two 3-by-3 matrices from a 30-byte parameter block,
 * converts both to transposed float matrices, and visits every model polygon set with
 * Field_LightPolygonSetCallbackSwapped.
 */
// FUNCTION: FF7 0x0064ec60
int __cdecl Field_ApplyModelMatrixPacket(PolygonModel *lightContext, FieldModelCommandRecord *matrixPacket)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[1520];
        int setListPtr;
        struct { unsigned char pad_to_4[0x4]; int setCount; };
        struct { unsigned char pad_to_38[0x38]; char qLowByte; };
        struct { unsigned char pad_to_39[0x39]; char pHighByte; };
        struct { unsigned char pad_to_3A[0x3A]; char commandByte; };
        struct { unsigned char pad_to_3C[0x3C]; GfxMatrix4x4 matrixB; };
        struct { unsigned char pad_to_7C[0x7C]; GfxMatrix4x4 matrixA; };
        struct { unsigned char pad_to_5B0[0x5B0]; PsxMatrix psxMatrixB; };
        struct { unsigned char pad_to_5D0[0x5D0]; PsxMatrix psxMatrixA; };
    } frame;
#pragma pack(pop)

    frame.commandByte = (*(char *)&matrixPacket->command);
    frame.pHighByte = matrixPacket->pHigh;
    frame.qLowByte = (*(char *)&matrixPacket->qLow);
    frame.psxMatrixA.m[0][0] = matrixPacket->qHigh << 4;
    frame.psxMatrixA.m[1][0] = matrixPacket->rLow << 4;
    frame.psxMatrixA.m[2][0] = matrixPacket->rHigh << 4;
    frame.psxMatrixA.m[0][1] = matrixPacket->param3Low << 4;
    frame.psxMatrixA.m[1][1] = matrixPacket->param3High << 4;
    frame.psxMatrixA.m[2][1] = matrixPacket->param4Low << 4;
    frame.psxMatrixA.m[0][2] = matrixPacket->param4High << 4;
    frame.psxMatrixA.m[1][2] = matrixPacket->param5Low << 4;
    frame.psxMatrixA.m[2][2] = matrixPacket->param5High << 4;
    frame.psxMatrixA.t[2] = 0;
    frame.psxMatrixA.t[1] = frame.psxMatrixA.t[2];
    frame.psxMatrixA.t[0] = frame.psxMatrixA.t[1];
    GtePsxMatrixToFloat4x4Transposed(&frame.psxMatrixA, &frame.matrixA);
    frame.psxMatrixB.m[0][0] = matrixPacket->param6High << 8 & 0xFF00 | matrixPacket->param6Low & 0xFF;
    frame.psxMatrixB.m[0][1] = matrixPacket->param7High << 8 & 0xFF00 | matrixPacket->param7Low & 0xFF;
    frame.psxMatrixB.m[0][2] = matrixPacket->param8High << 8 & 0xFF00 | matrixPacket->param8Low & 0xFF;
    frame.psxMatrixB.m[1][0] = matrixPacket->param9High << 8 & 0xFF00 | matrixPacket->param9Low & 0xFF;
    frame.psxMatrixB.m[1][1] = matrixPacket->param10High << 8 & 0xFF00 | matrixPacket->param10Low & 0xFF;
    frame.psxMatrixB.m[1][2] = matrixPacket->param11High << 8 & 0xFF00 | matrixPacket->param11Low & 0xFF;
    frame.psxMatrixB.m[2][0] = matrixPacket->param12High << 8 & 0xFF00 | matrixPacket->param12Low & 0xFF;
    frame.psxMatrixB.m[2][1] = matrixPacket->param13High << 8 & 0xFF00 | matrixPacket->param13Low & 0xFF;
    frame.psxMatrixB.m[2][2] = matrixPacket->param14High << 8 & 0xFF00 | matrixPacket->param14Low & 0xFF;
    frame.psxMatrixB.t[2] = 0;
    frame.psxMatrixB.t[1] = frame.psxMatrixB.t[2];
    frame.psxMatrixB.t[0] = frame.psxMatrixB.t[1];
    GtePsxMatrixToFloat4x4Transposed(&frame.psxMatrixB, &frame.matrixB);
    frame.setCount = 1;
    frame.setListPtr = (int)&frame.setCount;
    Gfx_ForEachModelPolygonSet(frame.setListPtr, (GfxPolygonSetFn)Field_LightPolygonSetCallbackSwapped, lightContext);
    return 1;
}
} /* extern "C" */
