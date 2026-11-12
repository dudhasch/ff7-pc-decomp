// src/lib/src/graphics/sw/sw_gfx_raster_blit_indexed_rect.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Float rounding and the indexed-texture rectangle blitters of the software rasterizer,
//   in plain, doubled, colour-keyed, blended and depth-tested variants, plus the blit offset tables
//   and quad dispatch.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/types/graphics.h"
#include "lib/api/graphics.h"
#include "lib/crt.h"
#include "lib/codegen.h"
#include "lib/enums/graphics.h"
extern "C" {

// clang-format off
void __cdecl Gfx_RasterBlitIndexedRect(void);
void __cdecl Gfx_RasterBlitIndexedRectColorKey(void);
void __cdecl Gfx_RasterBlitIndexedRectDepthWrite(void);
void __cdecl Gfx_RasterBlitIndexedRectColorKeyDepthWrite(void);
void __cdecl Gfx_RasterBlitIndexedRectBlend(void);
void __cdecl Gfx_RasterBlitIndexedRectColorKeyBlend(void);
void __cdecl Gfx_RasterBlitIndexedRectDepthTestBlend(void);
void __cdecl Gfx_RasterBlitIndexedRectColorKeyDepthTestBlend(void);
void __cdecl Gfx_RasterBlitIndexedRectBlendRemap(void);
void __cdecl Gfx_RasterBlitIndexedRectColorKeyBlendRemap(void);
void __cdecl Gfx_RasterBlitIndexedRectDepthTestBlendRemap(void);
void __cdecl Gfx_RasterBlitIndexedRectColorKeyDepthTestBlendRemap(void);
void __cdecl Gfx_RasterBlitIndexedRectDoubled(void);
void __cdecl Gfx_RasterBlitIndexedRectDoubledColorKey(void);
void __cdecl Gfx_RasterBlitIndexedRectDoubledDepthWrite(void);
void __cdecl Gfx_RasterBlitIndexedRectDoubledColorKeyDepthWrite(void);
void __cdecl Gfx_RasterBlitIndexedRectOffsetTableDepthWrite(void);
void __cdecl Gfx_RasterBlitIndexedRectOffsetTableColorKeyDepthWrite(void);
// GLOBAL: FF7 0x00919880
void * g_GfxRasterBlitIndexedRectCallbacks[4] = {&Gfx_RasterBlitIndexedRect, &Gfx_RasterBlitIndexedRectColorKey, &Gfx_RasterBlitIndexedRectDepthWrite, &Gfx_RasterBlitIndexedRectColorKeyDepthWrite};
// GLOBAL: FF7 0x00919890
void * g_GfxRasterBlitIndexedRectBlendCallbacks[4] = {&Gfx_RasterBlitIndexedRectBlend, &Gfx_RasterBlitIndexedRectColorKeyBlend, &Gfx_RasterBlitIndexedRectDepthTestBlend, &Gfx_RasterBlitIndexedRectColorKeyDepthTestBlend};
// GLOBAL: FF7 0x009198a0
void * g_GfxRasterBlitIndexedRectBlendRemapCallbacks[4] = {&Gfx_RasterBlitIndexedRectBlendRemap, &Gfx_RasterBlitIndexedRectColorKeyBlendRemap, &Gfx_RasterBlitIndexedRectDepthTestBlendRemap, &Gfx_RasterBlitIndexedRectColorKeyDepthTestBlendRemap};
// GLOBAL: FF7 0x009198b0
void * g_GfxRasterBlitIndexedRectDoubledCallbacks[4] = {&Gfx_RasterBlitIndexedRectDoubled, &Gfx_RasterBlitIndexedRectDoubledColorKey, &Gfx_RasterBlitIndexedRectDoubledDepthWrite, &Gfx_RasterBlitIndexedRectDoubledColorKeyDepthWrite};
unsigned char g_rawData_009198c0[16] = {0x3c, 0xd2, 0x6b, 0x00, 0x3d, 0xd1, 0x6b, 0x00, 0x6f, 0xd4, 0x6b, 0x00, 0x36, 0xd3, 0x6b, 0x00};
// GLOBAL: FF7 0x009198d0
void * g_GfxRasterBlitIndexedRectOffsetTableCallbacks[4] = {&Gfx_RasterBlitIndexedRectOffsetTable, &Gfx_RasterBlitIndexedRectOffsetTableColorKey, &Gfx_RasterBlitIndexedRectOffsetTableDepthWrite, &Gfx_RasterBlitIndexedRectOffsetTableColorKeyDepthWrite};
// clang-format on


void __cdecl crt_FloatToInt64(float value, void *out);
void __cdecl Gfx_RasterBlitIndexedRectDoubledColorKeyBlendRemap(void);
void __cdecl Gfx_RasterBlitIndexedRectDoubledBlendRemap(void);
void __cdecl Gfx_RasterBlitIndexedRectDoubledColorKeyDepthTestBlendRemap(void);


/*
 * Loads the supplied float into x87 and converts it to the returned integer using
 * FISTP under the current rounding mode. Retains x87 exception/out-of-range
 * conversion behavior.
 */
// FUNCTION: FF7 0x006bc350
int __cdecl Gfx_RasterRoundFloatToInt(float value)
{
    int result;

    __asm {
        fld value
        fistp result
    }
    return result;
}

/*
 * Naked palette span renderer whose stack operands are not in its void prototype. Saves ESP in
 * the span context supplied by stack operand +0x14 and uses that record as its stack.
 */
// FUNCTION: FF7 0x006bc367
__declspec(naked) void __cdecl Gfx_RasterBlitIndexedRectColorKey(void)
{
    __asm {
        push ebp
        mov ebp, esp
        push ecx
        push ebx
        push esi
        push edi
        mov eax, dword ptr [ebp + 14h]
        mov dword ptr [eax + 158h], esp
        mov esp, eax
        mov eax, dword ptr [esp + 0ch]
        mov edi, dword ptr [esp + 8]
        shl eax, 0ch
        add edi, eax
        mov eax, dword ptr [esp + 1ch]
        lea edi, [edi + eax*2]
        mov dword ptr [esp + 38h], edi
        mov eax, dword ptr [esp + 118h]
        mov ecx, dword ptr [esp + 70h]
        mov edx, dword ptr [esp + 80h]
        lea eax, [eax + ecx]
        mov cl, byte ptr [esp + 10ch]
        shl edx, cl
        add eax, edx
        mov dword ptr [ebp - 4], eax
        mov esi, dword ptr [esp + 11ch]
    asm_96:
        mov ebx, dword ptr [ebp - 4]
        mov edx, dword ptr [ebp + 8]
        add dword ptr [esp], edx
    asm_97:
        xor ecx, ecx
        mov cl, byte ptr [ebx]
        cmp ecx, 0
        je asm_98
        mov eax, dword ptr [esi + ecx*4]
        mov word ptr [edi], ax
    asm_98:
        add edi, 2
        inc ebx
        dec edx
        jg asm_97
        mov ebx, dword ptr [ebp - 4]
        add ebx, dword ptr [ebp + 10h]
        mov dword ptr [ebp - 4], ebx
        mov edi, dword ptr [esp + 38h]
        mov ebx, dword ptr [esp + 10h]
        add edi, dword ptr [esp + 4]
        dec ebx
        mov dword ptr [esp + 38h], edi
        mov dword ptr [esp + 10h], ebx
        sub dword ptr [ebp + 0ch], 1
        jg asm_96
        mov esp, dword ptr [esp + 158h]
        pop edi
        pop esi
        pop ebx
        mov esp, ebp
        pop ebp
        ret
    }
}

/*
 * Naked palette span renderer whose source prototype omits the stack operands used by the
 * assembly. Uses stack operand +0x14 as a span context, saves ESP in the context and derives a
 * 16-bit output row.
 */
// FUNCTION: FF7 0x006bc40c
__declspec(naked) void __cdecl Gfx_RasterBlitIndexedRect(void)
{
    __asm {
        push ebp
        mov ebp, esp
        push ecx
        push ebx
        push esi
        push edi
        mov eax, dword ptr [ebp + 14h]
        mov dword ptr [eax + 158h], esp
        mov esp, eax
        mov eax, dword ptr [esp + 0ch]
        mov edi, dword ptr [esp + 8]
        shl eax, 0ch
        add edi, eax
        mov eax, dword ptr [esp + 1ch]
        lea edi, [edi + eax*2]
        mov dword ptr [esp + 38h], edi
        mov eax, dword ptr [esp + 118h]
        mov ecx, dword ptr [esp + 70h]
        mov edx, dword ptr [esp + 80h]
        lea eax, [eax + ecx]
        mov cl, byte ptr [esp + 10ch]
        shl edx, cl
        add eax, edx
        mov dword ptr [ebp - 4], eax
        mov esi, dword ptr [esp + 11ch]
    asm_99:
        mov ebx, dword ptr [ebp - 4]
        mov edx, dword ptr [ebp + 8]
        add dword ptr [esp], edx
    asm_100:
        xor ecx, ecx
        mov cl, byte ptr [ebx]
        mov eax, dword ptr [esi + ecx*4]
        mov word ptr [edi], ax
        add edi, 2
        inc ebx
        dec edx
        jg asm_100
        mov ebx, dword ptr [ebp - 4]
        add ebx, dword ptr [ebp + 10h]
        mov dword ptr [ebp - 4], ebx
        mov edi, dword ptr [esp + 38h]
        mov ebx, dword ptr [esp + 10h]
        add edi, dword ptr [esp + 4]
        dec ebx
        mov dword ptr [esp + 38h], edi
        mov dword ptr [esp + 10h], ebx
        sub dword ptr [ebp + 0ch], 1
        jg asm_99
        mov esp, dword ptr [esp + 158h]
        pop edi
        pop esi
        pop ebx
        mov esp, ebp
        pop ebp
        ret
    }
}

/*
 * Naked assembly span renderer whose stack operands are not in its prototype. Saves ESP in the
 * context and derives source-byte, destination-word and depth-row addresses.
 */
// FUNCTION: FF7 0x006bc4ac
__declspec(naked) void __cdecl Gfx_RasterBlitIndexedRectColorKeyDepthWrite(void)
{
    __asm {
        push ebp
        mov ebp, esp
        sub esp, 8
        push ebx
        push esi
        push edi
        mov eax, dword ptr [ebp + 14h]
        mov dword ptr [eax + 158h], esp
        mov esp, eax
        mov eax, dword ptr [esp + 0ch]
        mov edi, dword ptr [esp + 8]
        shl eax, 0ch
        mov esi, dword ptr [esp + 40h]
        add edi, eax
        add esi, eax
        mov eax, dword ptr [esp + 1ch]
        lea esi, [esi + eax*4]
        lea edi, [edi + eax*2]
        mov dword ptr [esp + 64h], esi
        mov dword ptr [esp + 38h], edi
        mov eax, dword ptr [esp + 118h]
        mov ecx, dword ptr [esp + 70h]
        mov edx, dword ptr [esp + 80h]
        lea eax, [eax + ecx]
        mov cl, byte ptr [esp + 10ch]
        shl edx, cl
        add eax, edx
        mov dword ptr [ebp - 8], eax
        mov edx, dword ptr [esp + 144h]
    asm_101:
        mov eax, dword ptr [ebp + 8]
        mov ebx, dword ptr [ebp - 8]
        mov dword ptr [ebp - 4], eax
        mov esi, dword ptr [esp + 64h]
        add dword ptr [esp], eax
    asm_102:
        mov eax, dword ptr [esp + 11ch]
        xor ecx, ecx
        mov cl, byte ptr [ebx]
        cmp ecx, 0
        je asm_103
        cmp edx, dword ptr [esi]
        ja asm_103
        mov dword ptr [esi], edx
        mov eax, dword ptr [eax + ecx*4]
        mov word ptr [edi], ax
    asm_103:
        add edi, 2
        add esi, 4
        inc ebx
        sub dword ptr [ebp - 4], 1
        jg asm_102
        mov eax, dword ptr [esp + 64h]
        mov ebx, dword ptr [ebp - 8]
        add eax, dword ptr [esp + 3ch]
        add ebx, dword ptr [ebp + 10h]
        mov dword ptr [esp + 64h], eax
        mov dword ptr [ebp - 8], ebx
        mov edi, dword ptr [esp + 38h]
        mov ebx, dword ptr [esp + 10h]
        add edi, dword ptr [esp + 4]
        dec ebx
        mov dword ptr [esp + 38h], edi
        mov dword ptr [esp + 10h], ebx
        sub dword ptr [ebp + 0ch], 1
        jg asm_101
        mov esp, dword ptr [esp + 158h]
        pop edi
        pop esi
        pop ebx
        mov esp, ebp
        pop ebp
        ret
    }
}

/*
 * Naked assembly span renderer whose stack operands are not in its prototype. Saves ESP in the
 * context and derives source-byte, destination-word and depth-row addresses.
 */
// FUNCTION: FF7 0x006bc586
__declspec(naked) void __cdecl Gfx_RasterBlitIndexedRectDepthWrite(void)
{
    __asm {
        push ebp
        mov ebp, esp
        sub esp, 8
        push ebx
        push esi
        push edi
        mov eax, dword ptr [ebp + 14h]
        mov dword ptr [eax + 158h], esp
        mov esp, eax
        mov eax, dword ptr [esp + 0ch]
        mov edi, dword ptr [esp + 8]
        shl eax, 0ch
        mov esi, dword ptr [esp + 40h]
        add edi, eax
        add esi, eax
        mov eax, dword ptr [esp + 1ch]
        lea esi, [esi + eax*4]
        lea edi, [edi + eax*2]
        mov dword ptr [esp + 64h], esi
        mov dword ptr [esp + 38h], edi
        mov eax, dword ptr [esp + 118h]
        mov ecx, dword ptr [esp + 70h]
        mov edx, dword ptr [esp + 80h]
        lea eax, [eax + ecx]
        mov cl, byte ptr [esp + 10ch]
        shl edx, cl
        add eax, edx
        mov dword ptr [ebp - 8], eax
        mov edx, dword ptr [esp + 144h]
    asm_104:
        mov eax, dword ptr [ebp + 8]
        mov ebx, dword ptr [ebp - 8]
        mov dword ptr [ebp - 4], eax
        mov esi, dword ptr [esp + 64h]
        add dword ptr [esp], eax
    asm_105:
        cmp edx, dword ptr [esi]
        ja asm_106
        mov dword ptr [esi], edx
        mov eax, dword ptr [esp + 11ch]
        xor ecx, ecx
        mov cl, byte ptr [ebx]
        mov eax, dword ptr [eax + ecx*4]
        mov word ptr [edi], ax
    asm_106:
        add edi, 2
        add esi, 4
        inc ebx
        sub dword ptr [ebp - 4], 1
        jg asm_105
        mov eax, dword ptr [esp + 64h]
        mov ebx, dword ptr [ebp - 8]
        add eax, dword ptr [esp + 3ch]
        add ebx, dword ptr [ebp + 10h]
        mov dword ptr [esp + 64h], eax
        mov dword ptr [ebp - 8], ebx
        mov edi, dword ptr [esp + 38h]
        mov ebx, dword ptr [esp + 10h]
        add edi, dword ptr [esp + 4]
        dec ebx
        mov dword ptr [esp + 38h], edi
        mov dword ptr [esp + 10h], ebx
        sub dword ptr [ebp + 0ch], 1
        jg asm_104
        mov esp, dword ptr [esp + 158h]
        pop edi
        pop esi
        pop ebx
        mov esp, ebp
        pop ebp
        ret
    }
}

/*
 * Naked assembly span renderer whose stack operands are not in its prototype. Saves ESP in the
 * context, initializes source and destination row pointers and fixed-point palette coordinates,
 * and skips source bytes equal to zero.
 */
// FUNCTION: FF7 0x006bc65b
__declspec(naked) void __cdecl Gfx_RasterBlitIndexedRectColorKeyBlend(void)
{
    __asm {
        push ebp
        mov ebp, esp
        sub esp, 8
        push ebx
        push esi
        push edi
        mov eax, dword ptr [ebp + 14h]
        mov dword ptr [eax + 158h], esp
        mov esp, eax
        mov eax, dword ptr [esp + 0ch]
        mov edi, dword ptr [esp + 8]
        shl eax, 0ch
        add edi, eax
        mov eax, dword ptr [esp + 1ch]
        lea edi, [edi + eax*2]
        mov dword ptr [esp + 38h], edi
        mov eax, dword ptr [esp + 118h]
        mov ecx, dword ptr [esp + 70h]
        mov edx, dword ptr [esp + 80h]
        lea eax, [eax + ecx]
        mov cl, byte ptr [esp + 10ch]
        shl edx, cl
        add eax, edx
        mov dword ptr [ebp - 8], eax
        mov esi, dword ptr [esp + 11ch]
    asm_107:
        mov eax, dword ptr [ebp + 8]
        mov ebx, dword ptr [ebp - 8]
        mov dword ptr [ebp - 4], eax
        add dword ptr [esp], eax
    asm_108:
        xor ecx, ecx
        mov cl, byte ptr [ebx]
        cmp ecx, 0
        je asm_109
        mov eax, dword ptr [esi + ecx*4]
        mov ecx, dword ptr [esp + 13ch]
        xor edx, edx
        mov dx, word ptr [edi]
        mov edx, dword ptr [ecx + edx*4]
        mov eax, dword ptr [ecx + eax*4]
        add eax, edx
        mov word ptr [edi], ax
    asm_109:
        add edi, 2
        inc ebx
        sub dword ptr [ebp - 4], 1
        jg asm_108
        mov ebx, dword ptr [ebp - 8]
        add ebx, dword ptr [ebp + 10h]
        mov dword ptr [ebp - 8], ebx
        mov edi, dword ptr [esp + 38h]
        mov ebx, dword ptr [esp + 10h]
        add edi, dword ptr [esp + 4]
        dec ebx
        mov dword ptr [esp + 38h], edi
        mov dword ptr [esp + 10h], ebx
        sub dword ptr [ebp + 0ch], 1
        jg asm_107
        mov esp, dword ptr [esp + 158h]
        pop edi
        pop esi
        pop ebx
        mov esp, ebp
        pop ebp
        ret
    }
}

/*
 * Naked compositing span renderer whose stack operands are not in its prototype. Saves ESP in
 * the span context and derives output/source rows.
 */
// FUNCTION: FF7 0x006bc71c
__declspec(naked) void __cdecl Gfx_RasterBlitIndexedRectBlend(void)
{
    __asm {
        push ebp
        mov ebp, esp
        sub esp, 8
        push ebx
        push esi
        push edi
        mov eax, dword ptr [ebp + 14h]
        mov dword ptr [eax + 158h], esp
        mov esp, eax
        mov eax, dword ptr [esp + 0ch]
        mov edi, dword ptr [esp + 8]
        shl eax, 0ch
        add edi, eax
        mov eax, dword ptr [esp + 1ch]
        lea edi, [edi + eax*2]
        mov dword ptr [esp + 38h], edi
        mov eax, dword ptr [esp + 118h]
        mov ecx, dword ptr [esp + 70h]
        mov edx, dword ptr [esp + 80h]
        lea eax, [eax + ecx]
        mov cl, byte ptr [esp + 10ch]
        shl edx, cl
        add eax, edx
        mov dword ptr [ebp - 8], eax
        mov esi, dword ptr [esp + 11ch]
    asm_110:
        mov eax, dword ptr [ebp + 8]
        mov ebx, dword ptr [ebp - 8]
        mov dword ptr [ebp - 4], eax
        add dword ptr [esp], eax
    asm_111:
        xor ecx, ecx
        mov cl, byte ptr [ebx]
        mov eax, dword ptr [esi + ecx*4]
        mov ecx, dword ptr [esp + 13ch]
        xor edx, edx
        mov dx, word ptr [edi]
        mov edx, dword ptr [ecx + edx*4]
        mov eax, dword ptr [ecx + eax*4]
        add eax, edx
        mov word ptr [edi], ax
        add edi, 2
        inc ebx
        sub dword ptr [ebp - 4], 1
        jg asm_111
        mov ebx, dword ptr [ebp - 8]
        add ebx, dword ptr [ebp + 10h]
        mov dword ptr [ebp - 8], ebx
        mov edi, dword ptr [esp + 38h]
        mov ebx, dword ptr [esp + 10h]
        add edi, dword ptr [esp + 4]
        dec ebx
        mov dword ptr [esp + 38h], edi
        mov dword ptr [esp + 10h], ebx
        sub dword ptr [ebp + 0ch], 1
        jg asm_110
        mov esp, dword ptr [esp + 158h]
        pop edi
        pop esi
        pop ebx
        mov esp, ebp
        pop ebp
        ret
    }
}

/*
 * Naked assembly depth-tested composite span renderer whose stack operands are not in its
 * prototype. Saves ESP in the context and derives source, color and depth row pointers.
 */
// FUNCTION: FF7 0x006bc7d8
__declspec(naked) void __cdecl Gfx_RasterBlitIndexedRectColorKeyDepthTestBlend(void)
{
    __asm {
        push ebp
        mov ebp, esp
        sub esp, 8
        push ebx
        push esi
        push edi
        mov eax, dword ptr [ebp + 14h]
        mov dword ptr [eax + 158h], esp
        mov esp, eax
        mov eax, dword ptr [esp + 0ch]
        mov edi, dword ptr [esp + 8]
        shl eax, 0ch
        mov esi, dword ptr [esp + 40h]
        add edi, eax
        add esi, eax
        mov eax, dword ptr [esp + 1ch]
        lea esi, [esi + eax*4]
        lea edi, [edi + eax*2]
        mov dword ptr [esp + 64h], esi
        mov dword ptr [esp + 38h], edi
        mov eax, dword ptr [esp + 118h]
        mov ecx, dword ptr [esp + 70h]
        mov edx, dword ptr [esp + 80h]
        lea eax, [eax + ecx]
        mov cl, byte ptr [esp + 10ch]
        shl edx, cl
        add eax, edx
        mov dword ptr [ebp - 8], eax
        mov edx, dword ptr [esp + 144h]
    asm_112:
        mov eax, dword ptr [ebp + 8]
        mov ebx, dword ptr [ebp - 8]
        mov dword ptr [ebp - 4], eax
        mov esi, dword ptr [esp + 64h]
        add dword ptr [esp], eax
    asm_113:
        mov eax, dword ptr [esp + 11ch]
        xor ecx, ecx
        mov cl, byte ptr [ebx]
        cmp ecx, 0
        je asm_114
        cmp edx, dword ptr [esi]
        ja asm_114
        mov eax, dword ptr [eax + ecx*4]
        mov ecx, dword ptr [esp + 13ch]
        xor edx, edx
        mov dx, word ptr [edi]
        mov edx, dword ptr [ecx + edx*4]
        mov eax, dword ptr [ecx + eax*4]
        add eax, edx
        mov word ptr [edi], ax
        mov edx, dword ptr [esp + 144h]
    asm_114:
        add edi, 2
        add esi, 4
        inc ebx
        sub dword ptr [ebp - 4], 1
        jg asm_113
        mov eax, dword ptr [esp + 64h]
        mov ebx, dword ptr [ebp - 8]
        add eax, dword ptr [esp + 3ch]
        add ebx, dword ptr [ebp + 10h]
        mov dword ptr [esp + 64h], eax
        mov dword ptr [ebp - 8], ebx
        mov edi, dword ptr [esp + 38h]
        mov ebx, dword ptr [esp + 10h]
        add edi, dword ptr [esp + 4]
        dec ebx
        mov dword ptr [esp + 38h], edi
        mov dword ptr [esp + 10h], ebx
        sub dword ptr [ebp + 0ch], 1
        jg asm_112
        mov esp, dword ptr [esp + 158h]
        pop edi
        pop esi
        pop ebx
        mov esp, ebp
        pop ebp
        ret
    }
}

/*
 * Naked assembly depth-tested composite span renderer whose stack operands are not in its
 * prototype. Saves ESP in the context and derives source, color and depth row pointers.
 */
// FUNCTION: FF7 0x006bc8cf
__declspec(naked) void __cdecl Gfx_RasterBlitIndexedRectDepthTestBlend(void)
{
    __asm {
        push ebp
        mov ebp, esp
        sub esp, 8
        push ebx
        push esi
        push edi
        mov eax, dword ptr [ebp + 14h]
        mov dword ptr [eax + 158h], esp
        mov esp, eax
        mov eax, dword ptr [esp + 0ch]
        mov edi, dword ptr [esp + 8]
        shl eax, 0ch
        mov esi, dword ptr [esp + 40h]
        add edi, eax
        add esi, eax
        mov eax, dword ptr [esp + 1ch]
        lea esi, [esi + eax*4]
        lea edi, [edi + eax*2]
        mov dword ptr [esp + 64h], esi
        mov dword ptr [esp + 38h], edi
        mov eax, dword ptr [esp + 118h]
        mov ecx, dword ptr [esp + 70h]
        mov edx, dword ptr [esp + 80h]
        lea eax, [eax + ecx]
        mov cl, byte ptr [esp + 10ch]
        shl edx, cl
        add eax, edx
        mov dword ptr [ebp - 8], eax
        mov edx, dword ptr [esp + 144h]
    asm_115:
        mov eax, dword ptr [ebp + 8]
        mov ebx, dword ptr [ebp - 8]
        mov dword ptr [ebp - 4], eax
        mov esi, dword ptr [esp + 64h]
        add dword ptr [esp], eax
    asm_116:
        cmp edx, dword ptr [esi]
        ja asm_117
        mov eax, dword ptr [esp + 11ch]
        xor ecx, ecx
        mov cl, byte ptr [ebx]
        mov eax, dword ptr [eax + ecx*4]
        mov ecx, dword ptr [esp + 13ch]
        xor edx, edx
        mov dx, word ptr [edi]
        mov edx, dword ptr [ecx + edx*4]
        mov eax, dword ptr [ecx + eax*4]
        add eax, edx
        mov word ptr [edi], ax
        mov edx, dword ptr [esp + 144h]
    asm_117:
        add edi, 2
        add esi, 4
        inc ebx
        sub dword ptr [ebp - 4], 1
        jg asm_116
        mov eax, dword ptr [esp + 64h]
        mov ebx, dword ptr [ebp - 8]
        add eax, dword ptr [esp + 3ch]
        add ebx, dword ptr [ebp + 10h]
        mov dword ptr [esp + 64h], eax
        mov dword ptr [ebp - 8], ebx
        mov edi, dword ptr [esp + 38h]
        mov ebx, dword ptr [esp + 10h]
        add edi, dword ptr [esp + 4]
        dec ebx
        mov dword ptr [esp + 38h], edi
        mov dword ptr [esp + 10h], ebx
        sub dword ptr [ebp + 0ch], 1
        jg asm_115
        mov esp, dword ptr [esp + 158h]
        pop edi
        pop esi
        pop ebx
        mov esp, ebp
        pop ebp
        ret
    }
}

/*
 * Naked assembly span renderer whose stack operands are not in its prototype. Saves ESP in the
 * context, derives byte-source and word-destination row pointers and skips source bytes equal
 * to zero.
 */
// FUNCTION: FF7 0x006bc9bd
__declspec(naked) void __cdecl Gfx_RasterBlitIndexedRectColorKeyBlendRemap(void)
{
    __asm {
        push ebp
        mov ebp, esp
        sub esp, 8
        push ebx
        push esi
        push edi
        mov eax, dword ptr [ebp + 14h]
        mov dword ptr [eax + 158h], esp
        mov esp, eax
        mov eax, dword ptr [esp + 0ch]
        mov edi, dword ptr [esp + 8]
        shl eax, 0ch
        add edi, eax
        mov eax, dword ptr [esp + 1ch]
        lea edi, [edi + eax*2]
        mov dword ptr [esp + 38h], edi
        mov eax, dword ptr [esp + 118h]
        mov ecx, dword ptr [esp + 70h]
        mov edx, dword ptr [esp + 80h]
        lea eax, [eax + ecx]
        mov cl, byte ptr [esp + 10ch]
        shl edx, cl
        add eax, edx
        mov dword ptr [ebp - 8], eax
        mov esi, dword ptr [esp + 11ch]
    asm_118:
        mov eax, dword ptr [ebp + 8]
        mov ebx, dword ptr [ebp - 8]
        mov dword ptr [ebp - 4], eax
        add dword ptr [esp], eax
    asm_119:
        xor ecx, ecx
        mov cl, byte ptr [ebx]
        cmp ecx, 0
        je asm_120
        mov eax, dword ptr [esi + ecx*4]
        mov ecx, dword ptr [esp + 13ch]
        xor edx, edx
        mov dx, word ptr [edi]
        mov edx, dword ptr [ecx + edx*4]
        mov eax, dword ptr [ecx + eax*4]
        add edx, eax
        mov ecx, dword ptr [esp + 140h]
        mov eax, dword ptr [ecx + edx*4]
        mov word ptr [edi], ax
    asm_120:
        add edi, 2
        inc ebx
        sub dword ptr [ebp - 4], 1
        jg asm_119
        mov ebx, dword ptr [ebp - 8]
        add ebx, dword ptr [ebp + 10h]
        mov dword ptr [ebp - 8], ebx
        mov edi, dword ptr [esp + 38h]
        mov ebx, dword ptr [esp + 10h]
        add edi, dword ptr [esp + 4]
        dec ebx
        mov dword ptr [esp + 38h], edi
        mov dword ptr [esp + 10h], ebx
        sub dword ptr [ebp + 0ch], 1
        jg asm_118
        mov esp, dword ptr [esp + 158h]
        pop edi
        pop esi
        pop ebx
        mov esp, ebp
        pop ebp
        ret
    }
}

/*
 * Naked palette-compositing span renderer with stack operands omitted by its void prototype.
 * Saves the real ESP in its span context, derives source/output rows and maps each source byte
 * through the context palette.
 */
// FUNCTION: FF7 0x006bca88
__declspec(naked) void __cdecl Gfx_RasterBlitIndexedRectBlendRemap(void)
{
    __asm {
        push ebp
        mov ebp, esp
        sub esp, 8
        push ebx
        push esi
        push edi
        mov eax, dword ptr [ebp + 14h]
        mov dword ptr [eax + 158h], esp
        mov esp, eax
        mov eax, dword ptr [esp + 0ch]
        mov edi, dword ptr [esp + 8]
        shl eax, 0ch
        add edi, eax
        mov eax, dword ptr [esp + 1ch]
        lea edi, [edi + eax*2]
        mov dword ptr [esp + 38h], edi
        mov eax, dword ptr [esp + 118h]
        mov ecx, dword ptr [esp + 70h]
        mov edx, dword ptr [esp + 80h]
        lea eax, [eax + ecx]
        mov cl, byte ptr [esp + 10ch]
        shl edx, cl
        add eax, edx
        mov dword ptr [ebp - 8], eax
        mov esi, dword ptr [esp + 11ch]
    asm_121:
        mov eax, dword ptr [ebp + 8]
        mov ebx, dword ptr [ebp - 8]
        mov dword ptr [ebp - 4], eax
        add dword ptr [esp], eax
    asm_122:
        xor ecx, ecx
        mov cl, byte ptr [ebx]
        mov eax, dword ptr [esi + ecx*4]
        mov ecx, dword ptr [esp + 13ch]
        xor edx, edx
        mov dx, word ptr [edi]
        mov edx, dword ptr [ecx + edx*4]
        mov eax, dword ptr [ecx + eax*4]
        add edx, eax
        mov ecx, dword ptr [esp + 140h]
        mov eax, dword ptr [ecx + edx*4]
        mov word ptr [edi], ax
        add edi, 2
        inc ebx
        sub dword ptr [ebp - 4], 1
        jg asm_122
        mov ebx, dword ptr [ebp - 8]
        add ebx, dword ptr [ebp + 10h]
        mov dword ptr [ebp - 8], ebx
        mov edi, dword ptr [esp + 38h]
        mov ebx, dword ptr [esp + 10h]
        add edi, dword ptr [esp + 4]
        dec ebx
        mov dword ptr [esp + 38h], edi
        mov dword ptr [esp + 10h], ebx
        sub dword ptr [ebp + 0ch], 1
        jg asm_121
        mov esp, dword ptr [esp + 158h]
        pop edi
        pop esi
        pop ebx
        mov esp, ebp
        pop ebp
        ret
    }
}

/*
 * Naked assembly depth-tested composite span renderer whose stack operands are not in its
 * prototype. Saves ESP in the context and derives source, color and depth row pointers.
 */
// FUNCTION: FF7 0x006bcb4e
__declspec(naked) void __cdecl Gfx_RasterBlitIndexedRectColorKeyDepthTestBlendRemap(void)
{
    __asm {
        push ebp
        mov ebp, esp
        sub esp, 8
        push ebx
        push esi
        push edi
        mov eax, dword ptr [ebp + 14h]
        mov dword ptr [eax + 158h], esp
        mov esp, eax
        mov eax, dword ptr [esp + 0ch]
        mov edi, dword ptr [esp + 8]
        shl eax, 0ch
        mov esi, dword ptr [esp + 40h]
        add edi, eax
        add esi, eax
        mov eax, dword ptr [esp + 1ch]
        lea esi, [esi + eax*4]
        lea edi, [edi + eax*2]
        mov dword ptr [esp + 64h], esi
        mov dword ptr [esp + 38h], edi
        mov eax, dword ptr [esp + 118h]
        mov ecx, dword ptr [esp + 70h]
        mov edx, dword ptr [esp + 80h]
        lea eax, [eax + ecx]
        mov cl, byte ptr [esp + 10ch]
        shl edx, cl
        add eax, edx
        mov dword ptr [ebp - 8], eax
        mov edx, dword ptr [esp + 144h]
    asm_123:
        mov eax, dword ptr [ebp + 8]
        mov ebx, dword ptr [ebp - 8]
        mov dword ptr [ebp - 4], eax
        mov esi, dword ptr [esp + 64h]
        add dword ptr [esp], eax
    asm_124:
        mov eax, dword ptr [esp + 11ch]
        xor ecx, ecx
        mov cl, byte ptr [ebx]
        cmp ecx, 0
        je asm_125
        cmp edx, dword ptr [esi]
        ja asm_125
        mov eax, dword ptr [eax + ecx*4]
        mov ecx, dword ptr [esp + 13ch]
        xor edx, edx
        mov dx, word ptr [edi]
        mov edx, dword ptr [ecx + edx*4]
        mov eax, dword ptr [ecx + eax*4]
        add edx, eax
        mov ecx, dword ptr [esp + 140h]
        mov eax, dword ptr [ecx + edx*4]
        mov word ptr [edi], ax
        mov edx, dword ptr [esp + 144h]
    asm_125:
        add edi, 2
        add esi, 4
        inc ebx
        sub dword ptr [ebp - 4], 1
        jg asm_124
        mov eax, dword ptr [esp + 64h]
        mov ebx, dword ptr [ebp - 8]
        add eax, dword ptr [esp + 3ch]
        add ebx, dword ptr [ebp + 10h]
        mov dword ptr [esp + 64h], eax
        mov dword ptr [ebp - 8], ebx
        mov edi, dword ptr [esp + 38h]
        mov ebx, dword ptr [esp + 10h]
        add edi, dword ptr [esp + 4]
        dec ebx
        mov dword ptr [esp + 38h], edi
        mov dword ptr [esp + 10h], ebx
        sub dword ptr [ebp + 0ch], 1
        jg asm_123
        mov esp, dword ptr [esp + 158h]
        pop edi
        pop esi
        pop ebx
        mov esp, ebp
        pop ebp
        ret
    }
}

/*
 * Naked assembly depth-tested composite span renderer whose stack operands are not in its
 * prototype. Saves ESP in the context and derives source, color and depth row pointers.
 */
// FUNCTION: FF7 0x006bcc4f
__declspec(naked) void __cdecl Gfx_RasterBlitIndexedRectDepthTestBlendRemap(void)
{
    __asm {
        push ebp
        mov ebp, esp
        sub esp, 8
        push ebx
        push esi
        push edi
        mov eax, dword ptr [ebp + 14h]
        mov dword ptr [eax + 158h], esp
        mov esp, eax
        mov eax, dword ptr [esp + 0ch]
        mov edi, dword ptr [esp + 8]
        shl eax, 0ch
        mov esi, dword ptr [esp + 40h]
        add edi, eax
        add esi, eax
        mov eax, dword ptr [esp + 1ch]
        lea esi, [esi + eax*4]
        lea edi, [edi + eax*2]
        mov dword ptr [esp + 64h], esi
        mov dword ptr [esp + 38h], edi
        mov eax, dword ptr [esp + 118h]
        mov ecx, dword ptr [esp + 70h]
        mov edx, dword ptr [esp + 80h]
        lea eax, [eax + ecx]
        mov cl, byte ptr [esp + 10ch]
        shl edx, cl
        add eax, edx
        mov dword ptr [ebp - 8], eax
        mov edx, dword ptr [esp + 144h]
    asm_126:
        mov eax, dword ptr [ebp + 8]
        mov ebx, dword ptr [ebp - 8]
        mov dword ptr [ebp - 4], eax
        mov esi, dword ptr [esp + 64h]
        add dword ptr [esp], eax
    asm_127:
        cmp edx, dword ptr [esi]
        ja asm_128
        mov eax, dword ptr [esp + 11ch]
        xor ecx, ecx
        mov cl, byte ptr [ebx]
        mov eax, dword ptr [eax + ecx*4]
        mov ecx, dword ptr [esp + 13ch]
        xor edx, edx
        mov dx, word ptr [edi]
        mov edx, dword ptr [ecx + edx*4]
        mov eax, dword ptr [ecx + eax*4]
        add edx, eax
        mov ecx, dword ptr [esp + 140h]
        mov eax, dword ptr [ecx + edx*4]
        mov word ptr [edi], ax
        mov edx, dword ptr [esp + 144h]
    asm_128:
        add edi, 2
        add esi, 4
        inc ebx
        sub dword ptr [ebp - 4], 1
        jg asm_127
        mov eax, dword ptr [esp + 64h]
        mov ebx, dword ptr [ebp - 8]
        add eax, dword ptr [esp + 3ch]
        add ebx, dword ptr [ebp + 10h]
        mov dword ptr [esp + 64h], eax
        mov dword ptr [ebp - 8], ebx
        mov edi, dword ptr [esp + 38h]
        mov ebx, dword ptr [esp + 10h]
        add edi, dword ptr [esp + 4]
        dec ebx
        mov dword ptr [esp + 38h], edi
        mov dword ptr [esp + 10h], ebx
        sub dword ptr [ebp + 0ch], 1
        jg asm_126
        mov esp, dword ptr [esp + 158h]
        pop edi
        pop esi
        pop ebx
        mov esp, ebp
        pop ebp
        ret
    }
}

/*
 * Naked assembly enlarged palette span renderer whose stack operands are not in its prototype.
 * Arithmetic-shifts width and height right by one and returns if either result is nonpositive.
 */
// FUNCTION: FF7 0x006bcd4b
__declspec(naked) void __cdecl Gfx_RasterBlitIndexedRectDoubledColorKey(void)
{
    __asm {
        push ebp
        mov ebp, esp
        push ecx
        push ebx
        push esi
        push edi
        mov eax, dword ptr [ebp + 8]
        sar eax, 1
        mov dword ptr [ebp + 8], eax
        mov ecx, dword ptr [ebp + 0ch]
        sar ecx, 1
        mov dword ptr [ebp + 0ch], ecx
        cmp dword ptr [ebp + 8], 0
        jle asm_132
        cmp dword ptr [ebp + 0ch], 0
        jle asm_132
        mov eax, dword ptr [ebp + 14h]
        mov dword ptr [eax + 158h], esp
        mov esp, eax
        mov eax, dword ptr [esp + 0ch]
        mov edi, dword ptr [esp + 8]
        shl eax, 0ch
        add edi, eax
        mov eax, dword ptr [esp + 1ch]
        lea edi, [edi + eax*2]
        mov dword ptr [esp + 38h], edi
        mov eax, dword ptr [esp + 118h]
        mov ecx, dword ptr [esp + 70h]
        mov edx, dword ptr [esp + 80h]
        lea eax, [eax + ecx]
        mov cl, byte ptr [esp + 10ch]
        shl edx, cl
        add eax, edx
        mov dword ptr [ebp - 4], eax
        mov esi, dword ptr [esp + 11ch]
    asm_129:
        mov ebx, dword ptr [ebp - 4]
        mov edx, dword ptr [ebp + 8]
        add dword ptr [esp], edx
    asm_130:
        xor ecx, ecx
        mov cl, byte ptr [ebx]
        cmp ecx, 0
        je asm_131
        mov eax, dword ptr [esi + ecx*4]
        mov ecx, eax
        shl eax, 10h
        or eax, ecx
        mov dword ptr [edi], eax
        mov dword ptr [edi + 1000h], eax
    asm_131:
        add edi, 4
        inc ebx
        dec edx
        jg asm_130
        mov ebx, dword ptr [ebp - 4]
        add ebx, dword ptr [ebp + 10h]
        mov dword ptr [ebp - 4], ebx
        mov edi, dword ptr [esp + 38h]
        mov ebx, dword ptr [esp + 10h]
        add edi, dword ptr [esp + 4]
        add edi, dword ptr [esp + 4]
        dec ebx
        mov dword ptr [esp + 38h], edi
        mov dword ptr [esp + 10h], ebx
        sub dword ptr [ebp + 0ch], 1
        jg asm_129
        mov esp, dword ptr [esp + 158h]
    asm_132:
        pop edi
        pop esi
        pop ebx
        mov esp, ebp
        pop ebp
        ret
    }
}

/*
 * Naked assembly enlarged palette span renderer whose stack operands are not in its prototype.
 * Arithmetic-shifts supplied width and height right by one and returns when either result is
 * nonpositive.
 */
// FUNCTION: FF7 0x006bce24
__declspec(naked) void __cdecl Gfx_RasterBlitIndexedRectDoubled(void)
{
    __asm {
        push ebp
        mov ebp, esp
        push ecx
        push ebx
        push esi
        push edi
        mov eax, dword ptr [ebp + 8]
        sar eax, 1
        mov dword ptr [ebp + 8], eax
        mov ecx, dword ptr [ebp + 0ch]
        sar ecx, 1
        mov dword ptr [ebp + 0ch], ecx
        cmp dword ptr [ebp + 8], 0
        jle asm_135
        cmp dword ptr [ebp + 0ch], 0
        jle asm_135
        mov eax, dword ptr [ebp + 14h]
        mov dword ptr [eax + 158h], esp
        mov esp, eax
        mov eax, dword ptr [esp + 0ch]
        mov edi, dword ptr [esp + 8]
        shl eax, 0ch
        add edi, eax
        mov eax, dword ptr [esp + 1ch]
        lea edi, [edi + eax*2]
        mov dword ptr [esp + 38h], edi
        mov eax, dword ptr [esp + 118h]
        mov ecx, dword ptr [esp + 70h]
        mov edx, dword ptr [esp + 80h]
        lea eax, [eax + ecx]
        mov cl, byte ptr [esp + 10ch]
        shl edx, cl
        add eax, edx
        mov dword ptr [ebp - 4], eax
        mov esi, dword ptr [esp + 11ch]
    asm_133:
        mov ebx, dword ptr [ebp - 4]
        mov edx, dword ptr [ebp + 8]
        add dword ptr [esp], edx
    asm_134:
        xor ecx, ecx
        mov cl, byte ptr [ebx]
        mov eax, dword ptr [esi + ecx*4]
        mov ecx, eax
        shl eax, 10h
        or eax, ecx
        mov dword ptr [edi], eax
        mov dword ptr [edi + 1000h], eax
        add edi, 4
        inc ebx
        dec edx
        jg asm_134
        mov ebx, dword ptr [ebp - 4]
        add ebx, dword ptr [ebp + 10h]
        mov dword ptr [ebp - 4], ebx
        mov edi, dword ptr [esp + 38h]
        mov ebx, dword ptr [esp + 10h]
        add edi, dword ptr [esp + 4]
        add edi, dword ptr [esp + 4]
        dec ebx
        mov dword ptr [esp + 38h], edi
        mov dword ptr [esp + 10h], ebx
        sub dword ptr [ebp + 0ch], 1
        jg asm_133
        mov esp, dword ptr [esp + 158h]
    asm_135:
        pop edi
        pop esi
        pop ebx
        mov esp, ebp
        pop ebp
        ret
    }
}

/*
 * Naked assembly enlarged depth-writing palette span renderer whose stack operands are not in
 * its prototype. Arithmetic-shifts width and height right one and returns if either result is
 * nonpositive.
 */
// FUNCTION: FF7 0x006bcef8
__declspec(naked) void __cdecl Gfx_RasterBlitIndexedRectDoubledColorKeyDepthWrite(void)
{
    __asm {
        push ebp
        mov ebp, esp
        sub esp, 8
        push ebx
        push esi
        push edi
        mov eax, dword ptr [ebp + 8]
        sar eax, 1
        mov dword ptr [ebp + 8], eax
        mov ecx, dword ptr [ebp + 0ch]
        sar ecx, 1
        mov dword ptr [ebp + 0ch], ecx
        cmp dword ptr [ebp + 8], 0
        jle asm_139
        cmp dword ptr [ebp + 0ch], 0
        jle asm_139
        mov eax, dword ptr [ebp + 14h]
        mov dword ptr [eax + 158h], esp
        mov esp, eax
        mov eax, dword ptr [esp + 0ch]
        mov edi, dword ptr [esp + 8]
        shl eax, 0ch
        mov esi, dword ptr [esp + 40h]
        add edi, eax
        add esi, eax
        mov eax, dword ptr [esp + 1ch]
        lea esi, [esi + eax*4]
        lea edi, [edi + eax*2]
        mov dword ptr [esp + 64h], esi
        mov dword ptr [esp + 38h], edi
        mov eax, dword ptr [esp + 118h]
        mov ecx, dword ptr [esp + 70h]
        mov edx, dword ptr [esp + 80h]
        lea eax, [eax + ecx]
        mov cl, byte ptr [esp + 10ch]
        shl edx, cl
        add eax, edx
        mov dword ptr [ebp - 8], eax
        mov edx, dword ptr [esp + 144h]
    asm_136:
        mov eax, dword ptr [ebp + 8]
        mov ebx, dword ptr [ebp - 8]
        mov dword ptr [ebp - 4], eax
        mov esi, dword ptr [esp + 64h]
        add dword ptr [esp], eax
    asm_137:
        mov eax, dword ptr [esp + 11ch]
        xor ecx, ecx
        mov cl, byte ptr [ebx]
        cmp ecx, 0
        je asm_138
        cmp edx, dword ptr [esi]
        ja asm_138
        mov eax, dword ptr [eax + ecx*4]
        mov dword ptr [esi], edx
        mov ecx, eax
        mov dword ptr [esi + 4], edx
        shl eax, 10h
        or eax, ecx
        mov dword ptr [esi + 1000h], edx
        mov dword ptr [edi], eax
        mov dword ptr [esi + 1004h], edx
        mov dword ptr [edi + 1000h], eax
    asm_138:
        add edi, 4
        add esi, 8
        inc ebx
        sub dword ptr [ebp - 4], 1
        jg asm_137
        mov eax, dword ptr [esp + 64h]
        mov ebx, dword ptr [ebp - 8]
        add eax, dword ptr [esp + 3ch]
        add eax, dword ptr [esp + 3ch]
        add ebx, dword ptr [ebp + 10h]
        mov dword ptr [esp + 64h], eax
        mov dword ptr [ebp - 8], ebx
        mov edi, dword ptr [esp + 38h]
        mov ebx, dword ptr [esp + 10h]
        add edi, dword ptr [esp + 4]
        add edi, dword ptr [esp + 4]
        dec ebx
        mov dword ptr [esp + 38h], edi
        mov dword ptr [esp + 10h], ebx
        sub dword ptr [ebp + 0ch], 1
        jg asm_136
        mov esp, dword ptr [esp + 158h]
    asm_139:
        pop edi
        pop esi
        pop ebx
        mov esp, ebp
        pop ebp
        ret
    }
}

/*
 * Naked assembly enlarged depth-writing palette span renderer whose stack operands are not in
 * its prototype. Arithmetic-shifts width and height right one and returns if either result is
 * nonpositive.
 */
// FUNCTION: FF7 0x006bd01d
__declspec(naked) void __cdecl Gfx_RasterBlitIndexedRectDoubledDepthWrite(void)
{
    __asm {
        push ebp
        mov ebp, esp
        sub esp, 8
        push ebx
        push esi
        push edi
        mov eax, dword ptr [ebp + 8]
        sar eax, 1
        mov dword ptr [ebp + 8], eax
        mov ecx, dword ptr [ebp + 0ch]
        sar ecx, 1
        mov dword ptr [ebp + 0ch], ecx
        cmp dword ptr [ebp + 8], 0
        jle asm_143
        cmp dword ptr [ebp + 0ch], 0
        jle asm_143
        mov eax, dword ptr [ebp + 14h]
        mov dword ptr [eax + 158h], esp
        mov esp, eax
        mov eax, dword ptr [esp + 0ch]
        mov edi, dword ptr [esp + 8]
        shl eax, 0ch
        mov esi, dword ptr [esp + 40h]
        add edi, eax
        add esi, eax
        mov eax, dword ptr [esp + 1ch]
        lea esi, [esi + eax*4]
        lea edi, [edi + eax*2]
        mov dword ptr [esp + 64h], esi
        mov dword ptr [esp + 38h], edi
        mov eax, dword ptr [esp + 118h]
        mov ecx, dword ptr [esp + 70h]
        mov edx, dword ptr [esp + 80h]
        lea eax, [eax + ecx]
        mov cl, byte ptr [esp + 10ch]
        shl edx, cl
        add eax, edx
        mov dword ptr [ebp - 8], eax
        mov edx, dword ptr [esp + 144h]
    asm_140:
        mov eax, dword ptr [ebp + 8]
        mov ebx, dword ptr [ebp - 8]
        mov dword ptr [ebp - 4], eax
        mov esi, dword ptr [esp + 64h]
        add dword ptr [esp], eax
    asm_141:
        cmp edx, dword ptr [esi]
        ja asm_142
        mov eax, dword ptr [esp + 11ch]
        xor ecx, ecx
        mov cl, byte ptr [ebx]
        mov eax, dword ptr [eax + ecx*4]
        mov dword ptr [esi], edx
        mov ecx, eax
        mov dword ptr [esi + 4], edx
        shl eax, 10h
        or eax, ecx
        mov dword ptr [esi + 1000h], edx
        mov dword ptr [edi], eax
        mov dword ptr [esi + 1004h], edx
        mov dword ptr [edi + 1000h], eax
    asm_142:
        add edi, 4
        add esi, 8
        inc ebx
        sub dword ptr [ebp - 4], 1
        jg asm_141
        mov eax, dword ptr [esp + 64h]
        mov ebx, dword ptr [ebp - 8]
        add eax, dword ptr [esp + 3ch]
        add eax, dword ptr [esp + 3ch]
        add ebx, dword ptr [ebp + 10h]
        mov dword ptr [esp + 64h], eax
        mov dword ptr [ebp - 8], ebx
        mov edi, dword ptr [esp + 38h]
        mov ebx, dword ptr [esp + 10h]
        add edi, dword ptr [esp + 4]
        add edi, dword ptr [esp + 4]
        dec ebx
        mov dword ptr [esp + 38h], edi
        mov dword ptr [esp + 10h], ebx
        sub dword ptr [ebp + 0ch], 1
        jg asm_140
        mov esp, dword ptr [esp + 158h]
    asm_143:
        pop edi
        pop esi
        pop ebx
        mov esp, ebp
        pop ebp
        ret
    }
}

/*
 * Naked assembly enlarged composite span renderer whose stack operands are not in its
 * prototype. Arithmetic-shifts width and height right one and returns if either result is
 * nonpositive.
 */
// FUNCTION: FF7 0x006bd13d
__declspec(naked) void __cdecl Gfx_RasterBlitIndexedRectDoubledColorKeyBlendRemap(void)
{
    __asm {
        push ebp
        mov ebp, esp
        sub esp, 8
        push ebx
        push esi
        push edi
        mov eax, dword ptr [ebp + 8]
        sar eax, 1
        mov dword ptr [ebp + 8], eax
        mov ecx, dword ptr [ebp + 0ch]
        sar ecx, 1
        mov dword ptr [ebp + 0ch], ecx
        cmp dword ptr [ebp + 8], 0
        jle asm_147
        cmp dword ptr [ebp + 0ch], 0
        jle asm_147
        mov eax, dword ptr [ebp + 14h]
        mov dword ptr [eax + 158h], esp
        mov esp, eax
        mov eax, dword ptr [esp + 0ch]
        mov edi, dword ptr [esp + 8]
        shl eax, 0ch
        add edi, eax
        mov eax, dword ptr [esp + 1ch]
        lea edi, [edi + eax*2]
        mov dword ptr [esp + 38h], edi
        mov eax, dword ptr [esp + 118h]
        mov ecx, dword ptr [esp + 70h]
        mov edx, dword ptr [esp + 80h]
        lea eax, [eax + ecx]
        mov cl, byte ptr [esp + 10ch]
        shl edx, cl
        add eax, edx
        mov dword ptr [ebp - 8], eax
        mov esi, dword ptr [esp + 11ch]
    asm_144:
        mov eax, dword ptr [ebp + 8]
        mov ebx, dword ptr [ebp - 8]
        mov dword ptr [ebp - 4], eax
        add dword ptr [esp], eax
    asm_145:
        xor ecx, ecx
        mov cl, byte ptr [ebx]
        cmp ecx, 0
        je asm_146
        mov eax, dword ptr [esi + ecx*4]
        mov ecx, dword ptr [esp + 13ch]
        xor edx, edx
        mov dx, word ptr [edi]
        mov edx, dword ptr [ecx + edx*4]
        mov eax, dword ptr [ecx + eax*4]
        add edx, eax
        mov ecx, dword ptr [esp + 140h]
        mov eax, dword ptr [ecx + edx*4]
        mov ecx, eax
        shl eax, 10h
        or eax, ecx
        mov dword ptr [edi], eax
        mov dword ptr [edi + 1000h], eax
    asm_146:
        add edi, 4
        inc ebx
        sub dword ptr [ebp - 4], 1
        jg asm_145
        mov ebx, dword ptr [ebp - 8]
        add ebx, dword ptr [ebp + 10h]
        mov dword ptr [ebp - 8], ebx
        mov edi, dword ptr [esp + 38h]
        mov ebx, dword ptr [esp + 10h]
        add edi, dword ptr [esp + 4]
        add edi, dword ptr [esp + 4]
        dec ebx
        mov dword ptr [esp + 38h], edi
        mov dword ptr [esp + 10h], ebx
        sub dword ptr [ebp + 0ch], 1
        jg asm_144
        mov esp, dword ptr [esp + 158h]
    asm_147:
        pop edi
        pop esi
        pop ebx
        mov esp, ebp
        pop ebp
        ret
    }
}

/*
 * Naked assembly enlarged composite span renderer whose stack operands are not in its
 * prototype. Arithmetic-shifts width and height right one and returns if either result is
 * nonpositive.
 */
// FUNCTION: FF7 0x006bd23c
__declspec(naked) void __cdecl Gfx_RasterBlitIndexedRectDoubledBlendRemap(void)
{
    __asm {
        push ebp
        mov ebp, esp
        sub esp, 8
        push ebx
        push esi
        push edi
        mov eax, dword ptr [ebp + 8]
        sar eax, 1
        mov dword ptr [ebp + 8], eax
        mov ecx, dword ptr [ebp + 0ch]
        sar ecx, 1
        mov dword ptr [ebp + 0ch], ecx
        cmp dword ptr [ebp + 8], 0
        jle asm_150
        cmp dword ptr [ebp + 0ch], 0
        jle asm_150
        mov eax, dword ptr [ebp + 14h]
        mov dword ptr [eax + 158h], esp
        mov esp, eax
        mov eax, dword ptr [esp + 0ch]
        mov edi, dword ptr [esp + 8]
        shl eax, 0ch
        add edi, eax
        mov eax, dword ptr [esp + 1ch]
        lea edi, [edi + eax*2]
        mov dword ptr [esp + 38h], edi
        mov eax, dword ptr [esp + 118h]
        mov ecx, dword ptr [esp + 70h]
        mov edx, dword ptr [esp + 80h]
        lea eax, [eax + ecx]
        mov cl, byte ptr [esp + 10ch]
        shl edx, cl
        add eax, edx
        mov dword ptr [ebp - 8], eax
        mov esi, dword ptr [esp + 11ch]
    asm_148:
        mov eax, dword ptr [ebp + 8]
        mov ebx, dword ptr [ebp - 8]
        mov dword ptr [ebp - 4], eax
        add dword ptr [esp], eax
    asm_149:
        xor ecx, ecx
        mov cl, byte ptr [ebx]
        mov eax, dword ptr [esi + ecx*4]
        mov ecx, dword ptr [esp + 13ch]
        xor edx, edx
        mov dx, word ptr [edi]
        mov edx, dword ptr [ecx + edx*4]
        mov eax, dword ptr [ecx + eax*4]
        add edx, eax
        mov ecx, dword ptr [esp + 140h]
        mov eax, dword ptr [ecx + edx*4]
        mov ecx, eax
        shl eax, 10h
        or eax, ecx
        mov dword ptr [edi], eax
        mov dword ptr [edi + 1000h], eax
        add edi, 4
        inc ebx
        sub dword ptr [ebp - 4], 1
        jg asm_149
        mov ebx, dword ptr [ebp - 8]
        add ebx, dword ptr [ebp + 10h]
        mov dword ptr [ebp - 8], ebx
        mov edi, dword ptr [esp + 38h]
        mov ebx, dword ptr [esp + 10h]
        add edi, dword ptr [esp + 4]
        add edi, dword ptr [esp + 4]
        dec ebx
        mov dword ptr [esp + 38h], edi
        mov dword ptr [esp + 10h], ebx
        sub dword ptr [ebp + 0ch], 1
        jg asm_148
        mov esp, dword ptr [esp + 158h]
    asm_150:
        pop edi
        pop esi
        pop ebx
        mov esp, ebp
        pop ebp
        ret
    }
}

/*
 * Naked assembly enlarged depth-tested composite span renderer whose stack operands are not in
 * its prototype. Halves signed width and height and returns for nonpositive results.
 */
// FUNCTION: FF7 0x006bd336
__declspec(naked) void __cdecl Gfx_RasterBlitIndexedRectDoubledColorKeyDepthTestBlendRemap(void)
{
    __asm {
        push ebp
        mov ebp, esp
        sub esp, 8
        push ebx
        push esi
        push edi
        mov eax, dword ptr [ebp + 8]
        sar eax, 1
        mov dword ptr [ebp + 8], eax
        mov ecx, dword ptr [ebp + 0ch]
        sar ecx, 1
        mov dword ptr [ebp + 0ch], ecx
        cmp dword ptr [ebp + 8], 0
        jle asm_154
        cmp dword ptr [ebp + 0ch], 0
        jle asm_154
        mov eax, dword ptr [ebp + 14h]
        mov dword ptr [eax + 158h], esp
        mov esp, eax
        mov eax, dword ptr [esp + 0ch]
        mov edi, dword ptr [esp + 8]
        shl eax, 0ch
        mov esi, dword ptr [esp + 40h]
        add edi, eax
        add esi, eax
        mov eax, dword ptr [esp + 1ch]
        lea esi, [esi + eax*4]
        lea edi, [edi + eax*2]
        mov dword ptr [esp + 64h], esi
        mov dword ptr [esp + 38h], edi
        mov eax, dword ptr [esp + 118h]
        mov ecx, dword ptr [esp + 70h]
        mov edx, dword ptr [esp + 80h]
        lea eax, [eax + ecx]
        mov cl, byte ptr [esp + 10ch]
        shl edx, cl
        add eax, edx
        mov dword ptr [ebp - 8], eax
        mov edx, dword ptr [esp + 144h]
    asm_151:
        mov eax, dword ptr [ebp + 8]
        mov ebx, dword ptr [ebp - 8]
        mov dword ptr [ebp - 4], eax
        mov esi, dword ptr [esp + 64h]
        add dword ptr [esp], eax
    asm_152:
        mov eax, dword ptr [esp + 11ch]
        xor ecx, ecx
        mov cl, byte ptr [ebx]
        cmp ecx, 0
        je asm_153
        cmp edx, dword ptr [esi]
        ja asm_153
        mov eax, dword ptr [eax + ecx*4]
        mov ecx, dword ptr [esp + 13ch]
        xor edx, edx
        mov dx, word ptr [edi]
        mov edx, dword ptr [ecx + edx*4]
        mov eax, dword ptr [ecx + eax*4]
        add edx, eax
        mov ecx, dword ptr [esp + 140h]
        mov eax, dword ptr [ecx + edx*4]
        mov edx, dword ptr [esp + 144h]
        mov ecx, eax
        shl eax, 10h
        or eax, ecx
        mov dword ptr [edi], eax
        mov dword ptr [edi + 1000h], eax
    asm_153:
        add edi, 4
        add esi, 8
        inc ebx
        sub dword ptr [ebp - 4], 1
        jg asm_152
        mov eax, dword ptr [esp + 64h]
        mov ebx, dword ptr [ebp - 8]
        add eax, dword ptr [esp + 3ch]
        add eax, dword ptr [esp + 3ch]
        add ebx, dword ptr [ebp + 10h]
        mov dword ptr [esp + 64h], eax
        mov dword ptr [ebp - 8], ebx
        mov edi, dword ptr [esp + 38h]
        mov ebx, dword ptr [esp + 10h]
        add edi, dword ptr [esp + 4]
        add edi, dword ptr [esp + 4]
        dec ebx
        mov dword ptr [esp + 38h], edi
        mov dword ptr [esp + 10h], ebx
        sub dword ptr [ebp + 0ch], 1
        jg asm_151
        mov esp, dword ptr [esp + 158h]
    asm_154:
        pop edi
        pop esi
        pop ebx
        mov esp, ebp
        pop ebp
        ret
    }
}

/*
 * Halves signed width and height by arithmetic shifts and runs the authored ESP-repointed
 * assembly only for positive results. Tests the constant depth in the context against every
 * second depth cell using unsigned comparison, without writing depth.
 */
// FUNCTION: FF7 0x006bd46f
void __cdecl Gfx_RasterBlitIndexedRectDoubledDepthTestBlendRemap(int width, int height, int sourceStride, int rasterContext)
{
    int p;
    int q;
    int r;
    width >>= 1;
    height >>= 1;
    if (width > 0) {
        if (height > 0) {
            __asm {
                mov eax, dword ptr [ebp + 14h]
                mov dword ptr [eax + 158h], esp
                mov esp, eax
                mov eax, dword ptr [esp + 0ch]
                mov edi, dword ptr [esp + 8]
                shl eax, 0ch
                mov esi, dword ptr [esp + 40h]
                add edi, eax
                add esi, eax
                mov eax, dword ptr [esp + 1ch]
                lea esi, [esi + eax*4]
                lea edi, [edi + eax*2]
                mov dword ptr [esp + 64h], esi
                mov dword ptr [esp + 38h], edi
                mov eax, dword ptr [esp + 118h]
                mov ecx, dword ptr [esp + 70h]
                mov edx, dword ptr [esp + 80h]
                lea eax, [eax + ecx]
                mov cl, byte ptr [esp + 10ch]
                shl edx, cl
                add eax, edx
                mov dword ptr [ebp - 0ch], eax
                mov edx, dword ptr [esp + 144h]
            asm_155:
                mov eax, dword ptr [ebp + 8]
                mov ebx, dword ptr [ebp - 0ch]
                mov dword ptr [ebp - 4], eax
                mov esi, dword ptr [esp + 64h]
                add dword ptr [esp], eax
            asm_156:
                cmp edx, dword ptr [esi]
                ja asm_157
                mov eax, dword ptr [esp + 11ch]
                xor ecx, ecx
                mov cl, byte ptr [ebx]
                mov eax, dword ptr [eax + ecx*4]
                mov ecx, dword ptr [esp + 13ch]
                xor edx, edx
                mov dx, word ptr [edi]
                mov edx, dword ptr [ecx + edx*4]
                mov eax, dword ptr [ecx + eax*4]
                add edx, eax
                mov dword ptr [ebp - 8], eax
                mov ecx, dword ptr [esp + 140h]
                mov eax, dword ptr [ecx + edx*4]
                mov edx, dword ptr [esp + 144h]
                mov ecx, eax
                shl eax, 10h
                or eax, ecx
                mov dword ptr [edi], eax
                mov dword ptr [edi + 1000h], eax
            asm_157:
                add edi, 4
                add esi, 8
                inc ebx
                sub dword ptr [ebp - 4], 1
                jg asm_156
                mov eax, dword ptr [esp + 64h]
                mov ebx, dword ptr [ebp - 0ch]
                add eax, dword ptr [esp + 3ch]
                add eax, dword ptr [esp + 3ch]
                add ebx, dword ptr [ebp + 10h]
                mov dword ptr [esp + 64h], eax
                mov dword ptr [ebp - 0ch], ebx
                mov edi, dword ptr [esp + 38h]
                mov ebx, dword ptr [esp + 10h]
                add edi, dword ptr [esp + 4]
                add edi, dword ptr [esp + 4]
                dec ebx
                mov dword ptr [esp + 38h], edi
                mov dword ptr [esp + 10h], ebx
                sub dword ptr [ebp + 0ch], 1
                jg asm_155
                mov esp, dword ptr [esp + 158h]
            }
        }
    }
}

// Inline assembly that fills two dword tables from 64-bit fixed-point accumulators. The first loop
// writes the high word of the U accumulator (uStartLow/offsetTableX) to dstHigh for count
// iterations, advancing it by the 64-bit uStepLow/uStepHigh. The second loop shifts the high word
// of the V accumulator (vStartLow/offsetTableY) left by shiftX's byte and writes it to dstShifted for
// countB iterations, advancing it by vStepLow/vStepHigh.
// FUNCTION: FF7 0x006bd5a6
void __cdecl Gfx_RasterBuildBlitOffsetTables(int count, int countB, int dstHigh, int dstShifted, GfxRasterSetupBlock *tables)
{
    __asm {
        mov esi, dword ptr [ebp + 18h]
        mov edi, dword ptr [ebp + 10h]
        mov eax, dword ptr [esi + 6ch]
        mov ebx, dword ptr [esi + 70h]
    asm_158:
        mov dword ptr [edi], ebx
        add edi, 4
        add eax, dword ptr [esi + 74h]
        adc ebx, dword ptr [esi + 78h]
        sub dword ptr [ebp + 8], 1
        jg asm_158
        mov edi, dword ptr [ebp + 14h]
        mov eax, dword ptr [esi + 7ch]
        mov ebx, dword ptr [esi + 80h]
        xor ecx, ecx
    asm_159:
        mov edx, ebx
        mov cl, byte ptr [esi + 10ch]
        shl edx, cl
        mov dword ptr [edi], edx
        add edi, 4
        add eax, dword ptr [esi + 84h]
        adc ebx, dword ptr [esi + 88h]
        sub dword ptr [ebp + 0ch], 1
        jg asm_159
    }
}

// Inline-assembly span renderer that runs on a compiler-owned frame. It saves ESP in
// rasterContext and then uses rasterContext itself as its working context, deriving a 16-bit
// output address from the context's destination row, row stride and column offset. It restores ESP
// from the saved slot at the end.
// FUNCTION: FF7 0x006bd5fd
void __cdecl Gfx_RasterBlitIndexedRectOffsetTableColorKey(int width, int height, int columnOffsets, int rowOffsets, int rasterContext)
{
    int p;
    int q;
    __asm {
        mov eax, dword ptr [ebp + 18h]
        mov dword ptr [eax + 158h], esp
        mov esp, eax
        mov eax, dword ptr [esp + 0ch]
        mov edi, dword ptr [esp + 8]
        shl eax, 0ch
        add edi, eax
        mov eax, dword ptr [esp + 1ch]
        lea edi, [edi + eax*2]
        mov dword ptr [esp + 38h], edi
    asm_160:
        mov ebx, dword ptr [ebp + 10h]
        mov ecx, dword ptr [ebp + 14h]
        mov eax, dword ptr [ecx]
        add ecx, 4
        mov dword ptr [ebp + 14h], ecx
        add eax, dword ptr [esp + 118h]
        mov dword ptr [ebp - 8], eax
        mov eax, dword ptr [ebp + 8]
        mov dword ptr [ebp - 4], eax
        add dword ptr [esp], eax
    asm_161:
        mov eax, dword ptr [ebp - 8]
        xor ecx, ecx
        add eax, dword ptr [ebx]
        add ebx, 4
        mov cl, byte ptr [eax]
        mov eax, dword ptr [esp + 11ch]
        cmp ecx, 0
        je asm_162
        mov eax, dword ptr [eax + ecx*4]
        mov word ptr [edi], ax
    asm_162:
        add edi, 2
        sub dword ptr [ebp - 4], 1
        jg asm_161
        mov edi, dword ptr [esp + 38h]
        mov ebx, dword ptr [esp + 10h]
        add edi, dword ptr [esp + 4]
        dec ebx
        mov dword ptr [esp + 38h], edi
        mov dword ptr [esp + 10h], ebx
        sub dword ptr [ebp + 0ch], 1
        jg asm_160
        mov esp, dword ptr [esp + 158h]
    }
}

/*
 * Inline assembly span renderer with a compiler-owned frame. Saves ESP in rasterContext and
 * uses rasterContext as its working stack/context.
 */
// FUNCTION: FF7 0x006bd69a
void __cdecl Gfx_RasterBlitIndexedRectOffsetTable(int width, int height, int columnOffsets, int rowOffsets, int rasterContext)
{
    int p;
    int q;
    __asm {
        mov eax, dword ptr [ebp + 18h]
        mov dword ptr [eax + 158h], esp
        mov esp, eax
        mov eax, dword ptr [esp + 0ch]
        mov edi, dword ptr [esp + 8]
        shl eax, 0ch
        add edi, eax
        mov eax, dword ptr [esp + 1ch]
        lea edi, [edi + eax*2]
        mov dword ptr [esp + 38h], edi
    asm_163:
        mov ebx, dword ptr [ebp + 10h]
        mov ecx, dword ptr [ebp + 14h]
        mov eax, dword ptr [ecx]
        add ecx, 4
        mov dword ptr [ebp + 14h], ecx
        add eax, dword ptr [esp + 118h]
        mov dword ptr [ebp - 8], eax
        mov eax, dword ptr [ebp + 8]
        mov dword ptr [ebp - 4], eax
        add dword ptr [esp], eax
    asm_164:
        mov eax, dword ptr [ebp - 8]
        xor ecx, ecx
        add eax, dword ptr [ebx]
        add ebx, 4
        mov cl, byte ptr [eax]
        mov eax, dword ptr [esp + 11ch]
        mov eax, dword ptr [eax + ecx*4]
        mov word ptr [edi], ax
        add edi, 2
        sub dword ptr [ebp - 4], 1
        jg asm_164
        mov edi, dword ptr [esp + 38h]
        mov ebx, dword ptr [esp + 10h]
        add edi, dword ptr [esp + 4]
        dec ebx
        mov dword ptr [esp + 38h], edi
        mov dword ptr [esp + 10h], ebx
        sub dword ptr [ebp + 0ch], 1
        jg asm_163
        mov esp, dword ptr [esp + 158h]
    }
}

/*
 * Naked assembly indexed span renderer whose stack operands are not in its prototype. Each row
 * offset is combined with the context palette base and each pixel offset fetches a source byte.
 */
// FUNCTION: FF7 0x006bd732
__declspec(naked) void __cdecl Gfx_RasterBlitIndexedRectOffsetTableColorKeyDepthWrite(void)
{
    __asm {
        push ebp
        mov ebp, esp
        sub esp, 8
        push ebx
        push esi
        push edi
        mov eax, dword ptr [ebp + 18h]
        mov dword ptr [eax + 158h], esp
        mov esp, eax
        mov eax, dword ptr [esp + 0ch]
        mov edi, dword ptr [esp + 8]
        shl eax, 0ch
        mov esi, dword ptr [esp + 40h]
        add edi, eax
        add esi, eax
        mov eax, dword ptr [esp + 1ch]
        lea esi, [esi + eax*4]
        lea edi, [edi + eax*2]
        mov dword ptr [esp + 64h], esi
        mov dword ptr [esp + 38h], edi
        mov edx, dword ptr [esp + 144h]
    asm_165:
        mov ebx, dword ptr [ebp + 10h]
        mov ecx, dword ptr [ebp + 14h]
        mov eax, dword ptr [ecx]
        add ecx, 4
        mov dword ptr [ebp + 14h], ecx
        add eax, dword ptr [esp + 118h]
        mov dword ptr [ebp - 8], eax
        mov eax, dword ptr [ebp + 8]
        mov dword ptr [ebp - 4], eax
        mov esi, dword ptr [esp + 64h]
        add dword ptr [esp], eax
    asm_166:
        mov eax, dword ptr [ebp - 8]
        xor ecx, ecx
        add eax, dword ptr [ebx]
        add ebx, 4
        mov cl, byte ptr [eax]
        mov eax, dword ptr [esp + 11ch]
        cmp ecx, 0
        je asm_167
        cmp edx, dword ptr [esi]
        ja asm_167
        mov dword ptr [esi], edx
        mov eax, dword ptr [eax + ecx*4]
        mov word ptr [edi], ax
    asm_167:
        add edi, 2
        add esi, 4
        sub dword ptr [ebp - 4], 1
        jg asm_166
        mov eax, dword ptr [esp + 64h]
        add eax, dword ptr [esp + 3ch]
        mov dword ptr [esp + 64h], eax
        mov edi, dword ptr [esp + 38h]
        mov ebx, dword ptr [esp + 10h]
        add edi, dword ptr [esp + 4]
        dec ebx
        mov dword ptr [esp + 38h], edi
        mov dword ptr [esp + 10h], ebx
        sub dword ptr [ebp + 0ch], 1
        jg asm_165
        mov esp, dword ptr [esp + 158h]
        pop edi
        pop esi
        pop ebx
        mov esp, ebp
        pop ebp
        ret
    }
}

/*
 * Naked assembly indexed span renderer whose stack operands are not in its prototype. Each row
 * combines its supplied offset with the context palette base; each pixel adds an offset from
 * the supplied offset array to fetch a byte and palette word.
 */
// FUNCTION: FF7 0x006bd7fc
__declspec(naked) void __cdecl Gfx_RasterBlitIndexedRectOffsetTableDepthWrite(void)
{
    __asm {
        push ebp
        mov ebp, esp
        sub esp, 8
        push ebx
        push esi
        push edi
        mov eax, dword ptr [ebp + 18h]
        mov dword ptr [eax + 158h], esp
        mov esp, eax
        mov eax, dword ptr [esp + 0ch]
        mov edi, dword ptr [esp + 8]
        shl eax, 0ch
        mov esi, dword ptr [esp + 40h]
        add edi, eax
        add esi, eax
        mov eax, dword ptr [esp + 1ch]
        lea esi, [esi + eax*4]
        lea edi, [edi + eax*2]
        mov dword ptr [esp + 64h], esi
        mov dword ptr [esp + 38h], edi
        mov edx, dword ptr [esp + 144h]
    asm_168:
        mov ebx, dword ptr [ebp + 10h]
        mov ecx, dword ptr [ebp + 14h]
        mov eax, dword ptr [ecx]
        add ecx, 4
        mov dword ptr [ebp + 14h], ecx
        add eax, dword ptr [esp + 118h]
        mov dword ptr [ebp - 8], eax
        mov eax, dword ptr [ebp + 8]
        mov dword ptr [ebp - 4], eax
        mov esi, dword ptr [esp + 64h]
        add dword ptr [esp], eax
    asm_169:
        mov eax, dword ptr [ebp - 8]
        xor ecx, ecx
        add eax, dword ptr [ebx]
        add ebx, 4
        mov cl, byte ptr [eax]
        mov eax, dword ptr [esp + 11ch]
        cmp edx, dword ptr [esi]
        ja asm_170
        mov dword ptr [esi], edx
        mov eax, dword ptr [eax + ecx*4]
        mov word ptr [edi], ax
    asm_170:
        add edi, 2
        add esi, 4
        sub dword ptr [ebp - 4], 1
        jg asm_169
        mov eax, dword ptr [esp + 64h]
        add eax, dword ptr [esp + 3ch]
        mov dword ptr [esp + 64h], eax
        mov edi, dword ptr [esp + 38h]
        mov ebx, dword ptr [esp + 10h]
        add edi, dword ptr [esp + 4]
        dec ebx
        mov dword ptr [esp + 38h], edi
        mov dword ptr [esp + 10h], ebx
        sub dword ptr [ebp + 0ch], 1
        jg asm_168
        mov esp, dword ptr [esp + 158h]
        pop edi
        pop esi
        pop ebx
        mov esp, ebp
        pop ebp
        ret
    }
}

/*
 * Selects a software rectangle raster callback from flags, cached blend mode and
 * alpha metadata, returning whether a supported combination was selected. For each
 * 128-byte quad record, converts the first/fourth corners to a positive rectangle,
 * clips it and stores scan origin, depth bias and texture coordinates.
 */
// FUNCTION: FF7 0x006bd8c1
int __cdecl Gfx_RasterTryDrawTexturedRectQuads(int flags, int quadCount, int param, GfxRasterTexRect *quads, GfxEngineContext *engineContext)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[5284];
        int unclippedWidth;
        struct { unsigned char pad_to_4[0x4]; int unclippedHeight; };
        struct { unsigned char pad_to_8[0x8]; float depthBias; };
        struct { unsigned char pad_to_C[0xC]; GfxRasterTexRect *quad; };
        struct { unsigned char pad_to_10[0x10]; float vStep; };
        struct { unsigned char pad_to_14[0x14]; float uSpan; };
        struct { unsigned char pad_to_18[0x18]; float uStep; };
        struct { unsigned char pad_to_1C[0x1C]; float uStart; };
        struct { unsigned char pad_to_20[0x20]; float uEnd; };
        struct { unsigned char pad_to_24[0x24]; float vStart; };
        struct { unsigned char pad_to_28[0x28]; Int64Pair depthBiasHi; };
        struct { unsigned char pad_to_30[0x30]; int offsetY; };
        struct { unsigned char pad_to_34[0x34]; int offsetX; };
        struct { unsigned char pad_to_38[0x38]; float vSpan; };
        struct { unsigned char pad_to_3C[0x3C]; float rawWidth; };
        struct { unsigned char pad_to_40[0x40]; GfxTexturedRectCorner *fourthCorner; };
        struct { unsigned char pad_to_44[0x44]; float rawHeight; };
        struct { unsigned char pad_to_48[0x48]; float vEnd; };
        struct { unsigned char pad_to_4C[0x4C]; int quadIndex; };
        struct { unsigned char pad_to_50[0x50]; GfxRasterSetupBlock *blitTables; };
        struct { unsigned char pad_to_54[0x54]; GfxClipResult clipResult; };
        struct { unsigned char pad_to_68[0x68]; int rectTablePtr; };
        struct { unsigned char pad_to_6C[0x6C]; GfxRasterRectState *rectState; };
        struct { unsigned char pad_to_70[0x70]; int offsetTableA[640]; };
        struct { unsigned char pad_to_A70[0xA70]; int clipCode; };
        struct { unsigned char pad_to_A74[0xA74]; int callbackIndex; };
        struct { unsigned char pad_to_A78[0xA78]; int callbackFound; };
        struct { unsigned char pad_to_A7C[0xA7C]; int rectHeight; };
        struct { unsigned char pad_to_A80[0xA80]; SwTexturedRectState *texRectState; };
        struct { unsigned char pad_to_A84[0xA84]; int (__cdecl *blitCallback)(int, int, int, int); };
        struct { unsigned char pad_to_A88[0xA88]; int rectY; };
        struct { unsigned char pad_to_A8C[0xA8C]; int rectWidth; };
        struct { unsigned char pad_to_A90[0xA90]; int rectX; };
        struct { unsigned char pad_to_A94[0xA94]; int quadCursor; };
        struct { unsigned char pad_to_A98[0xA98]; int rectStateValue; };
        struct { unsigned char pad_to_A9C[0xA9C]; SwRasterRectContext *rectContext; };
        struct { unsigned char pad_to_AA0[0xAA0]; int (__cdecl *offsetBlitCallback)(int, int, int, int, int); };
        struct { unsigned char pad_to_AA4[0xAA4]; int offsetTableB[640]; };
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
#pragma pack(pop)

    frame.rectContext = (SwRasterRectContext *)((char *)engineContext->rasterBase + 0x38);
    frame.rectTablePtr = (int)frame.rectContext + 0x154;
    frame.blitTables = (GfxRasterSetupBlock *)(frame.rectContext->blitTables);
    frame.rectState = (GfxRasterRectState *)(frame.rectContext->rectState);
    frame.rectStateValue = frame.rectState->alignedVertices;
    frame.texRectState = (SwTexturedRectState *)(engineContext->presetPolygonSets);
    frame.quadCursor = (int)quads;
    frame.callbackFound = 0;
    frame.callbackIndex = frame.texRectState->colorKeyEnabled;
    if ((flags & 0x400) != 0) { /* empty first arm: lib/codegen.h */
    } else {
        frame.callbackIndex += 2;
    }
    if ((flags & 0x20) != 0) {
        switch (frame.texRectState->mode) {
        case BLEND_AVERAGE:
            frame.blitCallback = (int (__cdecl *)(int, int, int, int))g_GfxRasterBlitIndexedRectBlendCallbacks[frame.callbackIndex];
            frame.callbackFound = 1;
            break;
        case BLEND_ADDITIVE:
        case BLEND_SUBTRACTIVE:
        case BLEND_QUARTER_ADDITIVE:
            frame.blitCallback = (int (__cdecl *)(int, int, int, int))g_GfxRasterBlitIndexedRectBlendRemapCallbacks[frame.callbackIndex];
            frame.callbackFound = 1;
            break;
        case BLEND_NONE:
            frame.blitCallback = (int (__cdecl *)(int, int, int, int))g_GfxRasterBlitIndexedRectCallbacks[frame.callbackIndex];
            frame.callbackFound = 1;
        }
    } else if ((flags & 0x40) != 0) {
        switch (frame.texRectState->mode) {
        case BLEND_AVERAGE:
            break;
        case BLEND_NONE:
            frame.blitCallback = (int (__cdecl *)(int, int, int, int))g_GfxRasterBlitIndexedRectDoubledCallbacks[frame.callbackIndex];
            frame.callbackFound = 1;
        }
    } else if ((flags & 0x80) != 0) {
        switch (frame.texRectState->mode) {
        case BLEND_AVERAGE:
            break;
        case BLEND_NONE:
            frame.offsetBlitCallback = (int (__cdecl *)(int, int, int, int, int))g_GfxRasterBlitIndexedRectOffsetTableCallbacks[frame.callbackIndex];
            frame.callbackFound = 1;
        }
    }
    if (frame.callbackFound != 0) {
        for (frame.quadIndex = 0; frame.quadIndex < quadCount; frame.quadIndex++) {
            frame.quad = (GfxRasterTexRect *)frame.quadCursor;
            frame.fourthCorner = (GfxTexturedRectCorner *)(frame.quadCursor + 0x60);
            frame.quadCursor += 0x80;
            frame.rectX = Gfx_RasterRoundFloatToInt(frame.quad->x);
            frame.rectY = Gfx_RasterRoundFloatToInt(frame.quad->y);
            frame.rawWidth = frame.fourthCorner->posX - frame.quad->x;
            frame.rawHeight = frame.fourthCorner->posY - frame.quad->y;
            frame.rectWidth = Gfx_RasterRoundFloatToInt(frame.rawWidth);
            frame.rectHeight = Gfx_RasterRoundFloatToInt(frame.rawHeight);
            if (frame.rectWidth > 0 && frame.rectHeight > 0) {
                frame.clipCode = Gfx_RasterClassifyRectAgainstClipBounds(frame.rectX, frame.rectY, frame.rectWidth, frame.rectHeight, &frame.clipResult, engineContext);
                switch (frame.clipCode) {
                case GFX_RECT_CLIP_INSIDE:
                    if ((flags & 0x80) != 0) {
                        frame.blitTables->originX = frame.rectX;
                        frame.blitTables->originY = frame.rectY;
                        frame.depthBias = frame.quad->z * frame.rectState->scaleDepth;
                        crt_FloatToInt64(frame.depthBias, &frame.depthBiasHi.lo);
                        frame.blitTables->depthOffset = frame.depthBiasHi.hi + frame.rectState->outputOffset;
                        frame.uStart = frame.quad->texU * frame.rectState->scaleU;
                        frame.vStart = frame.quad->texV * frame.rectState->scaleV;
                        frame.uEnd = frame.fourthCorner->texU * frame.rectState->scaleU;
                        frame.vEnd = frame.fourthCorner->texV * frame.rectState->scaleV;
                        frame.uSpan = frame.uEnd - frame.uStart;
                        frame.vSpan = frame.vEnd - frame.vStart;
                        frame.uStep = frame.uSpan / (float)frame.rectWidth;
                        frame.vStep = frame.vSpan / (float)frame.rectHeight;
                        crt_FloatToInt64(frame.uStart, &frame.blitTables->uStartLow);
                        crt_FloatToInt64(frame.vStart, &frame.blitTables->vStartLow);
                        crt_FloatToInt64(frame.uStep, &frame.blitTables->uStepLow);
                        crt_FloatToInt64(frame.vStep, &frame.blitTables->vStepLow);
                        Gfx_RasterBuildBlitOffsetTables(frame.rectWidth, frame.rectHeight, (int)frame.offsetTableA, (int)frame.offsetTableB, frame.blitTables);
                        frame.offsetBlitCallback(frame.rectWidth, frame.rectHeight, (int)frame.offsetTableA, (int)frame.offsetTableB, (int)frame.blitTables);
                    } else {
                        frame.blitTables->originX = frame.rectX;
                        frame.blitTables->originY = frame.rectY;
                        frame.offsetX = Gfx_RasterRoundFloatToInt(frame.quad->texU * frame.rectState->scaleU);
                        frame.offsetY = Gfx_RasterRoundFloatToInt(frame.quad->texV * frame.rectState->scaleV);
                        frame.blitTables->offsetTableX = frame.offsetX;
                        frame.blitTables->offsetTableY = frame.offsetY;
                        frame.depthBias = frame.quad->z * frame.rectState->scaleDepth;
                        crt_FloatToInt64(frame.depthBias, &frame.depthBiasHi.lo);
                        frame.blitTables->depthOffset = frame.depthBiasHi.hi + frame.rectState->outputOffset;
                        frame.blitCallback(frame.rectWidth, frame.rectHeight, frame.rectState->drawCallback, (int)frame.blitTables);
                    }
                    break;
                case GFX_RECT_CLIP_HORIZONTAL:
                case GFX_RECT_CLIP_VERTICAL:
                case GFX_RECT_CLIP_BOTH:
                    frame.unclippedWidth = frame.rectWidth;
                    frame.unclippedHeight = frame.rectHeight;
                    frame.rectX += frame.clipResult.leftOver;
                    frame.rectWidth -= frame.clipResult.leftOver + frame.clipResult.overRight;
                    frame.rectY += frame.clipResult.topOver;
                    frame.rectHeight -= frame.clipResult.topOver + frame.clipResult.bottomOver;
                    if (frame.rectWidth > 0 && frame.rectHeight > 0) {
                        if ((flags & 0x80) != 0) {
                            frame.blitTables->originX = frame.rectX;
                            frame.blitTables->originY = frame.rectY;
                            frame.depthBias = frame.quad->z * frame.rectState->scaleDepth;
                            crt_FloatToInt64(frame.depthBias, &frame.depthBiasHi.lo);
                            frame.blitTables->depthOffset = frame.depthBiasHi.hi + frame.rectState->outputOffset;
                            frame.uStart = frame.quad->texU * frame.rectState->scaleU;
                            frame.vStart = frame.quad->texV * frame.rectState->scaleV;
                            frame.uEnd = frame.fourthCorner->texU * frame.rectState->scaleU;
                            frame.vEnd = frame.fourthCorner->texV * frame.rectState->scaleV;
                            frame.uSpan = frame.uEnd - frame.uStart;
                            frame.vSpan = frame.vEnd - frame.vStart;
                            frame.uStep = frame.uSpan / (float)frame.unclippedWidth;
                            frame.vStep = frame.vSpan / (float)frame.unclippedHeight;
                            frame.uStart = (float)frame.clipResult.leftOver * frame.uStep + frame.uStart;
                            frame.vStart = (float)frame.clipResult.topOver * frame.vStep + frame.vStart;
                            crt_FloatToInt64(frame.uStart, &frame.blitTables->uStartLow);
                            crt_FloatToInt64(frame.vStart, &frame.blitTables->vStartLow);
                            crt_FloatToInt64(frame.uStep, &frame.blitTables->uStepLow);
                            crt_FloatToInt64(frame.vStep, &frame.blitTables->vStepLow);
                            Gfx_RasterBuildBlitOffsetTables(frame.rectWidth, frame.rectHeight, (int)frame.offsetTableA, (int)frame.offsetTableB, frame.blitTables);
                            frame.offsetBlitCallback(frame.rectWidth, frame.rectHeight, (int)frame.offsetTableA, (int)frame.offsetTableB, (int)frame.blitTables);
                        } else {
                            frame.blitTables->originX = frame.rectX;
                            frame.blitTables->originY = frame.rectY;
                            frame.offsetX = Gfx_RasterRoundFloatToInt(frame.quad->texU * frame.rectState->scaleU);
                            frame.offsetY = Gfx_RasterRoundFloatToInt(frame.quad->texV * frame.rectState->scaleV);
                            if ((flags & 0x20) != 0) {
                                frame.blitTables->offsetTableX = frame.offsetX + frame.clipResult.leftOver;
                                frame.blitTables->offsetTableY = frame.offsetY + frame.clipResult.topOver;
                            } else {
                                frame.blitTables->offsetTableX = frame.offsetX + (frame.clipResult.leftOver >> 1);
                                frame.blitTables->offsetTableY = frame.offsetY + (frame.clipResult.topOver >> 1);
                            }
                            frame.depthBias = frame.quad->z * frame.rectState->scaleDepth;
                            crt_FloatToInt64(frame.depthBias, &frame.depthBiasHi.lo);
                            frame.blitTables->depthOffset = frame.depthBiasHi.hi + frame.rectState->outputOffset;
                            frame.blitCallback(frame.rectWidth, frame.rectHeight, frame.rectState->drawCallback, (int)frame.blitTables);
                        }
                    }
                }
            }
        }
    }
    return frame.callbackFound;
}
} /* extern "C" */
