// src/ff7/src/battle/battle_fx_special_fire_shell.cpp
/* Enemy Special Effect [idx 58]: FireShell (JP: kaen2 / 火炎2 (Flame 2), asset: "SPECIAL/KAEN2") — Fire Shell (0x273, 0x2c0, Edgehead / Dual Horn), Gush (0x2ba), S-Mine (0x384, Elena) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle special effect 'Kaen2' (flame): a per-target timeline with variant sprites, a
//   bouncing sprite and a projectile.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/api/battle.h"
#include "ff7/battle_tasks.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_special.h"
#include "lib/types/graphics.h"
#include "ff7/types/battle_3d.h"
#include "ff7/camera_script.h"
#include <string.h>
#include <stdlib.h>
#include "lib/codegen.h"
extern "C" {

struct BattleFxModelDraw;

// clang-format off
// GLOBAL: FF7 0x008a2930
int g_BattleSpecFireShellResourceDefs = 0x001900B1;
int g_BattleSpecFireShellResourceDefsHeadPtr[1] = {
    (int)((char *)&g_BattleFxSpecialFireShellResourceNames + 0x0)
};
int g_BattleSpecFireShellResourceDefsDataTail[64] = {
    0x0, 0x0, 0x0, 0x1900b1, (int)((char *)&g_BattleFxSpecialFireShellResourceNames + 0xc), 0x0,
    0x0, 0x0, 0x1900b1, (int)((char *)&g_BattleFxSpecialFireShellResourceNames + 0x18), 0x0, 0x0,
    0x0, 0x1900b1, (int)((char *)&g_BattleFxSpecialFireShellResourceNames + 0x24), 0x0, 0x0, 0x0,
    0x1900b1, (int)((char *)&g_BattleFxSpecialFireShellResourceNames + 0x30), 0x0, 0x0, 0x0, 0x1900b1,
    (int)((char *)&g_BattleFxSpecialFireShellResourceNames + 0x3c), 0x0, 0x0, 0x0, 0x1900b1, (int)((char *)&g_BattleFxSpecialFireShellResourceNames + 0x48),
    0x0, 0x0, 0x0, 0x1900b1, (int)((char *)&g_BattleFxSpecialFireShellResourceNames + 0x54), 0x0,
    0x0, 0x0, 0x192a39, (int)((char *)&g_BattleFxSpecialFireShellResourceNames2 + 0x0), (int)((char *)&g_BattleFxSpecialFireShellResourceNames2 + 0xc), 0x0,
    0x0, 0x192a39, (int)((char *)&g_BattleFxSpecialFireShellResourceNames2 + 0x18), (int)((char *)&g_BattleFxSpecialFireShellResourceNames2 + 0x24), 0x0, 0x0,
    0x192a39, (int)((char *)&g_BattleFxSpecialFireShellResourceNames2 + 0x30), (int)((char *)&g_BattleFxSpecialFireShellResourceNames2 + 0x3c), 0x0, 0x0, 0x192a39,
    (int)((char *)&g_BattleFxSpecialFireShellResourceNames2 + 0x48), (int)((char *)&g_BattleFxSpecialFireShellResourceNames2 + 0x54), 0x0, 0x0, 0x192a39, (int)((char *)&g_BattleFxSpecialFireShellResourceNames2 + 0x60),
    (int)((char *)&g_BattleFxSpecialFireShellResourceNames2 + 0x6c), 0x0, 0x0, 0x0
};
// GLOBAL: FF7 0x008a2a38
struct BattleSpecFireballSpriteReq1 g_BattleSpecFireShellSpriteReq1 = {0, 0, {0x80, 0x80, 0x80, 0x2c}, 0, {0}, 0, 0};
// GLOBAL: FF7 0x008a2a50
struct BattleSpecFireballSpriteReq2 g_BattleSpecFireShellSpriteReq2 = {-128, -128, -128, 44, 0, {0}};
// GLOBAL: FF7 0x008a2a58
int g_BattleSpecFireShellScaleParam = 64512;
// GLOBAL: FF7 0x008a2a60
struct BattleSpecFireShellModelParams g_BattleSpecFireShellModelParams = {{0, 8, 0, 0, 0, 0xc, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0}, 0, {0x28, 0, 0, 0, 0, 0, 0x80, 0, 0x20, 0, 0, 0}, 0, 0};
unsigned char g_rawData_008a2a88[8] = {0x80, 0x80, 0x80, 0x2e, 0x00, 0x00, 0x00, 0x00};
unsigned char g_BattleFxSpecialFireShellResourceNames[96] =
    "SOTO1.RSD\0\0\0"
    "SOTO2.RSD\0\0\0"
    "SOTO3.RSD\0\0\0"
    "SOTO4.RSD\0\0\0"
    "SOTO5.RSD\0\0\0"
    "SOTO6.RSD\0\0\0"
    "SOTO7.RSD\0\0\0"
    "SOTO8.RSD";
unsigned char g_BattleFxSpecialFireShellResourceNames2[120] =
    "KAEN202.TIM\0"
    "HASSYA1.SPT\0"
    "KAEN202.TIM\0"
    "HIKARI1.SPT\0"
    "KAEN202.TIM\0"
    "HIT1.SPT\0\0\0\0"
    "KAEN203.TIM\0"
    "BAKU1.SPT\0\0\0"
    "KAEN202.TIM\0"
    "HAHEN.SPT";
// clang-format on
#pragma intrinsic(memset)
#pragma intrinsic(memcpy)


// clang-format off
// GLOBAL: FF7 0x00b64fc4
int g_BattleSpecFireShellPacketCursor = 0;
// GLOBAL: FF7 0x00b64fc8
struct GfxTableCursor *g_BattleSpecFireShellLoadedResTable = 0;
// clang-format on

/*
 * When the engine context exists and g_BattleSpecFireShellLoadedResTable is empty, builds SPECIAL/KAEN2 with a default-
 * duration record and creates resource type 13 using g_BattleSpecFireShellResourceDefs.
 */
// FUNCTION: FF7 0x00535c30
void __cdecl BattleFx_SpecialLoadResFireShell(void)
{
    GfxModelTableBank *curOwnerCtx;
    GfxResourceRequest reqBuffer;
    int kind;
    char pathText[200];
    GfxResourceRequest *pRequest;
    int resourceDefsWord;

    if (g_pBattle3dScene != 0 && g_BattleSpecFireShellLoadedResTable == 0) {
        kind = 0xD;
        resourceDefsWord = (int)&g_BattleSpecFireShellResourceDefs;
        curOwnerCtx = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRequest = &reqBuffer;
        Gfx_InitResourceRequest(pRequest);
        pRequest->groupTag = 0;
        // STRING: FF7 0x008a2b68
        pRequest->resourcePath = BattleFx_BuildResourcePath("SPECIAL/KAEN2", pathText);
        Gfx_LoadModelTableIfAbsent(kind, resourceDefsWord, pRequest, &g_BattleSpecFireShellLoadedResTable, curOwnerCtx);
    }
}

/*
 * Forwards both arguments to BattleFx_SpecialBeginFireShell.
 */
// FUNCTION: FF7 0x00535cc0
void __cdecl BattleFx_SpecialEnterFireShell(int targetMask, int actorIdx)
{
    BattleFx_SpecialBeginFireShell(targetMask, actorIdx);
}

/*
 * Loads SPECIAL/KAEN2 through BattleFx_SpecialLoadResFireShell and creates a mode-four actor-mask callback task using
 * both arguments and BattleFx_SpecialSpawnFireShellPerTarget.
 */
// FUNCTION: FF7 0x00535cd5
void __cdecl BattleFx_SpecialBeginFireShell(int targetMask, int actorIdx)
{
    BattleFx_SpecialLoadResFireShell();
    Battle_CreateBitPatternCallbackTask(targetMask, actorIdx, 4, BattleFx_SpecialSpawnFireShellPerTarget);
}

/*
 * Allocates BattleFx_SpecialFireShellTimelineTask with the source actor's bone33 position and the target actor's
 * targetBone position, then stores the target and source actors in targetActor and sourceActor.
 */
// FUNCTION: FF7 0x00535cf6
void __cdecl BattleFx_SpecialSpawnFireShellPerTarget(int actor, int sourceActor)
{
    BattleFxBoulderThrowTask *taskPtr;

    taskPtr = (BattleFxBoulderThrowTask *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialFireShellTimelineTask);
    Battle_GetBoneWorldPos(sourceActor, g_BattleActorRecords[sourceActor].bone33, (short *)((char *)taskPtr + 4));
    Battle_GetBoneWorldPos(actor, g_BattleActorRecords[actor].targetBone, (short *)((char *)taskPtr + 0xC));
    taskPtr->targetActor = actor;
    taskPtr->sourceActor = sourceActor;
}

/*
 * Master timeline task that fires flame projectiles and impact bursts for Kaen2.
 */
// FUNCTION: FF7 0x00535d77
void __cdecl BattleFx_SpecialFireShellTimelineTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[72];
        SVECTOR angleX;
        struct { unsigned char pad_to_C[0xC]; SVECTOR dirX; };
        struct { unsigned char pad_to_14[0x14]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_34[0x34]; BattleFxFireballParticleRecord *spawnSlot; };
        struct { unsigned char pad_to_3C[0x3C]; short spawnIndex; };
        struct { unsigned char pad_to_44[0x44]; BattleFxFireShellTimelineSlot *effectSlot; };
    } frame;
#pragma pack(pop)

    frame.effectSlot = (BattleFxFireShellTimelineSlot *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.effectSlot->frameCounter < 12 && (frame.effectSlot->frameCounter % 4) == 0) {
        frame.spawnSlot = (BattleFxFireballParticleRecord *)&g_BattleTaskSlotBuffer.secondarySlots[(short)Battle_AllocateSecondaryResourceSlot(BattleFx_SpecialFireShellVariantSpriteTask)];
        frame.spawnSlot->posX = frame.effectSlot->posX;
        frame.spawnSlot->posY = frame.effectSlot->posY;
        frame.spawnSlot->posZ = frame.effectSlot->posZ;
        frame.spawnSlot->variant = 2;
        frame.spawnSlot = (BattleFxFireballParticleRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialFireShellProjectileTask);
        frame.spawnSlot->posX = frame.effectSlot->posX;
        frame.spawnSlot->posY = frame.effectSlot->posY;
        frame.spawnSlot->posZ = frame.effectSlot->posZ;
        frame.spawnSlot->velocityX = (frame.effectSlot->velocityX - frame.effectSlot->posX) / 8;
        frame.spawnSlot->velocityY = (frame.effectSlot->velocityY - frame.effectSlot->posY) / 8;
        frame.spawnSlot->velocityZ = (frame.effectSlot->velocityZ - frame.effectSlot->posZ) / 8;
        Battle_PlaySfxPannedToActor(0x163, (short)frame.effectSlot->sourceActor, 0);
    }
    if (frame.effectSlot->frameCounter == 14) {
        for (frame.spawnIndex = 0; frame.spawnIndex < 8; frame.spawnIndex++) {
            frame.spawnSlot = (BattleFxFireballParticleRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialFireShellBouncingSpriteTask);
            frame.spawnSlot->posX = frame.effectSlot->velocityX + rand() % 400 + -200;
            frame.spawnSlot->posZ = frame.effectSlot->velocityZ + rand() % 400 + -200;
            frame.spawnSlot->posY = 0;
            BattleFx_MakeRandomConeVector(0xFFFFFF06, 100, (char *)frame.spawnSlot + 0xC);
            frame.spawnSlot->variant = rand() % 20;
        }
        Battle_PlaySfxPannedToActor(0x7C, (short)frame.effectSlot->actorIndex, 0);
    }
    if (frame.effectSlot->frameCounter > 11 && frame.effectSlot->frameCounter < 19) {
        for (frame.spawnIndex = 0; frame.spawnIndex < 2; frame.spawnIndex++) {
            frame.spawnSlot = (BattleFxFireballParticleRecord *)&g_BattleTaskSlotBuffer.secondarySlots[(short)Battle_AllocateSecondaryResourceSlot(BattleFx_SpecialFireShellVariantSpriteTask)];
            frame.spawnSlot->posX = frame.effectSlot->velocityX + rand() % 1200 + -600;
            frame.spawnSlot->posY = frame.effectSlot->velocityY + rand() % 1200 + -600;
            frame.spawnSlot->posZ = frame.effectSlot->velocityZ + rand() % 1200 + -600;
            frame.angleX.vx = rand() % 0x4B0 - 0x258;
            frame.angleX.vy = 0;
            frame.angleX.vz = rand() % 0x4B0 - 0x258;
            frame.dirX.vx = 0;
            frame.dirX.vy = 0xFF60;
            frame.dirX.vz = 0;
            RotMatrix(&frame.angleX, &frame.rotMatrix);
            TransformVectorToInt16(&frame.rotMatrix, &frame.dirX, (char *)frame.spawnSlot + 0xC);
            frame.spawnSlot->variant = 1;
        }
    }
    if (frame.effectSlot->frameCounter == 15) {
        Battle_PostActorHitOrHurtMessage(frame.effectSlot->actorIndex);
    }
    (frame.effectSlot->frameCounter)++;
    if (frame.effectSlot->frameCounter == 40) {
        frame.effectSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Draws a secondary-table slot using sprite fields ten, eleven or eight from the variant, with
 * scale 0x2000 for variant two and 0x3000 otherwise. When unpaused integrates XYZ and subtracts
 * each velocity divided by eight only for variant one; with a nonnull sprite advances and retires
 * at or above its header frame count.
 */
// FUNCTION: FF7 0x00536139
void __cdecl BattleFx_SpecialFireShellVariantSpriteTask(void)
{
    struct {
        int table[3];
        BattleFxBombBlastSprite *slot;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    frame.slot = (BattleFxBombBlastSprite *)&g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx];
    frame.table[0] = 0xA;
    frame.table[1] = 0xB;
    frame.table[2] = 8;
    BattleFx_InstallBillboardMatrix((char *)frame.slot + 4, (frame.slot->mode - 2 != 0 ? 0x1000 : 0) + 0x2000, 0);
    (g_BattleSpecFireShellSpriteReq1.spriteEntry) = Gfx_ModelTableGetLoadedRecord(frame.table[frame.slot->mode], g_BattleSpecFireShellLoadedResTable);
    g_BattleSpecFireShellSpriteReq1.spriteObject = Gfx_ModelTableGetSpriteObject(frame.table[frame.slot->mode], g_BattleSpecFireShellLoadedResTable);
    g_BattleSpecFireShellSpriteReq1.frameIndex = frame.slot->frameIndex;
    g_BattleOverlayDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecFireShellSpriteReq1, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused == 0) {
        if (frame.slot->mode == 1) {
            frame.slot->posX += frame.slot->velocityX;
            frame.slot->posY += frame.slot->velocityY;
            frame.slot->posZ += frame.slot->velocityZ;
            frame.slot->velocityX -= (short)(frame.slot->velocityX / 8);
            frame.slot->velocityY -= (short)(frame.slot->velocityY / 8);
            frame.slot->velocityZ -= (short)(frame.slot->velocityZ / 8);
        }
        if ((g_BattleSpecFireShellSpriteReq1.spriteEntry) != 0) {
            (frame.slot->frameIndex)++;
            if (frame.slot->frameIndex >= ((BattleFxSpriteGroup *)g_BattleSpecFireShellSpriteReq1.spriteEntry)->header->frameCount) {
                frame.slot->ownerSlot = 0xFFFF;
            }
        }
    }
}

/*
 * Updates bouncing ember physics and submits flame sprites for Kaen2.
 */
// FUNCTION: FF7 0x00536301
void __cdecl BattleFx_SpecialFireShellBouncingSpriteTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[92];
        int tmpScale;
        struct { unsigned char pad_to_4[0x4]; SVECTOR angleX; };
        struct { unsigned char pad_to_C[0xC]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_2C[0x2C]; short scale; };
        struct { unsigned char pad_to_30[0x30]; PsxMatrix scaleMatrix; };
        struct { unsigned char pad_to_50[0x50]; short axisIndex; };
        struct { unsigned char pad_to_54[0x54]; int depthOut; };
        struct { unsigned char pad_to_58[0x58]; BattleFxTailLaserBouncingSpriteSlot *effectSlot; };
    } frame;
#pragma pack(pop)

    frame.effectSlot = (BattleFxTailLaserBouncingSpriteSlot *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecFireShellSpriteReq1.drawBlock = Gfx_ModelTableGetLoadedRecord(12, g_BattleSpecFireShellLoadedResTable);
    g_BattleSpecFireShellSpriteReq1.handle = Gfx_ModelTableGetSpriteObject(12, g_BattleSpecFireShellLoadedResTable);
    for (frame.axisIndex = 0; frame.axisIndex < 3; frame.axisIndex++) {
        frame.scaleMatrix.m[2][frame.axisIndex] = 0;
        frame.scaleMatrix.m[1][frame.axisIndex] = 0;
        frame.scaleMatrix.m[0][frame.axisIndex] = 0;
    }
    if (frame.effectSlot->frameCounter < 12) {
        frame.tmpScale = 8;
    } else {
        frame.tmpScale = 0x14 - frame.effectSlot->frameCounter;
    }
    frame.scale = *(short *)&frame.tmpScale;
    frame.angleX.vy = 0;
    frame.angleX.vx = frame.angleX.vy;
    frame.angleX.vz = frame.effectSlot->angle;
    SetRotMatrix(&g_BattleCameraTransform);
    SetTransMatrix(&g_BattleCameraTransform);
    RotTransPoint((char *)frame.effectSlot + 4, frame.scaleMatrix.t, &frame.depthOut);
    frame.scaleMatrix.m[2][2] = frame.effectSlot->variant < 4 ? 0x1400 : 0x2000;
    frame.scaleMatrix.m[1][1] = frame.scaleMatrix.m[2][2];
    frame.scaleMatrix.m[0][0] = frame.scaleMatrix.m[1][1];
    RotMatrix(&frame.angleX, &frame.rotMatrix);
    ConcatMatrixIntoFirst(&frame.scaleMatrix, &frame.rotMatrix);
    SetRotMatrix(&frame.scaleMatrix);
    SetTransMatrix(&frame.scaleMatrix);
    g_BattleSpecFireShellSpriteReq2.frame = frame.effectSlot->variant;
    g_BattleSpecFireShellSpriteReq2.code = (frame.scale - 8 != 0 ? 2 : 0) + 0x2C;
    g_BattleSpecFireShellSpriteReq2.blue = frame.scale << 4;
    g_BattleSpecFireShellSpriteReq2.green = g_BattleSpecFireShellSpriteReq2.blue;
    (g_BattleSpecFireShellSpriteReq2.red) = g_BattleSpecFireShellSpriteReq2.green;
    g_BattleOverlayDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecFireShellSpriteReq1.drawBlock, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.effectSlot->posX += frame.effectSlot->velocityX;
    frame.effectSlot->posY += frame.effectSlot->velocityY;
    frame.effectSlot->posZ += frame.effectSlot->velocityZ;
    if (frame.effectSlot->posY > 0) {
        frame.effectSlot->posY = 0;
        frame.effectSlot->velocityY *= -1;
    }
    frame.effectSlot->velocityY += 20;
    frame.effectSlot->angle += 0x200 /* 45 deg */;
    (frame.effectSlot->frameCounter)++;
    if (frame.effectSlot->frameCounter == 20) {
        frame.effectSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Advances a Kaen2 flame projectile toward its target and spawns impact embers on arrival.
 */
// FUNCTION: FF7 0x00536595
void __cdecl BattleFx_SpecialFireShellProjectileTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[144];
        PsxMatrix modelMatrix;
        struct { unsigned char pad_to_20[0x20]; BattleFxEnergyWaveSpinSlot *childSlot; };
        struct { unsigned char pad_to_24[0x24]; int stepIdx0; };
        struct { unsigned char pad_to_28[0x28]; int stepIdx1; };
        struct { unsigned char pad_to_2C[0x2C]; int stepIdx2; };
        struct { unsigned char pad_to_30[0x30]; int stepIdx3; };
        struct { unsigned char pad_to_34[0x34]; int stepIdx4; };
        struct { unsigned char pad_to_38[0x38]; int stepIdx5; };
        struct { unsigned char pad_to_3C[0x3C]; int stepIdx6; };
        struct { unsigned char pad_to_40[0x40]; int stepIdx7; };
        struct { unsigned char pad_to_4C[0x4C]; BattleFxHellBomberRocketSlot *slot; };
        struct { unsigned char pad_to_50[0x50]; PsxMatrix lookBasis; };
        struct { unsigned char pad_to_70[0x70]; PsxMatrix drawMatrix; };
    } frame;
#pragma pack(pop)

    frame.slot = (BattleFxHellBomberRocketSlot *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecFireShellModelParams.spriteEntry = Gfx_ModelTableGetLoadedRecord(9, g_BattleSpecFireShellLoadedResTable);
    g_BattleSpecFireShellModelParams.handle = Gfx_ModelTableGetSpriteObject(9, g_BattleSpecFireShellLoadedResTable);
    frame.stepIdx0 = 0;
    frame.stepIdx1 = 1;
    frame.stepIdx2 = 2;
    frame.stepIdx3 = 3;
    frame.stepIdx4 = 4;
    frame.stepIdx5 = 5;
    frame.stepIdx6 = 6;
    frame.stepIdx7 = 7;
    Gte_BuildLookBasisFromDirection((short *)&frame.slot->velocityX, frame.lookBasis.m[0]);
    RotMatrix((SVECTOR *)&g_BattleSpecFireShellScaleParam, &frame.modelMatrix);
    ConcatMatrixPreservingTranslation(&frame.lookBasis, &frame.modelMatrix, &frame.modelMatrix);
    ScaleMatrix(&frame.modelMatrix, (int *)&g_BattleSpecFireShellModelParams);
    frame.modelMatrix.t[0] = frame.slot->posX;
    frame.modelMatrix.t[1] = frame.slot->posY;
    frame.modelMatrix.t[2] = frame.slot->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.modelMatrix, &frame.drawMatrix);
    SetRotMatrix(&frame.drawMatrix);
    SetTransMatrix(&frame.drawMatrix);
    g_BattleSpecFireShellModelParams.drawBlock = Gfx_ModelTableGetRenderObject((&frame.stepIdx0)[frame.slot->frameCounter % 8], g_BattleSpecFireShellLoadedResTable);
    g_BattleSpecFireShellPacketCursor = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecFireShellModelParams.drawBlock, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecFireShellPacketCursor);
    BattleFx_InstallBillboardMatrix((char *)frame.slot + 4, 0x1000 /* 1.0 */, 0);
    g_BattleOverlayDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecFireShellModelParams.spriteEntry, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.slot->posX += frame.slot->velocityX;
    frame.slot->posY += frame.slot->velocityY;
    frame.slot->posZ += frame.slot->velocityZ;
    (frame.slot->frameCounter)++;
    if (frame.slot->frameCounter == 8) {
        frame.childSlot = (BattleFxEnergyWaveSpinSlot *)(&g_BattleTaskSlotBuffer.secondarySlots[(short)Battle_AllocateSecondaryResourceSlot(BattleFx_SpecialFireShellVariantSpriteTask)]);
        frame.childSlot->posX = frame.slot->posX + rand() % 800 + -400;
        frame.childSlot->posY = frame.slot->posY + rand() % 800 + -400;
        frame.childSlot->posZ = frame.slot->posZ + rand() % 800 + -400;
        frame.childSlot->spriteVariant = 0;
        frame.slot->ownerSlot = BATTLE_TASK_FREE;
    }
}
} /* extern "C" */
