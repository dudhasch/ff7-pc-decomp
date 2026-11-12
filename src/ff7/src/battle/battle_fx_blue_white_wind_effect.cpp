// src/ff7/src/battle/battle_fx_blue_white_wind_effect.cpp
// Enemy Skill [3]: White Wind (Japanese: ホワイトウィンド, dev tag: "BLUE/HOWIN" / Howin)
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Blue-magic Howin effect: loads its resources, picks the target, and spawns radial
//   sprite particles that fade out over time.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/enums/sound.h"
#include "ff7/battle_tasks.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_fx.h"
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
// GLOBAL: FF7 0x008c9788
int g_BattleBlueWhiteWind_ResourceDef = 0x00192A39;
char *g_BattleFxBlueWhiteWindResourceNames[1] = {
    // STRING: FF7 0x008c97a8
    "W_WIND.TIM"
};
unsigned int g_BattleFxBlueWhiteWindResourceNames2[6] = {
    // STRING: FF7 0x008c97b4
    (unsigned int)"LINE.SPT",
    0x00000000,
    0x00000000,
    0x00000000,
    0x000000c8,
    0x00000000
};
// clang-format on
#pragma intrinsic(memset)
#pragma intrinsic(memcpy)


// clang-format off
// GLOBAL: FF7 0x00bd5a90
int g_BattleBlueWhiteWind_LoadedRes = 0;
// GLOBAL: FF7 0x00bd5a94
int g_BattleBlueWhiteWind_TargetMask = 0;
// GLOBAL: FF7 0x00bd5a98
int g_BattleBlueWhiteWind_ParticleCount = 0;
// GLOBAL: FF7 0x00bd5a9c
struct GfxTableCursor *g_BattleBlueWhiteWind_EffectState = 0;
// GLOBAL: FF7 0x00bd5aa0
struct Int64Pair g_BattleBlueWhiteWind_AnglePair = {0};
// clang-format on

/*
 * Loads resources for the Howin blue magic effect when not already loaded.
 */
// FUNCTION: FF7 0x005a0dc0
void __cdecl BattleFx_BlueLoadResWhiteWind(void)
{
    GfxModelTableBank *loadBits;
    GfxResourceRequest requestRecord;
    int kind;
    char pathBuffer[200];
    GfxResourceRequest *request;
    int defPtr;

    if (g_pBattle3dScene != 0 && g_BattleBlueWhiteWind_EffectState == 0) {
        kind = 1;
        defPtr = (int)&g_BattleBlueWhiteWind_ResourceDef;
        loadBits = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        request = &requestRecord;
        Gfx_InitResourceRequest(request);
        request->groupTag = 0;
        // STRING: FF7 0x008c97c0
        request->resourcePath = BattleFx_BuildResourcePath("BLUE/HOWIN", pathBuffer);
        Gfx_LoadModelTableIfAbsent(kind, defPtr, request, &g_BattleBlueWhiteWind_EffectState, loadBits);
    }
}

/*
 * Entry point for Howin blue magic, selecting target entity and allocating radial sprite task.
 */
// FUNCTION: FF7 0x005a0e50
void __cdecl BattleFx_BlueEnterWhiteWind(int targetMask, int casterIdx)
{
    int actorIdx;
    BattleWhiteWindEnterSlot *task;
    int targetCnt;
    int sel;

    BattleFx_BlueLoadResWhiteWind();
    targetCnt = Battle_PopCountActorMask(targetMask);
    g_BattleBlueWhiteWind_ParticleCount = (targetCnt == 1) ? 6 : 3;
    g_BattleBlueWhiteWind_TargetMask = targetMask;
    if (targetMask & (1 << casterIdx)) {
        sel = casterIdx;
    } else {
        targetCnt = (targetCnt - 1) / 2;
        for (actorIdx = 0; actorIdx < 10; actorIdx++) {
            if (targetMask & (1 << actorIdx)) {
                if (targetCnt == 0) {
                    sel = actorIdx;
                    break;
                }
                targetCnt--;
            }
        }
    }
    task = (BattleWhiteWindEnterSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_BlueWhiteWindSpawnRadialSpritesTask);
    task->actorIndex = sel;
    Battle_GetBoneWorldPos(sel, g_BattleActorRecords[sel].targetBone, &task->bonePosX);
    task->bonePosY = 0;
    Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputePanForEntity(targetMask), 0x81);
}

/*
 * Timeline task spawning radial fading sprite particles for Howin.
 */
// FUNCTION: FF7 0x005a0f69
void __cdecl BattleFx_BlueWhiteWindSpawnRadialSpritesTask(void)
{
    int stackWords; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleTaskSlot *task;
    int particleIdx;
    int dataBlock;
    BattleWhiteWindFadingSpriteSlot *nodePtr;

    task = BATTLE_CURRENT_TASK_SLOT;
    dataBlock = Gfx_GetPsxScratchpadPtr(0);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (task->frameCounter < 0xA) {
        ((SVECTOR *)&g_BattleBlueWhiteWind_AnglePair)->vy = rand() & 0xFFF;
        for (particleIdx = 0; particleIdx < g_BattleBlueWhiteWind_ParticleCount; particleIdx++) {
            nodePtr = (BattleWhiteWindFadingSpriteSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_BlueWhiteWindFadingSpriteTask);
            nodePtr->origin = ((BattleWhiteWindFadingSpriteSlot *)task)->origin;
            nodePtr->scale = rand() % 0xC00 + 0x400;
            nodePtr->angle = g_BattleBlueWhiteWind_AnglePair;
            nodePtr->radialSpeed = rand() % 0x258 + 0xC8 << 4;
            nodePtr->spinStep = rand() % 0x64 - 0x32;
            ((SVECTOR *)&g_BattleBlueWhiteWind_AnglePair)->vy += (short)(0x1000 / g_BattleBlueWhiteWind_ParticleCount);
        }
    } else {
        for (particleIdx = 0; particleIdx < 0xA; particleIdx++) {
            if ((g_BattleBlueWhiteWind_TargetMask & 1 << particleIdx) != 0) {
                Battle_PostActorHitOrHurtMessage(particleIdx);
            }
        }
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    (task->frameCounter)++;
}

/*
 * Renders and advances a single fading sprite particle for Howin.
 */
// FUNCTION: FF7 0x005a10ed
void __cdecl BattleFx_BlueWhiteWindFadingSpriteTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[36];
        PsxMatrix *rotMatrix;
        struct { unsigned char pad_to_4[0x4]; BattleFxWhiteWindFadeState *fadeState; };
        struct { unsigned char pad_to_8[0x8]; BattleFxWhiteWindFadeSprite *sprite; };
        struct { unsigned char pad_to_C[0xC]; short originX; };
        struct { unsigned char pad_to_E[0xE]; short originY; };
        struct { unsigned char pad_to_10[0x10]; char reqFlags; };
        struct { unsigned char pad_to_11[0x11]; char reqMode; };
        struct { unsigned char pad_to_12[0x12]; char tintA; };
        struct { unsigned char pad_to_13[0x13]; char tintB; };
        struct { unsigned char pad_to_14[0x14]; char shadeB; };
        struct { unsigned char pad_to_15[0x15]; char shadeG; };
        struct { unsigned char pad_to_16[0x16]; char shadeR; };
        struct { unsigned char pad_to_17[0x17]; char uvU; };
        struct { unsigned char pad_to_18[0x18]; short uvV; };
        struct { unsigned char pad_to_1A[0x1A]; short scale; };
        struct { unsigned char pad_to_1C[0x1C]; int tableField; };
        struct { unsigned char pad_to_20[0x20]; int phase; };
    } frame;
#pragma pack(pop)

    frame.sprite = (BattleFxWhiteWindFadeSprite *)BATTLE_CURRENT_TASK_SLOT;
    frame.originX = 0xFFC0;
    frame.originY = 0xFFC0;
    frame.reqFlags = 0;
    frame.reqMode = 0;
    frame.tintA = 0x7F;
    frame.tintB = 0x7F;
    frame.shadeB = 0x80;
    frame.shadeG = 0x80;
    frame.shadeR = 0x80;
    frame.uvU = 0x2E;
    frame.uvV = 0xAC;
    frame.scale = 0x3FF0;
    memset(&frame.tableField, 0, 4);
    frame.tableField = Gfx_ModelTableGetSpriteObject(0, g_BattleBlueWhiteWind_EffectState);
    frame.fadeState = (BattleFxWhiteWindFadeState *)Gfx_GetPsxScratchpadPtr(0);
    frame.rotMatrix = (PsxMatrix *)((int)frame.fadeState + 8);
    frame.phase = frame.sprite->frame;
    if (frame.phase < 0xD) {
        frame.shadeR = 0x80;
        frame.shadeG = frame.shadeR;
        frame.shadeB = frame.shadeG;
    } else {
        frame.phase -= 0xD;
        if (frame.phase < 4) {
            frame.shadeR = ((frame.phase * -0x80) / 4) + 0x80;
            frame.shadeG = frame.shadeR;
            frame.shadeB = frame.shadeG;
        } else {
            frame.sprite->ownerSlot = BATTLE_TASK_FREE;
            return;
        }
    }
    BattleFx_InstallBillboardMatrix(&frame.sprite->posX, (short)frame.sprite->scale, 0);
    g_BattleBlueWhiteWind_LoadedRes = BattleFx_SubmitSpriteQuadDraw((BattleFxSpriteQuadRequest *)&frame.originX, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleBlueWhiteWind_LoadedRes);
    if (g_BattleIsPaused == 0) {
        (frame.sprite->frame)++;
        frame.fadeState->velocityX = frame.sprite->fadeLevel;
        frame.fadeState->velocityY = 0;
        frame.fadeState->velocityZ = 0;
        RotMatrix((SVECTOR *)&frame.sprite->angleX, frame.rotMatrix);
        TransformVectorToInt16(frame.rotMatrix, (void *)frame.fadeState, (void *)frame.fadeState);
        frame.sprite->posX += (short)(frame.fadeState->velocityX >> 4);
        frame.sprite->posY += (short)(frame.fadeState->velocityY >> 4);
        frame.sprite->posZ += (short)(frame.fadeState->velocityZ >> 4);
        frame.sprite->fadeLevel -= (short)(frame.sprite->fadeLevel << 9 >> 0xC);
        frame.sprite->angleZ += (short)(-0x400 - frame.sprite->angleZ << 9 >> 0xC);
        frame.sprite->angleX += frame.sprite->spinStep;
    }
}
} /* extern "C" */
