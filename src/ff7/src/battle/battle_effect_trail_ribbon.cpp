// src/ff7/src/battle/battle_effect_trail_ribbon.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Effect trail ribbons: a per-frame task computes trail endpoints, appends and fades quad
//   segments, and renders each ribbon; a packed colour divide helper supports the fades.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/api/battle.h"
#include "ff7/battle_tasks.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_actors.h"
#include "lib/types/graphics.h"
#include "ff7/types/battle_fx.h"
#include <stdlib.h>
#include "ff7/camera_script.h"
#include <string.h>
#include "lib/codegen.h"
extern "C" {

#pragma intrinsic(abs)

#pragma intrinsic(memset)
#pragma intrinsic(memcpy)

// GLOBAL: FF7 0x00c05fb8
struct BattleEffectTrailQuadPrim *g_BattleEffectTrail_ResourceAddr = 0;
unsigned char g_bssPad_00c05fbc[4] = {0};
// The previous trail point of each of the two trail rings, selected by g_BattleEffectTrailIndex.
// GLOBAL: FF7 0x00c05fc0
BattleEffectTrailPoint g_BattleEffectTrailPrevPoints[2] = {0};
// GLOBAL: FF7 0x00c05fe0
int g_BattleEffectTrailIndex = 0;
unsigned char g_bssPad_00c05fe4[4] = {0};

/*
 * Divides packed 0x00RRGGBB color channels individually by a divisor.
 */
// FUNCTION: FF7 0x005bf340
int __cdecl Battle_DivideRgb888(int color, int divisor)
{
    int result;

    result = (color & 0xFF0000) / divisor & 0xFF0000 | (color & 0xFF00) / divisor & 0xFF00 | (color & 0xFF) / divisor & 0xFF;
    return result;
}

/*
 * Allocates and initializes an actor effect trail task.
 */
// FUNCTION: FF7 0x005bf388
void __cdecl Battle_CreateEffectTrailTask(int actorIndex, int attachIndex, int yOffset, int colorRgba, int fadeDivisor, int lifetime)
{
    BattleFxEffectTrail *task;

    task = (BattleFxEffectTrail *)&g_BattleTaskSlotBuffer.secondarySlots[(short)Battle_AllocateSecondaryResourceSlot(Battle_EffectTrailTaskHandler)];
    task->alpha = (unsigned int)colorRgba >> 0x18;
    task->actorIndex = actorIndex;
    task->attachIndex = attachIndex;
    task->yOffset = -yOffset;
    task->tailNode = 0;
    task->colorWord = colorRgba & 0xFFFFFF | 0x3A000000;
    task->colorStep = Battle_DivideRgb888(colorRgba, fadeDivisor);
    task->lifetime = lifetime;
    g_BattleEffectTrail_ResourceAddr = (BattleEffectTrailQuadPrim *)&g_GfxPsxPacketScratch;
}

/*
 * Per-frame task computing trail endpoints and appending trail quad segments.
 */
// FUNCTION: FF7 0x005bf41f
void __cdecl Battle_EffectTrailTaskHandler(void)
{
    int scratch;
    BattleFxEffectTrail *trailTaskPtr;
    int attachMatrix[8];
    BattleEffectTrailSegment *segment;
    BattleEffectTrailSeed *trailDataPtr;
    BattleEffectTrailBlock *pTransform;
    BattleEffectTrailSample *sample;

    trailTaskPtr = (BattleFxEffectTrail *)&g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx];
    pTransform = (BattleEffectTrailBlock *)Gfx_GetPsxScratchpadPtr(0);
    trailDataPtr = &pTransform->seed;
    sample = (BattleEffectTrailSample *)(trailDataPtr + 1);
    if (g_BattleIsPaused != 0) {
        return;
    }
    Battle3d_GetActorAttachMatrix(trailTaskPtr->actorIndex, trailTaskPtr->attachIndex, (PsxMatrix *)attachMatrix);
    Battle_ComposeMatrixRelativeToCamera((short *)&g_BattleCameraTransform, attachMatrix, &pTransform->transform);
    SetRotMatrix(&pTransform->transform);
    SetTransMatrix(&pTransform->transform);
    sample->x = pTransform->transform.t[0];
    sample->y = pTransform->transform.t[1];
    sample->z = pTransform->transform.t[2];
    sample->trailX = 0;
    sample->trailY = 0;
    sample->yOffset = trailTaskPtr->yOffset;
    RotTransPoint((SVECTOR *)&sample->trailX, (int *)trailDataPtr, &scratch);
    sample->trailX = trailDataPtr->trailX;
    sample->trailY = trailDataPtr->trailY;
    sample->yOffset = trailDataPtr->yOffset;
    if (trailTaskPtr->alpha == 0 && trailTaskPtr->age >= 2) {
        segment = (BattleEffectTrailSegment *)&g_BattleTaskSlotBuffer.secondarySlots[(short)Battle_AllocateSecondaryResourceSlot(Battle_EffectTrailSegmentTask)];
        segment->posX = (sample->x - g_BattleEffectTrailPrevPoints[g_BattleEffectTrailIndex].posX) / 4 + g_BattleEffectTrailPrevPoints[g_BattleEffectTrailIndex].posX;
        segment->posY = (sample->y - g_BattleEffectTrailPrevPoints[g_BattleEffectTrailIndex].posY) / 4 + g_BattleEffectTrailPrevPoints[g_BattleEffectTrailIndex].posY;
        segment->posZ = (sample->z - g_BattleEffectTrailPrevPoints[g_BattleEffectTrailIndex].posZ) / 4 + g_BattleEffectTrailPrevPoints[g_BattleEffectTrailIndex].posZ;
        segment->targetX = (sample->trailX - g_BattleEffectTrailPrevPoints[g_BattleEffectTrailIndex].trailX) / 4 + g_BattleEffectTrailPrevPoints[g_BattleEffectTrailIndex].trailX;
        segment->targetY = (sample->trailY - g_BattleEffectTrailPrevPoints[g_BattleEffectTrailIndex].trailY) / 4 + g_BattleEffectTrailPrevPoints[g_BattleEffectTrailIndex].trailY;
        segment->targetZ = (sample->yOffset - g_BattleEffectTrailPrevPoints[g_BattleEffectTrailIndex].yOffset) / 4 + g_BattleEffectTrailPrevPoints[g_BattleEffectTrailIndex].yOffset;
        segment->color = trailTaskPtr->colorWord;
        segment->fadeStep = trailTaskPtr->colorStep;
        segment->nextSegment = 0;
        if (trailTaskPtr->tailNode != 0) {
            trailTaskPtr->tailNode->nextSegment = segment;
        }
        trailTaskPtr->tailNode = segment;
    }
    segment = (BattleEffectTrailSegment *)&g_BattleTaskSlotBuffer.secondarySlots[(short)Battle_AllocateSecondaryResourceSlot(Battle_EffectTrailSegmentTask)];
    *(Int64Pair *)&segment->posX = *(Int64Pair *)sample;
    *(Int64Pair *)&segment->targetX = *(Int64Pair *)&sample->trailX;
    *(Int64Pair *)&g_BattleEffectTrailPrevPoints[g_BattleEffectTrailIndex].posX = *(Int64Pair *)sample;
    *(Int64Pair *)&g_BattleEffectTrailPrevPoints[g_BattleEffectTrailIndex].trailX = *(Int64Pair *)&sample->trailX;
    segment->color = trailTaskPtr->colorWord;
    segment->fadeStep = trailTaskPtr->colorStep;
    segment->nextSegment = 0;
    if (trailTaskPtr->tailNode != 0) {
        trailTaskPtr->tailNode->nextSegment = segment;
    }
    trailTaskPtr->tailNode = segment;
    (trailTaskPtr->age)++;
    if (trailTaskPtr->age >= trailTaskPtr->lifetime) {
        trailTaskPtr->alpha = 0xFFFF;
    }
}

/*
 * Updates, projects, fades, and renders an individual trail ribbon quad.
 */
// FUNCTION: FF7 0x005bf7f6
void __cdecl Battle_EffectTrailSegmentTask(void)
{
    BattleEffectTrailQuadPrim *primPtr;
    int clipBits;
    BattleEffectTrailSegment *trailSeg;
    int clipOut;
    int data;
    int colorVal;
    int projState;
    BattleEffectTrailSegment *next;

    trailSeg = (BattleEffectTrailSegment *)&g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx];
    data = Gfx_GetPsxScratchpadPtr(0);
    next = trailSeg->nextSegment;
    if (next != 0) {
        primPtr = g_BattleEffectTrail_ResourceAddr;
        SetRotMatrix(&g_BattleCameraTransform);
        SetTransMatrix(&g_BattleCameraTransform);
        RefreshGlobalTransformState();
        projState = Gfx_ProjectQuadToScreen((SVECTOR *)&trailSeg->posX, (SVECTOR *)&trailSeg->targetX, (SVECTOR *)&next->posX, (SVECTOR *)&next->targetX, &primPtr->xy0, &primPtr->xy1, &primPtr->xy2, &primPtr->xy3, &clipOut, &clipBits);
        if ((clipBits & 0x60000) == 0) {
            primPtr->tag = 0x8000000;
            primPtr->color1 = trailSeg->color;
            primPtr->color0 = primPtr->color1;
            primPtr->color3 = next->color;
            primPtr->color2 = primPtr->color3;
            Gfx_DrawPsxPrimitive(primPtr);
        }
    }
    if (g_BattleIsPaused == 0) {
        if ((trailSeg->color & 0xFFFFFF) == 0) {
            trailSeg->ownerSlot = BATTLE_TASK_FREE;
        }
        colorVal = trailSeg->color & 0xFFFFFF;
        colorVal -= trailSeg->fadeStep;
        if (colorVal < 0) {
            colorVal = 0;
        }
        trailSeg->color = colorVal | 0x3A000000;
        trailSeg->posX += (short)(trailSeg->targetX - trailSeg->posX >> 3);
        trailSeg->posY += (short)(trailSeg->targetY - trailSeg->posY >> 3);
        trailSeg->posZ += (short)(trailSeg->targetZ - trailSeg->posZ >> 3);
    }
}
} /* extern "C" */
