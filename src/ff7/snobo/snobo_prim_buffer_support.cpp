// src/ff7/snobo/snobo_prim_buffer_support.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: A text buffer for the snowboard and highway minigames: it allocates the buffer and its
//   descriptor globals, appends formatted strings at the write cursor, rewinds the cursor, and
//   frees the buffer.

#include <stdarg.h>
#include <stdio.h>

#include "ff7/types/snobo.h"
#include "ff7/api/snobo.h"
#include "lib/types/mem.h"
#include "lib/api/mem.h"
/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
extern "C" {

// clang-format off
// GLOBAL: FF7 0x00d8d460
int g_SnoboTextBufferParam0 = 0;
// GLOBAL: FF7 0x00d8d464
int g_SnoboTextBufferParam1 = 0;
// GLOBAL: FF7 0x00d8d468
int g_SnoboTextBufferWidth = 0;
// GLOBAL: FF7 0x00d8d46c
int g_SnoboTextBufferHeight = 0;
// GLOBAL: FF7 0x00d8d470
int g_SnoboTextBufferParam4 = 0;
// GLOBAL: FF7 0x00d8d474
int g_SnoboTextBufferParam5 = 0;
// GLOBAL: FF7 0x00d8d478
char *g_pSnoboTextBufferBase = 0;
// GLOBAL: FF7 0x00d8d47c
char *g_pSnoboTextBufferCursor = 0;
// clang-format on
void __cdecl Mini_ResetPrimBuffer(void);

/*
 * Does nothing.
 */
// FUNCTION: FF7 0x0065ec60
void __cdecl Mini_PrimBufferNoOp(void)
{
}

/*
 * Stores the six arguments into the text buffer descriptor globals, allocates width * height + 1 bytes
 * into g_pSnoboTextBufferBase, rewinds g_pSnoboTextBufferCursor, writes a zero terminator and returns 1.
 */
// FUNCTION: FF7 0x0065ec65
int __cdecl Mini_InitPrimBuffer(int originX, int originY, int width, int height, int charWidth, int charHeight)
{
    g_SnoboTextBufferParam0 = originX;
    g_SnoboTextBufferParam1 = originY;
    g_SnoboTextBufferWidth = width;
    g_SnoboTextBufferHeight = height;
    g_SnoboTextBufferParam4 = charWidth;
    g_SnoboTextBufferParam5 = charHeight;
    g_pSnoboTextBufferBase = (char *)operator new(width * height + 1);
    g_pSnoboTextBufferCursor = g_pSnoboTextBufferBase;
    *g_pSnoboTextBufferCursor = 0;
    return 1;
}

/*
 * Formats and appends a string at the text buffer cursor using vsprintf, advancing the cursor.
 */
// FUNCTION: FF7 0x0065ecd0
int __cdecl Mini_HighwayAppendPrimFormat(const char *format, ...)
{
    va_list args;
    int written;
    va_start(args, format);
    written = vsprintf(g_pSnoboTextBufferCursor, format, args);
    if (written > 0)
        g_pSnoboTextBufferCursor += written;
    va_end(args);
    return written;
}

/*
 * Empties the text buffer for the next frame by moving its write cursor back to the start
 * (g_pSnoboTextBufferBase), so `Mini_HighwayAppendPrimFormat` writes over the old text. It does
 * not write a terminator. `Mini_HighwayRunFrame` calls it once per frame after the scene
 * update.
 */
// FUNCTION: FF7 0x0065ed18
void __cdecl Mini_ResetPrimBuffer(void)
{
    g_pSnoboTextBufferCursor = g_pSnoboTextBufferBase;
}

/*
 * Frees the text buffer `Mini_InitPrimBuffer` allocated, on minigame exit (the highway's
 * `Mini_Mode6ExitStoreScores`, `Mini_SnowboardShutdown`). The base and cursor globals keep the
 * freed address.
 */
// FUNCTION: FF7 0x0065ed27
void __cdecl Mini_ReleasePrimBuffer(void)
{
    char *rec;

    rec = g_pSnoboTextBufferBase;
    operator delete(rec);
}
} /* extern "C" */
