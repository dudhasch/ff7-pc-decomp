// src/ff7/src/battle/b3ddata.cpp
// Original: C:\FF7\src\Battle\b3ddata.cpp
// Contents: Battle 3D scene data: creates and destroys the render objects of scene and
//   special-effect records, looks up stage descriptors and part counts, loads stage models, and
//   resets the scene's frame pools and draws its layers. It also keeps the effect model bank, the actor draw
//   table and the offscreen render slot queue.
/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/types/battle.h"
#include "ff7/types/battle_3d.h"
#include "ff7/types/menu.h"
#include "lib/types/file.h"
#include "lib/types/graphics.h"
#include "lib/enums/graphics.h"
#include "lib/pfile.h"
#include "ff7/api/battle.h"
#include "lib/api/file.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "lib/api/token.h"
#include "lib/api/mem.h"
#include "lib/api/polygon.h"
#include "ff7/enums/battle.h"
#include <string.h>
#include <stdio.h>
extern "C" {


// clang-format off
// GLOBAL: FF7 0x009adf10
int g_BattleActorDrawSuppressed[10] = {0};
// GLOBAL: FF7 0x009adf38
BattleActorDrawEntry g_BattleActorDrawTable[3] = {0};
// GLOBAL: FF7 0x009adf68
BattleActorDrawEntry g_BattleActor3DrawTable[7] = {0};
// GLOBAL: FF7 0x009adfd8
struct Battle3dScene *g_pBattle3dScene = 0;
unsigned char g_bssPad_009adfdc[4] = {0};
// clang-format on


// Returns one render object of a scene record, chosen by selector 1 to 7: seffect1Object,
// seffect2Object, seffect3Object, batchListA, overlayQuadState, batchListB or quadRecord, in that
// order. Returns 0 for a null record or any other selector.
// FUNCTION: FF7 0x00428500
int __cdecl Battle3d_GetSceneRenderObject(int selector, void *record)
{
    int result;

    result = 0;
    if (record != 0) {
        switch (selector) {
        case BATTLE3D_SCENE_SLOT_SEFFECT1:
            result = (int)(((Battle3dScene *)record)->seffect1Object);
            break;
        case BATTLE3D_SCENE_SLOT_SEFFECT2:
            result = (int)(((Battle3dScene *)record)->seffect2Object);
            break;
        case BATTLE3D_SCENE_SLOT_SEFFECT3:
            result = (int)(((Battle3dScene *)record)->seffect3Object);
            break;
        case BATTLE3D_SCENE_SLOT_BATCH_LIST_A:
            result = ((Battle3dScene *)record)->batchListA;
            break;
        case BATTLE3D_SCENE_SLOT_OVERLAY_QUAD:
            result = ((Battle3dScene *)record)->overlayQuadState;
            break;
        case BATTLE3D_SCENE_SLOT_BATCH_LIST_B:
            result = ((Battle3dScene *)record)->batchListB;
            break;
        case BATTLE3D_SCENE_SLOT_QUAD_RECORD:
            result = ((Battle3dScene *)record)->quadRecord;
        }
    }
    return result;
}

// Returns one of the batch pointers of a scene record as a GfxPsxSpriteQuadState, chosen by
// selector 1 to 7: seffect1Batch, seffect2Batch, seffect3Batch, batchListA, overlayQuadState,
// batchListB or quadRecord. It is the batch counterpart of Battle3d_GetSceneRenderObject. Returns 0 for
// a null record or any other selector.
// FUNCTION: FF7 0x004285a0
int __cdecl Battle3d_GetSceneRenderObjectHigh(int selector, Battle3dScene *record)
{
    GfxPsxSpriteQuadState *result;

    result = 0;
    if (record != 0) {
        switch (selector) {
        case BATTLE3D_SCENE_SLOT_SEFFECT1:
            result = (GfxPsxSpriteQuadState *)(record->seffect1Batch);
            break;
        case BATTLE3D_SCENE_SLOT_SEFFECT2:
            result = (GfxPsxSpriteQuadState *)(record->seffect2Batch);
            break;
        case BATTLE3D_SCENE_SLOT_SEFFECT3:
            result = (GfxPsxSpriteQuadState *)(record->seffect3Batch);
            break;
        case BATTLE3D_SCENE_SLOT_BATCH_LIST_A:
            result = (GfxPsxSpriteQuadState *)(record->batchListA);
            break;
        case BATTLE3D_SCENE_SLOT_OVERLAY_QUAD:
            result = (GfxPsxSpriteQuadState *)(record->overlayQuadState);
            break;
        case BATTLE3D_SCENE_SLOT_BATCH_LIST_B:
            result = (GfxPsxSpriteQuadState *)(record->batchListB);
            break;
        case BATTLE3D_SCENE_SLOT_QUAD_RECORD:
            result = (GfxPsxSpriteQuadState *)(record->quadRecord);
        }
    }
    return (int)result;
}

/*
 * Releases and nulls the fifteen render objects of a scene record.
 */
// FUNCTION: FF7 0x00428640
void __cdecl Battle3d_DestroySceneRenderObjects(int sceneRecord)
{
    if (sceneRecord != 0) {
        DestroyRenderObjectAndClear((void *)(sceneRecord + 0x10));
        DestroyRenderObjectAndClear((void *)(sceneRecord + 0x14));
        DestroyRenderObjectAndClear((void *)(sceneRecord + 0x18));
        DestroyRenderObjectAndClear((void *)(sceneRecord + 0x1C));
        DestroyRenderObjectAndClear((void *)(sceneRecord + 0x20));
        DestroyRenderObjectAndClear((void *)(sceneRecord + 0x24));
        DestroyRenderObjectAndClear((void *)(sceneRecord + 0x28));
        DestroyRenderObjectAndClear((void *)(sceneRecord + 0x2C));
        DestroyRenderObjectAndClear((void *)(sceneRecord + 0x30));
        DestroyRenderObjectAndClear((void *)(sceneRecord + 0x34));
        DestroyRenderObjectAndClear((void *)(sceneRecord + 0x38));
        DestroyRenderObjectAndClear((void *)(sceneRecord + 0x3C));
        DestroyRenderObjectAndClear((void *)(sceneRecord + 0x40));
        DestroyRenderObjectAndClear((void *)(sceneRecord + 0x44));
        DestroyRenderObjectAndClear((void *)(sceneRecord + 0x48));
    }
}

/*
 * Creates the three SEFFECT render objects and the three SEFFECT batch objects of a scene record
 * from its model bank's settings, then loads the font and window textures into it.
 */
// FUNCTION: FF7 0x00428730
void __cdecl Battle3d_CreateSceneRenderObjects(Battle3dScene *sceneRecord)
{
    PfileLoadOptions scratchBlock;
    PfileLoadOptions *optionsRef;
    PfileLoadOptions fontWinDesc;
    GfxModelTableBank *pConfig;

    if (sceneRecord != 0) {
        pConfig = (GfxModelTableBank *)(sceneRecord->modelTableBank);
        if (pConfig != 0) {
            optionsRef = &scratchBlock;
            Pfile_InitLoadOptionsWithBlend(BLEND_ADDITIVE, (PfileLoadOptions *)optionsRef);
            optionsRef->colorKeyEnabled = 1;
            optionsRef->blendFlags |= 1;
            optionsRef->drawPool = pConfig->drawPool;
            optionsRef->directory = (char *)Battle_GetDataDirectory();
            optionsRef->extraFlags |= 1;
            optionsRef->colorKey |= 0xFF242424;
            // STRING: FF7 0x007c2040
            (sceneRecord->seffect1Object) = (GfxRenderObject *)Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_3D, (PfileLoadOptions *)optionsRef, "SEFFECT1.TIM", (void *)pConfig->primitiveMeshBank);
            // STRING: FF7 0x007c2050
            (sceneRecord->seffect2Object) = (GfxRenderObject *)Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_3D, (PfileLoadOptions *)optionsRef, "SEFFECT2.TIM", (void *)pConfig->primitiveMeshBank);
            // STRING: FF7 0x007c2060
            (sceneRecord->seffect3Object) = (GfxRenderObject *)Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_3D, (PfileLoadOptions *)optionsRef, "SEFFECT3.TIM", (void *)pConfig->primitiveMeshBank);
            // STRING: FF7 0x007c2070
            (sceneRecord->seffect1Batch) = (int)Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, (PfileLoadOptions *)optionsRef, "SEFFECT1.TIM", (void *)pConfig->primitiveMeshBank);
            // STRING: FF7 0x007c2080
            (sceneRecord->seffect2Batch) = (int)Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, (PfileLoadOptions *)optionsRef, "SEFFECT2.TIM", (void *)pConfig->primitiveMeshBank);
            // STRING: FF7 0x007c2090
            (sceneRecord->seffect3Batch) = (int)Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, (PfileLoadOptions *)optionsRef, "SEFFECT3.TIM", (void *)pConfig->primitiveMeshBank);
            Pfile_InitLoadOptionsWithBlend(BLEND_NONE, (PfileLoadOptions *)&fontWinDesc);
            fontWinDesc.emitterFlags |= 2;
            fontWinDesc.blendFlags |= 1;
            fontWinDesc.drawPool = pConfig->drawPool;
            fontWinDesc.directory = (char *)BattleFx_GetResourceRootAlt();
            Battle_LoadFontAndWindowTexturesInto((FontWindowHandles *)((int)sceneRecord + 0x28), &fontWinDesc, pConfig->primitiveMeshBank);
        }
    }
}

/*
 * Releases and nulls the six SEFFECT render objects of a record.
 */
// FUNCTION: FF7 0x00428920
void __cdecl Battle3d_DestroySeffectRenderObjects(int seffectRecord)
{
    if (seffectRecord != 0) {
        DestroyRenderObjectAndClear((void *)(seffectRecord + 0x10));
        DestroyRenderObjectAndClear((void *)(seffectRecord + 0x14));
        DestroyRenderObjectAndClear((void *)(seffectRecord + 0x18));
        DestroyRenderObjectAndClear((void *)(seffectRecord + 0x1C));
        DestroyRenderObjectAndClear((void *)(seffectRecord + 0x20));
        DestroyRenderObjectAndClear((void *)(seffectRecord + 0x24));
    }
}

/*
 * Creates the three SEFFECT render objects and the three SEFFECT batch objects of a record from
 * its model bank's settings.
 */
// FUNCTION: FF7 0x00428985
void __cdecl Battle3d_CreateSeffectRenderObjects(Battle3dScene *seffectRecord)
{
    PfileLoadOptions *loadOptions;
    PfileLoadOptions padWord; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    GfxModelTableBank *seffectSource;
    PfileLoadOptions loadBlock;

    if (seffectRecord != 0) {
        seffectSource = (GfxModelTableBank *)(seffectRecord->modelBank);
        if (seffectSource != 0) {
            loadOptions = &loadBlock;
            Pfile_InitLoadOptionsWithBlend(BLEND_ADDITIVE, (PfileLoadOptions *)loadOptions);
            loadOptions->colorKeyEnabled = 1;
            loadOptions->blendFlags |= 1;
            loadOptions->drawPool = seffectSource->drawPool;
            loadOptions->directory = (char *)Battle_GetDataDirectory();
            loadOptions->extraFlags |= 1;
            loadOptions->colorKey |= 0xFF242424;
            // STRING: FF7 0x007c20a0
            (seffectRecord->seffect1Object) = (GfxRenderObject *)Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_3D, (PfileLoadOptions *)loadOptions, "SEFFECT1.TIM", (void *)seffectSource->primitiveMeshBank);
            // STRING: FF7 0x007c20b0
            (seffectRecord->seffect2Object) = (GfxRenderObject *)Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_3D, (PfileLoadOptions *)loadOptions, "SEFFECT2.TIM", (void *)seffectSource->primitiveMeshBank);
            // STRING: FF7 0x007c20c0
            (seffectRecord->seffect3Object) = (GfxRenderObject *)Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_3D, (PfileLoadOptions *)loadOptions, "SEFFECT3.TIM", (void *)seffectSource->primitiveMeshBank);
            // STRING: FF7 0x007c20d0
            (seffectRecord->seffect1Batch) = (int)Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, (PfileLoadOptions *)loadOptions, "SEFFECT1.TIM", (void *)seffectSource->primitiveMeshBank);
            // STRING: FF7 0x007c20e0
            (seffectRecord->seffect2Batch) = (int)Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, (PfileLoadOptions *)loadOptions, "SEFFECT2.TIM", (void *)seffectSource->primitiveMeshBank);
            // STRING: FF7 0x007c20f0
            (seffectRecord->seffect3Batch) = (int)Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, (PfileLoadOptions *)loadOptions, "SEFFECT3.TIM", (void *)seffectSource->primitiveMeshBank);
        }
    }
}

/*
 * Allocates and initializes the Battle3dScene object, loading both shadow models and default banks.
 */
// FUNCTION: FF7 0x00428b12
void *Battle_Init3dData(void)
{
    struct {
        GfxModelTableBank *render;
        int count;
        int config[11];
        int *options;
        Battle3dScene *scene;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    // STRING: FF7 0x007c2100
    frame.scene = (Battle3dScene *)GameHeapAlloc(1, 0x4F4, "C:\\FF7\\src\\Battle\\b3ddata.cpp", 0x12B);
    if (frame.scene != 0) {
        frame.scene->model = (struct PolygonRsdModel *)Battle_LoadShadowModel(0);
        frame.scene->modelRequest = (int)Battle_LoadShadowModel(1);
        // STRING: FF7 0x007c2120
        frame.scene->markModel = (int)Battle_LoadLmdModelByName(0, 6, "MARK.DAT", 0);
        frame.options = frame.config;
        ZeroFillMemory32(0x2C, (void *)frame.options);
        frame.options[0] = 1;
        frame.options[1] = 50;
        frame.options[2] = 100;
        frame.options[3] = 100;
        frame.options[4] = 128;
        frame.options[5] = 1;
        frame.options[6] = 100;
        frame.options[7] = 0;
        if (g_BattleAssetSourceModeSelector != 0) {
            frame.options[8] = 1;
        } else {
            frame.options[8] = 0;
        }
        frame.options[9] = 3;
        frame.options[10] = 0;
        frame.count = 50;
        frame.render = (GfxModelTableBank *)Gfx_CreateModelTableBank(frame.count, (GfxModelTableDesc *)frame.options);
        if (frame.render != 0) {
            frame.scene->modelTableBank = (int)frame.render;
            if (Gfx_PsxCreatePrimitiveSystem(frame.render->drawPool, frame.render->primitiveMeshBank)) { /* empty first arm: lib/codegen.h */
            } else {
                // STRING: FF7 0x007c212c
                Gfx_DebugPrint("BATTLE ERROR: COULD NOT CREATE PRIMITIVE SYSTEM \n");
            }
            Battle3d_CreateSceneRenderObjects(frame.scene);
        } else {
            // STRING: FF7 0x007c2160
            Gfx_DebugPrint("BATTLE ERROR: COULD NOT CREATE RENDER SYSTEM LIST \n");
        }
        g_pBattle3dScene = frame.scene;
        BattleFx_MagicEnterLimitFlash();
        BattleFx_EffectLoadResHitMark();
    }
    Battle3d_ResetActorDrawTable();
    return frame.scene;
}

/*
 * Releases all RSD models, animation banks, and heap allocations owned by a Battle3dScene.
 */
// FUNCTION: FF7 0x00428c99
void __cdecl Battle3d_DestroySceneObject(Battle3dScene *scene)
{
    if (scene != 0) {
        Gfx_PsxFreePrimitiveSystem();
        BattleModel_FreeBank((BattleLimitAnimBank *)(scene->battleModelBank));
        scene->battleModelBank = 0;
        Battle3d_DestroySceneRenderObjects((int)scene);
        Gfx_FreeModelTableBank((GfxModelTableBank *)(scene->modelTableBank));
        scene->modelTableBank = 0;
        Pfile_FreeRsdModel((void *)scene->rsdModelC, (void *)GetEngineContext());
        scene->rsdModelC = 0;
        Pfile_FreeRsdModel((void *)scene->rsdModelB, (void *)GetEngineContext());
        scene->rsdModelB = 0;
        Pfile_FreeRsdModel((void *)scene->rsdModelA, (void *)GetEngineContext());
        scene->rsdModelA = 0;
        BattleStage_FreeStageDatModel((BattleStageDat *)(scene->stageDatModel));
        scene->stageDatModel = 0;
        // STRING: FF7 0x007c2194
        GameHeapFree(scene, "C:\\FF7\\src\\Battle\\b3ddata.cpp", 0x1B1);
    }
}

#pragma intrinsic(memset)

// clang-format off
// GLOBAL: FF7 0x007c1fc8
unsigned char g_battleModelLoadCategories[8] = {0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00};
unsigned char g_rawData_007c1fd0[112] = {
    0x01, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x08, 0x29, 0x5e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
    0x02, 0x00, 0x00, 0x00, 0x08, 0x29, 0x5e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00,
    0x08, 0x29, 0x5e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
// clang-format on


// Distinct C symbol from Battle_SpawnHitReactionTask (0x005BE9F0), which shares the same
// hit-reaction task setup in a separate battle TU.

/*
 * Returns the model pointer of a scene record (the second member), or 0 when record is null.
 */
// FUNCTION: FF7 0x00428d75
int __cdecl Battle3d_GetStageInfoBlockRaw(int record)
{
    BattleStageDescriptor *infoBlock;

    infoBlock = 0;
    if (record != 0) {
        infoBlock = (BattleStageDescriptor *)(*(int *)((char *)record + 4));
    }
    return (int)infoBlock;
}

/*
 * Walks the scene's stage data to its part block and returns the part count at the head of that
 * block. Returns 0 if the scene or either link in the chain is null.
 */
// FUNCTION: FF7 0x00428d96
int __cdecl Battle3d_GetStagePartCount(struct Battle3dScene *stage)
{
    int count;

    count = 0;
    if (stage != 0 && stage->stageId != 0 && *(int *)((char *)*(int *)stage + 8) != 0) {
        count = *(int *)((char *)*(int *)((char *)*(int *)stage + 8));
    }
    return count;
}

/*
 * Returns Battle3d_GetStageInfoBlockRaw of the pointer stored at offset 0 of stage, that is the
 * field at offset 4 of stage's first sub-object, or 0 when stage is null.
 */
// FUNCTION: FF7 0x00428dce
int __cdecl Battle3d_GetStageDescriptor(struct Battle3dScene *stage)
{
    BattleStageDescriptor *descriptor;

    descriptor = 0;
    if (stage != 0) {
        descriptor = (BattleStageDescriptor *)(Battle3d_GetStageInfoBlockRaw(stage->stageId));
    }
    return (int)descriptor;
}

/*
 * Initializes load options with the retained stage flags and a model descriptor, then configures
 * category 1 using forceFlag. When the output pointer outModel is nonzero, formats STAGE%02d.DAT
 * using stageNum. Loads the legacy stage when descriptor offset 0x28 is nonzero or offset 0x2C is
 * zero; when offset 0x2C is nonzero, frees the current output model and replaces it with
 * BattleStage_LoadModelSet. Thus when only the model-set path is selected it frees the value
 * already held by the caller output. Options, descriptor and filename are represented by addresses
 * of adjacent scalar locals in the current reconstruction, with no portable aggregate-layout
 * guarantee.
 */
// FUNCTION: FF7 0x00428df7
void __cdecl Battle_LoadStageModel(int forceFlag, int stageNum, void *outModel)
{
    Battle3dPartyLoadDesc lmdDesc;
    PfileLoadOptions *flagsBlock;
    int stageName[50];
    Battle3dPartyLoadDesc *descriptor;
    PfileLoadOptions optionFlags;

    flagsBlock = &optionFlags;
    Pfile_InitLoadOptions(flagsBlock);
    flagsBlock->flags |= 0x2000000;
    flagsBlock->extraFlags |= 2;
    flagsBlock->extraFlags |= 4;
    flagsBlock->paletteFlags |= 1;
    flagsBlock->paletteFlags |= 2;
    flagsBlock->renderFlags |= 0x8000;
    descriptor = &lmdDesc;
    Battle_InitLmdLoadDesc((int *)descriptor);
    BattleModel_ConfigureLoadCategory(forceFlag, 1, descriptor, flagsBlock);
    if (outModel != 0) {
        // STRING: FF7 0x007c21b4
        sprintf((char *)stageName, "STAGE%02d.DAT", stageNum);
        if (descriptor->secondCount != 0 || descriptor->loadCategory == 0) {
            *(int *)outModel = (int)BattleStage_LoadStageDatLegacy(stageNum, (int *)descriptor, flagsBlock, (char *)stageName);
        }
        if (descriptor->loadCategory != 0) {
            BattleStage_FreeStageDatModel((BattleStageDat *)(*(int *)outModel));
            *(int *)outModel = (int)BattleStage_LoadModelSet(forceFlag, stageNum, descriptor, flagsBlock, (char *)stageName);
        }
    }
}

/*
 * Releases scene offscreen slots, frees and clears seven model-bank entries, clears five shared
 * work regions and eight control globals, destroys the scene object, and clears g_pBattle3dScene.
 */
// FUNCTION: FF7 0x00428f59
void __cdecl Battle_DestroyScene(void)
{
    int i;

    Gfx_ReleaseOffscreenSlots(4, (GfxOffscreenSlot *)(((int)g_pBattle3dScene) + 0x54));
    for (i = 0; i < 7; i++) {
        BattleModel_FreeBank((BattleLimitAnimBank *)(g_BattleModelBankSlots[i]));
        g_BattleModelBankSlots[i] = 0;
    }
    ZeroFillMemory32(0x10D38, &g_BattleActorRecords[0].modelId);
    ZeroFillMemory32(0x488, &g_BattleActorStates[0].status);
    ZeroFillMemory32(0x270, &g_BattleHitDisplayTable[0].battleActorIndex);
    ZeroFillMemory32(0x4E, &g_BattleHitTargetActors);
    ZeroFillMemory32(0xA, &g_BattleSummonSavedDrawFlags);
    g_BattleSecondaryTaskActiveCount = 0;
    g_BattleActiveTaskSlotIdx = 0;
    g_BattleSecondaryResourceCount = 0;
    g_BattleSecondaryTaskActiveSlotIdx = 0;
    g_BattleOverlayTaskActiveCount = 0;
    g_BattleOverlayTaskSlotIdx = 0;
    g_BattleAuxTaskActiveCount = 0;
    g_BattleCameraAuxTaskSlot = 0;
    Battle3d_DestroySceneObject(g_pBattle3dScene);
    g_pBattle3dScene = 0;
}

/*
 * Resets the frame pools of the scene's model table bank. Does nothing without a scene.
 */
// FUNCTION: FF7 0x00429064
void __cdecl Battle_ResetSceneFramePools(void)
{
    GfxModelTableBank *rec;

    if (g_pBattle3dScene != 0) {
        rec = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        Gfx_ModelTableBankResetFramePools(rec);
    }
}

/*
 * Draws one layer of the battle scene, doing nothing when there is no scene. Every layer first
 * draws the model table bank and the display-list group; layer 0 then draws the offscreen slots
 * and six primitive batch lists, layer 1 the mesh polygon set, rain batch, offscreen slots, two
 * batch lists, the three SEFFECT render objects and the three SEFFECT batches; layers 2 and 3 draw nothing more.
 */
// FUNCTION: FF7 0x0042908c
void __cdecl Battle_DrawLayer(int context, int layer)
{
    GfxEngineContext *gfxCtx;
    GfxModelTableBank *modelBank;
    Battle3dScene *scene;

    scene = g_pBattle3dScene;
    if (scene != 0) {
        gfxCtx = GetEngineContext();
        modelBank = (GfxModelTableBank *)(scene->modelBank);
        switch (layer) {
        case GFX_PSX_DISPLAY_GROUP_OPAQUE:
            Gfx_ModelTableBankDrawLayerAndRemoveExpired(context, layer, modelBank);
            Gfx_DrawDisplayListGroup(layer);
            Gfx_DrawOffscreenSlots(0, 4, (GfxOffscreenSlot *)scene->offscreenSlotsRaw);
            Gfx_SetRenderStateCall(RENDER_STATE_LINEAR_FILTER, 0, gfxCtx);
            Gfx_SetRenderStateCall(RENDER_STATE_NO_CULL, 1, gfxCtx);
            Gfx_EmitPrimitiveBatchList((void *)scene->batchListA, gfxCtx);
            Gfx_EmitPrimitiveBatchList((void *)scene->overlayQuadState, gfxCtx);
            Gfx_EmitPrimitiveBatchList((void *)scene->batchListB, gfxCtx);
            Gfx_EmitPrimitiveBatchList((void *)scene->quadRecord, gfxCtx);
            Gfx_EmitPrimitiveBatchList((void *)scene->batchListC, gfxCtx);
            Gfx_EmitPrimitiveBatchList((void *)scene->batchListD, gfxCtx);
            Gfx_SetRenderStateCall(RENDER_STATE_LINEAR_FILTER, 1, gfxCtx);
            Gfx_SetRenderStateCall(RENDER_STATE_NO_CULL, 0, gfxCtx);
            break;
        case GFX_PSX_DISPLAY_GROUP_MAIN:
            Gfx_ModelTableBankDrawLayerAndRemoveExpired(context, layer, modelBank);
            Gfx_DrawMeshPolygonSet(scene->meshPolygonSet, (int)gfxCtx);
            Gfx_DrawDisplayListGroup(layer);
            Gfx_EmitPrimitiveBatchList((void *)scene->rainBatch, gfxCtx);
            Gfx_DrawOffscreenSlots(1, 4, (GfxOffscreenSlot *)scene->offscreenSlotsRaw);
            Gfx_SetRenderStateCall(RENDER_STATE_NO_CULL, 1, gfxCtx);
            Gfx_EmitPrimitiveBatchList((void *)scene->batchListE, gfxCtx);
            Gfx_EmitPrimitiveBatchList((void *)scene->batchListF, gfxCtx);
            Gfx_DrawRenderObjectCall(scene->seffect1Object, gfxCtx);
            Gfx_DrawRenderObjectCall(scene->seffect2Object, gfxCtx);
            Gfx_DrawRenderObjectCall(scene->seffect3Object, gfxCtx);
            Gfx_EmitPrimitiveBatchList((void *)scene->seffect1Batch, gfxCtx);
            Gfx_EmitPrimitiveBatchList((void *)scene->seffect2Batch, gfxCtx);
            Gfx_EmitPrimitiveBatchList((void *)scene->seffect3Batch, gfxCtx);
            Gfx_SetRenderStateCall(RENDER_STATE_NO_CULL, 0, gfxCtx);
            break;
        case GFX_PSX_DISPLAY_GROUP_OVERLAY_A:
            Gfx_ModelTableBankDrawLayerAndRemoveExpired(context, layer, modelBank);
            Gfx_DrawDisplayListGroup(layer);
            break;
        case GFX_PSX_DISPLAY_GROUP_OVERLAY_B:
            Gfx_ModelTableBankDrawLayerAndRemoveExpired(context, layer, modelBank);
            Gfx_DrawDisplayListGroup(layer);
        }
    }
}

/*
 * Removes the scene's model tables that have group tag 0. Does nothing without a scene.
 */
// FUNCTION: FF7 0x00429322
void __cdecl Battle_EffectFreeGroupTag0(void)
{
    if (g_pBattle3dScene != 0) {
        Gfx_ModelTableBankRemoveTablesByGroupTag(0, (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank));
    }
}

/*
 * Converts a fade level to an alpha byte: shifts fadeLevel right by 4 and returns 0xFF minus that, or 0
 * when the shifted value has reached 0x100 or more. So the 12-bit fade level 0 gives 255 and 0xFF0
 * gives 1.
 */
// FUNCTION: FF7 0x00429343
int __cdecl Battle3d_FadeLevelToAlpha(int fadeLevel)
{
    int shifted;

    shifted = fadeLevel >> 4;
    if (shifted >= 0x100) {
        shifted = 0;
    } else {
        shifted = 0xFF - shifted;
    }
    return shifted;
}

/*
 * Frees the model bank kept in an effect record and clears it. Does nothing when effect is null.
 */
// FUNCTION: FF7 0x00429374
void __cdecl Battle3d_FreeEffectModelBank(int effect)
{
    if (effect != 0) {
        BattleModel_FreeBank((BattleLimitAnimBank *)(*(int *)((char *)effect + 0x50)));
        *(int *)((char *)effect + 0x50) = 0;
    }
}

/*
 * Stores a model bank in an effect record after freeing the bank it held. Does nothing when effect
 * is null; note that the record is the second parameter.
 */
// FUNCTION: FF7 0x00429398
void __cdecl Battle3d_SetEffectModelBank(BattleLimitAnimBank *bank, int effect)
{
    if (effect != 0) {
        Battle3d_FreeEffectModelBank(effect);
        *(int *)((char *)effect + 0x50) = (int)bank;
    }
}

/*
 * For attachment index -1, copies the actor 0x40-stride matrix from
 * g_BattleActorTransformRecords[0] + 8 into outMatrix; otherwise converts the matrix at
 * g_BattleActorRecords[actorIndex].worldMatrix[0x40]+attachIndex*0x40 through
 * GteFloat4x4TransposedToPsxMatrix.
 */
// FUNCTION: FF7 0x004293b8
void __cdecl Battle3d_GetActorAttachMatrix(int actorIndex, int attachIndex, PsxMatrix *outMatrix)
{
    int srcMatrix;
    if (attachIndex == -1) {
        *(PsxMatrix *)outMatrix = *(PsxMatrix *)((actorIndex << 6) + (char *)g_BattleActorTransformRecords[0].auxNode);
    } else {
        srcMatrix = (int)g_BattleActorRecords[0].attachMatrices + actorIndex * 0x1AEC + (attachIndex << 6);
        GteFloat4x4TransposedToPsxMatrix(outMatrix, (float *)srcMatrix);
    }
}

/*
 * Uses an actor bank and selector plus eight or 0x34 to choose one of two bounds-checked pointer
 * tables; when an entry exists reads a nested float indexed by the actor animation field, scales
 * it by 11.377778053283691, and returns the integer conversion, otherwise zero.
 */
// FUNCTION: FF7 0x0042940d
int __cdecl Battle_GetActorAnimAngle(int actorIndex)
{
    int entryIndex;
    int res;
    int selector;
    BattleActorAnimTable *animTable;
    int idxBias;
    int entry;
    int floatBits;

    res = 0;
    selector = g_BattleActorRecords[actorIndex].animBank;
    animTable = (BattleActorAnimTable *)(g_BattleActorRecords[actorIndex].model);
    if (animTable != 0) {
        entry = 0;
        if (g_BattleActorRecords[actorIndex].useSecondAngleTable != 0) {
            idxBias = 8;
        } else {
            idxBias = 0x34;
        }
        entryIndex = g_BattleActorRecords[actorIndex].animFrame + idxBias;
        if (g_BattleActorRecords[actorIndex].useSecondAngleTable != 0) {
            if (entryIndex >= 0 && entryIndex < animTable->entryCountB) {
                entry = *(int *)((char *)animTable->entriesB + entryIndex * 4);
            }
        } else if (entryIndex >= 0 && entryIndex < animTable->entryCount) {
            entry = *(int *)((char *)animTable->entries + entryIndex * 4);
        }
        if (entry != 0) {
            floatBits = *(int *)((char *)*(int *)((char *)*(int *)((char *)entry + 0x14) + g_BattleActorRecords[actorIndex].animBank * 8) + 8);
            res = (int)(*(float *)&floatBits * 11.377778053283691f);
        }
    }
    return res;
}

/*
 * Calls ZeroFillMemory32(count << 4, table), zeroing count entries of 16 bytes each at address table.
 * The shift is the only work it does.
 */
// FUNCTION: FF7 0x00429516
void __cdecl Battle3d_ClearDrawTable(int count, BattleActorDrawEntry *table)
{
    ZeroFillMemory32(count << 4, table);
}

/*
 * Draws count consecutive entries of the 16-byte-stride actor draw table at drawTable, starting at
 * actor index startIndex, for the pass selected by pass. First scans the three 0x74-byte records
 * based at g_BattleActorStates[0].flags and sets a flag if bit 0 of any of their leading shorts is
 * set; that flag only chooses the iteration direction, back to front when set and front to back
 * when clear, the two loop bodies being otherwise identical. An entry is drawn when its actor index
 * is not 3, its field 0 is non-null, the suppression flag
 * g_BattleActorDrawSuppressed[index] is zero, and its field 0xC equals pass. For each
 * such entry it calls Gfx_ApplyPresetStateCall(1, GetEngineContext()) when pass is non-zero, then
 * Battle_DrawActor(index), and when field 8 is non-zero repeats the preset call and dispatches to
 * Battle3d_DrawActorSubModel if field 4 is set or Battle3d_DrawActorWeapon otherwise.
 */
// FUNCTION: FF7 0x0042952e
void __cdecl Battle_DrawActorRange(int pass, int startIndex, int count, BattleActorDrawEntry *drawTable)
{
    int reverse;
    BattleActorDrawEntry *entry;
    int i;

    reverse = 0;
    for (i = 0; i < 3; i++) {
        if ((g_BattleActorStates[i].flags & 1) != 0) {
            reverse = 1;
        }
    }
    if (reverse != 0) {
        for (i = count - 1; i >= 0; i--) {
            entry = (BattleActorDrawEntry *)((int)drawTable + (i << 4));
            if (startIndex + i != 3 && entry->queued != 0 && g_BattleActorDrawSuppressed[startIndex + i] == 0 && pass == entry->pass) {
                if (pass != 0) {
                    Gfx_ApplyPresetStateCall(1, GetEngineContext());
                }
                Battle_DrawActor(startIndex + i);
                if (entry->hasAttachment != 0) {
                    if (pass != 0) {
                        Gfx_ApplyPresetStateCall(1, GetEngineContext());
                    }
                    if (entry->drawParam != 0) {
                        Battle3d_DrawActorSubModel(startIndex + i);
                    } else {
                        Battle3d_DrawActorWeapon(startIndex + i);
                    }
                }
            }
        }
    } else {
        for (i = 0; i < count; i++) {
            entry = (BattleActorDrawEntry *)((int)drawTable + (i << 4));
            if (startIndex + i != 3 && entry->queued != 0 && g_BattleActorDrawSuppressed[startIndex + i] == 0 && pass == entry->pass) {
                if (pass != 0) {
                    Gfx_ApplyPresetStateCall(1, GetEngineContext());
                }
                Battle_DrawActor(startIndex + i);
                if (entry->hasAttachment != 0) {
                    if (pass != 0) {
                        Gfx_ApplyPresetStateCall(1, GetEngineContext());
                    }
                    if (entry->drawParam != 0) {
                        Battle3d_DrawActorSubModel(startIndex + i);
                    } else {
                        Battle3d_DrawActorWeapon(startIndex + i);
                    }
                }
            }
        }
    }
}

/*
 * The same actor-draw walk as Battle_DrawActorRange but always front to back, with no weapon or
 * sub-model pass, and with the suppression flag g_BattleActorDrawSuppressed[startIndex + p] overridable by
 * g_BattleIsPaused being non-zero. Note the record pointer entry is never advanced in the loop, so all
 * count iterations test the same entry's fields 0 and 0xC while the actor index startIndex + p does
 * advance. Draws each accepted actor with Battle_DrawActor, preceded by
 * Gfx_ApplyPresetStateCall(1, GetEngineContext()) when pass is non-zero.
 */
// FUNCTION: FF7 0x0042972d
void __cdecl Battle3d_DrawActorRangeUnsuppressed(int pass, int startIndex, int count, BattleActorDrawEntry *entry)
{
    int i;

    for (i = 0; i < count; i++) {
        if (entry->queued != 0 && (g_BattleActorDrawSuppressed[startIndex + i] == 0 || g_BattleIsPaused != 0)) {
            if (pass == entry->pass) {
                if (pass != 0) {
                    Gfx_ApplyPresetStateCall(1, GetEngineContext());
                }
                Battle_DrawActor(startIndex + i);
            }
        }
    }
}

/*
 * Zeroes the ten-entry actor draw table at g_BattleActorDrawTable by calling Battle3d_ClearDrawTable(0xA,
 * &g_BattleActorDrawTable), that is 10 records of 16 bytes.
 */
// FUNCTION: FF7 0x004297a5
void __cdecl Battle3d_ResetActorDrawTable(void)
{
    Battle3d_ClearDrawTable(0xA, (BattleActorDrawEntry *)&g_BattleActorDrawTable);
}

/*
 * Calls Battle_DrawActorRange(pass, 0, 0xA, &g_BattleActorDrawTable), running the full ten-entry actor draw
 * table from index 0 for the pass pass.
 */
// FUNCTION: FF7 0x004297b9
void __cdecl Battle3d_SweepActorDrawTable(int pass)
{
    Battle_DrawActorRange(pass, 0, 0xA, (BattleActorDrawEntry *)&g_BattleActorDrawTable);
}

/*
 * Calls Battle3d_DrawActorRangeUnsuppressed(pass, 3, 1, &g_BattleActor3DrawTable), drawing the single table
 * entry at g_BattleActor3DrawTable as actor index 3 for the pass pass.
 */
// FUNCTION: FF7 0x004297d3
void __cdecl Battle_DrawHudSprites(int pass)
{
    Battle3d_DrawActorRangeUnsuppressed(pass, 3, 1, (BattleActorDrawEntry *)&g_BattleActor3DrawTable);
}

/*
 * Returns the address of entry index in the actor draw table at g_BattleActorDrawTable, computed as (index <<
 * 4) + &g_BattleActorDrawTable. Unchecked; the stride is 16 bytes.
 */
// FUNCTION: FF7 0x004297ed
int __cdecl Battle3d_GetActorDrawEntry(int index)
{
    return (index << 4) + (int)&g_BattleActorDrawTable;
}

/*
 * Clears the actor queued flag, sets draw-entry enabled from enabled, and when enabled stores drawParam
 * and two actor flag bits. Returns early for the second flag, otherwise draws actor three through
 * the unsuppressed path or another actor through the ordinary path and marks it queued.
 */
// FUNCTION: FF7 0x004297fd
void __cdecl Battle3d_QueueActorDraw(int enabled, int drawParam, int actorIndex)
{
    BattleActorDrawEntry *entry;

    g_BattleActorDrawSuppressed[actorIndex] = 0;
    entry = (BattleActorDrawEntry *)Battle3d_GetActorDrawEntry(actorIndex);
    if (enabled != 0) {
        entry->queued = 1;
    } else {
        entry->queued = 0;
    }
    if (entry->queued != 0) {
        entry->drawParam = drawParam;
        if ((g_BattleActorRecords[actorIndex].modelFlags & 0x80) != 0) {
            entry->hasAttachment = 1;
        } else {
            entry->hasAttachment = 0;
        }
        if ((g_BattleActorRecords[actorIndex].fadeFlags & 8) != 0) {
            entry->pass = 1;
        } else {
            entry->pass = 0;
        }
        if (entry->pass != 0) {
            return;
        }
        if (actorIndex == 3) {
            Battle3d_DrawActorRangeUnsuppressed(0, actorIndex, 1, entry);
        } else {
            Battle_DrawActorRange(0, actorIndex, 1, entry);
        }
        g_BattleActorDrawSuppressed[actorIndex] = 1;
    }
}

/*
 * Releases offscreen slot slot (0 to 3) of the battle scene. Does nothing without a scene or for
 * an out-of-range slot.
 */
// FUNCTION: FF7 0x004298f8
void __cdecl Battle3d_ReleaseOffscreenSlot(int slot)
{
    if (g_pBattle3dScene != 0 && slot >= 0 && slot < 4) {
        Gfx_ReleaseOffscreenSlot((GfxOffscreenSlot *)(((int)g_pBattle3dScene) + slot * 0x124 + 0x54));
    }
}

// Takes one of the four offscreen slots at g_pBattle3dScene+0x54 and, on success, returns its
// request block at slot+0xC. Gfx_InitOffscreenRequest initialises the block first. Then
// renderObjectBank is copied from the scene's model bank, drawPool is copied from the model bank's
// drawPool, and spriteHandle is set from the slot's requestId. Returns 0 when g_pBattle3dScene is null
// or the allocation fails.
// FUNCTION: FF7 0x0042992e
int __cdecl Battle3d_AcquireOffscreenRequest(void)
{
    GfxOffscreenRequest *request;
    GfxOffscreenSlot *slot;

    request = 0;
    if (g_pBattle3dScene != 0) {
        slot = (GfxOffscreenSlot *)Gfx_AllocOffscreenSlot(4, (GfxOffscreenSlot *)(((int)g_pBattle3dScene) + 0x54));
        if (slot != 0) {
            request = (GfxOffscreenRequest *)((int)slot + 0xC);
            Gfx_InitOffscreenRequest((GfxOffscreenRequest *)request);
            request->renderObjectBank = ((GfxModelTableBank *)(g_pBattle3dScene->modelBank))->primitiveMeshBank;
            request->drawPool = ((GfxModelTableBank *)(g_pBattle3dScene->modelBank))->drawPool;
            request->slotIndex = slot->requestId;
        }
    }
    return (int)request;
}

/*
 * Returns the render object of offscreen slot slot (0 to 3) of the battle scene, or 0 without a
 * scene or for an out-of-range slot.
 */
// FUNCTION: FF7 0x004299b4
int __cdecl Battle3d_GetOffscreenRenderObject(int slot)
{
    GfxPsxSpriteQuadState *renderObj;

    renderObj = 0;
    if (g_pBattle3dScene != 0 && slot >= 0 && slot < 4) {
        renderObj = (GfxPsxSpriteQuadState *)(*(int *)((char *)g_pBattle3dScene + slot * 0x124 + 0x5C));
    }
    return (int)renderObj;
}

/*
 * Services the four offscreen slots of the battle scene. Does nothing without a scene.
 */
// FUNCTION: FF7 0x004299f1
void __cdecl Battle3d_ServiceOffscreenSlots(void)
{
    if (g_pBattle3dScene != 0) {
        Gfx_ServiceOffscreenSlots(4, (GfxOffscreenSlot *)(((int)g_pBattle3dScene) + 0x54));
    }
}

// Copies the load-category record for category out of g_battleModelLoadCategories into two
// descriptors. The record's secondCount goes to loadDesc, forced to 1 when forceFlag is non-zero,
// and loadCategory follows it. For the enemy and party categories (and 1, which no caller passes)
// the record's fromArchive goes to modelDesc's file request. For the limit, effect-model and summon
// categories modelDesc takes g_BattleAssetSourceModeSelector instead, and other categories leave it
// untouched. The archive id and the name callback follow.
// FUNCTION: FF7 0x00429a12
void __cdecl BattleModel_ConfigureLoadCategory(int forceFlag, int category, Battle3dPartyLoadDesc *loadDesc, PfileLoadOptions *modelDesc)
{
    BattleModelLoadCategory *record;

    record = (BattleModelLoadCategory *)(category * 0x14 + (char *)&g_battleModelLoadCategories);
    loadDesc->secondCount = record->secondCount;
    if (forceFlag != 0) {
        loadDesc->secondCount = 1;
    }
    loadDesc->loadCategory = record->loadCategory;
    switch (category) {
    case BATTLE_MODEL_LOAD_ENEMY:
    case BATTLE_MODEL_LOAD_UNUSED_1:
    case BATTLE_MODEL_LOAD_PARTY:
        modelDesc->file.fromArchive = record->fromArchive;
        break;
    case BATTLE_MODEL_LOAD_LIMIT:
    case BATTLE_MODEL_LOAD_EFFECT_MODEL:
    case BATTLE_MODEL_LOAD_SUMMON:
        modelDesc->file.fromArchive = g_BattleAssetSourceModeSelector;
    }
    modelDesc->file.archiveId = record->archiveId;
    modelDesc->file.callback = record->callback;
}
} /* extern "C" */
