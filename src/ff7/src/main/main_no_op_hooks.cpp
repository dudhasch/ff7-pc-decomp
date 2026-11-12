/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
extern "C" {
// Contents: The release build's trace printf, which ignores its format string and arguments and
//   returns zero (the movie and main code's log lines call it), and the empty application init
//   and shutdown hooks.
/*
 * main_no_op_hooks.cpp - the compiled-out trace printf and the empty application lifecycle hooks.
 */

unsigned char g_bssPad_009a0d04[4] = {0};

/*
 * The shipped build's log call: callers pass a format string and its arguments; the body ignores
 * them and returns zero.
 */
// FUNCTION: FF7 0x00414ee0
int __cdecl Trace_Printf(const char *format, ...)
{
    return 0;
}

/*
 * No-op hook for application initialization.
 */
// FUNCTION: FF7 0x00414ee7
void __cdecl App_InitHookNoOp(void)
{
}

/*
 * No-op hook for application shutdown.
 */
// FUNCTION: FF7 0x00414eec
void __cdecl App_ShutdownHookNoOp(void)
{
}
} /* extern "C" */
