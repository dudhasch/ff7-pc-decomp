// src/lib/src/graphics/graphics_raster_line_pairs.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Hand-written paired 16-bit line rasterizers and a vertical line helper, plus the 16-bit
//   line entry point that prepares and calls them.
/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/types/graphics.h"
#include "lib/api/graphics.h"
extern "C" {

// clang-format off
// GLOBAL: FF7 0x00db2d28
int g_GfxRasterLineErrorDelta = 0;
// GLOBAL: FF7 0x00db2d2c
int g_GfxRasterLineErrorStep = 0;
// GLOBAL: FF7 0x00db2d30
int g_GfxRasterLineErrorThreshold = 0;
// GLOBAL: FF7 0x00db2d34
int g_GfxRasterLineRemainderCount = 0;
// GLOBAL: FF7 0x00db2d38
int g_GfxRasterLineQuadLoopCount = 0;
// clang-format on
void __cdecl Gfx_RasterDrawLine16PairsBelowHalfSlope(int error, int diagonal, int straight, int color, int begin, int end);
void __cdecl Gfx_RasterDrawLine16PairsHalfSlopeOrMore(int error, int diagonal, int straight, int color, int begin, int end);
void __cdecl Gfx_RasterDrawVerticalLine16(int count, int stride, int value, int dst);

/*
 * Original hand-written paired line rasterizer: writes the low color word to both
 * endpoints before testing the global pair count, then advances begin forward and
 * end backward through pairs of pixels. Uses signed error comparisons against zero
 * and g_GfxRasterLineErrorThreshold to select straight/diagonal step combinations
 * and updates the error by g_GfxRasterLineErrorDelta or g_GfxRasterLineErrorStep.
 */
// FUNCTION: FF7 0x006aaae0
__declspec(naked) void __cdecl Gfx_RasterDrawLine16PairsBelowHalfSlope(int error, int diagonal, int straight, int color, int begin, int end)
{
    __asm {
        push ebp
        mov ebp, esp
        push ebx
        push esi
        push edi
        push ebp
        mov edx, dword ptr [ebp + 8]
        mov ecx, dword ptr [ebp + 0ch]
        mov ebx, dword ptr [ebp + 010h]
        mov eax, dword ptr [ebp + 014h]
        mov esi, dword ptr [ebp + 018h]
        mov edi, dword ptr [ebp + 01ch]
        mov ebp, dword ptr [g_GfxRasterLineQuadLoopCount]
        mov word ptr [esi], ax
        mov word ptr [edi], ax
        or ebp, ebp
        je rem_entry_lo
quad_loop_lo:
        add esi, ebx
        sub edi, ebx
        or edx, edx
        jge quad_nonneg_lo
        mov word ptr [esi], ax
        add esi, ebx
        mov word ptr [esi], ax
        mov word ptr [edi], ax
        sub edi, ebx
        mov word ptr [edi], ax
        add edx, dword ptr [g_GfxRasterLineErrorDelta]
        dec ebp
        jne quad_loop_lo
        jmp rem_entry_lo
quad_nonneg_lo:
        cmp edx, dword ptr [g_GfxRasterLineErrorThreshold]
        jge quad_diag_lo
        mov word ptr [esi], ax
        add esi, ebx
        add esi, ecx
        mov word ptr [esi], ax
        mov word ptr [edi], ax
        sub edi, ebx
        sub edi, ecx
        mov word ptr [edi], ax
        add edx, dword ptr [g_GfxRasterLineErrorStep]
        dec ebp
        jne quad_loop_lo
        jmp rem_entry_lo
quad_diag_lo:
        add esi, ecx
        mov word ptr [esi], ax
        add esi, ebx
        mov word ptr [esi], ax
        sub edi, ecx
        mov word ptr [edi], ax
        sub edi, ebx
        mov word ptr [edi], ax
        add edx, dword ptr [g_GfxRasterLineErrorStep]
        dec ebp
        jne quad_loop_lo
rem_entry_lo:
        mov ebp, dword ptr [g_GfxRasterLineRemainderCount]
        or ebp, ebp
        je done_lo
        or edx, edx
        jge rem_nonneg_lo
        add esi, ebx
        mov word ptr [esi], ax
        cmp ebp, 1
        jle done_lo
        add esi, ebx
        mov word ptr [esi], ax
        cmp ebp, 2
        jle done_lo
        sub edi, ebx
        mov word ptr [edi], ax
        jmp done_lo
rem_nonneg_lo:
        cmp edx, dword ptr [g_GfxRasterLineErrorThreshold]
        jge rem_diag_lo
        add esi, ebx
        mov word ptr [esi], ax
        cmp ebp, 1
        jle done_lo
        add esi, ebx
        add esi, ecx
        mov word ptr [esi], ax
        cmp ebp, 2
        jle done_lo
        sub edi, ebx
        mov word ptr [edi], ax
        jmp done_lo
rem_diag_lo:
        add esi, ebx
        add esi, ecx
        mov word ptr [esi], ax
        cmp ebp, 1
        jle done_lo
        add esi, ebx
        mov word ptr [esi], ax
        cmp ebp, 2
        jle done_lo
        sub edi, ebx
        sub edi, ecx
        mov word ptr [edi], ax
done_lo:
        pop ebp
        pop edi
        pop esi
        pop ebx
        pop ebp
        ret
    }
}

/*
 * Original hand-written paired line rasterizer: stores the low color word at both
 * endpoints first, then consumes the global pair count while moving begin forward
 * and end backward. Signed error greater than zero selects two diagonal-inclusive
 * steps per side and g_GfxRasterLineErrorDelta adjustment; other errors select the
 * retained straight/diagonal combinations according to
 * g_GfxRasterLineErrorThreshold and adjust by g_GfxRasterLineErrorStep.
 */
// FUNCTION: FF7 0x006aabe1
__declspec(naked) void __cdecl Gfx_RasterDrawLine16PairsHalfSlopeOrMore(int error, int diagonal, int straight, int color, int begin, int end)
{
    __asm {
        push ebp
        mov ebp, esp
        push ebx
        push esi
        push edi
        push ebp
        mov edx, dword ptr [ebp + 8]
        mov ecx, dword ptr [ebp + 0ch]
        mov ebx, dword ptr [ebp + 010h]
        mov eax, dword ptr [ebp + 014h]
        mov esi, dword ptr [ebp + 018h]
        mov edi, dword ptr [ebp + 01ch]
        mov ebp, dword ptr [g_GfxRasterLineQuadLoopCount]
        mov word ptr [esi], ax
        mov word ptr [edi], ax
        or ebp, ebp
        je rem_entry_hi
quad_loop_hi:
        add esi, ebx
        sub edi, ebx
        or edx, edx
        jle quad_nonpos_hi
        add esi, ecx
        mov word ptr [esi], ax
        add esi, ebx
        add esi, ecx
        mov word ptr [esi], ax
        sub edi, ecx
        mov word ptr [edi], ax
        sub edi, ebx
        sub edi, ecx
        mov word ptr [edi], ax
        add edx, dword ptr [g_GfxRasterLineErrorDelta]
        dec ebp
        jne quad_loop_hi
        jmp rem_entry_hi
quad_nonpos_hi:
        cmp edx, dword ptr [g_GfxRasterLineErrorThreshold]
        jge quad_diag_hi
        mov word ptr [esi], ax
        add esi, ebx
        add esi, ecx
        mov word ptr [esi], ax
        mov word ptr [edi], ax
        sub edi, ebx
        sub edi, ecx
        mov word ptr [edi], ax
        add edx, dword ptr [g_GfxRasterLineErrorStep]
        dec ebp
        jne quad_loop_hi
        jmp rem_entry_hi
quad_diag_hi:
        add esi, ecx
        mov word ptr [esi], ax
        add esi, ebx
        mov word ptr [esi], ax
        sub edi, ecx
        mov word ptr [edi], ax
        sub edi, ebx
        mov word ptr [edi], ax
        add edx, dword ptr [g_GfxRasterLineErrorStep]
        dec ebp
        jne quad_loop_hi
rem_entry_hi:
        mov ebp, dword ptr [g_GfxRasterLineRemainderCount]
        or ebp, ebp
        je done_hi
        or edx, edx
        jle rem_nonpos_hi
        add esi, ebx
        add esi, ecx
        mov word ptr [esi], ax
        cmp ebp, 1
        jle done_hi
        add esi, ebx
        add esi, ecx
        mov word ptr [esi], ax
        cmp ebp, 2
        jle done_hi
        sub edi, ebx
        sub edi, ecx
        mov word ptr [edi], ax
        jmp done_hi
rem_nonpos_hi:
        cmp edx, dword ptr [g_GfxRasterLineErrorThreshold]
        jge rem_diag_hi
        add esi, ebx
        mov word ptr [esi], ax
        cmp ebp, 1
        jle done_hi
        add esi, ebx
        add esi, ecx
        mov word ptr [esi], ax
        cmp ebp, 2
        jle done_hi
        sub edi, ebx
        mov word ptr [edi], ax
        jmp done_hi
rem_diag_hi:
        add esi, ebx
        add esi, ecx
        mov word ptr [esi], ax
        cmp ebp, 1
        jle done_hi
        add esi, ebx
        mov word ptr [esi], ax
        cmp ebp, 2
        jle done_hi
        sub edi, ebx
        cmp edx, dword ptr [g_GfxRasterLineErrorThreshold]
        jle rem_store_end_hi
        sub edi, ecx
rem_store_end_hi:
        mov word ptr [edi], ax
done_hi:
        pop ebp
        pop edi
        pop esi
        pop ebx
        pop ebp
        ret
    }
}

/*
 * Original naked assembly helper: stores the low 16 bits of value at dst, advances
 * by stride, and repeats while the decremented signed count is positive. The first
 * store occurs even when count is zero or negative; preserves the original
 * register-save sequence.
 */
// FUNCTION: FF7 0x006aacf8
__declspec(naked) void __cdecl Gfx_RasterDrawVerticalLine16(int count, int stride, int value, int dst)
{
    __asm {
        push ebp
        mov ebp, esp
        push ebx
        push esi
        push edi
        mov eax, dword ptr [ebp + 10h]
        mov edi, dword ptr [ebp + 14h]
        mov edx, dword ptr [ebp + 0ch]
        mov ecx, dword ptr [ebp + 8]
    loop_start:
        mov word ptr [edi], ax
        add edi, edx
        dec ecx
        jg loop_start
        pop edi
        pop esi
        pop ebx
        pop ebp
        ret
    }
}

/*
 * Prepares a 16-bit software line between x0/y0 and x1/y1 using pixel as
 * the stored pixel value and context for address/stride lookup. Selects stride 4096
 * when the nested context flag is set, otherwise the target format pitch of the context.
 */
// FUNCTION: FF7 0x006aad17
void __cdecl Gfx_RasterDrawLine16(int x0, int y0, int x1, int y1, int pixel, GfxEngineContext *context)
{
    int remainderCnt;
    int rightX;
    int dirY;
    int dirXPart;
    int leftYCoordinate;
    int xRemaining;
    int baseSignedStride;
    int minorLength;
    int majorLen;
    int signedRowStep;
    int iterRowPitch;
    int steep;
    int workInitialError;
    int errLimit;
    int horizStep;
    int rawQuadCount;
    int errStep;
    int dstSameDir;
    int scanSize;
    int swapTemp;
    int localSignedFactor;
    int strideSize;
    int signedTwo;
    int yDiffVal;
    int doubleThreshold;
    int rightY;

    if (context->rasterBase->usesAlignedHeap != 0) {
        scanSize = 4096;
    } else {
        scanSize = context->targetFormat.pitch;
    }
    horizStep = x1 - x0;
    if (horizStep < 0) {
        dirXPart = 0xFFFFFFFF;
        majorLen = -horizStep;
    } else {
        if (horizStep == 0) {
            yDiffVal = y1 - y0;
            if (yDiffVal < 0) {
                Gfx_RasterDrawVerticalLine16(-yDiffVal, scanSize, pixel, Gfx_PixelAddressFromContext(x0, y1, (int)context));
            } else {
                Gfx_RasterDrawVerticalLine16(yDiffVal, scanSize, pixel, Gfx_PixelAddressFromContext(x0, y0, (int)context));
            }
            return;
        }
        dirXPart = 1;
        majorLen = horizStep;
    }
    yDiffVal = y1 - y0;
    if (yDiffVal < 0) {
        dirY = 0xFFFFFFFF;
        minorLength = -yDiffVal;
    } else {
        dirY = 1;
        minorLength = yDiffVal;
    }
    if (dirXPart == dirY) {
        dstSameDir = 1;
    } else {
        dstSameDir = 0xFFFFFFFF;
    }
    if (minorLength > majorLen) {
        x0 ^= y0;
        y0 ^= x0;
        x0 ^= y0;
        x1 ^= y1;
        y1 ^= x1;
        x1 ^= y1;
        majorLen ^= minorLength;
        minorLength ^= majorLen;
        majorLen ^= minorLength;
        steep = 1;
    } else {
        steep = 0;
    }
    if (x0 > x1) {
        xRemaining = x1;
        leftYCoordinate = y1;
        rightX = x0;
        rightY = y0;
    } else {
        xRemaining = x0;
        leftYCoordinate = y0;
        rightX = x1;
        rightY = y1;
    }
    rawQuadCount = majorLen - 1 >> 2;
    remainderCnt = majorLen - 1 & 3;
    if (steep == 0) {
        errStep = (minorLength << 2) - (majorLen << 1);
        if (errStep < 0) {
            errLimit = minorLength << 1;
            doubleThreshold = errLimit << 1;
            workInitialError = doubleThreshold - majorLen;
            g_GfxRasterLineErrorDelta = doubleThreshold;
            g_GfxRasterLineErrorStep = errStep;
            g_GfxRasterLineErrorThreshold = errLimit;
            g_GfxRasterLineRemainderCount = remainderCnt;
            g_GfxRasterLineQuadLoopCount = rawQuadCount;
            baseSignedStride = scanSize;
            if (dstSameDir < 0) {
                baseSignedStride = -baseSignedStride;
            }
            Gfx_RasterDrawLine16PairsBelowHalfSlope(workInitialError, baseSignedStride, 2, pixel, Gfx_PixelAddressFromContext(xRemaining, leftYCoordinate, (int)context), Gfx_PixelAddressFromContext(rightX, rightY, (int)context));
        } else {
            errLimit = minorLength - majorLen << 1;
            doubleThreshold = errLimit << 1;
            workInitialError = doubleThreshold + majorLen;
            g_GfxRasterLineErrorDelta = doubleThreshold;
            g_GfxRasterLineErrorStep = errStep;
            g_GfxRasterLineErrorThreshold = errLimit;
            g_GfxRasterLineRemainderCount = remainderCnt;
            g_GfxRasterLineQuadLoopCount = rawQuadCount;
            signedRowStep = scanSize;
            if (dstSameDir < 0) {
                signedRowStep = -signedRowStep;
            }
            Gfx_RasterDrawLine16PairsHalfSlopeOrMore(workInitialError, signedRowStep, 2, pixel, Gfx_PixelAddressFromContext(xRemaining, leftYCoordinate, (int)context), Gfx_PixelAddressFromContext(rightX, rightY, (int)context));
        }
    } else {
        swapTemp = xRemaining;
        xRemaining = leftYCoordinate;
        leftYCoordinate = swapTemp;
        swapTemp = rightX;
        rightX = rightY;
        rightY = swapTemp;
        errStep = (minorLength << 2) - (majorLen << 1);
        if (errStep < 0) {
            errLimit = minorLength << 1;
            doubleThreshold = errLimit << 1;
            workInitialError = doubleThreshold - majorLen;
            g_GfxRasterLineErrorDelta = doubleThreshold;
            g_GfxRasterLineErrorStep = errStep;
            g_GfxRasterLineErrorThreshold = errLimit;
            g_GfxRasterLineRemainderCount = remainderCnt;
            g_GfxRasterLineQuadLoopCount = rawQuadCount;
            signedTwo = 2;
            if (dstSameDir < 0) {
                signedTwo = -signedTwo;
            }
            iterRowPitch = scanSize;
            Gfx_RasterDrawLine16PairsBelowHalfSlope(workInitialError, signedTwo, iterRowPitch, pixel, Gfx_PixelAddressFromContext(xRemaining, leftYCoordinate, (int)context), Gfx_PixelAddressFromContext(rightX, rightY, (int)context));
        } else {
            errLimit = minorLength - majorLen << 1;
            doubleThreshold = errLimit << 1;
            workInitialError = doubleThreshold + majorLen;
            g_GfxRasterLineErrorDelta = doubleThreshold;
            g_GfxRasterLineErrorStep = errStep;
            g_GfxRasterLineErrorThreshold = errLimit;
            g_GfxRasterLineRemainderCount = remainderCnt;
            g_GfxRasterLineQuadLoopCount = rawQuadCount;
            localSignedFactor = 2;
            if (dstSameDir < 0) {
                localSignedFactor = -localSignedFactor;
            }
            strideSize = scanSize;
            Gfx_RasterDrawLine16PairsHalfSlopeOrMore(workInitialError, localSignedFactor, strideSize, pixel, Gfx_PixelAddressFromContext(xRemaining, leftYCoordinate, (int)context), Gfx_PixelAddressFromContext(rightX, rightY, (int)context));
        }
    }
}
} /* extern "C" */
