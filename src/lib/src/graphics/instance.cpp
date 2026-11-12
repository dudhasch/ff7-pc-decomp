// src/lib/src/graphics/instance.cpp
// Original: C:\lib\src\graphics\instance.cpp
// Contents: Manages draw allocation pools: creating and destroying them, allocating draw list nodes
//   and recycling them as the pool generation advances, and initializing a polygon set state
//   record.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/api/graphics.h"
#include "lib/api/mem.h"
#include "lib/types/graphics.h"
#include "lib/types/mem.h"
#include <string.h>
extern "C" {

/*
 * Initializes a polygon set state management record.
 */
// FUNCTION: FF7 0x0068f7a0
void __cdecl Gfx_InitPolygonSetStateRecord(void *record)
{
    ZeroFillMemory32(0x18, record);
    *(int *)((char *)record + 8) = 2;
}

/*
 * Destroys a draw allocation pool and frees its node chunks.
 */
// FUNCTION: FF7 0x0068f7bd
void __cdecl Gfx_DestroyDrawPool(int pool)
{
    if (pool != 0) {
        DestroyBlockHeapChain((MemBlockHeap *)(*(int *)((char *)pool + 4)));
        // STRING: FF7 0x00910a1c
        GameHeapFree((void *)pool, "C:\\lib\\src\\graphics\\instance.cpp", 0x15);
    }
}

/*
 * Allocates and initializes a new draw pool node container.
 */
// FUNCTION: FF7 0x0068f7ea
int __cdecl Gfx_CreateDrawPool(int nodeCount)
{
    GfxDrawPool *pool;

    // STRING: FF7 0x00910a40
    pool = (GfxDrawPool *)GameHeapAlloc(1, 8, "C:\\lib\\src\\graphics\\instance.cpp", 0x1D);
    if (pool != 0) {
        pool->generation = 1;
        pool->blockHeap = CreateBlockHeap(0x90, nodeCount * 0x90, 0);
    }
    return (int)pool;
}
//
// Instance-pool allocation and transform helpers used by dx_sfx.cpp and polygon rendering.

#pragma intrinsic(strcmp)

/*
 * Advances active pool generation and recycles draw list nodes.
 */
// FUNCTION: FF7 0x0068f839
void __cdecl Gfx_AdvanceDrawPoolGeneration(int pool)
{
    if (pool != 0) {
        (*(int *)pool)++;
        ResetBlockHeapChain((MemBlockHeap *)(*(int *)((char *)pool + 4)));
    }
}

/*
 * Allocates a new draw node from pool chunks or heap.
 */
// FUNCTION: FF7 0x0068f860
int __cdecl Gfx_AllocDrawPoolListNode(GfxDrawPoolList *list, GfxDrawPoolDesc *pool)
{
    GfxDrawPoolNode *node;
    GfxDrawPoolNode *block;

    node = 0;
    if (list != 0) {
        if ((unsigned int)list->highWater < (unsigned int)pool->highWaterMark) {
            list->active = 1;
            list->head = 0;
            list->count = 0;
            list->highWater = pool->highWaterMark;
        }
        block = (GfxDrawPoolNode *)(AllocBlockFromHeap((MemBlockHeap *)(pool->heap)));
        node = block;
        if (node != 0) {
            node->batchBlock = 0;
            if (list->head != 0) {
                node->nextNode = list->head;
                list->head = (int)node;
            } else {
                node->nextNode = 0;
                list->head = (int)node;
            }
            (list->count)++;
        }
    }
    return (int)node;
}

/*
 * Recycles head node of an active draw pool linked list.
 */
// FUNCTION: FF7 0x0068f91b
void __cdecl Gfx_FreeDrawPoolListHeadNode(GfxDrawPoolList *list, GfxDrawPool *pool)
{
    GfxRenderObject *node;

    if (list != 0 && pool != 0) {
        list->active = 0;
        list->highWater = 0;
        node = (GfxRenderObject *)(list->head);
        FreeBlockToHeap(node, (MemBlockHeap *)(pool->blockHeap));
    }
}
} /* extern "C" */
