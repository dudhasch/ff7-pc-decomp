// src/ff7/src/battle/battle_fx_magic_demi3.cpp
/* Magic Effect [44]: Demi3 (Japanese: グラビガ Graviga, dev tag: "MAGIC/GRAV3" / Grav3) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle visual effect for the Grav3 magic: orbiting trail sprites, growing and rotating
//   curved sprites, and segment beam records that are drawn and stepped.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/enums/sound.h"
#include "ff7/battle_tasks.h"
#include "lib/types/graphics.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_magic.h"
#include <stdlib.h>
#include "ff7/camera_script.h"
#include <string.h>
#include "ff7/api/battle.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "lib/codegen.h"
extern "C" {

struct BattleFxMagicGravSegment;

#pragma intrinsic(abs)

// clang-format off
// GLOBAL: FF7 0x008f2db8
int g_BattleMagicDemi3_ResourceDefs = 0x00192A38;
char *g_BattleFxMagicDemi3ResourceNames[1] = {
    // STRING: FF7 0x008f2e58
    "GRAVIG03.TIM"
};
unsigned int g_BattleFxMagicDemi3ResourceNames2[18] = {
    // STRING: FF7 0x008f2e68
    (unsigned int)"GRAV_2.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x008f2e74
    (unsigned int)"GRAVIG00.TIM",
    // STRING: FF7 0x008f2e84
    (unsigned int)"ELEC_4.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x008f2e90
    (unsigned int)"GRAVIG01.TIM",
    0x00000000,
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x008f2ea0
    (unsigned int)"GRAVIG02.TIM",
    0x00000000,
    0x00000000,
    0x00000000
};
// clang-format off
// clang-format on
// GLOBAL: FF7 0x00bd5b88
struct BattleFxMagicGravSegment g_BattleMagicDemi3SegmentRecords[20] = {0};
// GLOBAL: FF7 0x00bd5e58
int g_BattleMagicDemi3_DrawListHandle = 0;
// GLOBAL: FF7 0x00bd5e5c
struct GfxTableCursor *g_BattleMagicDemi3_LoadedRes = 0;
// clang-format on

// clang-format off
#undef g_BattleMagicDemi3_SpriteReq
// GLOBAL: FF7 0x008f2e08
struct BattleFxSpriteRequest g_BattleMagicDemi3_SpriteReq = {0, 0, -128, -128, -128, 0x2e, 0, 0};
// GLOBAL: FF7 0x008f2e18
unsigned char g_BattleMagicDemi3_QuadReq1[8] = {0xc0, 0xff, 0xc0, 0xff, 0x00, 0x40, 0x7f, 0x7f};
// clang-format on

// clang-format off
// GLOBAL: FF7 0x008f2e20
struct BattleMagicRemove_SpriteData g_BattleMagicDemi3_SpriteData1 = {-128, -128, -128, {0x2e, 0x4f, 0, 0xc8, 0x78}, 0, {0}};
// GLOBAL: FF7 0x008f2e30
unsigned char g_BattleMagicDemi3_QuadReq2[8] = {0xf0, 0xff, 0xf0, 0xff, 0x80, 0x40, 0x1f, 0x1f};
// clang-format on

// clang-format off
// GLOBAL: FF7 0x008f2e38
struct BattleMagicDemi2_SpriteData2 g_BattleMagicDemi3_SpriteData2 = {-128, -128, -128, {0x2e, 0x2f, 0, 0x48, 0x79}, 0, {0}, 0, 0};
// GLOBAL: FF7 0x008f2e50
struct BattleMagicDemi2_SpriteData3 g_BattleMagicDemi3_SpriteData3 = {-128, -128, -128, {0x2e}, 0, {0}};
// clang-format on

#pragma intrinsic(memset)
#pragma intrinsic(memcpy)


/*
 * Loads resources for the Grav3 magic effect when not already loaded.
 */
// FUNCTION: FF7 0x005a9630
void __cdecl BattleFx_MagicLoadResDemi3(void)
{
    GfxModelTableBank *context;
    GfxResourceRequest requestBuffer;
    int resultType;
    char pathBuffer[200];
    GfxResourceRequest *request;
    int defArr;

    if (g_pBattle3dScene != 0 && g_BattleMagicDemi3_LoadedRes == 0) {
        resultType = 4;
        defArr = (int)&g_BattleMagicDemi3_ResourceDefs;
        context = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        request = &requestBuffer;
        Gfx_InitResourceRequest(request);
        request->groupTag = 0;
        // STRING: FF7 0x008f2eb0
        request->resourcePath = BattleFx_BuildResourcePath("MAGIC/GRAV3_1", pathBuffer);
        Gfx_LoadModelTableIfAbsent(resultType, defArr, request, &g_BattleMagicDemi3_LoadedRes, context);
    }
}

/*
 * Main entry point initializing render state and per-target effects for Grav3.
 */
// FUNCTION: FF7 0x005a96c0
void __cdecl BattleFx_MagicEnterDemi3(int targetMask, int actorIdx)
{
    int index;

    BattleFx_MagicLoadResDemi3();
    g_BattleMagicDemi3_DrawListHandle = Gfx_GetPsxPacketScratchPtr();
    for (index = 0; index < 20; index++) {
        g_BattleMagicDemi3SegmentRecords[index].state = 0xFFFF;
    }
    Battle_CreateBitPatternCallbackTask(targetMask, actorIdx, 8, BattleFx_MagicSpawnDemi3PerTarget);
}

/*
 * Spawns timeline task for a single Grav3 target.
 */
// FUNCTION: FF7 0x005a9717
void __cdecl BattleFx_MagicSpawnDemi3PerTarget(int actor, int param)
{
    BattleFxGrav3Slot *effect;

    effect = (BattleFxGrav3Slot *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicDemi3TimelineTask);
    effect->timeline.targetIndex = actor;
    effect->timeline.param = param;
    Battle_GetBoneWorldPos(actor, g_BattleActorRecords[actor].targetBone, &effect->timeline.position.x);
}

/*
 * Drives the main timeline and sprite generation for Grav3.
 */
// FUNCTION: FF7 0x005a9774
void __cdecl BattleFx_MagicDemi3TimelineTask(void)
{
    struct {
        int unused0;
        int unused1;
        BattleFxGrav3Slot *output;
        int unused2;
        int unused3;
        BattleFxGrav3Slot *slot;
        BattleFxDemi3Position position;
        int time;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    frame.slot = (BattleFxGrav3Slot *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0)
        return;
    frame.time = frame.slot->timeline.time;
    if (frame.time == 0) {
        Battle_StartFixedPointRamp(0x400, 4);
        g_BattleFxTimelineState.red = -1;
        g_BattleFxTimelineState.channel1 = -1;
        g_BattleFxTimelineState.channel2 = -1;
    }
    frame.time -= 4;
    if (frame.time == 0) {
        frame.output = (BattleFxGrav3Slot *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicDemi3TrailEmitterTask);
        Battle_GetBoneWorldPos(frame.slot->timeline.param, g_BattleActorRecords[frame.slot->timeline.param].targetBone, &frame.output->trailEmitter.position.x);
        frame.position = frame.slot->timeline.position;
        frame.output->trailEmitter.velocityX = (frame.position.x - frame.output->trailEmitter.position.x) / 8;
        frame.output->trailEmitter.velocityY = (frame.position.y - frame.output->trailEmitter.position.y) / 8;
        frame.output->trailEmitter.velocityZ = (frame.position.z - frame.output->trailEmitter.position.z) / 8;
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputeScreenPanFromPosition(&frame.output->trailEmitter.position), 0x50);
    }
    if (frame.time == 12) {
        frame.output = (BattleFxGrav3Slot *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicDemi3RotatingSpriteCurveTask);
        frame.output->curve.ownerSlot = 0;
        frame.output->curve.center = frame.slot->timeline.position;
        frame.output->curve.baseRadius = 1000;
        frame.output->curve.rotation.vz = 0;
        frame.output->curve.rotation.vy = 0;
        frame.output->curve.rotation.vx = 0;
        frame.output = (BattleFxGrav3Slot *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicDemi3RotatingSpriteCurveTask);
        frame.output->curve.ownerSlot = 15;
        frame.output->curve.center = frame.slot->timeline.position;
        frame.output->curve.baseRadius = 1000;
        frame.output->curve.rotation.vx = 0;
        frame.output->curve.rotation.vy = 0x400 /* 90 deg */;
        frame.output->curve.rotation.vz = 0;
    }
    if (frame.time == 8) {
        frame.output = (BattleFxGrav3Slot *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicDemi3AnimatedSpriteTask);
        frame.output->animated.frameOffset = 0;
        frame.output->animated.position = frame.slot->timeline.position;
        frame.output->animated.deltaX = 0;
        frame.output->animated.deltaY = -500;
        frame.output->animated.pushLength = -g_BattleActorRecords[frame.slot->timeline.targetIndex].radius;
        frame.output->animated.scale = 0x2000 /* 2.0 */;
        frame.output = (BattleFxGrav3Slot *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicDemi3AnimatedSpriteTask);
        frame.output->animated.frameOffset = 2;
        frame.output->animated.position = frame.slot->timeline.position;
        frame.output->animated.deltaX = -300;
        frame.output->animated.deltaY = 0;
        frame.output->animated.pushLength = -g_BattleActorRecords[frame.slot->timeline.targetIndex].radius;
        frame.output->animated.scale = 0x2000 /* 2.0 */;
        frame.output = (BattleFxGrav3Slot *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicDemi3AnimatedSpriteTask);
        frame.output->animated.frameOffset = 4;
        frame.output->animated.position = frame.slot->timeline.position;
        frame.output->animated.deltaX = 300;
        frame.output->animated.deltaY = 200;
        frame.output->animated.pushLength = -g_BattleActorRecords[frame.slot->timeline.targetIndex].radius;
        frame.output->animated.scale = 0x2000 /* 2.0 */;
    }
    if (frame.time == 8) {
        frame.output = (BattleFxGrav3Slot *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicDemi3GrowingQuadSpriteTask);
        frame.output->quad.position = frame.slot->timeline.position;
        frame.output->quad.pushLength = -g_BattleActorRecords[frame.slot->timeline.targetIndex].radius;
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputeScreenPanFromPosition(&frame.output->quad.position), 0x4a);
    }
    if (frame.time == 8) {
        frame.output = (BattleFxGrav3Slot *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicDemi3SegmentEmitterTask);
        frame.output->segmentEmitter.position = frame.slot->timeline.position;
    }
    if (frame.time == 15)
        Battle_PostActorHitOrHurtMessage(frame.slot->timeline.targetIndex);
    if (frame.time >= 15)
        frame.slot->timeline.ownerSlot = BATTLE_TASK_FREE;
    ++frame.slot->timeline.time;
}

/*
 * Spawns and manages orbiting trail sprite particles for Grav3.
 */
// FUNCTION: FF7 0x005a9b98
void __cdecl BattleFx_MagicDemi3TrailEmitterTask(void)
{
    BattleFxDemi3TrailEmitter *taskPtr;
    BattleFxDemi3TrailSprite *sprite;

    taskPtr = (BattleFxDemi3TrailEmitter *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    taskPtr->position.x += taskPtr->velocityX;
    taskPtr->position.y += taskPtr->velocityY;
    taskPtr->position.z += taskPtr->velocityZ;
    sprite = (BattleFxDemi3TrailSprite *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicDemi3TrailSpriteTask);
    sprite->position = taskPtr->position;
    (taskPtr->frameCount)++;
    if (taskPtr->frameCount >= 8) {
        taskPtr->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Updates and renders trailing particle sprites for Grav3.
 */
// FUNCTION: FF7 0x005a9c58
void __cdecl BattleFx_MagicDemi3TrailSpriteTask(void)
{
    BattleFxDemi3TrailSprite *taskPtr;
    PsxMatrix *billboard;

    taskPtr = (BattleFxDemi3TrailSprite *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleMagicDemi3_SpriteReq.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(0, g_BattleMagicDemi3_LoadedRes);
    g_BattleMagicDemi3_SpriteReq.drawObj = Gfx_ModelTableGetSpriteObject(0, g_BattleMagicDemi3_LoadedRes);
    billboard = (struct PsxMatrix *)BattleFx_InstallBillboardMatrix(&taskPtr->position.x, 0x1200 /* 1.125 */, 0);
    if (billboard->t[2] > 0) {
        g_BattleMagicDemi3_SpriteReq.frame = taskPtr->frameCounter;
        g_BattleMagicDemi3_DrawListHandle = BattleFx_SubmitSpriteDraw(&g_BattleMagicDemi3_SpriteReq, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleMagicDemi3_DrawListHandle);
    }
    if (g_BattleIsPaused == 0) {
        (taskPtr->frameCounter)++;
        if (taskPtr->frameCounter >= 5) {
            taskPtr->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Updates and renders expanding quad sprites for Grav3.
 */
// FUNCTION: FF7 0x005a9d25
void __cdecl BattleFx_MagicDemi3GrowingQuadSpriteTask(void)
{
    int angle;
    int phaseFrame;
    BattleFxDemi3GrowingQuadSlot *task;
    PsxMatrix *matrix;

    task = (BattleFxDemi3GrowingQuadSlot *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleMagicDemi3_SpriteData1.drawObj = Gfx_ModelTableGetSpriteObject(0, g_BattleMagicDemi3_LoadedRes);
    phaseFrame = task->frameIndex;
    if (phaseFrame < 15) {
        g_BattleMagicDemi3_SpriteData1.colorB = 0x80;
        g_BattleMagicDemi3_SpriteData1.colorG = g_BattleMagicDemi3_SpriteData1.colorB;
        (g_BattleMagicDemi3_SpriteData1.colorR) = g_BattleMagicDemi3_SpriteData1.colorG;
        angle = phaseFrame * 0x2400 / 15 + 0x100;
    } else {
        phaseFrame -= 15;
        if (phaseFrame < 20) {
            g_BattleMagicDemi3_SpriteData1.colorB = 0x80;
            g_BattleMagicDemi3_SpriteData1.colorG = g_BattleMagicDemi3_SpriteData1.colorB;
            (g_BattleMagicDemi3_SpriteData1.colorR) = g_BattleMagicDemi3_SpriteData1.colorG;
            angle = 0x2500 /* 832.5 deg */;
        } else {
            phaseFrame -= 20;
            if (phaseFrame < 15) {
                g_BattleMagicDemi3_SpriteData1.colorB = phaseFrame * -0x80 / 15 + 0x80;
                g_BattleMagicDemi3_SpriteData1.colorG = g_BattleMagicDemi3_SpriteData1.colorB;
                (g_BattleMagicDemi3_SpriteData1.colorR) = g_BattleMagicDemi3_SpriteData1.colorG;
                angle = phaseFrame * 0x1B00 / 15 + 0x2500;
            } else {
                task->ownerSlot = BATTLE_TASK_FREE;
                return;
            }
        }
    }
    matrix = (struct PsxMatrix *)BattleFx_InstallBillboardMatrix(&task->position.x, (short)angle, task->pushLength);
    if (matrix->t[2] > 0) {
        g_BattleMagicDemi3_DrawListHandle = BattleFx_SubmitSpriteQuadDraw((BattleFxSpriteQuadRequest *)&g_BattleMagicDemi3_QuadReq1, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleMagicDemi3_DrawListHandle);
    }
    if (g_BattleIsPaused == 0) {
        (task->frameIndex)++;
    }
}

/*
 * Updates and renders rotating sprites along curved paths for Grav3.
 */
// FUNCTION: FF7 0x005a9ea5
void __cdecl BattleFx_MagicDemi3RotatingSpriteCurveTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[72];
        struct { unsigned char pad_to_8[0x8]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_28[0x28]; int radius; };
        struct { unsigned char pad_to_2C[0x2C]; int spriteIndex; };
        struct { unsigned char pad_to_30[0x30]; int curveAngle; };
        struct { unsigned char pad_to_34[0x34]; BattleFxDemi3CurveSprite *curveSprite; };
        struct { unsigned char pad_to_38[0x38]; SVECTOR offsetX; };
        struct { unsigned char pad_to_40[0x40]; int phase; };
        struct { unsigned char pad_to_44[0x44]; int pulseScale; };
    } frame;
#pragma pack(pop)

    frame.curveSprite = (BattleFxDemi3CurveSprite *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleMagicDemi3_SpriteData2.drawObj = Gfx_ModelTableGetSpriteObject(0, g_BattleMagicDemi3_LoadedRes);
    frame.phase = frame.curveSprite->step;
    if (frame.phase < 5) {
        g_BattleMagicDemi3_SpriteData2.colorB = (frame.phase << 7) / 5;
        g_BattleMagicDemi3_SpriteData2.colorG = g_BattleMagicDemi3_SpriteData2.colorB;
        (g_BattleMagicDemi3_SpriteData2.colorR) = g_BattleMagicDemi3_SpriteData2.colorG;
    } else {
        frame.phase -= 5;
        if (frame.phase < 30) {
            g_BattleMagicDemi3_SpriteData2.colorB = 0x80;
            g_BattleMagicDemi3_SpriteData2.colorG = g_BattleMagicDemi3_SpriteData2.colorB;
            (g_BattleMagicDemi3_SpriteData2.colorR) = g_BattleMagicDemi3_SpriteData2.colorG;
        } else {
            frame.phase -= 0x1E;
            if (frame.phase < 5) {
                g_BattleMagicDemi3_SpriteData2.colorB = frame.phase * -0x80 / 5 + 0x80;
                g_BattleMagicDemi3_SpriteData2.colorG = g_BattleMagicDemi3_SpriteData2.colorB;
                (g_BattleMagicDemi3_SpriteData2.colorR) = g_BattleMagicDemi3_SpriteData2.colorG;
            } else {
                frame.curveSprite->ownerSlot = BATTLE_TASK_FREE;
                return;
            }
        }
    }
    frame.pulseScale = (SinFixed((frame.curveSprite->ownerSlot + frame.curveSprite->step << 12) / 30) << 10 >> 12) + 0xC00;
    frame.radius = frame.curveSprite->baseRadius * frame.pulseScale >> 12;
    for (frame.spriteIndex = 0; frame.spriteIndex < 16; frame.spriteIndex++) {
        frame.curveAngle = (frame.curveSprite->step << 6) + ((frame.spriteIndex << 12) / 16);
        frame.offsetX.vx = (SinFixed(frame.curveAngle) * CosFixed(frame.curveAngle) >> 12) * frame.radius >> 12;
        frame.offsetX.vy = CosFixed(frame.curveAngle) * frame.radius >> 12;
        frame.offsetX.vz = 0;
        RotMatrix(&frame.curveSprite->rotation, &frame.rotMatrix);
        TransformVectorToInt16(&frame.rotMatrix, &frame.offsetX, &frame.offsetX.vx);
        frame.offsetX.vx += frame.curveSprite->center.x;
        frame.offsetX.vy += frame.curveSprite->center.y;
        frame.offsetX.vz += frame.curveSprite->center.z;
        BattleFx_InstallBillboardMatrix(&frame.offsetX, 0x900 /* 0.5625 */, 0);
        g_BattleMagicDemi3_DrawListHandle = BattleFx_SubmitSpriteQuadDraw((BattleFxSpriteQuadRequest *)&g_BattleMagicDemi3_QuadReq2, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleMagicDemi3_DrawListHandle);
    }
    if (g_BattleIsPaused == 0) {
        frame.curveSprite->rotation.vx += 0x50;
        frame.curveSprite->rotation.vy += 0x40;
        frame.curveSprite->rotation.vz += 0x50;
        (frame.curveSprite->step)++;
    }
}

/*
 * Updates animated billboard sprites for Grav3.
 */
// FUNCTION: FF7 0x005aa136
void __cdecl BattleFx_MagicDemi3AnimatedSpriteTask(void)
{
    int time;
    BattleFxMagicDemi3AnimatedSprite *sprite;
    PsxMatrix *billboard;

    sprite = (BattleFxMagicDemi3AnimatedSprite *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleMagicDemi3_SpriteData2.fadingRecord = Gfx_ModelTableGetLoadedRecord(1, g_BattleMagicDemi3_LoadedRes);
    g_BattleMagicDemi3_SpriteData2.fadingObj = Gfx_ModelTableGetSpriteObject(1, g_BattleMagicDemi3_LoadedRes);
    if (g_BattleMagicDemi3_SpriteData2.fadingRecord != 0) {
        ((BattleMagicDemi3SpriteData *)g_BattleMagicDemi3_SpriteData2.fadingRecord)->spriteObject1 = Gfx_ModelTableGetSpriteObject(1, g_BattleMagicDemi3_LoadedRes);
        ((BattleMagicDemi3SpriteData *)g_BattleMagicDemi3_SpriteData2.fadingRecord)->spriteObject2 = Gfx_ModelTableGetSpriteObject(2, g_BattleMagicDemi3_LoadedRes);
        ((BattleMagicDemi3SpriteData *)g_BattleMagicDemi3_SpriteData2.fadingRecord)->spriteObject3 = Gfx_ModelTableGetSpriteObject(3, g_BattleMagicDemi3_LoadedRes);
    }
    time = sprite->frameIndex;
    if (time < 5) {
        g_BattleMagicDemi3_SpriteData3.colorB = (time << 7) / 5;
        g_BattleMagicDemi3_SpriteData3.colorG = g_BattleMagicDemi3_SpriteData3.colorB;
        (g_BattleMagicDemi3_SpriteData3.colorR) = g_BattleMagicDemi3_SpriteData3.colorG;
    } else {
        time -= 5;
        if (time < 40) {
            g_BattleMagicDemi3_SpriteData3.colorB = 0x80;
            g_BattleMagicDemi3_SpriteData3.colorG = g_BattleMagicDemi3_SpriteData3.colorB;
            (g_BattleMagicDemi3_SpriteData3.colorR) = g_BattleMagicDemi3_SpriteData3.colorG;
        } else {
            time -= 40;
            if (time < 5) {
                g_BattleMagicDemi3_SpriteData3.colorB = time * -0x80 / 5 + 0x80;
                g_BattleMagicDemi3_SpriteData3.colorG = g_BattleMagicDemi3_SpriteData3.colorB;
                (g_BattleMagicDemi3_SpriteData3.colorR) = g_BattleMagicDemi3_SpriteData3.colorG;
            } else {
                sprite->frameOffset = 0xFFFF;
                return;
            }
        }
    }
    g_BattleMagicDemi3_SpriteData3.framePhase = (sprite->frameIndex + sprite->frameOffset) % 12;
    billboard = (struct PsxMatrix *)BattleFx_InstallBillboardMatrix(&sprite->position.x, (short)sprite->scale, sprite->pushLength);
    billboard->t[0] += sprite->deltaX;
    billboard->t[1] += sprite->deltaY;
    SetTransMatrix(billboard);
    g_BattleMagicDemi3_DrawListHandle = BattleFx_SubmitSpriteDraw(&g_BattleMagicDemi3_SpriteData2.fadingRecord, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleMagicDemi3_DrawListHandle);
    if (g_BattleIsPaused == 0) {
        (sprite->frameIndex)++;
    }
}

/*
 * Emits and tracks active segment beam records for Grav3.
 */
// FUNCTION: FF7 0x005aa341
void __cdecl BattleFx_MagicDemi3SegmentEmitterTask(void)
{
    int cosVal;
    int sinValue;
    BattleFxDemi3SegmentScratch *dirVec;
    BattleFxDemi3EmitterSlot *task;
    PsxMatrix *matrix;
    int longRadius;
    int burst;
    int slotIdx;
    BattleFxDemi3SegmentEmitter *pBasis;
    int rotation;
    int tempInnerRadius;

    task = (BattleFxDemi3EmitterSlot *)BATTLE_CURRENT_TASK_SLOT;
    dirVec = (BattleFxDemi3SegmentScratch *)Gfx_GetPsxScratchpadPtr(0);
    pBasis = &dirVec->basis;
    if (g_BattleIsPaused == 0) {
        if (task->emitCount < 40) {
            for (burst = 0; burst < 5; burst++) {
                for (slotIdx = 0; slotIdx < 20; slotIdx++) {
                    if (g_BattleMagicDemi3SegmentRecords[slotIdx].state == -1) {
                        g_BattleMagicDemi3SegmentRecords[slotIdx].time = 0;
                        g_BattleMagicDemi3SegmentRecords[slotIdx].state = 0;
                        tempInnerRadius = rand() % 1500 + 500;
                        longRadius = tempInnerRadius + rand() % 1000 + 1000;
                        rotation = rand() & 0xFFF;
                        sinValue = SinFixed(rotation);
                        cosVal = CosFixed(rotation);
                        g_BattleMagicDemi3SegmentRecords[slotIdx].startX = longRadius * sinValue >> 12;
                        g_BattleMagicDemi3SegmentRecords[slotIdx].startY = longRadius * cosVal >> 12;
                        g_BattleMagicDemi3SegmentRecords[slotIdx].endX = tempInnerRadius * sinValue >> 12;
                        g_BattleMagicDemi3SegmentRecords[slotIdx].endY = tempInnerRadius * cosVal >> 12;
                        dirVec->scaleX = g_BattleMagicDemi3SegmentRecords[slotIdx].endY - g_BattleMagicDemi3SegmentRecords[slotIdx].startY;
                        dirVec->scaleY = g_BattleMagicDemi3SegmentRecords[slotIdx].startX - g_BattleMagicDemi3SegmentRecords[slotIdx].endX;
                        dirVec->scaleZ = 0;
                        VectorNormalToSVector(&dirVec->scaleX, (short *)pBasis);
                        g_BattleMagicDemi3SegmentRecords[slotIdx].offsetX = pBasis->componentA + pBasis->componentB >> 12;
                        g_BattleMagicDemi3SegmentRecords[slotIdx].offsetY = pBasis->componentB - pBasis->componentA >> 12;
                        g_BattleMagicDemi3SegmentRecords[slotIdx].offset2X = -pBasis->componentA + pBasis->componentB >> 12;
                        g_BattleMagicDemi3SegmentRecords[slotIdx].offset2Y = -pBasis->componentB - pBasis->componentA >> 12;
                        break;
                    }
                }
            }
        }
        (task->emitCount)++;
    }
    matrix = (struct PsxMatrix *)BattleFx_InstallBillboardMatrix(&task->position.x, 0x1000 /* 1.0 */, 0);
    RefreshGlobalTransformState();
    if (BattleFx_MagicDemi3DrawSegmentRecords() == 0) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Iterates and draws active segment beam records for Grav3.
 */
// FUNCTION: FF7 0x005aa5e0
int __cdecl BattleFx_MagicDemi3DrawSegmentRecords(void)
{
    int i;
    int activeCnt;

    activeCnt = 0;
    for (i = 0; i < 20; i++) {
        if (g_BattleMagicDemi3SegmentRecords[i].state != -1) {
            BattleFx_MagicDemi3DrawAndStepSegmentRecord(&g_BattleMagicDemi3SegmentRecords[i]);
            activeCnt++;
        }
    }
    return activeCnt;
}

/*
 * Draws and steps an individual segment beam record for Grav3.
 */
// FUNCTION: FF7 0x005aa63d
void __cdecl BattleFx_MagicDemi3DrawAndStepSegmentRecord(BattleFxMagicGravSegment *segment)
{
    struct {
        int depth;
        int unused0[2];
        int blend1z;
        int blend1w;
        struct BattleFxDemi3SegmentSlot *data;
        int unused1[2];
        int blend2z;
        int blend2w;
        int unused2[2];
        int blend0z;
        int blend0w;
        int unused3[2];
        int blend3z;
        int blend3w;
        struct BattleFxDemi3SegmentPrim *primitive;
        int flags;
        int time;
        int projected;
        BattleFxProjectedPoint *blend;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    frame.data = (struct BattleFxDemi3SegmentSlot *)Gfx_GetPsxScratchpadPtr(0);
    frame.data->end.vx = segment->endX;
    frame.data->delta.vx = frame.data->end.vx - (frame.data->start.vx = segment->startX);
    frame.data->end.vy = segment->endY;
    frame.data->delta.vy = frame.data->end.vy - (frame.data->start.vy = segment->startY);
    frame.data->start.vz = 0;
    frame.data->end.vz = 0;
    frame.data->delta.vz = 0;
    frame.time = segment->time;
    if (frame.time < 2) {
        frame.data->end.vx = frame.data->start.vx + frame.data->delta.vx * frame.time / 2;
        frame.data->end.vy = frame.data->start.vy + frame.data->delta.vy * frame.time / 2;
    } else {
        frame.time -= 2;
        if (frame.time < 2) {
            frame.data->start.vx += frame.data->delta.vx * frame.time / 2;
            frame.data->start.vy += frame.data->delta.vy * frame.time / 2;
        } else {
            segment->state = -1;
            return;
        }
    }
    frame.primitive = (struct BattleFxDemi3SegmentPrim *)g_BattleMagicDemi3_DrawListHandle;
    frame.depth = Gfx_ProjectTriangleToScreenAlt(&frame.data->start, &frame.data->end, &frame.data->end, &frame.primitive->x1, &frame.primitive->x2, &frame.primitive->x2, &frame.projected, &frame.flags);
    if ((frame.flags & 0x60000) == 0) {
        frame.primitive->tag = 0x08000000;
        frame.primitive->rgb2 = 0;
        frame.primitive->rgb3 = 0x38111111;
        frame.primitive->rgb1 = 0x38111111;
        frame.primitive->rgbCode0 = 0x38111111;
        frame.primitive->x0 = frame.primitive->x2 + segment->offsetX;
        frame.primitive->y0 = frame.primitive->y2 + segment->offsetY;
        frame.primitive->x3 = frame.primitive->x2 + segment->offset2X;
        frame.primitive->y3 = frame.primitive->y2 + segment->offset2Y;
        frame.blend = (BattleFxProjectedPoint *)Gfx_GetProjectedVertexRecord(0);
        frame.blend0z = frame.blend->screenX;
        frame.blend0w = frame.blend->screenY;
        frame.blend = (BattleFxProjectedPoint *)Gfx_GetProjectedVertexRecord(1);
        frame.blend3z = frame.blend2z = frame.blend1z = frame.blend->screenX;
        frame.blend3w = frame.blend2w = frame.blend1w = frame.blend->screenY;
        Gfx_SetBlendFactorQuad(*(float *)&frame.blend3z, *(float *)&frame.blend3w, *(float *)&frame.blend0z, *(float *)&frame.blend0w, *(float *)&frame.blend2z, *(float *)&frame.blend2w, *(float *)&frame.blend1z, *(float *)&frame.blend1w);
        Gfx_DrawPsxPrimitive(frame.primitive);
    }
    if (g_BattleIsPaused == 0)
        ++segment->time;
}
} /* extern "C" */
