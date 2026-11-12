// src/ff7/src/battle/battle_fx_special_coin_toss.cpp
/* Enemy Special Attack [84]: Coin Toss (orig. SPECIAL/KINZAN, COIN.TIM/SPT) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle special effect for the Kinzan attack: it loads resources and spawns a per-target
//   timeline that drives a bouncing sprite task.

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

// clang-format off
// GLOBAL: FF7 0x008a4000
int g_BattleSpecCoinTossResourceDefs = 0x00192A39;
int g_BattleSpecCoinTossResourceDefsHeadPtr[1] = {
    (int)((char *)&g_BattleFxSpecialCoinTossResourceNames + 0x0)
};
// GLOBAL: FF7 0x008a4008
int g_BattleSpecCoinTossResourceModelRows[10] = {
    (int)((char *)&g_BattleFxSpecialCoinTossResourceNames + 0xc), 0x0, 0x0, 0x192a3c, (int)((char *)&g_BattleFxSpecialCoinTossResourceNames + 0x18), (int)((char *)&g_BattleFxSpecialCoinTossResourceNames + 0x24),
    0x0, 0x0, 0x0, 0x0
};
// GLOBAL: FF7 0x008a4030
struct BattleSpecPoisonGasSpriteReq2 g_BattleSpecCoinTossSpriteReq1 = {-128, -128, -128, 44, 0, {0}};
unsigned char g_BattleFxSpecialCoinTossResourceNames[48] =
    "COIN.TIM\0\0\0\0"
    "COIN.SPT\0\0\0\0"
    "COIN.TIM\0\0\0\0"
    "COIN.SPT";
// GLOBAL: FF7 0x00b6ac20
int g_BattleSpecCoinTossSourceBonePos = 0;
// GLOBAL: FF7 0x00b6ac24
short g_BattleSpecCoinTossSourceBonePosZ = 0;
// GLOBAL: FF7 0x00b6ac28
int g_BattleSpecCoinTossDrawHandle = 0;
// GLOBAL: FF7 0x00b6ac2c
struct GfxTableCursor *g_BattleSpecCoinTossLoadedResTable = 0;
// GLOBAL: FF7 0x00b6ac30
short g_BattleSpecCoinTossFlashLevel = 0;
// clang-format on

#pragma intrinsic(memset)
#pragma intrinsic(memcpy)


/*
 * Loads the SPECIAL/KINZAN model table once, when the battle scene exists and the table is still empty.
 */
// FUNCTION: FF7 0x00540500
void __cdecl BattleFx_SpecialLoadResCoinToss(void)
{
    GfxModelTableBank *curOwnerCtx;
    GfxResourceRequest currentRecord;
    int kind;
    char pathText[200];
    GfxResourceRequest *pRequest;
    int resourceDefsWord;

    if (g_pBattle3dScene != 0 && g_BattleSpecCoinTossLoadedResTable == 0) {
        kind = 2;
        resourceDefsWord = (int)&g_BattleSpecCoinTossResourceDefs;
        curOwnerCtx = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRequest = &currentRecord;
        Gfx_InitResourceRequest(pRequest);
        pRequest->groupTag = 0;
        // STRING: FF7 0x008a4068
        pRequest->resourcePath = BattleFx_BuildResourcePath("SPECIAL/KINZAN", pathText);
        Gfx_LoadModelTableIfAbsent(kind, resourceDefsWord, pRequest, &g_BattleSpecCoinTossLoadedResTable, curOwnerCtx);
    }
}

/*
 * Entry point of the Kinzan special: forwards both indices to BattleFx_SpecialBeginCoinToss.
 */
// FUNCTION: FF7 0x00540590
void __cdecl BattleFx_SpecialEnterCoinToss(int targetMask, int actorIdx)
{
    BattleFx_SpecialBeginCoinToss(targetMask, actorIdx);
}

/*
 * Loads the resources, queues the per-target spawn callback and plays the Kinzan sound at the source actor.
 */
// FUNCTION: FF7 0x005405a5
void __cdecl BattleFx_SpecialBeginCoinToss(int targetMask, int actorIdx)
{
    BattleFx_SpecialLoadResCoinToss();
    Battle_CreateBitPatternCallbackTask(targetMask, actorIdx, 4, BattleFx_SpecialSpawnCoinTossPerTarget);
    Battle_PlaySfxPannedToActor(0x11E, (short)actorIdx, 0x40);
}

/*
 * Starts the timeline task for one target: stores the source actor's bone position in
 * g_BattleSpecCoinTossSourceBonePos and the target's bone position and actor index in the task slot.
 */
// FUNCTION: FF7 0x005405da
void __cdecl BattleFx_SpecialSpawnCoinTossPerTarget(int actor, int sourceIndex)
{
    BattleFxCoinTossTimelineTask *slotPtr;

    slotPtr = (BattleFxCoinTossTimelineTask *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialCoinTossTimelineTask);
    Battle_GetBoneWorldPos(sourceIndex, g_BattleActorRecords[sourceIndex].bone34, (short *)&g_BattleSpecCoinTossSourceBonePos);
    Battle_GetBoneWorldPos(actor, g_BattleActorRecords[actor].targetBone, slotPtr->bonePos);
    slotPtr->actorIndex = actor;
}

/*
 * Per-target timeline: ramps the white screen flash and spawns the bouncing coin sprites.
 */
// FUNCTION: FF7 0x0054064e
void __cdecl BattleFx_SpecialCoinTossTimelineTask(void)
{
    short heading;
    short tempRadius;
    short i8;
    BattleFxCoinTossTimelineTask *taskPtr;
    int t; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxCoinTossBounceSlot *pChild;
    short flash;

    taskPtr = (BattleFxCoinTossTimelineTask *)BATTLE_CURRENT_TASK_SLOT;
    flash = g_BattleSpecCoinTossFlashLevel * (taskPtr->frame % 2 + 3) >> 2;
    g_BattleOverlayDrawListHead = BattleFx_FlashScreenWhite((char)flash, (char)flash, flash >> 1);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (taskPtr->frame < 16) {
        g_BattleSpecCoinTossFlashLevel += 4;
    }
    if (taskPtr->frame > 54) {
        g_BattleSpecCoinTossFlashLevel -= 4;
    }
    if (taskPtr->frame < 38) {
        for (i8 = 0; i8 < 3; i8++) {
            pChild = (BattleFxCoinTossBounceSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialCoinTossBouncingSpriteTask);
            pChild->posX = ((short *)&g_BattleSpecCoinTossSourceBonePos)[0];
            pChild->posY = ((short *)&g_BattleSpecCoinTossSourceBonePos)[1];
            pChild->posZ = g_BattleSpecCoinTossSourceBonePosZ;
            heading = rand() % 0x1000;
            tempRadius = rand() % 200;
            pChild->velocityX = SinFixed(heading) * tempRadius >> 12;
            pChild->velocityZ = CosFixed(heading) * tempRadius >> 12;
            pChild->velocityY = 0xFFFFFF38 - rand() % 200;
            pChild->variant = rand() % 4;
            pChild->angle = rand() % 0x1000;
        }
    }
    if (taskPtr->frame == 60) {
        Battle_PostActorHitOrHurtMessage(taskPtr->actorIndex);
    }
    (taskPtr->frame)++;
    if (taskPtr->frame == 70) {
        taskPtr->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * One bouncing coin sprite: draws it as a billboard and steps its position, velocity and spin.
 */
// FUNCTION: FF7 0x00540863
void __cdecl BattleFx_SpecialCoinTossBouncingSpriteTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[92];
        int phase;
        struct { unsigned char pad_to_4[0x4]; SVECTOR rotX; };
        struct { unsigned char pad_to_C[0xC]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_2C[0x2C]; short phaseLevel; };
        struct { unsigned char pad_to_30[0x30]; PsxMatrix scaleMatrix; };
        struct { unsigned char pad_to_50[0x50]; short vecIndex; };
        struct { unsigned char pad_to_54[0x54]; int rotDepth; };
        struct { unsigned char pad_to_58[0x58]; BattleFxTailLaserBouncingSpriteSlot *kinzanSlot; };
    } frame;
#pragma pack(pop)

    frame.kinzanSlot = (BattleFxTailLaserBouncingSpriteSlot *)BATTLE_CURRENT_TASK_SLOT;
    for (frame.vecIndex = 0; frame.vecIndex < 3; frame.vecIndex++) {
        frame.scaleMatrix.m[2][frame.vecIndex] = 0;
        frame.scaleMatrix.m[1][frame.vecIndex] = 0;
        frame.scaleMatrix.m[0][frame.vecIndex] = 0;
    }
    if (frame.kinzanSlot->frameCounter < 22) {
        frame.phase = 8;
    } else {
        frame.phase = 0x1E - frame.kinzanSlot->frameCounter;
    }
    frame.phaseLevel = *(short *)&frame.phase;
    frame.rotX.vy = 0;
    frame.rotX.vx = frame.rotX.vy;
    frame.rotX.vz = frame.kinzanSlot->angle;
    SetRotMatrix(&g_BattleCameraTransform);
    SetTransMatrix(&g_BattleCameraTransform);
    RotTransPoint((SVECTOR *)&frame.kinzanSlot->posX, frame.scaleMatrix.t, &frame.rotDepth);
    frame.scaleMatrix.m[2][2] = 0x1000 /* 1.0 */;
    frame.scaleMatrix.m[1][1] = frame.scaleMatrix.m[2][2];
    frame.scaleMatrix.m[0][0] = frame.scaleMatrix.m[1][1];
    RotMatrix(&frame.rotX, &frame.rotMatrix);
    ConcatMatrixIntoFirst(&frame.scaleMatrix, &frame.rotMatrix);
    SetRotMatrix(&frame.scaleMatrix);
    SetTransMatrix(&frame.scaleMatrix);
    g_BattleSpecCoinTossSpriteReq1.frame = frame.kinzanSlot->variant;
    g_BattleSpecCoinTossSpriteReq1.blue = frame.phaseLevel << 4;
    g_BattleSpecCoinTossSpriteReq1.green = g_BattleSpecCoinTossSpriteReq1.blue;
    (g_BattleSpecCoinTossSpriteReq1.red) = g_BattleSpecCoinTossSpriteReq1.green;
    if (frame.phaseLevel == 8) {
        g_BattleSpecCoinTossSpriteReq1.code = 0x2C;
        g_BattleSpecCoinTossResourceModelRows[8] = Gfx_ModelTableGetLoadedRecord(1, g_BattleSpecCoinTossLoadedResTable);
        g_BattleSpecCoinTossResourceModelRows[9] = Gfx_ModelTableGetSpriteObject(1, g_BattleSpecCoinTossLoadedResTable);
    } else {
        g_BattleSpecCoinTossSpriteReq1.code = 0x2E;
        g_BattleSpecCoinTossResourceModelRows[8] = Gfx_ModelTableGetLoadedRecord(0, g_BattleSpecCoinTossLoadedResTable);
        g_BattleSpecCoinTossResourceModelRows[9] = Gfx_ModelTableGetSpriteObject(0, g_BattleSpecCoinTossLoadedResTable);
    }
    g_BattleSpecCoinTossDrawHandle = BattleFx_SubmitSpriteDraw(&g_BattleSpecCoinTossResourceModelRows[8], g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecCoinTossDrawHandle);
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.kinzanSlot->posX += frame.kinzanSlot->velocityX;
    frame.kinzanSlot->posY += frame.kinzanSlot->velocityY;
    frame.kinzanSlot->posZ += frame.kinzanSlot->velocityZ;
    if (frame.kinzanSlot->posY > 0) {
        frame.kinzanSlot->posY = 0;
        frame.kinzanSlot->velocityY *= -1;
    }
    frame.kinzanSlot->velocityY += 20;
    frame.kinzanSlot->angle += 0x200 /* 45 deg */;
    (frame.kinzanSlot->frameCounter)++;
    if (frame.kinzanSlot->frameCounter == 30) {
        frame.kinzanSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}
} /* extern "C" */
