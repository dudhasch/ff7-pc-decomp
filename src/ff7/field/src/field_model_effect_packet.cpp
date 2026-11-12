// src/ff7/field/src/field_model_effect_packet.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Decodes a model effect packet of signed shorts and passes the values to the model
//   diffuse selector effects.

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

#pragma intrinsic(memcpy)
#pragma function(memcpy)

/*
 * Decodes five signed shorts from a 10-byte effect packet and invokes model diffuse selector.
 */
// FUNCTION: FF7 0x0064e0c0
int __cdecl Field_ApplyModelEffectPacket(FieldModelInstance *modelRecord, FieldModelEffectPacket *effectPacket)
{
    short dstShortValue;
    int srcParam;
    short value0;
    short val1;
    short val2;

    value0 = *(unsigned char *)&effectPacket->word0Hi << 8 & 0xFF00 | *(unsigned char *)&effectPacket->word0Lo & 0xFF;
    val1 = *(unsigned char *)&effectPacket->word1Hi << 8 & 0xFF00 | *(unsigned char *)&effectPacket->word1Lo & 0xFF;
    val2 = *(unsigned char *)&effectPacket->word2Hi << 8 & 0xFF00 | *(unsigned char *)&effectPacket->word2Lo & 0xFF;
    dstShortValue = *(unsigned char *)&effectPacket->word3Hi << 8 & 0xFF00 | *(unsigned char *)&effectPacket->word3Lo & 0xFF;
    srcParam = (short)(effectPacket->word4Hi << 8 & 0xFF00 | *(unsigned char *)&effectPacket->word4Lo & 0xFF);
    Field_ApplyModelDiffuseSelectorEffect(modelRecord, (short)value0, (short)val1, (short)val2, (short)dstShortValue, srcParam);
    return 1;
}
} /* extern "C" */
