/* include/lib/enums/graphics.h: named codes of lib/graphics and the TUs that use them.
 * Enums only, so any TU can include it: no types, no declarations. */
#ifndef LIB_ENUMS_GRAPHICS_H
#define LIB_ENUMS_GRAPHICS_H

/* Render object primitive kinds: bit0 3D, bit1 Gouraud, bit2 textured, bit3 quad; Gfx_CreateRenderObject */
enum GfxDrawKind {
    GFX_DRAW_FLAT_TRI_2D = 0x00,
    GFX_DRAW_FLAT_TRI_3D = 0x01,
    GFX_DRAW_GOURAUD_TRI_2D = 0x02,
    GFX_DRAW_GOURAUD_TRI_3D = 0x03,
    GFX_DRAW_TEXTURED_TRI_2D = 0x04,
    GFX_DRAW_TEXTURED_TRI_3D = 0x05,
    GFX_DRAW_TEXTURED_GOURAUD_TRI_2D = 0x06,
    GFX_DRAW_TEXTURED_GOURAUD_TRI_3D = 0x07,
    GFX_DRAW_FLAT_QUAD_2D = 0x08,
    GFX_DRAW_FLAT_QUAD_3D = 0x09,
    GFX_DRAW_GOURAUD_QUAD_2D = 0x0A,
    GFX_DRAW_GOURAUD_QUAD_3D = 0x0B,
    GFX_DRAW_TEXTURED_QUAD_2D = 0x0C,
    GFX_DRAW_TEXTURED_QUAD_3D = 0x0D,
    GFX_DRAW_TEXTURED_GOURAUD_QUAD_2D = 0x0E,
    GFX_DRAW_TEXTURED_GOURAUD_QUAD_3D = 0x0F,
    GFX_DRAW_SPRITE_BATCH = 0x11,                 /* built as TEXTURED_QUAD_2D */
    GFX_DRAW_FLAT_LINE_2D = 0x12,
    GFX_DRAW_GOURAUD_LINE_2D = 0x13               /* no retail creator found; pairs with 0x12 */
};

/* Semi-transparency modes stored in load options +0x20 and applied by Pfile_SetHundredBlendMode */
enum BlendMode {
    BLEND_AVERAGE = 0x00,
    BLEND_ADDITIVE = 0x01,
    BLEND_SUBTRACTIVE = 0x02,
    BLEND_QUARTER_ADDITIVE = 0x03,
    BLEND_NONE = 0x04,
    BLEND_KEEP = 0x06
};

/* Engine render state switches passed to driver slot 0x64; applied to D3D by Gfx_SetRenderStateD3D */
enum RenderState {
    RENDER_STATE_WIREFRAME = 0x00,
    RENDER_STATE_LINEAR_FILTER = 0x02,
    RENDER_STATE_PERSPECTIVE = 0x03,
    RENDER_STATE_TEXTURE_BLEND = 0x04,
    RENDER_STATE_WRAP_U = 0x05,
    RENDER_STATE_WRAP_V = 0x06,
    RENDER_STATE_COLOR_KEY = 0x08,
    RENDER_STATE_DITHER = 0x09,
    RENDER_STATE_ALPHA_BLEND = 0x0A,
    RENDER_STATE_ANTIALIAS = 0x0C,
    RENDER_STATE_CULL_CLOCKWISE = 0x0D,
    RENDER_STATE_NO_CULL = 0x0E,
    RENDER_STATE_DEPTH_TEST = 0x0F,
    RENDER_STATE_DEPTH_WRITE = 0x10,
    RENDER_STATE_SPECULAR = 0x12,
    RENDER_STATE_TEXTURE_WRAP = 0x15
};

/* Bitmask values (1 << RenderState) in PolygonHundredState::flags and D3D preset state sets. */
enum RenderStateMask {
    RENDER_STATE_BIT_WIREFRAME = 0x000001,
    RENDER_STATE_BIT_LINEAR_FILTER = 0x000004,
    RENDER_STATE_BIT_PERSPECTIVE = 0x000008,
    RENDER_STATE_BIT_TEXTURE_BLEND = 0x000010,
    RENDER_STATE_BIT_WRAP_U = 0x000020,
    RENDER_STATE_BIT_WRAP_V = 0x000040,
    RENDER_STATE_BIT_COLOR_KEY = 0x000100,
    RENDER_STATE_BIT_DITHER = 0x000200,
    RENDER_STATE_BIT_ALPHA_BLEND = 0x000400,
    RENDER_STATE_BIT_ANTIALIAS = 0x001000,
    RENDER_STATE_BIT_CULL_CLOCKWISE = 0x002000,
    RENDER_STATE_BIT_NO_CULL = 0x004000,
    RENDER_STATE_BIT_DEPTH_TEST = 0x008000,
    RENDER_STATE_BIT_DEPTH_WRITE = 0x010000,
    RENDER_STATE_BIT_SPECULAR = 0x040000,
    RENDER_STATE_BIT_TEXTURE_WRAP = 0x200000,
    GFX_D3D_SUPPORTED_STATE_MASK = 0x25F77D
};

#ifndef __DDRAW_INCLUDED__
/* The DX5 SDK's DDERR_* codes (<ddraw.h>, MAKE_DDHRESULT(n) = 0x88760000 | n) that the code compares HRESULTs with; a TU with <ddraw.h> in scope has the SDK's macros instead. */
enum DirectDrawError {
    DD_OK = 0,                                              /* also D3D_OK: the call succeeded */
    DDERR_ALREADYINITIALIZED = (int)0x88760005,
    DDERR_CANNOTATTACHSURFACE = (int)0x8876000A,
    DDERR_CANNOTDETACHSURFACE = (int)0x88760014,
    DDERR_CURRENTLYNOTAVAIL = (int)0x88760028,
    DDERR_EXCEPTION = (int)0x88760037,
    DDERR_HEIGHTALIGN = (int)0x8876005A,
    DDERR_INCOMPATIBLEPRIMARY = (int)0x8876005F,
    DDERR_INVALIDCAPS = (int)0x88760064,
    DDERR_INVALIDCLIPLIST = (int)0x8876006E,
    DDERR_INVALIDMODE = (int)0x88760078,
    DDERR_INVALIDOBJECT = (int)0x88760082,
    DDERR_INVALIDPIXELFORMAT = (int)0x88760091,
    DDERR_INVALIDRECT = (int)0x88760096,
    DDERR_LOCKEDSURFACES = (int)0x887600A0,
    DDERR_NO3D = (int)0x887600AA,
    DDERR_NOALPHAHW = (int)0x887600B4,
    DDERR_NOCLIPLIST = (int)0x887600CD,
    DDERR_NOCOLORCONVHW = (int)0x887600D2,
    DDERR_NOCOOPERATIVELEVELSET = (int)0x887600D4,
    DDERR_NOCOLORKEY = (int)0x887600D7,
    DDERR_NOCOLORKEYHW = (int)0x887600DC,
    DDERR_NOEXCLUSIVEMODE = (int)0x887600E1,
    DDERR_NOFLIPHW = (int)0x887600E6,
    DDERR_NOGDI = (int)0x887600F0,
    DDERR_NOMIRRORHW = (int)0x887600FA,
    DDERR_NOTFOUND = (int)0x887600FF,
    DDERR_NOOVERLAYHW = (int)0x88760104,
    DDERR_NORASTEROPHW = (int)0x88760118,
    DDERR_NOROTATIONHW = (int)0x88760122,
    DDERR_NOSTRETCHHW = (int)0x88760136,
    DDERR_NOT4BITCOLOR = (int)0x8876013C,
    DDERR_NOT4BITCOLORINDEX = (int)0x8876013D,
    DDERR_NOT8BITCOLOR = (int)0x88760140,
    DDERR_NOTEXTUREHW = (int)0x8876014A,
    DDERR_NOVSYNCHW = (int)0x8876014F,
    DDERR_NOZBUFFERHW = (int)0x88760154,
    DDERR_NOZOVERLAYHW = (int)0x8876015E,
    DDERR_OUTOFCAPS = (int)0x88760168,
    DDERR_OUTOFVIDEOMEMORY = (int)0x8876017C,
    DDERR_OVERLAYCANTCLIP = (int)0x8876017E,
    DDERR_OVERLAYCOLORKEYONLYONEACTIVE = (int)0x88760180,
    DDERR_PALETTEBUSY = (int)0x88760183,
    DDERR_COLORKEYNOTSET = (int)0x88760190,
    DDERR_SURFACEALREADYATTACHED = (int)0x8876019A,
    DDERR_SURFACEALREADYDEPENDENT = (int)0x887601A4,
    DDERR_SURFACEBUSY = (int)0x887601AE,
    DDERR_SURFACEISOBSCURED = (int)0x887601B8,
    DDERR_SURFACELOST = (int)0x887601C2,
    DDERR_SURFACENOTATTACHED = (int)0x887601CC,
    DDERR_TOOBIGHEIGHT = (int)0x887601D6,
    DDERR_TOOBIGSIZE = (int)0x887601E0,
    DDERR_TOOBIGWIDTH = (int)0x887601EA,
    DDERR_UNSUPPORTEDFORMAT = (int)0x887601FE,
    DDERR_UNSUPPORTEDMASK = (int)0x88760208,
    DDERR_VERTICALBLANKINPROGRESS = (int)0x88760219,
    DDERR_WASSTILLDRAWING = (int)0x8876021C,
    DDERR_XALIGN = (int)0x88760230,
    DDERR_INVALIDDIRECTDRAWGUID = (int)0x88760231,
    DDERR_DIRECTDRAWALREADYCREATED = (int)0x88760232,
    DDERR_NODIRECTDRAWHW = (int)0x88760233,
    DDERR_PRIMARYSURFACEALREADYEXISTS = (int)0x88760234,
    DDERR_NOEMULATION = (int)0x88760235,
    DDERR_REGIONTOOSMALL = (int)0x88760236,
    DDERR_CLIPPERISUSINGHWND = (int)0x88760237,
    DDERR_NOCLIPPERATTACHED = (int)0x88760238,
    DDERR_NOHWND = (int)0x88760239,
    DDERR_HWNDSUBCLASSED = (int)0x8876023A,
    DDERR_HWNDALREADYSET = (int)0x8876023B,
    DDERR_NOPALETTEATTACHED = (int)0x8876023C,
    DDERR_NOPALETTEHW = (int)0x8876023D,
    DDERR_BLTFASTCANTCLIP = (int)0x8876023E,
    DDERR_NOBLTHW = (int)0x8876023F,
    DDERR_NODDROPSHW = (int)0x88760240,
    DDERR_OVERLAYNOTVISIBLE = (int)0x88760241,
    DDERR_NOOVERLAYDEST = (int)0x88760242,
    DDERR_INVALIDPOSITION = (int)0x88760243,
    DDERR_NOTAOVERLAYSURFACE = (int)0x88760244,
    DDERR_EXCLUSIVEMODEALREADYSET = (int)0x88760245,
    DDERR_NOTFLIPPABLE = (int)0x88760246,
    DDERR_CANTDUPLICATE = (int)0x88760247,
    DDERR_NOTLOCKED = (int)0x88760248,
    DDERR_CANTCREATEDC = (int)0x88760249,
    DDERR_NODC = (int)0x8876024A,
    DDERR_WRONGMODE = (int)0x8876024B,
    DDERR_IMPLICITLYCREATED = (int)0x8876024C,
    DDERR_NOTPALETTIZED = (int)0x8876024D
};

/* The DX5 SDK's DDSCAPS_* surface capability bits (<ddraw.h>) the code sets in a surface
 * description's ddsCaps.dwCaps and tests there. */
enum DirectDrawSurfaceCaps {
    DDSCAPS_ALPHA = 0x2,
    DDSCAPS_BACKBUFFER = 0x4,
    DDSCAPS_COMPLEX = 0x8,
    DDSCAPS_FLIP = 0x10,
    DDSCAPS_FRONTBUFFER = 0x20,
    DDSCAPS_OFFSCREENPLAIN = 0x40,
    DDSCAPS_PALETTE = 0x100,
    DDSCAPS_PRIMARYSURFACE = 0x200,
    DDSCAPS_SYSTEMMEMORY = 0x800,
    DDSCAPS_TEXTURE = 0x1000,
    DDSCAPS_3DDEVICE = 0x2000,
    DDSCAPS_VIDEOMEMORY = 0x4000,
    DDSCAPS_VISIBLE = 0x8000,
    DDSCAPS_WRITEONLY = 0x10000,
    DDSCAPS_ZBUFFER = 0x20000,
    DDSCAPS_OWNDC = 0x40000,
    DDSCAPS_ALLOCONLOAD = 0x4000000
};

/* The DX5 SDK's DDSD_* bits (<ddraw.h>): which members of a surface description are valid. */
enum DirectDrawSurfaceDescFlags {
    DDSD_CAPS = 0x1,
    DDSD_HEIGHT = 0x2,
    DDSD_WIDTH = 0x4,
    DDSD_BACKBUFFERCOUNT = 0x20,
    DDSD_ZBUFFERBITDEPTH = 0x40,
    DDSD_PIXELFORMAT = 0x1000
};

/* The DX5 SDK's DDBLTFAST_* flag (<ddraw.h>) passed to IDirectDrawSurface::BltFast. */
enum DirectDrawBltFastFlags {
    DDBLTFAST_WAIT = 0x10
};

/* The DX5 SDK's DDBD_* bit-depth flags (<ddraw.h>) of a driver's Z-buffer depth mask. */
enum DirectDrawBitDepth {
    DDBD_32 = 0x100,
    DDBD_24 = 0x200,
    DDBD_16 = 0x400,
    DDBD_8 = 0x800
};

/* The DX5 SDK's DDPF_* flags (<ddraw.h>) of a DDPIXELFORMAT that PixelFormatFromDDPixelFormat tests. */
enum DirectDrawPixelFormatFlags {
    DDPF_PALETTEINDEXED4 = 0x8,
    DDPF_PALETTEINDEXEDTO8 = 0x10,
    DDPF_PALETTEINDEXED8 = 0x20,
    DDPF_RGB = 0x40,
    DDPF_PALETTEINDEXED1 = 0x800,
    DDPF_PALETTEINDEXED2 = 0x1000
};

/* The DX5 SDK's DDCAPS_* driver capability bit (<ddraw.h>) the code tests in DDCAPS.dwCaps. */
enum DirectDrawCaps {
    DDCAPS_NOHARDWARE = 0x2000000
};

/* The DX5 SDK's DDSCL_* cooperative levels (<ddraw.h>) passed to SetCooperativeLevel. */
enum DirectDrawCooperativeLevel {
    DDSCL_FULLSCREEN = 0x1,
    DDSCL_NORMAL = 0x8,
    DDSCL_EXCLUSIVE = 0x10
};

/* The DX5 SDK's DDPCAPS_* palette capability bits (<ddraw.h>) passed to IDirectDraw::CreatePalette. */
enum DirectDrawPaletteCaps {
    DDPCAPS_4BIT = 0x1,
    DDPCAPS_8BIT = 0x4,
    DDPCAPS_ALLOW256 = 0x40,
    DDPCAPS_1BIT = 0x100,
    DDPCAPS_2BIT = 0x200
};

/* The DX5 SDK's DDLOCK_* flag (<ddraw.h>) passed to IDirectDrawSurface::Lock. */
enum DirectDrawLockFlags {
    DDLOCK_WAIT = 0x1
};
#endif /* __DDRAW_INCLUDED__ */

/* The search passes Gfx_SelectTextureFormat runs first, the bits of its *flagsPtr; each names the
 * texture-format list callback it runs. */
enum GfxTextureFormatSearch {
    GFX_TEXFORMAT_SEARCH_PALETTIZED = 0x1,     /* Gfx_TextureFormatEnumPalettizedCallback */
    GFX_TEXFORMAT_SEARCH_RANGE_FALLBACK = 0x2, /* Gfx_TextureFormatEnumRangeFallbackCallback */
    GFX_TEXFORMAT_SEARCH_CURRENT_MODE = 0x4    /* Gfx_TextureFormatEnumCurrentModeCallback */
};

/* Graphics driver types, GfxEngineContext::driverType: Gfx_LoadAppConfigFile sets them from the
 * config file's driver option ("dx", "sw", "dll", "gl"), the registry loader from the Graphics
 * key's Driver value; InitGraphicsDevice binds the Direct3D ops for DIRECTX, the DirectDraw blit
 * ops for SOFTWARE, the loaded DLL's procedure for DLL and Gl_BindDriverOps for OPENGL. A DLL
 * that does not install falls back to SOFTWARE. */
enum GfxDriverType {
    GFX_DRIVER_DIRECTX = 0,
    GFX_DRIVER_SOFTWARE = 1,
    GFX_DRIVER_DLL = 2,
    GFX_DRIVER_OPENGL = 3
};

#ifndef _D3D_H_
/* The DX5 SDK's D3DERR_* codes (<d3d.h>, MAKE_DDHRESULT(n) = 0x88760000 | n) that the code compares HRESULTs with; a TU with <d3d.h> in scope has the SDK's macros instead. */
enum Direct3DError {
    D3DERR_BADMAJORVERSION = (int)0x887602BC,
    D3DERR_BADMINORVERSION = (int)0x887602BD,
    D3DERR_EXECUTE_CREATE_FAILED = (int)0x887602C6,
    D3DERR_EXECUTE_DESTROY_FAILED = (int)0x887602C7,
    D3DERR_EXECUTE_LOCK_FAILED = (int)0x887602C8,
    D3DERR_EXECUTE_UNLOCK_FAILED = (int)0x887602C9,
    D3DERR_EXECUTE_LOCKED = (int)0x887602CA,
    D3DERR_EXECUTE_NOT_LOCKED = (int)0x887602CB,
    D3DERR_EXECUTE_FAILED = (int)0x887602CC,
    D3DERR_EXECUTE_CLIPPED_FAILED = (int)0x887602CD,
    D3DERR_TEXTURE_NO_SUPPORT = (int)0x887602D0,
    D3DERR_TEXTURE_CREATE_FAILED = (int)0x887602D1,
    D3DERR_TEXTURE_DESTROY_FAILED = (int)0x887602D2,
    D3DERR_TEXTURE_LOCK_FAILED = (int)0x887602D3,
    D3DERR_TEXTURE_UNLOCK_FAILED = (int)0x887602D4,
    D3DERR_TEXTURE_LOAD_FAILED = (int)0x887602D5,
    D3DERR_TEXTURE_LOCKED = (int)0x887602D7,
    D3DERR_TEXTURE_NOT_LOCKED = (int)0x887602D8,
    D3DERR_MATRIX_CREATE_FAILED = (int)0x887602DA,
    D3DERR_MATRIX_DESTROY_FAILED = (int)0x887602DB,
    D3DERR_MATRIX_SETDATA_FAILED = (int)0x887602DC,
    D3DERR_SETVIEWPORTDATA_FAILED = (int)0x887602DE,
    D3DERR_INVALIDRAMPTEXTURE = (int)0x887602E3,
    D3DERR_MATERIAL_CREATE_FAILED = (int)0x887602E4,
    D3DERR_MATERIAL_DESTROY_FAILED = (int)0x887602E5,
    D3DERR_MATERIAL_SETDATA_FAILED = (int)0x887602E6,
    D3DERR_LIGHT_SET_FAILED = (int)0x887602EE
};

/* The DX5 SDK's D3DDP_* flags (<d3d.h>) passed to IDirect3DDevice2::Draw(Indexed)Primitive. */
enum Direct3DDrawPrimitiveFlags {
    D3DDP_DONOTCLIP = 0x00000004
};
#endif /* _D3D_H_ */

#ifndef _D3DTYPES_H_
/* The DX5 SDK's state and primitive codes (<d3dtypes.h>) that the code passes to
 * IDirect3DDevice2's SetRenderState, SetLightState, SetTransform and Draw(Indexed)Primitive;
 * a TU with <d3d.h> in scope has the SDK's enums instead,
 * so include this header after <d3d.h>. */
enum Direct3DRenderState {
    D3DRENDERSTATE_TEXTUREHANDLE = 1,
    D3DRENDERSTATE_ANTIALIAS = 2,
    D3DRENDERSTATE_TEXTUREADDRESS = 3,
    D3DRENDERSTATE_TEXTUREPERSPECTIVE = 4,
    D3DRENDERSTATE_WRAPU = 5,
    D3DRENDERSTATE_WRAPV = 6,
    D3DRENDERSTATE_ZENABLE = 7,
    D3DRENDERSTATE_FILLMODE = 8,
    D3DRENDERSTATE_SHADEMODE = 9,
    D3DRENDERSTATE_LINEPATTERN = 10,
    D3DRENDERSTATE_MONOENABLE = 11,
    D3DRENDERSTATE_ROP2 = 12,
    D3DRENDERSTATE_PLANEMASK = 13,
    D3DRENDERSTATE_ZWRITEENABLE = 14,
    D3DRENDERSTATE_ALPHATESTENABLE = 15,
    D3DRENDERSTATE_LASTPIXEL = 16,
    D3DRENDERSTATE_TEXTUREMAG = 17,
    D3DRENDERSTATE_TEXTUREMIN = 18,
    D3DRENDERSTATE_SRCBLEND = 19,
    D3DRENDERSTATE_DESTBLEND = 20,
    D3DRENDERSTATE_TEXTUREMAPBLEND = 21,
    D3DRENDERSTATE_CULLMODE = 22,
    D3DRENDERSTATE_ZFUNC = 23,
    D3DRENDERSTATE_ALPHAREF = 24,
    D3DRENDERSTATE_ALPHAFUNC = 25,
    D3DRENDERSTATE_DITHERENABLE = 26,
    D3DRENDERSTATE_ALPHABLENDENABLE = 27,
    D3DRENDERSTATE_FOGENABLE = 28,
    D3DRENDERSTATE_SPECULARENABLE = 29,
    D3DRENDERSTATE_ZVISIBLE = 30,
    D3DRENDERSTATE_SUBPIXEL = 31,
    D3DRENDERSTATE_SUBPIXELX = 32,
    D3DRENDERSTATE_STIPPLEDALPHA = 33,
    D3DRENDERSTATE_FOGCOLOR = 34,
    D3DRENDERSTATE_FOGTABLEMODE = 35,
    D3DRENDERSTATE_FOGTABLESTART = 36,
    D3DRENDERSTATE_FOGTABLEEND = 37,
    D3DRENDERSTATE_FOGTABLEDENSITY = 38,
    D3DRENDERSTATE_STIPPLEENABLE = 39,
    D3DRENDERSTATE_EDGEANTIALIAS = 40,
    D3DRENDERSTATE_COLORKEYENABLE = 41,
    D3DRENDERSTATE_BORDERCOLOR = 43,
    D3DRENDERSTATE_TEXTUREADDRESSU = 44,
    D3DRENDERSTATE_TEXTUREADDRESSV = 45,
    D3DRENDERSTATE_MIPMAPLODBIAS = 46,
    D3DRENDERSTATE_ZBIAS = 47,
    D3DRENDERSTATE_RANGEFOGENABLE = 48,
    D3DRENDERSTATE_ANISOTROPY = 49,
    D3DRENDERSTATE_FLUSHBATCH = 50
};

enum Direct3DLightState {
    D3DLIGHTSTATE_MATERIAL = 1,
    D3DLIGHTSTATE_AMBIENT = 2,
    D3DLIGHTSTATE_COLORMODEL = 3,
    D3DLIGHTSTATE_FOGMODE = 4,
    D3DLIGHTSTATE_FOGSTART = 5,
    D3DLIGHTSTATE_FOGEND = 6,
    D3DLIGHTSTATE_FOGDENSITY = 7
};

enum Direct3DTransformState {
    D3DTRANSFORMSTATE_WORLD = 1,
    D3DTRANSFORMSTATE_VIEW = 2,
    D3DTRANSFORMSTATE_PROJECTION = 3
};

enum Direct3DPrimitiveType {
    D3DPT_POINTLIST = 1,
    D3DPT_LINELIST = 2,
    D3DPT_LINESTRIP = 3,
    D3DPT_TRIANGLELIST = 4,
    D3DPT_TRIANGLESTRIP = 5,
    D3DPT_TRIANGLEFAN = 6
};

enum Direct3DVertexType {
    D3DVT_VERTEX = 1,
    D3DVT_LVERTEX = 2,
    D3DVT_TLVERTEX = 3
};

/* D3DRENDERSTATE_ZFUNC's values */
enum Direct3DCompare {
    D3DCMP_NEVER = 1,
    D3DCMP_LESS = 2,
    D3DCMP_EQUAL = 3,
    D3DCMP_LESSEQUAL = 4,
    D3DCMP_GREATER = 5,
    D3DCMP_NOTEQUAL = 6,
    D3DCMP_GREATEREQUAL = 7,
    D3DCMP_ALWAYS = 8
};

/* D3DRENDERSTATE_TEXTUREADDRESS's values */
enum Direct3DTextureAddress {
    D3DTADDRESS_WRAP = 1,
    D3DTADDRESS_MIRROR = 2,
    D3DTADDRESS_CLAMP = 3,
    D3DTADDRESS_BORDER = 4
};

/* The DX5 SDK's execute-buffer instruction opcodes (D3DOPCODE). */
enum Direct3DOpcode {
    D3DOP_POINT = 1,
    D3DOP_LINE = 2,
    D3DOP_TRIANGLE = 3,
    D3DOP_MATRIXLOAD = 4,
    D3DOP_MATRIXMULTIPLY = 5,
    D3DOP_STATETRANSFORM = 6,
    D3DOP_STATELIGHT = 7,
    D3DOP_STATERENDER = 8,
    D3DOP_PROCESSVERTICES = 9,
    D3DOP_TEXTURELOAD = 10,
    D3DOP_EXIT = 11,
    D3DOP_BRANCHFORWARD = 12,
    D3DOP_SPAN = 13,
    D3DOP_SETSTATUS = 14
};

/* The DX5 SDK's D3DEXECUTE_* flags (<d3dtypes.h>), IDirect3DDevice::Execute's third argument. */
enum Direct3DExecuteFlags {
    D3DEXECUTE_CLIPPED = 0x00000001,
    D3DEXECUTE_UNCLIPPED = 0x00000002
};

/* The DX5 SDK's D3DPROCESSVERTICES_* operations (<d3dtypes.h>), D3DPROCESSVERTICES::wFlags of an
 * execute buffer's D3DOP_PROCESSVERTICES instruction. */
enum Direct3DProcessVerticesOp {
    D3DPROCESSVERTICES_TRANSFORMLIGHT = 0x00000000,
    D3DPROCESSVERTICES_TRANSFORM = 0x00000001,
    D3DPROCESSVERTICES_COPY = 0x00000002
};

/* D3DRENDERSTATE_SRCBLEND/DESTBLEND's values (PolygonHundredState srcBlend, dstBlend) */
enum Direct3DBlend {
    D3DBLEND_ZERO = 1,
    D3DBLEND_ONE = 2,
    D3DBLEND_SRCCOLOR = 3,
    D3DBLEND_INVSRCCOLOR = 4,
    D3DBLEND_SRCALPHA = 5,
    D3DBLEND_INVSRCALPHA = 6
};

/* D3DRENDERSTATE_SHADEMODE's values (PolygonHundredState shadeMode) */
enum Direct3DShadeMode {
    D3DSHADE_FLAT = 1,
    D3DSHADE_GOURAUD = 2
};

/* D3DRENDERSTATE_FILLMODE's values */
enum Direct3DFillMode {
    D3DFILL_WIREFRAME = 2,
    D3DFILL_SOLID = 3
};

/* D3DRENDERSTATE_TEXTUREMAG/TEXTUREMIN's values */
enum Direct3DTextureFilter {
    D3DFILTER_NEAREST = 1,
    D3DFILTER_LINEAR = 2
};

/* D3DRENDERSTATE_CULLMODE's values */
enum Direct3DCull {
    D3DCULL_NONE = 1,
    D3DCULL_CW = 2,
    D3DCULL_CCW = 3
};
#endif /* _D3DTYPES_H_ */

/* The state of a record in the depth-sorted draw queue: what Gfx_Draw2DRecordD3D draws it as. */
enum GfxSortedRecordState {
    GFX_SORTED_RECORD_TRIANGLE = 0,
    GFX_SORTED_RECORD_QUAD = 1,
    GFX_SORTED_RECORD_BATCH_TRIANGLES = 2,
    GFX_SORTED_RECORD_BATCH_QUADS = 3,
    GFX_SORTED_RECORD_GROUP = 4,
    GFX_SORTED_RECORD_GROUP_TRIANGLE = 5
};

/* A PSX GPU packet's command byte (packet +7) shifted right by 2: the primitive kind
 * Gfx_DrawPsxPrimitive dispatches on (the libgpu struct names), and the opcode
 * Gfx_SetPsxPrimQuadState takes for the textured kinds. */
enum GfxPsxPrimKind {
    GFX_PSX_PRIM_POLY_F3 = 0x08,                  /* flat triangle */
    GFX_PSX_PRIM_POLY_FT3 = 0x09,                 /* flat textured triangle */
    GFX_PSX_PRIM_POLY_F4 = 0x0A,                  /* flat quad */
    GFX_PSX_PRIM_POLY_FT4 = 0x0B,                 /* flat textured quad */
    GFX_PSX_PRIM_POLY_G3 = 0x0C,                  /* Gouraud triangle */
    GFX_PSX_PRIM_POLY_GT3 = 0x0D,                 /* Gouraud textured triangle */
    GFX_PSX_PRIM_POLY_G4 = 0x0E,                  /* Gouraud quad */
    GFX_PSX_PRIM_POLY_GT4 = 0x0F,                 /* Gouraud textured quad */
    GFX_PSX_PRIM_LINE_F2 = 0x10,                  /* flat line */
    GFX_PSX_PRIM_LINE_G2 = 0x14,                  /* Gouraud line */
    GFX_PSX_PRIM_LINE_G3 = 0x16,                  /* Gouraud two-segment polyline */
    GFX_PSX_PRIM_TILE = 0x18,                     /* flat rectangle of any size */
    GFX_PSX_PRIM_SPRT = 0x19,                     /* textured sprite of any size */
    GFX_PSX_PRIM_TILE_1 = 0x1A,                   /* one-pixel dot */
    GFX_PSX_PRIM_TILE_8 = 0x1C,                   /* 8x8 flat rectangle */
    GFX_PSX_PRIM_SPRT_8 = 0x1D,                   /* 8x8 textured sprite */
    GFX_PSX_PRIM_TILE_16 = 0x1E,                  /* 16x16 flat rectangle */
    GFX_PSX_PRIM_SPRT_16 = 0x1F                   /* 16x16 textured sprite */
};

/* Bits of a PSX GPU packet's command byte (packet +7) that Gfx_DrawPsxPrimitive tests. */
enum GfxPsxPrimCodeFlag {
    GFX_PSX_PRIM_CODE_SEMI_TRANSPARENT = 0x02     /* drawn with the g_GfxPsxRenderMode blend set */
};

/* Raw PSX GPU packet command byte (packet +7: GfxPsxPrimKind << 2 | flags). */
enum PsxPrimCode {
    PSX_PRIM_POLY_F3 = 0x20,
    PSX_PRIM_POLY_FT3 = 0x24,
    PSX_PRIM_POLY_FT4 = 0x2C,
    PSX_PRIM_POLY_FT4_TRANS = 0x2E,
    PSX_PRIM_POLY_G3 = 0x30,
    PSX_PRIM_POLY_G4 = 0x38,
    PSX_PRIM_POLY_G4_TRANS = 0x3A,
    PSX_PRIM_TILE = 0x60
};

/* PolygonFileArrays::kind: the DX5 vertex layout a polygon set is
 * built as (D3D5BuildVertexArray) and drawn with (D3D5 batch vertexType = this + 1). */
enum GfxD3dVertexFormat {
    GFX_D3D_VERTEX_FORMAT_VERTEX = 0,             /* D3DVERTEX: position, normal, UV */
    GFX_D3D_VERTEX_FORMAT_LVERTEX = 1,            /* D3DLVERTEX: position, colour, specular, UV */
    GFX_D3D_VERTEX_FORMAT_TLVERTEX = 2            /* D3DTLVERTEX: screen position, rhw, colour, specular, UV */
};

/* SwLineOps::mode: what Gfx_RasterDrawLineBetweenVertices steps along a software-rasterized line besides depth. */
enum SwLineInterpMode {
    SW_LINE_INTERP_DEPTH = 0,                     /* depth only */
    SW_LINE_INTERP_DEPTH_UVW = 1                  /* depth, texture U and V, and W */
};

/* GfxDrawSpec::kind: the vertex-color operation Gfx_ApplyDrawSpecToGroup applies to each group.
 * "Saved" colors are the group's lit colors kept beside its vertices; "palette" is the per-group
 * palette index stored at drawSpec +0x34. */
enum GfxDrawSpecKind {
    GFX_DRAW_SPEC_KIND_NONE = 0x00,
    GFX_DRAW_SPEC_KIND_VERTEX_ALPHA = 0x01,
    GFX_DRAW_SPEC_KIND_VERTEX_COLOR = 0x05,
    GFX_DRAW_SPEC_KIND_SCALE_SAVED_INT = 0x08,              /* integer scale = alpha */
    GFX_DRAW_SPEC_KIND_BLEND_SAVED = 0x09,                  /* saved * (1 - a) + color * a */
    GFX_DRAW_SPEC_KIND_SCALE_SAVED = 0x0A,                  /* saved * alpha / 255 */
    GFX_DRAW_SPEC_KIND_BLEND_SAVED_OR_PALETTE = 0x0B,
    GFX_DRAW_SPEC_KIND_SCALE_SAVED_OR_PALETTE = 0x0C,
    GFX_DRAW_SPEC_KIND_MODULATE_SAVED_OR_PALETTE = 0x0D,
    GFX_DRAW_SPEC_KIND_RESTORE_COLORS = 0x0F,
    GFX_DRAW_SPEC_KIND_VERTEX_CALLBACK_WITH_SPEC = 0x10,
    GFX_DRAW_SPEC_KIND_VERTEX_COLOR_CALLBACK = 0x11,
    GFX_DRAW_SPEC_KIND_SAVED_COLOR_CALLBACK = 0x12
};

/* GfxDrawSpec::uvMode: what Gfx_ApplyDrawSpecToGroup does to a group's UVs. */
enum GfxDrawSpecUvMode {
    GFX_DRAW_SPEC_UV_KEEP = 0,
    GFX_DRAW_SPEC_UV_RESTORE = 1,                           /* UVs from the base copy */
    GFX_DRAW_SPEC_UV_SCROLL = 2                             /* base + (scrollU, scrollV), wrapped */
};

/* GfxMeshDrawParams::mode: how DxMesh_TransformAndDrawGroup sets each vertex's screen depth. */
enum GfxMeshDepthMode {
    GFX_MESH_DEPTH_KEEP = 0,
    GFX_MESH_DEPTH_ADD_BIAS = 1,                            /* depth + depthBias */
    GFX_MESH_DEPTH_REPLACE = 2                              /* depth = depthBias */
};

/* GfxDrawSpec::vMode: what Gfx_ApplyDrawSpecToGroup does to a group's V coordinates. */
enum GfxDrawSpecVMode {
    GFX_DRAW_SPEC_V_KEEP = 0,
    GFX_DRAW_SPEC_V_RESTORE = 1,                            /* V from the base copy */
    GFX_DRAW_SPEC_V_MIRROR = 2                              /* scrollV - base V */
};

/* GfxDrawSpec::mode: whether GfxFlushDrawList calls the spec's callback. */
enum GfxDrawSpecMode {
    GFX_DRAW_SPEC_MODE_NONE = 0,
    GFX_DRAW_SPEC_MODE_CALLBACK = 1
};

/* GfxEngineContext::scenePass: which of the two polygon sets the driver's SetScenePass enables
 * and draws (Gl_OpsSetScenePass, Gfx_SetScenePassD3D). */
enum GfxScenePass {
    GFX_SCENE_PASS_OPAQUE = 0,                              /* the opaque set */
    GFX_SCENE_PASS_TRANSLUCENT = 1,                         /* the translucent set */
    GFX_SCENE_PASS_ALL_IMMEDIATE = 3,                       /* draws both polygon sets immediately */
    GFX_SCENE_PASS_BOTH = 5                                 /* D3D_RenderDeferredLists: opaque, then translucent */
};

/* GfxPolygonSetHeader::kind: the transform a polygon set's draw-list nodes are drawn with. */
enum GfxPolygonSetTransformKind {
    GFX_POLYGON_SET_TRANSFORM_NONE = 0,                     /* no matrix */
    GFX_POLYGON_SET_TRANSFORM_NODE = 1,                     /* the node's matrix as given */
    GFX_POLYGON_SET_TRANSFORM_NODE_CAMERA = 2               /* the node's matrix times the camera matrix */
};

/* GlDrawOpNode::opcode: the path Gl_OpsDrawPolygonSet draws a node list by. */
enum GfxDrawOpKind {
    GFX_DRAW_OP_BATCH = 0,                                  /* the slot's polygon batch */
    GFX_DRAW_OP_BATCH_DRAW_SPEC = 1,                        /* the batch, after the node's GfxDrawSpec */
    GFX_DRAW_OP_DRAW_LIST = 2                               /* the node's draw-list primitives */
};

/* The software rasterizer's six clip planes (GfxRasterClipScratch::planeThreshold index, bit n of
 * Gfx_RasterClipTriangleToPlaneMask's mask): a vertex is inside a MIN plane when its coordinate is
 * >= the threshold and inside a MAX plane when it is <=. */
enum GfxRasterClipPlane {
    GFX_CLIP_PLANE_MIN_X = 0,
    GFX_CLIP_PLANE_MAX_X = 1,
    GFX_CLIP_PLANE_MIN_Y = 2,
    GFX_CLIP_PLANE_MAX_Y = 3,
    GFX_CLIP_PLANE_MIN_Z = 4,                               /* the first depth plane */
    GFX_CLIP_PLANE_MAX_Z = 5
};

/* The matrix slot of a polygon set's matrix record (+0x14 pointers, +0x20 handles):
 * D3DSetMatrix, D3DMultiplyMatrix, GetCachedDeviceTransform; the D3D5 submit path hands slot
 * n to SetTransform as D3DTRANSFORMSTATE_WORLD/VIEW/PROJECTION. */
enum GfxMatrixSlot {
    GFX_MATRIX_SLOT_WORLD = 0,
    GFX_MATRIX_SLOT_VIEW = 1,
    GFX_MATRIX_SLOT_PROJECTION = 2
};

/* D3DFillMaterialColorFromBgra's colorSlot: which D3DMATERIAL color it fills. */
enum GfxMaterialColorSlot {
    GFX_MATERIAL_COLOR_AMBIENT = 0,                         /* D3DMATERIAL +0x14 */
    GFX_MATERIAL_COLOR_DIFFUSE = 1,                         /* +0x04 */
    GFX_MATERIAL_COLOR_SPECULAR = 2,                        /* +0x24 */
    GFX_MATERIAL_COLOR_EMISSIVE = 3                         /* +0x34 */
};

/* Bits of PolygonHundredState::flags (the state's value) and changeMask (which states the block
 * sets), as D3DExecEmitRenderStateBlock and the OpenGL driver apply them; bit n is mostly
 * enum RenderState n. */
enum GfxRenderStateFlag {
    GFX_STATE_FLAG_WIREFRAME = 0x1,                         /* D3DRENDERSTATE_FILLMODE */
    GFX_STATE_FLAG_TEXTURE = 0x2,                           /* bind textureIndex's texture */
    GFX_STATE_FLAG_LINEAR_FILTER = 0x4,                     /* TEXTUREMAG/TEXTUREMIN */
    GFX_STATE_FLAG_PERSPECTIVE = 0x8,                       /* TEXTUREPERSPECTIVE */
    GFX_STATE_FLAG_DITHER = 0x200,                          /* DITHERENABLE */
    GFX_STATE_FLAG_ALPHA_BLEND = 0x400,                     /* ALPHABLENDENABLE with srcBlend, dstBlend */
    GFX_STATE_FLAG_ALPHA_TEST = 0x800,                      /* ALPHATESTENABLE with alphaRef */
    GFX_STATE_FLAG_ANTIALIAS = 0x1000,                      /* ANTIALIAS */
    GFX_STATE_FLAG_NO_CULL = 0x4000,                        /* CULLMODE D3DCULL_NONE */
    GFX_STATE_FLAG_DEPTH_TEST = 0x8000,                     /* ZENABLE */
    GFX_STATE_FLAG_DEPTH_WRITE = 0x10000,                   /* ZWRITEENABLE */
    GFX_STATE_FLAG_SHADE_MODE = 0x20000,                    /* SHADEMODE from shadeMode */
    GFX_STATE_FLAG_SPECULAR = 0x40000,                      /* SPECULARENABLE */
    GFX_STATE_FLAG_MATERIAL = 0x80000,                      /* D3DLIGHTSTATE_MATERIAL from material */
    GFX_STATE_FLAG_FOG = 0x100000                           /* FOGENABLE */
};

/* PolygonPlyEntry::kind, from the type letter of a MAT file's face line (Pfile_ParsePlyAndMat);
 * also PolygonFileGroup::kind and what Pfile_BuildHundredFromMaterial shades and textures. */
enum PfileFaceKind {
    PFILE_FACE_FLAT = 0,                                    /* 'C': one colour */
    PFILE_FACE_GOURAUD = 1,                                 /* 'G': a colour per vertex */
    PFILE_FACE_TEXTURED = 2,                                /* 'T': texture coordinates, drawn white */
    PFILE_FACE_TEXTURED_FLAT = 3,                           /* 'D': texture coordinates and one colour */
    PFILE_FACE_TEXTURED_GOURAUD = 4                         /* 'H': texture coordinates and a colour per vertex */
};

/* The primitive type that starts each polygon line of a PLY file (Pfile_ParsePlyAndMat). */
enum PfilePlyPrimitive {
    PFILE_PLY_TRIANGLE = 0,                                 /* vertex and normal indices; drawn as one triangle */
    PFILE_PLY_QUAD = 1,                                     /* the same; drawn as two triangles */
    PFILE_PLY_LINE = 2,                                     /* two vertex indices; not drawn */
    PFILE_PLY_SPRITE = 3                                    /* one vertex index and two sizes; not drawn */
};

/* How a face line of a MAT file names its faces (Pfile_ParsePlyAndMat). */
enum PfileMatFaceList {
    PFILE_MAT_FACES_LIST = 0,                               /* "a,b,c" */
    PFILE_MAT_FACES_RANGE = 1,                              /* "a-b" */
    PFILE_MAT_FACES_SINGLE = 2                              /* "a" */
};

/* Indices of g_AnimTextKeywords, the keywords of animation text and HRC files that
 * AnimText_LookupKeyword returns (0 when the token is none of them). */
enum AnimTextKeyword {
    ANIM_KEYWORD_NONE = 0,
    ANIM_KEYWORD_HEADER_BLOCK = 1,
    ANIM_KEYWORD_SCRIPT = 2,
    ANIM_KEYWORD_SKELETON = 3,
    ANIM_KEYWORD_FRAMES = 4,
    ANIM_KEYWORD_CHANNELS = 5,
    ANIM_KEYWORD_ROTATION_ORDER = 6,
    ANIM_KEYWORD_ROTATE_RESOLUTION = 7,
    ANIM_KEYWORD_BONES = 8,
    ANIM_KEYWORD_ROOT_COORDINATE = 9,
    ANIM_KEYWORD_TX = 10,
    ANIM_KEYWORD_TY = 11,
    ANIM_KEYWORD_TZ = 12,
    ANIM_KEYWORD_RX = 13,
    ANIM_KEYWORD_RY = 14,
    ANIM_KEYWORD_RZ = 15
};

/* Anim_SetRootChannel's channel: one value of a frame's root pose, or the whole pose. */
enum AnimRootChannel {
    ANIM_ROOT_TRANSLATE_X = 0,
    ANIM_ROOT_TRANSLATE_Y = 1,
    ANIM_ROOT_TRANSLATE_Z = 2,
    ANIM_ROOT_ROTATE_X = 3,
    ANIM_ROOT_ROTATE_Y = 4,
    ANIM_ROOT_ROTATE_Z = 5,
    ANIM_ROOT_ALL = 6
};

/* Anim_SetBoneChannel's channel: one axis of a bone's rotation in a frame, or all three. */
enum AnimBoneChannel {
    ANIM_BONE_X = 0,
    ANIM_BONE_Y = 1,
    ANIM_BONE_Z = 2,
    ANIM_BONE_ALL = 3
};

/* Rotation axis numbers: the animation's rotation order (ROTATION_ORDER letters) and the axis of
 * AnimPlacement_GetAngleByIndex/SetAngleByIndex; also the axis RotateMatrixAboutAxis and the
 * dx_mat axis-rotation builders rotate about. */
enum AnimAxis {
    ANIM_AXIS_X = 0,
    ANIM_AXIS_Y = 1,
    ANIM_AXIS_Z = 2
};

/* What Gfx_RasterClassifyRectAgainstClipBounds returns for a rectangle against the context's
 * clip bounds; for the clipped results it stores how far each edge lies outside. */
enum GfxRectClipStatus {
    GFX_RECT_CLIP_INSIDE = 0,
    GFX_RECT_CLIP_HORIZONTAL = 1,                           /* clipped at the left or right */
    GFX_RECT_CLIP_VERTICAL = 2,                             /* clipped at the top or bottom */
    GFX_RECT_CLIP_BOTH = 3,
    GFX_RECT_CLIP_OUTSIDE = 4                               /* no overlap */
};

/* GfxTexturePageFill::mode: where Gfx_CreateSurfaceTexture and its OpenGL twin take a texture
 * page's pixels from; Gl_ReadFrameBuffer's bufferMode (the front or back buffer). */
enum GfxTexturePageFillSource {
    GFX_FILL_SOURCE_FRONT_BUFFER = 0,                       /* glReadBuffer(GL_FRONT) */
    GFX_FILL_SOURCE_BACK_BUFFER = 1,                        /* glReadBuffer(GL_BACK) */
    GFX_FILL_SOURCE_PIXELS = 2                              /* the fill's own format and pixels */
};

/* A PSX texture page's colour mode (tpage bits 7-8, Psx_TPageToColorMode): the texel depth, which sets
 * how many texels one 16-bit VRAM word holds and how wide a texture page is. */
enum GfxPsxColorMode {
    GFX_PSX_COLOR_MODE_4BIT = 0,   /* four texels per VRAM word, a 64-word-wide page */
    GFX_PSX_COLOR_MODE_8BIT = 1,   /* two texels per word, 128 words */
    GFX_PSX_COLOR_MODE_16BIT = 2   /* one texel per word, 256 words */
};

/* GlDrawGroup::flags and ::changeMask (PolygonHundredState's): the render states a draw group
 * sets, and which of them changed. Most bits are 1 << the RenderState code; Gl_ApplyRenderState and the D3D state-block
 * emitters apply them. */
enum GfxDrawGroupFlag {
    GFX_DRAW_GROUP_TEXTURED = 0x2,                          /* the group's texture is bound */
    GFX_DRAW_GROUP_LINEAR_FILTER = 0x4,
    GFX_DRAW_GROUP_PERSPECTIVE = 0x8,
    GFX_DRAW_GROUP_WRAP_U = 0x20,
    GFX_DRAW_GROUP_WRAP_V = 0x40,
    GFX_DRAW_GROUP_COLOR_KEY = 0x100,
    GFX_DRAW_GROUP_DITHER = 0x200,
    GFX_DRAW_GROUP_SEMI_TRANSPARENT = 0x400,                /* the group's blendMode, else BLEND_NONE */
    GFX_DRAW_GROUP_CULL_FRONT = 0x2000,
    GFX_DRAW_GROUP_NO_CULL = 0x4000,
    GFX_DRAW_GROUP_DEPTH_TEST = 0x8000,
    GFX_DRAW_GROUP_DEPTH_WRITE = 0x10000,
    GFX_DRAW_GROUP_SHADE_MODE = 0x20000                     /* the group's shade mode, else flat */
};

/* g_AnmTransformFlags (Anim_SetTransformFlags): how lib/polygon/anm builds the root and placement matrices. */
enum AnimTransformFlag {
    ANIM_TRANSFORM_ROTATE = 0x0001,                         /* rotate the root by the frame's root angles */
    ANIM_TRANSFORM_NEGATE_ROT_X = 0x0002,
    ANIM_TRANSFORM_NEGATE_ROT_Y = 0x0004,
    ANIM_TRANSFORM_NEGATE_ROT_Z = 0x0008,
    ANIM_TRANSFORM_TRANSLATE = 0x0010,                      /* take the root position from the frame */
    ANIM_TRANSFORM_NEGATE_POS_X = 0x0020,
    ANIM_TRANSFORM_NEGATE_POS_Y = 0x0040,
    ANIM_TRANSFORM_NEGATE_POS_Z = 0x0080,
    ANIM_TRANSFORM_OFFSET_BY_YAW = 0x0400,                  /* turn the placement offset by its Y rotation */
    ANIM_TRANSFORM_PLACEMENT_MATRIX = 0x4000,               /* multiply the root by the placement's matrix */
    ANIM_TRANSFORM_CAMERA_BONES = 0x8000                    /* combine each bone matrix with the camera matrix */
};

/* PolygonAnimBakeNode.opcode: a skeleton's push/pop node chain. */
enum AnimSkeletonNodeOp {
    ANIM_NODE_NONE = 0,
    ANIM_NODE_PUSH_BONE = 1,                                /* build the bone's matrix on its parent's and push it */
    ANIM_NODE_POP = 2
};

/* Skeleton model flags (PolygonAnimDraw.flags, PolygonAnimModel.flags; set from the load options) that
   Anim_DrawSkeletonFrame and Anim_BakeFrameMatrices test. */
enum PolygonModelFlag {
    POLYGON_MODEL_SAVE_ROOT_BEFORE_PLACEMENT = 0x00000400,  /* copy the root matrix to rootBeforePlacement */
    POLYGON_MODEL_SAVE_ROOT_AFTER_PLACEMENT = 0x00000800,   /* copy the root matrix to rootAfterPlacement */
    POLYGON_MODEL_WRITE_BONE_POSITIONS = 0x00002000,        /* write root and bone positions to transform->outPos */
    POLYGON_MODEL_STORE_CAMERA_BONES = 0x00004000,          /* keep the camera bone matrices in transform->transformBase */
    POLYGON_MODEL_DRAW = 0x00100000,                        /* draw the bones' polygon sets */
    POLYGON_MODEL_LIGHT = 0x02000000                        /* light the bones' polygon sets through their light owner */
};

/* GfxOffscreenRequest::kind: what Gfx_BuildOffscreenRenderObject renders into the texture. */
enum GfxOffscreenRequestKind {
    GFX_OFFSCREEN_REQUEST_NONE = 0,                         /* no scene; only the surface is built */
    GFX_OFFSCREEN_REQUEST_MESH = 1,                         /* request->mesh by its model matrix */
    GFX_OFFSCREEN_REQUEST_SKELETON = 2                      /* request->skeleton at its animation frame */
};

/* GfxOffscreenSlot::inUse: a slot's life. Gfx_AllocOffscreenSlot takes a FREE slot to PENDING,
 * Gfx_ServiceOffscreenSlots builds PENDING slots (BUILT) and releases RELEASE ones; BUILT and
 * RELEASE slots are drawn by Gfx_DrawOffscreenSlots. */
enum GfxOffscreenSlotState {
    GFX_OFFSCREEN_SLOT_FREE = 0,
    GFX_OFFSCREEN_SLOT_PENDING = 1,
    GFX_OFFSCREEN_SLOT_BUILT = 2,
    GFX_OFFSCREEN_SLOT_RELEASE = 3
};

#ifndef _D3DCAPS_H
/* The DX5 SDK's D3DDD_* bits (<d3dcaps.h>) of D3DDEVICEDESC::dwFlags: which members are valid,
 * as Gfx_LogD3DDeviceDesc tests them. */
enum Direct3DDeviceDescFlags {
    D3DDD_COLORMODEL = 0x00000001,
    D3DDD_DEVCAPS = 0x00000002,
    D3DDD_TRANSFORMCAPS = 0x00000004,
    D3DDD_LIGHTINGCAPS = 0x00000008,
    D3DDD_BCLIPPING = 0x00000010,
    D3DDD_LINECAPS = 0x00000020,
    D3DDD_TRICAPS = 0x00000040
};

/* The DX5 SDK's D3DLIGHTINGMODEL_* values (<d3dcaps.h>) of D3DLIGHTINGCAPS::dwLightingModel. */
enum Direct3DLightingModel {
    D3DLIGHTINGMODEL_RGB = 0x00000001,
    D3DLIGHTINGMODEL_MONO = 0x00000002
};
#endif /* _D3DCAPS_H */

#ifndef _D3DTYPES_H_
/* The DX5 SDK's D3DCOLOR_* color models (<d3dtypes.h>) of D3DDEVICEDESC::dcmColorModel. */
enum Direct3DColorModel {
    D3DCOLOR_MONO = 1,
    D3DCOLOR_RGB = 2
};
#endif /* _D3DTYPES_H_ */

/* The low byte of a PlayStation TIM file's id word (Pfile_LoadTimFromContext). */
enum TimFileId {
    TIM_FILE_ID = 0x10
};

/* The flags word of a TIM header: the pixel mode in the low three bits and the CLUT bit. */
enum TimHeaderFlag {
    TIM_FLAG_PIXEL_MODE_MASK = 0x7,               /* a TimPixelMode */
    TIM_FLAG_HAS_CLUT = 0x8                       /* a palette block precedes the image */
};

/* A TIM's pixel mode (header flags & TIM_FLAG_PIXEL_MODE_MASK). */
enum TimPixelMode {
    TIM_PIXEL_MODE_4BPP = 0,                      /* 16-colour CLUT */
    TIM_PIXEL_MODE_8BPP = 1,                      /* 256-colour CLUT */
    TIM_PIXEL_MODE_16BPP = 2,                     /* direct 15-bit colour */
    TIM_PIXEL_MODE_24BPP = 3                      /* direct 24-bit colour */
};

/* GfxEngineContext::setupFlags: what the driver's bring-up (the Direct3D and software drivers in
 * InitGraphicsDevice) sets up. */
enum GfxSetupFlag {
    GFX_SETUP_DIRECTDRAW = 0x1, /* the DirectDraw object, the display mode and the surfaces */
    GFX_SETUP_DIRECT3D = 0x2    /* the Direct3D device, its Z buffer and viewports */
};

/* GfxModelTableRecord.flags bits that Gfx_LoadModelTableFromRecords and Gfx_ModelTableDrawLayerOrExpire
 * test (the low three bits are the BlendMode). */
enum GfxModelRecordFlag {
    GFX_MODEL_RECORD_BLEND_MASK = 0x0007,
    GFX_MODEL_RECORD_GOURAUD = 0x0040,       /* the render object is Gouraud shaded */
    GFX_MODEL_RECORD_TRIANGLES = 0x0400,     /* the render object draws triangles, not quads */
    GFX_MODEL_RECORD_RENDER_OBJECT = 0x0800, /* a render object, not an RSD model */
    GFX_MODEL_RECORD_SCREEN_2D = 0x1000,     /* a 2D render object, drawn as a primitive batch */
    GFX_MODEL_RECORD_NO_CULL = 0x4000        /* drawn with RENDER_STATE_NO_CULL set */
};

/* GfxModelTableEntry.kind: how an entry is drawn. A semi-transparent entry (blend modes AVERAGE to
 * QUARTER_ADDITIVE) has GFX_MODEL_ENTRY_TRANSLUCENT added and is drawn in the translucent layer. */
enum GfxModelEntryKind {
    GFX_MODEL_ENTRY_MESH = 0,          /* an RSD model: Gfx_DrawMeshPolygonSet */
    GFX_MODEL_ENTRY_RENDER_OBJECT = 1, /* Gfx_DrawRenderObjectCall */
    GFX_MODEL_ENTRY_BATCH = 2,         /* Gfx_EmitPrimitiveBatchList */
    GFX_MODEL_ENTRY_TRANSLUCENT = 4,
    GFX_MODEL_ENTRY_TRANSLUCENT_MESH = 4,
    GFX_MODEL_ENTRY_TRANSLUCENT_RENDER_OBJECT = 5,
    GFX_MODEL_ENTRY_TRANSLUCENT_BATCH = 6
};

/* The layer argument of Gfx_ModelTableDrawLayerOrExpire: which entries a pass draws. */
enum GfxModelTableLayer {
    GFX_MODEL_LAYER_OPAQUE = 0,
    GFX_MODEL_LAYER_TRANSLUCENT = 1
};

/* What Gfx_ModelTableDrawLayerOrExpire returns for a table. */
enum GfxModelTableDrawResult {
    GFX_MODEL_TABLE_NONE = 0,    /* no table */
    GFX_MODEL_TABLE_EXPIRED = 1, /* group 0, past its lifetime: the bank removes it */
    GFX_MODEL_TABLE_HIDDEN = 2,  /* past its first ticks, not drawn */
    GFX_MODEL_TABLE_DRAWN = 3
};

/* GfxPaletteRecord::flags (Gfx_CreatePaletteRecord's flags): the extra copies of the entries the
 * record keeps, as floats (floatEntries) and as plain colors (colorEntries). */
enum GfxPaletteRecordFlag {
    GFX_PALETTE_FLOAT_ENTRIES = 0x1,
    GFX_PALETTE_COLOR_ENTRIES = 0x2
};

/* GfxLightPolygonSet::flags: bits 0-1 are the lighting mode Field_ApplyPolygonSetLighting{Gated,Ungated}
 * run; TINTED selects the calls that take colorBgra as a normalized tint, FLOOR the floor-lit calls. */
enum GfxLightFlag {
    GFX_LIGHT_MODE_MASK = 0x3,
    GFX_LIGHT_FLAG_TINTED = 0x4,
    GFX_LIGHT_FLAG_FLOOR = 0x8
};

/* GfxLightPolygonSet::flags & GFX_LIGHT_MODE_MASK. */
enum GfxLightMode {
    GFX_LIGHT_MODE_NONE = 0,
    GFX_LIGHT_MODE_DIFFUSE = 1,            /* vertex colors from the light records */
    GFX_LIGHT_MODE_SPECULAR = 2,           /* g_FieldSpecularDispatchTable[specularIndex] per vertex */
    GFX_LIGHT_MODE_DIFFUSE_SPECULAR = 3    /* both, per vertex */
};



enum GfxPsxDisplayGroup {
    GFX_PSX_DISPLAY_GROUP_OPAQUE = 0,
    GFX_PSX_DISPLAY_GROUP_MAIN = 1,
    GFX_PSX_DISPLAY_GROUP_OVERLAY_A = 2,
    GFX_PSX_DISPLAY_GROUP_OVERLAY_B = 3
};

enum GfxPsxClutDepth {
    GFX_PSX_CLUT_DEPTH_4BPP = 4,
    GFX_PSX_CLUT_DEPTH_8BPP = 8
};

enum GfxDrawSpecTexStateMode {
    GFX_DRAW_SPEC_TEX_STATE_KEEP = 0,
    GFX_DRAW_SPEC_TEX_STATE_APPLY = 1
};

enum GfxDrawSpecTexPageMode {
    GFX_DRAW_SPEC_TEX_PAGE_KEEP = 0,
    GFX_DRAW_SPEC_TEX_PAGE_SELECT = 1
};

enum GfxGouraudShadeFlag {
    GFX_SHADE_FLAG_FLAT = 0,
    GFX_SHADE_FLAG_GOURAUD = 1
};

/* Palette bit-depth selector passed to D3DCreatePaletteObject. */
enum GfxPaletteBitDepth {
    GFX_PALETTE_DEPTH_1BIT = 1,
    GFX_PALETTE_DEPTH_2BIT = 2,
    GFX_PALETTE_DEPTH_4BIT = 4
};

/* Z-buffer clear word width in bytes (SwZClearState::mode in Gfx_RasterClearZBufferRectIfDue). */
enum SwZClearMode {
    SW_ZCLEAR_8BIT = 1,
    SW_ZCLEAR_16BIT = 2,
    SW_ZCLEAR_32BIT = 4
};

/* GfxSpecularState::mode in Gfx_UpdateCachedSpecularColor. */
enum GfxSpecularMode {
    GFX_SPECULAR_MODE_CACHED = 2
};

enum GfxColorBitDepth {
    GFX_BPP_4 = 4,
    GFX_BPP_8 = 8,
    GFX_BPP_16 = 0x10,
    GFX_BPP_24 = 0x18,
    GFX_BPP_32 = 0x20
};

enum GfxPixelByteStride {
    GFX_BYTES_PER_PIXEL_1 = 1,
    GFX_BYTES_PER_PIXEL_2 = 2,
    GFX_BYTES_PER_PIXEL_3 = 3,
    GFX_BYTES_PER_PIXEL_4 = 4
};

enum GfxPsxVertexSlot {
    GFX_PSX_VERTEX_SLOT_0 = 0,
    GFX_PSX_VERTEX_SLOT_1 = 1,
    GFX_PSX_VERTEX_SLOT_2 = 2,
    GFX_PSX_VERTEX_SLOT_3 = 3
};

enum GfxModelTableGroupTag {
    GFX_MODEL_GROUP_DEFAULT = 0
};


enum AnimRotationAxisCount {
    ANIM_ROT_AXIS_NONE = 0,
    ANIM_ROT_AXIS_SINGLE = 1,
    ANIM_ROT_AXIS_DOUBLE = 2,
    ANIM_ROT_AXIS_TRIPLE = 3
};

#endif
