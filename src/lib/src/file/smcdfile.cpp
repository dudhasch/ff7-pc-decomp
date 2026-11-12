// Original translation unit: C:\Lib\src\file\smcdfile.cpp.
/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/api/mem.h"
#include <string.h>
#include "lib/api/file.h"
/* Retail CD-stream helper calls LzsDecompress directly in the battle LZS module. */
#include "ff7/api/battle.h"
extern "C" {

#pragma intrinsic(memcpy)

/*
 * Loads a block from a disk file into memory. If size is zero, the entire file
 * buffer is returned. Otherwise, allocates a buffer on the game heap, copies
 * the specified chunk, invokes the callback, and frees the raw disk buffer.
 */
// FUNCTION: FF7 0x00419210
int __cdecl LoadFileBlock(int fileId, int offset, int size, void **outBuffer, void(__cdecl *callback)(void))
{
    struct {
        void *data;
        int length;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    frame.data = LoadDiskFile(&frame.length, (void *)fileId);
    if (frame.data != 0) {
        if (size == 0) {
            size = frame.length;
            *outBuffer = frame.data;
            if (callback != 0)
                callback();
        } else {
            // STRING: FF7 0x007c0a68
            (*outBuffer) = GameHeapAllocRaw(size, "C:\\Lib\\src\\file\\smcdfile.cpp", 68);
            if (*outBuffer != 0) {
                memcpy(*outBuffer, (char *)frame.data + offset, size);
                if (callback != 0)
                    callback();
            } else {
                size = 0;
                *outBuffer = 0;
            }
            // STRING: FF7 0x007c0a88
            GameHeapFree(frame.data, "C:\\Lib\\src\\file\\smcdfile.cpp", 81);
        }
    } else {
        size = 0;
        *outBuffer = 0;
    }
    return size;
}

/*
 * Loads an LZS-compressed file from disk, allocates an uncompressed output buffer
 * on the game heap, decompresses the data, invokes the callback, and frees the
 * raw disk buffer.
 */
// FUNCTION: FF7 0x004192d9
int __cdecl LoadFileBlockLzs(int fileId, int uncompressedSize, void **outBuffer, void(__cdecl *callback)(void))
{
    struct {
        void *data;
        int length;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    frame.data = LoadDiskFile(&frame.length, (void *)fileId);
    if (frame.data != 0) {
        // STRING: FF7 0x007c0aa8
        (*outBuffer) = GameHeapAllocRaw(uncompressedSize, "C:\\Lib\\src\\file\\smcdfile.cpp", 128);
        LzsDecompress(frame.data, *outBuffer);
        if (callback != 0)
            callback();
        // STRING: FF7 0x007c0ac8
        GameHeapFree(frame.data, "C:\\Lib\\src\\file\\smcdfile.cpp", 136);
    } else {
        *outBuffer = 0;
    }
    return frame.length;
}
} /* extern "C" */
