// src/lib/src/file/file.cpp
// Original: C:\lib\src\file\file.cpp
// Contents: Opens, reads, writes and closes game files through a file context, with optional debug
//   logging and helpers that load whole files into memory.
/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/types/file.h"
#include "lib/enums/file.h"
#include "lib/types/graphics.h"
#include "lib/codegen.h"
#include "lib/api/file.h"
#include "lib/api/graphics.h"
#include "lib/api/mem.h"
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <direct.h>
#include "lib/crt.h"
#include <string.h>
#include <stdio.h>
extern "C" {

// clang-format off
// GLOBAL: FF7 0x00db2bc8
int g_FileDebugLogEnabled = 0;
unsigned char g_rawData_00db2bcc[4] = {0};
// GLOBAL: FF7 0x00db2bd0
struct GfxDebugLogTarget *g_FileDebugLogTarget = 0;
// clang-format on
// The optimize("p") pairs in this file set /Op per function, as the retail build did (lib/codegen.h, "/Op pairs").
#pragma optimize("p", off)

/*
 * Returns whether file debug logging is currently enabled.
 */
// FUNCTION: FF7 0x00681ff0
int __cdecl GetFileDebugLogEnabled(void)
{
    return g_FileDebugLogEnabled;
}
#pragma optimize("p", on)
#pragma optimize("p", off)

/*
 * Sets the file debug logging enabled flag.
 */
// FUNCTION: FF7 0x00681ffa
void __cdecl SetFileDebugLogEnabled(int enabled)
{
    g_FileDebugLogEnabled = enabled;
}
#pragma optimize("p", on)
#pragma optimize("p", off)

/*
 * Enables file debug logging and creates the debug log target.
 */
// FUNCTION: FF7 0x00682007
void __cdecl EnableFileDebugLog(int logPath)
{
    if (logPath != 0) {
        _unlink((const char *)logPath);
        g_FileDebugLogEnabled = 1;
        g_FileDebugLogTarget = (GfxDebugLogTarget *)Gfx_CreateDebugLogTargetTruncating(logPath);
        if (g_FileDebugLogTarget != 0) {
            g_FileDebugLogTarget->enabled = 0;
        }
    }
}
#pragma optimize("p", on)
#pragma optimize("p", off)

/*
 * Disables file debug logging and destroys the debug log target.
 */
// FUNCTION: FF7 0x0068204f
void __cdecl DisableFileDebugLog(void)
{
    Gfx_DestroyDebugLogTarget(g_FileDebugLogTarget);
    g_FileDebugLogEnabled = 0;
    g_FileDebugLogTarget = 0;
}
#pragma optimize("p", on)
#pragma optimize("p", off)

/*
 * Initializes a file context structure with the specified mode.
 */
// FUNCTION: FF7 0x00682076
void __cdecl InitFileContext(int mode, void *context)
{
    ZeroFillMemory32(16, context);
    ((FileOpenRequest *)context)->mode = mode;
}
#pragma optimize("p", on)
/* Period MSVCRT low-level POSIX I/O entry points. */
extern int __cdecl read(int, void *, unsigned int); /* Period MSVCRT _read entry point. */


// Original empty format string: one NUL byte, used only by OpenFileEx.
// GLOBAL: FF7 0x00db2bd4
char g_FileEmptyString[1] = {0};

#pragma optimize("p", off)

/*
 * Closes a file `OpenFileEx` opened and frees its 0x18-byte handle. A disk file's descriptor is
 * closed unless it is -1; an archive file has nothing to close, since it reads through the
 * archive slot's shared descriptor. A null `file` is ignored.
 */
// FUNCTION: FF7 0x00682091
void __cdecl CloseFileEx(FileHandle *file)
{
    if (file) {
        if (file->request.fromArchive) { /* empty first arm: lib/codegen.h */
        } else {
            if (file->handle != -1)
                close(file->handle);
        }
        // STRING: FF7 0x0090f690
        GameHeapFree(file, "C:\\lib\\src\\file\\file.cpp", 94);
    }
}
#pragma optimize("p", on)
#pragma optimize("p", off)

/*
 * Opens file `fileName` as `request` says and returns a new 0x18-byte handle (the name, the disk
 * descriptor or LGP entry offset, then a copy of the 16-byte request), or 0 when it fails.
 * `ReadFileData`, `WriteFileData`, `GetFileSizeEx` and `CloseFileEx` take that handle.
 *
 * With the file debug log on (`EnableFileDebugLog`) it first logs the mode ("READ", "READ TEXT",
 * "WRITE", "CREATE"), the current directory and the name. A `request->callback` maps the name
 * into a 200-byte buffer, and the mapped name is opened instead.
 *
 * From an archive (`request->fromArchive` set), mode 0 (read) makes `request->archiveId` the
 * current archive, finds the name with `FindLgpFile` and seeks the slot's descriptor to the
 * entry's data (offset + 0x18); a missing name ("OFFSET ERROR") or failed seek ("SEEK ERROR")
 * prints "COULD NOT OPEN FILE" and returns 0. Modes 2 and 3 only print "FILE SYSTEM IS READ
 * ONLY" and still return the handle. From disk it calls `open` with the mode's flags: 0 read
 * (0x8000, binary), 1 read text (0x4000), 2 write (0x8102, binary read/write, created if missing),
 * 3 create (0x8302, the same and truncated), with permission 0x100 (read) for the read modes and
 * 0x180 (read/write) for the others; descriptor -1 prints "COULD NOT OPEN FILE" and returns 0.
 *
 * Defects: from an archive, mode 1 (read text) is not handled and returns a handle with entry
 * offset 0 and no seek; from disk, a mode above 3 leaves the open flags and permission
 * uninitialised.
 */
// FUNCTION: FF7 0x006820d2
FileHandle *__cdecl OpenFileEx(FileOpenRequest *request, const char *fileName)
{
    struct {
        int permissions;
        int flags;
        int failed;
        const char *archiveName;
        FileHandle *file;
        char mappedName[200];
        char message[200];
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    if (g_FileDebugLogEnabled != 0) {
        switch (request->mode) {
        case FILE_OPEN_READ:
            // STRING: FF7 0x0090f6ac
            sprintf(frame.message, "READ \n");
            break;
        case FILE_OPEN_READ_TEXT:
            // STRING: FF7 0x0090f6b4
            sprintf(frame.message, "READ TEXT \n");
            break;
        case FILE_OPEN_WRITE:
            // STRING: FF7 0x0090f6c0
            sprintf(frame.message, "WRITE \n");
            break;
        case FILE_OPEN_CREATE:
            // STRING: FF7 0x0090f6c8
            sprintf(frame.message, "CREATE \n");
            break;
        default:
            sprintf(frame.message, g_FileEmptyString);
        }
        WriteDebugLog(frame.message, g_FileDebugLogTarget);
        // STRING: FF7 0x0090f6d4
        if (ChangeDirectorySaving(".", frame.mappedName) != 0) {
            // STRING: FF7 0x0090f6d8
            sprintf(frame.message, "%s \n", frame.mappedName);
            WriteDebugLog(frame.message, g_FileDebugLogTarget);
        }
        // STRING: FF7 0x0090f6e0
        sprintf(frame.message, "%s \n", fileName);
        WriteDebugLog(frame.message, g_FileDebugLogTarget);
    }
    if (request->callback != 0) {
        /* Invokes untyped callback slot `request->callback` with its runtime (const char *, char *) -> int signature. */
        ((int(__cdecl *)(const char *, char *)) request->callback)(fileName, frame.mappedName);
    }
    frame.file = 0;
    if (request->fromArchive != 0) {
        // STRING: FF7 0x0090f6e8
        frame.file = (FileHandle *)GameHeapAlloc(1, 24, "C:\\lib\\src\\file\\file.cpp", 147);
        if (frame.file != 0) {
            frame.file->name = (char *)fileName;
            frame.file->request = *request;
            switch (request->mode) {
            case FILE_OPEN_READ:
                if (request->callback != 0) {
                    frame.archiveName = frame.mappedName;
                } else {
                    frame.archiveName = fileName;
                }
                frame.failed = 1;
                SetCurrentLgpArchiveId(request->archiveId);
                frame.file->handle = (int)FindLgpFile((char *)frame.archiveName, request->archiveId);
                if (frame.file->handle != 0) {
                    if (SeekLgpArchive(frame.file->handle + 0x18, request->archiveId) != 0) {
                        frame.failed = 0;
                    } else {
                        // STRING: FF7 0x0090f704
                        sprintf(frame.message, "SEEK ERROR: %s \n", fileName);
                        Gfx_DebugPrint(frame.message);
                    }
                } else {
                    if (request->callback != 0) {
                        // STRING: FF7 0x0090f718
                        sprintf(frame.message, "OFFSET ERROR: %s %s \n", fileName, frame.archiveName);
                    } else {
                        // STRING: FF7 0x0090f730
                        sprintf(frame.message, "OFFSET ERROR: %s \n", fileName);
                    }
                    Gfx_DebugPrint(frame.message);
                }
                if (frame.failed != 0) {
                    // STRING: FF7 0x0090f744
                    sprintf(frame.message, "ERROR: COULD NOT OPEN FILE %s\n", fileName);
                    Gfx_DebugPrint(frame.message);
                    CloseFileEx(frame.file);
                    frame.file = 0;
                }
                break;
            case FILE_OPEN_WRITE:
            case FILE_OPEN_CREATE:
                // STRING: FF7 0x0090f764
                sprintf(frame.message, "ERROR: CANT CREATE/WRITE TO FILE %s; FILE SYSTEM IS READ ONLY \n", fileName);
                Gfx_DebugPrint(frame.message);
            }
        }
    } else {
        switch (request->mode) {
        case FILE_OPEN_READ:
            frame.flags = _O_RDONLY | _O_BINARY;
            frame.permissions = _S_IREAD;
            break;
        case FILE_OPEN_READ_TEXT:
            frame.flags = _O_RDONLY | _O_TEXT;
            frame.permissions = _S_IREAD;
            break;
        case FILE_OPEN_WRITE:
            frame.flags = _O_RDWR | _O_CREAT | _O_BINARY;
            frame.permissions = _S_IREAD | _S_IWRITE;
            break;
        case FILE_OPEN_CREATE:
            frame.flags = _O_RDWR | _O_CREAT | _O_TRUNC | _O_BINARY;
            frame.permissions = _S_IREAD | _S_IWRITE;
        }
        // STRING: FF7 0x0090f7a4
        frame.file = (FileHandle *)GameHeapAlloc(1, 24, "C:\\lib\\src\\file\\file.cpp", 256);
        if (frame.file != 0) {
            frame.file->name = (char *)fileName;
            if (request->callback != 0) {
                frame.file->handle = open(frame.mappedName, frame.flags, frame.permissions);
            } else {
                frame.file->handle = open(fileName, frame.flags, frame.permissions);
            }
            frame.file->request = *request;
            if (frame.file->handle == -1) {
                // STRING: FF7 0x0090f7c0
                sprintf(frame.message, "ERROR: COULD NOT OPEN FILE %s\n", fileName);
                Gfx_DebugPrint(frame.message);
                CloseFileEx(frame.file);
                frame.file = 0;
            }
        }
    }
    return frame.file;
}
#pragma optimize("p", on)
#pragma optimize("p", off)

/*
 * Opens disk file `fileName` in `mode` (an `enum FileOpenMode`; see `OpenFileEx`)
 * with a zeroed request on the stack: no archive and no name callback. The menu opens the save
 * files this way, and battle the files it writes from its model data. Returns the handle
 * `OpenFileEx` returns, or 0.
 */
// FUNCTION: FF7 0x006825d7
int __cdecl OpenFileSimple(int mode, void *fileName)
{
    FileOpenRequest fileContext;

    InitFileContext(mode, &fileContext);
    return (int)OpenFileEx(&fileContext, (const char *)fileName);
}
#pragma optimize("p", on)
#pragma optimize("p", off)

/*
 * Reads exactly `size` bytes from open file `file` into `buffer` and returns 1, or 0 when the
 * read comes up short or `file` is null. An archive file reads with `ReadLgpArchiveStream` from
 * wherever the slot's shared descriptor stands (after `OpenFileEx`, the entry's data); a disk
 * file reads with `read` and prints "COULD NOT READ FROM FILE" on a short read.
 */
// FUNCTION: FF7 0x00682601
int __cdecl ReadFileData(int size, void *buffer, FileHandle *file)
{
    int curBytesRead, resultScalar;
    char buf[200];
    resultScalar = 0;
    if (file) {
        if (file->request.fromArchive)
            resultScalar = ReadLgpArchiveStream(file->request.archiveId, buffer, size);
        else {
            curBytesRead = read(file->handle, buffer, size);
            if (curBytesRead == size)
                resultScalar = 1;
            else {
                // STRING: FF7 0x0090f7e0
                sprintf(buf, "ERROR: COULD NOT READ FROM FILE %s\n", file->name);
                Gfx_DebugPrint(buf);
            }
        }
    }
    return resultScalar;
}
#pragma optimize("p", on)
#pragma optimize("p", off)

/*
 * Reads up to `size` bytes from open file `file` into `buffer` and returns the count read, or -1
 * for a null `file` or a failed disk read (which prints "COULD NOT READ FROM FILE"). A disk file
 * may return fewer bytes than asked, for example at the end of the file.
 *
 * Defect: for an archive file it ignores the result of `ReadLgpArchiveStream` and always returns
 * `size`, so a short read looks complete.
 */
// FUNCTION: FF7 0x00682697
int __cdecl ReadFileDataPartial(int size, void *buffer, FileHandle *file)
{
    int p;
    char buf[200];
    p = -1;
    if (file) {
        if (file->request.fromArchive) {
            ReadLgpArchiveStream(file->request.archiveId, buffer, size);
            p = size;
        } else {
            p = read(file->handle, buffer, size);
            if (p == -1) {
                // STRING: FF7 0x0090f804
                sprintf(buf, "ERROR: COULD NOT READ FROM FILE %s\n", file->name);
                Gfx_DebugPrint(buf);
            }
        }
    }
    return p;
}
#pragma optimize("p", on)
#pragma optimize("p", off)

/*
 * Writes size bytes of buffer to the open file handle (a FileHandle *); returns 1 when all were
 * written, 0 for an archive member, a null handle or a short write (which it logs).
 */
// FUNCTION: FF7 0x00682725
int __cdecl WriteFileData(int size, const void *buffer, FileHandle *handle)
{
    int written;
    char msg[200];
    int ok;

    ok = 0;
    if (handle != 0 && size > 0) {
        if (handle->request.fromArchive != 0) { /* empty first arm: lib/codegen.h */
        } else {
            written = write(handle->handle, buffer, size);
            if (written == size) {
                ok = 1;
            } else {
                // STRING: FF7 0x0090f828
                sprintf(msg, "ERROR: COULD NOT WRITE TO FILE %s\n", handle->name);
                Gfx_DebugPrint(msg);
            }
        }
    }
    return ok;
}
#pragma optimize("p", on)
#pragma optimize("p", off)

/*
 * Reads file contents into a newly allocated heap buffer.
 */
// FUNCTION: FF7 0x006827a7
void *__cdecl ReadFileIntoNewBufferEx(int extra, int size, int count, FileHandle *file)
{
    void *rec;
    rec = 0;
    if (count > 0) {
        // STRING: FF7 0x0090f84c
        rec = GameHeapAllocRaw(size * count + extra, "C:\\lib\\src\\file\\file.cpp", 406);
        if (rec) {
            if (ReadFileData(size * count, rec, file)) { /* empty first arm: lib/codegen.h */
            } else {
                // STRING: FF7 0x0090f868
                GameHeapFree(rec, "C:\\lib\\src\\file\\file.cpp", 415);
                rec = 0;
            }
        }
    }
    return rec;
}
#pragma optimize("p", on)
#pragma optimize("p", off)

/*
 * ReadFileIntoNewBufferEx with no extra bytes: reads count records of size bytes from file into a
 * new heap buffer. Returns the buffer Ex returns.
 */
// FUNCTION: FF7 0x00682820
int __cdecl ReadFileIntoNewBuffer(int size, int count, FileHandle *file)
{
    return (int)ReadFileIntoNewBufferEx(0, size, count, file);
}
#pragma optimize("p", on)
#pragma optimize("p", off)

/*
 * Returns the total byte size of an open file.
 * A disk file's size comes from `filelength`, the POSIX name: OLDNAMES.LIB resolves it to
 * `_filelength`, and loading that alias member is what puts LIBCMT's flength.obj after
 * thread.obj, as in retail.
 */
// FUNCTION: FF7 0x0068283b
int __cdecl GetFileSizeEx(void *handle)
{
    int size;
    int entryHandle;

    size = 0;
    if (handle != 0) {
        if (((FileHandle *)handle)->request.fromArchive != 0) {
            entryHandle = ((FileHandle *)handle)->handle;
            size = GetLgpEntrySize(entryHandle, ((FileHandle *)handle)->request.archiveId);
        } else {
            size = filelength(((FileHandle *)handle)->handle);
        }
    }
    return size;
}
#pragma optimize("p", on)
#pragma optimize("p", off)

/*
 * Returns the current read/write position of an open file.
 */
// FUNCTION: FF7 0x00682891
int __cdecl GetFilePosition(FileHandle *file)
{
    int p;
    char buf[200];
    p = 0;
    if (file) {
        if (file->request.fromArchive) { /* empty first arm: lib/codegen.h */
        } else {
            p = tell(file->handle);
            if (p == -1) {
                // STRING: FF7 0x0090f884
                sprintf(buf, "ERROR: COULD NOT GET FILE POINTER%s\n", file->name);
                Gfx_DebugPrint(buf);
            }
        }
    }
    return p;
}
#pragma optimize("p", on)
#pragma optimize("p", off)

/*
 * Sets the file pointer position of an open file.
 */
// FUNCTION: FF7 0x006828fa
void __cdecl SetFilePosition(FileHandle *file, int position)
{
    int p;
    char buf[200];
    if (file) {
        if (file->request.fromArchive) { /* empty first arm: lib/codegen.h */
        } else {
            p = lseek(file->handle, position, SEEK_SET);
            if (p == -1) {
                // STRING: FF7 0x0090f8ac
                sprintf(buf, "ERROR: COULD NOT SEEK FILE %s\n", file->name);
                Gfx_DebugPrint(buf);
            }
        }
    }
}
#pragma optimize("p", on)
#pragma optimize("p", off)

/*
 * Loads an entire file into memory and returns its buffer pointer.
 */
// FUNCTION: FF7 0x0068295f
void *__cdecl LoadWholeFileEx(void *context, int extra, int *size, char *name)
{
    int length;
    void *buffer;
    FileHandle *file;
    buffer = 0;
    file = OpenFileEx((FileOpenRequest *)context, name);
    if (file) {
        length = GetFileSizeEx(file);
        if (length)
            buffer = ReadFileIntoNewBufferEx(extra, length, 1, file);
        *size = length;
        CloseFileEx(file);
    }
    return buffer;
}
#pragma optimize("p", on)
#pragma optimize("p", off)

/*
 * Loads the whole disk file fileName into a new heap buffer with extra spare bytes, through a
 * temporary file context, and stores its size at sizeOut. Returns the buffer.
 */
// FUNCTION: FF7 0x006829ce
void *__cdecl LoadDiskFileEx(int extra, void *sizeOut, void *fileName)
{
    int fileContext[4];

    InitFileContext(0, fileContext);
    return LoadWholeFileEx(fileContext, extra, (int *)sizeOut, (char *)fileName);
}
#pragma optimize("p", on)
#pragma optimize("p", off)

/*
 * Loads the whole file `fileName`, opened as `context` (a `FileOpenRequest`) says, from disk or
 * from an LGP archive, into a new heap buffer and returns it, or 0. The size (`GetFileSizeEx`) is
 * stored at `sizeOut` once the file is open, even when the read then fails; a file that does not
 * open leaves `sizeOut` unwritten. Battle loads its model descriptors this way.
 * `LoadWholeFileEx` with no spare bytes.
 */
// FUNCTION: FF7 0x006829fe
void *__cdecl LoadWholeFile(void *context, int *sizeOut, char *fileName)
{
    return LoadWholeFileEx(context, 0, sizeOut, fileName);
}
#pragma optimize("p", on)
#pragma optimize("p", off)

/*
 * Loads the whole disk file fileName into a new heap buffer and stores its size at sizeOut;
 * returns the buffer LoadDiskFileEx returns.
 */
// FUNCTION: FF7 0x00682a19
void *__cdecl LoadDiskFile(void *sizeOut, void *fileName)
{
    return LoadDiskFileEx(0, sizeOut, fileName);
}
#pragma optimize("p", on)
#pragma optimize("p", off)

/*
 * Writes `size` bytes of `data` to disk file `fileName`, created or truncated (`OpenFileSimple`
 * mode 3), and closes it; returns 1 when all bytes were written. Battle writes the textures and
 * info files it unpacks from its model data this way. A `size` of 0 or less creates an empty
 * file and returns 0, since `WriteFileData` writes nothing for it.
 */
// FUNCTION: FF7 0x00682a30
int __cdecl WriteWholeFile(int size, char *fileName, void *data)
{
    int ok;
    FileHandle *pHandle;

    ok = 0;
    pHandle = (FileHandle *)OpenFileSimple(FILE_OPEN_CREATE, fileName);
    if (pHandle != 0) {
        if (WriteFileData(size, data, pHandle) != 0) {
            ok = 1;
        }
        CloseFileEx(pHandle);
    }
    return ok;
}
#pragma optimize("p", on)
#pragma optimize("p", off)

/*
 * Frees a file record and its associated directory and name strings.
 */
// FUNCTION: FF7 0x00682a86
void __cdecl FileFreeRecord(FileRecord *record)
{
    char buf[200];
    if (record) {
        if (DEBUG_TRACE) {
            // STRING: FF7 0x0090f8cc
            sprintf(buf, "FREEING FILE %s %s\n", record->directory, record->name);
            Gfx_DebugPrint(buf);
        }
        // STRING: FF7 0x0090f8e0
        GameHeapFree(record->directory, "C:\\lib\\src\\file\\file.cpp", 590);
        // STRING: FF7 0x0090f8fc
        GameHeapFree(record->name, "C:\\lib\\src\\file\\file.cpp", 591);
        // STRING: FF7 0x0090f918
        GameHeapFree(record, "C:\\lib\\src\\file\\file.cpp", 592);
    }
}
#pragma optimize("p", on)
#pragma optimize("p", off)

/*
 * Callback wrapper that releases a file record.
 */
// FUNCTION: FF7 0x00682b18
void __cdecl FileFreeRecordCallback(int value)
{
    FileFreeRecord((FileRecord *)value);
}
#pragma optimize("p", on)
#pragma optimize("p", off)

/*
 * Allocates and initializes a new file record structure.
 */
// FUNCTION: FF7 0x00682b29
FileRecord *__cdecl FileCreateRecord(int kind, char *name, char *directory)
{
    FileRecord *p;
    char buf[200];
    // STRING: FF7 0x0090f934
    p = (FileRecord *)GameHeapAlloc(1, 32, "C:\\lib\\src\\file\\file.cpp", 606);
    if (p) {
        p->kind = kind;
        p->name = GameHeapStrDup(name);
        if (directory) {
            p->directory = GameHeapStrDup(directory);
        } else {
            if (getcwd(buf, 200))
                p->directory = GameHeapStrDup(buf);
            else
                p->directory = 0;
        }
    }
    return p;
}
#pragma optimize("p", on)
#pragma intrinsic(strlen)

/*
 * Compares two file records for matching kind, name, and directory.
 */
// FUNCTION: FF7 0x00682bc5
int __cdecl FileRecordMatchesKindNameDirectory(FileRecord *recordA, FileRecord *recordB)
{
    int match;

    match = 0;
    if (recordA != 0 && recordB != 0 && recordA->kind == recordB->kind && recordA->name != 0 && recordB->name != 0 && recordA->directory != 0 && recordB->directory != 0) {
        if (stricmp(recordA->name, recordB->name) == 0) {
            if (stricmp(recordA->directory, recordB->directory) == 0) {
                match = 1;
            }
        }
    }
    return match;
}

/*
 * Compares two file records and discards the result.
 */
// FUNCTION: FF7 0x00682c4e
void __cdecl FileRecordMatchesCompare(FileRecord *lhs, FileRecord *rhs)
{
    FileRecordMatchesKindNameDirectory(lhs, rhs);
}

/*
 * Checks whether an indexed entry matches the specified key.
 */
// FUNCTION: FF7 0x00682c63
int __cdecl IndexedEntryKeyMatches(int index, int key, void *table)
{
    int result;

    result = 0;
    if (table != 0 && *(int *)((char *)table + index * 8 + 0x14) == key) {
        result = 1;
    }
    return result;
}

/*
 * Copies a filename and strips its file extension.
 */
// FUNCTION: FF7 0x00682c91
void __cdecl StripFileExtension(const char *src, char *dst)
{
    int length, index;
    sprintf(dst, src);
    length = strlen(src);
    if (length > 0) {
        for (index = length - 1; index >= 0; index--) {
            if (dst[index] == '.') {
                dst[index] = 0;
                break;
            }
        }
    }
}

/*
 * Generates an indexed asset filename with formatted suffix.
 */
// FUNCTION: FF7 0x00682cfa
void __cdecl MakeIndexedAssetName(char extChar, int index, const char *fileName, char *out)
{
    char base[200];
    StripFileExtension(fileName, base);
    // STRING: FF7 0x0090f950
    sprintf(out, "%s.%c%02d", base, extChar, index);
}

/*
 * Formats an asset name with a new extension.
 */
// FUNCTION: FF7 0x00682d3b
void __cdecl MakeExtensionAssetName(const char *ext, const char *fileName, char *out)
{
    char base[200];
    StripFileExtension(fileName, base);
    // STRING: FF7 0x0090f95c
    sprintf(out, "%s.%s", base, ext);
}
} /* extern "C" */
