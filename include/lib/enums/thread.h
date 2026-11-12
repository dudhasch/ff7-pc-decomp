/* include/lib/enums/thread.h: named codes of lib/thread and the TUs that use them.
 * Enums only, so any TU can include it: no types, no declarations. */
#ifndef LIB_ENUMS_THREAD_H
#define LIB_ENUMS_THREAD_H

/* ThreadRecord.kind: how ThreadStartRecord starts the thread; only one kind exists. */
enum ThreadKind {
    THREAD_KIND_CRT = 0 /* _beginthread(ThreadRunEntryProc, stackSize, record) */
};

/* ThreadRecord.state (ThreadGetState/ThreadSetState). ThreadSystemFreeThread frees only a NEW or
 * FINISHED thread. */
enum ThreadState {
    THREAD_STATE_NEW = 0,
    THREAD_STATE_STARTING = 1, /* ThreadStartRecord, before the thread is created */
    THREAD_STATE_RUNNING = 2,  /* ThreadRunEntryProc, while entryProc runs */
    THREAD_STATE_FINISHED = 3  /* entryProc returned */
};

#endif
