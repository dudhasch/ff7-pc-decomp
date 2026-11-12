// src/ff7/src/battle/battle_fx_special_avalanche.cpp
/* Enemy Special Effect [idx 33]: Avalanche (JP: nadare / 雪崩 (Avalanche), asset: "SPECIAL/NADARE") — Avalanche (0x2a5, Lessaloploth) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle special effect 'Nadare': a per-target timeline that sorts the face order and
//   draws per-face offset models, with drifting animated sprites, a growing model and static
//   sprites.

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

struct BattleFxFreezeBreathDrawPrim;
struct BattleFxFreezeBreathDrawRecord;
struct GfxPsxSpriteQuadState;
struct BattleFxModelDraw;
#pragma intrinsic(memset)
#pragma intrinsic(memcpy)

// clang-format off

// GLOBAL: FF7 0x0089ebd0
unsigned int g_BattleSpecAvalancheResourceDefs[35] = {0x001940b4, (unsigned int)&g_BattleSpecAvalancheRsdNames, 0x00000000, 0x00000000, 0x00000000, 0x001940b1, (unsigned int)((char *)&g_BattleSpecAvalancheRsdNames + 0xc), 0x00000000, 0x00000000, 0x00000000, 0x00192a39, (unsigned int)&g_BattleSpecAvalancheBakuNames, (unsigned int)((char *)&g_BattleSpecAvalancheBakuNames + 0xc), 0x00000000, 0x00000000, 0x00192a39, (unsigned int)&g_BattleSpecAvalancheHitNames, (unsigned int)((char *)&g_BattleSpecAvalancheHitNames + 0xc), 0x00000000, 0x00000000, 0x00192a39, 0x00000000, (unsigned int)&g_BattleSpecAvalancheJoNames, 0x00000000, 0x00000000, 0x00197c3c, (unsigned int)((char *)&g_BattleSpecAvalancheJoNames + 0xc), 0x00000000, 0x00000000, 0x00000000, 0x00197c39, (unsigned int)&g_BattleSpecAvalancheKoNames, 0x00000000, 0x00000000, 0x00000000};
// GLOBAL: FF7 0x0089ec60
BattleAvalancheMeshData g_BattleSpecAvalancheMeshWorking = {{160, 184, 0xfe35, 296, 377, 0xff4b, 452, 0xfe1e, 67, 0xfed4, 0xfe82, 484, 0xffc9, -0x2f10000, 0, 0x2740000, 0, -0x1e0feb8, 0xffc9, -0x1e0ff31, 277, -0x1e00087, 306, -0x1e0014c, 39, -0x1e000c7, 0xfef0, -0x1e0ff7d, 0xfed8, 0x1640071, 0xfec5, 0x164015a, 0xffdb, 0x16400c3, 268, 0x164ff7d, 308, 0x164feb1, 36, 0x164ff43, 0xfefd, 0x2d0024, 152, 0x800070, 0x78489821, -0x68a39aa2, 0x200058, 0x800018, 0x7848609a, 0x5d59bd58, 0x280068, 0x800000, 0x78482461, 0x655e66bd, 0x100048, 0x800008, 0x78489b5d, 0x59605700, 0x900020, 0x800018, 0x7848bd58, 0x5d596027, 96, 0x800020, 0x7848241e, 0x6600655e, 0x80040, 0x800028, 0x784899a0, 0x57bd5960, 0x180050, 0x800010, 0x78481f9a, 0x585d59, 0x700028, 0x800000, 0x784866bd, 0x655e975c, 0x780008, 0x800028, 0x78485960, 0x57bd26a0, 0x800010, 0x800008, 0x78485700, 0x5960275b, 0x880018, 0x800010, 0x78485d59, 0x581f26, 0x980000, 0x800020, 0x7848655e, 0x66009821, 0x980090, 0x800038, 0x78486027, 0x60009e26, 0x880090, 0x800018, 0x78486027, 0x5d591f26, 0x800088, 0x800010, 0x7848261b, 0x5700275b, 0x780080, 0x800008, 0x7848275b, 0x596026a0, 0x700078, 0x800028, 0x784898a3, 0x66bd975c, 0x580050, 0x800018, 0x78481f9a, 0x5d59609a, 0x480050, 0x800030, 0x7848991b, -0x41a264a3, 0x480040, 0x800008, 0x784899a0, 0x59609b5d, 0x680040, 0x800030, 0x7848249e, 0x5e2461, 0x680060, 0x800000, 0x7848241e, 0x655e2461, 0x580060, 0x800030, 0x7848a29a, 0x60be609a, 0x200090, 0x800098, 0x78486027, -0x61d942a8, 0x500058, 0x800030, 0x7848609a, 0x60be1f9a, 0x480010, 0x800050, 0x78485700, -0x66e464a3, 0x400048, 0x800030, 0x78489b5d, -0x41a26660, 0x680028, 0x800040, 0x784866bd, 0x249e2461, 0x600068, 0x800030, 0x78482461, 0x5e241e, 0x700098, 0x800038, 0x78489821, -0x41a168a4, 0x600058, 0x800020, 0x7848609a, -0x42a75d66, 0x900088, 0x800038, 0x78481f26, 0x60006027, 0x880080, 0x800038, 0x7848275b, 0x5d261b, 0x800078, 0x800038, 0x784826a0, 0x5d275b, 0x780070, 0x800038, 0x7848975c, -0x41a1675d, 0, 0, 0}};
// GLOBAL: FF7 0x0089ef58
BattleAvalancheMeshData g_BattleSpecAvalancheMeshSource = {{160, 184, 0xfe35, 296, 377, 0xff4b, 452, 0xfe1e, 67, 0xfed4, 0xfe82, 484, 0xffc9, -0x2f10000, 0, 0x2740000, 0, -0x1e0feb8, 0xffc9, -0x1e0ff31, 277, -0x1e00087, 306, -0x1e0014c, 39, -0x1e000c7, 0xfef0, -0x1e0ff7d, 0xfed8, 0x1640071, 0xfec5, 0x164015a, 0xffdb, 0x16400c3, 268, 0x164ff7d, 308, 0x164feb1, 36, 0x164ff43, 0xfefd, 0x2d0024, 152, 0x800070, 0x78489821, -0x68a39aa2, 0x200058, 0x800018, 0x7848609a, 0x5d59bd58, 0x280068, 0x800000, 0x78482461, 0x655e66bd, 0x100048, 0x800008, 0x78489b5d, 0x59605700, 0x900020, 0x800018, 0x7848bd58, 0x5d596027, 96, 0x800020, 0x7848241e, 0x6600655e, 0x80040, 0x800028, 0x784899a0, 0x57bd5960, 0x180050, 0x800010, 0x78481f9a, 0x585d59, 0x700028, 0x800000, 0x784866bd, 0x655e975c, 0x780008, 0x800028, 0x78485960, 0x57bd26a0, 0x800010, 0x800008, 0x78485700, 0x5960275b, 0x880018, 0x800010, 0x78485d59, 0x581f26, 0x980000, 0x800020, 0x7848655e, 0x66009821, 0x980090, 0x800038, 0x78486027, 0x60009e26, 0x880090, 0x800018, 0x78486027, 0x5d591f26, 0x800088, 0x800010, 0x7848261b, 0x5700275b, 0x780080, 0x800008, 0x7848275b, 0x596026a0, 0x700078, 0x800028, 0x784898a3, 0x66bd975c, 0x580050, 0x800018, 0x78481f9a, 0x5d59609a, 0x480050, 0x800030, 0x7848991b, -0x41a264a3, 0x480040, 0x800008, 0x784899a0, 0x59609b5d, 0x680040, 0x800030, 0x7848249e, 0x5e2461, 0x680060, 0x800000, 0x7848241e, 0x655e2461, 0x580060, 0x800030, 0x7848a29a, 0x60be609a, 0x200090, 0x800098, 0x78486027, -0x61d942a8, 0x500058, 0x800030, 0x7848609a, 0x60be1f9a, 0x480010, 0x800050, 0x78485700, -0x66e464a3, 0x400048, 0x800030, 0x78489b5d, -0x41a26660, 0x680028, 0x800040, 0x784866bd, 0x249e2461, 0x600068, 0x800030, 0x78482461, 0x5e241e, 0x700098, 0x800038, 0x78489821, -0x41a168a4, 0x600058, 0x800020, 0x7848609a, -0x42a75d66, 0x900088, 0x800038, 0x78481f26, 0x60006027, 0x880080, 0x800038, 0x7848275b, 0x5d261b, 0x800078, 0x800038, 0x784826a0, 0x5d275b, 0x780070, 0x800038, 0x7848975c, -0x41a1675d, 0, 0, 0}};
// GLOBAL: FF7 0x0089f24c
short g_BattleSpecAvalancheDirectionSign = 1;
// GLOBAL: FF7 0x0089f250
struct BattleFxSpriteRequest g_BattleSpecAvalancheSpriteReq1 = {0, 0, -128, -128, -128, 0x2c, 0, 0};
// GLOBAL: FF7 0x0089f260
unsigned char g_BattleSpecAvalancheScaleVector[16] = {0x00, 0x14, 0x00, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x0089f270
unsigned int g_BattleSpecAvalancheModelDrawReq[4] = {(unsigned int)&g_BattleSpecAvalancheMeshWorking, 0x000000a8, 0x00800000, 0x00000020};
// GLOBAL: FF7 0x0089f280
struct BattleFxSpriteRequest g_BattleSpecAvalancheSpriteReq3 = {0, 0, -128, -128, -128, 0x2c, 0, 0};
// GLOBAL: FF7 0x0089f290
struct BattleFxSpriteRequest g_BattleSpecAvalancheSpriteReq2 = {0, 0, -128, -128, -128, 0x2c, 0, 0};
// GLOBAL: FF7 0x0089f2a0
unsigned char g_BattleSpecAvalancheModelReq[16] = {0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x20, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x0089f2b0
unsigned char g_BattleSpecAvalancheRsdNames[24] =
    "NADARE1.RSD\0"
    "NADARE1.RSD";
// GLOBAL: FF7 0x0089f2c8
unsigned char g_BattleSpecAvalancheBakuNames[22] =
    "BAKU1.TIM\0\0\0"
    "BAKU1.SPT";
// GLOBAL: FF7 0x0089f2e0
unsigned char g_BattleSpecAvalancheHitNames[21] =
    "HIT1.TIM\0\0\0\0"
    "HIT1.SPT";
// GLOBAL: FF7 0x0089f2f8
unsigned char g_BattleSpecAvalancheJoNames[21] =
    "JO_A.SPT\0\0\0\0"
    "KO-2.TIM";
// GLOBAL: FF7 0x0089f310
unsigned char g_BattleSpecAvalancheKoNames[9] = "KO-2.TIM";
// clang-format on


// clang-format off
// GLOBAL: FF7 0x00b5ddd8
short g_BattleSpecAvalancheFaceActiveFlags[128] = {0};
// GLOBAL: FF7 0x00b5ded8
unsigned char g_BattleSpecAvalancheFaceIndex[16] = {0};
unsigned char g_bssPad_00b5dee8[1016] = {0};
// GLOBAL: FF7 0x00b5e2e0
int g_BattleSpecAvalancheScale = 0;
// GLOBAL: FF7 0x00b5e2e4
int g_BattleSpecAvalancheScaleY = 0;
// GLOBAL: FF7 0x00b5e2e8
int g_BattleSpecAvalancheScaleZ = 0;
unsigned char g_bssPad_00b5e2ec[4] = {0};
// GLOBAL: FF7 0x00b5e2f0
int g_BattleSpecAvalancheDrawListHead = 0;
// GLOBAL: FF7 0x00b5e2f4
short g_BattleSpecAvalancheTargetMask = 0;
// GLOBAL: FF7 0x00b5e2f8
short g_BattleSpecAvalancheFaceOrder[128] = {0};
// GLOBAL: FF7 0x00b5e3f8
struct GfxTableCursor *g_BattleSpecAvalancheLoadedResTable = 0;
unsigned char g_bssPad_00b5e3fc[4] = {0};
// GLOBAL: FF7 0x00b5e400
int g_BattleSpecAvalancheRotationVector = 0;
unsigned char g_bssPad_00b5e404[4] = {0};
unsigned char g_bssPad_00b5e408[4] = {0};
// clang-format on

/*
 * Loads the SPECIAL/NADARE resource table once the 3D scene exists.
 */
// FUNCTION: FF7 0x00528ff0
void __cdecl BattleFx_SpecialLoadResAvalanche(void)
{
    GfxModelTableBank *curOwnerCtx;
    GfxResourceRequest currentRecord;
    int kind;
    char outSize[200];
    GfxResourceRequest *pRequest;
    int resourceDefsWord;

    if (g_pBattle3dScene != 0 && g_BattleSpecAvalancheLoadedResTable == 0) {
        kind = 7;
        resourceDefsWord = (int)&g_BattleSpecAvalancheResourceDefs;
        curOwnerCtx = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRequest = &currentRecord;
        Gfx_InitResourceRequest(pRequest);
        pRequest->groupTag = 0;
        // STRING: FF7 0x0089f31c
        pRequest->resourcePath = BattleFx_BuildResourcePath("SPECIAL/NADARE", outSize);
        Gfx_LoadModelTableIfAbsent(kind, resourceDefsWord, pRequest, &g_BattleSpecAvalancheLoadedResTable, curOwnerCtx);
    }
}

/*
 * Resets the face mesh from its source copy, loads resources, seeds the scale vector and starts the effect.
 */
// FUNCTION: FF7 0x00529080
void __cdecl BattleFx_SpecialEnterAvalanche(int targetMask, int actorIdx)
{
    *&g_BattleSpecAvalancheMeshWorking = *&g_BattleSpecAvalancheMeshSource;
    BattleFx_SpecialLoadResAvalanche();
    g_BattleSpecAvalancheDirectionSign = 1;
    g_BattleSpecAvalancheScale = 0x2000 /* 2.0 */;
    g_BattleSpecAvalancheScaleY = 0;
    g_BattleSpecAvalancheScaleZ = 0x5000 /* 5.0 */;
    (*(short *)&g_BattleSpecAvalancheFaceIndex) = 0;
    BattleFx_SpecialBeginAvalanche(targetMask, actorIdx);
}

/*
 * Starts the timeline task: stores the caster's bone position and the target centroid, flips the
 * direction sign when the caster's Z exceeds the targets' Z, sorts the faces and plays the sound.
 */
// FUNCTION: FF7 0x005290df
void __cdecl BattleFx_SpecialBeginAvalanche(int targetMask, int actorIdx)
{
    BattleFxAvalancheTaskSlot *taskSlot;

    taskSlot = (BattleFxAvalancheTaskSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialAvalancheTimelineTask);
    taskSlot->actorIndex = actorIdx;
    Battle_GetBoneWorldPos(actorIdx, g_BattleActorRecords[actorIdx].bone34, &taskSlot->posX);
    Battle_GetActorMaskCentroid(targetMask, &taskSlot->targetX);
    if (taskSlot->posZ > taskSlot->targetZ) {
        g_BattleSpecAvalancheDirectionSign *= -1;
    }
    g_BattleSpecAvalancheTargetMask = targetMask;
    BattleFx_SpecialAvalancheSortFaceOrder();
    Battle_PlaySfxPannedToActor(0x18B, (short)actorIdx, 0x40);
}

/*
 * Orders the 36 mesh faces by depth into g_BattleSpecAvalancheFaceOrder.
 */
// FUNCTION: FF7 0x0052918c
void __cdecl BattleFx_SpecialAvalancheSortFaceOrder(void)
{
    int facePointer;
    short chosen;
    int meshAddress;
    short maxDepth;
    int table;
    short pass, faceIndex;

    for (pass = 0; pass < 36; pass++) {
        g_BattleSpecAvalancheFaceActiveFlags[pass] = 0;
        *(short *)(((char *)&g_BattleSpecAvalancheFaceIndex + 12) + pass * 8) = 0;
        *(short *)(((char *)&g_BattleSpecAvalancheFaceIndex + 10) + pass * 8) = 0;
        *(short *)(((char *)&g_BattleSpecAvalancheFaceIndex + 8) + pass * 8) = 0;
    }
    meshAddress = (int)((char *)&g_BattleSpecAvalancheMeshWorking + 0x4);
    table = meshAddress + (*(int *)&g_BattleSpecAvalancheMeshWorking);
    table += 4;
    for (pass = 0; pass < 36; pass++) {
        maxDepth = 0x8000;
        for (faceIndex = 0; faceIndex < 36; faceIndex++) {
            if (g_BattleSpecAvalancheFaceActiveFlags[faceIndex] == 0) {
                facePointer = meshAddress + *(short *)((char *)table + (faceIndex << 3) * 2);
                if (*(short *)((char *)facePointer + 2) > maxDepth) {
                    maxDepth = *(short *)((char *)facePointer + 2);
                    chosen = faceIndex;
                }
            }
        }
        g_BattleSpecAvalancheFaceOrder[pass] = chosen;
        g_BattleSpecAvalancheFaceActiveFlags[chosen] = 1;
    }
    for (pass = 0; pass < 36; pass++) {
        g_BattleSpecAvalancheFaceActiveFlags[pass] = 0;
    }
}

/*
 * Timeline of the Nadare effect: spawns the growing model, the per-target face models, the hit sprites and the typed records.
 */
// FUNCTION: FF7 0x005292e7
void __cdecl BattleFx_SpecialAvalancheTimelineTask(void)
{
    int p; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxAvalancheTimelineSlot *pCur;
    int r; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    short i9;
    BattleFxAvalancheTimeline *sprite;
    int u; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int v; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int w; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int x; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int y; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int z; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int a; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int b; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int c; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int d; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int e; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int f; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int g; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int h; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int j; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int k; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int m; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */

    pCur = (BattleFxAvalancheTimelineSlot *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (pCur->timelineStep == 0) {
        sprite = (BattleFxAvalancheTimeline *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialAvalancheGrowingModelTask);
        sprite->offsetX = 0;
        sprite->offsetZ = g_BattleSpecAvalancheDirectionSign * 0xFFFFE000;
        sprite->offsetY = 0;
        sprite->directionOffset = g_BattleSpecAvalancheDirectionSign * 1000;
    }
    if (pCur->timelineStep > 86 && pCur->timelineStep < 94) {
        for (i9 = 0; i9 < 10; i9++) {
            if ((g_BattleSpecAvalancheTargetMask & 1 << i9) != 0) {
                sprite = (BattleFxAvalancheTimeline *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialAvalancheStaticAnimatedSpriteTask);
                Battle_GetBoneWorldPos(i9, g_BattleActorRecords[i9].targetBone, &sprite->offsetX);
                sprite->offsetX += (short)(rand() % 600 - 300);
                sprite->offsetY += (short)(rand() % 600 - 300);
                sprite->offsetZ += (short)(rand() % 600 - 300);
            }
        }
    }
    if (pCur->timelineStep == 40) {
        for (i9 = 0; i9 < 10; i9++) {
            if ((g_BattleSpecAvalancheTargetMask & 1 << i9) != 0) {
                sprite = (BattleFxAvalancheTimeline *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialAvalanchePerTargetFaceOffsetModelTask);
                Battle_GetBoneWorldPos(i9, g_BattleActorRecords[i9].targetBone, &sprite->offsetX);
            }
        }
        Battle_AllocateResourceSlot(BattleFx_SpecialAvalancheStaggeredFaceOffsetTask);
        Battle_PlaySfxPannedToActor(0x189, -1, 0x40);
    }
    if (pCur->timelineStep == 90) {
        for (i9 = 0; i9 < 10; i9++) {
            if ((g_BattleSpecAvalancheTargetMask & 1 << i9) != 0) {
                Battle_PostActorHitOrHurtMessage(i9);
            }
        }
    }
    (pCur->timelineStep)++;
    if (pCur->timelineStep == 100) {
        pCur->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Switches on two more faces per frame, raises their offset and shrinks their depth field, then retires after 60 frames.
 */
// FUNCTION: FF7 0x005295a2
void __cdecl BattleFx_SpecialAvalancheStaggeredFaceOffsetTask(void)
{
    BattleTaskSlot *taskPtr;
    int meshAddress;
    int table;
    short i;

    taskPtr = BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (taskPtr->frameCounter == 30) {
        Battle_PlaySfxPannedToActor(0x18A, -1, 0x40);
    }
    if (taskPtr->frameCounter >= 30) {
        if ((*(short *)&g_BattleSpecAvalancheFaceIndex) != 36) {
            for (i = 0; i < 2; i++, (*(short *)&g_BattleSpecAvalancheFaceIndex)++) {
                g_BattleSpecAvalancheFaceActiveFlags[g_BattleSpecAvalancheFaceOrder[(*(short *)&g_BattleSpecAvalancheFaceIndex)]] = 1;
            }
        }
        meshAddress = (int)((char *)&g_BattleSpecAvalancheMeshWorking + 0x4);
        table = meshAddress + (*(int *)&g_BattleSpecAvalancheMeshWorking);
        table += 4;
        for (i = 0; i < 36; i++) {
            if (g_BattleSpecAvalancheFaceActiveFlags[i] == 1) {
                *(short *)(((char *)&g_BattleSpecAvalancheFaceIndex + 10) + i * 8) += 40;
                if (*(short *)((char *)table + (i << 3) * 2 + 6) != 0) {
                    *(short *)((char *)table + (i << 3) * 2 + 6) -= 16;
                }
            }
        }
    }
    (taskPtr->frameCounter)++;
    if (taskPtr->frameCounter == 60) {
        taskPtr->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Draws the face-offset model at each target and spawns its drifting sprites.
 */
// FUNCTION: FF7 0x00529715
void __cdecl BattleFx_SpecialAvalanchePerTargetFaceOffsetModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[84];
        PsxMatrix poseMatrix;
        struct { unsigned char pad_to_20[0x20]; int modelMatrix; };
        struct { unsigned char pad_to_32[0x32]; int centerX; };
        struct { unsigned char pad_to_36[0x36]; int centerY; };
        struct { unsigned char pad_to_3A[0x3A]; int centerZ; };
        struct { unsigned char pad_to_40[0x40]; BattleFxPositionedSpriteSlot *driftSlot; };
        struct { unsigned char pad_to_48[0x48]; short angle; };
        struct { unsigned char pad_to_4C[0x4C]; short step; };
        struct { unsigned char pad_to_50[0x50]; BattleFxEnergyWaveSpinModelsSlot *faceSlot; };
    } frame;
#pragma pack(pop)

    frame.faceSlot = (BattleFxEnergyWaveSpinModelsSlot *)BATTLE_CURRENT_TASK_SLOT;
    RotMatrix((SVECTOR *)&g_BattleSpecAvalancheRotationVector, &frame.modelMatrix);
    ScaleMatrix(&frame.modelMatrix, (int *)&g_BattleSpecAvalancheScaleVector);
    frame.centerX = frame.faceSlot->centerX;
    frame.centerY = frame.faceSlot->centerY;
    frame.centerZ = frame.faceSlot->centerZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.modelMatrix, &frame.modelMatrix);
    RotMatrix((SVECTOR *)&g_BattleSpecAvalancheRotationVector, &frame.poseMatrix);
    SetDrawColorRGB(0, 0, 0);
    Gfx_SetPsxPrimQuadState(9, 4, (GfxPsxSpriteQuadState *)(Gfx_ModelTableGetSpriteObject(5, g_BattleSpecAvalancheLoadedResTable)));
    Gfx_SetPsxPrimQuadState(9, 1, (GfxPsxSpriteQuadState *)(Gfx_ModelTableGetSpriteObject(6, g_BattleSpecAvalancheLoadedResTable)));
    g_BattleSpecAvalancheDrawListHead = BattleFx_SpecialAvalancheDrawPerFaceOffsetModel((BattleFxFreezeBreathDrawRecord *)&g_BattleSpecAvalancheModelDrawReq, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, (BattleFxFreezeBreathDrawPrim *)g_BattleSpecAvalancheDrawListHead, (struct Vec3sPacked *)((char *)&g_BattleSpecAvalancheFaceIndex + 8), &frame.modelMatrix, (struct BattleFxOffsetTriMeshDesc *)&frame.poseMatrix);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.faceSlot->frameCounter >= 30 && frame.faceSlot->frameCounter < 48) {
        frame.step = frame.faceSlot->frameCounter - 30;
        frame.driftSlot = (BattleFxPositionedSpriteSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialAvalancheYDriftAnimatedSpriteTask);
        frame.angle = (rand() % 4096);
        frame.driftSlot->posX = frame.faceSlot->centerX + (SinFixed(frame.angle) * 0x2BC >> 12);
        frame.driftSlot->posZ = frame.faceSlot->centerZ + (CosFixed(frame.angle) * 0x2BC >> 12);
        frame.driftSlot->posY = -frame.step * 90 + rand() % 400 + -200;
    }
    (frame.faceSlot->frameCounter)++;
    if (frame.faceSlot->frameCounter == 60) {
        frame.faceSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Draws the Nadare face mesh with the per-face offsets applied.
 */
// FUNCTION: FF7 0x0052991b
int __cdecl BattleFx_SpecialAvalancheDrawPerFaceOffsetModel(BattleFxFreezeBreathDrawRecord *drawRecord, int flags, int value, BattleFxFreezeBreathDrawPrim *extraCount, Vec3sPacked *offsetTable, void *matrix, struct BattleFxOffsetTriMeshDesc *poseMatrix)
{
    Vec3sPacked *startVertex;
    Vec3sPacked *vertBPointer;
    int modelPtr;
    int faceTotal;
    BattleFxFreezeBreathDrawPrim *prim;
    int activeCapBits[2];
    BattleFxPsxTexturedQuad *curPointer;
    int faceIdx;
    int x; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int scratchMtx;
    int numElements;
    int a; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    unsigned short primFlagBits;
    Vec3sPacked *localVertexC;
    int d; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    short brightness;
    BattleFxFaceOffsets *pOffsetBlock;
    BattleFxFreezeBreathDrawPrim *drawState;

    pOffsetBlock = (BattleFxFaceOffsets *)Gfx_GetPsxScratchpadPtr(0xC0);
    scratchMtx = (int)pOffsetBlock + 0x28;
    modelPtr = drawRecord->modelData + 4;
    curPointer = (BattleFxPsxTexturedQuad *)(modelPtr + *(int *)drawRecord->modelData);
    drawState = (BattleFxFreezeBreathDrawPrim *)Gfx_GetPsxPacketScratchPtr();
    faceTotal = curPointer->flags;
    curPointer = (BattleFxPsxTexturedQuad *)((char *)curPointer + 2);
    primFlagBits = curPointer->flags & 0x19F | drawRecord->textureFlags;
    curPointer = (BattleFxPsxTexturedQuad *)((char *)curPointer + 2);
    startVertex = (Vec3sPacked *)(modelPtr + curPointer->flags);
    vertBPointer = (Vec3sPacked *)(modelPtr + curPointer->vertex1);
    localVertexC = (Vec3sPacked *)(modelPtr + curPointer->vertex2);
    for (faceIdx = 0; faceIdx < faceTotal; faceIdx++) {
        pOffsetBlock->centroidX = (startVertex->vx + vertBPointer->vx + localVertexC->vx) / 3;
        pOffsetBlock->centroidY = (startVertex->vy + vertBPointer->vy + localVertexC->vy) / 3;
        pOffsetBlock->centroidZ = (startVertex->vz + vertBPointer->vz + localVertexC->vz) / 3;
        pOffsetBlock->vertex0X = startVertex->vx - pOffsetBlock->centroidX;
        pOffsetBlock->vertex0Y = startVertex->vy - pOffsetBlock->centroidY;
        pOffsetBlock->vertex0Z = startVertex->vz - pOffsetBlock->centroidZ;
        pOffsetBlock->vertex1X = vertBPointer->vx - pOffsetBlock->centroidX;
        pOffsetBlock->vertex1Y = vertBPointer->vy - pOffsetBlock->centroidY;
        pOffsetBlock->vertex1Z = vertBPointer->vz - pOffsetBlock->centroidZ;
        pOffsetBlock->vertex2X = localVertexC->vx - pOffsetBlock->centroidX;
        pOffsetBlock->vertex2Y = localVertexC->vy - pOffsetBlock->centroidY;
        pOffsetBlock->vertex2Z = localVertexC->vz - pOffsetBlock->centroidZ;
        poseMatrix->transX = pOffsetBlock->centroidX + offsetTable->vx;
        poseMatrix->transY = pOffsetBlock->centroidY + offsetTable->vy;
        poseMatrix->transZ = pOffsetBlock->centroidZ + offsetTable->vz;
        offsetTable = (Vec3sPacked *)((char *)offsetTable + 8);
        MultiplyMatrix(matrix, poseMatrix, (void *)scratchMtx);
        SetRotMatrix((void *)scratchMtx);
        SetTransMatrix((void *)scratchMtx);
        RefreshGlobalTransformState();
        StoreDeferredDrawArgs(pOffsetBlock, (char *)pOffsetBlock + 8, (char *)pOffsetBlock + 0x10);
        ApplyDeferredDrawArgs();
        startVertex = (Vec3sPacked *)(modelPtr + curPointer->offsetC);
        vertBPointer = (Vec3sPacked *)(modelPtr + curPointer->offsetD);
        localVertexC = (Vec3sPacked *)(modelPtr + curPointer[1].flags);
        Gfx_PsxStoreClearFlag(activeCapBits);
        if ((activeCapBits[0] & 0x60000) == 0) {
            drawState->tag = 0x7000000;
            Gfx_ReadTransformOrderingDepth2(&numElements);
            drawState->colorWord = curPointer->colorWord;
            drawState->uvWord0 = curPointer->uvWord0;
            drawState->uvWord1 = curPointer->uvWord1;
            drawState->flagBits = primFlagBits;
            if ((drawRecord->drawFlags & 0x80) != 0) {
                brightness = curPointer->vertex3 & 0xFF;
                *(int *)((char *)drawState + 4) = brightness << 0x10 | 0x24000000 | brightness << 8 | brightness;
            } else {
                brightness = drawRecord->shade;
                *(int *)((char *)drawState + 4) = brightness << 0x10 | 0x24000000 | brightness << 8 | brightness;
            }
            drawState->polyCommand |= (char)((drawRecord->drawFlags & 8) != 0 ? 2 : 0);
            Gfx_DrawPsxPrimitive(drawState);
        }
        curPointer = (BattleFxPsxTexturedQuad *)((char *)curPointer + 0x10);
    }
    curPointer = (BattleFxPsxTexturedQuad *)((char *)curPointer + 4);
    faceTotal = curPointer->flags;
    curPointer = (BattleFxPsxTexturedQuad *)((char *)curPointer + 4);
    prim = (BattleFxFreezeBreathDrawPrim *)drawState;
    startVertex = (Vec3sPacked *)(modelPtr + curPointer->flags);
    vertBPointer = (Vec3sPacked *)(modelPtr + curPointer->vertex1);
    localVertexC = (Vec3sPacked *)(modelPtr + curPointer->vertex2);
    for (faceIdx = 0; faceIdx < faceTotal; faceIdx++) {
        pOffsetBlock->centroidX = (startVertex->vx + vertBPointer->vx + localVertexC->vx) / 3;
        pOffsetBlock->centroidY = (startVertex->vy + vertBPointer->vy + localVertexC->vy) / 3;
        pOffsetBlock->centroidZ = (startVertex->vz + vertBPointer->vz + localVertexC->vz) / 3;
        pOffsetBlock->vertex0X = startVertex->vx - pOffsetBlock->centroidX;
        pOffsetBlock->vertex0Y = startVertex->vy - pOffsetBlock->centroidY;
        pOffsetBlock->vertex0Z = startVertex->vz - pOffsetBlock->centroidZ;
        pOffsetBlock->vertex1X = vertBPointer->vx - pOffsetBlock->centroidX;
        pOffsetBlock->vertex1Y = vertBPointer->vy - pOffsetBlock->centroidY;
        pOffsetBlock->vertex1Z = vertBPointer->vz - pOffsetBlock->centroidZ;
        pOffsetBlock->vertex2X = localVertexC->vx - pOffsetBlock->centroidX;
        pOffsetBlock->vertex2Y = localVertexC->vy - pOffsetBlock->centroidY;
        pOffsetBlock->vertex2Z = localVertexC->vz - pOffsetBlock->centroidZ;
        poseMatrix->transX = pOffsetBlock->centroidX + offsetTable->vx;
        poseMatrix->transY = pOffsetBlock->centroidY + offsetTable->vy;
        poseMatrix->transZ = pOffsetBlock->centroidZ + offsetTable->vz;
        offsetTable = (Vec3sPacked *)((char *)offsetTable + 8);
        MultiplyMatrix(matrix, poseMatrix, (void *)scratchMtx);
        SetRotMatrix((void *)scratchMtx);
        SetTransMatrix((void *)scratchMtx);
        RefreshGlobalTransformState();
        StoreDeferredDrawArgs(pOffsetBlock, (char *)pOffsetBlock + 8, (char *)pOffsetBlock + 0x10);
        ApplyDeferredDrawArgs();
        startVertex = (Vec3sPacked *)(modelPtr + curPointer[1].flags);
        vertBPointer = (Vec3sPacked *)(modelPtr + curPointer[1].vertex1);
        localVertexC = (Vec3sPacked *)(modelPtr + curPointer[1].vertex2);
        Gfx_PsxStoreClearFlag(activeCapBits);
        if ((activeCapBits[0] & 0x60000) == 0) {
            prim->tag = 0x6000000;
            Gfx_ReadTransformOrderingDepth2(&numElements);
            Gfx_ShadePsxTriVertexColors((CVECTOR *)((char *)curPointer + 8), (CVECTOR *)((char *)curPointer + 0xC), (CVECTOR *)((char *)curPointer + 0x10), drawRecord->shade, (CVECTOR *)((int)prim + 4), (CVECTOR *)((int)prim + 0xC), (CVECTOR *)((int)prim + 0x14));
            prim->polyCommand = ((drawRecord->drawFlags & 8) != 0 ? 2 : 0) + 0x30;
            Gfx_DrawPsxPrimitive(prim);
        }
        curPointer++;
    }
    return (int)prim;
}

/*
 * Draws one drifting sprite of animation 4 and retires it after the animation's last frame.
 */
// FUNCTION: FF7 0x0052a001
void __cdecl BattleFx_SpecialAvalancheYDriftAnimatedSpriteTask(void)
{
    BattleAvalancheYDriftSprite *task;

    task = (BattleAvalancheYDriftSprite *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecAvalancheSpriteReq1.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(4, g_BattleSpecAvalancheLoadedResTable);
    (g_BattleSpecAvalancheSpriteReq1.drawObj) = Battle3d_GetSceneRenderObject(1, g_pBattle3dScene);
    BattleFx_InstallBillboardMatrix((char *)task + 4, 0x2000 /* 2.0 */, 0);
    (g_BattleSpecAvalancheSpriteReq1.frame) = task->frameIndex;
    g_BattleSpecAvalancheDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecAvalancheSpriteReq1, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecAvalancheDrawListHead);
    if (g_BattleIsPaused != 0) {
        return;
    }
    task->posY += 80;
    if (g_BattleSpecAvalancheSpriteReq1.spriteGroup != 0) {
        (task->frameIndex)++;
        if (task->frameIndex >= g_BattleSpecAvalancheSpriteReq1.spriteGroup->header->frameCount) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Draws one static sprite of animation 3 and retires it after the animation's last frame.
 */
// FUNCTION: FF7 0x0052a0e6
void __cdecl BattleFx_SpecialAvalancheStaticAnimatedSpriteTask(void)
{
    BattleTaskSlot *task;

    task = BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecAvalancheSpriteReq2.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(3, g_BattleSpecAvalancheLoadedResTable);
    (g_BattleSpecAvalancheSpriteReq2.drawObj) = Gfx_ModelTableGetSpriteObject(3, g_BattleSpecAvalancheLoadedResTable);
    BattleFx_InstallBillboardMatrix((char *)task + 4, 0x1800 /* 1.5 */, 0);
    (g_BattleSpecAvalancheSpriteReq2.frame) = task->frameCounter;
    g_BattleSpecAvalancheDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecAvalancheSpriteReq2, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecAvalancheDrawListHead);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (g_BattleSpecAvalancheSpriteReq2.spriteGroup != 0) {
        (task->frameCounter)++;
        if (task->frameCounter >= g_BattleSpecAvalancheSpriteReq2.spriteGroup->header->frameCount) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Draws the growing Nadare model.
 */
// FUNCTION: FF7 0x0052a1b9
void __cdecl BattleFx_SpecialAvalancheGrowingModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[108];
        int fadeLevel;
        struct { unsigned char pad_to_4[0x4]; SVECTOR rotX; };
        struct { unsigned char pad_to_14[0x14]; PsxMatrix modelMatrix; };
        struct { unsigned char pad_to_34[0x34]; BattleFxAvalancheGrowSlot *growSlot; };
        struct { unsigned char pad_to_38[0x38]; short spawnIndex; };
        struct { unsigned char pad_to_44[0x44]; BattleFxAvalancheGrowingModelSlot *growingSlot; };
    } frame;
#pragma pack(pop)

    frame.growingSlot = (BattleFxAvalancheGrowingModelSlot *)BATTLE_CURRENT_TASK_SLOT;
    frame.rotX.vz = 0;
    frame.rotX.vx = frame.rotX.vz;
    frame.rotX.vy = g_BattleSpecAvalancheDirectionSign * 0xFFFFFC00;
    RotMatrix(&frame.rotX, &frame.modelMatrix);
    ScaleMatrix(&frame.modelMatrix, &g_BattleSpecAvalancheScale);
    frame.modelMatrix.t[0] = frame.growingSlot->posX;
    frame.modelMatrix.t[1] = frame.growingSlot->posY;
    frame.modelMatrix.t[2] = frame.growingSlot->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.modelMatrix, &frame.modelMatrix);
    SetRotMatrix(&frame.modelMatrix);
    SetTransMatrix(&frame.modelMatrix);
    if (frame.growingSlot->frameCounter < 42) {
        frame.fadeLevel = 128;
    } else {
        frame.fadeLevel = 50 - frame.growingSlot->frameCounter << 4;
    }
    (((BattleFxModelDraw *)&g_BattleSpecAvalancheModelReq)->fadeLevel) = *(short *)&frame.fadeLevel;
    (((BattleFxModelDraw *)&g_BattleSpecAvalancheModelReq)->flags) = (((BattleFxModelDraw *)&g_BattleSpecAvalancheModelReq)->fadeLevel) - 0x80 != 0 ? 8 : 0;
    if ((((BattleFxModelDraw *)&g_BattleSpecAvalancheModelReq)->fadeLevel) == 128) {
        ((BattleFxModelDraw *)&g_BattleSpecAvalancheModelReq)->modelGroup = Gfx_ModelTableGetRenderObject(0, g_BattleSpecAvalancheLoadedResTable);
    } else {
        ((BattleFxModelDraw *)&g_BattleSpecAvalancheModelReq)->modelGroup = Gfx_ModelTableGetRenderObject(1, g_BattleSpecAvalancheLoadedResTable);
    }
    g_BattleSpecAvalancheDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecAvalancheModelReq, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecAvalancheDrawListHead);
    if (g_BattleIsPaused != 0) {
        return;
    }
    g_BattleSpecAvalancheScale += 0x12C;
    g_BattleSpecAvalancheScaleY += 0x190;
    if (frame.growingSlot->frameCounter < 15) {
        for (frame.spawnIndex = 0; frame.spawnIndex < 6; frame.spawnIndex++) {
            frame.growSlot = (BattleFxAvalancheGrowSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialAvalancheYZDriftAnimatedSpriteTask);
            frame.growSlot->posX = (rand() % 16384) - 8192;
            frame.growSlot->posZ = frame.growingSlot->posZ;
            frame.growSlot->posY = 0;
        }
    }
    frame.growingSlot->posZ += frame.growingSlot->velocity;
    frame.growingSlot->velocity -= (short)(g_BattleSpecAvalancheDirectionSign * 15);
    g_BattleCameraFocalPoint.focalPointY += (short)(64 - ((frame.growingSlot->frameCounter % 2) << 7));
    (frame.growingSlot->frameCounter)++;
    if (frame.growingSlot->frameCounter == 50) {
        frame.growingSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * BattleFx_SpecialAvalancheYZDriftAnimatedSpriteTask.
 *
 * Per-frame tick for the effect record at
 * g_BattleTaskSlotBuffer.mainSlots[g_BattleActiveTaskSlotIdx] using animation entry 2 of resource
 * g_BattleSpecAvalancheLoadedResTable, loaded into the sprite descriptor g_BattleSpecAvalancheSpriteReq3,
 * billboarded at scale 0x3000, with the frame counter at +2 copied into the descriptor's +0xC and
 * the draw chained into g_BattleSpecAvalancheDrawListHead. Unless g_BattleIsPaused is set it also
 * moves the record every frame, subtracting 0x64 from the coordinate at +6 and adding
 * g_BattleSpecAvalancheDirectionSign * 0x384 to the one at +8, then advances the counter and stores
 * 0xFFFF at +0 at the animation's frame count. Selects sprite field two, installs its billboard at
 * scale 0x3000, publishes the counter and draws before checking pause. When unpaused lowers Y by
 * 100 and adds g_BattleSpecAvalancheDirectionSign times 900 to Z even with a null sprite; only with a
 * nonnull sprite advances and retires at or above its header frame count.
 */
// FUNCTION: FF7 0x0052a440
void __cdecl BattleFx_SpecialAvalancheYZDriftAnimatedSpriteTask(void)
{
    BattleFxAvalancheDriftSprite *task;

    task = (BattleFxAvalancheDriftSprite *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecAvalancheSpriteReq3.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(2, g_BattleSpecAvalancheLoadedResTable);
    (g_BattleSpecAvalancheSpriteReq3.drawObj) = Gfx_ModelTableGetSpriteObject(2, g_BattleSpecAvalancheLoadedResTable);
    BattleFx_InstallBillboardMatrix((char *)task + 4, 0x3000 /* 3.0 */, 0);
    (g_BattleSpecAvalancheSpriteReq3.frame) = task->frameIndex;
    g_BattleSpecAvalancheDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecAvalancheSpriteReq3, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecAvalancheDrawListHead);
    if (g_BattleIsPaused != 0) {
        return;
    }
    task->posY -= 100;
    task->posZ += (short)(g_BattleSpecAvalancheDirectionSign * 900);
    if (g_BattleSpecAvalancheSpriteReq3.spriteGroup != 0) {
        (task->frameIndex)++;
        if (task->frameIndex >= g_BattleSpecAvalancheSpriteReq3.spriteGroup->header->frameCount) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}
} /* extern "C" */
