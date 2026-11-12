// src/ff7/src/battle/battle_fx_magic_comet.cpp
/* Magic Effect [45]: Comet (Japanese: コメット, dev tag: "MAGIC/COMET") */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle visual effect for the Comet magic: a falling meteor per target with spark
//   pixels, expanding explosion models, bouncing debris, impact sprites and camera shake.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/enums/sound.h"
#include "ff7/battle_tasks.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_magic.h"
#include "lib/types/graphics.h"
#include "lib/codegen.h"
#include <stdlib.h>
#include "ff7/camera_script.h"
#include <string.h>
#include "ff7/api/battle.h"
#include "lib/api/graphics.h"
#include "ff7/types/battle_3d.h"
#include "ff7/api/menu.h"
#include "lib/api/sound.h"
extern "C" {

struct BattleFxModelDraw;

#pragma intrinsic(abs)

#pragma intrinsic(memset)
#pragma intrinsic(memcpy)

// clang-format off

// GLOBAL: FF7 0x008f1e68
unsigned char g_BattleMagicComet_MeteorDelayTable[16] = {0x4c, 0x00, 0x00, 0x00, 0x4d, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x008f1e78
unsigned int g_BattleMagicComet_MeteorDelayDef[2] = {(unsigned int)&g_BattleMagicComet_MeteorDelayTable, 0x00000000};
// GLOBAL: FF7 0x008f1e80
unsigned int g_BattleMagicComet_ResourceRequests[45] = {0x006400b4, (unsigned int)&g_BattleMagicComet_ModelNames1, 0x00000000, 0x00000000, 0x00000000, 0x006400b1, (unsigned int)((char *)&g_BattleMagicComet_ModelNames1 + 0xc), 0x00000000, 0x00000000, 0x00000000, 0x006400b1, (unsigned int)&g_BattleMagicComet_ModelNames2, 0x00000000, 0x00000000, 0x00000000, 0x006400b1, (unsigned int)((char *)&g_BattleMagicComet_ModelNames2 + 0xc), 0x00000000, 0x00000000, 0x00000000, 0x006400b4, (unsigned int)&g_BattleMagicComet_ModelNames3, 0x00000000, 0x00000000, 0x00000000, 0x006400b4, (unsigned int)((char *)&g_BattleMagicComet_ModelNames3 + 0xc), 0x00000000, 0x00000000, 0x00000000, 0x006400b1, (unsigned int)&g_BattleMagicComet_ModelNames4, 0x00000000, 0x00000000, 0x00000000, 0x006400b1, (unsigned int)((char *)&g_BattleMagicComet_ModelNames4 + 0xc), 0x00000000, 0x00000000, 0x00000000, 0x00012a39, (unsigned int)&g_BattleMagicComet_TextureNames, (unsigned int)((char *)&g_BattleMagicComet_TextureNames + 0xc), 0x00000000, 0x00000000};
// GLOBAL: FF7 0x008f1f38
unsigned char g_BattleMagicComet_ModelReq1[16] = {0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x008f1f48
unsigned char g_BattleMagicComet_ModelReq2[8] = {0x00, 0x00, 0x00, 0x00, 0x88, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x008f1f50
unsigned char g_BattleMagicComet_ModelReq3[8] = {0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x008f1f58
unsigned char g_BattleMagicComet_ModelReq4[16] = {0x00, 0x0a, 0x00, 0x00, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x008f1f68
PsxMatrix g_BattleMagicComet_Matrix1 = {{{0x1000, 0, 0}, {0, 0x1000, 0}, {0, 0, 0x1000}}, {0, 0, 0}};
// GLOBAL: FF7 0x008f1f88
unsigned char g_BattleMagicComet_ModelReq5[16] = {0x00, 0x00, 0x00, 0x00, 0x88, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x008f1f98
unsigned char g_BattleMagicComet_ModelReq6[16] = {0x00, 0x00, 0x00, 0x00, 0x88, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x008f1fa8
PsxMatrix g_BattleMagicComet_Matrix2 = {{{0x1000, 0, 0}, {0, 0x1000, 0}, {0, 0, 0x1000}}, {0, 0, 0}};
// GLOBAL: FF7 0x008f1fc8
unsigned char g_BattleMagicComet_ModelReq7[16] = {0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x008f1fd8
PsxMatrix g_BattleMagicComet_Matrix3 = {{{0x500, 0, 0}, {0, 0x500, 0}, {0, 0, 0x500}}, {0, 0, 0}};
// GLOBAL: FF7 0x008f1ff8
unsigned char g_BattleMagicComet_ModelReq8[16] = {0x00, 0x00, 0x00, 0x00, 0x88, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x008f2008
BattleFxSpriteRequest g_BattleMagicComet_SpriteDrawReq = {0, 0, (char)0x80, (char)0x80, (char)0x80, 0x2e, 0, 0};
// GLOBAL: FF7 0x008f2018
unsigned char g_BattleMagicComet_ModelNames1[24] =
    "COMET_1.RSD\0"
    "COMET_2.RSD";
// GLOBAL: FF7 0x008f2030
unsigned char g_BattleMagicComet_ModelNames2[24] =
    "EXP_1.RSD\0\0\0"
    "SONIC_1.RSD";
// GLOBAL: FF7 0x008f2048
unsigned char g_BattleMagicComet_ModelNames3[24] =
    "STONE_1.RSD\0"
    "STONE_2.RSD";
// GLOBAL: FF7 0x008f2060
unsigned char g_BattleMagicComet_ModelNames4[24] =
    "STONE_1.RSD\0"
    "STONE_2.RSD";
// GLOBAL: FF7 0x008f2078
unsigned char g_BattleMagicComet_TextureNames[23] =
    "BOMB_A.TIM\0\0"
    "BOMB_A.SPT";
// GLOBAL: FF7 0x00bd5b48
int g_BattleMagicCometDrawListHandle = 0;
// GLOBAL: FF7 0x00bd5b4c
int g_BattleMagicComet_LoadedRes = 0;
// GLOBAL: FF7 0x00bd5b50
struct GfxTableCursor *g_BattleMagicComet_TimelineState = 0;
unsigned char g_bssPad_00bd5b54[4] = {0};
// GLOBAL: FF7 0x00bd5b58
short g_BattleMagicComet_Coords[2] = {0};
// GLOBAL: FF7 0x00bd5b5c
short g_BattleMagicComet_CoordZ = 0;
// clang-format on


/*
 * Loads resources for the Comet magic effect when not already loaded.
 */
// FUNCTION: FF7 0x005a6bf0
void __cdecl BattleFx_MagicLoadResComet(void)
{
    GfxModelTableBank *parentHandle;
    GfxResourceRequest requestRecord;
    int kind;
    char pathBuffer[200];
    GfxResourceRequest *pReq;
    int requestTable;

    if (g_pBattle3dScene != 0 && g_BattleMagicComet_TimelineState == 0) {
        kind = 9;
        requestTable = (int)&g_BattleMagicComet_ResourceRequests;
        parentHandle = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pReq = &requestRecord;
        Gfx_InitResourceRequest(pReq);
        pReq->groupTag = 0;
        // STRING: FF7 0x008f2090
        pReq->resourcePath = BattleFx_BuildResourcePath("MAGIC/COMET", pathBuffer);
        Gfx_LoadModelTableIfAbsent(kind, requestTable, pReq, &g_BattleMagicComet_TimelineState, parentHandle);
        SfxRegisterSoundGroups((struct SoundGroup **)&g_BattleMagicComet_MeteorDelayDef);
    }
}

/*
 * Entry point for Comet magic effect initializing execution.
 */
// FUNCTION: FF7 0x005a6c91
void __cdecl BattleFx_MagicEnterComet(int targetMask, int actorIdx)
{
    g_BattleMagicCometDrawListHandle = (int)&g_GfxPsxPacketScratch;
    BattleFx_MagicLoadResComet();
    BattleFx_MagicBeginComet(targetMask, actorIdx);
}

/*
 * Iterates targets and begins Comet effect execution.
 */
// FUNCTION: FF7 0x005a6cb5
void __cdecl BattleFx_MagicBeginComet(int targetMask, int actorIdx)
{
    Battle_AllocateResourceSlot(BattleFx_MagicCometWaitRampEndTask);
    g_BattleMagicComet_LoadedRes = actorIdx;
    if (REG_ALLOC_HINT) {
    }
    Battle_CreateBitPatternCallbackTask(targetMask, actorIdx, 0, BattleFx_MagicSpawnCometPerTarget);
}

/*
 * Spawns Comet timeline task for a single target.
 */
// FUNCTION: FF7 0x005a6ce6
void __cdecl BattleFx_MagicSpawnCometPerTarget(int actor)
{
    BattleFxCometTimelineSlot *effect;

    effect = (BattleFxCometTimelineSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicCometTimelineTask);
    effect->recordType = actor;
    Battle_GetBoneWorldPos(actor, g_BattleActorRecords[actor].targetBone, &effect->targetX);
}

/*
 * Timeline task managing falling meteor animation and camera shake for Comet.
 */
// FUNCTION: FF7 0x005a6d37
void __cdecl BattleFx_MagicCometTimelineTask(void)
{
    BattleFxCometTimelineSlot *taskSlotPtr;
    int localStep;
    BattleFxMagicCometParticle *meteor;
    int loop;
    VECTOR vec;

    taskSlotPtr = (BattleFxCometTimelineSlot *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    localStep = taskSlotPtr->step;
    if (localStep == 0) {
        Battle_StartFixedPointRamp(0x800, 4);
        g_BattleFxTimelineState.channel2 = 0;
        g_BattleFxTimelineState.channel1 = g_BattleFxTimelineState.channel2;
        g_BattleFxTimelineState.red = g_BattleFxTimelineState.channel1;
    }
    localStep -= 4;
    if (localStep == 0) {
        meteor = (BattleFxMagicCometParticle *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicCometDescendingModelTask);
        meteor->posX = g_BattleActorRecords[g_BattleMagicComet_LoadedRes].posX + (SinFixed(g_BattleActorRecords[g_BattleMagicComet_LoadedRes].yaw) * 0x7D0 >> 12);
        meteor->posY = 0xEC78;
        meteor->posZ = g_BattleActorRecords[g_BattleMagicComet_LoadedRes].posZ + (CosFixed(g_BattleActorRecords[g_BattleMagicComet_LoadedRes].yaw) * 0x7D0 >> 12);
        meteor->velocityX = (taskSlotPtr->targetX - meteor->posX) / 15;
        vec.vx = meteor->velocityX;
        meteor->velocityY = (0 - meteor->posY) / 15;
        vec.vy = meteor->velocityY;
        meteor->velocityZ = (taskSlotPtr->targetZ - meteor->posZ) / 15;
        vec.vz = meteor->velocityZ;
        NormalizeVectorFixed(&vec, &vec);
        meteor->posX -= (short)(vec.vx * 0x12C >> 12);
        meteor->posY -= (short)(vec.vy * 0x12C >> 12);
        meteor->posZ -= (short)(vec.vz * 0x12C >> 12);
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_2, 0x40, 0);
    }
    if (localStep == 15) {
        meteor = (BattleFxMagicCometParticle *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicCometExpandingModelPairTask);
        meteor->posX = taskSlotPtr->targetX;
        meteor->posY = 0;
        meteor->posZ = taskSlotPtr->targetZ;
        for (loop = 0; loop < 10; loop++) {
            meteor = (BattleFxMagicCometParticle *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicCometBouncingDebrisTask);
            *(Int64Pair *)&meteor->posX = *(Int64Pair *)&taskSlotPtr->targetX;
            meteor->posY = 0;
            meteor->velocityX = rand() % 500 - 250;
            meteor->velocityY = -(rand() % 200 + 300);
            meteor->velocityZ = rand() % 500 - 250;
            meteor->rotStepX = (rand() & 0x3F) - 32;
            meteor->rotStepY = (rand() & 0x3F) - 32;
            meteor->rotStepZ = (rand() & 0x3F) - 32;
            meteor->scale = rand() % 0xC00 + 0x200;
            meteor->ownerSlot = 0;
            meteor->modelVariant = rand() & 1;
        }
        BattleFx_StartScreenFlashRamp(1);
    }
    if (localStep == 16) {
        BattleFx_FadeOutScreenFlashRamp(8);
    }
    if (localStep == 15) {
        Battle_PostActorHitOrHurtMessage(taskSlotPtr->recordType);
    }
    if (localStep >= 15 && localStep < 35) {
        for (loop = 0; loop < 1; loop++) {
            meteor = (BattleFxMagicCometParticle *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicCometAnimatedSpriteTask);
            meteor->posX = taskSlotPtr->targetX + rand() % 2000 + -1000;
            meteor->posY = 0 - rand() % 1000;
            meteor->posZ = taskSlotPtr->targetZ + rand() % 2000 + -1000;
        }
    }
    if (localStep == 35) {
        taskSlotPtr->recordType = 0xFFFF;
        Sfx_ReleaseCachedBuffer(0x4C, 0xF);
        Sfx_ReleaseCachedBuffer(0x4D, 0xF);
    }
    (taskSlotPtr->step)++;
}

/*
 * Animates and renders a descending meteor model for Comet.
 */
// FUNCTION: FF7 0x005a71b0
void __cdecl BattleFx_MagicCometDescendingModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[48];
        PsxMatrix *partMatrix;
        struct { unsigned char pad_to_4[0x4]; PsxMatrix *transformMatrix; };
        struct { unsigned char pad_to_8[0x8]; BattleFxSpecialEnergyOrbSlot *sparkPixel; };
        struct { unsigned char pad_to_C[0xC]; PsxMatrix *lookBasisMatrix; };
        struct { unsigned char pad_to_10[0x10]; int sparkIndex; };
        struct { unsigned char pad_to_14[0x14]; BattleFxComet2Offset *descendOffset; };
        struct { unsigned char pad_to_18[0x18]; SVECTOR dirX; };
        struct { unsigned char pad_to_20[0x20]; int pointDepth; };
        struct { unsigned char pad_to_24[0x24]; BattleFxMeteorModelSlot *comet; };
        struct { unsigned char pad_to_28[0x28]; BattleFxPosition3s *positionVec; };
        struct { unsigned char pad_to_2C[0x2C]; int angle; };
    } frame;
#pragma pack(pop)

    frame.comet = (BattleFxMeteorModelSlot *)BATTLE_CURRENT_TASK_SLOT;
    (*(int *)&g_BattleMagicComet_ModelReq1) = Gfx_ModelTableGetRenderObject(0, g_BattleMagicComet_TimelineState);
    (*(int *)&g_BattleMagicComet_ModelReq2) = Gfx_ModelTableGetRenderObject(1, g_BattleMagicComet_TimelineState);
    if (g_pBattle3dScene != 0) {
        (*(int *)&g_BattleMagicComet_ModelReq5) = g_pBattle3dScene->modelRequest;
    } else {
        (*(int *)&g_BattleMagicComet_ModelReq5) = 0;
    }
    frame.lookBasisMatrix = (PsxMatrix *)Gfx_GetPsxScratchpadPtr(0);
    frame.transformMatrix = (PsxMatrix *)((int)frame.lookBasisMatrix + 0x1E);
    frame.partMatrix = (PsxMatrix *)((int)frame.transformMatrix + 0x1E);
    frame.descendOffset = (BattleFxComet2Offset *)((int)frame.partMatrix + 0x1E);
    frame.positionVec = (BattleFxPosition3s *)((int)frame.descendOffset + 8);
    frame.dirX.vx = -frame.comet->velocityX;
    frame.dirX.vy = -frame.comet->velocityY;
    frame.dirX.vz = -frame.comet->velocityZ;
    Gte_BuildLookBasisFromDirection(&frame.dirX.vx, (short *)frame.lookBasisMatrix);
    frame.lookBasisMatrix->t[0] = frame.comet->posX;
    frame.lookBasisMatrix->t[1] = frame.comet->posY;
    frame.lookBasisMatrix->t[2] = frame.comet->posZ;
    ScaleMatrix(frame.lookBasisMatrix, (int *)&g_BattleMagicComet_ModelReq4);
    MultiplyMatrix(&g_BattleCameraTransform, frame.lookBasisMatrix, frame.transformMatrix);
    SetRotMatrix(frame.transformMatrix);
    SetTransMatrix(frame.transformMatrix);
    g_BattleMagicCometDrawListHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleMagicComet_ModelReq2, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleMagicCometDrawListHandle);
    RotMatrix((SVECTOR *)g_BattleMagicComet_Coords, frame.partMatrix);
    MultiplyMatrix(frame.transformMatrix, frame.partMatrix, frame.partMatrix);
    SetRotMatrix(frame.partMatrix);
    g_BattleMagicCometDrawListHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleMagicComet_ModelReq1, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleMagicCometDrawListHandle);
    g_BattleMagicComet_Matrix1.t[0] = frame.comet->posX;
    g_BattleMagicComet_Matrix1.t[2] = frame.comet->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &g_BattleMagicComet_Matrix1, frame.transformMatrix);
    SetRotMatrix(frame.transformMatrix);
    SetTransMatrix(frame.transformMatrix);
    g_BattleMagicCometDrawListHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleMagicComet_ModelReq5, g_BattleCurrentOrderingTable + BATTLE_OT_AFTER_SORTED_2, 1, g_BattleMagicCometDrawListHandle);
    if (g_BattleIsPaused == 0) {
        SetRotMatrix(frame.lookBasisMatrix);
        SetTransMatrix(frame.lookBasisMatrix);
        for (frame.sparkIndex = 0; frame.sparkIndex < 3; frame.sparkIndex++) {
            frame.angle = (frame.sparkIndex << 12) / 3 + rand() % 0x555;
            frame.descendOffset->sinOffset = SinFixed(frame.angle) * 0x3E8 >> 12;
            frame.descendOffset->cosOffset = CosFixed(frame.angle) * 0x3E8 >> 12;
            frame.descendOffset->jitter = rand() % 1000;
            RotTransPoint((void *)frame.descendOffset, (int *)frame.positionVec, &frame.pointDepth);
            frame.sparkPixel = (BattleFxSpecialEnergyOrbSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicCometSparkPixelTask);
            frame.sparkPixel->posX = frame.positionVec->posX;
            frame.sparkPixel->posY = frame.positionVec->posY;
            frame.sparkPixel->posZ = frame.positionVec->posZ;
            frame.descendOffset->jitter = 0;
            RotateSVectorByInstalledMatrix((void *)frame.descendOffset, frame.positionVec);
            frame.sparkPixel->velocityX = -((BattleFxVelocityVec3 *)frame.positionVec)->velocityX / 6;
            frame.sparkPixel->velocityY = -((BattleFxVelocityVec3 *)frame.positionVec)->velocityY / 6;
            frame.sparkPixel->velocityZ = -((BattleFxVelocityVec3 *)frame.positionVec)->velocityZ / 6;
        }
        (*(short *)&g_BattleMagicComet_ModelReq3) ^= 0x80;
        g_BattleMagicComet_Coords[0] += 0x80;
        g_BattleMagicComet_Coords[1] += 0x100;
        g_BattleMagicComet_CoordZ += 384;
        (frame.comet->frameCount)++;
        if (frame.comet->frameCount >= 15) {
            frame.comet->ownerSlot = BATTLE_TASK_FREE;
        }
        frame.comet->posX += frame.comet->velocityX;
        frame.comet->posY += frame.comet->velocityY;
        frame.comet->posZ += frame.comet->velocityZ;
    }
}

/*
 * Renders and advances spark pixel particles for Comet.
 */
// FUNCTION: FF7 0x005a75e9
void __cdecl BattleFx_MagicCometSparkPixelTask(void)
{
    int screenPosition;
    BattleFxLivewireDotSlot *taskPtr;
    int clipFlags;
    int z;
    BattleFxPolyF4Prim *packet;

    taskPtr = (BattleFxLivewireDotSlot *)BATTLE_CURRENT_TASK_SLOT;
    packet = (BattleFxPolyF4Prim *)g_BattleMagicCometDrawListHandle;
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
 * Renders and animates expanding explosion model pair for Comet.
 */
// FUNCTION: FF7 0x005a7727
void __cdecl BattleFx_MagicCometExpandingModelPairTask(void)
{
    int localFrame;
    BattleCometExpandingPairState *task;
    int matrix;

    task = (BattleCometExpandingPairState *)BATTLE_CURRENT_TASK_SLOT;
    matrix = Gfx_GetPsxScratchpadPtr(0);
    localFrame = task->frameIndex;
    if (localFrame < 10) {
        (((BattleFxModelDraw *)&g_BattleMagicComet_ModelReq6)->flags) = 0x88;
        (((BattleFxModelDraw *)&g_BattleMagicComet_ModelReq6)->fadeLevel) = localFrame * -0x1000 / 0xA + 0x1000;
        SetDrawColorRGB(0xFF, 0xFF, 0xFF);
    } else {
        localFrame -= 10;
        if (localFrame < 0) {
            (((BattleFxModelDraw *)&g_BattleMagicComet_ModelReq6)->flags) = 0x88;
            (((BattleFxModelDraw *)&g_BattleMagicComet_ModelReq6)->fadeLevel) = 0;
        } else {
            localFrame = localFrame;
            if (localFrame < 15) {
                (((BattleFxModelDraw *)&g_BattleMagicComet_ModelReq6)->flags) = 0x88;
                (((BattleFxModelDraw *)&g_BattleMagicComet_ModelReq6)->fadeLevel) = (localFrame << 12) / 0xF;
                SetDrawColorRGB(0, 0, 0);
            } else {
                task->ownerSlot = BATTLE_TASK_FREE;
                return;
            }
        }
    }
    g_BattleMagicComet_Matrix2.m[2][2] = task->frameIndex * 5632 / 25 + 0xA00;
    g_BattleMagicComet_Matrix2.m[1][1] = g_BattleMagicComet_Matrix2.m[2][2];
    g_BattleMagicComet_Matrix2.m[0][0] = g_BattleMagicComet_Matrix2.m[1][1];
    g_BattleMagicComet_Matrix2.t[0] = task->transX;
    g_BattleMagicComet_Matrix2.t[2] = task->transZ;
    MultiplyMatrix(&g_BattleCameraTransform, &g_BattleMagicComet_Matrix2, (void *)matrix);
    SetRotMatrix((void *)matrix);
    SetTransMatrix((void *)matrix);
    (*(int *)&g_BattleMagicComet_ModelReq6) = Gfx_ModelTableGetRenderObject(2, g_BattleMagicComet_TimelineState);
    g_BattleMagicCometDrawListHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleMagicComet_ModelReq6, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleMagicCometDrawListHandle);
    (*(int *)&g_BattleMagicComet_ModelReq6) = Gfx_ModelTableGetRenderObject(3, g_BattleMagicComet_TimelineState);
    (((BattleFxModelDraw *)&g_BattleMagicComet_ModelReq6)->flags) = 0x88;
    g_BattleMagicCometDrawListHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleMagicComet_ModelReq6, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleMagicCometDrawListHandle);
    if (g_BattleIsPaused == 0) {
        (task->frameIndex)++;
    }
}

/*
 * Animates and renders bouncing stone debris models for Comet.
 */
// FUNCTION: FF7 0x005a7928
void __cdecl BattleFx_MagicCometBouncingDebrisTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[60];
        SVECTOR angleX;
        struct { unsigned char pad_to_8[0x8]; PsxMatrix debrisMatrix; };
        struct { unsigned char pad_to_28[0x28]; BattleCometDebris *debris; };
        struct { unsigned char pad_to_2C[0x2C]; VECTOR scale; };
    } frame;
#pragma pack(pop)

    frame.debris = (BattleCometDebris *)BATTLE_CURRENT_TASK_SLOT;
    if (frame.debris->modelVariant == 0) {
        (*(int *)&g_BattleMagicComet_ModelReq7) = Gfx_ModelTableGetRenderObject(4, g_BattleMagicComet_TimelineState);
    } else {
        (*(int *)&g_BattleMagicComet_ModelReq7) = Gfx_ModelTableGetRenderObject(5, g_BattleMagicComet_TimelineState);
    }
    if (frame.debris->state < 2) {
        (((BattleFxModelDraw *)&g_BattleMagicComet_ModelReq7)->flags) = 0x80;
        (((BattleFxModelDraw *)&g_BattleMagicComet_ModelReq7)->fadeLevel) = 0;
    } else {
        (((BattleFxModelDraw *)&g_BattleMagicComet_ModelReq7)->flags) = 0x88;
        (((BattleFxModelDraw *)&g_BattleMagicComet_ModelReq7)->fadeLevel) = ((frame.debris->step << 12) / 8);
        if (frame.debris->modelVariant == 0) {
            (*(int *)&g_BattleMagicComet_ModelReq7) = Gfx_ModelTableGetRenderObject(6, g_BattleMagicComet_TimelineState);
        } else {
            (*(int *)&g_BattleMagicComet_ModelReq7) = Gfx_ModelTableGetRenderObject(7, g_BattleMagicComet_TimelineState);
        }
    }
    frame.angleX.vx = frame.debris->rotX << 4;
    frame.angleX.vy = frame.debris->rotY << 4;
    frame.angleX.vz = frame.debris->rotZ << 4;
    RotMatrix(&frame.angleX, &frame.debrisMatrix);
    frame.debrisMatrix.t[0] = frame.debris->posX;
    frame.debrisMatrix.t[1] = frame.debris->posY;
    frame.debrisMatrix.t[2] = frame.debris->posZ;
    frame.scale.vz = frame.debris->scale;
    frame.scale.vy = frame.scale.vz;
    frame.scale.vx = frame.scale.vy;
    ScaleMatrix(&frame.debrisMatrix, &frame.scale);
    MultiplyMatrix(&g_BattleCameraTransform, &frame.debrisMatrix, &frame.debrisMatrix);
    SetRotMatrix(&frame.debrisMatrix);
    SetTransMatrix(&frame.debrisMatrix);
    SetDrawColorRGB(0, 0, 0);
    g_BattleMagicCometDrawListHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleMagicComet_ModelReq7, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleMagicCometDrawListHandle);
    g_BattleMagicComet_Matrix3.t[0] = frame.debris->posX;
    g_BattleMagicComet_Matrix3.t[1] = 0;
    g_BattleMagicComet_Matrix3.t[2] = frame.debris->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &g_BattleMagicComet_Matrix3, &frame.debrisMatrix);
    SetRotMatrix(&frame.debrisMatrix);
    SetTransMatrix(&frame.debrisMatrix);
    g_BattleMagicCometDrawListHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleMagicComet_ModelReq8, g_BattleCurrentOrderingTable + BATTLE_OT_AFTER_SORTED_2, 1, g_BattleMagicCometDrawListHandle);
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
 * Renders and advances animated impact sprite for Comet.
 */
// FUNCTION: FF7 0x005a7cb1
void __cdecl BattleFx_MagicCometAnimatedSpriteTask(void)
{
    BattleTaskSlot *task;

    task = BATTLE_CURRENT_TASK_SLOT;
    g_BattleMagicComet_SpriteDrawReq.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(8, g_BattleMagicComet_TimelineState);
    g_BattleMagicComet_SpriteDrawReq.drawObj = Gfx_ModelTableGetSpriteObject(8, g_BattleMagicComet_TimelineState);
    g_BattleMagicComet_SpriteDrawReq.frame = task->frameCounter;
    BattleFx_InstallBillboardMatrix((short *)task->data, 0x2800 /* 2.5 */, 0);
    g_BattleMagicCometDrawListHandle = BattleFx_SubmitSpriteDraw(&g_BattleMagicComet_SpriteDrawReq, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleMagicCometDrawListHandle);
    if (g_BattleIsPaused == 0 && g_BattleMagicComet_SpriteDrawReq.spriteGroup != 0) {
        (task->frameCounter)++;
        if (task->frameCounter >= g_BattleMagicComet_SpriteDrawReq.spriteGroup->header->frameCount) {
            task->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Waits for color ramp to complete before terminating Comet effect.
 */
// FUNCTION: FF7 0x005a7d82
void __cdecl BattleFx_MagicCometWaitRampEndTask(void)
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
