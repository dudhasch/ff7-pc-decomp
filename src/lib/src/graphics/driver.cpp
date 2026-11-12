// src/lib/src/graphics/driver.cpp
// Original: C:\lib\src\graphics\driver.cpp
// Contents: Allocates and frees the graphics driver operations structure.
/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/api/mem.h"
#include "lib/types/graphics.h"

#include "lib/api/graphics.h"
extern "C" {

/*
 * Frees a graphics driver ops structure.
 */
// FUNCTION: FF7 0x006a6740
void __cdecl GfxFreeDriverOps(GfxDriverOps *driverOps)
{
    if (driverOps != 0) {
        // STRING: FF7 0x009132b0
        GameHeapFree(driverOps, "C:\\lib\\src\\graphics\\driver.cpp", 10);
    }
}

/*
 * Allocates and zeroes a 0xF0-byte graphics driver ops structure.
 */
// FUNCTION: FF7 0x006a675e
GfxDriverOps *__cdecl GfxAllocDriverOps(void)
{
    GfxDriverOps *ops;

    // STRING: FF7 0x009132d0
    ops = (GfxDriverOps *)GameHeapAlloc(1, 240, "C:\\lib\\src\\graphics\\driver.cpp", 18);
    return ops;
}
} /* extern "C" */
