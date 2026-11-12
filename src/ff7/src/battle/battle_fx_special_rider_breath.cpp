// src/ff7/src/battle/battle_fx_special_rider_breath.cpp
/* Enemy Special Effect [idx 57]: RiderBreath (JP: koudan2 / 光弾2 (Light Bullet 2), asset: "SPECIAL/KOUDAN2") — Rider Breath (0x311, Dragon Rider), Four Laser (0x22e) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle special effect 'Koudan2': a per-target timeline with growing and variant
//   sprites, trail sprites and an expanding model.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/api/battle.h"
#include "ff7/battle_tasks.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_3d.h"
#include "ff7/types/battle_special.h"
#include "ff7/camera_script.h"
#include "lib/types/graphics.h"
#include <string.h>
#include <stdlib.h>
#include "lib/codegen.h"
extern "C" {

struct GfxRotationAngles2;
struct BattleFxModelDraw;
#pragma intrinsic(memset)
#pragma intrinsic(memcpy)

// clang-format off
// GLOBAL: FF7 0x0089f5f0
unsigned int g_BattleSpecRiderBreathResourceDefs[95] = {0x001900b1, (unsigned int)"HIKARI.RSD", 0x00000000, 0x00000000, 0x00000000, 0x00192a39, (unsigned int)"KOUDAN00.TIM", (unsigned int)"MARU1.SPT", 0x00000000, 0x00000000, 0x00192a39, (unsigned int)"KOUDAN03.TIM", (unsigned int)"HOSI1.SPT", 0x00000000, 0x00000000, 0x00192a39, (unsigned int)"KOUDAN01.TIM", (unsigned int)"WA1.SPT", 0x00000000, 0x00000000, 0x00192a39, (unsigned int)"KOUDAN01.TIM", (unsigned int)"BOKASI.SPT", 0x00000000, 0x00000000, 0x00192a39, (unsigned int)"KOUDAN00.TIM", (unsigned int)"MARU2.SPT", 0x00000000, 0x00000000, 0x00192a39, (unsigned int)"KOUDAN00.TIM", (unsigned int)"MARU3.SPT", 0x00000000, 0x00000000, 0x00192a39, (unsigned int)"KOUDAN00.TIM", (unsigned int)"MARU4.SPT", 0x00000000, 0x00000000, 0x00192a39, (unsigned int)"KOUDAN00.TIM", (unsigned int)"MARU5.SPT", 0x00000000, 0x00000000, 0x00192a39, (unsigned int)"KOUDAN00.TIM", (unsigned int)"MARU6.SPT", 0x00000000, 0x00000000, 0x00192a39, (unsigned int)"KOUDAN00.TIM", (unsigned int)"MARU7.SPT", 0x00000000, 0x00000000, 0x00192a39, (unsigned int)"KOUDAN00.TIM", (unsigned int)"MARU8.SPT", 0x00000000, 0x00000000, 0x00192a39, (unsigned int)"KOUDAN00.TIM", (unsigned int)"MARU9.SPT", 0x00000000, 0x00000000, 0x00192a39, (unsigned int)"KOUDAN00.TIM", (unsigned int)"MARU10.SPT", 0x00000000, 0x00000000, 0x00192a39, (unsigned int)"KOUDAN01.TIM", (unsigned int)"WA2.SPT", 0x00000000, 0x00000000, 0x00192a39, (unsigned int)"KOUDAN02.TIM", (unsigned int)"KOU1.SPT", 0x00000000, 0x00000000, 0x00192a39, (unsigned int)"KOUDAN02.TIM", (unsigned int)"HIKARI1.SPT", 0x00000000, 0x00000000, 0x00192a39, (unsigned int)"KOUDAN02.TIM", (unsigned int)"HIKARI2.SPT", 0x00000000, 0x00000000, 0x00192a39, (unsigned int)"KOUDAN02.TIM", (unsigned int)"HIKARI3.SPT", 0x00000000, 0x00000000};
// GLOBAL: FF7 0x0089f770
BattleFxSpriteRequest g_BattleSpecRiderBreathSpriteReq1 = {0, 0, -128, -128, -128, 0x2e, 0, 0};
// GLOBAL: FF7 0x0089f780
BattleFxSpriteRequest g_BattleSpecRiderBreathSpriteReq2 = {0, 0, (char)0x80, (char)0x80, (char)0x80, 0x2e, 0, 0};
// GLOBAL: FF7 0x0089f790
BattleFxSpriteRequest g_BattleSpecRiderBreathSpriteReq3 = {0, 0, (char)0x80, (char)0x80, (char)0x80, 0x2c, 0, 0};

// GLOBAL: FF7 0x0089f7a0
BattleFxModelDrawHead g_BattleSpecRiderBreathModelReq = {0, 0xa8, 0, 128, 0x20, 0};
// GLOBAL: FF7 0x0089f7b0
struct GfxRotationAngles2 g_BattleSpecRiderBreathRotAngles = {-1024, 0, 0};
// GLOBAL: FF7 0x00b5e440
int g_BattleSpecRiderBreathPacketCursor = 0;
// GLOBAL: FF7 0x00b5e444
struct GfxTableCursor *g_BattleSpecRiderBreathLoadedResTable = 0;
unsigned char g_bssPad_00b5e448[24] = {0};
// clang-format on


/*
 * Loads the SPECIAL/KOUDAN2 model table (resource type 0x13, from g_BattleSpecRiderBreathResourceDefs)
 * once the battle scene exists and while g_BattleSpecRiderBreathLoadedResTable is still empty.
 */
// FUNCTION: FF7 0x0052b920
void __cdecl BattleFx_SpecialLoadResRiderBreath(void)
{
    GfxModelTableBank *curOwnerCtx;
    GfxResourceRequest currentRecord;
    int kind;
    char pathText[200];
    GfxResourceRequest *pRequest;
    int resourceDefsWord;

    if (g_pBattle3dScene != 0 && g_BattleSpecRiderBreathLoadedResTable == 0) {
        kind = 0x13;
        resourceDefsWord = (int)&g_BattleSpecRiderBreathResourceDefs;
        curOwnerCtx = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRequest = &currentRecord;
        Gfx_InitResourceRequest(pRequest);
        pRequest->groupTag = 0;
        // STRING: FF7 0x0089f9b4
        pRequest->resourcePath = BattleFx_BuildResourcePath("SPECIAL/KOUDAN2", pathText);
        Gfx_LoadModelTableIfAbsent(kind, resourceDefsWord, pRequest, &g_BattleSpecRiderBreathLoadedResTable, curOwnerCtx);
    }
}

/*
 * Effect entry; forwards both actors to BattleFx_SpecialBeginRiderBreath.
 */
// FUNCTION: FF7 0x0052b9b0
void __cdecl BattleFx_SpecialEnterRiderBreath(int targetMask, int actorIdx)
{
    BattleFx_SpecialBeginRiderBreath(targetMask, actorIdx);
}

/*
 * Loads the resources, starts the per-target spawner task (interval 4) and plays sound 0x188
 * panned to the target.
 */
// FUNCTION: FF7 0x0052b9c5
void __cdecl BattleFx_SpecialBeginRiderBreath(int targetMask, int actorIdx)
{
    BattleFx_SpecialLoadResRiderBreath();
    Battle_CreateBitPatternCallbackTask(targetMask, actorIdx, 4, BattleFx_SpecialSpawnRiderBreathPerTarget);
    Battle_PlaySfxPannedToActor(0x188, (short)actorIdx, 0x40);
}

/*
 * Starts a timeline task for one actor: it begins at the bone33 position of targetActor and
 * aims at the targetBone position of actor, which it records as its actorIndex.
 */
// FUNCTION: FF7 0x0052b9fa
void __cdecl BattleFx_SpecialSpawnRiderBreathPerTarget(int actor, int targetActor)
{
    BattleFxDarkWaveTimelineSlot *taskSlot;

    taskSlot = (BattleFxDarkWaveTimelineSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialRiderBreathTimelineTask);
    Battle_GetBoneWorldPos(targetActor, g_BattleActorRecords[targetActor].bone33, (short *)&taskSlot->posX);
    Battle_GetBoneWorldPos(actor, g_BattleActorRecords[actor].targetBone, (short *)&taskSlot->velocityX);
    taskSlot->actorIndex = actor;
}

/*
 * Timeline task: spawns trail sprites for the first 9 frames, the expanding model at frame 0x19,
 * and bursts of variant and growing sprites around the target; frees itself at frame 0x32.
 */
// FUNCTION: FF7 0x0052ba70
void __cdecl BattleFx_SpecialRiderBreathTimelineTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[84];
        SVECTOR rotX;
        struct { unsigned char pad_to_8[0x8]; int depth; };
        struct { unsigned char pad_to_C[0xC]; SVECTOR offsetX; };
        struct { unsigned char pad_to_14[0x14]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_34[0x34]; BattleFxRiderBreathTimelineSpawnSlot *spawnSlot; };
        struct { unsigned char pad_to_3C[0x3C]; short burstIndex; };
        struct { unsigned char pad_to_40[0x40]; BattleFxDarkWaveTimelineSlot *timelineSlot; };
        struct { unsigned char pad_to_50[0x50]; short angle; };
    } frame;
#pragma pack(pop)

    frame.timelineSlot = (BattleFxDarkWaveTimelineSlot *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.timelineSlot->frameCounter == 0) {
        g_BattleFxTimelineState.channel2 = 0;
        g_BattleFxTimelineState.channel1 = g_BattleFxTimelineState.channel2;
        g_BattleFxTimelineState.red = g_BattleFxTimelineState.channel1;
        Battle_StartFixedPointRamp(0x1000, 10);
    }
    if (frame.timelineSlot->frameCounter < 9 && frame.timelineSlot->frameCounter % 3 == 0) {
        frame.spawnSlot = (BattleFxRiderBreathTimelineSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialRiderBreathTrailSpritesTask);
        frame.spawnSlot->posX = frame.timelineSlot->posX;
        frame.spawnSlot->posY = frame.timelineSlot->posY;
        frame.spawnSlot->posZ = frame.timelineSlot->posZ;
        frame.spawnSlot->velocityX = (frame.timelineSlot->velocityX - frame.timelineSlot->posX) / 15;
        frame.spawnSlot->velocityY = (frame.timelineSlot->velocityY - frame.timelineSlot->posY) / 15;
        frame.spawnSlot->velocityZ = (frame.timelineSlot->velocityZ - frame.timelineSlot->posZ) / 15;
    }
    if (frame.timelineSlot->frameCounter == 20) {
        Battle_PostActorHitOrHurtMessage(frame.timelineSlot->actorIndex);
    }
    if (frame.timelineSlot->frameCounter == 25) {
        frame.spawnSlot = (BattleFxRiderBreathTimelineSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialRiderBreathExpandingModelTask);
        Battle_GetBoneWorldPos(frame.timelineSlot->actorIndex, g_BattleActorRecords[frame.timelineSlot->actorIndex].targetBone, (short *)&frame.spawnSlot->posX);
        Battle_PlaySfxPannedToActor(0x189, (short)frame.timelineSlot->actorIndex, 0x40);
    }
    if (frame.timelineSlot->frameCounter > 18 && frame.timelineSlot->frameCounter < 38) {
        for (frame.burstIndex = 0; frame.burstIndex < 3; frame.burstIndex++) {
            frame.spawnSlot = (BattleFxRiderBreathTimelineSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialRiderBreathVariantSpriteTask);
            Battle_GetBoneWorldPos(frame.timelineSlot->actorIndex, g_BattleActorRecords[frame.timelineSlot->actorIndex].targetBone, (short *)&frame.spawnSlot->posX);
            frame.spawnSlot->posX += (short)(rand() % 1600 - 800);
            frame.spawnSlot->posY += (short)(rand() % 1600 - 800);
            frame.spawnSlot->posZ += (short)(rand() % 1600 - 800);
            frame.spawnSlot->variant = rand() % 3;
            frame.angle = (rand() % 4096);
            frame.offsetX.vx = CosFixed(frame.angle) * 0x78 >> 12;
            frame.offsetX.vy = SinFixed(frame.angle) * 0x78 >> 12;
            frame.offsetX.vz = 0;
            frame.rotX.vz = 0;
            frame.rotX.vx = frame.rotX.vz;
            frame.rotX.vy = (rand() % 4096);
            RotMatrix(&frame.rotX, &frame.rotMatrix);
            frame.rotMatrix.t[2] = 0;
            frame.rotMatrix.t[1] = frame.rotMatrix.t[2];
            frame.rotMatrix.t[0] = frame.rotMatrix.t[1];
            SetRotMatrix(&frame.rotMatrix);
            SetTransMatrix(&frame.rotMatrix);
            RotTransPointToInt16(&frame.offsetX, &frame.spawnSlot->velocityX, &frame.depth);
        }
    }
    if (frame.timelineSlot->frameCounter > 18 && frame.timelineSlot->frameCounter < 28) {
        for (frame.burstIndex = 0; frame.burstIndex < 3; frame.burstIndex++) {
            frame.spawnSlot = (BattleFxRiderBreathTimelineSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialRiderBreathGrowingSpriteTask);
            Battle_GetBoneWorldPos(frame.timelineSlot->actorIndex, g_BattleActorRecords[frame.timelineSlot->actorIndex].targetBone, (short *)&frame.spawnSlot->posX);
            frame.spawnSlot->posX += (short)(rand() % 1600 - 800);
            frame.spawnSlot->posY += (short)(rand() % 1600 - 800);
            frame.spawnSlot->posZ += (short)(rand() % 1600 - 800);
            frame.spawnSlot->variant = rand() % 7;
            frame.spawnSlot->scale = (rand() % 3 << 11) + 0x1000;
            frame.angle = (rand() % 4096);
            frame.offsetX.vx = CosFixed(frame.angle) * 0x3C >> 12;
            frame.offsetX.vy = SinFixed(frame.angle) * 0x3C >> 12;
            frame.offsetX.vz = 0;
            frame.rotX.vz = 0;
            frame.rotX.vx = frame.rotX.vz;
            frame.rotX.vy = (rand() % 4096);
            RotMatrix(&frame.rotX, &frame.rotMatrix);
            frame.rotMatrix.t[2] = 0;
            frame.rotMatrix.t[1] = frame.rotMatrix.t[2];
            frame.rotMatrix.t[0] = frame.rotMatrix.t[1];
            SetRotMatrix(&frame.rotMatrix);
            SetTransMatrix(&frame.rotMatrix);
            RotTransPointToInt16(&frame.offsetX, &frame.spawnSlot->velocityX, &frame.depth);
        }
    }
    if (frame.timelineSlot->frameCounter == 40) {
        Battle_StartFixedPointRamp(0, 10);
    }
    (frame.timelineSlot->frameCounter)++;
    if (frame.timelineSlot->frameCounter == 50) {
        frame.timelineSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

// Growing sprite task: draws animation entry 0xF of g_BattleSpecRiderBreathLoadedResTable at the
// slot's position with its angle and variant, pulsing the colour with SinFixed of the step. Unless
// paused, the angle grows by 0x180 a frame, the position integrates its velocity, and the slot is
// freed when step reaches 0x10.
// FUNCTION: FF7 0x0052c020
void __cdecl BattleFx_SpecialRiderBreathGrowingSpriteTask(void)
{
    BattleFxRiderBreathSprite *task;

    task = (BattleFxRiderBreathSprite *)BATTLE_CURRENT_TASK_SLOT;
    (g_BattleSpecRiderBreathSpriteReq1.spriteGroup) = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(15, g_BattleSpecRiderBreathLoadedResTable);
    g_BattleSpecRiderBreathSpriteReq1.drawObj = Gfx_ModelTableGetSpriteObject(15, g_BattleSpecRiderBreathLoadedResTable);
    BattleFx_InstallBillboardMatrix(&task->posX, (short)task->angle, 0);
    g_BattleSpecRiderBreathSpriteReq1.frame = task->variant;
    g_BattleSpecRiderBreathSpriteReq1.colorB = SinFixed(task->step << 7) * 0x50 >> 12;
    g_BattleSpecRiderBreathSpriteReq1.colorG = g_BattleSpecRiderBreathSpriteReq1.colorB;
    g_BattleSpecRiderBreathSpriteReq1.colorR = g_BattleSpecRiderBreathSpriteReq1.colorG;
    g_BattleSpecRiderBreathPacketCursor = BattleFx_SubmitSpriteDraw(&g_BattleSpecRiderBreathSpriteReq1, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecRiderBreathPacketCursor);
    if (g_BattleIsPaused == 0) {
        task->angle += 0x180;
        task->posX += task->velocityX;
        task->posY += task->velocityY;
        task->posZ += task->velocityZ;
        (task->step)++;
        if (task->step == 16) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

// Variant sprite task: draws animation entry 0x10, 0x11 or 0x12 (by the slot's variant) as a
// billboard at scale 0x2000. Unless paused, the position integrates its velocity; with a sprite
// loaded the frame index advances, and the slot is freed once it reaches the sprite group's
// frame count.
// FUNCTION: FF7 0x0052c16b
void __cdecl BattleFx_SpecialRiderBreathVariantSpriteTask(void)
{
    struct {
        int table[3];
        BattleFxRiderBreathVariantSprite *slot;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    frame.slot = (BattleFxRiderBreathVariantSprite *)BATTLE_CURRENT_TASK_SLOT;
    frame.table[0] = 0x10;
    frame.table[1] = 0x11;
    frame.table[2] = 0x12;
    BattleFx_InstallBillboardMatrix(&frame.slot->posA, 0x2000 /* 2.0 */, 0);
    g_BattleSpecRiderBreathSpriteReq2.frame = frame.slot->frameIndex;
    (g_BattleSpecRiderBreathSpriteReq2.spriteGroup) = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(frame.table[frame.slot->variant], g_BattleSpecRiderBreathLoadedResTable);
    g_BattleSpecRiderBreathSpriteReq2.drawObj = Gfx_ModelTableGetSpriteObject(frame.table[frame.slot->variant], g_BattleSpecRiderBreathLoadedResTable);
    g_BattleSpecRiderBreathPacketCursor = BattleFx_SubmitSpriteDraw(&g_BattleSpecRiderBreathSpriteReq2, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecRiderBreathPacketCursor);
    if (g_BattleIsPaused == 0) {
        frame.slot->posA += frame.slot->velA;
        frame.slot->posB += frame.slot->velB;
        frame.slot->posC += frame.slot->velC;
        if ((g_BattleSpecRiderBreathSpriteReq2.spriteGroup) != 0) {
            (frame.slot->frameIndex)++;
            if (frame.slot->frameIndex >= g_BattleSpecRiderBreathSpriteReq2.spriteGroup->header->frameCount) {
                frame.slot->ownerSlot = BATTLE_TASK_FREE;
            }
        }
    }
}

/*
 * Trail task: draws 11 billboards, each with its own animation entry, at the slot's position scaled
 * by (6 - index) / 6, then integrates the position; frees itself at frame 0xF.
 */
// FUNCTION: FF7 0x0052c2a5
void __cdecl BattleFx_SpecialRiderBreathTrailSpritesTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[20];
        SVECTOR scaledX;
        struct { unsigned char pad_to_8[0x8]; short trailIndex; };
        struct { unsigned char pad_to_C[0xC]; BattleFxHellBomberRocketSlot *trailSlot; };
    } frame;
#pragma pack(pop)

    frame.trailSlot = (BattleFxHellBomberRocketSlot *)BATTLE_CURRENT_TASK_SLOT;
    for (frame.trailIndex = 0; frame.trailIndex < 11; frame.trailIndex++) {
        frame.scaledX.vx = frame.trailSlot->posX * (6 - frame.trailIndex) / 6;
        frame.scaledX.vy = frame.trailSlot->posY * (6 - frame.trailIndex) / 6;
        frame.scaledX.vz = frame.trailSlot->posZ * (6 - frame.trailIndex) / 6;
        /* Numeric ribbon trail step index (0..11). */
        switch (frame.trailIndex) {
        case 0:
            (g_BattleSpecRiderBreathSpriteReq3.spriteGroup) = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(1, g_BattleSpecRiderBreathLoadedResTable);
            g_BattleSpecRiderBreathSpriteReq3.drawObj = Gfx_ModelTableGetSpriteObject(1, g_BattleSpecRiderBreathLoadedResTable);
            BattleFx_InstallBillboardMatrix(&frame.scaledX, 0x1000 /* 1.0 */, 0);
            g_BattleSpecRiderBreathPacketCursor = BattleFx_SubmitSpriteDraw(&g_BattleSpecRiderBreathSpriteReq3, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecRiderBreathPacketCursor);
            break;
        case 1:
            (g_BattleSpecRiderBreathSpriteReq3.spriteGroup) = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(2, g_BattleSpecRiderBreathLoadedResTable);
            g_BattleSpecRiderBreathSpriteReq3.drawObj = Gfx_ModelTableGetSpriteObject(2, g_BattleSpecRiderBreathLoadedResTable);
            BattleFx_InstallBillboardMatrix(&frame.scaledX, (SinFixed(frame.trailSlot->frameCounter << 10) << 11 >> 12) + 0x2000, 0);
            g_BattleSpecRiderBreathPacketCursor = BattleFx_SubmitSpriteDraw(&g_BattleSpecRiderBreathSpriteReq3, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecRiderBreathPacketCursor);
            (g_BattleSpecRiderBreathSpriteReq3.spriteGroup) = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(3, g_BattleSpecRiderBreathLoadedResTable);
            g_BattleSpecRiderBreathSpriteReq3.drawObj = Gfx_ModelTableGetSpriteObject(3, g_BattleSpecRiderBreathLoadedResTable);
            BattleFx_InstallBillboardMatrix(&frame.scaledX, 0x1000 /* 1.0 */, 0);
            g_BattleSpecRiderBreathPacketCursor = BattleFx_SubmitSpriteDraw(&g_BattleSpecRiderBreathSpriteReq3, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecRiderBreathPacketCursor);
            (g_BattleSpecRiderBreathSpriteReq3.spriteGroup) = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(4, g_BattleSpecRiderBreathLoadedResTable);
            g_BattleSpecRiderBreathSpriteReq3.drawObj = Gfx_ModelTableGetSpriteObject(4, g_BattleSpecRiderBreathLoadedResTable);
            BattleFx_InstallBillboardMatrix(&frame.scaledX, 0x1000 /* 1.0 */, 0);
            g_BattleSpecRiderBreathPacketCursor = BattleFx_SubmitSpriteDraw(&g_BattleSpecRiderBreathSpriteReq3, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecRiderBreathPacketCursor);
            break;
        case 2:
            (g_BattleSpecRiderBreathSpriteReq3.spriteGroup) = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(5, g_BattleSpecRiderBreathLoadedResTable);
            g_BattleSpecRiderBreathSpriteReq3.drawObj = Gfx_ModelTableGetSpriteObject(5, g_BattleSpecRiderBreathLoadedResTable);
            BattleFx_InstallBillboardMatrix(&frame.scaledX, 0x1000 /* 1.0 */, 0);
            g_BattleSpecRiderBreathPacketCursor = BattleFx_SubmitSpriteDraw(&g_BattleSpecRiderBreathSpriteReq3, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecRiderBreathPacketCursor);
            break;
        case 3:
            (g_BattleSpecRiderBreathSpriteReq3.spriteGroup) = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(6, g_BattleSpecRiderBreathLoadedResTable);
            g_BattleSpecRiderBreathSpriteReq3.drawObj = Gfx_ModelTableGetSpriteObject(6, g_BattleSpecRiderBreathLoadedResTable);
            BattleFx_InstallBillboardMatrix(&frame.scaledX, 0x1000 /* 1.0 */, 0);
            g_BattleSpecRiderBreathPacketCursor = BattleFx_SubmitSpriteDraw(&g_BattleSpecRiderBreathSpriteReq3, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecRiderBreathPacketCursor);
            break;
        case 4:
            (g_BattleSpecRiderBreathSpriteReq3.spriteGroup) = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(7, g_BattleSpecRiderBreathLoadedResTable);
            g_BattleSpecRiderBreathSpriteReq3.drawObj = Gfx_ModelTableGetSpriteObject(7, g_BattleSpecRiderBreathLoadedResTable);
            BattleFx_InstallBillboardMatrix(&frame.scaledX, 0x1000 /* 1.0 */, 0);
            g_BattleSpecRiderBreathPacketCursor = BattleFx_SubmitSpriteDraw(&g_BattleSpecRiderBreathSpriteReq3, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecRiderBreathPacketCursor);
            break;
        case 5:
            (g_BattleSpecRiderBreathSpriteReq3.spriteGroup) = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(8, g_BattleSpecRiderBreathLoadedResTable);
            g_BattleSpecRiderBreathSpriteReq3.drawObj = Gfx_ModelTableGetSpriteObject(8, g_BattleSpecRiderBreathLoadedResTable);
            BattleFx_InstallBillboardMatrix(&frame.scaledX, 0x1000 /* 1.0 */, 0);
            g_BattleSpecRiderBreathPacketCursor = BattleFx_SubmitSpriteDraw(&g_BattleSpecRiderBreathSpriteReq3, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecRiderBreathPacketCursor);
            break;
        case 6:
            (g_BattleSpecRiderBreathSpriteReq3.spriteGroup) = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(9, g_BattleSpecRiderBreathLoadedResTable);
            g_BattleSpecRiderBreathSpriteReq3.drawObj = Gfx_ModelTableGetSpriteObject(9, g_BattleSpecRiderBreathLoadedResTable);
            BattleFx_InstallBillboardMatrix(&frame.scaledX, 0x1000 /* 1.0 */, 0);
            g_BattleSpecRiderBreathPacketCursor = BattleFx_SubmitSpriteDraw(&g_BattleSpecRiderBreathSpriteReq3, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecRiderBreathPacketCursor);
            break;
        case 7:
            (g_BattleSpecRiderBreathSpriteReq3.spriteGroup) = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(10, g_BattleSpecRiderBreathLoadedResTable);
            g_BattleSpecRiderBreathSpriteReq3.drawObj = Gfx_ModelTableGetSpriteObject(10, g_BattleSpecRiderBreathLoadedResTable);
            BattleFx_InstallBillboardMatrix(&frame.scaledX, 0x1000 /* 1.0 */, 0);
            g_BattleSpecRiderBreathPacketCursor = BattleFx_SubmitSpriteDraw(&g_BattleSpecRiderBreathSpriteReq3, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecRiderBreathPacketCursor);
            break;
        case 8:
            (g_BattleSpecRiderBreathSpriteReq3.spriteGroup) = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(11, g_BattleSpecRiderBreathLoadedResTable);
            g_BattleSpecRiderBreathSpriteReq3.drawObj = Gfx_ModelTableGetSpriteObject(11, g_BattleSpecRiderBreathLoadedResTable);
            BattleFx_InstallBillboardMatrix(&frame.scaledX, 0x1000 /* 1.0 */, 0);
            g_BattleSpecRiderBreathPacketCursor = BattleFx_SubmitSpriteDraw(&g_BattleSpecRiderBreathSpriteReq3, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecRiderBreathPacketCursor);
            break;
        case 9:
            (g_BattleSpecRiderBreathSpriteReq3.spriteGroup) = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(12, g_BattleSpecRiderBreathLoadedResTable);
            g_BattleSpecRiderBreathSpriteReq3.drawObj = Gfx_ModelTableGetSpriteObject(12, g_BattleSpecRiderBreathLoadedResTable);
            BattleFx_InstallBillboardMatrix(&frame.scaledX, 0x1000 /* 1.0 */, 0);
            g_BattleSpecRiderBreathPacketCursor = BattleFx_SubmitSpriteDraw(&g_BattleSpecRiderBreathSpriteReq3, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecRiderBreathPacketCursor);
            break;
        case 0xA:
            (g_BattleSpecRiderBreathSpriteReq3.spriteGroup) = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(13, g_BattleSpecRiderBreathLoadedResTable);
            g_BattleSpecRiderBreathSpriteReq3.drawObj = Gfx_ModelTableGetSpriteObject(13, g_BattleSpecRiderBreathLoadedResTable);
            BattleFx_InstallBillboardMatrix(&frame.scaledX, 0x1000 /* 1.0 */, 0);
            g_BattleSpecRiderBreathPacketCursor = BattleFx_SubmitSpriteDraw(&g_BattleSpecRiderBreathSpriteReq3, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecRiderBreathPacketCursor);
            break;
        case 0xB:
            (g_BattleSpecRiderBreathSpriteReq3.spriteGroup) = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(14, g_BattleSpecRiderBreathLoadedResTable);
            g_BattleSpecRiderBreathSpriteReq3.drawObj = Gfx_ModelTableGetSpriteObject(14, g_BattleSpecRiderBreathLoadedResTable);
            BattleFx_InstallBillboardMatrix(&frame.scaledX, 0x1000 /* 1.0 */, 0);
            g_BattleSpecRiderBreathPacketCursor = BattleFx_SubmitSpriteDraw(&g_BattleSpecRiderBreathSpriteReq3, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecRiderBreathPacketCursor);
        }
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.trailSlot->posX += frame.trailSlot->velocityX;
    frame.trailSlot->posY += frame.trailSlot->velocityY;
    frame.trailSlot->posZ += frame.trailSlot->velocityZ;
    (frame.trailSlot->frameCounter)++;
    if (frame.trailSlot->frameCounter == 15) {
        frame.trailSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Expanding model task: draws the Koudan2 model at the slot's position, growing by 0x320 per frame,
 * for 15 frames.
 */
// FUNCTION: FF7 0x0052c9bc
void __cdecl BattleFx_SpecialRiderBreathExpandingModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[92];
        int depth;
        struct { unsigned char pad_to_4[0x4]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_28[0x28]; PsxMatrix combinedMat; };
        struct { unsigned char pad_to_48[0x48]; short axis; };
        struct { unsigned char pad_to_50[0x50]; BattleTaskSlot *expandSlot; };
    } frame;
#pragma pack(pop)

    frame.expandSlot = BATTLE_CURRENT_TASK_SLOT;
    (g_BattleSpecRiderBreathModelReq.modelGroup) = Gfx_ModelTableGetRenderObject(0, g_BattleSpecRiderBreathLoadedResTable);
    for (frame.axis = 0; frame.axis < 3; frame.axis++) {
        frame.combinedMat.m[2][frame.axis] = 0;
        frame.combinedMat.m[1][frame.axis] = 0;
        frame.combinedMat.m[0][frame.axis] = 0;
    }
    SetRotMatrix(&g_BattleCameraTransform);
    SetTransMatrix(&g_BattleCameraTransform);
    RotTransPoint((SVECTOR *)frame.expandSlot->data, frame.combinedMat.t, &frame.depth);
    frame.combinedMat.m[2][2] = frame.expandSlot->frameCounter * 800 + 0x1000;
    frame.combinedMat.m[1][1] = frame.combinedMat.m[2][2];
    frame.combinedMat.m[0][0] = frame.combinedMat.m[1][1];
    RotMatrix((SVECTOR *)&g_BattleSpecRiderBreathRotAngles, &frame.rotMatrix);
    ConcatMatrixIntoFirst(&frame.combinedMat, &frame.rotMatrix);
    SetRotMatrix(&frame.combinedMat);
    SetTransMatrix(&frame.combinedMat);
    SetDrawColorRGB(0, 0, 0);
    g_BattleSpecRiderBreathModelReq.fadeLevel = frame.expandSlot->frameCounter << 8;
    g_BattleSpecRiderBreathPacketCursor = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecRiderBreathModelReq, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecRiderBreathPacketCursor);
    if (g_BattleIsPaused != 0) {
        return;
    }
    (frame.expandSlot->frameCounter)++;
    if (frame.expandSlot->frameCounter == 15) {
        frame.expandSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}
} /* extern "C" */
