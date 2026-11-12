/* include/lib/api/input.h: the functions and objects lib/input defines that other TUs use.
 * One declaration per name. */
#ifndef LIB_API_INPUT_H
#define LIB_API_INPUT_H

#ifdef __cplusplus
extern "C" {
#endif

struct JoystickOutput;

/* input.cpp */
int __cdecl DI_InputInit(void *context);
void __cdecl DI_InputShutdown(void);
int __cdecl DI_MouseCreate(void *context);
int __cdecl DI_MouseAcquire(void);
void __cdecl DI_MouseUnacquire(void);
int __cdecl DI_MouseGetState(void);
int __cdecl DI_MouseGetNextEvent(void);
int __cdecl DI_MouseFlushBuffer(void);
int __cdecl DI_MouseBufferOverflowed(void);
int __cdecl DI_MouseGetPendingCount(void);
int __cdecl DI_KeyboardCreate(void *context);
int __cdecl DI_KeyboardAcquire(void);
void __cdecl DI_KeyboardUnacquire(void);
unsigned char *__cdecl DI_KeyboardGetState(void);
int __cdecl DI_KeyboardGetNextEvent(void);
int __cdecl DI_KeyboardFlushBuffer(void);
int __cdecl DI_KeyboardBufferOverflowed(void);
int __cdecl DI_KeyboardGetPendingCount(void);
struct JoystickOutput *__cdecl Joystick_Poll(void);
int __cdecl Joystick_Init(void);

#ifdef __cplusplus
}
#endif

#endif /* LIB_API_INPUT_H */
