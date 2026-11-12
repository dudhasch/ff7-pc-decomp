// src/ff7/src/battle/battle_fx_special_tail_laser.cpp
/* Enemy Special Effect [idx 76]: TailLaser (JP: tele / テールレーザー (Tail Laser, TAIL02.TIM / REZA.RSD), asset: "SPECIAL/TELE") — Tail Laser (0x11e, Guard Scorpion; 0x36f, Jenova*BIRTH), Lapis Laser (0x2f2..0x2f5, Carry Armor), Wide Grazer (0x33d) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle special effect 'Tele': a beam effect with segmented beam, target timeline,
//   bouncing, fading and radial spark sprite tasks, and a pulsing model.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/api/battle.h"
#include "ff7/battle_tasks.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_special.h"
#include "lib/types/graphics.h"
#include "ff7/types/battle_3d.h"
#include "lib/enums/sound.h"
#include "ff7/camera_script.h"
#include <string.h>
#include "lib/api/sound.h"
#include <stdlib.h>
#include "lib/codegen.h"
extern "C" {

struct BattleFxModelDraw;

// clang-format off

// GLOBAL: FF7 0x0089e7a0
unsigned int g_BattleSpecTailLaserResourceDefs[65] = {0x001900b1, (unsigned int)&g_BattleSpecTailLaserRsdNames1, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&((BattleFxResourceSlot12 *)&g_BattleSpecTailLaserRsdNames1)->marker, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleSpecTailLaserRsdNames2, 0x00000000, 0x00000000, 0x00000000, 0x00192a39, (unsigned int)&((BattleFxResourceSlot12 *)&g_BattleSpecTailLaserRsdNames2)->marker, (unsigned int)&g_BattleSpecTailLaserSptNames1, 0x00000000, 0x00000000, 0x00192a39, (unsigned int)&((BattleFxResourceSlot12 *)&g_BattleSpecTailLaserSptNames1)->marker, (unsigned int)&g_BattleSpecTailLaserSptNames2, 0x00000000, 0x00000000, 0x00192a39, (unsigned int)&((BattleFxResourceSlot12 *)&g_BattleSpecTailLaserSptNames2)->marker, (unsigned int)&g_BattleSpecTailLaserSptNames3, 0x00000000, 0x00000000, 0x00192a39, (unsigned int)&((BattleFxResourceSlot12 *)&g_BattleSpecTailLaserSptNames3)->marker, (unsigned int)&g_BattleSpecTailLaserSptNames4, 0x00000000, 0x00000000, 0x00192a39, (unsigned int)&((BattleFxResourceSlot12 *)&g_BattleSpecTailLaserSptNames4)->marker, (unsigned int)&g_BattleSpecTailLaserSptNames5, 0x00000000, 0x00000000, 0x00192a39, (unsigned int)&((BattleFxResourceSlot12 *)&g_BattleSpecTailLaserSptNames5)->marker, (unsigned int)&g_BattleSpecTailLaserSptNames6, 0x00000000, 0x00000000, 0x00192a39, (unsigned int)&((BattleFxResourceSlot12 *)&g_BattleSpecTailLaserSptNames6)->marker, (unsigned int)&g_BattleSpecTailLaserSptNames7, 0x00000000, 0x00000000, 0x00192a39, 0x00000000, (unsigned int)&((BattleFxResourceSlot12 *)&g_BattleSpecTailLaserSptNames7)->marker, 0x00000000, 0x00000000, 0x00192a39, (unsigned int)&g_BattleSpecTailLaserTimNames1, (unsigned int)&((BattleFxResourceSlot12 *)&g_BattleSpecTailLaserTimNames1)->marker, 0x00000000, 0x00000000, 0x00192a39, (unsigned int)&g_BattleSpecTailLaserTimNames2, (unsigned int)&((BattleFxResourceSlot12 *)&g_BattleSpecTailLaserTimNames2)->marker, 0x00000000, 0x00000000};
// GLOBAL: FF7 0x0089e8a8
BattleFxSpriteRequest g_BattleSpecTailLaserSpriteReq1 = {0, 0, -128, -128, -128, 0x2c, 0, 0};
// GLOBAL: FF7 0x0089e8b8
int g_BattleSpecTailLaserModelParams[6] = {5, 6, 7, 8, 9, 10};
// GLOBAL: FF7 0x0089e8d0
BattleFxSpriteRequest g_BattleSpecTailLaserSpriteReq2 = {0, 0, -128, -128, -128, 0x2e, 0, 0};
// GLOBAL: FF7 0x0089e8e0
BattleFxSpriteRequest g_BattleSpecTailLaserSpriteReq3 = {0, 0, -128, -128, -128, 44, 0, 0};
// GLOBAL: FF7 0x0089e8f0
BattleFxSpriteRequest g_BattleSpecTailLaserSpriteReq4 = {0, 0, -128, -128, -128, 0x2c, 0, 0};
// GLOBAL: FF7 0x0089e900
struct BattleSpecTailLaserSpriteReq5 g_BattleSpecTailLaserSpriteReq5 = {0, {0xa8, 0, 0, 0, 0, 0}, 128, {0x20, 0, 0, 0}};
// GLOBAL: FF7 0x0089e910
struct BattleSpecTailLaserScaleVector g_BattleSpecTailLaserScaleVector = {{0, 0x10, 0, 0}, 0x1000, {0, 0x10, 0, 0, 0, 0, 0, 0}, 0xfc00, {0}};
// GLOBAL: FF7 0x0089e928
struct BattleSpecTailLaserSpriteReq6 g_BattleSpecTailLaserSpriteReq6 = {0, {0x28, 0, 0, 0, 0, 0}, 128, {0x20, 0, 0, 0}};
// GLOBAL: FF7 0x0089e938
unsigned char g_BattleSpecTailLaserScale8[8] = {0x00, 0x20, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00};
// GLOBAL: FF7 0x0089e940
int g_BattleSpecTailLaserScaleStep = 8192;
// GLOBAL: FF7 0x0089e948
BattleFxSpriteRequest g_BattleSpecTailLaserSpriteReq7 = {0, 0, -128, -128, -128, 0x2e, 0, 0};
// GLOBAL: FF7 0x0089e958
unsigned char g_BattleSpecTailLaserSpriteReq8[16] = {0x00, 0x00, 0x00, 0x00, 0xa8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x20, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x0089e968
unsigned char g_BattleSpecTailLaserRsdNames1[21] =
    "BAKU11.RSD\0\0"
    "TAMA.RSD";
// GLOBAL: FF7 0x0089e980
unsigned char g_BattleSpecTailLaserRsdNames2[23] =
    "REZA.RSD\0\0\0\0"
    "TAIL02.TIM";
// GLOBAL: FF7 0x0089e998
unsigned char g_BattleSpecTailLaserSptNames1[23] =
    "KOU1.SPT\0\0\0\0"
    "TAIL02.TIM";
// GLOBAL: FF7 0x0089e9b0
unsigned char g_BattleSpecTailLaserSptNames2[23] =
    "HAHEN.SPT\0\0\0"
    "TAIL02.TIM";
// GLOBAL: FF7 0x0089e9c8
unsigned char g_BattleSpecTailLaserSptNames3[23] =
    "HIT11.SPT\0\0\0"
    "TAIL02.TIM";
// GLOBAL: FF7 0x0089e9e0
unsigned char g_BattleSpecTailLaserSptNames4[23] =
    "HIT12.SPT\0\0\0"
    "TAIL03.TIM";
// GLOBAL: FF7 0x0089e9f8
unsigned char g_BattleSpecTailLaserSptNames5[23] =
    "ELEC1.SPT\0\0\0"
    "TAIL03.TIM";
// GLOBAL: FF7 0x0089ea10
unsigned char g_BattleSpecTailLaserSptNames6[23] =
    "ELEC2.SPT\0\0\0"
    "TAIL02.TIM";
// GLOBAL: FF7 0x0089ea28
unsigned char g_BattleSpecTailLaserSptNames7[22] =
    "BAKU1.SPT\0\0\0"
    "KEMU1.SPT";
// GLOBAL: FF7 0x0089ea40
unsigned char g_BattleSpecTailLaserTimNames1[23] =
    "TAIL02.TIM\0\0"
    "HIBANA.SPT";
// GLOBAL: FF7 0x0089ea58
unsigned char g_BattleSpecTailLaserTimNames2[23] =
    "TAIL03.TIM\0\0"
    "KEMU11.SPT";
// clang-format on

#pragma intrinsic(memset)
#pragma intrinsic(memcpy)


// clang-format off
// GLOBAL: FF7 0x00b5ddb0
int g_BattleSpecTailLaserSourceBonePos = 0;
// GLOBAL: FF7 0x00b5ddb4
short g_BattleSpecTailLaserSourceBonePosZ = 0;
// GLOBAL: FF7 0x00b5ddb8
int g_BattleSpecTailLaserDrawListHead = 0;
// GLOBAL: FF7 0x00b5ddbc
short g_BattleSpecTailLaserTargetMask = 0;
// GLOBAL: FF7 0x00b5ddc0
struct GfxTableCursor *g_BattleSpecTailLaserLoadedResTable = 0;
unsigned char g_bssPad_00b5ddc4[4] = {0};
// GLOBAL: FF7 0x00b5ddc8
int g_BattleSpecTailLaserRotationVector = 0;
unsigned char g_bssPad_00b5ddcc[4] = {0};
// clang-format on

/*
 * Loads the SPECIAL/TELE resources once the battle scene exists and they are not loaded yet.
 */
// FUNCTION: FF7 0x00526da0
void __cdecl BattleFx_SpecialLoadResTailLaser(void)
{
    GfxModelTableBank *curOwnerCtx;
    GfxResourceRequest currentRecord;
    int kind;
    char outSize[200];
    GfxResourceRequest *pRequest;
    int resourceDefsWord;

    if (g_pBattle3dScene != 0 && g_BattleSpecTailLaserLoadedResTable == 0) {
        kind = 0xD;
        resourceDefsWord = (int)&g_BattleSpecTailLaserResourceDefs;
        curOwnerCtx = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRequest = &currentRecord;
        Gfx_InitResourceRequest(pRequest);
        pRequest->groupTag = 0;
        // STRING: FF7 0x0089ea70
        pRequest->resourcePath = (char *)BattleFx_BuildResourcePath("SPECIAL/TELE", outSize);
        Gfx_LoadModelTableIfAbsent(kind, resourceDefsWord, pRequest, &g_BattleSpecTailLaserLoadedResTable, curOwnerCtx);
    }
}

/*
 * Loads the Tele resources and starts its timeline.
 */
// FUNCTION: FF7 0x00526e30
void __cdecl BattleFx_SpecialEnterTailLaser(int targetMask, int actorIdx)
{
    BattleFx_SpecialLoadResTailLaser();
    BattleFx_SpecialTailLaserSpawnTimeline(targetMask, actorIdx);
}

/*
 * Stores the target mask, allocates the timeline task for the source actor, and records the source
 * bone position and the centroid of the targets in it.
 */
// FUNCTION: FF7 0x00526e4a
void __cdecl BattleFx_SpecialTailLaserSpawnTimeline(int targetMask, int sourceActor)
{
    BattleFxTailLaserTimeline *slot;

    g_BattleSpecTailLaserTargetMask = targetMask;
    slot = (BattleFxTailLaserTimeline *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialTailLaserTimelineTask);
    slot->sourceActor = sourceActor;
    Battle_GetBoneWorldPos(sourceActor, g_BattleActorRecords[sourceActor].bone3a, (short *)&g_BattleSpecTailLaserSourceBonePos);
    Battle_GetActorMaskCentroid(targetMask, &slot->targetX);
}

/*
 * Per-frame handler of the Tele timeline: returns while the battle is paused. On its first frame it
 * spawns the beam task starting 0x1000 right of the targets' centroid, plays the sound and starts
 * the fixed-point ramp; after 45 frames it frees its slot and ramps back down.
 */
// FUNCTION: FF7 0x00526eb8
void __cdecl BattleFx_SpecialTailLaserTimelineTask(void)
{
    BattleFxTailLaserTimeline *timeline;
    BattleFxTailLaserBeamRecord *beamTask;

    timeline = (BattleFxTailLaserTimeline *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (timeline->age == 0) {
        beamTask = (BattleFxTailLaserBeamRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialTailLaserBeamTask);
        beamTask->position = timeline->targetX + 4096;
        beamTask->clearedWordA = 0;
        beamTask->base = timeline->targetZ;
        beamTask->velocity = 0xFDDE;
        beamTask->clearedWordC = 0;
        beamTask->clearedWordB = 0;
        SoundDispatchCommand(SOUND_SFX_PLAY_CH1, 0x40, 0x173, 0, 0, 0, 0, 0, 0);
        g_BattleFxTimelineState.channel2 = 0;
        g_BattleFxTimelineState.channel1 = g_BattleFxTimelineState.channel2;
        g_BattleFxTimelineState.red = g_BattleFxTimelineState.channel1;
        Battle_StartFixedPointRamp(0xC00, 10);
    }
    (timeline->age)++;
    if (timeline->age == 45) {
        timeline->ownerSlot = BATTLE_TASK_FREE;
        Battle_StartFixedPointRamp(0, 10);
    }
}

/*
 * Sweeps the beam along X for 15 frames, spawning its segments, bouncing and animated sprites, and
 * a target timeline for every target the beam passes.
 */
// FUNCTION: FF7 0x00526fd0
void __cdecl BattleFx_SpecialTailLaserBeamTask(void)
{
    int p;
    BattleFxTailLaserBeamRecord *beam;
    short theta;
    short ai;
    BattleFxTailLaserBeamSegment *sprite;
    int bonePos[2];

    beam = (BattleFxTailLaserBeamRecord *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecTailLaserSpriteReq7.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(3, g_BattleSpecTailLaserLoadedResTable);
    g_BattleSpecTailLaserSpriteReq7.drawObj = Gfx_ModelTableGetSpriteObject(3, g_BattleSpecTailLaserLoadedResTable);
    ((BattleFxModelDraw *)g_BattleSpecTailLaserSpriteReq8)->modelGroup = Gfx_ModelTableGetRenderObject(1, g_BattleSpecTailLaserLoadedResTable);
    BattleFx_InstallBillboardMatrix((short *)&g_BattleSpecTailLaserSourceBonePos, 0x3000 /* 3.0 */, 0);
    if (g_BattleSpecTailLaserSpriteReq7.spriteGroup != 0) {
        g_BattleSpecTailLaserSpriteReq7.frame = beam->index % g_BattleSpecTailLaserSpriteReq7.spriteGroup->header->frameCount;
    }
    g_BattleSpecTailLaserDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecTailLaserSpriteReq7, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecTailLaserDrawListHead);
    BattleFx_InstallBillboardMatrix((short *)&g_BattleSpecTailLaserSourceBonePos, 0x1000 /* 1.0 */, 0);
    SetDrawColorRGB(0, 0, 0);
    g_BattleSpecTailLaserDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecTailLaserSpriteReq8, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecTailLaserDrawListHead);
    if (g_BattleIsPaused != 0) {
        return;
    }
    sprite = (BattleFxTailLaserBeamSegment *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialTailLaserBeamSegmentTask);
    sprite->offsetX = beam->position;
    sprite->offsetY = beam->base;
    sprite->driftSpeed = 0;
    sprite->lifetime = beam->index % 2 + 3;
    beam->position += beam->velocity;
    if (beam->index % 2 == 0) {
        sprite = (BattleFxTailLaserBeamSegment *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialTailLaserBouncingSpriteTask);
        sprite->posX = beam->position;
        sprite->posY = beam->base;
        sprite->height = 0;
        theta = rand() % 0x1000;
        sprite->offsetX = CosFixed(theta) * 0x64 >> 12;
        sprite->offsetY = SinFixed(theta) * 0x64 >> 12;
        sprite->driftSpeed = -rand() % 200 - 100;
        sprite->lifetime = rand() % 16 + 4;
    }
    sprite = (BattleFxTailLaserBeamSegment *)&g_BattleTaskSlotBuffer.secondarySlots[(short)Battle_AllocateSecondaryResourceSlot(BattleFx_SpecialTailLaserAnimatedSpriteTask)];
    sprite->posX = beam->position + rand() % 200 + -100;
    sprite->posY = beam->base + rand() % 200 + -100;
    sprite->height = 0;
    sprite->lifetime = 5;
    for (ai = 0; ai < 10; ai++) {
        if ((g_BattleSpecTailLaserTargetMask & 1 << ai) != 0) {
            Battle_GetBoneWorldPos(ai, g_BattleActorRecords[ai].targetBone, (short *)bonePos);
            p = (short)bonePos[0] - beam->position;
            if (p < 0) {
                p = -p;
            }
            if (p < 600) {
                sprite = (BattleFxTailLaserBeamSegment *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialTailLaserTargetTimelineTask);
                Battle_GetBoneWorldPos(ai, g_BattleActorRecords[ai].targetBone, &((BattleFxTailLaserTargetTimelineSlot *)sprite)->originX);
                g_BattleSpecTailLaserTargetMask ^= (short)(1 << ai);
                sprite->actorId = ai;
                SoundDispatchCommand(SOUND_SFX_PLAY_CH2, 0x40, 0x176, 0, 0, 0, 0, 0, 0);
            }
        }
    }
    (beam->index)++;
    if (beam->index == 15) {
        beam->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Draws one animated sprite of a secondary task slot: its variant picks the animation from
 * g_BattleSpecTailLaserModelParams (variant 5 takes the scene's own draw object), and the slot is freed
 * when the animation has played all its frames.
 */
// FUNCTION: FF7 0x005273d4
void __cdecl BattleFx_SpecialTailLaserAnimatedSpriteTask(void)
{
    BattleFxTailLaserAnimSprite *task;

    task = (BattleFxTailLaserAnimSprite *)&g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx];
    BattleFx_InstallBillboardMatrix(&task->posX, 0x2000 /* 2.0 */, 0);
    g_BattleSpecTailLaserSpriteReq1.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(g_BattleSpecTailLaserModelParams[task->variant], g_BattleSpecTailLaserLoadedResTable);
    if (task->variant != 5) {
        g_BattleSpecTailLaserSpriteReq1.drawObj = Gfx_ModelTableGetSpriteObject(g_BattleSpecTailLaserModelParams[task->variant], g_BattleSpecTailLaserLoadedResTable);
    } else {
        g_BattleSpecTailLaserSpriteReq1.drawObj = Battle3d_GetSceneRenderObject(1, g_pBattle3dScene);
    }
    g_BattleSpecTailLaserSpriteReq1.frame = task->frame;
    g_BattleOverlayDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecTailLaserSpriteReq1, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused == 0 && g_BattleSpecTailLaserSpriteReq1.spriteGroup != 0) {
        (task->frame)++;
        if (task->frame == g_BattleSpecTailLaserSpriteReq1.spriteGroup->header->frameCount) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Draws one spark sprite that falls, bounces off the ground and fades out over 15 frames.
 */
// FUNCTION: FF7 0x005274e2
void __cdecl BattleFx_SpecialTailLaserBouncingSpriteTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[92];
        int phase;
        struct { unsigned char pad_to_4[0x4]; SVECTOR angleX; };
        struct { unsigned char pad_to_C[0xC]; PsxMatrix angleMatrix; };
        struct { unsigned char pad_to_2C[0x2C]; short phaseWord; };
        struct { unsigned char pad_to_30[0x30]; PsxMatrix scaleMatrix; };
        struct { unsigned char pad_to_50[0x50]; short axisIndex; };
        struct { unsigned char pad_to_54[0x54]; int screenDepth; };
        struct { unsigned char pad_to_58[0x58]; BattleFxTailLaserBouncingSpriteSlot *spriteSlot; };
    } frame;
#pragma pack(pop)

    frame.spriteSlot = (BattleFxTailLaserBouncingSpriteSlot *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecTailLaserSpriteReq3.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(4, g_BattleSpecTailLaserLoadedResTable);
    g_BattleSpecTailLaserSpriteReq3.drawObj = Gfx_ModelTableGetSpriteObject(4, g_BattleSpecTailLaserLoadedResTable);
    for (frame.axisIndex = 0; frame.axisIndex < 3; frame.axisIndex++) {
        frame.scaleMatrix.m[2][frame.axisIndex] = 0;
        frame.scaleMatrix.m[1][frame.axisIndex] = 0;
        frame.scaleMatrix.m[0][frame.axisIndex] = 0;
    }
    if (frame.spriteSlot->frameCounter < 7) {
        frame.phase = 8;
    } else {
        frame.phase = 0xF - frame.spriteSlot->frameCounter;
    }
    frame.phaseWord = *(short *)&frame.phase;
    frame.angleX.vy = 0;
    frame.angleX.vx = frame.angleX.vy;
    frame.angleX.vz = frame.spriteSlot->angle;
    SetRotMatrix(&g_BattleCameraTransform);
    SetTransMatrix(&g_BattleCameraTransform);
    RotTransPoint((SVECTOR *)&frame.spriteSlot->posX, frame.scaleMatrix.t, &frame.screenDepth);
    frame.scaleMatrix.m[2][2] = frame.spriteSlot->variant < 4 ? 0x1400 : 0x2000;
    frame.scaleMatrix.m[1][1] = frame.scaleMatrix.m[2][2];
    frame.scaleMatrix.m[0][0] = frame.scaleMatrix.m[1][1];
    RotMatrix(&frame.angleX, &frame.angleMatrix);
    ConcatMatrixIntoFirst(&frame.scaleMatrix, &frame.angleMatrix);
    SetRotMatrix(&frame.scaleMatrix);
    SetTransMatrix(&frame.scaleMatrix);
    g_BattleSpecTailLaserSpriteReq3.frame = frame.spriteSlot->variant;
    g_BattleSpecTailLaserSpriteReq3.code = (frame.phaseWord - 8 != 0 ? 2 : 0) + 0x2C;
    g_BattleSpecTailLaserSpriteReq3.colorB = frame.phaseWord << 4;
    g_BattleSpecTailLaserSpriteReq3.colorG = g_BattleSpecTailLaserSpriteReq3.colorB;
    g_BattleSpecTailLaserSpriteReq3.colorR = g_BattleSpecTailLaserSpriteReq3.colorG;
    g_BattleOverlayDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecTailLaserSpriteReq3, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.spriteSlot->posX += frame.spriteSlot->velocityX;
    frame.spriteSlot->posY += frame.spriteSlot->velocityY;
    frame.spriteSlot->posZ += frame.spriteSlot->velocityZ;
    if (frame.spriteSlot->posY > 0) {
        frame.spriteSlot->posY = 0;
        frame.spriteSlot->velocityY *= -1;
    }
    frame.spriteSlot->velocityY += 20;
    frame.spriteSlot->angle += 0x200 /* 45 deg */;
    (frame.spriteSlot->frameCounter)++;
    if (frame.spriteSlot->frameCounter == 15) {
        frame.spriteSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Draws one beam segment model from the source bone to its position for 4 frames.
 */
// FUNCTION: FF7 0x00527776
void __cdecl BattleFx_SpecialTailLaserBeamSegmentTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[112];
        PsxMatrix beamMatrix;
        struct { unsigned char pad_to_20[0x20]; SVECTOR relX; };
        struct { unsigned char pad_to_28[0x28]; BattleFxTailLaserBeamSegmentSlot *beamSlot; };
        struct { unsigned char pad_to_2C[0x2C]; PsxMatrix lookBasis; };
        struct { unsigned char pad_to_4C[0x4C]; PsxMatrix worldMatrix; };
        struct { unsigned char pad_to_6C[0x6C]; short beamLength; };
    } frame;
#pragma pack(pop)

    frame.beamSlot = (BattleFxTailLaserBeamSegmentSlot *)BATTLE_CURRENT_TASK_SLOT;
    (g_BattleSpecTailLaserSpriteReq5.resField) = Gfx_ModelTableGetRenderObject(2, g_BattleSpecTailLaserLoadedResTable);
    RotMatrix((SVECTOR *)&g_BattleSpecTailLaserScaleVector.matrix, &frame.beamMatrix);
    frame.relX.vx = frame.beamSlot->posX - ((BattleSpecTailLaserSourceBonePos *)&g_BattleSpecTailLaserSourceBonePos)->sourceX;
    frame.relX.vy = frame.beamSlot->posY - ((BattleSpecTailLaserSourceBonePos *)&g_BattleSpecTailLaserSourceBonePos)->sourceY;
    frame.relX.vz = frame.beamSlot->posZ - g_BattleSpecTailLaserSourceBonePosZ;
    frame.beamLength = SqrtToInt(frame.relX.vx * frame.relX.vx + frame.relX.vy * frame.relX.vy + frame.relX.vz * frame.relX.vz);
    Gte_BuildLookBasisFromDirection(&frame.relX.vx, frame.lookBasis.m[0]);
    ConcatMatrixPreservingTranslation(&frame.lookBasis, &frame.beamMatrix, &frame.beamMatrix);
    g_BattleSpecTailLaserScaleVector.y = (frame.beamLength << 12) / 2600;
    ScaleMatrix(&frame.beamMatrix, (int *)&g_BattleSpecTailLaserScaleVector);
    frame.beamMatrix.t[0] = ((BattleSpecTailLaserSourceBonePos *)&g_BattleSpecTailLaserSourceBonePos)->sourceX;
    frame.beamMatrix.t[1] = ((BattleSpecTailLaserSourceBonePos *)&g_BattleSpecTailLaserSourceBonePos)->sourceY;
    frame.beamMatrix.t[2] = g_BattleSpecTailLaserSourceBonePosZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.beamMatrix, &frame.worldMatrix);
    SetRotMatrix(&frame.worldMatrix);
    SetTransMatrix(&frame.worldMatrix);
    SetDrawColorRGB(0, 0, 0);
    g_BattleSpecTailLaserSpriteReq5.req5Word10 = frame.beamSlot->frameCounter << 10;
    g_BattleSpecTailLaserDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecTailLaserSpriteReq5, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecTailLaserDrawListHead);
    if (g_BattleIsPaused != 0) {
        return;
    }
    (frame.beamSlot->frameCounter)++;
    if (frame.beamSlot->frameCounter == 4) {
        frame.beamSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Plays the hit on one target for 30 frames: radial sparks, pulsing models, growing fade sprites
 * and animated sprites around its position.
 */
// FUNCTION: FF7 0x00527932
void __cdecl BattleFx_SpecialTailLaserTargetTimelineTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[68];
        SVECTOR rotX;
        struct { unsigned char pad_to_8[0x8]; SVECTOR offsetX; };
        struct { unsigned char pad_to_10[0x10]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_30[0x30]; BattleFxTailLaserTargetTimelineSpawnSlot *spawnSlot; };
        struct { unsigned char pad_to_34[0x34]; short spawnIndex; };
        struct { unsigned char pad_to_38[0x38]; short angle; };
        struct { unsigned char pad_to_3C[0x3C]; BattleFxTailLaserTargetTimelineSlot *taskSlot; };
        struct { unsigned char pad_to_40[0x40]; short radius; };
    } frame;
#pragma pack(pop)

    frame.taskSlot = (BattleFxTailLaserTargetTimelineSlot *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.taskSlot->frameCounter == 0) {
        for (frame.spawnIndex = 0; frame.spawnIndex < 12; frame.spawnIndex++) {
            frame.spawnSlot = (BattleFxTailLaserTargetTimelineSpawnSlot *)&g_BattleTaskSlotBuffer.secondarySlots[(short)Battle_AllocateSecondaryResourceSlot(BattleFx_SpecialTailLaserRadialSparkSpriteTask)];
            frame.spawnSlot->posX = frame.taskSlot->originX;
            frame.spawnSlot->posY = 0;
            frame.spawnSlot->posZ = frame.taskSlot->originZ;
            frame.angle = (-rand() % 1024);
            frame.spawnSlot->angle = frame.angle;
            frame.offsetX.vx = CosFixed(frame.angle) * 0xA0 >> 12;
            frame.offsetX.vy = SinFixed(frame.angle) * 0xA0 >> 12;
            frame.offsetX.vz = 0;
            frame.rotX.vz = 0;
            frame.rotX.vx = frame.rotX.vz;
            frame.rotX.vy = frame.spawnIndex << 8;
            frame.spawnSlot->variant = frame.rotX.vy;
            RotMatrix(&frame.rotX, &frame.rotMatrix);
            TransformVectorToInt16(&frame.rotMatrix, &frame.offsetX, &frame.spawnSlot->velocityX);
        }
    }
    if (frame.taskSlot->frameCounter < 12 && (frame.taskSlot->frameCounter % 4) == 0) {
        frame.angle = frame.taskSlot->frameCounter >> 2;
        frame.radius = g_BattleActorRecords[frame.taskSlot->actorId].radius;
        frame.spawnSlot = (BattleFxTailLaserTargetTimelineSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialTailLaserPulsingModelTask);
        frame.spawnSlot->posX = frame.taskSlot->originX + (SinFixed(frame.angle * 1365) * frame.radius >> 12);
        frame.spawnSlot->posY = 0;
        frame.spawnSlot->posZ = frame.taskSlot->originZ + (CosFixed(frame.angle * 1365) * frame.radius >> 12);
    }
    if (frame.taskSlot->frameCounter < 6) {
        for (frame.spawnIndex = 0; frame.spawnIndex < 2; frame.spawnIndex++) {
            frame.spawnSlot = (BattleFxTailLaserTargetTimelineSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialTailLaserGrowingFadeSpriteTask);
            frame.spawnSlot->posX = frame.taskSlot->originX + rand() % 1600 + -800;
            frame.spawnSlot->posY = -rand() % 500;
            frame.spawnSlot->posZ = frame.taskSlot->originZ + rand() % 1600 + -800;
        }
    }
    if (frame.taskSlot->frameCounter > 18 && frame.taskSlot->frameCounter < 30) {
        frame.spawnSlot = (BattleFxTailLaserTargetTimelineSpawnSlot *)&g_BattleTaskSlotBuffer.secondarySlots[(short)Battle_AllocateSecondaryResourceSlot(BattleFx_SpecialTailLaserAnimatedSpriteTask)];
        frame.spawnSlot->posX = frame.taskSlot->originX + rand() % 1200 + -600;
        frame.spawnSlot->posY = frame.taskSlot->originY + -rand() % 500 + -200;
        frame.spawnSlot->posZ = frame.taskSlot->originZ + rand() % 1200 + -600;
        frame.spawnSlot->variant = (rand() % 2) + 2;
        frame.spawnSlot = (BattleFxTailLaserTargetTimelineSpawnSlot *)&g_BattleTaskSlotBuffer.secondarySlots[(short)Battle_AllocateSecondaryResourceSlot(BattleFx_SpecialTailLaserAnimatedSpriteTask)];
        frame.spawnSlot->posX = frame.taskSlot->originX + rand() % 1200 + -600;
        frame.spawnSlot->posY = frame.taskSlot->originY + -rand() % 500 + -200;
        frame.spawnSlot->posZ = frame.taskSlot->originZ + rand() % 1200 + -600;
        frame.spawnSlot->variant = (rand() % 2);
    }
    if (frame.taskSlot->frameCounter == 20) {
        Battle_PostActorHitOrHurtMessage(frame.taskSlot->actorId);
    }
    (frame.taskSlot->frameCounter)++;
    if (frame.taskSlot->frameCounter == 30) {
        frame.taskSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Draws one billboard sprite that grows and fades over its 0x18-frame life, using animation 0xC.
 */
// FUNCTION: FF7 0x00527d94
void __cdecl BattleFx_SpecialTailLaserGrowingFadeSpriteTask(void)
{
    BattleTaskSlot *sprite;
    short loopIndex;
    int frameVal;

    sprite = BATTLE_CURRENT_TASK_SLOT;
    (g_BattleSpecTailLaserSpriteReq2.spriteGroup) = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(12, g_BattleSpecTailLaserLoadedResTable);
    g_BattleSpecTailLaserSpriteReq2.drawObj = Gfx_ModelTableGetSpriteObject(12, g_BattleSpecTailLaserLoadedResTable);
    if (sprite->frameCounter < 8) {
        frameVal = sprite->frameCounter;
    } else {
        frameVal = 7;
    }
    loopIndex = frameVal;
    BattleFx_InstallBillboardMatrix((short *)sprite->data, sprite->frameCounter * 0xC0 + 0x2000, 0);
    g_BattleSpecTailLaserSpriteReq2.frame = loopIndex;
    g_BattleSpecTailLaserSpriteReq2.colorB = 0x60 - (sprite->frameCounter << 2);
    g_BattleSpecTailLaserSpriteReq2.colorG = g_BattleSpecTailLaserSpriteReq2.colorB;
    g_BattleSpecTailLaserSpriteReq2.colorR = g_BattleSpecTailLaserSpriteReq2.colorG;
    g_BattleSpecTailLaserDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecTailLaserSpriteReq2, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecTailLaserDrawListHead);
    if (g_BattleIsPaused == 0) {
        (sprite->frameCounter)++;
        if (sprite->frameCounter == 24) {
            sprite->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Draws one spark sprite flying outward from the target for 8 frames.
 */
// FUNCTION: FF7 0x00527eb5
void __cdecl BattleFx_SpecialTailLaserRadialSparkSpriteTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[84];
        SVECTOR rotX;
        struct { unsigned char pad_to_8[0x8]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_28[0x28]; PsxMatrix localMatrix; };
        struct { unsigned char pad_to_48[0x48]; short axisIndex; };
        struct { unsigned char pad_to_4C[0x4C]; int depth; };
        struct { unsigned char pad_to_50[0x50]; BattleFxTailLaserBouncingSpriteSlot *sparkSlot; };
    } frame;
#pragma pack(pop)

    frame.sparkSlot = (BattleFxTailLaserBouncingSpriteSlot *)&g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx];
    g_BattleSpecTailLaserSpriteReq4.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(11, g_BattleSpecTailLaserLoadedResTable);
    g_BattleSpecTailLaserSpriteReq4.drawObj = Gfx_ModelTableGetSpriteObject(11, g_BattleSpecTailLaserLoadedResTable);
    for (frame.axisIndex = 0; frame.axisIndex < 3; frame.axisIndex++) {
        frame.localMatrix.m[2][frame.axisIndex] = 0;
        frame.localMatrix.m[1][frame.axisIndex] = 0;
        frame.localMatrix.m[0][frame.axisIndex] = 0;
    }
    frame.rotX.vz = frame.sparkSlot->angle + 0x400;
    frame.rotX.vx = 0;
    frame.rotX.vy = frame.sparkSlot->variant;
    SetRotMatrix(&g_BattleCameraTransform);
    SetTransMatrix(&g_BattleCameraTransform);
    RotTransPoint((SVECTOR *)&frame.sparkSlot->posX, frame.localMatrix.t, &frame.depth);
    frame.localMatrix.m[2][2] = 0x6000;
    frame.localMatrix.m[1][1] = frame.localMatrix.m[2][2];
    frame.localMatrix.m[0][0] = frame.localMatrix.m[1][1];
    ConcatMatrixIntoFirst(&frame.localMatrix, &g_BattleCameraTransform);
    RotMatrix(&frame.rotX, &frame.rotMatrix);
    ConcatMatrixIntoFirst(&frame.localMatrix, &frame.rotMatrix);
    SetRotMatrix(&frame.localMatrix);
    SetTransMatrix(&frame.localMatrix);
    g_BattleSpecTailLaserSpriteReq4.colorB = 0x80 - (frame.sparkSlot->frameCounter << 4);
    g_BattleSpecTailLaserSpriteReq4.colorG = g_BattleSpecTailLaserSpriteReq4.colorB;
    g_BattleSpecTailLaserSpriteReq4.colorR = g_BattleSpecTailLaserSpriteReq4.colorG;
    g_BattleOverlayDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecTailLaserSpriteReq4, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.sparkSlot->posX += frame.sparkSlot->velocityX;
    frame.sparkSlot->posY += frame.sparkSlot->velocityY;
    frame.sparkSlot->posZ += frame.sparkSlot->velocityZ;
    (frame.sparkSlot->frameCounter)++;
    if (frame.sparkSlot->frameCounter == 8) {
        frame.sparkSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Draws one pulsing model at its position for 30 frames.
 */
// FUNCTION: FF7 0x005280b2
void __cdecl BattleFx_SpecialTailLaserPulsingModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[68];
        PsxMatrix rotMatrix;
        struct { unsigned char pad_to_20[0x20]; BattleFxTailLaserPulseSlot *pulseSlot; };
        struct { unsigned char pad_to_24[0x24]; PsxMatrix combinedMatrix; };
    } frame;
#pragma pack(pop)

    frame.pulseSlot = (BattleFxTailLaserPulseSlot *)BATTLE_CURRENT_TASK_SLOT;
    (g_BattleSpecTailLaserSpriteReq6.modelField) = Gfx_ModelTableGetRenderObject(0, g_BattleSpecTailLaserLoadedResTable);
    RotMatrix((SVECTOR *)&g_BattleSpecTailLaserRotationVector, &frame.rotMatrix);
    g_BattleSpecTailLaserScaleStep = (SinFixed(frame.pulseSlot->pulseStep * 0x22) << 13 >> 12) + 0x2000;
    (*(int *)&g_BattleSpecTailLaserScale8) = g_BattleSpecTailLaserScaleStep;
    ScaleMatrix(&frame.rotMatrix, (int *)&g_BattleSpecTailLaserScale8);
    frame.rotMatrix.t[0] = frame.pulseSlot->posX;
    frame.rotMatrix.t[1] = frame.pulseSlot->posY;
    frame.rotMatrix.t[2] = frame.pulseSlot->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.combinedMatrix);
    SetRotMatrix(&frame.combinedMatrix);
    SetTransMatrix(&frame.combinedMatrix);
    g_BattleSpecTailLaserSpriteReq6.req6PulseWord = CosFixed(frame.pulseSlot->pulseStep * 34) << 7 >> 12;
    g_BattleSpecTailLaserDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecTailLaserSpriteReq6, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecTailLaserDrawListHead);
    if (g_BattleIsPaused != 0) {
        return;
    }
    (frame.pulseSlot->pulseStep)++;
    if (frame.pulseSlot->pulseStep == 30) {
        frame.pulseSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}
} /* extern "C" */
