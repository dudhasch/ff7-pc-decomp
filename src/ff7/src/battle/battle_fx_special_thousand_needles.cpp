// src/ff7/src/battle/battle_fx_special_thousand_needles.cpp
/* Enemy Special Effect [idx 31]: ThousandNeedles (JP: hari / 針 (Needle), asset: "SPECIAL/HARI") — 1000 Needles (0x34e, Cactuar), 100 Needles (0x2fc), Spiky Hell (0x1e4, Bloatfloat); also GeneralDispatchTable[59] */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle special effect for the Hari attack: loads resources, spawns per-target timelines
//   that burst needles, and drives needle impact spark sprites and scattering needle models.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/api/battle.h"
#include "ff7/battle_tasks.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_special.h"
#include "ff7/camera_script.h"
#include "lib/types/graphics.h"
#include <string.h>
#include <stdlib.h>
#include "lib/codegen.h"
extern "C" {

struct BattleFxModelDraw;
// clang-format off
// GLOBAL: FF7 0x008a8790
int g_BattleSpecialThousandNeedlesResourceDefs = 0x001900B4;
char *g_BattleFxSpecialThousandNeedlesResourceNames[1] = {
    // STRING: FF7 0x008a8810
    "HARI1.RSD"
};
unsigned int g_BattleSpecialThousandNeedlesResourceDefsDataTail[18] = {
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x008a881c
    (unsigned int)"HARI1.RSD",
    0x00000000,
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x008a8828
    (unsigned int)"HAHEN.TIM",
    // STRING: FF7 0x008a8834
    (unsigned int)"HAHEN.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x008a8840
    (unsigned int)"IRO.TIM",
    // STRING: FF7 0x008a8848
    (unsigned int)"IRO.SPT",
    0x00000000,
    0x00000000
};
// GLOBAL: FF7 0x008a87e0
BattleSpecialThousandNeedlesSpriteData g_BattleSpecialThousandNeedlesSpriteData = {0, 0, {0x80, 0x80, 0x80, 0x2c}, 0, {0, 0}};
unsigned char g_rawData_008a87f0[16] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x2c, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x008a8800
int g_BattleSpecialThousandNeedlesScaleVector = 1536;
unsigned char g_rawData_008a8804[4] = {0x00, 0x06, 0x00, 0x00};
unsigned char g_rawData_008a8808[8] = {0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// clang-format on
#pragma intrinsic(memset)
#pragma intrinsic(memcpy)


// clang-format off
// GLOBAL: FF7 0x00b72f48
int g_BattleSpecialThousandNeedlesDrawListHandle = 0;
// GLOBAL: FF7 0x00b72f4c
struct GfxTableCursor *g_BattleSpecialThousandNeedlesLoadedRes = 0;
unsigned char g_bssPad_00b72f50[16] = {0};
// clang-format on

/*
 * Loads resources for the Hari special effect when not already loaded.
 */
// FUNCTION: FF7 0x0055da60
void __cdecl BattleFx_SpecialLoadResThousandNeedles(void)
{
    GfxModelTableBank *ownerHandle;
    GfxResourceRequest reqBuffer;
    int kind;
    char pathWorkBuffer[200];
    GfxResourceRequest *pRequest;
    int resourceDefsWord;

    if (g_pBattle3dScene != 0 && g_BattleSpecialThousandNeedlesLoadedRes == 0) {
        kind = 4;
        resourceDefsWord = (int)&g_BattleSpecialThousandNeedlesResourceDefs;
        ownerHandle = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRequest = &reqBuffer;
        Gfx_InitResourceRequest(pRequest);
        pRequest->groupTag = 0;
        // STRING: FF7 0x008a8850
        pRequest->resourcePath = BattleFx_BuildResourcePath("SPECIAL/HARI", pathWorkBuffer);
        Gfx_LoadModelTableIfAbsent(kind, resourceDefsWord, pRequest, &g_BattleSpecialThousandNeedlesLoadedRes, ownerHandle);
    }
}

/*
 * Main entry point for the Hari special effect.
 */
// FUNCTION: FF7 0x0055daf0
void __cdecl BattleFx_SpecialEnterThousandNeedles(int targetMask, int actorIdx)
{
    BattleFx_SpecialLoadResThousandNeedles();
    Battle_CreateBitPatternCallbackTask(targetMask, actorIdx, 4, BattleFx_SpecialSpawnThousandNeedlesPerTarget);
    Battle_PlaySfxPannedToActor(0x63, (short)actorIdx, 0x40);
}

/*
 * Spawns timeline task for a single Hari target.
 */
// FUNCTION: FF7 0x0055db22
void __cdecl BattleFx_SpecialSpawnThousandNeedlesPerTarget(int actor, int casterIndex)
{
    BattleFxOilTimelineSlot *taskRecord;

    taskRecord = (BattleFxOilTimelineSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialThousandNeedlesTimelineTask);
    Battle_GetBoneWorldPos(casterIndex, g_BattleActorRecords[casterIndex].targetBone, &taskRecord->startX);
    Battle_GetBoneWorldPos(actor, g_BattleActorRecords[actor].targetBone, &taskRecord->endX);
    taskRecord->actorIndex = actor;
}

/*
 * Drives the main timeline and needle burst spawning for Hari.
 */
// FUNCTION: FF7 0x0055db98
void __cdecl BattleFx_SpecialThousandNeedlesTimelineTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[88];
        int tmpSignedZ;
        struct { unsigned char pad_to_4[0x4]; SVECTOR angleA; };
        struct { unsigned char pad_to_C[0xC]; int projDepth; };
        struct { unsigned char pad_to_10[0x10]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_30[0x30]; BattleFxThousandNeedlesTimelineSpawnSlot *spawnSlot; };
        struct { unsigned char pad_to_38[0x38]; short needleIndex; };
        struct { unsigned char pad_to_3C[0x3C]; short stepX; };
        struct { unsigned char pad_to_3E[0x3E]; short stepY; };
        struct { unsigned char pad_to_40[0x40]; short stepZ; };
        struct { unsigned char pad_to_44[0x44]; BattleFxOilTimelineSlot *timelineSlot; };
        struct { unsigned char pad_to_4C[0x4C]; SVECTOR scatterX; };
        struct { unsigned char pad_to_54[0x54]; short randAngle; };
    } frame;
#pragma pack(pop)

    frame.timelineSlot = (BattleFxOilTimelineSlot *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.timelineSlot->frameCounter == 0) {
        frame.angleA.vy = ArcTangentFixed(frame.timelineSlot->endX - frame.timelineSlot->startX, frame.timelineSlot->endZ - frame.timelineSlot->startZ);
        frame.angleA.vz = 0;
        frame.angleA.vx = frame.angleA.vz;
        RotMatrix(&frame.angleA, &frame.rotMatrix);
        frame.rotMatrix.t[2] = 0;
        frame.rotMatrix.t[1] = frame.rotMatrix.t[2];
        frame.rotMatrix.t[0] = frame.rotMatrix.t[1];
        SetRotMatrix(&frame.rotMatrix);
        SetTransMatrix(&frame.rotMatrix);
        frame.stepX = (frame.timelineSlot->endX - frame.timelineSlot->startX) / 8;
        frame.stepY = (frame.timelineSlot->endY - frame.timelineSlot->startY) / 8;
        frame.stepZ = (frame.timelineSlot->endZ - frame.timelineSlot->startZ) / 8;
        for (frame.needleIndex = 0; frame.needleIndex < 30; frame.needleIndex++) {
            frame.spawnSlot = (BattleFxThousandNeedlesTimelineSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialThousandNeedlesScatterModelTask);
            frame.randAngle = rand() & 0xFFF;
            frame.scatterX.vx = CosFixed(frame.randAngle) * frame.needleIndex / 30 * 0x140 >> 12;
            frame.scatterX.vy = SinFixed(frame.randAngle) * frame.needleIndex / 30 * 0x140 >> 12;
            frame.scatterX.vz = rand() % (CosFixed((frame.needleIndex << 10) / 30) / 6);
            if ((rand() & 1) != 0) {
                frame.tmpSignedZ = frame.scatterX.vz;
            } else {
                frame.tmpSignedZ = -frame.scatterX.vz;
            }
            frame.scatterX.vz = *(short *)&frame.tmpSignedZ;
            RotTransPointToInt16(&frame.scatterX, (short *)&frame.scatterX, &frame.projDepth);
            frame.spawnSlot->posX = frame.timelineSlot->startX + frame.scatterX.vx;
            frame.spawnSlot->posY = frame.timelineSlot->startY + frame.scatterX.vy;
            frame.spawnSlot->posZ = frame.timelineSlot->startZ + frame.scatterX.vz;
            frame.spawnSlot->velocityX = frame.stepX;
            frame.spawnSlot->velocityY = frame.stepY;
            frame.spawnSlot->velocityZ = frame.stepZ;
            frame.spawnSlot->angle = frame.angleA.vy;
            frame.spawnSlot->spinAngle = 0xFC00 /* 5670 deg */;
        }
    }
    if (frame.timelineSlot->frameCounter == 8) {
        Battle_PostActorHitOrHurtMessage(frame.timelineSlot->actorIndex);
    }
    if (frame.timelineSlot->frameCounter > 8 && frame.timelineSlot->frameCounter < 12) {
        frame.spawnSlot = (BattleFxThousandNeedlesTimelineSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialThousandNeedlesScatteredSpriteTask);
        Battle_GetBoneWorldPos(frame.timelineSlot->actorIndex, g_BattleActorRecords[g_BattleSpecialActorId].targetBone, &frame.spawnSlot->posX);
        frame.spawnSlot->posX += (short)(rand() % 1000 - 500);
        frame.spawnSlot->posY += (short)(rand() % 1000 - 500);
        frame.spawnSlot->posZ += (short)(rand() % 1000 - 500);
        frame.spawnSlot->scale = 0x3000 /* 3.0 */;
        frame.spawnSlot->pushLength = -g_BattleActorRecords[g_BattleSpecialActorId].radius;
    }
    (frame.timelineSlot->frameCounter)++;
    if (frame.timelineSlot->frameCounter == 40) {
        frame.timelineSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Updates and renders needle impact spark sprites for Hari.
 */
// FUNCTION: FF7 0x0055df5f
void __cdecl BattleFx_SpecialThousandNeedlesScatteredSpriteTask(void)
{
    BattleFxThousandNeedlesScatteredSprite *task;

    task = (BattleFxThousandNeedlesScatteredSprite *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecialThousandNeedlesSpriteData.resource = Gfx_ModelTableGetLoadedRecord(3, g_BattleSpecialThousandNeedlesLoadedRes);
    g_BattleSpecialThousandNeedlesSpriteData.scatterSpriteField = Gfx_ModelTableGetSpriteObject(3, g_BattleSpecialThousandNeedlesLoadedRes);
    BattleFx_InstallBillboardMatrix(&task->posX, (short)task->scale, task->pushLength);
    g_BattleSpecialThousandNeedlesSpriteData.scatterFrameIndex = task->frameIndex;
    g_BattleSpecialThousandNeedlesDrawListHandle = BattleFx_SubmitSpriteDraw(&g_BattleSpecialThousandNeedlesSpriteData, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialThousandNeedlesDrawListHandle);
    if (g_BattleIsPaused == 0 && g_BattleSpecialThousandNeedlesSpriteData.resource != 0) {
        (task->frameIndex)++;
        if (task->frameIndex >= *(int *)((char *)*(int *)g_BattleSpecialThousandNeedlesSpriteData.resource + 4)) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Updates and renders scattering needle models for Hari.
 */
// FUNCTION: FF7 0x0055e037
void __cdecl BattleFx_SpecialThousandNeedlesScatterModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[116];
        SVECTOR spinAngle;
        struct { unsigned char pad_to_8[0x8]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_38[0x38]; BattleFxThousandNeedlesScatterModelSlot *needleSlot; };
        struct { unsigned char pad_to_3C[0x3C]; short speed; };
        struct { unsigned char pad_to_40[0x40]; PsxMatrix drawMatrix; };
        struct { unsigned char pad_to_60[0x60]; short randAngle; };
        struct { unsigned char pad_to_64[0x64]; int modelField; };
        struct { unsigned char pad_to_68[0x68]; int tmpMode; };
        struct { unsigned char pad_to_6C[0x6C]; short tmpZeroA; };
        struct { unsigned char pad_to_6E[0x6E]; short tmpLevel; };
        struct { unsigned char pad_to_70[0x70]; short tmpStart; };
        struct { unsigned char pad_to_72[0x72]; short tmpZeroB; };
    } frame;
#pragma pack(pop)

    frame.needleSlot = (BattleFxThousandNeedlesScatterModelSlot *)BATTLE_CURRENT_TASK_SLOT;
    frame.modelField = 0;
    frame.tmpMode = 0;
    frame.tmpZeroA = 0;
    frame.tmpLevel = 0x80;
    frame.tmpStart = 0x20;
    frame.tmpZeroB = 0;
    frame.spinAngle.vx = frame.needleSlot->spinAngle;
    frame.spinAngle.vy = frame.needleSlot->tiltAngle;
    frame.spinAngle.vz = 0;
    RotMatrix(&frame.spinAngle, &frame.rotMatrix);
    ScaleMatrix(&frame.rotMatrix, &g_BattleSpecialThousandNeedlesScaleVector);
    frame.rotMatrix.t[0] = frame.needleSlot->posX;
    frame.rotMatrix.t[1] = frame.needleSlot->posY;
    frame.rotMatrix.t[2] = frame.needleSlot->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.drawMatrix);
    SetRotMatrix(&frame.drawMatrix);
    SetTransMatrix(&frame.drawMatrix);
    if (frame.needleSlot->frameCounter >= 18) {
        frame.tmpLevel = 0x1A - frame.needleSlot->frameCounter << 4;
        frame.tmpMode = 8;
        frame.modelField = Gfx_ModelTableGetRenderObject(1, g_BattleSpecialThousandNeedlesLoadedRes);
    } else {
        frame.modelField = Gfx_ModelTableGetRenderObject(0, g_BattleSpecialThousandNeedlesLoadedRes);
    }
    g_BattleSpecialThousandNeedlesDrawListHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&frame.modelField, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialThousandNeedlesDrawListHandle);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.needleSlot->frameCounter == 8) {
        frame.randAngle = rand() & 0xFFF;
        frame.speed = (rand() % 16);
        frame.needleSlot->velocityX = CosFixed(frame.randAngle) * frame.speed >> 8;
        frame.needleSlot->velocityZ = SinFixed(frame.randAngle) * frame.speed >> 8;
        BattleFx_MakeRandomConeVector(0xFFFFFF00, 50, &frame.needleSlot->velocityX);
        frame.needleSlot->spinAngle = (rand() % 4096);
        frame.needleSlot->tiltAngle = (rand() % 4096);
    }
    if (frame.needleSlot->frameCounter > 8) {
        frame.needleSlot->spinAngle += 0x80;
        frame.needleSlot->velocityY += 32;
    }
    frame.needleSlot->posX += frame.needleSlot->velocityX;
    frame.needleSlot->posY += frame.needleSlot->velocityY;
    frame.needleSlot->posZ += frame.needleSlot->velocityZ;
    (frame.needleSlot->frameCounter)++;
    if (frame.needleSlot->frameCounter == 26) {
        frame.needleSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}
} /* extern "C" */
