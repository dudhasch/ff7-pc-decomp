// src/ff7/src/battle/battle_fx_blue_frog_song_effect.cpp
// Enemy Skill [0]: Frog Song (Japanese: カエルのうた, dev tag: "blue/kaeru" / Kaeru)
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: The Frog Song blue magic effect: musical note sprites emitted toward the target, with
//   aiming and expanding ring sprites.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/enums/sound.h"
#include "ff7/api/battle.h"
#include "ff7/battle_tasks.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "ff7/types/battle.h"
#include "lib/codegen.h"
#include "ff7/types/battle_fx.h"
#include "lib/types/graphics.h"
#include "ff7/camera_script.h"
#include <string.h>
#include <stdlib.h>
extern "C" {


#pragma intrinsic(memset)
#pragma intrinsic(memcpy)


/* ff7/gte.h declares it on VECTOR; this TU's calls pass the int locals that start each vector. */

// clang-format off
// GLOBAL: FF7 0x008aadc0
int g_BattleBlueFrogSongResourceDefs = 0x00192A39;
int g_BattleBlueFrogSongResourceDefsHeadPtr[1] = {
    (int)&g_BattleFxBlueFrogSongEffectResourceNames[8]
};
// GLOBAL: FF7 0x008aadc8
BattleBlueKaeruResources g_BattleBlueFrogSongResourceTable = {{
    (int)&g_BattleFxBlueFrogSongEffectResourceNames[0x14], 0x0, 0x0, 0x192a39, (int)&g_BattleFxBlueFrogSongEffectResourceNames[0x20], (int)&g_BattleFxBlueFrogSongEffectResourceNames[0x2c],
    0x0, 0x0}, 0x0, 0x0
};
// GLOBAL: FF7 0x008aadf0
struct BattleBlueFrogSongNoteSpriteReq g_BattleBlueFrogSongNoteSpriteReq = {{0x80, 0x80, 0x80, 0x2e}, 0, {0}, 0, 0};
// GLOBAL: FF7 0x008aae00
BattleBlueKaeruRingSpriteReq g_BattleBlueFrogSongRingSpriteReq = {-128, -128, -128, 0x2e};
unsigned char g_BattleFxBlueFrogSongEffectResourceNames[56] = {0x00, 0xf0, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x4b, 0x41, 0x45, 0x52, 0x55, 0x5f, 0x31, 0x2e, 0x54, 0x49, 0x4d, 0x00, 0x4b, 0x41, 0x45, 0x52, 0x55, 0x2e, 0x53, 0x50, 0x54, 0x00, 0x00, 0x00, 0x4f, 0x4e, 0x50, 0x55, 0x31, 0x2e, 0x54, 0x49, 0x4d, 0x00, 0x00, 0x00, 0x4f, 0x4e, 0x50, 0x55, 0x31, 0x2e, 0x53, 0x50, 0x54, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x00ba4a58
struct BattleBlueFrogSongState g_BattleBlueFrogSongState = {0};
// g_BattleBlueFrogSongBillboardPoints is the first of a ring of 222 eight-byte billboard points that the
// code indexes as g_BattleBlueFrogSongBillboardRing[i], past the declared object into the bytes
// that follow it.
// GLOBAL: FF7 0x00ba4a60
struct BattleBlueFrogSongBillboardPoints g_BattleBlueFrogSongBillboardPoints = {0};
#define g_BattleBlueFrogSongBillboardRing (&g_BattleBlueFrogSongBillboardPoints)
unsigned char g_bssPad_00ba4a68[1768] = {0};
// GLOBAL: FF7 0x00ba5150
PsxMatrix32 g_BattleBlueFrogSongAimVector = {0};
// GLOBAL: FF7 0x00ba5170
struct GfxTableCursor *g_BattleBlueFrogSongLoadedResTable = 0;
// clang-format on

/* Loads Frog Song blue magic resources. */
// FUNCTION: FF7 0x0056ffd0
void __cdecl BattleFx_BlueLoadResFrogSong(void)
{
    GfxModelTableBank *stage;
    GfxResourceRequest requestSlot;
    int kind;
    char pathText[200];
    GfxResourceRequest *pRequest;
    int resourceTable;

    if (g_pBattle3dScene != 0 && g_BattleBlueFrogSongLoadedResTable == 0) {
        kind = 2;
        resourceTable = (int)&g_BattleBlueFrogSongResourceDefs;
        stage = (GfxModelTableBank *)g_pBattle3dScene->modelTableBank;
        pRequest = &requestSlot;
        Gfx_InitResourceRequest(pRequest);
        ((BattleFxFrogSongResource *)pRequest)->groupTag = 0;
        // STRING: FF7 0x008aae48
        ((BattleFxFrogSongResource *)pRequest)->resourceHandle = (int)BattleFx_BuildResourcePath("blue/kaeru", pathText);
        Gfx_LoadModelTableIfAbsent(kind, resourceTable, pRequest, &g_BattleBlueFrogSongLoadedResTable, stage);
    }
}

/* Entry point for Frog Song blue magic effect. */
// FUNCTION: FF7 0x00570060
void __cdecl BattleFx_BlueEnterFrogSong(int targetMask, int actorIdx)
{
    BattleFx_BlueLoadResFrogSong();
    BattleFx_BlueBeginFrogSong(targetMask, actorIdx);
}

/* Initializes spawn and aim tasks for Frog Song. */
// FUNCTION: FF7 0x0057007a
void __cdecl BattleFx_BlueBeginFrogSong(int targetMask, int actorIdx)
{
    int idx, count;

    g_BattleFxDrawListBufferTaskSlotPtr = (unsigned char *)BATTLE_NEW_TASK_SLOT(BattleFx_SelectDrawListBuffer);
    idx = 0;
    for (count = 0; idx < 10; idx++) {
        if ((targetMask & 1 << idx) != 0) {
            count++;
            if (count == 1) {
                g_BattleBlueFrogSongState.actorIndex = idx;
            }
        }
    }
    Battle_AllocateResourceSlot(BattleFx_BlueFrogSongSpawnTask);
    g_BattleBlueFrogSongState.nextNoteIndex = 0;
    Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputePanForEntity(targetMask), 0x9B);
}

/* Spawns note emission and ring tasks for Frog Song. */
// FUNCTION: FF7 0x00570120
void __cdecl BattleFx_BlueFrogSongSpawnTask(void)
{
    int rad;
    BattleFxFrogSongAimTask *aim;
    BattleFxFrogSongNoteSlot *task;

    task = (BattleFxFrogSongNoteSlot *)BATTLE_CURRENT_TASK_SLOT;
    task->ownerSlot = BATTLE_TASK_FREE;
    task = (BattleFxFrogSongNoteSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_BlueFrogSongNoteEmitTask);
    Battle_GetBoneWorldPos(g_BattleBlueFrogSongState.actorIndex, g_BattleActorRecords[g_BattleBlueFrogSongState.actorIndex].targetBone, &task->posX);
    rad = g_BattleActorRecords[g_BattleBlueFrogSongState.actorIndex].radius + 0xE0;
    if (rad > 1266) {
        rad = 0x4F2;
    }
    task->noteIndex = rad;
    task->noteOffset = task->noteIndex << 1;
    task->riseRange = -(task->posY << 1);
    task->scale = (rad << 12) / 450;
    aim = (BattleFxFrogSongAimTask *)BATTLE_NEW_TASK_SLOT(BattleFx_BlueFrogSongAimTask);
    aim->pos = task->pos;
    aim->scale = (rad << 12) / 1500;
    aim->aimValue = rad;
}

/* Emits musical notes towards target for Frog Song. */
// FUNCTION: FF7 0x00570244
void __cdecl BattleFx_BlueFrogSongNoteEmitTask(void)
{
    int noteIndex;
    int p; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int ringIndex;
    int bytesRand;
    BattleFxFrogSongNoteTask *drawTask;
    BattleFxFrogSongNoteEmitter *task;

    if (g_BattleIsPaused != 0) {
        return;
    }
    task = (BattleFxFrogSongNoteEmitter *)BATTLE_CURRENT_TASK_SLOT;
    if (task->frameCount > 17 && task->frameCount < 54) {
        ringIndex = g_BattleBlueFrogSongState.nextNoteIndex;
        drawTask = (BattleFxFrogSongNoteTask *)BATTLE_NEW_TASK_SLOT(BattleFx_BlueFrogSongDrawNotesTask);
        drawTask->ringIndex = ringIndex;
        for (noteIndex = 0; noteIndex < 4; noteIndex++) {
            bytesRand = (rand() % 4096) + 2048;
            g_BattleBlueFrogSongBillboardRing[ringIndex].posX = task->baseX + rand() % task->jitterRange - task->jitterOffset;
            g_BattleBlueFrogSongBillboardRing[ringIndex].riseY = -(rand() % task->riseRange);
            g_BattleBlueFrogSongBillboardRing[ringIndex].posZ = task->baseZ + rand() % task->jitterRange - task->jitterOffset;
            g_BattleBlueFrogSongBillboardRing[ringIndex].scale = task->scale * bytesRand >> 12;
            ringIndex++;
            if (ringIndex >= 222) {
                ringIndex = 0;
            }
        }
        g_BattleBlueFrogSongState.nextNoteIndex = ringIndex;
    }
    if (task->frameCount == 17) {
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputeScreenPanFromPosition(&task->pos), 0x134);
    }
    if (task->frameCount == 30) {
        Battle_PostActorHitOrHurtMessage(g_BattleBlueFrogSongState.actorIndex);
    }
    if (task->frameCount >= 60) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    (task->frameCount)++;
}

/* Renders musical note billboard sprites for Frog Song. */
// FUNCTION: FF7 0x00570417
void __cdecl BattleFx_BlueFrogSongDrawNotesTask(void)
{
    int noteIndex, ringIndex;
    BattleFxFrogSongNoteTask *task;

    task = (BattleFxFrogSongNoteTask *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleBlueFrogSongNoteSpriteReq.frameIndex = task->frameIndex;
    ringIndex = task->ringIndex;
    g_BattleBlueFrogSongResourceTable.noteSpriteHandle = Gfx_ModelTableGetLoadedRecord(0, g_BattleBlueFrogSongLoadedResTable);
    g_BattleBlueFrogSongResourceTable.noteSpriteAltHandle = Gfx_ModelTableGetSpriteObject(0, g_BattleBlueFrogSongLoadedResTable);
    for (noteIndex = 0; noteIndex < 4; noteIndex++) {
        BattleFx_InstallBillboardMatrix(&g_BattleBlueFrogSongBillboardRing[ringIndex].posX, g_BattleBlueFrogSongBillboardRing[ringIndex].scale, -(g_BattleBlueFrogSongBillboardRing[ringIndex].scale >> 2));
        g_BattleFxActiveDrawListPtr = BattleFx_SubmitSpriteDraw(&g_BattleBlueFrogSongResourceTable.noteSpriteHandle, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
        ringIndex++;
        if (ringIndex >= 222) {
            ringIndex = 0;
        }
    }
    if (task->frameIndex >= 11) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    (task->frameIndex)++;
}

/* Computes aiming direction and ring orientation for Frog Song. */
// FUNCTION: FF7 0x00570531
void __cdecl BattleFx_BlueFrogSongAimTask(void)
{
    int workAngle;
    int aimDir[4];
    VECTOR activeAxisVec;
    VECTOR deltaX;
    BattleFxFrogSongAimTask *aimer;
    BattleFxFrogSongAimState *pRingDraw;
    int e; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */

    aimer = (BattleFxFrogSongAimTask *)BATTLE_CURRENT_TASK_SLOT;
    deltaX.vx = g_BattleCameraPosition.cameraPosX - aimer->posX;
    deltaX.vy = 0;
    deltaX.vz = g_BattleCameraPosZ - aimer->posZ;
    NormalizeVectorFixed(&deltaX, aimDir);
    workAngle = ComputeAngleBetweenVectors((VECTOR *)&g_BattleBlueFrogSongRingSpriteReq.ringAxisX, (VECTOR *)aimDir, &activeAxisVec);
    Gte_BuildAxisAngleMatrix(workAngle, &g_BattleBlueFrogSongAimVector.mat, &activeAxisVec);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (aimer->frameCount >= 1 && aimer->frameCount <= 20) {
        pRingDraw = (BattleFxFrogSongAimState *)BATTLE_NEW_TASK_SLOT(BattleFx_BlueFrogSongRingDrawTask);
        pRingDraw->pos = aimer->pos;
        pRingDraw->scale = aimer->scale;
        pRingDraw->angle = 7 - (aimer->frameCount & 7) << 9;
        pRingDraw->angularVelocity = 96;
        pRingDraw->aimStep = aimer->aimValue >> 2;
        pRingDraw->ringPhase = pRingDraw->aimStep;
    }
    if (aimer->frameCount > 20) {
        aimer->ownerSlot = BATTLE_TASK_FREE;
    }
    (aimer->frameCount)++;
}

/* Renders expanding ring sprites for Frog Song. */
// FUNCTION: FF7 0x00570688
void __cdecl BattleFx_BlueFrogSongRingDrawTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[56];
        PsxMatrix rotMatrix;
        struct { unsigned char pad_to_20[0x20]; BattleFxFrogSongRingDrawSlot *slot; };
        struct { unsigned char pad_to_24[0x24]; SVECTOR vecX; };
        struct { unsigned char pad_to_2C[0x2C]; SVECTOR rotX; };
        struct { unsigned char pad_to_34[0x34]; char colorByte; };
    } frame;
#pragma pack(pop)

    frame.slot = (BattleFxFrogSongRingDrawSlot *)BATTLE_CURRENT_TASK_SLOT;
    frame.rotX.vx = 0;
    frame.rotX.vy = 0;
    frame.rotX.vz = frame.slot->angle;
    frame.vecX.vx = 0;
    frame.vecX.vy = -frame.slot->ringPhase;
    frame.vecX.vz = 0;
    RotMatrixXYZ(&frame.rotX, &frame.rotMatrix);
    TransformVectorToInt16(&frame.rotMatrix, &frame.vecX, &frame.vecX.vx);
    TransformVectorToInt16(&g_BattleBlueFrogSongAimVector.mat, &frame.vecX, &frame.vecX.vx);
    frame.vecX.vx += frame.slot->posX;
    frame.vecX.vy += frame.slot->posY;
    frame.vecX.vz += frame.slot->posZ;
    if (frame.slot->frameCounter >= 8) {
        frame.colorByte = 0x80 - (frame.slot->frameCounter - 8 << 5);
        g_BattleBlueFrogSongRingSpriteReq.blue = frame.colorByte;
        g_BattleBlueFrogSongRingSpriteReq.green = g_BattleBlueFrogSongRingSpriteReq.blue;
        g_BattleBlueFrogSongRingSpriteReq.red = g_BattleBlueFrogSongRingSpriteReq.green;
    }
    BattleFx_InstallBillboardMatrix(&frame.vecX, (short)frame.slot->scale, -(frame.slot->scale >> 2));
    g_BattleBlueFrogSongNoteSpriteReq.kaeruNoteSpriteHandle = Gfx_ModelTableGetLoadedRecord(1, g_BattleBlueFrogSongLoadedResTable);
    g_BattleBlueFrogSongNoteSpriteReq.kaeruNoteSpriteAltHandle = Gfx_ModelTableGetSpriteObject(1, g_BattleBlueFrogSongLoadedResTable);
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitSpriteDraw(&g_BattleBlueFrogSongNoteSpriteReq.kaeruNoteSpriteHandle, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (frame.slot->frameCounter >= 12) {
        frame.slot->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.slot->angle += frame.slot->angularVelocity;
    frame.slot->ringPhase += frame.slot->phaseStep;
    (frame.slot->frameCounter)++;
}
} /* extern "C" */
