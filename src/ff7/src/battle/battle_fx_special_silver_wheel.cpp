// src/ff7/src/battle/battle_fx_special_silver_wheel.cpp
/* Enemy Special Effect [idx 14]: SilverWheel (JP: kiengiri / 気炎斬り (Chi Flame Slash), asset: "SPECIAL/KIENGIRI") — Silver Wheel (0x1dc, 0x1dd, Grangalan) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle special effect for the Kiengiri attack: loads resources, spawns per-target
//   timelines that generate slash waves, and drives the slash animation, scaled sprite placement,
//   hit sparks and a traveling blade model.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/api/battle.h"
#include "ff7/battle_tasks.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_special.h"
#include "ff7/camera_script.h"
#include "lib/types/graphics.h"
#include "ff7/types/battle_3d.h"
#include <string.h>
#include <stdlib.h>
#include "lib/codegen.h"
extern "C" {

struct BattleFxModelDraw;
// clang-format off
// GLOBAL: FF7 0x008a83b8
int g_BattleSpecialSilverWheelResourceDefs = 0x001900B1;
int g_BattleSpecialSilverWheelResourceDefsHeadPtr[1] = {
    (int)((char *)&g_BattleFxSpecialSilverWheelResourceNames + 0x0)
};
int g_BattleSpecialSilverWheelResourceDefsDataTail[38] = {
    0x0, 0x0, 0x0, 0x192a39, (int)((char *)&g_BattleFxSpecialSilverWheelResourceNames + 0xc), (int)((char *)&g_BattleFxSpecialSilverWheelResourceNames + 0x18),
    0x0, 0x0, 0x192a39, (int)((char *)&g_BattleFxSpecialSilverWheelResourceNames + 0x24), (int)((char *)&g_BattleFxSpecialSilverWheelResourceNames + 0x30), 0x0,
    0x0, 0x192a39, (int)((char *)&g_BattleFxSpecialSilverWheelResourceNames + 0x3c), (int)((char *)&g_BattleFxSpecialSilverWheelResourceNames + 0x48), 0x0, 0x0,
    0x192a39, (int)((char *)&g_BattleFxSpecialSilverWheelResourceNames + 0x54), (int)((char *)&g_BattleFxSpecialSilverWheelResourceNames + 0x60), 0x0, 0x0, 0x192a39,
    (int)((char *)&g_BattleFxSpecialSilverWheelResourceNames + 0x6c), (int)((char *)&g_BattleFxSpecialSilverWheelResourceNames + 0x78), 0x0, 0x0, 0x192a39, (int)((char *)&g_BattleFxSpecialSilverWheelResourceNames + 0x84),
    (int)((char *)&g_BattleFxSpecialSilverWheelResourceNames + 0x90), 0x0, 0x0, 0x192a39, (int)((char *)&g_BattleFxSpecialSilverWheelResourceNames + 0x9c), (int)((char *)&g_BattleFxSpecialSilverWheelResourceNames + 0xa8),
    0x0, 0x0
};
// GLOBAL: FF7 0x008a8458
struct BattleSpecialSilverWheelSpriteData g_BattleSpecialSilverWheelSpriteData = {0, 0, {0x60, 0x60, 0x60, 0x2c}, 0, {0}};
// GLOBAL: FF7 0x008a8470
unsigned char g_BattleSpecialSilverWheelColor2[8] = {0x80, 0x80, 0x80, 0x2c, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x00b72eb8
PsxMatrix g_BattleSpecialSilverWheelRotMatrix = {0};
// GLOBAL: FF7 0x00b72ed8
int g_BattleSpecialSilverWheelDrawListHandle = 0;
// GLOBAL: FF7 0x00b72edc
struct GfxTableCursor *g_BattleSpecialSilverWheelLoadedRes = 0;
// GLOBAL: FF7 0x00b72ee0
unsigned char g_BattleSpecialSilverWheelMatrix[0x4] = {0};
unsigned char g_bssPad_00b72ee4[4] = {0};
// GLOBAL: FF7 0x00b72ee8
short g_BattleSpecialSilverWheelMatrixScale = 0;
unsigned char g_bssPad_00b72eec[4] = {0};
// GLOBAL: FF7 0x00b72ef0
unsigned char g_BattleSpecialSilverWheelPoint[0x8] = {0};
unsigned char g_bssPad_00b72ef8[8] = {0};
// GLOBAL: FF7 0x00b72f00
short g_BattleSpecialSilverWheelStep = 0;
// clang-format on

// clang-format off
// GLOBAL: FF7 0x008a8478
struct BattleSpecialSilverWheelModelReq g_BattleSpecialSilverWheelModelReq = {0, 0, {0}, 0, {0x20, 0, 0, 0}};
unsigned char g_BattleFxSpecialSilverWheelResourceNames[180] =
    "KIEN.RSD\0\0\0\0"
    "KIEN00.TIM\0\0"
    "HIT11.SPT\0\0\0"
    "KIEN00.TIM\0\0"
    "HIT12.SPT\0\0\0"
    "KIEN00.TIM\0\0"
    "HIT21.SPT\0\0\0"
    "KIEN00.TIM\0\0"
    "HIT22.SPT\0\0\0"
    "KIEN01.TIM\0\0"
    "HIT31.SPT\0\0\0"
    "KIEN01.TIM\0\0"
    "HIT32.SPT\0\0\0"
    "KIEN01.TIM\0\0"
    "HASSYA.SPT";
// clang-format on

#pragma intrinsic(memset)
#pragma intrinsic(memcpy)


/*
 * Loads resources for the Kiengiri special effect when not already loaded.
 */
// FUNCTION: FF7 0x0055ba70
void __cdecl BattleFx_SpecialLoadResSilverWheel(void)
{
    GfxModelTableBank *value;
    GfxResourceRequest reqBuffer;
    int kind;
    char pathWorkBuffer[200];
    GfxResourceRequest *pRequest;
    int resourceDefsWord;

    if (g_pBattle3dScene != 0 && g_BattleSpecialSilverWheelLoadedRes == 0) {
        kind = 8;
        resourceDefsWord = (int)&g_BattleSpecialSilverWheelResourceDefs;
        value = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRequest = &reqBuffer;
        Gfx_InitResourceRequest(pRequest);
        pRequest->groupTag = 0;
        // STRING: FF7 0x008a853c
        pRequest->resourcePath = BattleFx_BuildResourcePath("SPECIAL/KIENGIRI", pathWorkBuffer);
        Gfx_LoadModelTableIfAbsent(kind, resourceDefsWord, pRequest, &g_BattleSpecialSilverWheelLoadedRes, value);
    }
}

/*
 * Main entry point for the Kiengiri special effect.
 */
// FUNCTION: FF7 0x0055bb00
void __cdecl BattleFx_SpecialEnterSilverWheel(int targetMask, int actorIdx)
{
    BattleFx_SpecialLoadResSilverWheel();
    g_BattleSpecialSilverWheelStep = 0;
    Battle_CreateBitPatternCallbackTask(targetMask, actorIdx, 4, BattleFx_SpecialSpawnSilverWheelPerTarget);
    Battle_PlaySfxPannedToActor(0x67, (short)actorIdx, 0x40);
}

/*
 * Spawns timeline task for a single Kiengiri target.
 */
// FUNCTION: FF7 0x0055bb3b
void __cdecl BattleFx_SpecialSpawnSilverWheelPerTarget(int actor, int actorIndex)
{
    int p; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int q; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxSilverWheelTimeline *taskPtr;

    taskPtr = (BattleFxSilverWheelTimeline *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialSilverWheelTimelineTask);
    Battle_GetBoneWorldPos(actorIndex, g_BattleActorRecords[actorIndex].bone33, (short *)((int)taskPtr + 4));
    Battle_GetBoneWorldPos(actor, g_BattleActorRecords[actor].targetBone, (short *)((int)taskPtr + 0xC));
    taskPtr->targetIndex = actor;
    taskPtr->actorIndex = actorIndex;
}

/*
 * Drives the main timeline and slash wave generation for Kiengiri.
 */
// FUNCTION: FF7 0x0055bbbe
void __cdecl BattleFx_SpecialSilverWheelTimelineTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[72];
        int projDepth;
        struct { unsigned char pad_to_4[0x4]; SVECTOR offsetX; };
        struct { unsigned char pad_to_C[0xC]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_2C[0x2C]; BattleFxSilverWheelTimelineSpawnSlot *spawnSlot; };
        struct { unsigned char pad_to_34[0x34]; short i; };
        struct { unsigned char pad_to_44[0x44]; BattleFxSilverWheelTimelineSlot *timelineSlot; };
    } frame;
#pragma pack(pop)

    frame.timelineSlot = (BattleFxSilverWheelTimelineSlot *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.timelineSlot->frameCounter == 0) {
        frame.spawnSlot = (BattleFxSilverWheelTimelineSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialSilverWheelAnimatedSpriteEntry7Task);
        frame.spawnSlot->velocityX = 0;
        frame.spawnSlot->velocityX = 0;
        frame.spawnSlot->heading = ArcTangentFixed(frame.timelineSlot->toX - frame.timelineSlot->fromX, frame.timelineSlot->toZ - frame.timelineSlot->fromZ);
        frame.offsetX.vy = 0;
        frame.offsetX.vx = frame.offsetX.vy;
        frame.offsetX.vz = 0xC8;
        RotMatrix((SVECTOR *)((int)frame.spawnSlot + 0xC), &frame.rotMatrix);
        frame.rotMatrix.t[2] = 0;
        frame.rotMatrix.t[1] = frame.rotMatrix.t[2];
        frame.rotMatrix.t[0] = frame.rotMatrix.t[1];
        SetRotMatrix(&frame.rotMatrix);
        SetTransMatrix(&frame.rotMatrix);
        RotTransPointToInt16(&frame.offsetX, (short *)&frame.offsetX, &frame.projDepth);
        frame.spawnSlot->posX = frame.timelineSlot->fromX + frame.offsetX.vx;
        frame.spawnSlot->posY = frame.timelineSlot->fromY + frame.offsetX.vy;
        frame.spawnSlot->posZ = frame.timelineSlot->fromZ + frame.offsetX.vz;
    }
    if (frame.timelineSlot->frameCounter == 2) {
        frame.spawnSlot = (BattleFxSilverWheelTimelineSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialSilverWheelTravelModelTask);
        frame.spawnSlot->posX = frame.timelineSlot->fromX;
        frame.spawnSlot->posY = frame.timelineSlot->fromY;
        frame.spawnSlot->posZ = frame.timelineSlot->fromZ;
        frame.spawnSlot->velocityX = (frame.timelineSlot->toX - frame.timelineSlot->fromX) / 0xC;
        frame.spawnSlot->heading = (frame.timelineSlot->toY - frame.timelineSlot->fromY) / 0xC;
        frame.spawnSlot->velocityZ = (frame.timelineSlot->toZ - frame.timelineSlot->fromZ) / 0xC;
        frame.spawnSlot->angle = ArcTangentFixed(frame.timelineSlot->toX - frame.timelineSlot->fromX, frame.timelineSlot->toZ - frame.timelineSlot->fromZ) + 0x400;
        frame.spawnSlot->casterIndex = frame.timelineSlot->casterIndex;
    }
    if (frame.timelineSlot->frameCounter == 0x10) {
        BattleFx_HitMarkSpawnScriptAtActorBone(frame.timelineSlot->actorIndex, 9, 0x1000);
        Battle_PostActorHitOrHurtMessage(frame.timelineSlot->actorIndex);
    }
    if (frame.timelineSlot->frameCounter > 0xA && frame.timelineSlot->frameCounter < 0x10) {
        for (frame.i = 0; frame.i < 2; frame.i++) {
            frame.spawnSlot = (BattleFxSilverWheelTimelineSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialSilverWheelVariantSpriteTask);
            Battle_GetBoneWorldPos(frame.timelineSlot->actorIndex, g_BattleActorRecords[frame.timelineSlot->actorIndex].targetBone, (short *)((int)frame.spawnSlot + 4));
            frame.spawnSlot->variantIndex = rand() % 6;
        }
    }
    (frame.timelineSlot->frameCounter)++;
    if (frame.timelineSlot->frameCounter == 0x28) {
        frame.timelineSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Updates and renders primary slash animation sprite for Kiengiri.
 */
// FUNCTION: FF7 0x0055bee1
void __cdecl BattleFx_SpecialSilverWheelAnimatedSpriteEntry7Task(void)
{
    int task;

    task = (int)BATTLE_CURRENT_TASK_SLOT;
    (g_BattleSpecialSilverWheelSpriteData.spriteResource) = Gfx_ModelTableGetLoadedRecord(7, g_BattleSpecialSilverWheelLoadedRes);
    g_BattleSpecialSilverWheelSpriteData.drawObj = Gfx_ModelTableGetSpriteObject(7, g_BattleSpecialSilverWheelLoadedRes);
    BattleFx_SpecialSilverWheelInstallScaledSpriteMatrix(task + 4, *(ShortQuad *)((char *)task + 0xC), 0x1600);
    g_BattleSpecialSilverWheelSpriteData.kiengiriSpriteArgWord = *(short *)((char *)task + 2);
    g_BattleSpecialSilverWheelDrawListHandle = BattleFx_SubmitSpriteDraw(&g_BattleSpecialSilverWheelSpriteData, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialSilverWheelDrawListHandle);
    if (g_BattleIsPaused == 0 && (g_BattleSpecialSilverWheelSpriteData.spriteResource) != 0) {
        (*(short *)((char *)task + 2))++;
        if (*(short *)((char *)task + 2) >= ((BattleFxSpriteGroup *)g_BattleSpecialSilverWheelSpriteData.spriteResource)->header->frameCount) {
            *(short *)task = 0xFFFF;
        }
    }
}

/*
 * Builds and sets scaled rotation and translation matrix for Kiengiri sprites.
 */
// FUNCTION: FF7 0x0055bfbb
int __cdecl BattleFx_SpecialSilverWheelInstallScaledSpriteMatrix(int srcVector, ShortQuad rotation, short scale)
{
    /* The output occupies the top DWORD of the original 20-byte frame. */
    int frame[5];

    SetRotMatrix(&g_BattleCameraTransform);
    SetTransMatrix(&g_BattleCameraTransform);
    RotTransPoint((void *)srcVector, (int *)((char *)&g_BattleSpecialSilverWheelPoint + 0x2), &frame[4]);
    *(short *)&g_BattleSpecialSilverWheelPoint = scale;
    g_BattleSpecialSilverWheelMatrixScale = *(short *)&g_BattleSpecialSilverWheelPoint;
    *(short *)&g_BattleSpecialSilverWheelMatrix = g_BattleSpecialSilverWheelMatrixScale;
    ConcatMatrixIntoFirst(&g_BattleSpecialSilverWheelMatrix, &g_BattleCameraTransform);
    RotMatrix((SVECTOR *)rotation.v, (int *)&g_BattleSpecialSilverWheelRotMatrix);
    ConcatMatrixIntoFirst(&g_BattleSpecialSilverWheelMatrix, &g_BattleSpecialSilverWheelRotMatrix);
    SetRotMatrix(&g_BattleSpecialSilverWheelMatrix);
    SetTransMatrix(&g_BattleSpecialSilverWheelMatrix);
    return (int)&g_BattleSpecialSilverWheelMatrix;
}

/*
 * Updates and renders variant hit spark sprites for Kiengiri.
 */
// FUNCTION: FF7 0x0055c06d
void __cdecl BattleFx_SpecialSilverWheelVariantSpriteTask(void)
{
    BattleFxFlyingSickleVariantSprite *task;
    int entryIds[6];

    task = (BattleFxFlyingSickleVariantSprite *)BATTLE_CURRENT_TASK_SLOT;
    entryIds[0] = 1;
    entryIds[1] = 2;
    entryIds[2] = 3;
    entryIds[3] = 4;
    entryIds[4] = 5;
    entryIds[5] = 6;
    BattleFx_InstallBillboardMatrix((void *)((int)task + 4), 0x3000, 0);
    *(short *)((char *)&g_BattleSpecialSilverWheelColor2 + 0x4) = task->frameIndex;
    ((BattleSpecSilverWheelSpriteData *)&g_BattleSpecialSilverWheelSpriteData)->frameTable = Gfx_ModelTableGetLoadedRecord(entryIds[task->variantIndex], g_BattleSpecialSilverWheelLoadedRes);
    ((BattleSpecSilverWheelSpriteData *)&g_BattleSpecialSilverWheelSpriteData)->variantField = Gfx_ModelTableGetSpriteObject(entryIds[task->variantIndex], g_BattleSpecialSilverWheelLoadedRes);
    g_BattleSpecialSilverWheelDrawListHandle = BattleFx_SubmitSpriteDraw((void *)(((int)&g_BattleSpecialSilverWheelSpriteData + 16)), g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialSilverWheelDrawListHandle);
    if (g_BattleIsPaused == 0 && ((BattleSpecSilverWheelSpriteData *)&g_BattleSpecialSilverWheelSpriteData)->frameTable != 0) {
        (task->frameIndex)++;
        if (task->frameIndex >= ((BattleFxSpriteGroup *)((BattleSpecSilverWheelSpriteData *)&g_BattleSpecialSilverWheelSpriteData)->frameTable)->header->frameCount) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Updates and renders traveling blade slash model for Kiengiri.
 */
// FUNCTION: FF7 0x0055c17d
void __cdecl BattleFx_SpecialSilverWheelTravelModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[72];
        int clampedCounter;
        struct { unsigned char pad_to_4[0x4]; SVECTOR rotX; };
        struct { unsigned char pad_to_C[0xC]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_30[0x30]; short progress; };
        struct { unsigned char pad_to_34[0x34]; BattleFxSilverWheelTravelModelSlot *travelSlot; };
        struct { unsigned char pad_to_38[0x38]; VECTOR scaleX; };
    } frame;
#pragma pack(pop)

    frame.travelSlot = (BattleFxSilverWheelTravelModelSlot *)BATTLE_CURRENT_TASK_SLOT;
    (g_BattleSpecialSilverWheelModelReq.modelResField) = Gfx_ModelTableGetRenderObject(0, g_BattleSpecialSilverWheelLoadedRes);
    if (frame.travelSlot->frameCounter < 0xC) {
        frame.clampedCounter = frame.travelSlot->frameCounter;
    } else {
        frame.clampedCounter = 0xC;
    }
    frame.progress = *(short *)&frame.clampedCounter;
    frame.rotX.vx = (frame.progress << 0xA) / 0xC + 0x400;
    frame.rotX.vy = frame.travelSlot->angleY;
    frame.rotX.vz = -frame.travelSlot->frameCounter << 9;
    RotMatrix(&frame.rotX, &frame.rotMatrix);
    frame.scaleX.vz = ((frame.progress << 8) + 0x800) * g_BattleActorRecords[frame.travelSlot->battleActorIndex].radius / 0x258;
    frame.scaleX.vy = frame.scaleX.vz;
    frame.scaleX.vx = frame.scaleX.vy;
    ScaleMatrix(&frame.rotMatrix, &frame.scaleX);
    frame.rotMatrix.t[0] = frame.travelSlot->posX;
    frame.rotMatrix.t[1] = frame.travelSlot->posY;
    frame.rotMatrix.t[2] = frame.travelSlot->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.rotMatrix);
    SetRotMatrix(&frame.rotMatrix);
    SetTransMatrix(&frame.rotMatrix);
    SetDrawColorRGB(0, 0, 0);
    g_BattleSpecialSilverWheelModelReq.stepOffset = g_BattleSpecialSilverWheelStep * 0x258;
    g_BattleSpecialSilverWheelModelReq.flags = (g_BattleSpecialSilverWheelStep != 0 ? 8 : 0) + 0x80;
    g_BattleSpecialSilverWheelDrawListHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecialSilverWheelModelReq, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialSilverWheelDrawListHandle);
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.travelSlot->posX += frame.travelSlot->velocityX;
    frame.travelSlot->posY += frame.travelSlot->velocityY;
    frame.travelSlot->posZ += frame.travelSlot->velocityZ;
    if (frame.travelSlot->frameCounter > 0xC) {
        g_BattleSpecialSilverWheelStep++;
    }
    (frame.travelSlot->frameCounter)++;
    if (frame.travelSlot->frameCounter == 0x12) {
        frame.travelSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}
} /* extern "C" */
