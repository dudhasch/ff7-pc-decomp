// src/lib/src/thread/thread.cpp
// Original: C:\lib\src\thread\thread.cpp
// Contents: Thread management: creates, initializes, starts, stops and frees thread records, tracks
//   their state flags, and runs the thread system that owns them.
/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/types/graphics.h"
#include "lib/types/mem.h"
#include "lib/codegen.h"
#include "lib/api/mem.h"
#include "lib/api/graphics.h"
#include "lib/api/thread.h"
#include "lib/types/thread.h"
#include "lib/enums/thread.h"
#include <stdio.h>
extern "C" {

// clang-format off
// GLOBAL: FF7 0x00db2c08
char g_ThreadEmptyName[1] = {0};
// clang-format on

/*
 * Initializes a thread callback record structure.
 */
// FUNCTION: FF7 0x006892b0
void __cdecl ThreadInitCallbackRecord(int callback, ThreadCallbackRecord *record)
{
    if (record != 0) {
        ZeroFillMemory32(0x10, record);
        record->kind = THREAD_KIND_CRT;
        record->stackSize = 0x10000;
        record->callback = callback;
    }
}

/*
 * Frees an owned thread record and its associated resources.
 */
// FUNCTION: FF7 0x006892e5
void __cdecl ThreadFreeOwnedRecord(ThreadRecord *record)
{
    if (record != 0 && record->ownsRecord != 0) {
        // STRING: FF7 0x00910480
        GameHeapFree(record, "C:\\lib\\src\\thread\\thread.cpp", 0x23);
    }
}

/*
 * Allocates and initializes a new thread record from template parameters.
 */
// FUNCTION: FF7 0x0068930c
int __cdecl ThreadCreateRecordFromTemplate(int params)
{
    ThreadRecord *record;

    record = 0;
    if (params != 0) {
        // STRING: FF7 0x009104a0
        record = (ThreadRecord *)GameHeapAlloc(1, 0x24, "C:\\lib\\src\\thread\\thread.cpp", 0x3B);
        if (record != 0) {
            record = (ThreadRecord *)ThreadInitRecordFromTemplate(record, params);
            record->ownsRecord = 1;
        }
    }
    return (int)record;
}

/*
 * Configures an existing thread record with template parameters.
 */
// FUNCTION: FF7 0x0068935d
int __cdecl ThreadInitRecordFromTemplate(ThreadRecord *record, int params)
{
    if (record != 0) {
        ZeroFillMemory32(0x24, record);
        *(ThreadTemplate *)((int)record) = *(ThreadTemplate *)(params);
    }
    return (int)record;
}

/*
 * Returns the thread's ThreadState (THREAD_STATE_NEW for a null record).
 */
// FUNCTION: FF7 0x00689395
int __cdecl ThreadGetState(ThreadRecord *record)
{
    int state;

    state = 0;
    if (record != 0) {
        state = record->state;
    }
    return state;
}

/*
 * Sets the thread record's ThreadState.
 */
// FUNCTION: FF7 0x006893b6
void __cdecl ThreadSetState(int state, ThreadRecord *record)
{
    if (record != 0) {
        record->state = state;
    }
}

/* Period MSVCRT thread entry points declared without <process.h>. */
extern unsigned long __cdecl _beginthread(void(__cdecl *entry)(void *), unsigned int stack, void *arg);
extern void __cdecl _endthread(void);

/*
 * Thread entry trampoline: marks the thread running, calls its entryProc, then marks it finished.
 */
// FUNCTION: FF7 0x006893ca
void __cdecl ThreadRunEntryProc(ThreadRecord *context)
{
    ThreadRecord *record;
    record = context;
    if (record != 0) {
        if (record->entryProc != 0) {
            ThreadSetState(THREAD_STATE_RUNNING, record);
            record->entryProc();
        }
        ThreadSetState(THREAD_STATE_FINISHED, record);
    }
    _endthread();
}

/*
 * Spawns and starts execution of an operating system thread.
 */
// FUNCTION: FF7 0x0068940e
int __cdecl ThreadStartRecord(ThreadRecord *record)
{
    int result, handle;
    result = 0;
    if (record != 0) {
        ThreadSetState(THREAD_STATE_STARTING, record);
        switch (record->kind) {
        case THREAD_KIND_CRT:
            handle = _beginthread((void (__cdecl *)(void *))ThreadRunEntryProc, record->stackSize, record);
            if (handle != -1) {
                record->handle = handle;
                result = 1;
            } else {
                // STRING: FF7 0x009104c0
                Gfx_DebugPrint("THREAD ERROR: THREAD COULD NOT BE CREATED \n");
            }
            break;
        }
    }
    return result;
}

/*
 * Terminates a thread and removes it from the thread system.
 */
// FUNCTION: FF7 0x00689486
void __cdecl ThreadSystemFreeThread(ThreadRecord *thread, ThreadSystemManager *manager)
{
    int idx;
    ThreadRecord *freeRecord;
    int flag, probeIdx;
    char message[200];
    if (thread != 0 && manager != 0 && manager->activeThreads > 0) {
        flag = 0;
        idx = 0;
        for (probeIdx = 0; probeIdx < manager->maxThreads; probeIdx++) {
            if (manager->threadSlots[probeIdx] == thread) {
                idx = probeIdx;
                flag = 1;
                break;
            }
        }
        if (flag != 0) {
            if (ThreadGetState(thread) == THREAD_STATE_NEW || ThreadGetState(thread) == THREAD_STATE_FINISHED) {
                manager->threadSlots[idx] = 0;
                ThreadFreeOwnedRecord(thread);
                freeRecord = thread;
                FreeBlockToHeap(freeRecord, manager->threadHeap);
                (manager->activeThreads)--;
            } else {
                // STRING: FF7 0x009104ec
                sprintf(message, "THREAD SYSTEM WARNING: ATTEMPTED TO FREE RUNNING THREAD %s \n", thread->name != 0 ? thread->name : g_ThreadEmptyName);
                Gfx_DebugPrint(message);
            }
        } else {
            // STRING: FF7 0x0091052c
            Gfx_DebugPrint("THREAD SYSTEM ERROR: ATTEMPTED TO FREE THREAD NOT IN THREAD SYSTEM \n");
        }
    }
}

/*
 * Stops all managed threads and destroys thread system manager.
 */
// FUNCTION: FF7 0x006895c9
void __cdecl ThreadSystemDestroy(ThreadSystemManager *manager)
{
    int done, i;
    ThreadRecord *pThread;
    if (manager != 0) {
        if (manager->threadSlots != 0) {
            if (manager->activeThreads > 0) {
                done = 0;
                while (done == 0) {
                    for (i = 0; i < manager->maxThreads; i++) {
                        pThread = manager->threadSlots[i];
                        if (pThread != 0)
                            ThreadSystemFreeThread(pThread, manager);
                    }
                    if (manager->activeThreads == 0)
                        done = 1;
                }
            }
            // STRING: FF7 0x00910574
            GameHeapFree(manager->threadSlots, "C:\\lib\\src\\thread\\thread.cpp", 0x132);
        }
        DestroyBlockHeapChain(manager->threadHeap);
        // STRING: FF7 0x00910594
        GameHeapFree(manager, "C:\\lib\\src\\thread\\thread.cpp", 0x137);
    }
}

/*
 * Creates and initializes a thread subsystem manager instance.
 */
// FUNCTION: FF7 0x00689692
int __cdecl ThreadSystemCreate(int maxThreads)
{
    ThreadSystemManager *manager;
    int error;
    // STRING: FF7 0x009105b4
    manager = (ThreadSystemManager *)GameHeapAlloc(1, 16, "C:\\lib\\src\\thread\\thread.cpp", 0x13f);
    if (manager != 0) {
        error = 1;
        manager->maxThreads = maxThreads;
        // STRING: FF7 0x009105d4
        manager->threadSlots = (ThreadRecord ** )GameHeapAlloc(maxThreads, 4, "C:\\lib\\src\\thread\\thread.cpp", 0x148);
        if (manager->threadSlots != 0) {
            manager->threadHeap = (MemBlockHeap *)CreateBlockHeap(36, maxThreads * 36, 0);
            if (manager->threadHeap != 0)
                error = 0;
        }
        if (error != 0) {
            // STRING: FF7 0x009105f4
            Gfx_DebugPrint("THREAD SYSTEM ERROR: THREAD SYSTEM COULD NOT BE CREATED \n");
            ThreadSystemDestroy(manager);
            manager = 0;
        }
    }
    return (int)manager;
}

/*
 * Registers and initializes a new worker thread in the thread system.
 */
// FUNCTION: FF7 0x00689748
int __cdecl ThreadSystemAddThread(int initParams, ThreadSystemManager *manager)
{
    ThreadRecord *thread;
    int i;
    ThreadRecord *pAlloc;
    ThreadRecord *block;

    thread = 0;
    if (initParams != 0 && manager != 0) {
        if (manager->activeThreads < manager->maxThreads) {
            block = (ThreadRecord *)AllocBlockFromHeap(manager->threadHeap);
            pAlloc = block;
            if (pAlloc != 0) {
                thread = pAlloc;
                thread = (ThreadRecord *)ThreadInitRecordFromTemplate(thread, initParams);
                if (thread != 0) {
                    for (i = 0; i < manager->maxThreads; i++) {
                        if (manager->threadSlots[i] == 0) {
                            manager->threadSlots[i] = thread;
                            break;
                        }
                    }
                    manager->activeThreads = manager->activeThreads + 1;
                }
            }
        } else {
            // STRING: FF7 0x00910630
            Gfx_DebugPrint("THREAD SYSTEM ERROR: OUT OF THREADS \n");
        }
    }
    return (int)thread;
}
} /* extern "C" */
