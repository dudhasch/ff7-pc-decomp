// src/ff7/src/battle/battle_fx_special_evil_poison.cpp
/* Enemy Special Effect [idx 21]: EvilPoison (JP: sumi / 墨 (Ink), asset: "SPECIAL/SUMI") — Evil Poison (0x182, Zenene), Erupt (0x13c, Whole Eater), Smog (0x129, 0x12a, Smogger); also GeneralDispatchTable[58] */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle special effect for the Sumi ink attack: loads its resources, spawns per-target
//   ink blasts, and drives the ink spray flight, droplet emitter, splash and drifting ink cloud
//   sprites, with an alternate entry point.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
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
#include "ff7/enums/battle.h"
extern "C" {

// clang-format off
// GLOBAL: FF7 0x008a73c0
int g_BattleSpecialEvilPoisonResourceDefs = 0x00192A39;
int g_BattleSpecialEvilPoisonResourceDefsHeadPtr[1] = {
    (int)((char *)&g_BattleFxSpecialEvilPoisonResourceNames + 0x4)
};
int g_BattleSpecialEvilPoisonResourceDefsDataTail[28] = {
    (int)((char *)&g_BattleFxSpecialEvilPoisonResourceNames + 0x14), 0x0, 0x0, 0x192a39, (int)((char *)&g_BattleFxSpecialEvilPoisonResourceNames + 0x20), (int)((char *)&g_BattleFxSpecialEvilPoisonResourceNames + 0x30),
    0x0, 0x0, 0x192a39, (int)((char *)&g_BattleFxSpecialEvilPoisonResourceNames + 0x3c), (int)((char *)&g_BattleFxSpecialEvilPoisonResourceNames + 0x4c), 0x0,
    0x0, 0x192a39, (int)((char *)&g_BattleFxSpecialEvilPoisonResourceNames + 0x58), (int)((char *)&g_BattleFxSpecialEvilPoisonResourceNames + 0x68), 0x0, 0x0,
    0x192a39, (int)((char *)&g_BattleFxSpecialEvilPoisonResourceNames + 0x74), (int)((char *)&g_BattleFxSpecialEvilPoisonResourceNames + 0x84), 0x0, 0x0, 0x192a39,
    (int)((char *)&g_BattleFxSpecialEvilPoisonResourceNames + 0x90), (int)((char *)&g_BattleFxSpecialEvilPoisonResourceNames + 0xa0), 0x0, 0x0
};
// GLOBAL: FF7 0x00b6f0b8
int g_BattleSpecialEvilPoisonDrawListHandle = 0;
// GLOBAL: FF7 0x00b6f0bc
struct GfxTableCursor *g_BattleSpecialEvilPoisonLoadedRes = 0;
// GLOBAL: FF7 0x00b6f0c0
short g_BattleSpecialEvilPoisonMode = 0;
// clang-format on

// clang-format off
#undef g_BattleSpecialEvilPoisonSpriteReq1
// GLOBAL: FF7 0x008a7438
struct BattleFxSpriteRequest g_BattleSpecialEvilPoisonSpriteReq1 = {0, 0, -128, -128, -128, 0x2c, 0, 0};
#undef g_BattleSpecialEvilPoisonSpriteReq2
// GLOBAL: FF7 0x008a7448
struct BattleSpecialEvilPoisonSpriteReq2 g_BattleSpecialEvilPoisonSpriteReq2 = {{2, 3, 4, 5}, {0, 0, 96, 96, 96, 0x2c, 0, 0}, 0, 0};
unsigned char g_rawData_008a7470[8] = {0x30, 0x30, 0x30, 0x2c, 0x00, 0x00, 0x00, 0x00};
// clang-format on

// clang-format off
// GLOBAL: FF7 0x008a7478
short g_BattleSpecialEvilPoisonScales[2] = {4096, 6144};
unsigned char g_rawData_008a747c[4] = {0x00, 0x14, 0x00, 0x11};
unsigned char g_BattleFxSpecialEvilPoisonResourceNames[172] = {
    0x80, 0x09, 0x00, 0x06, 0x73, 0x75, 0x6d, 0x69, 0x5f, 0x61, 0x30, 0x30, 0x2e, 0x74, 0x69, 0x6d, 0x00, 0x00, 0x00, 0x00, 0x73, 0x75, 0x6d, 0x69,
    0x2e, 0x73, 0x70, 0x74, 0x00, 0x00, 0x00, 0x00, 0x73, 0x75, 0x6d, 0x69, 0x5f, 0x61, 0x30, 0x30, 0x2e, 0x74, 0x69, 0x6d, 0x00, 0x00, 0x00, 0x00,
    0x68, 0x69, 0x74, 0x31, 0x2e, 0x73, 0x70, 0x74, 0x00, 0x00, 0x00, 0x00, 0x73, 0x75, 0x6d, 0x69, 0x5f, 0x61, 0x30, 0x31, 0x2e, 0x74, 0x69, 0x6d,
    0x00, 0x00, 0x00, 0x00, 0x68, 0x69, 0x74, 0x32, 0x31, 0x2e, 0x73, 0x70, 0x74, 0x00, 0x00, 0x00, 0x73, 0x75, 0x6d, 0x69, 0x5f, 0x61, 0x30, 0x31,
    0x2e, 0x74, 0x69, 0x6d, 0x00, 0x00, 0x00, 0x00, 0x68, 0x69, 0x74, 0x32, 0x32, 0x2e, 0x73, 0x70, 0x74, 0x00, 0x00, 0x00, 0x73, 0x75, 0x6d, 0x69,
    0x5f, 0x61, 0x30, 0x31, 0x2e, 0x74, 0x69, 0x6d, 0x00, 0x00, 0x00, 0x00, 0x68, 0x69, 0x74, 0x32, 0x33, 0x2e, 0x73, 0x70, 0x74, 0x00, 0x00, 0x00,
    0x73, 0x75, 0x6d, 0x69, 0x5f, 0x61, 0x30, 0x31, 0x2e, 0x74, 0x69, 0x6d, 0x00, 0x00, 0x00, 0x00, 0x68, 0x69, 0x74, 0x32, 0x34, 0x2e, 0x73, 0x70,
    0x74, 0x00, 0x00, 0x00
};
// clang-format on

#pragma intrinsic(memset)
#pragma intrinsic(memcpy)


/*
 * Loads and registers resources for the Sumi special effect.
 */
// FUNCTION: FF7 0x005535e0
void __cdecl BattleFx_SpecialLoadResEvilPoison(void)
{
    GfxModelTableBank *stageSceneHandle;
    GfxResourceRequest requestObj;
    int resourceCount;
    char pathWorkBuffer[200];
    GfxResourceRequest *reqPtrPtr;
    int resourceTable;

    if (g_pBattle3dScene != 0 && g_BattleSpecialEvilPoisonLoadedRes == 0) {
        resourceCount = 6;
        resourceTable = (int)&g_BattleSpecialEvilPoisonResourceDefs;
        stageSceneHandle = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        reqPtrPtr = &requestObj;
        Gfx_InitResourceRequest(reqPtrPtr);
        reqPtrPtr->groupTag = 0;
        // STRING: FF7 0x008a752c
        reqPtrPtr->resourcePath = (char *)BattleFx_BuildResourcePath("SPECIAL/SUMI", pathWorkBuffer);
        Gfx_LoadModelTableIfAbsent(resourceCount, resourceTable, reqPtrPtr, &g_BattleSpecialEvilPoisonLoadedRes, stageSceneHandle);
    }
}

/*
 * Entry point thunk for the Sumi special effect.
 */
// FUNCTION: FF7 0x00553670
void __cdecl BattleFx_SpecialEnterEvilPoison(int targetMask, int actorIdx)
{
    BattleFx_SpecialLoadResEvilPoison();
    Battle_CreateBitPatternCallbackTask(targetMask, actorIdx, 4, BattleFx_SpecialSpawnEvilPoisonPerTarget);
    Battle_PlaySfxPannedToActor(0x8A, (short)actorIdx, 0x40);
}

/*
 * Spawns per-target ink blast tasks for Sumi.
 */
// FUNCTION: FF7 0x005536a5
void __cdecl BattleFx_SpecialSpawnEvilPoisonPerTarget(int actor, int attackerId)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[12];
        BattleFxEvilPoisonSprite *sumiSprite;
        struct { unsigned char pad_to_4[0x4]; SVECTOR targetX; };
    } frame;
#pragma pack(pop)

    frame.sumiSprite = (BattleFxEvilPoisonSprite *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialEvilPoisonTimelineTask);
    Battle_GetBoneWorldPos(attackerId, g_BattleActorRecords[attackerId].bone34, &frame.sumiSprite->posX);
    Battle_GetBoneWorldPos(actor, g_BattleActorRecords[actor].targetBone, &frame.targetX.vx);
    switch (g_BattleSpecialEvilPoisonMode) {
    case BATTLE_EVIL_POISON_MODE_DEFAULT:
        frame.sumiSprite->velocityX = (frame.targetX.vx - frame.sumiSprite->posX) / 12;
        frame.sumiSprite->velocityY = (frame.targetX.vy - frame.sumiSprite->posY) / 12;
        frame.sumiSprite->velocityZ = (frame.targetX.vz - frame.sumiSprite->posZ) / 12;
        break;
    case BATTLE_EVIL_POISON_MODE_ALT:
        frame.sumiSprite->velocityX = (frame.targetX.vx - frame.sumiSprite->posX) / 10;
        frame.sumiSprite->velocityY = (frame.targetX.vy - frame.sumiSprite->posY) / 10;
        frame.sumiSprite->velocityZ = (frame.targetX.vz - frame.sumiSprite->posZ) / 10;
    }
    frame.sumiSprite->targetIndex = actor;
}

/*
 * Main timeline task controlling ink spray flight and impact sequence.
 */
// FUNCTION: FF7 0x005537df
void __cdecl BattleFx_SpecialEvilPoisonTimelineTask(void)
{
    short spriteIndex;
    BattleFxEvilPoisonTimelineSlot *taskPtr;
    BattleFxSpecialEvilPoisonSlot *sprite;
    int unused; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */

    taskPtr = (BattleFxEvilPoisonTimelineSlot *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    switch (g_BattleSpecialEvilPoisonMode) {
    case BATTLE_EVIL_POISON_MODE_DEFAULT:
        if (taskPtr->step < 12 && (taskPtr->step % 4) != 3) {
            for (spriteIndex = 0; spriteIndex < 2; spriteIndex++) {
                sprite = (BattleFxSpecialEvilPoisonSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialEvilPoisonDriftingSpriteTask);
                sprite->posX = taskPtr->posX - (taskPtr->spreadX / 2) * spriteIndex;
                sprite->posY = taskPtr->posY - (taskPtr->spreadY / 2) * spriteIndex;
                sprite->posZ = taskPtr->posZ - (taskPtr->spreadZ / 2) * spriteIndex;
                sprite->spreadX = taskPtr->spreadX;
                sprite->spreadY = taskPtr->spreadY;
                sprite->spreadZ = taskPtr->spreadZ;
                sprite->index = (taskPtr->step % 4) * 2 + spriteIndex;
            }
        }
        if (taskPtr->step == 12) {
            Battle_PostActorHitOrHurtMessage(taskPtr->targetIndex);
        }
        if (taskPtr->step == 12) {
            sprite = (BattleFxSpecialEvilPoisonSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialEvilPoisonPulseEmitterTask);
            Battle_GetBoneWorldPos(taskPtr->targetIndex, g_BattleActorRecords[taskPtr->targetIndex].targetBone, &sprite->posX);
            sprite->index = 0;
        }
        (taskPtr->step)++;
        if (taskPtr->step == 40) {
            taskPtr->ownerSlot = BATTLE_TASK_FREE;
        }
        break;
    case BATTLE_EVIL_POISON_MODE_ALT:
        if (taskPtr->step < 3) {
            for (spriteIndex = 0; spriteIndex < 2; spriteIndex++) {
                sprite = (BattleFxSpecialEvilPoisonSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialEvilPoisonDriftingSpriteTask);
                sprite->posX = taskPtr->posX - (taskPtr->spreadX / 2) * spriteIndex;
                sprite->posY = taskPtr->posY - (taskPtr->spreadY / 2) * spriteIndex;
                sprite->posZ = taskPtr->posZ - (taskPtr->spreadZ / 2) * spriteIndex;
                sprite->spreadX = taskPtr->spreadX;
                sprite->spreadY = taskPtr->spreadY;
                sprite->spreadZ = taskPtr->spreadZ;
                sprite->index = (taskPtr->step % 3) * 2 + spriteIndex;
            }
        }
        if (taskPtr->step == 10) {
            Battle_PostActorHitOrHurtMessage(taskPtr->targetIndex);
        }
        if (taskPtr->step == 8) {
            sprite = (BattleFxSpecialEvilPoisonSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialEvilPoisonPulseEmitterTask);
            Battle_GetBoneWorldPos(taskPtr->targetIndex, g_BattleActorRecords[taskPtr->targetIndex].targetBone, &sprite->posX);
        }
        (taskPtr->step)++;
        if (taskPtr->step == 40) {
            taskPtr->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Emits pulsing ink droplet particles for Sumi.
 */
// FUNCTION: FF7 0x00553b88
void __cdecl BattleFx_SpecialEvilPoisonPulseEmitterTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[68];
        SVECTOR rotX;
        struct { unsigned char pad_to_8[0x8]; int projDepth; };
        struct { unsigned char pad_to_C[0xC]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_2C[0x2C]; SVECTOR pulseX; };
        struct { unsigned char pad_to_34[0x34]; PsxMatrix *billboardRecord; };
        struct { unsigned char pad_to_38[0x38]; BattleFxEvilPoisonVariantSprite *variantSprite; };
        struct { unsigned char pad_to_3C[0x3C]; BattleFxEvilPoisonPulseEmitterSlot *emitterSlot; };
        struct { unsigned char pad_to_40[0x40]; short pulseAngle; };
    } frame;
#pragma pack(pop)

    frame.emitterSlot = (BattleFxEvilPoisonPulseEmitterSlot *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecialEvilPoisonSpriteReq2.pulse.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(1, g_BattleSpecialEvilPoisonLoadedRes);
    g_BattleSpecialEvilPoisonSpriteReq2.pulse.drawObj = Gfx_ModelTableGetSpriteObject(1, g_BattleSpecialEvilPoisonLoadedRes);
    g_BattleSpecialEvilPoisonSpriteReq2.pulse.frame = 0;
    frame.emitterSlot->pulseRadius = SinFixed(frame.emitterSlot->frameCounter * 68) * 0x28 >> 12;
    frame.billboardRecord = (PsxMatrix *)BattleFx_InstallBillboardMatrix(&frame.emitterSlot->posX, frame.emitterSlot->pulseRadius * 0x190, 0);
    if (frame.billboardRecord->t[2] > 0) {
        g_BattleSpecialEvilPoisonDrawListHandle = BattleFx_SubmitSpriteDraw(&g_BattleSpecialEvilPoisonSpriteReq2.pulse, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialEvilPoisonDrawListHandle);
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.emitterSlot->frameCounter < 10) {
        frame.emitterSlot->pulseRadius += 4;
    } else {
        (frame.emitterSlot->pulseRadius)--;
    }
    frame.variantSprite = (BattleFxEvilPoisonVariantSprite *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialEvilPoisonVariantSpriteTask);
    frame.pulseAngle = rand() & 0xFFF;
    frame.pulseX.vx = CosFixed(frame.pulseAngle) * frame.emitterSlot->pulseRadius * 0xA >> 12;
    frame.pulseX.vy = SinFixed(frame.pulseAngle) * frame.emitterSlot->pulseRadius * 0xA >> 12;
    frame.pulseX.vz = 0;
    frame.rotX.vz = 0;
    frame.rotX.vx = frame.rotX.vz;
    frame.rotX.vy = rand() & 0xFFF;
    RotMatrix(&frame.rotX, &frame.rotMatrix);
    frame.rotMatrix.t[0] = frame.emitterSlot->posX;
    frame.rotMatrix.t[1] = frame.emitterSlot->posY;
    frame.rotMatrix.t[2] = frame.emitterSlot->posZ;
    SetRotMatrix(&frame.rotMatrix);
    SetTransMatrix(&frame.rotMatrix);
    RotTransPointToInt16(&frame.pulseX, &frame.variantSprite->posX, &frame.projDepth);
    frame.variantSprite->variant = (rand() % 4);
    frame.variantSprite->scaleIndex = frame.emitterSlot->pulseRadius;
    (frame.emitterSlot->frameCounter)++;
    if (frame.emitterSlot->frameCounter == 30) {
        frame.emitterSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Animates variant ink splash sprites for Sumi.
 */
// FUNCTION: FF7 0x00553dc1
void __cdecl BattleFx_SpecialEvilPoisonVariantSpriteTask(void)
{
    BattleFxEvilPoisonVariantSprite *p;
    int q; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */

    p = (BattleFxEvilPoisonVariantSprite *)BATTLE_CURRENT_TASK_SLOT;
    BattleFx_InstallBillboardMatrix(&p->posX, p->scaleIndex * 0xC8, 0);
    g_BattleSpecialEvilPoisonSpriteReq1.frame = p->frame;
    g_BattleSpecialEvilPoisonSpriteReq1.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(g_BattleSpecialEvilPoisonSpriteReq2.variantSprites[p->variant], g_BattleSpecialEvilPoisonLoadedRes);
    g_BattleSpecialEvilPoisonSpriteReq1.drawObj = Gfx_ModelTableGetSpriteObject(g_BattleSpecialEvilPoisonSpriteReq2.variantSprites[p->variant], g_BattleSpecialEvilPoisonLoadedRes);
    g_BattleSpecialEvilPoisonDrawListHandle = BattleFx_SubmitSpriteDraw(&g_BattleSpecialEvilPoisonSpriteReq1, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialEvilPoisonDrawListHandle);
    if (g_BattleIsPaused == 0 && g_BattleSpecialEvilPoisonSpriteReq1.spriteGroup != 0) {
        (p->frame)++;
        if (p->frame >= g_BattleSpecialEvilPoisonSpriteReq1.spriteGroup->header->frameCount) {
            p->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Animates drifting ink cloud sprites for Sumi.
 */
// FUNCTION: FF7 0x00553eb6
void __cdecl BattleFx_SpecialEvilPoisonDriftingSpriteTask(void)
{
    BattleFxRefuseSpitDriftSprite *sprite;
    PsxMatrix *matrixResult;

    sprite = (BattleFxRefuseSpitDriftSprite *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecialEvilPoisonSpriteReq2.driftSpriteGroup = Gfx_ModelTableGetLoadedRecord(0, g_BattleSpecialEvilPoisonLoadedRes);
    g_BattleSpecialEvilPoisonSpriteReq2.driftDrawObj = Gfx_ModelTableGetSpriteObject(0, g_BattleSpecialEvilPoisonLoadedRes);
    matrixResult = (PsxMatrix *)BattleFx_InstallBillboardMatrix(&sprite->posX, g_BattleSpecialEvilPoisonScales[sprite->scaleIndex] * 6 >> 2, 0);
    if (matrixResult->t[2] > 0) {
        g_BattleSpecialEvilPoisonDrawListHandle = BattleFx_SubmitSpriteDraw(&g_BattleSpecialEvilPoisonSpriteReq2.driftSpriteGroup, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialEvilPoisonDrawListHandle);
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    sprite->posX += sprite->velocityX;
    sprite->posY += sprite->velocityY;
    sprite->posZ += sprite->velocityZ;
    (sprite->frameIndex)++;
    if (sprite->frameIndex == 12) {
        sprite->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Alternate entry point thunk for the Sumi special effect.
 */
// FUNCTION: FF7 0x00553fc8
void __cdecl BattleFx_SpecialEnterEvilPoisonAlt(int targetMask, int actorIdx)
{
    BattleFx_SpecialLoadResEvilPoison();
    Battle_CreateBitPatternCallbackTask(targetMask, actorIdx, 4, BattleFx_SpecialSpawnEvilPoisonPerTarget);
    Battle_PlaySfxPannedToActor(0x8A, (short)actorIdx, 0x40);
    g_BattleSpecialEvilPoisonMode = BATTLE_EVIL_POISON_MODE_ALT;
}
} /* extern "C" */
