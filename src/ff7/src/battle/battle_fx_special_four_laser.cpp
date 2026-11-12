// src/ff7/src/battle/battle_fx_special_four_laser.cpp
/* Enemy Special Effect [idx 5]: FourLaser (JP: reza2 / レーザー2 (Laser 2), asset: "SPECIAL/REZA2") — Four Laser (0x1e0, Search Crown), Sandgun (0x21a) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle special effect for the Reza2 attack: loads resources, spawns per-target
//   timelines, and drives a directed beam with quad trail records, orbiting particle trails and
//   animated sprites.

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

// clang-format off
// GLOBAL: FF7 0x008a7fb8
int g_BattleSpecialFourLaserResourceDefs = 0x00192A39;
char *g_BattleFxSpecialFourLaserResourceNames[1] = {
    // STRING: FF7 0x008a8008
    "HASSYA.TIM"
};
unsigned int g_BattleFxSpecialFourLaserResourceNames2[8] = {
    // STRING: FF7 0x008a8014
    (unsigned int)"HASSYA.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    0x00000000,
    // STRING: FF7 0x008a8020
    (unsigned int)"JO_C05.SPT",
    0x00000000,
    0x00000000
};
#undef g_BattleSpecialFourLaserSpriteConfig
// GLOBAL: FF7 0x008a7fe0
struct BattleSpecialFourLaserSpriteConfig g_BattleSpecialFourLaserSpriteConfig = {1, {0}, 0, 0, {0x60, 0x60, 0x60, 0x2c}, 0, {0}, 0, 0, {0x80, 0x80, 0x80, 0x2c}, 0, {0}};
// clang-format on
#pragma intrinsic(memset)
#pragma intrinsic(memcpy)



// clang-format off
// GLOBAL: FF7 0x00b6f160
struct BattleSpecialFourLaserQuadRecord g_BattleSpecialFourLaserQuadRecords[500] = {0};
// GLOBAL: FF7 0x00b71870
int g_BattleSpecialFourLaserDrawListHandle = 0;
// GLOBAL: FF7 0x00b71874
struct GfxTableCursor *g_BattleSpecialFourLaserLoadedRes = 0;
// clang-format on

/*
 * Loads resources for the Reza2 special effect when not already loaded.
 */
// FUNCTION: FF7 0x005586c0
void __cdecl BattleFx_SpecialLoadResFourLaser(void)
{
    GfxModelTableBank *newSceneLink;
    GfxResourceRequest resultPending;
    int kind;
    char pathWorkBuffer[200];
    GfxResourceRequest *reqPtrPtr;
    int resourceDefsWord;

    if (g_pBattle3dScene != 0 && g_BattleSpecialFourLaserLoadedRes == 0) {
        kind = 2;
        resourceDefsWord = (int)&g_BattleSpecialFourLaserResourceDefs;
        newSceneLink = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        reqPtrPtr = &resultPending;
        Gfx_InitResourceRequest(reqPtrPtr);
        reqPtrPtr->groupTag = 0;
        // STRING: FF7 0x008a802c
        reqPtrPtr->resourcePath = BattleFx_BuildResourcePath("SPECIAL/REZA2", pathWorkBuffer);
        Gfx_LoadModelTableIfAbsent(kind, resourceDefsWord, reqPtrPtr, &g_BattleSpecialFourLaserLoadedRes, newSceneLink);
    }
}

/*
 * Main entry point for the Reza2 special effect.
 */
// FUNCTION: FF7 0x00558750
void __cdecl BattleFx_SpecialEnterFourLaser(int targetMask, int actorIdx)
{
    BattleFx_SpecialLoadResFourLaser();
    g_BattleSpecialFourLaserDrawListHandle = (int)&g_GfxPsxPacketScratch;
    BattleFx_SpecialBeginFourLaser(targetMask, actorIdx);
}

/*
 * Initializes tasks and spawns per-target effects for Reza2.
 */
// FUNCTION: FF7 0x00558774
void __cdecl BattleFx_SpecialBeginFourLaser(int targetMask, int actorIdx)
{
    short i;

    Battle_CreateBitPatternCallbackTask(targetMask, actorIdx, 4, BattleFx_SpecialSpawnFourLaserPerTarget);
    Battle_PlaySfxPannedToActor(0x6C, (short)actorIdx, 0x40);
    for (i = 0; i < 0x1F4; i++) {
        g_BattleSpecialFourLaserQuadRecords[i].active = 0;
    }
}

/*
 * Spawns timeline task for a single Reza2 target.
 */
// FUNCTION: FF7 0x005587d6
void __cdecl BattleFx_SpecialSpawnFourLaserPerTarget(int actor, int attackerId)
{
    BattleFxDarkWaveTimelineSlot *taskPtr;

    taskPtr = (BattleFxDarkWaveTimelineSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialFourLaserTimelineTask);
    Battle_GetBoneWorldPos(attackerId, g_BattleActorRecords[attackerId].bone33, (short *)&taskPtr->posX);
    Battle_GetBoneWorldPos(actor, g_BattleActorRecords[actor].targetBone, (short *)&taskPtr->velocityX);
    taskPtr->actorIndex = actor;
    if (taskPtr->posZ > taskPtr->velocityZ) {
        (g_BattleSpecialFourLaserSpriteConfig.targetSlot) = 0xFFFF;
    }
    taskPtr = (BattleFxDarkWaveTimelineSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialFourLaserQuadRecordsTask);
}

/*
 * Manages and updates active quad trail records for Reza2.
 */
// FUNCTION: FF7 0x00558882
void __cdecl BattleFx_SpecialFourLaserQuadRecordsTask(void)
{
    BattleTaskSlot *taskPtr;
    short i, alive;

    taskPtr = BATTLE_CURRENT_TASK_SLOT;
    BattleFx_SpecialFourLaserDrawQuadRecords();
    if (g_BattleIsPaused != 0) {
        return;
    }
    i = 0;
    for (alive = 0; i < 0x1F4; i++) {
        if (g_BattleSpecialFourLaserQuadRecords[i].active != 0) {
            alive++;
            (g_BattleSpecialFourLaserQuadRecords[i].timer)++;
            if (g_BattleSpecialFourLaserQuadRecords[i].timer == 8) {
                g_BattleSpecialFourLaserQuadRecords[i].active = 0;
            }
        }
    }
    (taskPtr->frameCounter)++;
    if (taskPtr->frameCounter == 0x1E) {
        taskPtr->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Renders active quad trail polygons for Reza2.
 */
// FUNCTION: FF7 0x00558962
void __cdecl BattleFx_SpecialFourLaserDrawQuadRecords(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[132];
        int projOutB;
        struct { unsigned char pad_to_8[0x8]; int projResult; };
        struct { unsigned char pad_to_C[0xC]; short fadeLevel; };
        struct { unsigned char pad_to_18[0x18]; int screenX3; };
        struct { unsigned char pad_to_1C[0x1C]; int screenY3; };
        struct { unsigned char pad_to_20[0x20]; SVECTOR quadBLoZ; };
        struct { unsigned char pad_to_30[0x30]; int screenX2; };
        struct { unsigned char pad_to_34[0x34]; int screenY2; };
        struct { unsigned char pad_to_38[0x38]; short recordIndex; };
        struct { unsigned char pad_to_3C[0x3C]; SVECTOR quadALoZ; };
        struct { unsigned char pad_to_4C[0x4C]; int screenX1; };
        struct { unsigned char pad_to_50[0x50]; int screenY1; };
        struct { unsigned char pad_to_54[0x54]; SVECTOR quadBHiZ; };
        struct { unsigned char pad_to_64[0x64]; int screenX0; };
        struct { unsigned char pad_to_68[0x68]; int screenY0; };
        struct { unsigned char pad_to_6C[0x6C]; SVECTOR quadAHiZ; };
        struct { unsigned char pad_to_74[0x74]; int projOutA; };
        struct { unsigned char pad_to_78[0x78]; BattleFxFourLaserQuadBatchHead *listHandle; };
        struct { unsigned char pad_to_7C[0x7C]; BattleSpecialFourLaserQuadRecord *quadRecord; };
        struct { unsigned char pad_to_80[0x80]; BattleFxFourLaserProjectedVertexRecord *projVertex; };
    } frame;
#pragma pack(pop)

    SetRotMatrix(&g_BattleCameraTransform);
    SetTransMatrix(&g_BattleCameraTransform);
    RefreshGlobalTransformState();
    for (frame.recordIndex = 0; frame.recordIndex < 0x1F4; frame.recordIndex++) {
        if (g_BattleSpecialFourLaserQuadRecords[frame.recordIndex].active != 0) {
            frame.quadRecord = (BattleSpecialFourLaserQuadRecord *)(frame.recordIndex * 0x14 + (char *)&g_BattleSpecialFourLaserQuadRecords);
            frame.listHandle = (BattleFxFourLaserQuadBatchHead *)g_BattleSpecialFourLaserDrawListHandle;
            frame.quadAHiZ.vx = frame.quadRecord->startPoint.vx;
            frame.quadAHiZ.vy = frame.quadRecord->startPoint.vy + 0x14;
            frame.quadAHiZ.vz = frame.quadRecord->startPoint.vz;
            frame.quadALoZ.vx = frame.quadRecord->startPoint.vx;
            frame.quadALoZ.vy = frame.quadRecord->startPoint.vy - 0x14;
            frame.quadALoZ.vz = frame.quadRecord->startPoint.vz;
            frame.quadBHiZ.vx = frame.quadRecord->endPoint.vx;
            frame.quadBHiZ.vy = frame.quadRecord->endPoint.vy + 0x14;
            frame.quadBHiZ.vz = frame.quadRecord->endPoint.vz;
            frame.quadBLoZ.vx = frame.quadRecord->endPoint.vx;
            frame.quadBLoZ.vy = frame.quadRecord->endPoint.vy - 0x14;
            frame.quadBLoZ.vz = frame.quadRecord->endPoint.vz;
            frame.projResult = Gfx_ProjectQuadToScreen(&frame.quadAHiZ, &frame.quadBHiZ, &frame.quadALoZ, &frame.quadBLoZ, (char *)frame.listHandle + 8, (char *)frame.listHandle + 0x10, (char *)frame.listHandle + 0x18, (char *)frame.listHandle + 0x20, &frame.projOutA, &frame.projOutB);
            frame.projVertex = (BattleFxFourLaserProjectedVertexRecord *)Gfx_GetProjectedVertexRecord(0);
            frame.screenX0 = frame.projVertex->projectedX;
            frame.screenY0 = frame.projVertex->projectedY;
            frame.projVertex = (BattleFxFourLaserProjectedVertexRecord *)Gfx_GetProjectedVertexRecord(1);
            frame.screenX1 = frame.projVertex->projectedX;
            frame.screenY1 = frame.projVertex->projectedY;
            frame.projVertex = (BattleFxFourLaserProjectedVertexRecord *)Gfx_GetProjectedVertexRecord(2);
            frame.screenX2 = frame.projVertex->projectedX;
            frame.screenY2 = frame.projVertex->projectedY;
            frame.projVertex = (BattleFxFourLaserProjectedVertexRecord *)Gfx_GetProjectedVertexRecord(3);
            frame.screenX3 = frame.projVertex->projectedX;
            frame.screenY3 = frame.projVertex->projectedY;
            Gfx_SetBlendFactorQuad(*(float *)&frame.screenX0, *(float *)&frame.screenY0, *(float *)&frame.screenX1, *(float *)&frame.screenY1, *(float *)&frame.screenX2, *(float *)&frame.screenY2, *(float *)&frame.screenX3, *(float *)&frame.screenY3);
            frame.listHandle->primCode = 0x3A;
            frame.listHandle->tag = 0x8000000;
            frame.fadeLevel = 0x80 - (g_BattleSpecialFourLaserQuadRecords[frame.recordIndex].timer << 4);
            frame.listHandle->colorR3 = 0;
            frame.listHandle->colorR2 = 0;
            frame.listHandle->colorR1 = 0;
            frame.listHandle->colorR0 = 0;
            frame.listHandle->colorG3 = frame.fadeLevel >> 1;
            frame.listHandle->colorG2 = frame.listHandle->colorG3;
            frame.listHandle->colorG1 = frame.listHandle->colorG2;
            frame.listHandle->colorG0 = frame.listHandle->colorG1;
            frame.listHandle->colorB3 = *(char *)&frame.fadeLevel;
            frame.listHandle->colorB2 = *(char *)&frame.fadeLevel;
            frame.listHandle->colorB1 = *(char *)&frame.fadeLevel;
            frame.listHandle->colorB0 = *(char *)&frame.fadeLevel;
            Gfx_DrawPsxPrimitive(frame.listHandle);
        }
    }
}

/*
 * Drives the main timeline and sprite generation for Reza2.
 */
// FUNCTION: FF7 0x00558c04
void __cdecl BattleFx_SpecialFourLaserTimelineTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[72];
        int rotTransOut;
        struct { unsigned char pad_to_4[0x4]; SVECTOR offX; };
        struct { unsigned char pad_to_C[0xC]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_2C[0x2C]; BattleFxFourLaserTimelineSpawnSlot *spawnSlot; };
        struct { unsigned char pad_to_34[0x34]; short spawnIndex; };
        struct { unsigned char pad_to_38[0x38]; short spanY; };
        struct { unsigned char pad_to_3C[0x3C]; BattleFxDarkWaveTimelineSlot *taskSlot; };
        struct { unsigned char pad_to_40[0x40]; int bonePos; };
        struct { unsigned char pad_to_42[0x42]; short baseY; };
    } frame;
#pragma pack(pop)

    frame.taskSlot = (BattleFxDarkWaveTimelineSlot *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.taskSlot->frameCounter == 0) {
        frame.spawnSlot = (BattleFxFourLaserTimelineSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialFourLaserDirectedSpriteTask);
        frame.spawnSlot->velocityZ = 0;
        frame.spawnSlot->velocityX = 0;
        frame.spawnSlot->angle = ArcTangentFixed(frame.taskSlot->velocityX - frame.taskSlot->posX, frame.taskSlot->velocityZ - frame.taskSlot->posZ);
        frame.offX.vy = 0;
        frame.offX.vx = frame.offX.vy;
        frame.offX.vz = 0x190;
        RotMatrix((SVECTOR *)&frame.spawnSlot->velocityX, &frame.rotMatrix);
        frame.rotMatrix.t[2] = 0;
        frame.rotMatrix.t[1] = frame.rotMatrix.t[2];
        frame.rotMatrix.t[0] = frame.rotMatrix.t[1];
        SetRotMatrix(&frame.rotMatrix);
        SetTransMatrix(&frame.rotMatrix);
        RotTransPointToInt16(&frame.offX, (short *)&frame.offX, &frame.rotTransOut);
        frame.spawnSlot->posX = frame.taskSlot->posX + frame.offX.vx;
        frame.spawnSlot->posY = frame.taskSlot->posY + frame.offX.vy;
        frame.spawnSlot->posZ = frame.taskSlot->posZ + frame.offX.vz;
        frame.spawnSlot->angle -= 0x400;
        Battle_GetBoneWorldPos(frame.taskSlot->actorIndex, g_BattleActorRecords[frame.taskSlot->actorIndex].altBone, (short *)&frame.bonePos);
        frame.spanY = frame.baseY - 0xC8;
        for (frame.spawnIndex = 0; frame.spawnIndex < 0xA; frame.spawnIndex++) {
            frame.spawnSlot = (BattleFxFourLaserTimelineSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialFourLaserOrbitTrailTask);
            frame.spawnSlot->posX = frame.taskSlot->posX + frame.offX.vx;
            frame.spawnSlot->posY = frame.taskSlot->posY + frame.offX.vy;
            frame.spawnSlot->posZ = frame.taskSlot->posZ + frame.offX.vz;
            frame.spawnSlot->velocityX = (frame.taskSlot->velocityX - frame.spawnSlot->posX) / 0xA;
            frame.spawnSlot->angle = (frame.spanY / 0xA * frame.spawnIndex - frame.spawnSlot->posY) / 0xA;
            frame.spawnSlot->velocityZ = (frame.taskSlot->velocityZ - frame.spawnSlot->posZ) / 0xA;
            if ((frame.spawnIndex % 2) == 0) {
                frame.spawnSlot->orbitAngle = (rand() % 512) - 0x100;
                frame.spawnSlot->angularVelocity = (g_BattleSpecialFourLaserSpriteConfig.targetSlot) * 0x180;
            } else {
                frame.spawnSlot->orbitAngle = (rand() % 512) + 0x700;
                frame.spawnSlot->angularVelocity = (g_BattleSpecialFourLaserSpriteConfig.targetSlot) * 0xFFFFFE80;
            }
            frame.spawnSlot->radius = 0;
            frame.spawnSlot->radiusScale = (SinFixed(frame.spawnIndex * 0xCC) * 0x384 >> 0xC) + 0x12C;
        }
    }
    if (frame.taskSlot->frameCounter == 0xA) {
        Battle_PostActorHitOrHurtMessage(frame.taskSlot->actorIndex);
    }
    if (frame.taskSlot->frameCounter > 0xA && frame.taskSlot->frameCounter < 0xF) {
        frame.spawnSlot = (BattleFxFourLaserTimelineSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialFourLaserAnimatedSpriteEntry1Task);
        Battle_GetBoneWorldPos(frame.taskSlot->actorIndex, g_BattleActorRecords[g_BattleSpecialActorId].targetBone, (short *)&frame.spawnSlot->posX);
        frame.spawnSlot->posX += (short)(rand() % 0x3E8 - 0x1F4);
        frame.spawnSlot->posY += (short)(rand() % 0x3E8 - 0x1F4);
        frame.spawnSlot->posZ += (short)(rand() % 0x3E8 - 0x1F4);
    }
    (frame.taskSlot->frameCounter)++;
    if (frame.taskSlot->frameCounter == 0x1E) {
        frame.taskSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Updates and renders directional beam sprites for Reza2.
 */
// FUNCTION: FF7 0x00559033
void __cdecl BattleFx_SpecialFourLaserDirectedSpriteTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[52];
        PsxMatrix rotMatrix;
        struct { unsigned char pad_to_20[0x20]; BattleFxFourLaserTimelineSpawnSlot *taskSlot; };
        struct { unsigned char pad_to_24[0x24]; GfxDirectionVec3 scaleVec; };
        struct { unsigned char pad_to_30[0x30]; int tmpZero; };
    } frame;
#pragma pack(pop)

    frame.taskSlot = (BattleFxFourLaserTimelineSpawnSlot *)BATTLE_CURRENT_TASK_SLOT;
    frame.scaleVec.x = 0x1800;
    frame.scaleVec.y = 0x1800;
    frame.scaleVec.z = 0x1800;
    memset(&frame.tmpZero, 0, 4);
    g_BattleSpecialFourLaserSpriteConfig.reza2SpriteConfigTable8 = Gfx_ModelTableGetLoadedRecord(0, g_BattleSpecialFourLaserLoadedRes);
    g_BattleSpecialFourLaserSpriteConfig.drawObj = Gfx_ModelTableGetSpriteObject(0, g_BattleSpecialFourLaserLoadedRes);
    RotMatrix((SVECTOR *)&frame.taskSlot->velocityX, &frame.rotMatrix);
    ScaleMatrix(&frame.rotMatrix, &frame.scaleVec);
    frame.rotMatrix.t[0] = frame.taskSlot->posX;
    frame.rotMatrix.t[1] = frame.taskSlot->posY;
    frame.rotMatrix.t[2] = frame.taskSlot->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.rotMatrix);
    SetRotMatrix(&frame.rotMatrix);
    SetTransMatrix(&frame.rotMatrix);
    if (g_BattleSpecialFourLaserSpriteConfig.reza2SpriteConfigTable8 != 0) {
        g_BattleSpecialFourLaserSpriteConfig.reza2FrameIndex20 = frame.taskSlot->frameCounter % *(int *)((char *)*(int *)g_BattleSpecialFourLaserSpriteConfig.reza2SpriteConfigTable8 + 4);
    } else {
        g_BattleSpecialFourLaserSpriteConfig.reza2FrameIndex20 = 0;
    }
    g_BattleSpecialFourLaserDrawListHandle = BattleFx_SubmitSpriteDraw(&g_BattleSpecialFourLaserSpriteConfig.reza2SpriteConfigTable8, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialFourLaserDrawListHandle);
    if (g_BattleIsPaused != 0) {
        return;
    }
    (frame.taskSlot->frameCounter)++;
    if (frame.taskSlot->frameCounter == 8) {
        frame.taskSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Updates and renders secondary animated sprites for Reza2.
 */
// FUNCTION: FF7 0x00559188
void __cdecl BattleFx_SpecialFourLaserAnimatedSpriteEntry1Task(void)
{
    BattleTaskSlot *task;

    task = BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecialFourLaserSpriteConfig.reza2DrawTable24 = Gfx_ModelTableGetLoadedRecord(1, g_BattleSpecialFourLaserLoadedRes);
    g_BattleSpecialFourLaserSpriteConfig.reza2SceneObject28 = Battle3d_GetSceneRenderObject(3, g_pBattle3dScene);
    BattleFx_InstallBillboardMatrix((short *)task->data, 0x2000, 0);
    g_BattleSpecialFourLaserSpriteConfig.reza2ArgWord36 = task->frameCounter;
    g_BattleSpecialFourLaserDrawListHandle = BattleFx_SubmitSpriteDraw(&g_BattleSpecialFourLaserSpriteConfig.reza2DrawTable24, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialFourLaserDrawListHandle);
    if (g_BattleIsPaused == 0 && g_BattleSpecialFourLaserSpriteConfig.reza2DrawTable24 != 0) {
        (task->frameCounter)++;
        if (task->frameCounter >= *(int *)((char *)*(int *)g_BattleSpecialFourLaserSpriteConfig.reza2DrawTable24 + 4)) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Updates orbiting particle trails and emits quad records for Reza2.
 */
// FUNCTION: FF7 0x00559259
void __cdecl BattleFx_SpecialFourLaserOrbitTrailTask(void)
{
    BattleSpecialFourLaserQuadRecord *quad;
    BattleFxSpiderWebOrbit *orbit;
    int r, s, t, u;

    orbit = (BattleFxSpiderWebOrbit *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    quad = (BattleSpecialFourLaserQuadRecord *)BattleFx_SpecialFourLaserClaimQuadRecord();
    if (quad != 0) {
        BattleFx_SpecialFourLaserGetOrbitOffsetPoint(orbit, &quad->startPoint);
    }
    if (orbit->step < 0xA) {
        orbit->centerX += orbit->velocityX;
        orbit->centerY += orbit->velocityY;
        orbit->centerZ += orbit->velocityZ;
        orbit->radius = SinFixed(orbit->step * 0xC8) * orbit->radiusScale >> 0xC;
    } else {
        orbit->angle += orbit->angularVelocity;
        orbit->radius -= orbit->radius / 16;
    }
    if (quad != 0) {
        BattleFx_SpecialFourLaserGetOrbitOffsetPoint(orbit, &quad->endPoint);
    }
    (orbit->step)++;
    if (orbit->step == 0x10) {
        orbit->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Allocates an available quad trail record slot for Reza2.
 */
// FUNCTION: FF7 0x00559393
int __cdecl BattleFx_SpecialFourLaserClaimQuadRecord(void)
{
    short i;

    for (i = 0; i < 0x1F4; i++) {
        if (g_BattleSpecialFourLaserQuadRecords[i].active == 0) {
            g_BattleSpecialFourLaserQuadRecords[i].active = 1;
            g_BattleSpecialFourLaserQuadRecords[i].timer = 0;
            return i * 0x14 + (int)&g_BattleSpecialFourLaserQuadRecords;
        }
    }
    return 0;
}

/*
 * Computes 3D position offset for orbiting Reza2 particles.
 */
// FUNCTION: FF7 0x005593ff
void __cdecl BattleFx_SpecialFourLaserGetOrbitOffsetPoint(BattleFxSpiderWebOrbit *orbit, Vec3sPacked *outPoint)
{
    outPoint->vx = orbit->centerX + (CosFixed(orbit->angle) * orbit->radius >> 0xC);
    outPoint->vy = orbit->centerY;
    outPoint->vz = orbit->centerZ + (SinFixed(orbit->angle) * orbit->radius >> 0xC);
}
} /* extern "C" */
