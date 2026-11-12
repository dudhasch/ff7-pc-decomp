/* include/lib/api/sort.h: the functions and objects lib/sort defines that other TUs use.
 * One declaration per name. */
#ifndef LIB_API_SORT_H
#define LIB_API_SORT_H

#ifdef __cplusplus
extern "C" {
#endif
struct SortBucketTable;

/* sort.cpp */
struct SortBucketTable *__cdecl Sort_CreateBucketTable(int bucketCount, int entryCount);
int __cdecl Sort_DestroyBucketTable(struct SortBucketTable *table);
int __cdecl Sort_ResetBucketTable(struct SortBucketTable *table);
int __cdecl Sort_InsertNode(struct SortBucketTable *state, int key, int bucket, int payload);
void __cdecl Sort_NoOp(void);
void __cdecl Sort_ForEachInOrder(struct SortBucketTable *table, void(__cdecl *arg2)(int, int));

#ifdef __cplusplus
}
#endif

#endif /* LIB_API_SORT_H */
