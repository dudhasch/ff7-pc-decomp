// src/ff7/src/battle/battle_fx_special_wing_scales_effect.cpp
/* Enemy Special Attack [111]: Wing Scales (orig. SPECIAL/KONA, 粉/鱗粉) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle special effect 'Kona': a screen flash, a per-target timeline and impact model,
//   and a particle-pair pool whose records are claimed and drawn each frame.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/api/battle.h"
#include "ff7/battle_tasks.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_special.h"
#include "ff7/camera_script.h"
#include "lib/types/graphics.h"
#include <string.h>
#include <stdlib.h>
#include "lib/codegen.h"
extern "C" {

struct BattleFxModelDraw;
// clang-format off
// GLOBAL: FF7 0x0089d788
int g_BattleSpecWingScalesResourceDefs = 0x001900B4;
char *g_BattleFxSpecialWingScalesEffectResourceNames[1] = {
    // STRING: FF7 0x0089d838
    "HIKARI.RSD"
};
unsigned int g_BattleSpecWingScalesResourceDefsDataTail[24] = {
    0x00000000,
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x0089d844
    (unsigned int)"DOKU00.TIM",
    // STRING: FF7 0x0089d850
    (unsigned int)"DOKU1.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    0x00000000,
    // STRING: FF7 0x0089d85c
    (unsigned int)"JO_A.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x0089d868
    (unsigned int)"DOKU02.TIM",
    // STRING: FF7 0x0089d874
    (unsigned int)"PINK.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x0089d880
    (unsigned int)"DOKU01.TIM",
    // STRING: FF7 0x0089d88c
    (unsigned int)"BAKU1.SPT",
    0x00000000,
    0x00000000,
    0x00000000
};
// GLOBAL: FF7 0x0089d7f0
struct BattleSpecWingScalesSpriteReq1 g_BattleSpecWingScalesSpriteReq1 = {0, 0, {0x80, 0x80, 0x80, 0x2c}, 0, {0}, 0, {0xa8, 0, 0, 0, 0, 0}, 128};
unsigned char g_rawData_0089d80c[4] = {0x20, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x0089d810
struct BattleSpecWingScalesSpriteReq2 g_BattleSpecWingScalesSpriteReq2 = {{0, 0xfc, 0, 0, 0, 0, 0, 0}, 0, 0, {0x80, 0x80, 0x80, 0x2c}, 0, {0}, 0, 0, {0x80, 0x80, 0x80, 0x2c}, 0, {0}};
// clang-format on
#pragma intrinsic(memset)
#pragma intrinsic(memcpy)


// clang-format off
// GLOBAL: FF7 0x00b5b0f8
int g_BattleSpecWingScalesDrawListHead = 0;
// GLOBAL: FF7 0x00b5b0fc
short g_BattleSpecWingScalesParticleLoopIndex = 0;
// GLOBAL: FF7 0x00b5b100
BattleFxWingScalesParticlePair g_BattleSpecWingScalesParticlePairSlots[150] = {0};
// GLOBAL: FF7 0x00b5c3c0
short g_BattleSpecWingScalesFlashIntensity = 0;
// GLOBAL: FF7 0x00b5c3c4
struct GfxTableCursor *g_BattleSpecWingScalesLoadedResTable = 0;
// GLOBAL: FF7 0x00b5c3c8
short g_BattleSpecWingScalesParticleActiveFlag = 0;
unsigned char g_bssPad_00b5c3cc[4] = {0};
unsigned char g_bssPad_00b5c3d0[16] = {0};
// clang-format on

/*
 * Clears g_BattleSpecWingScalesFlashIntensity on every call; when the engine context exists and g_BattleSpecWingScalesLoadedResTable is empty,
 * builds SPECIAL/KONA with a default-duration record and creates resource type five using
 * g_BattleSpecWingScalesResourceDefs.
 */
// FUNCTION: FF7 0x0051f5f0
void __cdecl BattleFx_SpecialLoadResWingScales(void)
{
    GfxModelTableBank *workSceneCtx;
    GfxResourceRequest reqHandle;
    int kind;
    char pathCap[200];
    GfxResourceRequest *pRequest;
    int resDefStorage;

    g_BattleSpecWingScalesFlashIntensity = 0;
    if (g_pBattle3dScene != 0 && g_BattleSpecWingScalesLoadedResTable == 0) {
        kind = 5;
        resDefStorage = (int)&g_BattleSpecWingScalesResourceDefs;
        workSceneCtx = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRequest = &reqHandle;
        Gfx_InitResourceRequest(pRequest);
        pRequest->groupTag = 0;
        // STRING: FF7 0x0089d898
        pRequest->resourcePath = BattleFx_BuildResourcePath("SPECIAL/KONA", pathCap);
        Gfx_LoadModelTableIfAbsent(kind, resDefStorage, pRequest, &g_BattleSpecWingScalesLoadedResTable, workSceneCtx);
    }
}

/*
 * Clears g_BattleSpecWingScalesParticleActiveFlag, the completion flag its per-frame updater BattleFx_SpecialWingScalesParticlePairPoolTask tests, then calls
 * BattleFx_SpecialBeginWingScales(targetMask, actorIdx).
 */
// FUNCTION: FF7 0x0051f689
void __cdecl BattleFx_SpecialEnterWingScales(int targetMask, int actorIdx)
{
    g_BattleSpecWingScalesParticleActiveFlag = 0;
    BattleFx_SpecialBeginWingScales(targetMask, actorIdx);
}

/*
 * Calls BattleFx_SpecialLoadResWingScales, creates a task for BattleFx_SpecialSpawnWingScalesPerTarget with mode 4, dispatches event 0x11E for
 * actorIdx, calls BattleFx_SpecialWingScalesInitParticlePairPool, and allocates a slot for BattleFx_SpecialWingScalesScreenFlashTask.
 */
// FUNCTION: FF7 0x0051f6a7
void __cdecl BattleFx_SpecialBeginWingScales(int targetMask, int actorIdx)
{
    BattleFx_SpecialLoadResWingScales();
    Battle_CreateBitPatternCallbackTask(targetMask, actorIdx, 4, BattleFx_SpecialSpawnWingScalesPerTarget);
    Battle_PlaySfxPannedToActor(0x11E, (short)actorIdx, 0);
    BattleFx_SpecialWingScalesInitParticlePairPool();
    Battle_AllocateResourceSlot(BattleFx_SpecialWingScalesScreenFlashTask);
}

/*
 * Draws a screen flash using shared intensity as its first and third arguments before checking
 * pause. When unpaused adds three below counter sixteen, subtracts three above eighty-four and
 * holds between; advances and at exactly one hundred sets g_BattleSpecWingScalesParticleActiveFlag to one and retires.
 */
// FUNCTION: FF7 0x0051f6ee
void __cdecl BattleFx_SpecialWingScalesScreenFlashTask(void)
{
    BattleTaskSlot *task;

    task = BATTLE_CURRENT_TASK_SLOT;
    g_BattleOverlayDrawListHead = BattleFx_FlashScreenWhite(*(char *)&g_BattleSpecWingScalesFlashIntensity, 0, *(char *)&g_BattleSpecWingScalesFlashIntensity);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (task->frameCounter < 16) {
        g_BattleSpecWingScalesFlashIntensity += 3;
    }
    if (task->frameCounter > 84) {
        g_BattleSpecWingScalesFlashIntensity -= 3;
    }
    (task->frameCounter)++;
    if (task->frameCounter == 100) {
        g_BattleSpecWingScalesParticleActiveFlag = 1;
        task->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Allocates BattleFx_SpecialWingScalesTimelineTask; its start position is the source actor's bone34
 * world position, its end position the target actor's targetBone world position, and actorIndex
 * the target.
 */
// FUNCTION: FF7 0x0051f799
void __cdecl BattleFx_SpecialSpawnWingScalesPerTarget(int actor, int sourceActor)
{
        int p;
        BattleFxWingScalesTimeline *q;

    q = (BattleFxWingScalesTimeline *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialWingScalesTimelineTask);
    Battle_GetBoneWorldPos(sourceActor, g_BattleActorRecords[sourceActor].bone34, (short *)&q->startX);
    Battle_GetBoneWorldPos(actor, g_BattleActorRecords[actor].targetBone, (short *)&q->endX);
    q->actorIndex = actor;
}

/*
 * Per-target timeline: at frame 0 spawns the particle-pair spawner travelling from start to end, at
 * frame 0x30 the impact model at the end position with its sound, at 0x46 allocates a typed record
 * for the target, and retires at frame 0x50.
 */
// FUNCTION: FF7 0x0051f811
void __cdecl BattleFx_SpecialWingScalesTimelineTask(void)
{
    short dx8;
        BattleFxWingScalesTimeline *pCur;
        int r;
        BattleFxWingScalesTravel *workTravel;
        int t, u, v, w, x, y, z;
    short dz;
        int b, c, d, e, f, g;

    pCur = (BattleFxWingScalesTimeline *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (pCur->frame == 0) {
        workTravel = (BattleFxWingScalesTravel *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialWingScalesSpawnParticlePairsTask);
        workTravel->angle = ArcTangentFixed(pCur->endX - pCur->startX, pCur->endZ - pCur->startZ);
        workTravel->posX = pCur->startX;
        workTravel->posY = pCur->startY;
        workTravel->posZ = pCur->startZ;
        dx8 = pCur->endX - workTravel->posX;
        dz = pCur->endZ - workTravel->posZ;
        workTravel->speed = SqrtToInt(dx8 * dx8 + dz * dz) / 24;
    }
    if (pCur->frame == 70) {
        Battle_PostActorHitOrHurtMessage(pCur->actorIndex);
    }
    if (pCur->frame == 48) {
        workTravel = (BattleFxWingScalesTravel *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialWingScalesImpactModelTask);
        workTravel->posX = pCur->endX;
        workTravel->posY = pCur->endY;
        workTravel->posZ = pCur->endZ;
        Battle_PlaySfxPannedToActor(0x19B, (short)pCur->actorIndex, 0);
    }
    (pCur->frame)++;
    if (pCur->frame == 80) {
        pCur->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Draws the spinning impact model at the task's base position, emits two variant sprites a frame
 * for the first 20 frames, and retires at frame 0x1E.
 */
// FUNCTION: FF7 0x0051f9c6
void __cdecl BattleFx_SpecialWingScalesImpactModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[96];
        int depthZ;
        struct { unsigned char pad_to_4[0x4]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_28[0x28]; PsxMatrix rotAngles; };
        struct { unsigned char pad_to_48[0x48]; BattleFxNapalmTrailSlot *sprite; };
        struct { unsigned char pad_to_4C[0x4C]; short axisIndex; };
        struct { unsigned char pad_to_54[0x54]; BattleFxWingScalesImpactModelSlot *slot; };
        struct { unsigned char pad_to_5C[0x5C]; short spriteAngle; };
    } frame;
#pragma pack(pop)

    frame.slot = (BattleFxWingScalesImpactModelSlot *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecWingScalesSpriteReq1.res16 = Gfx_ModelTableGetRenderObject(0, g_BattleSpecWingScalesLoadedResTable);
    for (frame.axisIndex = 0; frame.axisIndex < 3; frame.axisIndex++) {
        frame.rotAngles.m[2][frame.axisIndex] = 0;
        frame.rotAngles.m[1][frame.axisIndex] = 0;
        frame.rotAngles.m[0][frame.axisIndex] = 0;
    }
    SetRotMatrix(&g_BattleCameraTransform);
    SetTransMatrix(&g_BattleCameraTransform);
    RotTransPoint((SVECTOR *)&frame.slot->baseX, frame.rotAngles.t, &frame.depthZ);
    frame.rotAngles.m[2][2] = frame.slot->frameCounter * 0x12C + 0x800;
    frame.rotAngles.m[1][1] = frame.rotAngles.m[2][2];
    frame.rotAngles.m[0][0] = frame.rotAngles.m[1][1];
    RotMatrix((SVECTOR *)&g_BattleSpecWingScalesSpriteReq2, &frame.rotMatrix);
    ConcatMatrixIntoFirst(&frame.rotAngles, &frame.rotMatrix);
    SetRotMatrix(&frame.rotAngles);
    SetTransMatrix(&frame.rotAngles);
    SetDrawColorRGB(0, 0, 0);
    g_BattleSpecWingScalesSpriteReq1.alpha = frame.slot->frameCounter << 7;
    g_BattleSpecWingScalesDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecWingScalesSpriteReq1.res16, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecWingScalesDrawListHead);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.slot->frameCounter < 20) {
        for (frame.axisIndex = 0; frame.axisIndex < 2; frame.axisIndex++) {
            frame.spriteAngle = ((rand()) % 4096);
            frame.sprite = (BattleFxNapalmTrailSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialWingScalesVariantSpriteTask);
            frame.sprite->posX = frame.slot->baseX + (SinFixed(frame.spriteAngle) * 0x258 >> 12);
            frame.sprite->posY = frame.slot->baseY + frame.slot->frameCounter * 50 + rand() % 800 + -1000;
            frame.sprite->posZ = frame.slot->baseZ + (CosFixed(frame.spriteAngle) * 0x258 >> 12);
            frame.sprite->variant = 0;
        }
    }
    (frame.slot->frameCounter)++;
    if (frame.slot->frameCounter == 30) {
        frame.slot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Selects sprite field three or four from slot variant, draws at scale 0x3000 for variant zero or
 * 0x2000 otherwise and publishes the counter before checking pause. When unpaused adds twenty to Y
 * only in variant zero; with a nonnull sprite advances and retires at or above its header frame
 * count.
 */
// FUNCTION: FF7 0x0051fc31
void __cdecl BattleFx_SpecialWingScalesVariantSpriteTask(void)
{
    struct {
        int resources[2];
        BattleSpecWingScalesSpriteTask *p;
    } local; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    local.p = (BattleSpecWingScalesSpriteTask *)BATTLE_CURRENT_TASK_SLOT;
    local.resources[0] = 3;
    local.resources[1] = 4;
    BattleFx_InstallBillboardMatrix(&local.p->posX,
                                    local.p->variant ? 0x2000 : 0x3000,
                                    0);
    g_BattleSpecWingScalesSpriteReq1.frame = local.p->frameIndex;
    (g_BattleSpecWingScalesSpriteReq1.spriteResource) = Gfx_ModelTableGetLoadedRecord(local.resources[local.p->variant], g_BattleSpecWingScalesLoadedResTable);
    g_BattleSpecWingScalesSpriteReq1.res4 = Gfx_ModelTableGetSpriteObject(local.resources[local.p->variant], g_BattleSpecWingScalesLoadedResTable);
    g_BattleSpecWingScalesDrawListHead = BattleFx_SubmitSpriteDraw((int *)&g_BattleSpecWingScalesSpriteReq1, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecWingScalesDrawListHead);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (local.p->variant == 0) {
        local.p->posY += 20;
    }
    if ((g_BattleSpecWingScalesSpriteReq1.spriteResource) != 0) {
        (local.p->frameIndex)++;
        if (local.p->frameIndex >= *(int *)((char *)*(int *)(g_BattleSpecWingScalesSpriteReq1.spriteResource) + 4)) {
            local.p->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Every other frame claims a particle-pair pool record and places it on a spiral around the
 * travelling position, with a velocity derived from the task's speed; retires at frame 0x20.
 */
// FUNCTION: FF7 0x0051fd56
void __cdecl BattleFx_SpecialWingScalesSpawnParticlePairsTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[64];
        SVECTOR rotX;
        struct { unsigned char pad_to_8[0x8]; int depthZ; };
        struct { unsigned char pad_to_C[0xC]; SVECTOR pointZ; };
        struct { unsigned char pad_to_14[0x14]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_34[0x34]; BattleFxWingScalesParticlePair *pairRecord; };
        struct { unsigned char pad_to_38[0x38]; short radius; };
        struct { unsigned char pad_to_3C[0x3C]; BattleFxWingScalesSpawnParticlePairsSlot *slot; };
    } frame;
#pragma pack(pop)

    frame.slot = (BattleFxWingScalesSpawnParticlePairsSlot *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if ((frame.slot->frameCounter % 2) == 0) {
        frame.radius = (SinFixed(frame.slot->frameCounter << 6) * 0x140 >> 12) + 320;
        frame.pairRecord = (BattleFxWingScalesParticlePair *)(((short)BattleFx_SpecialWingScalesClaimParticlePairSlot() << 5) + (char *)&g_BattleSpecWingScalesParticlePairSlots);
        frame.pairRecord->posX = frame.slot->posX;
        frame.pairRecord->posY = frame.slot->posY;
        frame.pairRecord->posZ = frame.slot->posZ;
        frame.pairRecord->spriteFrame = frame.slot->frameCounter >> 1;
        frame.pairRecord->scale = frame.radius * 0xA + 0x1000;
        frame.pairRecord->radius = frame.radius;
        frame.pairRecord->angle = (rand() % 4096);
        frame.pointZ.vx = 0;
        frame.pointZ.vy = 0;
        frame.pointZ.vz = frame.slot->speed;
        frame.rotX.vx = 0;
        frame.rotX.vy = (-SinFixed(frame.slot->frameCounter * 0x88)) / 16;
        frame.rotX.vz = 0x200 /* 45 deg */;
        RotMatrixZYX(&frame.rotX, &frame.rotMatrix);
        RotateMatrixY(frame.slot->angleY, &frame.rotMatrix);
        frame.rotMatrix.t[2] = 0;
        frame.rotMatrix.t[1] = frame.rotMatrix.t[2];
        frame.rotMatrix.t[0] = frame.rotMatrix.t[1];
        SetRotMatrix(&frame.rotMatrix);
        SetTransMatrix(&frame.rotMatrix);
        RotTransPointToInt16(&frame.pointZ, (short *)&frame.pairRecord->velocityX, &frame.depthZ);
    }
    (frame.slot->frameCounter)++;
    if (frame.slot->frameCounter == 32) {
        frame.slot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Clears the status and frame index of the 150 particle-pair records and allocates BattleFx_SpecialWingScalesParticlePairPoolTask.
 */
// FUNCTION: FF7 0x0051ff1d
void __cdecl BattleFx_SpecialWingScalesInitParticlePairPool(void)
{
    BattleFxWingScalesParticlePair *cursor;
    short i;

    cursor = (BattleFxWingScalesParticlePair *)&g_BattleSpecWingScalesParticlePairSlots;
    for (i = 0; i < 150; i++) {
        cursor->frameIndex = 0;
        cursor->status = 0;
        cursor++;
    }
    Battle_AllocateResourceSlot(BattleFx_SpecialWingScalesParticlePairPoolTask);
}

/*
 * Steps the 150 particle-pair records, using g_BattleSpecWingScalesParticleLoopIndex as the loop index
 * that the child sees. For each record whose status is 1 it calls
 * BattleFx_SpecialWingScalesDrawParticlePairSprites and, if that leaves the status at -1, clears the
 * status and frame index. Once g_BattleSpecWingScalesParticleActiveFlag is 1 it retires its own task slot.
 */
// FUNCTION: FF7 0x0051ff77
void __cdecl BattleFx_SpecialWingScalesParticlePairPoolTask(void)
{
    BattleTaskSlot *taskPtr;
    BattleFxWingScalesParticlePair *rec;

    taskPtr = BATTLE_CURRENT_TASK_SLOT;
    rec = (BattleFxWingScalesParticlePair *)&g_BattleSpecWingScalesParticlePairSlots;
    for (g_BattleSpecWingScalesParticleLoopIndex = 0; g_BattleSpecWingScalesParticleLoopIndex < 150; g_BattleSpecWingScalesParticleLoopIndex++) {
        if (rec->status == 1) {
            BattleFx_SpecialWingScalesDrawParticlePairSprites();
            if (rec->status == -1) {
                rec->frameIndex = 0;
                rec->status = 0;
            }
        }
        rec++;
    }
    if (g_BattleSpecWingScalesParticleActiveFlag == 1) {
        taskPtr->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Draws the pair of sprites of the pool record at g_BattleSpecWingScalesParticleLoopIndex, then advances
 * its position, angle and frame; marks the record finished (0xFFFF) when its animation ends.
 */
// FUNCTION: FF7 0x00520011
void __cdecl BattleFx_SpecialWingScalesDrawParticlePairSprites(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[12];
        SVECTOR spritePosX;
        struct { unsigned char pad_to_8[0x8]; BattleFxWingScalesParticlePair *pairRecord; };
    } frame;
#pragma pack(pop)

    frame.pairRecord = (BattleFxWingScalesParticlePair *)((g_BattleSpecWingScalesParticleLoopIndex << 5) + (char *)&g_BattleSpecWingScalesParticlePairSlots);
    g_BattleSpecWingScalesSpriteReq2.res8 = Gfx_ModelTableGetLoadedRecord(1, g_BattleSpecWingScalesLoadedResTable);
    g_BattleSpecWingScalesSpriteReq2.res12 = Gfx_ModelTableGetSpriteObject(1, g_BattleSpecWingScalesLoadedResTable);
    g_BattleSpecWingScalesSpriteReq2.res24 = Gfx_ModelTableGetLoadedRecord(2, g_BattleSpecWingScalesLoadedResTable);
    g_BattleSpecWingScalesSpriteReq2.object28 = Battle3d_GetSceneRenderObject(1, g_pBattle3dScene);
    BattleFx_InstallBillboardMatrix(&frame.pairRecord->posX, 0x2000 /* 2.0 */, 0);
    g_BattleSpecWingScalesSpriteReq2.frame = frame.pairRecord->spriteFrame;
    g_BattleSpecWingScalesDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecWingScalesSpriteReq2.res8, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecWingScalesDrawListHead);
    frame.spritePosX.vx = frame.pairRecord->posX + (CosFixed(frame.pairRecord->angle) * frame.pairRecord->radius >> 12);
    frame.spritePosX.vy = frame.pairRecord->posY + (SinFixed(frame.pairRecord->angle) * frame.pairRecord->radius >> 12);
    frame.spritePosX.vz = frame.pairRecord->posZ;
    BattleFx_InstallBillboardMatrix(&frame.spritePosX, (short)frame.pairRecord->scale, 0);
    g_BattleSpecWingScalesSpriteReq2.secondFrame = frame.pairRecord->frameIndex % *(int *)((char *)*(int *)g_BattleSpecWingScalesSpriteReq2.res8 + 4);
    g_BattleSpecWingScalesDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecWingScalesSpriteReq2.res24, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecWingScalesDrawListHead);
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.pairRecord->posX += frame.pairRecord->velocityX;
    frame.pairRecord->posY += frame.pairRecord->velocityY;
    frame.pairRecord->posZ += frame.pairRecord->velocityZ;
    frame.pairRecord->angle += 0x100 /* 22.5 deg */;
    if (g_BattleSpecWingScalesSpriteReq2.res8 != 0) {
        (frame.pairRecord->frameIndex)++;
        if (frame.pairRecord->frameIndex >> 1 >= *(int *)((char *)*(int *)g_BattleSpecWingScalesSpriteReq2.res8 + 4)) {
            frame.pairRecord->status = 0xFFFF;
        }
    }
}

/*
 * Claims the first particle-pair record whose status is zero by setting it to one
 * and returns its index; when full dispatches (6, -1, 0x40) and returns zero, which also
 * represents the first valid index.
 */
// FUNCTION: FF7 0x0052021c
short __cdecl BattleFx_SpecialWingScalesClaimParticlePairSlot(void)
{
    BattleFxWingScalesParticlePair *cursor;
    short i;

    cursor = (BattleFxWingScalesParticlePair *)&g_BattleSpecWingScalesParticlePairSlots;
    for (i = 0; i < 150; i++) {
        if (cursor->status == 0) {
            cursor->status = 1;
            return i;
        }
        cursor++;
    }
    i = 0;
    Battle_PlaySfxPannedToActor(6, -1, 0x40);
    return 0;
}
} /* extern "C" */
