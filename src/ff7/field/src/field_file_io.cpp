// src/ff7/field/src/field_file_io.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: The field module's thin stdio FILE wrappers (open, close, seek, tell, size, read, write) with
//   their error messages; the field data dumps and the chocobo and Fort Condor loaders use them.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/types/field.h"
#include "ff7/enums/field.h"
#include "ff7/enums/main.h"
#include "ff7/api/battle.h"
#include "ff7/api/field.h"
#include "lib/api/graphics.h"
#include "lib/api/sound.h"
#include "lib/api/token.h"
#include "ff7/field.h"
#include "ff7/savemap.h"
#include "lib/crt.h"
#include "lib/enums/sound.h"
#include "ff7/api/menu.h"
extern "C" {

struct FieldPointTo;

/*
 * Opens `name` with the C runtime's fopen in `mode` and returns the FILE pointer, or NULL when
 * the file cannot be opened. The first of the FILE wrappers (open, close, seek, tell, size, read,
 * write) beside it. Not only debug output uses it: the field data dumps in ad_data.cpp, ad_obj.cpp and
 * ad_tile.cpp write text files through it, while the chocobo and Fort Condor loaders open their
 * data files "rb" with it.
 */
// FUNCTION: FF7 0x00620510
FILE *__cdecl Field_FileOpen(char *name, char *mode)
{
    FILE *file;

    file = fopen(name, mode);
    return file;
}

/*
 * `rc` is returned uninitialised when the argument is null:
 * the store is inside the branch and the load is after it.
 * The image really does that, so it is reproduced.
 */
// FUNCTION: FF7 0x0062052e
short __cdecl Field_FileClose(FILE *file)
{
    short rc;

    if (file != 0) {
        rc = (short)fclose(file);
    }
    return rc;
}

/*
 * `mov eax, [ebp+0x10]; and eax, 0xffff` -- a dword
 * parameter narrowed at the call, not a narrow parameter.
 */
// FUNCTION: FF7 0x00620550
void __cdecl Field_FileSeek(FILE *file, int offset, int origin)
{
    fseek(file, offset, (unsigned short)origin);
}

/*
 * File helper: ftell.
 */
// FUNCTION: FF7 0x0062056e
int __cdecl Field_FileTell(FILE *file)
{
    return ftell(file);
}

/*
 * Saves current file position, seeks to end, reads end position and restores saved position,
 * returning end position. Seek failures are ignored and error positions propagate.
 */
// FUNCTION: FF7 0x0062057f
int __cdecl Field_FileSize(FILE *file)
{
    int size;
    int pos;

    pos = Field_FileTell(file);
    Field_FileSeek(file, 0, 2);
    size = Field_FileTell(file);
    Field_FileSeek(file, pos, 0);
    return size;
}

/*
 * Debug file helper: reads one byte into dst, returning 1 on success.
 */
// FUNCTION: FF7 0x006205cc
int __cdecl Field_DebugReadU8(FILE *file, void *dst)
{
    int got;
    int error;

    got = fread(dst, 1, 1, file);
    error = got != 1;
    return error == 0;
}

/*
 * Debug file helper: reads two bytes into dst, returning 1 on success.
 */
// FUNCTION: FF7 0x00620602
int __cdecl Field_DebugReadU16(FILE *file, void *dst)
{
    int got;
    int error;

    got = fread(dst, 1, 2, file);
    error = got != 2;
    return error == 0;
}

/*
 * Debug file helper: reads four bytes into dst, returning 1 on success.
 */
// FUNCTION: FF7 0x00620638
int __cdecl Field_DebugReadU32(FILE *file, void *dst)
{
    int got;
    int error;

    got = fread(dst, 1, 4, file);
    error = got != 4;
    return error == 0;
}

/*
 * Reads exactly size bytes using fread and returns whether its result equals size. The unsigned
 * size-less-than-zero test cannot succeed in this reconstructed signature; no error distinction or
 * pointer validation.
 */
// FUNCTION: FF7 0x0062066e
short __cdecl Field_FileRead(FILE *file, void *dst, unsigned int size)
{
    int got;
    int error;

    if (size < 0) {
        return 0;
    }
    got = fread(dst, 1, size, file);
    error = got != size;
    return error == 0;
}

/*
 * Writes `size` bytes from `src` to `file` with fwrite (element size 1). Returns 1 when all
 * `size` bytes were written, 0 on a short write. The field dumps that write text lines and image
 * blocks through it, and the Fort Condor TIM dump, ignore the result.
 */
// FUNCTION: FF7 0x006206b3
int __cdecl Field_FileWrite(FILE *file, void *src, int size)
{
    int got;
    int error;

    got = fwrite(src, 1, size, file);
    error = got != size;
    return error == 0;
}

/*
 * Debug file helper: writes one byte, returning 1 on success.
 */
// FUNCTION: FF7 0x006206ed
int __cdecl Field_DebugWriteU8(FILE *file, unsigned char value)
{
    int got;
    int error;

    got = fwrite(&value, 1, 1, file);
    error = got != 1;
    return error == 0;
}

/*
 * Debug file helper: writes a 16-bit value, returning 1 on success.
 */
// FUNCTION: FF7 0x00620723
int __cdecl Field_DebugWriteU16(FILE *file, unsigned short value)
{
    int got;
    int error;

    got = fwrite(&value, 1, 2, file);
    error = got != 2;
    return error == 0;
}

/*
 * Debug file helper: writes a 32-bit value, returning 1 on success.
 */
// FUNCTION: FF7 0x00620759
int __cdecl Field_DebugWriteU32(FILE *file, unsigned int value)
{
    int got;
    int error;

    got = fwrite(&value, 1, 4, file);
    error = got != 4;
    return error == 0;
}
} /* extern "C" */
