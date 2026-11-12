// src/ff7/src/battle/battle_fx_special_grand_spark.cpp
/* Enemy Special Effect [idx 66]: GrandSpark (JP: jibasiri / 地走り (Ground Runner), asset: "SPECIAL/JIBASIRI") — Grand Spark (0x1cd, Turks:Rude / Capparwire) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle special effect 'Jibasiri': a per-target timeline with animated, bouncing,
//   projectile and fixed-animation sprites, a spinning growing model and a streak model.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/api/battle.h"
#include "ff7/battle_tasks.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_special.h"
#include "ff7/camera_script.h"
#include "lib/types/graphics.h"
#include "lib/enums/sound.h"
#include <string.h>
#include <stdlib.h>
#include "lib/api/sound.h"
#include "lib/codegen.h"
extern "C" {

struct BattleFxModelDraw;
// clang-format off
// GLOBAL: FF7 0x0089e658
int g_BattleSpecGrandSparkResourceDefs = 0x001900B1;
int g_BattleSpecGrandSparkResourceDefsHeadPtr[1] = {
    (int)((char *)&g_BattleFxSpecialGrandSparkResourceNames + 0x0)
};
// GLOBAL: FF7 0x0089e660
int g_BattleSpecGrandSparkResourceModelRows[30] = {
    0x0, 0x0, 0x0, 0x1900b1, (int)((char *)&g_BattleFxSpecialGrandSparkResourceNames + 0xc), 0x0,
    0x0, 0x0, 0x192a39, (int)((char *)&g_BattleFxSpecialGrandSparkResourceNames + 0x18), (int)((char *)&g_BattleFxSpecialGrandSparkResourceNames + 0x24), 0x0,
    0x0, 0x192a39, 0x0, (int)((char *)&g_BattleFxSpecialGrandSparkResourceNames + 0x30), 0x0, 0x0,
    0x192a39, (int)((char *)&g_BattleFxSpecialGrandSparkResourceNames + 0x3c), (int)((char *)&g_BattleFxSpecialGrandSparkResourceNames + 0x48), 0x0, 0x0, 0x192a39,
    0x0, (int)((char *)&g_BattleFxSpecialGrandSparkResourceNames + 0x54), 0x0, 0x0, 0x0, 0x0
};
// GLOBAL: FF7 0x0089e6d8
unsigned char g_BattleSpecGrandSparkSpriteReq1[8] = {0x80, 0x80, 0x80, 0x2c, 0x00, 0x00, 0x00, 0x00};
#undef g_BattleSpecGrandSparkSpriteReq2
// GLOBAL: FF7 0x0089e6e0
struct BattleSpecGrandSparkSpriteReq2 g_BattleSpecGrandSparkSpriteReq2 = {0, {0x28, 0, 0, 0, 0, 0}, 128, {0x20, 0, 0, 0}, 0, 0, {0x80, 0x80, 0x80, 0x2e}, 0, {0}, 0, 0};
// GLOBAL: FF7 0x0089e708
struct BattleSpecPoisonGasSpriteReq2 g_BattleSpecGrandSparkSpriteReq3 = {-128, -128, -128, 44, 0, {0}};
// GLOBAL: FF7 0x0089e710
struct BattleSpecGrandSparkModelScale g_BattleSpecGrandSparkModelScale = {{0, 4, 0, 0xfc, 0, 0, 0, 0}, 0, {0x28, 0, 0, 0, 0, 0}, 128};
unsigned char g_rawData_0089e724[4] = {0x20, 0x00, 0x00, 0x00};
unsigned char g_BattleFxSpecialGrandSparkResourceNames[96] =
    "GEKI1.RSD\0\0\0"
    "BAKU11.RSD\0\0"
    "JI01.TIM\0\0\0\0"
    "BAKU1.SPT\0\0\0"
    "KEMU1.SPT\0\0\0"
    "JI00.TIM\0\0\0\0"
    "HAHEN1.SPT\0\0"
    "KEMU1.SPT";
// GLOBAL: FF7 0x00b5dd88
int g_BattleSpecGrandSparkSourceBonePos = 0;
// GLOBAL: FF7 0x00b5dd8c
short g_BattleSpecGrandSparkSourceBonePosZ = 0;
// GLOBAL: FF7 0x00b5dd90
int g_BattleSpecGrandSparkDrawListHead = 0;
unsigned char g_bssPad_00b5dd94[4] = {0};
// GLOBAL: FF7 0x00b5dd98
int g_BattleSpecGrandSparkScaleX = 0;
// GLOBAL: FF7 0x00b5dd9c
int g_BattleSpecGrandSparkScaleY = 0;
// GLOBAL: FF7 0x00b5dda0
int g_BattleSpecGrandSparkScaleZ = 0;
unsigned char g_bssPad_00b5dda4[4] = {0};
// GLOBAL: FF7 0x00b5dda8
struct GfxTableCursor *g_BattleSpecGrandSparkLoadedResTable = 0;
// GLOBAL: FF7 0x00b5ddac
short g_BattleSpecGrandSparkFlashCounter = 0;
// clang-format on

#pragma intrinsic(memset)
#pragma intrinsic(memcpy)

// clang-format off
// clang-format on


/*
 * When the engine context exists and g_BattleSpecGrandSparkLoadedResTable is empty, builds SPECIAL/JIBASIRI with a
 * default-duration record and creates resource type 6 using g_BattleSpecGrandSparkResourceDefs.
 */
// FUNCTION: FF7 0x00525ec0
void __cdecl BattleFx_SpecialLoadResGrandSpark(void)
{
    GfxModelTableBank *curOwnerCtx;
    GfxResourceRequest currentRecord;
    int kind;
    char outSize[200];
    GfxResourceRequest *pRequest;
    int resourceDefsWord;

    if (g_pBattle3dScene != 0 && g_BattleSpecGrandSparkLoadedResTable == 0) {
        kind = 6;
        resourceDefsWord = (int)&g_BattleSpecGrandSparkResourceDefs;
        curOwnerCtx = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRequest = &currentRecord;
        Gfx_InitResourceRequest(pRequest);
        pRequest->groupTag = 0;
        // STRING: FF7 0x0089e788
        pRequest->resourcePath = (char *)BattleFx_BuildResourcePath("SPECIAL/JIBASIRI", outSize);
        Gfx_LoadModelTableIfAbsent(kind, resourceDefsWord, pRequest, &g_BattleSpecGrandSparkLoadedResTable, curOwnerCtx);
    }
}

/*
 * Forwards both arguments unchanged to BattleFx_SpecialBeginGrandSpark; it adds nothing of its own.
 */
// FUNCTION: FF7 0x00525f50
void __cdecl BattleFx_SpecialEnterGrandSpark(int targetMask, int actorIdx)
{
    BattleFx_SpecialBeginGrandSpark(targetMask, actorIdx);
}

/*
 * Loads SPECIAL/JIBASIRI through BattleFx_SpecialLoadResGrandSpark and creates a mode-four actor-mask callback task
 * using both arguments and BattleFx_SpecialSpawnGrandSparkPerTarget.
 */
// FUNCTION: FF7 0x00525f65
void __cdecl BattleFx_SpecialBeginGrandSpark(int targetMask, int actorIdx)
{
    BattleFx_SpecialLoadResGrandSpark();
    Battle_CreateBitPatternCallbackTask(targetMask, actorIdx, 4, BattleFx_SpecialSpawnGrandSparkPerTarget);
}

/*
 * Allocates a slot for BattleFx_SpecialGrandSparkTimelineTask, writes the source actor's target bone position to
 * g_BattleSpecGrandSparkSourceBonePos and the target actor's bone position to the slot's posX/posY/posZ, and stores
 * actor in the slot's actorIndex.
 */
// FUNCTION: FF7 0x00525f86
void __cdecl BattleFx_SpecialSpawnGrandSparkPerTarget(int actor, int sourceActor)
{
    BattleFxGrandSparkParticle *slot;

    slot = (BattleFxGrandSparkParticle *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialGrandSparkTimelineTask);
    Battle_GetBoneWorldPos(sourceActor, g_BattleActorRecords[sourceActor].targetBone, (short *)&g_BattleSpecGrandSparkSourceBonePos);
    Battle_GetBoneWorldPos(actor, g_BattleActorRecords[actor].targetBone, (short *)((int)slot + 0xC));
    slot->actorIndex = actor;
}

/*
 * Timeline task driving screen flashes, ground-streak models, and bouncing debris for Jibasiri.
 */
// FUNCTION: FF7 0x00525ffa
void __cdecl BattleFx_SpecialGrandSparkTimelineTask(void)
{
    short idx;
    short heading;
    BattleFxGrandSparkParticle *pParticle;
    int s; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int t; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int u; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int v; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int w; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int x; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int y; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int z; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxGrandSparkParticle *mainSelf;
    int b; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int c; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int d; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int e; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int f; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int g; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */

    mainSelf = (BattleFxGrandSparkParticle *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleOverlayDrawListHead = BattleFx_FlashScreenWhite(*(char *)&g_BattleSpecGrandSparkFlashCounter, 0, 0);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (mainSelf->phase < 0x10) {
        g_BattleSpecGrandSparkFlashCounter += 4;
    }
    if (mainSelf->phase > 0xE) {
        g_BattleSpecGrandSparkFlashCounter -= 4;
    }
    if (mainSelf->phase == 0) {
        pParticle = (BattleFxGrandSparkParticle *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialGrandSparkProjectileTask);
        pParticle->spawnX = *(short *)&g_BattleSpecGrandSparkSourceBonePos;
        pParticle->spawnY = 0;
        pParticle->spawnZ = g_BattleSpecGrandSparkSourceBonePosZ;
        pParticle->posX = (mainSelf->posX - *(short *)&g_BattleSpecGrandSparkSourceBonePos) / 0xA;
        pParticle->posZ = (mainSelf->posZ - g_BattleSpecGrandSparkSourceBonePosZ) / 0xA;
        pParticle->posY = 0;
        SoundDispatchCommand(SOUND_SFX_PLAY_CH1, 0x40, 0x173, 0, 0, 0, 0, 0, 0);
    }
    if (mainSelf->phase == 0xA) {
        for (idx = 0; idx < 8; idx++) {
            pParticle = (BattleFxGrandSparkParticle *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialGrandSparkBouncingSpriteTask);
            pParticle->spawnX = mainSelf->posX + rand() % 0x190 + -0xC8;
            pParticle->spawnZ = mainSelf->posZ + rand() % 0x190 + -0xC8;
            pParticle->spawnY = 0;
            heading = rand() % 0x1000;
            pParticle->posX = CosFixed(heading) * 0x64 >> 0xC;
            pParticle->posZ = SinFixed(heading) * 0x64 >> 0xC;
            pParticle->posY = -rand() % 0xC8 - 0x64;
            pParticle->variant = rand() % 0x14;
        }
        pParticle = (BattleFxGrandSparkParticle *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialGrandSparkGrowingSpinModelTask);
        pParticle->spawnX = mainSelf->posX;
        pParticle->spawnY = 0;
        pParticle->spawnZ = mainSelf->posZ;
        SoundDispatchCommand(SOUND_SFX_PLAY_CH2, 0x40, 0xFA, 0, 0, 0, 0, 0, 0);
    }
    if (mainSelf->phase > 6 && mainSelf->phase < 0xE) {
        pParticle = (BattleFxGrandSparkParticle *)&g_BattleTaskSlotBuffer.secondarySlots[(short)Battle_AllocateSecondaryResourceSlot(BattleFx_SpecialGrandSparkAnimatedSpriteTask)];
        pParticle->spawnX = mainSelf->posX + rand() % 0x4B0 + -0x258;
        pParticle->spawnY = mainSelf->posY + rand() % 0x4B0 + -0x258;
        pParticle->spawnZ = mainSelf->posZ + rand() % 0x4B0 + -0x258;
        pParticle->variant = 0;
        pParticle = (BattleFxGrandSparkParticle *)&g_BattleTaskSlotBuffer.secondarySlots[(short)Battle_AllocateSecondaryResourceSlot(BattleFx_SpecialGrandSparkAnimatedSpriteTask)];
        pParticle->spawnX = mainSelf->posX + rand() % 0x640 + -0x320;
        pParticle->spawnY = mainSelf->posY + rand() % 0x640 + -0x320;
        pParticle->spawnZ = mainSelf->posZ + rand() % 0x640 + -0x320;
        pParticle->variant = 1;
        g_BattleCameraFocalPoint.focalPointY += (short)(0x20 - (mainSelf->phase % 2 << 6));
    }
    if (mainSelf->phase == 0xC) {
        Battle_PostActorHitOrHurtMessage(mainSelf->actorIndex);
    }
    (mainSelf->phase)++;
    if (mainSelf->phase == 0x1E) {
        mainSelf->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Per-frame draw for the current secondary task slot. Installs a billboard matrix from the slot's position at scale
 * 0x2000 and picks table entry {2, 3}[variant]: the sprite group from Gfx_ModelTableGetLoadedRecord and the draw object from
 * Gfx_ModelTableGetSpriteObject of g_BattleSpecGrandSparkLoadedResTable, or from Battle3d_GetSceneRenderObject(3,
 * g_pBattle3dScene) when variant is 1. It copies the slot's frame into the sprite request and submits it through
 * g_BattleOverlayDrawListHead. Unless paused, and when the sprite group is non-null, it advances the frame and frees
 * the slot once the frame reaches the sprite group's frame count.
 */
// FUNCTION: FF7 0x0052644c
void __cdecl BattleFx_SpecialGrandSparkAnimatedSpriteTask(void)
{
    BattleFxTailLaserAnimSprite *task;
    int entryIds[2];

    task = (BattleFxTailLaserAnimSprite *)&g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx];
    entryIds[0] = 2;
    entryIds[1] = 3;
    BattleFx_InstallBillboardMatrix((void *)((int)task + 4), 0x2000, 0);
    ((BattleSpecGrandSparkModelRows *)&g_BattleSpecGrandSparkResourceModelRows)->spriteEntry = Gfx_ModelTableGetLoadedRecord(entryIds[task->variant], g_BattleSpecGrandSparkLoadedResTable);
    if (task->variant != 1) {
        ((BattleSpecGrandSparkModelRows *)&g_BattleSpecGrandSparkResourceModelRows)->sceneObject = Gfx_ModelTableGetSpriteObject(entryIds[task->variant], g_BattleSpecGrandSparkLoadedResTable);
    } else {
        ((BattleSpecGrandSparkModelRows *)&g_BattleSpecGrandSparkResourceModelRows)->sceneObject = Battle3d_GetSceneRenderObject(3, g_pBattle3dScene);
    }
    *(short *)((char *)&g_BattleSpecGrandSparkSpriteReq1 + 0x4) = task->frame;
    g_BattleOverlayDrawListHead = BattleFx_SubmitSpriteDraw(&((BattleSpecGrandSparkModelRows *)&g_BattleSpecGrandSparkResourceModelRows)->spriteEntry, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused == 0 && ((BattleSpecGrandSparkModelRows *)&g_BattleSpecGrandSparkResourceModelRows)->spriteEntry != 0) {
        (task->frame)++;
        if (task->frame >= *(int *)(**(int **)&((BattleSpecGrandSparkModelRows *)&g_BattleSpecGrandSparkResourceModelRows)->spriteEntry + 4)) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Scales, spins, and draws the erupting shock model for the Jibasiri ground wave.
 */
// FUNCTION: FF7 0x00526564
void __cdecl BattleFx_SpecialGrandSparkGrowingSpinModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[80];
        struct { unsigned char pad_to_4[0x4]; SVECTOR angleX; };
        struct { unsigned char pad_to_C[0xC]; PsxMatrix spinMatrix; };
        struct { unsigned char pad_to_2C[0x2C]; BattleFxOilTargetModelSlot *spinSlot; };
        struct { unsigned char pad_to_30[0x30]; PsxMatrix worldMatrix; };
    } frame;
#pragma pack(pop)

    frame.spinSlot = (BattleFxOilTargetModelSlot *)BATTLE_CURRENT_TASK_SLOT;
    (g_BattleSpecGrandSparkSpriteReq2.modelResource) = Gfx_ModelTableGetRenderObject(1, g_BattleSpecGrandSparkLoadedResTable);
    frame.angleX.vx = 0;
    frame.angleX.vy = frame.spinSlot->frameCounter << 6;
    frame.angleX.vz = 0;
    RotMatrix(&frame.angleX, &frame.spinMatrix);
    g_BattleSpecGrandSparkScaleZ = (frame.spinSlot->frameCounter << 8) + 0x3000;
    g_BattleSpecGrandSparkScaleX = g_BattleSpecGrandSparkScaleZ;
    g_BattleSpecGrandSparkScaleY = (frame.spinSlot->frameCounter << 8) + 0x2000;
    ScaleMatrix(&frame.spinMatrix, &g_BattleSpecGrandSparkScaleX);
    frame.spinMatrix.t[0] = frame.spinSlot->posX;
    frame.spinMatrix.t[1] = frame.spinSlot->posY;
    frame.spinMatrix.t[2] = frame.spinSlot->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.spinMatrix, &frame.worldMatrix);
    SetRotMatrix(&frame.worldMatrix);
    SetTransMatrix(&frame.worldMatrix);
    g_BattleSpecGrandSparkSpriteReq2.alpha = 0x80 - (frame.spinSlot->frameCounter << 2);
    g_BattleSpecGrandSparkDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecGrandSparkSpriteReq2, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecGrandSparkDrawListHead);
    if (g_BattleIsPaused != 0) {
        return;
    }
    (frame.spinSlot->frameCounter)++;
    if (frame.spinSlot->frameCounter == 0x1E) {
        frame.spinSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Animates parabolic bounce trajectories and submits spark sprites for Jibasiri.
 */
// FUNCTION: FF7 0x005266c3
void __cdecl BattleFx_SpecialGrandSparkBouncingSpriteTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[92];
        int phase;
        struct { unsigned char pad_to_4[0x4]; SVECTOR angleX; };
        struct { unsigned char pad_to_C[0xC]; PsxMatrix angleMatrix; };
        struct { unsigned char pad_to_2C[0x2C]; short phaseWord; };
        struct { unsigned char pad_to_30[0x30]; PsxMatrix scaleMatrix; };
        struct { unsigned char pad_to_50[0x50]; short axisIndex; };
        struct { unsigned char pad_to_54[0x54]; int screenDepth; };
        struct { unsigned char pad_to_58[0x58]; BattleFxTailLaserBouncingSpriteSlot *spriteSlot; };
    } frame;
#pragma pack(pop)

    frame.spriteSlot = (BattleFxTailLaserBouncingSpriteSlot *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecGrandSparkSpriteReq2.res32 = Gfx_ModelTableGetLoadedRecord(4, g_BattleSpecGrandSparkLoadedResTable);
    g_BattleSpecGrandSparkSpriteReq2.res36 = Gfx_ModelTableGetSpriteObject(4, g_BattleSpecGrandSparkLoadedResTable);
    for (frame.axisIndex = 0; frame.axisIndex < 3; frame.axisIndex++) {
        frame.scaleMatrix.m[2][frame.axisIndex] = 0;
        frame.scaleMatrix.m[1][frame.axisIndex] = 0;
        frame.scaleMatrix.m[0][frame.axisIndex] = 0;
    }
    if (frame.spriteSlot->frameCounter < 7) {
        frame.phase = 8;
    } else {
        frame.phase = 0xF - frame.spriteSlot->frameCounter;
    }
    frame.phaseWord = *(short *)&frame.phase;
    frame.angleX.vy = 0;
    frame.angleX.vx = frame.angleX.vy;
    frame.angleX.vz = frame.spriteSlot->angle;
    SetRotMatrix(&g_BattleCameraTransform);
    SetTransMatrix(&g_BattleCameraTransform);
    RotTransPoint((char *)frame.spriteSlot + 4, frame.scaleMatrix.t, &frame.screenDepth);
    frame.scaleMatrix.m[2][2] = frame.spriteSlot->variant < 4 ? 0x1400 : 0x2000;
    frame.scaleMatrix.m[1][1] = frame.scaleMatrix.m[2][2];
    frame.scaleMatrix.m[0][0] = frame.scaleMatrix.m[1][1];
    RotMatrix(&frame.angleX, &frame.angleMatrix);
    ConcatMatrixIntoFirst(&frame.scaleMatrix, &frame.angleMatrix);
    SetRotMatrix(&frame.scaleMatrix);
    SetTransMatrix(&frame.scaleMatrix);
    g_BattleSpecGrandSparkSpriteReq3.frame = frame.spriteSlot->variant;
    g_BattleSpecGrandSparkSpriteReq3.code = (frame.phaseWord - 8 != 0 ? 2 : 0) + 0x2C;
    g_BattleSpecGrandSparkSpriteReq3.blue = frame.phaseWord << 4;
    g_BattleSpecGrandSparkSpriteReq3.green = g_BattleSpecGrandSparkSpriteReq3.blue;
    (g_BattleSpecGrandSparkSpriteReq3.red) = g_BattleSpecGrandSparkSpriteReq3.green;
    g_BattleOverlayDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecGrandSparkSpriteReq2.res32, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.spriteSlot->posX += frame.spriteSlot->velocityX;
    frame.spriteSlot->posY += frame.spriteSlot->velocityY;
    frame.spriteSlot->posZ += frame.spriteSlot->velocityZ;
    if (frame.spriteSlot->posY > 0) {
        frame.spriteSlot->posY = 0;
        frame.spriteSlot->velocityY *= -1;
    }
    frame.spriteSlot->velocityY += 0x14;
    frame.spriteSlot->angle += 0x200;
    (frame.spriteSlot->frameCounter)++;
    if (frame.spriteSlot->frameCounter == 0xF) {
        frame.spriteSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * When unpaused emits BattleFx_SpecialGrandSparkStreakModelTask with the parent X/Z in child velocity fields and current
 * counter, then integrates parent X/Z. Emits BattleFx_SpecialGrandSparkBouncingSpriteTask there with random radial velocity of
 * magnitude 100, negative Y velocity 100 through 299 and lifetime four through nineteen, then
 * BattleFx_SpecialGrandSparkFixedAnimSpriteTask with XZ jitter -300 through 299. Advances the parent counter and retires exactly at
 * twelve.
 */
// FUNCTION: FF7 0x00526957
void __cdecl BattleFx_SpecialGrandSparkProjectileTask(void)
{
    BattleFxSwampShootDriver *taskPtr;
    short angle;
    BattleFxSwampShootTrailEmitter *child;

    taskPtr = (BattleFxSwampShootDriver *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    child = (BattleFxSwampShootTrailEmitter *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialGrandSparkStreakModelTask);
    child->velocityX = taskPtr->posX;
    child->velocityZ = taskPtr->posZ;
    child->velocityY = 0;
    child->frameIndex = taskPtr->frameIndex;
    taskPtr->posX += taskPtr->velocityX;
    taskPtr->posZ += taskPtr->velocityZ;
    child = (BattleFxSwampShootTrailEmitter *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialGrandSparkBouncingSpriteTask);
    child->posX = taskPtr->posX;
    child->posZ = taskPtr->posZ;
    child->posY = 0;
    angle = rand() % 0x1000;
    child->velocityX = CosFixed(angle) * 0x64 >> 0xC;
    child->velocityZ = SinFixed(angle) * 0x64 >> 0xC;
    child->velocityY = -rand() % 0xC8 - 0x64;
    child->frameIndex = rand() % 0x10 + 4;
    child = (BattleFxSwampShootTrailEmitter *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialGrandSparkFixedAnimSpriteTask);
    child->posX = taskPtr->posX + rand() % 0x258 + -0x12C;
    child->posZ = taskPtr->posZ + rand() % 0x258 + -0x12C;
    child->posY = 0;
    (taskPtr->frameIndex)++;
    if (taskPtr->frameIndex == 0xC) {
        taskPtr->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Updates the current task slot using animation 5 of g_BattleSpecGrandSparkLoadedResTable: the sprite group and draw
 * object go into the sprite request inside g_BattleSpecGrandSparkSpriteReq2, the billboard is installed at scale 0x2000
 * from the slot's position, the slot's frameCounter is the request's frame, and the draw is chained through
 * g_BattleSpecGrandSparkDrawListHead. When unpaused with a non-null sprite group it advances frameCounter and frees the
 * slot when it reaches the group's frame count. The second local is never read; it is present because the original's
 * stack frame reserves the slot.
 */
// FUNCTION: FF7 0x00526b4d
void __cdecl BattleFx_SpecialGrandSparkFixedAnimSpriteTask(void)
{
    BattleTaskSlot *p;
    int q; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */

    p = BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecGrandSparkSpriteReq2.res16 = Gfx_ModelTableGetLoadedRecord(5, g_BattleSpecGrandSparkLoadedResTable);
    g_BattleSpecGrandSparkSpriteReq2.res20 = Gfx_ModelTableGetSpriteObject(5, g_BattleSpecGrandSparkLoadedResTable);
    BattleFx_InstallBillboardMatrix((void *)((int)p + 4), 0x2000, 0);
    g_BattleSpecGrandSparkSpriteReq2.frame = p->frameCounter;
    g_BattleSpecGrandSparkDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecGrandSparkSpriteReq2.res16, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecGrandSparkDrawListHead);
    if (g_BattleIsPaused == 0 && g_BattleSpecGrandSparkSpriteReq2.res16 != 0) {
        (p->frameCounter)++;
        if (p->frameCounter >= *(int *)((char *)*(int *)g_BattleSpecGrandSparkSpriteReq2.res16 + 4)) {
            p->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Orients and draws a ground-hugging energy streak model between caster and target for Jibasiri.
 */
// FUNCTION: FF7 0x00526c20
void __cdecl BattleFx_SpecialGrandSparkStreakModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[128];
        PsxMatrix streakMatrix;
        struct { unsigned char pad_to_20[0x20]; SVECTOR relX; };
        struct { unsigned char pad_to_28[0x28]; BattleFxGrandSparkStreakModelSlot *streakSlot; };
        struct { unsigned char pad_to_2C[0x2C]; PsxMatrix lookBasis; };
        struct { unsigned char pad_to_4C[0x4C]; PsxMatrix worldMatrix; };
        struct { unsigned char pad_to_70[0x70]; VECTOR scaleVec; };
    } frame;
#pragma pack(pop)

    frame.streakSlot = (BattleFxGrandSparkStreakModelSlot *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecGrandSparkModelScale.drawRecord = Gfx_ModelTableGetRenderObject(0, g_BattleSpecGrandSparkLoadedResTable);
    frame.relX.vx = frame.streakSlot->posX - *(short *)&g_BattleSpecGrandSparkSourceBonePos;
    frame.relX.vy = 0;
    frame.relX.vz = frame.streakSlot->posZ - g_BattleSpecGrandSparkSourceBonePosZ;
    Gte_BuildLookBasisFromDirection(&frame.relX.vx, frame.lookBasis.m[0]);
    RotMatrix((SVECTOR *)&g_BattleSpecGrandSparkModelScale, &frame.streakMatrix);
    ConcatMatrixPreservingTranslation(&frame.lookBasis, &frame.streakMatrix, &frame.streakMatrix);
    frame.scaleVec.vz = frame.streakSlot->trailIndex * 0x180 + 0xE00;
    frame.scaleVec.vy = frame.scaleVec.vz;
    frame.scaleVec.vx = frame.scaleVec.vy;
    ScaleMatrix(&frame.streakMatrix, &frame.scaleVec);
    frame.streakMatrix.t[0] = frame.streakSlot->posX;
    frame.streakMatrix.t[1] = 0;
    frame.streakMatrix.t[2] = frame.streakSlot->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.streakMatrix, &frame.worldMatrix);
    SetRotMatrix(&frame.worldMatrix);
    SetTransMatrix(&frame.worldMatrix);
    g_BattleSpecGrandSparkModelScale.offset = 0x60 - frame.streakSlot->frameCounter * 0x18;
    g_BattleSpecGrandSparkDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecGrandSparkModelScale.drawRecord, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecGrandSparkDrawListHead);
    if (g_BattleIsPaused != 0) {
        return;
    }
    (frame.streakSlot->frameCounter)++;
    if (frame.streakSlot->frameCounter == 4) {
        frame.streakSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}
} /* extern "C" */
