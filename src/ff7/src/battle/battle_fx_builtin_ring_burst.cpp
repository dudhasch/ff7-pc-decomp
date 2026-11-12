// src/ff7/src/battle/battle_fx_builtin_ring_burst.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Built-in ring-burst effect: its entry point starts a burst whose speed depends on the
//   target count, and per-target callbacks run an emitter and a frame-table billboard particle.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/enums/sound.h"
#include "ff7/api/battle.h"
#include "ff7/battle_tasks.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_fx.h"
#include "lib/types/graphics.h"
#include "ff7/battle.h"
#include "ff7/gte.h"
#include "lib/crt.h"
#include "lib/codegen.h"
extern "C" {

// clang-format off
// GLOBAL: FF7 0x008ff100
struct BattleFxRingBurstSpriteQuad g_BattleFxRingBurstSpriteQuad = {{0xf0, 0xff, 0xf0, 0xff}, 0, {0x40, 0x1f, 0x1f, 0x80, 0x80, 0x80, 0x2e, 0x3a, 0, 0x47, 0x79}, 0, {0}};
// GLOBAL: FF7 0x008ff118
char g_BattleFxRingBurstColorTable[8] = {0x00, 0x20, 0x40, 0x60, (char)0x80, (char)0xa0, (char)0x80, 0x60};
// clang-format on


// clang-format off
unsigned char g_rawData_008ff120[24] = {0x80, 0xa0, 0x80, 0x60, 0x80, 0xa0, 0xc0, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x64, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x00c3f2b8
int g_BattleFxRingBurstSpeed = 0;
unsigned char g_bssPad_00c3f2bc[4] = {0};
// GLOBAL: FF7 0x00c3f2c0
struct Int64Pair g_BattleFxRingBurstAngleVector = {0};
// clang-format on

/*
 * Entry point of the ring-burst effect.
 */
// FUNCTION: FF7 0x005d6a70
void BattleFx_EnterBuiltinRingBurst(int targetMask, int actorIdx)
{
    BattleFx_BuiltinBeginRingBurst(targetMask, actorIdx);
}

/*
 * Starts the effect: g_BattleFxRingBurstSpeed (particles per frame) is 6 for a single target and 3 for more; creates the
 * per-target task BattleFx_BuiltinSpawnRingBurstPerTarget (delayed 10 frames when more than three targets) and clears
 * g_BattleHurtMessageActorMask.
 */
// FUNCTION: FF7 0x005d6a85
void __cdecl BattleFx_BuiltinBeginRingBurst(int targetMask, int actorIdx)
{
    int count;

    count = Battle_PopCountActorMask(targetMask);
    g_BattleFxRingBurstSpeed = (count - 1 != 0 ? 0xFFFFFFFD : 0) + 6;
    Battle_CreateBitPatternCallbackTask(targetMask, actorIdx, count <= 3 ? 0 : 0xA, (void *)BattleFx_BuiltinSpawnRingBurstPerTarget);
    g_BattleHurtMessageActorMask = 0;
}

/*
 * Allocates callback record, writes actor short and bone position, overwrites position middle
 * short zero then dispatches sound opcode 0x20 with derived screen pan and 0x8B.
 */
// FUNCTION: FF7 0x005d6ae1
void __cdecl BattleFx_BuiltinSpawnRingBurstPerTarget(int actor)
{
    BattleFxRingBurstEmitter *record;

    record = (BattleFxRingBurstEmitter *)BATTLE_NEW_TASK_SLOT(BattleFx_BuiltinRingBurstEmitterTask);
    record->actorIndex = actor;
    Battle_GetBoneWorldPos(actor, g_BattleActorRecords[actor].targetBone, (short *)((int)record + 8));
    record->posY = 0;
    Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputeScreenPanFromPosition((char *)record + 8), 0x8B);
}

/*
 * Fetches scratch even while paused. Unpaused counters below ten emit configured count of
 * particles with shared random starting angle, equal angle steps and randomized
 * radius/speed/offset fields. Later allocates typed actor and retires; counter increments.
 * Zero/negative count emits none and avoids division.
 */
// FUNCTION: FF7 0x005d6b5b
void __cdecl BattleFx_BuiltinRingBurstEmitterTask(void)
{
    BattleFxRingBurstEmitterSlot *taskPtr;
    int unused; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int i;
    BattleRingBurstEmitterSlot *newParticle;
    int dataBlock;

    taskPtr = (BattleFxRingBurstEmitterSlot *)BATTLE_CURRENT_TASK_SLOT;
    dataBlock = Gfx_GetPsxScratchpadPtr(0);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (taskPtr->step < 0xA) {
        *(short *)(((unsigned char *)&g_BattleFxRingBurstAngleVector) + 2) = rand() & 0xFFF;
        for (i = 0; i < g_BattleFxRingBurstSpeed; i++) {
            newParticle = (BattleRingBurstEmitterSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_BuiltinRingBurstParticleTask);
            *(Int64Pair *)((char *)newParticle + 8) = *(Int64Pair *)((char *)taskPtr + 8);
            newParticle->speed = rand() % 0x2000 + 0x1000;
            *(Int64Pair *)((char *)newParticle + 0x10) = *(Int64Pair *)(&g_BattleFxRingBurstAngleVector);
            newParticle->radius = rand() % 0x190 + 0x64 << 4;
            newParticle->offset = rand() % 0x64 - 0x32;
            *(short *)(((unsigned char *)&g_BattleFxRingBurstAngleVector) + 2) += (short)(0x1000 / g_BattleFxRingBurstSpeed);
        }
    } else {
        Battle_PostActorHitOrHurtMessage(taskPtr->recordType);
        taskPtr->status = 0xFFFF;
    }
    (taskPtr->step)++;
}

/*
 * Draws frame-table billboard with ignored returned chain. Unpaused advances frame and retires at
 * seventeen; earlier frames rotate scalar velocity into triple and add shifted components to
 * position, damp speed by one-eighth, approach angle minus 1024 by quarter difference and add
 * angular step. Paused still draws.
 */
// FUNCTION: FF7 0x005d6cb6
void __cdecl BattleFx_BuiltinRingBurstParticleTask(void)
{
    BattleFxRingBurstParticle *taskPtr;
    int unusedSpace; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxWhiteWindFadeState *state;
    int matrix;

    taskPtr = (BattleFxRingBurstParticle *)BATTLE_CURRENT_TASK_SLOT;
    state = (BattleFxWhiteWindFadeState *)Gfx_GetPsxScratchpadPtr(0);
    matrix = (int)state + 8;
    g_BattleFxRingBurstSpriteQuad.ringBurstSpriteColor = *(char *)(g_BattleFxRingBurstColorTable + taskPtr->colorIndex);
    BattleFx_InstallBillboardMatrix((char *)taskPtr + 8, (short)taskPtr->angle, 0);
    g_BattleFxRingBurstSpriteQuad.ringBurstSceneObject = Battle3d_GetSceneRenderObject(1, g_pBattle3dScene);
    BattleFx_SubmitSpriteQuadDraw((BattleFxSpriteQuadRequest *)&g_BattleFxRingBurstSpriteQuad, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, 0);
    if (g_BattleIsPaused == 0) {
        (taskPtr->colorIndex)++;
        if (taskPtr->colorIndex >= 0x11) {
            taskPtr->ownerSlot = BATTLE_TASK_FREE;
        } else {
            state->velocityX = taskPtr->velocityX;
            state->velocityY = 0;
            state->velocityZ = 0;
            RotMatrix((SVECTOR *)((int)taskPtr + 0x10), (void *)matrix);
            TransformVectorToInt16((void *)matrix, (void *)state, (void *)state);
            taskPtr->posX += (short)(state->velocityX >> 4);
            taskPtr->posY += (short)(state->velocityY >> 4);
            taskPtr->posZ += (short)(state->velocityZ >> 4);
            taskPtr->velocityX -= (short)(taskPtr->velocityX >> 3);
            taskPtr->velocityY += (short)(-0x400 - taskPtr->velocityY >> 2);
            taskPtr->spinAngle += taskPtr->angularStep;
        }
    }
}
} /* extern "C" */
