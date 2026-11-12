// src/ff7/src/battle/battle_fx_limit_earth_rave_effect.cpp
// Limit Break [40] (Red XIII Lv 3-1): Earth Rave (Japanese: アースレイヴ, dev tag: "limit3/e_r" / ER)
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: ER limit break effect: a screen flash, an impact burst, sprite particle pools,
//   directional sprites, and spinning and tumbling models. Two helpers compute a stand point from
//   the target's facing or bearing.
/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/enums/sound.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_limit.h"
#include "ff7/battle_tasks.h"
#include "lib/types/graphics.h"
#include "lib/codegen.h"
#include "ff7/api/battle.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "lib/types/polygon.h"
#include "ff7/camera_script.h"
#include <stdlib.h>
#include <string.h>
#include "lib/api/sound.h"
extern "C" {

// clang-format off
// clang-format on

#pragma intrinsic(memset)

#pragma intrinsic(memcpy)
#pragma function(memcpy)

/* The file names the resource table points at: the textures and sprites, then the first rock
 * model, whose terminator is the first byte of g_BattleLimitEarthRaveResPaths1. */
typedef struct {
    char kemu1Tim[12];
    char kemu1Spt[12];
    char magmaTim[12];
    char magumaSpt[12];
    char isi1Rsd[8];
} BattleLimitERResPaths0;

/* The end of the name before it, then three rock model names. */
typedef struct {
    char prevNameEnd[4];
    char isi2Rsd[12];
    char isi3Rsd[12];
    char isi4Rsd[12];
} BattleLimitERRockPaths;

/* Defined later in this TU after the descriptor tables that reference them (retail data order). */
extern BattleLimitERResPaths0 g_BattleLimitEarthRaveResPaths0;
extern BattleLimitERRockPaths g_BattleLimitEarthRaveResPaths1; /* Defined later in this TU (retail data order). */
extern BattleLimitERRockPaths g_BattleLimitEarthRaveResPaths3; /* Defined later in this TU (retail data order). */

// clang-format off
// GLOBAL: FF7 0x0082c8e8
int g_BattleLimitEarthRaveResourceDef = 0x00192A39;
int g_BattleLimitEarthRaveResourceDefPath0[1] = {
    (int)g_BattleLimitEarthRaveResPaths0.kemu1Tim
};
int g_BattleLimitEarthRaveResourceDefEntries[54] = {
    (int)g_BattleLimitEarthRaveResPaths0.kemu1Spt, 0x0, 0x0, 0x192a39, (int)g_BattleLimitEarthRaveResPaths0.magmaTim, (int)g_BattleLimitEarthRaveResPaths0.magumaSpt,
    0x0, 0x0, 0x6400b4, (int)g_BattleLimitEarthRaveResPaths0.isi1Rsd, 0x0, 0x0,
    0x0, 0x6400b4, (int)g_BattleLimitEarthRaveResPaths1.isi2Rsd, 0x0, 0x0, 0x0,
    0x6400b4, (int)g_BattleLimitEarthRaveResPaths1.isi3Rsd, 0x0, 0x0, 0x0, 0x6400b4,
    (int)g_BattleLimitEarthRaveResPaths1.isi4Rsd, 0x0, 0x0, 0x0, 0x6400b1, (int)g_BattleLimitEarthRaveResPaths2,
    0x0, 0x0, 0x0, 0x6400b1, (int)g_BattleLimitEarthRaveResPaths3.isi2Rsd, 0x0,
    0x0, 0x0, 0x6400b1, (int)g_BattleLimitEarthRaveResPaths3.isi3Rsd, 0x0, 0x0,
    0x0, 0x6400b1, (int)g_BattleLimitEarthRaveResPaths3.isi4Rsd, 0x0, 0x0, 0x0,
    0x6400b1, (int)g_BattleLimitEarthRaveResPaths4, 0x0, 0x0, 0x0, 0x0
};
// GLOBAL: FF7 0x0082c9c8
int g_BattleLimitEarthRaveUpVector[3] = {0, -4096, 0};
unsigned char g_rawData_0082c9d8[16] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00};
/* The tumbling rocks' four models in the resource table, by the task's modelIndex. */
// GLOBAL: FF7 0x0082c9e8
int g_BattleLimitEarthRaveModelReq1[4] = {2, 3, 4, 5};
/* The fading variants of the tumbling rocks' models, then the first two words of the sprite pool's
 * BattleFxSpriteRequest; its colour follows in g_BattleLimitEarthRaveColorData, and its frame and UVs are
 * the low half of g_BattleLimitEarthRaveSpriteReq.spriteGroup. */
typedef struct {
    int fadeModels[4];
    int particleSpriteGroup;
    int particleSpriteDrawObj;
} BattleLimitERModelReq2;

// GLOBAL: FF7 0x0082c9f8
BattleLimitERModelReq2 g_BattleLimitEarthRaveModelReq2 = {{6, 7, 8, 9}, 0, 0};
/* The sprite pool's vertex colour and primitive code. */
typedef struct {
    char r;
    char g;
    char b;
    unsigned char code;
} BattleLimitERColorData;

// GLOBAL: FF7 0x0082ca10
BattleLimitERColorData g_BattleLimitEarthRaveColorData = {-128, -128, -128, 0x2e};
// GLOBAL: FF7 0x0082ca18
BattleFxSpriteRequest g_BattleLimitEarthRaveSpriteReq = {0, 0, 86, 100, 80, 46, 0, 0};
// GLOBAL: FF7 0x0082ca28
int g_BattleLimitEarthRaveTimingTable1[6] = {(-1), 40, 58, 74, 93, 0};
// GLOBAL: FF7 0x0082ca40
int g_BattleLimitEarthRaveTimingTable2[6] = {36, 50, 66, 89, 109, 0};
// GLOBAL: FF7 0x0082ca58
int g_BattleLimitEarthRaveTimingTable3[10] = {24, 40, 58, 64, 74, 93, 108, 114, 122, (-1)};
/* The frames the timeline plays a sound on, and the sound of each. */
typedef struct {
    int frames[8];
    int soundIds[8];
} BattleLimitERSoundTiming;

// GLOBAL: FF7 0x0082ca80
BattleLimitERSoundTiming g_BattleLimitEarthRaveTimingTable4 = {{31, 36, 50, 66, 80, 89, 105, 109}, {344, 109, 109, 141, 399, 152, 399, 152}};
BattleLimitERResPaths0 g_BattleLimitEarthRaveResPaths0 = {"KEMU1.TIM", "KEMU1.SPT", "MAGMA.TIM", "MAGUMA.SPT", {'I', 'S', 'I', '1', '.', 'R', 'S', 'D'}};
BattleLimitERRockPaths g_BattleLimitEarthRaveResPaths1 = {"", "ISI2.RSD", "ISI3.RSD", "ISI4.RSD"};
unsigned char g_BattleLimitEarthRaveResPaths2[8] = {'I', 'S', 'I', '1', '.', 'R', 'S', 'D'};
BattleLimitERRockPaths g_BattleLimitEarthRaveResPaths3 = {"", "ISI2.RSD", "ISI3.RSD", "ISI4.RSD"};
unsigned char g_BattleLimitEarthRaveResPaths4[12] = "YOUGAN1.RSD";
/* An actor position: x, y, z and a pad word, copied one word or all eight bytes at a time. */
typedef union {
    Int64Pair pair;
    int words[2];
    struct {
        short x;
        short y;
        short z;
        short pad;
    };
} BattleLimitERPos;

/* The actor's position and rotation before the limit moves it. */
typedef struct {
    BattleLimitERPos pos;
    Int64Pair rot;
} BattleLimitERSavedPose;

// GLOBAL: FF7 0x00b03a40
BattleLimitERSavedPose g_BattleLimitEarthRaveSavedActorPose = {0};
// GLOBAL: FF7 0x00b03a50
BattleLimitEarthRaveParticle g_BattleLimitEarthRaveParticlePool[5][100] = {0};
// GLOBAL: FF7 0x00b06930
int g_BattleLimitEarthRaveTargetMask = 0;
unsigned char g_bssPad_00b06934[4] = {0};
// GLOBAL: FF7 0x00b06938
short g_BattleLimitEarthRaveStepX = 0;
// GLOBAL: FF7 0x00b0693c
short g_BattleLimitEarthRaveStepZ = 0;
// GLOBAL: FF7 0x00b06940
int g_BattleLimitEarthRaveActorIndex = 0;
unsigned char g_bssPad_00b06944[4] = {0};
/* The point the actor stands at to strike a target, and the yaw that faces it. */
typedef struct {
    short x;
    short y;
    short z;
    short yaw;
} BattleLimitERStandPoint;

// GLOBAL: FF7 0x00b06948
BattleLimitERStandPoint g_BattleLimitEarthRaveStandPos = {0};
// GLOBAL: FF7 0x00b06950
BattleLimitERPos g_BattleLimitEarthRaveCurrentPos = {0};
// GLOBAL: FF7 0x00b06958
struct GfxTableCursor *g_BattleLimitEarthRaveModelTable = 0;
unsigned char g_bssPad_00b0695c[4] = {0};
// clang-format on

/*
 * Loads the ER resource package (limit3/e_r) into g_BattleLimitEarthRaveModelTable unless it is already
 * loaded.
 */
// FUNCTION: FF7 0x004c6860
void __cdecl BattleFx_LimitLoadResEarthRave(void)
{
    GfxModelTableBank *drawDev;
    GfxResourceRequest requestSlot;
    int kindId;
    char loadPath[200];
    GfxResourceRequest *pRequest;
    int stepResourceDef;

    if (g_pBattle3dScene != 0 && g_BattleLimitEarthRaveModelTable == 0) {
        kindId = 0xB;
        stepResourceDef = (int)&g_BattleLimitEarthRaveResourceDef;
        drawDev = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRequest = &requestSlot;
        Gfx_InitResourceRequest(pRequest);
        pRequest->groupTag = 0;
        // STRING: FF7 0x0082cb5c
        pRequest->resourcePath = BattleFx_BuildResourcePath("limit3/e_r", loadPath);
        Gfx_LoadModelTableIfAbsent(kindId, stepResourceDef, pRequest, &g_BattleLimitEarthRaveModelTable, drawDev);
    }
}

/*
 * Entry point of the ER limit: loads its resources and starts the effect.
 */
// FUNCTION: FF7 0x004c68f0
int __cdecl BattleFx_LimitEnterEarthRave(int targetMask, int actorIdx)
{
    BattleFx_LimitLoadResEarthRave();
    BattleFx_LimitBeginEarthRave(targetMask, actorIdx);
    return 0;
}

/*
 * Starts the draw-list buffer task and the timeline spawner, and keeps the target mask and the
 * acting actor for the tasks.
 */
// FUNCTION: FF7 0x004c690c
void __cdecl BattleFx_LimitBeginEarthRave(int targetMask, int actorIdx)
{
    g_BattleFxDrawListBufferTaskSlotPtr = (unsigned char *)BATTLE_NEW_TASK_SLOT(BattleFx_SelectDrawListBuffer);
    Battle_AllocateResourceSlot(BattleFx_LimitEarthRaveSpawnTimeline);
    g_BattleLimitEarthRaveTargetMask = targetMask;
    g_BattleLimitEarthRaveActorIndex = actorIdx;
}

// Retires itself and starts the timeline task with its timing-table cursors at zero.
// FUNCTION: FF7 0x004c694d
void __cdecl BattleFx_LimitEarthRaveSpawnTimeline(void)
{
    int p;
    BattleFxLimitEarthRaveRecord *q;

    q = (BattleFxLimitEarthRaveRecord *)BATTLE_CURRENT_TASK_SLOT;
    q->ownerSlot = BATTLE_TASK_FREE;
    q = (BattleFxLimitEarthRaveRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitEarthRaveTimelineTask);
    q->timingIndexA = 0;
    q->timingIndexB = 0;
    q->standIndex = 0;
}

/*
 * The ER timeline. Fades a white screen flash in and out (also while paused). It saves the actor's
 * pose, walks the actor to each of up to five targets in turn (frames from g_BattleLimitEarthRaveTimingTable1)
 * and back, spawns an impact burst at each strike (g_BattleLimitEarthRaveTimingTable2), scale ramps
 * (g_BattleLimitEarthRaveTimingTable3) and the sounds of g_BattleLimitEarthRaveTimingTable4, and restores the
 * actor's position and retires at frame 135.
 */
// FUNCTION: FF7 0x004c69a8
void __cdecl BattleFx_LimitEarthRaveTimelineTask(void)
{
    char alpha;
    BattleFxEarthRaveRecord *pRingRec;
    BattleFxLimitEarthRaveBurst *pBurst;
    VECTOR stepX;
    BattleFxLimitEarthRaveRecord *task;

    task = (BattleFxLimitEarthRaveRecord *)BATTLE_CURRENT_TASK_SLOT;
    if (task->frame < 1) {
        alpha = 0;
    } else if (task->frame < 17) {
        alpha = task->frame * 3;
    } else if (task->frame >= 119) {
        alpha = 0x30 - (task->frame - 0x77) * 3;
    } else {
        alpha = 0x30;
    }
    g_BattleOverlayDrawListHead = BattleFx_FlashScreenWhite((char)alpha, 0, 0);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (task->frame == 0) {
        g_BattleLimitEarthRaveSavedActorPose.pos.pair = *(Int64Pair *)&g_BattleActorRecords[g_BattleLimitEarthRaveActorIndex].posX;
        g_BattleLimitEarthRaveCurrentPos.words[0] = g_BattleLimitEarthRaveSavedActorPose.pos.words[0];
        g_BattleLimitEarthRaveCurrentPos.words[1] = g_BattleLimitEarthRaveSavedActorPose.pos.words[1];
        g_BattleLimitEarthRaveSavedActorPose.rot = *(Int64Pair *)&g_BattleActorRecords[g_BattleLimitEarthRaveActorIndex].rotX;
        task->standPoint = BattleFx_LimitEarthRaveComputeStandPointByBearing(task->standIndex);
        g_BattleActorRecords[g_BattleLimitEarthRaveActorIndex].yaw = g_BattleLimitEarthRaveStandPos.yaw;
    } else if (task->frame >= 15 && task->frame < 21) {
        if (task->frame == 15) {
            stepX.vx = g_BattleLimitEarthRaveStandPos.x - g_BattleLimitEarthRaveCurrentPos.x;
            stepX.vy = 0;
            stepX.vz = g_BattleLimitEarthRaveStandPos.z - g_BattleLimitEarthRaveCurrentPos.z;
            NormalizeVectorFixed(&stepX, &stepX);
            g_BattleLimitEarthRaveStepX = stepX.vx * 0x32 >> 12;
            g_BattleLimitEarthRaveStepZ = stepX.vz * 0x32 >> 12;
        }
        g_BattleLimitEarthRaveCurrentPos.x += g_BattleLimitEarthRaveStepX;
        g_BattleLimitEarthRaveCurrentPos.z += g_BattleLimitEarthRaveStepZ;
    } else if (task->frame >= 21 && task->frame < 24) {
        if (task->frame == 21) {
            g_BattleLimitEarthRaveStepX = (g_BattleLimitEarthRaveStandPos.x - g_BattleLimitEarthRaveCurrentPos.x) / 3;
            g_BattleLimitEarthRaveStepZ = (g_BattleLimitEarthRaveStandPos.z - g_BattleLimitEarthRaveCurrentPos.z) / 3;
        }
        g_BattleLimitEarthRaveCurrentPos.x += g_BattleLimitEarthRaveStepX;
        g_BattleLimitEarthRaveCurrentPos.z += g_BattleLimitEarthRaveStepZ;
    } else if (task->frame >= 114 && task->frame < 122) {
        if (task->frame == 114) {
            g_BattleLimitEarthRaveStepX = (g_BattleLimitEarthRaveSavedActorPose.pos.x - g_BattleLimitEarthRaveCurrentPos.x) / 8;
            g_BattleLimitEarthRaveStepZ = (g_BattleLimitEarthRaveSavedActorPose.pos.z - g_BattleLimitEarthRaveCurrentPos.z) / 8;
        }
        g_BattleLimitEarthRaveCurrentPos.x += g_BattleLimitEarthRaveStepX;
        g_BattleLimitEarthRaveCurrentPos.z += g_BattleLimitEarthRaveStepZ;
    } else if (task->frame >= 122 && task->frame < 126) {
        g_BattleLimitEarthRaveCurrentPos.words[0] = g_BattleLimitEarthRaveSavedActorPose.pos.words[0];
        g_BattleLimitEarthRaveCurrentPos.words[1] = g_BattleLimitEarthRaveSavedActorPose.pos.words[1];
    }
    if (task->frame == g_BattleLimitEarthRaveTimingTable1[task->standIndex] && task->standIndex < 5) {
        task->standPoint = BattleFx_LimitEarthRaveComputeStandPointByFacing(task->standIndex);
        g_BattleLimitEarthRaveCurrentPos.x = g_BattleLimitEarthRaveStandPos.x;
        g_BattleLimitEarthRaveCurrentPos.z = g_BattleLimitEarthRaveStandPos.z;
        g_BattleActorRecords[g_BattleLimitEarthRaveActorIndex].yaw = g_BattleLimitEarthRaveStandPos.yaw;
    }
    if (task->frame == g_BattleLimitEarthRaveTimingTable2[task->standIndex] && task->standIndex < 5) {
        pRingRec = (BattleFxEarthRaveRecord *)Battle_AllocSceneMessage(0xA);
        pRingRec->standPoint = task->standPoint;
        pRingRec->standIndex = task->standIndex;
        pBurst = (BattleFxLimitEarthRaveBurst *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitEarthRaveImpactBurstTask);
        Battle_GetBoneWorldPos(g_BattleLimitEarthRaveActorIndex, g_BattleActorRecords[g_BattleLimitEarthRaveActorIndex].altBone, &pBurst->posX);
        pBurst->stage = task->standIndex;
        pBurst->linkedIndex = g_BattleHitReactionRecords[task->standIndex].targetParam;
        (task->standIndex)++;
    }
    if (task->frame == g_BattleLimitEarthRaveTimingTable3[task->timingIndexA]) {
        Battle_SpawnActorScaleRampTask(g_BattleLimitEarthRaveActorIndex);
        (task->timingIndexA)++;
    }
    if (task->frame == g_BattleLimitEarthRaveTimingTable4.frames[task->timingIndexB]) {
        Battle_GetBoneWorldPos(g_BattleLimitEarthRaveActorIndex, g_BattleActorRecords[g_BattleLimitEarthRaveActorIndex].altBone, &task->posX);
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputeScreenPanFromPosition(&task->posX), g_BattleLimitEarthRaveTimingTable4.soundIds[task->timingIndexB]);
        (task->timingIndexB)++;
    }
    *(Int64Pair *)&g_BattleActorRecords[g_BattleLimitEarthRaveActorIndex].posX = g_BattleLimitEarthRaveCurrentPos.pair;
    if (task->frame >= 135) {
        *(Int64Pair *)&g_BattleActorRecords[g_BattleLimitEarthRaveActorIndex].posX = g_BattleLimitEarthRaveSavedActorPose.pos.pair;
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    (task->frame)++;
}

/*
 * The burst of one strike (stage 0 to 4). In its first phase it starts the sprite-pool emitter
 * (with a hit value of -1 for stage 0); it retires at once if the strike's hit value is negative.
 * From stage 2 it emits twelve directional sprites in its second phase; from stage 3 it also
 * spawns the spinning shrinking model and stage + 8 tumbling rocks in its first phase and shakes
 * the camera for three phases. It retires in its fourth phase.
 */
// FUNCTION: FF7 0x004c6f42
void __cdecl BattleFx_LimitEarthRaveImpactBurstTask(void)
{
    int idx;
    int baseSize;
    BattleFxLimitEarthRaveSprite *pDirSprite;
    BattleFxLimitEarthRaveBurst *curTask;
    BattleFxEarthRaveImpactBurst *childTask;
    BattleFxTumblingSlot *pTumbler;
    int power;
    int randFactor;

    curTask = (BattleFxLimitEarthRaveBurst *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (curTask->phase == 0) {
        childTask = (BattleFxEarthRaveImpactBurst *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitEarthRaveSpriteParticlePool100Task);
        *(Int64Pair *)&childTask->posX = *(Int64Pair *)&curTask->posX;
        childTask->stage = curTask->stage;
        if (curTask->stage == 0) {
            childTask->linkedIndex = 0xFFFF;
        } else {
            childTask->linkedIndex = curTask->linkedIndex;
        }
    }
    if (curTask->linkedIndex < 0) {
        curTask->ownerSlot = BATTLE_TASK_FREE;
    } else {
        if (curTask->phase == 1 && curTask->stage >= 2) {
            for (idx = 0; idx < 12; idx++) {
                pDirSprite = (BattleFxLimitEarthRaveSprite *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitEarthRaveDirectionalSpriteTask);
                *(Int64Pair *)&pDirSprite->posX = *(Int64Pair *)&curTask->posX;
                pDirSprite->speed = rand() % 280 + 68;
                pDirSprite->direction.v[0] = rand() % 4096 - 2048;
                pDirSprite->direction.v[1] = rand() % 4096 - 2048;
                pDirSprite->direction.v[2] = rand() % 4096 - 2048;
                NormalizeVectorFixed(pDirSprite->direction.v, pDirSprite->direction.v);
                baseSize = 0x2580;
                randFactor = rand() % 1536 + 2560;
                pDirSprite->sizeXZ = baseSize * randFactor >> 12;
                pDirSprite->scale = pDirSprite->sizeXZ;
            }
        }
        if (curTask->phase == 0 && curTask->stage >= 3) {
            childTask = (BattleFxEarthRaveImpactBurst *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitEarthRaveSpinningShrinkingModelTask);
            *(Int64Pair *)&childTask->posX = *(Int64Pair *)&curTask->posX;
            childTask->posY = 0;
            childTask->angle = rand() % 0x1000;
            childTask->angularVelocity = rand() % 220 + 240;
            childTask->scale = ((curTask->stage - 2) * 0xC8 + 0x514 << 12) / 1000;
            childTask->scaleStep = childTask->scale / 7;
        }
        if (curTask->phase == 0 && curTask->stage >= 3) {
            power = (curTask->stage << 4) + 0xC8;
            for (idx = 0; idx < curTask->stage + 8; idx++) {
                pTumbler = (BattleFxTumblingSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitEarthRaveTumblingModelTask);
                pTumbler->posX = curTask->posX + rand() % 1800 + -900;
                pTumbler->posY = -(rand() % 780 + (curTask->stage - 2) * 30 + 70);
                pTumbler->posZ = curTask->posZ + rand() % 1800 + -900;
                pTumbler->velocityY = -(rand() % 98 + (curTask->stage - 2) * 10 + 16);
                pTumbler->angleZ = rand() % 0x1000;
                pTumbler->angleX = pTumbler->angleZ;
                pTumbler->angleXStep = rand() % 72 + 96;
                pTumbler->angleZStep = rand() % 128 + 74;
                baseSize = (power << 12) / 600;
                randFactor = rand() % 2560 + 1280;
                pTumbler->scale = baseSize * randFactor >> 12;
                pTumbler->modelIndex = rand() % 4;
            }
        }
        if (curTask->phase >= 0 && curTask->phase <= 2 && curTask->stage >= 3) {
            g_BattleCameraTransform.posY = (curTask->phase + 1 & 1) * 140;
        }
        if (curTask->phase >= 3) {
            g_BattleCameraTransform.posY = 0;
            curTask->ownerSlot = BATTLE_TASK_FREE;
        }
        (curTask->phase)++;
    }
}

/*
 * Draws and moves the strike's pool of 100 sprite particles, which darken with age and die when
 * they reach the ground or age 15. In its first two frames it emits up to twelve particles a frame
 * around the strike point, faster ones when the hit value is positive. Retires at frame 20.
 */
// FUNCTION: FF7 0x004c740b
void __cdecl BattleFx_LimitEarthRaveSpriteParticlePool100Task(void)
{
    int idx;
    int p;
    BattleLimitEarthRaveParticle *pParticlePool;
    int s;
    int emptyIdx;
    int baseSize;
    int rnd;
    int speedValue;
    BattleFxLimitEarthRaveSpriteParticle *emitter;

    emitter = (BattleFxLimitEarthRaveSpriteParticle *)BATTLE_CURRENT_TASK_SLOT;
    pParticlePool = (BattleLimitEarthRaveParticle *)(emitter->stage * 0x960 + (char *)&g_BattleLimitEarthRaveParticlePool);
    g_BattleLimitEarthRaveModelReq2.particleSpriteGroup = Gfx_ModelTableGetLoadedRecord(1, g_BattleLimitEarthRaveModelTable);
    g_BattleLimitEarthRaveModelReq2.particleSpriteDrawObj = Gfx_ModelTableGetSpriteObject(1, g_BattleLimitEarthRaveModelTable);
    for (idx = 0; idx < 100; idx++) {
        if (pParticlePool[idx].active != 0) {
            g_BattleLimitEarthRaveColorData.b = 0x80 - (pParticlePool[idx].age << 3);
            g_BattleLimitEarthRaveColorData.g = g_BattleLimitEarthRaveColorData.b;
            g_BattleLimitEarthRaveColorData.r = g_BattleLimitEarthRaveColorData.g;
            BattleFx_InstallBillboardMatrix(&pParticlePool[idx].posX, (short)pParticlePool[idx].size, -(pParticlePool[idx].size >> 3));
            g_BattleFxActiveDrawListPtr = BattleFx_SubmitSpriteDraw(&g_BattleLimitEarthRaveModelReq2.particleSpriteGroup, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
            if (g_BattleIsPaused == 0) {
                pParticlePool[idx].posX += pParticlePool[idx].velocityX;
                pParticlePool[idx].posY += pParticlePool[idx].velocityY;
                pParticlePool[idx].posZ += pParticlePool[idx].velocityZ;
                pParticlePool[idx].velocityY += pParticlePool[idx].accelY;
                pParticlePool[idx].accelY += (short)(pParticlePool[idx].accelY >> 4);
                pParticlePool[idx].size -= 64;
                if (pParticlePool[idx].posY > 0 || pParticlePool[idx].age >= 15) {
                    pParticlePool[idx].active = 0;
                }
                (pParticlePool[idx].age)++;
            }
        }
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (emitter->frame >= 20) {
        emitter->ownerSlot = BATTLE_TASK_FREE;
    }
    if (emitter->frame < 2) {
        baseSize = 0x1800;
        for (idx = 0; idx < 12; idx++) {
            for (emptyIdx = 0; emptyIdx < 100 && pParticlePool[emptyIdx].active != 0; emptyIdx++) {
            }
            if (emptyIdx >= 100) {
                break;
            }
            pParticlePool[emptyIdx].active = 1;
            pParticlePool[emptyIdx].age = 0;
            pParticlePool[emptyIdx].posX = emitter->posX + rand() % 200 + -100;
            pParticlePool[emptyIdx].posY = emitter->posY + rand() % 200 + -100;
            pParticlePool[emptyIdx].posZ = emitter->posZ + rand() % 200 + -100;
            if (emitter->linkedIndex > 0) {
                rnd = ((rand()) % 4096);
                speedValue = rand() % 190 + 140;
                pParticlePool[emptyIdx].velocityX = -(SinFixed(rnd) * speedValue >> 12);
                pParticlePool[emptyIdx].velocityY = -(rand() % 270 + 120);
                pParticlePool[emptyIdx].velocityZ = CosFixed(rnd) * speedValue >> 12;
                pParticlePool[emptyIdx].accelY = rand() % 20 + 16;
                rnd = rand() % 2304 + 1536 - (emitter->frame << 9);
                pParticlePool[emptyIdx].size = baseSize * rnd >> 12;
            } else {
                rnd = ((rand()) % 4096);
                speedValue = rand() % 90 + 70;
                pParticlePool[emptyIdx].velocityX = -(SinFixed(rnd) * speedValue >> 12);
                pParticlePool[emptyIdx].velocityY = -(rand() % 140 + 60);
                pParticlePool[emptyIdx].velocityZ = CosFixed(rnd) * speedValue >> 12;
                pParticlePool[emptyIdx].accelY = rand() % 10 + 8;
                rnd = rand() % 1792 + 1024 - emitter->frame * 384;
                pParticlePool[emptyIdx].size = baseSize * rnd >> 12;
            }
        }
    }
    (emitter->frame)++;
}

/*
 * Draws a sprite stretched along its direction of flight and moves it along that direction,
 * slowing down and growing longer. Retires at frame 15.
 */
// FUNCTION: FF7 0x004c79a7
void __cdecl BattleFx_LimitEarthRaveDirectionalSpriteTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[92];
        BattleFxLimitEarthRaveSprite *spriteSlot;
        struct { unsigned char pad_to_4[0x4]; unsigned char spriteMatrix[30]; };
        struct { unsigned char pad_to_16[0x16]; int posX; };
        struct { unsigned char pad_to_1A[0x1A]; int posY; };
        struct { unsigned char pad_to_1E[0x1E]; int posZ; };
        struct { unsigned char pad_to_24[0x24]; BattleErVec16 direction; };
        struct { unsigned char pad_to_34[0x34]; int angle; };
        struct { unsigned char pad_to_38[0x38]; unsigned char axis[16]; };
        struct { unsigned char pad_to_48[0x48]; VECTOR scaleVec; };
    } frame;
#pragma pack(pop)
    frame.spriteSlot = (BattleFxLimitEarthRaveSprite *)BATTLE_CURRENT_TASK_SLOT;
    frame.direction = frame.spriteSlot->direction;
    frame.angle = ComputeAngleBetweenVectors((VECTOR *)&g_BattleLimitEarthRaveUpVector, (VECTOR *)&frame.direction, (VECTOR *)&frame.axis);
    Gte_BuildAxisAngleMatrix(frame.angle, (PsxMatrix *)&frame.spriteMatrix, (VECTOR *)&frame.axis);
    frame.posX = frame.spriteSlot->posX;
    frame.posY = frame.spriteSlot->posY;
    frame.posZ = frame.spriteSlot->posZ;
    frame.scaleVec.vz = frame.spriteSlot->sizeXZ;
    frame.scaleVec.vx = frame.scaleVec.vz;
    frame.scaleVec.vy = frame.spriteSlot->scale;
    ScaleMatrix(&frame.spriteMatrix, &frame.scaleVec);
    MultiplyMatrix(g_BattleCameraTransform.matrix, &frame.spriteMatrix, &frame.spriteMatrix);
    SetRotMatrix(&frame.spriteMatrix);
    SetTransMatrix(&frame.spriteMatrix);
    g_BattleLimitEarthRaveSpriteReq.frame = frame.spriteSlot->frame;
    g_BattleLimitEarthRaveSpriteReq.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(0, g_BattleLimitEarthRaveModelTable);
    g_BattleLimitEarthRaveSpriteReq.drawObj = Gfx_ModelTableGetSpriteObject(0, g_BattleLimitEarthRaveModelTable);
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitSpriteDraw(&g_BattleLimitEarthRaveSpriteReq, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 12, g_BattleFxActiveDrawListPtr);
    if (frame.spriteSlot->frame >= 15)
        frame.spriteSlot->ownerSlot = BATTLE_TASK_FREE;
    if (g_BattleIsPaused != 0)
        return;
    frame.spriteSlot->posX += frame.spriteSlot->speed * frame.direction.v[0] >> 12;
    frame.spriteSlot->posY += frame.spriteSlot->speed * frame.direction.v[1] >> 12;
    frame.spriteSlot->posZ += frame.spriteSlot->speed * frame.direction.v[2] >> 12;
    frame.spriteSlot->speed -= frame.spriteSlot->speed >> 4;
    frame.spriteSlot->scale += frame.spriteSlot->scale >> 4;
    (frame.spriteSlot->frame)++;
}

/*
 * Draws model 10 of the resource table spinning about Y and shrinking at the strike point.
 * Retires at frame 8 or when the scale falls below zero.
 */
// FUNCTION: FF7 0x004c7ba8
void __cdecl BattleFx_LimitEarthRaveSpinningShrinkingModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[64];
        PsxMatrix rotMatrix;
        struct { unsigned char pad_to_20[0x20]; BattleFxLimitEarthRaveSpinShrink *shrinkSlot; };
        struct { unsigned char pad_to_24[0x24]; SVECTOR rotX; };
        struct { unsigned char pad_to_2C[0x2C]; BattleFxModelDraw *drawDesc; };
        struct { unsigned char pad_to_30[0x30]; VECTOR scaleVec; };
    } frame;
#pragma pack(pop)

    frame.shrinkSlot = (BattleFxLimitEarthRaveSpinShrink *)BATTLE_CURRENT_TASK_SLOT;
    frame.rotX.vx = 0;
    frame.rotX.vy = frame.shrinkSlot->angle;
    frame.rotX.vz = 0;
    RotMatrixXYZ(&frame.rotX, &frame.rotMatrix);
    frame.rotMatrix.t[0] = frame.shrinkSlot->posX;
    frame.rotMatrix.t[1] = frame.shrinkSlot->posY;
    frame.rotMatrix.t[2] = frame.shrinkSlot->posZ;
    frame.scaleVec.vz = frame.shrinkSlot->scale;
    frame.scaleVec.vy = frame.scaleVec.vz;
    frame.scaleVec.vx = frame.scaleVec.vy;
    ScaleMatrix(&frame.rotMatrix, &frame.scaleVec);
    MultiplyMatrix(g_BattleCameraTransform.matrix, &frame.rotMatrix, &frame.rotMatrix);
    SetRotMatrix(&frame.rotMatrix);
    SetTransMatrix(&frame.rotMatrix);
    frame.drawDesc = (BattleFxModelDraw *)Gfx_GetPsxScratchpadPtr(0);
    frame.drawDesc->modelGroup = Gfx_ModelTableGetRenderObject(10, g_BattleLimitEarthRaveModelTable);
    frame.drawDesc->flags = 0x28;
    frame.drawDesc->uvScroll = 0;
    frame.drawDesc->fadeLevel = 128;
    frame.drawDesc->stateFlags = 0x20;
    frame.drawDesc->texturePageBits = 0;
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitModelDraw(frame.drawDesc, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (frame.shrinkSlot->frameCount >= 8) {
        frame.shrinkSlot->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.shrinkSlot->angle += frame.shrinkSlot->angularVelocity;
    frame.shrinkSlot->angularVelocity -= (short)(frame.shrinkSlot->angularVelocity >> 5);
    frame.shrinkSlot->scale -= frame.shrinkSlot->scaleStep;
    frame.shrinkSlot->scaleStep -= (short)(frame.shrinkSlot->scaleStep >> 4);
    if (frame.shrinkSlot->scale < 0) {
        frame.shrinkSlot->ownerSlot = BATTLE_TASK_FREE;
    }
    (frame.shrinkSlot->frameCount)++;
}

/*
 * Draws one of the four rock models tumbling about X and Z as it falls; from frame 10 it draws the
 * model's fading variant and fades it out. Retires at frame 16.
 */
// FUNCTION: FF7 0x004c7d7a
void __cdecl BattleFx_LimitEarthRaveTumblingModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[64];
        PsxMatrix modelMatrix;
        struct { unsigned char pad_to_20[0x20]; BattleFxTumblingSlot *tumblingSlot; };
        struct { unsigned char pad_to_24[0x24]; SVECTOR rotX; };
        struct { unsigned char pad_to_2C[0x2C]; BattleFxModelDraw *modelDraw; };
        struct { unsigned char pad_to_30[0x30]; VECTOR scale; };
    } frame;
#pragma pack(pop)

    frame.tumblingSlot = (BattleFxTumblingSlot *)BATTLE_CURRENT_TASK_SLOT;
    frame.rotX.vx = frame.tumblingSlot->angleX;
    frame.rotX.vy = 0;
    frame.rotX.vz = frame.tumblingSlot->angleZ;
    RotMatrixXYZ(&frame.rotX, &frame.modelMatrix);
    frame.modelMatrix.t[0] = frame.tumblingSlot->posX;
    frame.modelMatrix.t[1] = frame.tumblingSlot->posY;
    frame.modelMatrix.t[2] = frame.tumblingSlot->posZ;
    frame.scale.vz = frame.tumblingSlot->scale;
    frame.scale.vy = frame.scale.vz;
    frame.scale.vx = frame.scale.vy;
    ScaleMatrix(&frame.modelMatrix, &frame.scale);
    MultiplyMatrix(g_BattleCameraTransform.matrix, &frame.modelMatrix, &frame.modelMatrix);
    SetRotMatrix(&frame.modelMatrix);
    SetTransMatrix(&frame.modelMatrix);
    frame.modelDraw = (BattleFxModelDraw *)Gfx_GetPsxScratchpadPtr(0);
    frame.modelDraw->modelGroup = Gfx_ModelTableGetRenderObject(g_BattleLimitEarthRaveModelReq1[frame.tumblingSlot->modelIndex], g_BattleLimitEarthRaveModelTable);
    frame.modelDraw->flags = 0;
    frame.modelDraw->uvScroll = 0;
    frame.modelDraw->fadeLevel = 128;
    frame.modelDraw->stateFlags = 0x20;
    frame.modelDraw->texturePageBits = 0;
    if (frame.tumblingSlot->timer >= 10) {
        frame.modelDraw->flags |= 8;
        frame.modelDraw->fadeLevel = 128 - (frame.tumblingSlot->timer - 10) * 21;
        frame.modelDraw->modelGroup = Gfx_ModelTableGetRenderObject(g_BattleLimitEarthRaveModelReq2.fadeModels[frame.tumblingSlot->modelIndex], g_BattleLimitEarthRaveModelTable);
    }
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitModelDraw(frame.modelDraw, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (frame.tumblingSlot->timer >= 16) {
        frame.tumblingSlot->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.tumblingSlot->posY += frame.tumblingSlot->velocityY;
    frame.tumblingSlot->velocityY += (short)(frame.tumblingSlot->velocityY / 7);
    frame.tumblingSlot->angleX += frame.tumblingSlot->angleXStep;
    frame.tumblingSlot->angleZ += frame.tumblingSlot->angleZStep;
    (frame.tumblingSlot->timer)++;
}

/*
 * Maps sequenceIndex through the byte table g_BattleHitTargetActors to an actor index, stores that
 * actor's target bone world position in g_BattleLimitEarthRaveStandPos, and offsets X and Z by the summed
 * scaled radii of the source and target actors along the target's facing. Stores the facing plus
 * 0x800 in g_BattleLimitEarthRaveStandPos.yaw and returns the target actor.
 */
// FUNCTION: FF7 0x004c7f9f
int __cdecl BattleFx_LimitEarthRaveComputeStandPointByFacing(int sequenceIndex)
{
    int actorIdx;
    int distance;
    int facing;

    actorIdx = g_BattleHitTargetActors[sequenceIndex];
    Battle_GetBoneWorldPos(actorIdx, g_BattleActorRecords[actorIdx].targetBone, &g_BattleLimitEarthRaveStandPos.x);
    distance = (g_BattleActorRecords[g_BattleLimitEarthRaveActorIndex].radius * g_BattleActorRecords[g_BattleLimitEarthRaveActorIndex].scale >> 12) + (g_BattleActorRecords[actorIdx].radius * g_BattleActorRecords[actorIdx].scale >> 12);
    facing = g_BattleActorRecords[actorIdx].facing;
    g_BattleLimitEarthRaveStandPos.x += SinFixed(facing) * distance >> 12;
    g_BattleLimitEarthRaveStandPos.z -= CosFixed(facing) * distance >> 12;
    g_BattleLimitEarthRaveStandPos.yaw = facing + 0x800;
    return actorIdx;
}

/*
 * Maps sequenceIndex through g_BattleHitTargetActors to an actor index, stores that actor's target
 * bone world position in g_BattleLimitEarthRaveStandPos, and moves it back toward the acting actor by the
 * summed scaled radii of both actors. Stores the bearing plus 0x800 in g_BattleLimitEarthRaveStandPos.yaw
 * and returns the target actor.
 */
// FUNCTION: FF7 0x004c809e
int __cdecl BattleFx_LimitEarthRaveComputeStandPointByBearing(int sequenceIndex)
{
    int bearing;
    int range;
    int actorIndex;
    int ofsAngle;

    actorIndex = g_BattleHitTargetActors[sequenceIndex];
    Battle_GetBoneWorldPos(actorIndex, g_BattleActorRecords[actorIndex].targetBone, &g_BattleLimitEarthRaveStandPos.x);
    range = (g_BattleActorRecords[g_BattleLimitEarthRaveActorIndex].radius * g_BattleActorRecords[g_BattleLimitEarthRaveActorIndex].scale >> 12) + (g_BattleActorRecords[actorIndex].radius * g_BattleActorRecords[actorIndex].scale >> 12);
    bearing = ArcTangentFixed(g_BattleActorRecords[actorIndex].posX - g_BattleActorRecords[g_BattleLimitEarthRaveActorIndex].posX, g_BattleActorRecords[actorIndex].posZ - g_BattleActorRecords[g_BattleLimitEarthRaveActorIndex].posZ);
    ofsAngle = 0x1000 - (bearing + 0x800);
    g_BattleLimitEarthRaveStandPos.x -= (short)(SinFixed(ofsAngle) * range >> 12);
    g_BattleLimitEarthRaveStandPos.z += (short)(CosFixed(ofsAngle) * range >> 12);
    g_BattleLimitEarthRaveStandPos.yaw = bearing + 0x800;
    return actorIndex;
}
} /* extern "C" */
