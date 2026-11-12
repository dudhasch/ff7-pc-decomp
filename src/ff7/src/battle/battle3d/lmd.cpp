// src/ff7/src/battle/battle3d/lmd.cpp
// Original: C:\FF7\src\battle\battle3d\lmd.cpp
// Contents: LMD model support for battle: converts PSX texture-page and CLUT ids to VRAM positions
//   and colour modes, builds and destroys per-owner texture atlases with their slot textures, and
//   turns LMD polygon data into polygon groups for model parts and weapons.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/api/battle.h"
#include "lib/api/graphics.h"
#include "lib/api/list.h"
#include "lib/api/polygon.h"
#include "lib/api/file.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_3d.h"
#include "lib/types/polygon.h"
#include "lib/types/list.h"
#include "lib/pfile.h"
#include "lib/types/graphics.h"
#include "ff7/enums/battle.h"
#include "lib/enums/graphics.h"
#include "lib/api/mem.h"
#include <stdio.h>
#include <string.h>
#include "lib/codegen.h"
extern "C" {

/*
 * Builds the texture name for LMD slice `sliceIndex` of a model:
 * the 'T' prefix form of MakeIndexedAssetName.
 */
// FUNCTION: FF7 0x005e4310
void __cdecl Lmd_MakeSliceTextureName(int sliceIndex, char *baseName, char *outName)
{
    MakeIndexedAssetName(0x54, sliceIndex, baseName, outName);
}

/*
 * PSX CLUT id to VRAM x: the low six bits times 16.
 */
// FUNCTION: FF7 0x005e432b
int __cdecl Psx_ClutIdToVramX(int value)
{
    return (value & 0x3f) << 4;
}

/*
 * PSX CLUT id to VRAM y: the id shifted right by six.
 */
// FUNCTION: FF7 0x005e4339
int __cdecl Psx_ClutIdToVramY(int value)
{
    return value >> 6;
}

/*
 * PSX texture page to VRAM x: the low four bits times 64.
 */
// FUNCTION: FF7 0x005e4344
int __cdecl Psx_TPageToVramX(int value)
{
    return (value & 0xf) << 6;
}

/*
 * PSX texture page to VRAM y: bits 4-5 times 256.
 */
// FUNCTION: FF7 0x005e4352
int __cdecl Psx_TPageToVramY(int value)
{
    return (value & 0x30) << 4;
}

/*
 * PSX texture page to colour mode (enum GfxPsxColorMode): bits 7-8.
 */
// FUNCTION: FF7 0x005e4360
int __cdecl Psx_TPageToColorMode(int value)
{
    return (value & 0x180) >> 7;
}

/*
 * Texels per 16-bit VRAM word for a colour mode: four at 4-bit, two at 8-bit, one at 16-bit, zero
 * for any other value.
 */
// FUNCTION: FF7 0x005e4370
int __cdecl Psx_ColorModeToPixelsPerWord(int mode)
{
    int pixels = 0;

    switch (mode) {
    case GFX_PSX_COLOR_MODE_4BIT:
        pixels = 4;
        break;
    case GFX_PSX_COLOR_MODE_8BIT:
        pixels = 2;
        break;
    case GFX_PSX_COLOR_MODE_16BIT:
        pixels = 1;
        break;
    }
    return pixels;
}

/*
 * The texture page byte of an LMD face word: the low eight bits.
 */
// FUNCTION: FF7 0x005e43b7
int __cdecl Lmd_FaceTPageByte(int value)
{
    int page;

    page = value & 0xff;
    return page;
}

/*
 * Finishes a loaded LMD polygon file: flattens and frees the edit lists, welds duplicate vertices
 * and builds the polygon colours.
 */
// FUNCTION: FF7 0x005e43cd
void __cdecl Lmd_FinalisePolygonData(void *pfile)
{
    if (pfile != 0) {
        Pfile_FlattenEditLists((PolygonFileArrays *)pfile);
        Pfile_DestroyEditLists((PolygonFileArrays *)pfile);
        Pfile_WeldDuplicateVertices((PolygonFileArrays *)pfile);
        Pfile_BuildPolygonColors((PolygonFileArrays *)pfile);
    }
}

// The original caller supplies the 12-byte return buffer explicitly.
typedef struct {
    int status;
    int unused;
    int texture;
} LmdTextureResult;
/* Unprototyped retail call signature (parameter/return width differs from callee definition). */
extern LmdTextureResult *__cdecl Pfile_LoadRsdTexture(LmdTextureResult *result, int options, void *name);
__declspec(dllimport) void __stdcall OutputDebugStringA(const char *text);

/*
 * Destroys an LMD texture atlas: its slot list and the record.
 */
// FUNCTION: FF7 0x005e4408
void __cdecl Lmd_DestroyTextureAtlas(Battle3dLmdFaceData *atlas)
{
    if (atlas != 0) {
        DestroyList(atlas->slotList);
        // STRING: FF7 0x008ffc78
        GameHeapFree(atlas, "C:\\FF7\\src\\battle\\battle3d\\lmd.cpp", 250);
    }
}

/*
 * Creates an LMD texture atlas record for the specified owner.
 */
// FUNCTION: FF7 0x005e4438
void *__cdecl Lmd_CreateTextureAtlas(int owner, void *timObject)
{
    Battle3dLmdFaceData *atlas;

    // STRING: FF7 0x008ffc9c
    atlas = (struct Battle3dLmdFaceData *)GameHeapAlloc(1, 5656, "C:\\FF7\\src\\battle\\battle3d\\lmd.cpp", 258);
    if (atlas != 0) {
        atlas->faceKind = owner;
        atlas->slotList = CreateList(0);
    }
    return atlas;
}

/*
 * Packs CLUT/tpage into the list key and returns its atlas slot.
 */
// FUNCTION: FF7 0x005e447d
Battle3dLmdTextureSlot *__cdecl Lmd_FindTextureSlot(int tpage, int clut, Battle3dLmdFaceData *atlas)
{
    int key;
    Battle3dLmdTextureSlot *record;
    int foundIndex;
    record = 0;
    if (atlas != 0 && atlas->faceKind == 1) {
        *(short *)&key = clut;
        *(short *)((char *)&key + 2) = tpage;
        if (ListFindIndexByValue(&foundIndex, ListCompareEqual, key, atlas->slotList) != 0) {
            record = &atlas->slots[foundIndex];
        }
    }
    return record;
}

/*
 * Expands four LMD face formats, builds texture bounds, and assembles polygon groups.
 */
// FUNCTION: FF7 0x005e44df
int __cdecl Battle_BuildPolygonDataFromLmd(Battle3dPartyLoadDesc *desc, int count, Battle3dLmdFaceData *faceData, PolygonTim *lmdHeader)
{
    int localVertZ;
    float uScale;
    int pageWord;
    int yCoordinate;
    Battle3dPolygonData *curPoly;
    float vVal;
    int goodFlag;
    int vertexBlockSize;
    int faceIndex;
    int packedKey;
    float uCoord;
    float uBias;
    Battle3dLmdFloatVec *pVertBase;
    int sizingPosition;
    int decodePos;
    Battle3dLmdTextureSlot *atlasSlot;
    int frameRecIdx;
    int allocCnt;
    int workAnyFound;
    Battle3dLmdGouraudQuad *gouraudQuadPtr;
    int px;
    int vtxCount;
    int faceCap;
    int readPtr;
    char msgBuffer[200];
    Battle3dLmdPart *recordArray;
    PolygonFileArrays *pArrays;
    int tmpBlock;
    int semiMode;
    int blockFaces;
    Battle3dLmdTriangle *triPointer;
    Battle3dLmdGouraudTriangle *gTriPtr;
    int initialVal;
    BattleFxHellBomberEndVertex *pVertOut;
    Battle3dPolygonRecord *recCursor;
    Vec3sPacked *vertSrcPtr;
    Battle3dLmdFaceRecord *srcFace;
    Battle3dLmdTintedQuad *tintQuadPtr;
    int groupCnt;
    Battle3dLmdQuad *quadPtrPtr;
    int foundIndex;
    Battle3dPolygonData *polyBasePtr;
    float atlasOffsetV;
    int totalFaces;
    Battle3dLmdTextureSlot *uvSlotPtr;
    float textureHeightScale;
    int selShadeValue;
    int formatIdx;

    readPtr = count;
    goodFlag = 1;
    pVertBase = 0;
    tmpBlock = 0;
    polyBasePtr = 0;
    recordArray = 0;
    uBias = 0.0f;
    atlasOffsetV = 0.0f;
    uScale = 1.0f;
    textureHeightScale = 1.0f;
    pArrays = Pfile_CreatePolygonData(1, 1);
    if (pArrays != 0) {
        initialVal = 0;
        vertexBlockSize = *(int *)readPtr;
        vtxCount = (unsigned int)vertexBlockSize >> 3;
        readPtr += 4;
        // STRING: FF7 0x008ffcc0
        pVertBase = (Battle3dLmdFloatVec *)GameHeapAllocRaw(vtxCount * 0xC, "C:\\FF7\\src\\battle\\battle3d\\lmd.cpp", 357);
        pVertOut = (BattleFxHellBomberEndVertex *)pVertBase;
        if (pVertBase != 0) {
            vertSrcPtr = (Vec3sPacked *)readPtr;
            for (formatIdx = 0; formatIdx < vtxCount; formatIdx++) {
                px = vertSrcPtr->vx;
                pVertOut->posX = px;
                yCoordinate = vertSrcPtr->vy;
                pVertOut->posY = yCoordinate;
                localVertZ = vertSrcPtr->vz;
                pVertOut->posZ = localVertZ;
                vertSrcPtr = (Vec3sPacked *)((char *)vertSrcPtr + 8);
                pVertOut++;
            }
            readPtr += vertexBlockSize;
        } else {
            goodFlag = 1;
        }
        totalFaces = 0;
        sizingPosition = readPtr;
        for (formatIdx = 0; formatIdx < BATTLE3D_LMD_BLOCK_COUNT; formatIdx++) {
            switch (formatIdx) {
            case BATTLE3D_LMD_BLOCK_TEXTURED_TRIANGLES:
                blockFaces = *(unsigned short *)sizingPosition;
                sizingPosition += 2;
                pageWord = *(unsigned short *)sizingPosition;
                semiMode = (pageWord & 0x60) >> 5;
                sizingPosition += 2;
                switch (desc->loadKind) {
                case BATTLE3D_LMD_LOAD_UNCOLORED:
                    sizingPosition += blockFaces << 4;
                    break;
                case BATTLE3D_LMD_LOAD_VERTEX_COLORED:
                    sizingPosition += blockFaces * 20;
                }
                break;
            case BATTLE3D_LMD_BLOCK_TEXTURED_QUADS:
                blockFaces = *(int *)sizingPosition;
                sizingPosition += 4;
                switch (desc->loadKind) {
                case BATTLE3D_LMD_LOAD_UNCOLORED:
                    sizingPosition += blockFaces * 20;
                    break;
                case BATTLE3D_LMD_LOAD_VERTEX_COLORED:
                    sizingPosition += blockFaces * 24;
                }
                break;
            case BATTLE3D_LMD_BLOCK_GOURAUD_TRIANGLES:
                blockFaces = *(int *)sizingPosition;
                sizingPosition += 4;
                sizingPosition += blockFaces * 20;
                break;
            case BATTLE3D_LMD_BLOCK_GOURAUD_QUADS:
                blockFaces = *(int *)sizingPosition;
                sizingPosition += 4;
                sizingPosition += blockFaces * 24;
            }
            totalFaces += blockFaces;
        }
        faceCap = totalFaces;
        if (totalFaces > 0 && faceCap > 0) {
            // STRING: FF7 0x008ffce4
            polyBasePtr = (Battle3dPolygonData *)GameHeapAlloc(totalFaces, 56, "C:\\FF7\\src\\battle\\battle3d\\lmd.cpp", 451);
            curPoly = (Battle3dPolygonData *)polyBasePtr;
            if (totalFaces > faceCap) {
                allocCnt = totalFaces;
            } else {
                allocCnt = faceCap;
            }
            // STRING: FF7 0x008ffd08
            recordArray = (Battle3dLmdPart *)GameHeapAlloc(allocCnt, 88, "C:\\FF7\\src\\battle\\battle3d\\lmd.cpp", 463);
            recCursor = (Battle3dPolygonRecord *)recordArray;
            if (polyBasePtr != 0 && recordArray != 0) {
                decodePos = readPtr;
                for (formatIdx = 0; formatIdx < BATTLE3D_LMD_BLOCK_COUNT; formatIdx++) {
                    switch (formatIdx) {
                    case BATTLE3D_LMD_BLOCK_TEXTURED_TRIANGLES:
                        groupCnt = *(unsigned short *)decodePos;
                        decodePos += 2;
                        decodePos += 2;
                        switch (desc->loadKind) {
                        case BATTLE3D_LMD_LOAD_UNCOLORED:
                            srcFace = (Battle3dLmdFaceRecord *)decodePos;
                            for (faceIndex = 0; faceIndex < groupCnt; faceIndex++) {
                                curPoly->enabled = 0;
                                curPoly->isUsed = 1;
                                curPoly->scaledX = (unsigned int)srcFace->x >> 3;
                                curPoly->scaledY = (unsigned int)srcFace->y >> 3;
                                curPoly->scaledZ = (unsigned int)srcFace->z >> 3;
                                recCursor->polyKind = 3;
                                if (semiMode != 0) {
                                    recCursor->blendMode = semiMode;
                                    recCursor->flags |= 4;
                                    recCursor->flags |= recCursor->blendMode << 8;
                                } else {
                                    recCursor->blendMode = BLEND_NONE;
                                }
                                recCursor->isFlatShaded = 1;
                                recCursor->isGouraudShaded = 0;
                                recCursor->hasTexture = 1;
                                recCursor->hasTexturePage = 1;
                                if (desc->useFaceTexturePage != 0) {
                                    /* Retail caller prototype (char) -> int preserved for /Od argument promotion on `Lmd_FaceTPageByte`. */
                                    recCursor->texturePage = ((int(__cdecl *)(char))Lmd_FaceTPageByte)((char)srcFace->tpageByte);
                                } else {
                                    recCursor->texturePage = pageWord;
                                    if (*(unsigned char *)&srcFace->tpageByte != 0) {
                                        recCursor->flags |= 4;
                                        recCursor->flags |= 0x100;
                                    } else {
                                        recCursor->blendMode = BLEND_NONE;
                                    }
                                }
                                recCursor->clutId = srcFace->clutId;
                                uvSlotPtr = Lmd_FindTextureSlot(recCursor->texturePage, recCursor->clutId, faceData);
                                if (uvSlotPtr != 0) {
                                    uBias = -uvSlotPtr->atlasU;
                                    atlasOffsetV = -uvSlotPtr->atlasV;
                                    *(int *)&uScale = uvSlotPtr->uSizePow2Bits;
                                    *(int *)&textureHeightScale = uvSlotPtr->vSizePow2Bits;
                                    recCursor->widthUnits = uvSlotPtr->slotIndex;
                                }
                                uCoord = srcFace->u0 + uBias;
                                vVal = srcFace->v0 + atlasOffsetV;
                                if (uvSlotPtr != 0 && uvSlotPtr->uSize > 0.0f && uCoord == 0.0f) {
                                    uCoord = 0.5f;
                                }
                                if (uvSlotPtr != 0 && uvSlotPtr->uSize > 0.0f && uCoord == uvSlotPtr->uSize) {
                                    uCoord = uvSlotPtr->uSize - 0.5f;
                                }
                                if (uvSlotPtr != 0 && uvSlotPtr->vSize > 0.0f && vVal == 0.0f) {
                                    vVal = 0.5f;
                                }
                                if (uvSlotPtr != 0 && uvSlotPtr->vSize > 0.0f && vVal == uvSlotPtr->vSize) {
                                    vVal = uvSlotPtr->vSize - 0.5f;
                                }
                                recCursor->u0.f = uCoord / uScale;
                                recCursor->v0.f = vVal / textureHeightScale;
                                uCoord = srcFace->u1 + uBias;
                                vVal = srcFace->v1 + atlasOffsetV;
                                if (uvSlotPtr != 0 && uvSlotPtr->uSize > 0.0f && uCoord == 0.0f) {
                                    uCoord = 0.5f;
                                }
                                if (uvSlotPtr != 0 && uvSlotPtr->uSize > 0.0f && uCoord == uvSlotPtr->uSize) {
                                    uCoord = uvSlotPtr->uSize - 0.5f;
                                }
                                if (uvSlotPtr != 0 && uvSlotPtr->vSize > 0.0f && vVal == 0.0f) {
                                    vVal = 0.5f;
                                }
                                if (uvSlotPtr != 0 && uvSlotPtr->vSize > 0.0f && vVal == uvSlotPtr->vSize) {
                                    vVal = uvSlotPtr->vSize - 0.5f;
                                }
                                recCursor->u1.f = uCoord / uScale;
                                recCursor->v1.f = vVal / textureHeightScale;
                                uCoord = srcFace->u2 + uBias;
                                vVal = srcFace->v2 + atlasOffsetV;
                                if (uvSlotPtr != 0 && uvSlotPtr->uSize > 0.0f && uCoord == 0.0f) {
                                    uCoord = 0.5f;
                                }
                                if (uvSlotPtr != 0 && uvSlotPtr->uSize > 0.0f && uCoord == uvSlotPtr->uSize) {
                                    uCoord = uvSlotPtr->uSize - 0.5f;
                                }
                                if (uvSlotPtr != 0 && uvSlotPtr->vSize > 0.0f && vVal == 0.0f) {
                                    vVal = 0.5f;
                                }
                                if (uvSlotPtr != 0 && uvSlotPtr->vSize > 0.0f && vVal == uvSlotPtr->vSize) {
                                    vVal = uvSlotPtr->vSize - 0.5f;
                                }
                                recCursor->u2.f = uCoord / uScale;
                                recCursor->v2.f = vVal / textureHeightScale;
                                selShadeValue = srcFace->shade;
                                if (selShadeValue != 0) {
                                    selShadeValue = selShadeValue * 2 - 1;
                                }
                                recCursor->color0.packed = 0xFFFFFFFF;
                                recCursor->color0.byte.r = selShadeValue;
                                recCursor->color0.byte.g = selShadeValue;
                                recCursor->color0.byte.b = selShadeValue;
                                recCursor->color1.packed = 0xFFFFFFFF;
                                recCursor->color1.byte.r = selShadeValue;
                                recCursor->color1.byte.g = selShadeValue;
                                recCursor->color1.byte.b = selShadeValue;
                                recCursor->color2.packed = 0xFFFFFFFF;
                                recCursor->color2.byte.r = selShadeValue;
                                recCursor->color2.byte.g = selShadeValue;
                                recCursor->color2.byte.b = selShadeValue;
                                recCursor++;
                                curPoly++;
                                srcFace++;
                            }
                            decodePos += groupCnt << 4;
                            break;
                        case BATTLE3D_LMD_LOAD_VERTEX_COLORED:
                            triPointer = (Battle3dLmdTriangle *)decodePos;
                            for (faceIndex = 0; faceIndex < groupCnt; faceIndex++) {
                                curPoly->enabled = 0;
                                curPoly->isUsed = 1;
                                curPoly->scaledX = (unsigned int)triPointer->x >> 3;
                                curPoly->scaledY = (unsigned int)triPointer->y >> 3;
                                curPoly->scaledZ = (unsigned int)triPointer->z >> 3;
                                recCursor->polyKind = 3;
                                if ((triPointer->flags & 0x2000000) != 0) {
                                    recCursor->blendMode = (triPointer->flags & 0x60) >> 5;
                                    recCursor->flags |= 4;
                                    recCursor->flags |= recCursor->blendMode << 8;
                                } else {
                                    recCursor->blendMode = BLEND_NONE;
                                }
                                recCursor->isFlatShaded = 1;
                                recCursor->isGouraudShaded = 0;
                                recCursor->hasTexture = 1;
                                recCursor->hasTexturePage = 1;
                                recCursor->texturePage = triPointer->flags & 0xFFFFFF9F;
                                recCursor->clutId = triPointer->clutId;
                                uvSlotPtr = Lmd_FindTextureSlot(recCursor->texturePage, recCursor->clutId, faceData);
                                if (uvSlotPtr != 0) {
                                    uBias = -uvSlotPtr->atlasU;
                                    atlasOffsetV = -uvSlotPtr->atlasV;
                                    *(int *)&uScale = uvSlotPtr->uSizePow2Bits;
                                    *(int *)&textureHeightScale = uvSlotPtr->vSizePow2Bits;
                                    recCursor->widthUnits = uvSlotPtr->slotIndex;
                                }
                                recCursor->u0.f = (triPointer->u0 + uBias) / uScale;
                                recCursor->v0.f = (triPointer->v0 + atlasOffsetV) / textureHeightScale;
                                recCursor->u1.f = (triPointer->u1 + uBias) / uScale;
                                recCursor->v1.f = (triPointer->v1 + atlasOffsetV) / textureHeightScale;
                                recCursor->u2.f = (triPointer->u2 + uBias) / uScale;
                                recCursor->v2.f = (triPointer->v2 + atlasOffsetV) / textureHeightScale;
                                recCursor->color0.packed = triPointer->packedColor;
                                recCursor->color0.byte.a = 0xFF;
                                recCursor->color1.packed = triPointer->packedColor;
                                recCursor->color1.byte.a = 0xFF;
                                recCursor->color2.packed = triPointer->packedColor;
                                recCursor->color2.byte.a = 0xFF;
                                recCursor++;
                                curPoly++;
                                triPointer++;
                            }
                            decodePos += groupCnt * 20;
                        }
                        break;
                    case BATTLE3D_LMD_BLOCK_TEXTURED_QUADS:
                        switch (desc->loadKind) {
                        case BATTLE3D_LMD_LOAD_UNCOLORED:
                            groupCnt = *(int *)decodePos;
                            decodePos += 4;
                            quadPtrPtr = (Battle3dLmdQuad *)decodePos;
                            for (faceIndex = 0; faceIndex < groupCnt; faceIndex++) {
                                curPoly->enabled = 1;
                                curPoly->isUsed = 1;
                                curPoly->scaledX = (unsigned int)quadPtrPtr->x >> 3;
                                curPoly->scaledY = (unsigned int)quadPtrPtr->y >> 3;
                                curPoly->scaledZ = (unsigned int)quadPtrPtr->z >> 3;
                                curPoly->widthUnits = (unsigned int)quadPtrPtr->width >> 3;
                                recCursor->polyKind = 3;
                                if (semiMode != 0) {
                                    recCursor->blendMode = semiMode;
                                    recCursor->flags |= 4;
                                    recCursor->flags |= recCursor->blendMode << 8;
                                } else {
                                    recCursor->blendMode = BLEND_NONE;
                                }
                                recCursor->isFlatShaded = 1;
                                recCursor->isGouraudShaded = 0;
                                recCursor->hasTexture = 1;
                                recCursor->hasTexturePage = 1;
                                recCursor->texturePage = quadPtrPtr->tpageByte;
                                if (desc->useFaceTexturePage != 0) {
                                    /* Retail caller prototype (char) -> int preserved for /Od argument promotion on `Lmd_FaceTPageByte`. */
                                    recCursor->texturePage = ((int(__cdecl *)(char))Lmd_FaceTPageByte)(*(char *)&quadPtrPtr->tpageByte);
                                } else {
                                    recCursor->texturePage = pageWord;
                                    if (quadPtrPtr->tpageByte != 0) {
                                        recCursor->flags |= 4;
                                        recCursor->flags |= 0x100;
                                    } else {
                                        recCursor->blendMode = BLEND_NONE;
                                    }
                                }
                                recCursor->clutId = quadPtrPtr->clutId;
                                uvSlotPtr = Lmd_FindTextureSlot(recCursor->texturePage, recCursor->clutId, faceData);
                                if (uvSlotPtr != 0) {
                                    uBias = -uvSlotPtr->atlasU;
                                    atlasOffsetV = -uvSlotPtr->atlasV;
                                    *(int *)&uScale = uvSlotPtr->uSizePow2Bits;
                                    *(int *)&textureHeightScale = uvSlotPtr->vSizePow2Bits;
                                    recCursor->widthUnits = uvSlotPtr->slotIndex;
                                }
                                uCoord = quadPtrPtr->u0 + uBias;
                                vVal = quadPtrPtr->v0 + atlasOffsetV;
                                if (uvSlotPtr != 0 && uvSlotPtr->uSize > 0.0f && uCoord == 0.0f) {
                                    uCoord = 0.5f;
                                }
                                if (uvSlotPtr != 0 && uvSlotPtr->uSize > 0.0f && uCoord == uvSlotPtr->uSize) {
                                    uCoord = uvSlotPtr->uSize - 0.5f;
                                }
                                if (uvSlotPtr != 0 && uvSlotPtr->vSize > 0.0f && vVal == 0.0f) {
                                    vVal = 0.5f;
                                }
                                if (uvSlotPtr != 0 && uvSlotPtr->vSize > 0.0f && vVal == uvSlotPtr->vSize) {
                                    vVal = uvSlotPtr->vSize - 0.5f;
                                }
                                recCursor->u0.f = uCoord / uScale;
                                recCursor->v0.f = vVal / textureHeightScale;
                                uCoord = quadPtrPtr->u1 + uBias;
                                vVal = quadPtrPtr->v1 + atlasOffsetV;
                                if (uvSlotPtr != 0 && uvSlotPtr->uSize > 0.0f && uCoord == 0.0f) {
                                    uCoord = 0.5f;
                                }
                                if (uvSlotPtr != 0 && uvSlotPtr->uSize > 0.0f && uCoord == uvSlotPtr->uSize) {
                                    uCoord = uvSlotPtr->uSize - 0.5f;
                                }
                                if (uvSlotPtr != 0 && uvSlotPtr->vSize > 0.0f && vVal == 0.0f) {
                                    vVal = 0.5f;
                                }
                                if (uvSlotPtr != 0 && uvSlotPtr->vSize > 0.0f && vVal == uvSlotPtr->vSize) {
                                    vVal = uvSlotPtr->vSize - 0.5f;
                                }
                                recCursor->u1.f = uCoord / uScale;
                                recCursor->v1.f = vVal / textureHeightScale;
                                uCoord = quadPtrPtr->u2 + uBias;
                                vVal = quadPtrPtr->v2 + atlasOffsetV;
                                if (uvSlotPtr != 0 && uvSlotPtr->uSize > 0.0f && uCoord == 0.0f) {
                                    uCoord = 0.5f;
                                }
                                if (uvSlotPtr != 0 && uvSlotPtr->uSize > 0.0f && uCoord == uvSlotPtr->uSize) {
                                    uCoord = uvSlotPtr->uSize - 0.5f;
                                }
                                if (uvSlotPtr != 0 && uvSlotPtr->vSize > 0.0f && vVal == 0.0f) {
                                    vVal = 0.5f;
                                }
                                if (uvSlotPtr != 0 && uvSlotPtr->vSize > 0.0f && vVal == uvSlotPtr->vSize) {
                                    vVal = uvSlotPtr->vSize - 0.5f;
                                }
                                recCursor->u2.f = uCoord / uScale;
                                recCursor->v2.f = vVal / textureHeightScale;
                                uCoord = quadPtrPtr->u3 + uBias;
                                vVal = quadPtrPtr->v3 + atlasOffsetV;
                                if (uvSlotPtr != 0 && uvSlotPtr->uSize > 0.0f && uCoord == 0.0f) {
                                    uCoord = 0.5f;
                                }
                                if (uvSlotPtr != 0 && uvSlotPtr->uSize > 0.0f && uCoord == uvSlotPtr->uSize) {
                                    uCoord = uvSlotPtr->uSize - 0.5f;
                                }
                                if (uvSlotPtr != 0 && uvSlotPtr->vSize > 0.0f && vVal == 0.0f) {
                                    vVal = 0.5f;
                                }
                                if (uvSlotPtr != 0 && uvSlotPtr->vSize > 0.0f && vVal == uvSlotPtr->vSize) {
                                    vVal = uvSlotPtr->vSize - 0.5f;
                                }
                                recCursor->u3.f = uCoord / uScale;
                                recCursor->v3.f = vVal / textureHeightScale;
                                selShadeValue = quadPtrPtr->shade;
                                if (selShadeValue != 0) {
                                    selShadeValue = selShadeValue * 2 - 1;
                                }
                                recCursor->color0.packed = 0xFFFFFFFF;
                                recCursor->color0.byte.r = selShadeValue;
                                recCursor->color0.byte.g = selShadeValue;
                                recCursor->color0.byte.b = selShadeValue;
                                recCursor->color1.packed = 0xFFFFFFFF;
                                recCursor->color1.byte.r = selShadeValue;
                                recCursor->color1.byte.g = selShadeValue;
                                recCursor->color1.byte.b = selShadeValue;
                                recCursor->color2.packed = 0xFFFFFFFF;
                                recCursor->color2.byte.r = selShadeValue;
                                recCursor->color2.byte.g = selShadeValue;
                                recCursor->color2.byte.b = selShadeValue;
                                recCursor->color3.packed = 0xFFFFFFFF;
                                recCursor->color3.byte.r = selShadeValue;
                                recCursor->color3.byte.g = selShadeValue;
                                recCursor->color3.byte.b = selShadeValue;
                                recCursor++;
                                curPoly++;
                                quadPtrPtr++;
                            }
                            decodePos += groupCnt * 20;
                            break;
                        case BATTLE3D_LMD_LOAD_VERTEX_COLORED:
                            groupCnt = *(int *)decodePos;
                            decodePos += 4;
                            tintQuadPtr = (Battle3dLmdTintedQuad *)decodePos;
                            for (faceIndex = 0; faceIndex < groupCnt; faceIndex++) {
                                curPoly->enabled = 1;
                                curPoly->isUsed = 1;
                                curPoly->scaledX = (unsigned int)tintQuadPtr->x >> 3;
                                curPoly->scaledY = (unsigned int)tintQuadPtr->y >> 3;
                                curPoly->scaledZ = (unsigned int)tintQuadPtr->z >> 3;
                                curPoly->widthUnits = (unsigned int)tintQuadPtr->width >> 3;
                                recCursor->polyKind = 3;
                                if ((tintQuadPtr->flags & 0x2000000) != 0) {
                                    recCursor->blendMode = (tintQuadPtr->flags & 0x60) >> 5;
                                    recCursor->flags |= 4;
                                    recCursor->flags |= recCursor->blendMode << 8;
                                } else {
                                    recCursor->blendMode = BLEND_NONE;
                                }
                                recCursor->isFlatShaded = 1;
                                recCursor->isGouraudShaded = 0;
                                recCursor->hasTexture = 1;
                                recCursor->hasTexturePage = 1;
                                recCursor->texturePage = tintQuadPtr->flags;
                                recCursor->clutId = tintQuadPtr->clutId;
                                uvSlotPtr = Lmd_FindTextureSlot(recCursor->texturePage, recCursor->clutId, faceData);
                                if (uvSlotPtr != 0) {
                                    uBias = -uvSlotPtr->atlasU;
                                    atlasOffsetV = -uvSlotPtr->atlasV;
                                    *(int *)&uScale = uvSlotPtr->uSizePow2Bits;
                                    *(int *)&textureHeightScale = uvSlotPtr->vSizePow2Bits;
                                    recCursor->widthUnits = uvSlotPtr->slotIndex;
                                }
                                recCursor->u0.f = (tintQuadPtr->u0 + uBias) / uScale;
                                recCursor->v0.f = (tintQuadPtr->v0 + atlasOffsetV) / textureHeightScale;
                                recCursor->u1.f = (tintQuadPtr->u1 + uBias) / uScale;
                                recCursor->v1.f = (tintQuadPtr->v1 + atlasOffsetV) / textureHeightScale;
                                recCursor->u2.f = (tintQuadPtr->u2 + uBias) / uScale;
                                recCursor->v2.f = (tintQuadPtr->v2 + atlasOffsetV) / textureHeightScale;
                                recCursor->u3.f = (tintQuadPtr->u3 + uBias) / uScale;
                                recCursor->v3.f = (tintQuadPtr->v3 + atlasOffsetV) / textureHeightScale;
                                recCursor->color0.packed = tintQuadPtr->packedColor;
                                recCursor->color0.byte.a = 0xFF;
                                recCursor->color1.packed = tintQuadPtr->packedColor;
                                recCursor->color1.byte.a = 0xFF;
                                recCursor->color2.packed = tintQuadPtr->packedColor;
                                recCursor->color2.byte.a = 0xFF;
                                recCursor->color3.packed = tintQuadPtr->packedColor;
                                recCursor->color3.byte.a = 0xFF;
                                recCursor++;
                                curPoly++;
                                tintQuadPtr++;
                            }
                            decodePos += groupCnt * 24;
                        }
                        break;
                    case BATTLE3D_LMD_BLOCK_GOURAUD_TRIANGLES:
                        groupCnt = *(int *)decodePos;
                        decodePos += 4;
                        gTriPtr = (Battle3dLmdGouraudTriangle *)decodePos;
                        for (faceIndex = 0; faceIndex < groupCnt; faceIndex++) {
                            curPoly->enabled = 0;
                            curPoly->isUsed = 1;
                            curPoly->scaledX = (unsigned int)gTriPtr->x >> 3;
                            curPoly->scaledY = (unsigned int)gTriPtr->y >> 3;
                            curPoly->scaledZ = (unsigned int)gTriPtr->z >> 3;
                            if (desc->hasPolygons != 0) {
                                recCursor->polyKind = 0;
                            } else {
                                recCursor->polyKind = 1;
                            }
                            if ((gTriPtr->flags.word & 0x2000000) != 0) {
                                recCursor->flags |= 4;
                                recCursor->flags |= semiMode << 8;
                                recCursor->blendMode = semiMode;
                            } else {
                                recCursor->blendMode = BLEND_NONE;
                            }
                            recCursor->isFlatShaded = 0;
                            if (desc->hasPolygons != 0) {
                                recCursor->isGouraudShaded = 0;
                            } else {
                                recCursor->isGouraudShaded = 1;
                            }
                            recCursor->hasTexture = 0;
                            recCursor->color0.byte.r = gTriPtr->flags.rgb.r;
                            recCursor->color0.byte.g = gTriPtr->flags.rgb.g;
                            recCursor->color0.byte.b = gTriPtr->flags.rgb.b;
                            recCursor->color0.byte.a = 0xFF;
                            recCursor->color1.byte.r = gTriPtr->r1;
                            recCursor->color1.byte.g = gTriPtr->g1;
                            recCursor->color1.byte.b = gTriPtr->b1;
                            recCursor->color1.byte.a = 0xFF;
                            recCursor->color2.byte.r = gTriPtr->r2;
                            recCursor->color2.byte.g = gTriPtr->g2;
                            recCursor->color2.byte.b = gTriPtr->b2;
                            recCursor->color2.byte.a = 0xFF;
                            recCursor++;
                            curPoly++;
                            gTriPtr++;
                        }
                        decodePos += groupCnt * 20;
                        break;
                    case BATTLE3D_LMD_BLOCK_GOURAUD_QUADS:
                        groupCnt = *(int *)decodePos;
                        decodePos += 4;
                        gouraudQuadPtr = (Battle3dLmdGouraudQuad *)decodePos;
                        for (faceIndex = 0; faceIndex < groupCnt; faceIndex++) {
                            curPoly->enabled = 1;
                            curPoly->isUsed = 1;
                            curPoly->scaledX = (unsigned int)gouraudQuadPtr->x >> 3;
                            curPoly->scaledY = (unsigned int)gouraudQuadPtr->y >> 3;
                            curPoly->scaledZ = (unsigned int)gouraudQuadPtr->z >> 3;
                            curPoly->widthUnits = (unsigned int)gouraudQuadPtr->width >> 3;
                            if (desc->hasPolygons != 0) {
                                recCursor->polyKind = 0;
                            } else {
                                recCursor->polyKind = 1;
                            }
                            if ((gouraudQuadPtr->flags.word & 0x2000000) != 0) {
                                recCursor->flags |= 4;
                                recCursor->flags |= semiMode << 8;
                                recCursor->blendMode = semiMode;
                            } else {
                                recCursor->blendMode = BLEND_NONE;
                            }
                            recCursor->isFlatShaded = 0;
                            if (desc->hasPolygons != 0) {
                                recCursor->isGouraudShaded = 0;
                            } else {
                                recCursor->isGouraudShaded = 1;
                            }
                            recCursor->hasTexture = 0;
                            recCursor->color0.byte.r = gouraudQuadPtr->flags.rgb.r;
                            recCursor->color0.byte.g = gouraudQuadPtr->flags.rgb.g;
                            recCursor->color0.byte.b = gouraudQuadPtr->flags.rgb.b;
                            recCursor->color0.byte.a = 0xFF;
                            recCursor->color1.byte.r = gouraudQuadPtr->r1;
                            recCursor->color1.byte.g = gouraudQuadPtr->g1;
                            recCursor->color1.byte.b = gouraudQuadPtr->b1;
                            recCursor->color1.byte.a = 0xFF;
                            recCursor->color2.byte.r = gouraudQuadPtr->r2;
                            recCursor->color2.byte.g = gouraudQuadPtr->g2;
                            recCursor->color2.byte.b = gouraudQuadPtr->b2;
                            recCursor->color2.byte.a = 0xFF;
                            recCursor->color3.byte.r = gouraudQuadPtr->r3;
                            recCursor->color3.byte.g = gouraudQuadPtr->g3;
                            recCursor->color3.byte.b = gouraudQuadPtr->b3;
                            recCursor->color3.byte.a = 0xFF;
                            recCursor++;
                            curPoly++;
                            gouraudQuadPtr++;
                        }
                        decodePos += groupCnt * 24;
                    }
                }
                goodFlag = 0;
            } else {
                goodFlag = 1;
            }
        } else {
            goodFlag = 1;
        }
        switch (faceData->faceKind) {
        case BATTLE3D_LMD_PASS_COLLECT_ATLAS:
            if (totalFaces > 0) {
                workAnyFound = 0;
                recCursor = (Battle3dPolygonRecord *)recordArray;
                for (frameRecIdx = 0; frameRecIdx < totalFaces; frameRecIdx++) {
                    if (recCursor->hasTexture != 0) {
                        workAnyFound = 1;
                        break;
                    }
                    recCursor++;
                }
                if (workAnyFound != 0) {
                    curPoly = (Battle3dPolygonData *)polyBasePtr;
                    recCursor = (Battle3dPolygonRecord *)recordArray;
                    for (frameRecIdx = 0; frameRecIdx < totalFaces; frameRecIdx++) {
                        if (recCursor->hasTexture != 0) {
                            if (0, 0) {
                                // STRING: FF7 0x008ffd2c
                                sprintf(msgBuffer, "C: %d T: %d \n", recCursor->clutId, recCursor->texturePage);
                                OutputDebugStringA(msgBuffer);
                            }
                            *(short *)&packedKey = *(short *)&recCursor->clutId;
                            *(short *)((char *)&packedKey + 2) = *(short *)&recCursor->texturePage;
                            if (ListFindIndexByValue(&foundIndex, ListCompareEqual, packedKey, faceData->slotList) != 0) { /* empty first arm: lib/codegen.h */
                            } else {
                                ListAppendValue(packedKey, faceData->slotList);
                                ListFindIndexByValue(&foundIndex, ListCompareEqual, packedKey, faceData->slotList);
                            }
                            if (foundIndex >= 0 && foundIndex < 100) {
                                atlasSlot = &faceData->slots[foundIndex];
                                if (atlasSlot->state == 0) {
                                    atlasSlot->uMinBits = recCursor->u0.bits;
                                    atlasSlot->uMaxBits = recCursor->u0.bits;
                                    atlasSlot->vMinBits = recCursor->v0.bits;
                                    atlasSlot->vMaxBits = recCursor->v0.bits;
                                    atlasSlot->clutId = recCursor->clutId;
                                    atlasSlot->texturePage = recCursor->texturePage;
                                    atlasSlot->slotIndex = foundIndex;
                                    if (faceData->polygonsBuilt != 0) {
                                        if (Psx_ClutIdToVramX(atlasSlot->clutId) < faceData->clutMinX) {
                                            faceData->clutMinX = Psx_ClutIdToVramX(atlasSlot->clutId);
                                        }
                                        if (Psx_ClutIdToVramY(atlasSlot->clutId) < faceData->clutMinY) {
                                            faceData->clutMinY = Psx_ClutIdToVramY(atlasSlot->clutId);
                                        }
                                        if (Psx_ClutIdToVramX(atlasSlot->clutId) > faceData->clutMaxX) {
                                            faceData->clutMaxX = Psx_ClutIdToVramX(atlasSlot->clutId);
                                        }
                                        if (Psx_ClutIdToVramY(atlasSlot->clutId) > faceData->clutMaxY) {
                                            faceData->clutMaxY = Psx_ClutIdToVramY(atlasSlot->clutId);
                                        }
                                        if (Psx_TPageToVramX(atlasSlot->texturePage) < faceData->tpageMinX) {
                                            faceData->tpageMinX = Psx_TPageToVramX(atlasSlot->texturePage);
                                        }
                                        if (Psx_TPageToVramY(atlasSlot->texturePage) < faceData->tpageMinY) {
                                            faceData->tpageMinY = Psx_TPageToVramY(atlasSlot->texturePage);
                                        }
                                        if (Psx_TPageToVramX(atlasSlot->texturePage) > faceData->tpageMaxX) {
                                            faceData->tpageMaxX = Psx_TPageToVramX(atlasSlot->texturePage);
                                        }
                                        if (Psx_TPageToVramY(atlasSlot->texturePage) > faceData->tpageMaxY) {
                                            faceData->tpageMaxY = Psx_TPageToVramY(atlasSlot->texturePage);
                                        }
                                    } else {
                                        faceData->clutMinX = Psx_ClutIdToVramX(atlasSlot->clutId);
                                        faceData->clutMinY = Psx_ClutIdToVramY(atlasSlot->clutId);
                                        faceData->clutMaxX = Psx_ClutIdToVramX(atlasSlot->clutId);
                                        faceData->clutMaxY = Psx_ClutIdToVramY(atlasSlot->clutId);
                                        faceData->tpageMinX = Psx_TPageToVramX(atlasSlot->texturePage);
                                        faceData->tpageMinY = Psx_TPageToVramY(atlasSlot->texturePage);
                                        faceData->tpageMaxX = Psx_TPageToVramX(atlasSlot->texturePage);
                                        faceData->tpageMaxY = Psx_TPageToVramY(atlasSlot->texturePage);
                                        faceData->polygonsBuilt = 1;
                                    }
                                } else {
                                    if (recCursor->u0.f < atlasSlot->uMin) {
                                        atlasSlot->uMinBits = recCursor->u0.bits;
                                    }
                                    if (recCursor->u0.f > atlasSlot->uMax) {
                                        atlasSlot->uMaxBits = recCursor->u0.bits;
                                    }
                                    if (recCursor->v0.f < atlasSlot->vMin) {
                                        atlasSlot->vMinBits = recCursor->v0.bits;
                                    }
                                    if (recCursor->v0.f > atlasSlot->vMax) {
                                        atlasSlot->vMaxBits = recCursor->v0.bits;
                                    }
                                }
                                if (recCursor->u1.f < atlasSlot->uMin) {
                                    atlasSlot->uMinBits = recCursor->u1.bits;
                                }
                                if (recCursor->u1.f > atlasSlot->uMax) {
                                    atlasSlot->uMaxBits = recCursor->u1.bits;
                                }
                                if (recCursor->v1.f < atlasSlot->vMin) {
                                    atlasSlot->vMinBits = recCursor->v1.bits;
                                }
                                if (recCursor->v1.f > atlasSlot->vMax) {
                                    atlasSlot->vMaxBits = recCursor->v1.bits;
                                }
                                if (recCursor->u2.f < atlasSlot->uMin) {
                                    atlasSlot->uMinBits = recCursor->u2.bits;
                                }
                                if (recCursor->u2.f > atlasSlot->uMax) {
                                    atlasSlot->uMaxBits = recCursor->u2.bits;
                                }
                                if (recCursor->v2.f < atlasSlot->vMin) {
                                    atlasSlot->vMinBits = recCursor->v2.bits;
                                }
                                if (recCursor->v2.f > atlasSlot->vMax) {
                                    atlasSlot->vMaxBits = recCursor->v2.bits;
                                }
                                if (curPoly->enabled != 0) {
                                    if (recCursor->u3.f < atlasSlot->uMin) {
                                        atlasSlot->uMinBits = recCursor->u3.bits;
                                    }
                                    if (recCursor->u3.f > atlasSlot->uMax) {
                                        atlasSlot->uMaxBits = recCursor->u3.bits;
                                    }
                                    if (recCursor->v3.f < atlasSlot->vMin) {
                                        atlasSlot->vMinBits = recCursor->v3.bits;
                                    }
                                    if (recCursor->v3.f > atlasSlot->vMax) {
                                        atlasSlot->vMaxBits = recCursor->v3.bits;
                                    }
                                    atlasSlot->state += 2;
                                } else {
                                    (atlasSlot->state)++;
                                }
                            } else {
                                // STRING: FF7 0x008ffd3c
                                OutputDebugStringA("ERROR: LMD TEXTURE PALETTE INDEX \n");
                            }
                            if (desc->useFullTimBounds != 0) {
                                atlasSlot->uMinBits = 0;
                                atlasSlot->vMinBits = 0;
                                atlasSlot->uMax = lmdHeader->width;
                                atlasSlot->vMax = lmdHeader->height;
                            }
                            workAnyFound = 1;
                        }
                        recCursor++;
                        curPoly++;
                    }
                }
            }
        }
        if (goodFlag == 0) {
            switch (faceData->faceKind) {
            case BATTLE3D_LMD_PASS_COLLECT_ATLAS:
                Pfile_Free(pArrays);
                pArrays = 0;
                break;
            case BATTLE3D_LMD_PASS_BUILD_PART:
            case BATTLE3D_LMD_PASS_BUILD_DIRECT:
                Pfile_BuildGroupsFromFaces(0, pVertBase, tmpBlock, totalFaces, (struct PolygonPlyMatState *)polyBasePtr, faceCap, (struct PolygonPlyEntry *)recordArray, (struct PolygonFileArrays *)((void *)pArrays));
            }
        }
    }
    // STRING: FF7 0x008ffd60
    GameHeapFree((void *)recordArray, "C:\\FF7\\src\\battle\\battle3d\\lmd.cpp", 1455);
    // STRING: FF7 0x008ffd84
    GameHeapFree((void *)polyBasePtr, "C:\\FF7\\src\\battle\\battle3d\\lmd.cpp", 1456);
    // STRING: FF7 0x008ffda8
    GameHeapFree((void *)tmpBlock, "C:\\FF7\\src\\battle\\battle3d\\lmd.cpp", 1457);
    // STRING: FF7 0x008ffdcc
    GameHeapFree((void *)pVertBase, "C:\\FF7\\src\\battle\\battle3d\\lmd.cpp", 1458);
    if (goodFlag != 0) {
        Pfile_Free(pArrays);
        pArrays = 0;
    } else {
        Lmd_FinalisePolygonData((void *)pArrays);
    }
    return (int)pArrays;
}

/*
 * Aligns the atlas rectangle to TIM storage, writes its texture, and restores the TIM origin.
 */
// FUNCTION: FF7 0x005e698e
void __cdecl Lmd_BuildSlotTexture(int sliceIndex, Battle3dLmdTextureSlot *slot, PolygonTim *texCtx, Battle3dLmdFaceData *lmdFile, int baseName)
{
    float rectangleWidth;
    int inHPow;
    int pageLeft;
    GfxTexHeader *texHeaderPtr;
    int previousY;
    int uRounded;
    int tpageDeltaX8;
    int originU;
    int packing;
    int pgY;
    int yDelta;
    int deltaClutX;
    int previousX;
    int currRemainder;
    int clutVramY;
    int alignedU;
    int widthPerPage;
    char textureNameChars[200];
    float hF;
    int localPageStepY;
    int paletteX;
    int palRow;
    int pagesAcross;
    int originV;
    int outHeight;

    originU = 0;
    originV = 0;
    alignedU = 0;
    outHeight = 0;
    palRow = 0;
    texCtx->transparentToIndex0 = 1;
    Lmd_MakeSliceTextureName(sliceIndex, (char *)baseName, textureNameChars);
    if (texCtx != 0 && lmdFile->loadOptions != 0) {
        texCtx->flags = lmdFile->loadOptions->extraFlags;
        previousX = texCtx->image->rect.x;
        previousY = texCtx->image->rect.y;
    }
    paletteX = Psx_ClutIdToVramX(slot->clutId);
    clutVramY = Psx_ClutIdToVramY(slot->clutId);
    if (slot->clutId != 0) {
        deltaClutX = paletteX - texCtx->clut->rect.x;
        if (deltaClutX == 0) { /* empty first arm: lib/codegen.h */
        } else {
            if (0, 0) {
                // STRING: FF7 0x008ffdf0
                OutputDebugStringA("LMD: CLUT DELTA X IS NOT ZERO \n");
            }
        }
        if (texCtx->clut->rect.y == lmdFile->clutMinY) {
            palRow = clutVramY - texCtx->clut->rect.y;
        } else {
            palRow = clutVramY - lmdFile->clutMinY;
            palRow = clutVramY - texCtx->clut->rect.y;
            if (0, 0) {
                // STRING: FF7 0x008ffe10
                OutputDebugStringA("LMD: LMD MINIMUM CLUT Y != TIM CLUT Y \n");
            }
        }
        if (palRow < 0) {
            if (0, 0) {
                // STRING: FF7 0x008ffe38
                OutputDebugStringA("LMD: LMD PALETTE INDEX < 0 \n");
            }
            palRow = 0;
        }
    } else {
        if (0, 0) {
            // STRING: FF7 0x008ffe58
            OutputDebugStringA("LMD: CLUT ID IS ZERO \n");
        }
        palRow = 0;
    }
    pageLeft = Psx_TPageToVramX(slot->texturePage);
    pgY = Psx_TPageToVramY(slot->texturePage);
    tpageDeltaX8 = pageLeft - texCtx->image->rect.x;
    if (tpageDeltaX8 == 0) { /* empty first arm: lib/codegen.h */
    } else {
        texCtx->image->rect.x = pageLeft;
        tpageDeltaX8 = 0;
    }
    yDelta = pgY - texCtx->image->rect.y;
    if (yDelta == 0) { /* empty first arm: lib/codegen.h */
    } else {
        texCtx->image->rect.y = pgY;
        yDelta = 0;
    }
    packing = Psx_ColorModeToPixelsPerWord(Psx_TPageToColorMode(slot->texturePage));
    slot->slotIndex = sliceIndex;
    slot->atlasUBits = slot->uMinBits;
    slot->atlasVBits = slot->vMinBits;
    rectangleWidth = slot->uMax - slot->uMin;
    hF = slot->vMax - slot->vMin;
    slot->uSize = rectangleWidth;
    slot->vSize = hF;
    if (IsPowerOfTwo((int)rectangleWidth) != 0) {
        slot->uSizePow2 = rectangleWidth;
    } else {
        uRounded = RoundUpToPowerOfTwo((int)rectangleWidth);
        slot->uSizePow2 = uRounded;
    }
    if (IsPowerOfTwo((int)hF) != 0) {
        slot->vSizePow2 = hF;
    } else {
        inHPow = RoundUpToPowerOfTwo((int)hF);
        slot->vSizePow2 = inHPow;
    }
    pagesAcross = 0;
    localPageStepY = 0;
    widthPerPage = 0;
    switch (Psx_TPageToColorMode(slot->texturePage)) {
    case GFX_PSX_COLOR_MODE_4BIT:
        widthPerPage = 0x40;
        break;
    case GFX_PSX_COLOR_MODE_8BIT:
        widthPerPage = 0x80;
        break;
    case GFX_PSX_COLOR_MODE_16BIT:
        widthPerPage = 0x100;
    }
    if (lmdFile->tpageMinX == texCtx->image->rect.x) { /* empty first arm: lib/codegen.h */
    } else {
        tpageDeltaX8 = pageLeft - lmdFile->tpageMinX;
        if (0, 0) {
            // STRING: FF7 0x008ffe70
            OutputDebugStringA("LMD: TEXTURE PAGE MINIMUM X != TIM TEXTURE PAGE MINIMUM X \n");
        }
    }
    if (lmdFile->tpageMinY == texCtx->image->rect.y) { /* empty first arm: lib/codegen.h */
    } else {
        yDelta = pgY - lmdFile->tpageMinY;
        if (0, 0) {
            // STRING: FF7 0x008ffeac
            OutputDebugStringA("LMD: TEXTURE PAGE MINIMUM Y != TIM TEXTURE PAGE MINIMUM Y \n");
        }
    }
    pagesAcross = tpageDeltaX8 / widthPerPage;
    localPageStepY = yDelta / 256;
    originU = pagesAcross * 256.0f + slot->atlasU;
    originV = localPageStepY * 256.0f + slot->atlasV;
    alignedU = rectangleWidth;
    outHeight = hF;
    if (originU > texCtx->width) {
        originU = 0;
    }
    if (originV > texCtx->height) {
        originV = 0;
    }
    switch (Psx_TPageToColorMode(slot->texturePage)) {
    case GFX_PSX_COLOR_MODE_4BIT:
        currRemainder = originU & 3;
        if (currRemainder != 0) {
            originU -= currRemainder;
            alignedU += currRemainder;
        }
        currRemainder = alignedU & 3;
        if (currRemainder != 0) {
            alignedU += 4 - currRemainder;
        }
        break;
    case GFX_PSX_COLOR_MODE_8BIT:
        currRemainder = originU & 1;
        if (currRemainder != 0) {
            originU -= currRemainder;
            alignedU += currRemainder;
        }
        currRemainder = alignedU & 1;
        if (currRemainder != 0) {
            alignedU += 2 - currRemainder;
        }
    }
    slot->atlasU = originU;
    slot->atlasV = originV;
    if (IsPowerOfTwo(alignedU) != 0) {
        slot->uSizePow2 = alignedU;
    } else {
        slot->uSizePow2 = RoundUpToPowerOfTwo(alignedU);
    }
    if (IsPowerOfTwo(outHeight) != 0) {
        slot->vSizePow2 = outHeight;
    } else {
        slot->vSizePow2 = RoundUpToPowerOfTwo(outHeight);
    }
    texCtx->singlePalette = 1;
    texCtx->paletteRow = palRow;
    texHeaderPtr = (GfxTexHeader *)Pfile_BuildTextureFromTimRect(originU, originV, alignedU, outHeight, texCtx, textureNameChars);
    if (texHeaderPtr != 0) {
        texHeaderPtr->modeFlag = 1;
        Gfx_WriteTexFile((char *)&textureNameChars, texHeaderPtr);
        Gfx_DestroyTexture(texHeaderPtr);
    }
    if (texCtx != 0) {
        texCtx->image->rect.x = previousX;
        texCtx->image->rect.y = previousY;
    }
}

/*
 * Builds each list-backed atlas slot; the original retains disabled diagnostics.
 */
// FUNCTION: FF7 0x005e6fe8
void __cdecl Lmd_BuildAtlasTextures(Battle3dLmdFaceData *lmdFile, struct PolygonTim *texCtx)
{
    int entryIndex;
    int listCount;
    char debugText[200];
    Battle3dLmdTextureSlot *pSlotRecord;

    listCount = ListGetCount(lmdFile->slotList);
    if (0, 0) {
        // STRING: FF7 0x008ffee8
        sprintf(debugText, "LMD: FILE %s \n", lmdFile->fileName);
        OutputDebugStringA(debugText);
        // STRING: FF7 0x008ffef8
        sprintf(debugText, "CLUT X: %d %d CLUT Y: %d %d TX: %d %d TY: %d %d \n", lmdFile->clutMinX, lmdFile->clutMaxX, lmdFile->clutMinY, lmdFile->clutMaxY, lmdFile->tpageMinX, lmdFile->tpageMaxX, lmdFile->tpageMinY, lmdFile->tpageMaxY);
        OutputDebugStringA(debugText);
    }
    for (entryIndex = 0; entryIndex < listCount; entryIndex++) {
        pSlotRecord = &lmdFile->slots[entryIndex];
        if (lmdFile->allowLargeAtlas != 0 && entryIndex == 0 && pSlotRecord->uMax - pSlotRecord->uMin > 128.0f && pSlotRecord->vMax - pSlotRecord->vMin > 128.0f) {
            pSlotRecord->uMinBits = 0;
            pSlotRecord->uMaxBits = FLOAT_255_BITS;
            pSlotRecord->vMinBits = 0;
            pSlotRecord->vMaxBits = FLOAT_255_BITS;
        }
        if (0, 0) {
            // STRING: FF7 0x008fff2c
            sprintf(debugText, "%d P %d U %.0f %.0f V %.0f %.0f CX: %d CY %d T: %d 0x%X TX: %d TY: %d PF: %d \n", entryIndex, pSlotRecord->state, pSlotRecord->uMin, pSlotRecord->uMax, pSlotRecord->vMin, pSlotRecord->vMax, Psx_ClutIdToVramX(pSlotRecord->clutId), Psx_ClutIdToVramY(pSlotRecord->clutId), pSlotRecord->texturePage, pSlotRecord->texturePage, Psx_TPageToVramX(pSlotRecord->texturePage), Psx_TPageToVramY(pSlotRecord->texturePage), Psx_TPageToColorMode(pSlotRecord->texturePage));
            OutputDebugStringA(debugText);
        }
        Lmd_BuildSlotTexture(entryIndex, pSlotRecord, texCtx, lmdFile, lmdFile->fileName);
    }
}

/*
 * Loads SWORD.LMD, exercises polygon conversion, and releases its temporary data.
 */
// FUNCTION: FF7 0x005e7249
void __cdecl Lmd_DebugLoadSwordTest(void)
{
    char *modelName;
    PolygonFileArrays *polyData;
    PolygonTim *timObject;
    Battle3dPartyLoadDesc scratchBuf;
    Battle3dLmdFaceData *pAtlas;
    int byteCnt;
    Battle3dPartyLoadDesc *loadDescPtr;
    int fileData;

    loadDescPtr = &scratchBuf;
    Battle_InitLmdLoadDesc((int *)loadDescPtr);
    timObject = 0;
    // STRING: FF7 0x008fff7c
    modelName = "SWORD.LMD";
    fileData = (int)LoadDiskFile(&byteCnt, modelName);
    if (fileData != 0) {
        pAtlas = (Battle3dLmdFaceData *)(Lmd_CreateTextureAtlas(2, timObject));
        if (pAtlas != 0) {
            polyData = (PolygonFileArrays *)Battle_BuildPolygonDataFromLmd(loadDescPtr, fileData, pAtlas, timObject);
            if (polyData != 0) {
                Lmd_FinalisePolygonData((void *)polyData);
                Pfile_Free(polyData);
            }
            Lmd_DestroyTextureAtlas(pAtlas);
        }
        // STRING: FF7 0x008fff88
        GameHeapFree((void *)fileData, "C:\\FF7\\src\\battle\\battle3d\\lmd.cpp", 1924);
    }
}

/*
 * Builds a battle model's part file name: the 'P' indexed form of MakeIndexedAssetName.
 */
// FUNCTION: FF7 0x005e72fe
void __cdecl BattleModel_MakePartName(int index, char *modelName, char *outPath)
{
    MakeIndexedAssetName(0x50, index, modelName, outPath);
}

/*
 * Builds a battle model's weapon file name: the 'W' indexed form of MakeIndexedAssetName.
 */
// FUNCTION: FF7 0x005e7319
void __cdecl BattleModel_MakeWeaponName(int index, char *modelName, char *outPath)
{
    MakeIndexedAssetName(0x57, index, modelName, outPath);
}

/*
 * Loads indexed textures and wraps the supplied polygon data in an RSD model record.
 */
// FUNCTION: FF7 0x005e7334
int __cdecl BattleModel_BuildRsdSubstitute(Battle3dPartyLoadDesc *loadDesc, int count, int baseName, PfileLoadOptions *loader, PolygonFileArrays *polyArrays, PolygonTim *unused)
{
    int idx;
    int failed;
    PolygonRsdRecord *rsd;
    int savedVal;
    LmdTextureResult copy;
    char texName[200];
    LmdTextureResult rsdResult;
    LmdTextureResult retBuffer;

    failed = 1;
    // STRING: FF7 0x008fffac
    rsd = (PolygonRsdRecord *)GameHeapAlloc(1, 36, "C:\\FF7\\src\\battle\\battle3d\\lmd.cpp", 1952);
    if (rsd != 0) {
        loadDesc->subCount = count;
        rsd->count = count;
        if (rsd->count > 0) {
            // STRING: FF7 0x008fffd0
            rsd->block = (int)GameHeapAlloc(rsd->count, 4, "C:\\FF7\\src\\battle\\battle3d\\lmd.cpp", 1964);
            // STRING: FF7 0x008ffff4
            rsd->texRecordTable = (int)GameHeapAlloc(rsd->count, 4, "C:\\FF7\\src\\battle\\battle3d\\lmd.cpp", 1965);
            // STRING: FF7 0x00900018
            rsd->textures = (int)GameHeapAlloc(rsd->count, 4, "C:\\FF7\\src\\battle\\battle3d\\lmd.cpp", 1966);
            if (rsd->block != 0 && rsd->texRecordTable != 0 && rsd->textures != 0) {
                savedVal = loader->sourceMode;
                loader->sourceMode = 0;
                for (idx = 0; idx < rsd->count; idx++) {
                    Lmd_MakeSliceTextureName(idx, (char *)baseName, texName);
                    loader->flags |= 0x200;
                    // Preserve the original return-buffer copy and separate result assignment.
                    rsdResult = *Pfile_LoadRsdTexture(&retBuffer, (int)loader, &texName);
                    copy = rsdResult;
                    if (copy.status == 0) {
                        ((int *)(rsd->textures))[idx] = copy.texture;
                    }
                }
                loader->sourceMode = savedVal;
            }
        }
        rsd->meshCount = 1;
        // STRING: FF7 0x0090003c
        rsd->meshes = (int)GameHeapAlloc(rsd->meshCount, 4, "C:\\FF7\\src\\battle\\battle3d\\lmd.cpp", 1998);
        if (rsd->meshes != 0) {
            *(int *)rsd->meshes = (int)polyArrays;
            failed = 0;
        }
    }
    if (failed != 0) {
        // STRING: FF7 0x00900060
        GameHeapFree((void *)rsd, "C:\\FF7\\src\\battle\\battle3d\\lmd.cpp", 2008);
        rsd = 0;
    }
    return (int)rsd;
}
#pragma intrinsic(memset)

/*
 * Builds RSD substitute from reordered arguments; when result is nonzero loads model with
 * arguments loader, baseName, context and returns loaded pointer. Otherwise returns null; reserved unused.
 */
// FUNCTION: FF7 0x005e754a
void *__cdecl BattleModel_LoadPartMesh(Battle3dPartyLoadDesc *loadDesc, int count, int baseName, PolygonFileArrays *polyArrays, PolygonTim *unused, struct PfileLoadOptions *loader, Battle3dLmdFaceData *reserved, GfxEngineContext *context)
{
    PolygonRsdRecord *pRsdRecord;
    PolygonRsdModel *loaded;

    loaded = 0;
    pRsdRecord = (PolygonRsdRecord *)BattleModel_BuildRsdSubstitute(loadDesc, count, baseName, loader, polyArrays, unused);
    if (pRsdRecord != 0) {
        loaded = (PolygonRsdModel *)(Pfile_LoadRsdModelEx(pRsdRecord, loader, baseName, context));
    }
    return loaded;
}

/*
 * Builds and registers an LMD part mesh with the model manager.
 */
// FUNCTION: FF7 0x005e75a2
void *__cdecl Lmd_BuildAndRegisterPart(Battle3dPartyLoadDesc *loadDesc, int partIndex, PolygonTim *tim, void *loader, Battle3dLmdFaceData *lmdFile, char *baseName)
{
    int result;
    int partName[50];
    PolygonFileArrays *polygonDataPtr;

    result = 0;
    polygonDataPtr = (PolygonFileArrays *)Battle_BuildPolygonDataFromLmd(loadDesc, partIndex, lmdFile, tim);
    if (polygonDataPtr != 0) {
        if (loadDesc->secondCount != 0) {
            if (loadDesc->loaded != 0) {
                BattleModel_MakeWeaponName(loadDesc->nameId, (char *)baseName, (char *)partName);
            } else {
                BattleModel_MakePartName(loadDesc->nameId, (char *)baseName, (char *)partName);
            }
            Pfile_WriteBinary((int)partName, polygonDataPtr);
        }
        result = (int)BattleModel_LoadPartMesh(loadDesc, ListGetCount(lmdFile->slotList), (int)baseName, polygonDataPtr, tim, (struct PfileLoadOptions *)loader, lmdFile, GetEngineContext());
    }
    return (void *)result;
}
} /* extern "C" */
