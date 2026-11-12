// src/ff7/src/battle/battle_fx_weapon_emerald_shoot.cpp
/* Enemy Special Attack [152]: Emerald Weapon Emerald Shoot (orig. weapon/weapon3) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle effect for the third weapon attack: it loads its resources and runs a timeline
//   that drives projectile models, spinning and expanding sprites, and trails toward the targets.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/enums/sound.h"
#include "ff7/battle_tasks.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_fx.h"
#include "lib/types/graphics.h"
#include "ff7/camera_script.h"
#include "ff7/api/battle.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "lib/codegen.h"
#include <stdlib.h>
extern "C" {

struct BattleFxModelDraw;
// clang-format off
// GLOBAL: FF7 0x007d8530
int g_WeaponFxEmeraldShootResourceRecords = 0x00192A39;
int g_WeaponFxEmeraldShootResourceRecordsHeadPtr[1] = {
    (int)((char *)&g_BattleFxEmeraldShootProjectileResourceNames + 0x0)
};
int g_WeaponFxEmeraldShootResourceRecordsDataTail[34] = {
    (int)((char *)&g_BattleFxEmeraldShootProjectileResourceNames + 0xc), 0x0, 0x0, 0x192a39, (int)((char *)&g_BattleFxEmeraldShootProjectileResourceNames + 0x18), (int)((char *)&g_BattleFxEmeraldShootProjectileResourceNames + 0x24),
    0x0, 0x0, 0x192a39, (int)((char *)&g_BattleFxEmeraldShootProjectileResourceNames + 0x30), (int)((char *)&g_BattleFxEmeraldShootProjectileResourceNames + 0x3c), 0x0,
    0x0, 0x192a39, (int)((char *)&g_BattleFxEmeraldShootProjectileResourceNames + 0x48), (int)((char *)&g_BattleFxEmeraldShootProjectileResourceNames + 0x54), 0x0, 0x0,
    0x9600b1, (int)((char *)&g_BattleFxEmeraldShootProjectileResourceNames + 0x60), 0x0, 0x0, 0x0, 0x4b00b1,
    (int)((char *)&g_BattleFxEmeraldShootProjectileResourceNames + 0x6c), 0x0, 0x0, 0x0, 0x4b00b1, (int)((char *)&g_BattleFxEmeraldShootProjectileResourceNames + 0x78),
    0x0, 0x0, 0x0, 0x0
};
// GLOBAL: FF7 0x007d85c0
int g_WeaponFxUnitDirectionNegativeZ[3] = {0, 0, -4096};
// GLOBAL: FF7 0x007d85d0
int g_WeaponFxUnitDirectionPositiveY[3] = {0, 0x1000, 0};
// GLOBAL: FF7 0x007d85e0
BattleFxSpriteRequest g_WeaponFxAlternateSpriteDrawRequest = {0, 0, -128, -128, -128, 0x2e, 0, 0};
// GLOBAL: FF7 0x007d85f0
BattleFxSpriteRequest g_WeaponFxResource3BillboardSpriteGroup = {0, 0, -128, -128, -128, 0x2e, 0, 0};
// GLOBAL: FF7 0x007d8600
BattleFxSpriteRequest g_WeaponFxResource2SpriteDrawRequest = {0, 0, -128, -128, -128, 0x2e, 0, 0};
// GLOBAL: FF7 0x007d8610
BattleFxSpriteRequest g_WeaponFxEmeraldShootSpriteDrawRequest = {0, 0, -128, -128, -128, 0x2e, 0, 0};
unsigned char g_BattleFxEmeraldShootProjectileResourceNames[132] =
    "AWA_1.TIM\0\0\0"
    "AWA_1.SPT\0\0\0"
    "EXP_1.TIM\0\0\0"
    "EXP_1.SPT\0\0\0"
    "LINE_1.TIM\0\0"
    "LINE_1.SPT\0\0"
    "TAMA_1.TIM\0\0"
    "TAMA_1.SPT\0\0"
    "FLASH_1.rsd\0"
    "RING_1.rsd\0\0"
    "W_BALL.rsd";
// GLOBAL: FF7 0x009f2958
struct BattleMagicDethperFragmentBuffer g_WeaponFxEmeraldShootTrailState = {0};
// GLOBAL: FF7 0x009f2968
unsigned char g_WeaponFxEmeraldShootTrailStateExtension[480] = {0};
unsigned char g_bssPad_009f2b48[16] = {0};
// GLOBAL: FF7 0x009f2b58
struct Int64Pair g_WeaponFxEmeraldShootSourceBonePos = {0};
// GLOBAL: FF7 0x009f2b60
unsigned char g_WeaponFxEmeraldShootTrailPointBuffer[608] = {0};
unsigned char g_bssPad_009f2dc0[32] = {0};
// GLOBAL: FF7 0x009f2de0
struct Int64Pair g_WeaponFxEmeraldShootTargetCentroid = {0};
// GLOBAL: FF7 0x009f2de8
int g_WeaponFxEmeraldShootTargetActorMask = 0;
// GLOBAL: FF7 0x009f2dec
int g_WeaponFxEmeraldShootSourceActor = 0;
// GLOBAL: FF7 0x009f2df0
short g_WeaponFxEmeraldShootEmitterPos[3] = {0};
// GLOBAL: FF7 0x009f2df8
VECTOR g_WeaponFxEmeraldShootTargetDirection = {0};
// GLOBAL: FF7 0x009f2e08
GfxTableCursor *g_WeaponFxEmeraldShootResourceHandle = 0;
unsigned char g_bssPad_009f2e0c[4] = {0};
// clang-format on


/*
 * Loads weapon/weapon3 resources into the battle scene context if not already loaded.
 */
// FUNCTION: FF7 0x00448a60
void __cdecl BattleFx_WeaponLoadResEmeraldShoot(void)
{
    GfxModelTableBank *sceneContext;
    GfxResourceRequest requestBuffer;
    int resultId;
    char pathSize[200];
    GfxResourceRequest *pReq;
    int defsPtr;

    if (g_pBattle3dScene != 0 && g_WeaponFxEmeraldShootResourceHandle == 0) {
        resultId = 7;
        defsPtr = (int)&g_WeaponFxEmeraldShootResourceRecords;
        sceneContext = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pReq = &requestBuffer;
        Gfx_InitResourceRequest(pReq);
        pReq->groupTag = 0;
        // STRING: FF7 0x007d86a4
        pReq->resourcePath = BattleFx_BuildResourcePath("weapon/weapon3", pathSize);
        Gfx_LoadModelTableIfAbsent(resultId, defsPtr, pReq, &g_WeaponFxEmeraldShootResourceHandle, sceneContext);
    }
}

/*
 * Entry point: loads Weapon3 resources and begins Weapon3 timeline.
 */
// FUNCTION: FF7 0x00448af0
void __cdecl BattleFx_WeaponEnterEmeraldShoot(int targetMask, int actorIdx)
{
    BattleFx_WeaponLoadResEmeraldShoot();
    BattleFx_WeaponBeginEmeraldShoot(targetMask, actorIdx);
}

/*
 * Begins Weapon3 effect by allocating the draw list buffer, spawning timeline, and saving target and source actor.
 */
// FUNCTION: FF7 0x00448b0a
void __cdecl BattleFx_WeaponBeginEmeraldShoot(int targetMask, int actorIdx)
{
    *(int *)&g_BattleFxDrawListBufferTaskSlotPtr = (int)BATTLE_NEW_TASK_SLOT(BattleFx_SelectDrawListBuffer);
    Battle_AllocateResourceSlot(BattleFx_EmeraldShootSpawnTimeline);
    g_WeaponFxEmeraldShootTargetActorMask = targetMask;
    g_WeaponFxEmeraldShootSourceActor = actorIdx;
}

/*
 * Spawns the Weapon3 main timeline task and calculates the centroid of the target actor mask.
 */
// FUNCTION: FF7 0x00448b4b
void __cdecl BattleFx_EmeraldShootSpawnTimeline(void)
{
    BattleFxYuffieWeaponTimeline2 *task;

    task = (BattleFxYuffieWeaponTimeline2 *)BATTLE_CURRENT_TASK_SLOT;
    task->ownerSlot = BATTLE_TASK_FREE;
    task = (BattleFxYuffieWeaponTimeline2 *)BATTLE_NEW_TASK_SLOT(BattleFx_EmeraldShootTimelineTask);
    task->subCounter = 0;
    Battle_GetActorMaskCentroid(g_WeaponFxEmeraldShootTargetActorMask, ((short *)&g_WeaponFxEmeraldShootTargetCentroid));
}

/*
 * Main timeline task driving Weapon3 projectile effects, rotating models, stretching sprites, and trail bursts.
 */
// FUNCTION: FF7 0x00448ba6
void __cdecl BattleFx_EmeraldShootTimelineTask(void)
{
    int theta;
    int sizeBase;
    int burstIndex;
    short fadeValue;
    int mag;
    int spawnCount;
    BattleFxEmeraldShootParticle *particle;
    BattleFxEmeraldShootTask *task;
    BattleFxWeaponEmeraldBeamSprite *pSprite;

    if (g_BattleIsPaused != 0) {
        return;
    }
    task = (BattleFxEmeraldShootTask *)BATTLE_CURRENT_TASK_SLOT;
    if (task->timer < 0x40) {
        Battle_GetBoneWorldPos(g_WeaponFxEmeraldShootSourceActor, g_BattleActorRecords[g_WeaponFxEmeraldShootSourceActor].bone34, ((short *)&g_WeaponFxEmeraldShootSourceBonePos));
        ((short *)&g_WeaponFxEmeraldShootSourceBonePos)[3] = g_BattleActorRecords[g_WeaponFxEmeraldShootSourceActor].radius + 0x100;
        g_WeaponFxEmeraldShootTargetDirection.vx = ((short *)&g_WeaponFxEmeraldShootTargetCentroid)[0] - ((short *)&g_WeaponFxEmeraldShootSourceBonePos)[0];
        g_WeaponFxEmeraldShootTargetDirection.vy = ((short *)&g_WeaponFxEmeraldShootTargetCentroid)[1] - ((short *)&g_WeaponFxEmeraldShootSourceBonePos)[1];
        g_WeaponFxEmeraldShootTargetDirection.vz = ((short *)&g_WeaponFxEmeraldShootTargetCentroid)[2] - ((short *)&g_WeaponFxEmeraldShootSourceBonePos)[2];
        NormalizeVectorFixed(&g_WeaponFxEmeraldShootTargetDirection, &g_WeaponFxEmeraldShootTargetDirection);
    }
    if (task->timer >= 6 && task->timer <= 0x28) {
        if (task->timer < 0x14) {
            spawnCount = 3;
        } else if (task->timer < 0x1E) {
            spawnCount = 2;
        } else {
            spawnCount = 1;
        }
        sizeBase = 0x4CC;
        for (burstIndex = 0; burstIndex < spawnCount; burstIndex++) {
            pSprite = (BattleFxWeaponEmeraldBeamSprite *)BATTLE_NEW_TASK_SLOT(BattleFx_EmeraldShootRadialFadingSpriteTask);
            theta = (rand() % 4096);
            mag = ((short *)&g_WeaponFxEmeraldShootSourceBonePos)[3] / 3 * 2 + rand() % 0x64;
            pSprite->posX = ((short *)&g_WeaponFxEmeraldShootSourceBonePos)[0] + (CosFixed(theta) * mag >> 0xC);
            pSprite->posY = ((short *)&g_WeaponFxEmeraldShootSourceBonePos)[1] + (0xC8 - rand() % 0x190);
            pSprite->posZ = ((short *)&g_WeaponFxEmeraldShootSourceBonePos)[2] + (SinFixed(theta) * mag >> 0xC);
            mag = rand() % 0x5A + 0x5A;
            pSprite->velocityX = CosFixed(theta) * mag >> 0xC;
            pSprite->velocityY = -(rand() % 0x14 + 0x1A);
            pSprite->velocityZ = SinFixed(theta) * mag >> 0xC;
            theta = rand() % 0x600 + 0xD00;
            pSprite->scale = sizeBase * theta >> 0xC;
        }
    }
    if (task->timer == 0xE) {
        sizeBase = 0x14CC;
        g_WeaponFxEmeraldShootEmitterPos[0] = ((short *)&g_WeaponFxEmeraldShootSourceBonePos)[0] + (g_WeaponFxEmeraldShootTargetDirection.vx * 0xF0 >> 0xC);
        g_WeaponFxEmeraldShootEmitterPos[1] = ((short *)&g_WeaponFxEmeraldShootSourceBonePos)[1] + (g_WeaponFxEmeraldShootTargetDirection.vy * 0xF0 >> 0xC);
        g_WeaponFxEmeraldShootEmitterPos[2] = ((short *)&g_WeaponFxEmeraldShootSourceBonePos)[2] + (g_WeaponFxEmeraldShootTargetDirection.vz * 0xF0 >> 0xC);
        for (burstIndex = 0; burstIndex < 3; burstIndex++) {
            particle = (BattleFxEmeraldShootParticle *)BATTLE_NEW_TASK_SLOT(BattleFx_EmeraldShootFadingSpinningModelTask);
            theta = ((rand() % 1024) + 0x200) * burstIndex;
            mag = 0x118 - rand() % 0x230;
            mag += burstIndex * 0x46;
            particle->posX = g_WeaponFxEmeraldShootEmitterPos[0] + (CosFixed(theta) * mag >> 0xC);
            particle->posY = g_WeaponFxEmeraldShootEmitterPos[1] + -(rand() % 0xA0 + 0x50);
            particle->posZ = g_WeaponFxEmeraldShootEmitterPos[2] + (SinFixed(theta) * mag >> 0xC);
            theta = rand() % 0x900 + 0xB00;
            particle->angle = (rand() % 4096);
            particle->angularSpeed = (rand() % 128) + 0x40;
            particle->halfAngle = particle->angle >> 1;
            particle->angularSpeedZ = rand() % 0x60 + 0x20;
            particle->angleOffset = 0;
            particle->rate = sizeBase * theta >> 0xC;
            particle->kind = rand() % 6;
            particle->lifetime = rand() % 0xE + 0x1C;
            particle->rate = particle->rate / particle->lifetime;
        }
    }
    if (task->timer == 0x14) {
        particle = (BattleFxEmeraldShootParticle *)BATTLE_NEW_TASK_SLOT(BattleFx_EmeraldShootStretchingSpriteEmitterTask);
        particle->kind = 0;
    }
    if (task->timer >= 0x40 && task->timer <= 0x5E && (task->timer & 1) == 0) {
        particle = (BattleFxEmeraldShootParticle *)BATTLE_NEW_TASK_SLOT(BattleFx_EmeraldShootImpactTrailSpawnTask);
        *(Int64Pair *)((char *)particle + 4) = *(Int64Pair *)&g_WeaponFxEmeraldShootTargetCentroid;
        particle->posX += (short)(0x898 - rand() % 0x1130);
        particle->posY = -(rand() % 0x2BC + 0x96);
        particle->posZ += (short)(0x44C - rand() % 0x898);
        particle->kind = task->animFrame;
        (task->animFrame)++;
        if (task->animFrame >= 8) {
            task->animFrame = 0;
        }
    }
    if (task->timer == 0x3C) {
        particle = (BattleFxEmeraldShootParticle *)BATTLE_NEW_TASK_SLOT(BattleFx_EmeraldShootExpandingModelTask);
        *(Int64Pair *)((char *)particle + 4) = *(Int64Pair *)&g_WeaponFxEmeraldShootSourceBonePos;
        particle->rate = 0xAAA;
        particle->angleOffset = 0xAAA;
    }
    if (task->timer >= 0x42 && task->timer <= 0x6E) {
        g_BattleCameraTransform.posY = (task->timer & 1) * 0x50;
    }
    if (task->timer == 6) {
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_CH1, Battle_ComputeScreenPanFromPosition(((short *)&g_WeaponFxEmeraldShootSourceBonePos)), 0x1CF);
    }
    if (task->timer == 0x2C) {
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_CH1, Battle_ComputeScreenPanFromPosition(((short *)&g_WeaponFxEmeraldShootSourceBonePos)), 0x78);
    }
    if (task->timer == 0x14) {
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_CH2, Battle_ComputeScreenPanFromPosition(((short *)&g_WeaponFxEmeraldShootSourceBonePos)), 0x217);
    }
    if (task->timer < 0x14) {
        g_BattleFxTimelineState.channel2 = 0;
        g_BattleFxTimelineState.channel1 = g_BattleFxTimelineState.channel2;
        g_BattleFxTimelineState.red = g_BattleFxTimelineState.channel1;
        fadeValue = 0;
    } else if (task->timer >= 0x14 && task->timer < 0x3C) {
        fadeValue = (task->timer - 0x14) * 0x33;
    } else if (task->timer >= 0x7A) {
        fadeValue = 0x800 - (task->timer - 0x7A << 8);
    } else {
        fadeValue = 0x800;
    }
    if (task->timer == 0x78) {
        for (burstIndex = 0; burstIndex < 0xA; burstIndex++) {
            if ((g_WeaponFxEmeraldShootTargetActorMask & 1 << burstIndex) != 0) {
                Battle_PostActorHitOrHurtMessage(burstIndex);
            }
        }
    }
    if (task->timer >= 0x82) {
        g_BattleCameraTransform.posY = 0;
        fadeValue = 0;
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    g_BattleFxTimelineState.startValue = fadeValue;
    (task->timer)++;
}

/*
 * Displays an expanding 3D model billboard with scaling and alpha fade.
 */
// FUNCTION: FF7 0x0044939c
void __cdecl BattleFx_EmeraldShootExpandingModelTask(void)
{
    int padSlot[4]; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int padGap[7]; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxModelDraw *draw;
    int stackWords[8]; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxEmeraldShootExpandingRecord *task;

    task = (BattleFxEmeraldShootExpandingRecord *)BATTLE_CURRENT_TASK_SLOT;
    BattleFx_InstallBillboardMatrix(&task->posX, (short)task->expandLevel, -(task->expandLevel >> 4));
    draw = (BattleFxModelDraw *)Gfx_GetPsxScratchpadPtr(0);
    draw->modelGroup = Gfx_ModelTableGetRenderObject(4, g_WeaponFxEmeraldShootResourceHandle);
    draw->flags = 0x88;
    draw->uvScroll = 0;
    draw->fadeLevel = 0;
    draw->stateFlags = 0;
    draw->texturePageBits = 0;
    if (task->step >= 1) {
        draw->fadeLevel = (task->step - 1) * 0x333;
    }
    SetDrawColorRGB(0, 0, 0);
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitModelDraw(draw, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (task->step >= 6) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    task->expandLevel += task->expandStep;
    if (task->expandLevel > 0x7000) {
        task->expandLevel = 0x7000;
    }
    task->expandStep += (short)(task->expandStep >> 3);
    (task->step)++;
}

/*
 * Displays a fading and spinning 3D model with delayed start and rotational velocity.
 */
// FUNCTION: FF7 0x004494fa
void __cdecl BattleFx_EmeraldShootFadingSpinningModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[68];
        PsxMatrix rotMatrix;
        struct { unsigned char pad_to_20[0x20]; BattleFxEmeraldShootFadingSpinTask *fadeTask; };
        struct { unsigned char pad_to_28[0x28]; SVECTOR rotX; };
        struct { unsigned char pad_to_30[0x30]; BattleFxModelDraw *drawModel; };
        struct { unsigned char pad_to_34[0x34]; VECTOR scaleVec; };
    } frame;
#pragma pack(pop)

    frame.fadeTask = (BattleFxEmeraldShootFadingSpinTask *)BATTLE_CURRENT_TASK_SLOT;
    if (frame.fadeTask->timer > 0) {
        if (g_BattleIsPaused == 0) {
            (frame.fadeTask->timer)--;
        }
    } else {
        frame.rotX.vx = frame.fadeTask->angle;
        frame.rotX.vy = 0;
        frame.rotX.vz = frame.fadeTask->angleZ;
        RotMatrixXYZ(&frame.rotX, &frame.rotMatrix);
        frame.rotMatrix.t[0] = frame.fadeTask->posX;
        frame.rotMatrix.t[1] = frame.fadeTask->posY;
        frame.rotMatrix.t[2] = frame.fadeTask->posZ;
        frame.scaleVec.vz = frame.fadeTask->scale;
        frame.scaleVec.vy = frame.scaleVec.vz;
        frame.scaleVec.vx = frame.scaleVec.vy;
        ScaleMatrix(&frame.rotMatrix, &frame.scaleVec);
        MultiplyMatrix((void *)g_BattleCameraTransform.matrix, &frame.rotMatrix, &frame.rotMatrix);
        SetRotMatrix(&frame.rotMatrix);
        SetTransMatrix(&frame.rotMatrix);
        frame.drawModel = (BattleFxModelDraw *)Gfx_GetPsxScratchpadPtr(0);
        frame.drawModel->modelGroup = Gfx_ModelTableGetRenderObject(6, g_WeaponFxEmeraldShootResourceHandle);
        frame.drawModel->flags = 8;
        frame.drawModel->uvScroll = 0;
        frame.drawModel->fadeLevel = 0x80;
        frame.drawModel->stateFlags = 0x20;
        frame.drawModel->texturePageBits = 0;
        if (frame.fadeTask->frameIndex >= 0x24) {
            frame.drawModel->fadeLevel = 0x80 - (frame.fadeTask->frameIndex - 0x24 << 4);
        }
        g_BattleFxActiveDrawListPtr = BattleFx_SubmitModelDraw(frame.drawModel, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
        if (frame.fadeTask->frameIndex >= 0x2C) {
            frame.fadeTask->ownerSlot = BATTLE_TASK_FREE;
        }
        if (g_BattleIsPaused != 0) {
            return;
        }
        frame.fadeTask->angle += frame.fadeTask->angularSpeed;
        frame.fadeTask->angleZ += frame.fadeTask->angularSpeedZ;
        (frame.fadeTask->angularSpeed)--;
        if (frame.fadeTask->angularSpeed < 1) {
            frame.fadeTask->angularSpeed = 1;
        }
        (frame.fadeTask->angularSpeedZ)--;
        if (frame.fadeTask->angularSpeedZ < 1) {
            frame.fadeTask->angularSpeedZ = 1;
        }
        if (frame.fadeTask->frameIndex < frame.fadeTask->scaleGrowFrames) {
            frame.fadeTask->scale += frame.fadeTask->scaleStep;
        }
        (frame.fadeTask->frameIndex)++;
    }
}

/*
 * Displays a radial fading billboard sprite with physics integration and velocity damping.
 */
// FUNCTION: FF7 0x00449752
void __cdecl BattleFx_EmeraldShootRadialFadingSpriteTask(void)
{
    unsigned char brightness;
    BattleFxBeamFadeSprite *pSprite;

    pSprite = (BattleFxBeamFadeSprite *)BATTLE_CURRENT_TASK_SLOT;
    if (pSprite->frame >= 0xA) {
        brightness = 0x80 - (pSprite->frame - 0xA) * 0x15;
    } else {
        brightness = 0x80;
    }
    g_WeaponFxEmeraldShootSpriteDrawRequest.colorB = brightness;
    g_WeaponFxEmeraldShootSpriteDrawRequest.colorG = g_WeaponFxEmeraldShootSpriteDrawRequest.colorB;
    g_WeaponFxEmeraldShootSpriteDrawRequest.colorR = g_WeaponFxEmeraldShootSpriteDrawRequest.colorG;
    BattleFx_InstallBillboardMatrix(&pSprite->posA, (short)pSprite->angle, -(pSprite->angle >> 3));
    g_WeaponFxEmeraldShootSpriteDrawRequest.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(0, g_WeaponFxEmeraldShootResourceHandle);
    g_WeaponFxEmeraldShootSpriteDrawRequest.drawObj = Gfx_ModelTableGetSpriteObject(0, g_WeaponFxEmeraldShootResourceHandle);
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitSpriteDraw(&g_WeaponFxEmeraldShootSpriteDrawRequest, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (pSprite->frame >= 0x10) {
        pSprite->endFlag = 0xFFFF;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    pSprite->posA += pSprite->velA;
    pSprite->posB += pSprite->velB;
    pSprite->posC += pSprite->velC;
    pSprite->velA -= (short)(pSprite->velA >> 3);
    pSprite->velB += (short)(pSprite->velB >> 3);
    pSprite->velC -= (short)(pSprite->velC >> 3);
    (pSprite->frame)++;
}

/*
 * Spawns growing models and trail sprites along the path toward the target position.
 */
// FUNCTION: FF7 0x004498f1
void __cdecl BattleFx_EmeraldShootImpactTrailSpawnTask(void)
{
    int baseScale;
    int i;
    BattleFxWeaponImpactTrail *task;
    BattleFxEmeraldShootImpactTrail *pNewTask;
    int scaleRoll;
    BattleFxEmeraldShootImpactTrail *pTrail;

    if (g_BattleIsPaused != 0) {
        return;
    }
    task = (BattleFxWeaponImpactTrail *)BATTLE_CURRENT_TASK_SLOT;
    if (task->step == 0) {
        if ((task->slotIndex & 1) == 0) {
            pNewTask = (BattleFxEmeraldShootImpactTrail *)BATTLE_NEW_TASK_SLOT(BattleFx_EmeraldShootGrowingModelTask);
            *(Int64Pair *)((char *)pNewTask + 4) = *(Int64Pair *)&g_WeaponFxEmeraldShootSourceBonePos;
            pNewTask->expandStep = 0x61D;
            pNewTask->scale = 0x61D;
        }
        pTrail = (BattleFxEmeraldShootImpactTrail *)BATTLE_NEW_TASK_SLOT(BattleFx_EmeraldShootTrailSpriteTask);
        *(Int64Pair *)((char *)pTrail + 4) = *(Int64Pair *)&g_WeaponFxEmeraldShootSourceBonePos;
        pTrail->velocityX = (task->targetX - pTrail->posX) / 8;
        pTrail->velocityY = (task->targetY - pTrail->posY) / 8;
        pTrail->velocityZ = (task->targetZ - pTrail->posZ) / 8;
        pTrail->scale = 0xC80;
        pTrail->slotIndex = task->slotIndex;
        *(int *)((char *)&g_WeaponFxEmeraldShootTrailState.count + (pTrail->slotIndex << 6)) = 0;
        *(int *)((char *)&g_WeaponFxEmeraldShootTrailState.startIndex + (pTrail->slotIndex << 6)) = 0;
        *(int *)((char *)&(g_WeaponFxEmeraldShootTrailState.headIndex) + (pTrail->slotIndex << 6)) = 0;
        *(int *)((char *)&g_WeaponFxEmeraldShootTrailState.limit + (pTrail->slotIndex << 6)) = 6;
        *(int *)((char *)&g_WeaponFxEmeraldShootTrailStateExtension + (pTrail->slotIndex << 6)) = 0xFFFFFFFF;
        *(Int64Pair *)(pTrail->slotIndex * 0x50 + (char *)&g_WeaponFxEmeraldShootTrailPointBuffer) = *(Int64Pair *)((char *)pTrail + 4);
        BattleFx_EmeraldShootPushTrailPoint((BattleFxEmeraldShootTrailState *)((pTrail->slotIndex << 6) + (char *)&(g_WeaponFxEmeraldShootTrailState.headIndex)), pTrail->slotIndex * 0x50 + (int)&g_WeaponFxEmeraldShootTrailPointBuffer);
    }
    if (task->step == 0xB) {
        baseScale = 0x30C0;
        for (i = 0; i < 3; i++) {
            pNewTask = (BattleFxEmeraldShootImpactTrail *)BATTLE_NEW_TASK_SLOT(BattleFx_EmeraldShootDelayedFrameSpriteTask);
            *(Int64Pair *)((char *)pNewTask + 4) = *(Int64Pair *)((char *)task + 4);
            pNewTask->posX += (short)(0x258 - rand() % 0x4B0);
            pNewTask->posY += (short)(0x258 - rand() % 0x4B0);
            pNewTask->posZ += (short)(0x258 - rand() % 0x4B0);
            scaleRoll = rand() % 0xA00 + 0xC00;
            pNewTask->scale = baseScale * scaleRoll >> 0xC;
            pNewTask->variant = rand() % 6;
        }
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    if (task->step >= 0x1E) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    (task->step)++;
}

/*
 * Appends a new position entry into the circular trail point buffer.
 */
// FUNCTION: FF7 0x00449c0a
void __cdecl BattleFx_EmeraldShootPushTrailPoint(BattleFxEmeraldShootTrailState *trailStatePtr, int pointBufferPtr)
{
    int tail;
    int head;
    int count;
    int limit;

    trailStatePtr->prevPrevIndex = trailStatePtr->prevIndex;
    trailStatePtr->prevIndex = trailStatePtr->headIndex;
    head = trailStatePtr->prevIndex;
    tail = trailStatePtr->startIndex;
    count = trailStatePtr->count;
    limit = trailStatePtr->limit;
    *(int *)((char *)trailStatePtr + head * 4 + 0x18) = pointBufferPtr;
    head++;
    count++;
    if (head >= limit) {
        head = 0;
    }
    if (count >= limit) {
        count = limit;
    }
    if (head == tail) {
        tail++;
        count--;
        if (tail >= limit) {
            tail = 0;
        }
    }
    trailStatePtr->headIndex = head;
    trailStatePtr->startIndex = tail;
    trailStatePtr->count = count;
    trailStatePtr->limit = limit;
}

/*
 * Displays an animated sprite frame with initial delay and billboard orientation.
 */
// FUNCTION: FF7 0x00449cd7
void __cdecl BattleFx_EmeraldShootDelayedFrameSpriteTask(void)
{
    BattleFxEmeraldShootDelayedFrameSprite *task;

    task = (BattleFxEmeraldShootDelayedFrameSprite *)BATTLE_CURRENT_TASK_SLOT;
    if (task->delay > 0) {
        if (g_BattleIsPaused == 0) {
            (task->delay)--;
        }
    } else {
        g_WeaponFxAlternateSpriteDrawRequest.frame = task->frameCount;
        BattleFx_InstallBillboardMatrix(&task->posX, (short)task->scale, -(task->scale >> 3));
        g_WeaponFxAlternateSpriteDrawRequest.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(1, g_WeaponFxEmeraldShootResourceHandle);
        g_WeaponFxAlternateSpriteDrawRequest.drawObj = Gfx_ModelTableGetSpriteObject(1, g_WeaponFxEmeraldShootResourceHandle);
        g_BattleFxActiveDrawListPtr = BattleFx_SubmitSpriteDraw(&g_WeaponFxAlternateSpriteDrawRequest, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
        if (task->frameCount >= 0xF) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
        if (g_BattleIsPaused != 0) {
            return;
        }
        (task->frameCount)++;
    }
}

/*
 * Renders billboard sprites along the trail point history buffer.
 */
// FUNCTION: FF7 0x00449dd3
void __cdecl BattleFx_EmeraldShootTrailSpriteTask(void)
{
    int padSlot[5]; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int cursor;
    int growth;
    BattleFxEmeraldShootTrailState *trailStatus;
    int pointBase;
    int i;
    BattleFxEmeraldShootTrailSprite *task;

    task = (BattleFxEmeraldShootTrailSprite *)BATTLE_CURRENT_TASK_SLOT;
    trailStatus = (BattleFxEmeraldShootTrailState *)((task->trailIndex << 6) + (char *)&(g_WeaponFxEmeraldShootTrailState.headIndex));
    pointBase = task->trailIndex * 0x50 + (int)&g_WeaponFxEmeraldShootTrailPointBuffer;
    growth = 0xC80;
    g_WeaponFxResource3BillboardSpriteGroup.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(3, g_WeaponFxEmeraldShootResourceHandle);
    g_WeaponFxResource3BillboardSpriteGroup.drawObj = Gfx_ModelTableGetSpriteObject(3, g_WeaponFxEmeraldShootResourceHandle);
    cursor = trailStatus->startIndex;
    for (i = 0; i < trailStatus->count; cursor++, i++) {
        if (cursor >= trailStatus->limit) {
            cursor = 0;
        }
        BattleFx_InstallBillboardMatrix((void *)(*(int *)((char *)trailStatus + cursor * 4 + 0x18)), (short)growth, -(growth >> 4));
        g_BattleFxActiveDrawListPtr = BattleFx_SubmitSpriteDraw(&g_WeaponFxResource3BillboardSpriteGroup, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
        growth += growth / 3;
    }
    if (task->timer >= 0xB) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    *(Int64Pair *)((char *)pointBase + trailStatus->headIndex * 8) = *(Int64Pair *)((char *)task + 4);
    BattleFx_EmeraldShootPushTrailPoint(trailStatus, pointBase + trailStatus->headIndex * 8);
    if (task->timer < 8) {
        task->posX += task->velocityX;
        task->posY += task->velocityY;
        task->posZ += task->velocityZ;
    }
    (task->timer)++;
}

/*
 * Displays a growing oriented model aligned to the trajectory axis-angle matrix.
 */
// FUNCTION: FF7 0x00449f98
void __cdecl BattleFx_EmeraldShootGrowingModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[84];
        PsxMatrix rotMatrix;
        struct { unsigned char pad_to_20[0x20]; BattleFxEmeraldShootGrowingRecord *growRecord; };
        struct { unsigned char pad_to_24[0x24]; VECTOR axis; };
        struct { unsigned char pad_to_34[0x34]; int angle; };
        struct { unsigned char pad_to_40[0x40]; BattleFxModelDraw *drawModel; };
        struct { unsigned char pad_to_44[0x44]; VECTOR scaleVec; };
    } frame;
#pragma pack(pop)

    frame.growRecord = (BattleFxEmeraldShootGrowingRecord *)BATTLE_CURRENT_TASK_SLOT;
    frame.angle = ComputeAngleBetweenVectors((VECTOR *)&g_WeaponFxUnitDirectionNegativeZ, &g_WeaponFxEmeraldShootTargetDirection, &frame.axis);
    Gte_BuildAxisAngleMatrix(frame.angle, &frame.rotMatrix, &frame.axis);
    frame.rotMatrix.t[0] = frame.growRecord->posA;
    frame.rotMatrix.t[1] = frame.growRecord->posB;
    frame.rotMatrix.t[2] = frame.growRecord->posC;
    frame.scaleVec.vz = frame.growRecord->expandLevel;
    frame.scaleVec.vy = frame.scaleVec.vz;
    frame.scaleVec.vx = frame.scaleVec.vy;
    ScaleMatrix(&frame.rotMatrix, &frame.scaleVec);
    MultiplyMatrix((void *)g_BattleCameraTransform.matrix, &frame.rotMatrix, &frame.rotMatrix);
    SetRotMatrix(&frame.rotMatrix);
    SetTransMatrix(&frame.rotMatrix);
    frame.drawModel = (BattleFxModelDraw *)Gfx_GetPsxScratchpadPtr(0);
    frame.drawModel->modelGroup = Gfx_ModelTableGetRenderObject(5, g_WeaponFxEmeraldShootResourceHandle);
    frame.drawModel->flags = 0xA8;
    frame.drawModel->uvScroll = 0;
    frame.drawModel->fadeLevel = 0;
    frame.drawModel->stateFlags = 0;
    frame.drawModel->texturePageBits = 0;
    if (frame.growRecord->step >= 4) {
        frame.drawModel->fadeLevel = (frame.growRecord->step - 4) * 0x2AA;
    }
    SetDrawColorRGB(0, 0, 0);
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitModelDraw(frame.drawModel, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (frame.growRecord->step >= 0xA) {
        frame.growRecord->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.growRecord->expandLevel += frame.growRecord->expandStep;
    frame.growRecord->expandStep += (short)(frame.growRecord->expandStep >> 4);
    (frame.growRecord->step)++;
}

/*
 * Emits multiple stretching sprites in randomized 3D directions.
 */
// FUNCTION: FF7 0x0044a159
void __cdecl BattleFx_EmeraldShootStretchingSpriteEmitterTask(void)
{
    BattleFxStretchSprite *sprite;
    int stretchAmount;
    int i;
    BattleTaskSlot *task;
    int scaleRoll;
    int spawnCnt;

    if (g_BattleIsPaused != 0) {
        return;
    }
    task = BATTLE_CURRENT_TASK_SLOT;
    if (task->frameCounter < 0xD) {
        spawnCnt = (task->frameCounter >> 2) + 1;
    } else {
        spawnCnt = (0x1A - task->frameCounter >> 2) + 1;
    }
    if (spawnCnt != 0) {
        for (i = 0; i < spawnCnt; i++) {
            sprite = (BattleFxStretchSprite *)BATTLE_NEW_TASK_SLOT(BattleFx_EmeraldShootStretchingSpriteTask);
            sprite->offsetX = (rand() % 4096) - 0x800;
            sprite->offsetY = (rand() % 4096) - 0x800;
            sprite->offsetZ = (rand() % 2048);
            NormalizeVectorFixed(&sprite->offsetX, &sprite->offsetX);
            stretchAmount = 0x17C0;
            scaleRoll = rand() % 0xA00 + 0xA00;
            stretchAmount = stretchAmount * scaleRoll >> 0xC;
            sprite->angle = 0;
            sprite->stretch = 0;
            sprite->stretchRate = stretchAmount / 5;
            sprite->angleStep = stretchAmount / 5;
        }
    }
    if (task->frameCounter >= 0x1A) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    (task->frameCounter)++;
}

/*
 * Animates a stretching billboard sprite with non-uniform X/Z and Y scaling along an axis angle.
 */
// FUNCTION: FF7 0x0044a2f5
void __cdecl BattleFx_EmeraldShootStretchingSpriteTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[92];
        int rotMatrix;
        struct { unsigned char pad_to_12[0x12]; int bonePosX; };
        struct { unsigned char pad_to_16[0x16]; int bonePosY; };
        struct { unsigned char pad_to_1A[0x1A]; int bonePosZ; };
        struct { unsigned char pad_to_20[0x20]; int dirVec; };
        struct {
            unsigned char pad[36];
            int value;
        } at56;
        struct {
            unsigned char pad[40];
            int value;
        } at52;
        struct {
            unsigned char pad[44];
            int value;
        } at48;
        struct { unsigned char pad_to_30[0x30]; int axisAngle; };
        struct { unsigned char pad_to_34[0x34]; BattleFxEmeraldShootStretchRecord *stretchRecord; };
        struct { unsigned char pad_to_38[0x38]; VECTOR axis; };
        struct { unsigned char pad_to_48[0x48]; VECTOR scaleVec; };
    } frame;
#pragma pack(pop)

    frame.stretchRecord = (BattleFxEmeraldShootStretchRecord *)BATTLE_CURRENT_TASK_SLOT;
    *(BattleVec4i *)(&frame.dirVec) = *(BattleVec4i *)((int)frame.stretchRecord + 4);
    frame.axisAngle = ComputeAngleBetweenVectors((VECTOR *)&g_WeaponFxUnitDirectionPositiveY, (VECTOR *)&frame.dirVec, &frame.axis);
    Gte_BuildAxisAngleMatrix(frame.axisAngle, (PsxMatrix *)&frame.rotMatrix, &frame.axis);
    frame.bonePosX = ((short *)&g_WeaponFxEmeraldShootSourceBonePos)[0];
    frame.bonePosY = ((short *)&g_WeaponFxEmeraldShootSourceBonePos)[1];
    frame.bonePosZ = ((short *)&g_WeaponFxEmeraldShootSourceBonePos)[2];
    frame.scaleVec.vz = frame.stretchRecord->stretch;
    frame.scaleVec.vx = frame.scaleVec.vz;
    frame.scaleVec.vy = frame.stretchRecord->angle;
    ScaleMatrix(&frame.rotMatrix, &frame.scaleVec);
    MultiplyMatrix((void *)g_BattleCameraTransform.matrix, &frame.rotMatrix, &frame.rotMatrix);
    SetRotMatrix(&frame.rotMatrix);
    SetTransMatrix(&frame.rotMatrix);
    g_WeaponFxResource2SpriteDrawRequest.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(2, g_WeaponFxEmeraldShootResourceHandle);
    g_WeaponFxResource2SpriteDrawRequest.drawObj = Gfx_ModelTableGetSpriteObject(2, g_WeaponFxEmeraldShootResourceHandle);
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitSpriteDraw(&g_WeaponFxResource2SpriteDrawRequest, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (frame.stretchRecord->timer >= 0xA) {
        frame.stretchRecord->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.stretchRecord->timer < 5) {
        frame.stretchRecord->stretch += frame.stretchRecord->stretchRate;
    } else {
        frame.stretchRecord->stretch -= frame.stretchRecord->stretchRate;
        frame.stretchRecord->stretchRate += (short)(frame.stretchRecord->stretchRate >> 5);
        if (frame.stretchRecord->stretch < 0) {
            frame.stretchRecord->stretch = 0;
        }
    }
    frame.stretchRecord->angle += frame.stretchRecord->angleStep;
    frame.stretchRecord->angleStep += (short)(frame.stretchRecord->angleStep >> 5);
    if (frame.stretchRecord->angle > 0x7000) {
        frame.stretchRecord->angle = 0x7000;
    }
    (frame.stretchRecord->timer)++;
}
} /* extern "C" */
