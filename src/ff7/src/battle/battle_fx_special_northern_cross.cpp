// src/ff7/src/battle/battle_fx_special_northern_cross.cpp
/* Enemy Special Effect [idx 68]: NorthernCross (JP: nozan / ノーザンクロス (Northern Cross), asset: "SPECIAL/NOZAN") — Northern Cross (0x395, Maximum Kimaira) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle special effect for the Nozan attack: it loads resources, begins the effect for
//   target actors and runs a timeline that drives animated sprites, falling spiral sprites, a
//   falling spinning model and expanding spinning models.

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

// GLOBAL: FF7 0x008a4598
unsigned int g_BattleSpecialNorthernCrossResourceDefs[56] = {0x001900b1, (unsigned int)&g_BattleSpecialNorthernCrossResFilenameNorthernCross, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)((char *)&g_BattleSpecialNorthernCrossResFilenameNorthernCross + 0xc), 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleSpecialNorthernCrossResFilenameDomu, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)((char *)&g_BattleSpecialNorthernCrossResFilenameDomu + 0xc), 0x00000000, 0x00000000, 0x00000000, 0x00192a39, (unsigned int)&g_BattleSpecialNorthernCrossResFilenameNorthe03, (unsigned int)&g_BattleSpecialNorthernCrossResFilenameHosi, 0x00000000, 0x00000000, 0x00192a39, 0x00000000, (unsigned int)((char *)&g_BattleSpecialNorthernCrossResFilenameHosi + 0xc), 0x00000000, 0x00000000, 0x00192a39, (unsigned int)&g_BattleSpecialNorthernCrossResFilenameNorthe01A, (unsigned int)&g_BattleSpecialNorthernCrossResFilenameElehit1, 0x00000000, 0x00000000, 0x00192a39, (unsigned int)((char *)&g_BattleSpecialNorthernCrossResFilenameElehit1 + 0xc), (unsigned int)((char *)&g_BattleSpecialNorthernCrossResFilenameElehit1 + 0x1c), 0x00000000, 0x00000000, 0x00192a39, (unsigned int)&g_BattleSpecialNorthernCrossResFilenameNorthe01B, (unsigned int)&g_BattleSpecialNorthernCrossResFilenameElehit3, 0x00000000, 0x00000000, 0x00192a39, (unsigned int)((char *)&g_BattleSpecialNorthernCrossResFilenameElehit3 + 0xc), (unsigned int)((char *)&g_BattleSpecialNorthernCrossResFilenameElehit3 + 0x1c), 0x00000000, 0x00000000, 0x00192a39, (unsigned int)&g_BattleSpecialNorthernCrossResFilenameNorthe00, (unsigned int)&g_BattleSpecialNorthernCrossResFilenameBaku, 0x00000000, 0x00000000, 0x00000000};
// GLOBAL: FF7 0x008a4678
BattleFxSpriteRequest g_BattleSpecialNorthernCrossSpriteReq1 = {0, 0, -128, -128, -128, 0x2c, 0, 0};

// GLOBAL: FF7 0x008a4688
int g_BattleSpecialNorthernCrossModelIndices[6] = {5, 6, 7, 8, 9, 10};

// GLOBAL: FF7 0x008a46a0
BattleFxNorthernCrossModelReq g_BattleSpecialNorthernCrossModelReq1 = {0, 0, 0, 0, 0x20, 0};
// GLOBAL: FF7 0x008a46b0
BattleFxSpriteRequest g_BattleSpecialNorthernCrossSpriteReq2 = {0, 0, 0x60, 0x60, 0x60, 0x2c, 0, 0};

// GLOBAL: FF7 0x008a46c0
BattleFxNorthernCrossModelReq g_BattleSpecialNorthernCrossModelReq2 = {0, 0x80, 0, 0, 0x20, 0};
// GLOBAL: FF7 0x008a46d0
unsigned char g_BattleSpecialNorthernCrossResFilenameNorthernCross[22] =
    "nozan.rsd\0\0\0"
    "soto1.rsd";
// GLOBAL: FF7 0x008a46e8
unsigned char g_BattleSpecialNorthernCrossResFilenameDomu[21] =
    "domu1.rsd\0\0\0"
    "soko.rsd";
// GLOBAL: FF7 0x008a4700
unsigned char g_BattleSpecialNorthernCrossResFilenameNorthe03[13] = "northe03.tim";
// GLOBAL: FF7 0x008a4710
unsigned char g_BattleSpecialNorthernCrossResFilenameHosi[23] =
    "hosi.spt\0\0\0\0"
    "jo_c03.spt";
// GLOBAL: FF7 0x008a4728
unsigned char g_BattleSpecialNorthernCrossResFilenameNorthe01A[13] = "northe01.tim";
// GLOBAL: FF7 0x008a4738
unsigned char g_BattleSpecialNorthernCrossResFilenameElehit1[40] =
    "elehit1.spt\0"
    "northe01.tim\0\0\0\0"
    "elehit2.spt";
// GLOBAL: FF7 0x008a4760
unsigned char g_BattleSpecialNorthernCrossResFilenameNorthe01B[13] = "northe01.tim";
// GLOBAL: FF7 0x008a4770
unsigned char g_BattleSpecialNorthernCrossResFilenameElehit3[40] =
    "elehit3.spt\0"
    "northe01.tim\0\0\0\0"
    "elehit4.spt";
// GLOBAL: FF7 0x008a4798
unsigned char g_BattleSpecialNorthernCrossResFilenameNorthe00[13] = "northe00.tim";
// GLOBAL: FF7 0x008a47a8
unsigned char g_BattleSpecialNorthernCrossResFilenameBaku[10] = "baku1.spt";
// clang-format on

#pragma intrinsic(memset)
#pragma intrinsic(memcpy)


// clang-format off
// GLOBAL: FF7 0x00b6d2f8
int g_BattleSpecialNorthernCrossCentroidVec = 0;
// GLOBAL: FF7 0x00b6d2fc
short g_BattleSpecialNorthernCrossCentroidZ = 0;
unsigned char g_bssPad_00b6d300[4] = {0};
// GLOBAL: FF7 0x00b6d304
int g_BattleSpecialNorthernCrossDrawListHandle = 0;
// GLOBAL: FF7 0x00b6d308
short g_BattleSpecialNorthernCrossTargetMask = 0;
unsigned char g_bssPad_00b6d30c[4] = {0};
// GLOBAL: FF7 0x00b6d310
int g_BattleSpecialNorthernCrossScaleX = 0;
// GLOBAL: FF7 0x00b6d314
int g_BattleSpecialNorthernCrossScaleY = 0;
// GLOBAL: FF7 0x00b6d318
int g_BattleSpecialNorthernCrossScaleZ = 0;
unsigned char g_bssPad_00b6d31c[4] = {0};
// GLOBAL: FF7 0x00b6d320
struct GfxTableCursor *g_BattleSpecialNorthernCrossLoadedRes = 0;
unsigned char g_bssPad_00b6d324[4] = {0};
// clang-format on

/*
 * Loads resources for the Nozan special effect when not already loaded.
 */
// FUNCTION: FF7 0x00544590
void __cdecl BattleFx_SpecialLoadResNorthernCross(void)
{
    GfxModelTableBank *unitOwner;
    GfxResourceRequest requestSlot;
    int resCategory;
    char pathWorkBuffer[200];
    GfxResourceRequest *pRequest;
    int resourceDefsWord;

    if (g_pBattle3dScene != 0 && g_BattleSpecialNorthernCrossLoadedRes == 0) {
        resCategory = 0xB;
        resourceDefsWord = (int)&g_BattleSpecialNorthernCrossResourceDefs;
        unitOwner = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRequest = &requestSlot;
        Gfx_InitResourceRequest(pRequest);
        pRequest->groupTag = 0;
        // STRING: FF7 0x008a47b4
        pRequest->resourcePath = BattleFx_BuildResourcePath("SPECIAL/NOZAN", pathWorkBuffer);
        Gfx_LoadModelTableIfAbsent(resCategory, resourceDefsWord, pRequest, &g_BattleSpecialNorthernCrossLoadedRes, unitOwner);
    }
}

/*
 * Entry point for the Nozan special effect, dispatching to begin.
 */
// FUNCTION: FF7 0x00544620
void __cdecl BattleFx_SpecialEnterNorthernCross(int targetMask, int actorIdx)
{
    BattleFx_SpecialBeginNorthernCross(targetMask, actorIdx);
}

/*
 * Initializes and begins the Nozan special effect for target actors.
 */
// FUNCTION: FF7 0x00544635
void __cdecl BattleFx_SpecialBeginNorthernCross(int targetMask, int actorIdx)
{
    BattleFxNorthernCrossParticle *taskRecord;

    BattleFx_SpecialLoadResNorthernCross();
    g_BattleSpecialNorthernCrossTargetMask = targetMask;
    taskRecord = (BattleFxNorthernCrossParticle *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialNorthernCrossTimelineTask);
    Battle_GetBoneWorldPos(actorIdx, g_BattleActorRecords[actorIdx].bone34, &taskRecord->posX);
    Battle_GetActorMaskCentroid(targetMask, (short *)&g_BattleSpecialNorthernCrossCentroidVec);
    Battle_PlaySfxPannedToActor(0x123, (short)actorIdx, 0x40);
    g_BattleFxTimelineState.channel2 = 0;
    g_BattleFxTimelineState.channel1 = g_BattleFxTimelineState.channel2;
    g_BattleFxTimelineState.red = g_BattleFxTimelineState.channel1;
    Battle_StartFixedPointRamp(0x1000, 0x14);
}

/*
 * Timeline task controlling state progression for the Nozan special effect.
 */
// FUNCTION: FF7 0x005446dd
void __cdecl BattleFx_SpecialNorthernCrossTimelineTask(void)
{
    int p; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int q; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleTaskSlot *taskPtr;
    short i9;
    int s; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int u; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxNorthernCrossParticle *spawned;

    taskPtr = BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (taskPtr->frameCounter >= 0 && taskPtr->frameCounter < 0x1E) {
        for (i9 = 0; i9 < 2; i9++) {
            spawned = (BattleFxNorthernCrossParticle *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialNorthernCrossFallingSpiralSpriteTask);
            spawned->posX = *(short *)&g_BattleSpecialNorthernCrossCentroidVec;
            spawned->posY = 0xF060;
            spawned->posZ = g_BattleSpecialNorthernCrossCentroidZ;
            spawned->phase = rand() % 0x1000;
            spawned->startOffset = rand() % 0xFA0;
            spawned->variant = i9;
        }
    }
    if (taskPtr->frameCounter >= 0xF && taskPtr->frameCounter < 0x1E && taskPtr->frameCounter % 2 == 0) {
        spawned = (BattleFxNorthernCrossParticle *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialNorthernCrossFallingSpinModelTask);
        spawned->posX = *(short *)&g_BattleSpecialNorthernCrossCentroidVec;
        spawned->posY = 0xF060;
        spawned->posZ = g_BattleSpecialNorthernCrossCentroidZ;
        spawned->phase = rand() % 0x1000;
        spawned->startOffset = rand() % 0xFA0;
        spawned->variant = rand() & 1;
    }
    if (taskPtr->frameCounter > 0x14 && taskPtr->frameCounter < 0x3C) {
        for (i9 = 0; i9 < 0xA; i9++) {
            if ((g_BattleSpecialNorthernCrossTargetMask & 1 << i9) != 0) {
                spawned = (BattleFxNorthernCrossParticle *)&g_BattleTaskSlotBuffer.secondarySlots[(short)Battle_AllocateSecondaryResourceSlot(BattleFx_SpecialNorthernCrossAnimatedSpriteTask)];
                Battle_GetBoneWorldPos(i9, g_BattleActorRecords[i9].targetBone, &spawned->posX);
                spawned->posX += (short)(rand() % 0x3E8 - 0x1F4);
                spawned->posY += (short)(rand() % 0x3E8 - 0x1F4);
                spawned->posZ += (short)(rand() % 0x3E8 - 0x1F4);
                spawned->variant = 0;
            }
        }
    }
    if (taskPtr->frameCounter == 0x46) {
        for (i9 = 0; i9 < 0xA; i9++) {
            if ((g_BattleSpecialNorthernCrossTargetMask & 1 << i9) != 0) {
                Battle_PostActorHitOrHurtMessage(i9);
            }
        }
        Battle_PlaySfxPannedToActor(0xFA, -1, 0x40);
    }
    if (taskPtr->frameCounter > 0x42 && taskPtr->frameCounter < 0x4A && taskPtr->frameCounter % 2 == 0) {
        for (i9 = 0; i9 < 0xA; i9++) {
            if ((g_BattleSpecialNorthernCrossTargetMask & 1 << i9) != 0) {
                spawned = (BattleFxNorthernCrossParticle *)&g_BattleTaskSlotBuffer.secondarySlots[(short)Battle_AllocateSecondaryResourceSlot(BattleFx_SpecialNorthernCrossAnimatedSpriteTask)];
                Battle_GetBoneWorldPos(i9, g_BattleActorRecords[i9].targetBone, &spawned->posX);
                spawned->posX += (short)(rand() % 0x578 - 0x2BC);
                spawned->posY += (short)(rand() % 0x578 - 0x2BC);
                spawned->posZ += (short)(rand() % 0x578 - 0x2BC);
                spawned->variant = 5;
                spawned = (BattleFxNorthernCrossParticle *)&g_BattleTaskSlotBuffer.secondarySlots[(short)Battle_AllocateSecondaryResourceSlot(BattleFx_SpecialNorthernCrossAnimatedSpriteTask)];
                Battle_GetBoneWorldPos(i9, g_BattleActorRecords[i9].targetBone, &spawned->posX);
                spawned->posX += (short)(rand() % 0x258 - 0x12C);
                spawned->posY += (short)(rand() % 0x258 - 0x12C);
                spawned->posZ += (short)(rand() % 0x258 - 0x12C);
                spawned->variant = rand() % 4 + 1;
            }
        }
    }
    (taskPtr->frameCounter)++;
    if (taskPtr->frameCounter == 0x50) {
        Battle_StartFixedPointRamp(0, 0x14);
        taskPtr->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Task to animate and render sprites for the Nozan special effect.
 */
// FUNCTION: FF7 0x00544c22
void __cdecl BattleFx_SpecialNorthernCrossAnimatedSpriteTask(void)
{
    BattleFxSmogSpriteSlot *task;

    task = (BattleFxSmogSpriteSlot *)&g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx];
    BattleFx_InstallBillboardMatrix(&task->posX, (task->variant != 0 ? 0x800 : 0) + 0x1800, 0);
    g_BattleSpecialNorthernCrossSpriteReq1.frame = task->frameIndex;
    g_BattleSpecialNorthernCrossSpriteReq1.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(g_BattleSpecialNorthernCrossModelIndices[task->variant], g_BattleSpecialNorthernCrossLoadedRes);
    if (task->variant != 0) {
        g_BattleSpecialNorthernCrossSpriteReq1.drawObj = Gfx_ModelTableGetSpriteObject(g_BattleSpecialNorthernCrossModelIndices[task->variant], g_BattleSpecialNorthernCrossLoadedRes);
    } else {
        g_BattleSpecialNorthernCrossSpriteReq1.drawObj = Battle3d_GetSceneRenderObject(3, g_pBattle3dScene);
    }
    g_BattleOverlayDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecialNorthernCrossSpriteReq1, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (g_BattleSpecialNorthernCrossSpriteReq1.spriteGroup != 0) {
        (task->frameIndex)++;
        if (task->frameIndex >= g_BattleSpecialNorthernCrossSpriteReq1.spriteGroup->header->frameCount) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Task to animate falling spiral sprites for the Nozan special effect.
 */
// FUNCTION: FF7 0x00544d43
void __cdecl BattleFx_SpecialNorthernCrossFallingSpiralSpriteTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[20];
        int fadeLevel;
        struct { unsigned char pad_to_4[0x4]; SVECTOR posX; };
        struct { unsigned char pad_to_C[0xC]; short fadeValue; };
        struct { unsigned char pad_to_10[0x10]; BattleFxNorthernCrossFallingSpinModelSlot *taskSlot; };
    } frame;
#pragma pack(pop)

    frame.taskSlot = (BattleFxNorthernCrossFallingSpinModelSlot *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecialNorthernCrossSpriteReq2.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(4, g_BattleSpecialNorthernCrossLoadedRes);
    g_BattleSpecialNorthernCrossSpriteReq2.drawObj = Gfx_ModelTableGetSpriteObject(4, g_BattleSpecialNorthernCrossLoadedRes);
    if (frame.taskSlot->frameCounter < 0x16) {
        frame.fadeLevel = 8;
    } else {
        frame.fadeLevel = 0x1E - frame.taskSlot->frameCounter;
    }
    frame.fadeValue = *(short *)&frame.fadeLevel;
    frame.posX.vx = SinFixed(frame.taskSlot->angle) * frame.taskSlot->radius >> 0xC;
    frame.posX.vz = CosFixed(frame.taskSlot->angle) * frame.taskSlot->radius >> 0xC;
    frame.posX.vx += frame.taskSlot->velocityX;
    frame.posX.vz += frame.taskSlot->velocityZ;
    frame.posX.vy = frame.taskSlot->posY;
    BattleFx_InstallBillboardMatrix(&frame.posX, (SinFixed(frame.taskSlot->frameCounter * 0x300) << 0xB >> 0xC) + 0x1000, 0);
    g_BattleSpecialNorthernCrossSpriteReq2.colorB = frame.fadeValue << 4;
    g_BattleSpecialNorthernCrossSpriteReq2.colorG = g_BattleSpecialNorthernCrossSpriteReq2.colorB;
    g_BattleSpecialNorthernCrossSpriteReq2.colorR = g_BattleSpecialNorthernCrossSpriteReq2.colorG;
    g_BattleOverlayDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecialNorthernCrossSpriteReq2, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused == 0) {
        frame.taskSlot->posY += 0x85;
        frame.taskSlot->angle += (short)(0x40 - (frame.taskSlot->spinRate << 7));
        (frame.taskSlot->frameCounter)++;
        if (frame.taskSlot->frameCounter == 0x1E) {
            frame.taskSlot->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Task to animate and render the falling spinning model for Nozan.
 */
// FUNCTION: FF7 0x00544f03
void __cdecl BattleFx_SpecialNorthernCrossFallingSpinModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[120];
        int fadeLevel;
        struct { unsigned char pad_to_4[0x4]; SVECTOR rotAngleX; };
        struct { unsigned char pad_to_C[0xC]; short posX; };
        struct { unsigned char pad_to_E[0xE]; short posY; };
        struct { unsigned char pad_to_10[0x10]; short posZ; };
        struct { unsigned char pad_to_14[0x14]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_34[0x34]; short fadeValue; };
        struct { unsigned char pad_to_38[0x38]; BattleFxPositionedSpriteSlot *spawnSlot; };
        struct { unsigned char pad_to_44[0x44]; BattleFxNorthernCrossFallingSpinModelSlot *taskSlot; };
        struct { unsigned char pad_to_48[0x48]; PsxMatrix combinedMatrix; };
        struct { unsigned char pad_to_68[0x68]; VECTOR scaleX; };
    } frame;
#pragma pack(pop)

    frame.taskSlot = (BattleFxNorthernCrossFallingSpinModelSlot *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecialNorthernCrossModelReq2.modelGroup = Gfx_ModelTableGetRenderObject(0, g_BattleSpecialNorthernCrossLoadedRes);
    if (frame.taskSlot->frameCounter < 0x16) {
        frame.fadeLevel = 8;
    } else {
        frame.fadeLevel = 0x1E - frame.taskSlot->frameCounter;
    }
    frame.fadeValue = *(short *)&frame.fadeLevel;
    frame.posX = SinFixed(frame.taskSlot->angle) * frame.taskSlot->radius >> 0xC;
    frame.posZ = CosFixed(frame.taskSlot->angle) * frame.taskSlot->radius >> 0xC;
    frame.posX += frame.taskSlot->velocityX;
    frame.posZ += frame.taskSlot->velocityZ;
    frame.posY = frame.taskSlot->posY;
    frame.rotAngleX.vx = frame.taskSlot->frameCounter << 8;
    frame.rotAngleX.vy = frame.taskSlot->frameCounter << 9;
    frame.rotAngleX.vz = 0;
    RotMatrix(&frame.rotAngleX, &frame.rotMatrix);
    frame.scaleX.vz = 0x1000;
    frame.scaleX.vy = frame.scaleX.vz;
    frame.scaleX.vx = frame.scaleX.vy;
    ScaleMatrix(&frame.rotMatrix, &frame.scaleX);
    frame.rotMatrix.t[0] = frame.posX;
    frame.rotMatrix.t[1] = frame.posY;
    frame.rotMatrix.t[2] = frame.posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.combinedMatrix);
    SetRotMatrix(&frame.combinedMatrix);
    SetTransMatrix(&frame.combinedMatrix);
    SetDrawColorRGB(0, 0, 0);
    g_BattleSpecialNorthernCrossDrawListHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecialNorthernCrossModelReq2, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialNorthernCrossDrawListHandle);
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.taskSlot->posY += 0x85;
    frame.taskSlot->angle += (short)(0x40 - (frame.taskSlot->spinRate << 7));
    (frame.taskSlot->frameCounter)++;
    if (frame.taskSlot->frameCounter == 0x1E) {
        frame.spawnSlot = (BattleFxPositionedSpriteSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialNorthernCrossExpandingSpinModelsTask);
        frame.spawnSlot->posX = frame.posX;
        frame.spawnSlot->posY = frame.posY;
        frame.spawnSlot->posZ = frame.posZ;
        frame.taskSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Task to render expanding spinning models for the Nozan special effect.
 */
// FUNCTION: FF7 0x0054513f
void __cdecl BattleFx_SpecialNorthernCrossExpandingSpinModelsTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[80];
        struct { unsigned char pad_to_4[0x4]; SVECTOR rotAngleX; };
        struct { unsigned char pad_to_C[0xC]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_2C[0x2C]; BattleFxOilTargetModelSlot *taskSlot; };
        struct { unsigned char pad_to_30[0x30]; PsxMatrix combinedMatrix; };
    } frame;
#pragma pack(pop)

    frame.taskSlot = (BattleFxOilTargetModelSlot *)BATTLE_CURRENT_TASK_SLOT;
    frame.rotAngleX.vx = 0;
    frame.rotAngleX.vy = frame.taskSlot->frameCounter << 8;
    frame.rotAngleX.vz = 0;
    RotMatrix(&frame.rotAngleX, &frame.rotMatrix);
    g_BattleSpecialNorthernCrossScaleZ = (frame.taskSlot->frameCounter << 9) + 0x1000;
    g_BattleSpecialNorthernCrossScaleY = g_BattleSpecialNorthernCrossScaleZ;
    g_BattleSpecialNorthernCrossScaleX = g_BattleSpecialNorthernCrossScaleY;
    ScaleMatrix(&frame.rotMatrix, &g_BattleSpecialNorthernCrossScaleX);
    frame.rotMatrix.t[0] = frame.taskSlot->posX;
    frame.rotMatrix.t[1] = frame.taskSlot->posY;
    frame.rotMatrix.t[2] = frame.taskSlot->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.combinedMatrix);
    SetRotMatrix(&frame.combinedMatrix);
    SetTransMatrix(&frame.combinedMatrix);
    g_BattleSpecialNorthernCrossModelReq1.fadeLevel = 0x80 - frame.taskSlot->frameCounter * 6;
    g_BattleSpecialNorthernCrossModelReq1.flags = 0x28;
    g_BattleSpecialNorthernCrossModelReq1.modelGroup = Gfx_ModelTableGetRenderObject(2, g_BattleSpecialNorthernCrossLoadedRes);
    g_BattleSpecialNorthernCrossDrawListHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecialNorthernCrossModelReq1, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialNorthernCrossDrawListHandle);
    g_BattleSpecialNorthernCrossModelReq1.modelGroup = Gfx_ModelTableGetRenderObject(1, g_BattleSpecialNorthernCrossLoadedRes);
    g_BattleSpecialNorthernCrossDrawListHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecialNorthernCrossModelReq1, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialNorthernCrossDrawListHandle);
    if (g_BattleIsPaused != 0) {
        return;
    }
    (frame.taskSlot->frameCounter)++;
    if (frame.taskSlot->frameCounter == 0x14) {
        frame.taskSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}
} /* extern "C" */
