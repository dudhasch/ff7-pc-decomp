// src/lib/src/graphics/sw/sw_gfx_raster_line_draw.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Software line drawing: picks the span writer for each primitive mode, scans the
//   inclusive line between endpoints, and clips or rejects endpoints before drawing.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/types/graphics.h"
#include "lib/api/graphics.h"
#include "lib/crt.h"
#include "lib/enums/graphics.h"
#include "lib/codegen.h"
extern "C" {

// clang-format off
// clang-format on

// clang-format off
// GLOBAL: FF7 0x009198e0
void *g_GfxRasterLineSpanCallbacksMode4[6] = {
    (void *)0x006be38f, (void *)0x006be4a8, (void *)0x006be38f, (void *)0x006be4a8, (void *)0x006be38f
};
// GLOBAL: FF7 0x009198f8
void *g_GfxRasterLineSpanCallbacksMode0[6] = {
    (void *)0x006be40b, (void *)0x006be5c4, (void *)0x006be40b, (void *)0x006be5c4, (void *)0x006be40b
};
// GLOBAL: FF7 0x00919910
void *g_GfxRasterLineSpanCallbacksMode1[6] = {
    (void *)0x006be410, (void *)0x006be5c9, (void *)0x006be410, (void *)0x006be5c9, (void *)0x006be410
};
// clang-format on

void __cdecl crt_FloatToInt64(float value, void *out);
void __cdecl Gfx_RasterNegateFixed64(unsigned int *value);
void __cdecl Gfx_RasterLineSpanFlatDepthWrite(void *span);
void __cdecl Gfx_RasterLineSpanFlatBlendRemap(void *span);
void __cdecl Gfx_RasterLineSpanGouraudDepthWrite(void *span);
void __cdecl Gfx_RasterLineSpanGouraudBlendRemap(void *span);

/*
 * Empty. A blitter slot the shipped build fills with a function that does nothing.
 */
// FUNCTION: FF7 0x006be300
void __cdecl Gfx_RasterLineSpanNoOp(void)
{
    /* The original is empty. Its whole body is
       `55 8b ec 5d c3` -- 5 bytes at 0x006be300,
       push/mov/pop/ret plus alignment padding.
       The C body is empty, as in retail. */
}

// Stores the span writer for the primitive's blend mode in the line ops' draw callback. BLEND_AVERAGE,
// BLEND_ADDITIVE and BLEND_NONE index a six-entry callback table by callbackIndex; the subtractive and
// quarter-additive modes store Gfx_RasterLineSpanNoOp, so such lines are not drawn.
// FUNCTION: FF7 0x006be305
void __cdecl Gfx_RasterSelectLineSpanWriter(SwLineSpanPrim *prim, SwLineOps *span)
{
    switch (prim->mode) {
    case BLEND_AVERAGE:
        span->drawCallback =
            (int (__cdecl *)(int))g_GfxRasterLineSpanCallbacksMode0[prim->callbackIndex];
        break;
    case BLEND_ADDITIVE:
        span->drawCallback =
            (int (__cdecl *)(int))g_GfxRasterLineSpanCallbacksMode1[prim->callbackIndex];
        break;
    case BLEND_SUBTRACTIVE:
        span->drawCallback = (int (__cdecl *)(int))Gfx_RasterLineSpanNoOp;
        break;
    case BLEND_QUARTER_ADDITIVE:
        span->drawCallback = (int (__cdecl *)(int))Gfx_RasterLineSpanNoOp;
        break;
    case BLEND_NONE:
        span->drawCallback =
            (int (__cdecl *)(int))g_GfxRasterLineSpanCallbacksMode4[prim->callbackIndex];
        break;
    }
}

// Original naked, depth-tested 16-bit span blitter in hand-written assembly: it walks the colour and depth
// rows along the line, with the per-pixel steps and the pixel count taken from the line buffer.
// FUNCTION: FF7 0x006be38f
__declspec(naked) void __cdecl Gfx_RasterLineSpanFlatDepthWrite(void *span)
{
    __asm {
        push ebp
        mov ebp, esp
        push ebx
        push esi
        push edi
        mov esi, dword ptr [ebp+8]
        mov eax, dword ptr [esi+0ch]
        mov edx, dword ptr [esi+4]
        imul edx
        mov edi, dword ptr [esi+8]
        add edi, eax
        mov eax, dword ptr [esi+0ch]
        mov edx, dword ptr [esi+3ch]
        imul edx
        mov ebx, dword ptr [esi+40h]
        add ebx, eax
        mov dword ptr [esi+64h], ebx
        mov ebx, dword ptr [esi+64h]
        mov eax, dword ptr [esi+1ch]
        lea edi, [edi+eax*2]
        lea ebx, [ebx+eax*4]
        mov edx, 0
        mov ecx, dword ptr [esi+58h]
    blit_loop:
        cmp ecx, dword ptr [ebx]
        ja blit_step
        mov eax, dword ptr [esi+14h]
        mov word ptr [edi], ax
        mov dword ptr [ebx], ecx
    blit_step:
        add edi, dword ptr [esi+0f0h]
        add ebx, dword ptr [esi+0fch]
        add edx, dword ptr [esi+0f8h]
        jae blit_no_carry
        add edi, dword ptr [esi+0f4h]
        add ebx, dword ptr [esi+100h]
    blit_no_carry:
        mov ecx, dword ptr [esi+58h]
        add ecx, dword ptr [esi+60h]
        mov dword ptr [esi+58h], ecx
        dec dword ptr [esi+0ech]
        jg blit_loop
        pop edi
        pop esi
        pop ebx
        pop ebp
        ret
    }
}

/*
 * Empty. A blitter slot the shipped build fills with a function that does nothing.
 */
// FUNCTION: FF7 0x006be40b
void __cdecl Gfx_RasterLineSpanFlatNoOp(void)
{
    /* The original is empty. Its whole body is
       `55 8b ec 5d c3` -- 5 bytes at 0x006be40b,
       push/mov/pop/ret plus alignment padding.
       The C body is empty, as in retail. */
}

/*
 * Original naked palette-compositing 16-bit span blitter. Computes color/depth row
 * pointers and performs an unsigned depth test for each sample.
 */
// FUNCTION: FF7 0x006be410
__declspec(naked) void __cdecl Gfx_RasterLineSpanFlatBlendRemap(void *span)
{
    __asm {
        push ebp
        mov ebp, esp
        push ebx
        push esi
        push edi
        mov esi, dword ptr [ebp+8]
        mov eax, dword ptr [esi+0ch]
        mov edx, dword ptr [esi+4]
        imul edx
        mov edi, dword ptr [esi+8]
        add edi, eax
        mov eax, dword ptr [esi+0ch]
        mov edx, dword ptr [esi+3ch]
        imul edx
        mov ebx, dword ptr [esi+40h]
        add ebx, eax
        mov dword ptr [esi+64h], ebx
        mov ebx, dword ptr [esi+64h]
        mov eax, dword ptr [esi+1ch]
        lea edi, [edi+eax*2]
        lea ebx, [ebx+eax*4]
        mov edx, 0
        mov ecx, dword ptr [esi+58h]
    pal_loop:
        cmp ecx, dword ptr [ebx]
        ja pal_step
        mov eax, dword ptr [esi+14h]
        push edx
        mov ecx, dword ptr [esi+13ch]
        xor edx, edx
        mov dx, word ptr [edi]
        mov edx, dword ptr [ecx+edx*4]
        mov eax, dword ptr [ecx+eax*4]
        add edx, eax
        mov ecx, dword ptr [esi+140h]
        mov eax, dword ptr [ecx+edx*4]
        mov word ptr [edi], ax
        pop edx
    pal_step:
        add edi, dword ptr [esi+0f0h]
        add ebx, dword ptr [esi+0fch]
        add edx, dword ptr [esi+0f8h]
        jae pal_no_carry
        add edi, dword ptr [esi+0f4h]
        add ebx, dword ptr [esi+100h]
    pal_no_carry:
        mov ecx, dword ptr [esi+58h]
        add ecx, dword ptr [esi+60h]
        mov dword ptr [esi+58h], ecx
        dec dword ptr [esi+0ech]
        jg pal_loop
        pop edi
        pop esi
        pop ebx
        pop ebp
        ret
    }
}

/*
 * Original naked assembly three-channel raster compositor. Derives color and depth
 * pointers from row and column, then compares interpolated depth against stored
 * depth as unsigned values.
 */
// FUNCTION: FF7 0x006be4a8
__declspec(naked) void __cdecl Gfx_RasterLineSpanGouraudDepthWrite(void *span)
{
    __asm {
        push ebp
        mov ebp, esp
        push ebx
        push esi
        push edi
        mov esi, dword ptr [ebp+8]
        mov eax, dword ptr [esi+0ch]
        mov edx, dword ptr [esi+4]
        imul edx
        mov edi, dword ptr [esi+8]
        add edi, eax
        mov eax, dword ptr [esi+0ch]
        mov edx, dword ptr [esi+3ch]
        imul edx
        mov ebx, dword ptr [esi+40h]
        add ebx, eax
        mov dword ptr [esi+64h], ebx
        mov ebx, dword ptr [esi+64h]
        mov eax, dword ptr [esi+1ch]
        lea edi, [edi+eax*2]
        lea ebx, [ebx+eax*4]
        mov edx, 0
        mov ecx, dword ptr [esi+58h]
    rgb_loop:
        cmp ecx, dword ptr [ebx]
        ja rgb_step
        mov dword ptr [ebx], ecx
        push ebx
        push edx
        mov eax, dword ptr [esi+90h]
        mov ebx, dword ptr [esi+124h]
        mov ecx, dword ptr [esi+0a0h]
        mov edx, dword ptr [esi+128h]
        mov eax, dword ptr [ebx+eax*4]
        or eax, dword ptr [edx+ecx*4]
        mov ecx, dword ptr [esi+0b0h]
        mov edx, dword ptr [esi+12ch]
        or eax, dword ptr [edx+ecx*4]
        pop edx
        pop ebx
        mov word ptr [edi], ax
    rgb_step:
        add edi, dword ptr [esi+0f0h]
        add ebx, dword ptr [esi+0fch]
        add edx, dword ptr [esi+0f8h]
        jae rgb_no_carry
        add edi, dword ptr [esi+0f4h]
        add ebx, dword ptr [esi+100h]
    rgb_no_carry:
        push edx
        mov ecx, dword ptr [esi+8ch]
        mov edx, dword ptr [esi+90h]
        add ecx, dword ptr [esi+94h]
        adc edx, dword ptr [esi+98h]
        mov dword ptr [esi+8ch], ecx
        mov dword ptr [esi+90h], edx
        mov ecx, dword ptr [esi+9ch]
        mov edx, dword ptr [esi+0a0h]
        add ecx, dword ptr [esi+0a4h]
        adc edx, dword ptr [esi+0a8h]
        mov dword ptr [esi+9ch], ecx
        mov dword ptr [esi+0a0h], edx
        mov ecx, dword ptr [esi+0ach]
        mov edx, dword ptr [esi+0b0h]
        add ecx, dword ptr [esi+0b4h]
        adc edx, dword ptr [esi+0b8h]
        mov dword ptr [esi+0ach], ecx
        mov dword ptr [esi+0b0h], edx
        pop edx
        mov ecx, dword ptr [esi+58h]
        add ecx, dword ptr [esi+60h]
        mov dword ptr [esi+58h], ecx
        dec dword ptr [esi+0ech]
        jg rgb_loop
        pop edi
        pop esi
        pop ebx
        pop ebp
        ret
    }
}

/*
 * Empty. A blitter slot the shipped build fills with a function that does nothing.
 */
// FUNCTION: FF7 0x006be5c4
void __cdecl Gfx_RasterLineSpanGouraudNoOp(void)
{
    /* The original is empty. Its whole body is
       `55 8b ec 5d c3` -- 5 bytes at 0x006be5c4,
       push/mov/pop/ret plus alignment padding.
       The C body is empty, as in retail. */
}

/*
 * Original naked assembly three-channel depth-tested blend compositor: for interpolated depth no greater
 * than the stored depth (unsigned), it combines three channel-table lookups with the destination pixel
 * through the blend remap tables and writes the low word.
 */
// FUNCTION: FF7 0x006be5c9
__declspec(naked) void __cdecl Gfx_RasterLineSpanGouraudBlendRemap(void *span)
{
    __asm {
        push ebp
        mov ebp, esp
        push ebx
        push esi
        push edi
        mov esi, dword ptr [ebp+8]
        mov eax, dword ptr [esi+0ch]
        mov edx, dword ptr [esi+4]
        imul edx
        mov edi, dword ptr [esi+8]
        add edi, eax
        mov eax, dword ptr [esi+0ch]
        mov edx, dword ptr [esi+3ch]
        imul edx
        mov ebx, dword ptr [esi+40h]
        add ebx, eax
        mov dword ptr [esi+64h], ebx
        mov ebx, dword ptr [esi+64h]
        mov eax, dword ptr [esi+1ch]
        lea edi, [edi+eax*2]
        lea ebx, [ebx+eax*4]
        mov edx, 0
        mov ecx, dword ptr [esi+58h]
    blend_rgb_loop:
        cmp ecx, dword ptr [ebx]
        ja blend_rgb_step
        push ebx
        push edx
        mov eax, dword ptr [esi+90h]
        mov ebx, dword ptr [esi+124h]
        mov ecx, dword ptr [esi+0a0h]
        mov edx, dword ptr [esi+128h]
        mov eax, dword ptr [ebx+eax*4]
        or eax, dword ptr [edx+ecx*4]
        mov ecx, dword ptr [esi+0b0h]
        mov edx, dword ptr [esi+12ch]
        or eax, dword ptr [edx+ecx*4]
        mov ecx, dword ptr [esi+13ch]
        xor edx, edx
        mov dx, word ptr [edi]
        mov edx, dword ptr [ecx+edx*4]
        mov eax, dword ptr [ecx+eax*4]
        add edx, eax
        mov ecx, dword ptr [esi+140h]
        mov eax, dword ptr [ecx+edx*4]
        pop edx
        pop ebx
        mov word ptr [edi], ax
    blend_rgb_step:
        add edi, dword ptr [esi+0f0h]
        add ebx, dword ptr [esi+0fch]
        add edx, dword ptr [esi+0f8h]
        jae blend_rgb_no_carry
        add edi, dword ptr [esi+0f4h]
        add ebx, dword ptr [esi+100h]
    blend_rgb_no_carry:
        push edx
        mov ecx, dword ptr [esi+8ch]
        mov edx, dword ptr [esi+90h]
        add ecx, dword ptr [esi+94h]
        adc edx, dword ptr [esi+98h]
        mov dword ptr [esi+8ch], ecx
        mov dword ptr [esi+90h], edx
        mov ecx, dword ptr [esi+9ch]
        mov edx, dword ptr [esi+0a0h]
        add ecx, dword ptr [esi+0a4h]
        adc edx, dword ptr [esi+0a8h]
        mov dword ptr [esi+9ch], ecx
        mov dword ptr [esi+0a0h], edx
        mov ecx, dword ptr [esi+0ach]
        mov edx, dword ptr [esi+0b0h]
        add ecx, dword ptr [esi+0b4h]
        adc edx, dword ptr [esi+0b8h]
        mov dword ptr [esi+0ach], ecx
        mov dword ptr [esi+0b0h], edx
        pop edx
        mov ecx, dword ptr [esi+58h]
        add ecx, dword ptr [esi+60h]
        mov dword ptr [esi+58h], ecx
        dec dword ptr [esi+0ech]
        jg blend_rgb_loop
        pop edi
        pop esi
        pop ebx
        pop ebp
        ret
    }
}

/*
 * Sets up an inclusive software line scan from the two endpoints: maps the first
 * packed color, selects horizontal, vertical, diagonal or major-axis steps, and
 * stores the endpoint count and fixed-point error increment in the shared scan
 * record. Uses separate pixel and secondary-buffer strides, except that the
 * diagonal branch adds the pixel row stride to both steps.
 */
// FUNCTION: FF7 0x006be6ff
void __cdecl Gfx_RasterDrawLineBetweenVertices(GlRasterVertex *startVertex, GfxLineVertex *endVertex, SwLineOps *ops, GfxRasterContext *rasterContext)
{
    int isXMajor;
    int heightDiff;
    int color;
    float srcSlopeRatio;
    float recipDy;
    Int64Pair fixedDelta;
    float stepW;
    float vStepValue;
    float stepDepthY;
    float yStepU;
    int dyAbs;
    int absDx;
    int tmpDeltaX;
    float nextInvAbsDx;
    GlLineBuffer *lineBuffer;
    GfxRasterLineContext *lineContext;
    float depthDelta;
    float deltaU;
    float stepVY;
    float stepWY;

    lineContext = (GfxRasterLineContext *)(rasterContext->lineContext);
    lineBuffer = (GlLineBuffer *)((int)lineContext + 0xEC);
    lineContext->startY = startVertex->posY;
    lineContext->startX = startVertex->posX;
    color = Blit_MapArgb8888ViaRampTables(*(GfxArgb32 *)&startVertex->color, GetEngineContext());
    tmpDeltaX = endVertex->screenX - startVertex->posX;
    if (tmpDeltaX < 0) {
        absDx = -tmpDeltaX;
        nextInvAbsDx = -((SwRasterData *)rasterContext)->intToFloat[absDx];
    } else {
        absDx = tmpDeltaX;
        nextInvAbsDx = ((SwRasterData *)rasterContext)->intToFloat[absDx];
    }
    heightDiff = endVertex->screenY - startVertex->posY;
    if (heightDiff < 0) {
        dyAbs = -heightDiff;
        recipDy = ((SwRasterData *)rasterContext)->intToFloat[dyAbs];
    } else {
        dyAbs = heightDiff;
        recipDy = ((SwRasterData *)rasterContext)->intToFloat[dyAbs];
    }
    ((GfxRasterLineContext *)rasterContext->lineContext)->color = color;
    if (tmpDeltaX == 0) {
        if (heightDiff == 0) {
            lineBuffer->deltaA = 0;
            lineBuffer->deltaB = 0;
            lineBuffer->fixedDelta = 0;
            lineBuffer->count = 1;
            lineBuffer->stepA = 0;
            lineBuffer->stepB = 0;
            isXMajor = 1;
        } else {
            if (heightDiff < 0) {
                lineBuffer->deltaA = -lineContext->deltaX;
                lineBuffer->stepA = -lineContext->deltaY;
            } else {
                lineBuffer->deltaA = lineContext->deltaX;
                lineBuffer->stepA = lineContext->deltaY;
            }
            lineBuffer->deltaB = 0;
            lineBuffer->fixedDelta = 0;
            lineBuffer->count = dyAbs + 1;
            lineBuffer->stepB = 0;
            isXMajor = 0;
        }
    } else if (heightDiff == 0) {
        if (tmpDeltaX < 0) {
            lineBuffer->deltaA = 0xFFFFFFFE;
            lineBuffer->stepA = 0xFFFFFFFC;
        } else {
            lineBuffer->deltaA = 2;
            lineBuffer->stepA = 4;
        }
        lineBuffer->deltaB = 0;
        lineBuffer->fixedDelta = 0;
        lineBuffer->count = absDx + 1;
        lineBuffer->stepB = 0;
        isXMajor = 1;
    } else if (absDx == dyAbs) {
        if (tmpDeltaX < 0) {
            lineBuffer->deltaA = 0xFFFFFFFE;
            lineBuffer->stepA = 0xFFFFFFFC;
        } else {
            lineBuffer->deltaA = 2;
            lineBuffer->stepA = 4;
        }
        if (heightDiff < 0) {
            lineBuffer->deltaA += -lineContext->deltaX;
            lineBuffer->stepA += -lineContext->deltaX;
        } else {
            lineBuffer->deltaA += lineContext->deltaX;
            lineBuffer->stepA += lineContext->deltaX;
        }
        lineBuffer->deltaB = 0;
        lineBuffer->fixedDelta = 0;
        lineBuffer->count = absDx + 1;
        lineBuffer->stepB = 0;
        isXMajor = 1;
    } else if (absDx > dyAbs) {
        srcSlopeRatio = recipDy / nextInvAbsDx;
        if (tmpDeltaX < 0) {
            lineBuffer->deltaA = 0xFFFFFFFE;
            lineBuffer->stepA = 0xFFFFFFFC;
        } else {
            lineBuffer->deltaA = 2;
            lineBuffer->stepA = 4;
        }
        if (heightDiff < 0) {
            lineBuffer->deltaB = -lineContext->deltaX;
            lineBuffer->stepB = -lineContext->deltaY;
        } else {
            lineBuffer->deltaB = lineContext->deltaX;
            lineBuffer->stepB = lineContext->deltaY;
        }
        crt_FloatToInt64(srcSlopeRatio, (int *)&fixedDelta);
        if (fixedDelta.hi >= 0) {
            lineBuffer->fixedDelta = fixedDelta.lo;
        } else {
            Gfx_RasterNegateFixed64((unsigned int *)((int *)&fixedDelta));
            lineBuffer->fixedDelta = fixedDelta.lo;
        }
        lineBuffer->count = absDx + 1;
        isXMajor = 1;
    } else {
        srcSlopeRatio = nextInvAbsDx / recipDy;
        if (heightDiff < 0) {
            lineBuffer->deltaA = -lineContext->deltaX;
            lineBuffer->stepA = -lineContext->deltaY;
        } else {
            lineBuffer->deltaA = lineContext->deltaX;
            lineBuffer->stepA = lineContext->deltaY;
        }
        if (tmpDeltaX < 0) {
            lineBuffer->deltaB = 0xFFFFFFFE;
            lineBuffer->stepB = 0xFFFFFFFC;
        } else {
            lineBuffer->deltaB = 2;
            lineBuffer->stepB = 4;
        }
        crt_FloatToInt64(srcSlopeRatio, (int *)&fixedDelta);
        if (fixedDelta.hi >= 0) {
            lineBuffer->fixedDelta = fixedDelta.lo;
        } else {
            Gfx_RasterNegateFixed64((unsigned int *)((int *)&fixedDelta));
            lineBuffer->fixedDelta = fixedDelta.lo;
        }
        lineBuffer->count = dyAbs + 1;
        isXMajor = 0;
    }
    if (isXMajor != 0) {
        if (nextInvAbsDx != 0.0f) {
            if (nextInvAbsDx > 0.0f) {
                depthDelta = (endVertex->depth - startVertex->depth) / nextInvAbsDx;
            } else {
                depthDelta = (startVertex->depth - endVertex->depth) / nextInvAbsDx;
            }
        } else {
            depthDelta = 0.0f;
        }
        crt_FloatToInt64(depthDelta, (void *)((int)lineContext + 0x5C));
        switch (ops->mode) {
        case SW_LINE_INTERP_DEPTH:
            break;
        case SW_LINE_INTERP_DEPTH_UVW:
            if (nextInvAbsDx != 0.0f) {
                if (nextInvAbsDx > 0.0f) {
                    deltaU = (endVertex->u - startVertex->texU) / nextInvAbsDx;
                    vStepValue = (endVertex->v - startVertex->texV) / nextInvAbsDx;
                    stepW = (endVertex->w - startVertex->w) / nextInvAbsDx;
                } else {
                    deltaU = (startVertex->texU - endVertex->u) / nextInvAbsDx;
                    vStepValue = (startVertex->texV - endVertex->v) / nextInvAbsDx;
                    stepW = (startVertex->w - endVertex->w) / nextInvAbsDx;
                }
            } else {
                deltaU = 0.0f;
                vStepValue = 0.0f;
                stepW = 0.0f;
            }
            crt_FloatToInt64(deltaU, (void *)((int)lineContext + 0x94));
            crt_FloatToInt64(vStepValue, (void *)((int)lineContext + 0xA4));
            crt_FloatToInt64(stepW, (void *)((int)lineContext + 0xB4));
            crt_FloatToInt64(startVertex->texU, (void *)((int)lineContext + 0x8C));
            crt_FloatToInt64(startVertex->texV, (void *)((int)lineContext + 0x9C));
            crt_FloatToInt64(startVertex->w, (void *)((int)lineContext + 0xAC));
        }
        crt_FloatToInt64(startVertex->depth, (void *)((int)lineContext + 0x54));
        lineContext->accumulator += lineContext->accumulatorStep;
        ops->drawCallback((int)lineContext);
    } else {
        if (recipDy != 0.0f) {
            stepDepthY = (endVertex->depth - startVertex->depth) / recipDy;
        } else {
            stepDepthY = 0.0f;
        }
        crt_FloatToInt64(stepDepthY, (void *)((int)lineContext + 0x5C));
        switch (ops->mode) {
        case SW_LINE_INTERP_DEPTH:
            break;
        case SW_LINE_INTERP_DEPTH_UVW:
            if (recipDy != 0.0f) {
                yStepU = (endVertex->u - startVertex->texU) / recipDy;
                stepVY = (endVertex->v - startVertex->texV) / recipDy;
                stepWY = (endVertex->w - startVertex->w) / recipDy;
            } else {
                yStepU = 0.0f;
                stepVY = 0.0f;
                stepWY = 0.0f;
            }
            crt_FloatToInt64(yStepU, (void *)((int)lineContext + 0x94));
            crt_FloatToInt64(stepVY, (void *)((int)lineContext + 0xA4));
            crt_FloatToInt64(stepWY, (void *)((int)lineContext + 0xB4));
            crt_FloatToInt64(startVertex->texU, (void *)((int)lineContext + 0x8C));
            crt_FloatToInt64(startVertex->texV, (void *)((int)lineContext + 0x9C));
            crt_FloatToInt64(startVertex->w, (void *)((int)lineContext + 0xAC));
        }
        crt_FloatToInt64(startVertex->depth, (void *)((int)lineContext + 0x54));
        lineContext->accumulator += lineContext->accumulatorStep;
        ops->drawCallback((int)lineContext);
    }
}

/*
 * Copies the endpoints into local vertex records, runs the endpoint callback on each endpoint whose
 * clipFlags are clear, and draws the line between them with Gfx_RasterDrawLineBetweenVertices.
 */
// FUNCTION: FF7 0x006beee9
void __cdecl Gfx_RasterDrawClippedLineEndpoints(SwClippedLine *clipLine, GfxRasterLineEndpointA *endpointA, GfxRasterLineEndpointA *endpointB, SwLineOps *ops, GfxRasterContext *context)
{
    int padSlot[2]; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    GfxLineVertex vertEnd;
    int padEntry[2]; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int lineRec;
    GlRasterVertex vertexBufferBegin;

    lineRec = context->lineContext;
    vertexBufferBegin.posX = clipLine->xStart;
    vertexBufferBegin.posY = clipLine->y0;
    vertEnd.screenX = clipLine->xLast;
    vertEnd.screenY = clipLine->clipY1;
    if (clipLine->dyValue < 0) {
        if (endpointA->needsClipping != 0) { /* empty first arm: lib/codegen.h */
        } else {
            ops->endpointCallback((int)endpointA, (int)context);
        }
        if (endpointB->needsClipping != 0) { /* empty first arm: lib/codegen.h */
        } else {
            ops->endpointCallback((int)endpointB, (int)context);
        }
        vertexBufferBegin.color = endpointB->sample;
        vertEnd.color = endpointA->sample;
        vertexBufferBegin.depthBits = endpointB->depth;
        vertEnd.depthBits = endpointA->depth;
        vertexBufferBegin.texUBits = endpointB->attrU;
        vertexBufferBegin.texVBits = endpointB->attrV;
        vertexBufferBegin.wBits = endpointB->attrWidth;
        vertEnd.uBits = endpointA->attrU;
        vertEnd.vBits = endpointA->attrV;
        vertEnd.wBits = endpointA->attrWidth;
    } else {
        if (endpointA->needsClipping != 0) { /* empty first arm: lib/codegen.h */
        } else {
            ops->endpointCallback((int)endpointA, (int)context);
        }
        if (endpointB->needsClipping != 0) { /* empty first arm: lib/codegen.h */
        } else {
            ops->endpointCallback((int)endpointB, (int)context);
        }
        vertexBufferBegin.color = endpointA->sample;
        vertEnd.color = endpointB->sample;
        vertexBufferBegin.depthBits = endpointA->depth;
        vertEnd.depthBits = endpointB->depth;
        vertexBufferBegin.texUBits = endpointA->attrU;
        vertexBufferBegin.texVBits = endpointA->attrV;
        vertexBufferBegin.wBits = endpointA->attrWidth;
        vertEnd.uBits = endpointB->attrU;
        vertEnd.vBits = endpointB->attrV;
        vertEnd.wBits = endpointB->attrWidth;
    }
    Gfx_RasterDrawLineBetweenVertices(&vertexBufferBegin, &vertEnd, ops, context);
}

// Gets the engine context and rejects the line when the endpoints' clipFlags share any bit, or when
// their union contains 0x130. Otherwise it passes the endpoints' x and y to
// Gfx_RasterClipLineToViewportBounds with local output storage, and draws the clipped endpoints if
// the clip result is zero. The unused8 array reserves frame slots that the original has and is
// never read.
// FUNCTION: FF7 0x006bf086
void __cdecl Gfx_RasterDrawLineIfNotRejected(GfxRasterLineEndpointA *endpointA, GfxRasterLineEndpointA *endpointB, SwLineOps *ops, GfxRasterContext *rasterContext)
{
    int sharedFlags;
    int clipResult[10];
    int combinedFlags;
    GfxEngineContext *engineCtx;

    engineCtx = GetEngineContext();
    combinedFlags = endpointA->clipFlags | endpointB->clipFlags;
    sharedFlags = endpointA->clipFlags & endpointB->clipFlags;
    if (sharedFlags != 0) {
        return;
    }
    if ((combinedFlags & 0x130) != 0) {
        return;
    }
    Gfx_RasterClipLineToViewportBounds(endpointA->x, endpointA->y, endpointB->x, endpointB->y, (GfxClippedLine *)clipResult, engineCtx);
    if (clipResult[0] == 0) {
        Gfx_RasterDrawClippedLineEndpoints((SwClippedLine *)clipResult, endpointA, endpointB, ops, rasterContext);
    }
}

/*
 * Prepares the line vertices through Gfx_RasterPrepareVertexRecords, then draws each indexed line unless
 * the prepared vertex buffer's clipAnd is nonzero (all vertices clipped).
 */
// FUNCTION: FF7 0x006bf11a
void __cdecl Gfx_RasterDrawIndexedLineList(int vertexBase, int indices, int lineCount, SwLineVertexRecord *vertexCount, GfxEngineContext *engineContext)
{
    GfxRasterContext *rasterCtx;
    GfxRasterVertexBuffer *pIndexedState;
    int engineBits;
    int startIndex;
    int vertexPool;
    SwLineOps *lineOps;
    int endIndex;
    GfxRasterSetupBlock *lineStatus;
    int remaining;
    GfxRasterLineEndpointA *vertexB;
    GfxRasterLineEndpointA *endpointAPtr;
    SwRasterizerState *rasterBase;

    rasterBase = engineContext->rasterBase;
    rasterCtx = (GfxRasterContext *)((int)rasterBase + 0x38);
    pIndexedState = (GfxRasterVertexBuffer *)rasterBase->rasterVertexBuffer;
    Gfx_RasterPrepareVertexRecords(vertexBase, vertexCount, (struct GfxRasterVertexBuffer *)pIndexedState, rasterCtx);
    if (pIndexedState->clipAnd != 0) {
        return;
    }
    lineOps = (SwLineOps *)((int)rasterCtx + 0x154);
    vertexPool = pIndexedState->alignedVertices;
    engineBits = engineContext->presetPolygonSets;
    lineStatus = rasterBase->renderSetupBlock;
    lineStatus->depthOffset = pIndexedState->depthOffset;
    for (remaining = lineCount; remaining > 0; remaining--) {
        startIndex = *(unsigned short *)indices;
        indices += 2;
        endpointAPtr = (GfxRasterLineEndpointA *)(vertexPool + startIndex * 0x60);
        endIndex = *(unsigned short *)indices;
        indices += 2;
        vertexB = (GfxRasterLineEndpointA *)(vertexPool + endIndex * 0x60);
        Gfx_RasterDrawLineIfNotRejected(endpointAPtr, vertexB, lineOps, rasterCtx);
    }
}
} /* extern "C" */
