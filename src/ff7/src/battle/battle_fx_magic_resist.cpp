// src/ff7/src/battle/battle_fx_magic_resist.cpp
/* Magic Effect [6]: Resist (Japanese: レジスト, dev tag: "MAGIC/REZIST" / Rezist) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle visual effect for the Rezist magic: radial fading sprite particles spawned
//   around each target.

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


#pragma intrinsic(abs)

// clang-format off
// GLOBAL: FF7 0x008c97d0
int g_BattleMagicResist_ResourceDef = 0x00192A39;
char *g_BattleFxMagicResistResourceNames[1] = {
    // STRING: FF7 0x008c97f0
    "RESIST.TIM"
};
unsigned int g_BattleFxMagicResistResourceNames2[6] = {
    // STRING: FF7 0x008c97fc
    (unsigned int)"LINE.SPT",
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000064,
    0x00000000
};
// clang-format on
#pragma intrinsic(memset)
#pragma intrinsic(memcpy)


// clang-format off
unsigned char g_bssPad_00bd5aa8[4] = {0};
// GLOBAL: FF7 0x00bd5aac
int g_BattleMagicResist_TargetCount = 0;
// GLOBAL: FF7 0x00bd5ab0
int g_BattleMagicResist_SpritesPerTick = 0;
// GLOBAL: FF7 0x00bd5ab4
struct GfxTableCursor *g_BattleMagicResist_ResourceHandle = 0;
// GLOBAL: FF7 0x00bd5ab8
struct Int64Pair g_BattleMagicResist_ParticleRotation = {0};
// clang-format on

/*
 * Loads resources for the Rezist magic effect when not already loaded.
 */
// FUNCTION: FF7 0x005a1320
void __cdecl BattleFx_MagicLoadResResist(void)
{
    char pathBuf[200];
    GfxResourceRequest requestStorage;
    int kind;
    GfxModelTableBank *gfxCtx;
    GfxResourceRequest *requestPtr;
    int def;

    if (g_pBattle3dScene != 0 && g_BattleMagicResist_ResourceHandle == 0) {
        kind = 1;
        def = (int)&g_BattleMagicResist_ResourceDef;
        gfxCtx = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        requestPtr = &requestStorage;
        Gfx_InitResourceRequest(requestPtr);
        requestPtr->groupTag = 0;
        // STRING: FF7 0x008c9808
        requestPtr->resourcePath = BattleFx_BuildResourcePath("MAGIC/REZIST", pathBuf);
        Gfx_LoadModelTableIfAbsent(kind, def, requestPtr, &g_BattleMagicResist_ResourceHandle, gfxCtx);
    }
}

/*
 * Entry point initializing Rezist magic effect for targets.
 */
// FUNCTION: FF7 0x005a13b0
void __cdecl BattleFx_MagicEnterResist(int targetMask, int actorIdx)
{
    BattleFx_MagicLoadResResist();
    g_BattleMagicResist_TargetCount = Battle_PopCountActorMask(targetMask);
    g_BattleMagicResist_SpritesPerTick = (g_BattleMagicResist_TargetCount - 1 != 0 ? 0xFFFFFFFD : 0) + 6;
    Battle_CreateBitPatternCallbackTask(targetMask, actorIdx, g_BattleMagicResist_TargetCount <= 3 ? 0 : 0xA, BattleFx_MagicSpawnResistPerTarget);
    if (g_BattleMagicResist_TargetCount <= 3) {
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputePanForEntity(targetMask), 0x80);
    }
}

/*
 * Spawns Rezist radial sprite task for a single target.
 */
// FUNCTION: FF7 0x005a1432
void __cdecl BattleFx_MagicSpawnResistPerTarget(int actor)
{
    BattleFxResistTask *task;

    task = (BattleFxResistTask *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicResistSpawnRadialSpritesTask);
    task->targetIndex = actor;
    Battle_GetBoneWorldPos(actor, g_BattleActorRecords[actor].targetBone, &task->posX);
    task->posY = 0;
    if (g_BattleMagicResist_TargetCount > 3) {
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputeScreenPanFromPosition(&task->posX), 0x80);
    }
}

/*
 * Timeline task spawning radial fading sprite particles for Rezist.
 */
// FUNCTION: FF7 0x005a14b5
void __cdecl BattleFx_MagicResistSpawnRadialSpritesTask(void)
{
    BattleFxSwirlColumnTask *taskPtr;
    int i;
    int dataBlock;
    BattleWhiteWindFadingSpriteSlot *newTask;

    taskPtr = (BattleFxSwirlColumnTask *)BATTLE_CURRENT_TASK_SLOT;
    dataBlock = Gfx_GetPsxScratchpadPtr(0);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (taskPtr->step < 10) {
        ((SVECTOR *)&g_BattleMagicResist_ParticleRotation)->vy = rand() & 0xFFF;
        for (i = 0; i < g_BattleMagicResist_SpritesPerTick; i++) {
            newTask = (BattleWhiteWindFadingSpriteSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicResistFadingSpriteTask);
            newTask->origin = taskPtr->origin;
            newTask->scale = rand() % 0x300 + 0x100;
            newTask->angle = g_BattleMagicResist_ParticleRotation;
            newTask->radialSpeed = rand() % 400 + 100 << 4;
            newTask->spinStep = rand() % 0x64 - 0x32;
            ((SVECTOR *)&g_BattleMagicResist_ParticleRotation)->vy += (short)(4096 / g_BattleMagicResist_SpritesPerTick);
        }
    } else {
        Battle_PostActorHitOrHurtMessage(taskPtr->recordType);
        taskPtr->ownerSlot = BATTLE_TASK_FREE;
    }
    (taskPtr->step)++;
}

/*
 * Renders and advances a single fading sprite particle for Rezist.
 */
// FUNCTION: FF7 0x005a160a
void __cdecl BattleFx_MagicResistFadingSpriteTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[36];
        int rotMatrix;
        struct { unsigned char pad_to_4[0x4]; BattleFxWhiteWindFadeState *fadeVelocity; };
        struct { unsigned char pad_to_8[0x8]; BattleFxResistFadingSprite *sprite; };
        struct { unsigned char pad_to_C[0xC]; short quadX; };
        struct { unsigned char pad_to_E[0xE]; short quadY; };
        struct { unsigned char pad_to_10[0x10]; char attrA; };
        struct { unsigned char pad_to_11[0x11]; char attrB; };
        struct { unsigned char pad_to_12[0x12]; char tintA; };
        struct { unsigned char pad_to_13[0x13]; char tintB; };
        struct { unsigned char pad_to_14[0x14]; char shadeC; };
        struct { unsigned char pad_to_15[0x15]; char shadeB; };
        struct { unsigned char pad_to_16[0x16]; char shadeA; };
        struct { unsigned char pad_to_17[0x17]; char texByte; };
        struct { unsigned char pad_to_18[0x18]; short texWord; };
        struct { unsigned char pad_to_1A[0x1A]; short attrWord; };
        struct { unsigned char pad_to_1C[0x1C]; int effectState; };
        struct { unsigned char pad_to_20[0x20]; int age; };
    } frame;
#pragma pack(pop)

    frame.sprite = (BattleFxResistFadingSprite *)BATTLE_CURRENT_TASK_SLOT;
    frame.quadX = 0xFF80;
    frame.quadY = 0xFF80;
    frame.attrA = 0;
    frame.attrB = 0;
    frame.tintA = 0xFF;
    frame.tintB = 0xFF;
    frame.shadeC = 0x80;
    frame.shadeB = 0x80;
    frame.shadeA = 0x80;
    frame.texByte = 0x2E;
    frame.texWord = 0xAC;
    frame.attrWord = 0x3FF0;
    memset(&frame.effectState, 0, 4);
    frame.effectState = Gfx_ModelTableGetSpriteObject(0, g_BattleMagicResist_ResourceHandle);
    frame.fadeVelocity = (BattleFxWhiteWindFadeState *)Gfx_GetPsxScratchpadPtr(0);
    frame.rotMatrix = (int)frame.fadeVelocity + 8;
    frame.age = frame.sprite->timer;
    if (frame.age < 13) {
        frame.shadeA = 0x80;
        frame.shadeB = frame.shadeA;
        frame.shadeC = frame.shadeB;
    } else {
        frame.age -= 13;
        if (frame.age < 4) {
            frame.shadeA = ((frame.age * -0x80) / 4) + 0x80;
            frame.shadeB = frame.shadeA;
            frame.shadeC = frame.shadeB;
        } else {
            frame.sprite->ownerSlot = BATTLE_TASK_FREE;
            return;
        }
    }
    BattleFx_InstallBillboardMatrix((void *)((int)frame.sprite + 8), (short)frame.sprite->scale, 0);
    g_BattleOverlayDrawListHead = BattleFx_SubmitSpriteQuadDraw((BattleFxSpriteQuadRequest *)&frame.quadX, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused == 0) {
        (frame.sprite->timer)++;
        frame.fadeVelocity->velocityX = frame.sprite->radialSpeed;
        frame.fadeVelocity->velocityY = 0;
        frame.fadeVelocity->velocityZ = 0;
        RotMatrix((SVECTOR *)((int)frame.sprite + 0x10), (void *)frame.rotMatrix);
        TransformVectorToInt16((void *)frame.rotMatrix, (void *)frame.fadeVelocity, (void *)frame.fadeVelocity);
        frame.sprite->posX += (short)(frame.fadeVelocity->velocityX >> 4);
        frame.sprite->posY += (short)(frame.fadeVelocity->velocityY >> 4);
        frame.sprite->posZ += (short)(frame.fadeVelocity->velocityZ >> 4);
        frame.sprite->radialSpeed -= (short)(frame.sprite->radialSpeed * 0x300 /* 0.1875 */ >> 12);
        frame.sprite->rotZ += (short)((-0x400 - frame.sprite->rotZ) * 0x300 /* 0.1875 */ >> 12);
        frame.sprite->rotX += frame.sprite->spinStep;
    }
}
} /* extern "C" */
