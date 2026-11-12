// src/lib/src/file/direct.cpp
// Original: C:\lib\src\file\direct.cpp
// Contents: Builds, sorts and frees directory listings of filesystem entries, and saves and changes
//   the current working directory.
/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/api/mem.h"
#include "lib/types/file.h"
#include "lib/api/file.h"
#include <io.h>
#include "lib/enums/file.h"
#include <string.h>
#include <direct.h>
#include <stdio.h>
#include <direct.h>
#include <stdio.h>
#include <direct.h>
#include <stdio.h>
#include <direct.h>
#include <stdio.h>
#include <stdlib.h>
#include "lib/crt.h"
#include "lib/codegen.h"
#include "lib/api/graphics.h"
extern "C" {

#pragma intrinsic(strcmp)

/*
 * Returns true if directory entry name is neither '.' nor '..'.
 */
// FUNCTION: FF7 0x0068f960
int __cdecl IsNotDotOrDotDotEntry(char *name)
{
    int result = 1;

    // STRING: FF7 0x00910a64
    if (strcmp(name, ".") == 0) {
        result = 0;
    }
    // STRING: FF7 0x00910a68
    if (strcmp(name, "..") == 0) {
        result = 0;
    }
    return result;
}

/*
 * Frees memory allocated for directory listing entries and container.
 */
// FUNCTION: FF7 0x0068f9aa
void __cdecl FreeDirectoryListing(FileDirectoryListing *listing)
{
    if (listing != 0) {
        // STRING: FF7 0x00910a6c
        GameHeapFree((void *)listing->entries, "C:\\lib\\src\\file\\direct.cpp", 0x26);
        // STRING: FF7 0x00910a88
        GameHeapFree(listing, "C:\\lib\\src\\file\\direct.cpp", 0x27);
    }
}

/*
 * Allocates and initializes an empty directory listing container.
 */
// FUNCTION: FF7 0x0068f9de
int __cdecl AllocDirectoryListing(int entryCount)
{
    FileDirectoryListing *listing;

    // STRING: FF7 0x00910aa4
    listing = (FileDirectoryListing *)GameHeapAlloc(1, 8, "C:\\lib\\src\\file\\direct.cpp", 0x2F);
    if (listing != 0) {
        listing->entryCount = entryCount;
        if (entryCount > 0) {
            // STRING: FF7 0x00910ac0
            listing->entries = (int)GameHeapAlloc(entryCount, 0x20, "C:\\lib\\src\\file\\direct.cpp", 0x35);
        } else {
            listing->entries = 0;
        }
    }
    return (int)listing;
}

/*
 * Counts filesystem entries matching search pattern and filter mode.
 */
// FUNCTION: FF7 0x0068fa3a
int __cdecl CountDirectoryEntries(int mode, const char *pattern)
{
    struct {
        int count;
        int done;
        int attributes;
        long handle;
        int valid;
        struct _finddata_t found;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    frame.count = 0;
    frame.valid = 1;
    switch (mode) {
    case FILE_DIR_LIST_FILES:
        frame.attributes = 0;
        break;
    case FILE_DIR_LIST_DIRECTORIES:
        frame.attributes = _A_SUBDIR;
        break;
    case FILE_DIR_LIST_ALL:
        frame.attributes = _A_SUBDIR;
        break;
    default:
        frame.valid = 0;
        break;
    }
    if (frame.valid != 0) {
        frame.done = 0;
        frame.handle = _findfirst(pattern, &frame.found);
        if (frame.handle != -1) {
            while (frame.done == 0) {
                switch (mode) {
                case FILE_DIR_LIST_FILES:
                    if ((frame.found.attrib & _A_SUBDIR) == 0 && IsNotDotOrDotDotEntry(frame.found.name) != 0)
                        frame.count++;
                    break;
                case FILE_DIR_LIST_DIRECTORIES:
                    if ((frame.found.attrib & _A_SUBDIR) != 0 && IsNotDotOrDotDotEntry(frame.found.name) != 0)
                        frame.count++;
                    break;
                case FILE_DIR_LIST_ALL:
                    if (IsNotDotOrDotDotEntry(frame.found.name) != 0)
                        frame.count++;
                    break;
                }
                frame.done = _findnext(frame.handle, &frame.found);
            }
            _findclose(frame.handle);
        }
    }
    return frame.count;
}

/*
 * Constructs counted array of directory entries matching filter.
 */
// FUNCTION: FF7 0x0068fbdc
int __cdecl BuildDirectoryListing(int mode, const char *pattern)
{
    struct {
        FileDirectoryListing *listing;
        int count;
        int index;
        int done;
        int attributes;
        long handle;
        int valid;
        struct _finddata_t found;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    frame.count = CountDirectoryEntries(mode, pattern);
    frame.listing = (FileDirectoryListing *)AllocDirectoryListing(frame.count);
    if (frame.listing != 0) {
        frame.valid = 1;
        switch (mode) {
        case FILE_DIR_LIST_FILES:
            frame.attributes = 0;
            break;
        case FILE_DIR_LIST_DIRECTORIES:
            frame.attributes = _A_SUBDIR;
            break;
        case FILE_DIR_LIST_ALL:
            frame.attributes = _A_SUBDIR;
            break;
        default:
            frame.valid = 0;
            break;
        }
        if (frame.valid != 0) {
            frame.index = 0;
            frame.done = 0;
            frame.handle = _findfirst(pattern, &frame.found);
            if (frame.handle != -1) {
                while (frame.done == 0) {
                    if (frame.index < frame.count) {
                        switch (mode) {
                        case FILE_DIR_LIST_FILES:
                            if ((frame.found.attrib & _A_SUBDIR) == 0 && IsNotDotOrDotDotEntry(frame.found.name) != 0) {
                                // STRING: FF7 0x00910adc
                                sprintf((char *)(frame.listing->entries + (frame.index << 5)), "%s", frame.found.name);
                                frame.index++;
                            }
                            break;
                        case FILE_DIR_LIST_DIRECTORIES:
                            if ((frame.found.attrib & _A_SUBDIR) != 0 && IsNotDotOrDotDotEntry(frame.found.name) != 0) {
                                // STRING: FF7 0x00910ae0
                                sprintf((char *)(frame.listing->entries + (frame.index << 5)), "%s", frame.found.name);
                                frame.index++;
                            }
                            break;
                        case FILE_DIR_LIST_ALL:
                            if (IsNotDotOrDotDotEntry(frame.found.name) != 0) {
                                // STRING: FF7 0x00910ae4
                                sprintf((char *)(frame.listing->entries + (frame.index << 5)), "%s", frame.found.name);
                                frame.index++;
                            }
                            break;
                        }
                    } else {
                        break;
                    }
                    frame.done = _findnext(frame.handle, &frame.found);
                }
                _findclose(frame.handle);
            }
        }
    }
    return (int)frame.listing;
}

/*
 * Populates pointer array referencing each directory listing entry.
 */
// FUNCTION: FF7 0x0068fe4e
void __cdecl FillDirectoryEntryPointerArray(int pointers, FileDirectoryListing *listing)
{
    int i;

    if (pointers != 0 && listing != 0) {
        for (i = 0; i < listing->entryCount; i++) {
            *(int *)((char *)pointers + i * 4) = listing->entries + (i << 5);
        }
    }
}

/*
 * Allocates pointer array referencing entries in directory listing.
 */
// FUNCTION: FF7 0x0068fe97
int __cdecl AllocDirectoryEntryPointerArray(FileDirectoryListing *listing)
{
    int *pointers;
    int index;

    pointers = 0;
    if (listing != 0) {
        // STRING: FF7 0x00910ae8
        pointers = (int *)GameHeapAlloc(listing->entryCount, 4, "C:\\lib\\src\\file\\direct.cpp", 0xFE);
        for (index = 0; index < listing->entryCount; index++) {
            pointers[index] = listing->entries + (index << 5);
        }
    }
    return (int)pointers;
}

__declspec(dllimport) void __stdcall OutputDebugStringA(const char *);

/*
 * Compares two string pointers case-insensitively using _stricmp.
 */
// FUNCTION: FF7 0x0068ff03
int __cdecl CompareStringPointersCaseInsensitive(void *leftPtr, void *rightPtr)
{
    const char **rhs;
    const char **lhs;

    lhs = (const char **)leftPtr;
    rhs = (const char **)rightPtr;
    return _stricmp(*lhs, *rhs);
}

/*
 * Sorts an array of string pointers alphabetically with quicksort.
 */
// FUNCTION: FF7 0x0068ff2d
void __cdecl SortStringPointersCaseInsensitive(int count, void *strings)
{
    if (count > 1 && strings != 0) {
        qsort(strings, count, 4, (int(__cdecl *)(const void *, const void *))CompareStringPointersCaseInsensitive);
    }
}

/*
 * Compares two directory entry records by their filename field.
 */
// FUNCTION: FF7 0x0068ff55
int __cdecl CompareDirectoryEntryNames(int lhs, int rhs)
{
    int lhsName;
    int rhsName;

    lhsName = lhs;
    rhsName = rhs;
    return _stricmp((const char *)lhsName, (const char *)rhsName);
}

/*
 * Sorts directory listing entry names in alphabetical order.
 */
// FUNCTION: FF7 0x0068ff7b
void __cdecl SortDirectoryListingNames(FileDirectoryListing *listing)
{
    if (listing->entryCount > 1) {
        qsort((void *)listing->entries, listing->entryCount, 0x20, (int(__cdecl *)(const void *, const void *))CompareDirectoryEntryNames);
    }
}

/*
 * Builds directory listing and prints formatted entry names for debugging.
 */
// FUNCTION: FF7 0x0068ffa4
void __cdecl DebugDumpDirectoryListing(int mode)
{
    struct {
        char message[200];
        FileDirectoryListing *listing;
        int index;
    } f; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    f.listing = 0;
    switch (mode) {
    case FILE_DIR_DUMP_NONE:
        break;
    case FILE_DIR_DUMP_C_FILES:
        f.listing = (FileDirectoryListing *)BuildDirectoryListing(FILE_DIR_LIST_FILES,
                                          // STRING: FF7 0x00910b04
                                          "*.C");
        break;
    case FILE_DIR_DUMP_X_FILES:
        f.listing = (FileDirectoryListing *)BuildDirectoryListing(FILE_DIR_LIST_FILES,
                                          // STRING: FF7 0x00910b08
                                          "*.X");
        break;
    case FILE_DIR_DUMP_FILES:
        f.listing = (FileDirectoryListing *)BuildDirectoryListing(FILE_DIR_LIST_FILES,
                                          // STRING: FF7 0x00910b0c
                                          "*.*");
        break;
    case FILE_DIR_DUMP_DIRECTORIES:
        f.listing = (FileDirectoryListing *)BuildDirectoryListing(FILE_DIR_LIST_DIRECTORIES,
                                          // STRING: FF7 0x00910b10
                                          "*.*");
        break;
    case FILE_DIR_DUMP_EXTENSIONLESS_DIRECTORIES:
        f.listing = (FileDirectoryListing *)BuildDirectoryListing(FILE_DIR_LIST_DIRECTORIES,
                                          // STRING: FF7 0x00910b14
                                          "*.");
        break;
    case FILE_DIR_DUMP_ALL:
        f.listing = (FileDirectoryListing *)BuildDirectoryListing(FILE_DIR_LIST_ALL,
                                          // STRING: FF7 0x00910b18
                                          "*.*");
        break;
    }
    if (f.listing != 0) {
        SortDirectoryListingNames(f.listing);
        sprintf(f.message,
                // STRING: FF7 0x00910b1c
                "DIRECTORY START \n");
        OutputDebugStringA(f.message);
        for (f.index = 0; f.index < f.listing->entryCount; f.index++) {
            sprintf(f.message,
                    // STRING: FF7 0x00910b30
                    "  %s \n", (char *)f.listing->entries + f.index * 32);
            OutputDebugStringA(f.message);
        }
    }
    FreeDirectoryListing(f.listing);
}

/*
 * Changes the current directory to `dir` and returns 1, or 0 when `dir` is null or empty or a
 * step fails. When `flags` is not null it is a 200-byte buffer: it is cleared first and then
 * receives the old directory from `_getcwd`, so the caller can change back later; a failed
 * `_getcwd` prints "COULD NOT GET CURRENT DIRECTORY" and skips the change, a failed `_chdir`
 * prints "COULD NOT CHANGE TO DIRECTORY". `OpenFileEx` calls it with "." only to log the
 * current directory.
 */
// FUNCTION: FF7 0x006900fc
int __cdecl ChangeDirectorySaving(char *dir, char *flags)
{
    struct {
        int proceed;
        int result;
        char message[200];
    } f; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    f.result = 0;
    if (flags != 0) {
        *flags = 0;
    }
    if (dir != 0 && *dir != 0) {
        f.proceed = 1;
        if (flags != 0) {
            if (_getcwd(flags, 200) != 0) { /* empty first arm: lib/codegen.h */
            } else {
                sprintf(f.message,
                        // STRING: FF7 0x00910b38
                        "ERROR: COULD NOT GET CURRENT DIRECTORY \n");
                Gfx_DebugPrint(f.message);
                f.proceed = 0;
            }
        }
        if (f.proceed != 0) {
            if (_chdir(dir) == 0) {
                f.result = 1;
            } else {
                sprintf(f.message,
                        // STRING: FF7 0x00910b64
                        "ERROR: COULD NOT CHANGE TO DIRECTORY %s \n", dir);
                Gfx_DebugPrint(f.message);
            }
        }
    }
    return f.result;
}

/*
 * Enters directory `dir` for the files a loader opens with `context` (a `FileOpenRequest`), or
 * leaves it again. Loaders call it twice: with `saveBuf` before opening (enter), then with the
 * saved buffer as `dir` and `saveBuf` null afterwards (leave).
 *
 * For a disk request (`fromArchive` 0) it is `ChangeDirectorySaving(dir, saveBuf)`: enter saves
 * the old directory into `saveBuf` and changes to `dir`, leave changes back; it returns that
 * call's 1 or 0. For an archive request it changes no directory: enter clears `saveBuf` and sets
 * `dir` as the LGP lookup prefix (`SetLgpLookupDirPrefix`) when `dir` is not null, leave clears
 * the prefix; it returns 0 in both cases.
 */
// FUNCTION: FF7 0x006901dd
int __cdecl PushFileContextDirectory(FileOpenRequest *context, void *dir, void *saveBuf)
{
    int result;

    result = 0;
    if (context->fromArchive != 0) {
        if (saveBuf != 0) {
            *(char *)saveBuf = 0;
            if (dir != 0) {
                SetLgpLookupDirPrefix((char *)dir);
            }
        } else {
            ClearLgpLookupDirPrefix();
        }
    } else {
        result = ChangeDirectorySaving((char *)dir, (char *)saveBuf);
    }
    return result;
}
} /* extern "C" */
