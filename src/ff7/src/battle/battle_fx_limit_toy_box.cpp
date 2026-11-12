// src/ff7/src/battle/battle_fx_limit_toy_box.cpp
// Limit Break [72..75] (Cait Sith Slots): Toy Box (Japanese: トイボックス, dev tag: "LIMIT2/TOYBOX" / Toybox)
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Toybox limit break effects: several setup entry points each start tasks that draw
//   shared models and emit sprite quads and sprites.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/enums/sound.h"
#include "ff7/types/battle.h"
#include "lib/types/graphics.h"
#include "lib/types/polygon.h"
#include "ff7/camera_script.h"
#include <stdlib.h>
#include <string.h>
#include "lib/codegen.h"
#include "ff7/api/battle.h"
#include "ff7/api/menu.h"
#include "lib/api/sound.h"
#include "lib/api/graphics.h"
#include "ff7/battle_tasks.h"
#include "ff7/types/battle_limit.h"
#include "ff7/enums/battle.h"
extern "C" {

#pragma intrinsic(memset)
// clang-format off
// GLOBAL: FF7 0x0082d260
unsigned int g_BattleLimitToyBoxResourceDef[25] ={
    0x00192a39,
    (unsigned int)&g_BattleLimitToyBoxResNameQuestionMarksTim,
    (unsigned int)&g_BattleLimitToyBoxResNameQuestionMarksSpt,
    0x00000000,
    0x00000000,
    0x003200b4,
    // STRING: FF7 0x0082d31c
    (unsigned int)"hanmer.rsd",
    0x00000000,
    0x00000000,
    0x00000000,
    0x004b00b4,
    // STRING: FF7 0x0082d328
    (unsigned int)"hatena_2.rsd",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b4,
    // STRING: FF7 0x0082d338
    (unsigned int)"debuc.rsd",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b4,
    // STRING: FF7 0x0082d344
    (unsigned int)"hellh.rsd",
    0x00000000,
    0x00000000,
    0x00000000
};
// GLOBAL: FF7 0x0082d2c8
struct VECTOR g_BattleLimitToyBoxNegZVector = {0, 0, -0x1000, 0};
// GLOBAL: FF7 0x0082d2d8
BattleFxSpriteQuadRequest g_BattleLimitToyBoxQuadRequest = {-16, -16, 0, 0x20, 0x1f, 0x1f, 0x80, 0x80, 0x80, 0x2e, 0x3c, 0x7dc7, 0};
// GLOBAL: FF7 0x0082d2f0
BattleFxSpriteRequest g_BattleLimitToyBoxSpriteRequest = {0, 0, -128, -128, -128, 0x2e, 0, 0};
// GLOBAL: FF7 0x0082d300
unsigned char g_BattleLimitToyBoxResNameQuestionMarksTim[13] = "hatena_1.tim";
// GLOBAL: FF7 0x0082d310
unsigned char g_BattleLimitToyBoxResNameQuestionMarksSpt[12] = "hatena1.spt";
// GLOBAL: FF7 0x00b371c0
BattleFxModelDrawHead g_BattleLimitToyBoxModelMatrix = {0};
// GLOBAL: FF7 0x00b371d0
int g_BattleLimitToyBoxTableField = 0;
unsigned char g_bssPad_00b371d4[4] = {0};
// GLOBAL: FF7 0x00b371d8
short g_BattleLimitToyBoxActorBoneCoord = 0;
// GLOBAL: FF7 0x00b371dc
short g_BattleLimitToyBoxActorAngle = 0;
// GLOBAL: FF7 0x00b371e0
int g_BattleLimitToyBoxTargetMask = 0;
// GLOBAL: FF7 0x00b371e4
int g_BattleLimitToyBoxActorIndex = 0;
// GLOBAL: FF7 0x00b371e8
struct GfxTableCursor *g_BattleLimitToyBoxLoadedResTable = 0;
unsigned char g_bssPad_00b371ec[4] = {0};
// clang-format on
#pragma function(memcpy)
#pragma data_seg(".data$u4c_orbit")
#pragma bss_seg(".data$u4c_orbit")
#pragma data_seg(".bss$u4c_orbit_slot")
#pragma bss_seg(".bss$u4c_orbit_slot")
#pragma data_seg(".bss$u4c_orbit_flag")
#pragma bss_seg(".bss$u4c_orbit_flag")
#pragma data_seg()
#pragma bss_seg()

/*
 * Loads the shared model and sprite resource table for Cait Sith's Toy Box limit variants.
 */
// FUNCTION: FF7 0x004cbdf0
void __cdecl BattleFx_LimitLoadResToyBox(void)
{
    GfxModelTableBank *context;
    GfxResourceRequest requestBuffer;
    int kind;
    char pathBuffer[200];
    GfxResourceRequest *request;
    int defPtr;

    if (g_pBattle3dScene != 0 && g_BattleLimitToyBoxLoadedResTable == 0) {
        kind = 5;
        defPtr = (int)&g_BattleLimitToyBoxResourceDef;
        context = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        request = &requestBuffer;
        Gfx_InitResourceRequest(request);
        request->groupTag = 0;
        // STRING: FF7 0x0082d350
        request->resourcePath = BattleFx_BuildResourcePath("LIMIT2/TOYBOX", pathBuffer);
        Gfx_LoadModelTableIfAbsent(kind, defPtr, request, &g_BattleLimitToyBoxLoadedResTable, context);
    }
}

/*
 * Entry stub for Toy Box variant A.
 */
// FUNCTION: FF7 0x004cbe80
int __cdecl BattleFx_LimitEnterToyBox(int targetMask, int actorIdx)
{
    BattleFx_LimitBeginToyBox(targetMask, actorIdx, 0);
    return 0;
}

/*
 * Entry stub for Toy Box variant B.
 */
// FUNCTION: FF7 0x004cbe99
int __cdecl BattleFx_LimitEnterToyBoxB(int targetMask, int actorIdx)
{
    BattleFx_LimitBeginToyBox(targetMask, actorIdx, 1);
    return 0;
}

/*
 * Entry stub for Toy Box variant C.
 */
// FUNCTION: FF7 0x004cbeb2
int __cdecl BattleFx_LimitEnterToyBoxC(int targetMask, int actorIdx)
{
    BattleFx_LimitBeginToyBox(targetMask, actorIdx, 2);
    return 0;
}

/*
 * Entry stub for Toy Box variant D.
 */
// FUNCTION: FF7 0x004cbecb
int __cdecl BattleFx_LimitEnterToyBoxD(int targetMask, int actorIdx)
{
    BattleFx_LimitBeginToyBox(targetMask, actorIdx, 3);
    return 0;
}

/*
 * Selects the draw list buffer and dispatches the setup task for the chosen Toy Box variant.
 */
// FUNCTION: FF7 0x004cbee4
void __cdecl BattleFx_LimitBeginToyBox(int targetMask, int actorIdx, int variant)
{
    *(int *)&g_BattleFxDrawListBufferTaskSlotPtr = (int)BATTLE_NEW_TASK_SLOT(BattleFx_SelectDrawListBuffer);
    BattleFx_LimitLoadResToyBox();
    g_BattleLimitToyBoxTargetMask = targetMask;
    g_BattleLimitToyBoxActorIndex = actorIdx;
    switch (variant) {
    case BATTLE_TOYBOX_BOULDER:
        Battle_AllocateResourceSlot(BattleFx_LimitToyBoxASetupTask);
        break;
    case BATTLE_TOYBOX_ICICLE:
        Battle_AllocateResourceSlot(BattleFx_LimitToyBoxBSetupTask);
        break;
    case BATTLE_TOYBOX_WEIGHT:
        Battle_AllocateResourceSlot(BattleFx_LimitToyBoxCSetupTask);
        break;
    case BATTLE_TOYBOX_FAT_CHOCOBO:
        Battle_AllocateResourceSlot(BattleFx_LimitToyBoxDSetupTask);
    }
}

/*
 * Spawns the falling model and impact emitter tasks for Toy Box variant A.
 */
// FUNCTION: FF7 0x004cbf80
void __cdecl BattleFx_LimitToyBoxASetupTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[56];
        BattleFxLimitToyBoxASlot *slot;
        struct { unsigned char pad_to_4[0x4]; VECTOR dirX; };
        struct { unsigned char pad_to_14[0x14]; int angle; };
        struct { unsigned char pad_to_18[0x18]; int size; };
        struct { unsigned char pad_to_1C[0x1C]; VECTOR axisX; };
        struct { unsigned char pad_to_2C[0x2C]; int sizeTmp; };
        struct { unsigned char pad_to_30[0x30]; int centroid; };
        struct { unsigned char pad_to_32[0x32]; short tmpShort; };
        struct { unsigned char pad_to_34[0x34]; int targetAngle; };
    } frame;
#pragma pack(pop)

    frame.slot = (BattleFxLimitToyBoxASlot *)BATTLE_CURRENT_TASK_SLOT;
    frame.slot->ownerSlot = BATTLE_TASK_FREE;
    Battle_GetActorMaskCentroid(g_BattleLimitToyBoxTargetMask, (short *)&frame.centroid);
    frame.tmpShort = 0;
    Battle_GetBoneWorldPos(g_BattleLimitToyBoxActorIndex, g_BattleActorRecords[g_BattleLimitToyBoxActorIndex].targetBone, &g_BattleLimitToyBoxActorBoneCoord);
    frame.dirX.vx = g_BattleLimitToyBoxActorBoneCoord - *(short *)&frame.centroid;
    frame.dirX.vy = 0;
    frame.dirX.vz = g_BattleLimitToyBoxActorAngle - *(short *)&frame.targetAngle;
    NormalizeVectorFixed(&frame.dirX, &frame.dirX);
    frame.angle = ComputeAngleBetweenVectors(&g_BattleLimitToyBoxNegZVector, &frame.dirX, &frame.axisX);
    if (frame.axisX.vy < 0) {
        frame.angle = -frame.angle;
    }
    frame.slot = (BattleFxLimitToyBoxASlot *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitToyBoxADrawModelTask);
    frame.slot->worldPos = *(Int64Pair *)(&frame.centroid);
    frame.size = 1728;
    frame.slot->rollZOrRadius = 0;
    frame.slot->rollStepOrRange = 0xFF00 /* -256 */;
    frame.slot->rotationY = *(short *)&frame.angle;
    frame.slot->scale = 0;
    frame.slot->scaleStepOrSpeed = ((frame.size << 12) / 750) / 8;
    frame.sizeTmp = frame.size * 750;
    frame.slot->radialOffset = frame.sizeTmp / 650;
    frame.slot->heightOffset = frame.sizeTmp / 900;
    frame.slot = (BattleFxLimitToyBoxASlot *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitToyBoxAEmitSpriteQuadsTask);
    if (frame.size > 1266) {
        frame.size = 1266;
    }
    frame.slot->worldPos = *(Int64Pair *)(&frame.centroid);
    frame.slot->rollZOrRadius = frame.size / 2;
    frame.slot->rollStepOrRange = frame.slot->rollZOrRadius / 2;
    frame.slot->scale = (frame.size << 12) / 350;
    frame.slot->scaleStepOrSpeed = frame.size * 55 / 600;
    Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputePanForEntity(g_BattleLimitToyBoxTargetMask), 0xA4);
}

/*
 * Animates drop physics and submits the 3D toy model for Toy Box variant A.
 */
// FUNCTION: FF7 0x004cc18a
void __cdecl BattleFx_LimitToyBoxADrawModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[120];
        SVECTOR modelRotX;
        struct { unsigned char pad_to_8[0x8]; BattleFxToyBoxAnimSlot *animSlot; };
        struct { unsigned char pad_to_10[0x10]; SVECTOR localX; };
        struct { unsigned char pad_to_18[0x18]; int actorIndex; };
        struct { unsigned char pad_to_1C[0x1C]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_3C[0x3C]; PsxMatrix modelMatrix; };
        struct { unsigned char pad_to_5C[0x5C]; SVECTOR localRotX; };
        struct { unsigned char pad_to_64[0x64]; BattleFxModelDraw *modelDraw; };
        struct { unsigned char pad_to_68[0x68]; VECTOR scale; };
    } frame;
#pragma pack(pop)

    frame.animSlot = (BattleFxToyBoxAnimSlot *)BATTLE_CURRENT_TASK_SLOT;
    frame.modelRotX.vx = 0;
    frame.modelRotX.vy = 0;
    frame.modelRotX.vz = frame.animSlot->phase;
    RotMatrixXYZ(&frame.modelRotX, &frame.modelMatrix);
    frame.localX.vx = frame.animSlot->radialOffset;
    frame.localX.vy = 0;
    frame.localX.vz = 0;
    frame.localRotX.vx = 0;
    frame.localRotX.vy = frame.animSlot->yaw + 0x400;
    frame.localRotX.vz = 0;
    RotMatrixXYZ(&frame.localRotX, &frame.rotMatrix);
    ConcatMatrixPreservingTranslation(&frame.rotMatrix, &frame.modelMatrix, &frame.modelMatrix);
    ApplyMatrixToSVectorInt32(&frame.rotMatrix, &frame.localX, &frame.modelMatrix.t[0]);
    frame.modelMatrix.t[0] += frame.animSlot->posX;
    frame.modelMatrix.t[1] += frame.animSlot->posY - frame.animSlot->heightOffset;
    frame.modelMatrix.t[2] += frame.animSlot->posZ;
    frame.scale.vz = frame.animSlot->level;
    frame.scale.vy = frame.scale.vz;
    frame.scale.vx = frame.scale.vy;
    ScaleMatrix(&frame.modelMatrix, &frame.scale);
    MultiplyMatrix(&g_BattleCameraTransform, &frame.modelMatrix, &frame.modelMatrix);
    SetRotMatrix(&frame.modelMatrix);
    SetTransMatrix(&frame.modelMatrix);
    frame.modelDraw = (BattleFxModelDraw *)&g_BattleLimitToyBoxModelMatrix;
    frame.modelDraw->modelGroup = Gfx_ModelTableGetRenderObject(1, g_BattleLimitToyBoxLoadedResTable);
    frame.modelDraw->flags = BATTLE_FX_DRAW_FADE_TRANSPARENCY;
    frame.modelDraw->uvScroll = 0;
    frame.modelDraw->fadeLevel = 0;
    frame.modelDraw->stateFlags = 0;
    frame.modelDraw->texturePageBits = 0;
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitModelDraw(frame.modelDraw, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (frame.animSlot->timer >= 26) {
        frame.animSlot->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.animSlot->timer == 16) {
        for (frame.actorIndex = 0; frame.actorIndex < 10; frame.actorIndex++) {
            if ((g_BattleLimitToyBoxTargetMask & 1 << frame.actorIndex) != 0) {
                Battle_PostActorHitOrHurtMessage(frame.actorIndex);
            }
        }
    }
    if (frame.animSlot->timer >= 10 && frame.animSlot->timer < 14) {
        frame.animSlot->phase += frame.animSlot->phaseStep;
    }
    if (frame.animSlot->timer < 8) {
        frame.animSlot->level += frame.animSlot->levelStep;
    }
    (frame.animSlot->timer)++;
}

/*
 * Spawns bursts of impact sprite quads during Toy Box variant A's landing frames.
 */
// FUNCTION: FF7 0x004cc3fc
void __cdecl BattleFx_LimitToyBoxAEmitSpriteQuadsTask(void)
{
    VECTOR nx;
    int i;
    int burst;
    BattleFxToyBoxASpriteQuad *pSprite;
    int radius;
    BattleFxToyBoxASprite *emitter;

    emitter = (BattleFxToyBoxASprite *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (emitter->frame >= 14 && emitter->frame <= 17) {
        for (i = 0; i < 12; i++) {
            pSprite = (BattleFxToyBoxASpriteQuad *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitToyBoxASpriteQuadTask);
            nx.vx = rand() % 4096 - 2048;
            nx.vy = 0;
            nx.vz = rand() % 4096 - 2048;
            NormalizeVectorFixed(&nx, &nx);
            radius = emitter->spreadCenter - emitter->spreadHalfRange + rand() % (emitter->spreadHalfRange << 1);
            pSprite->posX = emitter->posX + (radius * nx.vx >> 12);
            pSprite->height = emitter->posY;
            pSprite->posY = emitter->posZ + (radius * nx.vz >> 12);
            pSprite->angle = emitter->angle;
            burst = emitter->baseSpeed + rand() % 32;
            pSprite->velocityX = burst * nx.vx >> 12;
            pSprite->velocityY = burst * nx.vz >> 12;
        }
    }
    if (emitter->frame >= 26) {
        emitter->state = BATTLE_TASK_FREE;
    }
    (emitter->frame)++;
}

/*
 * Steps and submits one animated impact quad for Toy Box variant A.
 */
// FUNCTION: FF7 0x004cc5b0
void __cdecl BattleFx_LimitToyBoxASpriteQuadTask(void)
{
    BattleFxToyBoxASpriteQuad *task;

    task = (BattleFxToyBoxASpriteQuad *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleLimitToyBoxQuadRequest.quadState = Gfx_ModelTableGetSpriteObject(0, g_BattleLimitToyBoxLoadedResTable);
    g_BattleLimitToyBoxQuadRequest.cellX = task->frameIndex << 5;
    BattleFx_InstallBillboardMatrix((char *)task + 4, (short)task->angle, 0);
    RefreshGlobalTransformState();
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitSpriteQuadDraw(&g_BattleLimitToyBoxQuadRequest, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (task->frameIndex >= 7) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    task->posX += task->velocityX;
    task->posY += task->velocityY;
    (task->frameIndex)++;
}

/*
 * Spawns the shared model and sprite burst tasks for Toy Box variant B.
 */
// FUNCTION: FF7 0x004cc68f
void __cdecl BattleFx_LimitToyBoxBSetupTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[16];
        BattleFxToyBoxBParticle *particle;
        struct { unsigned char pad_to_4[0x4]; int scaleSource; };
        struct { unsigned char pad_to_8[0x8]; int centroid; };
        struct { unsigned char pad_to_A[0xA]; short zeroShort; };
        struct {
            unsigned char pad[12];
            int value;
        } at4;
    } frame;
#pragma pack(pop)

    frame.particle = (BattleFxToyBoxBParticle *)BATTLE_CURRENT_TASK_SLOT;
    frame.particle->ownerSlot = BATTLE_TASK_FREE;
    Battle_GetActorMaskCentroid(g_BattleLimitToyBoxTargetMask, (short *)&frame.centroid);
    frame.zeroShort = 0;
    frame.particle = (BattleFxToyBoxBParticle *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitToyBoxSharedDrawModelTask);
    frame.particle->worldPos = *(Int64Pair *)(&frame.centroid);
    frame.particle->spawnRadiusMax = rand() % 4096;
    frame.scaleSource = 0x6A4;
    frame.particle->scale = (frame.scaleSource << 12) / 1050;
    frame.particle->velocityX = 0xF254;
    frame.particle->velocityY = 583;
    g_BattleLimitToyBoxTableField = Gfx_ModelTableGetRenderObject(2, g_BattleLimitToyBoxLoadedResTable);
    frame.particle = (BattleFxToyBoxBParticle *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitToyBoxEmitSpritesTask);
    if (frame.scaleSource > 0x4F2) {
        frame.scaleSource = 0x4F2;
    }
    frame.particle->worldPos = *(Int64Pair *)(&frame.centroid);
    frame.particle->spawnRadiusMax = 2600;
    frame.particle->spawnRadiusMin = 1700;
    frame.particle->scale = (frame.scaleSource << 12) / 350;
    frame.particle->emitSpeed = frame.scaleSource * 100 / 600;
    Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputePanForEntity(g_BattleLimitToyBoxTargetMask), 0x85);
}

/*
 * Animates and submits the falling 3D prop model shared by Toy Box variants B, C, and D.
 */
// FUNCTION: FF7 0x004cc7e8
void __cdecl BattleFx_LimitToyBoxSharedDrawModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[68];
        PsxMatrix rotMatrix;
        struct { unsigned char pad_to_20[0x20]; BattleFxToyBoxDrawModel *toyboxModel; };
        struct { unsigned char pad_to_24[0x24]; int actorIndex; };
        struct { unsigned char pad_to_28[0x28]; SVECTOR rotX; };
        struct { unsigned char pad_to_30[0x30]; BattleFxModelDraw *modelDraw; };
        struct { unsigned char pad_to_34[0x34]; VECTOR scaleX; };
    } frame;
#pragma pack(pop)

    frame.toyboxModel = (BattleFxToyBoxDrawModel *)BATTLE_CURRENT_TASK_SLOT;
    frame.rotX.vx = 0;
    frame.rotX.vy = frame.toyboxModel->yaw;
    frame.rotX.vz = 0;
    RotMatrixXYZ(&frame.rotX, &frame.rotMatrix);
    frame.rotMatrix.t[0] = frame.toyboxModel->posX;
    frame.rotMatrix.t[1] = frame.toyboxModel->posY + frame.toyboxModel->offset;
    frame.rotMatrix.t[2] = frame.toyboxModel->posZ;
    frame.scaleX.vz = frame.toyboxModel->scale;
    frame.scaleX.vy = frame.scaleX.vz;
    frame.scaleX.vx = frame.scaleX.vy;
    ScaleMatrix(&frame.rotMatrix, &frame.scaleX);
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.rotMatrix);
    SetRotMatrix(&frame.rotMatrix);
    SetTransMatrix(&frame.rotMatrix);
    frame.modelDraw = (BattleFxModelDraw *)&g_BattleLimitToyBoxModelMatrix;
    frame.modelDraw->modelGroup = g_BattleLimitToyBoxTableField;
    frame.modelDraw->flags = 0x180;
    frame.modelDraw->uvScroll = 0;
    frame.modelDraw->fadeLevel = 0;
    frame.modelDraw->stateFlags = 0;
    frame.modelDraw->texturePageBits = 0;
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitModelDraw(frame.modelDraw, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (frame.toyboxModel->step >= 26) {
        frame.toyboxModel->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.toyboxModel->step == 16) {
        for (frame.actorIndex = 0; frame.actorIndex < 10; frame.actorIndex++) {
            if ((g_BattleLimitToyBoxTargetMask & 1 << frame.actorIndex) != 0) {
                Battle_PostActorHitOrHurtMessage(frame.actorIndex);
            }
        }
    }
    if (frame.toyboxModel->step < 6) {
        frame.toyboxModel->offset += frame.toyboxModel->offsetStep;
    }
    if (frame.toyboxModel->step == 6) {
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputeScreenPanFromPosition((char *)frame.toyboxModel + 4), 0xA6);
    }
    (frame.toyboxModel->step)++;
}

/*
 * Emits randomized burst sprites on impact for Toy Box variants B, C, and D.
 */
// FUNCTION: FF7 0x004cc9d0
void __cdecl BattleFx_LimitToyBoxEmitSpritesTask(void)
{
    VECTOR nx;
    int i;
    int burst;
    BattleFxToyBoxSprite *pSprite;
    int radius;
    BattleFxLimitToyBoxSprite *emitter;

    emitter = (BattleFxLimitToyBoxSprite *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (emitter->step >= 6 && emitter->step <= 9) {
        for (i = 0; i < 12; i++) {
            pSprite = (BattleFxToyBoxSprite *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitToyBoxEmittedSpriteTask);
            nx.vx = rand() % 4096 - 2048;
            nx.vy = 0;
            nx.vz = rand() % 4096 - 2048;
            NormalizeVectorFixed(&nx, &nx);
            radius = emitter->randMin + rand() % (emitter->randMax - emitter->randMin + 100);
            pSprite->posX = emitter->posX + (radius * nx.vx >> 12);
            pSprite->height = emitter->heightY;
            pSprite->posY = emitter->posY + (radius * nx.vz >> 12);
            pSprite->scale = emitter->spriteScale;
            burst = emitter->emitSpeed + rand() % 32;
            pSprite->velocityX = burst * nx.vx >> 12;
            pSprite->velocityY = burst * nx.vz >> 12;
        }
    }
    if (emitter->step >= 26) {
        emitter->ownerSlot = BATTLE_TASK_FREE;
    }
    (emitter->step)++;
}

/*
 * Draws and advances one emitted burst sprite for Toy Box variants B, C, and D.
 */
// FUNCTION: FF7 0x004ccb85
void __cdecl BattleFx_LimitToyBoxEmittedSpriteTask(void)
{
    BattleFxToyBoxEmittedSprite *task;

    task = (BattleFxToyBoxEmittedSprite *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleLimitToyBoxSpriteRequest.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(0, g_BattleLimitToyBoxLoadedResTable);
    g_BattleLimitToyBoxSpriteRequest.drawObj = Gfx_ModelTableGetSpriteObject(0, g_BattleLimitToyBoxLoadedResTable);
    g_BattleLimitToyBoxSpriteRequest.frame = task->frameIndex;
    BattleFx_InstallBillboardMatrix((char *)task + 4, (short)task->angle, 0);
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitSpriteDraw(&g_BattleLimitToyBoxSpriteRequest, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (task->frameIndex >= 11) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    task->posX += task->velocityX;
    task->posY += task->velocityY;
    (task->frameIndex)++;
}

/*
 * Spawns the shared model and sprite burst tasks for Toy Box variant C.
 */
// FUNCTION: FF7 0x004ccc74
void __cdecl BattleFx_LimitToyBoxCSetupTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[16];
        BattleFxToyBoxBParticle *particle;
        struct { unsigned char pad_to_4[0x4]; int scaleSource; };
        struct { unsigned char pad_to_8[0x8]; int centroid; };
        struct { unsigned char pad_to_A[0xA]; short zeroShort; };
        struct {
            unsigned char pad[12];
            int value;
        } at4;
    } frame;
#pragma pack(pop)

    frame.particle = (BattleFxToyBoxBParticle *)BATTLE_CURRENT_TASK_SLOT;
    frame.particle->ownerSlot = BATTLE_TASK_FREE;
    Battle_GetActorMaskCentroid(g_BattleLimitToyBoxTargetMask, (short *)&frame.centroid);
    frame.zeroShort = 0;
    frame.particle = (BattleFxToyBoxBParticle *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitToyBoxSharedDrawModelTask);
    frame.particle->worldPos = *(Int64Pair *)(&frame.centroid);
    frame.particle->spawnRadiusMax = rand() % 4096;
    frame.scaleSource = 0x5DC;
    frame.particle->scale = (frame.scaleSource << 12) / 1400;
    frame.particle->velocityX = 0xF254;
    frame.particle->velocityY = 583;
    g_BattleLimitToyBoxTableField = Gfx_ModelTableGetRenderObject(3, g_BattleLimitToyBoxLoadedResTable);
    frame.particle = (BattleFxToyBoxBParticle *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitToyBoxEmitSpritesTask);
    if (frame.scaleSource > 0x4F2) {
        frame.scaleSource = 0x4F2;
    }
    frame.particle->worldPos = *(Int64Pair *)(&frame.centroid);
    frame.particle->spawnRadiusMax = 2300;
    frame.particle->spawnRadiusMin = 1400;
    frame.particle->scale = (frame.scaleSource << 12) / 350;
    frame.particle->emitSpeed = frame.scaleSource * 100 / 600;
    Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputePanForEntity(g_BattleLimitToyBoxTargetMask), 0x85);
}

/*
 * Spawns the shared model and sprite burst tasks for Toy Box variant D.
 */
// FUNCTION: FF7 0x004ccdcd
void __cdecl BattleFx_LimitToyBoxDSetupTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[16];
        BattleFxToyBoxBParticle *particle;
        struct { unsigned char pad_to_4[0x4]; int scaleSource; };
        struct { unsigned char pad_to_8[0x8]; int centroid; };
        struct { unsigned char pad_to_A[0xA]; short zeroShort; };
        struct {
            unsigned char pad[12];
            int value;
        } at4;
    } frame;
#pragma pack(pop)

    frame.particle = (BattleFxToyBoxBParticle *)BATTLE_CURRENT_TASK_SLOT;
    frame.particle->ownerSlot = BATTLE_TASK_FREE;
    Battle_GetActorMaskCentroid(g_BattleLimitToyBoxTargetMask, (short *)&frame.centroid);
    frame.zeroShort = 0;
    frame.particle = (BattleFxToyBoxBParticle *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitToyBoxSharedDrawModelTask);
    frame.particle->worldPos = *(Int64Pair *)(&frame.centroid);
    frame.particle->spawnRadiusMax = rand() % 4096;
    frame.scaleSource = 0x384;
    frame.particle->scale = (frame.scaleSource << 12) / 800;
    frame.particle->velocityX = 0xF254;
    frame.particle->velocityY = 583;
    g_BattleLimitToyBoxTableField = Gfx_ModelTableGetRenderObject(4, g_BattleLimitToyBoxLoadedResTable);
    frame.particle = (BattleFxToyBoxBParticle *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitToyBoxEmitSpritesTask);
    if (frame.scaleSource > 0x4F2) {
        frame.scaleSource = 0x4F2;
    }
    frame.particle->worldPos = *(Int64Pair *)(&frame.centroid);
    frame.particle->spawnRadiusMax = 1400;
    frame.particle->spawnRadiusMin = 900;
    frame.particle->scale = (frame.scaleSource << 12) / 350;
    frame.particle->emitSpeed = frame.scaleSource * 100 / 600;
    Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputePanForEntity(g_BattleLimitToyBoxTargetMask), 0x85);
}
} /* extern "C" */
