// src/ff7/src/battle/battle_fx_magic_silence.cpp
/* Magic Effect [13]: Silence (Japanese: サイレス, dev tag: "MAGIC/SAILESS" / Sailess) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Silence (Sailess) magic effect: a rotating, growing bubble model for each target.

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
// GLOBAL: FF7 0x008b02f0
int g_BattleMagicSilenceResourceDefs = 0x001900B4;
char *g_BattleFxMagicSilenceBubblesResourceNames[1] = {
    // STRING: FF7 0x008b0318
    "sailess.rsd"
};
unsigned int g_BattleMagicSilenceResourceDefsDataTail[8] = {
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x008b0324
    (unsigned int)"sailess.rsd",
    0x00000000,
    0x00000000,
    0x00000000
};
// clang-format on
#pragma intrinsic(memset)
#pragma intrinsic(memcpy)


// clang-format off
// GLOBAL: FF7 0x00bb083c
struct GfxTableCursor *g_BattleMagicSilenceLoadedResTable = 0;
// clang-format on

/* Loads the Silence (Sailess) model table when it is not already loaded. */
// FUNCTION: FF7 0x00581550
void __cdecl BattleFx_MagicLoadResSilence(void)
{
    char pathBuf[200];
    GfxResourceRequest requestBuf;
    int kind;
    GfxModelTableBank *sceneOwner;
    GfxResourceRequest *requestPtr;
    int resultDefsBank;

    if (g_pBattle3dScene != 0 && g_BattleMagicSilenceLoadedResTable == 0) {
        kind = 2;
        resultDefsBank = (int)&g_BattleMagicSilenceResourceDefs;
        sceneOwner = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        requestPtr = &requestBuf;
        Gfx_InitResourceRequest(requestPtr);
        requestPtr->groupTag = 0;
        // STRING: FF7 0x008b0330
        requestPtr->resourcePath = BattleFx_BuildResourcePath("magic/sailess", pathBuf);
        Gfx_LoadModelTableIfAbsent(kind, resultDefsBank, requestPtr, &g_BattleMagicSilenceLoadedResTable, sceneOwner);
    }
}

/* Loads the Silence (Sailess) resources and starts the effect. */
// FUNCTION: FF7 0x005815e0
void __cdecl BattleFx_MagicEnterSilence(int targetMask, int actorIdx)
{
    BattleFx_MagicLoadResSilence();
    BattleFx_MagicBeginSilence(targetMask, actorIdx);
}

/* Starts the draw list buffer task, queues a bubble per target and plays the sound. */
// FUNCTION: FF7 0x005815fa
void __cdecl BattleFx_MagicBeginSilence(int targetMask, int actorIdx)
{
    *(int *)&g_BattleFxDrawListBufferTaskSlotPtr = (int)BATTLE_NEW_TASK_SLOT(BattleFx_SelectDrawListBuffer);
    Battle_CreateBitPatternCallbackTask(targetMask, actorIdx, 1, BattleFx_MagicSpawnSilencePerTarget);
    Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputePanForEntity(targetMask), 0x54);
}

/* Spawns a growing bubble model task at the target's bone position. */
// FUNCTION: FF7 0x0058164c
void __cdecl BattleFx_MagicSpawnSilencePerTarget(int actor)
{
    int extent;
    BattleFxSilenceGrowingModel *pSpawn;

    pSpawn = (BattleFxSilenceGrowingModel *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicSilenceGrowingModelTask);
    Battle_GetBoneWorldPos(actor, g_BattleActorRecords[actor].targetBone, &pSpawn->posX);
    pSpawn->spinAngle = 0;
    pSpawn->yaw = g_BattleActorRecords[actor].yaw;
    extent = g_BattleActorRecords[actor].radius + 0xE0;
    if (extent > 0x76C) {
        extent = 0x76C;
    }
    pSpawn->extent = extent;
    pSpawn->extentStep = extent / 0x10;
    if (extent > 0x4F2) {
        extent = 0x4F2;
    }
    pSpawn->scale = 0;
    pSpawn->scaleStep = (extent << 0xC) / 0xFA / 8;
    pSpawn->targetIndex = actor;
}

/* Spins, grows and then shrinks one Silence bubble model, and frees it after its last frame. */
// FUNCTION: FF7 0x0058173e
void __cdecl BattleFx_MagicSilenceGrowingModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[116];
        SVECTOR rotX;
        struct { unsigned char pad_to_8[0x8]; BattleFxSilenceGrowingModel *bubbleRecord; };
        struct { unsigned char pad_to_C[0xC]; SVECTOR offsetX; };
        struct { unsigned char pad_to_18[0x18]; PsxMatrix orbitMatrix; };
        struct { unsigned char pad_to_38[0x38]; PsxMatrix modelMatrix; };
        struct { unsigned char pad_to_58[0x58]; SVECTOR orbitRotX; };
        struct { unsigned char pad_to_60[0x60]; BattleFxModelDraw *modelDraw; };
        struct { unsigned char pad_to_64[0x64]; VECTOR scaleVec; };
    } frame;
#pragma pack(pop)

    frame.bubbleRecord = (BattleFxSilenceGrowingModel *)BATTLE_CURRENT_TASK_SLOT;
    frame.rotX.vx = 0;
    frame.rotX.vy = frame.bubbleRecord->yaw;
    frame.rotX.vz = frame.bubbleRecord->spinAngle;
    RotMatrixXYZ(&frame.rotX, &frame.modelMatrix);
    frame.orbitRotX.vx = 0;
    frame.orbitRotX.vy = frame.bubbleRecord->yaw;
    frame.orbitRotX.vz = 0;
    RotMatrixXYZ(&frame.orbitRotX, &frame.orbitMatrix);
    frame.offsetX.vx = 0;
    frame.offsetX.vy = 0;
    frame.offsetX.vz = -frame.bubbleRecord->extent;
    ApplyMatrixToSVectorInt32(&frame.orbitMatrix, &frame.offsetX, &frame.modelMatrix.t[0]);
    frame.modelMatrix.t[0] += frame.bubbleRecord->posX;
    frame.modelMatrix.t[1] += frame.bubbleRecord->posY;
    frame.modelMatrix.t[2] += frame.bubbleRecord->posZ;
    frame.scaleVec.vz = frame.bubbleRecord->scale;
    frame.scaleVec.vy = frame.scaleVec.vz;
    frame.scaleVec.vx = frame.scaleVec.vy;
    ScaleMatrix(&frame.modelMatrix, &frame.scaleVec);
    MultiplyMatrix(&g_BattleCameraTransform, &frame.modelMatrix, &frame.modelMatrix);
    SetRotMatrix(&frame.modelMatrix);
    SetTransMatrix(&frame.modelMatrix);
    frame.modelDraw = (BattleFxModelDraw *)Gfx_GetPsxScratchpadPtr(0);
    frame.modelDraw->modelGroup = Gfx_ModelTableGetRenderObject(0, g_BattleMagicSilenceLoadedResTable);
    frame.modelDraw->flags = 0;
    frame.modelDraw->uvScroll = 0;
    frame.modelDraw->fadeLevel = 0x80;
    frame.modelDraw->stateFlags = 0x20;
    frame.modelDraw->texturePageBits = 0;
    if (frame.bubbleRecord->timer >= 0xC) {
        frame.modelDraw->flags = 8;
        frame.modelDraw->fadeLevel -= (short)(frame.bubbleRecord->timer * 8 + -0x60);
        frame.modelDraw->modelGroup = Gfx_ModelTableGetRenderObject(1, g_BattleMagicSilenceLoadedResTable);
    }
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitModelDraw(frame.modelDraw, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (frame.bubbleRecord->timer >= 0x1C) {
        frame.bubbleRecord->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.bubbleRecord->timer == 0x14) {
        Battle_PostActorHitOrHurtMessage(frame.bubbleRecord->targetIndex);
    }
    if (frame.bubbleRecord->timer < 8) {
        frame.bubbleRecord->spinAngle += 0x200;
        frame.bubbleRecord->scale += frame.bubbleRecord->scaleStep;
    } else if (frame.bubbleRecord->timer >= 0xC) {
        frame.bubbleRecord->extent -= frame.bubbleRecord->extentStep;
    }
    (frame.bubbleRecord->timer)++;
}
} /* extern "C" */
