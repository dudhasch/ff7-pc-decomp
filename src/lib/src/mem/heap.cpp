// src/lib/src/mem/heap.cpp
// Original: C:\lib\src\mem\heap.cpp

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/api/mem.h"
#include "lib/types/graphics.h"
#include "lib/types/mem.h"
extern "C" {

/*
 * Recursively destroys a chain of block heaps and their allocated buffers.
 */
// FUNCTION: FF7 0x006a5440
void __cdecl DestroyBlockHeapChain(MemBlockHeap *heap)
{
    if (heap != 0) {
        DestroyBlockHeapChain((MemBlockHeap *)(heap->childHeap));
        // STRING: FF7 0x00912f90
        GameHeapFree((void *)heap->freeList, "C:\\lib\\src\\mem\\heap.cpp", 0x11);
        // STRING: FF7 0x00912fa8
        GameHeapFree((void *)heap->dataAlloc, "C:\\lib\\src\\mem\\heap.cpp", 0x12);
        // STRING: FF7 0x00912fc0
        GameHeapFree(heap, "C:\\lib\\src\\mem\\heap.cpp", 0x13);
    }
}

/*
 * Creates an extended block heap with configured element size, capacity, and owner callback.
 */
// FUNCTION: FF7 0x006a5498
int __cdecl CreateBlockHeapEx(int blockSize, int blockCount, int depth, GfxSpriteBatch *owner, int callback)
{
    MemBlockHeap *result;
    int data;
    result = 0;
    if (blockSize > 0 && blockCount > 0) {
        // STRING: FF7 0x00912fd8
        result = (MemBlockHeap *)GameHeapAlloc(1, 0x3c, "C:\\lib\\src\\mem\\heap.cpp", 0x1e);
        if (result != 0) {
            result->depth = depth;
            result->owner = (int)owner;
            result->callback = callback;
            result->reservedWord = 0;
            result->dataSize = blockCount;
            // STRING: FF7 0x00912ff0
            result->dataAlloc = (int)GameHeapAlloc(blockCount + 0x20, 1, "C:\\lib\\src\\mem\\heap.cpp", 0x28);
            if (result->dataAlloc != 0) {
                data = result->dataAlloc;
                result->dataBase = (data + 0x1f) & ~0x1f;
                result->blockSize = blockSize;
                result->blockTotal = blockCount / blockSize;
                result->cursor = result->dataBase;
                if (result->blockTotal <= 0x100) {
                    result->indexMode = 0;
                    // STRING: FF7 0x00913008
                    result->freeList = (int)GameHeapAlloc(result->blockTotal, 1, "C:\\lib\\src\\mem\\heap.cpp", 0x3b);
                } else if (result->blockTotal <= 0x10000) {
                    result->indexMode = 1;
                    // STRING: FF7 0x00913020
                    result->freeList = (int)GameHeapAlloc(result->blockTotal, 2, "C:\\lib\\src\\mem\\heap.cpp", 0x42);
                } else {
                    result->indexMode = 2;
                    // STRING: FF7 0x00913038
                    result->freeList = (int)GameHeapAlloc(result->blockTotal, 4, "C:\\lib\\src\\mem\\heap.cpp", 0x47);
                }
                if (callback != 0)
                    /* Invokes untyped callback slot `callback` with its runtime (int, int) -> void signature. */
                    ((void(__cdecl *)(int, int))callback)((int)owner, (int)result);
            } else {
                DestroyBlockHeapChain(result);
                result = 0;
            }
        }
    }
    return (int)result;
}
} /* extern "C" */
