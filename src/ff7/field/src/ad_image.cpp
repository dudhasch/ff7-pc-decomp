// src/ff7/field/src/ad_image.cpp
// Original: C:\FF7\field\src\ad_image.cpp
//
// The original CC-delimited interval 0x0062d240..0x006305c0 is one input contribution:
// Field_UpdateAdjacentMapPreload carries the __FILE__ strings and calls Field_ClearAdjacentPreloadReady,
// Field_BltTileSpriteRange calls the last member Field_IsPointClearOfModels, and the .fpl writer's strings follow the
// __FILE__ strings directly. LINK supplies the six CC after Field_IsPointClearOfModels from this one
// 16-byte-aligned section. The file is
// compiled as C++ for the DirectDraw virtual calls; the other bodies keep C linkage.
#include "lib/api/mem.h"
#include "ff7/types/field.h"
#include "lib/pfile.h"
#include "ff7/field.h"
#include <windows.h>
#include <ddraw.h>
#include <stddef.h>
#include "ff7/enums/main.h"
#include "lib/codegen.h"
#include "ff7/window.h"
#include "lib/crt.h"
#include "lib/enums/graphics.h"
#include "ff7/api/field.h"

#pragma code_seg(".text")

typedef char FieldBlitEffectsSize[(sizeof(DDBLTFX) == 100) ? 1 : -1];
typedef char FieldBlitEffectsFillOffset[(offsetof(DDBLTFX, dwFillColor) == 80) ? 1 : -1];
typedef char FieldRectSize[(sizeof(RECT) == 16) ? 1 : -1];

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
extern "C" {

int __cdecl Field_UploadImageToBackSurface(void *arg1, unsigned short arg2);

// The TU's only data definition (retail 0x9066E0 holds 0x009066E4): a pointer to the cursor picture name, whose
// initializer literal is the first of the TU's literals (0x9066E4). No code in the image reads it.
// GLOBAL: FF7 0x009066e0
char *g_FieldHandCursorTextureName = "hand_1.tim";



/* C++ (/Tp) callers pass raw int surface handles where ad_ddraw.cpp defines LPDIRECTDRAWSURFACE. */
extern int __cdecl Field_LockSurfaceBits(int surface, int *bits, int *pitch);
extern int __cdecl Field_UnlockSurface(int surface); /* C++ TU passes int handle for LPDIRECTDRAWSURFACE. */

// clang-format off
// GLOBAL: FF7 0x00cff490
void * g_pFieldHandImageBuffer = 0;
// GLOBAL: FF7 0x00cff494
int g_FieldHandImageType = 0;
// GLOBAL: FF7 0x00cff498
int g_FieldHandImageTimId = 0;
// GLOBAL: FF7 0x00cff49c
int g_FieldHandImageTimFlags = 0;
// GLOBAL: FF7 0x00cff4a0
int g_pFieldPackedColorData1 = 0;
// GLOBAL: FF7 0x00cff4a4
int g_FieldPackedColorSize1 = 0;
// GLOBAL: FF7 0x00cff4a8
short g_FieldPackedColorOrgX1 = 0;
// GLOBAL: FF7 0x00cff4ac
short g_FieldPackedColorCount1 = 0;
unsigned char g_bssPad_00cff4b0[4] = {0};
// GLOBAL: FF7 0x00cff4b4
int g_pFieldPackedColorData2 = 0;
// GLOBAL: FF7 0x00cff4b8
int g_FieldPackedColorSize2 = 0;
// GLOBAL: FF7 0x00cff4bc
short g_FieldPackedColorOrgX2 = 0;
// GLOBAL: FF7 0x00cff4c0
short g_FieldPackedColorPixelWidth2 = 0;
unsigned char g_bssPad_00cff4c4[4] = {0};
unsigned char g_bssPad_00cff4c8[48] = {0};
// GLOBAL: FF7 0x00cff4f8
int g_FieldTimHeader1 = 0;
// GLOBAL: FF7 0x00cff4fc
int g_FieldTimHeader2 = 0;
// GLOBAL: FF7 0x00cff500
unsigned short *g_pFieldTimPaletteData = 0;
// GLOBAL: FF7 0x00cff504
int g_FieldTimPaletteSize = 0;
// GLOBAL: FF7 0x00cff508
short g_FieldTimPaletteOrgX = 0;
// GLOBAL: FF7 0x00cff50c
short g_FieldTimPaletteOrgY = 0;
unsigned char g_bssPad_00cff510[4] = {0};
// GLOBAL: FF7 0x00cff514
int g_pFieldTimPixelData1 = 0;
// GLOBAL: FF7 0x00cff518
int g_FieldTimPixelSize1 = 0;
// GLOBAL: FF7 0x00cff51c
short g_FieldTimPixelWidth1 = 0;
// GLOBAL: FF7 0x00cff520
short g_FieldTimImage1Width = 0;
unsigned char g_bssPad_00cff524[4] = {0};
unsigned char g_bssPad_00cff528[28] = {0};
// GLOBAL: FF7 0x00cff544
int g_pFieldTimPixelData2 = 0;
// GLOBAL: FF7 0x00cff548
int g_FieldTimPixelSize2 = 0;
// GLOBAL: FF7 0x00cff54c
short g_FieldTimPixelWidth2 = 0;
// GLOBAL: FF7 0x00cff550
short g_FieldTimImage2Width = 0;
unsigned char g_bssPad_00cff554[4] = {0};
// GLOBAL: FF7 0x00cff558
int g_FieldImageUploaded = 0;
// GLOBAL: FF7 0x00cff55c
int g_FieldFillSurfaceDirty = 0;
// GLOBAL: FF7 0x00cff560
short g_FieldPaletteCurrentOrgX = 0;
// GLOBAL: FF7 0x00cff564
short g_FieldPaletteCurrentOrgY = 0;
// GLOBAL: FF7 0x00cff568
short g_FieldPaletteTargetOrgX = 0;
// GLOBAL: FF7 0x00cff56c
short g_FieldPaletteTargetOrgY = 0;
// clang-format on
// A TIM block header word holds two 16-bit halves. The globals above keep the low half; each
// short sits in a four-byte slot, and these name the high half stored in the slot's upper two bytes.
// g_FieldTimPaletteOrgY holds the CLUT's width (its colour count) and g_FieldTimPixelWidth1/2 the
// image's VRAM x; ad_pal.cpp and ad_data.cpp spell those names.
#define g_FieldPackedColorOrgY1 (*(short *)((char *)&g_FieldPackedColorOrgX1 + 2))
#define g_FieldPackedColorRows1 (*(short *)((char *)&g_FieldPackedColorCount1 + 2))
#define g_FieldPackedColorOrgY2 (*(short *)((char *)&g_FieldPackedColorOrgX2 + 2))
#define g_FieldPackedColorPixelHeight2 (*(short *)((char *)&g_FieldPackedColorPixelWidth2 + 2))
#define g_FieldTimPaletteVramY (*(short *)((char *)&g_FieldTimPaletteOrgX + 2))
#define g_FieldTimPaletteRows (*(short *)((char *)&g_FieldTimPaletteOrgY + 2))
#define g_FieldTimPixelVramY1 (*(short *)((char *)&g_FieldTimPixelWidth1 + 2))
#define g_FieldTimImage1Height (*(short *)((char *)&g_FieldTimImage1Width + 2))
#define g_FieldTimPixelVramY2 (*(short *)((char *)&g_FieldTimPixelWidth2 + 2))
#define g_FieldTimImage2Height (*(short *)((char *)&g_FieldTimImage2Width + 2))
int __cdecl Field_ClassifyPaletteFormat(unsigned short colorCount, unsigned short sizeHigh);


void __cdecl Field_ClearAdjacentPreloadReady(void);
int __cdecl Field_ClampBackgroundPositionToBounds(short *xPtr, short *yPtr);
int __cdecl Field_ComputeBackgroundViewportClip(short *xPtr, short *yPtr, unsigned short *offsetXOut, unsigned short *offsetYOut, unsigned short *extentXOut, unsigned short *extentYOut);
int __cdecl Field_WriteTileSamplesToSurface(int surface, short centerX, short centerY, int width, int height, int scale);
int __cdecl Field_WriteTileSamplesToBuffer(int buffer, int tileIndex, short originX, short originY, int width, int height, int scale, int pitch);
void __cdecl Field_ClipBackgroundRecordGroup1ToViewport(short centerX, short centerY, short *posX, short *posY, short *srcX, short *srcY, short *width, short *height, int index, int extent);
void __cdecl Field_ClipBackgroundRecordGroup2ToViewport(short centerX, short centerY, short *posX, short *posY, short *srcX, short *srcY, short *width, short *height, int index, int extent);
void __cdecl Field_ClipBackgroundRecordGroup3ToViewport(short centerX, short centerY, short *posX, short *posY, short *srcX, short *srcY, short *width, short *height, int index, int extent);
int __cdecl Field_BltBackgroundLayer3Entries(short centerX, short centerY, int nativeScale);
int __cdecl Field_BltBackgroundLayer4Entries(short, short, int);
int __cdecl Field_BltTileSpriteRange(int startIndex, int endIndex, short centerX, short centerY, int normalScale, int threshold);
int __cdecl Field_CompositeBackgroundFrame(short, short);
void __cdecl Field_DebugWriteFplFile(void);
int __cdecl Field_UploadPalette(int blockPtr);
int __cdecl Field_ParsePaletteImageBlocks(int blockPtr);
int __cdecl Field_ParseBackgroundHeaders(void *blockPtr);
int __cdecl Field_ExtractImageFormatBits(int flags);
int __cdecl Field_SetImageUploadedAndUploadTimHeader1(void);
void __cdecl Field_SortTileSpritesByDepth(void);
int __cdecl Field_CompareTileSpriteDepth(FieldTileSpriteDepthTile *tilePtr, FieldTileSpriteDepthTile *otherTile);
int __cdecl Field_ColorFillSurfaceRect(int left, int top, int width, int height);
void __cdecl Field_FillFirstActiveWindowRect(void);
int __cdecl Field_ParsePaletteBlockPair(void *blockPtr);
void __cdecl Field_LoadHandCursorImage(struct GfxEngineContext *context, PfileLoadOptions *opts);
void __cdecl Field_BlitTile32At(short x, short y);
int __cdecl Field_IsPointClearOfModels(short screenX, short screenY);

extern int __cdecl Field_BltFastAtPoint(int dst, int src, void *srcRect, void *dstRect, int flags); // src/ff7/field/src/ad_ddraw.cpp
typedef RECT FieldRect;

int __cdecl Field_BltBackgroundLayer3Entries(short, short, int);
int __cdecl Field_RetryBltOnSurfaceLost(int, int, void *, void *, int, int);
void __cdecl SetProjectionOffsetAndRebuild(int, int);

// Picks the gateway nearest the player and preloads its map. It allocates a 0x1F40-byte scratch
// block and stores the player's posX, posY and posZ in it, each shifted down 12 bits. It scans the
// twelve 0x18-byte gateway records for the closest one whose targetMap is not 0x7FFF and records
// that map in g_FieldPreloadMapId. g_FieldPreloadState is cleared when the request code is
// FIELD_REQUEST_LOAD_MOVIE or FIELD_REQUEST_BATTLE or g_FieldLogicActive is 1. When the chosen map differs from g_FieldPreloadMapTarget, it
// calls Field_ClearAdjacentPreloadReady and, for ids below 0x100, stores the id in
// g_FieldPreloadMapTarget and sets g_FieldPreloadState to 1. Finally it frees the scratch block.
// FUNCTION: FF7 0x0062d240
void __cdecl Field_UpdateAdjacentMapPreload(FieldPlayerPos *playerPos, FieldMapGateway *gateways)
{
    int distance;
    FieldMapGateway *gatePtr;
    FieldAdjacentMapPreload *pScratch;
    int index;
    int minDist;

    pScratch = 0;
    // STRING: FF7 0x009066f0
    pScratch = (FieldAdjacentMapPreload *)GameHeapAllocRaw(8000, "C:\\FF7\\field\\src\\ad_image.cpp", 88);
    pScratch->centerX = playerPos->posX >> 12;
    pScratch->centerY = playerPos->posY >> 12;
    pScratch->centerZ = playerPos->posZ >> 12;
    minDist = 0x7FFFFFFF;
    for (index = 0; index < 12; index++, gateways++) {
        if (gateways->targetMap != 0x7FFF) {
            distance = (gateways->posX - pScratch->centerX) * (gateways->posX - pScratch->centerX) + (gateways->posY - pScratch->centerY) * (gateways->posY - pScratch->centerY);
            if (distance < minDist) {
                minDist = distance;
                gatePtr = gateways;
                g_FieldPreloadMapId = *(short *)&gatePtr->targetMap;
            }
        }
    }
    if (g_FieldActorState.state.requestCode == FIELD_REQUEST_LOAD_MOVIE || (g_FieldLogicActive == 1 || g_FieldActorState.state.requestCode == FIELD_REQUEST_BATTLE)) {
        g_FieldPreloadState = 0;
    }
    if (g_FieldPreloadMapTarget != g_FieldPreloadMapId) {
        Field_ClearAdjacentPreloadReady();
        if (g_FieldPreloadMapId < 0x100) {
            g_FieldPreloadMapTarget = g_FieldPreloadMapId;
            g_FieldPreloadState = 1;
        }
    }
    // STRING: FF7 0x00906710
    GameHeapFree(pScratch, "C:\\FF7\\field\\src\\ad_image.cpp", 130);
}

/*
 * Clears the global g_FieldPreloadState. That is the whole body.
 */
// FUNCTION: FF7 0x0062d3ae
void __cdecl Field_ClearAdjacentPreloadReady(void)
{
    g_FieldPreloadState = 0;
}

/*
 * Returns zero when the global clamp enable is zero. Otherwise derives unsigned SHORT background
 * dimensions, clamps the supplied SHORT X/Y through sequential lower and upper checks for a
 * 320-by-240 viewport, and copies them to global coordinates. Repeats equivalent clamps for each
 * enabled auxiliary background using its unsigned dimensions and temporary coordinates reduced by
 * 160 and 112, then writes the clamped temporary coordinates directly back. Sequential checks can
 * overwrite the first result for undersized backgrounds. Returns one.
 */
// FUNCTION: FF7 0x0062d3bc
int __cdecl Field_ClampBackgroundPositionToBounds(short *xPtr, short *yPtr)
{
    short tempX;
    short tempY;
    unsigned short bgWidth;
    unsigned short height;

    if (g_FieldCameraScrollE74 == 0) {
        return 0;
    } else {
        bgWidth = g_pFieldScrollBounds->maxX - g_pFieldScrollBounds->minX;
        height = g_pFieldScrollBounds->maxY - g_pFieldScrollBounds->minY;
        if (*xPtr + (bgWidth / 2) + -160 < 0) {
            *xPtr = 0xA0 - (bgWidth / 2);
        }
        if (*yPtr + (height / 2) + -120 < 0) {
            *yPtr = 0x78 - (height / 2);
        }
        if (*xPtr + (bgWidth / 2) + 160 > bgWidth) {
            *xPtr = (bgWidth / 2) - 0xA0;
        }
        if (*yPtr + (height / 2) + 120 > height) {
            *yPtr = (height / 2) - 0x78;
        }
        g_FieldBgLayer2ScrollX = *xPtr;
        g_FieldBgLayer2ScrollY = *yPtr;
        if (g_FieldDoDrawLayer3 != 0) {
            bgWidth = *(short *)&g_FieldLayer3StateE24.width;
            height = *(short *)&g_FieldLayer3StateE24.height;
            tempX = g_FieldBgLayer3ScrollX - 160;
            tempY = g_FieldBgLayer3ScrollY - 112;
            if (tempX + (bgWidth / 2) + -160 < 0) {
                tempX = 160 - (bgWidth / 2);
            }
            if (tempY + (height / 2) + -120 < 0) {
                tempY = 120 - (height / 2);
            }
            if (tempX + (bgWidth / 2) + 160 > bgWidth) {
                tempX = (bgWidth / 2) - 160;
            }
            if (tempY + (height / 2) + 120 > height) {
                tempY = (height / 2) - 120;
            }
            g_FieldBgLayer3ScrollX = tempX;
            g_FieldBgLayer3ScrollY = tempY;
        }
        if (g_FieldDoDrawLayer4 != 0) {
            bgWidth = *(short *)&g_FieldLayer4StateE8c.width;
            height = *(short *)&g_FieldLayer4StateE8c.height;
            tempX = g_FieldBgLayer4ScrollX - 160;
            tempY = g_FieldBgLayer4ScrollY - 112;
            if (tempX + (bgWidth / 2) + -160 < 0) {
                tempX = 160 - (bgWidth / 2);
            }
            if (tempY + (height / 2) + -120 < 0) {
                tempY = 120 - (height / 2);
            }
            if (tempX + (bgWidth / 2) + 160 > bgWidth) {
                tempX = (bgWidth / 2) - 160;
            }
            if (tempY + (height / 2) + 120 > height) {
                tempY = (height / 2) - 120;
            }
            g_FieldBgLayer4ScrollX = tempX;
            g_FieldBgLayer4ScrollY = tempY;
        }
        return 1;
    }
}

/*
 * Disabled returns zero without writes. Enabled derives unsigned-short trigger dimensions and
 * adjusts horizontal/vertical offsets and extents for viewport overflow with half-dimension
 * arithmetic. Sequential opposite-edge tests may overwrite earlier output. Returns one regardless
 * clipping.
 */
// FUNCTION: FF7 0x0062d760
int __cdecl Field_ComputeBackgroundViewportClip(short *xPtr, short *yPtr, unsigned short *offsetXOut, unsigned short *offsetYOut, unsigned short *extentXOut, unsigned short *extentYOut)
{
    unsigned short w;
    unsigned short triggerHeight;

    if (g_FieldCameraScrollE74 == 0) {
        return 0;
    } else {
        w = g_pFieldScrollBounds->maxX - g_pFieldScrollBounds->minX;
        triggerHeight = g_pFieldScrollBounds->maxY - g_pFieldScrollBounds->minY;
        if (*xPtr + (w / 2) + -160 < 0) {
            *offsetXOut = 160 - (w / 2) - *xPtr;
            *extentXOut = 320 - *offsetXOut;
        }
        if (*yPtr + (triggerHeight / 2) + -120 < 0) {
            *offsetYOut = 120 - (triggerHeight / 2) - *yPtr;
            *extentYOut = 240 - *offsetYOut;
        }
        if (*xPtr + (w / 2) + 160 > w) {
            *offsetXOut = 0;
            *extentXOut = -*xPtr - (w / 2) + w + 160;
        }
        if (*yPtr + (triggerHeight / 2) + 120 > triggerHeight) {
            *offsetYOut = 0;
            *extentYOut = -*yPtr - (triggerHeight / 2) + triggerHeight + 120;
        }
        return 1;
    }
}

/*
 * Returns zero for a null surface or failed lock. Otherwise visits marked tiles with nonzero
 * sample count and an unset processing flag, marks each tile processed while iterating its
 * samples, derives sample coordinates relative to the supplied center and dimensions, and for
 * in-range samples converts the selected 16-bit pixel through the color helper. Scale one stores
 * the result in both tile sample and surface; other scales repeat the resulting 16-bit pixel in
 * two dword writes on two computed rows. Unlocks and returns one. Marking occurs before
 * coordinate validity.
 */
// FUNCTION: FF7 0x0062d908
int __cdecl Field_WriteTileSamplesToSurface(int surface, short centerX, short centerY, int width, int height, int scale)
{
    int pairValue;
    int dstFlags;
    int bits;
    short sampleX;
    short sampleY;
    unsigned short tileIdx;
    unsigned short sampleNumber;
    int pitch;
    int dwordStride;
    short pixelIn;
    int lockPitch;
    unsigned short pixelValue;
    int sourceBits;

    bits = 0;
    sourceBits = 0;
    dstFlags = sourceBits;
    if (surface == 0) {
        return 0;
    } else if (Field_LockSurfaceBits(surface, &bits, &lockPitch) == 0) {
        return 0;
    } else {
        sourceBits = bits;
        dstFlags = bits;
        pitch = (unsigned int)(lockPitch * (scale & 0xFFFF)) >> 1;
        dwordStride = (unsigned int)(lockPitch * (scale & 0xFFFF)) >> 2;
        for (tileIdx = 0; tileIdx < g_FieldLayer2TilesNum; tileIdx++) {
            if (g_FieldLayer2Tiles[tileIdx].active != 0 && g_FieldLayer2Tiles[tileIdx].sampleCount > 0 && g_FieldLayer2Tiles[tileIdx].sampled == 0) {
                for (sampleNumber = 0; sampleNumber < g_FieldLayer2Tiles[tileIdx].sampleCount; sampleNumber++) {
                    sampleX = g_FieldLayer2Tiles[tileIdx].tileX + g_FieldLayer2Tiles[tileIdx].samples[sampleNumber].x - centerX + ((unsigned int)width >> 1);
                    sampleY = g_FieldLayer2Tiles[tileIdx].spanY + g_FieldLayer2Tiles[tileIdx].samples[sampleNumber].y - centerY + ((unsigned int)height >> 1) + 8;
                    g_FieldLayer2Tiles[tileIdx].sampled = 1;
                    if ((unsigned int)sampleX < (unsigned int)width && (unsigned int)sampleY < (unsigned int)height && sampleX >= 0 && sampleY >= 0) {
                        pixelIn = ((short *)sourceBits)[sampleY * pitch + sampleX * (scale & 0xFFFF)];
                        pixelValue = Field_LookupBrightnessPaletteEntry((short)pixelIn, g_FieldLayer2Tiles[tileIdx].samples[sampleNumber].brightnessDelta, g_FieldLayer2Tiles[tileIdx].samples[sampleNumber].brightnessMode, 1);
                        if ((scale & 0xFFFF) == 1) {
                            g_FieldLayer2Tiles[tileIdx].samples[sampleNumber].pixel = pixelValue;
                        }
                        if ((scale & 0xFFFF) == 1) {
                            ((short *)sourceBits)[sampleY * pitch + sampleX * (scale & 0xFFFF)] = pixelValue;
                        } else {
                            pairValue = pixelValue << 0x10 | pixelValue;
                            ((int *)dstFlags)[sampleY * dwordStride + (sampleX * (scale & 0xFFFF) / 2)] = pairValue;
                            ((int *)dstFlags)[sampleY * dwordStride + ((unsigned int)lockPitch >> 2) + (sampleX * (scale & 0xFFFF) / 2)] = pairValue;
                        }
                    }
                }
            }
        }
        Field_UnlockSurface(surface);
        return 1;
    }
}

/*
 * Applies the brightness samples of layer-2 tile tileIndex to a 16-bit pixel buffer: marks the tile
 * sampled and recolours each sample's pixel inside the width-by-height area through
 * Field_LookupBrightnessPaletteEntry. Scale one writes the pixel; other scales write it doubled
 * across two rows. Returns one.
 */
// FUNCTION: FF7 0x0062dce2
int __cdecl Field_WriteTileSamplesToBuffer(int buffer, int tileIndex, short originX, short originY, int width, int height, int scale, int pitch)
{
    unsigned short value;
    int bufferBase;
    int pairValue;
    short sampleX;
    short sampleY;
    short srcPixel;
    unsigned short pointIndex;

    bufferBase = buffer;
    for (pointIndex = 0; pointIndex < g_FieldLayer2Tiles[tileIndex & 0xFFFF].sampleCount; pointIndex++) {
        sampleX = g_FieldLayer2Tiles[tileIndex & 0xFFFF].samples[pointIndex].x - originX;
        sampleY = g_FieldLayer2Tiles[tileIndex & 0xFFFF].samples[pointIndex].y - originY + 8;
        g_FieldLayer2Tiles[tileIndex & 0xFFFF].sampled = 1;
        if ((unsigned int)sampleX < (unsigned int)width && (unsigned int)sampleY < (unsigned int)height && sampleX >= 0 && sampleY >= 0) {
            srcPixel = ((short *)buffer)[((unsigned int)(sampleY * pitch) >> 1) * (scale & 0xFFFF) + sampleX * (scale & 0xFFFF)];
            value = Field_LookupBrightnessPaletteEntry((short)srcPixel, g_FieldLayer2Tiles[tileIndex & 0xFFFF].samples[pointIndex].brightnessDelta, g_FieldLayer2Tiles[tileIndex & 0xFFFF].samples[pointIndex].brightnessMode, 1);
            if ((scale & 0xFFFF) == 1) {
                ((short *)buffer)[((unsigned int)(sampleY * pitch) >> 1) * (scale & 0xFFFF) + sampleX * (scale & 0xFFFF)] = value;
            } else {
                pairValue = value << 0x10 | value;
                ((int *)bufferBase)[((unsigned int)(sampleY * pitch * (scale & 0xFFFF)) >> 2) + (sampleX * (scale & 0xFFFF) / 2)] = pairValue;
                ((int *)bufferBase)[((unsigned int)(sampleY * pitch * (scale & 0xFFFF)) >> 2) + ((unsigned int)pitch >> 2) + (sampleX * (scale & 0xFFFF) / 2)] = pairValue;
            }
        }
    }
    return 1;
}

/*
 * Clips indexed table position against camera X plus/minus 160 and Y minus 120/plus 104, writing
 * output position/source coordinates on left/top clipping and extents on opposite edges. All tests
 * reread original table position rather than modified output; no enable guard, negative extent
 * rejection or bounds checks.
 */
// FUNCTION: FF7 0x0062df66
void __cdecl Field_ClipBackgroundRecordGroup1ToViewport(short centerX, short centerY, short *posX, short *posY, short *srcX, short *srcY, short *width, short *height, int index, int extent)
{
    if (g_FieldLayer2Tiles[index & 0xFFFF].spanY < centerY - 0x78) {
        *height = g_FieldLayer2Tiles[index & 0xFFFF].spanY + (extent & 0xFFFF) + 120 - centerY;
        *posY = centerY - 120;
        *srcY = g_FieldLayer2Tiles[index & 0xFFFF].srcY + ((extent & 0xFFFF) - *height);
    }
    if (g_FieldLayer2Tiles[index & 0xFFFF].spanY + (extent & 0xFFFF) > centerY + 0x68) {
        *height = centerY + 120 - g_FieldLayer2Tiles[index & 0xFFFF].spanY - 16;
    }
    if (g_FieldLayer2Tiles[index & 0xFFFF].tileX < centerX - 0xA0) {
        *width = g_FieldLayer2Tiles[index & 0xFFFF].tileX + (extent & 0xFFFF) + 160 - centerX;
        *posX = centerX - 160;
        *srcX = g_FieldLayer2Tiles[index & 0xFFFF].srcX + ((extent & 0xFFFF) - *width);
    }
    if (g_FieldLayer2Tiles[index & 0xFFFF].tileX + (extent & 0xFFFF) > centerX + 0xA0) {
        *width = centerX + 160 - g_FieldLayer2Tiles[index & 0xFFFF].tileX;
    }
}

/*
 * When enabled clips position shorts and output extents against center X plus/minus 160 and
 * asymmetric Y bounds minus 120/plus 112. Left/top clipping adjusts source shorts using indexed
 * table and low-sixteen extent; right/bottom clipping only changes extents. Does not reject
 * negative extents or clamp to zero.
 */
// FUNCTION: FF7 0x0062e156
void __cdecl Field_ClipBackgroundRecordGroup2ToViewport(short centerX, short centerY, short *posX, short *posY, short *srcX, short *srcY, short *width, short *height, int index, int extent)
{
    if (g_FieldDoDrawLayer3 == 0) {
        return;
    }
    if (*posY < centerY - 0x78) {
        *height = *posY + (extent & 0xFFFF) + 120 - centerY;
        *posY = centerY - 120;
        *srcY = g_FieldLayer3Tiles[index & 0xFFFF].srcY + ((extent & 0xFFFF) - *height);
    }
    if (*posY + (extent & 0xFFFF) > centerY + 0x70) {
        *height = centerY + 120 - *posY - 16;
    }
    if (*posX < centerX - 0xA0) {
        *width = *posX + (extent & 0xFFFF) + 160 - centerX;
        *posX = centerX - 160;
        *srcX = g_FieldLayer3Tiles[index & 0xFFFF].srcX + ((extent & 0xFFFF) - *width);
    }
    if (*posX + (extent & 0xFFFF) > centerX + 0xA0) {
        *width = centerX + 160 - *posX;
    }
}

/*
 * When second group enabled clips positions/extents against X center plus/minus 160 and Y minus
 * 120/plus 112, adjusting source shorts from second indexed table for left/top clipping. Uses same
 * low-sixteen extent for both spans; right/bottom clipping only adjusts extents and may produce
 * negative values.
 */
// FUNCTION: FF7 0x0062e2bd
void __cdecl Field_ClipBackgroundRecordGroup3ToViewport(short centerX, short centerY, short *posX, short *posY, short *srcX, short *srcY, short *width, short *height, int index, int extent)
{
    if (g_FieldDoDrawLayer4 == 0) {
        return;
    }
    if (*posY < centerY - 0x78) {
        *height = *posY + (extent & 0xFFFF) + 120 - centerY;
        *posY = centerY - 120;
        *srcY = g_FieldLayer4Tiles[index & 0xFFFF].srcY + ((extent & 0xFFFF) - *height);
    }
    if (*posY + (extent & 0xFFFF) > centerY + 0x70) {
        *height = centerY + 120 - *posY - 16;
    }
    if (*posX < centerX - 0xA0) {
        *width = *posX + (extent & 0xFFFF) + 160 - centerX;
        *posX = centerX - 160;
        *srcX = g_FieldLayer4Tiles[index & 0xFFFF].srcX + ((extent & 0xFFFF) - *width);
    }
    if (*posX + (extent & 0xFFFF) > centerX + 0xA0) {
        *width = centerX + 160 - *posX;
    }
}

/*
 * Returns zero if the background collection is disabled and one without drawing if it was already
 * processed. Otherwise scans 0x105C-byte records, wraps each position once by the configured field
 * dimensions when outside the view interval, and draws entries strictly inside the interval whose
 * optional byte selector mask passes. Sets inUse on qualifying records, passes a 32-by-32
 * tile through the clipping helper, and submits source/destination rectangles at normal scale when
 * nativeScale is nonzero or doubled scale with flag 0x8000 otherwise. Sets the global processed flag and
 * returns one after the scan. The two rectangles live in int locals followed by unreferenced
 * stack-padding slots preserved for VC5 /Od frame layout.
 */
// FUNCTION: FF7 0x0062e424
int __cdecl Field_BltBackgroundLayer3Entries(short centerX, short centerY, int nativeScale)
{
    short xPos;
    RECT rectSrc;
    short sourceRow;
    short tileY;
    int unused; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    short srcHeight;
    unsigned short recordIndex;
    short textureX;
    short width;
    unsigned short ent;
    int reservedSlots[2]; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    RECT destinationRect;

    if (g_FieldDoDrawLayer3 == 0) {
        return 0;
    } else if (g_FieldLayer3Flag != 0) {
        return 1;
    } else {
        for (recordIndex = 0; recordIndex < g_FieldLayer3TilesNum; recordIndex++) {
            ent = recordIndex;
            xPos = g_FieldLayer3Tiles[ent].xPos;
            tileY = g_FieldLayer3Tiles[ent].tileY;
            if (tileY <= centerY - 0x98 || tileY >= centerY + 0x68) {
                if (tileY < centerY) {
                    tileY += g_pFieldScrollBounds->tileHeight;
                } else {
                    tileY -= g_pFieldScrollBounds->tileHeight;
                }
            }
            if (xPos <= centerX - 0xC0 || xPos >= centerX + 0xA0) {
                if (xPos < centerX) {
                    xPos += g_pFieldScrollBounds->tileWidth;
                } else {
                    xPos -= g_pFieldScrollBounds->tileWidth;
                }
            }
            if (tileY > centerY - 0x98 && (tileY < centerY + 0x68 && (xPos > centerX - 0xC0 && (xPos < centerX + 0xA0 && (g_FieldLayer3Tiles[ent].zDepth == 0 || (g_FieldLayer3Tiles[ent].zFlags & g_FieldActorState.state.backgroundLayerVisibility[g_FieldLayer3Tiles[ent].zDepth]) != 0))))) {
                g_FieldLayer3Tiles[ent].inUse = 1;
                textureX = *(short *)&g_FieldLayer3Tiles[ent].srcX;
                sourceRow = *(short *)&g_FieldLayer3Tiles[ent].srcY;
                width = 32;
                srcHeight = 32;
                /* Retail caller prototype (short, short, void *, void *, void *, void *, void *, void *, short, int) -> void preserved for /Od argument promotion on `Field_ClipBackgroundRecordGroup2ToViewport`. */
                ((void(__cdecl *)(short, short, void *, void *, void *, void *, void *, void *, short, int))Field_ClipBackgroundRecordGroup2ToViewport)((short)centerX, (short)centerY, &xPos, &tileY, &textureX, &sourceRow, &width, &srcHeight, (short)ent, 0x20);
                SetRect(&rectSrc, textureX, sourceRow, textureX + width, sourceRow + srcHeight);
                if (nativeScale != 0) {
                    SetRect(&destinationRect, xPos + 0xA0 - centerX, tileY + 0x78 - centerY + 8, xPos + 0xA0 - centerX + width, tileY + 0x78 - centerY + srcHeight + 8);
                    Field_BltFastAtPoint(g_FieldDDrawSurfaceDd4, g_FieldLayer2ParamE18, &rectSrc, &destinationRect, 1);
                } else {
                    SetRect(&destinationRect, xPos + 0xA0 - centerX << 1, (tileY + 0x78 - centerY) * 2 + 0x10, xPos + 0xA0 - centerX + width << 1, (tileY + 0x78 - centerY + srcHeight) * 2 + 0x10);
                    Field_RetryBltOnSurfaceLost(g_pFieldBackSurface, g_FieldLayer2ParamE18, &rectSrc, &destinationRect, 0x8000, 0);
                }
            }
        }
        g_FieldLayer3Flag = 1;
        return 1;
    }
}

/*
 * Returns zero when layer four is disabled and one immediately when already processed. Otherwise
 * computes depth, scans all tiles, wraps coordinates once by background dimensions, and applies
 * strict XY culling plus parameter masks. Marks accepted tiles, then reloads their original
 * coordinates before clipping a 32-by-32 source rectangle through the helper. Copies each clipped
 * region by one of two paths: native-sized destination with the eight-pixel vertical offset when
 * nativeScale is nonzero, or doubled destination with flag 32768 otherwise. Sets processed flag and
 * returns one. Computed depth is unused.
 */
// FUNCTION: FF7 0x0062e7ff
int __cdecl Field_BltBackgroundLayer4Entries(short centerX, short centerY, int nativeScale)
{
    short tileHeightX;
    int passDepthFloatBits;
    FieldRect srcRect;
    unsigned short rawIndex;
    short posY;
    short sourceHeight;
    unsigned short tileNo;
    short texU;
    short width;
    short sourceY;
    int depthBits;
    float depthZ;
    FieldRect destinationRectangle;
    if (g_FieldDoDrawLayer4 == 0) {
        return 0;
    } else if (g_FieldLayer4Flag != 0) {
        return 1;
    } else {
        passDepthFloatBits = FLOAT_0_125_BITS;
        depthBits = FLOAT_0_125_BITS;
        depthZ = Field_ComputeTileDepthZ(g_FieldCameraTransform, g_FieldActorState.state.layer3Depth, 0, 0);
        for (tileNo = 0; tileNo < g_FieldLayer4TilesNum; tileNo++) {
            rawIndex = tileNo;
            tileHeightX = g_FieldLayer4Tiles[rawIndex].posX;
            posY = g_FieldLayer4Tiles[rawIndex].posY;
            if (posY <= centerY - 0x98 || posY >= centerY + 0x68) {
                if (posY < centerY) {
                    posY += g_pFieldScrollBounds->tileStrideY;
                } else {
                    posY -= g_pFieldScrollBounds->tileStrideY;
                }
            }
            if (tileHeightX <= centerX - 0xC0 || tileHeightX >= centerX + 0xA0) {
                if (tileHeightX < centerX) {
                    tileHeightX += g_pFieldScrollBounds->tileStrideX;
                } else {
                    tileHeightX -= g_pFieldScrollBounds->tileStrideX;
                }
            }
            if (posY > centerY - 0x98 && (posY < centerY + 0x68 && (tileHeightX > centerX - 0xC0 && (tileHeightX < centerX + 0xA0 && (g_FieldLayer4Tiles[rawIndex].zDepth == 0 || (g_FieldLayer4Tiles[rawIndex].zFlags & g_FieldActorState.state.backgroundLayerVisibility[g_FieldLayer4Tiles[rawIndex].zDepth]) != 0))))) {
                g_FieldLayer4Tiles[rawIndex].inUse = 1;
                tileHeightX = g_FieldLayer4Tiles[rawIndex].posX;
                posY = g_FieldLayer4Tiles[rawIndex].posY;
                texU = *(short *)&g_FieldLayer4Tiles[rawIndex].srcX;
                sourceY = *(short *)&g_FieldLayer4Tiles[rawIndex].srcY;
                width = 32;
                sourceHeight = 32;
                /* Retail caller prototype (short, short, void *, void *, void *, void *, void *, void *, short, short) -> void preserved for /Od argument promotion on `Field_ClipBackgroundRecordGroup3ToViewport`. */
                ((void(__cdecl *)(short, short, void *, void *, void *, void *, void *, void *, short, short))Field_ClipBackgroundRecordGroup3ToViewport)((short)centerX, (short)centerY, &tileHeightX, &posY, &texU, &sourceY, &width, &sourceHeight, (short)rawIndex, 0x20);
                SetRect((RECT *)&srcRect, texU, sourceY, texU + width, sourceY + sourceHeight);
                if (nativeScale != 0) {
                    SetRect((RECT *)&destinationRectangle, tileHeightX + 0xA0 - centerX, posY + 0x78 - centerY + 8, tileHeightX + 0xA0 - centerX + width, posY + 0x78 - centerY + sourceHeight + 8);
                    Field_BltFastAtPoint(g_FieldDDrawSurfaceDd4, g_FieldCameraScrollE80, &srcRect, &destinationRectangle, 1);
                } else {
                    SetRect((RECT *)&destinationRectangle, tileHeightX + 0xA0 - centerX << 1, (posY + 0x78 - centerY) * 2 + 0x10, tileHeightX + 0xA0 - centerX + width << 1, (posY + 0x78 - centerY + sourceHeight) * 2 + 0x10);
                    Field_RetryBltOnSurfaceLost(g_pFieldBackSurface, g_FieldCameraScrollE80, &srcRect, &destinationRectangle, 0x8000, 0);
                }
            }
        }
        g_FieldLayer4Flag = 1;
        return 1;
    }
}

/*
 * Processes an index range of field tiles, marking eligible tiles visited before viewport and
 * visibility checks. Visible unsuppressed tiles are clipped and blitted to normal or doubled
 * destination rectangles. The normal path also admits tiles passing an auxiliary test and
 * refreshes the 320 by 240 surface afterward. Returns zero only for the effective upper-bound
 * rejection; the masked lower-bound comparison cannot be negative.
 */
// FUNCTION: FF7 0x0062ec38
int __cdecl Field_BltTileSpriteRange(int startIndex, int endIndex, short centerX, short centerY, int normalScale, int threshold)
{
    short tileX;
    RECT srcRect;
    short sourceY;
    RECT dstRect;
    short spanY;
    short srcX;
    short height;
    unsigned short drawSlot;
    short rangeW;
    unsigned short entry;

    if ((startIndex & 0xFFFF) < 0 || (endIndex & 0xFFFF) > g_FieldLayer2TilesNum) {
        return 0;
    } else {
        if (normalScale != 0) {
            for (drawSlot = startIndex; drawSlot < (endIndex & 0xFFFF); drawSlot++) {
                entry = g_pFieldTileDrawOrder[drawSlot].tileIndex;
                if (g_FieldLayer2Tiles[entry].drawn == 0 && (g_FieldLayer2Tiles[entry].sortKey >= (threshold & 0xFFFF) || Field_IsPointClearOfModels((short)g_FieldLayer2Tiles[entry].tileX, g_FieldLayer2Tiles[entry].spanY + 8) != 0)) {
                    g_FieldLayer2Tiles[entry].drawn = 1;
                    if (g_FieldLayer2Tiles[entry].spanY > centerY - 0x88 && (g_FieldLayer2Tiles[entry].spanY < centerY + 0x68 && (g_FieldLayer2Tiles[entry].tileX > centerX - 0xB0 && (g_FieldLayer2Tiles[entry].tileX < centerX + 0xA0 && (g_FieldLayer2Tiles[entry].zBufferValue == 0 || (g_FieldLayer2Tiles[entry].visibilityMask & g_FieldActorState.state.backgroundLayerVisibility[g_FieldLayer2Tiles[entry].zBufferValue]) != 0))))) {
                        g_FieldLayer2Tiles[entry].active = 1;
                        if (g_FieldLayer2Tiles[entry].inUse == 0) {
                            tileX = g_FieldLayer2Tiles[entry].tileX;
                            spanY = g_FieldLayer2Tiles[entry].spanY;
                            srcX = *(short *)&g_FieldLayer2Tiles[entry].srcX;
                            sourceY = *(short *)&g_FieldLayer2Tiles[entry].srcY;
                            rangeW = 16;
                            height = 16;
                            /* Retail caller prototype (short, short, void *, void *, void *, void *, void *, void *, short, int) -> void preserved for /Od argument promotion on `Field_ClipBackgroundRecordGroup1ToViewport`. */
                            ((void(__cdecl *)(short, short, void *, void *, void *, void *, void *, void *, short, int))Field_ClipBackgroundRecordGroup1ToViewport)((short)centerX, (short)centerY, &tileX, &spanY, &srcX, &sourceY, &rangeW, &height, (short)entry, 0x10);
                            SetRect(&dstRect, tileX + 0xA0 - centerX, spanY + 0x78 - centerY + 8, tileX + 0xA0 - centerX + rangeW, spanY + 0x78 - centerY + height + 8);
                            SetRect(&srcRect, srcX, sourceY, srcX + rangeW, sourceY + height);
                            Field_BltFastAtPoint(g_FieldDDrawSurfaceDd4, g_pFieldBlitSourceSurface, &srcRect, &dstRect, 1);
                        }
                    }
                }
            }
            Field_WriteTileSamplesToSurface(g_FieldDDrawSurfaceDd4, (short)centerX, (short)centerY, 320, 240, 1);
        } else {
            for (drawSlot = startIndex; drawSlot < (endIndex & 0xFFFF); drawSlot++) {
                entry = g_pFieldTileDrawOrder[drawSlot].tileIndex;
                if (g_FieldLayer2Tiles[entry].drawn == 0 && g_FieldLayer2Tiles[entry].sortKey >= (threshold & 0xFFFF)) {
                    g_FieldLayer2Tiles[entry].drawn = 1;
                    if (g_FieldLayer2Tiles[entry].spanY > centerY - 0x88 && (g_FieldLayer2Tiles[entry].spanY < centerY + 0x68 && (g_FieldLayer2Tiles[entry].tileX > centerX - 0xB0 && (g_FieldLayer2Tiles[entry].tileX < centerX + 0xA0 && (g_FieldLayer2Tiles[entry].zBufferValue == 0 || (g_FieldLayer2Tiles[entry].visibilityMask & g_FieldActorState.state.backgroundLayerVisibility[g_FieldLayer2Tiles[entry].zBufferValue]) != 0))))) {
                        g_FieldLayer2Tiles[entry].active = 1;
                        if (g_FieldLayer2Tiles[entry].inUse == 0) {
                            tileX = g_FieldLayer2Tiles[entry].tileX;
                            spanY = g_FieldLayer2Tiles[entry].spanY;
                            srcX = *(short *)&g_FieldLayer2Tiles[entry].srcX;
                            sourceY = *(short *)&g_FieldLayer2Tiles[entry].srcY;
                            rangeW = 16;
                            height = 16;
                            /* Retail caller prototype (short, short, void *, void *, void *, void *, void *, void *, short, int) -> void preserved for /Od argument promotion on `Field_ClipBackgroundRecordGroup1ToViewport`. */
                            ((void(__cdecl *)(short, short, void *, void *, void *, void *, void *, void *, short, int))Field_ClipBackgroundRecordGroup1ToViewport)((short)centerX, (short)centerY, &tileX, &spanY, &srcX, &sourceY, &rangeW, &height, (short)entry, 0x10);
                            SetRect(&dstRect, tileX + 0xA0 - centerX << 1, (spanY + 0x78 - centerY) * 2 + 0x10, tileX + 0xA0 - centerX + rangeW << 1, (spanY + 0x78 - centerY + height) * 2 + 0x10);
                            SetRect(&srcRect, srcX, sourceY, srcX + rangeW, sourceY + height);
                            Field_RetryBltOnSurfaceLost(g_pFieldBackSurface, g_pFieldBlitSourceSurface, &srcRect, &dstRect, 0x8000, 0);
                        }
                    }
                }
            }
        }
        return 1;
    }
}

/*
 * Returns zero for a missing intermediate surface. Otherwise clips a 320-by-240 region, publishes
 * camera coordinates and subtracts fixed offsets from two auxiliary pairs, optionally black-fills
 * uncovered regions, and updates projection when its requested offsets change. Composites enabled
 * background layers in several depth-ordered helper calls, relying on their processed flags to
 * suppress repeats. Finally scales the central 320-by-224 region to a 640-by-448 destination
 * rectangle through the unconditional copy path and returns one. DirectDraw fill status is
 * ignored; surface pointers and source extents are delegated.
 */
// FUNCTION: FF7 0x0062f3a5
int __cdecl Field_CompositeBackgroundFrame(short scrollX, short scrollY)
{
    unsigned short endX;
    DDBLTFX effects;
    unsigned short endY;
    RECT source;
    unsigned short clipLeft;
    int hr;
    RECT dst;
    unsigned short top;

    clipLeft = 0;
    top = 0;
    endX = 320;
    endY = 240;
    if (g_FieldDDrawSurfaceDd4 == 0) {
        return 0;
    } else {
        Field_ComputeBackgroundViewportClip(&scrollX, &scrollY, &clipLeft, &top, &endX, &endY);
        endX += clipLeft;
        endY += top;
        g_FieldBgLayer2ScrollX = scrollX;
        g_FieldBgLayer2ScrollY = scrollY;
        g_FieldBgLayer3ScrollX = (int)g_FieldBgLayer3ScrollX - 160;
        g_FieldBgLayer3ScrollY = (int)g_FieldBgLayer3ScrollY - 112;
        g_FieldBgLayer4ScrollX = (int)g_FieldBgLayer4ScrollX - 160;
        g_FieldBgLayer4ScrollY = (int)g_FieldBgLayer4ScrollY - 112;
        if (clipLeft != 0 || (top != 0 || (endX != 320 || endY != 240))) {
            effects.dwSize = 100;
            effects.dwFillColor = 0;
            hr = ((LPDIRECTDRAWSURFACE)g_FieldDDrawSurfaceDd4)->Blt(0, 0, 0, 0x400, &effects);
        }
        if (g_FieldPaletteCurrentOrgX != g_FieldPaletteTargetOrgX || g_FieldPaletteCurrentOrgY != g_FieldPaletteTargetOrgY) {
            g_FieldPaletteCurrentOrgX = g_FieldPaletteTargetOrgX;
            g_FieldPaletteCurrentOrgY = g_FieldPaletteTargetOrgY;
            SetProjectionOffsetAndRebuild(g_FieldPaletteTargetOrgX << 1, g_FieldPaletteTargetOrgY << 1);
        }
        if (g_FieldActorState.state.layer2Depth >= 4095) {
            Field_BltBackgroundLayer3Entries((short)g_FieldBgLayer3ScrollX, (short)g_FieldBgLayer3ScrollY, 1);
        }
        if (g_FieldCameraScrollE74 != 0) {
            SetRect(&dst, clipLeft, top + 8, endX, endY - 8);
            SetRect(&source, scrollX - g_FieldLayer1StateE50[0] + clipLeft + -0xA0, top + (scrollY - g_FieldLayer1StateE50[1]) + -0x78, scrollX - g_FieldLayer1StateE50[0] + endX + -0xA0, scrollY - g_FieldLayer1StateE50[1] + endY + -0x88);
            if (g_FieldDoDrawLayer3 == 0 || g_FieldLayer3Flag == 0) {
                Field_BltFastAtPoint(g_FieldDDrawSurfaceDd4, g_FieldLayer1StateE48, &source, &dst, 0);
            } else {
                Field_BltFastAtPoint(g_FieldDDrawSurfaceDd4, g_FieldLayer1StateE48, &source, &dst, 1);
            }
        }
        if (g_FieldActorState.state.layer2Depth > *(unsigned short *)&g_FieldModelSortRecords[0].depth && g_FieldLayer3Flag == 0) {
            Field_BltBackgroundLayer3Entries((short)g_FieldBgLayer3ScrollX, (short)g_FieldBgLayer3ScrollY, 1);
        }
        if (g_FieldActorState.state.layer3Depth > *(unsigned short *)&g_FieldModelSortRecords[0].depth && g_FieldLayer4Flag == 0) {
            Field_BltBackgroundLayer4Entries((short)g_FieldBgLayer4ScrollX, (short)g_FieldBgLayer4ScrollY, 1);
        }
        if (g_FieldLayer2ParamE04 != 0) {
            /* Retail call-site prototype (int, short, short, short, int, short) -> int preserved for /Od caller codegen. */
            ((int(__cdecl *)(int, short, short, short, int, short))Field_BltTileSpriteRange)(0, *(short *)&g_FieldLayer2TilesNum, (short)scrollX, (short)scrollY, 1, g_FieldModelSortRecords[0].depth);
        }
        if (g_FieldModelSortCount > 0) {
            if (g_FieldActorState.state.layer2Depth > *(unsigned short *)&g_FieldModelSortRecords[1].depth && g_FieldLayer3Flag == 0) {
                Field_BltBackgroundLayer3Entries((short)g_FieldBgLayer3ScrollX, (short)g_FieldBgLayer3ScrollY, 1);
            }
            if (g_FieldActorState.state.layer3Depth > *(unsigned short *)&g_FieldModelSortRecords[1].depth && g_FieldLayer4Flag == 0) {
                Field_BltBackgroundLayer4Entries((short)g_FieldBgLayer4ScrollX, (short)g_FieldBgLayer4ScrollY, 1);
            }
        }
        if (1 != 0) {
            SetRect(&dst, 0, 16, 640, 464);
            SetRect(&source, 0, 8, 320, 232);
            Field_RetryBltOnSurfaceLost(g_pFieldBackSurface, g_FieldDDrawSurfaceDd4, &source, &dst, 0, 0);
        }
        return 1;
    }
}

#pragma intrinsic(strcpy, strcat)
/*
 * Debug dump: writes the current field polygon payload to <data dir><map>\<map>.fpl as its size, two
 * packed 16-bit pairs and the payload after its 12-byte header.
 * The original 684-byte frame holds the path at -684, the stream at -552 and the map name at -548.
 * It also reserves 4 unreferenced padding bytes at -556 and 515 unreferenced bytes after the
 * 33-byte name to preserve VC5 /Od stack layout.
 */
// FUNCTION: FF7 0x0062f822
void __cdecl Field_DebugWriteFplFile(void)
{

    struct {
        char path[128];
        int unrecovered0;
        FILE *stream;
        char name[33];
        char unrecovered1[515];
    } frame;
    if (Field_GetMapNameById((short)g_FieldCurrentMapId, frame.name) == 0) {
        return;
    }
    strcpy(frame.path, g_FieldDataDir);
    strcat(frame.path, frame.name);
    // STRING: FF7 0x00906730
    strcat(frame.path, "\\");
    strcat(frame.path, frame.name);
    // STRING: FF7 0x00906734
    strcat(frame.path, ".fpl");
    // STRING: FF7 0x0090673c
    frame.stream = Field_FileOpen(frame.path, "wb");
    if (frame.stream == 0) {
        return;
    }
    Field_DebugWriteU32(frame.stream, g_FieldTimPaletteSize);
    Field_DebugWriteU32(frame.stream, (g_FieldTimPaletteVramY << 16) | g_FieldTimPaletteOrgX);
    Field_DebugWriteU32(frame.stream, ((unsigned short)g_FieldTimPaletteRows << 16) | (unsigned short)g_FieldTimPaletteOrgY);
    Field_FileWrite(frame.stream, g_pFieldTimPaletteData, g_FieldTimPaletteSize - 12);
    Field_FileClose(frame.stream);
}
#pragma function(strcpy, strcat)

/*
 * Reads a TIM CLUT block (byte size, VRAM x and y, width and height) into the g_FieldTimPalette*
 * globals, points g_pFieldTimPaletteData past its 12-byte header and builds the palette records
 * with the pixel mode Field_ClassifyPaletteFormat derives from the CLUT size. Returns one on success.
 */
// FUNCTION: FF7 0x0062f9d2
int __cdecl Field_UploadPalette(int blockPtr)
{
    int *cursor;

    cursor = (int *)blockPtr;
    g_FieldTimPaletteSize = *cursor;
    cursor++;
    g_FieldTimPaletteOrgX = *cursor & 0xFFFF;
    g_FieldTimPaletteVramY = (*cursor & 0xFFFF0000) >> 16;
    cursor++;
    g_FieldTimPaletteOrgY = *cursor & 0xFFFF;
    g_FieldTimPaletteRows = (*cursor & 0xFFFF0000) >> 16;
    cursor++;
    g_pFieldTimPaletteData = (unsigned short *)cursor;
    if (Field_BuildPaletteRecords((short)g_FieldTimPaletteOrgY, (short)g_FieldTimPaletteRows, g_pFieldTimPaletteData, Field_ClassifyPaletteFormat((short)g_FieldTimPaletteOrgY, (short)g_FieldTimPaletteRows)) == 0) {
        return 0;
    } else {
        return 1;
    }
}

/*
 * Classifies a palette by its two size words as a TIM pixel mode: 16BPP when sizeHigh is zero, 4BPP
 * when colorCount is at most 0x10, 8BPP otherwise. Both parameters are unsigned shorts, which is what makes the callers push them
 * narrow.
 */
// FUNCTION: FF7 0x0062fa9e
int __cdecl Field_ClassifyPaletteFormat(unsigned short colorCount, unsigned short sizeHigh)
{
    int format;

    format = 0;
    if (sizeHigh == 0) {
        format = TIM_PIXEL_MODE_16BPP;
    } else if (colorCount <= 0x10) {
        format = TIM_PIXEL_MODE_4BPP;
    } else {
        format = TIM_PIXEL_MODE_8BPP;
    }
    return format;
}

/*
 * Reads a TIM CLUT block as Field_UploadPalette does and builds its palette records; on success
 * reads the image block after it into the g_FieldTimPixel*1 globals and, when the next block's size
 * is nonzero, that block into the *2 globals. Image widths are stored doubled. Returns zero, with
 * the CLUT globals already written, when the palette build fails.
 */
// FUNCTION: FF7 0x0062fae3
int __cdecl Field_ParsePaletteImageBlocks(int blockPtr)
{
    int *cursor;

    cursor = (int *)blockPtr;
    g_FieldTimPaletteSize = *cursor;
    cursor++;
    g_FieldTimPaletteOrgX = *cursor & 0xFFFF;
    g_FieldTimPaletteVramY = (*cursor & 0xFFFF0000) >> 16;
    cursor++;
    g_FieldTimPaletteOrgY = *cursor & 0xFFFF;
    g_FieldTimPaletteRows = (*cursor & 0xFFFF0000) >> 16;
    cursor++;
    g_pFieldTimPaletteData = (unsigned short *)cursor;
    cursor = (int *)((int)cursor + ((unsigned int)g_FieldTimPaletteSize >> 2) * 4 + -0xC);
    if (Field_BuildPaletteRecords((short)g_FieldTimPaletteOrgY, (short)g_FieldTimPaletteRows, g_pFieldTimPaletteData, Field_ClassifyPaletteFormat((short)g_FieldTimPaletteOrgY, (short)g_FieldTimPaletteRows)) == 0) {
        return 0;
    } else {
        g_FieldTimPixelSize1 = *cursor;
        cursor++;
        g_FieldTimPixelWidth1 = *cursor & 0xFFFF;
        g_FieldTimPixelVramY1 = (*cursor & 0xFFFF0000) >> 16;
        cursor++;
        g_FieldTimImage1Width = (*cursor & 0xFFFF & 0xFFFF) << 1;
        g_FieldTimImage1Height = (*cursor & 0xFFFF0000) >> 16;
        cursor++;
        g_pFieldTimPixelData1 = (int)cursor;
        cursor = (int *)((int)cursor + ((unsigned int)g_FieldTimPixelSize1 >> 2) * 4 + -0xC);
        g_FieldTimPixelSize2 = *cursor;
        cursor++;
        if ((unsigned int)g_FieldTimPixelSize2 > (unsigned int)0) {
            g_FieldTimPixelWidth2 = *cursor & 0xFFFF;
            g_FieldTimPixelVramY2 = (*cursor & 0xFFFF0000) >> 16;
            cursor++;
            g_FieldTimImage2Width = (*cursor & 0xFFFF & 0xFFFF) << 1;
            g_FieldTimImage2Height = (*cursor & 0xFFFF0000) >> 16;
            cursor++;
            g_pFieldTimPixelData2 = (int)cursor;
        }
        return 1;
    }
}

/*
 * Reads a TIM file: its id and flags into g_FieldTimHeader1/2, the CLUT block and the first image
 * block. Builds the palette records with the pixel mode from the flags; on success reads the second
 * image block and returns one, otherwise returns zero.
 */
// FUNCTION: FF7 0x0062fce4
int __cdecl Field_ParseBackgroundHeaders(void *blockPtr)
{
    int *cursor;

    cursor = (int *)blockPtr;
    g_FieldTimHeader1 = *cursor;
    cursor++;
    g_FieldTimHeader2 = *cursor;
    cursor++;
    g_FieldTimPaletteSize = *cursor;
    cursor++;
    g_FieldTimPaletteOrgX = *cursor & 0xFFFF;
    g_FieldTimPaletteVramY = (*cursor & 0xFFFF0000) >> 16;
    cursor++;
    g_FieldTimPaletteOrgY = *cursor & 0xFFFF;
    g_FieldTimPaletteRows = (*cursor & 0xFFFF0000) >> 16;
    cursor++;
    g_pFieldTimPaletteData = (unsigned short *)cursor;
    cursor += (unsigned int)(g_FieldTimPaletteSize - 0xC) >> 2;
    g_FieldTimPixelSize1 = *cursor;
    cursor++;
    g_FieldTimPixelWidth1 = *cursor & 0xFFFF;
    g_FieldTimPixelVramY1 = (*cursor & 0xFFFF0000) >> 16;
    cursor++;
    g_FieldTimImage1Width = (*cursor & 0xFFFF & 0xFFFF) << 1;
    g_FieldTimImage1Height = (*cursor & 0xFFFF0000) >> 16;
    cursor++;
    g_pFieldTimPixelData1 = (int)cursor;
    if (Field_BuildPaletteRecords((short)g_FieldTimPaletteOrgY, (short)g_FieldTimPaletteRows, g_pFieldTimPaletteData, Field_ExtractImageFormatBits(g_FieldTimHeader2)) == 0) {
        return 0;
    } else {
        cursor = (int *)((int)cursor + ((unsigned int)g_FieldTimPixelSize1 >> 2) * 4 + -0xC);
        g_FieldTimPixelSize2 = *cursor;
        cursor++;
        g_FieldTimPixelWidth2 = *cursor & 0xFFFF;
        g_FieldTimPixelVramY2 = (*cursor & 0xFFFF0000) >> 16;
        cursor++;
        g_FieldTimImage2Width = (*cursor & 0xFFFF & 0xFFFF) << 1;
        g_FieldTimImage2Height = (*cursor & 0xFFFF0000) >> 16;
        cursor++;
        g_pFieldTimPixelData2 = (int)cursor;
        return 1;
    }
}

/*
 * Returns the TIM pixel mode in the low two bits of a TIM header's flags, through an exhaustive
 * four-case switch.
 */
// FUNCTION: FF7 0x0062fef9
int __cdecl Field_ExtractImageFormatBits(int flags)
{
    int format;

    format = 0;
    switch (flags & 3) {
    case TIM_PIXEL_MODE_4BPP:
        format = TIM_PIXEL_MODE_4BPP;
        break;
    case TIM_PIXEL_MODE_8BPP:
        format = TIM_PIXEL_MODE_8BPP;
        break;
    case TIM_PIXEL_MODE_16BPP:
        format = TIM_PIXEL_MODE_16BPP;
        break;
    case TIM_PIXEL_MODE_24BPP:
        format = TIM_PIXEL_MODE_24BPP;
    }
    return format;
}

/*
 * Sets g_FieldImageUploaded to 1, calls Field_UploadImageToBackSurface(&g_FieldTimHeader1, 0) and returns 1 unconditionally.
 */
// FUNCTION: FF7 0x0062ff58
int __cdecl Field_SetImageUploadedAndUploadTimHeader1(void)
{
    g_FieldImageUploaded = 1;
    Field_UploadImageToBackSurface(&g_FieldTimHeader1, 0);
    return 1;
}

/*
 * Sorts the active field tile sprites by depth using their 16-byte records.
 */
// FUNCTION: FF7 0x0062ff7b
void __cdecl Field_SortTileSpritesByDepth(void)
{
    qsort(g_pFieldTileGroupBuffer, g_FieldTileGroupIndex, 0x10, (int(__cdecl *)(const void *, const void *))Field_CompareTileSpriteDepth);
}

/*
 * qsort comparator: a tile sprite's depth is depthLow + depthHigh * 256, with 0x8080 meaning zero.
 * Returns one when the first sprite is deeper than the second, otherwise zero.
 */
// FUNCTION: FF7 0x0062ff9e
int __cdecl Field_CompareTileSpriteDepth(FieldTileSpriteDepthTile *tilePtr, FieldTileSpriteDepthTile *otherTile)
{
    unsigned int depthA;
    unsigned int depthB;

    if (tilePtr->depthLow != 0x80 || tilePtr->depthHigh != 0x80) {
        depthA = tilePtr->depthLow + (tilePtr->depthHigh << 8);
    } else {
        depthA = 0;
    }
    if (otherTile->depthLow != 0x80 || otherTile->depthHigh != 0x80) {
        depthB = otherTile->depthLow + (otherTile->depthHigh << 8);
    } else {
        depthB = 0;
    }
    // Unsigned `p > q`, in that order: VC5 loads the left operand into ecx and
    // compares it against the other's slot, and the image compares [q] to p.
    return depthA > depthB;
}

/*
 * Masks each argument to sixteen bits and rejects summed extents above 0x276 by 0x1CC; the
 * masked-negative tests cannot succeed. Builds a rectangle, sets DDBLTFX size 0x64 and fill color
 * 0xFF, calls the global surface Blt with flag 0x400 and returns one only for zero HRESULT.
 */
// FUNCTION: FF7 0x00630032
int __cdecl Field_ColorFillSurfaceRect(int left, int top, int width, int height)
{
    struct {
        int hr;
        DDBLTFX effects;
        RECT rect;
    } frame;
    if ((left & 0xffff) < 0 || (top & 0xffff) < 0 ||
        (left & 0xffff) + (width & 0xffff) > 630 ||
        (top & 0xffff) + (height & 0xffff) > 460) {
        return 0;
    } else {
        frame.effects.dwSize = 100;
        frame.effects.dwFillColor = 0xff;
        SetRect(&frame.rect, left & 0xffff, top & 0xffff,
                (left & 0xffff) + (width & 0xffff), (top & 0xffff) + (height & 0xffff));
        frame.hr = ((LPDIRECTDRAWSURFACE)g_pFieldBackSurface)->Blt(&frame.rect, 0, 0, 0x400, &frame.effects);
        if (frame.hr != 0) {
            return 0;
        }
        return 1;
    }
}

/*
 * Scans the four windows for the first in state two or six, fills its rectangle at doubled short
 * coordinates/dimensions through Field_ColorFillSurfaceRect and sets g_FieldFillSurfaceDirty to
 * one; only that window is filled. If none qualifies, clears the flag.
 */
// FUNCTION: FF7 0x0063011b
void __cdecl Field_FillFirstActiveWindowRect(void)
{
    int found;
    unsigned short windowIndex;

    found = 0;
    for (windowIndex = 0; (windowIndex < 4 & found == 0) != 0; windowIndex++) {
        if (g_FieldWindows[windowIndex].state == 2 || g_FieldWindows[windowIndex].state == 6) {
            found = 1;
            /* Retail caller passes 5 arg(s) to 4-parameter `Field_ColorFillSurfaceRect` (__cdecl stack cleanup matches retail). */
            ((void(__cdecl *)(int, int, int, int, int))Field_ColorFillSurfaceRect)(g_FieldWindows[windowIndex].x << 1, g_FieldWindows[windowIndex].y << 1, g_FieldWindows[windowIndex].width << 1, g_FieldWindows[windowIndex].height << 1, 0);
            g_FieldFillSurfaceDirty = 1;
        }
    }
    if (found == 0) {
        g_FieldFillSurfaceDirty = 0;
    }
}

/*
 * Reads a TIM file into the hand-image globals: its id and flags, the CLUT block
 * (g_FieldPackedColor*1) and the image block (g_FieldPackedColor*2, width doubled). Then builds
 * packed colour records from the CLUT with the pixel mode Field_ClassifyPaletteFormat gives, and
 * returns one when that succeeds.
 */
// FUNCTION: FF7 0x0063021a
int __cdecl Field_ParsePaletteBlockPair(void *blockPtr)
{
    int *cursor;

    cursor = (int *)blockPtr;
    g_FieldHandImageTimId = *cursor;
    cursor++;
    g_FieldHandImageTimFlags = *cursor;
    cursor++;
    g_FieldPackedColorSize1 = *cursor;
    cursor++;
    g_FieldPackedColorOrgX1 = *cursor & 0xFFFF;
    g_FieldPackedColorOrgY1 = (*cursor & 0xFFFF0000) >> 16;
    cursor++;
    g_FieldPackedColorCount1 = *cursor & 0xFFFF;
    g_FieldPackedColorRows1 = (*cursor & 0xFFFF0000) >> 16;
    cursor++;
    g_pFieldPackedColorData1 = (int)cursor;
    cursor = (int *)((int)cursor + ((unsigned int)g_FieldPackedColorSize1 >> 2) * 4 + -0xC);
    g_FieldPackedColorSize2 = *cursor;
    cursor++;
    g_FieldPackedColorOrgX2 = *cursor & 0xFFFF;
    g_FieldPackedColorOrgY2 = (*cursor & 0xFFFF0000) >> 16;
    cursor++;
    g_FieldPackedColorPixelWidth2 = (*cursor & 0xFFFF & 0xFFFF) << 1;
    g_FieldPackedColorPixelHeight2 = (*cursor & 0xFFFF0000) >> 16;
    cursor++;
    g_pFieldPackedColorData2 = (int)cursor;
    if (Field_BuildPackedColorRecords((short)g_FieldPackedColorCount1, (short)g_FieldPackedColorRows1, (unsigned short *)g_pFieldPackedColorData1, Field_ClassifyPaletteFormat((short)g_FieldPackedColorCount1, (short)g_FieldPackedColorRows1)) == 0) {
        return 0;
    } else {
        return 1;
    }
}

/*
 * Loads the field's hand cursor picture: points the load options' directory at the field data
 * directory g_FieldDataDir, sets option flag 0x10, and stores Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, opts,
 * g_FieldHandTimName, context->renderObjectBank) in g_pFieldHandImageBuffer. g_FieldHandTimName is the
 * TIM file name Field_ModuleEnter copied there.
 */
// FUNCTION: FF7 0x006303a0
void __cdecl Field_LoadHandCursorImage(struct GfxEngineContext *context, PfileLoadOptions *opts)
{
    opts->directory = g_FieldDataDir;
    opts->flags |= 0x10;
    g_pFieldHandImageBuffer = Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, opts, (char *)g_FieldHandTimName, (void *)context->renderObjectBank);
}

/*
 * Blits the 32x32 source rectangle of g_FieldDDrawSurfaceDd8 to (x, y) on g_pFieldBackSurface through Field_BltFastAtPoint,
 * when there is a source surface. The two RECTs are built with SetRect.
 */
// FUNCTION: FF7 0x006303e0
void __cdecl Field_BlitTile32At(short x, short y)
{
    FieldRect source;
    FieldRect dst;

    if (g_FieldDDrawSurfaceDd8 == 0) {
        return;
    }
    SetRect(&source, 0, 0, 32, 32);
    SetRect(&dst, x, y, x + 0x20, y + 0x20);
    Field_BltFastAtPoint(g_pFieldBackSurface, g_FieldDDrawSurfaceDd8, &source, &dst, 1);
}

/*
 * Tests the screen point (screenX, screenY) against each visible model's sort record: records beyond
 * screen depth 0x15E use the smaller 25/25/70/20 rectangle, nearer ones 40/40/100/30. Returns
 * one when no record's rectangle contains the point. The record count is not clamped.
 */
// FUNCTION: FF7 0x0063044a
int __cdecl Field_IsPointClearOfModels(short screenX, short screenY)
{
    int hit;
    unsigned short halfRight;
    unsigned short extentLeft;
    unsigned short extentBottom;
    unsigned short extentTop;
    short relX;
    unsigned short i;
    short localY;

    i = 0;
    hit = 0;
    relX = screenX + 160 - g_FieldBgLayer2ScrollX;
    localY = screenY + 120 - g_FieldBgLayer2ScrollY;
    for (i = 0; i < g_FieldModelSortCount && hit == 0; i++) {
        if (*(unsigned short *)&g_FieldModelSortRecords[i].depth > 350) {
            halfRight = 25;
            extentLeft = 25;
            extentBottom = 70;
            extentTop = 20;
        } else {
            halfRight = 40;
            extentLeft = 40;
            extentBottom = 100;
            extentTop = 30;
        }
        if (g_FieldModelSortRecords[i].screenX < relX + halfRight && g_FieldModelSortRecords[i].screenX > relX - extentLeft && g_FieldModelSortRecords[i].screenY < localY + extentBottom && g_FieldModelSortRecords[i].screenY > localY - extentTop) {
            hit = 1;
        }
    }
    // The TU's last two literals (0x906740, 0x906744, after the .fpl writer's and before ad_cdr.cpp's __FILE__): the
    // mode and name of a TIM dump that no code in the image performs, discarded expressions that emit no code.
    // STRING: FF7 0x00906740
    (void)"wb";
    // STRING: FF7 0x00906744
    (void)"test.tim";
    return hit == 0;
}
}
