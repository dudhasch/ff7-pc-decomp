// src/ff7/src/battle/battle3d/stage.cpp
// Original: C:\FF7\src\battle\battle3d\stage.cpp
// Contents: Battle stage geometry: loads and frees legacy stage DAT models, keeps the table of
//   stages that have a floor, and submits and looks up stage part meshes, skipping an excluded
//   part.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/crt.h"

#include "ff7/api/battle.h"
#include "lib/api/graphics.h"
#include "lib/api/polygon.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_3d.h"
#include "lib/types/polygon.h"
#include "lib/types/graphics.h"
#include "lib/enums/graphics.h"
#include "lib/api/file.h"
#include "lib/api/mem.h"
#include "lib/codegen.h"
extern "C" {

/*
 * Frees a battle stage DAT model: its model set through the engine context, its heap block,
 * and the record.
 */
// FUNCTION: FF7 0x005e2f00
void __cdecl BattleStage_FreeStageDatModel(BattleStageDat *stage)
{
    if (stage != 0) {
        Pfile_FreeModelSet(stage->modelSet,
                           (void *)GetEngineContext());
        // STRING: FF7 0x008ffb10
        GameHeapFree(stage->heapBlock, "C:\\FF7\\src\\battle\\battle3d\\stage.cpp", 0x3b);
        // STRING: FF7 0x008ffb38
        GameHeapFree(stage, "C:\\FF7\\src\\battle\\battle3d\\stage.cpp", 0x3d);
    }
}

/*
 * Loads battle stage models, textures, and geometry from a legacy DAT file.
 */
// FUNCTION: FF7 0x005e2f49
void *__cdecl BattleStage_LoadStageDatLegacy(int stageId, int *battleLoadDesc, void *loadOptions, char *datName)
{
    /* These deliberately bucket-0 names are declared in reverse stack-slot order: VC5 emits
       locals sharing a hash bucket in reverse declaration order. */
    BattleStageDescriptor2 *jr;
    void *jb;
    unsigned int j2;
    int iv;
    void *i6;
    unsigned int hz;
    int hj;
    int gn;
    char fr[0xC8];
    int fb;
    int f2;
    unsigned int *ev;
    char ef[0xC8];
    char e6[0xC8];
    Battle3dLmdFaceData *dz;
    int dj;
    int *cn;
    Battle3dStageRecord *br;
    int bb;
    void *b2;
    PolygonTim *av;
    int af;
    void *a6;
    char p[0xCC];

    dj = 0;
    av = 0;
    br = 0;
    Battle_PushDataDirectory((int *)p);
    BattleStage_InitFloorFlagTable();
    b2 = LoadDiskFile(&bb, datName);
    if (b2 != 0) {
        fb = 0;
        dz = (Battle3dLmdFaceData *)Lmd_CreateTextureAtlas(2, (void *)av);
        if (dz != 0) {
            (*(char **)&dz->fileName) = datName;
            dz->allowLargeAtlas = 1;
            (*(void **)&dz->loadOptions) = loadOptions;
            // STRING: FF7 0x008ffb60
            br = (Battle3dStageRecord *)GameHeapAlloc(1, 0xC, "C:\\FF7\\src\\battle\\battle3d\\stage.cpp", 0x71);
            if (br != 0) {
                br->stageId = stageId;
                a6 = b2;
                cn = (int *)a6;
                f2 = *cn;
                cn++;
                ev = (unsigned int *)cn;
                if (DEBUG_TRACE) {
                    // STRING: FF7 0x008ffb88
                    sprintf(e6, "%s  TOTAL DATA %d \n", datName, f2);
                    Gfx_DebugPrint(e6);
                }
                for (gn = 0; gn < f2; gn++) {
                    if (DEAD_IN_RETAIL) {
                        if ((ev[gn] & 0x80000000) != 0) {
                            // STRING: FF7 0x008ffb9c
                            sprintf(e6, "OFFSET %d %d (SEMI-TRANSPARENT) \n", gn, ev[gn] & 0x7FFFFFFF);
                        } else {
                            // STRING: FF7 0x008ffbc0
                            sprintf(e6, "OFFSET %d %d \n", gn, ev[gn]);
                        }
                        Gfx_DebugPrint(e6);
                    }
                }
                cn += f2;
                af = ev[1] - ev[0];
                br->stageData = GameHeapMemDup(af, (char *)a6 + ev[0]);
                if (battleLoadDesc[0xA] != 0) {
                    BattleModel_WriteInfoFile(af, br->stageData, datName);
                }
                StripFileExtension(datName, fr);
                // STRING: FF7 0x008ffbd0
                sprintf(ef, "%s.TIM", fr);
                hj = bb - ev[f2 - 1];
                WriteWholeFile(hj, ef, (char *)a6 + ev[f2 - 1]);
                av = (PolygonTim *)(((void * (__cdecl *)(char *fileName))Pfile_LoadTim)(ef)); /* reads eax after a void call, as the original does */
                if (av != 0) {
                    fb = f2 - 2;
                    br->modelSet = (Battle3dStagePartTable *)Pfile_AllocModelSet(fb, GetEngineContext());
                    if (br->modelSet != 0) {
                        dz->faceKind = 0;
                        for (gn = 0; gn < fb; gn++) {
                            hz = ev[gn + 1] & 0x7FFFFFFF;
                            i6 = (char *)a6 + hz;
                            battleLoadDesc[0xD] = gn;
                            Battle_BuildPolygonDataFromLmd((Battle3dPartyLoadDesc *)battleLoadDesc, (int)i6, (Battle3dLmdFaceData *)dz, av);
                        }
                        Lmd_BuildAtlasTextures((Battle3dLmdFaceData *)dz, av);
                        dz->faceKind = 1;
                        for (gn = 0; gn < fb; gn++) {
                            iv = 0;
                            if ((ev[gn + 1] & 0x80000000) != 0) {
                                Pfile_SetLoadOptionsBlendMode(BLEND_ADDITIVE, (struct PfileLoadOptions *)loadOptions);
                                iv = 1;
                            }
                            j2 = ev[gn + 1] & 0x7FFFFFFF;
                            jb = (char *)a6 + j2;
                            battleLoadDesc[0xD] = gn;
                            ((void **)br->modelSet->partMeshes)[gn] =
                                Lmd_BuildAndRegisterPart((Battle3dPartyLoadDesc *)battleLoadDesc, (int)jb, av, loadOptions, (Battle3dLmdFaceData *)dz, datName);
                        }
                        dj = battleLoadDesc[0xE];
                    }
                    Pfile_DestroyTim(av);
                }
                if (battleLoadDesc[0xA] != 0) {
                    jr = (BattleStageDescriptor2 *)BattleModel_AllocDescriptor(1, 0);
                    if (jr != 0) {
                        jr->hasInfoFile = 1;
                        jr->clearedWord = 0;
                        jr->partCount = fb;
                        jr->subCount = dj;
                        BattleModel_WriteDescriptorFile((Battle3dDescriptorFile *)jr, datName);
                        BattleModel_FreeDescriptorWithBones(jr);
                    }
                }
            }
            Lmd_DestroyTextureAtlas(dz);
        }
        // STRING: FF7 0x008ffbd8
        GameHeapFree(b2, "C:\\FF7\\src\\battle\\battle3d\\stage.cpp", 0x10A);
    }
    Battle_PopDataDirectory((int *)p);
    return (void *)br;
}
// clang-format off
// GLOBAL: FF7 0x008ffb00
float g_BattleStageWhiteRgb[3] = {255.0f, 255.0f, 255.0f};
unsigned char g_rawData_008ffb0c[4] = {0x00, 0x00, 0x7f, 0x43};
// GLOBAL: FF7 0x00c3f680
struct BattleStageFloorFlags g_BattleStageFloorFlagTable = {0};
unsigned char g_bssPad_00c3f6d8[4] = {0};
// GLOBAL: FF7 0x00c3f6dc
int g_BattleStageExcludedPartIndex = 0;
// clang-format on

/*
 * Marks the stage ids that have a floor in the byte table at g_BattleStageFloorFlagTable.
 */
// FUNCTION: FF7 0x005e352b
void __cdecl BattleStage_InitFloorFlagTable(void)
{
    (g_BattleStageFloorFlagTable.stage12Floor) = 1;
    (g_BattleStageFloorFlagTable.partEnabled) = 1;
    (g_BattleStageFloorFlagTable.stage46Floor) = 1;
    (g_BattleStageFloorFlagTable.stage47Floor) = 1;
    (g_BattleStageFloorFlagTable.stage50Floor) = 1;
    (g_BattleStageFloorFlagTable.stage51Floor) = 1;
    ((unsigned char *)&g_BattleStageFloorFlagTable)[1] = 1;
    (g_BattleStageFloorFlagTable.stage44Floor) = 1;
    (g_BattleStageFloorFlagTable.stage45Floor) = 1;
    (g_BattleStageFloorFlagTable.stage0AFloor) = 1;
    (g_BattleStageFloorFlagTable.stage0BFloor) = 1;
    (g_BattleStageFloorFlagTable.stage1BFloor) = 1;
    (g_BattleStageFloorFlagTable.stage1CFloor) = 1;
    (g_BattleStageFloorFlagTable.stage55Floor) = 1;
}

/*
 * Returns g_BattleStageFloorFlagTable[stage]. No stage bound is checked.
 */
// FUNCTION: FF7 0x005e3592
int __cdecl BattleStage_UsesDepthTestedFloor(int stage)
{
    int flag;

    flag = ((unsigned char *)&g_BattleStageFloorFlagTable)[stage];
    return flag;
}

/*
 * Gets the engine context and skips the excluded part index g_BattleStageExcludedPartIndex. For a valid one-based
 * stage part, chooses draw preset 12/13 or 9/10 according to the engine driver type and nonzero RGB; the 13
 * path blends the packed color through Gfx_BlendFloatColorsToBgr. Installs packedColorAlpha into draw options,
 * applies them to the mesh polygon set, converts the installed matrix and adds 10.0f to matrix
 * element thirteen. Submits the transformed mesh, temporarily setting render states fifteen and
 * sixteen to zero then one for first parts whose stage flag is clear.
 */
// FUNCTION: FF7 0x005e35ab
void __cdecl BattleStage_SubmitPartGeometry(unsigned int packedColorAlpha, int partIndex, Battle3dStageRecord *stageObject)
{
    struct {
        float rgba[4];
        float alpha;
        int color;
        PsxMatrix *installedMatrix;
        float matrix[16];
        GfxGroupDrawSpec options;
        GfxDrawSpec *optionsPtr;
        int mesh;
        struct Battle3dStagePartTable *partList;
        struct GfxEngineContext *engine;
    } f; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    f.engine = (struct GfxEngineContext *)((int)GetEngineContext());
    if (partIndex == g_BattleStageExcludedPartIndex)
        return;
    if (stageObject != 0) {
        if ((g_BattleStageFloorFlagTable.partEnabled) == 0)
            BattleStage_InitFloorFlagTable();
        f.partList = (struct Battle3dStagePartTable *)stageObject->modelSet;
        if (f.partList != 0) {
            partIndex--;
            if (partIndex >= 0 && partIndex < *(int *)f.partList) {
                f.optionsPtr = &f.options.spec;
                if (f.engine->driverType == GFX_DRIVER_SOFTWARE) {
                    f.color = packedColorAlpha & 0xffffff;
                    if (f.color != 0) {
                        f.alpha = (float)(*(int *)((char *)&packedColorAlpha + 3) & 255) * (1.0f / 255.0f);
                        f.rgba[0] = (float)((int)packedColorAlpha & 255);
                        f.rgba[1] = (float)(*(int *)((char *)&packedColorAlpha + 1) & 255);
                        f.rgba[2] = (float)(*(int *)((char *)&packedColorAlpha + 2) & 255);
                        f.rgba[3] = 255.0f;
                        Gfx_BlendFloatColorsToBgr(f.alpha, 1.0f - f.alpha, (GfxFloatColor *)f.rgba, (GfxFloatColor *)g_BattleStageWhiteRgb, (int)&packedColorAlpha);
                        Gfx_InitDrawSpec(13, f.optionsPtr);
                    } else
                        Gfx_InitDrawSpec(12, f.optionsPtr);
                } else {
                    if ((packedColorAlpha & 0xffffff) != 0)
                        Gfx_InitDrawSpec(9, f.optionsPtr);
                    else
                        Gfx_InitDrawSpec(10, f.optionsPtr);
                }
                f.optionsPtr->packedColor = packedColorAlpha;
                f.mesh = *(int *)(*(int *)((char *)f.partList + 4) + partIndex * 4);
                if (f.mesh != 0) {
                    Gfx_ApplyDrawSpecToPolygonSet(f.optionsPtr, (GfxPolygonSet *)(*(int *)(f.mesh + 4)));
                    f.installedMatrix = GetInstalledRotMatrixPtr();
                    GtePsxMatrixToFloat4x4Transposed(f.installedMatrix, f.matrix);
                    f.matrix[13] += 10.0f;
                    Gfx_SetModelMatrix((int)f.matrix, f.mesh, (struct GfxEngineContext *)f.engine);
                    if (partIndex == 0 && !BattleStage_UsesDepthTestedFloor(stageObject->stageId)) {
                        Gfx_SetRenderStateCall(RENDER_STATE_DEPTH_TEST, 0, f.engine);
                        Gfx_SetRenderStateCall(RENDER_STATE_DEPTH_WRITE, 0, f.engine);
                    }
                    Gfx_DrawMeshPolygonSet(f.mesh, (int)f.engine);
                    if (partIndex == 0 && !BattleStage_UsesDepthTestedFloor(stageObject->stageId)) {
                        Gfx_SetRenderStateCall(RENDER_STATE_DEPTH_TEST, 1, f.engine);
                        Gfx_SetRenderStateCall(RENDER_STATE_DEPTH_WRITE, 1, f.engine);
                    }
                }
            }
        }
    }
}

/*
 * Returns the mesh for the one-based part number partNumber from the part table of the stage (count
 * first, then the pointer array), or 0 when stage or its list is null or the number is out of
 * range.
 */
// FUNCTION: FF7 0x005e383e
void *__cdecl BattleStage_GetPartMesh(int partNumber, Battle3dStageRecord *stage)
{
    int mesh;

    mesh = 0;
    if (stage != 0 && stage->modelSet != 0) {
        partNumber--;
        if (partNumber >= 0 && partNumber < stage->modelSet->partCount) {
            mesh = stage->modelSet->partMeshes[partNumber];
        }
    }
    return (void *)mesh;
}

/*
 * For a nonnull object with a part list, decrements the one-based part index and checks it against
 * the count; frees a nonnull selected mesh through Pfile_FreeRsdModel with GetEngineContext and
 * clears that entry.
 */
// FUNCTION: FF7 0x005e388d
void __cdecl BattleStage_FreePartMesh(int partNumber, int stage)
{
    int mesh;

    if (stage != 0 && ((Battle3dStageRecord *)stage)->modelSet != 0) {
        partNumber--;
        if (partNumber >= 0 && partNumber < ((Battle3dStageRecord *)stage)->modelSet->partCount) {
            mesh = ((Battle3dStageRecord *)stage)->modelSet->partMeshes[partNumber];
            if (mesh != 0) {
                Pfile_FreeRsdModel((void *)mesh, (void *)GetEngineContext());
                ((Battle3dStageRecord *)stage)->modelSet->partMeshes[partNumber] = 0;
            }
        }
    }
}
} /* extern "C" */
