// src/ff7/src/battle/battle_fx_magic_limit_flash.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle effect for the Limit flash: a flash on the actor built from grow, shrink and
//   sine-scaled models, radial flare particles and a periodic pulse, plus a three-frame screen
//   flash.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
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
#include "ff7/api/menu.h"
#include "ff7/enums/battle.h"
extern "C" {

struct BattleFxModelDraw;

#pragma intrinsic(abs)

// clang-format off
// GLOBAL: FF7 0x008fe2b8
int g_BattleLimitFlash_ResourceDescriptor = 0x001900B1;
// The MAGIC/LIMIT effect resource descriptors, four 0x14-byte records from g_BattleLimitFlash_ResourceDescriptor (flags, file name,
// ...) that BattleFx_MagicEnterLimitFlash passes to Gfx_LoadModelTableIfAbsent: the names are
// "JO_P01.RSD".."JO_P03.RSD" and "SUMMON.SPT" in the raw fill g_BattleFxMagicLimitFlashResourceNames (+0x4, +0x10, +0x1c, +0x28).
// The name words point into that fill, not at the retail addresses.
char *g_BattleLimitFlash_ResourceDescriptorFirstPath[1] = {(char *)g_BattleFxMagicLimitFlashResourceNames + 0x4};
// The rest of the descriptor list, then the Limit flash's model draw record (BattleFxModelDraw's first 0x10 bytes).
typedef struct {
    int descriptors[18];
    int drawHandle;
    int drawFlags;
    short drawUvScroll;
    short drawFadeLevel;
    short drawStateFlags;
    short drawTexturePageBits;
} BattleLimitFlashModelTable;
// GLOBAL: FF7 0x008fe2c0
BattleLimitFlashModelTable g_BattleLimitFlash_ModelTable = {
    {0, 0, 0, 0x1900b1, (int)((char *)g_BattleFxMagicLimitFlashResourceNames + 0x10), 0,
     0, 0, 0x1900b1, (int)((char *)g_BattleFxMagicLimitFlashResourceNames + 0x1c), 0, 0,
     0, 0x12a39, 0, (int)((char *)g_BattleFxMagicLimitFlashResourceNames + 0x28), 0, 0},
    0, 0x88, 0, 0, 0x20, 0};
// GLOBAL: FF7 0x008fe318
short g_BattleLimitFlash_ModelScales1[4] = {16384, 0, 0, 0};
// GLOBAL: FF7 0x008fe320
short g_BattleLimitFlash_ModelScale0 = 16384;
// GLOBAL: FF7 0x008fe328
BattleLimitFlash_SpriteData g_BattleLimitFlash_SpriteData = {
    0x4000, 0, 0, 0, {0, 0}, 0, 0, {0x80, 0x80, 0x80, 0x2e, 0x00, 0x80}, 0
};
// GLOBAL: FF7 0x008fe348
short g_BattleLimitFlash_SpriteDimensions[2] = {128, 448};
unsigned char g_rawData_008fe34c[4] = {0x40, 0x02, 0x40, 0x03};
unsigned char g_BattleFxMagicLimitFlashResourceNames[52] = {
    0x80, 0x03, 0x00, 0x00, 0x4a, 0x4f, 0x5f, 0x50, 0x30, 0x31, 0x2e, 0x52, 0x53, 0x44, 0x00, 0x00, 0x4a, 0x4f, 0x5f, 0x50, 0x30, 0x32, 0x2e, 0x52,
    0x53, 0x44, 0x00, 0x00, 0x4a, 0x4f, 0x5f, 0x50, 0x30, 0x33, 0x2e, 0x52, 0x53, 0x44, 0x00, 0x00, 0x53, 0x55, 0x4d, 0x4d, 0x4f, 0x4e, 0x2e, 0x53,
    0x50, 0x54, 0x00, 0x00
};
// clang-format on
#pragma intrinsic(memset)
#pragma intrinsic(memcpy)


/*
 * Lazily creates the Limit flash effect resource and descriptors.
 */
// FUNCTION: FF7 0x005c01a0
void __cdecl BattleFx_MagicEnterLimitFlash(void)
{
    GfxModelTableBank *rendererField;
    GfxResourceRequest requestRecord;
    int typeId;
    char pathBuffer[200];
    GfxResourceRequest *request;
    int resDescList;

    if (g_pBattle3dScene != 0 && g_pBattleLimitFlashTable == 0) {
        typeId = 4;
        resDescList = (int)&g_BattleLimitFlash_ResourceDescriptor;
        rendererField = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        request = &requestRecord;
        Gfx_InitResourceRequest(request);
        request->groupTag = 1;
        // STRING: FF7 0x008fe384
        request->resourcePath = BattleFx_BuildResourcePath("MAGIC/LIMIT", pathBuffer);
        Gfx_LoadModelTableIfAbsent(typeId, resDescList, request, &g_pBattleLimitFlashTable, rendererField);
    }
}

/*
 * Spawns limit flash model and emitter tasks for an actor.
 */
// FUNCTION: FF7 0x005c0230
void __cdecl BattleFx_MagicLimitFlashSpawnForActor(int actorId, int mode)
{
    BattleLimitFlashEmitter *sprite;
    BattleLimitFlashEmitter *record;

    BattleFx_MagicEnterLimitFlash();
    record = (BattleLimitFlashEmitter *)(&g_BattleTaskSlotBuffer.secondarySlots[(short)Battle_AllocateSecondaryResourceSlot(BattleFx_MagicLimitFlashPulseEmitterTask)]);
    Battle_GetBoneWorldPos(actorId, g_BattleActorRecords[actorId].targetBone, &record->bonePos);
    switch (mode) {
    case BATTLE_LIMIT_FLASH_SINE_SCALE:
        record->updateTask = BattleFx_MagicLimitFlashModel1SineScaleTask;
        break;
    case BATTLE_LIMIT_FLASH_GROW:
        record->updateTask = BattleFx_MagicLimitFlashModel0GrowTask;
        break;
    case BATTLE_LIMIT_FLASH_SHRINK:
        record->updateTask = BattleFx_MagicLimitFlashModel2ShrinkTask;
        break;
    case BATTLE_LIMIT_FLASH_SINE_SCALE_WITH_SPRITES:
        record->updateTask = BattleFx_MagicLimitFlashModel1SineScaleTask;
        sprite = (BattleLimitFlashEmitter *)&g_BattleTaskSlotBuffer.secondarySlots[(short)Battle_AllocateSecondaryResourceSlot(BattleFx_MagicLimitFlashRadialSpriteEmitterTask)];
        *(Int64Pair *)&sprite->bonePos = *(Int64Pair *)&record->bonePos;
        break;
    }
}

/*
 * Updates sine-scaled limit flash model animation and renders passes.
 */
// FUNCTION: FF7 0x005c0300
void __cdecl BattleFx_MagicLimitFlashModel1SineScaleTask(void)
{
    BattleFxLimitFlashSineTask *task;

    task = (BattleFxLimitFlashSineTask *)&g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx];
    if ((*(unsigned char *)&g_BattleLimitFlashModel1Started & 1) == 0) {
        g_BattleLimitFlashModel1Started |= 1;
        g_BattleLimitFlashModel1Data = Gfx_GetPsxScratchpadPtr(0);
    }
    g_BattleLimitFlashModel1Scale = SinFixed(task->step << 7) + 0x1000;
    g_BattleLimitFlashModel1MatrixTail.matrixM22 = task->step << 10;
    g_BattleLimitFlashModel1Matrix = g_BattleLimitFlashModel1MatrixTail.matrixM22;
    if (task->step < 8) {
        g_BattleLimitFlash_ModelTable.drawFadeLevel = 0;
    } else {
        g_BattleLimitFlash_ModelTable.drawFadeLevel = (task->step - 8 << 12) / 8;
    }
    g_BattleLimitFlashModel1MatrixTail.transX = task->posX;
    g_BattleLimitFlashModel1MatrixTail.transY = 0;
    g_BattleLimitFlashModel1MatrixTail.transZ = task->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, &g_BattleLimitFlashModel1Matrix, (void *)g_BattleLimitFlashModel1Data);
    g_BattleLimitFlash_ModelTable.drawHandle = Gfx_ModelTableGetRenderObject(1, g_pBattleLimitFlashTable);
    BattleFx_MagicLimitFlashDrawBlackModelPasses(g_BattleLimitFlashModel1Data);
    if (g_BattleIsPaused == 0 && (task->step)++ == 0xF) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Renders model passes with black tint and alternating render flags.
 */
// FUNCTION: FF7 0x005c0447
void __cdecl BattleFx_MagicLimitFlashDrawBlackModelPasses(int transform)
{
    SetDrawColorRGB(0, 0, 0);
    SetRotMatrix((void *)transform);
    SetTransMatrix((void *)transform);
    g_BattleLimitFlash_ModelTable.drawFlags &= 0xFFFFFFFA;
    g_BattleOverlayDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleLimitFlash_ModelTable.drawHandle, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    SetRotMatrix((void *)transform);
    g_BattleLimitFlash_ModelTable.drawFlags |= 1;
    g_BattleOverlayDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleLimitFlash_ModelTable.drawHandle, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    SetRotMatrix((void *)transform);
    g_BattleLimitFlash_ModelTable.drawFlags |= 4;
    g_BattleOverlayDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleLimitFlash_ModelTable.drawHandle, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    SetRotMatrix((void *)transform);
    g_BattleLimitFlash_ModelTable.drawFlags &= 0xFFFFFFFE;
    g_BattleOverlayDrawListHead = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_BattleLimitFlash_ModelTable.drawHandle, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    g_BattleLimitFlash_ModelTable.drawUvScroll = 0;
    g_BattleLimitFlash_ModelTable.drawTexturePageBits = 0;
}

/*
 * Updates growing limit flash model animation and renders passes.
 */
// FUNCTION: FF7 0x005c0572
void __cdecl BattleFx_MagicLimitFlashModel0GrowTask(void)
{
    BattleFxLimitFlashModel0Grow *task;

    task = (BattleFxLimitFlashModel0Grow *)&g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx];
    if ((*(unsigned char *)&g_BattleLimitFlashModel0Started & 1) == 0) {
        g_BattleLimitFlashModel0Started |= 1;
        g_BattleLimitFlashModel0Data = Gfx_GetPsxScratchpadPtr(0);
    }
    g_BattleLimitFlash_SpriteData.matrixM22 = task->frameCount * 1536;
    g_BattleLimitFlash_ModelScales1[0] = g_BattleLimitFlash_SpriteData.matrixM22;
    if (task->frameCount < 8) {
        g_BattleLimitFlash_ModelScale0 = task->frameCount * 3072;
        g_BattleLimitFlash_ModelTable.drawFadeLevel = 0;
    } else if (task->frameCount < 16) {
        g_BattleLimitFlash_ModelScale0 = 0x6000;
        g_BattleLimitFlash_ModelTable.drawFadeLevel = task->frameCount - 8 << 9;
    }
    g_BattleLimitFlash_SpriteData.transX = task->posX;
    g_BattleLimitFlash_SpriteData.transY = 0;
    g_BattleLimitFlash_SpriteData.transZ = task->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, (short *)&g_BattleLimitFlash_ModelScales1, (void *)g_BattleLimitFlashModel0Data);
    g_BattleLimitFlash_ModelTable.drawHandle = Gfx_ModelTableGetRenderObject(0, g_pBattleLimitFlashTable);
    BattleFx_MagicLimitFlashDrawBlackModelPasses(g_BattleLimitFlashModel0Data);
    if (g_BattleIsPaused == 0 && (task->frameCount)++ == 0xF) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
}

/* The tail of the shrink task's rotation matrix (g_BattleLimitFlashModel2Matrix is its first element). */

__declspec(dllimport) void __stdcall OutputDebugStringA(const char *);

/*
 * Updates shrinking limit flash model animation and renders passes.
 */
// FUNCTION: FF7 0x005c06bf
void __cdecl BattleFx_MagicLimitFlashModel2ShrinkTask(void)
{
    BattleFxLimitFlashShrink *task;

    task = (BattleFxLimitFlashShrink *)(&g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx]);
    if ((*(unsigned char *)&g_BattleLimitFlashModel2Started & 1) == 0) {
        *(unsigned char *)&g_BattleLimitFlashModel2Started |= 1;
        g_BattleLimitFlashModel2Data = Gfx_GetPsxScratchpadPtr(0);
    }
    if (DEBUG_TRACE) {
        // STRING: FF7 0x008fe390
        OutputDebugStringA("BLUE \n");
    }
    g_BattleLimitFlashModel2Scale = SinFixed((0xE - task->shrinkStep) << 7) + 0x1000;
    g_BattleLimitFlashModel2MatrixTail.matrixM22 = (0xE - task->shrinkStep) << 10;
    g_BattleLimitFlashModel2Matrix = g_BattleLimitFlashModel2MatrixTail.matrixM22;
    if (task->shrinkStep < 8) {
        g_BattleLimitFlash_ModelTable.drawFadeLevel = ((task->shrinkStep * -4096) / 8) + 4096;
    } else {
        g_BattleLimitFlash_ModelTable.drawFadeLevel = ((task->shrinkStep - 8) << 12) / 8;
    }
    g_BattleLimitFlashModel2MatrixTail.transX = task->shrinkState;
    g_BattleLimitFlashModel2MatrixTail.transY = 0;
    g_BattleLimitFlashModel2MatrixTail.transZ = task->shrinkValue;
    MultiplyMatrix(&g_BattleCameraTransform, &g_BattleLimitFlashModel2Matrix, (void *)g_BattleLimitFlashModel2Data);
    g_BattleLimitFlash_ModelTable.drawHandle = Gfx_ModelTableGetRenderObject(2, g_pBattleLimitFlashTable);
    g_BattleLimitFlash_ModelTable.drawUvScroll = 0x80;
    g_BattleLimitFlash_ModelTable.drawTexturePageBits = 0x80;
    BattleFx_MagicLimitFlashDrawBlackModelPasses(g_BattleLimitFlashModel2Data);
    if (g_BattleIsPaused == 0 && (task->shrinkStep)++ == 0xF) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Emitter task generating radial flare sprite particles.
 */
// FUNCTION: FF7 0x005c0850
void __cdecl BattleFx_MagicLimitFlashRadialSpriteEmitterTask(void)
{
    BattleFxLimitFlashRadialEmitter *task;
    int angle;
    BattleFxRadialFlashSprite *pSprite;
    int spriteOffset;

    task = (BattleFxLimitFlashRadialEmitter *)&g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx];
    if (g_BattleIsPaused != 0) {
        return;
    }
    spriteOffset = 1;
    pSprite = (BattleFxRadialFlashSprite *)&g_BattleTaskSlotBuffer.secondarySlots[(short)Battle_AllocateSecondaryResourceSlot(BattleFx_MagicLimitFlashRadialSpriteTask)];
    pSprite->spriteIndex = (task->spriteCounter + spriteOffset) % 5;
    *(Int64Pair *)&pSprite->bonePos = *(Int64Pair *)&task->bonePos;
    angle = rand() & 0xFFF;
    pSprite->offsetX = SinFixed(angle) * 0xC8 >> 12;
    pSprite->offsetY = CosFixed(angle) * 0xC8 >> 12;
    pSprite->rotatedX = pSprite->offsetY;
    pSprite->rotatedY = -pSprite->offsetX;
    (task->spriteCounter)++;
    if (task->spriteCounter > 22) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Updates and renders an individual radial flare sprite particle.
 */
// FUNCTION: FF7 0x005c0953
void __cdecl BattleFx_MagicLimitFlashRadialSpriteTask(void)
{
    VECTOR xPos;
    int screenY;
    BattleFxLimitFlashRadialSprite *taskPtr;
    int progress;
    int sinValue;
    PsxMatrix *matrix;

    taskPtr = (BattleFxLimitFlashRadialSprite *)&g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx];
    g_BattleLimitFlash_SpriteData.spriteRecord = Gfx_ModelTableGetLoadedRecord(3, g_pBattleLimitFlashTable);
    g_BattleLimitFlash_SpriteData.sceneField = Battle3d_GetSceneRenderObject(1, g_pBattle3dScene);
    progress = 0x1000 - (taskPtr->frameCount << 12) / 8;
    sinValue = SinFixed(progress / 2);
    SetRotMatrix(&g_BattleCameraTransform);
    SetTransMatrix(&g_BattleCameraTransform);
    RotTransPoint((SVECTOR *)&taskPtr->bonePos, &xPos.vx, &screenY);
    matrix = (PsxMatrix *)BattleFx_InstallBillboardMatrix(&taskPtr->bonePos, (progress * -0x500 /* 0.3125 */ >> 12) + 0xA00, (0x200 - xPos.vz) * progress >> 12);
    matrix->t[0] += (taskPtr->velocityX * progress >> 12) + (taskPtr->accelX * sinValue >> 11);
    matrix->t[1] += (taskPtr->velocityY * progress >> 12) + (taskPtr->accelY * sinValue >> 11);
    SetTransMatrix(matrix);
    g_BattleLimitFlash_SpriteData.dimension = g_BattleLimitFlash_SpriteDimensions[taskPtr->dimensionIndex];
    g_BattleOverlayDrawListHead = BattleFx_SubmitSpriteDraw(&g_BattleLimitFlash_SpriteData.spriteRecord, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleOverlayDrawListHead);
    if (g_BattleIsPaused == 0) {
        (taskPtr->frameCount)++;
        if (taskPtr->frameCount >= 8) {
            taskPtr->dimensionIndex = 0xFFFF;
        }
    }
}

/*
 * Periodic pulse emitter driving secondary flash model tasks.
 */
// FUNCTION: FF7 0x005c0aff
void __cdecl BattleFx_MagicLimitFlashPulseEmitterTask(void)
{
    BattleFxLimitFlashPulseSlot *task;
    int slotIdx;
    BattleFxLimitFlashPulseSlot *child;

    task = (BattleFxLimitFlashPulseSlot *)(&g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx]);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if ((task->pulseCounter & 3) == 0) {
        slotIdx = (short)Battle_AllocateSecondaryResourceSlot((BattleTaskFn)(task->allocTask));
        child = (BattleFxLimitFlashPulseSlot *)&g_BattleTaskSlotBuffer.secondarySlots[slotIdx];
        *(Int64Pair *)&child->bonePos = *(Int64Pair *)&task->bonePos;
    }
    if ((task->pulseCounter)++ == 0xC) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Spawns a three-frame screen flash effect task.
 */
// FUNCTION: FF7 0x005c0b93
void __cdecl BattleFx_SpawnThreeFrameScreenFlashTask(void)
{
    Battle_AllocateResourceSlot(BattleFx_ThreeFrameScreenFlashTask);
}

/*
 * Drives screen flash ramp up and fade out across three frames.
 */
// FUNCTION: FF7 0x005c0ba5
void __cdecl BattleFx_ThreeFrameScreenFlashTask(void)
{
    BattleTaskSlot *task;

    task = (BattleTaskSlot *)(&g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx]);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (task->frameCounter == 0) {
        BattleFx_StartScreenFlashRamp(1);
    }
    if (task->frameCounter == 2) {
        BattleFx_FadeOutScreenFlashRamp(1);
        task->ownerSlot = BATTLE_TASK_FREE;
    }
    (task->frameCounter)++;
}
} /* extern "C" */
