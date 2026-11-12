// src/ff7/condor/cd_init.cpp
// Original: C:\FF7\condor\cd_init.cpp
// Contents: Initializes and loads the Fort Condor battlefield: resets the row and slot tables, sets
//   the asset directory, loads the camera, unit, ID, model and help texture files, creates render
//   objects, and releases them again.
/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/api/mem.h"
#include "ff7/types/condor.h"
#include "lib/types/graphics.h"
#include "ff7/api/condor.h"
#include "ff7/api/field.h"
#include "lib/api/graphics.h"
#include "lib/pfile.h"
#include "lib/codegen.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "ff7/enums/main.h"
#include "lib/api/file.h"
#include "lib/enums/file.h"
#include "lib/enums/graphics.h"

#pragma intrinsic(abs)
extern "C" {

// clang-format off
// GLOBAL: FF7 0x00901644
char * g_CondorDataDirPath = (char *)&g_CondorDefaultDataDirString;
// GLOBAL: FF7 0x00901648
char g_CondorDefaultDataDirString[6] = "data\\";
// g_CondorRenderObjectSlots, g_CondorRenderObjectSlots + 4 are indexed by the code as fields of
// records. The span runs to the next named definition of the TU; the TU's pieces inside
// 0xc60550..0xc60558 are aliases.
// GLOBAL: FF7 0x00c60550
unsigned char g_CondorRenderObjectSlots[8] = {0};

// GLOBAL: FF7 0x00c60558
unsigned char g_CondorRenderObjectSlotsTail[124] = {0};
// GLOBAL: FF7 0x00c605d4
int g_CondorTranslucentRectRenderObject = 0;
// GLOBAL: FF7 0x00c605d8
unsigned char g_CondorBuffer[8] = {0};
// GLOBAL: FF7 0x00c605e0
int g_CondorRedFadeLineRenderObject = 0;
// GLOBAL: FF7 0x00c605e4
int g_CondorRedFadeLineDrawn = 0;
// GLOBAL: FF7 0x00c605e8
unsigned char g_CondorRedFadeLineFlag[1] = {0};
unsigned char g_bssPad_00c605ec[4] = {0};
// GLOBAL: FF7 0x00c605f0
unsigned char g_CondorAssetDirBuffer[256] = {0};
// GLOBAL: FF7 0x00c606f0
int g_pCondorDataBin = 0;
unsigned char g_bssPad_00c606f4[4] = {0};
// GLOBAL: FF7 0x00c606f8
CondorCameraBin *g_pCondorCameraBin = 0;
// GLOBAL: FF7 0x00c606fc
int g_CondorAuxFileBuffer = 0;
// GLOBAL: FF7 0x00c60700
void *g_CondorHelpTimBuffer = 0;
// GLOBAL: FF7 0x00c60704
int g_pCondorIdBin = 0;
// GLOBAL: FF7 0x00c60708
struct CondorTextureRenderObjects g_CondorTextureRenderObjects = {0};
// GLOBAL: FF7 0x00c6073c
int g_CondorRenderObjects = 0;
// GLOBAL: FF7 0x00c60740
unsigned char g_CondorRenderObjectsTail[60] = {0};
// GLOBAL: FF7 0x00c6077c
CondorRegionRecord *g_CondorUnitTableBuffer = 0;
// GLOBAL: FF7 0x00c60780
CondorUnitModel g_CondorUnitModelTable[7] = {0};
unsigned char g_bssPad_00c60908[8] = {0};
// GLOBAL: FF7 0x00c60910
struct CondorRowFlags g_CondorFlags = {0};
// GLOBAL: FF7 0x00c60914
struct CondorCurrentValues g_CondorCurrentValues = {0};
// GLOBAL: FF7 0x00c60918
struct CondorDefaultValues g_CondorDefaultValues = {0};
unsigned char g_bssPad_00c6091c[4] = {0};
unsigned char g_bssPad_00c60920[16] = {0};
// GLOBAL: FF7 0x00c60930
int g_CondorUsesSoftwareDriver = 0;
// GLOBAL: FF7 0x00c60934
int g_CondorPresetMode = 0;
// GLOBAL: FF7 0x00c60938
int g_CondorViewportX = 0;
// GLOBAL: FF7 0x00c6093c
int g_CondorViewportY = 0;
// GLOBAL: FF7 0x00c60940
int g_CondorViewportWidth = 0;
// GLOBAL: FF7 0x00c60944
int g_CondorViewportHeight = 0;
// GLOBAL: FF7 0x00c60948
int g_CondorViewportScale = 0;
// GLOBAL: FF7 0x00c6094c
int g_CondorViewportOffsetX = 0;
// GLOBAL: FF7 0x00c60950
int g_CondorViewportOffsetY = 0;
// GLOBAL: FF7 0x00c60954
int g_CondorViewportInset = 0;
// GLOBAL: FF7 0x00c60958
int g_CondorScreenReferenceScale = 0;
unsigned char g_bssPad_00c6095c[4] = {0};
// clang-format on
#pragma intrinsic(memset)




// Original contiguous native group 5F3160..5F434B, ending in LINK alignment.
// Prototypes of callees the C sources used implicitly (definitions elsewhere), required in C++.
/*
 * Resets table entries and row state structures for Fort Condor minigame.
 */
// FUNCTION: FF7 0x005f2bf0
void __cdecl Mini_CondorResetRowTables(void)
{
    CondorRecord *unit;
    CondorEffectSequence *seq;
    int slotIdx, recIdx;
    g_CondorCameraX = 96;
    g_CondorCameraY = 0;
    g_CondorInitResetShort = 0;
    unit = g_CondorRecordTable.records;
    for (recIdx = 0; recIdx < 70; recIdx++) {
        unit->status = 0;
        unit++;
    }
    seq = g_CondorEffectSlots;
    for (slotIdx = 0; slotIdx < 8; slotIdx++) {
        seq->tableIndex = 0xFF;
        seq->timer = 0;
        seq++;
    }
    for (slotIdx = 0; slotIdx < 16; slotIdx++)
        g_CondorCounterEffects[slotIdx].counter = 0;
    g_CondorFlags.red = 0;
    (g_CondorFlags.green) = 0;
    (g_CondorFlags.blue) = 0;
    (g_CondorCurrentValues.lineStartX) = 0;
    (g_CondorDefaultValues.lineEndX) = 512;
    (g_CondorDefaultValues.defaultValue) = 512;
    (g_CondorCurrentValues.currentValue) = (g_CondorDefaultValues.defaultValue);
}
/*
 * Binds texture render object handles and flags to corresponding slot records.
 */
// FUNCTION: FF7 0x005f2ce4
void __cdecl Mini_CondorBindRenderObjectSlots(void)
{
    int i;
    ((CondorRenderObjectSlotEntry *)(g_CondorRenderObjectSlots))[0].renderObject = ((int *)&g_CondorTextureRenderObjects)[2];
    ((CondorRenderObjectSlotEntry *)(g_CondorRenderObjectSlots))[0].flag = 0;
    ((CondorRenderObjectSlotEntry *)(g_CondorRenderObjectSlots))[1].renderObject = ((int *)&g_CondorTextureRenderObjects)[3];
    ((CondorRenderObjectSlotEntry *)(g_CondorRenderObjectSlots))[1].flag = 0;
    ((CondorRenderObjectSlotEntry *)(g_CondorRenderObjectSlots))[2].renderObject = ((int *)&g_CondorTextureRenderObjects)[4];
    ((CondorRenderObjectSlotEntry *)(g_CondorRenderObjectSlots))[2].flag = 0;
    ((CondorRenderObjectSlotEntry *)(g_CondorRenderObjectSlots))[3].renderObject = ((int *)&g_CondorTextureRenderObjects)[5];
    ((CondorRenderObjectSlotEntry *)(g_CondorRenderObjectSlots))[4].renderObject = ((int *)&g_CondorTextureRenderObjects)[6];
    ((CondorRenderObjectSlotEntry *)(g_CondorRenderObjectSlots))[3].flag = 1;
    ((CondorRenderObjectSlotEntry *)(g_CondorRenderObjectSlots))[4].flag = 1;
    ((CondorRenderObjectSlotEntry *)(g_CondorRenderObjectSlots))[5].renderObject = ((int *)&g_CondorTextureRenderObjects)[7];
    ((CondorRenderObjectSlotEntry *)(g_CondorRenderObjectSlots))[6].renderObject = ((int *)&g_CondorTextureRenderObjects)[8];
    ((CondorRenderObjectSlotEntry *)(g_CondorRenderObjectSlots))[7].renderObject = ((int *)&g_CondorTextureRenderObjects)[9];
    ((CondorRenderObjectSlotEntry *)(g_CondorRenderObjectSlots))[8].renderObject = ((int *)&g_CondorTextureRenderObjects)[10];
    ((CondorRenderObjectSlotEntry *)(g_CondorRenderObjectSlots))[5].flag = 0;
    ((CondorRenderObjectSlotEntry *)(g_CondorRenderObjectSlots))[6].flag = 0;
    ((CondorRenderObjectSlotEntry *)(g_CondorRenderObjectSlots))[7].flag = 0;
    ((CondorRenderObjectSlotEntry *)(g_CondorRenderObjectSlots))[8].flag = 0;
    ((CondorRenderObjectSlotEntry *)(g_CondorRenderObjectSlots))[9].renderObject = ((int *)&g_CondorTextureRenderObjects)[11];
    ((CondorRenderObjectSlotEntry *)(g_CondorRenderObjectSlots))[9].flag = 1;
    ((CondorRenderObjectSlotEntry *)(g_CondorRenderObjectSlots))[10].renderObject = ((int *)&g_CondorTextureRenderObjects)[12];
    ((CondorRenderObjectSlotEntry *)(g_CondorRenderObjectSlots))[11].renderObject = ((int *)&g_CondorTextureRenderObjects)[0];
    ((CondorRenderObjectSlotEntry *)(g_CondorRenderObjectSlots))[12].renderObject = ((int *)&g_CondorTextureRenderObjects)[1];
    ((CondorRenderObjectSlotEntry *)(g_CondorRenderObjectSlots))[10].flag = 1;
    ((CondorRenderObjectSlotEntry *)(g_CondorRenderObjectSlots))[11].flag = 1;
    ((CondorRenderObjectSlotEntry *)(g_CondorRenderObjectSlots))[12].flag = 1;
    for (i = 6; i < 7; i++)
        g_CondorUnitModelTable[i].ready = 0;
    Mini_CondorResetSlotCountsAndFlags();
}
/*
 * Zeroes out allocation counts and resets slot attributes across all slot tables.
 */
// FUNCTION: FF7 0x005f2e08
void __cdecl Mini_CondorResetSlotCountsAndFlags(void)
{
    unsigned short unitIndex, partIndex;
    for (unitIndex = 0; unitIndex < 13; unitIndex++)
        ((CondorRenderObjectSlotEntry *)(g_CondorRenderObjectSlots))[unitIndex].drawn = 0;
    for (unitIndex = 0; unitIndex < 7; unitIndex++) {
        if (g_CondorUnitModelTable[unitIndex].ready != 0) {
            if (unitIndex != 2 && unitIndex != 3)
                g_CondorUnitModelTable[unitIndex].drawPass = 0;
            else
                g_CondorUnitModelTable[unitIndex].drawPass = 1;
            for (partIndex = 0; partIndex < 4; partIndex++)
                g_CondorUnitModelTable[unitIndex].parts[partIndex].batched = 0;
        }
    }
}

// The existing Condor path owner is 256 bytes. The original caller supplies
// GetMinigameDirForCondor's unchecked, NUL-terminated in-allocation string.
#pragma intrinsic(strcpy)
/*
 * Sets the directory Fort Condor loads its loose files from. `App_InitSubsystems` passes
 * `GetMinigameDirForCondor()` at start-up; the string is copied with strcpy into the 256-byte
 * g_CondorAssetDirBuffer. `Mini_CondorLoadFile` prefixes it to the file name when it reads from
 * disk instead of condor.lgp, and the texture loader starts its TIM paths with it. The copy has
 * no length check: a source longer than 255 characters overruns the buffer.
 */
// FUNCTION: FF7 0x005f2f18
void __cdecl Mini_CondorSetAssetDirPath(const char *path)
{
    strcpy((char *)g_CondorAssetDirBuffer, path);
}
#pragma function(strcpy)
 // src/lib/src/mem/mem.cpp
/*
 * Searches for and loads a file buffer either from disk path or minigame archive.
 */
// FUNCTION: FF7 0x005f2f46
#pragma intrinsic(strcpy, strcat)

int __cdecl Mini_CondorLoadFile(void *fileName)
{
    int size;
    char pathBuf[256];
    char baseName[256];
    char suffix[256];
    FILE *fp;
    void *pHandle;
    char *dataPtr;
    char drive[4];
    char directory[256];
    dataPtr = 0;
    if (g_CondorUsesLgp == 0) {
        strcpy(pathBuf, (char *)g_CondorAssetDirBuffer);
        strcat(pathBuf, (char *)fileName);
        // STRING: FF7 0x00901650
        fp = Field_FileOpen(pathBuf, "rb");
        if (fp == 0)
            return 0;
        size = Field_FileSize(fp);
        // STRING: FF7 0x00901654
        dataPtr = (char *)(GameHeapAllocRaw(size + 2, "C:\\FF7\\condor\\cd_init.cpp", 204));
        if (dataPtr == 0)
            return 0;
        Field_FileRead(fp, dataPtr, size);
        dataPtr[size] = 0;
        Field_FileClose(fp);
    } else {
        _splitpath((char *)fileName, drive, directory, baseName, suffix);
        strcat(baseName, suffix);
        pHandle = FindLgpFile(baseName, LGP_ARCHIVE_CONDOR);
        if (pHandle) {
            size = GetLgpEntrySize((int)pHandle, LGP_ARCHIVE_CONDOR);
            // STRING: FF7 0x00901670
            dataPtr = (char *)(GameHeapAllocRaw(size + 2, "C:\\FF7\\condor\\cd_init.cpp", 221));
            if (dataPtr == 0)
                return 0;
            ReadLgpEntry(pHandle, LGP_ARCHIVE_CONDOR, dataPtr, size);
            dataPtr[size] = 0;
        } else {
            return 0;
        }
    }
    return (int)dataPtr;
}

#pragma function(strcpy, strcat)

/*
 * Parses vertex table header and loads 3D model vertex records.
 */
// FUNCTION: FF7 0x005f3160
void __cdecl Mini_CondorReadIdHeaderAndLoadVertTable(int header, int base)
{
    int bufA;
    int unusedSpace[4]; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int anchor;
    CondorVertexEntry *vertexTable;
    int unusedField; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int cursor;
    int auxBuf;
    int padField; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */

    g_CondorCameraX = 96;
    g_CondorCameraY = 408;
    anchor = base;
    bufA = base + 0x9800;
    auxBuf = base + 0xD000;
    cursor = header;
    g_CondorMatchListState.regionCount = ((CondorIdHeader *)cursor)->lowByte + (((CondorIdHeader *)cursor)->highByte << 8);
    cursor += 0x10;
    vertexTable = (CondorVertexEntry *)cursor;
    // STRING: FF7 0x0090168c
    Mini_CondorLoadUnitTable("data\\vert.bin", (struct CondorRegionRecord *)base);
}
/*
 * Loads the Fort Condor camera file (data\camera.bin: rotation, translation and screen distance)
 * into g_pCondorCameraBin and sets up the camera matrix from it.
 */
// FUNCTION: FF7 0x005f31da
int __cdecl Mini_CondorLoadCameraFile(void *fileName)
{
    CondorCameraBin *buf;
    unsigned short i;
    g_pCondorCameraBin = (struct CondorCameraBin *)(Mini_CondorLoadFile(fileName));
    if (g_pCondorCameraBin == 0) {
        return 0;
    } else {
        buf = g_pCondorCameraBin;
        for (i = 0; i < 9; i++) {
            (*(short(*)[3][3]) & g_CondorCameraMatrix)[i / 3][i % 3] = ((short *)buf)[i];
        }
        for (i = 0; i < 3; i++) {
            ((int *)((char *)&g_CondorCameraMatrix + 0x12))[i] = *(int *)((int)buf + i * 4 + 20);
        }
        (*(int *)&g_CondorScreenDistanceBuf) = buf->screenDistance;
        SetGeomScreen((*(int *)&g_CondorScreenDistanceBuf));
        SetRotMatrix(*(short(*)[3][3]) & g_CondorCameraMatrix);
        SetTransMatrix((void *)(*(short(*)[3][3]) & g_CondorCameraMatrix));
        g_CondorRotationAngles.rotX = 70;
        g_CondorRotationAngles.rotY = 0;
        g_CondorRotationAngles.rotZ = 0;
        g_CondorCameraTranslateX = 0;
        g_CondorCameraTranslateY = 0;
        g_CondorCameraTranslateZ = 0;
        RotMatrixXYZ(&g_CondorRotationAngles.rotX, g_CondorMatchListState.cameraWorkMatrix);
        SetMatrixTranslation(g_CondorMatchListState.cameraWorkMatrix, &g_CondorCameraTranslateX);
        ConcatMatrixInPlace(g_CondorMatchListState.cameraWorkMatrix, (void *)(*(short(*)[3][3]) & g_CondorCameraMatrix));
        SetRotMatrix(*(short(*)[3][3]) & g_CondorCameraMatrix);
        SetTransMatrix((void *)(*(short(*)[3][3]) & g_CondorCameraMatrix));
        return 1;
    }
}
/*
 * Loads unit ID mapping file and corresponding unit table entries.
 */
// FUNCTION: FF7 0x005f3378
int __cdecl Mini_CondorLoadIdFileAndUnitTable(void *fileName)
{
    g_pCondorIdBin = Mini_CondorLoadFile(fileName);
    if (g_pCondorIdBin == 0) {
        return 0;
    } else {
        Mini_CondorReadIdHeaderAndLoadVertTable(g_pCondorIdBin, (int)&g_CondorUnitTable);
        return 1;
    }
}
/*
 * Loads data\data.bin, the unit stats, action sequences and path tables the battle reads, into
 * g_pCondorDataBin.
 */
// FUNCTION: FF7 0x005f33b4
int __cdecl Mini_CondorLoadDataBinFile(void *fileName)
{
    g_pCondorDataBin = Mini_CondorLoadFile(fileName);
    if (g_pCondorDataBin != 0) {
        return 1;
    }
    return 0;
}
/*
 * Loads help screen TIM texture asset file.
 */
// FUNCTION: FF7 0x005f33dc
int __cdecl Mini_CondorLoadHelpTimFile(void *fileName)
{
    g_CondorHelpTimBuffer = (void *)(Mini_CondorLoadFile(fileName));
    if (g_CondorHelpTimBuffer != 0) {
        return 1;
    } else {
        return 0;
    }
}
/*
 * Loads a file into g_CondorAuxFileBuffer (no source calls this loader; data.bin goes to
 * g_pCondorDataBin).
 */
// FUNCTION: FF7 0x005f3404
int __cdecl Mini_CondorLoadAuxFile(void *fileName)
{
    g_CondorAuxFileBuffer = Mini_CondorLoadFile(fileName);
    if (g_CondorAuxFileBuffer != 0) {
        return 1;
    } else {
        return 0;
    }
}


// Mini_CondorCreateTextureRenderObjects -- 2696 bytes
#pragma intrinsic(strcpy, strcat)

/*
 * Creates graphic render objects and initializes texture batches for battlefield units.
 */
// FUNCTION: FF7 0x005f342c
int __cdecl Mini_CondorCreateTextureRenderObjects(GfxEngineContext *context, PfileLoadOptions *loadOptions)
{
    char fileName[64];
    char assetPath[128];
    int blendMode;
    int textureName;
    unsigned short slot;

    strcpy((char *)&assetPath, (char *)g_CondorAssetDirBuffer);
    strcat((char *)&assetPath, g_CondorDataDirPath);
    loadOptions->directory = (char *)(&assetPath);
    loadOptions->flags |= 0x10;
    if (g_CondorUsesLgp != 0) {
        loadOptions->file.fromArchive = 1;
        loadOptions->file.archiveId = 6;
        loadOptions->file.callback = 0;
    }
    loadOptions->renderFlags |= 0x400;
    if (g_CondorPresetMode == 2) {
        loadOptions->renderFlags |= 0x40;
    } else {
        loadOptions->renderFlags |= 0x20;
    }
    for (slot = 0; slot < 8; slot++) {
        // STRING: FF7 0x0090169c
        sprintf((char *)(&fileName), "map%d.tim", slot);
        ((CondorRenderObject *)&g_CondorRenderObjects)[slot].renderObject = (int)(Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, loadOptions, (char *)(&fileName), (void *)(context->renderObjectBank)));
        ((CondorRenderObject *)&g_CondorRenderObjectsTail)[slot].renderObject = 0;
    }
    if (g_CondorPresetMode == 2) {
        loadOptions->renderFlags &= 0xFFFFFFBF;
    } else {
        loadOptions->renderFlags &= 0xFFFFFFDF;
    }
    loadOptions->renderFlags &= 0xFFFFFBFF;
    // STRING: FF7 0x009016a8
    textureName = (int)("eft00.tim");
    (g_CondorTextureRenderObjects.renderObject08) = (int)(Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, loadOptions, (char *)(textureName), (void *)(context->renderObjectBank)));
    if ((g_CondorTextureRenderObjects.renderObject08) == 0) {
        return 0;
    } else if (g_CondorPresetMode != 2) {
        // STRING: FF7 0x009016b4
        textureName = (int)("emes00.tim");
        (g_CondorTextureRenderObjects.renderObject1C) = (int)(Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, loadOptions, (char *)(textureName), (void *)(context->renderObjectBank)));
        if ((g_CondorTextureRenderObjects.renderObject1C) == 0) {
            return 0;
        }
        // STRING: FF7 0x009016c0
        textureName = (int)("emes01.tim");
        (g_CondorTextureRenderObjects.renderObject20) = (int)(Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, loadOptions, (char *)(textureName), (void *)(context->renderObjectBank)));
        if ((g_CondorTextureRenderObjects.renderObject20) == 0) {
            return 0;
        }
        // STRING: FF7 0x009016cc
        textureName = (int)("eunit00.tim");
        (g_CondorTextureRenderObjects.renderObject0C) = (int)(Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, loadOptions, (char *)(textureName), (void *)(context->renderObjectBank)));
        if ((g_CondorTextureRenderObjects.renderObject0C) == 0) {
            return 0;
        }
        // STRING: FF7 0x009016d8
        textureName = (int)("eunit01.tim");
        (g_CondorTextureRenderObjects.renderObject10) = (int)(Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, loadOptions, (char *)(textureName), (void *)(context->renderObjectBank)));
        if ((g_CondorTextureRenderObjects.renderObject10) == 0) {
            return 0;
        }
    } else {
        // STRING: FF7 0x009016e4
        textureName = (int)("eunit00a.tim");
        (g_CondorUnitModelTable[0].parts[0].renderObject) = (int)(Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, loadOptions, (char *)(textureName), (void *)(context->renderObjectBank)));
        // STRING: FF7 0x009016f4
        textureName = (int)("eunit00b.tim");
        (g_CondorUnitModelTable[0].parts[1].renderObject) = (int)(Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, loadOptions, (char *)(textureName), (void *)(context->renderObjectBank)));
        // STRING: FF7 0x00901704
        textureName = (int)("eunit00c.tim");
        (g_CondorUnitModelTable[0].parts[2].renderObject) = (int)(Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, loadOptions, (char *)(textureName), (void *)(context->renderObjectBank)));
        // STRING: FF7 0x00901714
        textureName = (int)("eunit00d.tim");
        (g_CondorUnitModelTable[0].parts[3].renderObject) = (int)(Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, loadOptions, (char *)(textureName), (void *)(context->renderObjectBank)));
        // STRING: FF7 0x00901724
        textureName = (int)("eunit01a.tim");
        (g_CondorUnitModelTable[1].parts[0].renderObject) = (int)(Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, loadOptions, (char *)(textureName), (void *)(context->renderObjectBank)));
        // STRING: FF7 0x00901734
        textureName = (int)("eunit01b.tim");
        (g_CondorUnitModelTable[1].parts[1].renderObject) = (int)(Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, loadOptions, (char *)(textureName), (void *)(context->renderObjectBank)));
        // STRING: FF7 0x00901744
        textureName = (int)("eunit01c.tim");
        (g_CondorUnitModelTable[1].parts[2].renderObject) = (int)(Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, loadOptions, (char *)(textureName), (void *)(context->renderObjectBank)));
        // STRING: FF7 0x00901754
        textureName = (int)("eunit01d.tim");
        (g_CondorUnitModelTable[1].parts[3].renderObject) = (int)(Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, loadOptions, (char *)(textureName), (void *)(context->renderObjectBank)));
        // STRING: FF7 0x00901764
        textureName = (int)("emes00a.tim");
        (g_CondorUnitModelTable[4].parts[0].renderObject) = (int)(Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, loadOptions, (char *)(textureName), (void *)(context->renderObjectBank)));
        // STRING: FF7 0x00901770
        textureName = (int)("emes00b.tim");
        (g_CondorUnitModelTable[4].parts[1].renderObject) = (int)(Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, loadOptions, (char *)(textureName), (void *)(context->renderObjectBank)));
        // STRING: FF7 0x0090177c
        textureName = (int)("emes00c.tim");
        (g_CondorUnitModelTable[4].parts[2].renderObject) = (int)(Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, loadOptions, (char *)(textureName), (void *)(context->renderObjectBank)));
        // STRING: FF7 0x00901788
        textureName = (int)("emes00d.tim");
        (g_CondorUnitModelTable[4].parts[3].renderObject) = (int)(Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, loadOptions, (char *)(textureName), (void *)(context->renderObjectBank)));
        // STRING: FF7 0x00901794
        textureName = (int)("emes01a.tim");
        (g_CondorUnitModelTable[5].parts[0].renderObject) = (int)(Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, loadOptions, (char *)(textureName), (void *)(context->renderObjectBank)));
        // STRING: FF7 0x009017a0
        textureName = (int)("emes01b.tim");
        (g_CondorUnitModelTable[5].parts[1].renderObject) = (int)(Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, loadOptions, (char *)(textureName), (void *)(context->renderObjectBank)));
        (g_CondorUnitModelTable[5].parts[2].renderObject) = 0;
        (g_CondorUnitModelTable[5].parts[3].renderObject) = 0;
        (g_CondorUnitModelTable[0].ready) = 1;
        (g_CondorUnitModelTable[1].ready) = 1;
        (g_CondorUnitModelTable[4].ready) = 1;
        (g_CondorUnitModelTable[5].ready) = 1;
    }
    // STRING: FF7 0x009017ac
    textureName = (int)("mes02.tim");
    (g_CondorTextureRenderObjects.renderObject24) = (int)(Gfx_CreateRenderObject(0, GFX_DRAW_TEXTURED_QUAD_2D, loadOptions, (char *)(textureName), (void *)(context->renderObjectBank)));
    if ((g_CondorTextureRenderObjects.renderObject24) == 0) {
        return 0;
    } else {
        // STRING: FF7 0x009017b8
        textureName = (int)("emes08.tim");
        (g_CondorTextureRenderObjects.renderObject28) = (int)(Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, loadOptions, (char *)(textureName), (void *)(context->renderObjectBank)));
        if ((g_CondorTextureRenderObjects.renderObject28) == 0) {
            return 0;
        } else {
            blendMode = BLEND_AVERAGE;
            Pfile_InitLoadOptionsWithBlend(blendMode, loadOptions);
            loadOptions->directory = (char *)(&assetPath);
            loadOptions->flags |= 0x10;
            if (g_CondorPresetMode != 2) {
                // STRING: FF7 0x009017c4
                textureName = (int)("eunit00.tim");
                (g_CondorTextureRenderObjects.renderObject14) = (int)(Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, loadOptions, (char *)(textureName), (void *)(context->renderObjectBank)));
                if ((g_CondorTextureRenderObjects.renderObject14) == 0) {
                    return 0;
                }
            } else {
                // STRING: FF7 0x009017d0
                textureName = (int)("eunit00a.tim");
                (g_CondorUnitModelTable[2].parts[0].renderObject) = (int)(Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, loadOptions, (char *)(textureName), (void *)(context->renderObjectBank)));
                // STRING: FF7 0x009017e0
                textureName = (int)("eunit00b.tim");
                (g_CondorUnitModelTable[2].parts[1].renderObject) = (int)(Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, loadOptions, (char *)(textureName), (void *)(context->renderObjectBank)));
                // STRING: FF7 0x009017f0
                textureName = (int)("eunit00c.tim");
                (g_CondorUnitModelTable[2].parts[2].renderObject) = (int)(Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, loadOptions, (char *)(textureName), (void *)(context->renderObjectBank)));
                // STRING: FF7 0x00901800
                textureName = (int)("eunit00d.tim");
                (g_CondorUnitModelTable[2].parts[3].renderObject) = (int)(Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, loadOptions, (char *)(textureName), (void *)(context->renderObjectBank)));
                (g_CondorUnitModelTable[2].ready) = 1;
            }
            blendMode = BLEND_AVERAGE;
            Pfile_InitLoadOptionsWithBlend(blendMode, loadOptions);
            loadOptions->directory = (char *)(&assetPath);
            loadOptions->flags |= 0x10;
            if (g_CondorPresetMode != 2) {
                // STRING: FF7 0x00901810
                textureName = (int)("eunit01.tim");
                (g_CondorTextureRenderObjects.renderObject18) = (int)(Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, loadOptions, (char *)(textureName), (void *)(context->renderObjectBank)));
                if ((g_CondorTextureRenderObjects.renderObject18) == 0) {
                    return 0;
                }
            } else {
                // STRING: FF7 0x0090181c
                textureName = (int)("eunit01a.tim");
                (g_CondorUnitModelTable[3].parts[0].renderObject) = (int)(Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, loadOptions, (char *)(textureName), (void *)(context->renderObjectBank)));
                // STRING: FF7 0x0090182c
                textureName = (int)("eunit01b.tim");
                (g_CondorUnitModelTable[3].parts[1].renderObject) = (int)(Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, loadOptions, (char *)(textureName), (void *)(context->renderObjectBank)));
                // STRING: FF7 0x0090183c
                textureName = (int)("eunit01c.tim");
                (g_CondorUnitModelTable[3].parts[2].renderObject) = (int)(Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, loadOptions, (char *)(textureName), (void *)(context->renderObjectBank)));
                // STRING: FF7 0x0090184c
                textureName = (int)("eunit01d.tim");
                (g_CondorUnitModelTable[3].parts[3].renderObject) = (int)(Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, loadOptions, (char *)(textureName), (void *)(context->renderObjectBank)));
                (g_CondorUnitModelTable[3].ready) = 1;
            }
            blendMode = BLEND_AVERAGE;
            Pfile_InitLoadOptionsWithBlend(blendMode, loadOptions);
            loadOptions->directory = (char *)(&assetPath);
            loadOptions->flags |= 0x10;
            // STRING: FF7 0x0090185c
            textureName = (int)("eft00.tim");
            (g_CondorTextureRenderObjects.renderObject2C) = (int)(Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, loadOptions, (char *)(textureName), (void *)(context->renderObjectBank)));
            if ((g_CondorTextureRenderObjects.renderObject2C) == 0) {
                return 0;
            } else {
                blendMode = BLEND_QUARTER_ADDITIVE;
                Pfile_InitLoadOptionsWithBlend(blendMode, loadOptions);
                loadOptions->directory = (char *)(&assetPath);
                loadOptions->flags |= 0x10;
                // STRING: FF7 0x00901868
                textureName = (int)("eft00.tim");
                (g_CondorTextureRenderObjects.renderObject30) = (int)(Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, loadOptions, (char *)(textureName), (void *)(context->renderObjectBank)));
                if ((g_CondorTextureRenderObjects.renderObject30) == 0) {
                    return 0;
                } else {
                    blendMode = BLEND_AVERAGE;
                    Pfile_InitLoadOptionsWithBlend(blendMode, loadOptions);
                    ((int *)&g_CondorTextureRenderObjects)[0] = (int)(Gfx_CreateRenderObject(1, GFX_DRAW_FLAT_QUAD_2D, loadOptions, 0, (void *)(context->renderObjectBank)));
                    if (((int *)&g_CondorTextureRenderObjects)[0] == 0) {
                        return 0;
                    } else {
                        blendMode = BLEND_ADDITIVE;
                        Pfile_InitLoadOptionsWithBlend(blendMode, loadOptions);
                        (g_CondorTextureRenderObjects.renderObject04) = (int)(Gfx_CreateRenderObject(1, GFX_DRAW_FLAT_QUAD_2D, loadOptions, 0, (void *)(context->renderObjectBank)));
                        if ((g_CondorTextureRenderObjects.renderObject04) == 0) {
                            return 0;
                        } else {
                            Mini_CondorBindRenderObjectSlots();
                            Mini_CondorReleaseBatchesAndResetDrawPools(context);
                            return 1;
                        }
                    }
                }
            }
        }
    }
}

#pragma function(strcpy, strcat)

/*
 * Destroys battlefield render objects and frees loaded asset buffers.
 */
// FUNCTION: FF7 0x005f3eb4
void __cdecl Mini_CondorReleaseTexturesAndDataFiles(void)
{
    unsigned short objectIdx;
    unsigned short partIndex;

    for (objectIdx = 0; objectIdx < 8; objectIdx++) {
        DestroyRenderObject((void *)((CondorRenderObject *)&g_CondorRenderObjects)[objectIdx].renderObject);
        ((CondorRenderObject *)&g_CondorRenderObjects)[objectIdx].renderObject = 0;
    }
    if (g_pCondorDataBin != 0) {
        // STRING: FF7 0x00901874
        GameHeapFree((void *)(g_pCondorDataBin), "C:\\FF7\\condor\\cd_init.cpp", 1104);
        g_pCondorDataBin = 0;
    }
    if (g_CondorHelpTimBuffer != 0) {
        // STRING: FF7 0x00901890
        GameHeapFree(g_CondorHelpTimBuffer, "C:\\FF7\\condor\\cd_init.cpp", 1109);
        g_CondorHelpTimBuffer = 0;
    }
    if (g_CondorAuxFileBuffer != 0) {
        // STRING: FF7 0x009018ac
        GameHeapFree((void *)(g_CondorAuxFileBuffer), "C:\\FF7\\condor\\cd_init.cpp", 1114);
        g_CondorAuxFileBuffer = 0;
    }
    if (g_pCondorIdBin != 0) {
        // STRING: FF7 0x009018c8
        GameHeapFree((void *)(g_pCondorIdBin), "C:\\FF7\\condor\\cd_init.cpp", 1119);
        g_pCondorIdBin = 0;
    }
    if (g_pCondorCameraBin != 0) {
        // STRING: FF7 0x009018e4
        GameHeapFree(g_pCondorCameraBin, "C:\\FF7\\condor\\cd_init.cpp", 1124);
        g_pCondorCameraBin = 0;
    }
    if ((g_CondorTextureRenderObjects.renderObject08) != 0) {
        DestroyRenderObject((void *)(g_CondorTextureRenderObjects.renderObject08));
        (g_CondorTextureRenderObjects.renderObject08) = 0;
    }
    if ((g_CondorTextureRenderObjects.renderObject0C) != 0) {
        DestroyRenderObject((void *)(g_CondorTextureRenderObjects.renderObject0C));
        (g_CondorTextureRenderObjects.renderObject0C) = 0;
    }
    if ((g_CondorTextureRenderObjects.renderObject10) != 0) {
        DestroyRenderObject((void *)(g_CondorTextureRenderObjects.renderObject10));
        (g_CondorTextureRenderObjects.renderObject10) = 0;
    }
    if ((g_CondorTextureRenderObjects.renderObject14) != 0) {
        DestroyRenderObject((void *)(g_CondorTextureRenderObjects.renderObject14));
        (g_CondorTextureRenderObjects.renderObject14) = 0;
    }
    if ((g_CondorTextureRenderObjects.renderObject18) != 0) {
        DestroyRenderObject((void *)(g_CondorTextureRenderObjects.renderObject18));
        (g_CondorTextureRenderObjects.renderObject18) = 0;
    }
    if ((g_CondorTextureRenderObjects.renderObject1C) != 0) {
        DestroyRenderObject((void *)(g_CondorTextureRenderObjects.renderObject1C));
        (g_CondorTextureRenderObjects.renderObject1C) = 0;
    }
    if ((g_CondorTextureRenderObjects.renderObject20) != 0) {
        DestroyRenderObject((void *)(g_CondorTextureRenderObjects.renderObject20));
        (g_CondorTextureRenderObjects.renderObject20) = 0;
    }
    if ((g_CondorTextureRenderObjects.renderObject24) != 0) {
        DestroyRenderObject((void *)(g_CondorTextureRenderObjects.renderObject24));
        (g_CondorTextureRenderObjects.renderObject24) = 0;
    }
    if ((g_CondorTextureRenderObjects.renderObject28) != 0) {
        DestroyRenderObject((void *)(g_CondorTextureRenderObjects.renderObject28));
        (g_CondorTextureRenderObjects.renderObject28) = 0;
    }
    if ((g_CondorTextureRenderObjects.renderObject2C) != 0) {
        DestroyRenderObject((void *)(g_CondorTextureRenderObjects.renderObject2C));
        (g_CondorTextureRenderObjects.renderObject2C) = 0;
    }
    if (((int *)&g_CondorTextureRenderObjects)[0] != 0) {
        DestroyRenderObject((void *)((int *)&g_CondorTextureRenderObjects)[0]);
        ((int *)&g_CondorTextureRenderObjects)[0] = 0;
    }
    if ((g_CondorTextureRenderObjects.renderObject04) != 0) {
        DestroyRenderObject((void *)(g_CondorTextureRenderObjects.renderObject04));
        (g_CondorTextureRenderObjects.renderObject04) = 0;
    }
    if ((g_CondorTextureRenderObjects.renderObject30) != 0) {
        DestroyRenderObject((void *)(g_CondorTextureRenderObjects.renderObject30));
        (g_CondorTextureRenderObjects.renderObject30) = 0;
    }
    for (objectIdx = 0; objectIdx < 7; objectIdx++) {
        if (g_CondorUnitModelTable[objectIdx].ready != 0) {
            for (partIndex = 0; partIndex < 4; partIndex++) {
                if (g_CondorUnitModelTable[objectIdx].parts[partIndex].renderObject != 0) {
                    DestroyRenderObject((void *)g_CondorUnitModelTable[objectIdx].parts[partIndex].renderObject);
                    g_CondorUnitModelTable[objectIdx].parts[partIndex].renderObject = 0;
                }
            }
        }
    }
}
/*
 * Orchestrates loading of all textures, model tables, camera, and battlefield assets.
 */
// FUNCTION: FF7 0x005f4273
int __cdecl Mini_CondorLoadAllResources(struct GfxEngineContext *context, struct PfileLoadOptions *loadOptions, int offsetX, int offsetY)
{
    char *name;
    g_CondorInputMask = 0;
    SetGeomOffset(offsetX, offsetY);
    // STRING: FF7 0x00901900
    name = "data\\data.bin";
    if (Mini_CondorLoadDataBinFile(name) == 0) {
        return 0;
    } else {
        // STRING: FF7 0x00901910
        name = "data\\camera.bin";
        if (Mini_CondorLoadCameraFile(name) == 0) {
            return 0;
        } else {
            // STRING: FF7 0x00901920
            name = "data\\id.bin";
            if (Mini_CondorLoadIdFileAndUnitTable(name) == 0) {
                return 0;
            } else {
                SetMatrixLayoutMode(1);
                if (g_CondorPresetMode == 2) {
                    // STRING: FF7 0x0090192c
                    name = "data\\ehelp1.tim";
                } else {
                    // STRING: FF7 0x0090193c
                    name = "data\\ehelp.tim";
                }
                if (Mini_CondorLoadHelpTimFile(name) == 0) {
                    return 0;
                } else {
                    Mini_CondorBuildTimSurface(g_CondorHelpTimBuffer);
                    if (Mini_CondorCreateTextureRenderObjects(context, loadOptions) == 0) {
                        return 0;
                    } else {
                        return 1;
                    }
                }
            }
        }
    }
}
/*
 * Loads unit statistics and characteristics table from disk or archive.
 */
// FUNCTION: FF7 0x005f434b
int __cdecl Mini_CondorLoadUnitTable(char *fileName, CondorRegionRecord *unitTable)
{
    CondorRegionRecord *source;
    unsigned short i;

    g_CondorUnitTableBuffer = (struct CondorRegionRecord *)(Mini_CondorLoadFile(fileName));
    if (g_CondorUnitTableBuffer != 0) {
        source = g_CondorUnitTableBuffer;
    }
    for (i = 0; i < g_CondorMatchListState.regionCount; i++) {
        *(Int64Pair *)((char *)unitTable + i * 0x4C) = *(Int64Pair *)((char *)source + i * 0x4C);
        *(Int64Pair *)((char *)unitTable + i * 0x4C + 8) = *(Int64Pair *)((char *)source + i * 0x4C + 8);
        *(Int64Pair *)((char *)unitTable + i * 0x4C + 0x10) = *(Int64Pair *)((char *)source + i * 0x4C + 0x10);
        *(Int64Pair *)((char *)unitTable + i * 0x4C + 0x18) = *(Int64Pair *)((char *)source + i * 0x4C + 0x18);
        *(Int64Pair *)((char *)unitTable + i * 0x4C + 0x20) = *(Int64Pair *)((char *)source + i * 0x4C + 0x20);
        *(Int64Pair *)((char *)unitTable + i * 0x4C + 0x28) = *(Int64Pair *)((char *)source + i * 0x4C + 0x28);
        *(Int64Pair *)((char *)unitTable + i * 0x4C + 0x30) = *(Int64Pair *)((char *)source + i * 0x4C + 0x30);
        *(Int64Pair *)((char *)unitTable + i * 0x4C + 0x38) = *(Int64Pair *)((char *)source + i * 0x4C + 0x38);
        unitTable[i].minX = source[i].minX;
        unitTable[i].maxX = source[i].maxX;
        unitTable[i].minY = source[i].minY;
        unitTable[i].maxY = source[i].maxY;
        unitTable[i].rotX = source[i].rotX;
        unitTable[i].rotZ = source[i].rotZ;
    }
    // The two debug dumps of the unit table ("svt1.tst", "vert1.tst") are discarded expressions here: retail
    // keeps their 14 literals between Mini_CondorLoadAllResources' and this function's __FILE__, and no code.
    // STRING: FF7 0x0090194c
    (void)"wt";
    // STRING: FF7 0x00901950
    (void)"svt1.tst";
    // STRING: FF7 0x0090195c
    (void)"Svt %d:\n         ";
    // STRING: FF7 0x00901970
    (void)"n %d-%d-%d\n        ";
    // STRING: FF7 0x00901984
    (void)"v0 %d %d %d\n          ";
    // STRING: FF7 0x0090199c
    (void)"v1 %d %d %d\n          ";
    // STRING: FF7 0x009019b4
    (void)"v2 %d %d %d\n          ";
    // STRING: FF7 0x009019cc
    (void)"\n";
    // STRING: FF7 0x009019d0
    (void)"wt";
    // STRING: FF7 0x009019d4
    (void)"vert1.tst";
    // STRING: FF7 0x009019e0
    (void)"vert %d:\n      ";
    // STRING: FF7 0x009019f0
    (void)"minx %d - maxx%d \n    ";
    // STRING: FF7 0x00901a08
    (void)"miny %d - maxy%d \n    ";
    // STRING: FF7 0x00901a20
    (void)"\n";
    // STRING: FF7 0x00901a24
    GameHeapFree(g_CondorUnitTableBuffer, "C:\\FF7\\condor\\cd_init.cpp", 1381);
    g_CondorUnitTableBuffer = 0;
    return 1;
}
} /* extern "C" */
