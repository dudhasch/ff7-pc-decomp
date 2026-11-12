// src/ff7/highway/highway_field_list_draw.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: HighwayFieldList::draw, which draws the enabled players of a field list.

#include <string.h>
#include "ff7/api/snobo.h"
#include "ff7/highway_palette.h"
#include "lib/codegen.h"
#include "ff7/highway_psxdata.h"
#include <math.h>
#include <stdlib.h>
#include "ff7/types/highway.h"
#include "lib/types/file.h"
#include "lib/types/polygon.h"
#include "lib/types/mem.h"
#include "lib/types/graphics.h"
#include "ff7/api/highway.h"
#include "lib/api/mem.h"
#include "lib/api/polygon.h"
#include "lib/pfile.h"

#pragma intrinsic(strlen, strcpy, strcat, strcmp)
struct FileOpenRequest;
struct PolygonAnimation;

/*
 * Walks count entries of stride 0x18 in items and calls their draw(value) member only when their
 * player is non-null and enabled.
 */
// FUNCTION: FF7 0x0065c8a0
void HighwayFieldList::draw(int value)
{
    int i;
    HighwayPlayerState *playerPtr;

    for (i = 0; i < count; i++) {
        playerPtr = ((HighwayFieldItem *)(items + i * 0x18))->player;
        if (playerPtr != 0 && playerPtr->isEnabled != 0) {
            ((HighwayFieldItem *)(items + i * 0x18))->draw(value);
        }
    }
}
