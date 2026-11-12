// src/lib/src/graphics/gl/gl.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: OpenGL driver back end: context creation and teardown, pixel-format and texture-format
//   publishing, and the per-operation entry points of the GL driver table for surfaces, viewports,
//   clears, textures, render states, blending, polygon sets and scenes.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/api/mem.h"
#include "lib/types/graphics.h"
#include "lib/types/polygon.h"
#include "lib/codegen.h"
#include "lib/api/graphics.h"
#include "lib/crt.h"
#include "lib/enums/graphics.h"
extern "C" {


// clang-format off
// clang-format on

/* The blend descriptor Gl_OpsGetBlendDescriptor hands back: only the alpha is ever written. */
typedef struct {
    int pad_00;
    int pad_04;
    int alpha;
    int pad_0C[7];
} GlBlendDescriptor;

// clang-format off
// GLOBAL: FF7 0x00919608
unsigned char g_GlTextureFormatTemplates[0x30] = {
    0x01, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00,
    0xff, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0xff
};
// GLOBAL: FF7 0x00919638
GlBlendDescriptor g_GlBlendDescriptor = {1, 0, 0, {0, 0, 0, 0, 0, 0, 0}};
// GLOBAL: FF7 0x00919660
float g_GlTransformMatrix[16] = {1.0f, 0, 0, 0, 0, 1.0f, 0, 0, 0, 0, 1.0f, 0, 0, 0, 0, 1.0f};
// clang-format on


__declspec(dllimport) void __stdcall glFlush(void);
__declspec(dllimport) void __stdcall glFinish(void);
__declspec(dllimport) void __stdcall glViewport(int x, int y, int w, int h);
__declspec(dllimport) void __stdcall glMatrixMode(unsigned int mode);
__declspec(dllimport) void __stdcall glLoadIdentity(void);
__declspec(dllimport) void __stdcall glOrtho(double l, double r, double b,
                                             double t, double n, double f);
__declspec(dllimport) void *__stdcall GetDC(void *hwnd);
__declspec(dllimport) int __stdcall ReleaseDC(void *hwnd,
                                              void *hdc);
__declspec(dllimport) int __stdcall wglMakeCurrent(void *hdc,
                                                   void *rc);
/* Unprototyped retail call signature (parameter/return width differs from callee definition). */
extern void __cdecl Gl_ClearBuffers(int a, int b, int c,
                                    int d, int e, int f);

/*
 * One dword out of the device at +0xA7C. The frame is four bytes and the value
 * is stored and read straight back, so the original spelled it as a local
 * rather than returning the load.
 */
// FUNCTION: FF7 0x006b3d10
void *__cdecl Gl_GetState(struct GfxEngineContext *dev)
{
    void *st;

    st = dev->glState;
    return st;
}

/*
 * The lookup table's colour arrays at +0x3C, or null when it has none. `cmp
 * dword ptr [ebp+8], 0` tests the parameter in place rather than through a
 * register, which is the null check written on the pointer itself.
 */
// FUNCTION: FF7 0x006b3d27
void *__cdecl Gl_GetPaletteColorArrays(GfxPaletteRecord *lut)
{
    void *arrays;

    arrays = 0;
    if (lut != 0) {
        arrays = (void *)lut->arraysPtr;
    }
    return arrays;
}

/*
 * Gl_GetPaletteColorArrays against a different member: the texture's name
 * table at +0x9C, or null when there is no texture.
 */
// FUNCTION: FF7 0x006b3d48
void *__cdecl Gl_GetTextureNameTable(void *tex)
{
    void *names;

    names = 0;
    if (tex != 0) {
        names = *(void **)&((GfxTextureSurface *)tex)->glNameTable;
    }
    return names;
}

/*
 * Swap a dword into the GL state at +0x44C and hand the old one back. `st` and
 * `old` are the spellings that land at [ebp-4] and [ebp-8] in that order; the
 * names carry no meaning beyond the layout.
 */
// FUNCTION: FF7 0x006b3d6c
int __cdecl Gl_SetUnusedStateFlag(int value, struct GfxEngineContext *dev)
{
    GlContext *st;
    int old;

    st = (GlContext *)Gl_GetState(dev);
    old = st->unusedStateFlag;
    st->unusedStateFlag = value;
    return old;
}

/*
 * Walks two 0x18-byte texture-format templates, allocates and initializes each
 * successful record, and inserts it into the graphics device. Only the first
 * template is gated by GL state +0x444; the second is inserted whenever
 * allocated.
 */
// FUNCTION: FF7 0x006b3da0
void __cdecl Gl_PublishTextureFormats(struct GfxEngineContext *dev)
{
    GlContext *ctx;
    void *fmt;
    int use;
    int cnt;
    unsigned char *tmpl;
    void *made;
    void *have;
    void *alt;

    ctx = (GlContext *)Gl_GetState(dev);
    tmpl = g_GlTextureFormatTemplates;
    for (cnt = 0; cnt < 2; cnt++) {
        made = (void *)AllocTextureFormatRecord();
        fmt = made;
        have = fmt;
        if (have != 0) {
            PixelFormatLoadMaskTemplate((GfxPixelMaskTemplate *)tmpl, (GfxPixelFormat *)have);
            use = 1;
            if (cnt == 0) {
                if (ctx->hasColorTableExt != 0) {
                    use = 1;
                } else {
                    use = 0;
                }
            }
            if (use != 0) {
                InsertTextureFormatRecord((GfxTextureFormatRecord *)fmt, dev);
            }
        }
        tmpl = tmpl + 0x18;
    }
    if (DEAD_IN_RETAIL) {
        alt = (void *)AllocTextureFormatRecord();
        fmt = alt;
        have = fmt;
        if (have != 0) {
            *(GfxPixelFormat *)fmt = dev->targetFormat;
            InsertTextureFormatRecord((GfxTextureFormatRecord *)fmt, dev);
        }
    }
}

/*
 * Clears the destination pixel-format block. For PFD type zero, sums component
 * widths and derives RGB/alpha masks from their widths and shifts; for type
 * one, sets indexed-format flag and bit depth.
 */
// FUNCTION: FF7 0x006b3e96
void __cdecl Gl_PixelFormatFromPfd(GlPixelFormatDescriptor *pfd, GfxPixelFormat *format)
{
    int bitCount;

    ZeroFillMemory32(0x80, format);
    if (pfd->pixelType == 0) {
        bitCount = pfd->redBits + pfd->greenBits + pfd->blueBits + pfd->alphaBits;
        PixelFormatSetRgbMasks(bitCount, (1 << pfd->redBits) - 1 << pfd->redShift, (1 << pfd->greenBits) - 1 << pfd->greenShift, (1 << pfd->blueBits) - 1 << pfd->blueShift, (1 << pfd->alphaBits) - 1 << pfd->alphaShift, format);
    } else if (pfd->pixelType == 1) {
        format->palettized = 1;
        format->indexBits = pfd->colorBits;
    }
}

/*
 * Destroy the device's GL context. The load at +0xA7C is exactly what
 * Gl_GetState does, and this does not call it -- the source repeats the member
 * access rather than the accessor, which the absence of a call and the
 * presence of the inline load together settle.
 */
// FUNCTION: FF7 0x006b3f9d
void __cdecl Gl_OpsCleanup(struct GfxEngineContext *dev)
{
    GlContext *st;

    st = dev->glState;
    Gl_DestroyContext(st);
}

/*
 * Create the device's GL context if it has none, then publish the pixel format
 * and the texture formats. The frame is 0x14 and only four slots are read,
 * which reads like a hole at [ebp-8] and is not one.
 */
// FUNCTION: FF7 0x006b3fbd
int __cdecl Gl_OpsInit(struct GfxEngineContext *dev)
{
    GlContextConfig cfg;
    GlContext *st;
    int ok;

    ok = 0;
    st = dev->glState;
    if (st == 0) {
        Gl_InitConfigDefaults((GlConfig *)&cfg);
        cfg.flags = dev->fullScreen;
        st = Gl_CreateContext((GlContextConfig *)&cfg, dev);
        if (st != 0) {
            dev->glState = st;
        }
    }
    if (st != 0) {
        Gl_PixelFormatFromPfd(&st->pixelFormat, &dev->targetFormat);
        Gl_PublishTextureFormats(dev);
        ok = 1;
    }
    return ok;
}

/*
 * A lock that never locks anything: it rewrites surface 4 as surface 1,
 * records that it did, copies the surface index into a third local and returns
 * the zero it started with. The parameter is written in place -- `mov dword
 * ptr [ebp+8], 1` -- which is a parameter used as a local, and the two
 * remaining stores go to slots nothing reads again.
 */
// FUNCTION: FF7 0x006b4051
int __cdecl Gl_OpsLockSurface(int which, void *dev)
{
    int result;
    int flag;
    int sel;

    result = 0;
    flag = 0;
    if (which == 4) {
        which = 1;
        flag = 1;
    }
    sel = which;
    return result;
}

/*
 * The GL driver's surface-unlock entry point: nothing to unlock, always
 * succeeds.
 */
// FUNCTION: FF7 0x006b4086
int __cdecl Gl_OpsUnlockSurface(void)
{
    return 1;
}

/*
 * Flip the caller's top-left viewport into GL's bottom-left one and set up a
 * matching orthographic projection. The frame is 0x14 for FOUR named locals.
 */
// FUNCTION: FF7 0x006b4090
void __cdecl Gl_OpsSetViewport(int x, int y, int w, int h, struct GfxEngineContext *dev)
{
    int ytop;
    int scrH;
    int bot;
    int top;

    glFinish();
    ytop = dev->displayHeight - (y + h);
    glViewport(x, ytop, w, h);
    scrH = dev->displayHeight;
    top = scrH - (dev->clipTop + dev->viewSizeY);
    bot = scrH - dev->clipTop;
    glMatrixMode(0x1700);
    glLoadIdentity();
    glMatrixMode(0x1701);
    glLoadIdentity();
    glOrtho((double)dev->clipLeft,
            (double)(dev->clipLeft + dev->viewSizeX),
            (double)bot, (double)top, 1.0, -1.0);
}

/*
 * Clears colour, depth and stencil at once through Gl_OpsClear(1, 1, 1,
 * value).
 */
// FUNCTION: FF7 0x006b4174
void __cdecl Gl_OpsClearAll(int value)
{
    Gl_OpsClear(1, 1, 1, (GfxEngineContext *)value);
}

/*
 * Gets GL state and the window DC and attempts to make its GL context current.
 * Releases the DC only when both acquisition and make-current succeed, then
 * clears buffers regardless of either failure using state color fields and
 * supplied mask/flags.
 */
// FUNCTION: FF7 0x006b418b
void __cdecl Gl_OpsClear(int mask, int flags, int unused, struct GfxEngineContext *dev)
{
    GlContext *ctx;
    void *hdc;

    ctx = (GlContext *)Gl_GetState(dev);
    if (1) { /* Constant condition kept for /Od branch layout; see lib/codegen.h. */
        hdc = GetDC((void *)dev->hwnd);
        if (hdc != 0) {
            if (wglMakeCurrent(hdc, ctx->renderContext) != 0) {
                ReleaseDC((void *)dev->hwnd, hdc);
            }
        }
    }
    Gl_ClearBuffers(ctx->background.b, ctx->background.g,
                    ctx->background.r, ctx->background.a,
                    mask, flags);
}

/*
 * Copy the caller's four-dword colour into the GL state at +8. Both bases are
 * loaded once and then four load/store pairs run off them, which is a 16-byte
 * struct assignment and not four scalar ones: four separate assignments at /Od
 * would reload both pointers each time, the way Gfx_ApplyBlendRecordToHundred
 * does.
 */
// FUNCTION: FF7 0x006b4215
void __cdecl Gl_OpsSetBackgroundColor(Rgba *color, struct GfxEngineContext *dev)
{
    GlContext *st;

    st = (GlContext *)Gl_GetState(dev);
    st->background = *color;
}

/*
 * Give back everything one texture record owns: the GL name or the whole name
 * table, and the palette's colour arrays. Gl_GetTextureNameTable is called
 * twice on the same record rather than through a local, which is the tell that
 * the source calls it twice.
 */
// FUNCTION: FF7 0x006b424b
void __cdecl Gl_OpsUnloadTexture(GfxTextureSurface *tex)
{
    GfxPaletteRecord *lut;
    void *arrays;

    if (tex != 0) {
        if (Gl_GetTextureNameTable(tex) == 0) {
            if (tex->handle != 0) {
                Gl_DeleteTexture((unsigned int)(*(void **)&tex->handle));
            }
        } else {
            Gl_FreeTextureNameTable((GlTextureNameTable *)Gl_GetTextureNameTable(tex));
        }
        tex->handle = 0;
        lut = (GfxPaletteRecord *)tex->palette;
        if (lut != 0) {
            arrays = Gl_GetPaletteColorArrays(lut);
            if (arrays != 0) {
                Gl_FreeColorArrays((GlColorArrays *)arrays);
                lut->arraysPtr = 0;
            }
        }
    }
}

/*
 * The 68-byte frame has two four-byte colors and fifteen scalar DWORDs. The
 * original unaligned DWORD channel reads extend into the adjacent scalar, then
 * retain only their low byte.
 */
// FUNCTION: FF7 0x006b42e5
void __cdecl Gl_ConvertTexturePixels(int flipVertical, int indexed, GfxTexHeader *convertParams, GfxPixelFormat *texDesc, int dstPixels)
{
    struct {
        int p68, p64, p60, p56, p52, p48, p44;
        GfxArgb32 rgb;
        int p36, p32, p28, p24, p20, p16;
        GfxArgb32 color;
        int p8, p4;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    if (convertParams != 0) {
        if (indexed != 0 && convertParams->format.bytesPerPixel != 1) {
            indexed = 0;
        }
        frame.p8 = 0;
        frame.p24 = 0;
        if (convertParams->format.bytesPerPixel == 1) {
            if (convertParams->paletteMode != 0) {
                frame.p8 = *(unsigned char *)((char *)convertParams->colorLookup + convertParams->row);
            } else if (convertParams->colorKey != 0) {
                frame.p8 = 1;
            }
        } else if (convertParams->colorKey != 0) {
            frame.p8 = 1;
        }
        frame.p20 = convertParams->pixelBuffer;
        if (flipVertical != 0) {
            frame.p4 = dstPixels + (convertParams->format.height - 1) * texDesc->pitch;
            frame.p16 = -texDesc->pitch;
        } else {
            frame.p4 = dstPixels;
            frame.p16 = texDesc->pitch;
        }
        for (frame.p32 = 0; frame.p32 < convertParams->format.height; frame.p32++) {
            frame.p48 = frame.p4;
            if (indexed != 0) {
                for (frame.p28 = 0; frame.p28 < convertParams->format.width; frame.p28++) {
                    ((GlTexelDest *)frame.p48)->blue = ((GlTexelDest *)frame.p20)->blue;
                    frame.p20 += convertParams->format.bytesPerPixel;
                    frame.p48 += texDesc->bytesPerPixel;
                }
            } else {
                for (frame.p28 = 0; frame.p28 < convertParams->format.width; frame.p28++) {
                    /* Numeric pixel stride in bytes (1..4). */
                    switch (convertParams->format.bytesPerPixel) {
                    case GFX_BYTES_PER_PIXEL_1:
                        frame.p44 = *(unsigned char *)&((GlTexelDest *)frame.p20)->blue;
                        frame.p56 = UnpackPixelRowToArgb8888(frame.p44, convertParams->row, (GfxPixelFormat *)((int)convertParams + 0x3C));
                        frame.color.packed = frame.p56;
                        if (frame.p44 == frame.p24 && frame.p8 != 0) {
                            frame.color.ch.a = 0;
                        }
                        break;
                    case GFX_BYTES_PER_PIXEL_2:
                        frame.p60 = UnpackPixelToArgb8888(*(unsigned short *)frame.p20, &convertParams->format);
                        frame.color.packed = frame.p60;
                        break;
                    case GFX_BYTES_PER_PIXEL_3:
                        *(char *)&frame.rgb.packed = ((GlTexelDest *)frame.p20)->blue;
                        frame.rgb.ch.g = ((GlTexelDest *)frame.p20)->green;
                        frame.rgb.ch.r = ((GlTexelDest *)frame.p20)->red;
                        frame.rgb.ch.a = 0;
                        frame.p64 = UnpackPixelToArgb8888(frame.rgb.packed, &convertParams->format);
                        frame.color.packed = frame.p64;
                        break;
                    case GFX_BYTES_PER_PIXEL_4:
                        frame.p68 = UnpackPixelToArgb8888(*(int *)frame.p20, &convertParams->format);
                        frame.color.packed = frame.p68;
                    }
                    if ((unsigned char)*(int *)&frame.color.ch.a != 0) {
                        frame.p52 = convertParams->modeFlag == 0;
                        if ((unsigned char)*(int *)&frame.color.ch.a == 0xFE && convertParams->alphaValue >= 0) {
                            frame.color.ch.a = *(char *)&convertParams->alphaValue;
                        }
                        // Folded VC5 condition advances register selection without emitted code.
                        if (REG_ALLOC_HINT) {
                        }
                        if (frame.p52 != 0 && (unsigned char)*(int *)&frame.color.ch.r < 1 && (unsigned char)*(int *)&frame.color.ch.g < 1 && (unsigned char)frame.color.packed < 1) {
                            frame.color.ch.r = 1;
                            frame.color.ch.g = 1;
                            *(char *)&frame.color.packed = 1;
                        }
                    }
                    frame.p36 = PackArgb8888ToPixelFormat(frame.color, texDesc);
                    /* Numeric pixel stride in bytes (1..4). */
                    switch (texDesc->bytesPerPixel) {
                    case GFX_BYTES_PER_PIXEL_1:
                        ((GlTexelDest *)frame.p48)->blue = *(char *)&frame.p36;
                        break;
                    case GFX_BYTES_PER_PIXEL_2:
                        *(short *)frame.p48 = *(short *)&frame.p36;
                        break;
                    case GFX_BYTES_PER_PIXEL_3:
                        ((GlTexelDest *)frame.p48)->blue = *(char *)&frame.color.packed;
                        ((GlTexelDest *)frame.p48)->green = frame.color.ch.g;
                        ((GlTexelDest *)frame.p48)->red = frame.color.ch.r;
                        break;
                    case GFX_BYTES_PER_PIXEL_4:
                        *(int *)frame.p48 = frame.p36;
                    }
                    frame.p20 += convertParams->format.bytesPerPixel;
                    frame.p48 += texDesc->bytesPerPixel;
                }
            }
            frame.p4 += frame.p16;
        }
    }
}

/*
 * Returns the texture-object pointer, allocating it when absent and linking it
 * to the texture set and image. Compares format fields, optionally creates
 * palette conversion and color arrays, reuses or allocates pixel storage and
 * converts pixels with palette and transparency flags.
 */
// FUNCTION: FF7 0x006b466f
int __cdecl Gl_OpsLoadTexture(GfxTextureSurface *texture, GfxTexHeader *convertParams, int srcDesc)
{
    GfxEngineContext *ctx;
    int flag;
    int state;
    GfxPixelFormat *pTexDesc;
    GfxPixelFormat *convDescPtr;
    GfxPaletteRecord *palRecord;
    int status;
    GlColorArrays *arraysPtr;
    int storage;
    int mode;
    int paletted;
    int changed;

    state = 1;
    flag = 0;
    ctx = GetEngineContext();
    if (ctx != 0 && srcDesc != 0) {
        if (texture == 0) {
            texture = (GfxTextureSurface *)Gfx_AllocSurfaceRecord();
            if (texture != 0 && convertParams->paletteCount > 0) {
                texture->glNameTable = Gl_AllocTextureNameTable(convertParams->paletteCount);
            }
        }
        if (texture != 0) {
            texture->header = (int)convertParams;
            texture->format = srcDesc;
            convDescPtr = &convertParams->format;
            pTexDesc = (GfxPixelFormat *)srcDesc;
            if (convDescPtr->palettized == pTexDesc->palettized && convDescPtr->indexBits == pTexDesc->indexBits && convDescPtr->indexedTo8 == pTexDesc->indexedTo8 && convDescPtr->bitsPerPixel == pTexDesc->bitsPerPixel && convDescPtr->bytesPerPixel == pTexDesc->bytesPerPixel && convDescPtr->redMask == pTexDesc->redMask && convDescPtr->greenMask == pTexDesc->greenMask && convDescPtr->blueMask == pTexDesc->blueMask && convDescPtr->alphaMask == pTexDesc->alphaMask) {
                changed = 0;
            } else {
                changed = 1;
            }
            if (((GfxPixelFormat *)srcDesc)->palettized != 0) {
                palRecord = (GfxPaletteRecord *)(Gfx_CreateSurfacePaletteRecord(((GfxPixelFormat *)srcDesc)->bitsPerPixel, convertParams, texture));
                if (palRecord != 0) {
                    texture->palette = (int)palRecord;
                    status = (int)Gl_GetState(ctx);
                    arraysPtr = (GlColorArrays *)(Gl_AllocColorArrays(convertParams->format.paletteEntryCount));
                    if (arraysPtr != 0) {
                        Gl_BuildColorArrays(convertParams->format.paletteEntryCount, 0, (GlColorBytes *)(convertParams->format.palette), arraysPtr, status);
                        ((GfxPaletteRecord *)palRecord)->arraysPtr = (int)arraysPtr;
                        state = 0;
                    }
                } else {
                    state = 1;
                }
            } else {
                state = 0;
            }
            if (state == 0) {
                if (convertParams->storage != 0) {
                    storage = convertParams->storage;
                } else {
                    // STRING: FF7 0x009196a0
                    storage = (int)GameHeapAlloc(convDescPtr->width * convDescPtr->height * pTexDesc->bytesPerPixel, 1, "C:\\lib\\src\\graphics\\gl\\gl.cpp", 0x29F);
                }
                if (storage != 0) {
                    pTexDesc->pitch = convDescPtr->width * pTexDesc->bytesPerPixel;
                    if (changed == 0 && convDescPtr->palettized != 0) {
                        paletted = 1;
                    } else {
                        paletted = 0;
                    }
                    if ((convertParams->flags & 1) != 0) {
                        flag = 1;
                    }
                    Gl_ConvertTexturePixels(flag, paletted, convertParams, pTexDesc, storage);
                    convertParams->storage = storage;
                } else {
                    state = 1;
                }
            }
            if (state == 0 && convertParams->storage != 0) {
                mode = ((GfxPixelFormat *)srcDesc)->palettized;
                texture->handle = Gl_CreateTexture2D(mode, pTexDesc->bytesPerPixel, convDescPtr->width, convDescPtr->height, convertParams->storage);
                Gl_SetTextureName(texture->handle, convertParams->row, (int)Gl_GetTextureNameTable(texture));
            }
            if (state != 0) {
                Gfx_ReleaseTextureRecordRef((int)texture);
                texture = 0;
            }
        }
    }
    return (int)texture;
}

/*
 * Set the alpha of the one shared blend descriptor and hand it back. Four
 * modes and a default: 0x80 for the first, 0x40 for the fourth and 0xFF for
 * everything else, including two of the four table entries.
 */
// FUNCTION: FF7 0x006b4996
void *__cdecl Gl_OpsGetBlendDescriptor(int mode)
{
    GlBlendDescriptor *desc;

    desc = &g_GlBlendDescriptor;
    switch (mode) {
    case BLEND_AVERAGE:
        desc->alpha = 0x80;
        break;
    case BLEND_ADDITIVE:
        desc->alpha = 0xff;
        break;
    case BLEND_SUBTRACTIVE:
        desc->alpha = 0xff;
        break;
    case BLEND_QUARTER_ADDITIVE:
        desc->alpha = 0x40;
        break;
    default:
        desc->alpha = 0xff;
        break;
    }
    return desc;
}

/*
 * Write one byte into the render-state block the draw context carries at
 * +0x944, addressed by the caller's index. `mov al, byte ptr [ebp+0xc]` reads
 * the value parameter un-promoted, which pins the parameter's own type to a
 * byte -- an `int` parameter would have been stored and read as a dword
 */
// FUNCTION: FF7 0x006b4a0a
void __cdecl Gl_OpsSetRenderStateByte(int index, unsigned char value, struct GfxEngineContext *out)
{
    unsigned char *rs;

    rs = (unsigned char *)out->presetPolygonSets;
    if (rs != 0) {
        rs[index] = value;
    }
}

/*
 * A two-argument forwarder to Gl_ApplyRenderState with no frame of its own --
 * no `push ecx`, so no local, and the two parameters go straight back out in
 * the order they arrived.
 */
// FUNCTION: FF7 0x006b4a2f
void __cdecl Gl_OpsApplyRenderStateB(struct GlDrawGroup *state, struct GfxEngineContext *out)
{
    Gl_ApplyRenderState(state, out);
}

/*
 * The second of the two identical forwarders; both are ops-table slots that do
 * nothing but call Gl_ApplyRenderState with their own arguments.
 */
// FUNCTION: FF7 0x006b4a44
void __cdecl Gl_OpsApplyRenderStateC(struct GlDrawGroup *state, struct GfxEngineContext *out)
{
    Gl_ApplyRenderState(state, out);
}

/*
 * A five-way jump table over the blend mode, one setter per arm. The frame is
 * four bytes and the mode is stored to [ebp-4], compared against 4 there and
 * indexed out of it -- so that slot is the switch's own temporary and the
 * function has no named local at all.
 */
// FUNCTION: FF7 0x006b4a59
void __cdecl Gl_ApplyBlendMode(int mode, struct GfxEngineContext *dev)
{
    switch (mode) {
    case BLEND_AVERAGE:
        Gl_SetBlendModeAverage();
        break;
    case BLEND_ADDITIVE:
        Gl_SetBlendModeAdditive();
        break;
    case BLEND_SUBTRACTIVE:
        Gl_SetBlendModeSubtractive((GlContext *)Gl_GetState(dev));
        break;
    case BLEND_QUARTER_ADDITIVE:
        Gl_SetBlendModeQuarterAdd();
        break;
    case BLEND_NONE:
        Gl_SetBlendModeAlphaTest();
        break;
    }
}

/*
 * Record the mode in the render-state block at +0x944 and apply it. The apply
 * reads the mode back out of the block rather than reusing the parameter: `mov
 * eax, dword ptr [edx + 0x20]` after the store, where passing `mode` again
 * would have been `mov eax, dword ptr [ebp + 8]`.
 */
// FUNCTION: FF7 0x006b4abc
void __cdecl Gl_OpsSetBlendMode(int mode, struct GfxEngineContext *out)
{
    GlRenderState *rs;

    rs = (GlRenderState *)out->presetPolygonSets;
    if (rs != 0) {
        rs->blendMode = mode;
        Gl_ApplyBlendMode(rs->blendMode, out);
    }
}

/*
 * Applies the change mask at drawGroup+0xC to cached OpenGL state and selected
 * texture metadata. The broad-mask branch updates texture selection, dither,
 * filtering, perspective correction, culling, depth test/write, alpha
 * metadata, blend mode, shading and wrapping.
 */
// FUNCTION: FF7 0x006b4af2
void __cdecl Gl_ApplyRenderState(GlDrawGroup *drawGroup, struct GfxEngineContext *context)
{
    GfxPaletteRecord *palInfo;
    int enabled;
    GlRenderState *renderState;
    int changeMask;
    GfxTexHeader *texRecord;
    GfxTextureSurface *batchRef;
    GfxPaletteRecord *palRecord;
    GfxTexHeader *textureDesc;
    GfxTextureSurface *gbatch;

    if (drawGroup != 0) {
        changeMask = drawGroup->changeMask;
        renderState = (GlRenderState *)(context->presetPolygonSets);
        if ((changeMask & 0xFFFDFBFD) != 0) {
            if ((changeMask & GFX_DRAW_GROUP_TEXTURED) != 0) {
                batchRef = (GfxTextureSurface *)(drawGroup->batch);
                renderState->batch = (int)batchRef;
                if (batchRef != 0 && (drawGroup->flags & GFX_DRAW_GROUP_TEXTURED) != 0) {
                    texRecord = (GfxTexHeader *)batchRef->header;
                    palInfo = (GfxPaletteRecord *)(batchRef->palette);
                    if (palInfo != 0) {
                        if (palInfo->paletteMode != 0) {
                            if ((((unsigned char *)palInfo->colorLookup)[batchRef->paletteRow] & 1) != 0) {
                                enabled = 1;
                            } else {
                                enabled = 0;
                            }
                        } else if (texRecord->colorKey != 0) {
                            enabled = 1;
                        } else {
                            enabled = 0;
                        }
                    } else if (texRecord->colorKey != 0) {
                        enabled = 1;
                    } else {
                        enabled = 0;
                    }
                    renderState->colorKeyEnabled = enabled;
                }
            }
            if ((changeMask & GFX_DRAW_GROUP_DITHER) != 0) {
                if ((drawGroup->flags & GFX_DRAW_GROUP_DITHER) != 0) {
                    enabled = 1;
                } else {
                    enabled = 0;
                }
                renderState->ditherEnabled = enabled;
                Gl_SetDither(enabled);
            }
            if ((changeMask & GFX_DRAW_GROUP_LINEAR_FILTER) != 0) {
                if ((drawGroup->flags & GFX_DRAW_GROUP_LINEAR_FILTER) != 0) {
                    Gl_SetTextureFilter(1);
                } else {
                    Gl_SetTextureFilter(0);
                }
            }
            if ((changeMask & GFX_DRAW_GROUP_PERSPECTIVE) != 0) {
                if ((drawGroup->flags & GFX_DRAW_GROUP_PERSPECTIVE) != 0) {
                    enabled = 1;
                } else {
                    enabled = 0;
                }
                renderState->perspectiveCorrection = enabled;
                Gl_SetPerspectiveCorrection(enabled);
            }
            if ((changeMask & GFX_DRAW_GROUP_NO_CULL) != 0) {
                if ((drawGroup->flags & GFX_DRAW_GROUP_NO_CULL) != 0) {
                    renderState->cullingDisabled = 1;
                    Gl_DisableCulling();
                } else if ((changeMask & GFX_DRAW_GROUP_CULL_FRONT) != 0) {
                    if ((drawGroup->flags & GFX_DRAW_GROUP_CULL_FRONT) != 0) {
                        renderState->cullFrontFaces = 1;
                        Gl_CullFrontFaces();
                    } else {
                        renderState->cullFrontFaces = 0;
                        Gl_CullBackFaces();
                    }
                }
            } else if ((changeMask & GFX_DRAW_GROUP_CULL_FRONT) != 0) {
                if ((drawGroup->flags & GFX_DRAW_GROUP_CULL_FRONT) != 0) {
                    renderState->cullFrontFaces = 1;
                    Gl_CullFrontFaces();
                } else {
                    renderState->cullFrontFaces = 0;
                    Gl_CullBackFaces();
                }
            }
            if ((changeMask & GFX_DRAW_GROUP_DEPTH_TEST) != 0) {
                if ((drawGroup->flags & GFX_DRAW_GROUP_DEPTH_TEST) != 0) {
                    enabled = 1;
                } else {
                    enabled = 0;
                }
                renderState->depthTestEnabled = enabled;
                Gl_SetDepthTest(enabled);
            }
            if ((changeMask & GFX_DRAW_GROUP_DEPTH_WRITE) != 0) {
                if ((drawGroup->flags & GFX_DRAW_GROUP_DEPTH_WRITE) != 0) {
                    enabled = 1;
                } else {
                    enabled = 0;
                }
                renderState->depthWriteEnabled = enabled;
                Gl_SetDepthMask(enabled);
            }
            if ((changeMask & GFX_DRAW_GROUP_COLOR_KEY) != 0) {
                if ((drawGroup->flags & GFX_DRAW_GROUP_COLOR_KEY) != 0) {
                    enabled = 1;
                } else {
                    enabled = 0;
                }
                renderState->colorKeyEnabled = enabled;
            }
            if ((changeMask & GFX_DRAW_GROUP_SEMI_TRANSPARENT) != 0) {
                if ((drawGroup->flags & GFX_DRAW_GROUP_SEMI_TRANSPARENT) != 0) {
                    enabled = 1;
                } else {
                    enabled = 0;
                }
                if (enabled != 0) {
                    if (context->overrideActive != 0) {
                        renderState->blendMode = *(int *)((char *)context->overrideRenderState + 0x44);
                    } else {
                        renderState->blendMode = drawGroup->blendMode;
                    }
                } else {
                    renderState->blendMode = BLEND_NONE;
                }
                Gl_ApplyBlendMode(renderState->blendMode, context);
            }
            if ((changeMask & GFX_DRAW_GROUP_SHADE_MODE) != 0) {
                if ((drawGroup->flags & GFX_DRAW_GROUP_SHADE_MODE) != 0 && context->renderStateApplied == 0) {
                    if (drawGroup->groupKind == D3DSHADE_FLAT) {
                        renderState->isGouraud = 0;
                        Gl_SetShadeModelFlat();
                    } else {
                        renderState->isGouraud = 1;
                        Gl_SetShadeModelSmooth();
                    }
                } else {
                    renderState->isGouraud = 0;
                    Gl_SetShadeModelFlat();
                }
            }
            // VC5 consumes one register choice without emitting an instruction.
            if (REG_ALLOC_HINT) {
            }
            if ((changeMask & GFX_DRAW_GROUP_WRAP_U) != 0) {
                if ((drawGroup->flags & GFX_DRAW_GROUP_WRAP_U) != 0) {
                    renderState->wrapUEnabled = 1;
                } else {
                    renderState->wrapU = 0;
                }
                Gl_SetTextureWrapU(*(unsigned char *)&renderState->wrapU);
            }
            if ((changeMask & GFX_DRAW_GROUP_WRAP_V) != 0) {
                if ((drawGroup->flags & GFX_DRAW_GROUP_WRAP_V) != 0) {
                    renderState->wrapU = 1;
                } else {
                    renderState->wrapU = 0;
                }
                Gl_SetTextureWrapV(*(unsigned char *)&renderState->wrapU);
            }
        } else {
            if ((changeMask & 0xFFFDFFFD) != 0) {
                if ((changeMask & GFX_DRAW_GROUP_SEMI_TRANSPARENT) != 0) {
                    if ((drawGroup->flags & GFX_DRAW_GROUP_SEMI_TRANSPARENT) != 0) {
                        enabled = 1;
                    } else {
                        enabled = 0;
                    }
                    if (enabled != 0) {
                        if (context->overrideActive != 0) {
                            renderState->blendMode = *(int *)((char *)context->overrideRenderState + 0x44);
                        } else {
                            renderState->blendMode = drawGroup->blendMode;
                        }
                    } else {
                        renderState->blendMode = BLEND_NONE;
                    }
                    Gl_ApplyBlendMode(renderState->blendMode, context);
                } else {
                    renderState->blendMode = BLEND_NONE;
                    Gl_ApplyBlendMode(renderState->blendMode, context);
                }
            }
            if ((changeMask & GFX_DRAW_GROUP_TEXTURED) != 0) {
                gbatch = (GfxTextureSurface *)(drawGroup->batch);
                renderState->batch = (int)gbatch;
                if (gbatch != 0 && (drawGroup->flags & GFX_DRAW_GROUP_TEXTURED) != 0) {
                    textureDesc = (GfxTexHeader *)gbatch->header;
                    palRecord = (GfxPaletteRecord *)(gbatch->palette);
                    if (palRecord != 0) {
                        if (palRecord->paletteMode != 0) {
                            if ((((unsigned char *)palRecord->colorLookup)[gbatch->paletteRow] & 1) != 0) {
                                enabled = 1;
                            } else {
                                enabled = 0;
                            }
                        } else if (textureDesc->colorKey != 0) {
                            enabled = 1;
                        } else {
                            enabled = 0;
                        }
                    } else if (textureDesc->colorKey != 0) {
                        enabled = 1;
                    } else {
                        enabled = 0;
                    }
                    renderState->colorKeyEnabled = enabled;
                }
            }
            if ((changeMask & GFX_DRAW_GROUP_SHADE_MODE) != 0) {
                if ((drawGroup->flags & GFX_DRAW_GROUP_SHADE_MODE) != 0 && context->renderStateApplied == 0) {
                    if (drawGroup->groupKind == D3DSHADE_FLAT) {
                        renderState->isGouraud = 0;
                        Gl_SetShadeModelFlat();
                    } else {
                        renderState->isGouraud = 1;
                        Gl_SetShadeModelSmooth();
                    }
                } else {
                    renderState->isGouraud = 0;
                    Gl_SetShadeModelFlat();
                }
            }
        }
    }
}

/*
 * Hand the node's polygon set at +0x14 to Gl_OpsDrawPolygonSet, when there is
 * a node. The null test is against the parameter in place, so it is written on
 * the pointer and not on a copy.
 */
// FUNCTION: FF7 0x006b5122
void __cdecl Gl_OpsDrawPolygonSetIndirect(void *node, struct GfxEngineContext *context)
{
    if (node != 0) {
        Gl_OpsDrawPolygonSet((GlPolygonSet *)*(void **)((char *)node + 0x14), context);
    }
}

/*
 * Store the scene pass, raise the flag of that pass's polygon set (opaque or
 * translucent), clear the other's, and hand the set to the driver. The frame is four bytes
 * and that slot is the switch's own temporary.
 */
// FUNCTION: FF7 0x006b5140
void __cdecl Gl_OpsSetScenePass(int which, GfxEngineContext *dev)
{
    dev->scenePass = which;
    switch (which) {
    case GFX_SCENE_PASS_OPAQUE:
        dev->opaqueSet->drawEnabled = 1;
        dev->transSet->drawEnabled = 0;
        Gfx_DrawPolygonSetCall((PolygonSkeletonFrameRef *)dev->opaqueSet, (int)dev);
        break;
    case GFX_SCENE_PASS_TRANSLUCENT:
        dev->opaqueSet->drawEnabled = 0;
        dev->transSet->drawEnabled = 1;
        Gfx_DrawPolygonSetCall((PolygonSkeletonFrameRef *)dev->transSet, (int)dev);
        break;
    }
}

/*
 * Returns zero immediately when the device in-scene flag +0x81C is already
 * nonzero. Otherwise briefly sets and clears that flag, gets the GL state and
 * borrows the window DC.
 */
// FUNCTION: FF7 0x006b51d2
int __cdecl Gl_OpsBeginScene(int pass, struct GfxEngineContext *dev)
{
    int res;
    GlContext *st;
    void *hdc;

    res = 0;
    if (dev->sceneActive == 0) {
        res = 1;
        dev->sceneActive = 1;
        dev->sceneActive = 0;
        st = (GlContext *)Gl_GetState(dev);
        if (1) { /* Constant condition kept for /Od branch layout; see lib/codegen.h. */
            hdc = GetDC((void *)dev->hwnd);
            if (hdc != 0) {
                if (wglMakeCurrent(hdc, st->renderContext) != 0) {
                    ReleaseDC((void *)dev->hwnd, hdc);
                } else {
                    res = 0;
                }
            }
        }
        if (res != 0) {
            dev->sceneActive = 1;
            Gfx_SetScenePassCall(pass, dev);
        }
    }
    return res;
}

/*
 * Flushes OpenGL commands via glFlush and clears the engine context's in-scene
 * flag when a scene is active.
 */
// FUNCTION: FF7 0x006b5297
void __cdecl Gl_OpsEndScene(struct GfxEngineContext *dev)
{
    if (dev->sceneActive != 0) {
        glFlush();
        dev->sceneActive = 0;
    }
}

// Keeps a texture's palette slot in step with its GL texture when flags is zero. The slot is
// clamped to zero if negative, then copied into the header's row. When a texture-name
// table exists, the function reuses the GL name for the palette slot, or reloads the texture when
// that name is absent, and then binds the texture name. The function always returns 1.
// FUNCTION: FF7 0x006b52bb
int __cdecl Gl_OpsPaletteChanged(int paletteIndex, int firstEntry, int entryCount, int flags, GfxTextureSurface *tex)
{
    void *names;
    void *name;

    if (1) { /* Constant condition kept for /Od branch layout; see lib/codegen.h. */
        if (flags == 0) {
            if (tex->paletteRow < 0) {
                tex->paletteRow = 0;
            }
            ((GfxTexHeader *)tex->header)->row = tex->paletteRow;
            names = Gl_GetTextureNameTable(tex);
            if (names != 0) {
                name = (void *)Gl_GetTextureName(tex->paletteRow, (int)names);
                if (name != 0) {
                    tex->handle = (int)name;
                } else {
                    Gl_OpsLoadTexture(tex, (GfxTexHeader *)tex->header, tex->format);
                }
            }
            Gl_BindTexture(tex->handle);
        }
    }
    return 1;
}

/*
 * Rebuild a palette's colour arrays if it has any; otherwise, when the caller
 * named a texture rather than a palette, drop that texture's GL name so it is
 * uploaded again. Gl_OpsClear's `if (1)` for a third time -- `mov edx, 1 /
 * test edx, edx je` in the else arm, which VC5 does not fold at /Od.
 */
// FUNCTION: FF7 0x006b5380
int __cdecl Gl_OpsUpdatePaletteEntries(int first, int fmt, unsigned int *entries,
                                       int count, GfxPaletteRecord *lut, GfxTextureSurface *tex)
{
    int ok;
    int pos;
    void *old;
    void *gids;
    void *pal;
    void *gl;

    ok = 0;
    pal = Gl_GetPaletteColorArrays(lut);
    if (pal != 0) {
        gl = Gl_GetState(GetEngineContext());
        Gl_BuildColorArrays(fmt, count, (GlColorBytes *)(entries + first), (GlColorArrays *)pal, (int)gl);
        ok = 1;
    } else {
        if (1) { /* Constant condition kept for /Od branch layout; see lib/codegen.h. */
            if (lut == 0) {
                gids = Gl_GetTextureNameTable(tex);
                if (gids != 0) {
                    pos = count / ((GfxTexHeader *)tex->header)->entriesPerPalette;
                    old = (void *)Gl_GetTextureName(pos, (int)gids);
                    if (old != 0) {
                        Gl_DeleteTexture((unsigned int)old);
                    }
                    Gl_SetTextureName(0, pos, (int)gids);
                }
            }
        }
    }
    return ok;
}

/*
 * An OpenGL ops-table slot that does nothing: it copies its argument into a local and reads the
 * first dword through it, and uses neither.
 *
 * Codegen: `dev` and `v` are the spellings that put the two slots at [ebp-4] and [ebp-8]
 */
// FUNCTION: FF7 0x006b5453
void __cdecl Gl_OpsNoOp(void *arg)
{
    void *dev;
    int v;

    dev = arg;
    v = *(int *)dev;
}

// Stores a matrix in one of the device's three matrix slots. If the slot is empty, it points the
// slot at the caller's matrix; otherwise it copies the caller's matrix into the slot's existing
// storage. Slots 0, 1 and 2 are world, view and projection, and the three cases are identical apart
// from the slot. Any other index does nothing, and so does a null device.
// FUNCTION: FF7 0x006b546b
void __cdecl Gl_OpsSetMatrix(int which, void *src, GlMatrixDevice *dev)
{
    if (dev != 0) {
        switch (which) {
        case GFX_MATRIX_SLOT_WORLD:
            if (dev->matrixSlot0 != 0) {
                *(GfxMatrix4x4 *)dev->matrixSlot0 =
                    *(GfxMatrix4x4 *)src;
            } else {
                dev->matrixSlot0 = src;
            }
            break;
        case GFX_MATRIX_SLOT_VIEW:
            if (dev->matrixSlot1 != 0) {
                *(GfxMatrix4x4 *)dev->matrixSlot1 =
                    *(GfxMatrix4x4 *)src;
            } else {
                dev->matrixSlot1 = src;
            }
            break;
        case GFX_MATRIX_SLOT_PROJECTION:
            if (dev->matrixSlot2 != 0) {
                *(GfxMatrix4x4 *)dev->matrixSlot2 =
                    *(GfxMatrix4x4 *)src;
            } else {
                dev->matrixSlot2 = src;
            }
            break;
        }
    }
}

// Draws one polygon batch, combining the matrix with the context matrix at context+0x8D0 into the
// local combinedMatrix. The render state at context+0x944 supplies the texture. When it has one,
// the texture comes from the group's batch override if a group is given, or else from the render
// state, and it is bound with its palette uploaded when present. Flat or Gouraud drawing is chosen
// by the render state's isGouraud field. Without a texture, texturing is disabled and the untextured
// flat or Gouraud drawer runs.
// FUNCTION: FF7 0x006b550b
void __cdecl Gl_DrawPolygonBatch(int matrix, GfxPolygonBatch *polyBatch, GlDrawGroup *group, struct GfxEngineContext *context)
{
    GlPolygonRenderState *renderState;
    int combinedMatrix[16];
    GfxTextureSurface *texBatch;
    int paletteIdx;

    renderState = (GlPolygonRenderState *)context->presetPolygonSets;
    MultiplyMatrix4x4ByTranspose((void *)matrix, (char *)context + 0x8D0, combinedMatrix);
    if (renderState->texBatch != 0) {
        if (group != 0) {
            texBatch = (GfxTextureSurface *)(group->batch);
        } else {
            texBatch = (GfxTextureSurface *)(renderState->texBatch);
        }
        Gl_BindTexture(texBatch->handle);
        if (texBatch->palette != 0) {
            paletteIdx = renderState->paletteIdx;
            Gl_UploadPaletteEXT(paletteIdx, 0, ((GlPaletteSource *)(texBatch->palette))->entryCount, texBatch->paletteRow, ((GlPaletteSource *)(texBatch->palette))->paletteEntryCount, (int)Gl_GetPaletteColorArrays((GfxPaletteRecord *)texBatch->palette), texBatch);
        }
        if (renderState->isGouraud == 0) {
            Gl_DrawTrisTransformedTexFlat(polyBatch->indexCount, (unsigned short *)polyBatch->indices, polyBatch->vertices, (int)combinedMatrix);
        } else {
            Gl_DrawTrisTransformedTexGouraud(polyBatch->indexCount, (unsigned short *)polyBatch->indices, polyBatch->vertices, (int)combinedMatrix);
        }
    } else {
        Gl_SetTexture2DEnable(0);
        if (renderState->isGouraud == 0) {
            Gl_DrawTrisTransformedFlat(polyBatch->indexCount, (unsigned short *)polyBatch->indices, polyBatch->vertices, (int)combinedMatrix);
        } else {
            Gl_DrawTrisTransformedGouraud(polyBatch->indexCount, (unsigned short *)polyBatch->indices, polyBatch->vertices, (int)combinedMatrix);
        }
    }
}

/*
 * Dispatches a pretransformed polygon batch through textured or untextured
 * OpenGL triangle helpers and flat or Gouraud color according to context
 * state. For textured batches, chooses the override texture when supplied,
 * binds it and uploads its palette when present.
 */
// FUNCTION: FF7 0x006b5676
void __cdecl Gl_DrawPolygonBatchPreTransformed(GfxPolygonBatch *polyBatch, GlDrawGroup *group, struct GfxEngineContext *context)
{
    GlPolygonRenderState *rs;
    int palSlot;
    GfxTextureSurface *texBatch;

    rs = (GlPolygonRenderState *)(context->presetPolygonSets);
    if (rs->texBatch != 0) {
        if (group != 0) {
            texBatch = (GfxTextureSurface *)(group->batch);
        } else {
            texBatch = (GfxTextureSurface *)(rs->texBatch);
        }
        Gl_BindTexture(texBatch->handle);
        if (texBatch->palette != 0) {
            palSlot = rs->paletteIdx;
            Gl_UploadPaletteEXT(palSlot, 0, ((GlPaletteSource *)(texBatch->palette))->entryCount, texBatch->paletteRow, ((GlPaletteSource *)(texBatch->palette))->paletteEntryCount, (int)Gl_GetPaletteColorArrays((GfxPaletteRecord *)texBatch->palette), texBatch);
        }
        if (rs->isGouraud == 0) {
            Gl_DrawTrisTexFlat(polyBatch->indexCount, (unsigned short *)polyBatch->indices, polyBatch->vertices);
        } else {
            Gl_DrawTrisTexGouraud(polyBatch->indexCount, (unsigned short *)polyBatch->indices, polyBatch->vertices);
        }
    } else {
        Gl_SetTexture2DEnable(0);
        if (rs->isGouraud == 0) {
            Gl_DrawTrisFlat(polyBatch->indexCount, (unsigned short *)polyBatch->indices, polyBatch->vertices);
        } else {
            Gl_DrawTrisGouraud(polyBatch->indexCount, (unsigned short *)polyBatch->indices, polyBatch->vertices);
        }
    }
}

/*
 * Walks enabled polygon-set groups, selects shared or per-group render state
 * and chooses immediate OpenGL drawing, depth-sort enqueueing or deferred
 * heap-node recording from the output mode and state flags. For matching draw-
 * list generations, walks the op list by its opcode (GfxDrawOpKind) and draws
 * each node with no transform, the node's transform or that transform times the
 * camera matrix (GfxPolygonSetTransformKind).
 */
// FUNCTION: FF7 0x006b57b8
void __cdecl Gl_OpsDrawPolygonSet(GlPolygonSet *set, struct GfxEngineContext *context)
{
    GfxPolygonSetRecord *polyRecordPtr;
    GfxPolygonBatch *polygonBatchPtr;
    int slot;
    GfxPolygonSetHeader *headerPointer;
    GfxPolygonSetNode *nodeAPtr;
    GlDrawOpNode *opNodePtr;
    int depthSort;
    GfxPolygonSetNode *recordNode;
    int vertexPointer;
    int concatMtx;
    GfxDrawListNode *pDlNode;
    int unusedSpace;
    int hasList;
    int matched;
    int group;
    int recordMode;
    int enabled;
    int linkedPointer;
    int transformPtr;
    int curMatrix;
    int cachedMtx;
    GfxPolygonSetNode *pendingNode;

    unusedSpace = 0;
    if (set != 0 && set->drawEnabled != 0) {
        polyRecordPtr = (GfxPolygonSetRecord *)(set->polygonRecord);
        headerPointer = (GfxPolygonSetHeader *)((int)set + 0x14);
        hasList = 0;
        if (headerPointer->drawListActive != 0) {
            hasList = 1;
            if (*(int *)headerPointer->tagPtr == headerPointer->tag) {
                matched = 1;
            } else {
                matched = 0;
            }
        }
        group = 0;
        if (set->useSharedGroup != 0) {
            group = set->sharedDrawGroup;
        }
        linkedPointer = 0;
        if (set->linkedObject != 0) {
            linkedPointer = *(int *)((char *)set->linkedObject + 0x6C);
        }
        for (slot = 0; slot < set->count; slot++) {
            recordMode = 0;
            if (set->usePerEntryGroups != 0) {
                group = *(int *)((char *)set->entries + slot * 4);
            }
            if (context->sortedDrawEnabled != 0 && group != 0 && ((GlDrawGroup *)group)->polygonSet != 0) {
                depthSort = 1;
            } else {
                depthSort = 0;
            }
            if (group != 0) {
                switch (context->scenePass) {
                case GFX_SCENE_PASS_OPAQUE:
                    enabled = (((GlDrawGroup *)group)->flags & 0x410) == 0;
                    if (depthSort != 0) { /* empty first arm: lib/codegen.h */
                    } else {
                        recordMode = enabled == 0;
                    }
                    enabled = 1;
                    break;
                case GFX_SCENE_PASS_TRANSLUCENT:
                    enabled = 1;
                    break;
                case GFX_SCENE_PASS_ALL_IMMEDIATE:
                    enabled = 1;
                    break;
                default:
                    enabled = 0;
                }
            } else {
                enabled = 1;
            }
            if (enabled != 0) {
                if (recordMode != 0) { /* empty first arm: lib/codegen.h */
                } else {
                    Gl_ApplyRenderState((struct GlDrawGroup *)group, context);
                }
                if (hasList != 0) {
                    if (matched != 0) {
                        opNodePtr = (GlDrawOpNode *)(headerPointer->payload);
                        if (opNodePtr != 0) {
                            switch (opNodePtr->opcode) {
                            case GFX_DRAW_OP_BATCH:
                                do {
                                    if (recordMode != 0) {
                                        nodeAPtr = (GfxPolygonSetNode *)(AppendBlockHeapNode(context->nodeList));
                                        if (nodeAPtr != 0) {
                                            nodeAPtr->polyIndex = slot;
                                            nodeAPtr->drawGroup = (int)set;
                                            nodeAPtr->primGroupPtr = group;
                                            if (set->hasStateBlob != 0) {
                                                nodeAPtr->stateBlock = *(GfxPolygonGroupDrawSpec *)(set->stateBlob);
                                            }
                                            switch (headerPointer->kind) {
                                            case GFX_POLYGON_SET_TRANSFORM_NONE:
                                                nodeAPtr->inUse = 0;
                                                nodeAPtr->submitted = 0;
                                                break;
                                            case GFX_POLYGON_SET_TRANSFORM_NODE:
                                                nodeAPtr->inUse = 0;
                                                nodeAPtr->submitted = 1;
                                                nodeAPtr->payloadPtr = (int)opNodePtr + 0xC;
                                                break;
                                            case GFX_POLYGON_SET_TRANSFORM_NODE_CAMERA:
                                                nodeAPtr->inUse = 1;
                                                ConcatAffineMatrix4x4((char *)opNodePtr + 0xC, (void *)context->cameraMatrix, nodeAPtr->matrix);
                                                ClearMatrixColumn4(nodeAPtr->matrix);
                                                nodeAPtr->submitted = 0;
                                                break;
                                            default:
                                                nodeAPtr->inUse = 0;
                                                nodeAPtr->submitted = 0;
                                            }
                                        }
                                    } else {
                                        switch (headerPointer->kind) {
                                        case GFX_POLYGON_SET_TRANSFORM_NONE:
                                            transformPtr = 0;
                                            break;
                                        case GFX_POLYGON_SET_TRANSFORM_NODE:
                                            transformPtr = (int)opNodePtr + 0xC;
                                            break;
                                        case GFX_POLYGON_SET_TRANSFORM_NODE_CAMERA:
                                            ConcatAffineMatrix4x4((char *)opNodePtr + 0xC, (void *)context->cameraMatrix, &g_GlTransformMatrix);
                                            transformPtr = (int)&g_GlTransformMatrix;
                                            break;
                                        default:
                                            transformPtr = 0;
                                        }
                                        if (set->batches != 0) {
                                            polygonBatchPtr = (GfxPolygonBatch *)(((int *)(set->batches))[slot]);
                                            if (polygonBatchPtr != 0) {
                                                if (depthSort != 0) {
                                                    Gfx_EnqueueGroupForDepthSort(transformPtr, 0, polygonBatchPtr, (int)set, group, linkedPointer, context);
                                                } else if (set->drawUnsorted != 0 && transformPtr != 0) {
                                                    Gl_DrawPolygonBatch(transformPtr, polygonBatchPtr, (GlDrawGroup *)group, context);
                                                }
                                            }
                                        }
                                    }
                                    opNodePtr = (GlDrawOpNode *)(opNodePtr->next);
                                } while (opNodePtr != 0);
                                break;
                            case GFX_DRAW_OP_BATCH_DRAW_SPEC:
                                do {
                                    if (recordMode != 0) {
                                        recordNode = (GfxPolygonSetNode *)(AppendBlockHeapNode(context->nodeList));
                                        if (recordNode != 0) {
                                            recordNode->polyIndex = slot;
                                            recordNode->drawGroup = (int)set;
                                            recordNode->primGroupPtr = group;
                                            if (set->hasStateBlob != 0) {
                                                recordNode->stateBlock = *(GfxPolygonGroupDrawSpec *)(set->stateBlob);
                                            }
                                            switch (headerPointer->kind) {
                                            case GFX_POLYGON_SET_TRANSFORM_NONE:
                                                recordNode->inUse = 0;
                                                recordNode->submitted = 0;
                                                break;
                                            case GFX_POLYGON_SET_TRANSFORM_NODE:
                                                recordNode->inUse = 0;
                                                recordNode->submitted = 1;
                                                recordNode->payloadPtr = (int)opNodePtr + 0xC;
                                                break;
                                            case GFX_POLYGON_SET_TRANSFORM_NODE_CAMERA:
                                                recordNode->inUse = 1;
                                                ConcatAffineMatrix4x4((char *)opNodePtr + 0xC, (void *)context->cameraMatrix, recordNode->matrix);
                                                ClearMatrixColumn4(recordNode->matrix);
                                                recordNode->submitted = 0;
                                                break;
                                            default:
                                                recordNode->inUse = 0;
                                                recordNode->submitted = 0;
                                            }
                                        }
                                    } else {
                                        switch (headerPointer->kind) {
                                        case GFX_POLYGON_SET_TRANSFORM_NONE:
                                            concatMtx = 0;
                                            break;
                                        case GFX_POLYGON_SET_TRANSFORM_NODE:
                                            concatMtx = (int)opNodePtr + 0xC;
                                            break;
                                        case GFX_POLYGON_SET_TRANSFORM_NODE_CAMERA:
                                            ConcatAffineMatrix4x4((char *)opNodePtr + 0xC, (void *)context->cameraMatrix, &g_GlTransformMatrix);
                                            concatMtx = (int)&g_GlTransformMatrix;
                                            break;
                                        default:
                                            concatMtx = 0;
                                        }
                                        if (set->batches != 0) {
                                            polygonBatchPtr = (GfxPolygonBatch *)(((int *)(set->batches))[slot]);
                                            if (polygonBatchPtr != 0) {
                                                if (opNodePtr->offsetEnabled != 0 || opNodePtr->offsetMode != 0) {
                                                    Gfx_ApplyDrawSpecToGroup(slot, (struct GfxPolygonSet *)set, (GfxDrawSpec *)((int)opNodePtr + 0x4C));
                                                } else if (depthSort != 0) {
                                                    Gfx_EnqueueGroupForDepthSort(concatMtx, 0, polygonBatchPtr, (int)set, group, linkedPointer, context);
                                                } else {
                                                    Gfx_ApplyDrawSpecToGroup(slot, (struct GfxPolygonSet *)set, (GfxDrawSpec *)((int)opNodePtr + 0x4C));
                                                    if (set->drawUnsorted != 0 && concatMtx != 0) {
                                                        Gl_DrawPolygonBatch(concatMtx, polygonBatchPtr, (GlDrawGroup *)group, context);
                                                    }
                                                }
                                            }
                                        }
                                    }
                                    opNodePtr = (GlDrawOpNode *)(opNodePtr->next);
                                } while (opNodePtr != 0);
                                break;
                            case GFX_DRAW_OP_DRAW_LIST:
                                if (set->batches != 0) {
                                    polygonBatchPtr = (GfxPolygonBatch *)(((int *)(set->batches))[slot]);
                                    if (polygonBatchPtr != 0) {
                                        vertexPointer = polygonBatchPtr->vertices;
                                        do {
                                            switch (headerPointer->kind) {
                                            case GFX_POLYGON_SET_TRANSFORM_NONE:
                                                curMatrix = 0;
                                                break;
                                            case GFX_POLYGON_SET_TRANSFORM_NODE:
                                                curMatrix = (int)opNodePtr + 0xC;
                                                break;
                                            case GFX_POLYGON_SET_TRANSFORM_NODE_CAMERA:
                                                ConcatAffineMatrix4x4((char *)opNodePtr + 0xC, (void *)context->cameraMatrix, &g_GlTransformMatrix);
                                                curMatrix = (int)&g_GlTransformMatrix;
                                                break;
                                            default:
                                                curMatrix = 0;
                                            }
                                            pDlNode = (GfxDrawListNode *)(opNodePtr->drawNode);
                                            if (pDlNode->kind == GFX_DRAW_SPRITE_BATCH) {
                                                vertexPointer = pDlNode->vertexCount;
                                            } else {
                                                Gfx_EmitPolygonSetPrimitive(depthSort, group, pDlNode);
                                            }
                                            if (depthSort != 0) {
                                                Gfx_EnqueuePsxPrimitiveForDepthSort(curMatrix, vertexPointer, (int)polygonBatchPtr, group, pDlNode, context);
                                            } else if (set->drawUnsorted != 0) {
                                                if (curMatrix != 0) {
                                                    Gl_DrawPolygonBatch(curMatrix, polygonBatchPtr, (GlDrawGroup *)group, context);
                                                } else {
                                                    Gl_DrawPolygonBatchPreTransformed(polygonBatchPtr, (GlDrawGroup *)group, context);
                                                }
                                            }
                                            opNodePtr = (GlDrawOpNode *)(opNodePtr->next);
                                        } while (opNodePtr != 0);
                                    }
                                }
                            }
                        }
                    }
                } else if (recordMode != 0) {
                    pendingNode = (GfxPolygonSetNode *)(AppendBlockHeapNode(context->nodeList));
                    if (pendingNode != 0) {
                        pendingNode->polyIndex = slot;
                        pendingNode->drawGroup = (int)set;
                        pendingNode->primGroupPtr = group;
                        if (set->hasStateBlob != 0) {
                            pendingNode->stateBlock = *(GfxPolygonGroupDrawSpec *)(set->stateBlob);
                        }
                        cachedMtx = GetCachedDeviceTransform(0, polyRecordPtr);
                        if (cachedMtx != 0) {
                            pendingNode->inUse = 1;
                            *(GfxMatrix4x4 *)((int)pendingNode + 0x14) = *(GfxMatrix4x4 *)(cachedMtx);
                            pendingNode->submitted = 0;
                        } else {
                            pendingNode->inUse = 0;
                            pendingNode->submitted = 0;
                        }
                    }
                } else if (set->batches != 0) {
                    polygonBatchPtr = (GfxPolygonBatch *)(((int *)(set->batches))[slot]);
                    if (polygonBatchPtr != 0) {
                        if (depthSort != 0) {
                            Gfx_EnqueueGroupForDepthSort(polyRecordPtr->depthGroup, 0, polygonBatchPtr, (int)set, group, linkedPointer, context);
                        } else if (set->drawUnsorted != 0 && polyRecordPtr->depthGroup != 0) {
                            Gl_DrawPolygonBatch(polyRecordPtr->depthGroup, polygonBatchPtr, (GlDrawGroup *)group, context);
                        }
                    }
                }
            }
            if (group != 0) {
                group += 0x64;
            }
            if (linkedPointer != 0) {
                linkedPointer += 0x38;
            }
        }
    }
}

/*
 * Look a deferred node up in the device's node table, apply its render state
 * and hand it to the polygon batcher. Two things here are 1998 bugs and are
 * reproduced, not fixed: `st` is read as the batcher's first argument on a
 * path where neither of the two guards above it assigned to it.
 */
// FUNCTION: FF7 0x006b6090
void __cdecl Gl_OpsDrawDeferredNode(GlDeferredDrawNode *req, struct GfxEngineContext *context)
{
    GlDeferredDevice *dev;
    void *st;
    GlDrawGroup *ds;
    GfxPolygonBatch *obj;
    void *ext;

    dev = (GlDeferredDevice *)(req->device);
    ds = (GlDrawGroup *)req->drawGroup;
    if (dev->objectTable != 0) {
        obj = (GfxPolygonBatch *)*(void **)(*(char **)&dev->objectTable + req->recordIndex * 4);
        if (obj != 0) {
            if (*(int *)(dev->recordBlock + 0x6c) != 0) {
                ext = *(char **)(dev->recordBlock + 0x6c) + req->recordIndex * 0x38;
            } else {
                ext = 0;
            }
            Gl_ApplyRenderState(ds, context);
            if (req->hasInlineMatrix != 0) {
                st = (char *)req + 0x14;
            }
            if (req[1].recordIndex != 0) {
                st = req[1].device;
            }
            if (dev->drawUnsorted != 0) {
                // BUG: st is unassigned when the node has neither an inline matrix nor a second
                // record; the batcher then gets whatever was on the stack.
                Gl_DrawPolygonBatch((int)st, obj, ds, context);
            }
        }
    }
}

/*
 * Present the frame. The result of Gl_GetState goes straight into the next
 * call with no frame at all, so it is one nested expression and not a local.
 */
// FUNCTION: FF7 0x006b6156
void __cdecl Gl_OpsFlip(struct GfxEngineContext *dev)
{
    Gl_PresentFrame((GlContext *)Gl_GetState(dev));
}

/*
 * Empty. The GL driver's shutdown slot, which has nothing to release.
 */
// FUNCTION: FF7 0x006b6170
void __cdecl Gl_OpsShutdownNoOp(void)
{
    /* The original is empty. Its whole body is
       `55 8b ec 5d c3` -- 16 bytes at 0x006b6170,
       push/mov/pop/ret plus alignment padding.
       The C body is empty, as in retail. */
}
} /* extern "C" */
