// src/ff7/src/battle/yasui/yasui_limit_hammerblow.cpp
// Limit Break [10] (Barret Lv 2-2): Hammerblow (Japanese: ハンマーブロウ, dev tag: "LIMIT/HAMBLOW" / Hamblow)
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Hammer Blow limit break effect: growing black and eased impact models, radial sprite
//   bursts and a target actor that tumbles away.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/enums/sound.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_boss.h"
#include "ff7/battle_tasks.h"
#include "ff7/camera_script.h"
#include "lib/types/graphics.h"
#include "lib/codegen.h"
#include <string.h>
#include <stdlib.h>
#include "ff7/api/battle.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
extern "C" {

struct BattleFxModelDraw;

#pragma intrinsic(memcpy)
#pragma intrinsic(memset)
#pragma intrinsic(abs)

// GLOBAL: FF7 0x007ed5c0
unsigned int g_YasuiLimitHammerblowResourceRows[15] ={
    0x00c840b1,
    (unsigned int)&g_YasuiLimitHammerblowBlowRsdFilename,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00c840b1,
    // STRING: FF7 0x007ed64c
    (unsigned int)"HABSONIC.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x00196a39,
    // STRING: FF7 0x007ed65c
    (unsigned int)"H_BLOW01.TIM",
    // STRING: FF7 0x007ed66c
    (unsigned int)"ANG_M2.SPT",
    0x00000000,
    0x00000000
};
// GLOBAL: FF7 0x007ed600
struct YasuiLimitHammerblowModelDrawReq0 g_YasuiLimitHammerblowModelDrawReq0 = {0, {0xa8, 0, 0, 0, 0, 0}, 0, {0x20, 0, 0, 0}};
// GLOBAL: FF7 0x007ed610
struct YasuiLimitHammerblowModelDrawReq1 g_YasuiLimitHammerblowModelDrawReq1 = {0, {0x88, 0, 0, 0, 0, 0}, 0, {0x20, 0, 0, 0}};
// GLOBAL: FF7 0x007ed620
struct YasuiLimitHammerblowSpriteDrawReq g_YasuiLimitHammerblowSpriteDrawReq = {0, 0, {0x80, 0x80, 0x80, 0x2e}, 0, {0}};
// GLOBAL: FF7 0x007ed630
unsigned char g_YasuiLimitHammerblowSceneModelDrawReq[16] = {0x00, 0x00, 0x00, 0x00, 0xa8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x007ed640
unsigned char g_YasuiLimitHammerblowBlowRsdFilename[9] = "BLOW.RSD";
// GLOBAL: FF7 0x00a41e28
BattleFxLimitHammerblowSpriteState *g_YasuiLimitHammerblowSpriteScratch = 0;
unsigned char g_bssPad_00a41e2c[4] = {0};
// GLOBAL: FF7 0x00a41e30
PsxMatrix g_YasuiLimitHammerblowComposedMatrix = {0};
// GLOBAL: FF7 0x00a41e50
int g_YasuiLimitHammerblowTargetActor = 0;
// GLOBAL: FF7 0x00a41e54
BattleHammerblowModelScratch *g_YasuiLimitHammerblowModel1Scratch = 0;
typedef struct {
    unsigned char initFlag;
    unsigned char pad_01[0x7];
    PsxMatrix matrix;
} YasuiLimitHamblowImpactState;
// GLOBAL: FF7 0x00a41e58
YasuiLimitHamblowImpactState g_YasuiLimitHammerblowImpactState = {0};
// GLOBAL: FF7 0x00a41e80
BattleHammerblowModelScratch *g_YasuiLimitHammerblowModel0Scratch = 0;
// GLOBAL: FF7 0x00a41e84
unsigned char g_YasuiLimitHammerblowModel0InitFlag = 0;
// GLOBAL: FF7 0x00a41e88
unsigned char g_YasuiLimitHammerblowTumbleInitFlag = 0;
// GLOBAL: FF7 0x00a41e8c
int g_YasuiLimitHammerblowDrawListHead = 0;
// GLOBAL: FF7 0x00a41e90
unsigned char g_YasuiLimitHammerblowTimelineInitFlag = 0;
// GLOBAL: FF7 0x00a41e94
unsigned char g_YasuiLimitHammerblowSpriteInitFlag = 0;
// GLOBAL: FF7 0x00a41e98
int g_YasuiLimitHammerblowSourceActor = 0;
unsigned char g_bssPad_00a41e9c[4] = {0};
// GLOBAL: FF7 0x00a41ea0
__int64 g_YasuiLimitHammerblowSavedActorPos = {0};
// GLOBAL: FF7 0x00a41ea8
char g_YasuiLimitHammerblowSavedActorStatus = 0;
// GLOBAL: FF7 0x00a41eac
short *g_YasuiLimitHammerblowBonePosScratch = 0;
// GLOBAL: FF7 0x00a41eb0
BattleHammerblowTumbleScratch *g_YasuiLimitHammerblowTumbleScratch = 0;
// GLOBAL: FF7 0x00a41eb4
struct GfxTableCursor *g_YasuiLimitHammerblowResourceContext = 0;
// GLOBAL: FF7 0x00a41eb8
short g_YasuiLimitHammerblowTumbleScaleX = 0;
unsigned char g_bssPad_00a41ebc[4] = {0};
unsigned char g_bssPad_00a41ec0[8] = {0};
// GLOBAL: FF7 0x00a41ec8
struct YasuiLimitHammerblowTumbleMatrix g_YasuiLimitHammerblowTumbleMatrix = {0};

// clang-format on


/*
 * Initializes resource record, builds "LIMIT/HAMBLOW" path, and loads Hammer Blow resources.
 */
// FUNCTION: FF7 0x00467490
void __cdecl BattleFx_LimitLoadResHammerblow(void)
{
    char pathScratch[200];
    GfxModelTableBank *context;
    GfxResourceRequest requestRecord;
    int kind;
    GfxResourceRequest *pReq;
    int resourceRows;

    if (g_pBattle3dScene != 0 && g_YasuiLimitHammerblowResourceContext == 0) {
        kind = 3;
        resourceRows = (int)&g_YasuiLimitHammerblowResourceRows;
        context = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pReq = &requestRecord;
        Gfx_InitResourceRequest(pReq);
        pReq->groupTag = 0;
        // STRING: FF7 0x007ed678
        pReq->resourcePath = BattleFx_BuildResourcePath("LIMIT/HAMBLOW", pathScratch);
        Gfx_LoadModelTableIfAbsent(kind, resourceRows, pReq, &g_YasuiLimitHammerblowResourceContext, context);
    }
}

/*
 * Entry point for Hammer Blow limit break; loads resources and registers target callback.
 */
// FUNCTION: FF7 0x00467520
int __cdecl BattleFx_LimitEnterHammerblow(int targetMask, int actorIdx)
{
    BattleFx_LimitLoadResHammerblow();
    BattleFx_LimitRegisterHammerblowPerTargetCallback(targetMask, actorIdx);
    return 0;
}

/*
 * Registers a per-target callback task to spawn Hammer Blow effect tasks.
 */
// FUNCTION: FF7 0x0046753c
void __cdecl BattleFx_LimitRegisterHammerblowPerTargetCallback(int targetActor, int sourceActor)
{
    Battle_CreateBitPatternCallbackTask(targetActor, sourceActor, 0, (void *)BattleFx_LimitSpawnHammerblowPerTarget);
}

/*
 * Stores target and source actors and allocates the main Hammer Blow timeline task.
 */
// FUNCTION: FF7 0x00467558
void __cdecl BattleFx_LimitSpawnHammerblowPerTarget(int actor, int sourceActor)
{
    int slot;

    g_YasuiLimitHammerblowTargetActor = actor;
    g_YasuiLimitHammerblowSourceActor = sourceActor;
    slot = (int)BATTLE_NEW_TASK_SLOT(BattleFx_LimitHammerblowTimelineTask);
}

/*
 * Main timeline task driving Hammer Blow impact effects, sound, and target tumbling.
 */
// FUNCTION: FF7 0x0046758d
void __cdecl BattleFx_LimitHammerblowTimelineTask(void)
{
    BattleTaskSlot *rec;
    short bone[4];
    BattleFxHammerblowTimeline *child;

    rec = BATTLE_CURRENT_TASK_SLOT;
    if ((g_YasuiLimitHammerblowModel0InitFlag & 1) == 0) {
        g_YasuiLimitHammerblowModel0InitFlag |= 1;
        g_YasuiLimitHammerblowBonePosScratch = (short *)Gfx_GetPsxScratchpadPtr(0);
    }
    MultiplyMatrix(&g_BattleCameraTransform, &g_YasuiLimitHammerblowImpactState.matrix, &g_YasuiLimitHammerblowComposedMatrix);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (rec->frameCounter == 0xF) {
        RotMatrix((SVECTOR *)&g_BattleActorRecords[g_YasuiLimitHammerblowSourceActor].rotX, &g_YasuiLimitHammerblowImpactState.matrix);
        Battle_GetBoneWorldPos(g_YasuiLimitHammerblowSourceActor, g_BattleActorRecords[g_YasuiLimitHammerblowSourceActor].targetBone, g_YasuiLimitHammerblowBonePosScratch);
        g_YasuiLimitHammerblowImpactState.matrix.t[0] = g_YasuiLimitHammerblowBonePosScratch[0];
        g_YasuiLimitHammerblowImpactState.matrix.t[1] = 0;
        g_YasuiLimitHammerblowImpactState.matrix.t[2] = g_YasuiLimitHammerblowBonePosScratch[2];
        child = (BattleFxHammerblowTimeline *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitHammerblowGrowingBlackModelTask);
        child->posX = 0;
        child->posY = -0x1F4;
        child->posZ = -0x1F4;
        child->velocityZ = 0;
        child->velocityY = 0;
        child->velocityX = 0;
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputeScreenPanFromPosition(g_YasuiLimitHammerblowBonePosScratch), 0x163);
    }
    if (rec->frameCounter == 0xF) {
        child = (BattleFxHammerblowTimeline *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitHammerblowRadialSpriteEmitterTask);
    }
    if (rec->frameCounter == 0xF) {
        child = (BattleFxHammerblowTimeline *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitHammerblowSpawnEasedGrowingModelsTask);
        child->ownerSlot = 0;
    }
    if (rec->frameCounter == 0xF) {
        if (g_BattleActiveTargetMask & (1 << g_YasuiLimitHammerblowTargetActor)) {
            Battle_GetBoneWorldPos(g_YasuiLimitHammerblowTargetActor, g_BattleActorRecords[g_YasuiLimitHammerblowTargetActor].targetBone, bone);
            (*(Int64Pair *)&g_YasuiLimitHammerblowSavedActorPos) = *(Int64Pair *)&g_BattleActorRecords[g_YasuiLimitHammerblowTargetActor].posX;
            child = (BattleFxHammerblowTimeline *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitHammerblowTumblingActorTask);
            *(Int64Pair *)&child->posX = *(Int64Pair *)&g_BattleActorRecords[g_YasuiLimitHammerblowTargetActor].posX;
            child->velocityX = SinFixed(g_BattleActorRecords[g_YasuiLimitHammerblowSourceActor].yaw) * -0x1F4 >> 12;
            child->velocityY = -0x1F4;
            child->velocityZ = CosFixed(g_BattleActorRecords[g_YasuiLimitHammerblowSourceActor].yaw) * -0x1F4 >> 12;
            child->offsetX = SinFixed(g_BattleActorRecords[g_YasuiLimitHammerblowTargetActor].yaw) * (bone[0] - g_BattleActorRecords[g_YasuiLimitHammerblowTargetActor].posX) >> 12;
            child->offsetY = bone[1] - g_BattleActorRecords[g_YasuiLimitHammerblowTargetActor].posY;
            child->offsetZ = CosFixed(g_BattleActorRecords[g_YasuiLimitHammerblowTargetActor].yaw) * (bone[2] - g_BattleActorRecords[g_YasuiLimitHammerblowTargetActor].posZ) >> 12;
            child->scale = g_BattleActorRecords[g_YasuiLimitHammerblowTargetActor].scale;
            g_YasuiLimitHammerblowSavedActorStatus = *(unsigned char *)&g_BattleActorRecords[g_YasuiLimitHammerblowTargetActor].drawFlags;
            *(unsigned char *)&g_BattleActorRecords[g_YasuiLimitHammerblowTargetActor].drawFlags |= 0x10;
        } else {
            Battle_PostActorHitOrHurtMessage(g_YasuiLimitHammerblowTargetActor);
        }
    }
    if (rec->frameCounter == 0x3C) {
        rec->ownerSlot = BATTLE_TASK_FREE;
    }
    (rec->frameCounter)++;
}

/*
 * Task managing the primary growing black impact model effect and alpha fade.
 */
// FUNCTION: FF7 0x00467987
void __cdecl BattleFx_LimitHammerblowGrowingBlackModelTask(void)
{
    int scale;
    int frame;
    BattleFxHammerblowGrowingRecord *task;
    int ratio;

    task = (BattleFxHammerblowGrowingRecord *)BATTLE_CURRENT_TASK_SLOT;
    if ((g_YasuiLimitHammerblowTimelineInitFlag & 1) == 0) {
        g_YasuiLimitHammerblowTimelineInitFlag |= 1;
        g_YasuiLimitHammerblowModel0Scratch = (BattleHammerblowModelScratch *)Gfx_GetPsxScratchpadPtr(0);
    }
    (g_YasuiLimitHammerblowModelDrawReq0.hamblowModelResource) = Gfx_ModelTableGetRenderObject(0, g_YasuiLimitHammerblowResourceContext);
    frame = task->frame;
    if (frame < 0) { /* empty first arm: lib/codegen.h */
    } else {
        frame = frame;
        if (frame < 0) { /* empty first arm: lib/codegen.h */
        } else {
            frame = frame;
            if (frame < 0xF) {
                ratio = (frame << 0xC) / 0xF;
                g_YasuiLimitHammerblowModelDrawReq0.alpha = ratio;
            } else {
                task->ownerSlot = BATTLE_TASK_FREE;
                return;
            }
        }
    }
    frame = task->frame;
    if (frame < 0xF) {
        ratio = (frame << 0xC) / 0xF;
        scale = ratio << 9 >> 0xC;
    } else {
        frame -= 0xF;
        if (frame < 0) { /* empty first arm: lib/codegen.h */
        } else {
            frame = frame;
        }
    }
    RotMatrix((SVECTOR *)&task->rotX, &g_YasuiLimitHammerblowModel0Scratch->matrix);
    g_YasuiLimitHammerblowModel0Scratch->matrix.t[0] = task->posX;
    g_YasuiLimitHammerblowModel0Scratch->matrix.t[1] = task->posY;
    g_YasuiLimitHammerblowModel0Scratch->matrix.t[2] = task->posZ;
    g_YasuiLimitHammerblowModel0Scratch->scaleZ = scale;
    g_YasuiLimitHammerblowModel0Scratch->scaleY = scale;
    g_YasuiLimitHammerblowModel0Scratch->scaleX = scale;
    ScaleMatrix(&g_YasuiLimitHammerblowModel0Scratch->matrix, g_YasuiLimitHammerblowModel0Scratch);
    MultiplyMatrix(&g_YasuiLimitHammerblowComposedMatrix, &g_YasuiLimitHammerblowModel0Scratch->matrix, &g_YasuiLimitHammerblowModel0Scratch->matrix);
    SetRotMatrix(&g_YasuiLimitHammerblowModel0Scratch->matrix);
    SetTransMatrix(&g_YasuiLimitHammerblowModel0Scratch->matrix);
    SetDrawColorRGB(0, 0, 0);
    g_YasuiLimitHammerblowDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_YasuiLimitHammerblowModelDrawReq0, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_YasuiLimitHammerblowDrawListHead);
    if (g_BattleIsPaused == 0) {
        (task->frame)++;
        task->angle += 0x20;
    }
}

/*
 * Task that periodically emits secondary eased growing model tasks at offsets.
 */
// FUNCTION: FF7 0x00467ba8
void __cdecl BattleFx_LimitHammerblowSpawnEasedGrowingModelsTask(void)
{
    BattleFxHammerblowRecord *pEmitter;
    BattleFxHammerblowGrowingModel *child;

    pEmitter = (BattleFxHammerblowRecord *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (pEmitter->frameCount % 3 == 0) {
        child = (BattleFxHammerblowGrowingModel *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitHammerblowEasedGrowingModelTask);
        child->offsetX = 0;
        child->offsetY = 0xFE0C;
        child->offsetZ = pEmitter->index * 0xFFFFFED4 - 0x1F4;
        child->angleZ = 0;
        child->angleY = 0;
        child->angleX = 0;
        child->scale = 0x200;
        (pEmitter->index)++;
        if (pEmitter->index >= 3) {
            pEmitter->index = 0xFFFF;
        }
    }
    (pEmitter->frameCount)++;
}

/*
 * Task managing individual eased growing model scaling, fade, and rendering.
 */
// FUNCTION: FF7 0x00467c8b
void __cdecl BattleFx_LimitHammerblowEasedGrowingModelTask(void)
{
    int scale;
    int frame;
    int pass;
    BattleFxEasedTransformSlot *task;
    int ratio;

    task = (BattleFxEasedTransformSlot *)BATTLE_CURRENT_TASK_SLOT;
    if ((g_YasuiLimitHammerblowImpactState.initFlag & 1) == 0) {
        g_YasuiLimitHammerblowImpactState.initFlag |= 1;
        g_YasuiLimitHammerblowModel1Scratch = (BattleHammerblowModelScratch *)Gfx_GetPsxScratchpadPtr(0);
    }
    (g_YasuiLimitHammerblowModelDrawReq1.hamblowEasedModelResource) = Gfx_ModelTableGetRenderObject(1, g_YasuiLimitHammerblowResourceContext);
    frame = task->frame;
    if (frame < 0) { /* empty first arm: lib/codegen.h */
    } else {
        frame = frame;
        if (frame < 0) { /* empty first arm: lib/codegen.h */
        } else {
            frame = frame;
            if (frame < 0xA) {
                ratio = (frame << 0xC) / 0xA;
                g_YasuiLimitHammerblowModelDrawReq1.alpha = ratio;
            } else {
                task->ownerSlot = BATTLE_TASK_FREE;
                return;
            }
        }
    }
    frame = task->frame;
    if (frame < 0xA) {
        ratio = (frame << 0xC) / 0xA;
        for (pass = 0; pass < 2; pass++) {
            ratio = SinFixed(ratio / 4);
        }
        scale = task->scale * ratio >> 0xC;
    } else {
        frame -= 0xA;
        if (frame < 0) { /* empty first arm: lib/codegen.h */
        } else {
            frame = frame;
        }
    }
    RotMatrix((SVECTOR *)&task->rotX, &g_YasuiLimitHammerblowModel1Scratch->matrix);
    g_YasuiLimitHammerblowModel1Scratch->matrix.t[0] = task->posX;
    g_YasuiLimitHammerblowModel1Scratch->matrix.t[1] = task->posY;
    g_YasuiLimitHammerblowModel1Scratch->matrix.t[2] = task->posZ;
    g_YasuiLimitHammerblowModel1Scratch->scaleZ = scale;
    g_YasuiLimitHammerblowModel1Scratch->scaleY = scale;
    g_YasuiLimitHammerblowModel1Scratch->scaleX = scale;
    ScaleMatrix(&g_YasuiLimitHammerblowModel1Scratch->matrix, g_YasuiLimitHammerblowModel1Scratch);
    MultiplyMatrix(&g_YasuiLimitHammerblowComposedMatrix, &g_YasuiLimitHammerblowModel1Scratch->matrix, &g_YasuiLimitHammerblowModel1Scratch->matrix);
    SetRotMatrix(&g_YasuiLimitHammerblowModel1Scratch->matrix);
    SetTransMatrix(&g_YasuiLimitHammerblowModel1Scratch->matrix);
    SetDrawColorRGB(0, 0, 0);
    g_YasuiLimitHammerblowDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_YasuiLimitHammerblowModelDrawReq1, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_YasuiLimitHammerblowDrawListHead);
    if (g_BattleIsPaused == 0) {
        (task->frame)++;
    }
}

/*
 * Emits radial animated sprite bursts at randomized angles and rotations.
 */
// FUNCTION: FF7 0x00467ed4
void __cdecl BattleFx_LimitHammerblowRadialSpriteEmitterTask(void)
{
    int angle;
    BattleTaskSlot *task;
    BattleFxHammerblowRadialSprite *pSprite;
    int emitAmount;
    int rot;

    task = BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    emitAmount = (task->frameCounter << 9 & 0xFF) + 0x200;
    while (emitAmount >= 0x100) {
        pSprite = (BattleFxHammerblowRadialSprite *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitHammerblowSpriteAnimationTask);
        angle = rand() & 0xFFF;
        pSprite->offsetX = SinFixed(angle) * 0x64 >> 0xC;
        pSprite->offsetY = (CosFixed(angle) * 0x64 >> 0xC) - 0x1F4;
        pSprite->offsetZ = 0xFE0C;
        if (DEAD_IN_RETAIL != 0) {
            rot = 0xFFFFFE40;
        } else {
            rot = rand() % 0x80 - 0x240;
        }
        pSprite->rotation = rot;
        pSprite->clearedRotY = 0;
        pSprite->angle = angle;
        pSprite->scale = 0x2000;
        emitAmount -= 0x100;
    }
    (task->frameCounter)++;
    if (task->frameCounter >= 5) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Animated sprite task with rotation and scaling composed with the impact matrix.
 */
// FUNCTION: FF7 0x00468015
void __cdecl BattleFx_LimitHammerblowSpriteAnimationTask(void)
{
    BattleFxLimitHammerblowSprite *task;

    task = (BattleFxLimitHammerblowSprite *)BATTLE_CURRENT_TASK_SLOT;
    if ((g_YasuiLimitHammerblowSpriteInitFlag & 1) == 0) {
        g_YasuiLimitHammerblowSpriteInitFlag |= 1;
        g_YasuiLimitHammerblowSpriteScratch = (BattleFxLimitHammerblowSpriteState *)Gfx_GetPsxScratchpadPtr(0);
    }
    (g_YasuiLimitHammerblowSpriteDrawReq.animBlock) = Gfx_ModelTableGetLoadedRecord(2, g_YasuiLimitHammerblowResourceContext);
    g_YasuiLimitHammerblowSpriteDrawReq.object = Gfx_ModelTableGetSpriteObject(2, g_YasuiLimitHammerblowResourceContext);
    g_YasuiLimitHammerblowSpriteScratch->rotX = task->valueOld;
    g_YasuiLimitHammerblowSpriteScratch->rotY = 0;
    g_YasuiLimitHammerblowSpriteScratch->rotZ = 0;
    RotMatrix((SVECTOR *)g_YasuiLimitHammerblowSpriteScratch, &g_YasuiLimitHammerblowSpriteScratch->matrix);
    RotateMatrixZ(task->angleZ, &g_YasuiLimitHammerblowSpriteScratch->matrix);
    g_YasuiLimitHammerblowSpriteScratch->matrix.t[0] = task->posX;
    g_YasuiLimitHammerblowSpriteScratch->matrix.t[1] = task->posY;
    g_YasuiLimitHammerblowSpriteScratch->matrix.t[2] = task->posZ;
    g_YasuiLimitHammerblowSpriteScratch->valueNew = task->valueNew;
    g_YasuiLimitHammerblowSpriteScratch->valuePrev = g_YasuiLimitHammerblowSpriteScratch->valueNew;
    g_YasuiLimitHammerblowSpriteScratch->valueOld = g_YasuiLimitHammerblowSpriteScratch->valuePrev;
    ScaleMatrix(&g_YasuiLimitHammerblowSpriteScratch->matrix, &g_YasuiLimitHammerblowSpriteScratch->valueOld);
    MultiplyMatrix(&g_YasuiLimitHammerblowComposedMatrix, &g_YasuiLimitHammerblowSpriteScratch->matrix, &g_YasuiLimitHammerblowSpriteScratch->matrix);
    SetRotMatrix(&g_YasuiLimitHammerblowSpriteScratch->matrix);
    SetTransMatrix(&g_YasuiLimitHammerblowSpriteScratch->matrix);
    g_YasuiLimitHammerblowSpriteDrawReq.frame = task->frame;
    g_YasuiLimitHammerblowDrawListHead = BattleFx_SubmitSpriteDraw(&g_YasuiLimitHammerblowSpriteDrawReq, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_YasuiLimitHammerblowDrawListHead);
    if (g_BattleIsPaused == 0 && (g_YasuiLimitHammerblowSpriteDrawReq.animBlock) != 0) {
        (task->frame)++;
        if (task->frame >= *(int *)((char *)*(int *)(g_YasuiLimitHammerblowSpriteDrawReq.animBlock) + 4)) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Task animating the target actor tumbling away with velocity, rotation, and scaling.
 */
// FUNCTION: FF7 0x00468216
void __cdecl BattleFx_LimitHammerblowTumblingActorTask(void)
{
    BattleFxHammerblowTumble *p;
    int q; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */

    p = (BattleFxHammerblowTumble *)BATTLE_CURRENT_TASK_SLOT;
    if ((g_YasuiLimitHammerblowTumbleInitFlag & 1) == 0) {
        g_YasuiLimitHammerblowTumbleInitFlag |= 1;
        g_YasuiLimitHammerblowTumbleScratch = (BattleHammerblowTumbleScratch *)Gfx_GetPsxScratchpadPtr(0);
    }
    if (((int)g_pBattle3dScene) != 0) {
        (*(int *)&g_YasuiLimitHammerblowSceneModelDrawReq) = g_pBattle3dScene->modelRequest;
    } else {
        (*(int *)&g_YasuiLimitHammerblowSceneModelDrawReq) = 0;
    }
    if (g_BattleIsPaused == 0) {
        (p->timer)++;
        if (p->timer >= 0x14) {
            g_BattleActorRecords[g_YasuiLimitHammerblowTargetActor].scale = p->startY;
            *(Int64Pair *)&g_BattleActorRecords[g_YasuiLimitHammerblowTargetActor].posX = (*(Int64Pair *)&g_YasuiLimitHammerblowSavedActorPos);
            g_BattleActorRecords[g_YasuiLimitHammerblowTargetActor].rotZ = 0;
            g_BattleActorRecords[g_YasuiLimitHammerblowTargetActor].drawFlags = g_YasuiLimitHammerblowSavedActorStatus;
            Battle_RestoreActorAfterEffect(*(char *)&g_YasuiLimitHammerblowTargetActor);
            p->ownerSlot = BATTLE_TASK_FREE;
            return;
        }
        p->posX += p->velocityX;
        p->posY += p->velocityY;
        p->posZ += p->velocityZ;
        p->velocityY += 0x1E;
        g_BattleActorRecords[g_YasuiLimitHammerblowTargetActor].rotZ += 0x200;
    }
    g_YasuiLimitHammerblowTumbleScratch->prevOffsetX = p->offsetX;
    g_YasuiLimitHammerblowTumbleScratch->prevOffsetY = p->offsetY;
    g_YasuiLimitHammerblowTumbleScratch->prevOffsetZ = p->offsetZ;
    RotMatrix((SVECTOR *)&g_BattleActorRecords[g_YasuiLimitHammerblowTargetActor].rotX, &g_YasuiLimitHammerblowTumbleScratch->matrix);
    TransformVectorToInt16(&g_YasuiLimitHammerblowTumbleScratch->matrix, (SVECTOR *)g_YasuiLimitHammerblowTumbleScratch, (short *)g_YasuiLimitHammerblowTumbleScratch);
    g_BattleActorRecords[g_YasuiLimitHammerblowTargetActor].posX = p->posX - g_YasuiLimitHammerblowTumbleScratch->prevOffsetX + p->offsetX;
    g_BattleActorRecords[g_YasuiLimitHammerblowTargetActor].posY = p->posY - g_YasuiLimitHammerblowTumbleScratch->prevOffsetY + p->offsetY;
    g_BattleActorRecords[g_YasuiLimitHammerblowTargetActor].posZ = p->posZ - g_YasuiLimitHammerblowTumbleScratch->prevOffsetZ + p->offsetZ;
    g_BattleActorRecords[g_YasuiLimitHammerblowTargetActor].scale = p->startY + (0 - p->startY) * p->timer / 0x14;
    g_YasuiLimitHammerblowTumbleScaleX = g_BattleActorRecords[g_YasuiLimitHammerblowTargetActor].scaleX * g_BattleActorRecords[g_YasuiLimitHammerblowTargetActor].scale >> 0xC;
    g_YasuiLimitHammerblowTumbleMatrix.scaleY = g_BattleActorRecords[g_YasuiLimitHammerblowTargetActor].scaleY * g_BattleActorRecords[g_YasuiLimitHammerblowTargetActor].scale >> 0xC;
    g_YasuiLimitHammerblowTumbleMatrix.posX = p->posX;
    g_YasuiLimitHammerblowTumbleMatrix.posY = 0;
    g_YasuiLimitHammerblowTumbleMatrix.posZ = p->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &g_YasuiLimitHammerblowTumbleScaleX, &g_YasuiLimitHammerblowTumbleScratch->matrix);
    SetRotMatrix(&g_YasuiLimitHammerblowTumbleScratch->matrix);
    SetTransMatrix(&g_YasuiLimitHammerblowTumbleScratch->matrix);
    g_YasuiLimitHammerblowDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_YasuiLimitHammerblowSceneModelDrawReq, g_BattleCurrentOrderingTable + BATTLE_OT_AFTER_SORTED_2, 0, g_YasuiLimitHammerblowDrawListHead);
}
} /* extern "C" */
