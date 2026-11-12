// src/ff7/src/battle/battle_fx_magic_remove.cpp
/* Magic Effect [26]: Remove (Japanese: デジョン Dejon, dev tag: "MAGIC/DEJON" / Dejon) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle visual effect for the Dejon magic: a black hole vortex built from rotating
//   sprite layers, inward arc sprites and a scaled model, which pulls the target actor's model in.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/enums/sound.h"
#include "ff7/battle_tasks.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_magic.h"
#include "lib/types/graphics.h"
#include <stdlib.h>
#include "ff7/api/battle.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "ff7/camera_script.h"
#include <string.h>
#include "lib/codegen.h"
extern "C" {

struct BattleFxModelDraw;

#pragma intrinsic(abs)

#pragma intrinsic(memset)
#pragma intrinsic(memcpy)

// clang-format off
// GLOBAL: FF7 0x008f30c8
int g_BattleMagicRemove_ResourceDefs = 0x00192A39;
int g_BattleMagicRemove_ResourceDefsHeadPtr[1] = {
    (int)((char *)&g_BattleFxMagicRemoveResourceNames + 0x0)
};
int g_BattleMagicRemove_ResourceDefsDataTail[18] = {
    (int)((char *)&g_BattleFxMagicRemoveResourceNames + 0xc), 0x0, 0x0, 0x196a3c, (int)((char *)&g_BattleFxMagicRemoveResourceNames + 0x18), (int)((char *)&g_BattleFxMagicRemoveResourceNames + 0x24),
    0x0, 0x0, 0x6400b1, (int)((char *)&g_BattleFxMagicRemoveResourceNames + 0x30), 0x0, 0x0,
    0x0, 0x192a39, (int)((char *)&g_BattleFxMagicRemoveResourceNames + 0x3c), (int)((char *)&g_BattleFxMagicRemoveResourceNames + 0x48), 0x0, 0x0
};
// clang-format on

// clang-format off
// GLOBAL: FF7 0x008f3118
struct BattleMagicRemove_SpriteReq g_BattleMagicRemove_SpriteReq = {0, 0, {0x80, 0x80, 0x80, 0x2c, 0, 0, 0, 0}, 0, 0};
unsigned char g_rawData_008f3130[8] = {0x80, 0x80, 0x80, 0x2c, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x008f3138
struct BattleMagicRemove_QuadColors g_BattleMagicRemove_QuadColors = {0x1000, {0}, -0x1000, {0}, 0x1000, {0}, 0, 0, 0, {0}};
unsigned char g_rawData_008f3158[16] = {0x80, 0x00, 0x00, 0x00, 0xe0, 0x01, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x008f3168
unsigned char g_BattleMagicRemove_QuadReq[8] = {0xf0, 0xff, 0xf0, 0xff, 0x80, 0x00, 0x1f, 0x1f};
// GLOBAL: FF7 0x008f3170
struct BattleMagicRemove_SpriteData g_BattleMagicRemove_SpriteData = {-128, -128, -128, {0x2e, 0x2d, 0, 0x88, 0x78}, 0, {0}};
// GLOBAL: FF7 0x008f3180
int g_BattleMagicRemove_ScaleZ = 4096;
unsigned char g_rawData_008f3184[4] = {0x00, 0x10, 0x00, 0x00};
// GLOBAL: FF7 0x008f3188
int g_BattleMagicRemove_ScaleXY = 4096;
// GLOBAL: FF7 0x008f3190
struct BattleMagicRemove_ModelReq g_BattleMagicRemove_ModelReq = {0, {8, 0, 0, 0, 0, 0}, 128, {0x20, 0, 0, 0}};
unsigned char g_BattleFxMagicRemoveResourceNames[84] =
    "DEJON01.TIM\0"
    "DEJON_2.SPT\0"
    "DEJON00.TIM\0"
    "DEJON_1.SPT\0"
    "DEJON_2.RSD\0"
    "DEJON01.TIM\0"
    "DEJON_1.SPT";
// GLOBAL: FF7 0x00bd99f8
int g_BattleMagicRemove_DrawListHandle = 0;
// GLOBAL: FF7 0x00bd99fc
int g_BattleMagicRemove_Step = 0;
// The palette rows of the vortex layers (read by BattleFx_MagicBeginRemove). The span runs to the
// next named definition of the TU; the TU's pieces inside it are aliases.
// GLOBAL: FF7 0x00bd9a00
BattleMagicRemove_QuadRecord g_BattleMagicRemove_QuadRecords[15] = {0};
// GLOBAL: FF7 0x00bd9dc0
BattleMagicRemove_TargetPosition g_BattleMagicRemove_TargetPosition = {0};
// GLOBAL: FF7 0x00bd9dc8
struct BattleMagicRemove_LoadedRes g_BattleMagicRemove_LoadedRes = {0};
unsigned char g_bssPad_00bd9dd4[4] = {0};
// clang-format on


/*
 * Loads resources for the Dejon magic effect when not already loaded.
 */
// FUNCTION: FF7 0x005ac3a0
void __cdecl BattleFx_MagicLoadResRemove(void)
{
    GfxModelTableBank *context;
    GfxResourceRequest requestBuf;
    int resultType;
    char pathBuffer[200];
    GfxResourceRequest *pRequest;
    int dejonTable;

    if (g_pBattle3dScene != 0 && (g_BattleMagicRemove_LoadedRes.tableCursor) == 0) {
        resultType = 4;
        dejonTable = (int)&g_BattleMagicRemove_ResourceDefs;
        context = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRequest = &requestBuf;
        Gfx_InitResourceRequest(pRequest);
        pRequest->groupTag = 0;
        // STRING: FF7 0x008f31f4
        pRequest->resourcePath = BattleFx_BuildResourcePath("MAGIC/DEJON", pathBuffer);
        Gfx_LoadModelTableIfAbsent(resultType, dejonTable, pRequest, &g_BattleMagicRemove_LoadedRes, context);
    }
}

/*
 * Main entry point initializing render state for Dejon.
 */
// FUNCTION: FF7 0x005ac430
void __cdecl BattleFx_MagicEnterRemove(int targetMask, int actorIdx)
{
    BattleFx_MagicLoadResRemove();
    BattleFx_MagicBeginRemove(targetMask, actorIdx);
}

/*
 * Spawns timeline task for a single Dejon target.
 */
// FUNCTION: FF7 0x005ac44a
void __cdecl BattleFx_MagicBeginRemove(int targetIndex, int actorIdx)
{
    int rowTint;
    int value;
    int row;
    int column;
    int table;
    BattleFxRemoveRecord *readPointer;

    g_BattleMagicRemove_Step = targetIndex;
    Battle_GetActorMaskCentroid(targetIndex, &g_BattleMagicRemove_TargetPosition.targetX);
    table = Gfx_ModelTableGetSpriteObject(1, g_BattleMagicRemove_LoadedRes.tableCursor);
    readPointer = (BattleFxRemoveRecord *)(*(int *)((char *)*(int *)((char *)*(int *)((char *)*(int *)((char *)table + 0xC) + 0x14) + 0x8C) + 0x18));
    rowTint = readPointer->rowTint;
    readPointer++;
    for (row = 0; row < 0xE; row++) {
        value = readPointer->value;
        readPointer = (BattleFxRemoveRecord *)((char *)readPointer + 4);
        g_BattleMagicRemove_QuadRecords[row].unused0 = 0;
        g_BattleMagicRemove_QuadRecords[row].tintColor = rowTint;
        for (column = 0; column < 0xE; column++) {
            g_BattleMagicRemove_QuadRecords[column].cycleColor[(column + row) % 0xE] = value;
        }
    }
    Battle_AllocateResourceSlot(BattleFx_MagicRemoveTimelineTask);
    Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, 0x40, 0x4F);
}

/*
 * Drives the main timeline and black hole effect for Dejon.
 */
// FUNCTION: FF7 0x005ac547
void __cdecl BattleFx_MagicRemoveTimelineTask(void)
{
    BattleTaskSlot *taskPtr;
    BattleFxRemoveParticle *particle;
    int i;

    taskPtr = BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (taskPtr->frameCounter == 0) {
        particle = (BattleFxRemoveParticle *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicRemoveTwinSpriteLayersTask);
        particle->position = *(Int64Pair *)&g_BattleMagicRemove_TargetPosition;
    }
    if (taskPtr->frameCounter >= 0 && taskPtr->frameCounter < 0x1E) {
        for (i = 0; i < 3; i++) {
            particle = (BattleFxRemoveParticle *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicRemoveInwardArcSpriteTask);
            particle->posX = g_BattleMagicRemove_TargetPosition.targetX + rand() % 0x2710 + -0x1388;
            particle->posY = -rand() % 0x1388;
            particle->posZ = g_BattleMagicRemove_TargetPosition.targetZ + rand() % 0x2710 + -0x1388;
            particle->velocityX = (g_BattleMagicRemove_TargetPosition.targetX - particle->posX) / 0x1B;
            particle->velocityY = -particle->posY / 0x1B;
            particle->velocityZ = (g_BattleMagicRemove_TargetPosition.targetZ - particle->posZ) / 0x1B;
            particle->scale = rand() % 0xD00 + 0x500;
        }
    }
    if (taskPtr->frameCounter == 2) {
        particle = (BattleFxRemoveParticle *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicRemoveScaledModelTask);
        particle->position = *(Int64Pair *)&g_BattleMagicRemove_TargetPosition;
        particle->posY = 0;
    }
    if (taskPtr->frameCounter == 0x14) {
        for (i = 0; i < 0xA; i++) {
            if ((g_BattleMagicRemove_Step & 1 << i) != 0 && (g_BattleTimelineTargetMask & 1 << i) != 0) {
                particle = (BattleFxRemoveParticle *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicRemovePullActorTask);
                particle->position = *(Int64Pair *)&g_BattleActorRecords[i].posX;
                particle->scale = g_BattleActorRecords[i].scale;
                particle->index = i;
                particle->velocityX = (g_BattleMagicRemove_TargetPosition.targetX - particle->posX) / 0x1E;
                particle->velocityY = (g_BattleMagicRemove_TargetPosition.targetY - particle->posY) / 0x1E;
                particle->velocityZ = (g_BattleMagicRemove_TargetPosition.targetZ - particle->posZ) / 0x1E;
            }
        }
    }
    if (taskPtr->frameCounter == 0x1E) {
        taskPtr->ownerSlot = BATTLE_TASK_FREE;
    }
    (taskPtr->frameCounter)++;
}

/*
 * Updates and renders dual rotating sprite vortex layers for Dejon.
 */
// FUNCTION: FF7 0x005ac85e
void __cdecl BattleFx_MagicRemoveTwinSpriteLayersTask(void)
{
    int angle;
    int localFrame;
    BattleFxRemoveLayerSprite *task;
    int matrix[8];

    task = (BattleFxRemoveLayerSprite *)BATTLE_CURRENT_TASK_SLOT;
    (g_BattleMagicRemove_SpriteReq.spriteGroup) = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(0, g_BattleMagicRemove_LoadedRes.tableCursor);
    g_BattleMagicRemove_SpriteReq.drawObj = Gfx_ModelTableGetSpriteObject(0, g_BattleMagicRemove_LoadedRes.tableCursor);
    g_BattleMagicRemove_SpriteReq.nextSpriteGroup = Gfx_ModelTableGetLoadedRecord(1, g_BattleMagicRemove_LoadedRes.tableCursor);
    g_BattleMagicRemove_SpriteReq.paletteObj = Gfx_ModelTableGetSpriteObject(1, g_BattleMagicRemove_LoadedRes.tableCursor);
    localFrame = task->layerIndex;
    if (localFrame < 0x14) {
        angle = localFrame * 0x2500 / 0x14;
    } else {
        localFrame -= 0x14;
        if (localFrame < 0x28) {
            angle = 0x2500;
        } else {
            localFrame -= 0x28;
            if (localFrame < 0xA) {
                angle = localFrame * -0x2500 / 0xA + 0x2500;
            } else {
                task->ownerSlot = BATTLE_TASK_FREE;
                return;
            }
        }
    }
    g_BattleMagicRemove_QuadColors.scaleZ = angle;
    (g_BattleMagicRemove_QuadColors.scaleX) = g_BattleMagicRemove_QuadColors.scaleZ;
    g_BattleMagicRemove_QuadColors.scaleY = -angle;
    g_BattleMagicRemove_QuadColors.transX = task->posX;
    g_BattleMagicRemove_QuadColors.transY = 0;
    g_BattleMagicRemove_QuadColors.transZ = task->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, (short *)&g_BattleMagicRemove_QuadColors, matrix);
    SetRotMatrix(matrix);
    SetTransMatrix(matrix);
    g_BattleMagicRemove_DrawListHandle = BattleFx_SubmitSpriteDraw(&g_BattleMagicRemove_SpriteReq.nextSpriteGroup, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleMagicRemove_DrawListHandle);
    g_BattleMagicRemove_DrawListHandle = BattleFx_SubmitSpriteDraw(&(g_BattleMagicRemove_SpriteReq.spriteGroup), g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleMagicRemove_DrawListHandle);
    Gfx_UploadRenderObjectPalette(0, 0x10, (int)&g_BattleMagicRemove_QuadRecords[task->layerIndex % 0xE], 0, g_BattleMagicRemove_SpriteReq.paletteObj);
    if (g_BattleIsPaused == 0) {
        (task->layerIndex)++;
    }
}

/*
 * Updates and renders spiraling inward arc sprites for Dejon.
 */
// FUNCTION: FF7 0x005aca50
void __cdecl BattleFx_MagicRemoveInwardArcSpriteTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[16];
        BattleFxRemoveArcRecord *arcRecord;
        struct { unsigned char pad_to_4[0x4]; SVECTOR posX; };
        struct { unsigned char pad_to_C[0xC]; int phase; };
    } frame;
#pragma pack(pop)

    frame.arcRecord = (BattleFxRemoveArcRecord *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleMagicRemove_SpriteData.drawObj = Gfx_ModelTableGetSpriteObject(3, g_BattleMagicRemove_LoadedRes.tableCursor);
    frame.phase = frame.arcRecord->step;
    if (frame.phase < 2) {
        g_BattleMagicRemove_SpriteData.colorB = ((frame.phase << 7) / 2);
        g_BattleMagicRemove_SpriteData.colorG = g_BattleMagicRemove_SpriteData.colorB;
        (g_BattleMagicRemove_SpriteData.colorR) = g_BattleMagicRemove_SpriteData.colorG;
    } else {
        frame.phase -= 2;
        if (frame.phase < 0x19) {
            g_BattleMagicRemove_SpriteData.colorB = 0x80;
            g_BattleMagicRemove_SpriteData.colorG = g_BattleMagicRemove_SpriteData.colorB;
            (g_BattleMagicRemove_SpriteData.colorR) = g_BattleMagicRemove_SpriteData.colorG;
        } else {
            frame.arcRecord->ownerSlot = BATTLE_TASK_FREE;
            return;
        }
    }
    frame.posX.vx = frame.arcRecord->posX;
    frame.posX.vy = frame.arcRecord->posY - (SinFixed((frame.arcRecord->step << 0xB) / 0x1B) * 0x3E8 >> 0xC);
    frame.posX.vz = frame.arcRecord->posZ;
    BattleFx_InstallBillboardMatrix(&frame.posX, (short)frame.arcRecord->angle, 0);
    BattleFx_SubmitSpriteQuadDraw((BattleFxSpriteQuadRequest *)&g_BattleMagicRemove_QuadReq, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, 0);
    if (g_BattleIsPaused == 0) {
        (frame.arcRecord->step)++;
        frame.arcRecord->posX += frame.arcRecord->velocityX;
        frame.arcRecord->posY += frame.arcRecord->velocityY;
        frame.arcRecord->posZ += frame.arcRecord->velocityZ;
    }
}

/*
 * Renders and scales the vortex model geometry for Dejon.
 */
// FUNCTION: FF7 0x005acbd0
void __cdecl BattleFx_MagicRemoveScaledModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[72];
        PsxMatrix rotMatrix;
        struct { unsigned char pad_to_20[0x20]; BattleFreezeGrowthRecord *scaledModel; };
        struct { unsigned char pad_to_24[0x24]; PsxMatrix transformMatrix; };
        struct { unsigned char pad_to_44[0x44]; int phase; };
    } frame;
#pragma pack(pop)

    frame.scaledModel = (BattleFreezeGrowthRecord *)BATTLE_CURRENT_TASK_SLOT;
    (g_BattleMagicRemove_ModelReq.modelEntry) = Gfx_ModelTableGetRenderObject(2, g_BattleMagicRemove_LoadedRes.tableCursor);
    frame.phase = frame.scaledModel->frameIndex;
    if (frame.phase < 0x14) {
        g_BattleMagicRemove_ScaleXY = frame.phase * 0x3000 / 0x14;
        g_BattleMagicRemove_ScaleZ = g_BattleMagicRemove_ScaleXY;
        g_BattleMagicRemove_ModelReq.alpha = 0x80;
    } else {
        frame.phase -= 0x14;
        if (frame.phase < 0x1E) {
            g_BattleMagicRemove_ScaleXY = 0x3000;
            g_BattleMagicRemove_ScaleZ = g_BattleMagicRemove_ScaleXY;
            g_BattleMagicRemove_ModelReq.alpha = 0x80;
        } else {
            frame.phase -= 0x1E;
            if (frame.phase < 0xA) {
                g_BattleMagicRemove_ScaleXY = 0x3000;
                g_BattleMagicRemove_ScaleZ = g_BattleMagicRemove_ScaleXY;
                g_BattleMagicRemove_ModelReq.alpha = frame.phase * -0x80 / 0xA + 0x80;
            } else {
                frame.scaledModel->ownerSlot = BATTLE_TASK_FREE;
                return;
            }
        }
    }
    g_BattleMagicRemove_LoadedRes.alpha = frame.scaledModel->frameIndex << 5;
    RotMatrix((SVECTOR *)&g_BattleMagicRemove_LoadedRes.rotByte8, &frame.rotMatrix);
    ScaleMatrix(&frame.rotMatrix, &g_BattleMagicRemove_ScaleZ);
    frame.rotMatrix.t[0] = frame.scaledModel->posX;
    frame.rotMatrix.t[1] = 0;
    frame.rotMatrix.t[2] = frame.scaledModel->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.transformMatrix);
    SetRotMatrix(&frame.transformMatrix);
    SetTransMatrix(&frame.transformMatrix);
    g_BattleMagicRemove_DrawListHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleMagicRemove_ModelReq, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleMagicRemove_DrawListHandle);
    if (g_BattleIsPaused == 0) {
        (frame.scaledModel->frameIndex)++;
    }
}

/*
 * Animates and shrinks target actor model into the vortex for Dejon.
 */
// FUNCTION: FF7 0x005acd77
void __cdecl BattleFx_MagicRemovePullActorTask(void)
{
    int localStep;
    BattleFxRemovePullState *task;

    task = (BattleFxRemovePullState *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    localStep = task->step;
    if (localStep < 0x1E) {
        g_BattleActorRecords[task->slotIndex].scale = task->startValue + (0 - task->startValue) * localStep / 0x1E;
    } else {
        *(Int64Pair *)&g_BattleActorRecords[task->slotIndex].posX = task->position;
        g_BattleActorRecords[task->slotIndex].scale = task->startValue;
        Battle_RestoreActorAfterEffect(*(char *)&task->slotIndex);
        task->ownerSlot = BATTLE_TASK_FREE;
        return;
    }
    g_BattleActorRecords[task->slotIndex].posX += task->velocityX;
    g_BattleActorRecords[task->slotIndex].posY += task->velocityY;
    g_BattleActorRecords[task->slotIndex].posZ += task->velocityZ;
    (task->step)++;
}
} /* extern "C" */
