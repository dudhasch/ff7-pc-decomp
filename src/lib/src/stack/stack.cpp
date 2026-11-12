// src/lib/src/stack/stack.cpp
// Original: C:\lib\src\stack\stack.cpp

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/api/mem.h"
#include "lib/api/stack.h"
#include "lib/types/graphics.h"
#include "lib/types/polygon.h"
#include "lib/types/stack.h"
#include "lib/api/graphics.h"
extern "C" {

/*
 * Frees a matrix stack and its internal entry array.
 */
// FUNCTION: FF7 0x006a28f0
void __cdecl MatrixStack_Destroy(MatrixStack *stack)
{
    if (stack != 0) {
        // STRING: FF7 0x009129a8
        GameHeapFree((void *)stack->entries, "C:\\lib\\src\\stack\\stack.cpp", 0xC);
        // STRING: FF7 0x009129c4
        GameHeapFree(stack, "C:\\lib\\src\\stack\\stack.cpp", 0xD);
    }
}

/*
 * Creates a matrix stack with capacity for the given number of entries.
 */
// FUNCTION: FF7 0x006a2924
int __cdecl MatrixStack_Create(int capacity)
{
    MatrixStack *stack;

    // STRING: FF7 0x009129e0
    stack = (MatrixStack *)GameHeapAlloc(1, 0xC, "C:\\lib\\src\\stack\\stack.cpp", 0x15);
    if (stack != 0) {
        stack->capacity = capacity;
        stack->depth = 0;
        // STRING: FF7 0x009129fc
        stack->entries = (int)GameHeapAlloc(capacity, 4, "C:\\lib\\src\\stack\\stack.cpp", 0x1A);
        if (stack->entries == 0) {
            MatrixStack_Destroy(stack);
            stack = 0;
        }
    }
    return (int)stack;
}

/*
 * Pushes a value onto the matrix stack, reporting overflow if full.
 */
// FUNCTION: FF7 0x006a2994
void __cdecl MatrixStack_PushValue(int value, MatrixStack *stack)
{
    if (stack != 0 && stack->capacity != 0) {
        if (stack->depth < stack->capacity) {
            ((int *)stack->entries)[stack->depth] = value;
            (stack->depth)++;
        } else {
            // STRING: FF7 0x00912a18
            Gfx_DebugPrint("ERROR: STACK OVERFLOW \n");
        }
    }
}

/*
 * Reserves a slot on the matrix stack without storing a value.
 */
// FUNCTION: FF7 0x006a29e4
void __cdecl MatrixStack_Push(MatrixStack *stack)
{
    if (stack != 0 && stack->capacity != 0) {
        if (stack->depth < stack->capacity) {
            (stack->depth)++;
        } else {
            // STRING: FF7 0x00912a30
            Gfx_DebugPrint("ERROR: STACK OVERFLOW \n");
        }
    }
}

/*
 * Pops the top entry from the matrix stack, reporting underflow if empty.
 */
// FUNCTION: FF7 0x006a2a22
int __cdecl MatrixStack_Pop(MatrixStack *stack)
{
    int value;

    value = 0;
    if (stack != 0) {
        if (stack->depth > 0) {
            (stack->depth)--;
            value = ((int *)stack->entries)[stack->depth];
        } else {
            // STRING: FF7 0x00912a48
            Gfx_DebugPrint("ERROR: STACK UNDERFLOW \n");
        }
    }
    return value;
}

/*
 * Returns the top entry of the matrix stack without modifying depth.
 */
// FUNCTION: FF7 0x006a2a73
int __cdecl MatrixStack_Top(MatrixStack *stack)
{
    PolygonTransform *top;

    top = 0;
    if (stack != 0 && stack->depth > 0) {
        top = (PolygonTransform *)(((int *)stack->entries)[stack->depth - 1]);
    }
    return (int)top;
}
} /* extern "C" */
