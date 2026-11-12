/* include/lib/types/stack.h: the types lib/stack TUs share. */
#ifndef LIB_TYPES_STACK_H
#define LIB_TYPES_STACK_H

/* A bounded stack of ints (MatrixStack_Create): the capacity it was made with, the number of
 * entries pushed, and the heap array of `capacity` entries. The engine context keeps two of these
 * whose entries are matrix pointers (PushRotMatrix/PopRotMatrix); MatrixStack_PushValue pushes
 * plain values onto the same record. */
typedef struct MatrixStack {
    int capacity;
    int depth;
    int entries;
} MatrixStack;

#endif /* LIB_TYPES_STACK_H */
