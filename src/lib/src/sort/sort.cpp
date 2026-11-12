// src/lib/src/sort/sort.cpp
// Original: C:\lib\src\sort\sort.cpp

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/api/mem.h"
#include "lib/api/sort.h"
#include "lib/types/sort.h"
extern "C" {

/*
 * Allocates bucket table header, occupancy bitmap, bucket lists, and node storage.
 */
// FUNCTION: FF7 0x006a4f10
SortBucketTable *__cdecl Sort_CreateBucketTable(int bucketCount, int entryCount)
{
    SortBucketTable *table;
    int wordCnt;

    // STRING: FF7 0x00912e78
    table = (SortBucketTable *)GameHeapAlloc(1, 32, "C:\\lib\\src\\sort\\sort.cpp", 25);
    if (table == 0) {
        return 0;
    } else {
        wordCnt = bucketCount >> 5;
        if (bucketCount % 32 != 0) {
            wordCnt++;
        }
        // STRING: FF7 0x00912e94
        table->occupancyBits = (int *)GameHeapAlloc(wordCnt, 4, "C:\\lib\\src\\sort\\sort.cpp", 46);
        if (table->occupancyBits == 0) {
            return 0;
        } else {
            // STRING: FF7 0x00912eb0
            table->bucketFirst = (SortNode **)GameHeapAlloc(bucketCount, 4, "C:\\lib\\src\\sort\\sort.cpp", 54);
            if (table->bucketFirst == 0) {
                return 0;
            } else {
                // STRING: FF7 0x00912ecc
                table->bucketLast = (SortNode **)GameHeapAlloc(bucketCount, 4, "C:\\lib\\src\\sort\\sort.cpp", 62);
                if (table->bucketLast == 0) {
                    return 0;
                } else {
                    for (wordCnt = 0; wordCnt < bucketCount; wordCnt++) {
                        table->bucketLast[wordCnt] = 0;
                    }
                    // STRING: FF7 0x00912ee8
                    table->entries = (SortNode *)GameHeapAlloc(entryCount, 12, "C:\\lib\\src\\sort\\sort.cpp", 76);
                    if (table->entries == 0) {
                        return 0;
                    } else {
                        table->nextFree = table->entries;
                        table->bucketCount = bucketCount;
                        table->capacity = entryCount;
                        table->count = 0;
                        return table;
                    }
                }
            }
        }
    }
}

/*
 * Frees bucket table arrays and header record.
 */
// FUNCTION: FF7 0x006a505e
int __cdecl Sort_DestroyBucketTable(SortBucketTable *table)
{
    if (table != 0) {
        if (table->entries != 0) {
            // STRING: FF7 0x00912f04
            GameHeapFree(table->entries, "C:\\lib\\src\\sort\\sort.cpp", 115);
        }
        if (table->bucketLast != 0) {
            // STRING: FF7 0x00912f20
            GameHeapFree(table->bucketLast, "C:\\lib\\src\\sort\\sort.cpp", 120);
        }
        if (table->bucketFirst != 0) {
            // STRING: FF7 0x00912f3c
            GameHeapFree(table->bucketFirst, "C:\\lib\\src\\sort\\sort.cpp", 125);
        }
        if (table->occupancyBits != 0) {
            // STRING: FF7 0x00912f58
            GameHeapFree(table->occupancyBits, "C:\\lib\\src\\sort\\sort.cpp", 130);
        }
        // STRING: FF7 0x00912f74
        GameHeapFree(table, "C:\\lib\\src\\sort\\sort.cpp", 133);
        return 0;
    } else {
        return 1;
    }
}

/*
 * Resets node cursor, bucket lists, occupancy bitmap, and used count.
 */
// FUNCTION: FF7 0x006a510b
int __cdecl Sort_ResetBucketTable(SortBucketTable *table)
{
    int wordCount;
    int i;

    if (table != 0) {
        table->nextFree = table->entries;
        for (i = 0; i < table->bucketCount; i++) {
            table->bucketLast[i] = 0;
        }
        wordCount = table->bucketCount >> 5;
        if (table->bucketCount % 32 != 0) {
            wordCount++;
        }
        for (i = 0; i < wordCount; i++) {
            table->occupancyBits[i] = 0;
        }
        table->count = 0;
        return 0;
    } else {
        return 1;
    }
}

/*
 * Inserts a node into its sorted bucket, preserving ordered links.
 */
// FUNCTION: FF7 0x006a51c1
int __cdecl Sort_InsertNode(SortBucketTable *state, int key, int bucket, int payload)
{
    SortNode *previous;
    int bits;
    SortNode *current;
    int wordIndex;
    int bitIndex;

    if (state->count < state->capacity) {
        state->nextFree->key = key;
        state->nextFree->payload = payload;
        state->nextFree->next = 0;
        wordIndex = bucket >> 5;
        bitIndex = bucket - (wordIndex << 5);
        bits = state->occupancyBits[wordIndex];
        if ((bits >> bitIndex & 1) == 0) {
            state->occupancyBits[wordIndex] = bits | 1 << bitIndex;
            state->bucketFirst[bucket] = state->nextFree;
            state->bucketLast[bucket] = state->nextFree;
            state->bucketLast[bucket]->next = 0;
        } else {
            previous = state->bucketFirst[bucket];
            current = state->bucketFirst[bucket];
            while (current != 0 && current->key < key && previous != state->bucketLast[bucket]) {
                previous = current;
                current = current->next;
            }
            if (current == previous) {
                state->nextFree->next = state->bucketFirst[bucket];
                state->bucketFirst[bucket] = state->nextFree;
            } else if (previous == state->bucketLast[bucket]) {
                state->bucketLast[bucket]->next = state->nextFree;
                state->bucketLast[bucket] = state->nextFree;
            } else {
                previous->next = state->nextFree;
                state->nextFree->next = current;
            }
        }
        state->nextFree += 1;
        (state->count)++;
        return 0;
    } else {
        return 1;
    }
}

/*
 * Empty sorting callback returning immediately.
 */
// FUNCTION: FF7 0x006a5382
void __cdecl Sort_NoOp(void)
{
}

/*
 * Iterates occupied buckets in index order, invoking callback for each node.
 */
// FUNCTION: FF7 0x006a5387
void __cdecl Sort_ForEachInOrder(SortBucketTable *table, void(__cdecl *arg2)(int, int))
{
    SortNode *node;
    SortNode *previousNode;
    int bucketIndex;
    int extra;
    int bits;
    int zero;
    int wordIndex;
    int bitIndex;

    zero = 0;
    for (bucketIndex = 0; bucketIndex < table->bucketCount; bucketIndex++) {
        zero = 0;
        wordIndex = bucketIndex >> 5;
        bitIndex = bucketIndex - (wordIndex << 5);
        bits = table->occupancyBits[wordIndex];
        if ((bits >> bitIndex & 1) != 0) {
            node = table->bucketFirst[bucketIndex];
            do {
                arg2(node->payload, extra);
                previousNode = node;
                node = node->next;
            } while (previousNode != table->bucketLast[bucketIndex]);
        }
    }
}
} /* extern "C" */
