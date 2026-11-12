// src/ff7/src/battle/battle_fx_special_whirlwind_effect.cpp
/* Enemy Special Attack [85, 86]: Great Whirlwind / Gale (orig. special/daisenp, 大旋風) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle special effect 'Daisenpu': a setup task with a timeline, orbiting sprites and
//   converging sprites, with variant entry points selecting its variant.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/enums/sound.h"
#include "ff7/api/battle.h"
#include "ff7/battle_tasks.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "lib/types/graphics.h"
#include "ff7/types/battle.h"
#include "lib/codegen.h"
#include "ff7/types/battle_special.h"
#include "ff7/camera_script.h"
#include <string.h>
#include <stdlib.h>
extern "C" {

// clang-format off
// GLOBAL: FF7 0x008a10c0
int g_BattleSpecWhirlwindResourceDefs = 0x00192A39;
char *g_BattleFxSpecialWhirlwindEffectResourceNames[1] = {
    // STRING: FF7 0x008a10f8
    "DAISENP.TIM"
};
unsigned int g_BattleFxSpecialWhirlwindEffectAssetPrefix[4] = {
    // STRING: FF7 0x008a1104
    (unsigned int)"DAISENP.SPT",
    0x00000000,
    0x00000000,
    0x00000000
};
#undef g_BattleSpecWhirlwindSpriteReq
// GLOBAL: FF7 0x008a10d8
unsigned char g_BattleSpecWhirlwindSpriteReq[32] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x24, 0x24, 0x2e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x40, 0x40, 0x40, 0x2e, 0x00, 0x00, 0x00, 0x00
};
// clang-format on
#pragma intrinsic(memset)
#pragma intrinsic(memcpy)


// This TU passes int pointers, unlike the VECTOR prototype in ff7/gte.h.

// Preserve the original native contribution boundary and period LINK alignment.

// clang-format off
// GLOBAL: FF7 0x00b602d0
BattleFxWhirlwindPos g_BattleSpecWhirlwindTargetPos = {0};
// GLOBAL: FF7 0x00b602d8
BattleFxWhirlwindWhirlPool g_BattleSpecWhirlwindWhirlwind = {0};
// GLOBAL: FF7 0x00b60c98
int g_BattleSpecWhirlwindVariant = 0;
// GLOBAL: FF7 0x00b60c9c
int g_BattleSpecWhirlwindSourceActor = 0;
// GLOBAL: FF7 0x00b60ca0
BattleFxWhirlwindPos g_BattleSpecWhirlwindTargetOffset = {0};
// GLOBAL: FF7 0x00b60ca8
int g_BattleSpecWhirlwindDeltaX = 0;
// GLOBAL: FF7 0x00b60cac
int g_BattleSpecWhirlwindDeltaY = 0;
// GLOBAL: FF7 0x00b60cb0
int g_BattleSpecWhirlwindDeltaZ = 0;
unsigned char g_bssPad_00b60cb4[4] = {0};
// GLOBAL: FF7 0x00b60cb8
int g_BattleSpecWhirlwindNormalX = 0;
// GLOBAL: FF7 0x00b60cbc
int g_BattleSpecWhirlwindNormalY = 0;
// GLOBAL: FF7 0x00b60cc0
int g_BattleSpecWhirlwindNormalZ = 0;
unsigned char g_bssPad_00b60cc4[4] = {0};
// GLOBAL: FF7 0x00b60cc8
int g_BattleSpecWhirlwindStepX = 0;
// GLOBAL: FF7 0x00b60ccc
int g_BattleSpecWhirlwindStepY = 0;
// GLOBAL: FF7 0x00b60cd0
int g_BattleSpecWhirlwindStepZ = 0;
unsigned char g_bssPad_00b60cd4[4] = {0};
// GLOBAL: FF7 0x00b60cd8
struct GfxTableCursor *g_BattleSpecWhirlwindLoadedResTable = 0;
unsigned char g_bssPad_00b60cdc[4] = {0};
// clang-format on

/*
 * Loads the Daisenpu resources. When the engine context exists and g_BattleSpecWhirlwindLoadedResTable is empty, builds special/daisenp with a default-
 * duration record and creates resource type 1 using g_BattleSpecWhirlwindResourceDefs.
 */
// FUNCTION: FF7 0x005313f0
void __cdecl BattleFx_SpecialLoadResWhirlwind(void)
{
    GfxModelTableBank *curOwnerCtx;
    GfxResourceRequest currentRecord;
    int kind;
    char pathText[200];
    GfxResourceRequest *pRequest;
    int resourceDefsWord;

    if (g_pBattle3dScene != 0 && g_BattleSpecWhirlwindLoadedResTable == 0) {
        kind = 1;
        resourceDefsWord = (int)&g_BattleSpecWhirlwindResourceDefs;
        curOwnerCtx = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRequest = &currentRecord;
        Gfx_InitResourceRequest(pRequest);
        pRequest->groupTag = 0;
        // STRING: FF7 0x008a1110
        pRequest->resourcePath = BattleFx_BuildResourcePath("special/daisenp", pathText);
        Gfx_LoadModelTableIfAbsent(kind, resourceDefsWord, pRequest, &g_BattleSpecWhirlwindLoadedResTable, curOwnerCtx);
    }
}

/*
 * Entry point for variant 0 of the Daisenpu effect: clears g_BattleSpecWhirlwindVariant to 0, calls
 * BattleFx_SpecialLoadResWhirlwind, then forwards both arguments to BattleFx_SpecialBeginWhirlwind. Identical to
 * BattleFx_SpecialEnterWhirlwindVar1 except for the value stored in g_BattleSpecWhirlwindVariant.
 */
// FUNCTION: FF7 0x00531480
void __cdecl BattleFx_SpecialEnterWhirlwindVar0(int targetMask, int actorIdx)
{
    g_BattleSpecWhirlwindVariant = 0;
    BattleFx_SpecialLoadResWhirlwind();
    BattleFx_SpecialBeginWhirlwind(targetMask, actorIdx);
}

/*
 * Entry point for variant 1 of the Daisenpu effect: stores 1 in g_BattleSpecWhirlwindVariant, calls
 * BattleFx_SpecialLoadResWhirlwind, then forwards both arguments to BattleFx_SpecialBeginWhirlwind. Identical to
 * BattleFx_SpecialEnterWhirlwindVar0 except for the value stored in g_BattleSpecWhirlwindVariant.
 */
// FUNCTION: FF7 0x005314a4
void __cdecl BattleFx_SpecialEnterWhirlwindVar1(int targetMask, int actorIdx)
{
    g_BattleSpecWhirlwindVariant = 1;
    BattleFx_SpecialLoadResWhirlwind();
    BattleFx_SpecialBeginWhirlwind(targetMask, actorIdx);
}

/*
 * Stores the allocated BattleFx_SelectDrawListBuffer slot address in g_BattleFxDrawListBufferTaskSlotPtr, allocates a
 * slot for BattleFx_SpecialWhirlwindSetupTask, stores sourceActor in g_BattleSpecWhirlwindSourceActor, clears g_BattleSpecWhirlwindWhirlwind.writeIndex, and dispatches opcode
 * 0x20 with entity-derived pan and sound 0x16A.
 */
// FUNCTION: FF7 0x005314c8
void __cdecl BattleFx_SpecialBeginWhirlwind(int sourceActor, int actorIdx)
{
    *(int *)&g_BattleFxDrawListBufferTaskSlotPtr = (int)BATTLE_NEW_TASK_SLOT(BattleFx_SelectDrawListBuffer);
    Battle_AllocateResourceSlot(BattleFx_SpecialWhirlwindSetupTask);
    g_BattleSpecWhirlwindSourceActor = sourceActor;
    g_BattleSpecWhirlwindWhirlwind.writeIndex = 0;
    Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputePanForEntity(sourceActor), 0x16A);
}

/*
 * Setup task: frees itself and starts the timeline task centred on the source actor. Variant 1 also
 * picks a random line across the target for the whirlwind to travel along.
 */
// FUNCTION: FF7 0x00531526
void __cdecl BattleFx_SpecialWhirlwindSetupTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[36];
        BattleFxWhirlwindRecord *timelineSlot;
        struct { unsigned char pad_to_4[0x4]; int rangeSpan; };
        struct { unsigned char pad_to_8[0x8]; short startX; };
        struct { unsigned char pad_to_A[0xA]; short startY; };
        struct { unsigned char pad_to_C[0xC]; short startZ; };
        struct { unsigned char pad_to_10[0x10]; int range; };
        struct { unsigned char pad_to_14[0x14]; int endX; };
        struct { unsigned char pad_to_16[0x16]; short endY; };
        struct { unsigned char pad_to_18[0x18]; int endZ; };
        struct { unsigned char pad_to_20[0x20]; int stepSize; };
    } frame;
#pragma pack(pop)

    frame.timelineSlot = (BattleFxWhirlwindRecord *)BATTLE_CURRENT_TASK_SLOT;
    frame.timelineSlot->ownerSlot = BATTLE_TASK_FREE;
    frame.timelineSlot = (BattleFxWhirlwindRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialWhirlwindTimelineTask);
    Battle_GetActorMaskCentroid(g_BattleSpecWhirlwindSourceActor, (short *)&frame.timelineSlot->centerX);
    frame.timelineSlot->centerY = 0;
    if (g_BattleSpecWhirlwindVariant != 0) {
        frame.timelineSlot->angle = (rand() % 4096);
        frame.timelineSlot->angularVelocity = rand() % 0x60 + 0x48;
        frame.timelineSlot->jitterOffset = 0xD80;
        frame.timelineSlot->jitterRange = frame.timelineSlot->jitterOffset << 1;
        frame.timelineSlot->baseScale = 0x2507;
        frame.range = 0x7E0;
        frame.rangeSpan = frame.range << 1;
        *(short *)&frame.endX = (rand() % frame.rangeSpan - frame.range) * 3;
        *(short *)&frame.endZ = (rand() % frame.rangeSpan - frame.range) * 3;
        frame.startX = frame.timelineSlot->centerX - *(short *)&frame.endX;
        frame.startY = 0;
        frame.startZ = frame.timelineSlot->centerZ - *(short *)&frame.endZ;
        *(short *)&frame.endX = frame.timelineSlot->centerX + *(short *)&frame.endX;
        frame.endY = 0;
        *(short *)&frame.endZ = frame.timelineSlot->centerZ + *(short *)&frame.endZ;
        *(int *)&g_BattleSpecWhirlwindTargetOffset = frame.endX;
        *(int *)&g_BattleSpecWhirlwindTargetOffset.z = frame.endZ;
        frame.timelineSlot->center = g_BattleSpecWhirlwindTargetOffset.pair;
        g_BattleSpecWhirlwindDeltaX = frame.startX - *(short *)&frame.endX;
        g_BattleSpecWhirlwindDeltaY = 0;
        g_BattleSpecWhirlwindDeltaZ = frame.startZ - *(short *)&frame.endZ;
        frame.stepSize = NormalizeVectorFixed(&g_BattleSpecWhirlwindDeltaX, &g_BattleSpecWhirlwindDeltaX);
        frame.stepSize = SqrtToInt(frame.stepSize) / 0x3C;
        g_BattleSpecWhirlwindNormalX = -g_BattleSpecWhirlwindDeltaZ;
        g_BattleSpecWhirlwindNormalY = 0;
        g_BattleSpecWhirlwindNormalZ = g_BattleSpecWhirlwindDeltaX;
        g_BattleSpecWhirlwindStepX = frame.stepSize * g_BattleSpecWhirlwindDeltaX >> 0xC;
        g_BattleSpecWhirlwindStepY = 0;
        g_BattleSpecWhirlwindStepZ = frame.stepSize * g_BattleSpecWhirlwindDeltaZ >> 0xC;
        frame.timelineSlot->stepSize = *(short *)&frame.stepSize;
    } else {
        frame.timelineSlot->jitterOffset = 0x900;
        frame.timelineSlot->jitterRange = frame.timelineSlot->jitterOffset << 1;
        frame.timelineSlot->baseScale = 0x1EA0;
    }
    g_BattleSpecWhirlwindTargetPos.pair = frame.timelineSlot->center;
}

/*
 * Timeline task: spawns converging and orbiting sprites over the effect's steps, fades the screen
 * tint, and for variant 1 moves the whirlwind target along its line.
 */
// FUNCTION: FF7 0x0053177b
void __cdecl BattleFx_SpecialWhirlwindTimelineTask(void)
{
    short level;
    int spawnCount, i, writeIndex, rnd;
    BattleFxWhirlwindOrbit *pOrbit;
    BattleFxWhirlwindWhirlState *whirl;

    if (g_BattleIsPaused != 0) {
        return;
    }
    whirl = (BattleFxWhirlwindWhirlState *)BATTLE_CURRENT_TASK_SLOT;
    if (whirl->step > 1 && whirl->step < 0x2D) {
        writeIndex = g_BattleSpecWhirlwindWhirlwind.writeIndex;
        pOrbit = (BattleFxWhirlwindOrbit *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialWhirlwindConvergingSpritesTask);
        pOrbit->poolStartIndex = writeIndex;
        spawnCount = (g_BattleSpecWhirlwindVariant != 0) + 4;
        for (i = 0; i < spawnCount; i++) {
            rnd = rand() % 0x1000 + 0x800;
            g_BattleSpecWhirlwindWhirlwind.points[writeIndex].x = whirl->posX + rand() % whirl->jitterRange - whirl->jitterOffset;
            g_BattleSpecWhirlwindWhirlwind.points[writeIndex].y = 0;
            g_BattleSpecWhirlwindWhirlwind.points[writeIndex].z = whirl->posZ + rand() % whirl->jitterRange - whirl->jitterOffset;
            g_BattleSpecWhirlwindWhirlwind.points[writeIndex].scale = whirl->baseScale * rnd >> 0xC;
            writeIndex++;
            if (writeIndex >= 0x137) {
                writeIndex = 0;
            }
        }
        g_BattleSpecWhirlwindWhirlwind.writeIndex = writeIndex;
    }
    if (whirl->step > 4 && whirl->step < 0x32) {
        for (i = 0; i < 3; i++) {
            rnd = rand() % 0xA00 + 0xC00;
            pOrbit = (BattleFxWhirlwindOrbit *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialWhirlwindOrbitingSpriteTask);
            pOrbit->position = whirl->position;
            pOrbit->angle = (g_BattleSpecWhirlwindWhirlwind.writeIndex + i & 0xFFF) << 6;
            pOrbit->angleStep = rand() % 0x1C + 0x20;
            pOrbit->scale = whirl->baseScale * rnd >> 0xC;
            if (g_BattleSpecWhirlwindVariant != 0) {
                pOrbit->radius = rand() % 0x6E + 0x20;
                pOrbit->fallSpeed = -(rand() % 0xDC + 0x190);
                pOrbit->orbitSpeed = rand() % 0x50 + 0x64;
            } else {
                pOrbit->radius = rand() % 0xB4 + 0x30;
                pOrbit->fallSpeed = -(rand() % 0x104 + 0x1F4);
                pOrbit->orbitSpeed = rand() % 0x64 + 0x78;
            }
        }
    }
    if (whirl->step < 8) {
        g_BattleFxTimelineState.channel2 = 0;
        g_BattleFxTimelineState.channel1 = g_BattleFxTimelineState.channel2;
        g_BattleFxTimelineState.red = g_BattleFxTimelineState.channel1;
        level = whirl->step << 8;
    } else if (whirl->step >= 0x30) {
        level = 0x800 - (whirl->step - 0x30) * 0xAA;
    } else {
        level = 0x800;
    }
    if (whirl->step == 0x37) {
        for (i = 0; i < 0xA; i++) {
            if ((g_BattleSpecWhirlwindSourceActor & 1 << i) != 0) {
                Battle_PostActorHitOrHurtMessage(i);
            }
        }
    }
    if (whirl->step >= 0x3C) {
        level = 0;
        whirl->ownerSlot = BATTLE_TASK_FREE;
    }
    g_BattleFxTimelineState.startValue = level;
    if (g_BattleSpecWhirlwindVariant != 0) {
        g_BattleSpecWhirlwindTargetOffset.x += (*(short *)&g_BattleSpecWhirlwindStepX);
        g_BattleSpecWhirlwindTargetOffset.y = 0;
        g_BattleSpecWhirlwindTargetOffset.z += (*(short *)&g_BattleSpecWhirlwindStepZ);
        i = SinFixed(whirl->angle);
        whirl->angle += whirl->angularVelocity;
        writeIndex = whirl->stepSize << 3;
        rnd = writeIndex * i >> 0xC;
        whirl->posX = (rnd * g_BattleSpecWhirlwindNormalX >> 0xC) + g_BattleSpecWhirlwindTargetOffset.x;
        whirl->posY = 0;
        whirl->posZ = (rnd * g_BattleSpecWhirlwindNormalZ >> 0xC) + g_BattleSpecWhirlwindTargetOffset.z;
    }
    g_BattleSpecWhirlwindTargetPos.pair = whirl->position;
    (whirl->step)++;
}

/*
 * Draws one orbiting sprite around its centre and advances its fall and spin.
 */
// FUNCTION: FF7 0x00531c10
void __cdecl BattleFx_SpecialWhirlwindOrbitingSpriteTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[52];
        PsxMatrix rotMatrix;
        struct { unsigned char pad_to_20[0x20]; BattleFxWhirlwindOrbitingSpriteSlot *orbitSlot; };
        struct { unsigned char pad_to_24[0x24]; SVECTOR posX; };
        struct { unsigned char pad_to_2C[0x2C]; SVECTOR angleX; };
    } frame;
#pragma pack(pop)

    frame.orbitSlot = (BattleFxWhirlwindOrbitingSpriteSlot *)BATTLE_CURRENT_TASK_SLOT;
    (((BattleFxWhirlwindSpriteReq *)&g_BattleSpecWhirlwindSpriteReq)->frameCounter) = frame.orbitSlot->frameCounter;
    frame.angleX.vx = 0;
    frame.angleX.vy = frame.orbitSlot->fadeLevel;
    frame.angleX.vz = 0;
    RotMatrixXYZ(&frame.angleX, &frame.rotMatrix);
    frame.posX.vx = frame.orbitSlot->posA;
    frame.posX.vy = 0;
    frame.posX.vz = 0;
    TransformVectorToInt16(&frame.rotMatrix, &frame.posX, &frame.posX.vx);
    frame.posX.vx += frame.orbitSlot->centerX;
    frame.posX.vy += frame.orbitSlot->centerY;
    frame.posX.vz += frame.orbitSlot->centerZ;
    BattleFx_InstallBillboardMatrix(&frame.posX, (short)frame.orbitSlot->scale, -(frame.orbitSlot->scale >> 2));
    ((BattleFxWhirlwindSpriteReq *)&g_BattleSpecWhirlwindSpriteReq)->orbitHandle = Gfx_ModelTableGetLoadedRecord(0, g_BattleSpecWhirlwindLoadedResTable);
    (((BattleFxWhirlwindSpriteReq *)&g_BattleSpecWhirlwindSpriteReq)->orbitResource) = Gfx_ModelTableGetSpriteObject(0, g_BattleSpecWhirlwindLoadedResTable);
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitSpriteDraw(&g_BattleSpecWhirlwindSpriteReq, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (frame.orbitSlot->frameCounter >= 0xB) {
        frame.orbitSlot->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.orbitSlot->centerY += frame.orbitSlot->accelB;
    frame.orbitSlot->fadeLevel -= frame.orbitSlot->decayRate;
    frame.orbitSlot->scale += (short)(frame.orbitSlot->scale >> 8);
    frame.orbitSlot->posA += frame.orbitSlot->velA;
    frame.orbitSlot->velA += (short)(frame.orbitSlot->velA >> 4);
    (frame.orbitSlot->frameCounter)++;
}

/*
 * Moves four whirlwind points toward the target and draws them as sprites each step.
 */
// FUNCTION: FF7 0x00531dd5
void __cdecl BattleFx_SpecialWhirlwindConvergingSpritesTask(void)
{
    int idx;
    int q;
    int r;
    VECTOR normal;
    BattleFxWhirlwindOrbit *nodePtr;

    nodePtr = (BattleFxWhirlwindOrbit *)BATTLE_CURRENT_TASK_SLOT;
    (((BattleFxWhirlwindSpriteReq *)&g_BattleSpecWhirlwindSpriteReq)->frame) = nodePtr->frame;
    q = nodePtr->poolStartIndex;
    (((BattleFxWhirlwindSpriteReq *)&g_BattleSpecWhirlwindSpriteReq)->convergeHandle) = Gfx_ModelTableGetLoadedRecord(0, g_BattleSpecWhirlwindLoadedResTable);
    (((BattleFxWhirlwindSpriteReq *)&g_BattleSpecWhirlwindSpriteReq)->convergeResource) = Gfx_ModelTableGetSpriteObject(0, g_BattleSpecWhirlwindLoadedResTable);
    for (idx = 0; idx < 4; idx++) {
        if (g_BattleIsPaused == 0) {
            normal.vx = g_BattleSpecWhirlwindTargetPos.x - g_BattleSpecWhirlwindWhirlwind.points[q].x;
            normal.vy = g_BattleSpecWhirlwindTargetPos.y - g_BattleSpecWhirlwindWhirlwind.points[q].y;
            normal.vz = g_BattleSpecWhirlwindTargetPos.z - g_BattleSpecWhirlwindWhirlwind.points[q].z;
            r = NormalizeVectorFixed(&normal, &normal);
            r = SqrtToInt(r) >> 5;
            g_BattleSpecWhirlwindWhirlwind.points[q].x += (short)(r * normal.vx >> 0xC);
            g_BattleSpecWhirlwindWhirlwind.points[q].y += (short)(r * normal.vy >> 0xC);
            g_BattleSpecWhirlwindWhirlwind.points[q].z += (short)(r * normal.vz >> 0xC);
        }
        BattleFx_InstallBillboardMatrix(&g_BattleSpecWhirlwindWhirlwind.points[q].x, g_BattleSpecWhirlwindWhirlwind.points[q].scale, -(g_BattleSpecWhirlwindWhirlwind.points[q].scale >> 2));
        g_BattleFxActiveDrawListPtr = BattleFx_SubmitSpriteDraw(&((BattleFxWhirlwindSpriteReq *)&g_BattleSpecWhirlwindSpriteReq)->convergeHandle, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
        q++;
        if (q >= 0x137) {
            q = 0;
        }
    }
    if (nodePtr->frame >= 0xB) {
        nodePtr->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    (nodePtr->frame)++;
}
} /* extern "C" */
