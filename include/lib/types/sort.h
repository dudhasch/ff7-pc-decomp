/* include/lib/types/sort.h: the types lib/sort TUs share. */
#ifndef LIB_TYPES_SORT_H
#define LIB_TYPES_SORT_H

/* Sort list node: a next link, a sort key and the payload handed to the walk callback. */
typedef struct SortNode {
    struct SortNode *next;
    int key;
    int payload;
} SortNode;

/* Bucketed sort table: one occupancy bit per bucket, per-bucket first and last node, and a pool of 0xC-byte nodes. */
typedef struct SortBucketTable {
    int bucketCount;
    int *occupancyBits;          /* one bit per bucket */
    int count;                   /* entries in use */
    int capacity;                /* entries allocated */
    SortNode **bucketFirst;      /* per bucket: first entry */
    SortNode **bucketLast;       /* per bucket: last entry */
    SortNode *entries;           /* the node pool */
    SortNode *nextFree;          /* next unused entry */
} SortBucketTable;

#endif /* LIB_TYPES_SORT_H */
