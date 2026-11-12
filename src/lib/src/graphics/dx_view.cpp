// src/lib/src/graphics/dx_view.cpp
// Original: C:\lib\src\graphics\dx_view.cpp
// Contents: Direct3D viewport management: creates, attaches and detaches viewport interfaces,
//   initializes viewport data (including fullscreen variants), sets the viewport rectangle, and
//   clears colour and depth surfaces.
/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/types/graphics.h"
#include "lib/codegen.h"
#include "lib/com_abi.h"
#include "lib/api/graphics.h"
#include "lib/api/mem.h"
#include <string.h>

struct GfxEngineContext;

// Native COM views preserve the original virtual __stdcall receiver evaluation.
// Each unused slot is declaration-only; no instances or vtables are emitted.

extern "C" {
#pragma intrinsic(memset)

/*
 * Detaches Direct3D viewport interface from device context.
 */
// FUNCTION: FF7 0x006a91f0
void __cdecl D3DDetachViewport(GfxEngineContext *viewState)
{
    if (viewState != 0 && viewState->viewport != 0) {
        if (viewState->d3dDevice != 0) {
            // STRING: FF7 0x0091782c
            CheckDirectDrawResult(((ComDirect3DDevice *)(viewState->d3dDevice))->DeleteViewport(viewState->viewport), "C:\\lib\\src\\graphics\\dx_view.cpp", 18);
        }
        viewState->viewport = 0;
    }
}

/*
 * Creates and attaches Direct3D viewport interface to device context.
 */
// FUNCTION: FF7 0x006a9252
void __cdecl D3DCreateAndAttachViewport(GfxEngineContext *viewState)
{
    int viewport;

    if (viewState != 0 && viewState->direct3D != 0 && viewState->d3dDevice != 0) {
        viewport = 0;
        D3DDetachViewport(viewState);
        // STRING: FF7 0x0091784c
        if (CheckDirectDrawResult(((ComDirect3D *)(viewState->direct3D))->CreateViewport(&viewport, 0), "C:\\lib\\src\\graphics\\dx_view.cpp", 37) != 0) {
            // STRING: FF7 0x0091786c
            if (CheckDirectDrawResult(((ComDirect3DDevice *)(viewState->d3dDevice))->AddViewport(viewport), "C:\\lib\\src\\graphics\\dx_view.cpp", 39) != 0) {
                viewState->viewport = viewport;
            }
        }
    }
}

/*
 * Initializes Direct3D viewport parameters and scale matrices.
 */
// FUNCTION: FF7 0x006a9305
void __cdecl BuildD3DViewportData(int originX, int originY, int width, int height, GfxD3DScaledViewport *viewportData)
{
    float aspectRatio;
    ZeroFillMemory32(44, viewportData);
    viewportData->dwSize = 44;
    viewportData->dwX = originX;
    viewportData->dwY = originY;
    viewportData->dwWidth = width;
    viewportData->dwHeight = height;
    viewportData->scaleX = (float)*(unsigned int *)&viewportData->dwWidth / 2.0f;
    if (GetEngineContext()->viewportHeightOverride != 0)
        viewportData->scaleY = (float)GetEngineContext()->viewportHeightOverride / 2.0f;
    else
        viewportData->scaleY = (float)*(unsigned int *)&viewportData->dwHeight / 2.0f;
    viewportData->maxX = FLOAT_ONE_BITS;
    viewportData->maxY = FLOAT_ONE_BITS;
    if (GetEngineContext()->viewportHeightOverride != 0)
        *(float *)&viewportData->maxY = (aspectRatio = (float)height / width) / 0.75f;
    viewportData->dvMinZ = 0;
    viewportData->dvMaxZ = FLOAT_ONE_BITS;
}

/*
 * Initializes fullscreen Direct3D viewport parameters.
 */
// FUNCTION: FF7 0x006a93fa
void __cdecl BuildFullscreenD3DViewportData(struct GfxD3DScaledViewport *viewportData, GfxEngineContext *engineContext)
{
    BuildD3DViewportData(0, 0, engineContext->displayWidth, engineContext->displayHeight, viewportData);
}

/*
 * Applies viewport parameters to Direct3D viewport interface.
 */
// FUNCTION: FF7 0x006a9423
void __cdecl SetD3DViewportData(int viewport, GfxEngineContext *viewState)
{
    if (viewState != 0 && viewState->viewport != 0) {
        // STRING: FF7 0x0091788c
        CheckDirectDrawResult(((ComDirect3DViewport *)(viewState->viewport))->SetViewport(viewport), "C:\\lib\\src\\graphics\\dx_view.cpp", 108);
    }
}

/*
 * Resets viewport rectangle to cover the full render surface.
 */
// FUNCTION: FF7 0x006a9466
void __cdecl D3DResetFullscreenViewportRect(GfxEngineContext *viewState)
{
    if (viewState != 0 && viewState->viewport != 0) {
        GfxSetViewportRect(0, 0, viewState->displayWidth, viewState->displayHeight, viewState);
    }
}

/*
 * Detaches secondary Direct3D viewport interface from device context.
 */
// FUNCTION: FF7 0x006a94a1
void __cdecl D3DDetachViewport2(GfxEngineContext *viewState)
{
    if (viewState != 0 && viewState->viewport2 != 0) {
        if (viewState->d3dDevice2 != 0) {
            // STRING: FF7 0x009178ac
            CheckDirectDrawResult(((ComDirect3DDevice2 *)(viewState->d3dDevice2))->DeleteViewport(viewState->viewport2), "C:\\lib\\src\\graphics\\dx_view.cpp", 130);
        }
        viewState->viewport2 = 0;
    }
}

/*
 * Creates and attaches secondary Direct3D viewport interface.
 */
// FUNCTION: FF7 0x006a9506
void __cdecl D3DCreateAndAttachViewport2(GfxEngineContext *viewState)
{
    int viewport;

    if (viewState != 0 && viewState->direct3D2 != 0 && viewState->d3dDevice2 != 0) {
        viewport = 0;
        D3DDetachViewport2(viewState);
        // STRING: FF7 0x009178cc
        if (CheckDirectDrawResult(((ComDirect3D2 *)(viewState->direct3D2))->CreateViewport(&viewport, 0), "C:\\lib\\src\\graphics\\dx_view.cpp", 149) != 0) {
            // STRING: FF7 0x009178ec
            if (CheckDirectDrawResult(((ComDirect3DDevice2 *)(viewState->d3dDevice2))->AddViewport(viewport), "C:\\lib\\src\\graphics\\dx_view.cpp", 151) != 0) {
                viewState->viewport2 = viewport;
            }
        }
    }
}

/*
 * Initializes D3DVIEWPORT2 parameters and homogeneous clip planes.
 */
// FUNCTION: FF7 0x006a95bf
void __cdecl BuildD3DViewport(int originX, int originY, int width, int height, GfxD3DViewport *viewport)
{
    float aspectRatio;

    ZeroFillMemory32(44, viewport);
    viewport->viewportSize = 44;
    viewport->viewX = originX;
    viewport->viewY = originY;
    viewport->viewWidth = width;
    viewport->viewHeight = height;
    viewport->clipX = FLOAT_MINUS_ONE_BITS;
    viewport->clipWidth = FLOAT_2_BITS;
    if (GetEngineContext()->viewportHeightOverride != 0) {
        viewport->clipHeight = (aspectRatio = (float)height / width) / 0.75f * 2.0f;
        viewport->clipY = viewport->clipHeight / 2.0f;
    } else {
        *(int *)&viewport->clipHeight = 0x40000000;
        viewport->clipY = viewport->clipHeight / 2.0f;
    }
    viewport->dvMinZ = 0;
    viewport->dvMaxZ = FLOAT_ONE_BITS;
}

/*
 * Initializes fullscreen D3DVIEWPORT2 parameters.
 */
// FUNCTION: FF7 0x006a9683
void __cdecl BuildFullscreenD3DViewport2(struct GfxD3DViewport *viewport, GfxEngineContext *context)
{
    BuildD3DViewport(0, 0, context->displayWidth, context->displayHeight, viewport);
}

/*
 * Applies viewport parameters to secondary Direct3D viewport interface.
 */
// FUNCTION: FF7 0x006a96ac
void __cdecl SetD3DViewport2Data(int viewport, GfxEngineContext *context)
{
    if (context != 0 && context->viewport2 != 0) {
        // STRING: FF7 0x0091790c
        CheckDirectDrawResult(((ComDirect3DViewport2 *)(context->viewport2))->SetViewport2(viewport), "C:\\lib\\src\\graphics\\dx_view.cpp", 210);
    }
}

/*
 * Clears Direct3D viewport color and depth buffer surfaces.
 */
// FUNCTION: FF7 0x006a96f2
void __cdecl D3DViewportClear(int clearColor, int clearDepth, int value, GfxEngineContext *context)
{
    int flags;
    struct {
        int left, top, right, bottom;
    } rect;
    int count;

    if (clearColor != 0 || clearDepth != 0) {
        if (context->useDirect3D5 != 0) {
            if (context != 0 && context->viewport2 != 0 && context->backgroundMaterial != 0) {
                flags = 0;
                if (clearColor != 0) {
                    flags = 1;
                }
                if (context->zBufferEnabled != 0 && context->zBufferSurface != 0 && clearDepth != 0) {
                    flags |= 2;
                }
                if (flags != 0) {
                    count = 1;
                    rect.left = 0;
                    rect.top = 0;
                    rect.right = context->displayWidth;
                    rect.bottom = context->displayHeight;
                    // STRING: FF7 0x0091792c
                    CheckDirectDrawResult(((ComDirect3DViewport *)(context->viewport2))->Clear(count, (int)&rect, flags), "C:\\lib\\src\\graphics\\dx_view.cpp", 262);
                }
            }
        } else if (context != 0 && context->viewport != 0 && context->backgroundMaterial != 0) {
            flags = 0;
            if (clearColor != 0) {
                flags = 1;
            }
            if (context->zBufferEnabled != 0 && context->zBufferSurface != 0 && clearDepth != 0) {
                flags |= 2;
            }
            if (flags != 0) {
                count = 1;
                rect.left = 0;
                rect.top = 0;
                rect.right = context->displayWidth;
                rect.bottom = context->displayHeight;
                // STRING: FF7 0x0091794c
                CheckDirectDrawResult(((ComDirect3DViewport *)(context->viewport))->Clear(count, (int)&rect, flags), "C:\\lib\\src\\graphics\\dx_view.cpp", 317);
            }
        }
    }
}

/*
 * Clears rendering surfaces using Direct3D viewport clear.
 */
// FUNCTION: FF7 0x006a98bf
void __cdecl GfxClearColorAndDepth(struct GfxEngineContext *context)
{
    GfxClearCall(1, 1, 1, context);
}

/*
 * Sets Direct3D viewport rectangle and adjusts projection parameters.
 */
// FUNCTION: FF7 0x006a98d6
void __cdecl D3DSetViewportRect(int left, int top, int width, int height, struct GfxEngineContext *context)
{
    int mainViewport[11];
    int localViewportData[11];
    if (context->useDirect3D5) {
        BuildD3DViewport(left, top, width, height, (struct GfxD3DViewport *)mainViewport);
        if (!context->viewport2)
            D3DCreateAndAttachViewport2(context);
        SetD3DViewport2Data((int)mainViewport, context);
        D3D5SetRenderTargetAndViewport(0, 1, GfxGetSurfaceByIndex(1), context->viewport2, context);
    } else {
        BuildD3DViewportData(left, top, width, height, (struct GfxD3DScaledViewport *)localViewportData);
        D3DCreateAndAttachViewport(context);
        SetD3DViewportData((int)localViewportData, context);
    }
    context->clipLeft = left;
    context->clipTop = top;
    context->viewSizeX = width;
    context->viewSizeY = height;
    SetPerspective(context->fov, context->nearPlane, context->farPlane, context->offsetX, context->offsetY, (float)context->viewSizeX, (float)context->viewSizeY, context);
}

/*
 * Sets background material for Direct3D viewport.
 */
// FUNCTION: FF7 0x006a9a17
void __cdecl D3DSetBackgroundMaterial(struct GfxBgraColorSource *color, GfxEngineContext *context)
{
    int materialData[20];
    int *materialPointer;
    int handle;
    if (context && context->viewport) {
        materialPointer = materialData;
        memset(materialPointer, 0, 80);
        materialPointer[0] = 80;
        materialPointer[0x13] = 1;
        if (materialPointer) {
            D3DFillMaterialColorFromBgra(1, color, materialPointer);
            // STRING: FF7 0x0091796c
            if (CheckDirectDrawResult(((ComDirect3DMaterial *)context->backgroundMaterial)->SetMaterial(materialPointer), "C:\\lib\\src\\graphics\\dx_view.cpp", 407)) {
                // STRING: FF7 0x0091798c
                if (CheckDirectDrawResult(((ComDirect3DMaterial *)context->backgroundMaterial)->GetHandle(context->d3dDevice, &handle), "C:\\lib\\src\\graphics\\dx_view.cpp", 409)) {
                    if (context->useDirect3D5) {
                        // STRING: FF7 0x009179ac
                        CheckDirectDrawResult(((ComDirect3DViewport *)context->viewport2)->SetBackground(handle), "C:\\lib\\src\\graphics\\dx_view.cpp", 413);
                    } else {
                        // STRING: FF7 0x009179cc
                        CheckDirectDrawResult(((ComDirect3DViewport *)context->viewport)->SetBackground(handle), "C:\\lib\\src\\graphics\\dx_view.cpp", 420);
                    }
                }
            }
        }
    }
}
}
