// src/lib/src/graphics/sw/raster_float.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: The software rasterizer's fixed-point 64-bit float conversion helpers, hand-written x87 code.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/types/graphics.h"
#include "lib/api/graphics.h"
#include "lib/crt.h"
#include "lib/codegen.h"
extern "C" {

/*
 * Clears the two output dwords, multiplies the supplied float by 65536 and
 * converts it with inline x87 FISTP under the caller rounding mode. Stores the
 * resulting integer starting two bytes into output and sign-extends its high
 * word for a negative result.
 */
// FUNCTION: FF7 0x006bbbd0
void __cdecl Gfx_RasterFloatToInt64Fistp(float value, int *out)
{
    int p;
    out[1] = 0;
    out[0] = 0;
    value *= 65536.0f;
    __asm {
        fld value
        fistp p
    }
    *(int *)((char *)out + 2) = p;
    if (p < 0) {
        out[1] |= 0xFFFF0000;
    }
}

/*
 * Original naked exponent/mantissa converter. Reads the float bits, forces the
 * implicit mantissa bit, shifts the mantissa into low/high dwords using
 * exponent minus 118, applies two-dword negation for the sign and stores both
 * dwords.
 */
// FUNCTION: FF7 0x006bbc24
__declspec(naked) void __cdecl crt_FloatToInt64(float value, void *out)
{
    __asm {
        push ebp
        mov ebp, esp
        push ebx
        push esi
        push edi
        jmp convert_start
                            // VC5's inline assembler shortens this unreachable four-byte alignment LEA.
        _emit 08dh
        _emit 064h
        _emit 024h
        _emit 000h
    convert_start:
        mov edi, dword ptr [ebp+0ch]
        mov esi, dword ptr [ebp+8]
        mov eax, esi
        mov ecx, esi
        and eax, 7fffffh
        and ecx, 7f800000h
        or eax, 800000h
        shr ecx, 17h
        sub ecx, 76h
        jge convert_shift_left
        neg ecx
        xor edx, edx
        cmp ecx, 20h
        jge convert_zero_right
        shr eax, cl
        jmp convert_sign
    convert_zero_right:
        xor eax, eax
        jmp convert_sign
    convert_shift_left:
        xor edx, edx
        cmp ecx, 20h
        jge convert_wide_left
        shld edx, eax, cl
        shl eax, cl
        jmp convert_sign
    convert_wide_left:
        sub ecx, 20h
        cmp ecx, 20h
        jge convert_zero_left
        shl eax, cl
        mov edx, eax
        xor eax, eax
        jmp convert_sign
    convert_zero_left:
        xor eax, eax
    convert_sign:
        and esi, 80000000h
        je convert_store
        neg edx
        neg eax
        sbb edx, 0
    convert_store:
        mov dword ptr [edi], eax
        mov dword ptr [edi+4], edx
        pop edi
        pop esi
        pop ebx
        pop ebp
        ret
    }
}

/*
 * Decodes float bits into a two-dword signed fixed-point output, forcing the
 * implicit mantissa bit and shifting it according to exponent minus 118. Uses
 * authored SHR/SHLD/SHL blocks to form the low/high words, zeros shifts of at
 * least 64 bits and applies NEG/NEG/SBB for negative inputs.
 */
// FUNCTION: FF7 0x006bbc9c
void __cdecl Gfx_RasterFloatToInt64Framed(float value, unsigned int *out)
{
    unsigned int highWord;
    float valueCopy;
    int shiftCount;
    unsigned int rawFlags;
    unsigned int mantissa;
    valueCopy = value;
    rawFlags = *(unsigned int *)&valueCopy;
    mantissa = rawFlags & 0x007FFFFF;
    mantissa |= 0x00800000;
    shiftCount = ((rawFlags & 0x7F800000) >> 23) - 0x76;
    if (shiftCount < 0) {
        shiftCount = -shiftCount;
        if (shiftCount < 32) {
            __asm {
                mov cl, byte ptr shiftCount
                mov eax, mantissa
                xor edx, edx
                shr eax, cl
                mov mantissa, eax
                mov highWord, edx
            }
        } else {
            highWord = 0;
            mantissa = 0;
        }
    } else {
        if (shiftCount < 32) {
            __asm {
                mov cl, byte ptr shiftCount
                mov eax, mantissa
                xor edx, edx
                shld edx, eax, cl
                shl eax, cl
                mov mantissa, eax
                mov highWord, edx
            }
        } else {
            shiftCount -= 32;
            if (shiftCount < 32) {
                __asm {
                    mov cl, byte ptr shiftCount
                    mov eax, mantissa
                    shl eax, cl
                    mov mantissa, 0
                    mov highWord, eax
                }
            } else {
                highWord = 0;
                mantissa = 0;
            }
        }
    }
    if (rawFlags & 0x80000000) {
        __asm {
            mov eax, mantissa
            mov edx, highWord
            neg edx
            neg eax
            sbb edx, 0
            mov mantissa, eax
            mov highWord, edx
        }
    }
    out[1] = highWord;
    out[0] = mantissa;
}

/*
 * Negate a split 64-bit word in place. The original preserves unused ESI/EDI
 * and schedules NEG/NEG/SBB in authored assembly.
 */
// FUNCTION: FF7 0x006bbd94
__declspec(naked) void __cdecl Gfx_RasterNegateFixed64(unsigned int *value)
{
    __asm {
        push ebp
        mov ebp, esp
        push ebx
        push esi
        push edi
        mov ebx, dword ptr [ebp + 8]
        mov eax, dword ptr [ebx]
        mov edx, dword ptr [ebx + 4]
        neg edx
        neg eax
        sbb edx, 0
        mov dword ptr [ebx], eax
        mov dword ptr [ebx + 4], edx
        pop edi
        pop esi
        pop ebx
        pop ebp
        ret
    }
}
} /* extern "C" */
