// src/ff7/src/battle/battle_fx_special_poison_breath.cpp
/* Enemy Special Effect [idx 9, 122]: PoisonBreath (JP: dokufuki / 毒吹き (Poison Breath/Spray), asset: "SPECIAL/DOKUFUKI") — Poison Breath (0x1e6, Ark Dragon [idx 122]), Poison Storm (0x394, Schizo [idx 9]), Piazzo Shower (0x18a [idx 9]), Poison Blow (0x226 [idx 122]), Bio Gas (0x38e [idx 122]) */
// Contents: Battle special effect for the Dokufuki attack: it loads resources and spawns a
//   per-target timeline that drives variant, drifting and spray-emitting sprites, with a loud entry
//   point that also dispatches the sound effect.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/api/battle.h"
#include "ff7/battle_tasks.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "lib/api/sound.h"
#include "ff7/types/battle.h"
#include "ff7/camera_script.h"
#include "lib/types/graphics.h"
#include <string.h>
#include "lib/enums/sound.h"
#include <stdlib.h>
#include "lib/codegen.h"
extern "C" {

// clang-format off
// GLOBAL: FF7 0x008a5070
int g_BattleSpecialPoisonBreathResourceDefs = 0x00192A38;
int g_BattleSpecialPoisonBreathResourceDefsHeadPtr[1] = {
    (int)((char *)&g_BattleFxSpecialPoisonBreathResourceNames + 0x0)
};
int g_BattleSpecialPoisonBreathResourceDefsDataTail[13] = {
    (int)((char *)&g_BattleFxSpecialPoisonBreathResourceNames + 0xc), 0x0, 0x0, 0x192a38, (int)((char *)&g_BattleFxSpecialPoisonBreathResourceNames + 0x18), (int)((char *)&g_BattleFxSpecialPoisonBreathResourceNames + 0x24),
    0x0, 0x0, 0x192a39, (int)((char *)&g_BattleFxSpecialPoisonBreathResourceNames + 0x30), (int)((char *)&g_BattleFxSpecialPoisonBreathResourceNames + 0x3c), 0x0,
    0x0
};
// GLOBAL: FF7 0x008a50ac
short g_BattleSpecialPoisonBreathCount = 6;
// GLOBAL: FF7 0x008a50b0
struct BattleSpecialPoisonBreathColor1 g_BattleSpecialPoisonBreathColor1 = {0, 0, 0x80, 0x80, 0x80, {0x2c}, 0, {0}};
#undef g_BattleSpecialPoisonBreathColor2
// GLOBAL: FF7 0x008a50c0
struct BattleSpecialLiquidPoisonColor2 g_BattleSpecialPoisonBreathColor2 = {0, 0, {0x20, 0x20, 0x20, 0x2c}, 0, {0}};
unsigned char g_BattleFxSpecialPoisonBreathResourceNames[72] =
    "DOKU00.TIM\0\0"
    "DOKU1.SPT\0\0\0"
    "DOKU01.TIM\0\0"
    "BAKU1.SPT\0\0\0"
    "DOKU02.TIM\0\0"
    "HIT1.SPT";
// clang-format on
#pragma intrinsic(memset)
#pragma intrinsic(memcpy)

// clang-format off
// clang-format on


#define g_BattleActorBoneIndex (*(int *)((char *)&g_BattleActorRecords + 0x2b))

// clang-format off
// GLOBAL: FF7 0x00b6e0a4
int g_BattleSpecialPoisonBreathDrawListHandle = 0;
// GLOBAL: FF7 0x00b6e0a8
struct GfxTableCursor *g_BattleSpecialPoisonBreathLoadedRes = 0;
// clang-format on

/*
 * Loads resources for the Dokufuki special effect when not already loaded.
 */
// FUNCTION: FF7 0x00548ac0
void __cdecl BattleFx_SpecialLoadResPoisonBreath(void)
{
    GfxModelTableBank *unitOwner;
    GfxResourceRequest requestSlot;
    int resCategory;
    char pathWorkBuffer[200];
    GfxResourceRequest *pRequest;
    int resourceDefsWord;

    if (g_pBattle3dScene != 0 && g_BattleSpecialPoisonBreathLoadedRes == 0) {
        resCategory = 3;
        resourceDefsWord = (int)&g_BattleSpecialPoisonBreathResourceDefs;
        unitOwner = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRequest = &requestSlot;
        Gfx_InitResourceRequest(pRequest);
        pRequest->groupTag = 0;
        // STRING: FF7 0x008a5118
        pRequest->resourcePath = BattleFx_BuildResourcePath("SPECIAL/DOKUFUKI", pathWorkBuffer);
        Gfx_LoadModelTableIfAbsent(resCategory, resourceDefsWord, pRequest, &g_BattleSpecialPoisonBreathLoadedRes, unitOwner);
    }
}

/*
 * Entry point for the Dokufuki special effect, dispatching to begin.
 */
// FUNCTION: FF7 0x00548b50
void __cdecl BattleFx_SpecialEnterPoisonBreath(int targetMask, int actorIdx)
{
    BattleFx_SpecialLoadResPoisonBreath();
    SfxLoadIntoBuffer(0x6B, 0);
    SfxLoadIntoBuffer(0xB2, 0);
    g_BattleSpecialPoisonBreathCount = 6;
    Battle_CreateBitPatternCallbackTask(targetMask, actorIdx, 4, BattleFx_SpecialSpawnPoisonBreathPerTarget);
    Battle_PlaySfxPannedToActor(0x6B, (short)actorIdx, 0);
    SoundDispatchCommand(SOUND_SFX_PLAY_CH1, 0x40, 0xB2, 0, 0, 0, 0, 0, 0);
    Sfx_ReleaseCachedBuffer(0x6B, 0xF);
    Sfx_ReleaseCachedBuffer(0xB2, 0xF);
}

/*
 * Spawns Dokufuki effect instances for each targeted actor.
 */
// FUNCTION: FF7 0x00548bde
void __cdecl BattleFx_SpecialSpawnPoisonBreathPerTarget(int actor, int actorId)
{
    int p; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxLiquidPoisonTimeline *taskRecord;

    taskRecord = (BattleFxLiquidPoisonTimeline *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialPoisonBreathTimelineTask);
    Battle_GetBoneWorldPos(actorId, *(char *)((char *)&g_BattleActorBoneIndex + (actorId * 0x1AEC + g_BattleSpecialPoisonBreathCount)), &taskRecord->startX);
    Battle_GetBoneWorldPos(actor, g_BattleActorRecords[actor].targetBone, &taskRecord->endX);
    taskRecord->actorIndex = actor;
}

/*
 * Timeline task controlling state progression for the Dokufuki special effect.
 */
// FUNCTION: FF7 0x00548c5e
void __cdecl BattleFx_SpecialPoisonBreathTimelineTask(void)
{
    short dz;
    BattleFxLiquidPoisonTimeline *pCur;
    int r; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxLiquidPoisonParticleRecord *rawParticle;
    int t; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int u; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int v; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int w; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int x; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int y; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int z; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    short dx9;
    short theta;
    int c; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int d; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int e; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int f; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int g; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */

    pCur = (BattleFxLiquidPoisonTimeline *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (pCur->frame == 0) {
        rawParticle = (BattleFxLiquidPoisonParticleRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialPoisonBreathEmitSpriteTask);
        rawParticle->angle = ArcTangentFixed(pCur->endX - pCur->startX, pCur->endZ - pCur->startZ);
        rawParticle->posX = pCur->startX;
        rawParticle->posY = pCur->startY;
        rawParticle->posZ = pCur->startZ;
        dx9 = pCur->endX - rawParticle->posX;
        dz = pCur->endZ - rawParticle->posZ;
        rawParticle->distance = SqrtToInt(dx9 * dx9 + dz * dz) / 0x18;
    }
    if (pCur->frame == 0x32) {
        Battle_PostActorHitOrHurtMessage(pCur->actorIndex);
        SoundDispatchCommand(SOUND_SFX_PLAY_CH2, 0x40, 0x95, 0, 0, 0, 0, 0, 0);
    }
    if (pCur->frame > 0x28 && pCur->frame < 0x30) {
        theta = (pCur->frame - 0x28 << 9) - 0x400;
        rawParticle = (BattleFxLiquidPoisonParticleRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialPoisonBreathVariantSpriteTask);
        Battle_GetBoneWorldPos(pCur->actorIndex, g_BattleActorRecords[pCur->actorIndex].targetBone, &rawParticle->posX);
        rawParticle->posX += (short)(CosFixed(theta) * 0x320 >> 0xC);
        rawParticle->posY += (short)(SinFixed(theta) * 0x320 >> 0xC);
        rawParticle->active = 1;
        rawParticle = (BattleFxLiquidPoisonParticleRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialPoisonBreathVariantSpriteTask);
        Battle_GetBoneWorldPos(pCur->actorIndex, g_BattleActorRecords[pCur->actorIndex].targetBone, &rawParticle->posX);
        rawParticle->posX += (short)(rand() % 0x258 - 0x12C);
        rawParticle->posY += (short)(rand() % 0x258 - 0x12C);
        rawParticle->posZ += (short)(rand() % 0x258 - 0x12C);
        rawParticle->active = 0;
    }
    (pCur->frame)++;
    if (pCur->frame == 0x3C) {
        pCur->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Task to animate and render variant sprites for the Dokufuki effect.
 */
// FUNCTION: FF7 0x00548f5a
void __cdecl BattleFx_SpecialPoisonBreathVariantSpriteTask(void)
{
    struct {
        int matrix;
        int table[2];
        BattleFxWaterGunSpriteTask *slot;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    frame.slot = (BattleFxWaterGunSpriteTask *)BATTLE_CURRENT_TASK_SLOT;
    frame.table[0] = 1;
    frame.table[1] = 2;
    frame.matrix = (int)BattleFx_InstallBillboardMatrix(&frame.slot->posX, (frame.slot->variant != 0 ? 0x1000 : 0) + 0x2000, 0);
    g_BattleSpecialPoisonBreathColor1.variantShort12 = frame.slot->frameIndex;
    g_BattleSpecialPoisonBreathColor1.variantByte10 = (frame.slot->variant - 4 != 0 ? 0x20 : 0) + 0x40;
    g_BattleSpecialPoisonBreathColor1.variantByte9 = g_BattleSpecialPoisonBreathColor1.variantByte10;
    g_BattleSpecialPoisonBreathColor1.variantByte8 = g_BattleSpecialPoisonBreathColor1.variantByte9;
    (g_BattleSpecialPoisonBreathColor1.spriteGroup) = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(frame.table[frame.slot->variant], g_BattleSpecialPoisonBreathLoadedRes);
    g_BattleSpecialPoisonBreathColor1.drawObj = Gfx_ModelTableGetSpriteObject(frame.table[frame.slot->variant], g_BattleSpecialPoisonBreathLoadedRes);
    if (frame.slot->variant == 0 && ((PsxMatrix *)frame.matrix)->t[2] > 0) {
        g_BattleSpecialPoisonBreathDrawListHandle = BattleFx_SubmitSpriteDraw(&(g_BattleSpecialPoisonBreathColor1.spriteGroup), g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialPoisonBreathDrawListHandle);
    } else {
        g_BattleSpecialPoisonBreathDrawListHandle = BattleFx_SubmitSpriteDraw(&(g_BattleSpecialPoisonBreathColor1.spriteGroup), g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialPoisonBreathDrawListHandle);
    }
    if (g_BattleIsPaused != 0)
        return;
    if ((g_BattleSpecialPoisonBreathColor1.spriteGroup) != 0) {
        (frame.slot->frameIndex)++;
        if (frame.slot->frameIndex >= g_BattleSpecialPoisonBreathColor1.spriteGroup->header->frameCount) {
            frame.slot->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Task to emit and animate spray sprites for the Dokufuki special effect.
 */
// FUNCTION: FF7 0x005490d1
void __cdecl BattleFx_SpecialPoisonBreathEmitSpriteTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[60];
        SVECTOR angleX;
        struct { unsigned char pad_to_8[0x8]; int projected; };
        struct { unsigned char pad_to_C[0xC]; SVECTOR pointX; };
        struct { unsigned char pad_to_14[0x14]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_34[0x34]; BattleFxNapalmTrailSlot *driftSlot; };
        struct { unsigned char pad_to_38[0x38]; BattleFxPoisonBreathEmit *emitSlot; };
    } frame;
#pragma pack(pop)

    frame.emitSlot = (BattleFxPoisonBreathEmit *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.driftSlot = (BattleFxNapalmTrailSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialPoisonBreathDriftingAnimatedSpriteTask);
    frame.driftSlot->posX = frame.emitSlot->posX;
    frame.driftSlot->posY = frame.emitSlot->posY;
    frame.driftSlot->posZ = frame.emitSlot->posZ;
    frame.driftSlot->variant = frame.emitSlot->step >> 1;
    frame.pointX.vx = 0;
    frame.pointX.vy = 0;
    frame.pointX.vz = frame.emitSlot->speed;
    frame.angleX.vx = 0;
    frame.angleX.vy = -SinFixed(frame.emitSlot->step * 0x88) / 16;
    frame.angleX.vz = 0x200;
    RotMatrixZYX(&frame.angleX, &frame.rotMatrix);
    RotateMatrixY(frame.emitSlot->angleY, &frame.rotMatrix);
    frame.rotMatrix.t[2] = 0;
    frame.rotMatrix.t[1] = frame.rotMatrix.t[2];
    frame.rotMatrix.t[0] = frame.rotMatrix.t[1];
    SetRotMatrix(&frame.rotMatrix);
    SetTransMatrix(&frame.rotMatrix);
    RotTransPointToInt16(&frame.pointX, &frame.driftSlot->velocityX, &frame.projected);
    (frame.emitSlot->step)++;
    if (frame.emitSlot->step == 0x20) {
        frame.emitSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Task to animate drifting sprites for the Dokufuki special effect.
 */
// FUNCTION: FF7 0x00549228
void __cdecl BattleFx_SpecialPoisonBreathDriftingAnimatedSpriteTask(void)
{
    int p; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int size;
    int ceiling;
    int entry;
    BattleFxLiquidPoisonDriftingAnimatedSprite *task;
    int matrix;

    task = (BattleFxLiquidPoisonDriftingAnimatedSprite *)BATTLE_CURRENT_TASK_SLOT;
    (g_BattleSpecialPoisonBreathColor2.resource) = Gfx_ModelTableGetLoadedRecord(0, g_BattleSpecialPoisonBreathLoadedRes);
    g_BattleSpecialPoisonBreathColor2.drawObj = Gfx_ModelTableGetSpriteObject(0, g_BattleSpecialPoisonBreathLoadedRes);
    entry = task->unusedParam;
    size = 0x4000;
    ceiling = size;
    matrix = (int)BattleFx_InstallBillboardMatrix(&task->posX, 0x1800, 0);
    g_BattleSpecialPoisonBreathColor2.driftSlot = task->slotIndex;
    if (((PsxMatrix *)matrix)->t[2] > 0) {
        g_BattleSpecialPoisonBreathDrawListHandle = BattleFx_SubmitSpriteDraw(&g_BattleSpecialPoisonBreathColor2, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialPoisonBreathDrawListHandle);
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    task->posX += task->velocityX;
    task->posY += task->velocityY;
    task->posZ += task->velocityZ;
    if ((g_BattleSpecialPoisonBreathColor2.resource) != 0) {
        (task->frame)++;
        if (task->frame >= *(int *)((char *)*(int *)(g_BattleSpecialPoisonBreathColor2.resource) + 4)) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Alternate entry point for Dokufuki with sound effect dispatch.
 */
// FUNCTION: FF7 0x0054935e
void __cdecl BattleFx_SpecialEnterPoisonBreathLoud(int targetMask, int actorIdx)
{
    g_BattleSpecialPoisonBreathCount = 8;
    BattleFx_SpecialLoadResPoisonBreath();
    SfxLoadIntoBuffer(0x6B, 0);
    SfxLoadIntoBuffer(0xB2, 0);
    Battle_CreateBitPatternCallbackTask(targetMask, actorIdx, 4, BattleFx_SpecialSpawnPoisonBreathPerTarget);
    Battle_PlaySfxPannedToActor(0x6B, (short)actorIdx, 0);
    SoundDispatchCommand(SOUND_SFX_PLAY_CH1, 0x40, 0xB2, 0, 0, 0, 0, 0, 0);
    Sfx_ReleaseCachedBuffer(0x6B, 0xF);
    Sfx_ReleaseCachedBuffer(0xB2, 0xF);
}
} /* extern "C" */
