// src/lib/src/graphics/graphics_apply_blend_record.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Copies the fields of a blend record into a destination object, and calls the driver's
//   blend-mode slot to fetch a blend record.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/types/graphics.h"
#include "lib/types/polygon.h"
#include "lib/api/graphics.h"
#include "lib/crt.h"
#include "lib/codegen.h"
extern "C" {

// clang-format off
// clang-format on


/*
 * Copy five dwords out of a blend record into scattered members of the
 * destination, when there is a record. Both base pointers are reloaded for
 * every one of the five copies, which is what /Od does with five separate
 * assignments and is not what a struct copy or a cached base would look like.
 */
// FUNCTION: FF7 0x006b3ca0
void __cdecl Gfx_ApplyBlendRecordToHundred(GfxBlendRecord *rec, PolygonHundredState *dst)
{
    if (rec != 0) {
        dst->unusedBlendWord = rec->reservedWord;
        dst->alphaLevel = rec->alphaLevel;
        dst->srcBlend = rec->srcBlend;
        dst->dstBlend = rec->dstBlend;
        dst->depthSort = rec->depthSortBatches;
    }
}

/*
 * Fetches the active graphics driver callback table and invokes its blend-mode
 * record lookup slot (+0x5C) with mode and ctx.
 */
// FUNCTION: FF7 0x006b3ce7
int __cdecl Gfx_GetBlendModeRecordCall(int mode, void *ctx)
{
    GfxDriverOps *drv;

    drv = GetGraphicsDriverObject((GfxEngineContext *)ctx);
    return (int)drv->getBlendMode(mode, (GfxEngineContext *)ctx);
}
} /* extern "C" */
