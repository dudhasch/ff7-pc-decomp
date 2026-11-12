// src/ff7/src/battle/battle_effect_model_fade_draw.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Loads effect model category 4 (null when no load ID is given) and draws a model with a
//   fade through a draw specification.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/battle.h"
#include "ff7/gte.h"
#include "lib/crt.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_fx.h"
#include "lib/types/polygon.h"
#include "lib/types/graphics.h"

#include "lib/api/graphics.h"
#include "ff7/api/battle.h"
#include "lib/api/polygon.h"
#include "lib/pfile.h"
#include "lib/codegen.h"
extern "C" {

/*
 * Returns null for a null loadId. Otherwise sets up a load descriptor and load options for
 * category 4 and groupIndex, enables the descriptor and applies blendMode. Loads the legacy enemy
 * bank when the descriptor's secondCount is nonzero or its loadCategory is zero. When loadCategory
 * is nonzero it also loads a type-1 descriptor through the options' file request; on success that
 * bank replaces (and frees) the legacy one and the descriptor is freed. Returns the bank.
 */
// FUNCTION: FF7 0x005d3650
void *__cdecl Battle_LoadEffectModelCategory4(int groupIndex, unsigned int blendMode, int loadId)
{
    Battle3dPartyLoadDesc descSpace;
    BattleLimitAnimBank *pBank;
    Battle3dPartyLoadDesc *desc;
    int descHandle;
    PfileLoadOptions optionsBuffer;
    PfileLoadOptions *loadOptions;

    pBank = 0;
    if (loadId != 0) {
        desc = &descSpace;
        loadOptions = &optionsBuffer;
        Battle_InitLmdLoadDesc((int *)desc);
        Pfile_InitLoadOptions(loadOptions);
        BattleModel_ConfigureLoadCategory(groupIndex, 4, desc, loadOptions);
        desc->enabled = 1;
        Pfile_SetLoadOptionsBlendMode(blendMode, loadOptions);
        loadOptions->flags |= 0x100000;
        loadOptions->flags |= 0x2000;
        loadOptions->flags |= 0x4000;
        loadOptions->flags |= 0x800;
        loadOptions->flags |= 0x2000000;
        if (desc->secondCount != 0 || desc->loadCategory == 0) {
            pBank = (BattleLimitAnimBank *)(BattleModel_LoadEnemyDatLegacy((Battle3dPartyLoadDesc *)desc, loadOptions, (char *)loadId));
        }
        if (desc->loadCategory != 0) {
            descHandle = (int)BattleModel_LoadDescriptor(1, (void *)&loadOptions->file, (char *)loadId);
            if (descHandle != 0) {
                BattleModel_FreeBank(pBank);
                pBank = (BattleLimitAnimBank *)(BattleModel_LoadBank(1, 0, (Battle3dPartyLoadDesc *)descHandle, desc, loadOptions, (char *)loadId));
                BattleModel_FreeDescriptor(0, (void *)descHandle);
            }
        }
    }
    return pBank;
}

/*
 * When the bank has a skeleton, builds a draw specification from the current packed draw color and
 * the alpha for fadeLevel (kind 9) and applies it to the skeleton. Points the bank's draw state at
 * actor 3's bone position and world matrix, installs transform on the engine context, and draws
 * frame animFrame when drawing is enabled.
 */
// FUNCTION: FF7 0x005d37d7
void __cdecl BattleFx_DrawModelWithFade(int fadeLevel, PolygonTransform *transform, int animFrame, BattleModelBank *modelBank)
{
    int flags;
    int alpha;
    PolygonModel *model;
    BattleFxModelFadeWrapper *wrapperSub;
    int actorIdx;
    GfxDrawSpec *drawSpec;
    GfxGroupDrawSpec drawSpecBuf;
    int packedColor;
    int stepLeadCount;

    actorIdx = 3;
    flags = 0;
    stepLeadCount = 0;
    if (modelBank != 0) {
        model = (PolygonModel *)modelBank->skeleton;
        if (model != 0) {
            drawSpec = &drawSpecBuf.spec;
            Gfx_InitDrawSpec(0, (GfxDrawSpec *)drawSpec);
            packedColor = GetDrawColorPacked();
            drawSpec->packedColor = packedColor;
            alpha = Battle3d_FadeLevelToAlpha(fadeLevel);
            drawSpec->alpha = alpha;
            drawSpec->kind = 9;
            Gfx_ApplyDrawSpecToModel(drawSpec, model);
            wrapperSub = (BattleFxModelFadeWrapper *)&modelBank->drawState;
            wrapperSub->boneWorldPosPtr = (int)&g_BattleActorRecords[actorIdx].boneWorldPosX;
            wrapperSub->worldMatrixPtr = (int)&g_BattleActorRecords[actorIdx].worldMatrix[0x40];
            Gfx_SetContextMatrix(transform, GetEngineContext());
            if (Gfx_IsDrawEnabled() != 0) {
                BattleModel_DrawAnimFrame(flags, stepLeadCount, animFrame, modelBank);
            }
        }
    }
}
} /* extern "C" */
