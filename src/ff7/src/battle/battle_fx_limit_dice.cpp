// src/ff7/src/battle/battle_fx_limit_dice.cpp
// Limit Break [42] (Cait Sith Lv 1): Dice (Japanese: ダイス, dev tag: "limit2/dise" / Dise)
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Dice limit effect: loads its resources, then runs a timeline that spawns tumbling dice
//   which hop and bounce over a number of frames.
/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/enums/sound.h"
#include "ff7/types/battle.h"
#include "ff7/types/battle_limit.h"
#include "ff7/battle_tasks.h"
#include "lib/types/graphics.h"
#include "lib/codegen.h"
#include "lib/types/polygon.h"
#include "ff7/camera_script.h"
#include <stdlib.h>
#include <string.h>
#include "ff7/api/battle.h"
#include "ff7/api/menu.h"
#include "lib/api/sound.h"
#include "lib/api/graphics.h"
extern "C" {

#pragma intrinsic(memset)

// clang-format off
// GLOBAL: FF7 0x007ffc30
int g_BattleLimitDiceResourceDef = 0x001900B4;
int g_BattleLimitDiceResourceDefHeadPtr[1] = {
    (int)((char *)&g_BattleLimitDiceModelDrawRequest + 0x10)
};
int g_BattleLimitDiceResourceDefDataTail[8] = {
    0x0, 0x0, 0x0, 0x1900b1, (int)((char *)&g_BattleLimitDiceModelDrawRequest + 0x1c), 0x0,
    0x0, 0x0
};
// GLOBAL: FF7 0x007ffc58
int g_BattleLimitDiceCollisionDirections[64] = {0, (-4096), 0, 0, 0, 0, (-4096), 0, 4096, 0, 0, 0, (-4096), 0, 0, 0, 0, 0, 4096, 0, 0, 4096, 0, 0, 0, (-4096), 0, 0, 0, 0, (-4096), 0, 4096, 0, 0, 0, (-4096), 0, 0, 0, 0, 0, 4096, 0, 0, 4096, 0, 0, 0, (-4096), 0, 0, 0, 0, (-4096), 0, 4096, 0, 0, 0, (-4096), 0, 0, 0};
// GLOBAL: FF7 0x007ffd58
int g_BattleLimitDiceCollisionRefVector[3] = {0, (-4096), 0};
// GLOBAL: FF7 0x007ffd68
unsigned char g_BattleLimitDiceModelDrawRequest[40] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x64, 0x69, 0x73, 0x65, 0x2e, 0x72, 0x73, 0x64, 0x00, 0x00, 0x00, 0x00, 0x64, 0x69, 0x73, 0x65, 0x2e, 0x72, 0x73, 0x64, 0x00, 0x00, 0x00, 0x00};
// GLOBAL: FF7 0x00ac5ed8
int g_BattleLimitDiceFirstTargetIdx = 0;
unsigned char g_bssPad_00ac5edc[4] = {0};
// GLOBAL: FF7 0x00ac5ee0
BattleLimitDiceDie g_BattleLimitDiceDies[6] = {0};
// GLOBAL: FF7 0x00ac6060
struct BattleFxDicePosRecord g_BattleLimitDicePosRecords[6] = {0};
// GLOBAL: FF7 0x00ac6090
int g_BattleLimitDiceTargetMask = 0;
// GLOBAL: FF7 0x00ac6094
int g_BattleLimitDiceActorIdx = 0;
// GLOBAL: FF7 0x00ac6098
struct GfxTableCursor *g_BattleLimitDiceLoadedResTable = 0;
unsigned char g_bssPad_00ac609c[4] = {0};
// clang-format on

#pragma intrinsic(memcpy)
#pragma function(memcpy)

// Loads the limit2/dise model table once, when the battle scene exists and the table is still empty.
// FUNCTION: FF7 0x0049fa40
void __cdecl BattleFx_LimitLoadResDice(void)
{
    GfxModelTableBank *newSceneField;
    GfxResourceRequest requestSlot;
    int kind;
    char pathText[200];
    GfxResourceRequest *resourcePtrPtr;
    int stepResourceDef;

    if (g_pBattle3dScene != 0 && g_BattleLimitDiceLoadedResTable == 0) {
        kind = 2;
        stepResourceDef = (int)&g_BattleLimitDiceResourceDef;
        newSceneField = (GfxModelTableBank *)(g_pBattle3dScene->modelTableBank);
        resourcePtrPtr = &requestSlot;
        Gfx_InitResourceRequest(resourcePtrPtr);
        resourcePtrPtr->groupTag = 0;
        // STRING: FF7 0x007ffd90
        resourcePtrPtr->resourcePath = BattleFx_BuildResourcePath("limit2/dise", pathText);
        Gfx_LoadModelTableIfAbsent(kind, stepResourceDef, resourcePtrPtr, &g_BattleLimitDiceLoadedResTable, newSceneField);
    }
}

// Entry point of the Dice limit: loads the resources and starts the effect.
// FUNCTION: FF7 0x0049fad0
int __cdecl Battle_LimitFx42_Dice(int targetMask, int actorIdx)
{
    BattleFx_LimitLoadResDice();
    BattleFx_LimitBeginDice(targetMask, actorIdx);
    return 0;
}

// Allocates the draw-list selection task, remembers the first selected target among the low ten mask bits
// and the acting actor, and queues the spawn task.
// FUNCTION: FF7 0x0049faec
void __cdecl BattleFx_LimitBeginDice(int targetMask, int actorIdx)
{
    int i;
    int found;

    *(int *)&g_BattleFxDrawListBufferTaskSlotPtr = (int)BATTLE_NEW_TASK_SLOT(BattleFx_SelectDrawListBuffer);
    g_BattleFxActiveDrawListPtr = Gfx_GetPsxPacketScratchPtr();
    g_BattleLimitDiceTargetMask = targetMask;
    i = 0;
    for (found = 0; i < 10; i++) {
        if ((targetMask & 1 << i) != 0) {
            found++;
            if (found == 1) {
                g_BattleLimitDiceFirstTargetIdx = i;
            }
        }
    }
    g_BattleLimitDiceActorIdx = actorIdx;
    Battle_AllocateResourceSlot(BattleFx_LimitDiceSpawnTimeline);
}

// Retires this task, starts the timeline task and marks all six dice positions as freshly spawned.
// FUNCTION: FF7 0x0049fb87
void __cdecl BattleFx_LimitDiceSpawnTimeline(void)
{
    int i;
    BattleFxDiceTask *task;

    task = (BattleFxDiceTask *)BATTLE_CURRENT_TASK_SLOT;
    task->ownerSlot = BATTLE_TASK_FREE;
    task = (BattleFxDiceTask *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitDiceTimelineTask);
    task->spawnCursor = 0;
    for (i = 0; i < 6; i++) {
        g_BattleLimitDicePosRecords[i].spawned = 1;
    }
}

// Timeline: at frame 9 spawns one tumbling die per occupied square slot at the actor's bone position, with
// random offsets, an initial velocity from a randomly yawed vector and randomized spin rates, and plays the
// roll sound. Ends at frame 60.
// FUNCTION: FF7 0x0049fbf7
void __cdecl BattleFx_LimitDiceTimelineTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[104];
        int rotMatrix;
        struct { unsigned char pad_to_20[0x20]; BattleTaskSlot *timeline; };
        struct { unsigned char pad_to_24[0x24]; BattleFxDiceTumbleState *dieState; };
        struct { unsigned char pad_to_30[0x30]; int rotVecX; };
        struct {
            unsigned char pad[52];
            int value;
        } at52;
        struct { unsigned char pad_to_38[0x38]; int rotVecZ; };
        struct {
            unsigned char pad[60];
            int value;
        } at44;
        struct { unsigned char pad_to_40[0x40]; int randScale; };
        struct { unsigned char pad_to_44[0x44]; int recordIndex; };
        struct { unsigned char pad_to_48[0x48]; int squareBits; };
        struct { unsigned char pad_to_4C[0x4C]; int nibbleIndex; };
        struct { unsigned char pad_to_50[0x50]; SVECTOR angleX; };
        struct { unsigned char pad_to_58[0x58]; int actorYaw; };
        struct { unsigned char pad_to_5C[0x5C]; int nibble; };
        struct { unsigned char pad_to_60[0x60]; int bonePos; };
        struct {
            unsigned char pad[100];
            int value;
        } at4;
    } frame;
#pragma pack(pop)

    if (g_BattleIsPaused != 0) {
        return;
    }
    frame.timeline = BATTLE_CURRENT_TASK_SLOT;
    if (frame.timeline->frameCounter == 9) {
        Battle_GetBoneWorldPos(g_BattleLimitDiceActorIdx, g_BattleActorRecords[g_BattleLimitDiceActorIdx].primaryBone, (short *)&frame.bonePos);
        frame.actorYaw = g_BattleActorRecords[g_BattleLimitDiceActorIdx].yaw;
        frame.squareBits = *(int *)g_BattleScene.squareSlots;
        frame.nibbleIndex = 0;
        for (frame.recordIndex = 0; frame.nibbleIndex < 6; frame.nibbleIndex++) {
            frame.nibble = frame.squareBits & 0xF;
            frame.squareBits >>= 4;
            if (frame.nibble == 0xF) {
                continue;
            }
            frame.dieState = (BattleFxDiceTumbleState *)BATTLE_NEW_TASK_SLOT(BattleFx_LimitDiceTumblingDieTask);
            *(Int64Pair *)&frame.dieState->posX = *(Int64Pair *)(&frame.bonePos);
            frame.dieState->posX += (short)(rand() % 200 - 100);
            frame.dieState->posZ += (short)(rand() % 200 - 100);
            frame.dieState->spawned = 0;
            frame.dieState->scale = 0x5D1;
            frame.dieState->state = BATTLE_DICE_TUMBLE_FALL;
            frame.dieState->recordIndex = *(short *)&frame.recordIndex;
            frame.angleX.vy = frame.actorYaw + rand() % 0x2BC + -0x15E;
            frame.angleX.vz = 0;
            frame.angleX.vx = frame.angleX.vz;
            RotMatrixXYZ(&frame.angleX, &frame.rotMatrix);
            TransformVectorToInt32((PsxMatrix *)&frame.rotMatrix, &g_BattleLimitDiceModelDrawRequest, &frame.rotVecX);
            frame.randScale = rand() % 0x1E + 0x50;
            g_BattleLimitDiceDies[frame.recordIndex].velocityX = frame.randScale * frame.rotVecX >> 12;
            g_BattleLimitDiceDies[frame.recordIndex].velocityY = -(rand() % 90 + 320);
            g_BattleLimitDiceDies[frame.recordIndex].velocityZ = frame.randScale * frame.rotVecZ >> 12;
            g_BattleLimitDiceDies[frame.recordIndex].gravity = rand() % 20 + 80;
            g_BattleLimitDiceDies[frame.recordIndex].spinStep = 0;
            g_BattleLimitDiceDies[frame.recordIndex].spinAngle = 0;
            g_BattleLimitDiceDies[frame.recordIndex].angleZ = 0;
            g_BattleLimitDiceDies[frame.recordIndex].angle = 0;
            g_BattleLimitDiceDies[frame.recordIndex].angleStep = rand() % 0x64 + 0x190;
            g_BattleLimitDiceDies[frame.recordIndex].angleZStep = rand() % 90 + 340;
            *(BattleVec4i *)g_BattleLimitDiceDies[frame.recordIndex].moveDir = *(BattleVec4i *)(&frame.rotVecX);
            g_BattleLimitDiceDies[frame.recordIndex].directionIndex = frame.nibble;
            frame.recordIndex++;
        }
        Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputeScreenPanFromPosition(&frame.bonePos), 0x15);
    }
    if (frame.timeline->frameCounter == 55) {
        Battle_PostActorHitOrHurtMessage(g_BattleLimitDiceFirstTargetIdx);
    }
    if (frame.timeline->frameCounter >= 60) {
        frame.timeline->ownerSlot = BATTLE_TASK_FREE;
    }
    (frame.timeline->frameCounter)++;
}

// One tumbling die: publishes its position, steers away from nearby dice, then runs the hop/roll state
// machine (state 0 falling, 1 hopping, 2 settling onto the face given by directionIndex) and draws the model
// with a fade after frame 42. Ends at frame 50.
// FUNCTION: FF7 0x0049ff2e
void __cdecl BattleFx_LimitDiceTumblingDieTask(void)
{
#pragma pack(push, 1)
    /* Locals by stack slot: see FRAME UNION in lib/codegen.h. */
    union {
        unsigned char storage[144];
        int rotMatrix;
        struct { unsigned char pad_to_12[0x12]; int prevPosX; };
        struct { unsigned char pad_to_16[0x16]; int prevPosY; };
        struct { unsigned char pad_to_1A[0x1A]; int prevPosZ; };
        struct { unsigned char pad_to_20[0x20]; BattleLimitDiceTumble *tumbleRec; };
        struct { unsigned char pad_to_24[0x24]; BattleLimitDiceDie *dieRec; };
        struct { unsigned char pad_to_28[0x28]; int vecX; };
        struct { unsigned char pad_to_2C[0x2C]; int vecY; };
        struct { unsigned char pad_to_30[0x30]; int vecZ; };
        struct {
            unsigned char pad[52];
            int value;
        } at92;
        struct { unsigned char pad_to_38[0x38]; int spinAngle; };
        struct { unsigned char pad_to_3C[0x3C]; int tmpScalar; };
        struct { unsigned char pad_to_40[0x40]; int snapIndex; };
        struct { unsigned char pad_to_44[0x44]; SVECTOR rotX; };
        struct { unsigned char pad_to_4C[0x4C]; PsxMatrix turnMatrix; };
        struct { unsigned char pad_to_6C[0x6C]; VECTOR collisionAxis; };
        struct { unsigned char pad_to_7C[0x7C]; BattleFxModelDraw *draw; };
        struct { unsigned char pad_to_80[0x80]; VECTOR scaleX; };
    } frame;
#pragma pack(pop)

    frame.tumbleRec = (BattleLimitDiceTumble *)BATTLE_CURRENT_TASK_SLOT;
    frame.dieRec = &g_BattleLimitDiceDies[frame.tumbleRec->recordIndex];
    *(Int64Pair *)&g_BattleLimitDicePosRecords[frame.tumbleRec->recordIndex] = *(Int64Pair *)&frame.tumbleRec->posX;
    if (frame.tumbleRec->timer > 7) {
        for (frame.snapIndex = 0; frame.snapIndex < 6; frame.snapIndex++) {
            if (g_BattleLimitDicePosRecords[frame.snapIndex].spawned != 0 || frame.snapIndex == frame.tumbleRec->recordIndex) {
                continue;
            }
            frame.vecX = g_BattleLimitDicePosRecords[frame.snapIndex].posX - frame.tumbleRec->posX;
            frame.vecY = g_BattleLimitDicePosRecords[frame.snapIndex].posY - frame.tumbleRec->posY;
            frame.vecZ = g_BattleLimitDicePosRecords[frame.snapIndex].posZ - frame.tumbleRec->posZ;
            frame.tmpScalar = NormalizeVectorFixed(&frame.vecX, &frame.vecX);
            frame.spinAngle = SqrtToInt(frame.tmpScalar);
            if (frame.spinAngle > 0x96) {
                continue;
            }
            frame.vecY = 0;
            NormalizeVectorFixed(&frame.vecX, &frame.vecX);
            frame.tmpScalar = ComputeAngleBetweenVectors((VECTOR *)frame.dieRec->moveDir, (VECTOR *)&frame.vecX, &frame.collisionAxis);
            frame.spinAngle = -(rand() % 0xC8 + 0x190);
            Gte_BuildAxisAngleMatrix(frame.spinAngle, &frame.turnMatrix, &frame.collisionAxis);
            frame.vecX = frame.dieRec->velocityX;
            frame.vecY = 0;
            frame.vecZ = frame.dieRec->velocityZ;
            TransformVectorToInt32(&frame.turnMatrix, &frame.vecX, &frame.vecX);
            frame.dieRec->velocityX = *(short *)&frame.vecX;
            frame.dieRec->velocityZ = *(short *)&frame.vecZ;
            NormalizeVectorFixed(&frame.vecX, &frame.vecX);
            *(BattleVec4i *)frame.dieRec->moveDir = *(BattleVec4i *)(&frame.vecX);
            frame.tumbleRec->posX += (short)(frame.dieRec->velocityX + (frame.dieRec->velocityX >> 1));
            frame.tumbleRec->posZ += (short)(frame.dieRec->velocityZ + (frame.dieRec->velocityZ >> 1));
        }
    }
    frame.rotX.vx = frame.dieRec->angle;
    frame.rotX.vy = 0;
    frame.rotX.vz = frame.dieRec->angleZ;
    RotMatrixXYZ(&frame.rotX, &frame.rotMatrix);
    frame.prevPosX = frame.tumbleRec->posX;
    frame.prevPosY = frame.tumbleRec->posY;
    frame.prevPosZ = frame.tumbleRec->posZ;
    switch (frame.tumbleRec->state) {
    case BATTLE_DICE_TUMBLE_FALL:
        if (g_BattleIsPaused != 0) {
            break;
        }
        if (frame.tumbleRec->posY < -100) {
            frame.tumbleRec->posX += frame.dieRec->velocityX;
            frame.tumbleRec->posY += frame.dieRec->velocityY;
            frame.tumbleRec->posZ += frame.dieRec->velocityZ;
            frame.dieRec->velocityY += frame.dieRec->gravity;
            frame.dieRec->gravity -= (short)(frame.dieRec->gravity >> 4);
            frame.dieRec->angle += frame.dieRec->angleStep;
            frame.dieRec->angleZ += frame.dieRec->angleZStep;
            if (frame.tumbleRec->posY > -100) {
                frame.tumbleRec->posY = 0xFF9C;
            }
        } else {
            TransformVectorToInt32((PsxMatrix *)&frame.rotMatrix, (void *)((frame.dieRec->directionIndex << 4) + (char *)&g_BattleLimitDiceCollisionDirections), &frame.vecX);
            frame.snapIndex = ComputeAngleBetweenVectors((VECTOR *)&frame.vecX, (VECTOR *)&g_BattleLimitDiceCollisionRefVector, (VECTOR *)frame.dieRec->turnAxis);
            if (frame.snapIndex > 512) {
                frame.tumbleRec->state = BATTLE_DICE_TUMBLE_HOP;
                if ((frame.snapIndex & 1) != 0) {
                    frame.spinAngle = rand() % 0x1000;
                    frame.rotX.vx = 0;
                    frame.rotX.vy = *(short *)&frame.spinAngle;
                    frame.rotX.vz = 0;
                    RotMatrixXYZ(&frame.rotX, &frame.turnMatrix);
                    TransformVectorToInt16(&frame.turnMatrix, (void *)frame.dieRec, (void *)frame.dieRec);
                    frame.dieRec->velocityX = frame.dieRec->velocityX / 3 << 1;
                    frame.dieRec->velocityZ = frame.dieRec->velocityZ / 3 << 1;
                    frame.dieRec->velocityY = -(rand() % 40 + 100);
                    frame.dieRec->gravity = rand() % 16 + 32;
                    frame.tmpScalar = BattleFx_LimitDiceCountHopFrames(0, frame.dieRec->velocityY, frame.dieRec->gravity, 0);
                    frame.dieRec->spinAngle = 0;
                    if (frame.snapIndex > 1024) {
                        frame.snapIndex = 1024;
                    }
                    frame.dieRec->spinStep = (frame.snapIndex + 0x1000) / frame.tmpScalar;
                    frame.dieRec->spinFramesLeft = frame.tmpScalar;
                    frame.tumbleRec->posY += frame.dieRec->velocityY;
                    frame.dieRec->velocityY += frame.dieRec->gravity;
                    frame.dieRec->gravity -= (short)(frame.dieRec->gravity >> 4);
                } else {
                    frame.spinAngle = rand() % 0x258 - 0x12C;
                    frame.rotX.vx = 0;
                    frame.rotX.vy = *(short *)&frame.spinAngle;
                    frame.rotX.vz = 0;
                    RotMatrixXYZ(&frame.rotX, &frame.turnMatrix);
                    TransformVectorToInt16(&frame.turnMatrix, (void *)frame.dieRec, (void *)frame.dieRec);
                    frame.dieRec->velocityX = frame.dieRec->velocityX / 3 << 1;
                    frame.dieRec->velocityZ = frame.dieRec->velocityZ / 3 << 1;
                    frame.tumbleRec->posY = 0xFF9B;
                    frame.dieRec->gravity = 0;
                    frame.dieRec->velocityY = 0;
                    frame.tmpScalar = rand() % 4 + 6;
                    frame.dieRec->spinAngle = 0;
                    frame.dieRec->spinStep = (frame.snapIndex + 0x2000) / frame.tmpScalar;
                    frame.dieRec->spinFramesLeft = frame.tmpScalar;
                    frame.tumbleRec->posX += frame.dieRec->velocityX;
                    frame.tumbleRec->posZ += frame.dieRec->velocityZ;
                    Battle_DispatchSfxOpcodeDirect(SOUND_SFX_PLAY_1, Battle_ComputeScreenPanFromPosition(&frame.tumbleRec->posX), 0x226);
                }
            } else {
                frame.tumbleRec->state = BATTLE_DICE_TUMBLE_SETTLE;
                frame.dieRec->spinAngle = *(short *)&frame.snapIndex;
                frame.dieRec->spinStep = 0;
                frame.dieRec->spinFramesLeft = 1;
            }
        }
        break;
    case BATTLE_DICE_TUMBLE_HOP:
        if (g_BattleIsPaused == 0) {
            if (frame.tumbleRec->posY < -100 && frame.dieRec->spinFramesLeft > 0) {
                frame.tumbleRec->posX += frame.dieRec->velocityX;
                frame.tumbleRec->posY += frame.dieRec->velocityY;
                frame.tumbleRec->posZ += frame.dieRec->velocityZ;
                frame.dieRec->velocityY += frame.dieRec->gravity;
                frame.dieRec->gravity -= (short)(frame.dieRec->gravity >> 4);
                if (frame.tumbleRec->posY > -100) {
                    frame.tumbleRec->posY = 0xFF9C;
                }
            } else {
                frame.tumbleRec->state = BATTLE_DICE_TUMBLE_SETTLE;
            }
        }
    case BATTLE_DICE_TUMBLE_SETTLE:
        Gte_BuildAxisAngleMatrix(frame.dieRec->spinAngle, &frame.turnMatrix, (VECTOR *)frame.dieRec->turnAxis);
        ConcatMatrixInPlace(&frame.turnMatrix, &frame.rotMatrix);
        if (frame.dieRec->spinFramesLeft > 0) {
            frame.dieRec->spinAngle += frame.dieRec->spinStep;
            (frame.dieRec->spinFramesLeft)--;
        } else {
            TransformVectorToInt32((PsxMatrix *)&frame.rotMatrix, (void *)((frame.dieRec->directionIndex << 4) + (char *)&g_BattleLimitDiceCollisionDirections), &frame.vecX);
            frame.snapIndex = ComputeAngleBetweenVectors((VECTOR *)&frame.vecX, (VECTOR *)&g_BattleLimitDiceCollisionRefVector, &frame.collisionAxis);
            if (frame.snapIndex > 512) {
                frame.spinAngle = rand() % 0x1000;
                frame.rotX.vx = 0;
                frame.rotX.vy = *(short *)&frame.spinAngle;
                frame.rotX.vz = 0;
                RotMatrixXYZ(&frame.rotX, &frame.turnMatrix);
                TransformVectorToInt16(&frame.turnMatrix, (void *)frame.dieRec, (void *)frame.dieRec);
                frame.tumbleRec->state = BATTLE_DICE_TUMBLE_HOP;
                frame.dieRec->velocityX = frame.dieRec->velocityX / 3 << 1;
                frame.dieRec->velocityZ = frame.dieRec->velocityZ / 3 << 1;
                frame.dieRec->velocityY = -(rand() % 30 + 80);
                frame.dieRec->gravity = rand() % 10 + 26;
                frame.tmpScalar = BattleFx_LimitDiceCountHopFrames(0, frame.dieRec->velocityY, frame.dieRec->gravity, 0);
                frame.dieRec->spinStep = frame.snapIndex / frame.tmpScalar;
                frame.dieRec->spinFramesLeft = frame.tmpScalar;
                frame.tumbleRec->posY += frame.dieRec->velocityY;
                frame.dieRec->velocityY += frame.dieRec->gravity;
                frame.dieRec->gravity -= (short)(frame.dieRec->gravity >> 4);
            } else {
                frame.dieRec->spinAngle += *(short *)&frame.snapIndex;
                frame.dieRec->spinStep = 0;
                frame.dieRec->spinFramesLeft = 1;
                frame.tumbleRec->posY = 0xFF9C;
            }
        }
    }
    frame.scaleX.vz = frame.tumbleRec->modelScale;
    frame.scaleX.vy = frame.scaleX.vz;
    frame.scaleX.vx = frame.scaleX.vy;
    ScaleMatrix(&frame.rotMatrix, &frame.scaleX);
    MultiplyMatrix(&g_BattleCameraTransform, &frame.rotMatrix, &frame.rotMatrix);
    SetRotMatrix(&frame.rotMatrix);
    SetTransMatrix(&frame.rotMatrix);
    frame.draw = (BattleFxModelDraw *)Gfx_GetPsxScratchpadPtr(0);
    frame.draw->modelGroup = Gfx_ModelTableGetRenderObject(0, g_BattleLimitDiceLoadedResTable);
    frame.draw->flags = 0;
    frame.draw->uvScroll = 0;
    frame.draw->fadeLevel = 128;
    frame.draw->stateFlags = 0x20;
    frame.draw->texturePageBits = 0;
    if (frame.tumbleRec->timer >= 42) {
        frame.draw->flags |= 8;
        frame.draw->fadeLevel -= (short)(frame.tumbleRec->timer - 42 << 4);
        frame.draw->modelGroup = Gfx_ModelTableGetRenderObject(1, g_BattleLimitDiceLoadedResTable);
    }
    if (frame.dieRec->directionIndex > 6) {
        frame.draw->fadeLevel = 0;
    }
    SetDrawColorRGB(0, 0, 0);
    g_BattleFxActiveDrawListPtr = BattleFx_SubmitModelDraw(frame.draw, g_BattleCurrentOrderingTable + BATTLE_OT_DEPTH_SORTED, 0xC, g_BattleFxActiveDrawListPtr);
    if (frame.tumbleRec->timer >= 50) {
        frame.tumbleRec->ownerSlot = BATTLE_TASK_FREE;
    }
    if (g_BattleIsPaused != 0) {
        return;
    }
    (frame.tumbleRec->timer)++;
}

// Counts the frames a hop lasts: steps position by velocity and velocity by accel (accel damped by a sixteenth
// each step) until position reaches target while falling, at most 99 further steps.
// FUNCTION: FF7 0x004a09d9
int __cdecl BattleFx_LimitDiceCountHopFrames(int position, int velocity, int accel, int target)
{
    int i;

    position += velocity;
    velocity += accel;
    for (i = 0; i < 99; i++) {
        position += velocity;
        velocity += accel;
        accel -= accel >> 4;
        if (position >= target && velocity > 0) {
            break;
        }
    }
    return i;
}
} /* extern "C" */
