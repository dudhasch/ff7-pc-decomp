/* include/lib/api/list.h: the functions and objects lib/list defines that other TUs use.
 * One declaration per name. */
#ifndef LIB_API_LIST_H
#define LIB_API_LIST_H

#ifdef __cplusplus
extern "C" {
#endif
struct List;

/* list.cpp */
struct ListNode;
struct ListNode *__cdecl AllocListNode(struct List *list);
void __cdecl FreeListNode(struct ListNode *node, struct List *list);
void __cdecl DestroyListNodesRecursive(struct ListNode *node, struct List *list);
struct List *__cdecl CreateListCrtHeap(void(__cdecl *freeValue)(int value));
struct List *__cdecl CreateList(void(__cdecl *freeValue)(int value));
void __cdecl DestroyList(struct List *list);
void __cdecl DestroyListAndNull(struct List **slot);
int __cdecl ListGetCount(struct List *list);
void __cdecl ListPrependValue(int value, struct List *list);
void __cdecl ListAppendValue(int value, struct List *list);
void __cdecl RemoveFromList(int value, struct List *list);
void __cdecl ListForEachValueRecursive(int(__cdecl *visit)(int value, int arg), int arg, struct List *list);
void __cdecl ListVisitNodeRecursive(struct ListNode *node, int arg, struct List *list);
void __cdecl ForEachListEntry(int(__cdecl *visit)(int value, int arg), int arg, struct List *list);
void __cdecl ListFreeValueGameHeap(int ptr);
void *__cdecl ListToArray(int indirect, int size, struct List *list);
int __cdecl ListFindValueByCompare(int *index, int(__cdecl *compare)(int key, int value), int key, struct List *list);
int __cdecl ListFindIndexByValue(int *index, int(__cdecl *compare)(int key, int value), int key, struct List *list);
void __cdecl ListInsertSortedByCompare(int(__cdecl *compare)(int key, int value), int value, struct List *list);
int __cdecl ListCompareEqual(int lhs, int rhs);

#ifdef __cplusplus
}
#endif

#endif /* LIB_API_LIST_H */
