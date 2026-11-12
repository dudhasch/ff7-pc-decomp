/* include/lib/api/stack.h: the functions and objects lib/stack defines that other TUs use.
 * One declaration per name. */
#ifndef LIB_API_STACK_H
#define LIB_API_STACK_H

#ifdef __cplusplus
extern "C" {
#endif
struct MatrixStack;

/* stack.cpp */
void __cdecl MatrixStack_Destroy(struct MatrixStack *stack);
int __cdecl MatrixStack_Create(int capacity);
void __cdecl MatrixStack_PushValue(int value, struct MatrixStack *stack);
void __cdecl MatrixStack_Push(struct MatrixStack *stack);
int __cdecl MatrixStack_Pop(struct MatrixStack *stack);
int __cdecl MatrixStack_Top(struct MatrixStack *stack);

#ifdef __cplusplus
}
#endif

#endif /* LIB_API_STACK_H */
