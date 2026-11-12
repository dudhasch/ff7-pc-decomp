// Contents: Field data tables and parsing: tile and background buffers, map name and id lookups,
//   the map list loaded from the field LGP, the section parsers for the field file, and debug dumps
//   with tile draw ordering.
/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/types/field.h"
#include "lib/types/graphics.h"
#include "ff7/api/field.h"
#include "lib/api/graphics.h"
#include "ff7/enums/main.h"
#include <string.h>
#include "lib/crt.h"
#include "lib/api/mem.h"
#include "lib/enums/graphics.h"
#include "lib/api/file.h"
#include "lib/enums/file.h"
extern "C" {

// src/ff7/field/src/ad_data.cpp
// Original: C:\FF7\field\src\ad_data.cpp

typedef int(__cdecl *SectionParseFn)(char *);
// Section 5 of the field file: the offsets of the four tile lists, then the group entries.
typedef struct FieldTileMapHeader {
    int spriteOffset;
    int contextOffset;
    int nodeOffset;
    int rawOffset;
} FieldTileMapHeader;
// One tile's entry in the depth table: the low six bits of depth are the z value and bit 7 marks the tile in use.
typedef struct FieldTileZEntry {
    unsigned char depth;
    unsigned char flags;
} FieldTileZEntry;
#define FIELD_TILE_LIST_END 0x7fff
#define FIELD_TILE_GROUP_BREAK 0x7ffe
// clang-format off
// The nine layer 2 tiles whose depthZ is set to 0.99529 on map 0xCC.
// GLOBAL: FF7 0x00905e58
int g_FieldDepthOverrideTiles_0xcc[9] = {4, 6, 8, 33, 35, 37, 64, 66, 69};
// GLOBAL: FF7 0x00cff3d0
short g_FieldTileSpriteCount = 0;
// GLOBAL: FF7 0x00cff3d4
short g_FieldTileGroupCount = 0;
// The field camera: the 30 bytes of a PsxMatrix.
// GLOBAL: FF7 0x00cff3d8
unsigned char g_FieldCameraTransform[30] = {0};
// GLOBAL: FF7 0x00cff3f8
struct FieldLayerDrawOrder *g_pFieldLayer1DrawOrder = 0;
// GLOBAL: FF7 0x00cff3fc
struct FieldLayerDrawOrder *g_pFieldLayer2DrawOrder = 0;
// GLOBAL: FF7 0x00cff400
struct FieldLayerDrawOrder *g_pFieldLayer4DrawOrder = 0;
// GLOBAL: FF7 0x00cff404
struct FieldLayerDrawOrder *g_pFieldLayer3DrawOrder = 0;
// GLOBAL: FF7 0x00cff408
unsigned short g_FieldTileDrawGroupCount = 0;
// GLOBAL: FF7 0x00cff40c
struct FieldTileGroup *g_pFieldTileGroupBuffer = 0;
// GLOBAL: FF7 0x00cff410
struct FieldTilePaletteEntry *g_pFieldTilePaletteBuffer = 0;
// GLOBAL: FF7 0x00cff414
FieldTileZEntry *g_pFieldTileZBuffer = 0;
// GLOBAL: FF7 0x00cff418
struct FieldTileAlphaRecord *g_pFieldTileAlphaBuffer = 0;
// GLOBAL: FF7 0x00cff41c
short g_FieldTileGroupIndex = 0;
// GLOBAL: FF7 0x00cff420
short g_FieldTileLayerIndex = 0;
// GLOBAL: FF7 0x00cff424
short g_FieldTileLayerOffset = 0;
// GLOBAL: FF7 0x00cff428
int g_FieldTileMinZ = 0;
// GLOBAL: FF7 0x00cff42c
int g_FieldFieldFileLoaded = 0;
// GLOBAL: FF7 0x00cff430
FieldMapName *g_pFieldMapNameBuffer = 0;
// GLOBAL: FF7 0x00cff434
void * g_pFieldWalkmesh = 0;
// GLOBAL: FF7 0x00cff438
FieldTileMapHeader *g_pFieldTileMap = 0;
// GLOBAL: FF7 0x00cff43c
struct FieldCameraData *g_pFieldSection1 = 0;
// GLOBAL: FF7 0x00cff440
void * g_pFieldTriggers = 0;
// GLOBAL: FF7 0x00cff444
void * g_pFieldEncounters = 0;
// GLOBAL: FF7 0x00cff448
void * g_pFieldPalette = 0;
unsigned char g_bssPad_00cff44c[4] = {0};
// GLOBAL: FF7 0x00cff450
char *g_pFieldTextBuffer = 0;
// GLOBAL: FF7 0x00cff454
struct FieldBoundsRect *g_pFieldScrollBounds = 0;
// GLOBAL: FF7 0x00cff458
struct FieldCameraData *g_pFieldCameraData = 0;
// GLOBAL: FF7 0x00cff45c
struct FieldEncounterEntry *g_pFieldEncountersAlias = 0;
// GLOBAL: FF7 0x00cff460
struct FieldTileDrawOrder *g_pFieldTileDrawOrder = 0;
// GLOBAL: FF7 0x00cff464
unsigned int g_FieldMapCount = 0;
// GLOBAL: FF7 0x00cff468
unsigned short g_FieldCurrentMapId = 0;
// GLOBAL: FF7 0x00cff46c
unsigned short g_FieldTileGroupTotal = 0;
// GLOBAL: FF7 0x00cff470
unsigned short g_FieldTileDrawGroupTotal = 0;
unsigned char g_bssPad_00cff474[4] = {0};
unsigned char g_bssPad_00cff478[4] = {0};
// GLOBAL: FF7 0x00cff47c
int g_FieldMinMapId = 0;
// GLOBAL: FF7 0x00cff480
int g_FieldMaxMapId = 0;
// GLOBAL: FF7 0x00cff484
int g_FieldMapIdFirst = 0;
// GLOBAL: FF7 0x00cff488
int g_FieldMapIdLast = 0;
// GLOBAL: FF7 0x00cff48c
unsigned char * g_pFieldBgCursor = 0;
// clang-format on
void __cdecl Field_BuildTileSprites(struct FieldSmallTileSprite *small, char *attributes, struct FieldTileSprite *large, FieldTileDrawGroup *groups);
int __cdecl Field_ComposeBackgroundFromTileMap(void);

// The string and memory copies here are inlined `rep` moves: the original made them intrinsic.
#pragma intrinsic(memcpy, strcat, strcpy, strlen)

struct FieldLayer1Tile;

/* Unprototyped retail call signature (parameter/return width differs from callee definition). */
extern void __cdecl Field_PackTileDepth(unsigned short *dst, unsigned char depthLow, unsigned char depthHigh);
/* Unprototyped retail call signature (parameter/return width differs from callee definition). */
extern void __cdecl Field_DecodeClutToPaletteRow(short clut, unsigned short *row);
__declspec(dllimport) int __stdcall SetRect(int *rect, int left, int top, int right, int bottom);
#define g_FieldTimPixelWidth1High (*(short *)((char *)&g_FieldTimPixelWidth1 + 2))
#define g_FieldTimPixelWidth2High (*(short *)((char *)&g_FieldTimPixelWidth2 + 2))
#define g_FieldPaletteGridStride (*(unsigned short *)((char *)&g_FieldPaletteGridHeight + 2))

/*
 * Allocates the four tile buffers (groups, palettes, depths, alpha) that are not
 * allocated yet; returns 0 if one allocation fails. Each pointer is tested twice:
 * once to decide whether to allocate, once to decide whether to give up.
 */
// FUNCTION: FF7 0x00620790
int __cdecl Field_AllocTileBuffers(void)
{
    if (g_pFieldTileGroupBuffer == 0) {
        // STRING: FF7 0x00905e7c
        g_pFieldTileGroupBuffer = (struct FieldTileGroup *)GameHeapAllocRaw(0xbb80, "C:\\FF7\\field\\src\\ad_data.cpp", 0x60);
    }
    if (g_pFieldTileGroupBuffer == 0) {
        return 0;
    }
    if (g_pFieldTilePaletteBuffer == 0) {
        // STRING: FF7 0x00905e9c
        g_pFieldTilePaletteBuffer = (struct FieldTilePaletteEntry *)GameHeapAllocRaw(0xea60, "C:\\FF7\\field\\src\\ad_data.cpp", 0x64);
    }
    if (g_pFieldTilePaletteBuffer == 0) {
        return 0;
    }
    if (g_pFieldTileZBuffer == 0) {
        // STRING: FF7 0x00905ebc
        g_pFieldTileZBuffer = (struct FieldTileZEntry *)GameHeapAllocRaw(0x1770, "C:\\FF7\\field\\src\\ad_data.cpp", 0x68);
    }
    if (g_pFieldTileZBuffer == 0) {
        return 0;
    }
    if (g_pFieldTileAlphaBuffer == 0) {
        // STRING: FF7 0x00905edc
        g_pFieldTileAlphaBuffer = (struct FieldTileAlphaRecord *)GameHeapAllocRaw(0xea60, "C:\\FF7\\field\\src\\ad_data.cpp", 0x6c);
    }
    if (g_pFieldTileAlphaBuffer == 0) {
        return 0;
    }
    g_FieldTileGroupIndex = 0;
    g_FieldTileLayerIndex = 0;
    return 1;
}

/*
 * Frees the four tile buffers that are allocated and clears their pointers.
 */
// FUNCTION: FF7 0x0062086b
void __cdecl Field_FreeTileBuffers(void)
{
    if (g_pFieldTileGroupBuffer != 0) {
        // STRING: FF7 0x00905efc
        GameHeapFree(g_pFieldTileGroupBuffer, "C:\\FF7\\field\\src\\ad_data.cpp", 0x7b);
        g_pFieldTileGroupBuffer = 0;
    }
    if (g_pFieldTilePaletteBuffer != 0) {
        // STRING: FF7 0x00905f1c
        GameHeapFree(g_pFieldTilePaletteBuffer, "C:\\FF7\\field\\src\\ad_data.cpp", 0x80);
        g_pFieldTilePaletteBuffer = 0;
    }
    if (g_pFieldTileZBuffer != 0) {
        // STRING: FF7 0x00905f3c
        GameHeapFree(g_pFieldTileZBuffer, "C:\\FF7\\field\\src\\ad_data.cpp", 0x85);
        g_pFieldTileZBuffer = 0;
    }
    if (g_pFieldTileAlphaBuffer != 0) {
        // STRING: FF7 0x00905f5c
        GameHeapFree(g_pFieldTileAlphaBuffer, "C:\\FF7\\field\\src\\ad_data.cpp", 0x8b);
        g_pFieldTileAlphaBuffer = 0;
    }
}

/*
 * Walk a layer 2 tile array and keep the smallest and largest of
 * one 16-bit member, seeded 0xFF and 0 so the first real value wins
 * both. The pair the second of them writes is the one
 * Field_IsMapIdInRange tests, so the member is a map id.
 * Values below 0xF are skipped, which the sprite scan below does
 * not do.
 */
// FUNCTION: FF7 0x0062091b
void __cdecl Field_ScanMapIdRangeAlt(void)
{
    int i;

    g_FieldMinMapId = 0xff;
    g_FieldMaxMapId = 0;
    if (g_FieldDoDrawLayer3 != 0) {
        for (i = 0; i < g_FieldLayer3TilesNum; i++) {
            if (g_FieldLayer3Tiles[i].mapId >= 0xf) {
                if (g_FieldMinMapId > g_FieldLayer3Tiles[i].mapId) {
                    g_FieldMinMapId = g_FieldLayer3Tiles[i].mapId;
                }
                if (g_FieldMaxMapId < g_FieldLayer3Tiles[i].mapId) {
                    g_FieldMaxMapId = g_FieldLayer3Tiles[i].mapId;
                }
            }
        }
    }
}

/*
 * Walk a layer 2 tile array and keep the smallest and largest of
 * one 16-bit member, seeded 0xFF and 0 so the first real value wins
 * both. The pair the second of them writes is the one
 * Field_IsMapIdInRange tests, so the member is a map id.
 * Three separate gates before the loop, and record 0 is skipped.
 */
// FUNCTION: FF7 0x00620a06
void __cdecl Field_ScanMapIdRange(void)
{
    int i;

    if (g_FieldDataReady == 0) {
        return;
    }
    g_FieldMapIdFirst = 0xff;
    g_FieldMapIdLast = 0;
    if (g_FieldDoDrawLayer3 == 0) {
        if (g_FieldCameraScrollE74 != 0) {
            for (i = 1; i < g_FieldLayer1TilesNum; i++) {
                if (g_FieldMapIdFirst > g_FieldLayer1Tiles[i].mapId) {
                    g_FieldMapIdFirst = g_FieldLayer1Tiles[i].mapId;
                }
                if (g_FieldMapIdLast < g_FieldLayer1Tiles[i].mapId) {
                    g_FieldMapIdLast = g_FieldLayer1Tiles[i].mapId;
                }
            }
        }
    }
}

/*
 * Two jumps out of the first test and one out of each half of the
 * second: the first is `if (...) return`, the second a single `&&`
 * with both arms landing on the same `xor eax, eax`.
 */
// FUNCTION: FF7 0x00620aef
int __cdecl Field_IsMapIdInRange(int mapId)
{
    if (g_FieldDataReady == 0) {
        return 0;
    }
    if (mapId >= g_FieldMapIdFirst && mapId <= g_FieldMapIdLast) {
        return 1;
    } else {
        return 0;
    }
}

/*
 * The same shape as Field_FreeTileBuffers on a different four
 * pointers, and four more copies of the file name.
 */
// FUNCTION: FF7 0x00620b20
void __cdecl Field_FreeBackgroundIndexArrays(void)
{
    if (g_pFieldLayer1DrawOrder != 0) {
        // STRING: FF7 0x00905f7c
        GameHeapFree(g_pFieldLayer1DrawOrder, "C:\\FF7\\field\\src\\ad_data.cpp", 0xc2);
        g_pFieldLayer1DrawOrder = 0;
    }
    if (g_pFieldLayer2DrawOrder != 0) {
        // STRING: FF7 0x00905f9c
        GameHeapFree(g_pFieldLayer2DrawOrder, "C:\\FF7\\field\\src\\ad_data.cpp", 0xc7);
        g_pFieldLayer2DrawOrder = 0;
    }
    if (g_pFieldLayer4DrawOrder != 0) {
        // STRING: FF7 0x00905fbc
        GameHeapFree(g_pFieldLayer4DrawOrder, "C:\\FF7\\field\\src\\ad_data.cpp", 0xcc);
        g_pFieldLayer4DrawOrder = 0;
    }
    if (g_pFieldLayer3DrawOrder != 0) {
        // STRING: FF7 0x00905fdc
        GameHeapFree(g_pFieldLayer3DrawOrder, "C:\\FF7\\field\\src\\ad_data.cpp", 0xd1);
        g_pFieldLayer3DrawOrder = 0;
    }
}

/*
 * Thunk to Field_ComposeBackgroundFromTileMap.
 */
// FUNCTION: FF7 0x00620bd3
void __cdecl Field_ComposeBackgroundFromTileMapThunk(void)
{
    Field_ComposeBackgroundFromTileMap();
}

/*
 * `repne scasb` for the length then the same movsd/movsb pair: an
 * inlined strcpy, not a memcpy with a computed size.
 */
// FUNCTION: FF7 0x00620bdd
void __cdecl Field_SetDataDir(char *dir)
{
    strcpy(g_FieldDataDir, dir);
}

/*
 * Reads maplist.dir from the field data directory into a new 0x9600-byte table of 0x20-byte
 * map names, one per line with its newline stripped, freeing the previous table first, and
 * counts them in g_FieldMapCount. Returns 0 when the file does not open, the allocation fails
 * or the list is empty, else 1.
 */
// FUNCTION: FF7 0x00620c0b
int __cdecl Field_LoadMapListFromFile(void)
{
    char path[0x80];
    FILE *file;

    if (g_pFieldMapNameBuffer != 0) {
        GameHeapFree(
            g_pFieldMapNameBuffer,
            // STRING: FF7 0x00905ffc
            "C:\\FF7\\field\\src\\ad_data.cpp", 0xea);
        g_pFieldMapNameBuffer = 0;
    }

    strcpy(path, g_FieldDataDir);
    strcat(path,
           // STRING: FF7 0x0090601c
           "maplist.dir");
    file = Field_FileOpen(
        path,
        // STRING: FF7 0x00906028
        "rb");
    if (file == 0) {
        return 0;
    }

    g_pFieldMapNameBuffer = (char (*)[32])GameHeapAllocRaw(
        0x9600,
        // STRING: FF7 0x0090602c
        "C:\\FF7\\field\\src\\ad_data.cpp", 0xf5);
    if (g_pFieldMapNameBuffer == 0) {
        return 0;
    }

    g_FieldMapCount = 0;
    while (!feof(file)) {
        path[0] = 0;
        fgets(path, 0x7f, file);
        if (strlen(path) != 0) {
            if (path[strlen(path) - 1] == '\n') {
                path[strlen(path) - 1] = 0;
            }
        }
        strncpy(g_pFieldMapNameBuffer[g_FieldMapCount], path,
                0x20);
        g_FieldMapCount++;
    }
    Field_FileClose(file);
    if (g_FieldMapCount == 0) {
        return 0;
    }
    return 1;
}

/*
 * Loads MAPLIST from the field LGP, copies its counted 32-byte records into the persistent map-name
 * table, and releases the temporary archive buffer.
 */
// FUNCTION: FF7 0x00620db6
int __cdecl Field_LoadMapListFromLgp(void)
{
    int size;
    char *data;
    int j;
    void *handle;
    int index;
    char *base;

    if (g_pFieldMapNameBuffer != 0) {
        GameHeapFree(
            g_pFieldMapNameBuffer,
            // STRING: FF7 0x0090604c
            "C:\\FF7\\field\\src\\ad_data.cpp", 0x12c);
        g_pFieldMapNameBuffer = 0;
    }

    handle = FindLgpFile(
        // STRING: FF7 0x0090606c
        "MAPLIST",
        LGP_ARCHIVE_FIELD_LEVEL);
    if (handle != 0) {
        size = GetLgpEntrySize((int)handle, LGP_ARCHIVE_FIELD_LEVEL);
        data = (char *)GameHeapAllocRaw(
            size,
            // STRING: FF7 0x00906074
            "C:\\FF7\\field\\src\\ad_data.cpp", 0x134);
        base = data;
        if (data == 0) {
            return 0;
        }
        ReadLgpEntry(handle, LGP_ARCHIVE_FIELD_LEVEL, data, size);
    } else {
        return 0;
    }

    g_pFieldMapNameBuffer = (char (*)[32])GameHeapAllocRaw(
        0x9600,
        // STRING: FF7 0x00906094
        "C:\\FF7\\field\\src\\ad_data.cpp", 0x14e);
    if (g_pFieldMapNameBuffer == 0) {
        return 0;
    }

    g_FieldMapCount = *(unsigned short *)data;
    if (g_FieldMapCount == 0) {
        GameHeapFree(
            data,
            // STRING: FF7 0x009060b4
            "C:\\FF7\\field\\src\\ad_data.cpp", 0x155);
        return 0;
    }

    data += 2;
    for (index = 0; (unsigned int)index < g_FieldMapCount; index++) {
        strncpy(g_pFieldMapNameBuffer[index], data, 0x20);
        data += 0x20;
    }
    GameHeapFree(
        base,
        // STRING: FF7 0x009060d4
        "C:\\FF7\\field\\src\\ad_data.cpp", 0x163);
    return 1;
}

/*
 * Frees the ad_data buffer g_pFieldMapNameBuffer if allocated.
 */
// FUNCTION: FF7 0x00620f24
void __cdecl Field_FreeAdDataBuffer(void)
{
    if (g_pFieldMapNameBuffer != 0) {
        // STRING: FF7 0x009060f4
        GameHeapFree(g_pFieldMapNameBuffer, "C:\\FF7\\field\\src\\ad_data.cpp", 0x16d);
        g_pFieldMapNameBuffer = 0;
    }
}

/*
 * Copies one fixed-width map-name record when the unsigned 16-bit id is in range and appends a
 * terminator in the caller's extra byte.
 */
// FUNCTION: FF7 0x00620f54
int __cdecl Field_GetMapNameById(unsigned short mapId, char *dst)
{
    if (g_pFieldMapNameBuffer == 0 || (unsigned int)mapId >= g_FieldMapCount) {
        return 0;
    } else {
        strncpy(dst, g_pFieldMapNameBuffer[mapId], 0x20);
        dst[0x20] = 0;
        return 1;
    }
}

/*
 * Points g_pFieldPalette at field section 3.
 */
// FUNCTION: FF7 0x00620fa5
int __cdecl Field_ParsePaletteSection(void)
{
    g_pFieldPalette = Field_GetSectionPtr(3);
    return 1;
}

/*
 * Read a whole file into a buffer of its own, NUL-terminated, and
 * hand the previous one back to the heap first. The allocation is
 * two bytes longer than the file and only one of them is written,
 * which is the original's margin and is left alone.
 */
// FUNCTION: FF7 0x00620fbe
int __cdecl Field_LoadTextFile(char *name)
{
    int size;
    FILE *file;

    // STRING: FF7 0x00906114
    file = Field_FileOpen(name, "rb");
    if (file == 0) {
        return 0;
    }
    if (g_pFieldTextBuffer != 0) {
        // STRING: FF7 0x00906118
        GameHeapFree(g_pFieldTextBuffer, "C:\\FF7\\field\\src\\ad_data.cpp", 0x1b1);
        g_pFieldTextBuffer = 0;
    }
    size = Field_FileSize(file);
    // STRING: FF7 0x00906138
    g_pFieldTextBuffer = (char *)GameHeapAllocRaw(size + 2, "C:\\FF7\\field\\src\\ad_data.cpp", 0x1b7);
    if (g_pFieldTextBuffer == 0) {
        return 0;
    }
    Field_FileRead(file, g_pFieldTextBuffer, size);
    g_pFieldTextBuffer[size] = 0;
    Field_FileClose(file);
    return 1;
}

/*
 * Section 1 is the camera matrix: three rows of three 16-bit values
 * (matrix), three 32-bit ones (translation), and a projection distance
 * (geomScreen) that goes straight to the GTE.
 * The 30-byte copy into g_FieldCameraMatrixWork takes the rotation and the
 * translation together.
 * `Field_GetSectionPtr(1)` is stored twice, into two different
 * globals, and every read afterwards goes through the second.
 */
// FUNCTION: FF7 0x00621083
int __cdecl Field_ParseCameraSection(void)
{
    unsigned short rowIndex;
    unsigned short colIndex;

    g_pFieldSection1 = (struct FieldCameraData *)Field_GetSectionPtr(1);
    g_pFieldCameraData = g_pFieldSection1;
    for (rowIndex = 0; rowIndex < 3; rowIndex++) {
        ((PsxMatrix *)g_FieldCameraTransform)->t[rowIndex] = g_pFieldCameraData->translation[rowIndex];
        for (colIndex = 0; colIndex < 3; colIndex++) {
            ((PsxMatrix *)g_FieldCameraTransform)->m[rowIndex][colIndex] =
                g_pFieldCameraData->matrix[rowIndex][colIndex];
        }
    }
    memcpy(g_FieldCameraMatrixWork, g_FieldCameraTransform, 0x1e);
    SetGeomScreen(g_pFieldCameraData->geomScreen);
    return 1;
}

/*
 * Points g_pFieldEncounters (and its alias) at field section 6.
 */
// FUNCTION: FF7 0x00621182
int __cdecl Field_ParseEncounterSection(void)
{
    g_pFieldEncounters = Field_GetSectionPtr(6);
    g_pFieldEncountersAlias = (struct FieldEncounterEntry *)g_pFieldEncounters;
    return 1;
}

/*
 * Points g_pFieldTileMap at field section 5 and frees the tile buffers built from the previous
 * map.
 */
// FUNCTION: FF7 0x006211a5
int __cdecl Field_ParseTileMapSection(void)
{
    g_pFieldTileMap = (struct FieldTileMapHeader *)Field_GetSectionPtr(5);
    Field_FreeTileBuffers();
    return 1;
}

/*
 * Points g_pFieldTriggers (and its alias) at field section 7.
 */
// FUNCTION: FF7 0x006211c3
int __cdecl Field_ParseTriggersSection(void)
{
    g_pFieldTriggers = Field_GetSectionPtr(7);
    g_pFieldScrollBounds = (struct FieldBoundsRect *)g_pFieldTriggers;
    return 1;
}

/*
 * Points g_pFieldWalkmesh at field section 4.
 */
// FUNCTION: FF7 0x006211e6
int __cdecl Field_ParseWalkmeshSection(void)
{
    g_pFieldWalkmesh = Field_GetSectionPtr(4);
    return 1;
}

/*
 * Frees the flevel and ad_obj file buffers.
 */
// FUNCTION: FF7 0x006211ff
void __cdecl Field_ReleaseFieldFileBuffers(void)
{
    Field_FreeFlevelBuffer();
    Field_FreeAdObjBuffer();
}

/*
 * Loads the named field sections in their original order and returns the final parse status.
 */
// FUNCTION: FF7 0x0062120e
int __cdecl Field_ParseAllSections(void)
{
    // STRING: FF7 0x00906158
    char *mim = ".mim";
    // STRING: FF7 0x00906160
    char *map = ".map";
    // STRING: FF7 0x00906168
    char *id = ".id";
    // STRING: FF7 0x0090616c
    char *ca = ".ca";
    // STRING: FF7 0x00906170
    char *ate = ".ate";
    // STRING: FF7 0x00906178
    char *bsx = ".bsx";
    // STRING: FF7 0x00906180
    char *sta = ".sta";
    int w;
    // STRING: FF7 0x00906188
    char *inf = ".inf";
    // STRING: FF7 0x00906190
    char *enc = ".enc";
    // STRING: FF7 0x00906198
    char *fcr = ".fcr";
    // STRING: FF7 0x009061a0
    char *fda = ".fda";
    char entry[0x80];
    // STRING: FF7 0x009061a8
    char *sep = "\\";
    // STRING: FF7 0x009061ac
    char *fpl = ".fpl";
    // STRING: FF7 0x009061b4
    char *fcc = ".fcc";
    char temp[0x100];
    char file[0x100];

    if (Field_GetMapNameById(g_FieldCurrentMapId, entry) == 0) {
        return 0;
    }
    g_FieldFieldFileLoaded = 0;
    strcpy(temp, g_FieldDataDir);
    strcat(temp, entry);
    strcat(temp, sep);
    strcat(temp, entry);
    strcpy(file, temp);
    strcat(file, fda);
    g_FieldTileStateDc4 = 1;
    if (g_FieldLoadAssets == 0) {
        strcpy(file, temp);
        strcat(file, mim);
        if (((SectionParseFn)Field_ParsePaletteSection)(file) == 0) {
            return 0;
        }
    } else {
        strcpy(file, temp);
        strcat(file, fpl);
        if (((SectionParseFn)Field_ParsePaletteSection)(file) == 0) {
            return 0;
        }
    }
    strcpy(file, temp);
    strcat(file, map);
    if (((SectionParseFn)Field_ParseTileMapSection)(file) == 0) {
        return 0;
    }
    strcpy(file, temp);
    strcat(file, inf);
    if (((SectionParseFn)Field_ParseTriggersSection)(file) == 0) {
        return 0;
    }
    strcpy(file, temp);
    strcat(file, ca);
    if (((SectionParseFn)Field_ParseCameraSection)(file) == 0) {
        return 0;
    }
    strcpy(file, temp);
    strcat(file, enc);
    if (((SectionParseFn)Field_ParseEncounterSection)(file) == 0) {
        return 0;
    }
    strcpy(file, temp);
    strcat(file, id);
    if (((SectionParseFn)Field_ParseWalkmeshSection)(file) == 0) {
        return 0;
    }
    if (g_FieldLoadAssets == 0) {
        strcpy(file, temp);
        strcat(file, fcr);
        if (Field_ParseModelListFile(file) == 0) {
            return 0;
        }
    } else {
        strcpy(file, temp);
        strcat(file, fcc);
        if (Field_ParseModelSection(file) == 0) {
            return 0;
        }
    }
    g_FieldFieldFileLoaded = 1;
    return 1;
}

/*
 * Builds the per-map directory prefix and parses the palette, tile-map, trigger, camera, encounter,
 * walkmesh and model files in their original fail-fast order.
 */
// FUNCTION: FF7 0x00621835
int __cdecl Field_ParseAllSectionsByMapIndex(void)
{
    // STRING: FF7 0x009061bc
    char *mim = ".mim";
    // STRING: FF7 0x009061c4
    char *map = ".map";
    // STRING: FF7 0x009061cc
    char *id = ".id";
    // STRING: FF7 0x009061d0
    char *ca = ".ca";
    // STRING: FF7 0x009061d4
    char *ate = ".ate";
    // STRING: FF7 0x009061dc
    char *bsx = ".bsx";
    // STRING: FF7 0x009061e4
    char *sta = ".sta";
    int w;
    // STRING: FF7 0x009061ec
    char *inf = ".inf";
    // STRING: FF7 0x009061f4
    char *enc = ".enc";
    // STRING: FF7 0x009061fc
    char *fcr = ".fcr";
    // STRING: FF7 0x00906204
    char *fda = ".fda";
    char entry[0x80];
    // STRING: FF7 0x0090620c
    char *sep = "\\";
    // STRING: FF7 0x00906210
    char *fpl = ".fpl";
    // STRING: FF7 0x00906218
    char *fcc = ".fcc";
    char temp[0x100];
    char file[0x100];

    if (Field_GetMapNameById(g_FieldCurrentMapId, entry) == 0) {
        return 0;
    }
    g_FieldFieldFileLoaded = 0;
    strcpy(temp, g_FieldDataDir);
    strcat(temp, entry);
    strcat(temp, sep);
    strcat(temp, entry);
    g_FieldTileStateDc4 = 0;
    strcpy(file, temp);
    strcat(file, mim);
    if (((SectionParseFn)Field_ParsePaletteSection)(file) == 0) {
        return 0;
    }
    strcpy(file, temp);
    strcat(file, map);
    if (((SectionParseFn)Field_ParseTileMapSection)(file) == 0) {
        return 0;
    }
    strcpy(file, temp);
    strcat(file, inf);
    if (((SectionParseFn)Field_ParseTriggersSection)(file) == 0) {
        return 0;
    }
    strcpy(file, temp);
    strcat(file, ca);
    if (((SectionParseFn)Field_ParseCameraSection)(file) == 0) {
        return 0;
    }
    strcpy(file, temp);
    strcat(file, enc);
    if (((SectionParseFn)Field_ParseEncounterSection)(file) == 0) {
        return 0;
    }
    strcpy(file, temp);
    strcat(file, id);
    if (((SectionParseFn)Field_ParseWalkmeshSection)(file) == 0) {
        return 0;
    }
    if (g_FieldLoadAssets == 0) {
        strcpy(file, temp);
        strcat(file, fcr);
        if (Field_ParseModelListFile(file) == 0) {
            return 0;
        }
    } else {
        strcpy(file, temp);
        strcat(file, fcc);
        if (Field_ParseModelSection(file) == 0) {
            return 0;
        }
    }
    g_FieldFieldFileLoaded = 1;
    return 1;
}

/*
 * Linear search of the 0x20-byte map-name table for `name`, leaving
 * the index in g_FieldCurrentMapId -- which Script_OpMenu reads, comparing
 * it against 0x9A.
 * `match` is part of the loop condition rather than a `break`.
 */
// FUNCTION: FF7 0x00621d7e
void __cdecl Field_FindMapIndexByName(char *name)
{
    int match;
    unsigned short i;

    match = 0;
    if (g_pFieldMapNameBuffer == 0 || name == 0) {
        g_FieldCurrentMapId = 0;
    } else {
        for (i = 0; i < g_FieldMapCount && match == 0; i++) {
            if (strncmp(g_pFieldMapNameBuffer[i], name, 0x20) == 0) {
                match = 1;
                g_FieldCurrentMapId = i;
            }
        }
    }
}

/*
 * Builds 16-byte small sprites and 20-byte large sprites from sentinel-delimited records.
 * Local names share VC5 bucket zero; reverse declarations preserve the original stack slots.
 */
// FUNCTION: FF7 0x00621e0e
void __cdecl Field_BuildTileSprites(FieldSmallTileSprite *small, char *attributes, FieldTileSprite *large, FieldTileDrawGroup *groups)
{
    unsigned short high;
    FieldTileRawEntry *srcRaw;
    unsigned short idx3;
    FieldTileGroupEntry *p;
    short rem;
    FieldTileSpriteSource *currRec;
    FieldTileNode *node;
    unsigned short val;
    unsigned short idx;
    int *disp;
    short span0;
    short *ctx;

    idx = 0;
    idx3 = 0;
    val = 0;
    high = 0;
    span0 = 0;
    rem = 0;
    g_FieldTileGroupIndex = 0;
    g_FieldTileGroupCount = 0;
    g_FieldTileLayerIndex = 0;
    g_FieldTileSpriteCount = 0;
    g_FieldTileDrawGroupCount = 0;
    disp = &g_pFieldTileMap->spriteOffset;
    currRec = (FieldTileSpriteSource *)((char *)g_pFieldTileMap + *disp);
    disp = &g_pFieldTileMap->contextOffset;
    ctx = (short *)((char *)g_pFieldTileMap + *disp);
    disp = &g_pFieldTileMap->nodeOffset;
    node = (FieldTileNode *)((char *)g_pFieldTileMap + *disp);
    disp = &g_pFieldTileMap->rawOffset;
    srcRaw = (FieldTileRawEntry *)((char *)g_pFieldTileMap + *disp);
    p = (FieldTileGroupEntry *)(g_pFieldTileMap + 1);
    while (1) { /* Constant condition kept for /Od branch/register layout; see lib/codegen.h. */
        if (p->marker == FIELD_TILE_LIST_END) {
            p = (FieldTileGroupEntry *)((char *)p + 2);
            break;
        }
        if (p->marker == FIELD_TILE_GROUP_BREAK) {
            Field_AddTileDrawGroup(groups, g_FieldTileDrawGroupCount, rem, g_FieldTileGroupIndex - rem, 0, 1, *ctx, 0);
            ctx++;
            g_FieldTileDrawGroupCount++;
            rem = 0;
        } else {
            span0 = p->tileCount;
            if (span0 != 0) {
                rem += span0;
            }
            while (span0 != 0) {
                small[idx].indexLow = 0x80;
                small[idx].indexHigh = 0x80;
                small[idx].depth = 0x80;
                small[idx].posX = currRec->posX;
                small[idx].posY = currRec->posY;
                small[idx].texU = currRec->texU;
                small[idx].texV = currRec->texV;
                small[idx].clut = currRec->clut;
                currRec++;
                idx++;
                idx3++;
                span0--;
                g_FieldTileGroupIndex++;
            }
        }
        p++;
    }
    g_FieldTileGroupCount = g_FieldTileGroupIndex;
    g_FieldTileSpriteCount = g_FieldTileGroupCount;
    while (1) { /* Constant condition kept for /Od branch/register layout; see lib/codegen.h. */
        if (p->marker == FIELD_TILE_LIST_END) {
            p = (FieldTileGroupEntry *)((char *)p + 2);
            break;
        }
        span0 = p->tileCount;
        while (span0 != 0) {
            Field_AddTileDrawGroup(groups, g_FieldTileDrawGroupCount, 1, idx, 0, 1, node->tPage, 0);
            g_FieldTileDrawGroupCount++;
            g_FieldTileSpriteCount++;
            small[idx].indexLow = node->packedIndex & 0xff;
            small[idx].indexHigh = (node->packedIndex & 0xff00) >> 8;
            small[idx].depth = 0x80;
            small[idx].posX = node->posX;
            small[idx].posY = node->posY;
            small[idx].texU = node->texU;
            small[idx].texV = node->texV;
            small[idx].clut = node->clut;
            attributes[idx3 * 2] = node->attribute[0];
            attributes[idx3 * 2 + 1] = node->attribute[1];
            node++;
            idx++;
            idx3++;
            span0--;
            g_FieldTileGroupIndex++;
        }
        p++;
    }
    rem = 0;
    g_FieldTileDrawGroupTotal = g_FieldTileDrawGroupCount;
    while (1) { /* Constant condition kept for /Od branch/register layout; see lib/codegen.h. */
        if (p->marker == FIELD_TILE_LIST_END) {
            p = (FieldTileGroupEntry *)((char *)p + 2);
            break;
        }
        if (p->marker == FIELD_TILE_GROUP_BREAK) {
            Field_AddTileDrawGroup(groups, g_FieldTileDrawGroupCount, rem, g_FieldTileLayerIndex - rem, 0, 1, *ctx, 0);
            ctx++;
            g_FieldTileDrawGroupCount++;
            rem = 0;
        } else {
            p->firstTile = g_FieldTileLayerIndex;
            span0 = p->tileCount;
            if (span0 != 0) {
                rem += span0;
            }
            while (span0 != 0) {
                large[high].red = 0x80;
                large[high].green = 0x80;
                large[high].blue = 0x80;
                large[high].x = srcRaw->x;
                large[high].y = srcRaw->y;
                large[high].texU = srcRaw->texU;
                large[high].texV = srcRaw->texV;
                large[high].width = 32;
                large[high].height = 32;
                large[high].clut = srcRaw->clut;
                attributes[idx3 * 2] = srcRaw->attribute[0];
                attributes[idx3 * 2 + 1] = srcRaw->attribute[1];
                srcRaw++;
                high++;
                idx3++;
                span0--;
                g_FieldTileLayerIndex++;
            }
        }
        p++;
    }
    g_FieldTileLayerOffset = g_FieldTileLayerIndex;
    while (1) { /* Constant condition kept for /Od branch/register layout; see lib/codegen.h. */
        if (p->marker == FIELD_TILE_LIST_END) {
            p = (FieldTileGroupEntry *)((char *)p + 2);
            break;
        }
        if (p->marker == FIELD_TILE_GROUP_BREAK) {
            Field_AddTileDrawGroup(groups, g_FieldTileDrawGroupCount, rem, g_FieldTileLayerIndex - rem, 0, 1, *ctx, 0);
            ctx++;
            g_FieldTileDrawGroupCount++;
            rem = 0;
        } else {
            p->firstTile = g_FieldTileLayerIndex;
            span0 = p->tileCount;
            if (span0 != 0) {
                rem += span0;
            }
            while (span0 != 0) {
                large[high].red = 0x80;
                large[high].green = 0x80;
                large[high].blue = 0x80;
                large[high].x = srcRaw->x;
                large[high].y = srcRaw->y;
                large[high].texU = srcRaw->texU;
                large[high].texV = srcRaw->texV;
                large[high].width = 32;
                large[high].height = 32;
                large[high].clut = srcRaw->clut;
                attributes[idx3 * 2] = srcRaw->attribute[0];
                attributes[idx3 * 2 + 1] = srcRaw->attribute[1];
                srcRaw++;
                high++;
                idx3++;
                span0--;
                g_FieldTileLayerIndex++;
            }
        }
        p++;
    }
}

/*
 * Writes the 16x16 and 32x32 tile-sprite records to sprite.tst, inserting the cover-data heading at
 * the first non-0x8080 depth in the first table.
 */
// FUNCTION: FF7 0x0062270b
int __cdecl Field_DebugDumpTileSprites(void)
{
    char buf[0x80];
    int flag;
    FILE *pOut;
    unsigned short index;

    flag = 1;
    pOut = Field_FileOpen(
        // STRING: FF7 0x00906224
        "sprite.tst",
        // STRING: FF7 0x00906220
        "w");
    if (pOut == 0) {
        return 0;
    }
    Field_FileWrite(
        pOut,
        // STRING: FF7 0x00906230
        "BG Sprites\n", 0xb);
    for (index = 0; index < g_FieldTileGroupIndex; index++) {
        if ((g_pFieldTileGroupBuffer[index].indexLow != 0x80 ||
             g_pFieldTileGroupBuffer[index].indexHigh != 0x80) &&
            flag != 0) {
            Field_FileWrite(
                pOut,
                // STRING: FF7 0x0090623c
                "Cover data\n", 0xb);
            flag = 0;
        }
        sprintf(buf,
                // STRING: FF7 0x00906248
                "# %d: x %d, y%d, u%d, v%d, clut%d Depth %d\n", index,
                g_pFieldTileGroupBuffer[index].posX,
                g_pFieldTileGroupBuffer[index].posY,
                g_pFieldTileGroupBuffer[index].offsetX,
                g_pFieldTileGroupBuffer[index].offsetY,
                *(unsigned short *)&g_pFieldTileGroupBuffer[index].clut,
                g_pFieldTileGroupBuffer[index].indexLow +
                    (g_pFieldTileGroupBuffer[index].indexHigh << 8));
        Field_FileWrite(pOut, buf, strlen(buf));
    }
    for (index = 0; index < g_FieldTileLayerIndex; index++) {
        sprintf(buf,
                // STRING: FF7 0x00906274
                "# %d: x %d, y%d, u%d, v%d, clut%d Depth %d\n", index,
                g_pFieldTilePaletteBuffer[index].xPos,
                g_pFieldTilePaletteBuffer[index].tileY,
                g_pFieldTilePaletteBuffer[index].offsetX,
                g_pFieldTilePaletteBuffer[index].offsetY,
                *(unsigned short *)&g_pFieldTilePaletteBuffer[index].clut,
                g_pFieldTilePaletteBuffer[index].indexLow +
                    (g_pFieldTilePaletteBuffer[index].indexHigh << 8));
        Field_FileWrite(pOut, buf, strlen(buf));
    }
    Field_FileClose(pOut);
    return 1;
}

/*
 * Writes the primary background layer; unlike layers 2/3 it has no alternate-texture branch.
 */
// FUNCTION: FF7 0x00622a6a
int __cdecl Field_DebugDumpBgLayer1(void)
{
    char buf[0x1e0];
    int flag;
    FILE *pOut;
    unsigned short index;

    flag = 1;
    pOut = Field_FileOpen(
        // STRING: FF7 0x009062a4
        "bk.tst",
        // STRING: FF7 0x009062a0
        "w");
    if (pOut == 0) {
        return 0;
    }
    Field_FileWrite(
        pOut,
        // STRING: FF7 0x009062ac
        "Background Data\n", 0xb);
    for (index = 0; index < g_FieldLayer1TilesNum; index++) {
        sprintf(buf,
                // STRING: FF7 0x009062c0
                "# %d: page %d ,u %5f, v%5f, pal%d, x%d, y%d Depth%d texture%d z%5f\n",
                index,
                g_FieldLayer1Tiles[index].pixelMode,
                g_FieldLayer1Tiles[index].texU,
                g_FieldLayer1Tiles[index].texV,
                g_FieldLayer1Tiles[index].paletteRow,
                g_FieldLayer1Tiles[index].posX,
                g_FieldLayer1Tiles[index].depth,
                g_FieldLayer1Tiles[index].sortKey,
                g_FieldLayer1Tiles[index].mapId,
                g_FieldLayer1Tiles[index].scaleFactor);
        Field_FileWrite(pOut, buf, strlen(buf));
    }
    Field_FileClose(pOut);
    return 1;
}

/*
 * Writes the cover-layer tile records and selects each record's primary or alternate texture id.
 */
// FUNCTION: FF7 0x00622c7d
int __cdecl Field_DebugDumpBgLayer2(void)
{
    char buf[0x1e0];
    int flag;
    unsigned short entryIndex;
    FILE *pStream;
    unsigned short tex;

    flag = 1;
    pStream = Field_FileOpen(
        // STRING: FF7 0x00906308
        "cover.tst",
        // STRING: FF7 0x00906304
        "w");
    if (pStream == 0) {
        return 0;
    }
    Field_FileWrite(
        pStream,
        // STRING: FF7 0x00906314
        "Cover Data\n", 0xb);
    for (entryIndex = 0; entryIndex < g_FieldLayer2TilesNum; entryIndex++) {
        if (g_FieldLayer2Tiles[entryIndex].inUse != 0) {
            tex = g_FieldLayer2Tiles[entryIndex].texPage;
        } else {
            tex = g_FieldLayer2Tiles[entryIndex].textureId;
        }
        sprintf(buf,
                // STRING: FF7 0x00906320
                "# %d: page %d ,mode %d,u %5f, v%5f, pal%d, x%d, y%d Depth%d texture%d z%5f\n",
                entryIndex,
                g_FieldLayer2Tiles[entryIndex].colorMode,
                g_FieldLayer2Tiles[entryIndex].transparencyMode,
                g_FieldLayer2Tiles[entryIndex].texU,
                g_FieldLayer2Tiles[entryIndex].texV,
                g_FieldLayer2Tiles[entryIndex].paletteRow,
                g_FieldLayer2Tiles[entryIndex].tileX,
                g_FieldLayer2Tiles[entryIndex].spanY,
                g_FieldLayer2Tiles[entryIndex].sortKey, tex,
                g_FieldLayer2Tiles[entryIndex].depthZ);
        Field_FileWrite(pStream, buf, strlen(buf));
    }
    Field_FileClose(pStream);
    return 1;
}

/*
 * Writes the lower-plane tile records when that background layer is present.
 */
// FUNCTION: FF7 0x00622f0d
int __cdecl Field_DebugDumpBgLayer3(void)
{
    char buf[0x1e0];
    int flag;
    unsigned short entryIndex;
    FILE *pStream;
    unsigned short tex;

    flag = 1;
    if (g_FieldDoDrawLayer3 == 0) {
        return 0;
    }
    pStream = Field_FileOpen(
        // STRING: FF7 0x00906370
        "lower.tst",
        // STRING: FF7 0x0090636c
        "w");
    if (pStream == 0) {
        return 0;
    }
    Field_FileWrite(
        pStream,
        // STRING: FF7 0x0090637c
        "Lower Plain Data\n", 0xb);
    for (entryIndex = 0; entryIndex < g_FieldLayer3TilesNum; entryIndex++) {
        if (g_FieldLayer3Tiles[entryIndex].ownerTag != 0) {
            tex = g_FieldLayer3Tiles[entryIndex].mapId;
        } else {
            tex = g_FieldLayer3Tiles[entryIndex].texId;
        }
        sprintf(buf,
                // STRING: FF7 0x00906390
                "# %d: page %d,mode %d,u %d, v %d, pal%d, x%d, y%d Depth%d texture%d z%5f\n",
                entryIndex,
                g_FieldLayer3Tiles[entryIndex].dstY,
                g_FieldLayer3Tiles[entryIndex].dstX,
                g_FieldLayer3Tiles[entryIndex].tileX,
                g_FieldLayer3Tiles[entryIndex].posY,
                g_FieldLayer3Tiles[entryIndex].paletteRow,
                g_FieldLayer3Tiles[entryIndex].xPos,
                g_FieldLayer3Tiles[entryIndex].tileY,
                g_FieldLayer3Tiles[entryIndex].fixedScale, tex,
                g_FieldLayer3Tiles[entryIndex].scale);
        Field_FileWrite(pStream, buf, strlen(buf));
    }
    Field_FileClose(pStream);
    return 1;
}

/*
 * Writes the fourth background layer using its own count, record base and dump strings.
 */
// FUNCTION: FF7 0x006231a6
int __cdecl Field_DebugDumpBgLayer4(void)
{
    char buf[0x1e0];
    int flag;
    unsigned short entryIndex;
    FILE *pStream;
    unsigned short tex;

    flag = 1;
    if (g_FieldDoDrawLayer4 == 0) {
        return 0;
    }
    pStream = Field_FileOpen(
        // STRING: FF7 0x009063e0
        "upper.tst",
        // STRING: FF7 0x009063dc
        "w");
    if (pStream == 0) {
        return 0;
    }
    Field_FileWrite(
        pStream,
        // STRING: FF7 0x009063ec
        "Upper Plain Data\n", 0xb);
    for (entryIndex = 0; entryIndex < g_FieldLayer4TilesNum; entryIndex++) {
        if (g_FieldLayer4Tiles[entryIndex].active != 0) {
            tex = g_FieldLayer4Tiles[entryIndex].texSlot;
        } else {
            tex = g_FieldLayer4Tiles[entryIndex].textureId;
        }
        sprintf(buf,
                // STRING: FF7 0x00906400
                "# %d: page %d,u %d, v%d, pal%d, x%d, y%d Depth%d texture%d z%5f\n",
                entryIndex,
                g_FieldLayer4Tiles[entryIndex].dstY,
                g_FieldLayer4Tiles[entryIndex].tileX,
                g_FieldLayer4Tiles[entryIndex].tileY,
                g_FieldLayer4Tiles[entryIndex].paletteRow,
                g_FieldLayer4Tiles[entryIndex].posX,
                g_FieldLayer4Tiles[entryIndex].posY,
                g_FieldLayer4Tiles[entryIndex].fixedScale, tex,
                g_FieldLayer4Tiles[entryIndex].scale);
        Field_FileWrite(pStream, buf, strlen(buf));
    }
    Field_FileClose(pStream);
    return 1;
}

// Writes six values into the 0x18-byte draw-group record at index in groups, in an order that is
// neither the argument order nor the member order: unusedParam0, unusedParam1, tileCount, firstTile, texPage, unusedParam2. The
// arguments are named for where they land, because that is all this function says about them. The
// index is checked against 0x9C4 and the table against null, each with its own early return;
// success returns 1.
// FUNCTION: FF7 0x0062341d
int __cdecl Field_AddTileDrawGroup(FieldTileDrawGroup *groups, unsigned short index,
                                   short at02, short at04, int at08,
                                   int at0c, short at00, int at10)
{
    if (index >= 0x9c4) {
        return 0;
    }
    if (groups == 0) {
        return 0;
    }
    groups[index].unusedParam0 = at08;
    groups[index].unusedParam1 = at0c;
    groups[index].tileCount = at02;
    groups[index].firstTile = at04;
    groups[index].texPage = at00;
    groups[index].unusedParam2 = at10;
    return 1;
}

/*
 * Flat subscripts preserve the original column-plus-pointer register allocation.
 * Local names share VC5 bucket zero; reverse declarations preserve the 0x74-byte frame.
 */
// FUNCTION: FF7 0x006234d3
int __cdecl Field_RasterizeTileSpritesToImage(unsigned short *count, unsigned char *pixels0, unsigned char *pixels1)
{
    int fmt;
    unsigned short high;
    unsigned int span0;
    unsigned short frameIndex;
    int rem;
    short temp0;
    unsigned short x03;
    unsigned short tx0;
    int scratch[4];
    unsigned short y03;
    short innerOrg;
    short ty;
    short curPos;
    short sx0;
    int flags;
    unsigned short clutId;
    void *tempImg;
    int quad[4];
    int curTypePart;
    unsigned short val;
    unsigned int *p;
    unsigned short wide;
    unsigned short idx;
    tempImg = 0;
    p = 0;
    idx = 0;
    flags = 0;
    wide = g_pFieldScrollBounds->maxX - g_pFieldScrollBounds->minX;
    high = g_pFieldScrollBounds->maxY - g_pFieldScrollBounds->minY;
    g_FieldLayer1StateE4c = Field_AllocSurfaceBuffer16(wide << 1, high << 1);
    if (g_FieldLayer1StateE4c == 0) {
        return 0;
    }
    (g_FieldLayer1StateE58.spanWidth) = wide << 1;
    g_FieldLayer1StateE58.spanHeight = high << 1;
    g_FieldLayer1StateE50[0] = g_pFieldScrollBounds->minX;
    g_FieldLayer1StateE50[1] = g_pFieldScrollBounds->minY;
    (g_FieldCameraScrollE70.clutId) = clutId;
    g_FieldCameraScrollE70.first = g_AdPalWords.first;
    tempImg = (void *)g_FieldLayer1StateE4c;
    span0 = (g_FieldLayer1StateE58.spanWidth) << 1;
    p = (unsigned int *)g_FieldLayer1StateE4c;
    idx = 0;
    for (frameIndex = 0; frameIndex < g_FieldTileGroupCount; frameIndex++) {
        curPos = g_pFieldTileGroupBuffer[frameIndex].posX - g_FieldLayer1StateE50[0];
        innerOrg = g_pFieldTileGroupBuffer[frameIndex].posY - g_FieldLayer1StateE50[1];
        Field_DecodeClutToPaletteRow(g_pFieldTileGroupBuffer[frameIndex].clut, &g_AdPalWords.first);
        g_FieldCameraScrollE70.first = g_AdPalWords.first;
        if (frameIndex >= g_pFieldTileAlphaBuffer[idx].frameStart + g_pFieldTileAlphaBuffer[idx].frameCount) {
            idx++;
        }
        Field_DecodeTPage(g_pFieldTileAlphaBuffer[idx].tPage, &curTypePart, &sx0, &ty, &fmt);
        if (ty == 0x100) {
            sx0 = (sx0 - g_FieldTimPixelWidth2) * 2 - 0x800;
            flags = 1;
            temp0 = ty - g_FieldTimPixelWidth2High;
        } else {
            flags = 0;
            sx0 = (sx0 - g_FieldTimPixelWidth1) << 1;
            temp0 = ty - g_FieldTimPixelWidth1High;
        }
        SetRect(quad,
                sx0 + g_pFieldTileGroupBuffer[frameIndex].offsetX,
                temp0 + g_pFieldTileGroupBuffer[frameIndex].offsetY,
                sx0 + g_pFieldTileGroupBuffer[frameIndex].offsetX + 16,
                temp0 + g_pFieldTileGroupBuffer[frameIndex].offsetY + 16);
        if (g_FieldPaletteGridBuffer == 0) {
            return 0;
        }
        for (y03 = innerOrg; y03 < innerOrg + 16; y03++) {
            if (flags == 0) {
                for (x03 = curPos, tx0 = (short)quad[0]; x03 < curPos + 16 && tx0 < quad[2]; x03++, tx0++) {
                    val = *(unsigned short *)&g_FieldPaletteGridBuffer[g_AdPalWords.first * g_FieldPaletteGridStride + pixels0[((quad[1] + y03 - innerOrg) * (unsigned short)g_FieldTimImage1Width) + tx0]].pixelWord;
                    p[y03 * (span0 >> 2) * 2 + x03] = (val << 16) | val;
                    p[y03 * (span0 >> 2) * 2 + (span0 >> 2) + x03] = (val << 16) | val;
                }
            } else {
                for (x03 = curPos, tx0 = (short)quad[0]; x03 < curPos + 16 && tx0 < quad[2]; x03++, tx0++) {
                    val = *(unsigned short *)&g_FieldPaletteGridBuffer[g_AdPalWords.first * g_FieldPaletteGridStride + pixels1[((quad[1] + y03 - innerOrg) * (unsigned short)g_FieldTimImage2Width) + tx0]].pixelWord;
                    p[y03 * (span0 >> 2) * 2 + x03] = (val << 16) | val;
                    p[y03 * (span0 >> 2) * 2 + (span0 >> 2) + x03] = (val << 16) | val;
                }
            }
        }
    }
    g_FieldCameraScrollE74 = 1;
    *count = idx;
    return 1;
}

/*
 * Fill `order` with tile indices. With g_FieldTileStateDc0 at 1 they come
 * out grouped by the paletteRow of each tile record, lowest
 * first and two past the last one asked for; otherwise the order is
 * the identity and the layer is ignored.
 */
// FUNCTION: FF7 0x00623aba
void __cdecl Field_BuildTileDrawOrder(FieldLayerDrawOrder *order, int count,
                                      void *tiles, int lastLayer)
{
    int written;
    int pass;
    int k;

    written = 0;
    if (g_FieldTileStateDc0 == 1) {
        for (pass = 0; pass < lastLayer + 2; pass++) {
            for (k = 0; k < count; k++) {
                if (((FieldLayer1Tile *)tiles)[k].paletteRow ==
                    pass) {
                    order[written].index = k;
                    written++;
                }
            }
        }
    } else {
        for (k = 0; k < count; k++) {
            order[k].index = k;
        }
    }
}

// Remaps a palette index for transparency. A value already in the palette's transparent list
// becomes 0. A 0 becomes the palette's own substitute (capacity), unless that is still the 0x100
// the allocator seeded. Every other value passes through. The list is transparentIndices and its
// length is usedSlots, which is the zero the allocator wrote.
// FUNCTION: FF7 0x00623b6a
unsigned char __cdecl Field_RemapTransparentIndex(unsigned char value,
                                                  int pal)
{
    unsigned short i;

    for (i = 0; i < g_FieldPalettePoolBuffer[pal].usedSlots; i++) {
        if (value == g_FieldPalettePoolBuffer[pal].transparentIndices[i]) {
            return 0;
        }
    }
    if (value == 0 && g_FieldPalettePoolBuffer[pal].capacity != 0x100) {
        return *(unsigned char *)&g_FieldPalettePoolBuffer[pal].capacity;
    }
    return value;
}

/*
 * Converts a PSX ordering-table depth into clip-space Z through the field camera, using the direct
 * scaled depth for the sentinel ranges and when projection falls outside [0, 1].
 */
// FUNCTION: FF7 0x00623c0f
float __cdecl Field_ComputeTileDepthZ(void *camera, int depth, int x, int y)
{
    float viewMtx[16];
    float clipVector[4];
    float projMtx[16];
    int worldPoint[3];
    int extra[2];
    float projected[4];

    if (depth < 1) {
        return 0.0001f;
    }
    if ((float)depth <= *(float *)&g_FieldTileDepthThreshold[0x0] / 4.0f) {
        return (float)depth / 1000.0f;
    }
    if (depth >= 0xfff) {
        return 0.9999f;
    }

    ScreenPointToWorld(x, y, depth, (int)camera, (float *)worldPoint, (short *)extra);
    GtePsxMatrixToFloat4x4ColVec((PsxMatrix *)camera, viewMtx);
    viewMtx[12] = (float)((PsxMatrix *)camera)->t[0];
    viewMtx[13] = (float)((PsxMatrix *)camera)->t[1];
    viewMtx[14] = (float)((PsxMatrix *)camera)->t[2];
    viewMtx[15] = 1.0f;
    TransposeMatrix4x4((struct GfxTransposedMatrix *)viewMtx, (struct GfxTransposedMatrix *)projMtx);
    ProjectPointWithBuiltMatrix((int)projMtx, (int)worldPoint, (struct GfxHomogeneousVec4 *)clipVector, (int)projected);
    if (projected[2] < 0.0f || projected[2] > 1.0f) {
        projected[2] = (float)depth / 4096.0f;
    }
    return projected[2];
}

/*
 * Composes the field background from its tile map: walks the tiles, decodes each tile's texture
 * page (Field_DecodeTPage) and palette row (Field_DecodeClutToPaletteRow) out of the packed PSX
 * fields, packs its depth (Field_PackTileDepth), converts its pixels (Field_ConvertPixelWordMode,
 * Field_RepackPixelWordMode2) into the background atlas, and builds the model draw order
 * (Field_BuildModelDrawOrder). Locals: t488 sprite index, t089 draw group, t149 pixel mode, t554
 * blend, t195 RECT, t415 texture page, t421/t115 atlas x/y, t409/t355 output column/row, t388
 * input column.
 *
 * The tNNN locals are declared in reverse stack-slot order.
 */
// FUNCTION: FF7 0x00623d28
int __cdecl Field_ComposeBackgroundFromTileMap(void)
{
    typedef struct {
        int values[4];
    } FieldComposeRect;
    int t554;
    int t548;
    int t520;
    unsigned short t514;
    unsigned short t508;
    unsigned char *t494;
    unsigned short t488;
    unsigned short *t460;
    unsigned short t454;
    short t448;
    unsigned short t421;
    unsigned short t415;
    unsigned short t409;
    unsigned short t394;
    unsigned short t388;
    unsigned short *t361;
    unsigned short t355;
    short t349;
    short t321;
    unsigned short t315;
    unsigned short t309;
    int t295;
    unsigned char *t289;
    unsigned short *t261;
    int t255;
    unsigned short t249;
    unsigned short t221;
    int t215;
    int t209;
    int t195[4];
    unsigned short t189;
    unsigned short t161;
    unsigned short t155;
    int t149;
    unsigned short t121;
    unsigned short t115;
    unsigned short t109;
    unsigned short t095;
    unsigned short t089;
    unsigned short t061;
    unsigned short t055;
    unsigned short t049;
    int t022;
    int t016;
    t289 = 0;
    t494 = 0;
    t460 = 0;
    t261 = 0;
    t361 = 0;
    t089 = 0;
    t255 = 0;
    t421 = 0;
    t115 = 0;
    t394 = 0;
    t189 = 0;
    t415 = 0;
    t161 = 0;
    t055 = 0;
    t121 = 0;
    t249 = 0;
    t315 = 0;
    t049 = 0;
    t109 = 0;
    t309 = 0;
    t221 = 0;
    t514 = 0;
    t061 = 0;
    t155 = 0;
    t215 = 0;
    t295 = 0;
    t548 = 0;
    t520 = 0;
    t022 = 0;
    t016 = 0;
    if (g_FieldTileStateDc4 != 0) {
        return 1;
    }
    if (g_FieldTileGroupIndex <= 0) {
        return 0;
    }
    t209 = g_AdPalWords.second;
    t289 = (unsigned char *)g_pFieldTimPixelData1;
    t261 = (unsigned short *)g_pFieldTimPixelData1;
    if ((unsigned int)g_FieldTimPixelSize2 > 0) {
        t494 = (unsigned char *)g_pFieldTimPixelData2;
        t361 = (unsigned short *)g_pFieldTimPixelData2;
    }
    t095 = g_pFieldScrollBounds->maxX - g_pFieldScrollBounds->minX + 16;
    t508 = g_pFieldScrollBounds->maxY - g_pFieldScrollBounds->minY + 16;
    (g_FieldLayer1StateE58.spanWidth) = t095;
    g_FieldLayer1StateE58.spanHeight = t508;
    g_FieldLayer1StateE50[0] = g_pFieldScrollBounds->minX;
    g_FieldLayer1StateE50[1] = g_pFieldScrollBounds->minY;
    (g_FieldCameraScrollE70.clutId) = t209;
    g_FieldCameraScrollE70.first = g_AdPalWords.first;
    g_FieldLayer1TilesNum = g_FieldTileGroupCount;
    // STRING: FF7 0x00906444
    g_FieldLayer1Tiles = (struct FieldLayer1Tile *)GameHeapAllocRaw(g_FieldLayer1TilesNum * 0x105c, "C:\\FF7\\field\\src\\ad_data.cpp", 0x646);
    // STRING: FF7 0x00906464
    g_pFieldLayer1DrawOrder = (struct FieldLayerDrawOrder *)GameHeapAllocRaw(g_FieldLayer1TilesNum * 4, "C:\\FF7\\field\\src\\ad_data.cpp", 0x648);
    t089 = 0;
    for (t488 = 0; t488 < g_FieldTileGroupCount; t488++) {
        g_FieldLayer1Tiles[t488].posX = g_pFieldTileGroupBuffer[t488].posX;
        g_FieldLayer1Tiles[t488].depth = g_pFieldTileGroupBuffer[t488].posY;
        if (g_FieldTileMinZ > g_FieldLayer1Tiles[t488].depth) {
            g_FieldTileMinZ = g_FieldLayer1Tiles[t488].depth;
        }
        Field_DecodeClutToPaletteRow(g_pFieldTileGroupBuffer[t488].clut, &g_FieldLayer1Tiles[t488].paletteRow);
        g_FieldLayer1Tiles[t488].sortKey = 0xfff;
        if (t488 >= g_pFieldTileAlphaBuffer[t089].frameStart + g_pFieldTileAlphaBuffer[t089].frameCount) {
            t089++;
        }
        Field_DecodeTPage(g_pFieldTileAlphaBuffer[t089].tPage, &t149, &t321, &t349, &t554);
        if (t349 == 256) {
            switch (t149) {
            case GFX_PSX_COLOR_MODE_4BIT:
                t321 = (t321 - 1024 - g_FieldTimPixelWidth2) << 2;
                break;
            case GFX_PSX_COLOR_MODE_8BIT:
                t321 = (t321 - 1024 - g_FieldTimPixelWidth2) << 1;
                break;
            case GFX_PSX_COLOR_MODE_16BIT:
                t321 = t321 - 1024 - g_FieldTimPixelWidth2;
                break;
            }
            t255 = 1;
            t448 = t349 - g_FieldTimPixelWidth2High;
        } else {
            switch (t149) {
            case GFX_PSX_COLOR_MODE_4BIT:
                t321 = (t321 - g_FieldTimPixelWidth1) << 2;
                break;
            case GFX_PSX_COLOR_MODE_8BIT:
                t321 = (t321 - g_FieldTimPixelWidth1) << 1;
                break;
            case GFX_PSX_COLOR_MODE_16BIT:
                t321 = t321 - g_FieldTimPixelWidth1;
                break;
            }
            t448 = t349 - g_FieldTimPixelWidth1High;
            t255 = 0;
        }
        if (t149 != GFX_PSX_COLOR_MODE_4BIT) {
            SetRect(t195, t321 + g_pFieldTileGroupBuffer[t488].offsetX, t448 + g_pFieldTileGroupBuffer[t488].offsetY, t321 + g_pFieldTileGroupBuffer[t488].offsetX + 16, t448 + g_pFieldTileGroupBuffer[t488].offsetY + 16);
        } else {
            SetRect(t195, (t321 + g_pFieldTileGroupBuffer[t488].offsetX) / 2, t448 + g_pFieldTileGroupBuffer[t488].offsetY, (t321 + g_pFieldTileGroupBuffer[t488].offsetX) / 2 + 8, t448 + g_pFieldTileGroupBuffer[t488].offsetY + 16);
        }
        if (t149 != GFX_PSX_COLOR_MODE_16BIT) {
            t415 = t548 / 256;
            if (t415 != t161) {
                t161 = t415;
            }
            t055 = (t548 - (t415 << 8)) / 16;
            t115 = t055 << 4;
            t421 = ((t548 - (t415 << 8) - (t055 << 4)) % 16) << 4;
            t548++;
        } else {
            t415 = t520 / 256;
            if (t415 != t514) {
                t514 = t415;
            }
            t055 = (t520 - (t415 << 8)) / 16;
            t115 = t055 << 4;
            t421 = ((t520 - (t415 << 8) - (t055 << 4)) % 16) << 4;
            t415 += 26;
            t520++;
        }
        g_FieldLayers[t415]->enabled = 1;
        g_FieldLayers[t415]->hasDoubleSizeTiles = 0;
        g_FieldLayer1Tiles[t488].texX = t421;
        g_FieldLayer1Tiles[t488].texY = t115;
        g_FieldLayer1Tiles[t488].texU = t421 / 256.0f;
        g_FieldLayer1Tiles[t488].texV = t115 / 256.0f;
        g_FieldLayer1Tiles[t488].mapId = t415;
        g_FieldLayer1Tiles[t488].pixelMode = t149;
        t460 = (unsigned short *)g_FieldLayers[t415]->pixels;
        if (t149 != GFX_PSX_COLOR_MODE_4BIT) {
            for (t355 = t115; t355 < t115 + 16; t355++) {
                if (t255 == 0) {
                    t409 = t421;
                    for (t388 = t195[0]; t409 < t421 + 16 && t388 < t195[2]; t409++, t388++) {
                        if (t149 == GFX_PSX_COLOR_MODE_8BIT) {
                            g_FieldLayers[t415]->pixels[t355 * 256 + t409] = Field_RemapTransparentIndex(t289[(t195[1] + t355 - t115) * (unsigned short)g_FieldTimImage1Width + t388], g_FieldLayer1Tiles[t488].paletteRow);
                        } else {
                            t460[t355 * 256 + t409] = Field_ConvertPixelWordMode(t261[(t195[1] + t355 - t115) * (unsigned short)g_FieldTimImage1Width / 2 + t388], 2);
                        }
                    }
                } else {
                    t409 = t421;
                    for (t388 = t195[0]; t409 < t421 + 16 && t388 < t195[2]; t409++, t388++) {
                        if (t149 == GFX_PSX_COLOR_MODE_8BIT) {
                            g_FieldLayers[t415]->pixels[t355 * 256 + t409] = Field_RemapTransparentIndex(t494[(t195[1] + t355 - t115) * (unsigned short)g_FieldTimImage2Width + t388], g_FieldLayer1Tiles[t488].paletteRow);
                        } else {
                            if (t361 != 0) {
                                t460[t355 * 256 + t409] = Field_ConvertPixelWordMode(t361[(t195[1] + t355 - t115) * (unsigned short)g_FieldTimImage2Width / 2 + t388], 2);
                            }
                        }
                    }
                }
            }
        } else {
            for (t355 = t115; t355 < t115 + 16; t355++) {
                if (t255 == 0) {
                    t409 = t421;
                    for (t388 = t195[0]; t409 < t421 + 16 && t388 < t195[2]; t388++) {
                        g_FieldLayers[t415]->pixels[t355 * 256 + t409] = Field_RemapTransparentIndex(t289[(t195[1] + t355 - t115) * (unsigned short)g_FieldTimImage1Width + t388] & 15, g_FieldLayer1Tiles[t488].paletteRow);
                        g_FieldLayers[t415]->pixels[t355 * 256 + t409 + 1] = Field_RemapTransparentIndex((t289[(t195[1] + t355 - t115) * (unsigned short)g_FieldTimImage1Width + t388] & 0xf0) >> 4, g_FieldLayer1Tiles[t488].paletteRow);
                        t409 += 2;
                    }
                } else {
                    t409 = t421;
                    for (t388 = t195[0]; t409 < t421 + 16 && t388 < t195[2]; t388++) {
                        g_FieldLayers[t415]->pixels[t355 * 256 + t409] = Field_RemapTransparentIndex(t494[(t195[1] + t355 - t115) * (unsigned short)g_FieldTimImage2Width + t388] & 15, g_FieldLayer1Tiles[t488].paletteRow);
                        g_FieldLayers[t415]->pixels[t355 * 256 + t409 + 1] = Field_RemapTransparentIndex((t494[(t195[1] + t355 - t115) * (unsigned short)g_FieldTimImage2Width + t388] & 0xf0) >> 4, g_FieldLayer1Tiles[t488].paletteRow);
                        t409 += 2;
                    }
                }
            }
        }
    }
    Field_BuildTileDrawOrder(g_pFieldLayer1DrawOrder, g_FieldLayer1TilesNum, g_FieldLayer1Tiles, g_FieldPaletteGridHeight);
    g_FieldCameraScrollE74 = 1;
    if (g_FieldCurrentMapId == 0xd2 || g_FieldCurrentMapId == 0x1fb) {
        g_FieldLayer1TilesNum--;
    }
    if (g_FieldCurrentMapId == 0x174) {
        g_FieldLayer1TilesNum -= 2;
    }
    if (g_FieldCurrentMapId == 0x20d) {
        g_FieldLayer1Tiles[626].posX = 10000;
    }
    if (g_FieldCurrentMapId == 0x2ec) {
        g_FieldLayer1TilesNum -= 4;
    }
    if (g_FieldCurrentMapId == 0x112) {
        g_FieldLayer1Tiles[480].posX = 10000;
    }
    if (g_FieldCurrentMapId == 0xe6) {
        g_FieldLayer1Tiles[640].posX = 10000;
        g_FieldLayer1Tiles[641].posX = 10000;
    }
    if (g_FieldCurrentMapId == 0x25b) {
        g_FieldLayer1Tiles[132].posX = 10000;
        g_FieldLayer1Tiles[163].posX = 10000;
        g_FieldLayer1Tiles[244].posX = 10000;
        g_FieldLayer1Tiles[245].posX = 10000;
    }
    (g_FieldLayer2StateDf8.width) = 640;
    g_FieldLayer2StateDf8.height = 480;
    g_FieldLayer2TilesNum = g_FieldTileGroupIndex - g_FieldTileGroupCount;
    g_FieldLayer2StateE08.firstIndex = t161;
    g_FieldLayer2StateE10.firstIndex = t514 + 26;
    (g_FieldLayer2StateE0c.pageStart) = 15;
    g_FieldLayer2StateE14.firstIndex = 33;
    // STRING: FF7 0x00906484
    g_FieldLayer2Tiles = (struct FieldLayer2Tile *)GameHeapAllocRaw(g_FieldLayer2TilesNum * 0x105c, "C:\\FF7\\field\\src\\ad_data.cpp", 0x708);
    // STRING: FF7 0x009064a4
    g_pFieldLayer2DrawOrder = (struct FieldLayerDrawOrder *)GameHeapAllocRaw(g_FieldLayer2TilesNum * 4, "C:\\FF7\\field\\src\\ad_data.cpp", 0x709);
    // STRING: FF7 0x009064c4
    g_pFieldTileDrawOrder = (struct FieldTileDrawOrder *)GameHeapAllocRaw(g_FieldLayer2TilesNum * 4, "C:\\FF7\\field\\src\\ad_data.cpp", 0x70b);
    if (g_FieldLayer2Tiles == 0) {
        return 0;
    }
    if (g_pFieldTileDrawOrder == 0) {
        return 0;
    }
    t421 = 0;
    t115 = 0;
    g_FieldTileGroupTotal = t089;

    // Overlay records, packed depth, texture rectangle.
    // PC counterpart pattern: BackgroundTile in src/field/ad_data.h in whodaresx/ff7.
    for (t488 = g_FieldTileGroupCount; t488 < g_FieldTileGroupIndex; t488++) {
        g_FieldLayer2Tiles[t488 - g_FieldTileGroupCount].tileX = g_pFieldTileGroupBuffer[t488].posX;
        g_FieldLayer2Tiles[t488 - g_FieldTileGroupCount].spanY = g_pFieldTileGroupBuffer[t488].posY;
        Field_DecodeClutToPaletteRow(g_pFieldTileGroupBuffer[t488].clut, &g_FieldLayer2Tiles[t488 - g_FieldTileGroupCount].paletteRow);
        Field_PackTileDepth(&g_FieldLayer2Tiles[t488 - g_FieldTileGroupCount].sortKey, g_pFieldTileGroupBuffer[t488].indexLow, g_pFieldTileGroupBuffer[t488].indexHigh);
        g_FieldLayer2Tiles[t488 - g_FieldTileGroupCount].depthZ = Field_ComputeTileDepthZ(g_FieldCameraTransform, g_FieldLayer2Tiles[t488 - g_FieldTileGroupCount].sortKey, g_pFieldTileGroupBuffer[t488].posX * 2 + 320, g_pFieldTileGroupBuffer[t488].posY * 2 + 240);
        g_FieldLayer2Tiles[t488 - g_FieldTileGroupCount].zBufferValue = g_pFieldTileZBuffer[t488].depth & 0x3f;
        g_FieldLayer2Tiles[t488 - g_FieldTileGroupCount].visibilityMask = g_pFieldTileZBuffer[t488].flags;
        g_FieldLayer2Tiles[t488 - g_FieldTileGroupCount].active = 1;
        if ((g_pFieldTileZBuffer[t488].depth & 0x80) == 0) {
            g_FieldLayer2Tiles[t488 - g_FieldTileGroupCount].inUse = 0;
        } else {
            g_FieldLayer2Tiles[t488 - g_FieldTileGroupCount].inUse = 1;
        }
        g_FieldLayer2Tiles[t488 - g_FieldTileGroupCount].width = 16;
        g_FieldLayer2Tiles[t488 - g_FieldTileGroupCount].height = 16;
        g_FieldLayer2Tiles[t488 - g_FieldTileGroupCount].clearedWord = 0;
        if (t488 >= g_pFieldTileAlphaBuffer[t089].frameStart + g_pFieldTileAlphaBuffer[t089].frameCount) {
            t089++;
        }
        Field_DecodeTPage(g_pFieldTileAlphaBuffer[t089].tPage, &t149, &t321, &t349, &t554);
        if (t349 == 256) {
            switch (t149) {
            case GFX_PSX_COLOR_MODE_4BIT:
                t321 = (t321 - 1024 - g_FieldTimPixelWidth2) << 2;
                break;
            case GFX_PSX_COLOR_MODE_8BIT:
                t321 = (t321 - 1024 - g_FieldTimPixelWidth2) << 1;
                break;
            case GFX_PSX_COLOR_MODE_16BIT:
                t321 = t321 - 1024 - g_FieldTimPixelWidth2;
                break;
            }
            t255 = 1;
            t448 = t349 - g_FieldTimPixelWidth2High;
        } else {
            switch (t149) {
            case GFX_PSX_COLOR_MODE_4BIT:
                t321 = (t321 - g_FieldTimPixelWidth1) << 2;
                break;
            case GFX_PSX_COLOR_MODE_8BIT:
                t321 = (t321 - g_FieldTimPixelWidth1) << 1;
                break;
            case GFX_PSX_COLOR_MODE_16BIT:
                t321 = t321 - g_FieldTimPixelWidth1;
                break;
            }
            t448 = t349 - g_FieldTimPixelWidth1High;
            t255 = 0;
        }
        if (t149 != GFX_PSX_COLOR_MODE_4BIT) {
            SetRect(t195, t321 + g_pFieldTileGroupBuffer[t488].offsetX, t448 + g_pFieldTileGroupBuffer[t488].offsetY, t321 + g_pFieldTileGroupBuffer[t488].offsetX + 16, t448 + g_pFieldTileGroupBuffer[t488].offsetY + 16);
        } else {
            SetRect(t195, (t321 + g_pFieldTileGroupBuffer[t488].offsetX) / 2, t448 + g_pFieldTileGroupBuffer[t488].offsetY, (t321 + g_pFieldTileGroupBuffer[t488].offsetX) / 2 + 8, t448 + g_pFieldTileGroupBuffer[t488].offsetY + 16);
        }

        g_FieldLayer2Tiles[t488 - g_FieldTileGroupCount].colorMode = t149;
        *(FieldComposeRect *)g_FieldLayer2Tiles[t488 - g_FieldTileGroupCount].sourceRect = *(FieldComposeRect *)t195;
        g_FieldLayer2Tiles[t488 - g_FieldTileGroupCount].usesSecondTim = t255;
        g_FieldLayer2Tiles[t488 - g_FieldTileGroupCount].transparencyMode = t554;
        if (t554 == 2) {
            g_FieldLayer2Tiles[t488 - g_FieldTileGroupCount].transparencyMode = 0;
            g_FieldLayer2Tiles[t488 - g_FieldTileGroupCount].paletteRow = g_FieldPaletteGridHeight + 1;
        }
        // Six opaque/blended atlas allocation paths.
        if (t149 != GFX_PSX_COLOR_MODE_16BIT) {
            if (g_FieldLayer2Tiles[t488 - g_FieldTileGroupCount].inUse == 0) {
                t415 = t548 / 256;
                t055 = (t548 - (t415 << 8)) / 16;
                t115 = t055 << 4;
                t421 = ((t548 - (t415 << 8) - (t055 << 4)) % 16) << 4;
                g_FieldLayer2Tiles[t488 - g_FieldTileGroupCount].texX = t421;
                g_FieldLayer2Tiles[t488 - g_FieldTileGroupCount].texY = t115;
                g_FieldLayer2Tiles[t488 - g_FieldTileGroupCount].texU = t421 / 256.0f;
                g_FieldLayer2Tiles[t488 - g_FieldTileGroupCount].texV = t115 / 256.0f;
                g_FieldLayer2Tiles[t488 - g_FieldTileGroupCount].textureId = t415;
                if (t415 != t161) {
                    t161 = t415;
                }
                t548++;
            } else {
                if (g_FieldLayer2Tiles[t488 - g_FieldTileGroupCount].transparencyMode != 0) {
                    t415 = t049 / 256;
                    if (t415 != t109) {
                        t109 = t415;
                    }
                    t055 = (t049 - (t415 << 8)) / 16;
                    t115 = t055 << 4;
                    t421 = ((t049 - (t415 << 8) - (t055 << 4)) % 16) << 4;
                    g_FieldLayer2Tiles[t488 - g_FieldTileGroupCount].altTexX = t421;
                    g_FieldLayer2Tiles[t488 - g_FieldTileGroupCount].altTexY = t115;
                    g_FieldLayer2Tiles[t488 - g_FieldTileGroupCount].texU = t421 / 256.0f;
                    g_FieldLayer2Tiles[t488 - g_FieldTileGroupCount].texV = t115 / 256.0f;
                    t415 += 15;
                    g_FieldLayer2Tiles[t488 - g_FieldTileGroupCount].texPage = t415;
                    t049++;
                } else {
                    t415 = t215 / 256;
                    t055 = (t215 - (t415 << 8)) / 16;
                    t115 = t055 << 4;
                    t421 = ((t215 - (t415 << 8) - (t055 << 4)) % 16) << 4;
                    g_FieldLayer2Tiles[t488 - g_FieldTileGroupCount].altTexX = t421;
                    g_FieldLayer2Tiles[t488 - g_FieldTileGroupCount].altTexY = t115;
                    g_FieldLayer2Tiles[t488 - g_FieldTileGroupCount].texU = t421 / 256.0f;
                    g_FieldLayer2Tiles[t488 - g_FieldTileGroupCount].texV = t115 / 256.0f;
                    t415 += 24;
                    g_FieldLayer2Tiles[t488 - g_FieldTileGroupCount].texPage = t415;
                    t215++;
                }
            }
        } else {
            if (g_FieldLayer2Tiles[t488 - g_FieldTileGroupCount].inUse == 0) {
                t415 = t520 / 256;
                if (t415 != t514) {
                    t514 = t415;
                }
                t055 = (t520 - (t415 << 8)) / 16;
                t115 = t055 << 4;
                t421 = ((t520 - (t415 << 8) - (t055 << 4)) % 16) << 4;
                g_FieldLayer2Tiles[t488 - g_FieldTileGroupCount].texX = t421;
                g_FieldLayer2Tiles[t488 - g_FieldTileGroupCount].texY = t115;
                g_FieldLayer2Tiles[t488 - g_FieldTileGroupCount].texU = t421 / 256.0f;
                g_FieldLayer2Tiles[t488 - g_FieldTileGroupCount].texV = t115 / 256.0f;
                t415 += 26;
                g_FieldLayer2Tiles[t488 - g_FieldTileGroupCount].textureId = t415;
                t520++;
            } else {
                if (g_FieldLayer2Tiles[t488 - g_FieldTileGroupCount].transparencyMode != 0) {
                    t415 = t155 / 256;
                    if (t415 != t061) {
                        t061 = t415;
                    }
                    t055 = (t155 - (t415 << 8)) / 16;
                    t115 = t055 << 4;
                    t421 = ((t155 - (t415 << 8) - (t055 << 4)) % 16) << 4;
                    g_FieldLayer2Tiles[t488 - g_FieldTileGroupCount].altTexX = t421;
                    g_FieldLayer2Tiles[t488 - g_FieldTileGroupCount].altTexY = t115;
                    g_FieldLayer2Tiles[t488 - g_FieldTileGroupCount].texU = t421 / 256.0f;
                    g_FieldLayer2Tiles[t488 - g_FieldTileGroupCount].texV = t115 / 256.0f;
                    t415 += 33;
                    g_FieldLayer2Tiles[t488 - g_FieldTileGroupCount].texPage = t415;
                    t155++;
                } else {
                    t415 = t295 / 256;
                    t055 = (t295 - (t415 << 8)) / 16;
                    t115 = t055 << 4;
                    t421 = ((t295 - (t415 << 8) - (t055 << 4)) % 16) << 4;
                    g_FieldLayer2Tiles[t488 - g_FieldTileGroupCount].altTexX = t421;
                    g_FieldLayer2Tiles[t488 - g_FieldTileGroupCount].altTexY = t115;
                    g_FieldLayer2Tiles[t488 - g_FieldTileGroupCount].texU = t421 / 256.0f;
                    g_FieldLayer2Tiles[t488 - g_FieldTileGroupCount].texV = t115 / 256.0f;
                    t415 += 40;
                    g_FieldLayer2Tiles[t488 - g_FieldTileGroupCount].texPage = t415;
                    t295++;
                }
            }
        }

        // Clipped 8/16-bit and packed 4-bit pixel paths.
        g_FieldLayers[t415]->enabled = 1;
        g_FieldLayers[t415]->hasDoubleSizeTiles = 0;
        t394 = 0;
        t460 = (unsigned short *)g_FieldLayers[t415]->pixels;
        if (t149 != GFX_PSX_COLOR_MODE_4BIT) {
            for (t355 = t115; t355 < t115 + 16; t355++) {
                if (t255 == 0) {
                    t409 = t421;
                    for (t388 = t195[0]; t409 < t421 + 16 && t388 < (t195[2] < (unsigned short)g_FieldTimImage1Width ? t195[2] : (unsigned short)g_FieldTimImage1Width); t409++, t388++) {
                        if (t149 == GFX_PSX_COLOR_MODE_8BIT) {
                            if (t554 != 2) {
                                g_FieldLayers[t415]->pixels[t355 * 256 + t409] = Field_RemapTransparentIndex(t289[(t195[1] + t355 - t115) * (unsigned short)g_FieldTimImage1Width + t388], g_FieldLayer2Tiles[t488 - g_FieldTileGroupCount].paletteRow);
                            } else {
                                g_FieldLayers[t415]->pixels[t355 * 256 + t409] = 1;
                            }
                        } else {
                            if (t554 != 2) {
                                t460[t355 * 256 + t409] = Field_ConvertPixelWordMode(t261[(t195[1] + t355 - t115) * (unsigned short)g_FieldTimImage1Width / 2 + t388], 2);
                            } else {
                                /* Retail caller treats `Field_RepackPixelWordMode2` return (`short`) as `unsigned short` for caller zero/sign extension. */
                                t460[t355 * 256 + t409] = ((unsigned short (__cdecl *)(unsigned short input, int mode))Field_RepackPixelWordMode2)(t261[(t195[1] + t355 - t115) * (unsigned short)g_FieldTimImage1Width / 2 + t388], 2);
                            }
                        }
                    }
                } else {
                    t409 = t421;
                    for (t388 = t195[0]; t409 < t421 + 16 && t388 < (t195[2] < (unsigned short)g_FieldTimImage2Width ? t195[2] : (unsigned short)g_FieldTimImage2Width); t409++, t388++) {
                        if (t149 == GFX_PSX_COLOR_MODE_8BIT) {
                            if (t554 != 2) {
                                g_FieldLayers[t415]->pixels[t355 * 256 + t409] = Field_RemapTransparentIndex(t494[(t195[1] + t355 - t115) * (unsigned short)g_FieldTimImage2Width + t388], g_FieldLayer2Tiles[t488 - g_FieldTileGroupCount].paletteRow);
                            } else {
                                g_FieldLayers[t415]->pixels[t355 * 256 + t409] = 1;
                            }
                        } else {
                            if (t361 != 0) {
                                if (t554 != 2) {
                                    t460[t355 * 256 + t409] = Field_ConvertPixelWordMode(t361[(t195[1] + t355 - t115) * (unsigned short)g_FieldTimImage2Width / 2 + t388], 2);
                                } else {
                                    /* Retail caller treats `Field_RepackPixelWordMode2` return (`short`) as `unsigned short` for caller zero/sign extension. */
                                    t460[t355 * 256 + t409] = ((unsigned short (__cdecl *)(unsigned short input, int mode))Field_RepackPixelWordMode2)(t361[(t195[1] + t355 - t115) * (unsigned short)g_FieldTimImage2Width / 2 + t388], 2);
                                }
                            }
                        }
                    }
                }
            }
        } else {
            for (t355 = t115; t355 < t115 + 16; t355++) {
                if (t255 == 0) {
                    t409 = t421;
                    for (t388 = t195[0]; t409 < t421 + 16 && t388 < t195[2]; t409 += 2, t388++) {
                        g_FieldLayers[t415]->pixels[t355 * 256 + t409] = Field_RemapTransparentIndex(t289[(t195[1] + t355 - t115) * (unsigned short)g_FieldTimImage1Width + t388] & 15, g_FieldLayer2Tiles[t488 - g_FieldTileGroupCount].paletteRow);
                        g_FieldLayers[t415]->pixels[t355 * 256 + t409 + 1] = Field_RemapTransparentIndex((t289[(t195[1] + t355 - t115) * (unsigned short)g_FieldTimImage1Width + t388] & 0xf0) >> 4, g_FieldLayer2Tiles[t488 - g_FieldTileGroupCount].paletteRow);
                    }
                } else {
                    t409 = t421;
                    for (t388 = t195[0]; t409 < t421 + 16 && t388 < t195[2]; t409 += 2, t388++) {
                        g_FieldLayers[t415]->pixels[t355 * 256 + t409] = Field_RemapTransparentIndex(t494[(t195[1] + t355 - t115) * (unsigned short)g_FieldTimImage2Width + t388] & 15, g_FieldLayer2Tiles[t488 - g_FieldTileGroupCount].paletteRow);
                        g_FieldLayers[t415]->pixels[t355 * 256 + t409 + 1] = Field_RemapTransparentIndex((t494[(t195[1] + t355 - t115) * (unsigned short)g_FieldTimImage2Width + t388] & 0xf0) >> 4, g_FieldLayer2Tiles[t488 - g_FieldTileGroupCount].paletteRow);
                    }
                }
            }
        }

        // Map-specific depth adjustments.
        if (g_FieldCurrentMapId == 0x8b && g_FieldLayer2Tiles[t488 - g_FieldTileGroupCount].inUse != 0 && g_FieldLayer2Tiles[t488 - g_FieldTileGroupCount].depthZ > 0.999f) {
            g_FieldLayer2Tiles[t488 - g_FieldTileGroupCount].depthZ += 0.0002f;
        }
        if (g_FieldCurrentMapId == 0x1ac && g_FieldLayer2Tiles[t488 - g_FieldTileGroupCount].depthZ > 0.993f && g_FieldLayer2Tiles[t488 - g_FieldTileGroupCount].depthZ < 0.994f) {
            g_FieldLayer2Tiles[t488 - g_FieldTileGroupCount].depthZ = 0.9933f;
        }
        if (g_FieldLayer2Tiles[t488 - g_FieldTileGroupCount].depthZ >= 0.9997f) {
            g_FieldLayer2Tiles[t488 - g_FieldTileGroupCount].depthZ -= 0.0004f;
        }
        if (g_FieldCurrentMapId == 0xae) {
            if (g_FieldLayer2Tiles[t488 - g_FieldTileGroupCount].sortKey == 0xffc) {
                g_FieldLayer2Tiles[t488 - g_FieldTileGroupCount].depthZ = 0.9983f;
            }
            if (g_FieldLayer2Tiles[t488 - g_FieldTileGroupCount].sortKey == 0xffe) {
                g_FieldLayer2Tiles[t488 - g_FieldTileGroupCount].depthZ = 0.9989f;
            }
        }
        if (g_FieldCurrentMapId == 0x2e6 && g_FieldLayer2Tiles[t488 - g_FieldTileGroupCount].paletteRow == g_FieldPaletteGridHeight + 1) {
            g_FieldLayer2Tiles[t488 - g_FieldTileGroupCount].tileX = 10000;
        }
    }

    // Fixed-record overrides, ordering and atlas ranges.
    if (g_FieldCurrentMapId == 0x1a9) {
        for (t454 = 0x124; t454 <= 0x12e; t454++) {
            g_FieldLayer2Tiles[t454].depthZ -= 0.01f;
        }
    }
    if (g_FieldCurrentMapId == 0x11f) {
        g_FieldLayer2Tiles[65].depthZ = 0.97f;
        g_FieldLayer2Tiles[37].depthZ = 0.97f;
        g_FieldLayer2Tiles[36].depthZ = 0.97f;
        g_FieldLayer2Tiles[35].depthZ = 0.97f;
    }
    if (g_FieldCurrentMapId == 0x94) {
        g_FieldLayer2Tiles[235].depthZ = 0.89f;
        g_FieldLayer2Tiles[209].depthZ = 0.89f;
    }
    if (g_FieldCurrentMapId == 0x2f5) {
        g_FieldLayer2Tiles[304].depthZ += 0.005f;
        g_FieldLayer2Tiles[237].depthZ += 0.005f;
        g_FieldLayer2Tiles[236].depthZ += 0.005f;
    }
    g_FieldLayer2ParamE04 = 1;
    Field_BuildModelDrawOrder();
    Field_BuildTileDrawOrder(g_pFieldLayer2DrawOrder, g_FieldLayer2TilesNum, g_FieldLayer2Tiles, g_FieldPaletteGridHeight);
    g_FieldLayer2StateE08.nextIndex = t161 + 1;
    g_FieldLayer2StateE10.nextIndex = t514 + 27;
    g_FieldLayer2StateE0c.sectionWord = t109 + 16;
    g_FieldLayer2StateE14.nextIndex = t061 + 34;
    t189 = t161 + 1;
    t249 = t109 + 16;
    t309 = t514 + 27;
    t221 = t061 + 34;
    if (g_FieldTileLayerIndex <= 0) {
        return 1;
    }
    // Lower 32x32 layer allocation and loop entry.
    g_FieldLayer3TilesNum = g_FieldTileLayerOffset;
    // STRING: FF7 0x009064e4
    g_FieldLayer3Tiles = (struct FieldBgLayer3Tile *)GameHeapAllocRaw(g_FieldLayer3TilesNum * 0x105c, "C:\\FF7\\field\\src\\ad_data.cpp", 0x877);
    // STRING: FF7 0x00906504
    g_pFieldLayer3DrawOrder = (struct FieldLayerDrawOrder *)GameHeapAllocRaw(g_FieldLayer3TilesNum * 4, "C:\\FF7\\field\\src\\ad_data.cpp", 0x878);
    (g_FieldLayer3StateE24.width) = 640;
    g_FieldLayer3StateE24.height = 480;
    g_FieldLayer3StateE20.originX = g_FieldLayer1StateE50[0];
    g_FieldLayer3StateE20.originY = g_FieldLayer1StateE50[1];
    t089 = g_FieldTileDrawGroupTotal;
    t049 = 0;
    t155 = 0;
    t548 = 0;
    t520 = 0;
    for (t488 = 0; t488 < g_FieldTileLayerOffset; t488++) {
        g_FieldLayer3Tiles[t488].mapId = 0;
        g_FieldLayer3Tiles[t488].xPos = g_pFieldTilePaletteBuffer[t488].xPos;
        g_FieldLayer3Tiles[t488].tileY = g_pFieldTilePaletteBuffer[t488].tileY;
        Field_DecodeClutToPaletteRow(g_pFieldTilePaletteBuffer[t488].clut, &g_FieldLayer3Tiles[t488].paletteRow);
        g_FieldLayer3Tiles[t488].fixedScale = 0x1000;
        if (t488 >= g_pFieldTileAlphaBuffer[t089].frameStart + g_pFieldTileAlphaBuffer[t089].frameCount) {
            t089++;
        }
        Field_DecodeTPage(g_pFieldTileAlphaBuffer[t089].tPage, &t149, &t321, &t349, &t554);
        g_FieldLayer3Tiles[t488].dstY = t149;
        g_FieldLayer3Tiles[t488].dstX = t554;
        if (t349 == 256) {
            switch (t149) {
            case GFX_PSX_COLOR_MODE_4BIT:
                t321 = (t321 - 1024 - g_FieldTimPixelWidth2) << 2;
                break;
            case GFX_PSX_COLOR_MODE_8BIT:
                t321 = (t321 - 1024 - g_FieldTimPixelWidth2) << 1;
                break;
            case GFX_PSX_COLOR_MODE_16BIT:
                t321 = t321 - 1024 - g_FieldTimPixelWidth2;
                break;
            }
            t255 = 1;
            t448 = t349 - g_FieldTimPixelWidth2High;
        } else {
            switch (t149) {
            case GFX_PSX_COLOR_MODE_4BIT:
                t321 = (t321 - g_FieldTimPixelWidth1) << 2;
                break;
            case GFX_PSX_COLOR_MODE_8BIT:
                t321 = (t321 - g_FieldTimPixelWidth1) << 1;
                break;
            case GFX_PSX_COLOR_MODE_16BIT:
                t321 = t321 - g_FieldTimPixelWidth1;
                break;
            }
            t448 = t349 - g_FieldTimPixelWidth1High;
            t255 = 0;
        }

        g_FieldLayer3Tiles[t488].zDepth = g_pFieldTileZBuffer[t488 + g_FieldTileGroupIndex].depth & 0x3f;
        g_FieldLayer3Tiles[t488].zFlags = g_pFieldTileZBuffer[t488 + g_FieldTileGroupIndex].flags;
        g_FieldLayer3Tiles[t488].inUse = 1;
        if ((g_pFieldTileZBuffer[t488 + g_FieldTileGroupIndex].depth & 0x80) == 0) {
            g_FieldLayer3Tiles[t488].ownerTag = 0;
        } else {
            g_FieldLayer3Tiles[t488].ownerTag = 1;
        }
        g_FieldLayer3Tiles[t488].tileWidth = 32;
        g_FieldLayer3Tiles[t488].tileHeight = 32;
        if (t149 != GFX_PSX_COLOR_MODE_4BIT) {
            SetRect(t195, t321 + g_pFieldTilePaletteBuffer[t488].offsetX, t448 + g_pFieldTilePaletteBuffer[t488].offsetY, t321 + g_pFieldTilePaletteBuffer[t488].offsetX + 32, t448 + g_pFieldTilePaletteBuffer[t488].offsetY + 32);
        } else {
            SetRect(t195, (t321 + g_pFieldTilePaletteBuffer[t488].offsetX) / 2, t448 + g_pFieldTilePaletteBuffer[t488].offsetY, (t321 + g_pFieldTilePaletteBuffer[t488].offsetX) / 2 + 16, t448 + g_pFieldTilePaletteBuffer[t488].offsetY + 32);
        }
        // Four 32x32 atlas allocation paths.
        if (t149 != GFX_PSX_COLOR_MODE_16BIT) {
            if (g_FieldLayer3Tiles[t488].ownerTag == 0) {
                t415 = t548 / 64;
                if (t415 != t161) {
                    t161 = t415;
                }
                t055 = (t548 - (t415 << 6)) / 8;
                t115 = t055 << 5;
                t421 = ((t548 - (t415 << 6) - (t055 << 3)) % 8) << 5;
                g_FieldLayer3Tiles[t488].texelU = t421;
                g_FieldLayer3Tiles[t488].texelV = t115;
                g_FieldLayer3Tiles[t488].texU = t421 / 256.0f;
                g_FieldLayer3Tiles[t488].texV = t115 / 256.0f;
                t415 += t189;
                g_FieldLayer3Tiles[t488].texId = t415;
                t548++;
            } else {
                t415 = t049 / 64;
                if (t415 != t109) {
                    t109 = t415;
                }
                t055 = (t049 - (t415 << 6)) / 8;
                t115 = t055 << 5;
                t421 = ((t049 - (t415 << 6) - (t055 << 3)) % 8) << 5;
                g_FieldLayer3Tiles[t488].tileX = t421;
                g_FieldLayer3Tiles[t488].posY = t115;
                g_FieldLayer3Tiles[t488].texU = t421 / 256.0f;
                g_FieldLayer3Tiles[t488].texV = t115 / 256.0f;
                t415 += t249;
                g_FieldLayer3Tiles[t488].mapId = t415;
                t049++;
            }
        } else {
            if (g_FieldLayer3Tiles[t488].ownerTag == 0) {
                t415 = t520 / 64;
                if (t415 != t514) {
                    t514 = t415;
                }
                t055 = (t520 - (t415 << 6)) / 8;
                t115 = t055 << 5;
                t421 = ((t520 - (t415 << 6) - (t055 << 3)) % 8) << 5;
                g_FieldLayer3Tiles[t488].texelU = t421;
                g_FieldLayer3Tiles[t488].texelV = t115;
                g_FieldLayer3Tiles[t488].texU = t421 / 256.0f;
                g_FieldLayer3Tiles[t488].texV = t115 / 256.0f;
                t415 += t309;
                g_FieldLayer3Tiles[t488].texId = t415;
                t520++;
            } else {
                t415 = t155 / 64;
                if (t415 != t061) {
                    t061 = t415;
                }
                t055 = (t155 - (t415 << 6)) / 8;
                t115 = t055 << 5;
                t421 = ((t155 - (t415 << 6) - (t055 << 3)) % 8) << 5;
                g_FieldLayer3Tiles[t488].tileX = t421;
                g_FieldLayer3Tiles[t488].posY = t115;
                g_FieldLayer3Tiles[t488].texU = t421 / 256.0f;
                g_FieldLayer3Tiles[t488].texV = t115 / 256.0f;
                t415 += t221;
                g_FieldLayer3Tiles[t488].mapId = t415;
                t155++;
            }
        }
        // Lower-layer pixels.
        g_FieldLayers[t415]->enabled = 1;
        g_FieldLayers[t415]->hasDoubleSizeTiles = 1;
        t394 = 0;
        t460 = (unsigned short *)g_FieldLayers[t415]->pixels;
        if (t149 != GFX_PSX_COLOR_MODE_4BIT) {
            for (t355 = t115; t355 < t115 + 32; t355++) {
                if (t255 == 0) {
                    t409 = t421;
                    for (t388 = t195[0]; t409 < t421 + 32 && t388 < (t195[2] < (unsigned short)g_FieldTimImage1Width ? t195[2] : (unsigned short)g_FieldTimImage1Width); t409++, t388++) {
                        if (t149 == GFX_PSX_COLOR_MODE_8BIT) {
                            g_FieldLayers[t415]->pixels[t355 * 256 + t409] = Field_RemapTransparentIndex(t289[(t195[1] + t355 - t115) * (unsigned short)g_FieldTimImage1Width + t388], g_FieldLayer3Tiles[t488].paletteRow);
                        } else {
                            t460[t355 * 256 + t409] = Field_ConvertPixelWordMode(t261[(t195[1] + t355 - t115) * (unsigned short)g_FieldTimImage1Width / 2 + t388], 2);
                        }
                    }
                } else {
                    t409 = t421;
                    for (t388 = t195[0]; t409 < t421 + 32 && t388 < (t195[2] < (unsigned short)g_FieldTimImage2Width ? t195[2] : (unsigned short)g_FieldTimImage2Width); t409++, t388++) {
                        if (t149 == GFX_PSX_COLOR_MODE_8BIT) {
                            g_FieldLayers[t415]->pixels[t355 * 256 + t409] = Field_RemapTransparentIndex(t494[(t195[1] + t355 - t115) * (unsigned short)g_FieldTimImage2Width + t388], g_FieldLayer3Tiles[t488].paletteRow);
                        } else {
                            if (t361 != 0) {
                                t460[t355 * 256 + t409] = Field_ConvertPixelWordMode(t361[(t195[1] + t355 - t115) * (unsigned short)g_FieldTimImage2Width / 2 + t388], 2);
                            }
                        }
                    }
                }
            }
        } else {
            for (t355 = t115; t355 < t115 + 32; t355++) {
                if (t255 == 0) {
                    t409 = t421;
                    for (t388 = t195[0]; t409 < t421 + 32 && t388 < t195[2]; t388++) {
                        g_FieldLayers[t415]->pixels[t355 * 256 + t409] = Field_RemapTransparentIndex(t289[(t195[1] + t355 - t115) * (unsigned short)g_FieldTimImage1Width + t388] & 15, g_FieldLayer3Tiles[t488].paletteRow);
                        g_FieldLayers[t415]->pixels[t355 * 256 + t409 + 1] = Field_RemapTransparentIndex((t289[(t195[1] + t355 - t115) * (unsigned short)g_FieldTimImage1Width + t388] & 0xf0) >> 4, g_FieldLayer3Tiles[t488].paletteRow);
                        t409 += 2;
                    }
                } else {
                    t409 = t421;
                    for (t388 = t195[0]; t409 < t421 + 32 && t388 < t195[2]; t388++) {
                        g_FieldLayers[t415]->pixels[t355 * 256 + t409] = Field_RemapTransparentIndex(t494[(t195[1] + t355 - t115) * (unsigned short)g_FieldTimImage2Width + t388] & 15, g_FieldLayer3Tiles[t488].paletteRow);
                        g_FieldLayers[t415]->pixels[t355 * 256 + t409 + 1] = Field_RemapTransparentIndex((t494[(t195[1] + t355 - t115) * (unsigned short)g_FieldTimImage2Width + t388] & 0xf0) >> 4, g_FieldLayer3Tiles[t488].paletteRow);
                        t409 += 2;
                    }
                }
            }
        }
    }
    g_FieldDoDrawLayer3 = 1;
    Field_BuildTileDrawOrder(g_pFieldLayer3DrawOrder, g_FieldLayer3TilesNum, g_FieldLayer3Tiles, g_FieldPaletteGridHeight);
    // Upper layer allocation and atlas continuation.
    if (g_FieldTileLayerIndex - g_FieldTileLayerOffset <= 0) {
        return 1;
    }
    g_FieldLayer4TilesNum = g_FieldTileLayerIndex - g_FieldTileLayerOffset;
    // STRING: FF7 0x00906524
    g_FieldLayer4Tiles = (struct FieldBgLayer4Tile *)GameHeapAllocRaw(g_FieldLayer4TilesNum * 0x105c, "C:\\FF7\\field\\src\\ad_data.cpp", 0x974);
    // STRING: FF7 0x00906544
    g_pFieldLayer4DrawOrder = (struct FieldLayerDrawOrder *)GameHeapAllocRaw(g_FieldLayer4TilesNum * 4, "C:\\FF7\\field\\src\\ad_data.cpp", 0x975);
    if (g_FieldTileLayerOffset != 0) {
        t189 += t161 + 1;
        t249 += t109 + 1;
        t309 += t514 + 1;
        t221 += t061 + 1;
        t049 = 0;
        t155 = 0;
        t548 = 0;
        t520 = 0;
    }
    (g_FieldLayer4StateE8c.width) = 640;
    g_FieldLayer4StateE8c.height = 480;
    g_FieldLayer4StateE88.originX = g_FieldLayer1StateE50[0];
    g_FieldLayer4StateE88.originY = g_FieldLayer1StateE50[1];
    g_FieldLayer4StateE98.pageStart = t249 + t109;
    g_FieldLayer4StateE9c.directPageStart = t221 + t061;
    for (t488 = g_FieldTileLayerOffset; t488 < g_FieldTileLayerIndex; t488++) {
        g_FieldLayer4Tiles[t488 - g_FieldTileLayerOffset].posX = g_pFieldTilePaletteBuffer[t488].xPos;
        g_FieldLayer4Tiles[t488 - g_FieldTileLayerOffset].posY = g_pFieldTilePaletteBuffer[t488].tileY;
        Field_DecodeClutToPaletteRow(g_pFieldTilePaletteBuffer[t488].clut, &g_FieldLayer4Tiles[t488 - g_FieldTileLayerOffset].paletteRow);
        if (t488 >= g_pFieldTileAlphaBuffer[t089].frameStart + g_pFieldTileAlphaBuffer[t089].frameCount) {
            t089++;
        }
        Field_DecodeTPage(g_pFieldTileAlphaBuffer[t089].tPage, &t149, &t321, &t349, &t554);
        g_FieldLayer4Tiles[t488 - g_FieldTileLayerOffset].dstY = t149;
        g_FieldLayer4Tiles[t488 - g_FieldTileLayerOffset].dstX = t554;
        if (t554 == 2) {
            g_FieldLayer4Tiles[t488 - g_FieldTileLayerOffset].dstX = 0;
            g_FieldLayer4Tiles[t488 - g_FieldTileLayerOffset].paletteRow = g_FieldPaletteGridHeight + 1;
        }
        if (t349 == 256) {
            switch (t149) {
            case GFX_PSX_COLOR_MODE_4BIT:
                t321 = (t321 - 1024 - g_FieldTimPixelWidth2) << 2;
                break;
            case GFX_PSX_COLOR_MODE_8BIT:
                t321 = (t321 - 1024 - g_FieldTimPixelWidth2) << 1;
                break;
            case GFX_PSX_COLOR_MODE_16BIT:
                t321 = t321 - 1024 - g_FieldTimPixelWidth2;
                break;
            }
            t255 = 1;
            t448 = t349 - g_FieldTimPixelWidth2High;
        } else {
            switch (t149) {
            case GFX_PSX_COLOR_MODE_4BIT:
                t321 = (t321 - g_FieldTimPixelWidth1) << 2;
                break;
            case GFX_PSX_COLOR_MODE_8BIT:
                t321 = (t321 - g_FieldTimPixelWidth1) << 1;
                break;
            case GFX_PSX_COLOR_MODE_16BIT:
                t321 = t321 - g_FieldTimPixelWidth1;
                break;
            }
            t448 = t349 - g_FieldTimPixelWidth1High;
            t255 = 0;
        }

        g_FieldLayer4Tiles[t488 - g_FieldTileLayerOffset].zDepth = g_pFieldTileZBuffer[t488 + g_FieldTileGroupIndex].depth & 0x3f;
        g_FieldLayer4Tiles[t488 - g_FieldTileLayerOffset].zFlags = g_pFieldTileZBuffer[t488 + g_FieldTileGroupIndex].flags;
        g_FieldLayer4Tiles[t488 - g_FieldTileLayerOffset].inUse = 1;
        if ((g_pFieldTileZBuffer[t488 + g_FieldTileGroupIndex].depth & 0x80) == 0) {
            g_FieldLayer4Tiles[t488 - g_FieldTileLayerOffset].active = 0;
        } else {
            g_FieldLayer4Tiles[t488 - g_FieldTileLayerOffset].active = 1;
        }
        g_FieldLayer4Tiles[t488 - g_FieldTileLayerOffset].tileWidth = 32;
        g_FieldLayer4Tiles[t488 - g_FieldTileLayerOffset].tileHeight = 32;
        if (t149 != GFX_PSX_COLOR_MODE_4BIT) {
            SetRect(t195, t321 + g_pFieldTilePaletteBuffer[t488].offsetX, t448 + g_pFieldTilePaletteBuffer[t488].offsetY, t321 + g_pFieldTilePaletteBuffer[t488].offsetX + 32, t448 + g_pFieldTilePaletteBuffer[t488].offsetY + 32);
        } else {
            SetRect(t195, (t321 + g_pFieldTilePaletteBuffer[t488].offsetX) / 2, t448 + g_pFieldTilePaletteBuffer[t488].offsetY, (t321 + g_pFieldTilePaletteBuffer[t488].offsetX) / 2 + 16, t448 + g_pFieldTilePaletteBuffer[t488].offsetY + 32);
        }
        g_FieldLayer4Tiles[t488 - g_FieldTileLayerOffset].fixedScale = 0;
        // Upper-layer atlas allocation.
        if (t149 != GFX_PSX_COLOR_MODE_16BIT) {
            if (g_FieldLayer4Tiles[t488 - g_FieldTileLayerOffset].active == 0) {
                t415 = t548 / 64;
                if (t415 != t161) {
                    t161 = t415;
                }
                t055 = (t548 - (t415 << 6)) / 8;
                t115 = t055 << 5;
                t421 = ((t548 - (t415 << 6) - (t055 << 3)) % 8) << 5;
                g_FieldLayer4Tiles[t488 - g_FieldTileLayerOffset].texU = t421;
                g_FieldLayer4Tiles[t488 - g_FieldTileLayerOffset].texV = t115;
                g_FieldLayer4Tiles[t488 - g_FieldTileLayerOffset].texUNorm = t421 / 256.0f;
                g_FieldLayer4Tiles[t488 - g_FieldTileLayerOffset].texVNorm = t115 / 256.0f;
                t415 += t189;
                g_FieldLayer4Tiles[t488 - g_FieldTileLayerOffset].textureId = t415;
                t548++;
            } else {
                t415 = t049 / 64;
                if (t415 != t109) {
                    t109 = t415;
                }
                t055 = (t049 - (t415 << 6)) / 8;
                t115 = t055 << 5;
                t421 = ((t049 - (t415 << 6) - (t055 << 3)) % 8) << 5;
                g_FieldLayer4Tiles[t488 - g_FieldTileLayerOffset].tileX = t421;
                g_FieldLayer4Tiles[t488 - g_FieldTileLayerOffset].tileY = t115;
                g_FieldLayer4Tiles[t488 - g_FieldTileLayerOffset].texUNorm = t421 / 256.0f;
                g_FieldLayer4Tiles[t488 - g_FieldTileLayerOffset].texVNorm = t115 / 256.0f;
                t415 += t249;
                g_FieldLayer4Tiles[t488 - g_FieldTileLayerOffset].texSlot = t415;
                t049++;
            }
        } else {
            if (g_FieldLayer4Tiles[t488 - g_FieldTileLayerOffset].active == 0) {
                t415 = t520 / 64;
                if (t415 != t514) {
                    t514 = t415;
                }
                t055 = (t520 - (t415 << 6)) / 8;
                t115 = t055 << 5;
                // The original reads t121 here, not the page counter t520.
                t421 = ((t121 - (t415 << 6) - (t055 << 3)) % 8) << 5;
                g_FieldLayer4Tiles[t488 - g_FieldTileLayerOffset].texU = t421;
                g_FieldLayer4Tiles[t488 - g_FieldTileLayerOffset].texV = t115;
                g_FieldLayer4Tiles[t488 - g_FieldTileLayerOffset].texUNorm = t421 / 256.0f;
                g_FieldLayer4Tiles[t488 - g_FieldTileLayerOffset].texVNorm = t115 / 256.0f;
                t415 += t309;
                g_FieldLayer4Tiles[t488 - g_FieldTileLayerOffset].textureId = t415;
                t520++;
            } else {
                t415 = t155 / 64;
                if (t415 != t061) {
                    t061 = t415;
                }
                t055 = (t155 - (t415 << 6)) / 8;
                t115 = t055 << 5;
                t421 = ((t155 - (t415 << 6) - (t055 << 3)) % 8) << 5;
                g_FieldLayer4Tiles[t488 - g_FieldTileLayerOffset].tileX = t421;
                g_FieldLayer4Tiles[t488 - g_FieldTileLayerOffset].tileY = t115;
                g_FieldLayer4Tiles[t488 - g_FieldTileLayerOffset].texUNorm = t421 / 256.0f;
                g_FieldLayer4Tiles[t488 - g_FieldTileLayerOffset].texVNorm = t115 / 256.0f;
                t415 += t221;
                g_FieldLayer4Tiles[t488 - g_FieldTileLayerOffset].texSlot = t415;
                t155++;
            }
        }
        // Upper-layer pixels and loop back edge.
        g_FieldLayers[t415]->enabled = 1;
        g_FieldLayers[t415]->hasDoubleSizeTiles = 1;
        t394 = 0;
        t460 = (unsigned short *)g_FieldLayers[t415]->pixels;
        if (t149 != GFX_PSX_COLOR_MODE_4BIT) {
            for (t355 = t115; t355 < t115 + 32; t355++) {
                if (t255 == 0) {
                    t409 = t421;
                    for (t388 = t195[0]; t409 < t421 + 32 && t388 < (t195[2] < (unsigned short)g_FieldTimImage1Width ? t195[2] : (unsigned short)g_FieldTimImage1Width); t409++, t388++) {
                        if (t149 == GFX_PSX_COLOR_MODE_8BIT) {
                            if (t554 != 2) {
                                g_FieldLayers[t415]->pixels[t355 * 256 + t409] = Field_RemapTransparentIndex(t289[(t195[1] + t355 - t115) * (unsigned short)g_FieldTimImage1Width + t388], g_FieldLayer4Tiles[t488 - g_FieldTileLayerOffset].paletteRow);
                            } else {
                                g_FieldLayers[t415]->pixels[t355 * 256 + t409] = 1;
                            }
                        } else {
                            if (t554 != 2) {
                                t460[t355 * 256 + t409] = Field_ConvertPixelWordMode(t261[(t195[1] + t355 - t115) * (unsigned short)g_FieldTimImage1Width / 2 + t388], 2);
                            } else {
                                /* Retail caller treats `Field_RepackPixelWordMode2` return (`short`) as `unsigned short` for caller zero/sign extension. */
                                t460[t355 * 256 + t409] = ((unsigned short (__cdecl *)(unsigned short input, int mode))Field_RepackPixelWordMode2)(t261[(t195[1] + t355 - t115) * (unsigned short)g_FieldTimImage1Width / 2 + t388], 2);
                            }
                        }
                    }
                } else {
                    t409 = t421;
                    for (t388 = t195[0]; t409 < t421 + 32 && t388 < (t195[2] < (unsigned short)g_FieldTimImage2Width ? t195[2] : (unsigned short)g_FieldTimImage2Width); t409++, t388++) {
                        if (t149 == GFX_PSX_COLOR_MODE_8BIT) {
                            if (t554 != 2) {
                                g_FieldLayers[t415]->pixels[t355 * 256 + t409] = Field_RemapTransparentIndex(t494[(t195[1] + t355 - t115) * (unsigned short)g_FieldTimImage2Width + t388], g_FieldLayer4Tiles[t488 - g_FieldTileLayerOffset].paletteRow);
                            } else {
                                g_FieldLayers[t415]->pixels[t355 * 256 + t409] = 1;
                            }
                        } else {
                            if (t361 != 0) {
                                if (t554 != 2) {
                                    t460[t355 * 256 + t409] = Field_ConvertPixelWordMode(t361[(t195[1] + t355 - t115) * (unsigned short)g_FieldTimImage2Width / 2 + t388], 2);
                                } else {
                                    /* Retail caller treats `Field_RepackPixelWordMode2` return (`short`) as `unsigned short` for caller zero/sign extension. */
                                    t460[t355 * 256 + t409] = ((unsigned short (__cdecl *)(unsigned short input, int mode))Field_RepackPixelWordMode2)(t361[(t195[1] + t355 - t115) * (unsigned short)g_FieldTimImage2Width / 2 + t388], 2);
                                }
                            }
                        }
                    }
                }
            }
        } else {
            for (t355 = t115; t355 < t115 + 32; t355++) {
                if (t255 == 0) {
                    t409 = t421;
                    for (t388 = t195[0]; t409 < t421 + 32 && t388 < t195[2]; t388++) {
                        g_FieldLayers[t415]->pixels[t355 * 256 + t409] = Field_RemapTransparentIndex(t289[(t195[1] + t355 - t115) * (unsigned short)g_FieldTimImage1Width + t388] & 15, g_FieldLayer4Tiles[t488 - g_FieldTileLayerOffset].paletteRow);
                        g_FieldLayers[t415]->pixels[t355 * 256 + t409 + 1] = Field_RemapTransparentIndex((t289[(t195[1] + t355 - t115) * (unsigned short)g_FieldTimImage1Width + t388] & 0xf0) >> 4, g_FieldLayer4Tiles[t488 - g_FieldTileLayerOffset].paletteRow);
                        t409 += 2;
                    }
                } else {
                    t409 = t421;
                    for (t388 = t195[0]; t409 < t421 + 32 && t388 < t195[2]; t388++) {
                        g_FieldLayers[t415]->pixels[t355 * 256 + t409] = Field_RemapTransparentIndex(t494[(t195[1] + t355 - t115) * (unsigned short)g_FieldTimImage2Width + t388] & 15, g_FieldLayer4Tiles[t488 - g_FieldTileLayerOffset].paletteRow);
                        g_FieldLayers[t415]->pixels[t355 * 256 + t409 + 1] = Field_RemapTransparentIndex((t494[(t195[1] + t355 - t115) * (unsigned short)g_FieldTimImage2Width + t388] & 0xf0) >> 4, g_FieldLayer4Tiles[t488 - g_FieldTileLayerOffset].paletteRow);
                        t409 += 2;
                    }
                }
            }
        }
    }
    // Publish upper atlas ranges and final draw order.
    (g_FieldLayer4StateE9c.pageEnd) = t109 + t249 + 1;
    g_FieldLayer4StateEa0 = t061 + t221 + 1;
    g_FieldDoDrawLayer4 = 1;
    Field_BuildTileDrawOrder(g_pFieldLayer4DrawOrder, g_FieldLayer4TilesNum, g_FieldLayer4Tiles, g_FieldPaletteGridHeight);
    return 1;
}

/*
 * Writes sixteen 0x10-byte rows out of the block g_FieldLayers[15]'s
 * second dword points at, starting 0xB000 in, to a file called
 * "tst.tst" -- a scratch dump, not something the game reads back.
 */
// FUNCTION: FF7 0x0062a070
void __cdecl Field_DebugDumpTexturePage(void)
{
    FILE *out;
    unsigned short i;
    int junk;

    out = Field_FileOpen(
        // STRING: FF7 0x00906568
        "tst.tst",
        // STRING: FF7 0x00906564
        "wb");
    for (i = 0xb0; i < 0xc0; i++) {
        Field_FileWrite(
            out, g_FieldLayers[15]->pixels + (i << 8), 0x10);
    }
    Field_FileClose(out);
}

/*
 * Serializes layer records and texture pages to the map .fda file; returns success/failure.
 * Reverse declarations within VC5 bucket zero preserve the buffers and scalar stack slots.
 */
// FUNCTION: FF7 0x0062a0e7
int __cdecl Field_DebugDumpFieldState(void)
{
    char fileName[0x80];
    int val;
    int flags;
    int frameIndex;
    FILE *fileCtx;
    char leafName[0x20];
    int rem;
    unsigned short idx;

    idx = 0;
    if (g_FieldTileStateDc4 != 0) {
        return 0;
    }
    if (Field_GetMapNameById(g_FieldCurrentMapId, leafName) == 0) {
        return 0;
    }
    strcpy(fileName, g_FieldDataDir);
    strcat(fileName, leafName);
    strcat(fileName,
           // STRING: FF7 0x00906570
           "\\");
    strcat(fileName, leafName);
    strcat(fileName,
           // STRING: FF7 0x00906574
           ".fda");
    fileCtx = Field_FileOpen(fileName,
                                  // STRING: FF7 0x0090657c
                                  "wb");
    if (fileCtx == 0) {
        return 0;
    }
    Field_DebugWriteU16(fileCtx, 0);
    Field_DebugWriteU16(fileCtx, g_FieldTileStateDc0);
    Field_DebugWriteU8(fileCtx, (unsigned char)g_FieldCameraScrollE74);
    Field_FileWrite(fileCtx,
                         // STRING: FF7 0x00906580
                         "PALETTE", 7);
    for (frameIndex = 0; frameIndex < 20; frameIndex++) {
        Field_DebugWriteU8(fileCtx, g_FieldPaletteFlags[frameIndex]);
    }
    Field_DebugWriteU16(fileCtx, idx);
    Field_DebugWriteU16(fileCtx, idx);
    Field_FileWrite(fileCtx,
                         // STRING: FF7 0x00906588
                         "BACK", 4);
    if (g_FieldCameraScrollE74 != 0) {
        Field_DebugWriteU16(fileCtx, g_FieldLayer1StateE58.spanWidth);
        Field_DebugWriteU16(fileCtx, *&g_FieldLayer1StateE58.spanHeight);
        Field_DebugWriteU16(fileCtx, g_FieldLayer1TilesNum);
        Field_DebugWriteU16(fileCtx, g_FieldCameraScrollE70.clutId);
        Field_DebugWriteU16(fileCtx, idx);
        Field_DebugWriteU16(fileCtx, idx);
        for (frameIndex = 0; frameIndex < g_FieldLayer1TilesNum; frameIndex++) {
            Field_DebugWriteU16(fileCtx, *(unsigned short *)&g_FieldLayer1Tiles[frameIndex].posX);
            Field_DebugWriteU16(fileCtx, *(unsigned short *)&g_FieldLayer1Tiles[frameIndex].depth);
            Field_DebugWriteU16(fileCtx, g_FieldLayer1Tiles[frameIndex].srcX);
            Field_DebugWriteU16(fileCtx, g_FieldLayer1Tiles[frameIndex].srcY);
            Field_DebugWriteU16(fileCtx, g_FieldLayer1Tiles[frameIndex].texX);
            Field_DebugWriteU16(fileCtx, g_FieldLayer1Tiles[frameIndex].texY);
            Field_DebugWriteU16(fileCtx, g_FieldLayer1Tiles[frameIndex].altTexX);
            Field_DebugWriteU16(fileCtx, g_FieldLayer1Tiles[frameIndex].altTexY);
            Field_DebugWriteU16(fileCtx, g_FieldLayer1Tiles[frameIndex].width);
            Field_DebugWriteU16(fileCtx, g_FieldLayer1Tiles[frameIndex].height);
            Field_DebugWriteU16(fileCtx, g_FieldLayer1Tiles[frameIndex].paletteRow);
            Field_DebugWriteU16(fileCtx, g_FieldLayer1Tiles[frameIndex].sortKey);
            Field_DebugWriteU8(fileCtx, g_FieldLayer1Tiles[frameIndex].zBufferValue);
            Field_DebugWriteU8(fileCtx, g_FieldLayer1Tiles[frameIndex].visibilityMask);
            if (g_FieldLayer1Tiles[frameIndex].active != 0) {
                Field_DebugWriteU8(fileCtx, 1);
            } else {
                Field_DebugWriteU8(fileCtx, 0);
            }
            if (g_FieldLayer1Tiles[frameIndex].isSet != 0) {
                Field_DebugWriteU8(fileCtx, 1);
            } else {
                Field_DebugWriteU8(fileCtx, 0);
            }
            Field_DebugWriteU16(fileCtx, g_FieldLayer1Tiles[frameIndex].transparencyMode);
            Field_DebugWriteU16(fileCtx, g_FieldLayer1Tiles[frameIndex].mapId);
            Field_DebugWriteU16(fileCtx, g_FieldLayer1Tiles[frameIndex].texPage);
            Field_DebugWriteU16(fileCtx, g_FieldLayer1Tiles[frameIndex].pixelMode);
            val = (int)(g_FieldLayer1Tiles[frameIndex].scaleFactor * 10000000.0f);
            Field_DebugWriteU32(fileCtx, val);
            val = (int)(g_FieldLayer1Tiles[frameIndex].texU * 10000000.0f);
            Field_DebugWriteU32(fileCtx, val);
            val = (int)(g_FieldLayer1Tiles[frameIndex].texV * 10000000.0f);
            Field_DebugWriteU32(fileCtx, val);
            Field_DebugWriteU16(fileCtx, idx);
            Field_DebugWriteU16(fileCtx, idx);
        }
    }
    if (g_FieldLayer2ParamE04 != 0) {
        Field_DebugWriteU8(fileCtx, 1);
    } else {
        Field_DebugWriteU8(fileCtx, 0);
    }
    if (g_FieldLayer2ParamE04 != 0) {
        Field_DebugWriteU16(fileCtx, (g_FieldLayer2StateDf8.width));
        Field_DebugWriteU16(fileCtx, g_FieldLayer2StateDf8.height);
        Field_DebugWriteU16(fileCtx, g_FieldLayer2TilesNum);
        Field_DebugWriteU16(fileCtx, g_FieldLayer2StateE08.firstIndex);
        Field_DebugWriteU16(fileCtx, g_FieldLayer2StateE08.nextIndex);
        Field_DebugWriteU16(fileCtx, *(unsigned short *)&g_FieldLayer2StateE0c);
        Field_DebugWriteU16(fileCtx, g_FieldLayer2StateE0c.sectionWord);
        Field_DebugWriteU16(fileCtx, g_FieldLayer2StateE10.firstIndex);
        Field_DebugWriteU16(fileCtx, g_FieldLayer2StateE10.nextIndex);
        Field_DebugWriteU16(fileCtx, g_FieldLayer2StateE14.firstIndex);
        Field_DebugWriteU16(fileCtx, g_FieldLayer2StateE14.nextIndex);
        Field_DebugWriteU16(fileCtx, idx);
        Field_DebugWriteU16(fileCtx, idx);
        for (frameIndex = 0; frameIndex < g_FieldLayer2TilesNum; frameIndex++) {
            Field_DebugWriteU16(fileCtx, *(unsigned short *)&g_FieldLayer2Tiles[frameIndex].tileX);
            Field_DebugWriteU16(fileCtx, *(unsigned short *)&g_FieldLayer2Tiles[frameIndex].spanY);
            Field_DebugWriteU16(fileCtx, g_FieldLayer2Tiles[frameIndex].srcX);
            Field_DebugWriteU16(fileCtx, g_FieldLayer2Tiles[frameIndex].srcY);
            Field_DebugWriteU16(fileCtx, g_FieldLayer2Tiles[frameIndex].texX);
            Field_DebugWriteU16(fileCtx, g_FieldLayer2Tiles[frameIndex].texY);
            Field_DebugWriteU16(fileCtx, g_FieldLayer2Tiles[frameIndex].altTexX);
            Field_DebugWriteU16(fileCtx, g_FieldLayer2Tiles[frameIndex].altTexY);
            Field_DebugWriteU16(fileCtx, g_FieldLayer2Tiles[frameIndex].width);
            Field_DebugWriteU16(fileCtx, g_FieldLayer2Tiles[frameIndex].height);
            Field_DebugWriteU16(fileCtx, g_FieldLayer2Tiles[frameIndex].paletteRow);
            Field_DebugWriteU16(fileCtx, g_FieldLayer2Tiles[frameIndex].sortKey);
            Field_DebugWriteU8(fileCtx, g_FieldLayer2Tiles[frameIndex].zBufferValue);
            Field_DebugWriteU8(fileCtx, g_FieldLayer2Tiles[frameIndex].visibilityMask);
            if (g_FieldLayer2Tiles[frameIndex].inUse != 0) {
                Field_DebugWriteU8(fileCtx, 1);
            } else {
                Field_DebugWriteU8(fileCtx, 0);
            }
            if (g_FieldLayer2Tiles[frameIndex].usesSecondTim != 0) {
                Field_DebugWriteU8(fileCtx, 1);
            } else {
                Field_DebugWriteU8(fileCtx, 0);
            }
            Field_DebugWriteU16(fileCtx, g_FieldLayer2Tiles[frameIndex].transparencyMode);
            Field_DebugWriteU16(fileCtx, g_FieldLayer2Tiles[frameIndex].textureId);
            Field_DebugWriteU16(fileCtx, g_FieldLayer2Tiles[frameIndex].texPage);
            Field_DebugWriteU16(fileCtx, g_FieldLayer2Tiles[frameIndex].colorMode);
            val = (int)(g_FieldLayer2Tiles[frameIndex].depthZ * 10000000.0f);
            Field_DebugWriteU32(fileCtx, val);
            val = (int)(g_FieldLayer2Tiles[frameIndex].texU * 10000000.0f);
            Field_DebugWriteU32(fileCtx, val);
            val = (int)(g_FieldLayer2Tiles[frameIndex].texV * 10000000.0f);
            Field_DebugWriteU32(fileCtx, val);
            Field_DebugWriteU16(fileCtx, idx);
            Field_DebugWriteU16(fileCtx, idx);
        }
    }
    if (g_FieldDoDrawLayer3 != 0) {
        Field_DebugWriteU8(fileCtx, 1);
    } else {
        Field_DebugWriteU8(fileCtx, 0);
    }
    if (g_FieldDoDrawLayer3 != 0) {
        Field_DebugWriteU16(fileCtx, (g_FieldLayer3StateE24.width));
        Field_DebugWriteU16(fileCtx, g_FieldLayer3StateE24.height);
        Field_DebugWriteU16(fileCtx, g_FieldLayer3TilesNum);
        Field_DebugWriteU16(fileCtx, (g_FieldLayer3StateE30.firstIndex));
        Field_DebugWriteU16(fileCtx, g_FieldLayer3StateE30.nextIndex);
        Field_DebugWriteU16(fileCtx, g_FieldLayer3StateE34.firstIndex);
        Field_DebugWriteU16(fileCtx, g_FieldLayer3StateE34.nextIndex);
        Field_DebugWriteU16(fileCtx, g_FieldLayer3StateE38);
        Field_DebugWriteU16(fileCtx, idx);
        Field_DebugWriteU16(fileCtx, idx);
        for (frameIndex = 0; frameIndex < g_FieldLayer3TilesNum; frameIndex++) {
            Field_DebugWriteU16(fileCtx, *(unsigned short *)&g_FieldLayer3Tiles[frameIndex].xPos);
            Field_DebugWriteU16(fileCtx, *(unsigned short *)&g_FieldLayer3Tiles[frameIndex].tileY);
            Field_DebugWriteU16(fileCtx, g_FieldLayer3Tiles[frameIndex].srcX);
            Field_DebugWriteU16(fileCtx, g_FieldLayer3Tiles[frameIndex].srcY);
            Field_DebugWriteU16(fileCtx, g_FieldLayer3Tiles[frameIndex].texelU);
            Field_DebugWriteU16(fileCtx, g_FieldLayer3Tiles[frameIndex].texelV);
            Field_DebugWriteU16(fileCtx, g_FieldLayer3Tiles[frameIndex].tileX);
            Field_DebugWriteU16(fileCtx, g_FieldLayer3Tiles[frameIndex].posY);
            Field_DebugWriteU16(fileCtx, g_FieldLayer3Tiles[frameIndex].tileWidth);
            Field_DebugWriteU16(fileCtx, g_FieldLayer3Tiles[frameIndex].tileHeight);
            Field_DebugWriteU16(fileCtx, g_FieldLayer3Tiles[frameIndex].paletteRow);
            Field_DebugWriteU16(fileCtx, g_FieldLayer3Tiles[frameIndex].fixedScale);
            Field_DebugWriteU8(fileCtx, g_FieldLayer3Tiles[frameIndex].zDepth);
            Field_DebugWriteU8(fileCtx, g_FieldLayer3Tiles[frameIndex].zFlags);
            if (g_FieldLayer3Tiles[frameIndex].ownerTag != 0) {
                Field_DebugWriteU8(fileCtx, 1);
            } else {
                Field_DebugWriteU8(fileCtx, 0);
            }
            if (g_FieldLayer3Tiles[frameIndex].flags != 0) {
                Field_DebugWriteU8(fileCtx, 1);
            } else {
                Field_DebugWriteU8(fileCtx, 0);
            }
            Field_DebugWriteU16(fileCtx, g_FieldLayer3Tiles[frameIndex].dstX);
            Field_DebugWriteU16(fileCtx, g_FieldLayer3Tiles[frameIndex].texId);
            Field_DebugWriteU16(fileCtx, g_FieldLayer3Tiles[frameIndex].mapId);
            Field_DebugWriteU16(fileCtx, g_FieldLayer3Tiles[frameIndex].dstY);
            val = (int)(g_FieldLayer3Tiles[frameIndex].scale * 10000000.0f);
            Field_DebugWriteU32(fileCtx, val);
            val = (int)(g_FieldLayer3Tiles[frameIndex].texU * 10000000.0f);
            Field_DebugWriteU32(fileCtx, val);
            val = (int)(g_FieldLayer3Tiles[frameIndex].texV * 10000000.0f);
            Field_DebugWriteU32(fileCtx, val);
            Field_DebugWriteU16(fileCtx, idx);
            Field_DebugWriteU16(fileCtx, idx);
        }
    }
    if (g_FieldDoDrawLayer4 != 0) {
        Field_DebugWriteU8(fileCtx, 1);
    } else {
        Field_DebugWriteU8(fileCtx, 0);
    }
    if (g_FieldDoDrawLayer4 != 0) {
        Field_DebugWriteU16(fileCtx, (g_FieldLayer4StateE8c.width));
        Field_DebugWriteU16(fileCtx, g_FieldLayer4StateE8c.height);
        Field_DebugWriteU16(fileCtx, g_FieldLayer4TilesNum);
        Field_DebugWriteU16(fileCtx, (g_FieldLayer4StateE98.headerWord));
        Field_DebugWriteU16(fileCtx, *(unsigned short *)&g_FieldLayer4StateE98.pageStart);
        Field_DebugWriteU16(fileCtx, (g_FieldLayer4StateE9c.pageEnd));
        Field_DebugWriteU16(fileCtx, g_FieldLayer4StateE9c.directPageStart);
        Field_DebugWriteU16(fileCtx, g_FieldLayer4StateEa0);
        Field_DebugWriteU16(fileCtx, idx);
        Field_DebugWriteU16(fileCtx, idx);
        for (frameIndex = 0; frameIndex < g_FieldLayer4TilesNum; frameIndex++) {
            Field_DebugWriteU16(fileCtx, *(unsigned short *)&g_FieldLayer4Tiles[frameIndex].posX);
            Field_DebugWriteU16(fileCtx, *(unsigned short *)&g_FieldLayer4Tiles[frameIndex].posY);
            Field_DebugWriteU16(fileCtx, g_FieldLayer4Tiles[frameIndex].srcX);
            Field_DebugWriteU16(fileCtx, g_FieldLayer4Tiles[frameIndex].srcY);
            Field_DebugWriteU16(fileCtx, g_FieldLayer4Tiles[frameIndex].texU);
            Field_DebugWriteU16(fileCtx, g_FieldLayer4Tiles[frameIndex].texV);
            Field_DebugWriteU16(fileCtx, g_FieldLayer4Tiles[frameIndex].tileX);
            Field_DebugWriteU16(fileCtx, g_FieldLayer4Tiles[frameIndex].tileY);
            Field_DebugWriteU16(fileCtx, g_FieldLayer4Tiles[frameIndex].tileWidth);
            Field_DebugWriteU16(fileCtx, g_FieldLayer4Tiles[frameIndex].tileHeight);
            Field_DebugWriteU16(fileCtx, g_FieldLayer4Tiles[frameIndex].paletteRow);
            Field_DebugWriteU16(fileCtx, g_FieldLayer4Tiles[frameIndex].fixedScale);
            Field_DebugWriteU8(fileCtx, g_FieldLayer4Tiles[frameIndex].zDepth);
            Field_DebugWriteU8(fileCtx, g_FieldLayer4Tiles[frameIndex].zFlags);
            if (g_FieldLayer4Tiles[frameIndex].active != 0) {
                Field_DebugWriteU8(fileCtx, 1);
            } else {
                Field_DebugWriteU8(fileCtx, 0);
            }
            if (g_FieldLayer4Tiles[frameIndex].flags != 0) {
                Field_DebugWriteU8(fileCtx, 1);
            } else {
                Field_DebugWriteU8(fileCtx, 0);
            }
            Field_DebugWriteU16(fileCtx, g_FieldLayer4Tiles[frameIndex].dstX);
            Field_DebugWriteU16(fileCtx, g_FieldLayer4Tiles[frameIndex].textureId);
            Field_DebugWriteU16(fileCtx, g_FieldLayer4Tiles[frameIndex].texSlot);
            Field_DebugWriteU16(fileCtx, g_FieldLayer4Tiles[frameIndex].dstY);
            val = (int)(g_FieldLayer4Tiles[frameIndex].scale * 10000000.0f);
            Field_DebugWriteU32(fileCtx, val);
            val = (int)(g_FieldLayer4Tiles[frameIndex].texUNorm * 10000000.0f);
            Field_DebugWriteU32(fileCtx, val);
            val = (int)(g_FieldLayer4Tiles[frameIndex].texVNorm * 10000000.0f);
            Field_DebugWriteU32(fileCtx, val);
            Field_DebugWriteU16(fileCtx, idx);
            Field_DebugWriteU16(fileCtx, idx);
        }
    }
    Field_FileWrite(fileCtx,
                         // STRING: FF7 0x00906590
                         "TEXTURE", 7);
    for (frameIndex = 0; frameIndex < 42; frameIndex++) {
        Field_DebugWriteU16(fileCtx, g_FieldLayers[frameIndex]->enabled16);
        if (g_FieldLayers[frameIndex]->enabled != 0) {
            Field_DebugWriteU16(fileCtx, g_FieldLayers[frameIndex]->hasDoubleSizeTiles);
            Field_DebugWriteU16(fileCtx, g_FieldLayers[frameIndex]->bytesPerPixel);
            if (g_FieldLayers[frameIndex]->bytesPerPixel == 1) {
                Field_FileWrite(fileCtx, g_FieldLayers[frameIndex]->pixels, 0x10000);
            } else {
                Field_FileWrite(fileCtx, g_FieldLayers[frameIndex]->pixels, 0x20000);
            }
        }
    }
    Field_FileWrite(fileCtx,
                         // STRING: FF7 0x00906598
                         "END", 3);
    Field_FileClose(fileCtx);
    return 1;
}

/*
 * Background reader: reads a 16-bit value from the stream cursor into *out; always succeeds.
 */
// FUNCTION: FF7 0x0062b5e1
int __cdecl Field_BgReadU16(int file, unsigned short *out)
{
    Field_BgStreamReadU16(out);
    return 1;
}

/*
 * Reads a 16-bit value at the background stream cursor and advances it two bytes.
 */
// FUNCTION: FF7 0x0062b5f7
void __cdecl Field_BgStreamReadU16(unsigned short *out)
{
    *out = *(unsigned short *)g_pFieldBgCursor;
    g_pFieldBgCursor = g_pFieldBgCursor + 2;
}

/*
 * Background reader: reads a byte from the stream cursor into *out; always succeeds.
 */
// FUNCTION: FF7 0x0062b618
int __cdecl Field_BgReadU8(int file, unsigned char *out)
{
    Field_BgStreamReadU8(out);
    return 1;
}

/*
 * Reads a byte at the background stream cursor and advances it.
 */
// FUNCTION: FF7 0x0062b62e
void __cdecl Field_BgStreamReadU8(unsigned char *out)
{
    *out = *(unsigned char *)g_pFieldBgCursor;
    g_pFieldBgCursor = g_pFieldBgCursor + 1;
}

/*
 * Background reader: reads a 32-bit value from the stream cursor into *out; always succeeds.
 */
// FUNCTION: FF7 0x0062b64d
int __cdecl Field_BgReadU32(int file, unsigned int *out)
{
    Field_BgStreamReadU32(out);
    return 1;
}

/*
 * Reads a 32-bit value at the background stream cursor and advances it four bytes.
 */
// FUNCTION: FF7 0x0062b663
void __cdecl Field_BgStreamReadU32(unsigned int *out)
{
    *out = *(unsigned int *)g_pFieldBgCursor;
    g_pFieldBgCursor = g_pFieldBgCursor + 4;
}

/*
 * Background reader: copies `size` bytes from the stream cursor into dst; always succeeds.
 */
// FUNCTION: FF7 0x0062b682
int __cdecl Field_BgReadBytes(int file, void *dst, int size)
{
    Field_BgStreamReadBytes(dst, size);
    return 1;
}

/* Copies `size` bytes from the background stream to `dst` and advances the cursor past them. */
// FUNCTION: FF7 0x0062b69c
void __cdecl Field_BgStreamReadBytes(void *dst, int size)
{
    memcpy(dst, g_pFieldBgCursor, size);
    g_pFieldBgCursor = g_pFieldBgCursor + size;
}

/*
 * Skips `count` bytes of the background stream; `file` is unused.
 */
// FUNCTION: FF7 0x0062b6ce
void __cdecl Field_BgSkipBytes(int file, int count)
{
    Field_BgStreamSkipBytes(count);
}

/*
 * Advances the background stream cursor by `count` bytes (the name says dwords; the arithmetic is
 * bytes).
 */
// FUNCTION: FF7 0x0062b6df
void __cdecl Field_BgStreamSkipBytes(int count)
{
    g_pFieldBgCursor = g_pFieldBgCursor + count;
}

/*
 * Reads the background stream: fileCtx is intentionally uninitialized because the
 * matched BgRead wrappers ignore it and use g_pFieldBgCursor. Keep the unsigned
 * 32-bit value conversions: VC5 generates a separate 64-bit temporary for each.
 * Reverse declarations within bucket zero retain the original 0x13c named-local area.
 */
// FUNCTION: FF7 0x0062b6f1
int __cdecl Field_ParseBackgroundSection(void)
{
    char fileName[128];
    unsigned int val;
    int flags;
    int frameIndex;
    int fileCtx;
    char leafName[32];
    int rem;
    unsigned short idx;
    unsigned short wide;
    char scratch[128];

    idx = 0;
    if (Field_GetMapNameById(g_FieldCurrentMapId, leafName) == 0) {
        return 0;
    }
    g_pFieldBgCursor = (unsigned char *)Field_GetSectionPtr(8);
    if (g_pFieldBgCursor == 0) {
        return 0;
    }
    g_FieldTileStateDc4 = 1;
    Field_BgReadU16(fileCtx, &wide);
    Field_BgReadU16(fileCtx, &g_FieldTileStateDc0);
    Field_BgReadU8(fileCtx, (unsigned char *)&g_FieldCameraScrollE74);
    Field_BgReadBytes(fileCtx, scratch, 7);
    for (frameIndex = 0; frameIndex < 20; frameIndex++) {
        Field_BgReadU8(fileCtx, (unsigned char *)&g_FieldPaletteFlags + frameIndex);
    }
    Field_BgReadU16(fileCtx, &idx);
    Field_BgReadU16(fileCtx, &idx);
    Field_BgReadBytes(fileCtx, scratch, 4);
    if (g_FieldCameraScrollE74 != 0) {
        Field_BgReadU16(fileCtx, &g_FieldLayer1StateE58.spanWidth);
        Field_BgReadU16(fileCtx, &g_FieldLayer1StateE58.spanHeight);
        Field_BgReadU16(fileCtx, &g_FieldLayer1TilesNum);
        Field_BgReadU16(fileCtx, (unsigned short *)&g_FieldCameraScrollE70);
        Field_BgReadU16(fileCtx, &idx);
        Field_BgReadU16(fileCtx, &idx);
        // STRING: FF7 0x0090659c
        g_FieldLayer1Tiles = (struct FieldLayer1Tile *)GameHeapAllocRaw(g_FieldLayer1TilesNum * 0x105c, "C:\\FF7\\field\\src\\ad_data.cpp", 3271);
        // STRING: FF7 0x009065bc
        g_pFieldLayer1DrawOrder = (struct FieldLayerDrawOrder *)GameHeapAllocRaw(g_FieldLayer1TilesNum * 4, "C:\\FF7\\field\\src\\ad_data.cpp", 3273);
        for (frameIndex = 0; frameIndex < g_FieldLayer1TilesNum; frameIndex++) {
            Field_BgReadU16(fileCtx, (unsigned short *)&g_FieldLayer1Tiles[frameIndex].posX);
            Field_BgReadU16(fileCtx, (unsigned short *)&g_FieldLayer1Tiles[frameIndex].depth);
            if (g_FieldTileMinZ > g_FieldLayer1Tiles[frameIndex].depth) {
                g_FieldTileMinZ = g_FieldLayer1Tiles[frameIndex].depth;
            }
            Field_BgReadU16(fileCtx, &g_FieldLayer1Tiles[frameIndex].srcX);
            Field_BgReadU16(fileCtx, &g_FieldLayer1Tiles[frameIndex].srcY);
            Field_BgReadU16(fileCtx, &g_FieldLayer1Tiles[frameIndex].texX);
            Field_BgReadU16(fileCtx, &g_FieldLayer1Tiles[frameIndex].texY);
            Field_BgReadU16(fileCtx, &g_FieldLayer1Tiles[frameIndex].altTexX);
            Field_BgReadU16(fileCtx, &g_FieldLayer1Tiles[frameIndex].altTexY);
            Field_BgReadU16(fileCtx, &g_FieldLayer1Tiles[frameIndex].width);
            Field_BgReadU16(fileCtx, &g_FieldLayer1Tiles[frameIndex].height);
            Field_BgReadU16(fileCtx, &g_FieldLayer1Tiles[frameIndex].paletteRow);
            Field_BgReadU16(fileCtx, &g_FieldLayer1Tiles[frameIndex].sortKey);
            Field_BgReadU8(fileCtx, &g_FieldLayer1Tiles[frameIndex].zBufferValue);
            Field_BgReadU8(fileCtx, &g_FieldLayer1Tiles[frameIndex].visibilityMask);
            Field_BgReadU8(fileCtx, (unsigned char *)&idx);
            if (idx == 1) {
                g_FieldLayer1Tiles[frameIndex].active = 1;
            } else {
                g_FieldLayer1Tiles[frameIndex].active = 0;
            }
            Field_BgReadU8(fileCtx, (unsigned char *)&g_FieldLayer1Tiles[frameIndex].isSet);
            Field_BgReadU16(fileCtx, &g_FieldLayer1Tiles[frameIndex].transparencyMode);
            Field_BgReadU16(fileCtx, &g_FieldLayer1Tiles[frameIndex].mapId);
            Field_BgReadU16(fileCtx, &g_FieldLayer1Tiles[frameIndex].texPage);
            Field_BgReadU16(fileCtx, &g_FieldLayer1Tiles[frameIndex].pixelMode);
            Field_BgReadU32(fileCtx, &val);
            g_FieldLayer1Tiles[frameIndex].scaleFactor = val / 10000000.0f;
            Field_BgReadU32(fileCtx, &val);
            g_FieldLayer1Tiles[frameIndex].texU = val / 10000000.0f;
            Field_BgReadU32(fileCtx, &val);
            g_FieldLayer1Tiles[frameIndex].texV = val / 10000000.0f;
            Field_BgReadU16(fileCtx, &idx);
            Field_BgReadU16(fileCtx, &idx);
        }
        Field_BuildTileDrawOrder(g_pFieldLayer1DrawOrder, g_FieldLayer1TilesNum, g_FieldLayer1Tiles, g_FieldPaletteGridHeight);
    }
    Field_BgReadU8(fileCtx, (unsigned char *)&g_FieldLayer2ParamE04);
    if (g_FieldLayer2ParamE04 != 0) {
        Field_BgReadU16(fileCtx, &g_FieldLayer2StateDf8.width);
        Field_BgReadU16(fileCtx, &g_FieldLayer2StateDf8.height);
        Field_BgReadU16(fileCtx, &g_FieldLayer2TilesNum);
        Field_BgReadU16(fileCtx, &g_FieldLayer2StateE08.firstIndex);
        Field_BgReadU16(fileCtx, &g_FieldLayer2StateE08.nextIndex);
        Field_BgReadU16(fileCtx, (unsigned short *)&g_FieldLayer2StateE0c);
        Field_BgReadU16(fileCtx, &g_FieldLayer2StateE0c.sectionWord);
        Field_BgReadU16(fileCtx, &g_FieldLayer2StateE10.firstIndex);
        Field_BgReadU16(fileCtx, &g_FieldLayer2StateE10.nextIndex);
        Field_BgReadU16(fileCtx, &g_FieldLayer2StateE14.firstIndex);
        Field_BgReadU16(fileCtx, &g_FieldLayer2StateE14.nextIndex);
        Field_BgReadU16(fileCtx, &idx);
        Field_BgReadU16(fileCtx, &idx);
        // STRING: FF7 0x009065dc
        g_FieldLayer2Tiles = (struct FieldLayer2Tile *)GameHeapAllocRaw(g_FieldLayer2TilesNum * 0x105c, "C:\\FF7\\field\\src\\ad_data.cpp", 3346);
        // STRING: FF7 0x009065fc
        g_pFieldLayer2DrawOrder = (struct FieldLayerDrawOrder *)GameHeapAllocRaw(g_FieldLayer2TilesNum * 4, "C:\\FF7\\field\\src\\ad_data.cpp", 3348);
        for (frameIndex = 0; frameIndex < g_FieldLayer2TilesNum; frameIndex++) {
            Field_BgReadU16(fileCtx, (unsigned short *)&g_FieldLayer2Tiles[frameIndex].tileX);
            Field_BgReadU16(fileCtx, (unsigned short *)&g_FieldLayer2Tiles[frameIndex].spanY);
            Field_BgReadU16(fileCtx, &g_FieldLayer2Tiles[frameIndex].srcX);
            Field_BgReadU16(fileCtx, &g_FieldLayer2Tiles[frameIndex].srcY);
            Field_BgReadU16(fileCtx, &g_FieldLayer2Tiles[frameIndex].texX);
            Field_BgReadU16(fileCtx, &g_FieldLayer2Tiles[frameIndex].texY);
            Field_BgReadU16(fileCtx, &g_FieldLayer2Tiles[frameIndex].altTexX);
            Field_BgReadU16(fileCtx, &g_FieldLayer2Tiles[frameIndex].altTexY);
            Field_BgReadU16(fileCtx, &g_FieldLayer2Tiles[frameIndex].width);
            Field_BgReadU16(fileCtx, &g_FieldLayer2Tiles[frameIndex].height);
            Field_BgReadU16(fileCtx, &g_FieldLayer2Tiles[frameIndex].paletteRow);
            Field_BgReadU16(fileCtx, &g_FieldLayer2Tiles[frameIndex].sortKey);
            Field_BgReadU8(fileCtx, &g_FieldLayer2Tiles[frameIndex].zBufferValue);
            Field_BgReadU8(fileCtx, &g_FieldLayer2Tiles[frameIndex].visibilityMask);
            Field_BgReadU8(fileCtx, (unsigned char *)&idx);
            if (idx == 1) {
                g_FieldLayer2Tiles[frameIndex].inUse = 1;
            } else {
                g_FieldLayer2Tiles[frameIndex].inUse = 0;
            }
            Field_BgReadU8(fileCtx, (unsigned char *)&g_FieldLayer2Tiles[frameIndex].usesSecondTim);
            Field_BgReadU16(fileCtx, &g_FieldLayer2Tiles[frameIndex].transparencyMode);
            Field_BgReadU16(fileCtx, &g_FieldLayer2Tiles[frameIndex].textureId);
            Field_BgReadU16(fileCtx, &g_FieldLayer2Tiles[frameIndex].texPage);
            Field_BgReadU16(fileCtx, &g_FieldLayer2Tiles[frameIndex].colorMode);
            Field_BgReadU32(fileCtx, &val);
            g_FieldLayer2Tiles[frameIndex].depthZ = val / 10000000.0f;
            Field_BgReadU32(fileCtx, &val);
            g_FieldLayer2Tiles[frameIndex].texU = val / 10000000.0f;
            Field_BgReadU32(fileCtx, &val);
            g_FieldLayer2Tiles[frameIndex].texV = val / 10000000.0f;
            Field_BgReadU16(fileCtx, &idx);
            Field_BgReadU16(fileCtx, &idx);
            if (g_FieldCurrentMapId == 0x43 && g_FieldLayer2Tiles[frameIndex].sortKey == 0xfb) {
                g_FieldLayer2Tiles[frameIndex].depthZ = 0.944f;
            }
        }
        Field_BuildTileDrawOrder(g_pFieldLayer2DrawOrder, g_FieldLayer2TilesNum, g_FieldLayer2Tiles, g_FieldPaletteGridHeight);
    }
    if (g_FieldCurrentMapId == 0xcc) {
        for (frameIndex = 0; frameIndex < 9; frameIndex++) {
            g_FieldLayer2Tiles[g_FieldDepthOverrideTiles_0xcc[frameIndex]].depthZ = 0.99529f;
        }
    }
    if (g_FieldCurrentMapId == 0x75) {
        g_FieldLayer2Tiles[78].depthZ = 0.97f;
    }
    Field_BgReadU8(fileCtx, (unsigned char *)&g_FieldDoDrawLayer3);
    if (g_FieldDoDrawLayer3 != 0) {
        Field_BgReadU16(fileCtx, &g_FieldLayer3StateE24.width);
        Field_BgReadU16(fileCtx, &g_FieldLayer3StateE24.height);
        Field_BgReadU16(fileCtx, &g_FieldLayer3TilesNum);
        Field_BgReadU16(fileCtx, &g_FieldLayer3StateE30.firstIndex);
        Field_BgReadU16(fileCtx, &g_FieldLayer3StateE30.nextIndex);
        Field_BgReadU16(fileCtx, &g_FieldLayer3StateE34.firstIndex);
        Field_BgReadU16(fileCtx, &g_FieldLayer3StateE34.nextIndex);
        Field_BgReadU16(fileCtx, &g_FieldLayer3StateE38);
        Field_BgReadU16(fileCtx, &idx);
        Field_BgReadU16(fileCtx, &idx);
        // STRING: FF7 0x0090661c
        g_FieldLayer3Tiles = (struct FieldBgLayer3Tile *)GameHeapAllocRaw(g_FieldLayer3TilesNum * 0x105c, "C:\\FF7\\field\\src\\ad_data.cpp", 3434);
        // STRING: FF7 0x0090663c
        g_pFieldLayer3DrawOrder = (struct FieldLayerDrawOrder *)GameHeapAllocRaw(g_FieldLayer3TilesNum * 4, "C:\\FF7\\field\\src\\ad_data.cpp", 3436);
        for (frameIndex = 0; frameIndex < g_FieldLayer3TilesNum; frameIndex++) {
            g_FieldLayer3Tiles[frameIndex].mapId = 0;
            Field_BgReadU16(fileCtx, (unsigned short *)&g_FieldLayer3Tiles[frameIndex].xPos);
            Field_BgReadU16(fileCtx, (unsigned short *)&g_FieldLayer3Tiles[frameIndex].tileY);
            Field_BgReadU16(fileCtx, &g_FieldLayer3Tiles[frameIndex].srcX);
            Field_BgReadU16(fileCtx, &g_FieldLayer3Tiles[frameIndex].srcY);
            Field_BgReadU16(fileCtx, &g_FieldLayer3Tiles[frameIndex].texelU);
            Field_BgReadU16(fileCtx, &g_FieldLayer3Tiles[frameIndex].texelV);
            Field_BgReadU16(fileCtx, &g_FieldLayer3Tiles[frameIndex].tileX);
            Field_BgReadU16(fileCtx, &g_FieldLayer3Tiles[frameIndex].posY);
            Field_BgReadU16(fileCtx, &g_FieldLayer3Tiles[frameIndex].tileWidth);
            Field_BgReadU16(fileCtx, &g_FieldLayer3Tiles[frameIndex].tileHeight);
            Field_BgReadU16(fileCtx, &g_FieldLayer3Tiles[frameIndex].paletteRow);
            Field_BgReadU16(fileCtx, &g_FieldLayer3Tiles[frameIndex].fixedScale);
            Field_BgReadU8(fileCtx, &g_FieldLayer3Tiles[frameIndex].zDepth);
            Field_BgReadU8(fileCtx, &g_FieldLayer3Tiles[frameIndex].zFlags);
            Field_BgReadU8(fileCtx, (unsigned char *)&idx);
            if (idx == 1) {
                g_FieldLayer3Tiles[frameIndex].ownerTag = 1;
            } else {
                g_FieldLayer3Tiles[frameIndex].ownerTag = 0;
            }
            Field_BgReadU8(fileCtx, (unsigned char *)&g_FieldLayer3Tiles[frameIndex].flags);
            Field_BgReadU16(fileCtx, &g_FieldLayer3Tiles[frameIndex].dstX);
            Field_BgReadU16(fileCtx, &g_FieldLayer3Tiles[frameIndex].texId);
            Field_BgReadU16(fileCtx, &g_FieldLayer3Tiles[frameIndex].mapId);
            Field_BgReadU16(fileCtx, &g_FieldLayer3Tiles[frameIndex].dstY);
            Field_BgReadU32(fileCtx, &val);
            g_FieldLayer3Tiles[frameIndex].scale = val / 10000000.0f;
            Field_BgReadU32(fileCtx, &val);
            g_FieldLayer3Tiles[frameIndex].texU = val / 10000000.0f;
            Field_BgReadU32(fileCtx, &val);
            g_FieldLayer3Tiles[frameIndex].texV = val / 10000000.0f;
            Field_BgReadU16(fileCtx, &idx);
            Field_BgReadU16(fileCtx, &idx);
        }
        Field_BuildTileDrawOrder(g_pFieldLayer3DrawOrder, g_FieldLayer3TilesNum, g_FieldLayer3Tiles, g_FieldPaletteGridHeight);
    }
    Field_BgReadU8(fileCtx, (unsigned char *)&g_FieldDoDrawLayer4);
    if (g_FieldDoDrawLayer4 != 0) {
        Field_BgReadU16(fileCtx, &g_FieldLayer4StateE8c.width);
        Field_BgReadU16(fileCtx, &g_FieldLayer4StateE8c.height);
        Field_BgReadU16(fileCtx, &g_FieldLayer4TilesNum);
        Field_BgReadU16(fileCtx, &g_FieldLayer4StateE98.headerWord);
        Field_BgReadU16(fileCtx, (unsigned short *)&g_FieldLayer4StateE98.pageStart);
        Field_BgReadU16(fileCtx, &g_FieldLayer4StateE9c.pageEnd);
        Field_BgReadU16(fileCtx, &g_FieldLayer4StateE9c.directPageStart);
        Field_BgReadU16(fileCtx, &g_FieldLayer4StateEa0);
        Field_BgReadU16(fileCtx, &idx);
        Field_BgReadU16(fileCtx, &idx);
        // STRING: FF7 0x0090665c
        g_FieldLayer4Tiles = (struct FieldBgLayer4Tile *)GameHeapAllocRaw(g_FieldLayer4TilesNum * 0x105c, "C:\\FF7\\field\\src\\ad_data.cpp", 3506);
        // STRING: FF7 0x0090667c
        g_pFieldLayer4DrawOrder = (struct FieldLayerDrawOrder *)GameHeapAllocRaw(g_FieldLayer4TilesNum * 4, "C:\\FF7\\field\\src\\ad_data.cpp", 3508);
        for (frameIndex = 0; frameIndex < g_FieldLayer4TilesNum; frameIndex++) {
            Field_BgReadU16(fileCtx, (unsigned short *)&g_FieldLayer4Tiles[frameIndex].posX);
            Field_BgReadU16(fileCtx, (unsigned short *)&g_FieldLayer4Tiles[frameIndex].posY);
            Field_BgReadU16(fileCtx, &g_FieldLayer4Tiles[frameIndex].srcX);
            Field_BgReadU16(fileCtx, &g_FieldLayer4Tiles[frameIndex].srcY);
            Field_BgReadU16(fileCtx, &g_FieldLayer4Tiles[frameIndex].texU);
            Field_BgReadU16(fileCtx, &g_FieldLayer4Tiles[frameIndex].texV);
            Field_BgReadU16(fileCtx, &g_FieldLayer4Tiles[frameIndex].tileX);
            Field_BgReadU16(fileCtx, &g_FieldLayer4Tiles[frameIndex].tileY);
            Field_BgReadU16(fileCtx, &g_FieldLayer4Tiles[frameIndex].tileWidth);
            Field_BgReadU16(fileCtx, &g_FieldLayer4Tiles[frameIndex].tileHeight);
            Field_BgReadU16(fileCtx, &g_FieldLayer4Tiles[frameIndex].paletteRow);
            Field_BgReadU16(fileCtx, &g_FieldLayer4Tiles[frameIndex].fixedScale);
            Field_BgReadU8(fileCtx, &g_FieldLayer4Tiles[frameIndex].zDepth);
            Field_BgReadU8(fileCtx, &g_FieldLayer4Tiles[frameIndex].zFlags);
            Field_BgReadU8(fileCtx, (unsigned char *)&idx);
            if (idx == 1) {
                g_FieldLayer4Tiles[frameIndex].active = 1;
            } else {
                g_FieldLayer4Tiles[frameIndex].active = 0;
            }
            Field_BgReadU8(fileCtx, (unsigned char *)&g_FieldLayer4Tiles[frameIndex].flags);
            Field_BgReadU16(fileCtx, &g_FieldLayer4Tiles[frameIndex].dstX);
            Field_BgReadU16(fileCtx, &g_FieldLayer4Tiles[frameIndex].textureId);
            Field_BgReadU16(fileCtx, &g_FieldLayer4Tiles[frameIndex].texSlot);
            Field_BgReadU16(fileCtx, &g_FieldLayer4Tiles[frameIndex].dstY);
            Field_BgReadU32(fileCtx, &val);
            g_FieldLayer4Tiles[frameIndex].scale = val / 10000000.0f;
            Field_BgReadU32(fileCtx, &val);
            g_FieldLayer4Tiles[frameIndex].texUNorm = val / 10000000.0f;
            Field_BgReadU32(fileCtx, &val);
            g_FieldLayer4Tiles[frameIndex].texVNorm = val / 10000000.0f;
            Field_BgReadU16(fileCtx, &idx);
            Field_BgReadU16(fileCtx, &idx);
        }
        Field_BuildTileDrawOrder(g_pFieldLayer4DrawOrder, g_FieldLayer4TilesNum, g_FieldLayer4Tiles, g_FieldPaletteGridHeight);
    }
    Field_BgSkipBytes(fileCtx, 7);
    for (frameIndex = 0; frameIndex < 42; frameIndex++) {
        Field_BgReadU16(fileCtx, (unsigned short *)&g_FieldLayers[frameIndex]->enabled);
        if (g_FieldLayers[frameIndex]->enabled != 0) {
            Field_BgReadU16(fileCtx, &g_FieldLayers[frameIndex]->hasDoubleSizeTiles);
            Field_BgReadU16(fileCtx, &g_FieldLayers[frameIndex]->bytesPerPixel);
            if (g_FieldLayers[frameIndex]->bytesPerPixel == 1) {
                // STRING: FF7 0x0090669c
                g_FieldLayers[frameIndex]->pixels = (unsigned char *)GameHeapAlloc(0x10000, 1, "C:\\FF7\\field\\src\\ad_data.cpp", 0xdf2);
                Field_BgReadBytes(fileCtx, g_FieldLayers[frameIndex]->pixels, 0x10000);
            } else {
                // STRING: FF7 0x009066bc
                g_FieldLayers[frameIndex]->pixels = (unsigned char *)GameHeapAlloc(0x10000, 2, "C:\\FF7\\field\\src\\ad_data.cpp", 0xdff);
                Field_BgReadBytes(fileCtx, g_FieldLayers[frameIndex]->pixels, 0x20000);
            }
            g_FieldLayers[frameIndex]->built = 0;
            g_FieldLayers[frameIndex]->clearedState = 0;
        }
    }
    // An unused file-mode literal; the expression emits no code.
    // STRING: FF7 0x009066dc
    (void)"wb";
    return 1;
}
} /* extern "C" */
