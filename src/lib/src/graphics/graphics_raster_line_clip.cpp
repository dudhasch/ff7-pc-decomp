// src/lib/src/graphics/graphics_raster_line_clip.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Signed multiply-divide helper and integer line clipping against the viewport bounds for
//   the software rasterizer, which clips a line before drawing it.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/api/graphics.h"
#include "lib/types/graphics.h"
#include "lib/codegen.h"
extern "C" {
int __cdecl Gfx_RasterMulDivSigned(int arg1, int arg2, int arg3);

/*
 * Multiplies two signed integers, divides the full product by a third and returns
 * the quotient. Assembly routine returns the signed quotient of the full 64-bit
 * signed product of multiplicand and multiplier divided by divisor.
 */
// FUNCTION: FF7 0x006aa340
__declspec(naked) int __cdecl Gfx_RasterMulDivSigned(int multiplicand, int multiplier, int divisor)
{
    __asm {
        push ebp
        mov ebp, esp
        push ecx
        push ebx
        push esi
        push edi
        mov eax, dword ptr [ebp + 8]
        mov edx, dword ptr [ebp + 0ch]
        mov ecx, dword ptr [ebp + 10h]
        imul edx
        idiv ecx
        mov dword ptr [ebp - 4], eax
        mov eax, dword ptr [ebp - 4]
        pop edi
        pop esi
        pop ebx
        mov esp, ebp
        pop ebp
        ret
    }
}

// Clips an integer line to the inclusive clip bounds in clipCtx (clipLeft, clipRight, clipTop and
// clipBottom). It sets outLine->visible to one first, then stores the signed Y difference as
// dyValue and its magnitude as heightAbs. A nonhorizontal line has its endpoints swapped so that
// the start has the smaller Y. The X difference for that order, wDelta, is stored once the line's Y
// range overlaps the clip.
// FUNCTION: FF7 0x006aa361
void __cdecl Gfx_RasterClipLineToViewportBounds(int startX, int startY, int endX, int endY, GfxClippedLine *outLine, GfxEngineContext *clipCtx)
{
    int yOverEnd;
    int wDelta;
    int dyValue;
    int heightAbs;
    int xDelta;
    int stepOfY;
    int remainingExcess;
    int topExcess;
    int rightExcess;
    int swapTemp;

    outLine->visible = 1;
    if (UNFOLDED(0) != 0 && (startX > 0x10000 || (startX < -0x10000 || (endX > 0x10000 || (endX < -0x10000 || (startY > 0x10000 || (startY < -0x10000 || (endY > 0x10000 || endY < -0x10000)))))))) { /* empty first arm: lib/codegen.h */
    } else {
        dyValue = endY - startY;
        outLine->dyValue = dyValue;
        if (dyValue != 0) {
            if (dyValue < 0) {
                swapTemp = startX;
                startX = endX;
                endX = swapTemp;
                swapTemp = startY;
                startY = endY;
                endY = swapTemp;
                heightAbs = -dyValue;
            } else {
                heightAbs = dyValue;
            }
            outLine->heightAbs = heightAbs;
            if (startY <= clipCtx->clipBottom && endY >= clipCtx->clipTop) {
                wDelta = endX - startX;
                outLine->wDelta = wDelta;
                if (wDelta != 0) {
                    if (wDelta < 0) {
                        if (startX >= clipCtx->clipLeft && endX <= clipCtx->clipRight) {
                            topExcess = clipCtx->clipTop - startY;
                            if (topExcess > 0) {
                                startY = clipCtx->clipTop;
                                xDelta = Gfx_RasterMulDivSigned(topExcess, wDelta, heightAbs);
                                startX += xDelta;
                                if (startX < clipCtx->clipLeft) {
                                    return;
                                }
                            }
                            yOverEnd = endY - clipCtx->clipBottom;
                            if (yOverEnd > 0) {
                                endY = clipCtx->clipBottom;
                                xDelta = Gfx_RasterMulDivSigned(yOverEnd, wDelta, heightAbs);
                                endX -= xDelta;
                                if (endX > clipCtx->clipRight) {
                                    return;
                                }
                            }
                            remainingExcess = clipCtx->clipLeft - endX;
                            if (remainingExcess > 0) {
                                endX = clipCtx->clipLeft;
                                stepOfY = Gfx_RasterMulDivSigned(remainingExcess, heightAbs, wDelta);
                                endY += stepOfY;
                                if (endY < clipCtx->clipTop) {
                                    return;
                                }
                            }
                            rightExcess = startX - clipCtx->clipRight;
                            if (rightExcess > 0) {
                                startX = clipCtx->clipRight;
                                stepOfY = Gfx_RasterMulDivSigned(rightExcess, heightAbs, wDelta);
                                startY -= stepOfY;
                                if (startY > clipCtx->clipBottom) {
                                    return;
                                }
                            }
                            outLine->rowCount = endY - startY + 1;
                            outLine->x0 = startX;
                            outLine->y0 = startY;
                            outLine->x1 = endX;
                            outLine->y1 = endY;
                            outLine->visible = 0;
                        }
                    } else if (endX >= clipCtx->clipLeft && startX <= clipCtx->clipRight) {
                        topExcess = clipCtx->clipTop - startY;
                        if (topExcess > 0) {
                            startY = clipCtx->clipTop;
                            xDelta = Gfx_RasterMulDivSigned(topExcess, wDelta, heightAbs);
                            startX += xDelta;
                            if (startX > clipCtx->clipRight) {
                                return;
                            }
                        }
                        yOverEnd = endY - clipCtx->clipBottom;
                        if (yOverEnd > 0) {
                            endY = clipCtx->clipBottom;
                            xDelta = Gfx_RasterMulDivSigned(yOverEnd, wDelta, heightAbs);
                            endX -= xDelta;
                            if (endX < clipCtx->clipLeft) {
                                return;
                            }
                        }
                        remainingExcess = clipCtx->clipLeft - startX;
                        if (remainingExcess > 0) {
                            startX = clipCtx->clipLeft;
                            stepOfY = Gfx_RasterMulDivSigned(remainingExcess, heightAbs, wDelta);
                            startY += stepOfY;
                            if (startY > clipCtx->clipBottom) {
                                return;
                            }
                        }
                        rightExcess = endX - clipCtx->clipRight;
                        if (rightExcess > 0) {
                            endX = clipCtx->clipRight;
                            stepOfY = Gfx_RasterMulDivSigned(rightExcess, heightAbs, wDelta);
                            endY -= stepOfY;
                            if (endY < clipCtx->clipTop) {
                                return;
                            }
                        }
                        outLine->rowCount = endY - startY + 1;
                        outLine->x0 = startX;
                        outLine->y0 = startY;
                        outLine->x1 = endX;
                        outLine->y1 = endY;
                        outLine->visible = 0;
                    }
                } else if (startX >= clipCtx->clipLeft && endX <= clipCtx->clipRight && startY <= clipCtx->clipBottom && endY >= clipCtx->clipTop) {
                    topExcess = clipCtx->clipTop - startY;
                    if (topExcess > 0) {
                        startY = clipCtx->clipTop;
                    }
                    yOverEnd = endY - clipCtx->clipBottom;
                    if (yOverEnd > 0) {
                        endY = clipCtx->clipBottom;
                    }
                    outLine->rowCount = endY - startY + 1;
                    outLine->x0 = startX;
                    outLine->y0 = startY;
                    outLine->x1 = endX;
                    outLine->y1 = endY;
                    outLine->visible = 0;
                }
            }
        } else {
            outLine->heightAbs = 0;
            if (startY <= clipCtx->clipBottom && startY >= clipCtx->clipTop) {
                wDelta = endX - startX;
                outLine->wDelta = wDelta;
                if (wDelta != 0) {
                    if (wDelta < 0) {
                        if (startX >= clipCtx->clipLeft && endX <= clipCtx->clipRight) {
                            remainingExcess = clipCtx->clipLeft - endX;
                            if (remainingExcess > 0) {
                                endX = clipCtx->clipLeft;
                            }
                            rightExcess = startX - clipCtx->clipRight;
                            if (rightExcess > 0) {
                                startX = clipCtx->clipRight;
                            }
                            outLine->rowCount = endY - startY + 1;
                            outLine->x0 = startX;
                            outLine->y0 = startY;
                            outLine->x1 = endX;
                            outLine->y1 = endY;
                            outLine->visible = 0;
                        }
                    } else if (endX >= clipCtx->clipLeft && startX <= clipCtx->clipRight) {
                        remainingExcess = clipCtx->clipLeft - startX;
                        if (remainingExcess > 0) {
                            startX = clipCtx->clipLeft;
                        }
                        rightExcess = endX - clipCtx->clipRight;
                        if (rightExcess > 0) {
                            endX = clipCtx->clipRight;
                        }
                        outLine->rowCount = 1;
                        outLine->x0 = startX;
                        outLine->y0 = startY;
                        outLine->x1 = endX;
                        outLine->y1 = endY;
                        outLine->visible = 0;
                    }
                } else if (startX >= clipCtx->clipLeft && endX <= clipCtx->clipRight) {
                    outLine->rowCount = 1;
                    outLine->x0 = startX;
                    outLine->y0 = startY;
                    outLine->x1 = endX;
                    outLine->y1 = endY;
                    outLine->visible = 0;
                }
            }
        }
    }
}

/*
 * Calls Gfx_RasterClipLineToViewportBounds with the address of its status local.
 * When status is zero, calls Gfx_RasterDrawLine16 with four other local slots and
 * the final two arguments.
 */
// FUNCTION: FF7 0x006aaa86
void __cdecl Gfx_RasterClipAndDrawLine16(int x0, int y0, int x1, int y1, int color, GfxEngineContext *context)
{
    GfxClippedLine status;

    Gfx_RasterClipLineToViewportBounds(x0, y0, x1, y1, &status, context);
    if (status.visible == 0) {
        Gfx_RasterDrawLine16(status.x0, status.y0, status.x1, status.y1, color, context);
    }
}
} /* extern "C" */
