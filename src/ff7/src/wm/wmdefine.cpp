// src/ff7/src/wm/wmdefine.cpp
// Original: C:\FF7\src\wm\wmdefine.cpp
//
// The world map's single memory arena. One 0xE7801-byte block is taken at
// startup and eight pointers are planted into it at fixed displacements; the
// free path zeroes them again. The two `__FILE__` literals are separate copies
// in the original, identical,
// because Visual C++ 5.0 pools duplicate literals only under /Gf, which
// this image was not built with.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/api/mem.h"

#include "ff7/api/wm.h"
extern "C" {
// GLOBAL: FF7 0x00e2d5e0
unsigned char *g_pWorldArenaRegion0 = 0;
// GLOBAL: FF7 0x00e2d5e4
unsigned char *g_pWorldArenaRegion1 = 0;
// GLOBAL: FF7 0x00e2d5e8
unsigned char *g_pWorldArenaRegion2 = 0;
// GLOBAL: FF7 0x00e2d5ec
unsigned char *g_pWorldArenaRegion3 = 0;
// GLOBAL: FF7 0x00e2d5f0
unsigned char *g_pWorldArenaRegion4 = 0;
// GLOBAL: FF7 0x00e2d5f4
unsigned char *g_pWorldArenaRegion5 = 0;
// GLOBAL: FF7 0x00e2d5f8
unsigned char *g_pWorldArenaRegion6 = 0;
// GLOBAL: FF7 0x00e2d5fc
unsigned char *g_pWorldArenaRegion7 = 0;
// GLOBAL: FF7 0x00e2d600
unsigned char *gWorldArena = 0;
unsigned char g_bssPad_00e2d604[4] = {0};

/*
 * Allocates the world map's 0xE7801-byte arena and carves it into the eight region pointers
 * at fixed offsets.
 */
// FUNCTION: FF7 0x0075ab50
void __cdecl World_AllocArena(void)
{
    // STRING: FF7 0x0096d55c
    gWorldArena = (unsigned char *)GameHeapAllocRaw(948225, "C:\\FF7\\src\\wm\\wmdefine.cpp", 31);
    if (gWorldArena != 0) {
        g_pWorldArenaRegion0 = gWorldArena;
        g_pWorldArenaRegion1 = gWorldArena + 0x79000;
        g_pWorldArenaRegion2 = gWorldArena;
        g_pWorldArenaRegion3 = gWorldArena + 0x23800;
        g_pWorldArenaRegion4 = gWorldArena + 0x33600;
        g_pWorldArenaRegion5 = gWorldArena + 0x38C00;
        g_pWorldArenaRegion6 = gWorldArena + 0x3F000;
        g_pWorldArenaRegion7 = gWorldArena + 0xA6800;
    }
}

/*
 * Frees the world arena and clears all eight region pointers.
 */
// FUNCTION: FF7 0x0075abf3
void __cdecl World_FreeArena(void)
{
    if (gWorldArena != 0) {
        // STRING: FF7 0x0096d578
        GameHeapFree(gWorldArena, "C:\\FF7\\src\\wm\\wmdefine.cpp", 52);
        gWorldArena = 0;
        // Chained assignments, right to left: the original stores 0 into g_pWorldArenaRegion6
        // and then copies it back out of memory into each pointer in turn, which is
        // what `a = b = c = 0` compiles to at /Od.
        g_pWorldArenaRegion0 = g_pWorldArenaRegion1 = g_pWorldArenaRegion3 = g_pWorldArenaRegion4 = g_pWorldArenaRegion6 = 0;
        g_pWorldArenaRegion2 = g_pWorldArenaRegion5 = 0;
        g_pWorldArenaRegion7 = 0;
    }
}
} /* extern "C" */
