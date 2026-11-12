// src/ff7/src/battle/battle_fx_magic_stop.cpp
/* Magic Effect [21]: Stop (Japanese: ストップ, dev tag: "MAGIC/STOP") */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Stop magic effect: central, side and angle-gated clock hand models for each target.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/enums/sound.h"
#include "ff7/battle_tasks.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_magic.h"
#include "lib/types/graphics.h"
#include <stdlib.h>
#include "ff7/camera_script.h"
#include <string.h>
#include "ff7/api/battle.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "lib/codegen.h"
extern "C" {

struct BattleFxModelDraw;

#pragma intrinsic(abs)

// clang-format off
// GLOBAL: FF7 0x008b0270
int g_BattleMagicStopResourceDefs = 0x006440B4;
int g_BattleMagicStopResourceDefsHeadPtr[1] = {
    (int)((char *)&g_BattleFxMagicStopClockResourceNames + 0x0)
};
unsigned int g_BattleMagicStopResourceDefsDataTail[14] = {
    0x00000000,
    0x00000000,
    0x00000000,
    0x006440b4,
    // STRING: FF7 0x008b02cc
    (unsigned int)"stop_1.rsd",
    0x00000000,
    0x00000000,
    0x00000000,
    0x006440b1,
    // STRING: FF7 0x008b02d8
    (unsigned int)"stop_2.rsd",
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000
};
// GLOBAL: FF7 0x008b02b0
short g_BattleMagicStopAngles[4] = {1792, 1536, 1280, 1024};
// GLOBAL: FF7 0x008b02b8
short g_BattleMagicStopSubAngles[4] = {2048, 3072, 3328, 3584};
unsigned char g_BattleFxMagicStopClockResourceNames[12] = {0x68, 0x61, 0x72, 0x69, 0x5f, 0x31, 0x2e, 0x72, 0x73, 0x64, 0x00, 0x00};
// clang-format on
#pragma intrinsic(memset)
#pragma intrinsic(memcpy)


// clang-format off
// GLOBAL: FF7 0x00bb0828
BattleFxModelDrawHead g_BattleMagicStopModelReq = {0};
// GLOBAL: FF7 0x00bb0838
struct GfxTableCursor *g_BattleMagicStopLoadedResTable = 0;
// clang-format on

/* Loads and registers resources for the Stop magic effect. */
// FUNCTION: FF7 0x00580750
void __cdecl BattleFx_MagicLoadResStop(void)
{
    GfxModelTableBank *context;
    GfxResourceRequest requestRecord;
    int kind;
    char pathBuffer[200];
    GfxResourceRequest *request;
    int resultDefsRows;

    if (g_pBattle3dScene != 0 && g_BattleMagicStopLoadedResTable == 0) {
        kind = 3;
        resultDefsRows = (int)&g_BattleMagicStopResourceDefs;
        context = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        request = &requestRecord;
        Gfx_InitResourceRequest(request);
        request->groupTag = 0;
        // STRING: FF7 0x008b02e4
        request->resourcePath = BattleFx_BuildResourcePath("MAGIC/STOP", pathBuffer);
        Gfx_LoadModelTableIfAbsent(kind, resultDefsRows, request, &g_BattleMagicStopLoadedResTable, context);
    }
}

/* Entry point for the Stop magic effect. */
// FUNCTION: FF7 0x005807e0
void __cdecl BattleFx_MagicEnterStop(int targetMask, int actorIdx)
{
    BattleFx_MagicBeginStop(targetMask, actorIdx);
}

/* Initializes Stop magic effect tasks and audio. */
// FUNCTION: FF7 0x005807f5
void __cdecl BattleFx_MagicBeginStop(int targetMask, int actorIdx)
{
    *(int *)&g_BattleFxDrawListBufferTaskSlotPtr = (int)BATTLE_NEW_TASK_SLOT(BattleFx_SelectDrawListBuffer);
    BattleFx_MagicLoadResStop();
    Battle_CreateBitPatternCallbackTask(targetMask, actorIdx, 1, BattleFx_MagicSpawnStopPerTarget);
    Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputePanForEntity(targetMask), 0x57);
}

/* Spawns per-target Stop clock hand and ring model tasks. */
// FUNCTION: FF7 0x0058084c
void __cdecl BattleFx_MagicSpawnStopPerTarget(int actor)
{
    int extent;
    int fixedExtent;
    BattleFxStopSpawnRecord *pSpawn;
    BattleFxMagicStopEntry *sideEntry;

    pSpawn = (BattleFxStopSpawnRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicStopCentralModelTask);
    Battle_GetBoneWorldPos(actor, g_BattleActorRecords[actor].targetBone, (short *)&pSpawn->worldPos);
    pSpawn->angle = 0;
    pSpawn->yaw = g_BattleActorRecords[actor].yaw;
    extent = g_BattleActorRecords[actor].radius + 0x100;
    if (extent > 1900) {
        extent = 0x76C;
    }
    pSpawn->base = extent;
    if (extent > 1266) {
        extent = 0x4F2;
    }
    fixedExtent = extent << 12;
    pSpawn->ramp = 0;
    pSpawn->extent = fixedExtent / 1000;
    pSpawn->currentExtent = pSpawn->extent;
    pSpawn->currentExtent = pSpawn->currentExtent / 4;
    pSpawn->extent = pSpawn->extent / 10;
    pSpawn->targetIndex = actor;
    sideEntry = (BattleFxMagicStopEntry *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicStopSideModelTask);
    sideEntry->worldPos = pSpawn->worldPos;
    sideEntry->scale = 0;
    sideEntry->yaw = pSpawn->yaw;
    sideEntry->offset = extent * 200 / 1000;
    sideEntry->position = pSpawn->base + sideEntry->offset;
    sideEntry->offset = sideEntry->offset / 10;
    sideEntry->extentTarget = 0;
    sideEntry->extent = fixedExtent / 1120;
    sideEntry->extentStep = sideEntry->extent;
    sideEntry->extentStep = sideEntry->extentStep / 8;
    sideEntry->extent = sideEntry->extent / 10;
    sideEntry = (BattleFxMagicStopEntry *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicStopAngleGatedModelTask);
    sideEntry->worldPos = pSpawn->worldPos;
    sideEntry->scale = 0x800 /* 0.5 */;
    sideEntry->yaw = pSpawn->yaw;
    sideEntry->offset = extent * 264 / 1000;
    sideEntry->position = pSpawn->base + sideEntry->offset;
    sideEntry->offset = sideEntry->offset / 10;
    sideEntry->extent = fixedExtent / 1000;
    sideEntry->extentTarget = sideEntry->extent;
    sideEntry->extentStep = sideEntry->extentTarget / 8;
    sideEntry->extentTarget <<= 1;
    sideEntry->extentSaved = sideEntry->extentTarget;
    sideEntry->extent = sideEntry->extent / 10;
    sideEntry = (BattleFxMagicStopEntry *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicStopSideModelTask);
    sideEntry->worldPos = pSpawn->worldPos;
    sideEntry->scale = 0;
    sideEntry->yaw = pSpawn->yaw;
    sideEntry->offset = extent * 200 / 1000;
    sideEntry->position = pSpawn->base - sideEntry->offset;
    sideEntry->offset = sideEntry->offset / 10;
    sideEntry->extentTarget = 0;
    sideEntry->extent = fixedExtent / 1120;
    sideEntry->extentStep = sideEntry->extent;
    sideEntry->extentStep = sideEntry->extentStep / 8;
    sideEntry->extent = sideEntry->extent / 10;
    sideEntry = (BattleFxMagicStopEntry *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicStopAngleGatedModelTask);
    sideEntry->worldPos = pSpawn->worldPos;
    sideEntry->scale = 0x800 /* 0.5 */;
    sideEntry->yaw = pSpawn->yaw;
    sideEntry->offset = extent * 264 / 1000;
    sideEntry->position = pSpawn->base - sideEntry->offset;
    sideEntry->offset = sideEntry->offset / 10;
    sideEntry->extent = fixedExtent / 1000;
    sideEntry->extentTarget = sideEntry->extent;
    sideEntry->extentStep = sideEntry->extentTarget / 8;
    sideEntry->extentTarget <<= 1;
    sideEntry->extentSaved = sideEntry->extentTarget;
    sideEntry->extent = sideEntry->extent / 10;
}

/* Animates central rotating clock hand model for Stop. */
// FUNCTION: FF7 0x00580d0c
void __cdecl BattleFx_MagicStopCentralModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[116];
        SVECTOR rotX;
        struct { unsigned char pad_to_8[0x8]; BattleFxStopCentralRecord *centralRecord; };
        struct { unsigned char pad_to_C[0xC]; SVECTOR offX; };
        struct { unsigned char pad_to_18[0x18]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_38[0x38]; PsxMatrix matrix; };
        struct { unsigned char pad_to_58[0x58]; SVECTOR secondRotX; };
        struct { unsigned char pad_to_60[0x60]; BattleFxModelDraw *drawModel; };
        struct { unsigned char pad_to_64[0x64]; VECTOR scale; };
    } frame;
#pragma pack(pop)

    frame.centralRecord = (BattleFxStopCentralRecord *)BATTLE_CURRENT_TASK_SLOT;
    frame.rotX.vx = 0;
    frame.rotX.vy = frame.centralRecord->yaw;
    frame.rotX.vz = frame.centralRecord->angle;
    RotMatrixXYZ(&frame.rotX, &frame.matrix);
    frame.secondRotX.vx = 0;
    frame.secondRotX.vy = frame.centralRecord->yaw;
    frame.secondRotX.vz = 0;
    RotMatrixXYZ(&frame.secondRotX, &frame.rotMatrix);
    frame.offX.vx = 0;
    frame.offX.vy = 0;
    frame.offX.vz = -frame.centralRecord->offsetDistance;
    ApplyMatrixToSVectorInt32(&frame.rotMatrix, &frame.offX, &frame.matrix.t[0]);
    frame.matrix.t[0] += frame.centralRecord->posX;
    frame.matrix.t[1] += frame.centralRecord->posY;
    frame.matrix.t[2] += frame.centralRecord->posZ;
    frame.scale.vz = frame.centralRecord->ramp;
    frame.scale.vy = frame.scale.vz;
    frame.scale.vx = frame.scale.vy;
    ScaleMatrix(&frame.matrix, &frame.scale);
    MultiplyMatrix(&g_BattleCameraTransform, &frame.matrix, &frame.matrix);
    SetRotMatrix(&frame.matrix);
    SetTransMatrix(&frame.matrix);
    frame.drawModel = (BattleFxModelDraw *)&g_BattleMagicStopModelReq;
    frame.drawModel->modelGroup = Gfx_ModelTableGetRenderObject(1, g_BattleMagicStopLoadedResTable);
    frame.drawModel->flags = 0x80;
    frame.drawModel->uvScroll = 0;
    frame.drawModel->fadeLevel = 0;
    frame.drawModel->stateFlags = 0;
    frame.drawModel->texturePageBits = 0;
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitModelDraw(frame.drawModel, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (frame.centralRecord->timer >= 35) {
        frame.centralRecord->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.centralRecord->timer == 25) {
        Battle_PostActorHitOrHurtMessage(frame.centralRecord->allocType);
    }
    if (frame.centralRecord->timer < 4) {
        frame.centralRecord->ramp += frame.centralRecord->rampAccel;
    }
    if (frame.centralRecord->timer >= 25) {
        frame.centralRecord->angle += 0x80;
        frame.centralRecord->ramp -= frame.centralRecord->rampDrag;
    }
    (frame.centralRecord->timer)++;
}

/* Animates side rotating clock hand model for Stop. */
// FUNCTION: FF7 0x00580f3e
void __cdecl BattleFx_MagicStopSideModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[120];
        SVECTOR rotX;
        struct { unsigned char pad_to_8[0x8]; BattleFxStopSideModel *sideRecord; };
        struct { unsigned char pad_to_C[0xC]; SVECTOR offX; };
        struct { unsigned char pad_to_18[0x18]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_38[0x38]; PsxMatrix matrix; };
        struct { unsigned char pad_to_5C[0x5C]; SVECTOR secondRotX; };
        struct { unsigned char pad_to_64[0x64]; BattleFxModelDraw *drawModel; };
        struct { unsigned char pad_to_68[0x68]; VECTOR scale; };
    } frame;
#pragma pack(pop)

    frame.sideRecord = (BattleFxStopSideModel *)BATTLE_CURRENT_TASK_SLOT;
    if (frame.sideRecord->timer < 3) {
        if (g_BattleIsPaused == 0) {
            (frame.sideRecord->timer)++;
        }
    } else {
        frame.rotX.vx = 0;
        frame.rotX.vy = frame.sideRecord->yaw;
        frame.rotX.vz = frame.sideRecord->spinAngle;
        RotMatrixXYZ(&frame.rotX, &frame.matrix);
        frame.secondRotX.vx = 0;
        frame.secondRotX.vy = frame.sideRecord->yaw;
        frame.secondRotX.vz = 0;
        RotMatrixXYZ(&frame.secondRotX, &frame.rotMatrix);
        frame.offX.vx = 0;
        frame.offX.vy = 0;
        frame.offX.vz = -frame.sideRecord->offsetDistance;
        ApplyMatrixToSVectorInt32(&frame.rotMatrix, &frame.offX, &frame.matrix.t[0]);
        frame.matrix.t[0] += frame.sideRecord->posX;
        frame.matrix.t[1] += frame.sideRecord->posY;
        frame.matrix.t[2] += frame.sideRecord->posZ;
        frame.scale.vz = frame.sideRecord->ramp;
        frame.scale.vy = frame.scale.vz;
        frame.scale.vx = frame.scale.vy;
        ScaleMatrix(&frame.matrix, &frame.scale);
        MultiplyMatrix(&g_BattleCameraTransform, &frame.matrix, &frame.matrix);
        SetRotMatrix(&frame.matrix);
        SetTransMatrix(&frame.matrix);
        frame.drawModel = (BattleFxModelDraw *)&g_BattleMagicStopModelReq;
        frame.drawModel->modelGroup = Gfx_ModelTableGetRenderObject(2, g_BattleMagicStopLoadedResTable);
        frame.drawModel->flags = 0x28;
        frame.drawModel->uvScroll = 0;
        frame.drawModel->fadeLevel = 128;
        frame.drawModel->stateFlags = 0x20;
        frame.drawModel->texturePageBits = 0;
        g_BattleFxActiveDrawListPtr = BattleFx_SubmitModelDraw(frame.drawModel, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
        if (frame.sideRecord->timer >= 35) {
            frame.sideRecord->ownerSlot = BATTLE_TASK_FREE;
        }
        if (g_BattleIsPaused != 0) {
            return;
        }
        if (frame.sideRecord->timer < 11) {
            frame.sideRecord->ramp += frame.sideRecord->rampAccel;
        }
        if (frame.sideRecord->timer >= 25) {
            frame.sideRecord->spinAngle += 0x80;
            frame.sideRecord->ramp -= frame.sideRecord->rampDrag;
            frame.sideRecord->offsetDistance -= frame.sideRecord->offsetDistanceStep;
        }
        (frame.sideRecord->timer)++;
    }
}

/* Animates angle-gated clock indicator models for Stop. */
// FUNCTION: FF7 0x00581195
void __cdecl BattleFx_MagicStopAngleGatedModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[132];
        int clampedValue;
        struct { unsigned char pad_to_4[0x4]; SVECTOR rotX; };
        struct { unsigned char pad_to_C[0xC]; BattleFxAngleGatedModel *gatedRecord; };
        struct { unsigned char pad_to_10[0x10]; SVECTOR offX; };
        struct { unsigned char pad_to_1C[0x1C]; int tmpValue; };
        struct { unsigned char pad_to_20[0x20]; int angleIndex; };
        struct { unsigned char pad_to_24[0x24]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_44[0x44]; PsxMatrix matrix; };
        struct { unsigned char pad_to_68[0x68]; SVECTOR secondRotX; };
        struct { unsigned char pad_to_70[0x70]; BattleFxModelDraw *drawModel; };
        struct { unsigned char pad_to_74[0x74]; VECTOR scale; };
    } frame;
#pragma pack(pop)

    frame.gatedRecord = (BattleFxAngleGatedModel *)BATTLE_CURRENT_TASK_SLOT;
    if (frame.gatedRecord->phase < 15) {
        if (g_BattleIsPaused == 0) {
            (frame.gatedRecord->phase)++;
        }
    } else {
        frame.rotX.vx = 0;
        frame.rotX.vy = frame.gatedRecord->yaw;
        frame.rotX.vz = frame.gatedRecord->spinAngle;
        RotMatrixXYZ(&frame.rotX, &frame.matrix);
        frame.secondRotX.vx = 0;
        frame.secondRotX.vy = frame.gatedRecord->yaw;
        frame.secondRotX.vz = 0;
        RotMatrixXYZ(&frame.secondRotX, &frame.rotMatrix);
        frame.offX.vx = 0;
        frame.offX.vy = 0;
        frame.offX.vz = -frame.gatedRecord->sideDistance;
        ApplyMatrixToSVectorInt32(&frame.rotMatrix, &frame.offX, &frame.matrix.t[0]);
        frame.matrix.t[0] += frame.gatedRecord->posX;
        frame.matrix.t[1] += frame.gatedRecord->posY;
        frame.matrix.t[2] += frame.gatedRecord->posZ;
        frame.scale.vz = frame.gatedRecord->scale;
        frame.scale.vy = frame.scale.vz;
        frame.scale.vx = frame.scale.vy;
        ScaleMatrix(&frame.matrix, &frame.scale);
        MultiplyMatrix(&g_BattleCameraTransform, &frame.matrix, &frame.rotMatrix);
        SetRotMatrix(&frame.rotMatrix);
        SetTransMatrix(&frame.rotMatrix);
        frame.drawModel = (BattleFxModelDraw *)&g_BattleMagicStopModelReq;
        frame.drawModel->modelGroup = Gfx_ModelTableGetRenderObject(0, g_BattleMagicStopLoadedResTable);
        frame.drawModel->flags = 0xA0;
        frame.drawModel->uvScroll = 0;
        frame.drawModel->fadeLevel = 0;
        frame.drawModel->stateFlags = 0;
        frame.drawModel->texturePageBits = 0;
        g_BattleFxActiveDrawListPtr = BattleFx_SubmitModelDraw(frame.drawModel, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
        frame.drawModel->flags = 0xA8;
        for (frame.angleIndex = 0; frame.angleIndex < 4; frame.angleIndex++) {
            frame.tmpValue = g_BattleMagicStopAngles[frame.angleIndex] + (frame.gatedRecord->phase - 0xF << 8);
            if (frame.tmpValue < 2048 || frame.tmpValue >= 4096) {
                continue;
            }
            frame.rotX.vz = *(short *)&frame.tmpValue;
            RotMatrixXYZ(&frame.rotX, &frame.matrix);
            frame.tmpValue = frame.scale.vx + frame.gatedRecord->scaleStep;
            if (frame.tmpValue > frame.gatedRecord->clampLimit) {
                frame.clampedValue = frame.gatedRecord->clampLimit;
            } else {
                frame.clampedValue = frame.tmpValue;
            }
            frame.tmpValue = frame.clampedValue;
            frame.scale.vz = frame.tmpValue;
            frame.scale.vy = frame.scale.vz;
            frame.scale.vx = frame.scale.vy;
            ScaleMatrix(&frame.matrix, &frame.scale);
            MultiplyMatrix(&g_BattleCameraTransform, &frame.matrix, &frame.rotMatrix);
            SetRotMatrix(&frame.rotMatrix);
            SetTransMatrix(&frame.rotMatrix);
            SetDrawColorRGB(0, 0, 0);
            frame.drawModel->fadeLevel = g_BattleMagicStopSubAngles[frame.angleIndex];
            g_BattleFxActiveDrawListPtr = BattleFx_SubmitModelDraw(frame.drawModel, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
        }
        if (frame.gatedRecord->phase >= 35) {
            frame.gatedRecord->ownerSlot = BATTLE_TASK_FREE;
        }
        if (g_BattleIsPaused != 0) {
            return;
        }
        if (frame.gatedRecord->phase < 23) {
            frame.gatedRecord->spinAngle += 0x100 /* 22.5 deg */;
            frame.gatedRecord->scale -= frame.gatedRecord->scaleStep;
        }
        if (frame.gatedRecord->phase >= 25) {
            frame.gatedRecord->spinAngle += 0x80;
            frame.gatedRecord->scale -= frame.gatedRecord->lateScaleStep;
            frame.gatedRecord->sideDistance -= frame.gatedRecord->sideDistanceStep;
        }
        (frame.gatedRecord->phase)++;
    }
}
} /* extern "C" */
