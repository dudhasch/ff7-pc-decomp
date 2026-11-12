// src/lib/src/file/is_lib.cpp
// Original: C:\lib\src\file\is_lib.cpp
// Contents: Provides the low-level and stdio file wrappers and the LGP archive support: opening,
//   reading the table of contents, locating and reading archive entries.
/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/api/mem.h"
#include "lib/types/file.h"
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include "lib/api/file.h"
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
extern "C" {

// clang-format off
// GLOBAL: FF7 0x00d8f678
struct FileLgpTocBlock g_LgpTocBlocks[18] = {0};
// GLOBAL: FF7 0x00d8f708
int g_LgpConflictCounts[18] = {0};
// GLOBAL: FF7 0x00d8f750
FileLgpConflictRecord g_LgpConflictTable[18][1000] = {0};
// GLOBAL: FF7 0x00db29d0
struct FileLgpLookupDir g_LgpFileHandles = {0};
// GLOBAL: FF7 0x00db2a98
FileLgpLookupRow *g_LgpLookupTables[18] = {0};
// GLOBAL: FF7 0x00db2ae0
short g_LgpActiveArchiveCount = 0;
// GLOBAL: FF7 0x00db2ae4
int g_LgpLookupPrefixEnabled = 0;
// GLOBAL: FF7 0x00db2ae8
int g_LgpCurrentArchiveId = 0;
unsigned char g_bssPad_00db2aec[4] = {0};
// clang-format on
struct _iobuf;

/*
 * Sets the active LGP archive index.
 */
// FUNCTION: FF7 0x006750e0
void __cdecl SetCurrentLgpArchiveId(int archiveIndex)
{
    g_LgpCurrentArchiveId = archiveIndex;
}

/*
 * Returns the active LGP archive index.
 */
// FUNCTION: FF7 0x006750ed
int __cdecl GetCurrentLgpArchiveId(void)
{
    return g_LgpCurrentArchiveId;
}

/*
 * Opens a file stream via stdio fopen.
 */
// FUNCTION: FF7 0x006750f7
struct _iobuf *__cdecl FileOpenStdio(char *fileName, char *mode)
{
    struct _iobuf *stream;

    stream = fopen(fileName, mode);
    return stream;
}

/*
 * Closes an open stdio file stream.
 */
// FUNCTION: FF7 0x00675115
short __cdecl FileCloseStdio(struct _iobuf *stream)
{
    unsigned short result;

    if (stream != 0) {
        result = fclose(stream);
    }
    return (short)result;
}

/*
 * Seeks to a position in a stdio file stream.
 */
// FUNCTION: FF7 0x00675137
void __cdecl FileSeekStdio(struct _iobuf *stream, int offset, int origin)
{
    fseek(stream, offset, origin & 0xFFFF);
}

/*
 * Queries the current position in a stdio stream and returns ftell's result.
 */
// FUNCTION: FF7 0x00675155
int __cdecl FileTellStdio(struct _iobuf *stream)
{
    return ftell(stream);
}

/*
 * Calculates total length of a stdio file stream.
 */
// FUNCTION: FF7 0x00675166
int __cdecl FileLengthStdio(struct _iobuf *stream)
{
    int length;
    int pos;

    pos = FileTellStdio(stream);
    FileSeekStdio(stream, 0, SEEK_END);
    length = FileTellStdio(stream);
    FileSeekStdio(stream, pos, SEEK_SET);
    return length;
}

/*
 * Reads an 8-bit unsigned integer from a stdio stream.
 */
// FUNCTION: FF7 0x006751b3
int __cdecl FileReadU8Stdio(struct _iobuf *stream, unsigned char *dst)
{
    int count;
    int shortRead;

    count = fread(dst, 1, 1, stream);
    shortRead = count != 1;
    return shortRead == 0;
}

/*
 * Reads a 16-bit unsigned integer from a stdio stream.
 */
// FUNCTION: FF7 0x006751e9
int __cdecl FileReadU16Stdio(struct _iobuf *stream, unsigned short *dst)
{
    int count;
    int shortRead;

    count = fread(dst, 1, 2, stream);
    shortRead = count != 2;
    return shortRead == 0;
}

/*
 * Reads a 32-bit unsigned integer from a stdio stream.
 */
// FUNCTION: FF7 0x0067521f
int __cdecl FileReadU32Stdio(struct _iobuf *stream, unsigned int *dst)
{
    int count;
    int shortRead;

    count = fread(dst, 1, 4, stream);
    shortRead = count != 4;
    return shortRead == 0;
}

/*
 * Reads a block of bytes from a stdio stream.
 */
// FUNCTION: FF7 0x00675255
short __cdecl FileReadBytesStdio(struct _iobuf *stream, void *buffer, int size)
{
    int numRead;
    int shortRead;

    if ((unsigned int)size < (unsigned int)0) {
        return 0;
    } else {
        numRead = fread(buffer, 1, size, stream);
        shortRead = numRead != size;
        return shortRead == 0;
    }
}

/*
 * Opens a low-level OS file descriptor.
 */
// FUNCTION: FF7 0x0067529a
int __cdecl FileOpenLL(char *fileName, int flags)
{
    int fd;

    fd = _open(fileName, flags);
    return fd;
}

/*
 * Closes a low-level OS file descriptor.
 */
// FUNCTION: FF7 0x006752b8
short __cdecl FileCloseLL(int fd)
{
    unsigned short result;

    if (fd != -1) {
        result = _close(fd);
    }
    return (short)result;
}

/*
 * Repositions low-level OS file descriptor offset.
 */
// FUNCTION: FF7 0x006752da
void __cdecl FileSeekLL(int fd, int offset, int origin)
{
    _lseek(fd, offset, origin & 0xFFFF);
}

/*
 * Queries the current offset of a low-level file descriptor and returns the result of `_tell`.
 */
// FUNCTION: FF7 0x006752f8
int __cdecl FileTellLL(int fd)
{
    return _tell(fd);
}

/*
 * Calculates total byte length of low-level OS file.
 */
// FUNCTION: FF7 0x00675309
int __cdecl FileLengthLL(int fd)
{
    int length;
    int pos;

    pos = FileTellLL(fd);
    FileSeekLL(fd, 0, SEEK_END);
    length = FileTellLL(fd);
    FileSeekLL(fd, pos, SEEK_SET);
    return length;
}

/*
 * Reads an 8-bit unsigned byte from low-level file descriptor.
 */
// FUNCTION: FF7 0x00675356
int __cdecl FileReadU8(int fd, unsigned char *dst)
{
    int count;
    int shortRead;

    count = _read(fd, dst, 1);
    shortRead = count != 1;
    return shortRead == 0;
}

/*
 * Reads a 16-bit unsigned integer from low-level file descriptor.
 */
// FUNCTION: FF7 0x0067538a
int __cdecl FileReadU16(int fd, unsigned short *dst)
{
    int count;
    int shortRead;

    count = _read(fd, dst, 2);
    shortRead = count != 2;
    return shortRead == 0;
}

/*
 * Reads a 32-bit unsigned integer from low-level file descriptor.
 */
// FUNCTION: FF7 0x006753be
int __cdecl FileReadU32(int fd, unsigned int *dst)
{
    int count;
    int shortRead;

    count = _read(fd, dst, 4);
    shortRead = count != 4;
    return shortRead == 0;
}

/*
 * Reads a byte buffer from low-level file descriptor.
 */
// FUNCTION: FF7 0x006753f2
short __cdecl FileReadBytes(int fd, void *buffer, int size)
{
    int numRead;
    int shortRead;

    if ((unsigned int)size < (unsigned int)0) {
        return 0;
    } else {
        numRead = _read(fd, buffer, size);
        shortRead = numRead != size;
        return shortRead == 0;
    }
}

/*
 * Writes a byte buffer to a stdio FILE stream (fwrite).
 */
// FUNCTION: FF7 0x00675435
int __cdecl FileWriteBytesStdio(struct _iobuf *file, void *buffer, int size)
{
    int failed;
    int numberWritten;

    numberWritten = fwrite(buffer, 1, size, file);
    failed = numberWritten != size;
    return failed == 0;
}

/*
 * Writes an 8-bit unsigned byte to a stdio FILE stream (fwrite).
 */
// FUNCTION: FF7 0x0067546f
int __cdecl FileWriteU8Stdio(struct _iobuf *file, unsigned char value)
{
    int failed;
    int numberWritten;

    numberWritten = fwrite(&value, 1, 1, file);
    failed = numberWritten != 1;
    return failed == 0;
}

/*
 * Writes a 16-bit unsigned integer to a stdio FILE stream (fwrite).
 */
// FUNCTION: FF7 0x006754a5
int __cdecl FileWriteU16Stdio(struct _iobuf *file, unsigned short value)
{
    int failed;
    int numberWritten;

    numberWritten = fwrite(&value, 1, 2, file);
    failed = numberWritten != 2;
    return failed == 0;
}

/*
 * Writes a 32-bit unsigned integer to a stdio file stream.
 */
// FUNCTION: FF7 0x006754db
int __cdecl FileWriteU32Stdio(struct _iobuf *file, unsigned int value)
{
    int failed;
    int numberWritten;

    numberWritten = fwrite(&value, 1, 4, file);
    failed = numberWritten != 4;
    return failed == 0;
}

// is_lib.cpp's literal stream follows the original source lines: the __FILE__ literals of OpenLgpArchive,
// ReadLgpToc, ReadLgpConflictTable, FreeAllLgpConflictTables and FreeLgpConflictTable (lines 0x192..0x233)
// precede CloseLgpArchive's, although .text places the two Free functions after the Close ones. Each of the
// five is returned by a static __inline function, a parse-time unit; under /Ob1 the call expands to the
// plain push.
static __inline char *IsLibCpp_FileLine192(void)
{
    // STRING: FF7 0x0090d16c
    return "C:\\lib\\src\\file\\is_lib.cpp";
}
static __inline char *IsLibCpp_FileLine1CC(void)
{
    // STRING: FF7 0x0090d188
    return "C:\\lib\\src\\file\\is_lib.cpp";
}
static __inline char *IsLibCpp_FileLine1F3(void)
{
    // STRING: FF7 0x0090d1a4
    return "C:\\lib\\src\\file\\is_lib.cpp";
}
static __inline char *IsLibCpp_FileLine224(void)
{
    // STRING: FF7 0x0090d1c0
    return "C:\\lib\\src\\file\\is_lib.cpp";
}
static __inline char *IsLibCpp_FileLine233(void)
{
    // STRING: FF7 0x0090d1dc
    return "C:\\lib\\src\\file\\is_lib.cpp";
}

/*
 * Opens LGP archive `name` into slot `lgp` (0..17) and loads the tables `FindLgpFile` searches.
 * The file is opened read-only in binary mode (`FileOpenLL(name, 0x8000)`, 0x8000 = _O_BINARY)
 * and its descriptor stored in `g_LgpFileHandles[lgp]`. `ReadLgpToc` reads the table of contents
 * (24-byte entries). A 0xE10-byte lookup table of 30 x 30 u16 pairs is allocated and read: per
 * cell, keyed by the first two characters of a name, the 1-based first TOC index and the entry
 * count. `ReadLgpConflictTable` then reads the directory records of names that occur more than
 * once. Each open adds one to `g_LgpActiveArchiveCount`, which nothing reads.
 *
 * Returns 1 on success; 0 when the slot is 18 or more, the file does not open, or the TOC or the
 * lookup table cannot be allocated. Defects: a negative slot is not refused; after a failed TOC
 * read or allocation the descriptor stays open in the slot; the 900 u16 reads are not checked
 * for a short file.
 */
// FUNCTION: FF7 0x00675511
int __cdecl OpenLgpArchive(char *name, int lgp)
{
    /* Members preserve the three observed EBP-relative scalar slots. */
    struct {
        int second;
        int first;
        int handle;
    } v;
    if (lgp >= 18)
        return 0;
    v.handle = FileOpenLL(name, _O_BINARY);
    if (v.handle == -1)
        return 0;
    g_LgpFileHandles.fileHandles[lgp] = v.handle;
    if (!ReadLgpToc(v.handle, lgp))
        return 0;
    g_LgpLookupTables[lgp] = (FileLgpLookupRow *)GameHeapAllocRaw(3600, (char *)IsLibCpp_FileLine192(), 0x192);
    if (g_LgpLookupTables[lgp] == 0)
        return 0;
    for (v.first = 0; v.first < 30; v.first++) {
        for (v.second = 0; v.second < 30; v.second++) {
            FileReadU16(v.handle, (unsigned short *)((int)g_LgpLookupTables[lgp] + v.first * 30 * 4 + v.second * 4));
            FileReadU16(v.handle, (unsigned short *)((int)g_LgpLookupTables[lgp] + v.first * 30 * 4 + v.second * 4 + 2));
        }
    }
    ReadLgpConflictTable(v.handle, lgp);
    g_LgpActiveArchiveCount++;
    return 1;
}

/*
 * Reads LGP archive table of contents and directory entries.
 */
// FUNCTION: FF7 0x0067564e
int __cdecl ReadLgpToc(int handle, int lgp)
{
    /* The 28-byte frame is an observed layout; only ten signature bytes are read. */
    struct {
        unsigned int index;
        unsigned short tag;
        unsigned short padding;
        unsigned int count;
        char signature[16];
    } v;
    FileReadU16(handle, &v.tag);
    FileReadBytes(handle, v.signature, 10);
    FileReadU32(handle, &v.count);
    g_LgpTocBlocks[lgp].entryCount = v.count;
    g_LgpTocBlocks[lgp].entries = (FileLgpTocEntry *)GameHeapAllocRaw(v.count * 24, (char *)IsLibCpp_FileLine1CC(), 0x1cc);
    if (g_LgpTocBlocks[lgp].entries == 0)
        return 0;
    for (v.index = 0; v.index < v.count; v.index++) {
        FileReadBytes(handle, &g_LgpTocBlocks[lgp].entries[v.index], 20);
        FileReadU32(handle, (unsigned int *)&g_LgpTocBlocks[lgp].entries[v.index].filePtr);
        FileReadU8(handle, &g_LgpTocBlocks[lgp].entries[v.index].typeByte);
        FileReadU16(handle, &g_LgpTocBlocks[lgp].entries[v.index].conflictLink);
    }
    return 1;
}

/*
 * Reads name conflict resolution tables for an LGP archive.
 */
// FUNCTION: FF7 0x0067577a
void __cdecl ReadLgpConflictTable(int fd, int archiveIndex)
{
    int entryIndex;
    int j;

    g_LgpConflictCounts[archiveIndex] = 0;
    FileReadU16(fd, (unsigned short *)&g_LgpConflictCounts[archiveIndex]);
    for (entryIndex = 0; entryIndex < g_LgpConflictCounts[archiveIndex]; entryIndex++) {
        FileReadU16(fd, &g_LgpConflictTable[archiveIndex][entryIndex].countWord);
        g_LgpConflictTable[archiveIndex][entryIndex].dirs = (FileLgpConflictDir *)GameHeapAllocRaw(g_LgpConflictTable[archiveIndex][entryIndex].count * 0x82, IsLibCpp_FileLine1F3(), 0x1F3);
        for (j = 0; j < g_LgpConflictTable[archiveIndex][entryIndex].count; j++) {
            FileReadBytes(fd, g_LgpConflictTable[archiveIndex][entryIndex].dirs[j].name, 128);
            FileReadU16(fd, &g_LgpConflictTable[archiveIndex][entryIndex].dirs[j].tocIndex);
        }
    }
}

#pragma intrinsic(strcpy, strlen)

/*
 * Sets directory prefix for resolving LGP archive lookups.
 */
// FUNCTION: FF7 0x006758c3
int __cdecl SetLgpLookupDirPrefix(char *name)
{
    int val;
    /* The biased endpoints share the handle/prefix physical view.
       Capacity and the empty-string behavior remain as in the original. */
    strcpy(g_LgpFileHandles.dirPrefix, name);
    val = strlen(g_LgpFileHandles.dirPrefix);
    if ((&g_LgpFileHandles.pathBuffer)[val] != '\\' && (&g_LgpFileHandles.pathBuffer)[val] != '/') {
        g_LgpFileHandles.dirPrefix[val] = '/';
        (&g_LgpFileHandles.dirPrefix[1])[val] = 0;
    }
    g_LgpLookupPrefixEnabled = 1;
    return 1;
}
#pragma function(strcpy, strlen)

/*
 * Clears directory prefix for LGP archive lookups.
 */
// FUNCTION: FF7 0x00675949
int __cdecl ClearLgpLookupDirPrefix(void)
{
    g_LgpLookupPrefixEnabled = 0;
    return 1;
}

/*
 * Maps filename characters to lookup table indices.
 */
// FUNCTION: FF7 0x0067595d
char __cdecl GetLgpLookupCharCode(char character)
{
    char lower;

    /* Signed char promotion and the original CRT locale behavior are retained. */
    lower = tolower(character);
    if (lower >= 97 && lower <= 122) {
        return (char)lower;
    } else if (lower >= 48 && lower <= 57) {
        return lower + 0x31;
    } else if (lower == 0x5F) {
        return 0x6B;
    } else if (lower == 0x2D) {
        return 0x6C;
    } else if (lower == 0x2E) {
        return (char)lower;
    } else {
        return (char)lower;
    }
}

#pragma intrinsic(strcpy, strcat, strlen)
/* Each member corresponds to a directly observed live frame slot or buffer. */
typedef struct ArchiveLookupFrame {
    char saved_dir[128];
    char drive[4];
    int conflict;
    char directory[256];
    char filename[256];
    char extension[256];
    int first;
    int length;
    int j;
    char work[256];
    int i;
    char normalized[256];
    int toc;
    char name[16];
    int count;
} ArchiveLookupFrame;

/*
 * Looks file `name` up in the LGP archive in slot `lgp` and returns its entry offset in the
 * archive (the TOC entry's `filePtr`), or 0 when the slot is 18 or more or the name is not there.
 * Callers pass the offset to `GetLgpEntrySize`, `ReadLgpEntry` or `SeekLgpArchive`; the entry
 * there holds the 20-byte name and the u32 size, and the data follows at offset + 0x18.
 *
 * The path is split with `_splitpath`; when `SetLgpLookupDirPrefix` set a prefix, it goes before
 * the directory. File name and extension are joined, and their first two characters pick the
 * lookup-table cell through `GetLgpLookupCharCode` (a..z give 0..25, the digits 0..9 share 0..9
 * with a..j, '_' gives 10 and '-' 11; a second character '.' selects column 0, any other one
 * column 1 + its code). The cell's TOC range is scanned with `strcmpi`. A match without a
 * conflict link returns its offset at once. For a match with one, the path's directory, with
 * leading and trailing slashes stripped, is compared against that link's conflict records
 * (128-byte directory, u16 TOC index), and the offset of the TOC entry the matching record names
 * is returned; when no record matches, the scan goes on. A path without a directory is matched
 * by the current directory.
 *
 * Defects: a first or second character below 'a' that is not a digit, '_', '-' (or '.' in second
 * place) gives a negative index and reads before the lookup table; a negative slot is not
 * refused. For a path without a directory it overwrites the terminator of the
 * `GetCurrentDirectoryA` string with '/' and writes no new one, so `_splitpath` reads on into
 * uninitialised stack bytes.
 */
// FUNCTION: FF7 0x006759d2
void *__cdecl FindLgpFile(char *name, int lgp)
{
    ArchiveLookupFrame rec;
    if (lgp >= 18) {
        return 0;
    }
    _splitpath(name, rec.drive, rec.directory, rec.filename, rec.extension);
    if (g_LgpLookupPrefixEnabled != 0) {
        strcpy(rec.saved_dir, rec.directory);
        strcpy(rec.directory, g_LgpFileHandles.dirPrefix);
        strcat(rec.directory, rec.saved_dir);
    }
    strcpy(rec.name, rec.filename);
    strcat(rec.name, rec.extension);
    rec.name[strlen(rec.name)] = 0;
    rec.i = GetLgpLookupCharCode(rec.name[0]) - 'a';
    if (rec.name[1] == '.') {
        rec.j = 0;
    } else {
        rec.j = GetLgpLookupCharCode(rec.name[1]) - 'a' + 1;
    }
    rec.first = *(unsigned short *)((int)g_LgpLookupTables[lgp] + rec.i * 30 * 4 + rec.j * 4);
    if (rec.first <= 0) {
        return 0;
    }
    rec.first -= 1;
    rec.count = *(unsigned short *)((int)g_LgpLookupTables[lgp] + rec.i * 30 * 4 + rec.j * 4 + 2);
    for (rec.i = rec.first; rec.i < rec.first + rec.count; rec.i++) {
        if (strcmpi(rec.name, g_LgpTocBlocks[lgp].entries[rec.i].name) == 0) {
            if (g_LgpTocBlocks[lgp].entries[rec.i].conflictLink == 0) {
                return g_LgpTocBlocks[lgp].entries[rec.i].filePtr;
            }
            rec.conflict = g_LgpTocBlocks[lgp].entries[rec.i].conflictLink - 1;
            if (rec.directory[0] == 0) {
                GetCurrentDirectoryA(256, rec.work);
                rec.work[strlen(rec.work)] = '/';
                _splitpath(rec.work, rec.drive, rec.directory, rec.filename, rec.extension);
            }
            if (rec.directory[0] == '\\' || rec.directory[0] == '/') {
                strncpy(rec.normalized, rec.directory + 1, strlen(rec.directory) - 1);
                rec.normalized[strlen(rec.directory) - 1] = 0;
            } else {
                strcpy(rec.normalized, rec.directory);
            }
            rec.length = strlen(rec.normalized);
            if (rec.normalized[rec.length - 1] == '\\' || rec.normalized[rec.length - 1] == '/') {
                rec.normalized[rec.length - 1] = 0;
                rec.length -= 1;
            } else {
                rec.normalized[rec.length] = 0;
            }
            for (rec.j = 0; rec.j < g_LgpConflictTable[lgp][rec.conflict].count; rec.j++) {
                strncpy(rec.work, g_LgpConflictTable[lgp][rec.conflict].dirs[rec.j].name, strlen(g_LgpConflictTable[lgp][rec.conflict].dirs[rec.j].name));
                rec.work[strlen(g_LgpConflictTable[lgp][rec.conflict].dirs[rec.j].name)] = 0;
                if (strcmpi(rec.normalized, rec.work) == 0) {
                    rec.toc = g_LgpConflictTable[lgp][rec.conflict].dirs[rec.j].tocIndex;
                    return g_LgpTocBlocks[lgp].entries[rec.toc].filePtr;
                }
            }
        }
    }
    return 0;
}

#pragma function(strcpy, strcat, strlen)

/*
 * Closes the LGP archive in slot `archiveIndex` and frees what `OpenLgpArchive` allocated: the
 * descriptor is closed and the slot set to 0, then the TOC, the lookup table and the slot's
 * conflict-record arrays (`FreeLgpConflictTable`) are freed. Modules call it when they leave
 * (battle, chocobo, coaster, field, menu; `MidiShutdown` for slot 11). Returns 1, or 0 for a slot
 * of 18 or more.
 *
 * Defect: the "no file" test compares the descriptor with -1, but an unused slot holds 0 and a
 * closed one is set to 0, so closing such a slot calls `FileCloseLL(0)` on descriptor 0.
 */
// FUNCTION: FF7 0x00675f1d
int __cdecl CloseLgpArchive(int archiveIndex)
{
    if (archiveIndex >= 18) {
        return 0;
    } else {
        if (g_LgpFileHandles.fileHandles[archiveIndex] != -1) {
            FileCloseLL(g_LgpFileHandles.fileHandles[archiveIndex]);
            g_LgpFileHandles.fileHandles[archiveIndex] = 0;
        }
        if (g_LgpTocBlocks[archiveIndex].entries != 0) {
            // STRING: FF7 0x0090d1f8
            GameHeapFree(g_LgpTocBlocks[archiveIndex].entries, "C:\\lib\\src\\file\\is_lib.cpp", 727);
            g_LgpTocBlocks[archiveIndex].entries = 0;
        }
        if (g_LgpLookupTables[archiveIndex] != 0) {
            // STRING: FF7 0x0090d214
            GameHeapFree(g_LgpLookupTables[archiveIndex], "C:\\lib\\src\\file\\is_lib.cpp", 732);
            g_LgpLookupTables[archiveIndex] = 0;
        }
        FreeLgpConflictTable(archiveIndex);
        return 1;
    }
}

/*
 * Frees conflict resolution table memory for an LGP archive.
 */
// FUNCTION: FF7 0x00675fde
void __cdecl FreeLgpConflictTable(int archiveIndex)
{
    int entryIndex;

    if (archiveIndex >= 18) {
        return;
    }
    for (entryIndex = 0; entryIndex < g_LgpConflictCounts[archiveIndex]; entryIndex++) {
        if (g_LgpConflictTable[archiveIndex][entryIndex].dirs != 0) {
            GameHeapFree(g_LgpConflictTable[archiveIndex][entryIndex].dirs, IsLibCpp_FileLine233(), 0x233);
            g_LgpConflictTable[archiveIndex][entryIndex].dirs = 0;
        }
    }
}

/*
 * Closes all 18 LGP archive slots at shutdown (`App_Shutdown`): for each slot the descriptor is
 * closed and set to 0 and the TOC and lookup table are freed, then `FreeAllLgpConflictTables`
 * frees every conflict-record array. Returns 1. `g_LgpActiveArchiveCount` is not reset.
 *
 * Defect: as in `CloseLgpArchive`, the test is against -1 while unused and closed slots hold 0,
 * so `FileCloseLL(0)` is called once for every slot that is not open.
 */
// FUNCTION: FF7 0x00676064
int __cdecl CloseAllLgpArchives(void)
{
    unsigned short i;

    for (i = 0; i < 18; i++) {
        if (g_LgpFileHandles.fileHandles[i] != -1) {
            FileCloseLL(g_LgpFileHandles.fileHandles[i]);
            g_LgpFileHandles.fileHandles[i] = 0;
        }
        if (g_LgpTocBlocks[i].entries != 0) {
            // STRING: FF7 0x0090d230
            GameHeapFree(g_LgpTocBlocks[i].entries, "C:\\lib\\src\\file\\is_lib.cpp", 762);
            g_LgpTocBlocks[i].entries = 0;
        }
        if (g_LgpLookupTables[i] != 0) {
            // STRING: FF7 0x0090d24c
            GameHeapFree(g_LgpLookupTables[i], "C:\\lib\\src\\file\\is_lib.cpp", 767);
            g_LgpLookupTables[i] = 0;
        }
    }
    FreeAllLgpConflictTables();
    return 1;
}

/*
 * Frees conflict tables across all LGP archive slots.
 */
// FUNCTION: FF7 0x00676172
void __cdecl FreeAllLgpConflictTables(void)
{
    int archive;
    int entryIdx;

    for (archive = 0; archive < 18; archive++) {
        for (entryIdx = 0; entryIdx < g_LgpConflictCounts[archive]; entryIdx++) {
            if (g_LgpConflictTable[archive][entryIdx].dirs != 0) {
                GameHeapFree(g_LgpConflictTable[archive][entryIdx].dirs, IsLibCpp_FileLine224(), 0x224);
                g_LgpConflictTable[archive][entryIdx].dirs = 0;
            }
        }
    }
}

/*
 * Returns low-level file descriptor for specified LGP archive slot.
 */
// FUNCTION: FF7 0x0067620f
int __cdecl GetLgpArchiveFd(int archiveIndex)
{
    if (archiveIndex >= 18) {
        return 0;
    } else {
        return g_LgpFileHandles.fileHandles[archiveIndex];
    }
}

/*
 * Seeks to an absolute byte offset within an LGP archive.
 */
// FUNCTION: FF7 0x00676228
int __cdecl SeekLgpArchive(int offset, int archiveIndex)
{
    int unused; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */

    if (archiveIndex >= 18) {
        return 0;
    } else if (g_LgpFileHandles.fileHandles[archiveIndex] == 0) {
        return 0;
    } else {
        FileSeekLL(g_LgpFileHandles.fileHandles[archiveIndex], 0, SEEK_SET);
        FileSeekLL(g_LgpFileHandles.fileHandles[archiveIndex], offset, SEEK_CUR);
        if (FileTellLL(g_LgpFileHandles.fileHandles[archiveIndex]) != offset) {
            FileSeekLL(g_LgpFileHandles.fileHandles[archiveIndex], offset, SEEK_SET);
            FileTellLL(g_LgpFileHandles.fileHandles[archiveIndex]);
        }
        return 1;
    }
}

/*
 * Seeks relative to current offset in an LGP archive.
 */
// FUNCTION: FF7 0x006762c7
int __cdecl SeekLgpArchiveRelative(int offset, int archiveIndex)
{
    FileSeekLL(g_LgpFileHandles.fileHandles[archiveIndex], offset, SEEK_CUR);
    return 1;
}

/*
 * Returns the size in bytes of the LGP entry at archive offset `offset` (a `FindLgpFile` result)
 * in slot `archiveIndex`: `SeekLgpArchive` moves to the entry, the 20-byte name is skipped and
 * the u32 after it is read. Callers allocate the buffer for `ReadLgpEntry` from it. Returns 0
 * when the slot is 18 or more or holds no file.
 */
// FUNCTION: FF7 0x006762ea
int __cdecl GetLgpEntrySize(int offset, int archiveIndex)
{
    unsigned int size;

    size = 0;
    if (SeekLgpArchive(offset, archiveIndex) != 0) {
        FileSeekLL(g_LgpFileHandles.fileHandles[archiveIndex], 0x14, SEEK_CUR);
        FileReadU32(g_LgpFileHandles.fileHandles[archiveIndex], &size);
    }
    return size;
}

/*
 * Reads `size` bytes of the data of the LGP entry at archive offset `file` (a `FindLgpFile`
 * result) in slot `lgp` into `buf`: `SeekLgpArchive` moves to the entry and the 0x18-byte entry
 * header (20-byte name, u32 size) is skipped. Returns 0 when `buf` is null or the read comes up
 * short, otherwise 1.
 *
 * Defect: when the seek fails (slot 18 or more, or no file in the slot) nothing is read and it
 * still returns 1.
 */
// FUNCTION: FF7 0x0067633e
int __cdecl ReadLgpEntry(void *file, int lgp, void *buf, int size)
{
    if (buf == 0) {
        return 0;
    }
    if (SeekLgpArchive((int)file, lgp) != 0) {
        FileSeekLL(g_LgpFileHandles.fileHandles[lgp], 0x18, SEEK_CUR);
        /* Retail caller treats `FileReadBytes` return (`short`) as `int` for caller zero/sign extension. */
        if ((((int(__cdecl *)(int, void *, int))FileReadBytes)(g_LgpFileHandles.fileHandles[lgp], buf, size) & 0xFFFF) == 0) {
            return 0;
        }
    }
    return 1;
}

/*
 * Reads bytes from LGP archive stream into memory buffer.
 */
// FUNCTION: FF7 0x006763a5
int __cdecl ReadLgpArchiveStream(int archiveIndex, void *buffer, int size)
{
    if (buffer == 0) {
        return 0;
    /* Retail caller treats `FileReadBytes` return (`short`) as `int` for caller zero/sign extension. */
    } else if ((((int(__cdecl *)(int, void *, int))FileReadBytes)(g_LgpFileHandles.fileHandles[archiveIndex], buffer, size) & 0xFFFF) == 0) {
        return 0;
    } else {
        return 1;
    }
}
} /* extern "C" */
