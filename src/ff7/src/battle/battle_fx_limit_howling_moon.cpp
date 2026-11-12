// src/ff7/src/battle/battle_fx_limit_howling_moon.cpp
// Limit Break [36] (Red XIII Lv 2-2): Howling Moon (Japanese: ハウリングムーン, dev tag: "limit3/h_moon" / HMoon)
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: H-Moon limit break effect: a timeline drives fading billboard sprites and wave strips,
//   which a helper draws as strips.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/enums/sound.h"
#include "ff7/types/battle.h"
#include "lib/types/graphics.h"
#include "lib/types/polygon.h"
#include "ff7/camera_script.h"
#include <stdlib.h>
#include <string.h>
#include "lib/codegen.h"
#include "ff7/api/battle.h"
#include "ff7/api/menu.h"
#include "lib/api/sound.h"
#include "lib/api/graphics.h"
#include "ff7/battle_tasks.h"
#include "ff7/types/battle_limit.h"
extern "C" {

#pragma intrinsic(memset)
#pragma function(memcpy)
// clang-format off
// GLOBAL: FF7 0x0082d188
unsigned int g_BattleLimitHowlingMoonModelRecords[20] ={
    0x0001383c,
    (unsigned int)&g_BattleLimitHowlingMoonResNameMoonTim,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00013839,
    // STRING: FF7 0x0082d214
    (unsigned int)"moon.tim",
    0x00000000,
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x0082d220
    (unsigned int)"MOON.TIM",
    // STRING: FF7 0x0082d22c
    (unsigned int)"MOON.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x0082d238
    (unsigned int)"SMOKE_1.TIM",
    // STRING: FF7 0x0082d244
    (unsigned int)"SMOKE_1.SPT",
    0x00000000,
    0x00000000
};
// GLOBAL: FF7 0x0082d1d8
BattleFxSpriteRequest g_BattleLimitHowlingMoonSpriteRequest = {0, 0, -128, -128, -128, 0x2e, 0, 0};
unsigned char g_rawData_0082d1e8[16] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x2e, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x0082d1f8
unsigned char g_BattleLimitHowlingMoonNegZVector[12] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0xff, 0xff};
// GLOBAL: FF7 0x0082d208
unsigned char g_BattleLimitHowlingMoonResNameMoonTim[9] = "moon.tim";
// GLOBAL: FF7 0x00b37180
int g_BattleLimitHowlingMoonTargetMask = 0;
// GLOBAL: FF7 0x00b37184
int g_BattleLimitHowlingMoonActorIndex = 0;
// GLOBAL: FF7 0x00b37188
PsxMatrix g_BattleLimitHowlingMoonRotationMatrix = {0};
// GLOBAL: FF7 0x00b371a8
int g_BattleLimitHowlingMoonCameraDirX = 0;
// GLOBAL: FF7 0x00b371ac
int g_BattleLimitHowlingMoonCameraDirY = 0;
// GLOBAL: FF7 0x00b371b0
int g_BattleLimitHowlingMoonCameraDirZ = 0;
unsigned char g_bssPad_00b371b4[4] = {0};
// GLOBAL: FF7 0x00b371b8
struct GfxTableCursor *g_BattleLimitHowlingMoonLoadedResTable = 0;
unsigned char g_bssPad_00b371bc[4] = {0};
// clang-format on
#pragma data_seg(".data$u4c_orbit")
#pragma bss_seg(".data$u4c_orbit")
#pragma data_seg(".bss$u4c_orbit_slot")
#pragma bss_seg(".bss$u4c_orbit_slot")
#pragma data_seg(".bss$u4c_orbit_flag")
#pragma bss_seg(".bss$u4c_orbit_flag")
#pragma data_seg()
#pragma bss_seg()

/*
 * Loads the model and sprite resource table for Red XIII's Howling Moon limit break.
 */
// FUNCTION: FF7 0x004cb120
void __cdecl BattleFx_LimitLoadResHowlingMoon(void)
{
    GfxModelTableBank *context;
    GfxResourceRequest requestBuffer;
    int kind;
    char pathBuffer[200];
    GfxResourceRequest *request;
    int defPtr;

    if (g_pBattle3dScene != 0 && g_BattleLimitHowlingMoonLoadedResTable == 0) {
        kind = 4;
        defPtr = (int)&g_BattleLimitHowlingMoonModelRecords;
        context = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        request = &requestBuffer;
        Gfx_InitResourceRequest(request);
        request->groupTag = 0;
        // STRING: FF7 0x0082d250
        request->resourcePath = BattleFx_BuildResourcePath("limit3/h_moon", pathBuffer);
        Gfx_LoadModelTableIfAbsent(kind, defPtr, request, &g_BattleLimitHowlingMoonLoadedResTable, context);
    }
}

/*
 * Preloads the Howling Moon sound effect on limit entry.
 */
// FUNCTION: FF7 0x004cb1b0
int __cdecl BattleFx_LimitEnterHowlingMoon(int targetMask, int actorIdx)
{
    BattleFx_LimitLoadResHowlingMoon();
    SfxLoadIntoBuffer(SFX_ID_HOWLING_MOON, 0);
    BattleFx_LimitBeginHowlingMoon(targetMask, actorIdx);
    return 0;
}

/*
 * Initializes the draw list and spawns the Howling Moon timeline task for actorIdx.
 */
// FUNCTION: FF7 0x004cb1db
void __cdecl BattleFx_LimitBeginHowlingMoon(int targetMask, int actorIdx)
{
    *(int *)&g_BattleFxDrawListBufferTaskSlotPtr = (int)BATTLE_NEW_TASK_SLOT(BattleFx_SelectDrawListBuffer);
    g_BattleFxActiveDrawListPtr = Gfx_GetPsxPacketScratchPtr();
    Battle_AllocateResourceSlot(BattleFx_LimitHowlingMoonSpawnTimeline);
    g_BattleLimitHowlingMoonTargetMask = targetMask;
    g_BattleLimitHowlingMoonActorIndex = actorIdx;
}

/*
 * Allocates and configures the active Howling Moon timeline slot from the caster's radius.
 */
// FUNCTION: FF7 0x004cb226
void __cdecl BattleFx_LimitHowlingMoonSpawnTimeline(void)
{
    BattleFxLimitHowlingMoonTimeline *task;
    int dist;

    task = (BattleFxLimitHowlingMoonTimeline *)BATTLE_CURRENT_TASK_SLOT;
    task->ownerSlot = BATTLE_TASK_FREE;
    task = (BattleFxLimitHowlingMoonTimeline *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitHowlingMoonTimelineTask);
    Battle_GetBoneWorldPos(g_BattleLimitHowlingMoonActorIndex, g_BattleActorRecords[g_BattleLimitHowlingMoonActorIndex].targetBone, &task->posX);
    dist = g_BattleActorRecords[g_BattleLimitHowlingMoonActorIndex].radius + 384;
    if (dist > 1900) {
        dist = 1900;
    }
    task->spread = dist;
    task->waveStarted = 0;
    Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_CH1, Battle_ComputeScreenPanFromPosition(&task->posX), 0x128);
    Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_CH2, Battle_ComputeScreenPanFromPosition(&task->posX), 0x1AA);
}

/*
 * Advances the Howling Moon effect timeline, camera-aligned aura rotation, and wave strip spawns.
 */
// FUNCTION: FF7 0x004cb30d
void __cdecl BattleFx_LimitHowlingMoonTimelineTask(void)
{
    unsigned short fadeLevel;
    int index;
    VECTOR axisVector;
    int fixedScale;
    VECTOR vecX;
    PsxMatrix mtxA;
    VECTOR deltaX;
    BattleFxLimitHowlingMoonTimeline *pW;
    BattleFxLimitHowlingMoonState *pG;
    int angle;
    int dstMagnitude;
    int phase;

    pW = (BattleFxLimitHowlingMoonTimeline *)BATTLE_CURRENT_TASK_SLOT;
    vecX.vx = g_BattleCameraPosition.cameraPosX - pW->posX;
    vecX.vy = 0;
    vecX.vz = g_BattleCameraPosZ - pW->posZ;
    NormalizeVectorFixed(&vecX, &g_BattleLimitHowlingMoonCameraDirX);
    angle = ComputeAngleBetweenVectors((VECTOR *)&g_BattleLimitHowlingMoonNegZVector, (VECTOR *)&g_BattleLimitHowlingMoonCameraDirX, &axisVector);
    Gte_BuildAxisAngleMatrix(angle, (PsxMatrix *)&g_BattleLimitHowlingMoonRotationMatrix, &axisVector);
    deltaX.vx = g_BattleCameraPosition.cameraPosX - pW->posX;
    deltaX.vy = g_BattleCameraPosition.cameraPosY - pW->posY;
    deltaX.vz = g_BattleCameraPosZ - pW->posZ;
    NormalizeVectorFixed(&deltaX, &deltaX);
    angle = ComputeAngleBetweenVectors((VECTOR *)&g_BattleLimitHowlingMoonCameraDirX, &deltaX, &axisVector);
    Gte_BuildAxisAngleMatrix(angle, &mtxA, &axisVector);
    ConcatMatrixInPlace(&mtxA, &g_BattleLimitHowlingMoonRotationMatrix);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (pW->frameCount == 2) {
        pW->waveStarted = 1;
        pG = (BattleFxLimitHowlingMoonState *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitHowlingMoonWaveStripTask);
        *(Int64Pair *)&pG->posX = *(Int64Pair *)&g_BattleActorRecords[g_BattleLimitHowlingMoonActorIndex].posX;
        pG->posY += 0xF6A0;
        pG->scale = pW->spread;
        pG->modelScale = 0xC80;
    }
    if (pW->frameCount >= 58 && pW->frameCount <= 60) {
        fixedScale = 0xC80;
        for (index = 0; index < 16; index++) {
            pG = (BattleFxLimitHowlingMoonState *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitHowlingMoonFadingBillboardSpriteTask);
            *(Int64Pair *)&pG->posX = *(Int64Pair *)&g_BattleActorRecords[g_BattleLimitHowlingMoonActorIndex].posX;
            phase = rand() % 4096;
            dstMagnitude = rand() % 80 + 200;
            pG->posX += (short)(SinFixed(phase) * dstMagnitude >> 12);
            pG->posZ += (short)(CosFixed(phase) * dstMagnitude >> 12);
            dstMagnitude = rand() % 50 + 80;
            pG->velocityX = SinFixed(phase) * dstMagnitude >> 12;
            pG->velocityZ = CosFixed(phase) * dstMagnitude >> 12;
            dstMagnitude = rand() % 1536 + 2560;
            pG->scale = fixedScale * dstMagnitude >> 12;
        }
    }
    if (pW->frameCount < 8) {
        g_BattleFxTimelineState.red = 0x40;
        g_BattleFxTimelineState.channel2 = 0;
        g_BattleFxTimelineState.channel1 = g_BattleFxTimelineState.channel2;
        fadeLevel = pW->frameCount << 8;
    } else if (pW->frameCount >= 62) {
        fadeLevel = 2048 - (pW->frameCount - 62 << 8);
    } else {
        fadeLevel = 2048;
    }
    if (pW->frameCount == 65) {
        for (index = 0; index < 10; index++) {
            if ((g_BattleLimitHowlingMoonTargetMask & 1 << index) != 0) {
                Battle_PostActorHitOrHurtMessage(index);
            }
        }
    }
    if (pW->frameCount >= 70) {
        fadeLevel = 0;
        g_BattleFxTimelineState.red = fadeLevel;
        pW->ownerSlot = BATTLE_TASK_FREE;
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_CH1, Battle_ComputeScreenPanFromPosition(&pW->posX), 0);
        Sfx_ReleaseCachedBuffer(0x1AA, 0xF);
    }
    g_BattleFxTimelineState.startValue = fadeLevel;
    (pW->frameCount)++;
}

/*
 * Draws and fades an expanding Howling Moon aura billboard sprite.
 */
// FUNCTION: FF7 0x004cb71d
void __cdecl BattleFx_LimitHowlingMoonFadingBillboardSpriteTask(void)
{
    unsigned char brightness;
    BattleFxHowlingMoonFadingSprite *task;

    task = (BattleFxHowlingMoonFadingSprite *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleLimitHowlingMoonSpriteRequest.frame = task->frame;
    if (task->frame >= 9) {
        brightness = 0x60 - (task->frame - 9 << 4);
    } else {
        brightness = 0x60;
    }
    g_BattleLimitHowlingMoonSpriteRequest.colorB = brightness;
    g_BattleLimitHowlingMoonSpriteRequest.colorG = g_BattleLimitHowlingMoonSpriteRequest.colorB;
    g_BattleLimitHowlingMoonSpriteRequest.colorR = g_BattleLimitHowlingMoonSpriteRequest.colorG;
    BattleFx_InstallBillboardMatrix(&task->posX, (short)task->spriteIndex, -(task->spriteIndex >> 3));
    g_BattleLimitHowlingMoonSpriteRequest.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(3, g_BattleLimitHowlingMoonLoadedResTable);
    g_BattleLimitHowlingMoonSpriteRequest.drawObj = Gfx_ModelTableGetSpriteObject(3, g_BattleLimitHowlingMoonLoadedResTable);
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitSpriteDraw(&g_BattleLimitHowlingMoonSpriteRequest, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (task->frame >= 15) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    task->posX += task->velocityX;
    task->posZ += task->velocityZ;
    task->velocityX -= (short)(task->velocityX >> 3);
    task->velocityZ -= (short)(task->velocityZ >> 3);
    (task->frame)++;
}

/*
 * Updates transform state and submits one animated Howling Moon wave-strip ring.
 */
// FUNCTION: FF7 0x004cb897
void __cdecl BattleFx_LimitHowlingMoonWaveStripTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[72];
        PsxMatrix modelMatrix;
        struct { unsigned char pad_to_20[0x20]; BattleFxLimitHowlingMoonWaveStrip *waveStrip; };
        struct { unsigned char pad_to_2C[0x2C]; BattleFxMoonWaveStrips *waveStrips; };
        struct { unsigned char pad_to_30[0x30]; SVECTOR rotX; };
        struct { unsigned char pad_to_38[0x38]; VECTOR scale; };
    } frame;
#pragma pack(pop)

    frame.waveStrip = (BattleFxLimitHowlingMoonWaveStrip *)BATTLE_CURRENT_TASK_SLOT;
    frame.rotX.vx = 0;
    frame.rotX.vy = 0;
    frame.rotX.vz = 0;
    RotMatrixXYZ(&frame.rotX, &frame.modelMatrix);
    ConcatMatrixInPlace(&g_BattleLimitHowlingMoonRotationMatrix, &frame.modelMatrix);
    frame.modelMatrix.t[0] = frame.waveStrip->posX - (frame.waveStrip->spread * g_BattleLimitHowlingMoonCameraDirX >> 12);
    frame.modelMatrix.t[1] = frame.waveStrip->posY - (frame.waveStrip->spread * g_BattleLimitHowlingMoonCameraDirY >> 12);
    frame.modelMatrix.t[2] = frame.waveStrip->posZ - (frame.waveStrip->spread * g_BattleLimitHowlingMoonCameraDirZ >> 12);
    frame.scale.vz = frame.waveStrip->uniformScale;
    frame.scale.vy = frame.scale.vz;
    frame.scale.vx = frame.scale.vy;
    ScaleMatrix(&frame.modelMatrix, &frame.scale);
    MultiplyMatrix(&g_BattleCameraTransform, &frame.modelMatrix, &frame.modelMatrix);
    SetRotMatrix(&frame.modelMatrix);
    SetTransMatrix(&frame.modelMatrix);
    RefreshGlobalTransformState();
    frame.waveStrips = (BattleFxMoonWaveStrips *)Gfx_GetPsxScratchpadPtr(0);
    frame.waveStrips->stepOffset = 0x20;
    frame.waveStrips->phase = frame.waveStrip->timer << 7;
    if (frame.waveStrip->timer <= 12) {
        frame.waveStrips->waveValue = CosFixed(frame.waveStrip->timer * 85);
        frame.waveStrips->waveValue = frame.waveStrips->waveValue << 11 >> 12;
        frame.waveStrips->enabled = 1;
    } else if (frame.waveStrip->timer >= 49) {
        frame.waveStrips->waveValue = SinFixed(frame.waveStrip->timer - 0x31 << 6);
        frame.waveStrips->waveValue = frame.waveStrips->waveValue << 11 >> 12;
        frame.waveStrips->enabled = 1;
    } else {
        frame.waveStrips->waveValue = 0;
        frame.waveStrips->enabled = 0;
    }
    if (frame.waveStrip->timer < 8) {
        frame.waveStrips->amplitude = frame.waveStrip->timer * 12;
    } else if (frame.waveStrip->timer >= 57) {
        frame.waveStrips->amplitude = 0x60 - (frame.waveStrip->timer - 0x39) * 12;
    } else {
        frame.waveStrips->amplitude = 0x60;
    }
    BattleFx_LimitHowlingMoonDrawWaveStrips(frame.waveStrips);
    if (frame.waveStrip->timer >= 65) {
        frame.waveStrip->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    (frame.waveStrip->timer)++;
}

/*
 * Emits PSX Gouraud strip primitives for a Howling Moon wave-strip ring.
 */
// FUNCTION: FF7 0x004cbae8
void __cdecl BattleFx_LimitHowlingMoonDrawWaveStrips(BattleFxMoonWaveStrips *taskData)
{
    int colorWord;
    BattleFxLimitHowlingMoonStripPrim *stripPtr;
    int baseB;
    int delta;
    int sinOut;
    int padField; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int baseRemaining;
    int baseUvB;
    int i;
    int phaseAngle;
    int primStorage[10];
    int uvWordA;

    colorWord = taskData->amplitude << 0x10 | taskData->amplitude << 8 | taskData->amplitude;
    stripPtr = (BattleFxLimitHowlingMoonStripPrim *)primStorage;
    taskData->quad[0].vx = 0xFC00;
    taskData->quad[0].vy = 0xFC00;
    taskData->quad[0].vz = 0;
    taskData->quad[1].vx = 1024;
    taskData->quad[1].vy = 0xFC00;
    taskData->quad[1].vz = 0;
    taskData->quad[2].vx = 0xFC00;
    taskData->quad[2].vy = 0xFC08;
    taskData->quad[2].vz = 0;
    taskData->quad[3].vx = 1024;
    taskData->quad[3].vy = 0xFC08;
    taskData->quad[3].vz = 0;
    phaseAngle = taskData->phase;
    baseRemaining = taskData->quad[0].vx;
    baseB = taskData->quad[1].vx;
    if (taskData->waveValue != 0) {
        sinOut = SinFixed(phaseAngle);
        phaseAngle += taskData->stepOffset;
        delta = taskData->waveValue * sinOut >> 12;
    } else {
        delta = 0;
    }
    taskData->quad[0].vx += delta;
    taskData->quad[1].vx += delta;
    uvWordA = 0x78080000;
    baseUvB = 0x2C00FF;
    for (i = 0; i < 255; i++) {
        if (taskData->waveValue != 0) {
            sinOut = SinFixed(phaseAngle);
            phaseAngle += taskData->stepOffset;
            delta = taskData->waveValue * sinOut >> 12;
        } else {
            delta = 0;
        }
        taskData->quad[2].vx = baseRemaining + delta;
        taskData->quad[3].vx = baseB + delta;
        taskData->depth = Gfx_TransformQuadReturnDepth((void *)taskData->quad, &taskData->quad[1], &taskData->quad[2], &taskData->quad[3], &taskData->unusedOut, &taskData->flags);
        taskData->quad[0].vx = taskData->quad[2].vx;
        taskData->quad[1].vx = taskData->quad[3].vx;
        taskData->quad[0].vy = taskData->quad[2].vy;
        taskData->quad[1].vy = taskData->quad[3].vy;
        taskData->quad[2].vy += 8;
        taskData->quad[3].vy += 8;
        sinOut = i << 8;
        stripPtr->uvWord0 = uvWordA + sinOut;
        stripPtr->uvWord1 = baseUvB + sinOut;
        stripPtr->uvWord2 = uvWordA + sinOut + 0x100;
        stripPtr->uvWord3 = baseUvB + sinOut + 0x100;
        stripPtr->colorWord = colorWord;
        stripPtr->tagWord = stripPtr->tagWord & 0xFFFFFF | 0x9000000;
        if (taskData->enabled != 0) {
            stripPtr->primCode = 0x2E;
            Gfx_SetPsxPrimQuadState(0xB, 1, (GfxPsxSpriteQuadState *)(Gfx_ModelTableGetSpriteObject(1, g_BattleLimitHowlingMoonLoadedResTable)));
        } else {
            stripPtr->primCode = 0x2C;
            Gfx_SetPsxPrimQuadState(0xB, 4, (GfxPsxSpriteQuadState *)(Gfx_ModelTableGetSpriteObject(0, g_BattleLimitHowlingMoonLoadedResTable)));
        }
        Gfx_DrawPsxPrimitive(stripPtr);
    }
    g_BattleFxActiveDrawListPtr = (int)stripPtr;
}
} /* extern "C" */
