// src/ff7/src/battle/battle_fx_blue_pandoras_box_effect.cpp
// Enemy Skill [23]: Pandora's Box (Japanese: パンドラの箱, dev tag: "blue/nanntoka" / Nanntoka)
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: The Nanntoka blue magic effect: a shattering sequence of textured fragments, spinning,
//   tumbling and growing models, and a particle field, using offscreen render requests.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/enums/sound.h"
#include "ff7/api/battle.h"
#include "lib/codegen.h"
#include "ff7/battle_tasks.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
#include "ff7/types/battle.h"
#include "lib/types/graphics.h"
#include "ff7/types/battle_fx.h"
#include "ff7/camera_script.h"
#include <string.h>
#include <stdlib.h>
#include "lib/api/sound.h"
#include "ff7/enums/battle.h"
#include "ff7/enums/main.h"
/* Opened here, ahead of the globals that ff7/nanntoka_render.h (included mid-file) reads. */
extern "C" {

struct BattleFxPandorasBoxFieldState;
struct BattleFxShatterState;
struct GfxPsxShortVertexSlot;
struct GfxPsxSpriteQuadState;
struct BattleFxModelDraw;
#pragma intrinsic(memset)
#pragma intrinsic(memcpy)

// clang-format off
unsigned char g_rawData_008a8d28[16] = {0x27, 0x01, 0x00, 0x00, 0x28, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x008a8d38
int g_BattleBluePandorasBoxSoundGroup[2] = {9080104, 0};
// GLOBAL: FF7 0x008a8d40
int g_BattleBluePandorasBoxResourceDef = 0x004B00B6;
char *g_BattleFxBluePandorasBoxEffectResourceNames[1] = {
    // STRING: FF7 0x008a8e24
    "hatena.rsd"
};
unsigned int g_BattleBluePandorasBoxResourceDefDataTail[48] = {
    0x00000000,
    0x00000000,
    0x00000000,
    0x006400b6,
    // STRING: FF7 0x008a8e30
    (unsigned int)"inseki_1.rsd",
    0x00000000,
    0x00000000,
    0x00000000,
    0x006400b6,
    // STRING: FF7 0x008a8e40
    (unsigned int)"inseki_2.rsd",
    0x00000000,
    0x00000000,
    0x00000000,
    0x006400b6,
    // STRING: FF7 0x008a8e50
    (unsigned int)"inseki_3.rsd",
    0x00000000,
    0x00000000,
    0x00000000,
    0x001900b1,
    // STRING: FF7 0x008a8e60
    (unsigned int)"light.rsd",
    0x00000000,
    0x00000000,
    0x00000000,
    0x03e800b6,
    // STRING: FF7 0x008a8e6c
    (unsigned int)"ster.rsd",
    0x00000000,
    0x00000000,
    0x00000000,
    0x78888000,
    0x002d807f,
    0x78888080,
    0x002d80ff,
    0x78880000,
    0x002e007f,
    0x78880080,
    0x002e00ff,
    0x78888000,
    0x002e807f,
    0x78888080,
    0x002e80ff,
    0x78880000,
    0x002f007f,
    0x78880080,
    0x002f00ff,
    0x78888000,
    0x002f807f,
    0x78888080,
    0x002f80ff
};
// GLOBAL: FF7 0x008a8e08
int g_BattleBluePandorasBoxOffscreenSlot1 = (-1);
// GLOBAL: FF7 0x008a8e0c
int g_BattleBluePandorasBoxOffscreenSlot2 = (-1);
// GLOBAL: FF7 0x008a8e10
int g_BattleBluePandorasBoxOffscreenSlot3 = (-1);
// GLOBAL: FF7 0x008a8e14
int g_BattleBluePandorasBoxOffscreenSlot4 = (-1);
// The model table entry each tumbling model uses, by its model index.
// GLOBAL: FF7 0x008a8e18
int g_BattleBluePandorasBoxModelIndices[3] = {1, 2, 3};
// GLOBAL: FF7 0x00b74fe8
struct SVECTOR g_BattleBluePandorasBoxRenderRecordA = {0};
unsigned char g_bssPad_00b74ff0[110] = {0};
// GLOBAL: FF7 0x00b75060
struct BattleFxShatterVelocity g_BattleBluePandorasBoxRenderRecordB = {0};
#define g_BattleBluePandorasBoxRenderRecordsA (&g_BattleBluePandorasBoxRenderRecordA)
#define g_BattleBluePandorasBoxRenderRecordsB (&g_BattleBluePandorasBoxRenderRecordB)
unsigned char g_bssPad_00b75068[110] = {0};
// GLOBAL: FF7 0x00b750d8
int g_BattleBluePandorasBoxStateFlag = 0;
unsigned char g_bssPad_00b750dc[4] = {0};
// GLOBAL: FF7 0x00b750e0
struct BattleBluePandorasBoxFragmentRecords g_BattleBluePandorasBoxFragmentRecords = {0};
// GLOBAL: FF7 0x00b77020
unsigned char g_BattleBluePandorasBoxTransformWorking[16] = {0};
// The texture words of the 40 by 21 shatter grid, in the cells' 0x18-byte stride (see
// BattleFxShatterCellUv); the cells' own shorts start in g_BattleBluePandorasBoxTransformWorking.
// The span runs to the next named definition of the TU.
// GLOBAL: FF7 0x00b77030
unsigned char g_BattleBluePandorasBoxShatterCellUvs[20144] = {0};

// GLOBAL: FF7 0x00b7bee0
int g_BattleBluePandorasBoxTargetMask = 0;
// GLOBAL: FF7 0x00b7bee4
int g_BattleBluePandorasBoxTimelineFlag = 0;
// GLOBAL: FF7 0x00b7bee8
struct GfxTableCursor *g_BattleBluePandorasBoxLoadedRes = 0;
// GLOBAL: FF7 0x00b7beec
int g_BattleBluePandorasBoxParticleCounter = 0;
// clang-format on


// The names the shared nanntoka_render.h spells for the particle pool's first record.
#define g_BattleBluePandorasBoxFragmentAge (g_BattleBluePandorasBoxFragmentRecords.fragments[0].age)

#define g_BattleBluePandorasBoxFragmentPosY (g_BattleBluePandorasBoxFragmentRecords.fragments[0].posY)

#define g_BattleBluePandorasBoxFragmentPosZ (g_BattleBluePandorasBoxFragmentRecords.fragments[0].posZ)

#define g_BattleBluePandorasBoxFragmentVelocityY (g_BattleBluePandorasBoxFragmentRecords.fragments[0].velocityY)

#define g_BattleBluePandorasBoxFragmentVelocityZ (g_BattleBluePandorasBoxFragmentRecords.fragments[0].velocityZ)

#define g_BattleBluePandorasBoxFragmentStep (g_BattleBluePandorasBoxFragmentRecords.fragments[0].step)

#define g_BattleBluePandorasBoxFragmentColor (g_BattleBluePandorasBoxFragmentRecords.fragments[0].colorR)

/*
 * Loads resources and sound groups for the Nanntoka blue magic effect.
 */
// FUNCTION: FF7 0x00566740
void __cdecl BattleFx_BlueLoadResPandorasBox(void)
{
    GfxModelTableBank *sceneLinkRef;
    GfxResourceRequest requestItem;
    int kind;
    char pathText[200];
    GfxResourceRequest *pGfxReq;
    int defRecPtr;

    if (g_pBattle3dScene != 0 && g_BattleBluePandorasBoxLoadedRes == 0) {
        kind = 6;
        defRecPtr = (int)&g_BattleBluePandorasBoxResourceDef;
        sceneLinkRef = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pGfxReq = &requestItem;
        Gfx_InitResourceRequest(pGfxReq);
        pGfxReq->groupTag = 0;
        // STRING: FF7 0x008a8e78
        pGfxReq->resourcePath = BattleFx_BuildResourcePath("blue/nanntoka", pathText);
        Gfx_LoadModelTableIfAbsent(kind, defRecPtr, pGfxReq, &g_BattleBluePandorasBoxLoadedRes, sceneLinkRef);
        SfxRegisterSoundGroups((struct SoundGroup **)&g_BattleBluePandorasBoxSoundGroup);
    }
}

/*
 * Entry point for the Nanntoka blue magic effect.
 */
// FUNCTION: FF7 0x005667e1
void __cdecl BattleFx_BlueEnterPandorasBox(int targetMask, int actorIdx)
{
    memset((int *)&g_BattleBluePandorasBoxFragmentRecords, 0, 0x1F40);
    BattleFx_BlueBeginPandorasBox(targetMask, actorIdx);
}

/*
 * Initializes and begins the Nanntoka blue magic effect.
 */
// FUNCTION: FF7 0x00566806
void __cdecl BattleFx_BlueBeginPandorasBox(int targetMask, int actorIdx)
{
    *(int *)&g_BattleFxDrawListBufferTaskSlotPtr = (int)BATTLE_NEW_TASK_SLOT(BattleFx_SelectDrawListBuffer);
    g_BattleFxActiveDrawListPtr = Gfx_GetPsxPacketScratchPtr();
    BattleFx_BlueLoadResPandorasBox();
    Battle_AllocateResourceSlot(BattleFx_BluePandorasBoxSetupTask);
    g_BattleBluePandorasBoxTargetMask = targetMask;
    g_BattleBluePandorasBoxStateFlag = 1;
    g_BattleBluePandorasBoxTimelineFlag = g_BattleBluePandorasBoxStateFlag;
    Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_2, 0x40, 0);
}

/*
 * Setup task initializing shatter fragments and offscreen resources for Nanntoka.
 */
// FUNCTION: FF7 0x00566871
void __cdecl BattleFx_BluePandorasBoxSetupTask(void)
{
    BattleFxPandorasBoxSelfRecord *task;

    task = (BattleFxPandorasBoxSelfRecord *)BATTLE_CURRENT_TASK_SLOT;
    task->ownerSlot = BATTLE_TASK_FREE;
    task = (BattleFxPandorasBoxSelfRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_BluePandorasBoxTimelineTask);
    task->state = 0;
    task = (BattleFxPandorasBoxSelfRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_BluePandorasBoxParticleFieldTask);
    BattleFx_BluePandorasBoxInitShatterFragments();
    BattleFx_BluePandorasBoxClearRecordPool();
}

/*
 * Initializes individual shatter fragment data records.
 */
// FUNCTION: FF7 0x005668de
void __cdecl BattleFx_BluePandorasBoxInitShatterFragments(void)
{
    int gridY, halfWidth, xIdx, screenCy, projDist, tileRow, blockX;

    blockX = 0;
    projDist = GetGeomScreen() << 2;
    GetGeomOffset(&halfWidth, &screenCy);
    halfWidth = 160;
    screenCy = 120;
    for (gridY = 0; gridY < 21; gridY++) {
        for (xIdx = 0; xIdx < 40; xIdx++) {
            ((BattleFxShatterCellRow *)&g_BattleBluePandorasBoxTransformWorking)[gridY][xIdx].x = xIdx * 8 + 4 - halfWidth;
            ((BattleFxShatterCellRow *)&g_BattleBluePandorasBoxTransformWorking)[gridY][xIdx].y = gridY * 8 + 4 - screenCy;
            ((BattleFxShatterCellRow *)&g_BattleBluePandorasBoxTransformWorking)[gridY][xIdx].z = projDist;
            ((BattleFxShatterCellRow *)&g_BattleBluePandorasBoxTransformWorking)[gridY][xIdx].rotIndex = rand() % 0xF;
            tileRow = rand();
            ((BattleFxShatterCellRow *)&g_BattleBluePandorasBoxTransformWorking)[gridY][xIdx].velocityX = xIdx * 2 - 0x28 - (xIdx - 0x14 >> 1);
            ((BattleFxShatterCellRow *)&g_BattleBluePandorasBoxTransformWorking)[gridY][xIdx].velocityY = gridY * 2 - 0x30 - (tileRow & 3);
            tileRow = tileRow % 5;
            ((BattleFxShatterCellRow *)&g_BattleBluePandorasBoxTransformWorking)[gridY][xIdx].velocityZ = -((32 - gridY << 1) * tileRow);
            ((BattleFxShatterCellUvRow *)&g_BattleBluePandorasBoxShatterCellUvs)[gridY][xIdx].tileWord = gridY << 0xB | xIdx << 3;
            ((BattleFxShatterCellUvRow *)&g_BattleBluePandorasBoxShatterCellUvs)[gridY][xIdx].tpageWord = ((BattleFxShatterCellUvRow *)&g_BattleBluePandorasBoxShatterCellUvs)[gridY][xIdx].tileWord + 8 | blockX;
        }
    }
    gridY = 0;
    for (tileRow = 0; gridY < 7; gridY++, tileRow++) {
        xIdx = 30;
        for (blockX = 0; xIdx < 40; xIdx++, blockX++) {
            ((BattleFxShatterCellUvRow *)&g_BattleBluePandorasBoxShatterCellUvs)[gridY][xIdx].tileWord = tileRow << 0xB | blockX << 3;
            ((BattleFxShatterCellUvRow *)&g_BattleBluePandorasBoxShatterCellUvs)[gridY][xIdx].tpageWord &= 0xFFFF0000;
            ((BattleFxShatterCellUvRow *)&g_BattleBluePandorasBoxShatterCellUvs)[gridY][xIdx].tpageWord |= ((BattleFxShatterCellUvRow *)&g_BattleBluePandorasBoxShatterCellUvs)[gridY][xIdx].tileWord + 8;
        }
    }
    gridY = 7;
    for (tileRow = 0; gridY < 14; gridY++, tileRow++) {
        xIdx = 30;
        for (blockX = 0; xIdx < 40; xIdx++, blockX++) {
            ((BattleFxShatterCellUvRow *)&g_BattleBluePandorasBoxShatterCellUvs)[gridY][xIdx].tileWord = tileRow << 0xB | blockX << 3;
            ((BattleFxShatterCellUvRow *)&g_BattleBluePandorasBoxShatterCellUvs)[gridY][xIdx].tpageWord &= 0xFFFF0000;
            ((BattleFxShatterCellUvRow *)&g_BattleBluePandorasBoxShatterCellUvs)[gridY][xIdx].tpageWord |= ((BattleFxShatterCellUvRow *)&g_BattleBluePandorasBoxShatterCellUvs)[gridY][xIdx].tileWord + 8;
        }
    }
    gridY = 14;
    for (tileRow = 0; gridY < 21; gridY++, tileRow++) {
        xIdx = 30;
        for (blockX = 0; xIdx < 40; xIdx++, blockX++) {
            ((BattleFxShatterCellUvRow *)&g_BattleBluePandorasBoxShatterCellUvs)[gridY][xIdx].tileWord = tileRow << 0xB | blockX << 3;
            ((BattleFxShatterCellUvRow *)&g_BattleBluePandorasBoxShatterCellUvs)[gridY][xIdx].tpageWord &= 0xFFFF0000;
            ((BattleFxShatterCellUvRow *)&g_BattleBluePandorasBoxShatterCellUvs)[gridY][xIdx].tpageWord |= ((BattleFxShatterCellUvRow *)&g_BattleBluePandorasBoxShatterCellUvs)[gridY][xIdx].tileWord + 8;
        }
    }
    for (gridY = 0; gridY < 15; gridY++) {
        g_BattleBluePandorasBoxRenderRecordsA[gridY].vz = 0;
        g_BattleBluePandorasBoxRenderRecordsA[gridY].vy = 0;
        g_BattleBluePandorasBoxRenderRecordsA[gridY].vx = 0;
        xIdx = gridY * 48 + 72;
        tileRow = xIdx << 1;
        g_BattleBluePandorasBoxRenderRecordsB[gridY].velX = rand() % tileRow - xIdx;
        g_BattleBluePandorasBoxRenderRecordsB[gridY].velY = rand() % tileRow - xIdx;
        g_BattleBluePandorasBoxRenderRecordsB[gridY].velZ = rand() % tileRow - xIdx;
    }
}

/*
 * Timeline task coordinating the Nanntoka effect phases and offscreen releases.
 */
// FUNCTION: FF7 0x00566e61
void __cdecl BattleFx_BluePandorasBoxTimelineTask(void)
{
    BattleFxBluePandorasBoxSlot *modelTaskPtr;
    BattleFxPandorasBoxChildRecord *currChild;
    int width, dstColor, sourceX, sourceY, xRemaining, posY, dstXB, quadFlags, spriteHeight, actorIdx;
    BattleFxPandorasBoxSelfRecord *taskPtr;

    taskPtr = (BattleFxPandorasBoxSelfRecord *)BATTLE_CURRENT_TASK_SLOT;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (DEAD_IN_RETAIL != 0) {
        xRemaining = g_BattleBluePandorasBoxParticleCounter % 512;
        dstXB = (xRemaining + 0x100) % 512;
        g_BattleBluePandorasBoxParticleCounter++;
        posY = 0;
        width = 256;
        spriteHeight = 256;
        sourceX = 0;
        sourceY = 0;
        quadFlags = 0;
        dstColor = 0xFFFFFFFF;
        if (Battle3d_GetOffscreenRenderObject(g_BattleBluePandorasBoxOffscreenSlot3) != 0) {
            Gfx_EmitPsxTexturedSpriteQuad(xRemaining, posY, width, spriteHeight, sourceX, sourceY, quadFlags, FLOAT_ONE_BITS, dstColor, 0, (GfxPsxSpriteQuadState *)Battle3d_GetOffscreenRenderObject(g_BattleBluePandorasBoxOffscreenSlot3));
        }
        if (Battle3d_GetOffscreenRenderObject(g_BattleBluePandorasBoxOffscreenSlot3) != 0) {
            Gfx_EmitPsxTexturedSpriteQuad(dstXB, posY, width, spriteHeight, sourceX, sourceY, quadFlags, FLOAT_ONE_BITS, dstColor, 0, (GfxPsxSpriteQuadState *)Battle3d_GetOffscreenRenderObject(g_BattleBluePandorasBoxOffscreenSlot3));
        }
    }
    switch (taskPtr->state) {
    case BLUE_NANNTOKA_PHASE_GROW_MODEL:
        currChild = (BattleFxPandorasBoxChildRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_BluePandorasBoxGrowShrinkModelTask);
        currChild->scaleFraction = 0;
        currChild->scaleStep = 0xA3;
        (taskPtr->state)++;
        break;
    case BLUE_NANNTOKA_PHASE_SPIN_MODELS:
        if (taskPtr->frame == 23) {
            g_BattleStageHideFlags |= BATTLE_STAGE_HIDE_OTHER_PARTS | BATTLE_STAGE_HIDE_PART_1_GROUP | BATTLE_STAGE_HIDE_PARTS_2_TO_4;
            for (actorIdx = 0; actorIdx < 10; actorIdx++) {
                g_BattleActorRecords[actorIdx].drawFlags |= 6;
            }
        }
        if (taskPtr->frame == 24) {
            currChild = (BattleFxPandorasBoxChildRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_BluePandorasBoxScaledSpinModelTask);
            currChild->posX = 0;
            currChild->posY = 0xFB8C;
            currChild->posZ = 19400;
            currChild->rotZ = 0x100 /* 22.5 deg */;
            currChild->segmentCount = 4;
            currChild->rotY = 0;
            currChild->rotYStep = 6;
            currChild->scaleFraction = 0xE66;
            currChild = (BattleFxPandorasBoxChildRecord *)BATTLE_NEW_TASK_SLOT(BattleFx_BluePandorasBoxScaledSpinModelTask);
            currChild->posX = 0;
            currChild->posY = 0xFBA0;
            currChild->posZ = 19100;
            currChild->rotZ = 0;
            currChild->segmentCount = 3;
            currChild->rotY = 0;
            currChild->rotYStep = 4;
            currChild->scaleFraction = 0xD99;
            (taskPtr->state)++;
        }
        break;
    case BLUE_NANNTOKA_PHASE_TUMBLING_MODELS:
        if (taskPtr->frame == 34) {
            modelTaskPtr = (BattleFxBluePandorasBoxSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_BluePandorasBoxTumblingModelTask);
            modelTaskPtr->offsetX = 0xFAEC;
            modelTaskPtr->offsetY = 0x64;
            modelTaskPtr->radius = 2000;
            modelTaskPtr->speed = 63;
            modelTaskPtr->drift = 0xFFF5 /* -11 */;
            modelTaskPtr->velocityY = 0xFFDE;
            modelTaskPtr->gravity = 0;
            modelTaskPtr->lifetime = 32;
            modelTaskPtr->tumbleAngle = 0;
            modelTaskPtr->delay = 24;
            modelTaskPtr->scale = 0x200 /* 0.125 */;
            modelTaskPtr->phase = 0;
        }
        if (taskPtr->frame == 35) {
            modelTaskPtr = (BattleFxBluePandorasBoxSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_BluePandorasBoxTumblingModelTask);
            modelTaskPtr->offsetX = 0xFAEC;
            modelTaskPtr->offsetY = 0xFF9C;
            modelTaskPtr->radius = 2200;
            modelTaskPtr->speed = 52;
            modelTaskPtr->drift = 3;
            modelTaskPtr->velocityY = 0xFFC0;
            modelTaskPtr->gravity = 0;
            modelTaskPtr->lifetime = 20;
            modelTaskPtr->tumbleAngle = 0;
            modelTaskPtr->delay = 38;
            modelTaskPtr->scale = 0x1F4;
            modelTaskPtr->phase = 1;
        }
        if (taskPtr->frame == 37) {
            modelTaskPtr = (BattleFxBluePandorasBoxSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_BluePandorasBoxTumblingModelTask);
            modelTaskPtr->offsetX = 0x514;
            modelTaskPtr->offsetY = 0x50;
            modelTaskPtr->radius = 1800;
            modelTaskPtr->speed = 0xFFD9;
            modelTaskPtr->drift = 0xFFFC /* -4 */;
            modelTaskPtr->velocityY = 0xFFEA;
            modelTaskPtr->gravity = 0;
            modelTaskPtr->lifetime = 28;
            modelTaskPtr->tumbleAngle = 0;
            modelTaskPtr->delay = 36;
            modelTaskPtr->scale = 0x199;
            modelTaskPtr->phase = 2;
            (taskPtr->state)++;
        }
        break;
    case BLUE_NANNTOKA_PHASE_DRIFT_MODEL:
        if (taskPtr->frame == 90) {
            modelTaskPtr = (BattleFxBluePandorasBoxSlot *)BATTLE_NEW_TASK_SLOT(BattleFx_BluePandorasBoxSpinDriftModelTask);
            modelTaskPtr->offsetX = 0xFF38;
            modelTaskPtr->offsetY = 0xFFD8;
            modelTaskPtr->radius = 512;
            modelTaskPtr->gravity = 0xFC00;
            modelTaskPtr->lifetime = 17;
            modelTaskPtr->scale = 0xB60;
            (taskPtr->state)++;
        }
        break;
    case BLUE_NANNTOKA_PHASE_RESTORE_STAGE:
        if (taskPtr->frame == 225) {
            g_BattleStageHideFlags &= 0xFF & ~(BATTLE_STAGE_HIDE_OTHER_PARTS | BATTLE_STAGE_HIDE_PART_1_GROUP | BATTLE_STAGE_HIDE_PARTS_2_TO_4);
            for (actorIdx = 0; actorIdx < 10; actorIdx++) {
                g_BattleActorRecords[actorIdx].drawFlags &= 0xF9;
            }
            (taskPtr->state)++;
        }
    }
    if (taskPtr->frame == 250) {
        for (actorIdx = 0; actorIdx < 10; actorIdx++) {
            if ((g_BattleBluePandorasBoxTargetMask & 1 << actorIdx) != 0) {
                Battle_PostActorHitOrHurtMessage(actorIdx);
            }
        }
    }
    if (taskPtr->frame >= 255) {
        taskPtr->ownerSlot = BATTLE_TASK_FREE;
        if (g_BattleBluePandorasBoxOffscreenSlot1 >= 0) {
            Battle3d_ReleaseOffscreenSlot(g_BattleBluePandorasBoxOffscreenSlot1);
            g_BattleBluePandorasBoxOffscreenSlot1 = 0xFFFFFFFF;
        }
        if (g_BattleBluePandorasBoxOffscreenSlot2 >= 0) {
            Battle3d_ReleaseOffscreenSlot(g_BattleBluePandorasBoxOffscreenSlot2);
            g_BattleBluePandorasBoxOffscreenSlot2 = 0xFFFFFFFF;
        }
        if (g_BattleBluePandorasBoxOffscreenSlot3 >= 0) {
            Battle3d_ReleaseOffscreenSlot(g_BattleBluePandorasBoxOffscreenSlot3);
            g_BattleBluePandorasBoxOffscreenSlot3 = 0xFFFFFFFF;
        }
        if (g_BattleBluePandorasBoxOffscreenSlot4 >= 0) {
            Battle3d_ReleaseOffscreenSlot(g_BattleBluePandorasBoxOffscreenSlot4);
            g_BattleBluePandorasBoxOffscreenSlot4 = 0xFFFFFFFF;
        }
        Sfx_ReleaseCachedBuffer(0x127, 0xF);
        Sfx_ReleaseCachedBuffer(0x128, 0xF);
    }
    (taskPtr->frame)++;
}

/*
 * Task animating spinning and drifting 3D models for Nanntoka.
 */
// FUNCTION: FF7 0x00567538
void __cdecl BattleFx_BluePandorasBoxSpinDriftModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[68];
        SVECTOR rotX;
        struct { unsigned char pad_to_C[0xC]; BattleFxPandorasBoxSpinDriftModelSlot *driftSlot; };
        struct { unsigned char pad_to_10[0x10]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_30[0x30]; BattleFxModelDraw *modelDraw; };
        struct { unsigned char pad_to_34[0x34]; VECTOR scale; };
    } frame;
#pragma pack(pop)

    frame.driftSlot = (BattleFxPandorasBoxSpinDriftModelSlot *)BATTLE_CURRENT_TASK_SLOT;
    frame.rotX.vx = 0x400 /* 90 deg */;
    frame.rotX.vy = frame.driftSlot->angle;
    frame.rotX.vz = 0;
    RotMatrix(&frame.rotX, &frame.rotMatrix);
    frame.rotMatrix.t[0] = frame.driftSlot->velocityX;
    frame.rotMatrix.t[1] = frame.driftSlot->velocityY;
    frame.rotMatrix.t[2] = frame.driftSlot->velocityZ;
    frame.scale.vz = frame.driftSlot->scale;
    frame.scale.vy = frame.scale.vz;
    frame.scale.vx = frame.scale.vy;
    ScaleMatrix(&frame.rotMatrix, &frame.scale);
    SetRotMatrix(&frame.rotMatrix);
    SetTransMatrix(&frame.rotMatrix);
    frame.modelDraw = (BattleFxModelDraw *)Gfx_GetPsxScratchpadPtr(0);
    frame.modelDraw->modelGroup = Gfx_ModelTableGetRenderObject(0, g_BattleBluePandorasBoxLoadedRes);
    frame.modelDraw->flags = 0x80;
    frame.modelDraw->uvScroll = 0;
    frame.modelDraw->fadeLevel = 0;
    frame.modelDraw->stateFlags = 0;
    frame.modelDraw->texturePageBits = 0;
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitModelDraw((BattleFxModelDraw *)frame.modelDraw, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (frame.driftSlot->frameCounter >= 135) {
        frame.driftSlot->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.driftSlot->velocityX < 0) {
        frame.driftSlot->velocityX += 2;
    }
    frame.driftSlot->velocityY -= 3;
    frame.driftSlot->velocityZ += 80;
    if (frame.driftSlot->frameCounter < 60) {
        frame.driftSlot->angle += frame.driftSlot->spinStep;
    }
    (frame.driftSlot->frameCounter)++;
}

/*
 * Task animating tumbling 3D models for Nanntoka.
 */
// FUNCTION: FF7 0x005676e3
void __cdecl BattleFx_BluePandorasBoxTumblingModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[64];
        SVECTOR angle;
        struct { unsigned char pad_to_8[0x8]; BattleFxPandorasBoxTumblingModelSlot *tumbleSlot; };
        struct { unsigned char pad_to_C[0xC]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_2C[0x2C]; BattleFxModelDraw *modelDraw; };
        struct { unsigned char pad_to_30[0x30]; VECTOR scale; };
    } frame;
#pragma pack(pop)

    frame.tumbleSlot = (BattleFxPandorasBoxTumblingModelSlot *)BATTLE_CURRENT_TASK_SLOT;
    frame.angle.vx = frame.tumbleSlot->angle;
    frame.angle.vy = frame.tumbleSlot->tumbleAngle;
    frame.angle.vz = 0;
    RotMatrixXYZ(&frame.angle, &frame.rotMatrix);
    frame.rotMatrix.t[0] = frame.tumbleSlot->posX;
    frame.rotMatrix.t[1] = frame.tumbleSlot->posY;
    frame.rotMatrix.t[2] = frame.tumbleSlot->posZ;
    frame.scale.vz = frame.tumbleSlot->scale;
    frame.scale.vy = frame.scale.vz;
    frame.scale.vx = frame.scale.vy;
    ScaleMatrix(&frame.rotMatrix, &frame.scale);
    SetRotMatrix(&frame.rotMatrix);
    SetTransMatrix(&frame.rotMatrix);
    frame.modelDraw = (BattleFxModelDraw *)Gfx_GetPsxScratchpadPtr(0);
    frame.modelDraw->modelGroup = Gfx_ModelTableGetRenderObject(g_BattleBluePandorasBoxModelIndices[frame.tumbleSlot->modelIndex], g_BattleBluePandorasBoxLoadedRes);
    frame.modelDraw->flags = 0;
    frame.modelDraw->uvScroll = 0;
    frame.modelDraw->fadeLevel = 128;
    frame.modelDraw->stateFlags = 0;
    frame.modelDraw->texturePageBits = 0;
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitModelDraw((BattleFxModelDraw *)frame.modelDraw, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (frame.tumbleSlot->frameCounter >= 65) {
        frame.tumbleSlot->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.tumbleSlot->posX += frame.tumbleSlot->velocityX;
    frame.tumbleSlot->posY += frame.tumbleSlot->velocityY;
    frame.tumbleSlot->posZ += frame.tumbleSlot->velocityZ;
    frame.tumbleSlot->angle += frame.tumbleSlot->angleStep;
    frame.tumbleSlot->tumbleAngle += frame.tumbleSlot->tumbleAngleStep;
    (frame.tumbleSlot->frameCounter)++;
}

/*
 * Task animating scaled spinning 3D models for Nanntoka.
 */
// FUNCTION: FF7 0x005678a5
void __cdecl BattleFx_BluePandorasBoxScaledSpinModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[68];
        SVECTOR rotX;
        struct { unsigned char pad_to_8[0x8]; BattleFxPandorasBoxScaledSpinModelSlot *scaledSlot; };
        struct { unsigned char pad_to_C[0xC]; int shatterSlot; };
        struct { unsigned char pad_to_10[0x10]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_30[0x30]; BattleFxModelDraw *modelDraw; };
        struct { unsigned char pad_to_34[0x34]; VECTOR scale; };
    } frame;
#pragma pack(pop)

    frame.scaledSlot = (BattleFxPandorasBoxScaledSpinModelSlot *)BATTLE_CURRENT_TASK_SLOT;
    frame.rotX.vx = 0;
    frame.rotX.vy = frame.scaledSlot->growth;
    frame.rotX.vz = frame.scaledSlot->angle;
    RotMatrixXYZ(&frame.rotX, &frame.rotMatrix);
    frame.rotMatrix.t[0] = frame.scaledSlot->posX;
    frame.rotMatrix.t[1] = frame.scaledSlot->posY;
    frame.rotMatrix.t[2] = frame.scaledSlot->posZ;
    frame.scale.vz = frame.scaledSlot->scale;
    frame.scale.vy = frame.scale.vz;
    frame.scale.vx = frame.scale.vy;
    ScaleMatrix(&frame.rotMatrix, &frame.scale);
    SetRotMatrix(&frame.rotMatrix);
    SetTransMatrix(&frame.rotMatrix);
    frame.modelDraw = (BattleFxModelDraw *)Gfx_GetPsxScratchpadPtr(0);
    frame.modelDraw->modelGroup = Gfx_ModelTableGetRenderObject(5, g_BattleBluePandorasBoxLoadedRes);
    frame.modelDraw->flags = 8;
    frame.modelDraw->uvScroll = 0;
    frame.modelDraw->fadeLevel = 48;
    frame.modelDraw->stateFlags = 0x20;
    frame.modelDraw->texturePageBits = 0;
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitModelDraw((BattleFxModelDraw *)frame.modelDraw, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (frame.scaledSlot->frameCounter >= 201) {
        frame.scaledSlot->ownerSlot = BATTLE_TASK_FREE;
        if (g_BattleBluePandorasBoxStateFlag != 0) {
            frame.shatterSlot = (int)BATTLE_NEW_TASK_SLOT(BattleFx_BluePandorasBoxShatterFragmentsDrawTask);
            g_BattleBluePandorasBoxStateFlag = 0;
            BattleFx_BluePandorasBoxQueueOffscreenRequests(6);
        }
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.scaledSlot->angle += frame.scaledSlot->spinStep;
    frame.scaledSlot->growth += frame.scaledSlot->growthStep;
    (frame.scaledSlot->frameCounter)++;
}

/*
 * Queues offscreen render requests for the shatter fragment targets.
 */
// FUNCTION: FF7 0x00567a57
void __cdecl BattleFx_BluePandorasBoxQueueOffscreenRequests(int count)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[40];
        BattleOffscreenRequest *request;
        struct { unsigned char pad_to_4[0x4]; short rectX4; };
        struct { unsigned char pad_to_6[0x6]; short rectY4; };
        struct { unsigned char pad_to_8[0x8]; short rectW4; };
        struct { unsigned char pad_to_A[0xA]; short rectH4; };
        struct { unsigned char pad_to_C[0xC]; short rectX1; };
        struct { unsigned char pad_to_E[0xE]; short rectY1; };
        struct { unsigned char pad_to_10[0x10]; short rectW1; };
        struct { unsigned char pad_to_12[0x12]; short rectH1; };
        struct { unsigned char pad_to_14[0x14]; short rectX3; };
        struct { unsigned char pad_to_16[0x16]; short rectY3; };
        struct { unsigned char pad_to_18[0x18]; short rectW3; };
        struct { unsigned char pad_to_1A[0x1A]; short rectH3; };
        struct { unsigned char pad_to_1C[0x1C]; short rectX2; };
        struct { unsigned char pad_to_1E[0x1E]; short rectY2; };
        struct { unsigned char pad_to_20[0x20]; short rectW2; };
        struct { unsigned char pad_to_22[0x22]; short rectH2; };
        struct { unsigned char pad_to_24[0x24]; int kind; };
    } frame;
#pragma pack(pop)

    frame.rectX1 = 0;
    frame.rectY1 = 0;
    frame.rectW1 = 240;
    frame.rectH1 = 168;
    frame.rectX2 = 480;
    frame.rectY2 = 0;
    frame.rectW2 = 80;
    frame.rectH2 = 56;
    frame.rectX3 = 480;
    frame.rectY3 = 112;
    frame.rectW3 = 80;
    frame.rectH3 = 56;
    frame.rectX4 = 480;
    frame.rectY4 = 224;
    frame.rectW4 = 80;
    frame.rectH4 = 56;
    frame.kind = 1;
    switch (g_BattleUiScalingMode) {
    case MAIN_GRAPHICS_MODE_320X240:
        frame.rectX1 = 0;
        frame.rectY1 = 0;
        frame.rectX2 = 240;
        frame.rectY2 = 0;
        frame.rectX3 = 240;
        frame.rectY3 = 56;
        frame.rectX4 = 240;
        frame.rectY4 = 112;
        frame.kind = 1;
        break;
    case MAIN_GRAPHICS_MODE_320X240_CENTERED:
        frame.rectX1 = 160;
        frame.rectY1 = 120;
        frame.rectX2 = 400;
        frame.rectY2 = 120;
        frame.rectX3 = 400;
        frame.rectY3 = 176;
        frame.rectX4 = 400;
        frame.rectY4 = 232;
        frame.kind = 1;
        break;
    case MAIN_GRAPHICS_MODE_640X480:
        frame.rectX1 = 0;
        frame.rectY1 = 0;
        frame.rectX2 = 480;
        frame.rectY2 = 0;
        frame.rectX3 = 480;
        frame.rectY3 = 112;
        frame.rectX4 = 480;
        frame.rectY4 = 224;
        frame.kind = 2;
    }
    if (g_pBattle3dScene != 0) {
        frame.request = (BattleOffscreenRequest *)Battle3d_AcquireOffscreenRequest();
        if (frame.request != 0) {
            frame.request->drawKind = 0;
            frame.request->colorKeyEnabled = 0;
            frame.request->renderKind = 0xC;
            frame.request->mode = 1;
            frame.request->mode = 4;
            frame.request->viewportX = frame.rectX1;
            frame.request->viewportY = frame.rectY1;
            frame.request->viewportW = frame.rectW1;
            frame.request->viewportH = frame.rectH1;
            frame.request->sampleStepX = frame.kind;
            frame.request->sampleStepY = frame.kind;
            g_BattleBluePandorasBoxOffscreenSlot1 = frame.request->offscreenSlot;
        }
        frame.request = (BattleOffscreenRequest *)Battle3d_AcquireOffscreenRequest();
        if (frame.request != 0) {
            frame.request->drawKind = 0;
            frame.request->colorKeyEnabled = 0;
            frame.request->renderKind = 0xC;
            frame.request->mode = 1;
            frame.request->mode = 4;
            frame.request->viewportX = frame.rectX2;
            frame.request->viewportY = frame.rectY2;
            frame.request->viewportW = frame.rectW2;
            frame.request->viewportH = frame.rectH2;
            frame.request->sampleStepX = frame.kind;
            frame.request->sampleStepY = frame.kind;
            g_BattleBluePandorasBoxOffscreenSlot2 = frame.request->offscreenSlot;
        }
        frame.request = (BattleOffscreenRequest *)Battle3d_AcquireOffscreenRequest();
        if (frame.request != 0) {
            frame.request->drawKind = 0;
            frame.request->colorKeyEnabled = 0;
            frame.request->renderKind = 0xC;
            frame.request->mode = 1;
            frame.request->mode = 4;
            frame.request->viewportX = frame.rectX3;
            frame.request->viewportY = frame.rectY3;
            frame.request->viewportW = frame.rectW3;
            frame.request->viewportH = frame.rectH3;
            frame.request->sampleStepX = frame.kind;
            frame.request->sampleStepY = frame.kind;
            g_BattleBluePandorasBoxOffscreenSlot3 = frame.request->offscreenSlot;
        }
        frame.request = (BattleOffscreenRequest *)Battle3d_AcquireOffscreenRequest();
        if (frame.request != 0) {
            frame.request->drawKind = 0;
            frame.request->colorKeyEnabled = 0;
            frame.request->renderKind = 0xC;
            frame.request->mode = 1;
            frame.request->mode = 4;
            frame.request->viewportX = frame.rectX4;
            frame.request->viewportY = frame.rectY4;
            frame.request->viewportW = frame.rectW4;
            frame.request->viewportH = frame.rectH4;
            frame.request->sampleStepX = frame.kind;
            frame.request->sampleStepY = frame.kind;
            g_BattleBluePandorasBoxOffscreenSlot4 = frame.request->offscreenSlot;
        }
    }
}

/*
 * Task driving the rendering of shatter fragment quads.
 */
// FUNCTION: FF7 0x00567e44
void __cdecl BattleFx_BluePandorasBoxShatterFragmentsDrawTask(void)
{
    BattleFxShatterState *shatterState;
    BattleTaskSlot *task;

    task = BATTLE_CURRENT_TASK_SLOT;
    shatterState = (BattleFxShatterState *)Gfx_GetPsxScratchpadPtr(0);
    BattleFx_BluePandorasBoxDrawShatterFragments(shatterState);
    if (task->frameCounter >= 30) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    (task->frameCounter)++;
}

/*
 * Renders individual textured shatter fragment quads from offscreen textures.
 */
// FUNCTION: FF7 0x00567ead
void __cdecl BattleFx_BluePandorasBoxDrawShatterFragments(BattleFxShatterState *drawState)
{
    int group;
    BattleFxShatterFragmentPrim *prim;
    int decayVal;
    int cellIndex;

    drawState->vertex0X = 0xFFF0;
    drawState->vertex0Y = 0xFFF0;
    drawState->vertex0Z = 0;
    drawState->vertex1X = 16;
    drawState->vertex1Y = 0xFFF0;
    drawState->vertex1Z = 0;
    drawState->vertex2X = 0xFFF0;
    drawState->vertex2Y = 16;
    drawState->vertex2Z = 0;
    drawState->vertex3X = 16;
    drawState->vertex3Y = 16;
    drawState->vertex3Z = 0;
    for (group = 0; group < 0xF; group++) {
        RotMatrixXYZ(&g_BattleBluePandorasBoxRenderRecordsA[group], &drawState->rotations[group]);
        if (g_BattleIsPaused == 0) {
            g_BattleBluePandorasBoxRenderRecordsA[group].vx += g_BattleBluePandorasBoxRenderRecordsB[group].velX;
            g_BattleBluePandorasBoxRenderRecordsA[group].vy += g_BattleBluePandorasBoxRenderRecordsB[group].velY;
            g_BattleBluePandorasBoxRenderRecordsA[group].vz += g_BattleBluePandorasBoxRenderRecordsB[group].velZ;
        }
    }
    prim = (BattleFxShatterFragmentPrim *)Gfx_GetPsxPacketScratchPtr();
    for (group = 0; group < 0x15; group++) {
        for (cellIndex = 0; cellIndex < 40; cellIndex++) {
            drawState->matrix.t[0] = ((BattleFxShatterCellRow *)&g_BattleBluePandorasBoxTransformWorking)[group][cellIndex].x << 2;
            drawState->matrix.t[1] = ((BattleFxShatterCellRow *)&g_BattleBluePandorasBoxTransformWorking)[group][cellIndex].y << 2;
            drawState->matrix.t[2] = ((BattleFxShatterCellRow *)&g_BattleBluePandorasBoxTransformWorking)[group][cellIndex].z;
            SetRotMatrix(&drawState->rotations[((BattleFxShatterCellRow *)&g_BattleBluePandorasBoxTransformWorking)[group][cellIndex].rotIndex]);
            SetTransMatrix((char *)&drawState->matrix);
            RefreshGlobalTransformState();
            StoreDeferredDrawArgs(&drawState->vertex0X, &drawState->vertex1X, &drawState->vertex2X);
            ApplyDeferredDrawArgs();
            if (g_BattleIsPaused == 0) {
                decayVal = ((BattleFxShatterCellRow *)&g_BattleBluePandorasBoxTransformWorking)[group][cellIndex].velocityZ;
                ((BattleFxShatterCellRow *)&g_BattleBluePandorasBoxTransformWorking)[group][cellIndex].x += ((BattleFxShatterCellRow *)&g_BattleBluePandorasBoxTransformWorking)[group][cellIndex].velocityX;
                ((BattleFxShatterCellRow *)&g_BattleBluePandorasBoxTransformWorking)[group][cellIndex].y += ((BattleFxShatterCellRow *)&g_BattleBluePandorasBoxTransformWorking)[group][cellIndex].velocityY;
                ((BattleFxShatterCellRow *)&g_BattleBluePandorasBoxTransformWorking)[group][cellIndex].z += decayVal;
                ((BattleFxShatterCellRow *)&g_BattleBluePandorasBoxTransformWorking)[group][cellIndex].velocityY += 5;
                decayVal -= decayVal >> 4;
                ((BattleFxShatterCellRow *)&g_BattleBluePandorasBoxTransformWorking)[group][cellIndex].velocityZ = decayVal;
            }
            Gfx_PsxStoreClearFlag((int *)&drawState->flags);
            if ((drawState->flags & 0x60000) != 0) {
                continue;
            }
            SetPendingResourceHandle((GfxPsxShortVertexSlot *)&drawState->vertex3X);
            ApplyPendingResourceHandle();
            decayVal = ((BattleFxShatterCellUvRow *)&g_BattleBluePandorasBoxShatterCellUvs)[group][cellIndex].tileWord;
            prim->color = 0x808080;
            prim->decayVal = decayVal;
            prim->uvTpageWord = ((BattleFxShatterCellUvRow *)&g_BattleBluePandorasBoxShatterCellUvs)[group][cellIndex].tpageWord;
            prim->uvWord2 = decayVal + 0x800;
            prim->uvWord3 = decayVal + 0x808;
            prim->tag = prim->tag & 0xFFFFFF | 0x9000000;
            prim->gpuCode = 0x2C;
            if (cellIndex < 30) {
                Gfx_SetPsxPrimQuadState(0xB, 4, (GfxPsxSpriteQuadState *)Battle3d_GetOffscreenRenderObject(g_BattleBluePandorasBoxOffscreenSlot1));
            } else if (group < 7) {
                Gfx_SetPsxPrimQuadState(0xB, 4, (GfxPsxSpriteQuadState *)Battle3d_GetOffscreenRenderObject(g_BattleBluePandorasBoxOffscreenSlot2));
            } else if (group < 0xE) {
                Gfx_SetPsxPrimQuadState(0xB, 4, (GfxPsxSpriteQuadState *)Battle3d_GetOffscreenRenderObject(g_BattleBluePandorasBoxOffscreenSlot3));
            } else {
                Gfx_SetPsxPrimQuadState(0xB, 4, (GfxPsxSpriteQuadState *)Battle3d_GetOffscreenRenderObject(g_BattleBluePandorasBoxOffscreenSlot4));
            }
            Gfx_DrawPsxPrimitive(prim);
        }
    }
    g_BattleFxActiveDrawListPtr = (int)prim;
}

/*
 * Task animating growing and shrinking 3D models for Nanntoka.
 */
// FUNCTION: FF7 0x00568371
void __cdecl BattleFx_BluePandorasBoxGrowShrinkModelTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[64];
        SVECTOR rotX;
        struct { unsigned char pad_to_8[0x8]; BattleFxPandorasBoxGrowShrinkModelSlot *growSlot; };
        struct { unsigned char pad_to_C[0xC]; PsxMatrix rotMatrix; };
        struct { unsigned char pad_to_2C[0x2C]; BattleFxModelDraw *modelDraw; };
        struct { unsigned char pad_to_30[0x30]; VECTOR scaleX; };
    } frame;
#pragma pack(pop)

    frame.growSlot = (BattleFxPandorasBoxGrowShrinkModelSlot *)BATTLE_CURRENT_TASK_SLOT;
    frame.rotX.vx = 0;
    frame.rotX.vy = 0;
    frame.rotX.vz = 0;
    RotMatrixXYZ(&frame.rotX, &frame.rotMatrix);
    frame.rotMatrix.t[0] = 0xFFFFFF56;
    frame.rotMatrix.t[1] = 0xFFFFFF7E;
    frame.rotMatrix.t[2] = 0x200 /* 45 deg */;
    frame.scaleX.vz = frame.growSlot->scale;
    frame.scaleX.vy = frame.scaleX.vz;
    frame.scaleX.vx = frame.scaleX.vy;
    ScaleMatrix(&frame.rotMatrix, &frame.scaleX);
    SetRotMatrix(&frame.rotMatrix);
    SetTransMatrix(&frame.rotMatrix);
    frame.modelDraw = (BattleFxModelDraw *)Gfx_GetPsxScratchpadPtr(0);
    frame.modelDraw->modelGroup = Gfx_ModelTableGetRenderObject(4, g_BattleBluePandorasBoxLoadedRes);
    frame.modelDraw->flags = 0x88;
    frame.modelDraw->uvScroll = 0;
    frame.modelDraw->fadeLevel = 0;
    frame.modelDraw->stateFlags = 0;
    frame.modelDraw->texturePageBits = 0;
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitModelDraw((BattleFxModelDraw *)frame.modelDraw, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (frame.growSlot->frameCounter >= 50) {
        frame.growSlot->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.growSlot->frameCounter < 20) {
        frame.growSlot->scale += frame.growSlot->scaleStep;
    }
    if (frame.growSlot->frameCounter == 30) {
        frame.growSlot->scaleStep = frame.growSlot->scale / 20;
    }
    if (frame.growSlot->frameCounter > 30) {
        frame.growSlot->scale -= frame.growSlot->scaleStep;
    }
    (frame.growSlot->frameCounter)++;
}

/*
 * Task driving the particle field animation and rendering for Nanntoka.
 */
// FUNCTION: FF7 0x0056850f
void __cdecl BattleFx_BluePandorasBoxParticleFieldTask(void)
{
    int spawnCount;
    int frameParam;
    BattleFxPandorasBoxFieldState *pBlock;
    int i;
    int emptyIndex;
    int searchIdx;
    int radius;
    BattleTaskSlot *pFieldTask;
    int fullRange;

    pFieldTask = BATTLE_CURRENT_TASK_SLOT;
    pBlock = (BattleFxPandorasBoxFieldState *)Gfx_GetPsxScratchpadPtr(0);
    BattleFx_BluePandorasBoxRenderRecordPool(pBlock, pFieldTask->frameCounter);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (pFieldTask->frameCounter >= 30 && pFieldTask->frameCounter <= 215) {
        if (pFieldTask->frameCounter < 100) {
            spawnCount = 7;
            frameParam = pFieldTask->frameCounter;
        } else if (pFieldTask->frameCounter < 140) {
            spawnCount = 10;
            frameParam = pFieldTask->frameCounter << 2;
        } else if (pFieldTask->frameCounter < 160) {
            spawnCount = 14;
            frameParam = pFieldTask->frameCounter << 3;
        } else {
            spawnCount = 18;
            frameParam = (pFieldTask->frameCounter << 4) + pFieldTask->frameCounter;
        }
        for (i = 0; i < spawnCount; i++) {
            searchIdx = 0;
            for (emptyIndex = 0xFFFFFFFF; searchIdx < 300; searchIdx++) {
                if (g_BattleBluePandorasBoxFragmentRecords.fragments[searchIdx].age == 0) {
                    emptyIndex = searchIdx;
                    break;
                }
            }
            if (emptyIndex < 0) {
                break;
            }
            g_BattleBluePandorasBoxFragmentRecords.fragments[emptyIndex].posX = 0;
            g_BattleBluePandorasBoxFragmentRecords.fragments[emptyIndex].posY = 0xFBB4;
            g_BattleBluePandorasBoxFragmentRecords.fragments[emptyIndex].posZ = 20000;
            g_BattleBluePandorasBoxFragmentRecords.fragments[emptyIndex].age = 1;
            radius = 2730;
            fullRange = radius << 1;
            pBlock->velocityX = rand() % fullRange - radius;
            pBlock->velocityY = rand() % fullRange - radius;
            pBlock->velocityZ = -((rand()) % 4096);
            NormalizeVectorFixed(&pBlock->velocityX, &pBlock->velocityX);
            radius = pFieldTask->frameCounter + (pFieldTask->frameCounter >> 1) + rand() % 170 + 100;
            g_BattleBluePandorasBoxFragmentRecords.fragments[emptyIndex].velocityX = radius * pBlock->velocityX >> 12;
            g_BattleBluePandorasBoxFragmentRecords.fragments[emptyIndex].velocityY = radius * pBlock->velocityY >> 12;
            g_BattleBluePandorasBoxFragmentRecords.fragments[emptyIndex].velocityZ = radius * pBlock->velocityZ >> 12;
            g_BattleBluePandorasBoxFragmentRecords.fragments[emptyIndex].step = frameParam;
            g_BattleBluePandorasBoxFragmentRecords.fragments[emptyIndex].colorR = (pBlock->velocityX & 0x3F) + 0x48;
            g_BattleBluePandorasBoxFragmentRecords.fragments[emptyIndex].colorG = (pBlock->velocityY & 0x3F) + 0x4A;
            g_BattleBluePandorasBoxFragmentRecords.fragments[emptyIndex].colorB = (pBlock->velocityZ & 0x3F) + 0x44;
        }
    }
    if (pFieldTask->frameCounter >= 225) {
        pFieldTask->ownerSlot = BATTLE_TASK_FREE;
    }
    (pFieldTask->frameCounter)++;
}

#include "ff7/nanntoka_render.h"

/*
 * Renders active particles from the Nanntoka particle record pool.
 */
// FUNCTION: FF7 0x00568827
void __cdecl BattleFx_BluePandorasBoxRenderRecordPool(BattleFxPandorasBoxFieldState *record, int pool)
{
    Vector16 position;
    int prim;
    int renderCount;
    int count;
    int unused_stack_slot; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */

    record->matrix.m[2][2] = 0x1000;
    record->matrix.m[1][1] = 0x1000;
    record->matrix.m[0][0] = 0x1000;
    record->matrix.m[2][1] = 0;
    record->matrix.m[2][0] = 0;
    record->matrix.m[1][2] = 0;
    record->matrix.m[1][0] = 0;
    record->matrix.m[0][2] = 0;
    record->matrix.m[0][1] = 0;
    record->matrix.t[2] = 0;
    record->matrix.t[1] = 0;
    record->matrix.t[0] = 0;
    record->originX = 0;
    record->originY = 0xFBB4;
    record->originZ = 20000;
    SetRotMatrix(&record->matrix);
    SetTransMatrix(&record->matrix);
    RefreshGlobalTransformState();
    prim = Gfx_GetPsxPacketScratchPtr();
    NanntokaRenderPool((int)record, pool, &count, prim, &renderCount, &position);
    g_BattleFxActiveDrawListPtr = prim;
}

/*
 * Clears and initializes the Nanntoka particle record pool.
 */
// FUNCTION: FF7 0x00568cfc
void __cdecl BattleFx_BluePandorasBoxClearRecordPool(void)
{
    int i;

    for (i = 0; i < 400; i++) {
        g_BattleBluePandorasBoxFragmentRecords.fragments[i].age = 0;
    }
}
} /* extern "C" */
