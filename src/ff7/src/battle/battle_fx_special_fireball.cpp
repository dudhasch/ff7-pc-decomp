// src/ff7/src/battle/battle_fx_special_fireball.cpp
/* Enemy Special Effect [idx 1]: Fireball (JP: kaen / 火炎 (Flame), asset: "SPECIAL/KAEN") — Fireball (0x1e9, Bomb), Flame (0x28b, Diablo), Fire Veil (0x363), Turk Light (0x370) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle special effect 'Kaen' (flame): a per-target timeline with animated sprites, a
//   bouncing sprite and a projectile.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/api/battle.h"
#include "ff7/battle_tasks.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_special.h"
#include "ff7/camera_script.h"
#include "lib/types/graphics.h"
#include "ff7/types/battle_3d.h"
#include <string.h>
#include <stdlib.h>
#include "lib/codegen.h"
extern "C" {

struct BattleFxModelDraw;
// clang-format off
// GLOBAL: FF7 0x008a2b78
int g_BattleSpecFireballResourceDefs = 0x001900B1;
int g_BattleSpecFireballFirstModelNamePtr[1] = {
    (int)((char *)&g_BattleSpecFireballModelNames + 0x0)
};
int g_BattleSpecFireballResourceRecords[64] = {
    0x0, 0x0, 0x0, 0x1900b1, (int)((char *)&g_BattleSpecFireballModelNames + 0xc), 0x0,
    0x0, 0x0, 0x1900b1, (int)((char *)&g_BattleSpecFireballModelNames + 0x18), 0x0, 0x0,
    0x0, 0x1900b1, (int)((char *)&g_BattleSpecFireballModelNames + 0x24), 0x0, 0x0, 0x0,
    0x1900b1, (int)((char *)&g_BattleSpecFireballModelNames + 0x30), 0x0, 0x0, 0x0, 0x1900b1,
    (int)((char *)&g_BattleSpecFireballModelNames + 0x3c), 0x0, 0x0, 0x0, 0x1900b1, (int)((char *)&g_BattleSpecFireballModelNames + 0x48),
    0x0, 0x0, 0x0, 0x1900b1, (int)((char *)&g_BattleSpecFireballModelNames + 0x54), 0x0,
    0x0, 0x0, 0x192a39, (int)((char *)&g_BattleSpecFireballSpriteNames + 0x0), (int)((char *)&g_BattleSpecFireballSpriteNames + 0xc), 0x0,
    0x0, 0x192a39, (int)((char *)&g_BattleSpecFireballSpriteNames + 0x18), (int)((char *)&g_BattleSpecFireballSpriteNames + 0x24), 0x0, 0x0,
    0x192a39, 0x0, (int)((char *)&g_BattleSpecFireballSpriteNames + 0x30), 0x0, 0x0, 0x192a39,
    (int)((char *)&g_BattleSpecFireballSpriteNames + 0x3c), (int)((char *)&g_BattleSpecFireballSpriteNames + 0x48), 0x0, 0x0, 0x192a39, (int)((char *)&g_BattleSpecFireballSpriteNames + 0x54),
    (int)((char *)&g_BattleSpecFireballSpriteNames + 0x60), 0x0, 0x0, 0x0
};
// GLOBAL: FF7 0x008a2c80
struct BattleSpecFireballSpriteReq1 g_BattleSpecFireballSpriteReq1 = {0, 0, {0x80, 0x80, 0x80, 0x2c}, 0, {0}, 0, 0};
// GLOBAL: FF7 0x008a2c98
struct BattleSpecFireballSpriteReq2 g_BattleSpecFireballSpriteReq2 = {-128, -128, -128, 44, 0, {0}};
// GLOBAL: FF7 0x008a2ca0
struct BattleSpecFireballModelParams g_BattleSpecFireballModelParams = {{0, 0xfc, 0, 0, 0, 0, 0, 0}, 0, {0x28, 0, 0, 0, 0, 0, 0x80, 0, 0x20, 0, 0, 0}, 0, 0};
unsigned char g_BattleSpecFireballColorWord[8] = {0x80, 0x80, 0x80, 0x2e, 0x00, 0x00, 0x00, 0x00};
unsigned char g_BattleSpecFireballModelNames[96] =
    "SOTO1.RSD\0\0\0"
    "SOTO2.RSD\0\0\0"
    "SOTO3.RSD\0\0\0"
    "SOTO4.RSD\0\0\0"
    "SOTO5.RSD\0\0\0"
    "SOTO6.RSD\0\0\0"
    "SOTO7.RSD\0\0\0"
    "SOTO8.RSD";
unsigned char g_BattleSpecFireballSpriteNames[108] =
    "KAEN202.TIM\0"
    "HIKARI1.SPT\0"
    "KAEN202.TIM\0"
    "HIT1.SPT\0\0\0\0"
    "JO_C02.SPT\0\0"
    "KAEN202.TIM\0"
    "HASSYA1.SPT\0"
    "KAEN202.TIM\0"
    "HAHEN.SPT";
// clang-format on
#pragma intrinsic(memset)
#pragma intrinsic(memcpy)


// clang-format off
// GLOBAL: FF7 0x00b64fcc
int g_BattleSpecFireballPacketCursor = 0;
// GLOBAL: FF7 0x00b64fd0
struct GfxTableCursor *g_BattleSpecFireballLoadedResTable = 0;
unsigned char g_bssPad_00b64fd4[4] = {0};
// clang-format on

/*
 * Loads the SPECIAL/KAEN resource table (resource type 13) the first time it is needed, once the battle scene exists.
 */
// FUNCTION: FF7 0x00536850
void __cdecl BattleFx_SpecialLoadResFireball(void)
{
    GfxModelTableBank *curOwnerCtx;
    GfxResourceRequest reqBuffer;
    int kind;
    char pathText[200];
    GfxResourceRequest *pRequest;
    int resourceDefsWord;

    if (g_pBattle3dScene != 0 && g_BattleSpecFireballLoadedResTable == 0) {
        kind = 0xD;
        resourceDefsWord = (int)&g_BattleSpecFireballResourceDefs;
        curOwnerCtx = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRequest = &reqBuffer;
        Gfx_InitResourceRequest(pRequest);
        pRequest->groupTag = 0;
        // STRING: FF7 0x008a2d94
        pRequest->resourcePath = BattleFx_BuildResourcePath("SPECIAL/KAEN", pathText);
        Gfx_LoadModelTableIfAbsent(kind, resourceDefsWord, pRequest, &g_BattleSpecFireballLoadedResTable, curOwnerCtx);
    }
}

/*
 * Mechanical forwarder: calls BattleFx_SpecialBeginFireball with both arguments unchanged.
 */
// FUNCTION: FF7 0x005368e0
void __cdecl BattleFx_SpecialEnterFireball(int targetMask, int actorIdx)
{
    BattleFx_SpecialBeginFireball(targetMask, actorIdx);
}

/*
 * Calls BattleFx_SpecialLoadResFireball, then creates a task for BattleFx_SpecialSpawnFireballPerTarget with both arguments and
 * mode 4, then dispatches event (0x163, actorIndex, 0).
 */
// FUNCTION: FF7 0x005368f5
void __cdecl BattleFx_SpecialBeginFireball(int targetMask, int actorIdx)
{
    BattleFx_SpecialLoadResFireball();
    Battle_CreateBitPatternCallbackTask(targetMask, actorIdx, 4, BattleFx_SpecialSpawnFireballPerTarget);
    Battle_PlaySfxPannedToActor(0x163, (short)actorIdx, 0);
}

/*
 * Starts a Kaen timeline task: the flame starts at the source actor's bone33 position, ends at the target's
 * targetBone position and remembers the target actor.
 */
// FUNCTION: FF7 0x0053692a
void __cdecl BattleFx_SpecialSpawnFireballPerTarget(int actor, int sourceActor)
{
    BattleFxFireballTimelineTask *taskPtr;

    taskPtr = (BattleFxFireballTimelineTask *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialFireballTimelineTask);
    Battle_GetBoneWorldPos(sourceActor, g_BattleActorRecords[sourceActor].bone33, &taskPtr->startX);
    Battle_GetBoneWorldPos(actor, g_BattleActorRecords[actor].targetBone, &taskPtr->endX);
    taskPtr->actorIndex = actor;
}

/*
 * Kaen timeline: launches the projectile, then the impact sparks and bouncing flames, then ends after 30 frames.
 */
// FUNCTION: FF7 0x005369a0
void __cdecl BattleFx_SpecialFireballTimelineTask(void)
{
    BattleFxFireballTimelineTask *mainSelf;
    short i7;
    BattleFxFireballParticleRecord *pParticle;
    int s; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int t; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int u; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int v; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int w; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int x; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int y; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int z; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int a; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int b; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int c; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int d; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int e; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int f; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int g; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */

    mainSelf = (BattleFxFireballTimelineTask *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (mainSelf->phase == 0) {
        pParticle = (BattleFxFireballParticleRecord *)&g_BattleTaskSlotBuffer.secondarySlots[(short)Battle_AllocateSecondaryResourceSlot(BattleFx_SpecialFireballAnimatedSpriteTask)];
        pParticle->posX = mainSelf->startX;
        pParticle->posY = mainSelf->startY;
        pParticle->posZ = mainSelf->startZ;
        pParticle->variant = 2;
        pParticle = (BattleFxFireballParticleRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialFireballProjectileTask);
        pParticle->posX = mainSelf->startX;
        pParticle->posY = mainSelf->startY;
        pParticle->posZ = mainSelf->startZ;
        pParticle->velocityX = (mainSelf->endX - mainSelf->startX) / 8;
        pParticle->velocityY = (mainSelf->endY - mainSelf->startY) / 8;
        pParticle->velocityZ = (mainSelf->endZ - mainSelf->startZ) / 8;
    }
    if (mainSelf->phase == 8) {
        for (i7 = 0; i7 < 8; i7++) {
            pParticle = (BattleFxFireballParticleRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialFireballBouncingSpriteTask);
            pParticle->posX = mainSelf->endX + rand() % 400 + -200;
            pParticle->posZ = mainSelf->endZ + rand() % 400 + -200;
            pParticle->posY = 0;
            BattleFx_MakeRandomConeVector(0xFFFFFF06, 100, &pParticle->velocityX);
            pParticle->variant = rand() % 20;
        }
        Battle_PlaySfxPannedToActor(0xFA, (short)mainSelf->actorIndex, 0);
    }
    if (mainSelf->phase > 11 && mainSelf->phase < 19) {
        pParticle = (BattleFxFireballParticleRecord *)&g_BattleTaskSlotBuffer.secondarySlots[(short)Battle_AllocateSecondaryResourceSlot(BattleFx_SpecialFireballAnimatedSpriteTask)];
        pParticle->posX = mainSelf->endX + rand() % 1200 + -600;
        pParticle->posY = mainSelf->endY + rand() % 1200 + -600;
        pParticle->posZ = mainSelf->endZ + rand() % 1200 + -600;
        pParticle->variant = 1;
    }
    if (mainSelf->phase == 0xF) {
        Battle_PostActorHitOrHurtMessage(mainSelf->actorIndex);
    }
    (mainSelf->phase)++;
    if (mainSelf->phase == 0x1E) {
        mainSelf->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Draws one animated flame sprite (table entry 9, 10 or 11 by mode, or the scene's sprite object for mode 1),
 * advances its frame and frees the slot at the last frame.
 */
// FUNCTION: FF7 0x00536c9e
void __cdecl BattleFx_SpecialFireballAnimatedSpriteTask(void)
{
    struct {
        int table[3];
        BattleSpecPoisonGasSprite *slot;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    frame.slot = (BattleSpecPoisonGasSprite *)&g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx];
    frame.table[0] = 9;
    frame.table[1] = 0xA;
    frame.table[2] = 0xB;
    BattleFx_InstallBillboardMatrix(&frame.slot->posX, (frame.slot->mode - 2 != 0 ? 0x1000 : 0) + 0x2000, 0);
    (g_BattleSpecFireballSpriteReq1.spriteEntry) = Gfx_ModelTableGetLoadedRecord(frame.table[frame.slot->mode], g_BattleSpecFireballLoadedResTable);
    if (frame.slot->mode != 1) {
        g_BattleSpecFireballSpriteReq1.spriteObject = Gfx_ModelTableGetSpriteObject(frame.table[frame.slot->mode], g_BattleSpecFireballLoadedResTable);
    } else {
        g_BattleSpecFireballSpriteReq1.spriteObject = Battle3d_GetSceneRenderObject(3, g_pBattle3dScene);
    }
    g_BattleSpecFireballSpriteReq1.frameIndex = frame.slot->frameIndex;
    g_BattleOverlayDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecFireballSpriteReq1, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused == 0 && (g_BattleSpecFireballSpriteReq1.spriteEntry) != 0) {
        (frame.slot->frameIndex)++;
        if (frame.slot->frameIndex >= ((BattleFxSpriteGroup *)g_BattleSpecFireballSpriteReq1.spriteEntry)->header->frameCount) {
            frame.slot->ownerSlot = 0xFFFF;
        }
    }
}

/*
 * Draws and moves one bouncing flame sprite: it falls under gravity, rebounds off the ground and ends after 20 frames.
 */
// FUNCTION: FF7 0x00536dd4
void __cdecl BattleFx_SpecialFireballBouncingSpriteTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[92];
        int phaseValue;
        struct { unsigned char pad_to_4[0x4]; SVECTOR rotX; };
        struct { unsigned char pad_to_C[0xC]; PsxMatrix spinMatrix; };
        struct { unsigned char pad_to_2C[0x2C]; short phase; };
        struct { unsigned char pad_to_30[0x30]; PsxMatrix scaleMatrix; };
        struct { unsigned char pad_to_50[0x50]; short axisIndex; };
        struct { unsigned char pad_to_54[0x54]; int screenDepth; };
        struct { unsigned char pad_to_58[0x58]; BattleFxTailLaserBouncingSpriteSlot *slot; };
    } frame;
#pragma pack(pop)

    frame.slot = (BattleFxTailLaserBouncingSpriteSlot *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecFireballSpriteReq1.drawBlock = Gfx_ModelTableGetLoadedRecord(12, g_BattleSpecFireballLoadedResTable);
    g_BattleSpecFireballSpriteReq1.handle = Gfx_ModelTableGetSpriteObject(12, g_BattleSpecFireballLoadedResTable);
    for (frame.axisIndex = 0; frame.axisIndex < 3; frame.axisIndex++) {
        frame.scaleMatrix.m[2][frame.axisIndex] = 0;
        frame.scaleMatrix.m[1][frame.axisIndex] = 0;
        frame.scaleMatrix.m[0][frame.axisIndex] = 0;
    }
    if (frame.slot->frameCounter < 12) {
        frame.phaseValue = 8;
    } else {
        frame.phaseValue = 0x14 - frame.slot->frameCounter;
    }
    frame.phase = *(short *)&frame.phaseValue;
    frame.rotX.vy = 0;
    frame.rotX.vx = frame.rotX.vy;
    frame.rotX.vz = frame.slot->angle;
    SetRotMatrix(&g_BattleCameraTransform);
    SetTransMatrix(&g_BattleCameraTransform);
    RotTransPoint((SVECTOR *)&frame.slot->posX, frame.scaleMatrix.t, &frame.screenDepth);
    frame.scaleMatrix.m[2][2] = frame.slot->variant < 4 ? 0x1400 : 0x2000;
    frame.scaleMatrix.m[1][1] = frame.scaleMatrix.m[2][2];
    frame.scaleMatrix.m[0][0] = frame.scaleMatrix.m[1][1];
    RotMatrix(&frame.rotX, &frame.spinMatrix);
    ConcatMatrixIntoFirst(&frame.scaleMatrix, &frame.spinMatrix);
    SetRotMatrix(&frame.scaleMatrix);
    SetTransMatrix(&frame.scaleMatrix);
    g_BattleSpecFireballSpriteReq2.frame = frame.slot->variant;
    g_BattleSpecFireballSpriteReq2.code = (frame.phase - 8 != 0 ? 2 : 0) + 0x2C;
    g_BattleSpecFireballSpriteReq2.blue = frame.phase << 4;
    g_BattleSpecFireballSpriteReq2.green = g_BattleSpecFireballSpriteReq2.blue;
    (g_BattleSpecFireballSpriteReq2.red) = g_BattleSpecFireballSpriteReq2.green;
    g_BattleOverlayDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecFireballSpriteReq1.drawBlock, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.slot->posX += frame.slot->velocityX;
    frame.slot->posY += frame.slot->velocityY;
    frame.slot->posZ += frame.slot->velocityZ;
    if (frame.slot->posY > 0) {
        frame.slot->posY = 0;
        frame.slot->velocityY *= -1;
    }
    frame.slot->velocityY += 20;
    frame.slot->angle += 0x200 /* 45 deg */;
    (frame.slot->frameCounter)++;
    if (frame.slot->frameCounter == 20) {
        frame.slot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Draws and moves the flame projectile (model and sprite); on frame 8 it spawns a flame sprite and ends.
 */
// FUNCTION: FF7 0x00537068
void __cdecl BattleFx_SpecialFireballProjectileTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[160];
        PsxMatrix modelMatrix;
        struct { unsigned char pad_to_20[0x20]; BattleFxEnergyWaveSpinSlot *childSlot; };
        struct { unsigned char pad_to_24[0x24]; int stepIdx0[8]; };
        struct { unsigned char pad_to_4C[0x4C]; BattleFxHellBomberRocketSlot *slot; };
        struct { unsigned char pad_to_50[0x50]; PsxMatrix lookBasis; };
        struct { unsigned char pad_to_70[0x70]; PsxMatrix drawMatrix; };
        struct { unsigned char pad_to_90[0x90]; GfxDirectionVec3 scaleX; };
        struct { unsigned char pad_to_9C[0x9C]; int scaleExtra; };
    } frame;
#pragma pack(pop)

    frame.slot = (BattleFxHellBomberRocketSlot *)BATTLE_CURRENT_TASK_SLOT;
    frame.scaleX.x = 0x800 /* 0.5 */;
    frame.scaleX.y = 0xC00 /* 0.75 */;
    frame.scaleX.z = 0x800 /* 0.5 */;
    memset(&frame.scaleExtra, 0, 4);
    g_BattleSpecFireballModelParams.spriteDraw = Gfx_ModelTableGetLoadedRecord(8, g_BattleSpecFireballLoadedResTable);
    g_BattleSpecFireballModelParams.handle = Gfx_ModelTableGetSpriteObject(8, g_BattleSpecFireballLoadedResTable);
    frame.stepIdx0[0] = 0;
    frame.stepIdx0[1] = 1;
    frame.stepIdx0[2] = 2;
    frame.stepIdx0[3] = 3;
    frame.stepIdx0[4] = 4;
    frame.stepIdx0[5] = 5;
    frame.stepIdx0[6] = 6;
    frame.stepIdx0[7] = 7;
    Gte_BuildLookBasisFromDirection(&frame.slot->velocityX, frame.lookBasis.m[0]);
    RotMatrix((SVECTOR *)&g_BattleSpecFireballModelParams, &frame.modelMatrix);
    ConcatMatrixPreservingTranslation(&frame.lookBasis, &frame.modelMatrix, &frame.modelMatrix);
    ScaleMatrix(&frame.modelMatrix, &frame.scaleX);
    frame.modelMatrix.t[0] = frame.slot->posX;
    frame.modelMatrix.t[1] = frame.slot->posY;
    frame.modelMatrix.t[2] = frame.slot->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.modelMatrix, &frame.drawMatrix);
    SetRotMatrix(&frame.drawMatrix);
    SetTransMatrix(&frame.drawMatrix);
    g_BattleSpecFireballModelParams.modelDraw = Gfx_ModelTableGetRenderObject(frame.stepIdx0[((frame.slot->frameCounter) % 8)], g_BattleSpecFireballLoadedResTable);
    g_BattleSpecFireballPacketCursor = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecFireballModelParams.modelDraw, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecFireballPacketCursor);
    BattleFx_InstallBillboardMatrix(&frame.slot->posX, 0x1000 /* 1.0 */, 0);
    g_BattleOverlayDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecFireballModelParams.spriteDraw, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.slot->posX += frame.slot->velocityX;
    frame.slot->posY += frame.slot->velocityY;
    frame.slot->posZ += frame.slot->velocityZ;
    (frame.slot->frameCounter)++;
    if (frame.slot->frameCounter == 8) {
        frame.childSlot = (BattleFxEnergyWaveSpinSlot *)(&g_BattleTaskSlotBuffer.secondarySlots[(short)Battle_AllocateSecondaryResourceSlot(BattleFx_SpecialFireballAnimatedSpriteTask)]);
        frame.childSlot->posX = frame.slot->posX + rand() % 800 + -400;
        frame.childSlot->posY = frame.slot->posY + rand() % 800 + -400;
        frame.childSlot->posZ = frame.slot->posZ + rand() % 800 + -400;
        frame.childSlot->spriteVariant = 0;
        frame.slot->ownerSlot = BATTLE_TASK_FREE;
    }
}
} /* extern "C" */
