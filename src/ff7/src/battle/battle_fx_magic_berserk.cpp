// src/ff7/src/battle/battle_fx_magic_berserk.cpp
/* Magic Effect [14]: Berserk (Japanese: バーサク, dev tag: "MAGIC/BARSAC" / Barsac) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle visual effect for the Barsac magic: a rage sequence with a spinning skull model,
//   fading aura sprites and falling anger steam sprites for each target.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/enums/sound.h"
#include "ff7/battle_tasks.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_magic.h"
#include "lib/types/graphics.h"
#include <stdlib.h>
#include "ff7/camera_script.h"
#include <string.h>
#include "ff7/api/battle.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "lib/codegen.h"
extern "C" {

struct BattleFxModelDraw;

#pragma intrinsic(abs)

#pragma intrinsic(memset)
#pragma intrinsic(memcpy)

// clang-format off
// GLOBAL: FF7 0x008fc410
int g_BattleMagicBerserk_ResourceDefs = 0x006440B1;
char *g_BattleFxMagicBerserkResourceNames[1] = {
    // STRING: FF7 0x008fc460
    "BARSAC_1.RSD"
};
unsigned int g_BattleMagicBerserk_ResourceDefsDataTail[14] = {
    0x00000000,
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x008fc470
    (unsigned int)"ELEC_1.TIM",
    // STRING: FF7 0x008fc47c
    (unsigned int)"ELEC_1.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x008fc488
    (unsigned int)"SMOKE_1.TIM",
    // STRING: FF7 0x008fc494
    (unsigned int)"SMOKE_1.SPT",
    0x00000000,
    0x00000000,
    0x00000000
};
// GLOBAL: FF7 0x008fc450
struct BattleMagicBerserk_SpriteReq g_BattleMagicBerserk_SpriteReq = {0, {0x48, 0, 0, 0}, 0, 128};
// GLOBAL: FF7 0x008fc45c
short g_BattleMagicBerserk_Config = 32;
// GLOBAL: FF7 0x00be0460
struct BattleMagicBerserk_LoadedRes g_BattleMagicBerserk_LoadedRes = {0};
unsigned char g_bssPad_00be046c[4] = {0};
// clang-format on


/*
 * Loads resources for the Barsac magic effect when not already loaded.
 */
// FUNCTION: FF7 0x005b31f0
void __cdecl BattleFx_MagicLoadResBerserk(void)
{
    GfxModelTableBank *baseOffset;
    GfxResourceRequest requestRecord;
    int kind;
    char pathEnd[200];
    GfxResourceRequest *request;
    int resultDefCatalog;

    if (g_pBattle3dScene != 0 && g_BattleMagicBerserk_LoadedRes.cursor == 0) {
        kind = 3;
        resultDefCatalog = (int)&g_BattleMagicBerserk_ResourceDefs;
        baseOffset = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        request = &requestRecord;
        Gfx_InitResourceRequest(request);
        request->groupTag = 0;
        // STRING: FF7 0x008fc4a0
        request->resourcePath = BattleFx_BuildResourcePath("MAGIC/BARSAC", pathEnd);
        Gfx_LoadModelTableIfAbsent(kind, resultDefCatalog, request, &g_BattleMagicBerserk_LoadedRes.cursor, baseOffset);
    }
}

/*
 * Main entry point initializing render state for Barsac.
 */
// FUNCTION: FF7 0x005b3280
void __cdecl BattleFx_MagicEnterBerserk(int targetMask, int actorIdx)
{
    int slotOffset;

    BattleFx_MagicLoadResBerserk();
    slotOffset = Battle_PopCountActorMask(targetMask) <= 3 ? 0 : 8;
    Battle_CreateBitPatternCallbackTask(targetMask, actorIdx, slotOffset, BattleFx_MagicSpawnBerserkPerTarget);
}

/*
 * Spawns timeline task for a single Barsac target.
 */
// FUNCTION: FF7 0x005b32c4
void __cdecl BattleFx_MagicSpawnBerserkPerTarget(int actor, int sourceActor)
{
    int unused; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxBerserkEffect *effectSlot;

    effectSlot = (BattleFxBerserkEffect *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicBerserkTimelineTask);
    effectSlot->actorIndex = actor;
    effectSlot->sourceActor = sourceActor;
    Battle_GetBoneWorldPos(actor, g_BattleActorRecords[actor].targetBone, (short *)((int)effectSlot + 8));
    Battle_PostActorHitOrHurtMessage(actor);
    Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputeScreenPanFromPosition(&g_BattleActorRecords[actor].posX), 0x42);
}

/*
 * Drives the main timeline and rage effect sequence for Barsac.
 */
// FUNCTION: FF7 0x005b3354
void __cdecl BattleFx_MagicBerserkTimelineTask(void)
{
    int angle;
    BattleFxBerserkTimelineTask *timeline;
    int i;
    BattleFxBerserkSpriteTask *pSprite;

    timeline = (BattleFxBerserkTimelineTask *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (timeline->step == 0) {
        pSprite = (BattleFxBerserkSpriteTask *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicBerserkSpinningModelTask);
        *(Int64Pair *)((char *)pSprite + 8) = *(Int64Pair *)((char *)timeline + 8);
        pSprite->posHeight = 0;
    }
    if (timeline->step == 0) {
        pSprite = (BattleFxBerserkSpriteTask *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicBerserkFadingSpriteTask);
        *(Int64Pair *)((char *)pSprite + 8) = *(Int64Pair *)((char *)timeline + 8);
        pSprite->orbitY = -g_BattleActorRecords[timeline->targetIndex].radius;
    }
    if (timeline->step >= 0 && timeline->step < 8) {
        for (i = 0; i < 3; i++) {
            pSprite = (BattleFxBerserkSpriteTask *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicBerserkFallingSpriteTask);
            *(Int64Pair *)((char *)pSprite + 8) = *(Int64Pair *)((char *)timeline + 8);
            pSprite->posHeight = 0;
            angle = rand() & 0xFFF;
            pSprite->posX += (short)(SinFixed(angle) * 0xFA >> 0xC);
            pSprite->posY += (short)(CosFixed(angle) * 0xFA >> 0xC);
            pSprite->orbitX = SinFixed(angle) * 0x46 >> 0xC;
            pSprite->orbitZ = CosFixed(angle) * 0x46 >> 0xC;
            pSprite->velocityHeight = 0;
        }
    }
    if (timeline->step == 8) {
        timeline->ownerSlot = BATTLE_TASK_FREE;
    }
    (timeline->step)++;
}

/*
 * Animates and renders rotating skull rage model for Barsac.
 */
// FUNCTION: FF7 0x005b3549
void __cdecl BattleFx_MagicBerserkSpinningModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[104];
        struct { unsigned char pad_to_20[0x20]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_40[0x40]; int angleBits; };
        struct { unsigned char pad_to_44[0x44]; BattleFxBerserkSpinRecord *spinRecord; };
        struct { unsigned char pad_to_48[0x48]; VECTOR scaleX; };
    } frame;
#pragma pack(pop)

    frame.spinRecord = (BattleFxBerserkSpinRecord *)BATTLE_CURRENT_TASK_SLOT;
    (g_BattleMagicBerserk_SpriteReq.modelGroup) = Gfx_ModelTableGetRenderObject(0, g_BattleMagicBerserk_LoadedRes.cursor);
    frame.angleBits = frame.spinRecord->angle & 3;
    g_BattleMagicBerserk_SpriteReq.uvOffset = (frame.angleBits & 1) << 7;
    g_BattleMagicBerserk_Config = (frame.angleBits & 2) >> 1 | 0x20;
    g_BattleMagicBerserk_LoadedRes.alpha = frame.spinRecord->angle * 0x50 & 0xFFF;
    if (frame.spinRecord->angle < 0) {
        frame.scaleX.vz = (frame.spinRecord->angle << 0xC) / 0;
        frame.scaleX.vy = frame.scaleX.vz;
        frame.scaleX.vx = frame.scaleX.vy;
    } else {
        frame.scaleX.vz = 0x1000;
        frame.scaleX.vy = frame.scaleX.vz;
        frame.scaleX.vx = frame.scaleX.vy;
    }
    if (frame.spinRecord->angle < 0xC) {
        g_BattleMagicBerserk_SpriteReq.alpha = 0x80;
    } else {
        g_BattleMagicBerserk_SpriteReq.alpha = 0x80 - (frame.spinRecord->angle - 0xC << 7) / 0xA;
    }
    SetDrawColorRGB(0, 0, 0);
    RotMatrix((SVECTOR *)&g_BattleMagicBerserk_LoadedRes.rotByte8, &frame.rotMatrix);
    ScaleMatrix(&frame.rotMatrix, &frame.scaleX);
    frame.rotMatrix.t[0] = frame.spinRecord->posX;
    frame.rotMatrix.t[1] = frame.spinRecord->posY;
    frame.rotMatrix.t[2] = frame.spinRecord->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.rotMatrix);
    SetRotMatrix(&frame.rotMatrix);
    SetTransMatrix(&frame.rotMatrix);
    (g_BattleMagicBerserk_LoadedRes.drawHandle) = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleMagicBerserk_SpriteReq, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, (g_BattleMagicBerserk_LoadedRes.drawHandle));
    if (g_BattleIsPaused == 0) {
        (frame.spinRecord->angle)++;
        if (frame.spinRecord->angle >= 0x16) {
            frame.spinRecord->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Updates and renders fading rage aura sprites for Barsac.
 */
// FUNCTION: FF7 0x005b3709
void __cdecl BattleFx_MagicBerserkFadingSpriteTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[20];
        int tableWordA;
        struct { unsigned char pad_to_4[0x4]; int tableWordB; };
        struct { unsigned char pad_to_8[0x8]; char colorR; };
        struct { unsigned char pad_to_9[0x9]; char colorG; };
        struct { unsigned char pad_to_A[0xA]; char colorB; };
        struct { unsigned char pad_to_B[0xB]; char colorCode; };
        struct { unsigned char pad_to_C[0xC]; int frameMod4; };
        struct { unsigned char pad_to_10[0x10]; BattleFxBerserkFadeSprite *sprite; };
    } frame;
#pragma pack(pop)

    frame.sprite = (BattleFxBerserkFadeSprite *)BATTLE_CURRENT_TASK_SLOT;
    frame.tableWordA = 0;
    frame.tableWordB = 0;
    frame.colorR = 0x80;
    frame.colorG = 0x80;
    frame.colorB = 0x80;
    frame.colorCode = 0x2E;
    memset(&frame.frameMod4, 0, 4);
    frame.tableWordA = Gfx_ModelTableGetLoadedRecord(1, g_BattleMagicBerserk_LoadedRes.cursor);
    frame.tableWordB = Gfx_ModelTableGetSpriteObject(1, g_BattleMagicBerserk_LoadedRes.cursor);
    *(short *)&frame.frameMod4 = frame.sprite->frame & 3;
    if (frame.sprite->frame < 0xC) {
        frame.colorB = 0x80;
        frame.colorG = frame.colorB;
        frame.colorR = frame.colorG;
    } else {
        frame.colorB = 0x80 - (frame.sprite->frame - 0xC << 7) / 5;
        frame.colorG = frame.colorB;
        frame.colorR = frame.colorG;
    }
    BattleFx_InstallBillboardMatrix((void *)((int)frame.sprite + 8), 0x2500, frame.sprite->ringIndex);
    (g_BattleMagicBerserk_LoadedRes.drawHandle) = BattleFx_SubmitSpriteDraw(&frame.tableWordA, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, (g_BattleMagicBerserk_LoadedRes.drawHandle));
    if (g_BattleIsPaused == 0) {
        (frame.sprite->frame)++;
        if (frame.sprite->frame >= 0x11) {
            frame.sprite->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Updates and renders descending anger steam sprites for Barsac.
 */
// FUNCTION: FF7 0x005b3837
void __cdecl BattleFx_MagicBerserkFallingSpriteTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[20];
        int tableWordA;
        struct { unsigned char pad_to_4[0x4]; int tableWordB; };
        struct { unsigned char pad_to_8[0x8]; char colorR; };
        struct { unsigned char pad_to_9[0x9]; char colorG; };
        struct { unsigned char pad_to_A[0xA]; char colorB; };
        struct { unsigned char pad_to_B[0xB]; char colorCode; };
        struct { unsigned char pad_to_C[0xC]; int spriteFrame; };
        struct { unsigned char pad_to_10[0x10]; BattleFxBerserkFallingSprite *sprite; };
    } frame;
#pragma pack(pop)

    frame.sprite = (BattleFxBerserkFallingSprite *)BATTLE_CURRENT_TASK_SLOT;
    frame.tableWordA = 0;
    frame.tableWordB = 0;
    frame.colorR = 0x80;
    frame.colorG = 0x80;
    frame.colorB = 0x80;
    frame.colorCode = 0x2E;
    memset(&frame.spriteFrame, 0, 4);
    frame.tableWordA = Gfx_ModelTableGetLoadedRecord(2, g_BattleMagicBerserk_LoadedRes.cursor);
    frame.tableWordB = Gfx_ModelTableGetSpriteObject(2, g_BattleMagicBerserk_LoadedRes.cursor);
    *(short *)&frame.spriteFrame = frame.sprite->frame;
    BattleFx_InstallBillboardMatrix((void *)((int)frame.sprite + 8), 0x1500, 0);
    (g_BattleMagicBerserk_LoadedRes.drawHandle) = BattleFx_SubmitSpriteDraw(&frame.tableWordA, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, (g_BattleMagicBerserk_LoadedRes.drawHandle));
    if (g_BattleIsPaused == 0) {
        frame.sprite->posX += frame.sprite->velocityX;
        frame.sprite->posY += frame.sprite->velocityY;
        frame.sprite->posZ += frame.sprite->velocityZ;
        frame.sprite->velocityY -= 8;
        (frame.sprite->frame)++;
        if (frame.sprite->frame >= 0xE) {
            frame.sprite->slotTag = 0xFFFF;
        }
    }
}
} /* extern "C" */
