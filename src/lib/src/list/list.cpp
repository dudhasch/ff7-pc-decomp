// src/lib/src/list/list.cpp
// Original: C:\lib\src\list\list.cpp
// Contents: Generic linked list: allocates nodes and lists through the CRT or game heap, adds,
//   removes and searches values, walks entries with callbacks, and converts lists to arrays.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/api/mem.h"
#include "lib/crt.h"
#include "lib/types/graphics.h"
#include "lib/types/list.h"
#include <stdlib.h>
#include "lib/api/list.h"
#include <string.h>
extern "C" {

#pragma intrinsic(memcpy)

// CreateListCrtHeap allocates via CRT calloc, whereas CreateList uses the engine heap.

/*
 * Allocates a linked list node using the CRT calloc or game heap.
 */
// FUNCTION: FF7 0x0069afc0
ListNode *__cdecl AllocListNode(List *list)
{
    ListNode *node;

    if (list->useCrtHeap != 0) {
        node = (ListNode *)calloc(1, 8);
    } else {
        // STRING: FF7 0x00911fdc
        node = (ListNode *)GameHeapAlloc(1, 8, "C:\\lib\\src\\list\\list.cpp", 19);
    }
    return node;
}

/*
 * Frees a linked list node through the appropriate allocator.
 */
// FUNCTION: FF7 0x0069affa
void __cdecl FreeListNode(ListNode *node, List *list)
{
    if (node != 0) {
        if (list->useCrtHeap != 0) {
            free(node);
        } else {
            // STRING: FF7 0x00911ff8
            GameHeapFree(node, "C:\\lib\\src\\list\\list.cpp", 35);
        }
    }
}

/*
 * Recursively frees linked list nodes from the head of the list.
 */
// FUNCTION: FF7 0x0069b02e
void __cdecl DestroyListNodesRecursive(ListNode *node, List *list)
{
    if (node != 0) {
        DestroyListNodesRecursive(node->next, list);
        if (list->freeValue != 0)
            list->freeValue(node->value);
        FreeListNode(node, list);
    }
}

/*
 * Allocates a list from the CRT heap (its nodes too) with the callback that frees its values.
 */
// FUNCTION: FF7 0x0069b074
List *__cdecl CreateListCrtHeap(ListFreeValueFn freeValue)
{
    List *list;

    list = (List *)calloc(1, 0x20);
    if (list != 0) {
        list->useCrtHeap = 1;
        list->initFlag = 1;
        list->freeValue = freeValue;
    }
    return list;
}

/*
 * Allocates a list from the game heap with the callback that frees its values.
 */
// FUNCTION: FF7 0x0069b0b0
List *__cdecl CreateList(ListFreeValueFn freeValue)
{
    List *localVal;

    // STRING: FF7 0x00912014
    localVal = (List *)GameHeapAlloc(1, 32, "C:\\lib\\src\\list\\list.cpp", 76);
    if (localVal != 0) {
        localVal->initFlag = 1;
        localVal->freeValue = freeValue;
    }
    return localVal;
}

/*
 * Walks and frees all nodes in a list, then frees the list container.
 */
// FUNCTION: FF7 0x0069b0ea
void __cdecl DestroyList(List *list)
{
    if (list != 0) {
        DestroyListNodesRecursive(list->head, list);
        if (list->useCrtHeap != 0) {
            free((void *)list);
        } else {
            // STRING: FF7 0x00912030
            GameHeapFree(list, "C:\\lib\\src\\list\\list.cpp", 98);
        }
    }
}

/*
 * Destroys a list and resets the caller pointer to null.
 */
// FUNCTION: FF7 0x0069b131
void __cdecl DestroyListAndNull(List **slot)
{
    DestroyList(*slot);
    *slot = 0;
}

/*
 * Returns the number of nodes currently stored in the list.
 */
// FUNCTION: FF7 0x0069b14d
int __cdecl ListGetCount(List *list)
{
    int count;

    count = 0;
    if (list != 0) {
        count = list->count;
    }
    return count;
}

/*
 * Inserts a new value at the beginning of the list.
 */
// FUNCTION: FF7 0x0069b16e
void __cdecl ListPrependValue(int value, List *list)
{
    ListNode *node;

    if (list != 0) {
        node = AllocListNode(list);
        if (node != 0) {
            if (list->head == 0) {
                list->tail = node;
            }
            node->next = list->head;
            node->value = value;
            list->head = node;
            (list->count)++;
        }
    }
}

/*
 * Appends a new value to the end of the list.
 */
// FUNCTION: FF7 0x0069b1cf
void __cdecl ListAppendValue(int value, List *list)
{
    ListNode *previousTail;
    ListNode *newNode;

    if (list != 0) {
        newNode = AllocListNode(list);
        if (newNode != 0) {
            previousTail = list->tail;
            if (previousTail != 0) {
                previousTail->next = newNode;
            } else {
                list->head = newNode;
            }
            list->tail = newNode;
            newNode->next = 0;
            newNode->value = value;
            (list->count)++;
        }
    }
}

/*
 * Finds and removes a node containing the specified value from the list.
 */
// FUNCTION: FF7 0x0069b240
void __cdecl RemoveFromList(int value, List *list)
{
    ListNode *previous;
    ListNode *current;
    if (list != 0) {
        previous = 0;
        current = list->head;
        while (current != 0) {
            if (current->value == value) {
                if (list->tail == current)
                    list->tail = previous;
                if (previous != 0)
                    previous->next = current->next;
                else
                    list->head = current->next;
                if (list->freeValue != 0)
                    list->freeValue(current->value);
                FreeListNode(current, list);
                --list->count;
                break;
            }
            previous = current;
            current = current->next;
        }
    }
}

/*
 * Stores the visit callback in the list and walks its values recursively, passing arg.
 */
// FUNCTION: FF7 0x0069b2f7
void __cdecl ListForEachValueRecursive(ListVisitFn visit, int arg, List *list)
{
    if (list != 0 && visit != 0) {
        list->visit = visit;
        ListVisitNodeRecursive(list->head, arg, list);
    }
}

/*
 * Calls the list's visit callback on this node's value and recurses to the next node until the
 * callback returns 0 or the list ends.
 */
// FUNCTION: FF7 0x0069b328
void __cdecl ListVisitNodeRecursive(ListNode *node, int arg, List *list)
{
    if (node != 0) {
        if (list->visit(node->value, arg) != 0)
            ListVisitNodeRecursive(node->next, arg, list);
    }
}

/*
 * Stores the visit callback in the list and calls it on each value, passing arg, until it
 * returns 0.
 */
// FUNCTION: FF7 0x0069b361
void __cdecl ForEachListEntry(ListVisitFn visit, int arg, List *list)
{
    ListNode *node;
    if (list != 0 && visit != 0) {
        list->visit = visit;
        node = list->head;
        while (node != 0) {
            if (visit(node->value, arg) != 0)
                node = node->next;
            else
                break;
        }
    }
}

/*
 * Frees a list value buffer using the game heap allocator.
 */
// FUNCTION: FF7 0x0069b3b0
void __cdecl ListFreeValueGameHeap(int ptr)
{
    // STRING: FF7 0x0091204c
    GameHeapFree((void *)ptr, "C:\\lib\\src\\list\\list.cpp", 266);
}

/*
 * Copies list element values or pointers into a contiguous array.
 */
// FUNCTION: FF7 0x0069b3cb
void *__cdecl ListToArray(int indirect, int size, List *list)
{
    void *result;
    char *dest;
    ListNode *cur;
    result = 0;
    if (list != 0 && ListGetCount(list) > 0) {
        // STRING: FF7 0x00912068
        result = GameHeapAllocRaw(size * ListGetCount(list), "C:\\lib\\src\\list\\list.cpp", 279);
        if (result != 0) {
            dest = (char *)result;
            cur = list->head;
            while (cur != 0) {
                if (indirect != 0)
                    memcpy(dest, (void *)cur->value, size);
                else
                    memcpy(dest, &cur->value, size);
                cur = cur->next;
                dest += size;
            }
        }
    }
    return result;
}

/*
 * Searches for an element matching a predicate and returns the value and index.
 */
// FUNCTION: FF7 0x0069b493
int __cdecl ListFindValueByCompare(int *index, ListCompareFn compare, int key, List *list)
{
    int foundValue;
    int counter;
    ListNode *cur;
    counter = 0;
    foundValue = 0;
    *index = -1;
    if (list != 0 && compare != 0) {
        cur = list->head;
        while (cur != 0) {
            if (compare(key, cur->value) != 0) {
                *index = counter;
                foundValue = cur->value;
                break;
            }
            cur = cur->next;
            counter++;
        }
    }
    return foundValue;
}

/*
 * Finds the index of a matching value using a comparison function.
 */
// FUNCTION: FF7 0x0069b50d
int __cdecl ListFindIndexByValue(int *index, ListCompareFn compare, int key, List *list)
{
    int result;
    int position;
    int foundValue;
    position = 0;
    result = 0;
    *index = -1;
    if (list != 0 && compare != 0) {
        foundValue = ListFindValueByCompare(index, compare, key, list);
        key = foundValue;
        if (*index >= 0) {
            result = 1;
        }
    }
    return result;
}

/*
 * Inserts a value into the list maintaining order defined by a comparator.
 */
// FUNCTION: FF7 0x0069b56d
void __cdecl ListInsertSortedByCompare(ListCompareFn compare, int value, List *list)
{
    ListNode *node;
    ListNode *previousNode;
    ListNode *newNode;
    int done;
    if (list != 0 && compare != 0) {
        previousNode = 0;
        node = list->head;
        if (node != 0) {
            done = 0;
            while (node != 0) {
                if (compare(value, node->value) <= 0) {
                    newNode = AllocListNode(list);
                    if (newNode != 0) {
                        newNode->next = node;
                        newNode->value = value;
                        if (previousNode != 0)
                            previousNode->next = newNode;
                        else
                            list->head = newNode;
                    }
                    done = 1;
                    break;
                }
                previousNode = node;
                node = node->next;
            }
            if (done == 0)
                ListAppendValue(value, list);
        } else
            ListAppendValue(value, list);
    }
}

/*
 * Equality comparison callback returning 1 if two values are equal.
 */
// FUNCTION: FF7 0x0069b647
int __cdecl ListCompareEqual(int lhs, int rhs)
{
    return lhs == rhs;
}
} /* extern "C" */
