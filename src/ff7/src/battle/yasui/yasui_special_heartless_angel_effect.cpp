// src/ff7/src/battle/yasui/yasui_special_heartless_angel_effect.cpp
/* Enemy Special Attack [145]: Heartless Angel (orig. SPECIAL/MAGEND) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle effect for the MagEnd special effect: emitters at each target's bone drop
//   falling sprite particles with randomized velocity until the stop flag ends the effect.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/types/battle.h"
#include "ff7/types/battle_boss.h"
#include "ff7/battle_tasks.h"
#include "ff7/camera_script.h"
#include "lib/types/graphics.h"
#include <string.h>
#include <stdlib.h>
#include "lib/codegen.h"
#include "ff7/api/battle.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
extern "C" {

// clang-format off
// GLOBAL: FF7 0x007e31a8
int g_HeartlessAngel_ResourceHeader = 0x0019AA39;
char *g_YasuiSpecialHeartlessAngelEffectResourceNames[1] = {
    // STRING: FF7 0x007e31d8
    "CHI_1.TIM"
};
unsigned int g_YasuiSpecialHeartlessAngelEffectAssetPrefix[4] = {
    // STRING: FF7 0x007e31e4
    (unsigned int)"CHI_1.SPT",
    0x00000000,
    0x00000000,
    0x00000000
};
// GLOBAL: FF7 0x007e31c0
struct HeartlessAngel_SpriteDrawReq g_HeartlessAngel_SpriteDrawReq = {0, 0, {0x80, 0x80, 0x80, 0x2e, 0, 0, 0, 0}, 0};
unsigned char g_rawData_007e31d4[4] = {0x64, 0x00, 0x00, 0x00};
// clang-format on

#pragma intrinsic(memcpy)
#pragma intrinsic(memset)
#pragma intrinsic(abs)


// clang-format off
// GLOBAL: FF7 0x00a38a88
int g_HeartlessAngel_SpriteDrawHandle = 0;
// GLOBAL: FF7 0x00a38a8c
short g_HeartlessAngel_StopFlag = 0;
// GLOBAL: FF7 0x00a38a90
struct GfxTableCursor *g_HeartlessAngel_LoadedResTable = 0;
// clang-format on

/*
 * Initializes and requests loading the SPECIAL/MAGEND battle effect resource if not already loaded.
 */
// FUNCTION: FF7 0x00457040
void __cdecl BattleFx_SpecialLoadResHeartlessAngel(void)
{
    int resourceId;
    char pathBuf[200];
    GfxResourceRequest requestBuf;
    GfxModelTableBank *sceneCtx;
    GfxResourceRequest *requestPtr;
    int header;

    if (g_pBattle3dScene != 0 && g_HeartlessAngel_LoadedResTable == 0) {
        resourceId = 1;
        header = (int)&g_HeartlessAngel_ResourceHeader;
        sceneCtx = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        requestPtr = &requestBuf;
        Gfx_InitResourceRequest(requestPtr);
        requestPtr->groupTag = 0;
        // STRING: FF7 0x007e31f0
        requestPtr->resourcePath = BattleFx_BuildResourcePath("SPECIAL/MAGEND", pathBuf);
        Gfx_LoadModelTableIfAbsent(resourceId, header, requestPtr, &g_HeartlessAngel_LoadedResTable, sceneCtx);
    }
}

/*
 * Resets the MagEnd stop flag and begins the MagEnd effect for the specified target.
 */
// FUNCTION: FF7 0x004570d0
void __cdecl BattleFx_SpecialEnterHeartlessAngel(int targetMask, int actorIdx)
{
    g_HeartlessAngel_StopFlag = 0;
    BattleFx_SpecialBeginHeartlessAngel(targetMask, actorIdx);
}

/*
 * Loads MagEnd resources and dispatches per-target emitter tasks via bit pattern callback.
 */
// FUNCTION: FF7 0x004570ee
void __cdecl BattleFx_SpecialBeginHeartlessAngel(int targetMask, int actorIdx)
{
    BattleFx_SpecialLoadResHeartlessAngel();
    Battle_CreateBitPatternCallbackTask(targetMask, actorIdx, 4, (void *)BattleFx_SpecialSpawnHeartlessAngelPerTarget);
}

/*
 * Spawns an emitter task slot for the given target actor and pattern.
 */
// FUNCTION: FF7 0x0045710f
void __cdecl BattleFx_SpecialSpawnHeartlessAngelPerTarget(int actor, int actorIdx)
{
    YasuiHeartlessAngelEmitterState *taskSlot;

    taskSlot = (YasuiHeartlessAngelEmitterState *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialHeartlessAngelEmitterTask);
    taskSlot->target = actor;
    taskSlot->actorIndex = actorIdx;
}

/*
 * Emitter task that spawns falling sprite particles at target bone with randomized velocity.
 */
// FUNCTION: FF7 0x00457148
void __cdecl BattleFx_SpecialHeartlessAngelEmitterTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[56];
        SVECTOR rotX;
        struct { unsigned char pad_to_8[0x8]; int projectedFlag; };
        struct { unsigned char pad_to_10[0x10]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_30[0x30]; BattleFxHeartlessAngelEmitterPoint *particle; };
        struct { unsigned char pad_to_34[0x34]; YasuiHeartlessAngelEmitterState *emitter; };
    } frame;
#pragma pack(pop)

    frame.emitter = (YasuiHeartlessAngelEmitterState *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.emitter->frameCounter < 0x38) {
        frame.particle = (BattleFxHeartlessAngelEmitterPoint *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialHeartlessAngelFallingSpriteParticleTask);
        Battle_GetBoneWorldPos(frame.emitter->actorIndex, g_BattleActorRecords[frame.emitter->actorIndex].targetBone, &frame.particle->x);
        frame.particle->x += (short)(rand() % 0xC8 - 0x64);
        frame.particle->y += (short)(rand() % 0xC8 - 0x64);
        frame.particle->z += (short)(rand() % 0xC8 - 0x64);
        frame.rotX.vx = rand() % 0x1F4 + 0x15E;
        frame.rotX.vy = rand() % 0x1F4 - 0xFA;
        frame.rotX.vz = 0;
        RotMatrix(&frame.rotX, &frame.rotMatrix);
        frame.rotMatrix.t[2] = 0;
        frame.rotMatrix.t[1] = frame.rotMatrix.t[2];
        frame.rotMatrix.t[0] = frame.rotMatrix.t[1];
        SetRotMatrix(&frame.rotMatrix);
        SetTransMatrix(&frame.rotMatrix);
        RotTransPointToInt16(&g_HeartlessAngel_SpriteDrawReq.emitterScreenPoint, frame.particle->projected, &frame.projectedFlag);
    }
    (frame.emitter->frameCounter)++;
    if (frame.emitter->frameCounter == 0x38) {
        Battle_PostActorHitOrHurtMessage(frame.emitter->target);
        g_HeartlessAngel_StopFlag = 1;
        frame.emitter->endMarker = 0xFFFF;
    }
}

/*
 * Per-frame falling sprite particle task updating velocity and submitting billboard sprite draw.
 */
// FUNCTION: FF7 0x004572f6
void __cdecl BattleFx_SpecialHeartlessAngelFallingSpriteParticleTask(void)
{
    BattleFxHeartlessAngelFallingSprite *taskSlot;

    taskSlot = (BattleFxHeartlessAngelFallingSprite *)BATTLE_CURRENT_TASK_SLOT;
    (g_HeartlessAngel_SpriteDrawReq.spriteField) = Gfx_ModelTableGetLoadedRecord(0, g_HeartlessAngel_LoadedResTable);
    g_HeartlessAngel_SpriteDrawReq.fallingSpriteResource = Gfx_ModelTableGetSpriteObject(0, g_HeartlessAngel_LoadedResTable);
    BattleFx_InstallBillboardMatrix(&taskSlot->posA, 0x200, 0);
    g_HeartlessAngel_SpriteDrawHandle = BattleFx_SubmitSpriteDraw(&g_HeartlessAngel_SpriteDrawReq, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_HeartlessAngel_SpriteDrawHandle);
    if (g_BattleIsPaused != 0) {
        return;
    }
    taskSlot->fallAccel += 3;
    taskSlot->posA += taskSlot->velocityA;
    taskSlot->fallSpeed += taskSlot->fallAccel;
    taskSlot->posB += taskSlot->velocityB;
    if (taskSlot->fallSpeed > 0 || g_HeartlessAngel_StopFlag == 1) {
        taskSlot->ownerSlot = BATTLE_TASK_FREE;
    }
    (taskSlot->frameIndex)++;
}
} /* extern "C" */
