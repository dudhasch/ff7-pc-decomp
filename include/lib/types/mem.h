/* include/lib/types/mem.h: the types lib/mem TUs share.
 * One definition per type. */
#ifndef LIB_TYPES_MEM_H
#define LIB_TYPES_MEM_H

/* Fixed-size block heap: child heap link, block size and count, cursor and free list. */
typedef struct MemBlockHeap {
    int childHeap;
    int depth;
    int reservedWord;
    int dataSize;
    int dataBase;
    int dataAlloc;
    int blockSize;
    int allocated;
    int blockTotal;
    int cursor;
    int indexMode;
    int freeCount;
    int freeList;
    int owner;
    int callback;
} MemBlockHeap;

/* Block-array heap descriptor: nesting depth, block size, used and capacity counts, and cursor. */
typedef struct MemBlockArrayHeap {
    int blockHeapEx;
    int depth;
    unsigned char pad_08[0x4];
    int blockCount;
    unsigned char pad_10[0x8];
    int blockSize;
    int usedCount;
    int capacity;
    int cursor;
    unsigned char pad_28[0xc];
    int spriteBatch;
    int callback;
} MemBlockArrayHeap;

/* Game heap block record (GameHeapAlloc): element count and size, source line and file, the data
   pointer and the raw block it lies in. */
typedef struct MemHeapBlock {
    int count;
    int elementSize;
    int line;
    int file;
    int data;
    int allocBase;
} MemHeapBlock;

/* A 64-bit value split into two dwords, low first: the timestamp counter ReadTimestampCounter
 * reads (RDTSC), and what Add64/Sub64/Compare64 and CombineSplitValueToFloat take. Compare64
 * orders by the signed high dword, then the unsigned low one. */
typedef struct Timestamp64 {
    unsigned int lo;
    int hi;
} Timestamp64;


/* Heap list node with a next link and the record it holds. */
typedef struct MemHeapNode {
    int next;
    int record;
} MemHeapNode;

/* Game heap tracker: enable flag and list head pointer used by the heap alloc and free paths. */
typedef struct GameHeapTracker {
    int enabled;
    int listHead;
} GameHeapTracker;

/* Heap tracker with an enable flag and a block list that DumpGameHeapBlocks walks. */
typedef struct MemHeapTracker {
    int enabled;
    int blockList;
} MemHeapTracker;


#endif /* LIB_TYPES_MEM_H */
