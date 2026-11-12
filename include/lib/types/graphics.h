/* include/lib/types/graphics.h: the types lib/graphics TUs share.
 * One definition per type. */
#ifndef LIB_TYPES_GRAPHICS_H
#define LIB_TYPES_GRAPHICS_H

#include "lib/types/mem.h"

/* The PSX geometry types the PC GTE emulation (psx.cpp, psxgraph.cpp) takes, under the PsyQ libgte
 * names the ported PSX code was written against (the PC function names are libgte's too,
 * SetRotMatrix, RotTransPers). The bodies read three shorts / three ints / three colour bytes at
 * +0/+2/+4, +0/+4/+8 and +0/+1/+2, PsyQ's SVECTOR, VECTOR and CVECTOR layouts. The matrix keeps
 * its own name, PsxMatrix: it is packed (the PC code reads the translation at +0x12, +0x16 and
 * +0x1A and copies the matrix as 30 bytes, PopRotMatrix), where PsyQ's MATRIX pads it to +0x14. */
typedef struct SVECTOR {
    short vx;
    short vy;
    short vz;
    short pad;
} SVECTOR;

/* VECTOR -- the fixed-point vector the PSX GTE helpers take.
 *
 * Sixteen bytes, three lanes and a pad, the PSX libgte VECTOR layout. What
 * confirms it on the PC
 * side is Gte_BuildLookBasisFromDirection at 0x005D2C1F: its 0x30-byte frame
 * is three of these, its up vector is written as an initializer -- two
 * immediates, then `xor eax, eax` for the pad, which is the only way VC5
 * spells the implicit zero of a partial aggregate initializer -- and every
 * fourth dword is never read.
 */
typedef struct VECTOR {
    int vx;
    int vy;
    int vz;
    int pad;
} VECTOR;

typedef struct CVECTOR {
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char cd;
} CVECTOR;

#pragma pack(push, 1)
typedef struct PsxMatrix {
    short m[3][3];
    int t[3];
} PsxMatrix;
#pragma pack(pop)
typedef char CheckPsxMatrix[(sizeof(PsxMatrix) == 30) ? 1 : -1];

#pragma pack(push, 1)
typedef struct PsxMatrix32 {
    union {
        PsxMatrix mat;
        struct {
            short m[3][3];
            int t[3];
        };
    };
    short pad;
} PsxMatrix32;
#pragma pack(pop)
typedef char CheckPsxMatrix32[(sizeof(PsxMatrix32) == 32) ? 1 : -1];

typedef union GfxArgb32 {
    unsigned int packed;
    struct {
        unsigned char b, g, r, a;
    } ch;
} GfxArgb32;

/* Seventeen-dword polygon-group draw specification copied into submitted GfxPolygonSetNode records. */
typedef struct GfxPolygonGroupDrawSpec {
    int w[17];
} GfxPolygonGroupDrawSpec;

/* Three-component direction vector copied into a light record. */
typedef struct GfxDirectionVec3 {
    int x;
    int y;
    int z;
} GfxDirectionVec3;

/* The 0x40 bytes Gl_OpsSetMatrix moves. It is a struct assignment and
   not a memcpy: the image loads esi before edi, and the intrinsic loads
   edi first. */
typedef struct GfxMatrix4x4 {
    union {
        float m[4][4];
        float v[16];
        struct {
            float m00, m01, m02, m03;
            float m10, m11, m12, m13;
            float m20, m21, m22, m23;
            float m30, m31, m32, m33;
        };
        struct {
            float _11, _12, _13, _14;
            float _21, _22, _23, _24;
            float _31, _32, _33, _34;
            float _41, _42, _43, _44;
        };
    };
} GfxMatrix4x4;
typedef char CheckGfxMatrix4x4[(sizeof(GfxMatrix4x4) == 64) ? 1 : -1];

/* A surface's pixel format (0x80 bytes). PixelFormatFromDDPixelFormat fills +0x10..+0x18
 * from a DDPIXELFORMAT's flags and hands its bit count and masks to PixelFormatSetRgbMasks, which
 * fills +0x28..+0x7C; Gl_PixelFormatFromPfd and PixelFormatLoadMaskTemplate do the same from a GL
 * pixel format and a mask template. GfxLockSurface stores the locked surface's size, pitch and
 * memory at +0..+0xC. It is the engine context's targetFormat, the head of each texture format
 * record, a .TEX header's format at +0x3C and the page-fill request's at +0x34.
 * PackArgb8888ToPixelFormat and UnpackPixelToArgb8888 convert through it; 0x80 bytes is what the
 * copies move (`rep movsd`, ecx 0x20) and what GfxUploadTexturePixels zero-fills. */
typedef struct GfxPixelFormat {
    int width; /* +0x00 */
    int height; /* +0x04 */
    int pitch; /* +0x08: bytes per row */
    int pixels; /* +0x0C: the surface memory (the locked DDSURFACEDESC's lpSurface) */
    int palettized; /* +0x10: set for DDPF_PALETTEINDEXED1/2/4/8; the pixel is a palette index */
    int indexBits; /* +0x14: 1, 2, 4 or 8 */
    int indexedTo8; /* +0x18: DDPF_PALETTEINDEXEDTO8 */
    int paletteEntryCount; /* +0x1C */
    int entriesPerPalette; /* +0x20: UnpackPixelRowToArgb8888 indexes palette `row` with it */
    union {
        struct PolygonTimColor *palette; /* +0x24: paletteEntryCount ARGB8888 words */
        int *paletteEntries;
        struct TileBgra *tilePalette;
    };
    int bitsPerPixel; /* +0x28 */
    int bytesPerPixel; /* +0x2C: bitsPerPixel / 8 + (bitsPerPixel & 1) */
    int redBits; /* +0x30 */
    int greenBits; /* +0x34 */
    int blueBits; /* +0x38 */
    int alphaBits; /* +0x3C: 0 for a format without alpha */
    int redMask; /* +0x40 */
    int greenMask; /* +0x44 */
    int blueMask; /* +0x48 */
    int alphaMask; /* +0x4C */
    int redShift; /* +0x50: the lowest set bit of the mask */
    int greenShift; /* +0x54 */
    int blueShift; /* +0x58 */
    int alphaShift; /* +0x5C */
    int redLoss; /* +0x60: 8 - bits, 0 for a channel without bits */
    int greenLoss; /* +0x64 */
    int blueLoss; /* +0x68 */
    int alphaLoss; /* +0x6C */
    unsigned int redMax; /* +0x70: (1 << bits) - 1 */
    unsigned int greenMax; /* +0x74 */
    unsigned int blueMax; /* +0x78 */
    unsigned int alphaMax; /* +0x7C */
} GfxPixelFormat;
typedef char CheckGfxPixelFormat[(sizeof(GfxPixelFormat) == 0x80) ? 1 : -1];

/* The twelve bytes Gl_OpsInit hands to Gl_InitConfigDefaults and then to
   Gl_CreateContext. Its size is read off that function's frame: 0x14
   bytes for two scalars and one object whose address is taken at
   [ebp-0xc], with a store to [ebp-4] that is the object's third member.
   Only that member's use is known, so only it is named. */
typedef struct GlContextConfig {
    int frontFaceClockwise;
    int doubleBuffered;
    int flags;
} GlContextConfig;

/* The 16 bytes Gl_OpsSetBackgroundColor moves into the GL state at +8.
   It is one assignment, not four: both base pointers are loaded once and
   the four members follow, where four scalar assignments at /Od reload
   both bases every time. */
typedef struct Rgba {
    int r;
    int g;
    int b;
    int a;
} Rgba;

/* Four-dword BGRA float-bit color passed to GfxSetBackgroundColorCall. */
typedef struct GfxClearColorBgra {
    int b;
    int g;
    int r;
    int a;
} GfxClearColorBgra;

/* Texture load result returned by Pfile_LoadRsdTexture (status, TIM record, cached texture handle). */
typedef struct DxTextureResult {
    int status, tim, texture;
} DxTextureResult;

typedef struct BlitRgb24 {
    unsigned char b;
    unsigned char g;
    unsigned char r;
} BlitRgb24;

#pragma pack(push, 4)
typedef struct {
    double timeGdi;
    double avgTimeM43;
    double avgTimeM4;
    int culledPolyCount;
    double timeM43;
    double avgTimeVt;
    float timeTs;
    double timeM4;
    double avgTimeTd;
    double avgTimeE;
    double avgTimeLvc;
    double timeLvc;
    double timeSst;
    double timeMr;
    float timeTc;
    double avgTimeMs;
    double timeSut;
    double avgTimeSut;
    float timeTd;
    float swTotalTime;
    double avgTimeMr;
    double avgTimeM3;
    double timeF;
    double timeM3;
    double timeMs;
    double avgTimeSst;
    double timeA;
    float timeVt;
    char text[200];
} DebugFrame;
#pragma pack(pop)

typedef union SwArgbColor {
    unsigned int packed;
    struct {
        unsigned char b, g, r, a;
    } ch;
} SwArgbColor;

#ifdef __cplusplus
/* The callback Gfx_ForEachPolygonSetGeometry calls for each part of a polygon set. */
typedef void(__cdecl *DxGroupCallback)(int part, int geometry);

typedef void(__cdecl *DxGroupPairCallback)(int part, int geometry, int vector, int other);

typedef void(__cdecl *DxGroupPartCallback)(int part, int vector, int geometry);


#endif

/* Resource load request: model table key, default duration and resource path. */
typedef struct GfxResourceRequest {
    int groupTag;
    int duration;
    char *resourcePath;
} GfxResourceRequest;

/* Depth-sort record holding a copied PSX primitive's vertex and colour words for later drawing. */
typedef struct GfxSortedPrimitive {
    int state;
    float sortDepth; /* +0x04: the depth Gfx_InsertSortedDrawRecord keyed the record by */
    int group;
    int batch;
    int texturePage;
    int renderState; /* a GfxGroupRenderState */
    int rasterGroup;
    GfxMatrix4x4 inlineMatrix; /* +0x1C: the matrix of a whole-group record */
    int matrixRef;
    int polyRecords;
    int batchPtr;
    int primData;
    unsigned char pad_6C[0x4];
    int vertex0X;
    int vertex0Y;
    int vertex0Z;
    int vertex0Rhw;
    int vertex0Color;
    int vertex0Specular;
    int vertex0U;
    int vertex0V;
    int vertex1X;
    int vertex1Y;
    int vertex1Z;
    int vertex1Rhw;
    int vertex1Color;
    int vertex1Specular;
    int vertex1U;
    int vertex1V;
    int vertex2X;
    int vertex2Y;
    int vertex2Z;
    int vertex2Rhw;
    int vertex2Color;
    int vertex2Specular;
    int vertex2U;
    int vertex2V;
    int vertex3X;
    int vertex3Y;
    int vertex3Z;
    int vertex3Rhw;
    int vertex3Color;
    int vertex3Specular;
    int vertex3U;
    int vertex3V;
} GfxSortedPrimitive;

struct GfxBatchNode;
struct GfxDrawListNode;
struct GfxEngineContext;
struct PolygonFileArrays;
struct GfxLightOwner;
struct GfxMatrixArray;
struct GfxPaletteRecord;
struct GfxPolygonSet;
struct GfxPrimitiveMeshBank;
struct GfxRenderObject;
struct GfxTextureSurface;
struct PolygonHundredState;
struct PolygonSkeletonFrameRef;

typedef int (__cdecl *GfxDriverInitFn)(struct GfxEngineContext *context);
typedef void (__cdecl *GfxDriverCleanupFn)(struct GfxEngineContext *context);
typedef int (__cdecl *GfxDriverSurfaceFn)(int surface);
typedef void (__cdecl *GfxDriverContextFn)(struct GfxEngineContext *context);
typedef void (__cdecl *GfxDriverClearViewportFn)(int clearTarget, int clearDepth, int clearValue, struct GfxEngineContext *context);
typedef void (__cdecl *GfxDriverSetViewportFn)(int x, int y, int width, int height, struct GfxEngineContext *context);
typedef void (__cdecl *GfxDriverSetBgColorFn)(void *color, struct GfxEngineContext *context);
typedef int (__cdecl *GfxDriverAllocBatchFn)(struct GfxPolygonSet *polygonSet);
typedef int (__cdecl *GfxDriverCompileGroupFn)(int groupIndex, struct GfxMatrixArray *matrices, struct PolygonHundredState *groupState, int groupRecord, struct PolygonFileArrays *lightModel, struct GfxPolygonSet *polygonSet, struct GfxEngineContext *context);
typedef void (__cdecl *GfxDriverSetMatrixFn)(int slot, int matrix, int mode, struct GfxEngineContext *context);
typedef void (__cdecl *GfxDriverUnloadTextureFn)(struct GfxTextureSurface *texture);
typedef int (__cdecl *GfxDriverLoadTextureFn)(int unused, int request, int format);
typedef int (__cdecl *GfxDriverPaletteChangedFn)(int entryOffset, int count, int startEntry, void *palette, struct GfxTextureSurface *texture);
typedef int (__cdecl *GfxDriverWritePaletteFn)(int startIndex, int count, int entries, int flags, int palette, int extra);
typedef void * (__cdecl *GfxDriverGetBlendModeFn)(int mode, struct GfxEngineContext *context);
typedef void (__cdecl *GfxDriverLightPolygonSetFn)(struct PolygonSkeletonFrameRef *polygonSet, struct GfxLightOwner *lightOwner);
typedef void (__cdecl *GfxDriverSetRenderStateFn)(int state, int value, struct GfxEngineContext *context);
typedef void (__cdecl *GfxDriverApplyStateFn)(int state, struct GfxEngineContext *context);
typedef void (__cdecl *GfxDriverDrawPolygonSetFn)(struct PolygonSkeletonFrameRef *polygonSet, struct GfxEngineContext *context);
typedef void (__cdecl *GfxDriverDrawBatchNodeFn)(struct GfxDrawListNode *node, struct GfxEngineContext *context);
typedef void (__cdecl *GfxDriverDrawRenderObjectFn)(struct GfxRenderObject *renderObject, struct GfxEngineContext *context);
typedef int (__cdecl *GfxDriverBeginSceneFn)(int pass, struct GfxEngineContext *context);
typedef void (__cdecl *GfxDriverDraw2DRecordFn)(int key, int record);
typedef void (__cdecl *GfxBatchEmitterFn)(struct GfxPolygonSet *polygonSet, struct GfxBatchNode *node, struct GfxEngineContext *context);

/* Driver entry-point table and background colour state bound per backend (DDraw blit, D3D, GL; 0xF0 bytes). */
typedef struct GfxDriverOps {
    GfxDriverInitFn init;
    GfxDriverCleanupFn cleanup;
    GfxDriverSurfaceFn lockSurface;
    GfxDriverSurfaceFn unlockSurface;
    GfxDriverContextFn present;
    GfxDriverClearViewportFn clearViewport;
    GfxDriverContextFn clearAll;
    GfxDriverSetViewportFn setViewport;
    GfxDriverSetBgColorFn setBackgroundColor;
    int backgroundPixel; /* +0x24: software driver's mapped background pixel */
    int backgroundColor[4]; /* +0x28: the 16-byte colour GfxSetBackgroundColorCall stores */
    SwArgbColor backgroundArgb; /* +0x38: software driver's packed ARGB background colour */
    unsigned char pad_3C[0x4];
    GfxDriverAllocBatchFn allocBatchArray;
    GfxDriverCompileGroupFn compileGroup;
    GfxDriverSetMatrixFn setMatrix;
    GfxDriverUnloadTextureFn unloadTexture;
    GfxDriverLoadTextureFn loadTexture;
    GfxDriverPaletteChangedFn paletteChanged;
    GfxDriverWritePaletteFn writePaletteEntries;
    GfxDriverGetBlendModeFn getBlendMode;
    GfxDriverLightPolygonSetFn lightPolygonSet;
    GfxDriverSetRenderStateFn setRenderState;
    GfxDriverApplyStateFn applyRenderState;
    GfxDriverApplyStateFn applyStateUntextured;
    GfxDriverApplyStateFn applyStateTextured;
    GfxDriverApplyStateFn applyPresetState;
    GfxDriverDrawPolygonSetFn drawPolygonSet;
    GfxDriverDrawBatchNodeFn drawBatchNode;
    GfxDriverDrawRenderObjectFn drawRenderObject;
    GfxDriverApplyStateFn selectScenePass;
    GfxDriverBeginSceneFn beginScene;
    GfxDriverContextFn endScene;
    GfxDriverDraw2DRecordFn draw2DRecord;
    GfxBatchEmitterFn begin2DUntextured;
    GfxBatchEmitterFn emitBegin2DUntextured;
    GfxBatchEmitterFn begin2DTexturedNoTex;
    GfxBatchEmitterFn begin2DTexturedOpaque;
    GfxBatchEmitterFn begin2DTexturedBlended;
    GfxBatchEmitterFn draw2DUntextured;
    GfxBatchEmitterFn emitDraw2DUntextured;
    GfxBatchEmitterFn emitDraw2DTextured;
    GfxBatchEmitterFn draw2DTextured;
    GfxBatchEmitterFn begin3DUntextured;
    GfxBatchEmitterFn emitBegin3DUntextured;
    GfxBatchEmitterFn begin3DTexturedNoTex;
    GfxBatchEmitterFn begin3DTexturedOpaque;
    GfxBatchEmitterFn begin3DTexturedBlended;
    GfxBatchEmitterFn draw3DUntextured;
    GfxBatchEmitterFn emitDraw3DUntextured;
    GfxBatchEmitterFn emitDraw3DTextured;
    GfxBatchEmitterFn draw3DTextured;
    GfxBatchEmitterFn beginLine;
    GfxBatchEmitterFn beginLineGouraud;
    GfxBatchEmitterFn drawLineFlat;
    GfxBatchEmitterFn drawLineGouraud;
    GfxDriverContextFn renderDeferredLists;
} GfxDriverOps;
typedef char CheckGfxDriverOps[(sizeof(GfxDriverOps) == 0xF0) ? 1 : -1];

/* Heap node for one polygon set submitted to the D3D or GL draw path. */
typedef struct GfxPolygonSetNode {
    int nextNode;
    int polyIndex;
    int drawGroup;
    int primGroupPtr;
    int inUse;
    float matrix[16]; /* +0x14: the node's 4x4 transform */
    int submitted;
    int payloadPtr;
    GfxPolygonGroupDrawSpec stateBlock; /* +0x5c */
} GfxPolygonSetNode;

/* D3D primitive caps block read back from the device and logged as flag names. */
typedef struct GfxD3dPrimCaps {
    int flags;
    int miscCaps;
    int rasterCaps;
    int zCompareCaps;
    int srcBlendCaps;
    int destBlendCaps;
    int alphaCompareCaps;
    int shadeCaps;
    int textureCaps;
    int textureFilterCaps;
    int textureBlendCaps;
    int textureAddressCaps;
} GfxD3dPrimCaps;

/* A .TEX texture (0xEC bytes; Gfx_LoadTexFile reads the header from the file, Gfx_AllocTexRecord
 * allocates one, the TIM converter Pfile_BuildTextureFromTimRect fills one). +0x14..+0x28 are the ranges Gfx_SelectTextureFormat's callbacks
 * accept a device format in (its green bits, alpha bits and bits per pixel); the pixels' own
 * format is at +0x3C, which the converters (Gfx_ConvertTexturePixelsToSurface,
 * Gl_ConvertTexturePixels, GfxUploadTexturePixels) unpack them through. */
typedef struct GfxTexHeader {
    int version; /* +0x00: 1 (Gfx_LoadTexFile accepts no other) */
    int unusedFormatFlag;
    int colorKey; /* +0x08: index 0 (or black) is transparent */
    int hasSemiTransparency;
    int timTexPage; /* +0x10: the TIM converter stores the TIM's texture page (PolygonTim.texPage) */
    int minColorBits; /* +0x14: a device format qualifies when its green bits lie in [min, max] */
    int maxColorBits; /* +0x18 */
    int minAlphaBits; /* +0x1C: ... its alpha bits in [min, max] */
    int maxAlphaBits; /* +0x20 */
    int minBitsPerPixel; /* +0x24: ... and its bits per pixel in [min, max] */
    int maxBitsPerPixel; /* +0x28 */
    int paletteFlags; /* +0x2C: Gfx_CreatePaletteRecord's flags */
    int paletteCount; /* +0x30: one GL texture name per palette (Gl_AllocTextureNameTable) */
    int entriesPerPalette; /* +0x34 */
    int bitsPerPixel; /* +0x38: the source pixels' (4, 8, 16 or 24) */
    GfxPixelFormat format; /* +0x3C: the pixels' format */
    int paletteMode; /* +0xBC: 1 when colorLookup holds a colour-key flag per palette */
    union {
        int colorLookup; /* +0xC0: paletteCount bytes */
        unsigned char *colorLookupBytes;
    };
    union {
        int alphaValue; /* +0xC4: the alpha an 0xFE alpha becomes (0xFF from Gfx_AllocTexRecord) */
        unsigned char alphaByte;
    };
    int blendMode; /* +0xC8: 4 from Gfx_AllocTexRecord */
    unsigned char modeFlag; /* +0xCC: 0 lifts opaque black to (1, 1, 1) so it is not the colour key */
    unsigned char flags; /* +0xCD */
    unsigned char pad_CE[0x2];
    int row; /* +0xD0: the palette in use */
    int pixelBuffer; /* +0xD4: the source pixels */
    int storage; /* +0xD8: the converted pixels (GL and the software rasterizer) */
    int clutX; /* +0xDC: the TIM's CLUT and image positions */
    int clutY; /* +0xE0 */
    int imageX; /* +0xE4 */
    int imageY; /* +0xE8 */
} GfxTexHeader;
typedef char CheckGfxTexHeader[(sizeof(GfxTexHeader) == 0xEC) ? 1 : -1];

/* Draw list node reached from a list entry at +8; checked for kind 0x11 before drawing. */
typedef struct GfxDrawListNode {
    int nextNode;
    int kind;
    int group;
    union {
        struct {
            int vertex0X;
            int drawEnabled;
            int vertex0Z;
            int vertex1X;
            int vertex1Y;
            int vertex1Z;
            int vertex2X;
            int vertex2Y;
            int vertex2Z;
            int vertex3X;
            int vertex3Y;
            int vertex3Z;
            int vertex0U;
            int vertex0V;
            int vertex1U;
            int vertex1V;
            int vertex2U;
            int vertex2V;
            int vertex3UOrCallback;
            int callbackArg;
            int vertex0Color;
            int vertex1Color;
            int vertex2Color;
            int vertex3Color;
            int vertex0Rhw;
            int vertex1Rhw;
            int vertex2Rhw;
            int vertex3Rhw;
        };
        struct { /* the same words as per-vertex arrays (the layout of GfxBatchPrimitive) */
            struct {
                int x;
                int y;
                int z;
            } positions[4]; /* +0x0C */
            struct {
                int u;
                int v;
            } texCoords[4]; /* +0x3C */
            int colors[4]; /* +0x5C */
            int rhw[4]; /* +0x6C */
        };
    };
    int vertexCount;
    int page;
} GfxDrawListNode;

/* Polygon batch record passed through the D3D5, GL and blit draw submit paths. */
typedef struct GfxPolygonBatch {
    unsigned char pad_00[0x8];
    int primType;
    int vertexType;
    int vertices;
    int vertexCount;
    int indices;
    int indexCount;
    int flags;
} GfxPolygonBatch;

/* Single-word struct used where VC5 /Od emits a 4-byte aggregate assignment instead of a scalar load/store. */
typedef struct GfxCopyWord {
    int value;
} GfxCopyWord;

/* Render object and per-frame primitive batch record (0xE0 bytes, allocated by Gfx_CreateRenderObject):
 * holds the polygon set, primitive/line strides, per-frame batch cursors, driver emitters and world matrix. */
typedef struct GfxRenderObject {
    int kind; /* +0x00: GfxDrawPrimitiveKind */
    int enabled;
    int immediateDraw;
    union {
        int hundredIndex;
        struct GfxGroupRenderState *groupState;
    };
    int material;
    union {
        int polygonSet;
        struct GfxPolygonSet *polygonSetPtr;
    };
    int releaseTexture;
    int spriteBatch;
    int vertexBuffer;
    float invWidth; /* +0x24: 1 / texture width (half-texel UV shift) */
    float invHeight; /* +0x28: 1 / texture height */
    union {
        int init;
        struct GfxPrimitiveMeshBank *meshBank; /* +0x2C: owning primitive mesh bank and block heaps */
    };
    union {
        int drawPool;
        struct GfxDrawPool *drawPoolPtr; /* +0x30: owning draw pool whose generation resets batches */
    };
    int emitterFlags; /* +0x34: GfxSelectPrimitiveEmitters' flags (the load options' emitterFlags) */
    int verticesPerPrim; /* +0x38: 3 for triangle kinds, 4 for quads (0x20-byte vertices) */
    int indicesPerPrim; /* +0x3C: 3 for triangles, 6 for quads */
    int cursorStep; /* +0x40: vertex byte stride per primitive */
    int indexCursorStep; /* +0x44: index byte stride per primitive */
    int textureMode; /* +0x48: 1 textured opaque, 2 textured blended (its primitives carry a page byte) */
    int commandCount; /* +0x4C: primitives reserved in the active generation */
    int allocCount; /* +0x50: remaining primitives in the current batch node */
    int batchGeneration; /* +0x54: draw-pool generation when the chain was opened */
    int indexBuffer; /* +0x58: shared prebuilt triangle/quad index run from meshBank */
    int lineVertexCount; /* +0x5C: 2 for line kinds */
    int lineIndexCount; /* +0x60: 2 for line kinds */
    int lineVertexBytes; /* +0x64: 64 for line kinds */
    int lineIndexBytes; /* +0x68: 4 for line kinds */
    int lineIndexBuffer; /* +0x6C: shared prebuilt line index pairs from meshBank */
    union {
        int vertexCursor; /* +0x70: the vertices of the primitives AcquireBatchNode reserved last */
        struct GfxTlVertex *vertices;
        float *vertexFloats;
        GfxCopyWord vertexCursorWord;
    };
    union {
        int indexCursor; /* +0x74: reserved index write cursor */
        GfxCopyWord indexCursorWord;
    };
    union {
        int colorIndex;
        unsigned char *colorBytePtr; /* +0x78: the byte the menu primitives store their palette selector through */
        char *colorSignedBytePtr;
        unsigned char *palette;
        int commandIndex;
    };
    union {
        int paletteIndex;
        int texturePage; /* +0x7C: active palette/page (-1 when unset) */
        unsigned int texturePageUnsigned;
        unsigned long lastPalette;
    };
    union {
        int cursorA; /* +0x80: next vertex cursor in the active node */
        GfxCopyWord cursorAWord;
    };
    union {
        int cursorB; /* +0x84: next index cursor in the active node */
        GfxCopyWord cursorBWord;
    };
    int cursor; /* +0x88: next page-byte cursor in the active node */
    union {
        int chainHead; /* +0x8C: the newest batch node (AcquireBatchNode); nodes link through their first word */
        struct GfxBatchNode *chainHeadNode;
    };
    GfxBatchEmitterFn beginEmitter; /* +0x90: called once per frame's chain (the driver's Gfx_EmitBegin*) */
    GfxBatchEmitterFn drawEmitter; /* +0x94: called for every node of the chain (Gfx_EmitDraw*) */
    int matrixByReference; /* +0x98: nonzero when worldMatrix points to caller's matrix */
    union {
        int matrixSource;
        void *worldMatrix; /* +0x9C */
    };
    GfxMatrix4x4 inlineMatrix; /* +0xA0..+0xDF */
} GfxRenderObject;
typedef char CheckGfxRenderObject[(sizeof(GfxRenderObject) == 0xE0) ? 1 : -1];

/* Draw-group record for the Gl and Gfx raster draw routines; batch leads to a polygon batch. */
typedef struct GlDrawGroup {
    unsigned char pad_00[0x8];
    int flags;
    int changeMask;
    unsigned char pad_10[0x4];
    int batch;
    unsigned char pad_18[0xc];
    int groupKind;
    unsigned char pad_28[0x1c];
    int blendMode;
    int polygonSet;
    int pad_4C;
    unsigned char pad_50[0x14]; /* to 0x64 bytes, the step of every pointer through it */
} GlDrawGroup;

/* Line scratch record at u+0xEC, filled with offsets derived from the vertex record u. */
typedef struct GlLineBuffer {
    int count;
    int deltaA;
    int deltaB;
    int fixedDelta;
    int stepA;
    int stepB;
} GlLineBuffer;

/* DirectDraw driver caps (DDCAPS, 0x16C = 364 bytes) queried by IDirectDraw::GetCaps. */
typedef struct GfxDirectDrawDriverCaps {
    int size; /* +0x00: 0x16C */
    int caps; /* +0x04 */
    int caps2; /* +0x08 */
    unsigned char pad_0C[0xc];
    int palCaps; /* +0x18 */
    unsigned char pad_1C[0x20];
    int videoMemTotal; /* +0x3C */
    int videoMemFree; /* +0x40 */
    unsigned char pad_44[0x40];
    int ddsCaps; /* +0x84: surface capability flags (e.g. DDSCAPS_VIDEOMEMORY) */
    unsigned char pad_88[0xe4];
} GfxDirectDrawDriverCaps;
typedef char CheckGfxDirectDrawDriverCaps[(sizeof(GfxDirectDrawDriverCaps) == 0x16C) ? 1 : -1];

/* Only the fields the code reads; the size is not known. */
typedef struct DxMeshGroupRecord {
    float pad_00;
    float pad_04;
    int pad_08;
    float pad_0C;
} DxMeshGroupRecord;

/* Only the fields the code reads; the size is not known. */
typedef struct GfxRasterVertexSeed {
    unsigned char pad_00[0xc];
    int y;
    int sortKey;
    unsigned char pad_14[0x4];
    int stepU;
    int x;
    int startU;
    int startV;
    int stepV;
    int vertexAttr;
    int endU;
    int endV;
    unsigned char pad_38[0x34];
    int startUCopyLow; /* +0x6C: copy of startUFixed */
    int startUCopyHigh;
    unsigned char pad_74[0x8];
    int startVCopyLow; /* +0x7C: copy of startVFixed */
    int startVCopyHigh;
    unsigned char pad_84[0x68];
    int startUFixedLow; /* +0xEC: crt_FloatToInt64 values */
    int startUFixedHigh;
    int endUFixedLow;
    int endUFixedHigh;
    int startVFixedLow;
    int startVFixedHigh;
    int endVFixedLow;
    int endVFixedHigh;
} GfxRasterVertexSeed;

/* Function-pointer set picked for the generated raster vertex path. */
typedef struct GfxRasterVertexCallbacks {
    int setupIndex;
    int generatedSetup;
    int generatedSpan;
    int triangleSetup;
    int edgePairSetup;
    int edgePairNextSetup;
    int textureSpan;
    int resolveVertex;
    int prepareVertex;
    int snapVertex;
} GfxRasterVertexCallbacks;

/* Polygon set record whose vertex count drives the per-vertex colour and lighting loops. */
typedef struct GfxPolygonSet {
    int drawEnabled; /* +0x00: the drivers draw or queue the set only when it is set (a scene pass
                      * sets the opaque or the translucent set's; the load options' +0x04) */
    int drawUnsorted; /* +0x04: GL draws a group that is not depth-sorted only when it is set
                       * (Gfx_CreateRenderObject's first argument) */
    unsigned char pad_08[0x8];
    int groupCount;
    int drawListActive;
    unsigned char pad_18[0x4];
    int flags;
    unsigned char pad_20[0x8];
    int drawPool;
    int hasGroupState;
    union {
        int source;
        struct PolygonFileArrays *modelSource;
        struct GfxPolygonData *polygonData;
    };
    int groupState;
    int useGroupStates; /* +0x38: when set, group i draws with groupStates[i] instead of groupState */
    int groupStates; /* +0x3C: one state per group (GfxLoadPolygonSet allocates groupCount words) */
    union {
        int matrices;
        struct GfxPolygonSetRecord *matrixRecord;
    };
    int lightGroup;
    struct ComUnknown **comObjectTable;
    struct ComDirect3DExecuteBuffer **execBuffers;
    struct GfxPolygonBatch **batchList;
    int vertexPointer;
    int elements;
    struct GfxPolygonSetEntry *entries;
    unsigned char pad_60[0x34];
    int drawSpecApplied;
    unsigned char pad_98[0x4];
    int drawSpec;
    unsigned char pad_A0[0x8];
    int renderFlags;
    int execBufferAlloc;
} GfxPolygonSet;

/* Result of clipping a raster line to the viewport: the clipped endpoints. */
typedef struct GfxClippedLine {
    int visible;
    unsigned char pad_04[0x4];
    int x0;
    int y0;
    int x1;
    int y1;
    int wDelta;
    int dyValue;
    int heightAbs;
    int rowCount;
} GfxClippedLine;

/* DirectX palette record (0x40 bytes): flags and converted palette tables. */
typedef struct GfxPaletteRecord {
    int flags;
    int entryCount;
    int bitDepth;
    int groupCount;
    int entriesPerGroup;
    int ownsEntries;
    int srcEntries;
    int paletteMode;
    int colorLookup;
    int floatEntries;
    int colorEntries;
    int d3dEntries;
    int releaseCallback;
    int rampEntries;
    int block;
    int arraysPtr;
} GfxPaletteRecord;

/* Transformed raster vertex (0x60 stride) for the triangle clipper: screen x, y and clip mask. */
typedef struct GfxRasterVertex {
    float x;
    float y;
    unsigned char pad_08[0x18];
    int flags;
    int clipMask;
    unsigned char pad_28[0x10];
    int srcPtr;
    int resolved;
    int srcAttr0;
    int srcAttr1;
    int paletteEntry2;
    int paletteEntry1;
    int paletteEntry0;
    int srcWord;
} GfxRasterVertex;

/* PSX primitive system record: one 4-byte value per primitive type, passed to each emitter. */
typedef struct GfxPsxPrimitiveAttrs {
    struct GfxRenderObject *flatLineBatch;
    unsigned char pad_04[0x8];
    struct GfxRenderObject *gouraudLineBatch;
    unsigned char pad_10[0x8];
    struct GfxRenderObject *flatTriangleBatch;
    struct GfxRenderObject *flatQuadBatch;
    struct GfxRenderObject *gouraudTriangleBatch;
    struct GfxRenderObject *gouraudQuadBatch;
    int texFlatTriangleBatch;
    int texQuadBatch;
    int texGouraudTriangleBatch;
    int texGouraudQuadBatch;
} GfxPsxPrimitiveAttrs;

/* PSX textured Gouraud quad source: four vertex colors and texture coordinates. */
typedef struct GfxPsxTexturedQuadSource {
    unsigned char pad_00[0x4];
    char color0R;
    char color0G;
    char color0B;
    unsigned char pad_07[0x1];
    short x0;
    short y0;
    unsigned char u0;
    unsigned char v0;
    unsigned short clut;
    char color1R;
    char color1G;
    char color1B;
    unsigned char pad_13[0x1];
    short x1;
    short y1;
    unsigned char u1;
    unsigned char v1;
    unsigned short texturePage;
    char color2R;
    char color2G;
    char color2B;
    unsigned char pad_1F[0x1];
    short x2;
    short y2;
    unsigned char u2;
    unsigned char v2;
    unsigned char pad_26[0x2];
    char color3R;
    char color3G;
    char color3B;
    unsigned char pad_2B[0x1];
    short x3;
    short y3;
    unsigned char u3;
    unsigned char v3;
} GfxPsxTexturedQuadSource;

/* Only the fields the code reads; the size is not known. */
typedef struct GfxModelTableRecord {
    unsigned short flags;
    unsigned short scale;
    int name;
    int sptRecordId;
    int optionValue;
    unsigned char pad_10[0x4]; /* to 0x14 bytes, the step of every pointer through it */
} GfxModelTableRecord;

/* Per-draw GL render state: texture wrap mode and source pointers. */
typedef struct GlRenderState {
    unsigned char pad_00[0x3];
    char perspectiveCorrection;
    unsigned char pad_04[0x1];
    char wrapUEnabled;
    char wrapU;
    unsigned char pad_07[0x1];
    char colorKeyEnabled;
    char ditherEnabled;
    unsigned char pad_0A[0x3];
    char cullFrontFaces;
    char cullingDisabled;
    char depthTestEnabled;
    char depthWriteEnabled;
    unsigned char pad_11[0xf];
    int blendMode;
    int isGouraud;
    int batch;
} GlRenderState;

/* Scratch record for building triangle edge chains in the software rasterizer. */
typedef struct GfxRasterEdgeChain {
    int vertexA;
    int vertexBWord;
    int vertexC;
    int active;
    int edgeState;
    int startVertex;
    int endVertex;
    unsigned char pad_1C[0x1c];
    int secondStartVertex;
    int secondEndVertex;
    unsigned char pad_40[0x1c];
    int midVertexIndex;
    int vertexB;
    unsigned char pad_64[0x1c];
    int secondMidVertexIndex;
    int secondVertexB;
    unsigned char pad_88[0x1c];
    int spanWidth;
} GfxRasterEdgeChain;

/* Direct3D device description block with D3DDEVCAPS-style caps, read by Gfx_LogD3DDeviceDesc. */
typedef struct GfxD3DDeviceDesc {
    int size;
    int flags;
    int colorModel;
    int devCaps;
    int transformCapsSize;
    int transformCaps;
    int clippingSupported;
    int lightingCapsSize;
    int lightCaps;
    int shadeColorModel;
    int maxLights;
    int lineCapsSize;
    unsigned char pad_30[0x34];
    int triCapsSize;
    unsigned char pad_68[0x34];
    int rBitDepth;
    int zBitDepth;
    int batchLimit;
    int maxVertices;
    unsigned char pad_AC[0x20]; /* min/max texture and stipple dimensions (0xCC bytes total) */
} GfxD3DDeviceDesc;
typedef char CheckGfxD3DDeviceDesc[(sizeof(GfxD3DDeviceDesc) == 0xCC) ? 1 : -1];

/* Raster vertex used while clipping: screen x and y, their table indices and flags. */
typedef struct GfxClipVertex {
    float x;
    float y;
    float z;
    float recipW;
    int homogeneousW;
    float srcPos[3]; /* +0x14: the position the vertex is projected from */
    int clipXFlag;
    int flags;
    int fixedXLow; /* +0x28: crt_FloatToInt64 of x; xIndex is its integer word */
    int xIndex;
    int fixedYLow; /* +0x30 */
    int yIndex;
    unsigned char pad_38[0x4];
    int clipYFlag;
    float texU;
    float texV;
} GfxClipVertex;

/* Render-state record that Blit_ApplyRenderSpec fills from the render spec. */
typedef struct SwBlitRenderState {
    unsigned char pad_00[0x3];
    char perspectiveCorrect;
    unsigned char pad_04[0x1];
    char wrapUEnabled;
    char isEnabled;
    unsigned char pad_07[0x1];
    char colorKeyEnabled;
    char ditherEnabled;
    unsigned char pad_0A[0x3];
    char cullFrontFaces;
    char cullingDisabled;
    char depthTestEnabled;
    char depthWriteEnabled;
    unsigned char pad_11[0xf];
    int blendMode;
    int isActive;
    int batch;
} SwBlitRenderState;

/* PSX textured-quad primitive (POLY_FT4; its first 0x1C bytes are a POLY_FT3): ordering-table tag,
 * flat color and GPU code, four corner positions, UVs, CLUT and texture page. The words overlay the
 * bytes for code that copies them whole. */
typedef struct GfxPsxTexturedQuad {
    int tag;
    union {
        int color;
        CVECTOR colorVec;
        struct {
            char colorR;
            char colorG;
            char colorB;
            char code; /* the GPU command byte */
        };
    };
    short x0;
    short y0;
    union {
        int uv0Clut;
        struct {
            unsigned char u0;
            unsigned char v0;
            unsigned short clut;
        };
    };
    short x1;
    short y1;
    union {
        short uv1;
        struct {
            unsigned char u1;
            unsigned char v1;
        };
    };
    unsigned short texturePage;
    short x2;
    short y2;
    union {
        short uv2;
        struct {
            unsigned char u2;
            unsigned char v2;
        };
    };
    unsigned char pad_1E[0x2];
    short x3;
    short y3;
    union {
        short uv3;
        struct {
            unsigned char u3;
            unsigned char v3;
        };
    };
} GfxPsxTexturedQuad;

/* PSX textured Gouraud triangle: three vertices, each with RGB, x, y and u, v, plus two CLUT words. */
typedef struct GfxPsxTexturedTriangle {
    unsigned char pad_00[0x4];
    char vertex0R;
    char vertex0G;
    char vertex0B;
    unsigned char pad_07[0x1];
    short vertex0X;
    short vertex0Y;
    unsigned char vertex0U;
    unsigned char vertex0V;
    unsigned short clut;
    char vertex1R;
    char vertex1G;
    char vertex1B;
    unsigned char pad_13[0x1];
    short vertex1X;
    short vertex1Y;
    unsigned char vertex1U;
    unsigned char vertex1V;
    unsigned short texturePage;
    char vertex2R;
    char vertex2G;
    char vertex2B;
    unsigned char pad_1F[0x1];
    short vertex2X;
    short vertex2Y;
    unsigned char vertex2U;
    unsigned char vertex2V;
} GfxPsxTexturedTriangle;

/* PSX Gouraud quad: four vertices, each with an RGB colour and screen x, y. */
typedef struct GfxPsxGouraudQuad {
    unsigned char pad_00[0x4];
    char vertex0R;
    char vertex0G;
    char vertex0B;
    unsigned char pad_07[0x1];
    short vertex0X;
    short vertex0Y;
    char vertex1R;
    char vertex1G;
    char vertex1B;
    unsigned char pad_0F[0x1];
    short vertex1X;
    short vertex1Y;
    char vertex2R;
    char vertex2G;
    char vertex2B;
    unsigned char pad_17[0x1];
    short vertex2X;
    short vertex2Y;
    char vertex3R;
    char vertex3G;
    char vertex3B;
    unsigned char pad_1F[0x1];
    short vertex3X;
    short vertex3Y;
} GfxPsxGouraudQuad;

/* First float matrix operand (16-byte rows, three floats per row) of Gfx_LightMultiplyTransposedWithAmbient. */
typedef struct GfxLightMatrixA {
    float m00;
    float m01;
    float m02;
    unsigned char pad_0C[0x4];
    float m10;
    float m11;
    float m12;
    unsigned char pad_1C[0x4];
    float m20;
    float m21;
    float m22;
} GfxLightMatrixA;

/* Model table record: entry count, entry array, lifetime, tick counter and owner reference. */
typedef struct GfxModelTable {
    int count;
    int fileContext;
    int records;
    int entries;
    int groupTag;
    int lifetime;
    int tickStart;
    int tick;
    int ownerRef;
} GfxModelTable;

/* Clip-edge vertex record of the rasteriser: float attributes interpolated across a clip plane. */
typedef struct GfxRasterEdgeVertex {
    float clipX; /* +0x0 */
    float clipY;
    unsigned char pad_08[0xc];
    float x;
    float y;
    float clipCoord;
    unsigned char pad_20[0x18];
    int extPtr;
    unsigned char pad_3C[0x4];
    float startU;
    float startV;
    float clipAttr1;
    float clipAttr2;
    float clipAttr3;
    float clipAttr4;
} GfxRasterEdgeVertex;

/* D3D5 render batch: vertex and index buffers with their stride, primitive type and counts. */
typedef struct GfxRenderBatch {
    unsigned char pad_00[0x4];
    int vertexStride;
    int primType;
    int vertexType;
    int vertexOut;
    int maxIndex;
    short *indices;
    int indexCount;
    int pad_20;
    int vertices;
} GfxRenderBatch;

/* Rasterizer vertex: float attributes interpolated along a line, plus an ARGB color. */
typedef struct GlRasterVertex {
    unsigned char pad_00[0x8];
    union { float depth; int depthBits; };
    unsigned char pad_0C[0x20];
    int posX;
    unsigned char pad_30[0x4];
    int posY;
    unsigned char pad_38[0x10];
    union { float texU; int texUBits; };
    union { float texV; int texVBits; };
    union { float w; int wBits; };
    int color;
} GlRasterVertex;

/* Batch node linked into a list, with a sub-buffer cursor and region bounds. */
typedef struct GfxBatchNode {
    struct GfxBatchNode *nextNode;
    unsigned char pad_04[0x4];
    int commandCount;
    int vertexCount;
    union {
        int vertexBase; /* +0x10: the first reserved vertex, a copy of buffer */
        GfxCopyWord vertexBaseWord;
    };
    union {
        int buffer;
        GfxCopyWord bufferWord;
    };
    int dataBytes;
    union {
        int indexBase; /* +0x1C: the first reserved index, a copy of regionStart */
        GfxCopyWord indexBaseWord;
    };
    union {
        int regionStart;
        GfxCopyWord regionStartWord;
    };
    int cursor;
    int regionEnd;
    unsigned char pad_2C[0x4];
    struct GfxRenderObject *owner;
} GfxBatchNode;

/* Per-raster-state record used when drawing textured rectangle quads. */
typedef struct GfxRasterRectState {
    unsigned char pad_00[0xc];
    int alignedVertices;
    unsigned char pad_10[0x28];
    float scaleDepth;
    float scaleU;
    float scaleV;
    int drawCallback;
    unsigned char pad_48[0x4];
    int outputOffset;
} GfxRasterRectState;

/* PSX textured flat primitive record with colour bytes, CLUT word and vertex data. */
typedef struct GfxPsxPrimitive {
    unsigned char pad_00[0x4];
    char colorR;
    char colorG;
    char colorB;
    unsigned char pad_07[0x1];
    short x0;
    short y0;
    unsigned char u0;
    unsigned char v0;
    unsigned short clut;
    short x1;
    short y1;
    unsigned char u1;
    unsigned char v1;
    unsigned short texturePage;
    short x2;
    short y2;
    unsigned char u2;
    unsigned char v2;
} GfxPsxPrimitive;

/* Per-line raster state reached through the draw context at +0x188. */
typedef struct GfxRasterLineContext {
    unsigned char pad_00[0x4];
    int deltaX;
    unsigned char pad_08[0x4];
    int startY;
    unsigned char pad_10[0x4];
    int color; /* +0x14: the first endpoint's colour mapped through the ramp tables */
    unsigned char pad_18[0x4];
    int startX;
    unsigned char pad_20[0x1c];
    int deltaY;
    unsigned char pad_40[0x18];
    int accumulator;
    unsigned char pad_5C[0xe8];
    int accumulatorStep;
} GfxRasterLineContext;

/* Clipped raster vertex written by the clip-plane edge intersection. */
typedef struct GfxRasterClipVertex {
    float clipX; /* +0x0 */
    float clipY;
    unsigned char pad_08[0xc];
    float x;
    float y;
    float clipCoord;
    int forcedInside;
    unsigned char pad_24[0x18];
    int resolved;
    float startU;
    float clipAttr0;
    float clipAttr1;
    float clipAttr2;
    float clipAttr3;
    float clipAttr4;
    unsigned char pad_58[0x8];
} GfxRasterClipVertex;

/* Heap block allocated for one scaled sprite quad. */
typedef struct GfxScaledSpriteQuad {
    float posX;
    float posY;
    int color;
    unsigned char pad_0C[0xc];
    float xSize;
    float ySize;
} GfxScaledSpriteQuad;

/* Heap block allocated for one sprite quad from the word emitter. */
typedef struct GfxSpriteQuadWord {
    float posX;
    float posY;
    int wordValue;
    unsigned char pad_0C[0xc];
    float sizeX;
    float sizeY;
} GfxSpriteQuadWord;

/* 16-bit index buffer allocated for sprite batches. */
typedef struct GfxSpriteIndexBuffer {
    short vertexIndexA;
    short vertexIndexB;
    short vertexIndexC;
    short vertexIndexD;
    short vertexIndexE;
    short vertexIndexF;
} GfxSpriteIndexBuffer;

/* Homogeneous x, y, z, w float vector that PerspectiveDivide divides by w. */
typedef struct GfxHomogeneousVec4 {
    float x;
    float y;
    float z;
    float w;
} GfxHomogeneousVec4;

/* Record at polygon set offset 0x40 holding a depth-sort group handle and an enable flag. */
typedef struct GfxPolygonSetRecord {
    unsigned char pad_00[0x14];
    union {
        int depthGroup;
        float *matrixFloats;
        float (*worldMatrix)[4];
    };
    int viewMatrixHandle;
    int projectionMatrixHandle;
    int enabled;
    int viewEnabled;
    int projectionEnabled;
} GfxPolygonSetRecord;

/* The 0x14-byte state of Gfx_SelectTextureFormat's search, which its enumeration callbacks read through
 * a pointer to a pointer: whether a format was found, how many candidates were seen, the last usable
 * candidate (result), the chosen candidate (selectedFormat) and the texture header it must suit. */
typedef struct GfxTextureFormatProbe {
    int formatFound;
    int count;
    int result;
    int selectedFormat;
    struct GfxTexHeader *formatDesc;
} GfxTextureFormatProbe;

/* Primitive mesh bank with four block heaps, created by Gfx_CreatePrimitiveMeshBank. */
typedef struct GfxPrimitiveMeshBank {
    struct MemBlockHeap *recordHeap; /* the 0xE0-byte render objects */
    struct MemBlockHeap *vertexHeap; /* the 0x84-byte batch primitives */
    struct MemBlockHeap *indexHeap; /* batch node block heap */
    union {
        struct MemBlockHeap *entryHeap;
        struct MemBlockArrayHeap *arrayHeap;
    };
    struct PolygonFileArrays *meshFiles[16]; /* +0x10: the loaded primitive mesh file of each kind (Gfx_CreatePrimitiveMeshBank) */
    int clearedCounterA; /* cleared with clearedCounterB when Gfx_ResetRenderObjectPool rewinds the heaps */
    int clearedCounterB;
    int triIndicesReversed;
    int quadIndicesReversed;
    int triIndices;
    int quadIndices;
    int lineIndices;
    struct GfxEngineContext *device;
} GfxPrimitiveMeshBank;

/* Sprite batch record: size, texture dimensions, source origin and vertex heap. */
typedef struct GfxSpriteBatch {
    int width;
    int height;
    float widthFloat;
    float heightFloat;
    int texWidth;
    int texHeight;
    float invTexWidth;
    float invTexHeight;
    int srcX;
    int srcY;
    float uOrigin;
    float vOrigin;
    int mode;
    int blockCount;
    struct MemBlockHeap *vertexHeap;
} GfxSpriteBatch;

/* Software raster state the triangle setup routines fill: mapped colour, 64-bit fixed-point steps and colour lookup tables. */
typedef struct SwRasterState {
    unsigned char pad_00[0x14];
    int packedColor;
    unsigned char pad_18[0x44];
    unsigned char depthStep[8];
    unsigned char pad_64[0xc];
    int invAlpha;
    unsigned char uStep[8];
    unsigned char pad_7C[0x8];
    unsigned char vStep[8];
    unsigned char pad_8C[0x8];
    unsigned char redStep[8];
    unsigned char pad_9C[0x8];
    unsigned char greenStep[8];
    unsigned char pad_AC[0x8];
    unsigned char blueStep[8];
    unsigned char pad_BC[0x68];
    int *lutE;
    int *lutD;
    int *lutC;
    unsigned char pad_130[0x4];
    int output;
    unsigned char pad_138[0x10];
    char *rampTable;
    union {
        int color;
        struct {
            unsigned char lutIndexC;
            unsigned char lutIndexD;
            unsigned char lutIndexE;
            unsigned char alpha;
        };
    };
    unsigned char pad_150[0x4];
    char *paletteBase;
} SwRasterState;

/* Float vertex record read by Gfx_EnqueuePsxPrimitiveForDepthSort; z is averaged into the sort key. */
typedef struct GfxDepthSortVertexA {
    float x;
    float y;
    float z;
    int recipW;
    float clipW;
} GfxDepthSortVertexA;

/* Transposed 4x4 matrix of 32-bit words, written from a row-major source. */
typedef struct GfxTransposedMatrix {
    int m00;
    int m01;
    int m02;
    int m03;
    int m10;
    int m11;
    int m12;
    int m13;
    int m20;
    int m21;
    int m22;
    int m23;
    int m30;
    int m31;
    int m32;
    int m33;
} GfxTransposedMatrix;

/* Vertex and poly record filled by Gfx_BuildPrimitiveMesh and Pfile_AddVertex. */
typedef struct GfxPrimitiveMeshVertex {
    int shade;
    int v;
    int polyCount;
    int firstPosition;
    int vertexCnt;
    unsigned char pad_14[0x18];
    int lightIndex;
    int textureFlag;
    int refIndex;
} GfxPrimitiveMeshVertex;

/* Screen-sprite command block allocated from the heap by Gfx_EmitScreenSprite. */
typedef struct GfxScreenSpriteCmd {
    float posX;
    float posY;
    unsigned char pad_08[0x10];
    float xExtent;
    float extY;
} GfxScreenSpriteCmd;

/* Screen-sprite command block with a word payload, allocated by Gfx_EmitScreenSpriteWithWord. */
typedef struct GfxScreenSpriteWordCmd {
    float posX;
    float yOffs;
    unsigned char pad_08[0x10];
    float xExtent;
    float bottomPos;
} GfxScreenSpriteWordCmd;

/* Sprite quad command block allocated from the heap by Gfx_EmitSpriteQuad. */
typedef struct GfxSpriteQuadCmd {
    float posX;
    float posY;
    unsigned char pad_08[0x10];
    float sizeX;
    float sizeY;
} GfxSpriteQuadCmd;

/* Edge slope output of Gfx_RasterComputeEdgeSlope; no field use is visible yet. */
typedef struct GfxRasterEdgeSlope {
    unsigned char pad_00[0x8];
    int dx;
    int dy;
    int rowCount;
    float slope;
    int slopeFixedLow;
    int slopeFixedHigh;
} GfxRasterEdgeSlope;

/* Float rectangle with texture scale terms, drawn by Gfx_RasterTryDrawTexturedRectQuads. */
typedef struct GfxRasterTexRect {
    float x;
    float y;
    float z;
    unsigned char pad_0C[0xc];
    float texU;
    float texV;
} GfxRasterTexRect;

/* Model table bank of arrays and mesh and draw-pool handles, created by Gfx_CreateModelTableBank. */
typedef struct GfxModelTableBank {
    int tableCount;
    int count;
    int table;
    int expiredCount;
    int slots;
    int primitiveMeshBank;
    int drawPool;
    int fileOpenMode;
} GfxModelTableBank;

/* Light owner record from Gfx_LightCreateOwner; holds three light pointers, a specular color and a device transform. */
typedef struct GfxLightOwner {
    int flags;
    int maxLights;
    struct GfxLight *light0;
    struct GfxLight *light1;
    struct GfxLight *light2;
    float colorFloats[4]; /* the color unpacked by Gfx_UnpackBgraToFloat4 */
    float ambientFloats[4]; /* colorFloats normalized by Gfx_LightNormalizeColorCopy */
    int color;
    unsigned char directionMatrix[0x40];
    unsigned char colorMatrix[0x40];
    unsigned char productMatrix[0x40]; /* colorMatrix times directionMatrix */
    unsigned char lightMatrix[0x40]; /* the matrix Gfx_LightVertexDiffuse transforms normals with */
    int lightMatrixEnabled;
    unsigned char identityMatrix[0x40]; /* set to identity by Gfx_LightCreateOwner */
    union {
        int specularMode;
        int rampMode;
    };
    union {
        int specularColor;
        unsigned int rampColor;
    };
    union {
        int cachedDepth;
        int rampDepth;
    };
    unsigned char pad_188[0x4];
    float rampStart;
    float rampEnd;
    float rampSpan;
    float invRampSpan;
    union {
        int deviceTransform;
        int depthMatrix;
    };
    float rampScale;
    union {
        unsigned char rampLut;
        unsigned char rampLutBytes[0x404];
    };
    int fadeColor;
} GfxLightOwner;

/* Win32-style pixel format descriptor read by Gl_PixelFormatFromPfd: type, color bits and shifts. */
typedef struct GlPixelFormatDescriptor {
    unsigned char pad_00[0x8];
    unsigned char pixelType;
    unsigned char colorBits;
    unsigned char redBits;
    unsigned char redShift;
    unsigned char greenBits;
    unsigned char greenShift;
    unsigned char blueBits;
    unsigned char blueShift;
    unsigned char alphaBits;
    unsigned char alphaShift;
} GlPixelFormatDescriptor;

/* Decoded PSX CLUT position and flags written by Gfx_DecodePsxClutPosition. */
typedef struct GfxPsxClutPosition {
    int clutX;
    int clutY;
    int flags;
    int colorMode;
    int yOfs;
} GfxPsxClutPosition;

/* Three Gouraud vertices (RGB and screen x/y) passed to the PSX gouraud polyline emitter. */
typedef struct GfxPsxGouraudPolylineVerts {
    unsigned char pad_00[0x4];
    char color0R;
    char color0G;
    char color0B;
    unsigned char pad_07[0x1];
    short vertex0X;
    short vertex0Y;
    char color1R;
    char color1G;
    char color1B;
    unsigned char pad_0F[0x1];
    short vertex1X;
    short vertex1Y;
    char color2R;
    char color2G;
    char color2B;
    unsigned char pad_17[0x1];
    short vertex2X;
    short vertex2Y;
} GfxPsxGouraudPolylineVerts;

/* 0x38-byte draw group record used by the software rasterizer's group draw paths. */
typedef struct GfxRasterDrawGroup {
    unsigned char pad_00[0x8];
    int primitiveCount;
    int vertexStart;
    int elementCount;
    unsigned char pad_14[0x24]; /* to 0x38 bytes, the step of every pointer through it */
} GfxRasterDrawGroup;

/* Vertex record with float attributes that Gfx_RasterDrawLineBetweenVertices interpolates along a line. */
typedef struct GfxLineVertex {
    unsigned char pad_00[0x8];
    union { float depth; int depthBits; };
    unsigned char pad_0C[0x20];
    int screenX;
    unsigned char pad_30[0x4];
    int screenY;
    unsigned char pad_38[0x10];
    union { float u; int uBits; };
    union { float v; int vBits; };
    union { float w; int wBits; };
    int color;
} GfxLineVertex;

/* Projection state for a textured flat quad: scale factors and a vertex buffer. */
typedef struct GfxTexturedFlatQuadProjection {
    unsigned char pad_00[0x24];
    float texScaleU;
    float texScaleV;
    unsigned char pad_2C[0x44];
    int vertexBuffer;
    unsigned char pad_74[0x4];
    char *attrDest;
    int attr;
} GfxTexturedFlatQuadProjection;

/* Refcounted texture cache record: cache refs, texture handle, record refs and a record handle. */
typedef struct GfxTextureRecord {
    int kind;          /* +0x00: FileCreateRecord's kind (0) */
    int fileName;      /* +0x04: the name FileCreateRecord copied */
    int directory;     /* +0x08: its directory (the working directory when none was given) */
    unsigned char pad_0C[0x4];
    int cacheRefs;     /* +0x10: 1 when Gfx_LoadTexFileCached registers it, +1 per cache hit; Gfx_ReleaseCachedTexture
                        * destroys `texture` at 0 */
    int texture;       /* +0x14: Gfx_LoadTexFile's GfxTexHeader; IndexedEntryKeyMatches key 0 */
    int refs;          /* +0x18: Gfx_ReleaseTextureRecordRef frees `recordHandle` at 0 */
    int recordHandle;  /* +0x1C: IndexedEntryKeyMatches key 1 */
} GfxTextureRecord;

/* Entry of a deferred draw pass: a kind switch, an expected word compared through a pointer. */
typedef struct GfxDeferredPassEntry {
    int active;
    unsigned char pad_04[0x4];
    int kind;
    int expected;
    int nodePtr;
    int statePtr;
} GfxDeferredPassEntry;

/* Deferred render pass state, reached through a pointer at +4 of a render record. */
typedef struct GfxDeferredPass {
    int active;
    int pending;
    unsigned char pad_08[0x8];
    int count;
    GfxDeferredPassEntry entry; /* +0x14: the draw-state gate (active, expected state word, node list) */
    unsigned char pad_2C[0x4];
    int recordSource;
    int drawGroup;
    unsigned char pad_38[0x8];
    int mtxTable;
    int flagSrc;
    unsigned char pad_48[0x4];
    int entries;
} GfxDeferredPass;

/* GL colour index record with per-component colour arrays and a stride-16 colour record table. */
typedef struct GlColorArrays {
    int cornerColor2;
    int cornerColor1;
    int cornerColor0;
    int cornerColor3;
    int colorRecords;
} GlColorArrays;

/* Clip classification result written by Gfx_RasterClassifyRectAgainstClipBounds. */
typedef struct GfxClipResult {
    int status;
    int leftOver;
    int overRight;
    int topOver;
    int bottomOver;
} GfxClipResult;

/* Render state object used when blitting context rects and presenting; holds two surface bases and a stride. */
typedef struct GfxRenderState {
    unsigned char pad_00[0x8];
    int dirty;
    int rowOffset;
    unsigned char pad_10[0x14];
    int surfaceA;
    int surfaceB;
} GfxRenderState;

/* Raster setup block filled from a render state and a global block; buffer addresses at fixed offsets. */
typedef struct GfxRasterSetupBlock {
    int frameCounter;
    int scanSize;
    int pixels;
    int originY;
    unsigned char pad_10[0xc];
    int originX;
    unsigned char pad_20[0x4c];
    int uStartLow; /* +0x6C: 64-bit fixed-point texture U start; the high word is offsetTableX */
    int offsetTableX;
    int uStepLow; /* +0x74: 64-bit fixed-point U step (low, high) */
    int uStepHigh;
    int vStartLow; /* +0x7C: 64-bit fixed-point texture V start; the high word is offsetTableY */
    int offsetTableY;
    int vStepLow; /* +0x84: 64-bit fixed-point V step (low, high) */
    int vStepHigh;
    unsigned char pad_8C[0x80];
    int shiftX;
    int maskX;
    int maskY;
    int maskSource;
    int rowPtr0;
    int rowPtr1;
    int rampTableR;
    int rampTableG;
    int rampTableB;
    unsigned char pad_130[0x4];
    int dxTableStart;
    unsigned char pad_138[0xc];
    int depthOffset;
    int divideByThreeTable;
    unsigned char pad_14C[0x4];
    int pixelToRgbTable;
    int alphaTable;
} GfxRasterSetupBlock;

/* Table cursor: an entry array at +0x0C and a current and initial value reset together. */
typedef struct GfxTableCursor {
    unsigned char pad_00[0xc];
    int entries;
    unsigned char pad_10[0x8];
    int current;
    int initial;
} GfxTableCursor;

/* Raster line record whose clipped endpoints Gfx_RasterDrawClippedLineEndpoints reads. */
typedef struct GfxRasterLineEndpointA {
    unsigned char pad_00[0x8];
    int depth;
    unsigned char pad_0C[0x14];
    int needsClipping;
    int clipFlags;
    unsigned char pad_28[0x4];
    int x;
    unsigned char pad_30[0x4];
    int y;
    unsigned char pad_38[0x10];
    int attrU;
    int attrV;
    int attrWidth;
    int sample;
} GfxRasterLineEndpointA;

/* Texture page fill parameters: origin, step, size, pitch and pixel data. */
typedef struct GfxTexturePageFill {
    int mode;
    int originX;
    int originY;
    int width;
    int height;
    int stepX;
    int stepY;
    int isEnabled;
    int surfaceWidth;
    int surfaceHeight;
    int pitch;
    int bytesPerPixel;
    int surfacePixels;
    GfxPixelFormat format; /* +0x34: the source surface's format (mode 2) */
    int pixelData;
} GfxTexturePageFill;

/* One row of three floats of a 3x3 matrix, read row by row by ConcatRotationAndTranslationRow. */
typedef struct GfxMatrix3Row {
    float col0;
    float col1;
    float col2;
    unsigned char pad_0C[0x4];
} GfxMatrix3Row;

/* Render-state request: flag bits and a texture reference applied by D3D5ApplyRenderState. */
typedef struct GfxRenderStateRecord {
    unsigned char pad_00[0x8];
    int flags;
    int bits;
    unsigned char pad_10[0x4];
    int texturePtr;
    unsigned char pad_18[0xc];
    int stateId;
    int biasOfs;
    unsigned char pad_2C[0x4];
    int textureRef;
    int stateValue;
    int stateFallback;
    unsigned char pad_3C[0x4];
    int parameterVal;
} GfxRenderStateRecord;

/* 3x4 float transform matrix, the right operand of a column concatenation (row-major, 16-byte rows). */
typedef struct GfxMatrix3x {
    float m00;
    float m01;
    float m02;
    float m03;
    float m10;
    float m11;
    float m12;
    float m13;
    float m20;
    float m21;
    float m22;
    float m23;
} GfxMatrix3x;

/* Canonical 32-byte Direct3D transformed-and-lit vertex (D3DTLVERTEX).
 * Anonymous unions expose float and raw-integer views at identical offsets without casts. */
typedef struct GfxTlVertex {
    union { float x; int xBits; };
    union { float y; int yBits; };
    union { float z; int zBits; };
    union { float rhw; int rhwBits; };
    union { int color; unsigned int colorUnsigned; };
    union { int specular; unsigned int specularUnsigned; };
    union { float u; int uBits; };
    union { float v; int vBits; };
} GfxTlVertex;
/* D3D exec buffer descriptor: vertex format, texture-coordinate count and the source arrays. */
typedef struct GfxExecBuffer {
    unsigned char pad_00[0x8];
    int vertexFormat;
    int vertexCnt;
    int vectorCount;
    unsigned char pad_14[0x4];
    int texCoordCount;
    unsigned char pad_1C[0x24];
    int positions;
    int vectorArray;
    unsigned char pad_48[0x4];
    int texCoords;
    int colors;
    unsigned char pad_54[0xc];
} GfxExecBuffer;


/* Direct3D matrix slots: last matrix pointers and uploaded matrix handles used by D3DSetMatrix. */
typedef struct GfxMatrixState {
    unsigned char pad_00[0x14];
    int matrixPtr0;
    int matrixPtr1;
    int matrixPtr2;
    int matrixHandle0;
    int matrixHandle1;
    int matrixHandle2;
} GfxMatrixState;

/* Left operand 3x3 rotation matrix of MultiplyRotation3x3, stored in 0x10-byte rows. */
typedef struct GfxRotationMatrixA {
    unsigned char pad_00[0x10];
    float m00;
    float m01;
    float m02;
    unsigned char pad_1C[0x4];
    float m10;
    float m11;
    float m12;
} GfxRotationMatrixA;

/* Entry of the PSX primitive system: attribute pointer, render object set and display-list record. */
typedef struct GfxPsxPrimitiveSet {
    int attrs;
    int renderObjectSet;
    unsigned char pad_08[0x8];
    int displayListRecord;
} GfxPsxPrimitiveSet;

/* Entry of a model table built from GfxModelTableRecord records: kind, render object and loaded record. */
typedef struct GfxModelTableEntry {
    int kind;
    union {
        int renderObject;
        struct GfxRenderObject *renderObj;
    };
    int loadedRecord;
} GfxModelTableEntry;

/* One D3DTLVERTEX of an immediate sprite quad, of which Gfx_EmitScreenSprite writes the screen position and the
 * texture coordinates. */
typedef struct GfxScreenVertex {
    float posX;
    float posY;
    unsigned char pad_08[0x10];
    float u;
    float v;
} GfxScreenVertex;

/* Software rasteriser render-state creation parameters: surface size, pixel size, vertex count. */
typedef struct GfxRasterRenderDesc {
    int width;
    int height;
    int allocatePixelBlock;
    int bytesPerPixel;
    int fixedPitch;
    int zDepth;
    int zClearValue;
    unsigned char pad_1C[0x4];
    int vertexCount;
} GfxRasterRenderDesc;

/* Three matrix pointers and three D3D matrix handles used by D3DMultiplyMatrix. */
typedef struct GfxD3DMatrixSlots {
    unsigned char pad_00[0x14];
    int matrixPtr0;
    int matrixPtr1;
    int matrixPtr2;
    int handle0;
    int handle1;
    int handle2;
} GfxD3DMatrixSlots;

/* D3D viewport record filled by BuildD3DViewport: size, view rectangle and clip volume. */
typedef struct GfxD3DViewport {
    int viewportSize;
    int viewX;
    int viewY;
    int viewWidth;
    int viewHeight;
    int clipX;
    float clipY;
    int clipWidth;
    float clipHeight;
    int dvMinZ;
    int dvMaxZ;
} GfxD3DViewport;

/* Screen scale and primitive buffer used when emitting PSX-style textured flat quads. */
typedef struct GfxPsxQuadEmitter {
    unsigned char pad_00[0x24];
    float scaleX;
    float scaleY;
    unsigned char pad_2C[0x44];
    int primBuffer;
    unsigned char pad_74[0x4];
    int packetPtr;
    int attr;
} GfxPsxQuadEmitter;

/* Scale and packet-buffer state for emitting PSX textured Gouraud quads. */
typedef struct GfxPsxQuadState {
    unsigned char pad_00[0x24];
    float scaleX;
    float scaleY;
    unsigned char pad_2C[0x44];
    int packetBase;
    unsigned char pad_74[0x4];
    char *packetPtr;
    int attr;
} GfxPsxQuadState;

/* D3D execute-buffer compile state: integer counters and offsets set during compile. */
typedef struct GfxExecCompileState {
    int expectedSize;
    int writePointer;
    int vertexOffset;
    int vertexCnt;
    int vertexBytes;
    int instructionLength;
    int hVertexOffset;
} GfxExecCompileState;

/* Homogeneous point input to TransformPointHomogeneous with x, y, z floats */
typedef struct GfxHomogeneousPoint {
    float x;
    float y;
    float z;
} GfxHomogeneousPoint;

/* Node for a 2D textured blit: draw count and vertex data pointer. */
typedef struct GlBlit2DNode {
    unsigned char pad_00[0x8];
    int count;
    void *vertexData;
    unsigned char pad_10[0x4];
    void *vertexCount;
    unsigned char pad_18[0x8];
    void *indices;
    unsigned char pad_24[0xc];
    int *blendModeRef;
} GlBlit2DNode;

/* Source block for a textured 2D blit; count feeds the indexed and textured-rect draw paths. */
typedef struct GlBlit2DTexturedSource {
    unsigned char pad_00[0x8];
    int numberPrims;
    unsigned char pad_0C[0x8];
    int vertCursor;
    unsigned char pad_18[0x8];
    int count;
    unsigned char pad_24[0x4];
    int pagePtr;
    unsigned char pad_2C[0x4];
    int textureStep;
} GlBlit2DTexturedSource;

/* Rasterizer edge pair record filled by Gfx_RasterSetupEdgePair from vertex records. */
typedef struct SwEdgePair {
    unsigned char pad_00[0xc];
    int vertexKey;
    int sortKey;
    unsigned char pad_14[0x4];
    int xFractionA;
    int sortKeyA;
    int slopeLowA;
    int slopeHighA;
    int xFractionB;
    int sortKeyB;
    int slopeLowB;
    int slopeHighB;
} SwEdgePair;

/* Rasterizer edge pair record filled by Gfx_RasterSetupEdgePairNext from vertex records. */
typedef struct SwEdgePairNext {
    unsigned char pad_00[0xc];
    int vertexKey;
    int edgeParam;
    unsigned char pad_14[0x4];
    int xFractionA;
    int zIndexA;
    int slopeLowA;
    int slopeHighA;
    int xFractionB;
    int zIndexB;
    int slopeLowB;
    int slopeHighB;
} SwEdgePairNext;

/* Rasterizer edge pair record filled by Gfx_RasterSetupEdgePairRgb from vertex records. */
typedef struct SwEdgePairRgb {
    unsigned char pad_00[0xc];
    int vertexKey;
    int sortKey;
    unsigned char pad_14[0x4];
    int xFractionA;
    int zIndex;
    int setupValueA;
    int setupValueB;
    int xFractionB;
    int zIndexDup;
    int edgeAttrA;
    int edgeAttrB;
} SwEdgePairRgb;

/* Rasterizer edge pair record filled by Gfx_RasterSetupEdgePairZ from vertex records. */
typedef struct SwEdgePairZ {
    unsigned char pad_00[0xc];
    int vertexKey;
    int sortKey;
    unsigned char pad_14[0x4];
    int xFractionA;
    int zIndex;
    int slopeLowA;
    int slopeHighA;
    int xFractionB;
    int zIndexDup;
    int edgeAttrA;
    int edgeAttrB;
} SwEdgePairZ;

/* Rasterizer edge pair record filled by Gfx_RasterSetupEdgePairNextZ from vertex records. */
typedef struct SwEdgePairNextZ {
    unsigned char pad_00[0xc];
    int vertexKey;
    int edgeParam;
    unsigned char pad_14[0x4];
    int xFractionA;
    int screenX;
    int slopeLowA;
    int slopeHighA;
    int xFractionB;
    int screenXDup;
    int slopeLowB;
    int slopeHighB;
} SwEdgePairNextZ;

/* Rasterizer edge pair record filled by Gfx_RasterSetupEdgePairZRgb from vertex records. */
typedef struct SwEdgePairZRgb {
    unsigned char pad_00[0xc];
    int vertexKey;
    int sortKey;
    unsigned char pad_14[0x4];
    int xFractionA;
    int screenX;
    int slopeLowA;
    int slopeHighA;
    int xFractionB;
    int screenXDup;
    int edgeAttrA;
    int edgeAttrB;
} SwEdgePairZRgb;

/* Rasterizer edge pair record filled by Gfx_RasterSetupEdgePairNextZRgb from vertex records. */
typedef struct SwEdgePairNextZRgb {
    unsigned char pad_00[0xc];
    int vertexKey;
    int edgeParam;
    unsigned char pad_14[0x4];
    int xFractionA;
    int zIndex;
    int edgeAttrA;
    int edgeAttrB;
    int xFractionB;
    int zIndexDup;
    int altAttrA;
    int altAttrB;
} SwEdgePairNextZRgb;

/* A debug log target (0x14 bytes, Gfx_CreateDebugLogTarget): WriteDebugLog writes to its file when
   logToFile is set, and to OutputDebugStringA when enabled is; a truncating target reopens its
   file by name for each write, the other kind keeps the FILE open. */
typedef struct GfxDebugLogTarget {
    int logToFile;  /* +0x00 */
    int enabled;    /* +0x04 */
    int truncate;   /* +0x08 */
    char *fileName; /* +0x0C */
    void *file;     /* +0x10: the FILE * of a target that is not truncating */
} GfxDebugLogTarget;

/* Sorted draw queue: block heap and bucket table created with the queue. */
typedef struct GfxSortedDrawQueue {
    int clearedState;
    unsigned char pad_04[0x4];
    int depthRange;
    float depthRangeFloat;
    int bucketCount;
    float bucketCountFloat;
    int setRenderState;
    int setScenePass;
    int blockHeap;
    int bucketTable;
} GfxSortedDrawQueue;

/* Raster group settings selecting generated-vertex callbacks: mode switch and variant. */
typedef struct GfxRasterGroupSetup {
    unsigned char pad_00[0x8];
    unsigned char generatedSpan;
    unsigned char pad_09[0x6];
    unsigned char generatedVertices;
    unsigned char pad_10[0x10];
    int mode;
    int variant;
    int resource;
} GfxRasterGroupSetup;

/* Projection record that scales texture coordinates and supplies the triangle vertex array. */
typedef struct GfxPsxTexturedFlatTriangle {
    unsigned char pad_00[0x24];
    float scaleU;
    float scaleV;
    unsigned char pad_2C[0x44];
    int vertices;
    unsigned char pad_74[0x4];
    char *packetPtr;
    int attr;
} GfxPsxTexturedFlatTriangle;

/* Flat-shaded PSX quad primitive record used by the primitive drawer. */
typedef struct GfxPsxFlatQuadPrim {
    unsigned char pad_00[0x4];
    char colorR;
    char colorG;
    char colorB;
    unsigned char pad_07[0x1];
    short x0;
    short y0;
    short x1;
    short y1;
    short x2;
    short y2;
    short x3;
    short y3;
} GfxPsxFlatQuadPrim;

/* Z edge-pair setup input: vertex index, sort key and two fields copied into SwEdgePairZ. */
typedef struct SwEdgeZSpec {
    int vertexIndex;
    unsigned char pad_04[0xc];
    int sortKey;
    int scale;
    int slopeLow;
    int slopeHigh;
} SwEdgeZSpec;

/* Clip-space vertex read when an edge is intersected with a clip plane. */
typedef struct SwClipVertex {
    float clipX;
    float clipY;
    unsigned char pad_08[0xc];
    float x;
    float y;
    float clipZ;
    int cachedResult;
    unsigned char pad_24[0x14];
    int endSlot;
    unsigned char pad_3C[0x4];
    float endA;
    float vEnd;
    float clipAttr1;
    float clipAttr2;
    float clipAttr3;
} SwClipVertex;

/* Textured sprite quad target: X and Y scale factors and a vertex array. */
typedef struct GfxPsxSpriteQuadState {
    unsigned char pad_00[0x24];
    float scaleX;
    float scaleY;
    unsigned char pad_2C[0x44];
    int vertices;
    unsigned char pad_74[0x4];
    int packetPtr;
    int attr;
} GfxPsxSpriteQuadState;

/* Projected vertex (screen x and y, depth, w) enqueued for depth sorting; 0x14-byte records. */
typedef struct GfxSortVertex {
    float screenX;
    float screenY;
    float depth;
    int recipW;
    float w;
} GfxSortVertex;

/* A DirectDraw surface description (DDSURFACEDESC, 0x6C bytes): dwSize, dwFlags (DDSD_*),
 * dwHeight, dwWidth, ..., the pixel format's dwSize at +0x48 and ddsCaps.dwCaps (DDSCAPS_*) at
 * +0x68. GfxCreateTextureSurfaces fills it for CreateSurface; the movie keeps its texture's. */
typedef struct GfxTextureSurfaceDesc {
    int size;
    int flags;
    int height;
    int width;
    int pitch; /* +0x10: lPitch */
    int backBufferCount; /* +0x14: dwBackBufferCount */
    int zBufferBitDepth; /* +0x18: dwZBufferBitDepth */
    unsigned char pad_1C[0x8];
    void *surface; /* +0x24: lpSurface, the locked pixels */
    unsigned char pad_28[0x20];
    int pixelFormatSize;
    unsigned char pad_4C[0x1c];
    int caps;
} GfxTextureSurfaceDesc;

/* Screen-space mesh vertex (x, y, z, scale) built by the mesh transform-and-draw routine. */
typedef struct GfxMeshScreenVertex {
    float x;
    float y;
    float z;
    float scale;
} GfxMeshScreenVertex;

/* Per-step texture coordinate increments of a 2D textured blit source. */
typedef struct GlBlitTexturedStep {
    int blendMode;
    unsigned char pad_04[0x34];
    int stepU;
    int stepV;
} GlBlitTexturedStep;

/* A texture surface record (0xA0 bytes, Gfx_AllocSurfaceRecord): what a driver's texture load
 * (driver table +0x50: GfxCreateTextureSurfaces, Gl_OpsLoadTexture, the software rasterizer's)
 * makes of a .TEX header for a device format, and what a render state holds as its texture.
 * Direct3D uploads the pixels into a system-memory surface (+0) and loads its texture (+4) into
 * the surface the device draws from (+8, +0xC), whose description and handle it keeps; GL keeps
 * its texture name at +0x7C and a name per palette at +0x9C. Every driver keeps the header, the
 * format, the palette and the palette row. */
typedef struct GfxTextureSurface {
    int sourceSurface; /* +0x00: IDirectDrawSurface in system memory the pixels are uploaded into */
    int sourceTexture; /* +0x04: its IDirect3DTexture2, which the device texture Loads from */
    int surface; /* +0x08: the IDirectDrawSurface the device draws from */
    int d3dTexture; /* +0x0C: its IDirect3DTexture2 */
    GfxTextureSurfaceDesc surfaceDesc; /* +0x10: GetSurfaceDesc of surface (caps tell video or system memory) */
    int handle; /* +0x7C: the D3DTEXTUREHANDLE (GetHandle); the GL driver's texture name */
    union {
        int format; /* +0x80: the device format: a GfxTextureFormatRecord (Direct3D), its GfxPixelFormat (GL, software) */
        struct GfxTextureFormatRecord *formatRecord;
    };
    union {
        int header; /* +0x84: the GfxTexHeader the surface was made from */
        struct GfxTexHeader *texHeader;
    };
    int paletteRow; /* +0x88: the palette the surface shows (the header's row); -1 after a palette upload */
    union {
        int palette; /* +0x8C: the GfxPaletteRecord (Gfx_CreateSurfacePaletteRecord) */
        struct GfxPaletteRecord *paletteRecord;
    };
    unsigned char pad_90[0x8];
    int pageHandles; /* +0x98: a texture handle per palette, read only on a path dead in retail */
    int glNameTable; /* +0x9C: the GL driver's texture name per palette (Gl_AllocTextureNameTable) */
} GfxTextureSurface;

/* The render state of a draw group, as the depth-sorted draw paths read it. */
typedef struct GfxGroupRenderState {
    unsigned char pad_00[0x14];
    GfxTextureSurface *texture; /* +0x14: restored when its surface is lost */
    unsigned char pad_18[0x8];
    int usesWorldTransform; /* +0x20: the group draws under its own world transform */
    unsigned char pad_24[0x24];
    int depthSorted; /* +0x48: with the context's sortedDrawEnabled, the group's batches are queued for depth sorting */
    int sortPerTriangle; /* +0x4C: set to queue the group's triangles one by one, clear to queue it whole */
} GfxGroupRenderState;
typedef char CheckGfxTextureSurface[(sizeof(GfxTextureSurface) == 0xA0) ? 1 : -1];

/* Wireframe triangle vertex: screen x and y, drawn as line endpoints. */
typedef struct GfxRasterWireVertex {
    unsigned char pad_00[0x20];
    int clipFlag;
    unsigned char pad_24[0x8];
    int screenX;
    unsigned char pad_30[0x4];
    int screenY;
} GfxRasterWireVertex;

/* Draw pool list header: node count, high-water mark and head node. */
typedef struct GfxDrawPoolList {
    int active;
    int count;
    unsigned char pad_08[0x4];
    int highWater;
    int head;
} GfxDrawPoolList;

/* PSX line primitive: two Gouraud endpoints, each with RGB colour and screen position. */
typedef struct GfxPsxGouraudLine {
    unsigned char pad_00[0x4];
    char colorR0;
    char colorG0;
    char colorB0;
    unsigned char pad_07[0x1];
    short x0;
    short y0;
    char colorR1;
    char colorG1;
    char colorB1;
    unsigned char pad_0F[0x1];
    short x1;
    short y1;
} GfxPsxGouraudLine;

/* PSX textured sprite primitive: colour, position, texture coordinates and CLUT. */
typedef struct GfxPsxTexturedSprite {
    unsigned char pad_00[0x4];
    char colorR;
    char colorG;
    char colorB;
    unsigned char pad_07[0x1];
    short x;
    short y;
    unsigned char texU;
    unsigned char texV;
    unsigned short clut;
    short width;
    short height;
} GfxPsxTexturedSprite;

/* Edge source record for edge-pair setup: vertex index and sort key. */
typedef struct GfxRasterEdgePairSource {
    int vertexIndex;
    unsigned char pad_04[0xc];
    int sortKey;
    int scale;
    int setupValueA;
    int setupValueB;
} GfxRasterEdgePairSource;

/* Software-raster depth buffer record with its storage pointer and ownership flag. */
typedef struct GfxRasterZBuffer {
    unsigned char pad_00[0x4];
    int depthScale;
    int clearValue;
    int bytesPerPixel;
    int byteSize;
    int stride;
    int ownsBuffer;
    int buffer;
} GfxRasterZBuffer;

/* Header of a polygon set, read by the D3D5 polygon submit path. */
typedef struct GfxPolygonSetHeader {
    int drawListActive;
    unsigned char pad_04[0x4];
    int kind;
    int tag;
    int payload;
    int tagPtr;
} GfxPolygonSetHeader;

/* 36-byte entry of a loaded polygon set, filled by GfxLoadPolygonSet. */
typedef struct GfxPolygonSetEntry {
    unsigned char pad_00[0x4];
    int elementCount;
    int vertices; /* +0x08: the locked vertex data (Gfx_LockPolygonSetGroup) */
    int auxPointer; /* +0x0C: a second cursor Gfx_EmitPolygonSetPrimitive steps by 0x20 with each vertex */
    int indexBase;
    int vertexTable;
    int lightRecords; /* +0x18: the 0x18-byte GfxLightVertexRecords (Gfx_LightBuildModelVertexRecords) */
    int lightRecordPtr;
    int texture; /* +0x20: the group's GfxTextureSurface (its palette is rewritten by the colour updates) */
} GfxPolygonSetEntry;

/* Three vertex references of a raster triangle, used by the clip and wireframe paths. */
typedef struct GfxRasterTriangle {
    int vertex0;
    int vertex1;
    int vertex2;
} GfxRasterTriangle;

/* Nine-word light direction block filled from three light direction vectors. */
typedef struct GfxLightDirectionMatrix {
    int dir0X;
    int dir0Y;
    int dir0Z;
    unsigned char pad_0C[0x4];
    int dir1X;
    int dir1Y;
    int dir1Z;
    unsigned char pad_1C[0x4];
    int dir2X;
    int dir2Y;
    int dir2Z;
} GfxLightDirectionMatrix;

/* Nine-word light colour block filled from three light records. */
typedef struct GfxLightColorMatrix {
    int m00;
    int m01;
    int m02;
    unsigned char pad_0C[0x4];
    int m10;
    int m11;
    int m12;
    unsigned char pad_1C[0x4];
    int m20;
    int m21;
    int m22;
} GfxLightColorMatrix;

/* Projected point (x, y, z, depth) emitted as a PSX Gouraud polyline. */
typedef struct GfxPsxProjectedPoint {
    float x;
    float y;
    float z;
    float depth;
} GfxPsxProjectedPoint;

/* PSX flat primitive: three colour bytes followed by six screen coordinate shorts. */
typedef struct GfxPsxFlatPrimitive {
    unsigned char pad_00[0x4];
    char colorR;
    char colorG;
    char colorB;
    unsigned char pad_07[0x1];
    short x0;
    short y0;
    short x1;
    short y1;
    short x2;
    short y2;
} GfxPsxFlatPrimitive;

/* Float X and Y gradients for the UV and depth span setup of a raster triangle. */
typedef struct GfxRasterUvDepthGradients {
    float deltaBa; /* +0x0 */
    float deltaCa;
    float scaleC;
    float scaleB;
    float invDenominator;
    float invDenominatorNeg;
    unsigned char pad_18[0x8];
    float gradientX0;
    unsigned char pad_24[0x10];
    float gradientX1;
    float gradientX2;
    unsigned char pad_3C[0x4];
    float gradientY0;
    unsigned char pad_44[0x10];
    float gradientY1;
    float gradientY2;
} GfxRasterUvDepthGradients;

/* Edge pair that seeds UV and attribute values for a raster span. */
typedef struct GfxRasterEdgePair {
    int vertexIndex;
    unsigned char pad_04[0xc];
    int sortKey;
    int scale;
    int startU;
    int startV;
} GfxRasterEdgePair;

/* Vertex copied with a per-axis offset into the raster submit buffer. */
typedef struct GfxRasterSubmitVertex {
    float x;
    float y;
    float z;
    unsigned char pad_0C[0x4];
    int packedColor;
    unsigned char pad_14[0x4];
    int u;
    int v;
} GfxRasterSubmitVertex;

/* Z-buffer clear schedule: countdown, interval, buffer, mode and stride. */
typedef struct GfxZBufferClearState {
    int countdown;
    int interval;
    int clearValue;
    int mode;
    unsigned char pad_10[0x4];
    int stride;
    unsigned char pad_18[0x4];
    int buffer;
} GfxZBufferClearState;

/* Four float colour components holding 0-255 values scaled to 0-1 by the light normalise helpers. */
typedef struct GfxNormalizedColor {
    float colorR;
    float colorG;
    float colorB;
    float colorA;
} GfxNormalizedColor;

/* Draw spec (0x34 bytes) that Gfx_ApplyDrawSpecToGroup applies to each group of a polygon set: a
 * colour operation, UV and texture modes and their operands. The battle draws fill one with
 * Gfx_InitDrawSpec (which also clears the 16 per-group palette row bytes that follow, read at
 * +0x34 + group) and pass it through Gfx_ApplyDrawSpecToModel; draw-list nodes embed one at
 * +0x4C (D3D, GL and software submits) or +0x5C (GfxFlushDrawList). */
typedef struct GfxDrawSpec {
    union {
        int kindWord;              /* +0x00: Gfx_InitDrawSpec clears the four mode bytes as one word */
        struct {
            unsigned char kind;    /* +0x00: the colour operation Gfx_ApplyDrawSpecToGroup switches on */
            char mode;             /* +0x01: 1 = GfxFlushDrawList calls `callback` */
            char uvMode;           /* +0x02: 1 restores the UVs, 2 scrolls them by scrollU/scrollV */
            char vMode;            /* +0x03: 1 restores V, 2 mirrors it by scrollV */
        };
    };
    union {
        int modeWord;              /* +0x04: cleared as one word by Gfx_InitDrawSpec */
        struct {
            char texturePageMode;  /* +0x04: 1 = Gfx_SelectTexturePage(texturePage, textureState) */
            char textureStateMode; /* +0x05: 1 = Gfx_ApplyStateTexturedCall(textureState) */
            unsigned char pad_06[0x2];
        };
    };
    union {
        int packedColor;           /* +0x08: the whole BGRA word (kind 5, 0xD) */
        struct {
            unsigned char colorA;  /* +0x08: the colour's bytes (kinds 9, 0xB) */
            unsigned char colorB;
            unsigned char colorC;
            unsigned char alpha;   /* +0x0B: the blend level (kinds 1, 8..0xC) */
        };
    };
    union {
        int callback;              /* +0x0C */
        void (__cdecl *beginCallback)(void);
    };
    union {
        int vertexAlphaCallback;   /* +0x10: the per-vertex callback (kinds 0x10..0x12) */
        void (__cdecl *vertexAlphaFn)(void *, void *);
    };
    unsigned char pad_14[0x10];
    float scrollU;                 /* +0x24 */
    float scrollV;                 /* +0x28 */
    int texturePage;               /* +0x2C */
    int textureState;              /* +0x30 */
} GfxDrawSpec;

/* Full 0x44-byte standalone draw spec initialized by Gfx_InitDrawSpec, including the
 * 16 per-group palette row bytes at +0x34..+0x43. */
typedef struct GfxGroupDrawSpec {
    GfxDrawSpec spec;
    unsigned char groupPaletteRows[0x10];
} GfxGroupDrawSpec;

/* Transformed vertex of a group queued for depth sort; its z is averaged into the sort key. */
typedef struct GfxDepthSortVertex {
    float posX;
    float posY;
    float depth;
    int recipW;
    float clipW;
} GfxDepthSortVertex;

/* Group batch entry (0x38 bytes) indexed by group number from a table in the device state. */
typedef struct GfxGroupBatchEntry {
    unsigned char pad_00[0x4];
    int baseIndex;
    int faceCount;
    int startVert;
    int maxIdx;
    unsigned char pad_14[0x18];
    int pad_2C;
    int pad_30;
} GfxGroupBatchEntry;

/* Draw group list header reached through a unit's matrices pointer; words 14-1C go to invoke. */
typedef struct GfxDrawListTransforms {
    unsigned char pad_00[0x14];
    int worldMatrix;
    int viewMatrix;
    int projectionMatrix;
    int setWorld;
    int setView;
    int setProjection;
} GfxDrawListTransforms;

/* Indexed draw arguments for the retry path; flags is a flag word whose bit 4 is toggled. */
typedef struct GfxIndexedDrawArgs {
    unsigned char pad_00[0x8];
    int primitiveType;
    int vertexType;
    unsigned char pad_10[0x4];
    int vertexCount;
    int startIndex;
    int primitiveCount;
    int flags;
} GfxIndexedDrawArgs;

/* Only the fields the code reads; the size is not known. */
typedef struct GfxSpriteBatchHeader {
    int posX;
    int posY;
    int mode;
    int rhw;
    int diffuse;
    int specular;
    int texU;
    int texV;
} GfxSpriteBatchHeader;

/* Four colour bytes per entry, written by Gl_BuildColorArrays. */
typedef struct GlColorBytes {
    unsigned char colorIndex0;
    unsigned char colorIndex1;
    unsigned char colorIndex2;
    unsigned char colorIndex3;
} GlColorBytes;

/* Scanline edge record: row delta, x start, fixed-point x copy and per-row slope. */
typedef struct GlRasterEdgeSlope {
    unsigned char pad_00[0xc];
    int deltaY;
    int rowCount;
    float x;
    int xFixedLow; /* crt_FloatToInt64 of x */
    int xFixedHigh;
    float xSlope;
} GlRasterEdgeSlope;

/* Light record for one model vertex: a tag, a float colour at +4 and a packed BGRA colour. */
typedef struct GfxLightVertexRecord {
    int tag;
    float color[4];
    int packedColor;
} GfxLightVertexRecord;

/* PSX textured sprite primitive: colour, screen position, texture coordinates and CLUT position. */
typedef struct GfxPsxSprite {
    unsigned char pad_00[0x4];
    char r;
    char g;
    char b;
    unsigned char pad_07[0x1];
    short x;
    short y;
    unsigned char u;
    unsigned char v;
    unsigned short clut;
} GfxPsxSprite;

/* Triangle vertex with depth and three colour channels used for gradient deltas. */
typedef struct SwRgbGradientVertexB {
    unsigned char pad_00[0x8];
    float depth;
    unsigned char pad_0C[0x3c];
    float colorChannel1;
    float colorChannel2;
    float colorChannel3;
} SwRgbGradientVertexB;

/* Edge-pair setup input: a vertex index and fields copied into the SwEdgePairNextZRgb state. */
typedef struct SwEdgeSpec {
    int vertexIndex;
    unsigned char pad_04[0xc];
    int edgeParam;
    int scale;
    int edgeAttrA;
    int edgeAttrB;
} SwEdgeSpec;

/* Rasterizer vertex record: depth, three colour channels, Z table index and texture coordinates. */
typedef struct SwVertex2 {
    unsigned char pad_00[0x8];
    int depth;
    unsigned char pad_0C[0x20];
    int screenX;
    unsigned char pad_30[0x4];
    int screenY;
    unsigned char pad_38[0x10];
    int colorChannel1;
    int colorChannel2;
    int colorChannel3;
} SwVertex2;

/* Matrix array created by CreateMatrixArray, with a count, a matrix block and a handle. */
typedef struct GfxMatrixArray {
    int hasMatrices;
    int count;
    unsigned char pad_08[0x4];
    int matrixBuffer;
    int matrices;
    unsigned char pad_14[0xc];
    int handle;
} GfxMatrixArray;

/* Polygon-set draw node sub-record used by the fallback submit path. */
typedef struct GfxSubmitFallbackNode {
    int drawListActive;
    unsigned char pad_04[0x4];
    int kind;
    int matchValue;
    int list;
    int matchPtr;
} GfxSubmitFallbackNode;

/* Polygon-set node in the D3D5 submit list, linked through its first field. */
typedef struct GfxD3DPolygonSetNode {
    int next;
    int kind;
    int drawNode;
    float matrix[16];             /* +0x0C: the node's 4x4 transform */
    unsigned char drawSpec[0x6];  /* +0x4C: the start of the node's draw spec */
    unsigned char flagA;
    unsigned char flagB;
} GfxD3DPolygonSetNode;

/* Draw node that points at D3D execute buffers and per-index tables. */
typedef struct GfxD3DExecuteBufferNode {
    unsigned char pad_00[0x4];
    int active;
    unsigned char pad_08[0x28];
    int buffers;
    int records;
    unsigned char pad_38[0x8];
    int matrix;
    unsigned char pad_44[0x8];
    int indexTable;
} GfxD3DExecuteBufferNode;


/* Draw-op list node walked by Gl_OpsDrawPolygonSet; opcode picks the draw path. */
typedef struct GlDrawOpNode {
    int next;
    int opcode;
    int drawNode;
    unsigned char pad_0C[0x46];
    unsigned char offsetEnabled;
    unsigned char offsetMode;
} GlDrawOpNode;

/* Clip bounds in vertex index space that Gfx_RasterProjectClippedVertex tests vertices against. */
typedef struct GlRasterClipBounds {
    unsigned char pad_00[0x14];
    int clipMinX;
    int clipMinY;
    unsigned char pad_1C[0x8];
    int clipMaxX;
    int clipMaxY;
    unsigned char pad_2C[0xc];
    float scaleDepth;
    float scaleU;
    float scaleV;
} GlRasterClipBounds;

/* Attribute block of a PSX primitive set; Gfx_SetPsxPrimQuadState stores one value per opcode slot. */
typedef struct GfxPsxAttrBlock {
    unsigned char pad_00[0x28];
    int polyFt3State;
    int state;
    int polyGt3State;
    int polyGt4State;
} GfxPsxAttrBlock;

/* 32-byte projected vertex in a PSX vertex buffer: scaled xyz and texture coordinates. */
typedef struct GfxPsxProjectedVertex {
    float x;
    float y;
    float z;
    float invW;
    int payload;
    unsigned char pad_14[0x4];
    float texU;
    float texV;
} GfxPsxProjectedVertex;

/* 32-byte vertex written at an index into the PSX vertex buffer by an unused projection helper. */
typedef struct GfxPsxVertexIndexed {
    float x;
    float y;
    float z;
    float invW;
    int payload;
    unsigned char pad_14[0x4];
    int texU;
    int texV;
} GfxPsxVertexIndexed;

/* Screen-space textured vertex record in the PSX primitive buffer, with texture coordinates. */
typedef struct GfxPsxScreenVertex {
    int x;
    int y;
    int depth;
    int reciprocal;
    int payload;
    unsigned char pad_14[0x4];
    float texU;
    float texV;
} GfxPsxScreenVertex;

/* Flat-colored line primitive record read by Gfx_DrawPsxPrimitive, with RGB bytes and endpoints. */
typedef struct GfxPsxLinePrimitive {
    unsigned char pad_00[0x4];
    char colorR;
    char colorG;
    char colorB;
    unsigned char pad_07[0x1];
    short x0;
    short y0;
    short x1;
    short y1;
} GfxPsxLinePrimitive;

/* Flat-colored rectangle primitive record read by Gfx_DrawPsxPrimitive, with RGB bytes and origin. */
typedef struct GfxPsxRectPrimitive {
    unsigned char pad_00[0x4];
    char colorR;
    char colorG;
    char colorB;
    unsigned char pad_07[0x1];
    short x;
    short y;
    short width;
    short height;
} GfxPsxRectPrimitive;

/* Descriptor for Gfx_CreateModelTableBank: mesh bank parameters, a flag and a draw pool size. */
typedef struct GfxModelTableDesc {
    int meshBankEnabled;
    int recordCount;
    int vertexCount;
    int indexCount;
    int entryCount;
    int drawPoolEnabled;
    int nodeCount;
} GfxModelTableDesc;

/* Node of the software blit polygon list: next link, kind, and a draw list node pointer. */
typedef struct SwBlitPolygonNode {
    int next;
    int kind;
    int drawNode;
    unsigned char pad_0C[0x46];
    unsigned char offsetEnabled;
    unsigned char offsetMode;
} SwBlitPolygonNode;

/* Edge-pair source record: vertex index and sort key used by the edge pair setup. */
typedef struct SwEdgePairSource {
    int vertexIndex;
    unsigned char pad_04[0xc];
    int sortKey;
    unsigned char pad_14[0x4];
    int slopeLow;
    int slopeHigh;
} SwEdgePairSource;

/* Raster vertex with depth and three colour channel seeds, read by the ZRgb edge setup. */
typedef struct SwVertexZRgb {
    unsigned char pad_00[0x8];
    int depth;
    unsigned char pad_0C[0x20];
    int screenX;
    unsigned char pad_30[0x4];
    int pixelY;
    unsigned char pad_38[0x10];
    int colorChannel1;
    int colorChannel2;
    int colorChannel3;
} SwVertexZRgb;

/* Raster vertex with depth and texture coordinates, read by the Z/UV edge setup. */
typedef struct SwVertexZUv {
    unsigned char pad_00[0x8];
    int depth;
    unsigned char pad_0C[0x20];
    int screenX;
    unsigned char pad_30[0x4];
    int screenY;
    unsigned char pad_38[0x8];
    int texCoordU;
    int texCoordV;
} SwVertexZUv;

/* Node of a graphics draw pool list, allocated from a memory heap. */
typedef struct GfxDrawPoolNode {
    int nextNode;
    int nodeKind;
    int batchBlock;
} GfxDrawPoolNode;

/* Three rotation angles (X, Y, Z) in shorts, converted for the rotation matrix builders. */
typedef struct GfxRotationAngles2 {
    short angleX;
    short angleY;
    short angleZ;
} GfxRotationAngles2;

/* Vertex-like record copied into a PSX primitive for depth sorting; z is read at +0x08. */
typedef struct GfxPsxSortVertex {
    int x;
    int y;
    float z;
    int recipW;
    float clipW;
} GfxPsxSortVertex;

/* 32-byte textured vertex read by the Gl_DrawTris textured triangle paths. */
typedef struct GlTexVertex {
    float posX;
    float posY;
    float posZ;
    unsigned char pad_0C[0x4];
    int color;
    unsigned char pad_14[0x4];
    float texU;
    float texV;
} GlTexVertex;

/* Function-pointer table at GfxRasterContext.callbacks, used by the triangle clip and draw path. */
typedef struct GfxRasterCallbacks {
    unsigned char pad_00[0xc];
    int (__cdecl *prepareCallback)(int scratch, int target, int chain);
    int (__cdecl *stepCallback)(int target, int chain, int scratch);
    int (__cdecl *finishCallback)(int status, int target, int chain, int scratch);
    int (__cdecl *releaseCallback)(int target);
    void (__cdecl *resolveVertex)(int vertex, int context); /* +0x1C: resolves a vertex that is in view */
    int (__cdecl *projectVertex)(int vertex, int context);
    int snapVertex;
    unsigned char pad_28[0x4];
    void (__cdecl *drawTriangle)(int v0, int v1, int v2, int callbacks, int context);
    int depthConfig;
} GfxRasterCallbacks;

/* Raster draw context at +0x38; its callbacks table is at +0x154, and lineContext is the line
 * context used by the line and triangle paths. */
typedef struct GfxRasterContext {
    unsigned char pad_00[0x154];
    GfxRasterCallbacks callbacks;
    int lineContext;
    int mtxSet;
} GfxRasterContext;

/* Corner of a textured rectangle quad built by Gfx_RasterTryDrawTexturedRectQuads. */
typedef struct GfxTexturedRectCorner {
    float posX;
    float posY;
    unsigned char pad_08[0x10];
    float texU;
    float texV;
} GfxTexturedRectCorner;

/* Six render-object handles of a PSX graphics set, created and freed together. */
typedef struct GfxPsxRenderObjectSet {
    int renderObjectSlot0;
    unsigned char pad_04[0x8];
    int renderObjectSlot1;
    unsigned char pad_10[0x8];
    int renderObjectSlot2;
    int renderObjectSlot3;
    int renderObjectSlot4;
    int renderObjectSlot5;
} GfxPsxRenderObjectSet;

/* Two-word model table record read by the model table loader. */
typedef struct GfxModelTableCreateDesc {
    int renderBits;
    int drawPool;
    int color;
} GfxModelTableCreateDesc;

/* The software rasterizer's 96-byte vertex record, which Gfx_RasterPrepareVertexRecords fills from
 * a 32-byte source vertex (SwRasterSourceVertex) and the Gfx_RasterResolveVertex* family completes:
 *   +0x00..+0x0C  x, y, z, w (projected); a resolver snaps x and y to the pixel and scales z
 *   +0x20         isSet: in view, passed to the context's callback +0x1C
 *   +0x24         clipFlags: 1/2/4/8 outside left/right/top/bottom, 0x10 z negative, 0x20 z >= 1,
 *                 0x100 w <= 0
 *   +0x28, +0x30  x and y as 64-bit integers (crt_FloatToInt64); +0x2C and +0x34 are the pixel
 *   +0x38         the source vertex
 *   +0x3C         resolved: the resolvers run only while it is 0
 *   +0x40..+0x54  texture u and v, the colour's red, green and blue as floats, the packed colour
 * Floats the code copies as ints have an int spelling beside them. */
typedef struct SwRasterVertex {
    union {
        float x;
        int xBits;
    };
    union {
        float y;
        int yBits;
    };
    union {
        float depth;
        int depthBits;
    };
    union {
        float w;
        int wBits;
    };
    unsigned char pad_10[0x10];
    int isSet;
    int clipFlags;
    int fixedXLow;
    int pixelX;
    int fixedYLow;
    int pixelY;
    int source;
    int resolved;
    float texCoordU;
    float texCoordV;
    union {
        float red;
        int redBits;
    };
    union {
        float green;
        int greenBits;
    };
    union {
        float blue;
        int blueBits;
    };
    int packedColor;
    unsigned char pad_58[0x8];
} SwRasterVertex;

/* Raster vertex used as the RGB gradient origin; each vertex's colour channels are taken relative to it. */
typedef struct SwRasterVertex3 {
    unsigned char pad_00[0x48];
    float colorChannel1;
    float colorChannel2;
    float colorChannel3;
} SwRasterVertex3;

/* Edge record at +0x5C of a raster setup block: vertex index, sort key and edge values. */
typedef struct SwEdgeRecord {
    int vertexIndex;
    unsigned char pad_04[0xc];
    int sortKey;
    int scale;
    int edgeAttrA;
    int edgeAttrB;
} SwEdgeRecord;

/* Raster vertex: depth, texture coordinates and colour channels used by the triangle setup. */
typedef struct SwRasterVertex4 {
    unsigned char pad_00[0x2c];
    int pixelX;
    unsigned char pad_30[0x4];
    int pixelY;
    unsigned char pad_38[0x10];
    int colorChannel1;
    int colorChannel2;
    int colorChannel3;
} SwRasterVertex4;

/* Raster vertex: depth, texture coordinates and colour channels used by the triangle setup. */
typedef struct SwRasterVertex5 {
    unsigned char pad_00[0x2c];
    int screenX;
    unsigned char pad_30[0x4];
    int screenY;
    unsigned char pad_38[0x8];
    int texCoordU;
    int texCoordV;
} SwRasterVertex5;

/* Texture descriptor whose width and height give the raster texture masks and shift. */
typedef struct SwTextureInfo {
    unsigned char pad_00[0x3c];
    int width;
    int height;
    unsigned char pad_44[0x90];
    int maskSourceFallback;
    int maskSource;
} SwTextureInfo;

/* Float clip rectangle built from the raster context; right and bottom are one past the edge. */
typedef struct SwFloatClipRect {
    unsigned char pad_00[0x4];
    float clipLeft;
    float clipRight;
    float clipTop;
    float clipBottom;
    int nearPlane;
    float farLimit;
} SwFloatClipRect;

/* Integer clip rectangle copied from the raster context; right and bottom are exclusive. */
typedef struct SwIntClipRect {
    unsigned char pad_00[0x14];
    int clipLeft;
    int clipTop;
    int viewSizeX;
    int viewSizeY;
    int clipRight;
    int clipBottom;
} SwIntClipRect;

/* DirectDraw DDPIXELFORMAT read by PixelFormatFromDDPixelFormat. */
typedef struct GfxDdPixelFormat {
    unsigned char pad_00[0x4];
    int flags;
    unsigned char pad_08[0x4];
    int rgbBitCount;
    int redMask;
    int greenMask;
    int blueMask;
    int alphaMask;
} GfxDdPixelFormat;


/* PSX textured flat triangle state with texture scale, vertex buffer and attribute output. */
typedef struct GfxPsxTexturedFlatTriangleState {
    unsigned char pad_00[0x24];
    float texScaleU;
    float texScaleV;
    unsigned char pad_2C[0x44];
    int vertexBuffer;
    unsigned char pad_74[0x4];
    char *attrPtr;
    int attr;
} GfxPsxTexturedFlatTriangleState;

/* DirectDraw display mode description handed to the enumeration callback. */
typedef struct GfxDisplayModeDesc {
    unsigned char pad_00[0x4];
    int flags;
    int height;
    int width;
    int pitch;
    unsigned char pad_14[0x30];
} GfxDisplayModeDesc;

/* One row of a 3x3 matrix of raw 32-bit words, the destination of TransposeMatrix3x3. */
typedef struct GfxRawMatrix3Row {
    int col0;
    int col1;
    int col2;
    unsigned char pad_0C[0x4];
} GfxRawMatrix3Row;

/* Render material whose D3D matrix handles, heap blocks and 4x4 matrices are freed on destroy. */
typedef struct GfxRenderMaterial {
    int matrix;
    unsigned char pad_04[0x8];
    int heapBlockA;
    int heapBlockB;
    struct PolygonTransform *matrix4A;
    int matrix4B;
    int matrix4C;
    int matrixHandleA;
    int matrixHandleB;
} GfxRenderMaterial;

/* GL device with three matrix slots, each a pointer to a 4x4 matrix stored by Gl_OpsSetMatrix. */
typedef struct GlMatrixDevice {
    unsigned char pad_00[0x14];
    void *matrixSlot0;
    void *matrixSlot1;
    void *matrixSlot2;
} GlMatrixDevice;

/* Software blit descriptor with fixed mode words and two values copied from the context at 0x954 and 0x958. */
typedef struct SwBlitDescriptor {
    int displayWidth;
    int displayHeight;
    int hasOwnBuffer;
    int bytesPerPixel;
    int usesAlignedBlock;
    int zDepth;
    int zClearValue;
    int zScale;
    int vertexCount;
} SwBlitDescriptor;

/* Three raster slots, each a pointer to a 64-bit blob that is overwritten and then replaced. */
typedef struct SwRasterSlots {
    unsigned char pad_00[0x14];
    int worldMatrix;
    int viewMatrix;
    int projectionMatrix;
} SwRasterSlots;

/* Mesh vertex whose x and y feed the wireframe line clipper as an end point. */
typedef struct GfxWireEndVertex {
    unsigned char pad_00[0x2c];
    int x;
    unsigned char pad_30[0x4];
    int y;
} GfxWireEndVertex;

/* Two depth values tracked as min and max while rasterising a depth-tested triangle. */
typedef struct GfxTriangleDepthRange {
    unsigned char pad_0[0x8];
    int depthSample; /* +0x8 */
    unsigned char pad_c[0x20];
    int x;
    unsigned char pad_30[0x4];
    int y;
} GfxTriangleDepthRange;

/* Sorted draw group record: a draw record pointer and a texture page, in pairs of entries. */
typedef struct GfxSortedGroupRecord {
    unsigned char pad_00[0x8];
    int statusObj;
    struct GfxPolygonBatch *drawRecord;
    int texturePage;
    GfxGroupRenderState *renderState;
    unsigned char pad_18[0x34];
} GfxSortedGroupRecord;

/* Pixel-format mask template copied into a surface pixel format by PixelFormatLoadMaskTemplate. */
typedef struct GfxPixelMaskTemplate {
    int palettized;
    int bits;
    int redMask;
    int greenMask;
    int blueMask;
    int alphaMask;
} GfxPixelMaskTemplate;

/* 4x4 float transform whose translation is cleared and whose w term is set to 1.0. */
typedef struct GfxTransformMatrix {
    unsigned char pad_00[0xc];
    int translateX;
    unsigned char pad_10[0xc];
    int translateY;
    unsigned char pad_20[0xc];
    int translateZ;
    int m30;
    int m31;
    int m32;
    int m33;
} GfxTransformMatrix;

/* Header words written when building a D3D execute-buffer vertex array. */
typedef struct GfxD3DExecuteBufferHeader {
    unsigned char pad_00[0x60];
    int structSize;
    int copyBuffer;
    int inVertexSize;
    int tmpVerts;
    int outVertexSize;
    int colorBuf;
    int flags;
} GfxD3DExecuteBufferHeader;

/* Draw node for a D3D execute buffer: a record index, a buffer node pointer and a flag. */
typedef struct GfxD3DExecuteDrawNode {
    unsigned char pad_00[0x4];
    int recordIndex;
    int bufferNode;
    unsigned char pad_0C[0x4];
    int active;
    unsigned char pad_14[0x3c];
} GfxD3DExecuteDrawNode;

/* 2-D untextured draw record: texture-coordinate offsets and draw parameters. */
typedef struct GfxDraw2DRecord {
    unsigned char pad_00[0xc];
    int vertexCount;
    unsigned char pad_10[0x4];
    int vertices;
    int indexCount;
    unsigned char pad_1C[0x4];
    int indices;
    unsigned char pad_24[0xc];
    struct GfxRenderObject *owner;
} GfxDraw2DRecord;

/* Deferred draw node: a record index, a device pointer and a flag. */
typedef struct GlDeferredDrawNode {
    unsigned char pad_00[0x4];
    int recordIndex;
    void *device;
    void *drawGroup;
    int hasInlineMatrix;
    unsigned char pad_14[0x3c];
} GlDeferredDrawNode;

/* Deferred draw node for the software blitter: a record index, a draw group and a GL group. */
typedef struct SwDeferredDrawNode {
    unsigned char pad_00[0x4];
    int recordIndex;
    int drawGroup;
    int glGroup;
    int mtx;
    unsigned char pad_14[0x34];
} SwDeferredDrawNode;

/* A texture format the device offers (0x84 bytes, AllocTextureFormatRecord): its pixel format and,
 * for the Direct3D formats, a heap copy of the DDSURFACEDESC D3DEnumTextureFormatsCallback got
 * (FreeTextureFormatRecord frees it). InsertTextureFormatRecord keeps the records in the engine
 * context's textureFormatList, ordered by bitsPerPixel and then alphaBits; Gfx_SelectTextureFormat's
 * callbacks pick one, and the texture keeps it as its surfaceSpec, whose DDSURFACEDESC each surface
 * is created from. */
typedef struct GfxTextureFormatRecord {
    GfxPixelFormat format; /* +0x00 */
    void *surfaceDesc; /* +0x80 */
} GfxTextureFormatRecord;
typedef char CheckGfxTextureFormatRecord[(sizeof(GfxTextureFormatRecord) == 0x84) ? 1 : -1];

/* Matrix table header: nonzero word, slot count and pointer to 64-byte matrix slots. */
typedef struct GfxMatrixTable {
    int slotBytes;
    int slotCount;
    unsigned char pad_08[0x4];
} GfxMatrixTable;

/* 4x4 viewport transform matrix built from the screen width, height and origin. */
typedef struct GfxViewportMatrix {
    float scaleX;
    unsigned char pad_04[0x10];
    float scaleY;
    unsigned char pad_18[0x18];
    float translateX;
} GfxViewportMatrix;

/* Execute group record: an entry count and two heap-allocated int arrays for the D3D run. */
typedef struct GfxExecGroup {
    unsigned char pad_00[0x10];
    int entryCount;
    unsigned char pad_14[0x34];
    int entryHandles;
    int entryStates;
} GfxExecGroup;

/* Compile-state record whose three values are emitted as transform states 1 to 3. */
typedef struct GfxExecTransformBlock {
    unsigned char pad_00[0x20];
    int worldMatrix;
    int viewMatrix;
    int projectionMatrix;
} GfxExecTransformBlock;

/* Render-state cache record: a base offset and cached words swapped during emission. */
typedef struct GfxRenderStateCache {
    unsigned char pad_00[0x4];
    int baseOffset;
    unsigned char pad_08[0x14];
    int renderCount;
    int lightSize;
} GfxRenderStateCache;

/* Texture info copied from a render object's texture record. */
typedef struct GfxRenderTextureInfo {
    int kind;
    int paletteCount;
    union { int pageU; int width; };
    union { int pageV; int height; };
} GfxRenderTextureInfo;

/* Draw node for untextured 2D triangle batches. */
typedef struct GlDraw2DNode {
    unsigned char pad_00[0x8];
    int count;
    void *srcVertices;
    unsigned char pad_10[0x4];
    void *vertexCount;
    unsigned char pad_18[0x8];
    void *indices;
    unsigned char pad_24[0xc];
    int *countPtr;
} GlDraw2DNode;

/* Clip-loop state; vertexIndex indexes a three-entry vertex pointer list in Gfx_RasterClipAndDrawTriangle. */
typedef struct GfxRasterClipState {
    int vertexIndex;
    int secondIndex;
    int thirdIndex;
} GfxRasterClipState;

/* Scroll group: UV scroll offsets kept wrapped below 1.0. */
typedef struct GfxScrollGroup {
    unsigned char pad_00[0x18];
    float scrollU;
    float scrollV;
} GfxScrollGroup;

/* Vertex with per-axis floor limits read by the floor-clamped colour transforms. */
typedef struct GfxFloorVertex {
    unsigned char pad_00[0xc];
    float floorX;
    unsigned char pad_10[0xc];
    float floorY;
    unsigned char pad_20[0xc];
    float floorZ;
} GfxFloorVertex;

/* Per-record size and texel step read by Gfx_EmitDraw2DTextured for a textured 2D run. */
typedef struct GfxTexturedRunInfo {
    unsigned char pad_00[0x24];
    float sizeX;
    float sizeY;
    unsigned char pad_2C[0xc];
    int stepU;
    int stepV;
} GfxTexturedRunInfo;

/* D3D execute buffer description filled while compiling a group. */
typedef struct GfxExecuteBufferDesc {
    int structSize; /* +0x0 */
    int flags;
    unsigned char pad_08[0x4];
    int bufferSize;
    int data;
} GfxExecuteBufferDesc;

/* Float colour (b, g, r) that modulates a vertex colour into bytes. */
typedef struct GfxFloatColor {
    float b;
    float g;
    float r;
    float a;
} GfxFloatColor;

/* Float light colour (b, g, r) for lit vertices, multiplied into a transformed colour. */
typedef struct GfxLitLightColor {
    float b;
    float g;
    float r;
} GfxLitLightColor;

/* Software raster vertex buffer: vertex count, aligned data size and the vertex array. */
typedef struct GfxRasterVertexBuffer {
    int clipFlags;
    int clipAnd;
    int vertexCount;
    int alignedVertices;
    int vertexData;
    int minX;
    int minY;
    unsigned char pad_1C[0x8];
    int maxX;
    int maxY;
    int stateA;
    int stateB;
    unsigned char pad_34[0x8];
    int scaleX;
    int scaleY;
    int scaleIndexX;
    unsigned char pad_48[0x4];
    int depthOffset;
} GfxRasterVertexBuffer;

struct GfxEngineContext;

/* A module's callbacks, as the engine calls them: the lifecycle ones get the engine context, the
 * mouse and key handlers the window message, its wParam and lParam and then the context. */
typedef void (__cdecl *GameModuleCallback)(struct GfxEngineContext *ctx);
typedef void (__cdecl *GameModuleMessageCallback)(int message, int wParam, int lParam, struct GfxEngineContext *ctx);

/* GameModule: the seven callbacks of a module (field, battle, world map, menu, the minigames, the
 * game-mode dispatcher). The engine context holds two: `activeModule` (+0x9F0), the one running,
 * and `pendingModule` (+0xA0C), the one RequestModuleSwitch (0x00666CF2) stages; it copies all
 * seven words (`rep movsd`, ecx = 7) and sets moduleSwitchPending. ServiceModuleSwitch then runs
 * the old exit, CommitPendingModule copies the five callbacks from enter to onKey across, and the
 * new enter runs. `init` and `shutdown` are set once, in the active record, when main creates the
 * context (App_InitSubsystems, App_Shutdown): the window procedure calls init when the window is
 * created and WinMain calls shutdown last; nothing copies them, so a staged record's two are
 * never read. A module that hands control back fills a record with the GameModeDispatcher_*
 * functions and the app's two message handlers (GetActiveModule reads the five active
 * callbacks into one first). All are __cdecl. */
typedef struct GameModule {
    int (__cdecl *init)(struct GfxEngineContext *ctx); /* +0x00 */
    GameModuleCallback shutdown; /* +0x04 */
    GameModuleCallback enter; /* +0x08 */
    GameModuleCallback exit; /* +0x0C */
    GameModuleCallback frame; /* +0x10 */
    GameModuleMessageCallback onMouse; /* +0x14 */
    GameModuleMessageCallback onKey; /* +0x18 */
} GameModule;

/* Node in the depth-sort batch list holding a record count and primitive and colour sources. */
typedef struct GfxDepthSortNode {
    int nextNode;
    unsigned char pad_04[0x4];
    int recordCount;
    unsigned char pad_0C[0x8];
    int primData;
    unsigned char pad_18[0x10];
    int colorSrc;
} GfxDepthSortNode;

/* Per-run U and V step values for the textured 3D draw, read from the record at +0x30. */
typedef struct Gfx3DTexturedInfo {
    unsigned char pad_00[0x38];
    int uRunStep;
    int vRunStep;
} Gfx3DTexturedInfo;

/* Polygon render state at context+0x944, read by Gl_DrawPolygonBatch. */
typedef struct GlPolygonRenderState {
    unsigned char pad_00[0x8];
    unsigned char paletteIdx;
    unsigned char pad_09[0x1b];
    int isGouraud;
    int texBatch;
} GlPolygonRenderState;

/* Device behind a deferred draw node; holds the record block and object table indexed by recordIndex. */
typedef struct GlDeferredDevice {
    unsigned char pad_00[0x4];
    int drawUnsorted;
    unsigned char pad_08[0x28];
    char *recordBlock;
    unsigned char pad_34[0x1c];
    int objectTable;
} GlDeferredDevice;

/* Textured GL vertex record with a 32-byte stride: position x/y/z and texture coordinates u/v. */
typedef struct GlTexVertexA {
    float x;
    float y;
    float z;
    unsigned char pad_0C[0x4];
    int color;
    unsigned char pad_14[0x4];
    float u;
    float v;
} GlTexVertexA;

/* Source blend record whose fields Gfx_ApplyBlendRecordToHundred copies into a PolygonHundredState. */
typedef struct GfxBlendRecord {
    unsigned char pad_00[0x4];
    int depthSortBatches;
    int alphaLevel;
    int srcBlend;
    int maskA;
    int dstBlend;
    int maskB;
    int enabled;
    int reservedWord;
} GfxBlendRecord;

/* PSX model table: entry count, the model directory, the records it was loaded from and a
   heap-allocated array of the loaded models. */
typedef struct GfxPsxModelTable {
    int count;
    char *directory;
    struct GfxModelRecord *recordTable;
    struct GfxPsxModelTableEntry *entries;
} GfxPsxModelTable;

/* PSX Gouraud polyline primitive slot 0 at batchNode+0x70: scaled x/y/z, invW and color. */
typedef struct GfxPsxPolylineSlot {
    float x;
    float y;
    float z;
    float invW;
    int color;
} GfxPsxPolylineSlot;

/* PSX flat screen-space line slot: x, y, z, w and color. */
typedef struct GfxPsxFlatScreenLineSlot {
    int x;
    int y;
    int z;
    int w;
    int color;
} GfxPsxFlatScreenLineSlot;

/* Screen-space flat quad vertex 0 with packed words and color. */
typedef struct GfxFlatQuadScreenVertex {
    int x;
    int y;
    int z;
    int w;
    int color;
} GfxFlatQuadScreenVertex;

/* Screen-space flat rectangle corner 0 with float x, y, packed words and color. */
typedef struct GfxFlatRectScreenCorner {
    float x;
    float y;
    int z;
    int w;
    int color;
} GfxFlatRectScreenCorner;

/* PSX primitive record drawn as an 8x8 flat rectangle at screen position x, y. */
typedef struct GfxPsxRect8Record {
    unsigned char pad_00[0x4];
    char colorR;
    char colorG;
    char colorB;
    unsigned char pad_07[0x1];
    short x;
    short y;
} GfxPsxRect8Record;

/* Per-context blit rectangle entry (x, y, width, rows) presented by Gfx_BlitContextRectsAndPresent. */
typedef struct SwBlitRectEntry {
    int active;
    int x;
    int y;
    int width;
    int rows;
} SwBlitRectEntry;

/* Raster vertex with depth and three colour bytes summed across a triangle for shading. */
typedef struct SwRasterVertexColorSum {
    unsigned char pad_00[0x8];
    float depth;
    unsigned char pad_0C[0x48];
    unsigned char colorR;
    unsigned char colorG;
    unsigned char colorB;
} SwRasterVertexColorSum;

/* Triangle record holding three vertex pointers and span-setup values for UV depth span tables. */
typedef struct SwRasterTriangle {
    int vertexA;
    int vertexB;
    int vertexC;
    unsigned char pad_0C[0x98];
    int spanWidth;
    int mappedColor;
} SwRasterTriangle;

/* Raster vertex whose U and V texture coordinates seed the edge-pair span gradients. */
typedef struct SwRasterVertexUv {
    unsigned char pad_00[0x2c];
    int screenX;
    unsigned char pad_30[0x4];
    int screenY;
    unsigned char pad_38[0x8];
    int u;
    int v;
} SwRasterVertexUv;

/* Source edge entry read by Gfx_RasterSetupEdgePairNextZ, indexing a vertex by vertexIndex. */
typedef struct SwEdgePairSourceEntry {
    int vertexIndex;
    unsigned char pad_04[0xc];
    int edgeParam;
    int depthSeed;
    int slopeLow;
    int slopeHigh;
} SwEdgePairSourceEntry;

/* Vertex fetched by index in the edge-pair NextZ setup; depth is the depth passed to the seed. */
typedef struct SwRasterVertexSource {
    unsigned char pad_00[0x8];
    int depth;
    unsigned char pad_0C[0x20];
    int screenX;
    unsigned char pad_30[0x4];
    int screenY;
} SwRasterVertexSource;

/* Transform block: enable flag, mode switch, and x/y/z offsets added to each submitted vertex. */
typedef struct SwRasterTransform {
    unsigned char pad_00[0x6];
    unsigned char enabled;
    unsigned char mode;
    unsigned char pad_08[0xc];
    float offsetX;
    float offsetY;
    float offsetZ;
} SwRasterTransform;

/* Line record with start and end coordinates used by the clipped-line endpoint code. */
typedef struct SwClippedLine {
    unsigned char pad_00[0x8];
    int xStart;
    int y0;
    int xLast;
    int clipY1;
    unsigned char pad_18[0x4];
    int dyValue;
} SwClippedLine;

/* Four-float slot block written by the PSX matrix to float 4x4 thunks. */
typedef struct GfxFloat4 {
    float pad_00;
    float pad_04;
    float pad_08;
    float pad_0C;
} GfxFloat4;

/* Screen-primitive item whose four words are passed to the DirectX draw call. */
typedef struct GfxScreenPrimitiveItem {
    unsigned char pad_00[0x14];
    int vertexCount;
    int indices;
    int indexCount;
    int flags;
} GfxScreenPrimitiveItem;

/* 32-byte entry of a depth-sort array; words at +0x10..+0x1C are copied into a sorted primitive. */
typedef struct GfxDepthSortEntryA {
    unsigned char pad_00[0x10];
    int color;
    int specular;
    int texU;
    int texV;
} GfxDepthSortEntryA;

/* DirectDraw surface description (size 0x6C) filled for a texture upload. */
typedef struct GfxDdSurfaceDesc {
    int size;
    unsigned char pad_04[0xc];
    int pitch;
    unsigned char pad_14[0x10];
    int surface;
} GfxDdSurfaceDesc;

/* 16-byte material colour filled from a BGRA colour word and floats. */
typedef struct GfxMaterialColor {
    float r;
    float g;
    int bgra;
    float a;
} GfxMaterialColor;

/* Geometry source of a draw group: position array and tag array, both indexed by a subrange index. */
typedef struct GfxGeometrySource {
    unsigned char pad_00[0x40];
    int positionBase;
    unsigned char pad_44[0x30];
    int tagBase;
} GfxGeometrySource;

/* Node of a draw-group list pass, linked through next, dispatched on kind, carrying a draw node. */
typedef struct GfxDrawStateNode {
    int next;
    int kind;
    int drawNode;
    float matrix[16]; /* +0x0C: the node's 4x4 transform */
} GfxDrawStateNode;

/* Range descriptor for building light-affected model vertex records. */
typedef struct GfxLightRange {
    unsigned char pad_00[0x4];
    int offset;
    int count;
    int colorBase;
    int elemCount;
    unsigned char pad_14[0x18];
    int pad_2C;
    int pad_30;
} GfxLightRange;

/* One entry of the GL context's colour-intensity table: i / 255 as a float, copied as its bits. */
typedef union GlColorTableEntry {
    float value;
    int bits;
} GlColorTableEntry;

/* GL context record allocated by Gl_AllocState (0x450 bytes), returned by Gl_GetState: the clear
   colour, the WGL rendering context and the pixel format the context was made with. */
typedef struct GlContext {
    int initFlag;
    int glQueryResult;
    Rgba background; /* +0x08: Gl_OpsSetBackgroundColor's colour; Gl_OpsClear passes it on */
    void *renderContext; /* +0x18: the WGL rendering context */
    GlPixelFormatDescriptor pixelFormat; /* +0x1C: Gl_PixelFormatFromPfd reads it */
    unsigned char pad_2E[0x16];
    GlColorTableEntry colorTable[0x100]; /* +0x44: Gl_AllocState fills it with i / 255 */
    int hasColorTableExt; /* +0x444 */
    int skipSubtractiveBlend; /* +0x448: when set, Gl_SetBlendModeSubtractive leaves glBlendFunc alone; nothing in src sets it */
    int unusedStateFlag; /* +0x44C: Gl_SetUnusedStateFlag's */
} GlContext;

/* 32-byte vertex with float x, y, z, drawn by glVertex3f in Gouraud triangles. */
typedef struct GlTriVertex {
    float x;
    float y;
    float z;
    unsigned char pad_0C[0x4];
    int color;
} GlTriVertex;

/* Device capability flags and the two masks used to match blend-mode records. */
typedef struct GfxBlendCaps {
    unsigned char pad_00[0x308];
    int flags;
    unsigned char pad_30C[0x8c];
    int maskA;
    int maskB;
} GfxBlendCaps;

/* Locked polygon-set group record used when mirroring V coordinates from the base copy. */
typedef struct GfxPolygonGroupVMirror {
    unsigned char pad_00[0x4];
    int vertexCount;
    int vertices;
    unsigned char pad_0C[0x10];
    int source;
} GfxPolygonGroupVMirror;

/* Four colour bytes that are scaled and clamped, then copied into a vertex colour. */
typedef struct GfxScaledColor {
    char channel0;
    char channel1;
    char channel2;
    char channel3;
} GfxScaledColor;

/* One 20-byte rectangle slot in the graphics context, stored as left, top, right, bottom. */
typedef struct GfxContextRectSlot {
    unsigned char pad_00[0x4];
    int left;
    int top;
    int right;
    int bottom;
} GfxContextRectSlot;

/* Four-byte BGRA colour written as blue, green, red, alpha. */
typedef struct GfxBgraPixel {
    char blue;
    char green;
    char red;
    char alpha;
} GfxBgraPixel;

/* Draw queue record for 2D textured blits without a texture. */
typedef struct SwBlitDrawQueue {
    unsigned char pad_00[0x13c];
    int colorScaleLut;
    int sumRemapLut;
} SwBlitDrawQueue;

/* Line-span primitive holding a mode switch and an index into the span callback tables. */
typedef struct SwLineSpanPrim {
    unsigned char pad_00[0x20];
    int mode;
    int callbackIndex;
} SwLineSpanPrim;

/* Triangle vertex compared by a sort key while building edge chains. */
typedef struct SwChainVertex {
    unsigned char pad_00[0x2c];
    int sortKey;
    unsigned char pad_30[0x4];
    int vertexKey;
} SwChainVertex;

/* Triangle vertex with a depth value and a 32-bit flat colour. */
typedef struct SwFlatColorVertex {
    unsigned char pad_00[0x8];
    float depth;
    unsigned char pad_0C[0x48];
    int color;
} SwFlatColorVertex;

/* Triangle vertex whose three colour channels are summed into a colour-table lookup. */
typedef struct SwColorSumVertexB {
    unsigned char pad_00[0x8];
    float depth;
    unsigned char pad_0C[0x48];
    unsigned char colorChannel1;
    unsigned char colorChannel2;
    unsigned char colorChannel3;
} SwColorSumVertexB;

/* Triangle vertex carrying texture coordinates u and v. */
typedef struct SwUvVertex {
    unsigned char pad_00[0x40];
    float u;
    float v;
} SwUvVertex;

/* Segment vertex referenced by edge records; its fields are copied into edge-pair nodes. */
typedef struct SwEdgeVertex {
    unsigned char pad_00[0x2c];
    int pixelX;
    unsigned char pad_30[0x4];
    int vertexKey;
} SwEdgeVertex;

/* Edge record at segment offset 0x80 for the Z-plus-RGB edge-pair path. */
typedef struct SwZRgbEdgeRecord {
    int vertexIndex;
    unsigned char pad_04[0xc];
    int edgeParam;
    unsigned char pad_14[0x4];
    int altAttrA;
    int altAttrB;
} SwZRgbEdgeRecord;

/* Edge record at segment offset 0x80 for the UV edge-pair path. */
typedef struct SwUvEdgeRecord {
    int vertexIndex;
    unsigned char pad_04[0xc];
    int sortKey;
    unsigned char pad_14[0x4];
    int endU;
    int endV;
} SwUvEdgeRecord;

/* Segment vertex whose depth seeds the Z edge-pair setup. */
typedef struct SwDepthVertex {
    unsigned char pad_00[0x8];
    int depth;
    unsigned char pad_0C[0x20];
    int pixelX;
    unsigned char pad_30[0x4];
    int vertexKey;
} SwDepthVertex;

/* Texture descriptor whose power-of-two width and height give the sampling masks. */
typedef struct SwTextureMask {
    unsigned char pad_00[0x3c];
    int width;
    int height;
    unsigned char pad_44[0x90];
    int pixelBuffer;
} SwTextureMask;

/* Callback slots in the render state at offset 0x38: the triangle draw callback and a depth value. */
typedef struct SwRenderCallbacks {
    unsigned char pad_00[0x180];
    int triangleCallback;
    int depthValue;
} SwRenderCallbacks;

/* Only the fields the code reads; the size is not known. */
typedef struct SwLineVertexRecord {
    unsigned char pad_00[0x4];
    int pad_04;
    int pad_08;
    float pad_0C;
} SwLineVertexRecord;

/* Line operation record holding a mode switch and a draw callback. */
typedef struct SwLineOps {
    int mode;
    unsigned char pad_04[0x1c];
    int (__cdecl *endpointCallback)(int endpoint, int context);
    unsigned char pad_24[0x4];
    int (__cdecl *drawCallback)(int lineContext);
} SwLineOps;

/* Textured rectangle draw state taken from the engine context at offset 0x944. */
typedef struct SwTexturedRectState {
    unsigned char pad_00[0x8];
    unsigned char colorKeyEnabled;
    unsigned char pad_09[0x17];
    int mode;
} SwTexturedRectState;

/* Z-buffer descriptor: storage pointer, fill value, byte count and an enable flag. */
typedef struct SwZBuffer {
    unsigned char pad_00[0x8];
    int clearValue;
    unsigned char pad_0C[0x4];
    int byteCount;
    unsigned char pad_14[0x4];
    int enabled;
    int buffer;
} SwZBuffer;

/* Three-short PSX vertex read by TransformVertexSlot3. */
typedef struct GfxPsxShortVertexSlot {
    short x;
    short y;
    short z;
} GfxPsxShortVertexSlot;

/* Stack frame object used during graphics device init, reading two object pointers at offset 0xA0. */
typedef struct GfxInitContext {
    unsigned char pad_00[0x18];
    int hardwareDesc;
    int softwareDesc;
} GfxInitContext;

/* 32-byte mesh record holding a record reference and an offset used by the group draw. */
typedef struct GfxMeshAttribRecordA {
    unsigned char pad_00[0x10];
    int recRef;
    unsigned char pad_14[0x4];
    int texU;
    int offset;
} GfxMeshAttribRecordA;

/* 32-byte mesh record holding an attribute offset and value stride used by the group draw. */
typedef struct GfxMeshAttribRecordB {
    unsigned char pad_00[0x10];
    int attrOffset;
    unsigned char pad_14[0x4];
    int valueStride;
    int texV;
} GfxMeshAttribRecordB;

/* 32-byte mesh record holding a value reference used by the group draw. */
typedef struct GfxMeshAttribRecordC {
    unsigned char pad_00[0x10];
    int valueRef;
    unsigned char pad_14[0x4];
    int texU;
    int cValueRec;
} GfxMeshAttribRecordC;

/* 18-byte face record whose three vertex indices are copied into the compiled index buffer. */
typedef struct GfxFaceRecord {
    unsigned char pad_00[0x2];
    short index0;
    short index1;
    short index2;
} GfxFaceRecord;

/* A 2D batch vertex's screen position (GfxBatchPrimitive.positions); the writers store their
 * `depth` argument's bits as z. */
typedef struct GfxBatchPosition {
    float x;
    float y;
    union {
        float z;
        int depthBits; /* the writers' `depth` argument, stored as z's bits */
    };
} GfxBatchPosition;

/* A 2D batch vertex's texture coordinates (GfxBatchPrimitive.texCoords). */
typedef struct GfxBatchTexCoord {
    union {
        float u;
        int uBits; /* the textured triangle writer's texU argument, stored as u's bits */
    };
    union {
        float v;
        int vBits;
    };
} GfxBatchTexCoord;

/* Batch primitive block allocated from a heap (Gfx_AcquireBatchPrimitive), storing the render object
 * it was acquired for and, from +0xC, the four vertices of a 2D rectangle or triangle as arrays:
 * the Gfx_Emit*Rect/Triangle writers fill them and Gfx_EmitPolygonSetPrimitive copies each vertex
 * into a D3DTLVERTEX (`rhw` is the TLVERTEX's rhw word: 1.0f for the untextured writers, the
 * textured ones' `specular` argument). */
typedef struct GfxBatchPrimitive {
    int renderObject;
    int renderKind;
    int clearedGroup;
    GfxBatchPosition positions[4]; /* +0x0C */
    GfxBatchTexCoord texCoords[4]; /* +0x3C */
    int colors[4]; /* +0x5C */
    int rhw[4]; /* +0x6C */
    unsigned char pad_7C[0x4];
    int clutRow; /* +0x80: the CLUT row of the effect sprite draws (BattleFx_SubmitSpriteDraw) */
} GfxBatchPrimitive;

/* Texture page record whose three fields are copied into a render texture info block. */
typedef struct GfxTexturePageInfo {
    unsigned char pad_00[0x30];
    int paletteCount;
    unsigned char pad_34[0x8];
    int pageU;
    int pageV;
} GfxTexturePageInfo;

/* Table of GL texture names with a count and a heap array of names to free. */
typedef struct GlTextureNameTable {
    int count;
    int *names;
} GlTextureNameTable;

/* 32-byte GL vertex record with texture coordinates passed to glTexCoord2f. */
typedef struct GlTexVertexC {
    unsigned char pad_00[0x10];
    int color;
    unsigned char pad_14[0x4];
    float u;
    float v;
} GlTexVertexC;

/* Record initialized with a flag word that gets bit 0x8000 set. */
typedef struct GfxFlaggedRecord {
    int flags;
    int clearedWord;
} GfxFlaggedRecord;

/* Set of three vertex pointers passed to the 16-bit pixel triangle put routine. */
typedef struct GfxTriangleVertexSet {
    int vertexA;
    int vertexB;
    int vertexC;
} GfxTriangleVertexSet;

/* Intermediate raster state for building triangle edge chains. */
typedef struct GfxRasterIntermediates {
    int midA[3];
    int midB[3];
    int midTable[16];
    unsigned char pad_58[0x138];
} GfxRasterIntermediates;

/* Polygon-set group header read by an unused no-op group walk. */
typedef struct GfxGroupWalkHeader {
    unsigned char pad_00[0x4];
    int count;
    int vertices;
    unsigned char pad_0C[0xc];
    int source;
} GfxGroupWalkHeader;

/* Float BGR colour addend applied to vertex or model colours. */
typedef struct GfxFloatColorAddend {
    float blue;
    float green;
    float red;
} GfxFloatColorAddend;

/* Draw pool descriptor: high-water mark and the heap that backs its list nodes. */
typedef struct GfxDrawPoolDesc {
    int highWaterMark;
    int heap;
} GfxDrawPoolDesc;

/* Light record whose direction vector at 0x08 to 0x10 feeds a cached direction matrix. */
typedef struct GfxLightSource2 {
    unsigned char pad_00[0x8];
    int dirX;
    int dirY;
    int dirZ;
    int colorB;
    int colorG;
    int colorR;
} GfxLightSource2;

/* Ambient colour triple copied into a light matrix. */
typedef struct GfxAmbientColor {
    int ambientFirst;
    int ambientSecond;
    int ambientThird;
} GfxAmbientColor;

/* Light matrix filled from an ambient triple in the transposed layout. */
typedef struct GfxLightMatrixTransposed {
    unsigned char pad_00[0xc];
    int ambientThird;
    unsigned char pad_10[0xc];
    int ambientSecond;
    unsigned char pad_20[0xc];
    int ambientFirst;
    int pad_30;
    int pad_34;
    int pad_38;
    int pad_3C;
} GfxLightMatrixTransposed;

/* Three-byte RGB colour copied into the PSX shaded-vertex colour globals. */
typedef struct GfxPsxRgbColorIn {
    char red;
    char green;
    char blue;
} GfxPsxRgbColorIn;

/* Position entry (0xC bytes) of the exec buffer, copied into GfxTlVertex x, y, z. */
typedef struct GfxExecPosition {
    int x;
    int y;
    int z;
} GfxExecPosition;

/* Finish-node descriptor holding two indices into the scale table. */
typedef struct SwFinishNodeDesc {
    unsigned char pad_00[0x3c];
    int scaleIndexX;
    int scaleIndexY;
} SwFinishNodeDesc;

/* Flat-shaded triangle vertex with a depth and an ARGB colour. */
typedef struct SwFlatTriangleVertex {
    unsigned char pad_00[0x8];
    float depth;
    unsigned char pad_0C[0x48];
    int argbColor;
} SwFlatTriangleVertex;

/* Triangle vertex whose three colour bytes are summed through a lookup table. */
typedef struct SwColorSumVertexA {
    unsigned char pad_00[0x54];
    unsigned char colorByte0;
    unsigned char colorByte1;
    unsigned char colorByte2;
} SwColorSumVertexA;

/* Vertex whose sort key and second field are copied into an edge pair. */
typedef struct SwEdgePairVertex {
    unsigned char pad_00[0x2c];
    int sortKey;
    unsigned char pad_30[0x4];
    int screenY;
} SwEdgePairVertex;

/* Row table giving the row stride and two row base arrays for texture-mask setup. */
typedef struct SwRowTable {
    unsigned char pad_00[0x10];
    int rowStride;
    unsigned char pad_14[0x20];
    int rowBase0;
    int rowBase1;
} SwRowTable;

/* Gradient matrix whose X scale is applied to the texture gradient in X. */
typedef struct SwGradientMatrixX {
    unsigned char pad_00[0x8];
    float dyCoef;
    float dxCoef;
    float scaleX;
    unsigned char pad_14[0x20];
    float pad_34;
    float pad_38;
    unsigned char pad_3C[0x18];
    float pad_54;
    float pad_58;
} SwGradientMatrixX;

/* Gradient matrix whose Y scale is applied to the texture gradient in Y. */
typedef struct SwGradientMatrixY {
    float dyCoef;
    float dxCoef;
    unsigned char pad_08[0xc];
    float scaleY;
    unsigned char pad_18[0x8];
    float pad_20;
    unsigned char pad_24[0x4];
    float pad_28;
    float pad_2C;
    float pad_30;
    unsigned char pad_34[0xc];
    float pad_40;
    unsigned char pad_44[0x4];
    float pad_48;
    float pad_4C;
    float pad_50;
} SwGradientMatrixY;

/* Raster setup state whose depth seed is built from a vertex. */
typedef struct SwRasterSetupState {
    unsigned char pad_00[0x44];
    int depthFixedLow; /* the 64-bit depth is depthFixedLow with depthSeed as its high word */
    int depthSeed;
    int nextDepthFixedLow;
    int nextDepthFixedHigh;
    int depthCopyLow;
    int depthCopyHigh;
    unsigned char pad_5C[0xe8];
    int depthOffset;
} SwRasterSetupState;

/* The colour-gradient view of the same edge record: for each of three channels, the start and
 * end values as 64-bit fixed point and a copy of the start value. */
typedef struct SwRasterSeedBlock {
    unsigned char pad_00[0x8c];
    int channel1CopyLow;
    int channel1CopyHigh;
    unsigned char pad_94[0x8];
    int channel2CopyLow;
    int channel2CopyHigh;
    unsigned char pad_A4[0x8];
    int channel3CopyLow;
    int channel3CopyHigh;
    unsigned char pad_B4[0x8];
    int channel1StartLow;
    int channel1StartHigh;
    int channel1EndLow;
    int channel1EndHigh;
    int channel2StartLow;
    int channel2StartHigh;
    int channel2EndLow;
    int channel2EndHigh;
    int channel3StartLow;
    int channel3StartHigh;
    int channel3EndLow;
    int channel3EndHigh;
} SwRasterSeedBlock;

/* Raster vertex scale record for depth, U and V on the ZUv path. */
typedef struct SwScaleZUv {
    unsigned char pad_00[0x38];
    float scaleDepth;
    float scaleU;
    float scaleV;
} SwScaleZUv;

/* Raster state record holding a float value, a tag and its integer form. */
typedef struct SwStateFloatTag {
    unsigned char pad_00[0x34];
    float floatValue;
    int tag;
    unsigned char pad_3C[0x10];
    int intValue;
} SwStateFloatTag;

/* Heap record allocated with size 0x50 by the graphics matrix allocator. */
typedef struct GfxSizedRecord {
    int size;
    unsigned char pad_04[0x48];
    int rampSize;
} GfxSizedRecord;

/* Per-group color source record, 0x64 bytes each, inside a batch state. */
typedef struct GfxColorSourceGroup {
    unsigned char pad_00[0x8];
    int pad_08;
    unsigned char pad_0C[0x50];
    char pad_5C;
} GfxColorSourceGroup;

/* Linked list node of a draw group pass, pointing at the next node and its group list. */
typedef struct GfxDrawGroupNode {
    int next;
    int drawGroupList;
} GfxDrawGroupNode;

/* Heap table of the preset polygon sets (Gfx_CreatePresetPolygonSetTable): for each render state
 * switch, the set that draws it enabled and the set that draws it disabled, then the two
 * untextured hundred-state records. */
typedef struct GfxPresetPolygonSetTable {
    unsigned char pad_00[0x2c];
    struct GfxPolygonSet *enabledSets[0x16];  /* +0x2C: indexed by render state */
    struct GfxPolygonSet *disabledSets[0x16]; /* +0x84 */
    int untextured0;                          /* +0xDC */
    int untextured1;
} GfxPresetPolygonSetTable;

/* Batch record inside a render object's polygon set, cleared when emptied. */
typedef struct GfxRenderObjectBatch {
    unsigned char pad_00[0x4];
    int count;
    unsigned char pad_08[0x4];
    int highWater;
} GfxRenderObjectBatch;

/* Primitive status block for a screen sprite, holding a command pointer and a word. */
typedef struct GfxScreenSpriteStatus {
    unsigned char pad_00[0x7c];
    int cmdPtr;
    int wordValue;
} GfxScreenSpriteStatus;

/* Primitive status block for a screen sprite with a word, holding a command cursor. */
typedef struct GfxScreenSpriteWordStatus {
    unsigned char pad_00[0x7c];
    int cmdCursor;
    int wordValue;
} GfxScreenSpriteWordStatus;

/* Primitive data for a sprite quad, holding the quad pointer and a word. */
typedef struct GfxSpriteQuadPrim {
    unsigned char pad_00[0x7c];
    int quadPointer;
    int wordValue;
} GfxSpriteQuadPrim;

/* Textured 2D vertex; the floats at 0x18 and 0x1C are texture coordinates. */
typedef struct GfxVertexTextured {
    unsigned char pad_00[0x10];
    union {
        int packedColor; /* +0x10 */
        unsigned char rgb[4]; /* blue, green, red */
    };
    unsigned char pad_14[0x4];
    float texU;
    float texV;
} GfxVertexTextured;

/* GL config record; init sets its first two words to 1. */
typedef struct GlConfig {
    int frontFaceClockwise;
    int doubleBuffered;
} GlConfig;

/* Triangle vertex whose words at 0x2C and 0x34 are the pixel position passed to PutSurfacePixel16. */
typedef struct GfxPixelVertexA {
    unsigned char pad_00[0x2c];
    int pixelX;
    unsigned char pad_30[0x4];
    int pixelY;
} GfxPixelVertexA;

/* Wireframe mesh header; the word at 0x0C counts the edges drawn. */
typedef struct GfxWireMesh {
    unsigned char pad_00[0xc];
    int edgeCount;
    int secondEdgeCount;
} GfxWireMesh;

/* Raster depth config from callbacks+0x30; byte 0x07 selects the mode, word 0x20 is a depth sample. */
typedef struct GfxRasterDepthConfig {
    unsigned char pad_00[0x7];
    unsigned char mode;
    unsigned char pad_08[0x18];
    int depthSample;
} GfxRasterDepthConfig;

/* Polygon-set group record from Gfx_LockPolygonSetGroup; vertex count and vertex array. */
typedef struct GfxPolygonGroupColor {
    unsigned char pad_00[0x4];
    int vertexCount;
    int vertices;
} GfxPolygonGroupColor;

/* Graphics driver description: a driver type word and the driver DLL name string. */
typedef struct GfxDriverDesc {
    int driverType;
    unsigned char pad_04[0x4];
    int dllName;
} GfxDriverDesc;

/* Nine-dword graphics driver selection and DLL state block (0x24 = 36 bytes, GfxEngineContext +0xA68). */
typedef struct GfxDriverLoadSpec {
    int driverType;
    void *libraryName;
    void *procedureName;
    int setupFlags;
    int flags;
    struct GlContext *glState;
    int driverInstalled;
    int driverLibrary;
    int driverProcedure;
} GfxDriverLoadSpec;
typedef char CheckGfxDriverLoadSpec[(sizeof(GfxDriverLoadSpec) == 0x24) ? 1 : -1];

/* Draw pool: a flag word and a block heap sized for nodeCount nodes of 0x90 bytes. */
typedef struct GfxDrawPool {
    int generation;
    int blockHeap;
} GfxDrawPool;

/* Light record of 0x24 bytes from Gfx_LightAllocRecord: the BGRA colour, the direction and the
 * colour again as four floats (Gfx_UnpackBgraToFloat4). The floats are stored through ints. */
typedef struct GfxLight {
    int clearedFlags;
    int color;
    int dirX;
    int dirY;
    int dirZ;
    int colorB;
    int colorG;
    int colorR;
    int colorA;
} GfxLight;

/* List of light-polygon-set entries: a count and an array of entry words. */
typedef struct GfxLightEntryList {
    int count;
    int entries;
} GfxLightEntryList;

/* PSX CLUT state reached through the owner chain at +0x84; mode switch at 0x38, Y base at 0xE0. */
typedef struct GfxPsxClutState {
    unsigned char pad_00[0x38];
    int mode;
    unsigned char pad_3C[0xa4];
    int clutYBase;
} GfxPsxClutState;

/* The 4 bytes that start each item's sprites in a SHP or SPT file; the sprite count is at +2. */
typedef struct GfxSpriteItemHeader {
    unsigned char pad_00[0x2];
    unsigned short spriteCount;
} GfxSpriteItemHeader;

/* One 0x14-byte sprite of a SHP or SPT item; texture is the page id SPT loading collects. */
typedef struct GfxSpriteRecord {
    unsigned char pad_00[0xC];
    short texture;
    unsigned char pad_0E[0x6];
} GfxSpriteRecord;

/* Item table entry of a SHP or SPT file: the item's header and the sprites that follow it. */
typedef struct GfxSpriteItem {
    GfxSpriteItemHeader *header;
    GfxSpriteRecord *sprites;
} GfxSpriteItem;

/* Shape record Gfx_ShpLoadFile builds: the file's start (its header), the item table and the file buffer. */
typedef struct GfxShpRecord {
    struct GfxShpHeader *header;
    GfxSpriteItem *itemTable;
    int fileData;
} GfxShpRecord;

/* Sprite-set record Gfx_SptLoadRecord builds (0x30 bytes): as GfxShpRecord, then the texture page ids found. */
typedef struct GfxSptRecord {
    struct GfxSptHeader *header;
    GfxSpriteItem *itemTable;
    int fileData;
    int textureCount;
    int textures[4];
    unsigned char pad_20[0x10];
} GfxSptRecord;

/* Surface information from engine+0x930; width and height words are copied to the size output. */
typedef struct SwSurfaceInfo {
    unsigned char pad_00[0x24];
    int width;
    int height;
} SwSurfaceInfo;

/* Finish node reached through rst+0x28; pointer at 0x80 leads to a state word, 0x84 is a descriptor. */
typedef struct SwFinishNode {
    unsigned char pad_00[0x80];
    char *streamState;
    void *descriptor;
} SwFinishNode;

/* Model emit node record at node+0x30; material pointer at 0x9C. */
typedef struct SwEmitModelStateOpaque {
    unsigned char pad_00[0x98];
    int useNodeMatrix;
    void *material;
    float matrix[16]; /* +0xa0: the node's own matrix, used when useNodeMatrix is 0 */
} SwEmitModelStateOpaque;

/* The node a 3D blit begin slot receives: the model state it draws sits at +0x30. */
typedef struct SwEmitModelNode {
    unsigned char pad_00[0x30];
    SwEmitModelStateOpaque *model;
} SwEmitModelNode;

/* Destination of a state-pair copy; words at 0x3C and 0x40. */
typedef struct GfxRasterStatePairOut {
    unsigned char pad_00[0x3c];
    int stateA;
    int stateB;
} GfxRasterStatePairOut;

/* Source of a state-pair copy; words at 0x14 and 0x1C. */
typedef struct GfxRasterStatePairIn {
    unsigned char pad_00[0x14];
    int stateA;
    unsigned char pad_18[0x4];
    int stateB;
} GfxRasterStatePairIn;

/* Vertex texture V source: word at 0x38 scaled, plus offset at 0x58. */
typedef struct GfxRasterVertexTexV {
    unsigned char pad_00[0x20];
    float depthSource;
    unsigned char pad_24[0x4];
    float channelSource1;
    float channelSource2;
    float channelSource3;
    float texUSource;
    float texVSource;
    unsigned char pad_3C[0x4];
    float depthOffset;
    unsigned char pad_44[0x4];
    float axisValue1;
    float axisValue2;
    float axisValue3;
    float texUOffset;
    float texVOffset;
} GfxRasterVertexTexV;

/* Texture mask of a draw group; two indices into a table at offset 0x27B8. */
typedef struct GfxRasterTextureMaskA {
    unsigned char pad_00[0x3c];
    int tableIndexA;
    int tableIndexB;
} GfxRasterTextureMaskA;

/* Draw record from drawGroup+0x30; index count at 0x0C and primitive count at 0x24. */
typedef struct GfxRasterDrawRecordA {
    unsigned char pad_00[0xc];
    int indexCount;
    unsigned char pad_10[0x14];
    int primitiveCount;
} GfxRasterDrawRecordA;

/* Draw record from drawGroup+0x30; index sum at 0x0C and primitive count at 0x24. */
typedef struct GfxRasterDrawRecordB {
    unsigned char pad_00[0xc];
    int indexSum;
    unsigned char pad_10[0x14];
    int primitiveCount;
} GfxRasterDrawRecordB;

/* Indexed depth-mesh record from drawGroup+0x30; index limit at 0x0C and primitive count at 0x24. */
typedef struct GfxRasterDrawRecordC {
    unsigned char pad_00[0xc];
    int indexLimit;
    unsigned char pad_10[0x14];
    int primitiveCount;
} GfxRasterDrawRecordC;

/* Context sub-record holding the U and V scale factors applied to raster vertex texture coordinates. */
typedef struct GfxRasterUvScale {
    unsigned char pad_00[0x3c];
    float texScaleU;
    float texScaleV;
} GfxRasterUvScale;

/* Rect-quad context at engine+0x930+0x38; pointers to the blit tables and rect state. */
typedef struct SwRasterRectContext {
    unsigned char pad_00[0x188];
    int blitTables;
    int rectState;
} SwRasterRectContext;

/* Model table record read by Psx_LoadModelTableFromRecords: flags, a scale value and two model references. */
typedef struct GfxModelRecord {
    unsigned short flags;
    unsigned short scale;
    int modelA;
    int modelB;
} GfxModelRecord;

/* Polygon set block behind GfxPolygonSet.source with vertex and face counts and a raster group pointer. */
typedef struct GfxPolygonData {
    unsigned char pad_00[0xc];
    int vertexCount;
    unsigned char pad_10[0x14];
    int faceCount;
    unsigned char pad_28[0x18];
    int vertexBase;
    unsigned char pad_44[0x28];
    int rasterGroup;
} GfxPolygonData;

/* Cached key of the last clear-rect call, stored at bltFrame.r. */
typedef struct BlitClearRectCache {
    int clearCountdown;
    unsigned char pad_04[0x1c];
    int cacheA;
    int cacheSlot;
    int cacheQ;
    int cacheB;
} BlitClearRectCache;

/* Row-fill parameter record pointed at by bltFrame.renderState in the clear-rect path. */
typedef struct BlitRowFillRecord {
    unsigned char pad_00[0x8];
    int fillFlag;
    int rowStride;
    unsigned char pad_10[0x14];
    int width;
    int pixels;
    unsigned char pad_2C[0x8];
    int fillColor;
} BlitRowFillRecord;

/* Texture coordinate record (tu, tv) indexed by texture index when building the vertex array. */
typedef struct GfxExecTexCoord {
    int tu;
    int tv;
} GfxExecTexCoord;

/* Source vertex whose two words are copied into the raster clip vertices. */
typedef struct GfxRasterSourceVertex {
    unsigned char pad_00[0x18];
    int srcAttr0;
    int srcAttr1;
} GfxRasterSourceVertex;

/* One texture U/V coordinate pair in a vertex source's texCoords array. */
typedef struct GfxTexCoordPair {
    int texU;
    int texV;
} GfxTexCoordPair;

/* SHP file header: two signature bytes checked on load, and the item count. */
typedef struct GfxShpHeader {
    unsigned char magic0;
    unsigned char magic1;
    unsigned char pad_02[0x2];
    unsigned short itemCount;
} GfxShpHeader;

/* SPT file header: two signature bytes checked on load, and the item count. */
typedef struct GfxSptHeader {
    unsigned char magic0;
    unsigned char magic1;
    unsigned char pad_02[0x2];
    int itemCount;
} GfxSptHeader;

/* Direct3D device enumeration record: names and copied hardware and software device descriptors. */
typedef struct GfxD3DDeviceRecord {
    unsigned char pad_00[0x10];
    int name;
    int description;
    int hardwareDesc;
    int softwareDesc;
} GfxD3DDeviceRecord;

/* Surface descriptor whose four words are copied into the graphics context when a surface is locked. */
typedef struct GfxSurfaceLockDesc {
    unsigned char pad_00[0x8];
    int height;
    int width;
    int pitch;
    unsigned char pad_14[0x10];
    int pixels;
} GfxSurfaceLockDesc;

/* Texture file registry frame texture with width and height used for pixel totals. */
typedef struct GfxTextureHeader {
    unsigned char pad_00[0x3c];
    int width;
    int height;
} GfxTextureHeader;

/* Render state table read by D3D5ApplyRenderState through the context pointer at 0x940. */
typedef struct GfxRenderStateTable {
    unsigned char pad_00[0x34];
    int stateValue;
    int stateFallback;
    unsigned char pad_3C[0x8];
    int blendMode; /* the blend mode the override applies */
} GfxRenderStateTable;

/* Model table entry filled from a model record during model table load. */
typedef struct GfxPsxModelTableEntry {
    int model;
    int secondModel;
} GfxPsxModelTableEntry;

/* Four palette-expanded colors for one color record built from byte indices. */
typedef struct GlColorRecord {
    int color0;
    int color1;
    int color2;
    int color3;
} GlColorRecord;

/* Palette source referenced by a GL polygon batch and uploaded with its texture page. */
typedef struct GlPaletteSource {
    unsigned char pad_00[0x10];
    int entryCount;
    unsigned char pad_14[0x4];
    int paletteEntryCount;
} GlPaletteSource;

/* Float copy of one palette entry, 16 bytes per record. */
typedef struct GfxPaletteFloatColor {
    float blue;
    float green;
    float red;
    float alpha;
} GfxPaletteFloatColor;

/* Four-byte palette entry copied from the source palette. */
typedef struct GfxPaletteEntry {
    char colorR;
    char colorG;
    char colorB;
    char flags;
} GfxPaletteEntry;

/* Triangle record whose three shorts are copied into the emitted triangle list. */
typedef struct GfxTriangleRecord {
    unsigned char pad_00[0x2];
    short vertexIndexA;
    short vertexIndexB;
    short vertexIndexC;
    unsigned char pad_08[0x10];
} GfxTriangleRecord;

/* Polygon set header whose part count and geometry pointer the part-pair walk reads. */
typedef struct GfxPolygonSetPart {
    unsigned char pad_00[0x4];
    int partCount;
    int geometry;
} GfxPolygonSetPart;

/* Second polygon set header of a pair, compared by part count and read for a vector. */
typedef struct GfxPolygonSetPairTarget {
    unsigned char pad_00[0x4];
    int partCount;
    int pairedPart;
    unsigned char pad_0C[0x8];
    int vector;
} GfxPolygonSetPairTarget;

/* Translation (three floats) added to a transformed point through the context pointer at +0x2FC. */
typedef struct GfxWorldOffset {
    unsigned char pad_00[0x30];
    float offsetX;
    float offsetY;
    float offsetZ;
} GfxWorldOffset;

/* 12-byte vertex source entry supplying rhw, color and specular for D3D exec vertices. */
typedef struct GfxExecVertexVector {
    int rhw;
    int color;
    int specular;
} GfxExecVertexVector;

/* 12-byte table entry whose int fields are returned by the table field accessors. */
typedef struct GfxTableEntry {
    unsigned char pad_00[0x4];
    int fieldAt4;
    int fieldAt8;
} GfxTableEntry;

/* 12-byte vertex source entry copied into the vertex array destination. */
typedef struct GfxVertexSourceEntry {
    int reciprocalW;
    int vertexColor;
    int specularColor;
} GfxVertexSourceEntry;

/* Four-byte palette entry; one byte is saved, cleared and restored during palette upload. */
typedef struct GlPaletteEntry {
    unsigned char pad_00[0x3];
    char alpha;
} GlPaletteEntry;

/* DirectDraw driver record whose two heap pointers are freed on release. */
typedef struct GfxDdDriverRecord {
    unsigned char pad_00[0x10];
    int driverName;
    int driverDescription;
} GfxDdDriverRecord;

/* Device info record; its count gates the mode setup and a table pointer is read at +4. */
typedef struct GfxDeviceInfo {
    unsigned char pad_00[0x8];
    int count;
    int driverRecord;
} GfxDeviceInfo;

/* Textured hundred record that stores its kind and an upload flag. */
typedef struct GfxTexturedHundredRecord {
    unsigned char pad_00[0x10];
    int kind;
    int uploaded;
} GfxTexturedHundredRecord;

/* Texture object whose flag byte is set to 1 on allocation. */
typedef struct GfxTextureObject {
    unsigned char pad_00[0xcc];
    char flag;
    unsigned char pad_CD[0x3];
    int paletteRow;
} GfxTextureObject;

/* The texture coordinates of one default quad corner, u and v as float bits (0.0 or 1.0). */
typedef struct GfxDefaultQuadTexCoord {
    int u;
    int v;
} GfxDefaultQuadTexCoord;

/* Mesh edge record for wire drawing: its two vertex indices. */
typedef struct GfxRasterWireEdge {
    unsigned char pad_00[0x14];
    int startIndex;
    int endIndex;
    unsigned char pad_1C[0x8];
} GfxRasterWireEdge;

/* A 32-byte source vertex of the software rasterizer: position, w, packed BGRA colour, u and v. */
typedef struct SwRasterSourceVertex {
    union {
        float x;
        int xBits;
    };
    union {
        float y;
        int yBits;
    };
    union {
        float z;
        int zBits;
    };
    union {
        float w;
        int wBits;
    };
    int packedColor;
    int attr14; /* +0x14: copied to the vertex's packedColor, which is then overwritten */
    float u;
    float v;
} SwRasterSourceVertex;

/* The software rasterizer's data block as the vertex resolvers read it: the scale object at
 * +0x18C and the table of (float)i for i = 0..640 at +0x27B8, then 1/i (sw.cpp fills both). */
typedef struct SwRasterData {
    unsigned char pad_000[0x18C];
    struct SwScaleZUv *scale;
    unsigned char pad_190[0x2628];
    float intToFloat[641];
    float reciprocal[641];
} SwRasterData;

/* The engine context's callbacks as the window procedure and the main loop call them (directx.cpp,
 * graphics_app_entry_point.cpp, graphics_driver_context_fields.cpp): every one gets the context, as an
 * int, as its last argument. A module's enter, exit and frame functions and the quit dialog take
 * only the context; the mouse and key handlers get the window message, its wParam and lParam. */
typedef void (__cdecl *GfxModeCallback)(int ctx);

/* The graphics engine context (0xAA0 bytes, AllocEngineContext; GetEngineContext returns it): the
 * window, the DirectDraw and Direct3D objects and surfaces, the driver, the render resources and
 * the module callbacks. */
typedef struct GfxEngineContext {
    int quitFlag; /* +0x000 */
    int desktopWidth; /* +0x004: GetDeviceCaps HORZRES of the window's DC (CreateGameWindow) */
    int desktopHeight; /* +0x008: VERTRES */
    int desktopBitsPerPixel; /* +0x00C: BITSPIXEL; colorDepth takes it when it is 0 */
    int windowX; /* +0x010: WM_MOVE's position (GameWindowProc) */
    int windowY; /* +0x014 */
    int clientWidth; /* +0x018: WM_SIZE's size; GfxLockSurface locks this much of a window */
    int clientHeight; /* +0x01C */
    int minimized; /* +0x020 */
    unsigned char pad_24[0x4];
    double frameCount; /* +0x028 */
    double tscFrequency; /* +0x030 */
    Timestamp64 tscCalibration; /* +0x038: Timer_CalibrateTscFrequency's reading */
    double framesThisSecond; /* +0x040: the frames counted since the last second (UpdateFrameTiming) */
    double fps; /* +0x048: the last second's count */
    Timestamp64 secondStart; /* +0x050: the timestamp the second started at */
    int hInstance; /* +0x058 */
    int hwnd; /* +0x05C */
    int result; /* +0x060 */
    int deviceReady; /* +0x064: InitGraphicsDevice's result (0 after ShutdownGraphicsDevice) */
    int ddDriverList; /* +0x068: the DirectDraw drivers DDEnumDriversCallback found */
    int displayModeCount; /* +0x06C: the display modes DDEnumDisplayModesCallback saw */
    int matchingDisplayModes; /* +0x070: those of displayWidth x displayHeight x colorDepth */
    int ddObject; /* +0x074: IDirectDraw of the chosen driver */
    int ddObject2; /* +0x078: its IDirectDraw2 */
    union {
        struct {
            int primarySurface; /* +0x07C: the surfaces, indexed by buffer: the primary surface ... */
            int backSurface; /* +0x080: ... and up to two back buffers (bufferCount) */
            int backSurface2; /* +0x084 */
        };
        int surfaces[3];
    };
    GfxTextureSurfaceDesc surfaceDescs[3]; /* +0x088: each surface's DDSURFACEDESC */
    int zBufferSurface; /* +0x1CC: the DirectDraw Z buffer the init creates */
    int unusedComSurface; /* +0x1D0: a COM object ShutdownGraphicsDevice releases; nothing in src/ sets it */
    int ddClipper; /* +0x1D4: the windowed mode's clipper (CreateClipper, SetHWnd) */
    int displayModeSaved; /* +0x1D8: savedDisplayMode holds the mode to restore at shutdown */
    GfxTextureSurfaceDesc savedDisplayMode; /* +0x1DC: GetDisplayMode before the full-screen switch */
    int displayDdObject; /* +0x248: a DirectDraw object of the primary display, when the chosen
                          * driver is not it (modeActive 0): it sets the display mode */
    int displayDdModeSaved; /* +0x24C */
    GfxTextureSurfaceDesc displayDdSavedMode; /* +0x250: that display's mode before the switch */
    int d3dDeviceList; /* +0x2BC: the Direct3D devices D3DEnumDevicesCallback found */
    int direct3D; /* +0x2C0 */
    int d3dDevice; /* +0x2C4: IDirect3DDevice (matrices, execute buffers) */
    int textureFormatList; /* +0x2C8 */
    int d3dDeviceRecord; /* +0x2CC: the device D3DPickDeviceCallback picked */
    int viewport; /* +0x2D0: IDirect3DViewport */
    int backgroundMaterial; /* +0x2D4: IDirect3DMaterial */
    unsigned char pad_2D8[0x4];
    int direct3D2; /* +0x2DC: IDirect3D2 (D3DQueryIDirect3D2) */
    int d3dDevice2; /* +0x2E0 */
    int viewport2; /* +0x2E4: IDirect3DViewport2 (useDirect3D5) */
    int polygonSetList; /* +0x2E8 */
    struct GfxPolygonSet *opaqueSet; /* +0x2EC: the device's opaque and translucent polygon sets; a scene pass */
    struct GfxPolygonSet *transSet; /* +0x2F0: enables one of them (word 0) and draws it */
    int matrixStack; /* +0x2F4 */
    int matrixStack2; /* +0x2F8: a second stack of matrixStackSize identity matrices */
    int cameraMatrix; /* +0x2FC: the view matrix the draws concatenate (an identity at init) */
    int drawPool; /* +0x300: Gfx_CreateDrawPool(0x200); Gfx_AdvanceDrawPoolGeneration each frame */
    unsigned char pad_304[0x4];
    int triAlphaShadeCaps; /* +0x308 */
    unsigned char pad_30C[0x18];
    GfxD3DDeviceDesc hwDeviceDesc; /* +0x324: HAL D3DDEVICEDESC (0xCC bytes) */
    GfxD3DDeviceDesc swDeviceDesc; /* +0x3F0: HEL D3DDEVICEDESC (0xCC bytes) */
    union {
        GfxDirectDrawDriverCaps driverCapsHw; /* +0x4BC: HAL DDCAPS (0x16C bytes) */
        struct {
            unsigned char pad_4BC[0x4];
            int blitFlags; /* +0x4C0: driverCapsHw.caps */
            unsigned char pad_4C4[0x7c];
            int ddHwSurfaceCaps; /* +0x540: driverCapsHw.ddsCaps (DDSCAPS_VIDEOMEMORY) */
            unsigned char pad_544[0xe4];
        };
    };
    GfxDirectDrawDriverCaps driverCapsHel; /* +0x628: HEL DDCAPS (0x16C bytes) */
    struct GfxStatsRecord *statsRecord; /* +0x794 */
    int frameStatsRecord; /* +0x798: zeroed at init; ShutdownGraphicsDevice frees both */
    GfxPixelFormat targetFormat; /* +0x79C: the render target's; GfxLockSurface stores the locked
                                 * surface's size, pitch and memory and, with its pixel format
                                 * flag set, the format (Gl_PixelFormatFromPfd on GL) */
    int sceneActive; /* +0x81C */
    int presetHundreds[5]; /* +0x820: the five preset hundred-state records (Gfx_BuildPresetHundreds) */
    int activeA; /* +0x834 */
    int activeB; /* +0x838 */
    int debugLogTarget; /* +0x83C */
    int cursorX; /* +0x840 */
    int cursorY; /* +0x844 */
    int clipLeft; /* +0x848 */
    int clipTop; /* +0x84C */
    int viewSizeX; /* +0x850 */
    int viewSizeY; /* +0x854 */
    int clipRight; /* +0x858 */
    int clipBottom; /* +0x85C */
    int viewportHeightOverride; /* +0x860 */
    int blitRectsEnabled; /* +0x864 */
    SwBlitRectEntry blitRects[2]; /* +0x868: the dirty rectangles presented while blitRectsEnabled is set */
    unsigned char screenTransform[0x40]; /* +0x890: the screen transform matrix (BuildScreenTransformMatrix) */
    unsigned char screenTransformTransposed[0x40]; /* +0x8D0: screenTransform transposed (Gfx_RebuildContextScreenTransform) */
    int renderObjectBank; /* +0x910 */
    int fileRegistry; /* +0x914 */
    int renderResourceTable; /* +0x918 */
    int sortedDrawEnabled; /* +0x91C */
    struct GfxSortedDrawQueue *depthSortQueue; /* +0x920 */
    int moduleSwitchPending; /* +0x924 */
    int scenePass; /* +0x928 */
    int renderStateApplied; /* +0x92C: D3D5ApplyRenderState and the GL driver's skip a state twice */
    struct SwRasterizerState *rasterBase; /* +0x930 */
    struct GfxDriverOps *driverObject; /* +0x934 */
    int nodeList; /* +0x938 */
    int overrideActive; /* +0x93C: overrideRenderState replaces the draws' blend states */
    int overrideRenderState; /* +0x940: one of the five states at +0x820 */
    int presetPolygonSets; /* +0x944: Gfx_CreatePresetPolygonSetTable's table */
    int deviceIndex; /* +0x948 */
    int alphaOption; /* +0x94C: the config file's "alpha" option */
    int paused; /* +0x950 */
    int displayWidth; /* +0x954: the display mode (SetDisplayMode) and the window's size: 320 or 640 */
    int displayHeight; /* +0x958: 240 or 480; Input_Update clamps the cursor to both */
    int colorDepth; /* +0x95C */
    int bufferCount; /* +0x960 */
    int fullScreen; /* +0x964 */
    int modeActive; /* +0x968 */
    int modeTransition; /* +0x96C */
    int backBuffersInSystemMemory; /* +0x970: the windowed mode's back buffers in system memory */
    int showCursor; /* +0x974: CreateGameWindow hides the cursor when it is 0 */
    int ddSurface; /* +0x978 */
    int use3DDevice; /* +0x97C: the surfaces take DDSCAPS_3DDEVICE and a Direct3D device and Z buffer
                       * are created (1 by default; the software and GL drivers clear it) */
    unsigned char pad_980[0x4];
    int useDirect3D5; /* +0x984: the Direct3D 5 path (DrawPrimitive batches, viewport2) instead of
                       * execute buffers (GfxBindDriverOpsD3D, D3DViewportClear) */
    int modeSetupDone; /* +0x988 */
    int zBufferEnabled; /* +0x98C: create and clear a Z buffer */
    int zBufferDepth; /* +0x990 */
    int modeLoaded; /* +0x994 */
    int matrixStackSize; /* +0x998 */
    float fov; /* +0x99C */
    float nearPlane; /* +0x9A0 */
    float farPlane; /* +0x9A4 */
    float offsetX; /* +0x9A8 */
    float offsetY; /* +0x9AC */
    int randomSeed; /* +0x9B0 */
    char *windowTitle; /* +0x9B4 */
    char *windowClassName; /* +0x9B8 */
    int customWindowClass; /* +0x9BC: register windowClass instead of the default class */
    int windowClass[10]; /* +0x9C0: a WNDCLASS */
    int customWindowStyle; /* +0x9E8: create the window with windowStyle */
    int windowStyle; /* +0x9EC */
    GameModule activeModule; /* +0x9F0: the running module */
    GameModule pendingModule; /* +0xA0C: the module RequestModuleSwitch staged */
    void (__cdecl *onActivateApp)(int active, int ctx); /* +0xA28 */
    void (__cdecl *onDeviceChange)(int event, int deviceData, int ctx); /* +0xA2C */
    int configPending; /* +0xA30 */
    int rampTableR; /* +0xA34 */
    int rampTableG; /* +0xA38 */
    int rampTableB; /* +0xA3C */
    unsigned char pad_A40[0x4];
    int rampBlock; /* +0xA44 */
    int presentEnable; /* +0xA48 */
    unsigned char pad_A4C[0x8];
    int pendingWindowEvent; /* +0xA54 */
    GfxModeCallback quitConfirmRun; /* +0xA58 */
    int (__cdecl *allowCtrlQQuit)(int ctx); /* +0xA5C */
    int driverType; /* +0xA60 */
    int driverName; /* +0xA64 */
    unsigned char pad_A68[0xc];
    int setupFlags; /* +0xA74 */
    int flags; /* +0xA78 */
    struct GlContext *glState; /* +0xA7C: the GL driver's context (Gl_GetState); null until Gl_OpsInit makes it */
    int driverInstalled; /* +0xA80 */
    int driverLibrary; /* +0xA84 */
    int driverProcedure; /* +0xA88 */
    int driverExportTable; /* +0xA8C */
    int fallbackFlag; /* +0xA90 */
    int configApplied; /* +0xA94 */
    int fallbackUsed; /* +0xA98 */
} GfxEngineContext;
typedef char CheckGfxEngineContext[(sizeof(GfxEngineContext) == 0xAA0) ? 1 : -1];

/* Raster clip scratch record: plane mask, three vertex pointers and the output vertex list. */
typedef struct GfxRasterClipScratch {
    int maxPlane;
    float planeThreshold[6]; /* +0x04: the coordinate each clip plane compares against */
    int vertexA; /* +0x1C: the input list for the first plane is vertexA..vertexC */
    int vertexB;
    int vertexC;
    unsigned char pad_28[0x4];
    int planeLists[6][8]; /* +0x2C: the vertex pointers each plane's clip pass outputs */
    GfxRasterClipVertex planeVertices[6][8]; /* +0xEC: the vertices each plane's pass creates at an intersection */
    int vertCount;
    int destinationList;
} GfxRasterClipScratch;

/* Three palette-index bytes that select the vertex colours of an RGB record. */
typedef struct SwRasterRgbPaletteIndices {
    unsigned char colorIndex0;
    unsigned char colorIndex1;
    unsigned char colorIndex2;
} SwRasterRgbPaletteIndices;

/* Global GTE emulation state: rotation matrix, geometry and projection offsets, draw and secondary colour. */
typedef struct {
    float projectionScale;
    unsigned char pad_04[0x4];
    double projectionDistance;
    PsxMatrix rotMatrix; /* +0x10, the installed rotation and translation */
    unsigned char pad_2E[0x2];
    int geomOffsetX;
    int geomOffsetY;
    float screenOffsetX;
    float screenOffsetY;
    int projOffsetX;
    int projOffsetY;
    int matrixLayoutMode;
    char secondaryColorB;
    char secondaryColorG;
    char secondaryColorR;
    char secondaryColorA;
    union {
        int drawColorPacked;
        struct {
            char drawColorB;
            char drawColorG;
            char drawColorR;
            char drawColorA;
        };
    };
    float lastNdcZ;
    float lastNdcW;
} GraphicsGteState;

/* 4-byte palette source entry, converted to GfxPaletteEntry and float colours. */
typedef struct GfxPaletteSourceEntry {
    char blue;
    char green;
    char red;
    unsigned char alpha;
} GfxPaletteSourceEntry;

/* Destination texel for Gl_ConvertTexturePixels: blue, green and red bytes. */
typedef struct GlTexelDest {
    char blue;
    char green;
    char red;
} GlTexelDest;

/* Source texel for Gfx_ConvertTexturePixelsToSurface: blue, green and red bytes. */
typedef struct GfxTexelSource {
    unsigned char blue;
    char green;
    char red;
} GfxTexelSource;

/* Output vertex written by DxMesh_TransformAndDrawGroup, in 0x20-byte slots: clip-space position and
 * inverse-width term, or -1.0f in every word (the Bits spellings) when the point is behind the eye. */
typedef struct GfxMeshVertex {
    union {
        float clipX;
        int clipXBits;
    };
    union {
        float clipY;
        int clipYBits;
    };
    union {
        int clipZ;
        float depth;
    };
    union {
        float invW;
        int invWBits;
    };
    unsigned char pad_10[0x10];
} GfxMeshVertex;

/* Draw parameters for a transformed mesh group: x, y, z offsets and a mode switch. */
typedef struct GfxMeshDrawParams {
    unsigned char pad_00[0x6];
    unsigned char offsetEnabled;
    char mode;
    unsigned char pad_08[0xc];
    float offsetX;
    float offsetY;
    float offsetZ;
    float depthBias;
} GfxMeshDrawParams;

/* Software blitter render state: Z-buffer, heap block and row byte stride. */
typedef struct SwRasterizerState {
    unsigned char pad_00[0x8];
    int allocatePixelBlock; /* +0x08: the creation desc's (Blit_CreateRenderState copies its first 0x24 bytes) */
    unsigned char pad_0C[0x4];
    int usesAlignedHeap;
    unsigned char pad_14[0x8];
    float zScale;
    unsigned char pad_20[0x4];
    int rowBytes;
    int block;
    int alignedBase;
    int heapBlock;
    int zBuffer;
    unsigned char pad_38[0x188];
    int setupBlock;
    int vertexBuffer;
    unsigned char pad_1c8[0x25e8];
    float modelMatrix[16]; /* +0x27b0: the 3D blit begin slots' node matrix times the screen transform */
    unsigned char pad_27f0[0x5210c];
    int rasterVertexBuffer; /* +0x548fc */
    unsigned char pad_54900[0x160];
    union {
        int renderSetup; /* the raster setup block's address */
        struct GfxRasterSetupBlock *renderSetupBlock;
    };
    int halfScaleLut; /* +0x54a64 */
    int doubleBrightLut; /* +0x54a68 */
    int quarterScaleLut; /* +0x54a6c */
    int threeQuarterScaleLut; /* +0x54a70 */
} SwRasterizerState;

/* GL polygon set: count, batch and record arrays, and a state blob used by Gl_OpsDrawPolygonSet. */
typedef struct GlPolygonSet {
    int drawEnabled; /* GfxPolygonSet's +0x00 */
    int drawUnsorted; /* +0x04 */
    unsigned char pad_08[0x8];
    int count;
    unsigned char pad_14[0x18];
    int useSharedGroup;
    int linkedObject;
    int sharedDrawGroup;
    int usePerEntryGroups;
    int entries;
    int polygonRecord;
    unsigned char pad_44[0xc];
    int batches;
    unsigned char pad_54[0x40];
    int hasStateBlob;
    unsigned char pad_98[0x4];
    int stateBlob;
} GlPolygonSet;

/* Screen-space gradients written by the software triangle setup, including the depth gradients. */
typedef struct SwTriangleGradients {
    float deltaBa;
    float deltaCa;
    float scaleC;
    float scaleB;
    float invDenominator;
    float invDenominatorNeg;
    unsigned char pad_18[0x8];
    /* +0x20: d/dx and, from +0x40, d/dy of the depth, the colour channels and u/v; the
       interpolators of sw_gfx_raster_vertex_snap.c add tx * d/dx + ty * d/dy to the previous
       vertex's value */
    float depthGradX;
    unsigned char pad_24[0x4];
    float redGradX;
    float greenGradX;
    float blueGradX;
    float uGradX;
    float vGradX;
    unsigned char pad_3C[0x4];
    float depthGradY;
    unsigned char pad_44[0x4];
    float redGradY;
    float greenGradY;
    float blueGradY;
    float uGradY;
    float vGradY;
} SwTriangleGradients;

/* Blit target table: default integer settings, a mapped color and the triangle draw function pointer. */
typedef struct SwBlitTargetTable {
    int defaultMode00;
    int defaultMode04;
    int defaultMode08;
    int defaultMode0C;
    int defaultMode10;
    int defaultMode14;
    int defaultMode18;
    int defaultMode1C;
    int defaultMode20;
    unsigned char pad_24[0x4];
    int defaultMode28;
    int defaultMode2C;
    int defaultMode30;
    unsigned char pad_34[0x4];
    int defaultMode38;
    int defaultMode3C;
    int defaultMode40;
    unsigned char pad_44[0xac];
    int mappedColor;
    unsigned char pad_F4[0x8c];
    int drawTriangle;
    unsigned char pad_184[0x2634];
    float indexAsFloat[641]; /* [i] = (float)i */
    float indexReciprocal[641]; /* [i] = 1.0f / i, from 1 */
    unsigned char pad_3BC0[0xa04];
    char thirds[768]; /* [i] = i / 3 */
    int argbOfPixel[65536]; /* 16-bit pixel to ARGB8888 through the target format */
    char channelProduct[65536]; /* [(inner << 0) + (outer << 8)] = inner * outer / 255 */
} SwBlitTargetTable;

/* D3D viewport record built by BuildD3DViewportData: size 44, origin and extent, then float clip values. */
typedef struct GfxD3DScaledViewport {
    int dwSize;
    int dwX;
    int dwY;
    int dwWidth;
    int dwHeight;
    float scaleX;
    float scaleY;
    int maxX;
    int maxY;
    int dvMinZ;
    int dvMaxZ;
} GfxD3DScaledViewport;

/* Polygon-set load spec read by GfxLoadPolygonSet. */
typedef struct GfxPolygonSetSpec {
    int flags;
    unsigned char pad_04[0xc];
    int polygonFlags;
    unsigned char pad_14[0x4];
    int drawPoolGeneration;
    int optionBits;
    int blendMode;
    unsigned char pad_24[0xc];
    int lightGroup;
    unsigned char pad_34[0x3c];
    int renderFlags;
} GfxPolygonSetSpec;

/* Table of colour rows (row count, row length, base pointers) filled by Gfx_FillColorRowBytes. */
typedef struct GfxColorRowTable {
    unsigned char pad_00[0xc];
    int rowCount;
    int rowLength;
    unsigned char pad_14[0x4];
    int dstBase;
    unsigned char pad_1C[0x8];
    int floatBase;
    int srcBase;
} GfxColorRowTable;

/* Specular colour state: a depth-driven cached colour clamped between depthMin and depthMax. */
typedef struct GfxSpecularState {
    unsigned char pad_00[0x17c];
    int mode;
    int defaultColor;
    float depth;
    int cachedColor;
    float depthMin;
    float depthMax;
    unsigned char pad_194[0xc];
    float depthScale;
    int colorTable[1]; /* +0x1A4: the colours Gfx_UpdateCachedSpecularColor picks by depth */
} GfxSpecularState;

/* Per-vertex colour bytes: lit diffuse channels clamped to 0..255, with alpha copied from the source. */
typedef struct GfxVertexColor {
    char colorR;
    char colorG;
    char colorB;
    char colorA;
} GfxVertexColor;

/* Options for allocating a textured hundred: a flags word and a texture object pointer. */
typedef struct GfxTexturedHundredOptions {
    int flags;
    unsigned char pad_04[0x1c];
    int blendMode;
    unsigned char pad_24[0x4];
    int texture;
    unsigned char pad_2C[0x2c];
    int paletteFlags;
    int paletteRow;
} GfxTexturedHundredOptions;

/* Triangle group whose vertex range, vertex count and texture flags drive the vertex array build. */
typedef struct GfxExecTriangleGroup {
    unsigned char pad_00[0xc];
    int firstVertex;
    int vertexCount;
    unsigned char pad_14[0x18];
    int textureStart;
    int hasTextureCoords;
} GfxExecTriangleGroup;

/* Vertex operation record written by D3DExecCompileBuffer with a vertex format and count. */
typedef struct GfxExecVertexOp {
    int vertexFormat;
    short startIndex;
    short destIndex;
    int vertexCount;
    int reserved;
} GfxExecVertexOp;

/* Per-run record for drawing textured 2D sprites: counts, vertex position, page and run info. */
typedef struct GfxDraw2DTexturedRecord {
    unsigned char pad_00[0x8];
    int leftCount;
    unsigned char pad_0C[0x8];
    int vertexPos;
    unsigned char pad_18[0x8];
    int indices;
    unsigned char pad_24[0x4];
    unsigned char *pagePtr;
    unsigned char pad_2C[0x4];
    struct GfxTexturedRunInfo *runInfo;
} GfxDraw2DTexturedRecord;

/* Per-run record for drawing textured 3D sprites: sprite count, vertex offset, page and run info. */
typedef struct GfxDraw3DTexturedRecord {
    unsigned char pad_00[0x8];
    int spritesToDo;
    unsigned char pad_0C[0x8];
    int vertexOffset;
    unsigned char pad_18[0x8];
    int drawFlags;
    unsigned char pad_24[0x4];
    unsigned char *pagePtr;
    unsigned char pad_2C[0x4];
    struct Gfx3DTexturedInfo *runInfo;
} GfxDraw3DTexturedRecord;

/* Depth-dependent specular state: a specular value, near and far depth thresholds and a scale. */
typedef struct GfxSpecularDepthState {
    unsigned char pad_00[0x180];
    int specularValue;
    unsigned char pad_184[0x4];
    int cachedSpecular;
    float depthLow;
    float depthHigh;
    unsigned char pad_194[0x8];
    int matrix;
    float depthScale;
} GfxSpecularDepthState;

/* Saved color entry with two float fields and three masked color bytes (blue, green, red). */
typedef struct GfxSavedColorRecord {
    unsigned char pad_00[0x8];
    float greenFloat;
    float redFloat;
    unsigned char pad_10[0x4];
    unsigned char blue;
    char green;
    char red;
} GfxSavedColorRecord;

/* Input vertex of three shorts (x, y, z) for the PSX-style vertex transform. */
typedef struct GfxPsxInputVertex {
    short x;
    short y;
    int z;
} GfxPsxInputVertex;

/* Source descriptor for the 3D textured blit: primitive count, vertex cursor, page and descriptor. */
typedef struct BlitDraw3DTexturedSource {
    unsigned char pad_00[0x8];
    int primitiveCount;
    unsigned char pad_0C[0x8];
    int vertexCursor;
    unsigned char pad_18[0x8];
    int indices;
    unsigned char pad_24[0x4];
    int pagePtr;
    unsigned char pad_2C[0x4];
    int descriptor;
} BlitDraw3DTexturedSource;

/* Three vertex indices copied from a triangle record, plus a constant 0x700 flag word. */
typedef struct GfxTriangleIndexRecord {
    short vertexIndexA;
    short vertexIndexB;
    short vertexIndexC;
    short flags;
} GfxTriangleIndexRecord;

/* Batch record holding a texture page and a sorted primitive batch. */
typedef struct GfxSortedBatchRecord {
    unsigned char pad_00[0x10];
    int texturePage;
    unsigned char pad_14[0x50];
    struct GfxRenderObject *batch;
    int primData;
    int indices;
} GfxSortedBatchRecord;

/* 4x4 float matrix whose fourth column ClearMatrixColumn4 sets to 0, 0, 0, 1. */
typedef struct GfxMatrixColumn4 {
    unsigned char pad_00[0xc];
    int m03;
    unsigned char pad_10[0xc];
    int m13;
    unsigned char pad_20[0xc];
    int m23;
    unsigned char pad_30[0xc];
    int m33;
} GfxMatrixColumn4;

/* 4x4 float matrix whose fourth row ClearMatrixRow4 sets to 0, 0, 0, 1. */
typedef struct GfxMatrixRow4 {
    unsigned char pad_00[0x30];
    int m30;
    int m31;
    int m32;
    int m33;
} GfxMatrixRow4;

/* Colour source: a BGRA word then three floats, read by D3DFillMaterialColorFromBgra. */
typedef struct GfxBgraColorSource {
    int bgra;
    float green;
    float red;
    float alpha;
} GfxBgraColorSource;

/* Vector written by PerspectiveDivide: divided x, y, z with w set to 1. */
typedef struct GfxHomogeneousVec43 {
    float x;
    float y;
    float z;
    int w;
} GfxHomogeneousVec43;

/* Homogeneous 4D clip-space vector (16 bytes) with dual float and raw IEEE-754 int bit views. */
typedef struct GfxClipVec4 {
    union { float x; int xBits; };
    union { float y; int yBits; };
    union { float z; int zBits; };
    union { float w; int wBits; };
} GfxClipVec4;

typedef struct GfxVec3Dual {
    union { float x; int xBits; };
    union { float y; int yBits; };
    union { float z; int zBits; };
} GfxVec3Dual;

typedef union GfxMatrix4x4Dual {
    GfxMatrix4x4 f;
    float m[4][4];
    int bits[4][4];
} GfxMatrix4x4Dual;

typedef struct GfxBlitRect {
    int left;
    int top;
    int right;
    int bottom;
} GfxBlitRect;

/* Vector written by PerspectiveDivideRecipW: x, y, z scaled by recipW, which is stored as w. */
typedef struct GfxRecipWVec4 {
    float x;
    float y;
    float z;
    float recipW;
} GfxRecipWVec4;

/* Matrix whose column 2 TransformPointZByTranspose reads for the z component. */
typedef struct GfxMatrixZRowSource {
    unsigned char pad_00[0x8];
    float m02;
    unsigned char pad_0C[0xc];
    float m12;
    unsigned char pad_1C[0xc];
    float m22;
    unsigned char pad_2C[0xc];
    float m32;
} GfxMatrixZRowSource;

/* Matrix column 2 read by TransposeTransformPointZ for the z component. */
typedef struct GfxTransposeZSource {
    unsigned char pad_00[0x8];
    float weightX;
    unsigned char pad_0C[0xc];
    float weightY;
    unsigned char pad_1C[0xc];
    float weightZ;
    unsigned char pad_2C[0xc];
    float offsetZ;
} GfxTransposeZSource;

/* Polygon set read by D3DExecEmitTriangleList: triangle count and triangle record array. */
typedef struct GfxPolySet {
    unsigned char pad_00[0x24];
    int triCount;
    unsigned char pad_28[0x34];
    int triangles;
} GfxPolySet;

/* Vertex group read by D3D5BuildVertexArray: first vertex, vertex count, texture start, UV flag. */
typedef struct GfxVertexGroup {
    unsigned char pad_00[0xc];
    int firstVertex;
    int vertexCount;
    unsigned char pad_14[0x18];
    int textureStart;
    int hasTexCoords;
} GfxVertexGroup;

/* Polygon texture state: texture page state and the destination for the texture handle. */
typedef struct GfxPolygonTexture {
    unsigned char pad_00[0x14];
    int pageState;
    unsigned char pad_18[0x38];
    int handleEnabled;
    unsigned char pad_54[0x4];
    int texHandleDest;
} GfxPolygonTexture;

/* Draw request read by DxRendx_SetStateAndDrawIndexedRetry: draw spec, args reference, base vertex. */
typedef struct GfxIndexedDrawRequest {
    unsigned char pad_00[0x4];
    int clipEnabled;
    unsigned char pad_08[0x2c];
    int drawSpec;
    unsigned char pad_38[0x18];
    int *indexedArgsRef;
    int baseVertex;
} GfxIndexedDrawRequest;

/* Draw record passed to the device invoke call in Gfx_EmitDraw3DUntextured. */
typedef struct GfxDraw3DRecord {
    unsigned char pad_00[0xc];
    int vertexCount;
    unsigned char pad_10[0x4];
    int vertices;
    int indexCount;
    unsigned char pad_1C[0x4];
    int indices;
} GfxDraw3DRecord;

/* Blit state read by Blit_EmitDraw2DTextured: texture object and status flags. */
typedef struct SwBlitState {
    unsigned char pad_00[0x2c];
    int hasTexture;
    unsigned char pad_30[0x4];
    int texObject;
    unsigned char pad_38[0x70];
    int statusFlags;
} SwBlitState;

/* Line draw node in Blit_EmitDrawLineFlat whose vertices points at the line vertex records. */
typedef struct SwLineDrawFlatNode {
    unsigned char pad_00[0x8];
    int lineCount;
    void *vertexBase;
    unsigned char pad_10[0x4];
    void *vertices;
    unsigned char pad_18[0x8];
    void *indices;
} SwLineDrawFlatNode;

/* Triangle vertex set for Gfx_RasterSetupTriangleUvNoDepth: three vertex pointers. */
typedef struct SwUvTriangle {
    int vertexA;
    int vertexB;
    int vertexC;
    unsigned char pad_0C[0x9c];
    int mappedColor;
} SwUvTriangle;

/* Primitive mesh record whose two short words at 0x02 and 0x06 Gfx_BuildPrimitiveMesh sets. */
typedef struct GfxPrimitiveRecord {
    unsigned char pad_00[0x2];
    short kind;
    unsigned char pad_04[0x2];
    short mode;
} GfxPrimitiveRecord;

/* Sorted screen draw record: item pointer, texture page and render state. */
typedef struct GfxSortedDrawRecord {
    unsigned char pad_00[0xc];
    GfxScreenPrimitiveItem *item;
    int texturePage;
    GfxGroupRenderState *renderState;
} GfxSortedDrawRecord;

/* Float matrix whose translation row (0x30, 0x34, 0x38) is offset. */
typedef struct GfxMatrixTranslationRow {
    unsigned char pad_00[0x30];
    float translateX;
    float translateY;
    float translateZ;
} GfxMatrixTranslationRow;

/* Float matrix whose translation column (0x0C, 0x1C, 0x2C) is offset. */
typedef struct GfxMatrixTranslationColumn {
    unsigned char pad_00[0xc];
    float translateX;
    unsigned char pad_10[0xc];
    float translateY;
    unsigned char pad_20[0xc];
    float translateZ;
} GfxMatrixTranslationColumn;

/* 3x4 matrix whose diagonal BuildScaleMatrixXYZ fills from a scale vector. */
typedef struct GfxScaleMatrix {
    int scaleX;
    unsigned char pad_04[0x10];
    int scaleY;
    unsigned char pad_18[0x10];
    int scaleZ;
} GfxScaleMatrix;

/* Record whose words GetCachedDeviceTransform reads to get a cached transform handle. */
typedef struct GfxCachedTransformRecord {
    unsigned char pad_00[0x14];
    int matrixPtr0;
    int matrixPtr1;
    int matrixPtr2;
} GfxCachedTransformRecord;

/* Table of matrix slots; Gfx_SetMatrixTableSlotCall checks its enable flag and slot count. */
typedef struct GfxMatrixTable2 {
    int enabled;
    int slotCount;
    unsigned char pad_08[0x8];
    int slotBase;
} GfxMatrixTable2;

/* Matrix whose row-3 translation words SetMatrixTranslationRowDup writes. */
typedef struct GfxTransformMatrixRowForm {
    unsigned char pad_00[0x30];
    int translationX;
    int translationY;
    int translationZ;
} GfxTransformMatrixRowForm;

/* Four-byte D3D execute-buffer instruction header written by D3DExecEmitInstruction. */
typedef struct GfxD3DExecInstruction {
    char opcode;
    char instrSize;
    short count;
} GfxD3DExecInstruction;

/* List node whose heap chain ResetBlockHeapNodeList resets and whose two counters it zeroes. */
typedef struct GfxBlockHeapNodeList {
    int heapChain;
    int head;
    int tail;
} GfxBlockHeapNodeList;

/* Poly set that D3D5AllocBatchArray fills with a GameHeapAlloc'd batch array. */
typedef struct GfxPolySetBatchArray {
    unsigned char pad_00[0x10];
    int batchCount;
    unsigned char pad_14[0x3c];
    int *batchArray;
} GfxPolySetBatchArray;

/* The three per-frame block heaps of the primitive mesh bank (GfxPrimitiveMeshBank's vertex, index and entry
 * heaps), as Gfx_TrimRenderPoolOverflow trims them. */
typedef struct GfxRenderPool {
    unsigned char pad_00[0x4];
    struct MemBlockHeap *blockHeapA;
    struct MemBlockHeap *blockHeapB;
    struct MemBlockHeap *blockHeapC;
} GfxRenderPool;

/* A GL draw node, the emitters' second argument: a triangle batch, a textured (paged) batch or a line batch. */
typedef struct GlDrawNode {
    unsigned char pad_00[0x8];
    int entryCount; /* +0x08: the line count, or the page-id count of a paged node */
    int vertexCount; /* +0x0C */
    unsigned char pad_10[0x4];
    void *vertices; /* +0x14 */
    int indexCount; /* +0x18: the strip counts */
    unsigned char pad_1C[0x4];
    void *indices; /* +0x20 */
    unsigned char pad_24[0x4];
    unsigned char *pageIds; /* +0x28: one texture-page id per entry of a paged node */
    unsigned char pad_2C[0x4];
    struct GlNodeMesh *mesh; /* +0x30 */
} GlDrawNode;

/* The mesh record at GlDrawNode.mesh: the texture's draw group, the texture-page sizes and strides
 * of a paged node, and the model-space matrix. */
typedef struct GlNodeMesh {
    unsigned char pad_00[0xc];
    GlDrawGroup *texture; /* +0x0C */
    unsigned char pad_10[0x14];
    float sizeX; /* +0x24: half of it offsets the texture coordinates */
    float sizeY; /* +0x28 */
    unsigned char pad_2C[0xc];
    int stride; /* +0x38: vertices per entry */
    int pageStep; /* +0x3C: primitives per entry */
    unsigned char pad_40[0x58];
    int usePointedMatrix; /* +0x98 */
    void *dataPtr; /* +0x9C: the matrix, when usePointedMatrix is set */
    unsigned char matrix[0x40]; /* +0xA0: the inline matrix otherwise */
} GlNodeMesh;

/* The emitters' first argument: the draw state of a node. */
typedef struct GlDrawState {
    unsigned char pad_00[0x2c];
    int isTextured; /* +0x2C: the node has a texture to bind */
    unsigned char pad_30[0x4];
    GlDrawGroup *drawGroup; /* +0x34: its batch is the texture surface */
    unsigned char pad_38[0x70];
    int statusFlags;
} GlDrawState;

/* Packed three-byte BGR pixel written by Gfx_FillColorRowBytes. */
typedef struct GfxBgrPixel {
    unsigned char pad_00[0x1];
    unsigned char channel1;
    unsigned char channel2;
    unsigned char channel3;
} GfxBgrPixel;

/* Three-byte vertex color written from float colors by Gfx_AddFloatColorsToVertexBgr. */
typedef struct GfxVertexBgr {
    char colorB;
    char colorG;
    char colorR;
} GfxVertexBgr;

/* Polygon group record with an element count and a light-table index, read by GfxLoadPolygonSet. */
typedef struct GfxPolygonGroupRecord {
    unsigned char pad_00[0xc];
    int firstVertex; /* +0x0C: GfxLoadPolygonSet scales it by the vertex size (0xC) and the index size (4) */
    int elementCount;
    unsigned char pad_14[0x18];
    int lightIndex;
    int textureFlag;
} GfxPolygonGroupRecord;

/* Palette descriptor whose entry count Blit_WritePaletteEntries reads. */
typedef struct SwPaletteDesc {
    unsigned char pad_00[0x34];
    int entryCount;
    int dstEntries;
} SwPaletteDesc;

/* Raster context whose target and depth state the triangle draw functions read. */
typedef struct GfxRasterTriangleContext {
    unsigned char pad_00[0x48];
    GfxRasterEdgeChain edgeChain;
    unsigned char pad_F0[0x4];
    unsigned char scratch[0x94];
    int target;
    int depthState;
} GfxRasterTriangleContext;

/* Vertex record buffer with a capacity and an output word read by the raster prepare routine. */
typedef struct SwVertexRecordBuffer {
    unsigned char pad_00[0x4];
    int clipAnd;
    int capacity;
    int output;
} SwVertexRecordBuffer;

/* Execute-buffer light state op: a state type and its value. */
typedef struct GfxExecLightStateOp {
    int stateType;
    int value;
} GfxExecLightStateOp;

/* Execute-buffer render state op: a state type and its value. */
typedef struct GfxExecRenderStateOp {
    int stateType;
    int stateValue;
} GfxExecRenderStateOp;

/* Application descriptor initialised with a flag of 1 and a DirectX object. */
typedef struct GfxAppDescriptor {
    int debugLogEnabled;
    int logFileName;
} GfxAppDescriptor;

/* Sorted triangle record holding a texture page and a render state. */
typedef struct GfxSortedTriangleRecord {
    unsigned char pad_00[0x10];
    int texturePage;
    GfxGroupRenderState *renderState;
} GfxSortedTriangleRecord;

/* Palette object holding a buffer offset and an invoke object used by SetEntries. */
typedef struct GfxPaletteState {
    unsigned char pad_00[0x2c];
    int entryBuffer;
    int ddPalette;
} GfxPaletteState;

/* Range of triangles to emit: base index and triangle count. */
typedef struct GfxTriangleRange {
    unsigned char pad_00[0x4];
    int baseIndex;
    int triCount;
} GfxTriangleRange;

/* Record whose flags gate the global colour modulate and whose byte at +0x5C is read out. */
typedef struct GfxColorModulateRecord {
    unsigned char pad_00[0x8];
    int flags;
    unsigned char pad_0C[0x50];
    char modulateValue;
} GfxColorModulateRecord;

/* D3D5 batch whose two heap pointers are freed on release. */
typedef struct GfxD3d5BatchState {
    unsigned char pad_00[0x18];
    int indices;
    unsigned char pad_1C[0x8];
    int vertices;
} GfxD3d5BatchState;

/* D3D5 batch initialised with two counts at +0x08 and +0x20. */
typedef struct GfxD3d5BatchHeader {
    unsigned char pad_00[0x8];
    int primType;
    unsigned char pad_0C[0x14];
    int flags;
} GfxD3d5BatchHeader;

/* Pair of batch primitive cursors set from a pool node. */
typedef struct GfxBatchPrimitiveCursors {
    int matrix;
    int primitive;
} GfxBatchPrimitiveCursors;

/* Base UV pair copied into a group vertex when its texture coordinates are restored. */
typedef struct GfxGroupUvBase {
    int baseU;
    int baseV;
} GfxGroupUvBase;

/* Group vertex whose texture coordinates are restored from a base UV pair. */
typedef struct GfxGroupUvVertex {
    unsigned char pad_00[0x18];
    int u;
    int v;
} GfxGroupUvVertex;

/* Base UV pair that a wrapped scrolling group adds its scroll offset to. */
typedef struct GfxScrollUvBase {
    float baseU;
    float baseV;
} GfxScrollUvBase;

/* Vertex whose texture coordinates are offset by a delta. */
typedef struct GfxTexVertex {
    unsigned char pad_00[0x18];
    float u;
    float v;
} GfxTexVertex;

/* Width and height written out from a surface information record. */
typedef struct BlitSurfaceSize {
    int width;
    int height;
} BlitSurfaceSize;

/* Emit state read by the flat line begin path, carrying the texture pointer. */
typedef struct BlitLineFlatState {
    unsigned char pad_00[0x2c];
    int hasTexture;
    unsigned char pad_30[0x4];
    void *texture;
} BlitLineFlatState;

/* Draw state for the 3D textured blit path, carrying the texture object. */
typedef struct BlitDraw3DTexturedState {
    unsigned char pad_00[0x2c];
    int hasTexture;
    unsigned char pad_30[0x4];
    int texObj;
} BlitDraw3DTexturedState;

/* Projected screen point written by ProjectModelPointToScreen: x, y, z and width. */
typedef struct GfxProjectedPoint {
    int x;
    int y;
    int z;
    int coordWidth;
} GfxProjectedPoint;

/* Float 4x4 matrix whose translation row is set from a translation vector. */
typedef struct GfxMatrix4x4Translation {
    unsigned char pad_00[0x30];
    int translateX;
    int translateY;
    int translateZ;
} GfxMatrix4x4Translation;

/* Primitive written by Gfx_EmitPolygonSetPrimitive; two float accumulators at 0x18 and 0x1C. */
typedef struct GfxPolygonPrimitive {
    unsigned char pad_00[0x18];
    float tu;
    float tv;
} GfxPolygonPrimitive;

/* Actor attachment matrix stored transposed as PSX shorts; the conversion writes it from a float matrix. */
typedef struct GfxPsxMatrixTransposed {
    short m00;
    short m10;
    short m20;
    short m01;
    short m11;
    short m21;
    short m02;
    short m12;
    short m22;
} GfxPsxMatrixTransposed;

/* The frame statistics block Gfx_StatsCreateRecord allocates (0xFC bytes, `enabled` set) and the
 * engine context's statsRecord points at; Gfx_StatsClearRecord zeroes all but `enabled` each frame.
 * Gfx_StatsDrawTextRows prints it; the members carry its labels. The counters are bumped by the
 * Gfx_StatsBump*Count functions, the times are timestamp-counter deltas Gfx_StatsAdd*Time and the
 * frame loops add (Add64/Sub64). */
typedef struct GfxStatsRecord {
    int enabled; /* +0x00 */
    int countP; /* +0x04 "P" */
    int countV; /* +0x08 "V" */
    int countG; /* +0x0C "G" */
    int countS2; /* +0x10 "S2" */
    int countS3; /* +0x14 "S3" */
    int countM3; /* +0x18 "M3" */
    int countM43; /* +0x1C "M43" */
    int countM4; /* +0x20 "M4" */
    int countMr; /* +0x24 "MR" */
    int countMs; /* +0x28 "MS" */
    int level; /* +0x2C "LV" */
    int countTss; /* +0x30 "TSS" */
    int countTsu; /* +0x34 "TSU" */
    int countPc; /* +0x38 "PC" */
    int countPm; /* +0x3C "PM" */
    int wcCount; /* +0x40 "WC" */
    int bfCount; /* +0x44 "BF" */
    int wcCountAlt; /* +0x48 the second "WC" */
    int hcCount; /* +0x4C "HC" */
    Timestamp64 timeM3; /* +0x50 */
    Timestamp64 timeM43; /* +0x58 */
    Timestamp64 timeM4; /* +0x60 */
    Timestamp64 timeMr; /* +0x68 */
    Timestamp64 timeMs; /* +0x70 */
    Timestamp64 timeC; /* +0x78 "C" */
    Timestamp64 timeE; /* +0x80 "E" */
    Timestamp64 timeRl; /* +0x88 "RL" */
    Timestamp64 timeP; /* +0x90 "P" */
    Timestamp64 timeF; /* +0x98 "F": WinMain's whole frame */
    Timestamp64 timeA; /* +0xA0 "A": the field's, the chocobo race's and Fort Condor's frame */
    Timestamp64 timeGdi; /* +0xA8 "GDI" */
    Timestamp64 timeLvc; /* +0xB0 "LVC" */
    Timestamp64 timeSst; /* +0xB8 "SST" */
    Timestamp64 timeSut; /* +0xC0 "SUT" */
    unsigned char pad_C8[0x8];
    Timestamp64 timeVt; /* +0xD0 "VT" */
    Timestamp64 timeTs; /* +0xD8 "TS" */
    Timestamp64 timeTd; /* +0xE0 "TD" */
    Timestamp64 timeTc; /* +0xE8 "TC" */
    int rasterFrameCount; /* +0xF0 "TP": the software rasterizer's frame counter */
    Timestamp64 timeD; /* +0xF4 "D" */
} GfxStatsRecord;

/* Textured-rect state; its two ints are the first arguments of the untextured preset calls. */
typedef struct GfxTexturedRectState {
    unsigned char pad_00[0xdc];
    int presetAArg;
    int presetBArg;
} GfxTexturedRectState;

typedef struct {
    unsigned char b, g, r, a;
} ColorWord;

/* Palette block reached at offset 0x8C of a polygon-set group: entry count and stride. */
typedef struct GfxPaletteBlock {
    unsigned char pad_00[0xc];
    int count;
    int stride;
    unsigned char pad_14[0x4];
    int startIndex;
    unsigned char pad_1C[0x8];
    int dataPtr;
} GfxPaletteBlock;

/* Row buffers for Gfx_ScaleFloatColorRowToBytes: float input and byte output per row. */
typedef struct MemFloatColorRowBuffer {
    unsigned char pad_00[0xc];
    int rowCount;
    int width;
    unsigned char pad_14[0x4];
    int byteBuffer;
    unsigned char pad_1C[0x8];
    int floatBuffer;
} MemFloatColorRowBuffer;

#pragma pack(push, 1)
typedef struct GfxPsxShadedVertexColor {
    char red; /* +0x0 */
    char green; /* +0x1 */
    char blue; /* +0x2 */
    unsigned char pad_3[0x1];
} GfxPsxShadedVertexColor;
#pragma pack(pop)

/* g_GfxPsxCornerAlphas (src/lib/src/graphics/psxgraph.cpp): the alphas of a semi-transparent
 * primitive's four corners, set by Gfx_SetPsxCornerAlphas (Gfx_SetPsxAlpha sets the first, which
 * is all a flat primitive reads). */
#pragma pack(push, 1)
typedef struct GfxPsxCornerAlphas {
    char alpha0; /* +0x0 */
    char alpha1; /* +0x1 */
    char alpha2; /* +0x2 */
    char alpha3; /* +0x3 */
} GfxPsxCornerAlphas;
#pragma pack(pop)

/* Three 16-bit vector components (x, y, z) read by the GTE vector helpers. */
typedef struct PsxSVectorZ32 {
    short x;
    short y;
    int z;
} PsxSVectorZ32;

/* Offscreen render slot: in-use flag, request id and the render object it owns. */
typedef struct GfxOffscreenSlot {
    int inUse;
    int requestId;
    int renderObject;
    unsigned char request[0xd0]; /* the slot's GfxOffscreenRequest starts here and runs on through state */
    int state;
    unsigned char pad_E0[0x44];
} GfxOffscreenSlot;

/* Projected vertex record from Gfx_GetProjectedVertexRecord, carrying a blend factor pair. */
typedef struct GfxProjectedVertexRecord {
    unsigned char pad_00[0x8];
    int blendFactorA;
    int blendFactorB;
} GfxProjectedVertexRecord;

/* Polygon set record read by Gfx_LightPolygonSet and the field lighting callbacks: flags, specular index, BGRA colour. */
typedef struct GfxLightPolygonSet {
    int flags;
    unsigned char pad_04[0xf4];
    unsigned char floorVertex[0x84]; /* +0xF8: the GfxFloorVertex the lighting calls take */
    int specularIndex;
    unsigned char pad_180[0x428];
    int colorBgra;
} GfxLightPolygonSet;

/* Offscreen sprite record for the swirl effect: sprite rectangle, scale multiplier and handle. */
typedef struct GfxOffscreenRequest {
    int kind;                   /* +0x00: what Gfx_BuildOffscreenRenderObject draws: 0 nothing, 1 `mesh`,
                                 * 2 the skeleton `skeleton` at frame `animFrame` with `drawSpec` */
    int mesh;                   /* +0x04: kind 1: the polygon set (BattleStage_GetPartMesh) */
    int skeleton;               /* +0x08: kind 2 */
    int animFrame;              /* +0x0C: kind 2: the PolygonSkeletonAnim frame */
    int drawSpec;               /* +0x10: kind 2: the model Gfx_ApplyDrawSpecToModel is applied to */
    unsigned char placement[0x6c]; /* +0x14: the PolygonBoneTransform AnimPlacement_Init clears (0x74 bytes with the two members below) */
    int placementOutPos;        /* +0x80: its outPos */
    int placementTransformBase; /* +0x84: its transformBase */
    float modelMatrix[16];      /* +0x88: the 4x4 model matrix (Gfx_SetModelMatrix, Gfx_SetContextMatrix) */
    int pageFillEnabled;        /* +0xC8: copied to the GfxTexturePageFill's isEnabled */
    int renderKind;             /* +0xCC: the render object kind Gfx_CreateSurfaceRenderObject makes (4, 0xC, 0xE) */
    int blendMode;              /* +0xD0: Pfile_InitLoadOptionsWithBlend's blend */
    int drawPool;               /* +0xD4: the load options' drawPool */
    int renderObjectBank;       /* +0xD8: Gfx_CreateRenderObject's init (the context's or the battle scene's bank) */
    int viewportX;              /* +0xDC: the captured rectangle, also the page fill's origin and size */
    int viewportY;
    int viewportW;
    int viewportH;
    int projOffsetX;            /* +0xEC: SetProjectionOffset */
    int projOffsetY;
    int altViewportX;           /* +0xF4: the viewport drawn into when any of the four is set */
    int altViewportY;
    int altViewportW;
    int altViewportH;
    int viewportHeightOverride; /* +0x104: the engine context's viewportHeightOverride while drawing */
    int pageFillMode;           /* +0x108: the GfxTexturePageFill's mode */
    int pageFillStepX;          /* +0x10C: its stepX */
    int pageFillStepY;          /* +0x110: its stepY */
    int slotIndex;              /* +0x114: the slot's requestId (Gfx_ClaimOffscreenSlotRequest) */
} GfxOffscreenRequest;







typedef union BgraValue {
    unsigned int bgra;
    struct {
        unsigned char b, g, r, a;
    } c;
} BgraValue;

/* Unpacked 4-byte RGBA colour record shared by PSX 5-5-5 colour unpacking. */
typedef struct BattleRgbaColor {
    char colorR;
    char colorG;
    char colorB;
    char colorA;
} BattleRgbaColor;

/* 16-byte DirectX GUID record. */
typedef struct DxGuid {
    int word[4];
} DxGuid;

#endif /* LIB_TYPES_GRAPHICS_H */
