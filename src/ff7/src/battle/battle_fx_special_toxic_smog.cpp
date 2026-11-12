// src/ff7/src/battle/battle_fx_special_toxic_smog.cpp
/* Enemy Special Effect [idx 20]: ToxicSmog (JP: osen / 汚染 (Pollution/Contamination), asset: "SPECIAL/OSEN") — Isogin Smog (0x292, Acrophies), Refuse (0x17e, Brain Pod) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle special effect for the Osen attack: loads its resources and drives a toxic cloud
//   timeline with drifting and explosion smoke sprites, spinning particles, wavy ribbon geometry
//   and expanding cloud models.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/api/battle.h"
#include "ff7/battle_tasks.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "ff7/types/battle.h"
#include "lib/types/graphics.h"
#include "ff7/types/battle_special.h"
#include "ff7/camera_script.h"
#include "ff7/types/battle_3d.h"
#include <string.h>
#include <stdlib.h>
#include "lib/codegen.h"
extern "C" {

struct GfxPsxSpriteQuadState;
struct BattleFxModelDraw;
struct Vec3sPacked;
#pragma intrinsic(memset)
#pragma intrinsic(memcpy)

// clang-format off
// GLOBAL: FF7 0x008a6f18
int g_BattleSpecialToxicSmogResourceDefs = 0x001940B1;
char *g_BattleFxSpecialToxicSmogResourceNames[1] = {
    // STRING: FF7 0x008a7008
    "OKURI2.RSD"
};
unsigned int g_BattleSpecialToxicSmogResourceDefsDataTail[34] = {
    0x00000000,
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x008a7014
    (unsigned int)"OSEN_A02.TIM",
    // STRING: FF7 0x008a7024
    (unsigned int)"KEMU2.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x008a7030
    (unsigned int)"OSEN_A03.TIM",
    // STRING: FF7 0x008a7040
    (unsigned int)"BAKU1.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    0x00000000,
    // STRING: FF7 0x008a704c
    (unsigned int)"JO_C01.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x008a7058
    (unsigned int)"OSEN_A02.TIM",
    // STRING: FF7 0x008a7068
    (unsigned int)"HIGE.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x008a7074
    (unsigned int)"OSEN_A01.TIM",
    // STRING: FF7 0x008a7084
    (unsigned int)"OSEN.SPT",
    0x00000000,
    0x00000000,
    0x00193a39,
    // STRING: FF7 0x008a7090
    (unsigned int)"OSEN_A02.TIM",
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000
};
// GLOBAL: FF7 0x008a6fa8
unsigned char g_BattleSpecialToxicSmogSpriteReq1[40] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x20, 0x20, 0x2c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x80, 0x80, 0x80, 0x2c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
// GLOBAL: FF7 0x008a6fd0
BattleSpecialToxicSmogColor1 g_BattleSpecialToxicSmogColor1 = {-128, -128, -128, {0x2e}, 0, 0};
// GLOBAL: FF7 0x008a6fe0
BattleSpecialToxicSmogColor2 g_BattleSpecialToxicSmogColor2 = {0x68, 0x68, 0x68, {0x2c}, 0, {0}};
// GLOBAL: FF7 0x008a6fe8
int g_BattleSpecialToxicSmogScale1 = 6144;
// GLOBAL: FF7 0x008a6fec
int g_BattleSpecialToxicSmogScale2 = 6144;
unsigned char g_rawData_008a6ff0[8] = {0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x008a6ff8
struct BattleSpecialToxicSmogModelReq1 g_BattleSpecialToxicSmogModelReq1 = {0, {0x28, 0, 0, 0, 0, 0}, 0};
unsigned char g_rawData_008a7004[4] = {0x20, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x00b6f068
short g_BattleSpecialToxicSmogState = 0;
// GLOBAL: FF7 0x00b6f06c
int g_BattleSpecialToxicSmogDrawListHandle = 0;
// GLOBAL: FF7 0x00b6f070
struct BattleSpecialToxicSmogRibbonBuffer g_BattleSpecialToxicSmogRibbonBuffer = {0};
// GLOBAL: FF7 0x00b6f098
struct GfxTableCursor *g_BattleSpecialToxicSmogLoadedRes = 0;
// clang-format on


/*
 * Loads and registers resources for the Osen special effect.
 */
// FUNCTION: FF7 0x00550cb0
void __cdecl BattleFx_SpecialLoadResToxicSmog(void)
{
    GfxModelTableBank *stageSceneHandle;
    GfxResourceRequest requestObj;
    int resourceCount;
    char pathWorkBuffer[200];
    GfxResourceRequest *reqPtrPtr;
    int resourceTable;

    if (g_pBattle3dScene != 0 && g_BattleSpecialToxicSmogLoadedRes == 0) {
        resourceCount = 7;
        resourceTable = (int)&g_BattleSpecialToxicSmogResourceDefs;
        stageSceneHandle = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        reqPtrPtr = &requestObj;
        Gfx_InitResourceRequest(reqPtrPtr);
        reqPtrPtr->groupTag = 0;
        // STRING: FF7 0x008a70a0
        reqPtrPtr->resourcePath = BattleFx_BuildResourcePath("SPECIAL/OSEN", pathWorkBuffer);
        Gfx_LoadModelTableIfAbsent(resourceCount, resourceTable, reqPtrPtr, &g_BattleSpecialToxicSmogLoadedRes, stageSceneHandle);
    }
}

/*
 * Entry point thunk for the Osen special effect.
 */
// FUNCTION: FF7 0x00550d40
void __cdecl BattleFx_SpecialEnterToxicSmog(int targetMask, int actorIdx)
{
    BattleFx_SpecialLoadResToxicSmog();
    g_BattleSpecialToxicSmogRibbonBuffer.ribbonIndex = 0;
    g_BattleSpecialToxicSmogState = 0;
    BattleFx_SpecialBeginToxicSmog(targetMask, actorIdx);
}

/*
 * Initializes resources and starts target tasks for Osen.
 */
// FUNCTION: FF7 0x00550d6c
void __cdecl BattleFx_SpecialBeginToxicSmog(int targetMask, int actorIdx)
{
    BattleFxToxicSmogTimelineSlot *effect;

    (g_BattleSpecialToxicSmogRibbonBuffer.targetMask) = targetMask;
    Battle_PlaySfxPannedToActor(0xBA, (short)actorIdx, 0);
    effect = (BattleFxToxicSmogTimelineSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialToxicSmogTimelineTask);
    Battle_GetBoneWorldPos(actorIdx, g_BattleActorRecords[actorIdx].bone34, &effect->startX);
    Battle_GetActorMaskCentroid(targetMask, &effect->endX);
}

/*
 * Main timeline task controlling Osen toxic cloud and ribbon emission.
 */
// FUNCTION: FF7 0x00550de3
void __cdecl BattleFx_SpecialToxicSmogTimelineTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[76];
        SVECTOR angleX;
        struct { unsigned char pad_to_8[0x8]; int transDepth; };
        struct { unsigned char pad_to_C[0xC]; SVECTOR jitterX; };
        struct { unsigned char pad_to_14[0x14]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_34[0x34]; BattleFxToxicSmogTimelineSpawnSlot *spawnSlot; };
        struct { unsigned char pad_to_3C[0x3C]; short actorIndex; };
        struct { unsigned char pad_to_40[0x40]; short randAngle; };
        struct { unsigned char pad_to_44[0x44]; BattleFxToxicSmogTimelineSlot *timelineSlot; };
    } frame;
#pragma pack(pop)

    frame.timelineSlot = (BattleFxToxicSmogTimelineSlot *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.timelineSlot->frameCounter < 0xC) {
        frame.spawnSlot = (BattleFxToxicSmogTimelineSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialToxicSmogDriftingRibbonSpriteTask);
        frame.spawnSlot->posX = frame.timelineSlot->startX;
        frame.spawnSlot->posY = frame.timelineSlot->startY;
        frame.spawnSlot->posZ = frame.timelineSlot->startZ;
        frame.randAngle = rand() & 0xFFF;
        frame.jitterX.vx = CosFixed(frame.randAngle) * 0x50 >> 0xC;
        frame.jitterX.vy = SinFixed(frame.randAngle) * 0x50 >> 0xC;
        frame.jitterX.vz = 0;
        frame.angleX.vz = 0;
        frame.angleX.vx = frame.angleX.vz;
        frame.angleX.vy = ArcTangentFixed(frame.timelineSlot->endX - frame.timelineSlot->startX, frame.timelineSlot->endZ - frame.timelineSlot->startZ);
        RotMatrix(&frame.angleX, &frame.rotMatrix);
        frame.rotMatrix.t[2] = 0;
        frame.rotMatrix.t[1] = frame.rotMatrix.t[2];
        frame.rotMatrix.t[0] = frame.rotMatrix.t[1];
        SetRotMatrix(&frame.rotMatrix);
        SetTransMatrix(&frame.rotMatrix);
        RotTransPointToInt16(&frame.jitterX, (short *)&frame.jitterX, &frame.transDepth);
        frame.spawnSlot->velocityX = (frame.timelineSlot->endX - frame.timelineSlot->startX) / 0x1E + frame.jitterX.vx;
        frame.spawnSlot->velocityY = (frame.timelineSlot->endY - frame.timelineSlot->startY) / 0x1E + frame.jitterX.vy;
        frame.spawnSlot->velocityZ = (frame.timelineSlot->endZ - frame.timelineSlot->startZ) / 0x1E + frame.jitterX.vz;
        frame.spawnSlot->phaseOrResIndex = 0x32;
    }
    if (frame.timelineSlot->frameCounter > 0x1E && frame.timelineSlot->frameCounter < 0x28) {
        for (frame.actorIndex = 0; frame.actorIndex < 0xA; frame.actorIndex++) {
            if (((g_BattleSpecialToxicSmogRibbonBuffer.targetMask) & 1 << frame.actorIndex) != 0) {
                frame.spawnSlot = (BattleFxToxicSmogTimelineSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialToxicSmogAnimatedSpriteTask);
                Battle_GetBoneWorldPos(frame.actorIndex, g_BattleActorRecords[frame.actorIndex].targetBone, &frame.spawnSlot->posX);
                frame.spawnSlot->posX += (short)(rand() % 0x320 - 0x190);
                frame.spawnSlot->posY += (short)(rand() % 0x320 - 0x190);
                frame.spawnSlot->posZ += (short)(rand() % 0x320 - 0x190);
                frame.spawnSlot = (BattleFxToxicSmogTimelineSpawnSlot *)&g_BattleTaskSlotBuffer.secondarySlots[(short)Battle_AllocateSecondaryResourceSlot(BattleFx_SpecialToxicSmogSecondaryAnimatedSpriteTask)];
                Battle_GetBoneWorldPos(frame.actorIndex, g_BattleActorRecords[frame.actorIndex].targetBone, &frame.spawnSlot->posX);
                frame.spawnSlot->posX += (short)(rand() % 0x4B0 - 0x258);
                frame.spawnSlot->posY += (short)(rand() % 0x4B0 - 0x258);
                frame.spawnSlot->posZ += (short)(rand() % 0x4B0 - 0x258);
            }
        }
    }
    if (frame.timelineSlot->frameCounter == 0x1E) {
        frame.spawnSlot = (BattleFxToxicSmogTimelineSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialToxicSmogSpinFadeSpriteTask);
        frame.spawnSlot->posX = 0;
        frame.spawnSlot->posY = 0;
        frame.spawnSlot->posZ = 0x7D0;
        frame.spawnSlot->velocityZ = 0;
        frame.spawnSlot->velocityY = 0;
        frame.spawnSlot->velocityX = 0;
    }
    if (frame.timelineSlot->frameCounter == 0x1E) {
        frame.spawnSlot = (BattleFxToxicSmogTimelineSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialToxicSmogSpinGrowFadeModelTask);
    }
    if (frame.timelineSlot->frameCounter == 0x50) {
        for (frame.actorIndex = 0; frame.actorIndex < 0xA; frame.actorIndex++) {
            if (((g_BattleSpecialToxicSmogRibbonBuffer.targetMask) & 1 << frame.actorIndex) != 0) {
                Battle_PostActorHitOrHurtMessage(frame.actorIndex);
            }
        }
    }
    (frame.timelineSlot->frameCounter)++;
    if (frame.timelineSlot->frameCounter == 0x64) {
        frame.timelineSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Animates primary drifting toxic smoke sprites for Osen.
 */
// FUNCTION: FF7 0x00551261
void __cdecl BattleFx_SpecialToxicSmogAnimatedSpriteTask(void)
{
    BattleTaskSlot *task;
    PsxMatrix *matrix;

    task = BATTLE_CURRENT_TASK_SLOT;
    ((BattleFxSpriteRequest *)g_BattleSpecialToxicSmogSpriteReq1)[0].spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(2, g_BattleSpecialToxicSmogLoadedRes);
    ((BattleFxSpriteRequest *)g_BattleSpecialToxicSmogSpriteReq1)[0].drawObj = Gfx_ModelTableGetSpriteObject(2, g_BattleSpecialToxicSmogLoadedRes);
    matrix = (PsxMatrix *)BattleFx_InstallBillboardMatrix((short *)task->data, 0x2000, 0);
    if (matrix->t[2] > 0) {
        ((BattleFxSpriteRequest *)g_BattleSpecialToxicSmogSpriteReq1)[0].frame = task->frameCounter;
        g_BattleSpecialToxicSmogDrawListHandle = BattleFx_SubmitSpriteDraw(&((BattleFxSpriteRequest *)g_BattleSpecialToxicSmogSpriteReq1)[0], g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialToxicSmogDrawListHandle);
    }
    if (g_BattleIsPaused == 0 && ((BattleFxSpriteRequest *)g_BattleSpecialToxicSmogSpriteReq1)[0].spriteGroup != 0) {
        (task->frameCounter)++;
        if (task->frameCounter == ((BattleFxSpriteGroup *)((BattleFxSpriteRequest *)g_BattleSpecialToxicSmogSpriteReq1)[0].spriteGroup)->header->frameCount) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Animates secondary toxic explosion sprites for Osen.
 */
// FUNCTION: FF7 0x0055133e
void __cdecl BattleFx_SpecialToxicSmogSecondaryAnimatedSpriteTask(void)
{
    BattleTaskSlot *task;

    task = (BattleTaskSlot *)(&g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx]);
    ((BattleFxSpriteRequest *)g_BattleSpecialToxicSmogSpriteReq1)[1].spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(3, g_BattleSpecialToxicSmogLoadedRes);
    ((BattleFxSpriteRequest *)g_BattleSpecialToxicSmogSpriteReq1)[1].drawObj = Battle3d_GetSceneRenderObject(3, g_pBattle3dScene);
    BattleFx_InstallBillboardMatrix((short *)task->data, 0x1000, 0);
    ((BattleFxSpriteRequest *)g_BattleSpecialToxicSmogSpriteReq1)[1].frame = task->frameCounter;
    g_BattleOverlayDrawListHead = BattleFx_SubmitSpriteDraw(&((BattleFxSpriteRequest *)g_BattleSpecialToxicSmogSpriteReq1)[1], g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused == 0 && ((BattleFxSpriteRequest *)g_BattleSpecialToxicSmogSpriteReq1)[1].spriteGroup != 0) {
        (task->frameCounter)++;
        if (task->frameCounter == ((BattleFxSpriteGroup *)((BattleFxSpriteRequest *)g_BattleSpecialToxicSmogSpriteReq1)[1].spriteGroup)->header->frameCount) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Animates spinning and fading particle sprites for Osen.
 */
// FUNCTION: FF7 0x0055140f
void __cdecl BattleFx_SpecialToxicSmogSpinFadeSpriteTask(void)
{
    BattleFxToxicSmogSpinFadeSprite *task;

    task = (BattleFxToxicSmogSpinFadeSprite *)BATTLE_CURRENT_TASK_SLOT;
    ((BattleFxSpriteRequest *)g_BattleSpecialToxicSmogSpriteReq1)[2].spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(5, g_BattleSpecialToxicSmogLoadedRes);
    ((BattleFxSpriteRequest *)g_BattleSpecialToxicSmogSpriteReq1)[2].drawObj = Gfx_ModelTableGetSpriteObject(5, g_BattleSpecialToxicSmogLoadedRes);
    BattleFx_SpecialToxicSmogInstallRotatedScaledMatrix(&task->pos, &task->rotX, 0x1000);
    g_BattleSpecialToxicSmogColor1.blue = g_BattleSpecialToxicSmogRibbonBuffer.ribbonIndex * 6;
    g_BattleSpecialToxicSmogColor1.green = g_BattleSpecialToxicSmogColor1.blue;
    g_BattleSpecialToxicSmogColor1.red = g_BattleSpecialToxicSmogColor1.green;
    g_BattleSpecialToxicSmogDrawListHandle = BattleFx_SubmitSpriteDraw(&((BattleFxSpriteRequest *)g_BattleSpecialToxicSmogSpriteReq1)[2], g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialToxicSmogDrawListHandle);
    if (g_BattleIsPaused != 0) {
        return;
    }
    task->angle += 0x10;
    if (task->age < 0x10) {
        g_BattleSpecialToxicSmogRibbonBuffer.ribbonIndex++;
    }
    if (task->age > 0x2C) {
        g_BattleSpecialToxicSmogRibbonBuffer.ribbonIndex--;
    }
    (task->age)++;
    if (task->age == 0x3C) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Calculates and installs rotated and scaled matrix for Osen primitives.
 */
// FUNCTION: FF7 0x0055153c
int __cdecl BattleFx_SpecialToxicSmogInstallRotatedScaledMatrix(Vec3sPacked *translation, void *rotation, short scale)
{
    int p; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    VECTOR scaleVector;

    RotMatrix((SVECTOR *)rotation, &g_BattleSpecialToxicSmogRibbonBuffer.rotMatrix);
    scaleVector.vz = scale;
    scaleVector.vy = scaleVector.vz;
    scaleVector.vx = scaleVector.vy;
    ScaleMatrix(&g_BattleSpecialToxicSmogRibbonBuffer.rotMatrix, &scaleVector);
    g_BattleSpecialToxicSmogRibbonBuffer.rotMatrix.t[0] = translation->vx;
    g_BattleSpecialToxicSmogRibbonBuffer.rotMatrix.t[1] = translation->vy;
    g_BattleSpecialToxicSmogRibbonBuffer.rotMatrix.t[2] = translation->vz;
    SetRotMatrix(&g_BattleSpecialToxicSmogRibbonBuffer.rotMatrix);
    SetTransMatrix(&g_BattleSpecialToxicSmogRibbonBuffer.rotMatrix);
    return (int)&g_BattleSpecialToxicSmogRibbonBuffer.rotMatrix;
}

/*
 * Task managing drifting ribbon trail segments for Osen.
 */
// FUNCTION: FF7 0x005515bf
void __cdecl BattleFx_SpecialToxicSmogDriftingRibbonSpriteTask(void)
{
    BattleFxToxicSmogDriftRibbon *pRibbon;
    int q; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    PsxMatrix *mtx;
    short fadeAlpha;
    int raw;

    pRibbon = (BattleFxToxicSmogDriftRibbon *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecialToxicSmogColor1.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(4, g_BattleSpecialToxicSmogLoadedRes);
    g_BattleSpecialToxicSmogColor1.drawObj = Gfx_ModelTableGetSpriteObject(4, g_BattleSpecialToxicSmogLoadedRes);
    if (pRibbon->frameCount < 0x1B) {
        raw = 8;
    } else {
        raw = 0x23 - pRibbon->frameCount;
    }
    fadeAlpha = raw;
    mtx = (PsxMatrix *)BattleFx_InstallBillboardMatrix(&pRibbon->posX, 0x1800, 0);
    RefreshGlobalTransformState();
    if (mtx->t[2] > 0) {
        BattleFx_SpecialToxicSmogDrawWavyRibbon(8, (short)pRibbon->wavePhase, 0x320, fadeAlpha << 2);
    }
    BattleFx_InstallBillboardMatrix(&pRibbon->posX, 0x1800, 0);
    g_BattleSpecialToxicSmogColor2.frame = pRibbon->frameCount % g_BattleSpecialToxicSmogColor1.spriteGroup->header->frameCount;
    g_BattleSpecialToxicSmogColor2.blue = fadeAlpha * 0xC;
    g_BattleSpecialToxicSmogColor2.green = g_BattleSpecialToxicSmogColor2.blue;
    g_BattleSpecialToxicSmogColor2.red = g_BattleSpecialToxicSmogColor2.green;
    g_BattleSpecialToxicSmogDrawListHandle = BattleFx_SubmitSpriteDraw(&g_BattleSpecialToxicSmogColor1.spriteGroup, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialToxicSmogDrawListHandle);
    pRibbon->wavePhase += 0x100;
    if (g_BattleIsPaused != 0) {
        return;
    }
    pRibbon->posX += pRibbon->velocityX;
    pRibbon->posY += pRibbon->velocityY;
    pRibbon->posZ += pRibbon->velocityZ;
    (pRibbon->frameCount)++;
    if (pRibbon->frameCount == 0x23) {
        pRibbon->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Generates and submits wavy ribbon vertex geometry for Osen.
 */
// FUNCTION: FF7 0x00551778
void __cdecl BattleFx_SpecialToxicSmogDrawWavyRibbon(int amplitude, short phase, int angleStep, int color)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[76];
        int quadState;
        struct { unsigned char pad_to_4[0x4]; int transformOutB; };
        struct { unsigned char pad_to_8[0x8]; int quadDepth; };
        struct { unsigned char pad_to_C[0xC]; short paletteWord; };
        struct { unsigned char pad_to_10[0x10]; BattleFxToxicSmogRibbonState *renderBlock; };
        struct { unsigned char pad_to_14[0x14]; SVECTOR endRightX; };
        struct { unsigned char pad_to_1C[0x1C]; SVECTOR endLeftX; };
        struct { unsigned char pad_to_24[0x24]; short segment; };
        struct { unsigned char pad_to_28[0x28]; SVECTOR startRightX; };
        struct { unsigned char pad_to_30[0x30]; SVECTOR startLeftX; };
        struct { unsigned char pad_to_38[0x38]; int packedColor; };
        struct { unsigned char pad_to_3C[0x3C]; short blockTag; };
        struct { unsigned char pad_to_40[0x40]; short sinEnd; };
        struct { unsigned char pad_to_44[0x44]; int transformOutA; };
        struct { unsigned char pad_to_48[0x48]; short sinStart; };
    } frame;
#pragma pack(pop)

    frame.packedColor = ((short)color) << 0x10 | 0x2E000000 | ((short)color) << 8 | ((short)color);
    frame.blockTag = 0;
    frame.paletteWord = 0x78C8;
    frame.renderBlock = (BattleFxToxicSmogRibbonState *)Gfx_GetPsxPacketScratchPtr();
    *(char *)((char *)frame.renderBlock + 7) |= 2;
    for (frame.segment = 0; frame.segment < 8; frame.segment++) {
        frame.sinStart = SinFixed(phase + frame.segment * ((short)angleStep)) * ((short)amplitude) >> 0xC;
        frame.sinEnd = SinFixed(phase + (frame.segment + 1) * ((short)angleStep)) * ((short)amplitude) >> 0xC;
        frame.startLeftX.vx = frame.sinStart - 0x3F;
        frame.startRightX.vx = frame.sinStart + 0x3F;
        frame.endLeftX.vx = frame.sinEnd - 0x3F;
        frame.endRightX.vx = frame.sinEnd + 0x3F;
        frame.startRightX.vy = ((frame.segment * 0x7F) / 8) - 0x3F;
        frame.startLeftX.vy = frame.startRightX.vy;
        frame.endRightX.vy = (((frame.segment + 1) * 0x7F) / 8) - 0x3F;
        frame.endLeftX.vy = frame.endRightX.vy;
        frame.startLeftX.vx <<= 3;
        frame.startRightX.vx <<= 3;
        frame.endLeftX.vx <<= 3;
        frame.endRightX.vx <<= 3;
        frame.startLeftX.vy <<= 3;
        frame.startRightX.vy <<= 3;
        frame.endLeftX.vy <<= 3;
        frame.endRightX.vy <<= 3;
        frame.endRightX.vz = 0;
        frame.endLeftX.vz = frame.endRightX.vz;
        frame.startRightX.vz = frame.endLeftX.vz;
        frame.startLeftX.vz = frame.startRightX.vz;
        frame.quadDepth = Gfx_TransformQuadReturnDepthAlt(&frame.startLeftX, &frame.startRightX, &frame.endLeftX, &frame.endRightX, &frame.transformOutA, &frame.transformOutB);
        if (frame.quadDepth > 0) {
            frame.renderBlock->tag = 0x9000000;
            frame.renderBlock->colorCmd = frame.packedColor;
            frame.renderBlock->u2 = 0x80;
            frame.renderBlock->u0 = 0x80;
            frame.renderBlock->u3 = 0xFF;
            frame.renderBlock->u1 = 0xFF;
            frame.renderBlock->v1 = (frame.segment * 0x7F) / 8;
            frame.renderBlock->v0 = frame.renderBlock->v1;
            frame.renderBlock->v3 = ((frame.segment + 1) * 0x7F) / 8;
            frame.renderBlock->v2 = frame.renderBlock->v3;
            frame.renderBlock->tpage = frame.blockTag;
            frame.renderBlock->clut = frame.paletteWord;
            frame.quadState = Gfx_ModelTableGetSpriteObject(6, g_BattleSpecialToxicSmogLoadedRes);
            if (frame.quadState != 0) {
                Gfx_SetPsxPrimQuadState(0xB, 1, (GfxPsxSpriteQuadState *)frame.quadState);
            }
            Gfx_DrawPsxPrimitive(frame.renderBlock);
        }
    }
    g_BattleSpecialToxicSmogDrawListHandle = (int)frame.renderBlock;
}

/*
 * Animates spinning and expanding cloud models for Osen.
 */
// FUNCTION: FF7 0x00551a1c
void __cdecl BattleFx_SpecialToxicSmogSpinGrowFadeModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[92];
        SVECTOR angleX;
        struct { unsigned char pad_to_8[0x8]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_30[0x30]; BattleTaskSlot *slot; };
    } frame;
#pragma pack(pop)

    frame.slot = BATTLE_CURRENT_TASK_SLOT;
    (g_BattleSpecialToxicSmogModelReq1.modelResField) = Gfx_ModelTableGetRenderObject(0, g_BattleSpecialToxicSmogLoadedRes);
    g_BattleSpecialToxicSmogScale2 = frame.slot->frameCounter * 0x12C + 0x2000;
    frame.angleX.vx = 0x420;
    frame.angleX.vy = frame.slot->frameCounter << 4;
    frame.angleX.vz = 0;
    RotMatrixXYZ(&frame.angleX, &frame.rotMatrix);
    ScaleMatrix(&frame.rotMatrix, &g_BattleSpecialToxicSmogScale1);
    frame.rotMatrix.t[0] = 0;
    frame.rotMatrix.t[1] = 0xFFFFFE70;
    frame.rotMatrix.t[2] = 0x1F40;
    SetRotMatrix(&frame.rotMatrix);
    SetTransMatrix(&frame.rotMatrix);
    g_BattleSpecialToxicSmogModelReq1.osenModelReq1StateShift = g_BattleSpecialToxicSmogState << 4;
    g_BattleSpecialToxicSmogDrawListHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecialToxicSmogModelReq1, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialToxicSmogDrawListHandle);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.slot->frameCounter < 8) {
        g_BattleSpecialToxicSmogState++;
    }
    if (frame.slot->frameCounter > 0x2A) {
        g_BattleSpecialToxicSmogState--;
    }
    (frame.slot->frameCounter)++;
    if (frame.slot->frameCounter == 0x32) {
        frame.slot->ownerSlot = BATTLE_TASK_FREE;
    }
}
} /* extern "C" */
