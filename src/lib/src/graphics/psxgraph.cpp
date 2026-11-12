// src/lib/src/graphics/psxgraph.cpp
// Original: C:\lib\src\graphics\psxgraph.cpp
// Contents: Holds the PSX-style graphics state the renderer reads: blend factors and mode, default
//   colors and alpha, vertex slot storage and the render state block. It also projects vertices,
//   applies transforms and handles deferred draw arguments.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/api/mem.h"
#include "lib/pfile.h"
#include "lib/types/graphics.h"
#include "lib/enums/graphics.h"
#include "lib/codegen.h"
#include "lib/api/graphics.h"
extern "C" {

struct GfxPsxShortVertexSlot;
struct GfxPsxSpriteQuadState;
typedef struct {
    float x, y, z;
} GfxPsxFloatVec3;
typedef GfxHomogeneousVec4 Vector16;
// clang-format off
// GLOBAL: FF7 0x0090c9d8
int g_GfxPsxBlendFactorSecond0 = FLOAT_ONE_BITS; /* 1.0f */
// GLOBAL: FF7 0x0090c9dc
int g_GfxPsxBlendFactorSecond1 = FLOAT_ONE_BITS; /* 1.0f */
// GLOBAL: FF7 0x0090c9e0
int g_GfxPsxBlendFactorSecond2 = FLOAT_ONE_BITS; /* 1.0f */
// GLOBAL: FF7 0x0090c9e4
int g_GfxPsxBlendFactorSecond3 = FLOAT_ONE_BITS; /* 1.0f */
// GLOBAL: FF7 0x0090c9e8
struct GfxPsxCornerAlphas g_GfxPsxCornerAlphas = {-1, -1, -1, -1};
// GLOBAL: FF7 0x0090c9ec
int g_GfxPsxRenderMode = BLEND_ADDITIVE;
// GLOBAL: FF7 0x0090c9f0
int g_GfxPsxBlendMode = 1;
// GLOBAL: FF7 0x00d8e490
unsigned char g_GfxPsxPacketScratch[36] = {0};
unsigned char g_bssPad_00d8e4b4[4] = {0};
unsigned char g_bssPad_00d8e4b8[4056] = {0};
// GLOBAL: FF7 0x00d8f490
int g_GfxPsxClutPosition = 0;
// GLOBAL: FF7 0x00d8f494
int g_GfxPsxVertexDepth = 0;
// GLOBAL: FF7 0x00d8f498
unsigned char g_GfxPsxScreenMatrix[64] = {0};
// GLOBAL: FF7 0x00d8f4d8
struct GfxPsxShadedVertexColor g_GfxPsxShadedVertexColor = {0};
unsigned char g_bssPad_00d8f4dc[4] = {0};
// GLOBAL: FF7 0x00d8f4e0
GfxHomogeneousVec4 g_GfxPsxScreenCoords0 = {0.0f};
// GLOBAL: FF7 0x00d8f4f0
GfxHomogeneousVec4 g_GfxPsxScreenCoords1 = {0.0f};
// GLOBAL: FF7 0x00d8f500
GfxHomogeneousVec4 g_GfxPsxScreenCoords2 = {0.0f};
// GLOBAL: FF7 0x00d8f510
GfxHomogeneousVec4 g_GfxPsxScreenCoords3 = {0.0f};
// GLOBAL: FF7 0x00d8f520
GfxPsxFloatVec3 g_GfxPsxDepthPoint0 = {0.0f};
unsigned char g_bssPad_00d8f52c[4] = {0};
// GLOBAL: FF7 0x00d8f530
GfxPsxFloatVec3 g_GfxPsxDepthPoint1 = {0.0f};
unsigned char g_bssPad_00d8f53c[4] = {0};
// GLOBAL: FF7 0x00d8f540
GfxPsxFloatVec3 g_GfxPsxDepthPoint2 = {0.0f};
unsigned char g_bssPad_00d8f54c[4] = {0};
// GLOBAL: FF7 0x00d8f550
GfxPsxFloatVec3 g_GfxPsxDepthPoint3 = {0.0f};
// GLOBAL: FF7 0x00d8f55c
void *g_GfxPsxVertexSlot0 = 0;
// GLOBAL: FF7 0x00d8f560
void *g_GfxPsxVertexSlot1 = 0;
// GLOBAL: FF7 0x00d8f564
void *g_GfxPsxVertexSlot2 = 0;
// GLOBAL: FF7 0x00d8f568
struct GfxPsxShortVertexSlot *g_GfxPsxVertexSlot3 = 0;
// GLOBAL: FF7 0x00d8f56c
int g_GfxPsxVertexSlot3Depth = 0;
// GLOBAL: FF7 0x00d8f570
unsigned char g_GfxPsxRotMatrixTransposed[64] = {0};
// GLOBAL: FF7 0x00d8f5b0
int g_GfxPsxVertexLightCode = 0;
unsigned char g_bssPad_00d8f5b4[4] = {0};
// GLOBAL: FF7 0x00d8f5b8
GfxPsxFloatVec3 g_GfxPsxInputVertex0 = {0.0f};
unsigned char g_bssPad_00d8f5c4[4] = {0};
// GLOBAL: FF7 0x00d8f5c8
GfxPsxFloatVec3 g_GfxPsxInputVertex1 = {0.0f};
unsigned char g_bssPad_00d8f5d4[4] = {0};
// GLOBAL: FF7 0x00d8f5d8
GfxPsxFloatVec3 g_GfxPsxInputVertex2 = {0.0f};
unsigned char g_bssPad_00d8f5e4[4] = {0};
// GLOBAL: FF7 0x00d8f5e8
GfxPsxFloatVec3 g_GfxPsxInputVertex3 = {0.0f};
unsigned char g_bssPad_00d8f5f4[4] = {0};
// GLOBAL: FF7 0x00d8f5f8
GfxHomogeneousVec4 g_GfxPsxClipVertex0 = {0.0f};
// GLOBAL: FF7 0x00d8f608
GfxHomogeneousVec4 g_GfxPsxClipVertex1 = {0.0f};
// GLOBAL: FF7 0x00d8f618
GfxHomogeneousVec4 g_GfxPsxClipVertex2 = {0.0f};
// GLOBAL: FF7 0x00d8f628
GfxHomogeneousVec4 g_GfxPsxClipVertex3 = {0.0f};
// GLOBAL: FF7 0x00d8f638
struct GfxPsxPrimitiveSet *g_pGfxPsxPrimitiveSystem = 0;
unsigned char g_bssPad_00d8f63c[4] = {0};
// GLOBAL: FF7 0x00d8f640
int g_GfxPsxRotatedVectorX = 0;
// GLOBAL: FF7 0x00d8f644
int g_GfxPsxRotatedVectorY = 0;
// GLOBAL: FF7 0x00d8f648
int g_GfxPsxRotatedVectorZ = 0;
unsigned char g_bssPad_00d8f64c[4] = {0};
// GLOBAL: FF7 0x00d8f650
int g_GfxPsxBlendFactorPairEnable = 0;
// GLOBAL: FF7 0x00d8f654
int g_GfxPsxBlendFactorFirst0 = 0;
// GLOBAL: FF7 0x00d8f658
int g_GfxPsxBlendFactorFirst1 = 0;
// GLOBAL: FF7 0x00d8f65c
int g_GfxPsxBlendFactorFirst2 = 0;
// GLOBAL: FF7 0x00d8f660
int g_GfxPsxBlendFactorFirst3 = 0;
// GLOBAL: FF7 0x00d8f664
int g_GfxPsxBlendFactorQuadEnable = 0;
// GLOBAL: FF7 0x00d8f668
int g_GfxPsxBlendEnable = 0;
// GLOBAL: FF7 0x00d8f66c
int g_GfxPsxOpaqueTableAlt = 0;
// clang-format on
void __cdecl Gfx_ReadVertexSlotVector(int slot, Vector16 *vertex);
void __cdecl Gfx_SetVertexSlotClipRect(int slot, Vector16 *vertex);
void __cdecl Gfx_EmitPsxTexturedFlatQuadScreen(int payload, int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3, int texU0, int texV0, int texU1, int texV1, int texU2, int texV2, int texU3, int texV3, int attr, int depth, int reciprocal, struct GfxPsxQuadEmitter *emitter);

/*
 * Stores enable in g_GfxPsxBlendFactorPairEnable.
 */
// FUNCTION: FF7 0x00666f60
void __cdecl Gfx_SetBlendFactorPairEnable(int enable)
{
    g_GfxPsxBlendFactorPairEnable = enable;
}

/*
 * Stores factor in g_GfxPsxBlendFactorFirst0.
 */
// FUNCTION: FF7 0x00666f6d
void __cdecl Gfx_SetBlendFactorFirst(int factor)
{
    g_GfxPsxBlendFactorFirst0 = factor;
}

/*
 * Stores factor in g_GfxPsxBlendFactorSecond0.
 */
// FUNCTION: FF7 0x00666f7a
void __cdecl Gfx_SetBlendFactorSecond(int factor)
{
    g_GfxPsxBlendFactorSecond0 = factor;
}

/*
 * Stores one in g_GfxPsxBlendFactorPairEnable, firstFactor in g_GfxPsxBlendFactorFirst0 and secondFactor in g_GfxPsxBlendFactorSecond0.
 */
// FUNCTION: FF7 0x00666f87
void __cdecl Gfx_SetBlendFactorPair(int firstFactor, int secondFactor)
{
    g_GfxPsxBlendFactorPairEnable = 1;
    g_GfxPsxBlendFactorFirst0 = firstFactor;
    g_GfxPsxBlendFactorSecond0 = secondFactor;
}

/*
 * Raises the flags g_GfxPsxBlendFactorPairEnable and g_GfxPsxBlendFactorQuadEnable to 1, then stores its eight arguments alternately
 * into g_GfxPsxBlendFactorFirst0 to g_GfxPsxBlendFactorFirst3 and g_GfxPsxBlendFactorSecond0 to g_GfxPsxBlendFactorSecond3, so four pairs of
 * the kind Gfx_SetBlendFactorPair sets one of. No other work.
 */
// FUNCTION: FF7 0x00666fa7
void __cdecl Gfx_SetBlendFactorQuad(float first0, float second0, float first1, float second1, float first2, float second2, float first3, float second3)
{
    g_GfxPsxBlendFactorPairEnable = 1;
    g_GfxPsxBlendFactorQuadEnable = 1;
    g_GfxPsxBlendFactorFirst0 = *(int *)&first0;
    g_GfxPsxBlendFactorSecond0 = *(int *)&second0;
    g_GfxPsxBlendFactorFirst1 = *(int *)&first1;
    g_GfxPsxBlendFactorSecond1 = *(int *)&second1;
    g_GfxPsxBlendFactorFirst2 = *(int *)&first2;
    g_GfxPsxBlendFactorSecond2 = *(int *)&second2;
    g_GfxPsxBlendFactorFirst3 = *(int *)&first3;
    g_GfxPsxBlendFactorSecond3 = *(int *)&second3;
}

/*
 * Returns the address of one of four globals chosen by index: 0 gives &g_GfxPsxScreenCoords2, 1 gives
 * &g_GfxPsxScreenCoords3, 2 gives &g_GfxPsxScreenCoords0 and 3 gives &g_GfxPsxScreenCoords1. The switch has no default, so
 * any other argument returns the uninitialised local.
 */
// FUNCTION: FF7 0x00667005
int __cdecl Gfx_GetProjectedVertexRecord(int index)
{
    GfxProjectedVertexRecord *record;

    /* Projected vertex slot index (0..3). */
    switch (index) {
    case GFX_PSX_VERTEX_SLOT_0:
        record = (GfxProjectedVertexRecord *)&g_GfxPsxScreenCoords2;
        break;
    case GFX_PSX_VERTEX_SLOT_1:
        record = (GfxProjectedVertexRecord *)&g_GfxPsxScreenCoords3;
        break;
    case GFX_PSX_VERTEX_SLOT_2:
        record = (GfxProjectedVertexRecord *)&g_GfxPsxScreenCoords0;
        break;
    case GFX_PSX_VERTEX_SLOT_3:
        record = (GfxProjectedVertexRecord *)&g_GfxPsxScreenCoords1;
    }
    return (int)record;
}

/*
 * Copies the full 16-byte vector from one of four global slot pointers to vertex in order 0, 1, 2,
 * 3 mapped to g_GfxPsxClipVertex1, g_GfxPsxClipVertex0, g_GfxPsxClipVertex3 and
 * g_GfxPsxClipVertex2. Other slots use slot zero.
 */
// FUNCTION: FF7 0x0066705a
void __cdecl Gfx_ReadVertexSlotVector(int slot, Vector16 *vertex)
{
    /* Projected vertex slot index (0..3). */
    switch (slot) {
    case GFX_PSX_VERTEX_SLOT_0:
        *vertex = g_GfxPsxClipVertex1;
        break;
    case GFX_PSX_VERTEX_SLOT_1:
        *vertex = g_GfxPsxClipVertex0;
        break;
    case GFX_PSX_VERTEX_SLOT_2:
        *vertex = g_GfxPsxClipVertex3;
        break;
    case GFX_PSX_VERTEX_SLOT_3:
        *vertex = g_GfxPsxClipVertex2;
        break;
    default:
        *vertex = g_GfxPsxClipVertex1;
    }
}

/*
 * Copies vertex full 16-byte vector into one of four global slot pointers, mapped0..3 to
 * g_GfxPsxClipVertex1, g_GfxPsxClipVertex0, g_GfxPsxClipVertex3 and g_GfxPsxClipVertex2. Other
 * slots use slot zero.
 */
// FUNCTION: FF7 0x0066714b
void __cdecl Gfx_SetVertexSlotClipRect(int slot, Vector16 *vertex)
{
    /* Projected vertex slot index (0..3). */
    switch (slot) {
    case GFX_PSX_VERTEX_SLOT_0:
        g_GfxPsxClipVertex1 = *vertex;
        break;
    case GFX_PSX_VERTEX_SLOT_1:
        g_GfxPsxClipVertex0 = *vertex;
        break;
    case GFX_PSX_VERTEX_SLOT_2:
        g_GfxPsxClipVertex3 = *vertex;
        break;
    case GFX_PSX_VERTEX_SLOT_3:
        g_GfxPsxClipVertex2 = *vertex;
        break;
    default:
        g_GfxPsxClipVertex1 = *vertex;
    }
}

/*
 * Sets the first corner alpha, the one a flat semi-transparent primitive reads.
 */
// FUNCTION: FF7 0x0066723c
void __cdecl Gfx_SetPsxAlpha(int alpha)
{
    (g_GfxPsxCornerAlphas.alpha0) = alpha;
}

/*
 * Sets the four corner alphas of the next semi-transparent primitive.
 */
// FUNCTION: FF7 0x00667249
void __cdecl Gfx_SetPsxCornerAlphas(int alpha0, int alpha1, int alpha2, int alpha3)
{
    (g_GfxPsxCornerAlphas.alpha0) = alpha0;
    g_GfxPsxCornerAlphas.alpha1 = alpha1;
    g_GfxPsxCornerAlphas.alpha2 = alpha2;
    g_GfxPsxCornerAlphas.alpha3 = alpha3;
}

/*
 * Stores mode (an enum BlendMode: which semi-transparent set Gfx_DrawPsxPrimitive draws into) in
 * g_GfxPsxRenderMode.
 */
// FUNCTION: FF7 0x00667270
void __cdecl Gfx_SetPsxBlendMode(int mode)
{
    g_GfxPsxRenderMode = mode;
}

/*
 * Stores enable in g_GfxPsxBlendEnable.
 */
// FUNCTION: FF7 0x0066727d
void __cdecl Gfx_SetPsxBlendEnable(int enable)
{
    g_GfxPsxBlendEnable = enable;
}

/*
 * Stores table in g_GfxPsxOpaqueTableAlt.
 */
// FUNCTION: FF7 0x0066728a
void __cdecl Gfx_SetPsxOpaqueTableAlt(int table)
{
    g_GfxPsxOpaqueTableAlt = table;
}

/*
 * Returns the address of g_GfxPsxPacketScratch.
 */
// FUNCTION: FF7 0x00667297
int __cdecl Gfx_GetPsxPacketScratchPtr(void)
{
    return (int)&g_GfxPsxPacketScratch;
}

/*
 * Calls GtePsxMatrixToFloat4x4Transposed with GetInstalledRotMatrixPtr() and the address of
 * g_GfxPsxRotMatrixTransposed.
 */
// FUNCTION: FF7 0x006672a1
void __cdecl Gfx_LoadInstalledRotMatrixFloatTransposed(void)
{
    GtePsxMatrixToFloat4x4Transposed(GetInstalledRotMatrixPtr(), &g_GfxPsxRotMatrixTransposed);
}

/*
 * Calls ComposeWithScreenMatrix with the addresses of g_GfxPsxRotMatrixTransposed and g_GfxPsxScreenMatrix.
 */
// FUNCTION: FF7 0x006672b9
void __cdecl Gfx_ComposeRotMatrixFloatWithScreenMatrix(void)
{
    ComposeWithScreenMatrix(&g_GfxPsxRotMatrixTransposed, &g_GfxPsxScreenMatrix);
}

/*
 * Converts the installed rotation matrix into g_GfxPsxRotMatrixTransposed, then composes it with the screen
 * matrix into g_GfxPsxScreenMatrix.
 */
// FUNCTION: FF7 0x006672d0
void __cdecl RefreshGlobalTransformState(void)
{
    GtePsxMatrixToFloat4x4Transposed(GetInstalledRotMatrixPtr(), &g_GfxPsxRotMatrixTransposed);
    ComposeWithScreenMatrix(&g_GfxPsxRotMatrixTransposed, &g_GfxPsxScreenMatrix);
}

// Does nothing unless g_pGfxPsxPrimitiveSystem is set. It then picks an attribute block: the attrs
// pointer of the second table entry when g_GfxPsxBlendEnable (set by Gfx_SetPsxBlendEnable) is
// nonzero, otherwise the index-th pointer in the table (the set for blend mode index). If the block
// is nonnull, the textured primitive kind in opcode selects which field receives state: POLY_FT3
// writes polyFt3State, POLY_GT3 polyGt3State, POLY_GT4 polyGt4State, and POLY_FT4 and the three SPRT kinds write state.
// Any other opcode writes nothing.
// FUNCTION: FF7 0x006672fa
void __cdecl Gfx_SetPsxPrimQuadState(int opcode, int index, GfxPsxSpriteQuadState *state)
{
    GfxPsxAttrBlock *attrBlock;

    if (g_pGfxPsxPrimitiveSystem != 0) {
        if (g_GfxPsxBlendEnable != 0) {
            attrBlock = (GfxPsxAttrBlock *)(g_pGfxPsxPrimitiveSystem[1].attrs);
        } else {
            attrBlock = (GfxPsxAttrBlock *)(((int *)g_pGfxPsxPrimitiveSystem)[index]);
        }
        if (attrBlock != 0) {
            switch (opcode) {
            case GFX_PSX_PRIM_POLY_FT3:
                attrBlock->polyFt3State = (int)state;
                break;
            case GFX_PSX_PRIM_POLY_FT4:
                attrBlock->state = (int)state;
                break;
            case GFX_PSX_PRIM_POLY_GT3:
                attrBlock->polyGt3State = (int)state;
                break;
            case GFX_PSX_PRIM_POLY_GT4:
                attrBlock->polyGt4State = (int)state;
                break;
            case GFX_PSX_PRIM_SPRT:
                attrBlock->state = (int)state;
                break;
            case GFX_PSX_PRIM_SPRT_8:
                attrBlock->state = (int)state;
                break;
            case GFX_PSX_PRIM_SPRT_16:
                attrBlock->state = (int)state;
            }
        }
    }
}

/*
 * Saturate doubled unsigned byte channels into a borrowed BGRA pixel.
 */
// FUNCTION: FF7 0x006673e4
void __cdecl Gfx_PackDoubledColorToBgra(char redByte, char greenByte, char blueByte, int alphaByte, GfxBgraPixel *dst)
{
    int doubledGreen;
    int redDoubledValue;
    int doubledBlue;

    redDoubledValue = (unsigned char)redByte;
    if (redDoubledValue >= 0x80) {
        redDoubledValue = 0xFF;
    } else {
        redDoubledValue <<= 1;
    }
    doubledGreen = (unsigned char)greenByte;
    if (doubledGreen >= 0x80) {
        doubledGreen = 0xFF;
    } else {
        doubledGreen <<= 1;
    }
    doubledBlue = (unsigned char)blueByte;
    if (doubledBlue >= 0x80) {
        doubledBlue = 0xFF;
    } else {
        doubledBlue <<= 1;
    }
    dst->red = redDoubledValue;
    dst->green = doubledGreen;
    dst->blue = doubledBlue;
    dst->alpha = alphaByte;
}

/*
 * Reconstructed native function.
 */
// FUNCTION: FF7 0x00667482
void __cdecl Gfx_DecodePsxClutPosition(int flagsWord, int clutWord, int owner, GfxPsxClutPosition *clutPos)
{
    int posX;
    int posY;
    GfxPsxClutState *clutState;
    int yOfs;

    if (owner != 0) {
        clutState = (GfxPsxClutState *)(*(int *)((char *)*(int *)((char *)*(int *)((char *)owner + 0xC) + 0x14) + 0x84));
        if (clutState != 0) {
            switch (clutState->mode) {
            case GFX_PSX_CLUT_DEPTH_4BPP:
                clutPos->colorMode = (flagsWord & 0x60) >> 5;
                clutPos->flags = flagsWord & 0xFFFFFF9F;
                break;
            case GFX_PSX_CLUT_DEPTH_8BPP:
                clutPos->colorMode = (flagsWord & 0x180) >> 7;
                clutPos->flags = flagsWord & 0xFFFFFE7F;
            }
            posX = (clutWord & 0x3F) << 4;
            posY = clutWord >> 6;
            if (REG_ALLOC_HINT) {
            }
            if (REG_ALLOC_HINT) {
            }
            yOfs = posY - clutState->clutYBase;
            if (yOfs < 0) {
                yOfs = 0;
            }
            clutPos->clutX = posX;
            clutPos->clutY = posY;
        }
        clutPos->yOfs = yOfs;
    } else {
        clutPos->yOfs = 0;
    }
}

/*
 * Converts three input shorts to global float slot-zero input, transforms it with the two global
 * matrices into full and depth-only results, narrows depth float to shared int and returns it.
 */
// FUNCTION: FF7 0x0066755d
int __cdecl TransformVertexSlot0(GfxPsxInputVertex *vertex)
{
    g_GfxPsxInputVertex3.x = (float)vertex->x;
    g_GfxPsxInputVertex3.y = (float)vertex->y;
    g_GfxPsxInputVertex3.z = (float)*(short *)&vertex->z;
    TransformPointByMatrix4x4(&g_GfxPsxScreenMatrix, &g_GfxPsxInputVertex3, &g_GfxPsxClipVertex1);
    TransformPointDepthOnly((float *)&g_GfxPsxRotMatrixTransposed, (float *)&g_GfxPsxInputVertex3, (float *)&g_GfxPsxDepthPoint3);
    g_GfxPsxVertexSlot3Depth = (int)g_GfxPsxDepthPoint3.z;
    return g_GfxPsxVertexSlot3Depth;
}

/*
 * Converts three input shorts into slot-one float globals, transforms through full and depth-only
 * matrices, narrows depth to g_GfxPsxVertexSlot3Depth and returns it. No range checks.
 */
// FUNCTION: FF7 0x006675e2
int __cdecl TransformVertexSlot1(GfxPsxShortVertexSlot *vertex)
{
    g_GfxPsxInputVertex1.x = (float)vertex->x;
    g_GfxPsxInputVertex1.y = (float)vertex->y;
    g_GfxPsxInputVertex1.z = (float)vertex->z;
    TransformPointByMatrix4x4(&g_GfxPsxScreenMatrix, &g_GfxPsxInputVertex1, &g_GfxPsxClipVertex0);
    TransformPointDepthOnly((float *)&g_GfxPsxRotMatrixTransposed, (float *)&g_GfxPsxInputVertex1, (float *)&g_GfxPsxDepthPoint2);
    g_GfxPsxVertexSlot3Depth = (int)g_GfxPsxDepthPoint2.z;
    return g_GfxPsxVertexSlot3Depth;
}

/*
 * Converts three input shorts into slot-three float globals, transforms full and depth-only
 * results, narrows depth to g_GfxPsxVertexDepth and returns that distinct depth global. No range checks.
 */
// FUNCTION: FF7 0x00667667
int __cdecl TransformVertexSlot3(GfxPsxShortVertexSlot *vertex)
{
    g_GfxPsxInputVertex0.x = (float)vertex->x;
    g_GfxPsxInputVertex0.y = (float)vertex->y;
    g_GfxPsxInputVertex0.z = (float)vertex->z;
    TransformPointByMatrix4x4(&g_GfxPsxScreenMatrix, &g_GfxPsxInputVertex0, &g_GfxPsxClipVertex2);
    TransformPointDepthOnly((float *)&g_GfxPsxRotMatrixTransposed, (float *)&g_GfxPsxInputVertex0, (float *)&g_GfxPsxDepthPoint0);
    g_GfxPsxVertexDepth = (int)g_GfxPsxDepthPoint0.z;
    return g_GfxPsxVertexDepth;
}

/*
 * Converts three input short vectors into separate float globals and transforms all through the
 * full and depth-only matrices. Returns/stores int of the sum of three depth floats times
 * 0.33333334f.
 */
// FUNCTION: FF7 0x006676ec
int __cdecl TransformVertices3(short *vertexA, short *vertexB, short *vertexC)
{
    g_GfxPsxInputVertex3.x = (float)vertexA[0];
    g_GfxPsxInputVertex3.y = (float)vertexA[1];
    g_GfxPsxInputVertex3.z = (float)vertexA[2];
    TransformPointByMatrix4x4(&g_GfxPsxScreenMatrix, &g_GfxPsxInputVertex3, &g_GfxPsxClipVertex1);
    g_GfxPsxInputVertex1.x = (float)vertexB[0];
    g_GfxPsxInputVertex1.y = (float)vertexB[1];
    g_GfxPsxInputVertex1.z = (float)vertexB[2];
    TransformPointByMatrix4x4(&g_GfxPsxScreenMatrix, &g_GfxPsxInputVertex1, &g_GfxPsxClipVertex0);
    g_GfxPsxInputVertex2.x = (float)vertexC[0];
    g_GfxPsxInputVertex2.y = (float)vertexC[1];
    g_GfxPsxInputVertex2.z = (float)vertexC[2];
    TransformPointByMatrix4x4(&g_GfxPsxScreenMatrix, &g_GfxPsxInputVertex2, &g_GfxPsxClipVertex3);
    TransformPointDepthOnly((float *)&g_GfxPsxRotMatrixTransposed, (float *)&g_GfxPsxInputVertex3, (float *)&g_GfxPsxDepthPoint3);
    TransformPointDepthOnly((float *)&g_GfxPsxRotMatrixTransposed, (float *)&g_GfxPsxInputVertex1, (float *)&g_GfxPsxDepthPoint2);
    TransformPointDepthOnly((float *)&g_GfxPsxRotMatrixTransposed, (float *)&g_GfxPsxInputVertex2, (float *)&g_GfxPsxDepthPoint1);
    g_GfxPsxVertexSlot3Depth = (int)((g_GfxPsxDepthPoint3.z + g_GfxPsxDepthPoint2.z + g_GfxPsxDepthPoint1.z) * 0.33333334f);
    return g_GfxPsxVertexSlot3Depth;
}

/*
 * Converts four input signed-short triples to separate float globals, transforms each by shared
 * matrix and computes depth-only transform by second matrix. Returns/stores integer cast of
 * average four depth outputs, with quarter factor applied after sum; globals make routine
 * nonreentrant.
 */
// FUNCTION: FF7 0x0066784f
int __cdecl TransformVertices4(short *vertexA, short *vertexB, short *vertexC, short *vertexD)
{
    g_GfxPsxInputVertex3.x = (float)vertexA[0];
    g_GfxPsxInputVertex3.y = (float)vertexA[1];
    g_GfxPsxInputVertex3.z = (float)vertexA[2];
    TransformPointByMatrix4x4(&g_GfxPsxScreenMatrix, &g_GfxPsxInputVertex3, &g_GfxPsxClipVertex1);
    g_GfxPsxInputVertex1.x = (float)vertexB[0];
    g_GfxPsxInputVertex1.y = (float)vertexB[1];
    g_GfxPsxInputVertex1.z = (float)vertexB[2];
    TransformPointByMatrix4x4(&g_GfxPsxScreenMatrix, &g_GfxPsxInputVertex1, &g_GfxPsxClipVertex0);
    g_GfxPsxInputVertex2.x = (float)vertexC[0];
    g_GfxPsxInputVertex2.y = (float)vertexC[1];
    g_GfxPsxInputVertex2.z = (float)vertexC[2];
    TransformPointByMatrix4x4(&g_GfxPsxScreenMatrix, &g_GfxPsxInputVertex2, &g_GfxPsxClipVertex3);
    g_GfxPsxInputVertex0.x = (float)vertexD[0];
    g_GfxPsxInputVertex0.y = (float)vertexD[1];
    g_GfxPsxInputVertex0.z = (float)vertexD[2];
    TransformPointByMatrix4x4(&g_GfxPsxScreenMatrix, &g_GfxPsxInputVertex0, &g_GfxPsxClipVertex2);
    TransformPointDepthOnly((float *)&g_GfxPsxRotMatrixTransposed, (float *)&g_GfxPsxInputVertex3, (float *)&g_GfxPsxDepthPoint3);
    TransformPointDepthOnly((float *)&g_GfxPsxRotMatrixTransposed, (float *)&g_GfxPsxInputVertex1, (float *)&g_GfxPsxDepthPoint2);
    TransformPointDepthOnly((float *)&g_GfxPsxRotMatrixTransposed, (float *)&g_GfxPsxInputVertex2, (float *)&g_GfxPsxDepthPoint1);
    TransformPointDepthOnly((float *)&g_GfxPsxRotMatrixTransposed, (float *)&g_GfxPsxInputVertex0, (float *)&g_GfxPsxDepthPoint0);
    g_GfxPsxVertexSlot3Depth = (int)((g_GfxPsxDepthPoint3.z + g_GfxPsxDepthPoint2.z + g_GfxPsxDepthPoint1.z + g_GfxPsxDepthPoint0.z) * 0.25);
    return g_GfxPsxVertexSlot3Depth;
}

/*
 * Stores value in g_GfxPsxVertexSlot0.
 */
// FUNCTION: FF7 0x00667a1e
void __cdecl Gfx_SetPendingVertexInput(const void *value)
{
    g_GfxPsxVertexSlot0 = (void *)value;
}

/*
 * Stores value0, value1 and value2 in g_GfxPsxVertexSlot0, g_GfxPsxVertexSlot1 and g_GfxPsxVertexSlot2.
 */
// FUNCTION: FF7 0x00667a2b
void __cdecl StoreDeferredDrawArgs(const void *value0, const void *value1, const void *value2)
{
    g_GfxPsxVertexSlot0 = (void *)value0;
    g_GfxPsxVertexSlot1 = (void *)value1;
    g_GfxPsxVertexSlot2 = (void *)value2;
}

/*
 * Calls TransformVertexSlot0 with g_GfxPsxVertexSlot0.
 */
// FUNCTION: FF7 0x00667a4a
void __cdecl Gfx_TransformPendingVertexSlot0(void)
{
    TransformVertexSlot0((struct GfxPsxInputVertex *)g_GfxPsxVertexSlot0);
}

/*
 * Calls TransformVertices3 with g_GfxPsxVertexSlot0, g_GfxPsxVertexSlot1 and g_GfxPsxVertexSlot2.
 */
// FUNCTION: FF7 0x00667a5d
void __cdecl ApplyDeferredDrawArgs(void)
{
    TransformVertices3((short *)g_GfxPsxVertexSlot0, (short *)g_GfxPsxVertexSlot1, (short *)g_GfxPsxVertexSlot2);
}

/*
 * Stores handle in g_GfxPsxVertexSlot3.
 */
// FUNCTION: FF7 0x00667a7e
void __cdecl SetPendingResourceHandle(GfxPsxShortVertexSlot *handle)
{
    g_GfxPsxVertexSlot3 = handle;
}

/*
 * Calls TransformVertexSlot3 with g_GfxPsxVertexSlot3.
 */
// FUNCTION: FF7 0x00667a8b
void __cdecl ApplyPendingResourceHandle(void)
{
    TransformVertexSlot3(g_GfxPsxVertexSlot3);
}

/*
 * Stores the cleared PSX transform flag word: writes zero through result as a dword.
 */
// FUNCTION: FF7 0x00667a9e
void __cdecl Gfx_PsxStoreClearFlag(int *result)
{
    *result = 0;
}

/*
 * Writes g_GfxPsxVertexSlot3Depth shifted right by two through outPtr as a dword.
 */
// FUNCTION: FF7 0x00667aac
void __cdecl Gfx_ReadTransformOrderingDepth(int *outPtr)
{
    *outPtr = g_GfxPsxVertexSlot3Depth >> 2;
}

/*
 * Gfx_ReadTransformOrderingDepth's twin: writes g_GfxPsxVertexSlot3Depth shifted right by two through count as a dword.
 */
// FUNCTION: FF7 0x00667abe
void __cdecl Gfx_ReadTransformOrderingDepth2(int *count)
{
    *count = g_GfxPsxVertexSlot3Depth >> 2;
}

/*
 * Stores the front-facing result the PSX callers test, always 1: writes one through result as a dword.
 */
// FUNCTION: FF7 0x00667ad0
void __cdecl Gfx_PsxStoreFrontFacing(int *result)
{
    *result = 1;
}

/*
 * Stores weight in g_GfxPsxVertexLightCode.
 */
// FUNCTION: FF7 0x00667ade
void __cdecl Gfx_SetPendingShadeWeight(int weight)
{
    g_GfxPsxVertexLightCode = weight;
}

/*
 * Copies three bytes from color to g_GfxPsxShadedVertexColor.red, .green and .blue.
 */
// FUNCTION: FF7 0x00667aeb
void __cdecl Gfx_SetPendingColorRgb(GfxPsxRgbColorIn *color)
{
    g_GfxPsxShadedVertexColor.red = color->red;
    g_GfxPsxShadedVertexColor.green = color->green;
    g_GfxPsxShadedVertexColor.blue = color->blue;
}

/*
 * Copies g_GfxPsxShadedVertexColor.red, .green and .blue to three bytes through color.
 */
// FUNCTION: FF7 0x00667b12
void __cdecl Gfx_ReadPendingColorRgb(GfxPsxRgbColorIn *color)
{
    color->red = (*(int *)&g_GfxPsxShadedVertexColor);
    color->green = g_GfxPsxShadedVertexColor.green;
    color->blue = g_GfxPsxShadedVertexColor.blue;
}

/*
 * Calls Gfx_ShadePsxVertexColor with g_GfxPsxShadedVertexColor as both source and destination and g_GfxPsxVertexLightCode
 * as the middle argument.
 */
// FUNCTION: FF7 0x00667b39
void __cdecl Gfx_ShadePendingColor(void)
{
    Gfx_ShadePsxVertexColor((CVECTOR *)&g_GfxPsxShadedVertexColor, g_GfxPsxVertexLightCode, (CVECTOR *)&g_GfxPsxShadedVertexColor);
}

/*
 * Calls RotateSVectorByInstalledMatrix with g_GfxPsxVertexSlot0 and the address of g_GfxPsxRotatedVectorX.
 */
// FUNCTION: FF7 0x00667b56
void __cdecl Gfx_RotatePendingVectorByInstalledMatrix(void)
{
    RotateSVectorByInstalledMatrix((void *)g_GfxPsxVertexSlot0, &g_GfxPsxRotatedVectorX);
}

/*
 * Copies g_GfxPsxRotatedVectorX, g_GfxPsxRotatedVectorY and g_GfxPsxRotatedVectorZ as three dwords through vector.
 */
// FUNCTION: FF7 0x00667b6e
void __cdecl Gfx_ReadRotatedVector(GfxExecPosition *vector)
{
    vector->x = g_GfxPsxRotatedVectorX;
    vector->y = g_GfxPsxRotatedVectorY;
    vector->z = g_GfxPsxRotatedVectorZ;
}

/*
 * Returns TransformVertexSlot0(vertex) shifted right by two after clearing the dword through flags;
 * unused is not read.
 */
// FUNCTION: FF7 0x00667b95
int __cdecl Gfx_TransformVertexSlot0ReturnDepth(void *vertex, void *unused, int *flags)
{
    int result;

    result = TransformVertexSlot0((struct GfxPsxInputVertex *)vertex) >> 2;
    *flags = 0;
    return result;
}

/*
 * Returns TransformVertexSlot1(vertex) shifted right by two after clearing the dword through flags;
 * unused is not read.
 */
// FUNCTION: FF7 0x00667bbb
int __cdecl Gfx_TransformVertexSlot1ReturnDepth(GfxPsxShortVertexSlot *vertex, void *unused, int *flags)
{
    int result;

    result = TransformVertexSlot1(vertex) >> 2;
    *flags = 0;
    return result;
}

/*
 * Returns TransformVertices3(vertexA, vertexB, vertexC) shifted right by two after clearing the dword
 * through flags; unused is not read.
 */
// FUNCTION: FF7 0x00667be1
int __cdecl Gfx_TransformTriangleReturnDepth(void *vertexA, SVECTOR *vertexB, SVECTOR *vertexC, void *unused, void *flags)
{
    int depth;

    depth = TransformVertices3((short *)vertexA, &vertexB->vx, &vertexC->vx) >> 2;
    *(int *)flags = 0;
    return depth;
}

/*
 * Returns TransformVertices4(vertexA, vertexB, vertexC, vertexD) shifted right by two after clearing flags;
 * unused is not read.
 */
// FUNCTION: FF7 0x00667c0f
int __cdecl Gfx_TransformQuadReturnDepth(void *vertexA, SVECTOR *vertexB, SVECTOR *vertexC, SVECTOR *vertexD, int *unused, int *flags)
{
    int result;

    result = TransformVertices4((short *)vertexA, &vertexB->vx, &vertexC->vx, &vertexD->vx) >> 2;
    *flags = 0;
    return result;
}

/*
 * For positive w, packs int(x) low sixteen bits and int(y) shifted left sixteen into out;
 * otherwise writes zero.
 */
// FUNCTION: FF7 0x00667c41
void __cdecl Gfx_PackProjectedXY(float x, float y, float w, unsigned int *out)
{
    if (w > 0.0f)
        *out = ((int)x & 0xffff) | ((int)y << 16);
    else
        *out = 0;
}

/*
 * Transforms three vertices and takes returned depth shifted two. Perspective-divides their global
 * results and calls Gfx_PackProjectedXY with x, y, w and each output pointer, clears flags and returns
 * depth. Arg7 is unused; projection failure/range checks are absent here.
 */
// FUNCTION: FF7 0x00667c85
int __cdecl Gfx_ProjectTriangleToScreen(void *vertexA, SVECTOR *vertexB, SVECTOR *vertexC, void *outPtr0, void *outPtr1, void *outPtr2, void *unused, void *flags)
{
    int depth;

    depth = TransformVertices3((short *)vertexA, &vertexB->vx, &vertexC->vx) >> 2;
    PerspectiveDivide(&g_GfxPsxClipVertex1, (GfxHomogeneousVec43 *)&g_GfxPsxScreenCoords2);
    Gfx_PackProjectedXY(g_GfxPsxScreenCoords2.x, g_GfxPsxScreenCoords2.y, g_GfxPsxScreenCoords2.w, (unsigned int *)outPtr0);
    PerspectiveDivide(&g_GfxPsxClipVertex0, (GfxHomogeneousVec43 *)&g_GfxPsxScreenCoords3);
    Gfx_PackProjectedXY(g_GfxPsxScreenCoords3.x, g_GfxPsxScreenCoords3.y, g_GfxPsxScreenCoords3.w, (unsigned int *)outPtr1);
    PerspectiveDivide(&g_GfxPsxClipVertex3, (GfxHomogeneousVec43 *)&g_GfxPsxScreenCoords0);
    Gfx_PackProjectedXY(g_GfxPsxScreenCoords0.x, g_GfxPsxScreenCoords0.y, g_GfxPsxScreenCoords0.w, (unsigned int *)outPtr2);
    *(int *)flags = 0;
    return depth;
}

/*
 * Transforms four vertices, returns depth shifted two and perspective-divides each global result
 * before calling Gfx_PackProjectedXY with x, y, w and corresponding output. Clears flags; the `unused` parameter is ignored
 * and no projection failure test occurs here.
 */
// FUNCTION: FF7 0x00667d49
int __cdecl Gfx_ProjectQuadToScreen(SVECTOR *vertexA, SVECTOR *vertexB, SVECTOR *vertexC, SVECTOR *vertexD, void *outPtr0, void *outPtr1, void *outPtr2, void *outPtr3, void *unused, void *flags)
{
    int depth;

    depth = TransformVertices4(&vertexA->vx, &vertexB->vx, &vertexC->vx, &vertexD->vx) >> 2;
    PerspectiveDivide(&g_GfxPsxClipVertex1, (GfxHomogeneousVec43 *)&g_GfxPsxScreenCoords2);
    Gfx_PackProjectedXY(g_GfxPsxScreenCoords2.x, g_GfxPsxScreenCoords2.y, g_GfxPsxScreenCoords2.w, (unsigned int *)outPtr0);
    PerspectiveDivide(&g_GfxPsxClipVertex0, (GfxHomogeneousVec43 *)&g_GfxPsxScreenCoords3);
    Gfx_PackProjectedXY(g_GfxPsxScreenCoords3.x, g_GfxPsxScreenCoords3.y, g_GfxPsxScreenCoords3.w, (unsigned int *)outPtr1);
    PerspectiveDivide(&g_GfxPsxClipVertex3, (GfxHomogeneousVec43 *)&g_GfxPsxScreenCoords0);
    Gfx_PackProjectedXY(g_GfxPsxScreenCoords0.x, g_GfxPsxScreenCoords0.y, g_GfxPsxScreenCoords0.w, (unsigned int *)outPtr2);
    PerspectiveDivide(&g_GfxPsxClipVertex2, (GfxHomogeneousVec43 *)&g_GfxPsxScreenCoords1);
    Gfx_PackProjectedXY(g_GfxPsxScreenCoords1.x, g_GfxPsxScreenCoords1.y, g_GfxPsxScreenCoords1.w, (unsigned int *)outPtr3);
    *(int *)flags = 0;
    return depth;
}

/*
 * Returns TransformVertices3(vertexA, vertexB, vertexC) shifted right by two after clearing the dword
 * through flags; unused is not read.
 */
// FUNCTION: FF7 0x00667e43
int __cdecl Gfx_TransformTriangleReturnDepthAlt(void *vertexA, SVECTOR *vertexB, SVECTOR *vertexC, void *unused, void *flags)
{
    int depth;

    depth = TransformVertices3((short *)vertexA, &vertexB->vx, &vertexC->vx) >> 2;
    *(int *)flags = 0;
    return depth;
}

/*
 * Transforms three vertices, perspective-divides each global result and passes x, y, w with its
 * output pointer to Gfx_PackProjectedXY. Clears flags and returns transformed depth shifted two. Arg7 is
 * unused; failure/range checks absent.
 */
// FUNCTION: FF7 0x00667e71
int __cdecl Gfx_ProjectTriangleToScreenAlt(void *vertexA, SVECTOR *vertexB, SVECTOR *vertexC, void *outPtr0, void *outPtr1, void *outPtr2, void *unused, void *flags)
{
    int depth;

    depth = TransformVertices3((short *)vertexA, &vertexB->vx, &vertexC->vx) >> 2;
    PerspectiveDivide(&g_GfxPsxClipVertex1, (GfxHomogeneousVec43 *)&g_GfxPsxScreenCoords2);
    Gfx_PackProjectedXY(g_GfxPsxScreenCoords2.x, g_GfxPsxScreenCoords2.y, g_GfxPsxScreenCoords2.w, (unsigned int *)outPtr0);
    PerspectiveDivide(&g_GfxPsxClipVertex0, (GfxHomogeneousVec43 *)&g_GfxPsxScreenCoords3);
    Gfx_PackProjectedXY(g_GfxPsxScreenCoords3.x, g_GfxPsxScreenCoords3.y, g_GfxPsxScreenCoords3.w, (unsigned int *)outPtr1);
    PerspectiveDivide(&g_GfxPsxClipVertex3, (GfxHomogeneousVec43 *)&g_GfxPsxScreenCoords0);
    Gfx_PackProjectedXY(g_GfxPsxScreenCoords0.x, g_GfxPsxScreenCoords0.y, g_GfxPsxScreenCoords0.w, (unsigned int *)outPtr2);
    *(int *)flags = 0;
    return depth;
}

/*
 * Returns TransformVertices4(vertexA, vertexB, vertexC, vertexD) shifted right by two after clearing flags;
 * unused is not read.
 */
// FUNCTION: FF7 0x00667f35
int __cdecl Gfx_TransformQuadReturnDepthAlt(SVECTOR *vertexA, SVECTOR *vertexB, SVECTOR *vertexC, SVECTOR *vertexD, int *unused, int *flags)
{
    int result;

    result = TransformVertices4(&vertexA->vx, &vertexB->vx, &vertexC->vx, &vertexD->vx) >> 2;
    *flags = 0;
    return result;
}

/*
 * Transforms four vertices, perspective-divides their global results and passes x, y, w with each
 * output pointer to Gfx_PackProjectedXY. Clears flags and returns transformed depth shifted two. Arg9 is
 * unused; failure/range checks absent.
 */
// FUNCTION: FF7 0x00667f67
int __cdecl Gfx_ProjectQuadToScreenAlt(SVECTOR *vertexA, SVECTOR *vertexB, SVECTOR *vertexC, SVECTOR *vertexD, void *outPtr0, void *outPtr1, void *outPtr2, void *outPtr3, void *unused, void *flags)
{
    int depth;

    depth = TransformVertices4(&vertexA->vx, &vertexB->vx, &vertexC->vx, &vertexD->vx) >> 2;
    PerspectiveDivide(&g_GfxPsxClipVertex1, (GfxHomogeneousVec43 *)&g_GfxPsxScreenCoords2);
    Gfx_PackProjectedXY(g_GfxPsxScreenCoords2.x, g_GfxPsxScreenCoords2.y, g_GfxPsxScreenCoords2.w, (unsigned int *)outPtr0);
    PerspectiveDivide(&g_GfxPsxClipVertex0, (GfxHomogeneousVec43 *)&g_GfxPsxScreenCoords3);
    Gfx_PackProjectedXY(g_GfxPsxScreenCoords3.x, g_GfxPsxScreenCoords3.y, g_GfxPsxScreenCoords3.w, (unsigned int *)outPtr1);
    PerspectiveDivide(&g_GfxPsxClipVertex3, (GfxHomogeneousVec43 *)&g_GfxPsxScreenCoords0);
    Gfx_PackProjectedXY(g_GfxPsxScreenCoords0.x, g_GfxPsxScreenCoords0.y, g_GfxPsxScreenCoords0.w, (unsigned int *)outPtr2);
    PerspectiveDivide(&g_GfxPsxClipVertex2, (GfxHomogeneousVec43 *)&g_GfxPsxScreenCoords1);
    Gfx_PackProjectedXY(g_GfxPsxScreenCoords1.x, g_GfxPsxScreenCoords1.y, g_GfxPsxScreenCoords1.w, (unsigned int *)outPtr3);
    *(int *)flags = 0;
    return depth;
}

// Emits a flat-coloured two-vertex primitive into the batch. It writes nothing unless both vertexA
// and vertexB have a positive w and AcquireBatchNodeNoCommandArea(1, batch) returns nonzero. Each
// vertex is projected with p = 1.0f / w, and x*p, y*p and z*p are stored in its slot with p as invW
// and the colour color. The first slot is at batch->vertexCursor and the second is 0x20 bytes
// after it.
// FUNCTION: FF7 0x00668061
void __cdecl Gfx_EmitPsxFlatLineProjected(int color, GfxHomogeneousVec4 *vertexA, GfxHomogeneousVec4 *vertexB, GfxRenderObject *batch)
{
    float invW;
    GfxPsxPolylineSlot *slot;
    float secondInvW;
    GfxPsxPolylineSlot *secondSlot;

    if (vertexA->w > 0.0 && vertexB->w > 0.0) {
        if (AcquireBatchNodeNoCommandArea(1, batch) != 0) {
            invW = 1.0f / vertexA->w;
            slot = (GfxPsxPolylineSlot *)(batch->vertexCursor);
            slot->x = vertexA->x * invW;
            slot->y = vertexA->y * invW;
            slot->z = vertexA->z * invW;
            slot->invW = invW;
            slot->color = color;
            secondInvW = 1.0f / vertexB->w;
            secondSlot = (GfxPsxPolylineSlot *)(batch->vertexCursor + 0x20);
            secondSlot->x = vertexB->x * secondInvW;
            secondSlot->y = vertexB->y * secondInvW;
            secondSlot->z = vertexB->z * secondInvW;
            secondSlot->invW = secondInvW;
            secondSlot->color = color;
        }
    }
}

// Integer counterpart of Gfx_EmitPsxFlatLineProjected, with no perspective divide and no w test.
// After AcquireBatchNodeNoCommandArea(1, batch) returns nonzero it writes two 0x20-byte slots
// starting at batch->vertexCursor. The first takes x0 and y0, the second takes x1 and y1, and
// both take z, w and the colour color.
// FUNCTION: FF7 0x0066815c
void __cdecl Gfx_EmitPsxFlatLineScreen(int color, int x0, int y0, int x1, int y1, int z, int w, GfxRenderObject *batch)
{
    GfxPsxFlatScreenLineSlot *slot;
    GfxPsxFlatScreenLineSlot *secondSlot;

    if (AcquireBatchNodeNoCommandArea(1, batch) != 0) {
        slot = (GfxPsxFlatScreenLineSlot *)(batch->vertexCursor);
        slot->x = x0;
        slot->y = y0;
        slot->z = z;
        slot->w = w;
        slot->color = color;
        secondSlot = (GfxPsxFlatScreenLineSlot *)(batch->vertexCursor + 0x20);
        secondSlot->x = x1;
        secondSlot->y = y1;
        secondSlot->z = z;
        secondSlot->w = w;
        secondSlot->color = color;
    }
}

/*
 * Same two-vertex perspective-divided emit as Gfx_EmitPsxFlatLineProjected, but with a colour per vertex: vertexA and
 * vertexB are the source vertices, batch the batch context, and the color of the first slot gets
 * color0 while the second gets color1. Skipped unless both w values are positive and
 * AcquireBatchNodeNoCommandArea(1, batch) returns nonzero.
 */
// FUNCTION: FF7 0x006681e5
void __cdecl Gfx_EmitPsxGouraudLineProjected(int color0, int color1, GfxHomogeneousVec4 *vertexA, GfxHomogeneousVec4 *vertexB, GfxRenderObject *batch)
{
    float invW;
    GfxPsxPolylineSlot *slot;
    float secondInvW;
    GfxPsxPolylineSlot *secondSlot;

    if (vertexA->w > 0.0 && vertexB->w > 0.0) {
        if (AcquireBatchNodeNoCommandArea(1, batch) != 0) {
            invW = 1.0f / vertexA->w;
            slot = (GfxPsxPolylineSlot *)(batch->vertexCursor);
            slot->x = vertexA->x * invW;
            slot->y = vertexA->y * invW;
            slot->z = vertexA->z * invW;
            slot->invW = invW;
            slot->color = color0;
            secondInvW = 1.0f / vertexB->w;
            secondSlot = (GfxPsxPolylineSlot *)(batch->vertexCursor + 0x20);
            secondSlot->x = vertexB->x * secondInvW;
            secondSlot->y = vertexB->y * secondInvW;
            secondSlot->z = vertexB->z * secondInvW;
            secondSlot->invW = secondInvW;
            secondSlot->color = color1;
        }
    }
}

// Integer two-vertex emit with a colour per vertex. After AcquireBatchNodeNoCommandArea(1, batch)
// returns nonzero it fills the slot at batch->vertexCursor from x0, y0 and color0, and the slot
// 0x20 bytes later from x1, y1 and color1. Both slots take z and w.
// FUNCTION: FF7 0x006682e0
void __cdecl Gfx_EmitPsxGouraudLineScreen(int color0, int color1, int x0, int y0, int x1, int y1, int z, int w, GfxRenderObject *batch)
{
    GfxPsxFlatScreenLineSlot *slot;
    GfxPsxFlatScreenLineSlot *secondSlot;

    if (AcquireBatchNodeNoCommandArea(1, batch) != 0) {
        slot = (GfxPsxFlatScreenLineSlot *)(batch->vertexCursor);
        slot->x = x0;
        slot->y = y0;
        slot->z = z;
        slot->w = w;
        slot->color = color0;
        secondSlot = (GfxPsxFlatScreenLineSlot *)(batch->vertexCursor + 0x20);
        secondSlot->x = x1;
        secondSlot->y = y1;
        secondSlot->z = z;
        secondSlot->w = w;
        secondSlot->color = color1;
    }
}

/*
 * When all three input vertices have positive fourth float components and the reservation helper
 * for mode 2 succeeds, writes four 32-byte output records at context offset 112. Each contains XYZ
 * divided by the fourth component, its reciprocal, and a supplied dword payload. The sequence is
 * first vertex and color, second vertex and color1 twice, then third vertex and color2. Leaves trailing
 * record fields untouched and performs no writes when either guard fails.
 */
// FUNCTION: FF7 0x00668369
void __cdecl Gfx_EmitPsxGouraudPolylineProjected(int color, int color1, int color2, GfxHomogeneousVec4 *vertexA, GfxPsxProjectedPoint *secondPoint, GfxHomogeneousVec4 *vertexC, GfxRenderObject *batchNode)
{
    float invW;
    GfxPsxPolylineSlot *slot0;
    GfxPsxPolylineSlot *slot1;
    float invZ;
    float subInvDepth;
    float subInvWidth;
    GfxPsxPolylineSlot *pickedOutPtr;
    GfxPsxPolylineSlot *calcRecord;

    if (vertexA->w > 0.0 && secondPoint->depth > 0.0 && vertexC->w > 0.0) {
        if (AcquireBatchNodeNoCommandArea(2, batchNode) != 0) {
            invW = 1.0f / vertexA->w;
            slot0 = (GfxPsxPolylineSlot *)(batchNode->vertexCursor);
            slot0->x = vertexA->x * invW;
            slot0->y = vertexA->y * invW;
            slot0->z = vertexA->z * invW;
            slot0->invW = invW;
            slot0->color = color;
            invZ = 1.0f / secondPoint->depth;
            slot1 = (GfxPsxPolylineSlot *)(batchNode->vertexCursor + 0x20);
            slot1->x = secondPoint->x * invZ;
            slot1->y = secondPoint->y * invZ;
            slot1->z = secondPoint->z * invZ;
            slot1->invW = invZ;
            slot1->color = color1;
            subInvDepth = 1.0f / secondPoint->depth;
            pickedOutPtr = (GfxPsxPolylineSlot *)(batchNode->vertexCursor + 0x40);
            pickedOutPtr->x = secondPoint->x * subInvDepth;
            pickedOutPtr->y = secondPoint->y * subInvDepth;
            pickedOutPtr->z = secondPoint->z * subInvDepth;
            pickedOutPtr->invW = subInvDepth;
            pickedOutPtr->color = color1;
            subInvWidth = 1.0f / vertexC->w;
            calcRecord = (GfxPsxPolylineSlot *)(batchNode->vertexCursor + 0x60);
            calcRecord->x = vertexC->x * subInvWidth;
            calcRecord->y = vertexC->y * subInvWidth;
            calcRecord->z = vertexC->z * subInvWidth;
            calcRecord->invW = subInvWidth;
            calcRecord->color = color2;
        }
    }
}

// Integer counterpart of Gfx_EmitPsxGouraudPolylineProjected. After
// AcquireBatchNodeNoCommandArea(2, batchNode) returns nonzero it writes four 0x20-byte slots from
// batchNode->vertexCursor. They take the vertices x0/y0 with color0, x1/y1 with color1, x1/y1
// again with color1, and x2/y2 with color2, and every slot takes word0 and word1. The middle vertex
// is duplicated, so this is one polyline of two segments.
// FUNCTION: FF7 0x0066852b
void __cdecl Gfx_EmitPsxGouraudPolylineScreen(int color0, int color1, int color2, int x0, int y0, int x1, int y1, int x2, int y2, int word0, int word1, GfxRenderObject *batchNode)
{
    GfxFlatQuadScreenVertex *slot0;
    GfxFlatQuadScreenVertex *slot1;
    GfxFlatQuadScreenVertex *slot2;
    GfxFlatQuadScreenVertex *slot3;

    if (AcquireBatchNodeNoCommandArea(2, batchNode) != 0) {
        slot0 = (GfxFlatQuadScreenVertex *)(batchNode->vertexCursor);
        slot0->x = x0;
        slot0->y = y0;
        slot0->z = word0;
        slot0->w = word1;
        slot0->color = color0;
        slot1 = (GfxFlatQuadScreenVertex *)(batchNode->vertexCursor + 0x20);
        slot1->x = x1;
        slot1->y = y1;
        slot1->z = word0;
        slot1->w = word1;
        slot1->color = color1;
        slot2 = (GfxFlatQuadScreenVertex *)(batchNode->vertexCursor + 0x40);
        slot2->x = x1;
        slot2->y = y1;
        slot2->z = word0;
        slot2->w = word1;
        slot2->color = color1;
        slot3 = (GfxFlatQuadScreenVertex *)(batchNode->vertexCursor + 0x60);
        slot3->x = x2;
        slot3->y = y2;
        slot3->z = word0;
        slot3->w = word1;
        slot3->color = color2;
    }
}

/*
 * For three vertices all positive homogeneous W and successful batch-node acquisition writes three
 * 32-byte entries with XYZ divided by W, reciprocal W and shared packed color field. Other per-
 * entry fields untouched; any invalid W or allocation failure leaves no explicit write.
 */
// FUNCTION: FF7 0x00668628
void __cdecl Gfx_EmitPsxFlatTriangleProjected(int color, GfxHomogeneousVec4 *vertex0, GfxHomogeneousVec4 *vertex1, GfxHomogeneousVec4 *vertex2, GfxRenderObject *batchNode)
{
    GfxPsxPolylineSlot *slot0;
    float invW0;
    GfxPsxPolylineSlot *slot1;
    float invW1;
    GfxPsxPolylineSlot *slot2;
    float invW2;

    if (vertex0->w > 0.0 && vertex1->w > 0.0 && vertex2->w > 0.0) {
        if (AcquireBatchNode(1, batchNode) != 0) {
            invW0 = 1.0f / vertex0->w;
            slot0 = (GfxPsxPolylineSlot *)(batchNode->vertexCursor);
            slot0->x = vertex0->x * invW0;
            slot0->y = vertex0->y * invW0;
            slot0->z = vertex0->z * invW0;
            slot0->invW = invW0;
            slot0->color = color;
            invW1 = 1.0f / vertex1->w;
            slot1 = (GfxPsxPolylineSlot *)(batchNode->vertexCursor + 0x20);
            slot1->x = vertex1->x * invW1;
            slot1->y = vertex1->y * invW1;
            slot1->z = vertex1->z * invW1;
            slot1->invW = invW1;
            slot1->color = color;
            invW2 = 1.0f / vertex2->w;
            slot2 = (GfxPsxPolylineSlot *)(batchNode->vertexCursor + 0x40);
            slot2->x = vertex2->x * invW2;
            slot2->y = vertex2->y * invW2;
            slot2->z = vertex2->z * invW2;
            slot2->invW = invW2;
            slot2->color = color;
        }
    }
}

/*
 * Requires positive fourth float components for four input vertices and successful batch
 * acquisition in mode one. Writes four 32-byte records at context offset 112, each with XYZ
 * divided by its fourth component, the reciprocal, and the same color dword payload. Leaves each
 * record after offset 16 untouched and makes no output writes when a guard fails.
 */
// FUNCTION: FF7 0x00668792
void __cdecl Gfx_EmitPsxFlatQuadProjected(int color, GfxHomogeneousVec4 *vertex0, GfxHomogeneousVec4 *vertex1, GfxHomogeneousVec4 *vertex2, GfxHomogeneousVec4 *vertex3, GfxRenderObject *batchNode)
{
    GfxPsxPolylineSlot *slot0;
    float invW0;
    GfxPsxPolylineSlot *slot1;
    float invW1;
    GfxPsxPolylineSlot *slot2;
    float invW2;
    GfxPsxPolylineSlot *slot3;
    float invW3;

    if (vertex0->w > 0.0 && vertex1->w > 0.0 && vertex2->w > 0.0 && vertex3->w > 0.0) {
        if (AcquireBatchNode(1, batchNode) != 0) {
            invW0 = 1.0f / vertex0->w;
            slot0 = (GfxPsxPolylineSlot *)(batchNode->vertexCursor);
            slot0->x = vertex0->x * invW0;
            slot0->y = vertex0->y * invW0;
            slot0->z = vertex0->z * invW0;
            slot0->invW = invW0;
            slot0->color = color;
            invW1 = 1.0f / vertex1->w;
            slot1 = (GfxPsxPolylineSlot *)(batchNode->vertexCursor + 0x20);
            slot1->x = vertex1->x * invW1;
            slot1->y = vertex1->y * invW1;
            slot1->z = vertex1->z * invW1;
            slot1->invW = invW1;
            slot1->color = color;
            invW2 = 1.0f / vertex2->w;
            slot2 = (GfxPsxPolylineSlot *)(batchNode->vertexCursor + 0x40);
            slot2->x = vertex2->x * invW2;
            slot2->y = vertex2->y * invW2;
            slot2->z = vertex2->z * invW2;
            slot2->invW = invW2;
            slot2->color = color;
            invW3 = 1.0f / vertex3->w;
            slot3 = (GfxPsxPolylineSlot *)(batchNode->vertexCursor + 0x60);
            slot3->x = vertex3->x * invW3;
            slot3->y = vertex3->y * invW3;
            slot3->z = vertex3->z * invW3;
            slot3->invW = invW3;
            slot3->color = color;
        }
    }
}

// Integer flat-coloured triangle. After AcquireBatchNode(1, (int)batchNode) returns nonzero it writes
// three 0x20-byte slots from batchNode->vertexCursor, using the xy pairs x0/y0, x1/y1 and x2/y2.
// Each slot gets word0 and word1 in its word0 and word1 fields and color in its color field. No
// perspective divide and no w test.
// FUNCTION: FF7 0x0066896b
void __cdecl Gfx_EmitPsxFlatTriangleScreen(int color, int x0, int y0, int x1, int y1, int x2, int y2, int word0, int word1, GfxRenderObject *batchNode)
{
    GfxFlatQuadScreenVertex *slot0;
    GfxFlatQuadScreenVertex *slot1;
    GfxFlatQuadScreenVertex *slot2;

    if (AcquireBatchNode(1, batchNode) != 0) {
        slot0 = (GfxFlatQuadScreenVertex *)(batchNode->vertexCursor);
        slot0->x = x0;
        slot0->y = y0;
        slot0->z = word0;
        slot0->w = word1;
        slot0->color = color;
        slot1 = (GfxFlatQuadScreenVertex *)(batchNode->vertexCursor + 0x20);
        slot1->x = x1;
        slot1->y = y1;
        slot1->z = word0;
        slot1->w = word1;
        slot1->color = color;
        slot2 = (GfxFlatQuadScreenVertex *)(batchNode->vertexCursor + 0x40);
        slot2->x = x2;
        slot2->y = y2;
        slot2->z = word0;
        slot2->w = word1;
        slot2->color = color;
    }
}

// Integer flat-coloured quad, the four-vertex form of Gfx_EmitPsxFlatTriangleScreen. After
// AcquireBatchNode(1, (int)batchNode) returns nonzero it fills four 0x20-byte slots from
// batchNode->vertexCursor with the xy pairs x0/y0, x1/y1, x2/y2 and x3/y3. Every slot gets word0 and
// word1 in its word0 and word1 fields and color in its color field.
// FUNCTION: FF7 0x00668a30
void __cdecl Gfx_EmitPsxFlatQuadScreen(int color, int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3, int word0, int word1, GfxRenderObject *batchNode)
{
    GfxFlatQuadScreenVertex *slot0;
    GfxFlatQuadScreenVertex *slot1;
    GfxFlatQuadScreenVertex *slot2;
    GfxFlatQuadScreenVertex *slot3;

    if (AcquireBatchNode(1, batchNode) != 0) {
        slot0 = (GfxFlatQuadScreenVertex *)(batchNode->vertexCursor);
        slot0->x = x0;
        slot0->y = y0;
        slot0->z = word0;
        slot0->w = word1;
        slot0->color = color;
        slot1 = (GfxFlatQuadScreenVertex *)(batchNode->vertexCursor + 0x20);
        slot1->x = x1;
        slot1->y = y1;
        slot1->z = word0;
        slot1->w = word1;
        slot1->color = color;
        slot2 = (GfxFlatQuadScreenVertex *)(batchNode->vertexCursor + 0x40);
        slot2->x = x2;
        slot2->y = y2;
        slot2->z = word0;
        slot2->w = word1;
        slot2->color = color;
        slot3 = (GfxFlatQuadScreenVertex *)(batchNode->vertexCursor + 0x60);
        slot3->x = x3;
        slot3->y = y3;
        slot3->z = word0;
        slot3->w = word1;
        slot3->color = color;
    }
}

/*
 * For three positive homogeneous W vertices and acquired batch node writes three 32-byte entries
 * containing XYZ/W, reciprocal W and individual color0/color1/color2 packed fields. Remaining fields
 * untouched; failed acquisition or invalid W causes no explicit writes.
 */
// FUNCTION: FF7 0x00668b2d
void __cdecl Gfx_EmitPsxGouraudTriangleProjected(int color0, int color1, int color2, GfxHomogeneousVec4 *vertex0, GfxHomogeneousVec4 *vertex1, GfxHomogeneousVec4 *vertex2, GfxRenderObject *batchNode)
{
    GfxPsxPolylineSlot *slot0;
    float invW0;
    GfxPsxPolylineSlot *slot1;
    float invW1;
    GfxPsxPolylineSlot *slot2;
    float invW2;

    if (vertex0->w > 0.0 && vertex1->w > 0.0 && vertex2->w > 0.0) {
        if (AcquireBatchNode(1, batchNode) != 0) {
            invW0 = 1.0f / vertex0->w;
            slot0 = (GfxPsxPolylineSlot *)(batchNode->vertexCursor);
            slot0->x = vertex0->x * invW0;
            slot0->y = vertex0->y * invW0;
            slot0->z = vertex0->z * invW0;
            slot0->invW = invW0;
            slot0->color = color0;
            invW1 = 1.0f / vertex1->w;
            slot1 = (GfxPsxPolylineSlot *)(batchNode->vertexCursor + 0x20);
            slot1->x = vertex1->x * invW1;
            slot1->y = vertex1->y * invW1;
            slot1->z = vertex1->z * invW1;
            slot1->invW = invW1;
            slot1->color = color1;
            invW2 = 1.0f / vertex2->w;
            slot2 = (GfxPsxPolylineSlot *)(batchNode->vertexCursor + 0x40);
            slot2->x = vertex2->x * invW2;
            slot2->y = vertex2->y * invW2;
            slot2->z = vertex2->z * invW2;
            slot2->invW = invW2;
            slot2->color = color2;
        }
    }
}

/*
 * Requires positive fourth float components for all four input vertices and successful batch
 * acquisition in mode one. Writes four 32-byte records at context offset 112 in input order, each
 * containing XYZ divided by its fourth component, its reciprocal, and its corresponding dword
 * payload from color0 through color3. Leaves fields beyond offset 16 unchanged and writes no output
 * when a guard fails.
 */
// FUNCTION: FF7 0x00668c97
void __cdecl Gfx_EmitPsxGouraudQuadProjected(int color0, int color1, int color2, int color3, GfxHomogeneousVec4 *vertex0, GfxHomogeneousVec4 *vertex1, GfxHomogeneousVec4 *vertex2, GfxHomogeneousVec4 *vertex3, GfxRenderObject *batchNode)
{
    GfxPsxPolylineSlot *slot0;
    float invW0;
    GfxPsxPolylineSlot *slot1;
    float invW1;
    GfxPsxPolylineSlot *slot2;
    float invW2;
    GfxPsxPolylineSlot *slot3;
    float invW3;

    if (vertex0->w > 0.0 && vertex1->w > 0.0 && vertex2->w > 0.0 && vertex3->w > 0.0) {
        if (AcquireBatchNode(1, batchNode) != 0) {
            invW0 = 1.0f / vertex0->w;
            slot0 = (GfxPsxPolylineSlot *)(batchNode->vertexCursor);
            slot0->x = vertex0->x * invW0;
            slot0->y = vertex0->y * invW0;
            slot0->z = vertex0->z * invW0;
            slot0->invW = invW0;
            slot0->color = color0;
            invW1 = 1.0f / vertex1->w;
            slot1 = (GfxPsxPolylineSlot *)(batchNode->vertexCursor + 0x20);
            slot1->x = vertex1->x * invW1;
            slot1->y = vertex1->y * invW1;
            slot1->z = vertex1->z * invW1;
            slot1->invW = invW1;
            slot1->color = color1;
            invW2 = 1.0f / vertex2->w;
            slot2 = (GfxPsxPolylineSlot *)(batchNode->vertexCursor + 0x40);
            slot2->x = vertex2->x * invW2;
            slot2->y = vertex2->y * invW2;
            slot2->z = vertex2->z * invW2;
            slot2->invW = invW2;
            slot2->color = color2;
            invW3 = 1.0f / vertex3->w;
            slot3 = (GfxPsxPolylineSlot *)(batchNode->vertexCursor + 0x60);
            slot3->x = vertex3->x * invW3;
            slot3->y = vertex3->y * invW3;
            slot3->z = vertex3->z * invW3;
            slot3->invW = invW3;
            slot3->color = color3;
        }
    }
}

// Integer gouraud triangle. After AcquireBatchNode(1, (int)batchNode) returns nonzero it writes three
// 0x20-byte slots from batchNode->vertexCursor with the xy pairs x0/y0, x1/y1 and x2/y2. Each
// vertex takes its colour from color0, color1 or color2 in its color field.z and word1 are
// shared by all three slots.
// FUNCTION: FF7 0x00668e70
void __cdecl Gfx_EmitPsxGouraudTriangleScreen(int color0, int color1, int color2, int x0, int y0, int x1, int y1, int x2, int y2, int word0, int word1, GfxRenderObject *batchNode)
{
    GfxFlatQuadScreenVertex *slot0;
    GfxFlatQuadScreenVertex *slot1;
    GfxFlatQuadScreenVertex *slot2;

    if (AcquireBatchNode(1, batchNode) != 0) {
        slot0 = (GfxFlatQuadScreenVertex *)(batchNode->vertexCursor);
        slot0->x = x0;
        slot0->y = y0;
        slot0->z = word0;
        slot0->w = word1;
        slot0->color = color0;
        slot1 = (GfxFlatQuadScreenVertex *)(batchNode->vertexCursor + 0x20);
        slot1->x = x1;
        slot1->y = y1;
        slot1->z = word0;
        slot1->w = word1;
        slot1->color = color1;
        slot2 = (GfxFlatQuadScreenVertex *)(batchNode->vertexCursor + 0x40);
        slot2->x = x2;
        slot2->y = y2;
        slot2->z = word0;
        slot2->w = word1;
        slot2->color = color2;
    }
}

// Integer gouraud quad. After AcquireBatchNode(1, (int)batchNode) returns nonzero it writes four
// 0x20-byte slots from batchNode->vertexCursor with the xy pairs x0/y0 through x3/y3. Each
// vertex takes its colour from color0 through color3 in its color field. The single pair
// word0/word1 is copied into every slot.
// FUNCTION: FF7 0x00668f35
void __cdecl Gfx_EmitPsxGouraudQuadScreen(int color0, int color1, int color2, int color3, int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3, int word0, int word1, GfxRenderObject *batchNode)
{
    GfxFlatQuadScreenVertex *slot0;
    GfxFlatQuadScreenVertex *slot1;
    GfxFlatQuadScreenVertex *slot2;
    GfxFlatQuadScreenVertex *slot3;

    if (AcquireBatchNode(1, batchNode) != 0) {
        slot0 = (GfxFlatQuadScreenVertex *)(batchNode->vertexCursor);
        slot0->x = x0;
        slot0->y = y0;
        slot0->z = word0;
        slot0->w = word1;
        slot0->color = color0;
        slot1 = (GfxFlatQuadScreenVertex *)(batchNode->vertexCursor + 0x20);
        slot1->x = x1;
        slot1->y = y1;
        slot1->z = word0;
        slot1->w = word1;
        slot1->color = color1;
        slot2 = (GfxFlatQuadScreenVertex *)(batchNode->vertexCursor + 0x40);
        slot2->x = x2;
        slot2->y = y2;
        slot2->z = word0;
        slot2->w = word1;
        slot2->color = color2;
        slot3 = (GfxFlatQuadScreenVertex *)(batchNode->vertexCursor + 0x60);
        slot3->x = x3;
        slot3->y = y3;
        slot3->z = word0;
        slot3->w = word1;
        slot3->color = color3;
    }
}

// Gouraud quad with one word pair per vertex, otherwise identical to Gfx_EmitPsxGouraudQuadScreen.
// After AcquireBatchNode(1, (int)batchNode) returns nonzero it writes four 0x20-byte slots from
// batchNode->vertexCursor with the xy pairs x0/y0 through x3/y3 and the colours color0 through color3.
// The first vertex takes word0/word1, the second word2/word3, the third word4/word5 and the fourth
// word6/word7, stored in each vertex's word0 and word1 fields.
// FUNCTION: FF7 0x00669032
void __cdecl Gfx_EmitPsxGouraudQuadScreenPerVertexBlend(int color0, int color1, int color2, int color3, int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3, int word0, int word1, int word2, int word3, int word4, int word5, int word6, int word7, GfxRenderObject *batchNode)
{
    GfxFlatQuadScreenVertex *slot0;
    GfxFlatQuadScreenVertex *slot1;
    GfxFlatQuadScreenVertex *slot2;
    GfxFlatQuadScreenVertex *slot3;

    if (AcquireBatchNode(1, batchNode) != 0) {
        slot0 = (GfxFlatQuadScreenVertex *)(batchNode->vertexCursor);
        slot0->x = x0;
        slot0->y = y0;
        slot0->z = word0;
        slot0->w = word1;
        slot0->color = color0;
        slot1 = (GfxFlatQuadScreenVertex *)(batchNode->vertexCursor + 0x20);
        slot1->x = x1;
        slot1->y = y1;
        slot1->z = word2;
        slot1->w = word3;
        slot1->color = color1;
        slot2 = (GfxFlatQuadScreenVertex *)(batchNode->vertexCursor + 0x40);
        slot2->x = x2;
        slot2->y = y2;
        slot2->z = word4;
        slot2->w = word5;
        slot2->color = color2;
        slot3 = (GfxFlatQuadScreenVertex *)(batchNode->vertexCursor + 0x60);
        slot3->x = x3;
        slot3->y = y3;
        slot3->z = word6;
        slot3->w = word7;
        slot3->color = color3;
    }
}

// Emits a three-vertex primitive from the global clip point g_GfxPsxClipVertex1. Nothing is written unless w is greater than 0.0 and
// AcquireBatchNode(1, (int)batchNode) returns nonzero. The point is divided by w once, and the result is
// written into the slot at batchNode->vertexCursor. The slots at +0x20 and +0x40 hold the same
// point with x + 1.0f and with y + 1.0f. All three store 1/w in invW and the colour in color.
// FUNCTION: FF7 0x0066912f
void __cdecl Gfx_EmitPsxDotTriangleProjected(int color, GfxRenderObject *batchNode)
{
    float invW;
    float scaledX;
    float scaledY;
    float z;
    GfxPsxPolylineSlot *dst0;
    GfxPsxPolylineSlot *record2;
    GfxPsxPolylineSlot *dst1;

    if (g_GfxPsxClipVertex1.w > 0.0) {
        invW = 1.0f / g_GfxPsxClipVertex1.w;
        if (AcquireBatchNode(1, batchNode) != 0) {
            scaledX = g_GfxPsxClipVertex1.x * invW;
            scaledY = g_GfxPsxClipVertex1.y * invW;
            z = g_GfxPsxClipVertex1.z * invW;
            dst0 = (GfxPsxPolylineSlot *)(batchNode->vertexCursor);
            dst0->x = scaledX;
            dst0->y = scaledY;
            dst0->z = z;
            dst0->invW = invW;
            dst0->color = color;
            dst1 = (GfxPsxPolylineSlot *)(batchNode->vertexCursor + 0x20);
            dst1->x = scaledX + 1.0f;
            dst1->y = scaledY;
            dst1->z = z;
            dst1->invW = invW;
            dst1->color = color;
            record2 = (GfxPsxPolylineSlot *)(batchNode->vertexCursor + 0x40);
            record2->x = scaledX;
            record2->y = scaledY + 1.0f;
            record2->z = z;
            record2->invW = invW;
            record2->color = color;
        }
    }
}

// Integer form of Gfx_EmitPsxDotTriangleProjected. It converts x and y to float and, after a
// nonzero AcquireBatchNode(1, (int)batchNode), writes three 0x20-byte slots from batchNode->vertexCursor
// holding (x, y), (x + 1.0f, y) and (x, y + 1.0f). Each slot gets word0, word1 and color in
// its word0, word1 and color fields.
// FUNCTION: FF7 0x0066924a
void __cdecl Gfx_EmitPsxDotTriangleScreen(int x, int y, int word0, int word1, int color, GfxRenderObject *batchNode)
{
    GfxFlatRectScreenCorner *slot0;
    float floatY;
    float xFloat;
    GfxFlatRectScreenCorner *slot1;
    GfxFlatRectScreenCorner *slot2;

    if (AcquireBatchNode(1, batchNode) != 0) {
        xFloat = x;
        floatY = y;
        slot0 = (GfxFlatRectScreenCorner *)(batchNode->vertexCursor);
        slot0->x = xFloat;
        slot0->y = floatY;
        slot0->z = word0;
        slot0->w = word1;
        slot0->color = color;
        slot1 = (GfxFlatRectScreenCorner *)(batchNode->vertexCursor + 0x20);
        slot1->x = xFloat + 1.0f;
        slot1->y = floatY;
        slot1->z = word0;
        slot1->w = word1;
        slot1->color = color;
        slot2 = (GfxFlatRectScreenCorner *)(batchNode->vertexCursor + 0x40);
        slot2->x = xFloat;
        slot2->y = floatY + 1.0f;
        slot2->z = word0;
        slot2->w = word1;
        slot2->color = color;
    }
}

/*
 * Computes integer x+width and y+height before float conversion. On successful AcquireBatchNode
 * writes four stride 0x20 vertices for(x, y), (x+width, y), (x, y+height), (x+width, y+height) with
 * common raw fields word0, word1 and color color. Does nothing on acquisition failure.
 */
// FUNCTION: FF7 0x00669327
void __cdecl Gfx_EmitPsxFlatRectScreen(int x, int y, int width, int height, int word0, int word1, int color, GfxRenderObject *batchNode)
{
    float floatBottom;
    float floatY;
    float floatX;
    float rightF;
    GfxFlatRectScreenCorner *dst0;
    GfxFlatRectScreenCorner *record2;
    GfxFlatRectScreenCorner *dst1;
    GfxFlatRectScreenCorner *record3;

    floatX = (float)x;
    floatY = (float)y;
    rightF = (float)(x + width);
    floatBottom = (float)(y + height);
    if (AcquireBatchNode(1, batchNode) != 0) {
        dst0 = (GfxFlatRectScreenCorner *)(batchNode->vertexCursor);
        dst0->x = floatX;
        dst0->y = floatY;
        dst0->z = word0;
        dst0->w = word1;
        dst0->color = color;
        dst1 = (GfxFlatRectScreenCorner *)(batchNode->vertexCursor + 0x20);
        dst1->x = rightF;
        dst1->y = floatY;
        dst1->z = word0;
        dst1->w = word1;
        dst1->color = color;
        record2 = (GfxFlatRectScreenCorner *)(batchNode->vertexCursor + 0x40);
        record2->x = floatX;
        record2->y = floatBottom;
        record2->z = word0;
        record2->w = word1;
        record2->color = color;
        record3 = (GfxFlatRectScreenCorner *)(batchNode->vertexCursor + 0x60);
        record3->x = rightF;
        record3->y = floatBottom;
        record3->z = word0;
        record3->w = word1;
        record3->color = color;
    }
}

/*
 * When three input vertices have positive fourth float components and mode-one batch acquisition
 * succeeds, writes three 32-byte records containing XYZ divided by the fourth component, its
 * reciprocal, shared payload payload, and integer texture coordinates scaled by context fields 36 and
 * 40. Leaves offset 20 unchanged. Stores narrowed attr through the metadata pointer and full
 * attr at context offset 124. Failed guards leave output untouched.
 */
// FUNCTION: FF7 0x0066944e
void __cdecl Gfx_EmitPsxTexturedFlatTriangleProjected(int payload, GfxHomogeneousVec4 *vertex0, GfxHomogeneousVec4 *vertex1, GfxHomogeneousVec4 *vertex2, int texU0, int texV0, int texU1, int texV1, int texU2, int texV2, int attr, GfxPsxTexturedFlatTriangle *batch)
{
    GfxPsxProjectedVertex *pVertex0Out;
    float invW0;
    float invW1;
    GfxPsxProjectedVertex *pRecord1;
    float subInvWidth;
    GfxPsxProjectedVertex *record2;

    if (vertex0->w > 0.0 && vertex1->w > 0.0 && vertex2->w > 0.0) {
        if (AcquireBatchNode(1, batch) != 0) {
            invW0 = 1.0f / vertex0->w;
            pVertex0Out = (GfxPsxProjectedVertex *)(batch->vertices);
            pVertex0Out->x = vertex0->x * invW0;
            pVertex0Out->y = vertex0->y * invW0;
            pVertex0Out->z = vertex0->z * invW0;
            pVertex0Out->invW = invW0;
            pVertex0Out->payload = payload;
            pVertex0Out->texU = texU0 * batch->scaleU;
            pVertex0Out->texV = texV0 * batch->scaleV;
            invW1 = 1.0f / vertex1->w;
            pRecord1 = (GfxPsxProjectedVertex *)(batch->vertices + 0x20);
            pRecord1->x = vertex1->x * invW1;
            pRecord1->y = vertex1->y * invW1;
            pRecord1->z = vertex1->z * invW1;
            pRecord1->invW = invW1;
            pRecord1->payload = payload;
            pRecord1->texU = texU1 * batch->scaleU;
            pRecord1->texV = texV1 * batch->scaleV;
            subInvWidth = 1.0f / vertex2->w;
            record2 = (GfxPsxProjectedVertex *)(batch->vertices + 0x40);
            record2->x = vertex2->x * subInvWidth;
            record2->y = vertex2->y * subInvWidth;
            record2->z = vertex2->z * subInvWidth;
            record2->invW = subInvWidth;
            record2->payload = payload;
            record2->texU = texU2 * batch->scaleU;
            record2->texV = texV2 * batch->scaleV;
            *batch->packetPtr = attr;
            batch->attr = attr;
        }
    }
}

// Writes a single vertex into an explicitly indexed slot, with no reservation call and no w test.
// The destination is context->vertexCursor + (index << 5), so index counts 0x20-byte records. It
// stores vertex's x, y and z each multiplied by invW = 1.0f / vertex->w, unscaled raw words aside.
// Then it stores invW, payload, and raw0 and raw1 unscaled into texU and texV, unlike the batch
// emitters around it.
// FUNCTION: FF7 0x00669626
void __cdecl Gfx_UnusedWriteProjectedVertexAtIndex(GfxHomogeneousVec4 *vertex, int payload, int raw0, int raw1, int index, GfxRenderObject *context)
{
    float invW;
    GfxPsxVertexIndexed *dst;

    invW = 1.0f / vertex->w;
    dst = (GfxPsxVertexIndexed *)(context->vertexCursor + (index << 5));
    dst->x = vertex->x * invW;
    dst->y = vertex->y * invW;
    dst->z = vertex->z * invW;
    dst->invW = invW;
    dst->payload = payload;
    dst->texU = raw0;
    dst->texV = raw1;
}

/*
 * Requires positive fourth components for four float vertices and successful mode-one batch
 * acquisition. Writes four 32-byte records with divided XYZ, reciprocal, shared payload payload, and
 * corresponding integer texture-coordinate pairs scaled by context fields 36 and 40. Leaves offset
 * 20 untouched and writes narrowed attr through metadata plus full attr at context offset 124.
 * Failed guards produce no output writes.
 */
// FUNCTION: FF7 0x0066969f
void __cdecl Gfx_EmitPsxTexturedFlatQuadProjected(int payload, GfxHomogeneousVec4 *vertex0, GfxHomogeneousVec4 *vertex1, GfxHomogeneousVec4 *vertex2, GfxHomogeneousVec4 *vertex3, int texU0, int texV0, int texU1, int texV1, int texU2, int texV2, int texU3, int texV3, int attr, GfxTexturedFlatQuadProjection *batch)
{
    GfxPsxProjectedVertex *pVertex0Out;
    float invW0;
    float invW1;
    GfxPsxProjectedVertex *pRecord1;
    float localReciprocal;
    GfxPsxProjectedVertex *record2Ptr;
    float subInvWidth;
    GfxPsxProjectedVertex *calcRecord;

    if (vertex0->w > 0.0 && vertex1->w > 0.0 && vertex2->w > 0.0 && vertex3->w > 0.0) {
        if (AcquireBatchNode(1, batch) != 0) {
            invW0 = 1.0f / vertex0->w;
            pVertex0Out = (GfxPsxProjectedVertex *)batch->vertexBuffer;
            pVertex0Out->x = vertex0->x * invW0;
            pVertex0Out->y = vertex0->y * invW0;
            pVertex0Out->z = vertex0->z * invW0;
            pVertex0Out->invW = invW0;
            pVertex0Out->payload = payload;
            pVertex0Out->texU = texU0 * batch->texScaleU;
            pVertex0Out->texV = texV0 * batch->texScaleV;
            invW1 = 1.0f / vertex1->w;
            pRecord1 = (GfxPsxProjectedVertex *)(batch->vertexBuffer + 0x20);
            pRecord1->x = vertex1->x * invW1;
            pRecord1->y = vertex1->y * invW1;
            pRecord1->z = vertex1->z * invW1;
            pRecord1->invW = invW1;
            pRecord1->payload = payload;
            pRecord1->texU = texU1 * batch->texScaleU;
            pRecord1->texV = texV1 * batch->texScaleV;
            subInvWidth = 1.0f / vertex2->w;
            record2Ptr = (GfxPsxProjectedVertex *)(batch->vertexBuffer + 0x40);
            record2Ptr->x = vertex2->x * subInvWidth;
            record2Ptr->y = vertex2->y * subInvWidth;
            record2Ptr->z = vertex2->z * subInvWidth;
            record2Ptr->invW = subInvWidth;
            record2Ptr->payload = payload;
            record2Ptr->texU = texU2 * batch->texScaleU;
            record2Ptr->texV = texV2 * batch->texScaleV;
            localReciprocal = 1.0f / vertex3->w;
            calcRecord = (GfxPsxProjectedVertex *)(batch->vertexBuffer + 0x60);
            calcRecord->x = vertex3->x * localReciprocal;
            calcRecord->y = vertex3->y * localReciprocal;
            calcRecord->z = vertex3->z * localReciprocal;
            calcRecord->invW = localReciprocal;
            calcRecord->payload = payload;
            calcRecord->texU = texU3 * batch->texScaleU;
            calcRecord->texV = texV3 * batch->texScaleV;
            *batch->attrDest = attr;
            batch->attr = attr;
        }
    }
}

/*
 * Acquired batch node receives three 32-byte vertex records with shared raw
 * depth/reciprocal/packed fields, individual XY and UVs scaled by context factors. Writes attr
 * narrowed to metadata byte and full dword context field. Failure causes no explicit writes;
 * record field 0x14 untouched.
 */
// FUNCTION: FF7 0x00669904
void __cdecl Gfx_EmitPsxTexturedFlatTriangleScreen(int payload, int x0, int y0, int x1, int y1, int x2, int y2, int texU0, int texV0, int texU1, int texV1, int texU2, int texV2, int attr, int depth, int reciprocal, GfxPsxTexturedFlatTriangleState *batch)
{
    GfxPsxScreenVertex *record0Ptr;
    GfxPsxScreenVertex *pRecord1;
    GfxPsxScreenVertex *record2;

    if (AcquireBatchNode(1, batch) != 0) {
        record0Ptr = (GfxPsxScreenVertex *)(batch->vertexBuffer);
        record0Ptr->x = x0;
        record0Ptr->y = y0;
        record0Ptr->depth = depth;
        record0Ptr->reciprocal = reciprocal;
        record0Ptr->payload = payload;
        record0Ptr->texU = texU0 * batch->texScaleU;
        record0Ptr->texV = texV0 * batch->texScaleV;
        pRecord1 = (GfxPsxScreenVertex *)(batch->vertexBuffer + 0x20);
        pRecord1->x = x1;
        pRecord1->y = y1;
        pRecord1->depth = depth;
        pRecord1->reciprocal = reciprocal;
        pRecord1->payload = payload;
        pRecord1->texU = texU1 * batch->texScaleU;
        pRecord1->texV = texV1 * batch->texScaleV;
        record2 = (GfxPsxScreenVertex *)(batch->vertexBuffer + 0x40);
        record2->x = x2;
        record2->y = y2;
        record2->depth = depth;
        record2->reciprocal = reciprocal;
        record2->payload = payload;
        record2->texU = texU2 * batch->texScaleU;
        record2->texV = texV2 * batch->texScaleV;
        *batch->attrPtr = attr;
        batch->attr = attr;
    }
}

/*
 * After successful mode-one batch acquisition, writes four 32-byte vertex records with four
 * supplied raw XY dword pairs, shared depth and reciprocal fields, and shared payload payload. Multiplies
 * four supplied integer texture-coordinate pairs by context scale fields 36 and 40 and stores the
 * resulting floats at offsets 24 and 28. Leaves offset 20 unchanged. Writes the low byte of attr
 * to the metadata pointer and full attr to context offset 124. Acquisition failure leaves output
 * untouched.
 */
// FUNCTION: FF7 0x00669a37
void __cdecl Gfx_EmitPsxTexturedFlatQuadScreen(int payload, int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3, int texU0, int texV0, int texU1, int texV1, int texU2, int texV2, int texU3, int texV3, int attr, int depth, int reciprocal, GfxPsxQuadEmitter *emitter)
{
    GfxPsxScreenVertex *record0Ptr;
    GfxPsxScreenVertex *pRecord1;
    GfxPsxScreenVertex *record2;
    GfxPsxScreenVertex *record3;

    if (AcquireBatchNode(1, emitter) != 0) {
        record0Ptr = (GfxPsxScreenVertex *)(emitter->primBuffer);
        record0Ptr->x = x0;
        record0Ptr->y = y0;
        record0Ptr->depth = depth;
        record0Ptr->reciprocal = reciprocal;
        record0Ptr->payload = payload;
        record0Ptr->texU = texU0 * emitter->scaleX;
        record0Ptr->texV = texV0 * emitter->scaleY;
        pRecord1 = (GfxPsxScreenVertex *)(emitter->primBuffer + 0x20);
        pRecord1->x = x1;
        pRecord1->y = y1;
        pRecord1->depth = depth;
        pRecord1->reciprocal = reciprocal;
        pRecord1->payload = payload;
        pRecord1->texU = texU1 * emitter->scaleX;
        pRecord1->texV = texV1 * emitter->scaleY;
        record2 = (GfxPsxScreenVertex *)(emitter->primBuffer + 0x40);
        record2->x = x2;
        record2->y = y2;
        record2->depth = depth;
        record2->reciprocal = reciprocal;
        record2->payload = payload;
        record2->texU = texU2 * emitter->scaleX;
        record2->texV = texV2 * emitter->scaleY;
        record3 = (GfxPsxScreenVertex *)(emitter->primBuffer + 0x60);
        record3->x = x3;
        record3->y = y3;
        record3->depth = depth;
        record3->reciprocal = reciprocal;
        record3->payload = payload;
        record3->texU = texU3 * emitter->scaleX;
        record3->texV = texV3 * emitter->scaleY;
        *(char *)emitter->packetPtr = attr;
        emitter->attr = attr;
    }
}

/*
 * Requires positive fourth float components for three vertices and successful mode-one batch
 * acquisition. Writes three 32-byte records with XYZ divided by the fourth component, reciprocal,
 * corresponding dword payloads payload0 through payload2, and corresponding integer texture-coordinate
 * pairs scaled by context fields 36 and 40. Leaves record offset 20 untouched. Writes the low byte
 * of attr through the metadata pointer and full attr at context offset 124. No output writes
 * occur when a guard fails.
 */
// FUNCTION: FF7 0x00669bc0
void __cdecl Gfx_EmitPsxTexturedGouraudTriangleProjected(int payload0, int payload1, int payload2, GfxHomogeneousVec4 *vertex0, GfxHomogeneousVec4 *vertex1, GfxHomogeneousVec4 *vertex2, int texU0, int texV0, int texU1, int texV1, int texU2, int texV2, int attr, GfxPsxTexturedFlatTriangle *batch)
{
    GfxPsxProjectedVertex *pVertex0Out;
    float invW0;
    float invW1;
    GfxPsxProjectedVertex *pRecord1;
    float subInvWidth;
    GfxPsxProjectedVertex *record2;

    if (vertex0->w > 0.0 && vertex1->w > 0.0 && vertex2->w > 0.0) {
        if (AcquireBatchNode(1, batch) != 0) {
            invW0 = 1.0f / vertex0->w;
            pVertex0Out = (GfxPsxProjectedVertex *)(batch->vertices);
            pVertex0Out->x = vertex0->x * invW0;
            pVertex0Out->y = vertex0->y * invW0;
            pVertex0Out->z = vertex0->z * invW0;
            pVertex0Out->invW = invW0;
            pVertex0Out->payload = payload0;
            pVertex0Out->texU = texU0 * batch->scaleU;
            pVertex0Out->texV = texV0 * batch->scaleV;
            invW1 = 1.0f / vertex1->w;
            pRecord1 = (GfxPsxProjectedVertex *)(batch->vertices + 0x20);
            pRecord1->x = vertex1->x * invW1;
            pRecord1->y = vertex1->y * invW1;
            pRecord1->z = vertex1->z * invW1;
            pRecord1->invW = invW1;
            pRecord1->payload = payload1;
            pRecord1->texU = texU1 * batch->scaleU;
            pRecord1->texV = texV1 * batch->scaleV;
            subInvWidth = 1.0f / vertex2->w;
            record2 = (GfxPsxProjectedVertex *)(batch->vertices + 0x40);
            record2->x = vertex2->x * subInvWidth;
            record2->y = vertex2->y * subInvWidth;
            record2->z = vertex2->z * subInvWidth;
            record2->invW = subInvWidth;
            record2->payload = payload2;
            record2->texU = texU2 * batch->scaleU;
            record2->texV = texV2 * batch->scaleV;
            *batch->packetPtr = attr;
            batch->attr = attr;
        }
    }
}

/*
 * Requires four positive fourth float components and successful mode-one batch acquisition. Writes
 * four 32-byte vertex records with divided XYZ, reciprocal, per-record dword payloads payload0 through
 * payload3, and integer texture-coordinate pairs scaled by context fields 36 and 40. Leaves offset 20
 * untouched and transfers attr to metadata as both a byte and a full dword. Failed guards leave
 * output unchanged.
 */
// FUNCTION: FF7 0x00669d98
void __cdecl Gfx_EmitPsxTexturedGouraudQuadProjected(int payload0, int payload1, int payload2, int payload3, GfxHomogeneousVec4 *vertex0, GfxHomogeneousVec4 *vertex1, GfxHomogeneousVec4 *vertex2, GfxHomogeneousVec4 *vertex3, int texU0, int texV0, int texU1, int texV1, int texU2, int texV2, int texU3, int texV3, int attr, GfxTexturedFlatQuadProjection *batch)
{
    GfxPsxProjectedVertex *pVertex0Out;
    float invW0;
    float invW1;
    GfxPsxProjectedVertex *pRecord1;
    float localReciprocal;
    GfxPsxProjectedVertex *record2Ptr;
    float subInvWidth;
    GfxPsxProjectedVertex *calcRecord;

    if (vertex0->w > 0.0 && vertex1->w > 0.0 && vertex2->w > 0.0 && vertex3->w > 0.0) {
        if (AcquireBatchNode(1, batch) != 0) {
            invW0 = 1.0f / vertex0->w;
            pVertex0Out = (GfxPsxProjectedVertex *)batch->vertexBuffer;
            pVertex0Out->x = vertex0->x * invW0;
            pVertex0Out->y = vertex0->y * invW0;
            pVertex0Out->z = vertex0->z * invW0;
            pVertex0Out->invW = invW0;
            pVertex0Out->payload = payload0;
            pVertex0Out->texU = texU0 * batch->texScaleU;
            pVertex0Out->texV = texV0 * batch->texScaleV;
            invW1 = 1.0f / vertex1->w;
            pRecord1 = (GfxPsxProjectedVertex *)(batch->vertexBuffer + 0x20);
            pRecord1->x = vertex1->x * invW1;
            pRecord1->y = vertex1->y * invW1;
            pRecord1->z = vertex1->z * invW1;
            pRecord1->invW = invW1;
            pRecord1->payload = payload1;
            pRecord1->texU = texU1 * batch->texScaleU;
            pRecord1->texV = texV1 * batch->texScaleV;
            subInvWidth = 1.0f / vertex2->w;
            record2Ptr = (GfxPsxProjectedVertex *)(batch->vertexBuffer + 0x40);
            record2Ptr->x = vertex2->x * subInvWidth;
            record2Ptr->y = vertex2->y * subInvWidth;
            record2Ptr->z = vertex2->z * subInvWidth;
            record2Ptr->invW = subInvWidth;
            record2Ptr->payload = payload2;
            record2Ptr->texU = texU2 * batch->texScaleU;
            record2Ptr->texV = texV2 * batch->texScaleV;
            localReciprocal = 1.0f / vertex3->w;
            calcRecord = (GfxPsxProjectedVertex *)(batch->vertexBuffer + 0x60);
            calcRecord->x = vertex3->x * localReciprocal;
            calcRecord->y = vertex3->y * localReciprocal;
            calcRecord->z = vertex3->z * localReciprocal;
            calcRecord->invW = localReciprocal;
            calcRecord->payload = payload3;
            calcRecord->texU = texU3 * batch->texScaleU;
            calcRecord->texV = texV3 * batch->texScaleV;
            *batch->attrDest = attr;
            batch->attr = attr;
        }
    }
}

/*
 * Acquired batch node receives three 32-byte entries with individual raw XY/colors, shared
 * depth/reciprocal and UVs scaled by context factors. Writes metadata attr as byte and full
 * dword; field 0x14 untouched and no explicit writes on acquisition failure.
 */
// FUNCTION: FF7 0x00669ffd
void __cdecl Gfx_EmitPsxTexturedGouraudTriangleScreen(int payload0, int payload1, int payload2, int x0, int y0, int x1, int y1, int x2, int y2, int texU0, int texV0, int texU1, int texV1, int texU2, int texV2, int attr, int depth, int reciprocal, GfxPsxTexturedFlatTriangleState *batch)
{
    GfxPsxScreenVertex *record0Ptr;
    GfxPsxScreenVertex *pRecord1;
    GfxPsxScreenVertex *record2;

    if (AcquireBatchNode(1, batch) != 0) {
        record0Ptr = (GfxPsxScreenVertex *)(batch->vertexBuffer);
        record0Ptr->x = x0;
        record0Ptr->y = y0;
        record0Ptr->depth = depth;
        record0Ptr->reciprocal = reciprocal;
        record0Ptr->payload = payload0;
        record0Ptr->texU = texU0 * batch->texScaleU;
        record0Ptr->texV = texV0 * batch->texScaleV;
        pRecord1 = (GfxPsxScreenVertex *)(batch->vertexBuffer + 0x20);
        pRecord1->x = x1;
        pRecord1->y = y1;
        pRecord1->depth = depth;
        pRecord1->reciprocal = reciprocal;
        pRecord1->payload = payload1;
        pRecord1->texU = texU1 * batch->texScaleU;
        pRecord1->texV = texV1 * batch->texScaleV;
        record2 = (GfxPsxScreenVertex *)(batch->vertexBuffer + 0x40);
        record2->x = x2;
        record2->y = y2;
        record2->depth = depth;
        record2->reciprocal = reciprocal;
        record2->payload = payload2;
        record2->texU = texU2 * batch->texScaleU;
        record2->texV = texV2 * batch->texScaleV;
        *batch->attrPtr = attr;
        batch->attr = attr;
    }
}

/*
 * After successful mode-one batch acquisition, writes four 32-byte records with four supplied raw
 * XY dword pairs, shared depth and reciprocal fields, and per-record dword payloads payload0 through payload3.
 * Scales four integer texture-coordinate pairs by context fields 36 and 40 and stores float
 * results at offsets 24 and 28, leaving offset 20 untouched. Writes narrowed attr to the metadata
 * pointer and full attr to context offset 124. Acquisition failure leaves the output unchanged.
 */
// FUNCTION: FF7 0x0066a130
void __cdecl Gfx_EmitPsxTexturedGouraudQuadScreen(int payload0, int payload1, int payload2, int payload3, int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3, int texU0, int texV0, int texU1, int texV1, int texU2, int texV2, int texU3, int texV3, int attr, int depth, int reciprocal, GfxPsxQuadState *quadState)
{
    GfxPsxScreenVertex *record0Ptr;
    GfxPsxScreenVertex *pRecord1;
    GfxPsxScreenVertex *record2;
    GfxPsxScreenVertex *record3;

    if (AcquireBatchNode(1, quadState) != 0) {
        record0Ptr = (GfxPsxScreenVertex *)(quadState->packetBase);
        record0Ptr->x = x0;
        record0Ptr->y = y0;
        record0Ptr->depth = depth;
        record0Ptr->reciprocal = reciprocal;
        record0Ptr->payload = payload0;
        record0Ptr->texU = texU0 * quadState->scaleX;
        record0Ptr->texV = texV0 * quadState->scaleY;
        pRecord1 = (GfxPsxScreenVertex *)(quadState->packetBase + 0x20);
        pRecord1->x = x1;
        pRecord1->y = y1;
        pRecord1->depth = depth;
        pRecord1->reciprocal = reciprocal;
        pRecord1->payload = payload1;
        pRecord1->texU = texU1 * quadState->scaleX;
        pRecord1->texV = texV1 * quadState->scaleY;
        record2 = (GfxPsxScreenVertex *)(quadState->packetBase + 0x40);
        record2->x = x2;
        record2->y = y2;
        record2->depth = depth;
        record2->reciprocal = reciprocal;
        record2->payload = payload2;
        record2->texU = texU2 * quadState->scaleX;
        record2->texV = texV2 * quadState->scaleY;
        record3 = (GfxPsxScreenVertex *)(quadState->packetBase + 0x60);
        record3->x = x3;
        record3->y = y3;
        record3->depth = depth;
        record3->reciprocal = reciprocal;
        record3->payload = payload3;
        record3->texU = texU3 * quadState->scaleX;
        record3->texV = texV3 * quadState->scaleY;
        *quadState->packetPtr = attr;
        quadState->attr = attr;
    }
}

/*
 * Computes float rectangle corners from x through height and texture coordinates from texX and
 * texY plus the dimensions, multiplied by context scale fields 36 and 40. After successful mode-
 * one batch acquisition, writes four 32-byte vertices in top-left, top-right, bottom-left, bottom-
 * right coordinate order, with shared raw dwords raw0 through raw2 and paired texture coordinates.
 * Leaves offset 20 of each vertex untouched. Writes narrowed attr to the context metadata pointer
 * and full attr to context offset 124. No output writes occur on acquisition failure.
 */
// FUNCTION: FF7 0x0066a2b9
void __cdecl Gfx_EmitPsxTexturedSpriteQuad(int x, int y, int width, int height, int texX, int texY, int raw0, int raw1, int raw2, int attr, GfxPsxSpriteQuadState *batch)
{
    float texBottom;
    float xRemaining;
    float right;
    float baseYTop;
    float yBottom;
    float textureLeft;
    float texRight;
    GfxTlVertex *dst1;
    GfxTlVertex *pVertex0;
    float textureTop;
    GfxTlVertex *vertex2;
    GfxTlVertex *vertex3;

    textureLeft = texX * batch->scaleX;
    texRight = (texX + width) * batch->scaleX;
    textureTop = texY * batch->scaleY;
    texBottom = (texY + height) * batch->scaleY;
    xRemaining = (float)x;
    baseYTop = (float)y;
    right = (float)(x + width);
    yBottom = (float)(y + height);
    if (AcquireBatchNode(1, batch) != 0) {
        pVertex0 = (GfxTlVertex *)(batch->vertices);
        pVertex0->x = xRemaining;
        pVertex0->y = baseYTop;
        pVertex0->zBits = raw0;
        pVertex0->rhwBits = raw1;
        pVertex0->color = raw2;
        pVertex0->u = textureLeft;
        pVertex0->v = textureTop;
        dst1 = (GfxTlVertex *)(batch->vertices + 0x20);
        dst1->x = right;
        dst1->y = baseYTop;
        dst1->zBits = raw0;
        dst1->rhwBits = raw1;
        dst1->color = raw2;
        dst1->u = texRight;
        dst1->v = textureTop;
        vertex2 = (GfxTlVertex *)(batch->vertices + 0x40);
        vertex2->x = xRemaining;
        vertex2->y = yBottom;
        vertex2->zBits = raw0;
        vertex2->rhwBits = raw1;
        vertex2->color = raw2;
        vertex2->u = textureLeft;
        vertex2->v = texBottom;
        vertex3 = (GfxTlVertex *)(batch->vertices + 0x60);
        vertex3->x = right;
        vertex3->y = yBottom;
        vertex3->zBits = raw0;
        vertex3->rhwBits = raw1;
        vertex3->color = raw2;
        vertex3->u = texRight;
        vertex3->v = texBottom;
        *(char *)batch->packetPtr = attr;
        batch->attr = attr;
    }
}

/*
 * Dispatches a PSX primitive by its command byte shifted right by 2 after selecting an enabled
 * render table and alpha from translucency/global state. Packs flat or per-vertex RGB/alpha and
 * calls type-specific line, triangle, quad or rectangle helpers, using shared projected
 * coordinates or explicit signed-short coordinates according to the coordinate-mode flag. Textured
 * types resolve texture/page state and UV bytes before submission; fixed rectangles use 8 or 16
 * dimensions. One four-color explicit quad consumes an extended-coordinate flag only on that path.
 * Unsupported commands or unavailable render tables draw nothing, but every call resets coordinate
 * mode, blend mode and two render-table override flags.
 */
// FUNCTION: FF7 0x0066a47e
void __cdecl Gfx_DrawPsxPrimitive(void *primitive)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[152];
        GfxPsxSprite *sprite16;
        struct { unsigned char pad_to_4[0x4]; GfxPsxSprite *sprite8; };
        struct { unsigned char pad_to_8[0x8]; GfxPsxTexturedSprite *texSprite; };
        struct { unsigned char pad_to_C[0xC]; GfxPsxTexturedQuadSource *texGouraudQuad; };
        struct { unsigned char pad_to_10[0x10]; GfxPsxTexturedTriangle *texTri; };
        struct { unsigned char pad_to_14[0x14]; GfxPsxTexturedQuad *texQuad; };
        struct { unsigned char pad_to_18[0x18]; GfxPsxPrimitive *texFlatTri; };
        struct { unsigned char pad_to_1C[0x1C]; GfxPsxRect8Record *rect16; };
        struct { unsigned char pad_to_20[0x20]; GfxPsxRect8Record *rect8; };
        struct { unsigned char pad_to_24[0x24]; GfxPsxRect8Record *dotRec; };
        struct { unsigned char pad_to_28[0x28]; GfxPsxRectPrimitive *rectPrim; };
        struct { unsigned char pad_to_2C[0x2C]; GfxPsxGouraudQuad *gouraudQuad; };
        struct { unsigned char pad_to_30[0x30]; GfxPsxGouraudPolylineVerts *gouraudTri; };
        struct { unsigned char pad_to_34[0x34]; GfxPsxFlatQuadPrim *flatQuad; };
        struct { unsigned char pad_to_38[0x38]; GfxPsxFlatPrimitive *flatPrim; };
        struct { unsigned char pad_to_3C[0x3C]; GfxPsxGouraudPolylineVerts *gouraudPolyline; };
        struct { unsigned char pad_to_40[0x40]; GfxPsxGouraudLine *gouraudLine; };
        struct { unsigned char pad_to_44[0x44]; GfxPsxLinePrimitive *linePrim; };
        struct { unsigned char pad_to_48[0x48]; int primFlags; };
        struct { unsigned char pad_to_4C[0x4C]; union { int packed; GfxBgraPixel bgra; } color2; };
        struct { unsigned char pad_to_50[0x50]; union { int packed; GfxBgraPixel bgra; } color3; };
        struct { unsigned char pad_to_54[0x54]; int extCoordFlag; };
        struct { unsigned char pad_to_58[0x58]; int clutY; };
        struct { unsigned char pad_to_5C[0x5C]; union { int packed; GfxBgraPixel bgra; } color0; };
        struct { unsigned char pad_to_60[0x60]; int primKind; };
        struct { unsigned char pad_to_64[0x64]; int blendFirst; };
        struct { unsigned char pad_to_68[0x68]; union { int packed; GfxBgraPixel bgra; } color1; };
        struct { unsigned char pad_to_6C[0x6C]; int blendSecond; };
        struct { unsigned char pad_to_70[0x70]; GfxPsxClutPosition *clutPos; };
        struct { unsigned char pad_to_74[0x74]; union { int packed; GfxBgraPixel bgra; } flatColor; };
        struct { unsigned char pad_to_78[0x78]; GfxPsxClutPosition clutPosRec; };
        struct { unsigned char pad_to_8C[0x8C]; void *primPtr; };
        struct { unsigned char pad_to_90[0x90]; GfxPsxPrimitiveAttrs *primAttrs; };
        struct { unsigned char pad_to_94[0x94]; int alpha; };
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
#pragma pack(pop)

    frame.clutPos = &frame.clutPosRec;
    frame.extCoordFlag = 0;
    frame.primAttrs = 0;
    frame.primPtr = primitive;
    frame.primFlags = *(unsigned char *)((char *)frame.primPtr + 7);
    frame.primKind = frame.primFlags >> 2;
    if (g_pGfxPsxPrimitiveSystem != 0 && g_GfxPsxBlendMode != 0) {
        if ((frame.primFlags & GFX_PSX_PRIM_CODE_SEMI_TRANSPARENT) != 0) {
            switch (g_GfxPsxRenderMode) {
            case BLEND_AVERAGE:
                if (g_GfxPsxBlendEnable != 0) {
                    frame.primAttrs = (GfxPsxPrimitiveAttrs *)(g_pGfxPsxPrimitiveSystem[1].attrs);
                } else {
                    frame.primAttrs = (GfxPsxPrimitiveAttrs *)(g_pGfxPsxPrimitiveSystem->attrs);
                }
                frame.alpha = *(unsigned char *)&g_GfxPsxCornerAlphas;
                break;
            case BLEND_ADDITIVE:
                frame.primAttrs = (GfxPsxPrimitiveAttrs *)g_pGfxPsxPrimitiveSystem->renderObjectSet;
                frame.alpha = 0xFF;
                break;
            case BLEND_QUARTER_ADDITIVE:
                frame.primAttrs = (GfxPsxPrimitiveAttrs *)g_pGfxPsxPrimitiveSystem->renderObjectSet;
                frame.alpha = 0x40;
            }
        } else {
            if (g_GfxPsxOpaqueTableAlt != 0) {
                frame.primAttrs = (GfxPsxPrimitiveAttrs *)(g_pGfxPsxPrimitiveSystem[1].renderObjectSet);
            } else {
                frame.primAttrs = (GfxPsxPrimitiveAttrs *)(g_pGfxPsxPrimitiveSystem->displayListRecord);
            }
            frame.alpha = 0xFF;
        }
    }
    if (frame.primAttrs != 0) {
        frame.blendFirst = g_GfxPsxBlendFactorFirst0;
        frame.blendSecond = g_GfxPsxBlendFactorSecond0;
        switch (frame.primKind) {
        case GFX_PSX_PRIM_LINE_F2:
            frame.linePrim = (GfxPsxLinePrimitive *)primitive;
            frame.flatColor.bgra.red = frame.linePrim->colorR;
            frame.flatColor.bgra.green = frame.linePrim->colorG;
            frame.flatColor.bgra.blue = frame.linePrim->colorB;
            frame.flatColor.bgra.alpha = *(char *)&frame.alpha;
            if (g_GfxPsxBlendFactorPairEnable == 0) {
                Gfx_EmitPsxFlatLineProjected(frame.flatColor.packed, &g_GfxPsxClipVertex1, &g_GfxPsxClipVertex0, frame.primAttrs->flatLineBatch);
            } else {
                /* Retail caller prototype (int, float, float, float, float, int, int, GfxRenderObject *) -> void preserved for /Od argument promotion on `Gfx_EmitPsxFlatLineScreen`. */
                ((void(__cdecl *)(int, float, float, float, float, int, int, GfxRenderObject *))Gfx_EmitPsxFlatLineScreen)(frame.flatColor.packed, (float)(float)frame.linePrim->x0, (float)(float)frame.linePrim->y0, (float)(float)frame.linePrim->x1, (float)(float)frame.linePrim->y1, frame.blendFirst, frame.blendSecond, frame.primAttrs->flatLineBatch);
            }
            break;
        case GFX_PSX_PRIM_LINE_G2:
            frame.gouraudLine = (GfxPsxGouraudLine *)primitive;
            frame.color0.bgra.red = frame.gouraudLine->colorR0;
            frame.color0.bgra.green = frame.gouraudLine->colorG0;
            frame.color0.bgra.blue = frame.gouraudLine->colorB0;
            frame.color0.bgra.alpha = *(char *)&frame.alpha;
            frame.color1.bgra.red = frame.gouraudLine->colorR1;
            frame.color1.bgra.green = frame.gouraudLine->colorG1;
            frame.color1.bgra.blue = frame.gouraudLine->colorB1;
            frame.color1.bgra.alpha = *(char *)&frame.alpha;
            if (g_GfxPsxBlendFactorPairEnable == 0) {
                Gfx_EmitPsxGouraudLineProjected(frame.color0.packed, frame.color1.packed, &g_GfxPsxClipVertex1, &g_GfxPsxClipVertex0, frame.primAttrs->gouraudLineBatch);
            } else {
                /* Retail caller prototype (int, int, float, float, float, float, int, int, GfxRenderObject *) -> void preserved for /Od argument promotion on `Gfx_EmitPsxGouraudLineScreen`. */
                ((void(__cdecl *)(int, int, float, float, float, float, int, int, GfxRenderObject *))Gfx_EmitPsxGouraudLineScreen)(frame.color0.packed, frame.color1.packed, (float)(float)frame.gouraudLine->x0, (float)(float)frame.gouraudLine->y0, (float)(float)frame.gouraudLine->x1, (float)(float)frame.gouraudLine->y1, frame.blendFirst, frame.blendSecond, frame.primAttrs->gouraudLineBatch);
            }
            break;
        case GFX_PSX_PRIM_LINE_G3:
            frame.gouraudPolyline = (GfxPsxGouraudPolylineVerts *)primitive;
            frame.color0.bgra.red = frame.gouraudPolyline->color0R;
            frame.color0.bgra.green = frame.gouraudPolyline->color0G;
            frame.color0.bgra.blue = frame.gouraudPolyline->color0B;
            frame.color0.bgra.alpha = *(char *)&frame.alpha;
            frame.color1.bgra.red = frame.gouraudPolyline->color1R;
            frame.color1.bgra.green = frame.gouraudPolyline->color1G;
            frame.color1.bgra.blue = frame.gouraudPolyline->color1B;
            frame.color1.bgra.alpha = *(char *)&frame.alpha;
            frame.color2.bgra.red = frame.gouraudPolyline->color2R;
            frame.color2.bgra.green = frame.gouraudPolyline->color2G;
            frame.color2.bgra.blue = frame.gouraudPolyline->color2B;
            frame.color2.bgra.alpha = *(char *)&frame.alpha;
            if (g_GfxPsxBlendFactorPairEnable == 0) {
                Gfx_EmitPsxGouraudPolylineProjected(frame.color0.packed, frame.color1.packed, frame.color2.packed, &g_GfxPsxClipVertex1, (GfxPsxProjectedPoint *)&g_GfxPsxClipVertex0, &g_GfxPsxClipVertex3, frame.primAttrs->gouraudLineBatch);
            } else {
                /* Retail caller prototype (int, int, int, float, float, float, float, float, float, int, int, GfxRenderObject *) -> void preserved for /Od argument promotion on `Gfx_EmitPsxGouraudPolylineScreen`. */
                ((void(__cdecl *)(int, int, int, float, float, float, float, float, float, int, int, GfxRenderObject *))Gfx_EmitPsxGouraudPolylineScreen)(frame.color0.packed, frame.color1.packed, frame.color2.packed, (float)(float)frame.gouraudPolyline->vertex0X, (float)(float)frame.gouraudPolyline->vertex0Y, (float)(float)frame.gouraudPolyline->vertex1X, (float)(float)frame.gouraudPolyline->vertex1Y, (float)(float)frame.gouraudPolyline->vertex2X, (float)(float)frame.gouraudPolyline->vertex2Y, frame.blendFirst, frame.blendSecond, frame.primAttrs->gouraudLineBatch);
            }
            break;
        case GFX_PSX_PRIM_POLY_F3:
            frame.flatPrim = (GfxPsxFlatPrimitive *)primitive;
            frame.flatColor.bgra.red = frame.flatPrim->colorR;
            frame.flatColor.bgra.green = frame.flatPrim->colorG;
            frame.flatColor.bgra.blue = frame.flatPrim->colorB;
            frame.flatColor.bgra.alpha = *(char *)&frame.alpha;
            if (g_GfxPsxBlendFactorPairEnable == 0) {
                Gfx_EmitPsxFlatTriangleProjected(frame.flatColor.packed, &g_GfxPsxClipVertex1, &g_GfxPsxClipVertex0, &g_GfxPsxClipVertex3, frame.primAttrs->flatTriangleBatch);
            } else {
                /* Retail caller prototype (int, float, float, float, float, float, float, int, int, GfxRenderObject *) -> void preserved for /Od argument promotion on `Gfx_EmitPsxFlatTriangleScreen`. */
                ((void(__cdecl *)(int, float, float, float, float, float, float, int, int, GfxRenderObject *))Gfx_EmitPsxFlatTriangleScreen)(frame.flatColor.packed, (float)(float)frame.flatPrim->x0, (float)(float)frame.flatPrim->y0, (float)(float)frame.flatPrim->x1, (float)(float)frame.flatPrim->y1, (float)(float)frame.flatPrim->x2, (float)(float)frame.flatPrim->y2, frame.blendFirst, frame.blendSecond, frame.primAttrs->flatTriangleBatch);
            }
            break;
        case GFX_PSX_PRIM_POLY_F4:
            frame.flatQuad = (GfxPsxFlatQuadPrim *)primitive;
            frame.flatColor.bgra.red = frame.flatQuad->colorR;
            frame.flatColor.bgra.green = frame.flatQuad->colorG;
            frame.flatColor.bgra.blue = frame.flatQuad->colorB;
            frame.flatColor.bgra.alpha = *(char *)&frame.alpha;
            if (g_GfxPsxBlendFactorPairEnable == 0) {
                Gfx_EmitPsxFlatQuadProjected(frame.flatColor.packed, &g_GfxPsxClipVertex1, &g_GfxPsxClipVertex0, &g_GfxPsxClipVertex3, &g_GfxPsxClipVertex2, frame.primAttrs->flatQuadBatch);
            } else {
                /* Retail caller prototype (int, float, float, float, float, float, float, float, float, int, int, GfxRenderObject *) -> void preserved for /Od argument promotion on `Gfx_EmitPsxFlatQuadScreen`. */
                ((void(__cdecl *)(int, float, float, float, float, float, float, float, float, int, int, GfxRenderObject *))Gfx_EmitPsxFlatQuadScreen)(frame.flatColor.packed, (float)(float)frame.flatQuad->x0, (float)(float)frame.flatQuad->y0, (float)(float)frame.flatQuad->x1, (float)(float)frame.flatQuad->y1, (float)(float)frame.flatQuad->x2, (float)(float)frame.flatQuad->y2, (float)(float)frame.flatQuad->x3, (float)(float)frame.flatQuad->y3, frame.blendFirst, frame.blendSecond, frame.primAttrs->flatQuadBatch);
            }
            break;
        case GFX_PSX_PRIM_POLY_G3:
            frame.gouraudTri = (GfxPsxGouraudPolylineVerts *)primitive;
            frame.color0.bgra.red = frame.gouraudTri->color0R;
            frame.color0.bgra.green = frame.gouraudTri->color0G;
            frame.color0.bgra.blue = frame.gouraudTri->color0B;
            frame.color0.bgra.alpha = *(char *)&frame.alpha;
            frame.color1.bgra.red = frame.gouraudTri->color1R;
            frame.color1.bgra.green = frame.gouraudTri->color1G;
            frame.color1.bgra.blue = frame.gouraudTri->color1B;
            frame.color1.bgra.alpha = *(char *)&frame.alpha;
            frame.color2.bgra.red = frame.gouraudTri->color2R;
            frame.color2.bgra.green = frame.gouraudTri->color2G;
            frame.color2.bgra.blue = frame.gouraudTri->color2B;
            frame.color2.bgra.alpha = *(char *)&frame.alpha;
            if (g_GfxPsxBlendFactorPairEnable == 0) {
                Gfx_EmitPsxGouraudTriangleProjected(frame.color0.packed, frame.color1.packed, frame.color2.packed, &g_GfxPsxClipVertex1, &g_GfxPsxClipVertex0, &g_GfxPsxClipVertex3, frame.primAttrs->gouraudTriangleBatch);
            } else {
                /* Retail caller prototype (int, int, int, float, float, float, float, float, float, int, int, GfxRenderObject *) -> void preserved for /Od argument promotion on `Gfx_EmitPsxGouraudTriangleScreen`. */
                ((void(__cdecl *)(int, int, int, float, float, float, float, float, float, int, int, GfxRenderObject *))Gfx_EmitPsxGouraudTriangleScreen)(frame.color0.packed, frame.color1.packed, frame.color2.packed, (float)(float)frame.gouraudTri->vertex0X, (float)(float)frame.gouraudTri->vertex0Y, (float)(float)frame.gouraudTri->vertex1X, (float)(float)frame.gouraudTri->vertex1Y, (float)(float)frame.gouraudTri->vertex2X, (float)(float)frame.gouraudTri->vertex2Y, frame.blendFirst, frame.blendSecond, frame.primAttrs->gouraudTriangleBatch);
            }
            break;
        case GFX_PSX_PRIM_POLY_G4:
            frame.gouraudQuad = (GfxPsxGouraudQuad *)primitive;
            frame.color0.bgra.red = frame.gouraudQuad->vertex0R;
            frame.color0.bgra.green = frame.gouraudQuad->vertex0G;
            frame.color0.bgra.blue = frame.gouraudQuad->vertex0B;
            frame.color0.bgra.alpha = (g_GfxPsxCornerAlphas.alpha0);
            frame.color1.bgra.red = frame.gouraudQuad->vertex1R;
            frame.color1.bgra.green = frame.gouraudQuad->vertex1G;
            frame.color1.bgra.blue = frame.gouraudQuad->vertex1B;
            frame.color1.bgra.alpha = g_GfxPsxCornerAlphas.alpha1;
            frame.color2.bgra.red = frame.gouraudQuad->vertex2R;
            frame.color2.bgra.green = frame.gouraudQuad->vertex2G;
            frame.color2.bgra.blue = frame.gouraudQuad->vertex2B;
            frame.color2.bgra.alpha = g_GfxPsxCornerAlphas.alpha2;
            frame.color3.bgra.red = frame.gouraudQuad->vertex3R;
            frame.color3.bgra.green = frame.gouraudQuad->vertex3G;
            frame.color3.bgra.blue = frame.gouraudQuad->vertex3B;
            frame.color3.bgra.alpha = g_GfxPsxCornerAlphas.alpha3;
            if (g_GfxPsxBlendFactorPairEnable == 0) {
                Gfx_EmitPsxGouraudQuadProjected(frame.color0.packed, frame.color1.packed, frame.color2.packed, frame.color3.packed, &g_GfxPsxClipVertex1, &g_GfxPsxClipVertex0, &g_GfxPsxClipVertex3, &g_GfxPsxClipVertex2, frame.primAttrs->gouraudQuadBatch);
            } else if (g_GfxPsxBlendFactorQuadEnable != 0) {
                /* Retail caller prototype (int, int, int, int, float, float, float, float, float, float, float, float, int, int, int, int, int, int, int, int, GfxRenderObject *) -> void preserved for /Od argument promotion on `Gfx_EmitPsxGouraudQuadScreenPerVertexBlend`. */
                ((void(__cdecl *)(int, int, int, int, float, float, float, float, float, float, float, float, int, int, int, int, int, int, int, int, GfxRenderObject *))Gfx_EmitPsxGouraudQuadScreenPerVertexBlend)(frame.color0.packed, frame.color1.packed, frame.color2.packed, frame.color3.packed, (float)(float)frame.gouraudQuad->vertex0X, (float)(float)frame.gouraudQuad->vertex0Y, (float)(float)frame.gouraudQuad->vertex1X, (float)(float)frame.gouraudQuad->vertex1Y, (float)(float)frame.gouraudQuad->vertex2X, (float)(float)frame.gouraudQuad->vertex2Y, (float)(float)frame.gouraudQuad->vertex3X, (float)(float)frame.gouraudQuad->vertex3Y, frame.blendFirst, frame.blendSecond, g_GfxPsxBlendFactorFirst1, g_GfxPsxBlendFactorSecond1, g_GfxPsxBlendFactorFirst2, g_GfxPsxBlendFactorSecond2, g_GfxPsxBlendFactorFirst3, g_GfxPsxBlendFactorSecond3, frame.primAttrs->gouraudQuadBatch);
                g_GfxPsxBlendFactorQuadEnable = 0;
            } else {
                /* Retail caller prototype (int, int, int, int, float, float, float, float, float, float, float, float, int, int, GfxRenderObject *) -> void preserved for /Od argument promotion on `Gfx_EmitPsxGouraudQuadScreen`. */
                ((void(__cdecl *)(int, int, int, int, float, float, float, float, float, float, float, float, int, int, GfxRenderObject *))Gfx_EmitPsxGouraudQuadScreen)(frame.color0.packed, frame.color1.packed, frame.color2.packed, frame.color3.packed, (float)(float)frame.gouraudQuad->vertex0X, (float)(float)frame.gouraudQuad->vertex0Y, (float)(float)frame.gouraudQuad->vertex1X, (float)(float)frame.gouraudQuad->vertex1Y, (float)(float)frame.gouraudQuad->vertex2X, (float)(float)frame.gouraudQuad->vertex2Y, (float)(float)frame.gouraudQuad->vertex3X, (float)(float)frame.gouraudQuad->vertex3Y, frame.blendFirst, frame.blendSecond, frame.primAttrs->gouraudQuadBatch);
            }
            break;
        case GFX_PSX_PRIM_TILE:
            frame.rectPrim = (GfxPsxRectPrimitive *)primitive;
            frame.flatColor.bgra.red = frame.rectPrim->colorR;
            frame.flatColor.bgra.green = frame.rectPrim->colorG;
            frame.flatColor.bgra.blue = frame.rectPrim->colorB;
            frame.flatColor.bgra.alpha = *(char *)&frame.alpha;
            if (g_GfxPsxBlendFactorPairEnable == 0) { /* empty first arm: lib/codegen.h */
            } else {
                Gfx_EmitPsxFlatRectScreen(frame.rectPrim->x, frame.rectPrim->y, frame.rectPrim->width, frame.rectPrim->height, frame.blendFirst, frame.blendSecond, frame.flatColor.packed, frame.primAttrs->flatQuadBatch);
            }
            break;
        case GFX_PSX_PRIM_TILE_1:
            frame.dotRec = (GfxPsxRect8Record *)primitive;
            frame.flatColor.bgra.red = frame.dotRec->colorR;
            frame.flatColor.bgra.green = frame.dotRec->colorG;
            frame.flatColor.bgra.blue = frame.dotRec->colorB;
            frame.flatColor.bgra.alpha = *(char *)&frame.alpha;
            if (g_GfxPsxBlendFactorPairEnable == 0) {
                Gfx_EmitPsxDotTriangleProjected(frame.flatColor.packed, frame.primAttrs->flatTriangleBatch);
            } else {
                Gfx_EmitPsxDotTriangleScreen(frame.dotRec->x, frame.dotRec->y, frame.blendFirst, frame.blendSecond, frame.flatColor.packed, frame.primAttrs->flatTriangleBatch);
            }
            break;
        case GFX_PSX_PRIM_TILE_8:
            frame.rect8 = (GfxPsxRect8Record *)primitive;
            frame.flatColor.bgra.red = frame.rect8->colorR;
            frame.flatColor.bgra.green = frame.rect8->colorG;
            frame.flatColor.bgra.blue = frame.rect8->colorB;
            frame.flatColor.bgra.alpha = *(char *)&frame.alpha;
            if (g_GfxPsxBlendFactorPairEnable == 0) { /* empty first arm: lib/codegen.h */
            } else {
                Gfx_EmitPsxFlatRectScreen(frame.rect8->x, frame.rect8->y, 8, 8, frame.blendFirst, frame.blendSecond, frame.flatColor.packed, frame.primAttrs->flatQuadBatch);
            }
            break;
        case GFX_PSX_PRIM_TILE_16:
            frame.rect16 = (GfxPsxRect8Record *)primitive;
            frame.flatColor.bgra.red = frame.rect16->colorR;
            frame.flatColor.bgra.green = frame.rect16->colorG;
            frame.flatColor.bgra.blue = frame.rect16->colorB;
            frame.flatColor.bgra.alpha = *(char *)&frame.alpha;
            if (g_GfxPsxBlendFactorPairEnable == 0) { /* empty first arm: lib/codegen.h */
            } else {
                Gfx_EmitPsxFlatRectScreen(frame.rect16->x, frame.rect16->y, 0x10, 0x10, frame.blendFirst, frame.blendSecond, frame.flatColor.packed, frame.primAttrs->flatQuadBatch);
            }
            break;
        case GFX_PSX_PRIM_POLY_FT3:
            frame.texFlatTri = (GfxPsxPrimitive *)primitive;
            Gfx_DecodePsxClutPosition(frame.texFlatTri->texturePage, frame.texFlatTri->clut, frame.primAttrs->texFlatTriangleBatch, frame.clutPos);
            frame.clutY = frame.clutPos->yOfs;
            Gfx_PackDoubledColorToBgra((char)frame.texFlatTri->colorR, (char)frame.texFlatTri->colorG, (char)frame.texFlatTri->colorB, frame.alpha, &frame.flatColor.bgra);
            if (g_GfxPsxBlendFactorPairEnable == 0) {
                Gfx_EmitPsxTexturedFlatTriangleProjected(frame.flatColor.packed, &g_GfxPsxClipVertex1, &g_GfxPsxClipVertex0, &g_GfxPsxClipVertex3, frame.texFlatTri->u0, frame.texFlatTri->v0, frame.texFlatTri->u1, frame.texFlatTri->v1, frame.texFlatTri->u2, frame.texFlatTri->v2, frame.clutY, (GfxPsxTexturedFlatTriangle *)frame.primAttrs->texFlatTriangleBatch);
            } else {
                /* Retail caller prototype (int, float, float, float, float, float, float, int, int, int, int, int, int, int, int, int, int) -> void preserved for /Od argument promotion on `Gfx_EmitPsxTexturedFlatTriangleScreen`. */
                ((void(__cdecl *)(int, float, float, float, float, float, float, int, int, int, int, int, int, int, int, int, int))Gfx_EmitPsxTexturedFlatTriangleScreen)(frame.flatColor.packed, (float)(float)frame.texFlatTri->x0, (float)(float)frame.texFlatTri->y0, (float)(float)frame.texFlatTri->x1, (float)(float)frame.texFlatTri->y1, (float)(float)frame.texFlatTri->x2, (float)(float)frame.texFlatTri->y2, frame.texFlatTri->u0, frame.texFlatTri->v0, frame.texFlatTri->u1, frame.texFlatTri->v1, frame.texFlatTri->u2, frame.texFlatTri->v2, frame.clutY, frame.blendFirst, frame.blendSecond, frame.primAttrs->texFlatTriangleBatch);
            }
            break;
        case GFX_PSX_PRIM_POLY_FT4:
            frame.texQuad = (GfxPsxTexturedQuad *)primitive;
            Gfx_DecodePsxClutPosition(frame.texQuad->texturePage, frame.texQuad->clut, frame.primAttrs->texQuadBatch, frame.clutPos);
            frame.clutY = frame.clutPos->yOfs;
            Gfx_PackDoubledColorToBgra((char)frame.texQuad->colorR, (char)frame.texQuad->colorG, (char)frame.texQuad->colorB, frame.alpha, &frame.flatColor.bgra);
            if (g_GfxPsxBlendFactorPairEnable == 0) {
                Gfx_EmitPsxTexturedFlatQuadProjected(frame.flatColor.packed, &g_GfxPsxClipVertex1, &g_GfxPsxClipVertex0, &g_GfxPsxClipVertex3, &g_GfxPsxClipVertex2, frame.texQuad->u0, frame.texQuad->v0, frame.texQuad->u1, frame.texQuad->v1, frame.texQuad->u2, frame.texQuad->v2, frame.texQuad->u3, frame.texQuad->v3, frame.clutY, (GfxTexturedFlatQuadProjection *)(frame.primAttrs->texQuadBatch));
            } else {
                /* Retail call-site prototype (int, float, float, float, float, float, float, float, float, int, int, int, int, int, int, int, int, int, int, int, int) -> void preserved for /Od caller codegen. */
                ((void(__cdecl *)(int, float, float, float, float, float, float, float, float, int, int, int, int, int, int, int, int, int, int, int, int))Gfx_EmitPsxTexturedFlatQuadScreen)(frame.flatColor.packed, (float)(float)frame.texQuad->x0, (float)(float)frame.texQuad->y0, (float)(float)frame.texQuad->x1, (float)(float)frame.texQuad->y1, (float)(float)frame.texQuad->x2, (float)(float)frame.texQuad->y2, (float)(float)frame.texQuad->x3, (float)(float)frame.texQuad->y3, frame.texQuad->u0, frame.texQuad->v0, frame.texQuad->u1, frame.texQuad->v1, frame.texQuad->u2, frame.texQuad->v2, frame.texQuad->u3, frame.texQuad->v3, frame.clutY, frame.blendFirst, frame.blendSecond, frame.primAttrs->texQuadBatch);
            }
            break;
        case GFX_PSX_PRIM_POLY_GT3:
            frame.texTri = (GfxPsxTexturedTriangle *)primitive;
            Gfx_DecodePsxClutPosition(frame.texTri->texturePage, frame.texTri->clut, frame.primAttrs->texGouraudTriangleBatch, frame.clutPos);
            frame.clutY = frame.clutPos->yOfs;
            Gfx_PackDoubledColorToBgra((char)frame.texTri->vertex0R, (char)frame.texTri->vertex0G, (char)frame.texTri->vertex0B, frame.alpha, &frame.color0.bgra);
            Gfx_PackDoubledColorToBgra((char)frame.texTri->vertex1R, (char)frame.texTri->vertex1G, (char)frame.texTri->vertex1B, frame.alpha, &frame.color1.bgra);
            Gfx_PackDoubledColorToBgra((char)frame.texTri->vertex2R, (char)frame.texTri->vertex2G, (char)frame.texTri->vertex2B, frame.alpha, &frame.color2.bgra);
            if (g_GfxPsxBlendFactorPairEnable == 0) {
                Gfx_EmitPsxTexturedGouraudTriangleProjected(frame.color0.packed, frame.color1.packed, frame.color2.packed, &g_GfxPsxClipVertex1, &g_GfxPsxClipVertex0, &g_GfxPsxClipVertex3, frame.texTri->vertex0U, frame.texTri->vertex0V, frame.texTri->vertex1U, frame.texTri->vertex1V, frame.texTri->vertex2U, frame.texTri->vertex2V, frame.clutY, (GfxPsxTexturedFlatTriangle *)frame.primAttrs->texGouraudTriangleBatch);
            } else {
                /* Retail caller prototype (int, int, int, float, float, float, float, float, float, int, int, int, int, int, int, int, int, int, int) -> void preserved for /Od argument promotion on `Gfx_EmitPsxTexturedGouraudTriangleScreen`. */
                ((void(__cdecl *)(int, int, int, float, float, float, float, float, float, int, int, int, int, int, int, int, int, int, int))Gfx_EmitPsxTexturedGouraudTriangleScreen)(frame.color0.packed, frame.color1.packed, frame.color2.packed, (float)(float)frame.texTri->vertex0X, (float)(float)frame.texTri->vertex0Y, (float)(float)frame.texTri->vertex1X, (float)(float)frame.texTri->vertex1Y, (float)(float)frame.texTri->vertex2X, (float)(float)frame.texTri->vertex2Y, frame.texTri->vertex0U, frame.texTri->vertex0V, frame.texTri->vertex1U, frame.texTri->vertex1V, frame.texTri->vertex2U, frame.texTri->vertex2V, frame.clutY, frame.blendFirst, frame.blendSecond, frame.primAttrs->texGouraudTriangleBatch);
            }
            break;
        case GFX_PSX_PRIM_POLY_GT4:
            frame.texGouraudQuad = (GfxPsxTexturedQuadSource *)primitive;
            Gfx_DecodePsxClutPosition(frame.texGouraudQuad->texturePage, frame.texGouraudQuad->clut, frame.primAttrs->texGouraudQuadBatch, frame.clutPos);
            frame.clutY = frame.clutPos->yOfs;
            Gfx_PackDoubledColorToBgra((char)frame.texGouraudQuad->color0R, (char)frame.texGouraudQuad->color0G, (char)frame.texGouraudQuad->color0B, frame.alpha, &frame.color0.bgra);
            Gfx_PackDoubledColorToBgra((char)frame.texGouraudQuad->color1R, (char)frame.texGouraudQuad->color1G, (char)frame.texGouraudQuad->color1B, frame.alpha, &frame.color1.bgra);
            Gfx_PackDoubledColorToBgra((char)frame.texGouraudQuad->color2R, (char)frame.texGouraudQuad->color2G, (char)frame.texGouraudQuad->color2B, frame.alpha, &frame.color2.bgra);
            Gfx_PackDoubledColorToBgra((char)frame.texGouraudQuad->color3R, (char)frame.texGouraudQuad->color3G, (char)frame.texGouraudQuad->color3B, frame.alpha, &frame.color3.bgra);
            if (g_GfxPsxBlendFactorPairEnable == 0) {
                Gfx_EmitPsxTexturedGouraudQuadProjected(frame.color0.packed, frame.color1.packed, frame.color2.packed, frame.color3.packed, &g_GfxPsxClipVertex1, &g_GfxPsxClipVertex0, &g_GfxPsxClipVertex3, &g_GfxPsxClipVertex2, frame.texGouraudQuad->u0, frame.texGouraudQuad->v0, frame.texGouraudQuad->u1, frame.texGouraudQuad->v1, frame.texGouraudQuad->u2, frame.texGouraudQuad->v2, frame.texGouraudQuad->u3, frame.texGouraudQuad->v3, frame.clutY, (GfxTexturedFlatQuadProjection *)(frame.primAttrs->texGouraudQuadBatch));
            } else {
                /* Retail caller prototype (int, int, int, int, float, float, float, float, float, float, float, float, int, int, int, int, int, int, int, int, int, int, int, int) -> void preserved for /Od argument promotion on `Gfx_EmitPsxTexturedGouraudQuadScreen`. */
                ((void(__cdecl *)(int, int, int, int, float, float, float, float, float, float, float, float, int, int, int, int, int, int, int, int, int, int, int, int))Gfx_EmitPsxTexturedGouraudQuadScreen)(frame.color0.packed, frame.color1.packed, frame.color2.packed, frame.color3.packed, (float)(float)frame.texGouraudQuad->x0, (float)(float)frame.texGouraudQuad->y0, (float)(float)frame.texGouraudQuad->x1, (float)(float)frame.texGouraudQuad->y1, (float)(float)frame.texGouraudQuad->x2, (float)(float)frame.texGouraudQuad->y2, (float)(float)frame.texGouraudQuad->x3, (float)(float)frame.texGouraudQuad->y3, frame.texGouraudQuad->u0, frame.texGouraudQuad->v0, frame.texGouraudQuad->u1, frame.texGouraudQuad->v1, frame.texGouraudQuad->u2, frame.texGouraudQuad->v2, frame.texGouraudQuad->u3, frame.texGouraudQuad->v3, frame.clutY, frame.blendFirst, frame.blendSecond, frame.primAttrs->texGouraudQuadBatch);
            }
            break;
        case GFX_PSX_PRIM_SPRT:
            frame.texSprite = (GfxPsxTexturedSprite *)primitive;
            Gfx_DecodePsxClutPosition(g_GfxPsxClutPosition, frame.texSprite->clut, frame.primAttrs->texQuadBatch, frame.clutPos);
            frame.clutY = frame.clutPos->yOfs;
            Gfx_PackDoubledColorToBgra((char)frame.texSprite->colorR, (char)frame.texSprite->colorG, (char)frame.texSprite->colorB, frame.alpha, &frame.flatColor.bgra);
            if (g_GfxPsxBlendFactorPairEnable == 0) { /* empty first arm: lib/codegen.h */
            } else {
                Gfx_EmitPsxTexturedSpriteQuad(frame.texSprite->x, frame.texSprite->y, frame.texSprite->width, frame.texSprite->height, frame.texSprite->texU, frame.texSprite->texV, frame.blendFirst, frame.blendSecond, frame.flatColor.packed, frame.clutY, (GfxPsxSpriteQuadState *)frame.primAttrs->texQuadBatch);
            }
            break;
        case GFX_PSX_PRIM_SPRT_8:
            frame.sprite8 = (GfxPsxSprite *)primitive;
            Gfx_DecodePsxClutPosition(g_GfxPsxClutPosition, frame.sprite8->clut, frame.primAttrs->texQuadBatch, frame.clutPos);
            frame.clutY = frame.clutPos->yOfs;
            Gfx_PackDoubledColorToBgra((char)frame.sprite8->r, (char)frame.sprite8->g, (char)frame.sprite8->b, frame.alpha, &frame.flatColor.bgra);
            if (g_GfxPsxBlendFactorPairEnable == 0) { /* empty first arm: lib/codegen.h */
            } else {
                Gfx_EmitPsxTexturedSpriteQuad(frame.sprite8->x, frame.sprite8->y, 8, 8, frame.sprite8->u, frame.sprite8->v, frame.blendFirst, frame.blendSecond, frame.flatColor.packed, frame.clutY, (GfxPsxSpriteQuadState *)frame.primAttrs->texQuadBatch);
            }
            break;
        case GFX_PSX_PRIM_SPRT_16:
            frame.sprite16 = (GfxPsxSprite *)primitive;
            Gfx_DecodePsxClutPosition(g_GfxPsxClutPosition, frame.sprite16->clut, frame.primAttrs->texQuadBatch, frame.clutPos);
            frame.clutY = frame.clutPos->yOfs;
            Gfx_PackDoubledColorToBgra((char)frame.sprite16->r, (char)frame.sprite16->g, (char)frame.sprite16->b, frame.alpha, &frame.flatColor.bgra);
            if (g_GfxPsxBlendFactorPairEnable == 0) {
                break;
            }
            Gfx_EmitPsxTexturedSpriteQuad(frame.sprite16->x, frame.sprite16->y, 0x10, 0x10, frame.sprite16->u, frame.sprite16->v, frame.blendFirst, frame.blendSecond, frame.flatColor.packed, frame.clutY, (GfxPsxSpriteQuadState *)frame.primAttrs->texQuadBatch);
        }
    }
    g_GfxPsxBlendFactorPairEnable = 0;
    g_GfxPsxRenderMode = BLEND_ADDITIVE;
    g_GfxPsxBlendEnable = 0;
    g_GfxPsxOpaqueTableAlt = 0;
}

// Destroys a PSX-style graphics set: its six render objects (renderObjectSlot0 through
// renderObjectSlot5), then the record itself. Null is accepted.
// FUNCTION: FF7 0x0066bd4b
void __cdecl Gfx_PsxFreeRenderObjectSet(GfxPsxRenderObjectSet *objectSet)
{
    if (objectSet != 0) {
        DestroyRenderObject((void *)objectSet->renderObjectSlot0);
        DestroyRenderObject((void *)objectSet->renderObjectSlot1);
        DestroyRenderObject((void *)objectSet->renderObjectSlot2);
        DestroyRenderObject((void *)objectSet->renderObjectSlot3);
        DestroyRenderObject((void *)objectSet->renderObjectSlot4);
        DestroyRenderObject((void *)objectSet->renderObjectSlot5);
        // STRING: FF7 0x0090c9f4
        GameHeapFree(objectSet, "C:\\lib\\src\\graphics\\psxgraph.cpp", 0x775);
    }
}

/*
 * Reconstructed native function.
 */
// FUNCTION: FF7 0x0066bdc5
int __cdecl Gfx_PsxCreateRenderObjectSet(int blendMode, int flags, int context)
{
    int objectType;
    GfxPsxRenderObjectSet *pObjectSet;
    PfileLoadOptions *optPtr;
    PfileLoadOptions loadOptions;

    optPtr = &loadOptions;
    Pfile_InitLoadOptionsWithBlend(blendMode, optPtr);
    optPtr->polygonFlags = 1;
    optPtr->drawPool = flags;
    optPtr->emitterFlags = 2;
    // STRING: FF7 0x0090ca18
    pObjectSet = (GfxPsxRenderObjectSet *)GameHeapAlloc(1, 0x38, "C:\\lib\\src\\graphics\\psxgraph.cpp", 0x788);
    if (pObjectSet != 0) {
        objectType = 0x12;
        pObjectSet->renderObjectSlot0 = (int)Gfx_CreateRenderObject(1, objectType, optPtr, 0, (void *)context);
        objectType = 0x13;
        pObjectSet->renderObjectSlot1 = (int)Gfx_CreateRenderObject(1, objectType, optPtr, 0, (void *)context);
        objectType = 0;
        pObjectSet->renderObjectSlot2 = (int)Gfx_CreateRenderObject(1, objectType, optPtr, 0, (void *)context);
        objectType = 8;
        pObjectSet->renderObjectSlot3 = (int)Gfx_CreateRenderObject(1, objectType, optPtr, 0, (void *)context);
        objectType = 2;
        pObjectSet->renderObjectSlot4 = (int)Gfx_CreateRenderObject(1, objectType, optPtr, 0, (void *)context);
        objectType = 0xA;
        pObjectSet->renderObjectSlot5 = (int)Gfx_CreateRenderObject(1, objectType, optPtr, 0, (void *)context);
    }
    return (int)pObjectSet;
}

// Destroys the global PSX graphics table at g_pGfxPsxPrimitiveSystem. It frees the five sets in the
// table's first five pointer words, then the attrs and renderObjectSet sets of the second entry,
// then the table itself, and nulls the global.
// FUNCTION: FF7 0x0066bf08
void __cdecl Gfx_PsxFreePrimitiveSystem(void)
{
    int i;

    if (g_pGfxPsxPrimitiveSystem != 0) {
        for (i = 0; i < 5; i++) {
            Gfx_PsxFreeRenderObjectSet((GfxPsxRenderObjectSet *)(((int *)g_pGfxPsxPrimitiveSystem)[i]));
        }
        Gfx_PsxFreeRenderObjectSet((GfxPsxRenderObjectSet *)(g_pGfxPsxPrimitiveSystem[1].attrs));
        Gfx_PsxFreeRenderObjectSet((GfxPsxRenderObjectSet *)(g_pGfxPsxPrimitiveSystem[1].renderObjectSet));
        // STRING: FF7 0x0090ca3c
        GameHeapFree(g_pGfxPsxPrimitiveSystem, "C:\\lib\\src\\graphics\\psxgraph.cpp", 0x7B1);
        g_pGfxPsxPrimitiveSystem = 0;
    }
}

/*
 * Reconstructed native function.
 */
// FUNCTION: FF7 0x0066bf8e
int __cdecl Gfx_PsxCreatePrimitiveSystem(int flags, int context)
{
    int idx;
    int result;
    int flagMask;
    GfxPsxPrimitiveAttrs *attrSet;

    result = 0;
    if (g_pGfxPsxPrimitiveSystem != 0) {
        Gfx_PsxFreePrimitiveSystem();
    }
    // STRING: FF7 0x0090ca60
    g_pGfxPsxPrimitiveSystem = (GfxPsxPrimitiveSet *)GameHeapAlloc(1, 0x1C, "C:\\lib\\src\\graphics\\psxgraph.cpp", 0x7C2);
    if (g_pGfxPsxPrimitiveSystem != 0) {
        for (idx = 0; idx < 5; idx++) {
            switch (idx) {
            case BLEND_AVERAGE:
                ((int *)g_pGfxPsxPrimitiveSystem)[idx] = Gfx_PsxCreateRenderObjectSet(idx, flags, context);
                break;
            case BLEND_ADDITIVE:
                ((int *)g_pGfxPsxPrimitiveSystem)[idx] = Gfx_PsxCreateRenderObjectSet(idx, flags, context);
                break;
            case BLEND_NONE:
                ((int *)g_pGfxPsxPrimitiveSystem)[idx] = Gfx_PsxCreateRenderObjectSet(idx, flags, context);
            }
        }
        g_pGfxPsxPrimitiveSystem[1].attrs = Gfx_PsxCreateRenderObjectSet(BLEND_AVERAGE, flags, context);
        attrSet = (GfxPsxPrimitiveAttrs *)(g_pGfxPsxPrimitiveSystem[1].attrs);
        if (attrSet != 0) {
            flagMask = 0x4000;
            Gfx_OrRenderObjectPolygonSetFlags(flagMask, (int)attrSet->flatTriangleBatch);
            Gfx_OrRenderObjectPolygonSetFlags(flagMask, (int)attrSet->flatQuadBatch);
        }
        g_pGfxPsxPrimitiveSystem[1].renderObjectSet = Gfx_PsxCreateRenderObjectSet(BLEND_NONE, flags, context);
        result = 1;
    }
    return result;
}

// Draws one display-list record. When record is non-null it fetches the engine context with
// GetEngineContext and turns render state RENDER_STATE_NO_CULL (0xE) on with
// Gfx_SetRenderStateCall. It then passes the six batch lists renderObjectSlot0 through
// renderObjectSlot5 to Gfx_EmitPrimitiveBatchList in that order, and turns the state off again.
// FUNCTION: FF7 0x0066c0f3
void __cdecl Gfx_DrawDisplayListRecord(GfxPsxRenderObjectSet *record)
{
    GfxEngineContext *context;

    if (record != 0) {
        context = GetEngineContext();
        Gfx_SetRenderStateCall(RENDER_STATE_NO_CULL, 1, context);
        Gfx_EmitPrimitiveBatchList((void *)record->renderObjectSlot0, context);
        Gfx_EmitPrimitiveBatchList((void *)record->renderObjectSlot1, context);
        Gfx_EmitPrimitiveBatchList((void *)record->renderObjectSlot2, context);
        Gfx_EmitPrimitiveBatchList((void *)record->renderObjectSlot3, context);
        Gfx_EmitPrimitiveBatchList((void *)record->renderObjectSlot4, context);
        Gfx_EmitPrimitiveBatchList((void *)record->renderObjectSlot5, context);
        Gfx_SetRenderStateCall(RENDER_STATE_NO_CULL, 0, context);
    }
}

// Draws one group of the display list rooted at g_pGfxPsxPrimitiveSystem, selected by group, with
// each record going through Gfx_DrawDisplayListRecord. Case 0 draws displayListRecord. Case 1 draws
// attrs and then renderObjectSet. Case 2 draws the attrs record of the second entry. Case 3 draws
// the renderObjectSet record of the second entry with RENDER_STATE_DEPTH_TEST and
// RENDER_STATE_DEPTH_WRITE (0xF and 0x10) set to 0 around the call and back to 1 afterwards. Does
// nothing when g_pGfxPsxPrimitiveSystem is null or group is out of range.
// FUNCTION: FF7 0x0066c19e
void __cdecl Gfx_DrawDisplayListGroup(int group)
{
    struct GfxEngineContext *context;

    if (g_pGfxPsxPrimitiveSystem != 0) {
        switch (group) {
        case GFX_PSX_DISPLAY_GROUP_OPAQUE:
            Gfx_DrawDisplayListRecord((GfxPsxRenderObjectSet *)(g_pGfxPsxPrimitiveSystem->displayListRecord));
            break;
        case GFX_PSX_DISPLAY_GROUP_MAIN:
            Gfx_DrawDisplayListRecord((GfxPsxRenderObjectSet *)(g_pGfxPsxPrimitiveSystem->attrs));
            Gfx_DrawDisplayListRecord((GfxPsxRenderObjectSet *)(g_pGfxPsxPrimitiveSystem->renderObjectSet));
            break;
        case GFX_PSX_DISPLAY_GROUP_OVERLAY_A:
            Gfx_DrawDisplayListRecord((GfxPsxRenderObjectSet *)(g_pGfxPsxPrimitiveSystem[1].attrs));
            break;
        case GFX_PSX_DISPLAY_GROUP_OVERLAY_B:
            context = (struct GfxEngineContext *)GetEngineContext();
            Gfx_SetRenderStateCall(RENDER_STATE_DEPTH_TEST, 0, context);
            Gfx_SetRenderStateCall(RENDER_STATE_DEPTH_WRITE, 0, context);
            Gfx_DrawDisplayListRecord((GfxPsxRenderObjectSet *)(g_pGfxPsxPrimitiveSystem[1].renderObjectSet));
            Gfx_SetRenderStateCall(RENDER_STATE_DEPTH_TEST, 1, context);
            Gfx_SetRenderStateCall(RENDER_STATE_DEPTH_WRITE, 1, context);
        }
    }
}

/*
 * Uses weightRaw/4096 as source-color weight, upper-clamping to one for weightRaw at least 4096 without
 * lower clamp. Expands source and current draw colors into float channels, blends via Gfx_BlendFloatColorsToBgr
 * and writes three reordered output bytes, leaving alpha untouched. Negative weights and packed-
 * byte extraction beyond the local dword remain reconstructed hazards.
 */
// FUNCTION: FF7 0x0066c287
void __cdecl Gfx_ShadePsxVertexColor(CVECTOR *srcPtr, int weightRaw, CVECTOR *dst)
{
#pragma pack(push, 1)
    union {
        int packed;
        struct {
            char value;
        } at0;
        struct {
            char pad;
            char value;
        } at1;
        struct {
            char pad[2];
            char value;
        } at2;
    } drawColorUnion;
#pragma pack(pop)
    unsigned char outBytes[4];
    float blendWeight;
    struct {
        float b, g, r, a;
    } srcRecord;
    struct {
        float b, g, r, a;
    } destinationColor;
    int colorBits;
    if (weightRaw >= 4096)
        blendWeight = 1.0f;
    else
        blendWeight = (float)weightRaw / 4096.0f;
    srcRecord.b = srcPtr->b;
    srcRecord.g = srcPtr->g;
    srcRecord.r = srcPtr->r;
    srcRecord.a = 255.0f;
    colorBits = GetDrawColorPacked();
    drawColorUnion.packed = colorBits;
    destinationColor.b = (float)(drawColorUnion.packed & 255);
    destinationColor.g = (float)(*(int *)&drawColorUnion.at1.value & 255);
    destinationColor.r = (float)(*(int *)&drawColorUnion.at2.value & 255);
    destinationColor.a = 255.0f;
    Gfx_BlendFloatColorsToBgr(1.0f - blendWeight, blendWeight, (GfxFloatColor *)&destinationColor, (GfxFloatColor *)&srcRecord, (int)&outBytes);
    dst->r = outBytes[2];
    dst->g = outBytes[1];
    dst->b = outBytes[0];
}

/*
 * Shades three vertices by calling Gfx_ShadePsxVertexColor once per vertex, passing
 * src0/weightRaw/dst0, src1/weightRaw/dst1 and src2/weightRaw/dst2, so weightRaw is the value shared by all three.
 * Purely a fan-out wrapper; its single-vertex worker preserves the original packed frame views.
 */
// FUNCTION: FF7 0x0066c371
void __cdecl Gfx_ShadePsxTriVertexColors(CVECTOR *src0, CVECTOR *src1, CVECTOR *src2, int weightRaw, CVECTOR *dst0, CVECTOR *dst1, CVECTOR *dst2)
{
    Gfx_ShadePsxVertexColor(src0, weightRaw, dst0);
    Gfx_ShadePsxVertexColor(src1, weightRaw, dst1);
    Gfx_ShadePsxVertexColor(src2, weightRaw, dst2);
}

/*
 * Stores blendMode in g_GfxPsxBlendMode.
 */
// FUNCTION: FF7 0x0066c3b2
void __cdecl Gfx_SetDrawEnabled(int blendMode)
{
    g_GfxPsxBlendMode = blendMode;
}

/*
 * Returns g_GfxPsxBlendMode.
 */
// FUNCTION: FF7 0x0066c3bf
int __cdecl Gfx_IsDrawEnabled(void)
{
    return g_GfxPsxBlendMode;
}
} /* extern "C" */
