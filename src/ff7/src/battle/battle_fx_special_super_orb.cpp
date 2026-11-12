// src/ff7/src/battle/battle_fx_special_super_orb.cpp
/* Enemy Special Attack [141]: Super Orb / Gravity Ball (orig. SPECIAL/TYOKYU, 超球) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle special effect Tyokyu: loads its resources, runs a timeline that spawns
//   callbacks and sounds, and runs the screen flash, model, sprite and trail tasks that draw it.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/types/battle.h"
#include "lib/types/graphics.h"
#include "lib/types/polygon.h"
#include "ff7/camera_script.h"
#include <stdlib.h>
#include <string.h>
#include "lib/codegen.h"
#include "ff7/api/battle.h"
#include "ff7/api/menu.h"
#include "lib/api/sound.h"
#include "lib/api/graphics.h"
#include "ff7/battle_tasks.h"
#include "ff7/types/battle_special.h"
#include "ff7/types/battle_3d.h"
extern "C" {

/* The first 12 bytes of a BattleFxModelDraw: the model, flags, UV scroll and fade level. Its
 * stateFlags word is the next definition in .data. */
typedef struct {
    int modelGroup;
    int flags;
    short uvScroll;
    short fadeLevel;
} BattleFxSuperOrbModelReq;

/* The first 8 bytes of a BattleFxModelDraw: the model and flags; the rest is the next definition. */
typedef struct {
    int modelGroup;
    int flags;
} BattleFxSuperOrbModelReqWords;

/* The first 8 bytes of a BattleFxSpriteRequest: the sprite group and its draw object. */
typedef struct {
    int spriteGroup;
    int drawObj;
} BattleFxSuperOrbSpriteHead;

/* The last 8 bytes of a BattleFxSpriteRequest: the vertex colour, primitive code and frame. */
typedef struct {
    char colorR;
    char colorG;
    char colorB;
    unsigned char code;
    unsigned short frame;
    unsigned short uvPacked;
} BattleFxSuperOrbSpriteTail;

/* The expanding model's angles, then the head of the bouncing sprite's request (its tail is
 * g_BattleSpecialSuperOrbColor1). */
typedef struct {
    SVECTOR rotation;
    BattleFxSuperOrbSpriteHead sprite;
} BattleFxSuperOrbRotatedSprite;

/* The falling models' scale, then the head of their model request. */
typedef struct {
    VECTOR scale;
    BattleFxSuperOrbModelReq draw;
} BattleFxSuperOrbScaledModel;

/* The growing model's angles, then the head of its model request. */
typedef struct {
    SVECTOR rotation;
    BattleFxSuperOrbModelReq draw;
} BattleFxSuperOrbRotatedModel;

/* The trail model's angles and request, the drifting sprite's request and the head of the glow
 * sprite's request (its frame is g_BattleSpecialSuperOrbGlowSpriteFrame). */
typedef struct {
    SVECTOR rotation;
    BattleFxModelDrawHead trailDraw;
    BattleFxSpriteRequest driftSprite;
    BattleFxSuperOrbSpriteHead glowSprite;
} BattleFxSuperOrbRequestBlock;

/* The projectile's scale, model request and the head of its sprite request (its tail is
 * g_BattleSpecialSuperOrbColor2). */
typedef struct {
    VECTOR scale;
    BattleFxModelDrawHead draw;
    BattleFxSuperOrbSpriteHead sprite;
} BattleFxSuperOrbProjectileRequests;

/* The projectile's angles, then the decelerating sprite's request. */
typedef struct {
    SVECTOR rotation;
    BattleFxSpriteRequest sprite;
} BattleFxSuperOrbRotatedSpriteRequest;

/* The look basis of the source-to-target direction, with the projectile's position as translation. */
typedef struct {
    PsxMatrix matrix;
    unsigned char pad_1e[0x2];
} BattleFxSuperOrbBasis;

// clang-format off
// GLOBAL: FF7 0x0082fc90
unsigned int g_BattleSpecialSuperOrbResourceDefs[93] = {0x001940b1, (unsigned int)&g_BattleSpecialSuperOrbResFilenameHado, 0x00000000, 0x00000000, 0x00000000, 0x001940b1, (unsigned int)&g_BattleSpecialSuperOrbResFilenameHado[0xc], 0x00000000, 0x00000000, 0x00000000, 0x001940b1, (unsigned int)&g_BattleSpecialSuperOrbResFilenameTama, 0x00000000, 0x00000000, 0x00000000, 0x001940b1, (unsigned int)&g_BattleSpecialSuperOrbResFilenameTama[0xc], 0x00000000, 0x00000000, 0x00000000, 0x003240b1, (unsigned int)&g_BattleSpecialSuperOrbResFilenameMoto, 0x00000000, 0x00000000, 0x00000000, 0x001940b1, (unsigned int)&g_BattleSpecialSuperOrbResFilenameMoto[0xc], 0x00000000, 0x00000000, 0x00000000, 0x001940b1, (unsigned int)&g_BattleSpecialSuperOrbResFilenameSenkou, 0x00000000, 0x00000000, 0x00000000, 0x001940b1, (unsigned int)&g_BattleSpecialSuperOrbResFilenameSenkou[0xc], 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleSpecialSuperOrbResFilenameIwa1, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleSpecialSuperOrbResFilenameIwa1[0xc], 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleSpecialSuperOrbResFilenameIwa2, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleSpecialSuperOrbResFilenameIwa2[0xc], 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleSpecialSuperOrbResFilenameUchi, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleSpecialSuperOrbResFilenameUchi[0xc], 0x00000000, 0x00000000, 0x00000000, 0x00192a39, (unsigned int)&g_BattleSpecialSuperOrbResFilenameEnergy00, (unsigned int)&g_BattleSpecialSuperOrbResFilenameKou, 0x00000000, 0x00000000, 0x00192a39, (unsigned int)&g_BattleSpecialSuperOrbResFilenameEnergy03, (unsigned int)&g_BattleSpecialSuperOrbResFilenameKemu, 0x00000000, 0x00000000, 0x00192a39, (unsigned int)&g_BattleSpecialSuperOrbResFilenameKemu[0xc], (unsigned int)&g_BattleSpecialSuperOrbResFilenameKemu[0x1c], 0x00000000, 0x00000000, 0x00192a39, (unsigned int)&g_BattleSpecialSuperOrbResFilenameEnergy03B, (unsigned int)&g_BattleSpecialSuperOrbResFilenameBaku, 0x00000000, 0x00000000, 0x00192a39, (unsigned int)&g_BattleSpecialSuperOrbResFilenameBaku[0xc], (unsigned int)&g_BattleSpecialSuperOrbResFilenameBaku[0x1c]};
unsigned char g_rawData_0082fe08[24] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x40, 0x40, 0x2e, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x0082fe20
BattleFxSuperOrbModelReq g_BattleSpecialSuperOrbModelReqTama = {0, 0xa8, 0, 0x80};
unsigned char g_rawData_0082fe2c[4] = {0x20, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x0082fe30
BattleFxSuperOrbRotatedSprite g_BattleSpecialSuperOrbRotationTama = {{-0x400, 0, 0, 0}, {0, 0}};
// GLOBAL: FF7 0x0082fe40
BattleFxSuperOrbSpriteTail g_BattleSpecialSuperOrbColor1 = {(char)0x80, (char)0x80, (char)0x80, 0x2c, 0, 0};
// GLOBAL: FF7 0x0082fe48
BattleFxSuperOrbScaledModel g_BattleSpecialSuperOrbScaleTama = {{0x800, 0x800, 0x800, 0}, {0, 0, 0, 0}};
unsigned char g_rawData_0082fe64[4] = {0x20, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x0082fe68
BattleFxSuperOrbRotatedModel g_BattleSpecialSuperOrbRotationMoto = {{-0x400, 0, 0, 0}, {0, 0xa8, 0, 0}};
unsigned char g_rawData_0082fe7c[4] = {0x20, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x0082fe80
BattleFxSuperOrbRequestBlock g_BattleSpecialSuperOrbModelReqMoto = {
    {-0x400, 0, 0, 0},
    {0, 0xa8, 0, 0, 0x20, 0},
    {0, 0, (char)0x80, (char)0x80, (char)0x80, 0x2e, 0, 0},
    {0, 0}
};
unsigned char g_rawData_0082feb0[4] = {0x80, 0x80, 0x80, 0x2e};
// GLOBAL: FF7 0x0082feb4
short g_BattleSpecialSuperOrbGlowSpriteFrame = 128;
// GLOBAL: FF7 0x0082feb8
BattleFxSuperOrbModelReqWords g_BattleSpecialSuperOrbModelReqIwa = {0, 0xa8};
unsigned char g_rawData_0082fec0[8] = {0x00, 0x00, 0x80, 0x00, 0x20, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x0082fec8
short g_BattleSpecialSuperOrbStretchFadeCounter = 8;
// GLOBAL: FF7 0x0082fed0
BattleFxSuperOrbModelReq g_BattleSpecialSuperOrbModelReqUchi = {0, 0xa8, 0, 0x80};
unsigned char g_rawData_0082fedc[4] = {0x20, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x0082fee0
SVECTOR g_BattleSpecialSuperOrbRotationUchi = {-0x400, 0, 0, 0};
// GLOBAL: FF7 0x0082fee8
short g_BattleSpecialSuperOrbLifetimeCounter = 8;
// GLOBAL: FF7 0x0082fef0
BattleFxSuperOrbProjectileRequests g_BattleSpecialSuperOrbScaleTrajectory = {
    {0x3000, 0x3000, 0x3000, 0},
    {0, 0x28, 0, 0x80, 0x20, 0},
    {0, 0}
};
// GLOBAL: FF7 0x0082ff18
BattleFxSuperOrbSpriteTail g_BattleSpecialSuperOrbColor2 = {(char)0x80, (char)0x80, (char)0x80, 0x2e, 0, 0};
// GLOBAL: FF7 0x0082ff20
BattleFxSuperOrbRotatedSpriteRequest g_BattleSpecialSuperOrbRotationTrajectory = {{-0x400, 0, 0, 0}, {0, 0, (char)0x80, (char)0x80, (char)0x80, 0x2e, 0, 0}};
// GLOBAL: FF7 0x0082ff38
unsigned char g_BattleSpecialSuperOrbResFilenameHado[22] =
    "HADO.RSD\0\0\0\0"
    "HADO1.RSD";
// GLOBAL: FF7 0x0082ff50
unsigned char g_BattleSpecialSuperOrbResFilenameTama[22] =
    "TAMA1.RSD\0\0\0"
    "TAMA2.RSD";
// GLOBAL: FF7 0x0082ff68
unsigned char g_BattleSpecialSuperOrbResFilenameMoto[21] =
    "MOTO.RSD\0\0\0\0"
    "SOTO.RSD";
// GLOBAL: FF7 0x0082ff80
unsigned char g_BattleSpecialSuperOrbResFilenameSenkou[23] =
    "SENKOU1.RSD\0"
    "HIKARI.RSD";
// GLOBAL: FF7 0x0082ff98
unsigned char g_BattleSpecialSuperOrbResFilenameIwa1[21] =
    "IWA1.RSD\0\0\0\0"
    "IWA2.RSD";
// GLOBAL: FF7 0x0082ffb0
unsigned char g_BattleSpecialSuperOrbResFilenameIwa2[21] =
    "IWA3.RSD\0\0\0\0"
    "IWA4.RSD";
// GLOBAL: FF7 0x0082ffc8
unsigned char g_BattleSpecialSuperOrbResFilenameUchi[22] =
    "UCHI1.RSD\0\0\0"
    "UCHI2.RSD";
// GLOBAL: FF7 0x0082ffe0
unsigned char g_BattleSpecialSuperOrbResFilenameEnergy00[13] = "ENERGY00.TIM";
// GLOBAL: FF7 0x0082fff0
unsigned char g_BattleSpecialSuperOrbResFilenameKou[8] = "KOU.SPT";
// GLOBAL: FF7 0x0082fff8
unsigned char g_BattleSpecialSuperOrbResFilenameEnergy03[13] = "ENERGY03.TIM";
// GLOBAL: FF7 0x00830008
unsigned char g_BattleSpecialSuperOrbResFilenameKemu[39] =
    "KEMU11.SPT\0\0"
    "ENERGY00.TIM\0\0\0\0"
    "HAHEN1.SPT";
// GLOBAL: FF7 0x00830030
unsigned char g_BattleSpecialSuperOrbResFilenameEnergy03B[13] = "ENERGY03.TIM";
// GLOBAL: FF7 0x00830040
unsigned char g_BattleSpecialSuperOrbResFilenameBaku[36] =
    "BAKU1.SPT\0\0\0"
    "ENERGY00.TIM\0\0\0\0"
    "KYU.SPT";
// clang-format on
// GLOBAL: FF7 0x00b44e60
BattleFxSuperOrbBasis g_BattleSpecialSuperOrbBasisMatrix = {0};
unsigned char g_bssPad_00b44e80[8] = {0};
// GLOBAL: FF7 0x00b44e88
int g_BattleSpecialSuperOrbScaleX = 0;
// GLOBAL: FF7 0x00b44e8c
int g_BattleSpecialSuperOrbScaleY = 0;
// GLOBAL: FF7 0x00b44e90
int g_BattleSpecialSuperOrbScaleZ = 0;
unsigned char g_bssPad_00b44e94[4] = {0};
// GLOBAL: FF7 0x00b44e98
struct BattleSpecialScorchCentroidPos g_BattleSpecialSuperOrbCentroidPos = {0};
// GLOBAL: FF7 0x00b44e9c
short g_BattleSpecialSuperOrbCentroidZ = 0;
// GLOBAL: FF7 0x00b44ea0
int g_BattleSpecialSuperOrbDrawListHandle = 0;
// GLOBAL: FF7 0x00b44ea4
short g_BattleSpecialSuperOrbTargetMask = 0;
unsigned char g_bssPad_00b44ea8[16] = {0};
// GLOBAL: FF7 0x00b44eb8
short g_BattleSpecialSuperOrbRandomAngle = 0;
// GLOBAL: FF7 0x00b44ebc
struct GfxTableCursor *g_BattleSpecialSuperOrbLoadedResTable = 0;
// GLOBAL: FF7 0x00b44ec0
short g_BattleSpecialSuperOrbFlashIntensity = 0;
unsigned char g_bssPad_00b44ec4[4] = {0};
// GLOBAL: FF7 0x00b44ec8
int g_BattleSpecialSuperOrbRotationVector = 0;
// GLOBAL: FF7 0x00b44ecc
short g_BattleSpecialSuperOrbRotationRoll = 0;
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

/*
 * Loads the SPECIAL/TYOKYU resources once the battle scene exists and they are not loaded yet.
 */
// FUNCTION: FF7 0x004db2e0
void __cdecl BattleFx_SpecialLoadResSuperOrb(void)
{
    GfxModelTableBank *rendererField;
    GfxResourceRequest requestRecord;
    int typeId;
    char pathBuffer[200];
    GfxResourceRequest *request;
    int resDescList;

    if (g_pBattle3dScene != 0 && g_BattleSpecialSuperOrbLoadedResTable == 0) {
        typeId = 0x13;
        resDescList = (int)&g_BattleSpecialSuperOrbResourceDefs;
        rendererField = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        request = &requestRecord;
        Gfx_InitResourceRequest(request);
        request->groupTag = 0;
        // STRING: FF7 0x00830064
        request->resourcePath = BattleFx_BuildResourcePath("SPECIAL/TYOKYU", pathBuffer);
        Gfx_LoadModelTableIfAbsent(typeId, resDescList, request, &g_BattleSpecialSuperOrbLoadedResTable, rendererField);
    }
}

/*
 * Loads Tyokyu resources and forwards both arguments to BattleFx_SpecialBeginSuperOrb.
 */
// FUNCTION: FF7 0x004db370
void __cdecl BattleFx_SpecialEnterSuperOrb(int targetMask, int actorIdx)
{
    BattleFx_SpecialLoadResSuperOrb();
    BattleFx_SpecialBeginSuperOrb(targetMask, actorIdx);
}

/*
 * Stores the target mask, starts the timeline task at the source actor's bone, aims it at the
 * centroid of the targets (the look basis and a velocity of 600 along the direction), resets the
 * shared flash color to black and starts a sixteen-step ramp.
 */
// FUNCTION: FF7 0x004db38a
void __cdecl BattleFx_SpecialBeginSuperOrb(int targetMask, int actorIdx)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[28];
        VECTOR dirX;
        struct { unsigned char pad_to_10[0x10]; BattleFxScorchParticle *particle; };
        struct { unsigned char pad_to_14[0x14]; SVECTOR deltaX; };
    } frame;
#pragma pack(pop)

    g_BattleSpecialSuperOrbTargetMask = targetMask;
    frame.particle = (BattleFxScorchParticle *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialSuperOrbTimelineTask);
    Battle_GetBoneWorldPos(actorIdx, g_BattleActorRecords[actorIdx].bone3a, &frame.particle->posX);
    Battle_GetTargetMaskBoneCentroid((short)targetMask, (Vec3sPacked *)&g_BattleSpecialSuperOrbCentroidPos);
    frame.deltaX.vx = (g_BattleSpecialSuperOrbCentroidPos.centroidX) - frame.particle->posX;
    frame.dirX.vx = frame.deltaX.vx;
    frame.deltaX.vy = g_BattleSpecialSuperOrbCentroidPos.y - frame.particle->posY;
    frame.dirX.vy = frame.deltaX.vy;
    frame.deltaX.vz = g_BattleSpecialSuperOrbCentroidZ - frame.particle->posZ;
    frame.dirX.vz = frame.deltaX.vz;
    Gte_BuildLookBasisFromDirection(&frame.deltaX.vx, g_BattleSpecialSuperOrbBasisMatrix.matrix.m[0]);
    NormalizeVectorFixed(&frame.dirX, &frame.dirX);
    frame.particle->velocityX = frame.dirX.vx * 0x258 >> 12;
    frame.particle->velocityY = frame.dirX.vy * 0x258 >> 12;
    frame.particle->velocityZ = frame.dirX.vz * 0x258 >> 12;
    frame.particle->actorIdx = actorIdx;
    g_BattleFxTimelineState.channel2 = 0;
    g_BattleFxTimelineState.channel1 = g_BattleFxTimelineState.channel2;
    g_BattleFxTimelineState.red = g_BattleFxTimelineState.channel1;
    Battle_StartFixedPointRamp(0x1000, 16);
}

/*
 * Runs the 70-frame timeline: spawns the growing, counter-rotating and expanding models and the
 * sound at the start, radial sprites during the first 16 frames, the stretching models and the
 * projectile at frame 20, drifting sprites on the targets in frames 37 to 43 and their typed
 * records at frame 40. Starts the fade at frame 54 and shakes the camera after frame 20.
 */
// FUNCTION: FF7 0x004db4d2
void __cdecl BattleFx_SpecialSuperOrbTimelineTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[72];
        SVECTOR angleX;
        struct { unsigned char pad_to_8[0x8]; int rotTransOut; };
        struct { unsigned char pad_to_C[0xC]; SVECTOR pointX; };
        struct { unsigned char pad_to_14[0x14]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_34[0x34]; BattleFxSuperOrbOrbitRecord *spawnRecord; };
        struct { unsigned char pad_to_3C[0x3C]; short targetSlot; };
        struct { unsigned char pad_to_40[0x40]; short angle; };
        struct { unsigned char pad_to_44[0x44]; BattleFxSuperOrbRecord *taskRecord; };
    } frame;
#pragma pack(pop)

    frame.taskRecord = (BattleFxSuperOrbRecord *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.taskRecord->step == 0) {
        frame.spawnRecord = (BattleFxSuperOrbOrbitRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialSuperOrbGrowingModelTask);
        frame.spawnRecord->posX = frame.taskRecord->posX;
        frame.spawnRecord->posY = frame.taskRecord->posY;
        frame.spawnRecord->posZ = frame.taskRecord->posZ;
        frame.spawnRecord = (BattleFxSuperOrbOrbitRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialSuperOrbCounterRotatingModelsTask);
        frame.spawnRecord->posX = frame.taskRecord->posX;
        frame.spawnRecord->posY = frame.taskRecord->posY;
        frame.spawnRecord->posZ = frame.taskRecord->posZ;
        frame.spawnRecord = (BattleFxSuperOrbOrbitRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialSuperOrbExpandingModelTask);
        frame.spawnRecord->posX = frame.taskRecord->posX;
        frame.spawnRecord->posY = frame.taskRecord->posY;
        frame.spawnRecord->posZ = frame.taskRecord->posZ;
        Battle_PlaySfxPannedToActor(0x18B, (short)frame.taskRecord->modelIndex, 0x40);
    }
    if (frame.taskRecord->step < 16) {
        for (frame.targetSlot = 0; frame.targetSlot < 4; frame.targetSlot++) {
            frame.spawnRecord = (BattleFxSuperOrbOrbitRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialSuperOrbDecelSpriteTask);
            frame.spawnRecord->posX = frame.taskRecord->posX;
            frame.spawnRecord->posY = frame.taskRecord->posY;
            frame.spawnRecord->posZ = frame.taskRecord->posZ;
            frame.angle = (rand() % 4096);
            frame.spawnRecord->offsetX = CosFixed(frame.angle) * 0x7D0 >> 12;
            frame.spawnRecord->offsetY = SinFixed(frame.angle) * 0x7D0 >> 12;
            frame.spawnRecord->radius = 2000;
        }
    }
    if (frame.taskRecord->step == 20) {
        frame.spawnRecord = (BattleFxSuperOrbOrbitRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialSuperOrbStretchingModelsTask);
        frame.spawnRecord->posX = frame.taskRecord->posX;
        frame.spawnRecord->posY = frame.taskRecord->posY;
        frame.spawnRecord->posZ = frame.taskRecord->posZ;
        frame.spawnRecord = (BattleFxSuperOrbOrbitRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialSuperOrbProjectileTask);
        frame.spawnRecord->posX = frame.taskRecord->posX;
        frame.spawnRecord->posY = frame.taskRecord->posY;
        frame.spawnRecord->posZ = frame.taskRecord->posZ;
        frame.spawnRecord->offsetX = frame.taskRecord->velocityX;
        frame.spawnRecord->offsetY = frame.taskRecord->velocityY;
        frame.spawnRecord->radius = frame.taskRecord->velocityZ;
        Battle_PlaySfxPannedToActor(0x176, (short)frame.taskRecord->modelIndex, 0x40);
    }
    if (frame.taskRecord->step == 20) {
        Battle_AllocateResourceSlot(BattleFx_SpecialSuperOrbScreenFlashTask);
    }
    if (frame.taskRecord->step == 54) {
        Battle_StartFixedPointRamp(0, 16);
    }
    if (frame.taskRecord->step > 36 && frame.taskRecord->step < 44) {
        for (frame.targetSlot = 0; frame.targetSlot < 0xA; frame.targetSlot++) {
            if ((g_BattleSpecialSuperOrbTargetMask & 1 << frame.targetSlot) != 0) {
                frame.spawnRecord = (BattleFxSuperOrbOrbitRecord *)&g_BattleTaskSlotBuffer.secondarySlots[(short)Battle_AllocateSecondaryResourceSlot(BattleFx_SpecialSuperOrbDriftingAnimatedSpriteTask)];
                Battle_GetBoneWorldPos(frame.targetSlot, g_BattleActorRecords[frame.targetSlot].targetBone, &frame.spawnRecord->posX);
                frame.pointX.vy = 100;
                frame.pointX.vz = 0;
                frame.pointX.vx = frame.pointX.vz;
                frame.angleX.vx = (rand() % 4096);
                frame.angleX.vz = (rand() % 4096);
                frame.angleX.vy = 0;
                RotMatrix(&frame.angleX, &frame.rotMatrix);
                frame.rotMatrix.t[2] = 0;
                frame.rotMatrix.t[1] = frame.rotMatrix.t[2];
                frame.rotMatrix.t[0] = frame.rotMatrix.t[1];
                SetRotMatrix(&frame.rotMatrix);
                SetTransMatrix(&frame.rotMatrix);
                RotTransPointToInt16(&frame.pointX, &frame.spawnRecord->offsetX, &frame.rotTransOut);
            }
        }
    }
    if (frame.taskRecord->step == 40) {
        for (frame.targetSlot = 0; frame.targetSlot < 0xA; frame.targetSlot++) {
            if ((g_BattleSpecialSuperOrbTargetMask & 1 << frame.targetSlot) != 0) {
                Battle_PostActorHitOrHurtMessage(frame.targetSlot);
            }
        }
        Battle_PlaySfxPannedToActor(0xFA, -1, 0x40);
    }
    if (frame.taskRecord->step > 20) {
        g_BattleCameraFocalPoint.focalPointY += (short)(16 - ((frame.taskRecord->step % 2) << 5));
    }
    (frame.taskRecord->step)++;
    if (frame.taskRecord->step == 70) {
        frame.taskRecord->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Draws a blue screen flash whose intensity rises by 15 per frame for 16 frames, then falls by 15
 * per frame after frame 34; frees its slot at frame 50.
 */
// FUNCTION: FF7 0x004db9d6
void __cdecl BattleFx_SpecialSuperOrbScreenFlashTask(void)
{
    BattleTaskSlot *task;

    task = BATTLE_CURRENT_TASK_SLOT;
    g_BattleOverlayDrawListHead = BattleFx_FlashScreenWhite(0, 0, *(char *)&g_BattleSpecialSuperOrbFlashIntensity);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (task->frameCounter < 16) {
        g_BattleSpecialSuperOrbFlashIntensity += 0xF;
    }
    if (task->frameCounter > 34) {
        g_BattleSpecialSuperOrbFlashIntensity -= 0xF;
    }
    (task->frameCounter)++;
    if (task->frameCounter == 50) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Draws model entry seven at the task position, growing by 0x320 per frame from 0x1000; frees its
 * slot at frame 20. It keeps drawing while paused.
 */
// FUNCTION: FF7 0x004dba78
void __cdecl BattleFx_SpecialSuperOrbExpandingModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[92];
        int projDepth;
        struct { unsigned char pad_to_4[0x4]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_28[0x28]; PsxMatrix scaleMatrix; };
        struct { unsigned char pad_to_48[0x48]; short axisIndex; };
        struct { unsigned char pad_to_50[0x50]; BattleTaskSlot *expandModel; };
    } frame;
#pragma pack(pop)

    frame.expandModel = BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecialSuperOrbModelReqTama.modelGroup = Gfx_ModelTableGetRenderObject(7, g_BattleSpecialSuperOrbLoadedResTable);
    for (frame.axisIndex = 0; frame.axisIndex < 3; frame.axisIndex++) {
        frame.scaleMatrix.m[2][frame.axisIndex] = 0;
        frame.scaleMatrix.m[1][frame.axisIndex] = 0;
        frame.scaleMatrix.m[0][frame.axisIndex] = 0;
    }
    SetRotMatrix(&g_BattleCameraTransform);
    SetTransMatrix(&g_BattleCameraTransform);
    RotTransPoint((SVECTOR *)frame.expandModel->data, frame.scaleMatrix.t, &frame.projDepth);
    frame.scaleMatrix.m[2][2] = frame.expandModel->frameCounter * 0x320 + 0x1000;
    frame.scaleMatrix.m[1][1] = frame.scaleMatrix.m[2][2];
    frame.scaleMatrix.m[0][0] = frame.scaleMatrix.m[1][1];
    RotMatrix(&g_BattleSpecialSuperOrbRotationTama.rotation, &frame.rotMatrix);
    ConcatMatrixIntoFirst(&frame.scaleMatrix, &frame.rotMatrix);
    SetRotMatrix(&frame.scaleMatrix);
    SetTransMatrix(&frame.scaleMatrix);
    SetDrawColorRGB(0, 0, 0);
    g_BattleSpecialSuperOrbModelReqTama.fadeLevel = frame.expandModel->frameCounter << 8;
    g_BattleSpecialSuperOrbDrawListHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecialSuperOrbModelReqTama, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialSuperOrbDrawListHandle);
    if (g_BattleIsPaused != 0) {
        return;
    }
    (frame.expandModel->frameCounter)++;
    if (frame.expandModel->frameCounter == 20) {
        frame.expandModel->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Draws model entry six at the task position, scaled by 1500 per frame, with the shared
 * orientation; frees its slot at frame 20. It keeps drawing while paused.
 */
// FUNCTION: FF7 0x004dbbf2
void __cdecl BattleFx_SpecialSuperOrbGrowingModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[84];
        PsxMatrix modelMatrix;
        struct { unsigned char pad_to_20[0x20]; BattleFxCosmoMemoryTriSpin *growTask; };
        struct { unsigned char pad_to_24[0x24]; PsxMatrix drawMatrix; };
        struct { unsigned char pad_to_44[0x44]; VECTOR scaleVec; };
    } frame;
#pragma pack(pop)

    frame.growTask = (BattleFxCosmoMemoryTriSpin *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecialSuperOrbRotationMoto.draw.modelGroup = Gfx_ModelTableGetRenderObject(6, g_BattleSpecialSuperOrbLoadedResTable);
    RotMatrix(&g_BattleSpecialSuperOrbRotationMoto.rotation, &frame.modelMatrix);
    ConcatMatrixPreservingTranslation(&g_BattleSpecialSuperOrbBasisMatrix.matrix, &frame.modelMatrix, &frame.modelMatrix);
    frame.scaleVec.vz = frame.growTask->step * 0x5DC;
    frame.scaleVec.vy = frame.scaleVec.vz;
    frame.scaleVec.vx = frame.scaleVec.vy;
    ScaleMatrix(&frame.modelMatrix, &frame.scaleVec);
    frame.modelMatrix.t[0] = frame.growTask->posX;
    frame.modelMatrix.t[1] = frame.growTask->posY;
    frame.modelMatrix.t[2] = frame.growTask->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.modelMatrix, &frame.drawMatrix);
    SetRotMatrix(&frame.drawMatrix);
    SetTransMatrix(&frame.drawMatrix);
    SetDrawColorRGB(0, 0, 0);
    g_BattleSpecialSuperOrbRotationMoto.draw.fadeLevel = frame.growTask->step << 8;
    g_BattleSpecialSuperOrbDrawListHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecialSuperOrbRotationMoto.draw, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialSuperOrbDrawListHandle);
    if (g_BattleIsPaused != 0) {
        return;
    }
    (frame.growTask->step)++;
    if (frame.growTask->step == 20) {
        frame.growTask->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Draws one drifting animated sprite (resource 0x11) in a secondary task slot, moving it by its
 * velocity; frees the slot when the animation has played all its frames.
 */
// FUNCTION: FF7 0x004dbd38
void __cdecl BattleFx_SpecialSuperOrbDriftingAnimatedSpriteTask(void)
{
    BattleFxPulseOfLifeSprite *task;

    task = (BattleFxPulseOfLifeSprite *)&g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx];
    g_BattleSpecialSuperOrbModelReqMoto.driftSprite.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(17, g_BattleSpecialSuperOrbLoadedResTable);
    g_BattleSpecialSuperOrbModelReqMoto.driftSprite.drawObj = Gfx_ModelTableGetSpriteObject(17, g_BattleSpecialSuperOrbLoadedResTable);
    BattleFx_InstallBillboardMatrix(&task->posX, 0x6000 /* 6.0 */, 0);
    g_BattleSpecialSuperOrbModelReqMoto.driftSprite.frame = task->frame;
    g_BattleOverlayDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecialSuperOrbModelReqMoto.driftSprite, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused == 0) {
        task->posX += task->velocityX;
        task->posY += task->velocityY;
        task->posZ += task->velocityZ;
        if (g_BattleSpecialSuperOrbModelReqMoto.driftSprite.spriteGroup != 0) {
            (task->frame)++;
            if (task->frame >= g_BattleSpecialSuperOrbModelReqMoto.driftSprite.spriteGroup->header->frameCount) {
                task->ownerSlot = BATTLE_TASK_FREE;
            }
        }
    }
}

/*
 * Draws sprite entry 14 with a sine-driven size and model entries 12 and 13 spinning in opposite
 * directions around Y, concatenated with the shared orientation; frees its slot at frame 40. It
 * keeps drawing while paused.
 */
// FUNCTION: FF7 0x004dbe48
void __cdecl BattleFx_SpecialSuperOrbCounterRotatingModelsTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[72];
        PsxMatrix modelMatrix;
        struct { unsigned char pad_to_20[0x20]; short sineScale; };
        struct { unsigned char pad_to_24[0x24]; BattleFxCosmoMemoryTriSpin *counterSlot; };
        struct { unsigned char pad_to_28[0x28]; PsxMatrix drawMatrix; };
    } frame;
#pragma pack(pop)

    frame.counterSlot = (BattleFxCosmoMemoryTriSpin *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecialSuperOrbModelReqMoto.glowSprite.spriteGroup = Gfx_ModelTableGetLoadedRecord(14, g_BattleSpecialSuperOrbLoadedResTable);
    g_BattleSpecialSuperOrbModelReqMoto.glowSprite.drawObj = Gfx_ModelTableGetSpriteObject(14, g_BattleSpecialSuperOrbLoadedResTable);
    frame.sineScale = SinFixed(frame.counterSlot->step * 0x33) << 7 >> 12;
    BattleFx_InstallBillboardMatrix(&frame.counterSlot->posX, (frame.sineScale << 7) + 0x1000, 0);
    g_BattleSpecialSuperOrbGlowSpriteFrame = (frame.counterSlot->step % 2);
    g_BattleSpecialSuperOrbDrawListHandle = BattleFx_SubmitSpriteDraw(&g_BattleSpecialSuperOrbModelReqMoto.glowSprite, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialSuperOrbDrawListHandle);
    g_BattleSpecialSuperOrbScaleZ = (frame.sineScale << 7) + 0x1000;
    g_BattleSpecialSuperOrbScaleY = g_BattleSpecialSuperOrbScaleZ;
    g_BattleSpecialSuperOrbScaleX = g_BattleSpecialSuperOrbScaleY;
    g_BattleSpecialSuperOrbRotationRoll = frame.counterSlot->step << 8;
    RotMatrix((SVECTOR *)&g_BattleSpecialSuperOrbRotationVector, &frame.modelMatrix);
    ConcatMatrixPreservingTranslation(&g_BattleSpecialSuperOrbBasisMatrix.matrix, &frame.modelMatrix, &frame.modelMatrix);
    ScaleMatrix(&frame.modelMatrix, &g_BattleSpecialSuperOrbScaleX);
    frame.modelMatrix.t[0] = frame.counterSlot->posX;
    frame.modelMatrix.t[1] = frame.counterSlot->posY;
    frame.modelMatrix.t[2] = frame.counterSlot->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.modelMatrix, &frame.drawMatrix);
    SetRotMatrix(&frame.drawMatrix);
    SetTransMatrix(&frame.drawMatrix);
    g_BattleSpecialSuperOrbModelReqIwa.modelGroup = Gfx_ModelTableGetRenderObject(12, g_BattleSpecialSuperOrbLoadedResTable);
    SetDrawColorRGB(0, 0, 0);
    g_BattleSpecialSuperOrbDrawListHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecialSuperOrbModelReqIwa, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialSuperOrbDrawListHandle);
    g_BattleSpecialSuperOrbRotationRoll = -frame.counterSlot->step << 8;
    RotMatrix((SVECTOR *)&g_BattleSpecialSuperOrbRotationVector, &frame.modelMatrix);
    ConcatMatrixPreservingTranslation(&g_BattleSpecialSuperOrbBasisMatrix.matrix, &frame.modelMatrix, &frame.modelMatrix);
    ScaleMatrix(&frame.modelMatrix, &g_BattleSpecialSuperOrbScaleX);
    frame.modelMatrix.t[0] = frame.counterSlot->posX;
    frame.modelMatrix.t[1] = frame.counterSlot->posY;
    frame.modelMatrix.t[2] = frame.counterSlot->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.modelMatrix, &frame.drawMatrix);
    SetRotMatrix(&frame.drawMatrix);
    SetTransMatrix(&frame.drawMatrix);
    g_BattleSpecialSuperOrbModelReqIwa.modelGroup = Gfx_ModelTableGetRenderObject(13, g_BattleSpecialSuperOrbLoadedResTable);
    g_BattleSpecialSuperOrbDrawListHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecialSuperOrbModelReqIwa, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialSuperOrbDrawListHandle);
    if (g_BattleIsPaused == 0) {
        (frame.counterSlot->step)++;
        if (frame.counterSlot->step == 40) {
            frame.counterSlot->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Draws model entries 0 and 1 at the task position with the shared orientation, shrinking in X/Z
 * and growing in Y; frees its slot at frame 20. It keeps drawing while paused.
 */
// FUNCTION: FF7 0x004dc10c
void __cdecl BattleFx_SpecialSuperOrbStretchingModelsTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[84];
        PsxMatrix modelMatrix;
        struct { unsigned char pad_to_20[0x20]; BattleFxEnergyOrbTrailModel *stretchTask; };
        struct { unsigned char pad_to_24[0x24]; PsxMatrix drawMatrix; };
        struct { unsigned char pad_to_44[0x44]; VECTOR scaleVec; };
    } frame;
#pragma pack(pop)

    frame.stretchTask = (BattleFxEnergyOrbTrailModel *)BATTLE_CURRENT_TASK_SLOT;
    RotMatrix(&g_BattleSpecialSuperOrbRotationUchi, &frame.modelMatrix);
    ConcatMatrixPreservingTranslation(&g_BattleSpecialSuperOrbBasisMatrix.matrix, &frame.modelMatrix, &frame.modelMatrix);
    frame.scaleVec.vz = 0x4000 - frame.stretchTask->frameCount * 0x12C;
    frame.scaleVec.vx = frame.scaleVec.vz;
    frame.scaleVec.vy = frame.stretchTask->frameCount * 0x384;
    ScaleMatrix(&frame.modelMatrix, &frame.scaleVec);
    frame.modelMatrix.t[0] = frame.stretchTask->posX;
    frame.modelMatrix.t[1] = frame.stretchTask->posY;
    frame.modelMatrix.t[2] = frame.stretchTask->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.modelMatrix, &frame.drawMatrix);
    SetRotMatrix(&frame.drawMatrix);
    SetTransMatrix(&frame.drawMatrix);
    SetDrawColorRGB(0, 0, 0);
    g_BattleSpecialSuperOrbModelReqUchi.fadeLevel = 4096 - (g_BattleSpecialSuperOrbStretchFadeCounter << 8);
    g_BattleSpecialSuperOrbModelReqUchi.modelGroup = Gfx_ModelTableGetRenderObject(0, g_BattleSpecialSuperOrbLoadedResTable);
    g_BattleSpecialSuperOrbDrawListHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecialSuperOrbModelReqUchi, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialSuperOrbDrawListHandle);
    g_BattleSpecialSuperOrbModelReqUchi.modelGroup = Gfx_ModelTableGetRenderObject(1, g_BattleSpecialSuperOrbLoadedResTable);
    g_BattleSpecialSuperOrbDrawListHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecialSuperOrbModelReqUchi, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialSuperOrbDrawListHandle);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.stretchTask->frameCount > 12) {
        g_BattleSpecialSuperOrbStretchFadeCounter--;
    }
    (frame.stretchTask->frameCount)++;
    if (frame.stretchTask->frameCount == 20) {
        frame.stretchTask->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Draws model entries 2, 3 and 4 and a sprite at the projectile position. While unpaused it moves
 * the projectile, spawns a hit mark at a random point, a trail model every second frame and, while
 * it is below ground level, bouncing sprites and falling fade models; frees its slot at frame 20.
 */
// FUNCTION: FF7 0x004dc2bf
void __cdecl BattleFx_SpecialSuperOrbProjectileTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[84];
        int transDepth;
        struct { unsigned char pad_to_4[0x4]; short spawnX; };
        struct { unsigned char pad_to_6[0x6]; short spawnZ; };
        struct { unsigned char pad_to_8[0x8]; short spawnY; };
        struct { unsigned char pad_to_C[0xC]; PsxMatrix modelMatrix; };
        struct { unsigned char pad_to_2C[0x2C]; BattleFxEnergyOrbProjectile *spawnedSlot; };
        struct { unsigned char pad_to_30[0x30]; BattleFxEnergyOrbProjectile *projTask; };
        struct { unsigned char pad_to_34[0x34]; PsxMatrix drawMatrix; };
    } frame;
#pragma pack(pop)

    frame.projTask = (BattleFxEnergyOrbProjectile *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecialSuperOrbScaleTrajectory.sprite.spriteGroup = Gfx_ModelTableGetLoadedRecord(14, g_BattleSpecialSuperOrbLoadedResTable);
    g_BattleSpecialSuperOrbScaleTrajectory.sprite.drawObj = Gfx_ModelTableGetSpriteObject(14, g_BattleSpecialSuperOrbLoadedResTable);
    RotMatrix(&g_BattleSpecialSuperOrbRotationTrajectory.rotation, &frame.modelMatrix);
    ConcatMatrixPreservingTranslation(&g_BattleSpecialSuperOrbBasisMatrix.matrix, &frame.modelMatrix, &frame.modelMatrix);
    ScaleMatrix(&frame.modelMatrix, &g_BattleSpecialSuperOrbScaleTrajectory.scale);
    frame.modelMatrix.t[0] = frame.projTask->posX;
    frame.modelMatrix.t[1] = frame.projTask->posY;
    frame.modelMatrix.t[2] = frame.projTask->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.modelMatrix, &frame.drawMatrix);
    SetRotMatrix(&frame.drawMatrix);
    SetTransMatrix(&frame.drawMatrix);
    g_BattleSpecialSuperOrbScaleTrajectory.draw.fadeLevel = g_BattleSpecialSuperOrbLifetimeCounter << 4;
    g_BattleSpecialSuperOrbScaleTrajectory.draw.modelGroup = Gfx_ModelTableGetRenderObject(2, g_BattleSpecialSuperOrbLoadedResTable);
    g_BattleSpecialSuperOrbDrawListHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecialSuperOrbScaleTrajectory.draw, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialSuperOrbDrawListHandle);
    g_BattleSpecialSuperOrbScaleTrajectory.draw.modelGroup = Gfx_ModelTableGetRenderObject(3, g_BattleSpecialSuperOrbLoadedResTable);
    g_BattleSpecialSuperOrbDrawListHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecialSuperOrbScaleTrajectory.draw, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialSuperOrbDrawListHandle);
    g_BattleSpecialSuperOrbScaleTrajectory.draw.modelGroup = Gfx_ModelTableGetRenderObject(4, g_BattleSpecialSuperOrbLoadedResTable);
    g_BattleSpecialSuperOrbDrawListHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecialSuperOrbScaleTrajectory.draw, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialSuperOrbDrawListHandle);
    BattleFx_InstallBillboardMatrix(&frame.projTask->posX, 0x4000 /* 4.0 */, 0);
    g_BattleSpecialSuperOrbColor2.colorB = g_BattleSpecialSuperOrbLifetimeCounter * 20;
    g_BattleSpecialSuperOrbColor2.colorG = g_BattleSpecialSuperOrbColor2.colorB;
    g_BattleSpecialSuperOrbColor2.colorR = g_BattleSpecialSuperOrbColor2.colorG;
    g_BattleSpecialSuperOrbColor2.frame = (frame.projTask->frameCount % 2);
    g_BattleSpecialSuperOrbDrawListHandle = BattleFx_SubmitSpriteDraw(&g_BattleSpecialSuperOrbScaleTrajectory.sprite, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialSuperOrbDrawListHandle);
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.projTask->posX += frame.projTask->velocityX;
    frame.projTask->posY += frame.projTask->velocityY;
    frame.projTask->posZ += frame.projTask->velocityZ;
    g_BattleSpecialSuperOrbBasisMatrix.matrix.t[0] = frame.projTask->posX;
    g_BattleSpecialSuperOrbBasisMatrix.matrix.t[1] = frame.projTask->posY;
    g_BattleSpecialSuperOrbBasisMatrix.matrix.t[2] = frame.projTask->posZ;
    SetRotMatrix(&g_BattleSpecialSuperOrbBasisMatrix.matrix);
    SetTransMatrix(&g_BattleSpecialSuperOrbBasisMatrix.matrix);
    g_BattleSpecialSuperOrbRandomAngle = (rand() % 4096);
    frame.spawnX = CosFixed(g_BattleSpecialSuperOrbRandomAngle) * 0x4B0 >> 12;
    frame.spawnZ = SinFixed(g_BattleSpecialSuperOrbRandomAngle) * 0x4B0 >> 12;
    frame.spawnY = 0;
    RotTransPointToInt16(&frame.spawnX, &frame.spawnX, &frame.transDepth);
    BattleFx_HitMarkSpawnAtPoint(&frame.spawnX, 0x44, 0);
    if ((frame.projTask->frameCount % 2) == 0) {
        frame.spawnedSlot = (BattleFxEnergyOrbProjectile *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialSuperOrbTrailModelTask);
        frame.spawnedSlot->posX = frame.projTask->posX;
        frame.spawnedSlot->posY = frame.projTask->posY;
        frame.spawnedSlot->posZ = frame.projTask->posZ;
    }
    if (frame.projTask->posY < 0) {
        frame.spawnedSlot = (BattleFxEnergyOrbProjectile *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialSuperOrbBouncingSpriteTask);
        frame.spawnedSlot->posX = frame.projTask->posX;
        frame.spawnedSlot->posZ = frame.projTask->posZ;
        frame.spawnedSlot->posY = 0;
        g_BattleSpecialSuperOrbRandomAngle = (rand() % 4096);
        frame.spawnedSlot->velocityX = CosFixed(g_BattleSpecialSuperOrbRandomAngle) * 0x78 >> 12;
        frame.spawnedSlot->velocityZ = SinFixed(g_BattleSpecialSuperOrbRandomAngle) * 0x78 >> 12;
        frame.spawnedSlot->velocityY = -rand() % 300 - 50;
        frame.spawnedSlot->variant = rand() % 7;
        if (frame.projTask->frameCount % 3 == 0) {
            frame.spawnedSlot = (BattleFxEnergyOrbProjectile *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialSuperOrbFallingFadeModelTask);
            frame.spawnedSlot->posX = frame.projTask->posX + rand() % 4000 + -2000;
            frame.spawnedSlot->posZ = frame.projTask->posZ + rand() % 4000 + -2000;
            frame.spawnedSlot->posY = 0;
            frame.spawnedSlot->variant = (rand() % 4);
        }
    }
    if (frame.projTask->frameCount > 12) {
        g_BattleSpecialSuperOrbLifetimeCounter--;
    }
    (frame.projTask->frameCount)++;
    if (frame.projTask->frameCount == 20) {
        frame.projTask->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Draws sprite entry 16 at the task position, bouncing off the ground (Y = 0) and fading out; frees
 * its slot at frame 20. It keeps drawing while paused.
 */
// FUNCTION: FF7 0x004dc7d7
void __cdecl BattleFx_SpecialSuperOrbBouncingSpriteTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[92];
        int grayLevel;
        struct { unsigned char pad_to_4[0x4]; SVECTOR rotTiltB; };
        struct { unsigned char pad_to_C[0xC]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_2C[0x2C]; short tmpGray; };
        struct { unsigned char pad_to_30[0x30]; PsxMatrix spriteMatrix; };
        struct { unsigned char pad_to_50[0x50]; short componentIndex; };
        struct { unsigned char pad_to_54[0x54]; int pointDepth; };
        struct { unsigned char pad_to_58[0x58]; BattleFxScorchBounceSprite *bounceTask; };
    } frame;
#pragma pack(pop)

    frame.bounceTask = (BattleFxScorchBounceSprite *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecialSuperOrbRotationTama.sprite.spriteGroup = Gfx_ModelTableGetLoadedRecord(16, g_BattleSpecialSuperOrbLoadedResTable);
    g_BattleSpecialSuperOrbRotationTama.sprite.drawObj = Gfx_ModelTableGetSpriteObject(16, g_BattleSpecialSuperOrbLoadedResTable);
    for (frame.componentIndex = 0; frame.componentIndex < 3; frame.componentIndex++) {
        frame.spriteMatrix.m[2][frame.componentIndex] = 0;
        frame.spriteMatrix.m[1][frame.componentIndex] = 0;
        frame.spriteMatrix.m[0][frame.componentIndex] = 0;
    }
    if (frame.bounceTask->frameCount < 12) {
        frame.grayLevel = 8;
    } else {
        frame.grayLevel = 0x14 - frame.bounceTask->frameCount;
    }
    frame.tmpGray = *(short *)&frame.grayLevel;
    frame.rotTiltB.vy = 0;
    frame.rotTiltB.vx = frame.rotTiltB.vy;
    frame.rotTiltB.vz = frame.bounceTask->angle;
    SetRotMatrix(&g_BattleCameraTransform);
    SetTransMatrix(&g_BattleCameraTransform);
    RotTransPoint((SVECTOR *)&frame.bounceTask->posX, frame.spriteMatrix.t, &frame.pointDepth);
    frame.spriteMatrix.m[2][2] = 0x2000;
    frame.spriteMatrix.m[1][1] = frame.spriteMatrix.m[2][2];
    frame.spriteMatrix.m[0][0] = frame.spriteMatrix.m[1][1];
    RotMatrix(&frame.rotTiltB, &frame.rotMatrix);
    ConcatMatrixIntoFirst(&frame.spriteMatrix, &frame.rotMatrix);
    SetRotMatrix(&frame.spriteMatrix);
    SetTransMatrix(&frame.spriteMatrix);
    g_BattleSpecialSuperOrbColor1.frame = frame.bounceTask->spriteFrame;
    g_BattleSpecialSuperOrbColor1.code = (frame.tmpGray - 8 != 0 ? 2 : 0) + 0x2C;
    g_BattleSpecialSuperOrbColor1.colorB = frame.tmpGray << 4;
    g_BattleSpecialSuperOrbColor1.colorG = g_BattleSpecialSuperOrbColor1.colorB;
    g_BattleSpecialSuperOrbColor1.colorR = g_BattleSpecialSuperOrbColor1.colorG;
    g_BattleOverlayDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecialSuperOrbRotationTama.sprite, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.bounceTask->posX += frame.bounceTask->velocityX;
    frame.bounceTask->posY += frame.bounceTask->velocityY;
    frame.bounceTask->posZ += frame.bounceTask->velocityZ;
    if (frame.bounceTask->posY > 0) {
        frame.bounceTask->posY = 0;
        frame.bounceTask->velocityY *= -1;
    }
    frame.bounceTask->velocityY += 20;
    frame.bounceTask->angle += 0x200 /* 45 deg */;
    (frame.bounceTask->frameCount)++;
    if (frame.bounceTask->frameCount == 20) {
        frame.bounceTask->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Draws the model picked by modelIndex (resources 8 to 11) falling and spinning, fading out; frees
 * its slot at frame 20. It keeps drawing while paused.
 */
// FUNCTION: FF7 0x004dca51
void __cdecl BattleFx_SpecialSuperOrbFallingFadeModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[92];
        SVECTOR rotTiltB;
        struct { unsigned char pad_to_8[0x8]; PsxMatrix modelMatrix; };
        struct { unsigned char pad_to_28[0x28]; int resId8[4]; };
        struct { unsigned char pad_to_38[0x38]; BattleFxSpecialEnergyOrbFallingSlot *fallingSlot; };
        struct { unsigned char pad_to_3C[0x3C]; PsxMatrix drawMatrix; };
    } frame;
#pragma pack(pop)

    frame.fallingSlot = (BattleFxSpecialEnergyOrbFallingSlot *)BATTLE_CURRENT_TASK_SLOT;
    frame.resId8[0] = 8;
    frame.resId8[1] = 9;
    frame.resId8[2] = 0xA;
    frame.resId8[3] = 0xB;
    frame.rotTiltB.vy = 0;
    frame.rotTiltB.vx = frame.rotTiltB.vy;
    frame.rotTiltB.vz = frame.fallingSlot->angleZ;
    RotMatrixXYZ(&frame.rotTiltB, &frame.modelMatrix);
    ScaleMatrix(&frame.modelMatrix, &g_BattleSpecialSuperOrbScaleTama.scale);
    frame.modelMatrix.t[0] = frame.fallingSlot->posX;
    frame.modelMatrix.t[1] = frame.fallingSlot->posY;
    frame.modelMatrix.t[2] = frame.fallingSlot->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.modelMatrix, &frame.drawMatrix);
    SetRotMatrix(&frame.drawMatrix);
    SetTransMatrix(&frame.drawMatrix);
    g_BattleSpecialSuperOrbScaleTama.draw.fadeLevel = 0x80 - frame.fallingSlot->frame * 6;
    g_BattleSpecialSuperOrbScaleTama.draw.modelGroup = Gfx_ModelTableGetRenderObject(frame.resId8[frame.fallingSlot->modelIndex], g_BattleSpecialSuperOrbLoadedResTable);
    g_BattleSpecialSuperOrbDrawListHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecialSuperOrbScaleTama.draw, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialSuperOrbDrawListHandle);
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.fallingSlot->posY -= 200;
    frame.fallingSlot->angleZ += 0x100 /* 22.5 deg */;
    (frame.fallingSlot->frame)++;
    if (frame.fallingSlot->frame == 20) {
        frame.fallingSlot->slotTag = BATTLE_TASK_FREE;
    }
}

/*
 * Draws model entry five shrinking behind the projectile once it is older than two frames; frees
 * its slot at frame 20.
 */
// FUNCTION: FF7 0x004dcbc4
void __cdecl BattleFx_SpecialSuperOrbTrailModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[84];
        PsxMatrix modelMatrix;
        struct { unsigned char pad_to_20[0x20]; BattleFxEnergyOrbTrailModel *trailTask; };
        struct { unsigned char pad_to_24[0x24]; PsxMatrix drawMatrix; };
        struct { unsigned char pad_to_44[0x44]; VECTOR scaleVec; };
    } frame;
#pragma pack(pop)

    frame.trailTask = (BattleFxEnergyOrbTrailModel *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecialSuperOrbModelReqMoto.trailDraw.modelGroup = Gfx_ModelTableGetRenderObject(5, g_BattleSpecialSuperOrbLoadedResTable);
    if (frame.trailTask->frameCount > 2) {
        RotMatrix(&g_BattleSpecialSuperOrbModelReqMoto.rotation, &frame.modelMatrix);
        ConcatMatrixPreservingTranslation(&g_BattleSpecialSuperOrbBasisMatrix.matrix, &frame.modelMatrix, &frame.modelMatrix);
        frame.scaleVec.vz = 0x4000 - frame.trailTask->frameCount * 0x190;
        frame.scaleVec.vy = frame.scaleVec.vz;
        frame.scaleVec.vx = frame.scaleVec.vy;
        ScaleMatrix(&frame.modelMatrix, &frame.scaleVec);
        frame.modelMatrix.t[0] = frame.trailTask->posX;
        frame.modelMatrix.t[1] = frame.trailTask->posY;
        frame.modelMatrix.t[2] = frame.trailTask->posZ;
        MultiplyMatrix(&g_BattleCameraTransform, &frame.modelMatrix, &frame.drawMatrix);
        SetRotMatrix(&frame.drawMatrix);
        SetTransMatrix(&frame.drawMatrix);
        g_BattleSpecialSuperOrbModelReqMoto.trailDraw.fadeLevel = frame.trailTask->frameCount * 200;
        SetDrawColorRGB(0, 0, 0);
        g_BattleSpecialSuperOrbDrawListHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecialSuperOrbModelReqMoto.trailDraw, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialSuperOrbDrawListHandle);
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    (frame.trailTask->frameCount)++;
    if (frame.trailTask->frameCount == 20) {
        frame.trailTask->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Draws one decelerating sprite (resource 0x12): its velocity decays toward zero in X and Y and
 * falls by 0xC8 in Z per frame; frees its slot at step 10.
 */
// FUNCTION: FF7 0x004dcd24
void __cdecl BattleFx_SpecialSuperOrbDecelSpriteTask(void)
{
    BattleFxLivewireDotSlot *decel;
    int projX[2];
    int projY;

    decel = (BattleFxLivewireDotSlot *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecialSuperOrbRotationTrajectory.sprite.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(18, g_BattleSpecialSuperOrbLoadedResTable);
    g_BattleSpecialSuperOrbRotationTrajectory.sprite.drawObj = Gfx_ModelTableGetSpriteObject(18, g_BattleSpecialSuperOrbLoadedResTable);
    g_BattleSpecialSuperOrbBasisMatrix.matrix.t[0] = decel->posX;
    g_BattleSpecialSuperOrbBasisMatrix.matrix.t[1] = decel->posY;
    g_BattleSpecialSuperOrbBasisMatrix.matrix.t[2] = decel->posZ;
    SetRotMatrix(&g_BattleSpecialSuperOrbBasisMatrix.matrix);
    SetTransMatrix(&g_BattleSpecialSuperOrbBasisMatrix.matrix);
    RotTransPointToInt16(&decel->velocityX, (short *)projX, &projY);
    BattleFx_InstallBillboardMatrix(projX, 0x2000 /* 2.0 */, 0);
    g_BattleSpecialSuperOrbRotationTrajectory.sprite.frame = decel->step % 2;
    g_BattleSpecialSuperOrbDrawListHandle = BattleFx_SubmitSpriteDraw(&g_BattleSpecialSuperOrbRotationTrajectory.sprite, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialSuperOrbDrawListHandle);
    if (g_BattleIsPaused != 0) {
        return;
    }
    decel->velocityX += (short)((0 - decel->velocityX) / 4);
    decel->velocityY += (short)((0 - decel->velocityY) / 4);
    decel->velocityZ -= 200;
    (decel->step)++;
    if (decel->step == 10) {
        decel->ownerSlot = BATTLE_TASK_FREE;
    }
}
} /* extern "C" */
