// src/ff7/src/battle/battle_fx_special_sandstorm.cpp
/* Enemy Special Effect [idx 12]: Sandstorm (JP: suna / 砂 (Sand), asset: "SPECIAL/SUNA") — Sandstorm (0x1f1, Land Worm / Sea Worm) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle special effect for the Suna attack: loads resources, sets the target mask, and
//   drives a swirling sand particle field, cloud variant sprites and a spinning sandstorm model. It
//   also holds an unused placeholder callback.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/api/battle.h"
#include "ff7/battle_tasks.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_special.h"
#include "ff7/camera_script.h"
#include "lib/types/graphics.h"
#include "lib/codegen.h"
#include <string.h>
#include <stdlib.h>
extern "C" {

struct BattleFxSandstormParticleRecord;
struct BattleFxModelDraw;
// clang-format off
// GLOBAL: FF7 0x008a81b0
int g_BattleSpecialSandstormResourceDefs = 0x001900B1;
char *g_BattleFxSpecialSandstormResourceNames[1] = {
    // STRING: FF7 0x008a8248
    "SUNA2.RSD"
};
unsigned int g_BattleSpecialSandstormResourceDefsDataTail[24] = {
    0x00000000,
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x008a8254
    (unsigned int)"HIT1.TIM",
    // STRING: FF7 0x008a8260
    (unsigned int)"HIT1.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x008a826c
    (unsigned int)"HIT1.TIM",
    // STRING: FF7 0x008a8278
    (unsigned int)"HIT2.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x008a8284
    (unsigned int)"HIT1.TIM",
    // STRING: FF7 0x008a8290
    (unsigned int)"HIT3.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x008a829c
    (unsigned int)"HIT1.TIM",
    // STRING: FF7 0x008a82a8
    (unsigned int)"HIT4.SPT",
    0x00000000,
    0x00000000,
    0x00000000
};
#undef g_BattleSpecialSandstormSpriteData
// GLOBAL: FF7 0x008a8218
struct BattleSpecialSandstormSpriteData g_BattleSpecialSandstormSpriteData = {0, 0, {0x80, 0x80, 0x80, 0x2c}, 0, {0}};
#undef g_BattleSpecialSandstormConfig
// GLOBAL: FF7 0x008a8228
struct BattleSpecialSandstormConfig g_BattleSpecialSandstormConfig = {{1, 0, 0, 0, 2, 0, 0, 0, 3, 0, 0, 0, 4, 0, 0, 0}, 0, {8, 0, 0, 0, 0, 0}, 128};
unsigned char g_rawData_008a8244[4] = {0x20, 0x00, 0x00, 0x00};
// clang-format on
#pragma intrinsic(memset)
#pragma intrinsic(memcpy)


/* The 0xC8 records of the sand particle field. */
#define g_BattleSandstormSprite (&g_BattleSpecialSandstormSpriteData)
#define g_BattleSandstormConfig (&g_BattleSpecialSandstormConfig)

// clang-format off
// GLOBAL: FF7 0x00b72840
int g_BattleSpecialSandstormDrawListHandle = 0;
// GLOBAL: FF7 0x00b72844
short g_BattleSpecialSandstormTargetMask = 0;
// GLOBAL: FF7 0x00b72848
int g_BattleSpecialSandstormScaleVector = 0;
// GLOBAL: FF7 0x00b7284c
int g_BattleSpecialSandstormScaleFactor = 0;
// GLOBAL: FF7 0x00b72850
int g_BattleSpecialSandstormScale = 0;
unsigned char g_bssPad_00b72854[4] = {0};
// GLOBAL: FF7 0x00b72858
short g_BattleSpecialSandstormStep = 0;
unsigned char g_bssPad_00b7285c[4] = {0};
// GLOBAL: FF7 0x00b72860
struct BattleSpecialSandstormParticleRecord g_BattleSpecialSandstormParticleRecords[2] = {0};
unsigned char g_bssPad_00b72870[1584] = {0};
// GLOBAL: FF7 0x00b72ea0
struct GfxTableCursor *g_BattleSpecialSandstormLoadedRes = 0;
// clang-format on

/*
 * Loads resources for the Suna special effect when not already loaded.
 */
// FUNCTION: FF7 0x0055a710
void __cdecl BattleFx_SpecialLoadResSandstorm(void)
{
    GfxModelTableBank *value;
    GfxResourceRequest reqBuffer;
    int kind;
    char pathWorkBuffer[200];
    GfxResourceRequest *pRequest;
    int resourceDefsWord;

    if (g_pBattle3dScene != 0 && g_BattleSpecialSandstormLoadedRes == 0) {
        kind = 5;
        resourceDefsWord = (int)&g_BattleSpecialSandstormResourceDefs;
        value = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRequest = &reqBuffer;
        Gfx_InitResourceRequest(pRequest);
        pRequest->groupTag = 0;
        // STRING: FF7 0x008a82b4
        pRequest->resourcePath = (char *)BattleFx_BuildResourcePath("SPECIAL/SUNA", pathWorkBuffer);
        Gfx_LoadModelTableIfAbsent(kind, resourceDefsWord, pRequest, &g_BattleSpecialSandstormLoadedRes, value);
    }
}

/*
 * Unused placeholder callback for Suna.
 */
// FUNCTION: FF7 0x0055a7a0
void __cdecl BattleFx_SpecialSandstormNoOp(void)
{
}

/*
 * Initializes tasks and sets target bitmask for Suna.
 */
// FUNCTION: FF7 0x0055a7a5
void __cdecl BattleFx_SpecialEnterSandstorm(int targetMask, int actorIdx)
{
    short i;
    BattleFxSandstormRecord *taskPointer;

    BattleFx_SpecialLoadResSandstorm();
    g_BattleSpecialSandstormDrawListHandle = (int)&g_GfxPsxPacketScratch;
    g_BattleSpecialSandstormStep = 0;
    g_BattleSpecialSandstormScaleVector = 0x1A00;
    g_BattleSpecialSandstormScaleFactor = 0x1000;
    g_BattleSpecialSandstormScale = 0x1A00;
    for (i = 0; i < 0xC8; i++) {
        g_BattleSpecialSandstormParticleRecords[i].active = 0;
    }
    g_BattleSpecialSandstormTargetMask = targetMask;
    taskPointer = (BattleFxSandstormRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialSandstormTimelineTask);
    Battle_GetBoneWorldPos(actorIdx, g_BattleActorRecords[actorIdx].bone33, &taskPointer->startX);
    Battle_GetActorMaskCentroid(targetMask, &taskPointer->endX);
    Battle_PlaySfxPannedToActor(0x69, actorIdx, 0x40);
}

/*
 * Drives the main timeline and particle field for Suna.
 */
// FUNCTION: FF7 0x0055a883
void __cdecl BattleFx_SpecialSandstormTimelineTask(void)
{
    short heading;
    short resolvedDeltaY;
    short curDeltaX;
    BattleFxSandstormRecord *taskSlot;
    short length;
    short actorIndex;
    int stackWords; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxSandstormParticle *spawned;
    short puffIdx;

    taskSlot = (BattleFxSandstormRecord *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (taskSlot->step == 0) {
        heading = ArcTangentFixed(taskSlot->endX - taskSlot->startX, taskSlot->endZ - taskSlot->startZ);
        spawned = (BattleFxSandstormParticle *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialSandstormParticleFieldTask);
        spawned->posX = taskSlot->startX;
        spawned->posY = taskSlot->startY;
        spawned->posZ = taskSlot->startZ;
        spawned->stepX = (taskSlot->endX - taskSlot->startX) / 0x14;
        spawned->angle = 0;
        spawned->stepZ = (taskSlot->endZ - taskSlot->startZ) / 0x14;
        spawned->rotationY = heading;
        spawned->growth = 0;
        spawned = (BattleFxSandstormParticle *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialSandstormSpinGrowModelTask);
        spawned->posX = taskSlot->startX;
        spawned->posY = taskSlot->startY;
        spawned->posZ = taskSlot->startZ;
        spawned->stepZ = 0x400;
        spawned->angle = heading - 0x400;
        curDeltaX = taskSlot->endX - taskSlot->startX;
        resolvedDeltaY = taskSlot->endZ - taskSlot->startZ;
        length = SqrtToInt(curDeltaX * curDeltaX + resolvedDeltaY * resolvedDeltaY);
        spawned->scaleY = length * 0x1500 / 0xAF0;
        spawned->growth = 0;
    }
    if (taskSlot->step == 0x23) {
        for (actorIndex = 0; actorIndex < 0xA; actorIndex++) {
            if ((g_BattleSpecialSandstormTargetMask & 1 << actorIndex) != 0) {
                Battle_PostActorHitOrHurtMessage(actorIndex);
            }
        }
    }
    if (taskSlot->step > 0x1F && taskSlot->step < 0x27) {
        for (actorIndex = 0; actorIndex < 0xA; actorIndex++) {
            if ((g_BattleSpecialSandstormTargetMask & 1 << actorIndex) != 0) {
                for (puffIdx = 0; puffIdx < 2; puffIdx++) {
                    spawned = (BattleFxSandstormParticle *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialSandstormVariantSpriteTask);
                    Battle_GetBoneWorldPos(actorIndex, g_BattleActorRecords[actorIndex].targetBone, &spawned->posX);
                    spawned->posX += (short)(rand() % 0x3E8 - 0x1F4);
                    spawned->posY += (short)(rand() % 0x3E8 - 0x1F4);
                    spawned->posZ += (short)(rand() % 0x3E8 - 0x1F4);
                    spawned->variant = rand() & 3;
                }
            }
        }
    }
    (taskSlot->step)++;
    if (taskSlot->step == 0x32) {
        taskSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Updates and renders sand cloud variant sprites for Suna.
 */
// FUNCTION: FF7 0x0055ac15
void __cdecl BattleFx_SpecialSandstormVariantSpriteTask(void)
{
    BattleFxSandstormVariantSprite *task;

    task = (BattleFxSandstormVariantSprite *)BATTLE_CURRENT_TASK_SLOT;
    BattleFx_InstallBillboardMatrix(&task->posX, 0x1000, 0);
    g_BattleSandstormSprite->sunaSpriteFrameWord = task->frameIndex;
    g_BattleSandstormSprite->animTable = Gfx_ModelTableGetLoadedRecord(((int *)&g_BattleSpecialSandstormConfig)[task->variantIndex], g_BattleSpecialSandstormLoadedRes);
    g_BattleSandstormSprite->drawObj = Gfx_ModelTableGetSpriteObject(((int *)&g_BattleSpecialSandstormConfig)[task->variantIndex], g_BattleSpecialSandstormLoadedRes);
    g_BattleSpecialSandstormDrawListHandle = BattleFx_SubmitSpriteDraw(&g_BattleSpecialSandstormSpriteData, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialSandstormDrawListHandle);
    if (g_BattleIsPaused == 0 && g_BattleSandstormSprite->animTable != 0) {
        (task->frameIndex)++;
        if (task->frameIndex >= *(int *)((char *)*(int *)g_BattleSandstormSprite->animTable + 4)) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Emits and manages swirling sand particle records for Suna.
 */
// FUNCTION: FF7 0x0055acff
void __cdecl BattleFx_SpecialSandstormParticleFieldTask(void)
{
    BattleFxSandstormParticleRecord *field;
    short recordIndex, activeCnt;
    BattleSpecialSandstormParticleRecord *claimed;

    field = (BattleFxSandstormParticleRecord *)BATTLE_CURRENT_TASK_SLOT;
    BattleFx_SpecialSandstormDrawParticleField(field);
    if (g_BattleIsPaused != 0) {
        return;
    }
    field->angle += 8;
    if (field->age > 0 && field->age < 0x28) {
        for (recordIndex = 0; recordIndex < 8; recordIndex++) {
            claimed = BattleFx_SpecialSandstormClaimParticleRecord();
            if (claimed != 0) {
                claimed->radius = (rand() % 512);
                claimed->angle = rand() & 0xFFF;
            }
        }
    }
    recordIndex = 0;
    for (activeCnt = 0; recordIndex < 0xC8; recordIndex++) {
        if (g_BattleSpecialSandstormParticleRecords[recordIndex].active != 0) {
            activeCnt++;
            g_BattleSpecialSandstormParticleRecords[recordIndex].angle -= 0x80;
            (g_BattleSpecialSandstormParticleRecords[recordIndex].age)++;
            if (g_BattleSpecialSandstormParticleRecords[recordIndex].age == 0x19) {
                g_BattleSpecialSandstormParticleRecords[recordIndex].active = 0;
            }
        }
    }
    (field->age)++;
    if (field->age > 0x28 && activeCnt == 0) {
        field->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Allocates an available particle record slot for Suna.
 */
// FUNCTION: FF7 0x0055ae85
BattleSpecialSandstormParticleRecord *__cdecl BattleFx_SpecialSandstormClaimParticleRecord(void)
{
    short i;

    for (i = 0; i < 0xC8; i++) {
        if (g_BattleSpecialSandstormParticleRecords[i].active == 0) {
            g_BattleSpecialSandstormParticleRecords[i].active = 1;
            g_BattleSpecialSandstormParticleRecords[i].age = 0;
            return &g_BattleSpecialSandstormParticleRecords[i];
        }
    }
    return 0;
}

/*
 * Transforms and renders sand particles for Suna.
 */
// FUNCTION: FF7 0x0055aeea
void __cdecl BattleFx_SpecialSandstormDrawParticleField(BattleFxSandstormParticleRecord *fieldRecord)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[80];
        int sizeByAge;
        struct { unsigned char pad_to_4[0x4]; SVECTOR rotX; };
        struct { unsigned char pad_to_C[0xC]; int projDepth; };
        struct { unsigned char pad_to_10[0x10]; SVECTOR posX; };
        struct { unsigned char pad_to_18[0x18]; short activeCount; };
        struct { unsigned char pad_to_1C[0x1C]; BattleFxSandstormDotBatchHead *primitive; };
        struct { unsigned char pad_to_20[0x20]; int depth; };
        struct { unsigned char pad_to_24[0x24]; short size; };
        struct { unsigned char pad_to_28[0x28]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_48[0x48]; short particleIndex; };
        struct { unsigned char pad_to_4C[0x4C]; int screenPos; };
    } frame;
#pragma pack(pop)

    frame.rotX.vz = 0;
    frame.rotX.vx = frame.rotX.vz;
    frame.rotX.vy = fieldRecord->rotationY;
    RotMatrix(&frame.rotX, &frame.rotMatrix);
    frame.rotMatrix.t[2] = 0;
    frame.rotMatrix.t[1] = frame.rotMatrix.t[2];
    frame.rotMatrix.t[0] = frame.rotMatrix.t[1];
    frame.particleIndex = 0;
    for (frame.activeCount = 0; frame.particleIndex < 0xC8; frame.particleIndex++) {
        if (g_BattleSpecialSandstormParticleRecords[frame.particleIndex].active != 0) {
            frame.activeCount++;
            frame.posX.vx = CosFixed(g_BattleSpecialSandstormParticleRecords[frame.particleIndex].angle) * g_BattleSpecialSandstormParticleRecords[frame.particleIndex].radius * g_BattleSpecialSandstormParticleRecords[frame.particleIndex].age / 0x14 * (fieldRecord->angle + 0x100) >> 0x14;
            frame.posX.vy = SinFixed(g_BattleSpecialSandstormParticleRecords[frame.particleIndex].angle) * g_BattleSpecialSandstormParticleRecords[frame.particleIndex].radius * g_BattleSpecialSandstormParticleRecords[frame.particleIndex].age / 0x14 * (fieldRecord->angle + 0x100) >> 0x14;
            frame.posX.vz = 0;
            SetRotMatrix(&frame.rotMatrix);
            SetTransMatrix(&frame.rotMatrix);
            RotTransPointToInt16(&frame.posX, (short *)&frame.posX, &frame.projDepth);
            frame.posX.vx += (short)(fieldRecord->baseA + fieldRecord->ampA * g_BattleSpecialSandstormParticleRecords[frame.particleIndex].age);
            frame.posX.vy += (short)(fieldRecord->baseB + fieldRecord->ampB * g_BattleSpecialSandstormParticleRecords[frame.particleIndex].age);
            frame.posX.vz += (short)(fieldRecord->baseC + fieldRecord->ampC * g_BattleSpecialSandstormParticleRecords[frame.particleIndex].age);
            SetRotMatrix(&g_BattleCameraTransform);
            SetTransMatrix(&g_BattleCameraTransform);
            RefreshGlobalTransformState();
            frame.primitive = (BattleFxSandstormDotBatchHead *)g_BattleSpecialSandstormDrawListHandle;
            frame.primitive->tag = frame.primitive->tag & 0xFFFFFF | 0x2000000;
            frame.primitive->primCode = 0x68;
            frame.primitive->primCode = 0x6A;
            frame.depth = Gfx_TransformVertexSlot0ReturnDepth(&frame.posX, &frame.screenPos, &frame.projDepth);
            if (g_BattleSpecialSandstormParticleRecords[frame.particleIndex].age < 0x11) {
                frame.sizeByAge = 8;
            } else {
                frame.sizeByAge = 0x19 - g_BattleSpecialSandstormParticleRecords[frame.particleIndex].age;
            }
            frame.size = *(short *)&frame.sizeByAge;
            frame.primitive->colorR = frame.size << 7 >> 3;
            frame.primitive->colorG = frame.size << 7 >> 3;
            frame.primitive->colorB = UNFOLDED(0) >> 3;
            Gfx_DrawPsxPrimitive(frame.primitive);
        }
    }
}

/*
 * Updates and renders spinning and expanding sandstorm model for Suna.
 */
// FUNCTION: FF7 0x0055b1a3
void __cdecl BattleFx_SpecialSandstormSpinGrowModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[84];
        PsxMatrix spinMatrix;
        struct { unsigned char pad_to_30[0x30]; BattleFxSandstormSpinGrowModelSlot *spinSlot; };
        struct { unsigned char pad_to_34[0x34]; PsxMatrix combinedMatrix; };
    } frame;
#pragma pack(pop)

    frame.spinSlot = (BattleFxSandstormSpinGrowModelSlot *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSandstormConfig->modelHandle = Gfx_ModelTableGetRenderObject(0, g_BattleSpecialSandstormLoadedRes);
    frame.spinSlot->spinAngleX = -frame.spinSlot->frameCounter << 7;
    RotMatrix((SVECTOR *)&frame.spinSlot->spinAngleX, &frame.spinMatrix);
    g_BattleSpecialSandstormScaleFactor = frame.spinSlot->scaleY;
    g_BattleSpecialSandstormScale = (frame.spinSlot->growth + 0x100) * 0x1A00 >> 8;
    g_BattleSpecialSandstormScaleVector = g_BattleSpecialSandstormScale;
    ScaleMatrix(&frame.spinMatrix, &g_BattleSpecialSandstormScaleVector);
    frame.spinMatrix.t[0] = frame.spinSlot->posX;
    frame.spinMatrix.t[1] = frame.spinSlot->posY;
    frame.spinMatrix.t[2] = frame.spinSlot->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.spinMatrix, &frame.combinedMatrix);
    SetRotMatrix(&frame.combinedMatrix);
    SetTransMatrix(&frame.combinedMatrix);
    g_BattleSandstormConfig->sunaConfigStepShift26 = g_BattleSpecialSandstormStep << 4;
    g_BattleSpecialSandstormDrawListHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSandstormConfig->modelHandle, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialSandstormDrawListHandle);
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.spinSlot->growth += 8;
    if (frame.spinSlot->frameCounter < 8) {
        g_BattleSpecialSandstormStep++;
    }
    if (frame.spinSlot->frameCounter > 0x2D) {
        g_BattleSpecialSandstormStep--;
    }
    (frame.spinSlot->frameCounter)++;
    if (frame.spinSlot->frameCounter == 0x35) {
        frame.spinSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}
} /* extern "C" */
