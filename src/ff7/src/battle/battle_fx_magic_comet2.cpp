// src/ff7/src/battle/battle_fx_magic_comet2.cpp
/* Magic Effect [46]: Comet2 (Japanese: コメテオ Cometeo, dev tag: "MAGIC/COMETEO" / Cometeo) */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle visual effect for the Cometeo magic: a meteor sequence falls across the targets
//   with camera shake, descending meteors, sparks, explosions, bouncing debris and impact sprites.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include <stdlib.h>
#include <string.h>

#include "ff7/api/battle.h"
#include "ff7/api/menu.h"
#include "ff7/battle_tasks.h"
#include "ff7/camera_script.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_magic.h"
#include "ff7/types/battle_3d.h"
#include "lib/api/graphics.h"
#include "lib/api/sound.h"
#include "lib/codegen.h"
#include "lib/enums/sound.h"
#include "lib/types/graphics.h"
#include "lib/types/sound.h"
extern "C" {

#pragma intrinsic(abs)
#pragma intrinsic(memset)
#pragma intrinsic(memcpy)

struct BattleFxModelDraw;

// clang-format off
// GLOBAL: FF7 0x008edbb8
struct SoundGroup g_BattleMagicComet2_MeteorDelayTable = {{0x94, 0x19c, 0, 0}};
// GLOBAL: FF7 0x008edbc8
struct SoundGroup *g_BattleMagicComet2_MeteorDelayDef[2] = {&g_BattleMagicComet2_MeteorDelayTable, 0};
// GLOBAL: FF7 0x008edbd0
unsigned int g_BattleMagicComet2_ResourceRequests[45] = {0x006400b4, (unsigned int)&g_BattleMagicComet2_ModelNames1, 0x00000000, 0x00000000, 0x00000000, 0x006400b1, (unsigned int)&g_BattleMagicComet2_ModelNames1[0xc], 0x00000000, 0x00000000, 0x00000000, 0x006400b1, (unsigned int)&g_BattleMagicComet2_ModelNames2, 0x00000000, 0x00000000, 0x00000000, 0x006400b1, (unsigned int)&g_BattleMagicComet2_ModelNames2[0xc], 0x00000000, 0x00000000, 0x00000000, 0x006400b4, (unsigned int)&g_BattleMagicComet2_ModelNames3, 0x00000000, 0x00000000, 0x00000000, 0x006400b4, (unsigned int)&g_BattleMagicComet2_ModelNames3[0xc], 0x00000000, 0x00000000, 0x00000000, 0x006400b1, (unsigned int)&g_BattleMagicComet2_ModelNames4, 0x00000000, 0x00000000, 0x00000000, 0x006400b1, (unsigned int)&g_BattleMagicComet2_ModelNames4[0xc], 0x00000000, 0x00000000, 0x00000000, 0x00012a39, (unsigned int)&g_BattleMagicComet2_TextureNames, (unsigned int)&g_BattleMagicComet2_TextureNames[0xc], 0x00000000, 0x00000000};
// GLOBAL: FF7 0x008edc88
BattleFxModelDrawHead g_BattleMagicComet2_ModelReq1 = {0, 0x80, 0, 0, 0, 0};
// GLOBAL: FF7 0x008edc98
int g_BattleMagicComet2_ModelReq2[2] = {0, 0x88};
// The tail of the ModelReq2 request: uvScroll, fadeLevel, stateFlags and texturePageBits.
// GLOBAL: FF7 0x008edca0
short g_BattleMagicComet2_ModelReq3[4] = {0, 0, 0x20, 0};
// GLOBAL: FF7 0x008edca8
int g_BattleMagicComet2_ModelReq4[4] = {0xa00, 0xa00, 0xa00, 0};
// GLOBAL: FF7 0x008edcb8
PsxMatrix g_BattleMagicComet2_Matrix1 = {{{0x1000, 0, 0}, {0, 0x1000, 0}, {0, 0, 0x1000}}, {0, 0, 0}};
// GLOBAL: FF7 0x008edcd8
BattleFxModelDrawHead g_BattleMagicComet2_ModelReq5 = {0, 0x88, 0, 0, 0x40, 0};
// GLOBAL: FF7 0x008edce8
BattleFxModelDrawHead g_BattleMagicComet2_ModelReq6 = {0, 0x88, 0, 0, 0x20, 0};
// GLOBAL: FF7 0x008edcf8
PsxMatrix g_BattleMagicComet2_Matrix2 = {{{0x1000, 0, 0}, {0, 0x1000, 0}, {0, 0, 0x1000}}, {0, 0, 0}};
// GLOBAL: FF7 0x008edd18
BattleFxModelDrawHead g_BattleMagicComet2_ModelReq7 = {0, 0x80, 0, 0, 0x20, 0};
// GLOBAL: FF7 0x008edd28
PsxMatrix g_BattleMagicComet2_Matrix3 = {{{0x500, 0, 0}, {0, 0x500, 0}, {0, 0, 0x500}}, {0, 0, 0}};
// GLOBAL: FF7 0x008edd48
BattleFxModelDrawHead g_BattleMagicComet2_ModelReq8 = {0, 0x88, 0, 0, 0x40, 0};
// GLOBAL: FF7 0x008edd58
struct BattleFxSpriteRequest g_BattleMagicComet2_SpriteDrawReq = {0, 0, -128, -128, -128, 0x2e, 0, 0};
// GLOBAL: FF7 0x008edd68
unsigned char g_BattleMagicComet2_ModelNames1[24] =
    "COMET_1.RSD\0"
    "COMET_2.RSD";
// GLOBAL: FF7 0x008edd80
unsigned char g_BattleMagicComet2_ModelNames2[24] =
    "EXP_1.RSD\0\0\0"
    "SONIC_1.RSD";
// GLOBAL: FF7 0x008edd98
unsigned char g_BattleMagicComet2_ModelNames3[24] =
    "STONE_1.RSD\0"
    "STONE_2.RSD";
// GLOBAL: FF7 0x008eddb0
unsigned char g_BattleMagicComet2_ModelNames4[24] =
    "STONE_1.RSD\0"
    "STONE_2.RSD";
// GLOBAL: FF7 0x008eddc8
unsigned char g_BattleMagicComet2_TextureNames[23] =
    "BOMB_A.TIM\0\0"
    "BOMB_A.SPT";
// GLOBAL: FF7 0x00bd5b10
int g_BattleMagicComet2_DrawCursor = 0;
// GLOBAL: FF7 0x00bd5b14
int g_BattleMagicComet2_TargetMask = 0;
// GLOBAL: FF7 0x00bd5b18
int g_BattleMagicComet2_CasterActorIdx = 0;
unsigned char g_bssPad_00bd5b1c[4] = {0};
// GLOBAL: FF7 0x00bd5b20
short g_BattleMagicComet2_CenterXY[2] = {0};
// GLOBAL: FF7 0x00bd5b24
short g_BattleMagicComet2_CenterZ = 0;
// GLOBAL: FF7 0x00bd5b28
struct GfxTableCursor *g_BattleMagicComet2_ResourceTable = 0;
unsigned char g_bssPad_00bd5b2c[4] = {0};
// GLOBAL: FF7 0x00bd5b30
short g_BattleMagicComet2_SpinAngleXY[2] = {0};
// GLOBAL: FF7 0x00bd5b34
short g_BattleMagicComet2_SpinAngleZ = 0;
// clang-format on


/*
 * Loads resources for the Cometeo magic effect when not already loaded.
 */
// FUNCTION: FF7 0x005a4220
void __cdecl BattleFx_MagicLoadResComet2(void)
{
    GfxModelTableBank *parentHandle;
    GfxResourceRequest requestRecord;
    int kind;
    char pathBuffer[200];
    GfxResourceRequest *pReq;
    int requestTable;

    if (g_pBattle3dScene != 0 && g_BattleMagicComet2_ResourceTable == 0) {
        kind = 9;
        requestTable = (int)&g_BattleMagicComet2_ResourceRequests;
        parentHandle = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pReq = &requestRecord;
        Gfx_InitResourceRequest(pReq);
        pReq->groupTag = 0;
        // STRING: FF7 0x008edde0
        pReq->resourcePath = BattleFx_BuildResourcePath("MAGIC/COMETEO", pathBuffer);
        Gfx_LoadModelTableIfAbsent(kind, requestTable, pReq, &g_BattleMagicComet2_ResourceTable, parentHandle);
        SfxRegisterSoundGroups(g_BattleMagicComet2_MeteorDelayDef);
    }
}

/*
 * Entry point for Cometeo magic effect initializing execution.
 */
// FUNCTION: FF7 0x005a42c1
void __cdecl BattleFx_MagicEnterComet2(int targetMask, int actorIdx)
{
    g_BattleMagicComet2_DrawCursor = (int)&g_GfxPsxPacketScratch;
    BattleFx_MagicLoadResComet2();
    BattleFx_MagicBeginComet2(targetMask, actorIdx);
}

/*
 * Initializes target selection and allocates Cometeo timeline task.
 */
// FUNCTION: FF7 0x005a42e5
void __cdecl BattleFx_MagicBeginComet2(int targetMask, int actorIdx)
{
    struct BattleTaskSlot *effect;

    Battle_AllocateResourceSlot(BattleFx_MagicComet2WaitRampEndTask);
    g_BattleMagicComet2_TargetMask = targetMask;
    g_BattleMagicComet2_CasterActorIdx = actorIdx;
    Battle_GetActorMaskCentroid(targetMask, g_BattleMagicComet2_CenterXY);
    if (REG_ALLOC_HINT) {
    }
    effect = BATTLE_NEW_TASK_SLOT(BattleFx_MagicComet2TimelineTask);
    effect->ownerSlot = 0;
    Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_2, Battle_ComputePanForEntity(targetMask), 0);
}

/*
 * Timeline task managing meteor spawn sequence and camera shake for Cometeo.
 */
// FUNCTION: FF7 0x005a4359
void __cdecl BattleFx_MagicComet2TimelineTask(void)
{
    int localStep;
    BattleFxPoint3s *pRecord;
    BattleComet2Timeline *task;
    BattleFxPositionedSpriteSlot *child;
    int target;

    task = (BattleComet2Timeline *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    localStep = task->frameIndex;
    if (localStep == 0) {
        Battle_StartFixedPointRamp(0x800, 8);
        g_BattleFxTimelineState.channel2 = 0;
        g_BattleFxTimelineState.channel1 = g_BattleFxTimelineState.channel2;
        g_BattleFxTimelineState.red = g_BattleFxTimelineState.channel1;
    } else {
        localStep -= 8;
        if (localStep >= 0) {
            if (localStep - 20 >= 0 && (localStep - 0x14) % 8 == 0) {
                target = g_BattleHitTargetActors[task->sequenceIndex];
                if (target != 0xFF) {
                    pRecord = (BattleFxPoint3s *)Battle_AllocSceneMessage(0xA);
                    pRecord->targetIndex = target;
                    pRecord->sequenceIndex = task->sequenceIndex;
                    (task->sequenceIndex)++;
                }
            }
            if (localStep < 60) {
                if (localStep % 10 == 0) {
                    child = (BattleFxPositionedSpriteSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicComet2MeteorSequenceTask);
                    child->posX = g_BattleMagicComet2_CenterXY[0] + rand() % 6000 + -3000;
                    child->posY = 0;
                    child->posZ = g_BattleMagicComet2_CenterZ + rand() % 6000 + -3000;
                }
            } else {
                task->sequenceIndex = 0xFFFF;
                Sfx_ReleaseCachedBuffer(0x94, 0xF);
                Sfx_ReleaseCachedBuffer(0x19C, 0xF);
            }
        }
    }
    (task->frameIndex)++;
}

/*
 * Coordinates falling meteor impacts across target positions for Cometeo.
 */
// FUNCTION: FF7 0x005a4504
void __cdecl BattleFx_MagicComet2MeteorSequenceTask(void)
{
    BattleFxComet2MeteorSlot *taskPtr;
    BattleFxMeteorParticle *meteor;
    int loop;
    VECTOR vec;

    taskPtr = (BattleFxComet2MeteorSlot *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (taskPtr->step == 0) {
        meteor = (BattleFxMeteorParticle *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicComet2DescendingModelTask);
        meteor->posX = g_BattleActorRecords[g_BattleMagicComet2_CasterActorIdx].posX + (SinFixed(g_BattleActorRecords[g_BattleMagicComet2_CasterActorIdx].yaw) * 0xBB8 >> 12);
        meteor->posY = 0xE890;
        meteor->posZ = g_BattleActorRecords[g_BattleMagicComet2_CasterActorIdx].posZ + (CosFixed(g_BattleActorRecords[g_BattleMagicComet2_CasterActorIdx].yaw) * 0xBB8 >> 12);
        meteor->velX = (taskPtr->targetX - meteor->posX) / 10;
        vec.vx = meteor->velX;
        meteor->velY = (taskPtr->targetY - meteor->posY) / 10;
        vec.vy = meteor->velY;
        meteor->velZ = (taskPtr->targetZ - meteor->posZ) / 10;
        vec.vz = meteor->velZ;
        NormalizeVectorFixed(&vec, &vec);
        meteor->posX -= (short)(vec.vx * 0x1F4 >> 12);
        meteor->posY -= (short)(vec.vy * 0x1F4 >> 12);
        meteor->posZ -= (short)(vec.vz * 0x1F4 >> 12);
    }
    if (taskPtr->step == 9) {
        meteor = (BattleFxMeteorParticle *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicComet2ExpandingModelPairTask);
        *(Int64Pair *)((char *)meteor + 4) = *(Int64Pair *)((char *)taskPtr + 4);
        for (loop = 0; loop < 5; loop++) {
            meteor = (BattleFxMeteorParticle *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicComet2BouncingDebrisTask);
            *(Int64Pair *)((char *)meteor + 4) = *(Int64Pair *)((char *)taskPtr + 4);
            meteor->posY = 0;
            meteor->velX = rand() % 500 - 250;
            meteor->velY = -(rand() % 200 + 300);
            meteor->velZ = rand() % 500 - 250;
            meteor->tiltX = (rand() & 0x3F) - 32;
            meteor->tiltY = (rand() & 0x3F) - 32;
            meteor->tiltZ = (rand() & 0x3F) - 32;
            meteor->randomSpan = rand() % 3584 + 512;
            meteor->ownerSlot = 0;
            meteor->randomFlag = rand() & 1;
        }
    }
    if (taskPtr->step >= 10 && taskPtr->step < 25) {
        for (loop = 0; loop < 1; loop++) {
            meteor = (BattleFxMeteorParticle *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicComet2AnimatedSpriteTask);
            meteor->posX = taskPtr->targetX + rand() % 2560 + -1280;
            meteor->posY = taskPtr->targetY - rand() % 1280;
            meteor->posZ = taskPtr->targetZ + rand() % 2560 + -1280;
        }
    }
    if (taskPtr->step == 25) {
        taskPtr->ownerSlot = BATTLE_TASK_FREE;
    }
    (taskPtr->step)++;
}

/*
 * Animates and renders a descending meteor model for Cometeo.
 */
// FUNCTION: FF7 0x005a48f6
void __cdecl BattleFx_MagicComet2DescendingModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[48];
        PsxMatrix *tiltMatrix;
        struct { unsigned char pad_to_4[0x4]; PsxMatrix *worldMatrix; };
        struct { unsigned char pad_to_8[0x8]; BattleFxSpecialEnergyOrbSlot *sparkPixel; };
        struct { unsigned char pad_to_C[0xC]; PsxMatrix *dataBlock; };
        struct { unsigned char pad_to_10[0x10]; BattleFxComet2Offset *sparkOffset; };
        struct { unsigned char pad_to_14[0x14]; int sparkIndex; };
        struct { unsigned char pad_to_18[0x18]; SVECTOR negVelX; };
        struct { unsigned char pad_to_20[0x20]; int pointDepth; };
        struct { unsigned char pad_to_24[0x24]; BattleFxMeteorModelSlot *cometModel; };
        struct { unsigned char pad_to_28[0x28]; BattleFxPosition3s *cometPos; };
        struct { unsigned char pad_to_2C[0x2C]; int sparkAngle; };
    } frame;
#pragma pack(pop)

    frame.cometModel = (BattleFxMeteorModelSlot *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleMagicComet2_ModelReq1.modelGroup = Gfx_ModelTableGetRenderObject(0, g_BattleMagicComet2_ResourceTable);
    g_BattleMagicComet2_ModelReq2[0] = Gfx_ModelTableGetRenderObject(1, g_BattleMagicComet2_ResourceTable);
    if (g_pBattle3dScene != 0) {
        g_BattleMagicComet2_ModelReq5.modelGroup = g_pBattle3dScene->modelRequest;
    } else {
        g_BattleMagicComet2_ModelReq5.modelGroup = 0;
    }
    frame.dataBlock = (PsxMatrix *)Gfx_GetPsxScratchpadPtr(0);
    frame.worldMatrix = (PsxMatrix *)((int)frame.dataBlock + 0x1E);
    frame.tiltMatrix = (PsxMatrix *)((int)frame.worldMatrix + 0x1E);
    frame.sparkOffset = (BattleFxComet2Offset *)((int)frame.tiltMatrix + 0x1E);
    frame.cometPos = (BattleFxPosition3s *)((int)frame.sparkOffset + 8);
    frame.negVelX.vx = -frame.cometModel->velocityX;
    frame.negVelX.vy = -frame.cometModel->velocityY;
    frame.negVelX.vz = -frame.cometModel->velocityZ;
    Gte_BuildLookBasisFromDirection(&frame.negVelX.vx, (short *)frame.dataBlock);
    frame.dataBlock->t[0] = frame.cometModel->posX;
    frame.dataBlock->t[1] = frame.cometModel->posY;
    frame.dataBlock->t[2] = frame.cometModel->posZ;
    ScaleMatrix(frame.dataBlock, g_BattleMagicComet2_ModelReq4);
    MultiplyMatrix(&g_BattleCameraTransform, frame.dataBlock, frame.worldMatrix);
    SetRotMatrix(frame.worldMatrix);
    SetTransMatrix(frame.worldMatrix);
    g_BattleMagicComet2_DrawCursor = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleMagicComet2_ModelReq2, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleMagicComet2_DrawCursor);
    RotMatrix((SVECTOR *)g_BattleMagicComet2_SpinAngleXY, frame.tiltMatrix);
    MultiplyMatrix(frame.worldMatrix, frame.tiltMatrix, frame.tiltMatrix);
    SetRotMatrix(frame.tiltMatrix);
    g_BattleMagicComet2_DrawCursor = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleMagicComet2_ModelReq1, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleMagicComet2_DrawCursor);
    g_BattleMagicComet2_Matrix1.t[0] = frame.cometModel->posX;
    g_BattleMagicComet2_Matrix1.t[2] = frame.cometModel->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &g_BattleMagicComet2_Matrix1, frame.worldMatrix);
    SetRotMatrix(frame.worldMatrix);
    SetTransMatrix(frame.worldMatrix);
    g_BattleMagicComet2_DrawCursor = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleMagicComet2_ModelReq5, g_BattleCurrentOrderingTable + BATTLE_OT_AFTER_SORTED_2, 1, g_BattleMagicComet2_DrawCursor);
    if (g_BattleIsPaused == 0) {
        SetRotMatrix(frame.dataBlock);
        SetTransMatrix(frame.dataBlock);
        for (frame.sparkIndex = 0; frame.sparkIndex < 3; frame.sparkIndex++) {
            frame.sparkAngle = (frame.sparkIndex << 12) / 3 + rand() % 0x555;
            frame.sparkOffset->sinOffset = SinFixed(frame.sparkAngle) * 0x3E8 >> 12;
            frame.sparkOffset->cosOffset = CosFixed(frame.sparkAngle) * 0x3E8 >> 12;
            frame.sparkOffset->jitter = rand() % 1000;
            RotTransPoint((void *)frame.sparkOffset, (int *)frame.cometPos, &frame.pointDepth);
            frame.sparkPixel = (BattleFxSpecialEnergyOrbSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicComet2SparkPixelTask);
            frame.sparkPixel->posX = frame.cometPos->posX;
            frame.sparkPixel->posY = frame.cometPos->posY;
            frame.sparkPixel->posZ = frame.cometPos->posZ;
            frame.sparkOffset->jitter = 0;
            RotateSVectorByInstalledMatrix((void *)frame.sparkOffset, frame.cometPos);
            frame.sparkPixel->velocityX = -((BattleFxVelocityVec3 *)frame.cometPos)->velocityX / 6;
            frame.sparkPixel->velocityY = -((BattleFxVelocityVec3 *)frame.cometPos)->velocityY / 6;
            frame.sparkPixel->velocityZ = -((BattleFxVelocityVec3 *)frame.cometPos)->velocityZ / 6;
        }
        g_BattleMagicComet2_ModelReq3[0] ^= 0x80;
        g_BattleMagicComet2_SpinAngleXY[0] += 0x80;
        g_BattleMagicComet2_SpinAngleXY[1] += 0x100 /* 22.5 deg */;
        g_BattleMagicComet2_SpinAngleZ += 0x180;
        (frame.cometModel->frameCount)++;
        if (frame.cometModel->frameCount >= 10) {
            frame.cometModel->ownerSlot = BATTLE_TASK_FREE;
        }
        frame.cometModel->posX += frame.cometModel->velocityX;
        frame.cometModel->posY += frame.cometModel->velocityY;
        frame.cometModel->posZ += frame.cometModel->velocityZ;
    }
}

/*
 * Renders and advances spark pixel particles for Cometeo.
 */
// FUNCTION: FF7 0x005a4d2f
void __cdecl BattleFx_MagicComet2SparkPixelTask(void)
{
    int screenPosition;
    BattleFxLivewireDotSlot *taskPtr;
    int clipFlags;
    int z;
    BattleFxPolyF4Prim *packet;

    taskPtr = (BattleFxLivewireDotSlot *)BATTLE_CURRENT_TASK_SLOT;
    packet = (BattleFxPolyF4Prim *)g_BattleMagicComet2_DrawCursor;
    SetRotMatrix(&g_BattleCameraTransform);
    SetTransMatrix(&g_BattleCameraTransform);
    RefreshGlobalTransformState();
    z = Gfx_TransformVertexSlot0ReturnDepth(&taskPtr->posX, &screenPosition, &clipFlags);
    if ((clipFlags & 0x60000) == 0) {
        packet->tag = packet->tag & 0xFFFFFF | 0x2000000;
        packet->primCode = 0x68;
        packet->primCode |= 2;
        packet->colorR = 0xFF;
        packet->colorG = rand() & 0xFF;
        packet->colorB = 0;
        Gfx_DrawPsxPrimitive(packet);
    }
    if (g_BattleIsPaused == 0) {
        if (taskPtr->step < 6) {
            taskPtr->posX += taskPtr->velocityX;
            taskPtr->posY += taskPtr->velocityY;
            taskPtr->posZ += taskPtr->velocityZ;
        }
        (taskPtr->step)++;
        if (taskPtr->step >= 8) {
            taskPtr->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Renders and animates expanding explosion model pair for Cometeo.
 */
// FUNCTION: FF7 0x005a4e6d
void __cdecl BattleFx_MagicComet2ExpandingModelPairTask(void)
{
    int localFrame;
    BattleCometExpandingPairState *task;
    int matrix;

    task = (BattleCometExpandingPairState *)BATTLE_CURRENT_TASK_SLOT;
    matrix = Gfx_GetPsxScratchpadPtr(0);
    localFrame = task->frameIndex;
    if (localFrame < 10) {
        g_BattleMagicComet2_ModelReq6.flags = 0x88;
        g_BattleMagicComet2_ModelReq6.fadeLevel = localFrame * -4096 / 10 + 4096;
        SetDrawColorRGB(0xFF, 0xFF, 0xFF);
    } else {
        localFrame -= 10;
        if (localFrame < 0) {
            g_BattleMagicComet2_ModelReq6.flags = 0x88;
            g_BattleMagicComet2_ModelReq6.fadeLevel = 0;
        } else {
            localFrame = localFrame;
            if (localFrame < 10) {
                g_BattleMagicComet2_ModelReq6.flags = 0x88;
                g_BattleMagicComet2_ModelReq6.fadeLevel = (localFrame << 12) / 10;
                SetDrawColorRGB(0, 0, 0);
            } else {
                task->ownerSlot = BATTLE_TASK_FREE;
                return;
            }
        }
    }
    g_BattleMagicComet2_Matrix2.m[2][2] = task->frameIndex * 4608 / 10 + 0x800;
    g_BattleMagicComet2_Matrix2.m[1][1] = g_BattleMagicComet2_Matrix2.m[2][2];
    g_BattleMagicComet2_Matrix2.m[0][0] = g_BattleMagicComet2_Matrix2.m[1][1];
    g_BattleMagicComet2_Matrix2.t[0] = task->transX;
    g_BattleMagicComet2_Matrix2.t[2] = task->transZ;
    MultiplyMatrix(&g_BattleCameraTransform, &g_BattleMagicComet2_Matrix2, (void *)matrix);
    SetRotMatrix((void *)matrix);
    SetTransMatrix((void *)matrix);
    g_BattleMagicComet2_ModelReq6.modelGroup = Gfx_ModelTableGetRenderObject(2, g_BattleMagicComet2_ResourceTable);
    g_BattleMagicComet2_DrawCursor = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleMagicComet2_ModelReq6, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleMagicComet2_DrawCursor);
    g_BattleMagicComet2_ModelReq6.modelGroup = Gfx_ModelTableGetRenderObject(3, g_BattleMagicComet2_ResourceTable);
    g_BattleMagicComet2_ModelReq6.flags = 0x88;
    g_BattleMagicComet2_DrawCursor = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleMagicComet2_ModelReq6, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleMagicComet2_DrawCursor);
    if (g_BattleIsPaused == 0) {
        (task->frameIndex)++;
    }
}

/*
 * Animates and renders bouncing stone debris models for Cometeo.
 */
// FUNCTION: FF7 0x005a506e
void __cdecl BattleFx_MagicComet2BouncingDebrisTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[60];
        SVECTOR rotX;
        struct { unsigned char pad_to_8[0x8]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_28[0x28]; BattleCometDebris *debris; };
        struct { unsigned char pad_to_2C[0x2C]; VECTOR scaleVec; };
    } frame;
#pragma pack(pop)

    frame.debris = (BattleCometDebris *)BATTLE_CURRENT_TASK_SLOT;
    if (frame.debris->modelVariant == 0) {
        g_BattleMagicComet2_ModelReq7.modelGroup = Gfx_ModelTableGetRenderObject(4, g_BattleMagicComet2_ResourceTable);
    } else {
        g_BattleMagicComet2_ModelReq7.modelGroup = Gfx_ModelTableGetRenderObject(5, g_BattleMagicComet2_ResourceTable);
    }
    if (frame.debris->state < 2) {
        g_BattleMagicComet2_ModelReq7.flags = 0x80;
        g_BattleMagicComet2_ModelReq7.fadeLevel = 0;
    } else {
        g_BattleMagicComet2_ModelReq7.flags = 0x88;
        g_BattleMagicComet2_ModelReq7.fadeLevel = ((frame.debris->step << 12) / 8);
        if (frame.debris->modelVariant == 0) {
            g_BattleMagicComet2_ModelReq7.modelGroup = Gfx_ModelTableGetRenderObject(6, g_BattleMagicComet2_ResourceTable);
        } else {
            g_BattleMagicComet2_ModelReq7.modelGroup = Gfx_ModelTableGetRenderObject(7, g_BattleMagicComet2_ResourceTable);
        }
    }
    frame.rotX.vx = frame.debris->rotX << 4;
    frame.rotX.vy = frame.debris->rotY << 4;
    frame.rotX.vz = frame.debris->rotZ << 4;
    RotMatrix(&frame.rotX, &frame.rotMatrix);
    frame.rotMatrix.t[0] = frame.debris->posX;
    frame.rotMatrix.t[1] = frame.debris->posY;
    frame.rotMatrix.t[2] = frame.debris->posZ;
    frame.scaleVec.vz = frame.debris->scale;
    frame.scaleVec.vy = frame.scaleVec.vz;
    frame.scaleVec.vx = frame.scaleVec.vy;
    ScaleMatrix(&frame.rotMatrix, &frame.scaleVec);
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.rotMatrix);
    SetRotMatrix(&frame.rotMatrix);
    SetTransMatrix(&frame.rotMatrix);
    SetDrawColorRGB(0, 0, 0);
    g_BattleMagicComet2_DrawCursor = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleMagicComet2_ModelReq7, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleMagicComet2_DrawCursor);
    g_BattleMagicComet2_Matrix3.t[0] = frame.debris->posX;
    g_BattleMagicComet2_Matrix3.t[1] = 0;
    g_BattleMagicComet2_Matrix3.t[2] = frame.debris->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &g_BattleMagicComet2_Matrix3, &frame.rotMatrix);
    SetRotMatrix(&frame.rotMatrix);
    SetTransMatrix(&frame.rotMatrix);
    g_BattleMagicComet2_DrawCursor = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleMagicComet2_ModelReq8, g_BattleCurrentOrderingTable + BATTLE_OT_AFTER_SORTED_2, 1, g_BattleMagicComet2_DrawCursor);
    if (g_BattleIsPaused == 0) {
        if (frame.debris->state >= 2) {
            (frame.debris->step)++;
            if (frame.debris->step >= 8) {
                frame.debris->state = 0xFFFF;
                return;
            }
        }
        frame.debris->posX += frame.debris->velocityX;
        frame.debris->posY += frame.debris->velocityY;
        frame.debris->posZ += frame.debris->velocityZ;
        frame.debris->velocityY += 50;
        if (frame.debris->posY > 0) {
            frame.debris->posY = -frame.debris->posY;
            frame.debris->velocityY = (-frame.debris->velocityY) / 4;
            frame.debris->velocityX >>= 1;
            frame.debris->velocityZ >>= 1;
            frame.debris->rotStepX >>= 1;
            frame.debris->rotStepY >>= 1;
            frame.debris->rotStepZ >>= 1;
            (frame.debris->state)++;
        }
        *(char *)&frame.debris->rotX += *(char *)&frame.debris->rotStepX;
        *(char *)&frame.debris->rotY += *(char *)&frame.debris->rotStepY;
        *(char *)&frame.debris->rotZ += *(char *)&frame.debris->rotStepZ;
    }
}

/*
 * Renders and advances animated impact sprite for Cometeo.
 */
// FUNCTION: FF7 0x005a53f7
void __cdecl BattleFx_MagicComet2AnimatedSpriteTask(void)
{
    BattleTaskSlot *task;

    task = BATTLE_CURRENT_TASK_SLOT;
    g_BattleMagicComet2_SpriteDrawReq.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(8, g_BattleMagicComet2_ResourceTable);
    g_BattleMagicComet2_SpriteDrawReq.drawObj = Gfx_ModelTableGetSpriteObject(8, g_BattleMagicComet2_ResourceTable);
    g_BattleMagicComet2_SpriteDrawReq.frame = task->frameCounter;
    BattleFx_InstallBillboardMatrix((void *)((int)task + 4), 0x2500 /* 2.3125 */, 0);
    g_BattleMagicComet2_DrawCursor = BattleFx_SubmitSpriteDraw(&g_BattleMagicComet2_SpriteDrawReq, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleMagicComet2_DrawCursor);
    if (g_BattleIsPaused == 0 && g_BattleMagicComet2_SpriteDrawReq.spriteGroup != 0) {
        (task->frameCounter)++;
        if (task->frameCounter >= g_BattleMagicComet2_SpriteDrawReq.spriteGroup->header->frameCount) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Waits for color ramp to complete before terminating Cometeo effect.
 */
// FUNCTION: FF7 0x005a54c8
void __cdecl BattleFx_MagicComet2WaitRampEndTask(void)
{
    BattleFxIcicleDropRamp *task;

    task = (BattleFxIcicleDropRamp *)BATTLE_CURRENT_TASK_SLOT;
    task->toggle ^= 1;
    if (g_BattleSecondaryTaskActiveCount <= 1) {
        Battle_StartFixedPointRamp(0, 4);
        task->ownerSlot = BATTLE_TASK_FREE;
    }
}
} /* extern "C" */
