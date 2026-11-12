// Contents: A single helper that sets the texture render state and draws an indexed primitive,
//   retrying while the device is still drawing.
#include "lib/com_abi.h"
#include "lib/types/graphics.h"
#include "lib/types/polygon.h"
#include "lib/enums/graphics.h"

#include "lib/api/graphics.h"
/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
extern "C" {

/*
 * Sets texture render state and retries DrawIndexedPrimitive on WASSTILLDRAWING.
 */
// FUNCTION: FF7 0x006a6790
void __cdecl DxRendx_SetStateAndDrawIndexedRetry(GfxIndexedDrawRequest *drawArgs, struct GfxEngineContext *context)
{
    int device;
    GfxIndexedDrawArgs *recordPtr;
    int baseVertex;
    PolygonHundredState *drawSpec;
    GfxTextureSurface *tex;
    int code;
    int surfaceStatus;

    device = context->d3dDevice2;
    drawSpec = (PolygonHundredState *)(drawArgs->drawSpec);
    tex = (GfxTextureSurface *)(drawSpec->texture);
    if (tex != 0 && tex->surface != 0) {
        surfaceStatus = ((ComDirectDrawSurface *)(tex->surface))->IsLost();
        if (surfaceStatus == DDERR_SURFACELOST) {
            *(GfxTextureSurface **)&drawSpec->texture = Gfx_RestoreTextureSurfaces(tex);
            if (drawSpec->texture == 0) {
            }
        }
    }
    ((ComDirect3DDevice2 *)device)->SetRenderState(D3DRENDERSTATE_TEXTUREHANDLE, tex->handle);
    ((ComDirect3DDevice2 *)device)->SetRenderState(D3DRENDERSTATE_SHADEMODE, drawSpec->shadeMode);
    recordPtr = (GfxIndexedDrawArgs *)(*drawArgs->indexedArgsRef);
    if (drawArgs->clipEnabled != 0) {
        recordPtr->flags &= ~D3DDP_DONOTCLIP;
    } else {
        recordPtr->flags |= D3DDP_DONOTCLIP;
    }
    baseVertex = drawArgs->baseVertex;
    do {
        code = ((ComDirect3DDevice2 *)device)->DrawIndexedPrimitive(recordPtr->primitiveType, recordPtr->vertexType, baseVertex, recordPtr->vertexCount, recordPtr->startIndex, recordPtr->primitiveCount, recordPtr->flags);
        switch (code) {
        case DD_OK:
            break;
        case DDERR_WASSTILLDRAWING:
            D3D5CountWasStillDrawing(1, context);
            break;
        default:
            // STRING: FF7 0x009132f0
            CheckDirectDrawResult(code, "C:\\lib\\src\\graphics\\dx_rendx.cpp", 0xEC);
            code = 0;
        }
    } while (code != 0);
}
}
