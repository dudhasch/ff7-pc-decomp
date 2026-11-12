// src/lib/src/polygon/rsd.cpp
// Original: C:\lib\src\polygon\rsd.cpp
// Contents: RSD model loading and release: sets load options and default model flags, loads and
//   frees models and model sets, and installs transform matrices and vertex colours on them.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include <windows.h>
#include "lib/codegen.h"
#include <d3d.h>
#include "lib/api/file.h"
#include "lib/api/graphics.h"
#include "lib/api/polygon.h"
#include "lib/api/mem.h"
#include "lib/types/graphics.h"
#include "lib/types/polygon.h"
#include "lib/types/file.h"
#include <stdio.h>
#include <stdlib.h>
#include "lib/pfile.h"
#include "lib/enums/graphics.h"

struct GfxEngineContext;
struct GfxMatrixArray;
struct PolygonFileArrays;
struct PolygonRsdRecord;
extern "C" {
// clang-format off
// GLOBAL: FF7 0x0090cf38
int g_PfileAssetSourceMode = 1;
// GLOBAL: FF7 0x0090cf3c
int g_PfileDefaultModelFlags = 32;
// The two empty strings the RSD load error message starts with (its path separator and directory): the TU's .bss
// GLOBAL: FF7 0x00d8f670
unsigned char g_RsdPathSeparator[4] = {0};
// GLOBAL: FF7 0x00d8f674
unsigned char g_RsdDirectory[4] = {0};
// clang-format on
/* Unprototyped retail call signature (parameter/return width differs from callee definition). */
extern int __cdecl Pfile_LoadRsdContents(void *options, int scale, struct PolygonRsdRecord *rsd);

/*
 * Returns the current asset source mode flag for model loading.
 */
// FUNCTION: FF7 0x00674530
int __cdecl Pfile_GetAssetSourceMode(void)
{
    return g_PfileAssetSourceMode;
}

/*
 * Sets `g_PfileAssetSourceMode`, which the next `Pfile_InitLoadOptions` and
 * `Pfile_InitLoadOptionsWithBlend` copy into the load options' `sourceMode`: 0 loads the
 * prebuilt `.TEX`, `.P` and binary animation files; nonzero (the initial value 1) first builds
 * them from the `.TIM`, PLY/MAT and animation text sources. The main dispatcher sets 0 when it
 * enters each game mode; battle and the credits switch it around their own loads.
 */
// FUNCTION: FF7 0x0067453a
void __cdecl Pfile_SetAssetSourceMode(int value)
{
    g_PfileAssetSourceMode = value;
}

/*
 * Returns the default model flags used during model loading.
 */
// FUNCTION: FF7 0x00674547
int __cdecl Pfile_GetDefaultModelFlags(void)
{
    return g_PfileDefaultModelFlags;
}

/*
 * Sets the default model flags used during model loading.
 */
// FUNCTION: FF7 0x00674551
void __cdecl Pfile_SetDefaultModelFlags(int value)
{
    g_PfileDefaultModelFlags = value;
}

/*
 * Fills the 0x74-byte load options `opts` with the defaults a module uses before loading models:
 * all zero, then `flags` from `g_PfileDefaultModelFlags` (initially 0x20; the main dispatcher,
 * field, chocobo and condor set 0x100000), `modelKind` 1, `sourceMode` from
 * `g_PfileAssetSourceMode`, `createMatrices` 1, `polygonFlags` 2, `matrixCount` 0, `blendMode`
 * BLEND_NONE, `scale` 1.0, `file.mode` 0 (read) and `loadBoneParts` 1 (`Skel_BuildHierarchyAndLoadParts`
 * then loads the parts' RSD models). A null `opts` is ignored.
 */
// FUNCTION: FF7 0x0067455e
void __cdecl Pfile_InitLoadOptions(PfileLoadOptions *opts)
{
    if (opts != 0) {
        ZeroFillMemory32(sizeof(*opts), opts);
        opts->flags = Pfile_GetDefaultModelFlags();
        opts->drawEnabled = 1;
        opts->sourceMode = Pfile_GetAssetSourceMode();
        opts->createMatrices = 1;
        opts->polygonFlags = 2;
        opts->matrixCount = 0;
        opts->blendMode = BLEND_NONE;
        opts->lightFlags = 0;
        opts->scale = 1.0f;
        opts->file.mode = 0;
        opts->loadBoneParts = 1;
    }
}

/*
 * Fills the load options `opts` for a model drawn with semi-transparency mode `blend` (a
 * `BlendMode`): all zero, then `modelKind` 1, `sourceMode` from `g_PfileAssetSourceMode`,
 * `createMatrices` 1, `matrixCount` 1, `blendMode` = `blend`, `blendFlags` 1, `scale` 1.0 and
 * `loadBoneParts` 1. Unlike `Pfile_InitLoadOptions` it leaves `flags` and `polygonFlags` at 0. A null `opts`
 * is ignored.
 */
// FUNCTION: FF7 0x006745e6
void __cdecl Pfile_InitLoadOptionsWithBlend(int blend, PfileLoadOptions *opts)
{
    if (opts != 0) {
        ZeroFillMemory32(sizeof(*opts), opts);
        opts->drawEnabled = 1;
        opts->sourceMode = Pfile_GetAssetSourceMode();
        opts->createMatrices = 1;
        opts->matrixCount = 1;
        opts->blendMode = blend;
        opts->blendFlags = 1;
        opts->lightFlags = 0;
        opts->scale = 1.0f;
        opts->loadBoneParts = 1;
    }
}

/*
 * Sets the semi-transparency mode (`BlendMode`) of the models loaded next with `opts`: writes
 * `opts->blendMode`, which `Pfile_LoadRsdModelEx` applies to every hundred record of the model.
 * A null `opts` is ignored.
 */
// FUNCTION: FF7 0x00674659
void __cdecl Pfile_SetLoadOptionsBlendMode(int blend,
                                           PfileLoadOptions *opts)
{
    if (opts != 0) {
        opts->blendMode = blend;
    }
}

/*
 * Frees a model `Pfile_LoadRsdModelEx` loaded, when a module drops it: the matrix array
 * (`DestroyRenderMaterial`), the polygon set after removing it from the graphics context's list,
 * the parsed RSD record (`Pfile_FreeRsdRecord`), and the 0xC-byte handle. A
 * null `model` is ignored.
 */
// FUNCTION: FF7 0x0067466d
void __cdecl Pfile_FreeRsdModel(void *model, void *ctx)
{
    if (model != 0) {
        DestroyRenderMaterial(((PolygonRsdModel *)model)->matrices);
        if (((PolygonRsdModel *)model)->polygonSet != 0) {
            D3DRemovePolygonSetFromList(((PolygonRsdModel *)model)->polySet, ctx);
            D3DFreePolygonSet((struct GfxPolygonSet *)(((PolygonRsdModel *)model)->polygonSet));
        }
        Pfile_FreeRsdRecord((PolygonRsdRecord *)(((PolygonRsdModel *)model)->rsdRecord));
        // STRING: FF7 0x0090cf40
        GameHeapFree(model, "C:\\lib\\src\\polygon\\rsd.cpp", 113);
    }
}

/*
 * Frees a model handle and clears the caller's handle pointer.
 */
// FUNCTION: FF7 0x006746d3
void __cdecl Pfile_FreeModelAndNull(int handlePtr, void *context)
{
    if (handlePtr != 0 && *(int *)handlePtr != 0) {
        Pfile_FreeRsdModel(*(void **)handlePtr, context);
        *(int *)handlePtr = 0;
    }
}

/*
 * Loads the `.RSD` model `fileName` as `source` (the load options) says and returns a 0xC-byte
 * handle (matrix array, polygon set, parsed RSD record) ready to draw, or 0.
 *
 * When `source->directory` is set, the file is looked up there (`PushFileContextDirectory`, left
 * again at the end). Without `existingRecord` (battle passes one it parsed itself)
 * `Pfile_ParseRsdFile` reads the RSD and `Pfile_LoadRsdContents` loads its textures and geometry
 * at `source->scale`; when the contents fail it prints "ERROR: COULD NOT LOAD RSD DATA" with the
 * directory and name and frees the handle. With a record, a matrix array of `matrixCount`
 * entries with an identity matrix in slot 0 is made when `createMatrices` is set. For a model
 * with meshes the blend mode goes onto each hundred record, the textures are bound,
 * `GfxLoadPolygonSet` builds the polygon set, `D3DAppendPolygonSetToList` adds it to the graphics
 * context's list, and the set gets `modelKind` and, with matrices, `drawListActive` 1.
 *
 * Defects: when `Pfile_ParseRsdFile` fails (a missing file) it returns the empty handle, not 0,
 * and prints nothing. The defaults for a null `source` are dead: without `existingRecord` it
 * passes `&source->file` to the parser, and with a record it reads
 * `source->createMatrices` through the null pointer.
 */
// FUNCTION: FF7 0x00674701
int __cdecl Pfile_LoadRsdModelEx(PolygonRsdRecord *existingRecord, PfileLoadOptions *source, int fileName, struct GfxEngineContext *context)
{
    int scale;
    GfxPolygonSet *polygonSet;
    int matrixCount;
    int srcMode;
    PolygonRsdRecord *pParsedRecord;
    int modelKind;
    int blendParams;
    PolygonRsdModel *handle;
    char msgBuf[200];
    char buffer[200];
    GfxMatrixArray *matrices;
    int parameterValue;
    char *pDirName;
    char *pathSepPtr;
    GfxMatrix4x4 *pIdentity;

    modelKind = 1;
    srcMode = Pfile_GetAssetSourceMode();
    scale = FLOAT_ONE_BITS;
    matrixCount = 1;
    blendParams = 0;
    if (source != 0) {
        modelKind = source->drawEnabled;
        srcMode = source->sourceMode;
        scale = *(int *)&source->scale;
        matrixCount = source->matrixCount;
        blendParams = (int)&source->blendFlags;
        if (source->directory != 0) {
            PushFileContextDirectory(&source->file, source->directory, buffer);
        }
    }
    polygonSet = 0;
    // STRING: FF7 0x0090cf5c
    handle = (PolygonRsdModel *)GameHeapAlloc(1, 12, "C:\\lib\\src\\polygon\\rsd.cpp", 162);
    if (handle != 0) {
        if (existingRecord != 0) {
            pParsedRecord = existingRecord;
        } else {
            pParsedRecord = (PolygonRsdRecord *)Pfile_ParseRsdFile(&source->file, fileName);
            if (pParsedRecord != 0) {
                if (Pfile_LoadRsdContents((void *)source, scale, pParsedRecord)) {
                    // Loaded. The empty arm is the image: the compiler emits the
                    // jump over the else, not a single inverted branch.
                } else {
                    Pfile_FreeRsdRecord(pParsedRecord);
                    pParsedRecord = 0;
                    pathSepPtr = (char *)&g_RsdPathSeparator;
                    pDirName = (char *)&g_RsdDirectory;
                    if (source->directory != 0) {
                        // STRING: FF7 0x0090cf78
                        pathSepPtr = "\\";
                        pDirName = source->directory;
                    }
                    // STRING: FF7 0x0090cf7c
                    sprintf(msgBuf, "ERROR: COULD NOT LOAD RSD DATA %s%s%s\n", pDirName, pathSepPtr, fileName);
                    Gfx_DebugPrint(msgBuf);
                    Pfile_FreeModelAndNull((int)&handle, context);
                }
            }
        }
        if (pParsedRecord != 0) {
            handle->rsdRecord = (int)pParsedRecord;
            if (pParsedRecord != 0) {
                matrices = 0;
                parameterValue = 0;
                if (source->createMatrices != 0) {
                    matrices = (GfxMatrixArray *)CreateMatrixArray(matrixCount);
                    if (matrices != 0) {
                        pIdentity = (GfxMatrix4x4 *)Gfx_AllocIdentityMatrix4x4();
                        if (pIdentity != 0) {
                            Gfx_SetMatrixCall(0, (int)pIdentity, (int)matrices, context);
                        }
                    }
                }
                handle->matrices = (struct GfxRenderMaterial *)matrices;
                if (pParsedRecord->meshCount > 0) {
                    Pfile_RefreshHundredBlendModes((PolygonFileArrays *)(*(int *)pParsedRecord->meshes));
                    /* Retail caller passes 3 arg(s) to 2-parameter `Pfile_ApplyBlendModeToHundreds` (__cdecl stack cleanup matches retail). */
                    ((void(__cdecl *)(int, int, int))Pfile_ApplyBlendModeToHundreds)(blendParams, *(int *)pParsedRecord->meshes, (int)context);
                    Pfile_BindTexturesToHundreds(source, pParsedRecord->count,
                                                 pParsedRecord->textures,
                                                 (PolygonFileArrays *)(*(int *)pParsedRecord->meshes), context);
                    polygonSet = GfxLoadPolygonSet(matrices, (struct PolygonHundredState *)parameterValue, (PolygonFileArrays *)(*(int *)pParsedRecord->meshes), (struct GfxPolygonSetSpec *)source, context);
                    if (polygonSet != 0) {
                        handle->polygonSet = (int)polygonSet;
                        D3DAppendPolygonSetToList((struct GfxPolygonSet *)(handle->polygonSet), context);
                        polygonSet->drawEnabled = modelKind;
                        if (matrixCount > 0) {
                            polygonSet->drawListActive = 1;
                        }
                    }
                }
            }
        }
    }
    if (source != 0 && source->directory != 0) {
        PushFileContextDirectory(&source->file, (char *)((int)buffer), 0);
    }
    return (int)handle;
}

/*
 * Loads the `.RSD` model `fileName` with the caller's load options `source` and returns its
 * handle, or 0: `Pfile_LoadRsdModelEx` without a pre-parsed record. Skeleton parts, model sets,
 * model tables and the highway roster load their models this way.
 */
// FUNCTION: FF7 0x00674a0a
int __cdecl LoadRsdModel(struct PfileLoadOptions *source, const char *fileName, void *context)
{
    return Pfile_LoadRsdModelEx(0, source, (int)fileName, (struct GfxEngineContext *)context);
}

/*
 * Sets the element count in the model polygon set header array.
 */
// FUNCTION: FF7 0x00674a25
void __cdecl Pfile_StoreValueInModelPolygonSetHead(int value, PolygonRsdModel *modelHandle)
{
    if (modelHandle != 0 && modelHandle->polygonSet != 0) {
        ((GfxPolygonSet *)modelHandle->polygonSet)->drawEnabled = value;
    }
}

/*
 * Installs device default transform matrix on the model if not yet set.
 */
// FUNCTION: FF7 0x00674a44
void __cdecl Pfile_InstallDefaultMatrixOnModel(PolygonRsdModel *handlePtr, struct GfxEngineContext *context)
{
    int matrixArray;

    if (handlePtr != 0) {
        matrixArray = handlePtr->matricesRaw;
        if (matrixArray != 0) {
            if (((GfxMatrixArray *)matrixArray)->hasMatrices != 0) {
                return;
            }
            Gfx_SetMatrixCall(0, context->cameraMatrix, matrixArray, context);
        }
    }
}

/*
 * Installs a transformation matrix on the specified model.
 */
// FUNCTION: FF7 0x00674a86
void __cdecl Gfx_SetModelMatrix(int matrix, int modelHandle, struct GfxEngineContext *context)
{
    int matrixArray;

    if (modelHandle != 0) {
        matrixArray = ((PolygonRsdModel *)modelHandle)->matricesRaw;
        if (matrixArray != 0) {
            Gfx_SetMatrixCall(0, matrix, matrixArray, context);
        }
    }
}

/*
 * Forwards matrix parameters to the device matrix table slot for a model.
 */
// FUNCTION: FF7 0x00674ab8
void __cdecl Pfile_SetModelMatrixSlot(int slot, int value, int option, PolygonRsdModel *modelHandle)
{
    if (modelHandle != 0) {
        Gfx_SetMatrixTableSlot(slot, value, option, (struct GfxMatrixTable *)modelHandle->matrices);
    }
}

/*
 * Sets uniform vertex colors across all polygon sets in a model.
 */
// FUNCTION: FF7 0x00674add
void __cdecl Pfile_SetModelVertexColor(int color, int modelHandle)
{
    Gfx_SetPolygonSetVertexColor(color, (GfxPolygonSet *)(((PolygonRsdModel *)modelHandle)->polygonSet));
}

/*
 * Restores original vertex colors on model polygon sets.
 */
// FUNCTION: FF7 0x00674af5
void __cdecl Pfile_RestoreModelVertexColors(int modelHandle)
{
    Gfx_RestorePolygonSetVertexColors((GfxPolygonSet *)(((PolygonRsdModel *)modelHandle)->polygonSet));
}

/*
 * Prepares and initializes a model set request descriptor.
 */
// FUNCTION: FF7 0x00674b09
int __cdecl Pfile_InitModelSetRequest(int request)
{
    if (request != 0) {
        ZeroFillMemory32(16, (void *)(request));
    } else {
        // STRING: FF7 0x0090cfa4
        request = (int)(GameHeapAlloc(1, 16, "C:\\lib\\src\\polygon\\rsd.cpp", 377));
    }
    if (request != 0) {
        *(int *)request = 1;
    }
    return request;
}

/*
 * Releases each model in a model set, followed by its array and header.
 */
// FUNCTION: FF7 0x00674b4f
void __cdecl Pfile_FreeModelSet(void *models, void *ctx)
{
    int i;

    if (models != 0) {
        if (((PolygonModelSet *)models)->modelPointers != 0) {
            for (i = 0; i < ((PolygonModelSet *)models)->count; i++) {
                Pfile_FreeRsdModel(((PolygonModelSet *)models)->modelPointers[i], ctx);
            }
            // STRING: FF7 0x0090cfc0
            GameHeapFree(((PolygonModelSet *)models)->modelPointers, "C:\\lib\\src\\polygon\\rsd.cpp", 402);
        }
        // STRING: FF7 0x0090cfdc
        GameHeapFree(models, "C:\\lib\\src\\polygon\\rsd.cpp", 405);
    }
}

/*
 * Allocates memory for a model set header and child model pointer array.
 */
// FUNCTION: FF7 0x00674bcc
int __cdecl Pfile_AllocModelSet(int count, struct GfxEngineContext *context)
{
    PolygonModelSet *modelSet;

    // STRING: FF7 0x0090cff8
    modelSet = (PolygonModelSet *)((GameHeapAlloc(1, 8, "C:\\lib\\src\\polygon\\rsd.cpp", 413)));
    if (modelSet != 0) {
        // STRING: FF7 0x0090d014
        modelSet->modelPointers = (PolygonRsdModel **)(GameHeapAlloc(count, 4, "C:\\lib\\src\\polygon\\rsd.cpp", 416));
        if (modelSet->modelPointers != 0) {
            modelSet->count = count;
        } else {
            Pfile_FreeModelSet((void *)((int)modelSet), context);
            modelSet = 0;
        }
    }
    return (int)modelSet;
}

/*
 * Loads a set of `.RSD` models with load options `context` and returns a `PolygonModelSet`
 * (count, model pointers), or 0 when `fileList` is null or names no files. The condor minigame
 * loads its unit models this way.
 *
 * With `fileList->hasFiles` set, the names are every `*.RSD` in `context->directory`, or in the
 * current directory when it has none (a directory listing made there); otherwise they are the
 * list's own `count` names. Each name goes through `LoadRsdModel`. With `fileList->isEnumerated`
 * set, each name is split into its directory and file part first, and the model is loaded from
 * inside that directory (entered and left with `PushFileContextDirectory`). A model that fails
 * leaves its pointer 0. The listing and its name array are freed afterwards.
 *
 * Defect: the `isEnumerated` path dereferences `context` without the null check the other uses
 * have.
 */
// FUNCTION: FF7 0x00674c3e
int __cdecl Pfile_LoadModelSet(PolygonModelFileList *fileList, PfileLoadOptions *context, struct GfxEngineContext *engine)
{
    int idx;
    int entries;
    PolygonModelSet *result;
    char oldDir[200];
    int count;
    FileDirectoryListing *pListing;
    PolygonRsdModel *modelPointer;
    char *baseName;
    char *pDirName;
    char *fullPath;
    char *extensionPtr;
    char *pRsdName;
    char *drive;

    result = 0;
    if (fileList != 0) {
        if (context != 0 && context->directory != 0) {
            PushFileContextDirectory(&context->file, context->directory, oldDir);
        }
        count = 0;
        entries = 0;
        if (fileList->hasFiles != 0) {
            // STRING: FF7 0x0090d030
            pListing = (FileDirectoryListing *)BuildDirectoryListing(0, "*.RSD");
            if (pListing != 0) {
                count = pListing->entryCount;
                entries = AllocDirectoryEntryPointerArray(pListing);
            }
        } else {
            count = fileList->count;
            entries = fileList->entries;
        }
        if (context != 0 && context->directory != 0) {
            PushFileContextDirectory(&context->file, (char *)((int)oldDir), 0);
        }
        if (count > 0) {
            result = (PolygonModelSet *)Pfile_AllocModelSet(count, engine);
            if (result != 0) {
                for (idx = 0; idx < count; idx++) {
                    if (fileList->isEnumerated != 0) {
                        // STRING: FF7 0x0090d038
                        fullPath = (char *)(GameHeapAllocRaw(260, "C:\\lib\\src\\polygon\\rsd.cpp", 490));
                        // STRING: FF7 0x0090d054
                        drive = (char *)(GameHeapAllocRaw(3, "C:\\lib\\src\\polygon\\rsd.cpp", 491));
                        // STRING: FF7 0x0090d070
                        pDirName = (char *)(GameHeapAllocRaw(256, "C:\\lib\\src\\polygon\\rsd.cpp", 492));
                        // STRING: FF7 0x0090d08c
                        baseName = (char *)(GameHeapAllocRaw(256, "C:\\lib\\src\\polygon\\rsd.cpp", 493));
                        // STRING: FF7 0x0090d0a8
                        extensionPtr = (char *)(GameHeapAllocRaw(256, "C:\\lib\\src\\polygon\\rsd.cpp", 494));
                        pRsdName = ((char **)entries)[idx];
                        _splitpath(pRsdName, drive, pDirName, baseName, extensionPtr);
                        _makepath(fullPath, 0, 0, baseName, extensionPtr);
                        PushFileContextDirectory(&context->file, (char *)((int)pDirName), oldDir);
                        modelPointer = (PolygonRsdModel *)(LoadRsdModel(context, fullPath, engine));
                        if (modelPointer != 0) {
                            result->modelPointers[idx] = modelPointer;
                        }
                        PushFileContextDirectory(&context->file, (char *)((int)oldDir), 0);
                        // STRING: FF7 0x0090d0c4
                        GameHeapFree(fullPath, "C:\\lib\\src\\polygon\\rsd.cpp", 519);
                        // STRING: FF7 0x0090d0e0
                        GameHeapFree(drive, "C:\\lib\\src\\polygon\\rsd.cpp", 520);
                        // STRING: FF7 0x0090d0fc
                        GameHeapFree(pDirName, "C:\\lib\\src\\polygon\\rsd.cpp", 521);
                        // STRING: FF7 0x0090d118
                        GameHeapFree(baseName, "C:\\lib\\src\\polygon\\rsd.cpp", 522);
                        // STRING: FF7 0x0090d134
                        GameHeapFree(extensionPtr, "C:\\lib\\src\\polygon\\rsd.cpp", 523);
                    } else {
                        modelPointer = (PolygonRsdModel *)(LoadRsdModel(context, ((char **)entries)[idx], engine));
                        if (modelPointer != 0) {
                            result->modelPointers[idx] = modelPointer;
                        }
                    }
                }
            }
        }
        if (fileList->hasFiles != 0) {
            FreeDirectoryListing(pListing);
            // STRING: FF7 0x0090d150
            GameHeapFree((void *)entries, "C:\\lib\\src\\polygon\\rsd.cpp", 541);
        }
    }
    return (int)result;
}

/*
 * Stores a value in each entry of the model set list.
 */
// FUNCTION: FF7 0x00674fbd
void __cdecl Pfile_StoreValueInEachModelSetEntry(int value, PolygonModelSetMatrix *modelSet)
{
    PolygonRsdModel *model;
    int index;

    if (modelSet != 0) {
        for (index = 0; index < modelSet->count; index++) {
            model = modelSet->modelList[index];
            Pfile_StoreValueInModelPolygonSetHead(value, model);
        }
    }
}

/*
 * Installs default transformation matrix on each model in the set.
 */
// FUNCTION: FF7 0x0067500a
void __cdecl Pfile_InstallDefaultMatrixOnModelSet(PolygonModelSetMatrix *modelSet, struct GfxEngineContext *context)
{
    int index;

    if (modelSet != 0) {
        for (index = 0; index < modelSet->count; index++) {
            Pfile_InstallDefaultMatrixOnModel(modelSet->modelList[index], context);
        }
    }
}

/*
 * Applies a transformation matrix to each model in the model set.
 */
// FUNCTION: FF7 0x0067504f
void __cdecl Pfile_SetModelSetMatrix(int matrix, PolygonModelSetMatrix *modelSet, struct GfxEngineContext *context)
{
    int index;

    if (modelSet != 0) {
        for (index = 0; index < modelSet->count; index++) {
            Gfx_SetModelMatrix(matrix, (int)modelSet->modelList[index], context);
        }
    }
}

/*
 * Invokes a callback for each model entry member in a model set.
 */
// FUNCTION: FF7 0x00675098
void __cdecl Pfile_ForEachModelSetEntryMember(int context, void(__cdecl *arg2)(int, int), int modelSet)
{
    int index;
    if (modelSet != 0) {
        for (index = 0; index < ((PolygonModelSet *)modelSet)->count; index++) {
            arg2(context, ((PolygonModelSet *)modelSet)->modelPointers[index]->polygonSet);
        }
    }
}
} /* extern "C" */
