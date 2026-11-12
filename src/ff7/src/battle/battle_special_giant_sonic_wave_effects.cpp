// src/ff7/src/battle/battle_special_giant_sonic_wave_effects.cpp
/* Enemy Special Attack [132]: Giant Sonic Wave (orig. SPECIAL/DAIONPA, 大音波) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle special effect for the Daionpa spell. It loads its resources, starts the
//   timeline, and runs the variant sprite, growing and spinning model, and spin-fade sprite tasks.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/api/battle.h"
#include "ff7/battle_tasks.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_actors.h"
#include "ff7/camera_script.h"
#include "lib/types/graphics.h"
#include <string.h>
#include <stdlib.h>
#include "lib/codegen.h"
extern "C" {

struct BattleFxModelDraw;
// clang-format off
// GLOBAL: FF7 0x0089ac00
int g_BattleSpecGiantSonicWaveResourceDef = 0x001900B1;
int g_BattleSpecGiantSonicWaveResDefPath0[1] = {
    (int)((char *)&g_BattleSpecGiantSonicWaveResPaths0 + 0x0)
};
int g_BattleSpecGiantSonicWaveResDefEntries[74] = {
    0x0, 0x0, 0x0, 0x1940b1, (int)((char *)&g_BattleSpecGiantSonicWaveResPaths0 + 0xc), 0x0,
    0x0, 0x0, 0x192a39, (int)((char *)&g_BattleSpecGiantSonicWaveResPaths0 + 0x18), (int)((char *)&g_BattleSpecGiantSonicWaveResPaths1 + 0x4), 0x0,
    0x0, 0x192a39, (int)((char *)&g_BattleSpecGiantSonicWaveResPaths1 + 0x10), (int)((char *)&g_BattleSpecGiantSonicWaveResPaths1 + 0x1c), 0x0, 0x0,
    0x192a39, (int)((char *)&g_BattleSpecGiantSonicWaveResPaths1 + 0x28), (int)((char *)&g_BattleSpecGiantSonicWaveResPaths1 + 0x34), 0x0, 0x0, 0x192a39,
    (int)((char *)&g_BattleSpecGiantSonicWaveResPaths1 + 0x40), (int)((char *)&g_BattleSpecGiantSonicWaveResPaths1 + 0x4c), 0x0, 0x0, 0x192a39, (int)((char *)&g_BattleSpecGiantSonicWaveResPaths1 + 0x58),
    (int)((char *)&g_BattleSpecGiantSonicWaveResPaths1 + 0x64), 0x0, 0x0, 0x192a39, (int)((char *)&g_BattleSpecGiantSonicWaveResPaths1 + 0x70), (int)((char *)&g_BattleSpecGiantSonicWaveResPaths1 + 0x7c),
    0x0, 0x0, 0x192a39, (int)((char *)&g_BattleSpecGiantSonicWaveResPaths1 + 0x88), (int)((char *)&g_BattleSpecGiantSonicWaveResPaths1 + 0x94), 0x0,
    0x0, 0x192a39, (int)((char *)&g_BattleSpecGiantSonicWaveResPaths1 + 0xa0), (int)((char *)&g_BattleSpecGiantSonicWaveResPaths1 + 0xac), 0x0, 0x0,
    0x192a39, (int)((char *)&g_BattleSpecGiantSonicWaveResPaths1 + 0xb8), (int)((char *)&g_BattleSpecGiantSonicWaveResPaths1 + 0xc4), 0x0, 0x0, 0x192a39,
    (int)((char *)&g_BattleSpecGiantSonicWaveResPaths1 + 0xd0), (int)((char *)&g_BattleSpecGiantSonicWaveResPaths1 + 0xdc), 0x0, 0x0, 0x192a39, (int)((char *)&g_BattleSpecGiantSonicWaveResPaths1 + 0xe8),
    (int)((char *)&g_BattleSpecGiantSonicWaveResPaths1 + 0xf4), 0x0, 0x0, 0x192a39, (int)((char *)&g_BattleSpecGiantSonicWaveResPaths1 + 0x100), (int)((char *)&g_BattleSpecGiantSonicWaveResPaths1 + 0x10c),
    0x0, 0x0, 0x192a39, (int)((char *)&g_BattleSpecGiantSonicWaveResPaths1 + 0x118), (int)((char *)&g_BattleSpecGiantSonicWaveResPaths1 + 0x124), 0x0,
    0x0, 0x0
};
// GLOBAL: FF7 0x0089ad30
struct BattleFxSpriteRequest g_BattleSpecGiantSonicWaveSpriteRequest = {0, 0, 0x80, 0x80, 0x80, 0x2e, 0, 0};
// GLOBAL: FF7 0x0089ad40
struct BattleSpecGiantSonicWaveAnimData1 g_BattleSpecGiantSonicWaveAnimData1 = {0, {0xa8, 0, 0, 0, 0, 0}, 128};
unsigned char g_rawData_0089ad4c[4] = {0x20, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x0089ad50
struct SVECTOR g_BattleSpecGiantSonicWaveGrowAngles = {-1024, 0, 0, 0};
// GLOBAL: FF7 0x0089ad58
int g_BattleSpecGiantSonicWaveSpinScaleX = 6144;
// GLOBAL: FF7 0x0089ad5c
int g_BattleSpecGiantSonicWaveSpinScaleY = 6144;
// GLOBAL: FF7 0x0089ad60
struct BattleSpecGiantSonicWaveAnimData2 g_BattleSpecGiantSonicWaveAnimData2 = {{0, 0x18, 0, 0, 0, 0, 0, 0}, 0, {0xa8, 0, 0, 0, 0, 0}, 0, {0x20, 0, 0, 0}, 0, 0};
// GLOBAL: FF7 0x0089ad80
struct BattleSpecHeartBreathSpriteReq1 g_BattleSpecGiantSonicWaveColorData = {-128, -128, -128, {0x2e, 0, 0, 0, 0}};
unsigned char g_BattleSpecGiantSonicWaveResPaths0[28] = {
    0x48, 0x49, 0x4b, 0x41, 0x52, 0x49, 0x2e, 0x52, 0x53, 0x44, 0x00, 0x00, 0x4e, 0x49, 0x5a, 0x49, 0x2e, 0x52, 0x53, 0x44, 0x00, 0x00, 0x00, 0x00,
    0x4a, 0x41, 0x4d, 0x30
};
unsigned char g_rawData_0089ada4[4] = {0x30, 0x2e, 0x54, 0x49};
unsigned char g_BattleSpecGiantSonicWaveResPaths1[304] = {
    0x4d, 0x00, 0x00, 0x00, 0x4f, 0x4e, 0x50, 0x41, 0x31, 0x2e, 0x53, 0x50, 0x54, 0x00, 0x00, 0x00, 0x4a, 0x41, 0x4d, 0x30, 0x30, 0x2e, 0x54, 0x49,
    0x4d, 0x00, 0x00, 0x00, 0x4f, 0x4e, 0x50, 0x41, 0x32, 0x2e, 0x53, 0x50, 0x54, 0x00, 0x00, 0x00, 0x4a, 0x41, 0x4d, 0x30, 0x30, 0x2e, 0x54, 0x49,
    0x4d, 0x00, 0x00, 0x00, 0x4f, 0x4e, 0x50, 0x41, 0x33, 0x2e, 0x53, 0x50, 0x54, 0x00, 0x00, 0x00, 0x4a, 0x41, 0x4d, 0x30, 0x30, 0x2e, 0x54, 0x49,
    0x4d, 0x00, 0x00, 0x00, 0x4f, 0x4e, 0x50, 0x41, 0x34, 0x2e, 0x53, 0x50, 0x54, 0x00, 0x00, 0x00, 0x4a, 0x41, 0x4d, 0x30, 0x30, 0x2e, 0x54, 0x49,
    0x4d, 0x00, 0x00, 0x00, 0x4f, 0x4e, 0x50, 0x41, 0x35, 0x2e, 0x53, 0x50, 0x54, 0x00, 0x00, 0x00, 0x4a, 0x41, 0x4d, 0x30, 0x30, 0x2e, 0x54, 0x49,
    0x4d, 0x00, 0x00, 0x00, 0x4f, 0x4e, 0x50, 0x41, 0x36, 0x2e, 0x53, 0x50, 0x54, 0x00, 0x00, 0x00, 0x4a, 0x41, 0x4d, 0x30, 0x30, 0x2e, 0x54, 0x49,
    0x4d, 0x00, 0x00, 0x00, 0x4f, 0x4e, 0x50, 0x41, 0x37, 0x2e, 0x53, 0x50, 0x54, 0x00, 0x00, 0x00, 0x4a, 0x41, 0x4d, 0x30, 0x30, 0x2e, 0x54, 0x49,
    0x4d, 0x00, 0x00, 0x00, 0x48, 0x49, 0x4b, 0x41, 0x52, 0x49, 0x31, 0x2e, 0x53, 0x50, 0x54, 0x00, 0x4a, 0x41, 0x4d, 0x30, 0x30, 0x2e, 0x54, 0x49,
    0x4d, 0x00, 0x00, 0x00, 0x48, 0x49, 0x4b, 0x41, 0x52, 0x49, 0x32, 0x2e, 0x53, 0x50, 0x54, 0x00, 0x4a, 0x41, 0x4d, 0x30, 0x30, 0x2e, 0x54, 0x49,
    0x4d, 0x00, 0x00, 0x00, 0x48, 0x49, 0x4b, 0x41, 0x52, 0x49, 0x33, 0x2e, 0x53, 0x50, 0x54, 0x00, 0x4a, 0x41, 0x4d, 0x30, 0x31, 0x2e, 0x54, 0x49,
    0x4d, 0x00, 0x00, 0x00, 0x42, 0x41, 0x4b, 0x55, 0x31, 0x2e, 0x53, 0x50, 0x54, 0x00, 0x00, 0x00, 0x4a, 0x41, 0x4d, 0x30, 0x31, 0x2e, 0x54, 0x49,
    0x4d, 0x00, 0x00, 0x00, 0x42, 0x41, 0x4b, 0x55, 0x32, 0x2e, 0x53, 0x50, 0x54, 0x00, 0x00, 0x00, 0x4a, 0x41, 0x4d, 0x30, 0x31, 0x2e, 0x54, 0x49,
    0x4d, 0x00, 0x00, 0x00, 0x42, 0x41, 0x4b, 0x55, 0x33, 0x2e, 0x53, 0x50, 0x54, 0x00, 0x00, 0x00
};
// clang-format on
#pragma intrinsic(memset)
#pragma intrinsic(memcpy)


// clang-format off
// GLOBAL: FF7 0x00b5a840
int g_BattleSpecGiantSonicWaveDrawHandle = 0;
// GLOBAL: FF7 0x00b5a844
int g_BattleSpecGiantSonicWaveTargetMask = 0;
// GLOBAL: FF7 0x00b5a848
struct GfxTableCursor *g_BattleSpecGiantSonicWaveResTable = 0;
// GLOBAL: FF7 0x00b5a84c
short g_BattleSpecGiantSonicWaveFade = 0;
// clang-format on

/*
 * BattleFx_SpecialLoadResGiantSonicWave
 * When the engine context exists and g_BattleSpecGiantSonicWaveResTable is empty, builds SPECIAL/DAIONPA with a default-
 * duration record and creates resource type 15 using g_BattleSpecGiantSonicWaveResourceDef.
 */
// FUNCTION: FF7 0x0051a500
void __cdecl BattleFx_SpecialLoadResGiantSonicWave(void)
{
    GfxModelTableBank *ownerHandle;
    GfxResourceRequest recBlock;
    int kind;
    char pathDstBuf[200];
    GfxResourceRequest *pRequest;
    int resourceDefAddr;

    if (g_pBattle3dScene != 0 && g_BattleSpecGiantSonicWaveResTable == 0) {
        kind = 0xF;
        resourceDefAddr = (int)&g_BattleSpecGiantSonicWaveResourceDef;
        ownerHandle = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRequest = &recBlock;
        Gfx_InitResourceRequest(pRequest);
        pRequest->groupTag = 0;
        // STRING: FF7 0x0089aed8
        pRequest->resourcePath = BattleFx_BuildResourcePath("SPECIAL/DAIONPA", pathDstBuf);
        Gfx_LoadModelTableIfAbsent(kind, resourceDefAddr, pRequest, &g_BattleSpecGiantSonicWaveResTable, ownerHandle);
    }
}

/*
 * BattleFx_SpecialEnterGiantSonicWave
 * Entry point for the Daionpa special effect: loads its resources via
 * BattleFx_SpecialLoadResGiantSonicWave, then forwards actorMask and sourceIndex to BattleFx_SpecialGiantSonicWaveSpawnTimeline. It touches no
 * globals of its own.
 * Loads Daionpa resources and calls BattleFx_SpecialGiantSonicWaveSpawnTimeline with both arguments.
 */
// FUNCTION: FF7 0x0051a590
void __cdecl BattleFx_SpecialEnterGiantSonicWave(int targetMask, int actorIdx)
{
    BattleFx_SpecialLoadResGiantSonicWave();
    BattleFx_SpecialGiantSonicWaveSpawnTimeline(targetMask, actorIdx);
}

/*
 * BattleFx_SpecialGiantSonicWaveSpawnTimeline
 * Stores actorMask, allocates BattleFx_SpecialGiantSonicWaveTimelineTask with the sourceIndex actor bone selected by g_BattleActorRecords[sourceIndex].bone33 and actorMask
 * actor-mask centroid, and dispatches (0x118, narrowed sourceIndex, 0).
 */
// FUNCTION: FF7 0x0051a5aa
void __cdecl BattleFx_SpecialGiantSonicWaveSpawnTimeline(int actorMask, int sourceIndex)
{
    int effectSlot;

    g_BattleSpecGiantSonicWaveTargetMask = actorMask;
    effectSlot = (int)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialGiantSonicWaveTimelineTask);
    Battle_GetBoneWorldPos(sourceIndex, g_BattleActorRecords[sourceIndex].bone33, (short *)(effectSlot + 4));
    Battle_GetActorMaskCentroid(actorMask, (short *)(effectSlot + 0xC));
    Battle_PlaySfxPannedToActor(0x118, (short)sourceIndex, 0);
}

/*
 * Timeline task that orchestrates sonic wave rings, camera shake, and hit reactions for Daionpa.
 */
// FUNCTION: FF7 0x0051a621
void __cdecl BattleFx_SpecialGiantSonicWaveTimelineTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[80];
        SVECTOR rotX;
        struct { unsigned char pad_to_8[0x8]; int transformedPos; };
        struct { unsigned char pad_to_C[0xC]; SVECTOR offsetX; };
        struct { unsigned char pad_to_14[0x14]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_34[0x34]; BattleFxIcicleDropDebrisState *spawnSlot; };
        struct { unsigned char pad_to_38[0x38]; short innerIndex; };
        struct { unsigned char pad_to_40[0x40]; short loopIndex; };
        struct { unsigned char pad_to_44[0x44]; short randomAngle; };
        struct { unsigned char pad_to_48[0x48]; BattleFxGiantSonicWaveTimelineSlot *timelineSlot; };
        struct { unsigned char pad_to_4C[0x4C]; short tmpAngle; };
    } frame;
#pragma pack(pop)

    frame.timelineSlot = (BattleFxGiantSonicWaveTimelineSlot *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.timelineSlot->frameCounter < 20) {
        for (frame.loopIndex = 0; frame.loopIndex < 2; frame.loopIndex++) {
            frame.spawnSlot = (BattleFxIcicleDropDebrisState *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialGiantSonicWaveSpinFadeSpriteTask);
            frame.spawnSlot->posX = frame.timelineSlot->posX;
            frame.spawnSlot->posY = frame.timelineSlot->posY;
            frame.spawnSlot->posZ = frame.timelineSlot->posZ;
            frame.spawnSlot->velocityX = (frame.timelineSlot->targetX - frame.timelineSlot->posX) / 15;
            frame.spawnSlot->velocityY = (frame.timelineSlot->targetY - frame.timelineSlot->posY) / 15;
            frame.spawnSlot->velocityZ = (frame.timelineSlot->targetZ - frame.timelineSlot->posZ) / 15;
            frame.randomAngle = (rand() % 4096);
            frame.offsetX.vx = CosFixed(frame.randomAngle) * 0x64 >> 12;
            frame.offsetX.vy = SinFixed(frame.randomAngle) * 0x64 >> 12;
            frame.offsetX.vz = 0;
            frame.rotX.vz = 0;
            frame.rotX.vx = frame.rotX.vz;
            frame.spawnSlot->angle = ArcTangentFixed(frame.timelineSlot->targetX - frame.timelineSlot->posX, frame.timelineSlot->targetZ - frame.timelineSlot->posZ);
            frame.rotX.vy = frame.spawnSlot->angle;
            RotMatrix(&frame.rotX, &frame.rotMatrix);
            frame.rotMatrix.t[0] = frame.spawnSlot->velocityX;
            frame.rotMatrix.t[1] = frame.spawnSlot->velocityY;
            frame.rotMatrix.t[2] = frame.spawnSlot->velocityZ;
            SetRotMatrix(&frame.rotMatrix);
            SetTransMatrix(&frame.rotMatrix);
            RotTransPointToInt16(&frame.offsetX, (short *)((char *)frame.spawnSlot + 0xC), &frame.transformedPos);
            frame.spawnSlot->variant = rand() % 7;
        }
    }
    if (frame.timelineSlot->frameCounter == 30) {
        frame.spawnSlot = (BattleFxIcicleDropDebrisState *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialGiantSonicWaveSpinGrowingModelTask);
        Battle_StartFixedPointRamp(0x1000, 10);
        g_BattleFxTimelineState.red = 0;
        g_BattleFxTimelineState.channel1 = 0;
        g_BattleFxTimelineState.channel2 = 0x20;
        Battle_PlaySfxPannedToActor(0xF6, -1, 0x40);
    }
    if (frame.timelineSlot->frameCounter == 54) {
        for (frame.loopIndex = 0; frame.loopIndex < 10; frame.loopIndex++) {
            if ((g_BattleSpecGiantSonicWaveTargetMask & 1 << frame.loopIndex) != 0) {
                frame.spawnSlot = (BattleFxIcicleDropDebrisState *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialGiantSonicWaveGrowingModelTask);
                Battle_GetBoneWorldPos(frame.loopIndex, g_BattleActorRecords[frame.loopIndex].targetBone, (short *)((char *)frame.spawnSlot + 4));
            }
        }
        Battle_PlaySfxPannedToActor(0x189, -1, 0x40);
    }
    if (frame.timelineSlot->frameCounter > 56 && frame.timelineSlot->frameCounter < 64) {
        for (frame.loopIndex = 0; frame.loopIndex < 10; frame.loopIndex++) {
            if ((g_BattleSpecGiantSonicWaveTargetMask & 1 << frame.loopIndex) != 0) {
                for (frame.innerIndex = 0; frame.innerIndex < 2; frame.innerIndex++) {
                    frame.spawnSlot = (BattleFxIcicleDropDebrisState *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialGiantSonicWaveVariantSpriteTask);
                    Battle_GetBoneWorldPos(frame.loopIndex, g_BattleActorRecords[frame.loopIndex].targetBone, (short *)((char *)frame.spawnSlot + 4));
                    frame.spawnSlot->posX += (short)(rand() % 1200 - 600);
                    frame.spawnSlot->posY += (short)(rand() % 1200 - 600);
                    frame.spawnSlot->posZ += (short)(rand() % 1200 - 600);
                    frame.spawnSlot->variant = rand() % 6;
                    frame.spawnSlot->spinAngle = (rand() % 3 << 11) + 0x1000;
                    frame.tmpAngle = (rand() % 4096);
                    frame.offsetX.vx = CosFixed(frame.tmpAngle) * 0x3C >> 12;
                    frame.offsetX.vy = SinFixed(frame.tmpAngle) * 0x3C >> 12;
                    frame.offsetX.vz = 0;
                    frame.rotX.vz = 0;
                    frame.rotX.vx = frame.rotX.vz;
                    frame.rotX.vy = (rand() % 4096);
                    RotMatrix(&frame.rotX, &frame.rotMatrix);
                    frame.rotMatrix.t[2] = 0;
                    frame.rotMatrix.t[1] = frame.rotMatrix.t[2];
                    frame.rotMatrix.t[0] = frame.rotMatrix.t[1];
                    SetRotMatrix(&frame.rotMatrix);
                    SetTransMatrix(&frame.rotMatrix);
                    RotTransPointToInt16(&frame.offsetX, (short *)((char *)frame.spawnSlot + 0xC), &frame.transformedPos);
                }
            }
        }
    }
    if (frame.timelineSlot->frameCounter == 60) {
        for (frame.loopIndex = 0; frame.loopIndex < 10; frame.loopIndex++) {
            if ((g_BattleSpecGiantSonicWaveTargetMask & 1 << frame.loopIndex) != 0) {
                Battle_PostActorHitOrHurtMessage(frame.loopIndex);
            }
        }
    }
    (frame.timelineSlot->frameCounter)++;
    if (frame.timelineSlot->frameCounter == 70) {
        Battle_StartFixedPointRamp(0, 10);
        frame.timelineSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * BattleFx_SpecialGiantSonicWaveVariantSpriteTask
 * Selects sprite field nine through fourteen using the slot selector, installs its billboard at
 * scale 0x1800, publishes the counter and draws before checking pause. When unpaused integrates
 * XYZ even with a null sprite; only with a nonnull sprite advances the counter and retires at or
 * above its header frame count.
 */
// FUNCTION: FF7 0x0051abc8
void __cdecl BattleFx_SpecialGiantSonicWaveVariantSpriteTask(void)
{
    struct {
        int table[6];
        BattleFxJammerVariantSprite *slot;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    frame.slot = (BattleFxJammerVariantSprite *)BATTLE_CURRENT_TASK_SLOT;
    frame.table[0] = 9;
    frame.table[1] = 0xA;
    frame.table[2] = 0xB;
    frame.table[3] = 0xC;
    frame.table[4] = 0xD;
    frame.table[5] = 0xE;
    BattleFx_InstallBillboardMatrix((char *)frame.slot + 4, 0x1800 /* 1.5 */, 0);
    g_BattleSpecGiantSonicWaveSpriteRequest.frame = frame.slot->frameIndex;
    g_BattleSpecGiantSonicWaveSpriteRequest.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(frame.table[frame.slot->variant], g_BattleSpecGiantSonicWaveResTable);
    g_BattleSpecGiantSonicWaveSpriteRequest.drawObj = Gfx_ModelTableGetSpriteObject(frame.table[frame.slot->variant], g_BattleSpecGiantSonicWaveResTable);
    g_BattleSpecGiantSonicWaveDrawHandle = BattleFx_SubmitSpriteDraw(&g_BattleSpecGiantSonicWaveSpriteRequest, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecGiantSonicWaveDrawHandle);
    if (g_BattleIsPaused == 0) {
        frame.slot->posA += frame.slot->velA;
        frame.slot->posB += frame.slot->velB;
        frame.slot->posC += frame.slot->velC;
        if (g_BattleSpecGiantSonicWaveSpriteRequest.spriteGroup != 0) {
            (frame.slot->frameIndex)++;
            if (frame.slot->frameIndex >= g_BattleSpecGiantSonicWaveSpriteRequest.spriteGroup->header->frameCount) {
                frame.slot->ownerSlot = 0xFFFF;
            }
        }
    }
}

/*
 * Expands and submits the three-layered Daionpa sonic wave cone model.
 */
// FUNCTION: FF7 0x0051ad17
void __cdecl BattleFx_SpecialGiantSonicWaveGrowingModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[92];
        int projDepth;
        struct { unsigned char pad_to_4[0x4]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_28[0x28]; PsxMatrix growMatrixA; };
        struct { unsigned char pad_to_48[0x48]; short loopIndex; };
        struct { unsigned char pad_to_50[0x50]; BattleTaskSlot *growingModel; };
    } frame;
#pragma pack(pop)

    frame.growingModel = BATTLE_CURRENT_TASK_SLOT;
    (g_BattleSpecGiantSonicWaveAnimData1.modelGroup) = Gfx_ModelTableGetRenderObject(0, g_BattleSpecGiantSonicWaveResTable);
    for (frame.loopIndex = 0; frame.loopIndex < 3; frame.loopIndex++) {
        frame.growMatrixA.m[2][frame.loopIndex] = 0;
        frame.growMatrixA.m[1][frame.loopIndex] = 0;
        frame.growMatrixA.m[0][frame.loopIndex] = 0;
    }
    SetRotMatrix(&g_BattleCameraTransform);
    SetTransMatrix(&g_BattleCameraTransform);
    RotTransPoint(frame.growingModel->data, frame.growMatrixA.t, &frame.projDepth);
    frame.growMatrixA.m[2][2] = frame.growingModel->frameCounter * 600 + 0x800;
    frame.growMatrixA.m[1][1] = frame.growMatrixA.m[2][2];
    frame.growMatrixA.m[0][0] = frame.growMatrixA.m[1][1];
    RotMatrix(&g_BattleSpecGiantSonicWaveGrowAngles, &frame.rotMatrix);
    ConcatMatrixIntoFirst(&frame.growMatrixA, &frame.rotMatrix);
    SetRotMatrix(&frame.growMatrixA);
    SetTransMatrix(&frame.growMatrixA);
    SetDrawColorRGB(0, 0, 0);
    g_BattleSpecGiantSonicWaveAnimData1.alpha = frame.growingModel->frameCounter << 8;
    g_BattleSpecGiantSonicWaveDrawHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecGiantSonicWaveAnimData1, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecGiantSonicWaveDrawHandle);
    if (g_BattleIsPaused != 0) {
        return;
    }
    (frame.growingModel->frameCounter)++;
    if (frame.growingModel->frameCounter == 15) {
        frame.growingModel->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Spins and stretches the secondary Daionpa shock-ring model along its forward axis.
 */
// FUNCTION: FF7 0x0051ae91
void __cdecl BattleFx_SpecialGiantSonicWaveSpinGrowingModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[88];
        SVECTOR rotX;
        struct { unsigned char pad_to_8[0x8]; PsxMatrix spinMatrix; };
        struct { unsigned char pad_to_2C[0x2C]; BattleTaskSlot *spinSlot; };
    } frame;
#pragma pack(pop)

    frame.spinSlot = BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecGiantSonicWaveAnimData2.modelRecord = Gfx_ModelTableGetRenderObject(1, g_BattleSpecGiantSonicWaveResTable);
    g_BattleSpecGiantSonicWaveSpinScaleY = frame.spinSlot->frameCounter * 0x12C + 0x2000;
    frame.rotX.vx = 0x420;
    frame.rotX.vy = frame.spinSlot->frameCounter << 5;
    frame.rotX.vz = 0;
    RotMatrixXYZ(&frame.rotX, &frame.spinMatrix);
    ScaleMatrix(&frame.spinMatrix, &g_BattleSpecGiantSonicWaveSpinScaleX);
    frame.spinMatrix.t[0] = 0;
    frame.spinMatrix.t[1] = 0xFFFFFE70;
    frame.spinMatrix.t[2] = 0x1F40;
    SetRotMatrix(&frame.spinMatrix);
    SetTransMatrix(&frame.spinMatrix);
    SetDrawColorRGB(0, 0, 0);
    g_BattleSpecGiantSonicWaveAnimData2.alpha = 0x1000 - (g_BattleSpecGiantSonicWaveFade << 8);
    g_BattleSpecGiantSonicWaveDrawHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecGiantSonicWaveAnimData2.modelRecord, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecGiantSonicWaveDrawHandle);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.spinSlot->frameCounter < 8) {
        g_BattleSpecGiantSonicWaveFade++;
    }
    if (frame.spinSlot->frameCounter > 22) {
        g_BattleSpecGiantSonicWaveFade--;
    }
    (frame.spinSlot->frameCounter)++;
    if (frame.spinSlot->frameCounter == 30) {
        frame.spinSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Rotates and fades an expanding sonic-ring billboard sprite for Daionpa.
 */
// FUNCTION: FF7 0x0051aff7
void __cdecl BattleFx_SpecialGiantSonicWaveSpinFadeSpriteTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[120];
        int fadeLevel;
        struct { unsigned char pad_to_4[0x4]; SVECTOR rotAngleX; };
        struct { unsigned char pad_to_C[0xC]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_2C[0x2C]; short fadeLevelCopy; };
        struct { unsigned char pad_to_30[0x30]; PsxMatrix matrixBase; };
        struct { unsigned char pad_to_50[0x50]; int recordIds[7]; };
        struct { unsigned char pad_to_6C[0x6C]; short axisIndex; };
        struct { unsigned char pad_to_70[0x70]; int transDepth; };
        struct { unsigned char pad_to_74[0x74]; BattleFxGiantSonicWaveSpinFadeSpriteSlot *spriteSlot; };
    } frame;
#pragma pack(pop)

    frame.spriteSlot = (BattleFxGiantSonicWaveSpinFadeSpriteSlot *)BATTLE_CURRENT_TASK_SLOT;
    frame.recordIds[0] = 2;
    frame.recordIds[1] = 3;
    frame.recordIds[2] = 4;
    frame.recordIds[3] = 5;
    frame.recordIds[4] = 6;
    frame.recordIds[5] = 7;
    frame.recordIds[6] = 8;
    frame.rotAngleX.vy = 0;
    frame.rotAngleX.vx = 0;
    frame.rotAngleX.vz = frame.spriteSlot->frameCounter << 8;
    for (frame.axisIndex = 0; frame.axisIndex < 3; frame.axisIndex++) {
        frame.matrixBase.m[frame.axisIndex][2] = 0;
        frame.matrixBase.m[frame.axisIndex][1] = 0;
        frame.matrixBase.m[frame.axisIndex][0] = 0;
    }
    SetRotMatrix(&g_BattleCameraTransform);
    SetTransMatrix(&g_BattleCameraTransform);
    RotTransPoint((char *)frame.spriteSlot + 4, frame.matrixBase.t, &frame.transDepth);
    frame.matrixBase.m[2][2] = 0x1400;
    frame.matrixBase.m[1][1] = frame.matrixBase.m[2][2];
    frame.matrixBase.m[0][0] = frame.matrixBase.m[1][1];
    RotMatrix(&frame.rotAngleX, &frame.rotMatrix);
    ConcatMatrixIntoFirst(&frame.matrixBase, &frame.rotMatrix);
    SetRotMatrix(&frame.matrixBase);
    SetTransMatrix(&frame.matrixBase);
    if (frame.spriteSlot->frameCounter < 12) {
        frame.fadeLevel = 8;
    } else {
        frame.fadeLevel = 20 - frame.spriteSlot->frameCounter;
    }
    frame.fadeLevelCopy = *(short *)&frame.fadeLevel;
    g_BattleSpecGiantSonicWaveAnimData2.spriteRecord = Gfx_ModelTableGetLoadedRecord(frame.recordIds[frame.spriteSlot->recordIndex], g_BattleSpecGiantSonicWaveResTable);
    g_BattleSpecGiantSonicWaveAnimData2.drawObj = Gfx_ModelTableGetSpriteObject(frame.recordIds[frame.spriteSlot->recordIndex], g_BattleSpecGiantSonicWaveResTable);
    g_BattleSpecGiantSonicWaveColorData.blue = frame.fadeLevelCopy << 4;
    g_BattleSpecGiantSonicWaveColorData.green = g_BattleSpecGiantSonicWaveColorData.blue;
    (g_BattleSpecGiantSonicWaveColorData.red) = g_BattleSpecGiantSonicWaveColorData.green;
    g_BattleSpecGiantSonicWaveDrawHandle = BattleFx_SubmitSpriteDraw(&g_BattleSpecGiantSonicWaveAnimData2.spriteRecord, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecGiantSonicWaveDrawHandle);
    if (g_BattleIsPaused == 0) {
        frame.spriteSlot->posX += frame.spriteSlot->velocityX;
        frame.spriteSlot->posY += frame.spriteSlot->velocityY;
        frame.spriteSlot->posZ += frame.spriteSlot->velocityZ;
        (frame.spriteSlot->frameCounter)++;
        if (frame.spriteSlot->frameCounter == 20) {
            frame.spriteSlot->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}
} /* extern "C" */
