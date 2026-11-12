// src/lib/src/graphics/graphics_matrix_transform_asm.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Helpers that set or clear flag bits on objects and model polygon sets.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/api/graphics.h"
#include "lib/api/mem.h"
#include "lib/types/polygon.h"
#include "lib/api/polygon.h"
extern "C" {

/*
 * For nonnull object, ANDs its flag word (A8) with mask.
 */
// FUNCTION: FF7 0x0066c3d0
void __cdecl Gfx_AndPolygonSetRenderFlags(int mask, int object)
{
    if (object != 0) {
        *(int *)((char *)object + 0xA8) &= mask;
    }
}

/*
 * For nonnull object, ORs flags into its flag word (A8).
 */
// FUNCTION: FF7 0x0066c3f0
void __cdecl Gfx_OrPolygonSetRenderFlags(int flags, int object)
{
    if (object != 0) {
        *(int *)((char *)object + 0xA8) |= flags;
    }
}

/*
 * For nonnull wrapper, calls Gfx_AndPolygonSetRenderFlags with mask and the object pointer stored in its second word.
 */
// FUNCTION: FF7 0x0066c410
void __cdecl Gfx_AndLmdPolygonSetRenderFlags(int mask, void *wrapper)
{
    if (wrapper != 0) {
        Gfx_AndPolygonSetRenderFlags(mask, *(int *)((char *)wrapper + 4));
    }
}

/*
 * For nonnull wrapper, calls Gfx_OrPolygonSetRenderFlags with flags and the object pointer stored in its second word.
 */
// FUNCTION: FF7 0x0066c42e
void __cdecl Gfx_OrLmdPolygonSetRenderFlags(int flags, void *wrapper)
{
    if (wrapper != 0) {
        Gfx_OrPolygonSetRenderFlags(flags, *(int *)((char *)wrapper + 4));
    }
}

/*
 * Calls Pfile_ForEachModelSetEntryMember with modelSet, callback Gfx_AndPolygonSetRenderFlags and mask.
 */
// FUNCTION: FF7 0x0066c44c
void __cdecl Gfx_AndModelSetPolygonSetRenderFlags(int modelSet, int mask)
{
    Pfile_ForEachModelSetEntryMember(modelSet, Gfx_AndPolygonSetRenderFlags, mask);
}

/*
 * Calls Pfile_ForEachModelSetEntryMember with modelSet, callback Gfx_OrPolygonSetRenderFlags and flags.
 */
// FUNCTION: FF7 0x0066c466
void __cdecl Gfx_OrModelSetPolygonSetRenderFlags(int modelSet, int flags)
{
    Pfile_ForEachModelSetEntryMember(modelSet, Gfx_OrPolygonSetRenderFlags, flags);
}

/*
 * For nonnull wrapper, calls Gfx_AndPolygonSetRenderFlags with mask and the object pointer stored in its sixth word.
 */
// FUNCTION: FF7 0x0066c480
void __cdecl Gfx_AndRenderObjectPolygonSetFlags(int mask, void *wrapper)
{
    if (wrapper != 0) {
        Gfx_AndPolygonSetRenderFlags(mask, *(int *)((char *)wrapper + 0x14));
    }
}

/*
 * For nonnull wrapper, calls Gfx_OrPolygonSetRenderFlags with flags and the object pointer stored in its sixth word.
 */
// FUNCTION: FF7 0x0066c49e
void __cdecl Gfx_OrRenderObjectPolygonSetFlags(int flags, int wrapper)
{
    if (wrapper != 0) {
        Gfx_OrPolygonSetRenderFlags(flags, *(int *)((char *)wrapper + 0x14));
    }
}

/*
 * Calls Gfx_ForEachModelPolygonSet with polygonSet, callback Gfx_AndPolygonSetRenderFlags and mask.
 */
// FUNCTION: FF7 0x0066c4bc
void __cdecl Gfx_AndModelPolygonSetRenderFlags(int polygonSet, PolygonModel *mask)
{
    Gfx_ForEachModelPolygonSet(polygonSet, Gfx_AndPolygonSetRenderFlags, mask);
}

/*
 * Calls Gfx_ForEachModelPolygonSet with polygonSet, callback Gfx_OrPolygonSetRenderFlags and flags.
 */
// FUNCTION: FF7 0x0066c4d6
void __cdecl Gfx_OrModelPolygonSetRenderFlags(int polygonSet, PolygonModel *flags)
{
    Gfx_ForEachModelPolygonSet(polygonSet, Gfx_OrPolygonSetRenderFlags, flags);
}

} /* extern "C" */
