// src/ff7/src/battle/yasui/yasui_item_echo_screen.cpp
/* Item FX: Echo Screen (Japanese: やまびこえんまく Yamabiko Enmaku, dev tag: "ITEM/YAMABIKO" / Yamabiko) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle effect for the Echo Screen item: a per-target callback spawns emitters that
//   burst animated echo sprites around each target, with the echo sound.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/enums/sound.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_boss.h"
#include "ff7/battle_tasks.h"
#include "ff7/camera_script.h"
#include "lib/types/graphics.h"
#include <string.h>
#include <stdlib.h>
#include "lib/codegen.h"
#include "ff7/api/battle.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
extern "C" {

#pragma intrinsic(memcpy)
#pragma intrinsic(memset)
#pragma intrinsic(abs)

// clang-format off
// GLOBAL: FF7 0x007ed350
unsigned int g_BattleFxEchoScreenModelLoadEntry[5] = {
    0x00192a39,
    (unsigned int)&g_YasuiItemEchoScreenModelFilename,
    // STRING: FF7 0x007ed384
    (unsigned int)"KEMURI.SPT"
};
// GLOBAL: FF7 0x007ed368
struct YasuiItemEchoScreenSpriteRequest g_BattleFxEchoScreenSpriteRequest = {0, 0, {0x80, 0x80, 0x80, 0x2e}, 0, {0}};
// GLOBAL: FF7 0x007ed378
unsigned char g_YasuiItemEchoScreenModelFilename[11] = "ECHOES.TIM";
// clang-format on


// clang-format off
// GLOBAL: FF7 0x00a419fc
int g_YasuiItemEchoScreenSpriteDrawHandle = 0;
// GLOBAL: FF7 0x00a41a00
struct GfxTableCursor *g_YasuiItemEchoScreenLoadedResTable = 0;
unsigned char g_bssPad_00a41a04[4] = {0};
// clang-format on

/*
 * Loads the "ITEM/YAMABIKO" resource package for the Echo Screen item effect.
 */
// FUNCTION: FF7 0x00465b30
void __cdecl Battle_ItemFxLoadResEchoScreen(void)
{
    GfxModelTableBank *graphicsParameter;
    GfxResourceRequest requestBuffer;
    int kind;
    char pathSize[200];
    GfxResourceRequest *request;
    int ent;

    if (g_pBattle3dScene != 0 && g_YasuiItemEchoScreenLoadedResTable == 0) {
        kind = 1;
        ent = (int)&g_BattleFxEchoScreenModelLoadEntry;
        graphicsParameter = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        request = &requestBuffer;
        Gfx_InitResourceRequest(request);
        request->groupTag = 0;
        // STRING: FF7 0x007ed390
        request->resourcePath = BattleFx_BuildResourcePath("ITEM/YAMABIKO", pathSize);
        Gfx_LoadModelTableIfAbsent(kind, ent, request, &g_YasuiItemEchoScreenLoadedResTable, graphicsParameter);
    }
}

/*
 * Loads the Echo Screen item effect's resources and registers its per-target callback.
 */
// FUNCTION: FF7 0x00465bc0
void __cdecl Battle_ItemFxEnterEchoScreen(int targetMask, int actorIdx)
{
    Battle_ItemFxLoadResEchoScreen();
    Battle_ItemFxEchoScreenRegisterPerTargetCallback(targetMask, actorIdx);
}

/*
 * Registers a bit-pattern callback task to spawn Echo Screen effects per target.
 */
// FUNCTION: FF7 0x00465bda
void __cdecl Battle_ItemFxEchoScreenRegisterPerTargetCallback(int targetMask, int actorIdx)
{
    Battle_CreateBitPatternCallbackTask(targetMask, actorIdx, 0, (void *)Battle_ItemFxEchoScreenSpawnPerTarget);
}

/*
 * Allocates an emitter task for the target actor and plays the echo screen sound.
 */
// FUNCTION: FF7 0x00465bf6
void __cdecl Battle_ItemFxEchoScreenSpawnPerTarget(int actor)
{
    BattleFxEchoScreenTask *slot;

    slot = (BattleFxEchoScreenTask *)BATTLE_NEW_TASK_SLOT(Battle_ItemFxEchoScreenSpriteEmitterTask);
    slot->ownerSlot = actor;
    Battle_GetBoneWorldPos(actor, g_BattleActorRecords[actor].targetBone, slot->pos);
    slot->offsetY = g_BattleActorRecords[actor].radius * 0x1800 >> 0xC;
    slot->offsetX = slot->offsetY;
    slot->pushLength = -g_BattleActorRecords[actor].radius << 0xC >> 0xC;
    Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputeScreenPanFromPosition(slot->pos), 0x134);
}

/*
 * Emitter task spawning radial bursts of animated Echo Screen sprites around the target.
 */
// FUNCTION: FF7 0x00465cb3
void __cdecl Battle_ItemFxEchoScreenSpriteEmitterTask(void)
{
    int angle;
    int i;
    BattleFxEchoScreenTask *pEmitter;
    BattleFxEchoScreenTask *pSprite;
    int scatterX;
    int ySpread;

    pEmitter = (BattleFxEchoScreenTask *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (pEmitter->frame < 0xF) {
        for (i = 0; i < 7; i++) {
            pSprite = (BattleFxEchoScreenTask *)BATTLE_NEW_TASK_SLOT(Battle_ItemFxEchoScreenAnimatedSpriteTask);
            *(Int64Pair *)pSprite->pos = *(Int64Pair *)pEmitter->pos;
            angle = rand() & 0x7FF;
            if (angle < 0x400) {
                scatterX = SinFixed(angle) - 0x1000;
            } else {
                scatterX = 0x1000 - SinFixed(angle);
            }
            angle = scatterX;
            pSprite->offsetX = pEmitter->offsetX * angle >> 0xC;
            angle = rand() & 0x7FF;
            if (angle < 0x400) {
                ySpread = SinFixed(angle) - 0x1000;
            } else {
                ySpread = 0x1000 - SinFixed(angle);
            }
            angle = ySpread;
            pSprite->offsetY = pEmitter->offsetY * angle >> 0xC;
            pSprite->scale = rand() % 0x1000 + 0x2000;
            pSprite->pushLength = pEmitter->pushLength;
        }
    } else {
        Battle_PostActorHitOrHurtMessage(pEmitter->ownerSlot);
        pEmitter->ownerSlot = 0xFFFF;
    }
    (pEmitter->frame)++;
}

/*
 * Animates a single Echo Screen sprite with billboard matrix positioning and fading.
 */
// FUNCTION: FF7 0x00465e51
void __cdecl Battle_ItemFxEchoScreenAnimatedSpriteTask(void)
{
    BattleFxEchoScreenTask *sprite;
    PsxMatrix *matrix;

    sprite = (BattleFxEchoScreenTask *)BATTLE_CURRENT_TASK_SLOT;
    (g_BattleFxEchoScreenSpriteRequest.loadedRes) = Gfx_ModelTableGetLoadedRecord(0, g_YasuiItemEchoScreenLoadedResTable);
    g_BattleFxEchoScreenSpriteRequest.battleYamabikoSpriteObjectField = Gfx_ModelTableGetSpriteObject(0, g_YasuiItemEchoScreenLoadedResTable);
    matrix = (PsxMatrix *)BattleFx_InstallBillboardMatrix(sprite->pos, (short)sprite->scale, sprite->pushLength);
    matrix->t[0] += sprite->offsetX;
    matrix->t[1] += sprite->offsetY;
    SetTransMatrix(matrix);
    g_BattleFxEchoScreenSpriteRequest.battleYamabikoSpriteFrameIndex = sprite->frame;
    g_YasuiItemEchoScreenSpriteDrawHandle = BattleFx_SubmitSpriteDraw(&g_BattleFxEchoScreenSpriteRequest, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_YasuiItemEchoScreenSpriteDrawHandle);
    if (g_BattleIsPaused == 0 && (g_BattleFxEchoScreenSpriteRequest.loadedRes) != 0) {
        (sprite->frame)++;
        if (sprite->frame >= *(int *)((char *)*(int *)(g_BattleFxEchoScreenSpriteRequest.loadedRes) + 4)) {
            sprite->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}
} /* extern "C" */
