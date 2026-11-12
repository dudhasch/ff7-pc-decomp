// src/ff7/src/battle/battle_fx_special_light_blast3_effect.cpp
/* Enemy Special Attack [99]: Light Blast 3 (orig. SPECIAL/HIKARI3, 光3) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle special effect 'Hikari3' (light): a white screen flash that fades in and out, a
//   per-target timeline, and animated sprite entry tasks.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/api/battle.h"
#include "ff7/battle_tasks.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_3d.h"
#include "ff7/camera_script.h"
#include "lib/types/graphics.h"
#include <string.h>
#include <stdlib.h>
#include "lib/codegen.h"
extern "C" {

// clang-format off
// GLOBAL: FF7 0x008a24c8
int g_BattleSpecLightBlast3ResourceDefs = 0x00192A39;
char *g_BattleFxSpecialLightBlast3EffectResourceNames[1] = {
    // STRING: FF7 0x008a2510
    "HIKARI1.TIM"
};
unsigned int g_BattleFxSpecialLightBlast3EffectResourceNames2[8] = {
    // STRING: FF7 0x008a251c
    (unsigned int)"HIKARI1.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x008a2528
    (unsigned int)"HIT1.TIM",
    // STRING: FF7 0x008a2534
    (unsigned int)"HIT1.SPT",
    0x00000000,
    0x00000000
};
#undef g_BattleSpecLightBlast3SpriteReq
// GLOBAL: FF7 0x008a24f0
struct BattleFxSpriteRequest g_BattleSpecLightBlast3SpriteReq[2] = {{0, 0, -128, -128, -128, 0x2c, 0, 0}, {0, 0, 64, 64, 64, 0x2c, 0, 0}};
// clang-format on
#pragma intrinsic(memset)
#pragma intrinsic(memcpy)


// clang-format off
// GLOBAL: FF7 0x00b64f30
short g_BattleSpecLightBlast3SavedActorAlpha[10] = {0};
// GLOBAL: FF7 0x00b64f44
int g_BattleSpecLightBlast3PacketCursor = 0;
// GLOBAL: FF7 0x00b64f48
struct GfxTableCursor *g_BattleSpecLightBlast3LoadedResTable = 0;
// GLOBAL: FF7 0x00b64f4c
short g_BattleSpecLightBlast3FlashIntensity = 0;
// GLOBAL: FF7 0x00b64f50
short g_BattleSpecLightBlast3FlashStep = 0;
unsigned char g_bssPad_00b64f54[4] = {0};
// clang-format on

/*
 * When the engine context exists and g_BattleSpecLightBlast3LoadedResTable is empty, builds SPECIAL/HIKARI3 with a default-
 * duration record and creates resource type 2 using g_BattleSpecLightBlast3ResourceDefs.
 */
// FUNCTION: FF7 0x00533fe0
void __cdecl BattleFx_SpecialLoadResLightBlast3(void)
{
    GfxModelTableBank *curOwnerCtx;
    GfxResourceRequest reqBuffer;
    int kind;
    char pathText[200];
    GfxResourceRequest *pRequest;
    int resourceDefsWord;

    if (g_pBattle3dScene != 0 && g_BattleSpecLightBlast3LoadedResTable == 0) {
        kind = 2;
        resourceDefsWord = (int)&g_BattleSpecLightBlast3ResourceDefs;
        curOwnerCtx = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRequest = &reqBuffer;
        Gfx_InitResourceRequest(pRequest);
        pRequest->groupTag = 0;
        // STRING: FF7 0x008a2540
        pRequest->resourcePath = BattleFx_BuildResourcePath("SPECIAL/HIKARI3", pathText);
        Gfx_LoadModelTableIfAbsent(kind, resourceDefsWord, pRequest, &g_BattleSpecLightBlast3LoadedResTable, curOwnerCtx);
    }
}

/* Clears the white flash state, then begins the effect. */
// FUNCTION: FF7 0x00534070
void __cdecl BattleFx_SpecialEnterLightBlast3(int targetMask, int actorIdx)
{
    g_BattleSpecLightBlast3FlashIntensity = 0;
    g_BattleSpecLightBlast3FlashStep = 0;
    BattleFx_SpecialBeginLightBlast3(targetMask, actorIdx);
}

/*
 * Calls BattleFx_SpecialLoadResLightBlast3, then creates a task for BattleFx_SpecialSpawnLightBlast3PerTarget with both arguments and mode 4, then
 * dispatches event (0x6B, actorIndex, 0x40).
 */
// FUNCTION: FF7 0x00534097
void __cdecl BattleFx_SpecialBeginLightBlast3(int targetMask, int actorIdx)
{
    BattleFx_SpecialLoadResLightBlast3();
    Battle_CreateBitPatternCallbackTask(targetMask, actorIdx, 4, BattleFx_SpecialSpawnLightBlast3PerTarget);
    Battle_PlaySfxPannedToActor(0x6B, (short)actorIdx, 0x40);
}

/* Starts a timeline task for one target: it begins at the actor's bone34 world position and remembers the actor. */
// FUNCTION: FF7 0x005340c9
void __cdecl BattleFx_SpecialSpawnLightBlast3PerTarget(int actor, int actorIndex)
{
    int p; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxLightBlast3Slot *taskPtr;

    taskPtr = (BattleFxLightBlast3Slot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialLightBlast3TimelineTask);
    Battle_GetBoneWorldPos(actorIndex, g_BattleActorRecords[actorIndex].bone34, &taskPtr->posX);
    taskPtr->boneIndex = actor;
}

/* Timeline of the Hikari 3 effect, one stage per frame: the light particle, the actors' fade, the white flash and the sparkle sprites. */
// FUNCTION: FF7 0x0053411d
void __cdecl BattleFx_SpecialLightBlast3TimelineTask(void)
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
        frame.particle = (BattleFxStareDownParticle *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialLightBlast3AnimatedSpriteEntry0Task);
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
            g_BattleSpecLightBlast3SavedActorAlpha[frame.actorIndex] = (short)*(unsigned char *)&g_BattleActorRecords[frame.actorIndex].drawFlags;
            g_BattleActorRecords[frame.actorIndex].drawFlags |= 2;
        }
    }
    if (frame.effectSlot->stage == 4) {
        g_BattleFxTimelineState.startValue = 0;
        BattleFx_SpecialLightBlast3BeginWhiteFlashFadeIn(15);
        for (frame.actorIndex = 0; frame.actorIndex < 10; frame.actorIndex++) {
            g_BattleActorRecords[frame.actorIndex].drawFlags = *(char *)&g_BattleSpecLightBlast3SavedActorAlpha[frame.actorIndex];
        }
    }
    if (frame.effectSlot->stage == 20) {
        BattleFx_SpecialLightBlast3BeginWhiteFlashFadeOut(15);
    }
    if (frame.effectSlot->stage == 30) {
        Battle_PostActorHitOrHurtMessage(frame.effectSlot->boneIndex);
        Battle_PlaySfxPannedToActor(0x95, (short)frame.effectSlot->boneIndex, 0x40);
    }
    if (frame.effectSlot->stage > 30 && frame.effectSlot->stage < 35) {
        for (frame.actorIndex = 0; frame.actorIndex < 2; frame.actorIndex++) {
            frame.particle = (BattleFxStareDownParticle *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialLightBlast3AnimatedSpriteEntry1Task);
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

/*
 * Clears g_BattleSpecLightBlast3FlashIntensity, stores 0xff/duration in g_BattleSpecLightBlast3FlashStep, then allocates a slot for BattleFx_SpecialLightBlast3WhiteFlashTask.
 */
// FUNCTION: FF7 0x005344b1
void __cdecl BattleFx_SpecialLightBlast3BeginWhiteFlashFadeIn(short duration)
{
    g_BattleSpecLightBlast3FlashIntensity = 0;
    g_BattleSpecLightBlast3FlashStep = 0xFF / duration;
    Battle_AllocateResourceSlot(BattleFx_SpecialLightBlast3WhiteFlashTask);
}

/*
 * Calls BattleFx_FlashScreenWhite using the low signed byte of g_BattleSpecLightBlast3FlashIntensity before checking pause;
 * when unpaused adds g_BattleSpecLightBlast3FlashStep to that value and retires the current slot if the updated value
 * equals zero.
 */
// FUNCTION: FF7 0x005344de
void __cdecl BattleFx_SpecialLightBlast3WhiteFlashTask(void)
{
    BattleTaskSlot *task;

    task = BATTLE_CURRENT_TASK_SLOT;
    g_BattleOverlayDrawListHead = BattleFx_FlashScreenWhite(*(char *)&g_BattleSpecLightBlast3FlashIntensity, 0, 0);
    if (g_BattleIsPaused != 0) {
        return;
    }
    g_BattleSpecLightBlast3FlashIntensity += g_BattleSpecLightBlast3FlashStep;
    if (g_BattleSpecLightBlast3FlashIntensity == 0) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Sets g_BattleSpecLightBlast3FlashIntensity to 255 and g_BattleSpecLightBlast3FlashStep to the negation of integer division 255 by the short
 * duration; no zero-divisor guard or guarantee of reaching zero appears.
 */
// FUNCTION: FF7 0x00534544
void __cdecl BattleFx_SpecialLightBlast3BeginWhiteFlashFadeOut(short duration)
{
    g_BattleSpecLightBlast3FlashIntensity = 0xFF;
    g_BattleSpecLightBlast3FlashStep = -(255 / duration);
}

/*
 * Draws the first sprite group of g_BattleSpecLightBlast3LoadedResTable as a billboard at the task's position,
 * then advances its frame and frees the task once the frame reaches the group's frame count.
 */
// FUNCTION: FF7 0x00534566
void __cdecl BattleFx_SpecialLightBlast3AnimatedSpriteEntry0Task(void)
{
    BattleTaskSlot *task;

    task = BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecLightBlast3SpriteReq[0].spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(0, g_BattleSpecLightBlast3LoadedResTable);
    g_BattleSpecLightBlast3SpriteReq[0].drawObj = Gfx_ModelTableGetSpriteObject(0, g_BattleSpecLightBlast3LoadedResTable);
    BattleFx_InstallBillboardMatrix((short *)task->data, 0x1800 /* 1.5 */, 0);
    g_BattleSpecLightBlast3SpriteReq[0].frame = task->frameCounter;
    g_BattleSpecLightBlast3PacketCursor = BattleFx_SubmitSpriteDraw(g_BattleSpecLightBlast3SpriteReq, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecLightBlast3PacketCursor);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (g_BattleSpecLightBlast3SpriteReq[0].spriteGroup != 0) {
        (task->frameCounter)++;
        if (task->frameCounter >= g_BattleSpecLightBlast3SpriteReq[0].spriteGroup->header->frameCount) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Draws the second sprite group of g_BattleSpecLightBlast3LoadedResTable as a billboard at the task's position,
 * then advances its frame and frees the task once the frame reaches the group's frame count.
 */
// FUNCTION: FF7 0x00534639
void __cdecl BattleFx_SpecialLightBlast3AnimatedSpriteEntry1Task(void)
{
    BattleTaskSlot *task;

    task = BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecLightBlast3SpriteReq[1].spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(1, g_BattleSpecLightBlast3LoadedResTable);
    g_BattleSpecLightBlast3SpriteReq[1].drawObj = Gfx_ModelTableGetSpriteObject(1, g_BattleSpecLightBlast3LoadedResTable);
    BattleFx_InstallBillboardMatrix((short *)task->data, 0x1800 /* 1.5 */, 0);
    g_BattleSpecLightBlast3SpriteReq[1].frame = task->frameCounter;
    g_BattleSpecLightBlast3PacketCursor = BattleFx_SubmitSpriteDraw(&g_BattleSpecLightBlast3SpriteReq[1], g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecLightBlast3PacketCursor);
    if (g_BattleIsPaused == 0 && g_BattleSpecLightBlast3SpriteReq[1].spriteGroup != 0) {
        (task->frameCounter)++;
        if (task->frameCounter >= g_BattleSpecLightBlast3SpriteReq[1].spriteGroup->header->frameCount) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}
} /* extern "C" */
