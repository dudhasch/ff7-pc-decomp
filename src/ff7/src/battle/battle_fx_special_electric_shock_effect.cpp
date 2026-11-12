// src/ff7/src/battle/battle_fx_special_electric_shock_effect.cpp
/* Enemy Special Attack [115]: Electric Shock (orig. SPECIAL/DENGEKI, 電撃) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle special effect 'Dengeki': a per-target timeline with a trail emitter and trail
//   sprites, shrinking and fading sprites, and a ring of sprites.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/enums/sound.h"
#include "ff7/api/battle.h"
#include "ff7/battle_tasks.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_special.h"
#include "lib/types/graphics.h"
#include "ff7/camera_script.h"
#include <string.h>
#include "lib/codegen.h"
extern "C" {


// clang-format off
// GLOBAL: FF7 0x0089ea80
int g_BattleSpecElectricShockResourceDefs = 0x00192A39;
char *g_BattleFxSpecialElectricShockEffectResourceNames[1] = {
    // STRING: FF7 0x0089eb48
    "GRAVIR03.TIM"
};
unsigned int g_BattleFxSpecialElectricShockEffectResourceNames2[24] = {
    // STRING: FF7 0x0089eb58
    (unsigned int)"GRAV_2.SPT",
    0x00000000,
    0x00000000,
    0x00196a39,
    // STRING: FF7 0x0089eb64
    (unsigned int)"GRAVIR00.TIM",
    // STRING: FF7 0x0089eb74
    (unsigned int)"ELEC_4.SPT",
    0x00000000,
    0x00000000,
    0x00196a39,
    // STRING: FF7 0x0089eb80
    (unsigned int)"GRAVIR01.TIM",
    0x00000000,
    0x00000000,
    0x00000000,
    0x00196a39,
    // STRING: FF7 0x0089eb90
    (unsigned int)"GRAVIR02.TIM",
    0x00000000,
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x0089eba0
    (unsigned int)"GRAVIR03.TIM",
    // STRING: FF7 0x0089ebb0
    (unsigned int)"GRAV_2.SPT",
    0x00000000,
    0x00000000,
    0x00000000
};
// GLOBAL: FF7 0x00b5ddd0
int g_BattleSpecElectricShockDrawListHead = 0;
// GLOBAL: FF7 0x00b5ddd4
struct GfxTableCursor *g_BattleSpecElectricShockLoadedResTable = 0;
// clang-format on

// clang-format off
#undef g_BattleSpecElectricShockSpriteReq1
// GLOBAL: FF7 0x0089eae8
struct BattleFxSpriteRequest g_BattleSpecElectricShockSpriteReq1 = {0, 0, -128, -128, -128, 0x2e, 0, 0};
// GLOBAL: FF7 0x0089eaf8
char g_BattleSpecElectricShockOffsets24[24] = {-64, -1, -64, -1, 0, 64, 127, 127, -128, -128, -128, 46, 47, 0, -56, 120, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x0089eb10
unsigned char g_BattleSpecElectricShockColorParam[8] = {0xf0, 0xff, 0xf0, 0xff, 0x80, 0x40, 0x1f, 0x1f};
// clang-format on

// clang-format off
// GLOBAL: FF7 0x0089eb18
struct BattleSpecElectricShockSpriteReq2 g_BattleSpecElectricShockSpriteReq2 = {-128, -128, -128, {0x2e, 0x2f, 0, 0x48, 0x79}, 0, {0}, 0, 0};
// GLOBAL: FF7 0x0089eb30
struct BattleSpecElectricShockSpriteReq3 g_BattleSpecElectricShockSpriteReq3 = {-128, -128, -128, {0x2e}, 0, {0}, 0, 0};
// GLOBAL: FF7 0x0089eb40
struct BattleSpecElectricShockSpriteReq4 g_BattleSpecElectricShockSpriteReq4 = {-128, -128, -128, {0x2e}, 0, {0}};
// clang-format on

#pragma intrinsic(memset)
#pragma intrinsic(memcpy)

// clang-format off
// clang-format on


/*
 * When the engine context exists and g_BattleSpecElectricShockLoadedResTable is empty, builds SPECIAL/DENGEKI with a default-
 * duration record and creates resource type 5 using g_BattleSpecElectricShockResourceDefs.
 */
// FUNCTION: FF7 0x00528200
void __cdecl BattleFx_SpecialLoadResElectricShock(void)
{
    GfxModelTableBank *curOwnerCtx;
    GfxResourceRequest currentRecord;
    int kind;
    char outSize[200];
    GfxResourceRequest *pRequest;
    int resourceDefsWord;

    if (g_pBattle3dScene != 0 && g_BattleSpecElectricShockLoadedResTable == 0) {
        kind = 5;
        resourceDefsWord = (int)&g_BattleSpecElectricShockResourceDefs;
        curOwnerCtx = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRequest = &currentRecord;
        Gfx_InitResourceRequest(pRequest);
        pRequest->groupTag = 0;
        // STRING: FF7 0x0089ebbc
        pRequest->resourcePath = BattleFx_BuildResourcePath("SPECIAL/DENGEKI", outSize);
        Gfx_LoadModelTableIfAbsent(kind, resourceDefsWord, pRequest, &g_BattleSpecElectricShockLoadedResTable, curOwnerCtx);
    }
}

/*
 * Calls BattleFx_SpecialLoadResElectricShock, allocates a slot for BattleFx_SpecialElectricShockTimelineTask, and stores both arguments in
 * the new slot.
 */
// FUNCTION: FF7 0x00528290
void __cdecl BattleFx_SpecialEnterElectricShock(int targetMask, int actorIdx)
{
    BattleFxElectricShockTimeline *slot;

    BattleFx_SpecialLoadResElectricShock();
    slot = (BattleFxElectricShockTimeline *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialElectricShockTimelineTask);
    slot->targetMask = targetMask;
    slot->sourceActor = actorIdx;
}

/*
 * When unpaused at counter zero allocates BattleFx_SpecialElectricShockSmallFadingSpriteTask at the selected actor bone in child at its bone position,
 * stores negative actor radius in it and plays sound 0x13F. At fifteen creates a bit-pattern
 * callback task from two parent shorts and BattleFx_SpecialSpawnElectricShockPerTarget, retires the parent, then advances its
 * counter.
 */
// FUNCTION: FF7 0x005282ce
void __cdecl BattleFx_SpecialElectricShockTimelineTask(void)
{
    BattleFxElectricShockFadingSprite *pSprite;
    BattleFxElectricShockTimelineSlot *task;

    task = (BattleFxElectricShockTimelineSlot *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (task->timelineStep == 0) {
        pSprite = (BattleFxElectricShockFadingSprite *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialElectricShockSmallFadingSpriteTask);
        Battle_GetBoneWorldPos(task->targetIndex,
                               g_BattleActorRecords[task->targetIndex].bone33,
                               &pSprite->bonePosX);
        pSprite->negRadius = -g_BattleActorRecords[task->targetIndex].radius;
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputeScreenPanFromPosition(&pSprite->bonePosX), 0x13F);
    }
    if (task->timelineStep == 15) {
        Battle_CreateBitPatternCallbackTask(task->bitPattern, task->targetIndex, 0, BattleFx_SpecialSpawnElectricShockPerTarget);
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    (task->timelineStep)++;
}

/*
 * Allocates a slot for BattleFx_SpecialElectricShockPerTargetTimelineTask and stores both arguments in the new slot.
 */
// FUNCTION: FF7 0x005283d3
void __cdecl BattleFx_SpecialSpawnElectricShockPerTarget(int actor, int sourceActor)
{
    BattleFxElectricShockPerTargetTimeline *slot;

    slot = (BattleFxElectricShockPerTargetTimeline *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialElectricShockPerTargetTimelineTask);
    slot->targetIndex = actor;
    slot->sourceActor = sourceActor;
}

/*
 * Per-target timeline task that sequences lightning rings, flashes, and hit reactions for Dengeki.
 */
// FUNCTION: FF7 0x0052840c
void __cdecl BattleFx_SpecialElectricShockPerTargetTimelineTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[16];
        BattleFxGrav2Particle *particle;
        struct { unsigned char pad_to_4[0x4]; BattleFxMagicDemi2Timeline *perTarget; };
        struct { unsigned char pad_to_8[0x8]; int boneX; };
        struct { unsigned char pad_to_A[0xA]; short boneY; };
        struct { unsigned char pad_to_C[0xC]; int boneZ; };
    } frame;
#pragma pack(pop)

    frame.perTarget = (BattleFxMagicDemi2Timeline *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.perTarget->step == 0) {
        frame.particle = (BattleFxGrav2Particle *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialElectricShockTrailEmitterTask);
        Battle_GetBoneWorldPos(frame.perTarget->targetIndex, g_BattleActorRecords[frame.perTarget->targetIndex].bone33, &frame.particle->startX);
        Battle_GetBoneWorldPos(frame.perTarget->actorIndex, g_BattleActorRecords[frame.perTarget->actorIndex].targetBone, (short *)&frame.boneX);
        frame.particle->velocityX = ((short)frame.boneX - frame.particle->startX) / 8;
        frame.particle->velocityY = (frame.boneY - frame.particle->startY) / 8;
        frame.particle->velocityZ = (*(short *)&frame.boneZ - frame.particle->startZ) / 8;
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputeScreenPanFromPosition(&frame.particle->startX), 0x50);
    }
    if (frame.perTarget->step == 8) {
        frame.particle = (BattleFxGrav2Particle *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialElectricShockShrinkingFadeSpriteTask);
        Battle_GetBoneWorldPos(frame.perTarget->actorIndex, g_BattleActorRecords[frame.perTarget->actorIndex].targetBone, &frame.particle->startX);
        frame.particle->pushLength = -g_BattleActorRecords[frame.perTarget->actorIndex].radius;
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputeScreenPanFromPosition(&frame.particle->startX), 0x4B);
    }
    if (frame.perTarget->step == 8) {
        Battle_GetBoneWorldPos(frame.perTarget->actorIndex, g_BattleActorRecords[frame.perTarget->actorIndex].targetBone, (short *)&frame.boneX);
        frame.particle = (BattleFxGrav2Particle *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialElectricShockRingSpritesTask);
        *(Int64Pair *)&frame.particle->startX = *(Int64Pair *)(&frame.boneX);
        frame.particle->pushLength = -g_BattleActorRecords[frame.perTarget->actorIndex].radius;
        frame.particle->velocityZ = 0;
        frame.particle->velocityY = 0;
        frame.particle->velocityX = 0;
        frame.particle = (BattleFxGrav2Particle *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialElectricShockRingSpritesTask);
        *(Int64Pair *)&frame.particle->startX = *(Int64Pair *)(&frame.boneX);
        frame.particle->pushLength = -g_BattleActorRecords[frame.perTarget->actorIndex].radius;
        frame.particle->velocityX = 0;
        frame.particle->velocityY = 1024;
        frame.particle->velocityZ = 0;
    }
    if (frame.perTarget->step == 8) {
        frame.particle = (BattleFxGrav2Particle *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialElectricShockLargeFadingSpriteTask);
        Battle_GetBoneWorldPos(frame.perTarget->actorIndex, g_BattleActorRecords[frame.perTarget->actorIndex].targetBone, &frame.particle->startX);
        frame.particle->pushLength = -g_BattleActorRecords[frame.perTarget->actorIndex].radius;
    }
    if (frame.perTarget->step == 8) {
        Battle_PostActorHitOrHurtMessage(frame.perTarget->actorIndex);
    }
    if (frame.perTarget->step >= 8) {
        frame.perTarget->ownerSlot = BATTLE_TASK_FREE;
    }
    (frame.perTarget->step)++;
}

/*
 * When unpaused adds the velocity triple to its position triple,
 * allocates BattleFx_SpecialElectricShockTrailSpriteTask with an eight-byte position copy, advances the counter, and retires the
 * slot at an updated count of eight or above.
 */
// FUNCTION: FF7 0x0052876e
void __cdecl BattleFx_SpecialElectricShockTrailEmitterTask(void)
{
    BattleFxElectricShockTrailEmitter *pEmitter;
    BattleFxElectricShockTrailEmitter *child;

    pEmitter = (BattleFxElectricShockTrailEmitter *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    pEmitter->posX += pEmitter->velocityX;
    pEmitter->posY += pEmitter->velocityY;
    pEmitter->posZ += pEmitter->velocityZ;
    child = (BattleFxElectricShockTrailEmitter *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialElectricShockTrailSpriteTask);
    *(Int64Pair *)&child->posX = *(Int64Pair *)&pEmitter->posX;
    (pEmitter->frameCount)++;
    if (pEmitter->frameCount >= 8) {
        pEmitter->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Update for the record at (g_BattleActiveTaskSlotIdx << 5) + &g_BattleHitLivewireEmitterRecords using
 * animation 0 of g_BattleSpecElectricShockLoadedResTable into
 * g_BattleSpecElectricShockSpriteReq1/g_BattleSpecElectricShockSpriteReq1[4]. Installs the billboard matrix
 * from the slot's second position triple (not the first, as its neighbours do) at scale 0xD00 and submits the sprite through
 * g_BattleSpecElectricShockDrawListHead only when the returned structure's matrix field is
 * positive. While g_BattleIsPaused is zero the frame counter advances and the slot is freed
 * after 5 frames.
 */
// FUNCTION: FF7 0x0052882e
void __cdecl BattleFx_SpecialElectricShockTrailSpriteTask(void)
{
    BattleTaskSlot *task;
    int matrix;

    task = BATTLE_CURRENT_TASK_SLOT;
    (g_BattleSpecElectricShockSpriteReq1.spriteGroup) = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(0, g_BattleSpecElectricShockLoadedResTable);
    g_BattleSpecElectricShockSpriteReq1.drawObj = Gfx_ModelTableGetSpriteObject(0, g_BattleSpecElectricShockLoadedResTable);
    matrix = (int)BattleFx_InstallBillboardMatrix((void *)((char *)task + 8), 0xD00 /* 0.8125 */, 0);
    if (*(int *)((char *)matrix + 0x1A) > 0) {
        g_BattleSpecElectricShockSpriteReq1.frame = task->frameCounter;
        g_BattleSpecElectricShockDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecElectricShockSpriteReq1, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecElectricShockDrawListHead);
    }
    if (g_BattleIsPaused == 0) {
        (task->frameCounter)++;
        if (task->frameCounter >= 5) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Draws and shrinks a fading central electric glow sprite for the Dengeki effect.
 */
// FUNCTION: FF7 0x005288fb
void __cdecl BattleFx_SpecialElectricShockShrinkingFadeSpriteTask(void)
{
    struct {
        void *matrix;
        BattleFxElectricShockFadeRecord *p;
        int frame;
        int scale;
    } local; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    local.p = (BattleFxElectricShockFadeRecord *)BATTLE_CURRENT_TASK_SLOT;
    *(int *)&g_BattleSpecElectricShockOffsets24[0x10] = Gfx_ModelTableGetSpriteObject(4, g_BattleSpecElectricShockLoadedResTable);
    local.scale = local.p->frame * -0x3000 / 40 + 0x3000;
    local.frame = local.p->frame;
    if (local.frame < 20) {
        g_BattleSpecElectricShockOffsets24[0xA] = (local.frame << 7) / 20;
        g_BattleSpecElectricShockOffsets24[9] = g_BattleSpecElectricShockOffsets24[0xA];
        g_BattleSpecElectricShockOffsets24[8] = g_BattleSpecElectricShockOffsets24[9];
    } else {
        local.frame -= 20;
        if (local.frame < 20) {
            g_BattleSpecElectricShockOffsets24[0xA] = 0x80;
            g_BattleSpecElectricShockOffsets24[9] = g_BattleSpecElectricShockOffsets24[0xA];
            g_BattleSpecElectricShockOffsets24[8] = g_BattleSpecElectricShockOffsets24[9];
        } else {
            local.p->ownerSlot = 0xFFFF;
            return;
        }
    }
    local.matrix = BattleFx_InstallBillboardMatrix(&local.p->posX,
                                                   (short)local.scale,
                                                   local.p->pushLength);
    if (*(int *)((char *)local.matrix + 0x1A) > 0) {
        g_BattleSpecElectricShockDrawListHead = BattleFx_SubmitSpriteQuadDraw((BattleFxSpriteQuadRequest *)&g_BattleSpecElectricShockOffsets24,
                                                                      g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED,
                                                                      0xC,
                                                                      g_BattleSpecElectricShockDrawListHead);
    }
    if (g_BattleIsPaused == 0) {
        (local.p->frame)++;
    }
}

/*
 * Animates and submits the orbiting ring of electric spark sprites for Dengeki.
 */
// FUNCTION: FF7 0x00528a27
void __cdecl BattleFx_SpecialElectricShockRingSpritesTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[52];
        PsxMatrix rotMatrix;
        struct { unsigned char pad_to_20[0x20]; int pointIndex; };
        struct { unsigned char pad_to_24[0x24]; BattleFxElectricShockRingSpritesSlot *ringSlot; };
        struct { unsigned char pad_to_28[0x28]; SVECTOR pointX; };
        struct { unsigned char pad_to_30[0x30]; int phase; };
    } frame;
#pragma pack(pop)

    frame.ringSlot = (BattleFxElectricShockRingSpritesSlot *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecElectricShockSpriteReq2.req2ResourceHandle = Gfx_ModelTableGetSpriteObject(4, g_BattleSpecElectricShockLoadedResTable);
    frame.phase = frame.ringSlot->frameCounter;
    if (frame.phase < 5) {
        g_BattleSpecElectricShockSpriteReq2.req2Byte2 = (frame.phase << 7) / 5;
        g_BattleSpecElectricShockSpriteReq2.req2Byte1 = g_BattleSpecElectricShockSpriteReq2.req2Byte2;
        (g_BattleSpecElectricShockSpriteReq2.savedByte1) = g_BattleSpecElectricShockSpriteReq2.req2Byte1;
    } else {
        frame.phase -= 5;
        if (frame.phase < 15) {
            g_BattleSpecElectricShockSpriteReq2.req2Byte2 = 0x80;
            g_BattleSpecElectricShockSpriteReq2.req2Byte1 = g_BattleSpecElectricShockSpriteReq2.req2Byte2;
            (g_BattleSpecElectricShockSpriteReq2.savedByte1) = g_BattleSpecElectricShockSpriteReq2.req2Byte1;
        } else {
            frame.phase -= 0xF;
            if (frame.phase < 5) {
                g_BattleSpecElectricShockSpriteReq2.req2Byte2 = frame.phase * -0x80 / 5 + 0x80;
                g_BattleSpecElectricShockSpriteReq2.req2Byte1 = g_BattleSpecElectricShockSpriteReq2.req2Byte2;
                (g_BattleSpecElectricShockSpriteReq2.savedByte1) = g_BattleSpecElectricShockSpriteReq2.req2Byte1;
            } else {
                frame.ringSlot->ownerSlot = BATTLE_TASK_FREE;
                return;
            }
        }
    }
    for (frame.pointIndex = 0; frame.pointIndex < 16; frame.pointIndex++) {
        frame.pointX.vx = SinFixed((frame.pointIndex << 12) / 16) * 0x1F4 >> 12;
        frame.pointX.vy = CosFixed((frame.pointIndex << 12) / 16) * 0x1F4 >> 12;
        frame.pointX.vz = 0;
        RotMatrix((SVECTOR *)((int)frame.ringSlot + 0x10), &frame.rotMatrix);
        TransformVectorToInt16(&frame.rotMatrix, &frame.pointX, &frame.pointX.vx);
        frame.pointX.vx += frame.ringSlot->velA;
        frame.pointX.vy += frame.ringSlot->velB;
        frame.pointX.vz += frame.ringSlot->velC;
        BattleFx_InstallBillboardMatrix(&frame.pointX, 0x600 /* 0.375 */, frame.ringSlot->pushLength);
        g_BattleSpecElectricShockDrawListHead = BattleFx_SubmitSpriteQuadDraw((BattleFxSpriteQuadRequest *)&g_BattleSpecElectricShockColorParam, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecElectricShockDrawListHead);
    }
    if (g_BattleIsPaused == 0) {
        frame.ringSlot->rotX += 0x50;
        frame.ringSlot->rotY += 0x40;
        frame.ringSlot->rotZ += 0x50;
        (frame.ringSlot->frameCounter)++;
    }
}

/*
 * Ramps brightness and draws the large electric burst quad for the Dengeki effect.
 */
// FUNCTION: FF7 0x00528c64
void __cdecl BattleFx_SpecialElectricShockLargeFadingSpriteTask(void)
{
    int age;
    BattleFxGrav3QuadSlot *task;

    task = (BattleFxGrav3QuadSlot *)BATTLE_CURRENT_TASK_SLOT;
    age = task->frameIndex;
    if (age < 5) {
        g_BattleSpecElectricShockSpriteReq3.req3Byte2 = (age << 7) / 5;
        g_BattleSpecElectricShockSpriteReq3.req3Byte1 = g_BattleSpecElectricShockSpriteReq3.req3Byte2;
        (g_BattleSpecElectricShockSpriteReq3.savedByte1) = g_BattleSpecElectricShockSpriteReq3.req3Byte1;
    } else {
        age -= 5;
        if (age < 35) {
            g_BattleSpecElectricShockSpriteReq3.req3Byte2 = 0x80;
            g_BattleSpecElectricShockSpriteReq3.req3Byte1 = g_BattleSpecElectricShockSpriteReq3.req3Byte2;
            (g_BattleSpecElectricShockSpriteReq3.savedByte1) = g_BattleSpecElectricShockSpriteReq3.req3Byte1;
        } else {
            age -= 35;
            if (age < 5) {
                g_BattleSpecElectricShockSpriteReq3.req3Byte2 = age * -0x80 / 5 + 0x80;
                g_BattleSpecElectricShockSpriteReq3.req3Byte1 = g_BattleSpecElectricShockSpriteReq3.req3Byte2;
                (g_BattleSpecElectricShockSpriteReq3.savedByte1) = g_BattleSpecElectricShockSpriteReq3.req3Byte1;
            } else {
                task->ownerSlot = BATTLE_TASK_FREE;
                return;
            }
        }
    }
    g_BattleSpecElectricShockSpriteReq3.req3Word4 = task->frameIndex % 12;
    BattleFx_InstallBillboardMatrix((void *)((char *)task + 8), 0x2000 /* 2.0 */, task->pushLength);
    g_BattleSpecElectricShockSpriteReq2.fadingResourceHandle = Gfx_ModelTableGetLoadedRecord(1, g_BattleSpecElectricShockLoadedResTable);
    g_BattleSpecElectricShockSpriteReq2.fadingSpriteHandle = Gfx_ModelTableGetSpriteObject(1, g_BattleSpecElectricShockLoadedResTable);
    if (g_BattleSpecElectricShockSpriteReq2.fadingResourceHandle != 0) {
        ((BattleMagicDemi3SpriteData *)g_BattleSpecElectricShockSpriteReq2.fadingResourceHandle)->spriteObject1 = Gfx_ModelTableGetSpriteObject(1, g_BattleSpecElectricShockLoadedResTable);
        ((BattleMagicDemi3SpriteData *)g_BattleSpecElectricShockSpriteReq2.fadingResourceHandle)->spriteObject2 = Gfx_ModelTableGetSpriteObject(2, g_BattleSpecElectricShockLoadedResTable);
        ((BattleMagicDemi3SpriteData *)g_BattleSpecElectricShockSpriteReq2.fadingResourceHandle)->spriteObject3 = Gfx_ModelTableGetSpriteObject(3, g_BattleSpecElectricShockLoadedResTable);
    }
    g_BattleSpecElectricShockDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecElectricShockSpriteReq2.fadingResourceHandle, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecElectricShockDrawListHead);
    if (g_BattleIsPaused == 0) {
        (task->frameIndex)++;
    }
}

/*
 * Ramps brightness and draws the small secondary electric spark quad for Dengeki.
 */
// FUNCTION: FF7 0x00528e2a
void __cdecl BattleFx_SpecialElectricShockSmallFadingSpriteTask(void)
{
    int age;
    BattleFxGrav3QuadSlot *task;

    task = (BattleFxGrav3QuadSlot *)BATTLE_CURRENT_TASK_SLOT;
    age = task->frameIndex;
    if (age < 5) {
        g_BattleSpecElectricShockSpriteReq4.req4Byte2 = (age << 7) / 5;
        g_BattleSpecElectricShockSpriteReq4.req4Byte1 = g_BattleSpecElectricShockSpriteReq4.req4Byte2;
        (g_BattleSpecElectricShockSpriteReq4.savedByte1) = g_BattleSpecElectricShockSpriteReq4.req4Byte1;
    } else {
        age -= 5;
        if (age < 10) {
            g_BattleSpecElectricShockSpriteReq4.req4Byte2 = 0x80;
            g_BattleSpecElectricShockSpriteReq4.req4Byte1 = g_BattleSpecElectricShockSpriteReq4.req4Byte2;
            (g_BattleSpecElectricShockSpriteReq4.savedByte1) = g_BattleSpecElectricShockSpriteReq4.req4Byte1;
        } else {
            age -= 10;
            if (age < 5) {
                g_BattleSpecElectricShockSpriteReq4.req4Byte2 = age * -0x80 / 5 + 0x80;
                g_BattleSpecElectricShockSpriteReq4.req4Byte1 = g_BattleSpecElectricShockSpriteReq4.req4Byte2;
                (g_BattleSpecElectricShockSpriteReq4.savedByte1) = g_BattleSpecElectricShockSpriteReq4.req4Byte1;
            } else {
                task->ownerSlot = BATTLE_TASK_FREE;
                return;
            }
        }
    }
    g_BattleSpecElectricShockSpriteReq4.req4Word4 = task->frameIndex % 12;
    BattleFx_InstallBillboardMatrix((void *)((char *)task + 8), 0x1000 /* 1.0 */, task->pushLength);
    g_BattleSpecElectricShockSpriteReq3.req3Handle8 = Gfx_ModelTableGetLoadedRecord(1, g_BattleSpecElectricShockLoadedResTable);
    g_BattleSpecElectricShockSpriteReq3.req3Handle12 = Gfx_ModelTableGetSpriteObject(1, g_BattleSpecElectricShockLoadedResTable);
    if (g_BattleSpecElectricShockSpriteReq3.req3Handle8 != 0) {
        ((BattleFxMagicDemi3MultiDrawReq *)g_BattleSpecElectricShockSpriteReq3.req3Handle8)->drawObj0 = Gfx_ModelTableGetSpriteObject(1, g_BattleSpecElectricShockLoadedResTable);
        ((BattleFxMagicDemi3MultiDrawReq *)g_BattleSpecElectricShockSpriteReq3.req3Handle8)->drawObj1 = Gfx_ModelTableGetSpriteObject(2, g_BattleSpecElectricShockLoadedResTable);
        ((BattleFxMagicDemi3MultiDrawReq *)g_BattleSpecElectricShockSpriteReq3.req3Handle8)->drawObj2 = Gfx_ModelTableGetSpriteObject(3, g_BattleSpecElectricShockLoadedResTable);
    }
    g_BattleSpecElectricShockDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecElectricShockSpriteReq3.req3Handle8, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecElectricShockDrawListHead);
    if (g_BattleIsPaused == 0) {
        (task->frameIndex)++;
    }
}
} /* extern "C" */
