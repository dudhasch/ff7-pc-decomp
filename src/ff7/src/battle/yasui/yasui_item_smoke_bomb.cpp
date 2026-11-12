// src/ff7/src/battle/yasui/yasui_item_smoke_bomb.cpp
/* Item FX: Smoke Bomb (Japanese: けむりだま Kemuridama, dev tag: "ITEM/KEMURI" / Kemuri) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle effect for the Smoke Bomb item: loads its resource package, then emitters
//   scatter animated smoke sprites across the screen with the grenade sound.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/enums/sound.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_boss.h"
#include "ff7/types/battle_3d.h"
#include "ff7/battle_tasks.h"
#include "ff7/camera_script.h"
#include "lib/types/graphics.h"
#include <string.h>
#include <stdlib.h>
#include "ff7/api/battle.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "lib/codegen.h"
extern "C" {

#pragma intrinsic(memcpy)
#pragma intrinsic(memset)
#pragma intrinsic(abs)

// clang-format off

// GLOBAL: FF7 0x007ed2e0
unsigned int g_BattleFxSmokeBombModelLoadEntry[5] = {0x00192a39, (unsigned int)"SMKBALL.TIM", (unsigned int)"KEMURI.SPT", 0x00000000, 0x00000000};
// GLOBAL: FF7 0x007ed2f8
struct YasuiItemSmokeBombTransformMatrix g_BattleFxSmokeBombTransformMatrix = {{0, 0x10, 0, 0, 0, 0, 0, 0, 0, 0x10, 0, 0, 0, 0, 0, 0, 0, 0x10}, 0, 0, 0};
// GLOBAL: FF7 0x007ed318
BattleFxSpriteRequest g_BattleFxSmokeBombSpriteRequest = {0, 0, 0x80, 0x80, 0x80, 0x2e, 0, 0};
// GLOBAL: FF7 0x00a419f4
int g_YasuiItemSmokeBombSpriteDrawHandle = 0;
// GLOBAL: FF7 0x00a419f8
struct GfxTableCursor *g_YasuiItemSmokeBombLoadedResTable = 0;
// clang-format on


/*
 * Loads the "ITEM/KEMURI" resource package for the Smoke Bomb item effect.
 */
// FUNCTION: FF7 0x004658a0
void __cdecl Battle_ItemFxLoadResSmokeBomb(void)
{
    GfxModelTableBank *graphicsParameter;
    GfxResourceRequest requestBuffer;
    int kind;
    char pathSize[200];
    GfxResourceRequest *request;
    int ent;

    if (g_pBattle3dScene != 0 && g_YasuiItemSmokeBombLoadedResTable == 0) {
        kind = 1;
        ent = (int)&g_BattleFxSmokeBombModelLoadEntry;
        graphicsParameter = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        request = &requestBuffer;
        Gfx_InitResourceRequest(request);
        request->groupTag = 0;
        // STRING: FF7 0x007ed340
        request->resourcePath = (char *)BattleFx_BuildResourcePath("ITEM/KEMURI", pathSize);
        Gfx_LoadModelTableIfAbsent(kind, ent, request, &g_YasuiItemSmokeBombLoadedResTable, graphicsParameter);
    }
}

/*
 * Entry point for Smoke Bomb item effect; loads resources and spawns emitter.
 */
// FUNCTION: FF7 0x00465930
void __cdecl Battle_ItemFxEnterSmokeBomb(int targetMask, int actorIdx)
{
    Battle_ItemFxLoadResSmokeBomb();
    Battle_ItemFxSmokeBombSpawnEmitterWithSound(targetMask, actorIdx);
}

/*
 * Spawns smoke emitter task slot and dispatches smoke grenade sound effect.
 */
// FUNCTION: FF7 0x0046594a
void __cdecl Battle_ItemFxSmokeBombSpawnEmitterWithSound(int targetMask, int actorIdx)
{
    Battle_AllocateResourceSlot(Battle_ItemFxSmokeBombSpriteEmitterTask);
    Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, 0x40, 0x261);
}

/*
 * Smoke emitter task that spawns random animated smoke sprites across the screen.
 */
// FUNCTION: FF7 0x0046596d
void __cdecl Battle_ItemFxSmokeBombSpriteEmitterTask(void)
{
    int i;
    BattleFxSmokeBombEmitterCounter *pEmitter;
    BattleItemFxSmokeBombSpriteTask *smoke;

    pEmitter = (BattleFxSmokeBombEmitterCounter *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (pEmitter->count < 30) {
        for (i = 0; i < 7; i++) {
            smoke = (BattleItemFxSmokeBombSpriteTask *)BATTLE_NEW_TASK_SLOT(Battle_ItemFxSmokeBombAnimatedSpriteTask);
            smoke->offsetX = rand() % 320 - 160;
            smoke->offsetY = rand() % 92 - 120;
        }
    } else {
        pEmitter->ownerSlot = BATTLE_TASK_FREE;
    }
    (pEmitter->count)++;
}

/*
 * Animates a smoke sprite billboard with screen-depth translation and frame cycling.
 */
// FUNCTION: FF7 0x00465a29
void __cdecl Battle_ItemFxSmokeBombAnimatedSpriteTask(void)
{
    BattleFxSmokeBombSprite *task;

    task = (BattleFxSmokeBombSprite *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleFxSmokeBombSpriteRequest.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(0, g_YasuiItemSmokeBombLoadedResTable);
    g_BattleFxSmokeBombSpriteRequest.drawObj = Gfx_ModelTableGetSpriteObject(0, g_YasuiItemSmokeBombLoadedResTable);
    g_BattleFxSmokeBombTransformMatrix.battleKemuriTranslationX = task->translationX << 2;
    g_BattleFxSmokeBombTransformMatrix.battleKemuriTranslationY = task->translationY << 2;
    g_BattleFxSmokeBombTransformMatrix.battleKemuriTranslationDepth = GetGeomScreen() << 2;
    SetRotMatrix((int *)&g_BattleFxSmokeBombTransformMatrix);
    SetTransMatrix((int *)&g_BattleFxSmokeBombTransformMatrix);
    g_BattleFxSmokeBombSpriteRequest.frame = task->frame;
    g_YasuiItemSmokeBombSpriteDrawHandle = BattleFx_SubmitSpriteDraw(&g_BattleFxSmokeBombSpriteRequest, g_BattleCurrentOrderingTable + BATTLE_OT_AFTER_SORTED_5, 0, g_YasuiItemSmokeBombSpriteDrawHandle);
    if (g_BattleIsPaused == 0 && g_BattleFxSmokeBombSpriteRequest.spriteGroup != 0) {
        (task->frame)++;
        if (task->frame >= g_BattleFxSmokeBombSpriteRequest.spriteGroup->header->frameCount) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}
} /* extern "C" */
