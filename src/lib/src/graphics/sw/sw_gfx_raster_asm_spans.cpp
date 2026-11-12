// src/lib/src/graphics/sw/sw_gfx_raster_asm_spans.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: The software rasterizer's empty no-op handler and the original hand-written span rasterizers for
//   solid, Gouraud and textured triangles and spans, with depth, blend and colour-key variants.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/types/graphics.h"
#include "lib/api/graphics.h"
#include "lib/crt.h"
#include "lib/codegen.h"
extern "C" {

// clang-format off
// clang-format on


void __cdecl crt_FloatToInt64(float value, void *out);
void __cdecl Gfx_RasterTriangleFlatNoDepth(void *span);
void __cdecl Gfx_RasterTriangleFlatZ(void);
void __cdecl Gfx_RasterTriangleFlatZTestBlend(void);
void __cdecl Gfx_RasterTriangleFlatZTestBlendLookup(void);
void __cdecl Gfx_RasterTriangleRowShadedZ(void);
void __cdecl Gfx_RasterTriangleGouraudZ(void);
void __cdecl Gfx_RasterGouraudSpanBlend(void);
void __cdecl Gfx_RasterGouraudSpanBlendRemap(void);
void __cdecl Gfx_RasterPaletteTexturedTriangleColorKey(void);
void __cdecl Gfx_RasterIndexedTextureSpanColorKeyDepthWrite(void);
void __cdecl Gfx_RasterIndexedTextureSpanDepthWrite(void);
void __cdecl Gfx_RasterPaletteTexturedTriangleOpaque(void);
void __cdecl Gfx_RasterIndexedTextureSpanOffsetTableDepthWrite(void);
void __cdecl Gfx_RasterIndexedTextureSpanColorKeyBlendRemap(void);
void __cdecl Gfx_RasterIndexedTextureSpanBlendRemap(void);
void __cdecl Gfx_RasterDirectTextureSpanColorKeyDepthWrite(void);
void __cdecl Gfx_RasterDirectTextureSpanDepthWrite(void);
void __cdecl Gfx_RasterIndexedTextureSpanColorKeyBlend(void);
void __cdecl Gfx_RasterIndexedTextureSpanBlend(void);
void __cdecl Gfx_RasterDirectTextureSpanColorKeyBlend(void);
void __cdecl Gfx_RasterDirectTextureSpanBlend(void);
void __cdecl Gfx_RasterDirectTextureSpanColorKeyBlendRemap(void);
void __cdecl Gfx_RasterDirectTextureSpanBlendRemap(void);
void __cdecl Gfx_RasterSpanRecolorDestination(void);
void __cdecl Gfx_RasterDirectTextureSpanColorKeyBlendDestRemap(void);
void __cdecl Gfx_RasterNegateFixed64(unsigned int *value);


/*
 * Empty. A render-handler slot the shipped build fills with a function that
 * does nothing.
 */
// FUNCTION: FF7 0x006b9200
void __cdecl Gfx_NoOpHandler(void)
{
    /* The original function body is empty. */
}

/*
 * Original naked 16-bit solid rasterizer. Saves the real stack pointer at
 * span+0x158 and uses the span record as ESP.
 */
// FUNCTION: FF7 0x006b9205
__declspec(naked) void __cdecl Gfx_RasterTriangleFlatNoDepth(void *span)
{
    __asm {
        push ebp
        mov ebp, esp
        push ebx
        push esi
        push edi
        push ebp
        mov eax, dword ptr [ebp+8]
        mov dword ptr [eax+158h], esp
        mov esp, eax
        mov eax, dword ptr [esp+0ch]
        mov edi, dword ptr [esp+8]
        shl eax, 0ch
        add edi, eax
        mov eax, dword ptr [esp+1ch]
        mov ecx, dword ptr [esp+2ch]
        mov dword ptr [esp+38h], edi
    raster_row:
        sub ecx, eax
        jle raster_step
        add dword ptr [esp], ecx
        lea edi, [edi+eax*2]
        mov eax, dword ptr [esp+14h]
    raster_fill:
        mov word ptr [edi], ax
        add edi, 2
        dec ecx
        jg raster_fill
    raster_step:
        mov ebx, dword ptr [esp+18h]
        mov eax, dword ptr [esp+1ch]
        add ebx, dword ptr [esp+20h]
        adc eax, dword ptr [esp+24h]
        mov dword ptr [esp+18h], ebx
        mov dword ptr [esp+1ch], eax
        mov edx, dword ptr [esp+28h]
        mov ecx, dword ptr [esp+2ch]
        add edx, dword ptr [esp+30h]
        adc ecx, dword ptr [esp+34h]
        mov dword ptr [esp+28h], edx
        mov dword ptr [esp+2ch], ecx
        mov edi, dword ptr [esp+38h]
        mov ebx, dword ptr [esp+10h]
        add edi, dword ptr [esp+4]
        dec ebx
        mov dword ptr [esp+38h], edi
        mov dword ptr [esp+10h], ebx
        jg raster_row
        mov esp, dword ptr [esp+158h]
        pop ebp
        pop edi
        pop esi
        pop ebx
        pop ebp
        ret
    }
}

/*
 * Original naked assembly solid triangle rasterizer. Saves ESP in context
 * +0x158 and walks rows between split fixed-point left and right edge
 * accumulators.
 */
// FUNCTION: FF7 0x006b929b
__declspec(naked) void __cdecl Gfx_RasterTriangleFlatZ(void)
{
    __asm {
        push ebp
        mov ebp, esp
        push ebx
        push esi
        push edi
        push ebp
        mov eax, dword ptr [ebp + 8]
        mov dword ptr [eax + 158h], esp
        mov esp, eax
        mov eax, dword ptr [esp + 0ch]
        mov edi, dword ptr [esp + 8]
        shl eax, 0ch
        mov esi, dword ptr [esp + 40h]
        add edi, eax
        add esi, eax
        mov dword ptr [esp + 64h], esi
        mov eax, dword ptr [esp + 1ch]
        mov ecx, dword ptr [esp + 2ch]
        mov dword ptr [esp + 38h], edi
    asm_1:
        sub ecx, eax
        jle asm_4
        add dword ptr [esp], ecx
        lea edi, [edi + eax*2]
        mov ebp, ecx
        mov esi, dword ptr [esp + 64h]
        lea esi, [esi + eax*4]
        mov edx, dword ptr [esp + 58h]
        mov eax, dword ptr [esp + 14h]
        mov ebx, dword ptr [esp + 60h]
    asm_2:
        cmp edx, dword ptr [esi]
        ja asm_3
        mov dword ptr [esi], edx
        mov word ptr [edi], ax
    asm_3:
        add edi, 2
        add esi, 4
        add edx, ebx
        dec ebp
        jg asm_2
    asm_4:
        mov eax, dword ptr [esp + 64h]
        mov edx, dword ptr [esp + 48h]
        add eax, dword ptr [esp + 3ch]
        add edx, dword ptr [esp + 50h]
        mov dword ptr [esp + 58h], edx
        mov dword ptr [esp + 64h], eax
        mov dword ptr [esp + 48h], edx
        mov ebx, dword ptr [esp + 18h]
        mov eax, dword ptr [esp + 1ch]
        add ebx, dword ptr [esp + 20h]
        adc eax, dword ptr [esp + 24h]
        mov dword ptr [esp + 18h], ebx
        mov dword ptr [esp + 1ch], eax
        mov edx, dword ptr [esp + 28h]
        mov ecx, dword ptr [esp + 2ch]
        add edx, dword ptr [esp + 30h]
        adc ecx, dword ptr [esp + 34h]
        mov dword ptr [esp + 28h], edx
        mov dword ptr [esp + 2ch], ecx
        mov edi, dword ptr [esp + 38h]
        mov ebx, dword ptr [esp + 10h]
        add edi, dword ptr [esp + 4]
        dec ebx
        mov dword ptr [esp + 38h], edi
        mov dword ptr [esp + 10h], ebx
        jg asm_1
        mov esp, dword ptr [esp + 158h]
        pop ebp
        pop edi
        pop esi
        pop ebx
        pop ebp
        ret
    }
}

/*
 * Original naked assembly depth-tested solid-color compositor. Saves ESP in
 * context +0x158 and walks positive-width spans between split fixed-point edge
 * accumulators.
 */
// FUNCTION: FF7 0x006b9377
__declspec(naked) void __cdecl Gfx_RasterTriangleFlatZTestBlend(void)
{
    __asm {
        push ebp
        mov ebp, esp
        push ebx
        push esi
        push edi
        push ebp
        mov eax, dword ptr [ebp + 8]
        mov dword ptr [eax + 158h], esp
        mov esp, eax
        mov eax, dword ptr [esp + 0ch]
        mov edi, dword ptr [esp + 8]
        shl eax, 0ch
        mov esi, dword ptr [esp + 40h]
        add edi, eax
        add esi, eax
        mov dword ptr [esp + 64h], esi
        mov eax, dword ptr [esp + 1ch]
        mov ecx, dword ptr [esp + 2ch]
        mov dword ptr [esp + 38h], edi
    asm_5:
        sub ecx, eax
        jle asm_8
        add dword ptr [esp], ecx
        lea edi, [edi + eax*2]
        mov ebp, ecx
        mov ebx, dword ptr [esp + 64h]
        lea ebx, [ebx + eax*4]
        mov edx, dword ptr [esp + 58h]
        mov ecx, dword ptr [esp + 54h]
    asm_6:
        cmp edx, dword ptr [ebx]
        ja asm_7
        mov ecx, dword ptr [esp + 13ch]
        xor edx, edx
        mov dx, word ptr [edi]
        mov eax, dword ptr [esp + 14h]
        mov edx, dword ptr [ecx + edx*4]
        mov eax, dword ptr [ecx + eax*4]
        add eax, edx
        mov word ptr [edi], ax
    asm_7:
        add edi, 2
        add ebx, 4
        mov edx, dword ptr [esp + 58h]
        add edx, dword ptr [esp + 60h]
        mov dword ptr [esp + 58h], edx
        dec ebp
        jg asm_6
    asm_8:
        mov eax, dword ptr [esp + 64h]
        mov edx, dword ptr [esp + 48h]
        add eax, dword ptr [esp + 3ch]
        add edx, dword ptr [esp + 50h]
        mov dword ptr [esp + 64h], eax
        mov dword ptr [esp + 48h], edx
        mov dword ptr [esp + 58h], edx
        mov ebx, dword ptr [esp + 18h]
        mov eax, dword ptr [esp + 1ch]
        add ebx, dword ptr [esp + 20h]
        adc eax, dword ptr [esp + 24h]
        mov dword ptr [esp + 18h], ebx
        mov dword ptr [esp + 1ch], eax
        mov edx, dword ptr [esp + 28h]
        mov ecx, dword ptr [esp + 2ch]
        add edx, dword ptr [esp + 30h]
        adc ecx, dword ptr [esp + 34h]
        mov dword ptr [esp + 28h], edx
        mov dword ptr [esp + 2ch], ecx
        mov edi, dword ptr [esp + 38h]
        mov ebx, dword ptr [esp + 10h]
        add edi, dword ptr [esp + 4]
        dec ebx
        mov dword ptr [esp + 38h], edi
        mov dword ptr [esp + 10h], ebx
        jg asm_5
        mov esp, dword ptr [esp + 158h]
        pop ebp
        pop edi
        pop esi
        pop ebx
        pop ebp
        ret
    }
}

/*
 * Original naked assembly depth-tested solid-color compositor. Saves ESP in
 * context +0x158 and walks positive-width spans between split fixed-point edge
 * accumulators.
 */
// FUNCTION: FF7 0x006b946f
__declspec(naked) void __cdecl Gfx_RasterTriangleFlatZTestBlendLookup(void)
{
    __asm {
        push ebp
        mov ebp, esp
        push ebx
        push esi
        push edi
        push ebp
        mov eax, dword ptr [ebp + 8]
        mov dword ptr [eax + 158h], esp
        mov esp, eax
        mov eax, dword ptr [esp + 0ch]
        mov edi, dword ptr [esp + 8]
        shl eax, 0ch
        mov esi, dword ptr [esp + 40h]
        add edi, eax
        add esi, eax
        mov dword ptr [esp + 64h], esi
        mov eax, dword ptr [esp + 1ch]
        mov ecx, dword ptr [esp + 2ch]
        mov dword ptr [esp + 38h], edi
    asm_9:
        sub ecx, eax
        jle asm_12
        add dword ptr [esp], ecx
        lea edi, [edi + eax*2]
        mov ebp, ecx
        mov ebx, dword ptr [esp + 64h]
        lea ebx, [ebx + eax*4]
        mov edx, dword ptr [esp + 58h]
        mov ecx, dword ptr [esp + 54h]
    asm_10:
        cmp edx, dword ptr [ebx]
        ja asm_11
        mov ecx, dword ptr [esp + 13ch]
        xor edx, edx
        mov dx, word ptr [edi]
        mov eax, dword ptr [esp + 14h]
        mov edx, dword ptr [ecx + edx*4]
        mov eax, dword ptr [ecx + eax*4]
        add edx, eax
        mov ecx, dword ptr [esp + 140h]
        mov eax, dword ptr [ecx + edx*4]
        mov word ptr [edi], ax
    asm_11:
        add edi, 2
        add ebx, 4
        mov edx, dword ptr [esp + 58h]
        add edx, dword ptr [esp + 60h]
        mov dword ptr [esp + 58h], edx
        dec ebp
        jg asm_10
    asm_12:
        mov eax, dword ptr [esp + 64h]
        mov edx, dword ptr [esp + 48h]
        add eax, dword ptr [esp + 3ch]
        add edx, dword ptr [esp + 50h]
        mov dword ptr [esp + 64h], eax
        mov dword ptr [esp + 48h], edx
        mov dword ptr [esp + 58h], edx
        mov ebx, dword ptr [esp + 18h]
        mov eax, dword ptr [esp + 1ch]
        add ebx, dword ptr [esp + 20h]
        adc eax, dword ptr [esp + 24h]
        mov dword ptr [esp + 18h], ebx
        mov dword ptr [esp + 1ch], eax
        mov edx, dword ptr [esp + 28h]
        mov ecx, dword ptr [esp + 2ch]
        add edx, dword ptr [esp + 30h]
        adc ecx, dword ptr [esp + 34h]
        mov dword ptr [esp + 28h], edx
        mov dword ptr [esp + 2ch], ecx
        mov edi, dword ptr [esp + 38h]
        mov ebx, dword ptr [esp + 10h]
        add edi, dword ptr [esp + 4]
        dec ebx
        mov dword ptr [esp + 38h], edi
        mov dword ptr [esp + 10h], ebx
        jg asm_9
        mov esp, dword ptr [esp + 158h]
        pop ebp
        pop edi
        pop esi
        pop ebx
        pop ebp
        ret
    }
}

/*
 * Original naked assembly triangle rasterizer with one three-channel color per
 * row. Saves ESP in context +0x158, finds positive spans between split fixed-
 * point edges and ORs three channel-table lookups to select the row color.
 */
// FUNCTION: FF7 0x006b9571
__declspec(naked) void __cdecl Gfx_RasterTriangleRowShadedZ(void)
{
    __asm {
        push ebp
        mov ebp, esp
        push ebx
        push esi
        push edi
        push ebp
        mov eax, dword ptr [ebp + 8]
        mov dword ptr [eax + 158h], esp
        mov esp, eax
        mov eax, dword ptr [esp + 0ch]
        mov edi, dword ptr [esp + 8]
        shl eax, 0ch
        mov esi, dword ptr [esp + 40h]
        add edi, eax
        add esi, eax
        mov dword ptr [esp + 64h], esi
        mov eax, dword ptr [esp + 1ch]
        mov ecx, dword ptr [esp + 2ch]
        mov dword ptr [esp + 38h], edi
    asm_13:
        sub ecx, eax
        jle asm_16
        add dword ptr [esp], ecx
        lea edi, [edi + eax*2]
        mov ebp, ecx
        mov esi, dword ptr [esp + 64h]
        lea esi, [esi + eax*4]
        mov eax, dword ptr [esp + 0c0h]
        mov ebx, dword ptr [esp + 124h]
        mov ecx, dword ptr [esp + 0d0h]
        mov edx, dword ptr [esp + 128h]
        mov eax, dword ptr [ebx + eax*4]
        or eax, dword ptr [edx + ecx*4]
        mov ecx, dword ptr [esp + 0e0h]
        mov edx, dword ptr [esp + 12ch]
        or eax, dword ptr [edx + ecx*4]
        mov edx, dword ptr [esp + 58h]
    asm_14:
        cmp edx, dword ptr [esi]
        ja asm_15
        mov dword ptr [esi], edx
        mov word ptr [edi], ax
    asm_15:
        add edi, 2
        add esi, 4
        add edx, dword ptr [esp + 60h]
        dec ebp
        jg asm_14
    asm_16:
        mov eax, dword ptr [esp + 64h]
        mov edx, dword ptr [esp + 48h]
        add eax, dword ptr [esp + 3ch]
        add edx, dword ptr [esp + 50h]
        mov dword ptr [esp + 64h], eax
        mov dword ptr [esp + 48h], edx
        mov dword ptr [esp + 58h], edx
        mov ebx, dword ptr [esp + 0bch]
        mov edx, dword ptr [esp + 0c0h]
        add ebx, dword ptr [esp + 0c4h]
        adc edx, dword ptr [esp + 0c8h]
        mov dword ptr [esp + 0bch], ebx
        mov dword ptr [esp + 0c0h], edx
        mov ebx, dword ptr [esp + 0cch]
        mov edx, dword ptr [esp + 0d0h]
        add ebx, dword ptr [esp + 0d4h]
        adc edx, dword ptr [esp + 0d8h]
        mov dword ptr [esp + 0cch], ebx
        mov dword ptr [esp + 0d0h], edx
        mov ebx, dword ptr [esp + 0dch]
        mov edx, dword ptr [esp + 0e0h]
        add ebx, dword ptr [esp + 0e4h]
        adc edx, dword ptr [esp + 0e8h]
        mov dword ptr [esp + 0dch], ebx
        mov dword ptr [esp + 0e0h], edx
        mov ebx, dword ptr [esp + 18h]
        mov eax, dword ptr [esp + 1ch]
        add ebx, dword ptr [esp + 20h]
        adc eax, dword ptr [esp + 24h]
        mov dword ptr [esp + 18h], ebx
        mov dword ptr [esp + 1ch], eax
        mov edx, dword ptr [esp + 28h]
        mov ecx, dword ptr [esp + 2ch]
        add edx, dword ptr [esp + 30h]
        adc ecx, dword ptr [esp + 34h]
        mov dword ptr [esp + 28h], edx
        mov dword ptr [esp + 2ch], ecx
        mov edi, dword ptr [esp + 38h]
        mov ebx, dword ptr [esp + 10h]
        add edi, dword ptr [esp + 4]
        dec ebx
        mov dword ptr [esp + 38h], edi
        mov dword ptr [esp + 10h], ebx
        jg asm_13
        mov esp, dword ptr [esp + 158h]
        pop ebp
        pop edi
        pop esi
        pop ebx
        pop ebp
        ret
    }
}

// Original hand-written interpolated-color span rasterizer, using the argument frame as ESP. It
// tests the unsigned interpolated depth as no greater than the stored depth and writes the accepted
// depth. For each pixel it combines three frame-resident channel lookup tables at +0x168, +0x578
// and +0x988, indexed by the current split channel integer values, and stores the low color word.
// FUNCTION: FF7 0x006b96f8
__declspec(naked) void __cdecl Gfx_RasterTriangleGouraudZ(void)
{
    __asm {
        push ebp
        mov ebp, esp
        push ebx
        push esi
        push edi
        push ebp
        mov eax, dword ptr [ebp + 8]
        mov dword ptr [eax + 158h], esp
        mov esp, eax
        mov eax, dword ptr [esp + 0ch]
        mov edi, dword ptr [esp + 8]
        shl eax, 0ch
        mov esi, dword ptr [esp + 40h]
        add edi, eax
        add esi, eax
        mov dword ptr [esp + 64h], esi
        mov eax, dword ptr [esp + 1ch]
        mov ecx, dword ptr [esp + 2ch]
        mov dword ptr [esp + 38h], edi
    asm_17:
        sub ecx, eax
        jle asm_20
        add dword ptr [esp], ecx
        lea edi, [edi + eax*2]
        mov ebp, ecx
        mov esi, dword ptr [esp + 64h]
        lea esi, [esi + eax*4]
        mov edx, dword ptr [esp + 58h]
    asm_18:
        cmp edx, dword ptr [esi]
        ja asm_19
        mov dword ptr [esi], edx
        mov eax, dword ptr [esp + 90h]
        mov ebx, dword ptr [esp + 0a0h]
        mov ecx, dword ptr [esp + 0b0h]
        mov eax, dword ptr [esp + eax*4 + 168h]
        or eax, dword ptr [esp + ebx*4 + 578h]
        or eax, dword ptr [esp + ecx*4 + 988h]
        mov word ptr [edi], ax
    asm_19:
        add edi, 2
        add esi, 4
        mov ecx, dword ptr [esp + 8ch]
        mov edx, dword ptr [esp + 90h]
        add ecx, dword ptr [esp + 94h]
        adc edx, dword ptr [esp + 98h]
        mov dword ptr [esp + 8ch], ecx
        mov dword ptr [esp + 90h], edx
        mov ecx, dword ptr [esp + 9ch]
        mov edx, dword ptr [esp + 0a0h]
        add ecx, dword ptr [esp + 0a4h]
        adc edx, dword ptr [esp + 0a8h]
        mov dword ptr [esp + 9ch], ecx
        mov dword ptr [esp + 0a0h], edx
        mov ecx, dword ptr [esp + 0ach]
        mov edx, dword ptr [esp + 0b0h]
        add ecx, dword ptr [esp + 0b4h]
        adc edx, dword ptr [esp + 0b8h]
        mov dword ptr [esp + 0ach], ecx
        mov dword ptr [esp + 0b0h], edx
        mov edx, dword ptr [esp + 58h]
        add edx, dword ptr [esp + 60h]
        mov dword ptr [esp + 58h], edx
        dec ebp
        jg asm_18
    asm_20:
        mov eax, dword ptr [esp + 64h]
        mov edx, dword ptr [esp + 48h]
        add eax, dword ptr [esp + 3ch]
        add edx, dword ptr [esp + 50h]
        mov dword ptr [esp + 64h], eax
        mov dword ptr [esp + 48h], edx
        mov dword ptr [esp + 58h], edx
        mov ebx, dword ptr [esp + 0bch]
        mov edx, dword ptr [esp + 0c0h]
        add ebx, dword ptr [esp + 0c4h]
        adc edx, dword ptr [esp + 0c8h]
        mov dword ptr [esp + 0bch], ebx
        mov dword ptr [esp + 0c0h], edx
        mov dword ptr [esp + 8ch], ebx
        mov dword ptr [esp + 90h], edx
        mov ebx, dword ptr [esp + 0cch]
        mov edx, dword ptr [esp + 0d0h]
        add ebx, dword ptr [esp + 0d4h]
        adc edx, dword ptr [esp + 0d8h]
        mov dword ptr [esp + 0cch], ebx
        mov dword ptr [esp + 0d0h], edx
        mov dword ptr [esp + 9ch], ebx
        mov dword ptr [esp + 0a0h], edx
        mov ebx, dword ptr [esp + 0dch]
        mov edx, dword ptr [esp + 0e0h]
        add ebx, dword ptr [esp + 0e4h]
        adc edx, dword ptr [esp + 0e8h]
        mov dword ptr [esp + 0dch], ebx
        mov dword ptr [esp + 0e0h], edx
        mov dword ptr [esp + 0ach], ebx
        mov dword ptr [esp + 0b0h], edx
        mov ebx, dword ptr [esp + 18h]
        mov eax, dword ptr [esp + 1ch]
        add ebx, dword ptr [esp + 20h]
        adc eax, dword ptr [esp + 24h]
        mov dword ptr [esp + 18h], ebx
        mov dword ptr [esp + 1ch], eax
        mov edx, dword ptr [esp + 28h]
        mov ecx, dword ptr [esp + 2ch]
        add edx, dword ptr [esp + 30h]
        adc ecx, dword ptr [esp + 34h]
        mov dword ptr [esp + 28h], edx
        mov dword ptr [esp + 2ch], ecx
        mov edi, dword ptr [esp + 38h]
        mov ebx, dword ptr [esp + 10h]
        add edi, dword ptr [esp + 4]
        dec ebx
        mov dword ptr [esp + 38h], edi
        mov dword ptr [esp + 10h], ebx
        jg asm_17
        mov esp, dword ptr [esp + 158h]
        pop ebp
        pop edi
        pop esi
        pop ebx
        pop ebp
        ret
    }
}

// Original hand-written interpolated-color span rasterizer, using the argument frame as ESP. It
// tests the unsigned interpolated depth as no greater than the stored depth, and it does not write
// depth. Where the test passes, it combines the three frame-resident channel tables at +0x168,
// +0x578 and +0x988 into a color index, adds the +0x13C table entry for that color to the +0x13C
// entry for the existing destination word, and stores the low word of the sum.
// FUNCTION: FF7 0x006b992e
__declspec(naked) void __cdecl Gfx_RasterGouraudSpanBlend(void)
{
    __asm {
        push ebp
        mov ebp, esp
        push ebx
        push esi
        push edi
        push ebp
        mov eax, dword ptr [ebp + 8]
        mov dword ptr [eax + 158h], esp
        mov esp, eax
        mov eax, dword ptr [esp + 0ch]
        mov edi, dword ptr [esp + 8]
        shl eax, 0ch
        mov esi, dword ptr [esp + 40h]
        add edi, eax
        add esi, eax
        mov dword ptr [esp + 64h], esi
        mov eax, dword ptr [esp + 1ch]
        mov ecx, dword ptr [esp + 2ch]
        mov dword ptr [esp + 38h], edi
    asm_21:
        sub ecx, eax
        jle asm_24
        add dword ptr [esp], ecx
        lea edi, [edi + eax*2]
        mov ebp, ecx
        mov esi, dword ptr [esp + 64h]
        lea esi, [esi + eax*4]
        mov edx, dword ptr [esp + 58h]
    asm_22:
        cmp edx, dword ptr [esi]
        ja asm_23
        mov eax, dword ptr [esp + 90h]
        mov ebx, dword ptr [esp + 0a0h]
        mov ecx, dword ptr [esp + 0b0h]
        mov eax, dword ptr [esp + eax*4 + 168h]
        or eax, dword ptr [esp + ebx*4 + 578h]
        or eax, dword ptr [esp + ecx*4 + 988h]
        mov ecx, dword ptr [esp + 13ch]
        xor edx, edx
        mov dx, word ptr [edi]
        mov edx, dword ptr [ecx + edx*4]
        mov eax, dword ptr [ecx + eax*4]
        add eax, edx
        mov word ptr [edi], ax
    asm_23:
        add edi, 2
        add esi, 4
        mov ecx, dword ptr [esp + 8ch]
        mov edx, dword ptr [esp + 90h]
        add ecx, dword ptr [esp + 94h]
        adc edx, dword ptr [esp + 98h]
        mov dword ptr [esp + 8ch], ecx
        mov dword ptr [esp + 90h], edx
        mov ecx, dword ptr [esp + 9ch]
        mov edx, dword ptr [esp + 0a0h]
        add ecx, dword ptr [esp + 0a4h]
        adc edx, dword ptr [esp + 0a8h]
        mov dword ptr [esp + 9ch], ecx
        mov dword ptr [esp + 0a0h], edx
        mov ecx, dword ptr [esp + 0ach]
        mov edx, dword ptr [esp + 0b0h]
        add ecx, dword ptr [esp + 0b4h]
        adc edx, dword ptr [esp + 0b8h]
        mov dword ptr [esp + 0ach], ecx
        mov dword ptr [esp + 0b0h], edx
        mov edx, dword ptr [esp + 58h]
        add edx, dword ptr [esp + 60h]
        mov dword ptr [esp + 58h], edx
        dec ebp
        jg asm_22
    asm_24:
        mov eax, dword ptr [esp + 64h]
        mov edx, dword ptr [esp + 48h]
        add eax, dword ptr [esp + 3ch]
        add edx, dword ptr [esp + 50h]
        mov dword ptr [esp + 64h], eax
        mov dword ptr [esp + 48h], edx
        mov dword ptr [esp + 58h], edx
        mov ebx, dword ptr [esp + 0bch]
        mov edx, dword ptr [esp + 0c0h]
        add ebx, dword ptr [esp + 0c4h]
        adc edx, dword ptr [esp + 0c8h]
        mov dword ptr [esp + 0bch], ebx
        mov dword ptr [esp + 0c0h], edx
        mov dword ptr [esp + 8ch], ebx
        mov dword ptr [esp + 90h], edx
        mov ebx, dword ptr [esp + 0cch]
        mov edx, dword ptr [esp + 0d0h]
        add ebx, dword ptr [esp + 0d4h]
        adc edx, dword ptr [esp + 0d8h]
        mov dword ptr [esp + 0cch], ebx
        mov dword ptr [esp + 0d0h], edx
        mov dword ptr [esp + 9ch], ebx
        mov dword ptr [esp + 0a0h], edx
        mov ebx, dword ptr [esp + 0dch]
        mov edx, dword ptr [esp + 0e0h]
        add ebx, dword ptr [esp + 0e4h]
        adc edx, dword ptr [esp + 0e8h]
        mov dword ptr [esp + 0dch], ebx
        mov dword ptr [esp + 0e0h], edx
        mov dword ptr [esp + 0ach], ebx
        mov dword ptr [esp + 0b0h], edx
        mov ebx, dword ptr [esp + 18h]
        mov eax, dword ptr [esp + 1ch]
        add ebx, dword ptr [esp + 20h]
        adc eax, dword ptr [esp + 24h]
        mov dword ptr [esp + 18h], ebx
        mov dword ptr [esp + 1ch], eax
        mov edx, dword ptr [esp + 28h]
        mov ecx, dword ptr [esp + 2ch]
        add edx, dword ptr [esp + 30h]
        adc ecx, dword ptr [esp + 34h]
        mov dword ptr [esp + 28h], edx
        mov dword ptr [esp + 2ch], ecx
        mov edi, dword ptr [esp + 38h]
        mov ebx, dword ptr [esp + 10h]
        add edi, dword ptr [esp + 4]
        dec ebx
        mov dword ptr [esp + 38h], edi
        mov dword ptr [esp + 10h], ebx
        jg asm_21
        mov esp, dword ptr [esp + 158h]
        pop ebp
        pop edi
        pop esi
        pop ebx
        pop ebp
        ret
    }
}

// Original hand-written interpolated-color span rasterizer, using the argument frame as ESP. It
// tests the unsigned interpolated depth as no greater than the stored depth, and it does not write
// depth. Where the test passes, it combines the three channel tables at +0x168, +0x578 and +0x988
// into a color index, adds the +0x13C entries for that color and for the existing destination word,
// translates the sum through the +0x140 table, and stores its low word.
// FUNCTION: FF7 0x006b9b76
__declspec(naked) void __cdecl Gfx_RasterGouraudSpanBlendRemap(void)
{
    __asm {
        push ebp
        mov ebp, esp
        push ebx
        push esi
        push edi
        push ebp
        mov eax, dword ptr [ebp + 8]
        mov dword ptr [eax + 158h], esp
        mov esp, eax
        mov eax, dword ptr [esp + 0ch]
        mov edi, dword ptr [esp + 8]
        shl eax, 0ch
        mov esi, dword ptr [esp + 40h]
        add edi, eax
        add esi, eax
        mov dword ptr [esp + 64h], esi
        mov eax, dword ptr [esp + 1ch]
        mov ecx, dword ptr [esp + 2ch]
        mov dword ptr [esp + 38h], edi
    asm_25:
        sub ecx, eax
        jle asm_28
        add dword ptr [esp], ecx
        lea edi, [edi + eax*2]
        mov ebp, ecx
        mov esi, dword ptr [esp + 64h]
        lea esi, [esi + eax*4]
        mov edx, dword ptr [esp + 58h]
    asm_26:
        cmp edx, dword ptr [esi]
        ja asm_27
        mov eax, dword ptr [esp + 90h]
        mov ebx, dword ptr [esp + 0a0h]
        mov ecx, dword ptr [esp + 0b0h]
        mov eax, dword ptr [esp + eax*4 + 168h]
        or eax, dword ptr [esp + ebx*4 + 578h]
        or eax, dword ptr [esp + ecx*4 + 988h]
        mov ecx, dword ptr [esp + 13ch]
        xor edx, edx
        mov dx, word ptr [edi]
        mov edx, dword ptr [ecx + edx*4]
        mov eax, dword ptr [ecx + eax*4]
        add edx, eax
        mov ecx, dword ptr [esp + 140h]
        mov eax, dword ptr [ecx + edx*4]
        mov word ptr [edi], ax
    asm_27:
        add edi, 2
        add esi, 4
        mov ecx, dword ptr [esp + 8ch]
        mov edx, dword ptr [esp + 90h]
        add ecx, dword ptr [esp + 94h]
        adc edx, dword ptr [esp + 98h]
        mov dword ptr [esp + 8ch], ecx
        mov dword ptr [esp + 90h], edx
        mov ecx, dword ptr [esp + 9ch]
        mov edx, dword ptr [esp + 0a0h]
        add ecx, dword ptr [esp + 0a4h]
        adc edx, dword ptr [esp + 0a8h]
        mov dword ptr [esp + 9ch], ecx
        mov dword ptr [esp + 0a0h], edx
        mov ecx, dword ptr [esp + 0ach]
        mov edx, dword ptr [esp + 0b0h]
        add ecx, dword ptr [esp + 0b4h]
        adc edx, dword ptr [esp + 0b8h]
        mov dword ptr [esp + 0ach], ecx
        mov dword ptr [esp + 0b0h], edx
        mov edx, dword ptr [esp + 58h]
        add edx, dword ptr [esp + 60h]
        mov dword ptr [esp + 58h], edx
        dec ebp
        jg asm_26
    asm_28:
        mov eax, dword ptr [esp + 64h]
        mov edx, dword ptr [esp + 48h]
        add eax, dword ptr [esp + 3ch]
        add edx, dword ptr [esp + 50h]
        mov dword ptr [esp + 64h], eax
        mov dword ptr [esp + 48h], edx
        mov dword ptr [esp + 58h], edx
        mov ebx, dword ptr [esp + 0bch]
        mov edx, dword ptr [esp + 0c0h]
        add ebx, dword ptr [esp + 0c4h]
        adc edx, dword ptr [esp + 0c8h]
        mov dword ptr [esp + 0bch], ebx
        mov dword ptr [esp + 0c0h], edx
        mov dword ptr [esp + 8ch], ebx
        mov dword ptr [esp + 90h], edx
        mov ebx, dword ptr [esp + 0cch]
        mov edx, dword ptr [esp + 0d0h]
        add ebx, dword ptr [esp + 0d4h]
        adc edx, dword ptr [esp + 0d8h]
        mov dword ptr [esp + 0cch], ebx
        mov dword ptr [esp + 0d0h], edx
        mov dword ptr [esp + 9ch], ebx
        mov dword ptr [esp + 0a0h], edx
        mov ebx, dword ptr [esp + 0dch]
        mov edx, dword ptr [esp + 0e0h]
        add ebx, dword ptr [esp + 0e4h]
        adc edx, dword ptr [esp + 0e8h]
        mov dword ptr [esp + 0dch], ebx
        mov dword ptr [esp + 0e0h], edx
        mov dword ptr [esp + 0ach], ebx
        mov dword ptr [esp + 0b0h], edx
        mov ebx, dword ptr [esp + 18h]
        mov eax, dword ptr [esp + 1ch]
        add ebx, dword ptr [esp + 20h]
        adc eax, dword ptr [esp + 24h]
        mov dword ptr [esp + 18h], ebx
        mov dword ptr [esp + 1ch], eax
        mov edx, dword ptr [esp + 28h]
        mov ecx, dword ptr [esp + 2ch]
        add edx, dword ptr [esp + 30h]
        adc ecx, dword ptr [esp + 34h]
        mov dword ptr [esp + 28h], edx
        mov dword ptr [esp + 2ch], ecx
        mov edi, dword ptr [esp + 38h]
        mov ebx, dword ptr [esp + 10h]
        add edi, dword ptr [esp + 4]
        dec ebx
        mov dword ptr [esp + 38h], edi
        mov dword ptr [esp + 10h], ebx
        jg asm_25
        mov esp, dword ptr [esp + 158h]
        pop ebp
        pop edi
        pop esi
        pop ebx
        pop ebp
        ret
    }
}

/*
 * Original naked assembly palette-textured triangle rasterizer. Saves ESP in
 * context +0x158 and draws positive spans between split fixed-point edge
 * accumulators.
 */
// FUNCTION: FF7 0x006b9dc8
__declspec(naked) void __cdecl Gfx_RasterPaletteTexturedTriangleColorKey(void)
{
    __asm {
        push ebp
        mov ebp, esp
        push ebx
        push esi
        push edi
        push ebp
        mov eax, dword ptr [ebp + 8]
        mov dword ptr [eax + 158h], esp
        mov esp, eax
        mov eax, dword ptr [esp + 0ch]
        mov edi, dword ptr [esp + 8]
        shl eax, 0ch
        add edi, eax
        mov eax, dword ptr [esp + 1ch]
        mov ecx, dword ptr [esp + 2ch]
        mov dword ptr [esp + 38h], edi
    asm_29:
        sub ecx, eax
        jle asm_32
        add dword ptr [esp], ecx
        lea edi, [edi + eax*2]
        mov ebp, ecx
    asm_30:
        mov eax, dword ptr [esp + 118h]
        mov ecx, dword ptr [esp + 70h]
        mov edx, dword ptr [esp + 80h]
        and ecx, dword ptr [esp + 110h]
        and edx, dword ptr [esp + 114h]
        lea eax, [eax + ecx]
        mov cl, byte ptr [esp + 10ch]
        shl edx, cl
        xor ecx, ecx
        mov cl, byte ptr [eax + edx]
        cmp ecx, 0
        je asm_31
        mov eax, dword ptr [esp + 11ch]
        mov eax, dword ptr [eax + ecx*4]
        mov word ptr [edi], ax
    asm_31:
        add edi, 2
        mov ecx, dword ptr [esp + 6ch]
        mov edx, dword ptr [esp + 70h]
        add ecx, dword ptr [esp + 74h]
        adc edx, dword ptr [esp + 78h]
        mov dword ptr [esp + 6ch], ecx
        mov dword ptr [esp + 70h], edx
        mov ecx, dword ptr [esp + 7ch]
        mov edx, dword ptr [esp + 80h]
        add ecx, dword ptr [esp + 84h]
        adc edx, dword ptr [esp + 88h]
        mov dword ptr [esp + 7ch], ecx
        mov dword ptr [esp + 80h], edx
        dec ebp
        jg asm_30
    asm_32:
        mov ebx, dword ptr [esp + 0ech]
        mov edx, dword ptr [esp + 0f0h]
        add ebx, dword ptr [esp + 0f4h]
        adc edx, dword ptr [esp + 0f8h]
        mov dword ptr [esp + 0ech], ebx
        mov dword ptr [esp + 0f0h], edx
        mov dword ptr [esp + 6ch], ebx
        mov dword ptr [esp + 70h], edx
        mov ebx, dword ptr [esp + 0fch]
        mov edx, dword ptr [esp + 100h]
        add ebx, dword ptr [esp + 104h]
        adc edx, dword ptr [esp + 108h]
        mov dword ptr [esp + 0fch], ebx
        mov dword ptr [esp + 100h], edx
        mov dword ptr [esp + 7ch], ebx
        mov dword ptr [esp + 80h], edx
        mov ebx, dword ptr [esp + 18h]
        mov eax, dword ptr [esp + 1ch]
        add ebx, dword ptr [esp + 20h]
        adc eax, dword ptr [esp + 24h]
        mov dword ptr [esp + 18h], ebx
        mov dword ptr [esp + 1ch], eax
        mov edx, dword ptr [esp + 28h]
        mov ecx, dword ptr [esp + 2ch]
        add edx, dword ptr [esp + 30h]
        adc ecx, dword ptr [esp + 34h]
        mov dword ptr [esp + 28h], edx
        mov dword ptr [esp + 2ch], ecx
        mov edi, dword ptr [esp + 38h]
        mov ebx, dword ptr [esp + 10h]
        add edi, dword ptr [esp + 4]
        dec ebx
        mov dword ptr [esp + 38h], edi
        mov dword ptr [esp + 10h], ebx
        jg asm_29
        mov esp, dword ptr [esp + 158h]
        pop ebp
        pop edi
        pop esi
        pop ebx
        pop ebp
        ret
    }
}

/*
 * Original hand-written indexed-texture span rasterizer using the argument
 * frame as ESP. Loads the texture byte at masked split U/V coordinates and
 * skips index zero before testing depth.
 */
// FUNCTION: FF7 0x006b9f4b
__declspec(naked) void __cdecl Gfx_RasterIndexedTextureSpanColorKeyDepthWrite(void)
{
    __asm {
        push ebp
        mov ebp, esp
        push ebx
        push esi
        push edi
        push ebp
        mov eax, dword ptr [ebp + 8]
        mov dword ptr [eax + 158h], esp
        mov esp, eax
        mov eax, dword ptr [esp + 0ch]
        mov edi, dword ptr [esp + 8]
        shl eax, 0ch
        mov esi, dword ptr [esp + 40h]
        add edi, eax
        add esi, eax
        mov dword ptr [esp + 64h], esi
        mov eax, dword ptr [esp + 1ch]
        mov ecx, dword ptr [esp + 2ch]
        mov dword ptr [esp + 38h], edi
    asm_33:
        sub ecx, eax
        jle asm_36
        add dword ptr [esp], ecx
        lea edi, [edi + eax*2]
        mov ebp, ecx
        mov ebx, dword ptr [esp + 64h]
        lea ebx, [ebx + eax*4]
    asm_34:
        mov eax, dword ptr [esp + 118h]
        mov ecx, dword ptr [esp + 70h]
        mov edx, dword ptr [esp + 80h]
        and ecx, dword ptr [esp + 110h]
        and edx, dword ptr [esp + 114h]
        lea eax, [eax + ecx]
        mov cl, byte ptr [esp + 10ch]
        shl edx, cl
        xor ecx, ecx
        mov cl, byte ptr [eax + edx]
        cmp ecx, 0
        je asm_35
        mov edx, dword ptr [esp + 58h]
        cmp edx, dword ptr [ebx]
        ja asm_35
        mov dword ptr [ebx], edx
        mov eax, dword ptr [esp + 11ch]
        mov eax, dword ptr [eax + ecx*4]
        mov word ptr [edi], ax
    asm_35:
        add edi, 2
        add ebx, 4
        mov ecx, dword ptr [esp + 6ch]
        mov edx, dword ptr [esp + 70h]
        add ecx, dword ptr [esp + 74h]
        adc edx, dword ptr [esp + 78h]
        mov dword ptr [esp + 6ch], ecx
        mov dword ptr [esp + 70h], edx
        mov ecx, dword ptr [esp + 7ch]
        mov edx, dword ptr [esp + 80h]
        add ecx, dword ptr [esp + 84h]
        adc edx, dword ptr [esp + 88h]
        mov dword ptr [esp + 7ch], ecx
        mov dword ptr [esp + 80h], edx
        mov edx, dword ptr [esp + 58h]
        add edx, dword ptr [esp + 60h]
        mov dword ptr [esp + 58h], edx
        dec ebp
        jg asm_34
    asm_36:
        mov eax, dword ptr [esp + 64h]
        mov edx, dword ptr [esp + 48h]
        add eax, dword ptr [esp + 3ch]
        add edx, dword ptr [esp + 50h]
        mov dword ptr [esp + 64h], eax
        mov dword ptr [esp + 48h], edx
        mov dword ptr [esp + 58h], edx
        mov ebx, dword ptr [esp + 0ech]
        mov edx, dword ptr [esp + 0f0h]
        add ebx, dword ptr [esp + 0f4h]
        adc edx, dword ptr [esp + 0f8h]
        mov dword ptr [esp + 0ech], ebx
        mov dword ptr [esp + 0f0h], edx
        mov dword ptr [esp + 6ch], ebx
        mov dword ptr [esp + 70h], edx
        mov ebx, dword ptr [esp + 0fch]
        mov edx, dword ptr [esp + 100h]
        add ebx, dword ptr [esp + 104h]
        adc edx, dword ptr [esp + 108h]
        mov dword ptr [esp + 0fch], ebx
        mov dword ptr [esp + 100h], edx
        mov dword ptr [esp + 7ch], ebx
        mov dword ptr [esp + 80h], edx
        mov ebx, dword ptr [esp + 18h]
        mov eax, dword ptr [esp + 1ch]
        add ebx, dword ptr [esp + 20h]
        adc eax, dword ptr [esp + 24h]
        mov dword ptr [esp + 18h], ebx
        mov dword ptr [esp + 1ch], eax
        mov edx, dword ptr [esp + 28h]
        mov ecx, dword ptr [esp + 2ch]
        add edx, dword ptr [esp + 30h]
        adc ecx, dword ptr [esp + 34h]
        mov dword ptr [esp + 28h], edx
        mov dword ptr [esp + 2ch], ecx
        mov edi, dword ptr [esp + 38h]
        mov ebx, dword ptr [esp + 10h]
        add edi, dword ptr [esp + 4]
        dec ebx
        mov dword ptr [esp + 38h], edi
        mov dword ptr [esp + 10h], ebx
        jg asm_33
        mov esp, dword ptr [esp + 158h]
        pop ebp
        pop edi
        pop esi
        pop ebx
        pop ebp
        ret
    }
}

/*
 * Original hand-written indexed-texture span rasterizer using the argument
 * frame as ESP. For each positive span width, tests interpolated depth as
 * unsigned no-greater than stored depth, writes accepted depth, loads the
 * texture byte at masked split U/V coordinates and translates it through the
 * +0x11C palette into the low destination word, including index zero.
 */
// FUNCTION: FF7 0x006ba114
__declspec(naked) void __cdecl Gfx_RasterIndexedTextureSpanDepthWrite(void)
{
    __asm {
        push ebp
        mov ebp, esp
        push ebx
        push esi
        push edi
        push ebp
        mov eax, dword ptr [ebp + 8]
        mov dword ptr [eax + 158h], esp
        mov esp, eax
        mov eax, dword ptr [esp + 0ch]
        mov edi, dword ptr [esp + 8]
        shl eax, 0ch
        mov esi, dword ptr [esp + 40h]
        add edi, eax
        add esi, eax
        mov dword ptr [esp + 64h], esi
        mov eax, dword ptr [esp + 1ch]
        mov ecx, dword ptr [esp + 2ch]
        mov dword ptr [esp + 38h], edi
    asm_37:
        sub ecx, eax
        jle asm_40
        add dword ptr [esp], ecx
        lea edi, [edi + eax*2]
        mov ebp, ecx
        mov esi, dword ptr [esp + 64h]
        lea esi, [esi + eax*4]
        mov edx, dword ptr [esp + 58h]
    asm_38:
        cmp edx, dword ptr [esi]
        ja asm_39
        mov dword ptr [esi], edx
        mov eax, dword ptr [esp + 118h]
        mov ecx, dword ptr [esp + 70h]
        mov edx, dword ptr [esp + 80h]
        and ecx, dword ptr [esp + 110h]
        and edx, dword ptr [esp + 114h]
        lea eax, [eax + ecx]
        mov cl, byte ptr [esp + 10ch]
        shl edx, cl
        xor ecx, ecx
        mov cl, byte ptr [eax + edx]
        mov eax, dword ptr [esp + 11ch]
        mov eax, dword ptr [eax + ecx*4]
        mov word ptr [edi], ax
    asm_39:
        add edi, 2
        add esi, 4
        mov ecx, dword ptr [esp + 6ch]
        mov edx, dword ptr [esp + 70h]
        add ecx, dword ptr [esp + 74h]
        adc edx, dword ptr [esp + 78h]
        mov dword ptr [esp + 6ch], ecx
        mov dword ptr [esp + 70h], edx
        mov ecx, dword ptr [esp + 7ch]
        mov edx, dword ptr [esp + 80h]
        add ecx, dword ptr [esp + 84h]
        adc edx, dword ptr [esp + 88h]
        mov dword ptr [esp + 7ch], ecx
        mov dword ptr [esp + 80h], edx
        mov edx, dword ptr [esp + 58h]
        add edx, dword ptr [esp + 60h]
        mov dword ptr [esp + 58h], edx
        dec ebp
        jg asm_38
    asm_40:
        mov eax, dword ptr [esp + 64h]
        mov edx, dword ptr [esp + 48h]
        add eax, dword ptr [esp + 3ch]
        add edx, dword ptr [esp + 50h]
        mov dword ptr [esp + 64h], eax
        mov dword ptr [esp + 48h], edx
        mov dword ptr [esp + 58h], edx
        mov ebx, dword ptr [esp + 0ech]
        mov edx, dword ptr [esp + 0f0h]
        add ebx, dword ptr [esp + 0f4h]
        adc edx, dword ptr [esp + 0f8h]
        mov dword ptr [esp + 0ech], ebx
        mov dword ptr [esp + 0f0h], edx
        mov dword ptr [esp + 6ch], ebx
        mov dword ptr [esp + 70h], edx
        mov ebx, dword ptr [esp + 0fch]
        mov edx, dword ptr [esp + 100h]
        add ebx, dword ptr [esp + 104h]
        adc edx, dword ptr [esp + 108h]
        mov dword ptr [esp + 0fch], ebx
        mov dword ptr [esp + 100h], edx
        mov dword ptr [esp + 7ch], ebx
        mov dword ptr [esp + 80h], edx
        mov ebx, dword ptr [esp + 18h]
        mov eax, dword ptr [esp + 1ch]
        add ebx, dword ptr [esp + 20h]
        adc eax, dword ptr [esp + 24h]
        mov dword ptr [esp + 18h], ebx
        mov dword ptr [esp + 1ch], eax
        mov edx, dword ptr [esp + 28h]
        mov ecx, dword ptr [esp + 2ch]
        add edx, dword ptr [esp + 30h]
        adc ecx, dword ptr [esp + 34h]
        mov dword ptr [esp + 28h], edx
        mov dword ptr [esp + 2ch], ecx
        mov edi, dword ptr [esp + 38h]
        mov ebx, dword ptr [esp + 10h]
        add edi, dword ptr [esp + 4]
        dec ebx
        mov dword ptr [esp + 38h], edi
        mov dword ptr [esp + 10h], ebx
        jg asm_37
        mov esp, dword ptr [esp + 158h]
        pop ebp
        pop edi
        pop esi
        pop ebx
        pop ebp
        ret
    }
}

/*
 * Original naked assembly palette-textured triangle rasterizer. Saves ESP in
 * context +0x158 and draws positive spans between split fixed-point edge
 * accumulators.
 */
// FUNCTION: FF7 0x006ba2d8
__declspec(naked) void __cdecl Gfx_RasterPaletteTexturedTriangleOpaque(void)
{
    __asm {
        push ebp
        mov ebp, esp
        push ebx
        push esi
        push edi
        push ebp
        mov eax, dword ptr [ebp + 8]
        mov dword ptr [eax + 158h], esp
        mov esp, eax
        mov eax, dword ptr [esp + 0ch]
        mov edi, dword ptr [esp + 8]
        shl eax, 0ch
        add edi, eax
        mov eax, dword ptr [esp + 1ch]
        mov ecx, dword ptr [esp + 2ch]
        mov dword ptr [esp + 38h], edi
    asm_41:
        sub ecx, eax
        jle asm_43
        add dword ptr [esp], ecx
        lea edi, [edi + eax*2]
        mov ebp, ecx
    asm_42:
        mov eax, dword ptr [esp + 118h]
        mov ecx, dword ptr [esp + 70h]
        mov edx, dword ptr [esp + 80h]
        and ecx, dword ptr [esp + 110h]
        and edx, dword ptr [esp + 114h]
        lea eax, [eax + ecx]
        mov cl, byte ptr [esp + 10ch]
        shl edx, cl
        xor ecx, ecx
        mov cl, byte ptr [eax + edx]
        mov eax, dword ptr [esp + 11ch]
        mov eax, dword ptr [eax + ecx*4]
        mov word ptr [edi], ax
        add edi, 2
        mov ecx, dword ptr [esp + 6ch]
        mov edx, dword ptr [esp + 70h]
        add ecx, dword ptr [esp + 74h]
        adc edx, dword ptr [esp + 78h]
        mov dword ptr [esp + 6ch], ecx
        mov dword ptr [esp + 70h], edx
        mov ecx, dword ptr [esp + 7ch]
        mov edx, dword ptr [esp + 80h]
        add ecx, dword ptr [esp + 84h]
        adc edx, dword ptr [esp + 88h]
        mov dword ptr [esp + 7ch], ecx
        mov dword ptr [esp + 80h], edx
        dec ebp
        jg asm_42
    asm_43:
        mov ebx, dword ptr [esp + 0ech]
        mov edx, dword ptr [esp + 0f0h]
        add ebx, dword ptr [esp + 0f4h]
        adc edx, dword ptr [esp + 0f8h]
        mov dword ptr [esp + 0ech], ebx
        mov dword ptr [esp + 0f0h], edx
        mov dword ptr [esp + 6ch], ebx
        mov dword ptr [esp + 70h], edx
        mov ebx, dword ptr [esp + 0fch]
        mov edx, dword ptr [esp + 100h]
        add ebx, dword ptr [esp + 104h]
        adc edx, dword ptr [esp + 108h]
        mov dword ptr [esp + 0fch], ebx
        mov dword ptr [esp + 100h], edx
        mov dword ptr [esp + 7ch], ebx
        mov dword ptr [esp + 80h], edx
        mov ebx, dword ptr [esp + 18h]
        mov eax, dword ptr [esp + 1ch]
        add ebx, dword ptr [esp + 20h]
        adc eax, dword ptr [esp + 24h]
        mov dword ptr [esp + 18h], ebx
        mov dword ptr [esp + 1ch], eax
        mov edx, dword ptr [esp + 28h]
        mov ecx, dword ptr [esp + 2ch]
        add edx, dword ptr [esp + 30h]
        adc ecx, dword ptr [esp + 34h]
        mov dword ptr [esp + 28h], edx
        mov dword ptr [esp + 2ch], ecx
        mov edi, dword ptr [esp + 38h]
        mov ebx, dword ptr [esp + 10h]
        add edi, dword ptr [esp + 4]
        dec ebx
        mov dword ptr [esp + 38h], edi
        mov dword ptr [esp + 10h], ebx
        jg asm_41
        mov esp, dword ptr [esp + 158h]
        pop ebp
        pop edi
        pop esi
        pop ebx
        pop ebp
        ret
    }
}

// Original hand-written indexed-texture span rasterizer, using the argument frame as ESP. Once per
// span run it forms a masked texture row base from +0xF0 and +0x100 (masked by +0x110 and +0x114,
// shifted by +0x10C, plus +0x118). Per pixel it consumes one 32-bit address offset from the list at
// +0x134, including for pixels whose depth test is rejected. Accepted pixels write their depth and
// store the word from the +0x11C table indexed by the byte at that address.
// FUNCTION: FF7 0x006ba452
__declspec(naked) void __cdecl Gfx_RasterIndexedTextureSpanOffsetTableDepthWrite(void)
{
    __asm {
        push ebp
        mov ebp, esp
        push ebx
        push esi
        push edi
        push ebp
        mov eax, dword ptr [ebp + 8]
        mov dword ptr [eax + 158h], esp
        mov esp, eax
        mov eax, dword ptr [esp + 0ch]
        mov edi, dword ptr [esp + 8]
        shl eax, 0ch
        mov esi, dword ptr [esp + 40h]
        add edi, eax
        add esi, eax
        mov dword ptr [esp + 64h], esi
        mov eax, dword ptr [esp + 1ch]
        mov ecx, dword ptr [esp + 2ch]
        mov dword ptr [esp + 38h], edi
    asm_44:
        sub ecx, eax
        jle asm_47
        add dword ptr [esp], ecx
        lea edi, [edi + eax*2]
        mov ebp, ecx
        mov ebx, dword ptr [esp + 64h]
        lea ebx, [ebx + eax*4]
        mov ecx, dword ptr [esp + 0f0h]
        mov edx, dword ptr [esp + 100h]
        mov eax, dword ptr [esp + 118h]
        and ecx, dword ptr [esp + 110h]
        and edx, dword ptr [esp + 114h]
        lea eax, [eax + ecx]
        mov cl, byte ptr [esp + 10ch]
        shl edx, cl
        mov ecx, dword ptr [esp + 134h]
        lea eax, [eax + edx]
        mov dword ptr [esp + 138h], ecx
        mov dword ptr [esp + 130h], eax
        mov edx, dword ptr [esp + 58h]
    asm_45:
        mov eax, dword ptr [esp + 130h]
        mov ecx, dword ptr [esp + 138h]
        add eax, dword ptr [ecx]
        add ecx, 4
        mov dword ptr [esp + 138h], ecx
        cmp edx, dword ptr [ebx]
        ja asm_46
        mov dword ptr [ebx], edx
        xor ecx, ecx
        mov cl, byte ptr [eax]
        mov eax, dword ptr [esp + 11ch]
        mov eax, dword ptr [eax + ecx*4]
        mov word ptr [edi], ax
    asm_46:
        add edi, 2
        add ebx, 4
        mov edx, dword ptr [esp + 58h]
        add edx, dword ptr [esp + 60h]
        mov dword ptr [esp + 58h], edx
        dec ebp
        jg asm_45
    asm_47:
        mov eax, dword ptr [esp + 64h]
        mov edx, dword ptr [esp + 48h]
        add eax, dword ptr [esp + 3ch]
        add edx, dword ptr [esp + 50h]
        mov dword ptr [esp + 64h], eax
        mov dword ptr [esp + 48h], edx
        mov dword ptr [esp + 58h], edx
        mov ebx, dword ptr [esp + 0ech]
        mov edx, dword ptr [esp + 0f0h]
        add ebx, dword ptr [esp + 0f4h]
        adc edx, dword ptr [esp + 0f8h]
        mov dword ptr [esp + 0ech], ebx
        mov dword ptr [esp + 0f0h], edx
        mov dword ptr [esp + 6ch], ebx
        mov dword ptr [esp + 70h], edx
        mov ebx, dword ptr [esp + 0fch]
        mov edx, dword ptr [esp + 100h]
        add ebx, dword ptr [esp + 104h]
        adc edx, dword ptr [esp + 108h]
        mov dword ptr [esp + 0fch], ebx
        mov dword ptr [esp + 100h], edx
        mov dword ptr [esp + 7ch], ebx
        mov dword ptr [esp + 80h], edx
        mov ebx, dword ptr [esp + 18h]
        mov eax, dword ptr [esp + 1ch]
        add ebx, dword ptr [esp + 20h]
        adc eax, dword ptr [esp + 24h]
        mov dword ptr [esp + 18h], ebx
        mov dword ptr [esp + 1ch], eax
        mov edx, dword ptr [esp + 28h]
        mov ecx, dword ptr [esp + 2ch]
        add edx, dword ptr [esp + 30h]
        adc ecx, dword ptr [esp + 34h]
        mov dword ptr [esp + 28h], edx
        mov dword ptr [esp + 2ch], ecx
        mov edi, dword ptr [esp + 38h]
        mov ebx, dword ptr [esp + 10h]
        add edi, dword ptr [esp + 4]
        dec ebx
        mov dword ptr [esp + 38h], edi
        mov dword ptr [esp + 10h], ebx
        jg asm_44
        mov esp, dword ptr [esp + 158h]
        pop ebp
        pop edi
        pop esi
        pop ebx
        pop ebp
        ret
    }
}

/*
 * Original hand-written indexed-texture span rasterizer using the argument
 * frame as ESP. Skips texture index zero, then tests unsigned interpolated
 * depth as no-greater than stored depth.
 */
// FUNCTION: FF7 0x006ba60a
__declspec(naked) void __cdecl Gfx_RasterIndexedTextureSpanColorKeyBlendRemap(void)
{
    __asm {
        push ebp
        mov ebp, esp
        push ebx
        push esi
        push edi
        push ebp
        mov eax, dword ptr [ebp + 8]
        mov dword ptr [eax + 158h], esp
        mov esp, eax
        mov eax, dword ptr [esp + 0ch]
        mov edi, dword ptr [esp + 8]
        shl eax, 0ch
        mov esi, dword ptr [esp + 40h]
        add edi, eax
        add esi, eax
        mov dword ptr [esp + 64h], esi
        mov eax, dword ptr [esp + 1ch]
        mov ecx, dword ptr [esp + 2ch]
        mov dword ptr [esp + 38h], edi
    asm_48:
        sub ecx, eax
        jle asm_51
        add dword ptr [esp], ecx
        lea edi, [edi + eax*2]
        mov ebp, ecx
        mov ebx, dword ptr [esp + 64h]
        lea ebx, [ebx + eax*4]
    asm_49:
        mov eax, dword ptr [esp + 118h]
        mov ecx, dword ptr [esp + 70h]
        mov edx, dword ptr [esp + 80h]
        and ecx, dword ptr [esp + 110h]
        and edx, dword ptr [esp + 114h]
        lea eax, [eax + ecx]
        mov cl, byte ptr [esp + 10ch]
        shl edx, cl
        xor ecx, ecx
        mov cl, byte ptr [eax + edx]
        cmp ecx, 0
        je asm_50
        mov edx, dword ptr [esp + 58h]
        cmp edx, dword ptr [ebx]
        ja asm_50
        mov eax, dword ptr [esp + 11ch]
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
    asm_50:
        add edi, 2
        add ebx, 4
        mov ecx, dword ptr [esp + 6ch]
        mov edx, dword ptr [esp + 70h]
        add ecx, dword ptr [esp + 74h]
        adc edx, dword ptr [esp + 78h]
        mov dword ptr [esp + 6ch], ecx
        mov dword ptr [esp + 70h], edx
        mov ecx, dword ptr [esp + 7ch]
        mov edx, dword ptr [esp + 80h]
        add ecx, dword ptr [esp + 84h]
        adc edx, dword ptr [esp + 88h]
        mov dword ptr [esp + 7ch], ecx
        mov dword ptr [esp + 80h], edx
        mov edx, dword ptr [esp + 58h]
        add edx, dword ptr [esp + 60h]
        mov dword ptr [esp + 58h], edx
        dec ebp
        jg asm_49
    asm_51:
        mov eax, dword ptr [esp + 64h]
        mov edx, dword ptr [esp + 48h]
        add eax, dword ptr [esp + 3ch]
        add edx, dword ptr [esp + 50h]
        mov dword ptr [esp + 64h], eax
        mov dword ptr [esp + 48h], edx
        mov dword ptr [esp + 58h], edx
        mov ebx, dword ptr [esp + 0ech]
        mov edx, dword ptr [esp + 0f0h]
        add ebx, dword ptr [esp + 0f4h]
        adc edx, dword ptr [esp + 0f8h]
        mov dword ptr [esp + 0ech], ebx
        mov dword ptr [esp + 0f0h], edx
        mov dword ptr [esp + 6ch], ebx
        mov dword ptr [esp + 70h], edx
        mov ebx, dword ptr [esp + 0fch]
        mov edx, dword ptr [esp + 100h]
        add ebx, dword ptr [esp + 104h]
        adc edx, dword ptr [esp + 108h]
        mov dword ptr [esp + 0fch], ebx
        mov dword ptr [esp + 100h], edx
        mov dword ptr [esp + 7ch], ebx
        mov dword ptr [esp + 80h], edx
        mov ebx, dword ptr [esp + 18h]
        mov eax, dword ptr [esp + 1ch]
        add ebx, dword ptr [esp + 20h]
        adc eax, dword ptr [esp + 24h]
        mov dword ptr [esp + 18h], ebx
        mov dword ptr [esp + 1ch], eax
        mov edx, dword ptr [esp + 28h]
        mov ecx, dword ptr [esp + 2ch]
        add edx, dword ptr [esp + 30h]
        adc ecx, dword ptr [esp + 34h]
        mov dword ptr [esp + 28h], edx
        mov dword ptr [esp + 2ch], ecx
        mov edi, dword ptr [esp + 38h]
        mov ebx, dword ptr [esp + 10h]
        add edi, dword ptr [esp + 4]
        dec ebx
        mov dword ptr [esp + 38h], edi
        mov dword ptr [esp + 10h], ebx
        jg asm_48
        mov esp, dword ptr [esp + 158h]
        pop ebp
        pop edi
        pop esi
        pop ebx
        pop ebp
        ret
    }
}

/*
 * Original hand-written indexed-texture span rasterizer using the argument
 * frame as ESP. Loads texture bytes at masked split U/V coordinates, including
 * zero, and tests unsigned interpolated depth as no-greater than stored depth.
 */
// FUNCTION: FF7 0x006ba7ef
__declspec(naked) void __cdecl Gfx_RasterIndexedTextureSpanBlendRemap(void)
{
    __asm {
        push ebp
        mov ebp, esp
        push ebx
        push esi
        push edi
        push ebp
        mov eax, dword ptr [ebp + 8]
        mov dword ptr [eax + 158h], esp
        mov esp, eax
        mov eax, dword ptr [esp + 0ch]
        mov edi, dword ptr [esp + 8]
        shl eax, 0ch
        mov esi, dword ptr [esp + 40h]
        add edi, eax
        add esi, eax
        mov dword ptr [esp + 64h], esi
        mov eax, dword ptr [esp + 1ch]
        mov ecx, dword ptr [esp + 2ch]
        mov dword ptr [esp + 38h], edi
    asm_52:
        sub ecx, eax
        jle asm_55
        add dword ptr [esp], ecx
        lea edi, [edi + eax*2]
        mov ebp, ecx
        mov ebx, dword ptr [esp + 64h]
        lea ebx, [ebx + eax*4]
    asm_53:
        mov eax, dword ptr [esp + 118h]
        mov ecx, dword ptr [esp + 70h]
        mov edx, dword ptr [esp + 80h]
        and ecx, dword ptr [esp + 110h]
        and edx, dword ptr [esp + 114h]
        lea eax, [eax + ecx]
        mov cl, byte ptr [esp + 10ch]
        shl edx, cl
        xor ecx, ecx
        mov cl, byte ptr [eax + edx]
        mov edx, dword ptr [esp + 58h]
        cmp edx, dword ptr [ebx]
        ja asm_54
        mov eax, dword ptr [esp + 11ch]
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
    asm_54:
        add edi, 2
        add ebx, 4
        mov ecx, dword ptr [esp + 6ch]
        mov edx, dword ptr [esp + 70h]
        add ecx, dword ptr [esp + 74h]
        adc edx, dword ptr [esp + 78h]
        mov dword ptr [esp + 6ch], ecx
        mov dword ptr [esp + 70h], edx
        mov ecx, dword ptr [esp + 7ch]
        mov edx, dword ptr [esp + 80h]
        add ecx, dword ptr [esp + 84h]
        adc edx, dword ptr [esp + 88h]
        mov dword ptr [esp + 7ch], ecx
        mov dword ptr [esp + 80h], edx
        mov edx, dword ptr [esp + 58h]
        add edx, dword ptr [esp + 60h]
        mov dword ptr [esp + 58h], edx
        dec ebp
        jg asm_53
    asm_55:
        mov eax, dword ptr [esp + 64h]
        mov edx, dword ptr [esp + 48h]
        add eax, dword ptr [esp + 3ch]
        add edx, dword ptr [esp + 50h]
        mov dword ptr [esp + 64h], eax
        mov dword ptr [esp + 48h], edx
        mov dword ptr [esp + 58h], edx
        mov ebx, dword ptr [esp + 0ech]
        mov edx, dword ptr [esp + 0f0h]
        add ebx, dword ptr [esp + 0f4h]
        adc edx, dword ptr [esp + 0f8h]
        mov dword ptr [esp + 0ech], ebx
        mov dword ptr [esp + 0f0h], edx
        mov dword ptr [esp + 6ch], ebx
        mov dword ptr [esp + 70h], edx
        mov ebx, dword ptr [esp + 0fch]
        mov edx, dword ptr [esp + 100h]
        add ebx, dword ptr [esp + 104h]
        adc edx, dword ptr [esp + 108h]
        mov dword ptr [esp + 0fch], ebx
        mov dword ptr [esp + 100h], edx
        mov dword ptr [esp + 7ch], ebx
        mov dword ptr [esp + 80h], edx
        mov ebx, dword ptr [esp + 18h]
        mov eax, dword ptr [esp + 1ch]
        add ebx, dword ptr [esp + 20h]
        adc eax, dword ptr [esp + 24h]
        mov dword ptr [esp + 18h], ebx
        mov dword ptr [esp + 1ch], eax
        mov edx, dword ptr [esp + 28h]
        mov ecx, dword ptr [esp + 2ch]
        add edx, dword ptr [esp + 30h]
        adc ecx, dword ptr [esp + 34h]
        mov dword ptr [esp + 28h], edx
        mov dword ptr [esp + 2ch], ecx
        mov edi, dword ptr [esp + 38h]
        mov ebx, dword ptr [esp + 10h]
        add edi, dword ptr [esp + 4]
        dec ebx
        mov dword ptr [esp + 38h], edi
        mov dword ptr [esp + 10h], ebx
        jg asm_52
        mov esp, dword ptr [esp + 158h]
        pop ebp
        pop edi
        pop esi
        pop ebx
        pop ebp
        ret
    }
}

/*
 * Original hand-written direct 16-bit textured span rasterizer using the
 * argument frame as ESP. Loads a zero-extended texture word at masked split
 * U/V coordinates and skips it when it equals the full 32-bit key at +0x14C.
 */
// FUNCTION: FF7 0x006ba9cf
__declspec(naked) void __cdecl Gfx_RasterDirectTextureSpanColorKeyDepthWrite(void)
{
    __asm {
        push ebp
        mov ebp, esp
        push ebx
        push esi
        push edi
        push ebp
        mov eax, dword ptr [ebp + 8]
        mov dword ptr [eax + 158h], esp
        mov esp, eax
        mov eax, dword ptr [esp + 0ch]
        mov edi, dword ptr [esp + 8]
        shl eax, 0ch
        mov esi, dword ptr [esp + 40h]
        add edi, eax
        add esi, eax
        mov dword ptr [esp + 64h], esi
        mov eax, dword ptr [esp + 1ch]
        mov ecx, dword ptr [esp + 2ch]
        mov dword ptr [esp + 38h], edi
    asm_56:
        sub ecx, eax
        jle asm_59
        add dword ptr [esp], ecx
        lea edi, [edi + eax*2]
        mov ebp, ecx
        mov ebx, dword ptr [esp + 64h]
        lea ebx, [ebx + eax*4]
    asm_57:
        mov eax, dword ptr [esp + 118h]
        mov ecx, dword ptr [esp + 70h]
        mov edx, dword ptr [esp + 80h]
        and ecx, dword ptr [esp + 110h]
        and edx, dword ptr [esp + 114h]
        lea eax, [eax + ecx*2]
        mov cl, byte ptr [esp + 10ch]
        shl edx, cl
        xor ecx, ecx
        mov cx, word ptr [eax + edx*2]
        cmp ecx, dword ptr [esp + 14ch]
        je asm_58
        mov edx, dword ptr [esp + 58h]
        cmp edx, dword ptr [ebx]
        ja asm_58
        mov dword ptr [ebx], edx
        mov word ptr [edi], cx
    asm_58:
        add edi, 2
        add ebx, 4
        mov ecx, dword ptr [esp + 6ch]
        mov edx, dword ptr [esp + 70h]
        add ecx, dword ptr [esp + 74h]
        adc edx, dword ptr [esp + 78h]
        mov dword ptr [esp + 6ch], ecx
        mov dword ptr [esp + 70h], edx
        mov ecx, dword ptr [esp + 7ch]
        mov edx, dword ptr [esp + 80h]
        add ecx, dword ptr [esp + 84h]
        adc edx, dword ptr [esp + 88h]
        mov dword ptr [esp + 7ch], ecx
        mov dword ptr [esp + 80h], edx
        mov edx, dword ptr [esp + 58h]
        add edx, dword ptr [esp + 60h]
        mov dword ptr [esp + 58h], edx
        dec ebp
        jg asm_57
    asm_59:
        mov eax, dword ptr [esp + 64h]
        mov edx, dword ptr [esp + 48h]
        add eax, dword ptr [esp + 3ch]
        add edx, dword ptr [esp + 50h]
        mov dword ptr [esp + 64h], eax
        mov dword ptr [esp + 48h], edx
        mov dword ptr [esp + 58h], edx
        mov ebx, dword ptr [esp + 0ech]
        mov edx, dword ptr [esp + 0f0h]
        add ebx, dword ptr [esp + 0f4h]
        adc edx, dword ptr [esp + 0f8h]
        mov dword ptr [esp + 0ech], ebx
        mov dword ptr [esp + 0f0h], edx
        mov dword ptr [esp + 6ch], ebx
        mov dword ptr [esp + 70h], edx
        mov ebx, dword ptr [esp + 0fch]
        mov edx, dword ptr [esp + 100h]
        add ebx, dword ptr [esp + 104h]
        adc edx, dword ptr [esp + 108h]
        mov dword ptr [esp + 0fch], ebx
        mov dword ptr [esp + 100h], edx
        mov dword ptr [esp + 7ch], ebx
        mov dword ptr [esp + 80h], edx
        mov ebx, dword ptr [esp + 18h]
        mov eax, dword ptr [esp + 1ch]
        add ebx, dword ptr [esp + 20h]
        adc eax, dword ptr [esp + 24h]
        mov dword ptr [esp + 18h], ebx
        mov dword ptr [esp + 1ch], eax
        mov edx, dword ptr [esp + 28h]
        mov ecx, dword ptr [esp + 2ch]
        add edx, dword ptr [esp + 30h]
        adc ecx, dword ptr [esp + 34h]
        mov dword ptr [esp + 28h], edx
        mov dword ptr [esp + 2ch], ecx
        mov edi, dword ptr [esp + 38h]
        mov ebx, dword ptr [esp + 10h]
        add edi, dword ptr [esp + 4]
        dec ebx
        mov dword ptr [esp + 38h], edi
        mov dword ptr [esp + 10h], ebx
        jg asm_56
        mov esp, dword ptr [esp + 158h]
        pop ebp
        pop edi
        pop esi
        pop ebx
        pop ebp
        ret
    }
}

/*
 * Original hand-written direct 16-bit textured span rasterizer using the
 * argument frame as ESP. For each positive span width, tests interpolated
 * depth as unsigned no-greater than stored depth, writes accepted depth and
 * copies the texture word at the masked split U/V coordinates into the
 * destination, including zero texture values.
 */
// FUNCTION: FF7 0x006bab93
__declspec(naked) void __cdecl Gfx_RasterDirectTextureSpanDepthWrite(void)
{
    __asm {
        push ebp
        mov ebp, esp
        push ebx
        push esi
        push edi
        push ebp
        mov eax, dword ptr [ebp + 8]
        mov dword ptr [eax + 158h], esp
        mov esp, eax
        mov eax, dword ptr [esp + 0ch]
        mov edi, dword ptr [esp + 8]
        shl eax, 0ch
        mov esi, dword ptr [esp + 40h]
        add edi, eax
        add esi, eax
        mov dword ptr [esp + 64h], esi
        mov eax, dword ptr [esp + 1ch]
        mov ecx, dword ptr [esp + 2ch]
        mov dword ptr [esp + 38h], edi
    asm_60:
        sub ecx, eax
        jle asm_63
        add dword ptr [esp], ecx
        lea edi, [edi + eax*2]
        mov ebp, ecx
        mov ebx, dword ptr [esp + 64h]
        lea ebx, [ebx + eax*4]
        mov edx, dword ptr [esp + 58h]
    asm_61:
        cmp edx, dword ptr [ebx]
        ja asm_62
        mov dword ptr [ebx], edx
        mov eax, dword ptr [esp + 118h]
        mov ecx, dword ptr [esp + 70h]
        mov edx, dword ptr [esp + 80h]
        and ecx, dword ptr [esp + 110h]
        and edx, dword ptr [esp + 114h]
        lea eax, [eax + ecx*2]
        mov cl, byte ptr [esp + 10ch]
        shl edx, cl
        xor ecx, ecx
        mov cx, word ptr [eax + edx*2]
        mov word ptr [edi], cx
    asm_62:
        add edi, 2
        add ebx, 4
        mov ecx, dword ptr [esp + 6ch]
        mov edx, dword ptr [esp + 70h]
        add ecx, dword ptr [esp + 74h]
        adc edx, dword ptr [esp + 78h]
        mov dword ptr [esp + 6ch], ecx
        mov dword ptr [esp + 70h], edx
        mov ecx, dword ptr [esp + 7ch]
        mov edx, dword ptr [esp + 80h]
        add ecx, dword ptr [esp + 84h]
        adc edx, dword ptr [esp + 88h]
        mov dword ptr [esp + 7ch], ecx
        mov dword ptr [esp + 80h], edx
        mov edx, dword ptr [esp + 58h]
        add edx, dword ptr [esp + 60h]
        mov dword ptr [esp + 58h], edx
        dec ebp
        jg asm_61
    asm_63:
        mov eax, dword ptr [esp + 64h]
        mov edx, dword ptr [esp + 48h]
        add eax, dword ptr [esp + 3ch]
        add edx, dword ptr [esp + 50h]
        mov dword ptr [esp + 64h], eax
        mov dword ptr [esp + 48h], edx
        mov dword ptr [esp + 58h], edx
        mov ebx, dword ptr [esp + 0ech]
        mov edx, dword ptr [esp + 0f0h]
        add ebx, dword ptr [esp + 0f4h]
        adc edx, dword ptr [esp + 0f8h]
        mov dword ptr [esp + 0ech], ebx
        mov dword ptr [esp + 0f0h], edx
        mov dword ptr [esp + 6ch], ebx
        mov dword ptr [esp + 70h], edx
        mov ebx, dword ptr [esp + 0fch]
        mov edx, dword ptr [esp + 100h]
        add ebx, dword ptr [esp + 104h]
        adc edx, dword ptr [esp + 108h]
        mov dword ptr [esp + 0fch], ebx
        mov dword ptr [esp + 100h], edx
        mov dword ptr [esp + 7ch], ebx
        mov dword ptr [esp + 80h], edx
        mov ebx, dword ptr [esp + 18h]
        mov eax, dword ptr [esp + 1ch]
        add ebx, dword ptr [esp + 20h]
        adc eax, dword ptr [esp + 24h]
        mov dword ptr [esp + 18h], ebx
        mov dword ptr [esp + 1ch], eax
        mov edx, dword ptr [esp + 28h]
        mov ecx, dword ptr [esp + 2ch]
        add edx, dword ptr [esp + 30h]
        adc ecx, dword ptr [esp + 34h]
        mov dword ptr [esp + 28h], edx
        mov dword ptr [esp + 2ch], ecx
        mov edi, dword ptr [esp + 38h]
        mov ebx, dword ptr [esp + 10h]
        add edi, dword ptr [esp + 4]
        dec ebx
        mov dword ptr [esp + 38h], edi
        mov dword ptr [esp + 10h], ebx
        jg asm_60
        mov esp, dword ptr [esp + 158h]
        pop ebp
        pop edi
        pop esi
        pop ebx
        pop ebp
        ret
    }
}

/*
 * Original hand-written indexed-texture span rasterizer using the argument
 * frame as ESP. Skips texture index zero, then tests unsigned interpolated
 * depth as no-greater than stored depth.
 */
// FUNCTION: FF7 0x006bad4e
__declspec(naked) void __cdecl Gfx_RasterIndexedTextureSpanColorKeyBlend(void)
{
    __asm {
        push ebp
        mov ebp, esp
        push ebx
        push esi
        push edi
        push ebp
        mov eax, dword ptr [ebp + 8]
        mov dword ptr [eax + 158h], esp
        mov esp, eax
        mov eax, dword ptr [esp + 0ch]
        mov edi, dword ptr [esp + 8]
        shl eax, 0ch
        mov esi, dword ptr [esp + 40h]
        add edi, eax
        add esi, eax
        mov dword ptr [esp + 64h], esi
        mov eax, dword ptr [esp + 1ch]
        mov ecx, dword ptr [esp + 2ch]
        mov dword ptr [esp + 38h], edi
    asm_64:
        sub ecx, eax
        jle asm_67
        add dword ptr [esp], ecx
        lea edi, [edi + eax*2]
        mov ebp, ecx
        mov ebx, dword ptr [esp + 64h]
        lea ebx, [ebx + eax*4]
    asm_65:
        mov eax, dword ptr [esp + 118h]
        mov ecx, dword ptr [esp + 70h]
        mov edx, dword ptr [esp + 80h]
        and ecx, dword ptr [esp + 110h]
        and edx, dword ptr [esp + 114h]
        lea eax, [eax + ecx]
        mov cl, byte ptr [esp + 10ch]
        shl edx, cl
        xor ecx, ecx
        mov cl, byte ptr [eax + edx]
        cmp ecx, 0
        je asm_66
        mov edx, dword ptr [esp + 58h]
        cmp edx, dword ptr [ebx]
        ja asm_66
        mov eax, dword ptr [esp + 11ch]
        mov eax, dword ptr [eax + ecx*4]
        mov ecx, dword ptr [esp + 13ch]
        xor edx, edx
        mov dx, word ptr [edi]
        mov edx, dword ptr [ecx + edx*4]
        mov eax, dword ptr [ecx + eax*4]
        add edx, eax
        mov word ptr [edi], dx
    asm_66:
        add edi, 2
        add ebx, 4
        mov ecx, dword ptr [esp + 6ch]
        mov edx, dword ptr [esp + 70h]
        add ecx, dword ptr [esp + 74h]
        adc edx, dword ptr [esp + 78h]
        mov dword ptr [esp + 6ch], ecx
        mov dword ptr [esp + 70h], edx
        mov ecx, dword ptr [esp + 7ch]
        mov edx, dword ptr [esp + 80h]
        add ecx, dword ptr [esp + 84h]
        adc edx, dword ptr [esp + 88h]
        mov dword ptr [esp + 7ch], ecx
        mov dword ptr [esp + 80h], edx
        mov edx, dword ptr [esp + 58h]
        add edx, dword ptr [esp + 60h]
        mov dword ptr [esp + 58h], edx
        dec ebp
        jg asm_65
    asm_67:
        mov eax, dword ptr [esp + 64h]
        mov edx, dword ptr [esp + 48h]
        add eax, dword ptr [esp + 3ch]
        add edx, dword ptr [esp + 50h]
        mov dword ptr [esp + 64h], eax
        mov dword ptr [esp + 48h], edx
        mov dword ptr [esp + 58h], edx
        mov ebx, dword ptr [esp + 0ech]
        mov edx, dword ptr [esp + 0f0h]
        add ebx, dword ptr [esp + 0f4h]
        adc edx, dword ptr [esp + 0f8h]
        mov dword ptr [esp + 0ech], ebx
        mov dword ptr [esp + 0f0h], edx
        mov dword ptr [esp + 6ch], ebx
        mov dword ptr [esp + 70h], edx
        mov ebx, dword ptr [esp + 0fch]
        mov edx, dword ptr [esp + 100h]
        add ebx, dword ptr [esp + 104h]
        adc edx, dword ptr [esp + 108h]
        mov dword ptr [esp + 0fch], ebx
        mov dword ptr [esp + 100h], edx
        mov dword ptr [esp + 7ch], ebx
        mov dword ptr [esp + 80h], edx
        mov ebx, dword ptr [esp + 18h]
        mov eax, dword ptr [esp + 1ch]
        add ebx, dword ptr [esp + 20h]
        adc eax, dword ptr [esp + 24h]
        mov dword ptr [esp + 18h], ebx
        mov dword ptr [esp + 1ch], eax
        mov edx, dword ptr [esp + 28h]
        mov ecx, dword ptr [esp + 2ch]
        add edx, dword ptr [esp + 30h]
        adc ecx, dword ptr [esp + 34h]
        mov dword ptr [esp + 28h], edx
        mov dword ptr [esp + 2ch], ecx
        mov edi, dword ptr [esp + 38h]
        mov ebx, dword ptr [esp + 10h]
        add edi, dword ptr [esp + 4]
        dec ebx
        mov dword ptr [esp + 38h], edi
        mov dword ptr [esp + 10h], ebx
        jg asm_64
        mov esp, dword ptr [esp + 158h]
        pop ebp
        pop edi
        pop esi
        pop ebx
        pop ebp
        ret
    }
}

/*
 * Original hand-written indexed-texture span rasterizer using the argument
 * frame as ESP. Loads texture bytes at masked split U/V coordinates, including
 * index zero, and tests unsigned interpolated depth as no-greater than stored
 * depth.
 */
// FUNCTION: FF7 0x006baf29
__declspec(naked) void __cdecl Gfx_RasterIndexedTextureSpanBlend(void)
{
    __asm {
        push ebp
        mov ebp, esp
        push ebx
        push esi
        push edi
        push ebp
        mov eax, dword ptr [ebp + 8]
        mov dword ptr [eax + 158h], esp
        mov esp, eax
        mov eax, dword ptr [esp + 0ch]
        mov edi, dword ptr [esp + 8]
        shl eax, 0ch
        mov esi, dword ptr [esp + 40h]
        add edi, eax
        add esi, eax
        mov dword ptr [esp + 64h], esi
        mov eax, dword ptr [esp + 1ch]
        mov ecx, dword ptr [esp + 2ch]
        mov dword ptr [esp + 38h], edi
    asm_68:
        sub ecx, eax
        jle asm_71
        add dword ptr [esp], ecx
        lea edi, [edi + eax*2]
        mov ebp, ecx
        mov ebx, dword ptr [esp + 64h]
        lea ebx, [ebx + eax*4]
    asm_69:
        mov eax, dword ptr [esp + 118h]
        mov ecx, dword ptr [esp + 70h]
        mov edx, dword ptr [esp + 80h]
        and ecx, dword ptr [esp + 110h]
        and edx, dword ptr [esp + 114h]
        lea eax, [eax + ecx]
        mov cl, byte ptr [esp + 10ch]
        shl edx, cl
        xor ecx, ecx
        mov cl, byte ptr [eax + edx]
        mov edx, dword ptr [esp + 58h]
        cmp edx, dword ptr [ebx]
        ja asm_70
        mov eax, dword ptr [esp + 11ch]
        mov eax, dword ptr [eax + ecx*4]
        mov ecx, dword ptr [esp + 13ch]
        xor edx, edx
        mov dx, word ptr [edi]
        mov edx, dword ptr [ecx + edx*4]
        mov eax, dword ptr [ecx + eax*4]
        add edx, eax
        mov word ptr [edi], dx
    asm_70:
        add edi, 2
        add ebx, 4
        mov ecx, dword ptr [esp + 6ch]
        mov edx, dword ptr [esp + 70h]
        add ecx, dword ptr [esp + 74h]
        adc edx, dword ptr [esp + 78h]
        mov dword ptr [esp + 6ch], ecx
        mov dword ptr [esp + 70h], edx
        mov ecx, dword ptr [esp + 7ch]
        mov edx, dword ptr [esp + 80h]
        add ecx, dword ptr [esp + 84h]
        adc edx, dword ptr [esp + 88h]
        mov dword ptr [esp + 7ch], ecx
        mov dword ptr [esp + 80h], edx
        mov edx, dword ptr [esp + 58h]
        add edx, dword ptr [esp + 60h]
        mov dword ptr [esp + 58h], edx
        dec ebp
        jg asm_69
    asm_71:
        mov eax, dword ptr [esp + 64h]
        mov edx, dword ptr [esp + 48h]
        add eax, dword ptr [esp + 3ch]
        add edx, dword ptr [esp + 50h]
        mov dword ptr [esp + 64h], eax
        mov dword ptr [esp + 48h], edx
        mov dword ptr [esp + 58h], edx
        mov ebx, dword ptr [esp + 0ech]
        mov edx, dword ptr [esp + 0f0h]
        add ebx, dword ptr [esp + 0f4h]
        adc edx, dword ptr [esp + 0f8h]
        mov dword ptr [esp + 0ech], ebx
        mov dword ptr [esp + 0f0h], edx
        mov dword ptr [esp + 6ch], ebx
        mov dword ptr [esp + 70h], edx
        mov ebx, dword ptr [esp + 0fch]
        mov edx, dword ptr [esp + 100h]
        add ebx, dword ptr [esp + 104h]
        adc edx, dword ptr [esp + 108h]
        mov dword ptr [esp + 0fch], ebx
        mov dword ptr [esp + 100h], edx
        mov dword ptr [esp + 7ch], ebx
        mov dword ptr [esp + 80h], edx
        mov ebx, dword ptr [esp + 18h]
        mov eax, dword ptr [esp + 1ch]
        add ebx, dword ptr [esp + 20h]
        adc eax, dword ptr [esp + 24h]
        mov dword ptr [esp + 18h], ebx
        mov dword ptr [esp + 1ch], eax
        mov edx, dword ptr [esp + 28h]
        mov ecx, dword ptr [esp + 2ch]
        add edx, dword ptr [esp + 30h]
        adc ecx, dword ptr [esp + 34h]
        mov dword ptr [esp + 28h], edx
        mov dword ptr [esp + 2ch], ecx
        mov edi, dword ptr [esp + 38h]
        mov ebx, dword ptr [esp + 10h]
        add edi, dword ptr [esp + 4]
        dec ebx
        mov dword ptr [esp + 38h], edi
        mov dword ptr [esp + 10h], ebx
        jg asm_68
        mov esp, dword ptr [esp + 158h]
        pop ebp
        pop edi
        pop esi
        pop ebx
        pop ebp
        ret
    }
}

/*
 * Original hand-written direct 16-bit textured span rasterizer using the
 * argument frame as ESP. Loads texture words at masked split U/V coordinates,
 * skipping values equal to the full 32-bit key at +0x14C, then tests unsigned
 * interpolated depth as no-greater than stored depth.
 */
// FUNCTION: FF7 0x006bb0ff
__declspec(naked) void __cdecl Gfx_RasterDirectTextureSpanColorKeyBlend(void)
{
    __asm {
        push ebp
        mov ebp, esp
        push ebx
        push esi
        push edi
        push ebp
        mov eax, dword ptr [ebp + 8]
        mov dword ptr [eax + 158h], esp
        mov esp, eax
        mov eax, dword ptr [esp + 0ch]
        mov edi, dword ptr [esp + 8]
        shl eax, 0ch
        mov esi, dword ptr [esp + 40h]
        add edi, eax
        add esi, eax
        mov dword ptr [esp + 64h], esi
        mov eax, dword ptr [esp + 1ch]
        mov ecx, dword ptr [esp + 2ch]
        mov dword ptr [esp + 38h], edi
    asm_72:
        sub ecx, eax
        jle asm_75
        add dword ptr [esp], ecx
        lea edi, [edi + eax*2]
        mov ebp, ecx
        mov ebx, dword ptr [esp + 64h]
        lea ebx, [ebx + eax*4]
    asm_73:
        mov eax, dword ptr [esp + 118h]
        mov ecx, dword ptr [esp + 70h]
        mov edx, dword ptr [esp + 80h]
        and ecx, dword ptr [esp + 110h]
        and edx, dword ptr [esp + 114h]
        lea eax, [eax + ecx*2]
        mov cl, byte ptr [esp + 10ch]
        shl edx, cl
        xor ecx, ecx
        mov cx, word ptr [eax + edx*2]
        cmp ecx, dword ptr [esp + 14ch]
        je asm_74
        mov edx, dword ptr [esp + 58h]
        cmp edx, dword ptr [ebx]
        ja asm_74
        mov eax, ecx
        mov ecx, dword ptr [esp + 13ch]
        xor edx, edx
        mov dx, word ptr [edi]
        mov edx, dword ptr [ecx + edx*4]
        mov eax, dword ptr [ecx + eax*4]
        add edx, eax
        mov word ptr [edi], dx
    asm_74:
        add edi, 2
        add ebx, 4
        mov ecx, dword ptr [esp + 6ch]
        mov edx, dword ptr [esp + 70h]
        add ecx, dword ptr [esp + 74h]
        adc edx, dword ptr [esp + 78h]
        mov dword ptr [esp + 6ch], ecx
        mov dword ptr [esp + 70h], edx
        mov ecx, dword ptr [esp + 7ch]
        mov edx, dword ptr [esp + 80h]
        add ecx, dword ptr [esp + 84h]
        adc edx, dword ptr [esp + 88h]
        mov dword ptr [esp + 7ch], ecx
        mov dword ptr [esp + 80h], edx
        mov edx, dword ptr [esp + 58h]
        add edx, dword ptr [esp + 60h]
        mov dword ptr [esp + 58h], edx
        dec ebp
        jg asm_73
    asm_75:
        mov eax, dword ptr [esp + 64h]
        mov edx, dword ptr [esp + 48h]
        add eax, dword ptr [esp + 3ch]
        add edx, dword ptr [esp + 50h]
        mov dword ptr [esp + 64h], eax
        mov dword ptr [esp + 48h], edx
        mov dword ptr [esp + 58h], edx
        mov ebx, dword ptr [esp + 0ech]
        mov edx, dword ptr [esp + 0f0h]
        add ebx, dword ptr [esp + 0f4h]
        adc edx, dword ptr [esp + 0f8h]
        mov dword ptr [esp + 0ech], ebx
        mov dword ptr [esp + 0f0h], edx
        mov dword ptr [esp + 6ch], ebx
        mov dword ptr [esp + 70h], edx
        mov ebx, dword ptr [esp + 0fch]
        mov edx, dword ptr [esp + 100h]
        add ebx, dword ptr [esp + 104h]
        adc edx, dword ptr [esp + 108h]
        mov dword ptr [esp + 0fch], ebx
        mov dword ptr [esp + 100h], edx
        mov dword ptr [esp + 7ch], ebx
        mov dword ptr [esp + 80h], edx
        mov ebx, dword ptr [esp + 18h]
        mov eax, dword ptr [esp + 1ch]
        add ebx, dword ptr [esp + 20h]
        adc eax, dword ptr [esp + 24h]
        mov dword ptr [esp + 18h], ebx
        mov dword ptr [esp + 1ch], eax
        mov edx, dword ptr [esp + 28h]
        mov ecx, dword ptr [esp + 2ch]
        add edx, dword ptr [esp + 30h]
        adc ecx, dword ptr [esp + 34h]
        mov dword ptr [esp + 28h], edx
        mov dword ptr [esp + 2ch], ecx
        mov edi, dword ptr [esp + 38h]
        mov ebx, dword ptr [esp + 10h]
        add edi, dword ptr [esp + 4]
        dec ebx
        mov dword ptr [esp + 38h], edi
        mov dword ptr [esp + 10h], ebx
        jg asm_72
        mov esp, dword ptr [esp + 158h]
        pop ebp
        pop edi
        pop esi
        pop ebx
        pop ebp
        ret
    }
}

/*
 * Original hand-written direct 16-bit textured span rasterizer using the
 * argument frame as ESP. Loads texture words from masked split U/V
 * coordinates, including zero, and tests unsigned interpolated depth as no-
 * greater than stored depth.
 */
// FUNCTION: FF7 0x006bb2d7
__declspec(naked) void __cdecl Gfx_RasterDirectTextureSpanBlend(void)
{
    __asm {
        push ebp
        mov ebp, esp
        push ebx
        push esi
        push edi
        push ebp
        mov eax, dword ptr [ebp + 8]
        mov dword ptr [eax + 158h], esp
        mov esp, eax
        mov eax, dword ptr [esp + 0ch]
        mov edi, dword ptr [esp + 8]
        shl eax, 0ch
        mov esi, dword ptr [esp + 40h]
        add edi, eax
        add esi, eax
        mov dword ptr [esp + 64h], esi
        mov eax, dword ptr [esp + 1ch]
        mov ecx, dword ptr [esp + 2ch]
        mov dword ptr [esp + 38h], edi
    asm_76:
        sub ecx, eax
        jle asm_79
        add dword ptr [esp], ecx
        lea edi, [edi + eax*2]
        mov ebp, ecx
        mov ebx, dword ptr [esp + 64h]
        lea ebx, [ebx + eax*4]
    asm_77:
        mov eax, dword ptr [esp + 118h]
        mov ecx, dword ptr [esp + 70h]
        mov edx, dword ptr [esp + 80h]
        and ecx, dword ptr [esp + 110h]
        and edx, dword ptr [esp + 114h]
        lea eax, [eax + ecx*2]
        mov cl, byte ptr [esp + 10ch]
        shl edx, cl
        xor ecx, ecx
        mov cx, word ptr [eax + edx*2]
        mov edx, dword ptr [esp + 58h]
        cmp edx, dword ptr [ebx]
        ja asm_78
        mov eax, ecx
        mov ecx, dword ptr [esp + 13ch]
        xor edx, edx
        mov dx, word ptr [edi]
        mov edx, dword ptr [ecx + edx*4]
        mov eax, dword ptr [ecx + eax*4]
        add edx, eax
        mov word ptr [edi], dx
    asm_78:
        add edi, 2
        add ebx, 4
        mov ecx, dword ptr [esp + 6ch]
        mov edx, dword ptr [esp + 70h]
        add ecx, dword ptr [esp + 74h]
        adc edx, dword ptr [esp + 78h]
        mov dword ptr [esp + 6ch], ecx
        mov dword ptr [esp + 70h], edx
        mov ecx, dword ptr [esp + 7ch]
        mov edx, dword ptr [esp + 80h]
        add ecx, dword ptr [esp + 84h]
        adc edx, dword ptr [esp + 88h]
        mov dword ptr [esp + 7ch], ecx
        mov dword ptr [esp + 80h], edx
        mov edx, dword ptr [esp + 58h]
        add edx, dword ptr [esp + 60h]
        mov dword ptr [esp + 58h], edx
        dec ebp
        jg asm_77
    asm_79:
        mov eax, dword ptr [esp + 64h]
        mov edx, dword ptr [esp + 48h]
        add eax, dword ptr [esp + 3ch]
        add edx, dword ptr [esp + 50h]
        mov dword ptr [esp + 64h], eax
        mov dword ptr [esp + 48h], edx
        mov dword ptr [esp + 58h], edx
        mov ebx, dword ptr [esp + 0ech]
        mov edx, dword ptr [esp + 0f0h]
        add ebx, dword ptr [esp + 0f4h]
        adc edx, dword ptr [esp + 0f8h]
        mov dword ptr [esp + 0ech], ebx
        mov dword ptr [esp + 0f0h], edx
        mov dword ptr [esp + 6ch], ebx
        mov dword ptr [esp + 70h], edx
        mov ebx, dword ptr [esp + 0fch]
        mov edx, dword ptr [esp + 100h]
        add ebx, dword ptr [esp + 104h]
        adc edx, dword ptr [esp + 108h]
        mov dword ptr [esp + 0fch], ebx
        mov dword ptr [esp + 100h], edx
        mov dword ptr [esp + 7ch], ebx
        mov dword ptr [esp + 80h], edx
        mov ebx, dword ptr [esp + 18h]
        mov eax, dword ptr [esp + 1ch]
        add ebx, dword ptr [esp + 20h]
        adc eax, dword ptr [esp + 24h]
        mov dword ptr [esp + 18h], ebx
        mov dword ptr [esp + 1ch], eax
        mov edx, dword ptr [esp + 28h]
        mov ecx, dword ptr [esp + 2ch]
        add edx, dword ptr [esp + 30h]
        adc ecx, dword ptr [esp + 34h]
        mov dword ptr [esp + 28h], edx
        mov dword ptr [esp + 2ch], ecx
        mov edi, dword ptr [esp + 38h]
        mov ebx, dword ptr [esp + 10h]
        add edi, dword ptr [esp + 4]
        dec ebx
        mov dword ptr [esp + 38h], edi
        mov dword ptr [esp + 10h], ebx
        jg asm_76
        mov esp, dword ptr [esp + 158h]
        pop ebp
        pop edi
        pop esi
        pop ebx
        pop ebp
        ret
    }
}

/*
 * Original hand-written direct 16-bit textured span rasterizer using the
 * argument frame as ESP. Skips a zero-extended texture word equal to the full
 * 32-bit key at +0x14C, then tests unsigned interpolated depth as no-greater
 * than stored depth.
 */
// FUNCTION: FF7 0x006bb4a6
__declspec(naked) void __cdecl Gfx_RasterDirectTextureSpanColorKeyBlendRemap(void)
{
    __asm {
        push ebp
        mov ebp, esp
        push ebx
        push esi
        push edi
        push ebp
        mov eax, dword ptr [ebp + 8]
        mov dword ptr [eax + 158h], esp
        mov esp, eax
        mov eax, dword ptr [esp + 0ch]
        mov edi, dword ptr [esp + 8]
        shl eax, 0ch
        mov esi, dword ptr [esp + 40h]
        add edi, eax
        add esi, eax
        mov dword ptr [esp + 64h], esi
        mov eax, dword ptr [esp + 1ch]
        mov ecx, dword ptr [esp + 2ch]
        mov dword ptr [esp + 38h], edi
    asm_80:
        sub ecx, eax
        jle asm_83
        add dword ptr [esp], ecx
        lea edi, [edi + eax*2]
        mov ebp, ecx
        mov ebx, dword ptr [esp + 64h]
        lea ebx, [ebx + eax*4]
    asm_81:
        mov eax, dword ptr [esp + 118h]
        mov ecx, dword ptr [esp + 70h]
        mov edx, dword ptr [esp + 80h]
        and ecx, dword ptr [esp + 110h]
        and edx, dword ptr [esp + 114h]
        lea eax, [eax + ecx*2]
        mov cl, byte ptr [esp + 10ch]
        shl edx, cl
        xor ecx, ecx
        mov cx, word ptr [eax + edx*2]
        cmp ecx, dword ptr [esp + 14ch]
        je asm_82
        mov edx, dword ptr [esp + 58h]
        cmp edx, dword ptr [ebx]
        ja asm_82
        mov eax, ecx
        mov ecx, dword ptr [esp + 13ch]
        xor edx, edx
        mov dx, word ptr [edi]
        mov edx, dword ptr [ecx + edx*4]
        mov eax, dword ptr [ecx + eax*4]
        add edx, eax
        mov ecx, dword ptr [esp + 140h]
        mov eax, dword ptr [ecx + edx*4]
        mov word ptr [edi], ax
    asm_82:
        add edi, 2
        add ebx, 4
        mov ecx, dword ptr [esp + 6ch]
        mov edx, dword ptr [esp + 70h]
        add ecx, dword ptr [esp + 74h]
        adc edx, dword ptr [esp + 78h]
        mov dword ptr [esp + 6ch], ecx
        mov dword ptr [esp + 70h], edx
        mov ecx, dword ptr [esp + 7ch]
        mov edx, dword ptr [esp + 80h]
        add ecx, dword ptr [esp + 84h]
        adc edx, dword ptr [esp + 88h]
        mov dword ptr [esp + 7ch], ecx
        mov dword ptr [esp + 80h], edx
        mov edx, dword ptr [esp + 58h]
        add edx, dword ptr [esp + 60h]
        mov dword ptr [esp + 58h], edx
        dec ebp
        jg asm_81
    asm_83:
        mov eax, dword ptr [esp + 64h]
        mov edx, dword ptr [esp + 48h]
        add eax, dword ptr [esp + 3ch]
        add edx, dword ptr [esp + 50h]
        mov dword ptr [esp + 64h], eax
        mov dword ptr [esp + 48h], edx
        mov dword ptr [esp + 58h], edx
        mov ebx, dword ptr [esp + 0ech]
        mov edx, dword ptr [esp + 0f0h]
        add ebx, dword ptr [esp + 0f4h]
        adc edx, dword ptr [esp + 0f8h]
        mov dword ptr [esp + 0ech], ebx
        mov dword ptr [esp + 0f0h], edx
        mov dword ptr [esp + 6ch], ebx
        mov dword ptr [esp + 70h], edx
        mov ebx, dword ptr [esp + 0fch]
        mov edx, dword ptr [esp + 100h]
        add ebx, dword ptr [esp + 104h]
        adc edx, dword ptr [esp + 108h]
        mov dword ptr [esp + 0fch], ebx
        mov dword ptr [esp + 100h], edx
        mov dword ptr [esp + 7ch], ebx
        mov dword ptr [esp + 80h], edx
        mov ebx, dword ptr [esp + 18h]
        mov eax, dword ptr [esp + 1ch]
        add ebx, dword ptr [esp + 20h]
        adc eax, dword ptr [esp + 24h]
        mov dword ptr [esp + 18h], ebx
        mov dword ptr [esp + 1ch], eax
        mov edx, dword ptr [esp + 28h]
        mov ecx, dword ptr [esp + 2ch]
        add edx, dword ptr [esp + 30h]
        adc ecx, dword ptr [esp + 34h]
        mov dword ptr [esp + 28h], edx
        mov dword ptr [esp + 2ch], ecx
        mov edi, dword ptr [esp + 38h]
        mov ebx, dword ptr [esp + 10h]
        add edi, dword ptr [esp + 4]
        dec ebx
        mov dword ptr [esp + 38h], edi
        mov dword ptr [esp + 10h], ebx
        jg asm_80
        mov esp, dword ptr [esp + 158h]
        pop ebp
        pop edi
        pop esi
        pop ebx
        pop ebp
        ret
    }
}

/*
 * Original hand-written direct 16-bit textured span rasterizer using the
 * argument frame as ESP. Loads texture words at masked split U/V coordinates,
 * including zero, then tests unsigned interpolated depth as no-greater than
 * stored depth.
 */
// FUNCTION: FF7 0x006bb688
__declspec(naked) void __cdecl Gfx_RasterDirectTextureSpanBlendRemap(void)
{
    __asm {
        push ebp
        mov ebp, esp
        push ebx
        push esi
        push edi
        push ebp
        mov eax, dword ptr [ebp + 8]
        mov dword ptr [eax + 158h], esp
        mov esp, eax
        mov eax, dword ptr [esp + 0ch]
        mov edi, dword ptr [esp + 8]
        shl eax, 0ch
        mov esi, dword ptr [esp + 40h]
        add edi, eax
        add esi, eax
        mov dword ptr [esp + 64h], esi
        mov eax, dword ptr [esp + 1ch]
        mov ecx, dword ptr [esp + 2ch]
        mov dword ptr [esp + 38h], edi
    asm_84:
        sub ecx, eax
        jle asm_87
        add dword ptr [esp], ecx
        lea edi, [edi + eax*2]
        mov ebp, ecx
        mov ebx, dword ptr [esp + 64h]
        lea ebx, [ebx + eax*4]
    asm_85:
        mov eax, dword ptr [esp + 118h]
        mov ecx, dword ptr [esp + 70h]
        mov edx, dword ptr [esp + 80h]
        and ecx, dword ptr [esp + 110h]
        and edx, dword ptr [esp + 114h]
        lea eax, [eax + ecx*2]
        mov cl, byte ptr [esp + 10ch]
        shl edx, cl
        xor ecx, ecx
        mov cx, word ptr [eax + edx*2]
        mov edx, dword ptr [esp + 58h]
        cmp edx, dword ptr [ebx]
        ja asm_86
        mov eax, ecx
        mov ecx, dword ptr [esp + 13ch]
        xor edx, edx
        mov dx, word ptr [edi]
        mov edx, dword ptr [ecx + edx*4]
        mov eax, dword ptr [ecx + eax*4]
        add edx, eax
        mov ecx, dword ptr [esp + 140h]
        mov eax, dword ptr [ecx + edx*4]
        mov word ptr [edi], ax
    asm_86:
        add edi, 2
        add ebx, 4
        mov ecx, dword ptr [esp + 6ch]
        mov edx, dword ptr [esp + 70h]
        add ecx, dword ptr [esp + 74h]
        adc edx, dword ptr [esp + 78h]
        mov dword ptr [esp + 6ch], ecx
        mov dword ptr [esp + 70h], edx
        mov ecx, dword ptr [esp + 7ch]
        mov edx, dword ptr [esp + 80h]
        add ecx, dword ptr [esp + 84h]
        adc edx, dword ptr [esp + 88h]
        mov dword ptr [esp + 7ch], ecx
        mov dword ptr [esp + 80h], edx
        mov edx, dword ptr [esp + 58h]
        add edx, dword ptr [esp + 60h]
        mov dword ptr [esp + 58h], edx
        dec ebp
        jg asm_85
    asm_87:
        mov eax, dword ptr [esp + 64h]
        mov edx, dword ptr [esp + 48h]
        add eax, dword ptr [esp + 3ch]
        add edx, dword ptr [esp + 50h]
        mov dword ptr [esp + 64h], eax
        mov dword ptr [esp + 48h], edx
        mov dword ptr [esp + 58h], edx
        mov ebx, dword ptr [esp + 0ech]
        mov edx, dword ptr [esp + 0f0h]
        add ebx, dword ptr [esp + 0f4h]
        adc edx, dword ptr [esp + 0f8h]
        mov dword ptr [esp + 0ech], ebx
        mov dword ptr [esp + 0f0h], edx
        mov dword ptr [esp + 6ch], ebx
        mov dword ptr [esp + 70h], edx
        mov ebx, dword ptr [esp + 0fch]
        mov edx, dword ptr [esp + 100h]
        add ebx, dword ptr [esp + 104h]
        adc edx, dword ptr [esp + 108h]
        mov dword ptr [esp + 0fch], ebx
        mov dword ptr [esp + 100h], edx
        mov dword ptr [esp + 7ch], ebx
        mov dword ptr [esp + 80h], edx
        mov ebx, dword ptr [esp + 18h]
        mov eax, dword ptr [esp + 1ch]
        add ebx, dword ptr [esp + 20h]
        adc eax, dword ptr [esp + 24h]
        mov dword ptr [esp + 18h], ebx
        mov dword ptr [esp + 1ch], eax
        mov edx, dword ptr [esp + 28h]
        mov ecx, dword ptr [esp + 2ch]
        add edx, dword ptr [esp + 30h]
        adc ecx, dword ptr [esp + 34h]
        mov dword ptr [esp + 28h], edx
        mov dword ptr [esp + 2ch], ecx
        mov edi, dword ptr [esp + 38h]
        mov ebx, dword ptr [esp + 10h]
        add edi, dword ptr [esp + 4]
        dec ebx
        mov dword ptr [esp + 38h], edi
        mov dword ptr [esp + 10h], ebx
        jg asm_84
        mov esp, dword ptr [esp + 158h]
        pop ebp
        pop edi
        pop esi
        pop ebx
        pop ebp
        ret
    }
}

/*
 * Original hand-written span rasterizer using the argument frame as ESP after
 * saving the caller stack. For each positive-width span, increments the
 * processed-pixel counter, tests interpolated depth as unsigned no-greater
 * than the existing depth and recolors the existing 16-bit destination through
 * the +0x150 packed-color table.
 */
// FUNCTION: FF7 0x006bb861
__declspec(naked) void __cdecl Gfx_RasterSpanRecolorDestination(void)
{
    __asm {
        push ebp
        mov ebp, esp
        push ebx
        push esi
        push edi
        push ebp
        mov eax, dword ptr [ebp + 8]
        mov dword ptr [eax + 158h], esp
        mov esp, eax
        mov eax, dword ptr [esp + 0ch]
        mov edi, dword ptr [esp + 8]
        shl eax, 0ch
        mov esi, dword ptr [esp + 40h]
        add edi, eax
        add esi, eax
        mov eax, dword ptr [esp + 70h]
        mov edx, dword ptr [esp + 154h]
        shl eax, 8
        add edx, eax
        mov dword ptr [esp + 6ch], edx
        mov dword ptr [esp + 64h], esi
        mov eax, dword ptr [esp + 1ch]
        mov ecx, dword ptr [esp + 2ch]
        mov dword ptr [esp + 38h], edi
    asm_88:
        sub ecx, eax
        jle asm_91
        add dword ptr [esp], ecx
        lea edi, [edi + eax*2]
        mov ebp, ecx
        mov esi, dword ptr [esp + 64h]
        lea esi, [esi + eax*4]
        mov edx, dword ptr [esp + 58h]
        mov ecx, dword ptr [esp + 54h]
    asm_89:
        cmp edx, dword ptr [esi]
        ja asm_90
        xor eax, eax
        mov ecx, dword ptr [esp + 150h]
        mov ax, word ptr [edi]
        mov eax, dword ptr [ecx + eax*4]
        mov dword ptr [esp + 14ch], eax
        mov edx, dword ptr [esp + 6ch]
        xor eax, eax
        xor ecx, ecx
        mov al, byte ptr [esp + 14eh]
        mov al, byte ptr [edx + eax]
        mov byte ptr [esp + 14eh], al
        xor eax, eax
        mov al, byte ptr [esp + 14dh]
        mov al, byte ptr [edx + eax]
        mov byte ptr [esp + 14dh], al
        xor eax, eax
        mov al, byte ptr [esp + 14ch]
        mov al, byte ptr [edx + eax]
        mov byte ptr [esp + 14ch], al
        xor eax, eax
        xor ecx, ecx
        mov al, byte ptr [esp + 14eh]
        mov ebx, dword ptr [esp + 124h]
        mov cl, byte ptr [esp + 14dh]
        mov edx, dword ptr [esp + 128h]
        mov eax, dword ptr [ebx + eax*4]
        or eax, dword ptr [edx + ecx*4]
        mov cl, byte ptr [esp + 14ch]
        mov edx, dword ptr [esp + 12ch]
        or eax, dword ptr [edx + ecx*4]
        add eax, dword ptr [esp + 14h]
        mov word ptr [edi], ax
    asm_90:
        add edi, 2
        add esi, 4
        mov edx, dword ptr [esp + 58h]
        add edx, dword ptr [esp + 60h]
        mov dword ptr [esp + 58h], edx
        dec ebp
        jg asm_89
    asm_91:
        mov eax, dword ptr [esp + 64h]
        mov edx, dword ptr [esp + 48h]
        add eax, dword ptr [esp + 3ch]
        add edx, dword ptr [esp + 50h]
        mov dword ptr [esp + 64h], eax
        mov dword ptr [esp + 48h], edx
        mov dword ptr [esp + 58h], edx
        mov ebx, dword ptr [esp + 18h]
        mov eax, dword ptr [esp + 1ch]
        add ebx, dword ptr [esp + 20h]
        adc eax, dword ptr [esp + 24h]
        mov dword ptr [esp + 18h], ebx
        mov dword ptr [esp + 1ch], eax
        mov edx, dword ptr [esp + 28h]
        mov ecx, dword ptr [esp + 2ch]
        add edx, dword ptr [esp + 30h]
        adc ecx, dword ptr [esp + 34h]
        mov dword ptr [esp + 28h], edx
        mov dword ptr [esp + 2ch], ecx
        mov edi, dword ptr [esp + 38h]
        mov ebx, dword ptr [esp + 10h]
        add edi, dword ptr [esp + 4]
        dec ebx
        mov dword ptr [esp + 38h], edi
        mov dword ptr [esp + 10h], ebx
        jg asm_88
        mov esp, dword ptr [esp + 158h]
        pop ebp
        pop edi
        pop esi
        pop ebx
        pop ebp
        ret
    }
}

/*
 * Original hand-written direct 16-bit textured span rasterizer using the
 * argument frame as ESP. Skips a zero-extended texture word when it equals the
 * full 32-bit key at +0x14C, then tests unsigned interpolated depth as no-
 * greater than stored depth.
 */
// FUNCTION: FF7 0x006bb9f1
__declspec(naked) void __cdecl Gfx_RasterDirectTextureSpanColorKeyBlendDestRemap(void)
{
    __asm {
        push ebp
        mov ebp, esp
        push ebx
        push esi
        push edi
        push ebp
        mov eax, dword ptr [ebp + 8]
        mov dword ptr [eax + 158h], esp
        mov esp, eax
        mov eax, dword ptr [esp + 0ch]
        mov edi, dword ptr [esp + 8]
        shl eax, 0ch
        mov esi, dword ptr [esp + 40h]
        add edi, eax
        add esi, eax
        mov dword ptr [esp + 64h], esi
        mov eax, dword ptr [esp + 1ch]
        mov ecx, dword ptr [esp + 2ch]
        mov dword ptr [esp + 38h], edi
    asm_92:
        sub ecx, eax
        jle asm_95
        add dword ptr [esp], ecx
        lea edi, [edi + eax*2]
        mov ebp, ecx
        mov ebx, dword ptr [esp + 64h]
        lea ebx, [ebx + eax*4]
    asm_93:
        mov eax, dword ptr [esp + 118h]
        mov ecx, dword ptr [esp + 70h]
        mov edx, dword ptr [esp + 80h]
        and ecx, dword ptr [esp + 110h]
        and edx, dword ptr [esp + 114h]
        lea eax, [eax + ecx*2]
        mov cl, byte ptr [esp + 10ch]
        shl edx, cl
        xor ecx, ecx
        mov cx, word ptr [eax + edx*2]
        cmp ecx, dword ptr [esp + 14ch]
        je asm_94
        mov edx, dword ptr [esp + 58h]
        cmp edx, dword ptr [ebx]
        ja asm_94
        mov eax, ecx
        mov ecx, dword ptr [esp + 140h]
        xor edx, edx
        mov dx, word ptr [edi]
        mov edx, dword ptr [ecx + edx*4]
        mov ecx, dword ptr [esp + 13ch]
        mov eax, dword ptr [ecx + eax*4]
        add edx, eax
        mov word ptr [edi], dx
    asm_94:
        add edi, 2
        add ebx, 4
        mov ecx, dword ptr [esp + 6ch]
        mov edx, dword ptr [esp + 70h]
        add ecx, dword ptr [esp + 74h]
        adc edx, dword ptr [esp + 78h]
        mov dword ptr [esp + 6ch], ecx
        mov dword ptr [esp + 70h], edx
        mov ecx, dword ptr [esp + 7ch]
        mov edx, dword ptr [esp + 80h]
        add ecx, dword ptr [esp + 84h]
        adc edx, dword ptr [esp + 88h]
        mov dword ptr [esp + 7ch], ecx
        mov dword ptr [esp + 80h], edx
        mov edx, dword ptr [esp + 58h]
        add edx, dword ptr [esp + 60h]
        mov dword ptr [esp + 58h], edx
        dec ebp
        jg asm_93
    asm_95:
        mov eax, dword ptr [esp + 64h]
        mov edx, dword ptr [esp + 48h]
        add eax, dword ptr [esp + 3ch]
        add edx, dword ptr [esp + 50h]
        mov dword ptr [esp + 64h], eax
        mov dword ptr [esp + 48h], edx
        mov dword ptr [esp + 58h], edx
        mov ebx, dword ptr [esp + 0ech]
        mov edx, dword ptr [esp + 0f0h]
        add ebx, dword ptr [esp + 0f4h]
        adc edx, dword ptr [esp + 0f8h]
        mov dword ptr [esp + 0ech], ebx
        mov dword ptr [esp + 0f0h], edx
        mov dword ptr [esp + 6ch], ebx
        mov dword ptr [esp + 70h], edx
        mov ebx, dword ptr [esp + 0fch]
        mov edx, dword ptr [esp + 100h]
        add ebx, dword ptr [esp + 104h]
        adc edx, dword ptr [esp + 108h]
        mov dword ptr [esp + 0fch], ebx
        mov dword ptr [esp + 100h], edx
        mov dword ptr [esp + 7ch], ebx
        mov dword ptr [esp + 80h], edx
        mov ebx, dword ptr [esp + 18h]
        mov eax, dword ptr [esp + 1ch]
        add ebx, dword ptr [esp + 20h]
        adc eax, dword ptr [esp + 24h]
        mov dword ptr [esp + 18h], ebx
        mov dword ptr [esp + 1ch], eax
        mov edx, dword ptr [esp + 28h]
        mov ecx, dword ptr [esp + 2ch]
        add edx, dword ptr [esp + 30h]
        adc ecx, dword ptr [esp + 34h]
        mov dword ptr [esp + 28h], edx
        mov dword ptr [esp + 2ch], ecx
        mov edi, dword ptr [esp + 38h]
        mov ebx, dword ptr [esp + 10h]
        add edi, dword ptr [esp + 4]
        dec ebx
        mov dword ptr [esp + 38h], edi
        mov dword ptr [esp + 10h], ebx
        jg asm_92
        mov esp, dword ptr [esp + 158h]
        pop ebp
        pop edi
        pop esi
        pop ebx
        pop ebp
        ret
    }
}

// Original 6BBBD0 begins a 16-byte-aligned contribution. Preserve its three
// complete preceding bodies so the helper at +452 retains its suffix.

} /* extern "C" */
