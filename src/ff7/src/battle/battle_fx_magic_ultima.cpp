// src/ff7/src/battle/battle_fx_magic_ultima.cpp
/* Magic Effect [53]: Ultima (Japanese: アルテマ, dev tag: "MAGIC/ALTEMA" / Altema) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Ultima (Altema) magic effect: drifting quad sprites, a growing energy model, screen
//   flashes from a timeline, a spinning fade model, and Gouraud quad rows.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/enums/sound.h"
#include "ff7/api/battle.h"
#include "lib/codegen.h"
#include "ff7/battle_tasks.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_magic.h"
#include "ff7/camera_script.h"
#include "lib/types/graphics.h"
#include <string.h>
#include <stdlib.h>
#include "lib/api/sound.h"
extern "C" {

struct BattleFxModelDraw;
#pragma intrinsic(memset)
#pragma intrinsic(memcpy)

// clang-format off
unsigned char g_rawData_008adb28[16] = {0x36, 0x01, 0x00, 0x00, 0x37, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x008adb38
int g_BattleMagicUltimaSoundDefs[2] = {9100072, 0};
// GLOBAL: FF7 0x008adb40
int g_BattleMagicUltimaResourceDefs = 79929;
int g_BattleMagicUltimaResourceDefsHeadPtr[1] = {
    (int)((char *)&g_BattleFxMagicUltimaUltimaDataPool + 0x28)
};
unsigned int g_BattleMagicUltimaResourceDefsDataTail[18] = {
    0x00000000,
    0x00000000,
    0x00000000,
    0x004b00b1,
    // STRING: FF7 0x008adc84
    (unsigned int)"altema_1.rsd",
    0x00000000,
    0x00000000,
    0x00000000,
    0x003200b1,
    // STRING: FF7 0x008adc94
    (unsigned int)"light_1.rsd",
    0x00000000,
    0x00000000,
    0x00000000,
    0x004b00b1,
    // STRING: FF7 0x008adca0
    (unsigned int)"cyltest.rsd",
    0x00000000,
    0x00000000,
    0x00000000
};
// GLOBAL: FF7 0x008adb90
BattleMagicUltimaQuadReq g_BattleMagicUltimaQuadReq = {
    -16, -32, 0x00, {0x20, 0x1f, 0x1f, 0x80, 0x80, 0x80, 0x2e, 0x3a, 0x00, 0x47, 0x78}, 0, {0}, 0, {0}, 0, {0}, 0x1000, 0, 0, 0, {0}
};
// GLOBAL: FF7 0x008adbc8
int g_BattleMagicUltimaMatrix = 4096;
unsigned char g_rawData_008adbd0[24] = {0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0xd8, 0xff, 0xff, 0xff, 0xee, 0x02, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x008adbe8
short g_BattleMagicUltimaVelocityTableX[2] = {40, -40};
// clang-format off
unsigned char g_rawData_008adbec[4] = {0xda, 0xff, 0x0c, 0x00};
unsigned char g_rawData_008adbf0[40] = {
    0xf8, 0xff, 0x30, 0x00, 0xf8, 0xff, 0xe4, 0xff, 0x24, 0x00, 0x0c, 0x00, 0x0e, 0x00, 0xe6, 0xff, 0xd4, 0xff, 0x28, 0x00, 0x14, 0x00, 0xe0, 0xff,
    0x10, 0x00, 0xf0, 0xff, 0xe8, 0xff, 0xd4, 0xff, 0x20, 0x00, 0x04, 0x00, 0xe4, 0xff, 0x20, 0x00
};
// clang-format on
// GLOBAL: FF7 0x008adc18
short g_BattleMagicUltimaVelocityTableY[2] = {-32, -24};
// clang-format off
unsigned char g_rawData_008adc1c[4] = {0x1a, 0x00, 0x20, 0x00};
unsigned char g_rawData_008adc20[40] = {
    0xec, 0xff, 0x08, 0x00, 0x14, 0x00, 0x0a, 0x00, 0x18, 0x00, 0xf8, 0xff, 0xe8, 0xff, 0xe0, 0xff, 0x18, 0x00, 0xee, 0xff, 0xe8, 0xff, 0xe4, 0xff,
    0x20, 0x00, 0xf4, 0xff, 0x26, 0x00, 0xfc, 0xff, 0x14, 0x00, 0x0c, 0x00, 0xe8, 0xff, 0xf0, 0xff
};
// clang-format on
// GLOBAL: FF7 0x008adc48
short g_BattleMagicUltimaVelocityTableZ[2] = {-16, -16};
// clang-format off
unsigned char g_rawData_008adc4c[4] = {0xf2, 0xff, 0xf2, 0xff};
unsigned char g_BattleFxMagicUltimaUltimaDataPool[52] = {0xf4, 0xff, 0xf0, 0xff, 0xf4, 0xff, 0xf6, 0xff, 0xf4, 0xff, 0xf8, 0xff, 0xf2, 0xff, 0xf0, 0xff, 0xf0, 0xff, 0xf2, 0xff, 0xf6, 0xff, 0xee, 0xff, 0xf4, 0xff, 0xf4, 0xff, 0xf6, 0xff, 0xf0, 0xff, 0xf6, 0xff, 0xee, 0xff, 0xf2, 0xff, 0xf4, 0xff, 0x66, 0x69, 0x72, 0x5f, 0x31, 0x2e, 0x54, 0x49, 0x4d, 0x00, 0x00, 0x00};
// clang-format on
// GLOBAL: FF7 0x00bae0d0
struct BattleMagicUltimaRotationVector g_BattleMagicUltimaRotationVector = {0};
// GLOBAL: FF7 0x00bae0d4
short g_BattleMagicUltimaAngle = 0;
// GLOBAL: FF7 0x00bae0d8
int g_BattleMagicUltimaTargetMask = 0;
// GLOBAL: FF7 0x00bae0dc
struct GfxTableCursor *g_BattleMagicUltimaLoadedResTable = 0;
// GLOBAL: FF7 0x00bae0e0
short g_BattleMagicUltimaScreenFade = 0;
unsigned char g_bssPad_00bae0e4[4] = {0};
// clang-format on


/* Loads and registers resources and sound groups for Ultima (Altema). */
// FUNCTION: FF7 0x005796b0
void __cdecl BattleFx_MagicLoadResUltima(void)
{
    GfxModelTableBank *allocRef;
    GfxResourceRequest reqBuffer;
    int kind;
    char pathArea[200];
    GfxResourceRequest *pRequest;
    int resDefBlock;

    if (g_pBattle3dScene != 0 && g_BattleMagicUltimaLoadedResTable == 0) {
        kind = 4;
        resDefBlock = (int)&g_BattleMagicUltimaResourceDefs;
        allocRef = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRequest = &reqBuffer;
        Gfx_InitResourceRequest(pRequest);
        pRequest->groupTag = 0;
        // STRING: FF7 0x008adcac
        pRequest->resourcePath = (char *)BattleFx_BuildResourcePath("magic/altema", pathArea);
        Gfx_LoadModelTableIfAbsent(kind, resDefBlock, pRequest, &g_BattleMagicUltimaLoadedResTable, allocRef);
        SfxRegisterSoundGroups((struct SoundGroup **)&g_BattleMagicUltimaSoundDefs);
    }
}

/* Entry point for the Ultima (Altema) magic effect. */
// FUNCTION: FF7 0x00579751
void __cdecl BattleFx_MagicEnterUltima(int targetMask, int actorIdx)
{
    BattleFx_MagicLoadResUltima();
    BattleFx_MagicBeginUltima(targetMask, actorIdx);
}

/* Initializes Ultima (Altema) effect tasks and audio. */
// FUNCTION: FF7 0x0057976b
void __cdecl BattleFx_MagicBeginUltima(int targetMask, int actorIdx)
{
    *(int *)&g_BattleFxDrawListBufferTaskSlotPtr = (int)BATTLE_NEW_TASK_SLOT(BattleFx_SelectDrawListBuffer);
    Battle_AllocateResourceSlot(BattleFx_MagicUltimaSetupTask);
    g_BattleMagicUltimaTargetMask = targetMask;
    Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_2, 0x40, 0);
}

/* Sets up initial effect parameters and target centroid for Ultima. */
// FUNCTION: FF7 0x005797b1
void __cdecl BattleFx_MagicUltimaSetupTask(void)
{
    BattleTaskSlot *task;

    task = BATTLE_CURRENT_TASK_SLOT;
    task->ownerSlot = BATTLE_TASK_FREE;
    task = BATTLE_NEW_TASK_SLOT(BattleFx_MagicUltimaSpawnDriftingQuadSpritesTask);
}

/* Spawns drifting quad particle sprites for Ultima. */
// FUNCTION: FF7 0x005797ef
void __cdecl BattleFx_MagicUltimaSpawnDriftingQuadSpritesTask(void)
{
    BattleFxUltimaDriftingQuad *timeline;
    int i, tableIdx;
    BattleMagicUltimaDriftSprite *drift;
    BattleFxUltimaDriftState *driftTaskPtr;

    if (g_BattleIsPaused != 0) {
        return;
    }
    driftTaskPtr = (BattleFxUltimaDriftState *)BATTLE_CURRENT_TASK_SLOT;
    if (driftTaskPtr->phase < 1) {
        tableIdx = driftTaskPtr->velocityTableIndex;
        for (i = 0; i < 24; i++, tableIdx++) {
            drift = (BattleMagicUltimaDriftSprite *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicUltimaDriftingQuadSpriteTask);
            drift->posX = 0;
            drift->posY = 0xFFE2;
            drift->scale = 0x1000 /* 1.0 */;
            drift->velocityX = g_BattleMagicUltimaVelocityTableX[tableIdx] << 1;
            drift->velocityY = g_BattleMagicUltimaVelocityTableY[tableIdx] << 1;
            drift->velocityZ = g_BattleMagicUltimaVelocityTableZ[tableIdx] << 2;
            drift->randomBits = rand() % 4096 + 2048;
            drift->flag = drift->randomBits >> 3 & 1;
        }
        driftTaskPtr->velocityTableIndex = tableIdx;
    } else if (driftTaskPtr->phase < 3) {
        for (i = 0; i < 24; i++, tableIdx++) {
            drift = (BattleMagicUltimaDriftSprite *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicUltimaDriftingQuadSpriteTask);
            drift->posX = 0;
            drift->posY = 0xFFE2;
            drift->scale = 0x1000 /* 1.0 */;
            drift->velocityX = (rand() % 128) * 2 - 128;
            drift->velocityY = (rand() % 128) * 2 - 128;
            drift->velocityZ = (drift->velocityX & 0xF) * 4 + 40;
            drift->randomBits = rand() % 4096 + 2048;
            drift->flag = drift->randomBits >> 3 & 1;
        }
    }
    if (driftTaskPtr->phase >= 2) {
        drift = (BattleMagicUltimaDriftSprite *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicUltimaGrowingModelTask);
        drift->velocityX = 0;
        drift->velocityY = 18;
        drift->flag = 0;
        timeline = (BattleFxUltimaDriftingQuad *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicUltimaTimelineTask);
        timeline->colorRampUp = 0;
        timeline->color = 0x242424;
        driftTaskPtr->ownerSlot = BATTLE_TASK_FREE;
    }
    (driftTaskPtr->phase)++;
}

/* Animates drifting quad sprite for Ultima. */
// FUNCTION: FF7 0x00579a97
void __cdecl BattleFx_MagicUltimaDriftingQuadSpriteTask(void)
{
    int p; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxDriftingQuadSprite *q;

    q = (BattleFxDriftingQuadSprite *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleMagicUltimaQuadReq.sceneObject = Battle3d_GetSceneRenderObject(1, g_pBattle3dScene);
    if (q->isFadingOut != 0) {
        g_BattleMagicUltimaQuadReq.fadeLevel = 15 - q->frameCount >> 2 << 5;
    } else {
        g_BattleMagicUltimaQuadReq.fadeLevel = q->frameCount >> 2 << 5;
    }
    g_BattleMagicUltimaQuadReq.rotationSource = q->rotationSource;
    g_BattleMagicUltimaQuadReq.rotationValue = g_BattleMagicUltimaQuadReq.rotationSource;
    g_BattleMagicUltimaQuadReq.posX = q->posX;
    g_BattleMagicUltimaQuadReq.posY = q->posY;
    g_BattleMagicUltimaQuadReq.posZ = q->posZ;
    SetRotMatrix((PsxMatrix *)&g_BattleMagicUltimaQuadReq.rotationValue);
    SetTransMatrix((PsxMatrix *)&g_BattleMagicUltimaQuadReq.rotationValue);
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitSpriteQuadDraw((BattleFxSpriteQuadRequest *)&g_BattleMagicUltimaQuadReq, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (g_BattleIsPaused == 0) {
        (q->frameCount)++;
        if (q->frameCount >= 15) {
            q->ownerSlot = BATTLE_TASK_FREE;
        }
        q->posX += q->velocityX;
        q->posY += q->velocityY;
        q->posZ += q->velocityZ;
    }
}

/* Animates growing central energy model for Ultima. */
// FUNCTION: FF7 0x00579bf3
void __cdecl BattleFx_MagicUltimaGrowingModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[80];
        SVECTOR rotX;
        struct { unsigned char pad_to_8[0x8]; BattleFxUltimaGrowingModelSlot *slot; };
        struct { unsigned char pad_to_C[0xC]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_2C[0x2C]; BattleFxModelDraw *drawModel; };
        struct { unsigned char pad_to_30[0x30]; BattleFxModelDrawHead drawModelBuf; };
        struct { unsigned char pad_to_40[0x40]; VECTOR scaleArg; };
    } frame;
#pragma pack(pop)

    frame.slot = (BattleFxUltimaGrowingModelSlot *)BATTLE_CURRENT_TASK_SLOT;
    frame.rotX.vx = 0;
    frame.rotX.vy = 0;
    frame.rotX.vz = 0;
    RotMatrixXYZ(&frame.rotX, &frame.rotMatrix);
    frame.rotMatrix.t[0] = 0;
    frame.rotMatrix.t[1] = 0xFFFFFFE2;
    frame.rotMatrix.t[2] = 0x200 /* 45 deg */;
    frame.scaleArg.vz = frame.slot->value;
    frame.scaleArg.vy = frame.scaleArg.vz;
    frame.scaleArg.vx = frame.scaleArg.vy;
    ScaleMatrix(&frame.rotMatrix, &frame.scaleArg);
    SetRotMatrix(&frame.rotMatrix);
    SetTransMatrix(&frame.rotMatrix);
    RefreshGlobalTransformState();
    frame.drawModel = (BattleFxModelDraw *)&frame.drawModelBuf;
    frame.drawModel->modelGroup = Gfx_ModelTableGetRenderObject(2, g_BattleMagicUltimaLoadedResTable);
    frame.drawModel->flags = 0x88;
    frame.drawModel->uvScroll = 0;
    frame.drawModel->fadeLevel = 0;
    frame.drawModel->stateFlags = 0;
    frame.drawModel->texturePageBits = 0;
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitModelDraw(frame.drawModel, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (frame.slot->frameCounter >= 32) {
        frame.slot->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.slot->frameCounter < 20) {
        frame.slot->value += frame.slot->velocity;
    }
    (frame.slot->frameCounter)++;
}

/* Main timeline task controlling Ultima screen flashes and sub-tasks. */
// FUNCTION: FF7 0x00579d4b
void __cdecl BattleFx_MagicUltimaTimelineTask(void)
{
    BattleFxUltimaTimeline *taskPtr;
    int i;
    BattleFxUltimaDrawRecord *pRecord;
    BattleFxUltimaSpinState *spin;

    taskPtr = (BattleFxUltimaTimeline *)BATTLE_CURRENT_TASK_SLOT;
    if (taskPtr->timer < 32) {
        pRecord = (BattleFxUltimaDrawRecord *)Gfx_GetPsxScratchpadPtr(0);
        pRecord->drawList = g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED;
        pRecord->renderState = Gfx_GetPsxPacketScratchPtr();
        pRecord->stepCount = 6;
        pRecord->duration = 30;
        pRecord->colorRamp = taskPtr->colorRampUp;
        /* Retail caller passes 2 arg(s) to 1-parameter `BattleFx_MagicUltimaDrawPsxGouraudQuadRows` (__cdecl stack cleanup matches retail). */
        g_BattleFxActiveDrawListPtr = ((int(__cdecl *)(int, short))BattleFx_MagicUltimaDrawPsxGouraudQuadRows)((int)pRecord, (short)taskPtr->timer);
    } else {
        pRecord = (BattleFxUltimaDrawRecord *)Gfx_GetPsxScratchpadPtr(0);
        pRecord->drawList = g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED;
        pRecord->renderState = Gfx_GetPsxPacketScratchPtr();
        pRecord->stepCount = 3;
        pRecord->duration = 30;
        pRecord->colorRamp = taskPtr->colorRampDown;
        g_BattleFxActiveDrawListPtr = BattleFx_MagicUltimaDrawPsxGouraudQuadGradientRows((BattleFxUltimaGradientDesc *)pRecord, taskPtr->timer - 0x20);
    }
    if (taskPtr->timer == 32) {
        spin = (BattleFxUltimaSpinState *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicUltimaSpinFadeModelTask);
        spin->scale = 0x1000 /* 1.0 */;
        spin->spinSpeed = 0x200 /* 45 deg */;
        spin->value = 0;
        spin->velocity = 0xA;
        spin->spinRate = 0xA;
        spin->isActive = 1;
        spin = (BattleFxUltimaSpinState *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicUltimaSpinFadeModelTask);
        spin->scale = 0x64;
        spin->spinSpeed = 0x228;
        spin->value = 0;
        spin->velocity = 4;
        spin->spinRate = 4;
        spin->isActive = 1;
        g_BattleStageHideFlags |= 7;
        g_BattleFxTimelineState.channel2 = 0;
        g_BattleFxTimelineState.channel1 = g_BattleFxTimelineState.channel2;
        g_BattleFxTimelineState.red = g_BattleFxTimelineState.channel1;
        g_BattleMagicUltimaScreenFade = 4096;
    }
    if (taskPtr->timer >= 74) {
        if (taskPtr->timer == 75) {
            g_BattleStageHideFlags &= 0xF8;
        }
        g_BattleMagicUltimaScreenFade = 4096 - (taskPtr->timer - 74) * 409;
        if (g_BattleMagicUltimaScreenFade < 0) {
            g_BattleMagicUltimaScreenFade = 0;
        }
    }
    if (taskPtr->timer >= 84) {
        g_BattleMagicUltimaScreenFade = 0;
        taskPtr->ownerSlot = BATTLE_TASK_FREE;
        Sfx_ReleaseCachedBuffer(0x136, 0xF);
        Sfx_ReleaseCachedBuffer(0x137, 0xF);
    }
    g_BattleFxTimelineState.startValue = g_BattleMagicUltimaScreenFade;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (taskPtr->timer == 78) {
        for (i = 0; i < 10; i++) {
            if ((g_BattleMagicUltimaTargetMask & 1 << i) != 0) {
                Battle_PostActorHitOrHurtMessage(i);
            }
        }
    }
    if (taskPtr->timer < 32) {
        taskPtr->colorRampUp += 0x80808;
        if (taskPtr->colorRampUp > 16777215) {
            taskPtr->colorRampUp = 0xFFFFFF;
        }
    } else {
        taskPtr->colorRampDown -= 0x20202;
        if (taskPtr->colorRampDown < 65793) {
            taskPtr->colorRampDown = 0x10101;
        }
    }
    (taskPtr->timer)++;
}

/* Animates spinning and fading model for Ultima. */
// FUNCTION: FF7 0x0057a082
void __cdecl BattleFx_MagicUltimaSpinFadeModelTask(void)
{
    BattleFxModelDraw *fxModelPtr;
    int u; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int v; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int w; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxUltimaSpinSlot *slotPtr;
    int y;
    int z; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    VECTOR curScaleZTerm;
    BattleFxModelDrawHead baseReqBuf;
    int e; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int f; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */

    slotPtr = (BattleFxUltimaSpinSlot *)BATTLE_CURRENT_TASK_SLOT;
    curScaleZTerm.vz = slotPtr->scale;
    curScaleZTerm.vy = curScaleZTerm.vz;
    curScaleZTerm.vx = curScaleZTerm.vy;
    ScaleMatrix(&g_BattleMagicUltimaMatrix, &curScaleZTerm);
    g_BattleMagicUltimaAngle = 0;
    (g_BattleMagicUltimaRotationVector.spinAngle) = g_BattleMagicUltimaAngle;
    g_BattleMagicUltimaRotationVector.y = slotPtr->step * -slotPtr->spinRate - 0x1F4;
    RotMatrixXYZ((short *)&g_BattleMagicUltimaRotationVector, &g_BattleMagicUltimaMatrix);
    SetRotMatrix(&g_BattleMagicUltimaMatrix);
    SetTransMatrix(&g_BattleMagicUltimaMatrix);
    RefreshGlobalTransformState();
    fxModelPtr = (BattleFxModelDraw *)&baseReqBuf;
    fxModelPtr->modelGroup = Gfx_ModelTableGetRenderObject(3, g_BattleMagicUltimaLoadedResTable);
    fxModelPtr->flags = 8;
    fxModelPtr->uvScroll = 0;
    fxModelPtr->fadeLevel = 128;
    fxModelPtr->stateFlags = 0x20;
    fxModelPtr->texturePageBits = 0;
    if (slotPtr->step == 40) {
        slotPtr->ownerSlot = BATTLE_TASK_FREE;
    }
    if (slotPtr->step >= 28) {
        fxModelPtr->fadeLevel -= (short)((slotPtr->step - 28) * 10);
    }
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitModelDraw(fxModelPtr, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (g_BattleIsPaused != 0) {
        return;
    }
    slotPtr->value += slotPtr->velocity;
    (slotPtr->step)++;
}

/* Renders Gouraud-shaded quad strip rows for Ultima. */
// FUNCTION: FF7 0x0057a20a
int __cdecl BattleFx_MagicUltimaDrawPsxGouraudQuadRows(BattleFxPsxGouraudRowBlock *rowBlock)
{
    int p; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int q; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int r; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int workLeftX;
    BattleFxPsxGouraudQuad *prim;
    int rightX;
    int yIndex;
    int nRows;
    int x; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int rowStep;
    int z; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int packedRgb;
    int b; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */

    prim = (BattleFxPsxGouraudQuad *)(rowBlock->primPtr);
    nRows = rowBlock->rowCount;
    workLeftX = 0;
    rightX = 319;
    rowStep = rowBlock->rowStep << 16;
    packedRgb = rowBlock->packedRgb;
    for (yIndex = 0; yIndex < nRows; yIndex++) {
        prim->leftX1 = workLeftX;
        prim->rightX1 = rightX;
        workLeftX += rowStep;
        rightX += rowStep;
        prim->leftX0 = workLeftX;
        prim->rightX0 = rightX;
        prim->color3 = packedRgb;
        prim->color2 = packedRgb;
        prim->color1 = packedRgb;
        prim->color0 = packedRgb;
        prim->tag = prim->tag & 0xFFFFFF | 0x8000000;
        ((BattleFxPsxGouraudQuadRow *)prim)->command = 0x3A;
        *(short *)&prim->leftX1 <<= 1;
        ((BattleFxPsxGouraudQuadRow *)prim)->vertex2Y <<= 1;
        *(short *)&prim->rightX0 <<= 1;
        ((BattleFxPsxGouraudQuadRow *)prim)->vertex1Y <<= 1;
        *(short *)&prim->leftX0 <<= 1;
        ((BattleFxPsxGouraudQuadRow *)prim)->vertex0Y <<= 1;
        *(short *)&prim->rightX1 <<= 1;
        ((BattleFxPsxGouraudQuadRow *)prim)->vertex3Y <<= 1;
        Gfx_SetBlendFactorPair(FLOAT_0_2_BITS, FLOAT_ONE_BITS);
        Gfx_DrawPsxPrimitive(prim);
    }
    return (int)prim;
}

/* Renders gradient Gouraud quad rows for Ultima. */
// FUNCTION: FF7 0x0057a38c
int __cdecl BattleFx_MagicUltimaDrawPsxGouraudQuadGradientRows(BattleFxUltimaGradientDesc *effectDesc, short skipRows)
{
    int vertexLeft;
    int unusedSpace; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int colorDelta;
    int unusedField[2]; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxPsxGouraudPrim *prim;
    int rowIdx;
    int posRight;
    int rowStep;
    int unusedWord; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int rowCnt;
    int stackWords; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int colAccum;
    int unusedHole; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */

    prim = (BattleFxPsxGouraudPrim *)(effectDesc->primPtr);
    rowCnt = effectDesc->rowCount;
    vertexLeft = 0x5A0000;
    posRight = 0x5A013F;
    rowStep = effectDesc->rowStep << 16;
    colAccum = 0;
    colorDelta = effectDesc->colorDelta;
    for (rowIdx = 0; rowIdx < rowCnt; rowIdx++) {
        if (rowIdx < skipRows) {
            vertexLeft -= rowStep;
            posRight -= rowStep;
            continue;
        }
        prim->vertex2 = vertexLeft;
        prim->vertex3 = posRight;
        vertexLeft -= rowStep;
        posRight -= rowStep;
        prim->vertex0 = vertexLeft;
        prim->vertex1 = posRight;
        prim->color3 = colAccum;
        prim->color2 = colAccum;
        colAccum += colorDelta;
        if (colAccum > 16777215) {
            colAccum = 0xFFFFFF;
        }
        prim->color1 = colAccum;
        prim->color0 = colAccum;
        prim->tag = prim->tag & 0xFFFFFF | 0x8000000;
        ((BattleFxGouraudQuadPrim *)prim)->primCode = 0x3A;
        *(short *)&prim->vertex2 <<= 1;
        ((BattleFxGouraudQuadPrim *)prim)->vertex2Y <<= 1;
        *(short *)&prim->vertex1 <<= 1;
        ((BattleFxGouraudQuadPrim *)prim)->vertex1Y <<= 1;
        *(short *)&prim->vertex0 <<= 1;
        ((BattleFxGouraudQuadPrim *)prim)->vertex0Y <<= 1;
        *(short *)&prim->vertex3 <<= 1;
        ((BattleFxGouraudQuadPrim *)prim)->vertex3Y <<= 1;
        Gfx_SetBlendFactorPair(FLOAT_0_2_BITS, FLOAT_ONE_BITS);
        Gfx_DrawPsxPrimitive(prim);
    }
    vertexLeft = 0x5A0000;
    posRight = 0x5A013F;
    colAccum = 0;
    colorDelta = effectDesc->colorDelta;
    for (rowIdx = 0; rowIdx < rowCnt; rowIdx++) {
        if (rowIdx < skipRows) {
            vertexLeft += rowStep;
            posRight += rowStep;
            continue;
        }
        prim->vertex0 = vertexLeft;
        prim->vertex1 = posRight;
        vertexLeft += rowStep;
        posRight += rowStep;
        prim->vertex2 = vertexLeft;
        prim->vertex3 = posRight;
        prim->color1 = colAccum;
        prim->color0 = colAccum;
        colAccum += colorDelta;
        if (colAccum > 16777215) {
            colAccum = 0xFFFFFF;
        }
        prim->color3 = colAccum;
        prim->color2 = colAccum;
        prim->tag = prim->tag & 0xFFFFFF | 0x8000000;
        ((BattleFxGouraudQuadPrim *)prim)->primCode = 0x3A;
        *(short *)&prim->vertex2 <<= 1;
        ((BattleFxGouraudQuadPrim *)prim)->vertex2Y <<= 1;
        *(short *)&prim->vertex1 <<= 1;
        ((BattleFxGouraudQuadPrim *)prim)->vertex1Y <<= 1;
        *(short *)&prim->vertex0 <<= 1;
        ((BattleFxGouraudQuadPrim *)prim)->vertex0Y <<= 1;
        *(short *)&prim->vertex3 <<= 1;
        ((BattleFxGouraudQuadPrim *)prim)->vertex3Y <<= 1;
        Gfx_SetBlendFactorPair(FLOAT_0_2_BITS, FLOAT_ONE_BITS);
        Gfx_DrawPsxPrimitive(prim);
    }
    return (int)prim;
}
} /* extern "C" */
