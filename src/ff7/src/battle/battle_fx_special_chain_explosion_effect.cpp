// src/ff7/src/battle/battle_fx_special_chain_explosion_effect.cpp
/* Enemy Special Attack [121]: Chain Explosion (orig. special/renbaku, 連爆) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle special effect 'Renbaku': a setup task and timeline with converging sprites.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/enums/sound.h"
#include "ff7/api/battle.h"
#include "ff7/battle_tasks.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_special.h"
#include "ff7/camera_script.h"
#include "lib/types/graphics.h"
#include "ff7/types/battle_3d.h"
#include <string.h>
#include <stdlib.h>
#include "lib/codegen.h"
extern "C" {

// clang-format off
// GLOBAL: FF7 0x008a0f70
int g_BattleSpecChainExplosionResourceDefs = 0x00192A39;
char *g_BattleFxSpecialChainExplosionEffectResourceNames[1] = {
    // STRING: FF7 0x008a0fb0
    "EXPLO1.TIM"
};
typedef struct {
    char *filename;
    unsigned int descriptor;
} BattleSpecialChainExplosionResourceTailEntry;
BattleSpecialChainExplosionResourceTailEntry g_BattleFxSpecialChainExplosionEffectResourceNames2[5] = {
    {
        // STRING: FF7 0x008a0fbc
        "EXPLO.SPT",
        0x00000000
    },
    {
        0,
        0x00013839
    },
    {
        // STRING: FF7 0x008a0fc8
        "EXPLO2.TIM",
        0x00000000
    },
    {
        0,
        0x00000000
    },
    {
        (char *)0xf0000000,
        0x00000000
    }
};
#undef g_BattleSpecChainExplosionSpriteReq
// GLOBAL: FF7 0x008a0fa0
struct BattleFxSpriteRequest g_BattleSpecChainExplosionSpriteReq = {0, 0, -128, -128, -128, 0x2e, 0, 0};
// clang-format on
#pragma intrinsic(memset)
#pragma intrinsic(memcpy)


// clang-format off
// GLOBAL: FF7 0x00b5eab8
SVECTOR g_BattleSpecChainExplosionTargetPos = {0};
// GLOBAL: FF7 0x00b5eac0
int g_BattleSpecChainExplosionExplosionHead = 0;
unsigned char g_bssPad_00b5eac4[4] = {0};
// GLOBAL: FF7 0x00b5eac8
struct BattleFxChainExplosionExplosionPoint g_BattleSpecChainExplosionExplosionCoords[311] = {0};
// GLOBAL: FF7 0x00b5f480
int g_BattleSpecChainExplosionTargetMask = 0;
unsigned char g_bssPad_00b5f484[4] = {0};
// GLOBAL: FF7 0x00b5f488
SVECTOR g_BattleSpecChainExplosionTargetOffset = {0};
// GLOBAL: FF7 0x00b5f490
int g_BattleSpecChainExplosionDeltaX = 0;
// GLOBAL: FF7 0x00b5f494
int g_BattleSpecChainExplosionDeltaY = 0;
// GLOBAL: FF7 0x00b5f498
int g_BattleSpecChainExplosionDeltaZ = 0;
unsigned char g_bssPad_00b5f49c[4] = {0};
// GLOBAL: FF7 0x00b5f4a0
int g_BattleSpecChainExplosionNormalX = 0;
// GLOBAL: FF7 0x00b5f4a4
int g_BattleSpecChainExplosionNormalY = 0;
// GLOBAL: FF7 0x00b5f4a8
int g_BattleSpecChainExplosionNormalZ = 0;
unsigned char g_bssPad_00b5f4ac[4] = {0};
// GLOBAL: FF7 0x00b5f4b0
int g_BattleSpecChainExplosionStepX = 0;
// GLOBAL: FF7 0x00b5f4b4
int g_BattleSpecChainExplosionStepY = 0;
// GLOBAL: FF7 0x00b5f4b8
int g_BattleSpecChainExplosionStepZ = 0;
unsigned char g_bssPad_00b5f4bc[4] = {0};
// GLOBAL: FF7 0x00b5f4c0
struct GfxTableCursor *g_BattleSpecChainExplosionLoadedResTable = 0;
unsigned char g_bssPad_00b5f4c4[4] = {0};
// clang-format on

/* Loads the special/renbaku resources once the battle scene exists. */
// FUNCTION: FF7 0x0052fe10
void __cdecl BattleFx_SpecialLoadResChainExplosion(void)
{
    GfxModelTableBank *curOwnerCtx;
    GfxResourceRequest currentRecord;
    int kind;
    char pathText[200];
    GfxResourceRequest *pRequest;
    int resourceDefsWord;

    if (g_pBattle3dScene != 0 && g_BattleSpecChainExplosionLoadedResTable == 0) {
        kind = 2;
        resourceDefsWord = (int)&g_BattleSpecChainExplosionResourceDefs;
        curOwnerCtx = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRequest = &currentRecord;
        Gfx_InitResourceRequest(pRequest);
        pRequest->groupTag = 0;
        // STRING: FF7 0x008a0fd4
        pRequest->resourcePath = BattleFx_BuildResourcePath("special/renbaku", pathText);
        Gfx_LoadModelTableIfAbsent(kind, resourceDefsWord, pRequest, &g_BattleSpecChainExplosionLoadedResTable, curOwnerCtx);
    }
}

/* Entry point of the Renbaku effect: loads the resources, then begins it. */
// FUNCTION: FF7 0x0052fea0
void __cdecl BattleFx_SpecialEnterChainExplosion(int targetMask, int actorIdx)
{
    BattleFx_SpecialLoadResChainExplosion();
    BattleFx_SpecialBeginChainExplosion(targetMask, actorIdx);
}

/* Allocates the draw list buffer and the setup task, and records the target mask. */
// FUNCTION: FF7 0x0052feba
void __cdecl BattleFx_SpecialBeginChainExplosion(int targetMask, int actorIdx)
{
    *(int *)&g_BattleFxDrawListBufferTaskSlotPtr = (int)BATTLE_NEW_TASK_SLOT(BattleFx_SelectDrawListBuffer);
    Battle_AllocateResourceSlot(BattleFx_SpecialChainExplosionSetupTask);
    g_BattleSpecChainExplosionTargetMask = targetMask;
    g_BattleSpecChainExplosionExplosionHead = 0;
}

/* Starts the timeline at the targets' centroid and works out the sweep direction, step and normal. */
// FUNCTION: FF7 0x0052fefc
void __cdecl BattleFx_SpecialChainExplosionSetupTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[76];
        struct { unsigned char pad_to_20[0x20]; BattleFxChainExplosionTimeline *timelineSlot; };
        struct { unsigned char pad_to_30[0x30]; int distance; };
        struct { unsigned char pad_to_34[0x34]; int offsetX; };
        struct { unsigned char pad_to_36[0x36]; short slotField06; };
        struct { unsigned char pad_to_38[0x38]; int offsetZ; };
        struct { unsigned char pad_to_48[0x48]; int stepSize; };
    } frame;
#pragma pack(pop)

    frame.timelineSlot = (BattleFxChainExplosionTimeline *)BATTLE_CURRENT_TASK_SLOT;
    frame.timelineSlot->ownerSlot = BATTLE_TASK_FREE;
    frame.timelineSlot = (BattleFxChainExplosionTimeline *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialChainExplosionTimelineTask);
    Battle_GetActorMaskCentroid(g_BattleSpecChainExplosionTargetMask, &frame.timelineSlot->pos.vx);
    frame.timelineSlot->angleStep = rand() % 0x3C + 0xC8;
    frame.timelineSlot->angle = frame.timelineSlot->angleStep;
    frame.timelineSlot->jitterBias = 0x360;
    frame.timelineSlot->jitterRange = frame.timelineSlot->jitterBias << 1;
    frame.timelineSlot->scale = 0x170A;
    frame.distance = 0xEA0;
    g_BattleSpecChainExplosionDeltaX = frame.timelineSlot->pos.vx - g_BattleCameraPosition.cameraPosX;
    g_BattleSpecChainExplosionDeltaY = 0;
    g_BattleSpecChainExplosionDeltaZ = frame.timelineSlot->pos.vz - g_BattleCameraPosZ;
    NormalizeVectorFixed(&g_BattleSpecChainExplosionDeltaX, &g_BattleSpecChainExplosionDeltaX);
    *(short *)&frame.offsetX = frame.timelineSlot->pos.vx - (frame.distance * g_BattleSpecChainExplosionDeltaX >> 0xC);
    frame.slotField06 = frame.timelineSlot->pos.vy;
    *(short *)&frame.offsetZ = frame.timelineSlot->pos.vz - (frame.distance * g_BattleSpecChainExplosionDeltaZ >> 0xC);
    frame.stepSize = (frame.distance << 1) / 0x1E;
    frame.timelineSlot->stepSize = *(short *)&frame.stepSize;
    g_BattleSpecChainExplosionNormalX = -g_BattleSpecChainExplosionDeltaZ;
    g_BattleSpecChainExplosionNormalY = 0;
    g_BattleSpecChainExplosionNormalZ = g_BattleSpecChainExplosionDeltaX;
    g_BattleSpecChainExplosionStepX = frame.stepSize * g_BattleSpecChainExplosionDeltaX >> 0xC;
    g_BattleSpecChainExplosionStepY = 0;
    g_BattleSpecChainExplosionStepZ = frame.stepSize * g_BattleSpecChainExplosionDeltaZ >> 0xC;
    *(int *)&g_BattleSpecChainExplosionTargetOffset.vx = frame.offsetX;
    *(int *)&g_BattleSpecChainExplosionTargetOffset.vz = frame.offsetZ;
    frame.timelineSlot->pos = g_BattleSpecChainExplosionTargetOffset;
    g_BattleSpecChainExplosionTargetPos = frame.timelineSlot->pos;
}

/* Moves the sweep along its line, scatters explosion points around it and hits the targets at the end. */
// FUNCTION: FF7 0x005300d1
void __cdecl BattleFx_SpecialChainExplosionTimelineTask(void)
{
    short level;
    int i7;
    BattleFxChainExplosionTimeline *taskPtr;
    struct BattleTaskSlot *convergePtr;
    int t; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int u; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int v; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int w; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int x; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int y; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int z; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int q; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int a; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int scaleRand;
    int head;
    int e; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */

    if (g_BattleIsPaused != 0) {
        return;
    }
    taskPtr = (BattleFxChainExplosionTimeline *)BATTLE_CURRENT_TASK_SLOT;
    if (taskPtr->phase > 1 && taskPtr->phase < 0x23) {
        head = g_BattleSpecChainExplosionExplosionHead;
        convergePtr = BATTLE_NEW_TASK_SLOT(BattleFx_SpecialChainExplosionConvergingSpritesTask);
        ((BattleFxBerserkFadeSprite *)convergePtr)->ringIndex = head;
        for (i7 = 0; i7 < 4; i7++) {
            scaleRand = rand() % 0x1000 + 0x800;
            g_BattleSpecChainExplosionExplosionCoords[head].posX = taskPtr->pos.vx + rand() % taskPtr->jitterRange - taskPtr->jitterBias;
            g_BattleSpecChainExplosionExplosionCoords[head].posY = taskPtr->pos.vy + rand() % taskPtr->jitterRange - taskPtr->jitterBias;
            g_BattleSpecChainExplosionExplosionCoords[head].posZ = taskPtr->pos.vz + rand() % taskPtr->jitterRange - taskPtr->jitterBias;
            g_BattleSpecChainExplosionExplosionCoords[head].scale = taskPtr->scale * scaleRand >> 0xC;
            head++;
            if (head >= 0x137) {
                head = 0;
            }
        }
        g_BattleSpecChainExplosionExplosionHead = head;
        if ((taskPtr->phase & 3) == 0) {
            Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputeScreenPanFromPosition(&taskPtr->pos), 0x7C);
        }
    }
    if (taskPtr->phase == 0x25) {
        for (i7 = 0; i7 < 0xA; i7++) {
            if ((g_BattleSpecChainExplosionTargetMask & 1 << i7) != 0) {
                Battle_PostActorHitOrHurtMessage(i7);
            }
        }
    }
    if (taskPtr->phase >= 0x2B) {
        level = 0;
        taskPtr->ownerSlot = BATTLE_TASK_FREE;
    }
    g_BattleSpecChainExplosionTargetOffset.vx += (short)g_BattleSpecChainExplosionStepX;
    g_BattleSpecChainExplosionTargetOffset.vy = 0;
    g_BattleSpecChainExplosionTargetOffset.vz += (short)g_BattleSpecChainExplosionStepZ;
    i7 = SinFixed(taskPtr->angle);
    taskPtr->angle += taskPtr->angleStep;
    head = taskPtr->stepSize << 3;
    scaleRand = head * i7 >> 0xC;
    taskPtr->pos.vx = (scaleRand * g_BattleSpecChainExplosionNormalX >> 0xC) + g_BattleSpecChainExplosionTargetOffset.vx;
    taskPtr->pos.vz = (scaleRand * g_BattleSpecChainExplosionNormalZ >> 0xC) + g_BattleSpecChainExplosionTargetOffset.vz;
    g_BattleSpecChainExplosionTargetPos = taskPtr->pos;
    (taskPtr->phase)++;
}

/* Pulls four explosion points toward the sweep position and draws them as sprites. */
// FUNCTION: FF7 0x005303a7
void __cdecl BattleFx_SpecialChainExplosionConvergingSpritesTask(void)
{
    int idx;
    int q;
    int r;
    VECTOR normal;
    BattleFxBerserkFadeSprite *nodePtr;

    nodePtr = (BattleFxBerserkFadeSprite *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecChainExplosionSpriteReq.frame = nodePtr->frame;
    q = nodePtr->ringIndex;
    g_BattleSpecChainExplosionSpriteReq.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(0, g_BattleSpecChainExplosionLoadedResTable);
    g_BattleSpecChainExplosionSpriteReq.drawObj = Gfx_ModelTableGetSpriteObject(0, g_BattleSpecChainExplosionLoadedResTable);
    if (g_BattleSpecChainExplosionSpriteReq.spriteGroup != 0) {
        g_BattleSpecChainExplosionSpriteReq.spriteGroup->renderObjects[0] = (GfxRenderObject *)Gfx_ModelTableGetSpriteObject(0, g_BattleSpecChainExplosionLoadedResTable);
        g_BattleSpecChainExplosionSpriteReq.spriteGroup->renderObjects[1] = (GfxRenderObject *)Gfx_ModelTableGetSpriteObject(1, g_BattleSpecChainExplosionLoadedResTable);
    }
    for (idx = 0; idx < 4; idx++) {
        if (g_BattleIsPaused == 0) {
            normal.vx = g_BattleSpecChainExplosionTargetPos.vx - g_BattleSpecChainExplosionExplosionCoords[q].posX;
            normal.vy = g_BattleSpecChainExplosionTargetPos.vy - g_BattleSpecChainExplosionExplosionCoords[q].posY;
            normal.vz = g_BattleSpecChainExplosionTargetPos.vz - g_BattleSpecChainExplosionExplosionCoords[q].posZ;
            r = NormalizeVectorFixed(&normal, &normal);
            r = SqrtToInt(r) >> 5;
            g_BattleSpecChainExplosionExplosionCoords[q].posX += (short)(r * normal.vx >> 0xC);
            g_BattleSpecChainExplosionExplosionCoords[q].posY += (short)(r * normal.vy >> 0xC);
            g_BattleSpecChainExplosionExplosionCoords[q].posZ += (short)(r * normal.vz >> 0xC);
        }
        BattleFx_InstallBillboardMatrix(&g_BattleSpecChainExplosionExplosionCoords[q].posX, (short)g_BattleSpecChainExplosionExplosionCoords[q].scale, -(g_BattleSpecChainExplosionExplosionCoords[q].scale >> 3));
        g_BattleFxActiveDrawListPtr = BattleFx_SubmitSpriteDraw(((unsigned char *)&g_BattleSpecChainExplosionSpriteReq), g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
        q++;
        if (q >= 0x137) {
            q = 0;
        }
    }
    if (nodePtr->frame >= 7) {
        nodePtr->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    (nodePtr->frame)++;
}
} /* extern "C" */
