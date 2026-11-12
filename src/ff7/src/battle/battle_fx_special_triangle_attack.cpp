// src/ff7/src/battle/battle_fx_special_triangle_attack.cpp
/* Enemy Special Effect [idx 39]: TriangleAttack (JP: triangle / デルタアタック (Triangle Attack), asset: "SPECIAL/TRIANGLE") — Triangle Attack (0x345..0x347, Mover) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle special effect for the Triangle attack: loads its resources, spawns per-target
//   electric triangles, and drives the growing and spinning triangle models plus pulsing and
//   rotated electric sprites.

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

struct GfxRotationAngles2;
struct BattleFxModelDraw;
#pragma intrinsic(memset)
#pragma intrinsic(memcpy)

// clang-format off
// GLOBAL: FF7 0x008a6b20
unsigned int g_BattleSpecialTriangleAttackResourceDefs[105] ={
    0x001900b1,
    (unsigned int)&g_BattleSpecialTriangleAttackResFilenameEletri1,
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x008a6dcc
    (unsigned int)"eletri2.rsd",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x008a6dd8
    (unsigned int)"eletri3.rsd",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x008a6de4
    (unsigned int)"eletri4.rsd",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x008a6df0
    (unsigned int)"eletri5.rsd",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x008a6dfc
    (unsigned int)"eletri6.rsd",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x008a6e08
    (unsigned int)"tri.rsd",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x008a6e10
    (unsigned int)"kaku.rsd",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x008a6e1c
    (unsigned int)"hikari.rsd",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x008a6e28
    (unsigned int)"elec1.rsd",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x008a6e34
    (unsigned int)"elec2.rsd",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x008a6e40
    (unsigned int)"elec3.rsd",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x008a6e4c
    (unsigned int)"elec4.rsd",
    0x00000000,
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x008a6e58
    (unsigned int)"tri01.tim",
    // STRING: FF7 0x008a6e64
    (unsigned int)"hit1.spt",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x008a6e70
    (unsigned int)"tri01.tim",
    // STRING: FF7 0x008a6e7c
    (unsigned int)"hit2.spt",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x008a6e88
    (unsigned int)"tri01.tim",
    // STRING: FF7 0x008a6e94
    (unsigned int)"hit3.spt",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x008a6ea0
    (unsigned int)"tri01.tim",
    // STRING: FF7 0x008a6eac
    (unsigned int)"hit4.spt",
    0x00000000,
    0x00000000,
    0x00192a39,
    0x00000000,
    // STRING: FF7 0x008a6eb8
    (unsigned int)"jo_c01.spt",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x008a6ec4
    (unsigned int)"tri02.tim",
    // STRING: FF7 0x008a6ed0
    (unsigned int)"t_1.spt",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x008a6ed8
    (unsigned int)"tri02.tim",
    // STRING: FF7 0x008a6ee4
    (unsigned int)"t_2.spt",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x008a6eec
    (unsigned int)"tri01.tim",
    // STRING: FF7 0x008a6ef8
    (unsigned int)"kou.spt"
};
// GLOBAL: FF7 0x008a6cc8
struct BattleSpecialTriangleAttackSpriteReq1 g_BattleSpecialTriangleAttackSpriteReq1 = {0, 0, 96, 96, 96, {0x2c}, 0, {0}};
// GLOBAL: FF7 0x008a6cd8
int g_BattleSpecialTriangleAttackModelIndices1[6] = {13, 14, 15, 16, 17, 0};
// GLOBAL: FF7 0x008a6cf0
struct BattleSpecialTriangleAttackModelReq1 g_BattleSpecialTriangleAttackModelReq1 = {0, {0xa8, 0, 0, 0, 0, 0}, 128, {0x20, 0, 0, 0}};
// GLOBAL: FF7 0x008a6d00
struct GfxRotationAngles2 g_BattleSpecialTriangleAttackRotAngles1 = {-1024, 0, 0};
// GLOBAL: FF7 0x008a6d08
struct BattleSpecialTriangleAttackModelReq2 g_BattleSpecialTriangleAttackModelReq2 = {0, {0x28, 0, 0, 0, 0, 0}, 0, {0x20, 0, 0, 0}};
// GLOBAL: FF7 0x008a6d18
struct BattleSpecialTriangleAttackModelReq1 g_BattleSpecialTriangleAttackModelReq3 = {0, {0x88, 0, 0, 0, 0, 0}, 0, {0x20, 0, 0, 0}};
// GLOBAL: FF7 0x008a6d28
int g_BattleSpecialTriangleAttackModelIndices2[6] = {0, 1, 2, 3, 4, 5};
// GLOBAL: FF7 0x008a6d40
struct BattleSpecialTriangleAttackModelReq1 g_BattleSpecialTriangleAttackModelReq4 = {0, {0x88, 0, 0, 0, 0, 0}, 0, {0x20, 0, 0, 0}};
// GLOBAL: FF7 0x008a6d50
struct BattleSpecialTriangleAttackSpriteReq2 g_BattleSpecialTriangleAttackSpriteReq2 = {0, 0, {0x80, 0x80, 0x80, 0x2e}, 0, {0}};
// GLOBAL: FF7 0x008a6d60
int g_BattleSpecialTriangleAttackModelIndices3[2] = {18, 19};
// GLOBAL: FF7 0x008a6d68
unsigned char g_BattleSpecialTriangleAttackRotAngles2[6] = {0x00, 0x30, 0x00, 0x00, 0x00, 0x20};
// GLOBAL: FF7 0x008a6d70
int g_BattleSpecialTriangleAttackScale = 12288;
// GLOBAL: FF7 0x008a6d78
struct BattleSpecialTriangleAttackModelReq2 g_BattleSpecialTriangleAttackModelReq5 = {0, {0x28, 0, 0, 0, 0, 0}, 0, {0x20, 0, 0, 0}};
// GLOBAL: FF7 0x008a6d88
int g_BattleSpecialTriangleAttackModelIndices4[4] = {9, 10, 11, 12};
// GLOBAL: FF7 0x008a6d98
struct BattleSpecialTriangleAttackSpriteReq3 g_BattleSpecialTriangleAttackSpriteReq3 = {0, 0, {0x80, 0x80, 0x80, 0x2e}, 0};
// GLOBAL: FF7 0x008a6da8
short g_BattleSpecialTriangleAttackVertexIndices[12] = {2, 8, 5, 0, 3, 10, 1, 4, 6, 11, 7, 9};
// GLOBAL: FF7 0x008a6dc0
unsigned char g_BattleSpecialTriangleAttackResFilenameEletri1[12] = "eletri1.rsd";
// clang-format on


// clang-format off
// GLOBAL: FF7 0x00b6f058
int g_BattleSpecialTriangleAttackDrawListHandle = 0;
// GLOBAL: FF7 0x00b6f05c
struct GfxTableCursor *g_BattleSpecialTriangleAttackLoadedRes = 0;
// GLOBAL: FF7 0x00b6f060
short g_BattleSpecialTriangleAttackAlphaParam1 = 0;
// GLOBAL: FF7 0x00b6f064
short g_BattleSpecialTriangleAttackAlphaParam2 = 0;
// clang-format on

/*
 * Entry point thunk for the Triangle special effect.
 */
// FUNCTION: FF7 0x0054fa50
void __cdecl BattleFx_SpecialEnterTriangleAttack(int targetMask, int actorIdx)
{
    BattleFx_SpecialBeginTriangleAttack(targetMask, actorIdx);
}

/*
 * Loads and registers resources for the Triangle special effect.
 */
// FUNCTION: FF7 0x0054fa65
void __cdecl BattleFx_SpecialLoadResTriangleAttack(void)
{
    GfxModelTableBank *stageSceneHandle;
    GfxResourceRequest requestObj;
    int resourceCount;
    char pathWorkBuffer[200];
    GfxResourceRequest *reqPtrPtr;
    int resourceTable;

    if (g_pBattle3dScene != 0 && g_BattleSpecialTriangleAttackLoadedRes == 0) {
        resourceCount = 0x15;
        resourceTable = (int)&g_BattleSpecialTriangleAttackResourceDefs;
        stageSceneHandle = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        reqPtrPtr = &requestObj;
        Gfx_InitResourceRequest(reqPtrPtr);
        reqPtrPtr->groupTag = 0;
        // STRING: FF7 0x008a6f00
        reqPtrPtr->resourcePath = BattleFx_BuildResourcePath("SPECIAL/TRIANGLE", pathWorkBuffer);
        Gfx_LoadModelTableIfAbsent(resourceCount, resourceTable, reqPtrPtr, &g_BattleSpecialTriangleAttackLoadedRes, stageSceneHandle);
    }
}

/*
 * Initializes resources and starts target tasks for Triangle.
 */
// FUNCTION: FF7 0x0054faf5
void __cdecl BattleFx_SpecialBeginTriangleAttack(int targetMask, int actorIdx)
{
    BattleFx_SpecialLoadResTriangleAttack();
    Battle_CreateBitPatternCallbackTask(targetMask, actorIdx, 4, BattleFx_SpecialSpawnTriangleAttackPerTarget);
    Battle_PlaySfxPannedToActor(0xFF, (short)actorIdx, 0);
}

/*
 * Spawns per-target Triangle effect tasks and creates timeline task.
 */
// FUNCTION: FF7 0x0054fb2a
void __cdecl BattleFx_SpecialSpawnTriangleAttackPerTarget(int actor, int casterIndex)
{
    BattleFxTriangleAttackRecord *effect;

    effect = (BattleFxTriangleAttackRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialTriangleAttackTimelineTask);
    Battle_GetBoneWorldPos(casterIndex, g_BattleActorRecords[casterIndex].targetBone, &effect->originX);
    Battle_GetBoneWorldPos(actor, g_BattleActorRecords[actor].targetBone, &effect->targetX);
    effect->actorIndex = actor;
}

/*
 * Main timeline task managing rotating electric triangle models and child tasks.
 */
// FUNCTION: FF7 0x0054fba0
void __cdecl BattleFx_SpecialTriangleAttackTimelineTask(void)
{
    int p; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int q; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int r; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int s; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int t; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int u; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int v; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int w; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int x; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int y; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int z; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxTriangleAttackRecord *a;
    short burstIndex;
    BattleFxTriangleAttackTrack *curTrack;
    int d; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int e; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int f; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int g; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int h; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */

    a = (BattleFxTriangleAttackRecord *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (a->frame == 0) {
        curTrack = (BattleFxTriangleAttackTrack *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialTriangleAttackSpinModelsTask);
        curTrack->prevX = 0;
        curTrack->prevY = 0xFDA8;
        curTrack->prevZ = 0xF95C;
        curTrack->stepX = (a->targetX - curTrack->prevX) / 5;
        curTrack->stepY = (-0x7D0 - curTrack->prevY) / 5;
        curTrack->stepZ = (a->targetZ - curTrack->prevZ) / 5;
        curTrack->scale = 0x6000;
        curTrack->spinPhase = 0;
    }
    if (a->frame == 0x26) {
        curTrack = (BattleFxTriangleAttackTrack *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialTriangleAttackCounterSpinModelsTask);
        curTrack->prevX = a->targetX;
        curTrack->prevY = a->targetY;
        curTrack->prevZ = a->targetZ;
        curTrack = (BattleFxTriangleAttackTrack *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialTriangleAttackPulsingSpriteSpawnTask);
        curTrack->prevX = a->targetX;
        curTrack->prevY = a->targetY;
        curTrack->prevZ = a->targetZ;
        g_BattleFxTimelineState.channel2 = 0x20;
        g_BattleFxTimelineState.channel1 = g_BattleFxTimelineState.channel2;
        g_BattleFxTimelineState.red = g_BattleFxTimelineState.channel1;
        Battle_StartFixedPointRamp(0x1000, 0xA);
    }
    if (a->frame == 0x3C) {
        curTrack = (BattleFxTriangleAttackTrack *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialTriangleAttackGrowingModelTask);
        curTrack->prevX = a->targetX;
        curTrack->prevY = a->targetY;
        curTrack->prevZ = a->targetZ;
    }
    if (a->frame == 0x3F) {
        Battle_PostActorHitOrHurtMessage(a->actorIndex);
        Battle_PlaySfxPannedToActor(0x95, (short)a->actorIndex, 0);
    }
    if (a->frame > 0x3A && a->frame < 0x3F) {
        curTrack = (BattleFxTriangleAttackTrack *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialTriangleAttackVariantSpriteTask);
        curTrack->prevX = a->targetX;
        curTrack->prevY = a->targetY;
        curTrack->prevZ = a->targetZ;
        curTrack->prevX += (short)(rand() % 0x258 - 0x12C);
        curTrack->prevY += (short)(rand() % 0x258 - 0x12C);
        curTrack->prevZ += (short)(rand() % 0x258 - 0x12C);
        curTrack->variant = rand() % 4;
        for (burstIndex = 0; burstIndex < 2; burstIndex++) {
            curTrack = (BattleFxTriangleAttackTrack *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialTriangleAttackVariantSpriteTask);
            curTrack->prevX = a->targetX;
            curTrack->prevY = a->targetY;
            curTrack->prevZ = a->targetZ;
            curTrack->prevX += (short)(rand() % 0x640 - 0x320);
            curTrack->prevY += (short)(rand() % 0x640 - 0x320);
            curTrack->prevZ += (short)(rand() % 0x640 - 0x320);
            curTrack->variant = 4;
        }
    }
    (a->frame)++;
    if (a->frame == 0x50) {
        Battle_StartFixedPointRamp(0, 0xA);
        a->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Animates variant billboard sprites for Triangle.
 */
// FUNCTION: FF7 0x0054ffc9
void __cdecl BattleFx_SpecialTriangleAttackVariantSpriteTask(void)
{
    BattleFxTriangleAttackSpriteTask *task;

    task = (BattleFxTriangleAttackSpriteTask *)BATTLE_CURRENT_TASK_SLOT;
    BattleFx_InstallBillboardMatrix(&task->posX, (task->variant - 4 != 0 ? 0 : 0) + 0x2000, 0);
    g_BattleSpecialTriangleAttackSpriteReq1.frame = task->frame;
    g_BattleSpecialTriangleAttackSpriteReq1.colorB = (task->variant - 4 != 0 ? 0x20 : 0) + 0x40;
    g_BattleSpecialTriangleAttackSpriteReq1.colorG = g_BattleSpecialTriangleAttackSpriteReq1.colorB;
    g_BattleSpecialTriangleAttackSpriteReq1.colorR = g_BattleSpecialTriangleAttackSpriteReq1.colorG;
    (g_BattleSpecialTriangleAttackSpriteReq1.animTable) = Gfx_ModelTableGetLoadedRecord(g_BattleSpecialTriangleAttackModelIndices1[task->variant], g_BattleSpecialTriangleAttackLoadedRes);
    if (task->variant != 4) {
        g_BattleSpecialTriangleAttackSpriteReq1.drawObj = Gfx_ModelTableGetSpriteObject(g_BattleSpecialTriangleAttackModelIndices1[task->variant], g_BattleSpecialTriangleAttackLoadedRes);
    } else {
        g_BattleSpecialTriangleAttackSpriteReq1.drawObj = Battle3d_GetSceneRenderObject(3, g_pBattle3dScene);
    }
    g_BattleSpecialTriangleAttackDrawListHandle = BattleFx_SubmitSpriteDraw(&g_BattleSpecialTriangleAttackSpriteReq1, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialTriangleAttackDrawListHandle);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if ((g_BattleSpecialTriangleAttackSpriteReq1.animTable) != 0) {
        (task->frame)++;
        if (task->frame >= *(int *)((char *)*(int *)(g_BattleSpecialTriangleAttackSpriteReq1.animTable) + 4)) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Animates growing electric triangle models.
 */
// FUNCTION: FF7 0x0055011d
void __cdecl BattleFx_SpecialTriangleAttackGrowingModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[76];
        int depth;
        struct { unsigned char pad_to_4[0x4]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_24[0x24]; PsxMatrix combinedMatrix; };
        struct { unsigned char pad_to_44[0x44]; short componentIndex; };
        struct { unsigned char pad_to_48[0x48]; BattleTaskSlot *growSlot; };
    } frame;
#pragma pack(pop)

    frame.growSlot = BATTLE_CURRENT_TASK_SLOT;
    (g_BattleSpecialTriangleAttackModelReq1.model) = Gfx_ModelTableGetRenderObject(8, g_BattleSpecialTriangleAttackLoadedRes);
    for (frame.componentIndex = 0; frame.componentIndex < 3; frame.componentIndex++) {
        frame.combinedMatrix.m[2][frame.componentIndex] = 0;
        frame.combinedMatrix.m[1][frame.componentIndex] = 0;
        frame.combinedMatrix.m[0][frame.componentIndex] = 0;
    }
    SetRotMatrix(&g_BattleCameraTransform);
    SetTransMatrix(&g_BattleCameraTransform);
    RotTransPoint(frame.growSlot->data, frame.combinedMatrix.t, &frame.depth);
    frame.combinedMatrix.m[2][2] = frame.growSlot->frameCounter * 0x258 + 0x800;
    frame.combinedMatrix.m[1][1] = frame.combinedMatrix.m[2][2];
    frame.combinedMatrix.m[0][0] = frame.combinedMatrix.m[1][1];
    RotMatrix((SVECTOR *)&g_BattleSpecialTriangleAttackRotAngles1, &frame.rotMatrix);
    ConcatMatrixIntoFirst(&frame.combinedMatrix, &frame.rotMatrix);
    SetRotMatrix(&frame.combinedMatrix);
    SetTransMatrix(&frame.combinedMatrix);
    SetDrawColorRGB(0, 0, 0);
    g_BattleSpecialTriangleAttackModelReq1.scale = frame.growSlot->frameCounter << 8;
    g_BattleSpecialTriangleAttackDrawListHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecialTriangleAttackModelReq1, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialTriangleAttackDrawListHandle);
    if (g_BattleIsPaused != 0) {
        return;
    }
    (frame.growSlot->frameCounter)++;
    if (frame.growSlot->frameCounter == 0x10) {
        frame.growSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Animates clockwise spinning electric triangle models.
 */
// FUNCTION: FF7 0x00550297
void __cdecl BattleFx_SpecialTriangleAttackSpinModelsTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[180];
        PsxMatrix modelMatrix;
        struct { unsigned char pad_to_20[0x20]; SVECTOR spinAngleX; };
        struct { unsigned char pad_to_28[0x28]; PsxMatrix localMatrix; };
        struct { unsigned char pad_to_48[0x48]; PsxMatrix baseMatrix; };
        struct { unsigned char pad_to_6C[0x6C]; short triangleIndex; };
        struct { unsigned char pad_to_70[0x70]; SVECTOR rotX; };
        struct { unsigned char pad_to_78[0x78]; BattleFxTriangleAttackSpinModelsSlot *spinSlot; };
        struct { unsigned char pad_to_A4[0xA4]; VECTOR scale; };
    } frame;
#pragma pack(pop)

    frame.spinSlot = (BattleFxTriangleAttackSpinModelsSlot *)BATTLE_CURRENT_TASK_SLOT;
    (g_BattleSpecialTriangleAttackModelReq3.model) = Gfx_ModelTableGetRenderObject(6, g_BattleSpecialTriangleAttackLoadedRes);
    (g_BattleSpecialTriangleAttackModelReq4.model) = Gfx_ModelTableGetRenderObject(7, g_BattleSpecialTriangleAttackLoadedRes);
    if (frame.spinSlot->frameCounter >= 0xF || (frame.spinSlot->frameCounter % 2) == 0) {
        frame.spinAngleX.vx = frame.spinSlot->spinPhase;
        frame.spinAngleX.vy = frame.spinSlot->spinPhase + 0x800;
        frame.spinAngleX.vz = 0;
        RotMatrix(&frame.spinAngleX, &frame.baseMatrix);
        frame.scale.vz = frame.spinSlot->timeLeft;
        frame.scale.vy = frame.scale.vz;
        frame.scale.vx = frame.scale.vy;
        ScaleMatrix(&frame.baseMatrix, &frame.scale);
        frame.baseMatrix.t[0] = frame.spinSlot->spinX;
        frame.baseMatrix.t[1] = frame.spinSlot->spinY;
        frame.baseMatrix.t[2] = frame.spinSlot->spinZ;
        MultiplyMatrix(&g_BattleCameraTransform, &frame.baseMatrix, &frame.localMatrix);
        SetRotMatrix(&frame.localMatrix);
        SetTransMatrix(&frame.localMatrix);
        SetDrawColorRGB(0, 0, 0);
        g_BattleSpecialTriangleAttackModelReq2.alpha = g_BattleSpecialTriangleAttackAlphaParam1;
        (g_BattleSpecialTriangleAttackModelReq2.model) = Gfx_ModelTableGetRenderObject(g_BattleSpecialTriangleAttackModelIndices2[frame.spinSlot->frameCounter % 6], g_BattleSpecialTriangleAttackLoadedRes);
        g_BattleSpecialTriangleAttackDrawListHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecialTriangleAttackModelReq2, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialTriangleAttackDrawListHandle);
        g_BattleSpecialTriangleAttackModelReq3.scale = 0x1000 - (g_BattleSpecialTriangleAttackAlphaParam1 << 5);
        g_BattleSpecialTriangleAttackDrawListHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecialTriangleAttackModelReq3, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialTriangleAttackDrawListHandle);
        g_BattleSpecialTriangleAttackModelReq4.scale = 0x1000 - (g_BattleSpecialTriangleAttackAlphaParam1 << 5);
        g_BattleSpecialTriangleAttackDrawListHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecialTriangleAttackModelReq4, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialTriangleAttackDrawListHandle);
        frame.spinAngleX.vx = frame.spinSlot->spinPhase;
        frame.spinAngleX.vy = frame.spinSlot->spinPhase + 0x800;
        frame.spinAngleX.vz = 0;
        RotMatrix(&frame.spinAngleX, &frame.baseMatrix);
        for (frame.triangleIndex = 0; frame.triangleIndex < 3; frame.triangleIndex++) {
            frame.rotX.vx = 0x4D8;
            frame.rotX.vy = frame.triangleIndex * 0x555;
            frame.rotX.vz = 0;
            RotMatrix(&frame.rotX, &frame.localMatrix);
            ConcatMatrixPreservingTranslation(&frame.baseMatrix, &frame.localMatrix, &frame.modelMatrix);
            ScaleMatrix(&frame.modelMatrix, &frame.scale);
            frame.modelMatrix.t[0] = frame.spinSlot->spinX;
            frame.modelMatrix.t[1] = frame.spinSlot->spinY;
            frame.modelMatrix.t[2] = frame.spinSlot->spinZ;
            MultiplyMatrix(&g_BattleCameraTransform, &frame.modelMatrix, &frame.modelMatrix);
            SetRotMatrix(&frame.modelMatrix);
            SetTransMatrix(&frame.modelMatrix);
            g_BattleSpecialTriangleAttackModelReq4.scale = 0x1000 - (g_BattleSpecialTriangleAttackAlphaParam1 << 5);
            g_BattleSpecialTriangleAttackDrawListHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecialTriangleAttackModelReq4, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialTriangleAttackDrawListHandle);
        }
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.spinSlot->frameCounter < 0xF) {
        g_BattleSpecialTriangleAttackAlphaParam1 += 8;
    }
    if (frame.spinSlot->frameCounter >= 0xF && frame.spinSlot->frameCounter < 0x14) {
        frame.spinSlot->spinX += frame.spinSlot->spinVelocityX;
        frame.spinSlot->spinY += frame.spinSlot->spinVelocityY;
        frame.spinSlot->spinZ += frame.spinSlot->spinVelocityZ;
        frame.spinSlot->spinPhase += 0x80;
    }
    if (frame.spinSlot->frameCounter >= 0x14 && frame.spinSlot->frameCounter < 0x19) {
        frame.spinSlot->spinPhase += 0x80;
    }
    if (frame.spinSlot->frameCounter >= 0x19) {
        frame.spinSlot->spinPhase += 0x100;
        frame.spinSlot->timeLeft -= 0x7D0;
        frame.spinSlot->spinY += 0x50;
        g_BattleSpecialTriangleAttackAlphaParam1 -= 4;
    }
    (frame.spinSlot->frameCounter)++;
    if (frame.spinSlot->frameCounter == 0x28) {
        frame.spinSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Animates counter-clockwise spinning electric triangle models.
 */
// FUNCTION: FF7 0x0055073b
void __cdecl BattleFx_SpecialTriangleAttackCounterSpinModelsTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[44];
        SVECTOR angleX;
        struct { unsigned char pad_to_8[0x8]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_28[0x28]; BattleFxOilTargetModelSlot *slot; };
    } frame;
#pragma pack(pop)

    frame.slot = (BattleFxOilTargetModelSlot *)BATTLE_CURRENT_TASK_SLOT;
    frame.angleX.vy = frame.slot->frameCounter << 6;
    frame.angleX.vz = 0;
    frame.angleX.vx = frame.angleX.vz;
    RotMatrixXYZ(&frame.angleX, &frame.rotMatrix);
    g_BattleSpecialTriangleAttackScale = 0x3000;
    (*(int *)&g_BattleSpecialTriangleAttackRotAngles2) = g_BattleSpecialTriangleAttackScale;
    ScaleMatrix(&frame.rotMatrix, (int *)&g_BattleSpecialTriangleAttackRotAngles2);
    frame.rotMatrix.t[0] = frame.slot->posX;
    frame.rotMatrix.t[1] = 0;
    frame.rotMatrix.t[2] = frame.slot->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.rotMatrix);
    SetRotMatrix(&frame.rotMatrix);
    SetTransMatrix(&frame.rotMatrix);
    g_BattleSpecialTriangleAttackModelReq5.alpha = g_BattleSpecialTriangleAttackAlphaParam2 << 2;
    (g_BattleSpecialTriangleAttackModelReq5.model) = Gfx_ModelTableGetRenderObject(g_BattleSpecialTriangleAttackModelIndices4[(frame.slot->frameCounter % 4)], g_BattleSpecialTriangleAttackLoadedRes);
    g_BattleSpecialTriangleAttackDrawListHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecialTriangleAttackModelReq5, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialTriangleAttackDrawListHandle);
    frame.angleX.vy = -frame.slot->frameCounter << 6;
    frame.angleX.vz = 0;
    frame.angleX.vx = frame.angleX.vz;
    RotMatrixXYZ(&frame.angleX, &frame.rotMatrix);
    g_BattleSpecialTriangleAttackScale = 0x5000;
    (*(int *)&g_BattleSpecialTriangleAttackRotAngles2) = g_BattleSpecialTriangleAttackScale;
    ScaleMatrix(&frame.rotMatrix, (int *)&g_BattleSpecialTriangleAttackRotAngles2);
    frame.rotMatrix.t[0] = frame.slot->posX;
    frame.rotMatrix.t[1] = 0;
    frame.rotMatrix.t[2] = frame.slot->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.rotMatrix);
    SetRotMatrix(&frame.rotMatrix);
    SetTransMatrix(&frame.rotMatrix);
    g_BattleSpecialTriangleAttackDrawListHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecialTriangleAttackModelReq5, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialTriangleAttackDrawListHandle);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.slot->frameCounter < 8) {
        g_BattleSpecialTriangleAttackAlphaParam2++;
    }
    if (frame.slot->frameCounter > 0xC) {
        g_BattleSpecialTriangleAttackAlphaParam2--;
    }
    (frame.slot->frameCounter)++;
    if (frame.slot->frameCounter == 0x14) {
        frame.slot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Task that spawns pulsing electric sprites around targets.
 */
// FUNCTION: FF7 0x0055097f
void __cdecl BattleFx_SpecialTriangleAttackPulsingSpriteSpawnTask(void)
{
    BattleFxTriangleAttackPulseSprite *pRotated;
    BattleFxTriangleAttackPulseSprite *pulse;

    pulse = (BattleFxTriangleAttackPulseSprite *)BATTLE_CURRENT_TASK_SLOT;
    (g_BattleSpecialTriangleAttackSpriteReq3.sprite) = Gfx_ModelTableGetLoadedRecord(0x14, g_BattleSpecialTriangleAttackLoadedRes);
    g_BattleSpecialTriangleAttackSpriteReq3.drawObj = Gfx_ModelTableGetSpriteObject(0x14, g_BattleSpecialTriangleAttackLoadedRes);
    BattleFx_InstallBillboardMatrix(&pulse->posX, SinFixed(pulse->pulsePhase * 0x66) << 0xE >> 0xC, 0);
    g_BattleSpecialTriangleAttackSpriteReq3.pulseShort12 = pulse->pulsePhase % 2;
    g_BattleSpecialTriangleAttackDrawListHandle = BattleFx_SubmitSpriteDraw(&g_BattleSpecialTriangleAttackSpriteReq3, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecialTriangleAttackDrawListHandle);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (pulse->pulsePhase < 0xC) {
        pRotated = (BattleFxTriangleAttackPulseSprite *)&g_BattleTaskSlotBuffer.secondarySlots[(short)Battle_AllocateSecondaryResourceSlot(BattleFx_SpecialTriangleAttackRotatedSpriteTask)];
        pRotated->posX = pulse->posX;
        pRotated->posY = pulse->posY;
        pRotated->posZ = pulse->posZ;
        pRotated->angleY = 0;
        pRotated->angleX = 0;
        pRotated->angleZ = g_BattleSpecialTriangleAttackVertexIndices[pulse->pulsePhase] * 0x154 + rand() % 0xC8 + -0x64;
        pRotated->modelIndex = rand() % 2;
    }
    (pulse->pulsePhase)++;
    if (pulse->pulsePhase == 0x14) {
        pulse->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Animates rotated pulsing electric sprites.
 */
// FUNCTION: FF7 0x00550b14
void __cdecl BattleFx_SpecialTriangleAttackRotatedSpriteTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[84];
        struct { unsigned char pad_to_8[0x8]; PsxMatrix slotRotMatrix; };
        struct { unsigned char pad_to_28[0x28]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_48[0x48]; short axisIndex; };
        struct { unsigned char pad_to_4C[0x4C]; int projectedDepth; };
        struct { unsigned char pad_to_50[0x50]; BattleFxTriangleAttackRotatedSpriteSlot *slot; };
    } frame;
#pragma pack(pop)

    frame.slot = (BattleFxTriangleAttackRotatedSpriteSlot *)&g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx];
    for (frame.axisIndex = 0; frame.axisIndex < 3; frame.axisIndex++) {
        frame.rotMatrix.m[2][frame.axisIndex] = 0;
        frame.rotMatrix.m[1][frame.axisIndex] = 0;
        frame.rotMatrix.m[0][frame.axisIndex] = 0;
    }
    SetRotMatrix(&g_BattleCameraTransform);
    SetTransMatrix(&g_BattleCameraTransform);
    RotTransPoint((SVECTOR *)&frame.slot->posX, frame.rotMatrix.t, &frame.projectedDepth);
    frame.rotMatrix.m[2][2] = 0x3000;
    frame.rotMatrix.m[1][1] = frame.rotMatrix.m[2][2];
    frame.rotMatrix.m[0][0] = frame.rotMatrix.m[1][1];
    RotMatrix((SVECTOR *)&frame.slot->angleX, &frame.slotRotMatrix);
    ConcatMatrixIntoFirst(&frame.rotMatrix, &frame.slotRotMatrix);
    SetRotMatrix(&frame.rotMatrix);
    SetTransMatrix(&frame.rotMatrix);
    g_BattleSpecialTriangleAttackSpriteReq2.frame = frame.slot->frameCounter;
    (g_BattleSpecialTriangleAttackSpriteReq2.animTable) = Gfx_ModelTableGetLoadedRecord(g_BattleSpecialTriangleAttackModelIndices3[frame.slot->modelIndex], g_BattleSpecialTriangleAttackLoadedRes);
    g_BattleSpecialTriangleAttackSpriteReq2.drawObj = Gfx_ModelTableGetSpriteObject(g_BattleSpecialTriangleAttackModelIndices3[frame.slot->modelIndex], g_BattleSpecialTriangleAttackLoadedRes);
    g_BattleOverlayDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecialTriangleAttackSpriteReq2, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if ((g_BattleSpecialTriangleAttackSpriteReq2.animTable) != 0) {
        (frame.slot->frameCounter)++;
        if (frame.slot->frameCounter >= *(int *)((char *)*(int *)(g_BattleSpecialTriangleAttackSpriteReq2.animTable) + 4)) {
            frame.slot->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}
} /* extern "C" */
