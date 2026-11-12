// src/ff7/field/src/ad_bk.cpp
// Original: C:\FF7\field\src\ad_bk.cpp

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/api/field.h"
#include "ff7/types/field.h"
#include "lib/types/graphics.h"
#include "lib/api/graphics.h"
#include "lib/api/mem.h"
#include <stdlib.h>
#include "lib/pfile.h"
#include "ff7/field.h"
#include "ff7/enums/field.h"
extern "C" {

struct FieldBgLayer3Tile;
struct FieldLayer1Tile;
struct FieldBgLayer4Tile;
unsigned char g_bssPad_00cffde8[8] = {0};
// GLOBAL: FF7 0x00cffdf0
int g_pFieldBlitSourceSurface = 0;
unsigned char g_bssPad_00cffdf4[4] = {0};
// clang-format off
// GLOBAL: FF7 0x00cffdf8
struct FieldLayer4StateE8c g_FieldLayer2StateDf8 = {0};
// GLOBAL: FF7 0x00cffdfc
unsigned short g_FieldLayer2TilesNum = 0;
// GLOBAL: FF7 0x00cffe00
struct FieldLayer2Tile *g_FieldLayer2Tiles = 0;
// GLOBAL: FF7 0x00cffe04
int g_FieldLayer2ParamE04 = 0;
// GLOBAL: FF7 0x00cffe08
struct FieldLayer2StateE14 g_FieldLayer2StateE08 = {0};
// GLOBAL: FF7 0x00cffe0c
struct FieldLayer2StateE0c g_FieldLayer2StateE0c = {0};
// GLOBAL: FF7 0x00cffe10
struct FieldLayer2StateE14 g_FieldLayer2StateE10 = {0};
// GLOBAL: FF7 0x00cffe14
struct FieldLayer2StateE14 g_FieldLayer2StateE14 = {0};
// GLOBAL: FF7 0x00cffe18
int g_FieldLayer2ParamE18 = 0;
unsigned char g_bssPad_00cffe1c[4] = {0};
// GLOBAL: FF7 0x00cffe20
struct FieldLayer3StateE20 g_FieldLayer3StateE20 = {0};
// GLOBAL: FF7 0x00cffe24
struct FieldLayer4StateE8c g_FieldLayer3StateE24 = {0};
// GLOBAL: FF7 0x00cffe28
unsigned short g_FieldLayer3TilesNum = 0;
// GLOBAL: FF7 0x00cffe2c
struct FieldBgLayer3Tile *g_FieldLayer3Tiles = 0;
// 32-bit state word of two 16-bit halves; callers read the upper half.
// GLOBAL: FF7 0x00cffe30
struct FieldLayer2StateE14 g_FieldLayer3StateE30 = {0};
// GLOBAL: FF7 0x00cffe34
struct FieldLayer2StateE14 g_FieldLayer3StateE34 = {0};
// GLOBAL: FF7 0x00cffe38
unsigned short g_FieldLayer3StateE38 = 0;
// GLOBAL: FF7 0x00cffe3c
int g_FieldDoDrawLayer3 = 0;
// GLOBAL: FF7 0x00cffe40
int g_FieldLayer3Flag = 0;
unsigned char g_bssPad_00cffe44[4] = {0};
// GLOBAL: FF7 0x00cffe48
int g_FieldLayer1StateE48 = 0;
// GLOBAL: FF7 0x00cffe4c
int g_FieldLayer1StateE4c = 0;
// GLOBAL: FF7 0x00cffe50
short g_FieldLayer1StateE50[2] = {0};
// GLOBAL: FF7 0x00cffe54
struct FieldLayer1Tile *g_FieldLayer1Tiles = 0;
// GLOBAL: FF7 0x00cffe58
struct FieldLayer1StateE58 g_FieldLayer1StateE58 = {0};
// GLOBAL: FF7 0x00cffe5c
unsigned short g_FieldLayer1TilesNum = 0;
unsigned char g_bssPad_00cffe60[16] = {0};
// GLOBAL: FF7 0x00cffe70
struct FieldCameraScrollE70 g_FieldCameraScrollE70 = {0};
// GLOBAL: FF7 0x00cffe74
int g_FieldCameraScrollE74 = 0;
unsigned char g_bssPad_00cffe78[8] = {0};
// GLOBAL: FF7 0x00cffe80
int g_FieldCameraScrollE80 = 0;
unsigned char g_bssPad_00cffe84[4] = {0};
// GLOBAL: FF7 0x00cffe88
struct FieldLayer3StateE20 g_FieldLayer4StateE88 = {0};
// GLOBAL: FF7 0x00cffe8c
struct FieldLayer4StateE8c g_FieldLayer4StateE8c = {0};
// GLOBAL: FF7 0x00cffe90
unsigned short g_FieldLayer4TilesNum = 0;
// GLOBAL: FF7 0x00cffe94
struct FieldBgLayer4Tile *g_FieldLayer4Tiles = 0;
// GLOBAL: FF7 0x00cffe98
struct FieldLayer4StateE98 g_FieldLayer4StateE98 = {0};
// 32-bit state word of two 16-bit halves; callers read the upper half.
// GLOBAL: FF7 0x00cffe9c
struct FieldLayer4StateE9c g_FieldLayer4StateE9c = {0};
// GLOBAL: FF7 0x00cffea0
unsigned short g_FieldLayer4StateEa0 = 0;
// GLOBAL: FF7 0x00cffea4
int g_FieldDoDrawLayer4 = 0;
// GLOBAL: FF7 0x00cffea8
int g_FieldLayer4Flag = 0;
unsigned char g_bssPad_00cffeac[4] = {0};
unsigned char g_bssPad_00cffeb0[28] = {0};
// GLOBAL: FF7 0x00cffecc
int g_FieldCameraScrollEcc = 0;
// GLOBAL: FF7 0x00cffed0
short g_FieldCameraScrollEd0 = 0;
// GLOBAL: FF7 0x00cffed4
short g_FieldCameraScrollEd4 = 0;
// GLOBAL: FF7 0x00cffed8
short g_FieldCameraScrollEd8 = 0;
// GLOBAL: FF7 0x00cffedc
short g_FieldCameraScrollEdc = 0;
// clang-format on

/*
 * Four groups, each a handful of counters cleared and then one buffer released: the sprite array,
 * the tile-record array, a third at g_FieldLayer4Tiles and a fourth at g_FieldLayer3Tiles.
 * Everything here is a store or a guarded free, which is why it is 381 bytes of nothing but.
 * g_FieldCameraScrollE74 is written twice with the same zero, once before the group and once after
 * g_FieldLayer1TilesNum. That is in the image.
 */
// FUNCTION: FF7 0x00642950
void __cdecl Field_ResetBackgroundLayers(void)
{
    (g_FieldLayer1StateE58.spanWidth) = 0;
    g_FieldLayer1StateE58.spanHeight = 0;
    (g_FieldCameraScrollE70.clutId) = 4;
    g_FieldCameraScrollE70.first = 0;
    g_FieldCameraScrollE74 = 0;
    g_FieldLayer1TilesNum = 0;
    g_FieldCameraScrollE74 = 0;
    if (g_FieldLayer1Tiles != 0) {
        // STRING: FF7 0x00907e10
        GameHeapFree(g_FieldLayer1Tiles, "C:\\FF7\\field\\src\\ad_bk.cpp", 0x3a);
        g_FieldLayer1Tiles = 0;
    }
    (g_FieldLayer2StateDf8.width) = 0;
    g_FieldLayer2StateDf8.height = 0;
    g_FieldLayer2TilesNum = 0;
    g_FieldLayer2ParamE04 = 0;
    if (g_FieldLayer2Tiles != 0) {
        // STRING: FF7 0x00907e2c
        GameHeapFree(g_FieldLayer2Tiles, "C:\\FF7\\field\\src\\ad_bk.cpp", 0x44);
        g_FieldLayer2Tiles = 0;
    }
    (g_FieldLayer4StateE8c.width) = 0;
    g_FieldLayer4StateE8c.height = 0;
    g_FieldLayer4TilesNum = 0;
    (g_FieldLayer4StateE98.headerWord) = 0;
    g_FieldDoDrawLayer4 = 0;
    g_FieldLayer4Flag = 0;
    if (g_FieldLayer4Tiles != 0) {
        // STRING: FF7 0x00907e48
        GameHeapFree(g_FieldLayer4Tiles, "C:\\FF7\\field\\src\\ad_bk.cpp", 0x4f);
        g_FieldLayer4Tiles = 0;
    }
    (g_FieldLayer3StateE24.width) = 0;
    g_FieldLayer3StateE24.height = 0;
    g_FieldLayer3TilesNum = 0;
    (g_FieldLayer3StateE30.firstIndex) = 1;
    g_FieldDoDrawLayer3 = 0;
    g_FieldLayer3Flag = 0;
    if (g_FieldLayer3Tiles != 0) {
        // STRING: FF7 0x00907e64
        GameHeapFree(g_FieldLayer3Tiles, "C:\\FF7\\field\\src\\ad_bk.cpp", 0x5c);
        g_FieldLayer3Tiles = 0;
    }
}

// Releases the tile-record array, but only when g_FieldLayer2ParamE04 is nonzero. It frees
// g_pFieldTileDrawOrder if set, then clears inUse, sampleCount and active in every record of
// g_FieldLayer2Tiles and frees that array. It also zeroes the width and height of
// g_FieldLayer2StateDf8 and clears g_FieldLayer2ParamE04. The three record fields are cleared
// before the array goes, which is either an owner pointer being unhooked or a debug habit; nothing
// here says which.
// FUNCTION: FF7 0x00642acd
void __cdecl Field_FreeTileRecordArray(void)
{
    unsigned short i;

    if (g_FieldLayer2ParamE04 == 0) {
        return;
    }
    if (g_pFieldTileDrawOrder != 0) {
        // STRING: FF7 0x00907e80
        GameHeapFree(g_pFieldTileDrawOrder, "C:\\FF7\\field\\src\\ad_bk.cpp", 0x6a);
        g_pFieldTileDrawOrder = 0;
    }
    if (g_FieldLayer2Tiles != 0) {
        for (i = 0; i < g_FieldLayer2TilesNum; i++) {
            g_FieldLayer2Tiles[i].inUse = 0;
            g_FieldLayer2Tiles[i].sampleCount = 0;
            g_FieldLayer2Tiles[i].active = 0;
        }
        // STRING: FF7 0x00907e9c
        GameHeapFree(g_FieldLayer2Tiles, "C:\\FF7\\field\\src\\ad_bk.cpp", 0x75);
        g_FieldLayer2Tiles = 0;
    }
    (g_FieldLayer2StateDf8.width) = 0;
    g_FieldLayer2StateDf8.height = 0;
    g_FieldLayer2ParamE04 = 0;
}

/*
 * Frees the field background: the block at g_FieldLayer1Tiles, the tile record array, and the two layer
 * tables at g_FieldLayer4Tiles and g_FieldLayer3Tiles (0x105C-byte entries whose three trailing fields are
 * cleared first) when their flags g_FieldDoDrawLayer4 and g_FieldDoDrawLayer3 say they exist, then the tile
 * records and the index arrays.
 */
// FUNCTION: FF7 0x00642bdc
void __cdecl Field_FreeBackgroundLayersAndTileRecords(void)
{
    unsigned short i;

    if (g_FieldLayer1Tiles != 0) {
        // STRING: FF7 0x00907eb8
        GameHeapFree(g_FieldLayer1Tiles, "C:\\FF7\\field\\src\\ad_bk.cpp", 0x86);
        g_FieldLayer1Tiles = 0;
    }
    g_FieldLayer1StateE48 = 0;
    g_FieldCameraScrollE74 = 0;
    Field_FreeTileRecordArray();
    if (g_FieldDoDrawLayer4 != 0) {
        g_FieldDoDrawLayer4 = 0;
        if (g_FieldLayer4Tiles != 0) {
            for (i = 0; i < g_FieldLayer4TilesNum; i++) {
                g_FieldLayer4Tiles[i].active = 0;
                g_FieldLayer4Tiles[i].freeMark = 0;
                g_FieldLayer4Tiles[i].inUse = 0;
            }
            // STRING: FF7 0x00907ed4
            GameHeapFree(g_FieldLayer4Tiles, "C:\\FF7\\field\\src\\ad_bk.cpp", 0x99);
            g_FieldLayer4Tiles = 0;
        }
    }
    if (g_FieldDoDrawLayer3 != 0) {
        g_FieldDoDrawLayer3 = 0;
        if (g_FieldLayer3Tiles != 0) {
            for (i = 0; i < g_FieldLayer3TilesNum; i++) {
                g_FieldLayer3Tiles[i].ownerTag = 0;
                g_FieldLayer3Tiles[i].freeMark = 0;
                g_FieldLayer3Tiles[i].inUse = 0;
            }
            // STRING: FF7 0x00907ef0
            GameHeapFree(g_FieldLayer3Tiles, "C:\\FF7\\field\\src\\ad_bk.cpp", 0xAA);
            g_FieldLayer3Tiles = 0;
        }
    }
    Field_FreeTileRecords();
    Field_FreeBackgroundIndexArrays();
}

/*
 * The same teardown as Field_FreeBackgroundLayersAndTileRecords, ending in Field_FreeTileTextures instead of Field_FreeTileRecords. Both
 * are in the image.
 */
// FUNCTION: FF7 0x00642dd0
void __cdecl Field_FreeBackgroundLayers(void)
{
    unsigned short i;

    if (g_FieldLayer1Tiles != 0) {
        // STRING: FF7 0x00907f0c
        GameHeapFree(g_FieldLayer1Tiles, "C:\\FF7\\field\\src\\ad_bk.cpp", 0xBA);
        g_FieldLayer1Tiles = 0;
    }
    g_FieldLayer1StateE48 = 0;
    g_FieldCameraScrollE74 = 0;
    Field_FreeTileRecordArray();
    if (g_FieldDoDrawLayer4 != 0) {
        g_FieldDoDrawLayer4 = 0;
        if (g_FieldLayer4Tiles != 0) {
            for (i = 0; i < g_FieldLayer4TilesNum; i++) {
                g_FieldLayer4Tiles[i].active = 0;
                g_FieldLayer4Tiles[i].freeMark = 0;
                g_FieldLayer4Tiles[i].inUse = 0;
            }
            // STRING: FF7 0x00907f28
            GameHeapFree(g_FieldLayer4Tiles, "C:\\FF7\\field\\src\\ad_bk.cpp", 0xCD);
            g_FieldLayer4Tiles = 0;
        }
    }
    if (g_FieldDoDrawLayer3 != 0) {
        g_FieldDoDrawLayer3 = 0;
        if (g_FieldLayer3Tiles != 0) {
            for (i = 0; i < g_FieldLayer3TilesNum; i++) {
                g_FieldLayer3Tiles[i].ownerTag = 0;
                g_FieldLayer3Tiles[i].freeMark = 0;
                g_FieldLayer3Tiles[i].inUse = 0;
            }
            // STRING: FF7 0x00907f44
            GameHeapFree(g_FieldLayer3Tiles, "C:\\FF7\\field\\src\\ad_bk.cpp", 0xDE);
            g_FieldLayer3Tiles = 0;
        }
    }
    Field_FreeTileTextures();
    Field_FreeBackgroundIndexArrays();
}
#pragma intrinsic(abs)

// 0x0064311C pushes the angle as one byte (mov cl, then push ecx).
extern short __cdecl Field_CosineByteAngle(unsigned char angle);
// The CRT's qsort, marked in include/lib/crt.h; declared by hand so the rest
// of <stdlib.h> stays out of this TU.

/*
 * Builds the model draw order: for each of the g_FieldLayer2TilesNum models writes (index, depth) into the
 * 4-byte pairs at g_pFieldTileDrawOrder, the depth being the sortKey of the model's 0x105C-byte
 * record in g_FieldLayer2Tiles, then qsorts the pairs by depth descending with Field_CompareDrawOrderDepthDescending.
 */
// FUNCTION: FF7 0x00642fc4
void __cdecl Field_BuildModelDrawOrder(void)
{
    unsigned short i;

    for (i = 0; i < g_FieldLayer2TilesNum; i++) {
        g_pFieldTileDrawOrder[i].tileIndex = i;
        g_pFieldTileDrawOrder[i].sortKey = g_FieldLayer2Tiles[i].sortKey;
    }
    qsort(g_pFieldTileDrawOrder, g_FieldLayer2TilesNum, 4, (int(__cdecl *)(const void *, const void *))Field_CompareDrawOrderDepthDescending);
}

/*
 * qsort comparator on the unsigned short sortKey (+2) of two FieldTileDrawOrder pairs, sorting descending: -1, 0 or 1.
 * Field_BuildModelDrawOrder orders its (index, depth) pairs with it.
 */
// FUNCTION: FF7 0x0064305d
int __cdecl Field_CompareDrawOrderDepthDescending(void *firstPair, void *secondPair)
{
    if (*(unsigned short *)((char *)firstPair + 2) > *(unsigned short *)((char *)secondPair + 2)) {
        return -1;
    }
    if (*(unsigned short *)((char *)firstPair + 2) == *(unsigned short *)((char *)secondPair + 2)) {
        return 0;
    }
    return 1;
}

/*
 * Empty; the frame reserves one unused slot.
 */
// FUNCTION: FF7 0x0064309c
void __cdecl Field_MapResetHookNoOp(void)
{
    int unused; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
}

/*
 * Adds a scaled end-start difference to start, multiplying before division when difference is
 * strictly between -0x80000 and 0x80000, dividing first otherwise. Returns the result.
 */
// FUNCTION: FF7 0x006430a4
int __cdecl Field_InterpolateLinear(int start, int end, int duration, int elapsed)
{
    int diff;

    diff = end - start;
    if (diff > -0x80000 && diff < 0x80000) {
        start += diff * elapsed / duration;
    } else {
        start += diff / duration * elapsed;
    }
    return start;
}

/*
 * Eases value toward end along a cosine: the phase elapsed/duration becomes a 256-step angle offset by
 * 128, Field_CosineByteAngle gives its cosine in 1/4096 units, and (cos + 4096) * (end - value) / 8192 is
 * added to value.
 */
// FUNCTION: FF7 0x006430f2
int __cdecl Field_InterpolateCosine(int value, int end, int duration, int elapsed)
{
    int diff;
    unsigned char ang;

    ang = (unsigned char)((elapsed << 12) / duration / 32 + 0x80);
    diff = end - value;
    value += (Field_CosineByteAngle(ang) + 0x1000) * diff / 0x2000;
    return value;
}

/*
 * Projects one point through the field camera. It loads g_FieldCameraTransform as both the rotation and the
 * translation matrix, switches the geometry offset to g_FieldDisplayOffsetX and g_FieldDisplayHeight, calls
 * RotTransPers(point, screenPos, &flag, &depth), restores the offset to g_FieldDisplayCenterX and g_FieldDisplayCenterY,
 * and returns the RotTransPers result. The two output slots are written to the frame and never
 * read.
 */
// FUNCTION: FF7 0x0064314f
int __cdecl Field_ProjectWorldToScreen(PsxSVectorZ32 *point, void *screenPos)
{
    int flag;
    int res;
    int depthOut;

    SetRotMatrix(&g_FieldCameraTransform);
    SetTransMatrix(&g_FieldCameraTransform);
    SetGeomOffset(g_FieldDisplayOffsetX, g_FieldDisplayHeight);
    res = RotTransPers((SVECTOR *)point, (short *)screenPos, &flag, &depthOut);
    SetGeomOffset(g_FieldDisplayCenterX, g_FieldDisplayCenterY);
    return res;
}

/*
 * Advances a SHORT interpolation state for a byte output. Enabled mode initializes start zero and
 * a random scaled target, then after elapsed exceeds duration copies the target to start and
 * selects a target of opposite sign while advancing a signed byte random-table index. During
 * interpolation increments elapsed before calling the helper. Disabled mode with active state one
 * begins a transition toward zero after elapsed exceeds duration and clears that active state;
 * otherwise continues interpolation. Fully inactive mode clears output only when elapsed equals
 * duration, otherwise increments and interpolates. Random scaling narrows the product differently
 * in the positive and negative branches.
 */
// FUNCTION: FF7 0x006431bb
void __cdecl Field_UpdateScreenShakeAxis(FieldScreenShake *shake)
{
    if (shake->active == 1) {
        if (shake->phase == 0) {
            shake->stepCount = 0;
            shake->previousOffset = 0;
            shake->currentOffset = ((short)(g_FieldRandomTable[shake->randomIndex] * shake->amplitude)) / 256;
            (shake->randomIndex)++;
            shake->phase = 1;
        } else if (shake->step < shake->stepCount) {
            shake->stepCount = 0;
            shake->previousOffset = shake->currentOffset;
            if (shake->previousOffset < 0) {
                shake->currentOffset = ((short)(g_FieldRandomTable[shake->randomIndex] * shake->amplitude)) / 256;
            } else {
                shake->currentOffset = (-(short)(g_FieldRandomTable[shake->randomIndex] * shake->amplitude)) / 256;
            }
            (shake->randomIndex)++;
        } else {
            (shake->stepCount)++;
            shake->blendedOffset = Field_InterpolateCosine(shake->previousOffset, shake->currentOffset, shake->step, shake->stepCount);
        }
    } else if (shake->phase == 1) {
        if (shake->step < shake->stepCount) {
            shake->stepCount = 0;
            shake->previousOffset = shake->currentOffset;
            shake->currentOffset = 0;
            (shake->randomIndex)++;
            shake->phase = 0;
        } else {
            (shake->stepCount)++;
            shake->blendedOffset = Field_InterpolateCosine(shake->previousOffset, shake->currentOffset, shake->step, shake->stepCount);
        }
    } else if (shake->step == shake->stepCount) {
        shake->blendedOffset = 0;
    } else {
        (shake->stepCount)++;
        shake->blendedOffset = Field_InterpolateCosine(shake->previousOffset, shake->currentOffset, shake->step, shake->stepCount);
    }
}

/*
 * Obtains the engine context and, only when cameraScrollState is REQUESTED, initializes the global
 * transition state from the signed cameraScrollMode. RESET disables and clears current offsets;
 * FOLLOW enables without resetting them; FIXED installs immediate signed SHORT offsets. The
 * TO_TARGET modes save current offsets and initialize duration and progress; the TO_POINT modes
 * also copy target offsets. Marks immediate modes DONE and timed modes RUNNING. Unsupported modes
 * leave the request state REQUESTED and perform no writes; the retrieved context is unused.
 */
// FUNCTION: FF7 0x0064341c
void __cdecl Field_BeginCameraTransitionFromRequest(void)
{
    GfxEngineContext *ctx;

    ctx = GetEngineContext();
    if (*(char *)&g_FieldActorState.state.cameraScrollState == FIELD_CAMERA_SCROLL_STATE_REQUESTED) {
        switch (g_FieldActorState.state.cameraScrollMode) {
        case FIELD_CAMERA_SCROLL_RESET:
            g_FieldBgFlag = 0;
            g_FieldLayerScrollX = 0;
            g_FieldLayerScrollY = 0;
            g_FieldActorState.state.cameraScrollState = FIELD_CAMERA_SCROLL_STATE_DONE;
            break;
        case FIELD_CAMERA_SCROLL_FOLLOW:
            g_FieldBgFlag = 1;
            g_FieldActorState.state.cameraScrollState = FIELD_CAMERA_SCROLL_STATE_RUNNING;
            break;
        case FIELD_CAMERA_SCROLL_TO_TARGET_LINEAR:
            g_FieldBgFlag = 1;
            g_FieldLayerScrollSteps = g_FieldActorState.state.cameraScrollNumSteps;
            g_FieldLayerScrollStepIndex = 0;
            g_FieldLayerScrollStartX = g_FieldLayerScrollX;
            g_FieldLayerScrollStartY = g_FieldLayerScrollY;
            g_FieldActorState.state.cameraScrollState = FIELD_CAMERA_SCROLL_STATE_RUNNING;
            break;
        case FIELD_CAMERA_SCROLL_TO_TARGET_SMOOTH:
            g_FieldBgFlag = 1;
            g_FieldLayerScrollSteps = g_FieldActorState.state.cameraScrollNumSteps;
            g_FieldLayerScrollStepIndex = 0;
            g_FieldLayerScrollStartX = g_FieldLayerScrollX;
            g_FieldLayerScrollStartY = g_FieldLayerScrollY;
            g_FieldActorState.state.cameraScrollState = FIELD_CAMERA_SCROLL_STATE_RUNNING;
            break;
        case FIELD_CAMERA_SCROLL_FIXED:
            g_FieldBgFlag = 1;
            g_FieldLayerScrollX = g_FieldActorState.state.cameraScrollTargetX;
            g_FieldLayerScrollY = g_FieldActorState.state.cameraScrollTargetY;
            g_FieldActorState.state.cameraScrollState = FIELD_CAMERA_SCROLL_STATE_DONE;
            break;
        case FIELD_CAMERA_SCROLL_TO_POINT_LINEAR:
            g_FieldBgFlag = 1;
            g_FieldLayerScrollSteps = g_FieldActorState.state.cameraScrollNumSteps;
            g_FieldLayerScrollStepIndex = 0;
            g_FieldLayerScrollStartX = g_FieldLayerScrollX;
            g_FieldLayerScrollStartY = g_FieldLayerScrollY;
            g_FieldCameraScrollX = g_FieldActorState.state.cameraScrollTargetX;
            g_FieldCameraScrollY = g_FieldActorState.state.cameraScrollTargetY;
            g_FieldActorState.state.cameraScrollState = FIELD_CAMERA_SCROLL_STATE_RUNNING;
            break;
        case FIELD_CAMERA_SCROLL_TO_POINT_SMOOTH:
            g_FieldBgFlag = 1;
            g_FieldLayerScrollSteps = g_FieldActorState.state.cameraScrollNumSteps;
            g_FieldLayerScrollStepIndex = 0;
            g_FieldLayerScrollStartX = g_FieldLayerScrollX;
            g_FieldLayerScrollStartY = g_FieldLayerScrollY;
            g_FieldCameraScrollX = g_FieldActorState.state.cameraScrollTargetX;
            g_FieldCameraScrollY = g_FieldActorState.state.cameraScrollTargetY;
            g_FieldActorState.state.cameraScrollState = FIELD_CAMERA_SCROLL_STATE_RUNNING;
        }
    }
}

/*
 * For diagonal mode one, projects the supplied SHORT camera pair using the inset rectangle
 * diagonal from (left plus 160, top plus 120) to (right minus 160, bottom minus 120), with integer
 * dot products and a denominator shifted right eight. Mode two uses the opposite vertical diagonal
 * but its denominator multiplies two differing vertical expressions, one subtracting 120 from the
 * first bound and one adding 120. Writes narrowed SHORT coordinates using division followed by
 * another right shift eight. Other modes do nothing. No segment-end clamp or zero-denominator
 * guard is present, so the name alone does not prove bounded camera clamping.
 */
// FUNCTION: FF7 0x00643628
void __cdecl Field_ClampCameraToDiagonal(FieldCameraClamp *clamp, FieldCameraCoordsRect *camera)
{
    int insetHeight;
    int dot;
    int normSq;
    int insetWidth;

    if (clamp->clampMode == 1) {
        dot = -((clamp->minX + 0xA0 - camera->x) * (clamp->maxX - 0xA0 - (clamp->minX + 0xA0)) + (clamp->minY + 0x78 - camera->y) * (clamp->maxY - 0x78 - (clamp->minY + 0x78)));
        normSq = (clamp->maxX - 0xA0 - (clamp->minX + 0xA0)) * (clamp->maxX - 0xA0 - (clamp->minX + 0xA0)) + (clamp->maxY - 0x78 - (clamp->minY + 0x78)) * (clamp->maxY - 0x78 - (clamp->minY + 0x78)) >> 8;
        insetWidth = clamp->maxX - 0xA0 - (clamp->minX + 0xA0);
        insetHeight = clamp->maxY - 0x78 - (clamp->minY + 0x78);
        camera->x = (dot * insetWidth / normSq >> 8) + clamp->minX + 0xA0;
        camera->y = (dot * insetHeight / normSq >> 8) + clamp->minY + 0x78;
    }
    if (clamp->clampMode == 2) {
        dot = -((clamp->minX + 0xA0 - camera->x) * (clamp->maxX - 0xA0 - (clamp->minX + 0xA0)) + (clamp->maxY - 0x78 - camera->y) * (clamp->minY + 0x78 - (clamp->maxY - 0x78)));
        normSq = (clamp->maxX - 0xA0 - (clamp->minX + 0xA0)) * (clamp->maxX - 0xA0 - (clamp->minX + 0xA0)) + (clamp->minY - 0x78 - (clamp->maxY - 0x78)) * (clamp->minY + 0x78 - (clamp->maxY - 0x78)) >> 8;
        insetWidth = clamp->maxX - 0xA0 - (clamp->minX + 0xA0);
        insetHeight = clamp->minY + 0x78 - (clamp->maxY - 0x78);
        camera->x = (dot * insetWidth / normSq >> 8) + clamp->minX + 0xA0;
        camera->y = (dot * insetHeight / normSq >> 8) + clamp->maxY + -0x78;
    }
}

/*
 * Applies upper then lower clamps to two shorts using trigger rectangle inset by 160 horizontally
 * and 120 vertically. If the inset rectangle is inverted the later lower clamp wins.
 */
// FUNCTION: FF7 0x006438f6
void __cdecl Field_ClampCameraToRect(FieldCameraCoordsRect *camera)
{
    if (camera->x > g_pFieldScrollBounds->maxX - 0xA0) {
        camera->x = g_pFieldScrollBounds->maxX - 0xA0;
    }
    if (camera->x < g_pFieldScrollBounds->minX + 0xA0) {
        camera->x = g_pFieldScrollBounds->minX + 0xA0;
    }
    if (camera->y > g_pFieldScrollBounds->maxY - 0x78) {
        camera->y = g_pFieldScrollBounds->maxY - 0x78;
    }
    if (camera->y < g_pFieldScrollBounds->minY + 0x78) {
        camera->y = g_pFieldScrollBounds->minY + 0x78;
    }
}

/*
 * Mechanical clear: zeroes g_FieldCameraScrollEd0, g_FieldCameraScrollEd4, g_FieldCameraScrollEd8 and g_FieldCameraScrollEdc. Nothing in
 * the tree has named those four.
 */
// FUNCTION: FF7 0x006439af
void __cdecl Field_ResetScrollAccelerationCounters(void)
{
    g_FieldCameraScrollEd0 = 0;
    g_FieldCameraScrollEd4 = 0;
    g_FieldCameraScrollEd8 = 0;
    g_FieldCameraScrollEdc = 0;
}

/*
 * For each of four enabled global accelerations, adds its accumulator times acceleration narrowed
 * to SHORT into the corresponding supplied coordinate and increments the accumulator. Applies
 * sequential lower and upper viewport bounds using trigger dimensions: horizontal margin 160 and
 * vertical margin 120. Resets that axis accumulator when either boundary check clamps the
 * coordinate. Disabled axes are untouched. Undersized dimensions can make the upper check
 * overwrite the lower clamp.
 */
// FUNCTION: FF7 0x006439d8
void __cdecl Field_AccelerateScrollPositions(short *scrollX, short *scrollY, short *scrollX2, short *scrollY2)
{
    if (g_FieldActorState.state.layer2ScrollXSpeed != 0) {
        *scrollX += (short)(g_FieldCameraScrollEd0 * g_FieldActorState.state.layer2ScrollXSpeed);
        g_FieldCameraScrollEd0++;
        if (*scrollX < 0xA0) {
            *scrollX = 0xA0;
            g_FieldCameraScrollEd0 = 0;
        }
        if (*scrollX > g_pFieldScrollBounds->maxX - g_pFieldScrollBounds->minX - 0xA0) {
            *scrollX = g_pFieldScrollBounds->maxX - g_pFieldScrollBounds->minX - 0xA0;
            g_FieldCameraScrollEd0 = 0;
        }
    }
    if (g_FieldActorState.state.layer2ScrollYSpeed != 0) {
        *scrollY += (short)(g_FieldCameraScrollEd4 * g_FieldActorState.state.layer2ScrollYSpeed);
        g_FieldCameraScrollEd4++;
        if (*scrollY < 0x78) {
            *scrollY = 0x78;
            g_FieldCameraScrollEd4 = 0;
        }
        if (*scrollY > g_pFieldScrollBounds->maxY - g_pFieldScrollBounds->minY - 0x78) {
            *scrollY = g_pFieldScrollBounds->maxY - g_pFieldScrollBounds->minY - 0x78;
            g_FieldCameraScrollEd4 = 0;
        }
    }
    if (g_FieldActorState.state.layer3ScrollXSpeed != 0) {
        *scrollX2 += (short)(g_FieldCameraScrollEd8 * g_FieldActorState.state.layer3ScrollXSpeed);
        g_FieldCameraScrollEd8++;
        if (*scrollX2 < 0xA0) {
            *scrollX2 = 0xA0;
            g_FieldCameraScrollEd8 = 0;
        }
        if (*scrollX2 > g_pFieldScrollBounds->maxX - g_pFieldScrollBounds->minX - 0xA0) {
            *scrollX2 = g_pFieldScrollBounds->maxX - g_pFieldScrollBounds->minX - 0xA0;
            g_FieldCameraScrollEd8 = 0;
        }
    }
    if (g_FieldActorState.state.layer3ScrollYSpeed != 0) {
        *scrollY2 += (short)(g_FieldCameraScrollEdc * g_FieldActorState.state.layer3ScrollYSpeed);
        g_FieldCameraScrollEdc++;
        if (*scrollY2 < 0x78) {
            *scrollY2 = 0x78;
            g_FieldCameraScrollEdc = 0;
        }
        if (*scrollY2 > g_pFieldScrollBounds->maxY - g_pFieldScrollBounds->minY - 0x78) {
            *scrollY2 = g_pFieldScrollBounds->maxY - g_pFieldScrollBounds->minY - 0x78;
            g_FieldCameraScrollEdc = 0;
        }
    }
}

/*
 * Selects a stride 0x88 field record with a signed byte index, narrows its three fixed-point
 * coordinates after shift twelve, adds a global short to the third, projects the packed triple
 * into screenPos and subtracts the global camera-origin shorts from the resulting pair.
 */
// FUNCTION: FF7 0x00643c86
void __cdecl Field_ProjectCameraTargetEntityToScreen(FieldCameraCoordsRect *screenPos)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[8];
        short targetX;
        struct { unsigned char pad_to_2[0x2]; short targetY; };
        struct { unsigned char pad_to_4[0x4]; short targetZ; };
    } frame;
#pragma pack(pop)

    frame.targetX = g_FieldEntityState[g_FieldActorState.state.cameraScrollTargetId].posX >> 0xC;
    frame.targetY = g_FieldEntityState[g_FieldActorState.state.cameraScrollTargetId].posY >> 0xC;
    frame.targetZ = (short)(g_FieldEntityState[g_FieldActorState.state.cameraScrollTargetId].posZ >> 0xC) + g_FieldActorState.state.viewOffset;
    Field_ProjectWorldToScreen((PsxSVectorZ32 *)&frame.targetX, screenPos);
    screenPos->x -= *(short *)&g_FieldDisplayOffsetX;
    screenPos->y -= *(short *)&g_FieldDisplayHeight;
}

/*
 * Steps the field camera transition only while cameraScrollState is RUNNING. FOLLOW continually
 * computes and rectangle-clamps a camera point and stores its negation. The TO_TARGET modes
 * interpolate saved offsets toward that current clamped point; the TO_POINT modes interpolate
 * toward stored fixed targets, linearly or by cosine. Timed modes with zero
 * duration finish immediately without changing offsets; otherwise finish when elapsed equals
 * duration after interpolation, or increment elapsed. Unsupported modes leave state and offsets
 * unchanged. The packed union explicitly backs the temporary point.
 */
// FUNCTION: FF7 0x00643d22
void __cdecl Field_StepCameraScrollTransition(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[8];
        FieldCameraCoordsRect clampedX;
    } frame;
#pragma pack(pop)

    if (*(char *)&g_FieldActorState.state.cameraScrollState == FIELD_CAMERA_SCROLL_STATE_RUNNING) {
        switch (g_FieldActorState.state.cameraScrollMode) {
        case FIELD_CAMERA_SCROLL_RESET:
            break;
        case FIELD_CAMERA_SCROLL_FOLLOW:
            Field_ProjectCameraTargetEntityToScreen(&frame.clampedX);
            Field_ClampCameraToRect(&frame.clampedX);
            g_FieldLayerScrollX = -frame.clampedX.x;
            g_FieldLayerScrollY = -frame.clampedX.y;
            break;
        case FIELD_CAMERA_SCROLL_TO_TARGET_LINEAR:
            Field_ProjectCameraTargetEntityToScreen(&frame.clampedX);
            Field_ClampCameraToRect(&frame.clampedX);
            if (g_FieldLayerScrollSteps == 0) {
                g_FieldActorState.state.cameraScrollState = FIELD_CAMERA_SCROLL_STATE_DONE;
                return;
            }
            g_FieldLayerScrollX = Field_InterpolateLinear(g_FieldLayerScrollStartX, -frame.clampedX.x, g_FieldLayerScrollSteps, g_FieldLayerScrollStepIndex);
            g_FieldLayerScrollY = Field_InterpolateLinear(g_FieldLayerScrollStartY, -frame.clampedX.y, g_FieldLayerScrollSteps, g_FieldLayerScrollStepIndex);
            if (g_FieldLayerScrollSteps == g_FieldLayerScrollStepIndex) {
                g_FieldActorState.state.cameraScrollState = FIELD_CAMERA_SCROLL_STATE_DONE;
                return;
            }
            g_FieldLayerScrollStepIndex++;
            break;
        case FIELD_CAMERA_SCROLL_TO_TARGET_SMOOTH:
            Field_ProjectCameraTargetEntityToScreen(&frame.clampedX);
            Field_ClampCameraToRect(&frame.clampedX);
            if (g_FieldLayerScrollSteps == 0) {
                g_FieldActorState.state.cameraScrollState = FIELD_CAMERA_SCROLL_STATE_DONE;
                return;
            }
            g_FieldLayerScrollX = Field_InterpolateCosine(g_FieldLayerScrollStartX, -frame.clampedX.x, g_FieldLayerScrollSteps, g_FieldLayerScrollStepIndex);
            g_FieldLayerScrollY = Field_InterpolateCosine(g_FieldLayerScrollStartY, -frame.clampedX.y, g_FieldLayerScrollSteps, g_FieldLayerScrollStepIndex);
            if (g_FieldLayerScrollSteps == g_FieldLayerScrollStepIndex) {
                g_FieldActorState.state.cameraScrollState = FIELD_CAMERA_SCROLL_STATE_DONE;
                return;
            }
            g_FieldLayerScrollStepIndex++;
            break;
        case FIELD_CAMERA_SCROLL_TO_POINT_LINEAR:
            if (g_FieldLayerScrollSteps == 0) {
                g_FieldActorState.state.cameraScrollState = FIELD_CAMERA_SCROLL_STATE_DONE;
                return;
            }
            g_FieldLayerScrollX = Field_InterpolateLinear(g_FieldLayerScrollStartX, g_FieldCameraScrollX, g_FieldLayerScrollSteps, g_FieldLayerScrollStepIndex);
            g_FieldLayerScrollY = Field_InterpolateLinear(g_FieldLayerScrollStartY, g_FieldCameraScrollY, g_FieldLayerScrollSteps, g_FieldLayerScrollStepIndex);
            if (g_FieldLayerScrollSteps == g_FieldLayerScrollStepIndex) {
                g_FieldActorState.state.cameraScrollState = FIELD_CAMERA_SCROLL_STATE_DONE;
                return;
            }
            g_FieldLayerScrollStepIndex++;
            break;
        case FIELD_CAMERA_SCROLL_TO_POINT_SMOOTH:
            if (g_FieldLayerScrollSteps == 0) {
                g_FieldActorState.state.cameraScrollState = FIELD_CAMERA_SCROLL_STATE_DONE;
            } else {
                g_FieldLayerScrollX = Field_InterpolateCosine(g_FieldLayerScrollStartX, g_FieldCameraScrollX, g_FieldLayerScrollSteps, g_FieldLayerScrollStepIndex);
                g_FieldLayerScrollY = Field_InterpolateCosine(g_FieldLayerScrollStartY, g_FieldCameraScrollY, g_FieldLayerScrollSteps, g_FieldLayerScrollStepIndex);
                if (g_FieldLayerScrollSteps == g_FieldLayerScrollStepIndex) {
                    g_FieldActorState.state.cameraScrollState = FIELD_CAMERA_SCROLL_STATE_DONE;
                } else {
                    g_FieldLayerScrollStepIndex++;
                }
            }
        }
    }
}

/*
 * Advances and wraps two background-layer scroll accumulators, then updates geometry projection
 * and camera offsets. Normal mode follows the projected player after rectangle/diagonal clamps or
 * uses scripted camera coordinates, deriving wrapped parallax-layer positions and screen offsets;
 * the followed-player vertical offset can interpolate linearly or smoothly. Updates player screen
 * coordinates from position plus entity offsets, conditionally skipping that projection for a
 * hidden player in scripted mode. The alternate mode sets a background flag and selects saved
 * scroll or camera-record offsets. Layer dimensions are unchecked modulo divisors.
 */
// FUNCTION: FF7 0x00644075
void __cdecl Field_UpdateCameraScroll(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[56];
        FieldCameraCoordsRect cameraX;
        struct { unsigned char pad_to_10[0x10]; short playerX; };
        struct { unsigned char pad_to_12[0x12]; short playerY; };
        struct { unsigned char pad_to_14[0x14]; short playerZ; };
        struct { unsigned char pad_to_18[0x18]; FieldCameraCoordsRect screenX; };
        struct { unsigned char pad_to_20[0x20]; short followX; };
        struct { unsigned char pad_to_22[0x22]; short followY; };
        struct { unsigned char pad_to_24[0x24]; short followZ; };
        struct { unsigned char pad_to_28[0x28]; short layer3Y; };
        struct { unsigned char pad_to_2C[0x2C]; short layer3X; };
        struct { unsigned char pad_to_30[0x30]; short layer2Y; };
        struct { unsigned char pad_to_34[0x34]; short layer2X; };
    } frame;
#pragma pack(pop)

    g_pFieldScrollBounds->layer2ScrollX += g_FieldActorState.state.layer2ScrollXSpeed;
    g_pFieldScrollBounds->layer2ScrollY += g_FieldActorState.state.layer2ScrollYSpeed;
    g_pFieldScrollBounds->layer2ScrollX = g_pFieldScrollBounds->layer2ScrollX % (g_pFieldScrollBounds->tileWidth << 4);
    g_pFieldScrollBounds->layer2ScrollY = g_pFieldScrollBounds->layer2ScrollY % (g_pFieldScrollBounds->tileHeight << 4);
    g_pFieldScrollBounds->layer3ScrollX += g_FieldActorState.state.layer3ScrollXSpeed;
    g_pFieldScrollBounds->layer3ScrollY += g_FieldActorState.state.layer3ScrollYSpeed;
    g_pFieldScrollBounds->layer3ScrollX = g_pFieldScrollBounds->layer3ScrollX % (g_pFieldScrollBounds->tileStrideX << 4);
    g_pFieldScrollBounds->layer3ScrollY = g_pFieldScrollBounds->layer3ScrollY % (g_pFieldScrollBounds->tileStrideY << 4);
    SetGeomScreen(g_pFieldCameraData->geomScreen);
    if (g_FieldLogicActive == 0 || g_FieldActorState.state.speedBoost != 0) {
        if (g_FieldBgFlag == 0) {
            switch (g_FieldActorState.state.viewOffsetMode) {
            case FIELD_VIEW_OFFSET_IMMEDIATE:
                break;
            case FIELD_VIEW_OFFSET_LINEAR:
                g_FieldActorState.state.viewOffset = (unsigned char)(Field_InterpolateLinear(g_FieldActorState.state.viewOffsetStart, g_FieldActorState.state.viewOffsetTarget, g_FieldActorState.state.viewOffsetNumSteps, g_FieldActorState.state.viewOffsetCurrentStep) & 0xFF);
                if (g_FieldActorState.state.viewOffsetCurrentStep == g_FieldActorState.state.viewOffsetNumSteps) {
                    g_FieldActorState.state.viewOffsetMode = FIELD_VIEW_OFFSET_DONE;
                } else {
                    (g_FieldActorState.state.viewOffsetCurrentStep)++;
                }
                break;
            case FIELD_VIEW_OFFSET_SMOOTH:
                g_FieldActorState.state.viewOffset = (unsigned char)(Field_InterpolateCosine(g_FieldActorState.state.viewOffsetStart, g_FieldActorState.state.viewOffsetTarget, g_FieldActorState.state.viewOffsetNumSteps, g_FieldActorState.state.viewOffsetCurrentStep) & 0xFF);
                if (g_FieldActorState.state.viewOffsetCurrentStep == g_FieldActorState.state.viewOffsetNumSteps) {
                    g_FieldActorState.state.viewOffsetMode = FIELD_VIEW_OFFSET_DONE;
                } else {
                    (g_FieldActorState.state.viewOffsetCurrentStep)++;
                }
            }
            frame.followX = g_FieldEntityState[g_FieldPlayerEntityIndex].posX >> 0xC;
            frame.followY = g_FieldEntityState[g_FieldPlayerEntityIndex].posY >> 0xC;
            frame.followZ = (short)(g_FieldEntityState[g_FieldPlayerEntityIndex].posZ >> 0xC) + g_FieldActorState.state.viewOffset;
            g_FieldProjectedScreenPos = Field_ProjectWorldToScreen((PsxSVectorZ32 *)&frame.followX, &frame.cameraX);
            frame.cameraX.x -= *(short *)&g_FieldDisplayOffsetX;
            frame.cameraX.y -= *(short *)&g_FieldDisplayHeight;
            Field_ClampCameraToRect(&frame.cameraX);
            Field_ClampCameraToDiagonal((FieldCameraClamp *)g_pFieldScrollBounds, &frame.cameraX);
            g_FieldLayerScrollX = -frame.cameraX.x;
            g_FieldLayerScrollY = -frame.cameraX.y;
            frame.layer2X = (short)(frame.cameraX.x * g_pFieldScrollBounds->layer2ParallaxX >> 8) + (g_pFieldScrollBounds->layer2ScrollX >> 4);
            frame.layer2Y = (short)(frame.cameraX.y * g_pFieldScrollBounds->layer2ParallaxY >> 8) + (g_pFieldScrollBounds->layer2ScrollY >> 4);
            frame.layer2X = frame.layer2X % g_pFieldScrollBounds->tileWidth;
            frame.layer2Y = frame.layer2Y % g_pFieldScrollBounds->tileHeight;
            frame.layer3X = (short)(frame.cameraX.x * g_pFieldScrollBounds->layer3ParallaxX >> 8) + (g_pFieldScrollBounds->layer3ScrollX >> 4);
            frame.layer3Y = (short)(frame.cameraX.y * g_pFieldScrollBounds->layer3ParallaxY >> 8) + (g_pFieldScrollBounds->layer3ScrollY >> 4);
            frame.layer3X = frame.layer3X % g_pFieldScrollBounds->tileStrideX;
            frame.layer3Y = frame.layer3Y % g_pFieldScrollBounds->tileStrideY;
            g_FieldPaletteTargetOrgX = (short)(g_FieldBgLayer1ScrollX - frame.cameraX.x + g_FieldActorState.state.shakeX.currentOffset) - 0xA0;
            g_FieldPaletteTargetOrgY = (short)(g_FieldBgLayer1ScrollY - frame.cameraX.y + g_FieldActorState.state.shakeY.currentOffset) - 0x78;
            g_FieldBgLayer2ScrollX = 0x140 - g_FieldBgLayer1ScrollX + frame.cameraX.x - g_FieldActorState.state.shakeX.currentOffset;
            g_FieldBgLayer2ScrollY = 0xE8 - g_FieldBgLayer1ScrollY + frame.cameraX.y - g_FieldActorState.state.shakeY.currentOffset;
            g_FieldBgLayer3ScrollX = 0x140 - g_FieldBgLayer1ScrollX + frame.layer2X - g_FieldActorState.state.shakeX.currentOffset;
            g_FieldBgLayer3ScrollY = 0xE8 - g_FieldBgLayer1ScrollY + frame.layer2Y - g_FieldActorState.state.shakeY.currentOffset;
            g_FieldBgLayer4ScrollX = 0x140 - g_FieldBgLayer1ScrollX + frame.layer3X - g_FieldActorState.state.shakeX.currentOffset;
            g_FieldBgLayer4ScrollY = 0xE8 - g_FieldBgLayer1ScrollY + frame.layer3Y - g_FieldActorState.state.shakeY.currentOffset;
            frame.playerX = (short)(g_FieldEntityState[g_FieldPlayerEntityIndex].posX >> 0xC) + g_FieldEntityState[g_FieldPlayerEntityIndex].offsetX;
            frame.playerY = (short)(g_FieldEntityState[g_FieldPlayerEntityIndex].posY >> 0xC) + g_FieldEntityState[g_FieldPlayerEntityIndex].offsetY;
            frame.playerZ = (short)(g_FieldEntityState[g_FieldPlayerEntityIndex].posZ >> 0xC) + g_FieldEntityState[g_FieldPlayerEntityIndex].offsetZ + (short)(g_FieldActorState.state.fieldScale << 7 >> 9);
            Field_ProjectWorldToScreen((PsxSVectorZ32 *)&frame.playerX, &frame.screenX);
            g_FieldHandCursorX = frame.screenX.x * g_FieldDisplayScaleFactor + g_FieldDisplayHalfCenterX + g_FieldPaletteTargetOrgX * g_FieldDisplayScaleFactor;
            g_FieldHandCursorY = frame.screenX.y * g_FieldDisplayScaleFactor + g_FieldDisplayHalfCenterY + g_FieldPaletteTargetOrgY * g_FieldDisplayScaleFactor - (g_FieldDisplayScaleFactor << 3);
        } else {
            frame.layer2X = (short)(-g_FieldLayerScrollX * g_pFieldScrollBounds->layer2ParallaxX >> 8) + (g_pFieldScrollBounds->layer2ScrollX >> 4);
            frame.layer2Y = (short)(-g_FieldLayerScrollY * g_pFieldScrollBounds->layer2ParallaxY >> 8) + (g_pFieldScrollBounds->layer2ScrollY >> 4);
            frame.layer2X = frame.layer2X % g_pFieldScrollBounds->tileWidth;
            frame.layer2Y = frame.layer2Y % g_pFieldScrollBounds->tileHeight;
            frame.layer3X = (short)(-g_FieldLayerScrollX * g_pFieldScrollBounds->layer3ParallaxX >> 8) + (g_pFieldScrollBounds->layer3ScrollX >> 4);
            frame.layer3Y = (short)(-g_FieldLayerScrollY * g_pFieldScrollBounds->layer3ParallaxY >> 8) + (g_pFieldScrollBounds->layer3ScrollY >> 4);
            frame.layer3X = frame.layer3X % g_pFieldScrollBounds->tileStrideX;
            frame.layer3Y = frame.layer3Y % g_pFieldScrollBounds->tileStrideY;
            g_FieldPaletteTargetOrgX = (short)(g_FieldBgLayer1ScrollX - g_pFieldCameraData->scrollOffsetX + g_FieldLayerScrollX + g_FieldActorState.state.shakeX.currentOffset) - 0xA0;
            g_FieldPaletteTargetOrgY = (short)(g_FieldBgLayer1ScrollY + g_pFieldCameraData->scrollOffsetY + g_FieldLayerScrollY + g_FieldActorState.state.shakeY.currentOffset) - 0x78;
            g_FieldBgLayer2ScrollX = 0x140 - g_FieldBgLayer1ScrollX - g_FieldLayerScrollX - g_FieldActorState.state.shakeX.currentOffset;
            g_FieldBgLayer2ScrollY = 0xE8 - g_FieldBgLayer1ScrollY - g_FieldLayerScrollY - g_FieldActorState.state.shakeY.currentOffset;
            g_FieldBgLayer2ScrollY += (*(short *)&g_FieldBgLayer2OffsetY);
            g_FieldBgLayer3ScrollX = 0x140 - g_FieldBgLayer1ScrollX + frame.layer2X - g_FieldActorState.state.shakeX.currentOffset;
            g_FieldBgLayer3ScrollY = 0xE8 - g_FieldBgLayer1ScrollY + frame.layer2Y - g_FieldActorState.state.shakeY.currentOffset;
            g_FieldBgLayer4ScrollX = 0x140 - g_FieldBgLayer1ScrollX + frame.layer3X - g_FieldActorState.state.shakeX.currentOffset;
            g_FieldBgLayer4ScrollY = 0xE8 - g_FieldBgLayer1ScrollY + frame.layer3Y - g_FieldActorState.state.shakeY.currentOffset;
            if (*(char *)&g_FieldEntityState[g_FieldPlayerEntityIndex].visible != 0) {
                frame.playerX = (short)(g_FieldEntityState[g_FieldPlayerEntityIndex].posX >> 0xC) + g_FieldEntityState[g_FieldPlayerEntityIndex].offsetX;
                frame.playerY = (short)(g_FieldEntityState[g_FieldPlayerEntityIndex].posY >> 0xC) + g_FieldEntityState[g_FieldPlayerEntityIndex].offsetY;
                frame.playerZ = (short)(g_FieldEntityState[g_FieldPlayerEntityIndex].posZ >> 0xC) + g_FieldEntityState[g_FieldPlayerEntityIndex].offsetZ + (short)(g_FieldActorState.state.fieldScale << 7 >> 9);
                Field_ProjectWorldToScreen((PsxSVectorZ32 *)&frame.playerX, &frame.screenX);
                g_FieldHandCursorX = frame.screenX.x * g_FieldDisplayScaleFactor + g_FieldDisplayHalfCenterX + g_FieldPaletteTargetOrgX * g_FieldDisplayScaleFactor;
                g_FieldHandCursorY = frame.screenX.y * g_FieldDisplayScaleFactor + g_FieldDisplayHalfCenterY + g_FieldPaletteTargetOrgY * g_FieldDisplayScaleFactor - (g_FieldDisplayScaleFactor << 3);
            }
        }
    } else {
        g_FieldBgLayersDirty = 1;
        if (g_FieldActorState.state.movieCameraDisabled == 1) {
            g_FieldPaletteTargetOrgX = (short)(g_FieldBgLayer1ScrollX + g_FieldLayerScrollX) - 0xA0;
            g_FieldPaletteTargetOrgY = (short)(g_FieldBgLayer1ScrollY + g_FieldLayerScrollY) - 0x78;
        } else {
            g_FieldPaletteTargetOrgX = (short)(g_FieldBgLayer1ScrollX - g_pFieldCameraData->scrollOffsetX) - 0xA0;
            g_FieldPaletteTargetOrgY = (short)(g_FieldBgLayer1ScrollY + g_pFieldCameraData->scrollOffsetY) - 0x78;
        }
    }
}
} /* extern "C" */
