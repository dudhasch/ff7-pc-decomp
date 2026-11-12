// src/ff7/src/battle/battle_fx_special_shield.cpp
/* Enemy Special Attack [127, 128]: Shield / Dual Shield (orig. SPECIAL/SHIELD) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle special effect Shield, in a normal and a wide variant: loads its resources,
//   spawns a timeline per target, and runs the flash, projectile and trail tasks that draw it.

// clang-format off
/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/api/battle.h"
#include "ff7/battle_tasks.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "ff7/types/battle.h"
#include "lib/types/graphics.h"
#include "lib/types/polygon.h"
#include "ff7/camera_script.h"
#include <string.h>
#include "lib/codegen.h"
#include "lib/api/sound.h"
#include "ff7/types/battle_special.h"
extern "C" {

// clang-format on

#pragma function(memcpy)
// clang-format off
// GLOBAL: FF7 0x008309b8
int g_BattleSpecialShieldResourceDefs = 0x001900B1;
char *g_BattleFxSpecialShieldResourceNames[1] = {
    // STRING: FF7 0x00830a38
    "mag_a.rsd"
};
unsigned int g_BattleSpecialShieldResourceDefsDataTail[18] = {
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x00830a44
    (unsigned int)"mag_b.rsd",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b4,
    // STRING: FF7 0x00830a50
    (unsigned int)"mag_a.rsd",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b4,
    // STRING: FF7 0x00830a5c
    (unsigned int)"mag_b.rsd",
    0x00000000,
    0x00000000,
    0x00000000
};
// GLOBAL: FF7 0x00830a08
short g_BattleSpecialShieldCameraStep = 8;
// GLOBAL: FF7 0x00830a10
short g_BattleSpecialShieldRotationVector[3] = {(-1024), 0, 0};
// GLOBAL: FF7 0x00830a18
struct BattleSpecialShieldModelReq g_BattleSpecialShieldModelReq = {0, 0, {0}, 128, {0x20, 0, 0, 0}};
// GLOBAL: FF7 0x00830a28
int g_BattleSpecialShieldModelIndices[4] = {0, 1, 2, 3};
// clang-format on
#pragma data_seg(".data$u4c_orbit")
#pragma bss_seg(".data$u4c_orbit")
#pragma data_seg(".bss$u4c_orbit_slot")
#pragma bss_seg(".bss$u4c_orbit_slot")
#pragma data_seg(".bss$u4c_orbit_flag")
#pragma bss_seg(".bss$u4c_orbit_flag")
#pragma data_seg()
#pragma bss_seg()

#pragma intrinsic(memset)
#pragma function(memset)

// clang-format off
// GLOBAL: FF7 0x00b44fac
int g_BattleSpecialShieldDrawListHandle = 0;
// GLOBAL: FF7 0x00b44fb0
short g_BattleSpecialShieldVariant = 0;
// GLOBAL: FF7 0x00b44fb4
struct GfxTableCursor *g_BattleSpecialShieldLoadedResTable = 0;
// GLOBAL: FF7 0x00b44fb8
short g_BattleSpecialShieldMode = 0;
unsigned char g_bssPad_00b44fbc[4] = {0};
// clang-format on

// When the resource context is present and the cached handle is zero, prepares a default-duration
// resource record, sets its first field to zero and its path to SPECIAL/SHIELD, then requests
// resource type 4 through the shared context field at 0x4C. The reconstructed record begins at a
// scalar local and helpers access its surrounding reserved frame; object extent and allocation
// outcome are not established here. No operation occurs when either guard fails.
// FUNCTION: FF7 0x004e0e40
void __cdecl BattleFx_SpecialLoadResShield(void)
{
    GfxModelTableBank *context;
    GfxResourceRequest requestRecord;
    int kind;
    char pathOut[200];
    GfxResourceRequest *pReq;
    int resultDefEntry;

    if (g_pBattle3dScene != 0 && g_BattleSpecialShieldLoadedResTable == 0) {
        kind = 4;
        resultDefEntry = (int)&g_BattleSpecialShieldResourceDefs;
        context = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pReq = &requestRecord;
        Gfx_InitResourceRequest(pReq);
        pReq->groupTag = 0;
        // STRING: FF7 0x00830a68
        pReq->resourcePath = (char *)BattleFx_BuildResourcePath("SPECIAL/SHIELD", pathOut);
        Gfx_LoadModelTableIfAbsent(kind, resultDefEntry, pReq, &g_BattleSpecialShieldLoadedResTable, context);
    }
}

/*
 * Entry point for the wide variant of the Shield effect: sets g_BattleSpecialShieldVariant to 6 and g_BattleSpecialShieldMode
 * to 1, then calls BattleFx_SpecialRunShield with both arguments. The only difference from
 * BattleFx_SpecialEnterShield is the g_BattleSpecialShieldMode = 1.
 */
// FUNCTION: FF7 0x004e0ed0
void __cdecl BattleFx_SpecialEnterDualShield(int targetMask, int actorIdx)
{
    g_BattleSpecialShieldVariant = 6;
    g_BattleSpecialShieldMode = 1;
    BattleFx_SpecialRunShield(targetMask, actorIdx);
}

/*
 * Entry point for the Shield effect: sets g_BattleSpecialShieldVariant to 6 and calls BattleFx_SpecialRunShield
 * with both arguments, leaving g_BattleSpecialShieldMode as it was.
 */
// FUNCTION: FF7 0x004e0ef7
void __cdecl BattleFx_SpecialEnterShield(int targetMask, int actorIdx)
{
    g_BattleSpecialShieldVariant = 6;
    BattleFx_SpecialRunShield(targetMask, actorIdx);
}

/*
 * Loads Shield resources, creates a task for BattleFx_SpecialSpawnShieldPerTarget with mode 4, and dispatches event 0x142
 * for actorIdx.
 */
// FUNCTION: FF7 0x004e0f15
void __cdecl BattleFx_SpecialRunShield(int targetMask, int actorIdx)
{
    BattleFx_SpecialLoadResShield();
    Battle_CreateBitPatternCallbackTask(targetMask, actorIdx, 4, (void *)BattleFx_SpecialSpawnShieldPerTarget);
    Battle_PlaySfxPannedToActor(0x142, (short)actorIdx, 0);
}

// Allocates a slot running BattleFx_SpecialShieldTimelineTask for one target. It stores the
// target's bone world position in targetX/Y/Z, then sets actorIndex and targetIndex in the slot.
// FUNCTION: FF7 0x004e0f4a
void __cdecl BattleFx_SpecialSpawnShieldPerTarget(int actor, int actorIdx)
{
    BattleFxShieldTimeline *slot;

    slot = (BattleFxShieldTimeline *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialShieldTimelineTask);
    Battle_GetBoneWorldPos(actor, g_BattleActorRecords[actor].targetBone, &slot->targetX);
    slot->actorIndex = actorIdx;
    slot->targetIndex = actor;
}

/*
 * Returns immediately when paused. At age zero allocates a moving effect at a selected actor bone,
 * computes each velocity component as target displacement divided by the shared duration, marks
 * variant zero and dispatches its initial effect. A shared flag equal to one repeats this for the
 * second bone with variant one. At age equal to duration, allocates a follow-up callback and
 * dispatches three actor-related operations. Increments age and retires exactly at 30. Age-zero
 * and duration events are independent.
 */
// FUNCTION: FF7 0x004e0fa7
void __cdecl BattleFx_SpecialShieldTimelineTask(void)
{
    BattleFxShieldTimeline *timeline;
    int padLocal[11]; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxShieldProjectile *projectile;
    int padWord; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */

    timeline = (BattleFxShieldTimeline *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (timeline->step == 0) {
        projectile = (BattleFxShieldProjectile *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialShieldProjectileTask);
        Battle_GetBoneWorldPos(timeline->actorIndex, g_BattleActorRecords[timeline->actorIndex].primaryBone, &projectile->posX);
        projectile->velocityX = (timeline->targetX - projectile->posX) / g_BattleSpecialShieldVariant;
        projectile->velocityY = (timeline->targetY - projectile->posY) / g_BattleSpecialShieldVariant;
        projectile->velocityZ = (timeline->targetZ - projectile->posZ) / g_BattleSpecialShieldVariant;
        projectile->modelIndex = 0;
        BattleFx_HitMarkSpawnAtPoint(&projectile->posX, 0x27, 0);
        if (g_BattleSpecialShieldMode == 1) {
            projectile = (BattleFxShieldProjectile *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialShieldProjectileTask);
            Battle_GetBoneWorldPos(timeline->actorIndex, g_BattleActorRecords[timeline->actorIndex].secondaryBone, &projectile->posX);
            projectile->velocityX = (timeline->targetX - projectile->posX) / g_BattleSpecialShieldVariant;
            projectile->velocityY = (timeline->targetY - projectile->posY) / g_BattleSpecialShieldVariant;
            projectile->velocityZ = (timeline->targetZ - projectile->posZ) / g_BattleSpecialShieldVariant;
            projectile->modelIndex = 1;
            BattleFx_HitMarkSpawnAtPoint(&projectile->posX, 0x27, 0);
        }
    }
    if (timeline->step == g_BattleSpecialShieldVariant) {
        Battle_AllocateResourceSlot(BattleFx_SpecialShieldFlashShakeTask);
        BattleFx_HitMarkSpawnScriptAtActorBone(timeline->targetIndex, 0x46, 0x1000);
        Battle_PostActorHitOrHurtMessage(timeline->targetIndex);
        Battle_PlaySfxPannedToActor(0x14, (short)timeline->targetIndex, 0);
    }
    (timeline->step)++;
    if (timeline->step == 0x1E) {
        timeline->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Calls BattleFx_FlashScreenWhite before the pause check; when unpaused applies an alternating
 * shrinking camera displacement, subtracts two from g_BattleSpecialShieldCameraStep, increments the current-slot
 * counter, and retires it exactly at four.
 */
// FUNCTION: FF7 0x004e1200
void __cdecl BattleFx_SpecialShieldFlashShakeTask(void)
{
    BattleShieldFlashShake *task;

    task = (BattleShieldFlashShake *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleOverlayDrawListHead = BattleFx_FlashScreenWhite(0, g_BattleSpecialShieldCameraStep << 4, g_BattleSpecialShieldCameraStep << 4);
    if (g_BattleIsPaused != 0) {
        return;
    }
    g_BattleCameraFocalPoint.focalPointY += (short)((4 - task->shakeStep) * 0x14 * (1 - (task->shakeStep % 2 << 1)));
    g_BattleSpecialShieldCameraStep -= 2;
    (task->shakeStep)++;
    if (task->shakeStep == 4) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
}

// Per-frame handler for the active main task slot, which returns at once while the game is paused.
// It adds the velocity to the position, then allocates a child slot with
// BattleFx_SpecialShieldTrailModelTask and copies the position, velocity and modelIndex into it. It
// advances step and retires the slot (ownerSlot 0xFFFF) when step equals
// g_BattleSpecialShieldVariant.
// FUNCTION: FF7 0x004e12c3
void __cdecl BattleFx_SpecialShieldProjectileTask(void)
{
    BattleFxShieldProjectileState *projectile;
    BattleFxShieldTrailRecord *pTrail;

    projectile = (BattleFxShieldProjectileState *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    projectile->posX += projectile->velocityX;
    projectile->posY += projectile->velocityY;
    projectile->posZ += projectile->velocityZ;
    pTrail = (BattleFxShieldTrailRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialShieldTrailModelTask);
    pTrail->posX = projectile->posX;
    pTrail->posY = projectile->posY;
    pTrail->posZ = projectile->posZ;
    pTrail->velocityX = projectile->velocityX;
    pTrail->velocityY = projectile->velocityY;
    pTrail->velocityZ = projectile->velocityZ;
    pTrail->modelIndex = projectile->modelIndex;
    (projectile->step)++;
    if (projectile->step == g_BattleSpecialShieldVariant) {
        projectile->ownerSlot = BATTLE_TASK_FREE;
    }
}

// Draws one shield trail model at the trail's position, aligned with its velocity, for four frames.
#pragma intrinsic(memset)
/*
 * Draws a fading afterimage trail copy of the Shield barrier model.
 */
// FUNCTION: FF7 0x004e13d9
void __cdecl BattleFx_SpecialShieldTrailModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[124];
        PsxMatrix rotMatrix;
        struct { unsigned char pad_to_28[0x28]; BattleFxShieldTrail *shieldTrail; };
        struct { unsigned char pad_to_2C[0x2C]; PsxMatrix basisMatrix; };
        struct { unsigned char pad_to_4C[0x4C]; PsxMatrix drawMatrix; };
        struct { unsigned char pad_to_6C[0x6C]; GfxDirectionVec3 scaleX; };
        struct { unsigned char pad_to_78[0x78]; int zeroWord; };
    } frame;
#pragma pack(pop)

    frame.shieldTrail = (BattleFxShieldTrail *)BATTLE_CURRENT_TASK_SLOT;
    frame.scaleX.x = 0x1000;
    frame.scaleX.y = 0x1000;
    frame.scaleX.z = 0x1000;
    memset(&frame.zeroWord, 0, 4);
    Gte_BuildLookBasisFromDirection(&frame.shieldTrail->velocityX, frame.basisMatrix.m[0]);
    RotMatrix((SVECTOR *)&g_BattleSpecialShieldRotationVector, &frame.rotMatrix);
    ConcatMatrixPreservingTranslation(&frame.basisMatrix, &frame.rotMatrix, &frame.rotMatrix);
    ScaleMatrix(&frame.rotMatrix, &frame.scaleX);
    frame.rotMatrix.t[0] = frame.shieldTrail->posX;
    frame.rotMatrix.t[1] = frame.shieldTrail->posY;
    frame.rotMatrix.t[2] = frame.shieldTrail->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.drawMatrix);
    SetRotMatrix(&frame.drawMatrix);
    SetTransMatrix(&frame.drawMatrix);
    SetDrawColorRGB(0, 0, 0);
    g_BattleSpecialShieldModelReq.flags4 = 0x80;
    g_BattleSpecialShieldModelReq.flags4 |= frame.shieldTrail->variant != 0 ? 8 : 0;
    g_BattleSpecialShieldModelReq.alpha = frame.shieldTrail->variant * 0x300;
    (g_BattleSpecialShieldModelReq.modelDraw) = Gfx_ModelTableGetRenderObject(g_BattleSpecialShieldModelIndices[frame.shieldTrail->modelIndex + (frame.shieldTrail->variant != 0 ? -2 + 2 : 2)], g_BattleSpecialShieldLoadedResTable);
    g_BattleSpecialShieldDrawListHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&(g_BattleSpecialShieldModelReq.modelDraw), g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialShieldDrawListHandle);
    if (g_BattleIsPaused != 0) {
        return;
    }
    (frame.shieldTrail->variant)++;
    if (frame.shieldTrail->variant == 4) {
        frame.shieldTrail->ownerSlot = BATTLE_TASK_FREE;
    }
}
} /* extern "C" */
