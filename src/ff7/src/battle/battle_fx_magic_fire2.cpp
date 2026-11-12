// src/ff7/src/battle/battle_fx_magic_fire2.cpp
/* Magic Effect [28]: Fire2 (Japanese: ファイラ Fira, dev tag: "MAGIC/FAIRA" / Faira) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle effect for the Fira (Faira) magic: loads its resources and spawns per-target
//   tasks that animate a cycling fire model.

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
// GLOBAL: FF7 0x008fdf50
int g_BattleMagicFire2_ResourceDefs = 0x001941B1;
int g_BattleMagicFire2_ResourceDefsHeadPtr[1] = {
    (int)((char *)&g_BattleFxMagicFire2ResourceNames2 + 0x0)
};
unsigned int g_BattleFxMagicFire2ResourceNames[38] = {
    // STRING: FF7 0x008fe000
    (unsigned int)"FAIRA00.TIM",
    0x00000000,
    0x00000000,
    0x001941b1,
    // STRING: FF7 0x008fe00c
    (unsigned int)"2.RSD",
    // STRING: FF7 0x008fe014
    (unsigned int)"FAIRA00.TIM",
    0x00000000,
    0x00000000,
    0x001941b1,
    // STRING: FF7 0x008fe020
    (unsigned int)"3.RSD",
    // STRING: FF7 0x008fe028
    (unsigned int)"FAIRA00.TIM",
    0x00000000,
    0x00000000,
    0x001941b1,
    // STRING: FF7 0x008fe034
    (unsigned int)"4.RSD",
    // STRING: FF7 0x008fe03c
    (unsigned int)"FAIRA00.TIM",
    0x00000000,
    0x00000000,
    0x001941b1,
    // STRING: FF7 0x008fe048
    (unsigned int)"5.RSD",
    // STRING: FF7 0x008fe050
    (unsigned int)"FAIRA01.TIM",
    0x00000000,
    0x00000000,
    0x001941b1,
    // STRING: FF7 0x008fe05c
    (unsigned int)"6.RSD",
    // STRING: FF7 0x008fe064
    (unsigned int)"FAIRA01.TIM",
    0x00000000,
    0x00000000,
    0x001941b1,
    // STRING: FF7 0x008fe070
    (unsigned int)"7.RSD",
    // STRING: FF7 0x008fe078
    (unsigned int)"FAIRA01.TIM",
    0x00000000,
    0x00000000,
    0x001941b1,
    // STRING: FF7 0x008fe084
    (unsigned int)"8.RSD",
    // STRING: FF7 0x008fe08c
    (unsigned int)"FAIRA01.TIM",
    0x00000000,
    0x00000000
};
// GLOBAL: FF7 0x008fdff0
struct SVECTOR g_BattleMagicFire2_ScaleFactors = {1024, 1024, 1024, 0};
unsigned char g_BattleFxMagicFire2ResourceNames2[8] = "1.RSD";
// clang-format on
#pragma intrinsic(memset)
#pragma intrinsic(memcpy)


// clang-format off
// GLOBAL: FF7 0x00be0de8
struct BattleMagicFire2_ModelReq g_BattleMagicFire2_ModelReq = {0};
// GLOBAL: FF7 0x00be0e10
struct GfxTableCursor *g_BattleMagicFire2_ResourceHandle = 0;
unsigned char g_bssPad_00be0e14[4] = {0};
// GLOBAL: FF7 0x00be0e18
short g_BattleMagicFire2_InitialRotation[4] = {0};
// clang-format on

/* Loads the Fira (Faira) magic resource package if not already loaded. */
// FUNCTION: FF7 0x005b9480
void __cdecl BattleFx_MagicLoadResFire2(void)
{
    GfxModelTableBank *rendererField;
    GfxResourceRequest requestRecord;
    int typeId;
    char pathBuffer[200];
    GfxResourceRequest *request;
    int resDescList;

    if (g_pBattle3dScene != 0 && g_BattleMagicFire2_ResourceHandle == 0) {
        typeId = 8;
        resDescList = (int)&g_BattleMagicFire2_ResourceDefs;
        rendererField = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        request = &requestRecord;
        Gfx_InitResourceRequest(request);
        request->groupTag = 0;
        // STRING: FF7 0x008fe098
        request->resourcePath = BattleFx_BuildResourcePath("MAGIC/FAIRA", pathBuffer);
        Gfx_LoadModelTableIfAbsent(typeId, resDescList, request, &g_BattleMagicFire2_ResourceHandle, rendererField);
    }
}

/* Entry point for the Fira (Faira) magic effect. */
// FUNCTION: FF7 0x005b9510
void __cdecl BattleFx_MagicEnterFire2(int targetMask, int actorIdx)
{
    BattleFx_MagicLoadResFire2();
    g_BattleMagicFire2_ModelReq.cycleIndex = 0;
    BattleFx_MagicBeginFire2(targetMask, actorIdx);
}

/* Initializes parameters and spawns per-target callback for Fira (Faira). */
// FUNCTION: FF7 0x005b9534
void __cdecl BattleFx_MagicBeginFire2(int targetMask, int actorIdx)
{
    Battle_CreateBitPatternCallbackTask(targetMask, actorIdx, 0, BattleFx_MagicSpawnFire2PerTarget);
    Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputePanForEntity(targetMask), 0xA);
}

/* Spawns cycling model tasks for Fira (Faira) per target actor. */
// FUNCTION: FF7 0x005b9569
void __cdecl BattleFx_MagicSpawnFire2PerTarget(int actor)
{
    int unused; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int scaledBytes;
    BattleFxFire2ModelTask *record;
    int clampedSize;

    record = (BattleFxFire2ModelTask *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicFire2CyclingModelTask);
    Battle_GetBoneWorldPos(actor, g_BattleActorRecords[actor].targetBone, &record->posX);
    record->posY = 0;
    scaledBytes = Battle_GetActorScaledEffectSize(actor);
    if (scaledBytes <= 32767) {
        clampedSize = scaledBytes;
    } else {
        clampedSize = 0x7FFF;
    }
    record->size = clampedSize;
    Battle_PostActorHitOrHurtMessage(actor);
}

/* Updates and renders a cycling fire model for Fira (Faira). */
// FUNCTION: FF7 0x005b95f9
void __cdecl BattleFx_MagicFire2CyclingModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[108];
        struct { unsigned char pad_to_4[0x4]; BattleFxFire2CycleSlot *cycleSlot; };
        struct { unsigned char pad_to_8[0x8]; PsxMatrix transformMatrix; };
        struct { unsigned char pad_to_28[0x28]; int cycleIdx0[8]; };
        struct { unsigned char pad_to_58[0x58]; BattleFxModelDraw *modelDraw; };
        struct { unsigned char pad_to_5C[0x5C]; VECTOR scaleVec; };
    } frame;
#pragma pack(pop)

    frame.cycleSlot = (BattleFxFire2CycleSlot *)BATTLE_CURRENT_TASK_SLOT;
    frame.modelDraw = (BattleFxModelDraw *)Gfx_GetPsxScratchpadPtr(0);
    frame.cycleIdx0[0] = 0;
    frame.cycleIdx0[1] = 1;
    frame.cycleIdx0[2] = 2;
    frame.cycleIdx0[3] = 3;
    frame.cycleIdx0[4] = 4;
    frame.cycleIdx0[5] = 5;
    frame.cycleIdx0[6] = 6;
    frame.cycleIdx0[7] = 7;
    RotMatrix(&g_BattleMagicFire2_ScaleFactors, &g_BattleMagicFire2_ModelReq.rotationMatrix);
    frame.scaleVec.vz = frame.cycleSlot->scale;
    frame.scaleVec.vy = frame.scaleVec.vz;
    frame.scaleVec.vx = frame.scaleVec.vy;
    ScaleMatrix(&g_BattleMagicFire2_ModelReq.rotationMatrix, &frame.scaleVec);
    g_BattleMagicFire2_ModelReq.transX = frame.cycleSlot->posX;
    g_BattleMagicFire2_ModelReq.transY = frame.cycleSlot->posY;
    g_BattleMagicFire2_ModelReq.transZ = frame.cycleSlot->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &g_BattleMagicFire2_ModelReq.rotationMatrix, &frame.transformMatrix);
    SetRotMatrix(&frame.transformMatrix);
    SetTransMatrix(&frame.transformMatrix);
    frame.modelDraw->modelGroup = Gfx_ModelTableGetRenderObject(frame.cycleIdx0[g_BattleMagicFire2_ModelReq.cycleIndex], g_BattleMagicFire2_ResourceHandle);
    frame.modelDraw->flags = 8;
    frame.modelDraw->uvScroll = g_BattleMagicFire2_InitialRotation[frame.cycleSlot->step & 3];
    if (frame.cycleSlot->step < 8) {
        frame.modelDraw->fadeLevel = (frame.cycleSlot->step + 1) * 14;
    } else if (frame.cycleSlot->step < 16) {
        frame.modelDraw->fadeLevel = 128;
    } else if (frame.cycleSlot->step < 24) {
        frame.modelDraw->fadeLevel = 128 - (frame.cycleSlot->step - 15) * 14;
    } else {
        frame.cycleSlot->ownerSlot = BATTLE_TASK_FREE;
        return;
    }
    frame.modelDraw->stateFlags = frame.cycleSlot->step >> 3 & 1 | 0x20;
    frame.modelDraw->texturePageBits = 0;
    (g_BattleMagicFire2_ModelReq.drawHandle) = BattleFx_SubmitModelDraw(frame.modelDraw, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, (g_BattleMagicFire2_ModelReq.drawHandle));
    if (g_BattleIsPaused == 0) {
        (frame.cycleSlot->step)++;
        g_BattleMagicFire2_ModelReq.cycleIndex = g_BattleMagicFire2_ModelReq.cycleIndex + 1 & 7;
    }
}
} /* extern "C" */
