// src/lib/src/graphics/matrix_lib.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Matrix routines written as assembly or x87 code: identity, multiply, concatenate, rotate and
//   transform.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/api/graphics.h"
#include "lib/api/mem.h"
#include "lib/types/polygon.h"
#include "lib/api/polygon.h"
extern "C" {

/*
 * Assembly writes float one at the four diagonal4x4 entries and zero to all twelve others, forming
 * identity across 64 bytes.
 */
// FUNCTION: FF7 0x0066c4f0
void __cdecl SetIdentityMatrix4x4(void *matrix)
{
    float one;
    one = 1.0f;
    __asm {
        mov edx, matrix
        mov eax, one
        xor ecx, ecx
        mov dword ptr [edx], eax
        mov dword ptr [edx + 4], ecx
        mov dword ptr [edx + 8], ecx
        mov dword ptr [edx + 0ch], ecx
        mov dword ptr [edx + 010h], ecx
        mov dword ptr [edx + 014h], eax
        mov dword ptr [edx + 018h], ecx
        mov dword ptr [edx + 01ch], ecx
        mov dword ptr [edx + 020h], ecx
        mov dword ptr [edx + 024h], ecx
        mov dword ptr [edx + 028h], eax
        mov dword ptr [edx + 02ch], ecx
        mov dword ptr [edx + 030h], ecx
        mov dword ptr [edx + 034h], ecx
        mov dword ptr [edx + 038h], ecx
        mov dword ptr [edx + 03ch], eax
    }
}

/*
 * Assembly zeroes indices 3, 7, 11, 12, 13, 14 and writes float one at 15, preserving the
 * leading3x3 rotation entries.
 */
// FUNCTION: FF7 0x0066c53c
void __cdecl ClearMatrixTranslationAndW(void *m)
{
    float one;

    one = 1.0f;
    __asm {
        mov edx, m
        xor ecx, ecx
        mov eax, one
        mov dword ptr [edx + 0ch], ecx
        mov dword ptr [edx + 1ch], ecx
        mov dword ptr [edx + 2ch], ecx
        mov dword ptr [edx + 30h], ecx
        mov dword ptr [edx + 34h], ecx
        mov dword ptr [edx + 38h], ecx
        mov dword ptr [edx + 3ch], eax
    }
}

/*
 * Uses a scheduled x87 instruction sequence to multiply only the upper-left three-by-three blocks
 * of a and b, storing the nine row-by-column sums at output offsets 0/4/8, 0x10/0x14/0x18, and
 * 0x20/0x24/0x28. Leaves the remaining seven floats of the nominal four-by-four output untouched.
 * Reads timestamp counters before and after and computes their difference into a local eight-byte
 * buffer without exposing it. Input/output aliasing is not generally safe because stores occur
 * before all source reads.
 */
// FUNCTION: FF7 0x0066c56e
void __cdecl MultiplyMatrix4x4Core(void *a, void *b, void *out)
{
    char endTime[8];
    char elapsed[8];
    char startStamp[8];

    ReadTimestampCounter((struct Timestamp64 *)startStamp);
    __asm {
        mov edx, out
        mov eax, a
        mov ebx, b
        fld dword ptr [eax]
        fmul dword ptr [ebx]
        fld dword ptr [eax + 4]
        fmul dword ptr [ebx + 0x10]
        fld dword ptr [eax + 8]
        fmul dword ptr [ebx + 0x20]
        fld dword ptr [eax]
        fmul dword ptr [ebx + 4]
        fld dword ptr [eax + 4]
        fxch st(4)
        faddp st(3), st(0)
        fxch st(3)
        fmul dword ptr [ebx + 0x14]
        fld dword ptr [eax + 8]
        fmul dword ptr [ebx + 0x24]
        fxch st(3)
        faddp st(2), st(0)
        fld dword ptr [eax]
        fmul dword ptr [ebx + 8]
        fxch st(4)
        faddp st(1), st(0)
        fxch st(1)
        fstp dword ptr [edx]
        fld dword ptr [eax + 4]
        fmul dword ptr [ebx + 0x18]
        fxch st(1)
        faddp st(2), st(0)
        fld dword ptr [eax + 8]
        fmul dword ptr [ebx + 0x28]
        fld dword ptr [eax + 0x10]
        fxch st(3)
        fstp dword ptr [edx + 4]
        fxch st(3)
        faddp st(1), st(0)
        fxch st(1)
        fmul dword ptr [ebx]
        fld dword ptr [eax + 0x14]
        fmul dword ptr [ebx + 0x10]
        fxch st(2)
        faddp st(3), st(0)
        fld dword ptr [eax + 0x18]
        fmul dword ptr [ebx + 0x20]
        fld dword ptr [eax + 0x10]
        fxch st(4)
        fstp dword ptr [edx + 8]
        fxch st(1)
        faddp st(2), st(0)
        fxch st(2)
        fmul dword ptr [ebx + 4]
        fld dword ptr [eax + 0x14]
        fmul dword ptr [ebx + 0x14]
        fxch st(2)
        faddp st(3), st(0)
        fld dword ptr [eax + 0x18]
        fmul dword ptr [ebx + 0x24]
        fld dword ptr [eax + 0x10]
        fxch st(4)
        fstp dword ptr [edx + 0x10]
        fxch st(1)
        faddp st(2), st(0)
        fxch st(2)
        fmul dword ptr [ebx + 8]
        fld dword ptr [eax + 0x14]
        fmul dword ptr [ebx + 0x18]
        fxch st(2)
        faddp st(3), st(0)
        fld dword ptr [eax + 0x18]
        fmul dword ptr [ebx + 0x28]
        fld dword ptr [eax + 0x20]
        fxch st(4)
        fstp dword ptr [edx + 0x14]
        fxch st(1)
        faddp st(2), st(0)
        fxch st(2)
        fmul dword ptr [ebx]
        fld dword ptr [eax + 0x24]
        fmul dword ptr [ebx + 0x10]
        fxch st(2)
        faddp st(3), st(0)
        fld dword ptr [eax + 0x28]
        fmul dword ptr [ebx + 0x20]
        fld dword ptr [eax + 0x20]
        fxch st(4)
        fstp dword ptr [edx + 0x18]
        fxch st(1)
        faddp st(2), st(0)
        fxch st(2)
        fmul dword ptr [ebx + 4]
        fld dword ptr [eax + 0x24]
        fmul dword ptr [ebx + 0x14]
        fxch st(2)
        faddp st(3), st(0)
        fld dword ptr [eax + 0x28]
        fmul dword ptr [ebx + 0x24]
        fld dword ptr [eax + 0x20]
        fxch st(4)
        fstp dword ptr [edx + 0x20]
        fxch st(1)
        faddp st(2), st(0)
        fxch st(2)
        fmul dword ptr [ebx + 8]
        fld dword ptr [eax + 0x24]
        fmul dword ptr [ebx + 0x18]
        fxch st(2)
        faddp st(3), st(0)
        fld dword ptr [eax + 0x28]
        fmul dword ptr [ebx + 0x28]
        fxch st(1)
        faddp st(2), st(0)
        fxch st(2)
        fstp dword ptr [edx + 0x24]
        faddp st(1), st(0)
        fstp dword ptr [edx + 0x28]
    }
    ReadTimestampCounter((struct Timestamp64 *)endTime);
    Sub64((struct Timestamp64 *)endTime, (struct Timestamp64 *)startStamp, (struct Timestamp64 *)elapsed);
}

/*
 * Writes all 16 output floats as the product of a and the transpose of b: each result is a dot
 * product between an a row and a b row. Uses a single scheduled x87 assembly block without calls
 * or temporary output storage; overlapping output with either input can overwrite elements before
 * later loads.
 */
// FUNCTION: FF7 0x0066c6cd
void __cdecl MultiplyMatrix4x4ByTranspose(void *a, void *b, void *out)
{
    __asm {
        mov edx, out
        mov eax, a
        mov ebx, b
        fld dword ptr [eax]
        fmul dword ptr [ebx]
        fld dword ptr [eax + 4]
        fmul dword ptr [ebx + 4]
        fld dword ptr [eax + 8]
        fmul dword ptr [ebx + 8]
        fld dword ptr [eax + 0xc]
        fmul dword ptr [ebx + 0xc]
        fld dword ptr [eax]
        fxch st(4)
        faddp st(3), st(0)
        fxch st(3)
        fmul dword ptr [ebx + 0x10]
        fld dword ptr [eax + 4]
        fmul dword ptr [ebx + 0x14]
        fxch st(3)
        faddp st(2), st(0)
        fld dword ptr [eax + 8]
        fmul dword ptr [ebx + 0x18]
        fld dword ptr [eax + 0xc]
        fxch st(3)
        faddp st(5), st(0)
        fxch st(1)
        faddp st(3), st(0)
        fxch st(1)
        fmul dword ptr [ebx + 0x1c]
        fld dword ptr [eax]
        fxch st(4)
        fstp dword ptr [edx]
        fxch st(2)
        faddp st(1), st(0)
        fxch st(2)
        fmul dword ptr [ebx + 0x20]
        fld dword ptr [eax + 4]
        fmul dword ptr [ebx + 0x24]
        fxch st(3)
        faddp st(2), st(0)
        fld dword ptr [eax + 8]
        fmul dword ptr [ebx + 0x28]
        fld dword ptr [eax + 0xc]
        fxch st(3)
        fstp dword ptr [edx + 4]
        fxch st(1)
        faddp st(3), st(0)
        fxch st(1)
        fmul dword ptr [ebx + 0x2c]
        fld dword ptr [eax]
        fmul dword ptr [ebx + 0x30]
        fxch st(3)
        faddp st(2), st(0)
        fld dword ptr [eax + 4]
        fmul dword ptr [ebx + 0x34]
        fld dword ptr [eax + 8]
        fxch st(3)
        faddp st(2), st(0)
        fxch st(2)
        fmul dword ptr [ebx + 0x38]
        fld dword ptr [eax + 0xc]
        fmul dword ptr [ebx + 0x3c]
        fxch st(4)
        faddp st(3), st(0)
        fxch st(1)
        fstp dword ptr [edx + 8]
        fld dword ptr [eax + 0x10]
        fmul dword ptr [ebx]
        fxch st(2)
        faddp st(1), st(0)
        fld dword ptr [eax + 0x14]
        fmul dword ptr [ebx + 4]
        fld dword ptr [eax + 0x18]
        fxch st(2)
        faddp st(4), st(0)
        fxch st(1)
        fmul dword ptr [ebx + 8]
        fld dword ptr [eax + 0x1c]
        fmul dword ptr [ebx + 0xc]
        fxch st(3)
        faddp st(2), st(0)
        fxch st(3)
        fstp dword ptr [edx + 0xc]
        fld dword ptr [eax + 0x10]
        fmul dword ptr [ebx + 0x10]
        fxch st(1)
        faddp st(3), st(0)
        fld dword ptr [eax + 0x14]
        fmul dword ptr [ebx + 0x14]
        fld dword ptr [eax + 0x18]
        fxch st(4)
        faddp st(3), st(0)
        fxch st(3)
        fmul dword ptr [ebx + 0x18]
        fld dword ptr [eax + 0x1c]
        fmul dword ptr [ebx + 0x1c]
        fxch st(2)
        faddp st(4), st(0)
        fxch st(2)
        fstp dword ptr [edx + 0x10]
        fld dword ptr [eax + 0x10]
        fmul dword ptr [ebx + 0x20]
        fxch st(3)
        faddp st(2), st(0)
        fld dword ptr [eax + 0x14]
        fmul dword ptr [ebx + 0x24]
        fld dword ptr [eax + 0x18]
        fxch st(3)
        faddp st(2), st(0)
        fxch st(2)
        fmul dword ptr [ebx + 0x28]
        fld dword ptr [eax + 0x1c]
        fmul dword ptr [ebx + 0x2c]
        fxch st(4)
        faddp st(3), st(0)
        fxch st(1)
        fstp dword ptr [edx + 0x14]
        fld dword ptr [eax + 0x10]
        fmul dword ptr [ebx + 0x30]
        fxch st(2)
        faddp st(1), st(0)
        fld dword ptr [eax + 0x14]
        fmul dword ptr [ebx + 0x34]
        fld dword ptr [eax + 0x18]
        fxch st(2)
        faddp st(4), st(0)
        fxch st(1)
        fmul dword ptr [ebx + 0x38]
        fld dword ptr [eax + 0x1c]
        fmul dword ptr [ebx + 0x3c]
        fxch st(3)
        faddp st(2), st(0)
        fxch st(3)
        fstp dword ptr [edx + 0x18]
        fld dword ptr [eax + 0x20]
        fmul dword ptr [ebx]
        fxch st(1)
        faddp st(3), st(0)
        fld dword ptr [eax + 0x24]
        fmul dword ptr [ebx + 4]
        fld dword ptr [eax + 0x28]
        fxch st(4)
        faddp st(3), st(0)
        fxch st(3)
        fmul dword ptr [ebx + 8]
        fld dword ptr [eax + 0x2c]
        fmul dword ptr [ebx + 0xc]
        fxch st(2)
        faddp st(4), st(0)
        fxch st(2)
        fstp dword ptr [edx + 0x1c]
        fld dword ptr [eax + 0x20]
        fmul dword ptr [ebx + 0x10]
        fxch st(3)
        faddp st(2), st(0)
        fld dword ptr [eax + 0x24]
        fmul dword ptr [ebx + 0x14]
        fld dword ptr [eax + 0x28]
        fxch st(3)
        faddp st(2), st(0)
        fxch st(2)
        fmul dword ptr [ebx + 0x18]
        fld dword ptr [eax + 0x2c]
        fmul dword ptr [ebx + 0x1c]
        fxch st(4)
        faddp st(3), st(0)
        fxch st(1)
        fstp dword ptr [edx + 0x20]
        fld dword ptr [eax + 0x20]
        fmul dword ptr [ebx + 0x20]
        fxch st(2)
        faddp st(1), st(0)
        fld dword ptr [eax + 0x24]
        fmul dword ptr [ebx + 0x24]
        fld dword ptr [eax + 0x28]
        fxch st(2)
        faddp st(4), st(0)
        fxch st(1)
        fmul dword ptr [ebx + 0x28]
        fld dword ptr [eax + 0x2c]
        fmul dword ptr [ebx + 0x2c]
        fxch st(3)
        faddp st(2), st(0)
        fxch st(3)
        fstp dword ptr [edx + 0x24]
        fld dword ptr [eax + 0x20]
        fmul dword ptr [ebx + 0x30]
        fxch st(1)
        faddp st(3), st(0)
        fld dword ptr [eax + 0x24]
        fmul dword ptr [ebx + 0x34]
        fld dword ptr [eax + 0x28]
        fxch st(4)
        faddp st(3), st(0)
        fxch st(3)
        fmul dword ptr [ebx + 0x38]
        fld dword ptr [eax + 0x2c]
        fmul dword ptr [ebx + 0x3c]
        fxch st(2)
        faddp st(4), st(0)
        fxch st(2)
        fstp dword ptr [edx + 0x28]
        fld dword ptr [eax + 0x30]
        fmul dword ptr [ebx]
        fxch st(3)
        faddp st(2), st(0)
        fld dword ptr [eax + 0x34]
        fmul dword ptr [ebx + 4]
        fld dword ptr [eax + 0x38]
        fxch st(3)
        faddp st(2), st(0)
        fxch st(2)
        fmul dword ptr [ebx + 8]
        fld dword ptr [eax + 0x3c]
        fmul dword ptr [ebx + 0xc]
        fxch st(4)
        faddp st(3), st(0)
        fxch st(1)
        fstp dword ptr [edx + 0x2c]
        fld dword ptr [eax + 0x30]
        fmul dword ptr [ebx + 0x10]
        fxch st(2)
        faddp st(1), st(0)
        fld dword ptr [eax + 0x34]
        fmul dword ptr [ebx + 0x14]
        fld dword ptr [eax + 0x38]
        fxch st(2)
        faddp st(4), st(0)
        fxch st(1)
        fmul dword ptr [ebx + 0x18]
        fld dword ptr [eax + 0x3c]
        fmul dword ptr [ebx + 0x1c]
        fxch st(3)
        faddp st(2), st(0)
        fxch st(3)
        fstp dword ptr [edx + 0x30]
        fld dword ptr [eax + 0x30]
        fmul dword ptr [ebx + 0x20]
        fxch st(1)
        faddp st(3), st(0)
        fld dword ptr [eax + 0x34]
        fmul dword ptr [ebx + 0x24]
        fld dword ptr [eax + 0x38]
        fxch st(4)
        faddp st(3), st(0)
        fxch st(3)
        fmul dword ptr [ebx + 0x28]
        fld dword ptr [eax + 0x3c]
        fmul dword ptr [ebx + 0x2c]
        fxch st(2)
        faddp st(4), st(0)
        fxch st(2)
        fstp dword ptr [edx + 0x34]
        fld dword ptr [eax + 0x30]
        fmul dword ptr [ebx + 0x30]
        fxch st(3)
        faddp st(2), st(0)
        fld dword ptr [eax + 0x34]
        fmul dword ptr [ebx + 0x34]
        fld dword ptr [eax + 0x38]
        fxch st(3)
        faddp st(2), st(0)
        fxch st(2)
        fmul dword ptr [ebx + 0x38]
        fld dword ptr [eax + 0x3c]
        fmul dword ptr [ebx + 0x3c]
        fxch st(4)
        faddp st(3), st(0)
        fxch st(1)
        fstp dword ptr [edx + 0x38]
        faddp st(1), st(0)
        faddp st(1), st(0)
        fstp dword ptr [edx + 0x3c]
    }
}

// Writes all sixteen floats of out with the full 4x4 product of a and b, where out[i][j] is the dot
// product of row i of a and column j of b. It is one hand-scheduled FPU __asm block, and the
// function has no locals and no calls.
// FUNCTION: FF7 0x0066c984
void __cdecl MultiplyMatrix4x4Full(void *a, void *b, void *out)
{
    __asm {
        mov edx, out
        mov eax, a
        mov ebx, b
        fld dword ptr [eax]
        fmul dword ptr [ebx]
        fld dword ptr [eax + 4]
        fmul dword ptr [ebx + 0x10]
        fld dword ptr [eax + 8]
        fmul dword ptr [ebx + 0x20]
        fld dword ptr [eax + 0xc]
        fmul dword ptr [ebx + 0x30]
        fld dword ptr [eax]
        fxch st(4)
        faddp st(3), st(0)
        fxch st(3)
        fmul dword ptr [ebx + 4]
        fld dword ptr [eax + 4]
        fmul dword ptr [ebx + 0x14]
        fxch st(3)
        faddp st(2), st(0)
        fld dword ptr [eax + 8]
        fmul dword ptr [ebx + 0x24]
        fld dword ptr [eax + 0xc]
        fxch st(3)
        faddp st(5), st(0)
        fxch st(1)
        faddp st(3), st(0)
        fxch st(1)
        fmul dword ptr [ebx + 0x34]
        fld dword ptr [eax]
        fxch st(4)
        fstp dword ptr [edx]
        fxch st(2)
        faddp st(1), st(0)
        fxch st(2)
        fmul dword ptr [ebx + 8]
        fld dword ptr [eax + 4]
        fmul dword ptr [ebx + 0x18]
        fxch st(3)
        faddp st(2), st(0)
        fld dword ptr [eax + 8]
        fmul dword ptr [ebx + 0x28]
        fld dword ptr [eax + 0xc]
        fxch st(3)
        fstp dword ptr [edx + 4]
        fxch st(1)
        faddp st(3), st(0)
        fxch st(1)
        fmul dword ptr [ebx + 0x38]
        fld dword ptr [eax]
        fmul dword ptr [ebx + 0xc]
        fxch st(3)
        faddp st(2), st(0)
        fld dword ptr [eax + 4]
        fmul dword ptr [ebx + 0x1c]
        fld dword ptr [eax + 8]
        fxch st(3)
        faddp st(2), st(0)
        fxch st(2)
        fmul dword ptr [ebx + 0x2c]
        fld dword ptr [eax + 0xc]
        fmul dword ptr [ebx + 0x3c]
        fxch st(4)
        faddp st(3), st(0)
        fxch st(1)
        fstp dword ptr [edx + 8]
        fld dword ptr [eax + 0x10]
        fmul dword ptr [ebx]
        fxch st(2)
        faddp st(1), st(0)
        fld dword ptr [eax + 0x14]
        fmul dword ptr [ebx + 0x10]
        fld dword ptr [eax + 0x18]
        fxch st(2)
        faddp st(4), st(0)
        fxch st(1)
        fmul dword ptr [ebx + 0x20]
        fld dword ptr [eax + 0x1c]
        fmul dword ptr [ebx + 0x30]
        fxch st(3)
        faddp st(2), st(0)
        fxch st(3)
        fstp dword ptr [edx + 0xc]
        fld dword ptr [eax + 0x10]
        fmul dword ptr [ebx + 4]
        fxch st(1)
        faddp st(3), st(0)
        fld dword ptr [eax + 0x14]
        fmul dword ptr [ebx + 0x14]
        fld dword ptr [eax + 0x18]
        fxch st(4)
        faddp st(3), st(0)
        fxch st(3)
        fmul dword ptr [ebx + 0x24]
        fld dword ptr [eax + 0x1c]
        fmul dword ptr [ebx + 0x34]
        fxch st(2)
        faddp st(4), st(0)
        fxch st(2)
        fstp dword ptr [edx + 0x10]
        fld dword ptr [eax + 0x10]
        fmul dword ptr [ebx + 8]
        fxch st(3)
        faddp st(2), st(0)
        fld dword ptr [eax + 0x14]
        fmul dword ptr [ebx + 0x18]
        fld dword ptr [eax + 0x18]
        fxch st(3)
        faddp st(2), st(0)
        fxch st(2)
        fmul dword ptr [ebx + 0x28]
        fld dword ptr [eax + 0x1c]
        fmul dword ptr [ebx + 0x38]
        fxch st(4)
        faddp st(3), st(0)
        fxch st(1)
        fstp dword ptr [edx + 0x14]
        fld dword ptr [eax + 0x10]
        fmul dword ptr [ebx + 0xc]
        fxch st(2)
        faddp st(1), st(0)
        fld dword ptr [eax + 0x14]
        fmul dword ptr [ebx + 0x1c]
        fld dword ptr [eax + 0x18]
        fxch st(2)
        faddp st(4), st(0)
        fxch st(1)
        fmul dword ptr [ebx + 0x2c]
        fld dword ptr [eax + 0x1c]
        fmul dword ptr [ebx + 0x3c]
        fxch st(3)
        faddp st(2), st(0)
        fxch st(3)
        fstp dword ptr [edx + 0x18]
        fld dword ptr [eax + 0x20]
        fmul dword ptr [ebx]
        fxch st(1)
        faddp st(3), st(0)
        fld dword ptr [eax + 0x24]
        fmul dword ptr [ebx + 0x10]
        fld dword ptr [eax + 0x28]
        fxch st(4)
        faddp st(3), st(0)
        fxch st(3)
        fmul dword ptr [ebx + 0x20]
        fld dword ptr [eax + 0x2c]
        fmul dword ptr [ebx + 0x30]
        fxch st(2)
        faddp st(4), st(0)
        fxch st(2)
        fstp dword ptr [edx + 0x1c]
        fld dword ptr [eax + 0x20]
        fmul dword ptr [ebx + 4]
        fxch st(3)
        faddp st(2), st(0)
        fld dword ptr [eax + 0x24]
        fmul dword ptr [ebx + 0x14]
        fld dword ptr [eax + 0x28]
        fxch st(3)
        faddp st(2), st(0)
        fxch st(2)
        fmul dword ptr [ebx + 0x24]
        fld dword ptr [eax + 0x2c]
        fmul dword ptr [ebx + 0x34]
        fxch st(4)
        faddp st(3), st(0)
        fxch st(1)
        fstp dword ptr [edx + 0x20]
        fld dword ptr [eax + 0x20]
        fmul dword ptr [ebx + 8]
        fxch st(2)
        faddp st(1), st(0)
        fld dword ptr [eax + 0x24]
        fmul dword ptr [ebx + 0x18]
        fld dword ptr [eax + 0x28]
        fxch st(2)
        faddp st(4), st(0)
        fxch st(1)
        fmul dword ptr [ebx + 0x28]
        fld dword ptr [eax + 0x2c]
        fmul dword ptr [ebx + 0x38]
        fxch st(3)
        faddp st(2), st(0)
        fxch st(3)
        fstp dword ptr [edx + 0x24]
        fld dword ptr [eax + 0x20]
        fmul dword ptr [ebx + 0xc]
        fxch st(1)
        faddp st(3), st(0)
        fld dword ptr [eax + 0x24]
        fmul dword ptr [ebx + 0x1c]
        fld dword ptr [eax + 0x28]
        fxch st(4)
        faddp st(3), st(0)
        fxch st(3)
        fmul dword ptr [ebx + 0x2c]
        fld dword ptr [eax + 0x2c]
        fmul dword ptr [ebx + 0x3c]
        fxch st(2)
        faddp st(4), st(0)
        fxch st(2)
        fstp dword ptr [edx + 0x28]
        fld dword ptr [eax + 0x30]
        fmul dword ptr [ebx]
        fxch st(3)
        faddp st(2), st(0)
        fld dword ptr [eax + 0x34]
        fmul dword ptr [ebx + 0x10]
        fld dword ptr [eax + 0x38]
        fxch st(3)
        faddp st(2), st(0)
        fxch st(2)
        fmul dword ptr [ebx + 0x20]
        fld dword ptr [eax + 0x3c]
        fmul dword ptr [ebx + 0x30]
        fxch st(4)
        faddp st(3), st(0)
        fxch st(1)
        fstp dword ptr [edx + 0x2c]
        fld dword ptr [eax + 0x30]
        fmul dword ptr [ebx + 4]
        fxch st(2)
        faddp st(1), st(0)
        fld dword ptr [eax + 0x34]
        fmul dword ptr [ebx + 0x14]
        fld dword ptr [eax + 0x38]
        fxch st(2)
        faddp st(4), st(0)
        fxch st(1)
        fmul dword ptr [ebx + 0x24]
        fld dword ptr [eax + 0x3c]
        fmul dword ptr [ebx + 0x34]
        fxch st(3)
        faddp st(2), st(0)
        fxch st(3)
        fstp dword ptr [edx + 0x30]
        fld dword ptr [eax + 0x30]
        fmul dword ptr [ebx + 8]
        fxch st(1)
        faddp st(3), st(0)
        fld dword ptr [eax + 0x34]
        fmul dword ptr [ebx + 0x18]
        fld dword ptr [eax + 0x38]
        fxch st(4)
        faddp st(3), st(0)
        fxch st(3)
        fmul dword ptr [ebx + 0x28]
        fld dword ptr [eax + 0x3c]
        fmul dword ptr [ebx + 0x38]
        fxch st(2)
        faddp st(4), st(0)
        fxch st(2)
        fstp dword ptr [edx + 0x34]
        fld dword ptr [eax + 0x30]
        fmul dword ptr [ebx + 0xc]
        fxch st(3)
        faddp st(2), st(0)
        fld dword ptr [eax + 0x34]
        fmul dword ptr [ebx + 0x1c]
        fld dword ptr [eax + 0x38]
        fxch st(3)
        faddp st(2), st(0)
        fxch st(2)
        fmul dword ptr [ebx + 0x2c]
        fld dword ptr [eax + 0x3c]
        fmul dword ptr [ebx + 0x3c]
        fxch st(4)
        faddp st(3), st(0)
        fxch st(1)
        fstp dword ptr [edx + 0x38]
        faddp st(1), st(0)
        faddp st(1), st(0)
        fstp dword ptr [edx + 0x3c]
    }
}

// Concatenates two affine 4x4 float matrices into out. The 3x3 block at offsets 0..8, 0x10..0x18
// and 0x20..0x28 is a's 3x3 block times b's 3x3 block. The translation row at 0x30, 0x34 and 0x38
// is a's translation row through b's 3x3 block plus b's translation row. The fourth column at 0xC,
// 0x1C, 0x2C and 0x3C is never written. It is one hand-scheduled FPU __asm block.
// FUNCTION: FF7 0x0066cc3b
void __cdecl ConcatAffineMatrix4x4(void *a, void *b, void *out)
{
    __asm {
        mov edx, out
        mov eax, a
        mov ebx, b
        fld dword ptr [eax]
        fmul dword ptr [ebx]
        fld dword ptr [eax + 4]
        fmul dword ptr [ebx + 0x10]
        fld dword ptr [eax + 8]
        fmul dword ptr [ebx + 0x20]
        fld dword ptr [eax]
        fmul dword ptr [ebx + 4]
        fld dword ptr [eax + 4]
        fxch st(4)
        faddp st(3), st(0)
        fxch st(3)
        fmul dword ptr [ebx + 0x14]
        fld dword ptr [eax + 8]
        fmul dword ptr [ebx + 0x24]
        fxch st(3)
        faddp st(2), st(0)
        fld dword ptr [eax]
        fmul dword ptr [ebx + 8]
        fxch st(4)
        faddp st(1), st(0)
        fxch st(1)
        fstp dword ptr [edx]
        fld dword ptr [eax + 4]
        fmul dword ptr [ebx + 0x18]
        fxch st(1)
        faddp st(2), st(0)
        fld dword ptr [eax + 8]
        fmul dword ptr [ebx + 0x28]
        fld dword ptr [eax + 0x10]
        fxch st(3)
        fstp dword ptr [edx + 4]
        fxch st(3)
        faddp st(1), st(0)
        fxch st(1)
        fmul dword ptr [ebx]
        fld dword ptr [eax + 0x14]
        fmul dword ptr [ebx + 0x10]
        fxch st(2)
        faddp st(3), st(0)
        fld dword ptr [eax + 0x18]
        fmul dword ptr [ebx + 0x20]
        fld dword ptr [eax + 0x10]
        fxch st(4)
        fstp dword ptr [edx + 8]
        fxch st(1)
        faddp st(2), st(0)
        fxch st(2)
        fmul dword ptr [ebx + 4]
        fld dword ptr [eax + 0x14]
        fmul dword ptr [ebx + 0x14]
        fxch st(2)
        faddp st(3), st(0)
        fld dword ptr [eax + 0x18]
        fmul dword ptr [ebx + 0x24]
        fld dword ptr [eax + 0x10]
        fxch st(4)
        fstp dword ptr [edx + 0x10]
        fxch st(1)
        faddp st(2), st(0)
        fxch st(2)
        fmul dword ptr [ebx + 8]
        fld dword ptr [eax + 0x14]
        fmul dword ptr [ebx + 0x18]
        fxch st(2)
        faddp st(3), st(0)
        fld dword ptr [eax + 0x18]
        fmul dword ptr [ebx + 0x28]
        fld dword ptr [eax + 0x20]
        fxch st(4)
        fstp dword ptr [edx + 0x14]
        fxch st(1)
        faddp st(2), st(0)
        fxch st(2)
        fmul dword ptr [ebx]
        fld dword ptr [eax + 0x24]
        fmul dword ptr [ebx + 0x10]
        fxch st(2)
        faddp st(3), st(0)
        fld dword ptr [eax + 0x28]
        fmul dword ptr [ebx + 0x20]
        fld dword ptr [eax + 0x20]
        fxch st(4)
        fstp dword ptr [edx + 0x18]
        fxch st(1)
        faddp st(2), st(0)
        fxch st(2)
        fmul dword ptr [ebx + 4]
        fld dword ptr [eax + 0x24]
        fmul dword ptr [ebx + 0x14]
        fxch st(2)
        faddp st(3), st(0)
        fld dword ptr [eax + 0x28]
        fmul dword ptr [ebx + 0x24]
        fld dword ptr [eax + 0x20]
        fxch st(4)
        fstp dword ptr [edx + 0x20]
        fxch st(1)
        faddp st(2), st(0)
        fxch st(2)
        fmul dword ptr [ebx + 8]
        fld dword ptr [eax + 0x24]
        fmul dword ptr [ebx + 0x18]
        fxch st(2)
        faddp st(3), st(0)
        fld dword ptr [eax + 0x28]
        fmul dword ptr [ebx + 0x28]
        fld dword ptr [eax + 0x30]
        fxch st(4)
        fstp dword ptr [edx + 0x24]
        fxch st(1)
        faddp st(2), st(0)
        fxch st(2)
        fmul dword ptr [ebx]
        fld dword ptr [eax + 0x34]
        fmul dword ptr [ebx + 0x10]
        fxch st(2)
        faddp st(3), st(0)
        fld dword ptr [eax + 0x38]
        fmul dword ptr [ebx + 0x20]
        fld dword ptr [eax + 0x30]
        fxch st(4)
        fstp dword ptr [edx + 0x28]
        fxch st(1)
        faddp st(2), st(0)
        fxch st(2)
        fmul dword ptr [ebx + 4]
        fld dword ptr [eax + 0x34]
        fmul dword ptr [ebx + 0x14]
        fxch st(2)
        faddp st(3), st(0)
        fld dword ptr [eax + 0x38]
        fmul dword ptr [ebx + 0x24]
        fld dword ptr [eax + 0x30]
        fxch st(4)
        fadd dword ptr [ebx + 0x30]
        fxch st(2)
        faddp st(3), st(0)
        fxch st(3)
        fmul dword ptr [ebx + 8]
        fld dword ptr [eax + 0x34]
        fxch st(2)
        fstp dword ptr [edx + 0x30]
        fxch st(2)
        faddp st(3), st(0)
        fmul dword ptr [ebx + 0x18]
        fld dword ptr [eax + 0x38]
        fmul dword ptr [ebx + 0x28]
        fxch st(3)
        fadd dword ptr [ebx + 0x34]
        fxch st(2)
        faddp st(1), st(0)
        faddp st(2), st(0)
        fstp dword ptr [edx + 0x34]
        fadd dword ptr [ebx + 0x38]
        fstp dword ptr [edx + 0x38]
    }
}

/*
 * Assembly computes three row-vector dot products against matrix columns using the input float x,
 * y, z and stores three floats. Translation and fourth components are ignored; x87 operation order
 * is explicit.
 */
// FUNCTION: FF7 0x0066cdd6
void __cdecl RotateVector3RowVec(void *matrix, void *point, void *out)
{
    __asm {
        mov edx, out
        mov eax, matrix
        mov ebx, point
        fld dword ptr [eax]
        fmul dword ptr [ebx]
        fld dword ptr [eax + 010h]
        fmul dword ptr [ebx + 4]
        fld dword ptr [eax + 020h]
        fmul dword ptr [ebx + 8]
        fld dword ptr [eax + 4]
        fmul dword ptr [ebx]
        fld dword ptr [eax + 014h]
        fxch st(4)
        faddp st(3), st
        fxch st(3)
        fmul dword ptr [ebx + 4]
        fld dword ptr [eax + 024h]
        fmul dword ptr [ebx + 8]
        fxch st(3)
        faddp st(2), st
        fld dword ptr [eax + 8]
        fmul dword ptr [ebx]
        fxch st(4)
        faddp st(1), st
        fxch st(1)
        fstp dword ptr [edx]
        fld dword ptr [eax + 018h]
        fmul dword ptr [ebx + 4]
        fxch st(1)
        faddp st(2), st
        fld dword ptr [eax + 028h]
        fmul dword ptr [ebx + 8]
        fxch st(3)
        faddp st(1), st
        fxch st(1)
        fstp dword ptr [edx + 4]
        faddp st(1), st
        fstp dword ptr [edx + 8]
    }
}

/*
 * x87 assembly multiplies input XYZ by four columns of4x4 float matrix plus translation and stores
 * four output floats, with explicit staged multiply/add order and interleaved stores. Does not
 * read input W; point/matrix alias with output can affect later rereads. Extended precision and
 * rounding follow x87 stores.
 */
// FUNCTION: FF7 0x0066ce40
void __cdecl TransformPointByMatrix4x4(void *matrix, void *point, void *out)
{
    __asm {
        mov edx, out
        mov eax, matrix
        mov ebx, point
        fld dword ptr [eax]
        fmul dword ptr [ebx]
        fld dword ptr [eax + 010h]
        fmul dword ptr [ebx + 4]
        fld dword ptr [eax + 020h]
        fmul dword ptr [ebx + 8]
        fld dword ptr [eax + 4]
        fmul dword ptr [ebx]
        fld dword ptr [eax + 014h]
        fxch st(4)
        faddp st(3), st
        fxch st(3)
        fmul dword ptr [ebx + 4]
        fld dword ptr [eax + 024h]
        fmul dword ptr [ebx + 8]
        fxch st(3)
        faddp st(2), st
        fld dword ptr [eax + 8]
        fmul dword ptr [ebx]
        fxch st(4)
        faddp st(1), st
        fxch st(1)
        fadd dword ptr [eax + 030h]
        fld dword ptr [eax + 018h]
        fmul dword ptr [ebx + 4]
        fxch st(2)
        faddp st(3), st
        fstp dword ptr [edx]
        fld dword ptr [eax + 028h]
        fmul dword ptr [ebx + 8]
        fxch st(2)
        fadd dword ptr [eax + 034h]
        fxch st(3)
        faddp st(1), st
        fld dword ptr [eax + 0ch]
        fmul dword ptr [ebx]
        fxch st(3)
        fstp dword ptr [edx + 4]
        faddp st(1), st
        fld dword ptr [eax + 01ch]
        fmul dword ptr [ebx + 4]
        fld dword ptr [eax + 02ch]
        fxch st(2)
        fadd dword ptr [eax + 038h]
        fxch st(2)
        fmul dword ptr [ebx + 8]
        fxch st(3)
        faddp st(1), st
        fxch st(1)
        fstp dword ptr [edx + 8]
        faddp st(1), st
        fadd dword ptr [eax + 03ch]
        fstp dword ptr [edx + 0ch]
    }
}

/*
 * Assembly computes row-vector three-component rotation and adds matrix translation floats at
 * 0x30, 0x34, 0x38 before storing output x, y, z. Fourth components are ignored; x87 order is
 * explicit.
 */
// FUNCTION: FF7 0x0066ced8
void __cdecl TransformPoint3RowVec(void *matrix, void *point, void *out)
{
    __asm {
        mov edx, out
        mov eax, matrix
        mov ebx, point
        fld dword ptr [eax]
        fmul dword ptr [ebx]
        fld dword ptr [eax + 010h]
        fmul dword ptr [ebx + 4]
        fld dword ptr [eax + 020h]
        fmul dword ptr [ebx + 8]
        fld dword ptr [eax + 4]
        fmul dword ptr [ebx]
        fld dword ptr [eax + 014h]
        fxch st(4)
        faddp st(3), st
        fxch st(3)
        fmul dword ptr [ebx + 4]
        fld dword ptr [eax + 024h]
        fmul dword ptr [ebx + 8]
        fxch st(3)
        faddp st(2), st
        fld dword ptr [eax + 8]
        fmul dword ptr [ebx]
        fxch st(4)
        faddp st(1), st
        fxch st(1)
        fadd dword ptr [eax + 030h]
        fld dword ptr [eax + 018h]
        fmul dword ptr [ebx + 4]
        fxch st(2)
        faddp st(3), st
        fstp dword ptr [edx]
        fld dword ptr [eax + 028h]
        fmul dword ptr [ebx + 8]
        fxch st(2)
        fadd dword ptr [eax + 034h]
        fxch st(3)
        faddp st(1), st
        faddp st(1), st
        fxch st(1)
        fstp dword ptr [edx + 4]
        fadd dword ptr [eax + 038h]
        fstp dword ptr [edx + 8]
    }
}

/*
 * Assembly computes only point dot matrix column z plus translation 0x38 and stores out+eight.
 * Other output components are untouched.
 */
// FUNCTION: FF7 0x0066cf4d
void __cdecl TransformPointDepthOnly(float *matrix, float *point, float *out)
{
    __asm {
        mov edx, out
        mov eax, matrix
        mov ebx, point
        fld dword ptr [eax + 8]
        fmul dword ptr [ebx]
        fld dword ptr [eax + 18h]
        fmul dword ptr [ebx + 4]
        fld dword ptr [eax + 28h]
        fmul dword ptr [ebx + 8]
        fxch st(2)
        faddp st(1), st
        faddp st(1), st
        fadd dword ptr [eax + 38h]
        fstp dword ptr [edx + 8]
    }
}

/*
 * Assembly computes three column-vector dot products against matrix rows from input float x, y, z
 * and stores three floats without translation. x87 order is explicit.
 */
// FUNCTION: FF7 0x0066cf7e
void __cdecl RotateVector3ColVec(void *matrix, void *point, void *out)
{
    __asm {
        mov edx, out
        mov eax, matrix
        mov ebx, point
        fld dword ptr [eax]
        fmul dword ptr [ebx]
        fld dword ptr [eax + 4]
        fmul dword ptr [ebx + 4]
        fld dword ptr [eax + 8]
        fmul dword ptr [ebx + 8]
        fld dword ptr [eax + 010h]
        fmul dword ptr [ebx]
        fld dword ptr [eax + 014h]
        fxch st(4)
        faddp st(3), st
        fxch st(3)
        fmul dword ptr [ebx + 4]
        fld dword ptr [eax + 018h]
        fmul dword ptr [ebx + 8]
        fxch st(3)
        faddp st(2), st
        fld dword ptr [eax + 020h]
        fmul dword ptr [ebx]
        fxch st(4)
        faddp st(1), st
        fxch st(1)
        fstp dword ptr [edx]
        fld dword ptr [eax + 024h]
        fmul dword ptr [ebx + 4]
        fxch st(1)
        faddp st(2), st
        fld dword ptr [eax + 028h]
        fmul dword ptr [ebx + 8]
        fxch st(3)
        faddp st(1), st
        fxch st(1)
        fstp dword ptr [edx + 4]
        faddp st(1), st
        fstp dword ptr [edx + 8]
    }
}

/*
 * Assembly computes three column-vector rotation dot products and adds translation floats at 0xC,
 * 0x1C, 0x2C, then stores output x, y, z. Fourth components are ignored; x87 order is explicit.
 */
// FUNCTION: FF7 0x0066cfe8
void __cdecl TransformPoint3ColVec(float *matrix, float *point, float *out)
{
    __asm {
        mov edx, out
        mov eax, matrix
        mov ebx, point
        fld dword ptr [eax]
        fmul dword ptr [ebx]
        fld dword ptr [eax + 4]
        fmul dword ptr [ebx + 4]
        fld dword ptr [eax + 8]
        fmul dword ptr [ebx + 8]
        fld dword ptr [eax + 010h]
        fmul dword ptr [ebx]
        fld dword ptr [eax + 014h]
        fxch st(4)
        faddp st(3), st
        fxch st(3)
        fmul dword ptr [ebx + 4]
        fld dword ptr [eax + 018h]
        fmul dword ptr [ebx + 8]
        fxch st(3)
        faddp st(2), st
        fld dword ptr [eax + 020h]
        fmul dword ptr [ebx]
        fxch st(4)
        faddp st(1), st
        fxch st(1)
        fadd dword ptr [eax + 0ch]
        fld dword ptr [eax + 024h]
        fmul dword ptr [ebx + 4]
        fxch st(2)
        faddp st(3), st
        fstp dword ptr [edx]
        fld dword ptr [eax + 028h]
        fmul dword ptr [ebx + 8]
        fxch st(2)
        fadd dword ptr [eax + 01ch]
        fxch st(3)
        faddp st(1), st
        faddp st(1), st
        fxch st(1)
        fstp dword ptr [edx + 4]
        fadd dword ptr [eax + 02ch]
        fstp dword ptr [edx + 8]
    }
}
} /* extern "C" */
