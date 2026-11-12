// src/ff7/src/battle/battle_fx_magic_bolt2.cpp
/* Magic Effect [34]: Bolt2 (Japanese: サンダラ Thundara, dev tag: "MAGIC/THUNDERA" / Thundera) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle effect for the Thundara (Thundera) magic: loads its resources and runs a
//   timeline that spawns growing-and-fading models plus billboard and flipped sprites.

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

// clang-format off
// GLOBAL: FF7 0x008fdd60
int g_BattleMagicBolt2_ResourceDefs = 0x00192A39;
int g_BattleMagicBolt2_ResourceDefsHeadPtr[1] = {
    (int)((char *)&g_BattleFxMagicBolt2ResourceNames + 0x0)
};
int g_BattleMagicBolt2_ResourceDefsDataTail[24] = {
    (int)((char *)&g_BattleFxMagicBolt2ResourceNames + 0x10), 0x0, 0x0, 0x192a39, (int)((char *)&g_BattleFxMagicBolt2ResourceNames + 0x1c), (int)((char *)&g_BattleFxMagicBolt2ResourceNames + 0x2c),
    0x0, 0x0, 0x192a39, (int)((char *)&g_BattleFxMagicBolt2ResourceNames + 0x38), (int)((char *)&g_BattleFxMagicBolt2ResourceNames + 0x48), 0x0,
    0x0, 0x192a39, (int)((char *)&g_BattleFxMagicBolt2ResourceNames + 0x54), (int)((char *)&g_BattleFxMagicBolt2ResourceNames + 0x64), 0x0, 0x0,
    0x1900b1, (int)((char *)&g_BattleFxMagicBolt2ResourceNames + 0x70), 0x0, 0x0, 0x0, 0x0
};
// clang-format on

// clang-format off
// The sprite request the billboard task fills and submits.
// GLOBAL: FF7 0x008fddc8
BattleFxSpriteRequest g_BattleMagicBolt2_ModelReq = {0, 0, (char)0x80, (char)0x80, (char)0x80, 0x2c, 0, 0};
// GLOBAL: FF7 0x008fddd8
short g_BattleMagicBolt2_Scale[4] = {8192, 0, 0, 0};
// GLOBAL: FF7 0x008fdde0
struct BattleMagicBolt2_SpriteReq g_BattleMagicBolt2_SpriteReq = {0x2000, {0, 0, 0, 0, 0, 0, 0, 0x20}, 0, 0, 0, {0}};
unsigned char g_BattleFxMagicBolt2ResourceNames[124] =
    "THUNDE00.TIM\0\0\0\0"
    "KAMI_2.SPT\0\0"
    "THUNDE01.TIM\0\0\0\0"
    "SPARK_2.SPT\0"
    "THUNDE02.TIM\0\0\0\0"
    "SPARK_3.SPT\0"
    "THUNDE01.TIM\0\0\0\0"
    "ELEC_1.SPT\0\0"
    "THUND_4.RSD";
// clang-format on

#pragma intrinsic(memset)
#pragma intrinsic(memcpy)


// clang-format off
// GLOBAL: FF7 0x00be0dac
int g_BattleMagicBolt2_ChainTail = 0;
unsigned char g_bssPad_00be0db0[4] = {0};
// GLOBAL: FF7 0x00be0db4
struct GfxTableCursor *g_BattleMagicBolt2_ResourceHandle = 0;
// clang-format on

/* Loads the Thundara (Thundera) magic resource package if not already loaded. */
// FUNCTION: FF7 0x005b8570
void __cdecl BattleFx_MagicLoadResBolt2(void)
{
    GfxModelTableBank *stage;
    GfxResourceRequest requestSlot;
    int kind;
    char pathText[200];
    GfxResourceRequest *pRequest;
    int resourceTable;

    if (g_pBattle3dScene != 0 && g_BattleMagicBolt2_ResourceHandle == 0) {
        kind = 5;
        resourceTable = (int)&g_BattleMagicBolt2_ResourceDefs;
        stage = (GfxModelTableBank *)g_pBattle3dScene->modelTableBank;
        pRequest = &requestSlot;
        Gfx_InitResourceRequest(pRequest);
        ((BattleFxDiamondCardResource *)pRequest)->groupTag = 0;
        // STRING: FF7 0x008fde74
        pRequest->resourcePath = BattleFx_BuildResourcePath("MAGIC/THUNDERA", pathText);
        Gfx_LoadModelTableIfAbsent(kind, resourceTable, pRequest, &g_BattleMagicBolt2_ResourceHandle, stage);
    }
}

/* Entry point for the Thundara (Thundera) magic effect. */
// FUNCTION: FF7 0x005b8600
void __cdecl BattleFx_MagicEnterBolt2(int targetMask, int actorIdx)
{
    BattleFx_MagicLoadResBolt2();
    BattleFx_MagicBeginBolt2(targetMask, actorIdx);
}

/* Initializes parameters and spawns per-target callback for Thundara (Thundera). */
// FUNCTION: FF7 0x005b861a
void __cdecl BattleFx_MagicBeginBolt2(int targetMask, int actorIdx)
{
    Battle_CreateBitPatternCallbackTask(targetMask, actorIdx, 2, BattleFx_MagicSpawnBolt2PerTarget);
    Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputePanForEntity(targetMask), 0xC);
}

/* Spawns the timeline task for Thundara (Thundera) per target actor. */
// FUNCTION: FF7 0x005b864f
void __cdecl BattleFx_MagicSpawnBolt2PerTarget(int actor)
{
    BattleFxBolt2Record *record;

    record = (BattleFxBolt2Record *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicBolt2TimelineTask);
    Battle_GetBoneWorldPos(actor, g_BattleActorRecords[actor].targetBone, record->bonePos);
    record->negRadius = -g_BattleActorRecords[actor].radius;
    record->actorId = actor;
}

/* Manages the timeline and spawns models and sprites for Thundara (Thundera). */
// FUNCTION: FF7 0x005b86ba
void __cdecl BattleFx_MagicBolt2TimelineTask(void)
{
    BattleFxMagicBolt2Timeline *task;
    BattleFxBolt2Slot *child;

    task = (BattleFxMagicBolt2Timeline *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused == 0) {
        if (task->step == 1) {
            child = (BattleFxBolt2Slot *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicBolt2FadeBillboardSpriteTask);
            child->position = task->position;
            child->posY = 0;
            child->pushLength = task->pushLength;
            Battle_PostActorHitOrHurtMessage(task->allocType);
        }
        if (task->step >= 0 && task->step <= 0x14 && (task->step & 1) == 0) {
            child = (BattleFxBolt2Slot *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicBolt2GrowFadeModelTask);
            child->posX = task->posX + rand() % 0x7D0 + -0x3E8;
            child->posY = 0;
            child->posZ = task->posZ + rand() % 0x7D0 + -0x3E8;
            child->scale = 0x1000;
            child->velocity = 0x800;
        }
        if (task->step == 8) {
            child = (BattleFxBolt2Slot *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicBolt2BillboardSpriteTask);
            child->position = task->position;
            child->pushLength = task->pushLength;
        }
        if (task->step >= 0 && task->step <= 0xF) {
            child = (BattleFxBolt2Slot *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicBolt2FlippedSpriteTask);
            child->posX = task->posX + rand() % 0x3E8 + -0x1F4;
            child->posY = task->posY + rand() % 0x3E8 + -0x1F4;
            child->posZ = task->posZ + rand() % 0x3E8 + -0x1F4;
            child->flags = rand() & 7;
            child->pushLength = task->pushLength;
        }
        if ((task->step)++ == 0x14) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/* Updates and renders a growing and fading model for Thundara (Thundera). */
// FUNCTION: FF7 0x005b891c
void __cdecl BattleFx_MagicBolt2GrowFadeModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[44];
        short fadeLevel;
        struct { unsigned char pad_to_4[0x4]; PsxMatrix posX; };
        struct { unsigned char pad_to_24[0x24]; BattleFxBolt2GrowFade *growFade; };
        struct { unsigned char pad_to_28[0x28]; BattleFxModelDraw *modelDraw; };
    } frame;
#pragma pack(pop)

    frame.growFade = (BattleFxBolt2GrowFade *)BATTLE_CURRENT_TASK_SLOT;
    frame.modelDraw = (BattleFxModelDraw *)Gfx_GetPsxScratchpadPtr(0);
    frame.fadeLevel = 0x80 - (frame.growFade->step << 4);
    frame.posX.m[2][1] = frame.growFade->offset;
    frame.posX.m[0][0] = frame.posX.m[2][1];
    frame.posX.m[1][2] = -frame.growFade->offset;
    frame.posX.m[2][2] = 0;
    frame.posX.m[2][0] = frame.posX.m[2][2];
    frame.posX.m[1][1] = frame.posX.m[2][0];
    frame.posX.m[1][0] = frame.posX.m[1][1];
    frame.posX.m[0][2] = frame.posX.m[1][0];
    frame.posX.m[0][1] = frame.posX.m[0][2];
    frame.posX.t[0] = frame.growFade->posX;
    frame.posX.t[1] = frame.growFade->posY;
    frame.posX.t[2] = frame.growFade->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.posX, &frame.posX);
    SetRotMatrix(&frame.posX);
    SetTransMatrix(&frame.posX);
    frame.modelDraw->flags = 8;
    frame.modelDraw->uvScroll = 0;
    frame.modelDraw->fadeLevel = frame.fadeLevel;
    frame.modelDraw->stateFlags = 0x20;
    frame.modelDraw->texturePageBits = 0;
    frame.modelDraw->modelGroup = Gfx_ModelTableGetRenderObject(4, g_BattleMagicBolt2_ResourceHandle);
    g_BattleMagicBolt2_ChainTail = BattleFx_SubmitModelDraw(frame.modelDraw, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleMagicBolt2_ChainTail);
    if (g_BattleIsPaused == 0) {
        frame.growFade->offset += frame.growFade->velocity;
        frame.growFade->velocity -= (short)(frame.growFade->velocity >> 2);
        if ((frame.growFade->step)++ == 7) {
            frame.growFade->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/* Updates and renders a fading billboard sprite for Thundara (Thundera). */
// FUNCTION: FF7 0x005b8abd
void __cdecl BattleFx_MagicBolt2FadeBillboardSpriteTask(void)
{
    BattleFxBolt2FadeSlot *task;
    BattleFxBolt2FadeSprite *pSprite;

    task = (BattleFxBolt2FadeSlot *)BATTLE_CURRENT_TASK_SLOT;
    pSprite = (BattleFxBolt2FadeSprite *)Gfx_GetPsxScratchpadPtr(0);
    pSprite->spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(0, g_BattleMagicBolt2_ResourceHandle);
    pSprite->drawObj = Gfx_ModelTableGetSpriteObject(0, g_BattleMagicBolt2_ResourceHandle);
    pSprite->index = 0;
    pSprite->fadeLevel = 0xFF - (task->fadeStep << 6);
    pSprite->fadePrev = pSprite->fadeLevel;
    pSprite->fadeOlder = pSprite->fadePrev;
    pSprite->primCode = 0x2C;
    BattleFx_InstallBillboardMatrix(&task->posX, 0x2000, task->pushLength);
    g_BattleMagicBolt2_ChainTail = BattleFx_SubmitSpriteDraw(pSprite, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleMagicBolt2_ChainTail);
    if (g_BattleIsPaused == 0) {
        if ((task->fadeStep)++ == 3) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/* Updates and renders a billboard sprite for Thundara (Thundera). */
// FUNCTION: FF7 0x005b8bc2
void __cdecl BattleFx_MagicBolt2BillboardSpriteTask(void)
{
    BattleFxBolt2BillboardSlot *task;

    task = (BattleFxBolt2BillboardSlot *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleMagicBolt2_ModelReq.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(3, g_BattleMagicBolt2_ResourceHandle);
    g_BattleMagicBolt2_ModelReq.drawObj = Gfx_ModelTableGetSpriteObject(3, g_BattleMagicBolt2_ResourceHandle);
    BattleFx_InstallBillboardMatrix(&task->posX, 0x3000, task->pushLength);
    g_BattleMagicBolt2_ModelReq.frame = task->frameCounter >> 1;
    g_BattleMagicBolt2_ChainTail = BattleFx_SubmitSpriteDraw(&g_BattleMagicBolt2_ModelReq, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleMagicBolt2_ChainTail);
    if (g_BattleIsPaused == 0) {
        if ((task->frameCounter)++ == 0xF) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/* Updates and renders a flipped sprite for Thundara (Thundera). */
// FUNCTION: FF7 0x005b8c8a
void __cdecl BattleFx_MagicBolt2FlippedSpriteTask(void)
{
    BattleFxBolt2FlipSprite *taskPtr;
    int screenZ;
    BattleFxBolt2FlippedSprite *spriteData;

    taskPtr = (BattleFxBolt2FlipSprite *)BATTLE_CURRENT_TASK_SLOT;
    spriteData = (BattleFxBolt2FlippedSprite *)Gfx_GetPsxScratchpadPtr(0);
    SetRotMatrix(&g_BattleCameraTransform);
    SetTransMatrix(&g_BattleCameraTransform);
    RotTransPoint((SVECTOR *)&taskPtr->posX, &g_BattleMagicBolt2_SpriteReq.transX, &screenZ);
    g_BattleMagicBolt2_SpriteReq.transZ += taskPtr->depthStep;
    if ((taskPtr->flags & 1) != 0) {
        g_BattleMagicBolt2_Scale[0] = 0xE000;
    } else {
        g_BattleMagicBolt2_Scale[0] = 0x2000;
    }
    if ((taskPtr->flags & 2) != 0) {
        g_BattleMagicBolt2_SpriteReq.rotM11 = 0xE000;
    } else {
        g_BattleMagicBolt2_SpriteReq.rotM11 = 0x2000;
    }
    SetRotMatrix((PsxMatrix *)g_BattleMagicBolt2_Scale);
    SetTransMatrix((PsxMatrix *)g_BattleMagicBolt2_Scale);
    if ((taskPtr->flags & 4) != 0) {
        spriteData->sourceFirst = Gfx_ModelTableGetLoadedRecord(1, g_BattleMagicBolt2_ResourceHandle);
        spriteData->sourceSecond = Gfx_ModelTableGetSpriteObject(1, g_BattleMagicBolt2_ResourceHandle);
    } else {
        spriteData->sourceFirst = Gfx_ModelTableGetLoadedRecord(2, g_BattleMagicBolt2_ResourceHandle);
        spriteData->sourceSecond = Gfx_ModelTableGetSpriteObject(2, g_BattleMagicBolt2_ResourceHandle);
    }
    spriteData->primCommand = 0x2C808080;
    spriteData->frame = taskPtr->frame;
    g_BattleMagicBolt2_ChainTail = BattleFx_SubmitSpriteDraw(spriteData, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleMagicBolt2_ChainTail);
    if (g_BattleIsPaused == 0 && (taskPtr->frame)++ == 7) {
        taskPtr->ownerSlot = BATTLE_TASK_FREE;
    }
}
} /* extern "C" */
