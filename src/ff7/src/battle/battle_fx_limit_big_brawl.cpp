// src/ff7/src/battle/battle_fx_limit_big_brawl.cpp
// Limit Break [33] (Cid Lv 3-2): Big Brawl (Japanese: 大乱闘, dev tag: "LIMIT3/RANNTOU" / Ranntou)
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Ranntou limit effect: loads its resources, stores the target mask and centroid, and
//   runs a moving sprite task on an effect slot.
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
#include "ff7/types/battle_3d.h"
extern "C" {

#pragma intrinsic(memset)

// clang-format off
// GLOBAL: FF7 0x008065e8
int g_BattleLimitBigBrawlResourceDef = 0x00192A39;
char *g_BattleFxLimitBigBrawlResourceNames[1] = {
    // STRING: FF7 0x00806620
    "KEMURI.TIM"
};
unsigned int g_BattleFxLimitBigBrawlAssetPrefix[4] = {
    // STRING: FF7 0x0080662c
    (unsigned int)"KEMURI.SPT",
    0x00000000,
    0x00000000,
    0x00000000
};
// GLOBAL: FF7 0x00806600
BattleFxSpriteRequest g_BattleLimitBigBrawlSpriteRequest = {0, 0, 0x20, 0x20, 0x20, 0x2e, 0, 0};
// GLOBAL: FF7 0x00806610
short g_BattleLimitBigBrawlHitFrames[2] = {31, 35};
unsigned char g_rawData_00806614[4] = {0x29, 0x00, 0x2e, 0x00};
unsigned char g_rawData_00806618[8] = {0x34, 0x00, 0x39, 0x00, 0x3f, 0x00, 0x44, 0x00};
// clang-format on

#pragma intrinsic(memcpy)
#pragma function(memcpy)

// clang-format off
// GLOBAL: FF7 0x00aef800
int g_BattleLimitBigBrawlCentroidX = 0;
// GLOBAL: FF7 0x00aef804
short g_BattleLimitBigBrawlCentroidY = 0;
unsigned char g_bssPad_00aef808[4] = {0};
// GLOBAL: FF7 0x00aef80c
short g_BattleLimitBigBrawlTargetMask = 0;
// GLOBAL: FF7 0x00aef810
struct GfxTableCursor *g_BattleLimitBigBrawlLoadedResTable = 0;
// GLOBAL: FF7 0x00aef814
short g_BattleLimitBigBrawlTimelineFrame = 0;
// clang-format on

// Loads the LIMIT3/RANNTOU model table into g_BattleLimitBigBrawlLoadedResTable once the battle scene
// exists; does nothing if it is already loaded.
// FUNCTION: FF7 0x004aeb60
void __cdecl BattleFx_LimitLoadResBigBrawl(void)
{
    GfxModelTableBank *ownerHandle;
    GfxResourceRequest recordBuf;
    int kind;
    char pathText[200];
    GfxResourceRequest *pRequest;
    int stepResourceDef;

    if (g_pBattle3dScene != 0 && g_BattleLimitBigBrawlLoadedResTable == 0) {
        kind = 1;
        stepResourceDef = (int)&g_BattleLimitBigBrawlResourceDef;
        ownerHandle = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRequest = &recordBuf;
        Gfx_InitResourceRequest(pRequest);
        pRequest->groupTag = 0;
        // STRING: FF7 0x00806638
        pRequest->resourcePath = BattleFx_BuildResourcePath("LIMIT3/RANNTOU", pathText);
        Gfx_LoadModelTableIfAbsent(kind, stepResourceDef, pRequest, &g_BattleLimitBigBrawlLoadedResTable, ownerHandle);
    }
}

// Loads the Ranntou limit resources, starts its effect task and returns zero.
// FUNCTION: FF7 0x004aebf0
int __cdecl BattleFx_LimitEnterBigBrawl(int targetMask, int actorIdx)
{
    BattleFx_LimitLoadResBigBrawl();
    BattleFx_LimitBeginBigBrawl(targetMask, actorIdx);
    return 0;
}

// Stores the target mask, computes the targets' centroid into g_BattleLimitBigBrawlCentroidX/Y and
// starts BattleFx_LimitBigBrawlTimelineTask for the attacking actor.
// FUNCTION: FF7 0x004aec0c
void __cdecl BattleFx_LimitBeginBigBrawl(int targetMask, int actorIdx)
{
    struct BattleTaskSlot *slot;

    g_BattleLimitBigBrawlTargetMask = targetMask;
    Battle_GetActorMaskCentroid(targetMask, (short *)&g_BattleLimitBigBrawlCentroidX);
    slot = BATTLE_NEW_TASK_SLOT(BattleFx_LimitBigBrawlTimelineTask);
    ((BattleLimitBigBrawlTimeline *)slot)->actorIndex = actorIdx;
}

// Timeline of the Ranntou limit. At each hit frame it places the attacker on the ground around the
// next target, facing it, allocates a ring record and plays the hit sound. From frame 31 to 70 each
// call spawns four BattleFx_LimitBigBrawlMovingSpriteTask sprites: two at the attacker and two at a
// random point around the targets' centroid. Frame 80 retires the task.
// FUNCTION: FF7 0x004aec56
void __cdecl BattleFx_LimitBigBrawlTimelineTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[48];
        short targetActor;
        struct { unsigned char pad_to_4[0x4]; short targetX; };
        struct { unsigned char pad_to_6[0x6]; short targetY; };
        struct { unsigned char pad_to_8[0x8]; short targetZ; };
        struct { unsigned char pad_to_C[0xC]; BattleFxLimitBigBrawlSprite *sprite; };
        struct { unsigned char pad_to_14[0x14]; BattleFxBigBrawlRecord *ringRecord; };
        struct { unsigned char pad_to_18[0x18]; short emitIndex; };
        struct { unsigned char pad_to_1C[0x1C]; short radius; };
        struct { unsigned char pad_to_20[0x20]; BattleLimitBigBrawlTimeline *timeline; };
        struct { unsigned char pad_to_24[0x24]; short angle; };
        struct { unsigned char pad_to_28[0x28]; short placedX; };
        struct { unsigned char pad_to_2A[0x2A]; short placedY; };
        struct { unsigned char pad_to_2C[0x2C]; short placedZ; };
    } frame;
#pragma pack(pop)

    frame.timeline = (BattleLimitBigBrawlTimeline *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (g_BattleLimitBigBrawlTimelineFrame < 8 && frame.timeline->frame == g_BattleLimitBigBrawlHitFrames[g_BattleLimitBigBrawlTimelineFrame]) {
        frame.targetActor = g_BattleHitTargetActors[g_BattleLimitBigBrawlTimelineFrame];
        frame.targetX = g_BattleActorRecords[frame.targetActor].posX;
        frame.targetY = g_BattleActorRecords[frame.targetActor].posY;
        frame.targetZ = g_BattleActorRecords[frame.targetActor].posZ;
        frame.angle = (rand() % 4096);
        frame.radius = g_BattleActorRecords[frame.targetActor].radius + 1500;
        frame.targetX += (short)(SinFixed(frame.angle) * frame.radius >> 12);
        frame.placedX = frame.targetX;
        frame.placedY = 0;
        frame.targetZ += (short)(CosFixed(frame.angle) * frame.radius >> 12);
        frame.placedZ = frame.targetZ;
        g_BattleActorRecords[frame.timeline->actorIndex].posX = frame.placedX;
        g_BattleActorRecords[frame.timeline->actorIndex].posY = frame.placedY;
        g_BattleActorRecords[frame.timeline->actorIndex].posZ = frame.placedZ;
        g_BattleActorRecords[frame.timeline->actorIndex].rotX = 0;
        g_BattleActorRecords[frame.timeline->actorIndex].yaw = frame.angle;
        g_BattleActorRecords[frame.timeline->actorIndex].rotZ = 0;
        frame.ringRecord = (BattleFxBigBrawlRecord *)Battle_AllocSceneMessage(0xA);
        frame.ringRecord->actorIndex = frame.targetActor;
        frame.ringRecord->timelineFrame = g_BattleLimitBigBrawlTimelineFrame;
        Battle_PlaySfxPannedToActor(0x15A, (short)frame.targetActor, 0);
        g_BattleLimitBigBrawlTimelineFrame++;
    }
    if (frame.timeline->frame >= 31 && frame.timeline->frame < 71) {
        for (frame.emitIndex = 0; frame.emitIndex < 2; frame.emitIndex++) {
            frame.sprite = (BattleFxLimitBigBrawlSprite *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitBigBrawlMovingSpriteTask);
            frame.sprite->posX = g_BattleActorRecords[frame.timeline->actorIndex].posX;
            frame.sprite->posY = g_BattleActorRecords[frame.timeline->actorIndex].posY;
            frame.sprite->posZ = g_BattleActorRecords[frame.timeline->actorIndex].posZ;
            frame.angle = (rand() % 4096);
            frame.sprite->velocityX = SinFixed(frame.angle) * 0xC8 >> 12;
            frame.sprite->velocityY = -rand() % 100;
            frame.sprite->velocityZ = CosFixed(frame.angle) * 0xC8 >> 12;
        }
        for (frame.emitIndex = 0; frame.emitIndex < 2; frame.emitIndex++) {
            frame.sprite = (BattleFxLimitBigBrawlSprite *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitBigBrawlMovingSpriteTask);
            frame.angle = (rand() % 4096);
            frame.radius = rand() % 4000;
            frame.sprite->posX = *(short *)&g_BattleLimitBigBrawlCentroidX + (SinFixed(frame.angle) * frame.radius >> 12);
            frame.sprite->posY = 0;
            frame.sprite->posZ = g_BattleLimitBigBrawlCentroidY + (CosFixed(frame.angle) * frame.radius >> 12);
            frame.angle = (rand() % 4096);
            frame.sprite->velocityX = SinFixed(frame.angle) * 0x64 >> 12;
            frame.sprite->velocityY = -rand() % 100;
            frame.sprite->velocityZ = CosFixed(frame.angle) * 0x64 >> 12;
        }
    }
    (frame.timeline->frame)++;
    if (frame.timeline->frame == 80) {
        frame.timeline->ownerSlot = BATTLE_TASK_FREE;
    }
}

// One frame of a moving sprite: draws the Ranntou sprite request as a billboard at the task's position,
// then (unless paused) integrates the velocity and retires the task at the sprite group's last frame.
// FUNCTION: FF7 0x004af0c8
void __cdecl BattleFx_LimitBigBrawlMovingSpriteTask(void)
{
    BattleFxPulseOfLifeSprite *task;

    task = (BattleFxPulseOfLifeSprite *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleLimitBigBrawlSpriteRequest.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(0, g_BattleLimitBigBrawlLoadedResTable);
    g_BattleLimitBigBrawlSpriteRequest.drawObj = Gfx_ModelTableGetSpriteObject(0, g_BattleLimitBigBrawlLoadedResTable);
    BattleFx_InstallBillboardMatrix((char *)task + 4, 0x4000 /* 4.0 */, 0);
    g_BattleLimitBigBrawlSpriteRequest.frame = task->frame;
    g_BattleOverlayDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleLimitBigBrawlSpriteRequest, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused == 0) {
        task->posX += task->velocityX;
        task->posY += task->velocityY;
        task->posZ += task->velocityZ;
        if (g_BattleLimitBigBrawlSpriteRequest.spriteGroup != 0) {
            (task->frame)++;
            if (task->frame >= g_BattleLimitBigBrawlSpriteRequest.spriteGroup->header->frameCount) {
                task->ownerSlot = BATTLE_TASK_FREE;
            }
        }
    }
}
} /* extern "C" */
