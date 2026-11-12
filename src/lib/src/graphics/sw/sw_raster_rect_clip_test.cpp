// src/lib/src/graphics/sw/sw_raster_rect_clip_test.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Classifies a rectangle against the context's clip bounds for the software rasterizer.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/types/graphics.h"
#include "lib/api/graphics.h"
#include "lib/crt.h"
#include "lib/codegen.h"
extern "C" {

// clang-format off
// clang-format on

// Classifies the inclusive rectangle from (x, y) to (x + width - 1, y + height - 1) against the
// context clip bounds clipLeft, clipRight, clipTop and clipBottom. Returns and stores this status:
// 0 for fully inside, 1 for horizontal clipping, 2 for vertical clipping, 3 for both, or 4 for no
// overlap.
// FUNCTION: FF7 0x006bf220
int __cdecl Gfx_RasterClassifyRectAgainstClipBounds(int x, int y, int width, int height, GfxClipResult *clipResult, GfxEngineContext *context)
{
    int right;
    int leftOver;
    int bottomOver;
    int overRight;
    int status;
    int topOver;
    int bottom;

    right = x + width + -1;
    bottom = y + height + -1;
    if (x >= context->clipLeft && right <= context->clipRight) {
        if (y >= context->clipTop && bottom <= context->clipBottom) {
            status = 0;
        } else {
            status = 4;
            if (y <= context->clipBottom && bottom >= context->clipTop) {
                topOver = context->clipTop - y;
                if (topOver > 0) { /* empty first arm: lib/codegen.h */
                } else {
                    topOver = 0;
                }
                bottomOver = bottom - context->clipBottom;
                if (bottomOver > 0) { /* empty first arm: lib/codegen.h */
                } else {
                    bottomOver = 0;
                }
                status = 2;
                clipResult->leftOver = 0;
                clipResult->overRight = 0;
                clipResult->topOver = topOver;
                clipResult->bottomOver = bottomOver;
            }
        }
    } else if (y >= context->clipTop && bottom <= context->clipBottom) {
        status = 4;
        if (x <= context->clipRight && right >= context->clipLeft) {
            leftOver = context->clipLeft - x;
            if (leftOver > 0) { /* empty first arm: lib/codegen.h */
            } else {
                leftOver = 0;
            }
            overRight = right - context->clipRight;
            if (overRight > 0) { /* empty first arm: lib/codegen.h */
            } else {
                overRight = 0;
            }
            status = 1;
            clipResult->leftOver = leftOver;
            clipResult->overRight = overRight;
            clipResult->topOver = 0;
            clipResult->bottomOver = 0;
        }
    } else {
        status = 4;
        if (y <= context->clipBottom && bottom >= context->clipTop && x <= context->clipRight && right >= context->clipLeft) {
            topOver = context->clipTop - y;
            if (topOver > 0) { /* empty first arm: lib/codegen.h */
            } else {
                topOver = 0;
            }
            bottomOver = bottom - context->clipBottom;
            if (bottomOver > 0) { /* empty first arm: lib/codegen.h */
            } else {
                bottomOver = 0;
            }
            leftOver = context->clipLeft - x;
            if (leftOver > 0) { /* empty first arm: lib/codegen.h */
            } else {
                leftOver = 0;
            }
            overRight = right - context->clipRight;
            if (overRight > 0) { /* empty first arm: lib/codegen.h */
            } else {
                overRight = 0;
            }
            status = 3;
            clipResult->leftOver = leftOver;
            clipResult->overRight = overRight;
            clipResult->topOver = topOver;
            clipResult->bottomOver = bottomOver;
        }
    }
    clipResult->status = status;
    return status;
}
} /* extern "C" */
