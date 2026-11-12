// src/ff7/src/battle/battle_fx_blue_angel_whisper_effect.cpp
// Enemy Skill [5]: Angel Whisper & Life/Life2 revives (Japanese: エンジェルウィスパー / アレイズ Arise, dev tag: "magic/aleiz" / Aleiz)
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: The Aleiz blue magic effect: loads its resources and runs a primary and a secondary
//   sequence, with arcing and descending quads, actor fades, rising fades and an expanding model.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/enums/sound.h"
#include "ff7/api/battle.h"
#include "ff7/battle_tasks.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_fx.h"
#include "ff7/camera_script.h"
#include "lib/types/graphics.h"
#include <string.h>
#include <stdlib.h>
#include "lib/codegen.h"
extern "C" {

struct BattleFxModelDraw;
// clang-format off
// GLOBAL: FF7 0x008a8b80
int g_BattleBlueAngelWhisperResourceDef = 0x00192A39;
int g_BattleBlueAngelWhisperResourceDefHeadPtr[1] = {
    (int)((char *)&g_BattleFxBlueAngelWhisperEffectResourceNames + 0x0)
};
int g_BattleBlueAngelWhisperResourceDefDataTail[8] = {
    (int)((char *)&g_BattleFxBlueAngelWhisperEffectResourceNames + 0xc), 0x0, 0x0, 0x6400b1, (int)((char *)&g_BattleFxBlueAngelWhisperEffectResourceNames + 0x18), 0x0,
    0x0, 0x0
};
#undef g_BattleBlueAngelWhisperSpriteDrawReq1
// GLOBAL: FF7 0x008a8ba8
struct BattleBlueAngelWhisperSpriteDrawReq1 g_BattleBlueAngelWhisperSpriteDrawReq1 = {{0xf0, 0xff, 0xf0, 0xff}, 0, {0x40, 0x1f, 0x1f, 0x80, 0x80, 0x80, 0x2e, 0x3a, 0}, 0x78c7, 0};
#undef g_BattleBlueAngelWhisperSpriteDrawReq2
// GLOBAL: FF7 0x008a8bc0
struct BattleBlueAngelWhisperSpriteDrawReq2 g_BattleBlueAngelWhisperSpriteDrawReq2 = {{0xf0, 0xff, 0xf0, 0xff}, 0, {0x40, 0x1f, 0x1f, 0x80, 0x80, 0x80, 0x2e, 0x3a, 0}, 0x78c7, 0, {0}, 0, 0};
#undef g_BattleBlueAngelWhisperColor1
// GLOBAL: FF7 0x008a8be0
struct BattleBlueAngelWhisperColor1 g_BattleBlueAngelWhisperColor1 = {-128, -128, -128, {0x2e}};
unsigned char g_BattleFxBlueAngelWhisperEffectResourceNames[36] =
    "hikari.TIM\0\0"
    "hikari.SPT\0\0"
    "light_1.RSD";
// clang-format on
#pragma intrinsic(memset)
#pragma intrinsic(memcpy)


// clang-format off
// GLOBAL: FF7 0x00b74f30
int g_BattleBlueAngelWhisperStepCount = 0;
// GLOBAL: FF7 0x00b74f34
int g_BattleBlueAngelWhisperVariantMode = 0;
// GLOBAL: FF7 0x00b74f38
int g_BattleBlueAngelWhisperTargetMask = 0;
// GLOBAL: FF7 0x00b74f3c
int g_BattleBlueAngelWhisperOriginActor = 0;
// Per-actor tint saved while the fade runs.
// GLOBAL: FF7 0x00b74f40
BattleBlueAleizSavedTint g_BattleBlueAngelWhisperSavedTints[10] = {0};
// GLOBAL: FF7 0x00b74f90
struct GfxTableCursor *g_BattleBlueAngelWhisperLoadedRes = 0;
unsigned char g_bssPad_00b74f94[4] = {0};
// clang-format on

/*
 * Loads resources for the Aleiz blue magic effect when not already loaded.
 */
// FUNCTION: FF7 0x00564390
void __cdecl BattleFx_BlueLoadResAngelWhisper(void)
{
    GfxModelTableBank *workSceneCtx;
    GfxResourceRequest recordBuf;
    int workResourceId;
    char pathBlock[200];
    GfxResourceRequest *pRequest;
    int resBlueprint;

    if (g_pBattle3dScene != 0 && g_BattleBlueAngelWhisperLoadedRes == 0) {
        workResourceId = 2;
        resBlueprint = (int)&g_BattleBlueAngelWhisperResourceDef;
        workSceneCtx = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRequest = &recordBuf;
        Gfx_InitResourceRequest(pRequest);
        pRequest->groupTag = 0;
        // STRING: FF7 0x008a8c0c
        pRequest->resourcePath = BattleFx_BuildResourcePath("magic/aleiz", pathBlock);
        Gfx_LoadModelTableIfAbsent(workResourceId, resBlueprint, pRequest, &g_BattleBlueAngelWhisperLoadedRes, workSceneCtx);
    }
}

/*
 * Entry point for Aleiz variation B.
 */
// FUNCTION: FF7 0x00564420
int __cdecl BattleFx_BlueEnterAngelWhisperB(int targetMask, int actorIdx)
{
    BattleFx_BlueLoadResAngelWhisper();
    g_BattleBlueAngelWhisperVariantMode = 0;
    BattleFx_BlueBeginAngelWhisperQuadSprites(targetMask, actorIdx);
    return 0;
}

// Loads Aleiz resources, sets g_BattleBlueAngelWhisperVariantMode to 0, calls BattleFx_BlueBeginAngelWhisperActorFade with both arguments, and
// returns zero.
// FUNCTION: FF7 0x00564446
int __cdecl BattleFx_BlueEnterAngelWhisperC(int targetMask, int actorIdx)
{
    BattleFx_BlueLoadResAngelWhisper();
    g_BattleBlueAngelWhisperVariantMode = 0;
    BattleFx_BlueBeginAngelWhisperActorFade(targetMask, actorIdx);
    return 0;
}

// Loads Aleiz resources, sets g_BattleBlueAngelWhisperVariantMode to 1, calls BattleFx_BlueBeginAngelWhisperActorFade with both arguments, and
// returns zero.
// FUNCTION: FF7 0x0056446c
int __cdecl BattleFx_BlueEnterAngelWhisper(int targetMask, int actorIdx)
{
    BattleFx_BlueLoadResAngelWhisper();
    g_BattleBlueAngelWhisperVariantMode = 1;
    BattleFx_BlueBeginAngelWhisperActorFade(targetMask, actorIdx);
    return 0;
}

// Loads Aleiz resources, sets g_BattleBlueAngelWhisperVariantMode to 1, calls BattleFx_BlueBeginAngelWhisperQuadSprites with both arguments, and
// returns zero.
// FUNCTION: FF7 0x00564492
int __cdecl BattleFx_BlueEnterAngelWhisperD(int targetMask, int actorIdx)
{
    BattleFx_BlueLoadResAngelWhisper();
    g_BattleBlueAngelWhisperVariantMode = 1;
    BattleFx_BlueBeginAngelWhisperQuadSprites(targetMask, actorIdx);
    return 0;
}

/*
 * Initializes and begins the primary Aleiz effect sequence.
 */
// FUNCTION: FF7 0x005644b8
void __cdecl BattleFx_BlueBeginAngelWhisperQuadSprites(int targetMask, int actorIdx)
{
    g_BattleFxDrawListBufferTaskSlotPtr = (unsigned char *)BATTLE_NEW_TASK_SLOT(BattleFx_SelectDrawListBuffer);
    Battle_AllocateResourceSlot(BattleFx_BlueAngelWhisperQuadSpritesSetupTask);
    g_BattleBlueAngelWhisperTargetMask = targetMask;
    g_BattleBlueAngelWhisperOriginActor = actorIdx;
    Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputePanForEntity(targetMask), 0x17E);
}

/*
 * Setup task for the primary Aleiz sequence.
 */
// FUNCTION: FF7 0x00564515
void __cdecl BattleFx_BlueAngelWhisperQuadSpritesSetupTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[32];
        BattleFxAngelWhisperSetupSpawnSlot *spawnSlot;
        struct { unsigned char pad_to_4[0x4]; BattleAngelWhisperTaskSlot *aleizSlot; };
        struct { unsigned char pad_to_8[0x8]; int actorIdx; };
        struct { unsigned char pad_to_C[0xC]; int targetCount; };
        struct { unsigned char pad_to_10[0x10]; int bitIndex; };
        struct { unsigned char pad_to_14[0x14]; int spriteSize; };
        struct { unsigned char pad_to_18[0x18]; int altBonePos; };
        struct { unsigned char pad_to_1A[0x1A]; short altBoneY; };
    } frame;
#pragma pack(pop)

    frame.bitIndex = 0;
    for (frame.targetCount = 0; frame.bitIndex < 0xA; frame.bitIndex++) {
        if ((g_BattleBlueAngelWhisperTargetMask & 1 << frame.bitIndex) != 0) {
            frame.targetCount++;
            if (frame.targetCount == 1) {
                frame.actorIdx = frame.bitIndex;
            }
        }
    }
    frame.spawnSlot = (BattleFxAngelWhisperSetupSpawnSlot *)BATTLE_CURRENT_TASK_SLOT;
    frame.spawnSlot->ownerSlot = BATTLE_TASK_FREE;
    frame.spawnSlot = (BattleFxAngelWhisperSetupSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_BlueAngelWhisperQuadSpritesTimelineTask);
    if (frame.targetCount == 1) {
        Battle_GetBoneWorldPos(frame.actorIdx, g_BattleActorRecords[frame.actorIdx].targetBone, (short *)&frame.spawnSlot->posX);
        Battle_GetBoneWorldPos(frame.actorIdx, g_BattleActorRecords[frame.actorIdx].altBone, (short *)&frame.altBonePos);
        frame.spawnSlot->posY = frame.altBoneY - 448;
        frame.spriteSize = g_BattleActorRecords[frame.actorIdx].radius + 224;
        if (frame.spriteSize > 950) {
            frame.spriteSize = 950;
        }
        frame.aleizSlot = (BattleAngelWhisperTaskSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_BlueAngelWhisperSpawnDescendingQuadSpritesTask);
        frame.aleizSlot->pos = *(Int64Pair *)&frame.spawnSlot->posX;
        frame.aleizSlot->spreadRadius = *(short *)&frame.spriteSize;
        frame.aleizSlot->spreadDiameter = frame.spriteSize << 1;
    } else {
        Battle_GetActorMaskCentroid(g_BattleBlueAngelWhisperTargetMask, (short *)&frame.spawnSlot->posX);
        frame.spawnSlot->posY = 0xF9E0;
        frame.spriteSize = 2688;
        frame.aleizSlot = (BattleAngelWhisperTaskSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_BlueAngelWhisperSpawnArcingQuadSpritesTask);
        frame.aleizSlot->pos = *(Int64Pair *)&frame.spawnSlot->posX;
        frame.aleizSlot->spreadRadius = *(short *)&frame.spriteSize;
    }
    frame.spawnSlot->posY -= 672;
    frame.spawnSlot->yaw = g_BattleActorRecords[g_BattleBlueAngelWhisperOriginActor].yaw + 0x800 & 0xFFF;
    frame.spawnSlot->heightStep = 42;
}

/*
 * Timeline task controlling progression of the primary Aleiz sequence.
 */
// FUNCTION: FF7 0x0056470f
void __cdecl BattleFx_BlueAngelWhisperQuadSpritesTimelineTask(void)
{
    BattleFxAngelWhisperRingRecord *ringRecord;
    int actorIndex;
    BattleFxBlueAngelWhisperTimeline *task;

    task = (BattleFxBlueAngelWhisperTimeline *)BATTLE_CURRENT_TASK_SLOT;
    if (task->step == 1) {
        ringRecord = (BattleFxAngelWhisperRingRecord *)Battle_AllocSceneMessage(1);
        ringRecord->pos = task->pos;
        ringRecord->yaw = task->param;
    } else if (task->step == 32) {
        Battle_AllocSceneMessage(2);
    }
    if (task->step > 1 && task->step < 17) {
        g_BattleActorRecords[3].posY += task->counterDelta;
    }
    if (task->step > 32 && task->step < 48) {
        g_BattleActorRecords[3].posY -= task->counterDelta;
    }
    if (task->step == 40) {
        for (actorIndex = 0; actorIndex < 10; actorIndex++) {
            if ((g_BattleBlueAngelWhisperTargetMask & 1 << actorIndex) != 0) {
                Battle_PostActorHitOrHurtMessage(actorIndex);
            }
        }
    }
    if (task->step >= 45) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    (task->step)++;
}

// Without a pause check, at counters eighteen through twenty-one emits sixteen BattleFx_BlueAngelWhisperArcingQuadSpriteTask slots
// copying eight position bytes. Normalizes a random XZ direction through adjacent int locals,
// chooses the radius from the spawner plus zero through 447, stores X/Z displacement divided by sixteen, Y
// velocity minus 250 through 309, scale 8874 times random 0xA00 through 0x15FF shifted twelve,
// and acceleration minus Y divided by twenty-four. Retires at forty-five or above and always
// advances.
// FUNCTION: FF7 0x00564844
void __cdecl BattleFx_BlueAngelWhisperSpawnArcingQuadSpritesTask(void)
{
    VECTOR nx;
    int emitIdx;
    int baseScale;
    int radius;
    BattleFxAngelWhisperArcSprite *spawner;
    BattleFxAngelWhisperArcingQuad *arcQuad;

    spawner = (BattleFxAngelWhisperArcSprite *)BATTLE_CURRENT_TASK_SLOT;
    if (spawner->frame >= 18 && spawner->frame <= 21) {
        baseScale = 0x22AA;
        for (emitIdx = 0; emitIdx < 16; emitIdx++) {
            arcQuad = (BattleFxAngelWhisperArcingQuad *)BATTLE_NEW_TASK_SLOT(BattleFx_BlueAngelWhisperArcingQuadSpriteTask);
            *(Int64Pair *)&arcQuad->originX = spawner->pos;
            nx.vx = rand() % 4096 - 2048;
            nx.vy = 0;
            nx.vz = rand() % 4096 - 2048;
            NormalizeVectorFixed(&nx, &nx);
            radius = spawner->radius + rand() % 448;
            arcQuad->targetX = arcQuad->originX + (radius * nx.vx >> 12);
            arcQuad->velocity = 0;
            arcQuad->targetY = arcQuad->originY + (radius * nx.vz >> 12);
            arcQuad->targetX = (arcQuad->targetX - arcQuad->originX) / 16;
            arcQuad->velocity = -(rand() % 60 + 250);
            arcQuad->targetY = (arcQuad->targetY - arcQuad->originY) / 16;
            radius = rand() % 3072 + 2560;
            arcQuad->scale = baseScale * radius >> 12;
            arcQuad->arcStep = -arcQuad->arcHeight / 24;
        }
    }
    if (spawner->frame >= 45) {
        spawner->ownerSlot = BATTLE_TASK_FREE;
    }
    (spawner->frame)++;
}

// Per-frame tick for one arcing quad: sets the sprite request's draw attribute word to 0x7947 when
// g_BattleBlueAngelWhisperVariantMode is set, steps its row byte by 32 every second frame, puts the scene's
// first render object into the request, billboards the quad at its position with angle as the scale
// and forward offset -(angle >> 4), and submits the quad into the OT bucket at BATTLE_OT_DEPTH_SORTED
// with shift 0xC on the shared draw-list cursor. The record is killed after 14 frames or once posB goes positive; unless paused
// the position advances by the velocity, the Y velocity gains the acceleration, and the acceleration
// decays by its own value shifted right 4.
// FUNCTION: FF7 0x00564a3b
void __cdecl BattleFx_BlueAngelWhisperArcingQuadSpriteTask(void)
{
    BattleFxArcingQuadSprite *task;

    if (g_BattleBlueAngelWhisperVariantMode != 0) {
        g_BattleBlueAngelWhisperSpriteDrawReq1.arcDrawAttrWord = 0x7947;
    }
    task = (BattleFxArcingQuadSprite *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleBlueAngelWhisperSpriteDrawReq1.arcSpriteRowByte = task->frame >> 1 << 5;
    g_BattleBlueAngelWhisperSpriteDrawReq1.arcDrawObject = Battle3d_GetSceneRenderObject(1, g_pBattle3dScene);
    BattleFx_InstallBillboardMatrix(&task->posA, (short)task->angle, -(task->angle >> 4));
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitSpriteQuadDraw((BattleFxSpriteQuadRequest *)&g_BattleBlueAngelWhisperSpriteDrawReq1, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (task->frame >= 14) {
        task->endFlag = 0xFFFF;
    }
    if (task->posB > 0) {
        task->endFlag = 0xFFFF;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    task->posA += task->velA;
    task->posB += task->velB;
    task->posC += task->velC;
    task->velB += task->accelB;
    task->accelB -= (short)(task->accelB >> 4);
    (task->frame)++;
}

// Spawns descending quad sprites from a blue Aleiz spawner while its step is from 0x12 to 0x1C.
// Each such step allocates two quad tasks scattered around baseX, baseY and baseZ, using
// spreadRange and spreadOffset, with posY set near baseY minus 0x12C. Each task gets its scale from
// 8874 times a random factor, and a spread from spreadOffset divided by 6 plus a random amount.
// From step 0x2D on the spawner is killed (ownerSlot 0xFFFF), and step increments on every call.
// FUNCTION: FF7 0x00564b8f
void __cdecl BattleFx_BlueAngelWhisperSpawnDescendingQuadSpritesTask(void)
{
    int idx, size, scaleRand;
    BattleFxDescendingQuadSprite *sprite;
    BattleFxBlueAngelWhisperSpawner *task;

    task = (BattleFxBlueAngelWhisperSpawner *)BATTLE_CURRENT_TASK_SLOT;
    if (task->step >= 18 && task->step <= 28) {
        size = 8874;
        for (idx = 0; idx < 2; idx++) {
            sprite = (BattleFxDescendingQuadSprite *)BATTLE_NEW_TASK_SLOT(BattleFx_BlueAngelWhisperDescendingQuadSpriteTask);
            scaleRand = rand() % 0xC00 + 0xA00;
            sprite->posX = task->baseX + rand() % task->spreadRange - task->spreadOffset;
            sprite->posY = task->baseY + rand() % 100 + -300;
            sprite->posZ = task->baseZ + rand() % task->spreadRange - task->spreadOffset;
            sprite->scale = size * scaleRand >> 12;
            sprite->spread = task->spreadOffset / 6 + rand() % 100;
        }
    }
    if (task->step >= 45) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    (task->step)++;
}

// Draws one frame of a blue Aleiz descending quad and advances it. When variant mode is set, the
// draw attribute word is forced to 0x7947. The sprite row is (frameCount & 7) << 5, so it cycles
// through eight steps of 32, and the scene's first render object becomes the draw object. The
// record is billboarded from its 4.12 scale, with a forward offset of minus scale shifted right 4.
// The quad is submitted into the draw list bucket at +0x70 with shift 0xC on the shared cursor. The
// record is killed after 10 frames. Unless paused, posY advances by velocityY, velocityY decays by
// its own value shifted right 3, and frameCount increments.
// FUNCTION: FF7 0x00564cf6
void __cdecl BattleFx_BlueAngelWhisperDescendingQuadSpriteTask(void)
{
    BattleFxAngelWhisperQuadTask *task;

    if (g_BattleBlueAngelWhisperVariantMode != 0) {
        g_BattleBlueAngelWhisperSpriteDrawReq2.descentDrawAttrWord = 0x7947;
    }
    task = (BattleFxAngelWhisperQuadTask *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleBlueAngelWhisperSpriteDrawReq2.descentSpriteRowByte = (task->frameCount & 7) << 5;
    g_BattleBlueAngelWhisperSpriteDrawReq2.descentDrawObject = Battle3d_GetSceneRenderObject(1, g_pBattle3dScene);
    BattleFx_InstallBillboardMatrix(&task->posX, (short)task->scale, -(task->scale >> 4));
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitSpriteQuadDraw((BattleFxSpriteQuadRequest *)&g_BattleBlueAngelWhisperSpriteDrawReq2, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (task->frameCount >= 10) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    task->posY += task->velocityY;
    task->velocityY -= (short)(task->velocityY >> 3);
    (task->frameCount)++;
}

/*
 * Initializes and begins the secondary Aleiz effect sequence.
 */
// FUNCTION: FF7 0x00564df6
void __cdecl BattleFx_BlueBeginAngelWhisperActorFade(int targetMask, int actorIdx)
{
    g_BattleFxDrawListBufferTaskSlotPtr = (unsigned char *)BATTLE_NEW_TASK_SLOT(BattleFx_SelectDrawListBuffer);
    Battle_AllocateResourceSlot(BattleFx_BlueAngelWhisperActorFadeSetupTask);
    g_BattleBlueAngelWhisperTargetMask = targetMask;
    g_BattleBlueAngelWhisperOriginActor = actorIdx;
}

/*
 * Setup task for the secondary Aleiz sequence.
 */
// FUNCTION: FF7 0x00564e37
void __cdecl BattleFx_BlueAngelWhisperActorFadeSetupTask(void)
{
    int p; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int actorIdx;
    int r;
    int s; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxBlueAngelWhisperTaskSlot *task;

    actorIdx = 0;
    for (r = 0; actorIdx < 10; actorIdx++) {
        if ((g_BattleBlueAngelWhisperTargetMask & 1 << actorIdx) != 0) {
            r++;
            if (r == 1) {
                g_BattleBlueAngelWhisperStepCount = actorIdx;
            }
        }
    }
    task = (BattleFxBlueAngelWhisperTaskSlot *)BATTLE_CURRENT_TASK_SLOT;
    task->ownerSlot = BATTLE_TASK_FREE;
    if (r == 1) {
        if (g_BattleBlueAngelWhisperVariantMode != 0) {
            task = (BattleFxBlueAngelWhisperTaskSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_BlueAngelWhisperActorFadeVariantTimelineTask);
            Battle_GetBoneWorldPos(g_BattleBlueAngelWhisperStepCount, g_BattleActorRecords[g_BattleBlueAngelWhisperStepCount].targetBone, (short *)((int)task + 4));
            task->posY = g_BattleActorRecords[g_BattleBlueAngelWhisperStepCount].posY;
            task->posY -= 2560;
            Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputePanForEntity(g_BattleBlueAngelWhisperTargetMask), 0x1A7);
        } else {
            task = (BattleFxBlueAngelWhisperTaskSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_BlueAngelWhisperActorFadeTimelineTask);
            Battle_GetBoneWorldPos(g_BattleBlueAngelWhisperStepCount, g_BattleActorRecords[g_BattleBlueAngelWhisperStepCount].targetBone, (short *)((int)task + 4));
            task->posY = g_BattleActorRecords[g_BattleBlueAngelWhisperStepCount].posY;
            Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputePanForEntity(g_BattleBlueAngelWhisperTargetMask), 0x1A7);
        }
        task->yaw = g_BattleActorRecords[g_BattleBlueAngelWhisperStepCount].yaw;
        task->duration = 700;
        task->isActive = 0;
    } else {
        task = (BattleFxBlueAngelWhisperTaskSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_BlueAngelWhisperActorFadeTimelineTask);
        Battle_GetActorMaskCentroid(g_BattleBlueAngelWhisperTargetMask, (short *)((int)task + 4));
        task->posY = g_BattleActorRecords[g_BattleBlueAngelWhisperOriginActor].posY;
        task->yaw = g_BattleActorRecords[g_BattleBlueAngelWhisperOriginActor].yaw;
        task->duration = 1400;
        task->isActive = 1;
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputePanForEntity(g_BattleBlueAngelWhisperTargetMask), 0x1A7);
    }
    task = (BattleFxBlueAngelWhisperTaskSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_BlueAngelWhisperActorFadeTask);
}

// Fades the battle actors for a blue Aleiz effect over the task's frameCount, and does nothing
// while paused. On frame 0 it saves each actor's tintPhase, tintPulse and RGB tint into the save
// arrays, sets bit 0 of drawFlags, and clears the tint. Each later frame sets tintPhase per actor
// by frame range. Before frame 10 every actor ramps up. From frame 0x43 actors outside
// g_BattleBlueAngelWhisperTargetMask ramp down. Over frames 0x26 to 0x38 actors inside the mask ramp down.
// The timeline's startValue takes the same ramp, and channel2, channel1 and red are cleared on
// frames below 10.
// FUNCTION: FF7 0x005650b3
void __cdecl BattleFx_BlueAngelWhisperActorFadeTask(void)
{
    unsigned short p;
    int actorIdx;
    int localTintValue;
    int s; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleTaskSlot *task;

    if (g_BattleIsPaused != 0) {
        return;
    }
    task = BATTLE_CURRENT_TASK_SLOT;
    if (task->frameCounter == 0) {
        for (actorIdx = 0; actorIdx < 10; actorIdx++) {
            g_BattleBlueAngelWhisperSavedTints[actorIdx].tintPhase = g_BattleActorRecords[actorIdx].tintPhase;
            g_BattleBlueAngelWhisperSavedTints[actorIdx].tintPulse = g_BattleActorRecords[actorIdx].tintPulse;
            g_BattleBlueAngelWhisperSavedTints[actorIdx].tintR = g_BattleActorRecords[actorIdx].tintR;
            g_BattleBlueAngelWhisperSavedTints[actorIdx].tintG = g_BattleActorRecords[actorIdx].tintG;
            g_BattleBlueAngelWhisperSavedTints[actorIdx].tintB = g_BattleActorRecords[actorIdx].tintB;
            g_BattleActorRecords[actorIdx].drawFlags |= 1;
            g_BattleActorRecords[actorIdx].tintPhase = 0;
            g_BattleActorRecords[actorIdx].tintB = 0;
            g_BattleActorRecords[actorIdx].tintG = 0;
            g_BattleActorRecords[actorIdx].tintR = 0;
        }
    }
    if (task->frameCounter < 10) {
        localTintValue = task->frameCounter * 409;
        for (actorIdx = 0; actorIdx < 10; actorIdx++) {
            g_BattleActorRecords[actorIdx].tintPhase = localTintValue;
        }
    } else if (task->frameCounter >= 67) {
        localTintValue = 0x1000 - (task->frameCounter - 0x43 << 9);
        for (actorIdx = 0; actorIdx < 10; actorIdx++) {
            if ((g_BattleBlueAngelWhisperTargetMask & 1 << actorIdx) != 0) {
                continue;
            }
            g_BattleActorRecords[actorIdx].tintPhase = localTintValue;
        }
    }
    if (task->frameCounter >= 38 && task->frameCounter <= 56) {
        localTintValue = 0x1000 - (task->frameCounter - 0x26) * 227;
        for (actorIdx = 0; actorIdx < 10; actorIdx++) {
            if ((g_BattleBlueAngelWhisperTargetMask & 1 << actorIdx) != 0) {
                g_BattleActorRecords[actorIdx].tintPhase = localTintValue;
            }
        }
    }
    if (task->frameCounter < 10) {
        g_BattleFxTimelineState.channel2 = 0;
        g_BattleFxTimelineState.channel1 = g_BattleFxTimelineState.channel2;
        g_BattleFxTimelineState.red = g_BattleFxTimelineState.channel1;
        p = task->frameCounter * 409;
    } else if (task->frameCounter >= 67) {
        p = 0x1000 - (task->frameCounter - 0x43 << 9);
    } else {
        p = 0x1000;
    }
    g_BattleFxTimelineState.startValue = p;
    if (task->frameCounter >= 75) {
        for (actorIdx = 0; actorIdx < 10; actorIdx++) {
            g_BattleActorRecords[actorIdx].drawFlags &= 0xFE;
            g_BattleActorRecords[actorIdx].tintPhase = g_BattleBlueAngelWhisperSavedTints[actorIdx].tintPhase;
            g_BattleActorRecords[actorIdx].tintPulse = *(char *)&g_BattleBlueAngelWhisperSavedTints[actorIdx].tintPulse;
            g_BattleActorRecords[actorIdx].tintR = g_BattleBlueAngelWhisperSavedTints[actorIdx].tintR;
            g_BattleActorRecords[actorIdx].tintG = g_BattleBlueAngelWhisperSavedTints[actorIdx].tintG;
            g_BattleActorRecords[actorIdx].tintB = g_BattleBlueAngelWhisperSavedTints[actorIdx].tintB;
        }
        g_BattleFxTimelineState.startValue = 0;
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    (task->frameCounter)++;
}

/*
 * Timeline task controlling progression of the secondary Aleiz sequence.
 */
// FUNCTION: FF7 0x00565484
void __cdecl BattleFx_BlueAngelWhisperActorFadeTimelineTask(void)
{
    int numSprites, i8, halfRange, doubleRange, bytesRand;
    BattleFxBlueAngelWhisperRecordA *task;
    BattleFxAngelWhisperRecord *pRecord;

    if (g_BattleIsPaused != 0) {
        return;
    }
    task = (BattleFxBlueAngelWhisperRecordA *)BATTLE_CURRENT_TASK_SLOT;
    if (task->step >= 10 && task->step <= 32) {
        i8 = task->step % 3;
        if (i8 == 0) {
            i8 = (task->elapsed << 12) / 700;
            pRecord = (BattleFxAngelWhisperRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_BlueAngelWhisperExpandingFadeModelTask);
            *(Int64Pair *)&pRecord->posX = task->pos;
            pRecord->baseScale = 0x800;
            pRecord->scale = i8 >> 1;
            pRecord->scaleStep = i8 / 8;
            pRecord->velocity = i8 / 3 << 1;
            pRecord->accel = pRecord->velocity / 8;
        }
    }
    if (task->step >= 24 && task->step <= 38) {
        halfRange = task->elapsed;
        doubleRange = halfRange << 1;
        numSprites = task->isMultiTarget + 4;
        for (i8 = 0; i8 < numSprites; i8++) {
            pRecord = (BattleFxAngelWhisperRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_BlueAngelWhisperRisingFadeSpriteTask);
            *(Int64Pair *)&pRecord->posX = task->pos;
            pRecord->posX += (short)(rand() % doubleRange - halfRange);
            pRecord->posY += (short)-(rand() % 233);
            pRecord->posZ += (short)(rand() % doubleRange - halfRange);
            pRecord->scale = 0x555;
            bytesRand = rand() % 3072 + 2048;
            pRecord->scale = pRecord->scale * bytesRand >> 12;
            pRecord->scaleStep = pRecord->scale >> 4;
            pRecord->velocity = -(rand() % 120 + 100);
        }
    }
    if (task->step == 28) {
        pRecord = (BattleFxAngelWhisperRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_BlueAngelWhisperRingRecordTimelineTask);
        *(Int64Pair *)&pRecord->posX = task->pos;
        pRecord->baseScale = task->baseScale;
        pRecord->accel = 0xFFB0 /* -80 */;
    }
    if (task->step == 60) {
        for (i8 = 0; i8 < 10; i8++) {
            if ((g_BattleBlueAngelWhisperTargetMask & 1 << i8) != 0) {
                Battle_PostActorHitOrHurtMessage(i8);
            }
        }
    }
    if (task->step >= 75) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    (task->step)++;
}

// Drives the timeline of a blue Aleiz ring effect, skipped entirely while paused. At step 1 it
// allocates a type-1 ring buffer record and copies the eight bytes at +4 and yaw into it at +4 and
// +0x10. At step 0x1A it allocates a type-2 record and discards it. Between steps 2 and 0xF it adds
// half of velocityY to actor 3's posY, and from step 0x10 on it adds the whole value. At step 0x2A
// or later the record is killed (ownerSlot 0xFFFF), and step increments. An unreferenced padding
// slot preserves the retail VC5 /Od stack frame size.
// FUNCTION: FF7 0x00565789
void __cdecl BattleFx_BlueAngelWhisperRingRecordTimelineTask(void)
{
    BattleFxAngelWhisperRingRecord *innerRecord;
    int q; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxBlueAngelWhisperRingTaskSlot *task;

    if (g_BattleIsPaused != 0) {
        return;
    }
    task = (BattleFxBlueAngelWhisperRingTaskSlot *)BATTLE_CURRENT_TASK_SLOT;
    if (task->step == 1) {
        innerRecord = (BattleFxAngelWhisperRingRecord *)Battle_AllocSceneMessage(1);
        innerRecord->pos = task->pos;
        innerRecord->yaw = task->yaw;
    } else if (task->step == 26) {
        Battle_AllocSceneMessage(2);
    }
    if (task->step > 1 && task->step < 16) {
        g_BattleActorRecords[3].posY += (short)(task->velocityY >> 1);
    } else if (task->step >= 16) {
        g_BattleActorRecords[3].posY += task->velocityY;
    }
    if (task->step >= 42) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    (task->step)++;
}

// Draws one frame of a blue Aleiz rising fade sprite and advances it. The billboard uses riseSpeed
// as its 4.12 scale, with a forward offset of minus riseSpeed shifted right 3. The byte 0x80 minus
// timer times 6 is written to all three colour bytes, so the sprite greys out by 6 each frame. Slot
// 0 of the loaded resource goes into the request's fields, and the quad is submitted into the draw
// list bucket at +0x70 with shift 0xC on the shared cursor. The record is killed once timer reaches
// 16. Unless paused, height advances by riseAccel, riseSpeed is clamped to 0 if negative, riseAccel
// decays by its own value shifted right 4, and timer increments.
// FUNCTION: FF7 0x00565882
void __cdecl BattleFx_BlueAngelWhisperRisingFadeSpriteTask(void)
{
    unsigned char grayByte;
    BattleFxRisingFadeSprite *sprite;

    sprite = (BattleFxRisingFadeSprite *)BATTLE_CURRENT_TASK_SLOT;
    BattleFx_InstallBillboardMatrix(&sprite->posX, (short)sprite->riseSpeed, -(sprite->riseSpeed >> 3));
    grayByte = 0x80 - sprite->timer * 6;
    g_BattleBlueAngelWhisperColor1.aleizColor1Byte2 = grayByte;
    g_BattleBlueAngelWhisperColor1.channelByte = g_BattleBlueAngelWhisperColor1.aleizColor1Byte2;
    g_BattleBlueAngelWhisperColor1.aleizColor1Byte1 = g_BattleBlueAngelWhisperColor1.channelByte;
    g_BattleBlueAngelWhisperSpriteDrawReq2.risingFadeRecord = Gfx_ModelTableGetLoadedRecord(0, g_BattleBlueAngelWhisperLoadedRes);
    g_BattleBlueAngelWhisperSpriteDrawReq2.risingFadeObject = Gfx_ModelTableGetSpriteObject(0, g_BattleBlueAngelWhisperLoadedRes);
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitSpriteDraw(&g_BattleBlueAngelWhisperSpriteDrawReq2.risingFadeRecord, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (sprite->timer >= 16) {
        sprite->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    sprite->height += sprite->riseAccel;
    if (sprite->riseSpeed < 0) {
        sprite->riseSpeed = 0;
    }
    sprite->riseAccel -= (short)(sprite->riseAccel >> 4);
    (sprite->timer)++;
}

// BattleFx_BlueAngelWhisperExpandingFadeModelTask -- 482 bytes
// FUNCTION: FF7 0x005659bd
void __cdecl BattleFx_BlueAngelWhisperExpandingFadeModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[64];
        PsxMatrix rotMatrix;
        struct { unsigned char pad_to_20[0x20]; BattleFxAngelWhisperExpandingFadeModelSlot *fadeSlot; };
        struct { unsigned char pad_to_24[0x24]; SVECTOR rotX; };
        struct { unsigned char pad_to_2C[0x2C]; BattleFxModelDraw *modelDraw; };
        struct { unsigned char pad_to_30[0x30]; VECTOR scale; };
    } frame;
#pragma pack(pop)

    frame.fadeSlot = (BattleFxAngelWhisperExpandingFadeModelSlot *)BATTLE_CURRENT_TASK_SLOT;
    frame.rotX.vx = frame.fadeSlot->rotX;
    frame.rotX.vy = 0;
    frame.rotX.vz = 0;
    RotMatrixXYZ(&frame.rotX, &frame.rotMatrix);
    frame.rotMatrix.t[0] = frame.fadeSlot->posX;
    frame.rotMatrix.t[1] = frame.fadeSlot->posY;
    frame.rotMatrix.t[2] = frame.fadeSlot->posZ;
    frame.scale.vz = frame.fadeSlot->scaleXZ;
    frame.scale.vx = frame.scale.vz;
    frame.scale.vy = frame.fadeSlot->scaleY;
    ScaleMatrix(&frame.rotMatrix, &frame.scale);
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.rotMatrix);
    SetRotMatrix(&frame.rotMatrix);
    SetTransMatrix(&frame.rotMatrix);
    frame.modelDraw = (BattleFxModelDraw *)Gfx_GetPsxScratchpadPtr(0);
    frame.modelDraw->modelGroup = Gfx_ModelTableGetRenderObject(1, g_BattleBlueAngelWhisperLoadedRes);
    frame.modelDraw->flags = 8;
    frame.modelDraw->uvScroll = 0;
    frame.modelDraw->stateFlags = 0x20;
    frame.modelDraw->texturePageBits = 0;
    if (frame.fadeSlot->frameCounter >= 2) {
        frame.modelDraw->fadeLevel = 128 - (frame.fadeSlot->frameCounter - 2 << 4);
    } else {
        frame.modelDraw->fadeLevel = 128;
    }
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitModelDraw((BattleFxModelDraw *)frame.modelDraw, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (frame.fadeSlot->frameCounter >= 10) {
        frame.fadeSlot->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.fadeSlot->scaleXZ += frame.fadeSlot->scaleXZStep;
    frame.fadeSlot->scaleY -= frame.fadeSlot->scaleYStep;
    if (frame.fadeSlot->scaleY < 0) {
        frame.fadeSlot->scaleY = 0;
    }
    frame.fadeSlot->scaleYStep -= (short)(frame.fadeSlot->scaleYStep >> 4);
    (frame.fadeSlot->frameCounter)++;
}

/*
 * Sub-timeline task coordinating final stage effects for Aleiz.
 */
// FUNCTION: FF7 0x00565b9f
void __cdecl BattleFx_BlueAngelWhisperActorFadeVariantTimelineTask(void)
{
    int numSprites, i8, halfRange, doubleRange, bytesRand;
    BattleFxBlueAngelWhisperRecordB *task;
    BattleFxBlueAngelWhisperRecord *pRecord;

    if (g_BattleIsPaused != 0) {
        return;
    }
    task = (BattleFxBlueAngelWhisperRecordB *)BATTLE_CURRENT_TASK_SLOT;
    if (task->step >= 10 && task->step <= 32) {
        i8 = task->step % 3;
        if (i8 == 0) {
            i8 = (task->elapsed << 12) / 700;
            pRecord = (BattleFxBlueAngelWhisperRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_BlueAngelWhisperExpandingFadeModelTask);
            *(Int64Pair *)&pRecord->jitterA = task->pos;
            pRecord->startValue = 0;
            pRecord->amplitude = i8 >> 1;
            pRecord->spread = i8 / 8;
            pRecord->duration = i8 / 3 << 1;
            pRecord->delay = pRecord->duration / 8;
        }
    }
    if (task->step >= 24 && task->step <= 38) {
        halfRange = task->elapsed;
        doubleRange = halfRange << 1;
        numSprites = task->isMultiTarget + 4;
        for (i8 = 0; i8 < numSprites; i8++) {
            pRecord = (BattleFxBlueAngelWhisperRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_BlueAngelWhisperRisingFadeSpriteTask);
            *(Int64Pair *)&pRecord->jitterA = task->pos;
            pRecord->jitterA += (short)(rand() % doubleRange - halfRange);
            pRecord->velocity += (short)(rand() % 233 + 100);
            pRecord->jitterB += (short)(rand() % doubleRange - halfRange);
            pRecord->amplitude = 0x555;
            bytesRand = rand() % 3072 + 2048;
            pRecord->amplitude = pRecord->amplitude * bytesRand >> 12;
            pRecord->spread = pRecord->amplitude >> 4;
            pRecord->duration = rand() % 100 + 90;
        }
    }
    if (task->step == 28) {
        pRecord = (BattleFxBlueAngelWhisperRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_BlueAngelWhisperRingRecordTimelineTask);
        *(Int64Pair *)&pRecord->jitterA = task->pos;
        pRecord->velocity += 512;
        pRecord->startValue = task->startValue;
        pRecord->delay = 60;
    }
    if (task->step == 60) {
        for (i8 = 0; i8 < 10; i8++) {
            if ((g_BattleBlueAngelWhisperTargetMask & 1 << i8) != 0) {
                Battle_PostActorHitOrHurtMessage(i8);
            }
        }
    }
    if (task->step >= 75) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    (task->step)++;
}
} /* extern "C" */
