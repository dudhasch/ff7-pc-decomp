// src/ff7/src/battle/battle_fx_magic_full_cure.cpp
/* Magic Effect [50]: Full Cure (Japanese: フルケア, dev tag: "MAGIC/FULL" / Full) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Full Cure magic: loads its models and sprites, then spawns per-target light beams and
//   rising sparkle particles through a callback table, and collapses the light mesh at the end.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/enums/sound.h"
#include "ff7/types/battle.h"
#include "lib/types/graphics.h"
#include "lib/types/polygon.h"
#include "ff7/camera_script.h"
#include <stdlib.h>
#include <string.h>
#include "ff7/api/battle.h"
#include "ff7/api/menu.h"
#include "lib/api/sound.h"
#include "lib/api/graphics.h"
#include "ff7/battle_tasks.h"
#include "ff7/types/battle_magic.h"
#include "lib/codegen.h"
extern "C" {

#pragma intrinsic(memset)
// clang-format off
// GLOBAL: FF7 0x0082ef00
int g_BattleMagicFullCureMagicId = 0x00C840B1;
char *g_BattleFxMagicFullCureCureResourceNames[1] = {
    // STRING: FF7 0x0082efc0
    "SONIC.RSD"
};
unsigned int g_BattleMagicFullCureMagicIdDataTail[14] = {
    0x00000000,
    0x00000000,
    0x00000000,
    0x00c800b1,
    // STRING: FF7 0x0082efcc
    (unsigned int)"FULL_1.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x0082efd8
    (unsigned int)"LIGHT.TIM",
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000
};
// clang-format on
/* The first 12 bytes of the growing light mesh's model draw, which BattleFx_SubmitModelDraw reads
 * as a BattleFxModelDraw; its stateFlags word (0x20) is the next object. */
typedef struct {
    int modelGroup;
    int flags;
    short uvScroll;
    short fadeLevel;
} BattleMagicFullCureModelDef1;

// clang-format off
// GLOBAL: FF7 0x0082ef40
BattleMagicFullCureModelDef1 g_BattleMagicFullCureModelDef1 = {0, 0xa8, 0, 0};
unsigned char g_rawData_0082ef4c[4] = {0x20, 0x00, 0x00, 0x00};
// clang-format on

// clang-format off
// GLOBAL: FF7 0x0082ef50
unsigned char g_BattleMagicFullCureSpriteDef1[8] = {0xe0, 0xff, 0xe0, 0xff, 0x00, 0x00, 0x3f, 0x3f};
// clang-format on

/* Bytes 8..0x17 of the rising sparkle's sprite quad request, which starts at
 * g_BattleMagicFullCureSpriteDef1: the colour, the flags and cell word, and the sprite draw object. */
typedef struct {
    char colorR;
    char colorG;
    char colorB;
    unsigned char pad_03;
    short flags;
    short cellWord;
    int drawObj;
    unsigned char pad_0C[0x4];
} BattleMagicFullCureSpriteRequest1;

// clang-format off
// GLOBAL: FF7 0x0082ef58
BattleMagicFullCureSpriteRequest1 g_BattleMagicFullCureSpriteRequest1 = {(char)0x80, (char)0x80, (char)0x80, 0x2e, 0x2e, 0x7888, 0, {0}};
// clang-format off
// GLOBAL: FF7 0x0082ef68
unsigned char g_BattleMagicFullCureSpriteDef2[8] = {0xe0, 0xff, 0xe0, 0xff, 0x00, 0x00, 0x3f, 0x3f};
// clang-format on

/* The colour bytes of the fading sparkle's sprite quad request, which starts at
 * g_BattleMagicFullCureSpriteDef2. */
typedef struct {
    char colorR;
    char colorG;
    char colorB;
    unsigned char pad_03;
} BattleMagicFullCureSpriteRequest2;

// clang-format off
// GLOBAL: FF7 0x0082ef70
BattleMagicFullCureSpriteRequest2 g_BattleMagicFullCureSpriteRequest2 = {(char)0x80, (char)0x80, (char)0x80, 0x2e};
unsigned char g_rawData_0082ef74[4] = {0x2e, 0x00, 0x88, 0x78};
// clang-format on

/* The end of the fading sparkle's sprite quad request (its sprite draw object), then the shrinking
 * light mesh's model draw head and its transform. */
typedef struct {
    int spriteDrawObj;
    unsigned char pad_04[0x4];
    BattleFxModelDrawHead model;
    PsxMatrix transform;
    unsigned char pad_36[0x2];
} BattleMagicFullCureModelDef2;

// clang-format off
// GLOBAL: FF7 0x0082ef78
BattleMagicFullCureModelDef2 g_BattleMagicFullCureModelDef2 = {
    0, {0}, {0, 0x88, 0, 0, 0x20, 0}, {{{0, 0, 0}, {0, 0x800, 0}, {0, 0, 0}}, {0, 0, 0}}, {0}
};
unsigned char g_rawData_0082efb0[16] = {0x80, 0x00, 0x00, 0x00, 0xe0, 0x01, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00};
/* The ...Init bytes are once-only guards: bit 0 is set when the matching scratch pointer has been
 * fetched with Gfx_GetPsxScratchpadPtr(0). */
// GLOBAL: FF7 0x00b390d8
unsigned char g_BattleMagicFullCureGrowMatrixInit = 0;
unsigned char g_bssPad_00b390dc[4] = {0};
/* One update callback per pool sparkle; 0 marks a free slot. */
// GLOBAL: FF7 0x00b390e0
void (__cdecl *g_BattleMagicFullCureParticleCallbacks[200])(BattleFxMagicFullCureSparkle *) = {0};
// clang-format on

/* The block Gfx_GetPsxScratchpadPtr(0) returns to the fading sparkle: the point it spins and the rotation
 * that spins it. */
typedef struct {
    SVECTOR point;
    PsxMatrix rotation;
} BattleMagicFullCureSparkleScratch;

// clang-format off
// GLOBAL: FF7 0x00b39400
BattleMagicFullCureSparkleScratch *g_BattleMagicFullCureSparkleScratch = 0;
// GLOBAL: FF7 0x00b39404
PsxMatrix *g_BattleMagicFullCureGrowMatrix = 0;
// GLOBAL: FF7 0x00b39408
PsxMatrix *g_BattleMagicFullCureShrinkMatrix = 0;
// GLOBAL: FF7 0x00b3940c
int g_BattleMagicFullCureDrawHandle = 0;
// GLOBAL: FF7 0x00b39410
int g_BattleMagicFullCurePausedFrames = 0;
// GLOBAL: FF7 0x00b39414
unsigned char g_BattleMagicFullCureShrinkMatrixInit = 0;
unsigned char g_bssPad_00b39418[512] = {0};
// clang-format on

/* The head of the 200-sparkle pool (BattleFxMagicFullCureSparkle records); the records run on through
 * the filler that follows it. */
typedef struct {
    short state;
    short frame;
} BattleMagicFullCureParticlePool;

// clang-format off
// GLOBAL: FF7 0x00b39618
BattleMagicFullCureParticlePool g_BattleMagicFullCureParticlePool = {0};
unsigned char g_bssPad_00b3961c[4] = {0};
unsigned char g_bssPad_00b39620[5568] = {0};
unsigned char g_bssPad_00b3abe0[24] = {0};
// GLOBAL: FF7 0x00b3abf8
unsigned char g_BattleMagicFullCureSparkleScratchInit = 0;
// GLOBAL: FF7 0x00b3abfc
struct GfxTableCursor *g_BattleMagicFullCureLoadedResTable = 0;
// GLOBAL: FF7 0x00b3ac00
PsxMatrix g_BattleMagicFullCureTransformMatrix = {0};
// clang-format on
#pragma function(memcpy)
#pragma data_seg(".data$u4c_orbit")
#pragma bss_seg(".data$u4c_orbit")
#pragma data_seg(".bss$u4c_orbit_slot")
#pragma bss_seg(".bss$u4c_orbit_slot")
#pragma data_seg(".bss$u4c_orbit_flag")
#pragma bss_seg(".bss$u4c_orbit_flag")
#pragma data_seg()
#pragma bss_seg()

/* Loads Full Cure magic models and sprite resources. */
// FUNCTION: FF7 0x004d4790
void __cdecl BattleFx_MagicLoadResFullCure(void)
{
    GfxModelTableBank *context;
    GfxResourceRequest requestRecord;
    int resultKind;
    char pathBuffer[200];
    GfxResourceRequest *pReq;
    int idTable;

    if (g_pBattle3dScene != 0 && g_BattleMagicFullCureLoadedResTable == 0) {
        resultKind = 3;
        idTable = (int)&g_BattleMagicFullCureMagicId;
        context = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pReq = &requestRecord;
        Gfx_InitResourceRequest(pReq);
        pReq->groupTag = 0;
        // STRING: FF7 0x0082efe4
        pReq->resourcePath = BattleFx_BuildResourcePath("MAGIC/FULL", pathBuffer);
        Gfx_LoadModelTableIfAbsent(resultKind, idTable, pReq, &g_BattleMagicFullCureLoadedResTable, context);
    }
}

/* Starts Full Cure: clears the paused-frame count, loads the resources and begins the cast. */
// FUNCTION: FF7 0x004d4820
void __cdecl BattleFx_MagicEnterFullCure(int targetMask, int actorIdx)
{
    g_BattleMagicFullCurePausedFrames = 0;
    BattleFx_MagicLoadResFullCure();
    BattleFx_MagicBeginFullCure(targetMask, actorIdx);
}

/* Begins the cast: empties the sparkle pool, starts its update task, starts a timeline for each
 * target and plays the cast sound. */
// FUNCTION: FF7 0x004d4844
void __cdecl BattleFx_MagicBeginFullCure(int targetMask, int actorIdx)
{
    BattleFx_MagicFullCureNoOp();
    BattleFx_MagicFullCureClearParticleCallbacks();
    Battle_AllocateResourceSlot(BattleFx_MagicFullCureParticleCallbackTask);
    Battle_CreateBitPatternCallbackTask(targetMask, actorIdx, 0, BattleFx_MagicSpawnFullCurePerTarget);
    Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputePanForEntity(targetMask), 0x1FE);
}

/* Clears all entries in the active particle callback table. */
// FUNCTION: FF7 0x004d4893
void __cdecl BattleFx_MagicFullCureClearParticleCallbacks(void)
{
    int i;

    for (i = 0; i < 0xC8; i++) {
        g_BattleMagicFullCureParticleCallbacks[i] = 0;
    }
}

/* Runs each live sparkle's callback and frees the sparkles that finished; frees its own task slot
 * when none is left and no other secondary task runs. */
// FUNCTION: FF7 0x004d48c6
void __cdecl BattleFx_MagicFullCureParticleCallbackTask(void)
{
    struct {
        int particleIndex;
        BattleTaskSlot *slot;
        int active;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    frame.slot = BATTLE_CURRENT_TASK_SLOT;
    frame.active = 0;
    for (frame.particleIndex = 0; frame.particleIndex < 0xc8; frame.particleIndex++) {
        if (g_BattleMagicFullCureParticleCallbacks[frame.particleIndex] != 0) {
            g_BattleMagicFullCureParticleCallbacks[frame.particleIndex](&((BattleFxMagicFullCureSparkle *)&g_BattleMagicFullCureParticlePool)[frame.particleIndex]);
            if (((BattleFxMagicFullCureSparkle *)&g_BattleMagicFullCureParticlePool)[frame.particleIndex].state == -1) {
                g_BattleMagicFullCureParticleCallbacks[frame.particleIndex] = 0;
            } else {
                frame.active++;
            }
        }
    }
    if (frame.active == 0 && g_BattleSecondaryTaskActiveCount <= 1) {
        frame.slot->ownerSlot = -1;
    }
}

/* Starts one target's timeline task at the position of the target's targeting bone. */
// FUNCTION: FF7 0x004d4971
void __cdecl BattleFx_MagicSpawnFullCurePerTarget(int actor)
{
    BattleFxFullCureTimelineSlot *slot;

    slot = (BattleFxFullCureTimelineSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicFullCureTimelineTask);
    slot->ownerSlot = actor;
    Battle_GetBoneWorldPos(actor, g_BattleActorRecords[actor].targetBone, &slot->targetPos.vx);
}

/* Runs one target's timeline: ramps the light up, spawns the growing beams at frame 5, the rising
 * sparkles at 10, the spinning sparkles at 15 and the closing beam at 25, calls Battle_PostActorHitOrHurtMessage for
 * the target at 35, and ramps the light back down and ends at 75. */
// FUNCTION: FF7 0x004d49c2
void __cdecl BattleFx_MagicFullCureTimelineTask(void)
{
    BattleFxFullCureTimelineSlot *task;
    BattleFxFullCureTimelineSlot *child;

    task = (BattleFxFullCureTimelineSlot *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (task->frame == 0) {
        g_BattleFxTimelineState.red = 0;
        g_BattleFxTimelineState.channel1 = 0;
        g_BattleFxTimelineState.channel2 = 0;
        Battle_StartFixedPointRamp(0x800, 5);
    }
    if (task->frame == 5) {
        child = (BattleFxFullCureTimelineSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicFullCureSpawnGrowingModelsTask);
        child->targetPos = task->targetPos;
    }
    if (task->frame == 0xA) {
        child = (BattleFxFullCureTimelineSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicFullCureSpawnRisingSpritesTask);
        child->targetPos = task->targetPos;
    }
    if (task->frame == 0xF) {
        child = (BattleFxFullCureTimelineSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicFullCureSpawnFadingRisingSpritesTask);
        child->targetPos = task->targetPos;
    }
    if (task->frame == 0x19) {
        child = (BattleFxFullCureTimelineSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicFullCureShrinkingModelTask);
        child->targetPos = task->targetPos;
    }
    if (task->frame == 0x23) {
        Battle_PostActorHitOrHurtMessage(task->ownerSlot);
    }
    if (task->frame == 0x4B) {
        Battle_StartFixedPointRamp(0, 5);
    }
    if (task->frame == 0x4B) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    (task->frame)++;
}

/* Spawns a growing light beam every fifth frame for 30 frames. */
// FUNCTION: FF7 0x004d4b5d
void __cdecl BattleFx_MagicFullCureSpawnGrowingModelsTask(void)
{
    BattleFxFullCureTimelineSlot *task;
    BattleFxFullCureTimelineSlot *child;

    task = (BattleFxFullCureTimelineSlot *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (task->frame % 5 == 0) {
        child = (BattleFxFullCureTimelineSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicFullCureGrowingModelTask);
        child->targetPos = task->targetPos;
    }
    (task->frame)++;
    if (task->frame >= 0x1E) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
}

/* Grows one light beam over 15 frames at the target position and draws it. */
// FUNCTION: FF7 0x004d4bee
void __cdecl BattleFx_MagicFullCureGrowingModelTask(void)
{
    int localFrame;
    BattleFxFullCureTimelineSlot *task;
    int ratio;

    task = (BattleFxFullCureTimelineSlot *)BATTLE_CURRENT_TASK_SLOT;
    if ((g_BattleMagicFullCureGrowMatrixInit & 1) == 0) {
        g_BattleMagicFullCureGrowMatrixInit |= 1;
        g_BattleMagicFullCureGrowMatrix = (PsxMatrix *)Gfx_GetPsxScratchpadPtr(0);
    }
    g_BattleMagicFullCureModelDef1.modelGroup = Gfx_ModelTableGetRenderObject(0, g_BattleMagicFullCureLoadedResTable);
    localFrame = task->frame;
    if (localFrame < 0xF) {
        /* The mesh lies on its side: m[0][0], m[1][2] and m[2][1] carry the scale. */
        ratio = (localFrame << 0xC) / 0xF;
        g_BattleMagicFullCureTransformMatrix.m[2][1] = (ratio * 0x780 >> 0xC) + 0x80;
        g_BattleMagicFullCureTransformMatrix.m[1][2] = g_BattleMagicFullCureTransformMatrix.m[2][1];
        g_BattleMagicFullCureTransformMatrix.m[0][0] = g_BattleMagicFullCureTransformMatrix.m[1][2];
        g_BattleMagicFullCureModelDef1.fadeLevel = ratio;
    } else {
        task->ownerSlot = BATTLE_TASK_FREE;
        return;
    }
    g_BattleMagicFullCureTransformMatrix.t[0] = task->targetPos.vx;
    g_BattleMagicFullCureTransformMatrix.t[1] = 0;
    g_BattleMagicFullCureTransformMatrix.t[2] = task->targetPos.vz;
    MultiplyMatrix(&g_BattleCameraTransform, &g_BattleMagicFullCureTransformMatrix, g_BattleMagicFullCureGrowMatrix);
    SetRotMatrix(g_BattleMagicFullCureGrowMatrix);
    SetTransMatrix(g_BattleMagicFullCureGrowMatrix);
    SetDrawColorRGB(0, 0, 0);
    g_BattleMagicFullCureDrawHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleMagicFullCureModelDef1, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleMagicFullCureDrawHandle);
    if (g_BattleIsPaused == 0) {
        (task->frame)++;
    }
}

/* For 20 frames, spawns three rising sparkles a frame at random points around the target. */
// FUNCTION: FF7 0x004d4d64
void __cdecl BattleFx_MagicFullCureSpawnRisingSpritesTask(void)
{
    int angle;
    BattleFxFullCureTimelineSlot *task;
    int extent;
    int k;
    BattleFxMagicFullCureSparkle *pSprite;
    int rndRadius;
    int speedValue;
    int hRoll;

    task = (BattleFxFullCureTimelineSlot *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    for (k = 0; k < 3; k++) {
        pSprite = BattleFx_MagicFullCureClaimParticleRecord(BattleFx_MagicFullCureRisingSpriteParticle);
        if (DEAD_IN_RETAIL != 0) {
            rndRadius = 0x3E8;
        } else {
            rndRadius = rand() % 0x3E8;
        }
        extent = rndRadius;
        angle = rand() & 0xFFF;
        pSprite->pos.vx = task->targetPos.vx + (extent * SinFixed(angle) >> 0xC);
        pSprite->pos.vy = 0;
        pSprite->pos.vz = task->targetPos.vz + (extent * CosFixed(angle) >> 0xC);
        if (DEAD_IN_RETAIL != 0) {
            hRoll = 0x190;
        } else {
            hRoll = rand() % 0xC8 + 0xC8;
        }
        pSprite->velocityY = -hRoll;
        if (DEAD_IN_RETAIL != 0) {
            speedValue = 0x800;
        } else {
            speedValue = rand() % 0x400 + 0x400;
        }
        pSprite->scale = speedValue;
    }
    (task->frame)++;
    if (task->frame >= 0x14) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
}

/* Claims a free pool sparkle for the callback; returns it, or 0 when the pool is full. */
// FUNCTION: FF7 0x004d4ed5
BattleFxMagicFullCureSparkle *__cdecl BattleFx_MagicFullCureClaimParticleRecord(void (__cdecl *pIndex)(BattleFxMagicFullCureSparkle *))
{
    int i;

    for (i = 0; i < 0xC8; i++) {
        if (g_BattleMagicFullCureParticleCallbacks[i] == 0) {
            g_BattleMagicFullCureParticleCallbacks[i] = pIndex;
            ((BattleFxMagicFullCureSparkle *)&g_BattleMagicFullCureParticlePool)[i].state = 0;
            ((BattleFxMagicFullCureSparkle *)&g_BattleMagicFullCureParticlePool)[i].frame = 0;
            return &((BattleFxMagicFullCureSparkle *)&g_BattleMagicFullCureParticlePool)[i];
        }
    }
    return 0;
}

/* Draws a rising sparkle, which fades out after 10 frames, and moves it up by its damped speed. */
// FUNCTION: FF7 0x004d4f41
void __cdecl BattleFx_MagicFullCureRisingSpriteParticle(BattleFxMagicFullCureSparkle *particleRecord)
{
    int localFrame;
    int frac;

    g_BattleMagicFullCureSpriteRequest1.drawObj = Gfx_ModelTableGetSpriteObject(2, g_BattleMagicFullCureLoadedResTable);
    localFrame = particleRecord->frame;
    if (localFrame < 0xA) {
        g_BattleMagicFullCureSpriteRequest1.colorB = 0x80;
        g_BattleMagicFullCureSpriteRequest1.colorG = g_BattleMagicFullCureSpriteRequest1.colorB;
        g_BattleMagicFullCureSpriteRequest1.colorR = g_BattleMagicFullCureSpriteRequest1.colorG;
    } else {
        localFrame -= 0xA;
        if (localFrame < 5) {
            frac = (localFrame << 0xC) / 5;
            g_BattleMagicFullCureSpriteRequest1.colorB = (frac * -0x80 >> 0xC) + 0x80;
            g_BattleMagicFullCureSpriteRequest1.colorG = g_BattleMagicFullCureSpriteRequest1.colorB;
            g_BattleMagicFullCureSpriteRequest1.colorR = g_BattleMagicFullCureSpriteRequest1.colorG;
        } else {
            particleRecord->state = 0xFFFF;
            return;
        }
    }
    BattleFx_InstallBillboardMatrix(&particleRecord->pos, (short)particleRecord->scale, 0);
    g_BattleMagicFullCureDrawHandle = BattleFx_SubmitSpriteQuadDraw((BattleFxSpriteQuadRequest *)&g_BattleMagicFullCureSpriteDef1, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleMagicFullCureDrawHandle);
    if (g_BattleIsPaused == 0) {
        (particleRecord->frame)++;
        particleRecord->pos.vy += particleRecord->velocityY;
        particleRecord->velocityY = particleRecord->velocityY * 0xF80 >> 0xC;
    }
}

/* For 20 frames, spawns three spinning sparkles a frame at random points around the target. */
// FUNCTION: FF7 0x004d5070
void __cdecl BattleFx_MagicFullCureSpawnFadingRisingSpritesTask(void)
{
    int angle;
    BattleFxFullCureTimelineSlot *task;
    int extent;
    int k;
    BattleFxMagicFullCureSparkle *pSprite;
    int rndRadius;
    int speedValue;
    int accelTmp;
    int orbitRnd;

    task = (BattleFxFullCureTimelineSlot *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    for (k = 0; k < 3; k++) {
        pSprite = BattleFx_MagicFullCureClaimParticleRecord(BattleFx_MagicFullCureFadingRisingSpriteTask);
        if (DEAD_IN_RETAIL != 0) {
            rndRadius = 0xBB8;
        } else {
            rndRadius = rand() % 0x9C4 + 0x1F4;
        }
        extent = rndRadius;
        angle = rand() & 0xFFF;
        pSprite->pos.vx = task->targetPos.vx + (extent * SinFixed(angle) >> 0xC);
        pSprite->pos.vy = 0xF448;
        pSprite->pos.vz = task->targetPos.vz + (extent * CosFixed(angle) >> 0xC);
        pSprite->angles.vx = rand() & 0xFFF;
        pSprite->angles.vy = rand() & 0xFFF;
        pSprite->angles.vz = rand() & 0xFFF;
        if (DEAD_IN_RETAIL != 0) {
            speedValue = 0x800;
        } else {
            speedValue = rand() % 0x400 + 0x400;
        }
        pSprite->scale = speedValue;
        if (DEAD_IN_RETAIL != 0) {
            orbitRnd = 0xC8;
        } else {
            orbitRnd = rand() % 0xBE + 0xA;
        }
        pSprite->orbitRadius = orbitRnd;
        if (DEAD_IN_RETAIL != 0) {
            accelTmp = 0x55;
        } else {
            accelTmp = rand() % 0x32 + 0x23;
        }
        pSprite->fallSpeed = accelTmp;
    }
    (task->frame)++;
    if (task->frame >= 0x14) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
}

/* Draws a spinning sparkle, which fades in, holds and fades out over 35 frames, then spins it and
 * moves it down by its fall speed. */
// FUNCTION: FF7 0x004d5243
void __cdecl BattleFx_MagicFullCureFadingRisingSpriteTask(BattleFxMagicFullCureSparkle *particleRecord)
{
    int phaseFrame;
    int fade;

    if ((g_BattleMagicFullCureSparkleScratchInit & 1) == 0) {
        g_BattleMagicFullCureSparkleScratchInit |= 1;
        g_BattleMagicFullCureSparkleScratch = (BattleMagicFullCureSparkleScratch *)Gfx_GetPsxScratchpadPtr(0);
    }
    g_BattleMagicFullCureModelDef2.spriteDrawObj = Gfx_ModelTableGetSpriteObject(2, g_BattleMagicFullCureLoadedResTable);
    phaseFrame = particleRecord->frame;
    if (phaseFrame < 5) {
        fade = (phaseFrame << 0xC) / 5;
        g_BattleMagicFullCureSpriteRequest2.colorB = fade << 7 >> 0xC;
        g_BattleMagicFullCureSpriteRequest2.colorG = g_BattleMagicFullCureSpriteRequest2.colorB;
        g_BattleMagicFullCureSpriteRequest2.colorR = g_BattleMagicFullCureSpriteRequest2.colorG;
    } else {
        phaseFrame -= 5;
        if (phaseFrame < 0x19) {
            g_BattleMagicFullCureSpriteRequest2.colorB = 0x80;
            g_BattleMagicFullCureSpriteRequest2.colorG = g_BattleMagicFullCureSpriteRequest2.colorB;
            g_BattleMagicFullCureSpriteRequest2.colorR = g_BattleMagicFullCureSpriteRequest2.colorG;
        } else {
            phaseFrame -= 0x19;
            if (phaseFrame < 5) {
                fade = (phaseFrame << 0xC) / 5;
                g_BattleMagicFullCureSpriteRequest2.colorB = (fade * -0x80 >> 0xC) + 0x80;
                g_BattleMagicFullCureSpriteRequest2.colorG = g_BattleMagicFullCureSpriteRequest2.colorB;
                g_BattleMagicFullCureSpriteRequest2.colorR = g_BattleMagicFullCureSpriteRequest2.colorG;
            } else {
                particleRecord->state = 0xFFFF;
                return;
            }
        }
    }
    RotMatrix(&particleRecord->angles, &g_BattleMagicFullCureSparkleScratch->rotation);
    g_BattleMagicFullCureSparkleScratch->point.vx = particleRecord->orbitRadius;
    g_BattleMagicFullCureSparkleScratch->point.vz = 0;
    g_BattleMagicFullCureSparkleScratch->point.vy = 0;
    TransformVectorToInt16(&g_BattleMagicFullCureSparkleScratch->rotation, &g_BattleMagicFullCureSparkleScratch->point, &g_BattleMagicFullCureSparkleScratch->point.vx);
    g_BattleMagicFullCureSparkleScratch->point.vx += particleRecord->pos.vx;
    g_BattleMagicFullCureSparkleScratch->point.vy += particleRecord->pos.vy;
    g_BattleMagicFullCureSparkleScratch->point.vz += particleRecord->pos.vz;
    BattleFx_InstallBillboardMatrix(&g_BattleMagicFullCureSparkleScratch->point, (short)particleRecord->scale, 0);
    g_BattleMagicFullCureDrawHandle = BattleFx_SubmitSpriteQuadDraw((BattleFxSpriteQuadRequest *)&g_BattleMagicFullCureSpriteDef2, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleMagicFullCureDrawHandle);
    if (g_BattleIsPaused == 0) {
        (particleRecord->frame)++;
        particleRecord->pos.vy += particleRecord->fallSpeed;
        particleRecord->angles.vx += 0x12;
        particleRecord->angles.vy += 0x45;
        particleRecord->angles.vz += 0x78;
    }
}

/* Narrows and fades the closing light beam at the target position over 50 frames and draws it. */
// FUNCTION: FF7 0x004d54b2
void __cdecl BattleFx_MagicFullCureShrinkingModelTask(void)
{
    int frameCount;
    BattleFxFullCureTimelineSlot *task;
    int ratio;

    task = (BattleFxFullCureTimelineSlot *)BATTLE_CURRENT_TASK_SLOT;
    if ((g_BattleMagicFullCureShrinkMatrixInit & 1) == 0) {
        g_BattleMagicFullCureShrinkMatrixInit |= 1;
        g_BattleMagicFullCureShrinkMatrix = (PsxMatrix *)Gfx_GetPsxScratchpadPtr(0);
    }
    g_BattleMagicFullCureModelDef2.model.modelGroup = Gfx_ModelTableGetRenderObject(1, g_BattleMagicFullCureLoadedResTable);
    frameCount = task->frame;
    if (frameCount < 0xA) {
        ratio = (frameCount << 0xC) / 0xA;
        g_BattleMagicFullCureModelDef2.transform.m[2][2] = 0x1000;
        g_BattleMagicFullCureModelDef2.transform.m[0][0] = g_BattleMagicFullCureModelDef2.transform.m[2][2];
        g_BattleMagicFullCureModelDef2.model.fadeLevel = 0x1000 - ratio;
    } else {
        frameCount -= 0xA;
        if (frameCount < 0x28) {
            ratio = (frameCount << 0xC) / 0x28;
            g_BattleMagicFullCureModelDef2.transform.m[2][2] = (ratio * -0x1000 >> 0xC) + 0x1000;
            g_BattleMagicFullCureModelDef2.transform.m[0][0] = g_BattleMagicFullCureModelDef2.transform.m[2][2];
            if (frameCount < 0x14) {
                g_BattleMagicFullCureModelDef2.model.fadeLevel = 0;
            } else {
                ratio = (frameCount - 0x14 << 0xC) / 0x14;
                g_BattleMagicFullCureModelDef2.model.fadeLevel = ratio;
            }
        } else {
            task->ownerSlot = BATTLE_TASK_FREE;
            return;
        }
    }
    g_BattleMagicFullCureModelDef2.transform.t[0] = task->targetPos.vx;
    g_BattleMagicFullCureModelDef2.transform.t[1] = 0;
    g_BattleMagicFullCureModelDef2.transform.t[2] = task->targetPos.vz;
    MultiplyMatrix(&g_BattleCameraTransform, &g_BattleMagicFullCureModelDef2.transform, g_BattleMagicFullCureShrinkMatrix);
    SetRotMatrix(g_BattleMagicFullCureShrinkMatrix);
    SetTransMatrix(g_BattleMagicFullCureShrinkMatrix);
    SetDrawColorRGB(0, 0, 0);
    g_BattleMagicFullCureDrawHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleMagicFullCureModelDef2.model, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleMagicFullCureDrawHandle);
    if (g_BattleIsPaused == 0) {
        (task->frame)++;
    } else {
        g_BattleMagicFullCurePausedFrames++;
    }
}

/* Does nothing; BattleFx_MagicBeginFullCure calls it first. */
// FUNCTION: FF7 0x004d569a
void __cdecl BattleFx_MagicFullCureNoOp(void)
{
    int padSlot[3]; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
}
} /* extern "C" */
