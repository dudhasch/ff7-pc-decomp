// src/ff7/src/battle/battle_fx_magic_ice.cpp
/* Magic Effect [30]: Ice (Japanese: ブリザド Blizzard, dev tag: "MAGIC/BRIZAD" / Brizad) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle effect for the Blizzard (Brizad) magic: loads its resource package, spawns the
//   per-target model task, and animates the growing ramp models.

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
// GLOBAL: FF7 0x008fd958
int g_BattleMagicIce_ResourceDefs[6] = {1638577, (int)&g_BattleMagicIce_Filename, 0, 0, 0, 0};
// GLOBAL: FF7 0x008fd970
struct BattleMagicIce_ModelReq g_BattleMagicIce_ModelReq = {0, {0x88, 0, 0, 0, 0, 0}, 0};
unsigned char g_rawData_008fd97c[4] = {0x20, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x008fd980
unsigned char g_BattleMagicIce_Filename[12] = "BRIZAD.RSD";
// GLOBAL: FF7 0x00be0d88
int g_BattleMagicIce_ChainTail = 0;
// GLOBAL: FF7 0x00be0d8c
struct GfxTableCursor *g_BattleMagicIce_ResourceHandle = 0;
// clang-format on

#pragma intrinsic(memset)
#pragma intrinsic(memcpy)



/* Loads the Blizzard (Brizad) magic resource package if not already loaded. */
// FUNCTION: FF7 0x005b6540
void __cdecl BattleFx_MagicLoadResIce(void)
{
    GfxModelTableBank *baseOffset;
    GfxResourceRequest requestRecord;
    int kind;
    char pathEnd[200];
    GfxResourceRequest *request;
    int resultDefCatalog;

    if (g_pBattle3dScene != 0 && g_BattleMagicIce_ResourceHandle == 0) {
        kind = 1;
        resultDefCatalog = (int)&g_BattleMagicIce_ResourceDefs;
        baseOffset = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        request = &requestRecord;
        Gfx_InitResourceRequest(request);
        // STRING: FF7 0x008fd98c
        request->resourcePath = (char *)BattleFx_BuildResourcePath("MAGIC/BRIZAD", pathEnd);
        Gfx_LoadModelTableIfAbsent(kind, resultDefCatalog, request, &g_BattleMagicIce_ResourceHandle, baseOffset);
    }
}

/* Entry point for the Blizzard (Brizad) magic effect. */
// FUNCTION: FF7 0x005b65c7
void __cdecl BattleFx_MagicEnterIce(int targetMask, int actorIdx)
{
    BattleFx_MagicLoadResIce();
    Battle_CreateBitPatternCallbackTask(targetMask, actorIdx, 4, BattleFx_MagicSpawnIcePerTarget);
    Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputePanForEntity(g_BattleEffectScratch.targetMask), 0x18);
}

/* Spawns the initial model task for Blizzard (Brizad) per target actor. */
// FUNCTION: FF7 0x005b6605
void __cdecl BattleFx_MagicSpawnIcePerTarget(int actor)
{
    struct BattleTaskSlot *effectSlot;

    effectSlot = BATTLE_NEW_TASK_SLOT(BattleFx_MagicIceSpawnRampModelTask);
    ((BattleFxIceRampRecord *)effectSlot)->actorIdx = actor;
}

/* Spawns ramp model and initial parameters for Blizzard (Brizad). */
// FUNCTION: FF7 0x005b6633
void __cdecl BattleFx_MagicIceSpawnRampModelTask(void)
{
    int unused; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int stackWords; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxIceRampRecord *task;
    BattleIceRampSlot *child;

    task = (BattleFxIceRampRecord *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused == 0) {
        if (task->stage == 0) {
            child = (BattleIceRampSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicIceGrowingRampModelTask);
            Battle_GetBoneWorldPos(task->actorIdx,
                                   g_BattleActorRecords[task->actorIdx].targetBone,
                                   &child->posX);
            child->rotZ = 0;
            child->rotY = 0;
            child->rotX = 0;
            child->effectSize = Battle_GetActorScaledEffectSize(task->actorIdx);
            Battle_PostActorHitOrHurtMessage(task->actorIdx);
        }
        if (task->stage == 0) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
        (task->stage)++;
    }
}

/* Updates and renders a growing ramp model for Blizzard (Brizad). */
// FUNCTION: FF7 0x005b671d
void __cdecl BattleFx_MagicIceGrowingRampModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[76];
        int clampedScale;
        struct { unsigned char pad_to_4[0x4]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_24[0x24]; int unclampedScale; };
        struct { unsigned char pad_to_2C[0x2C]; int blend; };
        struct { unsigned char pad_to_38[0x38]; BattleFxIceRampSlot *slot; };
        struct { unsigned char pad_to_3C[0x3C]; VECTOR scaleX; };
    } frame;
#pragma pack(pop)

    frame.slot = (BattleFxIceRampSlot *)BATTLE_CURRENT_TASK_SLOT;
    frame.unclampedScale = frame.slot->step * 0x36D * frame.slot->scale >> 0xC;
    if (frame.unclampedScale <= 0x7FFF) {
        frame.clampedScale = frame.unclampedScale;
    } else {
        frame.clampedScale = 0x7FFF;
    }
    frame.scaleX.vz = frame.clampedScale;
    frame.scaleX.vy = frame.scaleX.vz;
    frame.scaleX.vx = frame.scaleX.vy;
    if (frame.slot->step < 0) {
        frame.blend = 0;
    } else {
        frame.blend = frame.slot->step * 0x124;
    }
    RotMatrix((SVECTOR *)&frame.slot->rotX, &frame.rotMatrix);
    ScaleMatrix(&frame.rotMatrix, &frame.scaleX);
    frame.rotMatrix.t[0] = frame.slot->posX;
    frame.rotMatrix.t[1] = frame.slot->posY;
    frame.rotMatrix.t[2] = frame.slot->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.rotMatrix);
    SetRotMatrix(&frame.rotMatrix);
    SetTransMatrix(&frame.rotMatrix);
    SetDrawColorRGB(0, 0, 0);
    g_BattleMagicIce_ModelReq.alpha = *(short *)&frame.blend;
    (g_BattleMagicIce_ModelReq.modelEntry) = Gfx_ModelTableGetRenderObject(0, g_BattleMagicIce_ResourceHandle);
    g_BattleMagicIce_ChainTail = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleMagicIce_ModelReq, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleMagicIce_ChainTail);
    if (g_BattleIsPaused == 0) {
        (frame.slot->step)++;
        if (frame.slot->step >= 0xF) {
            frame.slot->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}
} /* extern "C" */
