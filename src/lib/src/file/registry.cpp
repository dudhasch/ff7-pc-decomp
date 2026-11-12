// src/lib/src/file/registry.cpp
// Original: C:\lib\src\file\registry.cpp

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/api/mem.h"
#include "lib/api/list.h"
#include "lib/api/file.h"
#include "lib/types/graphics.h"
#include "lib/types/polygon.h"
#include "lib/types/list.h"
extern "C" {

/* A file registry: a four-byte record holding the list of registered files. */
typedef struct FileRegistry {
    List *list;
} FileRegistry;

/*
 * Destroys a registry: its list, then the four-byte record. Null is accepted.
 */
// FUNCTION: FF7 0x006a9060
void __cdecl FileRegistryDestroy(int registry)
{
    if (registry != 0) {
        DestroyList(((FileRegistry *)registry)->list);
        // STRING: FF7 0x009177ec
        GameHeapFree((void *)registry, "C:\\lib\\src\\file\\registry.cpp", 14);
    }
}

/*
 * Creates a registry: a four-byte record holding a list created with freeValue. When either
 * allocation fails the partial record is destroyed and 0 is returned.
 */
// FUNCTION: FF7 0x006a908c
int __cdecl FileRegistryCreate(int freeValue)
{
    int registry;
    int isError;

    isError = 1;
    // STRING: FF7 0x0091780c
    registry = (int)GameHeapAlloc(1, 4, "C:\\lib\\src\\file\\registry.cpp", 24);
    if (registry != 0) {
        ((FileRegistry *)registry)->list = CreateList((ListFreeValueFn)freeValue);
        if (((FileRegistry *)registry)->list != 0) {
            isError = 0;
        }
    }
    if (isError != 0) {
        FileRegistryDestroy(registry);
        registry = 0;
    }
    return registry;
}

/*
 * Null-guarded wrapper around ListFindValueByCompare. Returns 0 when holder is null; otherwise
 * calls ListFindValueByCompare with the address of an uninitialised local, compare, key
 * (arguments one and two swapped relative to this function's own order) and the holder's
 * list, and returns that call's result.
 */
// FUNCTION: FF7 0x006a90f5
int __cdecl ListFindValueByCompareInHolder(int key, ListCompareFn compare, int holder)
{
    int result;
    int val;
    int callResult;

    result = 0;
    if (holder != 0) {
        callResult = ListFindValueByCompare(&val, compare, key, ((FileRegistry *)holder)->list);
        result = callResult;
    }
    return result;
}

/*
 * For a nonnull holder, walks the holder's list from its head node. Calls arg3(context, key,
 * node value) until a nonzero result, then returns that value; returns 0 when no node passes
 * or holder is null.
 */
// FUNCTION: FF7 0x006a9132
int __cdecl ListFindPayloadInHolder(int context, void *key, int(__cdecl *arg3)(int, int, int), int holder)
{
    GfxTextureRecord *found;
    ListNode *cur;
    found = 0;
    if (holder != 0) {
        cur = (ListNode *)((FileRegistry *)holder)->list->head;
        while (cur != 0) {
            if (arg3(context, (int)key, cur->value) != 0) {
                found = (GfxTextureRecord *)cur->value;
                break;
            }
            cur = (ListNode *)cur->next;
        }
    }
    return (int)found;
}

/*
 * Null-guarded forwarder: if holder is non-null, appends value to the holder's list.
 */
// FUNCTION: FF7 0x006a918b
void __cdecl ListAppendValueToHolder(int value, int holder)
{
    if (holder != 0) {
        ListAppendValue(value, ((FileRegistry *)holder)->list);
    }
}

/*
 * Null-guarded forwarder: if holder is non-null, removes value from the holder's list.
 */
// FUNCTION: FF7 0x006a91a8
void __cdecl ListRemoveValueFromHolder(int value, int holder)
{
    if (holder != 0) {
        RemoveFromList(value, ((FileRegistry *)holder)->list);
    }
}

/*
 * Null-guarded forwarder: if holder is non-null, calls visit(value, arg) on each entry of
 * the holder's list.
 */
// FUNCTION: FF7 0x006a91c5
void __cdecl ListForEachEntryInHolder(ListVisitFn visit, int arg, int holder)
{
    if (holder != 0) {
        ForEachListEntry(visit, arg, ((FileRegistry *)holder)->list);
    }
}
} /* extern "C" */
