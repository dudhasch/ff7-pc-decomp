// src/ff7/src/battle/battle_fx_special_full_explosion_effect.cpp
/* Enemy Special Attack [93]: Full-Party Explosion (orig. special/zenbaku, 全体爆発) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle special effect 'Zenbaku': a setup task and timeline with pooled sprite entry
//   tasks.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/enums/sound.h"
#include "ff7/api/battle.h"
#include "ff7/battle_tasks.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_special.h"
#include "ff7/camera_script.h"
#include "lib/types/graphics.h"
#include <string.h>
#include <stdlib.h>
#include "lib/codegen.h"
extern "C" {

// clang-format off
// GLOBAL: FF7 0x008a0fe8
int g_BattleSpecFullExplosionResourceDefs = 0x00192A39;
int g_BattleSpecFullExplosionResourceDefsHeadPtr[1] = {
    (int)((char *)&g_BattleFxSpecialFullExplosionEffectResourceNames + 0x0)
};
int g_BattleSpecFullExplosionResourceDefsDataTail[8] = {
    (int)((char *)&g_BattleFxSpecialFullExplosionEffectResourceNames + 0xc), 0x0, 0x0, 0x192a39, (int)((char *)&g_BattleFxSpecialFullExplosionEffectResourceNames + 0x18), (int)((char *)&g_BattleFxSpecialFullExplosionEffectResourceNames + 0x24),
    0x0, 0x0
};
// The first 16-byte sprite record the code fills and submits by number.
// GLOBAL: FF7 0x008a1010
struct BattleFxSpriteRequest g_BattleSpecFullExplosionSpriteReq1 = {0, 0, -128, -128, -128, 0x2e, 0, 0};
// The second 16-byte sprite record the code fills and submits by number.
// GLOBAL: FF7 0x008a1020
struct BattleFxSpriteRequest g_BattleSpecFullExplosionSpriteReq2 = {0, 0, -128, -128, -128, 0x2e, 0, 0};
unsigned char g_BattleFxSpecialFullExplosionEffectResourceNames[48] =
    "BAKU1.TIM\0\0\0"
    "BAKU_1.SPT\0\0"
    "SMOKE_1.TIM\0"
    "SMOKE_1.SPT";
// clang-format on
#pragma intrinsic(memset)
#pragma intrinsic(memcpy)


// clang-format off
// GLOBAL: FF7 0x00b5f4c8
int g_BattleSpecFullExplosionSourceActor = 0;
unsigned char g_bssPad_00b5f4cc[4] = {0};
// GLOBAL: FF7 0x00b5f4d0
BattleFxDiamondCardBillboard g_BattleSpecFullExplosionExplosionCoords[444] = {0};
// GLOBAL: FF7 0x00b602b0
int g_BattleSpecFullExplosionTargetMask = 0;
// GLOBAL: FF7 0x00b602b4
struct GfxTableCursor *g_BattleSpecFullExplosionLoadedResTable = 0;
// clang-format on

/*
 * BattleFx_SpecialLoadResFullExplosion.
 *
 * When the engine context exists and g_BattleSpecFullExplosionLoadedResTable is empty, builds special/zenbaku with a default-
 * duration record and creates resource type 2 using g_BattleSpecFullExplosionResourceDefs.
 */
// FUNCTION: FF7 0x005305f0
void __cdecl BattleFx_SpecialLoadResFullExplosion(void)
{
    GfxModelTableBank *stage;
    GfxResourceRequest requestSlot;
    int kind;
    char pathText[200];
    GfxResourceRequest *pRequest;
    int resourceTable;

    if (g_pBattle3dScene != 0 && g_BattleSpecFullExplosionLoadedResTable == 0) {
        kind = 2;
        resourceTable = (int)&g_BattleSpecFullExplosionResourceDefs;
        stage = (GfxModelTableBank *)g_pBattle3dScene->modelTableBank;
        pRequest = &requestSlot;
        Gfx_InitResourceRequest(pRequest);
        ((BattleFxDiamondCardResource *)pRequest)->groupTag = 0;
        // STRING: FF7 0x008a1060
        ((BattleFxDiamondCardResource *)pRequest)->resourcePath = (int)BattleFx_BuildResourcePath("special/zenbaku", pathText);
        Gfx_LoadModelTableIfAbsent(kind, resourceTable, pRequest, &g_BattleSpecFullExplosionLoadedResTable, stage);
    }
}

/*
 * BattleFx_SpecialEnterFullExplosion.
 *
 * Mechanical entry point: calls BattleFx_SpecialLoadResFullExplosion with no arguments, then forwards its own two
 * arguments to BattleFx_SpecialBeginFullExplosion.
 * Calls BattleFx_SpecialLoadResFullExplosion, then BattleFx_SpecialBeginFullExplosion with both arguments.
 */
// FUNCTION: FF7 0x00530680
void __cdecl BattleFx_SpecialEnterFullExplosion(int targetMask, int actorIdx)
{
    BattleFx_SpecialLoadResFullExplosion();
    BattleFx_SpecialBeginFullExplosion(targetMask, actorIdx);
}

/*
 * BattleFx_SpecialBeginFullExplosion.
 *
 * Stores the allocated BattleFx_SelectDrawListBuffer slot address in g_BattleFxDrawListBufferTaskSlotPtr, allocates a
 * slot for BattleFx_SpecialFullExplosionSetupTask, stores targetMask in g_BattleSpecFullExplosionTargetMask, and clears g_BattleSpecFullExplosionSourceActor.
 */
// FUNCTION: FF7 0x0053069a
void __cdecl BattleFx_SpecialBeginFullExplosion(int targetMask, int actorIdx)
{
    *(int *)&g_BattleFxDrawListBufferTaskSlotPtr = (int)BATTLE_NEW_TASK_SLOT(BattleFx_SelectDrawListBuffer);
    Battle_AllocateResourceSlot(BattleFx_SpecialFullExplosionSetupTask);
    g_BattleSpecFullExplosionTargetMask = targetMask;
    g_BattleSpecFullExplosionSourceActor = 0;
}

// Retires the current slot with 0xFFFF, then allocates a BattleFx_SpecialFullExplosionTimelineTask slot
// whose position at +4 is the centroid of g_BattleSpecFullExplosionTargetMask. launchSpeed is 0xFAC0
// (-1344 as a short) and radius is 0x9A0, with diameter twice the radius. velocity is the negated
// half of launchSpeed, scaledVelocity is three times that, and velocity is then doubled. spriteScale is
// 0x380.
// FUNCTION: FF7 0x005306dc
void __cdecl BattleFx_SpecialFullExplosionSetupTask(void)
{
    BattleFxFullExplosionSlot *task;

    task = (BattleFxFullExplosionSlot *)BATTLE_CURRENT_TASK_SLOT;
    task->ownerSlot = BATTLE_TASK_FREE;
    task = (BattleFxFullExplosionSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialFullExplosionTimelineTask);
    Battle_GetActorMaskCentroid(g_BattleSpecFullExplosionTargetMask, (short *)((char *)task + 4));
    task->launchSpeed = -1344;
    task->radius = 2464;
    task->diameter = task->radius << 1;
    task->velocity = -(task->launchSpeed >> 1);
    task->scaledVelocity = task->velocity * 2 + task->velocity;
    task->velocity += task->velocity;
    task->spriteScale = 0x380;
}

/*
 * BattleFx_SpecialFullExplosionTimelineTask implementation.
 */
// FUNCTION: FF7 0x0053079a
void __cdecl BattleFx_SpecialFullExplosionTimelineTask(void)
{
    int fraction;
    int puffIndex;
    int cursor;
    int sizeRoll;
    int puffCount;
    struct BattleTaskSlot *pSprite;
    BattleFxFullExplosionExplosionState *explosion;

    if (g_BattleIsPaused != 0) {
        return;
    }
    explosion = (BattleFxFullExplosionExplosionState *)BATTLE_CURRENT_TASK_SLOT;
    if (explosion->step > 1 && explosion->step < 37) {
        cursor = g_BattleSpecFullExplosionSourceActor;
        pSprite = BATTLE_NEW_TASK_SLOT(BattleFx_SpecialFullExplosionPooledSpriteEntry0Task);
        ((BattleFxBerserkFadeSprite *)pSprite)->ringIndex = cursor;
        fraction = (explosion->timer << 12) / 180;
        puffCount = (rand() & 1) + 1;
        for (puffIndex = 0; puffIndex < puffCount; puffIndex++) {
            sizeRoll = rand() % 0x1400 + 0x800;
            g_BattleSpecFullExplosionExplosionCoords[cursor].posX = explosion->baseX + rand() % explosion->jitterRange - explosion->jitterOffset;
            g_BattleSpecFullExplosionExplosionCoords[cursor].posY = explosion->baseY - rand() % explosion->liftRange + explosion->liftOffset;
            g_BattleSpecFullExplosionExplosionCoords[cursor].posZ = explosion->baseZ + rand() % explosion->jitterRange - explosion->jitterOffset;
            g_BattleSpecFullExplosionExplosionCoords[cursor].scale = fraction * sizeRoll >> 12;
            cursor++;
            if (cursor >= 444) {
                cursor = 0;
            }
        }
        pSprite = BATTLE_NEW_TASK_SLOT(BattleFx_SpecialFullExplosionPooledSpriteEntry1Task);
        ((BattleFxBerserkFadeSprite *)pSprite)->ringIndex = cursor;
        fraction = (explosion->timer << 12) / 200;
        puffCount = (rand() & 1) + 1;
        for (puffIndex = 0; puffIndex < puffCount; puffIndex++) {
            sizeRoll = rand() % 0x1100 + 0x700;
            g_BattleSpecFullExplosionExplosionCoords[cursor].posX = explosion->baseX + rand() % explosion->jitterRange - explosion->jitterOffset;
            g_BattleSpecFullExplosionExplosionCoords[cursor].posY = explosion->baseY - rand() % explosion->liftRange + explosion->liftOffset;
            g_BattleSpecFullExplosionExplosionCoords[cursor].posZ = explosion->baseZ + rand() % explosion->jitterRange - explosion->jitterOffset;
            g_BattleSpecFullExplosionExplosionCoords[cursor].scale = fraction * sizeRoll >> 12;
            cursor++;
            if (cursor >= 444) {
                cursor = 0;
            }
        }
        g_BattleSpecFullExplosionSourceActor = cursor;
        if ((explosion->step & 1) == 0) {
            Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputeScreenPanFromPosition((char *)explosion + 4), 0x7C);
        }
    }
    if (explosion->step == 44) {
        for (puffIndex = 0; puffIndex < 10; puffIndex++) {
            if ((g_BattleSpecFullExplosionTargetMask & 1 << puffIndex) != 0) {
                Battle_PostActorHitOrHurtMessage(puffIndex);
            }
        }
    }
    if (explosion->step >= 52) {
        explosion->ownerSlot = BATTLE_TASK_FREE;
    }
    (explosion->step)++;
}

/*
 * BattleFx_SpecialFullExplosionPooledSpriteEntry0Task.
 *
 * Per-frame handler for the current battle-effect slot at g_BattleHitLivewireEmitterRecords +
 * g_BattleActiveTaskSlotIdx*0x20. Copies the frame counter at +2 into *(short*)0x8A101C, loads
 * *(g_BattleSpecFullExplosionSpriteReq1, 0x8A1010)/*(0x8A1014) from Gfx_ModelTableGetLoadedRecord/4Alt(0,
 * *(int*)g_BattleSpecFullExplosionLoadedResTable), and for the single iteration of a 0..0 loop installs a
 * billboard matrix for the 8-byte record k = slot+0x18 at g_BattleSpecFullExplosionExplosionCoords
 * (0xB5F4D0) (scale from its short at +6, forward offset -(scale>>4)), submits the sprite at
 * 0x8A1010 into g_BattleFxActiveDrawListPtr through g_BattleCurrentOrderingTable+0x70 with shift
 * 0xC, and steps k modulo 0x1BC. Retires the slot with 0xFFFF once the counter reaches 0xF, and
 * increments the counter unless g_BattleIsPaused is set.
 */
// FUNCTION: FF7 0x00530afc
void __cdecl BattleFx_SpecialFullExplosionPooledSpriteEntry0Task(void)
{
    int loopIndex, entry;
    BattleFxBerserkFadeSprite *pSprite;

    pSprite = (BattleFxBerserkFadeSprite *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecFullExplosionSpriteReq1.frame = pSprite->frame;
    entry = pSprite->ringIndex;
    g_BattleSpecFullExplosionSpriteReq1.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(0, g_BattleSpecFullExplosionLoadedResTable);
    g_BattleSpecFullExplosionSpriteReq1.drawObj = Gfx_ModelTableGetSpriteObject(0, g_BattleSpecFullExplosionLoadedResTable);
    for (loopIndex = 0; loopIndex < 1; loopIndex++) {
        BattleFx_InstallBillboardMatrix(&g_BattleSpecFullExplosionExplosionCoords[entry].posX, g_BattleSpecFullExplosionExplosionCoords[entry].scale,
                                        -(g_BattleSpecFullExplosionExplosionCoords[entry].scale >> 4));
        g_BattleFxActiveDrawListPtr = BattleFx_SubmitSpriteDraw(&g_BattleSpecFullExplosionSpriteReq1, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
        entry++;
        if (entry >= 444) {
            entry = 0;
        }
    }
    if (pSprite->frame >= 15) {
        pSprite->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    (pSprite->frame)++;
}

/*
 * BattleFx_SpecialFullExplosionPooledSpriteEntry1Task.
 *
 * Per-frame draw for the record at (g_BattleActiveTaskSlotIdx << 5) +
 * g_BattleHitLivewireEmitterRecords. Copies the counter at +2 into the short at 0x8A102C, loads
 * g_BattleSpecFullExplosionSpriteReq2 (0x8A1020)/0x8A1024 from entry 1 of group
 * g_BattleSpecFullExplosionLoadedResTable (0xB602B4) (Gfx_ModelTableGetLoadedRecord /
 * Gfx_ModelTableGetSpriteObject), and, for a single iteration, installs a billboard from the 8-byte
 * position at g_BattleSpecFullExplosionExplosionCoords (0xB5F4D0) + n*8 (scale = its short at +6, forward
 * offset = minus that short >> 4) and submits the sprite at 0x8A1020 into
 * g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED at shift 0xC through g_BattleFxActiveDrawListPtr, stepping
 * n modulo 0x1BC. Retires the record with 0xFFFF at +0 at or above counter 7; unless
 * g_BattleIsPaused is set, increments the counter.
 */
// FUNCTION: FF7 0x00530c16
void __cdecl BattleFx_SpecialFullExplosionPooledSpriteEntry1Task(void)
{
    int loopIndex, entry;
    BattleFxBerserkFadeSprite *pSprite;

    pSprite = (BattleFxBerserkFadeSprite *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecFullExplosionSpriteReq2.frame = pSprite->frame;
    entry = pSprite->ringIndex;
    g_BattleSpecFullExplosionSpriteReq2.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(1, g_BattleSpecFullExplosionLoadedResTable);
    g_BattleSpecFullExplosionSpriteReq2.drawObj = Gfx_ModelTableGetSpriteObject(1, g_BattleSpecFullExplosionLoadedResTable);
    for (loopIndex = 0; loopIndex < 1; loopIndex++) {
        BattleFx_InstallBillboardMatrix(&g_BattleSpecFullExplosionExplosionCoords[entry].posX, g_BattleSpecFullExplosionExplosionCoords[entry].scale, -(g_BattleSpecFullExplosionExplosionCoords[entry].scale >> 4));
        g_BattleFxActiveDrawListPtr = BattleFx_SubmitSpriteDraw(&g_BattleSpecFullExplosionSpriteReq2, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
        entry++;
        if (entry >= 444) {
            entry = 0;
        }
    }
    if (pSprite->frame >= 7) {
        pSprite->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    (pSprite->frame)++;
}
} /* extern "C" */
