// src/ff7/field/src/field_make_point.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Constructs a 3D point by storing its x, y and z components.

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
 * Stores x, y and z into the three components of the point.
 */
// FUNCTION: FF7 0x00649b30
void __cdecl Field_MakePoint(FieldVec3i *point, int x, int y, int z)
{
    point->x = x;
    point->y = y;
    point->z = z;
}
} /* extern "C" */
