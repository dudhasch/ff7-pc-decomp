// src/ff7/src/battle/yasui/yasui_limit_livewire_effect.cpp
// Limit Break [65] (Vincent Lv 2 Death Gigas): Livewire (Japanese: エレクトスパーク Electro Spark, dev tag: "LIMIT2/SPARK" / Spark)
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Spark limit break effect: streak lines from the source to the targets, expanding and
//   table-scaled models, oriented sprites along the streaks and radial sprite bursts.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/enums/sound.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_boss.h"
#include "ff7/battle_tasks.h"
#include "ff7/camera_script.h"
#include "lib/types/graphics.h"
#include <string.h>
#include <stdlib.h>
#include "ff7/api/battle.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "lib/codegen.h"
extern "C" {

struct BattleFxModelDraw;
// clang-format off
// GLOBAL: FF7 0x007ed158
int g_YasuiLimitLivewireResourceDef = 0x00192A39;
char *g_YasuiLimitLivewireEffectResourceNames[1] = {
    // STRING: FF7 0x007ed258
    "elec_1.tim"
};
unsigned int g_YasuiLimitLivewireEffectResourceNames2[28] = {
    // STRING: FF7 0x007ed264
    (unsigned int)"elec_1.spt",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x007ed270
    (unsigned int)"elec_2.tim",
    // STRING: FF7 0x007ed27c
    (unsigned int)"elec_2.spt",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x007ed288
    (unsigned int)"elec_3.tim",
    // STRING: FF7 0x007ed294
    (unsigned int)"elec_3.spt",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x007ed2a0
    (unsigned int)"hit_1.tim",
    // STRING: FF7 0x007ed2ac
    (unsigned int)"hit_1.spt",
    0x00000000,
    0x00000000,
    0x006400b1,
    // STRING: FF7 0x007ed2b8
    (unsigned int)"power_1.rsd",
    0x00000000,
    0x00000000,
    0x00000000,
    0x006400b1,
    // STRING: FF7 0x007ed2c4
    (unsigned int)"spark_1.rsd",
    0x00000000,
    0x00000000,
    0x00000000
};
// GLOBAL: FF7 0x007ed1d0
struct YasuiLimitLivewireVectorTable g_YasuiLimitLivewireVectorTable = {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0xf0, 0xff, 0xff, 0, 0, 0, 0}, 0};
unsigned char g_rawData_007ed1e4[4] = {0x00, 0xf0, 0xff, 0xff};
// GLOBAL: FF7 0x007ed1e8
int g_YasuiLimitLivewireScaleTable[7] = {0, 0, 0x1000, 0, 0, 0, 0x200};
unsigned char g_rawData_007ed204[4] = {0x00, 0x05, 0x00, 0x00};
unsigned char g_rawData_007ed208[16] = {0x00, 0x10, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x007ed218
struct YasuiLimitLivewireSpriteRequest1 g_YasuiLimitLivewireSpriteRequest1 = {0, 0, {0x80, 0x80, 0x80, 0x2e}, 0, {0}, 0, 0, {0x80, 0x80, 0x80, 0x2e}, 0, {0}, 0, 0};
// GLOBAL: FF7 0x007ed240
struct YasuiLimitLivewireSpriteRequest2 g_YasuiLimitLivewireSpriteRequest2 = {-128, -128, -128, {0x2e}, 0, {0}, 0, 0};
// GLOBAL: FF7 0x007ed250
struct YasuiLimitLivewireSpriteRequest3 g_YasuiLimitLivewireSpriteRequest3 = {-128, -128, -128, {0x2e}, 0, {0}};
// GLOBAL: FF7 0x00a41930
BattleFxModelDrawHead g_YasuiLimitLivewireModelDrawReq = {0};
// GLOBAL: FF7 0x00a41940
struct YasuiLimitLivewireRandom g_YasuiLimitLivewireRandomTable[10] = {0};
// GLOBAL: FF7 0x00a41990
YasuiLimitSparkPoint g_YasuiLimitLivewireSourceBonePos = {0};
// GLOBAL: FF7 0x00a41998
int g_YasuiLimitLivewireTargetMask = 0;
// GLOBAL: FF7 0x00a4199c
int g_YasuiLimitLivewireSourceActor = 0;
// GLOBAL: FF7 0x00a419a0
YasuiLimitSparkPoint g_YasuiLimitLivewireSavedActorPositions[10] = {0};
// GLOBAL: FF7 0x00a419f0
struct GfxTableCursor *g_YasuiLimitLivewireLoadedResTable = 0;
// clang-format on

#pragma intrinsic(memcpy)
#pragma intrinsic(memset)
#pragma intrinsic(abs)


/*
 * Loads the "LIMIT2/SPARK" resource package for the Spark limit break.
 */
// FUNCTION: FF7 0x00463e50
void __cdecl BattleFx_LimitLoadResLivewire(void)
{
    char pathScratch[200];
    GfxModelTableBank *context;
    GfxResourceRequest requestRecord;
    int kind;
    GfxResourceRequest *pReq;
    int resourceRows;

    if (g_pBattle3dScene != 0 && g_YasuiLimitLivewireLoadedResTable == 0) {
        kind = 6;
        resourceRows = (int)&g_YasuiLimitLivewireResourceDef;
        context = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pReq = &requestRecord;
        Gfx_InitResourceRequest(pReq);
        pReq->groupTag = 0;
        // STRING: FF7 0x007ed2d0
        pReq->resourcePath = BattleFx_BuildResourcePath("LIMIT2/SPARK", pathScratch);
        Gfx_LoadModelTableIfAbsent(kind, resourceRows, pReq, &g_YasuiLimitLivewireLoadedResTable, context);
    }
}

/*
 * Entry point for Spark limit effect; loads resources and begins effect.
 */
// FUNCTION: FF7 0x00463ee0
int __cdecl BattleFx_LimitEnterLivewire(int targetMask, int actorIdx)
{
    BattleFx_LimitBeginLivewire(targetMask, actorIdx);
    return 0;
}

/*
 * Sets up render states, stores target/actor parameters, and spawns timeline task.
 */
// FUNCTION: FF7 0x00463ef7
void __cdecl BattleFx_LimitBeginLivewire(int targetMask, int actorIdx)
{
    int padSlot[2]; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */

    *(int *)&g_BattleFxDrawListBufferTaskSlotPtr = (int)BATTLE_NEW_TASK_SLOT(BattleFx_SelectDrawListBuffer);
    BattleFx_LimitLoadResLivewire();
    Battle_AllocateResourceSlot(BattleFx_LimitLivewireSpawnTimeline);
    g_YasuiLimitLivewireTargetMask = targetMask;
    g_YasuiLimitLivewireSourceActor = actorIdx;
}

/*
 * Computes source bone position and target centroid, and spawns the main timeline task.
 */
// FUNCTION: FF7 0x00463f42
void __cdecl BattleFx_LimitLivewireSpawnTimeline(void)
{
    int actorIndex;
    BattleFxLimitLivewireTimeline *task;

    task = (BattleFxLimitLivewireTimeline *)BATTLE_CURRENT_TASK_SLOT;
    task->ownerSlot = BATTLE_TASK_FREE;
    Battle_GetBoneWorldPos(g_YasuiLimitLivewireSourceActor, g_BattleActorRecords[g_YasuiLimitLivewireSourceActor].targetBone, &g_YasuiLimitLivewireSourceBonePos.x);
    g_YasuiLimitLivewireSourceBonePos.radius = g_BattleActorRecords[g_YasuiLimitLivewireSourceActor].radius;
    task = (BattleFxLimitLivewireTimeline *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitLivewireTimelineTask);
    Battle_GetActorMaskCentroid(g_YasuiLimitLivewireTargetMask, &task->posX);
    task->posY = 0xFCA0;
    task->spreadX = 3456;
    task->spreadY = 1440;
    task->clearedWord = 0;
    for (actorIndex = 0; actorIndex < 10; actorIndex++) {
        if ((g_YasuiLimitLivewireTargetMask & 1 << actorIndex) != 0) {
            g_YasuiLimitLivewireSavedActorPositions[actorIndex] = *(YasuiLimitSparkPoint *)&g_BattleActorRecords[actorIndex].posX;
            g_YasuiLimitLivewireRandomTable[actorIndex].randomValue = rand();
            g_YasuiLimitLivewireRandomTable[actorIndex].shakeStep = 0;
        }
    }
}

/*
 * Master timeline task coordinating camera shake, screen flash, streak models, and sprite bursts.
 */
// FUNCTION: FF7 0x0046406e
void __cdecl BattleFx_LimitLivewireTimelineTask(void)
{
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[104];
        struct { unsigned char pad_to_64[0x64]; short trailStart; };
        struct {
            unsigned char pad[96];
            int value;
        } at8;
        struct {
            unsigned char pad[92];
            int value;
        } at12;
        struct {
            unsigned char pad[88];
            int value;
        } at16;
        struct { unsigned char pad_to_54[0x54]; int axisVec; };
        struct { unsigned char pad_to_50[0x50]; int tmpIndex; };
        struct { unsigned char pad_to_4C[0x4C]; int spread; };
        struct { unsigned char pad_to_48[0x48]; int tmpRange; };
        struct {
            unsigned char pad[68];
            int value;
        } at36;
        struct { unsigned char pad_to_38[0x38]; GfxDirectionVec3 vecX; };
        struct { unsigned char pad_to_34[0x34]; int spreadY; };
        struct { unsigned char pad_to_30[0x30]; int scale; };
        struct { unsigned char pad_to_2C[0x2C]; int randomScale; };
        struct { unsigned char pad_to_28[0x28]; BattleFxLimitLivewireParticle *streak; };
        struct { unsigned char pad_to_24[0x24]; BattleFxLimitLivewireParticle *particle; };
        struct { unsigned char pad_to_20[0x20]; BattleFxLimitLivewireTimelineSlot *sparkSprite; };
        struct {
            unsigned char pad[28];
            int value;
        } at76;
        struct {
            unsigned char pad[24];
            int value;
        } at80;
        struct {
            unsigned char pad[20];
            int value;
        } at84;
        struct {
            unsigned char pad[16];
            int value;
        } at88;
        struct {
            unsigned char pad[12];
            int value;
        } at92;
        struct {
            unsigned char pad[8];
            int value;
        } at96;
        struct {
            unsigned char pad[4];
            int value;
        } at100;
        int rotMatrix;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.sparkSprite = (BattleFxLimitLivewireTimelineSlot *)BATTLE_CURRENT_TASK_SLOT;
    if (frame.sparkSprite->phase < 16) {
        if (frame.sparkSprite->phase % 3 == 1) {
            BattleFx_LimitLivewireSpawnSpriteBurst(frame.sparkSprite, 1);
        }
    } else if (frame.sparkSprite->phase < 25) {
        if ((frame.sparkSprite->phase & 1) != 0) {
            BattleFx_LimitLivewireSpawnSpriteBurst(frame.sparkSprite, 1);
        }
    } else if (frame.sparkSprite->phase < 38) {
        BattleFx_LimitLivewireSpawnSpriteBurst(frame.sparkSprite, 1);
    } else if (frame.sparkSprite->phase < 63) {
        BattleFx_LimitLivewireSpawnSpriteBurst(frame.sparkSprite, 2);
    } else if (frame.sparkSprite->phase < 65) {
        BattleFx_LimitLivewireSpawnSpriteBurst(frame.sparkSprite, 1);
    } else if (frame.sparkSprite->phase < 80) {
        if ((frame.sparkSprite->phase & 1) != 0) {
            BattleFx_LimitLivewireSpawnSpriteBurst(frame.sparkSprite, 1);
        }
    } else if (frame.sparkSprite->phase % 3 == 1) {
        BattleFx_LimitLivewireSpawnSpriteBurst(frame.sparkSprite, 1);
    }
    if (frame.sparkSprite->phase == 6) {
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_CH1, Battle_ComputeScreenPanFromPosition(&g_YasuiLimitLivewireSourceBonePos), 0x183);
    } else if (frame.sparkSprite->phase == 0x26) {
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_CH2, 0x40, 0x176);
    } else if (frame.sparkSprite->phase == 0x41) {
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_CH1, Battle_ComputeScreenPanFromPosition(&g_YasuiLimitLivewireSourceBonePos), 0);
    }
    if (frame.sparkSprite->phase >= 37 && frame.sparkSprite->phase <= 56) {
        frame.tmpIndex = (frame.sparkSprite->phase + 1) % 4;
        if (frame.tmpIndex == 0) {
            frame.particle = (BattleFxLimitLivewireParticle *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitLivewireExpandingModelTask);
            *(YasuiLimitSparkPoint *)&frame.particle->posX = g_YasuiLimitLivewireSourceBonePos;
            frame.particle->scale = 0;
            frame.particle->scaleStep = (g_YasuiLimitLivewireSourceBonePos.radius << 12) / 900;
        }
    }
    if (frame.sparkSprite->phase >= 40 && frame.sparkSprite->phase <= 55) {
        frame.spread = frame.sparkSprite->spreadX;
        frame.tmpRange = frame.spread << 1;
        frame.spreadY = frame.sparkSprite->spreadY;
        frame.particle = (BattleFxLimitLivewireParticle *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitLivewireOrientedSpriteTask);
        *(Int64Pair *)&frame.particle->posX = *(Int64Pair *)&frame.sparkSprite->posX;
        frame.particle->posX += (short)(rand() % frame.tmpRange - frame.spread);
        frame.particle->posY += (short)(rand() % (frame.spreadY << 1) - frame.spreadY);
        frame.spread = frame.spread / 3 << 1;
        frame.tmpRange = frame.spread << 1;
        frame.particle->posZ += (short)(rand() % frame.tmpRange - frame.spread);
        frame.spread = 6144;
        frame.tmpRange = rand() % 4608 + 3584;
        frame.particle->scale = frame.spread * frame.tmpRange >> 12;
    }
    if (frame.sparkSprite->phase >= 40 && frame.sparkSprite->phase <= 55) {
        frame.spread = frame.sparkSprite->spreadX;
        frame.tmpRange = frame.spread << 1;
        frame.spreadY = frame.sparkSprite->spreadY;
        frame.streak = (BattleFxLimitLivewireParticle *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitLivewireStreakTask);
        *(Int64Pair *)&frame.streak->posX = *(Int64Pair *)&frame.sparkSprite->posX;
        frame.streak->posX += (short)(rand() % frame.tmpRange - frame.spread);
        frame.streak->posY += (short)(rand() % (frame.spreadY << 1) - frame.spreadY);
        frame.spread = frame.spread / 3 << 1;
        frame.tmpRange = frame.spread << 1;
        frame.streak->posZ += (short)(rand() % frame.tmpRange - frame.spread);
        frame.streak->lifetime = (rand() % 32) + 16;
        frame.vecX.x = frame.streak->posX - g_YasuiLimitLivewireSourceBonePos.x;
        frame.vecX.y = frame.streak->posY - g_YasuiLimitLivewireSourceBonePos.y;
        frame.vecX.z = frame.streak->posZ - g_YasuiLimitLivewireSourceBonePos.z;
        frame.tmpIndex = NormalizeVectorFixed((VECTOR *)&frame.vecX, &frame.vecX);
        frame.spread = SqrtToInt(frame.tmpIndex) / 3;
        frame.streak->posX += (short)(frame.spread * frame.vecX.x >> 12);
        frame.streak->posY += (short)(frame.spread * frame.vecX.y >> 12);
        frame.streak->posZ += (short)(frame.spread * frame.vecX.z >> 12);
        frame.streak->scale = 8;
        frame.streak->scaleStep = (rand() % 0x1A4) * 2 + 0x1E0;
        frame.vecX.x = -frame.streak->posY - frame.streak->posZ;
        frame.vecX.y = frame.streak->posX - frame.streak->posZ;
        frame.vecX.z = frame.streak->posX + frame.streak->posY;
        NormalizeVectorFixed((VECTOR *)&frame.vecX, &frame.vecX);
        frame.spread = (rand() % 4096);
        Gte_BuildAxisAngleMatrix(frame.spread, (PsxMatrix *)&frame.rotMatrix, (VECTOR *)&frame.axisVec);
        TransformVectorToInt32((PsxMatrix *)&frame.rotMatrix, &frame.vecX, &frame.vecX);
        frame.streak->offsetX = frame.vecX.x;
        frame.streak->offsetY = frame.vecX.y;
        frame.streak->offsetZ = frame.vecX.z;
        frame.streak->durationOrIndex = (rand() % 8) + 10;
        frame.streak->scaleStep = frame.streak->scaleStep / frame.streak->durationOrIndex;
    }
    if (frame.sparkSprite->phase >= 42 && frame.sparkSprite->phase <= 60) {
        frame.spread = frame.sparkSprite->spreadX;
        frame.tmpRange = frame.spread << 1;
        frame.spreadY = frame.sparkSprite->spreadY;
        frame.scale = 0x1CCC;
        for (frame.tmpIndex = 0; frame.tmpIndex < 2; frame.tmpIndex++) {
            frame.particle = (BattleFxLimitLivewireParticle *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitLivewireTableScaledModelTask);
            *(Int64Pair *)&frame.particle->posX = *(Int64Pair *)&frame.sparkSprite->posX;
            frame.particle->posX += (short)(rand() % frame.tmpRange - frame.spread);
            frame.particle->posY += (short)(rand() % (frame.spreadY << 1) - frame.spreadY);
            frame.particle->posZ += (short)(rand() % frame.tmpRange - frame.spread);
            frame.randomScale = (rand() % 4096) + 0x200;
            frame.particle->scale = frame.scale * frame.randomScale >> 12;
            frame.particle->durationOrIndex = 0;
        }
        frame.spread = 32;
        for (frame.tmpIndex = 0; frame.tmpIndex < 10; frame.tmpIndex++) {
            if ((g_YasuiLimitLivewireTargetMask & 1 << frame.tmpIndex) != 0) {
                if ((g_YasuiLimitLivewireRandomTable[frame.tmpIndex].shakeStep & 1) != 0) {
                    frame.tmpRange = frame.spread;
                } else {
                    frame.tmpRange = -frame.spread;
                }
                if ((g_YasuiLimitLivewireRandomTable[frame.tmpIndex].randomValue & 1) != 0) {
                    frame.tmpRange = -frame.tmpRange;
                }
                g_BattleActorRecords[frame.tmpIndex].posX = g_YasuiLimitLivewireSavedActorPositions[frame.tmpIndex].x + frame.tmpRange;
                (g_YasuiLimitLivewireRandomTable[frame.tmpIndex].shakeStep)++;
            }
        }
    }
    if (frame.sparkSprite->phase < 8) {
        g_BattleFxTimelineState.channel2 = 0;
        g_BattleFxTimelineState.channel1 = g_BattleFxTimelineState.channel2;
        g_BattleFxTimelineState.red = g_BattleFxTimelineState.channel1;
        frame.trailStart = frame.sparkSprite->phase * 320;
    } else if (frame.sparkSprite->phase >= 72) {
        frame.trailStart = 0xA00 - (frame.sparkSprite->phase - 0x48) * 320;
    } else {
        frame.trailStart = 0xA00;
    }
    if (frame.sparkSprite->phase == 0x4B) {
        for (frame.tmpIndex = 0; frame.tmpIndex < 10; frame.tmpIndex++) {
            if ((g_YasuiLimitLivewireTargetMask & 1 << frame.tmpIndex) != 0) {
                Battle_PostActorHitOrHurtMessage(frame.tmpIndex);
            }
        }
    }
    if (frame.sparkSprite->phase >= 80) {
        for (frame.tmpIndex = 0; frame.tmpIndex < 10; frame.tmpIndex++) {
            if ((g_YasuiLimitLivewireTargetMask & 1 << frame.tmpIndex) != 0) {
                *(YasuiLimitSparkPoint *)&g_BattleActorRecords[frame.tmpIndex].posX = g_YasuiLimitLivewireSavedActorPositions[frame.tmpIndex];
            }
        }
        frame.trailStart = 0;
        frame.sparkSprite->ownerSlot = BATTLE_TASK_FREE;
    }
    g_BattleFxTimelineState.startValue = frame.trailStart;
    (frame.sparkSprite->phase)++;
}

/*
 * Updates table-scaled model animation and submits model draw request.
 */
// FUNCTION: FF7 0x00464960
void __cdecl BattleFx_LimitLivewireTableScaledModelTask(void)
{
    BattleFxModelDraw *modelDraw;
    BattleFxLimitLivewireRecord *rec;
    int sizeScale;

    rec = (BattleFxLimitLivewireRecord *)BATTLE_CURRENT_TASK_SLOT;
    sizeScale = rec->baseScale * (&g_YasuiLimitLivewireScaleTable[6])[rec->tableIndex] >> 12;
    BattleFx_InstallBillboardMatrix(&rec->posX, sizeScale, -(sizeScale >> 4));
    modelDraw = (BattleFxModelDraw *)&g_YasuiLimitLivewireModelDrawReq;
    modelDraw->modelGroup = Gfx_ModelTableGetRenderObject(5, g_YasuiLimitLivewireLoadedResTable);
    modelDraw->flags = 0x88;
    modelDraw->uvScroll = 0;
    modelDraw->fadeLevel = 0;
    modelDraw->stateFlags = 0;
    modelDraw->texturePageBits = 0;
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitModelDraw(modelDraw, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (rec->stage >= 4) {
        rec->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    (rec->tableIndex)++;
    (rec->stage)++;
}

/*
 * Updates expanding model animation with scaling and rotation.
 */
// FUNCTION: FF7 0x00464a6a
void __cdecl BattleFx_LimitLivewireExpandingModelTask(void)
{
    BattleFxModelDraw *modelDraw;
    BattleFxLimitLivewireExpander *task;

    task = (BattleFxLimitLivewireExpander *)BATTLE_CURRENT_TASK_SLOT;
    BattleFx_InstallBillboardMatrix(&task->posX, (short)task->angle, -(task->angle >> 2));
    modelDraw = (BattleFxModelDraw *)&g_YasuiLimitLivewireModelDrawReq;
    modelDraw->modelGroup = Gfx_ModelTableGetRenderObject(4, g_YasuiLimitLivewireLoadedResTable);
    modelDraw->flags = 0x88;
    modelDraw->uvScroll = 0;
    modelDraw->fadeLevel = 0;
    modelDraw->stateFlags = 0;
    modelDraw->texturePageBits = 0;
    if (task->step >= 4) {
        modelDraw->fadeLevel = task->step - 4 << 10;
    }
    SetDrawColorRGB(0, 0, 0);
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitModelDraw(modelDraw, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (task->step >= 8) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    task->angle += task->angularVelocity;
    task->angularVelocity -= (short)(task->angularVelocity / 3);
    (task->step)++;
}

/*
 * Coordinates streak line particle tasks from source toward target actors.
 */
// FUNCTION: FF7 0x00464bad
void __cdecl BattleFx_LimitLivewireStreakTask(void)
{
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[112];
        int clipFlags;
        struct { unsigned char pad_to_4[0x4]; PsxMatrix matrix; };
        struct { unsigned char pad_to_24[0x24]; char tmpBlue; };
        struct { unsigned char pad_to_28[0x28]; BattleFxLivewireStreakSlot *streak; };
        struct { unsigned char pad_to_2C[0x2C]; YasuiLimitSparkPoint tail; };
        struct { unsigned char pad_to_34[0x34]; int dir[3]; };
        struct { unsigned char pad_to_48[0x48]; char tmpUnused; };
        struct { unsigned char pad_to_4C[0x4C]; int angle; };
        struct { unsigned char pad_to_50[0x50]; int step; };
        struct { unsigned char pad_to_54[0x54]; BattleFxLivewireStreakPrim *prim; };
        struct { unsigned char pad_to_58[0x58]; short tmpY; };
        struct { unsigned char pad_to_5C[0x5C]; int depth; };
        struct { unsigned char pad_to_60[0x60]; char tmpGreen; };
        struct { unsigned char pad_to_64[0x64]; short tmpX; };
        struct { unsigned char pad_to_68[0x68]; int screenOut; };
        struct { unsigned char pad_to_6C[0x6C]; char tmpRed; };
    } frame;

    frame.streak = (BattleFxLivewireStreakSlot *)BATTLE_CURRENT_TASK_SLOT;
    frame.tail = *(YasuiLimitSparkPoint *)&frame.streak->posX;
    frame.step = frame.streak->phase;
    frame.tail.x += (short)(frame.step * frame.streak->velocityX >> 12);
    frame.tail.y += (short)(frame.step * frame.streak->velocityY >> 12);
    frame.tail.z += (short)(frame.step * frame.streak->velocityZ >> 12);
    SetRotMatrix(&g_BattleCameraTransform);
    SetTransMatrix(&g_BattleCameraTransform);
    frame.prim = (BattleFxLivewireStreakPrim *)g_BattleFxActiveDrawListPtr;
    frame.depth = Gfx_TransformTriangleReturnDepth(&g_YasuiLimitLivewireSourceBonePos, (SVECTOR *)&frame.tail, (SVECTOR *)&frame.streak->posX, &frame.screenOut, &frame.clipFlags);
    if (frame.depth > 0) {
        frame.prim->color = 0x00A0A0A0;
        frame.prim->frontColor = 0x40404;
        frame.prim->backColor = 0x40404;
        frame.prim->packetTag = frame.prim->packetTag & 0xFFFFFF | 0x6000000;
        frame.prim->code = 0x32;
        frame.tmpX = frame.prim->frontX;
        frame.tmpY = frame.prim->frontY;
        frame.tmpRed = frame.prim->frontRed;
        frame.tmpGreen = frame.prim->frontGreen;
        frame.tmpBlue = frame.prim->frontBlue;
        frame.tmpUnused = frame.prim->frontUnused;
        frame.prim->frontX = frame.prim->backX;
        frame.prim->frontY = frame.prim->backY;
        frame.prim->frontRed = frame.prim->backRed;
        frame.prim->frontGreen = frame.prim->backGreen;
        frame.prim->frontBlue = frame.prim->backBlue;
        frame.prim->frontUnused = frame.prim->backUnused;
        frame.prim->backX = frame.tmpX;
        frame.prim->backY = frame.tmpY;
        frame.prim->backRed = frame.tmpRed;
        frame.prim->backGreen = frame.tmpGreen;
        frame.prim->backBlue = frame.tmpBlue;
        frame.prim->backUnused = frame.tmpUnused;
        Gfx_DrawPsxPrimitive(frame.prim);
        frame.prim++;
    }
    g_BattleFxActiveDrawListPtr = (int)frame.prim;
    if (frame.streak->elapsed >= frame.streak->duration) {
        frame.streak->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.streak->elapsed < frame.streak->duration >> 1) {
        frame.streak->phase += frame.streak->phaseStep;
    } else {
        frame.streak->phase -= frame.streak->phaseStep;
    }
    frame.dir[0] = frame.streak->posX - g_YasuiLimitLivewireSourceBonePos.x;
    frame.dir[1] = frame.streak->posY - g_YasuiLimitLivewireSourceBonePos.y;
    frame.dir[2] = frame.streak->posZ - g_YasuiLimitLivewireSourceBonePos.z;
    NormalizeVectorFixed(frame.dir, frame.dir);
    frame.step = frame.streak->speed;
    frame.streak->posX += (short)(frame.step * frame.streak->velocityX >> 12);
    frame.streak->posY += (short)(frame.step * frame.streak->velocityY >> 12);
    frame.streak->posZ += (short)(frame.step * frame.streak->velocityZ >> 12);
    frame.angle = rand() % 0x32 + 0x18;
    Gte_BuildAxisAngleMatrix(frame.angle, &frame.matrix, (VECTOR *)frame.dir);
    TransformVectorToInt16(&frame.matrix, &frame.streak->velocityX, &frame.streak->velocityX);
    (frame.streak->elapsed)++;
}

/*
 * Renders oriented billboard sprites aligned along streak trajectories.
 */
// FUNCTION: FF7 0x00464f0d
void __cdecl BattleFx_LimitLivewireOrientedSpriteTask(void)
{
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[180];
        PsxMatrix matrix;
        struct { unsigned char pad_to_20[0x20]; BattleFxLimitLivewireOrientedSprite *sprite; };
        struct { unsigned char pad_to_24[0x24]; YasuiLimitSparkPoint anchor; };
        struct { unsigned char pad_to_30[0x30]; int flat[3]; };
        struct { unsigned char pad_to_40[0x40]; int radialLength; };
        struct { unsigned char pad_to_44[0x44]; int angle; };
        struct { unsigned char pad_to_48[0x48]; int flatLength; };
        struct { unsigned char pad_to_4C[0x4C]; short spin[3]; };
        struct { unsigned char pad_to_54[0x54]; PsxMatrix matrixB; };
        struct { unsigned char pad_to_74[0x74]; int rel[3]; };
        struct { unsigned char pad_to_84[0x84]; int toCamera[3]; };
        struct { unsigned char pad_to_94[0x94]; int radial[3]; };
        struct { unsigned char pad_to_A4[0xA4]; int scale[3]; };
    } frame;

    (g_YasuiLimitLivewireSpriteRequest1.sparkSpriteResource) = Gfx_ModelTableGetLoadedRecord(0, g_YasuiLimitLivewireLoadedResTable);
    g_YasuiLimitLivewireSpriteRequest1.sparkDrawObj = Gfx_ModelTableGetSpriteObject(0, g_YasuiLimitLivewireLoadedResTable);
    g_YasuiLimitLivewireSpriteRequest1.flareSpriteRecord = Gfx_ModelTableGetLoadedRecord(1, g_YasuiLimitLivewireLoadedResTable);
    g_YasuiLimitLivewireSpriteRequest1.flareDrawObj = Gfx_ModelTableGetSpriteObject(1, g_YasuiLimitLivewireLoadedResTable);
    frame.sprite = (BattleFxLimitLivewireOrientedSprite *)BATTLE_CURRENT_TASK_SLOT;
    frame.flat[0] = frame.sprite->posX - g_YasuiLimitLivewireSourceBonePos.x;
    frame.flat[1] = 0;
    frame.flat[2] = frame.sprite->posZ - g_YasuiLimitLivewireSourceBonePos.z;
    frame.flatLength = NormalizeVectorFixed(frame.flat, frame.flat);
    frame.angle = ComputeAngleBetweenVectors((VECTOR *)&g_YasuiLimitLivewireVectorTable, (VECTOR *)frame.flat, (VECTOR *)frame.radial);
    Gte_BuildAxisAngleMatrix(frame.angle, &frame.matrixB, (VECTOR *)frame.radial);
    frame.radial[0] = frame.sprite->posX - g_YasuiLimitLivewireSourceBonePos.x;
    frame.radial[1] = frame.sprite->posY - g_YasuiLimitLivewireSourceBonePos.y;
    frame.radial[2] = frame.sprite->posZ - g_YasuiLimitLivewireSourceBonePos.z;
    frame.radialLength = NormalizeVectorFixed(frame.radial, frame.radial);
    frame.angle = ComputeAngleBetweenVectors((VECTOR *)frame.flat, (VECTOR *)frame.radial, (VECTOR *)frame.toCamera);
    Gte_BuildAxisAngleMatrix(frame.angle, &frame.matrix, (VECTOR *)frame.toCamera);
    ConcatMatrixInPlace(&frame.matrix, &frame.matrixB);
    frame.rel[0] = frame.sprite->posX - g_YasuiLimitLivewireSourceBonePos.x;
    frame.rel[1] = frame.sprite->posY - g_YasuiLimitLivewireSourceBonePos.y;
    frame.rel[2] = frame.sprite->posZ - g_YasuiLimitLivewireSourceBonePos.z;
    frame.toCamera[0] = g_BattleCameraPosition.cameraPosX - g_YasuiLimitLivewireSourceBonePos.x;
    frame.toCamera[1] = g_BattleCameraPosition.cameraPosY - g_YasuiLimitLivewireSourceBonePos.y;
    frame.toCamera[2] = g_BattleCameraPosZ - g_YasuiLimitLivewireSourceBonePos.z;
    frame.angle = frame.rel[0] * frame.toCamera[0] + frame.rel[1] * frame.toCamera[1] + frame.rel[2] * frame.toCamera[2] >> 12;
    frame.anchor = g_YasuiLimitLivewireSourceBonePos;
    frame.anchor.x += (short)(frame.angle * frame.radial[0] >> 12);
    frame.anchor.y += (short)(frame.angle * frame.radial[1] >> 12);
    frame.anchor.z += (short)(frame.angle * frame.radial[2] >> 12);
    frame.toCamera[0] = g_BattleCameraPosition.cameraPosX - frame.anchor.x;
    frame.toCamera[1] = g_BattleCameraPosition.cameraPosY - frame.anchor.y;
    frame.toCamera[2] = g_BattleCameraPosZ - frame.anchor.z;
    NormalizeVectorFixed(frame.toCamera, frame.toCamera);
    frame.angle = ComputeAngleBetweenVectors((VECTOR *)&g_YasuiLimitLivewireVectorTable.limitSparkVectorAt16, (VECTOR *)frame.toCamera, (VECTOR *)frame.rel);
    CrossProductFixed12((VECTOR *)frame.radial, (VECTOR *)frame.toCamera, (VECTOR *)frame.rel);
    if (frame.rel[1] > 0) {
        frame.angle = -frame.angle;
    }
    frame.spin[0] = 0x400 /* 90 deg */;
    frame.spin[1] = 0;
    frame.spin[2] = frame.angle;
    RotMatrixZYX((SVECTOR *)frame.spin, &frame.matrix);
    frame.matrix.t[0] = g_YasuiLimitLivewireSourceBonePos.x;
    frame.matrix.t[1] = g_YasuiLimitLivewireSourceBonePos.y;
    frame.matrix.t[2] = g_YasuiLimitLivewireSourceBonePos.z;
    ConcatMatrixInPlace(&frame.matrixB, &frame.matrix);
    frame.angle = (SqrtToInt(frame.flatLength) << 12) / 2048;
    frame.scale[1] = frame.angle + frame.angle / 3;
    frame.scale[2] = frame.sprite->scale;
    frame.scale[0] = frame.scale[2];
    ScaleMatrix(&frame.matrix, frame.scale);
    MultiplyMatrix(&g_BattleCameraTransform, &frame.matrix, &frame.matrix);
    SetRotMatrix(&frame.matrix);
    SetTransMatrix(&frame.matrix);
    if (frame.sprite->stage < 4) {
        g_YasuiLimitLivewireSpriteRequest1.sparkFrame = frame.sprite->stage;
        g_BattleFxActiveDrawListPtr = BattleFx_SubmitSpriteDraw(&g_YasuiLimitLivewireSpriteRequest1, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    } else {
        g_YasuiLimitLivewireSpriteRequest1.flareFrame = frame.sprite->stage - 4;
        g_BattleFxActiveDrawListPtr = BattleFx_SubmitSpriteDraw(&g_YasuiLimitLivewireSpriteRequest1.flareSpriteRecord, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    }
    if (frame.sprite->stage >= 8) {
        frame.sprite->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    (frame.sprite->stage)++;
}

/*
 * Spawns a radial burst of animated sprites around the target position.
 */
// FUNCTION: FF7 0x004653d2
void __cdecl BattleFx_LimitLivewireSpawnSpriteBurst(BattleFxLimitLivewireTimelineSlot *spriteParams, int count)
{
    int velScaleB;
    int scaleA;
    int i;
    int extent;
    int span;
    BattleFxLimitLivewireBurstSprite *burstSprite;
    int padField; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int randVal;

    Battle_GetBoneWorldPos(g_YasuiLimitLivewireSourceActor, g_BattleActorRecords[g_YasuiLimitLivewireSourceActor].targetBone, &g_YasuiLimitLivewireSourceBonePos.x);
    g_YasuiLimitLivewireSourceBonePos.radius = g_BattleActorRecords[g_YasuiLimitLivewireSourceActor].radius;
    extent = g_YasuiLimitLivewireSourceBonePos.radius;
    scaleA = (extent << 12) / 430;
    velScaleB = (extent << 12) / 350;
    span = extent << 1;
    for (i = 0; i < count; i++) {
        burstSprite = (BattleFxLimitLivewireBurstSprite *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitLivewireAnimatedSpriteEntry2Task);
        *(YasuiLimitSparkPoint *)&burstSprite->posX = g_YasuiLimitLivewireSourceBonePos;
        burstSprite->posX += (short)(rand() % span - extent);
        burstSprite->posY += (short)(rand() % span - extent);
        burstSprite->posZ += (short)(rand() % span - extent);
        burstSprite->rotY = rand() % 2 << 11;
        randVal = rand() % 3584 + 1280;
        burstSprite->speed = scaleA * randVal >> 12;
        burstSprite->lifetime = rand() % 64 + 80;
    }
    extent += 0x90;
    span = extent << 1;
    for (i = 0; i < count >> 1; i++) {
        burstSprite = (BattleFxLimitLivewireBurstSprite *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitLivewireAnimatedSpriteEntry3Task);
        *(YasuiLimitSparkPoint *)&burstSprite->posX = g_YasuiLimitLivewireSourceBonePos;
        burstSprite->posX += (short)(rand() % span - extent);
        burstSprite->posY += (short)(rand() % span - extent);
        burstSprite->posZ += (short)(rand() % span - extent);
        randVal = rand() % 3072 + 1280;
        burstSprite->speed = velScaleB * randVal >> 12;
        burstSprite->lifetime = rand() % 80 + 64;
    }
}

/*
 * Animates expanding burst sprite particles (entry 2) with texture cycling and fading.
 */
// FUNCTION: FF7 0x00465655
void __cdecl BattleFx_LimitLivewireAnimatedSpriteEntry2Task(void)
{
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[60];
        PsxMatrix matrix;
        struct { unsigned char pad_to_20[0x20]; BattleFxLimitLivewireSpriteTask *sprite; };
        struct { unsigned char pad_to_24[0x24]; short angles[3]; };
        struct { unsigned char pad_to_2C[0x2C]; int scale[3]; };
    } frame;

    g_YasuiLimitLivewireSpriteRequest1.burstSpriteRecord = Gfx_ModelTableGetLoadedRecord(2, g_YasuiLimitLivewireLoadedResTable);
    g_YasuiLimitLivewireSpriteRequest1.burstDrawObj = Gfx_ModelTableGetSpriteObject(2, g_YasuiLimitLivewireLoadedResTable);
    frame.sprite = (BattleFxLimitLivewireSpriteTask *)BATTLE_CURRENT_TASK_SLOT;
    g_YasuiLimitLivewireSpriteRequest2.frame = frame.sprite->frameIndex;
    g_YasuiLimitLivewireSpriteRequest2.limitSparkSpriteRequest2Byte2 = frame.sprite->brightness;
    g_YasuiLimitLivewireSpriteRequest2.limitSparkSpriteRequest2Byte1 = g_YasuiLimitLivewireSpriteRequest2.limitSparkSpriteRequest2Byte2;
    (g_YasuiLimitLivewireSpriteRequest2.colorRed) = g_YasuiLimitLivewireSpriteRequest2.limitSparkSpriteRequest2Byte1;
    frame.angles[0] = 0;
    frame.angles[1] = frame.sprite->rotY;
    frame.angles[2] = 0;
    RotMatrixXYZ(frame.angles, &frame.matrix);
    frame.scale[2] = frame.sprite->size;
    frame.scale[1] = frame.scale[2];
    frame.scale[0] = frame.scale[1];
    ScaleMatrix(&frame.matrix, frame.scale);
    BattleFx_MakeViewSpacePointPushed(&frame.sprite->posX, -(frame.sprite->size >> 1), (Vec3sPacked *)&frame.matrix);
    SetRotMatrix(&frame.matrix);
    SetTransMatrix(&frame.matrix);
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitSpriteDraw(&g_YasuiLimitLivewireSpriteRequest1.burstSpriteRecord, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (frame.sprite->frameIndex >= 3) {
        frame.sprite->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    (frame.sprite->frameIndex)++;
}

/*
 * Animates secondary burst sprite particles (entry 3) with rotation and fading.
 */
// FUNCTION: FF7 0x004657aa
void __cdecl BattleFx_LimitLivewireAnimatedSpriteEntry3Task(void)
{
    BattleFxLimitLivewireSecondaryBurstSprite *task;

    task = (BattleFxLimitLivewireSecondaryBurstSprite *)BATTLE_CURRENT_TASK_SLOT;
    g_YasuiLimitLivewireSpriteRequest2.nextSpriteRecord = Gfx_ModelTableGetLoadedRecord(3, g_YasuiLimitLivewireLoadedResTable);
    g_YasuiLimitLivewireSpriteRequest2.nextDrawObj = Gfx_ModelTableGetSpriteObject(3, g_YasuiLimitLivewireLoadedResTable);
    g_YasuiLimitLivewireSpriteRequest3.limitSparkSpriteRequest3Frame = task->frame;
    g_YasuiLimitLivewireSpriteRequest3.limitSparkSpriteRequest3Byte2 = task->brightness;
    g_YasuiLimitLivewireSpriteRequest3.limitSparkSpriteRequest3Byte1 = g_YasuiLimitLivewireSpriteRequest3.limitSparkSpriteRequest3Byte2;
    (g_YasuiLimitLivewireSpriteRequest3.colorRed) = g_YasuiLimitLivewireSpriteRequest3.limitSparkSpriteRequest3Byte1;
    BattleFx_InstallBillboardMatrix(&task->posX, (short)task->angle, -(task->angle >> 2));
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitSpriteDraw(&g_YasuiLimitLivewireSpriteRequest2.nextSpriteRecord, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (task->frame >= 7) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    (task->frame)++;
}
} /* extern "C" */
