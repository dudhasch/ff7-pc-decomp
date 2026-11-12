// src/ff7/src/battle/battle_fx_magic_break.cpp
/* Magic Effect [48]: Break (Japanese: ブレイク, dev tag: "MAGIC/BREAK") */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle visual effect for the Break magic: a timeline task phases in growing, staged and
//   black models with sound effects for each target.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/enums/sound.h"
#include "ff7/battle_tasks.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_magic.h"
#include "lib/types/graphics.h"
#include <stdlib.h>
#include "ff7/camera_script.h"
#include <string.h>
#include "ff7/api/battle.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "lib/codegen.h"
extern "C" {

struct BattleFxModelDraw;

#pragma intrinsic(abs)

// clang-format off
// GLOBAL: FF7 0x008c9878
int g_BattleMagicBreak_ResourceDef = 0x01F400B1;
char *g_BattleMagicBreak_Wind1Path[1] = {
    // STRING: FF7 0x008c9938
    "WIND_1.RSD"
};
unsigned int g_BattleMagicBreak_ModelRecords[18] = {
    0x00000000,
    0x00000000,
    0x00000000,
    0x01f400b4,
    // STRING: FF7 0x008c9944
    (unsigned int)"BREAK_1.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x01f400b1,
    // STRING: FF7 0x008c9950
    (unsigned int)"BREAK_1.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x006400b1,
    // STRING: FF7 0x008c995c
    (unsigned int)"WIND_2.RSD",
    0x00000000,
    0x00000000,
    0x00000000
};
// GLOBAL: FF7 0x008c98c8
unsigned char g_BattleMagicBreak_ModelData1[28] = {
    0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00
};
unsigned char g_rawData_008c98e4[4] = {0x20, 0x00, 0x00, 0x00};
// clang-format on

// clang-format off
// GLOBAL: FF7 0x008c98e8
unsigned char g_BattleMagicBreak_ModelData2[32] = {
    0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
// GLOBAL: FF7 0x008c9908
short g_BattleMagicBreak_BlackMatrix = 4096;
unsigned char g_BattleMagicBreak_BlackMatrixM11[8] = {0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// clang-format on

// clang-format off
// GLOBAL: FF7 0x008c9918
struct BattleFxMagicBreakModelData g_BattleMagicBreak_ModelData3 = {{0, 0x10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 0, {0x88, 0, 0, 0, 0, 0}, 0};
unsigned char g_rawData_008c9934[4] = {0x20, 0x00, 0x00, 0x00};
// clang-format on

#pragma intrinsic(memset)
#pragma intrinsic(memcpy)


// clang-format off
// GLOBAL: FF7 0x00bd5ad8
int g_BattleMagicBreak_DrawHandle = 0;
// GLOBAL: FF7 0x00bd5adc
struct GfxTableCursor *g_BattleMagicBreak_ModelTable = 0;
// GLOBAL: FF7 0x00bd5ae0
struct SVECTOR g_BattleMagicBreak_RotAngles = {0};
// clang-format on

/*
 * Loads resources for the Break magic effect when not already loaded.
 */
// FUNCTION: FF7 0x005a1d60
void __cdecl BattleFx_MagicLoadResBreak(void)
{
    char pathBuf[200];
    GfxResourceRequest requestStorage;
    int kind;
    GfxModelTableBank *gfxCtx;
    GfxResourceRequest *requestPtr;
    int def;

    if (g_pBattle3dScene != 0 && g_BattleMagicBreak_ModelTable == 0) {
        kind = 4;
        def = (int)&g_BattleMagicBreak_ResourceDef;
        gfxCtx = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        requestPtr = &requestStorage;
        Gfx_InitResourceRequest(requestPtr);
        requestPtr->groupTag = 0;
        // STRING: FF7 0x008c9968
        requestPtr->resourcePath = (char *)BattleFx_BuildResourcePath("MAGIC/BREAK", pathBuf);
        Gfx_LoadModelTableIfAbsent(kind, def, requestPtr, &g_BattleMagicBreak_ModelTable, gfxCtx);
    }
}

/*
 * Entry point for Break magic effect initializing execution.
 */
// FUNCTION: FF7 0x005a1df0
void __cdecl BattleFx_MagicEnterBreak(int targetMask, int actorIdx)
{
    BattleFx_MagicLoadResBreak();
    Battle_CreateBitPatternCallbackTask(targetMask, actorIdx, 0, BattleFx_MagicSpawnBreakPerTarget);
    Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputePanForEntity(targetMask), 0x55);
}

/*
 * Spawns Break timeline task for a single target.
 */
// FUNCTION: FF7 0x005a1e2a
void __cdecl BattleFx_MagicSpawnBreakPerTarget(int actor)
{
    BattleFxMagicBreakTask *task;

    task = (BattleFxMagicBreakTask *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicBreakTimelineTask);
    task->ownerSlot = actor;
    task->size = Battle_GetActorScaledEffectSize(actor) / 2;
    Battle_GetBoneWorldPos(actor, g_BattleActorRecords[actor].targetBone, &task->posX);
}

/*
 * Timeline task managing phased model spawning and sound effects for Break.
 */
// FUNCTION: FF7 0x005a1e93
void __cdecl BattleFx_MagicBreakTimelineTask(void)
{
    BattleFxMagicBreakTask *task;
    BattleFxMagicBreakTask *child;

    task = (BattleFxMagicBreakTask *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (task->frame == 0) {
        child = (BattleFxMagicBreakTask *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicBreakGrowingModelFadeTask);
        child->posX = task->posX;
        child->posY = 0;
        child->posZ = task->posZ;
        child->size = task->size;
    }
    if (task->frame == 0xF) {
        child = (BattleFxMagicBreakTask *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicBreakStagedModelFadeTask);
        child->posX = task->posX;
        child->posY = 0;
        child->posZ = task->posZ;
        child->size = task->size;
    }
    if (task->frame == 0x14) {
        child = (BattleFxMagicBreakTask *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicBreakBlackModelGrowTask);
        child->posX = task->posX;
        child->posY = 0;
        child->posZ = task->posZ;
        child->size = task->size;
    }
    if (task->frame == 0x14) {
        Battle_PostActorHitOrHurtMessage(task->ownerSlot);
    }
    if (task->frame == 0x14) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    (task->frame)++;
}

/*
 * Renders and animates the growing fading model effect for Break.
 */
// FUNCTION: FF7 0x005a2010
void __cdecl BattleFx_MagicBreakGrowingModelFadeTask(void)
{
    BattleFxFixedScaleVec *pScales;
    int localFrame;
    BattleFxMagicBreakTask *task;
    PsxMatrix *matrix;

    task = (BattleFxMagicBreakTask *)BATTLE_CURRENT_TASK_SLOT;
    ((BattleFxModelDraw *)&g_BattleMagicBreak_ModelData1)->modelGroup = Gfx_ModelTableGetRenderObject(0, g_BattleMagicBreak_ModelTable);
    matrix = (PsxMatrix *)Gfx_GetPsxScratchpadPtr(0);
    pScales = (BattleFxFixedScaleVec *)(matrix + 1);
    pScales->scaleZ = (task->frame * 0x350 / 0x19 + 0x100) * task->size >> 0xC;
    pScales->scaleX = pScales->scaleZ;
    pScales->scaleY = task->frame * 0x380 / 0x19 * task->size >> 0xC;
    if (pScales->scaleZ > 0x7FFF) {
        pScales->scaleZ = 0x7FFF;
    }
    if (pScales->scaleY > 0x7FFF) {
        pScales->scaleY = 0x7FFF;
    }
    localFrame = task->frame;
    if (localFrame < 0xF) {
        ((BattleFxModelDraw *)&g_BattleMagicBreak_ModelData1)->fadeLevel = 0x80;
    } else {
        localFrame -= 0xF;
        if (localFrame < 0xA) {
            ((BattleFxModelDraw *)&g_BattleMagicBreak_ModelData1)->fadeLevel = localFrame * -0x80 / 0xA + 0x80;
        } else {
            task->ownerSlot = BATTLE_TASK_FREE;
            return;
        }
    }
    RotMatrix(&g_BattleMagicBreak_RotAngles, (int *)matrix);
    matrix->t[0] = task->posX;
    matrix->t[1] = task->posY;
    matrix->t[2] = task->posZ;
    ScaleMatrix(matrix, pScales);
    MultiplyMatrix(&g_BattleCameraTransform, matrix, matrix);
    SetRotMatrix(matrix);
    SetTransMatrix(matrix);
    g_BattleMagicBreak_DrawHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleMagicBreak_ModelData1, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleMagicBreak_DrawHandle);
    if (g_BattleIsPaused == 0) {
        g_BattleMagicBreak_RotAngles.vy -= 0xC0;
        (task->frame)++;
    }
}

/*
 * Renders and animates the staged model fade-out effect for Break.
 */
// FUNCTION: FF7 0x005a2200
void __cdecl BattleFx_MagicBreakStagedModelFadeTask(void)
{
    BattleFxFixedScaleVec *scaleVecPtr;
    int value;
    BattleFxMagicBreakTask *faderPtr;
    int stage;
    PsxMatrix *mtx;
    int clamped;

    faderPtr = (BattleFxMagicBreakTask *)BATTLE_CURRENT_TASK_SLOT;
    mtx = (PsxMatrix *)Gfx_GetPsxScratchpadPtr(0);
    scaleVecPtr = (BattleFxFixedScaleVec *)(mtx + 1);
    stage = faderPtr->frame;
    if (stage < 5) {
        ((BattleFxModelDraw *)&g_BattleMagicBreak_ModelData1)->tintLevel = 0x88;
        ((BattleFxModelDraw *)&g_BattleMagicBreak_ModelData1)->blendFraction = stage * -0x1000 / 5 + 0x1000;
        ((BattleFxModelDraw *)&g_BattleMagicBreak_ModelData1)->partHandle = Gfx_ModelTableGetRenderObject(2, g_BattleMagicBreak_ModelTable);
    } else {
        stage -= 5;
        if (stage < 0xF) {
            ((BattleFxModelDraw *)&g_BattleMagicBreak_ModelData1)->tintLevel = 0x80;
            ((BattleFxModelDraw *)&g_BattleMagicBreak_ModelData1)->blendFraction = 0;
            ((BattleFxModelDraw *)&g_BattleMagicBreak_ModelData1)->partHandle = Gfx_ModelTableGetRenderObject(1, g_BattleMagicBreak_ModelTable);
        } else {
            stage -= 0xF;
            if (stage < 0xA) {
                ((BattleFxModelDraw *)&g_BattleMagicBreak_ModelData1)->tintLevel = 0x88;
                ((BattleFxModelDraw *)&g_BattleMagicBreak_ModelData1)->blendFraction = (stage << 0xC) / 0xA;
                ((BattleFxModelDraw *)&g_BattleMagicBreak_ModelData1)->partHandle = Gfx_ModelTableGetRenderObject(2, g_BattleMagicBreak_ModelTable);
            } else {
                faderPtr->ownerSlot = BATTLE_TASK_FREE;
                return;
            }
        }
    }
    value = faderPtr->size * 0x300 >> 0xC;
    if (value <= 0x7FFF) {
        clamped = value;
    } else {
        clamped = 0x7FFF;
    }
    scaleVecPtr->scaleZ = clamped;
    scaleVecPtr->scaleY = clamped;
    scaleVecPtr->scaleX = clamped;
    ((PsxMatrix *)&g_BattleMagicBreak_ModelData2)->t[0] = faderPtr->posX;
    ((PsxMatrix *)&g_BattleMagicBreak_ModelData2)->t[2] = faderPtr->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &g_BattleMagicBreak_ModelData2, mtx);
    ScaleMatrix(mtx, scaleVecPtr);
    SetRotMatrix(mtx);
    SetTransMatrix(mtx);
    g_BattleMagicBreak_DrawHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)(((char *)&g_BattleMagicBreak_ModelData1 + 16)), g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleMagicBreak_DrawHandle);
    if (g_BattleIsPaused == 0) {
        (faderPtr->frame)++;
    }
}

/*
 * Renders and grows the black model effect for Break.
 */
// FUNCTION: FF7 0x005a23ee
void __cdecl BattleFx_MagicBreakBlackModelGrowTask(void)
{
    int localFrame;
    int rawBytes;
    BattleFxMagicBreakTask *pTaskSlot;
    PsxMatrix *matrix;
    int clamped8;

    pTaskSlot = (BattleFxMagicBreakTask *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleMagicBreak_ModelData3.modelGroup = Gfx_ModelTableGetRenderObject(3, g_BattleMagicBreak_ModelTable);
    matrix = (PsxMatrix *)Gfx_GetPsxScratchpadPtr(0);
    rawBytes = (pTaskSlot->frame * 0x1100 / 0x14 + 0x400) * pTaskSlot->size >> 0xC;
    if (rawBytes <= 0x7FFF) {
        clamped8 = rawBytes;
    } else {
        clamped8 = 0x7FFF;
    }
    ((BattleFxMagicBreakMatrixTail *)&g_BattleMagicBreak_ModelData3)->m22 = clamped8;
    g_BattleMagicBreak_BlackMatrix = ((BattleFxMagicBreakMatrixTail *)&g_BattleMagicBreak_ModelData3)->m22;
    ((BattleFxMagicBreakMatrixTail *)&g_BattleMagicBreak_ModelData3)->transX = pTaskSlot->posX;
    ((BattleFxMagicBreakMatrixTail *)&g_BattleMagicBreak_ModelData3)->transZ = pTaskSlot->posZ;
    localFrame = pTaskSlot->frame;
    if (localFrame < 0) {
        g_BattleMagicBreak_ModelData3.scale = 0;
    } else {
        localFrame = localFrame;
        if (localFrame < 0x14) {
            g_BattleMagicBreak_ModelData3.scale = (localFrame << 0xC) / 0x14;
        } else {
            pTaskSlot->ownerSlot = BATTLE_TASK_FREE;
            return;
        }
    }
    SetDrawColorRGB(0, 0, 0);
    MultiplyMatrix(&g_BattleCameraTransform, (PsxMatrix *)&g_BattleMagicBreak_BlackMatrix, matrix);
    SetRotMatrix(matrix);
    SetTransMatrix(matrix);
    g_BattleMagicBreak_DrawHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)(((char *)&g_BattleMagicBreak_ModelData3 + 16)), g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleMagicBreak_DrawHandle);
    if (g_BattleIsPaused == 0) {
        (pTaskSlot->frame)++;
    }
}
} /* extern "C" */
