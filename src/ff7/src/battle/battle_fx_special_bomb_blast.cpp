// src/ff7/src/battle/battle_fx_special_bomb_blast.cpp
/* Enemy Special Effect [idx 67]: BombBlast (JP: jibaku2 / 自爆2 (Self-Destruct 2), asset: "SPECIAL/JIBAKU2") — Bomb Blast (0x1ea, Bomb / Grenade) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle special effect for the Jibaku attack: it loads the effect's resource table once,
//   then spawns a per-target timeline task that drives its sprite and spinning-model tasks,
//   including a screen-ramp timer, animated, grow-fade, bouncing, radial-spark and growing-model
//   steps.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/api/battle.h"
#include "ff7/battle_tasks.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_special.h"
#include "ff7/camera_script.h"
#include "lib/types/graphics.h"
#include <string.h>
#include <stdlib.h>
#include "lib/codegen.h"
extern "C" {

struct BattleFxModelDraw;
// clang-format off
// GLOBAL: FF7 0x008a2da8
int g_BattleSpecBombBlastResourceDefs = 0x001940B1;
int g_BattleSpecBombBlastResourceDefsHeadPtr[1] = {
    (int)((char *)&g_BattleFxSpecialBombBlastResourceNames + 0x0)
};
int g_BattleSpecBombBlastResourceDefsDataTail[54] = {
    0x0, 0x0, 0x0, 0x192a39, (int)((char *)&g_BattleFxSpecialBombBlastResourceNames + 0xc), (int)((char *)&g_BattleFxSpecialBombBlastResourceNames + 0x1c),
    0x0, 0x0, 0x192a39, (int)((char *)&g_BattleFxSpecialBombBlastResourceNames + 0x28), (int)((char *)&g_BattleFxSpecialBombBlastResourceNames + 0x38), 0x0,
    0x0, 0x192a39, (int)((char *)&g_BattleFxSpecialBombBlastResourceNames + 0x44), (int)((char *)&g_BattleFxSpecialBombBlastResourceNames + 0x54), 0x0, 0x0,
    0x192a39, (int)((char *)&g_BattleFxSpecialBombBlastResourceNames + 0x60), (int)((char *)&g_BattleFxSpecialBombBlastResourceNames + 0x70), 0x0, 0x0, 0x192a39,
    (int)((char *)&g_BattleFxSpecialBombBlastResourceNames + 0x7c), (int)((char *)&g_BattleFxSpecialBombBlastResourceNames + 0x8c), 0x0, 0x0, 0x192a39, (int)((char *)&g_BattleFxSpecialBombBlastResourceNames + 0x98),
    (int)((char *)&g_BattleFxSpecialBombBlastResourceNames + 0xa8), 0x0, 0x0, 0x192a39, 0x0, (int)((char *)&g_BattleFxSpecialBombBlastResourceNames + 0xb4),
    0x0, 0x0, 0x192a39, (int)((char *)&g_BattleFxSpecialBombBlastResourceNames + 0xc0), (int)((char *)&g_BattleFxSpecialBombBlastResourceNames + 0xd0), 0x0,
    0x0, 0x192a39, (int)((char *)&g_BattleFxSpecialBombBlastResourceNames + 0xdc), (int)((char *)&g_BattleFxSpecialBombBlastResourceNames + 0xec), 0x0, 0x0,
    0x192a39, (int)((char *)&g_BattleFxSpecialBombBlastResourceNames + 0xf8), (int)((char *)&g_BattleFxSpecialBombBlastResourceNames + 0x108), 0x0, 0x0, 0x0
};
// GLOBAL: FF7 0x008a2e88
struct BattleSpecBombBlastSpriteReq1 g_BattleSpecBombBlastSpriteReq1 = {0, 0, {0x80, 0x80, 0x80, 0x2c}, 0, {0}, 0, 0};
// GLOBAL: FF7 0x008a2ea0
struct BattleSpecBombBlastSpriteReq2 g_BattleSpecBombBlastSpriteReq2 = {-128, -128, -128, 0x2e, 0, {0}, 0, 0};
// GLOBAL: FF7 0x008a2eb0
struct BattleSpecBombBlastSpriteReq2 g_BattleSpecBombBlastSpriteReq3 = {-128, -128, -128, 44, 0, {0}, 0, 0};
// GLOBAL: FF7 0x008a2ec0
struct BattleSpecBombBlastSpriteReq4 g_BattleSpecBombBlastSpriteReq4 = {-128, -128, -128, {0x2c, 0, 0, 0, 0}, 0, {0x28, 0, 0, 0, 0, 0}, 128};
unsigned char g_rawData_008a2ed4[4] = {0x20, 0x00, 0x00, 0x00};
unsigned char g_BattleFxSpecialBombBlastResourceNames[276] =
    "BAKU11.RSD\0\0"
    "JIBAKU01.TIM\0\0\0\0"
    "ELEHIT1.SPT\0"
    "JIBAKU01.TIM\0\0\0\0"
    "ELEHIT2.SPT\0"
    "JIBAKU01.TIM\0\0\0\0"
    "ELEHIT3.SPT\0"
    "JIBAKU01.TIM\0\0\0\0"
    "ELEHIT4.SPT\0"
    "JIBAKU02.TIM\0\0\0\0"
    "ELEC1.SPT\0\0\0"
    "JIBAKU00.TIM\0\0\0\0"
    "BAKU1.SPT\0\0\0"
    "JO_C01.SPT\0\0"
    "JIBAKU03.TIM\0\0\0\0"
    "HAHEN1.SPT\0\0"
    "JIBAKU02.TIM\0\0\0\0"
    "HIBANA.SPT\0\0"
    "JIBAKU03.TIM\0\0\0\0"
    "KEMU11.SPT";
// clang-format on
#pragma intrinsic(memset)
#pragma intrinsic(memcpy)


// clang-format off
// GLOBAL: FF7 0x00b64fd8
int g_BattleSpecBombBlastPacketCursor = 0;
unsigned char g_bssPad_00b64fdc[4] = {0};
// GLOBAL: FF7 0x00b64fe0
int g_BattleSpecBombBlastScaleVectorX = 0;
// GLOBAL: FF7 0x00b64fe4
int g_BattleSpecBombBlastScaleVectorY = 0;
// GLOBAL: FF7 0x00b64fe8
int g_BattleSpecBombBlastScaleVectorZ = 0;
unsigned char g_bssPad_00b64fec[4] = {0};
// GLOBAL: FF7 0x00b64ff0
struct GfxTableCursor *g_BattleSpecBombBlastLoadedResTable = 0;
unsigned char g_bssPad_00b64ff4[4] = {0};
// clang-format on

/*
 * When the engine context exists and g_BattleSpecBombBlastLoadedResTable is empty, builds SPECIAL/JIBAKU2 with a default-
 * duration record and creates resource type 11 using g_BattleSpecBombBlastResourceDefs.
 */
// FUNCTION: FF7 0x00537340
void __cdecl BattleFx_SpecialLoadResBombBlast(void)
{
    GfxModelTableBank *curOwnerCtx;
    GfxResourceRequest reqBuffer;
    int kind;
    char pathText[200];
    GfxResourceRequest *pRequest;
    int resourceDefsWord;

    if (g_pBattle3dScene != 0 && g_BattleSpecBombBlastLoadedResTable == 0) {
        kind = 0xB;
        resourceDefsWord = (int)&g_BattleSpecBombBlastResourceDefs;
        curOwnerCtx = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRequest = &reqBuffer;
        Gfx_InitResourceRequest(pRequest);
        pRequest->groupTag = 0;
        // STRING: FF7 0x008a2fec
        pRequest->resourcePath = BattleFx_BuildResourcePath("SPECIAL/JIBAKU2", pathText);
        Gfx_LoadModelTableIfAbsent(kind, resourceDefsWord, pRequest, &g_BattleSpecBombBlastLoadedResTable, curOwnerCtx);
    }
}

/*
 * Forwards both arguments to BattleFx_SpecialBeginBombBlast.
 */
// FUNCTION: FF7 0x005373d0
void __cdecl BattleFx_SpecialEnterBombBlast(int targetMask, int actorIdx)
{
    BattleFx_SpecialBeginBombBlast(targetMask, actorIdx);
}

/*
 * Calls BattleFx_SpecialLoadResBombBlast, creates a task for BattleFx_SpecialSpawnBombBlastPerTarget with mode 4, dispatches event 0x13E for
 * targetIndex, and allocates a slot for BattleFx_SpecialBombBlastScreenRampTimerTask.
 */
// FUNCTION: FF7 0x005373e5
void __cdecl BattleFx_SpecialBeginBombBlast(int targetMask, int actorIdx)
{
    BattleFx_SpecialLoadResBombBlast();
    Battle_CreateBitPatternCallbackTask(targetMask, actorIdx, 4, BattleFx_SpecialSpawnBombBlastPerTarget);
    Battle_PlaySfxPannedToActor(0x13E, (short)actorIdx, 0);
    Battle_AllocateResourceSlot(BattleFx_SpecialBombBlastScreenRampTimerTask);
}

/*
 * Timing-only task; it draws nothing. On the first frame it clears the three colour channels of
 * g_BattleFxTimelineState and calls Battle_StartFixedPointRamp(0x1000, 0x14). It then counts frames, and
 * at frame 0x28 calls Battle_StartFixedPointRamp(0, 0x14) and frees its slot.
 */
// FUNCTION: FF7 0x00537427
void __cdecl BattleFx_SpecialBombBlastScreenRampTimerTask(void)
{
    BattleTaskSlot *task;

    task = BATTLE_CURRENT_TASK_SLOT;
    if (task->frameCounter == 0) {
        g_BattleFxTimelineState.channel2 = 0;
        g_BattleFxTimelineState.channel1 = g_BattleFxTimelineState.channel2;
        g_BattleFxTimelineState.red = g_BattleFxTimelineState.channel1;
        Battle_StartFixedPointRamp(0x1000, 0x14);
    }
    (task->frameCounter)++;
    if (task->frameCounter == 0x28) {
        Battle_StartFixedPointRamp(0, 0x14);
        task->ownerSlot = BATTLE_TASK_FREE;
    }
}

// Allocates a BattleFx_SpecialBombBlastTimelineTask slot. The world position of targetIndex's
// targetBone goes to the slot's position, and actor and targetIndex are stored in its
// actorMask and targetIndex fields.
// FUNCTION: FF7 0x005374aa
void __cdecl BattleFx_SpecialSpawnBombBlastPerTarget(int actor, int targetIndex)
{
    BattleFxBombBlastTimelineSlot *taskSlot;

    taskSlot = (BattleFxBombBlastTimelineSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialBombBlastTimelineTask);
    Battle_GetBoneWorldPos(targetIndex, g_BattleActorRecords[targetIndex].targetBone, &taskSlot->posA);
    taskSlot->actorMask = actor;
    taskSlot->targetIndex = targetIndex;
}

/*
 * Timeline task of one target: over 0x32 frames it spawns the animated sprites, the bouncing and
 * radial-spark sprites, the growing spinning models and the grow-fade sprites.
 */
// FUNCTION: FF7 0x00537507
void __cdecl BattleFx_SpecialBombBlastTimelineTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[72];
        SVECTOR rotX;
        struct { unsigned char pad_to_8[0x8]; int screenDepth; };
        struct { unsigned char pad_to_C[0xC]; SVECTOR offsetX; };
        struct { unsigned char pad_to_14[0x14]; PsxMatrix spinMatrix; };
        struct { unsigned char pad_to_34[0x34]; BattleFxBombBlastTimelineSpawnSlot *spawnSlot; };
        struct { unsigned char pad_to_3C[0x3C]; short angleStep; };
        struct { unsigned char pad_to_40[0x40]; short angle; };
        struct { unsigned char pad_to_44[0x44]; BattleFxBombBlastTimelineSlot *slot; };
    } frame;
#pragma pack(pop)

    frame.slot = (BattleFxBombBlastTimelineSlot *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.slot->frameCounter < 0xF) {
        frame.spawnSlot = (BattleFxBombBlastTimelineSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialBombBlastAnimatedSpriteTask);
        frame.spawnSlot->posA = frame.slot->posA + rand() % 0x4B0 + -0x258;
        frame.spawnSlot->posB = frame.slot->posB + rand() % 0x4B0 + -0x258;
        frame.spawnSlot->posC = frame.slot->posC + rand() % 0x4B0 + -0x258;
        frame.spawnSlot->variant = (rand() % 4);
    }
    if (frame.slot->frameCounter == 0xF) {
        for (frame.angleStep = 0; frame.angleStep < 0xC; frame.angleStep++) {
            frame.spawnSlot = (BattleFxBombBlastTimelineSpawnSlot *)&g_BattleTaskSlotBuffer.secondarySlots[(short)Battle_AllocateSecondaryResourceSlot(BattleFx_SpecialBombBlastBouncingSpriteTask)];
            frame.spawnSlot->posA = frame.slot->posA + rand() % 0x190 + -0xC8;
            frame.spawnSlot->posC = frame.slot->posC + rand() % 0x190 + -0xC8;
            frame.spawnSlot->posB = 0;
            BattleFx_MakeRandomConeVector(0xFFFFFED4, 0x64, &frame.spawnSlot->velA);
            frame.spawnSlot->variant = rand() % 0x14;
        }
        for (frame.angleStep = 0; frame.angleStep < 0xC; frame.angleStep++) {
            frame.spawnSlot = (BattleFxBombBlastTimelineSpawnSlot *)&g_BattleTaskSlotBuffer.secondarySlots[(short)Battle_AllocateSecondaryResourceSlot(BattleFx_SpecialBombBlastRadialSparkSpriteTask)];
            frame.spawnSlot->posA = frame.slot->posA;
            frame.spawnSlot->posB = 0;
            frame.spawnSlot->posC = frame.slot->posC;
            frame.angle = (-rand() % 1024);
            frame.spawnSlot->angle = frame.angle;
            frame.offsetX.vx = CosFixed(frame.angle) * 0xC8 >> 0xC;
            frame.offsetX.vy = SinFixed(frame.angle) * 0xC8 >> 0xC;
            frame.offsetX.vz = 0;
            frame.rotX.vz = 0;
            frame.rotX.vx = frame.rotX.vz;
            frame.rotX.vy = frame.angleStep << 8;
            frame.spawnSlot->variant = frame.rotX.vy;
            RotMatrix(&frame.rotX, &frame.spinMatrix);
            frame.spinMatrix.t[2] = 0;
            frame.spinMatrix.t[1] = frame.spinMatrix.t[2];
            frame.spinMatrix.t[0] = frame.spinMatrix.t[1];
            SetRotMatrix(&frame.spinMatrix);
            SetTransMatrix(&frame.spinMatrix);
            RotTransPointToInt16(&frame.offsetX, &frame.spawnSlot->velA, &frame.screenDepth);
        }
        Battle_PlaySfxPannedToActor(0x98, (short)frame.slot->targetIndex, 0);
    }
    if (frame.slot->frameCounter >= 0xF && frame.slot->frameCounter < 0x1B && (frame.slot->frameCounter % 4) == 0) {
        frame.angle = frame.slot->frameCounter - 0xF >> 2;
        frame.spawnSlot = (BattleFxBombBlastTimelineSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialBombBlastGrowingSpinningModelTask);
        frame.spawnSlot->posA = frame.slot->posA + (SinFixed(frame.angle * 0x555) * 0x4B0 >> 0xC);
        frame.spawnSlot->posB = 0;
        frame.spawnSlot->posC = frame.slot->posC + (CosFixed(frame.angle * 0x555) * 0x4B0 >> 0xC);
    }
    if (frame.slot->frameCounter > 0xF && frame.slot->frameCounter < 0x15) {
        for (frame.angleStep = 0; frame.angleStep < 4; frame.angleStep++) {
            frame.spawnSlot = (BattleFxBombBlastTimelineSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialBombBlastGrowFadeSpriteTask);
            frame.spawnSlot->posA = frame.slot->posA + rand() % 0x640 + -0x320;
            frame.spawnSlot->posB = -rand() % 0x4B0;
            frame.spawnSlot->posC = frame.slot->posC + rand() % 0x640 + -0x320;
        }
        for (frame.angleStep = 0; frame.angleStep < 8; frame.angleStep++) {
            frame.angle = (rand() % 4096);
            frame.spawnSlot = (BattleFxBombBlastTimelineSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialBombBlastAnimatedSpriteTask);
            frame.spawnSlot->posA = frame.slot->posA;
            frame.spawnSlot->posB = 0xFF38;
            frame.spawnSlot->posC = frame.slot->posC;
            frame.spawnSlot->velA = SinFixed(frame.angle) * 0x3C >> 0xC;
            frame.spawnSlot->velC = CosFixed(frame.angle) * 0x3C >> 0xC;
            frame.spawnSlot->velB = -rand() % 0x28 - 0x78;
            frame.spawnSlot->variant = 5;
        }
    }
    if (frame.slot->frameCounter > 0x20 && frame.slot->frameCounter < 0x2C) {
        frame.spawnSlot = (BattleFxBombBlastTimelineSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialBombBlastAnimatedSpriteTask);
        frame.spawnSlot->posA = frame.slot->posA + rand() % 0x4B0 + -0x258;
        frame.spawnSlot->posB = frame.slot->posB + rand() % 0x4B0 + -0x258;
        frame.spawnSlot->posC = frame.slot->posC + rand() % 0x4B0 + -0x258;
        frame.spawnSlot->variant = 4;
        frame.spawnSlot = (BattleFxBombBlastTimelineSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialBombBlastAnimatedSpriteTask);
        frame.spawnSlot->posA = frame.slot->posA + rand() % 0x4B0 + -0x258;
        frame.spawnSlot->posB = frame.slot->posB + rand() % 0x4B0 + -0x258;
        frame.spawnSlot->posC = frame.slot->posC + rand() % 0x4B0 + -0x258;
        frame.spawnSlot->variant = 6;
    }
    if (frame.slot->frameCounter == 0x28) {
        Battle_PostActorHitOrHurtMessage(frame.slot->actorMask);
    }
    (frame.slot->frameCounter)++;
    if (frame.slot->frameCounter == 0x32) {
        frame.slot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Draws one animated sprite, picking its sprite table entry from its mode, and retires it after
 * its last frame; mode 5 sprites also move by their velocity.
 */
// FUNCTION: FF7 0x00537bc3
void __cdecl BattleFx_SpecialBombBlastAnimatedSpriteTask(void)
{
    struct {
        int table[7];
        BattleFxBombBlastSprite *slot;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    frame.slot = (BattleFxBombBlastSprite *)BATTLE_CURRENT_TASK_SLOT;
    frame.table[0] = 1;
    frame.table[1] = 2;
    frame.table[2] = 3;
    frame.table[3] = 4;
    frame.table[4] = 5;
    frame.table[5] = 6;
    frame.table[6] = 7;
    BattleFx_InstallBillboardMatrix(&frame.slot->posX, (frame.slot->mode - 5 != 0 ? 0xFFFFF000 : 0) + 0x3000, 0);
    g_BattleSpecBombBlastSpriteReq1.spriteEntry = Gfx_ModelTableGetLoadedRecord(frame.table[frame.slot->mode], g_BattleSpecBombBlastLoadedResTable);
    if (frame.slot->mode != 6) {
        g_BattleSpecBombBlastSpriteReq1.drawObj = Gfx_ModelTableGetSpriteObject(frame.table[frame.slot->mode], g_BattleSpecBombBlastLoadedResTable);
    } else {
        g_BattleSpecBombBlastSpriteReq1.drawObj = Battle3d_GetSceneRenderObject(3, g_pBattle3dScene);
    }
    g_BattleSpecBombBlastSpriteReq1.frame = frame.slot->frameIndex;
    g_BattleSpecBombBlastPacketCursor = BattleFx_SubmitSpriteDraw(&g_BattleSpecBombBlastSpriteReq1, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecBombBlastPacketCursor);
    if (g_BattleIsPaused == 0) {
        if (frame.slot->mode == 5) {
            frame.slot->posX += frame.slot->velocityX;
            frame.slot->posY += frame.slot->velocityY;
            frame.slot->posZ += frame.slot->velocityZ;
        }
        if (g_BattleSpecBombBlastSpriteReq1.spriteEntry != 0) {
            (frame.slot->frameIndex)++;
            if (frame.slot->frameIndex >= *(int *)((char *)*(int *)g_BattleSpecBombBlastSpriteReq1.spriteEntry + 4)) {
                frame.slot->ownerSlot = 0xFFFF;
            }
        }
    }
}

/*
 * Per-frame step that grows and fades a sprite. Reads entry 0xA of g_BattleSpecBombBlastLoadedResTable
 * into the request's second sprite, installs the billboard matrix at the slot's position with the
 * scale ramping as frame * 0xC0 + 0x2000, and writes the frame counter clamped at 7 into
 * g_BattleSpecBombBlastSpriteReq2.frame. The three colour bytes of g_BattleSpecBombBlastSpriteReq2
 * all get 0x60 - (frame << 2). Submits with the cursor in g_BattleSpecBombBlastPacketCursor, then,
 * while g_BattleIsPaused is zero, advances the frame and frees the slot at frame 0x18.
 */
// FUNCTION: FF7 0x00537d63
void __cdecl BattleFx_SpecialBombBlastGrowFadeSpriteTask(void)
{
    BattleFxBombBlastBouncingSpriteSlot *taskPtr;
    unsigned short spriteFrame;
    int cap;

    taskPtr = (BattleFxBombBlastBouncingSpriteSlot *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecBombBlastSpriteReq1.secondSpriteResource = Gfx_ModelTableGetLoadedRecord(0xA, g_BattleSpecBombBlastLoadedResTable);
    g_BattleSpecBombBlastSpriteReq1.secondDrawObj = Gfx_ModelTableGetSpriteObject(0xA, g_BattleSpecBombBlastLoadedResTable);
    if (taskPtr->frameCounter < 8) {
        cap = taskPtr->frameCounter;
    } else {
        cap = 7;
    }
    spriteFrame = cap;
    BattleFx_InstallBillboardMatrix(&taskPtr->posA, taskPtr->frameCounter * 0xC0 + 0x2000, 0);
    g_BattleSpecBombBlastSpriteReq2.frame = spriteFrame;
    g_BattleSpecBombBlastSpriteReq2.blue = 0x60 - (taskPtr->frameCounter << 2);
    g_BattleSpecBombBlastSpriteReq2.green = g_BattleSpecBombBlastSpriteReq2.blue;
    g_BattleSpecBombBlastSpriteReq2.red = g_BattleSpecBombBlastSpriteReq2.green;
    g_BattleSpecBombBlastPacketCursor = BattleFx_SubmitSpriteDraw(&g_BattleSpecBombBlastSpriteReq1.secondSpriteResource, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecBombBlastPacketCursor);
    if (g_BattleIsPaused == 0) {
        (taskPtr->frameCounter)++;
        if (taskPtr->frameCounter == 0x18) {
            taskPtr->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Draws one bouncing spark sprite under the camera and moves it with its velocity, flipping its
 * vertical velocity when it hits the ground.
 */
// FUNCTION: FF7 0x00537e84
void __cdecl BattleFx_SpecialBombBlastBouncingSpriteTask(void)
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
        struct { unsigned char pad_to_50[0x50]; short componentIndex; };
        struct { unsigned char pad_to_54[0x54]; int screenDepth; };
        struct { unsigned char pad_to_58[0x58]; BattleFxBombBlastBouncingSpriteSlot *slot; };
    } frame;
#pragma pack(pop)

    frame.slot = (BattleFxBombBlastBouncingSpriteSlot *)&g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx];
    g_BattleSpecBombBlastSpriteReq2.drawBlock = Gfx_ModelTableGetLoadedRecord(8, g_BattleSpecBombBlastLoadedResTable);
    g_BattleSpecBombBlastSpriteReq2.handle = Gfx_ModelTableGetSpriteObject(8, g_BattleSpecBombBlastLoadedResTable);
    for (frame.componentIndex = 0; frame.componentIndex < 3; frame.componentIndex++) {
        frame.scaleMatrix.m[2][frame.componentIndex] = 0;
        frame.scaleMatrix.m[1][frame.componentIndex] = 0;
        frame.scaleMatrix.m[0][frame.componentIndex] = 0;
    }
    if (frame.slot->frameCounter < 7) {
        frame.phaseValue = 8;
    } else {
        frame.phaseValue = 0xF - frame.slot->frameCounter;
    }
    frame.phase = *(short *)&frame.phaseValue;
    frame.rotX.vy = 0;
    frame.rotX.vx = frame.rotX.vy;
    frame.rotX.vz = frame.slot->angle;
    SetRotMatrix(&g_BattleCameraTransform);
    SetTransMatrix(&g_BattleCameraTransform);
    RotTransPoint((SVECTOR *)&frame.slot->posA, frame.scaleMatrix.t, &frame.screenDepth);
    frame.scaleMatrix.m[2][2] = frame.slot->variant < 4 ? 0x2000 : 0x2800;
    frame.scaleMatrix.m[1][1] = frame.scaleMatrix.m[2][2];
    frame.scaleMatrix.m[0][0] = frame.scaleMatrix.m[1][1];
    RotMatrix(&frame.rotX, &frame.spinMatrix);
    ConcatMatrixIntoFirst(&frame.scaleMatrix, &frame.spinMatrix);
    SetRotMatrix(&frame.scaleMatrix);
    SetTransMatrix(&frame.scaleMatrix);
    g_BattleSpecBombBlastSpriteReq3.frame = frame.slot->variant;
    g_BattleSpecBombBlastSpriteReq3.code = (frame.phase - 8 != 0 ? 2 : 0) + 0x2C;
    g_BattleSpecBombBlastSpriteReq3.blue = frame.phase << 4;
    g_BattleSpecBombBlastSpriteReq3.green = g_BattleSpecBombBlastSpriteReq3.blue;
    g_BattleSpecBombBlastSpriteReq3.red = g_BattleSpecBombBlastSpriteReq3.green;
    g_BattleOverlayDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecBombBlastSpriteReq2.drawBlock, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.slot->posA += frame.slot->velA;
    frame.slot->posB += frame.slot->velB;
    frame.slot->posC += frame.slot->velC;
    if (frame.slot->posB > 0) {
        frame.slot->posB = 0;
        frame.slot->velB *= -1;
    }
    frame.slot->velB += 0x14;
    frame.slot->angle += 0x200;
    (frame.slot->frameCounter)++;
    if (frame.slot->frameCounter == 0xF) {
        frame.slot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Draws one radial spark sprite, fading it over eight frames, and moves it with its velocity.
 */
// FUNCTION: FF7 0x00538118
void __cdecl BattleFx_SpecialBombBlastRadialSparkSpriteTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[84];
        SVECTOR rotX;
        struct { unsigned char pad_to_8[0x8]; PsxMatrix spinMatrix; };
        struct { unsigned char pad_to_28[0x28]; PsxMatrix scaleMatrix; };
        struct { unsigned char pad_to_48[0x48]; short componentIndex; };
        struct { unsigned char pad_to_4C[0x4C]; int screenDepth; };
        struct { unsigned char pad_to_50[0x50]; BattleFxBombBlastRadialSparkSpriteSlot *slot; };
    } frame;
#pragma pack(pop)

    frame.slot = (BattleFxBombBlastRadialSparkSpriteSlot *)&g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx];
    g_BattleSpecBombBlastSpriteReq3.drawBlock = Gfx_ModelTableGetLoadedRecord(9, g_BattleSpecBombBlastLoadedResTable);
    g_BattleSpecBombBlastSpriteReq3.handle = Gfx_ModelTableGetSpriteObject(9, g_BattleSpecBombBlastLoadedResTable);
    for (frame.componentIndex = 0; frame.componentIndex < 3; frame.componentIndex++) {
        frame.scaleMatrix.m[2][frame.componentIndex] = 0;
        frame.scaleMatrix.m[1][frame.componentIndex] = 0;
        frame.scaleMatrix.m[0][frame.componentIndex] = 0;
    }
    frame.rotX.vz = frame.slot->angle + 0x400;
    frame.rotX.vx = 0;
    frame.rotX.vy = frame.slot->angleY;
    SetRotMatrix(&g_BattleCameraTransform);
    SetTransMatrix(&g_BattleCameraTransform);
    RotTransPoint((SVECTOR *)&frame.slot->posA, frame.scaleMatrix.t, &frame.screenDepth);
    frame.scaleMatrix.m[2][2] = 0x6000;
    frame.scaleMatrix.m[1][1] = frame.scaleMatrix.m[2][2];
    frame.scaleMatrix.m[0][0] = frame.scaleMatrix.m[1][1];
    ConcatMatrixIntoFirst(&frame.scaleMatrix, &g_BattleCameraTransform);
    RotMatrix(&frame.rotX, &frame.spinMatrix);
    ConcatMatrixIntoFirst(&frame.scaleMatrix, &frame.spinMatrix);
    SetRotMatrix(&frame.scaleMatrix);
    SetTransMatrix(&frame.scaleMatrix);
    g_BattleSpecBombBlastSpriteReq4.blue = 0x80 - (frame.slot->frameCounter << 4);
    g_BattleSpecBombBlastSpriteReq4.green = g_BattleSpecBombBlastSpriteReq4.blue;
    g_BattleSpecBombBlastSpriteReq4.red = g_BattleSpecBombBlastSpriteReq4.green;
    g_BattleOverlayDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecBombBlastSpriteReq3.drawBlock, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.slot->posA += frame.slot->velA;
    frame.slot->posB += frame.slot->velB;
    frame.slot->posC += frame.slot->velC;
    (frame.slot->frameCounter)++;
    if (frame.slot->frameCounter == 8) {
        frame.slot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Draws one spinning model that grows and fades over thirty frames.
 */
// FUNCTION: FF7 0x00538315
void __cdecl BattleFx_SpecialBombBlastGrowingSpinningModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[80];
        struct { unsigned char pad_to_4[0x4]; SVECTOR rotX; };
        struct { unsigned char pad_to_C[0xC]; PsxMatrix spinMatrix; };
        struct { unsigned char pad_to_2C[0x2C]; BattleFxOilTargetModelSlot *slot; };
        struct { unsigned char pad_to_30[0x30]; PsxMatrix drawMatrix; };
    } frame;
#pragma pack(pop)

    frame.slot = (BattleFxOilTargetModelSlot *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecBombBlastSpriteReq4.modelDraw = Gfx_ModelTableGetRenderObject(0, g_BattleSpecBombBlastLoadedResTable);
    frame.rotX.vx = 0;
    frame.rotX.vy = frame.slot->frameCounter << 6;
    frame.rotX.vz = 0;
    RotMatrix(&frame.rotX, &frame.spinMatrix);
    g_BattleSpecBombBlastScaleVectorZ = (frame.slot->frameCounter << 8) + 0x3000;
    g_BattleSpecBombBlastScaleVectorX = g_BattleSpecBombBlastScaleVectorZ;
    g_BattleSpecBombBlastScaleVectorY = (frame.slot->frameCounter << 8) + 0x2000;
    ScaleMatrix(&frame.spinMatrix, &g_BattleSpecBombBlastScaleVectorX);
    frame.spinMatrix.t[0] = frame.slot->posX;
    frame.spinMatrix.t[1] = frame.slot->posY;
    frame.spinMatrix.t[2] = frame.slot->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.spinMatrix, &frame.drawMatrix);
    SetRotMatrix(&frame.drawMatrix);
    SetTransMatrix(&frame.drawMatrix);
    g_BattleSpecBombBlastSpriteReq4.alphaWord = 0x80 - (frame.slot->frameCounter << 2);
    g_BattleSpecBombBlastPacketCursor = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecBombBlastSpriteReq4.modelDraw, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecBombBlastPacketCursor);
    if (g_BattleIsPaused != 0) {
        return;
    }
    (frame.slot->frameCounter)++;
    if (frame.slot->frameCounter == 0x1E) {
        frame.slot->ownerSlot = BATTLE_TASK_FREE;
    }
}
} /* extern "C" */
