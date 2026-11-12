// src/ff7/src/battle/battle_gfx_offscreen_slots.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Manages the offscreen render slots used by battle graphics: claiming, releasing,
//   servicing and drawing them, either all at once or one at a time.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include <stdio.h>
#include "ff7/api/battle.h"
#include "lib/api/file.h"
#include "lib/api/graphics.h"
#include "lib/api/token.h"
#include <string.h>
#include "lib/types/file.h"
#include "ff7/types/battle.h"
#include "lib/types/graphics.h"
extern "C" {

#pragma intrinsic(memset, strcpy, strcat)


// clang-format off
// GLOBAL: FF7 0x009a8130
unsigned char g_GfxOffscreenSlots[4 * 0x124] = {0};
// clang-format on

/*
 * Releases all four offscreen render slots.
 */
// FUNCTION: FF7 0x0041a090
void __cdecl Gfx_ReleaseAllOffscreenSlots(void)
{
    Gfx_ReleaseOffscreenSlots(4, (GfxOffscreenSlot *)g_GfxOffscreenSlots);
}

// Releases offscreen render slot `index` (0..3 of g_GfxOffscreenSlots).
// FUNCTION: FF7 0x0041a0a4
void __cdecl Gfx_ReleaseOffscreenSlotByIndex(int index)
{
    if (index >= 0 && index < 4) {
        Gfx_ReleaseOffscreenSlot(&((GfxOffscreenSlot *)g_GfxOffscreenSlots)[index]);
    }
}

/*
 * Allocates an offscreen render slot and initializes its request descriptor from the graphics context.
 */
// FUNCTION: FF7 0x0041a0cc
int __cdecl Gfx_ClaimOffscreenSlotRequest(GfxEngineContext *context)
{
    GfxOffscreenRequest *request;
    GfxOffscreenSlot *slot;

    request = 0;
    slot = (GfxOffscreenSlot *)(Gfx_AllocOffscreenSlot(4, (GfxOffscreenSlot *)&g_GfxOffscreenSlots));
    if (slot != 0) {
        request = (GfxOffscreenRequest *)((int)slot + 0xC);
        Gfx_InitOffscreenRequest(request);
        if (g_BattleModeTransition == 0) {
            request->pageFillMode = 0;
        }
        request->renderObjectBank = context->renderObjectBank;
        request->drawPool = 0;
        request->slotIndex = slot->requestId;
    }
    return (int)request;
}

/*
 * Returns the render object of the offscreen render slot at index (0-3), or 0.
 */
// FUNCTION: FF7 0x0041a151
int __cdecl Gfx_GetOffscreenSlotRenderObject(int index)
{
    int value = 0;

    if (index >= 0 && index < 4) {
        value = ((GfxOffscreenSlot *)g_GfxOffscreenSlots)[index].renderObject;
    }
    return value;
}

/*
 * Services all four offscreen render slots.
 */
// FUNCTION: FF7 0x0041a181
void __cdecl Gfx_ServiceAllOffscreenSlots(void)
{
    Gfx_ServiceOffscreenSlots(4, (GfxOffscreenSlot *)g_GfxOffscreenSlots);
}

// Draws the four offscreen render slots with parameter `value`.
// FUNCTION: FF7 0x0041a195
void __cdecl Gfx_DrawAllOffscreenSlots(int value)
{
    Gfx_DrawOffscreenSlots(value, 4, (GfxOffscreenSlot *)g_GfxOffscreenSlots);
}

} /* extern "C" */
