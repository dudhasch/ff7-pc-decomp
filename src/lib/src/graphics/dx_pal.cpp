// clang-format off
#include "lib/api/mem.h"
#include "lib/types/graphics.h"
#include "lib/types/polygon.h"
#include "lib/enums/graphics.h"
#include "lib/com_abi.h"
#include "lib/api/graphics.h"

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
struct PaletteColor {
    unsigned char b, g, r, a;
};

extern "C" {
// GLOBAL: FF7 0x00db2d1c
int g_GfxPaletteUpdateFlag = 0;
// clang-format on

/*
 * Creates DirectDraw palette object and initializes palette entries.
 */
// FUNCTION: FF7 0x006a5d80
int __cdecl D3DCreatePaletteObject(int bitDepth, struct GfxPaletteRecord *paletteDesc)
{
    int created;
    int flags;
    created = 0;
    if (paletteDesc) {
        flags = 0;
        switch (bitDepth) {
        case GFX_PALETTE_DEPTH_1BIT:
            flags |= DDPCAPS_1BIT;
            break;
        case GFX_PALETTE_DEPTH_2BIT:
            flags |= DDPCAPS_2BIT;
            break;
        case GFX_PALETTE_DEPTH_4BIT:
            flags |= DDPCAPS_4BIT;
            break;
        default:
            flags |= DDPCAPS_8BIT;
            flags |= DDPCAPS_ALLOW256;
            break;
        }
        // STRING: FF7 0x00913050
        if (CheckDirectDrawResult(((ComDirectDraw *)(GetEngineContext())->ddObject)->CreatePalette(flags, (void *)paletteDesc->d3dEntries, &paletteDesc->releaseCallback, 0), "C:\\lib\\src\\graphics\\dx_pal.cpp", 0x2c))
            created = 1;
        else
            created = 0;
    }
    return created;
}

/*
 * Attaches palette object to DirectDraw surface.
 */
// FUNCTION: FF7 0x006a5e40
int __cdecl D3DSurfaceSetPalette(struct GfxPaletteRecord *surface, int vtableSlot)
{
    int result;

    result = 0;
    if (surface != 0 && vtableSlot != 0) {
        // STRING: FF7 0x00913070
        if (CheckDirectDrawResult(((ComDirectDrawSurface *)(vtableSlot))->SetPalette(surface->releaseCallback), "C:\\lib\\src\\graphics\\dx_pal.cpp", 0x40) != 0) {
            result = 1;
        }
    }
    return result;
}

/*
 * Copies palette color entries into palette buffer.
 */
// FUNCTION: FF7 0x006a5e8c
void __cdecl Gfx_CopyPaletteEntriesD3D(int count, int srcStart, int srcEntries, int dstStart, int dstEntries)
{
    int i;

    if (srcEntries != 0 && dstEntries != 0) {
        for (i = 0; i < count; i++) {
            ((GfxPaletteEntry *)dstEntries)[dstStart + i].colorR = ((GfxPaletteSourceEntry *)srcEntries)[srcStart + i].red;
            ((GfxPaletteEntry *)dstEntries)[dstStart + i].colorG = ((GfxPaletteSourceEntry *)srcEntries)[srcStart + i].green;
            ((GfxPaletteEntry *)dstEntries)[dstStart + i].colorB = ((GfxPaletteSourceEntry *)srcEntries)[srcStart + i].blue;
            ((GfxPaletteEntry *)dstEntries)[dstStart + i].flags = 0;
        }
    }
}

/*
 * Converts palette color entries into float RGBA format.
 */
// FUNCTION: FF7 0x006a5f18
void __cdecl Gfx_ConvertPaletteEntriesToFloat(int count, int srcStart, int srcEntries, int dstStart, int dstFloats)
{
    int i;

    if (srcEntries != 0 && dstFloats != 0) {
        for (i = 0; i < count; i++) {
            ((GfxPaletteFloatColor *)dstFloats)[dstStart + i].red = (float)*(unsigned char *)&((GfxPaletteSourceEntry *)srcEntries)[srcStart + i].red;
            ((GfxPaletteFloatColor *)dstFloats)[dstStart + i].green = (float)*(unsigned char *)&((GfxPaletteSourceEntry *)srcEntries)[srcStart + i].green;
            ((GfxPaletteFloatColor *)dstFloats)[dstStart + i].blue = (float)*(unsigned char *)&((GfxPaletteSourceEntry *)srcEntries)[srcStart + i].blue;
            ((GfxPaletteFloatColor *)dstFloats)[dstStart + i].alpha = (float)((GfxPaletteSourceEntry *)srcEntries)[srcStart + i].alpha;
        }
    }
}

/*
 * Maps palette entries through color ramp lookup tables.
 */
// FUNCTION: FF7 0x006a5feb
void __cdecl Gfx_MapPaletteEntriesViaRampTables(int count, int srcStart, int srcEntries, int dstStart, int dstEntries)
{
    GfxEngineContext *context;
    int i;

    if (srcEntries != 0 && dstEntries != 0) {
        context = GetEngineContext();
        for (i = 0; i < count; i++) {
            ((int *)dstEntries)[dstStart + i] = Blit_MapArgb8888ViaRampTables(((GfxArgb32 *)srcEntries)[srcStart + i], context);
        }
    }
}

/*
 * Gathers palette entries into destination by index array.
 */
// FUNCTION: FF7 0x006a604a
void __cdecl Gfx_GatherPaletteEntriesByIndex(int count, int indexStart, int dstStart, int indexArray, int palette, int dstEntries)
{
    int i;

    if (indexArray != 0 && dstEntries != 0) {
        for (i = 0; i < count; i++) {
            ((int *)dstEntries)[dstStart + i] = ((int *)palette)[((int *)indexArray)[indexStart + i]];
        }
    }
}

/*
 * Frees palette record and releases DirectDraw palette interface.
 */
// FUNCTION: FF7 0x006a609a
void __cdecl Gfx_FreePaletteRecord(GfxPaletteRecord *paletteRecord)
{
    if (paletteRecord != 0) {
        // STRING: FF7 0x00913090
        GameHeapFree((void *)paletteRecord->floatEntries, "C:\\lib\\src\\graphics\\dx_pal.cpp", 0x8A);
        // STRING: FF7 0x009130b0
        GameHeapFree((void *)paletteRecord->colorEntries, "C:\\lib\\src\\graphics\\dx_pal.cpp", 0x8B);
        // STRING: FF7 0x009130d0
        GameHeapFree((void *)paletteRecord->rampEntries, "C:\\lib\\src\\graphics\\dx_pal.cpp", 0x8C);
        // STRING: FF7 0x009130f0
        GameHeapFree((void *)paletteRecord->block, "C:\\lib\\src\\graphics\\dx_pal.cpp", 0x8D);
        if (paletteRecord->ownsEntries != 0 && paletteRecord->srcEntries != 0) {
            // STRING: FF7 0x00913110
            GameHeapFree((void *)paletteRecord->srcEntries, "C:\\lib\\src\\graphics\\dx_pal.cpp", 0x91);
        }
        if (paletteRecord->d3dEntries != 0) {
            if (paletteRecord->releaseCallback != 0) {
                ((ComUnknown *)(paletteRecord->releaseCallback))->Release();
                paletteRecord->releaseCallback = 0;
            }
            // STRING: FF7 0x00913130
            GameHeapFree((void *)paletteRecord->d3dEntries, "C:\\lib\\src\\graphics\\dx_pal.cpp", 0x9C);
        }
        // STRING: FF7 0x00913150
        GameHeapFree((void *)paletteRecord->arraysPtr, "C:\\lib\\src\\graphics\\dx_pal.cpp", 0x9F);
        // STRING: FF7 0x00913170
        GameHeapFree(paletteRecord, "C:\\lib\\src\\graphics\\dx_pal.cpp", 0xA0);
    }
}

/*
 * Creates palette record and initializes color buffers and surfaces.
 */
// FUNCTION: FF7 0x006a61ae
int __cdecl Gfx_CreatePaletteRecord(int flags, int bitDepth, int entriesPerGroup, int entryCount, int srcEntries)
{
    GfxPaletteRecord *recordPtr;
    int allocCount;
    int state;
    GfxEngineContext *engine;
    int i;

    state = 1;
    // STRING: FF7 0x00913190
    recordPtr = (GfxPaletteRecord *)GameHeapAlloc(1, 0x40, "C:\\lib\\src\\graphics\\dx_pal.cpp", 0xAB);
    if (recordPtr != 0) {
        recordPtr->flags = flags;
        recordPtr->bitDepth = bitDepth;
        recordPtr->entryCount = entryCount;
        recordPtr->entriesPerGroup = entriesPerGroup;
        recordPtr->groupCount = entryCount / entriesPerGroup;
        if (entryCount % entriesPerGroup != 0) {
            (recordPtr->groupCount)++;
        }
        allocCount = entryCount;
        if (1 << bitDepth > allocCount) {
            allocCount = 1 << bitDepth;
        }
        // STRING: FF7 0x009131b0
        recordPtr->d3dEntries = (int)GameHeapAlloc(allocCount, 4, "C:\\lib\\src\\graphics\\dx_pal.cpp", 0xC0);
        // STRING: FF7 0x009131d0
        recordPtr->rampEntries = (int)GameHeapAlloc(allocCount, 4, "C:\\lib\\src\\graphics\\dx_pal.cpp", 0xC1);
        // STRING: FF7 0x009131f0
        recordPtr->block = (int)GameHeapAlloc(allocCount, 4, "C:\\lib\\src\\graphics\\dx_pal.cpp", 0xC2);
        if (recordPtr->d3dEntries != 0 && recordPtr->rampEntries != 0 && recordPtr->block != 0) {
            if (srcEntries != 0) {
                recordPtr->srcEntries = srcEntries;
                state = 0;
            } else {
                recordPtr->ownsEntries = 1;
                // STRING: FF7 0x00913210
                recordPtr->srcEntries = (int)GameHeapAlloc(entryCount, 4, "C:\\lib\\src\\graphics\\dx_pal.cpp", 0xCD);
                if (recordPtr->srcEntries != 0) {
                    state = 0;
                } else {
                    state = 1;
                }
            }
            if (state == 0 && (recordPtr->flags & GFX_PALETTE_FLOAT_ENTRIES) != 0) {
                // STRING: FF7 0x00913230
                recordPtr->floatEntries = (int)GameHeapAlloc(entryCount, 0x10, "C:\\lib\\src\\graphics\\dx_pal.cpp", 0xDA);
                if (recordPtr->floatEntries != 0) {
                    if (srcEntries != 0) {
                        Gfx_ConvertPaletteEntriesToFloat(entryCount, 0, srcEntries, 0, recordPtr->floatEntries);
                    }
                    state = 0;
                } else {
                    state = 1;
                }
            }
            if (state == 0 && (recordPtr->flags & GFX_PALETTE_COLOR_ENTRIES) != 0) {
                // STRING: FF7 0x00913250
                recordPtr->colorEntries = (int)GameHeapAlloc(entryCount, 4, "C:\\lib\\src\\graphics\\dx_pal.cpp", 0xEC);
                if (recordPtr->colorEntries != 0) {
                    if (srcEntries != 0) {
                        for (i = 0; i < entryCount; i++) {
                            ((PaletteColor *)recordPtr->colorEntries)[i] = ((PaletteColor *)srcEntries)[i];
                        }
                    }
                    state = 0;
                } else {
                    state = 1;
                }
            }
            if (state == 0) {
                if (srcEntries != 0) {
                    Gfx_CopyPaletteEntriesD3D(entryCount, 0, srcEntries, 0, recordPtr->d3dEntries);
                    Gfx_MapPaletteEntriesViaRampTables(entryCount, 0, srcEntries, 0, recordPtr->rampEntries);
                    Gfx_ConvertPaletteEntriesToFloat(entryCount, 0, srcEntries, 0, recordPtr->floatEntries);
                }
                engine = GetEngineContext();
                if (engine->driverType == GFX_DRIVER_DIRECTX) {
                    if (D3DCreatePaletteObject(bitDepth, recordPtr) != 0) { /* empty first arm: lib/codegen.h */
                    } else {
                        state = 1;
                    }
                }
            }
        }
    }
    if (state != 0) {
        Gfx_FreePaletteRecord(recordPtr);
        recordPtr = 0;
    }
    return (int)recordPtr;
}

/*
 * Sets palette entries on DirectDraw palette object.
 */
// FUNCTION: FF7 0x006a64c0
int __cdecl D3DPaletteSetEntries(int bufferIndex, int count, int startEntry, GfxPaletteState *palette)
{
    int result;
    int flags;

    result = 0;
    if (palette != 0) {
        flags = 0;
        // STRING: FF7 0x00913270
        if (CheckDirectDrawResult(((ComDirectDrawPalette *)(palette->ddPalette))->SetEntries(flags, startEntry, count, palette->entryBuffer + bufferIndex * 4), "C:\\lib\\src\\graphics\\dx_pal.cpp", 0x133) != 0) {
            result = 1;
        }
    }
    return result;
}

/*
 * Notifies texture palette update and updates Direct3D render state.
 */
// FUNCTION: FF7 0x006a652a
int __cdecl D3DTexturePaletteChanged(int entryOffset, int count, int startEntry, int palette, GfxTextureSurface *texture)
{
    int changed;
    int hresult;
    GfxEngineContext *engine;

    changed = 0;
    if (palette != 0 && texture != 0 && texture->d3dTexture != 0) {
        engine = GetEngineContext();
        if (engine->useDirect3D5 != 0 && engine->d3dDevice2 != 0) {
            ((ComDirect3DDevice2 *)(engine->d3dDevice2))->SetRenderState(D3DRENDERSTATE_FLUSHBATCH, 0);
        }
        if (texture != 0 && texture->surface != 0) {
            hresult = ((ComDirectDrawSurface *)(texture->surface))->IsLost();
            if (hresult == DDERR_SURFACELOST) {
                texture = Gfx_RestoreTextureSurfaces(texture);
            } else if (engine->fallbackFlag != 0) {
                texture = (GfxTextureSurface *)engine->driverObject->loadTexture((int)texture, texture->header, texture->format);
                if (engine->useDirect3D5 != 0) {
                    ((ComDirect3DDevice2 *)(engine->d3dDevice2))->SetRenderState(D3DRENDERSTATE_TEXTUREHANDLE, texture->handle);
                }
            }
            if (texture != 0) {
                palette = texture->palette;
            } else {
                palette = 0;
            }
        }
        if (D3DPaletteSetEntries(entryOffset, count, startEntry, (struct GfxPaletteState *)palette) != 0 && engine->fallbackFlag == 0) {
            // STRING: FF7 0x00913290
            if (CheckDirectDrawResult(((ComDirect3DTexture2 *)(texture->d3dTexture))->PaletteChanged(startEntry, count), "C:\\lib\\src\\graphics\\dx_pal.cpp", 0x174) != 0) {
                if (g_GfxPaletteUpdateFlag != 0 && engine->useDirect3D5 != 0) {
                    ((ComDirect3DDevice2 *)(engine->d3dDevice2))->SetRenderState(D3DRENDERSTATE_TEXTUREHANDLE, texture->handle);
                }
                changed = 1;
            }
        }
    }
    return changed;
}
/*
 * Writes palette entries to DirectDraw palette via Gfx_CopyPaletteEntriesD3D.
 */
// FUNCTION: FF7 0x006a66f5
int __cdecl Gfx_WritePaletteEntriesD3D(int srcStart, int count, int src, int dstStart, GfxPaletteRecord *record)
{
    int written;

    written = 0;
    if (record != 0) {
        Gfx_CopyPaletteEntriesD3D(count, srcStart, src, dstStart, record->d3dEntries);
        written = 1;
    }
    return written;
}
} /* extern "C" */
