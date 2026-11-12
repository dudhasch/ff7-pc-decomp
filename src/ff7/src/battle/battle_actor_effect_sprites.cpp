// src/ff7/src/battle/battle_actor_effect_sprites.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle actor and world effect tasks: runs the scaling ramp sprites that follow an
//   actor's bones, plus world-space billboard sprites and delayed sprites positioned relative to an
//   actor.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/battle_tasks.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_actors.h"
#include "lib/types/graphics.h"
#include <stdlib.h>
#include "ff7/camera_script.h"
#include <string.h>
#include "ff7/api/battle.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "lib/api/file.h"
#include "lib/api/movie.h"
#include "lib/codegen.h"
extern "C" {


#pragma intrinsic(abs)

// clang-format off
// The sprite quad the code submits (16 B) and the int after it that the code writes by number (0x8FE270), with its pad.
// GLOBAL: FF7 0x008fe260
unsigned char g_BattleActorScale_SpriteQuadData[24] = {0xf0, 0xff, 0xe0, 0xff, 0x00, 0xa0, 0x1f, 0x1f, 0x80, 0x80, 0x80, 0x2e, 0x3a, 0x00, 0x07, 0x7a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// The MATRIX the code fills and passes to SetRotMatrix/SetTransMatrix by number; its m[2][2] is 0x1000.
// GLOBAL: FF7 0x008fe278
unsigned char g_BattleActorScale_TransformMatrix[32] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x008fe298
BattleWorldSprite_DrawReq g_BattleWorldSprite_DrawReq = {{0xf0, 0xff, 0xe0, 0xff}, 0, {0xa0, 0x1f, 0x1f, 0x80, 0x80, 0x80, 0x2e, 0x3a, 0, 7, 0x7a}, 0};
// clang-format on
#pragma intrinsic(memset)
#pragma intrinsic(memcpy)


/*
 * Spawns an actor scale ramp emitter task.
 */
// FUNCTION: FF7 0x005be490
void __cdecl Battle_SpawnActorScaleRampTask(int actorIndex)
{
    BattleActorScaleRamp *effect;

    effect = (BattleActorScaleRamp *)(&g_BattleTaskSlotBuffer.secondarySlots[(short)Battle_AllocateSecondaryResourceSlot(Battle_ActorScaleRampEmitterTask)]);
    effect->actorIndex = actorIndex;
    effect->actorScale = g_BattleActorRecords[actorIndex].scale;
    effect->currentScale = effect->actorScale;
}

/*
 * Emitter task driving bone-attached scale ramp sprite quads for an actor.
 */
// FUNCTION: FF7 0x005be4e2
void __cdecl Battle_ActorScaleRampEmitterTask(void)
{
    BattleActorScaleRamp *ramp;
    BattleFxScaleRampSprite *pEmitter;
    int actorIndex;
    int boneIdx;

    ramp = (BattleActorScaleRamp *)&g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx];
    actorIndex = ramp->actorIndex;
    boneIdx = (&g_BattleActorRecords[actorIndex].bone36)[ramp->step & 1];
    if (boneIdx != -1) {
        pEmitter = (BattleFxScaleRampSprite *)(&g_BattleTaskSlotBuffer.secondarySlots[(short)Battle_AllocateSecondaryResourceSlot(Battle_ActorScaleRampSpriteQuadTask)]);
        Battle_GetBoneWorldPos(actorIndex, boneIdx, &pEmitter->posX);
        pEmitter->posY = 0;
        pEmitter->scaleX = ramp->currentScale;
        pEmitter->scaleY = ramp->actorScale;
    }
    (ramp->step)++;
    if (ramp->step == 4) {
        ramp->actorIndex = 0xFFFF;
    }
}

/*
 * Updates and submits a scaling sprite quad for an actor effect slot.
 */
// FUNCTION: FF7 0x005be5a9
void __cdecl Battle_ActorScaleRampSpriteQuadTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[40];
        short rotM00;
        struct { unsigned char pad_to_2[0x2]; short rotM01; };
        struct { unsigned char pad_to_4[0x4]; short rotM02; };
        struct { unsigned char pad_to_6[0x6]; short rotM10; };
        struct { unsigned char pad_to_8[0x8]; short rotM11; };
        struct { unsigned char pad_to_A[0xA]; short rotM12; };
        struct { unsigned char pad_to_C[0xC]; short rotM20; };
        struct { unsigned char pad_to_E[0xE]; short rotM21; };
        struct { unsigned char pad_to_10[0x10]; short rotM22; };
        struct { unsigned char pad_to_12[0x12]; int transX; };
        struct {
            unsigned char pad[22];
            int value;
        } at18;
        struct { unsigned char pad_to_1A[0x1A]; int transZ; };
        struct { unsigned char pad_to_20[0x20]; int depth; };
        struct { unsigned char pad_to_24[0x24]; BattleFxScaleRampSprite *rampSprite; };
    } frame;
#pragma pack(pop)

    frame.rampSprite = (BattleFxScaleRampSprite *)&g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx];
    frame.rotM00 = 0;
    frame.rotM01 = 0;
    frame.rotM02 = 0;
    frame.rotM10 = 0;
    frame.rotM11 = 0;
    frame.rotM12 = 0;
    frame.rotM20 = 0;
    frame.rotM21 = 0;
    frame.rotM22 = 0x1000 /* 360 deg */;
    memset(&frame.transX, 0, 12);
    g_BattleWorldSprite_DrawReq.objectDword = Battle3d_GetSceneRenderObject(1, g_pBattle3dScene);
    g_BattleWorldSprite_DrawReq.stepByte = frame.rampSprite->step << 5;
    SetRotMatrix(&g_BattleCameraTransform);
    SetTransMatrix(&g_BattleCameraTransform);
    RotTransPoint((SVECTOR *)&frame.rampSprite->posX, &frame.transX, &frame.depth);
    frame.transZ -= frame.rampSprite->scaleY >> 4;
    frame.rotM00 = frame.rampSprite->scaleX + (frame.rampSprite->scaleX * frame.rampSprite->step >> 3);
    frame.rotM11 = frame.rampSprite->scaleY + (frame.rampSprite->scaleY * frame.rampSprite->step >> 3);
    SetRotMatrix(&frame.rotM00);
    SetTransMatrix(&frame.rotM00);
    BattleFx_SubmitSpriteQuadDraw((BattleFxSpriteQuadRequest *)&g_BattleWorldSprite_DrawReq, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, 0);
    if (g_BattleIsPaused == 0 && (frame.rampSprite->step)++ >= 7) {
        frame.rampSprite->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Spawns a world-space billboard sprite effect task.
 */
// FUNCTION: FF7 0x005be716
void __cdecl Battle_SpawnWorldSpriteTask(Vec3sPacked *position, short width, short height)
{
    BattleWorldSpriteRecord *entry;

    entry = (BattleWorldSpriteRecord *)(&g_BattleTaskSlotBuffer.secondarySlots[(short)Battle_AllocateSecondaryResourceSlot(Battle_WorldSpriteTask)]);
    entry->position = *(Int64Pair *)position;
    entry->width = width;
    entry->height = height;
}

/*
 * Updates and renders a world-space billboard sprite effect slot.
 */
// FUNCTION: FF7 0x005be760
void __cdecl Battle_WorldSpriteTask(void)
{
    BattleWorldSpriteRecord *p;
    int q; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */

    p = (BattleWorldSpriteRecord *)&g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx];
    ((BattleSpriteQuadRequest *)&g_BattleActorScale_SpriteQuadData)->resource = Battle3d_GetSceneRenderObject(1, g_pBattle3dScene);
    ((BattleSpriteQuadRequest *)&g_BattleActorScale_SpriteQuadData)->fadeLevel = p->frameCount << 5;
    *(short *)&g_BattleActorScale_TransformMatrix = p->width;
    *(short *)((char *)&g_BattleActorScale_TransformMatrix + 8) = p->height;
    BattleFx_MakeViewSpacePointPushed(&p->posX, -p->width >> 4, (Vec3sPacked *)&g_BattleActorScale_TransformMatrix);
    SetRotMatrix(&g_BattleActorScale_TransformMatrix);
    SetTransMatrix(&g_BattleActorScale_TransformMatrix);
    BattleFx_SubmitSpriteQuadDraw((BattleFxSpriteQuadRequest *)&g_BattleActorScale_SpriteQuadData, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, 0);
    if (g_BattleIsPaused == 0) {
        if ((p->frameCount)++ >= 7) {
            p->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Spawns a delayed positioned sprite effect task relative to an actor.
 */
// FUNCTION: FF7 0x005be844
void __cdecl Battle_SpawnPositionedEffectTask(int actorId, Int64Pair *position, int delay, int width, int height)
{
    BattleDelayedActorSprite *entry;

    entry = (BattleDelayedActorSprite *)(&g_BattleTaskSlotBuffer.secondarySlots[(short)Battle_AllocateSecondaryResourceSlot(Battle_DelayedActorSpriteTask)]);
    entry->actorIndex = actorId;
    entry->position = *position;
    entry->delay = delay;
    entry->width = width;
    entry->height = height;
}

/*
 * Updates delayed actor sprite, spawning a world-space sprite on expiration.
 */
// FUNCTION: FF7 0x005be8a4
void __cdecl Battle_DelayedActorSpriteTask(void)
{
    BattleDelayedActorSprite *pDelayed;
    Vec3sPacked *offset;
    PsxMatrix *matrix;
    BattleWorldSpriteRecord *worldSprite;

    pDelayed = (BattleDelayedActorSprite *)(&g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx]);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if ((pDelayed->delay)-- == 0) {
        offset = (Vec3sPacked *)Gfx_GetPsxScratchpadPtr(0);
        matrix = (PsxMatrix *)((char *)offset + 8);
        worldSprite = (BattleWorldSpriteRecord *)(&g_BattleTaskSlotBuffer.secondarySlots[(short)Battle_AllocateSecondaryResourceSlot(Battle_WorldSpriteTask)]);
        RotMatrix((SVECTOR *)&g_BattleActorRecords[pDelayed->actorIndex].rotX, matrix);
        TransformVectorToInt16(matrix, (SVECTOR *)&pDelayed->offsetX, &offset->vx);
        worldSprite->posX = g_BattleActorRecords[pDelayed->actorIndex].posX + offset->vx;
        worldSprite->posY = g_BattleActorRecords[pDelayed->actorIndex].posY + offset->vy;
        worldSprite->posZ = g_BattleActorRecords[pDelayed->actorIndex].posZ + offset->vz;
        worldSprite->width = pDelayed->width;
        worldSprite->height = pDelayed->height;
        pDelayed->ownerSlot = -1;
    }
}
} /* extern "C" */
