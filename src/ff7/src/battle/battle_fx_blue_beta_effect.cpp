// src/ff7/src/battle/battle_fx_blue_beta_effect.cpp
// Enemy Skill [12]: Beta (Japanese: ベータ, dev tag: "blue/beata" / Beata)
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: The Beta blue magic effect: a particle field with motion trails, spinning and
//   stretching models, and screen brightness and explosions on the timeline.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/enums/sound.h"
#include "ff7/api/battle.h"
#include "ff7/battle_tasks.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_fx.h"
#include "ff7/camera_script.h"
#include "lib/types/graphics.h"
#include <string.h>
#include <stdlib.h>
#include "lib/codegen.h"
extern "C" {

struct BattleFxBlueBetaParticleField;
struct BattleFxModelDraw;
#pragma intrinsic(memset)
#pragma intrinsic(memcpy)


// clang-format off
// GLOBAL: FF7 0x008adcc0
int g_BattleBlueBetaResourceDefs = 0x003200B1;
int g_BattleBlueBetaResourceDefsHeadPtr[1] = {
    (int)((char *)&g_BattleFxBlueBetaEffectResourceNames + 0x0)
};
int g_BattleBlueBetaResourceDefsDataTail[8] = {
    0x0, 0x0, 0x0, 0xc800b1, (int)((char *)&g_BattleFxBlueBetaEffectResourceNames + 0xc), 0x0,
    0x0, 0x0
};
// GLOBAL: FF7 0x008adce8
struct BattleBlueBetaOffsetPoint g_BattleBlueBetaParticleOffsets[16] = {{0x1004, -100, 2600, {0}, -0x2710, -1000, 0x2710, {0}}, {-2600, -100, 0x1004, {0}, -0x2710, -1000, -0x2710, {0}}, {-0x1004, -100, -2600, {0}, 0x2710, -1000, -0x2710, {0}}, {2600, -100, -0x1004, {0}, 0x2710, -1000, 0x2710, {0}}, {2700, -700, 3000, {0}, -0x1770, -0x157c, 0x1b58, {0}}, {-3000, -700, 2700, {0}, -0x1b58, -0x1964, -0x1770, {0}}, {-2700, -700, -3000, {0}, 0x1770, -0x1964, -0x1b58, {0}}, {3000, -700, -2700, {0}, 0x1b58, -0x1770, 0x1770, {0}}, {3400, -1800, 3400, {0}, -4000, 1800, 0x2710, {0}}, {-3400, -1800, 3400, {0}, -4000, 1800, -0x2af8, {0}}, {-3400, -1800, -3400, {0}, 4000, 1800, -0x2ee0, {0}}, {3400, -1800, -3400, {0}, 4000, 1800, 0x2ee0, {0}}, {0x157c, -1300, -3300, {0}, -3000, 0x2328, 0x2328, {0}}, {-3300, -1300, 0x157c, {0}, -3000, 0x1f40, -0x2328, {0}}, {-0x157c, -1300, -3300, {0}, 3000, 0x2328, -0x2328, {0}}, {3300, -1300, -0x157c, {0}, 3000, 0x1f40, 0x2328, {0}}};
unsigned char g_BattleFxBlueBetaEffectResourceNames[24] =
    "BEATA_1.rsd\0"
    "BEATA_2.rsd";
// GLOBAL: FF7 0x00bae0e8
short g_BattleBlueBetaScreenBrightness = 0;
// GLOBAL: FF7 0x00bae0ec
int g_BattleBlueBetaTargetMask = 0;
// GLOBAL: FF7 0x00bae0f0
struct BattleBlueBetaParticle g_BattleBlueBetaParticles[120] = {0};
// GLOBAL: FF7 0x00baeff0
struct BattleBlueBetaCentroid g_BattleBlueBetaTargetCentroid = {0};
// GLOBAL: FF7 0x00baeff8
struct GfxTableCursor *g_BattleBlueBetaModelTable = 0;
unsigned char g_bssPad_00baeffc[4] = {0};
// clang-format on

/* Loads and registers resources for the Beta (Beata) blue magic effect. */
// FUNCTION: FF7 0x0057a6e0
void __cdecl BattleFx_BlueLoadResBeta(void)
{
    GfxModelTableBank *newSceneField;
    GfxResourceRequest requestSlot;
    int kind;
    char resPathBuf[200];
    GfxResourceRequest *pRequest;
    int resourceDefsWord;

    if (g_pBattle3dScene != 0 && g_BattleBlueBetaModelTable == 0) {
        kind = 2;
        resourceDefsWord = (int)&g_BattleBlueBetaResourceDefs;
        newSceneField = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRequest = &requestSlot;
        Gfx_InitResourceRequest(pRequest);
        pRequest->groupTag = 0;
        // STRING: FF7 0x008ade00
        pRequest->resourcePath = BattleFx_BuildResourcePath("blue/beata", resPathBuf);
        Gfx_LoadModelTableIfAbsent(kind, resourceDefsWord, pRequest, &g_BattleBlueBetaModelTable, newSceneField);
    }
}

/* Entry point for the Beta (Beata) blue magic effect. */
// FUNCTION: FF7 0x0057a770
void __cdecl BattleFx_BlueEnterBeta(int targetMask, int actorIdx)
{
    BattleFx_BlueLoadResBeta();
    BattleFx_BlueBeginBeta(targetMask, actorIdx);
}

/* Initializes Beta (Beata) blue magic effect tasks and audio. */
// FUNCTION: FF7 0x0057a78a
void __cdecl BattleFx_BlueBeginBeta(int targetMask, int actorIdx)
{
    *(int *)&g_BattleFxDrawListBufferTaskSlotPtr = (int)BATTLE_NEW_TASK_SLOT(BattleFx_SelectDrawListBuffer);
    Battle_AllocateResourceSlot(BattleFx_BlueBetaSetupTask);
    g_BattleBlueBetaTargetMask = targetMask;
    Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputePanForEntity(targetMask), 0x8C);
}

/* Sets up target centroid and spawns particle tasks for Beta. */
// FUNCTION: FF7 0x0057a7de
void __cdecl BattleFx_BlueBetaSetupTask(void)
{
    BattleFxBlueBetaTimeline *task;

    task = (BattleFxBlueBetaTimeline *)BATTLE_CURRENT_TASK_SLOT;
    task->ownerSlot = BATTLE_TASK_FREE;
    task = (BattleFxBlueBetaTimeline *)BATTLE_NEW_TASK_SLOT(BattleFx_BlueBetaTimelineTask);
    Battle_GetActorMaskCentroid(g_BattleBlueBetaTargetMask, task->centroid);
    task->stretchBase = 0xF00;
    g_BattleBlueBetaTargetCentroid = *(BattleBlueBetaCentroid *)task->centroid;
    BattleFx_BlueBetaInitParticles();
}

/* Initializes particle field positions and velocities for Beta. */
// FUNCTION: FF7 0x0057a855
void __cdecl BattleFx_BlueBetaInitParticles(void)
{
    int spreadX;
    int heightY;
    int rangeZ;
    int particle;
    int offX;
    int wordIndex;
    int coordinateY;
    int offZ;

    spreadX = 0x1F40;
    heightY = 0xBB8;
    rangeZ = 0x1F40;
    for (particle = 0; particle < 0x78; particle++) {
        g_BattleBlueBetaParticles[particle].timer = 0;
        offX = g_BattleBlueBetaTargetCentroid.x + rand() % spreadX - (spreadX >> 1);
        offZ = g_BattleBlueBetaTargetCentroid.z + rand() % rangeZ - (rangeZ >> 1);
        coordinateY = -(rand() % heightY);
        if (offX > 0) {
            if (offX < 0x320) {
                offX += 0x320;
            }
        } else if (offX > -0x320) {
            offX -= 0x320;
        }
        if (offZ > 0) {
            if (offZ < 0x320) {
                offZ += 0x320;
            }
        } else if (offZ > -0x320) {
            offZ -= 0x320;
        }
        g_BattleBlueBetaParticles[particle].posX = offX;
        g_BattleBlueBetaParticles[particle].posY = coordinateY;
        g_BattleBlueBetaParticles[particle].posZ = offZ;
        for (wordIndex = 0; wordIndex < 4; wordIndex++) {
            g_BattleBlueBetaParticles[particle].cleared[wordIndex] = 0;
        }
        g_BattleBlueBetaParticles[particle].period = (((offX & 0xF) + ((offZ & 0xF) + 0x56)) + ((coordinateY & 3) << 4));
    }
    for (particle = 0; particle < 0x10; particle++) {
        g_BattleBlueBetaParticleOffsets[particle].offsetX += g_BattleBlueBetaTargetCentroid.x;
        g_BattleBlueBetaParticleOffsets[particle].offsetY += g_BattleBlueBetaTargetCentroid.y;
        g_BattleBlueBetaParticleOffsets[particle].offsetZ += g_BattleBlueBetaTargetCentroid.z;
    }
}

/* Main timeline task controlling Beta screen brightness and explosion tasks. */
// FUNCTION: FF7 0x0057aa55
void __cdecl BattleFx_BlueBetaTimelineTask(void)
{
    int actorIdx;
    BattleFxBlueBetaTimeline2 *pChild;
    BattleFxBlueBetaTimeline *task;

    task = (BattleFxBlueBetaTimeline *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (task->step == 0) {
        BattleFx_StartScreenFlashRamp(0xA);
    }
    if (task->step == 0xE) {
        BattleFx_FadeOutScreenFlashRamp(0x24);
        pChild = (BattleFxBlueBetaTimeline2 *)BATTLE_NEW_TASK_SLOT(BattleFx_BlueBetaSpinStretchFadeModelTask);
        *(Int64Pair *)&pChild->posX = *(Int64Pair *)task->centroid;
        pChild->spinAngle = 0;
        pChild->offset = 0xFFF0;
        pChild->scale = (task->stretchBase << 0xC) / 0x898;
        pChild = (BattleFxBlueBetaTimeline2 *)BATTLE_NEW_TASK_SLOT(BattleFx_BlueBetaSpinStretchFadeModelEntry1Task);
        *(Int64Pair *)&pChild->posX = *(Int64Pair *)task->centroid;
        pChild->posY = 0xF63C;
        pChild->spinAngle = 0;
        pChild->offset = 0xFFF4;
        pChild->scale = (task->stretchBase << 0xC) / 0x960;
        pChild = (BattleFxBlueBetaTimeline2 *)BATTLE_NEW_TASK_SLOT(BattleFx_BlueBetaParticleFieldTask);
        pChild->nextIndex = 0;
        g_BattleFxTimelineState.channel2 = 0;
        g_BattleFxTimelineState.channel1 = g_BattleFxTimelineState.channel2;
        g_BattleFxTimelineState.red = g_BattleFxTimelineState.channel1;
    }
    if (task->step >= 0xE && task->step < 0x22) {
        g_BattleBlueBetaScreenBrightness = (task->step - 0xE) * 0xCC;
        if (g_BattleBlueBetaScreenBrightness > 0x1000) {
            g_BattleBlueBetaScreenBrightness = 0x1000;
        }
    }
    if (task->step >= 0x40) {
        g_BattleBlueBetaScreenBrightness = 0x1000 - (task->step - 0x40 << 9);
        if (g_BattleBlueBetaScreenBrightness < 0) {
            g_BattleBlueBetaScreenBrightness = 0;
        }
    }
    if (task->step == 0x3C) {
        for (actorIdx = 0; actorIdx < 0xA; actorIdx++) {
            if ((g_BattleBlueBetaTargetMask & 1 << actorIdx) != 0) {
                Battle_PostActorHitOrHurtMessage(actorIdx);
            }
        }
    }
    if (task->step >= 0x48) {
        g_BattleBlueBetaScreenBrightness = 0;
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    g_BattleFxTimelineState.startValue = g_BattleBlueBetaScreenBrightness;
    task->step++;
}

/* Animates spinning, stretching, and fading model task for Beta. */
// FUNCTION: FF7 0x0057acaf
void __cdecl BattleFx_BlueBetaSpinStretchFadeModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[68];
        SVECTOR rotX;
        struct { unsigned char pad_to_8[0x8]; BattleFxBetaSpinStretchFadeModelSlot *beataSlot; };
        struct { unsigned char pad_to_C[0xC]; int stretchStep; };
        struct { unsigned char pad_to_10[0x10]; PsxMatrix matrix; };
        struct { unsigned char pad_to_30[0x30]; BattleFxModelDraw *modelReq; };
        struct { unsigned char pad_to_34[0x34]; VECTOR stretchX; };
    } frame;
#pragma pack(pop)

    frame.beataSlot = (BattleFxBetaSpinStretchFadeModelSlot *)BATTLE_CURRENT_TASK_SLOT;
    frame.rotX.vx = 0;
    frame.rotX.vy = frame.beataSlot->spinAngle;
    frame.rotX.vz = 0;
    RotMatrixXYZ(&frame.rotX, &frame.matrix);
    frame.matrix.t[0] = frame.beataSlot->posX;
    frame.matrix.t[1] = frame.beataSlot->posY;
    frame.matrix.t[2] = frame.beataSlot->posZ;
    frame.stretchStep = frame.beataSlot->stretch / 3;
    frame.stretchX.vz = frame.beataSlot->stretch;
    frame.stretchX.vy = frame.stretchX.vz;
    frame.stretchX.vx = frame.stretchX.vy;
    frame.stretchX.vx += frame.stretchStep;
    frame.stretchX.vz += frame.stretchStep;
    ScaleMatrix(&frame.matrix, &frame.stretchX);
    MultiplyMatrix(&g_BattleCameraTransform, &frame.matrix, &frame.matrix);
    SetRotMatrix(&frame.matrix);
    SetTransMatrix(&frame.matrix);
    frame.modelReq = (BattleFxModelDraw *)Gfx_GetPsxScratchpadPtr(0);
    frame.modelReq->modelGroup = Gfx_ModelTableGetRenderObject(0, g_BattleBlueBetaModelTable);
    frame.modelReq->flags = 8;
    frame.modelReq->uvScroll = 0;
    frame.modelReq->fadeLevel = 0x80;
    frame.modelReq->stateFlags = 0x20;
    frame.modelReq->texturePageBits = 0;
    if (frame.beataSlot->frameCounter >= 0x2A) {
        frame.modelReq->fadeLevel -= (short)(frame.beataSlot->frameCounter - 0x2A << 4);
    }
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitModelDraw((BattleFxModelDraw *)frame.modelReq, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (frame.beataSlot->frameCounter >= 0x32) {
        frame.beataSlot->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.beataSlot->spinAngle += frame.beataSlot->spinSpeed;
    frame.beataSlot->frameCounter++;
}

/* Animates secondary variant spinning and stretching model for Beta. */
// FUNCTION: FF7 0x0057ae6e
void __cdecl BattleFx_BlueBetaSpinStretchFadeModelEntry1Task(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[68];
        SVECTOR rotX;
        struct { unsigned char pad_to_8[0x8]; BattleFxBetaSpinStretchFadeModelSlot *beataSlot; };
        struct { unsigned char pad_to_C[0xC]; int stretchStep; };
        struct { unsigned char pad_to_10[0x10]; PsxMatrix matrix; };
        struct { unsigned char pad_to_30[0x30]; BattleFxModelDraw *modelReq; };
        struct { unsigned char pad_to_34[0x34]; VECTOR stretchX; };
    } frame;
#pragma pack(pop)

    frame.beataSlot = (BattleFxBetaSpinStretchFadeModelSlot *)BATTLE_CURRENT_TASK_SLOT;
    frame.rotX.vx = 0;
    frame.rotX.vy = frame.beataSlot->spinAngle;
    frame.rotX.vz = 0;
    RotMatrixXYZ(&frame.rotX, &frame.matrix);
    frame.matrix.t[0] = frame.beataSlot->posX;
    frame.matrix.t[1] = frame.beataSlot->posY;
    frame.matrix.t[2] = frame.beataSlot->posZ;
    frame.stretchStep = frame.beataSlot->stretch / 3;
    frame.stretchX.vz = frame.beataSlot->stretch;
    frame.stretchX.vy = frame.stretchX.vz;
    frame.stretchX.vx = frame.stretchX.vy;
    frame.stretchX.vx += frame.stretchStep;
    frame.stretchX.vz += frame.stretchStep;
    ScaleMatrix(&frame.matrix, &frame.stretchX);
    MultiplyMatrix(&g_BattleCameraTransform, &frame.matrix, &frame.matrix);
    SetRotMatrix(&frame.matrix);
    SetTransMatrix(&frame.matrix);
    frame.modelReq = (BattleFxModelDraw *)Gfx_GetPsxScratchpadPtr(0);
    frame.modelReq->modelGroup = Gfx_ModelTableGetRenderObject(1, g_BattleBlueBetaModelTable);
    frame.modelReq->flags = 8;
    frame.modelReq->uvScroll = 0;
    frame.modelReq->fadeLevel = 0x80;
    frame.modelReq->stateFlags = 0x20;
    frame.modelReq->texturePageBits = 0;
    if (frame.beataSlot->frameCounter >= 0x2A) {
        frame.modelReq->fadeLevel -= (short)(frame.beataSlot->frameCounter - 0x2A << 4);
    }
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitModelDraw((BattleFxModelDraw *)frame.modelReq, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (frame.beataSlot->frameCounter >= 0x32) {
        frame.beataSlot->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.beataSlot->spinAngle += frame.beataSlot->spinSpeed;
    frame.beataSlot->frameCounter++;
}

/* Updates and animates particle field for Beta. */
// FUNCTION: FF7 0x0057b02d
void __cdecl BattleFx_BlueBetaParticleFieldTask(void)
{
    int first, end;
    BattleFxBlueBetaParticleField *pBlock;
    BattleFxBetaParticleField *task;

    task = (BattleFxBetaParticleField *)BATTLE_CURRENT_TASK_SLOT;
    pBlock = (BattleFxBlueBetaParticleField *)Gfx_GetPsxScratchpadPtr(0);
    pBlock->drawListPtr = g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED;
    pBlock->renderMode = 2;
    pBlock->end = Gfx_GetPsxPacketScratchPtr();
    pBlock->cameraTransform = g_BattleCameraTransform.psxMatrix;
    g_BattleFxActiveDrawListPtr = BattleFx_BlueBetaDrawParticleField(pBlock);
    if (task->frame >= 0x36) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (task->frame < 0xC) {
        first = task->nextIndex;
        end = first + 0xA;
        for (; first < end; first++) {
            g_BattleBlueBetaParticles[first].timer = rand() % 8 + 0x22;
        }
        task->nextIndex = end;
    }
    task->frame++;
}

/* Renders particle field polygons for Beta. */
// FUNCTION: FF7 0x0057b134
int __cdecl BattleFx_BlueBetaDrawParticleField(BattleFxBlueBetaParticleField *field)
{
    int dz;
    int pz;
    int particleCur;
    int loopDivisor;
    int t; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int tmpDeltaX;
    int yOffset;
    int xCoord;
    int absDeltaX;
    int absStepY;
    int absStepZ;
    int yPosMetric;

    SetRotMatrix(&g_BattleCameraTransform);
    SetTransMatrix(&g_BattleCameraTransform);
    RefreshGlobalTransformState();
    for (particleCur = 0; particleCur < 0x78; particleCur++) {
        if (g_BattleBlueBetaParticles[particleCur].timer == 0) {
            continue;
        }
        xCoord = g_BattleBlueBetaParticles[particleCur].posX;
        yPosMetric = g_BattleBlueBetaParticles[particleCur].posY;
        pz = g_BattleBlueBetaParticles[particleCur].posZ;
        field->minDeltaXSeen = 0x7FFFFFFF;
        field->minDeltaX = 0x7FFFFFFF;
        for (loopDivisor = 0; loopDivisor < 0x10; loopDivisor++) {
            tmpDeltaX = xCoord - g_BattleBlueBetaParticleOffsets[loopDivisor].offsetX;
            if (tmpDeltaX < 0) {
                absDeltaX = -tmpDeltaX;
            } else {
                absDeltaX = tmpDeltaX;
            }
            tmpDeltaX = absDeltaX;
            yOffset = yPosMetric - g_BattleBlueBetaParticleOffsets[loopDivisor].offsetY;
            if (yOffset < 0) {
                absStepY = -yOffset;
            } else {
                absStepY = yOffset;
            }
            yOffset = absStepY;
            dz = pz - g_BattleBlueBetaParticleOffsets[loopDivisor].offsetZ;
            if (dz < 0) {
                absStepZ = -dz;
            } else {
                absStepZ = dz;
            }
            dz = absStepZ;
            tmpDeltaX = tmpDeltaX + yOffset + dz;
            if (tmpDeltaX < field->minDeltaX) {
                if (field->minDeltaX < field->minDeltaXSeen) {
                    field->minDeltaXSeen = field->minDeltaX;
                    field->prevFrame = field->frame;
                }
                field->minDeltaX = tmpDeltaX;
                field->frame = loopDivisor;
            }
        }
        loopDivisor = g_BattleBlueBetaParticles[particleCur].period;
        if (g_BattleIsPaused != 0) {
            dz = 0;
            yOffset = dz;
            tmpDeltaX = yOffset;
        } else {
            tmpDeltaX = g_BattleBlueBetaParticleOffsets[field->frame].stepX / loopDivisor + (g_BattleBlueBetaParticleOffsets[field->prevFrame].stepX >> 1) / loopDivisor;
            yOffset = g_BattleBlueBetaParticleOffsets[field->frame].stepY / loopDivisor + (g_BattleBlueBetaParticleOffsets[field->prevFrame].stepY >> 1) / loopDivisor;
            dz = g_BattleBlueBetaParticleOffsets[field->frame].stepZ / loopDivisor + (g_BattleBlueBetaParticleOffsets[field->prevFrame].stepZ >> 1) / loopDivisor;
            (g_BattleBlueBetaParticles[particleCur].timer)--;
        }
        field->posX = xCoord + tmpDeltaX;
        field->posY = yPosMetric + yOffset;
        field->posZ = pz + dz;
        BattleFx_BlueBetaDrawParticleTrail(field, &field->posX, &g_BattleBlueBetaParticles[particleCur].posX);
        g_BattleBlueBetaParticles[particleCur].posX = field->posX;
        g_BattleBlueBetaParticles[particleCur].posY = field->posY;
        g_BattleBlueBetaParticles[particleCur].posZ = field->posZ;
    }
    return field->end;
}
/* Renders particle motion trail for Beta. */
// FUNCTION: FF7 0x0057b440
void __cdecl BattleFx_BlueBetaDrawParticleTrail(BattleFxBlueBetaParticleField *particleField, void *startPos, void *endPos)
{
    struct {
        unsigned char unused[28];
        int end;
        unsigned char triangle[28];
        unsigned char quad[24];
        BattleBlueBetaTrailPrim *trianglePtr;
        BattleFxTrailQuad *quadPtr;
        int point1, point0;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    particleField->depth = ProjectPointHardware(startPos, (short *)&particleField->point0, &particleField->projectFlagA, &particleField->flags);
    if ((particleField->flags & 0x60000) == 0) {
        Gfx_ReadTransformOrderingDepth(&particleField->depth);
        frame.quadPtr = (BattleFxTrailQuad *)frame.quad;
        frame.trianglePtr = (BattleBlueBetaTrailPrim *)frame.triangle;
        frame.end = (int)&frame.unused;
        ProjectPointHardware(endPos, (short *)&particleField->point1, &particleField->projectFlagA, &particleField->flags);
        frame.quadPtr->colorCommand = frame.quadPtr->colorCommand & 0xFFFFFF | 0x5000000;
        frame.quadPtr->flags = 0x28;
        frame.trianglePtr->tag = frame.trianglePtr->tag & 0xFFFFFF | 0x6000000;
        frame.trianglePtr->code = 0x30;
        frame.quadPtr->flags |= 2;
        frame.trianglePtr->code |= 2;
        frame.quadPtr->colorR = 0x42;
        frame.quadPtr->colorG = 0x32;
        frame.quadPtr->colorB = 0x22;
        frame.trianglePtr->colorR = 0x30;
        frame.trianglePtr->colorG = 0x14;
        frame.trianglePtr->colorB = 0x10;
        frame.trianglePtr->color1R = 0x32;
        frame.trianglePtr->color1G = 0x16;
        frame.trianglePtr->color1B = 0x12;
        frame.trianglePtr->color2R = 4;
        frame.trianglePtr->color2G = 4;
        frame.trianglePtr->color2B = 4;
        frame.point0 = particleField->point0;
        frame.point1 = particleField->point1;
        frame.quadPtr->vertex0 = frame.point0;
        frame.quadPtr->vertex1 = frame.point0 + 0x10001;
        frame.quadPtr->vertex2 = frame.point0 + 0xFFFF;
        frame.quadPtr->vertex3 = frame.point0 + 0x20000;
        frame.trianglePtr->point0 = frame.point0;
        frame.trianglePtr->point0Shifted = frame.point0 + 0x20000;
        frame.trianglePtr->point1 = frame.point1;
        /* Retail caller reads residual EAX/AX as `int` after `void Gfx_SetBlendFactorPair` returns. */
        ((int(__cdecl *)(float, float))Gfx_SetBlendFactorPair)(GetLastProjectedNdcZ(), GetLastProjectedNdcW());
        Gfx_DrawPsxPrimitive(frame.quadPtr);
        /* Retail caller reads residual EAX/AX as `int` after `void Gfx_SetBlendFactorPair` returns. */
        ((int(__cdecl *)(float, float))Gfx_SetBlendFactorPair)(GetLastProjectedNdcZ(), GetLastProjectedNdcW());
        Gfx_DrawPsxPrimitive(frame.trianglePtr);
        particleField->end = frame.end;
    }
}
} /* extern "C" */
