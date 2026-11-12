// src/ff7/src/battle/battle_fx_builtin_puff_mog_dance.cpp
// Builtin Puff & Limit Break [57] (Cait Sith Slots): Mog Dance (Japanese: モーグリダンス, dev tag: "LIMIT/MOGDAT.DAT" / Mog)
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Built-in puff effect with its per-target emitter and particle tasks, and the Mog limit
//   effect, which adds hit sparks, an animated sprite and a timeline that replaces its callback on
//   the actor mask.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/enums/sound.h"
#include "ff7/api/battle.h"
#include "ff7/battle_tasks.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "ff7/battle.h"
#include "ff7/gte.h"
#include "lib/crt.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_fx.h"
#include "lib/codegen.h"
extern "C" {

// clang-format off
// GLOBAL: FF7 0x008ff138
unsigned char g_BattleFxPuffSpriteQuad[8] = {0xf0, 0xff, 0xf0, 0xff, 0x00, 0x00, 0x1f, 0x1f};
// clang-format off
// GLOBAL: FF7 0x008ff140
struct BattleFxPuffColorDesc g_BattleFxPuffColorDesc = {-128, -128, -128, {0x2e, 0x3a, 0, 0x47, 0x79}, 0, {0}};
// clang-format on

// clang-format off
// Texture cell X offset per animation frame of the Limit Mog sprite, indexed by the task's timer.
// GLOBAL: FF7 0x008ff150
char g_BattleFxPuffTextureOffsets[16] = {0x00, 0x20, 0x40, 0x60, -128, -96, -64, -32, 0, 0, 0, 0, 0, 0, 0, 0};

// GLOBAL: FF7 0x008ff160
short g_BattleFxLimitMogDanceHitLivewireTexCoords[5] = {30727, 30791, 31047, 30855, 30919};
#define g_BattlePuffSpriteAngle (((BattleFxPuffSpriteQuad *)&g_BattleFxPuffSpriteQuad)->puffSpriteAngle)
// clang-format on


// clang-format off
// GLOBAL: FF7 0x00c3f2c8
int g_BattleFxPuffDrawPacketPtr = 0;
// GLOBAL: FF7 0x00c3f2cc
int g_BattleFxLimitMogDanceTargetActorMask = 0;
// GLOBAL: FF7 0x00c3f2d0
int g_BattleFxLimitMogDanceActingActorIdx = 0;
unsigned char g_bssPad_00c3f2d4[4] = {0};
// clang-format on

/*
 * Enters the built-in puff effect through BattleFx_BuiltinBeginPuff.
 */
// FUNCTION: FF7 0x005d6e70
void __cdecl BattleFx_EnterBuiltinPuff(unsigned int targetMask, int actingActor)
{
    BattleFx_BuiltinBeginPuff(targetMask, actingActor);
}

/*
 * Starts the puff effect: per-target task BattleFx_BuiltinSpawnPuffPerTarget with no delay, and sound 0x11E panned to
 * the target mask's entity.
 */
// FUNCTION: FF7 0x005d6e85
void __cdecl BattleFx_BuiltinBeginPuff(unsigned int targetMask, int actingActor)
{
    Battle_CreateBitPatternCallbackTask(targetMask, actingActor, 0, BattleFx_BuiltinSpawnPuffPerTarget);
    Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputePanForEntity(targetMask), 0x11e);
}

/*
 * Per-target puff start: allocates an effect record running BattleFx_BuiltinPuffEmitterTask, stores the target in
 * actorIndex and positions it at the target's hit bone.
 */
// FUNCTION: FF7 0x005d6ebd
void __cdecl BattleFx_BuiltinSpawnPuffPerTarget(int actor)
{
    BattleFxPuffEmitter *record;

    record = (BattleFxPuffEmitter *)BATTLE_NEW_TASK_SLOT(BattleFx_BuiltinPuffEmitterTask);
    record->actorIndex = actor;
    Battle_GetBoneWorldPos(actor, g_BattleActorRecords[actor].targetBone, &record->posX);
}

/*
 * While the frame counter is below 30 and the battle is not paused, creates three particles per call at a
 * random radius and angle around the emitter, with Y about -2100, velocity 100, scale 4096 and a random frame
 * phase. Then releases the emitter's actor record and retires it. Increments the frame counter.
 */
// FUNCTION: FF7 0x005d6f0e
void __cdecl BattleFx_BuiltinPuffEmitterTask(void)
{
    int distance;
    int theta;
    int i;
    BattleFxPuffEmitter *pEmitter;
    BattleFxPuffParticle *newParticle;
    int dataBlock;

    pEmitter = (BattleFxPuffEmitter *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    dataBlock = Gfx_GetPsxScratchpadPtr(0);
    if (pEmitter->frame < 30) {
        for (i = 0; i < 3; i++) {
            newParticle = (BattleFxPuffParticle *)BATTLE_NEW_TASK_SLOT(BattleFx_BuiltinPuffParticleTask);
            theta = rand() & 0xFFF;
            distance = rand() % 1000;
            newParticle->posX = pEmitter->posX + (SinFixed(theta) * distance >> 12);
            newParticle->posY = rand() % 100 - 2100;
            newParticle->posZ = pEmitter->posZ + (CosFixed(theta) * distance >> 12);
            newParticle->velocityY = 100;
            newParticle->scale = 0x1000 /* 1.0 */;
            newParticle->angleOffset = rand() & 7;
        }
    } else {
        Battle_PostActorHitOrHurtMessage(pEmitter->actorIndex);
        pEmitter->actorIndex = 0xFFFF;
    }
    (pEmitter->frame)++;
}

/*
 * Fades the particle's color up over its first five steps, then uses a fixed color, and draws a billboard
 * at the particle with a cycling frame and its scale. Unless paused, advances the step and Y position and
 * retires the particle once Y is positive. The Y velocity never changes.
 */
// FUNCTION: FF7 0x005d705f
void __cdecl BattleFx_BuiltinPuffParticleTask(void)
{
    BattleFxPuffParticle *task;

    task = (BattleFxPuffParticle *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleFxPuffColorDesc.puffSceneObject = Battle3d_GetSceneRenderObject(1, g_pBattle3dScene);
    if (task->step < 5) {
        g_BattleFxPuffColorDesc.puffColorByte2 = (task->step << 7) / 5;
        g_BattleFxPuffColorDesc.puffColorByte1 = g_BattleFxPuffColorDesc.puffColorByte2;
        (g_BattleFxPuffColorDesc.puffColorByte0) = g_BattleFxPuffColorDesc.puffColorByte1;
    } else {
        *(int *)&g_BattleFxPuffColorDesc = 0x2E808080;
    }
    BattleFx_InstallBillboardMatrix(&task->posX, (short)task->scale, 0);
    g_BattlePuffSpriteAngle = (task->step + task->angleOffset & 7) << 5;
    g_BattleFxPuffDrawPacketPtr = BattleFx_SubmitSpriteQuadDraw((BattleFxSpriteQuadRequest *)&g_BattleFxPuffSpriteQuad, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxPuffDrawPacketPtr);
    if (g_BattleIsPaused == 0) {
        (task->step)++;
        task->posY += task->velocityY;
        task->velocityY = task->velocityY;
        if (task->posY > 0) {
            task->angleOffset = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Runs BattleFx_LimitBeginMogDance on the targets and returns 0.
 */
// FUNCTION: FF7 0x005d7180
int BattleFx_LimitEnterMogDance(int targetMask, int actorIdx)
{
    BattleFx_LimitBeginMogDance(targetMask, actorIdx);
    return 0;
}

/*
 * Hit-spark flash with sound opcode 0x29/8: per-target hit-spark task BattleFx_LimitMogDanceSpawnHitSparksPerTarget (delayed
 * 10 frames for five or more targets).
 */
// FUNCTION: FF7 0x005d7197
void __cdecl BattleFx_LimitMogDanceBeginHitSparks(unsigned int targetMask, int actingActor)
{
    Battle_CreateBitPatternCallbackTask(
        targetMask, actingActor,
        (Battle_PopCountActorMask(targetMask) < 5) ? 0 : 10, BattleFx_LimitMogDanceSpawnHitSparksPerTarget);
    Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_CH2, Battle_ComputePanForEntity(targetMask), 8);
}

/*
 * Allocates an effect record running BattleFx_LimitMogDanceHitSparksEmitterTask for target actor with lifetime 0x3A.
 */
// FUNCTION: FF7 0x005d71e6
void BattleFx_LimitMogDanceSpawnHitSparksPerTarget(short actor)
{
    BattleHitLivewireShortEmitter *rec;

    rec = (BattleHitLivewireShortEmitter *)&g_BattleTaskSlotBuffer.mainSlots[Battle_AllocateResourceSlot(BattleFx_LimitMogDanceHitSparksEmitterTask)];
    rec->targetIndex = actor;
    rec->boneIndex = 0;
    rec->lifetime = 58;
}

/*
 * Unless paused, emits two sprites per call at the next randomized bone of the target actor, with random
 * position jitter, texture row and coordinate, and scale. At thirty frames or more, releases the actor
 * record and retires. A bone count of zero is not guarded.
 */
// FUNCTION: FF7 0x005d7226
void BattleFx_LimitMogDanceHitSparksEmitterTask(void)
{
    BattleFxLimitMogDanceHitSparks *pParent;
    int q;
    BattleFxLimitMogDanceLivewireParticle *particle;
    int i;

    pParent = (BattleFxLimitMogDanceHitSparks *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused == 0) {
        for (i = 0; i < 2; i++) {
            particle = (BattleFxLimitMogDanceLivewireParticle *)&g_BattleTaskSlotBuffer.mainSlots[Battle_AllocateResourceSlot(BattleFx_LimitMogDanceAnimatedSpriteTask)];
            pParent->boneIndex =
                (pParent->boneIndex + (rand() & 3) + 1) %
                g_BattleActorRecords[pParent->targetSlot].boneCount;
            Battle_GetBoneWorldPos(pParent->targetSlot, pParent->boneIndex,
                                   &particle->posX);
            particle->posX += rand() % 500 - 250;
            particle->posY += rand() % 500 - 250;
            particle->posZ += rand() % 500 - 250;
            particle->texRow = rand() % 3 << 5;
            particle->texCoord = g_BattleFxLimitMogDanceHitLivewireTexCoords[rand() % 5];
            particle->scale = (rand() & 0xFFF) + 0x1000;
            particle->spriteFlags = pParent->spriteFlags;
        }
        (pParent->frameCount)++;
        if (pParent->frameCount >= 30) {
            Battle_PostActorHitOrHurtMessage(pParent->targetSlot);
            pParent->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Installs a billboard with depth push -500 and submits a centered 31-unit square sprite whose cell X comes
 * from the frame table, storing the returned draw list in g_BattleFxActiveDrawListPtr. Unless paused,
 * advances the timer and retires at eight frames when cellY is 0x80, otherwise nine.
 */
// FUNCTION: FF7 0x005d73d8
void __cdecl BattleFx_LimitMogDanceAnimatedSpriteTask(void)
{
    BattleFxSpriteQuadRequest *request;
    BattleFxLimitMogDanceSlot *task;

    task = (BattleFxLimitMogDanceSlot *)BATTLE_CURRENT_TASK_SLOT;
    request = (BattleFxSpriteQuadRequest *)Gfx_GetPsxScratchpadPtr(0);
    BattleFx_InstallBillboardMatrix(&task->posX, (short)task->scale, 0xFFFFFE0C);
    request->offsetY = 0xFFF0;
    request->offsetX = 0xFFF0;
    request->cellX = g_BattleFxPuffTextureOffsets[task->timer];
    request->cellY = task->cellY;
    request->widthUnits = 31;
    request->heightUnits = 31;
    request->flags = task->flags;
    request->cellWord = task->cellWord;
    *(int *)&request->colorR = 0x2E808080;
    request->quadState = Battle3d_GetSceneRenderObject(1, g_pBattle3dScene);
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitSpriteQuadDraw(request, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (g_BattleIsPaused == 0) {
        (task->timer)++;
        if (task->timer >= (*(unsigned char *)&task->cellY != 128) + 8) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Sets up a two-record effect: g_BattleFxDrawListBufferTaskSlotPtr gets a draw-list-buffer record
 * (BattleFx_SelectDrawListBuffer) and a second record runs BattleFx_LimitMogDanceSpawnTimeline; the target mask and
 * acting actor go to g_BattleFxLimitMogDanceTargetActorMask and g_BattleFxLimitMogDanceActingActorIdx.
 */
// FUNCTION: FF7 0x005d7501
void BattleFx_LimitBeginMogDance(int targetMask, int actorIdx)
{
    g_BattleFxDrawListBufferTaskSlotPtr =
        (unsigned char *)&g_BattleTaskSlotBuffer.mainSlots[Battle_AllocateResourceSlot(BattleFx_SelectDrawListBuffer)];
    Battle_AllocateResourceSlot(BattleFx_LimitMogDanceSpawnTimeline);
    g_BattleFxLimitMogDanceTargetActorMask = targetMask;
    g_BattleFxLimitMogDanceActingActorIdx = actorIdx;
}

/*
 * Retires itself, allocates the timeline task and stores the target mask's centroid and the acting actor's
 * yaw in it. Shifts the centroid Z by +900 when the yaw is 0x800, otherwise by -900.
 */
// FUNCTION: FF7 0x005d7542
void __cdecl BattleFx_LimitMogDanceSpawnTimeline(void)
{
    BattleLimitMogDanceSpawnRecord *task;

    task = (BattleLimitMogDanceSpawnRecord *)BATTLE_CURRENT_TASK_SLOT;
    task->ownerSlot = BATTLE_TASK_FREE;
    task = (BattleLimitMogDanceSpawnRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitMogDanceTimelineTask);
    Battle_GetActorMaskCentroid(g_BattleFxLimitMogDanceTargetActorMask, &task->posX);
    task->yaw = g_BattleActorRecords[g_BattleFxLimitMogDanceActingActorIdx].yaw;
    if (task->yaw == 0x800) {
        task->posZ += 900;
    } else {
        task->posZ -= 900;
    }
}

/*
 * Unless paused: at step 6 allocates a kind-one ring record at the task's position with yaw plus 0x800. Plays
 * sound 0x263 at steps 21/29/37/44/50, adds 0x800 to the fourth actor's yaw at 36, starts the hit sparks at 48
 * and allocates a kind-two ring record and retires at 50. Increments the step last.
 */
// FUNCTION: FF7 0x005d75e6
void __cdecl BattleFx_LimitMogDanceTimelineTask(void)
{
    BattleFxAngelWhisperRingRecord *record;
    int padWord; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxMagicDeathStrip *task;

    if (g_BattleIsPaused != 0) {
        return;
    }
    task = (BattleFxMagicDeathStrip *)BATTLE_CURRENT_TASK_SLOT;
    if (task->step == 6) {
        record = (BattleFxAngelWhisperRingRecord *)Battle_AllocSceneMessage(1);
        record->pos = *(Int64Pair *)&task->posX;
        record->yaw = task->angle + 0x800;
    }
    if (task->step == 21 || (task->step == 29 || (task->step == 37 || (task->step == 44 || task->step == 50)))) {
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputeScreenPanFromPosition(&task->posX), 0x263);
    }
    if (task->step == 36) {
        g_BattleActorRecords[3].yaw += 0x800 /* 180 deg */;
    }
    if (task->step == 50) {
        Battle_AllocSceneMessage(2);
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    if (task->step == 48) {
        BattleFx_LimitMogDanceBeginHitSparks(g_BattleFxLimitMogDanceTargetActorMask, g_BattleFxLimitMogDanceActingActorIdx);
    }
    (task->step)++;
}
} /* extern "C" */
