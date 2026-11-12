// src/ff7/src/battle/yasui/yasui_special_rocket_punch_effect.cpp
/* Enemy Special Attack [129, 130]: Rocket Punch / Dual Rocket Punch (orig. SPECIAL/PUNCH) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle effect for the Punch special effect, in single and dual (Punch W) variants: a
//   screen flash and camera shake on impact, and a trail emitter that carries the punch model along
//   its path.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/types/battle.h"
#include "ff7/types/battle_boss.h"
#include "ff7/battle_tasks.h"
#include "ff7/camera_script.h"
#include "lib/types/graphics.h"
#include <string.h>
#include <stdlib.h>
#include "lib/codegen.h"
#include "ff7/api/battle.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
extern "C" {

struct BattleFxModelDraw;
// clang-format off
// GLOBAL: FF7 0x007e3610
int g_YasuiSpecRocketPunchResourceHeader = 0x001900B1;
char *g_YasuiSpecialRocketPunchEffectResourceNames[1] = {
    // STRING: FF7 0x007e3680
    "GARDL.RSD"
};
unsigned int g_YasuiSpecRocketPunchResourceHeaderDataTail[18] = {
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x007e368c
    (unsigned int)"GARDR.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b4,
    // STRING: FF7 0x007e3698
    (unsigned int)"GARDL.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b4,
    // STRING: FF7 0x007e36a4
    (unsigned int)"GARDR.RSD",
    0x00000000,
    0x00000000,
    0x00000000
};
// GLOBAL: FF7 0x007e3660
short g_YasuiSpecRocketPunchFlashIntensity = 8;
// GLOBAL: FF7 0x007e3668
struct YasuiSpecRocketPunchTrailModelDrawReq g_YasuiSpecRocketPunchTrailModelDrawReq = {{0, 0, 0, 8, 0, 0, 0, 0}, 0, 0, {0}, 128};
unsigned char g_rawData_007e367c[4] = {0x20, 0x00, 0x00, 0x00};
// clang-format on

#pragma intrinsic(memcpy)
#pragma intrinsic(memset)
#pragma intrinsic(abs)


// clang-format off
// GLOBAL: FF7 0x00a39904
int g_YasuiSpecRocketPunchRenderPacketCursor = 0;
// GLOBAL: FF7 0x00a39908
short g_YasuiSpecRocketPunchFlightDuration = 0;
// GLOBAL: FF7 0x00a3990c
struct GfxTableCursor *g_YasuiSpecRocketPunchResourceHandle = 0;
// GLOBAL: FF7 0x00a39910
short g_YasuiSpecRocketPunchDualVariantFlag = 0;
unsigned char g_bssPad_00a39914[4] = {0};
// clang-format on

/* Loads Punch special effect resources into the global handle if absent. */
// FUNCTION: FF7 0x0045b8a0
void __cdecl BattleFx_SpecialLoadResRocketPunch(void)
{
    int resourceId;
    char pathBuf[200];
    GfxResourceRequest requestBuf;
    GfxModelTableBank *sceneCtx;
    GfxResourceRequest *requestPtr;
    int header;

    if (g_pBattle3dScene != 0 && g_YasuiSpecRocketPunchResourceHandle == 0) {
        resourceId = 4;
        header = (int)&g_YasuiSpecRocketPunchResourceHeader;
        sceneCtx = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        requestPtr = &requestBuf;
        Gfx_InitResourceRequest(requestPtr);
        requestPtr->groupTag = 0;
        // STRING: FF7 0x007e36b0
        requestPtr->resourcePath = BattleFx_BuildResourcePath("SPECIAL/PUNCH", pathBuf);
        Gfx_LoadModelTableIfAbsent(resourceId, header, requestPtr, &g_YasuiSpecRocketPunchResourceHandle, sceneCtx);
    }
}

/* Entry point for the dual Punch (Punch W) variant. */
// FUNCTION: FF7 0x0045b930
void __cdecl BattleFx_SpecialEnterDualRocketPunch(int targetMask, int actorIdx)
{
    g_YasuiSpecRocketPunchFlightDuration = 6;
    g_YasuiSpecRocketPunchDualVariantFlag = 1;
    BattleFx_SpecialLoadResRocketPunch();
    g_YasuiSpecRocketPunchFlashIntensity = 8;
    BattleFx_SpecialRunRocketPunch(targetMask, actorIdx);
}

/* Entry point for the single Punch special effect. */
// FUNCTION: FF7 0x0045b965
void __cdecl BattleFx_SpecialEnterRocketPunch(int targetMask, int actorIdx)
{
    g_YasuiSpecRocketPunchDualVariantFlag = 0;
    g_YasuiSpecRocketPunchFlashIntensity = 8;
    g_YasuiSpecRocketPunchFlightDuration = 6;
    BattleFx_SpecialRunRocketPunch(targetMask, actorIdx);
    BattleFx_SpecialLoadResRocketPunch();
}

/* Loads resources, creates target callback tasks, and dispatches sound for Punch. */
// FUNCTION: FF7 0x0045b99a
void __cdecl BattleFx_SpecialRunRocketPunch(int targetMask, int sourceActor)
{
    BattleFx_SpecialLoadResRocketPunch();
    Battle_CreateBitPatternCallbackTask(targetMask, sourceActor, 4, (void *)BattleFx_SpecialSpawnRocketPunchPerTarget);
    Battle_PlaySfxPannedToActor(0x163, (short)sourceActor, 0);
}

/* Spawns the Punch timeline controller task for a target actor. */
// FUNCTION: FF7 0x0045b9cf
void __cdecl BattleFx_SpecialSpawnRocketPunchPerTarget(int actor, int sourceActor)
{
    BattleFxSpecialRocketPunch *slot;

    slot = (BattleFxSpecialRocketPunch *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialRocketPunchTimelineTask);
    Battle_GetBoneWorldPos(actor, g_BattleActorRecords[actor].targetBone, &slot->targetX);
    slot->actorIndex = sourceActor;
    slot->hitActor = actor;
}

// Returns while paused. At timer zero starts a trail emitter at the source actor's primary bone,
// with velocities of the target-minus-bone distance over g_YasuiSpecRocketPunchFlightDuration, and spawns
// a hit mark at it. In the dual variant it repeats for the secondary bone. When the timer equals
// g_YasuiSpecRocketPunchFlightDuration it starts the impact flash and shake, spawns the hit script at the
// target actor's bone, allocates its typed record and plays the impact sound. The task ends at
// timer thirty. Both branches can run at timer zero.
// FUNCTION: FF7 0x0045ba2c
void __cdecl BattleFx_SpecialRocketPunchTimelineTask(void)
{
    int unusedSpace[11]; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxSpecialRocketPunchTimeline *trail;
    int unusedField; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxSpecialRocketPunch *taskPtr;

    taskPtr = (BattleFxSpecialRocketPunch *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (taskPtr->timer == 0) {
        trail = (BattleFxSpecialRocketPunchTimeline *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialRocketPunchTrailEmitterTask);
        Battle_GetBoneWorldPos(taskPtr->actorIndex, g_BattleActorRecords[taskPtr->actorIndex].primaryBone, &trail->startX);
        trail->velocityX = (taskPtr->targetX - trail->startX) / g_YasuiSpecRocketPunchFlightDuration;
        trail->velocityY = (taskPtr->targetY - trail->startY) / g_YasuiSpecRocketPunchFlightDuration;
        trail->velocityZ = (taskPtr->targetZ - trail->startZ) / g_YasuiSpecRocketPunchFlightDuration;
        trail->state = 0;
        BattleFx_HitMarkSpawnAtPoint(&trail->startX, 0x27, 0);
        if (g_YasuiSpecRocketPunchDualVariantFlag == 1) {
            trail = (BattleFxSpecialRocketPunchTimeline *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialRocketPunchTrailEmitterTask);
            Battle_GetBoneWorldPos(taskPtr->actorIndex, g_BattleActorRecords[taskPtr->actorIndex].secondaryBone, &trail->startX);
            trail->velocityX = (taskPtr->targetX - trail->startX) / g_YasuiSpecRocketPunchFlightDuration;
            trail->velocityY = (taskPtr->targetY - trail->startY) / g_YasuiSpecRocketPunchFlightDuration;
            trail->velocityZ = (taskPtr->targetZ - trail->startZ) / g_YasuiSpecRocketPunchFlightDuration;
            trail->state = 1;
            BattleFx_HitMarkSpawnAtPoint(&trail->startX, 0x27, 0);
        }
    }
    if (taskPtr->timer == g_YasuiSpecRocketPunchFlightDuration) {
        Battle_AllocateResourceSlot(BattleFx_SpecialRocketPunchImpactFlashShakeTask);
        BattleFx_HitMarkSpawnScriptAtActorBone(taskPtr->hitActor, 0x3A, 0x1000);
        Battle_PostActorHitOrHurtMessage(taskPtr->hitActor);
        Battle_PlaySfxPannedToActor(0x14, (short)taskPtr->hitActor, 0);
    }
    (taskPtr->timer)++;
    if (taskPtr->timer == 0x1E) {
        taskPtr->step = BATTLE_TASK_FREE;
    }
}

/* Performs screen flash and camera shake upon Punch impact. */
// FUNCTION: FF7 0x0045bc85
void __cdecl BattleFx_SpecialRocketPunchImpactFlashShakeTask(void)
{
    BattleTaskSlot *task;

    task = BATTLE_CURRENT_TASK_SLOT;
    g_BattleOverlayDrawListHead = BattleFx_FlashScreenWhite(g_YasuiSpecRocketPunchFlashIntensity << 4, 0, 0);
    if (g_BattleIsPaused != 0) {
        return;
    }
    g_BattleCameraFocalPoint.focalPointY += (short)((4 - task->frameCounter) * 0x14 * (1 - (task->frameCounter % 2 << 1)));
    g_YasuiSpecRocketPunchFlashIntensity -= 2;
    (task->frameCounter)++;
    if (task->frameCounter == 4) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
}

/* Integrates punch position and spawns trail model tasks. */
// FUNCTION: FF7 0x0045bd42
void __cdecl BattleFx_SpecialRocketPunchTrailEmitterTask(void)
{
    BattleFxRocketPunchTrailParticle *pEmitter;
    BattleFxShieldTrailRecord *trailModel;

    pEmitter = (BattleFxRocketPunchTrailParticle *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    pEmitter->posX += pEmitter->velocityX;
    pEmitter->posY += pEmitter->velocityY;
    pEmitter->posZ += pEmitter->velocityZ;
    trailModel = (BattleFxShieldTrailRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialRocketPunchTrailModelTask);
    trailModel->posX = pEmitter->posX;
    trailModel->posY = pEmitter->posY;
    trailModel->posZ = pEmitter->posZ;
    trailModel->velocityX = pEmitter->velocityX;
    trailModel->velocityY = pEmitter->velocityY;
    trailModel->velocityZ = pEmitter->velocityZ;
    trailModel->modelIndex = pEmitter->slotIndex;
    (pEmitter->frameCount)++;
    if (pEmitter->frameCount == g_YasuiSpecRocketPunchFlightDuration) {
        pEmitter->ownerSlot = BATTLE_TASK_FREE;
    }
}

/* Computes orientation matrix and submits the trailing Punch 3D model draw. */
// FUNCTION: FF7 0x0045be58
void __cdecl BattleFx_SpecialRocketPunchTrailModelTask(void)
{
    VECTOR scale;
    struct {
        PsxMatrix matrix;
        short pad_matrix;
        int table[4];
        int unused[2];
        BattleFxRocketPunchTrailRecord *slot;
        PsxMatrix basis;
        short pad_basis;
        PsxMatrix view;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    frame.slot = (BattleFxRocketPunchTrailRecord *)BATTLE_CURRENT_TASK_SLOT;
    scale.vx = 0x1000;
    scale.vy = 0x1000;
    scale.vz = 0x1000;
    memset(&scale.pad, 0, sizeof(scale.pad));
    frame.table[0] = 0;
    frame.table[1] = 1;
    frame.table[2] = 2;
    frame.table[3] = 3;
    Gte_BuildLookBasisFromDirection((short *)((char *)frame.slot + 0xC), (short *)&frame.basis);
    RotMatrix((SVECTOR *)&g_YasuiSpecRocketPunchTrailModelDrawReq, (int *)&frame.matrix);
    ConcatMatrixPreservingTranslation(&frame.basis, &frame.matrix, &frame.matrix);
    ScaleMatrix(&frame.matrix, &scale);
    frame.matrix.t[0] = frame.slot->posX;
    frame.matrix.t[1] = frame.slot->posY;
    frame.matrix.t[2] = frame.slot->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.matrix, &frame.view);
    SetRotMatrix(&frame.view);
    SetTransMatrix(&frame.view);
    SetDrawColorRGB(0, 0, 0);
    g_YasuiSpecRocketPunchTrailModelDrawReq.punchTrailModelFlags = 0x80;
    g_YasuiSpecRocketPunchTrailModelDrawReq.punchTrailModelFlags |= (frame.slot->frameCount ? -1 : 0) & 8;
    g_YasuiSpecRocketPunchTrailModelDrawReq.punchTrailModelScale = frame.slot->frameCount * 0x300;
    g_YasuiSpecRocketPunchTrailModelDrawReq.punchTrailModelResource = Gfx_ModelTableGetRenderObject(
        frame.table[frame.slot->slotIndex + (frame.slot->frameCount ? 0 : 2)], g_YasuiSpecRocketPunchResourceHandle);
    g_YasuiSpecRocketPunchRenderPacketCursor = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_YasuiSpecRocketPunchTrailModelDrawReq.punchTrailModelResource, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_YasuiSpecRocketPunchRenderPacketCursor);
    if (g_BattleIsPaused != 0) {
        return;
    }
    (frame.slot->frameCount)++;
    if (frame.slot->frameCount == 4) {
        frame.slot->ownerSlot = BATTLE_TASK_FREE;
    }
}
} /* extern "C" */
