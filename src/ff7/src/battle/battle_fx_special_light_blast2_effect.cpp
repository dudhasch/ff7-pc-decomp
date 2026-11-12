// src/ff7/src/battle/battle_fx_special_light_blast2_effect.cpp
/* Enemy Special Attack [100]: Light Blast 2 (orig. SPECIAL/HIKARI2, 光2) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle special effect 'Hikari2' (light): a white screen flash with a linear fade in and
//   out, a per-target timeline, and animated sprite entry tasks.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/api/battle.h"
#include "ff7/battle_tasks.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "ff7/types/battle.h"
#include "ff7/camera_script.h"
#include "lib/types/graphics.h"
#include "ff7/types/battle_3d.h"
#include <string.h>
#include <stdlib.h>
#include "lib/codegen.h"
extern "C" {

// clang-format off
// GLOBAL: FF7 0x008a2550
int g_BattleSpecLightBlast2ResourceDefs = 0x00192A39;
char *g_BattleFxSpecialLightBlast2EffectResourceNames[1] = {
    // STRING: FF7 0x008a2598
    "HIKARI1.TIM"
};
unsigned int g_BattleFxSpecialLightBlast2EffectResourceNames2[8] = {
    // STRING: FF7 0x008a25a4
    (unsigned int)"HIKARI1.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x008a25b0
    (unsigned int)"HIT1.TIM",
    // STRING: FF7 0x008a25bc
    (unsigned int)"HIT1.SPT",
    0x00000000,
    0x00000000
};
#undef g_BattleSpecLightBlast2SpriteReq
// GLOBAL: FF7 0x008a2578
struct BattleFxSpriteRequest g_BattleSpecLightBlast2SpriteReq[2] = {{0, 0, -128, -128, -128, 0x2c, 0, 0}, {0, 0, 64, 64, 64, 0x2c, 0, 0}};
// clang-format on
#pragma intrinsic(memset)
#pragma intrinsic(memcpy)


// clang-format off
// GLOBAL: FF7 0x00b64f58
short g_BattleSpecLightBlast2SavedActorAlpha[10] = {0};
// GLOBAL: FF7 0x00b64f6c
int g_BattleSpecLightBlast2PacketCursor = 0;
// GLOBAL: FF7 0x00b64f70
struct GfxTableCursor *g_BattleSpecLightBlast2LoadedResTable = 0;
// GLOBAL: FF7 0x00b64f74
short g_BattleSpecLightBlast2FlashIntensity = 0;
// GLOBAL: FF7 0x00b64f78
short g_BattleSpecLightBlast2FlashStep = 0;
unsigned char g_bssPad_00b64f7c[4] = {0};
// clang-format on

/* Loads the SPECIAL/HIKARI2 sprite resources once the battle scene exists. */
// FUNCTION: FF7 0x00534710
void __cdecl BattleFx_SpecialLoadResLightBlast2(void)
{
    GfxModelTableBank *curOwnerCtx;
    GfxResourceRequest reqBuffer;
    int kind;
    char pathText[200];
    GfxResourceRequest *pRequest;
    int resourceDefsWord;

    if (g_pBattle3dScene != 0 && g_BattleSpecLightBlast2LoadedResTable == 0) {
        kind = 2;
        resourceDefsWord = (int)&g_BattleSpecLightBlast2ResourceDefs;
        curOwnerCtx = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRequest = &reqBuffer;
        Gfx_InitResourceRequest(pRequest);
        pRequest->groupTag = 0;
        // STRING: FF7 0x008a25c8
        pRequest->resourcePath = (char *)BattleFx_BuildResourcePath("SPECIAL/HIKARI2", pathText);
        Gfx_LoadModelTableIfAbsent(kind, resourceDefsWord, pRequest, &g_BattleSpecLightBlast2LoadedResTable, curOwnerCtx);
    }
}

/* Entry point of the Hikari2 effect: resets the white flash, then begins the effect. */
// FUNCTION: FF7 0x005347a0
void __cdecl BattleFx_SpecialEnterLightBlast2(int targetMask, int actorIdx)
{
    g_BattleSpecLightBlast2FlashIntensity = 0;
    g_BattleSpecLightBlast2FlashStep = 0;
    BattleFx_SpecialBeginLightBlast2(targetMask, actorIdx);
}

/* Loads the resources, queues a per-target spawn for every target and plays the cast sound. */
// FUNCTION: FF7 0x005347c7
void __cdecl BattleFx_SpecialBeginLightBlast2(int targetMask, int actorIdx)
{
    BattleFx_SpecialLoadResLightBlast2();
    Battle_CreateBitPatternCallbackTask(targetMask, actorIdx, 4, BattleFx_SpecialSpawnLightBlast2PerTarget);
    Battle_PlaySfxPannedToActor(0x6B, (short)actorIdx, 0x40);
}

/* Starts the Hikari2 timeline task at the target actor's bone position and records the actor in it. */
// FUNCTION: FF7 0x005347f9
void __cdecl BattleFx_SpecialSpawnLightBlast2PerTarget(int actor, int actorIndex)
{
    int p; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxLightBlast3Slot *taskPtr;

    taskPtr = (BattleFxLightBlast3Slot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialLightBlast2TimelineTask);
    Battle_GetBoneWorldPos(actorIndex, g_BattleActorRecords[actorIndex].bone34, &taskPtr->posX);
    taskPtr->boneIndex = actor;
}

/* Drives the Hikari2 timeline: the light sprite, the white flash, the actors' saved draw flags and the hit sparks. */
// FUNCTION: FF7 0x0053484d
void __cdecl BattleFx_SpecialLightBlast2TimelineTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[60];
        int depthOut;
        struct { unsigned char pad_to_4[0x4]; SVECTOR offsetX; };
        struct { unsigned char pad_to_C[0xC]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_2C[0x2C]; BattleFxStareDownParticle *particle; };
        struct { unsigned char pad_to_34[0x34]; short actorIndex; };
        struct { unsigned char pad_to_38[0x38]; BattleFxLightBlast3Slot *effectSlot; };
    } frame;
#pragma pack(pop)

    frame.effectSlot = (BattleFxLightBlast3Slot *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.effectSlot->stage == 0) {
        frame.particle = (BattleFxStareDownParticle *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialLightBlast2AnimatedSpriteEntry0Task);
        frame.particle->angleX = 0;
        frame.particle->angleX = 0;
        frame.particle->angle = ArcTangentFixed(frame.effectSlot->targetX - frame.effectSlot->posX, frame.effectSlot->targetZ - frame.effectSlot->posZ);
        frame.offsetX.vy = 0;
        frame.offsetX.vx = frame.offsetX.vy;
        frame.offsetX.vz = 100;
        RotMatrix((SVECTOR *)&frame.particle->angleX, &frame.rotMatrix);
        frame.rotMatrix.t[2] = 0;
        frame.rotMatrix.t[1] = frame.rotMatrix.t[2];
        frame.rotMatrix.t[0] = frame.rotMatrix.t[1];
        SetRotMatrix(&frame.rotMatrix);
        SetTransMatrix(&frame.rotMatrix);
        RotTransPointToInt16(&frame.offsetX, (short *)&frame.offsetX, &frame.depthOut);
        frame.particle->posX = frame.effectSlot->posX + frame.offsetX.vx;
        frame.particle->posY = frame.effectSlot->posY + frame.offsetX.vy;
        frame.particle->posZ = frame.effectSlot->posZ + frame.offsetX.vz;
    }
    if (frame.effectSlot->stage == 2) {
        g_BattleFxTimelineState.channel2 = 0;
        g_BattleFxTimelineState.channel1 = g_BattleFxTimelineState.channel2;
        g_BattleFxTimelineState.red = g_BattleFxTimelineState.channel1;
        g_BattleFxTimelineState.startValue = 0x1000;
        for (frame.actorIndex = 0; frame.actorIndex < 10; frame.actorIndex++) {
            g_BattleSpecLightBlast2SavedActorAlpha[frame.actorIndex] = (short)*(unsigned char *)&g_BattleActorRecords[frame.actorIndex].drawFlags;
            g_BattleActorRecords[frame.actorIndex].drawFlags |= 2;
        }
    }
    if (frame.effectSlot->stage == 4) {
        g_BattleFxTimelineState.startValue = 0;
        BattleFx_SpecialLightBlast2BeginWhiteFlashFadeIn(15);
        for (frame.actorIndex = 0; frame.actorIndex < 10; frame.actorIndex++) {
            g_BattleActorRecords[frame.actorIndex].drawFlags = (char)g_BattleSpecLightBlast2SavedActorAlpha[frame.actorIndex];
        }
    }
    if (frame.effectSlot->stage == 20) {
        BattleFx_SpecialLightBlast2BeginWhiteFlashFadeOut(15);
    }
    if (frame.effectSlot->stage == 30) {
        Battle_PostActorHitOrHurtMessage(frame.effectSlot->boneIndex);
        Battle_PlaySfxPannedToActor(0x95, (short)frame.effectSlot->boneIndex, 0x40);
    }
    if (frame.effectSlot->stage > 30 && frame.effectSlot->stage < 35) {
        for (frame.actorIndex = 0; frame.actorIndex < 2; frame.actorIndex++) {
            frame.particle = (BattleFxStareDownParticle *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialLightBlast2AnimatedSpriteEntry1Task);
            Battle_GetBoneWorldPos(frame.effectSlot->boneIndex, g_BattleActorRecords[frame.effectSlot->boneIndex].targetBone, &frame.particle->posX);
            frame.particle->posX += (short)(rand() % 1000 - 500);
            frame.particle->posY += (short)(rand() % 1000 - 500);
            frame.particle->posZ += (short)(rand() % 1000 - 500);
        }
    }
    (frame.effectSlot->stage)++;
    if (frame.effectSlot->stage == 40) {
        frame.effectSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/* Starts the white flash rising from 0 by 255 / duration per frame. */
// FUNCTION: FF7 0x00534be1
void __cdecl BattleFx_SpecialLightBlast2BeginWhiteFlashFadeIn(short duration)
{
    g_BattleSpecLightBlast2FlashIntensity = 0;
    g_BattleSpecLightBlast2FlashStep = 0xFF / duration;
    Battle_AllocateResourceSlot(BattleFx_SpecialLightBlast2WhiteFlashTask);
    g_BattleFxTimelineState.channel2 = 0;
    g_BattleFxTimelineState.channel1 = g_BattleFxTimelineState.channel2;
    g_BattleFxTimelineState.red = g_BattleFxTimelineState.channel1;
    Battle_StartFixedPointRamp(0x1000, 15);
}

/* Draws the white flash at the current intensity and steps it; frees the task when it returns to 0. */
// FUNCTION: FF7 0x00534c3a
void __cdecl BattleFx_SpecialLightBlast2WhiteFlashTask(void)
{
    BattleTaskSlot *task;

    task = BATTLE_CURRENT_TASK_SLOT;
    g_BattleOverlayDrawListHead = BattleFx_FlashScreenWhite(0, 0, *(char *)&g_BattleSpecLightBlast2FlashIntensity);
    if (g_BattleIsPaused != 0) {
        return;
    }
    g_BattleSpecLightBlast2FlashIntensity += g_BattleSpecLightBlast2FlashStep;
    if (g_BattleSpecLightBlast2FlashIntensity == 0) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
}

/* Starts the white flash falling from 255 to 0 over duration frames. */
// FUNCTION: FF7 0x00534ca0
void __cdecl BattleFx_SpecialLightBlast2BeginWhiteFlashFadeOut(short duration)
{
    g_BattleSpecLightBlast2FlashIntensity = 0xFF;
    g_BattleSpecLightBlast2FlashStep = -(255 / duration);
    Battle_StartFixedPointRamp(0, 15);
}

/* Draws sprite entry 0 (the light) at the task position and frees the task after its last frame. */
// FUNCTION: FF7 0x00534cce
void __cdecl BattleFx_SpecialLightBlast2AnimatedSpriteEntry0Task(void)
{
    BattleTaskSlot *task;

    task = BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecLightBlast2SpriteReq[0].spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(0, g_BattleSpecLightBlast2LoadedResTable);
    g_BattleSpecLightBlast2SpriteReq[0].drawObj = Gfx_ModelTableGetSpriteObject(0, g_BattleSpecLightBlast2LoadedResTable);
    BattleFx_InstallBillboardMatrix((short *)task->data, 0x1800 /* 1.5 */, 0);
    g_BattleSpecLightBlast2SpriteReq[0].frame = task->frameCounter;
    g_BattleSpecLightBlast2PacketCursor = BattleFx_SubmitSpriteDraw(&g_BattleSpecLightBlast2SpriteReq[0], g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecLightBlast2PacketCursor);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (g_BattleSpecLightBlast2SpriteReq[0].spriteGroup != 0) {
        (task->frameCounter)++;
        if (task->frameCounter >= g_BattleSpecLightBlast2SpriteReq[0].spriteGroup->header->frameCount) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/* Draws sprite entry 1 (a hit spark) at the task position and frees the task after its last frame. */
// FUNCTION: FF7 0x00534da1
void __cdecl BattleFx_SpecialLightBlast2AnimatedSpriteEntry1Task(void)
{
    BattleTaskSlot *task;

    task = BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecLightBlast2SpriteReq[1].spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(1, g_BattleSpecLightBlast2LoadedResTable);
    g_BattleSpecLightBlast2SpriteReq[1].drawObj = Gfx_ModelTableGetSpriteObject(1, g_BattleSpecLightBlast2LoadedResTable);
    BattleFx_InstallBillboardMatrix((short *)task->data, 0x1800 /* 1.5 */, 0);
    g_BattleSpecLightBlast2SpriteReq[1].frame = task->frameCounter;
    g_BattleSpecLightBlast2PacketCursor = BattleFx_SubmitSpriteDraw(&g_BattleSpecLightBlast2SpriteReq[1], g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecLightBlast2PacketCursor);
    if (g_BattleIsPaused == 0 && g_BattleSpecLightBlast2SpriteReq[1].spriteGroup != 0) {
        (task->frameCounter)++;
        if (task->frameCounter >= g_BattleSpecLightBlast2SpriteReq[1].spriteGroup->header->frameCount) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}
} /* extern "C" */
