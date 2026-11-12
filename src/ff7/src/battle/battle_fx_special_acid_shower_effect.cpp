// src/ff7/src/battle/battle_fx_special_acid_shower_effect.cpp
/* Enemy Special Attack [80]: Acid Shower (orig. SPECIAL/SHOWER) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle special effect 'Shower': a per-target timeline with animated and falling
//   sprites, an impact model, and a spiral emitter with spiral sprite tasks.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/api/battle.h"
#include "ff7/battle_tasks.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "ff7/types/battle.h"
#include "lib/types/graphics.h"
#include "ff7/types/battle_3d.h"
#include "ff7/types/battle_special.h"
#include "ff7/camera_script.h"
#include <string.h>
#include <stdlib.h>
#include "lib/codegen.h"
extern "C" {

// clang-format off
// GLOBAL: FF7 0x0089e4f8
int g_BattleSpecAcidShowerResourceDefs = 0x001900B1;
char *g_BattleFxSpecialAcidShowerEffectResourceNames[1] = {
    // STRING: FF7 0x0089e5d0
    "hikari.rsd"
};
unsigned int g_BattleSpecAcidShowerResourceDefsDataTail[28] = {
    0x00000000,
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x0089e5dc
    (unsigned int)"doku00.tim",
    // STRING: FF7 0x0089e5e8
    (unsigned int)"doku1.spt",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x0089e5f4
    (unsigned int)"doku02.tim",
    // STRING: FF7 0x0089e600
    (unsigned int)"tama.spt",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x0089e60c
    (unsigned int)"doku02.tim",
    // STRING: FF7 0x0089e618
    (unsigned int)"mizu.spt",
    0x00000000,
    0x00000000,
    0x00192a39,
    0x00000000,
    // STRING: FF7 0x0089e624
    (unsigned int)"jo_c01.spt",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x0089e630
    (unsigned int)"doku01.tim",
    // STRING: FF7 0x0089e63c
    (unsigned int)"baku1.spt",
    0x00000000,
    0x00000000
};
// GLOBAL: FF7 0x0089e570
BattleFxSpriteRequest g_BattleSpecAcidShowerSpriteReq1 = {0, 0, (char)0x80, (char)0x80, (char)0x80, 0x2c, 0, 0};
// GLOBAL: FF7 0x0089e580
BattleSpecAcidShowerSpriteAnims g_BattleSpecAcidShowerModelParams = {{4, 5}, 0, 0};
// GLOBAL: FF7 0x0089e590
BattleSpecAcidShowerImpactDraw g_BattleSpecAcidShowerSpriteReq2 = {(char)0x80, (char)0x80, (char)0x80, 0x2c, 0, 0, 0, 0xa8, 0, 0x80};
unsigned char g_rawData_0089e5a4[4] = {0x20, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x0089e5a8
BattleSpecAcidShowerSpiralSprites g_BattleSpecAcidShowerSpriteReq3 = {{-0x400, 0, 0, 0}, {0, 0, (char)0x80, (char)0x80, (char)0x80, 0x2c, 0, 0}, 0, 0};
unsigned char g_rawData_0089e5c8[8] = {0x80, 0x80, 0x80, 0x2c, 0x00, 0x00, 0x00, 0x00};
// clang-format on
#pragma intrinsic(memset)
#pragma intrinsic(memcpy)


// clang-format off
// GLOBAL: FF7 0x00b5dd80
int g_BattleSpecAcidShowerDrawListHead = 0;
// GLOBAL: FF7 0x00b5dd84
struct GfxTableCursor *g_BattleSpecAcidShowerLoadedResTable = 0;
// clang-format on

/*
 * Loads the SPECIAL/SHOWER model table (resource type 6, definitions g_BattleSpecAcidShowerResourceDefs) into
 * g_BattleSpecAcidShowerLoadedResTable once: only while a battle scene exists and the table is not loaded yet.
 */
// FUNCTION: FF7 0x00525190
void __cdecl BattleFx_SpecialLoadResAcidShower(void)
{
    GfxModelTableBank *curOwnerCtx;
    GfxResourceRequest currentRecord;
    int kind;
    char outSize[200];
    GfxResourceRequest *pRequest;
    int resourceDefsWord;

    if (g_pBattle3dScene != 0 && g_BattleSpecAcidShowerLoadedResTable == 0) {
        kind = 6;
        resourceDefsWord = (int)&g_BattleSpecAcidShowerResourceDefs;
        curOwnerCtx = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRequest = &currentRecord;
        Gfx_InitResourceRequest(pRequest);
        pRequest->groupTag = 0;
        // STRING: FF7 0x0089e648
        pRequest->resourcePath = (char *)BattleFx_BuildResourcePath("SPECIAL/SHOWER", outSize);
        Gfx_LoadModelTableIfAbsent(kind, resourceDefsWord, pRequest, &g_BattleSpecAcidShowerLoadedResTable, curOwnerCtx);
    }
}

/*
 * Forwards both arguments unchanged to BattleFx_SpecialBeginAcidShower; it adds nothing of its own.
 */
// FUNCTION: FF7 0x00525220
void __cdecl BattleFx_SpecialEnterAcidShower(int targetMask, int actorIdx)
{
    BattleFx_SpecialBeginAcidShower(targetMask, actorIdx);
}

/*
 * Calls BattleFx_SpecialLoadResAcidShower and creates a mode-four actor-mask callback task using both arguments and
 * BattleFx_SpecialSpawnAcidShowerPerTarget.
 */
// FUNCTION: FF7 0x00525235
void __cdecl BattleFx_SpecialBeginAcidShower(int targetMask, int actorIdx)
{
    BattleFx_SpecialLoadResAcidShower();
    Battle_CreateBitPatternCallbackTask(targetMask, actorIdx, 4, BattleFx_SpecialSpawnAcidShowerPerTarget);
}

/*
 * Allocates BattleFx_SpecialAcidShowerTimelineTask with the source actor's bone position as the start
 * point and the target actor's bone position as the end point, and records the target actor.
 * Plays the cast sound panned to the source actor.
 */
// FUNCTION: FF7 0x00525256
void __cdecl BattleFx_SpecialSpawnAcidShowerPerTarget(int actor, int sourceActor)
{
    int p; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxAcidShowerTrack *q;

    q = (BattleFxAcidShowerTrack *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialAcidShowerTimelineTask);
    Battle_GetBoneWorldPos(sourceActor, g_BattleActorRecords[sourceActor].bone34, &q->startX);
    Battle_GetBoneWorldPos(actor, g_BattleActorRecords[actor].targetBone, &q->endX);
    q->actorIndex = actor;
    Battle_PlaySfxPannedToActor(0x11F, (short)sourceActor, 0);
}

/*
 * Per-target timeline of the Shower: on frame 0 spawns the spiral emitter on the path from the source
 * toward the target, plays a sound on 0x20, spawns animated sprites around the target on frames
 * 0x29..0x2F, the impact model on 0x30, calls Battle_PostActorHitOrHurtMessage for the target on 0x5A, and ends on 0x64.
 */
// FUNCTION: FF7 0x005252e2
void __cdecl BattleFx_SpecialAcidShowerTimelineTask(void)
{
    short dz;
    BattleFxAcidShowerTrack *trackPtr;
    int r; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxAcidShowerDropRecord *dropRecPtr;
    int t; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int u; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int v; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int w; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int x; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int y; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int z; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    short loopDeltaX;
    short theta;
    int c; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int d; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int e; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int f; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int g; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */

    trackPtr = (BattleFxAcidShowerTrack *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (trackPtr->frame == 0) {
        dropRecPtr = (BattleFxAcidShowerDropRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialAcidShowerSpiralEmitterTask);
        dropRecPtr->heading = ArcTangentFixed(trackPtr->endX - trackPtr->startX, trackPtr->endZ - trackPtr->startZ);
        dropRecPtr->posX = trackPtr->startX;
        dropRecPtr->posY = trackPtr->startY;
        dropRecPtr->posZ = trackPtr->startZ;
        loopDeltaX = trackPtr->endX - dropRecPtr->posX;
        dz = trackPtr->endZ - dropRecPtr->posZ;
        dropRecPtr->frameCount = SqrtToInt(loopDeltaX * loopDeltaX + dz * dz) / 24;
    }
    if (trackPtr->frame == 32) {
        Battle_PlaySfxPannedToActor(0x10B, (short)trackPtr->actorIndex, 0);
    }
    if (trackPtr->frame == 48) {
        dropRecPtr = (BattleFxAcidShowerDropRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialAcidShowerImpactModelTask);
        dropRecPtr->posX = trackPtr->endX;
        dropRecPtr->posY = trackPtr->endY;
        dropRecPtr->posZ = trackPtr->endZ;
    }
    if (trackPtr->frame > 40 && trackPtr->frame < 48) {
        theta = (trackPtr->frame - 0x28 << 9) - 0x400;
        dropRecPtr = (BattleFxAcidShowerDropRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialAcidShowerAnimatedSpriteTask);
        Battle_GetBoneWorldPos(trackPtr->actorIndex, g_BattleActorRecords[trackPtr->actorIndex].targetBone, &dropRecPtr->posX);
        dropRecPtr->posX += (short)(CosFixed(theta) * 0x320 >> 12);
        dropRecPtr->posY += (short)(SinFixed(theta) * 0x320 >> 12);
        dropRecPtr->active = 1;
        dropRecPtr = (BattleFxAcidShowerDropRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialAcidShowerAnimatedSpriteTask);
        Battle_GetBoneWorldPos(trackPtr->actorIndex, g_BattleActorRecords[trackPtr->actorIndex].targetBone, &dropRecPtr->posX);
        dropRecPtr->posX += (short)(rand() % 600 - 300);
        dropRecPtr->posY += (short)(rand() % 600 - 300);
        dropRecPtr->posZ += (short)(rand() % 600 - 300);
        dropRecPtr->active = 0;
    }
    if (trackPtr->frame == 90) {
        Battle_PostActorHitOrHurtMessage(trackPtr->actorIndex);
    }
    (trackPtr->frame)++;
    if (trackPtr->frame == 100) {
        trackPtr->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Animated billboard sprite: the variant selects the animation looked up in
 * g_BattleSpecAcidShowerLoadedResTable and offsets the billboard scale (0x2000, or 0x3000 for a nonzero
 * variant). The sprite request takes the animation's draw object in a nonzero variant but
 * Battle3d_GetSceneRenderObject(3, g_pBattle3dScene) in variant 0. While unpaused the frame index
 * advances until it reaches the animation's frame count, then the slot is freed.
 */
// FUNCTION: FF7 0x00525635
void __cdecl BattleFx_SpecialAcidShowerAnimatedSpriteTask(void)
{
    BattleFxOilSpriteSlot *task;

    task = (BattleFxOilSpriteSlot *)BATTLE_CURRENT_TASK_SLOT;
    BattleFx_InstallBillboardMatrix(&task->posX, (task->variant != 0 ? 0x1000 : 0) + 0x2000, 0);
    g_BattleSpecAcidShowerSpriteReq1.frame = task->frameIndex;
    g_BattleSpecAcidShowerSpriteReq1.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(g_BattleSpecAcidShowerModelParams.variantAnim[task->variant], g_BattleSpecAcidShowerLoadedResTable);
    if (task->variant != 0) {
        g_BattleSpecAcidShowerSpriteReq1.drawObj = Gfx_ModelTableGetSpriteObject(g_BattleSpecAcidShowerModelParams.variantAnim[task->variant], g_BattleSpecAcidShowerLoadedResTable);
    } else {
        g_BattleSpecAcidShowerSpriteReq1.drawObj = Battle3d_GetSceneRenderObject(3, g_pBattle3dScene);
    }
    g_BattleSpecAcidShowerDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecAcidShowerSpriteReq1, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecAcidShowerDrawListHead);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (g_BattleSpecAcidShowerSpriteReq1.spriteGroup != 0) {
        (task->frameIndex)++;
        if (task->frameIndex >= g_BattleSpecAcidShowerSpriteReq1.spriteGroup->header->frameCount) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Impact model at the target: drawn turned by g_BattleSpecAcidShowerSpriteReq3.impactRotation, scaled by
 * 0x800 + 0xC8 per frame (4.12 fixed point) and faded in with its frame counter. For its first 0x23
 * frames it spawns two falling sprites a frame with random outward velocities; it ends after 60 frames.
 */
// FUNCTION: FF7 0x00525756
void __cdecl BattleFx_SpecialAcidShowerImpactModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[96];
        int screenDepth;
        struct { unsigned char pad_to_4[0x4]; PsxMatrix angleMatrix; };
        struct { unsigned char pad_to_28[0x28]; PsxMatrix impactMatrix; };
        struct { unsigned char pad_to_48[0x48]; BattleFxAcidShowerFallingSpriteSlot *fallingSprite; };
        struct { unsigned char pad_to_4C[0x4C]; short axisIndex; };
        struct { unsigned char pad_to_54[0x54]; BattleFxOilTargetModelSlot *impactSlot; };
        struct { unsigned char pad_to_5C[0x5C]; short spawnAngle; };
    } frame;
#pragma pack(pop)

    frame.impactSlot = (BattleFxOilTargetModelSlot *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecAcidShowerSpriteReq2.modelGroup = Gfx_ModelTableGetRenderObject(0, g_BattleSpecAcidShowerLoadedResTable);
    for (frame.axisIndex = 0; frame.axisIndex < 3; frame.axisIndex++) {
        frame.impactMatrix.m[2][frame.axisIndex] = 0;
        frame.impactMatrix.m[1][frame.axisIndex] = 0;
        frame.impactMatrix.m[0][frame.axisIndex] = 0;
    }
    SetRotMatrix(&g_BattleCameraTransform);
    SetTransMatrix(&g_BattleCameraTransform);
    RotTransPoint((SVECTOR *)&frame.impactSlot->posX, frame.impactMatrix.t, &frame.screenDepth);
    frame.impactMatrix.m[2][2] = frame.impactSlot->frameCounter * 200 + 0x800;
    frame.impactMatrix.m[1][1] = frame.impactMatrix.m[2][2];
    frame.impactMatrix.m[0][0] = frame.impactMatrix.m[1][1];
    RotMatrix(&g_BattleSpecAcidShowerSpriteReq3.impactRotation, &frame.angleMatrix);
    ConcatMatrixIntoFirst(&frame.impactMatrix, &frame.angleMatrix);
    SetRotMatrix(&frame.impactMatrix);
    SetTransMatrix(&frame.impactMatrix);
    SetDrawColorRGB(0, 0, 0);
    g_BattleSpecAcidShowerSpriteReq2.modelFadeLevel = frame.impactSlot->frameCounter << 6;
    g_BattleSpecAcidShowerDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecAcidShowerSpriteReq2.modelGroup, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecAcidShowerDrawListHead);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.impactSlot->frameCounter < 35) {
        for (frame.axisIndex = 0; frame.axisIndex < 2; frame.axisIndex++) {
            frame.fallingSprite = (BattleFxAcidShowerFallingSpriteSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialAcidShowerFallingSpriteTask);
            frame.fallingSprite->posX = frame.impactSlot->posX;
            frame.fallingSprite->posY = 0;
            frame.fallingSprite->posZ = frame.impactSlot->posZ;
            frame.spawnAngle = ((rand()) % 4096);
            frame.fallingSprite->velocityX = SinFixed(frame.spawnAngle) * 0x3C >> 12;
            frame.fallingSprite->velocityZ = CosFixed(frame.spawnAngle) * 0x3C >> 12;
            frame.fallingSprite->velocityY = 0xFFFFFF6A - rand() % 50;
            frame.fallingSprite->scale = (rand() % 3 << 8) + 0x600;
        }
    }
    (frame.impactSlot->frameCounter)++;
    if (frame.impactSlot->frameCounter == 60) {
        frame.impactSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Falling billboard sprite using animation 2 of g_BattleSpecAcidShowerLoadedResTable at the sprite's
 * scale. Brightness is flat at 8 for the first 22 frames and then ramps down as 0x1E - frame; it
 * is shifted left 4 into the three colour bytes of the sprite request. While g_BattleIsPaused is
 * zero the position advances by the velocity, 0xA is added to the Y velocity, and the slot is freed
 * after a fixed 0x1E-frame life.
 */
// FUNCTION: FF7 0x005259cd
void __cdecl BattleFx_SpecialAcidShowerFallingSpriteTask(void)
{
    BattleFxAcidShowerFallingSprite *sprite;
    int brightness;

    sprite = (BattleFxAcidShowerFallingSprite *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecAcidShowerModelParams.fallingSpriteGroup = Gfx_ModelTableGetLoadedRecord(2, g_BattleSpecAcidShowerLoadedResTable);
    g_BattleSpecAcidShowerModelParams.fallingDrawObj = Gfx_ModelTableGetSpriteObject(2, g_BattleSpecAcidShowerLoadedResTable);
    BattleFx_InstallBillboardMatrix(&sprite->posX, (short)sprite->scale, 0);
    if (sprite->frameCount < 22) {
        brightness = 8;
    } else {
        brightness = 0x1E - sprite->frameCount;
    }
    g_BattleSpecAcidShowerSpriteReq2.fallingColorB = brightness << 4;
    g_BattleSpecAcidShowerSpriteReq2.fallingColorG = g_BattleSpecAcidShowerSpriteReq2.fallingColorB;
    g_BattleSpecAcidShowerSpriteReq2.fallingColorR = g_BattleSpecAcidShowerSpriteReq2.fallingColorG;
    g_BattleSpecAcidShowerDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecAcidShowerModelParams.fallingSpriteGroup, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecAcidShowerDrawListHead);
    if (g_BattleIsPaused != 0) {
        return;
    }
    sprite->posX += sprite->velocityX;
    sprite->posY += sprite->velocityY;
    sprite->posZ += sprite->velocityZ;
    sprite->velocityY += 10;
    (sprite->frameCount)++;
    if (sprite->frameCount == 30) {
        sprite->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Spiral emitter on the path toward the target: each frame spawns a sprite pair at its position, with a
 * radius that swells and shrinks with the frame and a velocity of scaledPathLength turned by the path
 * heading and a small wobble. It ends after 32 frames.
 */
// FUNCTION: FF7 0x00525b1f
void __cdecl BattleFx_SpecialAcidShowerSpiralEmitterTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[64];
        SVECTOR rotX;
        struct { unsigned char pad_to_8[0x8]; int pointDepth; };
        struct { unsigned char pad_to_C[0xC]; SVECTOR pointX; };
        struct { unsigned char pad_to_14[0x14]; PsxMatrix spinMatrix; };
        struct { unsigned char pad_to_34[0x34]; BattleFxAcidShowerSpiralSpritePairSlot *spritePair; };
        struct { unsigned char pad_to_38[0x38]; short spiralRadius; };
        struct { unsigned char pad_to_3C[0x3C]; BattleFxAcidShowerSpiralEmitterSlot *emitter; };
    } frame;
#pragma pack(pop)

    frame.emitter = (BattleFxAcidShowerSpiralEmitterSlot *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.spiralRadius = (SinFixed(frame.emitter->frameCounter << 6) * 0x140 >> 12) + 320;
    frame.spritePair = (BattleFxAcidShowerSpiralSpritePairSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialAcidShowerSpiralSpritePairTask);
    frame.spritePair->posX = frame.emitter->posX;
    frame.spritePair->posZ = frame.emitter->posZ;
    frame.spritePair->posY = frame.emitter->posY;
    frame.spritePair->spiralPhase = frame.emitter->frameCounter >> 1;
    frame.spritePair->scale = frame.spiralRadius * 5;
    frame.spritePair->radius = frame.spiralRadius;
    frame.spritePair->angle = (rand() % 4096);
    frame.pointX.vx = 0;
    frame.pointX.vy = 0;
    frame.pointX.vz = frame.emitter->scaledPathLength;
    frame.rotX.vx = 0;
    frame.rotX.vy = (-SinFixed(frame.emitter->frameCounter * 0x88)) / 16;
    frame.rotX.vz = 0x200 /* 45 deg */;
    RotMatrixZYX(&frame.rotX, &frame.spinMatrix);
    RotateMatrixY(frame.emitter->angleY, &frame.spinMatrix);
    frame.spinMatrix.t[2] = 0;
    frame.spinMatrix.t[1] = frame.spinMatrix.t[2];
    frame.spinMatrix.t[0] = frame.spinMatrix.t[1];
    SetRotMatrix(&frame.spinMatrix);
    SetTransMatrix(&frame.spinMatrix);
    RotTransPointToInt16(&frame.pointX, &frame.spritePair->velocityX, &frame.pointDepth);
    (frame.emitter->frameCounter)++;
    if (frame.emitter->frameCounter == 32) {
        frame.emitter->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Draws a centre sprite (animation 1) at the pair's position and an orbiting sprite (animation 3) on a
 * circle of the pair's radius around it, moves the pair by its velocity and turns the orbit 0x100 a
 * frame. The slot is freed when the centre animation, played at half speed, reaches its last frame.
 */
// FUNCTION: FF7 0x00525cce
void __cdecl BattleFx_SpecialAcidShowerSpiralSpritePairTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[12];
        SVECTOR drawX;
        struct { unsigned char pad_to_8[0x8]; BattleFxAcidShowerSpiralSpritePairSlot *spritePair; };
    } frame;
#pragma pack(pop)

    frame.spritePair = (BattleFxAcidShowerSpiralSpritePairSlot *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecAcidShowerSpriteReq3.centreSprite.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(1, g_BattleSpecAcidShowerLoadedResTable);
    g_BattleSpecAcidShowerSpriteReq3.centreSprite.drawObj = Gfx_ModelTableGetSpriteObject(1, g_BattleSpecAcidShowerLoadedResTable);
    g_BattleSpecAcidShowerSpriteReq3.orbitSpriteGroup = Gfx_ModelTableGetLoadedRecord(3, g_BattleSpecAcidShowerLoadedResTable);
    g_BattleSpecAcidShowerSpriteReq3.orbitDrawObj = Gfx_ModelTableGetSpriteObject(3, g_BattleSpecAcidShowerLoadedResTable);
    BattleFx_InstallBillboardMatrix(&frame.spritePair->posX, 0x2000 /* 2.0 */, 0);
    g_BattleSpecAcidShowerSpriteReq3.centreSprite.frame = frame.spritePair->spiralPhase;
    g_BattleSpecAcidShowerDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecAcidShowerSpriteReq3.centreSprite, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecAcidShowerDrawListHead);
    frame.drawX.vx = frame.spritePair->posX + (CosFixed(frame.spritePair->angle) * frame.spritePair->radius >> 12);
    frame.drawX.vy = frame.spritePair->posZ + (SinFixed(frame.spritePair->angle) * frame.spritePair->radius >> 12);
    frame.drawX.vz = frame.spritePair->posY;
    BattleFx_InstallBillboardMatrix(&frame.drawX, (short)frame.spritePair->scale, 0);
    g_BattleSpecAcidShowerDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecAcidShowerSpriteReq3.orbitSpriteGroup, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecAcidShowerDrawListHead);
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.spritePair->posX += frame.spritePair->velocityX;
    frame.spritePair->posZ += frame.spritePair->velocityZ;
    frame.spritePair->posY += frame.spritePair->velocityY;
    frame.spritePair->angle += 0x100 /* 22.5 deg */;
    if (g_BattleSpecAcidShowerSpriteReq3.centreSprite.spriteGroup != 0) {
        (frame.spritePair->frameCounter)++;
        if (frame.spritePair->frameCounter >> 1 >= g_BattleSpecAcidShowerSpriteReq3.centreSprite.spriteGroup->header->frameCount) {
            frame.spritePair->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}
} /* extern "C" */
