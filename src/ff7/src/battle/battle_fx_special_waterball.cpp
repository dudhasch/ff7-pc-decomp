// src/ff7/src/battle/battle_fx_special_waterball.cpp
/* Enemy Special Effect [idx 74]: Waterball (JP: mizufuki / 水吹き (Water Spray/Jet), asset: "SPECIAL/MIZUFUKI") — Waterball (0x1d2..0x1d4, Bottomswell) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle special effect for the Mizufuki spell. It loads its resources, spawns a
//   per-target effect, and runs the timeline, spiral, falling, and projectile sprite tasks, and the
//   spin model task.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/api/battle.h"
#include "ff7/battle_tasks.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_actors.h"
#include "ff7/camera_script.h"
#include "lib/types/graphics.h"
#include <string.h>
#include <stdlib.h>
#include "lib/codegen.h"
extern "C" {

struct BattleFxModelDraw;
#pragma intrinsic(memset)
#pragma intrinsic(memcpy)

// clang-format off

// GLOBAL: FF7 0x0089a100
unsigned int g_BattleSpecWaterballResourceDef[50] = {0x001900b1, (unsigned int)&g_BattleSpecWaterballResName_nemoto_doku03, 0x00000000, 0x00000000, 0x00000000, 0x00192a39, (unsigned int)&g_BattleSpecWaterballResName_nemoto_doku03[12], (unsigned int)&g_BattleSpecWaterballResName_hassya_doku00, 0x00000000, 0x00000000, 0x00192a39, (unsigned int)&g_BattleSpecWaterballResName_hassya_doku00[12], (unsigned int)&g_BattleSpecWaterballResName_doku1_doku01, 0x00000000, 0x00000000, 0x00192a39, (unsigned int)&g_BattleSpecWaterballResName_doku1_doku01[12], (unsigned int)&g_BattleSpecWaterballResName_doku2_doku02, 0x00000000, 0x00000000, 0x00192a39, (unsigned int)&g_BattleSpecWaterballResName_doku2_doku02[12], (unsigned int)&g_BattleSpecWaterballResName_baku1_joc05, 0x00000000, 0x00000000, 0x00192a39, 0x00000000, (unsigned int)&g_BattleSpecWaterballResName_baku1_joc05[12], 0x00000000, 0x00000000, 0x00192a39, (unsigned int)&g_BattleSpecWaterballResName_doku00_snow1, (unsigned int)&g_BattleSpecWaterballResName_doku00_snow1[12], 0x00000000, 0x00000000, 0x00192a39, (unsigned int)&g_BattleSpecWaterballResName_doku00_snow2, (unsigned int)&g_BattleSpecWaterballResName_doku00_snow2[12], 0x00000000, 0x00000000, 0x00192a39, (unsigned int)&g_BattleSpecWaterballResName_doku00_mizu, (unsigned int)&g_BattleSpecWaterballResName_doku00_mizu[12], 0x00000000, 0x00000000, 0x00192a39, (unsigned int)&g_BattleSpecWaterballResName_doku00_tama, (unsigned int)&g_BattleSpecWaterballResName_doku00_tama[12], 0x00000000, 0x00000000};
// GLOBAL: FF7 0x0089a1c8
BattleFxSpriteRequest g_BattleSpecWaterballSpriteRequest1 = {0, 0, -128, -128, -128, 0x2c, 0, 0};
// GLOBAL: FF7 0x0089a1d8
BattleFxSpriteRequest g_BattleSpecWaterballSpriteRequest2 = {0, 0, -128, -128, -128, 0x2c, 0, 0};
// GLOBAL: FF7 0x0089a1e8
BattleFxSpriteRequest g_BattleSpecWaterballSpriteRequest3 = {0, 0, -128, -128, -128, 0x2c, 0, 0};
// GLOBAL: FF7 0x0089a1f8
BattleFxSpriteRequest g_BattleSpecWaterballSpriteRequest4 = {0, 0, -128, -128, -128, 0x2c, 0, 0};

// GLOBAL: FF7 0x0089a208
BattleFxSpriteRequest g_BattleSpecWaterballSpriteRequest5 = {0, 0, -128, -128, -128, 0x2c, 0, 0};

// GLOBAL: FF7 0x0089a218
struct BattleSpecWaterballTransformData g_BattleSpecWaterballTransformData = {{0, 0x50, 0, 0, 0, 0x50, 0, 0, 0, 0x50, 0, 0, 0, 0, 0, 0}, 0, {0x28, 0, 0, 0, 0, 0}, 0, {0x20, 0, 0, 0}};
// GLOBAL: FF7 0x0089a238
BattleFxSpriteRequest g_BattleSpecWaterballSpriteRequest6 = {0, 0, -128, -128, -128, 0x2e, 0, 0};

// GLOBAL: FF7 0x0089a248
unsigned char g_BattleSpecWaterballResName_nemoto_doku03[23] =
    "NEMOTO.RSD\0\0"
    "DOKU03.TIM";
// GLOBAL: FF7 0x0089a260
unsigned char g_BattleSpecWaterballResName_hassya_doku00[23] =
    "HASSYA.SPT\0\0"
    "DOKU00.TIM";
// GLOBAL: FF7 0x0089a278
unsigned char g_BattleSpecWaterballResName_doku1_doku01[23] =
    "DOKU1.SPT\0\0\0"
    "DOKU01.TIM";
// GLOBAL: FF7 0x0089a290
unsigned char g_BattleSpecWaterballResName_doku2_doku02[23] =
    "DOKU2.SPT\0\0\0"
    "DOKU02.TIM";
// GLOBAL: FF7 0x0089a2a8
unsigned char g_BattleSpecWaterballResName_baku1_joc05[23] =
    "BAKU1.SPT\0\0\0"
    "JO_C05.SPT";
// GLOBAL: FF7 0x0089a2c0
unsigned char g_BattleSpecWaterballResName_doku00_snow1[22] =
    "DOKU00.TIM\0\0"
    "SNOW1.SPT";
// GLOBAL: FF7 0x0089a2d8
unsigned char g_BattleSpecWaterballResName_doku00_snow2[22] =
    "DOKU00.TIM\0\0"
    "SNOW2.SPT";
// GLOBAL: FF7 0x0089a2f0
unsigned char g_BattleSpecWaterballResName_doku00_mizu[21] =
    "DOKU00.TIM\0\0"
    "MIZU.SPT";
// GLOBAL: FF7 0x0089a308
unsigned char g_BattleSpecWaterballResName_doku00_tama[21] =
    "DOKU00.TIM\0\0"
    "TAMA.SPT";
// GLOBAL: FF7 0x00b51ce0
short g_BattleSpecWaterballTimer = 0;
// GLOBAL: FF7 0x00b51ce4
short g_BattleSpecWaterballStep = 0;
// GLOBAL: FF7 0x00b51ce8
int g_BattleSpecWaterballTaskPtr = 0;
// GLOBAL: FF7 0x00b51cec
short g_BattleSpecWaterballActiveCount = 0;
// GLOBAL: FF7 0x00b51cf0
struct GfxTableCursor *g_BattleSpecWaterballLoadedResTable = 0;
// clang-format on


/*
 * Builds SPECIAL/MIZUFUKI and loads the Mizufuki resource table (type 10) if it is not loaded yet.
 */
// FUNCTION: FF7 0x005135b0
void __cdecl BattleFx_SpecialLoadResWaterball(void)
{
    GfxModelTableBank *ownerHandle;
    GfxResourceRequest recBlock;
    int kind;
    char pathDstBuf[200];
    GfxResourceRequest *pRequest;
    int resourceDefAddr;

    if (g_pBattle3dScene != 0 && g_BattleSpecWaterballLoadedResTable == 0) {
        kind = 0xA;
        resourceDefAddr = (int)&g_BattleSpecWaterballResourceDef;
        ownerHandle = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRequest = &recBlock;
        Gfx_InitResourceRequest(pRequest);
        pRequest->groupTag = 0;
        // STRING: FF7 0x0089a320
        pRequest->resourcePath = BattleFx_BuildResourcePath("SPECIAL/MIZUFUKI", pathDstBuf);
        Gfx_LoadModelTableIfAbsent(kind, resourceDefAddr, pRequest, &g_BattleSpecWaterballLoadedResTable, ownerHandle);
    }
}

/*
 * Entry point: clears the active-model counter, then begins the effect.
 */
// FUNCTION: FF7 0x00513640
void __cdecl BattleFx_SpecialEnterWaterball(int targetMask, int actorIdx)
{
    g_BattleSpecWaterballActiveCount = 0;
    BattleFx_SpecialBeginWaterball(targetMask, actorIdx);
}

/*
 * Loads the resources, queues the per-target spawner, clears the fade channels and starts the
 * fixed-point ramp (0xC00 over 10 frames).
 */
// FUNCTION: FF7 0x0051365e
void __cdecl BattleFx_SpecialBeginWaterball(int targetMask, int actorIdx)
{
    BattleFx_SpecialLoadResWaterball();
    Battle_CreateBitPatternCallbackTask(targetMask, actorIdx, 4, BattleFx_SpecialSpawnWaterballPerTarget);
    g_BattleFxTimelineState.channel2 = 0;
    g_BattleFxTimelineState.channel1 = g_BattleFxTimelineState.channel2;
    g_BattleFxTimelineState.red = g_BattleFxTimelineState.channel1;
    Battle_StartFixedPointRamp(0xC00, 10);
}

/*
 * Spawns the timeline task at the caster's bone and aims it at the target's bone: stores the
 * per-frame velocity and the frame on which the projectiles arrive.
 */
// FUNCTION: FF7 0x005136ab
void __cdecl BattleFx_SpecialSpawnWaterballPerTarget(int actor, int sourceIndex)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[52];
        int spawnTimer;
        struct { unsigned char pad_to_4[0x4]; VECTOR diffX; };
        struct { unsigned char pad_to_14[0x14]; VECTOR normX; };
        struct { unsigned char pad_to_24[0x24]; BattleFxWaterballParticle *particle; };
        struct { unsigned char pad_to_2C[0x2C]; SVECTOR targetX; };
    } frame;
#pragma pack(pop)

    frame.particle = (BattleFxWaterballParticle *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialWaterballTimelineTask);
    Battle_GetBoneWorldPos(sourceIndex, g_BattleActorRecords[sourceIndex].bone33, &frame.particle->posX);
    Battle_GetBoneWorldPos(actor, g_BattleActorRecords[actor].targetBone, &frame.targetX.vx);
    frame.diffX.vx = frame.targetX.vx - frame.particle->posX;
    frame.diffX.vy = frame.targetX.vy - frame.particle->posY;
    frame.diffX.vz = frame.targetX.vz - frame.particle->posZ;
    NormalizeVectorFixed(&frame.diffX, &frame.normX);
    frame.particle->deltaX = frame.normX.vx * 0x3E8 >> 12;
    frame.particle->deltaY = frame.normX.vy * 0x3E8 >> 12;
    frame.particle->deltaZ = frame.normX.vz * 0x3E8 >> 12;
    if (frame.particle->deltaZ == 0) {
        frame.spawnTimer = 1;
    } else {
        frame.spawnTimer = frame.diffX.vz / frame.particle->deltaZ;
    }
    g_BattleSpecWaterballTimer = *(short *)&frame.spawnTimer;
    g_BattleSpecWaterballStep = actor;
    frame.particle->step = g_BattleSpecWaterballStep;
    Battle_PlaySfxPannedToActor(0xEB, (short)sourceIndex, 0);
}

/*
 * Timeline: spawns the opening sprite, eight projectile sprites per frame while it launches,
 * the ring of target sprites, then the spin model at the target.
 */
// FUNCTION: FF7 0x005137e9
void __cdecl BattleFx_SpecialWaterballTimelineTask(void)
{
    int p; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxWaterballTrack *trackPtr;
    short baseParticleIdx;
    BattleFxWaterballParticle *spawn;
    int t; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int u; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int v; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int w; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int x; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int y; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int z; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int a; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    short theta;
    int c; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int d; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int e; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int f; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int g; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */

    trackPtr = (BattleFxWaterballTrack *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (trackPtr->frame == 0) {
        spawn = (BattleFxWaterballParticle *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialWaterballAnimatedSpriteTask);
        spawn->posX = trackPtr->posX;
        spawn->posY = trackPtr->posY;
        spawn->posZ = trackPtr->posZ;
    }
    if (trackPtr->frame < 5) {
        for (baseParticleIdx = 0; baseParticleIdx < 8; baseParticleIdx++) {
            spawn = (BattleFxWaterballParticle *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialWaterballProjectileSpriteTask);
            spawn->posX = trackPtr->posX;
            spawn->posY = trackPtr->posY;
            spawn->posZ = trackPtr->posZ;
            spawn->posX += (short)(trackPtr->velocityX * (7 - baseParticleIdx) / 8);
            spawn->posY += (short)(trackPtr->velocityY * (7 - baseParticleIdx) / 8);
            spawn->posZ += (short)(trackPtr->velocityZ * (7 - baseParticleIdx) / 8);
            spawn->deltaX = trackPtr->velocityX;
            spawn->deltaY = trackPtr->velocityY;
            spawn->deltaZ = trackPtr->velocityZ;
            spawn->stage = (5 - trackPtr->frame << 3) - baseParticleIdx;
            spawn->phase = rand() % 4096;
            spawn->variant = rand() % 2;
            spawn->timer = spawn->stage * 20 + rand() % 200;
            spawn->step = baseParticleIdx;
        }
    }
    if (trackPtr->frame == 55) {
        Battle_PostActorHitOrHurtMessage(trackPtr->boneIndex);
    }
    if (trackPtr->frame >= 10 && trackPtr->frame < 18) {
        theta = (trackPtr->frame - 0xA << 9) - 0x400;
        spawn = (BattleFxWaterballParticle *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialWaterballTargetAnimatedSpriteTask);
        Battle_GetBoneWorldPos(trackPtr->boneIndex, g_BattleActorRecords[trackPtr->boneIndex].targetBone, &spawn->posX);
        spawn->posX += (short)(CosFixed(theta) * 0x258 >> 12);
        spawn->posY += (short)(SinFixed(theta) * 0x258 >> 12);
        spawn->stage = 1;
    }
    if (trackPtr->frame == 15) {
        spawn = (BattleFxWaterballParticle *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialWaterballSpinModelTask);
        Battle_GetBoneWorldPos(trackPtr->boneIndex, g_BattleActorRecords[trackPtr->boneIndex].targetBone, &spawn->posX);
        spawn->posY = 0;
        Battle_PlaySfxPannedToActor(0x1BE, (short)trackPtr->boneIndex, 0);
    }
    (trackPtr->frame)++;
    if (trackPtr->frame == 70) {
        Battle_StartFixedPointRamp(0, 10);
        trackPtr->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Animated sprite at the target: picks table row 4 or 5 by variant (variant 1 uses the scene's
 * object instead of the row's), draws it and frees the task at the last frame.
 */
// FUNCTION: FF7 0x00513bad
void __cdecl BattleFx_SpecialWaterballTargetAnimatedSpriteTask(void)
{
    BattleFxSewerTsunamiAnimSprite *taskPtr;
    int rowSel[2];

    taskPtr = (BattleFxSewerTsunamiAnimSprite *)BATTLE_CURRENT_TASK_SLOT;
    rowSel[0] = 4;
    rowSel[1] = 5;
    BattleFx_InstallBillboardMatrix(&taskPtr->posX, 0x3000 /* 3.0 */, 0);
    g_BattleSpecWaterballSpriteRequest1.frame = taskPtr->frame;
    g_BattleSpecWaterballSpriteRequest1.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(rowSel[taskPtr->variant], g_BattleSpecWaterballLoadedResTable);
    if (taskPtr->variant != 1) {
        g_BattleSpecWaterballSpriteRequest1.drawObj = Gfx_ModelTableGetSpriteObject(rowSel[taskPtr->variant], g_BattleSpecWaterballLoadedResTable);
    } else {
        g_BattleSpecWaterballSpriteRequest1.drawObj = Battle3d_GetSceneRenderObject(3, g_pBattle3dScene);
    }
    g_BattleSpecWaterballTaskPtr = BattleFx_SubmitSpriteDraw(&g_BattleSpecWaterballSpriteRequest1, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecWaterballTaskPtr);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (g_BattleSpecWaterballSpriteRequest1.spriteGroup != 0) {
        (taskPtr->frame)++;
        if (taskPtr->frame >= g_BattleSpecWaterballSpriteRequest1.spriteGroup->header->frameCount) {
            taskPtr->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Animated sprite from table row 1: draws the current frame and frees the task at the last frame.
 */
// FUNCTION: FF7 0x00513cc7
void __cdecl BattleFx_SpecialWaterballAnimatedSpriteTask(void)
{
    BattleFxSewerTsunamiAnimSprite *task;

    task = (BattleFxSewerTsunamiAnimSprite *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecWaterballSpriteRequest2.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(1, g_BattleSpecWaterballLoadedResTable);
    g_BattleSpecWaterballSpriteRequest2.drawObj = Gfx_ModelTableGetSpriteObject(1, g_BattleSpecWaterballLoadedResTable);
    BattleFx_InstallBillboardMatrix(&task->posX, 0x2000 /* 2.0 */, 0);
    g_BattleSpecWaterballSpriteRequest2.frame = task->frame;
    g_BattleSpecWaterballTaskPtr = BattleFx_SubmitSpriteDraw(&g_BattleSpecWaterballSpriteRequest2, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecWaterballTaskPtr);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (g_BattleSpecWaterballSpriteRequest2.spriteGroup != 0) {
        (task->frame)++;
        if (task->frame >= g_BattleSpecWaterballSpriteRequest2.spriteGroup->header->frameCount) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Spinning model at the target; while it runs it spawns drifting, spiral and falling sprites.
 */
// FUNCTION: FF7 0x00513d9a
void __cdecl BattleFx_SpecialWaterballSpinModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[92];
        struct { unsigned char pad_to_4[0x4]; SVECTOR angleX; };
        struct { unsigned char pad_to_C[0xC]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_2C[0x2C]; BattleFxWaterballSpinModelSpawnSlot *spawnSlot; };
        struct { unsigned char pad_to_30[0x30]; short spawnIndex; };
        struct { unsigned char pad_to_34[0x34]; BattleFxOilTargetModelSlot *spinSlot; };
        struct { unsigned char pad_to_38[0x38]; PsxMatrix spinMatrix; };
        struct { unsigned char pad_to_58[0x58]; short randAngle; };
    } frame;
#pragma pack(pop)

    frame.spinSlot = (BattleFxOilTargetModelSlot *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecWaterballTransformData.record = Gfx_ModelTableGetRenderObject(0, g_BattleSpecWaterballLoadedResTable);
    frame.angleX.vz = 0;
    frame.angleX.vx = frame.angleX.vz;
    frame.angleX.vy = frame.spinSlot->frameCounter << 6;
    RotMatrix(&frame.angleX, &frame.rotMatrix);
    ScaleMatrix(&frame.rotMatrix, (int *)&g_BattleSpecWaterballTransformData);
    frame.rotMatrix.t[0] = frame.spinSlot->posX;
    frame.rotMatrix.t[1] = frame.spinSlot->posY + 0x190;
    frame.rotMatrix.t[2] = frame.spinSlot->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.spinMatrix);
    SetRotMatrix(&frame.spinMatrix);
    SetTransMatrix(&frame.spinMatrix);
    g_BattleSpecWaterballTransformData.alpha = g_BattleSpecWaterballActiveCount << 3;
    g_BattleSpecWaterballTaskPtr = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecWaterballTransformData.record, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecWaterballTaskPtr);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.spinSlot->frameCounter < 37) {
        for (frame.spawnIndex = 0; frame.spawnIndex < 2; frame.spawnIndex++) {
            frame.spawnSlot = (BattleFxWaterballSpinModelSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialWaterballDriftingAnimatedSpriteTask);
            frame.spawnSlot->posX = frame.spinSlot->posX;
            frame.spawnSlot->posY = frame.spinSlot->posY;
            frame.spawnSlot->posZ = frame.spinSlot->posZ;
            frame.spawnSlot->offsetX = SinFixed(frame.spinSlot->frameCounter * 192 + (frame.spawnIndex << 11)) * 0x46 >> 12;
            frame.spawnSlot->offsetZ = CosFixed(frame.spinSlot->frameCounter * 192 + (frame.spawnIndex << 11)) * 0x46 >> 12;
            frame.spawnSlot->offsetY = 0xFF4C;
        }
        frame.spawnSlot = (BattleFxWaterballSpinModelSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialWaterballSpiralSpriteTask);
        frame.spawnSlot->posX = frame.spinSlot->posX;
        frame.spawnSlot->posY = frame.spinSlot->posY;
        frame.spawnSlot->posZ = frame.spinSlot->posZ;
        frame.spawnSlot->startAngle = (rand() % 4096);
        frame.spawnSlot->startRadius = 0;
        frame.spawnSlot->spinDirection = (rand() % 2);
        for (frame.spawnIndex = 0; frame.spawnIndex < 2; frame.spawnIndex++) {
            frame.spawnSlot = (BattleFxWaterballSpinModelSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialWaterballFallingFadeSpriteTask);
            frame.spawnSlot->posX = frame.spinSlot->posX;
            frame.spawnSlot->posY = frame.spinSlot->posY;
            frame.spawnSlot->posZ = frame.spinSlot->posZ;
            frame.randAngle = (rand() % 4096);
            frame.spawnSlot->offsetX = SinFixed(frame.randAngle) * 0x78 >> 12;
            frame.spawnSlot->offsetZ = CosFixed(frame.randAngle) * 0x78 >> 12;
            frame.spawnSlot->offsetY = 0xFFFFFF9C - rand() % 100;
        }
    }
    if (frame.spinSlot->frameCounter < 16) {
        g_BattleSpecWaterballActiveCount++;
    }
    if (frame.spinSlot->frameCounter > 29) {
        g_BattleSpecWaterballActiveCount--;
    }
    (frame.spinSlot->frameCounter)++;
    if (frame.spinSlot->frameCounter == 45) {
        frame.spinSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Animated sprite from table row 3 that drifts by its velocity each frame and frees itself at the
 * last frame.
 */
// FUNCTION: FF7 0x00514131
void __cdecl BattleFx_SpecialWaterballDriftingAnimatedSpriteTask(void)
{
    BattleFxEnergyOrbDriftSprite *task;

    task = (BattleFxEnergyOrbDriftSprite *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecWaterballSpriteRequest3.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(3, g_BattleSpecWaterballLoadedResTable);
    g_BattleSpecWaterballSpriteRequest3.drawObj = Gfx_ModelTableGetSpriteObject(3, g_BattleSpecWaterballLoadedResTable);
    BattleFx_InstallBillboardMatrix(&task->posX, 0x2000 /* 2.0 */, 0);
    g_BattleSpecWaterballSpriteRequest3.frame = task->frameIndex;
    g_BattleSpecWaterballTaskPtr = BattleFx_SubmitSpriteDraw(&g_BattleSpecWaterballSpriteRequest3, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecWaterballTaskPtr);
    if (g_BattleIsPaused != 0) {
        return;
    }
    task->posX += task->velocityX;
    task->posY += task->velocityY;
    task->posZ += task->velocityZ;
    if (g_BattleSpecWaterballSpriteRequest3.spriteGroup != 0) {
        (task->frameIndex)++;
        if (task->frameIndex >= g_BattleSpecWaterballSpriteRequest3.spriteGroup->header->frameCount) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Spiral sprite: orbits its center at a growing radius while rising, draws table row 7 or 9 by
 * sprite index, and frees itself after 16 frames.
 */
// FUNCTION: FF7 0x00514243
void __cdecl BattleFx_SpecialWaterballSpiralSpriteTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[20];
        SVECTOR posX;
        struct { unsigned char pad_to_8[0x8]; int resSelect[2]; };
        struct { unsigned char pad_to_10[0x10]; BattleFxWaterballSpiralSpriteSlot *spiralSlot; };
    } frame;
#pragma pack(pop)

    frame.spiralSlot = (BattleFxWaterballSpiralSpriteSlot *)BATTLE_CURRENT_TASK_SLOT;
    frame.resSelect[0] = 7;
    frame.resSelect[1] = 9;
    frame.posX.vx = frame.spiralSlot->centerX + (SinFixed(frame.spiralSlot->angle) * frame.spiralSlot->radius >> 12);
    frame.posX.vz = frame.spiralSlot->centerZ + (CosFixed(frame.spiralSlot->angle) * frame.spiralSlot->radius >> 12);
    frame.posX.vy = frame.spiralSlot->posY;
    BattleFx_InstallBillboardMatrix(&frame.posX, 0x1000 /* 1.0 */, 0);
    g_BattleSpecWaterballSpriteRequest4.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(frame.resSelect[frame.spiralSlot->spriteIndex], g_BattleSpecWaterballLoadedResTable);
    g_BattleSpecWaterballSpriteRequest4.drawObj = Gfx_ModelTableGetSpriteObject(frame.resSelect[frame.spiralSlot->spriteIndex], g_BattleSpecWaterballLoadedResTable);
    g_BattleSpecWaterballTaskPtr = BattleFx_SubmitSpriteDraw(&g_BattleSpecWaterballSpriteRequest4, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecWaterballTaskPtr);
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.spiralSlot->posY -= 200;
    frame.spiralSlot->angle += 0x100 /* 22.5 deg */;
    frame.spiralSlot->radius += 100;
    (frame.spiralSlot->frameCounter)++;
    if (frame.spiralSlot->frameCounter == 16) {
        frame.spiralSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Falling sprite (table row 9): fades out over the last 8 of its 20 frames while it falls under
 * gravity.
 */
// FUNCTION: FF7 0x005143ae
void __cdecl BattleFx_SpecialWaterballFallingFadeSpriteTask(void)
{
    BattleFxLivewireDotSlot *dropPtr;
    int fadeStep;
    short shadeByte;

    dropPtr = (BattleFxLivewireDotSlot *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecWaterballSpriteRequest5.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(9, g_BattleSpecWaterballLoadedResTable);
    g_BattleSpecWaterballSpriteRequest5.drawObj = Gfx_ModelTableGetSpriteObject(9, g_BattleSpecWaterballLoadedResTable);
    if (dropPtr->step < 12) {
        fadeStep = 8;
    } else {
        fadeStep = 20 - dropPtr->step;
    }
    shadeByte = fadeStep;
    BattleFx_InstallBillboardMatrix(&dropPtr->posX, 0x800 /* 0.5 */, 0);
    g_BattleSpecWaterballSpriteRequest5.colorB = shadeByte << 4;
    g_BattleSpecWaterballSpriteRequest5.colorG = g_BattleSpecWaterballSpriteRequest5.colorB;
    g_BattleSpecWaterballSpriteRequest5.colorR = g_BattleSpecWaterballSpriteRequest5.colorG;
    g_BattleSpecWaterballTaskPtr = BattleFx_SubmitSpriteDraw(&g_BattleSpecWaterballSpriteRequest5, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecWaterballTaskPtr);
    if (g_BattleIsPaused != 0) {
        return;
    }
    dropPtr->posX += dropPtr->velocityX;
    dropPtr->posY += dropPtr->velocityY;
    dropPtr->posZ += dropPtr->velocityZ;
    dropPtr->velocityY += 10;
    (dropPtr->step)++;
    if (dropPtr->step == 20) {
        dropPtr->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Projectile sprite (table row 2): moves by its velocity; on the frame the projectiles arrive it
 * spawns a hit sprite at the target (unless its spawnIndex is nonzero) and frees itself.
 */
// FUNCTION: FF7 0x00514506
void __cdecl BattleFx_SpecialWaterballProjectileSpriteTask(void)
{
    BattleFxNapalmTrailSlot *hitSprite;
    BattleFxWaterballProjectileRecord *pCurrent;
    int r; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int s; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */

    pCurrent = (BattleFxWaterballProjectileRecord *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecWaterballSpriteRequest6.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(2, g_BattleSpecWaterballLoadedResTable);
    g_BattleSpecWaterballSpriteRequest6.drawObj = Gfx_ModelTableGetSpriteObject(2, g_BattleSpecWaterballLoadedResTable);
    BattleFx_InstallBillboardMatrix(&pCurrent->posX, pCurrent->scale * 0xC8 + 0x1000, 0);
    g_BattleSpecWaterballTaskPtr = BattleFx_SubmitSpriteDraw(&g_BattleSpecWaterballSpriteRequest6, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecWaterballTaskPtr);
    if (g_BattleIsPaused != 0) {
        return;
    }
    pCurrent->posX += pCurrent->velocityX;
    pCurrent->posY += pCurrent->velocityY;
    pCurrent->posZ += pCurrent->velocityZ;
    pCurrent->angle += 0x80;
    if (pCurrent->step == g_BattleSpecWaterballTimer) {
        if (pCurrent->spawnIndex == 0) {
            hitSprite = (BattleFxNapalmTrailSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialWaterballTargetAnimatedSpriteTask);
            Battle_GetBoneWorldPos(g_BattleSpecWaterballStep, g_BattleActorRecords[g_BattleSpecWaterballStep].targetBone, &hitSprite->posX);
            hitSprite->posX += (short)(rand() % 800 - 400);
            hitSprite->posY += (short)(rand() % 800 - 400);
            hitSprite->posZ += (short)(rand() % 800 - 400);
            hitSprite->variant = 0;
        }
        pCurrent->ownerSlot = BATTLE_TASK_FREE;
    }
    (pCurrent->step)++;
    if (pCurrent->step == 20) {
        pCurrent->ownerSlot = BATTLE_TASK_FREE;
    }
}
} /* extern "C" */
