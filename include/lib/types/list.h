/* include/lib/types/list.h: the types lib/list TUs share. */
#ifndef LIB_TYPES_LIST_H
#define LIB_TYPES_LIST_H

/* A list's callbacks (lib/list). The free callback gets each value the list drops (DestroyList,
 * RemoveFromList). A walk's visit callback gets (value, arg) and returns 0 to stop
 * (ForEachListEntry, ListForEachValueRecursive). A compare callback gets (key, value): nonzero is a
 * match for ListFindValueByCompare, <= 0 puts the key before the value for
 * ListInsertSortedByCompare. A value is the word a node holds. */
typedef void(__cdecl *ListFreeValueFn)(int value);
typedef int(__cdecl *ListVisitFn)(int value, int arg);
typedef int(__cdecl *ListCompareFn)(int key, int value);

/* List node holding a next pointer and an int value, allocated by AllocListNode. */
typedef struct ListNode {
    struct ListNode *next;
    int value;
} ListNode;

/* A list: the 0x20 bytes CreateList (game heap) and CreateListCrtHeap (CRT heap) allocate. */
typedef struct List {
    int useCrtHeap; /* +0x00: 1 when the list and its nodes come from calloc/free (CreateListCrtHeap) */
    int initFlag; /* +0x04: both creators set it to 1; no list function reads it */
    int count; /* +0x08 */
    ListNode *head; /* +0x0C */
    ListNode *tail; /* +0x10 */
    ListFreeValueFn freeValue; /* +0x14: called on each value the list drops, or 0 */
    ListVisitFn visit; /* +0x18: the running walk's callback */
    unsigned char pad_1C[0x4];
} List;

#endif /* LIB_TYPES_LIST_H */
