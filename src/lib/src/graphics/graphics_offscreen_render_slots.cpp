// src/lib/src/graphics/graphics_offscreen_render_slots.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Manages a pool of offscreen render slots: it allocates and releases them, renders their
//   scenes to textures, services their lifecycle, and draws the active ones.
/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/types/file.h"
#include "lib/codegen.h"
#include "lib/types/polygon.h"
#include "lib/types/graphics.h"
#include <string.h>
#include <stdio.h>
#include "lib/api/file.h"
#include "lib/api/graphics.h"
#include "lib/api/polygon.h"
#include "lib/api/mem.h"
#include "lib/enums/graphics.h"
extern "C" {

struct GfxEngineContext;

#pragma intrinsic(strlen)

/*
 * Initializes an offscreen render request and its animation placement record.
 */
// FUNCTION: FF7 0x00682d80
void __cdecl Gfx_InitOffscreenRequest(GfxOffscreenRequest *request)
{
    ZeroFillMemory32(0x118, request);
    AnimPlacement_Init((PolygonAnimPlacement *)request->placement);
    request->pageFillMode = 1;
}

/*
 * Renders an offscreen scene to a texture and builds the result render object.
 */
// FUNCTION: FF7 0x00682db2
int __cdecl Gfx_BuildOffscreenRenderObject(GfxOffscreenRequest *request, GfxEngineContext *context)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[280];
        int savedDrawSpecWord;
        struct { unsigned char pad_to_4[0x4]; GfxGroupDrawSpec drawSpecBuffer; };
        struct { unsigned char pad_to_48[0x48]; GfxDrawSpec *localDrawSpec; };
        struct { unsigned char pad_to_4C[0x4C]; int renderObject; };
        struct { unsigned char pad_to_50[0x50]; GfxTexturePageFill pageFill; };
        struct { unsigned char pad_to_108[0x108]; GfxClearColorBgra bgColor; };
    } frame;
#pragma pack(pop)

    frame.renderObject = 0;
    if (request != 0) {
        if (request->kind != GFX_OFFSCREEN_REQUEST_NONE) {
            GfxSetViewportRect(0, 0, context->displayWidth, context->displayHeight, (GfxEngineContext *)context);
            frame.bgColor.r = 0;
            frame.bgColor.g = 0;
            frame.bgColor.b = 0;
            frame.bgColor.a = FLOAT_ONE_BITS;
            GfxSetBackgroundColorCall(&frame.bgColor, context);
            GfxClearCall(1, 1, 1, context);
            SetProjectionOffset(request->projOffsetX, request->projOffsetY);
            context->viewportHeightOverride = request->viewportHeightOverride;
            if (request->altViewportX == 0 && request->altViewportY == 0 && request->altViewportW == 0 && request->altViewportH == 0) {
                GfxSetViewportRect(request->viewportX, request->viewportY, request->viewportW, request->viewportH, (GfxEngineContext *)context);
            } else {
                GfxSetViewportRect(request->altViewportX, request->altViewportY, request->altViewportW, request->altViewportH, (GfxEngineContext *)context);
            }
            Gfx_BeginSortedDrawFrame(context);
            Gfx_ResetNodeList(context);
            if (((int (__cdecl *)(int scene, int context))Gfx_BeginSceneCall)(0, (int)context) != 0) { /* reads eax after a void call, as the original does */
                switch (request->kind) {
                case GFX_OFFSCREEN_REQUEST_NONE:
                    break;
                case GFX_OFFSCREEN_REQUEST_MESH:
                    Gfx_SetModelMatrix((int)request->modelMatrix, request->mesh, context);
                    Gfx_DrawMeshPolygonSet(request->mesh, (int)context);
                    break;
                case GFX_OFFSCREEN_REQUEST_SKELETON:
                    frame.localDrawSpec = (GfxDrawSpec *)&frame.drawSpecBuffer;
                    Gfx_InitDrawSpec(GFX_DRAW_SPEC_KIND_RESTORE_COLORS, frame.localDrawSpec);
                    Gfx_ApplyDrawSpecToModel(frame.localDrawSpec, (PolygonModel *)request->drawSpec);
                    Gfx_SetContextMatrix((void *)request->modelMatrix, context);
                    frame.savedDrawSpecWord = *(int *)((char *)request->drawSpec + 4);
                    *(int *)((char *)request->drawSpec + 4) = 1;
                    Anim_DrawSkeletonFrame(request->skeleton, (PolygonSkeletonAnim *)request->animFrame, (PolygonBoneTransform *)request->placement, (PolygonAnimDraw *)(request->drawSpec), (GfxEngineContext *)context);
                    *(int *)((char *)request->drawSpec + 4) = frame.savedDrawSpecWord;
                }
                Gfx_SetScenePassCall(GFX_SCENE_PASS_TRANSLUCENT, context);
                GfxFlushFrameDrawList((struct GfxEngineContext *)context);
                Gfx_EndSceneCall(context);
            }
        }
        Gfx_ClearTexturePageFill((int)&frame.pageFill);
        frame.pageFill.mode = request->pageFillMode;
        frame.pageFill.originX = request->viewportX;
        frame.pageFill.originY = request->viewportY;
        frame.pageFill.width = request->viewportW;
        if (IsPowerOfTwo(frame.pageFill.width) != 0) { /* empty first arm: lib/codegen.h */
        } else {
            frame.pageFill.width = RoundUpToPowerOfTwo(frame.pageFill.width);
        }
        frame.pageFill.height = request->viewportH;
        if (IsPowerOfTwo(frame.pageFill.height) != 0) { /* empty first arm: lib/codegen.h */
        } else {
            frame.pageFill.height = RoundUpToPowerOfTwo(frame.pageFill.height);
        }
        frame.pageFill.stepX = request->pageFillStepX;
        frame.pageFill.stepY = request->pageFillStepY;
        frame.pageFill.isEnabled = request->pageFillEnabled;
        frame.renderObject = Gfx_CreateSurfaceRenderObject(request->renderKind, request->blendMode, 1, &frame.pageFill, request->drawPool, request->renderObjectBank);
    }
    return frame.renderObject;
}

/*
 * Releases an offscreen slot and destroys its associated render object.
 */
// FUNCTION: FF7 0x0068317e
void __cdecl Gfx_ReleaseOffscreenSlot(GfxOffscreenSlot *slot)
{
    if (slot != 0) {
        slot->inUse = GFX_OFFSCREEN_SLOT_FREE;
        DestroyRenderObject((void *)slot->renderObject);
        slot->renderObject = 0;
    }
}

/*
 * Iterates through an array of offscreen slots and releases each one.
 */
// FUNCTION: FF7 0x006831ab
void __cdecl Gfx_ReleaseOffscreenSlots(int count, GfxOffscreenSlot *slots)
{
    GfxOffscreenSlot *slot;
    int index;

    slot = slots;
    for (index = 0; index < count; index++) {
        Gfx_ReleaseOffscreenSlot(slot);
        slot++;
    }
}

/*
 * Finds and allocates the first available offscreen slot in an array.
 */
// FUNCTION: FF7 0x006831ef
GfxOffscreenSlot *__cdecl Gfx_AllocOffscreenSlot(int count, GfxOffscreenSlot *slots)
{
    GfxOffscreenSlot *slot;
    GfxOffscreenSlot *pFound;
    int index;

    pFound = 0;
    slot = slots;
    for (index = 0; index < count; index++) {
        if (slot->inUse == GFX_OFFSCREEN_SLOT_FREE) {
            pFound = slot;
            ZeroFillMemory32(0x124, pFound);
            pFound->inUse = GFX_OFFSCREEN_SLOT_PENDING;
            pFound->requestId = index;
            break;
        }
        slot++;
    }
    return pFound;
}

/*
 * Updates lifecycle states and builds render objects for active offscreen slots.
 */
// FUNCTION: FF7 0x00683263
void __cdecl Gfx_ServiceOffscreenSlots(int count, GfxOffscreenSlot *slots)
{
    GfxEngineContext *ctx;
    GfxOffscreenSlot *slot;
    GfxOffscreenRequest *renderObj;
    int index;

    ctx = GetEngineContext();
    slot = slots;
    for (index = 0; index < count; index++) {
        switch (slot->inUse) {
        case GFX_OFFSCREEN_SLOT_FREE:
            break;
        case GFX_OFFSCREEN_SLOT_PENDING:
            renderObj = (GfxOffscreenRequest *)slot->request;
            slot->renderObject = Gfx_BuildOffscreenRenderObject(renderObj, ctx);
            if (slot->renderObject != 0) {
                slot->inUse = GFX_OFFSCREEN_SLOT_BUILT;
            } else {
                Gfx_ReleaseOffscreenSlot(slot);
            }
            break;
        case GFX_OFFSCREEN_SLOT_RELEASE:
            Gfx_ReleaseOffscreenSlot(slot);
        }
        slot++;
    }
}

/*
 * Emits primitive batches for active offscreen slots matching batch mode.
 */
// FUNCTION: FF7 0x00683311
void __cdecl Gfx_DrawOffscreenSlots(int batchMode, int count, GfxOffscreenSlot *slots)
{
    GfxEngineContext *ctx;
    GfxOffscreenSlot *cursor;
    int shouldDraw;
    int index;
    ctx = GetEngineContext();
    cursor = slots;
    for (index = 0; index < count; index++) {
        switch (cursor->inUse) {
        case GFX_OFFSCREEN_SLOT_FREE:
            break;
        case GFX_OFFSCREEN_SLOT_BUILT:
        case GFX_OFFSCREEN_SLOT_RELEASE:
            if (cursor->state == 4) {
                if (batchMode)
                    shouldDraw = 0;
                else
                    shouldDraw = 1;
            } else {
                if (batchMode)
                    shouldDraw = 1;
                else
                    shouldDraw = 0;
            }
            if (shouldDraw)
                Gfx_EmitPrimitiveBatchList((void *)cursor->renderObject, ctx);
        }
        cursor++;
    }
}
} /* extern "C" */
