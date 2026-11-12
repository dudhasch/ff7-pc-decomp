// src/ff7/field/src/field_model_record_overrides.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Applies model record overrides: resets the array reset flag, optionally clears all
//   record flags for one field, and traverses the model records.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/types/field.h"
#include "lib/types/polygon.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "ff7/api/field.h"
#include "lib/api/polygon.h"
extern "C" {

#pragma intrinsic(abs)

#pragma intrinsic(pow)
#pragma intrinsic(sqrt)

#pragma intrinsic(memcpy)
#pragma function(memcpy)

/*
 * Marks array reset flag, optionally clears all record flags for field 0x1CD, traverses the model
 * with Field_StoreZeroFlagCallback and applies byte-index/byte-value overrides when each index is below
 * record count.
 */
// FUNCTION: FF7 0x0064eb50
int __cdecl Field_ApplyModelRecordOverrides(FieldModelInstance *modelRecord, FieldModelFlagTable *flagTable, FieldModelCommandRecord *overrideRecord)
{
    FieldModelFlagTable *flagBase;
    int overrideCnt;
    PolygonModel *drawState;
    int loopIndex;
    int entIndex;
    int flagVal;

    flagBase = flagTable;
    drawState = (PolygonModel *)modelRecord->skeleton;
    flagTable->isOverridden = 1;
    if (g_FieldCurrentMapId == 0x1CD) {
        for (loopIndex = 0; loopIndex < flagTable->count; loopIndex++) {
            ((FieldModelOverrideRecord *)flagTable)[loopIndex].overrideFlag = 0;
        }
    }
    Gfx_ForEachModelPolygonSet((int)flagBase, (GfxPolygonSetFn)Field_StoreZeroFlagCallback, drawState);
    overrideCnt = overrideRecord->command;
    if (overrideCnt > 0) {
        for (loopIndex = 0; loopIndex < overrideCnt; loopIndex++) {
            entIndex = *(unsigned char *)((char *)overrideRecord + loopIndex * 2 + 1);
            if (entIndex >= flagTable->count) {
                continue;
            }
            flagVal = *(unsigned char *)((char *)overrideRecord + loopIndex * 2 + 2);
            ((FieldModelOverrideRecord *)flagTable)[entIndex].overrideFlag = flagVal;
        }
    }
    flagTable->isOverridden = 1;
    Gfx_ForEachModelPolygonSet((int)flagBase, (GfxPolygonSetFn)Field_TraverseModelFlagRecord, drawState);
    return 1;
}
} /* extern "C" */
