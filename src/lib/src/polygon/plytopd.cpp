// src/lib/src/polygon/plytopd.cpp
// Original: C:\lib\src\polygon\plytopd.cpp
// Contents: Converts PLY and MAT text models into polygon data and loads animation and skeleton
//   data, with their RSD textures, from text or binary sources. Small token and colour parsing
//   helpers support the conversion.
/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/types/file.h"
#include "lib/enums/file.h"
#include "lib/types/graphics.h"
#include "lib/types/polygon.h"
#include "lib/pfile.h"
#include "lib/types/list.h"
#include "lib/types/mem.h"
#include "lib/codegen.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "lib/crt.h"
#include "lib/api/file.h"
#include "lib/api/graphics.h"
#include "lib/api/list.h"
#include "lib/api/mem.h"
#include "lib/api/polygon.h"
#include "lib/api/token.h"
#include "lib/enums/graphics.h"
extern "C" {

struct PolygonPlacement;
struct PolygonTexturePair;
struct FileOpenRequest;
struct PolygonAnimation;
struct PolygonRsdRecord;

/* Explicit prototype emits retail CRT call rather than <ctype.h>'s _pctype table macro. */
extern int __cdecl isspace(int);
__declspec(dllimport) void __stdcall OutputDebugStringA(const char *);
// clang-format off
// GLOBAL: FF7 0x00910b90
struct PolygonQuadTriIndex g_PlytopdQuadTriIndices[2] = {{0, 1, 2}, {1, 3, 2}};
// clang-format on
// plytopd.cpp's literal stream in retail order: four code literals of late functions ("A", "P", "RSD", "TEX")
// and the literals of the functions up to Anim_ScaleRootTranslation (0x00910BF0..0x00910E6B) precede the keyword
// table's strings; each is returned by a static __inline function ahead of the table, and under /Ob1 the call
// expands to the plain operand.
static __inline char *PlytopdLit_A(void)
{
    // STRING: FF7 0x00910bf0
    return "A";
}
static __inline char *PlytopdLit_P(void)
{
    // STRING: FF7 0x00910bf4
    return "P";
}
static __inline char *PlytopdLit_Rsd(void)
{
    // STRING: FF7 0x00910bf8
    return "RSD";
}
static __inline char *PlytopdLit_Tex(void)
{
    // STRING: FF7 0x00910bfc
    return "TEX";
}
static __inline char *PlytopdLit_TimXU(void)
{
    // STRING: FF7 0x00910c00
    return "TIM X %d > U %.3f  \n";
}
static __inline char *PlytopdLit_TimYV(void)
{
    // STRING: FF7 0x00910c18
    return "TIM Y %d > V %.3f \n";
}
static __inline char *PlytopdLit_SourcePath_1(void)
{
    // STRING: FF7 0x00910c2c
    return "C:\\lib\\src\\polygon\\plytopd.cpp";
}
static __inline char *PlytopdLit_SourcePath_2(void)
{
    // STRING: FF7 0x00910c4c
    return "C:\\lib\\src\\polygon\\plytopd.cpp";
}
static __inline char *PlytopdLit_SourcePath_3(void)
{
    // STRING: FF7 0x00910c6c
    return "C:\\lib\\src\\polygon\\plytopd.cpp";
}
static __inline char *PlytopdLit_SourcePath_4(void)
{
    // STRING: FF7 0x00910c8c
    return "C:\\lib\\src\\polygon\\plytopd.cpp";
}
static __inline char *PlytopdLit_SourcePath_5(void)
{
    // STRING: FF7 0x00910cac
    return "C:\\lib\\src\\polygon\\plytopd.cpp";
}
static __inline char *PlytopdLit_SourcePath_6(void)
{
    // STRING: FF7 0x00910ccc
    return "C:\\lib\\src\\polygon\\plytopd.cpp";
}
static __inline char *PlytopdLit_SourcePath_7(void)
{
    // STRING: FF7 0x00910cec
    return "C:\\lib\\src\\polygon\\plytopd.cpp";
}
static __inline char *PlytopdLit_SourcePath_8(void)
{
    // STRING: FF7 0x00910d0c
    return "C:\\lib\\src\\polygon\\plytopd.cpp";
}
static __inline char *PlytopdLit_SourcePath_9(void)
{
    // STRING: FF7 0x00910d2c
    return "C:\\lib\\src\\polygon\\plytopd.cpp";
}
static __inline char *PlytopdLit_SourcePath_10(void)
{
    // STRING: FF7 0x00910d4c
    return "C:\\lib\\src\\polygon\\plytopd.cpp";
}
static __inline char *PlytopdLit_SourcePath_11(void)
{
    // STRING: FF7 0x00910d6c
    return "C:\\lib\\src\\polygon\\plytopd.cpp";
}
static __inline char *PlytopdLit_SourcePath_12(void)
{
    // STRING: FF7 0x00910d8c
    return "C:\\lib\\src\\polygon\\plytopd.cpp";
}
static __inline char *PlytopdLit_SourcePath_13(void)
{
    // STRING: FF7 0x00910dac
    return "C:\\lib\\src\\polygon\\plytopd.cpp";
}
static __inline char *PlytopdLit_SourcePath_14(void)
{
    // STRING: FF7 0x00910dcc
    return "C:\\lib\\src\\polygon\\plytopd.cpp";
}
static __inline char *PlytopdLit_SourcePath_15(void)
{
    // STRING: FF7 0x00910dec
    return "C:\\lib\\src\\polygon\\plytopd.cpp";
}
static __inline char *PlytopdLit_SourcePath_16(void)
{
    // STRING: FF7 0x00910e0c
    return "C:\\lib\\src\\polygon\\plytopd.cpp";
}
static __inline char *PlytopdLit_SourcePath_17(void)
{
    // STRING: FF7 0x00910e2c
    return "C:\\lib\\src\\polygon\\plytopd.cpp";
}
static __inline char *PlytopdLit_SourcePath_18(void)
{
    // STRING: FF7 0x00910e4c
    return "C:\\lib\\src\\polygon\\plytopd.cpp";
}
// The keywords of the HRC/animation text files, indexed by AnimText_LookupKeyword (q = 1..15;
// slot 0 is an empty string in this TU's .bss and is never matched). Retail has the strings at
// 0x00910E6C.., after the helpers' literals above.
// GLOBAL: FF7 0x00910ba8
char *g_AnimTextKeywords[16] = {
    g_PlytopdEmptyKeyword,
    ":HEADER_BLOCK",
    ":SCRIPT",
    ":SKELETON",
    ":FRAMES",
    ":CHANNELS",
    ":ROTATION_ORDER",
    ":ROTATE_RESOLUTION",
    ":BONES",
    "root_coordinate",
    ":TX",
    ":TY",
    ":TZ",
    ":RX",
    ":RY",
    ":RZ",
};
// The parent name of the skeleton's root bones: Skel_LoadHrc and Skel_BuildHierarchyAndLoadParts
// compare each bone's parent with it (one object for three readers in two functions; three
// literals would be three copies without /Gf).
// GLOBAL: FF7 0x00910be8
char g_HrcRootName[] = "root";

typedef struct PolygonTimColorWord {
    int packed;
} PolygonTimColorWord;

#pragma intrinsic(strcmp)
#pragma intrinsic(memcpy)
#pragma intrinsic(memset)
__declspec(dllimport) void __stdcall OutputDebugStringA(const char *arg1);
DxTextureResult __cdecl Pfile_LoadRsdTexture(PfileLoadOptions *options, char *fileName);
void __cdecl AnimPlacement_SetScale(int scale, struct PolygonPlacement *placement);

/* Retail caller passes 32-bit packed ARGB word matching GfxArgb32 by-value ABI. */
/*
 * Returns true if input integer is a power of two.
 */
// FUNCTION: FF7 0x00690240
int __cdecl IsPowerOfTwo(int value)
{
    int result;
    int bitCount;

    result = 0;
    bitCount = CountSetBits32(value);
    if (bitCount == 1) {
        result = 1;
    }
    return result;
}

/*
 * Rounds positive integer up to next power of two.
 */
// FUNCTION: FF7 0x00690270
int __cdecl RoundUpToPowerOfTwo(int value)
{
    int result;
    int bitIdx;
    unsigned int mask;
    result = 1;
    if (value > 0) {
        mask = 0x80000000;
        for (bitIdx = 0; bitIdx < 32; bitIdx++) {
            if (value & mask) {
                result = mask << 1;
                break;
            }
            mask >>= 1;
        }
    }
    return result;
}

/*
 * Strips hash comments from token text buffer.
 */
// FUNCTION: FF7 0x006902c7
void __cdecl Token_StripHashComments(int length, char *buffer)
{
    char hashChar;
    int pos;

    hashChar = '#';
    pos = 0;
    while (pos < length) {
        if (buffer[pos] == hashChar) {
            while (pos < length) {
                if (isspace(buffer[pos]) != 0) {
                    if (buffer[pos] == '\r' || (buffer[pos] == '\n' || buffer[pos] == '\n')) {
                        break;
                    }
                    pos++;
                } else {
                    buffer[pos] = 0;
                    pos++;
                }
            }
        } else {
            pos++;
        }
    }
}

/*
 * Parses three color tokens and converts to BGRA color format.
 */
// FUNCTION: FF7 0x00690372
int __cdecl Pfile_ParseColorTokensToBgra(PolygonBgraColor *color, int index, char **tokens)
{
    int token;

    token = (int)tokens[index];
    color->channel2 = StringToInt(token);
    index++;
    token = (int)tokens[index];
    color->channel1 = StringToInt(token);
    index++;
    token = (int)tokens[index];
    color->channel0 = StringToInt(token);
    index++;
    token = (int)tokens[index];
    color->alpha = 0xFF;
    return index;
}

/*
 * Parses two UV coordinate tokens divided by 256.
 */
// FUNCTION: FF7 0x00690404
int __cdecl Pfile_ParseUvTokensDiv256(float *output, int index, char **tokens)
{
    char *rec;

    rec = tokens[index];
    output[0] = atof(rec) / 256.0;
    index++;
    rec = tokens[index];
    output[1] = atof(rec) / 256.0;
    index++;
    return index;
}

/*
 * Converts TIM pixel coordinates to normalized UV texture coordinates.
 */
// FUNCTION: FF7 0x00690468
int __cdecl Pfile_TimPixelToUv(int unused, float *uv, int index, char **tokens,
                               int width, int height, int textureWidth, int textureHeight)
{
    struct {
        int debug;
        char *token;
        float texV, texU;
        int pixelY;
        char message[200];
        int pixelX;
    } f; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    f.debug = 0;
    f.token = tokens[index];
    f.pixelX = atoi(f.token);
    if (f.pixelX >= 0) {
        if (f.pixelX == 0)
            f.pixelX = 1;
        if (f.pixelX == 255)
            f.pixelX = 254;
    }
    f.texU = (float)f.pixelX;
    if (f.texU < width && f.debug) {
        sprintf(f.message,
                PlytopdLit_TimXU(), width, f.texU);
        Gfx_DebugPrint(f.message);
    }
    uv[0] = f.texU / textureWidth;
    index++;
    f.token = tokens[index];
    f.pixelY = atoi(f.token);
    if (f.pixelY >= 0) {
        if (f.pixelY == 0)
            f.pixelY = 1;
        if (f.pixelY == 255)
            f.pixelY = 254;
    }
    f.texV = (float)f.pixelY;
    if (f.texV < height && f.debug) {
        sprintf(f.message,
                PlytopdLit_TimYV(), height, f.texV);
        Gfx_DebugPrint(f.message);
    }
    uv[1] = f.texV / textureHeight;
    index++;
    return index;
}

/*
 * Emits three vertices and polygon face into destination geometry buffer.
 */
// FUNCTION: FF7 0x006905f8
void __cdecl Pfile_EmitTriangle(int triIndex, int quadIndex, PolygonTranslation *vertices, int unused, PolygonPlyEntry *polygon, PolygonPlyMatState *mesh, PolygonFileArrays *geometry)
{
    struct {
        GfxPrimitiveMeshVertex *meshVert;
        int cornerIdx;
        PolygonBgraColor colorVal;
        int vertIndex;
        PolygonTranslation *posPtr;
        int baseIndex;
        PolygonTexCoord uvVal;
        PolygonBgraColor *colorPtr;
        PolygonTriangle *triPtr;
        PolygonTexCoord *uvPtr;
        PolygonTranslation posVal;
        PolygonTriangle triVal;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    frame.posPtr = &frame.posVal;
    frame.meshVert = 0;
    for (frame.cornerIdx = 0; frame.cornerIdx < 3; frame.cornerIdx++) {
        frame.uvPtr = 0;
        frame.colorPtr = 0;
        switch (polygon->kind) {
        case PFILE_FACE_FLAT:
            frame.colorPtr = &frame.colorVal;
            *frame.colorPtr = polygon->colors[g_PlytopdQuadTriIndices[quadIndex].vertex[0]];
            break;
        case PFILE_FACE_GOURAUD:
            frame.colorPtr = &frame.colorVal;
            *frame.colorPtr = polygon->colors[g_PlytopdQuadTriIndices[quadIndex].vertex[frame.cornerIdx]];
            break;
        case PFILE_FACE_TEXTURED:
            frame.colorPtr = &frame.colorVal;
            frame.colorPtr->packed = -1;
            frame.uvPtr = &frame.uvVal;
            *frame.uvPtr = polygon->texCoords[g_PlytopdQuadTriIndices[quadIndex].vertex[frame.cornerIdx]];
            break;
        case PFILE_FACE_TEXTURED_FLAT:
            frame.colorPtr = &frame.colorVal;
            *frame.colorPtr = polygon->colors[g_PlytopdQuadTriIndices[quadIndex].vertex[0]];
            frame.uvPtr = &frame.uvVal;
            *frame.uvPtr = polygon->texCoords[g_PlytopdQuadTriIndices[quadIndex].vertex[frame.cornerIdx]];
            break;
        case PFILE_FACE_TEXTURED_GOURAUD:
            frame.colorPtr = &frame.colorVal;
            *frame.colorPtr = polygon->colors[g_PlytopdQuadTriIndices[quadIndex].vertex[frame.cornerIdx]];
            frame.uvPtr = &frame.uvVal;
            *frame.uvPtr = polygon->texCoords[g_PlytopdQuadTriIndices[quadIndex].vertex[frame.cornerIdx]];
        }
        frame.vertIndex = mesh->vertex[g_PlytopdQuadTriIndices[quadIndex].vertex[frame.cornerIdx]];
        *frame.posPtr = vertices[frame.vertIndex];
        Pfile_AddVertex(frame.vertIndex, (int *)frame.colorPtr, (void *)frame.uvPtr, frame.meshVert, (void *)frame.posPtr, geometry);
    }
    frame.baseIndex = triIndex * 3;
    frame.triPtr = &frame.triVal;
    frame.triPtr->initZero_00 = 0;
    frame.triPtr->vertexA = frame.baseIndex;
    frame.triPtr->vertexB = frame.baseIndex + 1;
    frame.triPtr->vertexC = frame.baseIndex + 2;
    frame.triPtr->meshVertexA = mesh->normalShort[g_PlytopdQuadTriIndices[quadIndex].vertex[0]].value;
    frame.triPtr->meshVertexB = mesh->normalShort[g_PlytopdQuadTriIndices[quadIndex].vertex[1]].value;
    frame.triPtr->meshVertexC = mesh->normalShort[g_PlytopdQuadTriIndices[quadIndex].vertex[2]].value;
    Pfile_AddPolygon((void *)frame.triPtr, geometry);
}

/*
 * Parses geometry from a PLY file and materials from a MAT file.
 */
// FUNCTION: FF7 0x0069087e
void __cdecl Pfile_ParsePlyAndMat(int texPage, int textured, float scale, PolygonRsdRecord *rsdRecord, PolygonFileArrays *fileArrays, PolygonTokenFile *plyTokens, PolygonTokenFile *matTokens)
{
    PolygonPlyEntry *entries;
    int faceCount;
    int tokenIndex;
    PolygonTranslation *pointData;
    PolygonTranslation *vertCursor;
    int firstIdx;
    int indexList8;
    int transType;
    int faceFlags;
    int vramPositionX;
    int sprHeight;
    int numVerts;
    PolygonPlyEntry *curEntry;
    int stepIdx;
    PolygonTranslation *verticesPtr;
    int typeCode;
    int entryIndex;
    int offset;
    int sepPos;
    int originY;
    int mainPageY;
    int srcX;
    PolygonPlyMatState *pMatStates;
    int maxCount;
    int dashIdx;
    PolygonTranslation *destinationPoint;
    char *token;
    int textureWidth;
    int regionX;
    int upperBound;
    int h;
    PolygonTranslation *vertexPtr;
    int matCnt;
    PolygonTexRecord *texRecord;
    int pageW;
    int formKind;
    int numFaceVerts;
    int normalIndex;
    PolygonPageEntry *entRecord;
    int spriteWidth;
    PolygonPlyMatState *matPosition;
    int characterCount;
    int srcY;
    int regionH;
    PolygonSpriteItem *workRsdItemPtr;
    int pointCount;

    texPage = 1;
    tokenIndex = 1;
    token = plyTokens->tokens[tokenIndex];
    pointCount = atoi(token);
    tokenIndex = 2;
    token = plyTokens->tokens[tokenIndex];
    numVerts = atoi(token);
    tokenIndex = 3;
    token = plyTokens->tokens[tokenIndex];
    matCnt = atoi(token);
    tokenIndex = 4;
    pointData = 0;
    if (pointCount > 0) {
        pointData = (PolygonTranslation *)GameHeapAllocRaw(pointCount * 0xC, PlytopdLit_SourcePath_1(), 0x1C4);
        destinationPoint = pointData;
        if (pointData != 0) {
            for (stepIdx = 0; stepIdx < pointCount; stepIdx++) {
                token = plyTokens->tokens[tokenIndex];
                destinationPoint->x = StringToFloat(token) * scale;
                tokenIndex++;
                token = plyTokens->tokens[tokenIndex];
                destinationPoint->y = StringToFloat(token) * scale;
                tokenIndex++;
                token = plyTokens->tokens[tokenIndex];
                destinationPoint->z = StringToFloat(token) * scale;
                tokenIndex++;
                destinationPoint++;
            }
        }
    }
    verticesPtr = 0;
    if (numVerts > 0) {
        verticesPtr = (PolygonTranslation *)GameHeapAllocRaw(numVerts * 0xC, PlytopdLit_SourcePath_2(), 0x1E0);
        vertCursor = verticesPtr;
        if (verticesPtr != 0) {
            for (stepIdx = 0; stepIdx < numVerts; stepIdx++) {
                token = plyTokens->tokens[tokenIndex];
                vertCursor->x = StringToFloat(token);
                tokenIndex++;
                token = plyTokens->tokens[tokenIndex];
                vertCursor->y = StringToFloat(token);
                tokenIndex++;
                token = plyTokens->tokens[tokenIndex];
                vertCursor->z = StringToFloat(token);
                tokenIndex++;
                vertCursor++;
            }
        }
    }
    pMatStates = 0;
    if (matCnt > 0) {
        pMatStates = (PolygonPlyMatState *)GameHeapAlloc(matCnt, 0x38, PlytopdLit_SourcePath_3(), 0x1FC);
        matPosition = pMatStates;
        if (pMatStates != 0) {
            for (stepIdx = 0; stepIdx < matCnt; stepIdx++) {
                token = plyTokens->tokens[tokenIndex];
                typeCode = atoi(token);
                tokenIndex++;
                matPosition->isQuad = 0;
                switch (typeCode) {
                case PFILE_PLY_QUAD:
                    matPosition->isQuad = 1;
                case PFILE_PLY_TRIANGLE:
                    matPosition->isPolygon = 1;
                    token = plyTokens->tokens[tokenIndex];
                    matPosition->vertex[0] = atoi(token);
                    tokenIndex++;
                    token = plyTokens->tokens[tokenIndex];
                    matPosition->vertex[1] = atoi(token);
                    tokenIndex++;
                    token = plyTokens->tokens[tokenIndex];
                    matPosition->vertex[2] = atoi(token);
                    tokenIndex++;
                    token = plyTokens->tokens[tokenIndex];
                    matPosition->vertex[3] = atoi(token);
                    tokenIndex++;
                    token = plyTokens->tokens[tokenIndex];
                    matPosition->normal[0] = atoi(token);
                    tokenIndex++;
                    token = plyTokens->tokens[tokenIndex];
                    matPosition->normal[1] = atoi(token);
                    tokenIndex++;
                    token = plyTokens->tokens[tokenIndex];
                    matPosition->normal[2] = atoi(token);
                    tokenIndex++;
                    token = plyTokens->tokens[tokenIndex];
                    matPosition->normal[3] = atoi(token);
                    tokenIndex++;
                    break;
                case PFILE_PLY_LINE:
                    matPosition->isLine = 1;
                    token = plyTokens->tokens[tokenIndex];
                    matPosition->vertex[0] = atoi(token);
                    tokenIndex++;
                    token = plyTokens->tokens[tokenIndex];
                    matPosition->vertex[1] = atoi(token);
                    tokenIndex++;
                    token = plyTokens->tokens[tokenIndex];
                    tokenIndex++;
                    token = plyTokens->tokens[tokenIndex];
                    tokenIndex++;
                    token = plyTokens->tokens[tokenIndex];
                    tokenIndex++;
                    token = plyTokens->tokens[tokenIndex];
                    tokenIndex++;
                    token = plyTokens->tokens[tokenIndex];
                    tokenIndex++;
                    token = plyTokens->tokens[tokenIndex];
                    tokenIndex++;
                    break;
                case PFILE_PLY_SPRITE:
                    matPosition->isSprite = 1;
                    token = plyTokens->tokens[tokenIndex];
                    matPosition->vertex[0] = atoi(token);
                    tokenIndex++;
                    token = plyTokens->tokens[tokenIndex];
                    matPosition->spriteWidth = atoi(token);
                    tokenIndex++;
                    token = plyTokens->tokens[tokenIndex];
                    matPosition->spriteHeight = atoi(token);
                    tokenIndex++;
                    token = plyTokens->tokens[tokenIndex];
                    tokenIndex++;
                    token = plyTokens->tokens[tokenIndex];
                    tokenIndex++;
                    token = plyTokens->tokens[tokenIndex];
                    tokenIndex++;
                    token = plyTokens->tokens[tokenIndex];
                    tokenIndex++;
                    token = plyTokens->tokens[tokenIndex];
                    tokenIndex++;
                }
                matPosition++;
            }
        }
    }
    tokenIndex = 1;
    token = matTokens->tokens[tokenIndex];
    faceCount = atoi(token);
    tokenIndex = 2;
    entries = 0;
    if (faceCount > 0) {
        if (matCnt > faceCount) {
            maxCount = matCnt;
        } else {
            maxCount = faceCount;
        }
        entries = (PolygonPlyEntry *)GameHeapAlloc(maxCount, 0x58, PlytopdLit_SourcePath_4(), 0x297);
        curEntry = entries;
        if (entries != 0) {
            for (entryIndex = 0; entryIndex < faceCount; entryIndex++) {
                token = matTokens->tokens[tokenIndex];
                tokenIndex++;
                characterCount = CountCharInString(',', token);
                if (characterCount > 0) {
                    formKind = PFILE_MAT_FACES_LIST;
                    numFaceVerts = characterCount + 1;
                } else {
                    characterCount = CountCharInString('-', token);
                    if (characterCount > 0) {
                        formKind = PFILE_MAT_FACES_RANGE;
                        numFaceVerts = 0;
                        dashIdx = FindCharIndexInString('-', token);
                        if (dashIdx >= 0) {
                            token[dashIdx] = 0;
                            firstIdx = atoi(token);
                            upperBound = atoi((const char *)(token + dashIdx + 1));
                            numFaceVerts = upperBound - firstIdx + 1;
                        }
                    } else {
                        formKind = PFILE_MAT_FACES_SINGLE;
                        numFaceVerts = 1;
                    }
                }
                indexList8 = (int)GameHeapAlloc(numFaceVerts, 4, PlytopdLit_SourcePath_5(), 0x2CB);
                if (indexList8 != 0) {
                    switch (formKind) {
                    case PFILE_MAT_FACES_LIST:
                        offset = 0;
                        for (stepIdx = 0; stepIdx < numFaceVerts - 1; stepIdx++) {
                            sepPos = FindCharIndexInString(',', token + offset);
                            token[sepPos] = 0;
                            ((int *)indexList8)[stepIdx] = atoi((const char *)(token + offset));
                            offset = sepPos + 1;
                        }
                        ((int *)indexList8)[stepIdx] = atoi((const char *)(token + offset));
                        break;
                    case PFILE_MAT_FACES_RANGE:
                        for (stepIdx = 0; stepIdx < numFaceVerts; stepIdx++) {
                            ((int *)indexList8)[stepIdx] = firstIdx + stepIdx;
                        }
                        break;
                    case PFILE_MAT_FACES_SINGLE:
                        *(int *)indexList8 = atoi(token);
                    }
                    curEntry = &entries[*(int *)indexList8];
                    token = matTokens->tokens[tokenIndex];
                    faceFlags = ParseHexString(0, token);
                    curEntry->flags = faceFlags;
                    tokenIndex++;
                    token = matTokens->tokens[tokenIndex];
                    tokenIndex++;
                    token = matTokens->tokens[tokenIndex];
                    tokenIndex++;
                    transType = BLEND_NONE;
                    if ((curEntry->flags & 4) != 0) {
                        transType = (curEntry->flags & 0x38) >> 3;
                    }
                    curEntry->transType = transType;
                    switch (*token) {
                    case 'C':
                    case 'c':
                        curEntry->kind = PFILE_FACE_FLAT;
                        curEntry->isFlatShaded = 1;
                        curEntry->isGouraudShaded = 0;
                        curEntry->hasTexture = 0;
                        Pfile_ParseColorTokensToBgra(&curEntry->colors[0], tokenIndex, matTokens->tokens);
                        Pfile_ParseColorTokensToBgra(&curEntry->colors[1], tokenIndex, matTokens->tokens);
                        Pfile_ParseColorTokensToBgra(&curEntry->colors[2], tokenIndex, matTokens->tokens);
                        tokenIndex = Pfile_ParseColorTokensToBgra(&curEntry->colors[3], tokenIndex, matTokens->tokens);
                        break;
                    case 'G':
                    case 'g':
                        curEntry->kind = PFILE_FACE_GOURAUD;
                        curEntry->isFlatShaded = 0;
                        curEntry->isGouraudShaded = 1;
                        curEntry->hasTexture = 0;
                        tokenIndex = Pfile_ParseColorTokensToBgra(&curEntry->colors[0], tokenIndex, matTokens->tokens);
                        tokenIndex = Pfile_ParseColorTokensToBgra(&curEntry->colors[1], tokenIndex, matTokens->tokens);
                        tokenIndex = Pfile_ParseColorTokensToBgra(&curEntry->colors[2], tokenIndex, matTokens->tokens);
                        tokenIndex = Pfile_ParseColorTokensToBgra(&curEntry->colors[3], tokenIndex, matTokens->tokens);
                        break;
                    case 'T':
                    case 't':
                        curEntry->kind = PFILE_FACE_TEXTURED;
                        curEntry->isFlatShaded = 0;
                        curEntry->isGouraudShaded = 0;
                        curEntry->hasTexture = 1;
                        token = matTokens->tokens[tokenIndex];
                        curEntry->refIndex = atoi(token);
                        tokenIndex++;
                        if (textured != 0 && rsdRecord != 0 && curEntry->hasTexture != 0 && curEntry->refIndex >= 0 && curEntry->refIndex < rsdRecord->count) {
                            texRecord = (PolygonTexRecord *)(((int *)(rsdRecord->texRecordTable))[curEntry->refIndex]);
                            vramPositionX = texRecord->vramPositionX;
                            originY = texRecord->originY;
                            textureWidth = texRecord->textureWidth;
                            h = texRecord->height;
                            if (IsPowerOfTwo(textureWidth) == 0) {
                                textureWidth = RoundUpToPowerOfTwo(textureWidth);
                            }
                            if (IsPowerOfTwo(h) == 0) {
                                h = RoundUpToPowerOfTwo(h);
                            }
                            tokenIndex = Pfile_TimPixelToUv(texPage, curEntry->uv[0], tokenIndex, matTokens->tokens, vramPositionX, originY, textureWidth, h);
                            tokenIndex = Pfile_TimPixelToUv(texPage, curEntry->uv[1], tokenIndex, matTokens->tokens, vramPositionX, originY, textureWidth, h);
                            tokenIndex = Pfile_TimPixelToUv(texPage, curEntry->uv[2], tokenIndex, matTokens->tokens, vramPositionX, originY, textureWidth, h);
                            tokenIndex = Pfile_TimPixelToUv(texPage, curEntry->uv[3], tokenIndex, matTokens->tokens, vramPositionX, originY, textureWidth, h);
                        } else {
                            tokenIndex = Pfile_ParseUvTokensDiv256(curEntry->uv[0], tokenIndex, matTokens->tokens);
                            tokenIndex = Pfile_ParseUvTokensDiv256(curEntry->uv[1], tokenIndex, matTokens->tokens);
                            tokenIndex = Pfile_ParseUvTokensDiv256(curEntry->uv[2], tokenIndex, matTokens->tokens);
                            tokenIndex = Pfile_ParseUvTokensDiv256(curEntry->uv[3], tokenIndex, matTokens->tokens);
                        }
                        break;
                    case 'D':
                    case 'd':
                        curEntry->kind = PFILE_FACE_TEXTURED_FLAT;
                        curEntry->isFlatShaded = 1;
                        curEntry->isGouraudShaded = 0;
                        curEntry->hasTexture = 1;
                        token = matTokens->tokens[tokenIndex];
                        curEntry->refIndex = atoi(token);
                        tokenIndex++;
                        if (textured != 0 && rsdRecord != 0 && curEntry->hasTexture != 0 && curEntry->refIndex >= 0 && curEntry->refIndex < rsdRecord->count) {
                            entRecord = (PolygonPageEntry *)(((int *)(rsdRecord->texRecordTable))[curEntry->refIndex]);
                            regionX = entRecord->regionX;
                            mainPageY = entRecord->pageY;
                            pageW = entRecord->pageWidth;
                            regionH = entRecord->regionHeight;
                            if (IsPowerOfTwo(pageW) == 0) {
                                pageW = RoundUpToPowerOfTwo(pageW);
                            }
                            if (IsPowerOfTwo(regionH) == 0) {
                                regionH = RoundUpToPowerOfTwo(regionH);
                            }
                            tokenIndex = Pfile_TimPixelToUv(texPage, curEntry->uv[0], tokenIndex, matTokens->tokens, regionX, mainPageY, pageW, regionH);
                            tokenIndex = Pfile_TimPixelToUv(texPage, curEntry->uv[1], tokenIndex, matTokens->tokens, regionX, mainPageY, pageW, regionH);
                            tokenIndex = Pfile_TimPixelToUv(texPage, curEntry->uv[2], tokenIndex, matTokens->tokens, regionX, mainPageY, pageW, regionH);
                            tokenIndex = Pfile_TimPixelToUv(texPage, curEntry->uv[3], tokenIndex, matTokens->tokens, regionX, mainPageY, pageW, regionH);
                        } else {
                            tokenIndex = Pfile_ParseUvTokensDiv256(curEntry->uv[0], tokenIndex, matTokens->tokens);
                            tokenIndex = Pfile_ParseUvTokensDiv256(curEntry->uv[1], tokenIndex, matTokens->tokens);
                            tokenIndex = Pfile_ParseUvTokensDiv256(curEntry->uv[2], tokenIndex, matTokens->tokens);
                            tokenIndex = Pfile_ParseUvTokensDiv256(curEntry->uv[3], tokenIndex, matTokens->tokens);
                        }
                        Pfile_ParseColorTokensToBgra(&curEntry->colors[0], tokenIndex, matTokens->tokens);
                        Pfile_ParseColorTokensToBgra(&curEntry->colors[1], tokenIndex, matTokens->tokens);
                        Pfile_ParseColorTokensToBgra(&curEntry->colors[2], tokenIndex, matTokens->tokens);
                        tokenIndex = Pfile_ParseColorTokensToBgra(&curEntry->colors[3], tokenIndex, matTokens->tokens);
                        break;
                    case 'H':
                    case 'h':
                        curEntry->kind = PFILE_FACE_TEXTURED_GOURAUD;
                        curEntry->isFlatShaded = 0;
                        curEntry->isGouraudShaded = 1;
                        curEntry->hasTexture = 1;
                        token = matTokens->tokens[tokenIndex];
                        curEntry->refIndex = atoi(token);
                        tokenIndex++;
                        if (textured != 0 && rsdRecord != 0 && curEntry->hasTexture != 0 && curEntry->refIndex >= 0 && curEntry->refIndex < rsdRecord->count) {
                            workRsdItemPtr = (PolygonSpriteItem *)(((int *)(rsdRecord->texRecordTable))[curEntry->refIndex]);
                            srcX = workRsdItemPtr->srcX;
                            srcY = workRsdItemPtr->srcY;
                            spriteWidth = workRsdItemPtr->spriteWidth;
                            sprHeight = workRsdItemPtr->spriteHeight;
                            if (IsPowerOfTwo(spriteWidth) == 0) {
                                spriteWidth = RoundUpToPowerOfTwo(spriteWidth);
                            }
                            if (IsPowerOfTwo(sprHeight) == 0) {
                                sprHeight = RoundUpToPowerOfTwo(sprHeight);
                            }
                            tokenIndex = Pfile_TimPixelToUv(texPage, curEntry->uv[0], tokenIndex, matTokens->tokens, srcX, srcY, spriteWidth, sprHeight);
                            tokenIndex = Pfile_TimPixelToUv(texPage, curEntry->uv[1], tokenIndex, matTokens->tokens, srcX, srcY, spriteWidth, sprHeight);
                            tokenIndex = Pfile_TimPixelToUv(texPage, curEntry->uv[2], tokenIndex, matTokens->tokens, srcX, srcY, spriteWidth, sprHeight);
                            tokenIndex = Pfile_TimPixelToUv(texPage, curEntry->uv[3], tokenIndex, matTokens->tokens, srcX, srcY, spriteWidth, sprHeight);
                        } else {
                            tokenIndex = Pfile_ParseUvTokensDiv256(curEntry->uv[0], tokenIndex, matTokens->tokens);
                            tokenIndex = Pfile_ParseUvTokensDiv256(curEntry->uv[1], tokenIndex, matTokens->tokens);
                            tokenIndex = Pfile_ParseUvTokensDiv256(curEntry->uv[2], tokenIndex, matTokens->tokens);
                            tokenIndex = Pfile_ParseUvTokensDiv256(curEntry->uv[3], tokenIndex, matTokens->tokens);
                        }
                        tokenIndex = Pfile_ParseColorTokensToBgra(&curEntry->colors[0], tokenIndex, matTokens->tokens);
                        tokenIndex = Pfile_ParseColorTokensToBgra(&curEntry->colors[1], tokenIndex, matTokens->tokens);
                        tokenIndex = Pfile_ParseColorTokensToBgra(&curEntry->colors[2], tokenIndex, matTokens->tokens);
                        tokenIndex = Pfile_ParseColorTokensToBgra(&curEntry->colors[3], tokenIndex, matTokens->tokens);
                    }
                    for (stepIdx = 1; stepIdx < numFaceVerts - 1; stepIdx++) {
                        entries[((int *)indexList8)[stepIdx]] = *curEntry;
                    }
                    GameHeapFree((void *)indexList8, PlytopdLit_SourcePath_6(), 0x410);
                }
            }
        }
    }
    Pfile_BuildGroupsFromFaces(1, pointData, (int)verticesPtr, matCnt, pMatStates, faceCount, entries, fileArrays);
    if (fileArrays != 0) {
        vertexPtr = verticesPtr;
        for (normalIndex = 0; normalIndex < numVerts; normalIndex++) {
            Pfile_AddNormal(vertexPtr, fileArrays);
            vertexPtr++;
        }
    }
    GameHeapFree(entries, PlytopdLit_SourcePath_7(), 0x42A);
    GameHeapFree(pMatStates, PlytopdLit_SourcePath_8(), 0x42B);
    GameHeapFree(verticesPtr, PlytopdLit_SourcePath_9(), 0x42C);
    GameHeapFree(pointData, PlytopdLit_SourcePath_10(), 0x42D);
}

/*
 * Constructs binary polygon model from PLY and MAT text files.
 */
// FUNCTION: FF7 0x00691d99
int __cdecl Pfile_BuildFromPlyMat(int texPage, float scale, PolygonRsdRecord *rsdRecord, int options, int matPath, int plyPath, int outPath)
{
    PolygonTokenFile *plyTokens;
    PolygonTokenFile *matTokens;
    PolygonFileArrays *pPolyArrays;

    pPolyArrays = 0;
    plyTokens = TokenizeTextFileFromDisk((int)Token_StripHashComments, (void *)plyPath);
    if (plyTokens != 0) {
        matTokens = TokenizeTextFileFromDisk((int)Token_StripHashComments, (void *)matPath);
        if (matTokens != 0) {
            pPolyArrays = Pfile_CreatePolygonData(1, options);
            if (pPolyArrays != 0) {
                Pfile_ParsePlyAndMat(texPage, 1, scale, rsdRecord, pPolyArrays, plyTokens, matTokens);
                Pfile_FlattenEditLists(pPolyArrays);
                Pfile_WriteBinary(outPath, pPolyArrays);
                Pfile_DestroyEditLists(pPolyArrays);
            }
            FreeTokenFile(matTokens);
        }
        FreeTokenFile(plyTokens);
    }
    return (int)pPolyArrays;
}

/*
 * Calculates size of frame data for an animation.
 */
// FUNCTION: FF7 0x00691e5e
int __cdecl Anim_GetFrameDataSize(PolygonAnimation *animation)
{
    int size;

    size = 0;
    if (animation != 0) {
        size = animation->frameCount * (animation->boneCount * 0xC + 0x18);
    }
    return size;
}

/*
 * Frees animation data structures and frame allocations.
 */
// FUNCTION: FF7 0x00691e8e
void __cdecl Anim_Free(PolygonAnimBuffers *anim)
{
    if (anim != 0) {
        if (anim->ownsBlock != 0) {
            GameHeapFree((void *)anim->block, PlytopdLit_SourcePath_11(), 0x46F);
        }
        GameHeapFree((void *)anim->frameTable, PlytopdLit_SourcePath_12(), 0x472);
        GameHeapFree((void *)anim->frameData, PlytopdLit_SourcePath_13(), 0x473);
        GameHeapFree(anim, PlytopdLit_SourcePath_14(), 0x474);
    }
}

/*
 * Allocates and initializes an animation header structure.
 */
// FUNCTION: FF7 0x00691f03
int __cdecl Anim_Create(int frameCount, int boneCount, int flags)
{
    PolygonAnimation *anim;
    int frameDataSize;
    int dataPointer;
    PolygonAnimFrameEntry *tablePtr;
    int keyFrame;

    anim = (PolygonAnimation *)(GameHeapAlloc(1, 0x24, PlytopdLit_SourcePath_15(), 0x47C));
    if (anim != 0) {
        anim->isLoaded = 1;
        anim->frameCount = frameCount;
        anim->boneCount = boneCount;
        anim->flags = flags;
        frameDataSize = Anim_GetFrameDataSize(anim);
        anim->frameData = (int)GameHeapAlloc(1, frameDataSize, PlytopdLit_SourcePath_16(), 0x488);
        if (anim->frameData != 0) {
            anim->frameTable = (PolygonAnimFrameEntry *)GameHeapAlloc(anim->frameCount, 8, PlytopdLit_SourcePath_17(), 0x48C);
            if (anim->frameTable != 0) {
                dataPointer = anim->frameData;
                tablePtr = anim->frameTable;
                for (keyFrame = 0; keyFrame < anim->frameCount; keyFrame++) {
                    tablePtr->rootPose = (PolygonAnimRootPose *)dataPointer;
                    dataPointer += 0x18;
                    tablePtr->boneChannels = (PolygonBoneVector *)dataPointer;
                    dataPointer += anim->boneCount * 0xC;
                    tablePtr++;
                }
            }
        }
    }
    return (int)anim;
}

/*
 * Sets root translation or rotation channel for an animation frame.
 */
// FUNCTION: FF7 0x00692020
void __cdecl Anim_SetRootChannel(int channel, int frameIndex, PolygonAnimRootPose *pose, PolygonAnimation *anim)
{
    if (anim != 0) {
        switch (channel) {
        case ANIM_ROOT_TRANSLATE_X:
            anim->frameTable[frameIndex].rootPose->translationX = pose->translationX;
            break;
        case ANIM_ROOT_TRANSLATE_Y:
            anim->frameTable[frameIndex].rootPose->translationY = pose->translationY;
            break;
        case ANIM_ROOT_TRANSLATE_Z:
            anim->frameTable[frameIndex].rootPose->translationZ = pose->translationZ;
            break;
        case ANIM_ROOT_ROTATE_X:
            anim->frameTable[frameIndex].rootPose->rotationX = pose->rotationX;
            break;
        case ANIM_ROOT_ROTATE_Y:
            anim->frameTable[frameIndex].rootPose->rotationY = pose->rotationY;
            break;
        case ANIM_ROOT_ROTATE_Z:
            anim->frameTable[frameIndex].rootPose->rotationZ = pose->rotationZ;
            break;
        case ANIM_ROOT_ALL:
            *anim->frameTable[frameIndex].rootPose = *pose;
        }
    }
}

/*
 * Sets bone rotation or translation channel for an animation frame.
 */
// FUNCTION: FF7 0x0069210d
void __cdecl Anim_SetBoneChannel(int channel, int frameIndex, int boneIndex, PolygonBoneVector *vector, PolygonAnimation *anim)
{
    if (anim != 0) {
        switch (channel) {
        case ANIM_BONE_X:
            anim->frameTable[frameIndex].boneChannels[boneIndex].x = vector->x;
            break;
        case ANIM_BONE_Y:
            anim->frameTable[frameIndex].boneChannels[boneIndex].y = vector->y;
            break;
        case ANIM_BONE_Z:
            anim->frameTable[frameIndex].boneChannels[boneIndex].z = vector->z;
            break;
        case ANIM_BONE_ALL:
            anim->frameTable[frameIndex].boneChannels[boneIndex] = *vector;
        }
    }
}

/*
 * Loads the binary animation file `openFlags` (the file name; opened with `fileRequest`, from
 * disk or an LGP archive) and returns the animation, or 0. The 0x24-byte header is read into a
 * new buffer that becomes the `PolygonAnimation`; a version word other than 1 is refused. The
 * frame data follows: per frame a 0x18-byte root pose (rotation x, y, z, then translation x, y,
 * z) and one 0xC-byte rotation per bone, `frameCount * (boneCount * 0xC + 0x18)` bytes
 * (`Anim_GetFrameDataSize`). It then builds the frame table, two pointers per frame (root pose,
 * bone rotations). Field, the world map, the minigames and `AnimPlayer_SelectAnimation` load
 * their animations this way.
 *
 * Defect: on a failed load `Anim_Free` frees the header's `frameData` and `frameTable` words,
 * which, when the version is not 1 or the frame data does not load, still hold whatever the file
 * stored there (`Anim_WriteBinary` writes the writer's heap pointers into the header).
 */
// FUNCTION: FF7 0x006921ce
int __cdecl Anim_LoadBinary(FileOpenRequest *fileRequest, int openFlags)
{
    int frameDataBytes;
    PolygonAnimation *anim;
    int loaded;
    int handle;
    int keyFrame;
    PolygonAnimFrameEntry *entryPointer;
    int dataPtr;

    loaded = 0;
    anim = 0;
    handle = (int)OpenFileEx(fileRequest, (const char *)openFlags);
    if (handle != 0) {
        anim = (PolygonAnimation *)ReadFileIntoNewBuffer(0x24, 1, (FileHandle *)handle);
        if (anim != 0 && anim->isLoaded == 1) {
            anim->ownsBlock = 0;
            anim->block = 0;
            anim->matrixCursor = 0;
            frameDataBytes = Anim_GetFrameDataSize(anim);
            if (frameDataBytes > 0) {
                anim->frameData = ReadFileIntoNewBuffer(frameDataBytes, 1, (FileHandle *)handle);
                if (anim->frameData != 0) {
                    anim->frameTable = (PolygonAnimFrameEntry *)GameHeapAlloc(anim->frameCount, 8,
                                                               PlytopdLit_SourcePath_18(), 0x4FB);
                    if (anim->frameTable != 0) {
                        dataPtr = anim->frameData;
                        entryPointer = anim->frameTable;
                        for (keyFrame = 0; keyFrame < anim->frameCount; keyFrame++) {
                            entryPointer->rootPose = (PolygonAnimRootPose *)dataPtr;
                            dataPtr += 0x18;
                            entryPointer->boneChannels = (PolygonBoneVector *)dataPtr;
                            dataPtr += anim->boneCount * 0xC;
                            entryPointer++;
                        }
                        loaded = 1;
                    }
                }
            }
        }
        CloseFileEx((FileHandle *)handle);
    }
    if (loaded == 0) {
        Anim_Free((PolygonAnimBuffers *)anim);
        anim = 0;
    }
    return (int)anim;
}

/*
 * Opens an animation file from disk and loads its binary data.
 */
// FUNCTION: FF7 0x00692346
void __cdecl Anim_LoadBinaryFromDisk(int openFlags)
{
    int fileRequest[4];

    InitFileContext(0, fileRequest);
    Anim_LoadBinary((FileOpenRequest *)fileRequest, openFlags);
}

/*
 * Writes animation data to disk in binary format.
 */
// FUNCTION: FF7 0x0069236e
int __cdecl Anim_WriteBinary(PolygonAnimation *anim, int path)
{
    int result;
    int fileHandle;

    if (Pfile_GetAssetSourceMode() != 0) {
        result = 0;
        if (anim != 0) {
            if (Anim_GetFrameDataSize(anim) > 0) {
                fileHandle = OpenFileSimple(FILE_OPEN_CREATE, (void *)path);
                if (fileHandle != 0) {
                    if (WriteFileData(0x24, anim, (FileHandle *)fileHandle) != 0) {
                        if (WriteFileData(Anim_GetFrameDataSize(anim), (const void *)anim->frameData, (FileHandle *)fileHandle) != 0) {
                            result = 1;
                        }
                    }
                    CloseFileEx((FileHandle *)fileHandle);
                }
            }
        }
    } else {
        result = 1;
    }
    return result;
}

/*
 * Scales root translation channels in an animation by a float factor.
 */
// FUNCTION: FF7 0x00692412
void __cdecl Anim_ScaleRootTranslation(float scale, PolygonAnimation *anim)
{
    int frameI;

    if (anim != 0) {
        for (frameI = 0; frameI < anim->frameCount; frameI++) {
            ((AnimRootTranslation *)(((PolygonAnimFrameEntry *)(anim->frameTable))[frameI].rootPose))->translationX *= scale;
            ((AnimRootTranslation *)(((PolygonAnimFrameEntry *)(anim->frameTable))[frameI].rootPose))->translationY *= scale;
            ((AnimRootTranslation *)(((PolygonAnimFrameEntry *)(anim->frameTable))[frameI].rootPose))->translationZ *= scale;
        }
    }
}

/*
 * Looks up an animation script keyword token index.
 */
// FUNCTION: FF7 0x006924a6
int __cdecl AnimText_LookupKeyword(int tokenIndex, PolygonTokenFile *tokenFile)
{
    int keywordIdx;
    int index;

    keywordIdx = 0;
    for (index = 1; index < 0x10; index++) {
        if (strcmp(g_AnimTextKeywords[index],
                   tokenFile->tokens[tokenIndex]) == 0) {
            keywordIdx = index;
            break;
        }
    }
    return keywordIdx;
}

/*
 * Builds the binary animation file `outputFile` from the animation text file `fileName` when
 * models are built from their sources (`g_PfileAssetSourceMode` nonzero); otherwise it does
 * nothing and returns 1, since the binary file is expected to exist. Callers then load
 * `outputFile` with `Anim_LoadBinary`.
 *
 * The text is tokenised from disk. `:HEADER_BLOCK n` gives the number of header entries that
 * follow: `:FRAMES` (frame count), `:CHANNELS` (bone count), `:ROTATION_ORDER` (three letters
 * X, Y, Z packed as axis numbers 0..2 into the animation's flags) and `:ROTATE_RESOLUTION`
 * (units per full turn, default 256). With frames above 0 and a positive resolution,
 * `Anim_Create` makes the animation. An optional `root_coordinate` block follows: `:TX`, `:TY`,
 * `:TZ` give one root translation per frame, `:RX`, `:RY`, `:RZ` one root rotation per frame.
 * Then per bone: its name, and `:RX`, `:RY`, `:RZ` blocks with one rotation per frame. Each
 * rotation is converted to degrees (value / resolution * 360). `Anim_WriteBinary` writes the
 * result, and the animation is freed.
 *
 * Returns 1 when the file was written, 0 otherwise; a resolution of 0 or less prints "INVALID
 * ANIMATION DEGREE RESOLUTION", a missing file "COULD NOT LOAD ANIMATION FILE". Defects: without
 * `:CHANNELS` the bone count, and without `:ROTATION_ORDER` the axis order, is read
 * uninitialised; the per-frame value reads do not stop at the end of the token list.
 */
// FUNCTION: FF7 0x0069254c
int __cdecl Anim_CompileTextToBinary(int fileName, int outputFile)
{
    float rootPose[6];
    int af;
    int numFrames;
    int ah;
    PolygonAnimation *animationData;
    float degreeResolution;
    char *ai;
    int keyframe;
    int boneCnt;
    int boneIndex;
    int ak;
    int keyword8;
    PolygonTokenFile *tokens;
    float angleVec[3];
    int tokenPos;
    char axisBuffer[4];
    int aa;
    int ab;
    int ac;
    int ad;
    float ba;
    char au[0xC8];
    int ae;

    aa = 0;
    if (Pfile_GetAssetSourceMode() != 0) {
        ah = 0;
        numFrames = 0;
        degreeResolution = 256.0f;
        tokens = TokenizeTextFileFromDisk(0, (void *)fileName);
        if (tokens != 0) {
            tokenPos = 0;
            af = 0;
            while (tokenPos < tokens->tokenCount) {
                keyword8 = AnimText_LookupKeyword(tokenPos, tokens);
                tokenPos++;
                if (keyword8 == ANIM_KEYWORD_HEADER_BLOCK) {
                    af = TokenAsInt(tokenPos, tokens);
                    tokenPos++;
                    break;
                }
            }
            if (af > 0) {
                for (ak = 0; ak < af; ak++) {
                    keyword8 = AnimText_LookupKeyword(tokenPos, tokens);
                    tokenPos++;
                    switch (keyword8) {
                    case ANIM_KEYWORD_HEADER_BLOCK:
                        af = TokenAsInt(tokenPos, tokens);
                        break;
                    case ANIM_KEYWORD_FRAMES:
                        numFrames = TokenAsInt(tokenPos, tokens);
                        break;
                    case ANIM_KEYWORD_CHANNELS:
                        boneCnt = TokenAsInt(tokenPos, tokens);
                        break;
                    case ANIM_KEYWORD_ROTATION_ORDER:
                        ai = tokens->tokens[tokenPos];
                        *(int *)axisBuffer = 0;
                        for (ab = 0; ab < 3; ab++) {
                            switch (ai[ab]) {
                            case 'X':
                            case 'x':
                                axisBuffer[ab] = ANIM_AXIS_X;
                                break;
                            case 'Y':
                            case 'y':
                                axisBuffer[ab] = ANIM_AXIS_Y;
                                break;
                            case 'Z':
                            case 'z':
                                axisBuffer[ab] = ANIM_AXIS_Z;
                                break;
                            }
                        }
                        break;
                    case ANIM_KEYWORD_ROTATE_RESOLUTION:
                        degreeResolution = TokenAsFloat(tokenPos, tokens);
                        break;
                    }
                    tokenPos++;
                }
                if (numFrames > 0 && boneCnt >= 0 && degreeResolution > 0.0f) {
                    animationData = (PolygonAnimation *)(Anim_Create(numFrames, boneCnt, *(int *)axisBuffer));
                    if (animationData != 0) {
                        ad = 1;
                        keyword8 = AnimText_LookupKeyword(tokenPos, tokens);
                        if (keyword8 == ANIM_KEYWORD_ROOT_COORDINATE) {
                            tokenPos++;
                        } else {
                            ad = 0;
                        }
                        while (ad != 0 && tokenPos < *(int *)tokens) {
                            keyword8 = AnimText_LookupKeyword(tokenPos, tokens);
                            tokenPos++;
                            switch (keyword8) {
                            case ANIM_KEYWORD_TX:
                                ac = ANIM_ROOT_TRANSLATE_X;
                                break;
                            case ANIM_KEYWORD_TY:
                                ac = ANIM_ROOT_TRANSLATE_Y;
                                break;
                            case ANIM_KEYWORD_TZ:
                                ac = ANIM_ROOT_TRANSLATE_Z;
                                break;
                            case ANIM_KEYWORD_RX:
                                ac = ANIM_ROOT_ROTATE_X;
                                break;
                            case ANIM_KEYWORD_RY:
                                ac = ANIM_ROOT_ROTATE_Y;
                                break;
                            case ANIM_KEYWORD_RZ:
                                ac = ANIM_ROOT_ROTATE_Z;
                                break;
                            default:
                                ad = 0;
                                tokenPos--;
                            }
                            if (ad != 0) {
                                for (keyframe = 0; keyframe < numFrames; keyframe++) {
                                    switch (ac) {
                                    case ANIM_ROOT_TRANSLATE_X:
                                        rootPose[3] = TokenAsFloat(tokenPos, tokens);
                                        break;
                                    case ANIM_ROOT_TRANSLATE_Y:
                                        rootPose[4] = TokenAsFloat(tokenPos, tokens);
                                        break;
                                    case ANIM_ROOT_TRANSLATE_Z:
                                        rootPose[5] = TokenAsFloat(tokenPos, tokens);
                                        break;
                                    case ANIM_ROOT_ROTATE_X:
                                        rootPose[0] = TokenAsFloat(tokenPos, tokens);
                                        rootPose[0] = rootPose[0] / degreeResolution * 360.0f;
                                        break;
                                    case ANIM_ROOT_ROTATE_Y:
                                        rootPose[1] = TokenAsFloat(tokenPos, tokens);
                                        rootPose[1] = rootPose[1] / degreeResolution * 360.0f;
                                        break;
                                    case ANIM_ROOT_ROTATE_Z:
                                        rootPose[2] = TokenAsFloat(tokenPos, tokens);
                                        rootPose[2] = rootPose[2] / degreeResolution * 360.0f;
                                        break;
                                    }
                                    Anim_SetRootChannel(ac, keyframe, (PolygonAnimRootPose *)rootPose, animationData);
                                    tokenPos++;
                                }
                            }
                        }
                        for (boneIndex = 0; boneIndex < boneCnt; boneIndex++) {
                            if (aa != 0) {
                                // STRING: FF7 0x00910ef8
                                sprintf(au, "%d %s \n", boneIndex,
                                        tokens->tokens[tokenPos]);
                                OutputDebugStringA(au);
                            }
                            tokenPos++;
                            ae = 1;
                            while (ae != 0 && tokenPos < *(int *)tokens) {
                                keyword8 = AnimText_LookupKeyword(tokenPos, tokens);
                                tokenPos++;
                                switch (keyword8) {
                                case ANIM_KEYWORD_RX:
                                    ac = ANIM_BONE_X;
                                    break;
                                case ANIM_KEYWORD_RY:
                                    ac = ANIM_BONE_Y;
                                    break;
                                case ANIM_KEYWORD_RZ:
                                    ac = ANIM_BONE_Z;
                                    break;
                                default:
                                    ae = 0;
                                    tokenPos--;
                                }
                                if (ae != 0) {
                                    for (keyframe = 0; keyframe < numFrames; keyframe++) {
                                        switch (ac) {
                                        case ANIM_BONE_X:
                                            ba = TokenAsFloat(tokenPos, tokens);
                                            angleVec[0] = ba / degreeResolution * 360.0f;
                                            break;
                                        case ANIM_BONE_Y:
                                            ba = TokenAsFloat(tokenPos, tokens);
                                            angleVec[1] = ba / degreeResolution * 360.0f;
                                            break;
                                        case ANIM_BONE_Z:
                                            ba = TokenAsFloat(tokenPos, tokens);
                                            angleVec[2] = ba / degreeResolution * 360.0f;
                                            break;
                                        }
                                        Anim_SetBoneChannel(ac, keyframe, boneIndex, (PolygonBoneVector *)angleVec, animationData);
                                        tokenPos++;
                                    }
                                }
                            }
                        }
                        if (Anim_WriteBinary(animationData, outputFile) != 0) {
                            ah = 1;
                        }
                        Anim_Free((PolygonAnimBuffers *)animationData);
                    }
                } else if (degreeResolution <= 0.0f) {
                    // STRING: FF7 0x00910f00
                    Gfx_DebugPrint("INVALID ANIMATION DEGREE RESOLUTION \n");
                }
            }
            FreeTokenFile(tokens);
        } else {
            char al[0xC8];
            sprintf(al,
                    // STRING: FF7 0x00910f28
                    "ERROR: COULD NOT LOAD ANIMATION FILE %s\n", fileName);
            Gfx_DebugPrint(al);
        }
    } else {
        ah = 1;
    }
    return ah;
}

/*
 * Frees counted array of allocated skeleton string names.
 */
// FUNCTION: FF7 0x00692c85
void __cdecl Skel_FreeStringArray(int count, int strings)
{
    int index;

    if (strings != 0) {
        for (index = 0; index < count; index++) {
            // STRING: FF7 0x00910f54
            GameHeapFree((void *)((int *)strings)[index], "C:\\lib\\src\\polygon\\plytopd.cpp", 0x6ED);
            ((int *)strings)[index] = 0;
        }
        // STRING: FF7 0x00910f74
        GameHeapFree((void *)strings, "C:\\lib\\src\\polygon\\plytopd.cpp", 0x6F1);
    }
}

/*
 * Frees skeleton hierarchy structure, bone arrays, and meshes.
 */
// FUNCTION: FF7 0x00692cee
void __cdecl Skel_Free(PolygonSkelHierarchy *skel)
{
    PolygonSkelBone *part;
    int partIndex;
    PolygonSkelModelEntry *modelEntry;
    int strIdx;

    if (skel != 0) {
        DestroyListAndNull((List **)&skel->partList);
        part = (PolygonSkelBone *)skel->parts;
        if (skel->parts != 0) {
            for (partIndex = 0; partIndex < skel->partCount; partIndex++) {
                if (part->modelEntries != 0) {
                    modelEntry = (PolygonSkelModelEntry *)part->modelEntries;
                    for (strIdx = 0; strIdx < part->childCount; strIdx++) {
                        if (skel->modelPool != 0) {
                            Pfile_FreeRsdModel(modelEntry->model, (void *)skel->modelPool);
                            modelEntry->model = 0;
                        }
                        modelEntry++;
                    }
                    // STRING: FF7 0x00910f94
                    GameHeapFree((void *)part->modelEntries, "C:\\lib\\src\\polygon\\plytopd.cpp", 0x717);
                }
                Skel_FreeStringArray(part->childCount, part->childTable);
                // STRING: FF7 0x00910fb4
                GameHeapFree(part->parentName, "C:\\lib\\src\\polygon\\plytopd.cpp", 0x71B);
                // STRING: FF7 0x00910fd4
                GameHeapFree((void *)part->name, "C:\\lib\\src\\polygon\\plytopd.cpp", 0x71C);
                part++;
            }
            // STRING: FF7 0x00910ff4
            GameHeapFree((void *)skel->parts, "C:\\lib\\src\\polygon\\plytopd.cpp", 0x721);
        }
        // STRING: FF7 0x00911014
        GameHeapFree(skel, "C:\\lib\\src\\polygon\\plytopd.cpp", 0x724);
    }
}

/*
 * Allocates and initializes a skeleton header structure.
 */
// FUNCTION: FF7 0x00692e48
int __cdecl Skel_Create(int partCount)
{
    PolygonSkelHierarchy *skel;
    int error;

    error = 1;
    // STRING: FF7 0x00911034
    skel = (PolygonSkelHierarchy *)GameHeapAlloc(1, 0xA8, "C:\\lib\\src\\polygon\\plytopd.cpp", 0x72E);
    if (skel != 0) {
        skel->initFlag = 1;
        skel->drawEnabled = 1;
        skel->partCount = partCount;
        // STRING: FF7 0x00911054
        skel->parts = (int)GameHeapAlloc(partCount, 0x1C, "C:\\lib\\src\\polygon\\plytopd.cpp", 0x737);
        if (skel->parts != 0) {
            error = 0;
        }
    }
    if (error != 0) {
        Skel_Free((PolygonSkelHierarchy *)skel);
        skel = 0;
    }
    return (int)skel;
}

/*
 * Loads the `.HRC` skeleton `fileName` (opened with request `fileContext`, from disk or an LGP
 * archive, inside directory `pushDirectory` when it is not 0) and returns its
 * `PolygonSkelHierarchy`, or 0. Field, the world map, the submarine and the highway load their
 * skeletons this way; `Skel_BuildHierarchyAndLoadParts` then loads the parts.
 *
 * The file is tokenised with `#` comments stripped. `:HEADER_BLOCK n` gives the number of header
 * entries; `:BONES` (or `:CHANNELS`) gives the bone count, 0 counting as 1. Per bone follow its
 * name, its parent's name, its length (stored negated), the number of RSD models it carries and
 * their names. Each bone's parent index is then resolved: a parent named "root" gives -1,
 * otherwise the index of the bone with that name.
 *
 * Defects: without `:BONES` or `:CHANNELS` the bone count is read uninitialised; a parent name
 * that matches no bone leaves index 0, the first bone. The directory is left again only when
 * `PushFileContextDirectory` returned 1, which it never does for an archive request, so there
 * the LGP lookup prefix it set stays in force.
 */
// FUNCTION: FF7 0x00692ee1
int __cdecl Skel_LoadHrc(int fileContext, int pushDirectory, int fileName)
{
    int boneCnt;
    int partCount;
    int pushRes;
    PolygonTokenFile *tokenData;
    int loopIndex;
    int lexToken;
    PolygonSkelHierarchy *pSkel;
    int errorState;
    char dirStatus[0xC8];
    PolygonSkelBone *partPointer;
    int tokenIdx;
    int aa;
    int ab;
    PolygonSkelBone *ac;

    pSkel = 0;
    pushRes = 0;
    if (pushDirectory != 0) {
        pushRes = PushFileContextDirectory((FileOpenRequest *)fileContext, (void *)pushDirectory, dirStatus);
    }
    errorState = 1;
    tokenData = TokenizeTextFile(Token_StripHashComments, (void *)fileContext, (char *)fileName);
    if (tokenData != 0) {
        tokenIdx = 0;
        boneCnt = 0;
        while (tokenIdx < tokenData->tokenCount) {
            lexToken = AnimText_LookupKeyword(tokenIdx, tokenData);
            tokenIdx++;
            if (lexToken == ANIM_KEYWORD_HEADER_BLOCK) {
                boneCnt = TokenAsInt(tokenIdx, tokenData);
                tokenIdx++;
                break;
            }
        }
        if (boneCnt > 0) {
            for (loopIndex = 0; loopIndex < boneCnt; loopIndex++) {
                lexToken = AnimText_LookupKeyword(tokenIdx, tokenData);
                tokenIdx++;
                switch (lexToken) {
                case ANIM_KEYWORD_HEADER_BLOCK:
                    boneCnt = TokenAsInt(tokenIdx, tokenData);
                    break;
                case ANIM_KEYWORD_CHANNELS:
                    partCount = TokenAsInt(tokenIdx, tokenData);
                    if (partCount == 0) {
                        partCount = 1;
                    }
                    break;
                case ANIM_KEYWORD_BONES:
                    partCount = TokenAsInt(tokenIdx, tokenData);
                    if (partCount == 0) {
                        partCount = 1;
                    }
                    break;
                }
                tokenIdx++;
            }
            if (partCount > 0) {
                pSkel = (PolygonSkelHierarchy *)Skel_Create(partCount);
                if (pSkel != 0) {
                    errorState = 0;
                    partPointer = (PolygonSkelBone *)pSkel->parts;
                    for (loopIndex = 0; loopIndex < partCount; loopIndex++) {
                        partPointer->name = (int)GameHeapStrDup(TokenAsString(tokenIdx, tokenData));
                        tokenIdx++;
                        partPointer->parentName = GameHeapStrDup(TokenAsString(tokenIdx, tokenData));
                        tokenIdx++;
                        partPointer->boneOffset = TokenAsFloat(tokenIdx, tokenData);
                        partPointer->boneOffset = -partPointer->boneOffset;
                        tokenIdx++;
                        partPointer->childCount = TokenAsInt(tokenIdx, tokenData);
                        tokenIdx++;
                        if (partPointer->childCount > 0) {
                            partPointer->modelEntries = (int)GameHeapAlloc(
                                partPointer->childCount, 8,
                                // STRING: FF7 0x00911074
                                "C:\\lib\\src\\polygon\\plytopd.cpp", 0x7BC);
                            partPointer->childTable = (int)GameHeapAlloc(
                                partPointer->childCount, 4,
                                // STRING: FF7 0x00911094
                                "C:\\lib\\src\\polygon\\plytopd.cpp", 0x7BD);
                            if (partPointer->modelEntries != 0 &&
                                partPointer->childTable != 0) {
                                for (aa = 0; aa < partPointer->childCount; aa++) {
                                    ((int *)partPointer->childTable)[aa] =
                                        (int)GameHeapStrDup(TokenAsString(tokenIdx, tokenData));
                                    tokenIdx++;
                                }
                            } else {
                                errorState = 1;
                                break;
                            }
                        }
                        partPointer++;
                    }
                    partPointer = (PolygonSkelBone *)pSkel->parts;
                    for (loopIndex = 0; loopIndex < partCount; loopIndex++) {
                        if (strcmp(partPointer->parentName, g_HrcRootName) == 0) {
                            partPointer->parentIndex = -1;
                        } else {
                            ac = (PolygonSkelBone *)pSkel->parts;
                            for (ab = 0; ab < partCount; ab++) {
                                if (strcmp(partPointer->parentName, (char *)ac->name) == 0) {
                                    partPointer->parentIndex = ab;
                                    break;
                                }
                                ac++;
                            }
                        }
                        partPointer++;
                    }
                }
            }
        }
        FreeTokenFile(tokenData);
    }
    if (errorState != 0) {
        Skel_Free((PolygonSkelHierarchy *)pSkel);
        pSkel = 0;
    }
    if (pushDirectory != 0 && pushRes != 0) {
        PushFileContextDirectory((FileOpenRequest *)fileContext, dirStatus, 0);
    }
    return (int)pSkel;
}

/*
 * Opens an HRC file from disk and parses its hierarchy data.
 */
// FUNCTION: FF7 0x006934f5
int __cdecl Skel_LoadHrcFromDisk(int pushDirectory, int fileName)
{
    int fileContext[4];

    InitFileContext(0, fileContext);
    return Skel_LoadHrc((int)fileContext, pushDirectory, fileName);
}

/*
 * Recursively traverses skeleton hierarchy to emit bone definitions.
 */
// FUNCTION: FF7 0x00693521
int __cdecl Skel_EmitChildrenRecursive(int parentIndex, PolygonSkelHierarchy *skel)
{
    int listVal;
    int result;
    PolygonSkelBone *bone;
    int boneIndex;
    short tuple[2];
    int linked;
    PolygonSkelBone *parentBone;

    result = 0;
    if (skel != 0) {
        bone = (PolygonSkelBone *)skel->parts;
        parentBone = (PolygonSkelBone *)skel->parts + parentIndex;
        for (boneIndex = 0; boneIndex < skel->partCount; boneIndex++) {
            linked = 0;
            if (bone->parentName != 0 && parentBone->name != 0) {
                if (strcmp(bone->parentName, (char *)parentBone->name) == 0) {
                    linked = 1;
                }
            } else if (bone->parentIndex == parentIndex) {
                linked = 1;
            }
            if (linked != 0) {
                if (skel->parentIndex != 0) {
                    // STRING: FF7 0x009110b4
                    OutputDebugStringA("PUSH \n");
                    if (bone->name != 0) {
                        sprintf(g_PlytopdDebugBuffer,
                                // STRING: FF7 0x009110bc
                                "CHILD: %d %s \n", boneIndex, bone->name);
                        OutputDebugStringA(g_PlytopdDebugBuffer);
                    } else {
                        sprintf(g_PlytopdDebugBuffer,
                                // STRING: FF7 0x009110cc
                                "CHILD: %d \n", boneIndex);
                        OutputDebugStringA(g_PlytopdDebugBuffer);
                    }
                }
                tuple[0] = 1;
                tuple[1] = (short)boneIndex;
                listVal = *(int *)tuple;
                ListAppendValue(listVal, (List *)skel->partList);
                if (Skel_EmitChildrenRecursive(boneIndex, skel) == 0 && skel->parentIndex != 0) {
                    // STRING: FF7 0x009110d8
                    OutputDebugStringA("END \n");
                }
                if (skel->parentIndex != 0) {
                    // STRING: FF7 0x009110e0
                    OutputDebugStringA("POP \n");
                }
                tuple[0] = 2;
                tuple[1] = (short)boneIndex;
                listVal = *(int *)tuple;
                ListAppendValue(listVal, (List *)skel->partList);
                result = 1;
            }
            bone++;
        }
    }
    return result;
}

/*
 * Scales bone lengths across all bones in a skeleton.
 */
// FUNCTION: FF7 0x0069371e
void __cdecl Skel_ScaleBoneLengths(float scale, PolygonSkelHierarchy *skel)
{
    int partIndex;

    if (skel != 0) {
        for (partIndex = 0; partIndex < skel->partCount; partIndex++) {
            ((PolygonSkelBone *)skel->parts)[partIndex].boneOffset *= scale;
        }
    }
}

/*
 * Releases texture references held by material hundred records.
 */
// FUNCTION: FF7 0x0069376e
void __cdecl Pfile_ReleaseHundredTextures(PolygonFileArrays *model)
{
    int hundredIdx;
    PolygonHundredRecord *hundred;

    if (model != 0) {
        for (hundredIdx = 0; hundredIdx < model->hundredCount; hundredIdx++) {
            hundred = (PolygonHundredRecord *)(model->hundreds + hundredIdx * 0x64);
            if (hundred != 0) {
                Gfx_ReleaseTextureRecordRef(hundred->textureHandle);
                hundred->textureHandle = 0;
            }
        }
    }
}

/*
 * Applies blend mode settings to all material hundred records in model.
 */
// FUNCTION: FF7 0x006937cd
void __cdecl Pfile_ApplyBlendModeToHundreds(PolygonBlendSource *blendSource, PolygonFileArrays *model)
{
    int hundredIdx;
    int hundred;

    if (blendSource != 0 && model != 0) {
        for (hundredIdx = 0; hundredIdx < model->hundredCount; hundredIdx++) {
            hundred = model->hundreds + hundredIdx * 0x64;
            if (hundred != 0) {
                Pfile_SetHundredBlendMode(1, blendSource->blendMode, (PolygonHundredState *)hundred);
            }
        }
    }
}

/*
 * Copies blend mode flags to texture header records.
 */
// FUNCTION: FF7 0x0069382e
void __cdecl Pfile_CopyBlendStateToTexHeader(PolygonTextureRecord *texHeader, PolygonHundredRecord *hundred)
{
    if (texHeader != 0 && hundred != 0) {
        texHeader->blendStateA = hundred->blendStateA;
        texHeader->blendStateB = hundred->blendStateB;
    }
}

/*
 * Binds loaded texture objects to material hundred records.
 */
// FUNCTION: FF7 0x0069385d
void __cdecl Pfile_BindTexturesToHundreds(void *request, int textureCount, int textures, PolygonFileArrays *model, GfxEngineContext *flags)
{
    int hundredIdx;
    PolygonHundredRecord *hundred;
    PolygonTextureRecord *texture;

    if (model != 0 && textures != 0) {
        for (hundredIdx = 0; hundredIdx < model->hundredCount; hundredIdx++) {
            hundred = (PolygonHundredRecord *)(model->hundreds + hundredIdx * 0x64);
            if (hundred != 0 && (hundred->flags & 2) != 0 && hundred->textureIndex < textureCount) {
                texture = ((PolygonTextureRecord **)textures)[hundred->textureIndex];
                Pfile_CopyBlendStateToTexHeader(texture, (PolygonHundredRecord *)hundred);
                hundred->textureHandle = Gfx_AcquireTextureForRequest((int)request, (int)texture, (int)flags);
            }
        }
    }
}

/*
 * Makes a skeleton `Skel_LoadHrc` loaded drawable: builds the bone draw order and loads every
 * bone's RSD models, from directory `dirName` when it is not 0. `loadOptions` null means the
 * `Pfile_InitLoadOptions` defaults. `modelPool` is the graphics context the models are loaded
 * into, kept in the skeleton for `Skel_Free`.
 *
 * The skeleton keeps the options' `flags`. Bones whose parent is "root" (or, without a parent
 * name, parent index -1) are counted as roots, and every bone length is scaled by
 * `loadOptions->scale`. With at least one root a part list is built: for each root a push entry
 * (1, bone), its children in depth-first order (`Skel_EmitChildrenRecursive`) and a pop entry
 * (2, bone); `Anim_DrawSkeletonFrame` walks this list to push and pop bone matrices. Then, when
 * `loadOptions->loadBoneParts` is set, each RSD name of each bone is loaded as `<name>.RSD` with
 * `LoadRsdModel` into the bone's model table. A skeleton without a root gets neither. With the
 * skeleton's debug word (+0x08) set, the roots and children are printed.
 *
 * Defect: as in `Skel_LoadHrc`, the directory is left only when `PushFileContextDirectory`
 * returned 1, so for an archive request the LGP lookup prefix stays set.
 */
// FUNCTION: FF7 0x00693902
void __cdecl Skel_BuildHierarchyAndLoadParts(int dirName, PolygonSkelHierarchy *skel, PfileLoadOptions *loadOptions, int modelPool)
{
    PfileLoadOptions optionsBuffer;
    int rootTotal;
    int pushResult;
    PolygonSkelBone *part;
    int entry;
    char message[0xC8];
    int isRootPart;
    char dirStatus[0xC8];
    int partIndex;
    PolygonRsdModel *handle;
    PolygonSkelModelEntry *modelEntry;
    int modelOrdinal;
    char localNameBufData[0xC8];
    short tag[2];

    if (loadOptions == 0) {
        Pfile_InitLoadOptions(&optionsBuffer);
        loadOptions = &optionsBuffer;
    }
    if (skel != 0) {
        pushResult = 0;
        if (dirName != 0) {
            pushResult = PushFileContextDirectory(&loadOptions->file, (void *)dirName, dirStatus);
        }
        skel->modelPool = modelPool;
        skel->loadOptions = loadOptions->flags;
        rootTotal = 0;
        part = (PolygonSkelBone *)skel->parts;
        for (partIndex = 0; partIndex < skel->partCount; partIndex++) {
            if (part->parentName != 0) {
                if (strcmp(part->parentName, g_HrcRootName) == 0) {
                    rootTotal++;
                }
            } else if (part->parentIndex == -1) {
                rootTotal++;
            }
            part++;
        }
        skel->partTable = rootTotal;
        Skel_ScaleBoneLengths(loadOptions->scale, skel);
        if (skel->partTable > 0) {
            skel->partList = (int)CreateList(0);
            if (skel->partList != 0) {
                rootTotal = 0;
                part = (PolygonSkelBone *)skel->parts;
                for (partIndex = 0; partIndex < skel->partCount; partIndex++) {
                    isRootPart = 0;
                    if (part->parentName != 0) {
                        if (strcmp(part->parentName, g_HrcRootName) == 0) {
                            isRootPart = 1;
                        }
                    } else if (part->parentIndex == -1) {
                        isRootPart = 1;
                    }
                    if (isRootPart != 0) {
                        if (skel->parentIndex != 0) {
                            if (part->name != 0) {
                                // STRING: FF7 0x009110e8
                                sprintf(message, "ROOT CHILD: %d %s \n", partIndex, part->name);
                                OutputDebugStringA(message);
                            } else {
                                // STRING: FF7 0x009110fc
                                sprintf(message, "ROOT CHILD: %d \n", partIndex);
                                OutputDebugStringA(message);
                            }
                        }
                        tag[0] = 1;
                        tag[1] = (short)partIndex;
                        entry = *(int *)tag;
                        ListAppendValue(entry, (List *)skel->partList);
                        Skel_EmitChildrenRecursive(partIndex, skel);
                        tag[0] = 2;
                        tag[1] = (short)partIndex;
                        entry = *(int *)tag;
                        ListAppendValue(entry, (List *)skel->partList);
                        rootTotal++;
                    }
                    part++;
                }
                part = (PolygonSkelBone *)skel->parts;
                for (partIndex = 0; partIndex < skel->partCount; partIndex++) {
                    for (modelOrdinal = 0; modelOrdinal < part->childCount; modelOrdinal++) {
                        if (part->modelEntries != 0) {
                            modelEntry = (PolygonSkelModelEntry *)part->modelEntries + modelOrdinal;
                            if (part->childTable != 0) {
                                // STRING: FF7 0x00911110
                                sprintf(localNameBufData, "%s.%s", ((int *)part->childTable)[modelOrdinal],
                                        PlytopdLit_Rsd());
                                handle = 0;
                                if (loadOptions->loadBoneParts != 0) {
                                    handle = (PolygonRsdModel *)(LoadRsdModel(loadOptions, localNameBufData, (void *)modelPool));
                                }
                                if (handle != 0) {
                                    modelEntry->model = handle;
                                }
                            }
                        }
                    }
                    part++;
                }
            }
        }
        if (dirName != 0 && pushResult != 0) {
            PushFileContextDirectory(&loadOptions->file, dirStatus, 0);
        }
    }
}

/*
 * Toggles visibility flags for skeleton part meshes.
 */
// FUNCTION: FF7 0x00693e0d
void __cdecl Skel_SetPartsVisible(int visible, PolygonSkelHierarchy *skeleton)
{
    PolygonSetGroup *part;
    PolygonSkelModelEntry *entry;
    int partIndex;
    int entIndex;

    if (skeleton != 0) {
        part = (PolygonSetGroup *)skeleton->parts;
        for (partIndex = 0; partIndex < skeleton->partCount; partIndex++) {
            if (part->entryCount > 0 && part->entries != 0) {
                entry = (PolygonSkelModelEntry *)part->entries;
                for (entIndex = 0; entIndex < part->entryCount; entIndex++) {
                    if (entry->model != 0 && entry->model->polygonSet != 0) {
                        ((GfxPolygonSet *)entry->model->polygonSet)->drawEnabled = visible;
                    }
                    entry++;
                }
            }
            part++;
        }
    }
}

/*
 * Retrieves texture information descriptor from first skeleton part.
 */
// FUNCTION: FF7 0x00693ebb
int __cdecl Skel_GetFirstPartTexInfo(int partIndex, PolygonSkeleton *skeleton, PolygonPartTexInfo *out)
{
    int state;
    PolygonSkeletonPart *part;
    PolygonSkelModelEntry *entry;

    state = 0;
    if (skeleton != 0 && partIndex < skeleton->partCount) {
        part = (PolygonSkeletonPart *)(skeleton->parts + partIndex * 0x1C);
        if (part->entryCount > 0) {
            entry = (PolygonSkelModelEntry *)part->firstEntry;
            if (entry != 0 && entry->model != 0) {
                out->cachedTransform = GetCachedDeviceTransform(0, (void *)(((GfxPolygonSet *)entry->model->polygonSet)->matrices));
                out->source = ((GfxPolygonSet *)entry->model->polygonSet)->source;
                state = 1;
            }
        }
    }
    return state;
}

/*
 * Frees mesh geometry records associated with an RSD file.
 */
// FUNCTION: FF7 0x00693f47
void __cdecl Pfile_FreeRsdMeshes(PolygonRsdRecord *rsd)
{
    int index;

    if (rsd != 0) {
        if (rsd->texRecordTable != 0) {
            for (index = 0; index < rsd->count; index++) {
                Pfile_DestroyTim((PolygonTim *)(((int *)(rsd->texRecordTable))[index]));
            }
            // STRING: FF7 0x00911118
            GameHeapFree((void *)rsd->texRecordTable, "C:\\lib\\src\\polygon\\plytopd.cpp", 0x9CF);
        }
        rsd->texRecordTable = 0;
    }
}

/*
 * Frees an RSD record and all loaded textures and materials.
 */
// FUNCTION: FF7 0x00693fb5
void __cdecl Pfile_FreeRsdRecord(PolygonRsdRecord *rsd)
{
    int idx;
    int meshIndex;

    if (rsd != 0) {
        if (rsd->meshes != 0) {
            for (meshIndex = 0; meshIndex < rsd->meshCount; meshIndex++) {
                Pfile_ReleaseHundredTextures((PolygonFileArrays *)(((int *)(rsd->meshes))[meshIndex]));
                Pfile_Free((PolygonFileArrays *)(((int *)(rsd->meshes))[meshIndex]));
            }
            // STRING: FF7 0x00911138
            GameHeapFree((void *)rsd->meshes, "C:\\lib\\src\\polygon\\plytopd.cpp", 0x9E5);
        }
        if (rsd->textures != 0) {
            for (idx = 0; idx < rsd->count; idx++) {
                Gfx_ReleaseCachedTexture(((int *)(rsd->textures))[idx]);
            }
            // STRING: FF7 0x00911158
            GameHeapFree((void *)rsd->textures, "C:\\lib\\src\\polygon\\plytopd.cpp", 0x9EF);
        }
        Pfile_FreeRsdMeshes(rsd);
        if (rsd->block != 0) {
            for (idx = 0; idx < rsd->count; idx++) {
                // STRING: FF7 0x00911178
                GameHeapFree((void *)((int *)(rsd->block))[idx], "C:\\lib\\src\\polygon\\plytopd.cpp", 0x9F8);
            }
            // STRING: FF7 0x00911198
            GameHeapFree((void *)rsd->block, "C:\\lib\\src\\polygon\\plytopd.cpp", 0x9FB);
        }
        // STRING: FF7 0x009111b8
        GameHeapFree((void *)rsd->matFileName, "C:\\lib\\src\\polygon\\plytopd.cpp", 0x9FE);
        // STRING: FF7 0x009111d8
        GameHeapFree((void *)rsd->plyFileName, "C:\\lib\\src\\polygon\\plytopd.cpp", 0x9FF);
        // STRING: FF7 0x009111f8
        GameHeapFree((void *)rsd->fileName, "C:\\lib\\src\\polygon\\plytopd.cpp", 0xA00);
        // STRING: FF7 0x00911218
        GameHeapFree(rsd, "C:\\lib\\src\\polygon\\plytopd.cpp", 0xA02);
    }
}

/*
 * Parses RSD resource descriptor text file into an RSD record.
 */
// FUNCTION: FF7 0x00694156
int __cdecl Pfile_ParseRsdFile(void *fileBuffer, int fileName)
{
    int failed;
    PolygonRsdRecord *rsd;
    char errorMsg[0xC8];
    int position;
    int textureIndex;
    int token;
    int tokenPosition;
    PolygonTokenFile *tokenFile;
    int texNum;
    int textureName;

    failed = 1;
    // STRING: FF7 0x00911238
    rsd = (PolygonRsdRecord *)GameHeapAlloc(1, 0x24, "C:\\lib\\src\\polygon\\plytopd.cpp", 0xA0D);
    if (rsd != 0) {
        failed = 0;
        tokenPosition = 0;
        tokenFile = TokenizeTextFile(Token_StripHashComments, fileBuffer, (char *)fileName);
        if (tokenFile != 0) {
            tokenPosition++;
            token = (int)TokenAsString(tokenPosition, tokenFile);
            if (*(char *)token != 0) {
                position = FindCharIndexInString(0x3D, (char *)token);
                if (position >= 0) {
                    rsd->fileName = (int)GameHeapStrDup((char *)(token + position + 1));
                }
            }
            tokenPosition++;
            token = (int)TokenAsString(tokenPosition, tokenFile);
            if (*(char *)token != 0) {
                position = FindCharIndexInString(0x3D, (char *)token);
                if (position >= 0) {
                    rsd->plyFileName = (int)GameHeapStrDup((char *)(token + position + 1));
                }
            }
            tokenPosition++;
            token = (int)TokenAsString(tokenPosition, tokenFile);
            if (*(char *)token != 0) {
                position = FindCharIndexInString(0x3D, (char *)token);
                if (position >= 0) {
                    rsd->matFileName = (int)GameHeapStrDup((char *)(token + position + 1));
                }
            }
            tokenPosition++;
            token = (int)TokenAsString(tokenPosition, tokenFile);
            if (*(char *)token != 0) {
                position = FindCharIndexInString(0x3D, (char *)token);
                if (position >= 0) {
                    rsd->count = atoi((const char *)(token + position + 1));
                }
            }
            tokenPosition++;
            if (rsd->count > 0) {
                rsd->block = (int)GameHeapAlloc(rsd->count, 4,
                                                           // STRING: FF7 0x00911258
                                                           "C:\\lib\\src\\polygon\\plytopd.cpp",
                                                           0xA53);
                rsd->texRecordTable = (int)GameHeapAlloc(rsd->count, 4,
                                                           // STRING: FF7 0x00911278
                                                           "C:\\lib\\src\\polygon\\plytopd.cpp",
                                                           0xA54);
                rsd->textures = (int)GameHeapAlloc(rsd->count, 4,
                                                           // STRING: FF7 0x00911298
                                                           "C:\\lib\\src\\polygon\\plytopd.cpp",
                                                           0xA55);
                if (rsd->block != 0 && rsd->texRecordTable != 0 && rsd->textures != 0) {
                    for (textureIndex = 0; textureIndex < rsd->count; textureIndex++) {
                        token = (int)TokenAsString(tokenPosition, tokenFile);
                        if (*(char *)token != 0) {
                            position = FindCharIndexInString(0x3D, (char *)token);
                            if (position >= 0) {
                                textureName = token + position + 1;
                                position = FindCharIndexInString(0x5D, (char *)token);
                                if (position >= 0) {
                                    *(char *)(token + position) = 0;
                                }
                                texNum = 0;
                                position = FindCharIndexInString(0x5B, (char *)token);
                                if (position >= 0) {
                                    texNum = atoi((const char *)(token + position + 1));
                                }
                                if (texNum >= 0 && texNum < rsd->count) {
                                    ((int *)(rsd->block))[texNum] =
                                        (int)GameHeapStrDup((char *)textureName);
                                } else {
                                    // STRING: FF7 0x009112b8
                                    sprintf(errorMsg, "ERROR: ILLEGAL TEXTURE NUMBER %d IN FILE %s \n",
                                            texNum, fileName);
                                    Gfx_DebugPrint(errorMsg);
                                    failed = 1;
                                    break;
                                }
                            }
                        } else {
                            // STRING: FF7 0x009112e8
                            sprintf(errorMsg, "ERROR: MISSING TIM FILE NAME IN FILE %s \n", fileName);
                            Gfx_DebugPrint(errorMsg);
                            failed = 1;
                            break;
                        }
                        tokenPosition++;
                    }
                }
            }
            rsd->meshCount = 1;
            rsd->meshes = (int)GameHeapAlloc(rsd->meshCount, 4,
                                                       // STRING: FF7 0x00911314
                                                       "C:\\lib\\src\\polygon\\plytopd.cpp", 0xA95);
            FreeTokenFile(tokenFile);
        } else {
            failed = 1;
        }
    }
    if (failed != 0) {
        // STRING: FF7 0x00911334
        GameHeapFree(rsd, "C:\\lib\\src\\polygon\\plytopd.cpp", 0xAA1);
        rsd = 0;
    }
    return (int)rsd;
}

/*
 * Frees a texture pair record and releases associated surfaces.
 */
// FUNCTION: FF7 0x006946cd
void __cdecl Pfile_FreeTexturePairRecord(PolygonTexturePair *pair)
{
    if (pair != 0) {
        Pfile_DestroyTim((PolygonTim *)(pair->tim));
        Gfx_ReleaseCachedTexture(pair->cachedTexture);
        ZeroFillMemory32(0xC, pair);
    }
}

/*
 * Loads texture referenced by an RSD record using load options.
 */
// FUNCTION: FF7 0x00694704
DxTextureResult __cdecl Pfile_LoadRsdTexture(PfileLoadOptions *options, char *fileName)
{
    char pathBuf[0xC8];
    int texPath;
    GfxTexHeader *tex;
    char baseName[0xC8];
    PolygonTim *timPtrPtr;
    int error;
    DxTextureResult loadRes;
    char msgBuffer[0xC8];

    if ((options->flags & 0x200) != 0) {
        texPath = (int)fileName;
    } else {
        texPath = (int)pathBuf;
        StripFileExtension(fileName, (char *)baseName);
        sprintf(pathBuf,
                // STRING: FF7 0x00911354
                "%s.%s", baseName,
                PlytopdLit_Tex());
    }
    ZeroFillMemory32(0xC, &loadRes);
    error = 0;
    if (options->sourceMode != 0) {
        tex = 0;
        timPtrPtr = (PolygonTim *)((int (__cdecl *)(int fileName))Pfile_LoadTim)((int)fileName); /* reads eax after a void call, as the original does */
        if (timPtrPtr != 0) {
            timPtrPtr->transparentToIndex0 = 1;
            if ((options->flags & 0x10) != 0) {
                timPtrPtr->transparentToIndex0 = 0;
            }
            timPtrPtr->flags = options->extraFlags;
            *(PolygonTimColorWord *)&timPtrPtr->maxBlue = *(PolygonTimColorWord *)&options->colorKey;
            tex = (GfxTexHeader *)(Pfile_BuildTextureFromTim(timPtrPtr, fileName));
            if ((options->flags & 0x20) != 0) {
                tex->modeFlag = 1;
            }
            if (Gfx_WriteTexFile((char *)texPath, tex) == 0) {
                error = 1;
            }
            Gfx_ReleaseCachedTexture((int)tex);
            tex = 0;
            tex = (GfxTexHeader *)(Gfx_LoadTexFileCached((int)&options->file, (const char *)texPath));
            loadRes.tim = (int)timPtrPtr;
            loadRes.texture = (int)tex;
        } else {
            error = 1;
        }
        loadRes.status = error;
    } else {
        loadRes.tim = 0;
        loadRes.texture = Gfx_LoadTexFileCached((int)&options->file, (const char *)texPath);
        if (loadRes.texture != 0) {
            loadRes.status = 0;
        } else {
            loadRes.status = 1;
        }
    }
    if (loadRes.status != 0) {
        sprintf(msgBuffer,
                // STRING: FF7 0x0091135c
                "ERROR: COULD NOT LOAD TEXTURE DATA FILE %s\n", fileName);
        Gfx_DebugPrint(msgBuffer);
    }
    return loadRes;
}

/*
 * Loads all textures, materials, and geometry referenced by an RSD file.
 */
// FUNCTION: FF7 0x00694942
int __cdecl Pfile_LoadRsdContents(PfileLoadOptions *options, float scale, PolygonRsdRecord *rsd)
{
    int ptr;
    int textureIndex;
    char baseBuffer[0xC8];
    char nameBuf[0xC8];
    int state;
    DxTextureResult texInformation;
    PolygonFileArrays *geometry;
    int flagBit;

    state = 1;
    if (rsd != 0) {
        state = 0;
        ptr = (int)nameBuf;
        if (state == 0) {
            for (textureIndex = 0; textureIndex < rsd->count; textureIndex++) {
                texInformation = Pfile_LoadRsdTexture(options,
                                         *(char **)&((int *)(rsd->block))[textureIndex]);
                if (texInformation.status != 0) {
                    state = 1;
                    break;
                }
                ((int *)(rsd->texRecordTable))[textureIndex] = texInformation.tim;
                ((int *)(rsd->textures))[textureIndex] = texInformation.texture;
            }
        }
        if (state == 0) {
            StripFileExtension((const char *)rsd->fileName, (char *)baseBuffer);
            sprintf(nameBuf,
                    // STRING: FF7 0x00911388
                    "%s.%s", baseBuffer,
                    PlytopdLit_P());
            flagBit = 0;
            if ((options->flags & 0x20000) != 0) {
                flagBit = 1;
            }
            if (options->sourceMode != 0) {
                geometry = (PolygonFileArrays *)(Pfile_BuildFromPlyMat(flagBit, 1.0f, rsd, 1, rsd->plyFileName,
                                          rsd->fileName, ptr));
            } else {
                geometry = Pfile_LoadBinary(&options->file, 0, ptr);
            }
            Pfile_FreeRsdMeshes(rsd);
            Pfile_ScaleVertices(scale, geometry);
            if (geometry != 0) {
                **(int **)&rsd->meshes = (int)geometry;
            } else {
                state = 1;
            }
        }
    }
    return state == 0;
}

/*
 * Blits an indexed color image onto a 16-bit surface.
 */
// FUNCTION: FF7 0x00694b5e
void __cdecl Pfile_BlitIndexedImageToSurface16(int destX, int destY, PolygonIndexedImage *image, PolygonSurface *surface)
{
    GfxEngineContext *ctx;
    int argb;
    int destinationPixel;
    int rowPointer;
    int column;
    int paletteIdx;
    int rowIndex;
    int pitch;
    int workUnpacked;
    int srcPointer;
    int mapped;

    if (image != 0 && image->bytesPerPixel == 1 && surface != 0) {
        ctx = GetEngineContext();
        pitch = surface->pitchBytes / 2;
        srcPointer = image->pixels;
        rowPointer = surface->pixelData;
        rowPointer += (destY * pitch + destX) * 2;
        for (rowIndex = 0; rowIndex < image->height; rowIndex++) {
            destinationPixel = rowPointer;
            for (column = 0; column < image->width; column++) {
                paletteIdx = *(unsigned char *)srcPointer;
                srcPointer++;
                workUnpacked = UnpackPixelToArgb8888(paletteIdx, (GfxPixelFormat *)((int)image + 0x3C));
                argb = workUnpacked;
                if (((unsigned char *)&argb)[3] != 0) {
                    mapped = Blit_MapArgb8888ViaRampTables(*(GfxArgb32 *)&argb, ctx);
                    *(short *)destinationPixel = (short)mapped;
                }
                destinationPixel += 2;
            }
            rowPointer += pitch * 2;
        }
    }
}

/*
 * Configures blend mode and alpha settings on a material hundred record.
 */
// FUNCTION: FF7 0x00694c80
void __cdecl Pfile_SetHundredBlendMode(int apply, int blendMode, PolygonHundredState *hundred)
{
    int blendRecord;

    if (hundred != 0) {
        if (blendMode != BLEND_KEEP) {
            hundred->blendMode = blendMode;
        } else {
            blendMode = hundred->blendMode;
        }
        hundred->changeMask |= GFX_STATE_FLAG_ALPHA_BLEND;
        hundred->flags |= GFX_STATE_FLAG_ALPHA_BLEND;
        switch (blendMode) {
        case BLEND_AVERAGE:
            hundred->alphaLevel = 0x80;
            hundred->srcBlend = D3DBLEND_SRCALPHA;
            hundred->dstBlend = D3DBLEND_SRCALPHA;
            hundred->dstBlend = D3DBLEND_INVSRCALPHA;
            break;
        case BLEND_ADDITIVE:
            hundred->alphaLevel = 0x80;
            hundred->srcBlend = D3DBLEND_SRCALPHA;
            hundred->dstBlend = D3DBLEND_ONE;
            break;
        case BLEND_SUBTRACTIVE:
            hundred->alphaLevel = 0xFF;
            hundred->srcBlend = D3DBLEND_INVSRCCOLOR;
            hundred->dstBlend = D3DBLEND_ONE;
            break;
        case BLEND_QUARTER_ADDITIVE:
            hundred->alphaLevel = 0x40;
            hundred->srcBlend = D3DBLEND_SRCALPHA;
            hundred->dstBlend = D3DBLEND_ONE;
            break;
        default:
            hundred->changeMask &= ~GFX_STATE_FLAG_ALPHA_BLEND;
            hundred->flags &= ~GFX_STATE_FLAG_ALPHA_BLEND;
            hundred->alphaLevel = 0xFF;
            hundred->srcBlend = D3DBLEND_ONE;
            hundred->dstBlend = D3DBLEND_ZERO;
        }
        if (apply != 0 && blendMode != BLEND_NONE) {
            blendRecord = Gfx_GetBlendModeRecordCall(blendMode, (void *)GetEngineContext());
            if (blendRecord != 0) {
                Gfx_ApplyBlendRecordToHundred((GfxBlendRecord *)blendRecord, hundred);
            } else {
                // STRING: FF7 0x00911390
                Gfx_DebugPrint("BLEND MODE NOT FOUND \n");
            }
        }
    }
}

/*
 * Constructs a material hundred record from material attributes.
 */
// FUNCTION: FF7 0x00694e05
int __cdecl Pfile_BuildHundredFromMaterial(PolygonMaterialRecord *material)
{
    int flagWord;
    PolygonHundredState *hundred;
    int mode;

    hundred = 0;
    if (material != 0) {
        hundred = (PolygonHundredState *)Gfx_AllocPHundred();
        if (hundred != 0) {
            flagWord = GFX_STATE_FLAG_SHADE_MODE | GFX_STATE_FLAG_TEXTURE;
            hundred->changeMask = flagWord;
            hundred->flags = 0;
            mode = BLEND_NONE;
            if ((material->flags & 4) != 0) {
                mode = (material->flags & 0x38) >> 3;
            }
            Pfile_SetHundredBlendMode(0, mode, hundred);
            switch (material->materialKind) {
            case PFILE_FACE_FLAT:
                hundred->flags |= GFX_STATE_FLAG_SHADE_MODE;
                hundred->shadeMode = D3DSHADE_FLAT;
                break;
            case PFILE_FACE_GOURAUD:
                hundred->flags |= GFX_STATE_FLAG_SHADE_MODE;
                hundred->shadeMode = D3DSHADE_GOURAUD;
                break;
            case PFILE_FACE_TEXTURED:
                hundred->flags |= GFX_STATE_FLAG_SHADE_MODE;
                hundred->shadeMode = D3DSHADE_FLAT;
                break;
            case PFILE_FACE_TEXTURED_FLAT:
                hundred->flags |= GFX_STATE_FLAG_SHADE_MODE;
                hundred->shadeMode = D3DSHADE_FLAT;
                break;
            case PFILE_FACE_TEXTURED_GOURAUD:
                hundred->flags |= GFX_STATE_FLAG_SHADE_MODE;
                hundred->shadeMode = D3DSHADE_GOURAUD;
                break;
            default:
                break;
            }
            switch (material->materialKind) {
            case PFILE_FACE_FLAT:
            case PFILE_FACE_GOURAUD:
                break;
            case PFILE_FACE_TEXTURED:
            case PFILE_FACE_TEXTURED_FLAT:
            case PFILE_FACE_TEXTURED_GOURAUD:
                hundred->flags |= GFX_STATE_FLAG_TEXTURE;
                hundred->textureIndex = material->textureIndex;
                hundred->texture = 0;
                hundred->hasTexturePage = material->hasTexturePage;
                hundred->texturePage = material->texturePage;
                hundred->clutId = material->clutId;
                if (REG_ALLOC_HINT) {
                }
                if (REG_ALLOC_HINT) {
                }
                hundred->flags |= GFX_STATE_FLAG_LINEAR_FILTER;
                hundred->flags |= GFX_STATE_FLAG_PERSPECTIVE;
                break;
            default:
                break;
            }
            hundred->clearedWord = 0;
            hundred->material = 0;
            if (REG_ALLOC_HINT) {
            }
            if (REG_ALLOC_HINT) {
            }
            hundred->flags |= GFX_STATE_FLAG_DITHER;
            hundred->flags |= GFX_STATE_FLAG_DEPTH_TEST;
            hundred->flags |= GFX_STATE_FLAG_DEPTH_WRITE;
            if ((material->flags & 2) != 0) {
                hundred->flags |= GFX_STATE_FLAG_NO_CULL;
            }
            hundred->ambientColor = -1;
        }
    }
    return (int)hundred;
}

/*
 * Equality comparator returning true if integers are equal.
 */
// FUNCTION: FF7 0x00695035
int __cdecl Pfile_CompareIntsEqual(int lhs, int rhs)
{
    return lhs == rhs;
}

/*
 * Constructs polygon groups sorted by material from mesh face records.
 */
// FUNCTION: FF7 0x00695047
void __cdecl Pfile_BuildGroupsFromFaces(int flags, void *pointData, int normalData, int faceCount, PolygonPlyMatState *matStates, int entryCount,
                                        PolygonPlyEntry *entries, PolygonFileArrays *fileArrays)
{
    List *bb;
    List *br;
    int bs;
    int bc;
    List *bd[5][5];
    int bu;
    int be;
    int bv;
    PolygonPlyEntry *bf;
    int bw;
    int bg;
    int **bx;
    int *bh;
    int *by;
    int *bi;
    int bz;
    int bj;
    int cg;
    PolygonFileGroup cx;
    int ch;
    int cy;
    int ci;
    PolygonFileGroup *cj;
    PolygonPlyMatState *ck;
    int dh;
    int cl;
    PolygonPlyMatState *cm;

#define p br
#define q bb
#define r bc
#define s bs
#define t bd
#define u be
#define v bf
#define w bu
#define x bv
#define y bg
#define z bw
#define aa bh
#define ab bz
#define ac bj
#define ad by
#define ae bx
#define ag ch
#define ah bi
#define ai cg
#define ak cx
#define al ci
#define am cy
#define an ck
#define ao cj
#define ap cl
#define aq dh
#define ar cm

    w = 0;
    p = CreateList(0);
    s = 0;
    for (x = 0; x < 5; x++) {
        for (r = 0; r < 5; r++) {
            q = CreateList(0);
            t[x][r] = q;
        }
    }
    v = entries;
    for (u = 0; u < faceCount; u++) {
        y = u;
        x = v->kind;
        r = v->transType;
        q = t[x][r];
        ListAppendValue(y, q);
        v++;
    }
    for (x = 0; x < 5; x++) {
        for (r = 0; r < 5; r++) {
            q = t[x][r];
            if (q != 0) {
                z = ListGetCount(q);
                if (z > 0) {
                    aa = (int *)ListToArray(0, 4, q);
                    if (aa != 0) {
                        ac = 0;
                        u = aa[ac];
                        v = entries + u;
                        if (v->hasTexture != 0) {
                            ab = 0;
                            for (ac = 0; ac < z; ac++) {
                                v = entries + aa[ac];
                                if (v->refIndex > ab) {
                                    ab = v->refIndex;
                                }
                            }
                            ab++;
                            // STRING: FF7 0x009113a8
                            ad = (int *)GameHeapAlloc(ab, 4, "C:\\lib\\src\\polygon\\plytopd.cpp", 0xD3B);
                            // STRING: FF7 0x009113c8
                            ae = (int **)GameHeapAlloc(ab, 4, "C:\\lib\\src\\polygon\\plytopd.cpp", 0xD3C);
                            if (ad != 0 && ae != 0) {
                                for (ac = 0; ac < z; ac++) {
                                    u = aa[ac];
                                    v = entries + u;
                                    ad[v->refIndex]++;
                                }
                                for (ac = 0; ac < ab; ac++) {
                                    ae[ac] = (int *)GameHeapAlloc(
                                        ad[ac], 4,
                                        // STRING: FF7 0x009113e8
                                        "C:\\lib\\src\\polygon\\plytopd.cpp", 0xD4C);
                                    ad[ac] = 0;
                                }
                                for (ac = 0; ac < z; ac++) {
                                    u = aa[ac];
                                    v = entries + u;
                                    ae[v->refIndex][ad[v->refIndex]] = u;
                                    ad[v->refIndex]++;
                                }
                            }
                            // STRING: FF7 0x00911408
                            GameHeapFree(aa, "C:\\lib\\src\\polygon\\plytopd.cpp", 0xD5C);
                        } else {
                            ab = 1;
                            // STRING: FF7 0x00911428
                            ad = (int *)GameHeapAlloc(ab, 4, "C:\\lib\\src\\polygon\\plytopd.cpp", 0xD62);
                            // STRING: FF7 0x00911448
                            ae = (int **)GameHeapAlloc(ab, 4, "C:\\lib\\src\\polygon\\plytopd.cpp", 0xD63);
                            if (ad != 0 && ae != 0) {
                                *ad = z;
                                *ae = aa;
                            }
                        }
                        if (ad != 0 && ae != 0) {
                            for (ac = 0; ac < ab; ac++) {
                                ag = ad[ac];
                                ah = ae[ac];
                                if (ag > 0 && ah != 0) {
                                    u = *ah;
                                    v = entries + u;
                                    am = 0;
                                    for (al = 0; al < ag; al++) {
                                        u = ah[al];
                                        an = matStates + u;
                                        if (an->isPolygon != 0) {
                                            if (an->isQuad != 0) {
                                                am += 2;
                                            } else {
                                                am++;
                                            }
                                        }
                                    }
                                    ao = &ak;
                                    memset(ao, 0, 0x38);
                                    if (fileArrays->editSource != 0) {
                                        ao->kind = x;
                                        ao->firstPolygon = fileArrays->editSource->polygons->count;
                                        ao->polygonCount = am;
                                        ao->firstPosition = fileArrays->editSource->positions->count;
                                        ao->vertexCount =
                                            ao->polygonCount * 3;
                                        ao->firstTexCoord = fileArrays->editSource->texCoords->count;
                                        ao->hasTexture = v->hasTexture;
                                        ao->refIndex = v->refIndex;
                                        Pfile_AddGroup(ao, fileArrays);
                                    }
                                    ai = Pfile_BuildHundredFromMaterial((PolygonMaterialRecord *)v);
                                    if (ai != 0) {
                                        Pfile_AddHundred((void *)ai, fileArrays);
                                        ap = 0;
                                        for (aq = 0; aq < ag; aq++) {
                                            u = ah[aq];
                                            v = entries + u;
                                            ar = matStates + u;
                                            if (ar->isPolygon != 0) {
                                                Pfile_EmitTriangle(ap, 0, (PolygonTranslation *)pointData, normalData, v, ar, fileArrays);
                                                ap++;
                                                if (ar->isQuad != 0) {
                                                    Pfile_EmitTriangle(ap, 1, (PolygonTranslation *)pointData, normalData, v, ar, fileArrays);
                                                    ap++;
                                                }
                                            }
                                        }
                                        Gfx_FreePHundred(ai);
                                    }
                                }
                            }
                        }
                        if (ae != 0) {
                            for (ac = 0; ac < ab; ac++) {
                                GameHeapFree(ae[ac],
                                             // STRING: FF7 0x00911468
                                             "C:\\lib\\src\\polygon\\plytopd.cpp", 0xDE1);
                            }
                        }
                        // STRING: FF7 0x00911488
                        GameHeapFree(ad, "C:\\lib\\src\\polygon\\plytopd.cpp", 0xDE5);
                        // STRING: FF7 0x009114a8
                        GameHeapFree(ae, "C:\\lib\\src\\polygon\\plytopd.cpp", 0xDE6);
                    }
                }
            }
        }
    }
    for (x = 0; x < 5; x++) {
        for (r = 0; r < 5; r++) {
            q = t[x][r];
            DestroyList(q);
        }
    }
    DestroyList(p);
    // STRING: FF7 0x009114c8
    GameHeapFree((void *)s, "C:\\lib\\src\\polygon\\plytopd.cpp", 0xDF8);

#undef p
#undef q
#undef r
#undef s
#undef t
#undef u
#undef v
#undef w
#undef x
#undef y
#undef z
#undef aa
#undef ab
#undef ac
#undef ad
#undef ae
#undef ag
#undef ah
#undef ai
#undef ak
#undef al
#undef am
#undef an
#undef ao
#undef ap
#undef aq
#undef ar
}

/*
 * Initializes an animation placement transform record.
 */
// FUNCTION: FF7 0x00695945
void __cdecl AnimPlacement_Init(PolygonAnimPlacement *placement)
{
    ZeroFillMemory32(0x74, placement);
    placement->flags = 0;
    placement->offsetX = 0;
    placement->offsetY = 0;
    placement->offsetZ = 0;
    placement->angleX = 0;
    placement->angleY = 0;
    placement->angleZ = 0;
    placement->scaleX = FLOAT_ONE_BITS;
    placement->scaleY = FLOAT_ONE_BITS;
    placement->scaleZ = FLOAT_ONE_BITS;
    placement->scale = FLOAT_ONE_BITS;
    SetIdentityMatrix4x4((char *)placement + 0x2C);
}

/*
 * Destroys an animation player and releases bound resources.
 */
// FUNCTION: FF7 0x006959d4
void __cdecl AnimPlayer_Destroy(PolygonAnimPlayer *player)
{
    if (player != 0) {
        if (player->animData != 0) {
            Anim_Free((PolygonAnimBuffers *)(player->animData));
        }
        if (player->skeleton != 0) {
            Skel_Free((PolygonSkelHierarchy *)(player->skeleton));
        }
        // STRING: FF7 0x009114e8
        GameHeapFree(player, "C:\\lib\\src\\polygon\\plytopd.cpp", 0xE21);
    }
}

/*
 * Creates a self-timed animation player for `skeleton` and returns it (0xE4 bytes), or 0. The
 * first parameter (`frameCount`) is the playback rate in frames per second (at least 1): the
 * frame interval, the engine context's timestamp-counter frequency (`clip` is the engine
 * context) divided by it, is stored at +0xBC and the first deadline, now plus one interval, at
 * +0xC4, which `AnimPlayer_Tick` compares against. The player starts active and looping at frame
 * 0 with step 1, speed 1.0, no animation, a reset placement (+0x38, `AnimPlacement_Init`), and a
 * disk read request (+0xD4) for the animations it loads. `loadInfo` is the animation sequence
 * `AnimPlayer_SelectAnimation` steps through.
 */
// FUNCTION: FF7 0x00695a2b
int __cdecl AnimPlayer_Create(int frameCount, PolygonSkeletonLoadSpec *loadInfo, PolygonSkelHierarchy *skeleton, int clip)
{
    PolygonAnimPlayer *player;

    // STRING: FF7 0x00911508
    player = (PolygonAnimPlayer *)GameHeapAlloc(1, 0xE4, "C:\\lib\\src\\polygon\\plytopd.cpp", 0xE29);
    if (player != 0) {
        player->isActive = 1;
        player->startFrame = 0;
        player->frameCount = 0;
        player->frameStep = 1;
        player->tickEnabled = 1;
        player->isBound = 1;
        player->holdTimer = 1;
        player->holdTimerStart = 1;
        if (frameCount <= 0) {
            frameCount = 1;
        }
        player->totalFrames = frameCount;
        player->speed = FLOAT_ONE_BITS;
        player->animData = 0;
        AnimPlacement_Init((PolygonAnimPlacement *)((int)player + 0x38));
        player->loadInfo = (int)loadInfo;
        player->skeleton = (int)skeleton;
        player->clip = clip;
        SplitFloatToSplitValue(((GfxEngineContext *)clip)->tscFrequency / (double)player->totalFrames, (struct Timestamp64 *)((int)player + 0xBC));
        ReadTimestampCounter((struct Timestamp64 *)((char *)player + 0xC4));
        Add64((struct Timestamp64 *)((int)player + 0xBC), (struct Timestamp64 *)((int)player + 0xC4));
        InitFileContext(0, (char *)player + 0xD4);
    }
    return (int)player;
}

/*
 * Rewinds animation player to initial frame and reset step parameters.
 */
// FUNCTION: FF7 0x00695b6d
void __cdecl AnimPlayer_Rewind(AnimPlayerInstance *player)
{
    if (player != 0) {
        player->frame = player->startFrame;
        player->time = player->startTime;
    }
}

/*
 * Advances animation playback state based on elapsed timer delta.
 */
// FUNCTION: FF7 0x00695b90
void __cdecl AnimPlayer_Tick(void *timestamp, PolygonAnimPlayer *player)
{
    int needsDraw;
    struct Timestamp64 *clockPtr;
    struct Timestamp64 nowStamp;
    int flag;
    struct Timestamp64 dt;

    needsDraw = 0;
    if (player != 0 && player->isActive != 0) {
        if (timestamp != 0) {
            clockPtr = (struct Timestamp64 *)timestamp;
        } else {
            clockPtr = (struct Timestamp64 *)&nowStamp;
            ReadTimestampCounter((struct Timestamp64 *)clockPtr);
        }
        if (Compare64((struct Timestamp64 *)clockPtr, (struct Timestamp64 *)((int)player + 0xC4)) >= 0) {
            Add64((struct Timestamp64 *)((int)player + 0xBC), (struct Timestamp64 *)((int)player + 0xC4));
            if (player->tickEnabled != 0) {
                flag = 1;
                if (player->isPlaying == 0) {
                    player->frame += player->frameStep;
                }
                Sub64((struct Timestamp64 *)clockPtr, (struct Timestamp64 *)((int)player + 0xC4), (struct Timestamp64 *)&dt);
                if ((double)(unsigned int)dt.lo > (GetEngineContext())->tscFrequency) {
                    flag = 0;
                    *(struct Timestamp64 *)((char *)player + 0xC4) = *(struct Timestamp64 *)timestamp;
                    Add64((struct Timestamp64 *)((int)player + 0xBC), (struct Timestamp64 *)((int)player + 0xC4));
                }
                if (flag != 0) {
                    while (Compare64((struct Timestamp64 *)clockPtr, (struct Timestamp64 *)((int)player + 0xC4)) >= 0) {
                        Add64((struct Timestamp64 *)((int)player + 0xBC), (struct Timestamp64 *)((int)player + 0xC4));
                        if (player->isPlaying == 0) {
                            player->frame += player->frameStep;
                        } else {
                            break;
                        }
                    }
                }
            } else {
                if (Compare64((struct Timestamp64 *)clockPtr, (struct Timestamp64 *)((int)player + 0xC4)) >= 0) {
                    *(struct Timestamp64 *)((char *)player + 0xC4) = *(struct Timestamp64 *)timestamp;
                    Add64((struct Timestamp64 *)((int)player + 0xBC), (struct Timestamp64 *)((int)player + 0xC4));
                }
                if (player->isPlaying == 0) {
                    player->frame += player->frameStep;
                }
            }
            if (player->frame >= player->frameCount) {
                if (player->holdLastFrame != 0) {
                    player->frame = player->frameCount - 1;
                    needsDraw = 1;
                } else {
                    player->frame = player->startFrame;
                    if (player->isBound != 0) {
                        needsDraw = 1;
                    } else if (player->holdTimer > 0) {
                        player->holdTimer -= 1;
                        needsDraw = 1;
                    } else if (player->endCallback != 0) {
                        needsDraw = (*(int(__cdecl **)(int))((char *)player + 0xD0))((int)player);
                    } else {
                        Skel_SetPartsVisible(0, (PolygonSkelHierarchy *)(player->skeleton));
                    }
                }
            } else {
                needsDraw = 1;
            }
        } else {
            needsDraw = 1;
        }
        if (needsDraw != 0) {
            Anim_DrawSkeletonFrame(player->frame, (PolygonSkeletonAnim *)player->animData,
                                   (PolygonBoneTransform *)((int)player + 0x38), (PolygonAnimDraw *)(player->skeleton),
                                   (GfxEngineContext *)player->clip);
        }
    }
}

/*
 * Binds an animation asset to an animation player.
 */
// FUNCTION: FF7 0x00695e6a
void __cdecl AnimPlayer_BindAnimation(int unused, int anim, PolygonAnimPlayer *player)
{
    if (anim != 0 && player != 0) {
        player->animData = anim;
        player->startFrame = 0;
        player->frameCount = ((PolygonAnimation *)anim)->frameCount;
        player->isBound = 1;
        player->holdTimer = 1;
        player->holdTimerStart = 1;
    }
}

/*
 * Selects and activates an animation index in the player.
 */
// FUNCTION: FF7 0x00695eb8
void __cdecl AnimPlayer_SelectAnimation(int time, int delta, int bake, AnimPlayerInstance *player)
{
    PolygonAnimSequence *pSequenceInfo;
    int frameName;
    PolygonAnimation *pLoaded;
    char baseName[0xC8];
    char outPath[0xC8];
    int dirChanged;
    char prevDir[0xC8];

    if (player != 0) {
        pSequenceInfo = (PolygonAnimSequence *)player->animation;
        if (pSequenceInfo != 0) {
            if (delta != 0) {
                player->playTime += delta;
                if (player->playTime < 0) {
                    player->playTime += pSequenceInfo->stepCount;
                }
                if (player->playTime >= pSequenceInfo->stepCount) {
                    player->playTime -= pSequenceInfo->stepCount;
                }
            } else {
                player->playTime = time;
                if (player->playTime < 0) {
                    player->playTime = 0;
                }
                if (player->playTime >= pSequenceInfo->stepCount) {
                    player->playTime = 0;
                }
            }
            if (((PolygonAnimSequence *)player->animation)->fileNames != 0) {
                frameName = ((int *)pSequenceInfo->fileNames)[player->playTime];
                if (frameName != 0) {
                    if (player->animData != 0) {
                        Anim_Free((PolygonAnimBuffers *)player->animData);
                        player->animData = 0;
                    }
                    dirChanged = 0;
                    if (pSequenceInfo->directory != 0) {
                        dirChanged = ChangeDirectorySaving((char *)pSequenceInfo->directory, (char *)prevDir);
                    }
                    StripFileExtension((const char *)frameName, (char *)baseName);
                    sprintf(outPath,
                            // STRING: FF7 0x00911528
                            "%s.%s", baseName,
                            PlytopdLit_A());
                    if (Anim_CompileTextToBinary(frameName, (int)outPath) != 0) {
                        pLoaded = (PolygonAnimation *)(Anim_LoadBinary((FileOpenRequest *)((int)player + 0xD4), (int)outPath));
                        if (pLoaded != 0) {
                            player->animData = (int)pLoaded;
                            player->frameCount = pLoaded->frameCount;
                            AnimPlayer_Rewind(player);
                            Anim_ScaleRootTranslation(player->rootScale, pLoaded);
                            if (bake != 0) {
                                Anim_BakeAllFrameMatrices((PolygonAnimBake *)(player->animData),
                                                          (PolygonAnimModel *)player->model);
                            }
                        }
                    }
                    if (pSequenceInfo->directory != 0 && dirChanged != 0) {
                        ChangeDirectorySaving((char *)prevDir, 0);
                    }
                }
            }
        }
    }
}

/*
 * Adds translation vector to placement position offset.
 */
// FUNCTION: FF7 0x0069610a
void __cdecl AnimPlacement_AddTranslation(PolygonTranslation *offset, PolygonAnimPlacement *placement)
{
    if (placement != 0) {
        placement->translateX += offset->x;
        placement->translateY += offset->y;
        placement->translateZ += offset->z;
    }
}

/*
 * Returns the X rotation angle from the placement record.
 */
// FUNCTION: FF7 0x0069614a
float __cdecl AnimPlacement_GetAngleX(PolygonAnimAngles *placement)
{
    float angle;

    angle = 0.0f;
    if (placement != 0) {
        angle = placement->angleX;
    }
    return angle;
}

/*
 * Returns the Y rotation angle from the placement record.
 */
// FUNCTION: FF7 0x0069616b
float __cdecl AnimPlacement_GetAngleY(PolygonAnimAngles *placement)
{
    float angle;

    angle = 0.0f;
    if (placement != 0) {
        angle = placement->angleY;
    }
    return angle;
}

/*
 * Returns the Z rotation angle from the placement record.
 */
// FUNCTION: FF7 0x0069618c
float __cdecl AnimPlacement_GetAngleZ(PolygonAnimAngles *placement)
{
    float angle;

    angle = 0.0f;
    if (placement != 0) {
        angle = placement->angleZ;
    }
    return angle;
}

/*
 * Returns the rotation angle about `axis`.
 */
// FUNCTION: FF7 0x006961ad
float __cdecl AnimPlacement_GetAngleByIndex(int axis, PolygonAnimAngles *placement)
{
    float angle;

    angle = 0.0f;
    if (placement != 0) {
        switch (axis) {
        case ANIM_AXIS_X:
            angle = placement->angleX;
            break;
        case ANIM_AXIS_Y:
            angle = placement->angleY;
            break;
        case ANIM_AXIS_Z:
            angle = placement->angleZ;
        }
    }
    return angle;
}

/*
 * Sets the X rotation angle on the placement record.
 */
// FUNCTION: FF7 0x00696200
void __cdecl AnimPlacement_SetAngleX(int angle, PolygonAnimAngleSlots *placement)
{
    if (placement != 0) {
        placement->angleX = angle;
    }
}

/*
 * Sets the Y rotation angle on the placement record.
 */
// FUNCTION: FF7 0x00696214
void __cdecl AnimPlacement_SetAngleY(int angle, PolygonAnimAngleSlots *placement)
{
    if (placement != 0) {
        placement->angleY = angle;
    }
}

/*
 * Sets the Z rotation angle on the placement record.
 */
// FUNCTION: FF7 0x00696228
void __cdecl AnimPlacement_SetAngleZ(int angle, PolygonAnimAngleSlots *placement)
{
    if (placement != 0) {
        placement->angleZ = angle;
    }
}

/*
 * Sets the rotation angle about `axis`.
 */
// FUNCTION: FF7 0x0069623c
void __cdecl AnimPlacement_SetAngleByIndex(int axis, int angle, PolygonAnimAngleSlots *placement)
{
    if (placement != 0) {
        switch (axis) {
        case ANIM_AXIS_X:
            placement->angleX = angle;
            break;
        case ANIM_AXIS_Y:
            placement->angleY = angle;
            break;
        case ANIM_AXIS_Z:
            placement->angleZ = angle;
        }
    }
}

/*
 * Returns the scale factor from the placement record.
 */
// FUNCTION: FF7 0x00696283
float __cdecl AnimPlacement_GetScale(PolygonPlacement *placement)
{
    float scale;

    scale = 1.0f;
    if (placement != 0) {
        scale = *(float *)&placement->scale;
    }
    return scale;
}

/*
 * Sets uniform scale factor on an animation placement record.
 */
// FUNCTION: FF7 0x006962a4
void __cdecl AnimPlacement_SetScale(int scale, PolygonPlacement *placement)
{
    if (placement != 0) {
        placement->scale = scale;
        if (scale == FLOAT_ONE_BITS) {
            placement->flags &= 0xFFFFFFBF;
        } else {
            placement->flags |= 0x40;
        }
    }
}

/*
 * Loads skeleton HRC data and constructs its hierarchy nodes.
 */
// FUNCTION: FF7 0x006962df
int __cdecl Skel_LoadHrcAndBuild(struct PolygonSkeletonLoadSpec *skelLoad, int model, struct GfxEngineContext *context)
{
    int flags;
    PolygonSkelHierarchy *pHierarchy;

    flags = 0;
    pHierarchy = (PolygonSkelHierarchy *)(Skel_LoadHrc(model + 0x40, flags, skelLoad->fileName));
    if (pHierarchy != 0) {
        Skel_BuildHierarchyAndLoadParts(skelLoad->dirName, pHierarchy, (PfileLoadOptions *)model, (int)context);
        Skel_SetPartsVisible(1, pHierarchy);
    }
    return (int)pHierarchy;
}

/*
 * Destroys an animation group entry and frees member assets.
 */
// FUNCTION: FF7 0x0069633e
void __cdecl AnimGroupEntry_Destroy(PolygonAnimGroupEntry *entry)
{
    if (entry != 0) {
        AnimPlayer_Destroy((PolygonAnimPlayer *)entry->animPlayer);
        // STRING: FF7 0x00911530
        GameHeapFree((void *)entry->name, "C:\\lib\\src\\polygon\\plytopd.cpp", 0x1001);
        // STRING: FF7 0x00911550
        GameHeapFree((void *)entry->block, "C:\\lib\\src\\polygon\\plytopd.cpp", 0x1002);
        // STRING: FF7 0x00911570
        GameHeapFree((void *)entry->directory, "C:\\lib\\src\\polygon\\plytopd.cpp", 0x1003);
        // STRING: FF7 0x00911590
        GameHeapFree((void *)entry->extension, "C:\\lib\\src\\polygon\\plytopd.cpp", 0x1004);
        // STRING: FF7 0x009115b0
        GameHeapFree((void *)entry->entries, "C:\\lib\\src\\polygon\\plytopd.cpp", 0x1005);
        FreeDirectoryListing((FileDirectoryListing *)(entry->directoryListing));
        // STRING: FF7 0x009115d0
        GameHeapFree(entry, "C:\\lib\\src\\polygon\\plytopd.cpp", 0x1008);
    }
}

/*
 * Allocates a new animation group entry record.
 */
// FUNCTION: FF7 0x006963fd
int __cdecl AnimGroupEntry_Create(void)
{
    PolygonAnimGroupEntry *rec;

    // STRING: FF7 0x009115f0
    rec = (PolygonAnimGroupEntry *)GameHeapAlloc(1, 0x24, "C:\\lib\\src\\polygon\\plytopd.cpp", 0x1010);
    return (int)rec;
}

/*
 * Destroys an animation group and frees all member entries.
 */
// FUNCTION: FF7 0x00696421
void __cdecl AnimGroup_Destroy(PolygonAnimGroup *group)
{
    int index;

    if (group != 0) {
        if (group->entries != 0) {
            for (index = 0; index < group->count; index++) {
                AnimGroupEntry_Destroy(group->entries[index]);
            }
            // STRING: FF7 0x00911610
            GameHeapFree(group->entries, "C:\\lib\\src\\polygon\\plytopd.cpp", 0x1028);
        }
        // STRING: FF7 0x00911630
        GameHeapFree(group, "C:\\lib\\src\\polygon\\plytopd.cpp", 0x102B);
    }
}

/*
 * Loads and parses an animation group from a descriptor file.
 */
// FUNCTION: FF7 0x0069649b
int __cdecl AnimGroup_LoadFromTextFile(int fileName)
{
    PolygonAnimGroupText *group;
    PolygonTokenFile *tokens;
    int tokenPos;
    PolygonAnimGroupEntry *entry;
    int i;
    char name[0xC8];
    FileDirectoryListing *listingPointer;

    group = 0;
    tokens = TokenizeTextFileFromDisk(0, (void *)fileName);
    if (tokens != 0) {
        tokenPos = 0;
        // STRING: FF7 0x00911650
        group = (PolygonAnimGroupText *)GameHeapAlloc(1, 0xC, "C:\\lib\\src\\polygon\\plytopd.cpp", 0x103C);
        if (group != 0) {
            group->count = tokens->tokenCount / 5;
            if (group->count > 0) {
                group->entries = (PolygonAnimGroupEntry ** )GameHeapAlloc(group->count, 4,
                                                        // STRING: FF7 0x00911670
                                                        "C:\\lib\\src\\polygon\\plytopd.cpp", 0x1042);
                if (group->entries != 0) {
                    for (i = 0; i < group->count; i++) {
                        entry = (PolygonAnimGroupEntry *)AnimGroupEntry_Create();
                        if (entry != 0) {
                            group->entries[i] = entry;
                            entry->name = (int)GameHeapStrDup(TokenAsString(tokenPos, tokens));
                            tokenPos++;
                            entry->block = (int)GameHeapStrDup(TokenAsString(tokenPos, tokens));
                            tokenPos++;
                            entry->directory = (int)GameHeapStrDup(TokenAsString(tokenPos, tokens));
                            tokenPos++;
                            entry->extension = (int)GameHeapStrDup(TokenAsString(tokenPos, tokens));
                            tokenPos++;
                            entry->parsedParam = atoi(TokenAsString(tokenPos, tokens));
                            tokenPos++;
                            // STRING: FF7 0x00911690
                            sprintf(name, "%s\\*.%s", entry->directory,
                                    entry->extension);
                            listingPointer = (FileDirectoryListing *)(BuildDirectoryListing(0, name));
                            if (listingPointer != 0) {
                                if (listingPointer->entryCount > 0) {
                                    entry->entryCount = listingPointer->entryCount;
                                    entry->entries = (int)GameHeapAlloc(listingPointer->entryCount, 4,
                                                                               // STRING: FF7 0x00911698
                                                                               "C:\\lib\\src\\polygon\\plytopd.cpp",
                                                                               0x1066);
                                    entry->directoryListing = (int)listingPointer;
                                    SortDirectoryListingNames(listingPointer);
                                    FillDirectoryEntryPointerArray(entry->entries, listingPointer);
                                } else {
                                    FreeDirectoryListing(listingPointer);
                                }
                            }
                        }
                    }
                }
            }
        }
        FreeTokenFile(tokens);
    }
    return (int)group;
}

/*
 * Advances current entry index in an animation group.
 */
// FUNCTION: FF7 0x0069671f
void __cdecl AnimGroup_AdvanceCurrentIndex(int delta, PolygonAnimGroup *group)
{
    if (group != 0) {
        group->currentIndex += delta;
        if (group->currentIndex < 0) {
            group->currentIndex += group->count;
        }
        if (group->currentIndex >= group->count) {
            group->currentIndex -= group->count;
        }
    }
}

/*
 * Formats status information string for the active group entry.
 */
// FUNCTION: FF7 0x0069676c
void __cdecl AnimGroup_FormatCurrentEntryStatusLine(int isClosed, int xPos, int yPos, int color,
                                                    void(__cdecl *drawTextFn)(int, int, char *, int), PolygonAnimGroup *group)
{
    int p;
    PolygonAnimPlayer *q;
    PolygonAnimGroupEntry *r;
    int s;
    char t[0xc8];
    char *u;
    char v;
    int w;
    int x;

    if (group != 0) {
        p = group->currentIndex;
        if (p >= 0) {
            if (p < group->count) {
                r = group->entries[p];
                if (r != 0) {
                    q = r->animPlayer;
                    if (drawTextFn != 0) {
                        s = 0;
                        x = 0;
                        w = 0;
                        u = (char *)&g_PlytopdEmptyAnimName;
                        if (q != 0) {
                            s = q->frameCount;
                            x = q->frame;
                            w = q->totalFrames;
                            u = ((char **)r->entries)[q->animFileIndex];
                        }
                        if (isClosed != 0) {
                            v = 'C';
                        } else {
                            v = 'O';
                        }
                        sprintf(t,
                                // STRING: FF7 0x009116b8
                                "%c %03d %03d %02d %d %s  ANM: %s", v, x, s, w, p, (char *)r->name, u);
                        drawTextFn(xPos, yPos, t, color);
                    }
                }
            }
        }
    }
}

/*
 * Returns the currently active entry in an animation group.
 */
// FUNCTION: FF7 0x006968a2
PolygonAnimGroupEntry *__cdecl AnimGroup_GetCurrentEntry(PolygonAnimGroup *group)
{
    PolygonAnimGroupEntry *entry;

    if (group != 0) {
        entry = group->entries[group->currentIndex];
    }
    return entry;
}

/*
 * Loads a model with its skeleton and first animation and returns an animation player for it,
 * or 0; field, the minigames and the submarine use it to load an entity's skeleton when it has
 * none. `Skel_LoadHrcAndBuild` loads the `.HRC` named in `skelLoad` with load options `model`
 * and loads its parts into `context`. `AnimPlayer_Create` makes the player at rate `kind`
 * (frames per second), the options' `scale` becomes the player's root scale, and
 * `AnimPlayer_SelectAnimation` loads the animation of step `time` of the sequence `skelLoad`
 * describes. Returns 0 when the skeleton does not load; a failed player leaves the skeleton
 * allocated.
 */
// FUNCTION: FF7 0x006968c4
int __cdecl Skel_LoadHrcAndCreatePlayer(int kind, int time, PolygonSkeletonLoadSpec *skelLoad, int model, struct GfxEngineContext *context)
{
    AnimPlayerInstance *player;
    PolygonSkelHierarchy *hierarchy;

    player = 0;
    hierarchy = (PolygonSkelHierarchy *)Skel_LoadHrcAndBuild(skelLoad, model, context);
    if (hierarchy != 0) {
        player = (AnimPlayerInstance *)AnimPlayer_Create(kind, skelLoad, hierarchy, (int)context);
        if (player != 0) {
            *(int *)&player->rootScale = *(int *)&((PfileLoadOptions *)model)->scale;
            AnimPlayer_SelectAnimation(time, 0, 0, player);
        }
    }
    return (int)player;
}

/*
 * Advances all animation players in an animation group.
 */
// FUNCTION: FF7 0x00696936
void __cdecl AnimPlayer_TickGroup(PolygonAnimGroup *group)
{
    int idx;
    int tickValue[2];
    PolygonAnimPlayer *player;
    PolygonAnimGroupEntry *entry;

    if (group != 0) {
        ReadTimestampCounter((struct Timestamp64 *)tickValue);
        for (idx = 0; idx < group->count; idx++) {
            entry = group->entries[idx];
            if (entry != 0) {
                player = entry->animPlayer;
                if (player != 0 && player->isActive != 0) {
                    AnimPlayer_Tick(tickValue, (PolygonAnimPlayer *)player);
                }
            }
        }
    }
}

/*
 * Toggles playback pause state for active group animation player.
 */
// FUNCTION: FF7 0x006969ad
void __cdecl AnimGroup_ToggleCurrentPlayerPause(PolygonAnimGroup *group)
{
    PolygonAnimPlayer *player;
    PolygonAnimGroupEntry *entry;

    entry = AnimGroup_GetCurrentEntry(group);
    if (entry != 0) {
        player = entry->animPlayer;
        if (player != 0) {
            player->isPlaying ^= 1;
        }
    }
}

/*
 * Translates placement position of current group animation player.
 */
// FUNCTION: FF7 0x006969ea
void __cdecl AnimGroup_TranslateCurrentPlayer(PolygonTranslation *offset, PolygonAnimGroup *group)
{
    PolygonAnimGroupEntry *entry;

    entry = AnimGroup_GetCurrentEntry(group);
    if (entry != 0) {
        AnimPlacement_AddTranslation(offset, (PolygonAnimPlacement *)entry->animPlayer);
    }
}

/*
 * Returns X rotation angle for current group animation player.
 */
// FUNCTION: FF7 0x00696a1a
float __cdecl AnimGroup_GetCurrentPlayerFloat48(PolygonAnimGroup *group)
{
    PolygonAnimGroupEntry *ent;
    float angle;

    angle = 0.0f;
    ent = AnimGroup_GetCurrentEntry(group);
    if (ent != 0) {
        angle = AnimPlacement_GetAngleX((PolygonAnimAngles *)ent->animPlayer);
    }
    return angle;
}

/*
 * Returns Y rotation angle for current group animation player.
 */
// FUNCTION: FF7 0x00696a55
float __cdecl AnimGroup_GetCurrentPlayerFloat4C(PolygonAnimGroup *group)
{
    PolygonAnimGroupEntry *ent;
    float angle;

    angle = 0.0f;
    ent = AnimGroup_GetCurrentEntry(group);
    if (ent != 0) {
        angle = AnimPlacement_GetAngleY((PolygonAnimAngles *)ent->animPlayer);
    }
    return angle;
}

/*
 * Returns Z rotation angle for current group animation player.
 */
// FUNCTION: FF7 0x00696a90
float __cdecl AnimGroup_GetCurrentPlayerFloat50(PolygonAnimGroup *group)
{
    PolygonAnimGroupEntry *ent;
    float angle;

    angle = 0.0f;
    ent = AnimGroup_GetCurrentEntry(group);
    if (ent != 0) {
        angle = AnimPlacement_GetAngleZ((PolygonAnimAngles *)ent->animPlayer);
    }
    return angle;
}

/*
 * Returns rotation angle by axis index for active group player.
 */
// FUNCTION: FF7 0x00696acb
float __cdecl AnimGroup_GetCurrentPlayerFloatByIndex(int axis, PolygonAnimGroup *group)
{
    PolygonAnimGroupEntry *ent;
    float angle;

    angle = 0.0f;
    ent = AnimGroup_GetCurrentEntry(group);
    if (ent != 0) {
        angle = AnimPlacement_GetAngleByIndex(axis, (PolygonAnimAngles *)ent->animPlayer);
    }
    return angle;
}

/*
 * Sets X rotation angle on active group animation player.
 */
// FUNCTION: FF7 0x00696b0a
void __cdecl AnimGroup_SetCurrentPlayerAngleX(int angle, PolygonAnimGroup *group)
{
    PolygonAnimGroupEntry *entry;

    entry = AnimGroup_GetCurrentEntry(group);
    if (entry != 0) {
        AnimPlacement_SetAngleX(angle, (PolygonAnimAngleSlots *)entry->animPlayer);
    }
}

/*
 * Sets Y rotation angle on active group animation player.
 */
// FUNCTION: FF7 0x00696b3a
void __cdecl AnimGroup_SetCurrentPlayerAngleY(int angle, PolygonAnimGroup *group)
{
    PolygonAnimGroupEntry *entry;

    entry = AnimGroup_GetCurrentEntry(group);
    if (entry != 0) {
        AnimPlacement_SetAngleY(angle, (PolygonAnimAngleSlots *)entry->animPlayer);
    }
}

/*
 * Sets Z rotation angle on active group animation player.
 */
// FUNCTION: FF7 0x00696b6a
void __cdecl AnimGroup_SetCurrentPlayerAngleZ(int angle, PolygonAnimGroup *group)
{
    PolygonAnimGroupEntry *entry;

    entry = AnimGroup_GetCurrentEntry(group);
    if (entry != 0) {
        AnimPlacement_SetAngleZ(angle, (PolygonAnimAngleSlots *)entry->animPlayer);
    }
}

/*
 * Sets rotation angle by axis index on active group player.
 */
// FUNCTION: FF7 0x00696b9a
void __cdecl AnimGroup_SetCurrentPlayerAngleByAxis(int axis, int angle, PolygonAnimGroup *group)
{
    PolygonAnimGroupEntry *entry;

    entry = AnimGroup_GetCurrentEntry(group);
    if (entry != 0) {
        AnimPlacement_SetAngleByIndex(axis, angle, (PolygonAnimAngleSlots *)entry->animPlayer);
    }
}

/*
 * Returns scale factor of active group animation player.
 */
// FUNCTION: FF7 0x00696bce
float __cdecl AnimGroup_GetCurrentPlayerScale(PolygonAnimGroup *group)
{
    float value;
    PolygonAnimGroupEntry *currentEntry;

    value = 1.0f;
    currentEntry = AnimGroup_GetCurrentEntry(group);
    if (currentEntry != 0) {
        value = AnimPlacement_GetScale((PolygonPlacement *)currentEntry->animPlayer);
    }
    return value;
}

/*
 * Sets scale factor on active group animation player.
 */
// FUNCTION: FF7 0x00696c09
void __cdecl AnimGroup_SetCurrentPlayerScale(int scale, PolygonAnimGroup *group)
{
    PolygonAnimGroupEntry *entry;

    entry = AnimGroup_GetCurrentEntry(group);
    if (entry != 0) {
        AnimPlacement_SetScale(scale, (PolygonPlacement *)entry->animPlayer);
    }
}

/*
 * Clears placement state flags across all group player entries.
 */
// FUNCTION: FF7 0x00696c39
void __cdecl AnimGroup_ResetPlayerFrames(PolygonAnimGroup *group)
{
    int idx;
    PolygonAnimPlayer *placement;
    PolygonAnimGroupEntry *entry;

    if (group != 0) {
        for (idx = 0; idx < group->count; idx++) {
            entry = group->entries[idx];
            if (entry != 0) {
                placement = entry->animPlayer;
                if (placement != 0) {
                    placement->frame = 0;
                }
            }
        }
    }
}

/*
 * Refreshes blend modes across all material hundred records in model.
 */
// FUNCTION: FF7 0x00696c96
void __cdecl Pfile_RefreshHundredBlendModes(PolygonFileArrays *model)
{
    int hundred;
    int index;

    if (model != 0) {
        hundred = model->hundreds;
        if (hundred != 0) {
            for (index = 0; index < model->hundredCount; index++) {
                Pfile_SetHundredBlendMode(1, ((PolygonHundredState *)hundred)->blendMode, (PolygonHundredState *)hundred);
                hundred += 0x64;
            }
        }
    }
}

// Bytes no named object covers yet, kept as raw data.
// GLOBAL: FF7 0x00db2c10
char g_PlytopdDebugBuffer[200] = {0};
// End of the raw data.
// GLOBAL: FF7 0x00db2cd8
char g_PlytopdEmptyKeyword[4] = {0};
// GLOBAL: FF7 0x00db2cdc
unsigned char g_PlytopdEmptyAnimName[4] = {0};
} /* extern "C" */
