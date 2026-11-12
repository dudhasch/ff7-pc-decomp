// src/ff7/src/battle/battle_fx_weapon_diamond_flash.cpp
/* Enemy Special Attack [155]: Diamond Weapon Diamond Flash (orig. weapon/dia_u) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Attack effect for the Diamond Weapon: loads its resources and runs a timeline that
//   spawns pulsing, converging and trailing models, sprites, and a full-screen flash.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/battle_tasks.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_fx.h"
#include "lib/types/graphics.h"
#include "lib/enums/sound.h"
#include "ff7/camera_script.h"
#include <stdlib.h>
#include "ff7/api/battle.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "lib/api/sound.h"
extern "C" {

struct BattleFxModelDraw;

// clang-format off
// GLOBAL: FF7 0x007d7e38
unsigned int g_WeaponFxDiamondFlashResourceDefs[100] = {
    0x001900b1, (unsigned int)"senkou2.rsd", 0x00000000, 0x00000000, 0x00000000,
    0x001900b1, (unsigned int)"hikari.rsd", 0x00000000, 0x00000000, 0x00000000,
    0x001900b1, (unsigned int)"hikari1.rsd", 0x00000000, 0x00000000, 0x00000000,
    0x001900b1, (unsigned int)"line.rsd", 0x00000000, 0x00000000, 0x00000000,
    0x001900b1, (unsigned int)"gan1.rsd", 0x00000000, 0x00000000, 0x00000000,
    0x001900b1, (unsigned int)"gan2.rsd", 0x00000000, 0x00000000, 0x00000000,
    0x001900b1, (unsigned int)"gan3.rsd", 0x00000000, 0x00000000, 0x00000000,
    0x001900b1, (unsigned int)"gan4.rsd", 0x00000000, 0x00000000, 0x00000000,
    0x001900b1, (unsigned int)"gan5.rsd", 0x00000000, 0x00000000, 0x00000000,
    0x001900b1, (unsigned int)"gan6.rsd", 0x00000000, 0x00000000, 0x00000000,
    0x001900b1, (unsigned int)"tama1.rsd", 0x00000000, 0x00000000, 0x00000000,
    0x001900b1, (unsigned int)"tama2.rsd", 0x00000000, 0x00000000, 0x00000000,
    0x003200b1, (unsigned int)"moto.rsd", 0x00000000, 0x00000000, 0x00000000,
    0x001900b1, (unsigned int)"hado1.rsd", 0x00000000, 0x00000000, 0x00000000,
    0x001900b1, (unsigned int)"hado.rsd", 0x00000000, 0x00000000, 0x00000000,
    0x001900b1, (unsigned int)"soto.rsd", 0x00000000, 0x00000000, 0x00000000,
    0x00192a39, (unsigned int)"kemu1.tim", (unsigned int)"kemu1.spt", 0x00000000, 0x00000000,
    0x00192a39, (unsigned int)"kemu2.tim", (unsigned int)"kemu2.spt", 0x00000000, 0x00000000,
    0x00192a39, (unsigned int)"baku1.tim", (unsigned int)"baku1.spt", 0x00000000, 0x00000000,
    0x00192a39, (unsigned int)"kou.tim", (unsigned int)"kou.spt", 0x00000000, 0x00000000};
// clang-format off
// GLOBAL: FF7 0x007d7fc8
struct WeaponFxDiamondFlashPulsingOrientedDrawReq g_WeaponFxDiamondFlashPulsingOrientedDrawReq = {0, {0xa8, 0, 0, 0, 0, 0}, 128};
unsigned char g_rawData_007d7fd4[4] = {0x20, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x007d7fd8
WeaponFxDiamondFlashInwardModelReq g_WeaponFxDiamondFlashPulsingInwardDrawReqComposite = {{0, 0xfc, 0, 0, 0, 0, 0, 0}, 0, {0xa8, 0, 0, 0, 0, 0}, 128, {0x20, 0, 0, 0, 0, 0xfc, 0, 0, 0, 0, 0, 0}, 0};
unsigned char g_rawData_007d7ffc[4] = {0xa8, 0x00, 0x00, 0x00};
unsigned char g_rawData_007d8000[8] = {0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x007d8008
int g_WeaponFxDiamondFlashInwardOrientationAngle = 64512;
// GLOBAL: FF7 0x007d8010
struct WeaponFxDiamondFlashConvergingDrawReq g_WeaponFxDiamondFlashConvergingDrawReq = {{0, 0x60, 0, 0, 0, 0x30, 0, 0, 0, 0x30, 0, 0, 0, 0, 0, 0}, 0, {0xa8, 0, 0, 0, 0, 0}, 128};
unsigned char g_rawData_007d802c[4] = {0x20, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x007d8030
int g_WeaponFxDiamondFlashConvergingScaleFactor = 1024;
// clang-format on
// GLOBAL: FF7 0x007d8038
struct BattleFxSpriteRequest g_WeaponFxDiamondFlashDampedSpriteReq = {0, 0, -128, -128, -128, 0x2e, 0, 0};
// GLOBAL: FF7 0x007d8048
struct BattleFxDiamondFlashTripleScaleVector g_WeaponFxDiamondFlashTripleModelScaleVector = {0x1000, {0, 0x10, 0, 0}, 0x1000};
// GLOBAL: FF7 0x007d8058
unsigned char g_WeaponFxDiamondFlashTripleModelDrawReq[16] = {0x00, 0x00, 0x00, 0x00, 0xa8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00};
// clang-format off
// GLOBAL: FF7 0x007d8068
struct WeaponFxDiamondFlashBillboardSpriteReq g_WeaponFxDiamondFlashBillboardSpriteReq = {0, 0, {0x80, 0x80, 0x80, 0x2c}, 0, 0};
// GLOBAL: FF7 0x007d8078
struct WeaponFxDiamondFlashTrailModelDrawReq g_WeaponFxDiamondFlashTrailModelDrawReq = {{0, 0xfc, 0, 0, 0, 0, 0, 0}, 0, {0xa8, 0, 0, 0, 0, 0}, 0};
unsigned char g_rawData_007d808c[4] = {0x20, 0x00, 0x00, 0x00};
// clang-format on
// GLOBAL: FF7 0x007d8090
struct BattleFxWeaponDiamondFlashShrinkRequest g_WeaponFxDiamondFlashShrinkingModelDrawReq = {0, {0xa8, 0, 0, 0, 0, 0}, 128, {0x20, 0, 0, 0}};
// GLOBAL: FF7 0x007d80a0
short g_WeaponFxDiamondFlashRotationAngles[3] = {(-1024), 0, 0};
// clang-format off
// GLOBAL: FF7 0x007d80a8
struct WeaponFxDiamondFlashTrailingDrawReqComposite g_WeaponFxDiamondFlashTrailingDrawReqComposite = {{0, 0x48, 0, 0, 0, 0x48, 0, 0, 0, 0x48, 0, 0, 0, 0, 0, 0}, 0, {0x28, 0, 0, 0, 0, 0}, 128, {0x20, 0, 0, 0}, 0, 0};
// GLOBAL: FF7 0x007d80d0
struct WeaponFxDiamondFlashTrailingSpriteParams g_WeaponFxDiamondFlashTrailingSpriteParams = {-128, -128, -128, {0x2e}, 0, {0}};
// GLOBAL: FF7 0x007d80d8
struct SVECTOR g_WeaponFxDiamondFlashTrailingRotationVector = {-1024, 0, 0, 0};
// clang-format on
// GLOBAL: FF7 0x009f0228
struct BattleLimitHighwindLoadedResTable g_WeaponFxDiamondFlashProjectedPos = {0};
// GLOBAL: FF7 0x009f022c
short g_WeaponFxDiamondFlashProjectedPosZ = 0;
unsigned char g_bssPad_009f0230[32] = {0};
// GLOBAL: FF7 0x009f0250
PsxMatrix g_WeaponFxDiamondFlashMatrix = {0};
// GLOBAL: FF7 0x009f0270
int g_WeaponFxDiamondFlashSpriteDistance = 0;
unsigned char g_bssPad_009f0274[4] = {0};
// GLOBAL: FF7 0x009f0278
short g_WeaponFxDiamondFlashSpriteDeltaPos[3] = {0};
// GLOBAL: FF7 0x009f0280
struct BattleLimitHighwindLoadedResTable g_WeaponFxDiamondFlashActorPos = {0};
// GLOBAL: FF7 0x009f0284
short g_WeaponFxDiamondFlashActorPosZ = 0;
// GLOBAL: FF7 0x009f0288
short g_WeaponFxDiamondFlashActorIndex = 0;
// GLOBAL: FF7 0x009f028c
short g_WeaponFxDiamondFlashBoneIndex = 0;
// GLOBAL: FF7 0x009f0290
struct BattleLimitHighwindLoadedResTable g_WeaponFxDiamondFlashCentroidPos = {0};
// GLOBAL: FF7 0x009f0294
short g_WeaponFxDiamondFlashCentroidPosZ = 0;
// GLOBAL: FF7 0x009f0298
struct GfxTableCursor *g_WeaponFxDiamondFlashResourceContext = 0;
// GLOBAL: FF7 0x009f029c
short g_WeaponFxDiamondFlashSpriteExpandActive = 0;
// clang-format on


// Original complete sprite request, including its fixed primitive-code byte.

/*
 * Loads weapon effect resources for Diamond Weapon ("weapon/dia_u")
 * and initializes its resource descriptor context if not already loaded.
 */
// FUNCTION: FF7 0x004429b0
void __cdecl BattleFx_WeaponLoadResDiamondFlash(void)
{
    GfxModelTableBank *sceneContext;
    GfxResourceRequest requestBuffer;
    int resultId;
    char pathSize[200];
    GfxResourceRequest *pReq;
    int defsPtr;

    if (g_pBattle3dScene != 0 && g_WeaponFxDiamondFlashResourceContext == 0) {
        resultId = 0x14;
        defsPtr = (int)&g_WeaponFxDiamondFlashResourceDefs;
        sceneContext = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pReq = &requestBuffer;
        Gfx_InitResourceRequest(pReq);
        pReq->groupTag = 0;
        // STRING: FF7 0x007d81f8
        (&pReq->resourcePath)[0] = (char *)BattleFx_BuildResourcePath("weapon/dia_u", pathSize);
        Gfx_LoadModelTableIfAbsent(resultId, defsPtr, pReq, &g_WeaponFxDiamondFlashResourceContext, sceneContext);
    }
}

/*
 * Entry point for Diamond Weapon attack effect:
 * resets sprite expansion flag and begins the weapon effect sequence.
 */
// FUNCTION: FF7 0x00442a40
void __cdecl BattleFx_WeaponEnterDiamondFlash(int targetMask, int actorIdx)
{
    g_WeaponFxDiamondFlashSpriteExpandActive = 0;
    BattleFx_WeaponBeginDiamondFlash(targetMask, actorIdx);
}

/*
 * Initializes Diamond Weapon attack effect sequence:
 * loads resources, records target actor and bone indices, computes target
 * centroid and look-basis transform matrix, and starts brightness ramp.
 */
// FUNCTION: FF7 0x00442a5e
void __cdecl BattleFx_WeaponBeginDiamondFlash(int targetMask, int actorIdx)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[12];
        SVECTOR dirX;
        struct { unsigned char pad_to_8[0x8]; BattleFxDiamondFlashWeaponRecord *record; };
    } frame;
#pragma pack(pop)

    g_BattleOverlayDrawListHead = Gfx_GetPsxPacketScratchPtr();
    BattleFx_WeaponLoadResDiamondFlash();
    g_WeaponFxDiamondFlashBoneIndex = actorIdx;
    g_WeaponFxDiamondFlashActorIndex = targetMask;
    frame.record = (BattleFxDiamondFlashWeaponRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_WeaponDiamondFlashTimelineTask);
    frame.record->actorIdx = actorIdx;
    Battle_GetBoneWorldPos(actorIdx, g_BattleActorRecords[actorIdx].bone34, (short *)&g_WeaponFxDiamondFlashActorPos);
    Battle_GetActorMaskCentroid(targetMask, &frame.record->posX);
    frame.dirX.vx = frame.record->posX - g_WeaponFxDiamondFlashActorPos.posX;
    frame.dirX.vy = frame.record->posY - 800 - g_WeaponFxDiamondFlashActorPos.posY;
    frame.dirX.vz = frame.record->posZ - g_WeaponFxDiamondFlashActorPosZ;
    Gte_BuildLookBasisFromDirection(&frame.dirX.vx, (short *)&g_WeaponFxDiamondFlashMatrix);
    g_BattleFxTimelineState.channel2 = 0;
    g_BattleFxTimelineState.channel1 = g_BattleFxTimelineState.channel2;
    g_BattleFxTimelineState.red = g_BattleFxTimelineState.channel1;
    Battle_StartFixedPointRamp(0xE00, 20);
}

/*
 * Main timeline controller task for Diamond Weapon attack effect:
 * spawns sub-tasks (billboards, converging models, streak models, sprites,
 * flashes) across frame counts 0 through 90.
 */
// FUNCTION: FF7 0x00442b67
void __cdecl BattleFx_WeaponDiamondFlashTimelineTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[72];
        SVECTOR rotX;
        struct { unsigned char pad_to_C[0xC]; short offsetX; };
        struct { unsigned char pad_to_E[0xE]; short offsetY; };
        struct { unsigned char pad_to_10[0x10]; short offsetZ; };
        struct { unsigned char pad_to_14[0x14]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_34[0x34]; BattleFxDiamondFlashParticle *particle; };
        struct { unsigned char pad_to_3C[0x3C]; short actorIndex; };
        struct { unsigned char pad_to_40[0x40]; short angle; };
        struct { unsigned char pad_to_44[0x44]; BattleFxCosmoMemoryExpandingModel *centroid; };
    } frame;
#pragma pack(pop)

    frame.centroid = (BattleFxCosmoMemoryExpandingModel *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.centroid->frame == 0) {
        frame.particle = (BattleFxDiamondFlashParticle *)BATTLE_NEW_TASK_SLOT(BattleFx_WeaponDiamondFlashPulsingBillboardModelTask);
        frame.particle->posX = g_WeaponFxDiamondFlashActorPos.posX;
        frame.particle->posY = g_WeaponFxDiamondFlashActorPos.posY;
        frame.particle->posZ = g_WeaponFxDiamondFlashActorPosZ;
        SoundDispatchCommand(SOUND_SFX_PLAY_CH1, Battle_ComputeScreenPanFromPosition(&g_BattleActorRecords[g_WeaponFxDiamondFlashBoneIndex].posX), 0x2A8, 0, 0, 0, 0, 0, 0);
    }
    if (frame.centroid->frame >= 0 && frame.centroid->frame < 20) {
        frame.particle = (BattleFxDiamondFlashParticle *)BATTLE_NEW_TASK_SLOT(BattleFx_WeaponDiamondFlashInwardModelTask);
        frame.particle->posX = g_WeaponFxDiamondFlashActorPos.posX;
        frame.particle->posY = g_WeaponFxDiamondFlashActorPos.posY;
        frame.particle->posZ = g_WeaponFxDiamondFlashActorPosZ;
        frame.offsetX = 0;
        frame.offsetY = 0;
        frame.offsetZ = 0xFA0;
        frame.rotX.vx = rand() % 0x640 - 0x320;
        frame.rotX.vy = rand() % 0x640 - 0x320;
        frame.rotX.vz = 0;
        RotMatrixXYZ(&frame.rotX, &frame.rotMatrix);
        TransformVectorToInt16(&frame.rotMatrix, &frame.offsetX, &frame.offsetX);
        frame.particle->posX += frame.offsetX;
        frame.particle->posY += frame.offsetY;
        frame.particle->posZ += frame.offsetZ;
        frame.particle->velocityX = -frame.offsetX / 15;
        frame.particle->velocityY = -frame.offsetY / 15;
        frame.particle->velocityZ = -frame.offsetZ / 15;
    }
    if (frame.centroid->frame >= 10 && frame.centroid->frame < 25) {
        for (frame.actorIndex = 0; frame.actorIndex < 4; frame.actorIndex++) {
            frame.particle = (BattleFxDiamondFlashParticle *)BATTLE_NEW_TASK_SLOT(BattleFx_WeaponDiamondFlashConvergingModelTask);
            frame.particle->velocityX = rand() % 1600 - 800;
            frame.particle->velocityY = 0;
            frame.particle->velocityZ = (rand() % 4096);
            frame.particle->convergeDistance = 8000;
        }
    }
    if (frame.centroid->frame >= 60 && frame.centroid->frame < 75 && frame.centroid->frame % 5 == 0) {
        frame.particle = (BattleFxDiamondFlashParticle *)BATTLE_NEW_TASK_SLOT(BattleFx_WeaponDiamondFlashPulsingOrientedModelTask);
        frame.particle->posX = g_WeaponFxDiamondFlashActorPos.posX;
        frame.particle->posY = g_WeaponFxDiamondFlashActorPos.posY;
        frame.particle->posZ = g_WeaponFxDiamondFlashActorPosZ;
    }
    if (frame.centroid->frame == 30) {
        SoundDispatchCommand(SOUND_SFX_PLAY_CH2, Battle_ComputeScreenPanFromPosition(&g_BattleActorRecords[g_WeaponFxDiamondFlashBoneIndex].posX), 0x12E, 0, 0, 0, 0, 0, 0);
    }
    if (frame.centroid->frame >= 30 && frame.centroid->frame < 60) {
        for (frame.actorIndex = 0; frame.actorIndex < 4; frame.actorIndex++) {
            frame.particle = (BattleFxDiamondFlashParticle *)BATTLE_NEW_TASK_SLOT(BattleFx_WeaponDiamondFlashTripleModelStreakTask);
            frame.angle = (rand() % 4096);
            frame.particle->posX = g_WeaponFxDiamondFlashActorPos.posX + (CosFixed(frame.angle) * 0x258 >> 12);
            frame.particle->posY = g_WeaponFxDiamondFlashActorPos.posY + (SinFixed(frame.angle) * 0x258 >> 12);
            frame.particle->posZ = g_WeaponFxDiamondFlashActorPosZ;
            frame.offsetX = frame.centroid->posX + (CosFixed(frame.angle) * 0x7D0 >> 12);
            frame.offsetY = 0;
            frame.offsetZ = frame.centroid->posZ - (SinFixed(frame.angle) * 0x7D0 >> 12);
            frame.offsetX = frame.offsetX - frame.particle->posX;
            frame.offsetY = frame.offsetY - frame.particle->posY;
            frame.offsetZ = frame.offsetZ - frame.particle->posZ;
            VectorNormalSVectorToSVector(&frame.offsetX, &frame.offsetX);
            frame.particle->velocityX = frame.offsetX * 0x3E8 >> 12;
            frame.particle->velocityY = frame.offsetY * 0x3E8 >> 12;
            frame.particle->velocityZ = frame.offsetZ * 0x3E8 >> 12;
            frame.particle->variant = (rand() % 2);
            frame.particle->angle = frame.angle;
        }
    }
    if (frame.centroid->frame >= 35 && frame.centroid->frame < 50) {
        for (frame.actorIndex = 0; frame.actorIndex < 3; frame.actorIndex++) {
            frame.particle = (BattleFxDiamondFlashParticle *)&g_BattleTaskSlotBuffer.secondarySlots[(short)Battle_AllocateSecondaryResourceSlot(BattleFx_WeaponDiamondFlashBillboardSpriteTask)];
            frame.angle = (rand() % 4096);
            frame.offsetX = 0;
            frame.offsetY = -rand() % 1200;
            frame.offsetZ = 0;
            frame.rotX.vx = (rand() % 2048) - 0x400;
            frame.rotX.vy = (rand() % 4096);
            frame.rotX.vz = 0;
            RotMatrix(&frame.rotX, &frame.rotMatrix);
            TransformVectorToInt16(&frame.rotMatrix, &frame.offsetX, &frame.offsetX);
            frame.particle->posX = frame.centroid->posX + frame.offsetX;
            frame.particle->posY = frame.centroid->posY + frame.offsetY;
            frame.particle->posZ = frame.centroid->posZ + frame.offsetZ;
            frame.particle->velocityX = frame.offsetX / 4;
            frame.particle->velocityY = frame.offsetY / 4;
            frame.particle->velocityZ = frame.offsetZ / 4;
            frame.particle->variant = (rand() % 2);
        }
    }
    if (frame.centroid->frame >= 40 && frame.centroid->frame < 55) {
        for (frame.actorIndex = 0; frame.actorIndex < 3; frame.actorIndex++) {
            frame.particle = (BattleFxDiamondFlashParticle *)BATTLE_NEW_TASK_SLOT(BattleFx_WeaponDiamondFlashDampedSpriteTask);
            frame.angle = (rand() % 4096);
            frame.offsetX = 0;
            frame.offsetY = -rand() % 1500;
            frame.offsetZ = 0;
            frame.rotX.vx = (rand() % 2048) - 0x400;
            frame.rotX.vy = (rand() % 4096);
            frame.rotX.vz = 0;
            RotMatrix(&frame.rotX, &frame.rotMatrix);
            TransformVectorToInt16(&frame.rotMatrix, &frame.offsetX, &frame.offsetX);
            frame.particle->posX = frame.centroid->posX + frame.offsetX;
            frame.particle->posY = frame.centroid->posY + frame.offsetY;
            frame.particle->posZ = frame.centroid->posZ + frame.offsetZ;
            frame.particle->velocityX = frame.offsetX / 4;
            frame.particle->velocityY = frame.offsetY / 4;
            frame.particle->velocityZ = frame.offsetZ / 4;
            frame.particle->variant = (rand() % 2);
            frame.particle->fadeTimer = 12;
        }
    }
    if (frame.centroid->frame == 60) {
        frame.particle = (BattleFxDiamondFlashParticle *)BATTLE_NEW_TASK_SLOT(BattleFx_WeaponDiamondFlashShrinkingModelPairTask);
        frame.particle->posX = g_WeaponFxDiamondFlashActorPos.posX;
        frame.particle->posY = g_WeaponFxDiamondFlashActorPos.posY;
        frame.particle->posZ = g_WeaponFxDiamondFlashActorPosZ;
        frame.particle = (BattleFxDiamondFlashParticle *)BATTLE_NEW_TASK_SLOT(BattleFx_WeaponDiamondFlashTrailingProjectileTask);
        frame.particle->posX = g_WeaponFxDiamondFlashActorPos.posX;
        frame.particle->posY = g_WeaponFxDiamondFlashActorPos.posY;
        frame.particle->posZ = g_WeaponFxDiamondFlashActorPosZ;
        frame.offsetX = 0;
        frame.offsetY = 0;
        frame.offsetZ = 0x4B0;
        TransformVectorToInt16((void *)&g_WeaponFxDiamondFlashMatrix, &frame.offsetX, &frame.particle->velocityX);
    }
    if (frame.centroid->frame == 60) {
        g_WeaponFxDiamondFlashCentroidPos.posX = frame.centroid->posX;
        g_WeaponFxDiamondFlashCentroidPos.posY = 0xFC18;
        g_WeaponFxDiamondFlashCentroidPosZ = frame.centroid->posZ;
        g_WeaponFxDiamondFlashSpriteExpandActive = 1;
    }
    if (frame.centroid->frame == 70) {
        frame.particle = (BattleFxDiamondFlashParticle *)BATTLE_NEW_TASK_SLOT(BattleFx_WeaponDiamondFlashScreenFlashTask);
        SoundDispatchCommand(SOUND_SFX_PLAY_CH1, Battle_ComputeScreenPanFromPosition(&g_BattleActorRecords[g_WeaponFxDiamondFlashBoneIndex].posX), 0x176, 0, 0, 0, 0, 0, 0);
    }
    if (frame.centroid->frame == 80) {
        SoundDispatchCommand(SOUND_SFX_PLAY_CH2, 0, 0, 0, 0, 0, 0, 0, 0);
        for (frame.actorIndex = 0; frame.actorIndex < 10; frame.actorIndex++) {
            if ((g_WeaponFxDiamondFlashActorIndex & 1 << frame.actorIndex) != 0) {
                Battle_PostActorHitOrHurtMessage(frame.actorIndex);
            }
        }
    }
    (frame.centroid->frame)++;
    if (frame.centroid->frame == 90) {
        Battle_StartFixedPointRamp(0, 10);
        frame.centroid->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Submits oriented model 0 with pulsating sinusoidal scale and intensity.
 */
// FUNCTION: FF7 0x0044357d
void __cdecl BattleFx_WeaponDiamondFlashPulsingOrientedModelTask(void)
{
    BattleFxDiamondFlashPulsingSlot *task;

    task = (BattleFxDiamondFlashPulsingSlot *)BATTLE_CURRENT_TASK_SLOT;
    (g_WeaponFxDiamondFlashPulsingOrientedDrawReq.model) = Gfx_ModelTableGetRenderObject(0, g_WeaponFxDiamondFlashResourceContext);
    BattleFx_InstallOrientedMatrixUniform(&task->posX, (int *)&g_WeaponFxDiamondFlashPulsingInwardDrawReqComposite, SinFixed(task->frameCounter * 0x33) * 0x6000 /* 6.0 */ >> 12, 0xFFFFFC18);
    SetDrawColorRGB(0, 0, 0);
    g_WeaponFxDiamondFlashPulsingOrientedDrawReq.param = SinFixed(task->frameCounter * 51) << 12 >> 12;
    g_BattleOverlayDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_WeaponFxDiamondFlashPulsingOrientedDrawReq, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused != 0) {
        return;
    }
    (task->frameCounter)++;
    if (task->frameCounter == 20) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Submits billboard model 2 with oscillating scale and fades over 80 frames.
 */
// FUNCTION: FF7 0x00443668
void __cdecl BattleFx_WeaponDiamondFlashPulsingBillboardModelTask(void)
{
    BattleFxDiamondFlashPulsingSlot *task;

    task = (BattleFxDiamondFlashPulsingSlot *)BATTLE_CURRENT_TASK_SLOT;
    g_WeaponFxDiamondFlashPulsingInwardDrawReqComposite.billboardDrawReq = Gfx_ModelTableGetRenderObject(2, g_WeaponFxDiamondFlashResourceContext);
    BattleFx_InstallBillboardMatrix(&task->posX, SinFixed(task->frameCounter * 0xC) * 0x6000 /* 6.0 */ >> 12, 0xFFFFFC18);
    SetDrawColorRGB(0, 0, 0);
    g_WeaponFxDiamondFlashPulsingInwardDrawReqComposite.billboardScaleParam = 0x1000 - (SinFixed(task->frameCounter * 0x19) << 12 >> 12);
    g_BattleOverlayDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_WeaponFxDiamondFlashPulsingInwardDrawReqComposite.billboardDrawReq, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused != 0) {
        return;
    }
    (task->frameCounter)++;
    if (task->frameCounter == 80) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Moves model 1 inward along its velocity vector and submits it.
 */
// FUNCTION: FF7 0x00443756
void __cdecl BattleFx_WeaponDiamondFlashInwardModelTask(void)
{
    BattleFxWeaponDiaInwardModel *task;

    task = (BattleFxWeaponDiaInwardModel *)BATTLE_CURRENT_TASK_SLOT;
    g_WeaponFxDiamondFlashPulsingInwardDrawReqComposite.inwardModelDrawReq = Gfx_ModelTableGetRenderObject(1, g_WeaponFxDiamondFlashResourceContext);
    BattleFx_InstallOrientedMatrixUniform(&task->posX, &g_WeaponFxDiamondFlashInwardOrientationAngle, SinFixed(task->frame * 0x44) << 11 >> 12, 0xFFFFFC18);
    SetDrawColorRGB(0, 0, 0);
    g_BattleOverlayDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_WeaponFxDiamondFlashPulsingInwardDrawReqComposite.inwardModelDrawReq, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused != 0) {
        return;
    }
    task->posX += task->velocityX;
    task->posY += task->velocityY;
    task->posZ += task->velocityZ;
    (task->frame)++;
    if (task->frame == 15) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Steps rotating converging model 3 toward target actor and submits it.
 */
// FUNCTION: FF7 0x0044385e
void __cdecl BattleFx_WeaponDiamondFlashConvergingModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[112];
        PsxMatrix rotMatrix;
        struct { unsigned char pad_to_20[0x20]; PsxMatrix scaleMatrix; };
        struct { unsigned char pad_to_44[0x44]; BattleFxDiaConvergeSlot *convergeSlot; };
        struct { unsigned char pad_to_4C[0x4C]; PsxMatrix drawMatrix; };
    } frame;
#pragma pack(pop)

    frame.convergeSlot = (BattleFxDiaConvergeSlot *)BATTLE_CURRENT_TASK_SLOT;
    g_WeaponFxDiamondFlashConvergingDrawReq.modelDrawReq = Gfx_ModelTableGetRenderObject(3, g_WeaponFxDiamondFlashResourceContext);
    RotMatrixZYX((SVECTOR *)&frame.convergeSlot->angleX, &frame.rotMatrix);
    g_WeaponFxDiamondFlashProjectedPos.posX = 0;
    g_WeaponFxDiamondFlashProjectedPos.posY = 0;
    g_WeaponFxDiamondFlashProjectedPosZ = frame.convergeSlot->projectedPosZ;
    TransformVectorToInt16(&frame.rotMatrix, &g_WeaponFxDiamondFlashProjectedPos, &g_WeaponFxDiamondFlashProjectedPos);
    g_WeaponFxDiamondFlashProjectedPos.posX += g_WeaponFxDiamondFlashActorPos.posX;
    g_WeaponFxDiamondFlashProjectedPos.posY += g_WeaponFxDiamondFlashActorPos.posY;
    g_WeaponFxDiamondFlashProjectedPosZ += g_WeaponFxDiamondFlashActorPosZ;
    RotMatrixXYZ(&g_WeaponFxDiamondFlashConvergingScaleFactor, &frame.scaleMatrix);
    ConcatMatrixPreservingTranslation(&frame.rotMatrix, &frame.scaleMatrix, &frame.scaleMatrix);
    ScaleMatrix(&frame.scaleMatrix, (int *)&g_WeaponFxDiamondFlashConvergingDrawReq);
    frame.scaleMatrix.t[0] = g_WeaponFxDiamondFlashProjectedPos.posX;
    frame.scaleMatrix.t[1] = g_WeaponFxDiamondFlashProjectedPos.posY;
    frame.scaleMatrix.t[2] = g_WeaponFxDiamondFlashProjectedPosZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.scaleMatrix, &frame.drawMatrix);
    SetRotMatrix(&frame.drawMatrix);
    SetTransMatrix(&frame.drawMatrix);
    SetDrawColorRGB(0, 0, 0);
    g_WeaponFxDiamondFlashConvergingDrawReq.angleParam = CosFixed(frame.convergeSlot->frameCount * 0x44) << 12 >> 12;
    g_BattleOverlayDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_WeaponFxDiamondFlashConvergingDrawReq.modelDrawReq, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.convergeSlot->projectedPosZ -= 533;
    (frame.convergeSlot->frameCount)++;
    if (frame.convergeSlot->frameCount == 15) {
        frame.convergeSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Updates decelerating billboard sprite with distance damping and expansion.
 */
// FUNCTION: FF7 0x00443a2f
void __cdecl BattleFx_WeaponDiamondFlashDampedSpriteTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[12];
        int fieldIdA[2];
        struct { unsigned char pad_to_8[0x8]; BattleFxWeaponDiamondFlashSpriteTask *spriteTask; };
    } frame;
#pragma pack(pop)
    frame.spriteTask = (BattleFxWeaponDiamondFlashSpriteTask *)BATTLE_CURRENT_TASK_SLOT;
    frame.fieldIdA[0] = 0x10;
    frame.fieldIdA[1] = 0x11;
    BattleFx_InstallBillboardMatrix(&frame.spriteTask->posX, 0x4000 /* 4.0 */, 0);
    (*(int *)&g_WeaponFxDiamondFlashDampedSpriteReq) = Gfx_ModelTableGetLoadedRecord(frame.fieldIdA[frame.spriteTask->tableIndex], g_WeaponFxDiamondFlashResourceContext);
    (g_WeaponFxDiamondFlashDampedSpriteReq.drawObj) = Gfx_ModelTableGetSpriteObject(frame.fieldIdA[frame.spriteTask->tableIndex], g_WeaponFxDiamondFlashResourceContext);
    (g_WeaponFxDiamondFlashDampedSpriteReq.frame) = frame.spriteTask->frameIndex >> 1;
    ((unsigned char *)&g_WeaponFxDiamondFlashDampedSpriteReq)[10] = frame.spriteTask->timer * 0xA;
    ((unsigned char *)&g_WeaponFxDiamondFlashDampedSpriteReq)[9] = ((unsigned char *)&g_WeaponFxDiamondFlashDampedSpriteReq)[10];
    ((unsigned char *)&g_WeaponFxDiamondFlashDampedSpriteReq)[8] = ((unsigned char *)&g_WeaponFxDiamondFlashDampedSpriteReq)[9];
    g_BattleOverlayDrawListHead = BattleFx_SubmitSpriteDraw(&(*(int *)&g_WeaponFxDiamondFlashDampedSpriteReq), g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.spriteTask->velocityX -= frame.spriteTask->velocityX / 8;
    frame.spriteTask->velocityY -= frame.spriteTask->velocityY / 8;
    frame.spriteTask->velocityZ -= frame.spriteTask->velocityZ / 8;
    frame.spriteTask->posX += frame.spriteTask->velocityX;
    frame.spriteTask->posY += frame.spriteTask->velocityY;
    frame.spriteTask->posZ += frame.spriteTask->velocityZ;
    if (g_WeaponFxDiamondFlashSpriteExpandActive != 0) {
        (frame.spriteTask->timer)--;
        if (frame.spriteTask->timer == 0) {
            frame.spriteTask->ownerSlot = BATTLE_TASK_FREE;
            return;
        }
        g_WeaponFxDiamondFlashSpriteDeltaPos[0] = frame.spriteTask->posX - g_WeaponFxDiamondFlashCentroidPos.posX;
        g_WeaponFxDiamondFlashSpriteDeltaPos[1] = frame.spriteTask->posY - g_WeaponFxDiamondFlashCentroidPos.posY;
        g_WeaponFxDiamondFlashSpriteDeltaPos[2] = 0;
        g_WeaponFxDiamondFlashSpriteDistance = SqrtToInt(g_WeaponFxDiamondFlashSpriteDeltaPos[0] * g_WeaponFxDiamondFlashSpriteDeltaPos[0] + g_WeaponFxDiamondFlashSpriteDeltaPos[1] * g_WeaponFxDiamondFlashSpriteDeltaPos[1]);
        if (g_WeaponFxDiamondFlashSpriteDistance == 0) {
            frame.spriteTask->ownerSlot = BATTLE_TASK_FREE;
            return;
        }
        if (g_WeaponFxDiamondFlashSpriteDistance < 2000) {
            g_WeaponFxDiamondFlashSpriteDistance = (2000 - g_WeaponFxDiamondFlashSpriteDistance) * 400 / 2000;
            VectorNormalSVectorToSVector(&g_WeaponFxDiamondFlashSpriteDeltaPos[0], &g_WeaponFxDiamondFlashSpriteDeltaPos[0]);
            frame.spriteTask->posX += (short)(g_WeaponFxDiamondFlashSpriteDeltaPos[0] * g_WeaponFxDiamondFlashSpriteDistance >> 12);
            frame.spriteTask->posY += (short)(g_WeaponFxDiamondFlashSpriteDeltaPos[1] * g_WeaponFxDiamondFlashSpriteDistance >> 12);
        }
    }
    if ((*(int *)&g_WeaponFxDiamondFlashDampedSpriteReq) != 0) {
        (frame.spriteTask->frameIndex)++;
        if (frame.spriteTask->frameIndex >= *(int *)((char *)*(int *)(*(int *)&g_WeaponFxDiamondFlashDampedSpriteReq) + 4)) {
            frame.spriteTask->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Submits three sequential streak models along task trajectory.
 */
// FUNCTION: FF7 0x00443d29
void __cdecl BattleFx_WeaponDiamondFlashTripleModelStreakTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[96];
        PsxMatrix lookBasis;
        struct { unsigned char pad_to_20[0x20]; int modelIds[6]; };
        struct { unsigned char pad_to_38[0x38]; short modelIndex; };
        struct { unsigned char pad_to_3C[0x3C]; BattleFxDiamondFlashStreak *streak; };
        struct { unsigned char pad_to_40[0x40]; PsxMatrix drawMatrix; };
    } frame;
#pragma pack(pop)

    frame.streak = (BattleFxDiamondFlashStreak *)BATTLE_CURRENT_TASK_SLOT;
    frame.modelIds[0] = 4;
    frame.modelIds[1] = 5;
    frame.modelIds[2] = 6;
    frame.modelIds[3] = 7;
    frame.modelIds[4] = 8;
    frame.modelIds[5] = 9;
    Gte_BuildLookBasisFromDirection(&frame.streak->velocityX, frame.lookBasis.m[0]);
    (g_WeaponFxDiamondFlashTripleModelScaleVector.scale) = (SinFixed(frame.streak->step << 7) << 12 >> 12) + 0x1000;
    ScaleMatrix(&frame.lookBasis, &(g_WeaponFxDiamondFlashTripleModelScaleVector.scaleX));
    frame.lookBasis.t[0] = frame.streak->posX;
    frame.lookBasis.t[1] = frame.streak->posY;
    frame.lookBasis.t[2] = frame.streak->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.lookBasis, &frame.drawMatrix);
    SetRotMatrix(&frame.drawMatrix);
    SetTransMatrix(&frame.drawMatrix);
    SetDrawColorRGB(0, 0, 0);
    for (frame.modelIndex = 0; frame.modelIndex < 3; frame.modelIndex++) {
        (*(int *)&g_WeaponFxDiamondFlashTripleModelDrawReq) = Gfx_ModelTableGetRenderObject(frame.modelIds[(frame.streak->tableIndex * 3 + frame.modelIndex)], g_WeaponFxDiamondFlashResourceContext);
        g_BattleOverlayDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_WeaponFxDiamondFlashTripleModelDrawReq, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.streak->posX += frame.streak->velocityX;
    frame.streak->posY += frame.streak->velocityY;
    frame.streak->posZ += frame.streak->velocityZ;
    (frame.streak->step)++;
    if (frame.streak->step == 8 || frame.streak->posY > 0) {
        frame.streak->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Updates billboard sprite position with deceleration and submits for draw.
 */
// FUNCTION: FF7 0x00443ef8
void __cdecl BattleFx_WeaponDiamondFlashBillboardSpriteTask(void)
{
    BattleFxDiamondFlashBillboardRecord *task;

    task = (BattleFxDiamondFlashBillboardRecord *)&g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx];
    BattleFx_InstallBillboardMatrix(&task->posX, 0x2800 /* 2.5 */, 0);
    (g_WeaponFxDiamondFlashBillboardSpriteReq.spriteTable) = Gfx_ModelTableGetLoadedRecord(18, g_WeaponFxDiamondFlashResourceContext);
    g_WeaponFxDiamondFlashBillboardSpriteReq.drawObj = Gfx_ModelTableGetSpriteObject(18, g_WeaponFxDiamondFlashResourceContext);
    g_WeaponFxDiamondFlashBillboardSpriteReq.count = task->spriteIndex;
    if (task->active != 0) {
        *(unsigned short *)&g_WeaponFxDiamondFlashBillboardSpriteReq.count |= 0x8000;
        g_WeaponFxDiamondFlashBillboardSpriteReq.param = 0x40;
    }
    g_BattleOverlayDrawListHead = BattleFx_SubmitSpriteDraw(&g_WeaponFxDiamondFlashBillboardSpriteReq, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused != 0) {
        return;
    }
    task->velX -= (short)(task->velX / 8);
    task->velY -= (short)(task->velY / 8);
    task->velZ -= (short)(task->velZ / 8);
    task->posX += task->velX;
    task->posY += task->velY;
    task->posZ += task->velZ;
    if ((g_WeaponFxDiamondFlashBillboardSpriteReq.spriteTable) != 0) {
        (task->spriteIndex)++;
        if (task->spriteIndex >= *(int *)((char *)*(int *)(g_WeaponFxDiamondFlashBillboardSpriteReq.spriteTable) + 4)) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Submits models 0xD and 0xE with scale reduction over 20 frames.
 */
// FUNCTION: FF7 0x00444093
void __cdecl BattleFx_WeaponDiamondFlashShrinkingModelPairTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[92];
        int clampedStep;
        struct { unsigned char pad_to_4[0x4]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_24[0x24]; short stepShort; };
        struct { unsigned char pad_to_28[0x28]; BattleFxCosmoMemoryTriSpin *shrinkRecord; };
        struct { unsigned char pad_to_2C[0x2C]; PsxMatrix drawMatrix; };
        struct { unsigned char pad_to_4C[0x4C]; VECTOR scaleVector; };
    } frame;
#pragma pack(pop)

    frame.shrinkRecord = (BattleFxCosmoMemoryTriSpin *)BATTLE_CURRENT_TASK_SLOT;
    RotMatrix((SVECTOR *)&g_WeaponFxDiamondFlashRotationAngles, &frame.rotMatrix);
    ConcatMatrixPreservingTranslation(&g_WeaponFxDiamondFlashMatrix, &frame.rotMatrix, &frame.rotMatrix);
    frame.scaleVector.vz = 0x6000 - frame.shrinkRecord->step * 0x1C2;
    frame.scaleVector.vx = frame.scaleVector.vz;
    if (frame.shrinkRecord->step <= 6) {
        frame.clampedStep = frame.shrinkRecord->step;
    } else {
        frame.clampedStep = 6;
    }
    frame.stepShort = *(short *)&frame.clampedStep;
    frame.scaleVector.vy = frame.stepShort * 0x708;
    ScaleMatrix(&frame.rotMatrix, &frame.scaleVector);
    frame.rotMatrix.t[0] = frame.shrinkRecord->posX;
    frame.rotMatrix.t[1] = frame.shrinkRecord->posY;
    frame.rotMatrix.t[2] = frame.shrinkRecord->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.drawMatrix);
    SetRotMatrix(&frame.drawMatrix);
    SetTransMatrix(&frame.drawMatrix);
    SetDrawColorRGB(0, 0, 0);
    (g_WeaponFxDiamondFlashShrinkingModelDrawReq.shrinkAmount) = SinFixed(frame.shrinkRecord->step * 51) << 11 >> 12;
    (g_WeaponFxDiamondFlashShrinkingModelDrawReq.model) = Gfx_ModelTableGetRenderObject(13, g_WeaponFxDiamondFlashResourceContext);
    g_BattleOverlayDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&(g_WeaponFxDiamondFlashShrinkingModelDrawReq.model), g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    (g_WeaponFxDiamondFlashShrinkingModelDrawReq.model) = Gfx_ModelTableGetRenderObject(14, g_WeaponFxDiamondFlashResourceContext);
    g_BattleOverlayDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&(g_WeaponFxDiamondFlashShrinkingModelDrawReq.model), g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused != 0) {
        return;
    }
    (frame.shrinkRecord->step)++;
    if (frame.shrinkRecord->step == 20) {
        frame.shrinkRecord->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Updates projectile motion, emits trail model task, and submits draw requests.
 */
// FUNCTION: FF7 0x00444256
void __cdecl BattleFx_WeaponDiamondFlashTrailingProjectileTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[84];
        struct { unsigned char pad_to_C[0xC]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_2C[0x2C]; BattleFxWeaponDiamondFlashTrailPoint *trailPoint; };
        struct { unsigned char pad_to_30[0x30]; BattleFxDiamondFlashTrail *trail; };
        struct { unsigned char pad_to_34[0x34]; PsxMatrix drawMatrix; };
    } frame;
#pragma pack(pop)

    frame.trail = (BattleFxDiamondFlashTrail *)BATTLE_CURRENT_TASK_SLOT;
    RotMatrix(&g_WeaponFxDiamondFlashTrailingRotationVector, &frame.rotMatrix);
    ConcatMatrixPreservingTranslation(&g_WeaponFxDiamondFlashMatrix, &frame.rotMatrix, &frame.rotMatrix);
    ScaleMatrix(&frame.rotMatrix, (int *)&g_WeaponFxDiamondFlashTrailingDrawReqComposite);
    frame.rotMatrix.t[0] = frame.trail->posA;
    frame.rotMatrix.t[1] = frame.trail->posB;
    frame.rotMatrix.t[2] = frame.trail->posC;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.drawMatrix);
    SetRotMatrix(&frame.drawMatrix);
    SetTransMatrix(&frame.drawMatrix);
    g_WeaponFxDiamondFlashTrailingDrawReqComposite.cosParam = CosFixed(frame.trail->step * 51) << 7 >> 12;
    g_WeaponFxDiamondFlashTrailingDrawReqComposite.spriteRecord = Gfx_ModelTableGetLoadedRecord(19, g_WeaponFxDiamondFlashResourceContext);
    g_WeaponFxDiamondFlashTrailingDrawReqComposite.spriteObject = Gfx_ModelTableGetSpriteObject(19, g_WeaponFxDiamondFlashResourceContext);
    g_WeaponFxDiamondFlashTrailingDrawReqComposite.modelResource = Gfx_ModelTableGetRenderObject(10, g_WeaponFxDiamondFlashResourceContext);
    g_BattleOverlayDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_WeaponFxDiamondFlashTrailingDrawReqComposite.modelResource, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    g_WeaponFxDiamondFlashTrailingDrawReqComposite.modelResource = Gfx_ModelTableGetRenderObject(11, g_WeaponFxDiamondFlashResourceContext);
    g_BattleOverlayDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_WeaponFxDiamondFlashTrailingDrawReqComposite.modelResource, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    g_WeaponFxDiamondFlashTrailingDrawReqComposite.modelResource = Gfx_ModelTableGetRenderObject(12, g_WeaponFxDiamondFlashResourceContext);
    g_BattleOverlayDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_WeaponFxDiamondFlashTrailingDrawReqComposite.modelResource, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    BattleFx_InstallBillboardMatrix(&frame.trail->posA, 0x4000 /* 4.0 */, 0);
    g_WeaponFxDiamondFlashTrailingSpriteParams.b = SinFixed(frame.trail->step * 51) << 7 >> 12;
    g_WeaponFxDiamondFlashTrailingSpriteParams.g = g_WeaponFxDiamondFlashTrailingSpriteParams.b;
    g_WeaponFxDiamondFlashTrailingSpriteParams.r = g_WeaponFxDiamondFlashTrailingSpriteParams.g;
    g_WeaponFxDiamondFlashTrailingSpriteParams.frame = (frame.trail->step % 2);
    g_BattleOverlayDrawListHead = BattleFx_SubmitSpriteDraw(&g_WeaponFxDiamondFlashTrailingDrawReqComposite.spriteRecord, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.trail->posA += frame.trail->velocityA;
    frame.trail->posB += frame.trail->velocityB;
    frame.trail->posC += frame.trail->velocityC;
    frame.trailPoint = (BattleFxWeaponDiamondFlashTrailPoint *)BATTLE_NEW_TASK_SLOT(BattleFx_WeaponDiamondFlashTrailModelTask);
    frame.trailPoint->posA = frame.trail->posA;
    frame.trailPoint->posB = frame.trail->posB;
    frame.trailPoint->posC = frame.trail->posC;
    (frame.trail->step)++;
    if (frame.trail->step == 6) {
        frame.trail->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Renders diminishing trail model behind projectile over 20 frames.
 */
// FUNCTION: FF7 0x00444531
void __cdecl BattleFx_WeaponDiamondFlashTrailModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[84];
        PsxMatrix rotMatrix;
        struct { unsigned char pad_to_20[0x20]; BattleFxEnergyOrbTrailModel *trailRecord; };
        struct { unsigned char pad_to_24[0x24]; PsxMatrix drawMatrix; };
        struct { unsigned char pad_to_44[0x44]; VECTOR scaleVector; };
    } frame;
#pragma pack(pop)

    frame.trailRecord = (BattleFxEnergyOrbTrailModel *)BATTLE_CURRENT_TASK_SLOT;
    g_WeaponFxDiamondFlashTrailModelDrawReq.resource = Gfx_ModelTableGetRenderObject(15, g_WeaponFxDiamondFlashResourceContext);
    if (frame.trailRecord->frameCount > 2) {
        RotMatrix((SVECTOR *)&g_WeaponFxDiamondFlashTrailModelDrawReq, &frame.rotMatrix);
        ConcatMatrixPreservingTranslation(&g_WeaponFxDiamondFlashMatrix, &frame.rotMatrix, &frame.rotMatrix);
        frame.scaleVector.vz = 0x6000 - frame.trailRecord->frameCount * 0x258;
        frame.scaleVector.vy = frame.scaleVector.vz;
        frame.scaleVector.vx = frame.scaleVector.vy;
        ScaleMatrix(&frame.rotMatrix, &frame.scaleVector);
        frame.rotMatrix.t[0] = frame.trailRecord->posX;
        frame.rotMatrix.t[1] = frame.trailRecord->posY;
        frame.rotMatrix.t[2] = frame.trailRecord->posZ;
        MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.drawMatrix);
        SetRotMatrix(&frame.drawMatrix);
        SetTransMatrix(&frame.drawMatrix);
        SetDrawColorRGB(0, 0, 0);
        g_WeaponFxDiamondFlashTrailModelDrawReq.param = SinFixed(frame.trailRecord->frameCount * 51) << 12 >> 12;
        g_BattleOverlayDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_WeaponFxDiamondFlashTrailModelDrawReq.resource, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    (frame.trailRecord->frameCount)++;
    if (frame.trailRecord->frameCount == 20) {
        frame.trailRecord->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Applies full-screen sinusoidal white flash over 30 frames.
 */
// FUNCTION: FF7 0x0044469c
void __cdecl BattleFx_WeaponDiamondFlashScreenFlashTask(void)
{
    BattleFxPhaseControllerTask *taskPtr;
    unsigned short brightness;

    taskPtr = (BattleFxPhaseControllerTask *)BATTLE_CURRENT_TASK_SLOT;
    brightness = SinFixed(taskPtr->frameCount * 68) * 0xFF >> 12;
    g_BattleOverlayDrawListHead = BattleFx_FlashScreenWhite((char)brightness, (char)brightness, (char)brightness);
    if (g_BattleIsPaused != 0) {
        return;
    }
    (taskPtr->frameCount)++;
    if (taskPtr->frameCount == 30) {
        taskPtr->stage = 0xFFFF;
    }
}
} /* extern "C" */
