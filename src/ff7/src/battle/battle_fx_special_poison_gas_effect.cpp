// src/ff7/src/battle/battle_fx_special_poison_gas_effect.cpp
/* Enemy Special Attack [108]: Poison Gas (orig. SPECIAL/GAS) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle special effect 'Gas': loads its resources and runs a per-target timeline with
//   animated, bouncing and projectile sprite tasks.

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
// GLOBAL: FF7 0x0089e2d0
int g_BattleSpecPoisonGasResourceDefs = 0x001900B1;
int g_BattleSpecPoisonGasResourceDefsHeadPtr[1] = {
    (int)((char *)&g_BattleFxSpecialPoisonGasEffectResourceNames + 0x0)
};
int g_BattleSpecPoisonGasResourceDefsDataTail[64] = {
    0x0, 0x0, 0x0, 0x1900b1, (int)((char *)&g_BattleFxSpecialPoisonGasEffectResourceNames + 0xc), 0x0,
    0x0, 0x0, 0x1900b1, (int)((char *)&g_BattleFxSpecialPoisonGasEffectResourceNames + 0x18), 0x0, 0x0,
    0x0, 0x1900b1, (int)((char *)&g_BattleFxSpecialPoisonGasEffectResourceNames + 0x24), 0x0, 0x0, 0x0,
    0x1900b1, (int)((char *)&g_BattleFxSpecialPoisonGasEffectResourceNames + 0x30), 0x0, 0x0, 0x0, 0x1900b1,
    (int)((char *)&g_BattleFxSpecialPoisonGasEffectResourceNames + 0x3c), 0x0, 0x0, 0x0, 0x1900b1, (int)((char *)&g_BattleFxSpecialPoisonGasEffectResourceNames + 0x48),
    0x0, 0x0, 0x0, 0x1900b1, (int)((char *)&g_BattleFxSpecialPoisonGasEffectResourceNames + 0x54), 0x0,
    0x0, 0x0, 0x192a39, (int)((char *)&g_BattleFxSpecialPoisonGasEffectResourceNames2 + 0x0), (int)((char *)&g_BattleFxSpecialPoisonGasEffectResourceNames2 + 0xc), 0x0,
    0x0, 0x192a39, 0x0, (int)((char *)&g_BattleFxSpecialPoisonGasEffectResourceNames2 + 0x18), 0x0, 0x0,
    0x192a39, (int)((char *)&g_BattleFxSpecialPoisonGasEffectResourceNames2 + 0x24), (int)((char *)&g_BattleFxSpecialPoisonGasEffectResourceNames2 + 0x30), 0x0, 0x0, 0x192a39,
    (int)((char *)&g_BattleFxSpecialPoisonGasEffectResourceNames2 + 0x3c), (int)((char *)&g_BattleFxSpecialPoisonGasEffectResourceNames2 + 0x48), 0x0, 0x0, 0x192a39, (int)((char *)&g_BattleFxSpecialPoisonGasEffectResourceNames2 + 0x54),
    (int)((char *)&g_BattleFxSpecialPoisonGasEffectResourceNames2 + 0x60), 0x0, 0x0, 0x0
};
// GLOBAL: FF7 0x0089e3d8
struct BattleSpecPoisonGasSpriteReq1 g_BattleSpecPoisonGasSpriteReq1 = {0, 0, {0x80, 0x80, 0x80, 0x2c}, 0, {0}, 0, 0};
// GLOBAL: FF7 0x0089e3f0
struct BattleSpecPoisonGasSpriteReq2 g_BattleSpecPoisonGasSpriteReq2 = {-128, -128, -128, 44, 0, {0}};
// GLOBAL: FF7 0x0089e3f8
struct BattleSpecPoisonGasSpriteReq3 g_BattleSpecPoisonGasSpriteReq3 = {{0, 0xfc, 0, 0, 0, 0, 0, 0}, 0, {0x28, 0, 0, 0, 0, 0, 0x80, 0, 0x20, 0, 0, 0}, 0, 0};
unsigned char g_rawData_0089e418[8] = {0x80, 0x80, 0x80, 0x2e, 0x00, 0x00, 0x00, 0x00};
unsigned char g_BattleFxSpecialPoisonGasEffectResourceNames[96] =
    "SOTO1.RSD\0\0\0"
    "SOTO2.RSD\0\0\0"
    "SOTO3.RSD\0\0\0"
    "SOTO4.RSD\0\0\0"
    "SOTO5.RSD\0\0\0"
    "SOTO6.RSD\0\0\0"
    "SOTO7.RSD\0\0\0"
    "SOTO8.RSD";
unsigned char g_BattleFxSpecialPoisonGasEffectResourceNames2[108] =
    "GAS02.TIM\0\0\0"
    "HIKARI1.SPT\0"
    "JO_C05.SPT\0\0"
    "GAS03.TIM\0\0\0"
    "BAKU1.SPT\0\0\0"
    "GAS02.TIM\0\0\0"
    "HASSYA1.SPT\0"
    "GAS02.TIM\0\0\0"
    "HAHEN.SPT";
// clang-format on
#pragma intrinsic(memset)
#pragma intrinsic(memcpy)


// clang-format off
// GLOBAL: FF7 0x00b5dd5c
int g_BattleSpecPoisonGasDrawListHead = 0;
// GLOBAL: FF7 0x00b5dd60
struct GfxTableCursor *g_BattleSpecPoisonGasLoadedResTable = 0;
unsigned char g_bssPad_00b5dd64[4] = {0};
unsigned char g_bssPad_00b5dd68[4] = {0};
unsigned char g_bssPad_00b5dd6c[4] = {0};
unsigned char g_bssPad_00b5dd70[4] = {0};
unsigned char g_bssPad_00b5dd74[4] = {0};
unsigned char g_bssPad_00b5dd78[4] = {0};
unsigned char g_bssPad_00b5dd7c[4] = {0};
// clang-format on

/*
 * BattleFx_SpecialLoadResPoisonGas implementation.
 *
 * When the engine context exists and g_BattleSpecPoisonGasLoadedResTable is empty, builds SPECIAL/GAS with a default-
 * duration record and creates resource type 13 using g_BattleSpecPoisonGasResourceDefs.
 */
// FUNCTION: FF7 0x005246a0
void __cdecl BattleFx_SpecialLoadResPoisonGas(void)
{
    GfxModelTableBank *curOwnerCtx;
    GfxResourceRequest currentRecord;
    int kind;
    char outSize[200];
    GfxResourceRequest *pRequest;
    int resourceDefsWord;

    if (g_pBattle3dScene != 0 && g_BattleSpecPoisonGasLoadedResTable == 0) {
        kind = 0xD;
        resourceDefsWord = (int)&g_BattleSpecPoisonGasResourceDefs;
        curOwnerCtx = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRequest = &currentRecord;
        Gfx_InitResourceRequest(pRequest);
        pRequest->groupTag = 0;
        // STRING: FF7 0x0089e4ec
        pRequest->resourcePath = BattleFx_BuildResourcePath("SPECIAL/GAS", outSize);
        Gfx_LoadModelTableIfAbsent(kind, resourceDefsWord, pRequest, &g_BattleSpecPoisonGasLoadedResTable, curOwnerCtx);
    }
}

/*
 * Forwards both arguments unchanged to BattleFx_SpecialBeginPoisonGas; it adds nothing of its own.
 */
// FUNCTION: FF7 0x00524730
void __cdecl BattleFx_SpecialEnterPoisonGas(int targetMask, int actorIdx)
{
    BattleFx_SpecialBeginPoisonGas(targetMask, actorIdx);
}

/*
 * Calls BattleFx_SpecialLoadResPoisonGas and creates a mode-four actor-mask callback task using both arguments and
 * BattleFx_SpecialSpawnPoisonGasPerTarget.
 */
// FUNCTION: FF7 0x00524745
void __cdecl BattleFx_SpecialBeginPoisonGas(int targetMask, int actorIdx)
{
    BattleFx_SpecialLoadResPoisonGas();
    Battle_CreateBitPatternCallbackTask(targetMask, actorIdx, 4, BattleFx_SpecialSpawnPoisonGasPerTarget);
}

/*
 * Allocates BattleFx_SpecialPoisonGasTimelineTask with actor `sourceActor`'s bone selected at
 * g_BattleActorRecords[sourceActor].bone33 and actor `actor`'s bone selected at g_BattleActorRecords[actor].targetBone, stores actor at
 * word +0x1C, and dispatches (0x163, narrowed sourceActor, 0).
 */
// FUNCTION: FF7 0x00524766
void __cdecl BattleFx_SpecialSpawnPoisonGasPerTarget(int actor, int sourceActor)
{
    int slot;

    slot = (int)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialPoisonGasTimelineTask);
    Battle_GetBoneWorldPos(sourceActor, g_BattleActorRecords[sourceActor].bone33, (short *)(slot + 4));
    Battle_GetBoneWorldPos(actor, g_BattleActorRecords[actor].targetBone, (short *)(slot + 0xC));
    ((BattleFxPoisonGasTimeline *)slot)->actorIndex = actor;
    Battle_PlaySfxPannedToActor(0x163, (short)sourceActor, 0);
}

/*
 * BattleFx_SpecialPoisonGasTimelineTask task implementation.
 */
// FUNCTION: FF7 0x005247f0
void __cdecl BattleFx_SpecialPoisonGasTimelineTask(void)
{
    BattleFxPoisonGasTimeline *mainSelf;
    short i7;
    BattleFxPoisonGasParticleRecord *pParticle;
    int s; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int t; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int u; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int v; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int w; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int x; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int y; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int z; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int a; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int b; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int c; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int d; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int e; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int f; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int g; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */

    mainSelf = (BattleFxPoisonGasTimeline *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (mainSelf->phase == 0) {
        pParticle = (BattleFxPoisonGasParticleRecord *)&g_BattleTaskSlotBuffer.secondarySlots[(short)Battle_AllocateSecondaryResourceSlot(BattleFx_SpecialPoisonGasAnimatedSpriteTask)];
        pParticle->posX = mainSelf->startX;
        pParticle->posY = mainSelf->startY;
        pParticle->posZ = mainSelf->startZ;
        pParticle->countdown = 2;
        pParticle = (BattleFxPoisonGasParticleRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialPoisonGasProjectileTask);
        pParticle->posX = mainSelf->startX;
        pParticle->posY = mainSelf->startY;
        pParticle->posZ = mainSelf->startZ;
        pParticle->velocityX = (mainSelf->endX - mainSelf->startX) / 8;
        pParticle->velocityY = (mainSelf->endY - mainSelf->startY) / 8;
        pParticle->velocityZ = (mainSelf->endZ - mainSelf->startZ) / 8;
    }
    if (mainSelf->phase == 8) {
        for (i7 = 0; i7 < 12; i7++) {
            pParticle = (BattleFxPoisonGasParticleRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialPoisonGasBouncingSpriteTask);
            pParticle->posX = mainSelf->endX + rand() % 400 + -200;
            pParticle->posZ = mainSelf->endZ + rand() % 400 + -200;
            pParticle->posY = 0;
            BattleFx_MakeRandomConeVector(0xFFFFFF06, 100, (char *)pParticle + 0xC);
            pParticle->countdown = rand() % 20;
        }
        Battle_PlaySfxPannedToActor(0xFA, (short)mainSelf->actorIndex, 0);
    }
    if (mainSelf->phase > 11 && mainSelf->phase < 19) {
        pParticle = (BattleFxPoisonGasParticleRecord *)&g_BattleTaskSlotBuffer.secondarySlots[(short)Battle_AllocateSecondaryResourceSlot(BattleFx_SpecialPoisonGasAnimatedSpriteTask)];
        pParticle->posX = mainSelf->endX + rand() % 1200 + -600;
        pParticle->posY = mainSelf->endY + rand() % 1200 + -600;
        pParticle->posZ = mainSelf->endZ + rand() % 1200 + -600;
        pParticle->countdown = 1;
    }
    if (mainSelf->phase == 0xF) {
        Battle_PostActorHitOrHurtMessage(mainSelf->actorIndex);
    }
    (mainSelf->phase)++;
    if (mainSelf->phase == 0x2D) {
        mainSelf->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Draws a secondary-table slot using sprite field nine through eleven from its variant, with scale
 * 0x2000 for variant two and 0x3000 otherwise. Uses the stage selector for variant zero and
 * resource alternate fields otherwise; when unpaused with a nonnull sprite advances and retires at
 * or above its header frame count.
 */
// FUNCTION: FF7 0x00524aee
void __cdecl BattleFx_SpecialPoisonGasAnimatedSpriteTask(void)
{
    struct {
        int table[3];
        BattleSpecPoisonGasSprite *slot;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    frame.slot = (BattleSpecPoisonGasSprite *)&g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx];
    frame.table[0] = 9;
    frame.table[1] = 0xA;
    frame.table[2] = 0xB;
    BattleFx_InstallBillboardMatrix((char *)frame.slot + 4, (frame.slot->mode - 2 != 0 ? 0x1000 : 0) + 0x2000, 0);
    (g_BattleSpecPoisonGasSpriteReq1.spriteRes) = Gfx_ModelTableGetLoadedRecord(frame.table[frame.slot->mode], g_BattleSpecPoisonGasLoadedResTable);
    if (frame.slot->mode != 0) {
        g_BattleSpecPoisonGasSpriteReq1.drawObj = Gfx_ModelTableGetSpriteObject(frame.table[frame.slot->mode], g_BattleSpecPoisonGasLoadedResTable);
    } else {
        g_BattleSpecPoisonGasSpriteReq1.drawObj = Battle3d_GetSceneRenderObject(3, g_pBattle3dScene);
    }
    g_BattleSpecPoisonGasSpriteReq1.frame = frame.slot->frameIndex;
    g_BattleOverlayDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecPoisonGasSpriteReq1, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused == 0 && (g_BattleSpecPoisonGasSpriteReq1.spriteRes) != 0) {
        (frame.slot->frameIndex)++;
        if (frame.slot->frameIndex >= *(int *)((char *)*(int *)(g_BattleSpecPoisonGasSpriteReq1.spriteRes) + 4)) {
            frame.slot->ownerSlot = 0xFFFF;
        }
    }
}

/*
 * BattleFx_SpecialPoisonGasBouncingSpriteTask task implementation.
 */
// FUNCTION: FF7 0x00524c23
void __cdecl BattleFx_SpecialPoisonGasBouncingSpriteTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[92];
        int rampValue;
        struct { unsigned char pad_to_4[0x4]; SVECTOR rotX; };
        struct { unsigned char pad_to_C[0xC]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_2C[0x2C]; short rampShort; };
        struct { unsigned char pad_to_30[0x30]; PsxMatrix spriteMatrix; };
        struct { unsigned char pad_to_50[0x50]; short axisIndex; };
        struct { unsigned char pad_to_54[0x54]; int projDepth; };
        struct { unsigned char pad_to_58[0x58]; BattleFxPoisonGasBouncingSpriteSlot *bounceSlot; };
    } frame;
#pragma pack(pop)

    frame.bounceSlot = (BattleFxPoisonGasBouncingSpriteSlot *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecPoisonGasSpriteReq1.res16 = Gfx_ModelTableGetLoadedRecord(12, g_BattleSpecPoisonGasLoadedResTable);
    g_BattleSpecPoisonGasSpriteReq1.res20 = Gfx_ModelTableGetSpriteObject(12, g_BattleSpecPoisonGasLoadedResTable);
    for (frame.axisIndex = 0; frame.axisIndex < 3; frame.axisIndex++) {
        frame.spriteMatrix.m[2][frame.axisIndex] = 0;
        frame.spriteMatrix.m[1][frame.axisIndex] = 0;
        frame.spriteMatrix.m[0][frame.axisIndex] = 0;
    }
    if (frame.bounceSlot->frameCounter < 12) {
        frame.rampValue = 8;
    } else {
        frame.rampValue = 0x14 - frame.bounceSlot->frameCounter;
    }
    frame.rampShort = *(short *)&frame.rampValue;
    frame.rotX.vy = 0;
    frame.rotX.vx = frame.rotX.vy;
    frame.rotX.vz = frame.bounceSlot->phase;
    SetRotMatrix(&g_BattleCameraTransform);
    SetTransMatrix(&g_BattleCameraTransform);
    RotTransPoint((char *)frame.bounceSlot + 4, frame.spriteMatrix.t, &frame.projDepth);
    frame.spriteMatrix.m[2][2] = frame.bounceSlot->variant < 4 ? 0x1400 : 0x2000;
    frame.spriteMatrix.m[1][1] = frame.spriteMatrix.m[2][2];
    frame.spriteMatrix.m[0][0] = frame.spriteMatrix.m[1][1];
    RotMatrix(&frame.rotX, &frame.rotMatrix);
    ConcatMatrixIntoFirst(&frame.spriteMatrix, &frame.rotMatrix);
    SetRotMatrix(&frame.spriteMatrix);
    SetTransMatrix(&frame.spriteMatrix);
    g_BattleSpecPoisonGasSpriteReq2.frame = frame.bounceSlot->variant;
    g_BattleSpecPoisonGasSpriteReq2.code = (frame.rampShort - 8 != 0 ? 2 : 0) + 0x2C;
    g_BattleSpecPoisonGasSpriteReq2.blue = frame.rampShort << 4;
    g_BattleSpecPoisonGasSpriteReq2.green = g_BattleSpecPoisonGasSpriteReq2.blue;
    (g_BattleSpecPoisonGasSpriteReq2.red) = g_BattleSpecPoisonGasSpriteReq2.green;
    g_BattleOverlayDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecPoisonGasSpriteReq1.res16, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.bounceSlot->posX += frame.bounceSlot->velocityX;
    frame.bounceSlot->posY += frame.bounceSlot->velocityY;
    frame.bounceSlot->posZ += frame.bounceSlot->velocityZ;
    if (frame.bounceSlot->posY > 0) {
        frame.bounceSlot->posY = 0;
        frame.bounceSlot->velocityY *= -1;
    }
    frame.bounceSlot->velocityY += 20;
    frame.bounceSlot->phase += 0x200;
    (frame.bounceSlot->frameCounter)++;
    if (frame.bounceSlot->frameCounter == 20) {
        frame.bounceSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * BattleFx_SpecialPoisonGasProjectileTask task implementation.
 */
// FUNCTION: FF7 0x00524eb7
void __cdecl BattleFx_SpecialPoisonGasProjectileTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[160];
        PsxMatrix rotMatrix;
        struct { unsigned char pad_to_20[0x20]; BattleFxEnergyWaveSpinSlot *projectile; };
        struct { unsigned char pad_to_24[0x24]; int modelIdx0[8]; };
        struct { unsigned char pad_to_4C[0x4C]; BattleFxHellBomberRocketSlot *projSlot; };
        struct { unsigned char pad_to_50[0x50]; PsxMatrix lookBasis; };
        struct { unsigned char pad_to_70[0x70]; PsxMatrix combinedMatrix; };
        struct { unsigned char pad_to_90[0x90]; GfxDirectionVec3 scaleX; };
        struct { unsigned char pad_to_9C[0x9C]; int clearedWord; };
    } frame;
#pragma pack(pop)

    frame.projSlot = (BattleFxHellBomberRocketSlot *)BATTLE_CURRENT_TASK_SLOT;
    frame.scaleX.x = 0x800 /* 0.5 */;
    frame.scaleX.y = 0x1200 /* 1.125 */;
    frame.scaleX.z = 0x800 /* 0.5 */;
    memset(&frame.clearedWord, 0, 4);
    g_BattleSpecPoisonGasSpriteReq3.res24 = Gfx_ModelTableGetLoadedRecord(8, g_BattleSpecPoisonGasLoadedResTable);
    g_BattleSpecPoisonGasSpriteReq3.res28 = Gfx_ModelTableGetSpriteObject(8, g_BattleSpecPoisonGasLoadedResTable);
    frame.modelIdx0[0] = 0;
    frame.modelIdx0[1] = 1;
    frame.modelIdx0[2] = 2;
    frame.modelIdx0[3] = 3;
    frame.modelIdx0[4] = 4;
    frame.modelIdx0[5] = 5;
    frame.modelIdx0[6] = 6;
    frame.modelIdx0[7] = 7;
    Gte_BuildLookBasisFromDirection((short *)((char *)frame.projSlot + 0xC), frame.lookBasis.m[0]);
    RotMatrix((SVECTOR *)&g_BattleSpecPoisonGasSpriteReq3, &frame.rotMatrix);
    ConcatMatrixPreservingTranslation(&frame.lookBasis, &frame.rotMatrix, &frame.rotMatrix);
    ScaleMatrix(&frame.rotMatrix, &frame.scaleX);
    frame.rotMatrix.t[0] = frame.projSlot->posX;
    frame.rotMatrix.t[1] = frame.projSlot->posY;
    frame.rotMatrix.t[2] = frame.projSlot->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.combinedMatrix);
    SetRotMatrix(&frame.combinedMatrix);
    SetTransMatrix(&frame.combinedMatrix);
    g_BattleSpecPoisonGasSpriteReq3.res8 = Gfx_ModelTableGetRenderObject(frame.modelIdx0[((frame.projSlot->frameCounter) % 8)], g_BattleSpecPoisonGasLoadedResTable);
    g_BattleSpecPoisonGasDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecPoisonGasSpriteReq3.res8, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecPoisonGasDrawListHead);
    BattleFx_InstallBillboardMatrix((char *)frame.projSlot + 4, 0x1000 /* 1.0 */, 0);
    g_BattleOverlayDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecPoisonGasSpriteReq3.res24, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.projSlot->posX += frame.projSlot->velocityX;
    frame.projSlot->posY += frame.projSlot->velocityY;
    frame.projSlot->posZ += frame.projSlot->velocityZ;
    (frame.projSlot->frameCounter)++;
    if (frame.projSlot->frameCounter == 8) {
        frame.projectile = (BattleFxEnergyWaveSpinSlot *)(&g_BattleTaskSlotBuffer.secondarySlots[(short)Battle_AllocateSecondaryResourceSlot(BattleFx_SpecialPoisonGasAnimatedSpriteTask)]);
        frame.projectile->posX = frame.projSlot->posX + rand() % 800 + -400;
        frame.projectile->posY = frame.projSlot->posY + rand() % 800 + -400;
        frame.projectile->posZ = frame.projSlot->posZ + rand() % 800 + -400;
        frame.projectile->spriteVariant = 0;
        frame.projSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}
} /* extern "C" */
