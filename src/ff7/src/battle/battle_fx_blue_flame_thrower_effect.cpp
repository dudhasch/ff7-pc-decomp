// src/ff7/src/battle/battle_fx_blue_flame_thrower_effect.cpp
// Enemy Skill [8]: Flame Thrower (Japanese: 火炎放射, dev tag: "BLUE/KAENN" / Kaenn)
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Blue-magic Kaenn flame effect: entry points for its variants, plus the timeline tasks
//   that emit, update, draw and recycle flame particles along the breath cone, and the per-target
//   callbacks that start them.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/enums/sound.h"
#include "ff7/battle_tasks.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_fx.h"
#include "lib/types/graphics.h"
#include "lib/codegen.h"
#include <stdlib.h>
#include "ff7/camera_script.h"
#include <string.h>
#include "ff7/api/battle.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "lib/api/sound.h"
extern "C" {

struct Vec3sPacked;

#pragma intrinsic(abs)

// clang-format off
unsigned char g_rawData_008b4150[16] = {0xde, 0x01, 0x00, 0x00, 0xfa, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x008b4160
int g_BattleBlueFlameThrowerSoundGroups[2] = {9126224, 0};
// GLOBAL: FF7 0x008b4168
int g_BattleBlueFlameThrowerResourceDefs = 0x00192A39;
char *g_BattleFxBlueFlameThrowerResourceNames[1] = {
    // STRING: FF7 0x008b41b0
    "FLAME00.TIM"
};
unsigned int g_BattleFxBlueFlameThrowerResourceNames2[8] = {
    // STRING: FF7 0x008b41bc
    (unsigned int)"KAENN.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x008b41c8
    (unsigned int)"FLAME01.TIM",
    // STRING: FF7 0x008b41d4
    (unsigned int)"KAENN.SPT",
    0x00000000,
    0x00000000
};
// GLOBAL: FF7 0x008b4190
struct BattleBlueFlameThrowerSpriteDrawReq g_BattleBlueFlameThrowerSpriteDrawReq = {0, 0, {0x80, 0x80, 0x80, 0x2e}, 0, 0};
// GLOBAL: FF7 0x008b41a0
unsigned char g_BattleBlueFlameThrowerRotTransPoint1[8] = {0xd4, 0xfe, 0x70, 0xfe, 0xdb, 0xfb, 0x00, 0x00};
// clang-format on

// clang-format off
// GLOBAL: FF7 0x008b41a8
unsigned char g_BattleBlueFlameThrowerRotTransPoint2[8] = {0x2c, 0x01, 0x70, 0xfe, 0xdb, 0xfb, 0x00, 0x00};
// clang-format on

#pragma intrinsic(memset)
#pragma intrinsic(memcpy)



// clang-format off
// GLOBAL: FF7 0x00bbc860
struct BattleFxTimelineState *g_BattleBlueFlameThrowerDataBlock0 = 0;
// GLOBAL: FF7 0x00bbc864
char g_BattleBlueFlameThrowerFlag1 = 0;
// GLOBAL: FF7 0x00bbc868
struct BattleFxTimelineState *g_pBattleSpecialTwinBurnerScratch = 0;
// GLOBAL: FF7 0x00bbc86c
int g_BattleBlueFlameThrowerPacketCursor = 0;
// GLOBAL: FF7 0x00bbc870
int g_BattleBlueFlameThrowerTargetActorCentroid = 0;
// GLOBAL: FF7 0x00bbc874
char g_BattleBlueFlameThrowerFlag2 = 0;
// GLOBAL: FF7 0x00bbc878
char g_BattleBlueFlameThrowerFlag3 = 0;
// GLOBAL: FF7 0x00bbc87c
struct Vec3sPacked *g_BattleBlueFlameThrowerDataBlock1 = 0;
// GLOBAL: FF7 0x00bbc880
int g_BattleBlueFlameThrowerVariantFlag = 0;
unsigned char g_bssPad_00bbc884[4] = {0};
// GLOBAL: FF7 0x00bbc888
struct BattleFxFlameThrowerParticle g_BattleBlueFlameThrowerParticleRecords58bd1[200] = {0};
// GLOBAL: FF7 0x00bbe188
struct GfxTableCursor *g_BattleBlueFlameThrowerLoadedResTable58bd1 = 0;
// GLOBAL: FF7 0x00bbe18c
int g_BattleBlueFlameThrowerAngleStep = 0;

/*
 * Loads resources and registers sound groups for Kaenn.
 */
// FUNCTION: FF7 0x0058bd10
void __cdecl BattleFx_BlueLoadResFlameThrower(void)
{
    GfxModelTableBank *loadBits;
    GfxResourceRequest requestRecord;
    int kind;
    char pathBuffer[200];
    GfxResourceRequest *request;
    int defPtr;

    if (g_pBattle3dScene != 0 && g_BattleBlueFlameThrowerLoadedResTable58bd1 == 0) {
        kind = 2;
        defPtr = (int)&g_BattleBlueFlameThrowerResourceDefs;
        loadBits = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        request = &requestRecord;
        Gfx_InitResourceRequest(request);
        request->groupTag = 0;
        // STRING: FF7 0x008b41e0
        request->resourcePath = BattleFx_BuildResourcePath("BLUE/KAENN", pathBuffer);
        Gfx_LoadModelTableIfAbsent(kind, defPtr, request, &g_BattleBlueFlameThrowerLoadedResTable58bd1, loadBits);
        SfxRegisterSoundGroups((struct SoundGroup **)&g_BattleBlueFlameThrowerSoundGroups);
    }
}

/*
 * Entry point for standard Kaenn blue magic effect.
 */
// FUNCTION: FF7 0x0058bdb1
void __cdecl BattleFx_BlueEnterFlameThrower(int targetMask, int actorIdx)
{
    BattleFx_BlueLoadResFlameThrower();
    g_BattleBlueFlameThrowerVariantFlag = BATTLE_BLUE_FLAME_THROWER_PALETTE_ROW0;
    BattleFx_BlueRegisterFlameThrowerPerTargetCallback(targetMask, actorIdx);
    Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputePanForEntity(targetMask), 0x1DE);
}

/*
 * Entry point for Kaenn variation 1 effect.
 */
// FUNCTION: FF7 0x0058bdf1
void __cdecl BattleFx_SpecialEnterFlameThrowerVar1(int targetMask, int actorIdx)
{
    BattleFx_BlueLoadResFlameThrower();
    g_BattleBlueFlameThrowerVariantFlag = BATTLE_BLUE_FLAME_THROWER_PALETTE_ROW1;
    BattleFx_BlueRegisterFlameThrowerPerTargetCallback(g_BattleEffectScratch.targetMask, actorIdx);
    Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputePanForEntity(g_BattleEffectScratch.targetMask), 0x1FA);
}

/*
 * Entry point for left-oriented Kaenn effect.
 */
// FUNCTION: FF7 0x0058be39
void __cdecl BattleFx_SpecialEnterFlameThrowerLeft(int targetMask, int actorIdx)
{
    BattleFx_BlueLoadResFlameThrower();
    g_BattleBlueFlameThrowerVariantFlag = BATTLE_BLUE_FLAME_THROWER_PALETTE_ROW1;
    BattleFx_BlueRegisterFlameThrowerLeftPerTargetCallback(g_BattleEffectScratch.targetMask, actorIdx);
    Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputePanForEntity(g_BattleEffectScratch.targetMask), 0x1FA);
}

/*
 * Entry point for Kaenn fang attack variation.
 */
// FUNCTION: FF7 0x0058be81
void __cdecl BattleFx_SpecialEnterFlameThrowerFang(int targetMask, int actorIdx)
{
    BattleFx_BlueLoadResFlameThrower();
    g_BattleBlueFlameThrowerVariantFlag = BATTLE_BLUE_FLAME_THROWER_PALETTE_ROW0;
    BattleFx_BlueBeginFlameThrowerFang(targetMask, actorIdx);
    Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputePanForEntity(targetMask), 0x1DE);
}

/*
 * Entry point for Kaenn fang attack variation without sound.
 */
// FUNCTION: FF7 0x0058bec1
void __cdecl BattleFx_SpecialEnterFlameThrowerFangNoSound(int targetMask, int actorIdx)
{
    BattleFx_BlueLoadResFlameThrower();
    g_BattleBlueFlameThrowerVariantFlag = BATTLE_BLUE_FLAME_THROWER_PALETTE_ROW1;
    BattleFx_BlueBeginFlameThrowerFang(targetMask, actorIdx);
}

/*
 * Entry point for Kaenn variation 2 effect.
 */
// FUNCTION: FF7 0x0058bee5
void __cdecl BattleFx_SpecialEnterFlameThrowerVar2(int targetMask, int actorIdx)
{
    BattleFx_BlueLoadResFlameThrower();
    g_BattleBlueFlameThrowerVariantFlag = BATTLE_BLUE_FLAME_THROWER_PALETTE_ALTERNATE;
    BattleFx_BlueBeginFlameThrowerFang(targetMask, actorIdx);
    Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_2, Battle_ComputePanForEntity(targetMask), 0);
}

/*
 * Entry point for twin-stream Kaenn effect.
 */
// FUNCTION: FF7 0x0058bf22
void __cdecl BattleFx_SpecialEnterTwinBurner(int targetMask, int actorIdx)
{
    BattleFx_BlueLoadResFlameThrower();
    g_BattleBlueFlameThrowerVariantFlag = BATTLE_BLUE_FLAME_THROWER_PALETTE_ROW0;
    BattleFx_SpecialBeginTwinBurner(targetMask, actorIdx);
    Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputePanForEntity(targetMask), 0x1DE);
}

/*
 * Registers per-target callback task for Kaenn.
 */
// FUNCTION: FF7 0x0058bf62
void __cdecl BattleFx_BlueRegisterFlameThrowerPerTargetCallback(int targetMask, int casterIndex)
{
    Battle_CreateBitPatternCallbackTask(targetMask, casterIndex, 0, BattleFx_BlueSpawnFlameThrowerPerTarget);
}

/*
 * Spawns timeline task for a single Kaenn target.
 */
// FUNCTION: FF7 0x0058bf7e
void __cdecl BattleFx_BlueSpawnFlameThrowerPerTarget(int actor, int casterIndex)
{
    struct {
        BattleFxFlameThrowerSlot *slot;
        int unused0;
        int unused1;
        short pos[4];
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    frame.slot = (BattleFxFlameThrowerSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_BlueFlameThrowerTimelineTask);
    frame.slot->ownerSlot = actor;
    Battle_GetBoneWorldPos(actor, g_BattleActorRecords[actor].targetBone, frame.pos);
    Battle_GetBoneWorldPos(casterIndex, g_BattleActorRecords[casterIndex].bone33, &frame.slot->posX);
    frame.slot->dirX = frame.pos[0] - frame.slot->posX;
    frame.slot->dirY = frame.pos[1] - frame.slot->posY;
    frame.slot->dirZ = frame.pos[2] - frame.slot->posZ;
}

/*
 * Main timeline task controlling flame emission for Kaenn.
 */
// FUNCTION: FF7 0x0058c02e
void __cdecl BattleFx_BlueFlameThrowerTimelineTask(void)
{
    BattleFxFlameThrowerSlot *task;

    task = (BattleFxFlameThrowerSlot *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (task->step == 0) {
        BattleFx_BlueFlameThrowerClearParticleRecords();
        Battle_AllocateResourceSlot(BattleFx_BlueFlameThrowerParticleArrayTask);
    }
    if (task->step >= 0 && task->step < 18) {
        BattleFx_BlueFlameThrowerSpawnParticles((Vec3sPacked *)&task->posX, (Vec3sPacked *)&task->dirX, task->step, 4);
    }
    if (task->step == 30) {
        Battle_PostActorHitOrHurtMessage(task->ownerSlot);
    }
    if (task->step == 30) {
        task->ownerSlot = BATTLE_TASK_FREE;
        Sfx_ReleaseCachedBuffer(0x1DE, 0xF);
        Sfx_ReleaseCachedBuffer(0x1FA, 0xF);
    }
    (task->step)++;
}

/*
 * Updates and renders active flame particles for Kaenn.
 */
// FUNCTION: FF7 0x0058c10c
void __cdecl BattleFx_BlueFlameThrowerParticleArrayTask(void)
{
    int activeCount;
    BattleTaskSlot *task;
    int particleIdx;

    task = BATTLE_CURRENT_TASK_SLOT;
    activeCount = 0;
    for (particleIdx = 0; particleIdx < 200; particleIdx++) {
        if (g_BattleBlueFlameThrowerParticleRecords58bd1[particleIdx].state != -1) {
            BattleFx_BlueFlameThrowerDrawParticle((BattleFxFlameThrowerParticle *)((particleIdx << 5) + (char *)&g_BattleBlueFlameThrowerParticleRecords58bd1));
            activeCount++;
        }
    }
    if (activeCount == 0 && g_BattleSecondaryTaskActiveCount <= 2) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Transforms and renders a single flame particle for Kaenn.
 */
// FUNCTION: FF7 0x0058c196
void __cdecl BattleFx_BlueFlameThrowerDrawParticle(BattleFxFlameThrowerParticle *particle)
{
    int sinValue;
    int yScaled;
    int yOfsCopy;

    if ((*(unsigned char *)&g_BattleBlueFlameThrowerFlag2 & 1) == 0) {
        g_BattleBlueFlameThrowerFlag2 |= 1;
        g_BattleBlueFlameThrowerDataBlock1 = (Vec3sPacked *)Gfx_GetPsxScratchpadPtr(0);
    }
    (g_BattleBlueFlameThrowerSpriteDrawReq.drawReqPtr) = Gfx_ModelTableGetLoadedRecord(0, g_BattleBlueFlameThrowerLoadedResTable58bd1);
    g_BattleBlueFlameThrowerSpriteDrawReq.kaennSpriteDrawHandle = Gfx_ModelTableGetSpriteObject(0, g_BattleBlueFlameThrowerLoadedResTable58bd1);
    if ((g_BattleBlueFlameThrowerSpriteDrawReq.drawReqPtr) != 0) {
        ((BattleFxBlueFlameThrowerDrawReq *)(g_BattleBlueFlameThrowerSpriteDrawReq.drawReqPtr))->drawObj0 = Gfx_ModelTableGetSpriteObject(0, g_BattleBlueFlameThrowerLoadedResTable58bd1);
        ((BattleFxBlueFlameThrowerDrawReq *)(g_BattleBlueFlameThrowerSpriteDrawReq.drawReqPtr))->drawObj1 = Gfx_ModelTableGetSpriteObject(1, g_BattleBlueFlameThrowerLoadedResTable58bd1);
    }
    sinValue = SinFixed(particle->angleSpeed * particle->frameCounter / 20);
    g_BattleBlueFlameThrowerDataBlock1->vx = particle->posX + (particle->ampX * sinValue >> 12);
    if (particle->offsetY < 0) {
        yScaled = particle->offsetY;
    } else {
        yScaled = particle->offsetY / 4;
    }
    yOfsCopy = yScaled;
    g_BattleBlueFlameThrowerDataBlock1->vy = particle->posY + (particle->ampY * sinValue >> 12) + particle->offsetY;
    g_BattleBlueFlameThrowerDataBlock1->vz = particle->posZ + (particle->ampZ * sinValue >> 12);
    g_BattleBlueFlameThrowerSpriteDrawReq.kaennSpriteDrawFade = (particle->frameCounter * 10 / particle->lifeFrames) | 0x8000;
    g_BattleBlueFlameThrowerSpriteDrawReq.kaennSpriteDrawAux = particle->clutWord;
    BattleFx_InstallBillboardMatrix(g_BattleBlueFlameThrowerDataBlock1, 0x1800 /* 1.5 */, 0xFFFFFE80);
    g_BattleBlueFlameThrowerPacketCursor = BattleFx_SubmitSpriteDraw(&g_BattleBlueFlameThrowerSpriteDrawReq, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleBlueFlameThrowerPacketCursor);
    if (g_BattleIsPaused == 0) {
        (particle->frameCounter)++;
        if (particle->frameCounter >= particle->lifeFrames) {
            particle->state = 0xFFFF;
        } else {
            particle->offsetY += particle->velocityY;
            particle->velocityY += particle->accelY;
        }
    }
}

/*
 * Clears and resets all flame particle records for Kaenn.
 */
// FUNCTION: FF7 0x0058c3be
void __cdecl BattleFx_BlueFlameThrowerClearParticleRecords(void)
{
    int i;

    for (i = 0; i < 200; i++) {
        g_BattleBlueFlameThrowerParticleRecords58bd1[i].state = 0xFFFF;
    }
}

/*
 * Emits new flame particle records along breath cone for Kaenn.
 */
// FUNCTION: FF7 0x0058c3f2
void __cdecl BattleFx_BlueFlameThrowerSpawnParticles(Vec3sPacked *origin, Vec3sPacked *direction, int angle, int count)
{
    int speed;
    int spawnIndex;
    BattleFxFlameThrowerParticle *particle;
    int delta;
    int span;

    for (spawnIndex = 0; spawnIndex < count; spawnIndex++) {
        particle = (BattleFxFlameThrowerParticle *)BattleFx_BlueFlameThrowerClaimParticleRecord();
        particle->lifeFrames = rand() % 15 + 10;
        particle->posX = origin->vx + rand() % 200 + -100;
        particle->posY = origin->vy + rand() % 200 + -100;
        particle->posZ = origin->vz + rand() % 200 + -100;
        speed = (SinFixed((angle << 12) / 6) << 10 >> 12) + (rand() % 1024) + 3584;
        particle->ampX = direction->vx * speed >> 12;
        particle->ampY = direction->vy * speed >> 12;
        particle->ampZ = direction->vz * speed >> 12;
        particle->offsetY = 0;
        particle->velocityY = 75;
        if (DEAD_IN_RETAIL != 0) {
            delta = 0x14;
        } else {
            delta = rand() % 15 + 5;
        }
        particle->accelY = -delta;
        if (DEAD_IN_RETAIL != 0) {
            span = 0x5C0;
        } else {
            span = rand() % 384 + 1088;
        }
        particle->angleSpeed = span;
        switch (g_BattleBlueFlameThrowerVariantFlag) {
        case BATTLE_BLUE_FLAME_THROWER_PALETTE_ROW0:
            particle->clutWord = 0;
            break;
        case BATTLE_BLUE_FLAME_THROWER_PALETTE_ROW1:
            particle->clutWord = 0x40;
            break;
        case BATTLE_BLUE_FLAME_THROWER_PALETTE_ALTERNATE:
            particle->clutWord = *(short *)&g_BattleBlueFlameThrowerAngleStep;
        }
    }
    g_BattleBlueFlameThrowerAngleStep ^= 0x40;
}

/*
 * Allocates an available flame particle record slot for Kaenn.
 */
// FUNCTION: FF7 0x0058c5d4
int __cdecl BattleFx_BlueFlameThrowerClaimParticleRecord(void)
{
    int i;

    for (i = 0; i < 200; i++) {
        if (g_BattleBlueFlameThrowerParticleRecords58bd1[i].state == -1) {
            g_BattleBlueFlameThrowerParticleRecords58bd1[i].state = 0;
            g_BattleBlueFlameThrowerParticleRecords58bd1[i].frameCounter = 0;
            return (i << 5) + (int)&g_BattleBlueFlameThrowerParticleRecords58bd1;
        }
    }
    return 0;
}

/*
 * Registers per-target callback task for left-oriented Kaenn.
 */
// FUNCTION: FF7 0x0058c638
void __cdecl BattleFx_BlueRegisterFlameThrowerLeftPerTargetCallback(int targetMask, int casterIndex)
{
    Battle_CreateBitPatternCallbackTask(targetMask, casterIndex, 0, BattleFx_BlueSpawnFlameThrowerLeftPerTarget);
}

/*
 * Spawns timeline task for left-oriented Kaenn target.
 */
// FUNCTION: FF7 0x0058c654
void __cdecl BattleFx_BlueSpawnFlameThrowerLeftPerTarget(int actor, int casterIndex)
{
    struct {
        BattleFxFlameThrowerSlot *slot;
        int unused0;
        int unused1;
        short pos[4];
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    frame.slot = (BattleFxFlameThrowerSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_BlueFlameThrowerTimelineTask);
    frame.slot->ownerSlot = actor;
    Battle_GetBoneWorldPos(actor, g_BattleActorRecords[actor].targetBone, frame.pos);
    Battle_GetBoneWorldPos(casterIndex, g_BattleActorRecords[casterIndex].bone34, &frame.slot->posX);
    frame.slot->dirX = frame.pos[0] - frame.slot->posX;
    frame.slot->dirY = frame.pos[1] - frame.slot->posY;
    frame.slot->dirZ = frame.pos[2] - frame.slot->posZ;
}

/*
 * Initializes Kaenn fang effect and spawns timeline task.
 */
// FUNCTION: FF7 0x0058c704
void __cdecl BattleFx_BlueBeginFlameThrowerFang(int targetMask, int actorIdx)
{
    struct {
        BattleFxFlameThrowerSlot *slot;
        BattleBonePosFrame target;
        BattleBonePosFrame source;
    } frame;

    Battle_GetActorMaskCentroid(g_BattleBlueFlameThrowerTargetActorCentroid = targetMask, frame.source.s);
    frame.source.s[1] = 0xfe0c;
    Battle_GetBoneWorldPos(actorIdx, g_BattleActorRecords[actorIdx].targetBone, frame.target.s);
    frame.source.s[0] -= frame.target.s[0];
    frame.source.s[1] -= frame.target.s[1];
    frame.source.s[2] -= frame.target.s[2];
    frame.slot = (BattleFxFlameThrowerSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_BlueFlameThrowerFangTimelineTask);
    frame.slot->ownerSlot = actorIdx;
    *(BattleBonePosCopy *)&frame.slot->dirX = frame.source.pair;
    frame.slot->angle = 0x200 /* 45 deg */;
    frame.slot->angleStep = 0xffc8;
}

/*
 * Timeline task controlling Kaenn fang flame projection.
 */
// FUNCTION: FF7 0x0058c7bc
void __cdecl BattleFx_BlueFlameThrowerFangTimelineTask(void)
{
    BattleFxFlameThrowerSlot *task;
    int targetIdx;

    task = (BattleFxFlameThrowerSlot *)BATTLE_CURRENT_TASK_SLOT;
    if ((*(unsigned char *)&g_BattleBlueFlameThrowerFlag1 & 1) == 0) {
        g_BattleBlueFlameThrowerFlag1 |= 1;
        g_BattleBlueFlameThrowerDataBlock0 = (struct BattleFxTimelineState *)Gfx_GetPsxScratchpadPtr(0);
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (task->step == 0) {
        BattleFx_BlueFlameThrowerClearParticleRecords();
        Battle_AllocateResourceSlot(BattleFx_BlueFlameThrowerParticleArrayTask);
    }
    if (task->step >= 0 && task->step < 18) {
        g_BattleBlueFlameThrowerDataBlock0->vecX = 0;
        g_BattleBlueFlameThrowerDataBlock0->wheelAngle = task->angle;
        g_BattleBlueFlameThrowerDataBlock0->spinSpeed = 0;
        RotMatrix((SVECTOR *)g_BattleBlueFlameThrowerDataBlock0, (PsxMatrix *)((int)g_BattleBlueFlameThrowerDataBlock0 + 0x10));
        TransformVectorToInt16((PsxMatrix *)((int)g_BattleBlueFlameThrowerDataBlock0 + 0x10), (SVECTOR *)&task->dirX, (short *)g_BattleBlueFlameThrowerDataBlock0);
        Battle_GetBoneWorldPos(task->ownerSlot, g_BattleActorRecords[task->ownerSlot].bone33, (short *)((char *)g_BattleBlueFlameThrowerDataBlock0 + 8));
        BattleFx_BlueFlameThrowerSpawnParticles((Vec3sPacked *)((int)g_BattleBlueFlameThrowerDataBlock0 + 8), (Vec3sPacked *)g_BattleBlueFlameThrowerDataBlock0, task->step, 4);
        g_BattleBlueFlameThrowerDataBlock0->vecX = 0;
        g_BattleBlueFlameThrowerDataBlock0->wheelAngle = -task->angle;
        g_BattleBlueFlameThrowerDataBlock0->spinSpeed = 0;
        RotMatrix((SVECTOR *)g_BattleBlueFlameThrowerDataBlock0, (PsxMatrix *)((int)g_BattleBlueFlameThrowerDataBlock0 + 0x10));
        TransformVectorToInt16((PsxMatrix *)((int)g_BattleBlueFlameThrowerDataBlock0 + 0x10), (SVECTOR *)&task->dirX, (short *)g_BattleBlueFlameThrowerDataBlock0);
        Battle_GetBoneWorldPos(task->ownerSlot, g_BattleActorRecords[task->ownerSlot].bone34, (short *)((char *)g_BattleBlueFlameThrowerDataBlock0 + 8));
        BattleFx_BlueFlameThrowerSpawnParticles((Vec3sPacked *)((int)g_BattleBlueFlameThrowerDataBlock0 + 8), (Vec3sPacked *)g_BattleBlueFlameThrowerDataBlock0, task->step, 4);
        task->angle += task->angleStep;
    }
    if (task->step == 30) {
        for (targetIdx = 0; targetIdx < 0xA; targetIdx++) {
            if ((g_BattleBlueFlameThrowerTargetActorCentroid & 1 << targetIdx) != 0) {
                Battle_PostActorHitOrHurtMessage(targetIdx);
            }
        }
    }
    if (task->step == 30) {
        task->ownerSlot = BATTLE_TASK_FREE;
        Sfx_ReleaseCachedBuffer(0x1DE, 0xF);
        Sfx_ReleaseCachedBuffer(0x1FA, 0xF);
    }
    (task->step)++;
}

/*
 * Initializes twin-stream Kaenn effect and spawns timeline task.
 */
// FUNCTION: FF7 0x0058ca50
void __cdecl BattleFx_SpecialBeginTwinBurner(int targetMask, int actorIdx)
{
    struct {
        BattleFxFlameThrowerSlot *slot;
        BattleBonePosFrame target;
        BattleBonePosFrame source;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    Battle_GetActorMaskCentroid(g_BattleBlueFlameThrowerTargetActorCentroid = targetMask, frame.source.s);
    frame.source.s[1] = 0xfe0c;
    Battle_GetBoneWorldPos(actorIdx, g_BattleActorRecords[actorIdx].targetBone, frame.target.s);
    frame.source.s[0] -= frame.target.s[0];
    frame.source.s[1] -= frame.target.s[1];
    frame.source.s[2] -= frame.target.s[2];
    frame.slot = (BattleFxFlameThrowerSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialTwinBurnerTimelineTask);
    frame.slot->ownerSlot = actorIdx;
    *(BattleBonePosCopy *)&frame.slot->dirX = frame.source.pair;
    frame.slot->angle = 0x200 /* 45 deg */;
    frame.slot->angleStep = 0xffc8;
}

/*
 * Timeline task controlling twin-stream flame projection for Kaenn.
 */
// FUNCTION: FF7 0x0058cb08
void __cdecl BattleFx_SpecialTwinBurnerTimelineTask(void)
{
    BattleFxFlameThrowerSlot *task;
    int targetIdx;
    int attachXform[8];

    task = (BattleFxFlameThrowerSlot *)BATTLE_CURRENT_TASK_SLOT;
    if ((*(unsigned char *)&g_BattleBlueFlameThrowerFlag3 & 1) == 0) {
        g_BattleBlueFlameThrowerFlag3 |= 1;
        g_pBattleSpecialTwinBurnerScratch = (struct BattleFxTimelineState *)(Gfx_GetPsxScratchpadPtr(0));
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (task->step == 0) {
        BattleFx_BlueFlameThrowerClearParticleRecords();
        Battle_AllocateResourceSlot(BattleFx_BlueFlameThrowerParticleArrayTask);
    }
    if (task->step >= 0 && task->step < 18) {
        Battle3d_GetActorAttachMatrix(task->ownerSlot, 2, (PsxMatrix *)attachXform);
        Battle_ComposeMatrixRelativeToCamera((short *)&g_BattleCameraTransform, attachXform, (PsxMatrix *)(((int)g_pBattleSpecialTwinBurnerScratch) + 0x30));
        SetRotMatrix((void *)(((int)g_pBattleSpecialTwinBurnerScratch) + 0x30));
        SetTransMatrix((void *)(((int)g_pBattleSpecialTwinBurnerScratch) + 0x30));
        RotTransPoint(&g_BattleBlueFlameThrowerRotTransPoint1, (char *)g_pBattleSpecialTwinBurnerScratch + 0x10, (char *)g_pBattleSpecialTwinBurnerScratch + 0x6C);
        RotTransPoint(&g_BattleBlueFlameThrowerRotTransPoint2, (char *)g_pBattleSpecialTwinBurnerScratch + 0x20, (char *)g_pBattleSpecialTwinBurnerScratch + 0x6C);
        g_pBattleSpecialTwinBurnerScratch->vecX = 0;
        g_pBattleSpecialTwinBurnerScratch->wheelAngle = task->angle;
        g_pBattleSpecialTwinBurnerScratch->spinSpeed = 0;
        RotMatrix((SVECTOR *)((int)g_pBattleSpecialTwinBurnerScratch), (char *)g_pBattleSpecialTwinBurnerScratch + 0x30);
        TransformVectorToInt16((void *)(((int)g_pBattleSpecialTwinBurnerScratch) + 0x30), (SVECTOR *)&task->dirX, (void *)((int)g_pBattleSpecialTwinBurnerScratch));
        g_pBattleSpecialTwinBurnerScratch->posX = g_pBattleSpecialTwinBurnerScratch->posZ;
        g_pBattleSpecialTwinBurnerScratch->vecY = g_pBattleSpecialTwinBurnerScratch->rotPoint1Y;
        g_pBattleSpecialTwinBurnerScratch->posY = g_pBattleSpecialTwinBurnerScratch->rotPoint1Z;
        BattleFx_BlueFlameThrowerSpawnParticles((Vec3sPacked *)(((int)g_pBattleSpecialTwinBurnerScratch) + 8), (Vec3sPacked *)((int)g_pBattleSpecialTwinBurnerScratch), task->step, 4);
        g_pBattleSpecialTwinBurnerScratch->vecX = 0;
        g_pBattleSpecialTwinBurnerScratch->wheelAngle = -task->angle;
        g_pBattleSpecialTwinBurnerScratch->spinSpeed = 0;
        RotMatrix((SVECTOR *)((int)g_pBattleSpecialTwinBurnerScratch), (char *)g_pBattleSpecialTwinBurnerScratch + 0x30);
        TransformVectorToInt16((void *)(((int)g_pBattleSpecialTwinBurnerScratch) + 0x30), (SVECTOR *)&task->dirX, (void *)((int)g_pBattleSpecialTwinBurnerScratch));
        g_pBattleSpecialTwinBurnerScratch->posX = g_pBattleSpecialTwinBurnerScratch->rotPoint2X;
        g_pBattleSpecialTwinBurnerScratch->vecY = g_pBattleSpecialTwinBurnerScratch->rotPoint2Y;
        g_pBattleSpecialTwinBurnerScratch->posY = g_pBattleSpecialTwinBurnerScratch->rotPoint2Z;
        BattleFx_BlueFlameThrowerSpawnParticles((Vec3sPacked *)(((int)g_pBattleSpecialTwinBurnerScratch) + 8), (Vec3sPacked *)((int)g_pBattleSpecialTwinBurnerScratch), task->step, 4);
        task->angle += task->angleStep;
    }
    if (task->step == 30) {
        for (targetIdx = 0; targetIdx < 0xA; targetIdx++) {
            if ((g_BattleBlueFlameThrowerTargetActorCentroid & 1 << targetIdx) != 0) {
                Battle_PostActorHitOrHurtMessage(targetIdx);
            }
        }
    }
    if (task->step == 30) {
        task->ownerSlot = BATTLE_TASK_FREE;
        Sfx_ReleaseCachedBuffer(0x1DE, 0xF);
        Sfx_ReleaseCachedBuffer(0x1FA, 0xF);
    }
    (task->step)++;
}
} /* extern "C" */
