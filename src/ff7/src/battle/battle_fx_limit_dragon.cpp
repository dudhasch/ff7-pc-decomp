// src/ff7/src/battle/battle_fx_limit_dragon.cpp
// Limit Break [29] (Cid Lv 1-2): Dragon (Japanese: ドラゴンモッド Dragon Mod, dev tag: "LIMIT3/D_M" / DM)
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Dolphin Miracle limit effect: loads its resources and runs a master timeline that
//   drives a trailing projectile, animated variant sprites, a pulsing model and radial pulsing
//   sprites, with variant emitters.
/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/types/battle.h"
#include "ff7/types/battle_limit.h"
#include "ff7/battle_tasks.h"
#include "lib/types/graphics.h"
#include "lib/enums/sound.h"
#include "lib/codegen.h"
#include "lib/types/polygon.h"
#include "ff7/camera_script.h"
#include <stdlib.h>
#include <string.h>
#include "ff7/api/battle.h"
#include "ff7/api/menu.h"
#include "lib/api/sound.h"
#include "lib/api/graphics.h"
extern "C" {

#pragma intrinsic(memset)

// clang-format off
// GLOBAL: FF7 0x00808d08
unsigned int g_BattleLimitDragonResourceDef[130] ={
    0x001900b1,
    (unsigned int)&g_BattleLimitDragonHeadModelName,
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x00808fec
    (unsigned int)"CHEST_1.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x00808ff8
    (unsigned int)"CHEST_2.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x00809004
    (unsigned int)"CHEST_3.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x00809010
    (unsigned int)"CHEST_4.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x0080901c
    (unsigned int)"HIP_1.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x00809028
    (unsigned int)"HIP_2.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x00809034
    (unsigned int)"HIP_3.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x00809040
    (unsigned int)"TAIL_1.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x0080904c
    (unsigned int)"TAIL_2.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x00809058
    (unsigned int)"TAIL_3.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x00809064
    (unsigned int)"TAIL_4.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x00809070
    (unsigned int)"TAIL_5.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x0080907c
    (unsigned int)"TOP.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x00809084
    (unsigned int)"L_FIN.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x00809090
    (unsigned int)"R_FIN.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x0080909c
    (unsigned int)"UPPER.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x008090a8
    (unsigned int)"TONGUE1.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x008090b4
    (unsigned int)"TONGUE2.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x008090c0
    (unsigned int)"TONGUE3.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x008090cc
    (unsigned int)"HIKARI.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x008090d8
    (unsigned int)"MOD.TIM",
    // STRING: FF7 0x008090e0
    (unsigned int)"BIMU11.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x008090ec
    (unsigned int)"MOD.TIM",
    // STRING: FF7 0x008090f4
    (unsigned int)"BIMU22.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    0x00000000,
    // STRING: FF7 0x00809100
    (unsigned int)"JO_1.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    0x00000000,
    // STRING: FF7 0x0080910c
    (unsigned int)"JO_2.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    0x00000000,
    // STRING: FF7 0x00809118
    (unsigned int)"JO_A.SPT"
};
// GLOBAL: FF7 0x00808f10
BattleLimitDMSpriteReq1 g_BattleLimitDragonVariantSpriteReq = {0, 0, {0x80, 0x80, 0x80, 0x2c}, 0};
// The variant sprites' resource indices: JO_1, JO_2 and JO_A.
// GLOBAL: FF7 0x00808f20
int g_BattleLimitDragonVariantSpriteIndices[4] = {23, 24, 25, 0};
// GLOBAL: FF7 0x00808f30
BattleLimitDMModelReq g_BattleLimitDragonTrailModelReq = {0, 0, {0}, 0, {0x20, 0, 0, 0}};
// The trail's model for each segment after the first: HEAD to TOP.
// GLOBAL: FF7 0x00808f40
int g_BattleLimitDragonTrailModelIndices[14] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
// The first segment's models: HEAD, L_FIN, R_FIN and UPPER.
// GLOBAL: FF7 0x00808f78
int g_BattleLimitDragonHeadModelIndices[4] = {0, 14, 15, 16};
// The tongue's three animation frames, TONGUE1 to TONGUE3.
// GLOBAL: FF7 0x00808f88
int g_BattleLimitDragonTongueModelIndices[4] = {17, 18, 19, 0};
// The scale of each trail segment's model.
// GLOBAL: FF7 0x00808f98
short g_BattleLimitDragonTrailScales[14] = {0x2800, 0x2800, 0x3000, 0x3000, 0x3000, 0x3000, 0x3000, 0x3000, 0x3000, 0x3000, 0x3000, 0x3000, 0x3000, 0x3000};
// The pulsing model's draw request (HIKARI).
// GLOBAL: FF7 0x00808fb8
BattleLimitDMModelReq g_BattleLimitDragonPulseModelReq = {0, 0x88, {0}, 0, {0x20, 0, 0, 0}};
// GLOBAL: FF7 0x00808fc8
BattleLimitDMSpriteReq2 g_BattleLimitDragonRadialSpriteReq = {0, 0, -128, -128, -128, 0x2c, 0};
// The radial sprites' resource indices: BIMU11 and BIMU22.
// GLOBAL: FF7 0x00808fd8
int g_BattleLimitDragonRadialSpriteIndices[2] = {21, 22};
// GLOBAL: FF7 0x00808fe0
unsigned char g_BattleLimitDragonHeadModelName[9] = "HEAD.RSD";
// clang-format on

/* The timeline's phases: counted from 0, then from phaseStart, the phase the projectile reaches
 * the target (distance / 200 steps after DM_PHASE_LAUNCH). */
#define DM_PHASE_LAUNCH 0xA
#define DM_PHASE_LAUNCH_SOUND 0x22
#define DM_AFTER_ARRIVAL_HIT 0x14
#define DM_AFTER_ARRIVAL_EMIT1 0x28
#define DM_AFTER_ARRIVAL_EMIT2 0x3C
#define DM_AFTER_ARRIVAL_END 0x5A

/* The four sounds the effect loads, plays and releases. */
#define DM_SFX_LAUNCH 0x69
#define DM_SFX_ARRIVAL 0x241
#define DM_SFX_EMIT1 0x1F
#define DM_SFX_EMIT2 8

#pragma intrinsic(memcpy)
#pragma function(memcpy)

/* The trail's last 16 positions: one 8-byte point each over g_BattleLimitDragonPosHistory and the
 * 120 bytes after it. g_BattleLimitDragonTrailWriteIndex is the ring's write index. */
typedef struct {
    short x;
    short y;
    short z;
    short pad;
} BattleFxLimitDMTrailPoint;
// clang-format off
// GLOBAL: FF7 0x00af0390
BattleFxLimitDMTrailPoint g_BattleLimitDragonPosHistory = {0};
unsigned char g_bssPad_00af0398[120] = {0};
#define g_BattleLimitDragonTrail (&g_BattleLimitDragonPosHistory)
// GLOBAL: FF7 0x00af0410
int g_BattleLimitDragonDrawListHead = 0;
// GLOBAL: FF7 0x00af0414
short g_BattleLimitDragonTrailFade = 0;
// GLOBAL: FF7 0x00af0418
struct GfxTableCursor *g_BattleLimitDragonLoadedResTable = 0;
// GLOBAL: FF7 0x00af041c
short g_BattleLimitDragonTrailWriteIndex = 0;
unsigned char g_bssPad_00af0420[8] = {0};
// GLOBAL: FF7 0x00af0428
int g_BattleLimitDragonPulseAngles = 0;
unsigned char g_bssPad_00af042c[4] = {0};
// clang-format on

/*
 * Loads Dolphin Miracle (D_M) limit break resources (LIMIT3/D_M) if not already loaded.
 */
// FUNCTION: FF7 0x004b1800
void __cdecl BattleFx_LimitLoadResDragon(void)
{
    char pathBuf[200];
    GfxModelTableBank *sceneHandle;
    GfxResourceRequest requestStorage;
    int type;
    GfxResourceRequest *requestPtr;
    int descPtr;

    if (g_pBattle3dScene != 0 && g_BattleLimitDragonLoadedResTable == 0) {
        type = 0x1A;
        descPtr = (int)&g_BattleLimitDragonResourceDef;
        sceneHandle = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        requestPtr = &requestStorage;
        Gfx_InitResourceRequest(requestPtr);
        requestPtr->groupTag = 0;
        // STRING: FF7 0x00809124
        requestPtr->resourcePath = BattleFx_BuildResourcePath("LIMIT3/D_M", pathBuf);
        Gfx_LoadModelTableIfAbsent(type, descPtr, requestPtr, &g_BattleLimitDragonLoadedResTable, sceneHandle);
    }
}

/*
 * Loads the resources, clears the trail's write index and fade, and registers the per-target
 * spawn. Returns 0.
 */
// FUNCTION: FF7 0x004b1890
int __cdecl BattleFx_LimitEnterDragon(int targetMask, int actorIdx)
{
    BattleFx_LimitLoadResDragon();
    g_BattleLimitDragonTrailWriteIndex = 0;
    g_BattleLimitDragonTrailFade = 0;
    BattleFx_LimitRegisterDragonPerTargetCallback(targetMask, actorIdx);
    return 0;
}

/*
 * Creates a callback task that calls BattleFx_LimitSpawnDragonPerTarget for each target in targetMask.
 */
// FUNCTION: FF7 0x004b18be
void __cdecl BattleFx_LimitRegisterDragonPerTargetCallback(int targetMask, int sourceActor)
{
    Battle_CreateBitPatternCallbackTask(targetMask, sourceActor, 4, (void *)BattleFx_LimitSpawnDragonPerTarget);
}

/*
 * Starts the timeline task for one target: the source actor's bone position, the target's
 * position, both actors and a first phaseStart of 300.
 */
// FUNCTION: FF7 0x004b18da
void __cdecl BattleFx_LimitSpawnDragonPerTarget(int actor, int sourceActor)
{
    BattleFxLimitDmTrack *slot;

    slot = (BattleFxLimitDmTrack *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitDragonTimelineTask);
    Battle_GetBoneWorldPos(sourceActor, g_BattleActorRecords[sourceActor].targetBone, &slot->posX);
    slot->targetX = g_BattleActorRecords[actor].posX;
    slot->targetY = g_BattleActorRecords[actor].posY;
    slot->targetZ = g_BattleActorRecords[actor].posZ;
    slot->sourceActor = sourceActor;
    slot->targetActor = actor;
    slot->phaseStart = 0x12C;
}

/*
 * Master timeline task for Dolphin Miracle effect sequence. At DM_PHASE_LAUNCH it spawns the
 * trailing projectile 0x12C0 units behind the source and sets phaseStart to the phase it
 * arrives; at the arrival it spawns the pulsing model and 32 radial sprites, then hits the target
 * and runs the two sprite emitters. The checks before `(s->phase)++` see this frame's phase, the
 * end check after it sees the next one.
 */
// FUNCTION: FF7 0x004b1985
void __cdecl BattleFx_LimitDragonTimelineTask(void)
{
    short heading;
    short distance;
    BattleFxLimitDmTrack *taskPtr;
    short xDelta;
    short deltaZ;
    short i;
    BattleFxLimitDMTaskRecord *pRecord;

    taskPtr = (BattleFxLimitDmTrack *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (taskPtr->phase == 0) {
        SfxLoadIntoBuffer(DM_SFX_LAUNCH, 0);
        SfxLoadIntoBuffer(DM_SFX_ARRIVAL, 0);
        SfxLoadIntoBuffer(DM_SFX_EMIT1, 0);
        SfxLoadIntoBuffer(DM_SFX_EMIT2, 0);
    }
    if (taskPtr->phase == DM_PHASE_LAUNCH) {
        heading = ArcTangentFixed(taskPtr->targetX - taskPtr->posX, taskPtr->targetZ - taskPtr->posZ);
        pRecord = (BattleFxLimitDMTaskRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitDragonTrailingProjectileTask);
        pRecord->projectile.posX = taskPtr->posX - (SinFixed(heading) * 0x12C0 >> 0xC);
        pRecord->projectile.posZ = taskPtr->posZ - (CosFixed(heading) * 0x12C0 >> 0xC);
        pRecord->projectile.posY = 0xFB50;
        for (i = 0; i < 0x10; i++) {
            g_BattleLimitDragonTrail[i].x = pRecord->projectile.posX;
            g_BattleLimitDragonTrail[i].y = pRecord->projectile.posY;
            g_BattleLimitDragonTrail[i].z = pRecord->projectile.posZ;
        }
        xDelta = taskPtr->targetX - pRecord->projectile.posX;
        deltaZ = taskPtr->targetZ - pRecord->projectile.posZ;
        distance = SqrtToInt(xDelta * xDelta + deltaZ * deltaZ);
        pRecord->projectile.arrivalFrame = distance / 0xC8;
        pRecord->projectile.spiralRadius = 0;
        pRecord->projectile.velocityX = SinFixed(heading) * 0xC8 >> 0xC;
        pRecord->projectile.velocityZ = CosFixed(heading) * 0xC8 >> 0xC;
        pRecord->projectile.invDistance = 0x3E8 / distance;
        taskPtr->phaseStart = pRecord->projectile.arrivalFrame + DM_PHASE_LAUNCH;
    }
    if (taskPtr->phase == DM_PHASE_LAUNCH_SOUND) {
        SoundDispatchCommand(SOUND_SFX_PLAY_CH1, Battle_ComputeScreenPanFromPosition(&taskPtr->posX), DM_SFX_LAUNCH, 0, 0, 0, 0, 0, 0);
    }
    if (taskPtr->phase == taskPtr->phaseStart) {
        pRecord = (BattleFxLimitDMTaskRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitDragonPulsingModelTask);
        pRecord->pulse.posX = taskPtr->targetX;
        pRecord->pulse.posY = taskPtr->targetY;
        pRecord->pulse.posZ = taskPtr->targetZ;
        pRecord->pulse.pulsePhase = 0;
        for (i = 0; i < 0x20; i++) {
            pRecord = (BattleFxLimitDMTaskRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitDragonPulsingRadialSpriteTask);
            pRecord->radial.posX = taskPtr->targetX;
            pRecord->radial.posY = taskPtr->targetY;
            pRecord->radial.posZ = taskPtr->targetZ;
            pRecord->radial.resIndex = rand() % 2;
            pRecord->radial.cycleIndex = rand() % 4;
            pRecord->radial.radius = 0;
            pRecord->radial.angle = i << 7;
        }
        g_BattleFxTimelineState.channel2 = 0x10;
        g_BattleFxTimelineState.channel1 = g_BattleFxTimelineState.channel2;
        g_BattleFxTimelineState.red = g_BattleFxTimelineState.channel1;
        Battle_StartFixedPointRamp(0x1000, 0xA);
        SoundDispatchCommand(SOUND_SFX_PLAY_CH2, Battle_ComputeScreenPanFromPosition(&taskPtr->targetX), DM_SFX_ARRIVAL, 0, 0, 0, 0, 0, 0);
    }
    if (taskPtr->phase == taskPtr->phaseStart + DM_AFTER_ARRIVAL_HIT) {
        Battle_PostActorHitOrHurtMessage(taskPtr->targetActor);
    }
    if (taskPtr->phase == taskPtr->phaseStart + DM_AFTER_ARRIVAL_EMIT1) {
        pRecord = (BattleFxLimitDMTaskRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitDragonVariantSpriteEmitterTask);
        Battle_GetBoneWorldPos(taskPtr->sourceActor, g_BattleActorRecords[taskPtr->sourceActor].targetBone, &pRecord->emitter.baseX);
        pRecord->emitter.variantIndex = 0;
        SoundDispatchCommand(SOUND_SFX_PLAY_CH1, Battle_ComputeScreenPanFromPosition(&taskPtr->posX), DM_SFX_EMIT1, 0, 0, 0, 0, 0, 0);
    }
    if (taskPtr->phase == taskPtr->phaseStart + DM_AFTER_ARRIVAL_EMIT2) {
        pRecord = (BattleFxLimitDMTaskRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitDragonVariantSpriteEmitterTask);
        Battle_GetBoneWorldPos(taskPtr->sourceActor, g_BattleActorRecords[taskPtr->sourceActor].targetBone, &pRecord->emitter.baseX);
        pRecord->emitter.variantIndex = 1;
        SoundDispatchCommand(SOUND_SFX_PLAY_CH1, Battle_ComputeScreenPanFromPosition(&taskPtr->posX), DM_SFX_EMIT2, 0, 0, 0, 0, 0, 0);
    }
    (taskPtr->phase)++;
    if (taskPtr->phase == taskPtr->phaseStart + DM_AFTER_ARRIVAL_END) {
        Battle_StartFixedPointRamp(0, 0xA);
        taskPtr->ownerSlot = BATTLE_TASK_FREE;
        Sfx_ReleaseCachedBuffer(DM_SFX_LAUNCH, 0xF);
        Sfx_ReleaseCachedBuffer(DM_SFX_ARRIVAL, 0xF);
        Sfx_ReleaseCachedBuffer(DM_SFX_EMIT1, 0xF);
        Sfx_ReleaseCachedBuffer(DM_SFX_EMIT2, 0xF);
    }
}

/*
 * Draws the dolphin as 14 model segments along the trail ring (the first is the head, fins and
 * animated tongue), scaled by g_BattleLimitDragonTrailScales and faded by g_BattleLimitDragonTrailFade.
 * Moves the projectile on a wave until it arrives, then spirals it upward, spawns sprites along
 * the trail and records the new position in the ring.
 */
// FUNCTION: FF7 0x004b1f12
void __cdecl BattleFx_LimitDragonTrailingProjectileTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[124];
        short fromX;
        struct { unsigned char pad_to_2[0x2]; short fromY; };
        struct { unsigned char pad_to_4[0x4]; short fromZ; };
        struct { unsigned char pad_to_8[0x8]; PsxMatrix lookBasis; };
        struct { unsigned char pad_to_28[0x28]; BattleFxLimitDragonVariantSprite *spawnedSprite; };
        struct { unsigned char pad_to_2C[0x2C]; short partIndex; };
        struct { unsigned char pad_to_30[0x30]; short segmentIndex; };
        struct { unsigned char pad_to_34[0x34]; short randomOffset; };
        struct { unsigned char pad_to_38[0x38]; SVECTOR dirX; };
        struct { unsigned char pad_to_40[0x40]; BattleFxDmTrailProjectile *projectile; };
        struct { unsigned char pad_to_44[0x44]; PsxMatrix composedMatrix; };
        struct { unsigned char pad_to_64[0x64]; short toX; };
        struct { unsigned char pad_to_66[0x66]; short toY; };
        struct { unsigned char pad_to_68[0x68]; short toZ; };
        struct { unsigned char pad_to_6C[0x6C]; VECTOR scaleX; };
    } frame;
#pragma pack(pop)

    frame.projectile = (BattleFxDmTrailProjectile *)BATTLE_CURRENT_TASK_SLOT;
    if (frame.projectile->timer > 0xE) {
        for (frame.segmentIndex = 0; frame.segmentIndex < 0xE; frame.segmentIndex++) {
            frame.fromX = g_BattleLimitDragonTrail[(g_BattleLimitDragonTrailWriteIndex + frame.segmentIndex + 1) & 0xF].x;
            frame.fromY = g_BattleLimitDragonTrail[(g_BattleLimitDragonTrailWriteIndex + frame.segmentIndex + 1) & 0xF].y;
            frame.fromZ = g_BattleLimitDragonTrail[(g_BattleLimitDragonTrailWriteIndex + frame.segmentIndex + 1) & 0xF].z;
            frame.toX = g_BattleLimitDragonTrail[(g_BattleLimitDragonTrailWriteIndex + frame.segmentIndex + 2) & 0xF].x;
            frame.toY = g_BattleLimitDragonTrail[(g_BattleLimitDragonTrailWriteIndex + frame.segmentIndex + 2) & 0xF].y;
            frame.toZ = g_BattleLimitDragonTrail[(g_BattleLimitDragonTrailWriteIndex + frame.segmentIndex + 2) & 0xF].z;
            frame.dirX.vx = frame.toX - frame.fromX;
            frame.dirX.vy = frame.toY - frame.fromY;
            frame.dirX.vz = frame.toZ - frame.fromZ;
            Gte_BuildLookBasisFromDirection(&frame.dirX.vx, frame.lookBasis.m[0]);
            frame.scaleX.vz = g_BattleLimitDragonTrailScales[frame.segmentIndex];
            frame.scaleX.vy = frame.scaleX.vz;
            frame.scaleX.vx = frame.scaleX.vy;
            ScaleMatrix(&frame.lookBasis, &frame.scaleX);
            frame.lookBasis.t[0] = frame.fromX;
            frame.lookBasis.t[1] = frame.fromY;
            frame.lookBasis.t[2] = frame.fromZ;
            MultiplyMatrix(&g_BattleCameraTransform, &frame.lookBasis, &frame.composedMatrix);
            SetRotMatrix(&frame.composedMatrix);
            SetTransMatrix(&frame.composedMatrix);
            SetDrawColorRGB(0, 0, 0);
            g_BattleLimitDragonTrailModelReq.fadeLevel = 0x1000 - (g_BattleLimitDragonTrailFade << 8);
            g_BattleLimitDragonTrailModelReq.flags = (g_BattleLimitDragonTrailFade - 0x10 != 0 ? 8 : 0) | 0x80;
            if (frame.segmentIndex == 0) {
                for (frame.partIndex = 0; frame.partIndex < 4; frame.partIndex++) {
                    g_BattleLimitDragonTrailModelReq.modelGroup = Gfx_ModelTableGetRenderObject(g_BattleLimitDragonHeadModelIndices[frame.partIndex], g_BattleLimitDragonLoadedResTable);
                    g_BattleLimitDragonDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleLimitDragonTrailModelReq, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleLimitDragonDrawListHead);
                }
                g_BattleLimitDragonTrailModelReq.modelGroup = Gfx_ModelTableGetRenderObject(g_BattleLimitDragonTongueModelIndices[frame.projectile->timer % 3], g_BattleLimitDragonLoadedResTable);
                g_BattleLimitDragonDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleLimitDragonTrailModelReq, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleLimitDragonDrawListHead);
            } else {
                g_BattleLimitDragonTrailModelReq.modelGroup = Gfx_ModelTableGetRenderObject(g_BattleLimitDragonTrailModelIndices[frame.segmentIndex], g_BattleLimitDragonLoadedResTable);
                g_BattleLimitDragonDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleLimitDragonTrailModelReq, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleLimitDragonDrawListHead);
            }
        }
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.projectile->timer < frame.projectile->arrivalFrame) {
        frame.projectile->posX += frame.projectile->velocityX;
        frame.projectile->posZ += frame.projectile->velocityZ;
        frame.projectile->posY += (short)(SinFixed(frame.projectile->timer * 0x190) * 0x78 >> 0xC);
    } else {
        if (frame.projectile->spiralRadius != 0x320) {
            frame.projectile->spiralRadius += 0x50;
        }
        frame.projectile->posY += (short)((SinFixed(frame.projectile->timer << 9) * 0x32 >> 0xC) - 0x64);
    }
    if (frame.projectile->timer > frame.projectile->arrivalFrame + 5) {
        frame.randomOffset = rand() % 0xE + 1;
        frame.spawnedSprite = (BattleFxLimitDragonVariantSprite *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitDragonVariantSpriteTask);
        frame.spawnedSprite->posX = g_BattleLimitDragonTrail[(g_BattleLimitDragonTrailWriteIndex + frame.randomOffset) & 0xF].x;
        frame.spawnedSprite->posY = g_BattleLimitDragonTrail[(g_BattleLimitDragonTrailWriteIndex + frame.randomOffset) & 0xF].y;
        frame.spawnedSprite->posZ = g_BattleLimitDragonTrail[(g_BattleLimitDragonTrailWriteIndex + frame.randomOffset) & 0xF].z;
        frame.spawnedSprite->posX += (short)(rand() % 0x3E8 - 0x1F4);
        frame.spawnedSprite->posY += (short)(rand() % 0x3E8 - 0x1F4);
        frame.spawnedSprite->posZ += (short)(rand() % 0x3E8 - 0x1F4);
        frame.spawnedSprite->variantIndex = 2;
    }
    g_BattleLimitDragonTrail[g_BattleLimitDragonTrailWriteIndex].x = frame.projectile->posX + (SinFixed(frame.projectile->timer << 8) * frame.projectile->spiralRadius >> 0xC);
    g_BattleLimitDragonTrail[g_BattleLimitDragonTrailWriteIndex].z = frame.projectile->posZ + (CosFixed(frame.projectile->timer << 8) * frame.projectile->spiralRadius >> 0xC);
    g_BattleLimitDragonTrail[g_BattleLimitDragonTrailWriteIndex].y = frame.projectile->posY;
    g_BattleLimitDragonTrailWriteIndex = (g_BattleLimitDragonTrailWriteIndex - 1) & 0xF;
    if (frame.projectile->timer >= 0x18 && frame.projectile->timer < 0x28) {
        g_BattleLimitDragonTrailFade++;
    }
    if (frame.projectile->timer > frame.projectile->arrivalFrame + 0x32) {
        g_BattleLimitDragonTrailFade--;
        if (g_BattleLimitDragonTrailFade == 0) {
            frame.projectile->ownerSlot = BATTLE_TASK_FREE;
        }
    }
    (frame.projectile->timer)++;
}

/*
 * Draws one animated JO sprite at the slot's position until its animation ends; variant 2 rises.
 */
// FUNCTION: FF7 0x004b24f7
void __cdecl BattleFx_LimitDragonVariantSpriteTask(void)
{
    BattleFxLimitDragonVariantSprite *task;

    task = (BattleFxLimitDragonVariantSprite *)BATTLE_CURRENT_TASK_SLOT;
    BattleFx_InstallBillboardMatrix(&task->posX, 0x3000, 0);
    g_BattleLimitDragonVariantSpriteReq.frameIndex = task->frameIndex;
    g_BattleLimitDragonVariantSpriteReq.texture = Gfx_ModelTableGetLoadedRecord(g_BattleLimitDragonVariantSpriteIndices[task->variantIndex], g_BattleLimitDragonLoadedResTable);
    g_BattleLimitDragonVariantSpriteReq.clut = Battle3d_GetSceneRenderObject(1, g_pBattle3dScene);
    g_BattleLimitDragonDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleLimitDragonVariantSpriteReq, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleLimitDragonDrawListHead);
    if (g_BattleIsPaused == 0) {
        if (task->variantIndex == 2) {
            task->posY -= 0x64;
        }
        if (g_BattleLimitDragonVariantSpriteReq.texture != 0) {
            (task->frameIndex)++;
            if (task->frameIndex == *(int *)((char *)*(int *)(g_BattleLimitDragonVariantSpriteReq.texture) + 4)) {
                task->ownerSlot = BATTLE_TASK_FREE;
            }
        }
    }
}

/*
 * Draws the HIKARI model at the target, its scale growing for 16 frames and shrinking after 44;
 * retires at 60.
 */
// FUNCTION: FF7 0x004b25f2
void __cdecl BattleFx_LimitDragonPulsingModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[116];
        struct { unsigned char pad_to_20[0x20]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_40[0x40]; BattleFxLimitDragonPulseRecord *pulseRecord; };
        struct { unsigned char pad_to_44[0x44]; PsxMatrix combinedMatrix; };
        struct { unsigned char pad_to_64[0x64]; VECTOR scaleX; };
    } frame;
#pragma pack(pop)

    frame.pulseRecord = (BattleFxLimitDragonPulseRecord *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleLimitDragonPulseModelReq.modelGroup = Gfx_ModelTableGetRenderObject(0x14, g_BattleLimitDragonLoadedResTable);
    RotMatrixXYZ(&g_BattleLimitDragonPulseAngles, &frame.rotMatrix);
    frame.scaleX.vz = frame.pulseRecord->pulsePhase * 0x50;
    frame.scaleX.vy = frame.scaleX.vz;
    frame.scaleX.vx = frame.scaleX.vy;
    ScaleMatrix(&frame.rotMatrix, &frame.scaleX);
    frame.rotMatrix.t[0] = frame.pulseRecord->posX;
    frame.rotMatrix.t[1] = frame.pulseRecord->posY;
    frame.rotMatrix.t[2] = frame.pulseRecord->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.combinedMatrix);
    SetRotMatrix(&frame.combinedMatrix);
    SetTransMatrix(&frame.combinedMatrix);
    SetDrawColorRGB(0, 0, 0);
    g_BattleLimitDragonDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleLimitDragonPulseModelReq, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleLimitDragonDrawListHead);
    if (g_BattleIsPaused != 0) {
        return;
    }
    (frame.pulseRecord->age)++;
    if (frame.pulseRecord->step < 0x10) {
        frame.pulseRecord->pulsePhase += 8;
    }
    if (frame.pulseRecord->step > 0x2C) {
        frame.pulseRecord->pulsePhase -= 8;
    }
    (frame.pulseRecord->step)++;
    if (frame.pulseRecord->step == 0x3C) {
        frame.pulseRecord->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Draws one BIMU sprite circling the target on the ground, its radius and grey level growing for 16
 * frames and shrinking after 44; retires at 60.
 */
// FUNCTION: FF7 0x004b275d
void __cdecl BattleFx_LimitDragonPulsingRadialSpriteTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[12];
        SVECTOR spriteX;
        struct { unsigned char pad_to_8[0x8]; BattleFxPulsingRadialSprite *spriteRecord; };
    } frame;
#pragma pack(pop)

    frame.spriteRecord = (BattleFxPulsingRadialSprite *)BATTLE_CURRENT_TASK_SLOT;
    frame.spriteX.vx = frame.spriteRecord->posX + (SinFixed(frame.spriteRecord->angle) * frame.spriteRecord->radius << 3 >> 0xC);
    frame.spriteX.vz = frame.spriteRecord->posZ + (CosFixed(frame.spriteRecord->angle) * frame.spriteRecord->radius << 3 >> 0xC);
    frame.spriteX.vy = 0;
    BattleFx_InstallBillboardMatrix(&frame.spriteX, 0x3000, 0);
    g_BattleLimitDragonRadialSpriteReq.frameIndex = ((frame.spriteRecord->cycleIndex) % 4);
    g_BattleLimitDragonRadialSpriteReq.texture = Gfx_ModelTableGetLoadedRecord(g_BattleLimitDragonRadialSpriteIndices[frame.spriteRecord->resIndex], g_BattleLimitDragonLoadedResTable);
    g_BattleLimitDragonRadialSpriteReq.clut = Gfx_ModelTableGetSpriteObject(g_BattleLimitDragonRadialSpriteIndices[frame.spriteRecord->resIndex], g_BattleLimitDragonLoadedResTable);
    g_BattleLimitDragonRadialSpriteReq.colorB = *(char *)&frame.spriteRecord->radius;
    g_BattleLimitDragonRadialSpriteReq.colorG = g_BattleLimitDragonRadialSpriteReq.colorB;
    g_BattleLimitDragonRadialSpriteReq.colorR = g_BattleLimitDragonRadialSpriteReq.colorG;
    g_BattleLimitDragonDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleLimitDragonRadialSpriteReq, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleLimitDragonDrawListHead);
    if (g_BattleIsPaused != 0) {
        return;
    }
    (frame.spriteRecord->cycleIndex)++;
    if (frame.spriteRecord->frameCount < 0x10) {
        frame.spriteRecord->radius += 8;
    }
    if (frame.spriteRecord->frameCount > 0x2C) {
        frame.spriteRecord->radius -= 8;
    }
    (frame.spriteRecord->frameCount)++;
    if (frame.spriteRecord->frameCount == 0x3C) {
        frame.spriteRecord->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Spawns two variant sprites a frame on a ring of radius 400 around its base, higher each frame,
 * for 30 frames.
 */
// FUNCTION: FF7 0x004b2914
void __cdecl BattleFx_LimitDragonVariantSpriteEmitterTask(void)
{
    short angle;
    short i;
    BattleFxLimitDmSpriteEmitter *pEmitter;
    BattleFxLimitDragonVariantSprite *pSprite;

    pEmitter = (BattleFxLimitDmSpriteEmitter *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    for (i = 0; i < 2; i++) {
        angle = rand() % 0x1000;
        pSprite = (BattleFxLimitDragonVariantSprite *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitDragonVariantSpriteTask);
        pSprite->posX = pEmitter->baseX + (SinFixed(angle) * 0x190 >> 0xC);
        pSprite->posY = pEmitter->baseY + pEmitter->frameIndex * 0x1E + rand() % 0x258 + -0x320;
        pSprite->posZ = pEmitter->baseZ + (CosFixed(angle) * 0x190 >> 0xC);
        pSprite->variantIndex = pEmitter->variantIndex;
    }
    (pEmitter->frameIndex)++;
    if (pEmitter->frameIndex == 0x1E) {
        pEmitter->ownerSlot = BATTLE_TASK_FREE;
    }
}
} /* extern "C" */
