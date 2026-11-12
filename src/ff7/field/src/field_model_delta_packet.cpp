// src/ff7/field/src/field_model_delta_packet.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Decodes and range-checks a signed delta packet.

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
 * Decodes three little-endian signed shorts from six bytes and validates them within [-255, 255].
 * If valid, applies the color delta to the container via Field_ApplyColorDeltaToContainer.
 */
// FUNCTION: FF7 0x0064eee0
int __cdecl Field_ApplyModelDeltaPacket(PolygonModel *container, FieldModelCommandRecord *deltaPacket)
{
    short red;
    short green;
    short blue;

    red = (*(unsigned char *)&deltaPacket->pHigh) << 8 & 0xFF00 | deltaPacket->command & 0xFF;
    green = deltaPacket->qHigh << 8 & 0xFF00 | deltaPacket->qLow & 0xFF;
    blue = deltaPacket->rHigh << 8 & 0xFF00 | deltaPacket->rLow & 0xFF;
    if (red > 0xFF || (green > 0xFF || (blue > 0xFF || (red < -255 || (green < -255 || blue < -255))))) {
        return 0;
    } else {
        Field_ApplyColorDeltaToContainer(container, (short)red, (short)green, (short)blue);
        return 1;
    }
}

} /* extern "C" */
