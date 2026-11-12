// src/ff7/src/battle/battle_fx_magic_quake.cpp
/* Magic Effect [36]: Quake (Japanese: クエイク, dev tag: "MAGIC/QUEIK" / Queik) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Quake (Queik) magic effect: boulder models that grow and shrink for each target.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/enums/sound.h"
#include "ff7/battle_tasks.h"
#include "ff7/types/battle.h"
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
// GLOBAL: FF7 0x008b0340
int g_BattleMagicQuakeResourceDefs = 0x001900B4;
int g_BattleMagicQuakeRsdPath[1] = {
    (int)((char *)&g_BattleFxMagicQuakeBouldersResourceNames + 0xc)
};
unsigned char g_BattleFxMagicQuakeBouldersResourceNames[24] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x71, 0x75, 0x65, 0x69, 0x6b, 0x2e, 0x72, 0x73, 0x64, 0x00, 0x00, 0x00};
// clang-format on
#pragma intrinsic(memset)
#pragma intrinsic(memcpy)


// clang-format off
// GLOBAL: FF7 0x00bb0840
struct GfxTableCursor *g_BattleMagicQuakeLoadedResTable = 0;
unsigned char g_bssPad_00bb0844[4] = {0};
// clang-format on

/* Loads and registers resources for Quake (Queik). */
// FUNCTION: FF7 0x005819d0
void __cdecl BattleFx_MagicLoadResQuake(void)
{
    char pathBuf[200];
    GfxResourceRequest requestBuf;
    int kind;
    GfxModelTableBank *sceneOwner;
    GfxResourceRequest *requestPtr;
    int resDefsGroup;

    if (g_pBattle3dScene != 0 && g_BattleMagicQuakeLoadedResTable == 0) {
        kind = 1;
        resDefsGroup = (int)&g_BattleMagicQuakeResourceDefs;
        sceneOwner = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        requestPtr = &requestBuf;
        Gfx_InitResourceRequest(requestPtr);
        requestPtr->groupTag = 0;
        // STRING: FF7 0x008b0360
        requestPtr->resourcePath = BattleFx_BuildResourcePath("magic/queik", pathBuf);
        Gfx_LoadModelTableIfAbsent(kind, resDefsGroup, requestPtr, &g_BattleMagicQuakeLoadedResTable, sceneOwner);
    }
}

/* Entry point for the Quake (Queik) magic effect. */
// FUNCTION: FF7 0x00581a60
void __cdecl BattleFx_MagicEnterQuake(int targetMask, int actorIdx)
{
    BattleFx_MagicLoadResQuake();
    BattleFx_MagicBeginQuake(targetMask, actorIdx);
}

/* Initializes Quake (Queik) effect tasks and audio. */
// FUNCTION: FF7 0x00581a7a
void __cdecl BattleFx_MagicBeginQuake(int targetMask, int actorIdx)
{
    g_BattleFxDrawListBufferTaskSlotPtr = (unsigned char *)BATTLE_NEW_TASK_SLOT(BattleFx_SelectDrawListBuffer);
    Battle_CreateBitPatternCallbackTask(targetMask, actorIdx, 1, BattleFx_MagicSpawnQuakePerTarget);
    Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputePanForEntity(targetMask), 0x47);
}

/* Spawns per-target Quake (Queik) boulder model tasks. */
// FUNCTION: FF7 0x00581acc
void __cdecl BattleFx_MagicSpawnQuakePerTarget(int actor)
{
    BattleFxClubCardPulsingModel *task;

    task = (BattleFxClubCardPulsingModel *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicQuakeGrowShrinkModelTask);
    Battle_GetBoneWorldPos(actor, g_BattleActorRecords[actor].targetBone, &task->posX);
    task->posY = 0;
    task->extent = 0;
    task->radius = g_BattleActorRecords[actor].radius + 224;
    if (task->radius > 1900) {
        task->radius = 1900;
    }
    task->radius = task->radius / 8;
    task->targetIndex = actor;
}

/* Animates growing and shrinking Quake boulder model. */
// FUNCTION: FF7 0x00581b7c
void __cdecl BattleFx_MagicQuakeGrowShrinkModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[68];
        SVECTOR rotX;
        struct { unsigned char pad_to_8[0x8]; BattleFxGrowShrinkRecord *growRecord; };
        struct { unsigned char pad_to_C[0xC]; int growScale; };
        struct { unsigned char pad_to_10[0x10]; PsxMatrix modelMatrix; };
        struct { unsigned char pad_to_30[0x30]; BattleFxModelDraw *modelDraw; };
        struct { unsigned char pad_to_34[0x34]; VECTOR scaleVec; };
    } frame;
#pragma pack(pop)

    frame.growRecord = (BattleFxGrowShrinkRecord *)BATTLE_CURRENT_TASK_SLOT;
    frame.rotX.vx = 0;
    frame.rotX.vy = 0;
    frame.rotX.vz = 0;
    RotMatrixXYZ(&frame.rotX, &frame.modelMatrix);
    frame.modelMatrix.t[0] = frame.growRecord->posX;
    frame.modelMatrix.t[1] = frame.growRecord->posY;
    frame.modelMatrix.t[2] = frame.growRecord->posZ;
    frame.growScale = (frame.growRecord->value << 12) / 600;
    frame.scaleVec.vz = frame.growScale;
    frame.scaleVec.vy = frame.scaleVec.vz;
    frame.scaleVec.vx = frame.scaleVec.vy;
    ScaleMatrix(&frame.modelMatrix, &frame.scaleVec);
    MultiplyMatrix(&g_BattleCameraTransform, &frame.modelMatrix, &frame.modelMatrix);
    SetRotMatrix(&frame.modelMatrix);
    SetTransMatrix(&frame.modelMatrix);
    frame.modelDraw = (BattleFxModelDraw *)Gfx_GetPsxScratchpadPtr(0);
    frame.modelDraw->modelGroup = Gfx_ModelTableGetRenderObject(0, g_BattleMagicQuakeLoadedResTable);
    frame.modelDraw->flags = 0;
    frame.modelDraw->uvScroll = 0;
    frame.modelDraw->fadeLevel = 128;
    frame.modelDraw->stateFlags = 0;
    frame.modelDraw->texturePageBits = 0;
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitModelDraw(frame.modelDraw, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (frame.growRecord->stage >= 22) {
        frame.growRecord->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.growRecord->stage == 14) {
        Battle_PostActorHitOrHurtMessage(frame.growRecord->recordType);
    }
    if (frame.growRecord->stage < 8) {
        frame.growRecord->value += frame.growRecord->step;
    } else if (frame.growRecord->stage >= 14) {
        frame.growRecord->value -= frame.growRecord->step;
    }
    (frame.growRecord->stage)++;
}
} /* extern "C" */
