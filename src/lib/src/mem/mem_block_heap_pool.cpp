// src/lib/src/mem/mem_block_heap_pool.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Block heaps: fixed-size block pools that are created, allocated from, freed into and
//   reset, chaining to overflow heaps when the base heap is full.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/types/mem.h"

#include "lib/api/mem.h"
#include "lib/api/graphics.h"
#include "lib/types/graphics.h"
#include "lib/enums/mem.h"
extern "C" {

/*
 * Creates a block heap by forwarding to CreateBlockHeapEx with null owners.
 */
// FUNCTION: FF7 0x006a5630
int __cdecl CreateBlockHeap(int blockSize, int blockCount, int depth)
{
    GfxSpriteBatch *owner;

    owner = 0;
    return CreateBlockHeapEx(blockSize, blockCount, depth, owner, 0);
}

/*
 * Creates a block heap sized for count elements of the given size.
 */
// FUNCTION: FF7 0x006a5659
int __cdecl CreateBlockHeapCount(int count, int size, int depth)
{
    GfxSpriteBatch *owner;

    owner = 0;
    return CreateBlockHeapEx(size, count * size, depth, owner, 0);
}

/*
 * Rewinds a block heap and its overflow chain without freeing memory.
 */
// FUNCTION: FF7 0x006a5686
void __cdecl ResetBlockHeapChain(MemBlockHeap *heap)
{
    if (heap != 0) {
        heap->allocated = 0;
        heap->cursor = heap->dataBase;
        heap->freeCount = 0;
        if (heap->childHeap != 0) {
            ResetBlockHeapChain((MemBlockHeap *)(heap->childHeap));
        }
    }
}

/*
 * Frees any chained overflow heaps and rewinds the base heap.
 */
// FUNCTION: FF7 0x006a56c7
void __cdecl TrimBlockHeapOverflow(MemBlockHeap *heap)
{
    if (heap != 0) {
        DestroyBlockHeapChain((MemBlockHeap *)heap->childHeap);
        heap->childHeap = 0;
        ResetBlockHeapChain(heap);
    }
}

/*
 * Allocates a single block from the heap or recurses into its overflow chain.
 */
// FUNCTION: FF7 0x006a56f5
int __cdecl AllocBlockFromHeap(MemBlockHeap *heap)
{
    int idx;
    int result;
    int childResult;
    int newBlock;

    result = 0;
    if (heap != 0) {
        if (heap->freeCount > 0) {
            (heap->freeCount)--;
            switch (heap->indexMode) {
            case MEM_BLOCK_INDEX_8:
                idx = *(unsigned char *)((char *)heap->freeList + heap->freeCount);
                break;
            case MEM_BLOCK_INDEX_16:
                idx = *(unsigned short *)((char *)heap->freeList + heap->freeCount * 2);
                break;
            case MEM_BLOCK_INDEX_32:
                idx = *(int *)((char *)heap->freeList + heap->freeCount * 4);
            }
            result = heap->dataBase + heap->blockSize * idx;
        } else if (heap->allocated < heap->blockTotal) {
            result = heap->cursor;
            heap->cursor += heap->blockSize;
            (heap->allocated)++;
        } else if (heap->childHeap != 0) {
            childResult = AllocBlockFromHeap((MemBlockHeap *)(heap->childHeap));
            result = childResult;
        } else {
            heap->childHeap = CreateBlockHeapEx(heap->blockSize, heap->dataSize, heap->depth + 1, (GfxSpriteBatch *)(heap->owner), heap->callback);
            newBlock = AllocBlockFromHeap((MemBlockHeap *)(heap->childHeap));
            result = newBlock;
        }
    }
    return result;
}

/*
 * Returns a block address to the heap chain by index or bump rewind.
 */
// FUNCTION: FF7 0x006a584a
int __cdecl FreeBlockToHeap(void *block, MemBlockHeap *heap)
{
    int result, offset, cursorOffset, index;
    result = 0;
    if (heap != 0) {
        offset = (int)block - heap->dataBase;
        if (offset >= 0 && offset < heap->dataSize) {
            cursorOffset = (int)block - heap->cursor;
            if (offset + heap->blockSize == cursorOffset) {
                heap->cursor -= heap->blockSize;
            } else {
                index = offset / heap->blockSize;
                switch (heap->indexMode) {
                case MEM_BLOCK_INDEX_8:
                    *(unsigned char *)(heap->freeList + heap->freeCount) = (unsigned char)index;
                    break;
                case MEM_BLOCK_INDEX_16:
                    ((unsigned short *)heap->freeList)[heap->freeCount] = (unsigned short)index;
                    break;
                case MEM_BLOCK_INDEX_32:
                    ((int *)heap->freeList)[heap->freeCount] = index;
                    break;
                }
                (heap->freeCount)++;
            }
            result = 1;
        } else
            return FreeBlockToHeap(block, (MemBlockHeap *)(heap->childHeap));
    }
    return result;
}

/*
 * Allocates an array of consecutive blocks from heap or overflow chain.
 */
// FUNCTION: FF7 0x006a594d
int __cdecl AllocBlockArrayFromHeap(int count, MemBlockArrayHeap *heap)
{
    GfxSortedPrimitive *result;
    GfxSortedPrimitive *childResult;
    GfxSortedPrimitive *newBlock;

    result = 0;
    if (heap != 0) {
        if (heap->usedCount + count <= heap->capacity) {
            result = (GfxSortedPrimitive *)(heap->cursor);
            heap->cursor += count * heap->blockSize;
            heap->usedCount += count;
        } else if (heap->blockHeapEx != 0) {
            childResult = (GfxSortedPrimitive *)(AllocBlockArrayFromHeap(count, (MemBlockArrayHeap *)(heap->blockHeapEx)));
            result = childResult;
        } else {
            if (count > heap->capacity) {
                heap->blockHeapEx = CreateBlockHeapEx(heap->blockSize, count * heap->blockSize, heap->depth + 1, (GfxSpriteBatch *)(heap->spriteBatch), heap->callback);
            } else {
                heap->blockHeapEx = CreateBlockHeapEx(heap->blockSize, heap->blockCount, heap->depth + 1, (GfxSpriteBatch *)(heap->spriteBatch), heap->callback);
            }
            newBlock = (GfxSortedPrimitive *)(AllocBlockArrayFromHeap(count, (MemBlockArrayHeap *)(heap->blockHeapEx)));
            result = newBlock;
        }
    }
    return (int)result;
}
} /* extern "C" */
