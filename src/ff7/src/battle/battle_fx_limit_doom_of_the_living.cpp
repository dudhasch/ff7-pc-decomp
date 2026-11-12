// src/ff7/src/battle/battle_fx_limit_doom_of_the_living.cpp
// Limit Break [54] (Yuffie Lv 3-2): Doom of the Living (Japanese: 生者必滅, dev tag: "LIMIT2/HITUMETU" / Hitumetu)
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Hitumetu limit effect: loads its package and runs a master timeline that sequences
//   attacks, sprites, pulsing model pairs and a screen effect around the target's centre.
/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/types/battle.h"
#include "ff7/types/battle_limit.h"
#include "ff7/battle_tasks.h"
#include "lib/types/graphics.h"
#include "lib/codegen.h"
#include "lib/types/polygon.h"
#include "ff7/camera_script.h"
#include <stdlib.h>
#include <string.h>
#include "ff7/api/battle.h"
#include "ff7/api/menu.h"
#include "lib/api/sound.h"
#include "lib/api/graphics.h"
extern "C" {

#pragma intrinsic(memset)

// clang-format off
// GLOBAL: FF7 0x00806210
unsigned char g_BattleLimitDoomOfTheLivingSoundIds[16] = {0x55, 0x01, 0x00, 0x00, 0x42, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x00806220
unsigned int g_BattleLimitDoomOfTheLivingSoundGroups[2] = {(unsigned int)&g_BattleLimitDoomOfTheLivingSoundIds, 0x00000000};
// GLOBAL: FF7 0x00806228
unsigned int g_BattleLimitDoomOfTheLivingResourceTable[48] = {
    0x003200b1,
    (unsigned int)&g_BattleLimitDoomOfTheLivingHitRsdName,
    0x00000000,
    0x00000000,
    0x00000000,
    0x003200b1,
    (unsigned int)&g_BattleLimitDoomOfTheLivingHikariRsdName,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x008063a4
    (unsigned int)"chi_1.tim",
    // STRING: FF7 0x008063b0
    (unsigned int)"chi_1.spt",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x008063bc
    (unsigned int)"chi_2.tim",
    // STRING: FF7 0x008063c8
    (unsigned int)"chi_2.spt",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x008063d4
    (unsigned int)"chi_3.tim",
    // STRING: FF7 0x008063e0
    (unsigned int)"chi_3.spt",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x008063ec
    (unsigned int)"chi_4.tim",
    // STRING: FF7 0x008063f8
    (unsigned int)"chi_4.spt",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x00806404
    (unsigned int)"tama.tim",
    // STRING: FF7 0x00806410
    (unsigned int)"tama1.spt",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x0080641c
    (unsigned int)"tama.tim",
    // STRING: FF7 0x00806428
    (unsigned int)"tama2.spt",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x00806434
    (unsigned int)"snow.tim",
    // STRING: FF7 0x00806440
    (unsigned int)"snow1.spt",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x0080644c
    (unsigned int)"snow.tim",
    // STRING: FF7 0x00806458
    (unsigned int)"snow2.spt"};
unsigned char g_rawData_008062e8[40] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x2e, 0x00, 0x00, 0x00, 0x00,
    0x06, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00
};
// GLOBAL: FF7 0x00806310
struct BattleLimitDoomOfTheLivingSpriteRequest1 g_BattleLimitDoomOfTheLivingSpriteRequest1 = {0, 0, {0x80, 0x80, 0x80, 0x2e}, 0};
// GLOBAL: FF7 0x00806320
unsigned char g_BattleLimitDoomOfTheLivingAnimIndices[16] = {0x02, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x00806330
struct BattleLimitDoomOfTheLivingSpriteRequest2 g_BattleLimitDoomOfTheLivingSpriteRequest2 = {0, 0, -128, -128, -128, {0x2e}, 0};
// GLOBAL: FF7 0x00806340
struct BattleLimitDoomOfTheLivingModelRequest g_BattleLimitDoomOfTheLivingModelRequest = {0, {0xa8, 0, 0, 0, 0, 0}, 128, {0x20, 0}};
// GLOBAL: FF7 0x00806350
struct BattleLimitDoomOfTheLivingTimingTable1 g_BattleLimitDoomOfTheLivingTimingTable1 = {20, 24, {0x1f, 0, 0x27, 0, 0x2f, 0, 0x39, 0, 0x40, 0, 0x50, 0, 0x58, 0, 0x60, 0, 0x71, 0, 0x78, 0, 0x88, 0, 0x90, 0, 0x98, 0, 0xa9, 0}};
// GLOBAL: FF7 0x00806370
unsigned char g_BattleLimitDoomOfTheLivingTimingTable2[30] = {0x16, 0x00, 0x1b, 0x00, 0x24, 0x00, 0x2b, 0x00, 0x36, 0x00, 0x3c, 0x00, 0x47, 0x00, 0x55, 0x00, 0x5d, 0x00, 0x69, 0x00, 0x74, 0x00, 0x7f, 0x00, 0x8d, 0x00, 0x95, 0x00, 0xa1, 0x00};
// GLOBAL: FF7 0x00806390
unsigned char g_BattleLimitDoomOfTheLivingHitRsdName[8] = "hit.rsd";
// GLOBAL: FF7 0x00806398
unsigned char g_BattleLimitDoomOfTheLivingHikariRsdName[11] = "hikari.rsd";
// GLOBAL: FF7 0x00aef748
struct BattleLimitHighwindLoadedResTable g_BattleLimitDoomOfTheLivingAttackerPos = {0};
// GLOBAL: FF7 0x00aef74c
short g_BattleLimitDoomOfTheLivingAttackerZ = 0;
// GLOBAL: FF7 0x00aef750
short g_BattleLimitDoomOfTheLivingAttackerActorIdx = 0;
// GLOBAL: FF7 0x00aef754
short g_BattleLimitDoomOfTheLivingTargetActorIdx = 0;
// GLOBAL: FF7 0x00aef758
int g_BattleLimitDoomOfTheLivingTargetCentroid = 0;
unsigned char g_bssPad_00aef75c[4] = {0};
// GLOBAL: FF7 0x00aef760
int g_BattleLimitDoomOfTheLivingRenderSlot = 0;
// GLOBAL: FF7 0x00aef764
short g_BattleLimitDoomOfTheLivingTargetMask = 0;
// GLOBAL: FF7 0x00aef768
short g_BattleLimitDoomOfTheLivingCountdown = 0;
unsigned char g_bssPad_00aef76c[4] = {0};
// GLOBAL: FF7 0x00aef770
struct BattleLimitHighwindLoadedResTable g_BattleLimitDoomOfTheLivingTargetPos = {0};
// GLOBAL: FF7 0x00aef774
short g_BattleLimitDoomOfTheLivingTargetZ = 0;
// GLOBAL: FF7 0x00aef778
short g_BattleLimitDoomOfTheLivingSequenceIdx = 0;
// GLOBAL: FF7 0x00aef77c
struct GfxTableCursor *g_BattleLimitDoomOfTheLivingLoadedResTable = 0;
// GLOBAL: FF7 0x00aef780
short g_BattleLimitDoomOfTheLivingModelRotAngles[3] = {0};
// clang-format on
// The TU's own view of moved definitions declared with another type.

#pragma intrinsic(memcpy)
#pragma function(memcpy)

/*
 * Loads the "HITUMETU" resource package and registers sound groups.
 */
// FUNCTION: FF7 0x004ace20
void __cdecl BattleFx_LimitLoadResDoomOfTheLiving(void)
{
    GfxModelTableBank *sceneField;
    GfxResourceRequest requestRecord;
    int kind;
    char pathBuffer[200];
    GfxResourceRequest *requestPtr;
    int resourceTable;

    if (g_pBattle3dScene != 0 && g_BattleLimitDoomOfTheLivingLoadedResTable == 0) {
        kind = 0xA;
        resourceTable = (int)&g_BattleLimitDoomOfTheLivingResourceTable;
        sceneField = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        requestPtr = &requestRecord;
        Gfx_InitResourceRequest(requestPtr);
        requestPtr->groupTag = 0;
        // STRING: FF7 0x00806464
        requestPtr->resourcePath = BattleFx_BuildResourcePath("LIMIT2/HITUMETU", pathBuffer);
        Gfx_LoadModelTableIfAbsent(kind, resourceTable, requestPtr, &g_BattleLimitDoomOfTheLivingLoadedResTable, sceneField);
        SfxRegisterSoundGroups((struct SoundGroup **)&g_BattleLimitDoomOfTheLivingSoundGroups);
    }
}

/*
 * Entry point for Hitumetu limit effect; initializes state and returns zero.
 */
// FUNCTION: FF7 0x004acec1
int __cdecl BattleFx_LimitEnterDoomOfTheLiving(int targetMask, int actorIdx)
{
    BattleFx_LimitBeginDoomOfTheLiving(targetMask, actorIdx);
    g_BattleLimitDoomOfTheLivingCountdown = 0;
    g_BattleLimitDoomOfTheLivingSequenceIdx = g_BattleLimitDoomOfTheLivingCountdown;
    return 0;
}

/*
 * Sets up Hitumetu limit context, calculates target centroid, and spawns timeline.
 */
// FUNCTION: FF7 0x004aceef
void __cdecl BattleFx_LimitBeginDoomOfTheLiving(int targetMask, int actorIdx)
{
    struct BattleTaskSlot *slot;

    BattleFx_LimitLoadResDoomOfTheLiving();
    g_BattleLimitDoomOfTheLivingTargetMask = targetMask;
    Battle_GetActorMaskCentroid(targetMask, (short *)&g_BattleLimitDoomOfTheLivingTargetCentroid);
    slot = BATTLE_NEW_TASK_SLOT(BattleFx_LimitDoomOfTheLivingTimelineTask);
    ((BattleFxLimitBloodfestTaskSlot *)slot)->actorIndex = actorIdx;
    g_BattleFxTimelineState.channel1 = 0;
    g_BattleFxTimelineState.red = g_BattleFxTimelineState.channel1;
    g_BattleFxTimelineState.channel2 = 0x40;
    Battle_StartFixedPointRamp(0x1000, 0xA);
}

/*
 * Master timeline task coordinating sequential attacks, particles, models, and screen effects.
 */
// FUNCTION: FF7 0x004acf67
void __cdecl BattleFx_LimitDoomOfTheLivingTimelineTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[72];
        int depth;
        struct { unsigned char pad_to_4[0x4]; SVECTOR rotX; };
        struct { unsigned char pad_to_C[0xC]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_2C[0x2C]; BattleDoomOfTheLivingTimelineSlot *spriteSlot; };
        struct { unsigned char pad_to_34[0x34]; BattleFxDoomOfTheLivingRecord *hitRec; };
        struct { unsigned char pad_to_38[0x38]; short particleIndex; };
        struct { unsigned char pad_to_3C[0x3C]; short orbitRadius; };
        struct { unsigned char pad_to_40[0x40]; BattleFxLimitBloodfestTaskSlot *hitumetuSlot; };
        struct { unsigned char pad_to_44[0x44]; short yaw; };
    } frame;
#pragma pack(pop)

    frame.hitumetuSlot = (BattleFxLimitBloodfestTaskSlot *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (g_BattleLimitDoomOfTheLivingSequenceIdx < 0xF && frame.hitumetuSlot->step == (&(g_BattleLimitDoomOfTheLivingTimingTable1.startStep))[g_BattleLimitDoomOfTheLivingSequenceIdx]) {
        g_BattleLimitDoomOfTheLivingAttackerActorIdx = frame.hitumetuSlot->actorIndex;
        g_BattleLimitDoomOfTheLivingTargetActorIdx = g_BattleHitTargetActors[g_BattleLimitDoomOfTheLivingSequenceIdx];
        (g_BattleLimitDoomOfTheLivingTargetPos.posX) = g_BattleActorRecords[g_BattleLimitDoomOfTheLivingTargetActorIdx].posX;
        g_BattleLimitDoomOfTheLivingTargetPos.posY = g_BattleActorRecords[g_BattleLimitDoomOfTheLivingTargetActorIdx].posY;
        g_BattleLimitDoomOfTheLivingTargetZ = g_BattleActorRecords[g_BattleLimitDoomOfTheLivingTargetActorIdx].posZ;
        frame.yaw = (rand() % 4096);
        frame.orbitRadius = g_BattleActorRecords[g_BattleLimitDoomOfTheLivingTargetActorIdx].radius + 0x5DC;
        (g_BattleLimitDoomOfTheLivingAttackerPos.posX) = (g_BattleLimitDoomOfTheLivingTargetPos.posX) + (SinFixed(frame.yaw) * frame.orbitRadius >> 0xC);
        g_BattleLimitDoomOfTheLivingAttackerPos.posY = 0;
        g_BattleLimitDoomOfTheLivingAttackerZ = g_BattleLimitDoomOfTheLivingTargetZ + (CosFixed(frame.yaw) * frame.orbitRadius >> 0xC);
        g_BattleActorRecords[frame.hitumetuSlot->actorIndex].posX = (g_BattleLimitDoomOfTheLivingAttackerPos.posX);
        g_BattleActorRecords[frame.hitumetuSlot->actorIndex].posY = g_BattleLimitDoomOfTheLivingAttackerPos.posY;
        g_BattleActorRecords[frame.hitumetuSlot->actorIndex].posZ = g_BattleLimitDoomOfTheLivingAttackerZ;
        g_BattleActorRecords[frame.hitumetuSlot->actorIndex].rotX = 0;
        g_BattleActorRecords[frame.hitumetuSlot->actorIndex].yaw = frame.yaw;
        g_BattleActorRecords[frame.hitumetuSlot->actorIndex].rotZ = 0;
        g_BattleLimitDoomOfTheLivingCountdown = ((short *)&g_BattleLimitDoomOfTheLivingTimingTable2)[g_BattleLimitDoomOfTheLivingSequenceIdx] - (&(g_BattleLimitDoomOfTheLivingTimingTable1.startStep))[g_BattleLimitDoomOfTheLivingSequenceIdx];
        Battle_CreateEffectTrailTask(g_BattleLimitDoomOfTheLivingAttackerActorIdx, g_BattleActorRecords[g_BattleLimitDoomOfTheLivingAttackerActorIdx].primaryBone, 0xC8, 0x208020, 4, (&g_BattleLimitDoomOfTheLivingTimingTable1.endStep)[g_BattleLimitDoomOfTheLivingSequenceIdx] - (&(g_BattleLimitDoomOfTheLivingTimingTable1.startStep))[g_BattleLimitDoomOfTheLivingSequenceIdx]);
    }
    if (g_BattleLimitDoomOfTheLivingCountdown > 0) {
        g_BattleLimitDoomOfTheLivingCountdown--;
        if (g_BattleLimitDoomOfTheLivingCountdown == 0) {
            frame.hitRec = (BattleFxDoomOfTheLivingRecord *)Battle_AllocSceneMessage(0xA);
            frame.hitRec->targetActorIndex = g_BattleLimitDoomOfTheLivingTargetActorIdx;
            frame.hitRec->sequenceIndex = g_BattleLimitDoomOfTheLivingSequenceIdx;
            BattleFx_HitMarkSpawnScriptAtActorBone(g_BattleLimitDoomOfTheLivingTargetActorIdx, 0x23, 0xC00);
            frame.spriteSlot = (BattleDoomOfTheLivingTimelineSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitDoomOfTheLivingAnimatedSpriteTask);
            Battle_GetBoneWorldPos(g_BattleLimitDoomOfTheLivingTargetActorIdx, g_BattleActorRecords[g_BattleLimitDoomOfTheLivingTargetActorIdx].targetBone, (short *)&frame.spriteSlot->posX);
            frame.spriteSlot->variant = rand() % 3;
            for (frame.particleIndex = 0; frame.particleIndex < 0x1E; frame.particleIndex++) {
                frame.spriteSlot = (BattleDoomOfTheLivingTimelineSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitDoomOfTheLivingFadingParticleSpriteTask);
                Battle_GetBoneWorldPos(g_BattleLimitDoomOfTheLivingTargetActorIdx, g_BattleActorRecords[g_BattleLimitDoomOfTheLivingTargetActorIdx].targetBone, (short *)&frame.spriteSlot->posX);
                (g_BattleLimitDoomOfTheLivingTargetPos.posX) = 0;
                g_BattleLimitDoomOfTheLivingTargetPos.posY = 0;
                g_BattleLimitDoomOfTheLivingTargetZ = rand() % 0xC8 + 0xC8;
                frame.rotX.vx = (rand() % 4096);
                frame.rotX.vy = (rand() % 4096);
                frame.rotX.vz = 0;
                RotMatrixZYX(&frame.rotX, &frame.rotMatrix);
                frame.rotMatrix.t[2] = 0;
                frame.rotMatrix.t[1] = frame.rotMatrix.t[2];
                frame.rotMatrix.t[0] = frame.rotMatrix.t[1];
                SetRotMatrix(&frame.rotMatrix);
                SetTransMatrix(&frame.rotMatrix);
                RotTransPointToInt16((short *)&g_BattleLimitDoomOfTheLivingTargetPos, (short *)&frame.spriteSlot->velocityX, &frame.depth);
                frame.spriteSlot->variant = rand() % 3;
            }
            frame.spriteSlot = (BattleDoomOfTheLivingTimelineSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitDoomOfTheLivingSpawnPulsingModelPairTask);
            Battle_GetBoneWorldPos(g_BattleLimitDoomOfTheLivingTargetActorIdx, g_BattleActorRecords[g_BattleLimitDoomOfTheLivingTargetActorIdx].targetBone, (short *)&frame.spriteSlot->posX);
            frame.spriteSlot->posY = 0;
            SfxPlaySoundGroup(Battle_ComputeScreenPanFromPosition((short *)&g_BattleLimitDoomOfTheLivingTargetPos), 0);
            g_BattleOverlayDrawListHead = BattleFx_FlashScreenWhite(0x80, 0x80, 0x80);
            g_BattleLimitDoomOfTheLivingSequenceIdx++;
        }
    }
    (frame.hitumetuSlot->step)++;
    if (frame.hitumetuSlot->step == 0xAA) {
        Battle_StartFixedPointRamp(0, 0xA);
        frame.hitumetuSlot->ownerSlot = BATTLE_TASK_FREE;
        Sfx_ReleaseCachedBuffer(0x155, 0xF);
        Sfx_ReleaseCachedBuffer(0x142, 0xF);
    }
}

/*
 * Animates multi-frame billboard hit sprite at target bone location.
 */
// FUNCTION: FF7 0x004ad4b6
void __cdecl BattleFx_LimitDoomOfTheLivingAnimatedSpriteTask(void)
{
    BattleFxDoomOfTheLivingAnimSprite *task;

    task = (BattleFxDoomOfTheLivingAnimSprite *)BATTLE_CURRENT_TASK_SLOT;
    (g_BattleLimitDoomOfTheLivingSpriteRequest1.spriteGroup) = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(2, g_BattleLimitDoomOfTheLivingLoadedResTable);
    g_BattleLimitDoomOfTheLivingSpriteRequest1.req1Res = Gfx_ModelTableGetSpriteObject(2, g_BattleLimitDoomOfTheLivingLoadedResTable);
    BattleFx_InstallBillboardMatrix(&task->posX, 0x3000, 0);
    (g_BattleLimitDoomOfTheLivingSpriteRequest1.spriteGroup) = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(((int *)&g_BattleLimitDoomOfTheLivingAnimIndices)[task->animIndex], g_BattleLimitDoomOfTheLivingLoadedResTable);
    g_BattleLimitDoomOfTheLivingSpriteRequest1.req1Res = Gfx_ModelTableGetSpriteObject(((int *)&g_BattleLimitDoomOfTheLivingAnimIndices)[task->animIndex], g_BattleLimitDoomOfTheLivingLoadedResTable);
    g_BattleLimitDoomOfTheLivingSpriteRequest1.req1Frame = task->frame;
    g_BattleOverlayDrawListHead = BattleFx_SubmitSpriteDraw(&(g_BattleLimitDoomOfTheLivingSpriteRequest1.spriteGroup), g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused == 0 && (g_BattleLimitDoomOfTheLivingSpriteRequest1.spriteGroup) != 0) {
        (task->frame)++;
        if (task->frame >= g_BattleLimitDoomOfTheLivingSpriteRequest1.spriteGroup->header->frameCount) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Animates fading billboard particle sprites bursting outward with velocity deltas.
 */
// FUNCTION: FF7 0x004ad5cd
void __cdecl BattleFx_LimitDoomOfTheLivingFadingParticleSpriteTask(void)
{
    BattleFxDoomOfTheLivingParticle *task;

    task = (BattleFxDoomOfTheLivingParticle *)BATTLE_CURRENT_TASK_SLOT;
    (g_BattleLimitDoomOfTheLivingSpriteRequest2.spriteGroup) = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(3, g_BattleLimitDoomOfTheLivingLoadedResTable);
    g_BattleLimitDoomOfTheLivingSpriteRequest2.req2Res = Gfx_ModelTableGetSpriteObject(3, g_BattleLimitDoomOfTheLivingLoadedResTable);
    BattleFx_InstallBillboardMatrix(&task->posX, 0x2000, 0);
    g_BattleLimitDoomOfTheLivingSpriteRequest2.req2Short12 = task->spriteIndex;
    g_BattleLimitDoomOfTheLivingSpriteRequest2.req2Alpha = 0x80 - (task->frame << 4);
    g_BattleLimitDoomOfTheLivingSpriteRequest2.req2AlphaOld1 = g_BattleLimitDoomOfTheLivingSpriteRequest2.req2Alpha;
    g_BattleLimitDoomOfTheLivingSpriteRequest2.req2AlphaOld2 = g_BattleLimitDoomOfTheLivingSpriteRequest2.req2AlphaOld1;
    g_BattleOverlayDrawListHead = BattleFx_SubmitSpriteDraw(&(g_BattleLimitDoomOfTheLivingSpriteRequest2.spriteGroup), g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused == 0) {
        task->posX += task->velocityX;
        task->posY += task->velocityY;
        task->posZ += task->velocityZ;
        (task->frame)++;
        if (task->frame == 8) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Spawns pulsing model pair task at the given world position.
 */
// FUNCTION: FF7 0x004ad6f9
void __cdecl BattleFx_LimitDoomOfTheLivingSpawnPulsingModelPairTask(void)
{
    int stackWords[2]; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxLimitPulseModelSlot *task;
    BattleFxPositionedSpriteSlot *child;

    task = (BattleFxLimitPulseModelSlot *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (task->timer == 0) {
        child = (BattleFxPositionedSpriteSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitDoomOfTheLivingPulsingModelPairTask);
        child->posX = task->posX;
        child->posY = task->posY;
        child->posZ = task->posZ;
    }
    (task->timer)++;
    if (task->timer == 0xC) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Animates a pair of pulsing, rotating 3D models with sinusoidal scaling.
 */
// FUNCTION: FF7 0x004ad79a
void __cdecl BattleFx_LimitDoomOfTheLivingPulsingModelPairTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[96];
        PsxMatrix rotMatrix;
        struct { unsigned char pad_to_2C[0x2C]; BattleFxDoomOfTheLivingPulseRecord *pulseRecord; };
        struct { unsigned char pad_to_30[0x30]; PsxMatrix drawMatrix; };
        struct { unsigned char pad_to_50[0x50]; VECTOR scaleX; };
    } frame;
#pragma pack(pop)

    frame.pulseRecord = (BattleFxDoomOfTheLivingPulseRecord *)BATTLE_CURRENT_TASK_SLOT;
    (((BattleFxTimelineState *)&g_BattleLimitDoomOfTheLivingModelRotAngles)->wheelAngle) = frame.pulseRecord->phase << 9;
    RotMatrixXYZ(&g_BattleLimitDoomOfTheLivingModelRotAngles, &frame.rotMatrix);
    frame.scaleX.vz = SinFixed(frame.pulseRecord->phase * 0x66) * 0x7F00 >> 0xC;
    frame.scaleX.vx = frame.scaleX.vz;
    frame.scaleX.vy = 0x1800 - (SinFixed(frame.pulseRecord->phase * 0x66) << 0xC >> 0xC);
    ScaleMatrix(&frame.rotMatrix, &frame.scaleX);
    frame.rotMatrix.t[0] = frame.pulseRecord->posX;
    frame.rotMatrix.t[1] = frame.pulseRecord->posY;
    frame.rotMatrix.t[2] = frame.pulseRecord->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.drawMatrix);
    SetRotMatrix(&frame.drawMatrix);
    SetTransMatrix(&frame.drawMatrix);
    SetDrawColorRGB(0, 0, 0);
    g_BattleLimitDoomOfTheLivingModelRequest.reqPhaseAngle = frame.pulseRecord->phase * 0x199;
    (g_BattleLimitDoomOfTheLivingModelRequest.modelGroup) = Gfx_ModelTableGetRenderObject(1, g_BattleLimitDoomOfTheLivingLoadedResTable);
    g_BattleLimitDoomOfTheLivingRenderSlot = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&(g_BattleLimitDoomOfTheLivingModelRequest.modelGroup), g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleLimitDoomOfTheLivingRenderSlot);
    (g_BattleLimitDoomOfTheLivingModelRequest.modelGroup) = Gfx_ModelTableGetRenderObject(0, g_BattleLimitDoomOfTheLivingLoadedResTable);
    g_BattleLimitDoomOfTheLivingRenderSlot = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&(g_BattleLimitDoomOfTheLivingModelRequest.modelGroup), g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleLimitDoomOfTheLivingRenderSlot);
    if (g_BattleIsPaused != 0) {
        return;
    }
    (frame.pulseRecord->phase)++;
    if (frame.pulseRecord->phase == 0xA) {
        frame.pulseRecord->ownerSlot = BATTLE_TASK_FREE;
    }
}
} /* extern "C" */
