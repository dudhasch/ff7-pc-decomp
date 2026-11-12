// src/lib/src/mem/mem.cpp
// Original: C:\lib\src\mem\mem.cpp
// Contents: Game heap: allocation and free, optional tracking of outstanding blocks through a
//   tracker object, leak and usage reports, and string duplication.
/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/types/graphics.h"
#include "lib/types/list.h"
#include "lib/types/mem.h"
#include <stdarg.h>
#include <math.h>
#include <stdlib.h>
#include "lib/api/list.h"
#include "lib/api/mem.h"
#include "lib/api/graphics.h"
#include "lib/crt.h"
#include <string.h>

#pragma intrinsic(sin, cos)
#pragma intrinsic(memset, memcpy)
extern "C" {

// clang-format off
// The heap tracker (an enabled word, then its block list) is the 8-byte storage object g_MemHeapTrackerStorage.
// The extern stays above the marker: between them it binds the marker.
// GLOBAL: FF7 0x0090a838
GameHeapTracker * g_MemHeapTracker = &g_MemHeapTrackerStorage;
// Heap tracker block referenced by mem.cpp (g_MemHeapTrackerStorage), located at the end
// of this TU's .bss span before psx.c.
struct GameHeapTracker g_MemHeapTrackerStorage = {0};
// clang-format on


#pragma intrinsic(strlen)


// The literals up to PrintGameHeapBlockCallback's are parse-time units: retail emits the callback's format string
// (0x90A998) before DumpGameHeapBlocks's literals while its code follows DumpGameHeapBlocks, and VC5 emits a
// function's literals in function order. Each helper returns one literal and is defined before the first function;
// under /Ob1 its call is the plain push.
// CreateGameHeapTracker's __FILE__ (GameHeapMemDup pushes its own copy).
// STRING: FF7 0x0090a83c
static __inline char *Mem_TrackerFileName(void) { return "C:\\lib\\src\\mem\\mem.cpp"; }
// GameHeapFree's guard-header report.
// STRING: FF7 0x0090a854
static __inline char *Mem_HeaderCorruptedFormat(void) { return "MEMORY ERROR: MEMORY HEADER CORRUPTED %d %d :MEMORY POINTER %p IN FILE %s LINE %d \n"; }
// GameHeapFree's guard-footer report.
// STRING: FF7 0x0090a8a8
static __inline char *Mem_FooterCorruptedFormat(void) { return "MEMORY ERROR: MEMORY FOOTER CORRUPTED %d %d :MEMORY POINTER %p IN FILE %s LINE %d \n"; }
// GameHeapFree's report of a pointer the tracker does not hold.
// STRING: FF7 0x0090a8fc
static __inline char *Mem_UnallocatedFreeFormat(void) { return "MEMORY ERROR: TRIED TO FREE UNALLOCATED MEMORY POINTER %p IN FILE %s LINE %d \n"; }
// GameHeapReportOOM's report.
// STRING: FF7 0x0090a94c
static __inline char *Mem_OutOfMemoryFormat(void) { return "MEMORY ERROR: COULD NOT ALLOCATE MEMORY IN FILE %s LINE %d SIZE %d * %d \n"; }
// PrintGameHeapBlockCallback's line per block.
// STRING: FF7 0x0090a998
static __inline char *Mem_BlockFormat(void) { return "  MEMORY BLOCK: FILE %s LINE %d PTR 0x%p SIZE %d * %d \n"; }

/*
 * Destroys a heap tracker: destroys and nulls its block list, then frees the tracker itself. A
 * null tracker is accepted.
 */
// FUNCTION: FF7 0x0065fac0
void __cdecl DestroyGameHeapTracker(GameHeapTracker *tracker)
{
    if (tracker != 0) {
        DestroyListAndNull((List **)&tracker->listHead);
        free(tracker);
    }
}

/*
 * Allocates the eight-byte game-heap tracker and switches it on by setting its enabled flag.
 * Returns 0 if the heap refused.
 */
// FUNCTION: FF7 0x0065fae6
GameHeapTracker *__cdecl CreateGameHeapTracker(void)
{
    GameHeapTracker *rec;

    rec = (GameHeapTracker *)(GameHeapAlloc(1, 8, Mem_TrackerFileName(), 0x28));
    if (rec != 0) {
        rec->enabled = 1;
    }
    return rec;
}

/*
 * Writes value into the tracker's enabled flag. Silent on a null tracker; the
 * argument order is (value, tracker), not the other way round.
 */
// FUNCTION: FF7 0x0065fb16
void __cdecl SetGameHeapTrackerEnabledOn(int value, GameHeapTracker *tracker)
{
    if (tracker != 0) {
        tracker->enabled = value;
    }
}

/*
 * Frees one heap tracking record through the CRT's free -- not through the game heap, because the
 * record describes the game heap and cannot live in it.
 */
// FUNCTION: FF7 0x0065fb29
void __cdecl FreeGameHeapTrackRecord(void *record)
{
    if (record != 0) {
        free(record);
    }
}

/*
 * Frees a game-heap block. With the tracker on, finds the block's record, checks the 16 guard
 * bytes before and after it (0x4D, 0x4E, ...) and logs the first one that differs, frees the
 * allocation and drops the record; a block the tracker does not know is logged as freed
 * unallocated. With the tracker off it is the CRT's free. A null block does nothing.
 */
// FUNCTION: FF7 0x0065fb40
void __cdecl GameHeapFree(void *block, char *file, int line)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[232];
        int byteIndex;
        struct { unsigned char pad_to_4[0x4]; int allocBase; };
        struct { unsigned char pad_to_8[0x8]; int headerPtr; };
        struct { unsigned char pad_to_C[0xC]; int footerPtr; };
        struct { unsigned char pad_to_10[0x10]; MemHeapBlock *heapBlock; };
        struct { unsigned char pad_to_14[0x14]; MemHeapNode *node; };
        struct { unsigned char pad_to_18[0x18]; int found; };
        struct { unsigned char pad_to_1C[0x1C]; int listHead; };
        struct { unsigned char pad_to_20[0x20]; char msgBuf[200]; };
    } frame;
#pragma pack(pop)

    if (block != 0) {
        if (g_MemHeapTracker != 0 && g_MemHeapTracker->enabled != 0) {
            if (g_MemHeapTracker->listHead != 0) {
                frame.found = 0;
                frame.listHead = g_MemHeapTracker->listHead;
                frame.node = (MemHeapNode *)(*(int *)((char *)frame.listHead + 0xC));
                while (frame.node != 0) {
                    frame.heapBlock = (MemHeapBlock *)(frame.node->record);
                    if (frame.heapBlock->data == (int)block) {
                        frame.allocBase = frame.heapBlock->allocBase;
                        frame.headerPtr = frame.allocBase;
                        frame.footerPtr = frame.allocBase + frame.heapBlock->count * frame.heapBlock->elementSize + 0x10;
                        for (frame.byteIndex = 0; frame.byteIndex < 0x10; frame.byteIndex++) {
                            if (*(unsigned char *)(frame.headerPtr + frame.byteIndex) != frame.byteIndex + 0x4D) {
                                sprintf(frame.msgBuf, Mem_HeaderCorruptedFormat(), frame.byteIndex, *(unsigned char *)(frame.headerPtr + frame.byteIndex), block, file, line);
                                Gfx_DebugPrint(frame.msgBuf);
                                break;
                            }
                            if (*(unsigned char *)(frame.footerPtr + frame.byteIndex) != frame.byteIndex + 0x4D) {
                                sprintf(frame.msgBuf, Mem_FooterCorruptedFormat(), frame.byteIndex, *(unsigned char *)(frame.footerPtr + frame.byteIndex), block, file, line);
                                Gfx_DebugPrint(frame.msgBuf);
                                break;
                            }
                        }
                        free((void *)(frame.heapBlock->allocBase));
                        RemoveFromList((int)(frame.node->record), (List *)(g_MemHeapTracker->listHead));
                        frame.found = 1;
                        break;
                    }
                    frame.node = (MemHeapNode *)(frame.node->next);
                }
                if (frame.found == 0) {
                    sprintf(frame.msgBuf, Mem_UnallocatedFreeFormat(), block, file, line);
                    Gfx_DebugPrint(frame.msgBuf);
                }
            }
        } else {
            free(block);
        }
    }
}

/*
 * Allocates size bytes, not cleared. With the tracker on, allocates 0x20 more, fills the 16 guard
 * bytes before and after the block with 0x4D, 0x4E, ..., records the block (size, file, line)
 * in the tracker's list and returns the address after the leading guard. Reports a failed
 * allocation through GameHeapReportOOM and returns 0.
 */
// FUNCTION: FF7 0x0065fda1
void *__cdecl GameHeapAllocRaw(int size, char *file, int line)
{
    int result;
    MemHeapBlock *record;
    int footerPtr;
    MemHeapBlock *entry;
    int rawBase;
    int hdr;
    int k;

    if (g_MemHeapTracker != 0 && g_MemHeapTracker->enabled != 0) {
        result = (int)malloc(size + 0x20);
        if (result != 0) {
            if (g_MemHeapTracker->listHead == 0) {
                g_MemHeapTracker->listHead = (int)CreateListCrtHeap((ListFreeValueFn)FreeGameHeapTrackRecord);
            }
            record = (MemHeapBlock *)malloc(0x18);
            entry = record;
            if (entry != 0) {
                rawBase = result;
                entry->count = 1;
                entry->elementSize = size;
                entry->line = line;
                entry->file = (int)file;
                entry->data = rawBase + 0x10;
                entry->allocBase = result;
                hdr = rawBase;
                footerPtr = rawBase + entry->count * entry->elementSize + 0x10;
                for (k = 0; k < 0x10; k++) {
                    *(char *)(hdr + k) = k + 0x4D;
                    *(char *)(footerPtr + k) = k + 0x4D;
                }
                result = entry->data;
                ListAppendValue((int)record, (List *)(g_MemHeapTracker->listHead));
            }
        } else {
            GameHeapReportOOM(1, size, file, line);
        }
    } else {
        result = (int)malloc(size);
        if (result == 0) {
            GameHeapReportOOM(1, size, file, line);
        }
    }
    return (void *)result;
}

/*
 * Logs "MEMORY ERROR: COULD NOT ALLOCATE MEMORY IN FILE %s LINE %d SIZE %d * %d".
 */
// FUNCTION: FF7 0x0065ff19
void __cdecl GameHeapReportOOM(int count, int size, char *file, int line)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[200];
        char msgBuf[200];
    } frame;
#pragma pack(pop)

    sprintf(frame.msgBuf, Mem_OutOfMemoryFormat(), file, line, count, size);
    Gfx_DebugPrint(frame.msgBuf);
}

/*
 * Allocates count * size bytes, cleared (calloc). With the tracker on it adds the 16-byte guards
 * and the tracking record as GameHeapAllocRaw does. Reports a failed allocation through
 * GameHeapReportOOM and returns 0.
 */
// FUNCTION: FF7 0x0065ff59
void *__cdecl GameHeapAlloc(int count, int size, char *file, int line)
{
    int result;
    int totalSize;
    MemHeapBlock *record;
    int footerPtr;
    MemHeapBlock *entry;
    int rawBase;
    int hdr;
    int k;

    if (g_MemHeapTracker != 0 && g_MemHeapTracker->enabled != 0) {
        totalSize = count * size + 0x20;
        result = (int)malloc(totalSize);
        if (result != 0) {
            ZeroFillMemory32(totalSize, (void *)(result));
            if (g_MemHeapTracker->listHead == 0) {
                g_MemHeapTracker->listHead = (int)CreateListCrtHeap((ListFreeValueFn)FreeGameHeapTrackRecord);
            }
            record = (MemHeapBlock *)malloc(0x18);
            entry = record;
            if (entry != 0) {
                rawBase = result;
                entry->count = count;
                entry->elementSize = size;
                entry->line = line;
                entry->file = (int)file;
                entry->data = rawBase + 0x10;
                entry->allocBase = result;
                hdr = rawBase;
                footerPtr = rawBase + entry->count * entry->elementSize + 0x10;
                for (k = 0; k < 0x10; k++) {
                    *(char *)(hdr + k) = k + 0x4D;
                    *(char *)(footerPtr + k) = k + 0x4D;
                }
                result = entry->data;
                ListAppendValue((int)record, (List *)(g_MemHeapTracker->listHead));
            }
        } else {
            GameHeapReportOOM(count, size, file, line);
        }
    } else {
        result = (int)calloc(count, size);
        if (result == 0) {
            GameHeapReportOOM(count, size, file, line);
        }
    }
    return (void *)result;
}

/*
 * Clears size bytes at dst using the compiler's dword-plus-byte intrinsic path.
 */
// FUNCTION: FF7 0x006600f2
void __cdecl ZeroFillMemory32(unsigned int size, void *dst)
{
    memset(dst, 0, size);
}

/*
 * Logs every block the tracker still holds (file, line, pointer, size) between "HEAP START" and
 * "HEAP END", and the total as "HEAP USAGE". Does nothing without an enabled tracker.
 */
// FUNCTION: FF7 0x0066010f
void __cdecl DumpGameHeapBlocks(MemHeapTracker *tracker)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[204];
        char msgBuf[200];
        struct { unsigned char pad_to_C8[0xC8]; int totalBytes; };
    } frame;
#pragma pack(pop)

    frame.totalBytes = 0;
    if (tracker != 0 && tracker->enabled != 0 && tracker->blockList != 0) {
        // STRING: FF7 0x0090a9d0
        Gfx_DebugPrint("HEAP START \n");
        ForEachListEntry((ListVisitFn)PrintGameHeapBlockCallback, (int)(&frame.totalBytes), (List *)tracker->blockList);
        // STRING: FF7 0x0090a9e0
        sprintf(frame.msgBuf, "  HEAP USAGE %d \n", frame.totalBytes);
        Gfx_DebugPrint(frame.msgBuf);
        // STRING: FF7 0x0090a9f4
        Gfx_DebugPrint("HEAP END \n");
    }
}

/*
 * List-walk callback of DumpGameHeapBlocks: logs one block's file, line, pointer and size and
 * adds its bytes to *total. Always returns 1.
 */
// FUNCTION: FF7 0x00660193
int __cdecl PrintGameHeapBlockCallback(MemHeapBlock *blockInfo, void *total)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[204];
        MemHeapBlock *entry;
        struct { unsigned char pad_to_4[0x4]; char msgBuf[200]; };
    } frame;
#pragma pack(pop)

    if (blockInfo != 0) {
        frame.entry = blockInfo;
        sprintf(frame.msgBuf, Mem_BlockFormat(), frame.entry->file, frame.entry->line, frame.entry->data, frame.entry->count, frame.entry->elementSize);
        *(int *)total += frame.entry->count * frame.entry->elementSize;
        Gfx_DebugPrint(frame.msgBuf);
    }
    return 1;
}

/*
 * Logs the total bytes the tracker's blocks hold as "HEAP USAGE".
 */
// FUNCTION: FF7 0x00660226
void __cdecl ReportGameHeapUsage(MemHeapTracker *tracker)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[204];
        char msgBuf[200];
        struct { unsigned char pad_to_C8[0xC8]; int totalBytes; };
    } frame;
#pragma pack(pop)

    frame.totalBytes = 0;
    if (tracker != 0 && tracker->enabled != 0 && tracker->blockList != 0) {
        ForEachListEntry((ListVisitFn)SumGameHeapBlockCallback, (int)(&frame.totalBytes), (List *)tracker->blockList);
        // STRING: FF7 0x0090aa00
        sprintf(frame.msgBuf, "  HEAP USAGE %d \n", frame.totalBytes);
        Gfx_DebugPrint(frame.msgBuf);
    }
}

/*
 * Walk callback that accumulates a heap block's byte total into *total: count times elementSize.
 * Always returns 1, so the walk never stops early.
 */
// FUNCTION: FF7 0x00660290
int __cdecl SumGameHeapBlockCallback(MemHeapBlock *blockInfo, void *total)
{
    MemHeapBlock *block;

    if (blockInfo != 0) {
        block = blockInfo;
        *(int *)total += block->count * block->elementSize;
    }
    return 1;
}

/*
 * Enables or disables heap allocation tracking through the tracker object g_MemHeapTracker, when one
 * exists.
 */
// FUNCTION: FF7 0x006602c1
void __cdecl SetGameHeapTrackingEnabled(int value)
{
    if (g_MemHeapTracker != 0) {
        g_MemHeapTracker->enabled = value;
    }
}

/*
 * Dumps the blocks still outstanding on the tracker at g_MemHeapTracker -- the leak report. A one-line
 * forwarder to DumpGameHeapBlocks.
 */
// FUNCTION: FF7 0x006602d9
void __cdecl DumpGameHeapLeaks(void)
{
    DumpGameHeapBlocks((struct MemHeapTracker *)g_MemHeapTracker);
}

/*
 * Reports the game heap's usage through ReportGameHeapUsage with g_MemHeapTracker.
 */
// FUNCTION: FF7 0x006602ec
void __cdecl PrintGameHeapUsage(void)
{
    ReportGameHeapUsage((struct MemHeapTracker *)g_MemHeapTracker);
}

/*
 * Copies size bytes of source into a new game-heap block (GameHeapAllocRaw) and returns it, or 0
 * when the allocation fails.
 */
// FUNCTION: FF7 0x006602ff
void *__cdecl GameHeapMemDup(int size, void *source)
{
    void *result;
    // STRING: FF7 0x0090aa14
    result = GameHeapAllocRaw(size, "C:\\lib\\src\\mem\\mem.cpp", 0x18A);
    if (result != 0) {
        memcpy(result, source, size);
    }
    return result;
}

/*
 * Returns GameHeapMemDup(strlen(text)+1, text) cast to char pointer.
 */
// FUNCTION: FF7 0x00660344
char *__cdecl GameHeapStrDup(char *text)
{
    return (char *)GameHeapMemDup(strlen(text) + 1, text);
}

/*
 * Returns zero unconditionally.
 */
// FUNCTION: FF7 0x00660364
int __cdecl UnusedReturnZero(void)
{
    return 0;
}
} /* extern "C" */
