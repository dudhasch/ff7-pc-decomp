// src/ff7/src/battle/battle_fx_blue_trine_effect.cpp
// Enemy Skill [14]: Trine (Japanese: トライン, dev tag: "BLUE/TRAINN" / Trainn)
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: The Shadow Flare blue magic effect: spinning, growing models around the target that
//   flash or fade.

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
#include "lib/codegen.h"
extern "C" {

struct BattleFxModelDraw;
#pragma intrinsic(memset)
#pragma intrinsic(memcpy)


// clang-format off
// GLOBAL: FF7 0x008abe80
int g_BattleBlueTrineResourceDefs = 0x001900B1;
int g_BattleBlueTrineResourceDefsHeadPtr[1] = {
    (int)((char *)&g_BattleFxBlueTrineEffectResourceNames + 0xc)
};
unsigned char g_BattleFxBlueTrineEffectResourceNames[28] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x74, 0x72, 0x61, 0x69, 0x6e, 0x6e, 0x5f, 0x31, 0x2e, 0x72, 0x73, 0x64,
    0x00, 0x00, 0x00, 0x00
};
// clang-format on

// clang-format off
// GLOBAL: FF7 0x00ba8260
BattleFxModelDrawHead g_BattleBlueTrineModelReq = {0};
// GLOBAL: FF7 0x00ba8270
int g_BattleBlueTrineTargetMask = 0;
// GLOBAL: FF7 0x00ba8274
struct GfxTableCursor *g_BattleBlueTrineLoadedResTable = 0;
// clang-format on

// Loads the Shadow Flare (Trainn) blue magic resources, but only when a battle 3D scene exists and
// the resource table is still empty. It requests the BLUE/TRAINN resource path through the scene's
// model table bank.
// FUNCTION: FF7 0x005743c0
void __cdecl BattleFx_BlueLoadResTrine(void)
{
    GfxModelTableBank *sceneHeaderVal;
    GfxResourceRequest reqEntRec;
    int kind;
    char resPathBuf[200];
    GfxResourceRequest *pRequest;
    int resourceDefsWord;

    if (g_pBattle3dScene != 0 && g_BattleBlueTrineLoadedResTable == 0) {
        kind = 1;
        resourceDefsWord = (int)&g_BattleBlueTrineResourceDefs;
        sceneHeaderVal = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRequest = &reqEntRec;
        Gfx_InitResourceRequest(pRequest);
        pRequest->groupTag = 0;
        // STRING: FF7 0x008abea4
        pRequest->resourcePath = BattleFx_BuildResourcePath("BLUE/TRAINN", resPathBuf);
        Gfx_LoadModelTableIfAbsent(kind, resourceDefsWord, pRequest, &g_BattleBlueTrineLoadedResTable, sceneHeaderVal);
    }
}

// Entry point for the Shadow Flare (Trainn) blue magic effect. It starts the effect for the target
// mask; the caster index is not used.
// FUNCTION: FF7 0x00574450
void __cdecl BattleFx_BlueEnterTrine(int targetMask, int actorIdx)
{
    BattleFx_BlueBeginTrine(targetMask, actorIdx);
}

// Initializes the setup task for Shadow Flare (Trainn). It allocates a draw-list buffer task and
// stores its pointer, loads the Trainn resources, spawns the setup task, and stores the target
// mask. It then plays a sound panned for the target.
// FUNCTION: FF7 0x00574465
void __cdecl BattleFx_BlueBeginTrine(int targetMask, int actorIdx)
{
    *(int *)&g_BattleFxDrawListBufferTaskSlotPtr = (int)BATTLE_NEW_TASK_SLOT(BattleFx_SelectDrawListBuffer);
    BattleFx_BlueLoadResTrine();
    Battle_AllocateResourceSlot(BattleFx_BlueTrineSetupTask);
    g_BattleBlueTrineTargetMask = targetMask;
    Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputePanForEntity(targetMask), 0xA0);
}

// Sets up the Shadow Flare target centroid and spawns its two spinning model tasks. The setup
// slot's owner is cleared. The spin, grow and flash task receives the target centroid and fixed
// initial spin, position and flash values. The spin, grow and fade task copies that position and
// gets its own fixed initial spin and scale values.
// FUNCTION: FF7 0x005744be
void __cdecl BattleFx_BlueTrineSetupTask(void)
{
    int scaleTarget, scaleShifted;
    BattleFxTrineSetupTask *task;
    BattleFxBlueTrineSpinState *pFadeModel;

    task = (BattleFxTrineSetupTask *)BATTLE_CURRENT_TASK_SLOT;
    task->ownerSlot = BATTLE_TASK_FREE;
    task = (BattleFxTrineSetupTask *)BATTLE_NEW_TASK_SLOT(BattleFx_BlueTrineSpinGrowFlashModelTask);
    Battle_GetActorMaskCentroid(g_BattleBlueTrineTargetMask, (short *)((int)task + 4));
    task->posY = 0xFC80;
    scaleTarget = 0x6C0;
    scaleShifted = scaleTarget << 12;
    task->spinX = 0;
    task->spinStepX = 0xFFD8 /* -40 */;
    task->spinY = 0;
    task->spinStepY = 8;
    task->spinZ = 0;
    task->spinStepZ = 0xFFD8 /* -40 */;
    task->scale = 0;
    task->scaleStep = scaleShifted / 750 / 18;
    pFadeModel = (BattleFxBlueTrineSpinState *)BATTLE_NEW_TASK_SLOT(BattleFx_BlueTrineSpinGrowFadeModelTask);
    *(Int64Pair *)((char *)pFadeModel + 4) = *(Int64Pair *)((char *)task + 4);
    scaleTarget = 0x2D0;
    scaleShifted = scaleTarget << 12;
    pFadeModel->spinX = 0;
    pFadeModel->spinStepX = 40;
    pFadeModel->spinY = 0;
    pFadeModel->spinStepY = 0xFFF8 /* -8 */;
    pFadeModel->spinZ = 0;
    pFadeModel->spinStepZ = 40;
    pFadeModel->scale = 0;
    pFadeModel->scaleStep = scaleShifted / 750 / 18;
}

// Animates the spinning, growing and flashing model of Shadow Flare. It builds a matrix from the
// spin angles, the position and a scale taken from the flash level, multiplies it by the camera
// transform, and sets up the model request. The model's fade level drops by 0xA per frame after
// frame counter 0x2C, floored at zero, and the model is submitted for drawing. Frames below 8 set
// the channel fields for the flash, and from frame 0x34 the flash amount falls back toward zero.
// FUNCTION: FF7 0x0057461c
void __cdecl BattleFx_BlueTrineSpinGrowFlashModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[72];
        SVECTOR spinX;
        struct { unsigned char pad_to_8[0x8]; BattleFxTrineSpinGrowFadeModelSlot *trainnSlot; };
        struct { unsigned char pad_to_C[0xC]; int targetIndex; };
        struct { unsigned char pad_to_10[0x10]; PsxMatrix matrix; };
        struct { unsigned char pad_to_30[0x30]; BattleFxModelDraw *modelReq; };
        struct { unsigned char pad_to_34[0x34]; VECTOR scaleX; };
        struct { unsigned char pad_to_44[0x44]; short flashAmount; };
    } frame;
#pragma pack(pop)

    frame.trainnSlot = (BattleFxTrineSpinGrowFadeModelSlot *)BATTLE_CURRENT_TASK_SLOT;
    frame.spinX.vx = frame.trainnSlot->spinX;
    frame.spinX.vy = frame.trainnSlot->spinY;
    frame.spinX.vz = frame.trainnSlot->spinZ;
    RotMatrixXYZ(&frame.spinX, &frame.matrix);
    frame.matrix.t[0] = frame.trainnSlot->posX;
    frame.matrix.t[1] = frame.trainnSlot->posY;
    frame.matrix.t[2] = frame.trainnSlot->posZ;
    frame.scaleX.vz = frame.trainnSlot->scale;
    frame.scaleX.vy = frame.scaleX.vz;
    frame.scaleX.vx = frame.scaleX.vy;
    ScaleMatrix(&frame.matrix, &frame.scaleX);
    MultiplyMatrix(&g_BattleCameraTransform, &frame.matrix, &frame.matrix);
    SetRotMatrix(&frame.matrix);
    SetTransMatrix(&frame.matrix);
    frame.modelReq = (BattleFxModelDraw *)&g_BattleBlueTrineModelReq;
    frame.modelReq->modelGroup = Gfx_ModelTableGetRenderObject(0, g_BattleBlueTrineLoadedResTable);
    frame.modelReq->flags = 8;
    frame.modelReq->uvScroll = 0;
    frame.modelReq->fadeLevel = 128;
    frame.modelReq->stateFlags = 0x20;
    frame.modelReq->texturePageBits = 0;
    if (frame.trainnSlot->frameCounter >= 44) {
        frame.modelReq->fadeLevel -= (short)((frame.trainnSlot->frameCounter - 44) * 10);
        if (frame.modelReq->fadeLevel < 0) {
            frame.modelReq->fadeLevel = 0;
        }
    }
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitModelDraw((BattleFxModelDraw *)frame.modelReq, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (frame.trainnSlot->frameCounter < 8) {
        g_BattleFxTimelineState.channel1 = 0x30;
        g_BattleFxTimelineState.red = g_BattleFxTimelineState.channel1;
        g_BattleFxTimelineState.channel2 = 0;
        frame.flashAmount = frame.trainnSlot->frameCounter * 384;
    } else if (frame.trainnSlot->frameCounter >= 52) {
        frame.flashAmount = 3072 - (frame.trainnSlot->frameCounter - 52) * 384;
        if (frame.flashAmount < 0) {
            frame.flashAmount = 0;
        }
    } else {
        frame.flashAmount = 3072;
    }
    if (frame.trainnSlot->frameCounter >= 60) {
        frame.flashAmount = 0;
        g_BattleFxTimelineState.channel1 = *(char *)&frame.flashAmount;
        g_BattleFxTimelineState.red = g_BattleFxTimelineState.channel1;
        frame.trainnSlot->ownerSlot = BATTLE_TASK_FREE;
    }
    g_BattleFxTimelineState.startValue = frame.flashAmount;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.trainnSlot->frameCounter == 49) {
        for (frame.targetIndex = 0; frame.targetIndex < 0xA; frame.targetIndex++) {
            if ((g_BattleBlueTrineTargetMask & 1 << frame.targetIndex) != 0) {
                Battle_PostActorHitOrHurtMessage(frame.targetIndex);
            }
        }
    }
    frame.trainnSlot->spinX += frame.trainnSlot->spinStepX;
    frame.trainnSlot->spinY += frame.trainnSlot->spinStepY;
    frame.trainnSlot->spinZ += frame.trainnSlot->spinStepZ;
    if (frame.trainnSlot->frameCounter < 18) {
        frame.trainnSlot->scale += frame.trainnSlot->scaleStep;
    }
    (frame.trainnSlot->frameCounter)++;
}

// Animates the spinning, growing and fading model for Shadow Flare in the current main task slot
// and draws it each frame. The spin angles build the rotation matrix, the position builds the
// translation, and the slot's fadeLevel sets the scale. The draw request's fade level starts at
// 0x80 and drops by ten per frame from frame 0x2C. The slot retires at frame 0x3C, and the spin and
// fade step only while g_BattleIsPaused is clear.
// FUNCTION: FF7 0x00574901
void __cdecl BattleFx_BlueTrineSpinGrowFadeModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[64];
        SVECTOR spinX;
        struct { unsigned char pad_to_8[0x8]; BattleFxTrineSpinGrowFadeModelSlot *trainnSlot; };
        struct { unsigned char pad_to_C[0xC]; PsxMatrix matrix; };
        struct { unsigned char pad_to_2C[0x2C]; BattleFxModelDraw *modelReq; };
        struct { unsigned char pad_to_30[0x30]; VECTOR scaleX; };
    } frame;
#pragma pack(pop)

    frame.trainnSlot = (BattleFxTrineSpinGrowFadeModelSlot *)BATTLE_CURRENT_TASK_SLOT;
    frame.spinX.vx = frame.trainnSlot->spinX;
    frame.spinX.vy = frame.trainnSlot->spinY;
    frame.spinX.vz = frame.trainnSlot->spinZ;
    RotMatrixXYZ(&frame.spinX, &frame.matrix);
    frame.matrix.t[0] = frame.trainnSlot->posX;
    frame.matrix.t[1] = frame.trainnSlot->posY;
    frame.matrix.t[2] = frame.trainnSlot->posZ;
    frame.scaleX.vz = frame.trainnSlot->scale;
    frame.scaleX.vy = frame.scaleX.vz;
    frame.scaleX.vx = frame.scaleX.vy;
    ScaleMatrix(&frame.matrix, &frame.scaleX);
    MultiplyMatrix(&g_BattleCameraTransform, &frame.matrix, &frame.matrix);
    SetRotMatrix(&frame.matrix);
    SetTransMatrix(&frame.matrix);
    frame.modelReq = (BattleFxModelDraw *)&g_BattleBlueTrineModelReq;
    frame.modelReq->modelGroup = Gfx_ModelTableGetRenderObject(0, g_BattleBlueTrineLoadedResTable);
    frame.modelReq->flags = 8;
    frame.modelReq->uvScroll = 0;
    frame.modelReq->fadeLevel = 128;
    frame.modelReq->stateFlags = 0x20;
    frame.modelReq->texturePageBits = 0;
    if (frame.trainnSlot->frameCounter >= 44) {
        frame.modelReq->fadeLevel -= (short)((frame.trainnSlot->frameCounter - 44) * 10);
        if (frame.modelReq->fadeLevel < 0) {
            frame.modelReq->fadeLevel = 0;
        }
    }
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitModelDraw((BattleFxModelDraw *)frame.modelReq, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (frame.trainnSlot->frameCounter >= 60) {
        frame.trainnSlot->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.trainnSlot->spinX += frame.trainnSlot->spinStepX;
    frame.trainnSlot->spinY += frame.trainnSlot->spinStepY;
    frame.trainnSlot->spinZ += frame.trainnSlot->spinStepZ;
    if (frame.trainnSlot->frameCounter < 18) {
        frame.trainnSlot->scale += frame.trainnSlot->scaleStep;
    }
    (frame.trainnSlot->frameCounter)++;
}
} /* extern "C" */
