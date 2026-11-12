// src/ff7/src/battle/battle_fx_magic_freeze.cpp
/* Magic Effect [47]: Freeze (Japanese: フリーズ, dev tag: "MAGIC/FREEZE") */
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle visual effect for the Freeze magic: a timeline task sets off frost sprites,
//   converging snowflakes and several ice-crystal models that grow, hold, fade or damp out around
//   each target.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/enums/sound.h"
#include "ff7/battle_tasks.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_magic.h"
#include "lib/types/graphics.h"
#include <stdlib.h>
#include "ff7/camera_script.h"
#include <string.h>
#include "ff7/api/battle.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "lib/api/sound.h"
#include "lib/codegen.h"
#include "ff7/types/battle_3d.h"
#include "lib/types/sound.h"
extern "C" {

struct BattleFxModelDraw;

#pragma intrinsic(abs)

#pragma intrinsic(memset)
#pragma intrinsic(memcpy)

// clang-format off
// GLOBAL: FF7 0x008b41f0
struct SoundGroup g_BattleMagicFreezeSoundData = {{452, 453, 0, 0}};
// GLOBAL: FF7 0x008b4200
struct SoundGroup *g_BattleMagicFreezeSoundGroups[2] = {&g_BattleMagicFreezeSoundData, 0};
// GLOBAL: FF7 0x008b4208
unsigned int g_BattleMagicFreezeResourceDefs[45] = {0x00192a39, (unsigned int)&g_BattleMagicFreezeFilenameFreeze, (unsigned int)&g_BattleMagicFreezeFilenameSnow, 0x00000000, 0x00000000, 0x019000b4, (unsigned int)((char *)&g_BattleMagicFreezeFilenameSnow + 0xc), 0x00000000, 0x00000000, 0x00000000, 0x019000b1, (unsigned int)&g_BattleMagicFreezeFilenameIce, 0x00000000, 0x00000000, 0x00000000, 0x019000b4, (unsigned int)((char *)&g_BattleMagicFreezeFilenameIce + 0xc), 0x00000000, 0x00000000, 0x00000000, 0x019000b1, (unsigned int)&g_BattleMagicFreezeFilenameIceb, 0x00000000, 0x00000000, 0x00000000, 0x019000b1, (unsigned int)((char *)&g_BattleMagicFreezeFilenameIceb + 0xc), 0x00000000, 0x00000000, 0x00000000, 0x00192a39, (unsigned int)((char *)&g_BattleMagicFreezeFilenameIceb + 0x1c), (unsigned int)((char *)&g_BattleMagicFreezeFilenameIceb + 0x2c), 0x00000000, 0x00000000, 0x019000b1, (unsigned int)&g_BattleMagicFreezeFilenameRing, 0x00000000, 0x00000000, 0x00000000, 0x019000b1, (unsigned int)((char *)&g_BattleMagicFreezeFilenameRing + 0xc), 0x00000000, 0x00000000, 0x00000000};
// GLOBAL: FF7 0x008b42c0
BattleMagicFreezeSpriteDraw g_BattleMagicFreezeSpriteDrawReq1 = {0, 0, (char)0x80, (char)0x80, (char)0x80, 0x2e, 0};
// GLOBAL: FF7 0x008b42d0
BattleFxModelDrawHead g_BattleMagicFreezeModelDrawReq1 = {0, 0x88, 0, 0, 0x20, 0};
// GLOBAL: FF7 0x008b42e0
PsxMatrix g_BattleMagicFreezeTransformMatrix1 = {{{0x450, 0, 0}, {0, 0x450, 0}, {0, 0, 0x450}}, {0, 0, 0}};
// GLOBAL: FF7 0x008b4300
BattleFxModelDrawHead g_BattleMagicFreezeModelDrawReq2 = {0, 0x88, 0, 0, 0x20, 0};
// GLOBAL: FF7 0x008b4310
PsxMatrix g_BattleMagicFreezeTransformMatrix2 = {{{0x1000, 0, 0}, {0, 0x1000, 0}, {0, 0, 0x1000}}, {0, 0, 0}};
// GLOBAL: FF7 0x008b4330
BattleFxModelDrawHead g_BattleMagicFreezeModelDrawReq3 = {0, 0x88, 0, 0, 0x20, 0};
// GLOBAL: FF7 0x008b4340
BattleFxModelDrawHead g_BattleMagicFreezeModelDrawReq4 = {0, 0x88, 0, 0, 0x20, 0};
// GLOBAL: FF7 0x008b4350
BattleFxModelDrawHead g_BattleMagicFreezeModelDrawReq5 = {0, 0x88, 0, 0, 0x20, 0};
// GLOBAL: FF7 0x008b4360
BattleFxSpriteRequest g_BattleMagicFreezeSpriteDrawReq2 = {0, 0, (char)0x80, (char)0x80, (char)0x80, 0x2e, 0, 0};
// GLOBAL: FF7 0x008b4370
unsigned char g_BattleMagicFreezeFilenameFreeze[13] = "FREEZE02.TIM";
// GLOBAL: FF7 0x008b4380
unsigned char g_BattleMagicFreezeFilenameSnow[22] =
    "SNOW_1.SPT\0\0"
    "ICE_1.RSD";
// GLOBAL: FF7 0x008b4398
unsigned char g_BattleMagicFreezeFilenameIce[24] =
    "ICE_1.RSD\0\0\0"
    "ICEBASE.RSD";
// GLOBAL: FF7 0x008b43b0
unsigned char g_BattleMagicFreezeFilenameIceb[56] =
    "ICEBASE.RSD\0"
    "BREEZE_1.RSD\0\0\0\0"
    "FREEZE02.TIM\0\0\0\0"
    "SMOKE_1.SPT";
// GLOBAL: FF7 0x008b43e8
unsigned char g_BattleMagicFreezeFilenameRing[24] =
    "RING_2.RSD\0\0"
    "LIGHT_1.RSD";
// GLOBAL: FF7 0x00bbe190
int g_BattleMagicFreezeTargetActor = 0;
// GLOBAL: FF7 0x00bbe194
int g_BattleMagicFreezeDrawListHead = 0;
// GLOBAL: FF7 0x00bbe198
struct BattleFxMagicFreezeTargetPos g_BattleMagicFreezeTargetPos = {0};
// GLOBAL: FF7 0x00bbe1a0
struct GfxTableCursor *g_BattleMagicFreezeLoadedResTable = 0;
unsigned char g_bssPad_00bbe1a4[4] = {0};
/* Three model matrices, each a PsxMatrix laid out over three globals: the X scale m[0][0], the
 * Y scale m[1][1], then m[2][2] and the translation (a BattleFxFreezeModelRecord). The task
 * passes the X scale's address to MultiplyMatrix as the matrix. The damped model's Y scale is
 * never written and stays 0. */
// GLOBAL: FF7 0x00bbe1a8
short g_BattleMagicFreezeGrowingScaleX = 0;
unsigned char g_bssPad_00bbe1ac[4] = {0};
// GLOBAL: FF7 0x00bbe1b0
short g_BattleMagicFreezeGrowingScaleY = 0;
unsigned char g_bssPad_00bbe1b4[4] = {0};
// GLOBAL: FF7 0x00bbe1b8
struct BattleFxFreezeModelRecord g_BattleMagicFreezeModelRecord1 = {0};
// GLOBAL: FF7 0x00bbe1c8
short g_BattleMagicFreezeHoldScaleX = 0;
unsigned char g_bssPad_00bbe1cc[4] = {0};
// GLOBAL: FF7 0x00bbe1d0
short g_BattleMagicFreezeHoldScaleY = 0;
unsigned char g_bssPad_00bbe1d4[4] = {0};
// GLOBAL: FF7 0x00bbe1d8
struct BattleFxFreezeModelRecord g_BattleMagicFreezeModelRecord2 = {0};
// GLOBAL: FF7 0x00bbe1e8
short g_BattleMagicFreezeDampedScaleX = 0;
unsigned char g_bssPad_00bbe1ec[4] = {0};
unsigned char g_bssPad_00bbe1f0[8] = {0};
// GLOBAL: FF7 0x00bbe1f8
struct BattleFxFreezeModelRecord g_BattleMagicFreezeModelRecord3 = {0};
// clang-format on


/*
 * Loads resources and registers sound groups for Freeze.
 */
// FUNCTION: FF7 0x0058ce50
void __cdecl BattleFx_MagicLoadResFreeze(void)
{
    GfxResourceRequest reqBuf;
    char pathTemp[200];
    GfxModelTableBank *loadParam;
    int kind;
    GfxResourceRequest *reqPtr;
    int defsBlob;

    if (g_pBattle3dScene != 0 && g_BattleMagicFreezeLoadedResTable == 0) {
        kind = 9;
        defsBlob = (int)&g_BattleMagicFreezeResourceDefs;
        loadParam = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        reqPtr = &reqBuf;
        Gfx_InitResourceRequest(reqPtr);
        reqPtr->groupTag = 0;
        // STRING: FF7 0x008b4400
        reqPtr->resourcePath = BattleFx_BuildResourcePath("MAGIC/FREEZE", pathTemp);
        Gfx_LoadModelTableIfAbsent(kind, defsBlob, reqPtr, &g_BattleMagicFreezeLoadedResTable, loadParam);
        SfxRegisterSoundGroups(g_BattleMagicFreezeSoundGroups);
    }
}

/*
 * Entry point for the Freeze magic effect.
 */
// FUNCTION: FF7 0x0058cef1
void __cdecl BattleFx_MagicEnterFreeze(int targetMask, int actorIdx)
{
    BattleFx_MagicLoadResFreeze();
    BattleFx_MagicRegisterFreezePerTargetCallback(targetMask, actorIdx);
}

/*
 * Registers per-target callback task for Freeze.
 */
// FUNCTION: FF7 0x0058cf0b
void __cdecl BattleFx_MagicRegisterFreezePerTargetCallback(int entity, int bitPattern)
{
    Battle_CreateBitPatternCallbackTask(entity, bitPattern, 0, BattleFx_MagicSpawnFreezePerTarget);
}

/*
 * Spawns timeline task for a single Freeze target.
 */
// FUNCTION: FF7 0x0058cf27
void __cdecl BattleFx_MagicSpawnFreezePerTarget(int actor)
{
    BattleFxFreezeTimeline *task;

    g_BattleMagicFreezeTargetActor = actor;
    task = (BattleFxFreezeTimeline *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicFreezeTimelineTask);
    Battle_GetBoneWorldPos(actor, g_BattleActorRecords[actor].targetBone, (short *)&g_BattleMagicFreezeTargetPos);
    *(Int64Pair *)&task->posX = *(Int64Pair *)&g_BattleMagicFreezeTargetPos;
}

/*
 * Main timeline task controlling ice crystals and frost animation for Freeze.
 */
// FUNCTION: FF7 0x0058cf89
void __cdecl BattleFx_MagicFreezeTimelineTask(void)
{
    int stackWords; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxFreezeTimeline *task;
    BattleFxFreezeTimeline *child;

    task = (BattleFxFreezeTimeline *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (task->stage == 0) {
        Battle_StartFixedPointRamp(0x400, 4);
        g_BattleFxTimelineState.red = 0;
        g_BattleFxTimelineState.channel1 = 0;
        g_BattleFxTimelineState.channel2 = 0x80;
    }
    if (task->stage == 4) {
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_2, Battle_ComputeScreenPanFromPosition((short *)&g_BattleMagicFreezeTargetPos), 0);
        child = (BattleFxFreezeTimeline *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicFreezeSpawnSpriteBurstTask);
    }
    if (task->stage == 0x22) {
        child = (BattleFxFreezeTimeline *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicFreezeFadeModelTask);
        child->posX = g_BattleMagicFreezeTargetPos.posX;
        child->posY = 0xFA88;
        child->posZ = g_BattleMagicFreezeTargetPos.posZ;
    }
    if (task->stage == 0x22) {
        child = (BattleFxFreezeTimeline *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicFreezeScreenScaledModelTask);
        child->posX = g_BattleMagicFreezeTargetPos.posX;
        child->posY = 0xFA88;
        child->posZ = g_BattleMagicFreezeTargetPos.posZ;
    }
    if (task->stage == 0x36) {
        child = (BattleFxFreezeTimeline *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicFreezeSpawnGrowingModelsTask);
    }
    if (task->stage == 0x2C) {
        child = (BattleFxFreezeTimeline *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicFreezeGrowHoldFadeModelTask);
        child->posX = g_BattleMagicFreezeTargetPos.posX;
        child->posY = 0;
        child->posZ = g_BattleMagicFreezeTargetPos.posZ;
    }
    if (task->stage == 0x4F) {
        child = (BattleFxFreezeTimeline *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicFreezeSpawnDampedGrowthModelsTask);
        *(Int64Pair *)&child->posX = *(Int64Pair *)&task->posX;
    }
    if (task->stage == 0x27) {
        child = (BattleFxFreezeTimeline *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicFreezeSpawnAnimatedSpritesTask);
        *(Int64Pair *)&child->posX = *(Int64Pair *)&task->posX;
    }
    if (task->stage == 0x4F) {
        Battle_PostActorHitOrHurtMessage(g_BattleMagicFreezeTargetActor);
    }
    if (task->stage == 0x5E) {
        Battle_StartFixedPointRamp(0, 4);
        task->ownerSlot = BATTLE_TASK_FREE;
        Sfx_ReleaseCachedBuffer(0x1C4, 0xF);
        Sfx_ReleaseCachedBuffer(0x1C5, 0xF);
    }
    (task->stage)++;
}

/*
 * Spawns converging sprite burst task for Freeze.
 */
// FUNCTION: FF7 0x0058d210
void __cdecl BattleFx_MagicFreezeSpawnSpriteBurstTask(void)
{
    BattleFxPosition3s *srcPosPtr;
    int padSlot; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int distance;
    int transBuffer;
    BattleTaskSlot *taskPtr;
    BattleFxSpriteBurstParams *params;
    int i;
    PsxMatrix *matrix;
    BattleMagicFreezeBurstSpriteRecord *pSprite;

    taskPtr = BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    matrix = (PsxMatrix *)Gfx_GetPsxScratchpadPtr(0);
    params = (BattleFxSpriteBurstParams *)(matrix + 1);
    srcPosPtr = (BattleFxPosition3s *)((int)params + 8);
    transBuffer = (int)srcPosPtr + 0x10;
    for (i = 0; i < 3; i++) {
        pSprite = (BattleMagicFreezeBurstSpriteRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicFreezeConvergingSpriteTask);
        pSprite->frameIndex = taskPtr->frameCounter;
        distance = rand() % 0xBB8 + 0x7D0;
        params->startTime = 0;
        params->angle = rand() & 0xFFF;
        params->velocity = -(rand() % 0xA00 - 0x100);
        RotMatrix((SVECTOR *)params, matrix);
        matrix->t[0] = g_BattleMagicFreezeTargetPos.posX;
        matrix->t[1] = g_BattleMagicFreezeTargetPos.posY;
        matrix->t[2] = g_BattleMagicFreezeTargetPos.posZ;
        SetRotMatrix(matrix);
        SetTransMatrix(matrix);
        params->startTime = distance;
        params->angle = 0;
        params->velocity = 0;
        RotTransPoint((void *)params, (int *)srcPosPtr, (void *)transBuffer);
        pSprite->posX = srcPosPtr->posX;
        pSprite->posY = srcPosPtr->posY;
        pSprite->posZ = srcPosPtr->posZ;
        pSprite->burstRandom = rand() % 0xD00 + 0x500;
    }
    (taskPtr->frameCounter)++;
    if (taskPtr->frameCounter >= 0x14) {
        taskPtr->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Updates and renders converging frost sprite for Freeze.
 */
// FUNCTION: FF7 0x0058d3e1
void __cdecl BattleFx_MagicFreezeConvergingSpriteTask(void)
{
    int localFrame;
    BattleFxFreezeSpriteSlot *task;

    task = (BattleFxFreezeSpriteSlot *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleMagicFreezeSpriteDrawReq1.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(0, g_BattleMagicFreezeLoadedResTable);
    g_BattleMagicFreezeSpriteDrawReq1.drawObj = Gfx_ModelTableGetSpriteObject(0, g_BattleMagicFreezeLoadedResTable);
    if (task->timer < 5) {
        g_BattleMagicFreezeSpriteDrawReq1.colorB = (task->timer << 7) / 5;
        g_BattleMagicFreezeSpriteDrawReq1.colorG = g_BattleMagicFreezeSpriteDrawReq1.colorB;
        g_BattleMagicFreezeSpriteDrawReq1.colorR = g_BattleMagicFreezeSpriteDrawReq1.colorG;
    } else {
        g_BattleMagicFreezeSpriteDrawReq1.colorB = 0x80;
        g_BattleMagicFreezeSpriteDrawReq1.colorG = g_BattleMagicFreezeSpriteDrawReq1.colorB;
        g_BattleMagicFreezeSpriteDrawReq1.colorR = g_BattleMagicFreezeSpriteDrawReq1.colorG;
    }
    localFrame = task->timer + task->ownerSlot;
    if (localFrame < 0x19) {
        if (g_BattleIsPaused == 0) {
            task->posY += (short)(SinFixed((task->timer << 0xC) / 0x28) * 0x46 >> 0xC);
        }
    } else {
        localFrame -= 0x19;
        if (localFrame < 5) {
            if (localFrame == 0) {
                task->velocityX = (g_BattleMagicFreezeTargetPos.posX - task->posX) / 5;
                task->velocityY = (g_BattleMagicFreezeTargetPos.posY - task->posY) / 5;
                task->velocityZ = (g_BattleMagicFreezeTargetPos.posZ - task->posZ) / 5;
            }
            if (g_BattleIsPaused == 0) {
                task->posX += task->velocityX;
                task->posY += task->velocityY;
                task->posZ += task->velocityZ;
            }
        } else {
            task->ownerSlot = BATTLE_TASK_FREE;
            return;
        }
    }
    BattleFx_InstallBillboardMatrix(&task->posX, (short)task->scale, 0);
    g_BattleMagicFreezeDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleMagicFreezeSpriteDrawReq1, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleMagicFreezeDrawListHead);
    if (g_BattleIsPaused == 0) {
        (task->timer)++;
    }
}

/*
 * Updates and renders fading ice model for Freeze.
 */
// FUNCTION: FF7 0x0058d605
void __cdecl BattleFx_MagicFreezeFadeModelTask(void)
{
    int localFrame;
    BattleFreezeFadeState *task;
    PsxMatrix *matrix;

    task = (BattleFreezeFadeState *)BATTLE_CURRENT_TASK_SLOT;
    matrix = (PsxMatrix *)Gfx_GetPsxScratchpadPtr(0);
    localFrame = task->frameIndex;
    if (localFrame < 5) {
        g_BattleMagicFreezeModelDrawReq1.flags = 0x88;
        g_BattleMagicFreezeModelDrawReq1.fadeLevel = localFrame * -0x1000 / 5 + 0x1000;
        g_BattleMagicFreezeModelDrawReq1.modelGroup = Gfx_ModelTableGetRenderObject(2, g_BattleMagicFreezeLoadedResTable);
    } else {
        localFrame -= 5;
        if (localFrame < 0x28) {
            g_BattleMagicFreezeModelDrawReq1.flags = 0x80;
            g_BattleMagicFreezeModelDrawReq1.fadeLevel = 0;
            g_BattleMagicFreezeModelDrawReq1.modelGroup = Gfx_ModelTableGetRenderObject(1, g_BattleMagicFreezeLoadedResTable);
        } else {
            localFrame -= 0x28;
            if (localFrame < 0xF) {
                g_BattleMagicFreezeModelDrawReq1.flags = 0x88;
                g_BattleMagicFreezeModelDrawReq1.fadeLevel = (localFrame << 0xC) / 0xF;
                g_BattleMagicFreezeModelDrawReq1.modelGroup = Gfx_ModelTableGetRenderObject(2, g_BattleMagicFreezeLoadedResTable);
            } else {
                task->ownerSlot = BATTLE_TASK_FREE;
                return;
            }
        }
    }
    g_BattleMagicFreezeTransformMatrix1.t[0] = task->transX;
    g_BattleMagicFreezeTransformMatrix1.t[1] = task->transY;
    g_BattleMagicFreezeTransformMatrix1.t[2] = task->transZ;
    MultiplyMatrix(&g_BattleCameraTransform, &g_BattleMagicFreezeTransformMatrix1, matrix);
    SetRotMatrix(matrix);
    SetTransMatrix(matrix);
    SetDrawColorRGB(0, 0, 0);
    g_BattleMagicFreezeDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleMagicFreezeModelDrawReq1, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleMagicFreezeDrawListHead);
    if (g_BattleIsPaused == 0) {
        (task->frameIndex)++;
    }
}

/*
 * Updates and renders screen-scaled ice model for Freeze.
 */
// FUNCTION: FF7 0x0058d7af
void __cdecl BattleFx_MagicFreezeScreenScaledModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[32];
        short screenX;
        struct { unsigned char pad_to_2[0x2]; short screenY; };
        struct { unsigned char pad_to_4[0x4]; int geomOffsetY; };
        struct { unsigned char pad_to_8[0x8]; int geomOffsetX; };
        struct { unsigned char pad_to_C[0xC]; int transFlag; };
        struct { unsigned char pad_to_10[0x10]; BattleTaskSlot *screenSlot; };
        struct { unsigned char pad_to_14[0x14]; int frameStep; };
        struct { unsigned char pad_to_18[0x18]; int scaleValue; };
        struct { unsigned char pad_to_1C[0x1C]; int depth; };
    } frame;
#pragma pack(pop)

    frame.screenSlot = BATTLE_CURRENT_TASK_SLOT;
    g_BattleMagicFreezeModelDrawReq2.modelGroup = Gfx_ModelTableGetRenderObject(8, g_BattleMagicFreezeLoadedResTable);
    frame.frameStep = frame.screenSlot->frameCounter;
    if (frame.frameStep < 4) {
        g_BattleMagicFreezeModelDrawReq2.fadeLevel = 0;
        frame.scaleValue = ((frame.frameStep * 0x1100) / 4) + 0x400;
    } else {
        frame.frameStep -= 4;
        if (frame.frameStep < 4) {
            g_BattleMagicFreezeModelDrawReq2.fadeLevel = (frame.frameStep << 0xC) / 4;
            frame.scaleValue = 0x1500;
        } else {
            frame.screenSlot->ownerSlot = BATTLE_TASK_FREE;
            return;
        }
    }
    SetRotMatrix(&g_BattleCameraTransform);
    SetTransMatrix(&g_BattleCameraTransform);
    RotTransPers((SVECTOR *)frame.screenSlot->data, &frame.screenX, &frame.depth, &frame.transFlag);
    GetGeomOffset(&frame.geomOffsetX, &frame.geomOffsetY);
    g_BattleMagicFreezeTransformMatrix2.t[0] = frame.screenX - frame.geomOffsetX << 5;
    g_BattleMagicFreezeTransformMatrix2.t[1] = frame.screenY - frame.geomOffsetY << 5;
    g_BattleMagicFreezeTransformMatrix2.t[2] = GetGeomScreen() << 5;
    g_BattleMagicFreezeTransformMatrix2.m[2][2] = (short)frame.scaleValue;
    g_BattleMagicFreezeTransformMatrix2.m[1][1] = g_BattleMagicFreezeTransformMatrix2.m[2][2];
    g_BattleMagicFreezeTransformMatrix2.m[0][0] = g_BattleMagicFreezeTransformMatrix2.m[1][1];
    SetRotMatrix(&g_BattleMagicFreezeTransformMatrix2);
    SetTransMatrix(&g_BattleMagicFreezeTransformMatrix2);
    SetDrawColorRGB(0, 0, 0);
    g_BattleMagicFreezeDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleMagicFreezeModelDrawReq2, g_BattleCurrentOrderingTable + BATTLE_OT_AFTER_SORTED_5, 0, g_BattleMagicFreezeDrawListHead);
    if (g_BattleIsPaused == 0) {
        (frame.screenSlot->frameCounter)++;
    }
}

/*
 * Spawns expanding ice crystal models around Freeze target.
 */
// FUNCTION: FF7 0x0058d952
void __cdecl BattleFx_MagicFreezeSpawnGrowingModelsTask(void)
{
    BattleTaskSlot *task;
    BattleMagicFreezeGrowingSlot *child;

    task = BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (task->frameCounter % 5 == 0) {
        child = (BattleMagicFreezeGrowingSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicFreezeGrowingModelTask);
        child->posX = g_BattleMagicFreezeTargetPos.posX;
        child->posY = 0;
        child->posZ = g_BattleMagicFreezeTargetPos.posZ;
    }
    (task->frameCounter)++;
    if (task->frameCounter >= 5) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Updates and renders growing ice crystal model for Freeze.
 */
// FUNCTION: FF7 0x0058d9f6
void __cdecl BattleFx_MagicFreezeGrowingModelTask(void)
{
    int localFrame;
    BattleFxFreezeGrowTask *task;
    PsxMatrix *matrix;

    task = (BattleFxFreezeGrowTask *)BATTLE_CURRENT_TASK_SLOT;
    matrix = (PsxMatrix *)Gfx_GetPsxScratchpadPtr(0);
    g_BattleMagicFreezeModelDrawReq3.modelGroup = Gfx_ModelTableGetRenderObject(5, g_BattleMagicFreezeLoadedResTable);
    localFrame = task->growth;
    if (localFrame < 0xA) {
        g_BattleMagicFreezeModelDrawReq3.fadeLevel = 0;
    } else {
        localFrame -= 0xA;
        if (localFrame < 5) {
            g_BattleMagicFreezeModelDrawReq3.fadeLevel = (localFrame << 0xC) / 5;
        } else {
            task->ownerSlot = BATTLE_TASK_FREE;
            return;
        }
    }
    g_BattleMagicFreezeModelRecord1.scaleZ = task->growth * 0xF00 / 0xF + 0x100;
    g_BattleMagicFreezeGrowingScaleX = g_BattleMagicFreezeModelRecord1.scaleZ;
    g_BattleMagicFreezeGrowingScaleY = task->growth * 0x700 / 0xF + 0x100;
    g_BattleMagicFreezeModelRecord1.posX = task->posX;
    g_BattleMagicFreezeModelRecord1.posY = task->posY;
    g_BattleMagicFreezeModelRecord1.posZ = task->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &g_BattleMagicFreezeGrowingScaleX, matrix);
    SetRotMatrix(matrix);
    SetTransMatrix(matrix);
    SetDrawColorRGB(0, 0, 0);
    g_BattleMagicFreezeDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleMagicFreezeModelDrawReq3, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleMagicFreezeDrawListHead);
    if (g_BattleIsPaused == 0) {
        (task->growth)++;
    }
}

/*
 * Updates and renders expanding, holding, and fading ice model for Freeze.
 */
// FUNCTION: FF7 0x0058db74
void __cdecl BattleFx_MagicFreezeGrowHoldFadeModelTask(void)
{
    int localFrame;
    BattleFxCosmoMemoryExpandingModel *task;
    PsxMatrix *matrix;

    task = (BattleFxCosmoMemoryExpandingModel *)BATTLE_CURRENT_TASK_SLOT;
    matrix = (PsxMatrix *)Gfx_GetPsxScratchpadPtr(0);
    localFrame = task->frame;
    if (localFrame < 5) {
        g_BattleMagicFreezeModelDrawReq4.flags = 0x80;
        g_BattleMagicFreezeModelDrawReq4.fadeLevel = 0;
        g_BattleMagicFreezeModelRecord2.scaleZ = task->frame * 0x2D0 / 5 + 0x80;
        g_BattleMagicFreezeHoldScaleY = g_BattleMagicFreezeModelRecord2.scaleZ;
        g_BattleMagicFreezeHoldScaleX = g_BattleMagicFreezeHoldScaleY;
        g_BattleMagicFreezeModelDrawReq4.modelGroup = Gfx_ModelTableGetRenderObject(3, g_BattleMagicFreezeLoadedResTable);
    } else {
        localFrame -= 5;
        if (localFrame < 0x1E) {
            g_BattleMagicFreezeModelDrawReq4.flags = 0x80;
            g_BattleMagicFreezeModelDrawReq4.fadeLevel = 0;
            g_BattleMagicFreezeModelRecord2.scaleZ = 0x350;
            g_BattleMagicFreezeHoldScaleY = g_BattleMagicFreezeModelRecord2.scaleZ;
            g_BattleMagicFreezeHoldScaleX = g_BattleMagicFreezeHoldScaleY;
            g_BattleMagicFreezeModelDrawReq4.modelGroup = Gfx_ModelTableGetRenderObject(3, g_BattleMagicFreezeLoadedResTable);
        } else {
            localFrame -= 0x1E;
            if (localFrame < 0xF) {
                g_BattleMagicFreezeModelDrawReq4.flags = 0x88;
                g_BattleMagicFreezeModelDrawReq4.fadeLevel = (localFrame << 0xC) / 0xF;
                g_BattleMagicFreezeModelRecord2.scaleZ = 0x350;
                g_BattleMagicFreezeHoldScaleY = g_BattleMagicFreezeModelRecord2.scaleZ;
                g_BattleMagicFreezeHoldScaleX = g_BattleMagicFreezeHoldScaleY;
                g_BattleMagicFreezeModelDrawReq4.modelGroup = Gfx_ModelTableGetRenderObject(4, g_BattleMagicFreezeLoadedResTable);
            } else {
                task->ownerSlot = BATTLE_TASK_FREE;
                return;
            }
        }
    }
    g_BattleMagicFreezeModelRecord2.posX = task->posX;
    g_BattleMagicFreezeModelRecord2.posY = task->posY;
    g_BattleMagicFreezeModelRecord2.posZ = task->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &g_BattleMagicFreezeHoldScaleX, matrix);
    SetRotMatrix(matrix);
    SetTransMatrix(matrix);
    SetDrawColorRGB(0, 0, 0);
    g_BattleMagicFreezeDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleMagicFreezeModelDrawReq4, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleMagicFreezeDrawListHead);
    if (g_BattleIsPaused == 0) {
        (task->frame)++;
    }
}

/*
 * Spawns damped growth ice models around Freeze target.
 */
// FUNCTION: FF7 0x0058dd8b
void __cdecl BattleFx_MagicFreezeSpawnDampedGrowthModelsTask(void)
{
    BattleFreezeGrowthRecord *task;
    BattleMagicFreezeDampedGrowthRecord *child;

    task = (BattleFreezeGrowthRecord *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (task->frameIndex % 3 == 0) {
        child = (BattleMagicFreezeDampedGrowthRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicFreezeDampedGrowthModelTask);
        child->posX = task->posX;
        child->negativeRandom = -(rand() % 0x884 + 0x140);
        child->posZ = task->posZ;
        child->growthRandom = rand() % 0x300 + 0x400;
        child->growthVelocity = rand() % 0x100 + 0x300;
    }
    (task->frameIndex)++;
    if (task->frameIndex >= 9) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Updates and renders damped growth ice model for Freeze.
 */
// FUNCTION: FF7 0x0058de82
void __cdecl BattleFx_MagicFreezeDampedGrowthModelTask(void)
{
    int localFrame;
    BattleFxFreezeDampedGrowthSlot *task;
    PsxMatrix *matrix;

    task = (BattleFxFreezeDampedGrowthSlot *)BATTLE_CURRENT_TASK_SLOT;
    matrix = (PsxMatrix *)Gfx_GetPsxScratchpadPtr(0);
    g_BattleMagicFreezeModelDrawReq5.modelGroup = Gfx_ModelTableGetRenderObject(7, g_BattleMagicFreezeLoadedResTable);
    localFrame = task->frame;
    if (localFrame < 3) {
        g_BattleMagicFreezeModelDrawReq5.fadeLevel = 0;
    } else {
        localFrame -= 3;
        if (localFrame < 0xA) {
            g_BattleMagicFreezeModelDrawReq5.fadeLevel = (localFrame << 0xC) / 0xA;
        } else {
            task->ownerSlot = BATTLE_TASK_FREE;
            return;
        }
    }
    g_BattleMagicFreezeModelRecord3.scaleZ = task->scale;
    g_BattleMagicFreezeDampedScaleX = g_BattleMagicFreezeModelRecord3.scaleZ;
    g_BattleMagicFreezeModelRecord3.posX = task->posX;
    g_BattleMagicFreezeModelRecord3.posY = task->posY;
    g_BattleMagicFreezeModelRecord3.posZ = task->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &g_BattleMagicFreezeDampedScaleX, matrix);
    SetRotMatrix(matrix);
    SetTransMatrix(matrix);
    SetDrawColorRGB(0, 0, 0);
    g_BattleMagicFreezeDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleMagicFreezeModelDrawReq5, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleMagicFreezeDrawListHead);
    if (g_BattleIsPaused == 0) {
        task->scale += task->growthVelocity;
        task->growthVelocity = task->growthVelocity * 0x950 >> 0xC;
        (task->frame)++;
    }
}

/*
 * Spawns animated snowflake sprites around Freeze target.
 */
// FUNCTION: FF7 0x0058dffa
void __cdecl BattleFx_MagicFreezeSpawnAnimatedSpritesTask(void)
{
    int angle;
    int rad;
    BattleFxFreezeSpriteRecord *pTaskSlot;
    int i;
    BattleFxFreezeAnimatedSprite *pSprite;

    pTaskSlot = (BattleFxFreezeSpriteRecord *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    for (i = 0; i < 3; i++) {
        pSprite = (BattleFxFreezeAnimatedSprite *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicFreezeAnimatedSpriteTask);
        angle = rand() & 0xFFF;
        rad = rand() % 0x1F4 + 0x3E8;
        pSprite->posX = pTaskSlot->centerX + (SinFixed(angle) * rad >> 0xC);
        pSprite->posY = -(rand() % 0x9C4 + 0x1F4);
        pSprite->posZ = pTaskSlot->centerZ + (CosFixed(angle) * rad >> 0xC);
        pSprite->velocityX = 0;
        pSprite->velocityY = 0x32;
        pSprite->velocityZ = 0;
        pSprite->scale = rand() % 0x2800 + 0x1000;
    }
    for (i = 0; i < 3; i++) {
        pSprite = (BattleFxFreezeAnimatedSprite *)BATTLE_NEW_TASK_SLOT(BattleFx_MagicFreezeAnimatedSpriteTask);
        angle = rand() & 0xFFF;
        rad = rand() % 0x1F4 + 0x3E8;
        pSprite->posX = pTaskSlot->centerX + (SinFixed(angle) * rad >> 0xC);
        pSprite->posY = 0;
        pSprite->posZ = pTaskSlot->centerZ + (CosFixed(angle) * rad >> 0xC);
        pSprite->velocityX = SinFixed(angle) * 0x82 >> 0xC;
        pSprite->velocityY = 0;
        pSprite->velocityZ = CosFixed(angle) * 0x82 >> 0xC;
        pSprite->scale = rand() % 0x2800 + 0x1000;
    }
    (pTaskSlot->frame)++;
    if (pTaskSlot->frame >= 0x28) {
        pTaskSlot->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Updates physics and renders animated snowflake sprite for Freeze.
 */
// FUNCTION: FF7 0x0058e24f
void __cdecl BattleFx_MagicFreezeAnimatedSpriteTask(void)
{
    BattleFxFreezeAnimatedSprite *task;

    task = (BattleFxFreezeAnimatedSprite *)BATTLE_CURRENT_TASK_SLOT;
    g_BattleMagicFreezeSpriteDrawReq2.spriteGroup = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(6, g_BattleMagicFreezeLoadedResTable);
    g_BattleMagicFreezeSpriteDrawReq2.drawObj = Gfx_ModelTableGetSpriteObject(6, g_BattleMagicFreezeLoadedResTable);
    BattleFx_InstallBillboardMatrix(&task->posX, (short)task->scale, 0);
    g_BattleMagicFreezeSpriteDrawReq2.frame = task->frame;
    g_BattleMagicFreezeDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleMagicFreezeSpriteDrawReq2, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleMagicFreezeDrawListHead);
    if (g_BattleIsPaused == 0) {
        (task->frame)++;
        task->posX += task->velocityX;
        task->posY += task->velocityY;
        task->posZ += task->velocityZ;
        if (g_BattleMagicFreezeSpriteDrawReq2.spriteGroup != 0 && task->frame >= g_BattleMagicFreezeSpriteDrawReq2.spriteGroup->header->frameCount) {
            task->slotTag = 0xFFFF;
        }
    }
}
} /* extern "C" */
