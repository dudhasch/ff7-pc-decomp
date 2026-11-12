// src/ff7/snobo/memory.cpp
// Original: C:\FF7\snobo\memory.cpp

/* C++ TU (retail .cpp): the game's global operator new and operator delete. */
#include "lib/api/mem.h"

/*
 * The global operator new of the snowboard, highway and coaster code (their `new` expressions,
 * buffers and text buffer): allocates one zero-filled block of `size` bytes from the game heap
 * (GameHeapAlloc, calloc-style), tagged with source line 6 for the heap tracker, and returns
 * the block, or 0 when the heap refuses.
 */
// FUNCTION: FF7 0x0065ec20
void *__cdecl operator new(unsigned int size)
{
    // STRING: FF7 0x0090a808
    return GameHeapAlloc(1, size, "C:\\FF7\\snobo\\memory.cpp", 6);
}

/*
 * The global operator delete: gives a block from operator new back to the game heap
 * (GameHeapFree), tagged with source line 11 for the heap tracker. The snowboard, highway and
 * coaster code use it for their `delete`s and buffer releases; the pointer is passed on without
 * a null test.
 */
// FUNCTION: FF7 0x0065ec3a
void __cdecl operator delete(void *block)
{
    // STRING: FF7 0x0090a820
    GameHeapFree(block, "C:\\FF7\\snobo\\memory.cpp", 11);
}
