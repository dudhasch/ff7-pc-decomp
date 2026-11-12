// src/ff7/src/battle/battle_fx_limit_highwind.cpp
// Limit Break [34] (Cid Lv 4): Highwind (Japanese: ハイウインド, dev tag: "LIMIT3/HIWIND" / Hiwind)
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Highwind limit break effect, a missile barrage: a master timeline drives traveling
//   projectiles with their impact sparks and trails, explosion and smoke sprites, a shrinking
//   model, and a screen flash.
/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/types/battle.h"
#include "ff7/types/battle_limit.h"
#include "ff7/battle_tasks.h"
#include "lib/types/graphics.h"
#include "lib/enums/sound.h"
#include "lib/codegen.h"
#include "lib/types/polygon.h"
#include "ff7/camera_script.h"
#include <stdlib.h>
#include <string.h>
#include "ff7/api/battle.h"
#include "ff7/api/menu.h"
#include "lib/api/sound.h"
#include "lib/api/graphics.h"
#include "ff7/types/battle_3d.h"
extern "C" {

#pragma intrinsic(memset)

// clang-format off
// GLOBAL: FF7 0x008099e0
int g_BattleLimitHighwindResourceDef = 0x001900B4;
int g_BattleLimitHighwindResourceDefHeadPtr[1] = {
    (int)((char *)&g_BattleFxLimitHighwindResourceNames + 0x0)
};
int g_BattleLimitHighwindResourceDefDataTail[24] = {
    0x0, 0x0, 0x0, 0x3240b1, (int)((char *)&g_BattleFxLimitHighwindResourceNames + 0xc), 0x0,
    0x0, 0x0, 0x192a39, (int)((char *)&g_BattleFxLimitHighwindResourceNames + 0x18), (int)((char *)&g_BattleFxLimitHighwindResourceNames + 0x24), 0x0,
    0x0, 0x192a39, (int)((char *)&g_BattleFxLimitHighwindResourceNames + 0x30), (int)((char *)&g_BattleFxLimitHighwindResourceNames + 0x3c), 0x0, 0x0,
    0x192a3c, (int)((char *)&g_BattleFxLimitHighwindResourceNames + 0x48), (int)((char *)&g_BattleFxLimitHighwindResourceNames + 0x54), 0x0, 0x0, 0x0
};
/* [0] is the impact spark sprite (model table entry 4), [1] the burst sprite (entry 3). */
// GLOBAL: FF7 0x00809a48
BattleFxSpriteRequest g_BattleLimitHighwindSpriteReqPool[2] = {
    {0, 0, 0x80, 0x80, 0x80, 0x2e, 0, 0},
    {0, 0, 0x80, 0x80, 0x80, 0x2e, 0, 0x1e1}
};
// clang-format on
/* The burst sprite's scale, then the first two words of the trail sprite's request (entry 2):
 * its colour and frame are g_BattleLimitHighwindColorData80, the next object. */
typedef struct {
    VECTOR burstScale;
    int trailSpriteGroup;
    int trailDrawObj;
} BattleLimitHiwindAnimTable68;
// clang-format off
// GLOBAL: FF7 0x00809a68
BattleLimitHiwindAnimTable68 g_BattleLimitHighwindAnimTable68 = {{0x4000, 0x4000, 0x4000, 0}, 0, 0};
// clang-format on
/* The rest of the trail sprite's request, laid out as in BattleFxSpriteRequest. */
typedef struct {
    char colorR;
    char colorG;
    char colorB;
    unsigned char code;
    short frame;
    unsigned short uvPacked;
} BattleLimitHiwindColorData80;
/* The missile's scale, then the first word (modelGroup) of its BattleFxModelDraw request; the
 * request's flags and fade fields are the next two objects. */
typedef struct {
    VECTOR missileScale;
    int missileModelGroup;
} BattleLimitHiwindAnimTable88;
// clang-format off
// GLOBAL: FF7 0x00809a80
BattleLimitHiwindColorData80 g_BattleLimitHighwindColorData80 = {0x80, 0x80, 0x80, 0x2e, 0, 0};
// GLOBAL: FF7 0x00809a88
BattleLimitHiwindAnimTable88 g_BattleLimitHighwindAnimTable88 = {{0x2000, 0x2000, 0x2000, 0}, 0};
unsigned char g_rawData_00809a9c[4] = {0x80, 0x00, 0x00, 0x00};
unsigned char g_rawData_00809aa0[8] = {0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00};
// clang-format on
/* The missile model's base rotation, then the first word (modelGroup) of the shrinking model's
 * BattleFxModelDraw request; the request's flags and fade fields are the next two objects. */
typedef struct {
    SVECTOR missileRotation;
    int shrinkModelGroup;
} BattleLimitHiwindParamTableA8;
// clang-format off
// GLOBAL: FF7 0x00809aa8
BattleLimitHiwindParamTableA8 g_BattleLimitHighwindParamTableA8 = {{0, 0x400, 0, 0}, 0};
unsigned char g_rawData_00809ab4[4] = {0xa0, 0x01, 0x00, 0x00};
unsigned char g_rawData_00809ab8[8] = {0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x00809ac0
int g_BattleLimitHighwindScaleVector[3] = {0, 16384, 0};
unsigned char g_BattleFxLimitHighwindResourceNames[96] =
    "MISSO.RSD\0\0\0"
    "LEAZER.RSD\0\0"
    "WIND00.TIM\0\0"
    "FIR_1.SPT\0\0\0"
    "WIND00.TIM\0\0"
    "FIR_2.SPT\0\0\0"
    "WIND02.TIM\0\0"
    "EXPLO_1.SPT";
// clang-format on

#pragma intrinsic(memcpy)
#pragma function(memcpy)

// clang-format off
// GLOBAL: FF7 0x00af0468
short g_BattleLimitHighwindTargetCentroid[2] = {0};
// GLOBAL: FF7 0x00af046c
short g_BattleLimitHighwindTargetCentroidZ = 0;
// GLOBAL: FF7 0x00af0470
int g_BattleLimitHighwindDrawHandle = 0;
// GLOBAL: FF7 0x00af0474
short g_BattleLimitHighwindTargetBits = 0;
// GLOBAL: FF7 0x00af0478
short g_BattleLimitHighwindFlashLevel = 0;
// GLOBAL: FF7 0x00af047c
struct GfxTableCursor *g_BattleLimitHighwindModelTable = 0;
// GLOBAL: FF7 0x00af0480
short g_BattleLimitHighwindHitIndex = 0;
unsigned char g_bssPad_00af0484[4] = {0};
// GLOBAL: FF7 0x00af0488
short g_BattleLimitHighwindShrinkRotation[3] = {0};
// clang-format on

/*
 * Loads Highwind limit break effect resources (LIMIT3/HIWIND).
 */
// FUNCTION: FF7 0x004b38a0
void __cdecl BattleFx_LimitLoadResHighwind(void)
{
    GfxModelTableBank *ownerHandle;
    GfxResourceRequest recordBuf;
    int kind;
    char pathText[200];
    GfxResourceRequest *pRequest;
    int stepResourceDef;

    if (g_pBattle3dScene != 0 && g_BattleLimitHighwindModelTable == 0) {
        kind = 5;
        stepResourceDef = (int)&g_BattleLimitHighwindResourceDef;
        ownerHandle = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRequest = &recordBuf;
        Gfx_InitResourceRequest(pRequest);
        pRequest->groupTag = 0;
        // STRING: FF7 0x00809b30
        pRequest->resourcePath = BattleFx_BuildResourcePath("LIMIT3/HIWIND", pathText);
        Gfx_LoadModelTableIfAbsent(kind, stepResourceDef, pRequest, &g_BattleLimitHighwindModelTable, ownerHandle);
    }
}

/*
 * Starts the Highwind limit: full flash level, first hit, resources loaded, then the timeline.
 */
// FUNCTION: FF7 0x004b3930
int __cdecl BattleFx_LimitEnterHighwind(int targetMask, int actorIdx)
{
    g_BattleLimitHighwindFlashLevel = 0xF0;
    g_BattleLimitHighwindHitIndex = 0;
    BattleFx_LimitLoadResHighwind();
    BattleFx_LimitBeginHighwind(targetMask, actorIdx);
    return 0;
}

/*
 * Records the targets and their centroid and starts the timeline task 4000 units above the
 * attacker.
 */
// FUNCTION: FF7 0x004b395e
void __cdecl BattleFx_LimitBeginHighwind(int targetMask, int actorIdx)
{
    BattleFxCosmoMemoryTriSpin *slot;

    g_BattleLimitHighwindTargetBits = targetMask;
    slot = (BattleFxCosmoMemoryTriSpin *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitHighwindTimelineTask);
    Battle_GetActorMaskCentroid(targetMask, g_BattleLimitHighwindTargetCentroid);
    slot->posX = g_BattleActorRecords[actorIdx].posX;
    slot->posY = g_BattleActorRecords[actorIdx].posY;
    slot->posZ = g_BattleActorRecords[actorIdx].posZ;
    slot->posY = 0xF060;
    g_BattleScriptEffectFlag = 1;
}

/*
 * Master timeline task for Highwind missile barrage.
 */
// FUNCTION: FF7 0x004b39f2
void __cdecl BattleFx_LimitHighwindTimelineTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[72];
        struct { unsigned char pad_to_C[0xC]; short targetX; };
        struct { unsigned char pad_to_E[0xE]; short targetY; };
        struct { unsigned char pad_to_10[0x10]; short targetZ; };
        struct { unsigned char pad_to_34[0x34]; BattleFxHighwindProjectile *projectileSlot; };
        struct { unsigned char pad_to_38[0x38]; BattleFxHighwindHitRecord *ringRecord; };
        struct { unsigned char pad_to_3C[0x3C]; short typeIndex; };
        struct { unsigned char pad_to_40[0x40]; short angle; };
        struct { unsigned char pad_to_44[0x44]; BattleFxCosmoMemoryTriSpin *hiwindRecord; };
    } frame;
#pragma pack(pop)

    frame.hiwindRecord = (BattleFxCosmoMemoryTriSpin *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.hiwindRecord->step == 0) {
        SfxLoadIntoBuffer(0x223, 0);
        SfxLoadIntoBuffer(0x7C, 0);
        SfxLoadIntoBuffer(0x21B, 0);
    }
    if (frame.hiwindRecord->step == 53) {
        SoundDispatchCommand(SOUND_SFX_PLAY_CH2, 0x40, 0x21B, 0, 0, 0, 0, 0, 0);
    }
    if (frame.hiwindRecord->step == 69 || frame.hiwindRecord->step == 111) {
        SoundDispatchCommand(SOUND_SFX_PLAY_CH1, 0x40, 0x223, 0, 0, 0, 0, 0, 0);
    }
    if (((frame.hiwindRecord->step >= 60 && frame.hiwindRecord->step < 90) || (frame.hiwindRecord->step >= 102 && frame.hiwindRecord->step < 132)) && frame.hiwindRecord->step % 3 != 0) {
        frame.projectileSlot = (BattleFxHighwindProjectile *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitHighwindProjectileTask);
        frame.projectileSlot->posX = frame.hiwindRecord->posX;
        frame.projectileSlot->posY = frame.hiwindRecord->posY;
        frame.projectileSlot->posZ = frame.hiwindRecord->posZ;
        frame.angle = (rand() % 4096);
        frame.projectileSlot->posX += (short)(SinFixed(frame.angle) * 0x3E8 >> 12);
        frame.projectileSlot->posZ += (short)(CosFixed(frame.angle) * 0x3E8 >> 12);
        frame.targetX = g_BattleLimitHighwindTargetCentroid[0];
        frame.targetY = g_BattleLimitHighwindTargetCentroid[1];
        frame.targetZ = g_BattleLimitHighwindTargetCentroidZ;
        if (rand() % 3 == 0) {
            frame.angle = (rand() % 4096);
            frame.targetX += (short)(SinFixed(frame.angle) * 0xFA0 >> 12);
            frame.targetZ += (short)(CosFixed(frame.angle) * 0xFA0 >> 12);
        } else {
            frame.targetX += (short)(rand() % 3000 - 1500);
            frame.targetZ += (short)(rand() % 3000 - 1500);
        }
        frame.projectileSlot->velocityX = (frame.targetX - frame.projectileSlot->posX) / 10;
        frame.projectileSlot->velocityY = 600;
        frame.projectileSlot->velocityZ = (frame.targetZ - frame.projectileSlot->posZ) / 10;
    }
    if (((frame.hiwindRecord->step >= 69 && frame.hiwindRecord->step < 96) || (frame.hiwindRecord->step >= 111 && frame.hiwindRecord->step < 138)) && frame.hiwindRecord->step % 3 == 0) {
        frame.ringRecord = (BattleFxHighwindHitRecord *)Battle_AllocSceneMessage(0xA);
        frame.ringRecord->targetActor = g_BattleHitTargetActors[g_BattleLimitHighwindHitIndex];
        frame.ringRecord->hitIndex = g_BattleLimitHighwindHitIndex;
        g_BattleLimitHighwindHitIndex++;
    }
    if (frame.hiwindRecord->step == 140) {
        frame.projectileSlot = (BattleFxHighwindProjectile *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitHighwindShrinkingModelTask);
        frame.projectileSlot->posX = g_BattleLimitHighwindTargetCentroid[0];
        frame.projectileSlot->posY = g_BattleLimitHighwindTargetCentroid[1];
        frame.projectileSlot->posZ = g_BattleLimitHighwindTargetCentroidZ;
        Battle_AllocateResourceSlot(BattleFx_LimitHighwindScreenFlashTask);
        Battle_AllocateResourceSlot(BattleFx_LimitHighwindSpriteAnimationEmitterTask);
        SoundDispatchCommand(SOUND_SFX_PLAY_CH1, 0x40, 0x7C, 0, 0, 0, 0, 0, 0);
    }
    if (frame.hiwindRecord->step == 140) {
        for (frame.typeIndex = 0; frame.typeIndex < 0xA; frame.typeIndex++) {
            if ((g_BattleLimitHighwindTargetBits & 1 << frame.typeIndex) != 0) {
                Battle_PostActorHitOrHurtMessage(frame.typeIndex);
            }
        }
    }
    (frame.hiwindRecord->step)++;
    if (frame.hiwindRecord->step == 170) {
        g_BattleScriptEffectFlag = 0;
        frame.hiwindRecord->ownerSlot = BATTLE_TASK_FREE;
        Sfx_ReleaseCachedBuffer(0x223, 0xF);
        Sfx_ReleaseCachedBuffer(0x7C, 0xF);
        Sfx_ReleaseCachedBuffer(0x21B, 0xF);
    }
}

/*
 * Screen flash task for Highwind impact: flashes white at the flash level, which falls by 15 a
 * frame for 16 frames.
 */
// FUNCTION: FF7 0x004b3e72
void __cdecl BattleFx_LimitHighwindScreenFlashTask(void)
{
    BattleTaskSlot *task;

    task = BATTLE_CURRENT_TASK_SLOT;
    g_BattleOverlayDrawListHead = BattleFx_FlashScreenWhite(*(char *)&g_BattleLimitHighwindFlashLevel, *(char *)&g_BattleLimitHighwindFlashLevel, *(char *)&g_BattleLimitHighwindFlashLevel);
    if (g_BattleIsPaused != 0) {
        return;
    }
    g_BattleLimitHighwindFlashLevel -= 0xF;
    (task->frameCounter)++;
    if (task->frameCounter == 16) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Burst sprite emitter task for Highwind: for 16 frames, spawns three burst sprites a frame on a
 * 1000-unit circle around the target centroid, each tilted at random.
 */
// FUNCTION: FF7 0x004b3ef3
void __cdecl BattleFx_LimitHighwindSpriteAnimationEmitterTask(void)
{
    BattleTaskSlot *taskPtr;
    short angle;
    short i;
    BattleFxHighwindBurstSprite *pSprite;

    taskPtr = BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    for (i = 0; i < 3; i++) {
        angle = rand() % 0x1000;
        pSprite = (BattleFxHighwindBurstSprite *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitHighwindSpriteAnimationTask);
        pSprite->posX = g_BattleLimitHighwindTargetCentroid[0];
        pSprite->posY = g_BattleLimitHighwindTargetCentroid[1];
        pSprite->posZ = g_BattleLimitHighwindTargetCentroidZ;
        pSprite->posX += (short)(SinFixed(angle) * 0x3E8 >> 12);
        pSprite->posZ += (short)(CosFixed(angle) * 0x3E8 >> 12);
        pSprite->rotation.vx = 0xFFFFFE0C - rand() % 0x1F4;
        pSprite->rotation.vy = angle;
        pSprite->rotation.vz = 0;
    }
    (taskPtr->frameCounter)++;
    if (taskPtr->frameCounter == 16) {
        taskPtr->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Burst sprite task for Highwind: draws one burst sprite at its position and rotation and steps
 * its frame until the sprite group's last.
 */
// FUNCTION: FF7 0x004b4045
void __cdecl BattleFx_LimitHighwindSpriteAnimationTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[68];
        PsxMatrix rotMatrix;
        struct { unsigned char pad_to_20[0x20]; BattleFxHighwindBurstSprite *spriteSlot; };
        struct { unsigned char pad_to_24[0x24]; PsxMatrix combinedMatrix; };
    } frame;
#pragma pack(pop)

    frame.spriteSlot = (BattleFxHighwindBurstSprite *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleLimitHighwindSpriteReqPool[1].spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(3, g_BattleLimitHighwindModelTable);
    g_BattleLimitHighwindSpriteReqPool[1].drawObj = Gfx_ModelTableGetSpriteObject(3, g_BattleLimitHighwindModelTable);
    RotMatrix(&frame.spriteSlot->rotation, &frame.rotMatrix);
    frame.rotMatrix.t[0] = frame.spriteSlot->posX;
    frame.rotMatrix.t[1] = frame.spriteSlot->posY;
    frame.rotMatrix.t[2] = frame.spriteSlot->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.combinedMatrix);
    Gte_OrthonormaliseFromYAxis((BattleOrthoMatrix *)&frame.combinedMatrix);
    ScaleMatrix(&frame.combinedMatrix, &g_BattleLimitHighwindAnimTable68.burstScale);
    SetRotMatrix(&frame.combinedMatrix);
    SetTransMatrix(&frame.combinedMatrix);
    g_BattleLimitHighwindSpriteReqPool[1].uvPacked++;
    g_BattleLimitHighwindSpriteReqPool[1].frame = frame.spriteSlot->frameIndex;
    g_BattleOverlayDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleLimitHighwindSpriteReqPool[1], g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (g_BattleLimitHighwindSpriteReqPool[1].spriteGroup != 0) {
        (frame.spriteSlot->frameIndex)++;
        if (frame.spriteSlot->frameIndex >= g_BattleLimitHighwindSpriteReqPool[1].spriteGroup->header->frameCount) {
            frame.spriteSlot->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Missile task for Highwind: draws the missile model facing along its velocity, moves it under
 * gravity and leaves a trail sprite each frame; after ten frames it spawns the impact spark.
 */
// FUNCTION: FF7 0x004b4191
void __cdecl BattleFx_LimitHighwindProjectileTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[104];
        PsxMatrix rotMatrix;
        struct { unsigned char pad_to_20[0x20]; BattleFxPositionedSpriteSlot *spriteState; };
        struct { unsigned char pad_to_24[0x24]; BattleFxHighwindProjectile *projectile; };
        struct { unsigned char pad_to_28[0x28]; PsxMatrix lookBasis; };
        struct { unsigned char pad_to_48[0x48]; PsxMatrix combinedMatrix; };
    } frame;
#pragma pack(pop)

    frame.projectile = (BattleFxHighwindProjectile *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleLimitHighwindAnimTable88.missileModelGroup = Gfx_ModelTableGetRenderObject(0, g_BattleLimitHighwindModelTable);
    Gte_BuildLookBasisFromDirection(&frame.projectile->velocityX, frame.lookBasis.m[0]);
    RotMatrix(&g_BattleLimitHighwindParamTableA8.missileRotation, &frame.rotMatrix);
    ConcatMatrixPreservingTranslation(&frame.lookBasis, &frame.rotMatrix, &frame.rotMatrix);
    ScaleMatrix(&frame.rotMatrix, &g_BattleLimitHighwindAnimTable88.missileScale);
    frame.rotMatrix.t[0] = frame.projectile->posX;
    frame.rotMatrix.t[1] = frame.projectile->posY;
    frame.rotMatrix.t[2] = frame.projectile->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.combinedMatrix);
    SetRotMatrix(&frame.combinedMatrix);
    SetTransMatrix(&frame.combinedMatrix);
    SetDrawColorRGB(0, 0, 0);
    g_BattleLimitHighwindDrawHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleLimitHighwindAnimTable88.missileModelGroup, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleLimitHighwindDrawHandle);
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.projectile->posX += frame.projectile->velocityX;
    frame.projectile->posY += frame.projectile->velocityY;
    frame.projectile->posZ += frame.projectile->velocityZ;
    frame.projectile->velocityY -= 60;
    frame.spriteState = (BattleFxPositionedSpriteSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitHighwindProjectileTrailSpriteTask);
    frame.spriteState->posX = frame.projectile->posX;
    frame.spriteState->posY = frame.projectile->posY;
    frame.spriteState->posZ = frame.projectile->posZ;
    (frame.projectile->age)++;
    if (frame.projectile->age == 10) {
        frame.spriteState = (BattleFxPositionedSpriteSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitHighwindProjectileImpactSpriteTask);
        frame.spriteState->posX = frame.projectile->posX;
        frame.spriteState->posY = frame.projectile->posY;
        frame.spriteState->posZ = frame.projectile->posZ;
        frame.projectile->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Missile impact spark sprite task for Highwind.
 */
// FUNCTION: FF7 0x004b439d
void __cdecl BattleFx_LimitHighwindProjectileImpactSpriteTask(void)
{
    BattleTaskSlot *task;

    task = BATTLE_CURRENT_TASK_SLOT;
    g_BattleLimitHighwindSpriteReqPool[0].spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(4, g_BattleLimitHighwindModelTable);
    g_BattleLimitHighwindSpriteReqPool[0].drawObj = Gfx_ModelTableGetSpriteObject(4, g_BattleLimitHighwindModelTable);
    BattleFx_InstallBillboardMatrix(task->data, 0x3000 /* 3.0 */, 0);
    g_BattleLimitHighwindSpriteReqPool[0].frame = task->frameCounter;
    g_BattleLimitHighwindDrawHandle = BattleFx_SubmitSpriteDraw(&g_BattleLimitHighwindSpriteReqPool[0], g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleLimitHighwindDrawHandle);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (g_BattleLimitHighwindSpriteReqPool[0].spriteGroup != 0) {
        (task->frameCounter)++;
        if (task->frameCounter >= g_BattleLimitHighwindSpriteReqPool[0].spriteGroup->header->frameCount) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Missile trail sprite task for Highwind: from its third frame, draws the trail sprite, at full
 * colour on its first drawn frame and at a quarter after.
 */
// FUNCTION: FF7 0x004b4470
void __cdecl BattleFx_LimitHighwindProjectileTrailSpriteTask(void)
{
    BattleTaskSlot *task;

    task = BATTLE_CURRENT_TASK_SLOT;
    g_BattleLimitHighwindAnimTable68.trailSpriteGroup = Gfx_ModelTableGetLoadedRecord(2, g_BattleLimitHighwindModelTable);
    g_BattleLimitHighwindAnimTable68.trailDrawObj = Gfx_ModelTableGetSpriteObject(2, g_BattleLimitHighwindModelTable);
    if (task->frameCounter > 1) {
        BattleFx_InstallBillboardMatrix(task->data, 0x1C00 /* 1.75 */, 0);
        g_BattleLimitHighwindColorData80.frame = task->frameCounter - 2;
        g_BattleLimitHighwindColorData80.colorB = ((task->frameCounter - 2 != 0 ? -1 : 0) & (char)0xA0) + 0x80;
        g_BattleLimitHighwindColorData80.colorG = g_BattleLimitHighwindColorData80.colorB;
        g_BattleLimitHighwindColorData80.colorR = g_BattleLimitHighwindColorData80.colorG;
        g_BattleLimitHighwindDrawHandle = BattleFx_SubmitSpriteDraw(&g_BattleLimitHighwindAnimTable68.trailSpriteGroup, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleLimitHighwindDrawHandle);
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (g_BattleLimitHighwindAnimTable68.trailSpriteGroup != 0) {
        (task->frameCounter)++;
        if (task->frameCounter >= ((BattleFxSpriteGroup *)g_BattleLimitHighwindAnimTable68.trailSpriteGroup)->header->frameCount) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Shrinking model task for Highwind: draws the second model at the target centroid, shrinking its
 * X and Z scale over 20 frames.
 */
// FUNCTION: FF7 0x004b4584
void __cdecl BattleFx_LimitHighwindShrinkingModelTask(void)
{
    struct {
        PsxMatrix first;
        char pad0[2];
        BattleFxHighwindShrinkRecord *p;
        PsxMatrix second;
        char pad1[2];
    } local; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    local.p = (BattleFxHighwindShrinkRecord *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleLimitHighwindParamTableA8.shrinkModelGroup = Gfx_ModelTableGetRenderObject(1, g_BattleLimitHighwindModelTable);
    RotMatrixXYZ((void *)g_BattleLimitHighwindShrinkRotation, &local.first);
    g_BattleLimitHighwindScaleVector[2] = (0x14 - local.p->frameCount) * 0x640;
    g_BattleLimitHighwindScaleVector[0] = g_BattleLimitHighwindScaleVector[2];
    ScaleMatrix(&local.first, &g_BattleLimitHighwindScaleVector);
    local.first.t[0] = local.p->posX;
    local.first.t[1] = local.p->posY;
    local.first.t[2] = local.p->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &local.first, &local.second);
    SetRotMatrix(&local.second);
    SetTransMatrix(&local.second);
    SetDrawColorRGB(0, 0, 0);
    g_BattleLimitHighwindDrawHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleLimitHighwindParamTableA8.shrinkModelGroup,
                                            g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED,
                                            0xC,
                                            g_BattleLimitHighwindDrawHandle);
    if (g_BattleIsPaused != 0) {
        return;
    }
    (local.p->frameCount)++;
    if (local.p->frameCount == 20) {
        local.p->ownerSlot = BATTLE_TASK_FREE;
    }
}
} /* extern "C" */
