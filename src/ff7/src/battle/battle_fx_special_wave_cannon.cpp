// src/ff7/src/battle/battle_fx_special_wave_cannon.cpp
/* Enemy Special Effect [idx 69]: WaveCannon (JP: hadoho / 波動砲 (Wave Cannon), asset: "SPECIAL/HADOHO") — Dragon Cannon (0x32b, XCannon) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle special effect Hadoho: loads its resources, runs a timeline that spawns model
//   callbacks and sounds, and runs the tasks that draw its flash, models, sprites, trail and
//   spinning variant.

// clang-format off
/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/api/battle.h"
#include "ff7/battle_tasks.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_special.h"
#include "lib/types/graphics.h"
#include "lib/types/polygon.h"
#include "ff7/camera_script.h"
#include <stdlib.h>
#include <string.h>
#include "lib/codegen.h"
#include "lib/api/sound.h"
extern "C" {

// clang-format on
// GLOBAL: FF7 0x008306a8
unsigned int g_BattleSpecialWaveCannonResourceDefs[73] = {
    0x001900b1,
    (unsigned int)&g_BattleSpecialWaveCannonResFilenameHado,
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x008308c4
    (unsigned int)"HADO1.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x008308d0
    (unsigned int)"TAMA1.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x008308dc
    (unsigned int)"TAMA2.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x008308e8
    (unsigned int)"MOTO.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x008308f4
    (unsigned int)"SOTO.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x00830900
    (unsigned int)"SENKOU1.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x0083090c
    (unsigned int)"ISI1.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x00830918
    (unsigned int)"ISI2.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x00830924
    (unsigned int)"ISI3.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x00830930
    (unsigned int)"ISI4.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x0083093c
    (unsigned int)"ENERGY00.TIM",
    // STRING: FF7 0x0083094c
    (unsigned int)"KOU.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x00830954
    (unsigned int)"ENERGY03.TIM",
    // STRING: FF7 0x00830964
    (unsigned int)"KEMU11.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x00830970
    (unsigned int)"ENERGY00.TIM",
    // STRING: FF7 0x00830980
    (unsigned int)"HAHEN1.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x0083098c
    (unsigned int)"ENERGY02.TIM",
    // STRING: FF7 0x0083099c
    (unsigned int)"BAKU1.SPT"};
// clang-format off
unsigned char g_rawData_008307d0[24] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x40, 0x40, 0x2e, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x008307e8
struct BattleSpecialWaveCannonScaleTama g_BattleSpecialWaveCannonScaleTama = {0, 0, -128, -128, -128, 44, 0, {0}};
// GLOBAL: FF7 0x008307f8
unsigned char g_BattleSpecialWaveCannonColor1[16] = {0x00, 0x14, 0x00, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x00830808
struct BattleSpecialWaveCannonVector1 g_BattleSpecialWaveCannonVector1 = {0, {0}, 0, {0x20, 0, 0, 0}};
// clang-format on
// GLOBAL: FF7 0x00830818
struct GfxRotationAngles2 g_BattleSpecialWaveCannonRotationMoto = {-1024, 0, 0};
// clang-format off
// GLOBAL: FF7 0x00830820
struct BattleSpecialWaveCannonVector1 g_BattleSpecialWaveCannonParamVector1 = {0, {0xa8, 0, 0, 0, 0, 0}, 0, {0x20, 0, 0, 0}};
// clang-format on
// GLOBAL: FF7 0x00830830
struct GfxRotationAngles2 g_BattleSpecialWaveCannonRotationIsi = {-1024, 0, 0};
// clang-format off
// GLOBAL: FF7 0x00830838
struct BattleSpecialWaveCannonVector1 g_BattleSpecialWaveCannonParamVector2 = {0, {0xa8, 0, 0, 0, 0, 0}, 0, {0x20, 0, 0, 0}};
// GLOBAL: FF7 0x00830848
struct BattleSpecialWaveCannonModelReqMoto g_BattleSpecialWaveCannonModelReqMoto = {0, 0, {0x80, 0x80, 0x80, 0x2e}, 0, {0}};
// clang-format on
// GLOBAL: FF7 0x00830858
struct BattleSpecialWaveCannonModelReqMoto g_BattleSpecialWaveCannonModelReqIsi = {0, 0, {0x80, 0x80, 0x80, 0x2e}, 0x80, {0}};
// clang-format off
// GLOBAL: FF7 0x00830868
struct BattleSpecialWaveCannonModelReqTama g_BattleSpecialWaveCannonModelReqTama = {0, {0xa8, 0, 0, 0, 0, 0}, 128, {0x20, 0, 0, 0}};
// clang-format on
// GLOBAL: FF7 0x00830878
struct GfxRotationAngles2 g_BattleSpecialWaveCannonScaleTrajectory = {-1024, 0, 0};
// clang-format off
// GLOBAL: FF7 0x00830880
unsigned char g_BattleSpecialWaveCannonVectorTrajectory[16] = {0x00, 0x30, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x00830890
struct BattleSpecialWaveCannonModelReqTama g_BattleSpecialWaveCannonRotationTama = {0, {0x28, 0, 0, 0, 0, 0}, 128, {0x20, 0, 0, 0}};
// GLOBAL: FF7 0x008308a0
struct BattleSpecialWaveCannonRotationTrajectory g_BattleSpecialWaveCannonRotationTrajectory = {0, 0, -128, -128, -128, {0x2e}, 0, {0}};
// clang-format on
// GLOBAL: FF7 0x008308b0
struct GfxRotationAngles2 g_BattleSpecialWaveCannonColor2 = {-1024, 0, 0};
// clang-format off
// GLOBAL: FF7 0x008308b8
unsigned char g_BattleSpecialWaveCannonResFilenameHado[9] = "HADO.RSD";
// GLOBAL: FF7 0x00b44f68
PsxMatrix g_BattleSpecialWaveCannonBasisMatrix = {0};
// GLOBAL: FF7 0x00b44f88
struct BattleSpecialScorchCentroidPos g_BattleSpecialWaveCannonCentroidPos = {0};
// GLOBAL: FF7 0x00b44f8c
short g_BattleSpecialWaveCannonCentroidZ = 0;
// GLOBAL: FF7 0x00b44f90
int g_BattleSpecialWaveCannonDrawListHandle = 0;
// GLOBAL: FF7 0x00b44f94
short g_BattleSpecialWaveCannonTargetMask = 0;
// GLOBAL: FF7 0x00b44f98
short g_BattleSpecialWaveCannonFlashIntensity = 0;
// GLOBAL: FF7 0x00b44f9c
short g_BattleSpecialWaveCannonRandomAngle = 0;
// GLOBAL: FF7 0x00b44fa0
short g_BattleSpecialWaveCannonScaleCounter = 0;
// GLOBAL: FF7 0x00b44fa4
short g_BattleSpecialWaveCannonLifetimeCounter = 0;
// GLOBAL: FF7 0x00b44fa8
struct GfxTableCursor *g_BattleSpecialWaveCannonLoadedResTable = 0;
// clang-format on

#pragma function(memcpy)
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

// If the shared renderer exists and the cached effect resource is absent, initializes a
// duration/options record, builds the SPECIAL/HADOHO resource path and creates resource selector
// fifteen using the retained descriptor and renderer field. Existing resources are untouched. The
// options record is rooted at separate scalar requestRecord and the path helper receives scalar
// pathOut.
// FUNCTION: FF7 0x004df8f0
void __cdecl BattleFx_SpecialLoadResWaveCannon(void)
{
    GfxModelTableBank *context;
    GfxResourceRequest requestRecord;
    int kind;
    char pathOut[200];
    GfxResourceRequest *pReq;
    int resultDefEntry;

    if (g_pBattle3dScene != 0 && g_BattleSpecialWaveCannonLoadedResTable == 0) {
        kind = 0xF;
        resultDefEntry = (int)&g_BattleSpecialWaveCannonResourceDefs;
        context = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pReq = &requestRecord;
        Gfx_InitResourceRequest(pReq);
        pReq->groupTag = 0;
        // STRING: FF7 0x008309a8
        pReq->resourcePath = (char *)BattleFx_BuildResourcePath("SPECIAL/HADOHO", pathOut);
        Gfx_LoadModelTableIfAbsent(kind, resultDefEntry, pReq, &g_BattleSpecialWaveCannonLoadedResTable, context);
    }
}

/*
 * Setup wrapper: sets g_BattleSpecialWaveCannonLifetimeCounter and g_BattleSpecialWaveCannonScaleCounter to 8 and g_BattleSpecialWaveCannonFlashIntensity to 0, calls
 * BattleFx_SpecialLoadResWaveCannon, then forwards targetMask and actorIdx to BattleFx_SpecialBeginWaveCannon.
 */
// FUNCTION: FF7 0x004df980
void __cdecl BattleFx_SpecialEnterWaveCannon(int targetMask, int actorIdx)
{
    g_BattleSpecialWaveCannonLifetimeCounter = 8;
    g_BattleSpecialWaveCannonScaleCounter = 8;
    g_BattleSpecialWaveCannonFlashIntensity = 0;
    BattleFx_SpecialLoadResWaveCannon();
    BattleFx_SpecialBeginWaveCannon(targetMask, actorIdx);
}

/*
 * Stores the target mask, creates BattleFx_SpecialWaveCannonTimelineTask at the source actor bone,
 * obtains the target-mask bone centroid, builds its shared look basis and stores normalized
 * velocity scaled by six hundred toward that position. Records the source actor, sets shared flash
 * color black and starts a sixteen-step ramp.
 */
// FUNCTION: FF7 0x004df9b5
void __cdecl BattleFx_SpecialBeginWaveCannon(int targetMask, int actorIdx)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[28];
        VECTOR normX;
        struct { unsigned char pad_to_10[0x10]; BattleFxScorchParticle *particle; };
        struct { unsigned char pad_to_14[0x14]; SVECTOR dirX; };
    } frame;
#pragma pack(pop)

    g_BattleSpecialWaveCannonTargetMask = targetMask;
    frame.particle = (BattleFxScorchParticle *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialWaveCannonTimelineTask);
    Battle_GetBoneWorldPos(actorIdx, g_BattleActorRecords[actorIdx].bone33, (short *)&frame.particle->posX);
    Battle_GetTargetMaskBoneCentroid((short)targetMask, (Vec3sPacked *)&g_BattleSpecialWaveCannonCentroidPos);
    frame.dirX.vx = (g_BattleSpecialWaveCannonCentroidPos.centroidX) - frame.particle->posX;
    frame.normX.vx = frame.dirX.vx;
    frame.dirX.vy = g_BattleSpecialWaveCannonCentroidPos.y - frame.particle->posY;
    frame.normX.vy = frame.dirX.vy;
    frame.dirX.vz = g_BattleSpecialWaveCannonCentroidZ - frame.particle->posZ;
    frame.normX.vz = frame.dirX.vz;
    Gte_BuildLookBasisFromDirection(&frame.dirX.vx, (short *)&g_BattleSpecialWaveCannonBasisMatrix);
    NormalizeVectorFixed(&frame.normX, &frame.normX);
    frame.particle->velocityX = frame.normX.vx * 0x258 >> 12;
    frame.particle->velocityY = frame.normX.vy * 0x258 >> 12;
    frame.particle->velocityZ = frame.normX.vz * 0x258 >> 12;
    frame.particle->actorIdx = actorIdx;
    g_BattleFxTimelineState.channel2 = 0;
    g_BattleFxTimelineState.channel1 = g_BattleFxTimelineState.channel2;
    g_BattleFxTimelineState.red = g_BattleFxTimelineState.channel1;
    Battle_StartFixedPointRamp(0x1000, 16);
}

/*
 * When unpaused at zero, creates two model callbacks at the task position and dispatches opcode
 * 0x174. At twenty creates two more callbacks, one inheriting task velocity, starts another
 * callback and dispatches opcode 0x176. During counts 37 through 43 emits rotated-velocity
 * secondary callbacks for target-mask actors; creates their typed records and dispatches opcode
 * 0xFA at forty. Starts a fade at 54, alternates a shared short after twenty, advances and retires
 * at 75.
 */
// FUNCTION: FF7 0x004dfafd
void __cdecl BattleFx_SpecialWaveCannonTimelineTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[72];
        SVECTOR rotX;
        struct { unsigned char pad_to_8[0x8]; int depth; };
        struct { unsigned char pad_to_C[0xC]; SVECTOR pointX; };
        struct { unsigned char pad_to_14[0x14]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_34[0x34]; BattleFxSpecialEnergyOrbSlot *newSlot; };
        struct { unsigned char pad_to_3C[0x3C]; short actorIndex; };
        struct { unsigned char pad_to_44[0x44]; BattleFxScorchTimeline *timeline; };
    } frame;
#pragma pack(pop)

    frame.timeline = (BattleFxScorchTimeline *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.timeline->timer == 0) {
        frame.newSlot = (BattleFxSpecialEnergyOrbSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialWaveCannonGrowingModelTask);
        frame.newSlot->posX = frame.timeline->posX;
        frame.newSlot->posY = frame.timeline->posY;
        frame.newSlot->posZ = frame.timeline->posZ;
        frame.newSlot = (BattleFxSpecialEnergyOrbSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialWaveCannonPulsingSpriteTask);
        frame.newSlot->posX = frame.timeline->posX;
        frame.newSlot->posY = frame.timeline->posY;
        frame.newSlot->posZ = frame.timeline->posZ;
        Battle_PlaySfxPannedToActor(0x174, (short)frame.timeline->level, 0x40);
    }
    if (frame.timeline->timer == 20) {
        frame.newSlot = (BattleFxSpecialEnergyOrbSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialWaveCannonStretchingModelsTask);
        frame.newSlot->posX = frame.timeline->posX;
        frame.newSlot->posY = frame.timeline->posY;
        frame.newSlot->posZ = frame.timeline->posZ;
        frame.newSlot = (BattleFxSpecialEnergyOrbSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialWaveCannonProjectileTask);
        frame.newSlot->posX = frame.timeline->posX;
        frame.newSlot->posY = frame.timeline->posY;
        frame.newSlot->posZ = frame.timeline->posZ;
        frame.newSlot->velocityX = frame.timeline->velocityX;
        frame.newSlot->velocityY = frame.timeline->velocityY;
        frame.newSlot->velocityZ = frame.timeline->velocityZ;
        Battle_PlaySfxPannedToActor(0x176, (short)frame.timeline->level, 0x40);
    }
    if (frame.timeline->timer == 20) {
        Battle_AllocateResourceSlot(BattleFx_SpecialWaveCannonScreenFlashTask);
    }
    if (frame.timeline->timer == 54) {
        Battle_StartFixedPointRamp(0, 16);
    }
    if (frame.timeline->timer > 36 && frame.timeline->timer < 44) {
        for (frame.actorIndex = 0; frame.actorIndex < 10; frame.actorIndex++) {
            if ((g_BattleSpecialWaveCannonTargetMask & 1 << frame.actorIndex) != 0) {
                frame.newSlot = (BattleFxSpecialEnergyOrbSlot *)&g_BattleTaskSlotBuffer.secondarySlots[(short)Battle_AllocateSecondaryResourceSlot(BattleFx_SpecialWaveCannonDriftingAnimatedSpriteTask)];
                Battle_GetBoneWorldPos(frame.actorIndex, g_BattleActorRecords[frame.actorIndex].targetBone, (short *)((int)frame.newSlot + 4));
                frame.pointX.vy = 80;
                frame.pointX.vz = 0;
                frame.pointX.vx = frame.pointX.vz;
                frame.rotX.vx = (rand() % 4096);
                frame.rotX.vz = (rand() % 4096);
                frame.rotX.vy = 0;
                RotMatrix(&frame.rotX, &frame.rotMatrix);
                frame.rotMatrix.t[2] = 0;
                frame.rotMatrix.t[1] = frame.rotMatrix.t[2];
                frame.rotMatrix.t[0] = frame.rotMatrix.t[1];
                SetRotMatrix(&frame.rotMatrix);
                SetTransMatrix(&frame.rotMatrix);
                RotTransPointToInt16(&frame.pointX, (short *)((int)frame.newSlot + 0xC), &frame.depth);
            }
        }
    }
    if (frame.timeline->timer == 40) {
        for (frame.actorIndex = 0; frame.actorIndex < 10; frame.actorIndex++) {
            if ((g_BattleSpecialWaveCannonTargetMask & 1 << frame.actorIndex) != 0) {
                Battle_PostActorHitOrHurtMessage(frame.actorIndex);
            }
        }
        Battle_PlaySfxPannedToActor(0xFA, -1, 0x40);
    }
    if (frame.timeline->timer > 20) {
        g_BattleCameraFocalPoint.focalPointY += (short)(16 - ((frame.timeline->timer % 2) << 5));
    }
    (frame.timeline->timer)++;
    if (frame.timeline->timer == 75) {
        frame.timeline->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Draws a screen flash with red from g_BattleSpecialWaveCannonFlashIntensity, green from its signed right shift by one and
 * blue zero before checking pause. When unpaused increases the shared channel by fifteen below
 * counter sixteen and reduces it by fifteen above counter thirty-four without clamping, advances
 * the counter and retires exactly at fifty.
 */
// FUNCTION: FF7 0x004dfee6
void __cdecl BattleFx_SpecialWaveCannonScreenFlashTask(void)
{
    BattleTaskSlot *task;

    task = BATTLE_CURRENT_TASK_SLOT;
    g_BattleOverlayDrawListHead = BattleFx_FlashScreenWhite(*(char *)&g_BattleSpecialWaveCannonFlashIntensity, g_BattleSpecialWaveCannonFlashIntensity >> 1, 0);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (task->frameCounter < 16) {
        g_BattleSpecialWaveCannonFlashIntensity += 0xF;
    }
    if (task->frameCounter > 34) {
        g_BattleSpecialWaveCannonFlashIntensity -= 0xF;
    }
    (task->frameCounter)++;
    if (task->frameCounter == 50) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Draws model entry six at the task position with shared orientation concatenation, uniform scale
 * count times 1500 and a count-shifted descriptor field using black draw color. Rendering
 * continues while paused; unpaused calls increment the counter and retire at twenty.
 */
// FUNCTION: FF7 0x004dff8b
void __cdecl BattleFx_SpecialWaveCannonGrowingModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[84];
        PsxMatrix rotMatrix;
        struct { unsigned char pad_to_20[0x20]; BattleFxCosmoMemoryTriSpin *task; };
        struct { unsigned char pad_to_24[0x24]; PsxMatrix drawMatrix; };
        struct { unsigned char pad_to_44[0x44]; VECTOR scaleX; };
    } frame;
#pragma pack(pop)

    frame.task = (BattleFxCosmoMemoryTriSpin *)BATTLE_CURRENT_TASK_SLOT;
    (g_BattleSpecialWaveCannonParamVector1.modelResField) = Gfx_ModelTableGetRenderObject(6, g_BattleSpecialWaveCannonLoadedResTable);
    RotMatrix((SVECTOR *)&g_BattleSpecialWaveCannonRotationMoto, &frame.rotMatrix);
    ConcatMatrixPreservingTranslation(&g_BattleSpecialWaveCannonBasisMatrix, &frame.rotMatrix, &frame.rotMatrix);
    frame.scaleX.vz = frame.task->step * 0x5DC;
    frame.scaleX.vy = frame.scaleX.vz;
    frame.scaleX.vx = frame.scaleX.vy;
    ScaleMatrix(&frame.rotMatrix, &frame.scaleX);
    frame.rotMatrix.t[0] = frame.task->posX;
    frame.rotMatrix.t[1] = frame.task->posY;
    frame.rotMatrix.t[2] = frame.task->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.drawMatrix);
    SetRotMatrix(&frame.drawMatrix);
    SetTransMatrix(&frame.drawMatrix);
    SetDrawColorRGB(0, 0, 0);
    g_BattleSpecialWaveCannonParamVector1.alpha = frame.task->step << 8;
    g_BattleSpecialWaveCannonDrawListHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecialWaveCannonParamVector1, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialWaveCannonDrawListHandle);
    if (g_BattleIsPaused != 0) {
        return;
    }
    (frame.task->step)++;
    if (frame.task->step == 20) {
        frame.task->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Another copy of the BattleFx_SpecialSuperOrbDriftingAnimatedSpriteTask moving-sprite tick, on
 * request g_BattleSpecialWaveCannonModelReqMoto (resource, field4 and short12 filled from entry 0xE of
 * g_BattleSpecialWaveCannonLoadedResTable) with billboard scale 0x4000. The slot's velocity is added to
 * its position each unpaused frame, and the slot is freed once its frame index reaches the
 * resource's frame count. Counter advancement and retirement additionally require the resource to
 * be nonnull.
 */
// FUNCTION: FF7 0x004e00d1
void __cdecl BattleFx_SpecialWaveCannonDriftingAnimatedSpriteTask(void)
{
    BattleFxEnergyOrbDriftSprite *task;

    task = (BattleFxEnergyOrbDriftSprite *)&g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx];
    (g_BattleSpecialWaveCannonModelReqMoto.resource) = Gfx_ModelTableGetLoadedRecord(14, g_BattleSpecialWaveCannonLoadedResTable);
    g_BattleSpecialWaveCannonModelReqMoto.drawObj = Gfx_ModelTableGetSpriteObject(14, g_BattleSpecialWaveCannonLoadedResTable);
    BattleFx_InstallBillboardMatrix(&task->posX, 0x4000 /* 4.0 */, 0);
    g_BattleSpecialWaveCannonModelReqMoto.frame = task->frameIndex;
    g_BattleOverlayDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecialWaveCannonModelReqMoto, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused == 0) {
        task->posX += task->velocityX;
        task->posY += task->velocityY;
        task->posZ += task->velocityZ;
        if ((g_BattleSpecialWaveCannonModelReqMoto.resource) != 0) {
            (task->frameIndex)++;
            if (task->frameIndex >= *(int *)((char *)*(int *)(g_BattleSpecialWaveCannonModelReqMoto.resource) + 4)) {
                task->ownerSlot = BATTLE_TASK_FREE;
            }
        }
    }
}

/*
 * Per-frame handler for the current task slot. Loads sprite table entry 0xB of
 * g_BattleSpecialWaveCannonLoadedResTable into g_BattleSpecialWaveCannonModelReqIsi (resource and field4),
 * scales the billboard by SinFixed(frame * 0x33) << 7 >> 12 (kept as a short) as ((s << 7) + 0x1000)
 * through BattleFx_InstallBillboardMatrix on the slot position, writes frame % 2 into the request's
 * short12, and submits the sprite into g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED at
 * shift 0xC threading g_BattleSpecialWaveCannonDrawListHandle. When g_BattleIsPaused is clear and the
 * resource is non-null it advances the slot's frame counter and frees the slot once the counter
 * reaches the resource's frame count.
 */
// FUNCTION: FF7 0x004e01e1
void __cdecl BattleFx_SpecialWaveCannonPulsingSpriteTask(void)
{
    BattleTaskSlot *task;
    short wobble;

    task = BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecialWaveCannonModelReqIsi.resource = Gfx_ModelTableGetLoadedRecord(11, g_BattleSpecialWaveCannonLoadedResTable);
    g_BattleSpecialWaveCannonModelReqIsi.drawObj = Gfx_ModelTableGetSpriteObject(11, g_BattleSpecialWaveCannonLoadedResTable);
    wobble = SinFixed(task->frameCounter * 51) << 7 >> 12;
    BattleFx_InstallBillboardMatrix(task->data, (wobble << 7) + 0x1000, 0);
    g_BattleSpecialWaveCannonModelReqIsi.frame = task->frameCounter % 2;
    g_BattleSpecialWaveCannonDrawListHandle = BattleFx_SubmitSpriteDraw(&g_BattleSpecialWaveCannonModelReqIsi, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialWaveCannonDrawListHandle);
    if (g_BattleIsPaused == 0 && g_BattleSpecialWaveCannonModelReqIsi.resource != 0) {
        (task->frameCounter)++;
        if (task->frameCounter >= *(int *)((char *)*(int *)g_BattleSpecialWaveCannonModelReqIsi.resource + 4)) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Draws model entries zero and five at the task position with shared orientation, shrinking X/Z
 * scale, growing Y scale and a descriptor field derived from shared intensity. Rendering continues
 * while paused. Unpaused calls lower shared intensity after twelve, increment the counter and
 * retire at twenty.
 */
// FUNCTION: FF7 0x004e02e5
void __cdecl BattleFx_SpecialWaveCannonStretchingModelsTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[84];
        PsxMatrix rotMatrix;
        struct { unsigned char pad_to_20[0x20]; BattleFxEnergyOrbTrailModel *task; };
        struct { unsigned char pad_to_24[0x24]; PsxMatrix drawMatrix; };
        struct { unsigned char pad_to_44[0x44]; VECTOR scaleX; };
    } frame;
#pragma pack(pop)

    frame.task = (BattleFxEnergyOrbTrailModel *)BATTLE_CURRENT_TASK_SLOT;
    RotMatrix((SVECTOR *)&g_BattleSpecialWaveCannonScaleTrajectory, &frame.rotMatrix);
    ConcatMatrixPreservingTranslation(&g_BattleSpecialWaveCannonBasisMatrix, &frame.rotMatrix, &frame.rotMatrix);
    frame.scaleX.vz = 0x4000 - frame.task->frameCount * 0x12C;
    frame.scaleX.vx = frame.scaleX.vz;
    frame.scaleX.vy = frame.task->frameCount * 0x384;
    ScaleMatrix(&frame.rotMatrix, &frame.scaleX);
    frame.rotMatrix.t[0] = frame.task->posX;
    frame.rotMatrix.t[1] = frame.task->posY;
    frame.rotMatrix.t[2] = frame.task->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.drawMatrix);
    SetRotMatrix(&frame.drawMatrix);
    SetTransMatrix(&frame.drawMatrix);
    SetDrawColorRGB(0, 0, 0);
    g_BattleSpecialWaveCannonModelReqTama.alpha = 0x1000 - (g_BattleSpecialWaveCannonLifetimeCounter << 8);
    (g_BattleSpecialWaveCannonModelReqTama.model) = Gfx_ModelTableGetRenderObject(0, g_BattleSpecialWaveCannonLoadedResTable);
    g_BattleSpecialWaveCannonDrawListHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecialWaveCannonModelReqTama, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialWaveCannonDrawListHandle);
    (g_BattleSpecialWaveCannonModelReqTama.model) = Gfx_ModelTableGetRenderObject(5, g_BattleSpecialWaveCannonLoadedResTable);
    g_BattleSpecialWaveCannonDrawListHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecialWaveCannonModelReqTama, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialWaveCannonDrawListHandle);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.task->frameCount > 12) {
        g_BattleSpecialWaveCannonLifetimeCounter--;
    }
    (frame.task->frameCount)++;
    if (frame.task->frameCount == 20) {
        frame.task->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Builds and installs the current slot rotation, translation and scale, draws three table-selected
 * models, then installs a billboard matrix and draws a sprite with age-parity selection and shared
 * intensity. Drawing occurs before the pause check. When unpaused, advances short XYZ by velocity,
 * spawns a copy on even ages, and, below Y zero, spawns a randomized moving effect plus another
 * randomly offset effect every third age. Reduces shared intensity after age 12, increments age,
 * and retires the slot exactly at age 20. Resource allocations are used without failure checks.
 */
// FUNCTION: FF7 0x004e0498
void __cdecl BattleFx_SpecialWaveCannonProjectileTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[76];
        struct { unsigned char pad_to_4[0x4]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_24[0x24]; BattleFxEnergyOrbProjectile *newSlot; };
        struct { unsigned char pad_to_28[0x28]; BattleFxEnergyOrbProjectile *projectile; };
        struct { unsigned char pad_to_2C[0x2C]; PsxMatrix drawMatrix; };
    } frame;
#pragma pack(pop)

    frame.projectile = (BattleFxEnergyOrbProjectile *)BATTLE_CURRENT_TASK_SLOT;
    (g_BattleSpecialWaveCannonRotationTrajectory.spriteResField) = Gfx_ModelTableGetLoadedRecord(11, g_BattleSpecialWaveCannonLoadedResTable);
    g_BattleSpecialWaveCannonRotationTrajectory.drawObj = Gfx_ModelTableGetSpriteObject(11, g_BattleSpecialWaveCannonLoadedResTable);
    RotMatrix((SVECTOR *)&g_BattleSpecialWaveCannonColor2, &frame.rotMatrix);
    ConcatMatrixPreservingTranslation(&g_BattleSpecialWaveCannonBasisMatrix, &frame.rotMatrix, &frame.rotMatrix);
    ScaleMatrix(&frame.rotMatrix, (int *)&g_BattleSpecialWaveCannonVectorTrajectory);
    frame.rotMatrix.t[0] = frame.projectile->posX;
    frame.rotMatrix.t[1] = frame.projectile->posY;
    frame.rotMatrix.t[2] = frame.projectile->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.drawMatrix);
    SetRotMatrix(&frame.drawMatrix);
    SetTransMatrix(&frame.drawMatrix);
    g_BattleSpecialWaveCannonRotationTama.alpha = g_BattleSpecialWaveCannonScaleCounter << 4;
    (g_BattleSpecialWaveCannonRotationTama.model) = Gfx_ModelTableGetRenderObject(2, g_BattleSpecialWaveCannonLoadedResTable);
    g_BattleSpecialWaveCannonDrawListHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecialWaveCannonRotationTama, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialWaveCannonDrawListHandle);
    (g_BattleSpecialWaveCannonRotationTama.model) = Gfx_ModelTableGetRenderObject(3, g_BattleSpecialWaveCannonLoadedResTable);
    g_BattleSpecialWaveCannonDrawListHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecialWaveCannonRotationTama, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialWaveCannonDrawListHandle);
    (g_BattleSpecialWaveCannonRotationTama.model) = Gfx_ModelTableGetRenderObject(4, g_BattleSpecialWaveCannonLoadedResTable);
    g_BattleSpecialWaveCannonDrawListHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecialWaveCannonRotationTama, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialWaveCannonDrawListHandle);
    BattleFx_InstallBillboardMatrix(&frame.projectile->posX, 0x4000 /* 4.0 */, 0);
    g_BattleSpecialWaveCannonRotationTrajectory.trajectoryByte10 = g_BattleSpecialWaveCannonScaleCounter * 20;
    g_BattleSpecialWaveCannonRotationTrajectory.trajectoryByte9 = g_BattleSpecialWaveCannonRotationTrajectory.trajectoryByte10;
    g_BattleSpecialWaveCannonRotationTrajectory.trajectoryByte8 = g_BattleSpecialWaveCannonRotationTrajectory.trajectoryByte9;
    g_BattleSpecialWaveCannonRotationTrajectory.trajectoryShort12 = (frame.projectile->frameCount % 2);
    g_BattleSpecialWaveCannonDrawListHandle = BattleFx_SubmitSpriteDraw(&g_BattleSpecialWaveCannonRotationTrajectory, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialWaveCannonDrawListHandle);
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.projectile->posX += frame.projectile->velocityX;
    frame.projectile->posY += frame.projectile->velocityY;
    frame.projectile->posZ += frame.projectile->velocityZ;
    if ((frame.projectile->frameCount % 2) == 0) {
        frame.newSlot = (BattleFxEnergyOrbProjectile *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialWaveCannonTrailModelTask);
        frame.newSlot->posX = frame.projectile->posX;
        frame.newSlot->posY = frame.projectile->posY;
        frame.newSlot->posZ = frame.projectile->posZ;
    }
    if (frame.projectile->posY < 0) {
        frame.newSlot = (BattleFxEnergyOrbProjectile *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialWaveCannonBouncingSpriteTask);
        frame.newSlot->posX = frame.projectile->posX;
        frame.newSlot->posZ = frame.projectile->posZ;
        frame.newSlot->posY = 0;
        g_BattleSpecialWaveCannonRandomAngle = (rand() % 4096);
        frame.newSlot->velocityX = CosFixed(g_BattleSpecialWaveCannonRandomAngle) * 0x78 >> 12;
        frame.newSlot->velocityZ = SinFixed(g_BattleSpecialWaveCannonRandomAngle) * 0x78 >> 12;
        frame.newSlot->velocityY = -rand() % 300 - 50;
        frame.newSlot->variant = rand() % 7;
        if (frame.projectile->frameCount % 3 == 0) {
            frame.newSlot = (BattleFxEnergyOrbProjectile *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialWaveCannonVariantSpinModelTask);
            frame.newSlot->posX = frame.projectile->posX + rand() % 4000 + -2000;
            frame.newSlot->posZ = frame.projectile->posZ + rand() % 4000 + -2000;
            frame.newSlot->posY = 0;
            frame.newSlot->variant = (rand() % 4);
        }
    }
    if (frame.projectile->frameCount > 12) {
        g_BattleSpecialWaveCannonScaleCounter--;
    }
    (frame.projectile->frameCount)++;
    if (frame.projectile->frameCount == 20) {
        frame.projectile->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Draws a rotated sprite at the current slot position using table entry 13, an eight-step
 * intensity until age 12 and then intensity 20 minus age. Uses the slot variant and angle,
 * installs the composed matrix and submits before checking pause. When unpaused, advances XYZ by
 * velocity, clamps positive Y to zero and reverses vertical velocity, then adds gravity 20 and
 * angle 0x200. Increments age and retires exactly at 20.
 */
// FUNCTION: FF7 0x004e08f3
void __cdecl BattleFx_SpecialWaveCannonBouncingSpriteTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[92];
        int intensity;
        struct { unsigned char pad_to_4[0x4]; SVECTOR rotX; };
        struct { unsigned char pad_to_C[0xC]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_2C[0x2C]; short intensityShort; };
        struct { unsigned char pad_to_30[0x30]; PsxMatrix spriteMatrix; };
        struct { unsigned char pad_to_50[0x50]; short axisIndex; };
        struct { unsigned char pad_to_54[0x54]; int depth; };
        struct { unsigned char pad_to_58[0x58]; BattleFxEnergyOrbBouncingSprite *sprite; };
    } frame;
#pragma pack(pop)

    frame.sprite = (BattleFxEnergyOrbBouncingSprite *)BATTLE_CURRENT_TASK_SLOT;
    (g_BattleSpecialWaveCannonScaleTama.spriteResField) = Gfx_ModelTableGetLoadedRecord(13, g_BattleSpecialWaveCannonLoadedResTable);
    g_BattleSpecialWaveCannonScaleTama.drawObj = Gfx_ModelTableGetSpriteObject(13, g_BattleSpecialWaveCannonLoadedResTable);
    for (frame.axisIndex = 0; frame.axisIndex < 3; frame.axisIndex++) {
        frame.spriteMatrix.m[2][frame.axisIndex] = 0;
        frame.spriteMatrix.m[1][frame.axisIndex] = 0;
        frame.spriteMatrix.m[0][frame.axisIndex] = 0;
    }
    if (frame.sprite->frameCount < 12) {
        frame.intensity = 8;
    } else {
        frame.intensity = 0x14 - frame.sprite->frameCount;
    }
    frame.intensityShort = *(short *)&frame.intensity;
    frame.rotX.vy = 0;
    frame.rotX.vx = frame.rotX.vy;
    frame.rotX.vz = frame.sprite->angle;
    SetRotMatrix(&g_BattleCameraTransform);
    SetTransMatrix(&g_BattleCameraTransform);
    RotTransPoint((SVECTOR *)&frame.sprite->posA, frame.spriteMatrix.t, &frame.depth);
    frame.spriteMatrix.m[2][2] = 0x2000;
    frame.spriteMatrix.m[1][1] = frame.spriteMatrix.m[2][2];
    frame.spriteMatrix.m[0][0] = frame.spriteMatrix.m[1][1];
    RotMatrix(&frame.rotX, &frame.rotMatrix);
    ConcatMatrixIntoFirst(&frame.spriteMatrix, &frame.rotMatrix);
    SetRotMatrix(&frame.spriteMatrix);
    SetTransMatrix(&frame.spriteMatrix);
    g_BattleSpecialWaveCannonScaleTama.frame = frame.sprite->variant;
    g_BattleSpecialWaveCannonScaleTama.code = (frame.intensityShort - 8 != 0 ? 2 : 0) + 0x2C;
    g_BattleSpecialWaveCannonScaleTama.colorB = frame.intensityShort << 4;
    g_BattleSpecialWaveCannonScaleTama.colorG = g_BattleSpecialWaveCannonScaleTama.colorB;
    g_BattleSpecialWaveCannonScaleTama.colorR = g_BattleSpecialWaveCannonScaleTama.colorG;
    g_BattleOverlayDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecialWaveCannonScaleTama, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.sprite->posA += frame.sprite->velocityA;
    frame.sprite->posB += frame.sprite->velocityB;
    frame.sprite->posC += frame.sprite->velocityC;
    if (frame.sprite->posB > 0) {
        frame.sprite->posB = 0;
        frame.sprite->velocityB *= -1;
    }
    frame.sprite->velocityB += 20;
    frame.sprite->angle += 0x200 /* 45 deg */;
    (frame.sprite->frameCount)++;
    if (frame.sprite->frameCount == 20) {
        frame.sprite->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Builds a scaled XYZ rotation from the slot angle and position, installs its camera composition,
 * selects one of table entries 7 through 10 using the slot variant, and draws with intensity 128
 * minus six times age. Drawing precedes pause. When unpaused, subtracts 200 from Y, advances angle
 * by 0x100, increments age and retires exactly at 20.
 */
// FUNCTION: FF7 0x004e0b6d
void __cdecl BattleFx_SpecialWaveCannonVariantSpinModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[92];
        SVECTOR rotX;
        struct { unsigned char pad_to_8[0x8]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_28[0x28]; int variantModelA[4]; };
        struct { unsigned char pad_to_38[0x38]; BattleFxWaveCannonSpinRecord *spinRecord; };
        struct { unsigned char pad_to_3C[0x3C]; PsxMatrix drawMatrix; };
    } frame;
#pragma pack(pop)

    frame.spinRecord = (BattleFxWaveCannonSpinRecord *)BATTLE_CURRENT_TASK_SLOT;
    frame.variantModelA[0] = 7;
    frame.variantModelA[1] = 8;
    frame.variantModelA[2] = 9;
    frame.variantModelA[3] = 0xA;
    frame.rotX.vy = 0;
    frame.rotX.vx = frame.rotX.vy;
    frame.rotX.vz = frame.spinRecord->angleZ;
    RotMatrixXYZ(&frame.rotX, &frame.rotMatrix);
    ScaleMatrix(&frame.rotMatrix, (int *)&g_BattleSpecialWaveCannonColor1);
    frame.rotMatrix.t[0] = frame.spinRecord->posX;
    frame.rotMatrix.t[1] = frame.spinRecord->posY;
    frame.rotMatrix.t[2] = frame.spinRecord->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.drawMatrix);
    SetRotMatrix(&frame.drawMatrix);
    SetTransMatrix(&frame.drawMatrix);
    g_BattleSpecialWaveCannonVector1.alpha = 0x80 - frame.spinRecord->frame * 6;
    (g_BattleSpecialWaveCannonVector1.modelResField) = Gfx_ModelTableGetRenderObject(frame.variantModelA[frame.spinRecord->variant], g_BattleSpecialWaveCannonLoadedResTable);
    g_BattleSpecialWaveCannonDrawListHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecialWaveCannonVector1, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialWaveCannonDrawListHandle);
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.spinRecord->posY -= 0xC8;
    frame.spinRecord->angleZ += 0x100 /* 22.5 deg */;
    (frame.spinRecord->frame)++;
    if (frame.spinRecord->frame == 20) {
        frame.spinRecord->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Selects table model 5 and, only after age 2, composes its rotation and slot translation with
 * uniform scale 0x4000 minus age times 400, installs the matrix and submits a black model with
 * age-scaled draw attribute. Drawing precedes pause. When unpaused, increments age and retires
 * exactly at 20.
 */
// FUNCTION: FF7 0x004e0ce0
void __cdecl BattleFx_SpecialWaveCannonTrailModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[84];
        PsxMatrix rotMatrix;
        struct { unsigned char pad_to_20[0x20]; BattleFxEnergyOrbTrailModel *trailModel; };
        struct { unsigned char pad_to_24[0x24]; PsxMatrix drawMatrix; };
        struct { unsigned char pad_to_44[0x44]; VECTOR scaleX; };
    } frame;
#pragma pack(pop)

    frame.trailModel = (BattleFxEnergyOrbTrailModel *)BATTLE_CURRENT_TASK_SLOT;
    (g_BattleSpecialWaveCannonParamVector2.modelResField) = Gfx_ModelTableGetRenderObject(5, g_BattleSpecialWaveCannonLoadedResTable);
    if (frame.trailModel->frameCount > 2) {
        RotMatrix((SVECTOR *)&g_BattleSpecialWaveCannonRotationIsi, &frame.rotMatrix);
        ConcatMatrixPreservingTranslation(&g_BattleSpecialWaveCannonBasisMatrix, &frame.rotMatrix, &frame.rotMatrix);
        frame.scaleX.vz = 0x4000 - frame.trailModel->frameCount * 0x190;
        frame.scaleX.vy = frame.scaleX.vz;
        frame.scaleX.vx = frame.scaleX.vy;
        ScaleMatrix(&frame.rotMatrix, &frame.scaleX);
        frame.rotMatrix.t[0] = frame.trailModel->posX;
        frame.rotMatrix.t[1] = frame.trailModel->posY;
        frame.rotMatrix.t[2] = frame.trailModel->posZ;
        MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.drawMatrix);
        SetRotMatrix(&frame.drawMatrix);
        SetTransMatrix(&frame.drawMatrix);
        SetDrawColorRGB(0, 0, 0);
        g_BattleSpecialWaveCannonParamVector2.alpha = frame.trailModel->frameCount * 200;
        g_BattleSpecialWaveCannonDrawListHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecialWaveCannonParamVector2, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialWaveCannonDrawListHandle);
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    (frame.trailModel->frameCount)++;
    if (frame.trailModel->frameCount == 20) {
        frame.trailModel->ownerSlot = BATTLE_TASK_FREE;
    }
}
} /* extern "C" */
