// src/ff7/src/battle/battle_fx_magic_flare.cpp
/* Magic Effect [52]: Flare (Japanese: フレア, dev tag: "MAGIC/FLEA" / Flea) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Flare (Flea) magic effect: delayed explosion and burst sprites, expanding shockwave
//   models, growing and rotating billboard sprites and a moving spark.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/enums/sound.h"
#include "ff7/api/battle.h"
#include "ff7/battle_tasks.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_magic.h"
#include "lib/types/graphics.h"
#include "ff7/camera_script.h"
#include <string.h>
#include "lib/codegen.h"
#include <stdlib.h>
extern "C" {

struct BattleFxModelDraw;

#pragma intrinsic(memset)
#pragma intrinsic(memcpy)


// clang-format off
// GLOBAL: FF7 0x00bb081c
int g_BattleMagicFlareTargetActor = 0;
// GLOBAL: FF7 0x00bb0820
struct GfxTableCursor *g_BattleMagicFlareLoadedResTable = 0;
unsigned char g_bssPad_00bb0824[4] = {0};
// clang-format on

/* Loads and registers resources for Flare (Flea). */
// FUNCTION: FF7 0x0057f210
void __cdecl BattleFx_MagicLoadResFlare(void)
{
    GfxModelTableBank *ownerHandle;
    GfxResourceRequest recordBuf;
    int kind;
    char pathBufTemp[200];
    GfxResourceRequest *pRequest;
    int resourceDefsWord;

    if (g_pBattle3dScene != 0 && g_BattleMagicFlareLoadedResTable == 0) {
        kind = 5;
        resourceDefsWord = (int)&g_BattleMagicFlareResourceDefs;
        ownerHandle = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRequest = &recordBuf;
        Gfx_InitResourceRequest(pRequest);
        pRequest->groupTag = 0;
        // STRING: FF7 0x008b0260
        pRequest->resourcePath = (char *)BattleFx_BuildResourcePath("magic/flea", pathBufTemp);
        Gfx_LoadModelTableIfAbsent(kind, resourceDefsWord, pRequest, &g_BattleMagicFlareLoadedResTable, ownerHandle);
    }
}

/* Entry point for the Flare (Flea) magic effect. */
// FUNCTION: FF7 0x0057f2a0
void __cdecl BattleFx_MagicEnterFlare(int targetMask, int actorIdx)
{
    BattleFx_MagicLoadResFlare();
    BattleFx_MagicBeginFlare(targetMask, actorIdx);
}

/* Initializes Flare (Flea) effect tasks and audio. */
// FUNCTION: FF7 0x0057f2ba
void __cdecl BattleFx_MagicBeginFlare(int targetMask, int actorIdx)
{
    int bitIdx, count;

    g_BattleFxDrawListBufferTaskSlotPtr = (unsigned char *)BATTLE_NEW_TASK_SLOT(BattleFx_SelectDrawListBuffer);
    bitIdx = 0;
    count = 0;
    for (; bitIdx < 0xA; bitIdx++) {
        if ((targetMask & 1 << bitIdx) != 0) {
            count++;
            if (count == 1) {
                g_BattleMagicFlareTargetActor = bitIdx;
            }
        }
    }
    Battle_AllocateResourceSlot(BattleFx_MagicFlareSetupTask);
    Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputePanForEntity(targetMask), 0x174);
}

/* Sets up target position and spawns explosion tasks for Flare. */
// FUNCTION: FF7 0x0057f356
void __cdecl BattleFx_MagicFlareSetupTask(void)
{
    BattleFxMagicFlareRecord *rec;
    BattleFxMagicFlareSpawn *q;

    rec = (BattleFxMagicFlareRecord *)BATTLE_CURRENT_TASK_SLOT;
    rec->ownerSlot = BATTLE_TASK_FREE;
    rec = (BattleFxMagicFlareRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicFlareTimelineTask);
    Battle_GetBoneWorldPos(g_BattleMagicFlareTargetActor, g_BattleActorRecords[g_BattleMagicFlareTargetActor].targetBone, &rec->pos.xyz.x);
    rec->offsetIndex = 0;
    rec->radius = g_BattleActorRecords[g_BattleMagicFlareTargetActor].radius + 0x200;
    if (rec->radius > 0x76C) {
        rec->radius = 0x76C;
    }
    rec->pos.xyz.y = -rec->radius;
    q = (BattleFxMagicFlareSpawn *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicFlareDelayedSpawnTask);
    q->pos = rec->pos;
    q->ratio = rec->radius << 1;
}

/* Spawns delayed explosion and burst sprite tasks for Flare. */
// FUNCTION: FF7 0x0057f44c
void __cdecl BattleFx_MagicFlareDelayedSpawnTask(void)
{
    BattleMagicFlareSpawnSlot *pBurst;
    BattleFxMagicFlareSpawn *task;
    BattleFxFlareDelayedSpawn *child;

    if (g_BattleIsPaused != 0) {
        return;
    }
    task = (BattleFxMagicFlareSpawn *)BATTLE_CURRENT_TASK_SLOT;
    if (task->step == 0x2C) {
        pBurst = (BattleMagicFlareSpawnSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicFlareGrowFadeRotatedSpriteTask);
        pBurst->pos = task->pos;
        pBurst->pos.xyz.y = 0;
        pBurst->fadeLevel = 0;
        pBurst->ratioStep = (task->ratio << 0xC) / 0x120 / 0x18;
        child = (BattleFxFlareDelayedSpawn *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicFlareSpinGrowFadeModel4Task);
        child->pos = pBurst->pos;
        child->angle = 0;
        child->scale = 0;
        child->speed = 0xFF80;
        child->growScale = 0;
        child->rate = (task->ratio << 0xC) / 0x433 / 0x10;
        child = (BattleFxFlareDelayedSpawn *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicFlareSpinGrowFadeModel4LateFadeTask);
        child->pos = pBurst->pos;
        child->angle = 0;
        child->scale = 0;
        child->speed = 0xFFC0;
        child->growScale = 0;
        child->rate = (task->ratio << 0xC) / 0x5C3 / 0x10;
        child = (BattleFxFlareDelayedSpawn *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicFlareSpinGrowFadeModel3Task);
        child->pos = pBurst->pos;
        child->pos.xyz.y = -(task->ratio >> 1);
        child->angle = 0;
        child->scale = 0;
        child->speed = 0xFF80;
        child->growScale = 0;
        child->rate = (task->ratio << 0xC) / 0x2EE / 0x18;
        child = (BattleFxFlareDelayedSpawn *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicFlareSpinGrowFadeModel3Task);
        child->pos = pBurst->pos;
        child->pos.xyz.y = -(task->ratio >> 1);
        child->angle = 0;
        child->scale = 0x200;
        child->speed = 0xFFC0;
        child->growScale = 0;
        child->rate = (task->ratio << 0xC) / 0x320 / 0x18;
        child = (BattleFxFlareDelayedSpawn *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicFlareGrowFadeAnimatedSpriteTask);
        child->pos = pBurst->pos;
        child->pos.xyz.y = -(task->ratio >> 1);
        child->stepCount = 0;
        child->speed = task->ratio >> 3;
        child->growScale = 0;
        child->rate = (task->ratio << 0xC) / 0x180 / 0x18;
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    task->step++;
}

/* Animates spinning, growing, and fading model task for Flare. */
// FUNCTION: FF7 0x0057f775
void __cdecl BattleFx_MagicFlareSpinGrowFadeModel3Task(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[64];
        SVECTOR rotX;
        struct { unsigned char pad_to_8[0x8]; BattleFxFlareSpinGrowFadeModel3Slot *spinSlot; };
        struct { unsigned char pad_to_C[0xC]; PsxMatrix matrix; };
        struct { unsigned char pad_to_2C[0x2C]; BattleFxModelDraw *modelDraw; };
        struct { unsigned char pad_to_30[0x30]; VECTOR scaleX; };
    } frame;
#pragma pack(pop)

    frame.spinSlot = (BattleFxFlareSpinGrowFadeModel3Slot *)BATTLE_CURRENT_TASK_SLOT;
    frame.rotX.vx = 0;
    frame.rotX.vy = frame.spinSlot->angle + frame.spinSlot->angleOffset;
    frame.rotX.vz = 0;
    RotMatrixXYZ(&frame.rotX, &frame.matrix);
    frame.matrix.t[0] = frame.spinSlot->posX;
    frame.matrix.t[1] = frame.spinSlot->posY;
    frame.matrix.t[2] = frame.spinSlot->posZ;
    frame.scaleX.vz = frame.spinSlot->scale;
    frame.scaleX.vy = frame.scaleX.vz;
    frame.scaleX.vx = frame.scaleX.vy;
    ScaleMatrix(&frame.matrix, &frame.scaleX);
    MultiplyMatrix(&g_BattleCameraTransform, &frame.matrix, &frame.matrix);
    SetRotMatrix(&frame.matrix);
    SetTransMatrix(&frame.matrix);
    frame.modelDraw = (BattleFxModelDraw *)Gfx_GetPsxScratchpadPtr(0);
    frame.modelDraw->modelGroup = Gfx_ModelTableGetRenderObject(3, g_BattleMagicFlareLoadedResTable);
    frame.modelDraw->flags = 8;
    frame.modelDraw->uvScroll = 0;
    frame.modelDraw->fadeLevel = 0x80;
    frame.modelDraw->stateFlags = 0x20;
    frame.modelDraw->texturePageBits = 0;
    if (frame.spinSlot->frameCounter >= 0x10) {
        frame.modelDraw->fadeLevel -= (short)(frame.spinSlot->frameCounter - 0x10 << 4);
    }
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitModelDraw(frame.modelDraw, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (frame.spinSlot->frameCounter >= 0x18) {
        frame.spinSlot->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.spinSlot->scale += frame.spinSlot->scaleStep;
    if (frame.spinSlot->scale < 0) {
        frame.spinSlot->scale = 0x7000;
    }
    frame.spinSlot->angle += frame.spinSlot->angleStep;
    frame.spinSlot->frameCounter++;
}

/* Animates expanding shockwave model task for Flare. */
// FUNCTION: FF7 0x0057f942
void __cdecl BattleFx_MagicFlareSpinGrowFadeModel4Task(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[64];
        SVECTOR rotX;
        struct { unsigned char pad_to_8[0x8]; BattleFxFlareSpinGrowFadeModel4Slot *fleaSlot; };
        struct { unsigned char pad_to_C[0xC]; PsxMatrix matrix; };
        struct { unsigned char pad_to_2C[0x2C]; BattleFxModelDraw *drawModel; };
        struct { unsigned char pad_to_30[0x30]; VECTOR scale; };
    } frame;
#pragma pack(pop)

    frame.fleaSlot = (BattleFxFlareSpinGrowFadeModel4Slot *)BATTLE_CURRENT_TASK_SLOT;
    frame.rotX.vx = 0;
    frame.rotX.vy = frame.fleaSlot->angle;
    frame.rotX.vz = 0;
    RotMatrixXYZ(&frame.rotX, &frame.matrix);
    frame.matrix.t[0] = frame.fleaSlot->posX;
    frame.matrix.t[1] = frame.fleaSlot->posY;
    frame.matrix.t[2] = frame.fleaSlot->posZ;
    frame.scale.vz = frame.fleaSlot->scale;
    frame.scale.vy = frame.scale.vz;
    frame.scale.vx = frame.scale.vy;
    ScaleMatrix(&frame.matrix, &frame.scale);
    MultiplyMatrix(&g_BattleCameraTransform, &frame.matrix, &frame.matrix);
    SetRotMatrix(&frame.matrix);
    SetTransMatrix(&frame.matrix);
    frame.drawModel = (BattleFxModelDraw *)Gfx_GetPsxScratchpadPtr(0);
    frame.drawModel->modelGroup = Gfx_ModelTableGetRenderObject(4, g_BattleMagicFlareLoadedResTable);
    frame.drawModel->flags = 8;
    frame.drawModel->uvScroll = 0;
    frame.drawModel->fadeLevel = 0x80;
    frame.drawModel->stateFlags = 0x20;
    frame.drawModel->texturePageBits = 0;
    if (frame.fleaSlot->frameCounter >= 0x10) {
        frame.drawModel->fadeLevel -= (short)(frame.fleaSlot->frameCounter - 0x10 << 4);
    }
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitModelDraw(frame.drawModel, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (frame.fleaSlot->frameCounter >= 0x18) {
        frame.fleaSlot->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.fleaSlot->scale += frame.fleaSlot->scaleStep;
    if (frame.fleaSlot->scale < 0) {
        frame.fleaSlot->scale = 0x7000;
    }
    frame.fleaSlot->angle += frame.fleaSlot->angleStep;
    frame.fleaSlot->frameCounter++;
}

/* Animates secondary expanding shockwave model task for Flare. */
// FUNCTION: FF7 0x0057fb05
void __cdecl BattleFx_MagicFlareSpinGrowFadeModel4LateFadeTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[64];
        SVECTOR rotX;
        struct { unsigned char pad_to_8[0x8]; BattleFxFlareSpinGrowFadeModel4Slot *fleaSlot; };
        struct { unsigned char pad_to_C[0xC]; PsxMatrix matrix; };
        struct { unsigned char pad_to_2C[0x2C]; BattleFxModelDraw *drawModel; };
        struct { unsigned char pad_to_30[0x30]; VECTOR scale; };
    } frame;
#pragma pack(pop)

    frame.fleaSlot = (BattleFxFlareSpinGrowFadeModel4Slot *)BATTLE_CURRENT_TASK_SLOT;
    frame.rotX.vx = 0;
    frame.rotX.vy = frame.fleaSlot->angle;
    frame.rotX.vz = 0;
    RotMatrixXYZ(&frame.rotX, &frame.matrix);
    frame.matrix.t[0] = frame.fleaSlot->posX;
    frame.matrix.t[1] = frame.fleaSlot->posY;
    frame.matrix.t[2] = frame.fleaSlot->posZ;
    frame.scale.vz = frame.fleaSlot->scale;
    frame.scale.vy = frame.scale.vz;
    frame.scale.vx = frame.scale.vy;
    ScaleMatrix(&frame.matrix, &frame.scale);
    MultiplyMatrix(&g_BattleCameraTransform, &frame.matrix, &frame.matrix);
    SetRotMatrix(&frame.matrix);
    SetTransMatrix(&frame.matrix);
    frame.drawModel = (BattleFxModelDraw *)Gfx_GetPsxScratchpadPtr(0);
    frame.drawModel->modelGroup = Gfx_ModelTableGetRenderObject(4, g_BattleMagicFlareLoadedResTable);
    frame.drawModel->flags = 8;
    frame.drawModel->uvScroll = 0;
    frame.drawModel->fadeLevel = 0x80;
    frame.drawModel->stateFlags = 0x20;
    frame.drawModel->texturePageBits = 0;
    if (frame.fleaSlot->frameCounter >= 0x12) {
        frame.drawModel->fadeLevel -= (short)(frame.fleaSlot->frameCounter - 0x12 << 4);
    }
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitModelDraw(frame.drawModel, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (frame.fleaSlot->frameCounter >= 0x1A) {
        frame.fleaSlot->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.fleaSlot->scale += frame.fleaSlot->scaleStep;
    if (frame.fleaSlot->scale < 0) {
        frame.fleaSlot->scale = 0x7000;
    }
    frame.fleaSlot->angle += frame.fleaSlot->angleStep;
    frame.fleaSlot->frameCounter++;
}

/* Animates growing and fading billboard sprite for Flare. */
// FUNCTION: FF7 0x0057fcc8
void __cdecl BattleFx_MagicFlareGrowFadeAnimatedSpriteTask(void)
{
    unsigned char level;
    int q; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int r; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int s; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxFlareGrowFadeSprite *taskSlot;
    int u; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int v; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int w; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int x;
    int y; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int z; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int a; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int b; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int c; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int d; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int e; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int f; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */

    taskSlot = (BattleFxFlareGrowFadeSprite *)BATTLE_CURRENT_TASK_SLOT;
    BattleFx_InstallBillboardMatrix(&taskSlot->pos.xyz.x, (short)taskSlot->angle, -taskSlot->billboardDistance);
    level = 0x80;
    if (taskSlot->phase >= 0x10) {
        level -= (char)(taskSlot->phase - 0x10 << 4);
    }
    g_BattleMagicFlareSpriteReq1.colorB = level;
    g_BattleMagicFlareSpriteReq1.colorG = g_BattleMagicFlareSpriteReq1.colorB;
    g_BattleMagicFlareSpriteReq1.colorR = g_BattleMagicFlareSpriteReq1.colorG;
    g_BattleMagicFlareSpriteReq1.frame = taskSlot->stepCount;
    g_BattleMagicFlareSpriteReq1.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(1, g_BattleMagicFlareLoadedResTable);
    g_BattleMagicFlareSpriteReq1.drawObj = Gfx_ModelTableGetSpriteObject(1, g_BattleMagicFlareLoadedResTable);
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitSpriteDraw(&g_BattleMagicFlareSpriteReq1, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (taskSlot->phase >= 0x18) {
        taskSlot->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    taskSlot->angle += taskSlot->angleStep;
    if (taskSlot->angle < 0) {
        taskSlot->angle = 0x7000;
    }
    if (taskSlot->phase > 4) {
        taskSlot->stepCount++;
    }
    if (taskSlot->stepCount > 0xF) {
        taskSlot->stepCount = 0xF;
    }
    taskSlot->phase++;
}

/* Animates growing, rotating, and fading billboard sprite for Flare. */
// FUNCTION: FF7 0x0057fe36
void __cdecl BattleFx_MagicFlareGrowFadeRotatedSpriteTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[64];
        PsxMatrix matrix;
        struct { unsigned char pad_to_20[0x20]; BattleFxShadowFlareGrowFadeQuadSlot *spriteSlot; };
        struct { unsigned char pad_to_24[0x24]; SVECTOR rotX; };
        struct { unsigned char pad_to_2C[0x2C]; VECTOR scale; };
        struct { unsigned char pad_to_3C[0x3C]; char spriteFade; };
    } frame;
#pragma pack(pop)

    frame.spriteSlot = (BattleFxShadowFlareGrowFadeQuadSlot *)BATTLE_CURRENT_TASK_SLOT;
    frame.rotX.vx = 0x400;
    frame.rotX.vy = 0;
    frame.rotX.vz = 0;
    RotMatrixXYZ(&frame.rotX, &frame.matrix);
    frame.matrix.t[0] = frame.spriteSlot->posX;
    frame.matrix.t[1] = frame.spriteSlot->posY;
    frame.matrix.t[2] = frame.spriteSlot->posZ;
    frame.scale.vz = frame.spriteSlot->scale;
    frame.scale.vy = frame.scale.vz;
    frame.scale.vx = frame.scale.vy;
    ScaleMatrix(&frame.matrix, &frame.scale);
    MultiplyMatrix(&g_BattleCameraTransform, &frame.matrix, &frame.matrix);
    SetRotMatrix(&frame.matrix);
    SetTransMatrix(&frame.matrix);
    frame.spriteFade = 0x80;
    if (frame.spriteSlot->frameCounter >= 0x10) {
        frame.spriteFade -= frame.spriteSlot->frameCounter - 0x10 << 4;
    }
    g_BattleMagicFlareSpriteReq2.colorB = frame.spriteFade;
    g_BattleMagicFlareSpriteReq2.colorG = g_BattleMagicFlareSpriteReq2.colorB;
    g_BattleMagicFlareSpriteReq2.colorR = g_BattleMagicFlareSpriteReq2.colorG;
    g_BattleMagicFlareSpriteReq2.frame = 0;
    g_BattleMagicFlareSpriteReq2.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(2, g_BattleMagicFlareLoadedResTable);
    g_BattleMagicFlareSpriteReq2.drawObj = Gfx_ModelTableGetSpriteObject(2, g_BattleMagicFlareLoadedResTable);
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitSpriteDraw(&g_BattleMagicFlareSpriteReq2, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (frame.spriteSlot->frameCounter >= 0x18) {
        frame.spriteSlot->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.spriteSlot->scale += frame.spriteSlot->scaleStep;
    if (frame.spriteSlot->scale < 0) {
        frame.spriteSlot->scale = 0x7000;
    }
    frame.spriteSlot->frameCounter++;
}

/* Main timeline task controlling Flare detonation and burst sequence. */
// FUNCTION: FF7 0x0057ffdf
void __cdecl BattleFx_MagicFlareTimelineTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[76];
        PsxMatrix rotMatrix;
        struct { unsigned char pad_to_20[0x20]; BattleFxMagicFlareRecord *fleaRecord; };
        struct { unsigned char pad_to_24[0x24]; BattleMagicFlareSpriteSlot *spriteSlot; };
        struct { unsigned char pad_to_28[0x28]; int vecX; };
        struct { unsigned char pad_to_2A[0x2A]; short vecY; };
        struct { unsigned char pad_to_2C[0x2C]; int vecZ; };
        struct { unsigned char pad_to_30[0x30]; int randHalf; };
        struct { unsigned char pad_to_34[0x34]; int randRange; };
        struct { unsigned char pad_to_38[0x38]; int spawnIndex; };
        struct { unsigned char pad_to_3C[0x3C]; BattleFxFlareSlot *fleaSlot; };
        struct { unsigned char pad_to_40[0x40]; SVECTOR rotX; };
        struct { unsigned char pad_to_48[0x48]; short tmpStepValue; };
    } frame;
#pragma pack(pop)

    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.fleaRecord = (BattleFxMagicFlareRecord *)BATTLE_CURRENT_TASK_SLOT;
    if (frame.fleaRecord->offsetIndex < 0x12) {
        frame.fleaSlot = (BattleFxFlareSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicFlareMovingSpriteTask);
        frame.rotX.vx = g_BattleMagicFlareOffsetTableX[frame.fleaRecord->offsetIndex];
        frame.rotX.vy = g_BattleMagicFlareOffsetTableY[frame.fleaRecord->offsetIndex];
        frame.rotX.vz = 0;
        RotMatrixXYZ(&frame.rotX, &frame.rotMatrix);
        *(short *)&frame.vecX = 0x1450;
        frame.vecY = 0;
        *(short *)&frame.vecZ = 0;
        TransformVectorToInt16(&frame.rotMatrix, &frame.vecX, &frame.vecX);
        *(short *)&frame.vecX += frame.fleaRecord->pos.xyz.x;
        frame.vecY += frame.fleaRecord->pos.xyz.y;
        *(short *)&frame.vecZ += frame.fleaRecord->pos.xyz.z;
        frame.fleaSlot->pos.pair = *(Int64Pair *)(&frame.vecX);
        frame.fleaSlot->velocityX = (frame.fleaRecord->pos.xyz.x - *(short *)&frame.vecX) / 8;
        frame.fleaSlot->velocityY = (frame.fleaRecord->pos.xyz.y - frame.vecY) / 8;
        frame.fleaSlot->velocityZ = (frame.fleaRecord->pos.xyz.z - *(short *)&frame.vecZ) / 8;
        frame.fleaSlot->pushLength = -(frame.fleaRecord->radius >> 2);
        frame.fleaSlot->isLongLived = 0;
        frame.fleaRecord->offsetIndex++;
    }
    if (frame.fleaRecord->step > 0x18 && frame.fleaRecord->step < 0x25) {
        frame.randRange = 0xA28;
        frame.randHalf = frame.randRange >> 1;
        for (frame.spawnIndex = 0; frame.spawnIndex < 2; frame.spawnIndex++) {
            frame.spriteSlot = (BattleMagicFlareSpriteSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicFlareFrameTableSpriteTask);
            frame.spriteSlot->pos.xyz.x = frame.fleaRecord->pos.xyz.x + rand() % frame.randRange - frame.randHalf;
            frame.spriteSlot->pos.xyz.y = frame.fleaRecord->pos.xyz.y + rand() % frame.randRange - frame.randHalf;
            frame.spriteSlot->pos.xyz.z = frame.fleaRecord->pos.xyz.z + rand() % frame.randRange - frame.randHalf;
        }
    }
    if (frame.fleaRecord->offsetIndex < 0x3E && frame.fleaRecord->step > 0x2D) {
        for (frame.spawnIndex = 0; frame.spawnIndex < 4; frame.spawnIndex++) {
            frame.randHalf = frame.fleaRecord->offsetIndex - 0x12;
            frame.fleaSlot = (BattleFxFlareSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicFlareMovingSpriteTask);
            frame.rotX.vx = g_BattleMagicFlareOffsetTableX[frame.randHalf];
            frame.rotX.vy = g_BattleMagicFlareOffsetTableY[frame.randHalf];
            frame.rotX.vz = 0;
            RotMatrixXYZ(&frame.rotX, &frame.rotMatrix);
            *(short *)&frame.vecX = 0x1450;
            frame.vecY = 0;
            *(short *)&frame.vecZ = 0;
            TransformVectorToInt16(&frame.rotMatrix, &frame.vecX, &frame.vecX);
            *(short *)&frame.vecX += frame.fleaRecord->pos.xyz.x;
            frame.vecY += frame.fleaRecord->pos.xyz.y;
            *(short *)&frame.vecZ += frame.fleaRecord->pos.xyz.z;
            frame.fleaSlot->pos = frame.fleaRecord->pos;
            frame.fleaSlot->velocityX = (*(short *)&frame.vecX - frame.fleaRecord->pos.xyz.x) / 0x15;
            frame.fleaSlot->velocityY = (frame.vecY - frame.fleaRecord->pos.xyz.y) / 0x15;
            frame.fleaSlot->velocityZ = (*(short *)&frame.vecZ - frame.fleaRecord->pos.xyz.z) / 0x15;
            frame.fleaSlot->pushLength = 0;
            frame.fleaSlot->scale = rand() % 0x1C00 + 0x400;
            frame.fleaSlot->isLongLived = 1;
            frame.fleaRecord->offsetIndex++;
        }
    }
    if (frame.fleaRecord->step == 0x18) {
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputeScreenPanFromPosition(&frame.fleaRecord->pos), 0x175);
    } else if (frame.fleaRecord->step == 0x2D) {
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputeScreenPanFromPosition(&frame.fleaRecord->pos), 0x176);
    }
    if (frame.fleaRecord->step < 0xA) {
        g_BattleFxTimelineState.red = 0x20;
        g_BattleFxTimelineState.channel2 = 0;
        g_BattleFxTimelineState.channel1 = g_BattleFxTimelineState.channel2;
        frame.tmpStepValue = frame.fleaRecord->step * 0x166;
    } else if (frame.fleaRecord->step >= 0x40) {
        frame.tmpStepValue = 0xE00 - (frame.fleaRecord->step - 0x40) * 0x255;
        if (frame.tmpStepValue < 0) {
            frame.tmpStepValue = 0;
        }
    } else {
        frame.tmpStepValue = 0xE00;
    }
    if (frame.fleaRecord->step == 0x3A) {
        Battle_PostActorHitOrHurtMessage(g_BattleMagicFlareTargetActor);
    }
    if (frame.fleaRecord->step >= 0x46) {
        frame.tmpStepValue = 0;
        g_BattleFxTimelineState.red = *(char *)&frame.tmpStepValue;
        frame.fleaRecord->ownerSlot = BATTLE_TASK_FREE;
    }
    g_BattleFxTimelineState.startValue = frame.tmpStepValue;
    frame.fleaRecord->step++;
}

#pragma intrinsic(abs)

// clang-format off
// GLOBAL: FF7 0x008b0080
unsigned int g_BattleMagicFlareResourceDefs[25] = {0x00192a39, (unsigned int)"flea_4.TIM", (unsigned int)"exp_1.SPT", 0x00000000, 0x00000000, 0x00192a39, (unsigned int)"flea_3.TIM", (unsigned int)"exp_2.SPT", 0x00000000, 0x00000000, 0x00192a39, (unsigned int)"flea_5.TIM", (unsigned int)"exp_3.SPT", 0x00000000, 0x00000000, 0x004b00b1, (unsigned int)"FLEA_1.RSD", 0x00000000, 0x00000000, 0x00000000, 0x001900b1, (unsigned int)"FLEA_2.RSD", 0x00000000, 0x00000000, 0x00000000};
// GLOBAL: FF7 0x008b00e8
short g_BattleMagicFlareFrameTable[12] = {7, 5, 3, 1, 0, 0, 1, 2, 3, 4, 6, 7};
// GLOBAL: FF7 0x008b0100
struct BattleMagicFlareSpriteReq1 g_BattleMagicFlareSpriteReq1 = {0, 0, -128, -128, -128, {0x2e}, 0};
// GLOBAL: FF7 0x008b0110
struct BattleMagicFlareSpriteReq2 g_BattleMagicFlareSpriteReq2 = {0, 0, -128, -128, -128, {0x2e}, 0, {0}};
// GLOBAL: FF7 0x008b0128
struct BattleMagicFlareSpriteReq3 g_BattleMagicFlareSpriteReq3 = {{0x80, 0x80, 0x80, 0x2e}, 0, {0}, 0, 0};
// GLOBAL: FF7 0x008b0138
struct BattleMagicFlareSpriteColor g_BattleMagicFlareSpriteColor = {-128, -128, -128, {0x2e}, 0, {0}};
// GLOBAL: FF7 0x008b0140
short g_BattleMagicFlareOffsetTableX[44] = {0, -800, -420, 200, -550, 310, 540, -190, -650, 40, -140, 230, -240, 350, -430, 630, -210, -720, 190, 450, -310, 620, -390, 230, -260, 520, -610, 390, -730, 510, -490, 360, 40, 125, -225, 325, -455, -825, 15, 300, -515, -325, 85, -675};
unsigned char g_rawData_008b0198[8] = {0x9a, 0x01, 0x0d, 0xfd, 0x11, 0xfe, 0xaf, 0x00};
// GLOBAL: FF7 0x008b01a0
short g_BattleMagicFlareOffsetTableY[44] = {0, 1500, 3000, 500, 1000, 1900, 4000, 3300, 3100, 1300, 440, 2100, 2810, 600, 100, 2300, 2700, 230, 2000, 1200, 3800, 800, 2500, 150, 300, 1100, 3130, 2600, 900, 1450, 1650, 2400, 1650, 390, 3250, 2150, 880, 2950, 1300, 20, 1950, 2360, 3950, 550};
unsigned char g_rawData_008b01f8[8] = {0x3a, 0x07, 0xde, 0x0d, 0xaa, 0x0a, 0x52, 0x03};
// clang-format on

#pragma intrinsic(memset)
#pragma intrinsic(memcpy)

/* Renders animated frame-table sprite for Flare. */
// FUNCTION: FF7 0x005804d4
void __cdecl BattleFx_MagicFlareFrameTableSpriteTask(void)
{
    BattleMagicFlareSpriteSlot *task;

    task = (BattleMagicFlareSpriteSlot *)BATTLE_CURRENT_TASK_SLOT;
    BattleFx_InstallBillboardMatrix(&task->pos.xyz.x, 0x1800, 0);
    g_BattleMagicFlareSpriteReq3.frameValue = g_BattleMagicFlareFrameTable[task->frameCounter];
    g_BattleMagicFlareSpriteReq2.frameSpriteGroup = Gfx_ModelTableGetLoadedRecord(0, g_BattleMagicFlareLoadedResTable);
    g_BattleMagicFlareSpriteReq2.frameDrawObj = Gfx_ModelTableGetSpriteObject(0, g_BattleMagicFlareLoadedResTable);
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitSpriteDraw(&g_BattleMagicFlareSpriteReq2.frameSpriteGroup, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (task->frameCounter >= 0xB) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    task->frameCounter++;
}

/* Animates moving spark sprite for Flare. */
// FUNCTION: FF7 0x0058059d
void __cdecl BattleFx_MagicFlareMovingSpriteTask(void)
{
    short level;
    short size;
    short lifetime;
    BattleFxShadowFlareLivewire *task;

    task = (BattleFxShadowFlareLivewire *)BATTLE_CURRENT_TASK_SLOT;
    if (task->flag != 0) {
        size = task->scale;
        g_BattleMagicFlareSpriteColor.step = 7 - (task->step >> 1);
        lifetime = 0xE;
        level = 0x80;
        if (task->step >= 8) {
            level -= (short)((task->step - 8) * 0x15);
        }
        g_BattleMagicFlareSpriteColor.colorB = level;
        g_BattleMagicFlareSpriteColor.colorG = g_BattleMagicFlareSpriteColor.colorB;
        g_BattleMagicFlareSpriteColor.colorR = g_BattleMagicFlareSpriteColor.colorG;
    } else {
        size = 0x2000 - (task->step << 9);
        g_BattleMagicFlareSpriteColor.step = task->step;
        lifetime = 7;
    }
    BattleFx_InstallBillboardMatrix(&task->posX, (short)size, task->pushLength);
    g_BattleMagicFlareSpriteReq3.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(0, g_BattleMagicFlareLoadedResTable);
    g_BattleMagicFlareSpriteReq3.drawObj = Gfx_ModelTableGetSpriteObject(0, g_BattleMagicFlareLoadedResTable);
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitSpriteDraw(&g_BattleMagicFlareSpriteReq3.spriteGroup, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (task->step >= lifetime) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    task->posX += task->velocityX;
    task->posY += task->velocityY;
    task->posZ += task->velocityZ;
    task->step++;
}
} /* extern "C" */
