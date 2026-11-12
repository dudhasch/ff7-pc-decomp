/* include/lib/enums/mem.h: named codes of lib/mem and the TUs that use them.
 * Enums only, so any TU can include it: no types, no declarations. */
#ifndef LIB_ENUMS_MEM_H
#define LIB_ENUMS_MEM_H

/* MemBlockHeap::indexMode: the width of a free-list entry, which CreateBlockHeapEx picks from the
 * heap's block count (at most 0x100, at most 0x10000, more). */
enum MemBlockIndexWidth {
    MEM_BLOCK_INDEX_8 = 0,
    MEM_BLOCK_INDEX_16 = 1,
    MEM_BLOCK_INDEX_32 = 2
};

#endif
