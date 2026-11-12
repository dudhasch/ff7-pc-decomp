// src/ff7/src/battle/battle_fx_special_swamp_shoot.cpp
/* Enemy Special Effect [idx 24]: SwampShoot (JP: geki1 / 撃1 (Strike 1), asset: "SPECIAL/GEKI1") — Swamp Shoot (0x1bc, Madouge) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle special effect for the Geki1 attack: it loads resources, registers and spawns a
//   per-target timeline that drives animated and bouncing sprites, a trail emitter and a fading
//   oriented model.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "ff7/api/battle.h"
#include "ff7/battle_tasks.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_special.h"
#include "ff7/camera_script.h"
#include "lib/types/graphics.h"
#include "lib/enums/sound.h"
#include <string.h>
#include <stdlib.h>
#include "lib/api/sound.h"
#include "lib/codegen.h"
extern "C" {

struct BattleFxModelDraw;
// clang-format off
// GLOBAL: FF7 0x008a36c8
int g_BattleSpecSwampShootResourceDefs = 0x001900B1;
int g_BattleSpecSwampShootResourceDefsHeadPtr[1] = {
    (int)((char *)&g_BattleFxSpecialSwampShootResourceNames + 0x0)
};
// Rows 0..23 are the resource table; rows 24 and 25 are the animated sprite request's resource and scene object.
// GLOBAL: FF7 0x008a36d0
int g_BattleSpecSwampShootResourceModelRows[26] = {
    0x0, 0x0, 0x0, 0x192a39, (int)((char *)&g_BattleFxSpecialSwampShootResourceNames + 0xc), (int)((char *)&g_BattleFxSpecialSwampShootResourceNames + 0x18),
    0x0, 0x0, 0x192a39, 0x0, (int)((char *)&g_BattleFxSpecialSwampShootResourceNames + 0x24), 0x0,
    0x0, 0x192a39, (int)((char *)&g_BattleFxSpecialSwampShootResourceNames + 0x30), (int)((char *)&g_BattleFxSpecialSwampShootResourceNames + 0x3c), 0x0, 0x0,
    0x192a39, (int)((char *)&g_BattleFxSpecialSwampShootResourceNames + 0x48), (int)((char *)&g_BattleFxSpecialSwampShootResourceNames + 0x54), 0x0, 0x0, 0x0,
    0x0, 0x0
};
// GLOBAL: FF7 0x008a3738
unsigned char g_BattleSpecSwampShootSpriteReq1[8] = {0x80, 0x80, 0x80, 0x2c, 0x00, 0x00, 0x00, 0x00};
#undef g_BattleSpecSwampShootSpriteReq2
// GLOBAL: FF7 0x008a3740
struct BattleSpecSwampShootSpriteReq2 g_BattleSpecSwampShootSpriteReq2 = {0, 0, {0x80, 0x80, 0x80, 0x2e}, 0, {0}, 0, 0};
// GLOBAL: FF7 0x008a3758
struct BattleSpecPoisonGasSpriteReq2 g_BattleSpecSwampShootSpriteReq3 = {-128, -128, -128, 44, 0, {0}};
// GLOBAL: FF7 0x008a3760
struct BattleSpecSwampShootModelParam g_BattleSpecSwampShootModelParam = {{0, 4, 0, 0xfc, 0, 0, 0, 0}, 0, {0x28, 0, 0, 0, 0, 0}, 128};
unsigned char g_rawData_008a3774[4] = {0x20, 0x00, 0x00, 0x00};
unsigned char g_BattleFxSpecialSwampShootResourceNames[96] =
    "GEKI1.RSD\0\0\0"
    "GEKI01.TIM\0\0"
    "BAKU1.SPT\0\0\0"
    "JO_C01.SPT\0\0"
    "GEKI00.TIM\0\0"
    "HAHEN1.SPT\0\0"
    "GEKI00.TIM\0\0"
    "KEMU1.SPT";
// GLOBAL: FF7 0x00b650c8
int g_BattleSpecSwampShootSourceBonePos = 0;
// GLOBAL: FF7 0x00b650cc
short g_BattleSpecSwampShootSourceBonePosZ = 0;
// GLOBAL: FF7 0x00b650d0
int g_BattleSpecSwampShootDrawListHead = 0;
// GLOBAL: FF7 0x00b650d4
struct GfxTableCursor *g_BattleSpecSwampShootLoadedResTable = 0;
// clang-format on

#pragma intrinsic(memset)
#pragma intrinsic(memcpy)

// clang-format off
// clang-format on


// Creates the SPECIAL/GEKI1 resource, type 5 from g_BattleSpecSwampShootResourceDefs, when the battle 3D
// scene exists and g_BattleSpecSwampShootLoadedResTable is still empty. The request is a
// default-duration record with groupTag 0, and its resourcePath is built by
// BattleFx_BuildResourcePath.
// FUNCTION: FF7 0x0053b2b0
void __cdecl BattleFx_SpecialLoadResSwampShoot(void)
{
    GfxModelTableBank *stage;
    GfxResourceRequest currentRecord;
    int kind;
    char pathText[200];
    GfxResourceRequest *pRequest;
    int resourceDefsWord;

    if (g_pBattle3dScene != 0 && g_BattleSpecSwampShootLoadedResTable == 0) {
        kind = 5;
        resourceDefsWord = (int)&g_BattleSpecSwampShootResourceDefs;
        stage = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pRequest = &currentRecord;
        Gfx_InitResourceRequest(pRequest);
        pRequest->groupTag = 0;
        // STRING: FF7 0x008a37d8
        pRequest->resourcePath = BattleFx_BuildResourcePath("SPECIAL/GEKI1", pathText);
        Gfx_LoadModelTableIfAbsent(kind, resourceDefsWord, pRequest, &g_BattleSpecSwampShootLoadedResTable, stage);
    }
}

// Entry point for the Geki1 special. It loads the Geki1 resources, then registers the per-target
// callback with the same actor mask and target index. This is a mechanical wrapper with no logic of
// its own.
// FUNCTION: FF7 0x0053b340
void __cdecl BattleFx_SpecialEnterSwampShoot(int targetMask, int actorIdx)
{
    BattleFx_SpecialLoadResSwampShoot();
    BattleFx_SpecialRegisterSwampShootPerTargetCallback(targetMask, actorIdx);
}

// Registers BattleFx_SpecialSpawnSwampShootPerTarget as a bit-pattern callback task for the actor mask
// and target index, with mode 4. It then calls SoundDispatchCommand with opcode 0x28 on channel 1,
// pan 0x40, sound 0x173 and six zero arguments.
// FUNCTION: FF7 0x0053b35a
void __cdecl BattleFx_SpecialRegisterSwampShootPerTargetCallback(int actorMask, int targetIndex)
{
    Battle_CreateBitPatternCallbackTask(actorMask, targetIndex, 4, BattleFx_SpecialSpawnSwampShootPerTarget);
    SoundDispatchCommand(SOUND_SFX_PLAY_CH1, 0x40, 0x173, 0, 0, 0, 0, 0, 0);
}

// Allocates a slot for BattleFx_SpecialSwampShootTimelineTask. It writes the source actor's targetBone
// world position to g_BattleSpecSwampShootSourceBonePos and the target actor's targetBone world position
// to the timeline's posX, posY and posZ, then stores actor in actorIndex.
// FUNCTION: FF7 0x0053b393
void __cdecl BattleFx_SpecialSpawnSwampShootPerTarget(int actor, int sourceIndex)
{
    BattleFxSwampShootTimeline *taskSlot;

    taskSlot = (BattleFxSwampShootTimeline *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialSwampShootTimelineTask);
    Battle_GetBoneWorldPos(sourceIndex, g_BattleActorRecords[sourceIndex].targetBone, (short *)&g_BattleSpecSwampShootSourceBonePos);
    Battle_GetBoneWorldPos(actor, g_BattleActorRecords[actor].targetBone, &taskSlot->posX);
    taskSlot->actorIndex = actor;
}

// Timeline driver for the Geki1 effect, run once per unpaused frame from the slot's timer. At timer
// 0 it starts a trail emitter at the source bone position, with a velocity of one tenth of the
// distance to the target in X and Z. At timer 10 it spawns eight bouncing sprites scattered around
// the target and plays sound 0x14 on channel 2. Between timers 7 and 13 it spawns two animated
// sprites near the target and moves the camera focal point's Y by plus or minus 0x20 with the
// timer's parity. At timer 12 it allocates a typed record of the slot's allocType. The timer
// advances each frame and the slot retires at timer 0x23.
// FUNCTION: FF7 0x0053b407
void __cdecl BattleFx_SpecialSwampShootTimelineTask(void)
{
    short idx;
    short heading;
    BattleFxSwampShootParticle *pParticle;
    int s; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int t; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int u; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int v; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int w; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int x; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int y; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int z; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxSwampShootTimeline *mainSelf;
    int b; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int c; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int d; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int e; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int f; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int g; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */

    mainSelf = (BattleFxSwampShootTimeline *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (mainSelf->timer == 0) {
        pParticle = (BattleFxSwampShootParticle *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialSwampShootTrailEmitterTask);
        pParticle->posX = *(short *)&g_BattleSpecSwampShootSourceBonePos;
        pParticle->posY = 0;
        pParticle->posZ = g_BattleSpecSwampShootSourceBonePosZ;
        pParticle->velocityX = (mainSelf->posX - *(short *)&g_BattleSpecSwampShootSourceBonePos) / 0xA;
        pParticle->velocityZ = (mainSelf->posZ - g_BattleSpecSwampShootSourceBonePosZ) / 10;
        pParticle->velocityY = 0;
    }
    if (mainSelf->timer == 10) {
        for (idx = 0; idx < 8; idx++) {
            pParticle = (BattleFxSwampShootParticle *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialSwampShootBouncingSpriteTask);
            pParticle->posX = mainSelf->posX + rand() % 400 + -200;
            pParticle->posZ = mainSelf->posZ + rand() % 400 + -200;
            pParticle->posY = 0;
            heading = rand() % 0x1000;
            pParticle->velocityX = CosFixed(heading) * 0x64 >> 12;
            pParticle->velocityZ = SinFixed(heading) * 0x64 >> 12;
            pParticle->velocityY = -rand() % 200 - 100;
            pParticle->mode = rand() % 20;
        }
        SoundDispatchCommand(SOUND_SFX_PLAY_CH2, 0x40, 0x14, 0, 0, 0, 0, 0, 0);
    }
    if (mainSelf->timer > 6 && mainSelf->timer < 14) {
        pParticle = (BattleFxSwampShootParticle *)&g_BattleTaskSlotBuffer.secondarySlots[(short)Battle_AllocateSecondaryResourceSlot(BattleFx_SpecialSwampShootAnimatedSpriteTask)];
        pParticle->posX = mainSelf->posX + rand() % 1200 + -600;
        pParticle->posY = mainSelf->posY + rand() % 1200 + -600;
        pParticle->posZ = mainSelf->posZ + rand() % 1200 + -600;
        pParticle->mode = 0;
        pParticle = (BattleFxSwampShootParticle *)&g_BattleTaskSlotBuffer.secondarySlots[(short)Battle_AllocateSecondaryResourceSlot(BattleFx_SpecialSwampShootAnimatedSpriteTask)];
        pParticle->posX = mainSelf->posX + rand() % 1600 + -800;
        pParticle->posY = mainSelf->posY + rand() % 1600 + -800;
        pParticle->posZ = mainSelf->posZ + rand() % 1600 + -800;
        pParticle->mode = 1;
        g_BattleCameraFocalPoint.focalPointY += (short)(32 - (mainSelf->timer % 2 << 6));
    }
    if (mainSelf->timer == 12) {
        Battle_PostActorHitOrHurtMessage(mainSelf->actorIndex);
    }
    (mainSelf->timer)++;
    if (mainSelf->timer == 35) {
        mainSelf->ownerSlot = BATTLE_TASK_FREE;
    }
}

// Per-frame sprite handler for the secondary effect slot g_BattleSecondaryTaskActiveSlotIdx. It
// installs a billboard matrix for the slot at scale 0x2000 and takes the resource from table entry
// {1,2}[variant]. The scene object comes from that entry's Alt field, except for variant 1, which
// takes Battle3d_GetSceneRenderObject(3, g_pBattle3dScene). It copies the frame counter into the request
// and submits the sprite to g_BattleOverlayDrawListHead. When the battle is unpaused and the
// resource is non-null, it advances the frame and retires the slot with 0xFFFF once the frame
// reaches the count at [[resource]+4].
// FUNCTION: FF7 0x0053b7a2
void __cdecl BattleFx_SpecialSwampShootAnimatedSpriteTask(void)
{
    BattleFxTailLaserAnimSprite *task;
    int entryIds[2];

    task = (BattleFxTailLaserAnimSprite *)&g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx];
    entryIds[0] = 1;
    entryIds[1] = 2;
    BattleFx_InstallBillboardMatrix(&task->posX, 0x2000 /* 2.0 */, 0);
    g_BattleSpecSwampShootResourceModelRows[24] = Gfx_ModelTableGetLoadedRecord(entryIds[task->variant], g_BattleSpecSwampShootLoadedResTable);
    if (task->variant != 1) {
        g_BattleSpecSwampShootResourceModelRows[25] = Gfx_ModelTableGetSpriteObject(entryIds[task->variant], g_BattleSpecSwampShootLoadedResTable);
    } else {
        g_BattleSpecSwampShootResourceModelRows[25] = Battle3d_GetSceneRenderObject(3, g_pBattle3dScene);
    }
    *(short *)((char *)&g_BattleSpecSwampShootSpriteReq1 + 0x4) = task->frame;
    g_BattleOverlayDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecSwampShootResourceModelRows[24], g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused == 0 && g_BattleSpecSwampShootResourceModelRows[24] != 0) {
        (task->frame)++;
        if (task->frame >= *(int *)((char *)*(int *)g_BattleSpecSwampShootResourceModelRows[24] + 4)) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

// Per-frame handler for a bouncing sprite in the active main effect slot. It draws table entry 3,
// rotated by the slot's phase, at a scale of 0x1400 for variants below 4 and 0x2000 otherwise, and
// submits it to the draw list even while paused. The request's fields come from the variant and
// from a step value of 8 while the frame counter is below 7, then 0xF minus the counter. The locals
// sit in a packed union that reproduces the original's stack layout. Unless the battle is paused,
// the position then advances by the velocity.
// FUNCTION: FF7 0x0053b8ba
void __cdecl BattleFx_SpecialSwampShootBouncingSpriteTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[92];
        int stepValue;
        struct { unsigned char pad_to_4[0x4]; SVECTOR rotX; };
        struct { unsigned char pad_to_C[0xC]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_2C[0x2C]; short stepShort; };
        struct { unsigned char pad_to_30[0x30]; PsxMatrix scaleMatrix; };
        struct { unsigned char pad_to_50[0x50]; short axisIndex; };
        struct { unsigned char pad_to_54[0x54]; int depth; };
        struct { unsigned char pad_to_58[0x58]; BattleFxPoisonGasBouncingSpriteSlot *bounceSlot; };
    } frame;
#pragma pack(pop)

    frame.bounceSlot = (BattleFxPoisonGasBouncingSpriteSlot *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecSwampShootSpriteReq2.secondSpriteRecord = Gfx_ModelTableGetLoadedRecord(3, g_BattleSpecSwampShootLoadedResTable);
    g_BattleSpecSwampShootSpriteReq2.secondDrawObj = Gfx_ModelTableGetSpriteObject(3, g_BattleSpecSwampShootLoadedResTable);
    for (frame.axisIndex = 0; frame.axisIndex < 3; frame.axisIndex++) {
        frame.scaleMatrix.m[2][frame.axisIndex] = 0;
        frame.scaleMatrix.m[1][frame.axisIndex] = 0;
        frame.scaleMatrix.m[0][frame.axisIndex] = 0;
    }
    if (frame.bounceSlot->frameCounter < 7) {
        frame.stepValue = 8;
    } else {
        frame.stepValue = 15 - frame.bounceSlot->frameCounter;
    }
    frame.stepShort = *(short *)&frame.stepValue;
    frame.rotX.vy = 0;
    frame.rotX.vx = frame.rotX.vy;
    frame.rotX.vz = frame.bounceSlot->phase;
    SetRotMatrix(&g_BattleCameraTransform);
    SetTransMatrix(&g_BattleCameraTransform);
    RotTransPoint((char *)frame.bounceSlot + 4, frame.scaleMatrix.t, &frame.depth);
    frame.scaleMatrix.m[2][2] = frame.bounceSlot->variant < 4 ? 0x1400 : 0x2000;
    frame.scaleMatrix.m[1][1] = frame.scaleMatrix.m[2][2];
    frame.scaleMatrix.m[0][0] = frame.scaleMatrix.m[1][1];
    RotMatrix(&frame.rotX, &frame.rotMatrix);
    ConcatMatrixIntoFirst(&frame.scaleMatrix, &frame.rotMatrix);
    SetRotMatrix(&frame.scaleMatrix);
    SetTransMatrix(&frame.scaleMatrix);
    g_BattleSpecSwampShootSpriteReq3.frame = frame.bounceSlot->variant;
    g_BattleSpecSwampShootSpriteReq3.code = (frame.stepShort - 8 != 0 ? 2 : 0) + 0x2C;
    g_BattleSpecSwampShootSpriteReq3.blue = frame.stepShort << 4;
    g_BattleSpecSwampShootSpriteReq3.green = g_BattleSpecSwampShootSpriteReq3.blue;
    (g_BattleSpecSwampShootSpriteReq3.red) = g_BattleSpecSwampShootSpriteReq3.green;
    g_BattleOverlayDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecSwampShootSpriteReq2.secondSpriteRecord, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.bounceSlot->posX += frame.bounceSlot->velocityX;
    frame.bounceSlot->posY += frame.bounceSlot->velocityY;
    frame.bounceSlot->posZ += frame.bounceSlot->velocityZ;
    if (frame.bounceSlot->posY > 0) {
        frame.bounceSlot->posY = 0;
        frame.bounceSlot->velocityY *= -1;
    }
    frame.bounceSlot->velocityY += 20;
    frame.bounceSlot->phase += 0x200;
    (frame.bounceSlot->frameCounter)++;
    if (frame.bounceSlot->frameCounter == 15) {
        frame.bounceSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

// Trail emitter for the Geki1 effect, run once per unpaused frame. It spawns a fading oriented
// model with the parent X and Z in its velocity fields and the parent's counter as frameIndex, then
// integrates the parent's X and Z by its velocity. It also spawns a bouncing sprite with a random
// radial velocity of magnitude 100, a downward Y velocity of 100 to 299 and a frameIndex of 4 to
// 19, and an animated sprite at a random XZ offset of -300 to 299. The parent's counter advances,
// and the slot retires when it equals exactly 12.
// FUNCTION: FF7 0x0053bb4e
void __cdecl BattleFx_SpecialSwampShootTrailEmitterTask(void)
{
    BattleFxSwampShootDriver *taskPtr;
    short angle;
    BattleFxSwampShootTrailEmitter *child;

    taskPtr = (BattleFxSwampShootDriver *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    child = (BattleFxSwampShootTrailEmitter *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialSwampShootFadingOrientedModelTask);
    child->velocityX = taskPtr->posX;
    child->velocityZ = taskPtr->posZ;
    child->velocityY = 0;
    child->frameIndex = taskPtr->frameIndex;
    taskPtr->posX += taskPtr->velocityX;
    taskPtr->posZ += taskPtr->velocityZ;
    child = (BattleFxSwampShootTrailEmitter *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialSwampShootBouncingSpriteTask);
    child->posX = taskPtr->posX;
    child->posZ = taskPtr->posZ;
    child->posY = 0;
    angle = rand() % 0x1000;
    child->velocityX = CosFixed(angle) * 0x64 >> 12;
    child->velocityZ = SinFixed(angle) * 0x64 >> 12;
    child->velocityY = -rand() % 200 - 100;
    child->frameIndex = rand() % 16 + 4;
    child = (BattleFxSwampShootTrailEmitter *)BATTLE_NEW_TASK_SLOT(BattleFx_SpecialSwampShootAnimatedSpriteEntry4Task);
    child->posX = taskPtr->posX + rand() % 600 + -300;
    child->posZ = taskPtr->posZ + rand() % 600 + -300;
    child->posY = 0;
    (taskPtr->frameIndex)++;
    if (taskPtr->frameIndex == 12) {
        taskPtr->ownerSlot = BATTLE_TASK_FREE;
    }
}

// Per-frame step for the active main effect slot. It takes table entry 4 of
// g_BattleSpecSwampShootLoadedResTable into the sprite request and installs a billboard matrix for the
// slot at scale 0x2000. It copies the frame index into the request's word12 and submits the sprite
// at shift 0xC, with the cursor in g_BattleSpecSwampShootDrawListHead. When the battle is unpaused and
// the sprite is non-null, it advances the frame index and stores 0xFFFF in ownerSlot at the
// animation's frame count. One unreferenced local preserves the retail VC5 /Od stack slot.
// FUNCTION: FF7 0x0053bd44
void __cdecl BattleFx_SpecialSwampShootAnimatedSpriteEntry4Task(void)
{
    BattleTaskSlot *p;
    int q; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */

    p = BATTLE_CURRENT_TASK_SLOT;
    (g_BattleSpecSwampShootSpriteReq2.spriteEntry) = Gfx_ModelTableGetLoadedRecord(4, g_BattleSpecSwampShootLoadedResTable);
    g_BattleSpecSwampShootSpriteReq2.drawObj = Gfx_ModelTableGetSpriteObject(4, g_BattleSpecSwampShootLoadedResTable);
    BattleFx_InstallBillboardMatrix((short *)p->data, 0x2000 /* 2.0 */, 0);
    g_BattleSpecSwampShootSpriteReq2.frame = p->frameCounter;
    g_BattleSpecSwampShootDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleSpecSwampShootSpriteReq2, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecSwampShootDrawListHead);
    if (g_BattleIsPaused == 0 && (g_BattleSpecSwampShootSpriteReq2.spriteEntry) != 0) {
        (p->frameCounter)++;
        if (p->frameCounter >= *(int *)((char *)*(int *)(g_BattleSpecSwampShootSpriteReq2.spriteEntry) + 4)) {
            p->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

// Per-frame draw of a model oriented from the source bone toward the slot's position, with a
// look-at basis built from the direction between them. The scale comes from the slot's trailIndex field,
// and word18 of the model parameters falls from 0x60 by 0x18 per frame. The model is submitted at
// shift 0xC with the cursor in g_BattleSpecSwampShootDrawListHead. Unless the battle is paused, the
// frame counter advances and the slot retires with 0xFFFF when the counter reaches 4.
// FUNCTION: FF7 0x0053be17
void __cdecl BattleFx_SpecialSwampShootFadingOrientedModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[128];
        PsxMatrix rotMatrix;
        struct { unsigned char pad_to_20[0x20]; SVECTOR dirX; };
        struct { unsigned char pad_to_28[0x28]; BattleFxGrandSparkStreakModelSlot *fadingSlot; };
        struct { unsigned char pad_to_2C[0x2C]; PsxMatrix lookBasis; };
        struct { unsigned char pad_to_4C[0x4C]; PsxMatrix combinedMatrix; };
        struct { unsigned char pad_to_70[0x70]; VECTOR scaleX; };
    } frame;
#pragma pack(pop)

    frame.fadingSlot = (BattleFxGrandSparkStreakModelSlot *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleSpecSwampShootModelParam.modelResource = Gfx_ModelTableGetRenderObject(0, g_BattleSpecSwampShootLoadedResTable);
    frame.dirX.vx = frame.fadingSlot->posX - *(short *)&g_BattleSpecSwampShootSourceBonePos;
    frame.dirX.vy = 0;
    frame.dirX.vz = frame.fadingSlot->posZ - g_BattleSpecSwampShootSourceBonePosZ;
    Gte_BuildLookBasisFromDirection(&frame.dirX.vx, frame.lookBasis.m[0]);
    RotMatrix((SVECTOR *)&g_BattleSpecSwampShootModelParam, &frame.rotMatrix);
    ConcatMatrixPreservingTranslation(&frame.lookBasis, &frame.rotMatrix, &frame.rotMatrix);
    frame.scaleX.vz = frame.fadingSlot->trailIndex * 0x180 + 0xE00;
    frame.scaleX.vy = frame.scaleX.vz;
    frame.scaleX.vx = frame.scaleX.vy;
    ScaleMatrix(&frame.rotMatrix, &frame.scaleX);
    frame.rotMatrix.t[0] = frame.fadingSlot->posX;
    frame.rotMatrix.t[1] = 0;
    frame.rotMatrix.t[2] = frame.fadingSlot->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.combinedMatrix);
    SetRotMatrix(&frame.combinedMatrix);
    SetTransMatrix(&frame.combinedMatrix);
    g_BattleSpecSwampShootModelParam.alpha = 0x60 - frame.fadingSlot->frameCounter * 24;
    g_BattleSpecSwampShootDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleSpecSwampShootModelParam.modelResource, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleSpecSwampShootDrawListHead);
    if (g_BattleIsPaused != 0) {
        return;
    }
    (frame.fadingSlot->frameCounter)++;
    if (frame.fadingSlot->frameCounter == 4) {
        frame.fadingSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}
} /* extern "C" */
