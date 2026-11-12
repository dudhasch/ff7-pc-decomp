// src/ff7/src/battle/battle_fx_special_energy_orb.cpp
/* Enemy Special Attack [110]: Energy Orb (orig. SPECIAL/RE_DO) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle special effect ReDo: resets its shared intensity fields, runs a timeline that
//   orients the effect toward the actor, and runs the tasks that draw its flash, models, sprites
//   and trail.

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
#include <stdlib.h>
#include <string.h>
#include "lib/codegen.h"
#include "lib/api/sound.h"
#include "ff7/types/battle_special.h"
extern "C" {

// clang-format on

#pragma function(memcpy)
// clang-format off
// GLOBAL: FF7 0x00830388
unsigned int g_BattleSpecialEnergyOrbResourceDefs[73] ={
    0x001900b1,
    (unsigned int)&g_BattleSpecialEnergyOrbResFilenameHado,
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x008305b4
    (unsigned int)"hado1.rsd",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x008305c0
    (unsigned int)"tama1.rsd",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x008305cc
    (unsigned int)"tama2.rsd",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x008305d8
    (unsigned int)"moto.rsd",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x008305e4
    (unsigned int)"soto.rsd",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x008305f0
    (unsigned int)"senkou1.rsd",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b4,
    // STRING: FF7 0x008305fc
    (unsigned int)"isi1.rsd",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b4,
    // STRING: FF7 0x00830608
    (unsigned int)"isi2.rsd",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b4,
    // STRING: FF7 0x00830614
    (unsigned int)"isi3.rsd",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b4,
    // STRING: FF7 0x00830620
    (unsigned int)"isi4.rsd",
    0x00000000,
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x0083062c
    (unsigned int)"energy00.tim",
    // STRING: FF7 0x0083063c
    (unsigned int)"kou.spt",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x00830644
    (unsigned int)"energy03.tim",
    // STRING: FF7 0x00830654
    (unsigned int)"kemu11.spt",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x00830660
    (unsigned int)"energy00.tim",
    // STRING: FF7 0x00830670
    (unsigned int)"hahen1.spt",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x0083067c
    (unsigned int)"energy02.tim",
    // STRING: FF7 0x0083068c
    (unsigned int)"baku1.spt"
};
unsigned char g_rawData_008304b0[24] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x40, 0x40, 0x2e, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x008304c8
struct BattleSpecialEnergyOrbScaleTama g_BattleSpecialEnergyOrbScaleTama = {0, 0, -128, -128, -128, 44, 0};
// GLOBAL: FF7 0x008304d8
unsigned char g_BattleSpecialEnergyOrbRotationTama[12] = {0x00, 0x30, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00};
// GLOBAL: FF7 0x008304e8
struct BattleSpecialEnergyOrbColor1 g_BattleSpecialEnergyOrbVector1 = {0, {0}, 0, {0x20, 0, 0, 0}};
// GLOBAL: FF7 0x008304f8
unsigned char g_BattleSpecialEnergyOrbModelConfig[16] = {0x07, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x0a, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x00830508
struct GfxRotationAngles2 g_BattleSpecialEnergyOrbRotationMoto = {-1024, 0, 0};
// GLOBAL: FF7 0x00830510
struct BattleSpecialEnergyOrbColor1 g_BattleSpecialEnergyOrbParamVector1 = {0, {0xa8, 0, 0, 0, 0, 0}, 0, {0x20, 0, 0, 0}};
// GLOBAL: FF7 0x00830520
struct GfxRotationAngles2 g_BattleSpecialEnergyOrbRotationIsi = {-1024, 0, 0};
// GLOBAL: FF7 0x00830528
struct BattleSpecialEnergyOrbColor1 g_BattleSpecialEnergyOrbParamVector2 = {0, {0xa8, 0, 0, 0, 0, 0}, 0, {0x20, 0, 0, 0}};
// GLOBAL: FF7 0x00830538
struct BattleSpecialEnergyOrbModelReqIsi g_BattleSpecialEnergyOrbModelReqMoto = {0, 0, {0x80, 0x80, 0x80, 0x2e}, 0};
// GLOBAL: FF7 0x00830548
struct BattleSpecialEnergyOrbModelReqIsi g_BattleSpecialEnergyOrbModelReqIsi = {0, 0, {0x80, 0x80, 0x80, 0x2e}, 128};
// GLOBAL: FF7 0x00830558
struct BattleSpecialEnergyOrbColor1 g_BattleSpecialEnergyOrbModelReqTama = {0, {0xa8, 0, 0, 0, 0, 0}, 128, {0x20, 0, 0, 0}};
// GLOBAL: FF7 0x00830568
struct GfxRotationAngles2 g_BattleSpecialEnergyOrbScaleTrajectory = {-1024, 0, 0};
// GLOBAL: FF7 0x00830570
unsigned char g_BattleSpecialEnergyOrbVectorTrajectory[12] = {0x00, 0x30, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00};
// GLOBAL: FF7 0x00830580
struct BattleSpecialEnergyOrbColor1 g_BattleSpecialEnergyOrbColor1 = {0, {0x28, 0, 0, 0, 0, 0}, 128, {0x20, 0, 0, 0}};
// GLOBAL: FF7 0x00830590
struct BattleSpecialEnergyOrbRotationTrajectory g_BattleSpecialEnergyOrbRotationTrajectory = {0, 0, -128, -128, -128, {0x2e}, 0};
// GLOBAL: FF7 0x008305a0
struct GfxRotationAngles2 g_BattleSpecialEnergyOrbColor2 = {-1024, 0, 0};
// GLOBAL: FF7 0x008305a8
unsigned char g_BattleSpecialEnergyOrbResFilenameHado[9] = "hado.rsd";
// GLOBAL: FF7 0x00b44f18
short g_BattleSpecialEnergyOrbScaleCounter = 0;
unsigned char g_bssPad_00b44f1c[4] = {0};
// GLOBAL: FF7 0x00b44f20
PsxMatrix g_BattleSpecialEnergyOrbBasisMatrix = {0};
// GLOBAL: FF7 0x00b44f40
short g_BattleSpecialEnergyOrbLifetimeCounter = 0;
unsigned char g_bssPad_00b44f44[4] = {0};
// GLOBAL: FF7 0x00b44f48
int g_BattleSpecialEnergyOrbCentroidPos = 0;
// GLOBAL: FF7 0x00b44f4c
short g_BattleSpecialEnergyOrbCentroidZ = 0;
// GLOBAL: FF7 0x00b44f50
int g_BattleSpecialEnergyOrbDrawListHandle = 0;
// GLOBAL: FF7 0x00b44f54
short g_BattleSpecialEnergyOrbTargetMask = 0;
// GLOBAL: FF7 0x00b44f58
short g_BattleSpecialEnergyOrbRandomAngle = 0;
// GLOBAL: FF7 0x00b44f5c
short g_BattleSpecialEnergyOrbFlashIntensity = 0;
// GLOBAL: FF7 0x00b44f60
struct GfxTableCursor *g_BattleSpecialEnergyOrbLoadedResTable = 0;
unsigned char g_bssPad_00b44f64[4] = {0};
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

// Resets three shared effect fields, including two intensity counters to eight. If the shared
// renderer exists and the cached resource is absent, initializes duration/options and creates
// resource selector fifteen from SPECIAL/RE_DO, its retained descriptor and renderer field. The
// options record is rooted at separate scalar requestRecord and the path helper receives scalar
// pathOut.
// FUNCTION: FF7 0x004de3e0
void __cdecl BattleFx_SpecialLoadResEnergyOrb(void)
{
    GfxModelTableBank *context;
    GfxResourceRequest requestRecord;
    int kind;
    char pathOut[200];
    GfxResourceRequest *pReq;
    int resultDefEntry;

    g_BattleSpecialEnergyOrbFlashIntensity = 0;
    g_BattleSpecialEnergyOrbLifetimeCounter = 8;
    g_BattleSpecialEnergyOrbScaleCounter = 8;
    if (g_pBattle3dScene != 0 && g_BattleSpecialEnergyOrbLoadedResTable == 0) {
        kind = 0xF;
        resultDefEntry = (int)&g_BattleSpecialEnergyOrbResourceDefs;
        context = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pReq = &requestRecord;
        Gfx_InitResourceRequest(pReq);
        pReq->groupTag = 0;
        // STRING: FF7 0x00830698
        pReq->resourcePath = BattleFx_BuildResourcePath("SPECIAL/RE_DO", pathOut);
        Gfx_LoadModelTableIfAbsent(kind, resultDefEntry, pReq, &g_BattleSpecialEnergyOrbLoadedResTable, context);
    }
}

/*
 * Thin forwarder: passes targetMask and actorIdx straight to BattleFx_SpecialBeginEnergyOrb and does nothing else.
 */
// FUNCTION: FF7 0x004de48b
void __cdecl BattleFx_SpecialEnterEnergyOrb(int targetMask, int actorIdx)
{
    BattleFx_SpecialBeginEnergyOrb(targetMask, actorIdx);
}

/*
 * Calls BattleFx_SpecialLoadResEnergyOrb, stores targetMask, allocates BattleFx_SpecialEnergyOrbTimelineTask with the actor index stored in its record and
 * gets the target-mask bone centroid into shared state. Clears three shared color bytes and starts
 * a fixed-point ramp with arguments 0x1000 and sixteen.
 */
// FUNCTION: FF7 0x004de4a0
void __cdecl BattleFx_SpecialBeginEnergyOrb(int targetMask, int actorIdx)
{
    int padSlot[2]; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    struct BattleTaskSlot *r;
    int padBlock[4]; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */

    BattleFx_SpecialLoadResEnergyOrb();
    g_BattleSpecialEnergyOrbTargetMask = targetMask;
    r = BATTLE_NEW_TASK_SLOT(BattleFx_SpecialEnergyOrbTimelineTask);
    ((BattleFxEnergyOrbRecord *)r)->actorIndex = actorIdx;
    Battle_GetTargetMaskBoneCentroid((short)targetMask, (Vec3sPacked *)&g_BattleSpecialEnergyOrbCentroidPos);
    g_BattleFxTimelineState.channel2 = 0;
    g_BattleFxTimelineState.channel1 = g_BattleFxTimelineState.channel2;
    g_BattleFxTimelineState.red = g_BattleFxTimelineState.channel1;
    Battle_StartFixedPointRamp(0x1000, 16);
}

/*
 * When unpaused at four, obtains the actor bone position, builds a shared look basis toward the
 * reference point and stores normalized velocity scaled by six hundred, then creates two model
 * callbacks and dispatches opcode 0x174. At twenty creates two more callbacks, one inheriting
 * velocity, starts another callback and dispatches opcode 0x164. During counts 37 through 43 emits
 * rotated-velocity secondary callbacks for target-mask actors; creates their typed records at
 * forty, starts a fade at 54 and alternates a shared short after twenty. Advances and retires at
 * seventy.
 */
// FUNCTION: FF7 0x004de51e
void __cdecl BattleFx_SpecialEnergyOrbTimelineTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[72];
        SVECTOR rotX;
        struct { unsigned char pad_to_8[0x8]; int depth; };
        struct { unsigned char pad_to_C[0xC]; short dirX; };
        struct { unsigned char pad_to_E[0xE]; short dirY; };
        struct { unsigned char pad_to_10[0x10]; short dirZ; };
        struct { unsigned char pad_to_14[0x14]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_34[0x34]; BattleFxSpecialEnergyOrbSlot *newSlot; };
        struct { unsigned char pad_to_3C[0x3C]; short actorIndex; };
        struct { unsigned char pad_to_44[0x44]; BattleFxEnergyOrbRecord *record; };
    } frame;
#pragma pack(pop)

    frame.record = (BattleFxEnergyOrbRecord *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.record->step == 4) {
        Battle_GetBoneWorldPos(frame.record->actorIndex, g_BattleActorRecords[frame.record->actorIndex].bone33, (short *)((char *)frame.record + 4));
        frame.dirX = ((BattleSpecialEnergyOrbCentroidPos *)&g_BattleSpecialEnergyOrbCentroidPos)->centroidX - frame.record->posX;
        frame.dirY = ((BattleSpecialEnergyOrbCentroidPos *)&g_BattleSpecialEnergyOrbCentroidPos)->centroidY - frame.record->posY;
        frame.dirZ = g_BattleSpecialEnergyOrbCentroidZ - frame.record->posZ;
        Gte_BuildLookBasisFromDirection(&frame.dirX, (short *)&g_BattleSpecialEnergyOrbBasisMatrix);
        VectorNormalSVectorToSVector(&frame.dirX, &frame.dirX);
        frame.record->offsetX = frame.dirX * 0x258 >> 12;
        frame.record->offsetY = frame.dirY * 0x258 >> 12;
        frame.record->offsetZ = frame.dirZ * 0x258 >> 12;
        frame.newSlot = (BattleFxSpecialEnergyOrbSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialEnergyOrbGrowingModelTask);
        frame.newSlot->posX = frame.record->posX;
        frame.newSlot->posY = frame.record->posY;
        frame.newSlot->posZ = frame.record->posZ;
        frame.newSlot = (BattleFxSpecialEnergyOrbSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialEnergyOrbPulsingSpriteTask);
        frame.newSlot->posX = frame.record->posX;
        frame.newSlot->posY = frame.record->posY;
        frame.newSlot->posZ = frame.record->posZ;
        Battle_PlaySfxPannedToActor(0x174, (short)frame.record->actorIndex, 0x40);
    }
    if (frame.record->step == 20) {
        frame.newSlot = (BattleFxSpecialEnergyOrbSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialEnergyOrbStretchingModelsTask);
        frame.newSlot->posX = frame.record->posX;
        frame.newSlot->posY = frame.record->posY;
        frame.newSlot->posZ = frame.record->posZ;
        frame.newSlot = (BattleFxSpecialEnergyOrbSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialEnergyOrbProjectileTask);
        frame.newSlot->posX = frame.record->posX;
        frame.newSlot->posY = frame.record->posY;
        frame.newSlot->posZ = frame.record->posZ;
        frame.newSlot->velocityX = frame.record->offsetX;
        frame.newSlot->velocityY = frame.record->offsetY;
        frame.newSlot->velocityZ = frame.record->offsetZ;
        Battle_PlaySfxPannedToActor(0x164, (short)frame.record->actorIndex, 0x40);
    }
    if (frame.record->step == 20) {
        Battle_AllocateResourceSlot(BattleFx_SpecialEnergyOrbScreenFlashTask);
    }
    if (frame.record->step == 54) {
        Battle_StartFixedPointRamp(0, 16);
    }
    if (frame.record->step > 36 && frame.record->step < 44) {
        for (frame.actorIndex = 0; frame.actorIndex < 10; frame.actorIndex++) {
            if ((g_BattleSpecialEnergyOrbTargetMask & 1 << frame.actorIndex) != 0) {
                frame.newSlot = (BattleFxSpecialEnergyOrbSlot *)&g_BattleTaskSlotBuffer.secondarySlots[(short)Battle_AllocateSecondaryResourceSlot(BattleFx_SpecialEnergyOrbDriftingAnimatedSpriteTask)];
                Battle_GetBoneWorldPos(frame.actorIndex, g_BattleActorRecords[frame.actorIndex].targetBone, (short *)((char *)frame.newSlot + 4));
                frame.dirY = 80;
                frame.dirZ = 0;
                frame.dirX = frame.dirZ;
                frame.rotX.vx = (rand() % 4096);
                frame.rotX.vz = (rand() % 4096);
                frame.rotX.vy = 0;
                RotMatrix(&frame.rotX, &frame.rotMatrix);
                frame.rotMatrix.t[2] = 0;
                frame.rotMatrix.t[1] = frame.rotMatrix.t[2];
                frame.rotMatrix.t[0] = frame.rotMatrix.t[1];
                SetRotMatrix(&frame.rotMatrix);
                SetTransMatrix(&frame.rotMatrix);
                RotTransPointToInt16(&frame.dirX, (short *)((char *)frame.newSlot + 0xC), &frame.depth);
            }
        }
    }
    if (frame.record->step == 40) {
        for (frame.actorIndex = 0; frame.actorIndex < 10; frame.actorIndex++) {
            if ((g_BattleSpecialEnergyOrbTargetMask & 1 << frame.actorIndex) != 0) {
                Battle_PostActorHitOrHurtMessage(frame.actorIndex);
            }
        }
    }
    if (frame.record->step > 20) {
        g_BattleCameraFocalPoint.focalPointY += (short)(16 - ((frame.record->step % 2) << 5));
    }
    (frame.record->step)++;
    if (frame.record->step == 70) {
        frame.record->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Draws a screen flash with blue from g_BattleSpecialEnergyOrbFlashIntensity and the other channels zero before checking
 * pause. When unpaused increases that shared channel by fifteen below counter sixteen, reduces it
 * by fifteen above counter thirty-four without clamping, advances the counter and retires exactly
 * at fifty.
 */
// FUNCTION: FF7 0x004de9bc
void __cdecl BattleFx_SpecialEnergyOrbScreenFlashTask(void)
{
    BattleTaskSlot *task;

    task = BATTLE_CURRENT_TASK_SLOT;
    g_BattleOverlayDrawListHead = BattleFx_FlashScreenWhite(0, 0, *(char *)&g_BattleSpecialEnergyOrbFlashIntensity);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (task->frameCounter < 16) {
        g_BattleSpecialEnergyOrbFlashIntensity += 0xF;
    }
    if (task->frameCounter > 34) {
        g_BattleSpecialEnergyOrbFlashIntensity -= 0xF;
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
// FUNCTION: FF7 0x004dea5e
void __cdecl BattleFx_SpecialEnergyOrbGrowingModelTask(void)
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
    g_BattleSpecialEnergyOrbParamVector1.modelGroup = Gfx_ModelTableGetRenderObject(6, g_BattleSpecialEnergyOrbLoadedResTable);
    RotMatrix((SVECTOR *)&g_BattleSpecialEnergyOrbRotationMoto, &frame.rotMatrix);
    ConcatMatrixPreservingTranslation(&g_BattleSpecialEnergyOrbBasisMatrix, &frame.rotMatrix, &frame.rotMatrix);
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
    g_BattleSpecialEnergyOrbParamVector1.alpha = frame.task->step << 8;
    g_BattleSpecialEnergyOrbDrawListHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecialEnergyOrbParamVector1, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialEnergyOrbDrawListHandle);
    if (g_BattleIsPaused != 0) {
        return;
    }
    (frame.task->step)++;
    if (frame.task->step == 20) {
        frame.task->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Another copy of the BattleFx_SpecialSuperOrbDriftingAnimatedSpriteTask moving-sprite tick, drawing
 * with g_BattleSpecialEnergyOrbModelReqMoto and taking key 0xE from g_BattleSpecialEnergyOrbLoadedResTable.
 * Installs a billboard at the slot position with scale 0x4000, integrates the position by the
 * velocity, and frees the slot when frameIndex reaches the frame count of the loaded sprite
 * record. The position integrates only when unpaused; the frame counter and the retirement also
 * need the sprite record to be non-null.
 */
// FUNCTION: FF7 0x004deba4
void __cdecl BattleFx_SpecialEnergyOrbDriftingAnimatedSpriteTask(void)
{
    BattleFxEnergyOrbDriftSprite *task;

    task = (BattleFxEnergyOrbDriftSprite *)&g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx];
    g_BattleSpecialEnergyOrbModelReqMoto.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(14, g_BattleSpecialEnergyOrbLoadedResTable);
    g_BattleSpecialEnergyOrbModelReqMoto.drawObj = Gfx_ModelTableGetSpriteObject(14, g_BattleSpecialEnergyOrbLoadedResTable);
    BattleFx_InstallBillboardMatrix((char *)task + 4, 0x4000 /* 4.0 */, 0);
    g_BattleSpecialEnergyOrbModelReqMoto.frame = task->frameIndex;
    g_BattleOverlayDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecialEnergyOrbModelReqMoto, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused == 0) {
        task->posX += task->velocityX;
        task->posY += task->velocityY;
        task->posZ += task->velocityZ;
        if (g_BattleSpecialEnergyOrbModelReqMoto.spriteGroup != 0) {
            (task->frameIndex)++;
            if (task->frameIndex >= g_BattleSpecialEnergyOrbModelReqMoto.spriteGroup->header->frameCount) {
                task->ownerSlot = BATTLE_TASK_FREE;
            }
        }
    }
}

/*
 * Per-frame task of the pulsing sprite: loads sprite record 0xB from
 * g_BattleSpecialEnergyOrbLoadedResTable into g_BattleSpecialEnergyOrbModelReqIsi, installs a billboard at
 * the task position with scale 0x1000 + (SinFixed(frame * 0x33) << 7 >> 12) << 7 and a forward
 * offset of -500, stores frame % 2 in the record's short12, and submits the sprite into the
 * depth-sorted ordering table. Unless paused it advances the frame and retires the slot at 0x28.
 */
// FUNCTION: FF7 0x004decb4
void __cdecl BattleFx_SpecialEnergyOrbPulsingSpriteTask(void)
{
    BattleEnergyOrbPulsingSprite *task;
    short wobble;

    task = (BattleEnergyOrbPulsingSprite *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecialEnergyOrbModelReqIsi.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(11, g_BattleSpecialEnergyOrbLoadedResTable);
    g_BattleSpecialEnergyOrbModelReqIsi.drawObj = Gfx_ModelTableGetSpriteObject(11, g_BattleSpecialEnergyOrbLoadedResTable);
    wobble = SinFixed(task->frameIndex * 51) << 7 >> 12;
    BattleFx_InstallBillboardMatrix(&task->billboardMatrix, (wobble << 7) + 0x1000, -500);
    g_BattleSpecialEnergyOrbModelReqIsi.frame = task->frameIndex % 2;
    g_BattleSpecialEnergyOrbDrawListHandle = BattleFx_SubmitSpriteDraw(&g_BattleSpecialEnergyOrbModelReqIsi, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC,
                                                   g_BattleSpecialEnergyOrbDrawListHandle);
    if (g_BattleIsPaused == 0) {
        task->frameIndex += 1;
        if (task->frameIndex == 40) {
            task->ownerSlot = -1;
        }
    }
}

/*
 * Draws model entries zero and one at the task position with shared orientation, shrinking X/Z
 * scale, growing Y scale and a descriptor field derived from shared intensity. Rendering continues
 * while paused. Unpaused calls lower shared intensity after twelve, increment the counter and
 * retire at twenty.
 */
// FUNCTION: FF7 0x004dedaa
void __cdecl BattleFx_SpecialEnergyOrbStretchingModelsTask(void)
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
    RotMatrix((SVECTOR *)&g_BattleSpecialEnergyOrbScaleTrajectory, &frame.rotMatrix);
    ConcatMatrixPreservingTranslation(&g_BattleSpecialEnergyOrbBasisMatrix, &frame.rotMatrix, &frame.rotMatrix);
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
    g_BattleSpecialEnergyOrbModelReqTama.alpha = 0x1000 - (g_BattleSpecialEnergyOrbLifetimeCounter << 8);
    g_BattleSpecialEnergyOrbModelReqTama.modelGroup = Gfx_ModelTableGetRenderObject(0, g_BattleSpecialEnergyOrbLoadedResTable);
    g_BattleSpecialEnergyOrbDrawListHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecialEnergyOrbModelReqTama, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialEnergyOrbDrawListHandle);
    g_BattleSpecialEnergyOrbModelReqTama.modelGroup = Gfx_ModelTableGetRenderObject(1, g_BattleSpecialEnergyOrbLoadedResTable);
    g_BattleSpecialEnergyOrbDrawListHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecialEnergyOrbModelReqTama, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialEnergyOrbDrawListHandle);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.task->frameCount > 12) {
        g_BattleSpecialEnergyOrbLifetimeCounter--;
    }
    (frame.task->frameCount)++;
    if (frame.task->frameCount == 20) {
        frame.task->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Draws model entries two, three and four plus a two-frame sprite at the task position with shared
 * orientation/scale and shared-intensity descriptor/color fields. Unpaused calls move XYZ, emit a
 * trail callback on even counts and, while Y is negative, create a randomized ground callback plus
 * another model on counts divisible by three. Lowers shared intensity after twelve, advances and
 * retires at twenty. Drawing continues while paused.
 */
// FUNCTION: FF7 0x004def5d
void __cdecl BattleFx_SpecialEnergyOrbProjectileTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[76];
        struct { unsigned char pad_to_4[0x4]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_24[0x24]; BattleFxEnergyOrbProjectile *newSlot; };
        struct { unsigned char pad_to_28[0x28]; BattleFxEnergyOrbProjectile *task; };
        struct { unsigned char pad_to_2C[0x2C]; PsxMatrix drawMatrix; };
    } frame;
#pragma pack(pop)

    frame.task = (BattleFxEnergyOrbProjectile *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecialEnergyOrbRotationTrajectory.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(11, g_BattleSpecialEnergyOrbLoadedResTable);
    g_BattleSpecialEnergyOrbRotationTrajectory.drawObj = Gfx_ModelTableGetSpriteObject(11, g_BattleSpecialEnergyOrbLoadedResTable);
    RotMatrix((SVECTOR *)&g_BattleSpecialEnergyOrbColor2, &frame.rotMatrix);
    ConcatMatrixPreservingTranslation(&g_BattleSpecialEnergyOrbBasisMatrix, &frame.rotMatrix, &frame.rotMatrix);
    ScaleMatrix(&frame.rotMatrix, (int *)&g_BattleSpecialEnergyOrbVectorTrajectory);
    frame.rotMatrix.t[0] = frame.task->posX;
    frame.rotMatrix.t[1] = frame.task->posY;
    frame.rotMatrix.t[2] = frame.task->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.drawMatrix);
    SetRotMatrix(&frame.drawMatrix);
    SetTransMatrix(&frame.drawMatrix);
    g_BattleSpecialEnergyOrbColor1.alpha = g_BattleSpecialEnergyOrbScaleCounter << 4;
    g_BattleSpecialEnergyOrbColor1.modelGroup = Gfx_ModelTableGetRenderObject(2, g_BattleSpecialEnergyOrbLoadedResTable);
    g_BattleSpecialEnergyOrbDrawListHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecialEnergyOrbColor1, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialEnergyOrbDrawListHandle);
    g_BattleSpecialEnergyOrbColor1.modelGroup = Gfx_ModelTableGetRenderObject(3, g_BattleSpecialEnergyOrbLoadedResTable);
    g_BattleSpecialEnergyOrbDrawListHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecialEnergyOrbColor1, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialEnergyOrbDrawListHandle);
    g_BattleSpecialEnergyOrbColor1.modelGroup = Gfx_ModelTableGetRenderObject(4, g_BattleSpecialEnergyOrbLoadedResTable);
    g_BattleSpecialEnergyOrbDrawListHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecialEnergyOrbColor1, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialEnergyOrbDrawListHandle);
    BattleFx_InstallBillboardMatrix((char *)frame.task + 4, 0x4000 /* 4.0 */, 0);
    g_BattleSpecialEnergyOrbRotationTrajectory.trajectoryByte10 = g_BattleSpecialEnergyOrbScaleCounter * 20;
    g_BattleSpecialEnergyOrbRotationTrajectory.trajectoryByte9 = g_BattleSpecialEnergyOrbRotationTrajectory.trajectoryByte10;
    g_BattleSpecialEnergyOrbRotationTrajectory.trajectoryByte8 = g_BattleSpecialEnergyOrbRotationTrajectory.trajectoryByte9;
    g_BattleSpecialEnergyOrbRotationTrajectory.trajectoryShort12 = (frame.task->frameCount % 2);
    g_BattleSpecialEnergyOrbDrawListHandle = BattleFx_SubmitSpriteDraw(&g_BattleSpecialEnergyOrbRotationTrajectory, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialEnergyOrbDrawListHandle);
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.task->posX += frame.task->velocityX;
    frame.task->posY += frame.task->velocityY;
    frame.task->posZ += frame.task->velocityZ;
    if ((frame.task->frameCount % 2) == 0) {
        frame.newSlot = (BattleFxEnergyOrbProjectile *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialEnergyOrbTrailModelTask);
        frame.newSlot->posX = frame.task->posX;
        frame.newSlot->posY = frame.task->posY;
        frame.newSlot->posZ = frame.task->posZ;
    }
    if (frame.task->posY < 0) {
        frame.newSlot = (BattleFxEnergyOrbProjectile *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialEnergyOrbBouncingSpriteTask);
        frame.newSlot->posX = frame.task->posX;
        frame.newSlot->posZ = frame.task->posZ;
        frame.newSlot->posY = 0;
        g_BattleSpecialEnergyOrbRandomAngle = (rand() % 4096);
        frame.newSlot->velocityX = CosFixed(g_BattleSpecialEnergyOrbRandomAngle) * 0x78 >> 12;
        frame.newSlot->velocityZ = SinFixed(g_BattleSpecialEnergyOrbRandomAngle) * 0x78 >> 12;
        frame.newSlot->velocityY = -rand() % 300 - 50;
        frame.newSlot->variant = rand() % 7;
        if (frame.task->frameCount % 3 == 0) {
            frame.newSlot = (BattleFxEnergyOrbProjectile *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialEnergyOrbFallingFadeModelTask);
            frame.newSlot->posX = frame.task->posX + rand() % 4000 + -2000;
            frame.newSlot->posZ = frame.task->posZ + rand() % 4000 + -2000;
            frame.newSlot->posY = 0;
            frame.newSlot->variant = (rand() % 4);
        }
    }
    if (frame.task->frameCount > 12) {
        g_BattleSpecialEnergyOrbScaleCounter--;
    }
    (frame.task->frameCount)++;
    if (frame.task->frameCount == 20) {
        frame.task->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Draws sprite entry thirteen with task-selected frame, fixed doubled scale, stored Z rotation and
 * gray holding eight before twelve then twenty minus count. Unpaused calls move XYZ, clamp
 * positive Y to zero and reverse Y velocity, add gravity, rotate by 0x200, advance and retire at
 * twenty. Drawing continues while paused.
 */
// FUNCTION: FF7 0x004df3b8
void __cdecl BattleFx_SpecialEnergyOrbBouncingSpriteTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[92];
        int spriteFrame;
        struct { unsigned char pad_to_4[0x4]; SVECTOR rotX; };
        struct { unsigned char pad_to_C[0xC]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_2C[0x2C]; short frameShort; };
        struct { unsigned char pad_to_30[0x30]; PsxMatrix spriteMatrix; };
        struct { unsigned char pad_to_50[0x50]; short axisIndex; };
        struct { unsigned char pad_to_54[0x54]; int depth; };
        struct { unsigned char pad_to_58[0x58]; BattleFxEnergyOrbBouncingSprite *sprite; };
    } frame;
#pragma pack(pop)

    frame.sprite = (BattleFxEnergyOrbBouncingSprite *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecialEnergyOrbScaleTama.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(13, g_BattleSpecialEnergyOrbLoadedResTable);
    g_BattleSpecialEnergyOrbScaleTama.drawObj = Gfx_ModelTableGetSpriteObject(13, g_BattleSpecialEnergyOrbLoadedResTable);
    for (frame.axisIndex = 0; frame.axisIndex < 3; frame.axisIndex++) {
        frame.spriteMatrix.m[2][frame.axisIndex] = 0;
        frame.spriteMatrix.m[1][frame.axisIndex] = 0;
        frame.spriteMatrix.m[0][frame.axisIndex] = 0;
    }
    if (frame.sprite->frameCount < 12) {
        frame.spriteFrame = 8;
    } else {
        frame.spriteFrame = 20 - frame.sprite->frameCount;
    }
    frame.frameShort = *(short *)&frame.spriteFrame;
    frame.rotX.vy = 0;
    frame.rotX.vx = frame.rotX.vy;
    frame.rotX.vz = frame.sprite->angle;
    SetRotMatrix(&g_BattleCameraTransform);
    SetTransMatrix(&g_BattleCameraTransform);
    RotTransPoint((char *)frame.sprite + 4, frame.spriteMatrix.t, &frame.depth);
    frame.spriteMatrix.m[2][2] = 0x2000;
    frame.spriteMatrix.m[1][1] = frame.spriteMatrix.m[2][2];
    frame.spriteMatrix.m[0][0] = frame.spriteMatrix.m[1][1];
    RotMatrix(&frame.rotX, &frame.rotMatrix);
    ConcatMatrixIntoFirst(&frame.spriteMatrix, &frame.rotMatrix);
    SetRotMatrix(&frame.spriteMatrix);
    SetTransMatrix(&frame.spriteMatrix);
    g_BattleSpecialEnergyOrbScaleTama.frame = frame.sprite->variant;
    g_BattleSpecialEnergyOrbScaleTama.code = (frame.frameShort - 8 != 0 ? 2 : 0) + 0x2C;
    g_BattleSpecialEnergyOrbScaleTama.colorB = frame.frameShort << 4;
    g_BattleSpecialEnergyOrbScaleTama.colorG = g_BattleSpecialEnergyOrbScaleTama.colorB;
    g_BattleSpecialEnergyOrbScaleTama.colorR = g_BattleSpecialEnergyOrbScaleTama.colorG;
    g_BattleOverlayDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecialEnergyOrbScaleTama, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
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
 * Draws a task-selector-indexed model at the task position using stored Z rotation, shared scale
 * and gray 128 minus six times count. Unpaused calls lower Y by two hundred, rotate by 0x100,
 * advance and retire at twenty. Drawing continues while paused.
 */
// FUNCTION: FF7 0x004df632
void __cdecl BattleFx_SpecialEnergyOrbFallingFadeModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[76];
        SVECTOR rotX;
        struct { unsigned char pad_to_8[0x8]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_28[0x28]; BattleFxSpecialEnergyOrbFallingSlot *task; };
        struct { unsigned char pad_to_2C[0x2C]; PsxMatrix drawMatrix; };
    } frame;
#pragma pack(pop)

    frame.task = (BattleFxSpecialEnergyOrbFallingSlot *)BATTLE_CURRENT_TASK_SLOT;
    frame.rotX.vy = 0;
    frame.rotX.vx = frame.rotX.vy;
    frame.rotX.vz = frame.task->angleZ;
    RotMatrixXYZ(&frame.rotX, &frame.rotMatrix);
    ScaleMatrix(&frame.rotMatrix, (int *)&g_BattleSpecialEnergyOrbRotationTama);
    frame.rotMatrix.t[0] = frame.task->posX;
    frame.rotMatrix.t[1] = frame.task->posY;
    frame.rotMatrix.t[2] = frame.task->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.drawMatrix);
    SetRotMatrix(&frame.drawMatrix);
    SetTransMatrix(&frame.drawMatrix);
    g_BattleSpecialEnergyOrbVector1.alpha = 0x80 - frame.task->frame * 6;
    g_BattleSpecialEnergyOrbVector1.modelGroup = Gfx_ModelTableGetRenderObject(((int *)&g_BattleSpecialEnergyOrbModelConfig)[frame.task->modelIndex], g_BattleSpecialEnergyOrbLoadedResTable);
    g_BattleSpecialEnergyOrbDrawListHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecialEnergyOrbVector1, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialEnergyOrbDrawListHandle);
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.task->posY -= 200;
    frame.task->angleZ += 0x100 /* 22.5 deg */;
    (frame.task->frame)++;
    if (frame.task->frame == 20) {
        frame.task->slotTag = 0xFFFF;
    }
}

/*
 * Loads model entry five and, only after count two, draws it at the task position with shared
 * orientation, shrinking uniform scale and a count-times-two-hundred descriptor field. Rendering
 * continues while paused; unpaused calls increment the counter and retire at twenty even when the
 * draw branch was skipped.
 */
// FUNCTION: FF7 0x004df78c
void __cdecl BattleFx_SpecialEnergyOrbTrailModelTask(void)
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
    g_BattleSpecialEnergyOrbParamVector2.modelGroup = Gfx_ModelTableGetRenderObject(5, g_BattleSpecialEnergyOrbLoadedResTable);
    if (frame.task->frameCount > 2) {
        RotMatrix((SVECTOR *)&g_BattleSpecialEnergyOrbRotationIsi, &frame.rotMatrix);
        ConcatMatrixPreservingTranslation(&g_BattleSpecialEnergyOrbBasisMatrix, &frame.rotMatrix, &frame.rotMatrix);
        frame.scaleX.vz = 0x4000 - frame.task->frameCount * 0x190;
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
        g_BattleSpecialEnergyOrbParamVector2.alpha = frame.task->frameCount * 200;
        g_BattleSpecialEnergyOrbDrawListHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecialEnergyOrbParamVector2, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialEnergyOrbDrawListHandle);
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    (frame.task->frameCount)++;
    if (frame.task->frameCount == 20) {
        frame.task->ownerSlot = BATTLE_TASK_FREE;
    }
}
} /* extern "C" */
