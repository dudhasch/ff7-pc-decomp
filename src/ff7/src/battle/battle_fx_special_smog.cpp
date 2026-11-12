// src/ff7/src/battle/battle_fx_special_smog.cpp
/* Enemy Special Attack [124]: Smog (orig. SPECIAL/SMOG) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle special effect for the Smog attack: it loads resources and spawns a per-target
//   timeline that drives animated, drifting ring and drifting fade sprite tasks, and draws a
//   textured quad strip.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/api/battle.h"
#include "ff7/battle_tasks.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_3d.h"
#include "ff7/types/battle_special.h"
#include "lib/types/graphics.h"
#include "ff7/camera_script.h"
#include <string.h>
#include <stdlib.h>
#include "lib/codegen.h"
extern "C" {

struct GfxPsxSpriteQuadState;

#pragma intrinsic(memset)
#pragma intrinsic(memcpy)
#define PACKED_FRAME_MEMBER(type, field, padding) \
    struct {                                      \
        unsigned char pad[padding];               \
        type value;                               \
    } field

// clang-format off

// GLOBAL: FF7 0x008a4078
unsigned int g_BattleSpecSmogResourceDefs[50] = {0x00192a39, (unsigned int)((char *)&g_BattleSpecSmogQuadRingOffsets + 0xc), (unsigned int)&g_BattleSpecSmogResName_smoke, 0x00000000, 0x00000000, 0x00192a39, (unsigned int)((char *)&g_BattleSpecSmogResName_smoke + 0xc), (unsigned int)&g_BattleSpecSmogResName_SmokePlume, 0x00000000, 0x00000000, 0x00192a39, (unsigned int)((char *)&g_BattleSpecSmogResName_SmokePlume + 0xc), (unsigned int)&g_BattleSpecSmogResName_Burst, 0x00000000, 0x00000000, 0x00192a39, (unsigned int)((char *)&g_BattleSpecSmogResName_Burst + 0xc), (unsigned int)&g_BattleSpecSmogResName_hit1, 0x00000000, 0x00000000, 0x00192a39, (unsigned int)((char *)&g_BattleSpecSmogResName_hit1 + 0xc), (unsigned int)&g_BattleSpecSmogResName_elehit1, 0x00000000, 0x00000000, 0x00192a39, (unsigned int)((char *)&g_BattleSpecSmogResName_elehit1 + 0xc), (unsigned int)&g_BattleSpecSmogResName_elehit2, 0x00000000, 0x00000000, 0x00192a39, (unsigned int)((char *)&g_BattleSpecSmogResName_elehit2 + 0xc), (unsigned int)&g_BattleSpecSmogResName_elehit3, 0x00000000, 0x00000000, 0x00192a39, (unsigned int)((char *)&g_BattleSpecSmogResName_elehit3 + 0xc), (unsigned int)&g_BattleSpecSmogResName_elehit4, 0x00000000, 0x00000000, 0x00192a39, 0x00000000, (unsigned int)((char *)&g_BattleSpecSmogResName_elehit4 + 0xc), 0x00000000, 0x00000000, 0x00193a39, (unsigned int)&g_BattleSpecSmogResName_SmokePlumeTim, 0x00000000, 0x00000000, 0x00000000};
// GLOBAL: FF7 0x008a4140
struct BattleFxSpriteRequest g_BattleSpecSmogSpriteReq1 = {0, 0, -128, -128, -128, 0x2c, 0, 0};

// GLOBAL: FF7 0x008a4150
unsigned char g_BattleSpecSmogQuadOffsets[14] = {0x00, 0x10, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20};
// GLOBAL: FF7 0x008a4160
unsigned char g_BattleSpecSmogAnimFrameIndices[28] = {0x02, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x008a4180
struct BattleFxSpriteRequest g_BattleSpecSmogSpriteReq2 = {0, 0, 96, 96, 96, 0x2c, 0, 0};

// GLOBAL: FF7 0x008a4190
unsigned char g_BattleSpecSmogQuadRingOffsets[22] = {0x00, 0x10, 0x00, 0x18, 0x00, 0x14, 0x00, 0x11, 0x80, 0x09, 0x00, 0x06, 0x53, 0x4d, 0x4f, 0x4b, 0x45, 0x2e, 0x54, 0x49, 0x4d, 0x00};
// GLOBAL: FF7 0x008a41a8
unsigned char g_BattleSpecSmogResName_smoke[23] =
    "SMOKE.SPT\0\0\0"
    "SMOG03.TIM";
// GLOBAL: FF7 0x008a41c0
unsigned char g_BattleSpecSmogResName_SmokePlume[22] =
    "KEMU1.SPT\0\0\0"
    "BAKU1.TIM";
// GLOBAL: FF7 0x008a41d8
unsigned char g_BattleSpecSmogResName_Burst[23] =
    "BAKU1.SPT\0\0\0"
    "SMOG03.TIM";
// GLOBAL: FF7 0x008a41f0
unsigned char g_BattleSpecSmogResName_hit1[23] =
    "HIT1.SPT\0\0\0\0"
    "ELEHIT.TIM";
// GLOBAL: FF7 0x008a4208
unsigned char g_BattleSpecSmogResName_elehit1[23] =
    "ELEHIT1.SPT\0"
    "ELEHIT.TIM";
// GLOBAL: FF7 0x008a4220
unsigned char g_BattleSpecSmogResName_elehit2[23] =
    "ELEHIT2.SPT\0"
    "ELEHIT.TIM";
// GLOBAL: FF7 0x008a4238
unsigned char g_BattleSpecSmogResName_elehit3[23] =
    "ELEHIT3.SPT\0"
    "ELEHIT.TIM";
// GLOBAL: FF7 0x008a4250
unsigned char g_BattleSpecSmogResName_elehit4[23] =
    "ELEHIT4.SPT\0"
    "JO_C01.SPT";
// GLOBAL: FF7 0x008a4268
unsigned char g_BattleSpecSmogResName_SmokePlumeTim[10] = "KEMU1.TIM";
// clang-format on


// clang-format off
// GLOBAL: FF7 0x00b6ac34
int g_BattleSpecSmogDrawListHead = 0;
// GLOBAL: FF7 0x00b6ac38
struct GfxTableCursor *g_BattleSpecSmogLoadedResTable = 0;
unsigned char g_bssPad_00b6ac3c[4] = {0};
// clang-format on

/*
 * When the battle scene exists and g_BattleSpecSmogLoadedResTable is empty, builds SPECIAL/SMOG with a default-
 * duration record and creates resource type 10 using g_BattleSpecSmogResourceDefs.
 */
// FUNCTION: FF7 0x00540b10
void __cdecl BattleFx_SpecialLoadResSmog(void)
{
    GfxModelTableBank *curOwnerCtx;
    GfxResourceRequest currentRecord;
    int kind;
    char pathText[200];
    GfxResourceRequest *pRequest;
    int resourceDefsWord;

    if (g_pBattle3dScene != 0 && g_BattleSpecSmogLoadedResTable == 0) {
        kind = 0xA;
        resourceDefsWord = (int)&g_BattleSpecSmogResourceDefs;
        curOwnerCtx = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRequest = &currentRecord;
        Gfx_InitResourceRequest(pRequest);
        pRequest->groupTag = 0;
        // STRING: FF7 0x008a4274
        pRequest->resourcePath = BattleFx_BuildResourcePath("SPECIAL/SMOG", pathText);
        Gfx_LoadModelTableIfAbsent(kind, resourceDefsWord, pRequest, &g_BattleSpecSmogLoadedResTable, curOwnerCtx);
    }
}

/*
 * Entry wrapper: calls BattleFx_SpecialLoadResSmog for setup, then passes targetIndex and sourceIndex to BattleFx_SpecialRegisterSmogPerTargetCallback.
 */
// FUNCTION: FF7 0x00540ba0
void __cdecl BattleFx_SpecialEnterSmog(int targetMask, int actorIdx)
{
    BattleFx_SpecialLoadResSmog();
    BattleFx_SpecialRegisterSmogPerTargetCallback(targetMask, actorIdx);
}

/*
 * Creates a task for BattleFx_SpecialSpawnSmogPerTarget with both arguments and mode 4, then dispatches event (0x8A,
 * sourceIndex, 0x40).
 */
// FUNCTION: FF7 0x00540bba
void __cdecl BattleFx_SpecialRegisterSmogPerTargetCallback(int targetIndex, int sourceIndex)
{
    Battle_CreateBitPatternCallbackTask(targetIndex, sourceIndex, 4, BattleFx_SpecialSpawnSmogPerTarget);
    Battle_PlaySfxPannedToActor(0x8A, (short)sourceIndex, 0x40);
}

// Allocates a slot for BattleFx_SpecialSmogTimelineTask. It writes the source actor's bone34 world
// position to the slot's start point and the target actor's targetBone world position to its
// target point, then stores actor as the slot's actorIndex.
// FUNCTION: FF7 0x00540bea
void __cdecl BattleFx_SpecialSpawnSmogPerTarget(int actor, int sourceIndex)
{
    BattleFxSmogTimelineSlot *slotPtr;

    slotPtr = (BattleFxSmogTimelineSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialSmogTimelineTask);
    Battle_GetBoneWorldPos(sourceIndex, g_BattleActorRecords[sourceIndex].bone34, (short *)&slotPtr->startX);
    Battle_GetBoneWorldPos(actor, g_BattleActorRecords[actor].targetBone, (short *)&slotPtr->targetX);
    slotPtr->actorIndex = actor;
}

/*
 * Timeline of one Smog hit: at frame 0 spawns six drifting fade sprites along the source-to-target
 * direction, at frame 8 the first burst of animated sprites and twelve drifting rings around the
 * target, then more animated sprites until frame 0x1C. Starts the hit at frame 0x19 and frees its
 * slot at frame 0x1E.
 */
// FUNCTION: FF7 0x00540c60
void __cdecl BattleFx_SpecialSmogTimelineTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[76];
        struct { unsigned char pad_to_4[0x4]; SVECTOR stepX; };
        struct { unsigned char pad_to_C[0xC]; BattleFxSmogTimelineSpawnSlot *spawnSlot; };
        struct { unsigned char pad_to_14[0x14]; short spawnIndex; };
        struct { unsigned char pad_to_18[0x18]; SVECTOR deltaX; };
        struct { unsigned char pad_to_20[0x20]; BattleFxSmogTimelineSlot *timelineSlot; };
        struct { unsigned char pad_to_28[0x28]; PsxMatrix basisMatrix; };
    } frame;
#pragma pack(pop)

    frame.timelineSlot = (BattleFxSmogTimelineSlot *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.timelineSlot->frameCounter == 0) {
        frame.deltaX.vx = frame.timelineSlot->targetX - frame.timelineSlot->startX;
        frame.deltaX.vy = frame.timelineSlot->targetY - frame.timelineSlot->startY;
        frame.deltaX.vz = frame.timelineSlot->targetZ - frame.timelineSlot->startZ;
        Gte_BuildLookBasisFromDirection(&frame.deltaX.vx, frame.basisMatrix.m[0]);
        frame.stepX.vx = 0;
        frame.stepX.vy = 0;
        frame.stepX.vz = 0x12C;
        TransformVectorToInt16(&frame.basisMatrix, &frame.stepX, &frame.stepX.vx);
        for (frame.spawnIndex = 0; frame.spawnIndex < 6; frame.spawnIndex++) {
            frame.spawnSlot = (BattleFxSmogTimelineSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialSmogDriftingFadeSpriteTask);
            frame.spawnSlot->velocityX = frame.deltaX.vx / 0xE;
            frame.spawnSlot->velocityY = frame.deltaX.vy / 0xE;
            frame.spawnSlot->velocityZ = frame.deltaX.vz / 0xE;
            frame.spawnSlot->posX = frame.timelineSlot->startX + frame.stepX.vx * (6 - frame.spawnIndex);
            frame.spawnSlot->posY = frame.timelineSlot->startY + frame.stepX.vy * (6 - frame.spawnIndex);
            frame.spawnSlot->posZ = frame.timelineSlot->startZ + frame.stepX.vz * (6 - frame.spawnIndex);
            frame.spawnSlot->step = frame.spawnIndex;
        }
    }
    if (frame.timelineSlot->frameCounter == 8) {
        frame.spawnSlot = (BattleFxSmogTimelineSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialSmogAnimatedSpriteTask);
        frame.spawnSlot->posX = frame.timelineSlot->targetX + rand() % 0x258 + -0x12C;
        frame.spawnSlot->posY = frame.timelineSlot->targetY + rand() % 0x258 + -0x12C;
        frame.spawnSlot->posZ = frame.timelineSlot->targetZ + rand() % 0x258 + -0x12C;
        frame.spawnSlot->state = 1;
        for (frame.spawnIndex = 0; frame.spawnIndex < 4; frame.spawnIndex++) {
            frame.spawnSlot = (BattleFxSmogTimelineSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialSmogAnimatedSpriteTask);
            frame.spawnSlot->posX = frame.timelineSlot->targetX + rand() % 0x4B0 + -0x258;
            frame.spawnSlot->posY = frame.timelineSlot->targetY + rand() % 0x4B0 + -0x258;
            frame.spawnSlot->posZ = frame.timelineSlot->targetZ + rand() % 0x4B0 + -0x258;
            frame.spawnSlot->state = 0;
        }
        for (frame.spawnIndex = 0; frame.spawnIndex < 0xC; frame.spawnIndex++) {
            frame.spawnSlot = (BattleFxSmogTimelineSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialSmogDriftingRingTask);
            frame.spawnSlot->posX = frame.timelineSlot->targetX;
            frame.spawnSlot->posY = frame.timelineSlot->targetY;
            frame.spawnSlot->posZ = frame.timelineSlot->targetZ;
            frame.stepX.vx = 0;
            frame.stepX.vy = 0;
            frame.stepX.vz = 0xC8;
            frame.deltaX.vx = (rand() % 4096);
            frame.deltaX.vy = (rand() % 4096);
            frame.deltaX.vz = 0;
            RotMatrix(&frame.deltaX, &frame.basisMatrix);
            TransformVectorToInt16(&frame.basisMatrix, &frame.stepX, (void *)&frame.spawnSlot->velocityX);
            frame.spawnSlot->state = 0;
        }
    }
    if (frame.timelineSlot->frameCounter > 8 && frame.timelineSlot->frameCounter < 0x1C) {
        frame.spawnSlot = (BattleFxSmogTimelineSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialSmogAnimatedSpriteTask);
        frame.spawnSlot->posX = frame.timelineSlot->targetX + rand() % 0x3E8 + -0x1F4;
        frame.spawnSlot->posY = frame.timelineSlot->targetY + rand() % 0x3E8 + -0x1F4;
        frame.spawnSlot->posZ = frame.timelineSlot->targetZ + rand() % 0x3E8 + -0x1F4;
        frame.spawnSlot->state = 6;
        frame.spawnSlot = (BattleFxSmogTimelineSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialSmogAnimatedSpriteTask);
        frame.spawnSlot->posX = frame.timelineSlot->targetX + rand() % 0x3E8 + -0x1F4;
        frame.spawnSlot->posY = frame.timelineSlot->targetY + rand() % 0x3E8 + -0x1F4;
        frame.spawnSlot->posZ = frame.timelineSlot->targetZ + rand() % 0x3E8 + -0x1F4;
        frame.spawnSlot->state = (rand() % 4) + 2;
    }
    if (frame.timelineSlot->frameCounter == 0x19) {
        Battle_PostActorHitOrHurtMessage(frame.timelineSlot->actorIndex);
    }
    (frame.timelineSlot->frameCounter)++;
    if (frame.timelineSlot->frameCounter == 0x1E) {
        frame.timelineSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Per-frame handler for an animated smog sprite, driven by the slot's variant: scale from the short table g_BattleSpecSmogQuadOffsets,
 * handle for index g_BattleSpecSmogAnimFrameIndices[variant] of g_BattleSpecSmogLoadedResTable,
 * palette from the same entry except variant 6, which uses Battle3d_GetSceneRenderObject(3,
 * g_pBattle3dScene). Submits at shift 0xC with the cursor in g_BattleSpecSmogDrawListHead, and when
 * unpaused with a nonnull sprite advances the counter and retires when it equals the header frame
 * count.
 */
// FUNCTION: FF7 0x005411b3
void __cdecl BattleFx_SpecialSmogAnimatedSpriteTask(void)
{
    BattleFxSmogSpriteSlot *task;

    task = (BattleFxSmogSpriteSlot *)BATTLE_CURRENT_TASK_SLOT;
    BattleFx_InstallBillboardMatrix(&task->posX, (short)((short *)&g_BattleSpecSmogQuadOffsets)[task->variant], 0);
    (g_BattleSpecSmogSpriteReq1.frame) = task->frameIndex;
    (g_BattleSpecSmogSpriteReq1.spriteGroup) = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(((int *)&g_BattleSpecSmogAnimFrameIndices)[task->variant], g_BattleSpecSmogLoadedResTable);
    if (task->variant != 6) {
        (g_BattleSpecSmogSpriteReq1.drawObj) = Gfx_ModelTableGetSpriteObject(((int *)&g_BattleSpecSmogAnimFrameIndices)[task->variant], g_BattleSpecSmogLoadedResTable);
    } else {
        (g_BattleSpecSmogSpriteReq1.drawObj) = Battle3d_GetSceneRenderObject(3, g_pBattle3dScene);
    }
    g_BattleSpecSmogDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecSmogSpriteReq1, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecSmogDrawListHead);
    if (g_BattleIsPaused == 0 && (g_BattleSpecSmogSpriteReq1.spriteGroup) != 0) {
        (task->frameIndex)++;
        if (task->frameIndex == g_BattleSpecSmogSpriteReq1.spriteGroup->header->frameCount) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Installs a billboard at scale 0x1800, refreshes transforms and draws the textured quad strip
 * with the ring's angle, amplitude 800 and a shade of 120 minus six times its step. Advances the
 * angle by 0x100 even while paused; when unpaused integrates the position, damps each velocity by
 * subtracting its quotient over eight, advances the step and retires at step twenty.
 */
// FUNCTION: FF7 0x005412cc
void __cdecl BattleFx_SpecialSmogDriftingRingTask(void)
{
    BattleFxSmogRing *task;

    task = (BattleFxSmogRing *)BATTLE_CURRENT_TASK_SLOT;
    BattleFx_InstallBillboardMatrix(&task->posX, 0x1800, 0);
    RefreshGlobalTransformState();
    BattleFx_SpecialSmogDrawPsxTexturedQuadStrip(8, task->angle, 0x320, 0x78 - task->step * 6);
    task->angle += 0x100;
    if (g_BattleIsPaused != 0) {
        return;
    }
    task->posX += task->velocityX;
    task->posY += task->velocityY;
    task->posZ += task->velocityZ;
    task->velocityX -= (short)(task->velocityX / 8);
    task->velocityY -= (short)(task->velocityY / 8);
    task->velocityZ -= (short)(task->velocityZ / 8);
    (task->step)++;
    if (task->step == 0x14) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Draws eight textured quads as a strip whose x offsets follow a sine wave of the given amplitude,
 * phase and per-quad angle step, shaded with the given grey level.
 */
// FUNCTION: FF7 0x00541416
void __cdecl BattleFx_SpecialSmogDrawPsxTexturedQuadStrip(short amplitude, short phase, short angleStep, int shade)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[76];
        int quadState;
        struct { unsigned char pad_to_4[0x4]; int quadAux; };
        struct { unsigned char pad_to_8[0x8]; int quadDepth; };
        struct { unsigned char pad_to_C[0xC]; short stripAttr; };
        struct { unsigned char pad_to_10[0x10]; BattleFxSmogQuadStripRenderStateBlock *renderBlock; };
        struct { unsigned char pad_to_14[0x14]; SVECTOR x3; };
        struct { unsigned char pad_to_1C[0x1C]; SVECTOR x2; };
        struct { unsigned char pad_to_24[0x24]; short stripIndex; };
        struct { unsigned char pad_to_28[0x28]; SVECTOR x1; };
        PACKED_FRAME_MEMBER(short, at34, 42);
        struct { unsigned char pad_to_30[0x30]; SVECTOR x0; };
        PACKED_FRAME_MEMBER(short, at26, 50);
        struct { unsigned char pad_to_38[0x38]; int shadeColor; };
        struct { unsigned char pad_to_3C[0x3C]; short stripBias; };
        struct { unsigned char pad_to_40[0x40]; short sinNext; };
        struct { unsigned char pad_to_44[0x44]; int quadOut; };
        struct { unsigned char pad_to_48[0x48]; short sinCur; };
    } frame;
#pragma pack(pop)

    frame.shadeColor = ((short)shade) << 0x10 | 0x2E000000 | ((short)shade) << 8 | ((short)shade);
    frame.stripBias = 0;
    frame.stripAttr = 0x78C8;
    frame.renderBlock = (BattleFxSmogQuadStripRenderStateBlock *)Gfx_GetPsxPacketScratchPtr();
    frame.renderBlock->commandCode |= 2;
    frame.renderBlock->tag = frame.renderBlock->tag & 0xFFFFFF | 0x9000000;
    frame.renderBlock->commandCode = 0x2C;
    frame.quadState = Gfx_ModelTableGetSpriteObject(9, g_BattleSpecSmogLoadedResTable);
    if (frame.quadState != 0) {
        Gfx_SetPsxPrimQuadState(0xB, 1, (GfxPsxSpriteQuadState *)frame.quadState);
    }
    for (frame.stripIndex = 0; frame.stripIndex < 8; frame.stripIndex++) {
        frame.sinCur = SinFixed(phase + frame.stripIndex * angleStep) * amplitude >> 0xC;
        frame.sinNext = SinFixed(phase + (frame.stripIndex + 1) * angleStep) * amplitude >> 0xC;
        frame.x0.vx = frame.sinCur - 0x40;
        frame.x1.vx = frame.sinCur + 0x40;
        frame.x2.vx = frame.sinNext - 0x40;
        frame.x3.vx = frame.sinNext + 0x40;
        frame.at34.value = ((frame.stripIndex << 7) / 8) - 0x40;
        frame.at26.value = frame.at34.value;
        frame.x3.vy = ((frame.stripIndex + 1 << 7) / 8) - 0x40;
        frame.x2.vy = frame.x3.vy;
        frame.x0.vx <<= 3;
        frame.x1.vx <<= 3;
        frame.x2.vx <<= 3;
        frame.x3.vx <<= 3;
        frame.at26.value <<= 3;
        frame.at34.value <<= 3;
        frame.x2.vy <<= 3;
        frame.x3.vy <<= 3;
        frame.x3.vz = 0;
        frame.x2.vz = frame.x3.vz;
        frame.x1.vz = frame.x2.vz;
        frame.x0.vz = frame.x1.vz;
        frame.quadDepth = Gfx_TransformQuadReturnDepthAlt(&frame.x0, &frame.x1, &frame.x2, &frame.x3, &frame.quadOut, &frame.quadAux);
        if (frame.quadDepth > 0) {
            frame.renderBlock->tag = 0x9000000;
            frame.renderBlock->color = frame.shadeColor;
            frame.renderBlock->u2 = 0;
            frame.renderBlock->u0 = 0;
            frame.renderBlock->u3 = 0x80;
            frame.renderBlock->u1 = 0x80;
            frame.renderBlock->stripTop = (frame.stripIndex << 7) / 8;
            frame.renderBlock->v0 = frame.renderBlock->stripTop;
            frame.renderBlock->stripBottom = (frame.stripIndex + 1 << 7) / 8;
            frame.renderBlock->v2 = frame.renderBlock->stripBottom;
            frame.renderBlock->tpage = frame.stripBias;
            frame.renderBlock->clut = frame.stripAttr;
            Gfx_DrawPsxPrimitive(frame.renderBlock);
        }
    }
    g_BattleSpecSmogDrawListHead = (int)frame.renderBlock;
}

// Per-frame sprite step for a drifting, fading smog sprite. It takes the sprite group and draw
// object from entry 0 of g_BattleSpecSmogLoadedResTable and installs a billboard matrix
// at a scale from g_BattleSpecSmogQuadRingOffsets indexed by ringIndex, times 6 and shifted right
// by 2. The three alpha bytes all take 0x80 minus frame << 2, and the sprite is submitted at shift
// 0xC with the cursor in g_BattleSpecSmogDrawListHead. Unless the battle is paused, it adds the
// velocity to the position and retires the slot when frame reaches 8.
// FUNCTION: FF7 0x005416d9
void __cdecl BattleFx_SpecialSmogDriftingFadeSpriteTask(void)
{
    BattleFxSmogDriftingFadeSprite *task;

    task = (BattleFxSmogDriftingFadeSprite *)BATTLE_CURRENT_TASK_SLOT;
    (g_BattleSpecSmogSpriteReq2.spriteGroup) = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(0, g_BattleSpecSmogLoadedResTable);
    (g_BattleSpecSmogSpriteReq2.drawObj) = Gfx_ModelTableGetSpriteObject(0, g_BattleSpecSmogLoadedResTable);
    BattleFx_InstallBillboardMatrix(&task->posX, ((short *)&g_BattleSpecSmogQuadRingOffsets)[task->ringIndex] * 6 >> 2, 0);
    (g_BattleSpecSmogSpriteReq2.colorB) = 0x80 - (task->frame << 2);
    (g_BattleSpecSmogSpriteReq2.colorG) = (g_BattleSpecSmogSpriteReq2.colorB);
    (g_BattleSpecSmogSpriteReq2.colorR) = (g_BattleSpecSmogSpriteReq2.colorG);
    g_BattleSpecSmogDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecSmogSpriteReq2, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecSmogDrawListHead);
    if (g_BattleIsPaused != 0) {
        return;
    }
    task->posX += task->velocityX;
    task->posY += task->velocityY;
    task->posZ += task->velocityZ;
    (task->frame)++;
    if (task->frame == 8) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
}
} /* extern "C" */
