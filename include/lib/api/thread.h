/* include/lib/api/thread.h: the functions and objects lib/thread defines that other TUs use.
 * One declaration per name. */
#ifndef LIB_API_THREAD_H
#define LIB_API_THREAD_H

#ifdef __cplusplus
extern "C" {
#endif
struct ThreadCallbackRecord;
struct ThreadSystemManager;
struct ThreadRecord;

/* thread.cpp */
void __cdecl ThreadInitCallbackRecord(int callback, struct ThreadCallbackRecord *record);
void __cdecl ThreadFreeOwnedRecord(struct ThreadRecord *record);
int __cdecl ThreadCreateRecordFromTemplate(int params);
int __cdecl ThreadInitRecordFromTemplate(struct ThreadRecord *record, int params);
int __cdecl ThreadGetState(struct ThreadRecord *record);
void __cdecl ThreadSetState(int state, struct ThreadRecord *record);
void __cdecl ThreadRunEntryProc(struct ThreadRecord *context);
int __cdecl ThreadStartRecord(struct ThreadRecord *record);
void __cdecl ThreadSystemFreeThread(struct ThreadRecord *thread, struct ThreadSystemManager *manager);
void __cdecl ThreadSystemDestroy(struct ThreadSystemManager *manager);
int __cdecl ThreadSystemCreate(int maxThreads);

#ifdef __cplusplus
}
#endif

#endif /* LIB_API_THREAD_H */
