// src/ff7/src/battle/battle_lzs_decompress.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Decompresses LZS-compressed battle data, with the stream header giving the input range.

#include <stdio.h>
#include <windows.h>
#include "ff7/api/battle.h"
#include "lib/api/graphics.h"
#include "ff7/types/battle.h"
/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
extern "C" {

#pragma intrinsic(memset)

// Decompresses an LZS stream whose first dword supplies the input endpoint base plus size plus
// three, consuming flag bytes low bit first. Set bits copy literals; clear bits read a twelve-bit
// backreference and length three through eighteen, deriving a source relative to output with a
// 4096-byte wrap and 0xFEE bias. Zero-fills source positions preceding output start, then performs
// an overlapping forward copy. Checks input only before the flag byte, a literal and the first
// backreference byte, so the second backreference byte is read unchecked; the output has no
// bound guard.
// FUNCTION: FF7 0x0041cf10
void __cdecl LzsDecompress(void *inputStream, void *output)
{
    int flags;
    int copySrc;
    int lenByte;
    int flagCnt;
    int outputStart;
    int backDisp;
    int inputEnd;
    int copyEnd;

    flagCnt = 0;
    inputEnd = (int)inputStream + *(int *)inputStream + 3;
    inputStream = (void *)((char *)inputStream + 4);
    outputStart = (int)output;
    while (1 != 0) { /* Constant condition kept for /Od branch layout; see lib/codegen.h. */
        if (flagCnt == 0) {
            flagCnt = 8;
            if ((unsigned int)inputStream >= (unsigned int)inputEnd) {
                return;
            }
            flags = *(unsigned char *)inputStream;
            inputStream = (void *)((char *)inputStream + 1);
        }
        if ((flags & 1) != 0) {
            if ((unsigned int)inputStream >= (unsigned int)inputEnd) {
                return;
            }
            *(char *)output = *(char *)inputStream;
            output = (void *)((char *)output + 1);
            inputStream = (void *)((char *)inputStream + 1);
        } else {
            if ((unsigned int)inputStream >= (unsigned int)inputEnd) {
                return;
            }
            backDisp = *(unsigned char *)inputStream;
            inputStream = (void *)((char *)inputStream + 1);
            lenByte = *(unsigned char *)inputStream;
            inputStream = (void *)((char *)inputStream + 1);
            backDisp |= (lenByte & 0xF0) << 4;
            copyEnd = (int)output + (lenByte & 0xF) + 3;
            copySrc = (int)output - ((int)output - outputStart - (backDisp - 0xFEE) & 0xFFF);
            while ((unsigned int)copySrc < (unsigned int)outputStart) {
                *(char *)output = 0;
                output = (void *)((char *)output + 1);
                copySrc++;
            }
            while ((unsigned int)output < (unsigned int)copyEnd) {
                *(char *)output = *(char *)copySrc;
                output = (void *)((char *)output + 1);
                copySrc++;
            }
        }
        flags >>= 1;
        flagCnt--;
    }
}
} /* extern "C" */
