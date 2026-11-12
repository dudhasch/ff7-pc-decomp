// src/lib/src/graphics/graphics_raster_dda_span.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Original hand-written assembly span helper that steps the DDA outputs for the software
//   rasterizer.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/types/graphics.h"
#include "lib/api/graphics.h"
#include "lib/crt.h"
#include "lib/codegen.h"
extern "C" {

// clang-format off
// clang-format on



// Both original boundaries 6BC2E0 and 6BC350 are 16-byte aligned.

// Original naked assembly span helper. It stores count values to output, with zero as
// the first value. Each step advances two split 64-bit accumulators by the span fields at
// +0x74/+0x78 and +0x84/+0x88 with add and adc. The high word of the second accumulator is shifted
// left by the byte at +0x10C, and the result is added to the high word of the first accumulator and
// stored.
// FUNCTION: FF7 0x006bc2e0
__declspec(naked) void __cdecl Gfx_RasterStepDdaSpanOutputs(int count, int *output, void *span)
{
    __asm {
        push ebp
        mov ebp, esp
        sub esp, 10h
        push ebx
        push esi
        push edi
        xor eax, eax
        mov esi, dword ptr [ebp + 10h]
        mov edi, dword ptr [ebp + 0ch]
        mov ebx, dword ptr [ebp + 8]
        mov dword ptr [ebp - 4], eax
        mov dword ptr [ebp - 8], eax
        mov dword ptr [ebp - 10h], eax
        mov dword ptr [ebp - 0ch], eax
        jmp dda_store
    dda_step:
        mov ecx, dword ptr [ebp - 4]
        mov eax, dword ptr [ebp - 8]
        add ecx, dword ptr [esi + 74h]
        adc eax, dword ptr [esi + 78h]
        mov dword ptr [ebp - 4], ecx
        mov dword ptr [ebp - 8], eax
        mov ecx, dword ptr [ebp - 10h]
        mov edx, dword ptr [ebp - 0ch]
        add ecx, dword ptr [esi + 84h]
        adc edx, dword ptr [esi + 88h]
        mov dword ptr [ebp - 10h], ecx
        mov dword ptr [ebp - 0ch], edx
        mov cl, byte ptr [esi + 10ch]
        shl edx, cl
        add eax, edx
    dda_store:
        mov dword ptr [edi], eax
        add edi, 4
        dec ebx
        jg dda_step
        pop edi
        pop esi
        pop ebx
        mov esp, ebp
        pop ebp
        ret
    }
}
} /* extern "C" */
