// src/ff7/src/battle/battle_fx_limit_gauntlet.cpp
// Limit Break [53] (Yuffie Lv 3-1): Gauntlet (Japanese: 鎧袖一触, dev tag: "LIMIT2/ISSHOKU" / Isshoku)
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Isshoku limit effect: loads its resources and targets an actor mask, with a growing and
//   scaling model plus a Gouraud line task that draws the effect's lines.
/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/types/battle.h"
#include "ff7/types/battle_limit.h"
#include "ff7/battle_tasks.h"
#include "lib/types/graphics.h"
#include "lib/codegen.h"
#include "lib/types/polygon.h"
#include "ff7/camera_script.h"
#include <stdlib.h>
#include <string.h>
#include "ff7/api/battle.h"
#include "ff7/api/menu.h"
#include "lib/api/sound.h"
#include "lib/api/graphics.h"
extern "C" {

#pragma intrinsic(memset)

// clang-format off
unsigned char g_rawData_00806478[16] = {0x30, 0x01, 0x00, 0x00, 0x45, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x00806488
int g_BattleLimitGauntletSoundGroup[2] = {8414328, 0};
// GLOBAL: FF7 0x00806490
int g_BattleLimitGauntletResourceDef = 0x001940B1;
char *g_BattleFxLimitGauntletResourceNames[1] = {
    // STRING: FF7 0x008064d8
    "ISSHOKU.RSD"
};
unsigned int g_BattleLimitGauntletResourceDefDataTail[8] = {
    0x00000000,
    0x00000000,
    0x00000000,
    0x001940b1,
    // STRING: FF7 0x008064e4
    (unsigned int)"ISSHOKU2.RSD",
    0x00000000,
    0x00000000,
    0x00000000
};
// GLOBAL: FF7 0x008064b8
BattleLimitIsshokuModelDrawRequest g_BattleLimitGauntletModelDrawRequest = {0, 0xa8, 0, 0, 0x20, 0, 0xa8, 0, 0};
unsigned char g_rawData_008064d4[4] = {0x20, 0x00, 0x00, 0x00};
// clang-format on

#pragma intrinsic(memcpy)
#pragma function(memcpy)

// clang-format off
// GLOBAL: FF7 0x00aef788
short g_BattleLimitGauntletScaleAlpha = 0;
unsigned char g_bssPad_00aef78c[4] = {0};
// GLOBAL: FF7 0x00aef790
int g_BattleLimitGauntletCentroidX = 0;
// GLOBAL: FF7 0x00aef794
short g_BattleLimitGauntletCentroidZ = 0;
// GLOBAL: FF7 0x00aef798
int g_BattleLimitGauntletGouraudLineDrawHandle = 0;
// GLOBAL: FF7 0x00aef79c
short g_BattleLimitGauntletFadeStep2 = 0;
// GLOBAL: FF7 0x00aef7a0
short g_BattleLimitGauntletTargetMask = 0;
// GLOBAL: FF7 0x00aef7a4
short g_BattleLimitGauntletFadeStep1 = 0;
// GLOBAL: FF7 0x00aef7a8
struct GfxTableCursor *g_BattleLimitGauntletLoadedResTable = 0;
unsigned char g_bssPad_00aef7ac[4] = {0};
// GLOBAL: FF7 0x00aef7b0
short g_BattleLimitGauntletRotationY1[3] = {0};
// GLOBAL: FF7 0x00aef7b8
short g_BattleLimitGauntletRotationY2[3] = {0};
// clang-format on

// When scene pointer is nonzero and g_BattleLimitGauntletLoadedResTable is zero, initializes local
// resource record with default duration, clears first field and builds LIMIT2/ISSHOKU path into
// field eight. Calls Gfx_LoadModelTableIfAbsent with type two, descriptor
// g_BattleLimitGauntletResourceDef, local record, cache g_BattleLimitGauntletLoadedResTable and scene
// field 0x4C, then registers g_BattleLimitGauntletSoundGroup sound groups within the same guard.
// Otherwise does nothing. Helpers determine allocation/failure behavior; registration has no
// separate success check.
// FUNCTION: FF7 0x004ad950
void __cdecl BattleFx_LimitLoadResGauntlet(void)
{
    GfxModelTableBank *newSceneField;
    GfxResourceRequest requestSlot;
    int kind;
    char pathText[200];
    GfxResourceRequest *resourcePtrPtr;
    int stepResourceDef;

    if (g_pBattle3dScene != 0 && g_BattleLimitGauntletLoadedResTable == 0) {
        kind = 2;
        stepResourceDef = (int)&g_BattleLimitGauntletResourceDef;
        newSceneField = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        resourcePtrPtr = &requestSlot;
        Gfx_InitResourceRequest(resourcePtrPtr);
        resourcePtrPtr->groupTag = 0;
        // STRING: FF7 0x008064f4
        resourcePtrPtr->resourcePath = (char *)BattleFx_BuildResourcePath("LIMIT2/ISSHOKU", pathText);
        Gfx_LoadModelTableIfAbsent(kind, stepResourceDef, resourcePtrPtr, &g_BattleLimitGauntletLoadedResTable, newSceneField);
        SfxRegisterSoundGroups((struct SoundGroup **)&g_BattleLimitGauntletSoundGroup);
    }
}

// Loads Isshoku resources, points g_BattleLimitGauntletGouraudLineDrawHandle at g_GfxPsxPacketScratch, clears g_BattleLimitGauntletFadeStep1 and
// g_BattleLimitGauntletFadeStep2, sets g_BattleLimitGauntletScaleAlpha to 0x800, calls BattleFx_LimitBeginGauntlet, and returns zero.
// FUNCTION: FF7 0x004ad9f1
int __cdecl BattleFx_LimitEnterGauntlet(int targetMask, int actorIdx)
{
    BattleFx_LimitLoadResGauntlet();
    g_BattleLimitGauntletGouraudLineDrawHandle = (int)&g_GfxPsxPacketScratch;
    g_BattleLimitGauntletFadeStep1 = 0;
    g_BattleLimitGauntletScaleAlpha = 0x800;
    g_BattleLimitGauntletFadeStep2 = 0;
    BattleFx_LimitBeginGauntlet(targetMask, actorIdx);
    return 0;
}

// Stores arg1 in g_BattleLimitGauntletTargetMask, writes its actor-mask centroid to g_BattleLimitGauntletCentroidX, allocates a slot
// for BattleFx_LimitGauntletTimelineTask, and stores actorIdx in the task record.
// FUNCTION: FF7 0x004ada32
void __cdecl BattleFx_LimitBeginGauntlet(int targetMask, int actorIdx)
{
    BattleFxLimitGauntletTaskRecord *slot;

    g_BattleLimitGauntletTargetMask = targetMask;
    Battle_GetActorMaskCentroid(targetMask, (short *)&g_BattleLimitGauntletCentroidX);
    slot = (BattleFxLimitGauntletTaskRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitGauntletTimelineTask);
    slot->actorIdx = actorIdx;
}

// Returns while paused. At twenty plays sound group zero panned from
// g_BattleLimitGauntletTargetMask; at thirty schedules BattleFx_LimitGauntletScalingModelTask at
// retained position g_BattleLimitGauntletCentroidX, sets screen RGB (0,0,64) and starts 4096 ramp
// for ten. From twenty through seventy-nine emits integer (80 minus count) divided by ten
// BattleFx_LimitGauntletGouraudLineTask tasks per call at that position with random Y angle and zero
// X/Z angles. At forty schedules BattleFx_LimitGauntletGrowingModelTask at the same position. At
// eighty allocates typed records for set bits among ten actors in g_BattleLimitGauntletTargetMask.
// Increments count and exactly at one hundred retires, starts ten-count zero ramp and releases
// sounds 0x130/0x145 slot fifteen; the same global serves pan-helper argument and actor mask.
// FUNCTION: FF7 0x004ada7c
void __cdecl BattleFx_LimitGauntletTimelineTask(void)
{
    int p;
    int r;
    int b;
    short i9;
    int s;
    int u;
    BattleFxLimitGauntletTaskRecord *childTaskPtr;
    int v;
    int x;
    int y;
    int z;
    int a;
    BattleTaskSlot *taskPtr;

    taskPtr = BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (taskPtr->frameCounter == 0x14) {
        SfxPlaySoundGroup(Battle_ComputePanForEntity(g_BattleLimitGauntletTargetMask), 0);
    }
    if (taskPtr->frameCounter == 0x1E) {
        childTaskPtr = (BattleFxLimitGauntletTaskRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitGauntletScalingModelTask);
        childTaskPtr->centroidXLow = ((BattleLimitGauntletCentroidX *)&g_BattleLimitGauntletCentroidX)->centroidXLow;
        childTaskPtr->centroidXHigh = ((BattleLimitGauntletCentroidX *)&g_BattleLimitGauntletCentroidX)->centroidXHigh;
        childTaskPtr->centroidZ = g_BattleLimitGauntletCentroidZ;
        g_BattleFxTimelineState.channel1 = 0;
        g_BattleFxTimelineState.red = g_BattleFxTimelineState.channel1;
        g_BattleFxTimelineState.channel2 = 0x40;
        Battle_StartFixedPointRamp(0x1000, 0xA);
    }
    if (taskPtr->frameCounter >= 0x14 && taskPtr->frameCounter < 0x50) {
        for (i9 = 0; i9 < (0x50 - taskPtr->frameCounter) / 0xA; i9++) {
            childTaskPtr = (BattleFxLimitGauntletTaskRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitGauntletGouraudLineTask);
            childTaskPtr->centroidXLow = ((BattleLimitGauntletCentroidX *)&g_BattleLimitGauntletCentroidX)->centroidXLow;
            childTaskPtr->centroidXHigh = ((BattleLimitGauntletCentroidX *)&g_BattleLimitGauntletCentroidX)->centroidXHigh;
            childTaskPtr->centroidZ = g_BattleLimitGauntletCentroidZ;
            childTaskPtr->rotX = 0;
            childTaskPtr->angle = rand() % 0x1000;
            childTaskPtr->rotZ = 0;
        }
    }
    if (taskPtr->frameCounter == 0x28) {
        childTaskPtr = (BattleFxLimitGauntletTaskRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitGauntletGrowingModelTask);
        childTaskPtr->centroidXLow = ((BattleLimitGauntletCentroidX *)&g_BattleLimitGauntletCentroidX)->centroidXLow;
        childTaskPtr->centroidXHigh = ((BattleLimitGauntletCentroidX *)&g_BattleLimitGauntletCentroidX)->centroidXHigh;
        childTaskPtr->centroidZ = g_BattleLimitGauntletCentroidZ;
    }
    if (taskPtr->frameCounter == 0x50) {
        for (i9 = 0; i9 < 0xA; i9++) {
            if ((g_BattleLimitGauntletTargetMask & 1 << i9) != 0) {
                Battle_PostActorHitOrHurtMessage(i9);
            }
        }
    }
    (taskPtr->frameCounter)++;
    if (taskPtr->frameCounter == 0x64) {
        taskPtr->ownerSlot = BATTLE_TASK_FREE;
        Battle_StartFixedPointRamp(0, 0xA);
        Sfx_ReleaseCachedBuffer(0x130, 0xF);
        Sfx_ReleaseCachedBuffer(0x145, 0xF);
    }
}

// Writes retained Y angle as task count shifted seven, builds XYZ rotation and uniform scale from
// g_BattleLimitGauntletScaleAlpha, sets saved XYZ translation and composes g_BattleCameraTransform.
// Refreshes Isshoku resource field zero, installs matrix, sets black draw colour and writes
// descriptor short ten as 4096 minus g_BattleLimitGauntletFadeStep1 shifted nine. Submits model at
// destination plus 0x70 shift twelve, chaining g_BattleLimitGauntletGouraudLineDrawHandle. After
// paused return, first thirty counts increase retained scale by 600; later subtract 300 from task
// Y. First eight increment retained descriptor counter and counts greater than forty-two decrement
// it. Increments task count and exactly fifty retires.
// FUNCTION: FF7 0x004add05
void __cdecl BattleFx_LimitGauntletGrowingModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[84];
        PsxMatrix rotMatrix;
        struct { unsigned char pad_to_20[0x20]; BattleFxCosmoMemoryExpandingModel *growRecord; };
        struct { unsigned char pad_to_24[0x24]; PsxMatrix drawMatrix; };
        struct { unsigned char pad_to_44[0x44]; VECTOR scaleX; };
    } frame;
#pragma pack(pop)

    frame.growRecord = (BattleFxCosmoMemoryExpandingModel *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleLimitGauntletRotationY1[1] = frame.growRecord->frame << 7;
    RotMatrixXYZ(&g_BattleLimitGauntletRotationY1, &frame.rotMatrix);
    frame.scaleX.vz = g_BattleLimitGauntletScaleAlpha;
    frame.scaleX.vy = frame.scaleX.vz;
    frame.scaleX.vx = frame.scaleX.vy;
    g_BattleLimitGauntletModelDrawRequest.isshokuRes = Gfx_ModelTableGetRenderObject(0, g_BattleLimitGauntletLoadedResTable);
    ScaleMatrix(&frame.rotMatrix, &frame.scaleX);
    frame.rotMatrix.t[0] = frame.growRecord->posX;
    frame.rotMatrix.t[1] = frame.growRecord->posY;
    frame.rotMatrix.t[2] = frame.growRecord->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.drawMatrix);
    SetRotMatrix(&frame.drawMatrix);
    SetTransMatrix(&frame.drawMatrix);
    SetDrawColorRGB(0, 0, 0);
    g_BattleLimitGauntletModelDrawRequest.reqAngle1 = 0x1000 - (g_BattleLimitGauntletFadeStep1 << 9);
    g_BattleLimitGauntletGouraudLineDrawHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleLimitGauntletModelDrawRequest, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleLimitGauntletGouraudLineDrawHandle);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.growRecord->frame < 0x1E) {
        g_BattleLimitGauntletScaleAlpha += 0x258;
    } else {
        frame.growRecord->posY -= 0x12C;
    }
    if (frame.growRecord->frame < 8) {
        g_BattleLimitGauntletFadeStep1++;
    }
    if (frame.growRecord->frame > 0x2A) {
        g_BattleLimitGauntletFadeStep1--;
    }
    (frame.growRecord->frame)++;
    if (frame.growRecord->frame == 0x32) {
        frame.growRecord->ownerSlot = BATTLE_TASK_FREE;
    }
}

// Refreshes Isshoku resource field one. Writes retained Y angle as task count shifted five, builds
// XYZ rotation and uniform scale count times 200 plus 2048, sets saved XYZ translation and
// composes g_BattleCameraTransform. Installs matrix, sets black draw colour and writes descriptor short ten
// as 4096 minus g_BattleLimitGauntletFadeStep2 shifted eight. Submits model at destination plus 0x70 shift twelve,
// chaining g_BattleLimitGauntletGouraudLineDrawHandle. After paused return, first eight counts raise shared descriptor counter
// by two; from forty-four onward lower it by one. Increments task count and exactly sixty retires.
// FUNCTION: FF7 0x004adeb3
void __cdecl BattleFx_LimitGauntletScalingModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[84];
        PsxMatrix rotMatrix;
        struct { unsigned char pad_to_20[0x20]; BattleFxCosmoMemoryTriSpin *scaleRecord; };
        struct { unsigned char pad_to_24[0x24]; PsxMatrix drawMatrix; };
        struct { unsigned char pad_to_44[0x44]; VECTOR scaleX; };
    } frame;
#pragma pack(pop)

    frame.scaleRecord = (BattleFxCosmoMemoryTriSpin *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleLimitGauntletModelDrawRequest.reqModel = Gfx_ModelTableGetRenderObject(1, g_BattleLimitGauntletLoadedResTable);
    g_BattleLimitGauntletRotationY2[1] = frame.scaleRecord->step << 5;
    RotMatrixXYZ(&g_BattleLimitGauntletRotationY2, &frame.rotMatrix);
    frame.scaleX.vz = frame.scaleRecord->step * 0xC8 + 0x800;
    frame.scaleX.vy = frame.scaleX.vz;
    frame.scaleX.vx = frame.scaleX.vy;
    ScaleMatrix(&frame.rotMatrix, &frame.scaleX);
    frame.rotMatrix.t[0] = frame.scaleRecord->posX;
    frame.rotMatrix.t[1] = frame.scaleRecord->posY;
    frame.rotMatrix.t[2] = frame.scaleRecord->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.drawMatrix);
    SetRotMatrix(&frame.drawMatrix);
    SetTransMatrix(&frame.drawMatrix);
    SetDrawColorRGB(0, 0, 0);
    g_BattleLimitGauntletModelDrawRequest.reqAngle2 = 0x1000 - (g_BattleLimitGauntletFadeStep2 << 8);
    g_BattleLimitGauntletGouraudLineDrawHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleLimitGauntletModelDrawRequest.reqModel, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleLimitGauntletGouraudLineDrawHandle);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.scaleRecord->step < 8) {
        g_BattleLimitGauntletFadeStep2 += 2;
    }
    if (frame.scaleRecord->step >= 0x2C) {
        g_BattleLimitGauntletFadeStep2--;
    }
    (frame.scaleRecord->step)++;
    if (frame.scaleRecord->step == 0x3C) {
        frame.scaleRecord->ownerSlot = BATTLE_TASK_FREE;
    }
}

// Builds saved task rotation/XYZ translation, installs/refreshes it and transforms two in-place
// short endpoint vectors along local Z at (ten minus count) times 800 and times 200. Installs
// g_BattleCameraTransform, refreshes and uses retained g_BattleLimitGauntletGouraudLineDrawHandle primitive with four-word header and
// command 0x52 via constant-true blend-bit branch, retaining constant-false clearing branch.
// Projects first endpoint and only when helper returns positive sets first RGB black, second RGB
// 128, projects second endpoint and submits. Projection flags and copied command local are unused.
// After drawing returns paused; active count increments and exactly ten retires.
// FUNCTION: FF7 0x004ae03c
void __cdecl BattleFx_LimitGauntletGouraudLineTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[72];
        struct { unsigned char pad_to_3[0x3]; char copiedCommand; };
        struct { unsigned char pad_to_4[0x4]; int projFlags; };
        struct { unsigned char pad_to_8[0x8]; SVECTOR end2X; };
        struct { unsigned char pad_to_10[0x10]; SVECTOR end1X; };
        struct { unsigned char pad_to_18[0x18]; int depth; };
        struct { unsigned char pad_to_1C[0x1C]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_3C[0x3C]; int projected; };
        struct { unsigned char pad_to_40[0x40]; BattleFxLimitCosmoMemoryGrowState *lineState; };
        struct { unsigned char pad_to_44[0x44]; BattleGpuGouraudLine *gouraudLine; };
    } frame;
#pragma pack(pop)

    frame.lineState = (BattleFxLimitCosmoMemoryGrowState *)BATTLE_CURRENT_TASK_SLOT;
    RotMatrix((SVECTOR *)((char *)frame.lineState + 0xC), &frame.rotMatrix);
    frame.rotMatrix.t[0] = frame.lineState->posX;
    frame.rotMatrix.t[1] = frame.lineState->posY;
    frame.rotMatrix.t[2] = frame.lineState->posZ;
    SetRotMatrix(&frame.rotMatrix);
    SetTransMatrix(&frame.rotMatrix);
    RefreshGlobalTransformState();
    frame.end1X.vz = (0xA - frame.lineState->frameIndex) * 0x320;
    frame.end1X.vy = 0;
    frame.end1X.vx = frame.end1X.vy;
    RotTransPointToInt16(&frame.end1X, (short *)&frame.end1X, &frame.projFlags);
    frame.end2X.vz = (0xA - frame.lineState->frameIndex) * 0xC8;
    frame.end2X.vy = 0;
    frame.end2X.vx = frame.end2X.vy;
    RotTransPointToInt16(&frame.end2X, (short *)&frame.end2X, &frame.projFlags);
    SetRotMatrix(&g_BattleCameraTransform);
    SetTransMatrix(&g_BattleCameraTransform);
    RefreshGlobalTransformState();
    frame.gouraudLine = (BattleGpuGouraudLine *)g_BattleLimitGauntletGouraudLineDrawHandle;
    frame.gouraudLine->tag = frame.gouraudLine->tag & 0xFFFFFF | 0x4000000;
    frame.gouraudLine->command = 0x50;
    if (1 != 0) { /* Constant condition kept for /Od branch layout; see lib/codegen.h. */
        frame.gouraudLine->command = *(unsigned char *)&frame.gouraudLine->command | 2;
        frame.copiedCommand = frame.gouraudLine->command;
    } else {
        frame.gouraudLine->command = *(unsigned char *)&frame.gouraudLine->command & 0xFFFFFFFD;
        frame.copiedCommand = frame.gouraudLine->command;
    }
    frame.depth = Gfx_TransformVertexSlot0ReturnDepth(&frame.end1X, &frame.projected, &frame.projFlags);
    if (frame.depth > 0) {
        frame.gouraudLine->colorB0 = 0;
        frame.gouraudLine->colorG0 = 0;
        frame.gouraudLine->colorR0 = 0;
        frame.gouraudLine->colorB1 = 0x80;
        frame.gouraudLine->colorG1 = 0x80;
        frame.gouraudLine->colorR1 = 0x80;
        Gfx_TransformVertexSlot1ReturnDepth((GfxPsxShortVertexSlot *)&frame.end2X, &frame.projected, &frame.projFlags);
        Gfx_DrawPsxPrimitive(frame.gouraudLine);
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    (frame.lineState->frameIndex)++;
    if (frame.lineState->frameIndex == 0xA) {
        frame.lineState->ownerSlot = BATTLE_TASK_FREE;
    }
}
} /* extern "C" */
