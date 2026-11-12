// src/ff7/chocobo/chocobo_heading_angles.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Heading helpers for the chocobo race: compute a heading angle from vector coordinates
//   or from the difference between two positions, using the arc tangent table.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/api/chocobo.h"
#include "ff7/types/chocobo.h"
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <mmsystem.h>
#include "ff7/gte.h"

extern "C" {

#pragma intrinsic(abs)


#pragma intrinsic(strcpy, strcat)

#pragma data_seg(".data$u77_lut")
#pragma bss_seg(".data$u77_lut")
#pragma data_seg()
#pragma bss_seg()

/* Returns the 12-bit heading angle of the vector (dx, dz), read from the arc tangent table. */
// FUNCTION: FF7 0x0077cf40
short __cdecl Mini_ChocoboHeadingFromXZ(short dx, short dz)
{
    if (dx > 0xFFF) {
        return 0x400;
    } else if (dx < (-4095)) {
        return 0xC00;
    } else {
        dx = (int)dx + 0x1000;
        dx &= 0x1FFF;
        if (dz > 0) {
            return g_ChocoboArcTanTable[dx] + 0x400 & 0xFFF;
        } else {
            return g_ChocoboArcTanTable[0x2000 - dx] + 0xC00 & 0xFFF;
        }
    }
}
/* Normalizes the vector from fromPos to toPos and takes its heading in the XZ plane. */
// FUNCTION: FF7 0x0077cfbf
void __cdecl Mini_ChocoboHeadingFromPositionDelta(ChocoboShortPointDepth *fromPos, ChocoboShortPointDepth *toPos)
{
    struct VECTOR dx;

    dx.vx = toPos->x - fromPos->x;
    dx.vy = toPos->y - fromPos->y;
    dx.vz = toPos->z - fromPos->z;
    NormalizeVectorFixed(&dx, &dx);
    Mini_ChocoboHeadingFromXZ((short)dx.vx, (short)dx.vz);
}
} /* extern "C" */
