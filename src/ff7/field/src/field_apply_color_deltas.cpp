// src/ff7/field/src/field_apply_color_deltas.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Adds RGB colour deltas, saturated to the byte range, to every vertex of a polygon set
//   or to every polygon set in a two-level container.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/types/graphics.h"
#include "ff7/types/field.h"
#include "lib/types/polygon.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "ff7/api/field.h"
#include "lib/api/graphics.h"
extern "C" {

#pragma intrinsic(abs)

#pragma intrinsic(pow)
#pragma intrinsic(sqrt)

#pragma intrinsic(memcpy)
#pragma function(memcpy)

/*
 * Traverses a two-level container and applies a color delta to every polygon set leaf.
 */
// FUNCTION: FF7 0x0064f860
int __cdecl Field_ApplyColorDeltaToContainer(PolygonModel *container, short red, short green, short blue)
{
    PolygonSetGroup *group;
    GfxPolygonSet *pPolygonSet;
    int index;
    PolygonSetEntry *pairPtr;
    int groupIndex;

    if (container != 0) {
        group = (PolygonSetGroup *)container->polySets;
        for (groupIndex = 0; groupIndex < container->polySetCount; groupIndex++) {
            if (group->entryCount > 0) {
                pairPtr = (PolygonSetEntry *)group->entries;
                if (pairPtr != 0) {
                    for (index = 0; index < group->entryCount; index++) {
                        if (pairPtr->modelRef != 0) {
                            pPolygonSet = (GfxPolygonSet *)pairPtr->modelRef->polySet;
                            Field_ApplyColorDeltaToPolygonSet((short)red, (short)green, (short)blue, pPolygonSet);
                        }
                        pairPtr++;
                    }
                }
            }
            group++;
        }
    }
    return 1;
}

/*
 * Shifts RGB color components into byte lanes and applies them to every vertex
 * of the specified polygon set.
 */
// FUNCTION: FF7 0x0064f91d
void __cdecl Field_ApplyColorDeltaToPolygonSet(short red, short green, short blue, GfxPolygonSet *polySet)
{
    GfxEngineContext *ctx;
    int greenPart;
    GfxPolygonSetEntry *group;
    int blueVal;
    int redBits;
    int vertexIdx;
    int entry;
    int index;
    int vertexRecord;

    redBits = red << 0x10;
    greenPart = green << 8;
    blueVal = blue;
    if (polySet != 0) {
        ctx = GetEngineContext();
        for (vertexIdx = 0; vertexIdx < polySet->groupCount; vertexIdx++) {
            group = (GfxPolygonSetEntry *)(Gfx_LockPolygonSetGroup(vertexIdx, polySet, ctx));
            if (group != 0) {
                vertexRecord = group->vertices;
                entry = group->lightRecords;
                if (vertexRecord != 0 && entry != 0) {
                    for (index = 0; index < group->elementCount; index++) {
                        Field_AddBgraColorSaturated(redBits, greenPart, blueVal, entry + 0x14, vertexRecord + 0x10);
                        vertexRecord += 0x20;
                        entry += 0x18;
                    }
                }
                Gfx_UnlockPolygonSetGroup(vertexIdx, polySet, ctx);
            }
        }
    }
}

/*
 * Performs a saturating per-channel addition of RGB deltas onto a packed BGRA color dword.
 */
// FUNCTION: FF7 0x0064fa20
void __cdecl Field_AddBgraColorSaturated(int redLane, int greenLane, int blueLane, int srcPtr, int dstPtr)
{
    int color;
    int total;

    color = *(int *)srcPtr;
    total = (color & 0xFF0000) + redLane;
    if (total > 0xFFFFFF) {
        color |= 0xFF0000;
    } else if (total < 0) {
        color &= 0xFF00FFFF;
    } else {
        color = color & 0xFF00FFFF | total & 0xFF0000;
    }
    total = (color & 0xFF00) + greenLane;
    if (total > 0xFFFF) {
        color |= 0xFF00;
    } else if (total < 0) {
        color &= 0xFFFF00FF;
    } else {
        color = color & 0xFFFF00FF | total & 0xFF00;
    }
    total = (color & 0xFF) + blueLane;
    if (total > 0xFF) {
        color |= 0xFF;
    } else if (total < 0) {
        color &= 0xFFFFFF00;
    } else {
        color = color & 0xFFFFFF00 | total & 0xFF;
    }
    *(int *)dstPtr = color;
}
} /* extern "C" */
