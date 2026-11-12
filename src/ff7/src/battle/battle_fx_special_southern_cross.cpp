// src/ff7/src/battle/battle_fx_special_southern_cross.cpp
/* Enemy Special Effect [idx 29]: SouthernCross (JP: sazan / サザンクロス (Southern Cross), asset: "SPECIAL/SAZAN") — Southern Cross (0x26d, Ancient Dragon), Flame Dance (0x261) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle special effect for the Sazan attack: it loads resources, begins the effect for
//   target actors and runs a timeline that drives variant and orbiting sprites, a falling spinning
//   model and expanding spinning models.

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

// GLOBAL: FF7 0x008a47c8
unsigned int g_BattleSpecialSouthernCrossResourceDefs[56] = {0x001900b4, (unsigned int)&g_BattleSpecialSouthernCrossResFilenameSouthernCross, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)((char *)&g_BattleSpecialSouthernCrossResFilenameSouthernCross + 0xc), 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleSpecialSouthernCrossResFilenameDomu, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)((char *)&g_BattleSpecialSouthernCrossResFilenameDomu + 0xc), 0x00000000, 0x00000000, 0x00000000, 0x00192a39, (unsigned int)&g_BattleSpecialSouthernCrossResFilenameSouthe03, (unsigned int)&g_BattleSpecialSouthernCrossResFilenameHosi, 0x00000000, 0x00000000, 0x00192a39, 0x00000000, (unsigned int)((char *)&g_BattleSpecialSouthernCrossResFilenameHosi + 0xc), 0x00000000, 0x00000000, 0x00192a39, (unsigned int)&g_BattleSpecialSouthernCrossResFilenameSouthe01A, (unsigned int)&g_BattleSpecialSouthernCrossResFilenameElehit1, 0x00000000, 0x00000000, 0x00192a39, (unsigned int)((char *)&g_BattleSpecialSouthernCrossResFilenameElehit1 + 0xc), (unsigned int)((char *)&g_BattleSpecialSouthernCrossResFilenameElehit1 + 0x1c), 0x00000000, 0x00000000, 0x00192a39, (unsigned int)&g_BattleSpecialSouthernCrossResFilenameSouthe01B, (unsigned int)&g_BattleSpecialSouthernCrossResFilenameElehit3, 0x00000000, 0x00000000, 0x00192a39, (unsigned int)((char *)&g_BattleSpecialSouthernCrossResFilenameElehit3 + 0xc), (unsigned int)((char *)&g_BattleSpecialSouthernCrossResFilenameElehit3 + 0x1c), 0x00000000, 0x00000000, 0x00192a39, (unsigned int)&g_BattleSpecialSouthernCrossResFilenameSouthe00, (unsigned int)&g_BattleSpecialSouthernCrossResFilenameBaku, 0x00000000, 0x00000000, 0x00000000};
// GLOBAL: FF7 0x008a48a8
struct BattleFxSpriteRequest g_BattleSpecialSouthernCrossColor1 = {0, 0, -128, -128, -128, 0x2c, 0, 0};

// GLOBAL: FF7 0x008a48b8
int g_BattleSpecialSouthernCrossModelIndices[6] = {5, 6, 7, 8, 9, 10};

// GLOBAL: FF7 0x008a48d0
unsigned char g_BattleSpecialSouthernCrossModelReq1[16] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x008a48e0
struct BattleFxSpriteRequest g_BattleSpecialSouthernCrossColor2 = {0, 0, 96, 96, 96, 0x2c, 0, 0};

// GLOBAL: FF7 0x008a48f0
unsigned char g_BattleSpecialSouthernCrossModelReq2[16] = {0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x008a4900
unsigned char g_BattleSpecialSouthernCrossResFilenameSouthernCross[22] =
    "sazan.rsd\0\0\0"
    "soto1.rsd";
// GLOBAL: FF7 0x008a4918
unsigned char g_BattleSpecialSouthernCrossResFilenameDomu[21] =
    "domu1.rsd\0\0\0"
    "soko.rsd";
// GLOBAL: FF7 0x008a4930
unsigned char g_BattleSpecialSouthernCrossResFilenameSouthe03[13] = "southe03.tim";
// GLOBAL: FF7 0x008a4940
unsigned char g_BattleSpecialSouthernCrossResFilenameHosi[23] =
    "hosi.spt\0\0\0\0"
    "jo_c01.spt";
// GLOBAL: FF7 0x008a4958
unsigned char g_BattleSpecialSouthernCrossResFilenameSouthe01A[13] = "southe01.tim";
// GLOBAL: FF7 0x008a4968
unsigned char g_BattleSpecialSouthernCrossResFilenameElehit1[40] =
    "elehit1.spt\0"
    "southe01.tim\0\0\0\0"
    "elehit2.spt";
// GLOBAL: FF7 0x008a4990
unsigned char g_BattleSpecialSouthernCrossResFilenameSouthe01B[13] = "southe01.tim";
// GLOBAL: FF7 0x008a49a0
unsigned char g_BattleSpecialSouthernCrossResFilenameElehit3[40] =
    "elehit3.spt\0"
    "southe01.tim\0\0\0\0"
    "elehit4.spt";
// GLOBAL: FF7 0x008a49c8
unsigned char g_BattleSpecialSouthernCrossResFilenameSouthe00[13] = "southe00.tim";
// GLOBAL: FF7 0x008a49d8
unsigned char g_BattleSpecialSouthernCrossResFilenameBaku[10] = "baku1.spt";
// clang-format on

#pragma intrinsic(memset)
#pragma intrinsic(memcpy)


// clang-format off
// GLOBAL: FF7 0x00b6d328
int g_BattleSpecialSouthernCrossCentroidVec = 0;
// GLOBAL: FF7 0x00b6d32c
short g_BattleSpecialSouthernCrossCentroidZ = 0;
unsigned char g_bssPad_00b6d330[4] = {0};
// GLOBAL: FF7 0x00b6d334
int g_BattleSpecialSouthernCrossDrawListHandle = 0;
// GLOBAL: FF7 0x00b6d338
short g_BattleSpecialSouthernCrossTargetMask = 0;
unsigned char g_bssPad_00b6d33c[4] = {0};
// GLOBAL: FF7 0x00b6d340
int g_BattleSpecialSouthernCrossScaleX = 0;
// GLOBAL: FF7 0x00b6d344
int g_BattleSpecialSouthernCrossScaleY = 0;
// GLOBAL: FF7 0x00b6d348
int g_BattleSpecialSouthernCrossScaleZ = 0;
unsigned char g_bssPad_00b6d34c[4] = {0};
// GLOBAL: FF7 0x00b6d350
struct GfxTableCursor *g_BattleSpecialSouthernCrossLoadedRes = 0;
unsigned char g_bssPad_00b6d354[4] = {0};
// clang-format on

/*
 * Loads resources for the Sazan special effect when not already loaded.
 */
// FUNCTION: FF7 0x005452e0
void __cdecl BattleFx_SpecialLoadResSouthernCross(void)
{
    GfxModelTableBank *unitOwner;
    GfxResourceRequest requestSlot;
    int resCategory;
    char pathWorkBuffer[200];
    GfxResourceRequest *pRequest;
    int resourceDefsWord;

    if (g_pBattle3dScene != 0 && g_BattleSpecialSouthernCrossLoadedRes == 0) {
        resCategory = 0xB;
        resourceDefsWord = (int)&g_BattleSpecialSouthernCrossResourceDefs;
        unitOwner = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRequest = &requestSlot;
        Gfx_InitResourceRequest(pRequest);
        pRequest->groupTag = 0;
        // STRING: FF7 0x008a49e4
        pRequest->resourcePath = BattleFx_BuildResourcePath("SPECIAL/SAZAN", pathWorkBuffer);
        Gfx_LoadModelTableIfAbsent(resCategory, resourceDefsWord, pRequest, &g_BattleSpecialSouthernCrossLoadedRes, unitOwner);
    }
}

/*
 * Entry point for the Sazan special effect, dispatching to begin.
 */
// FUNCTION: FF7 0x00545370
void __cdecl BattleFx_SpecialEnterSouthernCross(int targetMask, int actorIdx)
{
    BattleFx_SpecialBeginSouthernCross(targetMask, actorIdx);
}

/*
 * Initializes and begins the Sazan special effect for target actors.
 */
// FUNCTION: FF7 0x00545385
void __cdecl BattleFx_SpecialBeginSouthernCross(int targetMask, int actorIdx)
{
    BattleFxSouthernCrossParticle *taskRecord;

    BattleFx_SpecialLoadResSouthernCross();
    g_BattleSpecialSouthernCrossTargetMask = targetMask;
    taskRecord = (BattleFxSouthernCrossParticle *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialSouthernCrossTimelineTask);
    Battle_GetBoneWorldPos(actorIdx, g_BattleActorRecords[actorIdx].bone34, &taskRecord->posX);
    Battle_GetActorMaskCentroid(targetMask, (short *)&g_BattleSpecialSouthernCrossCentroidVec);
    Battle_PlaySfxPannedToActor(0x123, (short)actorIdx, 0x40);
    g_BattleFxTimelineState.channel2 = 0;
    g_BattleFxTimelineState.channel1 = g_BattleFxTimelineState.channel2;
    g_BattleFxTimelineState.red = g_BattleFxTimelineState.channel1;
    Battle_StartFixedPointRamp(0x1000, 20);
}

/*
 * Timeline task controlling state progression for the Sazan special effect.
 */
// FUNCTION: FF7 0x0054542d
void __cdecl BattleFx_SpecialSouthernCrossTimelineTask(void)
{
    int p; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int q; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleTaskSlot *taskPtr;
    short i9;
    int s; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int u; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxSouthernCrossParticle *particle;

    taskPtr = BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (taskPtr->frameCounter >= 0 && taskPtr->frameCounter < 30) {
        for (i9 = 0; i9 < 2; i9++) {
            particle = (BattleFxSouthernCrossParticle *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialSouthernCrossOrbitSpriteTask);
            particle->posX = *(short *)&g_BattleSpecialSouthernCrossCentroidVec;
            particle->posY = 0xF060;
            particle->posZ = g_BattleSpecialSouthernCrossCentroidZ;
            particle->phase = rand() % 4096;
            particle->delay = rand() % 4000;
            particle->variant = i9;
        }
    }
    if (taskPtr->frameCounter >= 15 && taskPtr->frameCounter < 30 && taskPtr->frameCounter % 2 == 0) {
        particle = (BattleFxSouthernCrossParticle *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialSouthernCrossFallingSpinModelTask);
        particle->posX = *(short *)&g_BattleSpecialSouthernCrossCentroidVec;
        particle->posY = 0xF060;
        particle->posZ = g_BattleSpecialSouthernCrossCentroidZ;
        particle->phase = rand() % 4096;
        particle->delay = rand() % 4000;
        particle->variant = rand() & 1;
    }
    if (taskPtr->frameCounter > 20 && taskPtr->frameCounter < 60) {
        for (i9 = 0; i9 < 10; i9++) {
            if ((g_BattleSpecialSouthernCrossTargetMask & 1 << i9) != 0) {
                particle = (BattleFxSouthernCrossParticle *)&g_BattleTaskSlotBuffer.secondarySlots[(short)Battle_AllocateSecondaryResourceSlot(BattleFx_SpecialSouthernCrossVariantSpriteTask)];
                Battle_GetBoneWorldPos(i9, g_BattleActorRecords[i9].targetBone, (short *)((int)particle + 4));
                particle->posX += (short)(rand() % 1000 - 500);
                particle->posY += (short)(rand() % 1000 - 500);
                particle->posZ += (short)(rand() % 1000 - 500);
                particle->variant = 0;
            }
        }
    }
    if (taskPtr->frameCounter == 70) {
        for (i9 = 0; i9 < 10; i9++) {
            if ((g_BattleSpecialSouthernCrossTargetMask & 1 << i9) != 0) {
                Battle_PostActorHitOrHurtMessage(i9);
            }
        }
        Battle_PlaySfxPannedToActor(0xFA, -1, 0x40);
    }
    if (taskPtr->frameCounter > 66 && taskPtr->frameCounter < 74 && taskPtr->frameCounter % 3 == 0) {
        for (i9 = 0; i9 < 10; i9++) {
            if ((g_BattleSpecialSouthernCrossTargetMask & 1 << i9) != 0) {
                particle = (BattleFxSouthernCrossParticle *)&g_BattleTaskSlotBuffer.secondarySlots[(short)Battle_AllocateSecondaryResourceSlot(BattleFx_SpecialSouthernCrossVariantSpriteTask)];
                Battle_GetBoneWorldPos(i9, g_BattleActorRecords[i9].targetBone, (short *)((int)particle + 4));
                particle->posX += (short)(rand() % 1400 - 700);
                particle->posY += (short)(rand() % 1400 - 700);
                particle->posZ += (short)(rand() % 1400 - 700);
                particle->variant = 5;
                particle = (BattleFxSouthernCrossParticle *)&g_BattleTaskSlotBuffer.secondarySlots[(short)Battle_AllocateSecondaryResourceSlot(BattleFx_SpecialSouthernCrossVariantSpriteTask)];
                Battle_GetBoneWorldPos(i9, g_BattleActorRecords[i9].targetBone, (short *)((int)particle + 4));
                particle->posX += (short)(rand() % 600 - 300);
                particle->posY += (short)(rand() % 600 - 300);
                particle->posZ += (short)(rand() % 600 - 300);
                particle->variant = rand() % 4 + 1;
            }
        }
    }
    (taskPtr->frameCounter)++;
    if (taskPtr->frameCounter == 80) {
        Battle_StartFixedPointRamp(0, 20);
        taskPtr->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Task to animate and render sprites for the Sazan special effect.
 */
// FUNCTION: FF7 0x0054596f
void __cdecl BattleFx_SpecialSouthernCrossVariantSpriteTask(void)
{
    BattleFxSmogSpriteSlot *task;

    task = (BattleFxSmogSpriteSlot *)&g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx];
    BattleFx_InstallBillboardMatrix((void *)((int)task + 4), (task->variant != 0 ? 0x1000 : 0) + 0x1000, 0);
    g_BattleSpecialSouthernCrossColor1.frame = task->frameIndex;
    g_BattleSpecialSouthernCrossColor1.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(g_BattleSpecialSouthernCrossModelIndices[task->variant], g_BattleSpecialSouthernCrossLoadedRes);
    if (task->variant != 0) {
        g_BattleSpecialSouthernCrossColor1.drawObj = Gfx_ModelTableGetSpriteObject(g_BattleSpecialSouthernCrossModelIndices[task->variant], g_BattleSpecialSouthernCrossLoadedRes);
    } else {
        g_BattleSpecialSouthernCrossColor1.drawObj = Battle3d_GetSceneRenderObject(3, g_pBattle3dScene);
    }
    g_BattleOverlayDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecialSouthernCrossColor1, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (g_BattleSpecialSouthernCrossColor1.spriteGroup != 0) {
        (task->frameIndex)++;
        if (task->frameIndex >= g_BattleSpecialSouthernCrossColor1.spriteGroup->header->frameCount) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Task to animate orbiting sprites for the Sazan special effect.
 */
// FUNCTION: FF7 0x00545a90
void __cdecl BattleFx_SpecialSouthernCrossOrbitSpriteTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[20];
        int rampValue;
        struct { unsigned char pad_to_4[0x4]; SVECTOR posX; };
        struct { unsigned char pad_to_C[0xC]; short alphaLevel; };
        struct { unsigned char pad_to_10[0x10]; BattleFxSouthernCrossOrbitSpriteSlot *orbitSlot; };
    } frame;
#pragma pack(pop)

    frame.orbitSlot = (BattleFxSouthernCrossOrbitSpriteSlot *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecialSouthernCrossColor2.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(4, g_BattleSpecialSouthernCrossLoadedRes);
    g_BattleSpecialSouthernCrossColor2.drawObj = Gfx_ModelTableGetSpriteObject(4, g_BattleSpecialSouthernCrossLoadedRes);
    if (frame.orbitSlot->frameCounter < 22) {
        frame.rampValue = 8;
    } else {
        frame.rampValue = 0x1E - frame.orbitSlot->frameCounter;
    }
    frame.alphaLevel = *(short *)&frame.rampValue;
    frame.posX.vx = SinFixed(frame.orbitSlot->angle) * frame.orbitSlot->radius >> 12;
    frame.posX.vz = CosFixed(frame.orbitSlot->angle) * frame.orbitSlot->radius >> 12;
    frame.posX.vx += frame.orbitSlot->velocityX;
    frame.posX.vz += frame.orbitSlot->velocityZ;
    frame.posX.vy = frame.orbitSlot->velocityY;
    BattleFx_InstallBillboardMatrix(&frame.posX, (SinFixed(frame.orbitSlot->frameCounter * 0x300) << 11 >> 12) + 0x1000, 0);
    g_BattleSpecialSouthernCrossColor2.colorB = frame.alphaLevel << 4;
    g_BattleSpecialSouthernCrossColor2.colorG = g_BattleSpecialSouthernCrossColor2.colorB;
    g_BattleSpecialSouthernCrossColor2.colorR = g_BattleSpecialSouthernCrossColor2.colorG;
    g_BattleOverlayDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecialSouthernCrossColor2, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused == 0) {
        frame.orbitSlot->velocityY += 133;
        frame.orbitSlot->angle += (short)(0x40 - (frame.orbitSlot->spinRate << 7));
        (frame.orbitSlot->frameCounter)++;
        if (frame.orbitSlot->frameCounter == 30) {
            frame.orbitSlot->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Task to animate and render the falling spinning model for Sazan.
 */
// FUNCTION: FF7 0x00545c50
void __cdecl BattleFx_SpecialSouthernCrossFallingSpinModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[120];
        int rampValue;
        struct { unsigned char pad_to_4[0x4]; SVECTOR angleX; };
        struct { unsigned char pad_to_C[0xC]; short posX; };
        struct { unsigned char pad_to_E[0xE]; short posY; };
        struct { unsigned char pad_to_10[0x10]; short posZ; };
        struct { unsigned char pad_to_14[0x14]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_34[0x34]; short rampLevel; };
        struct { unsigned char pad_to_38[0x38]; BattleFxPositionedSpriteSlot *spawnSlot; };
        struct { unsigned char pad_to_44[0x44]; BattleFxSouthernCrossOrbitSpriteSlot *fallingSlot; };
        struct { unsigned char pad_to_48[0x48]; PsxMatrix combinedMatrix; };
        struct { unsigned char pad_to_68[0x68]; VECTOR scaleX; };
    } frame;
#pragma pack(pop)

    frame.fallingSlot = (BattleFxSouthernCrossOrbitSpriteSlot *)BATTLE_CURRENT_TASK_SLOT;
    ((BattleFxModelDraw *)&g_BattleSpecialSouthernCrossModelReq2)->modelGroup = Gfx_ModelTableGetRenderObject(0, g_BattleSpecialSouthernCrossLoadedRes);
    if (frame.fallingSlot->frameCounter < 22) {
        frame.rampValue = 8;
    } else {
        frame.rampValue = 0x1E - frame.fallingSlot->frameCounter;
    }
    frame.rampLevel = *(short *)&frame.rampValue;
    frame.posX = SinFixed(frame.fallingSlot->angle) * frame.fallingSlot->radius >> 12;
    frame.posZ = CosFixed(frame.fallingSlot->angle) * frame.fallingSlot->radius >> 12;
    frame.posX += frame.fallingSlot->velocityX;
    frame.posZ += frame.fallingSlot->velocityZ;
    frame.posY = frame.fallingSlot->velocityY;
    frame.angleX.vx = frame.fallingSlot->frameCounter << 8;
    frame.angleX.vy = frame.fallingSlot->frameCounter << 9;
    frame.angleX.vz = 0;
    RotMatrix(&frame.angleX, &frame.rotMatrix);
    frame.scaleX.vz = 0x1000 /* 1.0 */;
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
    g_BattleSpecialSouthernCrossDrawListHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecialSouthernCrossModelReq2, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialSouthernCrossDrawListHandle);
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.fallingSlot->velocityY += 133;
    frame.fallingSlot->angle += (short)(0x40 - (frame.fallingSlot->spinRate << 7));
    (frame.fallingSlot->frameCounter)++;
    if (frame.fallingSlot->frameCounter == 30) {
        frame.spawnSlot = (BattleFxPositionedSpriteSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialSouthernCrossExpandingSpinModelsTask);
        frame.spawnSlot->posX = frame.posX;
        frame.spawnSlot->posY = frame.posY;
        frame.spawnSlot->posZ = frame.posZ;
        frame.fallingSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Task to render expanding spinning models for the Sazan special effect.
 */
// FUNCTION: FF7 0x00545e8c
void __cdecl BattleFx_SpecialSouthernCrossExpandingSpinModelsTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[80];
        struct { unsigned char pad_to_4[0x4]; SVECTOR angleX; };
        struct { unsigned char pad_to_C[0xC]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_2C[0x2C]; BattleFxOilTargetModelSlot *expandingSlot; };
        struct { unsigned char pad_to_30[0x30]; PsxMatrix combinedMatrix; };
    } frame;
#pragma pack(pop)

    frame.expandingSlot = (BattleFxOilTargetModelSlot *)BATTLE_CURRENT_TASK_SLOT;
    frame.angleX.vx = 0;
    frame.angleX.vy = frame.expandingSlot->frameCounter << 8;
    frame.angleX.vz = 0;
    RotMatrix(&frame.angleX, &frame.rotMatrix);
    g_BattleSpecialSouthernCrossScaleZ = (frame.expandingSlot->frameCounter << 9) + 0x1000;
    g_BattleSpecialSouthernCrossScaleY = g_BattleSpecialSouthernCrossScaleZ;
    g_BattleSpecialSouthernCrossScaleX = g_BattleSpecialSouthernCrossScaleY;
    ScaleMatrix(&frame.rotMatrix, &g_BattleSpecialSouthernCrossScaleX);
    frame.rotMatrix.t[0] = frame.expandingSlot->posX;
    frame.rotMatrix.t[1] = frame.expandingSlot->posY;
    frame.rotMatrix.t[2] = frame.expandingSlot->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.combinedMatrix);
    SetRotMatrix(&frame.combinedMatrix);
    SetTransMatrix(&frame.combinedMatrix);
    ((BattleFxModelDraw *)&g_BattleSpecialSouthernCrossModelReq1)->fadeLevel = 128 - frame.expandingSlot->frameCounter * 6;
    ((BattleFxModelDraw *)&g_BattleSpecialSouthernCrossModelReq1)->flags = 0x28;
    ((BattleFxModelDraw *)&g_BattleSpecialSouthernCrossModelReq1)->modelGroup = Gfx_ModelTableGetRenderObject(2, g_BattleSpecialSouthernCrossLoadedRes);
    g_BattleSpecialSouthernCrossDrawListHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecialSouthernCrossModelReq1, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialSouthernCrossDrawListHandle);
    ((BattleFxModelDraw *)&g_BattleSpecialSouthernCrossModelReq1)->modelGroup = Gfx_ModelTableGetRenderObject(1, g_BattleSpecialSouthernCrossLoadedRes);
    g_BattleSpecialSouthernCrossDrawListHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecialSouthernCrossModelReq1, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialSouthernCrossDrawListHandle);
    if (g_BattleIsPaused != 0) {
        return;
    }
    (frame.expandingSlot->frameCounter)++;
    if (frame.expandingSlot->frameCounter == 20) {
        frame.expandingSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}
} /* extern "C" */
