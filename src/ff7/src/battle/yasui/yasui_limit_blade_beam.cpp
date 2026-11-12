// src/ff7/src/battle/yasui/yasui_limit_climhazzard.cpp
// Limit Break [2, 79] (Cloud Lv 2-1): Blade Beam (Japanese: 破晄撃, dev tag: "LIMIT/HAKOU" / Hakou)
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Battle effects for Cloud's Hakou (Blade Beam) limit break: the traveling energy blade
//   with its trailing sprites, the impact particles and sound, and a secondary phase that strikes
//   additional enemies.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/enums/sound.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_boss.h"
#include "ff7/battle_tasks.h"
#include "ff7/camera_script.h"
#include "lib/types/graphics.h"
#include <string.h>
#include <stdlib.h>
#include "lib/codegen.h"
#include "ff7/api/battle.h"
#include "lib/api/graphics.h"
#include "ff7/api/menu.h"
extern "C" {

struct BattleOrthoMatrix;
struct GfxRotationAngles2;
struct BattleFxModelDraw;

#pragma intrinsic(memcpy)
#pragma intrinsic(memset)
#pragma intrinsic(abs)

// clang-format off

// GLOBAL: FF7 0x007edde0
unsigned int g_YasuiLimitBladeBeamResourceRows[20] = {
    0x002800b1,
    (unsigned int)&g_YasuiLimitBladeBeamCutterRsdFilename,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x007edeb4
    (unsigned int)"HAKOU.TIM",
    // STRING: FF7 0x007edec0
    (unsigned int)"LINE.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x007edecc
    (unsigned int)"HAKOU.TIM",
    // STRING: FF7 0x007eded8
    (unsigned int)"EXPLD.SPT",
    0x00000000,
    0x00000000,
    0x00192a39,
    // STRING: FF7 0x007edee4
    (unsigned int)"HAKOU.TIM",
    // STRING: FF7 0x007edef0
    (unsigned int)"HITMARK.SPT",
    0x00000000,
    0x00000000};

// GLOBAL: FF7 0x007ede30
struct BattleLimitStardustRayModelReq2 g_YasuiLimitBladeBeamSecondarySpriteReqA = {0, 0, {0x80, 0x80, 0x80, 0x2e}, 0, {0}};

// GLOBAL: FF7 0x007ede40
struct BattleLimitStardustRayModelReq2 g_YasuiLimitBladeBeamSecondarySpriteReqB = {0, 0, {0x80, 0x80, 0x80, 0x2e}, 0, {0}};

// GLOBAL: FF7 0x007ede50
struct GfxRotationAngles2 g_YasuiLimitBladeBeamSecondaryRotationAngles = {0, 1024, 0};

// GLOBAL: FF7 0x007ede58
struct BattleLimitFuryBrandModelReqA g_YasuiLimitBladeBeamModelDrawReq = {0, {0xa8, 0, 0, 0, 0, 0}, 0, {0x20, 0, 0, 0}};

// GLOBAL: FF7 0x007ede68
struct BattleLimitStardustRayModelReq3 g_YasuiLimitBladeBeamFadeSpriteReq = {0, 0, -128, -128, -128, {0x2e, 0, 0, 0, 0}};

// GLOBAL: FF7 0x007ede78
struct BattleLimitHowlingMoonSpriteRequest g_YasuiLimitBladeBeamParticleSpriteReq = {0, 0, -128, -128, -128, {0x2e}, 0, {0}};

unsigned char g_rawData_007ede88[32] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x2e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00,
    0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

// GLOBAL: FF7 0x007edea8
unsigned char g_YasuiLimitBladeBeamCutterRsdFilename[11] = "CUTTER.RSD";

// GLOBAL: FF7 0x00a53c70
int g_YasuiLimitBladeBeamTargetActorPos[2] = {0};

// GLOBAL: FF7 0x00a53c78
struct Int64Pair g_YasuiLimitBladeBeamSourceActorPos = {0};

// GLOBAL: FF7 0x00a53c80
BattleFxBladeBeamParticle g_YasuiLimitBladeBeamParticleRecords[100] = {0};

// GLOBAL: FF7 0x00a54450
int g_YasuiLimitBladeBeamDrawHandle = 0;

// GLOBAL: FF7 0x00a54454
int g_YasuiLimitBladeBeamTargetBitmask = 0;

// GLOBAL: FF7 0x00a54458
int g_YasuiLimitBladeBeamMatrixCursor = 0;

unsigned char g_bssPad_00a5445c[4] = {0};

// GLOBAL: FF7 0x00a54460
struct YasuiLimitBladeBeamTargetBonePos g_YasuiLimitBladeBeamTargetBonePos = {0};

// GLOBAL: FF7 0x00a54464
short g_YasuiLimitBladeBeamTargetBonePosZ = 0;

unsigned char g_bssPad_00a54468[4] = {0};

// GLOBAL: FF7 0x00a5446c
int g_YasuiLimitBladeBeamIsPrimaryPhase = 0;

// GLOBAL: FF7 0x00a54470
int g_YasuiLimitBladeBeamMatrixBuffer = 0;

unsigned char g_bssPad_00a54474[4] = {0};

unsigned char g_bssPad_00a54478[236] = {0};

// GLOBAL: FF7 0x00a54564
struct GfxTableCursor *g_YasuiLimitBladeBeamLoadedResTable = 0;

// GLOBAL: FF7 0x00a54568
unsigned char g_YasuiLimitBladeBeamFirstTargetActor[4] = {0};

unsigned char g_bssPad_00a5456c[4] = {0};

// GLOBAL: FF7 0x00a54570
int g_YasuiLimitBladeBeamAltCompleteFlag = 0;

unsigned char g_bssPad_00a54574[4] = {0};
// clang-format on


/*
 * Loads resource files for Cloud's Hakou (Blade Beam) limit break effect.
 */
// FUNCTION: FF7 0x0046e7f0
void __cdecl BattleFx_LimitLoadResBladeBeam(void)
{
    char pathScratch[200];
    GfxModelTableBank *context;
    GfxResourceRequest requestRecord;
    int kind;
    GfxResourceRequest *pReq;
    int resourceRows;

    if (g_pBattle3dScene != 0 && g_YasuiLimitBladeBeamLoadedResTable == 0) {
        kind = 4;
        resourceRows = (int)&g_YasuiLimitBladeBeamResourceRows;
        context = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        pReq = &requestRecord;
        Gfx_InitResourceRequest(pReq);
        pReq->groupTag = 0;
        // STRING: FF7 0x007edefc
        pReq->resourcePath = BattleFx_BuildResourcePath("LIMIT/HAKOU", pathScratch);
        Gfx_LoadModelTableIfAbsent(kind, resourceRows, pReq, &g_YasuiLimitBladeBeamLoadedResTable, context);
    }
}

/*
 * Initializes and begins the primary Hakou limit break effect for the target.
 */
// FUNCTION: FF7 0x0046e880
int __cdecl BattleFx_LimitEnterBladeBeam(int targetMask, int actorIdx)
{
    BattleFx_LimitLoadResBladeBeam();
    BattleFx_LimitBeginBladeBeam(targetMask, actorIdx);
    return 0;
}

/*
 * Initializes and begins the secondary Hakou limit break effect.
 */
// FUNCTION: FF7 0x0046e89c
int __cdecl BattleFx_LimitEnterBladeBeamAlt(int targetMask, int actorIdx)
{
    BattleFx_LimitLoadResBladeBeam();
    BattleFx_LimitBeginBladeBeamAlt(targetMask, actorIdx);
    return 0;
}

/*
 * Sets up initial Hakou state and allocates the timeline spawn task.
 */
// FUNCTION: FF7 0x0046e8b8
void __cdecl BattleFx_LimitBeginBladeBeam(int targetMask, int casterIdx)
{
    int actorIdx;
    int unused; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxBladeBeamSlot *slot;

    g_YasuiLimitBladeBeamAltCompleteFlag = 0;
    g_YasuiLimitBladeBeamIsPrimaryPhase = 1;
    for (actorIdx = 0; actorIdx < 10; actorIdx++) {
        if ((targetMask & 1 << actorIdx) != 0) {
            break;
        }
    }
    *(int *)&g_YasuiLimitBladeBeamFirstTargetActor = actorIdx;
    slot = (BattleFxBladeBeamSlot *)(&g_BattleTaskSlotBuffer.secondarySlots[(short)Battle_AllocateSecondaryResourceSlot(BattleFx_LimitBladeBeamSpawnTimeline)]);
    slot->firstTargetActor = actorIdx;
    slot->sourceActor = casterIdx;
    g_YasuiLimitBladeBeamMatrixCursor = (int)&g_YasuiLimitBladeBeamMatrixBuffer;
}

/*
 * Waits for initial delay then transitions to the main Hakou timeline task.
 */
// FUNCTION: FF7 0x0046e946
void __cdecl BattleFx_LimitBladeBeamSpawnTimeline(void)
{
    BattleFxLimitBladeBeamSpawnSlot *slot;
    BattleFxBladeBeamTimelineSlot *timeline;

    slot = (BattleFxLimitBladeBeamSpawnSlot *)&g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx];
    if (g_BattleIsPaused != 0) {
        return;
    }
    (slot->timer)++;
    if (slot->timer == 30) {
        timeline = (BattleFxBladeBeamTimelineSlot *)(&g_BattleTaskSlotBuffer.secondarySlots[(short)Battle_AllocateSecondaryResourceSlot(BattleFx_LimitBladeBeamTimelineTask)]);
        timeline->firstTargetActor = slot->firstTargetActor;
        timeline->actorIndex = slot->actorIndex;
        slot->ownerSlot = BATTLE_TASK_FREE;
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputeScreenPanFromPosition(&g_BattleActorRecords[slot->actorIndex].posX), 0x173);
    }
}

/*
 * Main timeline task driving the Hakou wave beam, impact particles, and sound.
 */
// FUNCTION: FF7 0x0046e9f9
void __cdecl BattleFx_LimitBladeBeamTimelineTask(void)
{
    struct {
        BattleFxLimitBladeBeamBone *bone;
        int *rotMatrix;
        BattleFxBladeBeamSpawnRotation *rotation;
        int i;
        BattleFxBladeBeamRecord *vec;
        short dir[3];
        unsigned char pad_1A[0x2];
        BattleFxBladeBeamTimeline *timeline;
        unsigned char pad_20[0x8];
        BattleFxBladeBeamParticle *particle;
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    frame.timeline = (BattleFxBladeBeamTimeline *)&g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx];
    frame.vec = (BattleFxBladeBeamRecord *)Gfx_GetPsxScratchpadPtr(0);
    frame.rotation = (BattleFxBladeBeamSpawnRotation *)((int)frame.vec + 8);
    frame.rotMatrix = (int *)((int)frame.rotation + 8);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.timeline->stage == 0) {
        frame.bone = (BattleFxLimitBladeBeamBone *)&g_BattleTaskSlotBuffer.secondarySlots[(short)Battle_AllocateSecondaryResourceSlot(BattleFx_LimitBladeBeamTravelingBeamModelTask)];
        Battle_GetBoneWorldPos(frame.timeline->sourceActor, g_BattleActorRecords[frame.timeline->sourceActor].targetBone, &frame.bone->posX);
        Battle_GetBoneWorldPos(frame.timeline->firstTargetActor, g_BattleActorRecords[frame.timeline->firstTargetActor].targetBone, (short *)&g_YasuiLimitBladeBeamTargetActorPos);
        *&g_YasuiLimitBladeBeamSourceActorPos = *(Int64Pair *)&g_YasuiLimitBladeBeamTargetActorPos;
        frame.bone->velocityX = (*(short *)&g_YasuiLimitBladeBeamTargetActorPos - frame.bone->posX) / 8;
        frame.bone->velocityY = (*(short *)((char *)&g_YasuiLimitBladeBeamTargetActorPos + 2) - frame.bone->posY) / 8;
        frame.bone->velocityZ = (*(short *)&g_YasuiLimitBladeBeamTargetActorPos[1] - frame.bone->posZ) / 8;
        frame.dir[0] = -frame.bone->velocityX;
        frame.dir[1] = -frame.bone->velocityY;
        frame.dir[2] = -frame.bone->velocityZ;
        Gte_BuildLookBasisFromDirection(frame.dir, (short *)g_YasuiLimitBladeBeamMatrixCursor);
        frame.bone->matrixCursor = g_YasuiLimitBladeBeamMatrixCursor;
        g_YasuiLimitBladeBeamMatrixCursor += 0x1E;
    }
    if (frame.timeline->stage == 8) {
        if (g_BattleLimitBladeBeamTargetDiffers == 0) {
            for (frame.i = 0; frame.i < 50; frame.i++) {
                frame.particle = &g_YasuiLimitBladeBeamParticleRecords[frame.i];
                frame.particle->frameIndex = (rand() & 1) + 1;
                frame.particle->age = rand() % 4096 + 8192;
                *(Int64Pair *)&frame.particle->posX = *(Int64Pair *)&g_YasuiLimitBladeBeamTargetActorPos;
                frame.vec->x = 0;
                frame.vec->y = 0;
                frame.vec->lifetime = rand() % 600 + 200;
                frame.rotation->rotX = rand() % 0x900 - 0x80;
                frame.rotation->rotY = rand() & 0xFFF;
                frame.rotation->rotZ = 0;
                RotMatrix((SVECTOR *)frame.rotation, frame.rotMatrix);
                TransformVectorToInt16((void *)frame.rotMatrix, frame.vec, &frame.particle->velocityX);
            }
            Battle_AllocateSecondaryResourceSlot(BattleFx_LimitBladeBeamDampedSpriteParticlePool50Task);
            frame.bone = (BattleFxLimitBladeBeamBone *)&g_BattleTaskSlotBuffer.secondarySlots[(short)Battle_AllocateSecondaryResourceSlot(BattleFx_LimitBladeBeamFadingSpriteEmitterTask)];
            Battle_GetBoneWorldPos(frame.timeline->firstTargetActor, g_BattleActorRecords[frame.timeline->firstTargetActor].targetBone, &frame.bone->posX);
        } else {
            for (frame.i = 0; frame.i < 100; frame.i++) {
                frame.particle = &g_YasuiLimitBladeBeamParticleRecords[frame.i];
                *(Int64Pair *)&frame.particle->posX = *(Int64Pair *)&g_YasuiLimitBladeBeamTargetActorPos;
                frame.particle->frameIndex = 0;
                frame.particle->age = 0;
                frame.vec->x = 0;
                frame.vec->y = 0;
                frame.vec->lifetime = rand() % 600 + 200;
                frame.rotation->rotX = rand() % 0x900 - 0x80;
                frame.rotation->rotY = rand() & 0xFFF;
                frame.rotation->rotZ = 0;
                RotMatrix((SVECTOR *)frame.rotation, frame.rotMatrix);
                TransformVectorToInt16((void *)frame.rotMatrix, frame.vec, &frame.particle->velocityX);
            }
            Battle_AllocateSecondaryResourceSlot(BattleFx_LimitBladeBeamTrailParticlePool100Task);
            Battle_GetBoneWorldPos(frame.timeline->firstTargetActor, g_BattleActorRecords[frame.timeline->firstTargetActor].targetBone, (short *)frame.vec);
            frame.bone = (BattleFxLimitBladeBeamBone *)&g_BattleTaskSlotBuffer.secondarySlots[(short)Battle_AllocateSecondaryResourceSlot(BattleFx_LimitBladeBeamFadingSpriteEmitterTask)];
            *(Int64Pair *)&frame.bone->posX = *(Int64Pair *)frame.vec;
        }
    }
    if (frame.timeline->stage == 8) {
        Battle_PostActorHitOrHurtMessage(frame.timeline->firstTargetActor);
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputeScreenPanFromPosition(&g_BattleActorRecords[frame.timeline->firstTargetActor].posX), 0x1B);
    }
    if (frame.timeline->stage == 20) {
        frame.timeline->ownerSlot = BATTLE_TASK_FREE;
    }
    (frame.timeline->stage)++;
}

/*
 * Renders and updates the traveling energy blade model and trailing sprites.
 */
// FUNCTION: FF7 0x0046eea6
void __cdecl BattleFx_LimitBladeBeamTravelingBeamModelTask(void)
{
    int alongBeam;
    int scaler;
    int i;
    BladeBeamEffectRecord *pSelf;
    int frame;
    int matrix;
    int heightJitter;
    BattleFxBladeBeamBeamTravel *pSprite;
    int padField[2]; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    int blend;

    pSelf = (BladeBeamEffectRecord *)&g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx];
    (g_YasuiLimitBladeBeamModelDrawReq.modelGroup) = Gfx_ModelTableGetRenderObject(0, g_YasuiLimitBladeBeamLoadedResTable);
    matrix = Gfx_GetPsxScratchpadPtr(0);
    scaler = matrix + 0x1E;
    frame = pSelf->frame;
    if (frame < 8) {
        blend = 0;
    } else {
        frame -= 8;
        if (frame < 4) {
            if (g_YasuiLimitBladeBeamIsPrimaryPhase >= 1) {
                pSelf->active = 0xFFFF;
                return;
            }
            blend = (frame << 12) / 4;
        } else {
            pSelf->active = 0xFFFF;
            return;
        }
    }
    pSelf->matrix->t[0] = (int)pSelf->x;
    pSelf->matrix->t[1] = (int)pSelf->y;
    pSelf->matrix->t[2] = (int)pSelf->z;
    MultiplyMatrix(&g_BattleCameraTransform, pSelf->matrix, (void *)matrix);
    SetRotMatrix((void *)matrix);
    SetTransMatrix((void *)matrix);
    SetDrawColorRGB(0, 0, 0);
    g_YasuiLimitBladeBeamModelDrawReq.fadeLevel = blend / 2 + 2048;
    g_YasuiLimitBladeBeamDrawHandle = BattleFx_SubmitModelDraw((BattleFxModelDraw *)&g_YasuiLimitBladeBeamModelDrawReq, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_YasuiLimitBladeBeamDrawHandle);
    if (g_BattleIsPaused == 0) {
        for (i = 0; i < 2; i++) {
            pSprite = (BattleFxBladeBeamBeamTravel *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitBladeBeamMirroredSpritePairTask);
            pSprite->frameCounter = i;
            pSprite->posX = pSelf->x - pSelf->width * i / 2;
            pSprite->endY = 0;
            pSprite->posY = pSelf->z - pSelf->height * i / 2;
            pSprite->velocityX = -pSelf->width;
            pSprite->dirY = 0;
            pSprite->velocityY = -pSelf->height;
            pSprite->scale = rand() % 0x800 + 0x1000;
        }
        if (pSelf->frame > 0 && g_YasuiLimitBladeBeamIsPrimaryPhase >= 1) {
            for (i = 0; i < 6; i++) {
                pSprite = (BattleFxBladeBeamBeamTravel *)&g_BattleTaskSlotBuffer.secondarySlots[(short)Battle_AllocateSecondaryResourceSlot(BattleFx_LimitBladeBeamOrientedSpriteTask)];
                pSprite->variant = rand() % 3;
                alongBeam = rand() & 0xFFF;
                pSprite->posX = pSelf->x - (pSelf->width * alongBeam >> 12);
                pSprite->posY = pSelf->z - (pSelf->height * alongBeam >> 12);
                heightJitter = rand() % 600 - 300;
                pSprite->endY = pSelf->y + heightJitter;
                pSprite->frameCounter = abs(heightJitter) * 5 / 300;
                pSprite->transform = (int)pSelf->matrix;
            }
        }
        (pSelf->frame)++;
        pSelf->x += pSelf->width;
        pSelf->y += pSelf->stepY;
        pSelf->z += pSelf->height;
    }
}

/*
 * Renders mirrored energy sprites on both sides of the traveling beam.
 */
// FUNCTION: FF7 0x0046f22d
void __cdecl BattleFx_LimitBladeBeamMirroredSpritePairTask(void)
{
    BattleFxLimitSealEvilDelay *scaleVecPtr;
    BattleFxLimitBladeBeamSpritePair *task;

    task = (BattleFxLimitBladeBeamSpritePair *)BATTLE_CURRENT_TASK_SLOT;
    {
        SVECTOR angleA = {0, 0, 0x200};
        {
            SVECTOR angleB = {0, 0, (short)0xFE00};
            {
                BattleFxBladeBeamMirroredSpritePair *pairOffsets8;
                BattleFxBladeBeamSpriteOffsets *loopBasis;
                PsxMatrix *matrixPtr;
                int baseScratchA;
                int x;

                (g_YasuiLimitBladeBeamSecondarySpriteReqA.spriteGroup) = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(2, g_YasuiLimitBladeBeamLoadedResTable);
                g_YasuiLimitBladeBeamSecondarySpriteReqA.drawObj = Gfx_ModelTableGetSpriteObject(2, g_YasuiLimitBladeBeamLoadedResTable);
                loopBasis = (BattleFxBladeBeamSpriteOffsets *)Gfx_GetPsxScratchpadPtr(0);
                matrixPtr = (PsxMatrix *)((int)loopBasis + 0x1E);
                baseScratchA = (int)matrixPtr + 0x1E;
                x = baseScratchA + 30;
                scaleVecPtr = (BattleFxLimitSealEvilDelay *)(x + 0x10);
                pairOffsets8 = (BattleFxBladeBeamMirroredSpritePair *)((int)scaleVecPtr + 0x10);
                scaleVecPtr->nextValue = task->nextValue;
                scaleVecPtr->value = scaleVecPtr->nextValue;
                scaleVecPtr->prevValue = scaleVecPtr->value;
                Gte_BuildLookBasisFromDirection((short *)((int)task + 0x10), (short *)loopBasis);
                pairOffsets8->offsetX = loopBasis->posX >> 6;
                pairOffsets8->clearedMid = 0;
                pairOffsets8->offsetY = loopBasis->posY >> 6;
                RotMatrix((SVECTOR *)&angleA, (int *)matrixPtr);
                ConcatMatrixInPlace(loopBasis, matrixPtr);
                matrixPtr->t[0] = task->centerX + pairOffsets8->offsetY;
                matrixPtr->t[1] = task->centerY;
                matrixPtr->t[2] = task->spriteOffsetX + pairOffsets8->offsetX;
                MultiplyMatrix(&g_BattleCameraTransform, matrixPtr, matrixPtr);
                Gte_OrthonormaliseFromYAxis((BattleOrthoMatrix *)matrixPtr);
                ScaleMatrix(matrixPtr, scaleVecPtr);
                SetRotMatrix((void *)matrixPtr);
                SetTransMatrix(matrixPtr);
                g_YasuiLimitBladeBeamSecondarySpriteReqA.frameIndex = task->frameCounter / 2;
                g_YasuiLimitBladeBeamDrawHandle = BattleFx_SubmitSpriteDraw(&g_YasuiLimitBladeBeamSecondarySpriteReqA, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_YasuiLimitBladeBeamDrawHandle);
                RotMatrix((SVECTOR *)&angleB, (int *)matrixPtr);
                ConcatMatrixInPlace(loopBasis, matrixPtr);
                matrixPtr->t[0] = task->centerX - pairOffsets8->offsetY;
                matrixPtr->t[1] = task->centerY;
                matrixPtr->t[2] = task->spriteOffsetX - pairOffsets8->offsetX;
                MultiplyMatrix(&g_BattleCameraTransform, matrixPtr, matrixPtr);
                Gte_OrthonormaliseFromYAxis((BattleOrthoMatrix *)matrixPtr);
                ScaleMatrix(matrixPtr, scaleVecPtr);
                SetRotMatrix((void *)matrixPtr);
                SetTransMatrix(matrixPtr);
                g_YasuiLimitBladeBeamDrawHandle = BattleFx_SubmitSpriteDraw(&g_YasuiLimitBladeBeamSecondarySpriteReqA, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_YasuiLimitBladeBeamDrawHandle);
                if (g_BattleIsPaused == 0) {
                    task->frameCounter += 2;
                    if (task->frameCounter >= 10) {
                        task->ownerSlot = BATTLE_TASK_FREE;
                    }
                }
            }
        }
    }
}

/*
 * Renders oriented energy spark sprites along the beam trajectory.
 */
// FUNCTION: FF7 0x0046f50e
void __cdecl BattleFx_LimitBladeBeamOrientedSpriteTask(void)
{
    BattleFxBladeBeamSpriteRecord *sprite;
    PsxMatrix *matrix;

    sprite = (BattleFxBladeBeamSpriteRecord *)&g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx];
    (g_YasuiLimitBladeBeamSecondarySpriteReqB.spriteGroup) = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(1, g_YasuiLimitBladeBeamLoadedResTable);
    g_YasuiLimitBladeBeamSecondarySpriteReqB.drawObj = Gfx_ModelTableGetSpriteObject(1, g_YasuiLimitBladeBeamLoadedResTable);
    matrix = (PsxMatrix *)Gfx_GetPsxScratchpadPtr(0);
    RotMatrix((SVECTOR *)&g_YasuiLimitBladeBeamSecondaryRotationAngles, matrix);
    ConcatMatrixInPlace((void *)sprite->matrix, matrix);
    matrix->t[0] = sprite->posX;
    matrix->t[1] = sprite->posY;
    matrix->t[2] = sprite->posZ;
    MultiplyMatrix(&g_BattleCameraTransform, matrix, matrix);
    SetRotMatrix((void *)matrix);
    SetTransMatrix(matrix);
    g_YasuiLimitBladeBeamSecondarySpriteReqB.frameIndex = sprite->frameIndex;
    g_YasuiLimitBladeBeamDrawHandle = BattleFx_SubmitSpriteDraw(&g_YasuiLimitBladeBeamSecondarySpriteReqB, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_YasuiLimitBladeBeamDrawHandle);
    if (g_BattleIsPaused == 0) {
        (sprite->frame)++;
        if (sprite->frame >= 5) {
            sprite->frameIndex = 0xFFFF;
        }
    }
}

/*
 * Updates and renders a pool of 100 motion-trail particles upon impact.
 */
// FUNCTION: FF7 0x0046f63d
void __cdecl BattleFx_LimitBladeBeamTrailParticlePool100Task(void)
{
    BattleTaskSlot *taskPtr;
    unsigned char color[3];
    int i;

    taskPtr = (BattleTaskSlot *)(&g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx]);
    color[0] = color[1] = color[2] = taskPtr->frameCounter * -0xFF / 10 + 0xFF;
    SetRotMatrix(&g_BattleCameraTransform);
    SetTransMatrix(&g_BattleCameraTransform);
    RefreshGlobalTransformState();
    for (i = 0; i < 100; i++) {
        BattleFx_LimitBladeBeamUpdateTrailParticle(&g_YasuiLimitBladeBeamParticleRecords[i], (BattleFxSpriteColor *)color);
    }
    if (g_BattleIsPaused == 0) {
        (taskPtr->frameCounter)++;
        if (taskPtr->frameCounter >= 10) {
            taskPtr->ownerSlot = BATTLE_TASK_FREE;
        }
    }
}

/*
 * Projects and draws a single motion-trail particle polygon.
 */
// FUNCTION: FF7 0x0046f705
void __cdecl BattleFx_LimitBladeBeamUpdateTrailParticle(BattleFxBladeBeamParticle *targetRecord, BattleFxSpriteColor *colorRgb)
{
    int stepIdx;
    int outBits;
    BattleFxBladeBeamRenderBlock *pPacket;
    int screenOut;
    int steps;
    int unused; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */
    BattleFxBladeBeamTrailPoint *pTrailPoint;

    pPacket = (BattleFxBladeBeamRenderBlock *)Gfx_GetPsxPacketScratchPtr();
    pTrailPoint = (BattleFxBladeBeamTrailPoint *)Gfx_GetPsxScratchpadPtr(0);
    pTrailPoint->posX = targetRecord->posX;
    pTrailPoint->posY = targetRecord->posY;
    pTrailPoint->posZ = targetRecord->posZ;
    steps = targetRecord->age;
    if (steps > 3) {
        steps = 3;
    }
    for (stepIdx = 0; stepIdx < steps; stepIdx++) {
        pTrailPoint->posX -= targetRecord->velocityX;
        pTrailPoint->posY -= targetRecord->velocityY;
        pTrailPoint->posZ -= targetRecord->velocityZ;
    }
    Gfx_TransformTriangleReturnDepth((char *)targetRecord + 4, (SVECTOR *)pTrailPoint, (SVECTOR *)pTrailPoint, &screenOut, &outBits);
    if ((outBits & 0x60000) == 0) {
        pPacket->tag = pPacket->tag & 0xFFFFFF | 0x4000000;
        pPacket->flags = 0x50;
        pPacket->flags |= 2;
        pPacket->colorR = colorRgb->colorR;
        pPacket->colorG = colorRgb->colorG;
        pPacket->colorB = colorRgb->colorB;
        pPacket->color1R = 0;
        pPacket->color1G = 0;
        pPacket->color1B = 0;
        Gfx_DrawPsxPrimitive(pPacket);
    }
    if (g_BattleIsPaused == 0) {
        (targetRecord->age)++;
        if (targetRecord->age >= 10) {
            targetRecord->frameIndex = 0xFFFF;
        } else {
            targetRecord->posX += targetRecord->velocityX;
            targetRecord->posY += targetRecord->velocityY;
            targetRecord->posZ += targetRecord->velocityZ;
        }
    }
}

/*
 * Emits fading billboard sprites at the impact point.
 */
// FUNCTION: FF7 0x0046f8c9
void __cdecl BattleFx_LimitBladeBeamFadingSpriteEmitterTask(void)
{
    BattleTaskSlot *task;
    int child;

    task = (BattleTaskSlot *)(&g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx]);
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (task->frameCounter % 2 == 0) {
        child = (int)&g_BattleTaskSlotBuffer.secondarySlots[(short)Battle_AllocateSecondaryResourceSlot(BattleFx_LimitBladeBeamFadingBillboardSpriteTask)];
        *(Int64Pair *)((char *)child + 8) = *(Int64Pair *)((char *)task + 8);
    }
    (task->frameCounter)++;
    if (task->frameCounter >= 6) {
        task->ownerSlot = BATTLE_TASK_FREE;
    }
}

/*
 * Renders and fades out an impact billboard sprite.
 */
// FUNCTION: FF7 0x0046f95d
void __cdecl BattleFx_LimitBladeBeamFadingBillboardSpriteTask(void)
{
    int angle;
    int frame;
    BattleTaskSlot *task;

    task = (BattleTaskSlot *)(&g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx]);
    (g_YasuiLimitBladeBeamFadeSpriteReq.spriteGroup) = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(3, g_YasuiLimitBladeBeamLoadedResTable);
    g_YasuiLimitBladeBeamFadeSpriteReq.drawObj = Gfx_ModelTableGetSpriteObject(3, g_YasuiLimitBladeBeamLoadedResTable);
    angle = (task->frameCounter << 14) / 8;
    frame = task->frameCounter;
    if (frame < 0) {
        g_YasuiLimitBladeBeamFadeSpriteReq.colorB = 0x80;
        g_YasuiLimitBladeBeamFadeSpriteReq.colorG = g_YasuiLimitBladeBeamFadeSpriteReq.colorB;
        g_YasuiLimitBladeBeamFadeSpriteReq.colorR = g_YasuiLimitBladeBeamFadeSpriteReq.colorG;
    } else {
        frame = frame;
        if (frame < 8) {
            g_YasuiLimitBladeBeamFadeSpriteReq.colorB = frame * -0x80 / 8 + 0x80;
            g_YasuiLimitBladeBeamFadeSpriteReq.colorG = g_YasuiLimitBladeBeamFadeSpriteReq.colorB;
            g_YasuiLimitBladeBeamFadeSpriteReq.colorR = g_YasuiLimitBladeBeamFadeSpriteReq.colorG;
        } else {
            task->ownerSlot = BATTLE_TASK_FREE;
            return;
        }
    }
    BattleFx_InstallBillboardMatrix((void *)((int)task + 8), (short)angle, 0);
    g_YasuiLimitBladeBeamDrawHandle = BattleFx_SubmitSpriteDraw(&g_YasuiLimitBladeBeamFadeSpriteReq, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_YasuiLimitBladeBeamDrawHandle);
    if (g_BattleIsPaused == 0) {
        (task->frameCounter)++;
    }
}

/*
 * Updates and renders a pool of 50 damped sprite particles.
 */
// FUNCTION: FF7 0x0046fa89
void __cdecl BattleFx_LimitBladeBeamDampedSpriteParticlePool50Task(void)
{
    ColorLoopLocals locals;

    locals.record = (BattleTaskSlot *)&g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx];
    locals.color[2] = locals.record->frameCounter * -0x80 / 15 + 0x80;
    locals.color[1] = locals.color[2];
    locals.color[0] = locals.color[1];
    SetRotMatrix(&g_BattleCameraTransform);
    SetTransMatrix(&g_BattleCameraTransform);
    for (locals.index = 0; locals.index < 50; locals.index++) {
        BattleFx_LimitBladeBeamUpdateDampedSpriteParticle(&g_YasuiLimitBladeBeamParticleRecords[locals.index], (BattleFxSpriteColor *)locals.color);
    }
    if (g_BattleIsPaused == 0) {
        (locals.record->frameCounter)++;
        if (locals.record->frameCounter >= 15) {
            locals.record->ownerSlot = -1;
        }
    }
}

/*
 * Renders and updates physics for a single damped sprite particle.
 */
// FUNCTION: FF7 0x0046fb49
void __cdecl BattleFx_LimitBladeBeamUpdateDampedSpriteParticle(BattleFxBladeBeamParticle *particleRecord, BattleFxSpriteColor *colorRgb)
{
    (g_YasuiLimitBladeBeamParticleSpriteReq.spriteGroup) = (BattleFxSpriteGroup *)Gfx_ModelTableGetLoadedRecord(3, g_YasuiLimitBladeBeamLoadedResTable);
    g_YasuiLimitBladeBeamParticleSpriteReq.drawObj = Gfx_ModelTableGetSpriteObject(3, g_YasuiLimitBladeBeamLoadedResTable);
    g_YasuiLimitBladeBeamParticleSpriteReq.frame = particleRecord->frameIndex;
    g_YasuiLimitBladeBeamParticleSpriteReq.colorR = colorRgb->colorR;
    g_YasuiLimitBladeBeamParticleSpriteReq.colorG = colorRgb->colorG;
    g_YasuiLimitBladeBeamParticleSpriteReq.colorB = colorRgb->colorB;
    BattleFx_InstallBillboardMatrix((void *)((int)particleRecord + 4), (short)particleRecord->age, 0);
    g_YasuiLimitBladeBeamDrawHandle = BattleFx_SubmitSpriteDraw(&g_YasuiLimitBladeBeamParticleSpriteReq, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_YasuiLimitBladeBeamDrawHandle);
    if (g_BattleIsPaused == 0) {
        particleRecord->posX += particleRecord->velocityX;
        particleRecord->posY += particleRecord->velocityY;
        particleRecord->posZ += particleRecord->velocityZ;
        particleRecord->velocityX -= (short)(particleRecord->velocityX >> 2);
        particleRecord->velocityY -= (short)(particleRecord->velocityY >> 2);
        particleRecord->velocityZ -= (short)(particleRecord->velocityZ >> 2);
    }
}

/*
 * Begins the secondary Hakou phase targeting additional enemies.
 */
// FUNCTION: FF7 0x0046fc84
void __cdecl BattleFx_LimitBeginBladeBeamAlt(int targetMask, int actorIdx)
{
    int padSlot[3]; /* Unreferenced stack padding preserved for VC5 /Od frame size (see lib/codegen.h). */

    if ((targetMask & 1 << actorIdx) != 0) {
        g_YasuiLimitBladeBeamAltCompleteFlag = 1;
    } else {
        g_YasuiLimitBladeBeamIsPrimaryPhase = 0;
        g_YasuiLimitBladeBeamMatrixCursor = (int)&g_YasuiLimitBladeBeamMatrixBuffer;
        Battle_CreateBitPatternCallbackTask(g_YasuiLimitBladeBeamTargetBitmask = targetMask, *(int *)&g_YasuiLimitBladeBeamFirstTargetActor, 0, BattleFx_LimitSpawnBladeBeamAltPerTarget);
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputeScreenPanFromPosition(&g_BattleActorRecords[*(int *)&g_YasuiLimitBladeBeamFirstTargetActor].posX), 0x1A3);
    }
}

/*
 * Spawns a secondary timeline task for an individual target.
 */
// FUNCTION: FF7 0x0046fd11
void __cdecl BattleFx_LimitSpawnBladeBeamAltPerTarget(int actor)
{
    BattleFxBladeBeamAltSlot *task;

    task = (BattleFxBladeBeamAltSlot *)(&g_BattleTaskSlotBuffer.secondarySlots[(short)Battle_AllocateSecondaryResourceSlot(BattleFx_LimitBladeBeamAltTimelineTask)]);
    task->targetMask = actor;
    task->firstTargetActor = g_YasuiLimitBladeBeamFirstTargetActor[0];
}

/*
 * Timeline task directing secondary beam wave toward a target enemy.
 */
// FUNCTION: FF7 0x0046fd49
void __cdecl BattleFx_LimitBladeBeamAltTimelineTask(void)
{
    struct {
        BattleFxLimitBladeBeamBone *bone;
        int matrixBlock;
        int rotAngles;
        unsigned char pad_0C[0x4];
        int dataBlock;
        short dir[3];
        unsigned char pad_1A[0x2];
        BattleFxBladeBeamAltTimelineState *state;
        unsigned char pad_20[0xC];
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    frame.state = (BattleFxBladeBeamAltTimelineState *)&g_BattleTaskSlotBuffer.secondarySlots[g_BattleSecondaryTaskActiveSlotIdx];
    frame.dataBlock = Gfx_GetPsxScratchpadPtr(0);
    frame.rotAngles = frame.dataBlock + 8;
    frame.matrixBlock = frame.rotAngles + 8;
    if (g_BattleIsPaused != 0) {
        return;
    }
    if (frame.state->step == 0) {
        frame.bone = (BattleFxLimitBladeBeamBone *)&g_BattleTaskSlotBuffer.secondarySlots[(short)Battle_AllocateSecondaryResourceSlot(BattleFx_LimitBladeBeamTravelingBeamModelTask)];
        *(Int64Pair *)&frame.bone->posX = *&g_YasuiLimitBladeBeamSourceActorPos;
        Battle_GetBoneWorldPos(frame.state->actorIndex, g_BattleActorRecords[frame.state->actorIndex].targetBone, (short *)&g_YasuiLimitBladeBeamTargetBonePos);
        frame.bone->velocityX = ((g_YasuiLimitBladeBeamTargetBonePos.posX) - frame.bone->posX) / 8;
        frame.bone->velocityY = (g_YasuiLimitBladeBeamTargetBonePos.limitHakouTargetBonePosY - frame.bone->posY) / 8;
        frame.bone->velocityZ = (g_YasuiLimitBladeBeamTargetBonePosZ - frame.bone->posZ) / 8;
        frame.dir[0] = -frame.bone->velocityX;
        frame.dir[1] = -frame.bone->velocityY;
        frame.dir[2] = -frame.bone->velocityZ;
        Gte_BuildLookBasisFromDirection(frame.dir, (short *)g_YasuiLimitBladeBeamMatrixCursor);
        frame.bone->matrixCursor = g_YasuiLimitBladeBeamMatrixCursor;
        g_YasuiLimitBladeBeamMatrixCursor += 0x1E;
    }
    if (frame.state->step == 8) {
        Battle_PostActorHitOrHurtMessage(frame.state->actorIndex);
    }
    if (frame.state->step == 20) {
        g_YasuiLimitBladeBeamAltCompleteFlag = 1;
        frame.state->ownerSlot = BATTLE_TASK_FREE;
    }
    (frame.state->step)++;
}
} /* extern "C" */
