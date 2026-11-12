// src/ff7/src/battle/yasui/yasui_limit_omnislash_effect.cpp
// Limit Break [6] (Cloud Lv 4): Omnislash (Japanese: 超究武神覇斬, dev tag: "LIMIT/BUSIN" / Busin)
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Omnislash limit break effect for Cloud: slash sequences with scaling and pulsing
//   models, a damped spark particle pool, spinning energy cylinders and a white flash.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/types/battle.h"
#include "ff7/types/battle_boss.h"
#include "ff7/battle_tasks.h"
#include "ff7/camera_script.h"
#include "lib/types/graphics.h"
#include "lib/enums/sound.h"
#include <string.h>
#include <stdlib.h>
#include "lib/api/sound.h"
#include "lib/codegen.h"
#include "ff7/api/battle.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
extern "C" {

struct GfxPsxSpriteQuadState;

#pragma intrinsic(memcpy)
#pragma intrinsic(memset)
#pragma intrinsic(abs)

// clang-format off
// GLOBAL: FF7 0x007ed938
unsigned int g_YasuiLimitOmnislashResourceDef[80] = {
    0x006400b1,
    // STRING: FF7 0x007edce4
    (unsigned int)"RING_1.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x003200b1,
    // STRING: FF7 0x007edcf0
    (unsigned int)"RING_2.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x003200b1,
    // STRING: FF7 0x007edcfc
    (unsigned int)"RING_3.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x007edd08
    (unsigned int)"LINE_1.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x007edd14
    (unsigned int)"FLASH_1.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001940b1,
    // STRING: FF7 0x007edd20
    (unsigned int)"AIR_1.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x006440b1,
    // STRING: FF7 0x007edd2c
    (unsigned int)"FLASH_2.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x00c800b1,
    // STRING: FF7 0x007edd38
    (unsigned int)"LINE_2.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001940b1,
    // STRING: FF7 0x007edd44
    (unsigned int)"AIR_2.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x003200b1,
    // STRING: FF7 0x007edd50
    (unsigned int)"SONIC_1.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x00c800b1,
    // STRING: FF7 0x007edd5c
    (unsigned int)"LINE_3.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x00c840b1,
    // STRING: FF7 0x007edd68
    (unsigned int)"LINE_4.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x007edd74
    (unsigned int)"BUSHIN00.TIM",
    // STRING: FF7 0x007edd84
    (unsigned int)"HIT_1.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x007edd90
    (unsigned int)"BUSHIN01.TIM",
    // STRING: FF7 0x007edda0
    (unsigned int)"SMOKE_1.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x007eddac
    (unsigned int)"LIGHT_1.TIM",
    // STRING: FF7 0x007eddb8
    (unsigned int)"LIGHT_1.SPT",
    0x00000000,
    0x00000000,
    0x00197a39,
    // STRING: FF7 0x007eddc4
    (unsigned int)"BUSHIN01.TIM",
    0x00000000};
// clang-format off
// GLOBAL: FF7 0x007eda78
struct YasuiLimitOmnislashDrawReqPool1 g_YasuiLimitOmnislashDrawReqPool1 = {0, {0xa8, 0, 0, 0, 0, 0}, 128, {0x20, 0, 0, 0}, 0, {0xa8, 0, 0, 0, 0, 0}, 128, {0x20, 0, 0, 0}, 0, {0xa8, 0, 0, 0, 0, 0, 0, 0, 0x20, 0, 0, 0}, 0, {0xa8, 0, 0, 0, 0, 0, 0, 0, 0x20, 0, 0, 0}, 0, {0xa8, 0, 0, 0, 0, 0}, 0, {0x20, 0, 0, 0}, 0, {0xa8, 0, 0, 0, 0, 0, 0, 0, 0x20, 0, 0, 0}, 0, 0};

// GLOBAL: FF7 0x007edae0
struct YasuiLimitOmnislashSpriteTint g_YasuiLimitOmnislashSpriteTint = {-128, -128, -128, {0x2e}};

// GLOBAL: FF7 0x007edae8
struct YasuiLimitOmnislashDrawReqPool2 g_YasuiLimitOmnislashDrawReqPool2 = {0, {0xa8, 0, 0, 0, 0, 0, 0x80, 0, 0x20, 0, 0, 0}, 0, {0xa8, 0, 0, 0, 0, 0}, 128, {0x20, 0, 0, 0}, 0, 0, {0x80, 0x80, 0x80, 0x2e}, 0, {0}, 0, 0, {0x40, 0x40, 0x40, 0x2e}, 0, {0}};

unsigned char g_rawData_007edb28[16] = {0x00, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x20, 0x00, 0x00, 0x00};

// GLOBAL: FF7 0x007edb38
struct YasuiLimitOmnislashDrawReq1 g_YasuiLimitOmnislashDrawReq1 = {0, {0x28, 0, 0, 0, 0, 0}, 128};

unsigned char g_rawData_007edb44[4] = {0x20, 0x00, 0x00, 0x00};

// GLOBAL: FF7 0x007edb48
int g_YasuiLimitOmnislashScale1 = 16384;

unsigned char g_rawData_007edb4c[4] = {0x00, 0x40, 0x00, 0x00};

// GLOBAL: FF7 0x007edb50
int g_YasuiLimitOmnislashScale2 = 16384;

// GLOBAL: FF7 0x007edb58
struct YasuiLimitOmnislashDrawReq2 g_YasuiLimitOmnislashDrawReq2 = {0, {0xa8, 0, 0, 0, 0, 0}, 128};

unsigned char g_rawData_007edb64[4] = {0x20, 0x00, 0x00, 0x00};

// GLOBAL: FF7 0x007edb68
int g_YasuiLimitOmnislashScale3 = 16384;

unsigned char g_rawData_007edb6c[4] = {0x00, 0x40, 0x00, 0x00};

// GLOBAL: FF7 0x007edb70
int g_YasuiLimitOmnislashScale4 = 16384;

// GLOBAL: FF7 0x007edb78
struct YasuiLimitOmnislashDrawReq3 g_YasuiLimitOmnislashDrawReq3 = {0, {0xa8, 0, 0, 0, 0, 0}, 128, {0x20, 0, 0, 0}};

// GLOBAL: FF7 0x007edb88
short g_YasuiLimitOmnislashMotionTable1[16] = {0x20, 37, 0x2b, 0x31, 0x39, 0x3f, 0x46, 0x4e, 0x53, 0x59, 0x5f, 0x67, 0x6d, 0x74, 0x7f, 0xdb};

// GLOBAL: FF7 0x007edba8
short g_YasuiLimitOmnislashMotionTable2[16] = {0x22, 0x27, 0x2d, 0x36, 0x3c, 0x42, 0x4a, 0x50, 0x55, 0x5b, 0x64, 0x6a, 0x70, 0x78, 0xca, 0xdb};

// GLOBAL: FF7 0x007edbc8
short g_YasuiLimitOmnislashMotionTable3[14] = {0x1800, 0x1900, 0x1a00, 0x1b00, 0x1c00, 0x1d00, 0x1e00, 0x1f00, 0x2000, 0x2100, 0x2200, 0x2300, 0x2400, 0x2500};

// GLOBAL: FF7 0x007edbe4
short g_YasuiLimitOmnislashAngleOffset = 12288;

// GLOBAL: FF7 0x007edbe8
short g_YasuiLimitOmnislashMotionTable4[14] = {0x400, 0x200, -1, 0x400, 0x0, 0x200, -1, 0x400, -512, -1, 0x0, 0x400, 0x200, -512};

// GLOBAL: FF7 0x007edc08
short g_YasuiLimitOmnislashMotionTable5[15] = {0x2000, 0x2100, 0x2200, 0x2300, 0x2400, 0x2500, 0x2600, 0x2700, 0x2800, 0x2900, 0x2a00, 0x2b00, 0x2c00, 0x2d00, 0x3000};

// GLOBAL: FF7 0x007edc28
short g_YasuiLimitOmnislashMotionTable6[15] = {0x3, 0x3, 0x3, 0x3, 0x4, 0x4, 0x4, 0x4, 0x5, 0x5, 0x5, 0x5, 0x5, 0x5, 0x7};

// GLOBAL: FF7 0x007edc48
short g_YasuiLimitOmnislashMotionTable7[14] = {0x0, 0x0, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0xa, 0xb, 0xc, 0xc, 0xc, 0xc};

// GLOBAL: FF7 0x007edc64
short g_YasuiLimitOmnislashDurationConst = 30;

// GLOBAL: FF7 0x007edc68
short g_YasuiLimitOmnislashMotionTable8[16] = {0x0, 0x0, 0x0, 0x0, 0x4000, 0x6000, 0x7f00, 0x7f00, 0x7f00, 0x7f00, 0x7f00, 0x7f00, 0x7f00, 0x7f00, 0x7f00, 0x0};

// GLOBAL: FF7 0x007edc88
struct YasuiLimitOmnislashMotionTable9 g_YasuiLimitOmnislashMotionTable9 = {{{0x20, 0, 0x60, 0, 0x60, 0}, {0x60, 0, 0x60, 0, 0x10, 0}, {0x60, 0, 0x10, 0, 0x60, 0}, {0x60, 0, 0x40, 0, 0x8, 0}, {0x20, 0, 0x20, 0, 0x60, 0}, {0x20, 0, 0x60, 0, 0x20, 0}, {0x40, 0, 0x10, 0, 0x20, 0}, {0x20, 0, 0x40, 0, 0x10, 0}, {0x20, 0, 0x10, 0, 0x40, 0}, {0x20, 0, 0x10, 0, 0x8, 0}, {0x8, 0, 0x20, 0, 0x10, 0}, {0x8, 0, 0x10, 0, 0x20, 0}, {0x10, 0, 0x0, 0, 0x8, 0}}, {0x8, 0x0}};

unsigned char g_rawData_007edcd8[12] = {0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

// GLOBAL: FF7 0x00a52268
struct BattleLimitHighwindLoadedResTable g_YasuiLimitOmnislashTargetPosX = {0};

// GLOBAL: FF7 0x00a5226c
short g_YasuiLimitOmnislashTargetPosZ = 0;

// GLOBAL: FF7 0x00a52270
short g_YasuiLimitOmnislashTargetActor = 0;

// GLOBAL: FF7 0x00a52274
short g_YasuiLimitOmnislashAlphaPhase = 0;

// GLOBAL: FF7 0x00a52278
int g_YasuiLimitOmnislashStepCounter = 0;

unsigned char g_bssPad_00a5227c[4] = {0};

// GLOBAL: FF7 0x00a52280
short g_YasuiLimitOmnislashCentroid[2] = {0};

unsigned char g_bssPad_00a52284[4] = {0};

// GLOBAL: FF7 0x00a52288
short g_YasuiLimitOmnislashAttackAngle = 0;

// GLOBAL: FF7 0x00a5228c
short g_YasuiLimitOmnislashSubAngle = 0;

// GLOBAL: FF7 0x00a52290
int g_YasuiLimitOmnislashScaleInterp1 = 0;

// GLOBAL: FF7 0x00a52294
int g_YasuiLimitOmnislashScaleInterp2 = 0;

// GLOBAL: FF7 0x00a52298
int g_YasuiLimitOmnislashScaleTarget = 0;

unsigned char g_bssPad_00a5229c[4] = {0};

// GLOBAL: FF7 0x00a522a0
int g_YasuiLimitOmnislashPacketCursor = 0;

// GLOBAL: FF7 0x00a522a4
short g_YasuiLimitOmnislashTargetMask = 0;

// GLOBAL: FF7 0x00a522a8
short g_YasuiLimitOmnislashSlashDuration = 0;

unsigned char g_bssPad_00a522ac[4] = {0};

// GLOBAL: FF7 0x00a522b0
int g_YasuiLimitOmnislashScaleVector1 = 0;

// GLOBAL: FF7 0x00a522b4
int g_YasuiLimitOmnislashUnused2b4 = 0;

// GLOBAL: FF7 0x00a522b8
int g_YasuiLimitOmnislashSineOffset = 0;

unsigned char g_bssPad_00a522bc[4] = {0};

// GLOBAL: FF7 0x00a522c0
short g_YasuiLimitOmnislashSourceActor = 0;

// GLOBAL: FF7 0x00a522c4
short g_YasuiLimitOmnislashFlashIntensity = 0;

// GLOBAL: FF7 0x00a522c8
struct BattleLimitHighwindLoadedResTable g_YasuiLimitOmnislashBasePosX = {0};

// GLOBAL: FF7 0x00a522cc
short g_YasuiLimitOmnislashBasePosZ = 0;

// GLOBAL: FF7 0x00a522d0
int g_YasuiLimitOmnislashScaleVector2 = 0;

// GLOBAL: FF7 0x00a522d4
int g_YasuiLimitOmnislashRadiusInterp = 0;

// GLOBAL: FF7 0x00a522d8
int g_YasuiLimitOmnislashRadiusTarget = 0;

unsigned char g_bssPad_00a522dc[4] = {0};

// GLOBAL: FF7 0x00a522e0
short g_YasuiLimitOmnislashParticleIndex = 0;

// GLOBAL: FF7 0x00a522e4
short g_YasuiLimitOmnislashCylinderScale = 0;

// GLOBAL: FF7 0x00a522e8
int g_YasuiLimitOmnislashScaleInterp3 = 0;

// GLOBAL: FF7 0x00a522ec
int g_YasuiLimitOmnislashScaleInterp4 = 0;

// GLOBAL: FF7 0x00a522f0
int g_YasuiLimitOmnislashScaleTarget2 = 0;

unsigned char g_bssPad_00a522f4[4] = {0};

// GLOBAL: FF7 0x00a522f8
short g_YasuiLimitOmnislashSlashIndex = 0;

unsigned char g_bssPad_00a522fc[4] = {0};

// GLOBAL: FF7 0x00a52300
struct BattleFxOmnislashParticle g_YasuiLimitOmnislashParticleTable[200] = {0};

// GLOBAL: FF7 0x00a53c00
int g_YasuiLimitOmnislashScalePulse1 = 0;

// GLOBAL: FF7 0x00a53c04
int g_YasuiLimitOmnislashScalePulse2 = 0;

// GLOBAL: FF7 0x00a53c08
int g_YasuiLimitOmnislashScalePulse3 = 0;

unsigned char g_bssPad_00a53c0c[4] = {0};

// GLOBAL: FF7 0x00a53c10
struct YasuiLimitOmnislashCylinderData g_YasuiLimitOmnislashCylinderData = {0};

// GLOBAL: FF7 0x00a53c38
struct GfxTableCursor *g_YasuiLimitOmnislashResourceHandle = 0;

// GLOBAL: FF7 0x00a53c3c
short g_YasuiLimitOmnislashActiveFlag = 0;

unsigned char g_bssPad_00a53c40[8] = {0};

// GLOBAL: FF7 0x00a53c48
int g_YasuiLimitOmnislashModelRotX = 0;

// GLOBAL: FF7 0x00a53c4c
short g_YasuiLimitOmnislashModelPitch = 0;

// GLOBAL: FF7 0x00a53c50
int g_YasuiLimitOmnislashModelRotY = 0;

// GLOBAL: FF7 0x00a53c54
short g_YasuiLimitOmnislashModelYaw = 0;

// GLOBAL: FF7 0x00a53c58
struct YasuiLimitOmnislashRotationVector g_YasuiLimitOmnislashRotationVector = {0};

unsigned char g_bssPad_00a53c5c[4] = {0};

// GLOBAL: FF7 0x00a53c60
struct YasuiLimitOmnislashEulerAngles g_YasuiLimitOmnislashEulerAngles = {0};

unsigned char g_bssPad_00a53c6c[4] = {0};
// clang-format on


/* Loads resource and sound packages for Cloud's Omnislash limit break. */
// FUNCTION: FF7 0x0046b6c0
void __cdecl BattleFx_LimitLoadResOmnislash(void)
{
    GfxModelTableBank *graphicsOwner;
    GfxResourceRequest reqBuffer;
    int kind;
    char tmpBuffer[200];
    GfxResourceRequest *pRequest;
    int resourceTable;

    if (g_pBattle3dScene != 0 && g_YasuiLimitOmnislashResourceHandle == 0) {
        kind = 0x10;
        resourceTable = (int)&g_YasuiLimitOmnislashResourceDef;
        graphicsOwner = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRequest = &reqBuffer;
        Gfx_InitResourceRequest(pRequest);
        pRequest->groupTag = 0;
        // STRING: FF7 0x007eddd4
        pRequest->resourcePath = (char *)BattleFx_BuildResourcePath("LIMIT/BUSIN", tmpBuffer);
        Gfx_LoadModelTableIfAbsent(kind, resourceTable, pRequest, &g_YasuiLimitOmnislashResourceHandle, graphicsOwner);
    }
}

/* Validates target mask and initializes Omnislash combat effect state. */
// FUNCTION: FF7 0x0046b750
int __cdecl BattleFx_LimitEnterOmnislash(int targetMask, int actorIdx)
{
    BattleFx_LimitLoadResOmnislash();
    g_YasuiLimitOmnislashActiveFlag = 0;
    g_YasuiLimitOmnislashStepCounter = 0;
    (g_YasuiLimitOmnislashCylinderData.spinStep) = 0x80;
    g_YasuiLimitOmnislashSubAngle = 0;
    g_YasuiLimitOmnislashFlashIntensity = 0;
    g_YasuiLimitOmnislashAlphaPhase = 0;
    g_YasuiLimitOmnislashScaleVector1 = 0x2000;
    g_YasuiLimitOmnislashUnused2b4 = 0;
    g_YasuiLimitOmnislashSineOffset = 0;
    g_YasuiLimitOmnislashScaleTarget2 = 0x2000;
    g_YasuiLimitOmnislashScaleInterp4 = g_YasuiLimitOmnislashScaleTarget2;
    g_YasuiLimitOmnislashScaleInterp3 = g_YasuiLimitOmnislashScaleInterp4;
    g_YasuiLimitOmnislashScaleTarget = 0x2000;
    g_YasuiLimitOmnislashScaleInterp2 = g_YasuiLimitOmnislashScaleTarget;
    g_YasuiLimitOmnislashScaleInterp1 = g_YasuiLimitOmnislashScaleInterp2;
    g_YasuiLimitOmnislashScaleVector2 = 0x100;
    g_YasuiLimitOmnislashRadiusTarget = 0x400;
    g_YasuiLimitOmnislashRadiusInterp = g_YasuiLimitOmnislashRadiusTarget;
    g_YasuiLimitOmnislashScalePulse3 = 0;
    g_YasuiLimitOmnislashScalePulse2 = g_YasuiLimitOmnislashScalePulse3;
    g_YasuiLimitOmnislashScalePulse1 = g_YasuiLimitOmnislashScalePulse2;
    g_YasuiLimitOmnislashSlashIndex = 0;
    g_YasuiLimitOmnislashSlashDuration = 0;
    g_YasuiLimitOmnislashCylinderScale = 0x40;
    BattleFx_LimitBeginOmnislash(targetMask, actorIdx);
    return 0;
}

/* Spawns root timeline, particle pool, and initial animation tasks for Omnislash. */
// FUNCTION: FF7 0x0046b85d
void __cdecl BattleFx_LimitBeginOmnislash(int targetMask, int actorIdx)
{
    short radiusVal;
    int r;
    BattleFxLimitOmnislashRamp *rampTask;
    int s;
    int offsetZ;

    g_YasuiLimitOmnislashTargetMask = targetMask;
    g_YasuiLimitOmnislashSourceActor = actorIdx;
    Battle_GetActorMaskCentroid(targetMask, g_YasuiLimitOmnislashCentroid);
    rampTask = (BattleFxLimitOmnislashRamp *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitOmnislashTimelineTask);
    rampTask->rampFromA = g_BattleActorRecords[actorIdx].posX;
    rampTask->rampFromB = g_BattleActorRecords[actorIdx].posY;
    rampTask->rampFromC = g_BattleActorRecords[actorIdx].posZ;
    rampTask->rampToA = g_BattleActorRecords[*(unsigned char *)&g_BattleHitTargetActors].posX;
    rampTask->rampToB = g_BattleActorRecords[*(unsigned char *)&g_BattleHitTargetActors].posY;
    rampTask->rampToC = g_BattleActorRecords[*(unsigned char *)&g_BattleHitTargetActors].posZ;
    radiusVal = g_BattleActorRecords[*(unsigned char *)&g_BattleHitTargetActors].radius + 0x5DC;
    if (rampTask->rampFromC > rampTask->rampToC) {
        offsetZ = radiusVal;
    } else {
        offsetZ = -radiusVal;
    }
    rampTask->rampToC += offsetZ;
    rampTask->rampToA = (rampTask->rampToA - rampTask->rampFromA) / 4;
    rampTask->rampToC = (rampTask->rampToC - rampTask->rampFromC) / 4;
    g_BattleFxTimelineState.channel1 = 0;
    g_BattleFxTimelineState.channel2 = g_BattleFxTimelineState.channel1;
    g_BattleFxTimelineState.red = 0x60;
    Battle_StartFixedPointRamp(0x1000, 0xA);
    g_BattleScriptEffectFlag = 1;
    BattleFx_LimitOmnislashBeginParticlePool();
}

/* Drives multi-slash sequence, spawning slashes, models, flashes, and impacts. */
// FUNCTION: FF7 0x0046ba0b
void __cdecl BattleFx_LimitOmnislashTimelineTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[84];
        int rotFlag;
        struct { unsigned char pad_to_4[0x4]; SVECTOR angleX; };
        struct { unsigned char pad_to_0C[0xC]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_2C[0x2C]; BattleFxOmnislashSlashEntry *entry; };
        struct { unsigned char pad_to_34[0x34]; BattleFxLimitOmnislashState *ringState; };
        struct { unsigned char pad_to_38[0x38]; short index; };
        struct { unsigned char pad_to_3C[0x3C]; short count; };
        struct { unsigned char pad_to_40[0x40]; short param; };
        struct { unsigned char pad_to_44[0x44]; short pointX; };
        struct { unsigned char pad_to_46[0x46]; short pointY; };
        struct { unsigned char pad_to_48[0x48]; short pointZ; };
        struct { unsigned char pad_to_4C[0x4C]; BattleFxOmnislashTimeline *timeline; };
        struct { unsigned char pad_to_50[0x50]; short radius; };
    } frame;
#pragma pack(pop)

    frame.timeline = (BattleFxOmnislashTimeline *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.timeline->state == 0) {
        SfxLoadIntoBuffer(SFX_ID_OMNISLASH_CHARGE, 0);
        SfxLoadIntoBuffer(SFX_ID_OMNISLASH_SWING, 0);
        SfxLoadIntoBuffer(SFX_ID_OMNISLASH_FINISH, 0);
    }
    if (frame.timeline->state >= 0x45 && frame.timeline->state < 0x48) {
        g_BattleActorRecords[g_YasuiLimitOmnislashSourceActor].posX += frame.timeline->deltaPosX;
        g_BattleActorRecords[g_YasuiLimitOmnislashSourceActor].posZ += frame.timeline->deltaPosZ;
        BattleFx_LimitOmnislashSpawnDampedSpriteParticles(&g_BattleActorRecords[g_YasuiLimitOmnislashSourceActor].posX, 0xC8, -0x64, 2);
    }
    if (frame.timeline->state == 0x45) {
        SoundDispatchCommand(SOUND_SFX_PLAY_CH1, Battle_ComputeScreenPanFromPosition(&g_BattleActorRecords[g_YasuiLimitOmnislashSourceActor].posX), 0x1A, 0, 0, 0, 0, 0, 0);
    }
    if (frame.timeline->state == 0x12 || (frame.timeline->state == 0xB9 || frame.timeline->state == 0xF5)) {
        g_BattleActorStates[g_YasuiLimitOmnislashSourceActor].flags |= 2;
    }
    if (frame.timeline->state == 0x3C || (frame.timeline->state == 0xEB || frame.timeline->state == 0x10E)) {
        g_BattleActorStates[g_YasuiLimitOmnislashSourceActor].flags &= 0xFFFC;
    }
    if (frame.timeline->state == 0) {
        frame.entry = (BattleFxOmnislashSlashEntry *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitOmnislashSpinningCylinderPairTask);
        Battle_GetBoneWorldPos(g_YasuiLimitOmnislashSourceActor, g_BattleActorRecords[g_YasuiLimitOmnislashSourceActor].targetBone, &frame.entry->worldX);
        frame.entry->velocityY = 0;
        SoundDispatchCommand(SOUND_SFX_PLAY_CH1, Battle_ComputeScreenPanFromPosition(&g_BattleActorRecords[g_YasuiLimitOmnislashSourceActor].posX), 0x28E, 0, 0, 0, 0, 0, 0);
    }
    if (frame.timeline->state == 0x14) {
        frame.entry = (BattleFxOmnislashSlashEntry *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitOmnislashBoneModelShrinkingEmitterTask);
        frame.entry->animValue = 0x6000;
        SoundDispatchCommand(SOUND_SFX_PLAY_CH2, Battle_ComputeScreenPanFromPosition(&g_BattleActorRecords[g_YasuiLimitOmnislashSourceActor].posX), 0x188, 0, 0, 0, 0, 0, 0);
    }
    if (frame.timeline->state == 0x1E) {
        SoundDispatchCommand(SOUND_SFX_PLAY_CH1, Battle_ComputeScreenPanFromPosition(&g_BattleActorRecords[g_YasuiLimitOmnislashSourceActor].posX), 0, 0, 0, 0, 0, 0, 0);
    }
    if (frame.timeline->state == 0xB9) {
        frame.entry = (BattleFxOmnislashSlashEntry *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitOmnislashBoneModelGrowingEmitterTask);
        frame.entry->animValue = 0x80;
    }
    if (frame.timeline->state == 0xB9) {
        SoundDispatchCommand(SOUND_SFX_PLAY_CH1, Battle_ComputeScreenPanFromPosition(&g_BattleActorRecords[g_YasuiLimitOmnislashSourceActor].posX), 0x241, 0, 0, 0, 0, 0, 0);
    }
    if (frame.timeline->state == 0xDB) {
        SoundDispatchCommand(SOUND_SFX_PLAY_CH1, Battle_ComputeScreenPanFromPosition(&g_BattleActorRecords[g_YasuiLimitOmnislashSourceActor].posX), 0x189, 0, 0, 0, 0, 0, 0);
    }
    if (frame.timeline->state == 0xAB) {
        frame.entry = (BattleFxOmnislashSlashEntry *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitOmnislashSineScaledModelParticleEmitterTask);
        Battle_GetBoneWorldPos(g_YasuiLimitOmnislashSourceActor, g_BattleActorRecords[g_YasuiLimitOmnislashSourceActor].targetBone, &frame.entry->worldX);
        frame.entry->velocityY = 0;
        SoundDispatchCommand(SOUND_SFX_PLAY_CH1, Battle_ComputeScreenPanFromPosition(&g_BattleActorRecords[g_YasuiLimitOmnislashSourceActor].posX), 0x30, 0, 0, 0, 0, 0, 0);
    }
    if (frame.timeline->state >= 0xAB && frame.timeline->state < 0xB5) {
        Battle_GetBoneWorldPos(g_YasuiLimitOmnislashSourceActor, g_BattleActorRecords[g_YasuiLimitOmnislashSourceActor].bone36, &frame.pointX);
        BattleFx_LimitOmnislashSpawnDampedSpriteParticles(&frame.pointX, 0x64, 0x64, 1);
    }
    if (frame.timeline->state == 0xA9) {
        frame.entry = (BattleFxOmnislashSlashEntry *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitOmnislashSpritePairParticleEmitterTask);
        frame.entry->worldX = 0;
        frame.entry->velocityY = 0xFC18;
        frame.entry->worldZ = 0x3E80;
    }
    if (frame.timeline->state == 0xFA) {
        Battle_AllocateResourceSlot(BattleFx_LimitOmnislashWhiteFlashPulseTask);
    }
    if (g_YasuiLimitOmnislashSlashIndex < 0xF && frame.timeline->state == g_YasuiLimitOmnislashMotionTable1[g_YasuiLimitOmnislashSlashIndex] + 0x2A) {
        g_YasuiLimitOmnislashTargetActor = g_BattleHitTargetActors[g_YasuiLimitOmnislashSlashIndex];
        (g_YasuiLimitOmnislashBasePosX.posX) = g_BattleActorRecords[g_YasuiLimitOmnislashTargetActor].posX;
        g_YasuiLimitOmnislashBasePosX.posY = g_BattleActorRecords[g_YasuiLimitOmnislashTargetActor].posY;
        g_YasuiLimitOmnislashBasePosZ = g_BattleActorRecords[g_YasuiLimitOmnislashTargetActor].posZ;
        if (g_YasuiLimitOmnislashBasePosZ < frame.timeline->targetPosZ) {
            g_YasuiLimitOmnislashAttackAngle = 0;
        } else {
            g_YasuiLimitOmnislashAttackAngle = 0x800;
        }
        frame.param = g_BattleActorRecords[g_YasuiLimitOmnislashTargetActor].radius + 0x5DC;
        (g_YasuiLimitOmnislashTargetPosX.posX) = (g_YasuiLimitOmnislashBasePosX.posX) + (SinFixed(g_YasuiLimitOmnislashAttackAngle) * frame.param >> 0xC);
        g_YasuiLimitOmnislashTargetPosX.posY = 0;
        g_YasuiLimitOmnislashTargetPosZ = g_YasuiLimitOmnislashBasePosZ + (CosFixed(g_YasuiLimitOmnislashAttackAngle) * frame.param >> 0xC);
        g_BattleActorRecords[g_YasuiLimitOmnislashSourceActor].posX = (g_YasuiLimitOmnislashTargetPosX.posX);
        g_BattleActorRecords[g_YasuiLimitOmnislashSourceActor].posY = g_YasuiLimitOmnislashTargetPosX.posY;
        g_BattleActorRecords[g_YasuiLimitOmnislashSourceActor].posZ = g_YasuiLimitOmnislashTargetPosZ;
        g_BattleActorRecords[g_YasuiLimitOmnislashSourceActor].rotX = 0;
        g_BattleActorRecords[g_YasuiLimitOmnislashSourceActor].yaw = g_YasuiLimitOmnislashAttackAngle;
        g_BattleActorRecords[g_YasuiLimitOmnislashSourceActor].rotZ = 0;
        g_YasuiLimitOmnislashSlashDuration = g_YasuiLimitOmnislashMotionTable2[g_YasuiLimitOmnislashSlashIndex] - g_YasuiLimitOmnislashMotionTable1[g_YasuiLimitOmnislashSlashIndex];
        Battle_CreateEffectTrailTask(g_YasuiLimitOmnislashSourceActor, -1, 0x320, 0x208020, 4, (&g_YasuiLimitOmnislashMotionTable1[1])[g_YasuiLimitOmnislashSlashIndex] - g_YasuiLimitOmnislashMotionTable1[g_YasuiLimitOmnislashSlashIndex]);
        Battle_GetBoneWorldPos(g_YasuiLimitOmnislashSourceActor, g_BattleActorRecords[g_YasuiLimitOmnislashSourceActor].bone36, &frame.pointX);
        BattleFx_LimitOmnislashSpawnDampedSpriteParticles(&frame.pointX, 0xC8, -0x64, 4);
    }
    if (g_YasuiLimitOmnislashSlashDuration > 0) {
        g_YasuiLimitOmnislashSlashDuration--;
        if (g_YasuiLimitOmnislashSlashDuration == 0) {
            frame.ringState = (BattleFxLimitOmnislashState *)Battle_AllocSceneMessage(0xA);
            frame.ringState->targetActor = g_YasuiLimitOmnislashTargetActor;
            frame.ringState->slashIndex = g_YasuiLimitOmnislashSlashIndex;
            frame.entry = (BattleFxOmnislashSlashEntry *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitOmnislashBoneSpriteEmitterTask);
            frame.entry->targetActor = g_YasuiLimitOmnislashTargetActor;
            frame.entry->animValue = g_YasuiLimitOmnislashMotionTable5[g_YasuiLimitOmnislashSlashIndex];
            frame.entry->motionId = g_YasuiLimitOmnislashMotionTable6[g_YasuiLimitOmnislashSlashIndex];
            if (g_YasuiLimitOmnislashSlashIndex != 0xE) {
                frame.entry = (BattleFxOmnislashSlashEntry *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitOmnislashScalingModelEntry0Task);
                Battle_GetBoneWorldPos(g_YasuiLimitOmnislashTargetActor, g_BattleActorRecords[g_YasuiLimitOmnislashTargetActor].targetBone, &frame.entry->worldX);
                frame.param = g_YasuiLimitOmnislashMotionTable4[g_YasuiLimitOmnislashSlashIndex];
                if (frame.param == -1) {
                    frame.angleX.vx = 0x400;
                    frame.angleX.vy = 0;
                    frame.angleX.vz = 0;
                } else {
                    frame.angleX.vx = 0;
                    frame.angleX.vy = g_YasuiLimitOmnislashAttackAngle;
                    frame.angleX.vz = frame.param;
                }
                frame.entry->posX = frame.angleX.vx;
                frame.entry->posY = frame.angleX.vy;
                frame.entry->posZ = frame.angleX.vz;
                frame.entry->animValue = g_YasuiLimitOmnislashMotionTable3[g_YasuiLimitOmnislashSlashIndex];
                frame.entry->rampValue = 0;
                frame.entry->motionId = 4;
                frame.count = g_YasuiLimitOmnislashMotionTable7[g_YasuiLimitOmnislashSlashIndex];
                if (frame.count > 0) {
                    frame.count <<= 1;
                    frame.param = 0x1000 / frame.count;
                    for (frame.index = 0; frame.index < frame.count; (frame.index)++) {
                        frame.radius = rand() % 0xC8 + 0xC8;
                        frame.entry = (BattleFxOmnislashSlashEntry *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitOmnislashMovingPulsingModelTask);
                        Battle_GetBoneWorldPos(g_YasuiLimitOmnislashTargetActor, g_BattleActorRecords[g_YasuiLimitOmnislashTargetActor].targetBone, &frame.entry->worldX);
                        frame.pointX = SinFixed(frame.index * frame.param) * frame.radius >> 0xC;
                        frame.pointY = 0;
                        frame.pointZ = CosFixed(frame.index * frame.param) * frame.radius >> 0xC;
                        RotMatrixXYZ(&frame.angleX, &frame.rotMatrix);
                        frame.rotMatrix.t[2] = 0;
                        frame.rotMatrix.t[1] = frame.rotMatrix.t[2];
                        frame.rotMatrix.t[0] = frame.rotMatrix.t[1];
                        SetRotMatrix(&frame.rotMatrix);
                        SetTransMatrix(&frame.rotMatrix);
                        RotTransPointToInt16(&frame.pointX, &frame.entry->posX, &frame.rotFlag);
                        frame.entry->animValue = rand() % 0x6000 + 0x2000;
                    }
                }
                frame.param = g_YasuiLimitOmnislashMotionTable8[g_YasuiLimitOmnislashSlashIndex];
                if (frame.param > 0) {
                    frame.entry = (BattleFxOmnislashSlashEntry *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitOmnislashSpinningPulsingModelTask);
                    Battle_GetBoneWorldPos(g_YasuiLimitOmnislashTargetActor, g_BattleActorRecords[g_YasuiLimitOmnislashTargetActor].targetBone, &frame.entry->worldX);
                    frame.entry->velocityY = 0;
                    frame.entry->animValue = frame.param;
                }
                if (g_YasuiLimitOmnislashSlashIndex <= 0xD) {
                    if (g_YasuiLimitOmnislashSlashIndex == 0xD) {
                        g_BattleStageHideFlags |= 7;
                        g_BattleFxTimelineState.red = 0;
                        g_BattleFxTimelineState.channel1 = 0;
                        g_BattleFxTimelineState.channel2 = 0;
                    } else {
                        g_BattleFxTimelineState.red = g_YasuiLimitOmnislashMotionTable9.keys[g_YasuiLimitOmnislashSlashIndex].red;
                        g_BattleFxTimelineState.channel1 = g_YasuiLimitOmnislashMotionTable9.keys[g_YasuiLimitOmnislashSlashIndex].green;
                        g_BattleFxTimelineState.channel2 = g_YasuiLimitOmnislashMotionTable9.keys[g_YasuiLimitOmnislashSlashIndex].blue;
                    }
                }
                SoundDispatchCommand(SOUND_SFX_PLAY_CH1, Battle_ComputeScreenPanFromPosition(&g_BattleActorRecords[g_YasuiLimitOmnislashSourceActor].posX), 0x13C, 0, 0, 0, 0, 0, 0);
                SoundDispatchCommand(SOUND_SFX_PLAY_CH2, Battle_ComputeScreenPanFromPosition(&g_BattleActorRecords[g_YasuiLimitOmnislashSourceActor].posX), 0x1B, 0, 0, 0, 0, 0, 0);
            } else {
                for (frame.index = 0; frame.index < 4; (frame.index)++) {
                    frame.entry = (BattleFxOmnislashSlashEntry *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitOmnislashScalingModelEntry1Task);
                    Battle_GetBoneWorldPos(g_YasuiLimitOmnislashTargetActor, g_BattleActorRecords[g_YasuiLimitOmnislashTargetActor].targetBone, &frame.entry->worldX);
                    frame.entry->posX = 0;
                    frame.entry->posY = 0;
                    frame.entry->posZ = frame.index << 9;
                    frame.entry->animValue = g_YasuiLimitOmnislashAngleOffset;
                    frame.entry->rampValue = 0;
                    frame.entry->motionId = 6;
                }
                for (frame.index = 0; frame.index < g_YasuiLimitOmnislashDurationConst; (frame.index)++) {
                    frame.entry = (BattleFxOmnislashSlashEntry *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitOmnislashMovingPulsingModelTask);
                    Battle_GetBoneWorldPos(g_YasuiLimitOmnislashTargetActor, g_BattleActorRecords[g_YasuiLimitOmnislashTargetActor].targetBone, &frame.entry->worldX);
                    frame.pointX = 0;
                    frame.pointY = 0;
                    frame.pointZ = rand() % 0xC8 + 0x190;
                    frame.angleX.vx = rand() % 0x1000;
                    frame.angleX.vy = rand() % 0x1000;
                    frame.angleX.vz = 0;
                    RotMatrixXYZ(&frame.angleX, &frame.rotMatrix);
                    frame.rotMatrix.t[2] = 0;
                    frame.rotMatrix.t[1] = frame.rotMatrix.t[2];
                    frame.rotMatrix.t[0] = frame.rotMatrix.t[1];
                    SetRotMatrix(&frame.rotMatrix);
                    SetTransMatrix(&frame.rotMatrix);
                    RotTransPointToInt16(&frame.pointX, &frame.entry->posX, &frame.rotFlag);
                    frame.entry->animValue = rand() % 0x2000 + 0x6000;
                }
                SoundDispatchCommand(SOUND_SFX_PLAY_CH1, Battle_ComputeScreenPanFromPosition(&g_BattleActorRecords[g_YasuiLimitOmnislashSourceActor].posX), 0x285, 0, 0, 0, 0, 0, 0);
                SoundDispatchCommand(SOUND_SFX_PLAY_CH2, Battle_ComputeScreenPanFromPosition(&g_BattleActorRecords[g_YasuiLimitOmnislashSourceActor].posX), 0x22A, 0, 0, 0, 0, 0, 0);
            }
            if (g_YasuiLimitOmnislashSlashIndex >= 5) {
                frame.entry = (BattleFxOmnislashSlashEntry *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitOmnislashGrowingBillboardModelTask);
                Battle_GetBoneWorldPos(g_YasuiLimitOmnislashTargetActor, g_BattleActorRecords[g_YasuiLimitOmnislashTargetActor].targetBone, &frame.entry->worldX);
                frame.entry->animValue = 0x3000;
                frame.entry->rampValue = 0;
            }
            g_YasuiLimitOmnislashSlashIndex++;
        }
    }
    if (frame.timeline->state == 0x109) {
        g_BattleStageHideFlags &= 0xF8;
        g_BattleFxTimelineState.startValue = 0;
    }
    (frame.timeline->state)++;
    if (frame.timeline->state == 0x118) {
        g_BattleScriptEffectFlag = 0;
        g_YasuiLimitOmnislashActiveFlag = 1;
        frame.timeline->ownerSlot = BATTLE_TASK_FREE;
        Sfx_ReleaseCachedBuffer(0x28E, 0xF);
        Sfx_ReleaseCachedBuffer(0x188, 0xF);
        Sfx_ReleaseCachedBuffer(0x1A, 0xF);
        Sfx_ReleaseCachedBuffer(0x13C, 0xF);
        Sfx_ReleaseCachedBuffer(0x1B, 0xF);
        Sfx_ReleaseCachedBuffer(0x30, 0xF);
        Sfx_ReleaseCachedBuffer(0x241, 0xF);
        Sfx_ReleaseCachedBuffer(0x189, 0xF);
        Sfx_ReleaseCachedBuffer(0x285, 0xF);
        Sfx_ReleaseCachedBuffer(0x22A, 0xF);
        Sfx_ReleaseCachedBuffer(6, 0xF);
    }
}

/* Renders scaling model entry 0 with interpolated size progression. */
// FUNCTION: FF7 0x0046c9f2
void __cdecl BattleFx_LimitOmnislashScalingModelEntry0Task(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[84];
        PsxMatrix xform;
        struct { unsigned char pad_to_20[0x20]; YasuiLimitOmnislashScaleEntry0 *entry; };
        struct { unsigned char pad_to_24[0x24]; PsxMatrix viewMatrix; };
        struct { unsigned char pad_to_44[0x44]; VECTOR scaleX; };
    } frame;
#pragma pack(pop)

    frame.entry = (YasuiLimitOmnislashScaleEntry0 *)BATTLE_CURRENT_TASK_SLOT;
    (g_YasuiLimitOmnislashDrawReqPool1.modelRes) = Gfx_ModelTableGetRenderObject(0, g_YasuiLimitOmnislashResourceHandle);
    RotMatrixXYZ(&frame.entry->rotX, &frame.xform);
    frame.scaleX.vz = frame.entry->baseScale * frame.entry->scalePercent / 0x64;
    frame.scaleX.vy = frame.scaleX.vz;
    frame.scaleX.vx = frame.scaleX.vy;
    ScaleMatrix(&frame.xform, &frame.scaleX);
    frame.xform.t[0] = frame.entry->posX;
    frame.xform.t[1] = frame.entry->posY;
    frame.xform.t[2] = frame.entry->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.xform, &frame.viewMatrix);
    SetRotMatrix(&frame.viewMatrix);
    SetTransMatrix(&frame.viewMatrix);
    SetDrawColorRGB(0, 0, 0);
    g_YasuiLimitOmnislashDrawReqPool1.alpha0 = frame.entry->timer * 0x199;
    g_YasuiLimitOmnislashPacketCursor = BattleFx_SubmitModelDraw(&g_YasuiLimitOmnislashDrawReqPool1, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_YasuiLimitOmnislashPacketCursor);
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.entry->scalePercent += (short)((0x64 - frame.entry->scalePercent) / frame.entry->scaleDivisor);
    (frame.entry->timer)++;
    if (frame.entry->timer == 0xA) {
        frame.entry->ownerSlot = BATTLE_TASK_FREE;
    }
}

/* Renders scaling model entry 1 with interpolated size progression. */
// FUNCTION: FF7 0x0046cb5d
void __cdecl BattleFx_LimitOmnislashScalingModelEntry1Task(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[84];
        PsxMatrix xform;
        struct { unsigned char pad_to_20[0x20]; YasuiLimitOmnislashScaleEntry0 *entry; };
        struct { unsigned char pad_to_24[0x24]; PsxMatrix viewMatrix; };
        struct { unsigned char pad_to_44[0x44]; VECTOR scaleX; };
    } frame;
#pragma pack(pop)

    frame.entry = (YasuiLimitOmnislashScaleEntry0 *)BATTLE_CURRENT_TASK_SLOT;
    g_YasuiLimitOmnislashDrawReqPool1.modelRes1 = Gfx_ModelTableGetRenderObject(1, g_YasuiLimitOmnislashResourceHandle);
    RotMatrixXYZ(&frame.entry->rotX, &frame.xform);
    frame.scaleX.vz = frame.entry->baseScale * frame.entry->scalePercent / 0x96;
    frame.scaleX.vy = frame.scaleX.vz;
    frame.scaleX.vx = frame.scaleX.vy;
    ScaleMatrix(&frame.xform, &frame.scaleX);
    frame.xform.t[0] = frame.entry->posX;
    frame.xform.t[1] = frame.entry->posY;
    frame.xform.t[2] = frame.entry->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.xform, &frame.viewMatrix);
    SetRotMatrix(&frame.viewMatrix);
    SetTransMatrix(&frame.viewMatrix);
    SetDrawColorRGB(0, 0, 0);
    g_YasuiLimitOmnislashDrawReqPool1.alpha1 = frame.entry->timer * 0x111;
    g_BattleOverlayDrawListHead = BattleFx_SubmitModelDraw(&g_YasuiLimitOmnislashDrawReqPool1.modelRes1, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.entry->scalePercent += (short)((0x96 - frame.entry->scalePercent) / frame.entry->scaleDivisor);
    (frame.entry->timer)++;
    if (frame.entry->timer == 0xF) {
        frame.entry->ownerSlot = BATTLE_TASK_FREE;
    }
}

/* Updates position and pulsing scale for transient energy particle models. */
// FUNCTION: FF7 0x0046ccc8
void __cdecl BattleFx_LimitOmnislashMovingPulsingModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[68];
        PsxMatrix xform;
        struct { unsigned char pad_to_20[0x20]; BattleFxLimitOmnislashPulse *pulse; };
        struct { unsigned char pad_to_24[0x24]; PsxMatrix viewMatrix; };
    } frame;
#pragma pack(pop)

    frame.pulse = (BattleFxLimitOmnislashPulse *)BATTLE_CURRENT_TASK_SLOT;
    g_YasuiLimitOmnislashDrawReqPool1.modelRes2 = Gfx_ModelTableGetRenderObject(3, g_YasuiLimitOmnislashResourceHandle);
    Gte_BuildLookBasisFromDirection(&frame.pulse->velocityX, frame.xform.m[0]);
    g_YasuiLimitOmnislashSineOffset = SinFixed(frame.pulse->phase * 0xCC) * frame.pulse->amplitude >> 0xC;
    ScaleMatrix(&frame.xform, &g_YasuiLimitOmnislashScaleVector1);
    frame.xform.t[0] = frame.pulse->posX;
    frame.xform.t[1] = frame.pulse->posY;
    frame.xform.t[2] = frame.pulse->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.xform, &frame.viewMatrix);
    SetRotMatrix(&frame.viewMatrix);
    SetTransMatrix(&frame.viewMatrix);
    SetDrawColorRGB(0, 0, 0);
    g_YasuiLimitOmnislashPacketCursor = BattleFx_SubmitModelDraw(&g_YasuiLimitOmnislashDrawReqPool1.modelRes2, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_YasuiLimitOmnislashPacketCursor);
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.pulse->posX += frame.pulse->velocityX;
    frame.pulse->posY += frame.pulse->velocityY;
    frame.pulse->posZ += frame.pulse->velocityZ;
    (frame.pulse->phase)++;
    if (frame.pulse->phase == 0xA) {
        frame.pulse->ownerSlot = BATTLE_TASK_FREE;
    }
}

/* Emits paired sprite particles with opposing initial velocities. */
// FUNCTION: FF7 0x0046ce36
void __cdecl BattleFx_LimitOmnislashSpritePairParticleEmitterTask(void)
{
    BattleFxBlasterFlashSlot *taskPtr;
    int distance;
    int theta;
    int spawnIndex;
    BattleFxOmnislashParticle *ent;
    char u[0x20];

    taskPtr = (BattleFxBlasterFlashSlot *)BATTLE_CURRENT_TASK_SLOT;
    {
        SVECTOR v = {0, 0, 0};
        short w;
        int x;

        g_YasuiLimitOmnislashDrawReqPool1.spriteRecord = Gfx_ModelTableGetLoadedRecord(0xE, g_YasuiLimitOmnislashResourceHandle);
        g_YasuiLimitOmnislashDrawReqPool1.spriteObject = Gfx_ModelTableGetSpriteObject(0xE, g_YasuiLimitOmnislashResourceHandle);
        v.vz = -taskPtr->phase << 5;
        BattleFx_LimitOmnislashInstallRotatedScaledMatrix((BattleFxLimitOmnislashMatrix *)((int)taskPtr + 4), &v, 0x4000);
        g_YasuiLimitOmnislashSpriteTint.limitBusinSpriteTintByte2 = g_YasuiLimitOmnislashAlphaPhase << 3;
        g_YasuiLimitOmnislashSpriteTint.limitBusinSpriteTintByte1 = g_YasuiLimitOmnislashSpriteTint.limitBusinSpriteTintByte2;
        (g_YasuiLimitOmnislashSpriteTint.currentTintByte) = g_YasuiLimitOmnislashSpriteTint.limitBusinSpriteTintByte1;
        g_YasuiLimitOmnislashPacketCursor = BattleFx_SubmitSpriteDraw(&g_YasuiLimitOmnislashDrawReqPool1.spriteRecord, g_BattleCurrentOrderingTable + BATTLE_OT_AFTER_SORTED_0, 1, g_YasuiLimitOmnislashPacketCursor);
        v.vz = taskPtr->phase << 5;
        BattleFx_LimitOmnislashInstallRotatedScaledMatrix((BattleFxLimitOmnislashMatrix *)((int)taskPtr + 4), &v, 0x3000);
        g_YasuiLimitOmnislashSpriteTint.limitBusinSpriteTintByte2 = g_YasuiLimitOmnislashAlphaPhase << 3;
        g_YasuiLimitOmnislashSpriteTint.limitBusinSpriteTintByte1 = g_YasuiLimitOmnislashSpriteTint.limitBusinSpriteTintByte2;
        (g_YasuiLimitOmnislashSpriteTint.currentTintByte) = g_YasuiLimitOmnislashSpriteTint.limitBusinSpriteTintByte1;
        g_YasuiLimitOmnislashPacketCursor = BattleFx_SubmitSpriteDraw(&g_YasuiLimitOmnislashDrawReqPool1.spriteRecord, g_BattleCurrentOrderingTable + BATTLE_OT_AFTER_SORTED_0, 1, g_YasuiLimitOmnislashPacketCursor);
        if (g_BattleIsPaused != 0) {
            return;
        }
        w = 8 - (SinFixed(taskPtr->phase * 0xE + 0x400) << 3 >> 0xC);
        for (spawnIndex = 0; spawnIndex < w; spawnIndex++) {
            ent = &g_YasuiLimitOmnislashParticleTable[BattleFx_LimitOmnislashClaimParticleRecord()];
            ent->posX = 0;
            ent->posY = 0xFC18;
            ent->posZ = 0x36B0;
            ent->velocityZ = 0xFB50;
            ent->velocityY = 0x50;
            ent->spriteIndex = taskPtr->phase * 0x78 + 0x800;
            theta = rand() % 0x1000;
            ent->angle = (short)theta;
            distance = rand() % 0x1F4 + 0x1F4;
            ent->posX += (short)(CosFixed(theta) * distance >> 0xC);
            ent->posY += (short)(SinFixed(theta) * distance >> 0xC);
        }
        if (taskPtr->phase < 0x10) {
            g_YasuiLimitOmnislashAlphaPhase++;
        }
        (taskPtr->phase)++;
        if (taskPtr->phase == 0x55) {
            taskPtr->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/* Builds rotated and scaled transformation matrix for limit effect particles. */
// FUNCTION: FF7 0x0046d0f4
int __cdecl BattleFx_LimitOmnislashInstallRotatedScaledMatrix(BattleFxLimitOmnislashMatrix *matrix, SVECTOR *rotation, short scale)
{
    VECTOR zScale;

    RotMatrix(rotation, &g_YasuiLimitOmnislashCylinderData.rotMatrix);
    zScale.vz = scale;
    zScale.vy = zScale.vz;
    zScale.vx = zScale.vy;
    ScaleMatrix(&g_YasuiLimitOmnislashCylinderData.rotMatrix, &zScale);
    g_YasuiLimitOmnislashCylinderData.rotMatrix.t[0] = matrix->vx;
    g_YasuiLimitOmnislashCylinderData.rotMatrix.t[1] = matrix->vy;
    g_YasuiLimitOmnislashCylinderData.rotMatrix.t[2] = matrix->vz;
    SetRotMatrix(&g_YasuiLimitOmnislashCylinderData.rotMatrix);
    SetTransMatrix(&g_YasuiLimitOmnislashCylinderData.rotMatrix);
    return (int)&g_YasuiLimitOmnislashCylinderData.rotMatrix;
}

/* Emits shrinking sword slash models anchored to character attachment points. */
// FUNCTION: FF7 0x0046d177
void __cdecl BattleFx_LimitOmnislashBoneModelShrinkingEmitterTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[80];
        SVECTOR boneX;
        struct { unsigned char pad_to_28[0x28]; BattleFxOmnislashShrinkRecord *sineModel; };
        struct { unsigned char pad_to_2C[0x2C]; BattleFxShrinkingEmitterState *emitter; };
    } frame;
#pragma pack(pop)

    frame.emitter = (BattleFxShrinkingEmitterState *)BATTLE_CURRENT_TASK_SLOT;
    (g_YasuiLimitOmnislashDrawReqPool2.modelRes) = Gfx_ModelTableGetRenderObject(6, g_YasuiLimitOmnislashResourceHandle);
    Battle_GetBoneWorldPos(g_YasuiLimitOmnislashSourceActor, g_BattleActorRecords[g_YasuiLimitOmnislashSourceActor].primaryBone, &frame.boneX.vx);
    frame.boneX.vy -= 0x2BC;
    BattleFx_InstallBillboardMatrix(&frame.boneX, (short)frame.emitter->scale, 0xC8);
    SetDrawColorRGB(0, 0, 0);
    g_YasuiLimitOmnislashPacketCursor = BattleFx_SubmitModelDraw(&g_YasuiLimitOmnislashDrawReqPool2, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_YasuiLimitOmnislashPacketCursor);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.emitter->timer < 0xA) {
        frame.emitter->scale = frame.emitter->scale * 0xC0 / 0x100;
        g_YasuiLimitOmnislashStepCounter++;
    } else if (frame.emitter->timer < 0x1C) {
        frame.sineModel = (BattleFxOmnislashShrinkRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitOmnislashSineScaledModelTask);
        frame.sineModel->anchorX = frame.boneX.vx;
        frame.sineModel->anchorY = frame.boneX.vy;
        frame.sineModel->anchorZ = frame.boneX.vz;
        frame.sineModel->randomOffset = rand() % 0x1000;
    }
    if (frame.emitter->timer >= 0x20) {
        frame.emitter->scale = frame.emitter->scale * 0xE0 / 0x100;
    }
    (frame.emitter->timer)++;
    if (frame.emitter->timer == 0x28) {
        frame.emitter->ownerSlot = BATTLE_TASK_FREE;
    }
}

/* Updates model geometry with sinusoidal scale modulation envelope. */
// FUNCTION: FF7 0x0046d32c
void __cdecl BattleFx_LimitOmnislashSineScaledModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[72];
        int rotFlag;
        struct { unsigned char pad_to_04[0x4]; PsxMatrix viewMatrix; };
        struct { unsigned char pad_to_24[0x24]; BattleFxLimitOmnislashSineModel *sineModel; };
    } frame;
#pragma pack(pop)

    frame.sineModel = (BattleFxLimitOmnislashSineModel *)BATTLE_CURRENT_TASK_SLOT;
    g_YasuiLimitOmnislashDrawReqPool1.modelRes3 = Gfx_ModelTableGetRenderObject(7, g_YasuiLimitOmnislashResourceHandle);
    SetRotMatrix(&g_BattleCameraTransform);
    SetTransMatrix(&g_BattleCameraTransform);
    RotTransPoint(&frame.sineModel->posX, &frame.viewMatrix.t[0], &frame.rotFlag);
    g_YasuiLimitOmnislashModelPitch = frame.sineModel->modelPitch;
    RotMatrix((SVECTOR *)&g_YasuiLimitOmnislashModelRotX, &frame.viewMatrix);
    g_YasuiLimitOmnislashScaleInterp3 = SinFixed(frame.sineModel->step << 8) * 0x7F00 >> 0xC;
    ScaleMatrix(&frame.viewMatrix, &g_YasuiLimitOmnislashScaleInterp3);
    SetRotMatrix(&frame.viewMatrix);
    SetTransMatrix(&frame.viewMatrix);
    SetDrawColorRGB(0, 0, 0);
    g_YasuiLimitOmnislashPacketCursor = BattleFx_SubmitModelDraw(&g_YasuiLimitOmnislashDrawReqPool1.modelRes3, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_YasuiLimitOmnislashPacketCursor);
    if (g_BattleIsPaused != 0) {
        return;
    }
    (frame.sineModel->step)++;
    if (frame.sineModel->step == 8) {
        frame.sineModel->ownerSlot = BATTLE_TASK_FREE;
    }
}

/* Renders expanding billboarded slash flash model facing the camera. */
// FUNCTION: FF7 0x0046d45d
void __cdecl BattleFx_LimitOmnislashGrowingBillboardModelTask(void)
{
    int p;
    int q;
    int r;
    int s;
    BattleFxOmnislashBillboardRecord *t;
    int u;
    int v;
    int w;
    int x;
    int y;
    int z;
    int a;
    int b;
    int c;
    int d;
    int e;
    int f;

    t = (BattleFxOmnislashBillboardRecord *)BATTLE_CURRENT_TASK_SLOT;
    g_YasuiLimitOmnislashDrawReqPool2.ringModelRes = Gfx_ModelTableGetRenderObject(4, g_YasuiLimitOmnislashResourceHandle);
    BattleFx_InstallBillboardMatrix((void *)((int)t + 4), (short)t->angle, 0);
    SetDrawColorRGB(0, 0, 0);
    g_YasuiLimitOmnislashDrawReqPool2.ringScale = t->spin;
    g_YasuiLimitOmnislashPacketCursor = BattleFx_SubmitModelDraw(&g_YasuiLimitOmnislashDrawReqPool2.ringModelRes, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_YasuiLimitOmnislashPacketCursor);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (t->frameCount < 2) {
        t->angle += 0x2000;
    }
    if (t->frameCount >= 2) {
        t->spin += 0x2A8;
    }
    (t->frameCount)++;
    if (t->frameCount == 8) {
        t->ownerSlot = BATTLE_TASK_FREE;
    }
}

/* Spawns explosion burst of velocity-damped impact spark sprites. */
// FUNCTION: FF7 0x0046d558
void __cdecl BattleFx_LimitOmnislashSpawnDampedSpriteParticles(short *origin, short count, short speed, short angle)
{
    short theta;
    BattleFxOmnislashDampedSprite *spark;
    short sparkIdx;

    for (sparkIdx = 0; sparkIdx < angle; sparkIdx++) {
        spark = (BattleFxOmnislashDampedSprite *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitOmnislashDampedSpriteParticleTask);
        spark->originX = origin[0];
        spark->originY = origin[1];
        spark->originZ = origin[2];
        theta = rand() % 0x1000;
        spark->velocityX = SinFixed(theta) * count >> 0xC;
        spark->velocityZ = CosFixed(theta) * count >> 0xC;
        spark->lifetime = rand() % 0x1800 + 0x1000;
        spark->speed = speed;
    }
}

/* Updates physics and fading alpha for damped hit spark particle. */
// FUNCTION: FF7 0x0046d647
void __cdecl BattleFx_LimitOmnislashDampedSpriteParticleTask(void)
{
    BattleFxOmnislashParticle *task;

    task = (BattleFxOmnislashParticle *)BATTLE_CURRENT_TASK_SLOT;
    g_YasuiLimitOmnislashDrawReqPool2.sparkSpriteRecord = Gfx_ModelTableGetLoadedRecord(0xD, g_YasuiLimitOmnislashResourceHandle);
    g_YasuiLimitOmnislashDrawReqPool2.sparkSpriteObject = Gfx_ModelTableGetSpriteObject(0xD, g_YasuiLimitOmnislashResourceHandle);
    BattleFx_InstallBillboardMatrix((void *)((int)task + 4), (short)task->spriteIndex, 0);
    g_YasuiLimitOmnislashDrawReqPool2.limitBusinDrawReqPool2Frame = task->frame;
    g_BattleOverlayDrawListHead = BattleFx_SubmitSpriteDraw(&g_YasuiLimitOmnislashDrawReqPool2.sparkSpriteRecord, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused == 0) {
        task->posX += task->velocityX;
        task->posY += task->velocityY;
        task->posZ += task->velocityZ;
        task->velocityX = task->velocityX * 0xC0 / 0x100;
        task->velocityY = task->velocityY * 0xC0 / 0x100;
        task->velocityZ = task->velocityZ * 0xC0 / 0x100;
        if (g_YasuiLimitOmnislashDrawReqPool2.sparkSpriteRecord != 0) {
            (task->frame)++;
            if (task->frame == *(int *)((char *)*(int *)g_YasuiLimitOmnislashDrawReqPool2.sparkSpriteRecord + 4)) {
                task->ownerSlot = BATTLE_TASK_FREE;
            }
        }
    }
}

/* Emits animated slash sprites attached to character weapon bones. */
// FUNCTION: FF7 0x0046d7be
void __cdecl BattleFx_LimitOmnislashBoneSpriteEmitterTask(void)
{
    BattleFxOmnislashBoneEmitterRecord *taskPtr;
    short boneRadius;
    BattleFxMissileSalvoImpactSlot *slashSprite;

    taskPtr = (BattleFxOmnislashBoneEmitterRecord *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    slashSprite = (BattleFxMissileSalvoImpactSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitOmnislashSpriteAnimationTask);
    Battle_GetBoneWorldPos(taskPtr->boneIndex, g_BattleActorRecords[taskPtr->boneIndex].targetBone, (short *)((int)slashSprite + 4));
    boneRadius = g_BattleActorRecords[taskPtr->boneIndex].radius;
    slashSprite->posX += (short)((rand() % boneRadius << 1) - boneRadius);
    slashSprite->posY += (short)((rand() % boneRadius << 1) - boneRadius);
    slashSprite->posZ += (short)((rand() % boneRadius << 1) - boneRadius);
    slashSprite->scale = taskPtr->spriteScale;
    (taskPtr->frame)++;
    if (taskPtr->frame == taskPtr->endFrame) {
        taskPtr->ownerSlot = BATTLE_TASK_FREE;
    }
}

/* Advances sprite frame index and coordinates fade-out progression. */
// FUNCTION: FF7 0x0046d8f4
void __cdecl BattleFx_LimitOmnislashSpriteAnimationTask(void)
{
    BattleFxDeathForceAnimSprite *task;

    task = (BattleFxDeathForceAnimSprite *)BATTLE_CURRENT_TASK_SLOT;
    g_YasuiLimitOmnislashDrawReqPool2.burstSpriteRecord = Gfx_ModelTableGetLoadedRecord(0xC, g_YasuiLimitOmnislashResourceHandle);
    g_YasuiLimitOmnislashDrawReqPool2.burstSpriteObject = Gfx_ModelTableGetSpriteObject(0xC, g_YasuiLimitOmnislashResourceHandle);
    BattleFx_InstallBillboardMatrix((void *)((int)task + 4), (short)task->scale, 0);
    g_YasuiLimitOmnislashDrawReqPool2.burstFrame = task->frame;
    g_BattleOverlayDrawListHead = BattleFx_SubmitSpriteDraw(&g_YasuiLimitOmnislashDrawReqPool2.burstSpriteRecord, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused == 0 && g_YasuiLimitOmnislashDrawReqPool2.burstSpriteRecord != 0) {
        (task->frame)++;
        if (task->frame == *(int *)((char *)*(int *)g_YasuiLimitOmnislashDrawReqPool2.burstSpriteRecord + 4)) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/* Coordinates dual spinning energy cylinder models during sword slash arcs. */
// FUNCTION: FF7 0x0046d9c8
void __cdecl BattleFx_LimitOmnislashSpinningCylinderPairTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[80];
        PsxMatrix xform;
        struct { unsigned char pad_to_2C[0x2C]; BattleFxLimitOmnislashCylinderPair *pair; };
        struct { unsigned char pad_to_30[0x30]; PsxMatrix viewMatrix; };
    } frame;
#pragma pack(pop)

    frame.pair = (BattleFxLimitOmnislashCylinderPair *)BATTLE_CURRENT_TASK_SLOT;
    g_YasuiLimitOmnislashRotationVector.limitBusinRotationVectorY = frame.pair->step << 7;
    RotMatrixXYZ(&g_YasuiLimitOmnislashRotationVector, &frame.xform);
    g_YasuiLimitOmnislashScalePulse3 = 0x6000;
    g_YasuiLimitOmnislashScalePulse1 = g_YasuiLimitOmnislashScalePulse3;
    ScaleMatrix(&frame.xform, &g_YasuiLimitOmnislashScalePulse1);
    frame.xform.t[0] = frame.pair->posX;
    frame.xform.t[1] = frame.pair->posY;
    frame.xform.t[2] = frame.pair->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.xform, &frame.viewMatrix);
    SetRotMatrix(&frame.viewMatrix);
    SetTransMatrix(&frame.viewMatrix);
    RefreshGlobalTransformState();
    BattleFx_LimitOmnislashDrawTexturedCylinder((short)g_YasuiLimitOmnislashCylinderData.spinStep);
    g_YasuiLimitOmnislashRotationVector.limitBusinRotationVectorY = frame.pair->step * 0xC4;
    RotMatrixXYZ(&g_YasuiLimitOmnislashRotationVector, &frame.xform);
    g_YasuiLimitOmnislashScalePulse3 = 0x7000;
    g_YasuiLimitOmnislashScalePulse1 = g_YasuiLimitOmnislashScalePulse3;
    ScaleMatrix(&frame.xform, &g_YasuiLimitOmnislashScalePulse1);
    frame.xform.t[0] = frame.pair->posX;
    frame.xform.t[1] = frame.pair->posY;
    frame.xform.t[2] = frame.pair->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.xform, &frame.viewMatrix);
    SetRotMatrix(&frame.viewMatrix);
    SetTransMatrix(&frame.viewMatrix);
    RefreshGlobalTransformState();
    BattleFx_LimitOmnislashDrawTexturedCylinder((short)g_YasuiLimitOmnislashCylinderData.spinStep);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.pair->step < 0x14) {
        g_YasuiLimitOmnislashScalePulse2 += (0x14 - frame.pair->step) * 0x64;
    }
    if (frame.pair->step < 0x30) {
        BattleFx_LimitOmnislashSpawnDampedSpriteParticles(&frame.pair->posX, 0x258, -0x64, 4);
    }
    if (frame.pair->step >= 0x34) {
        (g_YasuiLimitOmnislashCylinderData.spinStep) -= 0x10;
    }
    (frame.pair->step)++;
    if (frame.pair->step == 0x3C) {
        frame.pair->ownerSlot = BATTLE_TASK_FREE;
    }
}

/* Submits draw requests for textured cylinder mesh primitives. */
// FUNCTION: FF7 0x0046dbd4
void __cdecl BattleFx_LimitOmnislashDrawTexturedCylinder(short angle)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[72];
        int edgeByte;
        char edgeByteLow;
        struct { unsigned char pad_to_04[0x4]; int quadState; };
        struct { unsigned char pad_to_08[0x8]; int outFlags; };
        struct { unsigned char pad_to_0C[0xC]; int depth; };
        struct { unsigned char pad_to_10[0x10]; int colorWord; };
        struct { unsigned char pad_to_14[0x14]; short clut; };
        struct { unsigned char pad_to_18[0x18]; BattleFxTexturedCylinder *prim; };
        struct { unsigned char pad_to_1C[0x1C]; SVECTOR bottom1; };
        struct { unsigned char pad_to_24[0x24]; short ringIdx; };
        struct { unsigned char pad_to_28[0x28]; SVECTOR bottom0; };
        struct { unsigned char pad_to_30[0x30]; SVECTOR top1; };
        struct { unsigned char pad_to_38[0x38]; SVECTOR top0; };
        struct { unsigned char pad_to_40[0x40]; short tpage; };
        struct { unsigned char pad_to_44[0x44]; int outDepth; };
    } frame;
#pragma pack(pop)

    frame.tpage = 0;
    frame.clut = 0x7948;
    frame.colorWord = angle | angle << 8 | angle << 0x10;
    frame.prim = (BattleFxTexturedCylinder *)Gfx_GetPsxPacketScratchPtr();
    *((char *)frame.prim + 7) |= 2;
    for (frame.ringIdx = 0; frame.ringIdx < 0x20; (frame.ringIdx)++) {
        frame.bottom0.vx = SinFixed(frame.ringIdx << 7) * 0x64 >> 0xC;
        frame.top0.vx = frame.bottom0.vx;
        frame.bottom0.vz = CosFixed(frame.ringIdx << 7) * 0x64 >> 0xC;
        frame.top0.vz = frame.bottom0.vz;
        frame.bottom1.vx = SinFixed(frame.ringIdx + 1 << 7) * 0x64 >> 0xC;
        frame.top1.vx = frame.bottom1.vx;
        frame.bottom1.vz = CosFixed(frame.ringIdx + 1 << 7) * 0x64 >> 0xC;
        frame.top1.vz = frame.bottom1.vz;
        frame.top1.vy = 0xFE70;
        frame.top0.vy = frame.top1.vy;
        frame.bottom1.vy = 0;
        frame.bottom0.vy = frame.bottom1.vy;
        frame.depth = Gfx_TransformQuadReturnDepthAlt(&frame.top0, &frame.top1, &frame.bottom0, &frame.bottom1, &frame.outDepth, &frame.outFlags);
        if (frame.depth > 0) {
            frame.prim->tag = 0x9000000;
            frame.prim->colorWord = frame.colorWord | 0x2E000000;
            frame.prim->scaleStep = frame.ringIdx << 3;
            frame.prim->u0 = frame.prim->scaleStep;
            frame.prim->stepCount = frame.ringIdx * 8 + 8;
            if (*(unsigned char *)&frame.prim->stepCount == 0) {
                frame.edgeByte = 0xFF;
            } else {
                frame.edgeByte = *(unsigned char *)&frame.prim->stepCount;
            }
            frame.prim->u3 = frame.edgeByteLow;
            frame.prim->stepCount = frame.edgeByteLow;
            frame.prim->v1 = 0x80;
            frame.prim->v0 = 0x80;
            frame.prim->v3 = 0xFF;
            frame.prim->v2 = 0xFF;
            frame.prim->tpage = frame.tpage;
            frame.prim->clut = frame.clut;
            frame.quadState = Gfx_ModelTableGetSpriteObject(0xF, g_YasuiLimitOmnislashResourceHandle);
            if (frame.quadState != 0) {
                Gfx_SetPsxPrimQuadState(0xB, 1, (GfxPsxSpriteQuadState *)(frame.quadState));
            }
            Gfx_DrawPsxPrimitive(frame.prim);
        }
    }
    g_YasuiLimitOmnislashPacketCursor = (int)frame.prim;
}

/* Emits particle models with sine-modulated scale along sword trajectory. */
// FUNCTION: FF7 0x0046ddfa
void __cdecl BattleFx_LimitOmnislashSineScaledModelParticleEmitterTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[80];
        PsxMatrix xform;
        struct { unsigned char pad_to_2C[0x2C]; BattleFxSineScaleRecord *sineRecord; };
        struct { unsigned char pad_to_30[0x30]; PsxMatrix viewMatrix; };
    } frame;
#pragma pack(pop)

    frame.sineRecord = (BattleFxSineScaleRecord *)BATTLE_CURRENT_TASK_SLOT;
    (g_YasuiLimitOmnislashDrawReq1.modelRes) = Gfx_ModelTableGetRenderObject(8, g_YasuiLimitOmnislashResourceHandle);
    g_YasuiLimitOmnislashEulerAngles.limitBusinEulerAnglesY = frame.sineRecord->angle << 7;
    RotMatrixXYZ(&g_YasuiLimitOmnislashEulerAngles, &frame.xform);
    g_YasuiLimitOmnislashScale2 = SinFixed(frame.sineRecord->angle << 6) * 0x7F00 >> 0xC;
    g_YasuiLimitOmnislashScale1 = g_YasuiLimitOmnislashScale2;
    ScaleMatrix(&frame.xform, &g_YasuiLimitOmnislashScale1);
    frame.xform.t[0] = frame.sineRecord->posX;
    frame.xform.t[1] = frame.sineRecord->posY;
    frame.xform.t[2] = frame.sineRecord->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.xform, &frame.viewMatrix);
    SetRotMatrix(&frame.viewMatrix);
    SetTransMatrix(&frame.viewMatrix);
    g_YasuiLimitOmnislashDrawReq1.alpha = SinFixed(frame.sineRecord->angle << 7) << 7 >> 0xC;
    g_YasuiLimitOmnislashPacketCursor = BattleFx_SubmitModelDraw(&g_YasuiLimitOmnislashDrawReq1, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_YasuiLimitOmnislashPacketCursor);
    if (g_BattleIsPaused != 0) {
        return;
    }
    BattleFx_LimitOmnislashSpawnDampedSpriteParticles(&frame.sineRecord->posX, 0x258, -0x64, 1);
    (frame.sineRecord->angle)++;
    if (frame.sineRecord->angle == 0x10) {
        frame.sineRecord->ownerSlot = BATTLE_TASK_FREE;
    }
}

/* Renders rotating energy model with cyclic scale pulsation. */
// FUNCTION: FF7 0x0046df65
void __cdecl BattleFx_LimitOmnislashSpinningPulsingModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[80];
        PsxMatrix xform;
        struct { unsigned char pad_to_2C[0x2C]; BattleFxLimitOmnislashSpinRecord *spin; };
        struct { unsigned char pad_to_30[0x30]; PsxMatrix viewMatrix; };
    } frame;
#pragma pack(pop)

    frame.spin = (BattleFxLimitOmnislashSpinRecord *)BATTLE_CURRENT_TASK_SLOT;
    (g_YasuiLimitOmnislashDrawReq2.modelRes) = Gfx_ModelTableGetRenderObject(9, g_YasuiLimitOmnislashResourceHandle);
    g_YasuiLimitOmnislashEulerAngles.spinAngleY = frame.spin->angle << 7;
    RotMatrixXYZ(&g_YasuiLimitOmnislashEulerAngles.spinAngleX, &frame.xform);
    g_YasuiLimitOmnislashScale4 = SinFixed(frame.spin->angle << 6) * frame.spin->scaleAmplitude >> 0xC;
    g_YasuiLimitOmnislashScale3 = g_YasuiLimitOmnislashScale4;
    ScaleMatrix(&frame.xform, &g_YasuiLimitOmnislashScale3);
    frame.xform.t[0] = frame.spin->posX;
    frame.xform.t[1] = frame.spin->posY;
    frame.xform.t[2] = frame.spin->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.xform, &frame.viewMatrix);
    SetRotMatrix(&frame.viewMatrix);
    SetTransMatrix(&frame.viewMatrix);
    SetDrawColorRGB(0, 0, 0);
    g_YasuiLimitOmnislashDrawReq2.alpha = frame.spin->angle << 8;
    g_YasuiLimitOmnislashPacketCursor = BattleFx_SubmitModelDraw(&g_YasuiLimitOmnislashDrawReq2, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_YasuiLimitOmnislashPacketCursor);
    if (g_BattleIsPaused != 0) {
        return;
    }
    (frame.spin->angle)++;
    if (frame.spin->angle == 0x10) {
        frame.spin->ownerSlot = BATTLE_TASK_FREE;
    }
}

/* Emits expanding energy trail models anchored to character bones. */
// FUNCTION: FF7 0x0046e0ba
void __cdecl BattleFx_LimitOmnislashBoneModelGrowingEmitterTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[84];
        SVECTOR boneX;
        struct { unsigned char pad_to_28[0x28]; BattleFxLimitOmnislashBoneEmitter2 *rotatingModel; };
        struct { unsigned char pad_to_2C[0x2C]; short factor; };
        struct { unsigned char pad_to_30[0x30]; BattleFxGrowingEmitterState *emitter; };
    } frame;
#pragma pack(pop)

    frame.emitter = (BattleFxGrowingEmitterState *)BATTLE_CURRENT_TASK_SLOT;
    (g_YasuiLimitOmnislashDrawReq3.modelRes) = Gfx_ModelTableGetRenderObject(6, g_YasuiLimitOmnislashResourceHandle);
    Battle_GetBoneWorldPos(g_YasuiLimitOmnislashSourceActor, g_BattleActorRecords[g_YasuiLimitOmnislashSourceActor].primaryBone, &frame.boneX.vx);
    frame.factor = g_BattleActorRecords[g_YasuiLimitOmnislashSourceActor].yaw;
    frame.boneX.vz += (short)((frame.factor != 0 ? 0xFFFFFC18 : 0) + 0x1F4);
    frame.boneX.vy += 0x2BC;
    BattleFx_InstallBillboardMatrix(&frame.boneX, (short)frame.emitter->angle, 0xC8);
    SetDrawColorRGB(0, 0, 0);
    g_YasuiLimitOmnislashDrawReq3.alpha = g_YasuiLimitOmnislashSubAngle;
    g_YasuiLimitOmnislashPacketCursor = BattleFx_SubmitModelDraw(&g_YasuiLimitOmnislashDrawReq3, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_YasuiLimitOmnislashPacketCursor);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.emitter->timer < 0xA) {
        frame.factor = 0xA - frame.emitter->timer;
        frame.emitter->angle += (frame.factor * 0x14);
    }
    if (frame.emitter->timer < 0x1C) {
        frame.rotatingModel = (BattleFxLimitOmnislashBoneEmitter2 *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitOmnislashSineScaledRotatingModelTask);
        frame.rotatingModel->posX = frame.boneX.vx;
        frame.rotatingModel->posY = frame.boneX.vy;
        frame.rotatingModel->posZ = frame.boneX.vz;
        frame.rotatingModel->angle = rand() % 0x1000;
        frame.rotatingModel->scale = rand() % 0x1000 + 0x2000;
    }
    if (frame.emitter->timer >= 0x20) {
        frame.factor = 0x28 - frame.emitter->timer;
        frame.emitter->angle += (frame.factor * 0xC8);
        g_YasuiLimitOmnislashSubAngle += 0x200;
    }
    (frame.emitter->timer)++;
    if (frame.emitter->timer == 0x28) {
        frame.emitter->ownerSlot = BATTLE_TASK_FREE;
    }
}

/* Updates rotation and sinusoidal scale for omnislash sword slash aura. */
// FUNCTION: FF7 0x0046e2ea
void __cdecl BattleFx_LimitOmnislashSineScaledRotatingModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[72];
        int rotFlag;
        struct { unsigned char pad_to_04[0x4]; PsxMatrix viewMatrix; };
        struct { unsigned char pad_to_24[0x24]; BattleFxLimitOmnislashRotationTask *rotTask; };
    } frame;
#pragma pack(pop)

    frame.rotTask = (BattleFxLimitOmnislashRotationTask *)BATTLE_CURRENT_TASK_SLOT;
    g_YasuiLimitOmnislashDrawReqPool1.modelRes4 = Gfx_ModelTableGetRenderObject(0xA, g_YasuiLimitOmnislashResourceHandle);
    SetRotMatrix(&g_BattleCameraTransform);
    SetTransMatrix(&g_BattleCameraTransform);
    RotTransPoint(&frame.rotTask->posX, &frame.viewMatrix.t[0], &frame.rotFlag);
    g_YasuiLimitOmnislashModelYaw = frame.rotTask->yaw;
    RotMatrix((SVECTOR *)&g_YasuiLimitOmnislashModelRotY, &frame.viewMatrix);
    g_YasuiLimitOmnislashScaleInterp1 = SinFixed(frame.rotTask->phase << 8) * 0x7F00 >> 0xC;
    g_YasuiLimitOmnislashScaleInterp2 = frame.rotTask->scale;
    ScaleMatrix(&frame.viewMatrix, &g_YasuiLimitOmnislashScaleInterp1);
    SetRotMatrix(&frame.viewMatrix);
    SetTransMatrix(&frame.viewMatrix);
    SetDrawColorRGB(0, 0, 0);
    g_YasuiLimitOmnislashDrawReqPool1.alpha4 = 0x1000 - (SinFixed(frame.rotTask->phase << 8) << 0xC >> 0xC);
    g_YasuiLimitOmnislashPacketCursor = BattleFx_SubmitModelDraw(&g_YasuiLimitOmnislashDrawReqPool1.modelRes4, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_YasuiLimitOmnislashPacketCursor);
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.rotTask->yaw += 0x60;
    (frame.rotTask->phase)++;
    if (frame.rotTask->phase == 8) {
        frame.rotTask->ownerSlot = BATTLE_TASK_FREE;
    }
}

/* Updates full-screen white flash intensity decay envelope. */
// FUNCTION: FF7 0x0046e461
void __cdecl BattleFx_LimitOmnislashWhiteFlashPulseTask(void)
{
    BattleTaskSlot *taskPtr;
    int clamped;

    taskPtr = BATTLE_CURRENT_TASK_SLOT;
    g_BattleOverlayDrawListHead = BattleFx_FlashScreenWhite(*(char *)&g_YasuiLimitOmnislashFlashIntensity, *(char *)&g_YasuiLimitOmnislashFlashIntensity, *(char *)&g_YasuiLimitOmnislashFlashIntensity);
    if (g_BattleIsPaused != 0) {
        return;
    }
    g_YasuiLimitOmnislashFlashIntensity = SinFixed(taskPtr->frameCounter * 0x66) * 0x180 >> 0xC;
    if (g_YasuiLimitOmnislashFlashIntensity > 0xFF) {
        clamped = 0xFF;
    } else {
        clamped = g_YasuiLimitOmnislashFlashIntensity;
    }
    g_YasuiLimitOmnislashFlashIntensity = clamped;
    (taskPtr->frameCounter)++;
    if (taskPtr->frameCounter == 0x14) {
        taskPtr->ownerSlot = BATTLE_TASK_FREE;
    }
}

/* Initializes 200-slot particle table and active element count. */
// FUNCTION: FF7 0x0046e520
void __cdecl BattleFx_LimitOmnislashBeginParticlePool(void)
{
    BattleFxOmnislashParticle *particle;
    short slotIdx;

    particle = g_YasuiLimitOmnislashParticleTable;
    for (slotIdx = 0; slotIdx < 0xC8; slotIdx++) {
        particle->frame = 0;
        particle->ownerSlot = 0;
        particle++;
    }
    Battle_AllocateResourceSlot(BattleFx_LimitOmnislashParticlePoolTask);
}

/* Iterates and updates active particle records within shared pool. */
// FUNCTION: FF7 0x0046e57a
void __cdecl BattleFx_LimitOmnislashParticlePoolTask(void)
{
    BattleTaskSlot *taskPtr;
    BattleFxOmnislashParticle *particle;

    taskPtr = BATTLE_CURRENT_TASK_SLOT;
    particle = g_YasuiLimitOmnislashParticleTable;
    for (g_YasuiLimitOmnislashParticleIndex = 0; g_YasuiLimitOmnislashParticleIndex < 0xC8; g_YasuiLimitOmnislashParticleIndex++) {
        if (particle->ownerSlot == 1) {
            BattleFx_LimitOmnislashOrientedPoolParticleModel();
            if (particle->ownerSlot == -1) {
                particle->frame = 0;
                particle->ownerSlot = 0;
            }
        }
        particle++;
    }
    if (g_YasuiLimitOmnislashActiveFlag == 1) {
        taskPtr->ownerSlot = BATTLE_TASK_FREE;
    }
}

/* Dispatches model draw request for active pooled particle record. */
// FUNCTION: FF7 0x0046e614
void __cdecl BattleFx_LimitOmnislashOrientedPoolParticleModel(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[76];
        SVECTOR angleX;
        struct { unsigned char pad_to_08[0x8]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_28[0x28]; BattleFxOmnislashParticle *particle; };
        struct { unsigned char pad_to_2C[0x2C]; PsxMatrix lookBasis; };
    } frame;
#pragma pack(pop)

    frame.particle = &g_YasuiLimitOmnislashParticleTable[g_YasuiLimitOmnislashParticleIndex];
    g_YasuiLimitOmnislashDrawReqPool1.modelRes5 = Gfx_ModelTableGetRenderObject(0xB, g_YasuiLimitOmnislashResourceHandle);
    Gte_BuildLookBasisFromDirection(&frame.particle->velocityX, frame.lookBasis.m[0]);
    frame.angleX.vx = 0xFC00;
    frame.angleX.vy = frame.particle->angle + 0x400;
    frame.angleX.vz = 0;
    RotMatrixXYZ(&frame.angleX, &frame.rotMatrix);
    ConcatMatrixPreservingTranslation(&frame.lookBasis, &frame.rotMatrix, &frame.rotMatrix);
    g_YasuiLimitOmnislashRadiusInterp = frame.particle->spriteIndex;
    ScaleMatrix(&frame.rotMatrix, &g_YasuiLimitOmnislashScaleVector2);
    frame.rotMatrix.t[0] = frame.particle->posX;
    frame.rotMatrix.t[1] = frame.particle->posY;
    frame.rotMatrix.t[2] = frame.particle->posZ;
    SetRotMatrix(&frame.rotMatrix);
    SetTransMatrix(&frame.rotMatrix);
    SetDrawColorRGB(0, 0, 0);
    g_YasuiLimitOmnislashPacketCursor = BattleFx_SubmitModelDraw(&g_YasuiLimitOmnislashDrawReqPool1.modelRes5, g_BattleCurrentOrderingTable + BATTLE_OT_AFTER_SORTED_0, 1, g_YasuiLimitOmnislashPacketCursor);
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.particle->posY += frame.particle->velocityY;
    frame.particle->posZ += frame.particle->velocityZ;
    (frame.particle->frame)++;
    if (frame.particle->frame == 0xC) {
        frame.particle->ownerSlot = BATTLE_TASK_FREE;
    }
}

/* Allocates first available free record slot from particle array. */
// FUNCTION: FF7 0x0046e780
short __cdecl BattleFx_LimitOmnislashClaimParticleRecord(void)
{
    BattleFxOmnislashParticle *recordAddr;
    short slotIdx;

    recordAddr = g_YasuiLimitOmnislashParticleTable;
    for (slotIdx = 0; slotIdx < 0xC8; slotIdx++) {
        if (recordAddr->ownerSlot == 0) {
            recordAddr->ownerSlot = 1;
            return slotIdx;
        }
        recordAddr++;
    }
    slotIdx = 0;
    Battle_PlaySfxPannedToActor(6, -1, 0x40);
    return slotIdx;
}
} /* extern "C" */
