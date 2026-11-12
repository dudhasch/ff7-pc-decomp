// src/ff7/src/battle/yasui/yasui_limit_breath_of_the_earth_orbit_trails.cpp
// Limit Break [16] (Aeris Lv 2-1): Breath of the Earth (Japanese: 大地の息吹, dev tag: "LIMIT/DAICHI" / Daichi)
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Daichi limit break effect: per-target timeline tasks that spawn paired orbiting trails,
//   drawn as textured PSX ribbons, plus animated billboard sprites.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/enums/sound.h"
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

struct GfxPsxSpriteQuadState;
// clang-format off
// GLOBAL: FF7 0x007e35a0
int g_YasuiLimitBreathOfTheEarthResourceHeader = 0x00192A39;
char *g_YasuiLimitBreathOfTheEarthOrbitTrailsResourceNames[1] = {
    // STRING: FF7 0x007e35d8
    "DAICHI.TIM"
};
typedef struct {
    char *filename;
    unsigned int descriptor;
} YasuiLimitDaichiResourceRecord;
YasuiLimitDaichiResourceRecord g_YasuiLimitBreathOfTheEarthOrbitTrailsAssetPrefix[4] = {
    {
        // STRING: FF7 0x007e35e4
        "DAICHI.SPT",
        0x00000000
    },
    {
        0,
        0x00197a39
    },
    {
        // STRING: FF7 0x007e35f0
        "hl_wind.TIM",
        0x00000000
    },
    {
        0,
        0x00000000
    }
};
// GLOBAL: FF7 0x007e35c8
struct YasuiLimitBreathOfTheEarthSpriteRequest g_YasuiLimitBreathOfTheEarthSpriteRequest = {0, 0, {0x80, 0x80, 0x80, 0x2e}, 0, {0}};
// clang-format on

#pragma intrinsic(memcpy)
#pragma intrinsic(memset)
#pragma intrinsic(abs)


// clang-format off
// GLOBAL: FF7 0x00a394d8
struct YasuiLimitBreathOfTheEarthTrailScratch *g_YasuiLimitBreathOfTheEarthTrailScratchPtr = 0;
// GLOBAL: FF7 0x00a394dc
int g_YasuiLimitBreathOfTheEarthRenderPacketCursor = 0;
// GLOBAL: FF7 0x00a394e0
struct Vec3sPacked *g_YasuiLimitBreathOfTheEarthOrbitScratchPtr = 0;
unsigned char g_bssPad_00a394e4[4] = {0};
// GLOBAL: FF7 0x00a394e8
char g_YasuiLimitBreathOfTheEarthOrbitScratchInit = 0;
// GLOBAL: FF7 0x00a394ec
char g_YasuiLimitBreathOfTheEarthTrailScratchInit = 0;
// GLOBAL: FF7 0x00a394f0
BattleFxPsxTrailRibbon g_YasuiLimitBreathOfTheEarthTrailBuffers[6] = {0};
// GLOBAL: FF7 0x00a398f8
struct GfxTableCursor *g_YasuiLimitBreathOfTheEarthResourceHandle = 0;
// GLOBAL: FF7 0x00a398fc
int g_YasuiLimitBreathOfTheEarthTrailBufferCount = 0;
// GLOBAL: FF7 0x00a39900
int g_YasuiLimitBreathOfTheEarthOrbitAnglePhase = 0;
// clang-format on

/* Loads the Daichi limit resources into the global resource handle if absent. */
// FUNCTION: FF7 0x0045adf0
void __cdecl BattleFx_LimitLoadResBreathOfTheEarth(void)
{
    GfxModelTableBank *bits;
    GfxResourceRequest requestStorage;
    int kind;
    char pathBuffer[200];
    GfxResourceRequest *requestPtr;
    int def;

    if (g_pBattle3dScene != 0 && g_YasuiLimitBreathOfTheEarthResourceHandle == 0) {
        kind = 2;
        def = (int)&g_YasuiLimitBreathOfTheEarthResourceHeader;
        bits = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        requestPtr = &requestStorage;
        Gfx_InitResourceRequest(requestPtr);
        requestPtr->groupTag = 0;
        // STRING: FF7 0x007e35fc
        requestPtr->resourcePath = BattleFx_BuildResourcePath("LIMIT/DAICHI", pathBuffer);
        Gfx_LoadModelTableIfAbsent(kind, def, requestPtr, &g_YasuiLimitBreathOfTheEarthResourceHandle, bits);
    }
}

/* Initializes state, loads Daichi limit resources, and triggers the effect. */
// FUNCTION: FF7 0x0045ae80
int __cdecl BattleFx_LimitEnterBreathOfTheEarth(int targetMask, int actorIdx)
{
    BattleFx_LimitLoadResBreathOfTheEarth();
    g_YasuiLimitBreathOfTheEarthTrailBufferCount = 0;
    BattleFx_LimitBeginBreathOfTheEarth(targetMask, actorIdx);
    return 0;
}

/* Stores the render state block, creates per-target tasks, and plays sound. */
// FUNCTION: FF7 0x0045aea6
void __cdecl BattleFx_LimitBeginBreathOfTheEarth(int targetMask, int actorIdx)
{
    g_YasuiLimitBreathOfTheEarthRenderPacketCursor = Gfx_GetPsxPacketScratchPtr();
    Battle_CreateBitPatternCallbackTask(targetMask, actorIdx, 0, BattleFx_LimitSpawnBreathOfTheEarthPerTarget);
    Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputePanForEntity(targetMask), 0x1DB);
}

/* Spawns the Daichi timeline task and attaches it to the target actor's bone. */
// FUNCTION: FF7 0x0045aee8
void __cdecl BattleFx_LimitSpawnBreathOfTheEarthPerTarget(int actor)
{
    YasuiBreathOfTheEarthTimeline *slot;

    slot = (YasuiBreathOfTheEarthTimeline *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitBreathOfTheEarthTimelineTask);
    slot->actorIndex = actor;
    Battle_GetBoneWorldPos(actor, g_BattleActorRecords[actor].targetBone, (short *)&slot->worldPos);
}

/* Timeline controller task for Daichi: spawns paired orbiting trail tasks. */
// FUNCTION: FF7 0x0045af39
void __cdecl BattleFx_LimitBreathOfTheEarthTimelineTask(void)
{
    YasuiBreathOfTheEarthTimeline *task;
    BattleFxBreathOfTheEarthTrailRecord *pTrail;

    task = (YasuiBreathOfTheEarthTimeline *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (task->step == 0) {
        g_YasuiLimitBreathOfTheEarthOrbitAnglePhase = 0;
    }
    if (task->step == 0) {
        pTrail = (BattleFxBreathOfTheEarthTrailRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitBreathOfTheEarthOrbitTrailTask);
        pTrail->worldPos = task->worldPos;
        pTrail->trailBuffer = (BattleFxPsxTrailRibbon *)(g_YasuiLimitBreathOfTheEarthTrailBufferCount * 0xAC + (int)&g_YasuiLimitBreathOfTheEarthTrailBuffers);
        g_YasuiLimitBreathOfTheEarthTrailBufferCount++;
        pTrail->velocityX = 0xBB8;
        pTrail->velocityY = 0xFFAD;
        pTrail->orbitAngle = *(short *)&g_YasuiLimitBreathOfTheEarthOrbitAnglePhase;
    }
    if (task->step == 0) {
        pTrail = (BattleFxBreathOfTheEarthTrailRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitBreathOfTheEarthOrbitTrailTask);
        pTrail->worldPos = task->worldPos;
        pTrail->trailBuffer = (BattleFxPsxTrailRibbon *)(g_YasuiLimitBreathOfTheEarthTrailBufferCount * 0xAC + (int)&g_YasuiLimitBreathOfTheEarthTrailBuffers);
        g_YasuiLimitBreathOfTheEarthTrailBufferCount++;
        pTrail->velocityX = 0xF448;
        pTrail->velocityY = 0x53;
        pTrail->orbitAngle = *(short *)&g_YasuiLimitBreathOfTheEarthOrbitAnglePhase;
        g_YasuiLimitBreathOfTheEarthOrbitAnglePhase ^= 0x800;
    }
    if (task->step == 0xF) {
        Battle_PostActorHitOrHurtMessage(task->actorIndex);
    }
    if (task->step == 0xF) {
        task->actorIndex = 0xFFFF;
    }
    (task->step)++;
}

/* Orbiting trail particle task: steps circular motion, records trail points, and spawns sprite particles. */
// FUNCTION: FF7 0x0045b0cf
void __cdecl BattleFx_LimitBreathOfTheEarthOrbitTrailTask(void)
{
    int frameIndex;
    int fadeAlpha;
    YasuiBreathOfTheEarthOrbit *orbitPtr;
    int i;
    int height;
    BattleFxBolt3MultiParticle *pSpriteTask;
    int blend;

    orbitPtr = (YasuiBreathOfTheEarthOrbit *)BATTLE_CURRENT_TASK_SLOT;
    if ((*(unsigned char *)&g_YasuiLimitBreathOfTheEarthOrbitScratchInit & 1) == 0) {
        g_YasuiLimitBreathOfTheEarthOrbitScratchInit |= 1;
        g_YasuiLimitBreathOfTheEarthOrbitScratchPtr = (struct Vec3sPacked *)Gfx_GetPsxScratchpadPtr(0);
    }
    if (orbitPtr->frameCount == 0) {
        orbitPtr->vertexBuffer->points[0].x = orbitPtr->radius * CosFixed(orbitPtr->angle) >> 0xC;
        orbitPtr->vertexBuffer->points[0].y = 0;
        orbitPtr->vertexBuffer->points[0].z = -orbitPtr->radius * SinFixed(orbitPtr->angle) >> 0xC;
        orbitPtr->vertexBuffer->count = 0;
        orbitPtr->vertexBuffer->headIndex = 1;
    }
    if (g_BattleIsPaused == 0) {
        for (i = 0; i < 2; i++) {
            orbitPtr->angle += (short)(0x1F400 / orbitPtr->radius);
            frameIndex = orbitPtr->frameCount;
            if (frameIndex < 0x1E) {
                blend = (frameIndex << 0xC) / 0x1E;
                orbitPtr->radius += orbitPtr->radiusSpeed;
                height = blend * 0 >> 0xC;
                if (orbitPtr->vertexBuffer->count < 0x14) {
                    (orbitPtr->vertexBuffer->count)++;
                }
                fadeAlpha = blend;
            } else {
                frameIndex -= 0x1E;
                if (frameIndex < 0x3C) {
                    blend = (frameIndex << 0xC) / 0x3C;
                    height = blend * -0x7D0 >> 0xC;
                    if (frameIndex > 0x28 && orbitPtr->vertexBuffer->count > 0) {
                        (orbitPtr->vertexBuffer->count)--;
                    }
                    fadeAlpha = (orbitPtr->vertexBuffer->count << 0xC) / 0x14;
                } else {
                    orbitPtr->ownerSlot = BATTLE_TASK_FREE;
                    return;
                }
            }
            g_YasuiLimitBreathOfTheEarthOrbitScratchPtr->vx = orbitPtr->centerX + (orbitPtr->radius * CosFixed(orbitPtr->angle) >> 0xC);
            orbitPtr->vertexBuffer->points[orbitPtr->vertexBuffer->headIndex].x = g_YasuiLimitBreathOfTheEarthOrbitScratchPtr->vx;
            g_YasuiLimitBreathOfTheEarthOrbitScratchPtr->vy = height;
            orbitPtr->vertexBuffer->points[orbitPtr->vertexBuffer->headIndex].y = height;
            g_YasuiLimitBreathOfTheEarthOrbitScratchPtr->vz = orbitPtr->centerY - (orbitPtr->radius * SinFixed(orbitPtr->angle) >> 0xC);
            orbitPtr->vertexBuffer->points[orbitPtr->vertexBuffer->headIndex].z = g_YasuiLimitBreathOfTheEarthOrbitScratchPtr->vz;
            orbitPtr->vertexBuffer->headIndex = (orbitPtr->vertexBuffer->headIndex + 1) % 0x15;
            (orbitPtr->frameCount)++;
        }
        for (i = 0; i < 1; i++) {
            pSpriteTask = (BattleFxBolt3MultiParticle *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitBreathOfTheEarthAnimatedSpriteTask);
            pSpriteTask->posX = g_YasuiLimitBreathOfTheEarthOrbitScratchPtr->vx + rand() % 0x258 + -0x12C;
            pSpriteTask->posY = g_YasuiLimitBreathOfTheEarthOrbitScratchPtr->vy + rand() % 0x258 + -0x12C;
            pSpriteTask->posZ = g_YasuiLimitBreathOfTheEarthOrbitScratchPtr->vz + rand() % 0x258 + -0x12C;
        }
    }
    BattleFx_LimitBreathOfTheEarthDrawPsxTrailRibbon(orbitPtr->vertexBuffer, fadeAlpha);
}

/* Submits an animated billboard sprite draw for Daichi. */
// FUNCTION: FF7 0x0045b487
void __cdecl BattleFx_LimitBreathOfTheEarthAnimatedSpriteTask(void)
{
    BattleTaskSlot *task;

    task = BATTLE_CURRENT_TASK_SLOT;
    (g_YasuiLimitBreathOfTheEarthSpriteRequest.animBlock) = Gfx_ModelTableGetLoadedRecord(0, g_YasuiLimitBreathOfTheEarthResourceHandle);
    g_YasuiLimitBreathOfTheEarthSpriteRequest.battleLimitDaichiSpriteResource = Gfx_ModelTableGetSpriteObject(0, g_YasuiLimitBreathOfTheEarthResourceHandle);
    BattleFx_InstallBillboardMatrix((void *)((int)task + 8), 0x1000, 0);
    g_YasuiLimitBreathOfTheEarthSpriteRequest.battleLimitDaichiSpriteFrameIndex = task->frameCounter;
    g_YasuiLimitBreathOfTheEarthRenderPacketCursor = BattleFx_SubmitSpriteDraw(&g_YasuiLimitBreathOfTheEarthSpriteRequest, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_YasuiLimitBreathOfTheEarthRenderPacketCursor);
    if (g_BattleIsPaused == 0 && (g_YasuiLimitBreathOfTheEarthSpriteRequest.animBlock) != 0) {
        (task->frameCounter)++;
        if (task->frameCounter >= *(int *)((char *)*(int *)(g_YasuiLimitBreathOfTheEarthSpriteRequest.animBlock) + 4)) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/* Draws a textured PSX quad ribbon connecting consecutive points in the orbit trail. */
// FUNCTION: FF7 0x0045b558
void __cdecl BattleFx_LimitBreathOfTheEarthDrawPsxTrailRibbon(BattleFxPsxTrailRibbon *trail, int alpha)
{
    int nextIndex;
    int currentIndex;
    int i;
    BattleFxBreathOfTheEarthRibbonPacket *packetPtr;
    GfxPsxSpriteQuadState *spriteStatus;

    if ((*(unsigned char *)&g_YasuiLimitBreathOfTheEarthTrailScratchInit & 1) == 0) {
        g_YasuiLimitBreathOfTheEarthTrailScratchInit |= 1;
        g_YasuiLimitBreathOfTheEarthTrailScratchPtr = (struct YasuiLimitBreathOfTheEarthTrailScratch *)(Gfx_GetPsxScratchpadPtr(0));
    }
    SetRotMatrix(&g_BattleCameraTransform);
    SetTransMatrix(&g_BattleCameraTransform);
    g_YasuiLimitBreathOfTheEarthTrailScratchPtr->primitiveWord = alpha << 7 >> 0xC;
    g_YasuiLimitBreathOfTheEarthTrailScratchPtr->primitiveWord = g_YasuiLimitBreathOfTheEarthTrailScratchPtr->primitiveWord << 0x10 | 0x2E000000 | g_YasuiLimitBreathOfTheEarthTrailScratchPtr->primitiveWord << 8 | g_YasuiLimitBreathOfTheEarthTrailScratchPtr->primitiveWord;
    RefreshGlobalTransformState();
    packetPtr = (BattleFxBreathOfTheEarthRibbonPacket *)Gfx_GetPsxPacketScratchPtr();
    currentIndex = (trail->headIndex + 0x15 - trail->count - 1) % 0x15;
    for (i = 0; i < trail->count; i++) {
        nextIndex = (currentIndex + 1) % 0x15;
        g_YasuiLimitBreathOfTheEarthTrailScratchPtr->currentSampleA = trail->points[currentIndex].x;
        g_YasuiLimitBreathOfTheEarthTrailScratchPtr->currentOffsetA = trail->points[currentIndex].y - 0x64;
        g_YasuiLimitBreathOfTheEarthTrailScratchPtr->currentSampleZA = trail->points[currentIndex].z;
        g_YasuiLimitBreathOfTheEarthTrailScratchPtr->currentSampleB = trail->points[currentIndex].x;
        g_YasuiLimitBreathOfTheEarthTrailScratchPtr->currentOffsetB = trail->points[currentIndex].y + 0x64;
        g_YasuiLimitBreathOfTheEarthTrailScratchPtr->currentSampleZB = trail->points[currentIndex].z;
        g_YasuiLimitBreathOfTheEarthTrailScratchPtr->nextSampleA = trail->points[nextIndex].x;
        g_YasuiLimitBreathOfTheEarthTrailScratchPtr->nextOffsetA = trail->points[nextIndex].y - 0x64;
        g_YasuiLimitBreathOfTheEarthTrailScratchPtr->nextSampleZA = trail->points[nextIndex].z;
        g_YasuiLimitBreathOfTheEarthTrailScratchPtr->nextSampleB = trail->points[nextIndex].x;
        g_YasuiLimitBreathOfTheEarthTrailScratchPtr->nextOffsetB = trail->points[nextIndex].y + 0x64;
        g_YasuiLimitBreathOfTheEarthTrailScratchPtr->nextSampleZB = trail->points[nextIndex].z;
        g_YasuiLimitBreathOfTheEarthTrailScratchPtr->depth = Gfx_TransformQuadReturnDepth((void *)(((int)g_YasuiLimitBreathOfTheEarthTrailScratchPtr)), (SVECTOR *)(((int)g_YasuiLimitBreathOfTheEarthTrailScratchPtr) + 8), (SVECTOR *)(((int)g_YasuiLimitBreathOfTheEarthTrailScratchPtr) + 0x10), (SVECTOR *)(((int)g_YasuiLimitBreathOfTheEarthTrailScratchPtr) + 0x18), (int *)(((int)g_YasuiLimitBreathOfTheEarthTrailScratchPtr) + 0x24), (int *)(((int)g_YasuiLimitBreathOfTheEarthTrailScratchPtr) + 0x28));
        if ((g_YasuiLimitBreathOfTheEarthTrailScratchPtr->flags & 0x60000) == 0) {
            packetPtr->tag = 0x9000000;
            packetPtr->primitiveWord = g_YasuiLimitBreathOfTheEarthTrailScratchPtr->primitiveWord;
            if (i == 0) {
                packetPtr->color0 = 0x3F40;
                packetPtr->color1 = 0x3F7F;
                packetPtr->color2 = 0x40;
                packetPtr->color3 = 0x7F;
            } else if (i == trail->count - 1) {
                packetPtr->color0 = 0x40;
                packetPtr->color1 = 0x7F;
                packetPtr->color2 = 0x3F40;
                packetPtr->color3 = 0x3F7F;
            } else {
                packetPtr->color0 = 0;
                packetPtr->color1 = 0x3F;
                packetPtr->color2 = 0x3F00;
                packetPtr->color3 = 0x3F3F;
            }
            packetPtr->clut = 0x7848;
            packetPtr->tpage = 0x2D;
            spriteStatus = (GfxPsxSpriteQuadState *)(Gfx_ModelTableGetSpriteObject(1, g_YasuiLimitBreathOfTheEarthResourceHandle));
            if (spriteStatus != 0) {
                Gfx_SetPsxPrimQuadState(0xB, 1, spriteStatus);
            }
            Gfx_DrawPsxPrimitive(packetPtr);
        }
        currentIndex = nextIndex;
    }
}
} /* extern "C" */
