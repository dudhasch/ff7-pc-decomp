/* include/lib/types/thread.h: the types lib/thread TUs share. */
#ifndef LIB_TYPES_THREAD_H
#define LIB_TYPES_THREAD_H

struct MemBlockHeap;
struct ThreadRecord;

/* Thread manager made by ThreadSystemCreate: thread count, block heap, and thread slot array. */
typedef struct ThreadSystemManager {
    int activeThreads; /* +0x0 */
    int maxThreads;
    struct MemBlockHeap *threadHeap;
    struct ThreadRecord **threadSlots; /* maxThreads pointers, 0 for a free slot */
} ThreadSystemManager;

/* Thread init callback record written by ThreadInitCallbackRecord. */
typedef struct ThreadCallbackRecord {
    int kind;
    int stackSize;
    int callback;
} ThreadCallbackRecord;

/* Sixteen-byte thread creation template copied into the head of ThreadRecord. */
typedef struct ThreadTemplate {
    int kind;
    unsigned int stackSize;
    void (__cdecl *entryProc)(void);
    char *name;
} ThreadTemplate;

/* A thread record (0x24 bytes, ThreadCreateRecordFromTemplate copies the first 0x10 from a
 * template): kind 0 starts with _beginthread(ThreadRunEntryProc, stackSize, record), which calls
 * entryProc and sets state 2 while it runs and 3 when it returns (ThreadStartRecord sets 1).
 * ownsRecord is set when ThreadCreateRecordFromTemplate allocated the record, and
 * ThreadFreeOwnedRecord frees only such a record. */
typedef struct ThreadRecord {
    int kind; /* +0x00 */
    unsigned int stackSize; /* +0x04 */
    void (__cdecl *entryProc)(void); /* +0x08 */
    char *name; /* +0x0C */
    int pad_10;
    int state; /* +0x14: 0 new, 1 starting, 2 running, 3 finished */
    int ownsRecord; /* +0x18 */
    int handle; /* +0x1C: the _beginthread handle */
    int pad_20;
} ThreadRecord;

#endif /* LIB_TYPES_THREAD_H */
