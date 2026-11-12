// src/ff7/src/battle/battle_fx_magic_fire.cpp
/* Magic Effect [27]: Fire (Japanese: ファイア, dev tag: "MAGIC/FIRE") */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle effect for the Fire magic: lazily loads its resources, then runs a per-target
//   timeline that emits animated fire sprites on the target's bones.

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

#pragma intrinsic(abs)

// clang-format off
// GLOBAL: FF7 0x008fe0a8
int g_BattleMagicFire_ResourceDescriptor = 0x00192A39;
char *g_BattleFxMagicFireResourceNames[1] = {
    // STRING: FF7 0x008fe0e0
    "FIRE00.TIM"
};
typedef struct {
    char *filename;
    unsigned int descriptor;
} BattleMagicFireResourceTailEntry;
BattleMagicFireResourceTailEntry g_BattleFxMagicFireAssetPrefix[4] = {
    {
        // STRING: FF7 0x008fe0ec
        "FI_A01.SPT",
        0x00000000
    },
    {
        0,
        0x00192a39
    },
    {
        // STRING: FF7 0x008fe0f8
        "FIRE01.TIM",
        0x00000000
    },
    {
        0,
        0x00000000
    }
};
#undef g_BattleMagicFire_SpriteDrawReq
// GLOBAL: FF7 0x008fe0d0
struct BattleMagicFire_SpriteDrawReq g_BattleMagicFire_SpriteDrawReq = {0, 0, {0x80, 0x80, 0x80, 0x2c}, 0, {0}};
// clang-format on
#pragma intrinsic(memset)
#pragma intrinsic(memcpy)

// clang-format off
// clang-format on



// clang-format off
// GLOBAL: FF7 0x00be0e20
int g_BattleMagicFire_DrawListHandle = 0;
// GLOBAL: FF7 0x00be0e24
struct GfxTableCursor *g_BattleMagicFire_LoadedRes = 0;
// GLOBAL: FF7 0x00be0e28
struct BattleActorTransformRecord g_BattleActorTransformRecords[10] = {0};
// GLOBAL: FF7 0x00be10a8
int g_BattleSummonSavedDrawFlags = 0;
unsigned char g_bssPad_00be10ac[4] = {0};
unsigned char g_bssPad_00be10b0[4] = {0};
// GLOBAL: FF7 0x00be10b4
char g_BattleApproachHoldFrames = 0;
// GLOBAL: FF7 0x00be10b8
short g_BattleCameraAuxTaskSlot = 0;
unsigned char g_bssPad_00be10bc[4] = {0};
// The overlay task callbacks: Battle_AllocateOverlayTaskSlot writes ten function pointers here.
// GLOBAL: FF7 0x00be10c0
BattleTaskFn g_BattleOverlayTaskCallbacks[10] = {0};
// GLOBAL: FF7 0x00be10e8
short g_BattleSecondaryResourceCount = 0;
// GLOBAL: FF7 0x00be10ec
char g_BattlePsxHookCalled = 0;
// GLOBAL: FF7 0x00be10f0
struct BattleCameraChannel g_BattleCameraEyeChannels[4] = {0};
// GLOBAL: FF7 0x00be1128
int g_BattleCurrentOrderingTable = 0;
unsigned char g_bssPad_00be112c[4] = {0};
// GLOBAL: FF7 0x00be1130
struct BattleCameraChannel g_BattleCameraFocusChannels[4] = {0};
// GLOBAL: FF7 0x00be1168
char g_BattleLoseMusicRequested = 0;
unsigned char g_bssPad_00be116c[4] = {0};
// GLOBAL: FF7 0x00be1170
char g_BattleActiveActorId = 0;
unsigned char g_bssPad_00be1174[4] = {0};
// GLOBAL: FF7 0x00be1178
struct BattleActor g_BattleActorRecords[10] = {0};
// GLOBAL: FF7 0x00bf1eb0
char g_BattleStatusOverlayAttached = 0;
unsigned char g_bssPad_00bf1eb4[4] = {0};
// clang-format on
// The TU's own view of moved definitions declared with another type.

/*
 * Lazily loads the Fire magic effect resource package and descriptors.
 */
// FUNCTION: FF7 0x005b9810
void __cdecl BattleFx_MagicLoadResFire(void)
{
    GfxModelTableBank *rendererField;
    GfxResourceRequest requestRecord;
    int typeId;
    char pathBuffer[200];
    GfxResourceRequest *request;
    int resDescList;

    if (g_pBattle3dScene != 0 && g_BattleMagicFire_LoadedRes == 0) {
        typeId = 2;
        resDescList = (int)&g_BattleMagicFire_ResourceDescriptor;
        rendererField = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        request = &requestRecord;
        Gfx_InitResourceRequest(request);
        request->groupTag = 0;
        // STRING: FF7 0x008fe104
        request->resourcePath = BattleFx_BuildResourcePath("MAGIC/FIRE", pathBuffer);
        Gfx_LoadModelTableIfAbsent(typeId, resDescList, request, &g_BattleMagicFire_LoadedRes, rendererField);
    }
}

/*
 * Entry point for Fire magic effect, spawning target callbacks and sound.
 */
// FUNCTION: FF7 0x005b98a0
void __cdecl BattleFx_MagicEnterFire(int targetMask, int actorIdx)
{
    BattleFx_MagicLoadResFire();
    Battle_CreateBitPatternCallbackTask(targetMask, actorIdx, 0, BattleFx_MagicSpawnFirePerTarget);
    Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputePanForEntity(g_BattleEffectScratch.targetMask), 9);
}

/*
 * Spawns timeline task for a target affected by Fire.
 */
// FUNCTION: FF7 0x005b98de
void __cdecl BattleFx_MagicSpawnFirePerTarget(int actor)
{
    BattleFxFireTargetRecord *record;

    record = (BattleFxFireTargetRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicFireTimelineTask);
    record->actorId = actor;
    record->boneIndex = 0;
    record->negativeRadius = -g_BattleActorRecords[actor].radius;
}

/*
 * Drives timeline for Fire effect, emitting animated fire sprites on bones.
 */
// FUNCTION: FF7 0x005b992e
void __cdecl BattleFx_MagicFireTimelineTask(void)
{
    int stackWords[2]; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int padSlot[3]; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxFireAnimatedSprite *pSprite;
    BattleFxFireTimeline *task;

    task = (BattleFxFireTimeline *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (task->stage == 0) {
        Battle_PostActorHitOrHurtMessage(task->actorIndex);
    }
    if ((task->stage & 1) == 0) {
        pSprite = (BattleFxFireAnimatedSprite *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicFireAnimatedSpriteTask);
        task->boneIndex = (task->boneIndex + (rand() & 0xF) + 1) %
                                    g_BattleActorRecords[task->actorIndex].boneCount;
        Battle_GetBoneWorldPos(task->actorIndex, task->boneIndex, &pSprite->posX);
        pSprite->param = task->param;
    }
    (task->stage)++;
    if (task->stage >= 5) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Updates and renders animated billboard sprite particles for Fire effect.
 */
// FUNCTION: FF7 0x005b9a26
void __cdecl BattleFx_MagicFireAnimatedSpriteTask(void)
{
    BattleMagicFireSpriteSlot *task;

    task = (BattleMagicFireSpriteSlot *)BATTLE_CURRENT_TASK_SLOT;
    (g_BattleMagicFire_SpriteDrawReq.drawReq) = Gfx_ModelTableGetLoadedRecord(0, g_BattleMagicFire_LoadedRes);
    g_BattleMagicFire_SpriteDrawReq.drawObj = Gfx_ModelTableGetSpriteObject(0, g_BattleMagicFire_LoadedRes);
    if ((g_BattleMagicFire_SpriteDrawReq.drawReq) != 0) {
        ((BattleFxBlueFlameThrowerDrawReq *)(g_BattleMagicFire_SpriteDrawReq.drawReq))->drawObj0 = Gfx_ModelTableGetSpriteObject(0, g_BattleMagicFire_LoadedRes);
        ((BattleFxBlueFlameThrowerDrawReq *)(g_BattleMagicFire_SpriteDrawReq.drawReq))->drawObj1 = Gfx_ModelTableGetSpriteObject(1, g_BattleMagicFire_LoadedRes);
    }
    BattleFx_InstallBillboardMatrix((void *)((int)task + 8), 0x1000 /* 1.0 */, task->pushLength);
    g_BattleMagicFire_SpriteDrawReq.frame = task->frameIndex;
    g_BattleMagicFire_DrawListHandle = BattleFx_SubmitSpriteDraw(&g_BattleMagicFire_SpriteDrawReq, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleMagicFire_DrawListHandle);
    if (g_BattleIsPaused == 0) {
        (task->frameIndex)++;
        if (task->frameIndex >= 14) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}
} /* extern "C" */
