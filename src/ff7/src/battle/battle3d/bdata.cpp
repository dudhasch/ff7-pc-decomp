// src/ff7/src/battle/battle3d/bdata.cpp
// Original: C:\FF7\src\battle\battle3d\bdata.cpp
// Contents: Battle data and effect resource directories: stores the data and effect roots, pushes
//   and pops the working directory around file access, and builds file paths. It also loads LMD
//   model files for battle actors, stages and shadows, setting their load options and vertex alpha.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/api/file.h"
#include "lib/api/mem.h"
#include "ff7/api/battle.h"
#include "lib/api/graphics.h"
#include "ff7/types/battle.h"
#include "lib/types/graphics.h"
#include "lib/enums/graphics.h"
#include "lib/pfile.h"
#include "lib/crt.h"
extern "C" {
// clang-format off
// Pointer words: they point at the objects that hold those bytes. The externs stay above the
// markers.
// GLOBAL: FF7 0x008ff290
char *gBattleDataDirectory = (char *)g_BattleDataDirBuffer;
// GLOBAL: FF7 0x008ff294
char *gBattleFxResourceRoot = (char *)g_BattleDataDirBuffer + 4;
// GLOBAL: FF7 0x008ff298
char *gBattleFxResourceRootAlt = (char *)g_BattleDataDirBuffer + 8;
unsigned char g_BattleDataDirBuffer[16] = {0};
// clang-format on

/*
 * Zero-fills a 0x44-byte LMD load descriptor and sets its three flags at indices 1..3 to 0, 1, 1.
 */
// FUNCTION: FF7 0x005dfdc0
void __cdecl Battle_InitLmdLoadDesc(int *desc)
{
    /* No local: the original reloads the parameter for each store. Caching it
       in a pointer variable costs a frame slot the target does not have. */
    ZeroFillMemory32(68, desc);
    desc[1] = 0;
    desc[2] = 1;
    desc[3] = 1;
}

/*
 * Returns the battle data directory string gBattleDataDirectory.
 */
// FUNCTION: FF7 0x005dfdf1
char *__cdecl Battle_GetDataDirectory(void)
{
    return gBattleDataDirectory;
}

/*
 * Sets the battle data directory string gBattleDataDirectory.
 */
// FUNCTION: FF7 0x005dfdfb
void __cdecl Battle_SetDataDirectory(char *dir)
{
    gBattleDataDirectory = dir;
}

/*
 * The second argument to ChangeDirectorySaving is where the OLD directory is written, which is why
 * Battle_PopDataDirectory passes 0 there and this passes a buffer. Its return goes
 * into the slot's first dword.
 */
// FUNCTION: FF7 0x005dfe08
void __cdecl Battle_PushDataDirectory(int *saveSlot)
{
    char *dir;

    ZeroFillMemory32(204, saveSlot);
    dir = Battle_GetDataDirectory();
    if (dir != 0 && *dir != 0) {
        saveSlot[0] = ChangeDirectorySaving(dir, (char *)(saveSlot + 1));
    }
}

/*
 * Restores the directory saved by the matching push (saveSlot[0] nonzero) through
 * ChangeDirectorySaving.
 */
// FUNCTION: FF7 0x005dfe51
void __cdecl Battle_PopDataDirectory(int *saveSlot)
{
    if (saveSlot[0] != 0) {
        ChangeDirectorySaving((char *)(saveSlot + 1), 0);
    }
}

/*
 * Formats the data directory plus slash and leaf when leaf is nonnull, otherwise only the data
 * directory; returns out. No output bound is checked.
 */
// FUNCTION: FF7 0x005dfe6f
char *__cdecl Battle_BuildDataPath(char *leaf, char *out)
{
    if (leaf != 0) {
        // STRING: FF7 0x008ff29c
        sprintf(out, "%s/%s", Battle_GetDataDirectory(), leaf);
    } else {
        // STRING: FF7 0x008ff2a4
        sprintf(out, "%s", Battle_GetDataDirectory());
    }
    return out;
}

/*
 * Returns the battle effect resource root gBattleFxResourceRoot.
 */
// FUNCTION: FF7 0x005dfeb1
char *__cdecl BattleFx_GetResourceRoot(void)
{
    return gBattleFxResourceRoot;
}

/*
 * Sets the battle effect resource root gBattleFxResourceRoot.
 */
// FUNCTION: FF7 0x005dfebb
void __cdecl BattleFx_SetResourceRoot(char *dir)
{
    gBattleFxResourceRoot = dir;
}

/*
 * Clears a 0xCC-byte save slot and, when a resource root is set, pushes it as the file context's
 * directory, saving the previous one after the slot's first dword.
 */
// FUNCTION: FF7 0x005dfec8
void __cdecl BattleFx_PushResourceDirectory(void *fileCtx, int *saveSlot)
{
    char *root;

    ZeroFillMemory32(204, saveSlot);
    root = BattleFx_GetResourceRoot();
    if (root != 0 && *root != 0) {
        saveSlot[0] =
            PushFileContextDirectory((FileOpenRequest *)fileCtx, root, (char *)(saveSlot + 1));
    }
}

/*
 * Pops the directory saved by BattleFx_PushResourceDirectory when the slot's first dword says one
 * was pushed.
 */
// FUNCTION: FF7 0x005dff15
void __cdecl BattleFx_PopResourceDirectory(void *fileCtx, int *saveSlot)
{
    if (saveSlot[0] != 0) {
        PushFileContextDirectory((FileOpenRequest *)fileCtx, (char *)(saveSlot + 1), 0);
    }
}

/*
 * Formats the effect resource root plus slash and leaf when leaf is nonnull, otherwise only the
 * root; returns out. No output bound is checked.
 */
// FUNCTION: FF7 0x005dff37
char *__cdecl BattleFx_BuildResourcePath(char *leaf, char *out)
{
    if (leaf != 0) {
        // STRING: FF7 0x008ff2a8
        sprintf(out, "%s/%s", BattleFx_GetResourceRoot(), leaf);
    } else {
        // STRING: FF7 0x008ff2b0
        sprintf(out, "%s", BattleFx_GetResourceRoot());
    }
    return out;
}

/*
 * Returns the alternate battle effect resource root gBattleFxResourceRootAlt.
 */
// FUNCTION: FF7 0x005dff79
char *__cdecl BattleFx_GetResourceRootAlt(void)
{
    return gBattleFxResourceRootAlt;
}

/*
 * Sets the alternate battle effect resource root gBattleFxResourceRootAlt.
 */
// FUNCTION: FF7 0x005dff83
void __cdecl BattleFx_SetResourceRootAlt(char *dir)
{
    gBattleFxResourceRootAlt = dir;
}

/*
 * Sets the model's vertex color (the dword at +0x10) to RGB zero with alpha 255 when the engine
 * context's flag bit 0 is set and mode is 2, otherwise with alpha 96.
 */
// FUNCTION: FF7 0x005dff90
void __cdecl BattleFx_SetLmdVertexAlphaByMode(int mode, void *model)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[8];
        int colorDword;
        struct { unsigned char pad_to_1[0x1]; char channelByteB; };
        struct { unsigned char pad_to_2[0x2]; char channelByteA; };
        struct { unsigned char pad_to_3[0x3]; char alphaByte; };
        struct { unsigned char pad_to_4[0x4]; GfxEngineContext *engineContext; };
    } frame;
#pragma pack(pop)

    frame.colorDword = *(int *)((char *)model + 0x10);
    frame.engineContext = GetEngineContext();
    if ((frame.engineContext->flags & 1) != 0) {
        if (mode == 2) {
            frame.alphaByte = 0xFF;
            frame.channelByteA = 0;
            frame.channelByteB = 0;
            *(char *)&frame.colorDword = 0;
        } else {
            frame.alphaByte = 0x60;
            frame.channelByteA = 0;
            frame.channelByteB = 0;
            *(char *)&frame.colorDword = 0;
        }
    } else {
        frame.alphaByte = 0x60;
        frame.channelByteA = 0;
        frame.channelByteB = 0;
        *(char *)&frame.colorDword = 0;
    }
    *(int *)((char *)model + 0x10) = frame.colorDword;
}

/*
 * Loads an LMD model file from disk, sets load options, and builds its texture atlas and parts.
 */
// FUNCTION: FF7 0x005dfffe
void *__cdecl Lmd_LoadModelFile(int flatShading, int wantMatrixSet, unsigned int blendMode, char *fileName, void *timObject)
{
    Battle3dPartyLoadDesc *loadDescPtr;
    void *fileData;
    PfileLoadOptions optBuf;
    int unusedFlag;
    void *atlas;
    int loadedSize;
    Battle3dPartyLoadDesc wordStorage;
    char dataPath[0xC8];
    int stackHole;
    void *resultPtr;
    PfileLoadOptions *optPtr;

    loadDescPtr = &wordStorage;
    Battle_InitLmdLoadDesc((int *)loadDescPtr);
    loadDescPtr->hasPolygons = flatShading;
    Battle_BuildDataPath(fileName, dataPath);
    unusedFlag = 0;
    stackHole = 0;
    optPtr = &optBuf;
    Pfile_InitLoadOptions(optPtr);
    Pfile_SetLoadOptionsBlendMode(blendMode, optPtr);
    optPtr->flags |= 0x2000000;
    if (wantMatrixSet != 0) {
        optPtr->matrixCount = 1;
    }
    resultPtr = 0;
    fileData = LoadDiskFile(&loadedSize, dataPath);
    if (fileData != 0) {
        atlas = Lmd_CreateTextureAtlas(2, timObject);
        if (atlas != 0) {
            *(char **)((char *)atlas + 4) = dataPath;
            resultPtr = Lmd_BuildAndRegisterPart((Battle3dPartyLoadDesc *)loadDescPtr, (int)fileData, (struct PolygonTim *)timObject, optPtr, (Battle3dLmdFaceData *)atlas, dataPath);
            Lmd_DestroyTextureAtlas((Battle3dLmdFaceData *)atlas);
        }
        // STRING: FF7 0x008ff2b4
        GameHeapFree(fileData, "C:\\FF7\\src\\battle\\battle3d\\bdata.cpp", 229);
    }
    return resultPtr;
}

/*
 * Forwards the four DWORD arguments and preserves the returned model pointer.
 */
// FUNCTION: FF7 0x005e0154
void *__cdecl Battle_LoadLmdModelByName(int wantMatrixSet, unsigned int blendMode, char *fileName,
                                        void *timObject)
{
    return Lmd_LoadModelFile(0, wantMatrixSet, blendMode, fileName, timObject);
}

/*
 * Loads the actors' ground shadow model KAGE.DAT into model slot `slot` and returns it (0 on
 * failure). Under the software driver it loads the software variant and sets object
 * flag 0x4000; every polygon set gets its vertex alpha from BattleFx_SetLmdVertexAlphaByMode.
 */
// FUNCTION: FF7 0x005e0173
void *__cdecl Battle_LoadShadowModel(int slot)
{
    GfxEngineContext *pEngineContext;
    char *fileName;
    void *model;
    int variant;
    int flagBits;
    // STRING: FF7 0x008ff2dc
    fileName = "KAGE.DAT";
    variant = 0;
    pEngineContext = GetEngineContext();
    if (pEngineContext->driverType == GFX_DRIVER_SOFTWARE) {
        variant = 1;
    }
    model = Lmd_LoadModelFile(variant, slot, 0, fileName, 0);
    if (model != 0) {
        Gfx_ForEachPolygonSetGeometry((DxGroupCallback)BattleFx_SetLmdVertexAlphaByMode, *(int *)((char *)model + 4));
        if (variant != 0) {
            flagBits = 0x4000;
            Gfx_OrLmdPolygonSetRenderFlags(flagBits, model);
        }
    }
    return model;
}
} /* extern "C" */
