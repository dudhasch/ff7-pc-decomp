// src/ff7/src/battle/battle_fx_special_light_shell.cpp
/* Enemy Special Effect [idx 0]: LightShell (JP: koudan1 / 光弾1 (Light Bullet 1), asset: "SPECIAL/KOUDAN1") — Light Shell (0x26a, Adamantaimai), Pyramid (0x15d..0x15f, Reno), Capsule (0x38b..0x38c, Hojo), Reanimagic (0x18e) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle special effect 'Koudan1': a per-target timeline with variant sprites, trail
//   sprites and an expanding model.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/api/battle.h"
#include "ff7/battle_tasks.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_special.h"
#include "lib/types/graphics.h"
#include "ff7/camera_script.h"
#include <string.h>
#include <stdlib.h>
#include "lib/codegen.h"
extern "C" {

struct BattleFxModelDraw;

// clang-format off
// GLOBAL: FF7 0x0089f9c8
unsigned int g_BattleSpecLightShellResourceDefs[90] ={
    0x001900b1,
    (unsigned int)&g_BattleSpecLightShellHikariName,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x0089fb74
    (unsigned int)"KOUDAN00.TIM",
    // STRING: FF7 0x0089fb84
    (unsigned int)"MARU1.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x0089fb90
    (unsigned int)"KOUDAN03.TIM",
    // STRING: FF7 0x0089fba0
    (unsigned int)"HOSI1.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x0089fbac
    (unsigned int)"KOUDAN01.TIM",
    // STRING: FF7 0x0089fbbc
    (unsigned int)"WA1.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x0089fbc4
    (unsigned int)"KOUDAN01.TIM",
    // STRING: FF7 0x0089fbd4
    (unsigned int)"BOKASI.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x0089fbe0
    (unsigned int)"KOUDAN00.TIM",
    // STRING: FF7 0x0089fbf0
    (unsigned int)"MARU2.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x0089fbfc
    (unsigned int)"KOUDAN00.TIM",
    // STRING: FF7 0x0089fc0c
    (unsigned int)"MARU3.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x0089fc18
    (unsigned int)"KOUDAN00.TIM",
    // STRING: FF7 0x0089fc28
    (unsigned int)"MARU4.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x0089fc34
    (unsigned int)"KOUDAN00.TIM",
    // STRING: FF7 0x0089fc44
    (unsigned int)"MARU5.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x0089fc50
    (unsigned int)"KOUDAN00.TIM",
    // STRING: FF7 0x0089fc60
    (unsigned int)"MARU6.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x0089fc6c
    (unsigned int)"KOUDAN00.TIM",
    // STRING: FF7 0x0089fc7c
    (unsigned int)"MARU7.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x0089fc88
    (unsigned int)"KOUDAN00.TIM",
    // STRING: FF7 0x0089fc98
    (unsigned int)"MARU8.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x0089fca4
    (unsigned int)"KOUDAN00.TIM",
    // STRING: FF7 0x0089fcb4
    (unsigned int)"MARU9.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x0089fcc0
    (unsigned int)"KOUDAN00.TIM",
    // STRING: FF7 0x0089fcd0
    (unsigned int)"MARU10.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x0089fcdc
    (unsigned int)"KOUDAN01.TIM",
    // STRING: FF7 0x0089fcec
    (unsigned int)"WA2.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x0089fcf4
    (unsigned int)"KOUDAN02.TIM",
    // STRING: FF7 0x0089fd04
    (unsigned int)"HIKARI1.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x0089fd10
    (unsigned int)"KOUDAN02.TIM",
    // STRING: FF7 0x0089fd20
    (unsigned int)"HIKARI2.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x0089fd2c
    (unsigned int)"KOUDAN02.TIM",
    // STRING: FF7 0x0089fd3c
    (unsigned int)"HIKARI3.SPT"
};
// GLOBAL: FF7 0x0089fb30
struct BattleSpecLightShellSpriteReq1 g_BattleSpecLightShellSpriteReq1 = {0, 0, {0x80, 0x80, 0x80, 0x2e}, 0, {0}};
// GLOBAL: FF7 0x0089fb40
struct BattleSpecLightShellSpriteReq2 g_BattleSpecLightShellSpriteReq2 = {0, 0, {0x80, 0x80, 0x80, 0x2c, 0, 0, 0, 0}};
// GLOBAL: FF7 0x0089fb50
struct BattleSpecLightShellModelReq g_BattleSpecLightShellModelReq = {0, {0xa8, 0, 0, 0, 0, 0}, 128, {0x20, 0, 0, 0}, 0xfc00, {0}};
// GLOBAL: FF7 0x0089fb68
unsigned char g_BattleSpecLightShellHikariName[11] = "HIKARI.RSD";
// clang-format on

#pragma intrinsic(memset)
#pragma intrinsic(memcpy)


// clang-format off
// GLOBAL: FF7 0x00b5e460
int g_BattleSpecLightShellDrawListHead = 0;
// GLOBAL: FF7 0x00b5e464
struct GfxTableCursor *g_BattleSpecLightShellLoadedResTable = 0;
// clang-format on

/*
 * When the engine context exists and g_BattleSpecLightShellLoadedResTable is empty, builds SPECIAL/KOUDAN1 with a default-
 * duration record and creates resource type 18 using g_BattleSpecLightShellResourceDefs.
 */
// FUNCTION: FF7 0x0052cb40
void __cdecl BattleFx_SpecialLoadResLightShell(void)
{
    GfxModelTableBank *stage;
    GfxResourceRequest requestSlot;
    int kind;
    char pathText[200];
    GfxResourceRequest *pRequest;
    int resourceTable;

    if (g_pBattle3dScene != 0 && g_BattleSpecLightShellLoadedResTable == 0) {
        kind = 0x12;
        resourceTable = (int)&g_BattleSpecLightShellResourceDefs;
        stage = (GfxModelTableBank *)g_pBattle3dScene->modelTableBank;
        pRequest = &requestSlot;
        Gfx_InitResourceRequest(pRequest);
        ((BattleFxDiamondCardResource *)pRequest)->groupTag = 0;
        // STRING: FF7 0x0089fd48
        pRequest->resourcePath = BattleFx_BuildResourcePath("SPECIAL/KOUDAN1", pathText);
        Gfx_LoadModelTableIfAbsent(kind, resourceTable, pRequest, &g_BattleSpecLightShellLoadedResTable, stage);
    }
}

/*
 * Mechanical forwarder: passes both arguments straight to BattleFx_SpecialRunLightShell and does
 * nothing else.
 */
// FUNCTION: FF7 0x0052cbd0
void __cdecl BattleFx_SpecialEnterLightShell(int targetMask, int actorIdx)
{
    BattleFx_SpecialRunLightShell(targetMask, actorIdx);
}

/*
 * Calls BattleFx_SpecialLoadResLightShell, then creates a task for
 * BattleFx_SpecialSpawnLightShellPerTarget with both arguments and mode 4, then dispatches event
 * (0x188, targetActor, 0x40).
 */
// FUNCTION: FF7 0x0052cbe5
void __cdecl BattleFx_SpecialRunLightShell(int sourceActor, int targetActor)
{
    BattleFx_SpecialLoadResLightShell();
    Battle_CreateBitPatternCallbackTask(sourceActor, targetActor, 4, BattleFx_SpecialSpawnLightShellPerTarget);
    Battle_PlaySfxPannedToActor(0x188, (short)targetActor, 0x40);
}

/*
 * Allocates a slot for BattleFx_SpecialLightShellTimelineTask, stores the target actor's bone33
 * position as the start position and the source actor's target-bone position as the end position,
 * then stores the source actor index.
 */
// FUNCTION: FF7 0x0052cc1a
void __cdecl BattleFx_SpecialSpawnLightShellPerTarget(int actor, int targetActor)
{
    BattleFxLightShellTimeline *taskSlot;

    taskSlot = (BattleFxLightShellTimeline *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialLightShellTimelineTask);
    Battle_GetBoneWorldPos(targetActor, g_BattleActorRecords[targetActor].bone33, (short *)&taskSlot->startX);
    Battle_GetBoneWorldPos(actor, g_BattleActorRecords[actor].targetBone, (short *)&taskSlot->endX);
    taskSlot->actorIndex = actor;
}

/*
 * Koudan1 timeline: on frame 0 spawns the trail task moving from start to end over 20 frames; on
 * frame 0x14 spawns the expanding model at the target and plays sound 0x189; on frames 0x13..0x1B
 * spawns three variant sprites per frame around the target; retires at frame 0x28.
 */
// FUNCTION: FF7 0x0052cc90
void __cdecl BattleFx_SpecialLightShellTimelineTask(void)
{
    int p; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int q; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxLightShellTimeline *taskPtr;
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
    short j6;
    BattleFxToxicSmogTimelineSpawnSlot *pChild;
    int f; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int g; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int h; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int j; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int k; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */

    taskPtr = (BattleFxLightShellTimeline *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (taskPtr->frame == 0) {
        pChild = (BattleFxToxicSmogTimelineSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialLightShellTrailSpritesTask);
        pChild->posX = taskPtr->startX;
        pChild->posY = taskPtr->startY;
        pChild->posZ = taskPtr->startZ;
        pChild->velocityX = (taskPtr->endX - taskPtr->startX) / 20;
        pChild->velocityY = (taskPtr->endY - taskPtr->startY) / 20;
        pChild->velocityZ = (taskPtr->endZ - taskPtr->startZ) / 20;
    }
    if (taskPtr->frame == 20) {
        Battle_PostActorHitOrHurtMessage(taskPtr->actorIndex);
        pChild = (BattleFxToxicSmogTimelineSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialLightShellExpandingModelTask);
        Battle_GetBoneWorldPos(taskPtr->actorIndex, g_BattleActorRecords[taskPtr->actorIndex].targetBone, (short *)((int)pChild + 4));
        Battle_PlaySfxPannedToActor(0x189, taskPtr->actorIndex, 0x40);
    }
    if (taskPtr->frame > 18 && taskPtr->frame < 28) {
        for (j6 = 0; j6 < 3; j6++) {
            pChild = (BattleFxToxicSmogTimelineSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialLightShellVariantSpriteTask);
            Battle_GetBoneWorldPos(taskPtr->actorIndex, g_BattleActorRecords[taskPtr->actorIndex].targetBone, (short *)((int)pChild + 4));
            pChild->posX += (short)(rand() % 1600 - 800);
            pChild->posY += (short)(rand() % 1600 - 800);
            pChild->posZ += (short)(rand() % 1600 - 800);
            pChild->phaseOrResIndex = j6;
        }
    }
    (taskPtr->frame)++;
    if (taskPtr->frame == 40) {
        taskPtr->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Per-frame sprite handler for one variant sprite. Installs a billboard matrix at scale 0x2000,
 * copies the frame index into the sprite request, loads the table entry {0xF, 0x10, 0x11} chosen
 * by resourceIndex from g_BattleSpecLightShellLoadedResTable and submits the sprite into the
 * depth-sorted ordering table. When not paused and the entry loaded, advances the frame index and
 * retires the slot once it reaches the sprite's frame count.
 */
// FUNCTION: FF7 0x0052cf15
void __cdecl BattleFx_SpecialLightShellVariantSpriteTask(void)
{
    BattleFxLightShellSpriteTask *task;
    int entryIds[3];

    task = (BattleFxLightShellSpriteTask *)BATTLE_CURRENT_TASK_SLOT;
    entryIds[0] = 0xF;
    entryIds[1] = 0x10;
    entryIds[2] = 0x11;
    BattleFx_InstallBillboardMatrix(&task->billboardMatrix, 0x2000 /* 2.0 */, 0);
    g_BattleSpecLightShellSpriteReq1.req1Word12 = task->frameIndex;
    (g_BattleSpecLightShellSpriteReq1.spriteGroup) = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(entryIds[task->resourceIndex], g_BattleSpecLightShellLoadedResTable);
    g_BattleSpecLightShellSpriteReq1.req1ResourceHandle = Gfx_ModelTableGetSpriteObject(entryIds[task->resourceIndex], g_BattleSpecLightShellLoadedResTable);
    g_BattleSpecLightShellDrawListHead = BattleFx_SubmitSpriteDraw(&(g_BattleSpecLightShellSpriteReq1.spriteGroup), g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecLightShellDrawListHead);
    if (g_BattleIsPaused == 0 && (g_BattleSpecLightShellSpriteReq1.spriteGroup) != 0) {
        (task->frameIndex)++;
        if (task->frameIndex >= *(int *)(**(int **)&(g_BattleSpecLightShellSpriteReq1.spriteGroup) + 4)) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Draws eleven trail sprites at the slot position scaled by (6 - index) / 6, then moves the slot by
 * its velocity. Retires after 20 frames.
 */
// FUNCTION: FF7 0x0052d010
void __cdecl BattleFx_SpecialLightShellTrailSpritesTask(void)
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
            (g_BattleSpecLightShellSpriteReq2.spriteResource) = Gfx_ModelTableGetLoadedRecord(1, g_BattleSpecLightShellLoadedResTable);
            g_BattleSpecLightShellSpriteReq2.req2ResourceHandle = Gfx_ModelTableGetSpriteObject(1, g_BattleSpecLightShellLoadedResTable);
            BattleFx_InstallBillboardMatrix(&frame.scaledX, 0x1000 /* 1.0 */, 0);
            g_BattleSpecLightShellDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecLightShellSpriteReq2, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecLightShellDrawListHead);
            break;
        case 1:
            (g_BattleSpecLightShellSpriteReq2.spriteResource) = Gfx_ModelTableGetLoadedRecord(2, g_BattleSpecLightShellLoadedResTable);
            g_BattleSpecLightShellSpriteReq2.req2ResourceHandle = Gfx_ModelTableGetSpriteObject(2, g_BattleSpecLightShellLoadedResTable);
            BattleFx_InstallBillboardMatrix(&frame.scaledX, (SinFixed(frame.trailSlot->frameCounter << 10) << 11 >> 12) + 0x2000, 0);
            g_BattleSpecLightShellDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecLightShellSpriteReq2, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecLightShellDrawListHead);
            (g_BattleSpecLightShellSpriteReq2.spriteResource) = Gfx_ModelTableGetLoadedRecord(3, g_BattleSpecLightShellLoadedResTable);
            g_BattleSpecLightShellSpriteReq2.req2ResourceHandle = Gfx_ModelTableGetSpriteObject(3, g_BattleSpecLightShellLoadedResTable);
            BattleFx_InstallBillboardMatrix(&frame.scaledX, 0x1000 /* 1.0 */, 0);
            g_BattleSpecLightShellDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecLightShellSpriteReq2, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecLightShellDrawListHead);
            (g_BattleSpecLightShellSpriteReq2.spriteResource) = Gfx_ModelTableGetLoadedRecord(4, g_BattleSpecLightShellLoadedResTable);
            g_BattleSpecLightShellSpriteReq2.req2ResourceHandle = Gfx_ModelTableGetSpriteObject(4, g_BattleSpecLightShellLoadedResTable);
            BattleFx_InstallBillboardMatrix(&frame.scaledX, 0x1000 /* 1.0 */, 0);
            g_BattleSpecLightShellDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecLightShellSpriteReq2, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecLightShellDrawListHead);
            break;
        case 2:
            (g_BattleSpecLightShellSpriteReq2.spriteResource) = Gfx_ModelTableGetLoadedRecord(5, g_BattleSpecLightShellLoadedResTable);
            g_BattleSpecLightShellSpriteReq2.req2ResourceHandle = Gfx_ModelTableGetSpriteObject(5, g_BattleSpecLightShellLoadedResTable);
            BattleFx_InstallBillboardMatrix(&frame.scaledX, 0x1000 /* 1.0 */, 0);
            g_BattleSpecLightShellDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecLightShellSpriteReq2, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecLightShellDrawListHead);
            break;
        case 3:
            (g_BattleSpecLightShellSpriteReq2.spriteResource) = Gfx_ModelTableGetLoadedRecord(6, g_BattleSpecLightShellLoadedResTable);
            g_BattleSpecLightShellSpriteReq2.req2ResourceHandle = Gfx_ModelTableGetSpriteObject(6, g_BattleSpecLightShellLoadedResTable);
            BattleFx_InstallBillboardMatrix(&frame.scaledX, 0x1000 /* 1.0 */, 0);
            g_BattleSpecLightShellDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecLightShellSpriteReq2, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecLightShellDrawListHead);
            break;
        case 4:
            (g_BattleSpecLightShellSpriteReq2.spriteResource) = Gfx_ModelTableGetLoadedRecord(7, g_BattleSpecLightShellLoadedResTable);
            g_BattleSpecLightShellSpriteReq2.req2ResourceHandle = Gfx_ModelTableGetSpriteObject(7, g_BattleSpecLightShellLoadedResTable);
            BattleFx_InstallBillboardMatrix(&frame.scaledX, 0x1000 /* 1.0 */, 0);
            g_BattleSpecLightShellDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecLightShellSpriteReq2, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecLightShellDrawListHead);
            break;
        case 5:
            (g_BattleSpecLightShellSpriteReq2.spriteResource) = Gfx_ModelTableGetLoadedRecord(8, g_BattleSpecLightShellLoadedResTable);
            g_BattleSpecLightShellSpriteReq2.req2ResourceHandle = Gfx_ModelTableGetSpriteObject(8, g_BattleSpecLightShellLoadedResTable);
            BattleFx_InstallBillboardMatrix(&frame.scaledX, 0x1000 /* 1.0 */, 0);
            g_BattleSpecLightShellDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecLightShellSpriteReq2, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecLightShellDrawListHead);
            break;
        case 6:
            (g_BattleSpecLightShellSpriteReq2.spriteResource) = Gfx_ModelTableGetLoadedRecord(9, g_BattleSpecLightShellLoadedResTable);
            g_BattleSpecLightShellSpriteReq2.req2ResourceHandle = Gfx_ModelTableGetSpriteObject(9, g_BattleSpecLightShellLoadedResTable);
            BattleFx_InstallBillboardMatrix(&frame.scaledX, 0x1000 /* 1.0 */, 0);
            g_BattleSpecLightShellDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecLightShellSpriteReq2, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecLightShellDrawListHead);
            break;
        case 7:
            (g_BattleSpecLightShellSpriteReq2.spriteResource) = Gfx_ModelTableGetLoadedRecord(10, g_BattleSpecLightShellLoadedResTable);
            g_BattleSpecLightShellSpriteReq2.req2ResourceHandle = Gfx_ModelTableGetSpriteObject(10, g_BattleSpecLightShellLoadedResTable);
            BattleFx_InstallBillboardMatrix(&frame.scaledX, 0x1000 /* 1.0 */, 0);
            g_BattleSpecLightShellDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecLightShellSpriteReq2, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecLightShellDrawListHead);
            break;
        case 8:
            (g_BattleSpecLightShellSpriteReq2.spriteResource) = Gfx_ModelTableGetLoadedRecord(11, g_BattleSpecLightShellLoadedResTable);
            g_BattleSpecLightShellSpriteReq2.req2ResourceHandle = Gfx_ModelTableGetSpriteObject(11, g_BattleSpecLightShellLoadedResTable);
            BattleFx_InstallBillboardMatrix(&frame.scaledX, 0x1000 /* 1.0 */, 0);
            g_BattleSpecLightShellDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecLightShellSpriteReq2, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecLightShellDrawListHead);
            break;
        case 9:
            (g_BattleSpecLightShellSpriteReq2.spriteResource) = Gfx_ModelTableGetLoadedRecord(12, g_BattleSpecLightShellLoadedResTable);
            g_BattleSpecLightShellSpriteReq2.req2ResourceHandle = Gfx_ModelTableGetSpriteObject(12, g_BattleSpecLightShellLoadedResTable);
            BattleFx_InstallBillboardMatrix(&frame.scaledX, 0x1000 /* 1.0 */, 0);
            g_BattleSpecLightShellDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecLightShellSpriteReq2, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecLightShellDrawListHead);
            break;
        case 0xA:
            (g_BattleSpecLightShellSpriteReq2.spriteResource) = Gfx_ModelTableGetLoadedRecord(13, g_BattleSpecLightShellLoadedResTable);
            g_BattleSpecLightShellSpriteReq2.req2ResourceHandle = Gfx_ModelTableGetSpriteObject(13, g_BattleSpecLightShellLoadedResTable);
            BattleFx_InstallBillboardMatrix(&frame.scaledX, 0x1000 /* 1.0 */, 0);
            g_BattleSpecLightShellDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecLightShellSpriteReq2, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecLightShellDrawListHead);
            break;
        case 0xB:
            (g_BattleSpecLightShellSpriteReq2.spriteResource) = Gfx_ModelTableGetLoadedRecord(14, g_BattleSpecLightShellLoadedResTable);
            g_BattleSpecLightShellSpriteReq2.req2ResourceHandle = Gfx_ModelTableGetSpriteObject(14, g_BattleSpecLightShellLoadedResTable);
            BattleFx_InstallBillboardMatrix(&frame.scaledX, 0x1000 /* 1.0 */, 0);
            g_BattleSpecLightShellDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecLightShellSpriteReq2, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecLightShellDrawListHead);
        }
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.trailSlot->posX += frame.trailSlot->velocityX;
    frame.trailSlot->posY += frame.trailSlot->velocityY;
    frame.trailSlot->posZ += frame.trailSlot->velocityZ;
    (frame.trailSlot->frameCounter)++;
    if (frame.trailSlot->frameCounter == 20) {
        frame.trailSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Draws the expanding Koudan1 model at its slot position, growing with the frame counter. Retires
 * at frame 15.
 */
// FUNCTION: FF7 0x0052d727
void __cdecl BattleFx_SpecialLightShellExpandingModelTask(void)
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
    (g_BattleSpecLightShellModelReq.modelResource) = Gfx_ModelTableGetRenderObject(0, g_BattleSpecLightShellLoadedResTable);
    for (frame.axis = 0; frame.axis < 3; frame.axis++) {
        frame.combinedMat.m[2][frame.axis] = 0;
        frame.combinedMat.m[1][frame.axis] = 0;
        frame.combinedMat.m[0][frame.axis] = 0;
    }
    SetRotMatrix(&g_BattleCameraTransform);
    SetTransMatrix(&g_BattleCameraTransform);
    RotTransPoint(frame.expandSlot->data, frame.combinedMat.t, &frame.depth);
    frame.combinedMat.m[2][2] = frame.expandSlot->frameCounter * 800 + 0x1000;
    frame.combinedMat.m[1][1] = frame.combinedMat.m[2][2];
    frame.combinedMat.m[0][0] = frame.combinedMat.m[1][1];
    RotMatrix((SVECTOR *)&g_BattleSpecLightShellModelReq.matrix, &frame.rotMatrix);
    ConcatMatrixIntoFirst(&frame.combinedMat, &frame.rotMatrix);
    SetRotMatrix(&frame.combinedMat);
    SetTransMatrix(&frame.combinedMat);
    SetDrawColorRGB(0, 0, 0);
    g_BattleSpecLightShellModelReq.alpha = frame.expandSlot->frameCounter << 8;
    g_BattleSpecLightShellDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecLightShellModelReq, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecLightShellDrawListHead);
    if (g_BattleIsPaused != 0) {
        return;
    }
    (frame.expandSlot->frameCounter)++;
    if (frame.expandSlot->frameCounter == 15) {
        frame.expandSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}
} /* extern "C" */
