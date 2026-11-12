// src/ff7/src/battle/battle_fx_special_blaster.cpp
/* Enemy Special Effect [idx 38]: Blaster (JP: b_2 / ブラスター2 (Blaster 2, BLASTE03.TIM), asset: "SPECIAL/B_2") — Blaster (0x2b8, Cuahl / Unknown) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle special effect for the B2 burst attack: loads its resources, spawns per-target
//   burst explosions, and drives a multi-stage blast with flare sprites, implosion, shockwave,
//   flash, trail and pulse models.

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

// GLOBAL: FF7 0x008a7920
unsigned int g_BattleSpecialBlasterResourceDefs[115] = {0x001900b1, (unsigned int)&g_BattleSpecialBlasterResFilenameWa, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleSpecialBlasterResFilenameSenkou1, 0x00000000, 0x00000000, 0x00000000, 0x001400b1, (unsigned int)((char *)&g_BattleSpecialBlasterResFilenameSenkou1 + 0xc), 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)((char *)&g_BattleSpecialBlasterResFilenameSenkou1 + 0x14), 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleSpecialBlasterResFilenameTama1, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)((char *)&g_BattleSpecialBlasterResFilenameTama1 + 0xc), 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleSpecialBlasterResFilenameMoto, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)((char *)&g_BattleSpecialBlasterResFilenameMoto + 0xc), 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleSpecialBlasterResFilenameNetu1, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)((char *)&g_BattleSpecialBlasterResFilenameNetu1 + 0xc), 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleSpecialBlasterResFilenameUchi1, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)((char *)&g_BattleSpecialBlasterResFilenameUchi1 + 0xc), 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleSpecialBlasterResFilenameUchi3, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)((char *)&g_BattleSpecialBlasterResFilenameUchi3 + 0xc), 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleSpecialBlasterResFilenameUchi5, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)((char *)&g_BattleSpecialBlasterResFilenameUchi5 + 0xc), 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleSpecialBlasterResFilenameUchi7, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)((char *)&g_BattleSpecialBlasterResFilenameUchi7 + 0xc), 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)&g_BattleSpecialBlasterResFilenameHikari1, 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)((char *)&g_BattleSpecialBlasterResFilenameHikari1 + 0xc), 0x00000000, 0x00000000, 0x00000000, 0x00192a39, (unsigned int)&g_BattleSpecialBlasterResFilenameBlaste03, (unsigned int)&g_BattleSpecialBlasterResFilenameKou, 0x00000000, 0x00000000, 0x00192a39, (unsigned int)&g_BattleSpecialBlasterResFilenameBlaste03b, (unsigned int)&g_BattleSpecialBlasterResFilenameBaku2, 0x00000000, 0x00000000, 0x00192a39, (unsigned int)((char *)&g_BattleSpecialBlasterResFilenameBaku2 + 0xc), (unsigned int)((char *)&g_BattleSpecialBlasterResFilenameBaku2 + 0x1c), 0x00000000, 0x00000000};
// GLOBAL: FF7 0x008a7af0
struct BattleLimitStardustRayModelReq2 g_BattleSpecialBlasterSpriteReq1 = {0, 0, {0x80, 0x80, 0x80, 0x2c}, 0, {0}};
// GLOBAL: FF7 0x008a7b00
int g_BattleSpecialBlasterModelReq1[4] = {0, 8, 0, 32};

// GLOBAL: FF7 0x008a7b10
int g_BattleSpecialBlasterModelReq2[4] = {0, 136, 0, 32};

// GLOBAL: FF7 0x008a7b20
struct BattleFxSpinModelReq g_BattleSpecialBlasterModelReq3 = {{0}, -128, -128, -128, {0x2e}, 0, {0}};

// GLOBAL: FF7 0x008a7b30
int g_BattleSpecialBlasterModelReq4[4] = {0, 168, 0, 32};
// GLOBAL: FF7 0x008a7b40
int g_BattleSpecialBlasterRotXYZ1 = 64512;
// GLOBAL: FF7 0x008a7b48
int g_BattleSpecialBlasterModelReq5[4] = {0, 168, 0, 32};

// GLOBAL: FF7 0x008a7b58
short g_BattleSpecialBlasterAlpha1 = 8;
// GLOBAL: FF7 0x008a7b60
int g_BattleSpecialBlasterModelReq6[4] = {0, 168, 8388608, 32};

// GLOBAL: FF7 0x008a7b70
struct SVECTOR g_BattleSpecialBlasterRotAngle1 = {-1024, 0, 0, 0};
// GLOBAL: FF7 0x008a7b78
short g_BattleSpecialBlasterAngles1[4] = {8, 0, 0, 0};
// GLOBAL: FF7 0x008a7b80
unsigned char g_BattleSpecialBlasterScale1[16] = {0x00, 0x30, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x008a7b90
int g_BattleSpecialBlasterModelReq7[4] = {0, 168, 8388608, 32};

// GLOBAL: FF7 0x008a7ba0
struct BattleFxBlasterModelReq g_BattleSpecialBlasterModelReq8 = {{0}, -128, -128, -128, {0x2e}, 0, {0}};

// GLOBAL: FF7 0x008a7bb0
int g_BattleSpecialBlasterRotXYZ2 = 64512;
// GLOBAL: FF7 0x008a7bb8
int g_BattleSpecialBlasterModelReq9[4] = {0, 168, 0, 32};
// GLOBAL: FF7 0x008a7bc8
int g_BattleSpecialBlasterModelReq10[4] = {0, 0, 780173440, 0};

// GLOBAL: FF7 0x008a7bd8
int g_BattleSpecialBlasterRotXYZ3 = 64512;
// GLOBAL: FF7 0x008a7be0
int g_BattleSpecialBlasterModelReq11[4] = {0, 168, 0, 32};

// GLOBAL: FF7 0x008a7bf0
unsigned char g_BattleSpecialBlasterResFilenameWa[7] = "WA.RSD";
// GLOBAL: FF7 0x008a7bf8
unsigned char g_BattleSpecialBlasterResFilenameSenkou1[32] =
    "SENKOU1.RSD\0"
    "ENE.RSD\0"
    "ENESOTO.RSD";
// GLOBAL: FF7 0x008a7c18
unsigned char g_BattleSpecialBlasterResFilenameTama1[22] =
    "TAMA1.RSD\0\0\0"
    "TAMA2.RSD";
// GLOBAL: FF7 0x008a7c30
unsigned char g_BattleSpecialBlasterResFilenameMoto[21] =
    "MOTO.RSD\0\0\0\0"
    "SOTO.RSD";
// GLOBAL: FF7 0x008a7c48
unsigned char g_BattleSpecialBlasterResFilenameNetu1[23] =
    "NETU1.RSD\0\0\0"
    "HONOU1.RSD";
// GLOBAL: FF7 0x008a7c60
unsigned char g_BattleSpecialBlasterResFilenameUchi1[22] =
    "UCHI1.RSD\0\0\0"
    "UCHI2.RSD";
// GLOBAL: FF7 0x008a7c78
unsigned char g_BattleSpecialBlasterResFilenameUchi3[22] =
    "UCHI3.RSD\0\0\0"
    "UCHI4.RSD";
// GLOBAL: FF7 0x008a7c90
unsigned char g_BattleSpecialBlasterResFilenameUchi5[22] =
    "UCHI5.RSD\0\0\0"
    "UCHI6.RSD";
// GLOBAL: FF7 0x008a7ca8
unsigned char g_BattleSpecialBlasterResFilenameUchi7[22] =
    "UCHI7.RSD\0\0\0"
    "UCHI8.RSD";
// GLOBAL: FF7 0x008a7cc0
unsigned char g_BattleSpecialBlasterResFilenameHikari1[24] =
    "HIKARI1.RSD\0"
    "HIKARI2.RSD";
// GLOBAL: FF7 0x008a7cd8
unsigned char g_BattleSpecialBlasterResFilenameBlaste03[13] = "BLASTE03.TIM";
// GLOBAL: FF7 0x008a7ce8
unsigned char g_BattleSpecialBlasterResFilenameKou[8] = "KOU.SPT";
// GLOBAL: FF7 0x008a7cf0
unsigned char g_BattleSpecialBlasterResFilenameBlaste03b[13] = "BLASTE03.TIM";
// GLOBAL: FF7 0x008a7d00
unsigned char g_BattleSpecialBlasterResFilenameBaku2[38] =
    "BAKU2.SPT\0\0\0"
    "BLASTE02.TIM\0\0\0\0"
    "BAKU1.SPT";
// GLOBAL: FF7 0x00b6f0e8
int g_BattleSpecialBlasterScaleHistory2 = 0;
// GLOBAL: FF7 0x00b6f0ec
int g_BattleSpecialBlasterScaleHistory1 = 0;
// GLOBAL: FF7 0x00b6f0f0
int g_BattleSpecialBlasterScaleHistory0 = 0;
unsigned char g_bssPad_00b6f0f4[4] = {0};
// GLOBAL: FF7 0x00b6f0f8
struct BattleSpecialBlasterColorBuffer g_BattleSpecialBlasterColorBuffer = {0};
// GLOBAL: FF7 0x00b6f0fc
short g_BattleSpecialBlasterState = 0;
// GLOBAL: FF7 0x00b6f100
unsigned char g_BattleSpecialBlasterLookBasis[0x4] = {0};
unsigned char g_bssPad_00b6f104[4] = {0};
unsigned char g_bssPad_00b6f108[8] = {0};
// GLOBAL: FF7 0x00b6f110
unsigned char g_BattleSpecialBlasterCoordScratch1[0x4] = {0};

// GLOBAL: FF7 0x00b6f114
unsigned char g_BattleSpecialBlasterCoordScratch2[0x4] = {0};

// GLOBAL: FF7 0x00b6f118
unsigned char g_BattleSpecialBlasterCoordScratch3[0x8] = {0};

// GLOBAL: FF7 0x00b6f120
short g_BattleSpecialBlasterAngle = 0;
// GLOBAL: FF7 0x00b6f124
int g_BattleSpecialBlasterDrawListHandle = 0;
// GLOBAL: FF7 0x00b6f128
int g_BattleSpecialBlasterParamHistory2 = 0;
// GLOBAL: FF7 0x00b6f12c
int g_BattleSpecialBlasterParamHistory1 = 0;
// GLOBAL: FF7 0x00b6f130
int g_BattleSpecialBlasterParamHistory0 = 0;
unsigned char g_bssPad_00b6f134[4] = {0};
// GLOBAL: FF7 0x00b6f138
struct GfxTableCursor *g_BattleSpecialBlasterLoadedRes = 0;
unsigned char g_bssPad_00b6f13c[4] = {0};
// GLOBAL: FF7 0x00b6f140
int g_BattleSpecialBlasterRotAngle = 0;
unsigned char g_bssPad_00b6f144[4] = {0};
// GLOBAL: FF7 0x00b6f148
short g_BattleSpecialBlasterAlphaStep = 0;
// clang-format on


#pragma intrinsic(memset)
#pragma intrinsic(memcpy)


/*
 * Loads and registers resources for the B2 special effect.
 */
// FUNCTION: FF7 0x00556130
void __cdecl BattleFx_SpecialLoadResBlaster(void)
{
    GfxModelTableBank *newSceneLink;
    GfxResourceRequest resultPending;
    int kind;
    char pathWorkBuffer[200];
    GfxResourceRequest *reqPtrPtr;
    int resourceDefsWord;

    if (g_pBattle3dScene != 0 && g_BattleSpecialBlasterLoadedRes == 0) {
        kind = 0x17;
        resourceDefsWord = (int)&g_BattleSpecialBlasterResourceDefs;
        newSceneLink = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        reqPtrPtr = &resultPending;
        Gfx_InitResourceRequest(reqPtrPtr);
        reqPtrPtr->groupTag = 0;
        // STRING: FF7 0x008a7d28
        reqPtrPtr->resourcePath = BattleFx_BuildResourcePath("SPECIAL/B_2", pathWorkBuffer);
        Gfx_LoadModelTableIfAbsent(kind, resourceDefsWord, reqPtrPtr, &g_BattleSpecialBlasterLoadedRes, newSceneLink);
    }
}

/*
 * Entry point thunk for the B2 special effect.
 */
// FUNCTION: FF7 0x005561c0
void __cdecl BattleFx_SpecialEnterBlaster(int targetMask, int actorIdx)
{
    BattleFx_SpecialLoadResBlaster();
    BattleFx_SpecialBeginBlaster(targetMask, actorIdx);
}

/*
 * Initializes resources and starts target tasks for B2.
 */
// FUNCTION: FF7 0x005561da
void __cdecl BattleFx_SpecialBeginBlaster(int targetMask, int actorIdx)
{
    Battle_CreateBitPatternCallbackTask(targetMask, actorIdx, 4, BattleFx_SpecialSpawnBlasterPerTarget);
    Battle_PlaySfxPannedToActor(0x78, (short)actorIdx, 0x40);
}

/*
 * Spawns per-target burst explosion tasks for B2.
 */
// FUNCTION: FF7 0x00556207
void __cdecl BattleFx_SpecialSpawnBlasterPerTarget(int actor, int attackerId)
{
    struct {
        short direction[4];
        BattleFxSpecialBlasterSlot *slot;
        int unused;
    } local; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    local.slot = (BattleFxSpecialBlasterSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialBlasterTimelineTask);
    Battle_GetBoneWorldPos(attackerId, g_BattleActorRecords[attackerId].bone34, &local.slot->posX);
    Battle_GetBoneWorldPos(actor, g_BattleActorRecords[actor].targetBone, &local.slot->targetX);
    local.direction[0] = *&local.slot->targetX - *&local.slot->posX;
    local.direction[1] = local.slot->targetY - local.slot->posY;
    local.direction[2] = local.slot->targetZ - local.slot->posZ;
    Gte_BuildLookBasisFromDirection(local.direction, (short *)&g_BattleSpecialBlasterLookBasis);
    local.slot->recordType = actor;
}

/*
 * Main timeline task managing multi-stage burst explosion for B2.
 */
// FUNCTION: FF7 0x005562cc
void __cdecl BattleFx_SpecialBlasterTimelineTask(void)
{
    int p[2]; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxSpecialBlasterSlot *taskPtr;
    short diff[4];
    short beamLen;
    short n6;
    BattleFxSpecialBlasterSlot *pChild;
    int e; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    short cosOffset;
    short sinOfs;
    int stackWords; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */

    taskPtr = (BattleFxSpecialBlasterSlot *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (taskPtr->step >= 0 && taskPtr->step <= 0xA && taskPtr->step % 4 == 0) {
        pChild = (BattleFxSpecialBlasterSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialBlasterShrinkingModelTask);
        pChild->posX = taskPtr->posX;
        pChild->posY = taskPtr->posY;
        pChild->posZ = taskPtr->posZ;
    }
    if (taskPtr->step == 0) {
        pChild = (BattleFxSpecialBlasterSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialBlasterDrawSpinModelsTask);
        pChild->posX = taskPtr->posX;
        pChild->posY = taskPtr->posY;
        pChild->posZ = taskPtr->posZ;
        pChild->timer = 0;
    }
    if (taskPtr->step == 0x1E) {
        pChild = (BattleFxSpecialBlasterSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialBlasterStretchModelsTask);
        pChild->posX = taskPtr->posX;
        pChild->posY = taskPtr->posY;
        pChild->posZ = taskPtr->posZ;
        diff[0] = taskPtr->targetX - pChild->posX;
        diff[1] = taskPtr->targetY - pChild->posY;
        diff[2] = taskPtr->targetZ - pChild->posZ;
        beamLen = SqrtToInt(diff[0] * diff[0] + diff[1] * diff[1] + diff[2] * diff[2]);
        pChild->timer = beamLen * 0x1200 / 0xBB8;
        pChild->clearedWord = 0;
        pChild = (BattleFxSpecialBlasterSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialBlasterForwardBurstTask);
        pChild->posX = taskPtr->posX;
        pChild->posY = taskPtr->posY;
        pChild->posZ = taskPtr->posZ;
        pChild->timer = beamLen;
        pChild->clearedWord = 0;
        pChild = (BattleFxSpecialBlasterSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialBlasterGrowModelTask);
        pChild->posX = taskPtr->posX;
        pChild->posY = taskPtr->posY;
        pChild->posZ = taskPtr->posZ;
        g_BattleOverlayDrawListHead = BattleFx_FlashScreenWhite(0x80, 0x80, 0x80);
    }
    if (taskPtr->step == 0x24) {
        pChild = (BattleFxSpecialBlasterSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialBlasterPulseModelsTask);
        pChild->posX = taskPtr->targetX;
        pChild->posY = taskPtr->targetY;
        pChild->posZ = taskPtr->targetZ;
        g_BattleFxTimelineState.channel2 = g_BattleFxTimelineState.channel1 = 0;
        g_BattleFxTimelineState.red = 0x20;
        Battle_StartFixedPointRamp(0x1000, 0xA);
    }
    if (taskPtr->step > 0x32 && taskPtr->step < 0x42) {
        for (n6 = 0; n6 < 2; n6++) {
            pChild = (BattleFxSpecialBlasterSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialBlasterVariantSpriteTask);
            g_BattleSpecialBlasterAngle = taskPtr->step << 9;
            sinOfs = SinFixed(g_BattleSpecialBlasterAngle) * 0x320 >> 0xC;
            cosOffset = CosFixed(g_BattleSpecialBlasterAngle) * 0x320 >> 0xC;
            pChild->posX = taskPtr->targetX + sinOfs;
            pChild->posY = taskPtr->targetY + rand() % 0x320;
            pChild->posZ = taskPtr->targetZ + cosOffset;
            pChild->targetX = sinOfs >> 2;
            pChild->targetZ = cosOffset >> 2;
            pChild->flag = 0;
        }
    }
    if (taskPtr->step == 0x43) {
        pChild = (BattleFxSpecialBlasterSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialBlasterFinalFlashModelsTask);
        pChild->posX = taskPtr->targetX;
        pChild->posY = taskPtr->targetY;
        pChild->posZ = taskPtr->targetZ;
    }
    if (taskPtr->step == 0x48) {
        Battle_PostActorHitOrHurtMessage(taskPtr->recordType);
        for (n6 = 0; n6 < 4; n6++) {
            pChild = (BattleFxSpecialBlasterSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialBlasterVariantSpriteTask);
            pChild->posX = taskPtr->targetX;
            pChild->posY = taskPtr->targetY;
            pChild->posZ = taskPtr->targetZ;
            pChild->posX += rand() % 0x4B0 - 0x258;
            pChild->posY += rand() % 0x4B0 - 0x258;
            pChild->posZ += rand() % 0x4B0 - 0x258;
            pChild->flag = 1;
        }
        Battle_PlaySfxPannedToActor(0xFA, taskPtr->recordType, 0x40);
    }
    (taskPtr->step)++;
    if (taskPtr->step == 0x50) {
        Battle_StartFixedPointRamp(0, 0xA);
        taskPtr->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Animates expanding blast flare sprites for B2.
 */
// FUNCTION: FF7 0x00556895
void __cdecl BattleFx_SpecialBlasterVariantSpriteTask(void)
{
    struct {
        int table[2];
        BattleFxSpriteFrameSlot *slot;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    frame.slot = (BattleFxSpriteFrameSlot *)BATTLE_CURRENT_TASK_SLOT;
    frame.table[0] = 0x15;
    frame.table[1] = 0x16;
    BattleFx_InstallBillboardMatrix(&frame.slot->posX, (frame.slot->flag != 0 ? 0x1000 : 0) + 0x4000, 0);
    g_BattleSpecialBlasterSpriteReq1.frameIndex = frame.slot->frameIndex;
    (g_BattleSpecialBlasterSpriteReq1.spriteGroup) = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(frame.table[frame.slot->flag], g_BattleSpecialBlasterLoadedRes);
    g_BattleSpecialBlasterSpriteReq1.drawObj = Gfx_ModelTableGetSpriteObject(frame.table[frame.slot->flag], g_BattleSpecialBlasterLoadedRes);
    g_BattleSpecialBlasterDrawListHandle = BattleFx_SubmitSpriteDraw(&g_BattleSpecialBlasterSpriteReq1, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialBlasterDrawListHandle);
    if (g_BattleIsPaused == 0) {
        if (frame.slot->flag == 0) {
            frame.slot->posX += frame.slot->velocityX;
            frame.slot->posZ += frame.slot->velocityZ;
            frame.slot->posY -= 0x64;
        }
        if ((g_BattleSpecialBlasterSpriteReq1.spriteGroup) != 0) {
            (frame.slot->frameIndex)++;
            if (frame.slot->frameIndex >= g_BattleSpecialBlasterSpriteReq1.spriteGroup->header->frameCount) {
                frame.slot->step = 0xFFFF;
            }
        }
    }
}

/*
 * Animates shrinking energy implosion model for B2.
 */
// FUNCTION: FF7 0x005569e3
void __cdecl BattleFx_SpecialBlasterShrinkingModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[84];
        PsxMatrix rotMatrix;
        struct { unsigned char pad_to_30[0x30]; BattleFxCosmoMemoryTriSpin *shrinkSlot; };
        struct { unsigned char pad_to_34[0x34]; PsxMatrix combinedMatrix; };
    } frame;
#pragma pack(pop)

    frame.shrinkSlot = (BattleFxCosmoMemoryTriSpin *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecialBlasterModelReq1[0] = Gfx_ModelTableGetRenderObject(0, g_BattleSpecialBlasterLoadedRes);
    RotMatrix((SVECTOR *)&g_BattleSpecialBlasterRotAngle, &frame.rotMatrix);
    g_BattleSpecialBlasterScaleHistory0 = 0x4000 - frame.shrinkSlot->step * 0x320;
    g_BattleSpecialBlasterScaleHistory1 = g_BattleSpecialBlasterScaleHistory0;
    g_BattleSpecialBlasterScaleHistory2 = g_BattleSpecialBlasterScaleHistory1;
    ScaleMatrix(&frame.rotMatrix, &g_BattleSpecialBlasterScaleHistory2);
    frame.rotMatrix.t[0] = frame.shrinkSlot->posX;
    frame.rotMatrix.t[1] = frame.shrinkSlot->posY;
    frame.rotMatrix.t[2] = frame.shrinkSlot->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.combinedMatrix);
    SetRotMatrix(&frame.combinedMatrix);
    SetTransMatrix(&frame.combinedMatrix);
    (((BattleFxModelDraw *)&g_BattleSpecialBlasterModelReq1)->fadeLevel) = frame.shrinkSlot->step * 6;
    g_BattleSpecialBlasterDrawListHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecialBlasterModelReq1, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialBlasterDrawListHandle);
    if (g_BattleIsPaused != 0) {
        return;
    }
    (frame.shrinkSlot->step)++;
    if (frame.shrinkSlot->step == 0x14) {
        frame.shrinkSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Renders spinning energy shockwave models for B2.
 */
// FUNCTION: FF7 0x00556b1b
void __cdecl BattleFx_SpecialBlasterDrawSpinModelsTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[92];
        SVECTOR angleX;
        struct { unsigned char pad_to_8[0x8]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_38[0x38]; BattleFxBlasterDrawSpinModelsSlot *spinSlot; };
        struct { unsigned char pad_to_3C[0x3C]; PsxMatrix combinedMatrix; };
    } frame;
#pragma pack(pop)

    frame.spinSlot = (BattleFxBlasterDrawSpinModelsSlot *)BATTLE_CURRENT_TASK_SLOT;
    frame.angleX.vx = 0;
    frame.angleX.vy = 0;
    frame.angleX.vz = 0;
    RotMatrix(&frame.angleX, &frame.rotMatrix);
    g_BattleSpecialBlasterParamHistory0 = frame.spinSlot->spinParam;
    g_BattleSpecialBlasterParamHistory1 = g_BattleSpecialBlasterParamHistory0;
    g_BattleSpecialBlasterParamHistory2 = g_BattleSpecialBlasterParamHistory1;
    ScaleMatrix(&frame.rotMatrix, &g_BattleSpecialBlasterParamHistory2);
    frame.rotMatrix.t[0] = frame.spinSlot->posX;
    frame.rotMatrix.t[1] = frame.spinSlot->posY;
    frame.rotMatrix.t[2] = frame.spinSlot->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.combinedMatrix);
    SetRotMatrix(&frame.combinedMatrix);
    SetTransMatrix(&frame.combinedMatrix);
    (((BattleFxModelDraw *)&g_BattleSpecialBlasterModelReq2)->fadeLevel) = 0x1000 - (g_BattleSpecialBlasterAlphaStep << 9);
    g_BattleSpecialBlasterModelReq2[0] = Gfx_ModelTableGetRenderObject(2, g_BattleSpecialBlasterLoadedRes);
    SetDrawColorRGB(0, 0, 0);
    g_BattleSpecialBlasterDrawListHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecialBlasterModelReq2, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialBlasterDrawListHandle);
    frame.angleX.vx = frame.spinSlot->frameCounter << 8;
    frame.angleX.vy = frame.spinSlot->frameCounter << 9;
    frame.angleX.vz = 0;
    RotMatrix(&frame.angleX, &frame.rotMatrix);
    g_BattleSpecialBlasterParamHistory0 = frame.spinSlot->spinParam;
    g_BattleSpecialBlasterParamHistory1 = g_BattleSpecialBlasterParamHistory0;
    g_BattleSpecialBlasterParamHistory2 = g_BattleSpecialBlasterParamHistory1;
    ScaleMatrix(&frame.rotMatrix, &g_BattleSpecialBlasterParamHistory2);
    frame.rotMatrix.t[0] = frame.spinSlot->posX;
    frame.rotMatrix.t[1] = frame.spinSlot->posY;
    frame.rotMatrix.t[2] = frame.spinSlot->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.combinedMatrix);
    SetRotMatrix(&frame.combinedMatrix);
    SetTransMatrix(&frame.combinedMatrix);
    g_BattleSpecialBlasterModelReq2[0] = Gfx_ModelTableGetRenderObject(3, g_BattleSpecialBlasterLoadedRes);
    g_BattleSpecialBlasterDrawListHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecialBlasterModelReq2, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialBlasterDrawListHandle);
    BattleFx_InstallBillboardMatrix(&frame.spinSlot->posX, 0x2000, 0);
    (g_BattleSpecialBlasterModelReq3.alpha) = g_BattleSpecialBlasterAlphaStep << 4;
    (g_BattleSpecialBlasterModelReq3.alphaPrev) = (g_BattleSpecialBlasterModelReq3.alpha);
    (g_BattleSpecialBlasterModelReq3.alphaOld) = (g_BattleSpecialBlasterModelReq3.alphaPrev);
    (g_BattleSpecialBlasterModelReq3.parity) = (frame.spinSlot->frameCounter % 2);
    g_BattleSpecialBlasterDrawListHandle = BattleFx_SubmitSpriteDraw(&g_BattleSpecialBlasterModelReq3, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialBlasterDrawListHandle);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.spinSlot->frameCounter <= 8) {
        g_BattleSpecialBlasterAlphaStep++;
    }
    if (frame.spinSlot->frameCounter > 0x16) {
        g_BattleSpecialBlasterAlphaStep--;
        frame.spinSlot->spinParam -= 0x514;
    } else {
        frame.spinSlot->spinParam += 0x1F4;
    }
    (frame.spinSlot->frameCounter)++;
    if (frame.spinSlot->frameCounter == 0x1E) {
        frame.spinSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Animates final bright flash explosion models for B2.
 */
// FUNCTION: FF7 0x00556e35
void __cdecl BattleFx_SpecialBlasterFinalFlashModelsTask(void)
{
    BattleFxBlasterFlashSlot *task;

    task = (BattleFxBlasterFlashSlot *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (task->phase == 2) {
        BattleFx_InstallBillboardMatrix(&task->posX, 0x2000, 0);
        SetDrawColorRGB(0, 0, 0);
        g_BattleSpecialBlasterModelReq4[0] = Gfx_ModelTableGetRenderObject(0x12, g_BattleSpecialBlasterLoadedRes);
        g_BattleSpecialBlasterDrawListHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecialBlasterModelReq4, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialBlasterDrawListHandle);
        g_BattleSpecialBlasterModelReq4[0] = Gfx_ModelTableGetRenderObject(0x13, g_BattleSpecialBlasterLoadedRes);
        g_BattleSpecialBlasterDrawListHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecialBlasterModelReq4, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialBlasterDrawListHandle);
    }
    if (task->phase == 3) {
        g_BattleOverlayDrawListHead = BattleFx_FlashScreenWhite(0x80, 0x80, 0x80);
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    (task->phase)++;
}

/*
 * Animates vertically stretched shockwave blast models for B2.
 */
// FUNCTION: FF7 0x00556f4a
void __cdecl BattleFx_SpecialBlasterStretchModelsTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[92];
        int clampedFrame;
        struct { unsigned char pad_to_4[0x4]; short clampedFrameWord; };
        struct { unsigned char pad_to_8[0x8]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_28[0x28]; BattleFxBlasterStretchModelsSlot *stretchSlot; };
        struct { unsigned char pad_to_2C[0x2C]; PsxMatrix combinedMatrix; };
        struct { unsigned char pad_to_4C[0x4C]; VECTOR scaleX; };
    } frame;
#pragma pack(pop)

    frame.stretchSlot = (BattleFxBlasterStretchModelsSlot *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecialBlasterModelReq6[0] = Gfx_ModelTableGetRenderObject(9, g_BattleSpecialBlasterLoadedRes);
    if (frame.stretchSlot->frameCounter < 6) {
        frame.clampedFrame = frame.stretchSlot->frameCounter;
    } else {
        frame.clampedFrame = 6;
    }
    frame.clampedFrameWord = *(short *)&frame.clampedFrame;
    RotMatrix(&g_BattleSpecialBlasterRotAngle1, &frame.rotMatrix);
    ConcatMatrixPreservingTranslation(&g_BattleSpecialBlasterLookBasis, &frame.rotMatrix, &frame.rotMatrix);
    frame.scaleX.vz = 0x4000 - (frame.clampedFrameWord << 0xA);
    frame.scaleX.vx = frame.scaleX.vz;
    frame.scaleX.vy = frame.stretchSlot->baseLength * frame.clampedFrameWord / 6;
    ScaleMatrix(&frame.rotMatrix, &frame.scaleX);
    frame.rotMatrix.t[0] = frame.stretchSlot->posX;
    frame.rotMatrix.t[1] = frame.stretchSlot->posY;
    frame.rotMatrix.t[2] = frame.stretchSlot->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.combinedMatrix);
    SetRotMatrix(&frame.combinedMatrix);
    SetTransMatrix(&frame.combinedMatrix);
    SetDrawColorRGB(0, 0, 0);
    (((BattleFxModelDraw *)&g_BattleSpecialBlasterModelReq6)->fadeLevel) = 0x1000 - (g_BattleSpecialBlasterAlpha1 << 7);
    g_BattleSpecialBlasterDrawListHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecialBlasterModelReq6, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialBlasterDrawListHandle);
    RotMatrix(&g_BattleSpecialBlasterRotAngle1, &frame.rotMatrix);
    ConcatMatrixPreservingTranslation(&g_BattleSpecialBlasterLookBasis, &frame.rotMatrix, &frame.rotMatrix);
    frame.scaleX.vz = 0x2000 - (frame.clampedFrameWord << 9);
    frame.scaleX.vx = frame.scaleX.vz;
    frame.scaleX.vy = frame.stretchSlot->baseLength * frame.clampedFrameWord / 6;
    ScaleMatrix(&frame.rotMatrix, &frame.scaleX);
    frame.rotMatrix.t[0] = frame.stretchSlot->posX;
    frame.rotMatrix.t[1] = frame.stretchSlot->posY;
    frame.rotMatrix.t[2] = frame.stretchSlot->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.combinedMatrix);
    SetRotMatrix(&frame.combinedMatrix);
    SetTransMatrix(&frame.combinedMatrix);
    g_BattleSpecialBlasterDrawListHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecialBlasterModelReq6, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialBlasterDrawListHandle);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.stretchSlot->frameCounter > 6) {
        g_BattleSpecialBlasterAlpha1 -= 2;
    }
    (frame.stretchSlot->frameCounter)++;
    if (frame.stretchSlot->frameCounter == 0xA) {
        frame.stretchSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Animates forward burst particle and model expansion for B2.
 */
// FUNCTION: FF7 0x005571c5
void __cdecl BattleFx_SpecialBlasterForwardBurstTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[84];
        int clampedFrame;
        struct { unsigned char pad_to_4[0x4]; int transDepth; };
        struct { unsigned char pad_to_8[0x8]; short clampedFrameWord; };
        struct { unsigned char pad_to_C[0xC]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_2C[0x2C]; BattleFxBlasterBurstSlot *trailSlot; };
        struct { unsigned char pad_to_30[0x30]; BattleFxBlasterForwardBurstSlot *burstSlot; };
        struct { unsigned char pad_to_34[0x34]; PsxMatrix combinedMatrix; };
    } frame;
#pragma pack(pop)

    frame.burstSlot = (BattleFxBlasterForwardBurstSlot *)BATTLE_CURRENT_TASK_SLOT;
    if (frame.burstSlot->frameCounter < 6) {
        frame.clampedFrame = frame.burstSlot->frameCounter;
    } else {
        frame.clampedFrame = 6;
    }
    frame.clampedFrameWord = *(short *)&frame.clampedFrame;
    (g_BattleSpecialBlasterColorBuffer.colorLow) = 0;
    (g_BattleSpecialBlasterColorBuffer.colorHigh) = 0;
    g_BattleSpecialBlasterState = frame.burstSlot->extent * frame.clampedFrameWord / 6;
    (*(int *)((char *)&g_BattleSpecialBlasterCoordScratch1 + 0x2)) = frame.burstSlot->posX;
    (*(int *)((char *)&g_BattleSpecialBlasterCoordScratch2 + 0x2)) = frame.burstSlot->posY;
    (*(int *)((char *)&g_BattleSpecialBlasterCoordScratch3 + 0x2)) = frame.burstSlot->posZ;
    SetRotMatrix(&g_BattleSpecialBlasterLookBasis);
    SetTransMatrix(&g_BattleSpecialBlasterLookBasis);
    RotTransPointToInt16((short *)&g_BattleSpecialBlasterColorBuffer, (short *)&g_BattleSpecialBlasterColorBuffer, &frame.transDepth);
    RotMatrix((SVECTOR *)&g_BattleSpecialBlasterRotXYZ2, &frame.rotMatrix);
    ConcatMatrixPreservingTranslation(&g_BattleSpecialBlasterLookBasis, &frame.rotMatrix, &frame.rotMatrix);
    ScaleMatrix(&frame.rotMatrix, &g_BattleSpecialBlasterScale1);
    frame.rotMatrix.t[0] = (g_BattleSpecialBlasterColorBuffer.colorLow);
    frame.rotMatrix.t[1] = (g_BattleSpecialBlasterColorBuffer.colorHigh);
    frame.rotMatrix.t[2] = g_BattleSpecialBlasterState;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.combinedMatrix);
    SetRotMatrix(&frame.combinedMatrix);
    SetTransMatrix(&frame.combinedMatrix);
    (((BattleFxModelDraw *)&g_BattleSpecialBlasterModelReq7)->fadeLevel) = 0x1000 - (g_BattleSpecialBlasterAngles1[0] << 9);
    SetDrawColorRGB(0, 0, 0);
    g_BattleSpecialBlasterModelReq7[0] = Gfx_ModelTableGetRenderObject(4, g_BattleSpecialBlasterLoadedRes);
    g_BattleSpecialBlasterDrawListHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecialBlasterModelReq7, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialBlasterDrawListHandle);
    g_BattleSpecialBlasterModelReq7[0] = Gfx_ModelTableGetRenderObject(5, g_BattleSpecialBlasterLoadedRes);
    g_BattleSpecialBlasterDrawListHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecialBlasterModelReq7, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialBlasterDrawListHandle);
    g_BattleSpecialBlasterModelReq7[0] = Gfx_ModelTableGetRenderObject(6, g_BattleSpecialBlasterLoadedRes);
    g_BattleSpecialBlasterDrawListHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecialBlasterModelReq7, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialBlasterDrawListHandle);
    BattleFx_InstallBillboardMatrix((short *)&g_BattleSpecialBlasterColorBuffer, 0x3000, 0);
    (g_BattleSpecialBlasterModelReq8.angle) = g_BattleSpecialBlasterAngles1[0] * 0x14;
    (g_BattleSpecialBlasterModelReq8.anglePrev) = (g_BattleSpecialBlasterModelReq8.angle);
    (g_BattleSpecialBlasterModelReq8.angleOld) = (g_BattleSpecialBlasterModelReq8.anglePrev);
    (g_BattleSpecialBlasterModelReq8.parity) = (frame.burstSlot->frameCounter % 2);
    g_BattleSpecialBlasterDrawListHandle = BattleFx_SubmitSpriteDraw(&g_BattleSpecialBlasterModelReq8, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialBlasterDrawListHandle);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.burstSlot->frameCounter < 5) {
        frame.trailSlot = (BattleFxBlasterBurstSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialBlasterTrailModelTask);
        frame.trailSlot->colorLow = (g_BattleSpecialBlasterColorBuffer.colorLow);
        frame.trailSlot->colorHigh = (g_BattleSpecialBlasterColorBuffer.colorHigh);
        frame.trailSlot->state = g_BattleSpecialBlasterState;
    }
    if (frame.burstSlot->frameCounter > 6) {
        g_BattleSpecialBlasterAngles1[0] -= 2;
    }
    (frame.burstSlot->frameCounter)++;
    if (frame.burstSlot->frameCounter == 0xA) {
        frame.burstSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Animates trailing debris models following the burst for B2.
 */
// FUNCTION: FF7 0x005574fe
void __cdecl BattleFx_SpecialBlasterTrailModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[84];
        PsxMatrix rotMatrix;
        struct { unsigned char pad_to_20[0x20]; BattleFxOilTargetModelSlot *trailSlot; };
        struct { unsigned char pad_to_24[0x24]; PsxMatrix combinedMatrix; };
        struct { unsigned char pad_to_44[0x44]; VECTOR scaleX; };
    } frame;
#pragma pack(pop)

    frame.trailSlot = (BattleFxOilTargetModelSlot *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecialBlasterModelReq5[0] = Gfx_ModelTableGetRenderObject(7, g_BattleSpecialBlasterLoadedRes);
    if (frame.trailSlot->frameCounter > 1) {
        RotMatrix((SVECTOR *)&g_BattleSpecialBlasterRotXYZ1, &frame.rotMatrix);
        ConcatMatrixPreservingTranslation(&g_BattleSpecialBlasterLookBasis, &frame.rotMatrix, &frame.rotMatrix);
        frame.scaleX.vz = 0x2000 - frame.trailSlot->frameCounter * 0x320;
        frame.scaleX.vy = frame.scaleX.vz;
        frame.scaleX.vx = frame.scaleX.vy;
        ScaleMatrix(&frame.rotMatrix, &frame.scaleX);
        frame.rotMatrix.t[0] = frame.trailSlot->posX;
        frame.rotMatrix.t[1] = frame.trailSlot->posY;
        frame.rotMatrix.t[2] = frame.trailSlot->posZ;
        MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.combinedMatrix);
        SetRotMatrix(&frame.combinedMatrix);
        SetTransMatrix(&frame.combinedMatrix);
        SetDrawColorRGB(0, 0, 0);
        (((BattleFxModelDraw *)&g_BattleSpecialBlasterModelReq5)->fadeLevel) = frame.trailSlot->frameCounter * 0x190;
        g_BattleSpecialBlasterDrawListHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecialBlasterModelReq5, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialBlasterDrawListHandle);
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    (frame.trailSlot->frameCounter)++;
    if (frame.trailSlot->frameCounter == 0xA) {
        frame.trailSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Animates pulsing energy wave models for B2.
 */
// FUNCTION: FF7 0x0055765e
void __cdecl BattleFx_SpecialBlasterPulseModelsTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[136];
        struct { unsigned char pad_to_4[0x4]; SVECTOR angleX; };
        struct { unsigned char pad_to_C[0xC]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_2C[0x2C]; int pulseModelId0[8]; };
        struct { unsigned char pad_to_54[0x54]; BattleFxOilTargetModelSlot *taskSlot; };
        struct { unsigned char pad_to_58[0x58]; PsxMatrix drawMatrix; };
        struct { unsigned char pad_to_78[0x78]; VECTOR scaleVec; };
    } frame;
#pragma pack(pop)

    frame.taskSlot = (BattleFxOilTargetModelSlot *)BATTLE_CURRENT_TASK_SLOT;
    frame.pulseModelId0[0] = 0xA;
    frame.pulseModelId0[1] = 0xB;
    frame.pulseModelId0[2] = 0xC;
    frame.pulseModelId0[3] = 0xD;
    frame.pulseModelId0[4] = 0xE;
    frame.pulseModelId0[5] = 0xF;
    frame.pulseModelId0[6] = 0x10;
    frame.pulseModelId0[7] = 0x11;
    frame.angleX.vz = 0;
    frame.angleX.vy = frame.angleX.vz;
    frame.angleX.vx = frame.angleX.vy;
    RotMatrix(&frame.angleX, &frame.rotMatrix);
    frame.scaleVec.vz = SinFixed(frame.taskSlot->frameCounter * 0x33) * 0x3000 >> 0xC;
    frame.scaleVec.vy = frame.scaleVec.vz;
    frame.scaleVec.vx = frame.scaleVec.vy;
    ScaleMatrix(&frame.rotMatrix, &frame.scaleVec);
    frame.rotMatrix.t[0] = frame.taskSlot->posX;
    frame.rotMatrix.t[1] = frame.taskSlot->posY;
    frame.rotMatrix.t[2] = frame.taskSlot->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.drawMatrix);
    SetRotMatrix(&frame.drawMatrix);
    SetTransMatrix(&frame.drawMatrix);
    SetDrawColorRGB(0, 0, 0);
    g_BattleSpecialBlasterModelReq9[0] = Gfx_ModelTableGetRenderObject(8, g_BattleSpecialBlasterLoadedRes);
    g_BattleSpecialBlasterDrawListHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecialBlasterModelReq9, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialBlasterDrawListHandle);
    g_BattleSpecialBlasterModelReq9[0] = Gfx_ModelTableGetRenderObject(frame.pulseModelId0[((frame.taskSlot->frameCounter) % 8)], g_BattleSpecialBlasterLoadedRes);
    g_BattleSpecialBlasterDrawListHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecialBlasterModelReq9, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialBlasterDrawListHandle);
    BattleFx_InstallBillboardMatrix(&frame.taskSlot->posX, 0x3000, 0);
    (*(short *)((char *)&g_BattleSpecialBlasterModelReq10 + 0xc)) = ((frame.taskSlot->frameCounter) % 2);
    g_BattleSpecialBlasterDrawListHandle = BattleFx_SubmitSpriteDraw(&g_BattleSpecialBlasterModelReq10, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialBlasterDrawListHandle);
    if (g_BattleIsPaused != 0) {
        return;
    }
    (frame.taskSlot->frameCounter)++;
    if (frame.taskSlot->frameCounter == 0x28) {
        frame.taskSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Animates rapidly expanding shockwave dome model for B2.
 */
// FUNCTION: FF7 0x0055788b
void __cdecl BattleFx_SpecialBlasterGrowModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[84];
        PsxMatrix rotMatrix;
        struct { unsigned char pad_to_20[0x20]; BattleFxCosmoMemoryTriSpin *taskSlot; };
        struct { unsigned char pad_to_24[0x24]; PsxMatrix drawMatrix; };
        struct { unsigned char pad_to_44[0x44]; VECTOR scaleVec; };
    } frame;
#pragma pack(pop)

    frame.taskSlot = (BattleFxCosmoMemoryTriSpin *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecialBlasterModelReq11[0] = Gfx_ModelTableGetRenderObject(1, g_BattleSpecialBlasterLoadedRes);
    RotMatrix((SVECTOR *)&g_BattleSpecialBlasterRotXYZ3, &frame.rotMatrix);
    ConcatMatrixPreservingTranslation(&g_BattleSpecialBlasterLookBasis, &frame.rotMatrix, &frame.rotMatrix);
    frame.scaleVec.vz = frame.taskSlot->step * 0x5DC;
    frame.scaleVec.vy = frame.scaleVec.vz;
    frame.scaleVec.vx = frame.scaleVec.vy;
    ScaleMatrix(&frame.rotMatrix, &frame.scaleVec);
    frame.rotMatrix.t[0] = frame.taskSlot->posX;
    frame.rotMatrix.t[1] = frame.taskSlot->posY;
    frame.rotMatrix.t[2] = frame.taskSlot->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.drawMatrix);
    SetRotMatrix(&frame.drawMatrix);
    SetTransMatrix(&frame.drawMatrix);
    SetDrawColorRGB(0, 0, 0);
    (((BattleFxModelDraw *)&g_BattleSpecialBlasterModelReq11)->fadeLevel) = frame.taskSlot->step << 8;
    g_BattleSpecialBlasterDrawListHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecialBlasterModelReq11, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialBlasterDrawListHandle);
    if (g_BattleIsPaused != 0) {
        return;
    }
    (frame.taskSlot->step)++;
    if (frame.taskSlot->step == 0x14) {
        frame.taskSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}
} /* extern "C" */
