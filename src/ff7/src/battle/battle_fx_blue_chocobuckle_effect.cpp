// src/ff7/src/battle/battle_fx_blue_chocobuckle_effect.cpp
// Enemy Skill [18]: Chocobuckle & Confuse status overlay (Japanese: チョコボックル, dev tag: "blue/chyobo" / Chyobo)
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: The Chyobo blue magic effect: loads its resources, then runs setup, timeline and sprite
//   tasks for orbiting, bouncing, scattering and billboard sprites.

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
#include "ff7/enums/battle.h"
extern "C" {

// clang-format off
// GLOBAL: FF7 0x008a8a30
int g_BattleBlueChocobuckleResourceDef = 0x00196A39;
int g_BattleBlueChocobuckleResourceDefHeadPtr[1] = {
    (int)&g_BattleFxBlueChocobuckleEffectResourceNames[0]
};
int g_BattleBlueChocobuckleResourceDefDataTail[8] = {
    (int)&g_BattleFxBlueChocobuckleEffectResourceNames[0xc], 0x0, 0x0, 0x192a39, (int)&g_BattleFxBlueChocobuckleEffectResourceNames[0x18], (int)&g_BattleFxBlueChocobuckleEffectResourceNames[0x24],
    0x0, 0x0
};
// GLOBAL: FF7 0x008a8a58
int g_BattleBlueChocobuckleParamVector1[3] = {0, 0, -0x1000};
// GLOBAL: FF7 0x008a8a68
int g_BattleBlueChocobuckleParamVector2[4] = {0, 0, -0x1000, 0};

// GLOBAL: FF7 0x008a8a78
BattleFxBlueChyoboSpriteReq g_BattleBlueChocobuckleSpriteDrawReq = {0, 0, 0x40, 0x40, 0x40, 0x2e};
// GLOBAL: FF7 0x008a8a90
BattleBlueChyoboColorStateAlt g_BattleBlueChocobuckleColor1 = {0x68, 0x68, 0x68, 0x2e};
// GLOBAL: FF7 0x008a8aa0
BattleBlueChyoboColorStateAlt g_BattleBlueChocobuckleColor2 = {0x68, 0x68, 0x68, 0x2e};
// GLOBAL: FF7 0x008a8ab0
BattleBlueChyoboColorBytes g_BattleBlueChocobuckleColor3 = {0x68, 0x68, 0x68, 0x2e};
unsigned char g_BattleFxBlueChocobuckleEffectResourceNames[48] =
    "CONFU_1.TIM\0"
    "CONFU_1.SPT\0"
    "SMOKE.TIM\0\0\0"
    "SMOKE_1.SPT";
// GLOBAL: FF7 0x00b73f98
int g_BattleBlueChocobuckleActorIdx = 0;
unsigned char g_bssPad_00b73f9c[4] = {0};
// GLOBAL: FF7 0x00b73fa0
int g_BattleBlueChocobuckleDeltaX = 0;
// GLOBAL: FF7 0x00b73fa4
int g_BattleBlueChocobuckleDeltaY = 0;
// GLOBAL: FF7 0x00b73fa8
int g_BattleBlueChocobuckleDeltaZ = 0;
// GLOBAL: FF7 0x00b73fac
int g_BattleBlueChocobuckleStepCount = 0;
// GLOBAL: FF7 0x00b73fb0
int g_BattleBlueChocobuckleTargetMask = 0;
// GLOBAL: FF7 0x00b73fb4
int g_BattleBlueChocobuckleOriginActor = 0;
// GLOBAL: FF7 0x00b73fb8
short g_BattleBlueChocobuckleJitterY = 0;
// GLOBAL: FF7 0x00b73fbc
short g_BattleBlueChocobuckleJitterX = 0;
// GLOBAL: FF7 0x00b73fc0
int g_BattleBlueChocobuckleScaleFactor = 0;
unsigned char g_bssPad_00b73fc4[4] = {0};
// GLOBAL: FF7 0x00b73fc8
struct BattleFxChocobuckleTargetPos g_BattleBlueChocobuckleTargetBonePos = {0};
// GLOBAL: FF7 0x00b73fd0
struct GfxTableCursor *g_BattleBlueChocobuckleLoadedRes = 0;
unsigned char g_bssPad_00b73fd4[4] = {0};
// clang-format on

#pragma intrinsic(memset)
#pragma intrinsic(memcpy)


/*
 * Loads resources for the Chyobo blue magic effect when not already loaded.
 */
// FUNCTION: FF7 0x0055fdd0
void __cdecl BattleFx_BlueLoadResChocobuckle(void)
{
    GfxModelTableBank *sceneHdr;
    GfxResourceRequest recordBuf;
    int resultNum;
    char pathArea[200];
    GfxResourceRequest *pRequest;
    int resEnt;

    if (g_pBattle3dScene != 0 && g_BattleBlueChocobuckleLoadedRes == 0) {
        resultNum = 2;
        resEnt = (int)&g_BattleBlueChocobuckleResourceDef;
        sceneHdr = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRequest = &recordBuf;
        Gfx_InitResourceRequest(pRequest);
        pRequest->groupTag = 0;
        // STRING: FF7 0x008a8ae8
        pRequest->resourcePath = BattleFx_BuildResourcePath("blue/chyobo", pathArea);
        Gfx_LoadModelTableIfAbsent(resultNum, resEnt, pRequest, &g_BattleBlueChocobuckleLoadedRes, sceneHdr);
    }
}

/*
 * Entry point for the Chyobo blue magic effect.
 */
// FUNCTION: FF7 0x0055fe60
int __cdecl BattleFx_BlueEnterChocobuckle(int targetMask, int actorIdx)
{
    BattleFx_BlueLoadResChocobuckle();
    BattleFx_BlueBeginChocobuckle(targetMask, actorIdx, 0);
    return 0;
}

// Loads Chyobo resources, calls BattleFx_BlueBeginChocobuckle with both arguments and variant one, and returns
// zero.
// FUNCTION: FF7 0x0055fe7e
int __cdecl BattleFx_BlueEnterChocobuckleAlt(int targetMask, int actorIdx)
{
    BattleFx_BlueLoadResChocobuckle();
    BattleFx_BlueBeginChocobuckle(targetMask, actorIdx, 1);
    return 0;
}

// Allocates BattleFx_SelectDrawListBuffer and stores its slot address, allocates BattleFx_BlueChocobuckleAltSetupTask when
// useAltSetup is nonzero or BattleFx_BlueChocobuckleSetupTask otherwise, and stores targetMask/originActor globally.
// FUNCTION: FF7 0x0055fe9c
void __cdecl BattleFx_BlueBeginChocobuckle(int targetMask, int originActor, int useAltSetup)
{
    g_BattleFxDrawListBufferTaskSlotPtr = (unsigned char *)BATTLE_NEW_TASK_SLOT(BattleFx_SelectDrawListBuffer);
    if (useAltSetup != 0) {
        Battle_AllocateResourceSlot(BattleFx_BlueChocobuckleAltSetupTask);
    } else {
        Battle_AllocateResourceSlot(BattleFx_BlueChocobuckleSetupTask);
    }
    g_BattleBlueChocobuckleTargetMask = targetMask;
    g_BattleBlueChocobuckleOriginActor = originActor;
}

// Sets up the blue Chyobo effect's timeline. It counts the targets in g_BattleBlueChocobuckleTargetMask,
// keeps the first one, retires the current task slot and allocates a
// BattleFx_BlueChocobuckleTimelineTask slot. A single target puts the timeline at its target bone, with
// posY from tmpOffset less 0x1C0, and spawns a BattleFx_BlueChocobuckleSpawnOrbitSpritesTask with
// spriteParam 0x460. Several targets put it at the mask's centroid, with posY at 0xF9E0, and spawn
// a BattleFx_BlueChocobuckleSpawnBouncingSpritesTask with spriteParam 0xA80. Finally posY drops by
// 0x2A0, yaw takes the origin actor's yaw plus 0x800 masked to 0xFFF, and actorYStep is 0x2A.
// FUNCTION: FF7 0x0055fef2
void __cdecl BattleFx_BlueChocobuckleSetupTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[32];
        BattleFxChocobuckleSetupSpawnSlot *setupSlot;
        struct { unsigned char pad_to_4[0x4]; BattleFxChocobuckleSpawnSlot *spriteSlot; };
        struct { unsigned char pad_to_8[0x8]; int firstTarget; };
        struct { unsigned char pad_to_C[0xC]; int targetCount; };
        struct { unsigned char pad_to_10[0x10]; int targetSlot; };
        struct { unsigned char pad_to_14[0x14]; int spriteParam; };
        struct { unsigned char pad_to_18[0x18]; int altBonePos; };
        struct { unsigned char pad_to_1A[0x1A]; short tmpOffset; };
    } frame;
#pragma pack(pop)

    frame.targetSlot = 0;
    for (frame.targetCount = 0; frame.targetSlot < 0xA; frame.targetSlot++) {
        if ((g_BattleBlueChocobuckleTargetMask & 1 << frame.targetSlot) != 0) {
            frame.targetCount++;
            if (frame.targetCount == 1) {
                frame.firstTarget = frame.targetSlot;
            }
        }
    }
    frame.setupSlot = (BattleFxChocobuckleSetupSpawnSlot *)BATTLE_CURRENT_TASK_SLOT;
    frame.setupSlot->ownerSlot = BATTLE_TASK_FREE;
    frame.setupSlot = (BattleFxChocobuckleSetupSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_BlueChocobuckleTimelineTask);
    if (frame.targetCount == 1) {
        Battle_GetBoneWorldPos(frame.firstTarget, g_BattleActorRecords[frame.firstTarget].targetBone, &frame.setupSlot->posX);
        Battle_GetBoneWorldPos(frame.firstTarget, g_BattleActorRecords[frame.firstTarget].altBone, (short *)&frame.altBonePos);
        frame.setupSlot->posY = frame.tmpOffset - 0x1C0;
        frame.spriteParam = 0x460;
        frame.spriteSlot = (BattleFxChocobuckleSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_BlueChocobuckleSpawnOrbitSpritesTask);
        frame.spriteSlot->pos = frame.setupSlot->pos;
        frame.spriteSlot->orbitRadius = *(short *)&frame.spriteParam;
    } else {
        Battle_GetActorMaskCentroid(g_BattleBlueChocobuckleTargetMask, &frame.setupSlot->posX);
        frame.setupSlot->posY = 0xF9E0;
        frame.spriteParam = 0xA80;
        frame.spriteSlot = (BattleFxChocobuckleSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_BlueChocobuckleSpawnBouncingSpritesTask);
        frame.spriteSlot->pos = frame.setupSlot->pos;
        frame.spriteSlot->baseOffset = *(short *)&frame.spriteParam;
    }
    frame.setupSlot->posY -= 0x2A0;
    frame.setupSlot->yaw = g_BattleActorRecords[g_BattleBlueChocobuckleOriginActor].yaw + 0x800 & 0xFFF;
    frame.setupSlot->actorYStep = 0x2A;
}

// Drives the blue Chyobo effect's timeline, one step per frame. It has no pause check. At step 1 it
// allocates a type-one ring record that copies the slot's position and yaw, and plays sound 0x19F
// panned from the position. Steps 1 to 16 add counterDelta to the posY of g_BattleActorRecords[3],
// and steps 42 to 57 subtract it. Step 42 allocates a type-two ring record. Step 54 calls
// Battle_PostActorHitOrHurtMessage for each set bit among the low ten of g_BattleBlueChocobuckleTargetMask. From step 60
// the slot is retired. The step counter always advances.
// FUNCTION: FF7 0x005600be
void __cdecl BattleFx_BlueChocobuckleTimelineTask(void)
{
    BattleFxChocobuckleRingRecord *ringRecord;
    int i;
    BattleFxBlueChocobuckleTimeline *task;

    task = (BattleFxBlueChocobuckleTimeline *)BATTLE_CURRENT_TASK_SLOT;
    if (task->step == 1) {
        ringRecord = (BattleFxChocobuckleRingRecord *)Battle_AllocSceneMessage(1);
        ringRecord->pos = task->pos;
        ringRecord->yaw = task->yaw;
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputeScreenPanFromPosition(&task->pos), 0x19F);
    } else if (task->step == 0x2A) {
        Battle_AllocSceneMessage(2);
    }
    if (task->step >= 1 && task->step < 0x11) {
        g_BattleActorRecords[3].posY += task->counterDelta;
    }
    if (task->step >= 0x2A && task->step < 0x3A) {
        g_BattleActorRecords[3].posY -= task->counterDelta;
    }
    if (task->step == 0x36) {
        for (i = 0; i < 0xA; i++) {
            if ((g_BattleBlueChocobuckleTargetMask & 1 << i) != 0) {
                Battle_PostActorHitOrHurtMessage(i);
            }
        }
    }
    if (task->step >= 0x3C) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    (task->step)++;
}

// Per-frame handler for the current slot of g_BattleTaskSlotBuffer.mainSlots, whose slots are 0x20
// bytes. At frame 0x11 it spawns eight BattleFx_BlueChocobuckleOrbitSpriteTask slots. Each copies this
// slot's position, takes angle i << 9 for i from 0 to 7, angleStep 0x55, index 0xAAA and this
// slot's spawnParam. It then plays sound 0x19E panned from the slot's position. From frame 0x3C the
// slot is retired. frameCount increments every frame.
// FUNCTION: FF7 0x0056020d
void __cdecl BattleFx_BlueChocobuckleSpawnOrbitSpritesTask(void)
{
    int spriteIndex;
    int z; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int baseScale;
    BattleFxChocobuckleOrbitSprite *pSprite;
    BattleFxChocobuckleSpawnSlot *spawner;

    spawner = (BattleFxChocobuckleSpawnSlot *)BATTLE_CURRENT_TASK_SLOT;
    if (spawner->frameCount == 0x11) {
        baseScale = 0xAAA;
        for (spriteIndex = 0; spriteIndex < 8; spriteIndex++) {
            pSprite = (BattleFxChocobuckleOrbitSprite *)BATTLE_NEW_TASK_SLOT(BattleFx_BlueChocobuckleOrbitSpriteTask);
            pSprite->pos = spawner->pos;
            pSprite->angle = spriteIndex << 9;
            pSprite->angleStep = 0x55;
            pSprite->scale = baseScale;
            pSprite->orbitRadius = spawner->orbitRadius;
        }
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputeScreenPanFromPosition(&spawner->pos), 0x19E);
    }
    if (spawner->frameCount >= 0x3C) {
        spawner->ownerSlot = BATTLE_TASK_FREE;
    }
    spawner->frameCount += 1;
}

// Per-frame update and draw of one orbiting sprite of the blue Chyobo effect. Its rotation matrix
// comes from the slot's angle, and its position is the vector (0, 0, orbitRadius) rotated by that matrix
// and advanced by stepX, stepY and stepZ. The matrix is scaled by scale, multiplied by
// g_BattleCameraTransform and set as the current transform. The colour level ramps up over frames 0
// to 7, holds at 0x68 through frame 0x14 and ramps down from frame 0x15, and the sprite is
// submitted to the draw list. The slot retires at frame 0x1D or later. The code after the
// g_BattleIsPaused check runs only while the battle is not paused.
// FUNCTION: FF7 0x005602fc
void __cdecl BattleFx_BlueChocobuckleOrbitSpriteTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[72];
        int rotMatrix;
        struct { unsigned char pad_to_12[0x12]; GfxDirectionVec3 posX; };
        struct { unsigned char pad_to_20[0x20]; BattleFxChocobuckleOrbitSpriteSlot *orbitSlot; };
        struct { unsigned char pad_to_24[0x24]; SVECTOR vecX; };
        struct { unsigned char pad_to_2C[0x2C]; SVECTOR rotX; };
        struct { unsigned char pad_to_34[0x34]; VECTOR scale; };
        struct { unsigned char pad_to_44[0x44]; char colorLevel; };
    } frame;
#pragma pack(pop)

    frame.orbitSlot = (BattleFxChocobuckleOrbitSpriteSlot *)BATTLE_CURRENT_TASK_SLOT;
    frame.rotX.vx = 0;
    frame.rotX.vy = frame.orbitSlot->angle;
    frame.rotX.vz = 0;
    RotMatrixXYZ(&frame.rotX, &frame.rotMatrix);
    frame.vecX.vx = 0;
    frame.vecX.vy = 0;
    frame.vecX.vz = frame.orbitSlot->orbitRadius;
    ApplyMatrixToSVectorInt32(&frame.rotMatrix, &frame.vecX, &frame.posX);
    frame.posX.x += frame.orbitSlot->stepX;
    frame.posX.y += frame.orbitSlot->stepY;
    frame.posX.z += frame.orbitSlot->stepZ;
    frame.scale.vz = frame.orbitSlot->scale;
    frame.scale.vy = frame.scale.vz;
    frame.scale.vx = frame.scale.vy;
    ScaleMatrix(&frame.rotMatrix, &frame.scale);
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.rotMatrix);
    SetRotMatrix(&frame.rotMatrix);
    SetTransMatrix(&frame.rotMatrix);
    if (frame.orbitSlot->frameCounter < 8) {
        frame.colorLevel = frame.orbitSlot->frameCounter * 0xD;
    } else if (frame.orbitSlot->frameCounter >= 0x15) {
        frame.colorLevel = 0x68 - (frame.orbitSlot->frameCounter - 0x15) * 0xD;
    } else {
        frame.colorLevel = 0x68;
    }
    g_BattleBlueChocobuckleColor2.b = frame.colorLevel;
    g_BattleBlueChocobuckleColor2.g = g_BattleBlueChocobuckleColor2.b;
    g_BattleBlueChocobuckleColor2.r = g_BattleBlueChocobuckleColor2.g;
    g_BattleBlueChocobuckleColor1.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(0, g_BattleBlueChocobuckleLoadedRes);
    g_BattleBlueChocobuckleColor1.spriteDrawObj = Gfx_ModelTableGetSpriteObject(0, g_BattleBlueChocobuckleLoadedRes);
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitSpriteDraw(&g_BattleBlueChocobuckleColor1.spriteGroup, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (frame.orbitSlot->frameCounter >= 0x1D) {
        frame.orbitSlot->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.orbitSlot->angle += frame.orbitSlot->angleStep;
    (frame.orbitSlot->frameCounter)++;
}

// Spawns the bouncing sprites of the blue Chyobo effect and advances the slot's timer each frame.
// From timer 0x11 to 0x14 it spawns 18 BattleFx_BlueChocobuckleBouncingSpriteTask slots, each at the
// slot's position with a random horizontal direction, a random distance from baseOffset and a
// random vertical velocity, with scale set to 0xAAA. At timer 0x11 it plays sound 0x19E panned from
// the slot's position. The slot retires at timer 0x3C or later. The sign test on each sprite's
// angle reads e, which this function never assigns.
// FUNCTION: FF7 0x005604e5
void __cdecl BattleFx_BlueChocobuckleSpawnBouncingSpritesTask(void)
{
    int spriteIndex;
    int s;
    VECTOR innerOutVec;
    VECTOR currRefX;
    BattleFxChocobuckleSpriteSlot *spawner;
    VECTOR normXTerm;
    int scaleConst;
    BattleFxBlueChocobuckleBounce *pNewSprite;

    currRefX.vx = 0;
    currRefX.vy = 0;
    currRefX.vz = 0x1000;
    memset(&currRefX.pad, 0, sizeof(currRefX.pad));
    spawner = (BattleFxChocobuckleSpriteSlot *)BATTLE_CURRENT_TASK_SLOT;
    if (spawner->timer >= 0x11 && spawner->timer <= 0x14) {
        scaleConst = 0xAAA;
        for (spriteIndex = 0; spriteIndex < 0x12; spriteIndex++) {
            pNewSprite = (BattleFxBlueChocobuckleBounce *)BATTLE_NEW_TASK_SLOT(BattleFx_BlueChocobuckleBouncingSpriteTask);
            pNewSprite->pos = spawner->pos;
            normXTerm.vx = rand() % 0x1000 - 0x800;
            normXTerm.vy = 0;
            normXTerm.vz = rand() % 0x1000 - 0x800;
            NormalizeVectorFixed(&normXTerm, &normXTerm);
            pNewSprite->angle = ComputeAngleBetweenVectors(&currRefX, &normXTerm, &innerOutVec);
            if (innerOutVec.vy < 0) {
                pNewSprite->angle = -pNewSprite->angle;
            }
            pNewSprite->bounceVelocity = 0;
            s = spawner->baseOffset + rand() % 0x1C0;
            pNewSprite->endX = pNewSprite->startX + (s * normXTerm.vx >> 0xC);
            pNewSprite->velocityY = 0;
            pNewSprite->endZ = pNewSprite->startZ + (s * normXTerm.vz >> 0xC);
            pNewSprite->endX = (pNewSprite->endX - pNewSprite->startX) / 0x10;
            pNewSprite->velocityY = -(rand() % 0x3C + 0xFA);
            pNewSprite->endZ = (pNewSprite->endZ - pNewSprite->startZ) / 0x10;
            pNewSprite->scale = scaleConst;
            pNewSprite->accelY = -pNewSprite->startY / 0x18;
        }
    }
    if (spawner->timer == 0x11) {
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputeScreenPanFromPosition(&spawner->pos), 0x19E);
    }
    if (spawner->timer >= 0x3C) {
        spawner->ownerSlot = BATTLE_TASK_FREE;
    }
    (spawner->timer)++;
}

// Updates and draws one bouncing sprite of the blue Chyobo effect from the current slot of
// g_BattleTaskSlotBuffer.mainSlots. The sprite is positioned at posX, posY and posZ, rotated by
// bounceOffset and scaled by scale, and it is drawn after multiplying by g_BattleCameraTransform.
// Its colour level ramps up over the first six frames, holds at 0x68 and ramps down from frame
// 0x15, and is stored in g_BattleBlueChocobuckleColor3. The slot retires at frame 0x1D or later. After
// the g_BattleIsPaused check, the sprite moves by its velocities, velocityY gains accelY, accelY
// loses a sixteenth of itself, and bounceOffset gains bounceVelocity.
// FUNCTION: FF7 0x00560741
void __cdecl BattleFx_BlueChocobuckleBouncingSpriteTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[72];
        PsxMatrix rotMatrix;
        struct { unsigned char pad_to_28[0x28]; BattleFxChocobuckleBouncingSpriteSlot *bounceSlot; };
        struct { unsigned char pad_to_2C[0x2C]; SVECTOR rotX; };
        struct { unsigned char pad_to_34[0x34]; VECTOR scale; };
        struct { unsigned char pad_to_44[0x44]; char colorLevel; };
    } frame;
#pragma pack(pop)

    frame.bounceSlot = (BattleFxChocobuckleBouncingSpriteSlot *)BATTLE_CURRENT_TASK_SLOT;
    frame.rotX.vx = 0;
    frame.rotX.vy = frame.bounceSlot->bounceOffset;
    frame.rotX.vz = 0;
    RotMatrixXYZ(&frame.rotX, &frame.rotMatrix);
    frame.rotMatrix.t[0] = frame.bounceSlot->posX;
    frame.rotMatrix.t[1] = frame.bounceSlot->posY;
    frame.rotMatrix.t[2] = frame.bounceSlot->posZ;
    frame.scale.vz = frame.bounceSlot->scale;
    frame.scale.vy = frame.scale.vz;
    frame.scale.vx = frame.scale.vy;
    ScaleMatrix(&frame.rotMatrix, &frame.scale);
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.rotMatrix);
    SetRotMatrix(&frame.rotMatrix);
    SetTransMatrix(&frame.rotMatrix);
    if (frame.bounceSlot->frameCounter < 6) {
        frame.colorLevel = frame.bounceSlot->frameCounter * 0x11;
    } else if (frame.bounceSlot->frameCounter >= 0x15) {
        frame.colorLevel = 0x68 - (frame.bounceSlot->frameCounter - 0x15) * 0xD;
    } else {
        frame.colorLevel = 0x68;
    }
    g_BattleBlueChocobuckleColor3.b = frame.colorLevel;
    g_BattleBlueChocobuckleColor3.g = g_BattleBlueChocobuckleColor3.b;
    g_BattleBlueChocobuckleColor3.r = g_BattleBlueChocobuckleColor3.g;
    g_BattleBlueChocobuckleColor2.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(0, g_BattleBlueChocobuckleLoadedRes);
    g_BattleBlueChocobuckleColor2.spriteDrawObj = Gfx_ModelTableGetSpriteObject(0, g_BattleBlueChocobuckleLoadedRes);
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitSpriteDraw(&g_BattleBlueChocobuckleColor2.spriteGroup, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (frame.bounceSlot->frameCounter >= 0x1D) {
        frame.bounceSlot->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.bounceSlot->posX += frame.bounceSlot->velocityX;
    frame.bounceSlot->posY += frame.bounceSlot->velocityY;
    frame.bounceSlot->posZ += frame.bounceSlot->velocityZ;
    frame.bounceSlot->velocityY += frame.bounceSlot->accelY;
    frame.bounceSlot->accelY -= (short)(frame.bounceSlot->accelY >> 4);
    frame.bounceSlot->bounceOffset += frame.bounceSlot->bounceVelocity;
    if (frame.bounceSlot->posY >= 0) {
        frame.bounceSlot->velocityY = -(rand() % 0x50 + 0xFA);
        frame.bounceSlot->accelY = (int)frame.bounceSlot->accelY + rand() % 0x32;
        frame.bounceSlot->bounceVelocity = rand() % 0x30 + 0x10;
        if (frame.bounceSlot->bounceOffset < 0) {
            frame.bounceSlot->bounceVelocity = -frame.bounceSlot->bounceVelocity;
        }
    }
    (frame.bounceSlot->frameCounter)++;
}

/*
 * Battle setup task for the alternate Chocobuckle/Bad Breath confusion effect across masked targets.
 */
// FUNCTION: FF7 0x005609db
void __cdecl BattleFx_BlueChocobuckleAltSetupTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[72];
        BattleFxChocobuckleAltTimelineSlot *timelineSlot;
        struct { unsigned char pad_to_4[0x4]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_24[0x24]; BattleTaskSlot *activeSlot; };
        struct { unsigned char pad_to_2C[0x2C]; int targetCount; };
        struct { unsigned char pad_to_30[0x30]; int bitIndex; };
        struct { unsigned char pad_to_38[0x38]; SVECTOR rotX; };
        struct { unsigned char pad_to_40[0x40]; int altBonePos; };
        struct { unsigned char pad_to_42[0x42]; short altBoneY; };
    } frame;
#pragma pack(pop)

    frame.bitIndex = 0;
    for (frame.targetCount = 0; frame.bitIndex < 0xA; frame.bitIndex++) {
        if ((g_BattleBlueChocobuckleTargetMask & 1 << frame.bitIndex) != 0) {
            frame.targetCount++;
            if (frame.targetCount == 1) {
                g_BattleBlueChocobuckleActorIdx = frame.bitIndex;
            }
        }
    }
    frame.activeSlot = BATTLE_CURRENT_TASK_SLOT;
    frame.activeSlot->ownerSlot = BATTLE_TASK_FREE;
    frame.timelineSlot = (BattleFxChocobuckleAltTimelineSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_BlueChocobuckleAltTimelineTask);
    Battle_GetBoneWorldPos(g_BattleBlueChocobuckleOriginActor, g_BattleActorRecords[g_BattleBlueChocobuckleOriginActor].targetBone, &frame.timelineSlot->posX);
    Battle_GetBoneWorldPos(g_BattleBlueChocobuckleOriginActor, g_BattleActorRecords[g_BattleBlueChocobuckleOriginActor].altBone, (short *)&frame.altBonePos);
    frame.timelineSlot->posY = frame.altBoneY - 0x1C0;
    frame.timelineSlot->posY -= 0x2A0;
    frame.timelineSlot->yaw = g_BattleActorRecords[g_BattleBlueChocobuckleOriginActor].yaw;
    frame.timelineSlot->value = 0x460;
    frame.timelineSlot->posYStep = 0x2A;
    frame.timelineSlot->state = BLUE_CHYOBO_STATE_START;
    frame.timelineSlot->frameCount = 0x63;
    Battle_GetBoneWorldPos(g_BattleBlueChocobuckleActorIdx, g_BattleActorRecords[g_BattleBlueChocobuckleActorIdx].altBone, &g_BattleBlueChocobuckleTargetBonePos.posX);
    if (g_BattleBlueChocobuckleTargetBonePos.posY >= 0) {
        g_BattleBlueChocobuckleTargetBonePos.posY = 0xFF9C;
    }
    if (g_BattleBlueChocobuckleOriginActor != g_BattleBlueChocobuckleActorIdx) {
        g_BattleBlueChocobuckleDeltaX = g_BattleBlueChocobuckleTargetBonePos.posX - frame.timelineSlot->posX;
        g_BattleBlueChocobuckleDeltaY = 0;
        g_BattleBlueChocobuckleDeltaZ = g_BattleBlueChocobuckleTargetBonePos.posZ - frame.timelineSlot->posZ;
        NormalizeVectorFixed(&g_BattleBlueChocobuckleDeltaX, &g_BattleBlueChocobuckleDeltaX);
    } else {
        frame.rotX.vx = 0;
        frame.rotX.vy = frame.timelineSlot->yaw;
        frame.rotX.vz = 0;
        RotMatrixXYZ(&frame.rotX, &frame.rotMatrix);
        TransformVectorToInt32(&frame.rotMatrix, g_BattleBlueChocobuckleParamVector1, &g_BattleBlueChocobuckleDeltaX);
    }
    g_BattleBlueChocobuckleScaleFactor = 0x5A;
    frame.rotX.vx = 0;
    frame.rotX.vy = frame.timelineSlot->yaw;
    frame.rotX.vz = 0;
    RotMatrixXYZ(&frame.rotX, &frame.rotMatrix);
    TransformVectorToInt32(&frame.rotMatrix, g_BattleBlueChocobuckleParamVector2, g_BattleBlueChocobuckleParamVector2);
}

// Drives the alternate blue Chyobo timeline of one task slot through BattleFxBlueChocobuckleState.
// START allocates a type-1 ring buffer record, copies the slot's position and yaw into it and
// plays sound 0x1A1 panned to the slot. RISE raises posY by posYStep each frame and, from timer
// 0x11, launches the slot with velocities from the shared scale factor and delta, sets
// accelerationY and plays sound 0x19F. In HOP the arc runs while posY is negative and posY is
// clamped to 0 once it passes; then the slot re-aims at the target bone, stores the direction in
// the shared delta globals and hops again, or, once near enough, leaps at the target (LEAP). On
// landing it bounces off for 20 frames (BOUNCE_OFF), the hit landing on frame 4, and FINISH frees
// the task.
// FUNCTION: FF7 0x00560c32
void __cdecl BattleFx_BlueChocobuckleAltTimelineTask(void)
{
    struct {
        BattleFxBlueChocobuckleAltTrack *track;
        int padZ, padJ, padY, padX, padH, padW, padG, padV;
        BattleFxBlueChocobuckleAltRecord *spawnTask;
        int stepLimit, padE, leapSpeed, distSq, padS, padC, dirX, dirY, dirZ, stepCount;
        BattleFxChocobuckleRingRecord *ringRec;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    frame.track = (BattleFxBlueChocobuckleAltTrack *)BATTLE_CURRENT_TASK_SLOT;
    switch (frame.track->state) {
    case BLUE_CHYOBO_STATE_START:
        frame.ringRec = (BattleFxChocobuckleRingRecord *)Battle_AllocSceneMessage(1);
        frame.ringRec->pos = frame.track->pos;
        frame.ringRec->yaw = frame.track->yaw;
        frame.track->state = BLUE_CHYOBO_STATE_RISE;
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputeScreenPanFromPosition(&frame.track->pos), 0x1A1);
        break;
    case BLUE_CHYOBO_STATE_RISE:
        frame.track->posY += frame.track->posYStep;
        if (frame.track->timer >= 0x11) {
            frame.track->velocityX = g_BattleBlueChocobuckleScaleFactor * g_BattleBlueChocobuckleDeltaX >> 0xC;
            frame.track->velocityY = 0xFED4;
            frame.track->velocityZ = g_BattleBlueChocobuckleScaleFactor * g_BattleBlueChocobuckleDeltaZ >> 0xC;
            frame.track->accelerationY = 0x44;
            g_BattleBlueChocobuckleScaleFactor += 0x14;
            frame.track->state = BLUE_CHYOBO_STATE_HOP;
            Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputeScreenPanFromPosition(&frame.track->pos), 0x19F);
        }
        break;
    case BLUE_CHYOBO_STATE_HOP:
        if (frame.track->posY < 0) {
            frame.track->posX += frame.track->velocityX;
            frame.track->posY += frame.track->velocityY;
            frame.track->posZ += frame.track->velocityZ;
            frame.track->velocityY += frame.track->accelerationY;
            frame.track->accelerationY -= (short)(frame.track->accelerationY >> 4);
            if (frame.track->posY > 0) {
                frame.track->posY = 0;
            }
        } else {
            if (g_BattleBlueChocobuckleActorIdx == g_BattleBlueChocobuckleOriginActor) {
                Battle_GetBoneWorldPos(g_BattleBlueChocobuckleActorIdx, g_BattleActorRecords[g_BattleBlueChocobuckleActorIdx].altBone, &g_BattleBlueChocobuckleTargetBonePos.posX);
            }
            frame.stepLimit = BattleFx_BlueChocobuckleCountStepsToTarget(0, 0xFFFFFE84, 0x60, 0);
            frame.stepLimit *= g_BattleBlueChocobuckleScaleFactor << 1;
            frame.stepLimit *= frame.stepLimit;
            frame.dirX = g_BattleBlueChocobuckleTargetBonePos.posX - frame.track->posX;
            frame.dirY = 0;
            frame.dirZ = g_BattleBlueChocobuckleTargetBonePos.posZ - frame.track->posZ;
            frame.distSq = NormalizeVectorFixed((int *)&frame.dirX, &frame.dirX);
            g_BattleBlueChocobuckleDeltaX = frame.dirX;
            g_BattleBlueChocobuckleDeltaY = frame.dirY;
            g_BattleBlueChocobuckleDeltaZ = frame.dirZ;
            g_BattleBlueChocobuckleStepCount = frame.stepCount;
            if (frame.distSq > frame.stepLimit) {
                frame.track->velocityX = g_BattleBlueChocobuckleScaleFactor * frame.dirX >> 0xC;
                frame.track->velocityY = 0xFE84;
                frame.track->velocityZ = g_BattleBlueChocobuckleScaleFactor * frame.dirZ >> 0xC;
                frame.track->accelerationY = 0x60;
                frame.track->posY += frame.track->velocityY;
                frame.track->velocityY += frame.track->accelerationY;
                g_BattleBlueChocobuckleScaleFactor += 0x14;
                Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputeScreenPanFromPosition(&frame.track->pos), 0x99);
            } else {
                frame.stepLimit = BattleFx_BlueChocobuckleCountStepsToTarget(0, 0xFFFFF8F8, 0xDC, g_BattleBlueChocobuckleTargetBonePos.posY);
                if (frame.stepLimit == 0) {
                    frame.stepLimit = 1;
                }
                frame.leapSpeed = SqrtToInt(frame.distSq) / frame.stepLimit;
                if (frame.leapSpeed <= 0) {
                    frame.distSq = 2;
                }
                frame.track->velocityX = frame.leapSpeed * frame.dirX >> 0xC;
                frame.track->velocityY = 0xF8F8;
                frame.track->velocityZ = frame.leapSpeed * frame.dirZ >> 0xC;
                frame.track->accelerationY = 0xDC;
                frame.track->posY += frame.track->velocityY;
                frame.track->velocityY += frame.track->accelerationY;
                frame.track->state = BLUE_CHYOBO_STATE_LEAP;
                Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputeScreenPanFromPosition(&frame.track->pos), 0x1A9);
            }
        }
        break;
    case BLUE_CHYOBO_STATE_LEAP:
        if (frame.track->posY < g_BattleBlueChocobuckleTargetBonePos.posY || frame.track->velocityY < 0) {
            frame.track->posX += frame.track->velocityX;
            frame.track->posY += frame.track->velocityY;
            frame.track->posZ += frame.track->velocityZ;
            frame.track->velocityY += frame.track->accelerationY;
            frame.track->accelerationY -= (short)(frame.track->accelerationY >> 4);
            if (frame.track->posY > g_BattleBlueChocobuckleTargetBonePos.posY) {
                frame.track->posY = g_BattleBlueChocobuckleTargetBonePos.posY;
            }
        } else {
            g_BattleBlueChocobuckleJitterY = rand() % 0x42 + 0x84;
            frame.distSq = g_BattleBlueChocobuckleDeltaX * g_BattleBlueChocobuckleParamVector2[0] + (g_BattleBlueChocobuckleDeltaY + g_BattleBlueChocobuckleParamVector2[1]) + g_BattleBlueChocobuckleDeltaZ * g_BattleBlueChocobuckleParamVector2[2];
            if (frame.distSq < 0) {
                g_BattleBlueChocobuckleJitterY = -g_BattleBlueChocobuckleJitterY;
            }
            g_BattleBlueChocobuckleJitterX = (rand() % 0x18 + 0x30) * (rand() % 2 - 1);
            frame.distSq = rand() % 0x50 + 0xA0;
            frame.track->velocityX = frame.distSq * g_BattleBlueChocobuckleDeltaX >> 0xC;
            frame.track->velocityY = -(rand() % 0x78 + 0x190);
            frame.track->velocityZ = frame.distSq * g_BattleBlueChocobuckleDeltaZ >> 0xC;
            frame.track->accelerationY = rand() % 0x28 + 0x46;
            frame.track->posY += frame.track->velocityY;
            frame.track->velocityY += frame.track->accelerationY;
            frame.track->state = BLUE_CHYOBO_STATE_BOUNCE_OFF;
            frame.track->frameCount = 0;
        }
        break;
    case BLUE_CHYOBO_STATE_BOUNCE_OFF:
        if (frame.track->frameCount < 0x14) {
            frame.track->posX += frame.track->velocityX;
            frame.track->posY += frame.track->velocityY;
            frame.track->posZ += frame.track->velocityZ;
            frame.track->velocityY += frame.track->accelerationY;
            frame.track->accelerationY -= (short)(frame.track->accelerationY >> 4);
            if (frame.track->posY > -160) {
                frame.track->posY = 0xFF60;
            }
        } else {
            frame.track->state = BLUE_CHYOBO_STATE_FINISH;
        }
        if (frame.track->frameCount == 4) {
            Battle_AllocSceneMessage(2);
            Battle_PostActorHitOrHurtMessage(g_BattleBlueChocobuckleActorIdx);
        }
        if (frame.track->posY >= -160) {
            frame.track->velocityY = -(rand() % 0x32 + 0xFA);
            frame.track->accelerationY = rand() % 0x1E + 0x32;
            frame.track->posY += frame.track->velocityY;
            frame.track->velocityY += frame.track->accelerationY;
        }
        (frame.track->frameCount)++;
        break;
    case BLUE_CHYOBO_STATE_FINISH:
        frame.track->state = BLUE_CHYOBO_STATE_DONE;
        frame.track->ownerSlot = BATTLE_TASK_FREE;
    }
    *(Int64Pair *)(&g_BattleActorRecords[3].posX) = frame.track->pos;
    if (frame.track->frameCount < 0x14) {
        g_BattleActorRecords[3].rotX += g_BattleBlueChocobuckleJitterY;
        g_BattleActorRecords[3].rotZ += g_BattleBlueChocobuckleJitterX;
    }
    if (frame.track->posY >= 0) {
        frame.spawnTask = (BattleFxBlueChocobuckleAltRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_BlueChocobuckleSpawnScatterSpritesTask);
        frame.spawnTask->pos = frame.track->pos;
        frame.spawnTask->value = frame.track->value;
        frame.spawnTask->stepRate = 0x1555;
    }
    if (frame.track->frameCount == 0) {
        frame.spawnTask = (BattleFxBlueChocobuckleAltRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_BlueChocobuckleSpawnBouncingBillboardSpritesTask);
        frame.spawnTask->pos = frame.track->pos;
        frame.spawnTask->value = frame.track->value;
        frame.spawnTask->stepRate = 0xAAA;
    }
    (frame.track->timer)++;
}

// Spawns the scatter sprites of a blue Chyobo effect. Until timer reaches 1 the task only counts
// up. Then each frame allocates up to 0x16 sprite tasks, stopping early once the secondary task
// count passes 0x5F. Each sprite copies the spawner's position, lowers posY by 0x3C, takes
// a random horizontal direction normalized to a fixed-point vector, and gets its speed from
// baseOffset plus a random spread. The spawner is then killed (ownerSlot BATTLE_TASK_FREE) and timer
// increments. The frame's first slot is reserved but never read, as in the original.
// FUNCTION: FF7 0x0056149a
void __cdecl BattleFx_BlueChocobuckleSpawnScatterSpritesTask(void)
{
    VECTOR nx;
    int baseScale;
    int i;
    int pace;
    BattleFxBlueChocobuckleSpawnTask *pSprite;
    BattleFxChocobuckleScatterSprite *spawner;

    spawner = (BattleFxChocobuckleScatterSprite *)BATTLE_CURRENT_TASK_SLOT;
    if (spawner->timer < 1) {
        (spawner->timer)++;
    } else {
        baseScale = spawner->scaleBase;
        for (i = 0; i < 0x16; i++) {
            if (g_BattleSecondaryTaskActiveCount > 0x5F) {
                break;
            }
            pSprite = (BattleFxBlueChocobuckleSpawnTask *)BATTLE_NEW_TASK_SLOT(BattleFx_BlueChocobuckleScatterSpriteTask);
            pSprite->pos = spawner->pos;
            pSprite->posY -= 0x3C;
            nx.vx = ((rand()) % 4096) - 0x800;
            nx.vy = 0;
            nx.vz = ((rand()) % 4096) - 0x800;
            NormalizeVectorFixed(&nx, &nx);
            pace = spawner->baseOffset + rand() % 0x1C0;
            pSprite->velocityX = (pace * nx.vx >> 0xC) / 0xC;
            pSprite->velocityY = 0;
            pSprite->velocityZ = (pace * nx.vz >> 0xC) / 0xC;
            pace = ((rand()) % 2048) + 0xA00;
            pSprite->scale = baseScale * pace >> 0xC;
        }
        spawner->ownerSlot = BATTLE_TASK_FREE;
        (spawner->timer)++;
    }
}

// Draws one frame of a blue Chyobo scatter sprite and advances it. The frame counter is copied into
// the sprite request, and the record is billboarded from its 4.12 scale with a forward offset of
// minus scale shifted right 3. Slot 1 of the loaded resource goes into the request, and the quad is
// submitted into the depth-sorted ordering table with shift 0xC on the shared packet cursor. The
// record is killed once frameCount reaches 11, even while paused. Unless paused, posX and posZ
// advance by velocityX and velocityZ, and frameCount increments.
// FUNCTION: FF7 0x0056163e
void __cdecl BattleFx_BlueChocobuckleScatterSpriteTask(void)
{
    BattleFxBlueScatterSprite *task;

    task = (BattleFxBlueScatterSprite *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleBlueChocobuckleSpriteDrawReq.frameCount = task->frameCount;
    BattleFx_InstallBillboardMatrix(&task->posX, (short)task->scale, -(task->scale >> 3));
    g_BattleBlueChocobuckleSpriteDrawReq.scatterHandle = Gfx_ModelTableGetLoadedRecord(1, g_BattleBlueChocobuckleLoadedRes);
    g_BattleBlueChocobuckleSpriteDrawReq.scatterResource = Gfx_ModelTableGetSpriteObject(1, g_BattleBlueChocobuckleLoadedRes);
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitSpriteDraw(&g_BattleBlueChocobuckleSpriteDrawReq, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (task->frameCount >= 0xB) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    task->posX += task->velocityX;
    task->posZ += task->velocityZ;
    (task->frameCount)++;
}

// Spawns the bouncing billboard sprites of a blue Chyobo effect. Until bounceCount reaches 1 the
// task only counts up. Then each frame allocates up to 0x18 sprite tasks, stopping early once the
// secondary task count passes 0x5F. Each sprite copies the spawner's position and takes a
// random horizontal direction. Its speed comes from speedBase with a random spread, and its scale
// comes from the spawner's scaleBase scaled by a random factor. It also gets a random upward velocityY, a variant from
// 0 to 3, and a random timer. The spawner is then killed (ownerSlot BATTLE_TASK_FREE) and bounceCount
// increments. The frame's first slot is reserved but never read, as in the original.
// FUNCTION: FF7 0x00561737
void __cdecl BattleFx_BlueChocobuckleSpawnBouncingBillboardSpritesTask(void)
{
    VECTOR nx;
    int baseScale;
    int i;
    int pace;
    BattleFxChocobuckleBillboardSprite *pSprite;
    BattleFxChocobuckleBillboardSpawner *spawner;

    spawner = (BattleFxChocobuckleBillboardSpawner *)BATTLE_CURRENT_TASK_SLOT;
    if (spawner->bounceCount < 1) {
        (spawner->bounceCount)++;
    } else {
        baseScale = spawner->scaleBase;
        for (i = 0; i < 0x18; i++) {
            if (g_BattleSecondaryTaskActiveCount > 0x5F) {
                break;
            }
            pSprite = (BattleFxChocobuckleBillboardSprite *)BATTLE_NEW_TASK_SLOT(BattleFx_BlueChocobuckleBouncingBillboardSpriteTask);
            pSprite->pos = spawner->pos;
            nx.vx = ((rand()) % 4096) - 0x800;
            nx.vy = 0;
            nx.vz = ((rand()) % 4096) - 0x800;
            NormalizeVectorFixed(&nx, &nx);
            pace = spawner->speedBase + rand() % 0x2A0 + 0xE0;
            pSprite->velocityX = (pace * nx.vx >> 0xC) / 0xA;
            pSprite->velocityY = -(rand() % 0x17C + 0x3C);
            pSprite->velocityZ = (pace * nx.vz >> 0xC) / 0xA;
            pace = rand() % 0xC00 + 0xA00;
            pSprite->scale = baseScale * pace >> 0xC;
            pSprite->phaseSlot = ((rand()) % 4);
            pSprite->gravity = rand() % 0x48 + 0x1C;
        }
        spawner->ownerSlot = BATTLE_TASK_FREE;
        (spawner->bounceCount)++;
    }
}

/*
 * Battle task that animates and fades a bouncing confusion billboard sprite above its target.
 */
// FUNCTION: FF7 0x00561908
void __cdecl BattleFx_BlueChocobuckleBouncingBillboardSpriteTask(void)
{
    unsigned char colorByte;
    BattleFxBillboardBounce *pSprite;

    pSprite = (BattleFxBillboardBounce *)BATTLE_CURRENT_TASK_SLOT;
    BattleFx_InstallBillboardMatrix(&pSprite->posX, (short)pSprite->scale, -(pSprite->scale >> 3));
    if (pSprite->phase < 6) {
        colorByte = pSprite->phase * 0x11;
    } else if (pSprite->phase >= 0xC) {
        colorByte = 0x68 - (pSprite->phase - 0xC) * 0xD;
    } else {
        colorByte = 0x68;
    }
    if (pSprite->phase % 4 == pSprite->phaseSlot) {
        colorByte <<= 1;
    }
    g_BattleBlueChocobuckleColor1.b = colorByte;
    g_BattleBlueChocobuckleColor1.g = g_BattleBlueChocobuckleColor1.b;
    g_BattleBlueChocobuckleColor1.r = g_BattleBlueChocobuckleColor1.g;
    g_BattleBlueChocobuckleSpriteDrawReq.billboardHandle = Gfx_ModelTableGetLoadedRecord(0, g_BattleBlueChocobuckleLoadedRes);
    g_BattleBlueChocobuckleSpriteDrawReq.billboardResource = Gfx_ModelTableGetSpriteObject(0, g_BattleBlueChocobuckleLoadedRes);
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitSpriteDraw(&g_BattleBlueChocobuckleSpriteDrawReq.billboardHandle, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (pSprite->phase >= 0x14) {
        pSprite->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    pSprite->posX += pSprite->velocityX;
    pSprite->posY += pSprite->velocityY;
    pSprite->posZ += pSprite->velocityZ;
    pSprite->velocityY += pSprite->gravity;
    pSprite->gravity -= (short)(pSprite->gravity >> 4);
    pSprite->angle += pSprite->angleStep;
    if (pSprite->posY >= 0) {
        pSprite->velocityY = -(rand() % 0x12C + 0x28);
        pSprite->gravity = pSprite->gravity + rand() % 0x32;
    }
    (pSprite->phase)++;
}

/*
 * Calculates step count and delta components along trajectory to target.
 */
// FUNCTION: FF7 0x00561b1f
int __cdecl BattleFx_BlueChocobuckleCountStepsToTarget(int position, int velocity, int accel, int target)
{
    int steps;

    position += velocity;
    velocity += accel;
    for (steps = 0; steps < 0x63; steps++) {
        position += velocity;
        velocity += accel;
        accel -= accel >> 4;
        if (position >= target && velocity > 0) {
            break;
        }
    }
    return steps;
}
} /* extern "C" */
