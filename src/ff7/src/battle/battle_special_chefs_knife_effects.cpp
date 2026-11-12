// src/ff7/src/battle/battle_special_chefs_knife_effects.cpp
/* Enemy Special Attack [120]: Chef's Knife (orig. special/korosi, 殺し) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle special effect for the Korosi spell. It loads its resources, starts the
//   timeline, and runs the pulse-fade and animated sprite tasks.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/enums/sound.h"
#include "ff7/api/battle.h"
#include "ff7/battle_tasks.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_actors.h"
#include "lib/types/graphics.h"
#include "ff7/camera_script.h"
#include <string.h>
#include <stdlib.h>
#include "lib/codegen.h"
extern "C" {


// clang-format off
// GLOBAL: FF7 0x0089a9f8
int g_BattleSpecChefsKnifeResourceDef = 0x00192A39;
int g_BattleSpecChefsKnifeResourceDefHeadPtr[1] = {
    (int)((char *)&g_BattleSpecialChefsKnifeEffectsResourceNames + 0x0)
};
// GLOBAL: FF7 0x0089aa00
int g_BattleSpecChefsKnifeSpriteDef[9] = {
    (int)((char *)&g_BattleSpecialChefsKnifeEffectsResourceNames + 0xc), 0x0, 0x0, 0x192a39, (int)((char *)&g_BattleSpecialChefsKnifeEffectsResourceNames + 0x14), (int)((char *)&g_BattleSpecialChefsKnifeEffectsResourceNames + 0x20),
    0x0, 0x0, 0x0
};
unsigned char g_rawData_0089aa28[8] = {0x00, 0xf0, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00};
// The three spawn directions are 16-byte vectors (x, y, z, pad) from g_BattleSpecChefsKnifeSpawnDir0X
// on: the first is (0x80, -0xC06, 0, 0) and the other two are members of g_BattleSpecChefsKnifeAnimData.
// GLOBAL: FF7 0x0089aa30
int g_BattleSpecChefsKnifeSpawnDir0X = 128;
// GLOBAL: FF7 0x0089aa34
int g_BattleSpecChefsKnifeSpawnDir0Y = -3078;
// GLOBAL: FF7 0x0089aa38
struct BattleSpecChefsKnifeAnimData g_BattleSpecChefsKnifeAnimData = {0, 0, {-0x800, 0x800, 0, 0}, {0x708, 0x200, 0, 0}, 0, 0};
// GLOBAL: FF7 0x0089aa68
BattleSpecChefsKnifeSpriteRequest g_BattleSpecChefsKnifeSpriteRequest = {-128, -128, -128, 0x2e, 0, 0, {0, 0, -128, -128, -128, 0x2e, 0, 0}};
unsigned char g_BattleSpecialChefsKnifeEffectsResourceNames[44] =
    "HIT_1.TIM\0\0\0"
    "HIT.SPT\0"
    "JODETH1.TIM\0"
    "JODETH1.SPT";
// GLOBAL: FF7 0x00b5a7a0
int g_BattleSpecChefsKnifeTargetActor = 0;
// GLOBAL: FF7 0x00b5a7a4
int g_BattleSpecChefsKnifeMatrixPending = 0;
// The sprites' rotation matrix (a PsxMatrix) starts here and runs through the next two pads.
// GLOBAL: FF7 0x00b5a7a8
PsxMatrix g_BattleSpecChefsKnifeSpriteMatrix = {0};
// GLOBAL: FF7 0x00b5a7c8
struct GfxTableCursor *g_BattleSpecChefsKnifeTaskPtr = 0;
unsigned char g_bssPad_00b5a7cc[4] = {0};
unsigned char g_bssPad_00b5a7d0[8] = {0};
// clang-format on

#pragma intrinsic(memset)
#pragma intrinsic(memcpy)


/*
 * When the engine context exists and g_BattleSpecChefsKnifeTaskPtr is empty, builds special/korosi with a default-
 * duration record and creates resource type 2 using g_BattleSpecChefsKnifeResourceDef.
 */
// FUNCTION: FF7 0x005190f0
void __cdecl BattleFx_SpecialLoadResChefsKnife(void)
{
    GfxModelTableBank *ownerHandle;
    GfxResourceRequest recBlock;
    int kind;
    char pathDstBuf[200];
    GfxResourceRequest *pRequest;
    int resourceDefAddr;

    if (g_pBattle3dScene != 0 && g_BattleSpecChefsKnifeTaskPtr == 0) {
        kind = 2;
        resourceDefAddr = (int)&g_BattleSpecChefsKnifeResourceDef;
        ownerHandle = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRequest = &recBlock;
        Gfx_InitResourceRequest(pRequest);
        pRequest->groupTag = 0;
        // STRING: FF7 0x0089aaac
        pRequest->resourcePath = (char *)BattleFx_BuildResourcePath("special/korosi", pathDstBuf);
        Gfx_LoadModelTableIfAbsent(kind, resourceDefAddr, pRequest, &g_BattleSpecChefsKnifeTaskPtr, ownerHandle);
    }
}

/*
 * Entry point for the Korosi special effect: loads its resources via
 * BattleFx_SpecialLoadResChefsKnife, then forwards actorMask and sourceIndex to BattleFx_SpecialBeginChefsKnife. It touches no
 * globals of its own.
 */
// FUNCTION: FF7 0x00519180
void __cdecl BattleFx_SpecialEnterChefsKnife(int targetMask, int actorIdx)
{
    BattleFx_SpecialLoadResChefsKnife();
    BattleFx_SpecialBeginChefsKnife(targetMask, actorIdx);
}

/*
 * Allocates BattleFx_SelectDrawListBuffer and stores its slot address, records the first actor set
 * among the low ten bits of actorMask as the target, allocates BattleFx_SpecialChefsKnifeSpawnTimelineTask,
 * and asks for the sprites' rotation matrix to be built.
 */
// FUNCTION: FF7 0x0051919a
void __cdecl BattleFx_SpecialBeginChefsKnife(int actorMask, int actorIdx)
{
    int bitIdx, setCount;

    g_BattleFxDrawListBufferTaskSlotPtr = (unsigned char *)BATTLE_NEW_TASK_SLOT(BattleFx_SelectDrawListBuffer);
    bitIdx = 0;
    for (setCount = 0; bitIdx < 0xA; bitIdx++) {
        if ((actorMask & 1 << bitIdx) != 0) {
            setCount++;
            if (setCount == 1) {
                g_BattleSpecChefsKnifeTargetActor = bitIdx;
            }
        }
    }
    Battle_AllocateResourceSlot(BattleFx_SpecialChefsKnifeSpawnTimelineTask);
    g_BattleSpecChefsKnifeMatrixPending = 1;
}

// Retires its own task slot, then allocates a slot running BattleFx_SpecialChefsKnifeTimelineTask and
// fills it. Its position (posX, posY, posZ) is the target bone of actor g_BattleSpecChefsKnifeTargetActor,
// from Battle_GetBoneWorldPos. effectRadius is that actor's radius plus 0xE0, clamped to 0x4F2, and
// doubleRadius is twice that. velocity is minus half of posY, doubledVelocity is twice velocity, and
// spawnIndex starts at zero. Finally the three spawn directions from g_BattleSpecChefsKnifeSpawnDir0X
// are normalised in place.
// FUNCTION: FF7 0x00519224
void __cdecl BattleFx_SpecialChefsKnifeSpawnTimelineTask(void)
{
    int size, vectorIdx;
    BattleFxChefsKnifeTimelineSlot *task;

    task = (BattleFxChefsKnifeTimelineSlot *)BATTLE_CURRENT_TASK_SLOT;
    task->ownerSlot = BATTLE_TASK_FREE;
    task = (BattleFxChefsKnifeTimelineSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialChefsKnifeTimelineTask);
    Battle_GetBoneWorldPos(g_BattleSpecChefsKnifeTargetActor, g_BattleActorRecords[g_BattleSpecChefsKnifeTargetActor].targetBone, &task->posX);
    size = g_BattleActorRecords[g_BattleSpecChefsKnifeTargetActor].radius + 224;
    if (size > 1266) {
        size = 1266;
    }
    task->effectRadius = size;
    task->doubleRadius = size << 1;
    task->velocity = -(task->posY >> 1);
    task->doubledVelocity = task->velocity << 1;
    task->spawnIndex = 0;
    for (vectorIdx = 0; vectorIdx < 3; vectorIdx++) {
        NormalizeVectorFixed((vectorIdx << 4) + (char *)&g_BattleSpecChefsKnifeSpawnDir0X, (int *)((vectorIdx << 4) + (char *)&g_BattleSpecChefsKnifeSpawnDir0X));
    }
}

// Spawns an animated sprite on frames 2, 6 and 9, each offset along the next spawn direction and
// turned to face the camera, and the pulse sprite on frame 0xC. On frame 0x23 it starts the target's
// reaction; it frees itself at frame 0x28.
// FUNCTION: FF7 0x00519336
void __cdecl BattleFx_SpecialChefsKnifeTimelineTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[76];
        BattleFxChefsKnifeTimelineSlot *korosiSlot;
        struct { unsigned char pad_to_4[0x4]; BattleFxChefsKnifeAnimatedSprite *korosiSprite; };
        struct { unsigned char pad_to_8[0x8]; SVECTOR offsetX; };
        struct { unsigned char pad_to_10[0x10]; VECTOR toCameraX; };
        struct { unsigned char pad_to_20[0x20]; int baseScale; };
        struct { unsigned char pad_to_24[0x24]; int tmpRandom; };
        struct { unsigned char pad_to_28[0x28]; VECTOR rotAxis; };
        struct { unsigned char pad_to_38[0x38]; VECTOR toCameraUnit; };
        struct { unsigned char pad_to_48[0x48]; int angle; };
    } frame;
#pragma pack(pop)

    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.korosiSlot = (BattleFxChefsKnifeTimelineSlot *)BATTLE_CURRENT_TASK_SLOT;
    if (frame.korosiSlot->frameCounter == 2 || (frame.korosiSlot->frameCounter == 6 || frame.korosiSlot->frameCounter == 9)) {
        if (g_BattleSpecChefsKnifeMatrixPending != 0) {
            frame.toCameraX.vx = g_BattleCameraPosition.cameraPosX - frame.korosiSlot->posX;
            frame.toCameraX.vy = 0;
            frame.toCameraX.vz = g_BattleCameraPosZ - frame.korosiSlot->posZ;
            NormalizeVectorFixed(&frame.toCameraX, &frame.toCameraUnit);
            /* The vector (0, 0, -0x1000) from g_BattleSpecChefsKnifeSpriteDef[8] runs over the alignment
               pad into g_rawData_0089aa28. */
            frame.angle = ComputeAngleBetweenVectors((VECTOR *)&g_BattleSpecChefsKnifeSpriteDef[8], &frame.toCameraUnit, &frame.rotAxis);
            Gte_BuildAxisAngleMatrix(frame.angle, (PsxMatrix *)&g_BattleSpecChefsKnifeSpriteMatrix, &frame.rotAxis);
            g_BattleSpecChefsKnifeMatrixPending = 0;
        }
        frame.korosiSprite = (BattleFxChefsKnifeAnimatedSprite *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialChefsKnifeAnimatedSpriteTask);
        *(Int64Pair *)&frame.korosiSprite->posX = *(Int64Pair *)&frame.korosiSlot->posX;
        frame.tmpRandom = rand() % (frame.korosiSlot->effectRadius >> 1) + (frame.korosiSlot->effectRadius >> 1);
        frame.offsetX.vx = *(int *)((char *)&g_BattleSpecChefsKnifeSpawnDir0X + (frame.korosiSlot->spawnIndex << 4)) * frame.tmpRandom >> 12;
        frame.offsetX.vy = *(int *)((char *)&g_BattleSpecChefsKnifeSpawnDir0Y + (frame.korosiSlot->spawnIndex << 4)) * frame.tmpRandom >> 12;
        frame.offsetX.vz = 0;
        TransformVectorToInt16((PsxMatrix *)&g_BattleSpecChefsKnifeSpriteMatrix, &frame.offsetX, &frame.offsetX.vx);
        frame.korosiSprite->posX += frame.offsetX.vx;
        frame.korosiSprite->posY += frame.offsetX.vy;
        frame.korosiSprite->posZ += frame.offsetX.vz;
        frame.tmpRandom = (rand() % 2048) + 2560;
        frame.baseScale = (frame.korosiSlot->effectRadius << 12) / 350;
        frame.korosiSprite->scale = frame.baseScale * frame.tmpRandom >> 12;
        (frame.korosiSlot->spawnIndex)++;
    }
    if (frame.korosiSlot->frameCounter == 2) {
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputeScreenPanFromPosition(&frame.korosiSlot->posX), 0x1A5);
    }
    if (frame.korosiSlot->frameCounter == 12) {
        frame.korosiSprite = (BattleFxChefsKnifeAnimatedSprite *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialChefsKnifePulseFadeSpriteTask);
        *(Int64Pair *)&frame.korosiSprite->posX = *(Int64Pair *)&frame.korosiSlot->posX;
        frame.korosiSprite->scale = (frame.korosiSlot->effectRadius << 12) / 800;
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputeScreenPanFromPosition(&frame.korosiSlot->posX), 0x1A4);
    }
    if (frame.korosiSlot->frameCounter == 35) {
        Battle_PostActorHitOrHurtMessage(g_BattleSpecChefsKnifeTargetActor);
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputeScreenPanFromPosition(&frame.korosiSlot->posX), 0x1A8);
    }
    if (frame.korosiSlot->frameCounter >= 40) {
        frame.korosiSlot->ownerSlot = BATTLE_TASK_FREE;
    }
    (frame.korosiSlot->frameCounter)++;
}

/*
 * Per-frame task of the pulse sprite. Its grey level ramps off the slot's timer: below frame 6 it is
 * timer * 16, from frame 0x11 on it decays as 0x60 - (timer - 0x11) * 0xC, and in between it holds at
 * 0x60. The level goes to the three colour bytes of the pulse request, whose sprite group and draw
 * object come from resource 1 of g_BattleSpecChefsKnifeTaskPtr. The task installs the billboard matrix at
 * the slot's position and scale, submits the sprite, frees the slot from timer 0x19 on, and advances
 * the timer unless g_BattleIsPaused is set.
 */
// FUNCTION: FF7 0x0051962a
void __cdecl BattleFx_SpecialChefsKnifePulseFadeSpriteTask(void)
{
    unsigned char level;
    int installResult;
    BattleFxDethRampSlot *task;

    task = (BattleFxDethRampSlot *)BATTLE_CURRENT_TASK_SLOT;
    if (task->timer < 6) {
        level = task->timer << 4;
    } else if (task->timer >= 17) {
        level = 0x60 - (task->timer - 0x11) * 12;
    } else {
        level = 0x60;
    }
    g_BattleSpecChefsKnifeSpriteRequest.pulseColorB = level;
    g_BattleSpecChefsKnifeSpriteRequest.pulseColorG = g_BattleSpecChefsKnifeSpriteRequest.pulseColorB;
    g_BattleSpecChefsKnifeSpriteRequest.pulseColorR = g_BattleSpecChefsKnifeSpriteRequest.pulseColorG;
    g_BattleSpecChefsKnifeAnimData.pulseSpriteGroup = Gfx_ModelTableGetLoadedRecord(1, g_BattleSpecChefsKnifeTaskPtr);
    g_BattleSpecChefsKnifeAnimData.pulseDrawObj = Gfx_ModelTableGetSpriteObject(1, g_BattleSpecChefsKnifeTaskPtr);
    installResult = (int)BattleFx_InstallBillboardMatrix(task->pos, (short)task->scale, -(task->scale >> 2));
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitSpriteDraw(&g_BattleSpecChefsKnifeAnimData.pulseSpriteGroup, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (task->timer >= 25) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    (task->timer)++;
}

/*
 * Per-frame task of an animated sprite, the counterpart of BattleFx_SpecialChefsKnifePulseFadeSpriteTask
 * without the colour ramp, drawing resource 0 of g_BattleSpecChefsKnifeTaskPtr. The slot's frameIndex is
 * the sprite's frame. It installs the billboard matrix at the slot's position and scale, submits the
 * sprite, frees the slot once frameIndex reaches 0xB, and advances it unless g_BattleIsPaused is set.
 */
// FUNCTION: FF7 0x00519754
void __cdecl BattleFx_SpecialChefsKnifeAnimatedSpriteTask(void)
{
    BattleFxChefsKnifeAnimatedSprite *task;

    task = (BattleFxChefsKnifeAnimatedSprite *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecChefsKnifeSpriteRequest.animated.frame = task->frameIndex;
    BattleFx_InstallBillboardMatrix(&task->posX, (short)task->scale, -(task->scale >> 2));
    g_BattleSpecChefsKnifeSpriteRequest.animated.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(0, g_BattleSpecChefsKnifeTaskPtr);
    g_BattleSpecChefsKnifeSpriteRequest.animated.drawObj = Gfx_ModelTableGetSpriteObject(0, g_BattleSpecChefsKnifeTaskPtr);
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitSpriteDraw(&g_BattleSpecChefsKnifeSpriteRequest.animated, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (task->frameIndex >= 11) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    (task->frameIndex)++;
}
} /* extern "C" */
